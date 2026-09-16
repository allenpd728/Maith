#!/usr/bin/env python3
"""Mutation guard for the #24 gate-path fix.

Disables each guarantee in turn and asserts the tests DETECT it. Same convention as
the other guards in this repo (python/test_mutation_guard.py,
python/test_invariants_8_9_guard.py, axiom-rewrite/test_mutation_guard.py).

Run: python3 python/test_manage_gate_paths_guard.py
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
SRC = REPO / "python" / "manage.py"
TEST = REPO / "python" / "test_manage_gate_paths.py"

MUTATIONS = [
    ("relative paths resolve against cwd again (the original bug)",
     'path = raw if raw.is_absolute() else (REPO / raw)',
     'path = raw'),
    ("missing dir silently accepted (no existence check)",
     'if not path.exists():',
     'if False:'),
    ("file-instead-of-dir not rejected",
     'if not path.is_dir():',
     'if False:'),
    ("gate header dropped",
     '    print("GATE TARGET")',
     '    pass  #'),
    ("provenance no longer printed in header",
     '    bits = [f"datasets={path}"]',
     '    bits = []'),
    # Well-formed: keeps the try/except shape but reintroduces the cwd-relative
    # default. (A first attempt removed the try too, which only produced a
    # SyntaxError -- that would have proved nothing about the tests.)
    # Mutate BOTH commands' dataset resolution. Mutating only cmd_invariants left
    # cmd_gate's `runs` resolution intact, so the CLI still errored on a bad path
    # and the guard reported a false GAP. The property under test is "a relative
    # path is REPO-anchored", and both callers must be exercised.
    ("dataset resolution no longer REPO-anchored (both commands)",
     'ds_path = resolve_repo_path(args.datasets or DATASETS_DIR, kind="datasets")',
     'ds_path = Path(args.datasets or "datasets")'),
    ("runs resolution no longer REPO-anchored (gate)",
     'runs_path = resolve_repo_path(args.runs or RUNS_DIR, kind="runs")',
     'runs_path = Path(args.runs or "runs")'),
]


def main() -> int:
    original = SRC.read_text()
    results = []
    try:
        for name, old, new in MUTATIONS:
            if old not in original:
                print(f"  [MISS] {name}: anchor not found")
                results.append((name, False))
                continue
            # Replace ALL occurrences: `ds_path = resolve_repo_path(...)` appears in
            # both cmd_gate and cmd_invariants, and the property must be exercised
            # in both.
            SRC.write_text(original.replace(old, new))
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

    control = subprocess.run([sys.executable, str(TEST)], capture_output=True,
                             text=True, cwd=str(REPO)).returncode == 0
    print(f"\n  [{'PASS' if control else 'FAIL'}] control: unmutated tests pass")

    missed = [n for n, ok in results if not ok]
    print()
    if missed or not control:
        print(f"MUTATION GUARD FAILED: {len(missed)} undetected; control_ok={control}")
        for m in missed:
            print(f"  - {m}")
        return 1
    print(f"MUTATION GUARD OK: {len(results)}/{len(results)} detected")
    return 0


if __name__ == "__main__":
    sys.exit(main())