#!/usr/bin/env python3
"""Tests for structural similarity over IR graphs (#28 phase 1).

The DoD asks for "tests on known-similar and known-different IR graph pairs (case
that must match, case that must not)". Those are `test_*_must_match` and
`test_*_must_not_match` below; the rest cover the invariances the fingerprint
claims and the corpus-mode guard.

Split deliberately into two groups:
  - SYNTHETIC pairs: precise, controlled; each isolates one invariance.
  - REAL corpus pairs: stronger, but only where the ground truth is decidable
    (structural identity is decidable; *semantic* similarity is not, and is not
    claimed anywhere here).

Run: python3 python/test_structural_similarity.py
"""

import copy
import json
import os
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "python"))

import structural_similarity as ss  # noqa: E402

CORPUS = REPO / "Corpus" / "corpus.jsonl"


# ---------------------------------------------------------------------------
# Synthetic graph builders
# ---------------------------------------------------------------------------

def _g(entities, attributes=None, relations=None, operations=None):
    return {
        "entities": entities,
        "attributes": attributes or [],
        "relations": relations or [],
        "operations": operations or [],
    }


def _e(kind, **kw):
    return {"kind": kind, **kw}


def _graph_a():
    """A small graph: E(var Eq), E(term), R(var Eq -> term, eq)."""
    return _g(
        entities=[_e("var", name="Eq"), _e("term", index=0)],
        relations=[{"src": _e("var", name="Eq"), "tgt": _e("term", index=0), "op": "eq"}],
    )


def _graph_a_renamed():
    """Same structure as `_graph_a`, differing only in *presentational* identity.

    This is the DoD's "case that must match". The differences are exactly the
    things that carry no content:
      - bound-scope strings and positional indexes (local per graph)
      - entity / relation ordering
      - polarity (dropped by C1 in v2)

    NOTE the deliberate boundary: free-variable *names* (`Eq`, `HMul.hMul`) are
    NOT varied here, because ENCODER_FORMAT.md says they are semantically stable
    content ("these are real vocab"). Changing one is a *semantic* difference and
    must NOT match -- see test_free_variable_name_change_must_not_match.
    """
    return _g(
        entities=[_e("bound", scope="Other/3/q"), _e("term", index=99)],
        relations=[{"src": _e("bound", scope="Other/3/q"), "tgt": _e("term", index=99),
                    "op": "eq", "polarity": "neg"}],
    )


def _graph_a_for_presentational_comparison():
    """`_graph_a` re-expressed so every id is positional/scope-based, not named.

    Needed because `_graph_a` uses a `var` id, whose *name* is content; comparing
    it against a differently-named `var` would be a semantic difference.
    """
    return _g(
        entities=[_e("bound", scope="d/0/x"), _e("term", index=0)],
        relations=[{"src": _e("bound", scope="d/0/x"), "tgt": _e("term", index=0),
                    "op": "eq"}],
    )


def _graph_different_structure():
    """Same entities, different relation operator -> must NOT match."""
    return _g(
        entities=[_e("var", name="Eq"), _e("term", index=0)],
        relations=[{"src": _e("var", name="Eq"), "tgt": _e("term", index=0), "op": "lt"}],
    )


# ---------------------------------------------------------------------------
# The DoD pair: must match
# ---------------------------------------------------------------------------

def test_same_structure_different_names_must_match():
    """Presentational differences only -> must fingerprint identically."""
    a = ss.fingerprint(_graph_a_for_presentational_comparison())
    b = ss.fingerprint(_graph_a_renamed())
    assert ss.structurally_identical(a, b), (
        "graphs differing only presentationally (scope strings, positional "
        f"indexes, ordering, polarity) must match\n  a={dict(a.facts)}\n  b={dict(b.facts)}"
    )
    assert ss.structural_similarity(a, b) == 1.0
    print("PASS: test_same_structure_different_names_must_match")


def test_free_variable_name_change_must_not_match():
    """PIN THE BOUNDARY: a free-variable name is *content*, not presentation.

    ENCODER_FORMAT.md: "`HMul.hMul`, `Eq`, `Semigroup.toMul` etc. are semantically
    stable across the entire Mathlib corpus... These are real vocab." So changing
    `Eq` to `Iff` is a semantic difference and must NOT fingerprint identically.

    This fixture exists so the decision is explicit: without it, a future reader
    could reasonably "fix" the implementation to abstract `var` names away and
    silently destroy the signal that distinguishes otherwise-identical graphs.
    """
    a = ss.fingerprint(_graph_a())            # var: Eq
    b = ss.fingerprint(_g(
        entities=[_e("var", name="Iff"), _e("term", index=0)],
        relations=[{"src": _e("var", name="Iff"), "tgt": _e("term", index=0), "op": "eq"}],
    ))
    assert not ss.structurally_identical(a, b), \
        "free-variable name change must be treated as a semantic difference"
    assert ss.structural_similarity(a, b) < 1.0, \
        "semantic difference must reduce similarity below 1.0"
    print("PASS: test_free_variable_name_change_must_not_match")


