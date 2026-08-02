#!/usr/bin/env python3
"""
watch_full_runs.py

Lightweight monitor for the sequential full A/B/C training log written by:
  runs/full_abc_runs.log

Usage:
    python3 python/watch_full_runs.py
    python3 python/watch_full_runs.py --log runs/full_abc_runs.log
"""

import argparse
import ast
from datetime import datetime, timedelta
try:
    from datetime import UTC
except ImportError:
    from datetime import timezone
    UTC = timezone.utc
import re
from pathlib import Path


VARIANTS = ("A", "B", "C")


def parse_status(log_text: str) -> dict:
    normalized = log_text.replace("\r", "\n")

    restart_idx = normalized.rfind("=== RESTART:")
    if restart_idx != -1:
        normalized = normalized[restart_idx:]

    lines = normalized.splitlines()

    done_variants: list[str] = []
    current_variant = None
    complete_line = None

    for line in lines:
        m_done = re.match(r"^\[done\] variant ([ABC]) ", line)
        if m_done:
            done_variants.append(m_done.group(1))
            continue
        m_variant = re.match(r"^Variant: ([ABC])$", line.strip())
        if m_variant:
            current_variant = m_variant.group(1)
            continue
        if line.startswith("[complete] "):
            complete_line = line

    progress_matches = list(re.finditer(r"(\d+)%\|.*?\|\s*(\d+)/(\d+)\s*\[", normalized))
    progress = None
    eta_utc = None
    eta_remaining = None
    if progress_matches:
        m = progress_matches[-1]
        progress = {
            "percent": int(m.group(1)),
            "step": int(m.group(2)),
            "total_steps": int(m.group(3)),
        }

    eta_matches = list(re.finditer(r"\|\s*\d+/\d+\s*\[[^\]]*?<([^,\]]+),", normalized))
    if eta_matches:
        remaining_text = eta_matches[-1].group(1).strip()
        parts = remaining_text.split(":")
        seconds = None
        try:
            if len(parts) == 3:
                h, m, s = (int(x) for x in parts)
                seconds = h * 3600 + m * 60 + s
            elif len(parts) == 2:
                m, s = (int(x) for x in parts)
                seconds = m * 60 + s
        except ValueError:
            seconds = None
        if seconds is not None:
            eta_remaining = remaining_text
            eta_utc = (datetime.now(UTC) + timedelta(seconds=seconds)).replace(microsecond=0).isoformat()

    last_train_loss = None
    last_eval_loss = None
    last_train_summary = None
    for line in lines:
        raw = line.strip()
        if not raw.startswith("{") or not raw.endswith("}"):
            continue
        try:
            payload = ast.literal_eval(raw)
        except (SyntaxError, ValueError):
            continue
        if not isinstance(payload, dict):
            continue
        if "loss" in payload:
            last_train_loss = payload
        if "eval_loss" in payload:
            last_eval_loss = payload
        if "train_runtime" in payload:
            last_train_summary = payload

    is_complete = complete_line is not None
    done_set = set(done_variants)
    pending = [v for v in VARIANTS if v not in done_set]
    if is_complete:
        run_state = "COMPLETE"
    elif current_variant:
        run_state = f"RUNNING (variant {current_variant})"
    elif done_variants:
        run_state = "PARTIAL"
    else:
        run_state = "STARTED"

    return {
        "run_state": run_state,
        "done_variants": done_variants,
        "pending_variants": pending,
        "current_variant": current_variant,
        "progress": progress,
        "eta_utc": eta_utc,
        "eta_remaining": eta_remaining,
        "last_train_loss": last_train_loss,
        "last_eval_loss": last_eval_loss,
        "last_train_summary": last_train_summary,
        "complete_line": complete_line,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description="Monitor full A/B/C training run progress.")
    parser.add_argument("--log", default="runs/full_abc_runs.log", help="Path to full run log file")
    args = parser.parse_args()

    log_path = Path(args.log)
    if not log_path.exists():
        print(f"Log not found: {log_path}")
        print("Start full runs first or pass --log with the correct path.")
        raise SystemExit(1)

    text = log_path.read_text(errors="replace")
    status = parse_status(text)

    print("=== Full A/B/C Run Status ===")
    print(f"Log: {log_path}")
    print(f"Size: {log_path.stat().st_size:,} bytes")
    print(f"State: {status['run_state']}")
    print(f"Done: {', '.join(status['done_variants']) if status['done_variants'] else '(none)'}")
    print(f"Pending: {', '.join(status['pending_variants']) if status['pending_variants'] else '(none)'}")

    if status["progress"] is not None:
        p = status["progress"]
        print(f"Progress: {p['percent']}% ({p['step']}/{p['total_steps']})")
    else:
        print("Progress: (not yet available)")
    if status["eta_utc"] is not None and status["eta_remaining"] is not None:
        print(f"ETA (from tqdm): {status['eta_utc']} (remaining {status['eta_remaining']})")
    else:
        print("ETA (from tqdm): (not yet available)")

    if status["last_train_loss"] is not None:
        t = status["last_train_loss"]
        print(f"Last train loss: loss={t.get('loss')} epoch={t.get('epoch')} step={t.get('step', '?')}")
    else:
        print("Last train loss: (not yet logged)")

    if status["last_eval_loss"] is not None:
        e = status["last_eval_loss"]
        print(f"Last eval loss: eval_loss={e.get('eval_loss')} epoch={e.get('epoch')} step={e.get('step', '?')}")
    else:
        print("Last eval loss: (not yet logged)")

    if status["last_train_summary"] is not None:
        s = status["last_train_summary"]
        print(
            "Last train summary: "
            f"runtime={s.get('train_runtime')} "
            f"loss={s.get('train_loss')} "
            f"epoch={s.get('epoch')}"
        )

    if status["complete_line"] is not None:
        print(status["complete_line"])


if __name__ == "__main__":
    main()
