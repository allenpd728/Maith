#!/usr/bin/env python3
"""
maith-run.py

Wrapper script that enforces a single-model-at-a-time rule across all maith-*
Ollama models. Acquires a lock file before invoking ollama, releases it on
exit (including on error or keyboard interrupt).

Usage:
    python3 local-models/maith-run.py maith-coder "write a pytest fixture for..."
    python3 local-models/maith-run.py maith-verifier "verify this claim: ..."
    python3 local-models/maith-run.py maith-checker  # interactive mode
    python3 local-models/maith-run.py --status       # check if a model is running

Valid model names: maith-coder, maith-docs, maith-checker, maith-verifier

Lock file: /tmp/maith-model.lock
  Contains: "model_name:pid" for the currently running model.
  Removed automatically when the model exits (clean exit, error, or Ctrl-C).
  On kill -9 the lock is left behind but detected as stale on the next run
  via PID liveness check.

If a model is already running, this script exits immediately with a non-zero
code and a clear message. It does not queue, wait, or retry.

Signal handling note:
  This script uses subprocess.run() (not os.exec*), so the Python process
  stays alive as a parent for the duration of the ollama child. The finally
  block fires on all exits EXCEPT kill -9 on this wrapper's PID. In that
  case, the stale lock is auto-cleared on the next invocation via PID check.

  kill -9 on the *ollama child* PID (not the wrapper) does not prevent lock
  release — subprocess.run() returns when the child exits, and the finally
  block fires normally.

PID liveness check:
  os.kill(pid, 0) is used to check whether the PID in the lock file is alive.
  Possible outcomes:
    - No exception         → process alive, same user. Lock is valid.
    - PermissionError      → process alive, different user/context. Lock is valid.
    - ProcessLookupError   → process dead. Lock is stale, cleared automatically.
    - Other OSError        → treated conservatively as "alive" to avoid false clears.
"""

import argparse
import os
import subprocess
import sys
import time
from pathlib import Path

LOCK_FILE = Path("/tmp/maith-model.lock")
VALID_MODELS = {"maith-coder", "maith-docs", "maith-checker", "maith-verifier"}


def get_pid_command(pid):
    """
    Return the base command name for a PID using ps.
    Returns None if the process doesn't exist or can't be read.
    """
    try:
        result = subprocess.run(
            ["ps", "-p", str(pid), "-o", "comm="],
            capture_output=True, text=True
        )
        if result.returncode != 0:
            return None
        return result.stdout.strip()
    except Exception:
        return None


def pid_is_our_process(pid, recorded_start_time):
    """
    Return True only if pid is alive AND is a Python process (our wrapper).
    The recorded_start_time is kept for documentation but we rely on the
    command name check, which is unambiguous: if the PID was reused by
    ollama or any non-Python process after kill -9, it won't match.
    """
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False  # definitively dead
    except PermissionError:
        pass  # alive, different user — continue to command check
    except OSError:
        return True  # unknown — assume alive conservatively

    # PID is alive — verify it's actually a Python process (our wrapper).
    # If kill -9 hit the wrapper and the PID was reused by ollama or llama-server,
    # the command name won't be 'Python' or 'python3'.
    cmd = get_pid_command(pid)
    if cmd is None:
        return False  # process vanished between kill(0) and ps
    cmd_lower = cmd.lower()
    is_python = "python" in cmd_lower
    return is_python


def read_lock():
    """
    Return (model_name, pid) from the lock file if it represents our live process.
    Return (None, None) if the lock file is absent, malformed, or stale.
    Clears stale locks automatically.
    """
    if not LOCK_FILE.exists():
        return None, None
    try:
        content = LOCK_FILE.read_text().strip()
        parts = content.split(":")
        if len(parts) < 2:
            LOCK_FILE.unlink(missing_ok=True)
            return None, None
        model = parts[0]
        pid = int(parts[1])
    except (ValueError, OSError):
        LOCK_FILE.unlink(missing_ok=True)
        return None, None

    if pid_is_our_process(pid, None):
        return model, pid
    else:
        print(
            f"[maith-run] Stale lock detected (model={model}, pid={pid} is dead or reused). Clearing.",
            file=sys.stderr,
        )
        LOCK_FILE.unlink(missing_ok=True)
        return None, None