def test_positional_index_renumbering_is_invariant():
    """FVAR_0 in one graph has no relation to FVAR_2 in another (GLOSSARY)."""
    a = _g(entities=[_e("bound", scope="d/0/x"), _e("term", index=0)])
    b = _g(entities=[_e("bound", scope="OTHER/7/y"), _e("term", index=42)])
    assert ss.structurally_identical(ss.fingerprint(a), ss.fingerprint(b)), \
        "positional id renumbering changed the fingerprint"
    print("PASS: test_positional_index_renumbering_is_invariant")


def test_ordering_is_invariant():
    a = _graph_a()
    b = copy.deepcopy(a)
    b["entities"] = list(reversed(b["entities"]))
    assert ss.structurally_identical(ss.fingerprint(a), ss.fingerprint(b)), \
        "entity ordering changed the fingerprint"
    print("PASS: test_ordering_is_invariant")


def test_polarity_is_ignored():
    """C1 dropped polarity in v2; it must not create spurious differences."""
    a = _graph_a()
    b = copy.deepcopy(a)
    for e in b["entities"]:
        e["polarity"] = "pos"
    b["relations"][0]["polarity"] = "neg"
    assert ss.structurally_identical(ss.fingerprint(a), ss.fingerprint(b)), \
        "polarity affected the fingerprint (C1 says it carries no signal)"
    print("PASS: test_polarity_is_ignored")


# ---------------------------------------------------------------------------
# The DoD pair: must NOT match
# ---------------------------------------------------------------------------

def test_different_structure_must_not_match():
    a, b = ss.fingerprint(_graph_a()), ss.fingerprint(_graph_different_structure())
    assert not ss.structurally_identical(a, b), \
        "graphs with different relation ops must NOT be identical"
    assert ss.structural_similarity(a, b) < 1.0
    print("PASS: test_different_structure_must_not_match")


def test_extra_operation_must_not_match():
    a = _graph_a()
    b = copy.deepcopy(a)
    b["operations"] = [{"inputs": [], "output": _e("term", index=0), "op": "op:Foo"}]
    assert not ss.structurally_identical(ss.fingerprint(a), ss.fingerprint(b)), \
        "adding an operation did not change the fingerprint"
    print("PASS: test_extra_operation_must_not_match")


def test_arity_change_must_not_match():
    base = _g(entities=[_e("term", index=0)],
              operations=[{"inputs": [], "output": _e("term", index=0), "op": "op:Foo"}])
    wider = _g(entities=[_e("term", index=0), _e("term", index=1)],
               operations=[{"inputs": [_e("term", index=0)], "output": _e("term", index=1),
                            "op": "op:Foo"}])
    assert not ss.structurally_identical(ss.fingerprint(base), ss.fingerprint(wider)), \
        "operation arity change did not change the fingerprint"
    print("PASS: test_arity_change_must_not_match")


def test_operation_identity_must_not_match():
    """Same arity/output, different op identity -> must NOT match.

    Added because the mutation guard, run WITHOUT the corpus present (the CI
    condition — `Corpus/corpus.jsonl` is gitignored), reported this as a GAP: the
    only fixture catching it was the real-corpus one, which SKIPs on a clean
    checkout. A guard whose detection depends on a gitignored artifact is not a
    guard.
    """
    foo = _g(entities=[_e("term", index=0), _e("term", index=1)],
             operations=[{"inputs": [_e("term", index=0)], "output": _e("term", index=1),
                          "op": "op:Foo"}])
    bar = _g(entities=[_e("term", index=0), _e("term", index=1)],
             operations=[{"inputs": [_e("term", index=0)], "output": _e("term", index=1),
                          "op": "op:Bar"}])
    a, b = ss.fingerprint(foo), ss.fingerprint(bar)
    assert a.n_operations == b.n_operations and a.n_entities == b.n_entities, \
        "fixture must isolate op identity only"
    assert not ss.structurally_identical(a, b), \
        "two graphs differing only in operation identity fingerprinted identically"
    print("PASS: test_operation_identity_must_not_match")


