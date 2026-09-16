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
    ("cmd_invariants uses bare relative default again",
     'ds_path = resolve_repo_path(args.datasets or DATASETS_DIR, kind="datasets")',
     'ds_path = Path(args.datasets or "datasets")'),
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