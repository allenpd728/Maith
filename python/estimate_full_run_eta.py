#!/usr/bin/env python3
"""
estimate_full_run_eta.py

Estimate ETA for the active full A/B/C run from progress lines in
runs/full_abc_runs.log. Read-only and safe during training.
"""

import argparse
from typing import Optional, List
import datetime as dt
import re
from pathlib import Path


PROGRESS_RE = re.compile(r"(\d+)%\|.*?\|\s*(\d+)/(\d+)\s*\[")
START_RE = re.compile(r"^\[start\]\s+([0-9T:\-]+Z)")


def parse_start_time(lines: List[str]) -> Optional[dt.datetime]:
    for line in lines:
        m = START_RE.match(line.strip())
        if m:
            try:
                return dt.datetime.strptime(m.group(1), "%Y-%m-%dT%H:%M:%SZ").replace(tzinfo=dt.timezone.utc)
            except ValueError:
                return None
    return None


def parse_progress_points(text: str) -> List[tuple[int, int, int]]:
    points = []
    normalized = text.replace("\r", "\n")
    for m in PROGRESS_RE.finditer(normalized):
        pct = int(m.group(1))
        step = int(m.group(2))
        total = int(m.group(3))
        points.append((pct, step, total))
    return points


def format_td(td: dt.timedelta) -> str:
    sec = int(td.total_seconds())
    sign = "-" if sec < 0 else ""
    sec = abs(sec)
    h, rem = divmod(sec, 3600)
    m, s = divmod(rem, 60)
    return f"{sign}{h:02d}:{m:02d}:{s:02d}"


def main() -> int:
    parser = argparse.ArgumentParser(description="Estimate ETA for full A/B/C run.")
    parser.add_argument("--log", default="runs/full_abc_runs.log")
    args = parser.parse_args()

    log_path = Path(args.log)
    if not log_path.exists():
        print(f"Log not found: {log_path}")
        return 1

    text = log_path.read_text(errors="replace")
    lines = text.replace("\r", "\n").splitlines()
    start = parse_start_time(lines)
    points = parse_progress_points(text)
    if not points:
        print("No progress points found yet.")
        return 1

    pct, step, total = points[-1]
    now = dt.datetime.now(dt.timezone.utc)
    print("=== Full Run ETA ===")
    print(f"Log: {log_path}")
    print(f"Progress: {pct}% ({step}/{total})")

    if start is None:
        print("Start time unavailable; ETA cannot be computed.")
        return 0

    elapsed = now - start
    print(f"Elapsed (from [start]): {format_td(elapsed)}")
    if step <= 0:
        print("Insufficient progress for ETA.")
        return 0

    sec_per_step = elapsed.total_seconds() / step
    remaining_steps = max(total - step, 0)
    eta_seconds = sec_per_step * remaining_steps
    eta_td = dt.timedelta(seconds=eta_seconds)
    eta_time = now + eta_td

    print(f"Avg sec/step: {sec_per_step:.2f}")
    print(f"Remaining steps: {remaining_steps}")
    print(f"ETA remaining: {format_td(eta_td)}")
    print(f"Estimated completion (UTC): {eta_time.strftime('%Y-%m-%dT%H:%M:%SZ')}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