def test_relation_operator_must_not_match():
    """Same endpoints, different relation operator -> must NOT match.

    Synthetic (not corpus-dependent) for the same reason as above.
    """
    eq = _g(entities=[_e("var", name="a"), _e("var", name="b")],
            relations=[{"src": _e("var", name="a"), "tgt": _e("var", name="b"), "op": "eq"}])
    lt = _g(entities=[_e("var", name="a"), _e("var", name="b")],
            relations=[{"src": _e("var", name="a"), "tgt": _e("var", name="b"), "op": "lt"}])
    assert not ss.structurally_identical(ss.fingerprint(eq), ss.fingerprint(lt)), \
        "relation operator difference not detected"
    print("PASS: test_relation_operator_must_not_match")


def test_relation_direction_must_not_match():
    """`R src tgt op` is ordered: swapping src/tgt is a structural difference.

    Added because the mutation guard found this was UNPROTECTED -- my
    `_graph_different_structure` fixture varies only the *operator*, so collapsing
    src/tgt left every test passing.
    """
    lhs = _g(entities=[_e("var", name="a"), _e("var", name="b")],
             relations=[{"src": _e("var", name="a"), "tgt": _e("var", name="b"), "op": "eq"}])
    rhs = _g(entities=[_e("var", name="a"), _e("var", name="b")],
             relations=[{"src": _e("var", name="b"), "tgt": _e("var", name="a"), "op": "eq"}])
    assert not ss.structurally_identical(ss.fingerprint(lhs), ss.fingerprint(rhs)), \
        "swapping relation src/tgt did not change the fingerprint (direction lost)"
    print("PASS: test_relation_direction_must_not_match")


def test_operation_arity_must_not_match_exactly():
    """Arity is structural: same op identity, different input count -> different.

    Added because the mutation guard found this UNPROTECTED. `test_arity_change_*`
    coincidentally also added an entity, so it did not isolate arity.
    """
    one = _g(entities=[_e("term", index=0), _e("term", index=1)],
             operations=[{"inputs": [_e("term", index=0)], "output": _e("term", index=1),
                          "op": "op:Same"}])
    two = _g(entities=[_e("term", index=0), _e("term", index=1)],
             operations=[{"inputs": [_e("term", index=0), _e("term", index=1)],
                          "output": _e("term", index=0), "op": "op:Same"}])
    a, b = ss.fingerprint(one), ss.fingerprint(two)
    assert a.n_entities == b.n_entities, "fixture must isolate arity, not entity count"
    assert not ss.structurally_identical(a, b), \
        "operation arity difference not detected with entity count held equal"
    print("PASS: test_operation_arity_must_not_match_exactly")


# ---------------------------------------------------------------------------
# Cost-function properties (documented: 0.7 facts + 0.3 ops Jaccard)
# ---------------------------------------------------------------------------

def test_similarity_properties():
    a = ss.fingerprint(_graph_a_for_presentational_comparison())
    b = ss.fingerprint(_graph_a_renamed())
    c = ss.fingerprint(_graph_different_structure())
    assert ss.structural_similarity(a, b) == 1.0, "identical -> not 1.0"
    assert 0.0 <= ss.structural_similarity(a, c) <= 1.0, "out of range"
    assert ss.structural_similarity(a, c) == ss.structural_similarity(c, a), "not symmetric"
    # Monotone in structure: a nearer graph scores higher than a farther one.
    d_far = _g(entities=[_e("var", name="Z")],
               relations=[{"src": _e("var", name="Z"), "tgt": _e("var", name="Z"), "op": "ne"}])
    assert ss.structural_similarity(a, c) > ss.structural_similarity(a, ss.fingerprint(d_far)), \
        "similarity is not monotone in structural distance"
    print("PASS: test_similarity_properties")


def test_empty_graphs():
    e = ss.fingerprint(_g([]))
    assert ss.structural_similarity(e, e) == 1.0
    assert ss.structurally_identical(e, ss.fingerprint(_g([])))
    print("PASS: test_empty_graphs")


# ---------------------------------------------------------------------------
# Corpus-mode guard (the #28 DoD addendum)
# ---------------------------------------------------------------------------

def test_module_mode_corpus_is_rejected():
    """Module mode collapses op identity, so structural search over it is lossy."""
    rec = {"graph": _g([_e("term", index=0)],
                       operations=[{"inputs": [], "output": _e("term", index=0),
                                    "op": "GEN_ALGEBRA"}])}
    try:
        ss.assert_per_operator_corpus([rec])
    except ss.WrongCorpusMode as e:
        assert "MODULE-bucketed" in str(e), e
        print("PASS: test_module_mode_corpus_is_rejected")
        return
    raise AssertionError("module-bucketed corpus was accepted")


