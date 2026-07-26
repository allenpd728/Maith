#!/usr/bin/env python3
"""
run_postrun_pipeline.py

One-command post-run pipeline for full A/B/C result publication checks.
Safe to run while training is active (will report failures until complete).
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
    parser = argparse.ArgumentParser(description="Run full post-run publication pipeline checks.")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    parser.add_argument(
        "--allow-incomplete",
        action="store_true",
        help="Return zero even when publish checks fail (useful before full runs complete).",
    )
    args = parser.parse_args()

    py = sys.executable
    steps = [
        ("Strict full comparison", [py, "python/compare_results.py", "--runs-dir", args.runs_dir, "--strict-full"]),
        ("Decision-grade gate", [py, "python/check_results_gate.py", "--runs-dir", args.runs_dir]),
        ("Publish summary", [py, "python/publish_results_summary.py", "--runs-dir", args.runs_dir]),
        ("Publish readiness", [py, "python/check_publish_readiness.py", "--runs-dir", args.runs_dir]),
        ("Artifact schema validation", [py, "python/validate_experiment_artifacts.py", "--runs-dir", args.runs_dir]),
        ("Representation matrix gate", [py, "python/check_representation_matrix_gate.py", "--runs-dir", args.runs_dir]),
        ("Decision log check", [py, "python/check_decision_log.py"]),
        ("Status snapshot refresh", [py, "python/full_run_status_snapshot.py", "--log", args.log, "--runs-dir", args.runs_dir]),
        ("Status report refresh", [py, "python/full_run_status_report.py", "--log", args.log, "--runs-dir", args.runs_dir]),
    ]

    results = [run_step(name, cmd) for name, cmd in steps]

    print("\n=== Post-run Pipeline Summary ===")
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
