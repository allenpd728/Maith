#!/usr/bin/env python3
"""
test_golden_examples.py

Golden examples fixture — verifies that specific Mathlib declarations encode
to exact, known token sequences. These are regression tests: if the encoder
changes in a way that alters these sequences, this file fails immediately.

Each example was extracted from a real corpus run (encoder v1.2.0,
Mathlib fabf563a / v4.31.0) and hand-verified for structural correctness.

Coverage:
  - forall-only binders (FVAR_N only)               -> LeftCancelSemigroup.toIsLeftCancelMul
  - forall + lambda binders (FVAR_N + BVAR_N mixed) -> IsAddCommutative.instDivisionAddCommMonoid
  - semantic op token (add, not gen:*)               -> IsAddCommutative.instAddCommSemigroup._proof_1
  - EntityId.var (named constant, no positional ID) -> npow_eq_pow

Usage:
    python3 python/test_golden_examples.py
    python3 python/test_golden_examples.py --corpus Corpus/corpus.jsonl
"""

import argparse
import json
import sys

# ---------------------------------------------------------------------------
# Golden examples (encoder v1.2.0, Mathlib fabf563a)
# ---------------------------------------------------------------------------

GOLDEN = {

    # Pure forall binders only — FVAR_0, FVAR_1, no BVAR tokens.
    "LeftCancelSemigroup.toIsLeftCancelMul": [
        "GRAPH_BEGIN",
        "E", "FVAR_0", "neut", "E", "FVAR_1", "neut",
        "E", "TERM_0", "neut", "E", "TERM_1", "neut", "E", "TERM_2", "neut",
        "E", "TERM_3", "neut", "E", "TERM_4", "neut",
        "A", "FVAR_1", "typeclass", "LeftCancelSemigroup", "neut",
        "A", "TERM_0", "sort", "u + 1", "neut",
        "R", "FVAR_0", "TERM_0", "eq", "neut",
        "O", "inputs:FVAR_0", "output:TERM_1", "gen:LeftCancelSemigroup", "neut",
        "O", "inputs:FVAR_0,FVAR_1", "output:TERM_2", "gen:LeftCancelSemigroup.toSemigroup", "neut",
        "O", "inputs:FVAR_0,TERM_2", "output:TERM_3", "gen:Semigroup.toMul", "neut",
        "O", "inputs:FVAR_0,TERM_3", "output:TERM_4", "gen:IsLeftCancelMul", "neut",
        "GRAPH_END",
    ],

    # Mixed forall + lambda binders — FVAR_N and BVAR_N both present.
    # Verifies separate counters each reset to 0, multiple typeclass attrs.
    "IsAddCommutative.instDivisionAddCommMonoid": [
        "GRAPH_BEGIN",
        "E", "BVAR_0", "neut", "E", "BVAR_1", "neut", "E", "BVAR_2", "neut",
        "E", "FVAR_0", "neut", "E", "FVAR_1", "neut", "E", "FVAR_2", "neut",
        "E", "TERM_0", "neut", "E", "TERM_1", "neut", "E", "TERM_2", "neut",
        "E", "TERM_3", "neut", "E", "TERM_4", "neut", "E", "TERM_5", "neut",
        "E", "TERM_6", "neut", "E", "TERM_7", "neut", "E", "TERM_8", "neut",
        "E", "TERM_9", "neut", "E", "TERM_10", "neut", "E", "TERM_11", "neut",
        "E", "TERM_12", "neut", "E", "TERM_13", "neut", "E", "TERM_14", "neut",
        "E", "TERM_15", "neut", "E", "TERM_16", "neut", "E", "TERM_17", "neut",
        "E", "TERM_18", "neut",
        "A", "BVAR_1", "typeclass", "SubtractionMonoid", "neut",
        "A", "BVAR_2", "typeclass", "IsAddCommutative", "neut",
        "A", "FVAR_1", "typeclass", "SubtractionMonoid", "neut",
        "A", "FVAR_2", "typeclass", "IsAddCommutative", "neut",
        "A", "TERM_0", "sort", "u_2 + 1", "neut",
        "A", "TERM_9", "sort", "u_2 + 1", "neut",
        "R", "BVAR_0", "TERM_9", "eq", "neut",
        "R", "FVAR_0", "TERM_0", "eq", "neut",
        "O", "inputs:FVAR_0", "output:TERM_1", "gen:SubtractionMonoid", "neut",
        "O", "inputs:FVAR_0,FVAR_1", "output:TERM_2", "gen:SubtractionMonoid.toSubNegMonoid", "neut",
        "O", "inputs:FVAR_0,TERM_2", "output:TERM_3", "gen:SubNegMonoid.toAddMonoid", "neut",
        "O", "inputs:FVAR_0,TERM_3", "output:TERM_4", "gen:AddMonoid.toAddZeroClass", "neut",
        "O", "inputs:FVAR_0,TERM_4", "output:TERM_5", "gen:AddZeroClass.toAddZero", "neut",
        "O", "inputs:FVAR_0,TERM_5", "output:TERM_6", "gen:AddZero.toAdd", "neut",
        "O", "inputs:FVAR_0,TERM_6", "output:TERM_7", "gen:IsAddCommutative", "neut",
        "O", "inputs:FVAR_0", "output:TERM_8", "gen:SubtractionCommMonoid", "neut",
        "O", "inputs:BVAR_0", "output:TERM_10", "gen:SubtractionMonoid", "neut",
        "O", "inputs:BVAR_0,BVAR_1", "output:TERM_11", "gen:SubtractionMonoid.toSubNegMonoid", "neut",
        "O", "inputs:BVAR_0,TERM_11", "output:TERM_12", "gen:SubNegMonoid.toAddMonoid", "neut",
        "O", "inputs:BVAR_0,TERM_12", "output:TERM_13", "gen:AddMonoid.toAddZeroClass", "neut",
        "O", "inputs:BVAR_0,TERM_13", "output:TERM_14", "gen:AddZeroClass.toAddZero", "neut",
        "O", "inputs:BVAR_0,TERM_14", "output:TERM_15", "gen:AddZero.toAdd", "neut",
        "O", "inputs:BVAR_0,TERM_15", "output:TERM_16", "gen:IsAddCommutative", "neut",
        "O", "inputs:BVAR_0,BVAR_1,BVAR_2", "output:TERM_17",
        "gen:IsAddCommutative.instDivisionAddCommMonoid._proof_1", "neut",
        "O", "inputs:BVAR_0,BVAR_1,TERM_17", "output:TERM_18", "gen:SubtractionCommMonoid.mk", "neut",
        "GRAPH_END",
    ],

    # Forall binders with semantic `add` op (not gen:*).
    # Verifies non-generic operation tokens pass through correctly.
    "IsAddCommutative.instAddCommSemigroup._proof_1": [
        "GRAPH_BEGIN",
        "E", "FVAR_0", "neut", "E", "FVAR_1", "neut", "E", "FVAR_2", "neut",
        "E", "FVAR_3", "neut", "E", "FVAR_4", "neut",
        "E", "TERM_0", "neut", "E", "TERM_1", "neut", "E", "TERM_2", "neut",
        "E", "TERM_3", "neut", "E", "TERM_4", "neut", "E", "TERM_5", "neut",
        "A", "FVAR_1", "typeclass", "AddSemigroup", "neut",
        "A", "FVAR_2", "typeclass", "IsAddCommutative", "neut",
        "A", "TERM_0", "sort", "u_1 + 1", "neut",
        "R", "FVAR_0", "TERM_0", "eq", "neut",
        "R", "FVAR_3", "FVAR_0", "eq", "neut",
        "R", "FVAR_4", "FVAR_0", "eq", "neut",
        "R", "TERM_4", "TERM_5", "eq", "neut",
        "O", "inputs:FVAR_0", "output:TERM_1", "gen:AddSemigroup", "neut",
        "O", "inputs:FVAR_0,FVAR_1", "output:TERM_2", "gen:AddSemigroup.toAdd", "neut",
        "O", "inputs:FVAR_0,TERM_2", "output:TERM_3", "gen:IsAddCommutative", "neut",
        "O", "inputs:FVAR_3,FVAR_4", "output:TERM_4", "add", "neut",
        "O", "inputs:FVAR_3,FVAR_4", "output:TERM_5", "add", "neut",
        "GRAPH_END",
    ],

    # EntityId.var (bare constant name) — Nat appears as a named entity, not positional.
    # Verifies: var IDs pass through unchanged alongside FVAR_N tokens.
    "npow_eq_pow": [
        "GRAPH_BEGIN",
        "E", "Nat", "neut",
        "E", "FVAR_0", "neut", "E", "FVAR_1", "neut",
        "E", "FVAR_2", "neut", "E", "FVAR_3", "neut",
        "E", "TERM_0", "neut", "E", "TERM_1", "neut", "E", "TERM_2", "neut",
        "E", "TERM_3", "neut", "E", "TERM_4", "neut", "E", "TERM_5", "neut",
        "A", "FVAR_1", "typeclass", "Monoid", "neut",
        "A", "TERM_0", "sort", "u_2 + 1", "neut",
        "R", "FVAR_0", "TERM_0", "eq", "neut",
        "R", "FVAR_2", "Nat", "eq", "neut",
        "R", "FVAR_3", "FVAR_0", "eq", "neut",
        "R", "TERM_2", "TERM_5", "eq", "neut",
        "O", "inputs:FVAR_0", "output:TERM_1", "gen:Monoid", "neut",
        "O", "inputs:FVAR_0,FVAR_1,FVAR_2,FVAR_3", "output:TERM_2", "gen:Monoid.npow", "neut",
        "O", "inputs:FVAR_0,FVAR_1", "output:TERM_3", "gen:Monoid.toPow", "neut",
        "O", "inputs:FVAR_0,Nat,TERM_3", "output:TERM_4", "gen:instHPow", "neut",
        "O", "inputs:FVAR_0,Nat,FVAR_0,TERM_4,FVAR_3,FVAR_2", "output:TERM_5", "gen:HPow.hPow", "neut",
        "GRAPH_END",
    ],
}


