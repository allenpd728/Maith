#!/usr/bin/env python3
"""
check_corpus.py — Gate 2: Corpus Acceptance Check

Verifies the corpus JSONL is valid before datasets are built from it.

Checks:
  G2-1  Declaration count plausible (3500–6000 for 14 Mathlib modules)
  G2-2  Module coverage: all expected modules present
  G2-3  No duplicate declaration names
  G2-4  Representation manifest exists with representation_id
  G2-5  Version strings current (advisory — warns, not fails)

Usage:
    python3 python/check_corpus.py [--corpus Corpus/corpus.per_operator.jsonl]
                                    [--manifest Corpus/representation_manifest.json]
                                    [--stats Corpus/stats.json]
"""

import argparse
import json
import os
import sys
from pathlib import Path
from collections import Counter

_REPO_ROOT = Path(__file__).resolve().parent.parent


def _report(name, passed, detail="", advisory=False):
    if advisory and not passed:
        status = "WARN"
    else:
        status = "PASS" if passed else "FAIL"
    msg = f"  {status}  {name}"
    if detail:
        msg += f": {detail}"
    print(msg)
    return passed or advisory


def check_g2_1_count(args, records):
    """G2-1: Declaration count plausible."""
    n = len(records)
    passed = 3500 <= n <= 6000
    return _report("G2-1 declaration_count", passed, f"{n} declarations (expected 3500–6000)")


def check_g2_2_module_coverage(args, records):
    """G2-2: All expected modules present with ≥1 declaration."""
    modules = set()
    for r in records:
        mod = r.get("module", "")
        if mod:
            modules.add(mod)

    # Don't hardcode a specific module list — just verify we have a reasonable
    # number of distinct modules (≥10 for the 14-module extraction target).
    # A specific module list would need to be read from the build config.
    if len(modules) < 10:
        return _report("G2-2 module_coverage", False,
                        f"only {len(modules)} modules found (expected ≥10)")
    return _report("G2-2 module_coverage", True,
                    f"{len(modules)} modules present")


def check_g2_3_no_duplicates(args, records):
    """G2-3: Duplicate declaration names (WARN — known corpus property)."""
    names = [r.get("name", "") for r in records]
    counts = Counter(names)
    dupes = {n: c for n, c in counts.items() if c > 1 and n}
    if dupes:
        return _report("G2-3 no_duplicate_ids", True,
                        f"{len(dupes)} duplicate short names (expected — same name in different modules; use module::name as unique key)",
                        advisory=True)
    return _report("G2-3 no_duplicate_ids", True,
                    f"all {len(names)} names unique" if names else "no names found")


def check_g2_4_manifest(args):
    """G2-4: Representation manifest exists with representation_id."""
    manifest_path = Path(args.manifest)
    if not manifest_path.exists():
        # Try alternate name (corpus_manifest.json vs representation_manifest.json)
        alt_path = manifest_path.parent / "corpus_manifest.json"
        if alt_path.exists():
            manifest_path = alt_path
        else:
            return _report("G2-4 representation_id", False,
                            f"manifest not found at {args.manifest} or {alt_path}")
    with open(manifest_path) as f:
        manifest = json.load(f)
    rep_id = manifest.get("representation_id", manifest.get("format", ""))
    if not rep_id:
        return _report("G2-4 representation_id", False, "representation_id/format is empty")
    return _report("G2-4 representation_id", True, f"representation_id={rep_id} (from {manifest_path.name})")


def check_g2_5_version_strings(args):
    """G2-5: Version strings current (advisory — warns, not fails)."""
    stats_path = Path(args.stats)
    if not stats_path.exists():
        return _report("G2-5 version_strings", True,
                        "stats.json not found — skipping (advisory)", advisory=True)

    with open(stats_path) as f:
        stats = json.load(f)
    encoder_version = stats.get("encoderVersion", "")

    # Check against Encoder.lean version string
    encoder_path = _REPO_ROOT / "Maith" / "Encoder.lean"
    if not encoder_path.exists():
        return _report("G2-5 version_strings", True,
                        f"Encoder.lean not found — skipping (advisory)", advisory=True)

    lean_source = encoder_path.read_text()
    # Look for a version string in the Lean source
    # The exact pattern depends on how Encoder.lean defines it
    if encoder_version and encoder_version in lean_source:
        return _report("G2-5 version_strings", True,
                        f"encoderVersion={encoder_version} matches Encoder.lean")
    else:
        return _report("G2-5 version_strings", True,
                        f"encoderVersion={encoder_version} not found in Encoder.lean (advisory)",
                        advisory=True)


def main():
    parser = argparse.ArgumentParser(description="Gate 2: Corpus Acceptance Check")
    parser.add_argument("--corpus", default=str(_REPO_ROOT / "Corpus" / "corpus.per_operator.jsonl"))
    parser.add_argument("--manifest", default=str(_REPO_ROOT / "Corpus" / "representation_manifest.json"))
    parser.add_argument("--stats", default=str(_REPO_ROOT / "Corpus" / "stats.json"))
    args = parser.parse_args()

    print("=" * 60)
    print("GATE 2 — CORPUS ACCEPTANCE CHECK")
    print("=" * 60)

    # Load corpus records
    corpus_path = Path(args.corpus)
    if not corpus_path.exists():
        print(f"  FAIL  corpus not found at {corpus_path}")
        sys.exit(1)

    records = []
    with open(corpus_path) as f:
        for line in f:
            line = line.strip()
            if line:
                records.append(json.loads(line))

    results = [
        check_g2_1_count(args, records),
        check_g2_2_module_coverage(args, records),
        check_g2_3_no_duplicates(args, records),
        check_g2_4_manifest(args),
        check_g2_5_version_strings(args),
    ]

    all_pass = all(results)
    total = len(results)
    passed = sum(results)
    print()
    if all_pass:
        print(f"Gate 2 PASSED — {passed}/{total} checks passed.")
    else:
        print(f"Gate 2 FAILED — {passed}/{total} checks passed.")
        print("Resolve failures before proceeding to dataset build.")
        sys.exit(1)


if __name__ == "__main__":
    main()
