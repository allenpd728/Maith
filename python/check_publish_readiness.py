#!/usr/bin/env python3
"""
check_publish_readiness.py

Read-only readiness check for publishing full A/B/C results.
Writes runs/publish_readiness.json by default.
"""

import argparse
from typing import Optional
import json
from pathlib import Path


def load_json(path: Path) -> Optional[dict]:
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def main() -> int:
    parser = argparse.ArgumentParser(description="Check whether full-run artifacts are ready for publishing.")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default=None, help="Readiness report path (default: <runs-dir>/publish_readiness.json)")
    args = parser.parse_args()

    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out) if args.out else (runs_dir / "publish_readiness.json")

    required_files = [
        runs_dir / "variant_A" / "results.json",
        runs_dir / "variant_B" / "results.json",
        runs_dir / "variant_C" / "results.json",
        runs_dir / "loss_curves.json",
        runs_dir / "full_results_gate.json",
        runs_dir / "publish_results_summary.json",
    ]
    missing = [str(p) for p in required_files if not p.exists()]

    gate = load_json(runs_dir / "full_results_gate.json")
    summary = load_json(runs_dir / "publish_results_summary.json")

    checks = [
        {"name": "required_files_present", "pass": len(missing) == 0, "details": {"missing": missing}},
        {
            "name": "decision_gate_passed",
            "pass": bool(gate and gate.get("decision_grade")),
            "details": {"decision_grade": gate.get("decision_grade") if gate else None},
        },
        {
            "name": "publish_summary_decision_grade",
            "pass": bool(summary and summary.get("decision_grade")),
            "details": {"decision_grade": summary.get("decision_grade") if summary else None},
        },
        {
            "name": "publish_summary_has_winner",
            "pass": bool(summary and summary.get("winner_by_perplexity")),
            "details": {"winner_by_perplexity": summary.get("winner_by_perplexity") if summary else None},
        },
    ]

    ready = all(c["pass"] for c in checks)
    report = {
        "ready_to_publish": ready,
        "checks": checks,
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")

    print(f"Readiness report written: {out_path}")
    print(f"Ready to publish: {'YES' if ready else 'NO'}")
    for c in checks:
        mark = "PASS" if c["pass"] else "FAIL"
        print(f"  [{mark}] {c['name']}")

    return 0 if ready else 1


if __name__ == "__main__":
    raise SystemExit(main())