# ---------------------------------------------------------------------------
# Test runner
# ---------------------------------------------------------------------------

def run_golden_tests(corpus_path: str) -> bool:
    print(f"Loading corpus from {corpus_path} ...")
    index = {}
    with open(corpus_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            ex = json.loads(line)
            name = ex.get("name", "")
            if name in GOLDEN:
                index[name] = ex.get("tokens", [])
    print(f"  Found {len(index)}/{len(GOLDEN)} golden declarations in corpus.")
    print()

    passed = 0
    failed = 0

    for name, expected in GOLDEN.items():
        if name not in index:
            print(f"  MISSING  {name}")
            print(f"           Declaration not found in corpus.")
            failed += 1
            continue

        actual = index[name]
        if actual == expected:
            print(f"  PASS     {name}")
            passed += 1
        else:
            print(f"  FAIL     {name}")
            # Find first divergence point
            for i, (a, e) in enumerate(zip(actual, expected)):
                if a != e:
                    start = max(0, i - 2)
                    print(f"           First difference at token {i}:")
                    print(f"           Expected: {expected[start:i+4]}")
                    print(f"           Actual:   {actual[start:i+4]}")
                    break
            else:
                print(f"           Length mismatch: expected {len(expected)}, got {len(actual)}")
            failed += 1

    print()
    print(f"Results: {passed}/{passed+failed} passed")
    if failed == 0:
        print("  All golden examples match. Encoder v1.2.0 output is stable.")
        return True
    else:
        print(f"  {failed} golden example(s) failed.")
        print("  If the change is intentional, update GOLDEN and bump encoderVersion in stats.")
        return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    args = parser.parse_args()

    ok = run_golden_tests(args.corpus)
    sys.exit(0 if ok else 1)