def acquire_lock(model_name):
    """
    Attempt to acquire the lock for model_name.
    Returns True on success, False if another model is already running.
    Exits immediately if blocked — no queuing, no waiting.
    """
    active_model, active_pid = read_lock()
    if active_model is not None:
        print(
            f"[maith-run] BLOCKED: '{active_model}' is already running (pid {active_pid}).\n"
            f"[maith-run] Wait for it to finish and try again.\n"
            f"[maith-run] If it crashed and left a stale lock, run:\n"
            f"[maith-run]   rm {LOCK_FILE}\n"
            f"[maith-run] Or use --status to check.",
            file=sys.stderr,
        )
        return False
    try:
        pid = os.getpid()
        LOCK_FILE.write_text(f"{model_name}:{pid}")
    except OSError as e:
        print(f"[maith-run] ERROR: Could not write lock file: {e}", file=sys.stderr)
        return False
    return True


def release_lock():
    """
    Remove the lock file, but only if it still belongs to this process.
    Guards against accidentally clearing a lock written by a subsequent run.
    """
    try:
        content = LOCK_FILE.read_text().strip()
        parts = content.split(":", 2)
        if len(parts) >= 2:
            _, pid_str = parts[0], parts[1]
            if int(pid_str) == os.getpid():
                LOCK_FILE.unlink(missing_ok=True)
    except (ValueError, OSError):
        pass  # Lock already gone or unreadable — nothing to do


def status():
    """Print current lock status and exit."""
    active_model, active_pid = read_lock()
    if active_model is None:
        print("[maith-run] No model is currently running. Ready.")
    else:
        print(f"[maith-run] Active model: {active_model} (pid {active_pid})")


def main():
    parser = argparse.ArgumentParser(
        description="Run a maith-* Ollama model with a single-instance lock.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=f"Valid models: {', '.join(sorted(VALID_MODELS))}",
    )
    parser.add_argument(
        "model",
        nargs="?",
        help="Model to run.",
    )
    parser.add_argument(
        "prompt",
        nargs="?",
        default=None,
        help="Optional prompt. Omit for interactive mode.",
    )
    parser.add_argument(
        "--status",
        action="store_true",
        help="Show which model is currently running (if any) and exit.",
    )

    args = parser.parse_args()

    if args.status:
        status()
        sys.exit(0)

    if not args.model:
        parser.print_help()
        sys.exit(1)

    if args.model not in VALID_MODELS:
        print(
            f"[maith-run] ERROR: Unknown model '{args.model}'.\n"
            f"[maith-run] Valid models: {', '.join(sorted(VALID_MODELS))}",
            file=sys.stderr,
        )
        sys.exit(1)

    if not acquire_lock(args.model):
        sys.exit(1)

    cmd = ["ollama", "run", args.model]
    if args.prompt:
        cmd.append(args.prompt)

    print(f"[maith-run] Starting {args.model} (lock acquired, pid={os.getpid()})")
    exit_code = 1
    try:
        result = subprocess.run(cmd)
        exit_code = result.returncode
    except KeyboardInterrupt:
        print(f"\n[maith-run] Interrupted.", file=sys.stderr)
        exit_code = 130
    except FileNotFoundError:
        print(
            "[maith-run] ERROR: 'ollama' not found. Is ollama installed and on PATH?",
            file=sys.stderr,
        )
        exit_code = 1
    finally:
        release_lock()
        print(f"[maith-run] Lock released.")

    sys.exit(exit_code)


if __name__ == "__main__":
    main()
