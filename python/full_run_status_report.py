#!/usr/bin/env python3
"""
full_run_status_report.py

Generate a markdown status report for an active full A/B/C run.
Read-only with respect to training artifacts (safe during active training).
"""

import argparse
from pathlib import Path

from full_run_status_snapshot import build_snapshot


def _fmt_progress(status: dict) -> str:
    p = status.get("progress")
    if not p:
        return "n/a"
    return f"{p['percent']}% ({p['step']}/{p['total_steps']})"


def _fmt_eta(eta: dict) -> str:
    if not eta.get("available"):
        return "n/a"
    return eta["estimated_completion_utc"]


def build_markdown(snapshot: dict) -> str:
    status = snapshot["status"]
    eta = snapshot["eta"]
    gate = snapshot["gate"]
    publish = snapshot["publish_summary"]
    lines = [
        "# Full A/B/C Run Status",
        "",
        f"- Generated (UTC): `{snapshot['generated_at_utc']}`",
        f"- Log: `{snapshot['log_path']}`",
        f"- State: **{status['run_state']}**",
        f"- Done variants: `{', '.join(status.get('done_variants', [])) or '(none)'}`",
        f"- Pending variants: `{', '.join(status.get('pending_variants', [])) or '(none)'}`",
        f"- Progress: `{_fmt_progress(status)}`",
        f"- ETA (UTC): `{_fmt_eta(eta)}`",
        f"- Decision gate present: `{gate.get('present')}`",
        f"- Decision grade: `{gate.get('decision_grade')}`",
        f"- Publish summary present: `{publish.get('present')}`",
        f"- Publish decision grade: `{publish.get('decision_grade')}`",
        f"- Winner by perplexity: `{publish.get('winner_by_perplexity')}`",
    ]
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description="Write markdown status report for full A/B/C run.")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default="runs/full_run_status.md")
    args = parser.parse_args()

    log_path = Path(args.log)
    if not log_path.exists():
        print(f"Log not found: {log_path}")
        return 1

    snapshot = build_snapshot(log_path=log_path, runs_dir=Path(args.runs_dir))
    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(build_markdown(snapshot))

    print(f"Wrote markdown status report: {out_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
