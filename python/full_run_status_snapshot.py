#!/usr/bin/env python3
"""
full_run_status_snapshot.py

Write a machine-readable snapshot of full-run status:
  - parsed run state/progress/loss markers
  - ETA estimate
  - gate and publish-summary presence

Safe to run during active training (read-only).
"""

import argparse
import datetime as dt
import json
from pathlib import Path

from estimate_full_run_eta import parse_progress_points, parse_start_time
from watch_full_runs import parse_status


def load_json(path: Path):
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def build_eta(log_text: str) -> dict:
    normalized = log_text.replace("\r", "\n")
    lines = normalized.splitlines()
    points = parse_progress_points(log_text)
    start = parse_start_time(lines)
    if not points or start is None:
        return {"available": False}

    _, step, total = points[-1]
    now = dt.datetime.now(dt.timezone.utc)
    if step <= 0:
        return {"available": False}
    elapsed = now - start
    sec_per_step = elapsed.total_seconds() / step
    remaining_steps = max(total - step, 0)
    eta_seconds = sec_per_step * remaining_steps
    eta_time = now + dt.timedelta(seconds=eta_seconds)
    return {
        "available": True,
        "start_utc": start.strftime("%Y-%m-%dT%H:%M:%SZ"),
        "elapsed_seconds": int(elapsed.total_seconds()),
        "seconds_per_step": round(sec_per_step, 3),
        "remaining_steps": remaining_steps,
        "eta_seconds": int(eta_seconds),
        "estimated_completion_utc": eta_time.strftime("%Y-%m-%dT%H:%M:%SZ"),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description="Export full-run status snapshot JSON.")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default="runs/full_run_status.json")
    args = parser.parse_args()

    log_path = Path(args.log)
    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out)

    if not log_path.exists():
        print(f"Log not found: {log_path}")
        return 1

    text = log_path.read_text(errors="replace")
    status = parse_status(text)
    eta = build_eta(text)
    gate = load_json(runs_dir / "full_results_gate.json")
    publish_summary = load_json(runs_dir / "publish_results_summary.json")

    snapshot = {
        "generated_at_utc": dt.datetime.now(dt.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "log_path": str(log_path),
        "status": status,
        "eta": eta,
        "gate": {
            "present": gate is not None,
            "decision_grade": gate.get("decision_grade") if gate else None,
        },
        "publish_summary": {
            "present": publish_summary is not None,
            "decision_grade": publish_summary.get("decision_grade") if publish_summary else None,
            "winner_by_perplexity": publish_summary.get("winner_by_perplexity") if publish_summary else None,
        },
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(snapshot, f, indent=2)
        f.write("\n")

    print(f"Wrote status snapshot: {out_path}")
    print(f"Run state: {status['run_state']}")
    if status.get("progress"):
        p = status["progress"]
        print(f"Progress: {p['percent']}% ({p['step']}/{p['total_steps']})")
    if eta.get("available"):
        print(f"ETA UTC: {eta['estimated_completion_utc']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
