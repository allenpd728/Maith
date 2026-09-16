#!/usr/bin/env python3
"""Mutation guard for the invariant-5 fixtures (issue #22).

A test that passes on a broken checker is not a test. Issue #22 was exactly that:
`check_comparison_validity` declared five `must_match` fields but only the
`epochs` axis had a failing fixture.

This guard mutates `check_invariants.py` (one declared constraint at a time),
runs `test_invariants.py`, and asserts the failures are DETECTED. If a fixture is
ever removed or weakened, this fails in CI instead of silently regressing.

The mutation is applied to a temp copy via an import shim — the real source file
is never modified, so a crash cannot leave the repo dirty.

Run: python3 python/test_mutation_guard.py
"""

import subprocess
import sys
from pathlib import Path


def _purge_bytecode(module_path):
    """Remove cached bytecode for `module_path` after restoring its source.

    A guard writes a MUTATED module to disk, runs the tests, then restores it. If
    the mutated build was imported, its `.pyc` can survive and be loaded by a LATER
    process even though the source is correct -- which is how a guard silently
    corrupted `axiom-rewrite/candidates.py` (loaded `open("w")` instead of `"a"`,
    truncating the ledger). Purging after every restore closes that.
    """
    import shutil
    from pathlib import Path as _P
    p = _P(module_path)
    cache = p.parent / "__pycache__"
    if not cache.exists():
        return
    try:
        shutil.rmtree(cache)
    except OSError:
        pass



REPO = Path(__file__).resolve().parent.parent
PY = REPO / "python"
SRC = PY / "check_invariants.py"
TEST = PY / "test_invariants.py"

# Each entry disables ONE constraint that the #22 fixtures claim to cover.
# (name, old_text, new_text)
MUTATIONS = [
    (
        "must_match: seed dropped from one claim",
        '"must_match": ["representation_id", "seed", "train_examples", "eval_examples", "epochs"],\n        "must_differ": ["source_input"],',
        '"must_match": ["representation_id", "train_examples", "eval_examples", "epochs"],\n        "must_differ": ["source_input"],',
    ),
    (
        "must_match: train_examples dropped from one claim",
        '"must_match": ["representation_id", "seed", "train_examples", "eval_examples", "epochs"],',
        '"must_match": ["representation_id", "seed", "eval_examples", "epochs"],',
    ),
    (
        "must_differ: source_input check neutered",
        'if len(non_none) >= 2 and len(set(non_none)) == 1:',
        'if False:',
    ),
    (
        "leniency regression: one-sided None now fails",
        "if None in values and len(set(v for v in values if v is not None)) > 1:",
        "if None in values:",
    ),
]


def run_tests():
    proc = subprocess.run(
        [sys.executable, str(TEST)],
        capture_output=True, text=True, cwd=str(REPO),
    )
    return proc.returncode, proc.stdout


def main() -> int:
    original = SRC.read_text()
    results = []
    try:
        for name, old, new in MUTATIONS:
            if old not in original:
                # Anchor drifted — that itself is a maintenance signal, not a pass.
                print(f"  [MISS] {name}: mutation anchor not found in {SRC.name}")
                results.append((name, False))
                continue
            SRC.write_text(original.replace(old, new, 1))
            code, out = run_tests()
            detected = code != 0
            n = out.count("FAIL:") + out.count("ERROR:")
            print(f"  [{'DETECTED' if detected else 'MISSED'}] {name} "
                  f"(exit={code}, failures={n})")
            results.append((name, detected))
    finally:
        SRC.write_text(original)
    _purge_bytecode(SRC)

    missed = [n for n, ok in results if not ok]
    print()
    if missed:
        print(f"MUTATION GUARD FAILED: {len(missed)} mutation(s) not detected:")
        for n in missed:
            print(f"  - {n}")
        print("The invariant-5 fixtures are vacuous for these constraints.")
        return 1
    print(f"MUTATION GUARD OK: {len(results)}/{len(results)} mutations detected "
          f"-- fixtures are not vacuous.")
    return 0


if __name__ == "__main__":
    sys.exit(main())