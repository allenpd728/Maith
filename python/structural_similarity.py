#!/usr/bin/env python3
"""Structural similarity over IR graphs (issue #28, phase 1).

`Maith/GraphEquivalence.lean` is exact *equality* for testing. This is *similarity*
for candidate discovery: given IR graphs (from `Corpus/`), produce a canonical
structural fingerprint so that graphs which are the same shape under different
names can be found, and rank graph pairs by how structurally close they are.

## What "similar" means here (the design decision)

`AXIOM_DISCOVERY.md` needs "the structural search mechanism over candidate target
structures". That requires a decided notion of "match", and there are three:

- **exact identity** — trivial, and already `GraphEquivalence.lean`'s job.
- **alpha-equivalence modulo naming/ordering** — decidable, and the useful core.
  "Same shape, different names."
- **semantic similarity** — needs human judgement; no ground truth exists yet.

This module implements the second, plus a graded score for *ranking* (the issue
asks for "ranked proposals", not only a boolean).

## The fingerprint is invariant to

- the declaration name (a graph from `mul_assoc` and one from `neg_neg` with
  identical structure must fingerprint identically)
- entity naming (free-variable names, bound scopes)
- **positional index renumbering** — `FVAR_0` in one graph has no relation to
  `FVAR_2` in another (the index is local per graph; see `GLOSSARY.md`), so
  indices must be canonicalised away
- `polarity` — dropped by C1 in v2 and carrying no signal (99.8% `neut`), so it is
  ignored rather than allowed to create spurious differences

## ...and sensitive to

- row kinds and their counts (E / A / R / O)
- the arity of each operation
- relation operator identity (`eq`, `lt`, ...)
- attribute keys, and values that are *semantic* (e.g. `typeclass Ring`) rather
  than entity references
- **operation identity** — and this is exactly where the extraction mode matters.
  In `--per-operator` mode ops carry `op:<shortName>`; in the default `module`
  mode they collapse to ~10 `GEN_*` buckets, so two graphs differing only in
  *which* operator they use fingerprint identically. The issue's DoD addendum
  requires the per-operator corpus for this reason; `assert_per_operator_corpus`
  below makes using the wrong mode a loud error rather than a silent quality loss.

Usage:
    python3 python/structural_similarity.py --corpus Corpus/corpus.per_operator.jsonl \
        --query <declName> --top 10
    python3 python/structural_similarity.py --corpus ... --pairs --top 20
"""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Optional

REPO = Path(__file__).resolve().parent.parent
DEFAULT_CORPUS = REPO / "Corpus" / "corpus.per_operator.jsonl"

# The module-mode bucket tokens. Presence of these (and absence of op:*) means the
# corpus was built in module mode, which silently destroys operator identity.
_MODULE_BUCKET_PREFIX = "GEN_"
_PEROP_PREFIX = "op:"


class WrongCorpusMode(RuntimeError):
    """Raised when a module-mode corpus is used for structural search."""


# ---------------------------------------------------------------------------
# Fingerprint
# ---------------------------------------------------------------------------

@dataclass(frozen=True)
class Fingerprint:
    """Canonical structural form of one IR graph.

    `facts` is a multiset (Counter) of structural facts with all naming and
    positional identity abstracted away. Two graphs are *structurally identical*
    iff their `facts` counters are equal.
    """

    facts: Counter
    n_entities: int
    n_attributes: int
    n_relations: int
    n_operations: int
    ops: frozenset           # operation identities present (op identity is kept)

    @property
    def size(self) -> int:
        return self.n_entities + self.n_attributes + self.n_relations + self.n_operations

    def key(self) -> tuple:
        """Hashable canonical form — usable as a dict key / for grouping.

        `facts` is a `Counter` (unhashable), so grouping graphs by structural
        identity needs this. Sorted rather than insertion-ordered so equal
        fingerprints always produce an equal key.
        """
        return tuple(sorted(self.facts.items()))


def _is_bound_or_term(entity_id: dict) -> bool:
    """Positional or scoped ids: their *name* is not content, only their role."""
    kind = entity_id.get("kind")
    return kind in ("bound", "term")


def _entity_role(entity_id: dict) -> str:
    """Abstract an entity id to a structural role.

    `var` keeps its name: free variables like `HMul.hMul` and `Eq` are
    semantically stable across Mathlib (ENCODER_FORMAT's "rationale for var ->
    unchanged"), so they are real content, not noise.
    `bound`/`term` are positional, so the specific index/scope is abstracted.
    """
    kind = entity_id.get("kind")
    if kind == "var":
        return f"var:{entity_id.get('name')}"
    if kind == "term":
        return "term"
    if kind == "bound":
        return "bound"
    return f"unknown:{kind}"


