#!/usr/bin/env python3
"""
finalize_full_experiment.py

Run post-training checks/reports in one command after full A/B/C completes.
Safe to run while training is active (reads state and reports failures).
"""

import argparse
import subprocess
import sys
from dataclasses import dataclass


@dataclass
class StepResult:
    name: str
    returncode: int


def run_step(name: str, cmd: list[str]) -> StepResult:
    print(f"\n=== {name} ===")
    print("$", " ".join(cmd))
    code = subprocess.run(cmd).returncode
    print(f"[{name}] exit={code}")
    return StepResult(name=name, returncode=code)


def main() -> int:
    parser = argparse.ArgumentParser(description="Finalize full A/B/C experiment outputs.")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="Do not fail overall if strict checks fail (useful while runs are still active).",
    )
    args = parser.parse_args()

    steps = [
        ("Strict full comparison", [sys.executable, "python/compare_results.py", "--runs-dir", args.runs_dir, "--strict-full"]),
        ("Decision-grade gate", [sys.executable, "python/check_results_gate.py", "--runs-dir", args.runs_dir]),
        ("Publish summary", [sys.executable, "python/publish_results_summary.py", "--runs-dir", args.runs_dir]),
        ("Representation audit (strict)", [sys.executable, "python/representation_audit.py", "--strict-nonempty"]),
        ("Representation matrix status", [sys.executable, "python/representation_matrix_status.py"]),
    ]

    results = [run_step(name, cmd) for name, cmd in steps]

    print("\n=== Finalization Summary ===")
    all_ok = True
    for r in results:
        ok = r.returncode == 0
        all_ok = all_ok and ok
        print(f"{'PASS' if ok else 'FAIL'}: {r.name}")

    if all_ok:
        print("Overall: PASS")
        return 0
    print("Overall: FAIL")
    return 0 if args.allow_incomplete else 1


if __name__ == "__main__":
    raise SystemExit(main())
