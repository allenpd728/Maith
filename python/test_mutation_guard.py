#!/usr/bin/env python3
"""Mutation guard for the invariant-5 fixtures (issue #22).

A test that passes on a broken checker is not a test. Issue #22 was exactly that:
`check_comparison_validity` declared five `must_match` fields but only the
`epochs` axis had a failing fixture.

This guard mutates `check_invariants.py` (one declared constraint at a time),
runs `test_invariants.py`, and asserts the failures are DETECTED. If a fixture is
ever removed or weakened, this fails in CI instead of silently regressing.

The mutation is applied to the real source; `guarded_source` restores it and
purges the bytecode cache on EVERY exit path (normal, exception, timeout), so a
crash cannot leave a stale mutated `.pyc` for a later process to load (#36).

Run: python3 python/test_mutation_guard.py
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tooling"))
from mutation_guard_lib import guarded_source, run_python  # noqa: E402

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
    proc = run_python(
        [str(TEST)],
        capture_output=True, text=True, cwd=str(REPO),
    )
    return proc.returncode, proc.stdout


def main() -> int:
    results = []
    with guarded_source(SRC) as original:
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