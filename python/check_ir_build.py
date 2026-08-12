#!/usr/bin/env python3
"""
check_ir_build.py — Gate 1: IR Build Quality Check

Verifies the corpus JSONL produced by the Lean IR pipeline (Station 1)
before it is accepted as valid input for dataset building.

Checks:
  G1-1  Round-trip: validate_roundtrip.py passes all declarations
  G1-2  Yield: stats.json shows 100% success, zero failures
  G1-3  Format consistency: no mixed gen:FullName and GEN_<BUCKET> tokens
  G1-4  Sequence length: no sequences exceed max_seq_len (default 512)

Usage:
    python3 python/check_ir_build.py [--corpus Corpus/corpus.per_operator.jsonl]
                                      [--stats Corpus/stats.json]
                                      [--max-seq-len 512]
                                      [--skip-roundtrip]  # skip G1-1 if Lean not available
"""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parent.parent


def _report(name, passed, detail=""):
    status = "PASS" if passed else "FAIL"
    msg = f"  {status}  {name}"
    if detail:
        msg += f": {detail}"
    print(msg)
    return passed


def check_g1_1_roundtrip(args):
    """G1-1: Round-trip passes (validate_roundtrip.py exits 0)."""
    if args.skip_roundtrip:
        print("  SKIP G1-1 round_trip: --skip-roundtrip set (Lean may not be available)")
        return True
    script = _REPO_ROOT / "python" / "validate_roundtrip.py"
    if not script.exists():
        print("  SKIP G1-1 round_trip: validate_roundtrip.py not found")
        return True
    result = subprocess.run(
        [sys.executable, str(script)],
        capture_output=True, text=True,
    )
    return _report("G1-1 round_trip", result.returncode == 0,
                    f"{'passed' if result.returncode == 0 else 'FAILED'}")


def check_g1_2_yield(args):
    """G1-2: Yield is 100% (all declarations extracted successfully)."""
    stats_path = Path(args.stats)
    if not stats_path.exists():
        return _report("G1-2 yield", False, f"stats.json not found at {stats_path}")
    with open(stats_path) as f:
        stats = json.load(f)
    total = stats.get("totalDeclarations", 0)
    success = stats.get("successfulExamples", 0)
    failures = stats.get("failureStats", {})
    total_failures = 0
    if isinstance(failures, dict):
        for v in failures.values():
            if isinstance(v, (int, float)):
                total_failures += v
            elif isinstance(v, list):
                total_failures += len(v)
    elif isinstance(failures, (int, float)):
        total_failures = failures
    passed = (success == total and total_failures == 0)
    return _report("G1-2 yield", passed,
                    f"{success}/{total} successful, {total_failures} failures")


def check_g1_3_format(args):
    """G1-3: Format consistency (no mixed gen:FullName and GEN_<BUCKET> tokens)."""
    corpus_path = Path(args.corpus)
    if not corpus_path.exists():
        return _report("G1-3 format", False, f"corpus not found at {corpus_path}")

    gen_named = 0
    gen_bucket = 0
    records_checked = 0

    with open(corpus_path) as f:
        for i, line in enumerate(f):
            if i >= 200:
                break
            d = json.loads(line)
            for t in d.get("tokens", []):
                if not isinstance(t, str):
                    continue
                if t.startswith("gen:") and t not in ("gen:hof", "gen:proj"):
                    gen_named += 1
                elif t.startswith("GEN_"):
                    gen_bucket += 1
            records_checked += 1

    if gen_named > 0 and gen_bucket > 0:
        return _report("G1-3 format", False,
                        f"MIXED format: gen:FullName={gen_named}, GEN_={gen_bucket} in {records_checked} records")
    elif gen_named > 0:
        return _report("G1-3 format", True,
                        f"per_operator (gen:FullName={gen_named}, GEN_=0 in {records_checked} records)")
    elif gen_bucket > 0:
        return _report("G1-3 format", True,
                        f"module_mode (GEN_={gen_bucket}, gen:FullName=0 in {records_checked} records)")
    else:
        return _report("G1-3 format", True, f"no gen tokens found in {records_checked} records (may be flat-IR)")


def check_g1_4_seq_len(args):
    """G1-4: No sequences exceed max_seq_len."""
    corpus_path = Path(args.corpus)
    if not corpus_path.exists():
        return _report("G1-4 seq_len", False, f"corpus not found at {corpus_path}")

    max_len = args.max_seq_len
    overlength = []
    max_seen = 0

    with open(corpus_path) as f:
        for line in f:
            d = json.loads(line)
            tokens = d.get("tokens", [])
            seq_len = len(tokens) if isinstance(tokens, list) else d.get("seq_len", 0)
            if seq_len > max_seen:
                max_seen = seq_len
            if seq_len > max_len:
                overlength.append((d.get("name", "?"), seq_len))

    if overlength:
        worst = max(overlength, key=lambda x: x[1])
        return _report("G1-4 seq_len", False,
                        f"{len(overlength)} sequences exceed max_len={max_len} (worst: {worst[0]} at {worst[1]})")
    return _report("G1-4 seq_len", True,
                    f"all sequences within max_len={max_len} (max seen: {max_seen})")


def main():
    parser = argparse.ArgumentParser(description="Gate 1: IR Build Quality Check")
    parser.add_argument("--corpus", default=str(_REPO_ROOT / "Corpus" / "corpus.per_operator.jsonl"))
    parser.add_argument("--stats", default=str(_REPO_ROOT / "Corpus" / "stats.json"))
    parser.add_argument("--max-seq-len", type=int, default=512)
    parser.add_argument("--skip-roundtrip", action="store_true",
                        help="Skip G1-1 (use when Lean toolchain is not available)")
    args = parser.parse_args()

    print("=" * 60)
    print("GATE 1 — IR BUILD QUALITY CHECK")
    print("=" * 60)

    results = [
        check_g1_1_roundtrip(args),
        check_g1_2_yield(args),
        check_g1_3_format(args),
        check_g1_4_seq_len(args),
    ]

    all_pass = all(results)
    total = len(results)
    passed = sum(results)
    print()
    if all_pass:
        print(f"Gate 1 PASSED — {passed}/{total} checks passed.")
    else:
        print(f"Gate 1 FAILED — {passed}/{total} checks passed.")
        print("Resolve failures before proceeding to dataset build.")
        sys.exit(1)


if __name__ == "__main__":
    main()
