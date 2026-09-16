#!/usr/bin/env python3
"""Mutation guard for the candidate-ledger validation (issue #27).

A test that passes on a broken validator is not a test. This disables each
validation rule in `candidates.py` one at a time, asserts the test suite DETECTS
it, and restores the file. If a fixture is ever weakened, this fails in CI instead
of silently regressing.

The mutation is applied to the real source and restored in a `finally`, so a
crash cannot leave the repo dirty (the same hazard the guard itself checks for).

Run: python3 axiom-rewrite/test_mutation_guard.py
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



HERE = Path(__file__).resolve().parent
REPO = HERE.parent
SRC = HERE / "candidates.py"
TEST = HERE / "test_candidates.py"

MUTATIONS = [
    ("compression gate-3 rule disabled",
     'if self.compression is not None and self.gates.get(3) != "pass":',
     'if False:'),
    ("reusable gate-5 rule disabled",
     'if self.reusable and self.gates.get(5) != "pass":',
     'if False:'),
    ("ordered-pipeline check disabled",
     'if earlier_fails:',
     'if False:'),
    ("invalid gate outcome accepted",
     'if outcome not in VALID_OUTCOMES:',
     'if False:'),
    ("duplicate-id check disabled",
     'if any(c.candidate_id == cand.candidate_id for c in existing):',
     'if False:'),
    ("required-field check disabled",
     'if not getattr(self, req):',
     'if False:'),
    ("append-only broken (file reopened for write)",
     'with path.open("a", encoding="utf-8") as f:',
     'with path.open("w", encoding="utf-8") as f:'),
]


def main() -> int:
    original = SRC.read_text()
    results = []
    try:
        for name, old, new in MUTATIONS:
            if old not in original:
                print(f"  [MISS] {name}: mutation anchor not found")
                results.append((name, False))
                continue
            SRC.write_text(original.replace(old, new, 1))
            r = subprocess.run([sys.executable, str(TEST)],
                               capture_output=True, text=True, cwd=str(REPO))
            detected = r.returncode != 0
            n = r.stdout.count("FAIL:") + r.stdout.count("ERROR:")
            print(f"  [{'DETECTED' if detected else 'MISSED'}] {name} "
                  f"(exit={r.returncode}, failures={n})")
            results.append((name, detected))
    finally:
        SRC.write_text(original)
    _purge_bytecode(SRC)

    missed = [n for n, ok in results if not ok]
    print()
    if missed:
        print(f"MUTATION GUARD FAILED: {len(missed)} mutation(s) not detected:")
        for m in missed:
            print(f"  - {m}")
        print("The ledger validation tests are vacuous for these rules.")
        return 1
    print(f"MUTATION GUARD OK: {len(results)}/{len(results)} mutations detected "
          f"-- validation fixtures are not vacuous.")
    return 0


if __name__ == "__main__":
    sys.exit(main())