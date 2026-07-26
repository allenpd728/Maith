#!/usr/bin/env python3
"""
run_full_experiment.py

Run full (or smoke) A/B/C training sequentially and stream output to a single log.

Usage:
    python3 python/run_full_experiment.py
    python3 python/run_full_experiment.py --smoke-test
    python3 python/run_full_experiment.py --variants A,B,C --runs-dir runs/
"""

import argparse
import datetime as dt
import os
import subprocess
import sys
from pathlib import Path


def now_utc() -> str:
    return dt.datetime.now(dt.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def run_and_tee(cmd: list[str], log_file) -> None:
    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
    )
    assert proc.stdout is not None
    for line in proc.stdout:
        print(line, end="")
        log_file.write(line)
    code = proc.wait()
    if code != 0:
        raise RuntimeError(f"Command failed ({code}): {' '.join(cmd)}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Run sequential A/B/C training with shared logging.")
    parser.add_argument("--variants", default="A,B,C", help="Comma-separated list of variants (default: A,B,C)")
    parser.add_argument("--datasets", default="datasets/", help="Dataset directory for train.py")
    parser.add_argument("--runs-dir", default="runs/", help="Runs output directory")
    parser.add_argument("--log", default=None, help="Log path (default: <runs-dir>/full_abc_runs.log)")
    parser.add_argument("--smoke-test", action="store_true", help="Run train.py with --smoke-test")
    args = parser.parse_args()

    variants = [v.strip().upper() for v in args.variants.split(",") if v.strip()]
    if not variants:
        print("No variants selected.")
        raise SystemExit(1)
    for v in variants:
        if v not in {"A", "B", "C"}:
            print(f"Invalid variant: {v} (expected A/B/C)")
            raise SystemExit(1)

    runs_dir = Path(args.runs_dir)
    runs_dir.mkdir(parents=True, exist_ok=True)
    log_path = Path(args.log) if args.log else (runs_dir / "full_abc_runs.log")

    with open(log_path, "w") as log_file:
        header = f"[start] {now_utc()} full {'/'.join(variants)} {'smoke' if args.smoke_test else 'full'}\n"
        print(header, end="")
        log_file.write(header)
        log_file.flush()

        for variant in variants:
            cmd = [
                sys.executable,
                "python/train.py",
                "--variant",
                variant,
                "--datasets",
                args.datasets,
                "--out",
                os.path.join(args.runs_dir, f"variant_{variant}"),
            ]
            if args.smoke_test:
                cmd.append("--smoke-test")

            run_and_tee(cmd, log_file)
            done = f"[done] variant {variant} {now_utc()}\n"
            print(done, end="")
            log_file.write(done)
            log_file.flush()

        run_and_tee([sys.executable, "python/compare_results.py", "--runs-dir", args.runs_dir], log_file)
        end = f"[complete] {now_utc()} full {'/'.join(variants)} {'smoke' if args.smoke_test else 'full'}\n"
        print(end, end="")
        log_file.write(end)
        log_file.flush()


if __name__ == "__main__":
    main()
