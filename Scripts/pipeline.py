#!/usr/bin/env python3
"""
pipeline.py

Single command to run the full Maith pipeline:
  1. build corpus  (lake env ./.lake/build/bin/buildCorpus)
  2. validate      (python3 python/validate_roundtrip.py --all)
  3. report        (python3 python/corpus_report.py)
  4. build dataset (python3 python/build_dataset.py)
  5. spot check    (python3 python/spot_check.py)

Each stage prints a clear header and stops immediately on failure so you
know exactly which step broke and why. Pass --skip-corpus-build if the
corpus is already fresh and you only want to rebuild the datasets.

Usage:
    python3 scripts/pipeline.py                     # full run
    python3 scripts/pipeline.py --skip-corpus-build # skip lake build step
    python3 scripts/pipeline.py --dry-run           # print commands only
"""

import argparse
import subprocess
import sys
import time
from pathlib import Path


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

BOLD  = "\033[1m"
GREEN = "\033[32m"
RED   = "\033[31m"
CYAN  = "\033[36m"
RESET = "\033[0m"


def header(title: str) -> None:
    bar = "─" * (len(title) + 4)
    print(f"\n{CYAN}{BOLD}{bar}{RESET}")
    print(f"{CYAN}{BOLD}  {title}{RESET}")
    print(f"{CYAN}{BOLD}{bar}{RESET}\n")


def run(cmd: list[str], dry_run: bool = False) -> None:
    """Run a command, stream output, and raise on non-zero exit."""
    display = " ".join(cmd)
    print(f"  $ {display}")
    if dry_run:
        return
    t0 = time.time()
    result = subprocess.run(cmd, text=True)
    elapsed = time.time() - t0
    if result.returncode != 0:
        print(f"\n{RED}{BOLD}✗ Failed (exit {result.returncode}) after {elapsed:.1f}s{RESET}")
        print(f"  Command: {display}")
        sys.exit(result.returncode)
    print(f"\n{GREEN}✓ Done in {elapsed:.1f}s{RESET}")


# ---------------------------------------------------------------------------
# Stages
# ---------------------------------------------------------------------------

def stage_build_corpus(dry_run: bool) -> None:
    header("1 / 5  Build corpus  (lake)")
    # Ensure the binary exists
    binary = Path(".lake/build/bin/buildCorpus")
    if not binary.exists() and not dry_run:
        print("  Binary not found — running lake build first ...")
        run(["lake", "build", "buildCorpus"], dry_run)
    run(["lake", "env", str(binary)], dry_run)


def stage_validate(dry_run: bool) -> None:
    header("2 / 5  Round-trip validation")
    run(["python3", "python/validate_roundtrip.py", "--all"], dry_run)


def stage_report(dry_run: bool) -> None:
    header("3 / 5  Corpus report  (node-type frequencies)")
    run(["python3", "python/corpus_report.py"], dry_run)


def stage_build_dataset(dry_run: bool) -> None:
    header("4 / 5  Build datasets  (A / B / C variants)")
    run(["python3", "python/build_dataset.py"], dry_run)


def stage_spot_check(dry_run: bool) -> None:
    header("5 / 5  Spot check")
    run(["python3", "python/spot_check.py", "Corpus/corpus.jsonl"], dry_run)


def stage_eval_completion(dry_run: bool) -> None:
    header("6 / 6  Completion accuracy  (requires trained models in runs/)")
    run(["python3", "python/eval_completion.py"], dry_run)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Run the full Maith pipeline end-to-end."
    )
    parser.add_argument(
        "--skip-corpus-build",
        action="store_true",
        help="Skip the lake corpus build step (corpus.jsonl already fresh)",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print commands without executing them",
    )
    parser.add_argument(
        "--eval-completion",
        action="store_true",
        help="Also run completion accuracy eval (requires trained models in runs/)",
    )
    args = parser.parse_args()

    # Must be run from the repo root
    if not Path("lakefile.lean").exists():
        print(f"{RED}ERROR: run this from the Maith repo root{RESET}")
        sys.exit(1)

    total_start = time.time()

    if not args.skip_corpus_build:
        stage_build_corpus(args.dry_run)
    else:
        print(f"\n{CYAN}Skipping corpus build (--skip-corpus-build){RESET}")

    stage_validate(args.dry_run)
    stage_report(args.dry_run)
    stage_build_dataset(args.dry_run)
    stage_spot_check(args.dry_run)

    if args.eval_completion:
        stage_eval_completion(args.dry_run)

    elapsed = time.time() - total_start
    print(f"\n{GREEN}{BOLD}✓ Pipeline complete in {elapsed/60:.1f} minutes{RESET}\n")


if __name__ == "__main__":
    main()
