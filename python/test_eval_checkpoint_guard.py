#!/usr/bin/env python3
"""
test_eval_checkpoint_guard.py

Tests for the eval_completion.py stale-checkpoint consistency guard
(warn_if_checkpoint_stale). Guards against the bug documented in
docs/variant_c_eval_bug.md / docs/runs_audit.md where a run dir's
results.json describes a different (later) run than the checkpoint on disk.

CPU-only, synthetic temp dirs, < 3s. Run with:
    python3 python/test_eval_checkpoint_guard.py
"""

import os
import sys
import json
import time
import tempfile
import importlib.util
from pathlib import Path
from io import StringIO

sys.path.insert(0, "python")

_spec = importlib.util.spec_from_file_location(
    "eval_completion", os.path.join("python", "eval_completion.py")
)
_mod = importlib.util.module_from_spec(_spec)
# avoid importing torch/transformers at module load if not needed: the guard
# helpers are defined before those imports are *used*, but the module-level
# import attempt still runs. Guard against a missing-dep environment by
# execing only if imports succeed; otherwise skip.
try:
    _spec.loader.exec_module(_mod)
except SystemExit:
    print("SKIP: torch/transformers not available in this environment")
    sys.exit(0)

warn_if_checkpoint_stale = _mod.warn_if_checkpoint_stale
_checkpoint_mtime = _mod._checkpoint_mtime
_results_perplexity = _mod._results_perplexity

failures = []


def check(cond, msg):
    print(("  [OK] " if cond else "  [FAIL] ") + msg)
    if not cond:
        failures.append(msg)


def make_run(tmp, ckpt_offset_days=0.0, results_offset_days=0.0, with_results=True,
             with_weights=True, ppl=1.098):
    run = Path(tmp) / "variant_X"
    ckpt = run / "checkpoint-final"
    ckpt.mkdir(parents=True, exist_ok=True)
    now = time.time()
    if with_weights:
        wf = ckpt / "model.safetensors"
        wf.write_text("x")
        os.utime(wf, (now - ckpt_offset_days * 86400,) * 2)
    if with_results:
        rp = run / "results.json"
        rp.write_text(json.dumps({"eval_perplexity": ppl}))
        os.utime(rp, (now - results_offset_days * 86400,) * 2)
    return run, ckpt


def capture(fn):
    old = sys.stdout
    sys.stdout = StringIO()
    try:
        fn()
        return sys.stdout.getvalue()
    finally:
        sys.stdout = old


def test_stale_warns():
    print("\n== stale checkpoint (results 2d newer) warns ==")
    with tempfile.TemporaryDirectory() as tmp:
        # checkpoint 2 days older than results => results newer by 2d
        run, ckpt = make_run(tmp, ckpt_offset_days=2.0, results_offset_days=0.0)
        out = capture(lambda: warn_if_checkpoint_stale(run, ckpt, "C"))
        check("STALE CHECKPOINT WARNING" in out, f"warning emitted; got: {out!r}")
        check("newer" in out, "warning says 'newer'")


def test_consistent_no_warn():
    print("\n== consistent checkpoint (same day) does not warn ==")
    with tempfile.TemporaryDirectory() as tmp:
        run, ckpt = make_run(tmp, ckpt_offset_days=0.1, results_offset_days=0.0)
        out = capture(lambda: warn_if_checkpoint_stale(run, ckpt, "A"))
        check(out.strip() == "", f"no warning; got: {out!r}")


def test_missing_results_no_warn():
    print("\n== missing results.json does not warn (no crash) ==")
    with tempfile.TemporaryDirectory() as tmp:
        run, ckpt = make_run(tmp, with_results=False)
        out = capture(lambda: warn_if_checkpoint_stale(run, ckpt, "B"))
        check(out.strip() == "", "no warning, no crash when results.json absent")


def test_missing_weights_no_warn():
    print("\n== missing weights does not warn (no crash) ==")
    with tempfile.TemporaryDirectory() as tmp:
        run, ckpt = make_run(tmp, with_weights=False)
        out = capture(lambda: warn_if_checkpoint_stale(run, ckpt, "A"))
        check(out.strip() == "", "no warning, no crash when weights absent")


def test_checkpoint_mtime_helper():
    print("\n== _checkpoint_mtime / _results_perplexity helpers ==")
    with tempfile.TemporaryDirectory() as tmp:
        run, ckpt = make_run(tmp)
        check(_checkpoint_mtime(ckpt) is not None, "checkpoint mtime found")
        ppl, mt = _results_perplexity(run)
        check(ppl == 1.098, f"results perplexity read; got {ppl}")
        check(mt is not None, "results mtime read")


def main():
    test_stale_warns()
    test_consistent_no_warn()
    test_missing_results_no_warn()
    test_missing_weights_no_warn()
    test_checkpoint_mtime_helper()
    print()
    if failures:
        print(f"FAIL: {len(failures)} test(s) failed:")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    print("PASS: all eval checkpoint guard tests passed")


if __name__ == "__main__":
    main()
