#!/usr/bin/env python3
"""
check_results_gate.py

Decision-grade gate for A/B/C experiment outputs.

Checks:
  1) results.json exists for A/B/C
  2) all runs are full (smoke_test == false)
  3) shared eval sequence cap is identical across A/B/C
  4) shared eval example count is identical across A/B/C

Writes gate summary to runs/full_results_gate.json by default.
"""

import argparse
import json
from pathlib import Path


def load_result(path: Path) -> dict | None:
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def main() -> None:
    parser = argparse.ArgumentParser(description="Validate whether A/B/C results are decision-grade.")
    parser.add_argument("--runs-dir", default="runs/", help="Runs directory")
    parser.add_argument("--out", default=None, help="Output gate report path")
    args = parser.parse_args()

    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out) if args.out else runs_dir / "full_results_gate.json"

    variants = ["A", "B", "C"]
    results = {
        v: load_result(runs_dir / f"variant_{v}" / "results.json")
        for v in variants
    }

    checks = []
    missing = [v for v, r in results.items() if r is None]
    checks.append({
        "name": "all_results_present",
        "pass": not missing,
        "details": {"missing": missing},
    })

    present = {v: r for v, r in results.items() if r is not None}
    smoke_variants = [v for v, r in present.items() if r.get("smoke_test")]
    checks.append({
        "name": "all_runs_full_not_smoke",
        "pass": len(smoke_variants) == 0 and len(present) == 3,
        "details": {"smoke_variants": smoke_variants},
    })

    eval_caps = {v: r.get("eval_seq_len_cap") for v, r in present.items()}
    checks.append({
        "name": "shared_eval_seq_len_cap",
        "pass": len(set(eval_caps.values())) <= 1 and len(eval_caps) == 3,
        "details": eval_caps,
    })

    eval_counts = {v: r.get("eval_examples") for v, r in present.items()}
    checks.append({
        "name": "shared_eval_example_count",
        "pass": len(set(eval_counts.values())) <= 1 and len(eval_counts) == 3,
        "details": eval_counts,
    })

    passed = all(c["pass"] for c in checks)
    report = {
        "decision_grade": passed,
        "checks": checks,
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(report, f, indent=2)

    print(f"Gate report written: {out_path}")
    print(f"Decision-grade: {'YES' if passed else 'NO'}")
    for c in checks:
        mark = "PASS" if c["pass"] else "FAIL"
        print(f"  [{mark}] {c['name']}")

    raise SystemExit(0 if passed else 1)


if __name__ == "__main__":
    main()