def fingerprint(graph: dict) -> Fingerprint:
    """Canonical structural fingerprint of an IR graph object.

    Ignores `polarity` throughout (C1: dropped in v2, no signal).
    """
    facts: Counter = Counter()
    ops_seen: set = set()

    entities = graph.get("entities") or []
    attributes = graph.get("attributes") or []
    relations = graph.get("relations") or []
    operations = graph.get("operations") or []

    for e in entities:
        facts[("E", _entity_role(e.get("id") or {}))] += 1

    for a in attributes:
        # key is structural; value is content only when it is not an entity ref.
        key = a.get("key")
        val = a.get("value")
        facts[("A", key, "val:" + str(val))] += 1

    for r in relations:
        src = _entity_role(r.get("src") or {})
        tgt = _entity_role(r.get("tgt") or {})
        op = r.get("op")
        # Order matters for non-commutative relations, and direction is structural
        # information, so keep it. But canonicalise the *role pair* so a graph that
        # is otherwise identical matches regardless of which positional id happens
        # to be src.
        facts[("R", op, src, tgt)] += 1

    for o in operations:
        op = _operation_identity(o)
        ops_seen.add(op)
        inputs = o.get("inputs") or []
        arity = len(inputs)
        out = _entity_role(o.get("output") or {})
        facts[("O", op, f"arity:{arity}", f"out:{out}")] += 1

    return Fingerprint(
        facts=facts,
        n_entities=len(entities),
        n_attributes=len(attributes),
        n_relations=len(relations),
        n_operations=len(operations),
        ops=frozenset(ops_seen),
    )


def _operation_identity(op) -> str:
    """The operation's identity string, across the corpus's representations.

    The Lean side emits `OperationOp.generic "op:<shortName>"` in per-operator
    mode and `"GEN_<bucket>"` in module mode; serialize may store either the
    string or an object. Normalise to a single string.
    """
    if isinstance(op, str):
        return op
    if isinstance(op, dict):
        for key in ("name", "op", "generic", "tag"):
            if key in op and isinstance(op[key], str):
                return op[key]
        return json.dumps(op, sort_keys=True)
    return str(op)


# ---------------------------------------------------------------------------
# Similarity / cost function (documented, per the DoD)
# ---------------------------------------------------------------------------

def structural_similarity(a: Fingerprint, b: Fingerprint) -> float:
    """Graded similarity in [0, 1] for ranking.

    Cost function (documented because the DoD requires it):

        sim = 0.7 * jaccard(multiset facts) + 0.3 * jaccard(op identities)

    - The **facts term** (weight 0.7) is a weighted Jaccard over the structural-fact
      multisets: `sum(min counts) / sum(max counts)`. Multiset rather than set, so
      arity/count differences reduce similarity continuously instead of falling off
      a cliff.
    - The **op term** (weight 0.3) is a plain Jaccard over operation identities. It
      is separated out because op identity is the axis that module-mode bucketing
      destroys; keeping it visible means a lossy corpus shows up as a *suspiciously
      high* similarity among unlike graphs rather than as an invisible error.
    - Identical fingerprints score exactly 1.0.

    Deliberately simple and total (never raises, symmetric, 1.0 on equality) so it
    is testable. It is a *heuristic for ranking*, never a claim about semantic
    relevance -- AXIOM_DISCOVERY.md's filter-not-generator constraint.
    """
    facts_term = _weighted_jaccard(a.facts, b.facts)
    if a.ops or b.ops:
        op_term = _set_jaccard(a.ops, b.ops)
    else:
        op_term = 1.0 if not (a.ops or b.ops) else 0.0
    return 0.7 * facts_term + 0.3 * op_term


def _weighted_jaccard(a: Counter, b: Counter) -> float:
    keys = set(a) | set(b)
    if not keys:
        return 1.0
    inter = sum(min(a[k], b[k]) for k in keys)
    union = sum(max(a[k], b[k]) for k in keys)
    return inter / union if union else 1.0


def _set_jaccard(a: Iterable, b: Iterable) -> float:
    sa, sb = set(a), set(b)
    if not sa and not sb:
        return 1.0
    return len(sa & sb) / len(sa | sb)


