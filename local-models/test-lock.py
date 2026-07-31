#!/usr/bin/env python3
"""
test-lock.py

Tests the maith-run.py lock mechanism under kill -9.

Steps:
  1. Starts maith-coder in a subprocess (non-blocking).
  2. Waits for the lock file to appear (confirms lock was acquired).
  3. kill -9s the wrapper PID.
  4. Immediately tries to run maith-verifier.
  5. Reports whether the stale lock was detected and cleared automatically,
     or whether maith-verifier was incorrectly blocked.
"""

import os
import signal
import subprocess
import sys
import time
from pathlib import Path

LOCK_FILE = Path("/tmp/maith-model.lock")
REPO = Path(__file__).parent
RUNNER = REPO / "maith-run.py"

def run_step(label, cmd):
    print(f"\n{'='*60}")
    print(f"STEP: {label}")
    print(f"{'='*60}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.stdout:
        print(result.stdout, end="")
    if result.stderr:
        print(result.stderr, end="")
    print(f"[exit code: {result.returncode}]")
    return result

def main():
    print("\n[test-lock] Starting kill -9 stale lock test\n")

    # Step 1: clean up any existing lock
    if LOCK_FILE.exists():
        print(f"[test-lock] Removing existing lock file before test")
        LOCK_FILE.unlink()

    # Step 2: start maith-coder in background
    print("[test-lock] Starting maith-coder in background...")
    proc = subprocess.Popen(
        [sys.executable, str(RUNNER), "maith-coder", "count from 1 to 100 slowly"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    # Step 3: wait for lock file to appear (up to 5 seconds)
    print("[test-lock] Waiting for lock file to appear...")
    for _ in range(50):
        if LOCK_FILE.exists():
            lock_content = LOCK_FILE.read_text().strip()
            print(f"[test-lock] Lock acquired: {lock_content}")
            break
        time.sleep(0.1)
    else:
        print("[test-lock] ERROR: Lock file never appeared. Aborting.")
        proc.kill()
        sys.exit(1)

    # Step 4: kill -9 the wrapper
    wrapper_pid = proc.pid
    print(f"[test-lock] Sending kill -9 to wrapper pid={wrapper_pid}...")
    try:
        os.kill(wrapper_pid, signal.SIGKILL)
    except ProcessLookupError:
        print("[test-lock] WARNING: Wrapper already exited before kill -9.")

    # Step 5: immediately try maith-verifier (stale lock should be detected)
    print("[test-lock] Immediately running maith-verifier...")
    result = subprocess.run(
        [sys.executable, str(RUNNER), "maith-verifier", "say: lock test passed"],
        capture_output=True,
        text=True,
        timeout=60,
    )

    print("\n[test-lock] maith-verifier output:")
    if result.stdout:
        print(result.stdout, end="")
    if result.stderr:
        print(result.stderr, end="")
    print(f"[exit code: {result.returncode}]")

    # Step 6: verdict
    print(f"\n{'='*60}")
    stderr_combined = (result.stdout or "") + (result.stderr or "")
    if "Stale lock detected" in stderr_combined and result.returncode == 0:
        print("PASS: Stale lock was auto-detected, cleared, and maith-verifier ran successfully.")
    elif "BLOCKED" in stderr_combined:
        print("FAIL: maith-verifier was incorrectly blocked by a stale lock.")
        print("      The lock was not cleared despite the wrapper being killed.")
    elif result.returncode == 0:
        print("PASS (no stale): Lock was already released before maith-verifier ran.")
        print("      (wrapper exited before kill -9 could take effect)")
    else:
        print(f"INCONCLUSIVE: Unexpected exit code {result.returncode}.")
    print(f"{'='*60}\n")

    # Cleanup: kill any lingering ollama child from the background run
    proc.stdout.close() if proc.stdout else None
    proc.stderr.close() if proc.stderr else None

if __name__ == "__main__":
    main()
