#!/usr/bin/env python3
"""
run_status_dashboard.py

One-shot read-only dashboard for active full A/B/C runs.
Runs monitor/snapshot/report/history/doc checks in sequence.
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
    parser = argparse.ArgumentParser(description="Run status/health dashboard helpers for active full runs.")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--strict", action="store_true", help="Return non-zero if any step fails.")
    args = parser.parse_args()

    py = sys.executable
    steps = [
        ("Watch full runs", [py, "python/watch_full_runs.py", "--log", args.log]),
        ("Estimate full-run ETA", [py, "python/estimate_full_run_eta.py", "--log", args.log]),
        (
            "Write status snapshot JSON",
            [py, "python/full_run_status_snapshot.py", "--log", args.log, "--runs-dir", args.runs_dir],
        ),
        (
            "Write status markdown report",
            [py, "python/full_run_status_report.py", "--log", args.log, "--runs-dir", args.runs_dir],
        ),
        (
            "Append status history",
            [py, "python/full_run_status_history.py", "--log", args.log, "--runs-dir", args.runs_dir],
        ),
        ("Check docs consistency", [py, "python/check_docs_consistency.py"]),
    ]

    results = [run_step(name, cmd) for name, cmd in steps]

    print("\n=== Dashboard Summary ===")
    all_ok = True
    for r in results:
        ok = r.returncode == 0
        all_ok = all_ok and ok
        print(f"{'PASS' if ok else 'FAIL'}: {r.name}")

    if all_ok:
        print("Overall: PASS")
        return 0
    print("Overall: FAIL")
    return 1 if args.strict else 0


if __name__ == "__main__":
    raise SystemExit(main())