def test_per_operator_corpus_is_accepted():
    rec = {"graph": _g([_e("term", index=0)],
                       operations=[{"inputs": [], "output": _e("term", index=0),
                                    "op": "op:Foo"}])}
    ss.assert_per_operator_corpus([rec])   # must not raise
    print("PASS: test_per_operator_corpus_is_accepted")


# ---------------------------------------------------------------------------
# Real-corpus tests (only where ground truth is decidable)
# ---------------------------------------------------------------------------

def _real():
    if not CORPUS.exists():
        return None
    recs = []
    with open(CORPUS) as f:
        for line in f:
            r = json.loads(line)
            if r.get("graph"):
                recs.append(r)
    return recs


def test_real_corpus_known_group_matches():
    """A real structural group: CancelMonoid / CommMonoid / SubNegMonoid.

    These are distinct declarations whose extracted graphs are the same shape.
    Ground truth is *structural identity*, which is decidable -- not a claim of
    semantic similarity.
    """
    recs = _real()
    if recs is None:
        print("SKIP: test_real_corpus_known_group_matches (no corpus)")
        return
    by_name = {r["name"]: r for r in recs}
    missing = [n for n in ("CancelMonoid", "CommMonoid") if n not in by_name]
    if missing:
        print(f"SKIP: test_real_corpus_known_group_matches ({missing} absent)")
        return
    a = ss.fingerprint(by_name["CancelMonoid"]["graph"])
    b = ss.fingerprint(by_name["CommMonoid"]["graph"])
    assert ss.structurally_identical(a, b), \
        "the CancelMonoid/CommMonoid structural group did not match"
    print("PASS: test_real_corpus_known_group_matches")


def test_real_corpus_distinct_graphs_differ():
    recs = _real()
    if recs is None:
        print("SKIP: test_real_corpus_distinct_graphs_differ (no corpus)")
        return
    # Find a graph with many operations and perturb exactly one.
    richest = max(recs, key=lambda r: len(r["graph"].get("operations") or []))
    g = richest["graph"]
    if not g.get("operations"):
        print("SKIP: test_real_corpus_distinct_graphs_differ (no op-rich graph)")
        return
    base = ss.fingerprint(g)
    changed = copy.deepcopy(g)
    changed["operations"][0]["op"] = "op:ZZZ_DELIBERATELY_DIFFERENT"
    assert not ss.structurally_identical(base, ss.fingerprint(changed)), \
        "changing one real operation identity did not break structural identity"
    assert ss.structural_similarity(base, ss.fingerprint(changed)) < 1.0
    print(f"PASS: test_real_corpus_distinct_graphs_differ ({richest['name'][:30]})")


def test_real_corpus_indexes_and_group_count():
    """Sanity on real scale, and a regression pin for the group count."""
    recs = _real()
    if recs is None:
        print("SKIP: test_real_corpus_indexes_and_group_count (no corpus)")
        return
    from collections import Counter
    fps = [ss.fingerprint(r["graph"]) for r in recs]
    assert len(fps) > 1000, f"corpus unexpectedly small: {len(fps)}"
    groups = Counter(fp.key() for fp in fps)
    shared = sum(v for v in groups.values() if v > 1)
    assert shared > 100, (
        f"only {shared} graphs share a fingerprint -- the abstraction may be too "
        f"fine to be useful (it should find real structural groups)"
    )
    print(f"PASS: test_real_corpus_indexes_and_group_count "
          f"({len(fps)} graphs, {len(groups)} distinct, {shared} in shared groups)")


def main() -> int:
    tests = [
        test_same_structure_different_names_must_match,
        test_free_variable_name_change_must_not_match,
        test_positional_index_renumbering_is_invariant,
        test_ordering_is_invariant,
        test_polarity_is_ignored,
        test_different_structure_must_not_match,
        test_extra_operation_must_not_match,
        test_arity_change_must_not_match,
        test_relation_direction_must_not_match,
        test_operation_identity_must_not_match,
        test_relation_operator_must_not_match,
        test_operation_arity_must_not_match_exactly,
        test_similarity_properties,
        test_empty_graphs,
        test_module_mode_corpus_is_rejected,
        test_per_operator_corpus_is_accepted,
        test_real_corpus_known_group_matches,
        test_real_corpus_distinct_graphs_differ,
        test_real_corpus_indexes_and_group_count,
    ]
    passed = failed = skipped = 0
    for t in tests:
        try:
            t()
            passed += 1
        except AssertionError as e:
            print(f"FAIL: {t.__name__}: {e}")
            failed += 1
        except Exception as e:
            print(f"ERROR: {t.__name__}: {type(e).__name__}: {e}")
            failed += 1
    print()
    print("=" * 40)
    print(f"Tests: {passed} passed, {failed} failed")
    print("=" * 40)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())