#!/usr/bin/env python3
"""
full_run_status_history.py

Append full-run status snapshots to a JSONL history file for trend tracking.
Safe during active training (read-only from run artifacts).
"""

import argparse
from typing import Optional
import json
from typing import Optional
from pathlib import Path

from full_run_status_snapshot import build_snapshot


def build_fingerprint(snapshot: dict) -> str:
    status = snapshot.get("status", {})
    progress = status.get("progress") or {}
    gate = snapshot.get("gate", {})
    publish = snapshot.get("publish_summary", {})
    done = ",".join(status.get("done_variants", []))
    return (
        f"{status.get('run_state')}|"
        f"{progress.get('step')}/{progress.get('total_steps')}|"
        f"done={done}|"
        f"gate={gate.get('decision_grade')}|"
        f"publish={publish.get('decision_grade')}"
    )


def read_last_record(path: Path) -> Optional[dict]:
    if not path.exists():
        return None
    lines = path.read_text(errors="replace").splitlines()
    if not lines:
        return None
    for line in reversed(lines):
        s = line.strip()
        if not s:
            continue
        try:
            return json.loads(s)
        except json.JSONDecodeError:
            continue
    return None


def read_last_line(path: Path) -> Optional[str]:
    if not path.exists():
        return None
    lines = path.read_text(errors="replace").splitlines()
    for line in reversed(lines):
        s = line.strip()
        if s:
            return s
    return None


def main() -> int:
    parser = argparse.ArgumentParser(description="Append run status snapshot to history JSONL.")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--history-out", default="runs/full_run_status_history.jsonl")
    parser.add_argument("--force", action="store_true", help="Append even if fingerprint did not change.")
    args = parser.parse_args()

    log_path = Path(args.log)
    if not log_path.exists():
        print(f"Log not found: {log_path}")
        return 1

    snapshot = build_snapshot(log_path=log_path, runs_dir=Path(args.runs_dir))
    fingerprint = build_fingerprint(snapshot)

    out_path = Path(args.history_out)
    last_line = read_last_line(out_path)
    last = read_last_record(out_path)
    if not args.force and last_line and f"\"fingerprint\": \"{fingerprint}\"" in last_line:
        print(f"No change since last history entry; skipped append: {out_path}")
        return 0
    if not args.force and last and last.get("fingerprint") == fingerprint:
        print(f"No change since last history entry; skipped append: {out_path}")
        return 0

    record = {
        "captured_at_utc": snapshot.get("generated_at_utc"),
        "fingerprint": fingerprint,
        "snapshot": snapshot,
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "a") as f:
        f.write(json.dumps(record) + "\n")

    print(f"Appended history entry: {out_path}")
    print(f"Fingerprint: {fingerprint}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
