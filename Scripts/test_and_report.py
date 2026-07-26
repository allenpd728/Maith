#!/usr/bin/env python3
"""
test_and_report.py

Runs the full Maith test suite and prints a clean pass/fail summary.
Removes the manual "run lake build + tests, then report back" loop.

Stages:
  1. lake build tests       — compile the test binary
  2. .lake/build/bin/tests  — run Lean unit tests, capture output
  3. python golden examples — python3 python/test_golden_examples.py
  4. python round-trip      — python3 python/validate_roundtrip.py --sample 200

Exits 0 only if every stage passes. On failure, the failing output is
printed in full so you can act on it immediately without a second prompt.

Usage:
    python3 scripts/test_and_report.py
    python3 scripts/test_and_report.py --lean-only     # skip Python tests
    python3 scripts/test_and_report.py --python-only   # skip Lean tests
"""

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path


# ---------------------------------------------------------------------------
# Colours
# ---------------------------------------------------------------------------

BOLD  = "\033[1m"
GREEN = "\033[32m"
RED   = "\033[31m"
YELLOW = "\033[33m"
CYAN  = "\033[36m"
RESET = "\033[0m"


# ---------------------------------------------------------------------------
# Run helpers
# ---------------------------------------------------------------------------

def run_captured(cmd: list[str]) -> tuple[int, str, float]:
    """Run a command and return (returncode, combined output, elapsed seconds)."""
    t0 = time.time()
    result = subprocess.run(cmd, capture_output=True, text=True)
    elapsed = time.time() - t0
    output = result.stdout + result.stderr
    return result.returncode, output, elapsed


# ---------------------------------------------------------------------------
# Result accumulator
# ---------------------------------------------------------------------------

class Results:
    def __init__(self):
        self.stages: list[dict] = []

    def add(self, name: str, passed: bool, detail: str, elapsed: float) -> None:
        self.stages.append({
            "name": name,
            "passed": passed,
            "detail": detail,
            "elapsed": elapsed,
        })

    def print_summary(self) -> bool:
        """Print the summary table and return True if all passed."""
        print(f"\n{'═' * 60}")
        print(f"{BOLD}  Test Summary{RESET}")
        print(f"{'═' * 60}")
        all_passed = True
        for s in self.stages:
            icon  = f"{GREEN}✓{RESET}" if s["passed"] else f"{RED}✗{RESET}"
            label = f"{GREEN}{s['name']}{RESET}" if s["passed"] else f"{RED}{s['name']}{RESET}"
            print(f"  {icon}  {label:<40} {s['elapsed']:>5.1f}s")
            if not s["passed"]:
                all_passed = False
        print(f"{'═' * 60}")
        if all_passed:
            print(f"  {GREEN}{BOLD}All tests passed{RESET}")
        else:
            failed = [s["name"] for s in self.stages if not s["passed"]]
            print(f"  {RED}{BOLD}Failed: {', '.join(failed)}{RESET}")
        print(f"{'═' * 60}\n")
        return all_passed


# ---------------------------------------------------------------------------
# Stages
# ---------------------------------------------------------------------------

def stage_lean_build(results: Results) -> bool:
    """Compile the test binary with lake build tests."""
    print(f"\n{CYAN}{BOLD}── Lean: lake build tests ──{RESET}")
    code, out, elapsed = run_captured(["lake", "build", "tests"])
    passed = code == 0
    if not passed:
        print(out)
    else:
        # Print any warnings but don't fail
        warnings = [l for l in out.splitlines() if "warning" in l.lower()]
        if warnings:
            for w in warnings[:5]:
                print(f"  {YELLOW}{w}{RESET}")
        print(f"  {GREEN}Build succeeded{RESET}")
    results.add("lake build tests", passed, out, elapsed)
    return passed


def stage_lean_tests(results: Results) -> bool:
    """Run the compiled Lean test binary."""
    print(f"\n{CYAN}{BOLD}── Lean: run tests ──{RESET}")
    binary = Path(".lake/build/bin/tests")
    if not binary.exists():
        msg = "Test binary not found — did lake build tests succeed?"
        print(f"  {RED}{msg}{RESET}")
        results.add("Lean tests", False, msg, 0.0)
        return False

    code, out, elapsed = run_captured([str(binary)])

    # Parse pass/fail counts from test output
    total_passed = sum(int(m) for m in re.findall(r"Results:\s*(\d+)/\d+\s+passed", out))
    total_run    = sum(int(m) for m in re.findall(r"Results:\s*\d+/(\d+)\s+passed", out))
    failures     = re.findall(r"✗.+", out)

    passed = code == 0 and not failures
    print(out)

    summary = f"{total_passed}/{total_run} passed"
    if failures:
        summary += f", {len(failures)} failed"
    results.add("Lean tests", passed, summary, elapsed)
    return passed


def stage_python_golden(results: Results) -> bool:
    """Run python/test_golden_examples.py."""
    print(f"\n{CYAN}{BOLD}── Python: golden examples ──{RESET}")
    path = Path("python/test_golden_examples.py")
    if not path.exists():
        print(f"  {YELLOW}Skipping — python/test_golden_examples.py not found{RESET}")
        results.add("Python golden examples", True, "skipped (not found)", 0.0)
        return True

    code, out, elapsed = run_captured(["python3", str(path)])
    passed = code == 0
    print(out if not passed else out[:2000])  # full output on failure, truncate on pass
    results.add("Python golden examples", passed, out, elapsed)
    return passed


def stage_python_roundtrip(results: Results) -> bool:
    """Run python/validate_roundtrip.py --sample 200."""
    print(f"\n{CYAN}{BOLD}── Python: round-trip validation (sample 200) ──{RESET}")
    code, out, elapsed = run_captured(
        ["python3", "python/validate_roundtrip.py", "--sample", "200"]
    )
    passed = code == 0
    print(out if not passed else out[:2000])
    results.add("Python round-trip (200)", passed, out, elapsed)
    return passed


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Run the Maith test suite and print a clean summary."
    )
    parser.add_argument("--lean-only",   action="store_true", help="Skip Python tests")
    parser.add_argument("--python-only", action="store_true", help="Skip Lean tests")
    args = parser.parse_args()

    if not Path("lakefile.lean").exists():
        print(f"{RED}ERROR: run this from the Maith repo root{RESET}")
        sys.exit(1)

    results = Results()
    total_start = time.time()

    if not args.python_only:
        build_ok = stage_lean_build(results)
        if build_ok:
            stage_lean_tests(results)
        else:
            results.add("Lean tests", False, "skipped — build failed", 0.0)

    if not args.lean_only:
        stage_python_golden(results)
        stage_python_roundtrip(results)

    total_elapsed = time.time() - total_start
    print(f"  Total: {total_elapsed:.1f}s")

    all_passed = results.print_summary()
    sys.exit(0 if all_passed else 1)


if __name__ == "__main__":
    main()
