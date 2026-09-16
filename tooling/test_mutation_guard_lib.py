#!/usr/bin/env python3
"""Tests for the shared mutation-guard helper (issue #36).

The helper exists because the mutation-guard convention has a silent failure
mode: a guard that restores its source but leaves the mutated `.pyc` behind
corrupts a LATER process -- DEC-048 truncated the candidate ledger exactly this
way (`open("w")` surviving a restore). These tests pin the two properties that
prevent it:

  * `guarded_source` restores AND purges on the normal path.
  * `guarded_source` restores AND purges when the body raises -- the #36 gap,
    where the old guards purged OUTSIDE the try/finally, so a child timeout or a
    Ctrl-C skipped it.

Plus a **positive control**: an unguarded restore is shown to leave a stale
`.pyc` that a later process loads. Without that control the assertions above
could pass for the wrong reason (e.g. if `purge` were a no-op, or if CPython had
stopped validating caches by mtime).

Run: python3 tooling/test_mutation_guard_lib.py
"""

import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

from mutation_guard_lib import guarded_source, purge_bytecode, run_python  # noqa: E402

ORIGINAL = 'VALUE = "a"\n'
MUTATED = 'VALUE = "w"\n'  # deliberately the same length as ORIGINAL


def _clean_env():
    """Environment with bytecode writing *allowed*, so a child writes its `.pyc`."""
    env = dict(os.environ)
    env.pop("PYTHONDONTWRITEBYTECODE", None)
    return env


def _importer(d: Path) -> Path:
    """A script that imports `target` from `d` and prints its VALUE."""
    p = d / "imp.py"
    p.write_text(f"import sys\nsys.path.insert(0, {str(d)!r})\n"
                 "import target\nprint(target.VALUE)\n")
    return p


def _import_value(d: Path) -> str:
    """Run the importer the way a *later* process would (bytecode allowed)."""
    r = subprocess.run([sys.executable, str(_importer(d))],
                       capture_output=True, text=True, env=_clean_env())
    return r.stdout.strip()


def test_restores_and_purges_normal():
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        src = d / "target.py"
        src.write_text(ORIGINAL)
        with guarded_source(src) as original:
            assert original == ORIGINAL
            src.write_text(MUTATED)
            assert src.read_text() == MUTATED
        assert src.read_text() == ORIGINAL, "source not restored on the normal path"
        assert not (d / "__pycache__").exists(), "bytecode not purged"


def test_restores_and_purges_on_exception():
    """The #36 regression: a crash inside the guard must NOT skip the purge."""
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        src = d / "target.py"
        src.write_text(ORIGINAL)
        try:
            with guarded_source(src) as original:
                src.write_text(MUTATED)
                # A child imports the mutated module and caches it -- the exact
                # bytecode the old guards left behind.
                subprocess.run([sys.executable, str(_importer(d))],
                               capture_output=True, text=True, env=_clean_env())
                raise RuntimeError("simulated crash mid-guard")
        except RuntimeError:
            pass  # the point is what the helper left behind
        assert src.read_text() == ORIGINAL, "source not restored after an exception"
        assert not (d / "__pycache__").exists(), \
            "stale mutated bytecode survived an exception (#36)"
        assert _import_value(d) == "a", \
            "a later process loaded the mutation instead of the restored source"


def test_entry_purge_removes_preexisting_stale_bytecode():
    """A stale `.pyc` from an earlier crashed run must not survive entry."""
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        src = d / "target.py"
        src.write_text(ORIGINAL)
        # Leftover from a prior crashed guard: mutated pyc, source already restored.
        src.write_text(MUTATED)
        mtime = int(os.stat(src).st_mtime)
        subprocess.run([sys.executable, str(_importer(d))],
                       capture_output=True, text=True, env=_clean_env(),
                       check=True)
        src.write_text(ORIGINAL)
        os.utime(src, (mtime, mtime))
        assert _import_value(d) == "w", "precondition: stale pyc should load mutation"
        with guarded_source(src) as original:
            # Asserted INSIDE the block: the exit purge would mask a missing entry
            # purge, so checking afterwards would be vacuous.
            assert not (d / "__pycache__").exists(), \
                "entry purge left a pre-existing stale pyc in place"
            assert src.read_text() == original


def test_run_python_child_writes_no_bytecode():
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        (d / "target.py").write_text(ORIGINAL)
        r = run_python([str(_importer(d))], capture_output=True, text=True)
        assert r.stdout.strip() == "a"
        assert not (d / "__pycache__").exists(), \
            "run_python child wrote bytecode despite -B"


def test_positive_control_detector_sees_hazard():
    """Prove the assertions above can fail: without the purge, the `.pyc` is stale.

    Restores the source WITHOUT purging, pinning the mtime to the mutation's
    second so the cache stays valid -- then shows a later process loading the
    mutation. If this control stops reproducing, the hazard has changed shape and
    the other tests are no longer evidence.
    """
    with tempfile.TemporaryDirectory() as td:
        d = Path(td)
        src = d / "target.py"
        src.write_text(ORIGINAL)
        src.write_text(MUTATED)
        mtime = int(os.stat(src).st_mtime)
        subprocess.run([sys.executable, str(_importer(d))],
                       capture_output=True, text=True, env=_clean_env(),
                       check=True)
        # Restore the source but do NOT purge -- the pre-#36 bug.
        src.write_text(ORIGINAL)
        os.utime(src, (mtime, mtime))
        assert _import_value(d) == "w", \
            "control failed: an unguarded restore did not leave loadable stale bytecode"


def main() -> int:
    tests = [
        test_restores_and_purges_normal,
        test_restores_and_purges_on_exception,
        test_entry_purge_removes_preexisting_stale_bytecode,
        test_run_python_child_writes_no_bytecode,
        test_positive_control_detector_sees_hazard,
    ]
    failures = []
    for t in tests:
        try:
            t()
            print(f"  [OK]    {t.__name__}")
        except AssertionError as e:
            failures.append(t.__name__)
            print(f"  [FAIL]  {t.__name__}: {e}")
        except Exception as e:  # noqa: BLE001 - surface anything unexpected
            failures.append(t.__name__)
            print(f"  [ERROR] {t.__name__}: {e!r}")
    print()
    if failures:
        print(f"FAIL: {len(failures)}/{len(tests)} test(s) failed: {failures}")
        return 1
    print(f"PASS: {len(tests)} mutation-guard-helper tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