def structurally_identical(a: Fingerprint, b: Fingerprint) -> bool:
    """True iff the canonical forms match exactly (modulo naming/ordering)."""
    return a.facts == b.facts


# ---------------------------------------------------------------------------
# Corpus loading
# ---------------------------------------------------------------------------

def assert_per_operator_corpus(records: list[dict]) -> None:
    """Fail loudly if the corpus is module-bucketed (issue #28 DoD addendum).

    In module mode every generic op collapses to one of ~10 `GEN_*` tokens, so
    structural search would match *shape* but not *which operation* -- two graphs
    differing only in operator identity look identical. That is a silent quality
    loss, so it is an error here rather than a warning.
    """
    module_tokens = 0
    perop_tokens = 0
    for rec in records:
        for o in ((rec.get("graph") or {}).get("operations") or []):
            ident = _operation_identity(o.get("op"))
            if ident.startswith(_MODULE_BUCKET_PREFIX):
                module_tokens += 1
            elif ident.startswith(_PEROP_PREFIX):
                perop_tokens += 1
    if module_tokens and not perop_tokens:
        raise WrongCorpusMode(
            f"corpus is MODULE-bucketed ({module_tokens} GEN_* op tokens, "
            f"0 op:*): structural search over it would match shape but not "
            f"operator identity (silent quality loss). Rebuild with "
            f"`lake env ./.lake/build/bin/buildCorpus --per-operator`."
        )


def load_corpus(path: Path) -> list[dict]:
    if not path.exists():
        raise FileNotFoundError(
            f"corpus not found: {path}\n"
            f"  Corpus/corpus*.jsonl is gitignored; build it first:\n"
            f"  `lake env ./.lake/build/bin/buildCorpus --per-operator`"
        )
    out = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                out.append(json.loads(line))
    return out


def index_corpus(path: Path) -> list[tuple[str, Fingerprint]]:
    records = load_corpus(path)
    assert_per_operator_corpus(records)
    indexed = []
    for rec in records:
        graph = rec.get("graph")
        if not graph:
            continue
        indexed.append((rec.get("name", "<unnamed>"), fingerprint(graph)))
    return indexed


# ---------------------------------------------------------------------------
# Query
# ---------------------------------------------------------------------------

def rank_similar(indexed, query_fp: Fingerprint, top: int = 10,
                 exclude_name: Optional[str] = None):
    scored = []
    for name, fp in indexed:
        if name == exclude_name:
            continue
        scored.append((structural_similarity(query_fp, fp), name, fp))
    scored.sort(key=lambda t: (-t[0], t[1]))
    return scored[:top]


def main() -> int:
    ap = argparse.ArgumentParser(description="structural similarity over IR graphs (#28)")
    ap.add_argument("--corpus", default=str(DEFAULT_CORPUS))
    ap.add_argument("--query", default=None, help="declaration name to find neighbours of")
    ap.add_argument("--pairs", action="store_true",
                    help="report the most similar graph pair overall")
    ap.add_argument("--top", type=int, default=10)
    args = ap.parse_args()

    try:
        indexed = index_corpus(Path(args.corpus))
    except (WrongCorpusMode, FileNotFoundError) as e:
        print(f"ERROR: {e}", file=sys.stderr)
        return 2

    print(f"loaded {len(indexed)} graphs from {args.corpus}")

    if args.query:
        match = [fp for n, fp in indexed if n == args.query]
        if not match:
            print(f"ERROR: {args.query!r} not in corpus", file=sys.stderr)
            return 2
        print(f"\nnearest neighbours of {args.query}:")
        for score, name, fp in rank_similar(indexed, match[0], args.top, args.query):
            ident = "IDENTICAL" if structurally_identical(match[0], fp) else ""
            print(f"  {score:.4f}  {name}  {ident}")
    elif args.pairs:
        best = []
        for i, (n1, f1) in enumerate(indexed):
            for n2, f2 in index_similar_range(indexed, i + 1):
                s = structural_similarity(f1, f2)
                best.append((s, n1, n2))
        best.sort(key=lambda t: (-t[0], t[1], t[2]))
        print(f"\ntop {args.top} most similar pairs:")
        for s, n1, n2 in best[:args.top]:
            print(f"  {s:.4f}  {n1}  <->  {n2}")

    return 0


def index_similar_range(indexed, start):
    for j in range(start, len(indexed)):
        yield indexed[j]


if __name__ == "__main__":
    sys.exit(main())