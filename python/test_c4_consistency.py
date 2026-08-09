#!/usr/bin/env python3
"""
test_c4_consistency.py

Guards against Python/Lean skew in the C4 GEN module bucketing.

`bucket_from_module` in build_dataset.py hand-replicates `bucketFromModule` in
Maith/MetaExtractor.lean. If the Lean mapping changes and the Python side is
not updated (or vice versa), the token stream will bucket gen:* tokens
inconsistently with the IR design. This test parses the Lean match arms from
MetaExtractor.lean and asserts the Python function produces identical results
for every case the Lean side defines.

CPU-only, < 3s. Run with:
    python3 python/test_c4_consistency.py
Non-zero exit = mismatch detected.
"""

import os
import re
import sys
import importlib.util
from pathlib import Path

sys.path.insert(0, "python")

failures = []


def check(cond, msg):
    print(("  [OK] " if cond else "  [FAIL] ") + msg)
    if not cond:
        failures.append(msg)


# load build_dataset.bucket_from_module
_spec = importlib.util.spec_from_file_location(
    "build_dataset", os.path.join("python", "build_dataset.py")
)
_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)
bucket_from_module = _mod.bucket_from_module


def parse_lean_buckets(lean_path: str) -> dict:
    """Extract the second-component -> GEN_* mapping from bucketFromModule.

    The Lean function has a block like:
        | "ALGEBRA"   => "GEN_ALGEBRA"
        | "ORDER"     => "GEN_ORDER"
        ...
    and a first-component block:
        | "LEAN" => "GEN_LEAN"
    Returns {component: bucket} for both blocks.
    """
    src = Path(lean_path).read_text()
    # isolate the bucketFromModule function body
    m = re.search(r"def bucketFromModule.*?(?=\n  private def |\nprivate def |\n@end)", src, re.S)
    if not m:
        return {}
    body = m.group(0)
    pairs = {}
    for mm in re.finditer(r'\|\s*"([A-Z_]+)"\s*=>\s*"(GEN_[A-Z_]+)"', body):
        pairs[mm.group(1)] = mm.group(2)
    return pairs


def main():
    lean_path = os.path.join("Maith", "MetaExtractor.lean")
    print("== C4 Python/Lean consistency ==")

    if not os.path.exists(lean_path):
        print(f"  [SKIP] {lean_path} not found (Lean source absent)")
        sys.exit(0)

    lean_pairs = parse_lean_buckets(lean_path)
    check(bool(lean_pairs), f"parsed Lean bucket cases ({len(lean_pairs)} found)")
    if not lean_pairs:
        print("  [SKIP] could not parse Lean match arms; skipping parity check")
        sys.exit(0)

    print(f"  Lean defines {len(lean_pairs)} component->bucket pairs")

    # For each Lean second-component (ALGEBRA, ORDER, ...) build a Mathlib
    # module that exercises it and check Python agrees.
    mismatches = []
    for component, want_bucket in lean_pairs.items():
        # Skip non-second-component entries we can't turn into a Mathlib.<x> module.
        # The Lean second-component block keys: ALGEBRA, ORDER, TOPOLOGY, ANALYSIS,
        # LOGIC, TACTIC, DATA, COMBINATORICS. First-component keys: LEAN, INIT, STD.
        if component in ("LEAN", "INIT", "STD"):
            mod = component.title() + ".X"
        else:
            mod = f"Mathlib.{component.title()}.Something"
        got = bucket_from_module(mod)
        if got != want_bucket:
            mismatches.append((mod, component, want_bucket, got))
        check(got == want_bucket, f"{mod:42} Lean={want_bucket} Python={got}")

    # also check the default fallbacks the Lean code defines (GEN_MATHLIB)
    check(bucket_from_module("Mathlib.Unknown.Area") == "GEN_MATHLIB",
          "unknown Mathlib area -> GEN_MATHLIB (Lean default)")
    check(bucket_from_module("") == "GEN_MATHLIB",
          "empty module -> GEN_MATHLIB (Lean default)")

    print()
    if failures:
        print(f"FAIL: {len(failures)} mismatch(es) — Python/Lean bucketing skew detected:")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    print("PASS: Python bucket_from_module matches Lean bucketFromModule for all parsed cases")


if __name__ == "__main__":
    main()
