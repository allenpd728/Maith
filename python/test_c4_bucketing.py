#!/usr/bin/env python3
"""
test_c4_bucketing.py

Regression tests for the v2 IR C4 fix (gen:* → GEN_<bucket>) and the train
resume numeric-checkpoint sort.

C4 (module-based GEN bucketing) was approved in DEC-026 but was NOT present in
the v2 dataset — encode_ir mapped every gen:* token to a single GEN_UNK. These
tests guard against that regression: gen:* tokens must map to the bucket derived
from the declaration's module, matching bucketFromModule in Maith/MetaExtractor.lean.

Also guards the train_v2_resume.py resume fix: checkpoint dirs must be sorted
numerically (checkpoint-1000 > checkpoint-200), not lexicographically.

CPU-only, synthetic fixtures, < 5s. Run with:
    python3 python/test_c4_bucketing.py
Non-zero exit = at least one failure.
"""

import os
import sys
import importlib.util

sys.path.insert(0, "python")

# load build_dataset.py
_spec = importlib.util.spec_from_file_location(
    "build_dataset", os.path.join("python", "build_dataset.py")
)
_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)

bucket_from_module = _mod.bucket_from_module
encode_ir = _mod.encode_ir

failures = []


def check(cond, msg):
    print(("  [OK] " if cond else "  [FAIL] ") + msg)
    if not cond:
        failures.append(msg)


# ---------------------------------------------------------------------------
# bucket_from_module — must match Lean bucketFromModule exactly
# ---------------------------------------------------------------------------

def test_bucket_from_module():
    print("\n== bucket_from_module (Lean parity) ==")
    cases = {
        "Mathlib.Algebra.Group.Basic": "GEN_ALGEBRA",
        "Mathlib.Algebra.Ring.Defs": "GEN_ALGEBRA",
        "Mathlib.Order.Lattice": "GEN_ORDER",
        "Mathlib.Order.Basic": "GEN_ORDER",
        "Mathlib.Topology.Basic": "GEN_TOPOLOGY",
        "Mathlib.Analysis.Normed": "GEN_ANALYSIS",
        "Mathlib.Logic.Basic": "GEN_LOGIC",
        "Mathlib.Tactic.Linarith": "GEN_LOGIC",
        "Mathlib.Data.Set.Basic": "GEN_DATA",
        "Mathlib.Combinatorics.SimpleGraph": "GEN_DATA",
        "Mathlib.Set.Something": "GEN_MATHLIB",   # unknown 2nd component
        "Mathlib": "GEN_MATHLIB",                  # only one component
        "Lean.Parser.Term": "GEN_LEAN",
        "Init.Prelude": "GEN_INIT",
        "Std.Data.List": "GEN_STD",
        "Bare": "GEN_MATHLIB",                      # non-Mathlib, unknown
        "": "GEN_MATHLIB",                          # empty module
    }
    for mod, want in cases.items():
        got = bucket_from_module(mod)
        check(got == want, f"{mod!r:42} -> {got} (want {want})")


# ---------------------------------------------------------------------------
# encode_ir — gen:* must map to the module bucket, not GEN_UNK
# ---------------------------------------------------------------------------

def test_encode_ir_gen_bucketing():
    print("\n== encode_ir gen:* -> GEN_<bucket> ==")
    vocab = {
        "<PAD>": 0, "<BOS>": 1, "<EOS>": 2, "<UNK>": 3, "GEN_UNK": 4,
        "GRAPH_BEGIN": 5, "GRAPH_END": 6, "E": 7, "O": 8, "eq": 9,
        "GEN_ALGEBRA": 29, "GEN_ORDER": 30, "GEN_MATHLIB": 40,
    }
    tokens = ["GRAPH_BEGIN", "E", "eq", "gen:Mathlib.Algebra.Group.add", "GRAPH_END"]
    ids = encode_ir(tokens, vocab, "Mathlib.Algebra.Group.Basic")
    check(ids[3] == 29, f"gen:Mathlib.Algebra... -> GEN_ALGEBRA (29); got {ids[3]}")
    check(ids[3] != 4, "gen:* must NOT collapse to GEN_UNK (4) when a bucket exists")

    # different module -> different bucket
    ids2 = encode_ir(tokens, vocab, "Mathlib.Order.Lattice")
    check(ids2[3] == 30, f"Mathlib.Order... -> GEN_ORDER (30); got {ids2[3]}")

    # unknown module -> GEN_MATHLIB fallback (not GEN_UNK)
    ids3 = encode_ir(tokens, vocab, "Mathlib.Unknown.Area")
    check(ids3[3] == 40, f"unknown Mathlib area -> GEN_MATHLIB (40); got {ids3[3]}")

    # bucket not in vocab -> falls back to GEN_UNK (graceful)
    ids4 = encode_ir(tokens, {"<UNK>": 3, "GEN_UNK": 4}, "Mathlib.Algebra.Group.Basic")
    check(ids4[3] == 4, "bucket missing from vocab -> GEN_UNK fallback (4)")

    # empty module -> GEN_UNK (no bucket info available)
    ids5 = encode_ir(tokens, vocab, "")
    check(ids5[3] == 4, "empty module -> GEN_UNK (4)")


# ---------------------------------------------------------------------------
# All gen:* tokens in a declaration get the SAME (module) bucket
# ---------------------------------------------------------------------------

def test_gen_tokens_share_bucket():
    print("\n== gen:* tokens share the declaration's bucket ==")
    vocab = {"<UNK>": 3, "GEN_UNK": 4, "GEN_ALGEBRA": 29, "E": 7, "O": 8}
    tokens = ["E", "gen:Foo", "O", "gen:Bar.Baz", "gen:Quux"]
    ids = encode_ir(tokens, vocab, "Mathlib.Algebra.Ring.Defs")
    gen_ids = [ids[i] for i in (1, 3, 4)]
    check(all(g == 29 for g in gen_ids), f"all gen:* -> GEN_ALGEBRA (29); got {gen_ids}")


# ---------------------------------------------------------------------------
# train_v2_resume.py — numeric checkpoint sort for resume
# ---------------------------------------------------------------------------

def test_resume_numeric_sort():
    print("\n== train_v2_resume.py numeric checkpoint sort ==")
    path = os.path.join("python", "train_v2_resume.py")
    if not os.path.exists(path):
        print("  [SKIP] python/train_v2_resume.py not present")
        return
    spec = importlib.util.spec_from_file_location("trv", path)
    # don't exec (heavy imports); just grep the sort key from source
    src = open(path).read()
    has_numeric = ("key=lambda" in src and "int(" in src and
                   'split("-")[-1]' in src)
    check(has_numeric, "resume uses numeric sort key (int(split('-')[-1]))")
    check("sorted(" in src and "checkpoint-" in src, "resume sorts checkpoint-* dirs")


def main():
    test_bucket_from_module()
    test_encode_ir_gen_bucketing()
    test_gen_tokens_share_bucket()
    test_resume_numeric_sort()
    print()
    if failures:
        print(f"FAIL: {len(failures)} test(s) failed:")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    print("PASS: all C4 bucketing + resume sort tests passed")


if __name__ == "__main__":
    main()
