#!/usr/bin/env python3
"""Tests for `manage.py gate` path resolution (issue #24).

The hazard: `gate --datasets <path>` used the path verbatim, so a RELATIVE path
resolved against the caller's cwd — silently gating a different tree if one
existed there. `cmd_invariants` was worse: it used bare `"datasets"` as both the
default and the user value, i.e. always cwd-relative.

These tests pin the three guarantees:
  1. relative paths resolve against REPO, not cwd
  2. a missing directory is a hard error (exit 2), before any gate runs
  3. the header states the resolved absolute path + provenance

Run: python3 python/test_manage_gate_paths.py
"""

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
MANAGE = REPO / "python" / "manage.py"
sys.path.insert(0, str(REPO / "python"))

import manage  # noqa: E402


def _run(*args, cwd=None):
    return subprocess.run([sys.executable, str(MANAGE), *args],
                          capture_output=True, text=True, cwd=str(cwd or REPO))


# ---------------------------------------------------------------------------
# resolve_repo_path — the unit
# ---------------------------------------------------------------------------

def test_relative_resolves_against_repo_not_cwd():
    """The core fix: a relative path means REPO/datasets, from anywhere."""
    expected = (REPO / "datasets").resolve()
    got = manage.resolve_repo_path("datasets", kind="datasets")
    assert got == expected, f"relative resolved to {got}, expected {expected}"

    # And it must be cwd-independent: chdir elsewhere, same answer.
    old = os.getcwd()
    try:
        os.chdir(tempfile.gettempdir())
        again = manage.resolve_repo_path("datasets", kind="datasets")
    finally:
        os.chdir(old)
    assert again == expected, f"cwd-dependent resolution: {again} != {expected}"
    print("PASS: test_relative_resolves_against_repo_not_cwd")


def test_absolute_path_is_honoured():
    got = manage.resolve_repo_path(str(REPO / "datasets"), kind="datasets")
    assert got == (REPO / "datasets").resolve(), got
    print("PASS: test_absolute_path_is_honoured")


def test_missing_dir_raises():
    """A missing directory must be a hard error, not a silent default."""
    try:
        manage.resolve_repo_path("datasets_DOES_NOT_EXIST", kind="datasets")
    except FileNotFoundError as e:
        assert "not found" in str(e), e
        print("PASS: test_missing_dir_raises")
        return
    raise AssertionError("missing dir did not raise")


def test_file_instead_of_dir_raises():
    try:
        manage.resolve_repo_path("README.md", kind="datasets")
    except NotADirectoryError:
        print("PASS: test_file_instead_of_dir_raises")
        return
    raise AssertionError("a file path did not raise NotADirectoryError")


# ---------------------------------------------------------------------------
# describe_dataset_dir — the header
# ---------------------------------------------------------------------------

def test_describe_reports_real_provenance():
    """The header must name the representation actually in the tree.

    Regression guard: my first implementation read train_manifest.json (which is a
    per-example LIST, not a summary) and printed "manifest=unreadable".
    """
    desc = manage.describe_dataset_dir(REPO / "datasets")
    assert "datasets=" in desc, desc
    assert "representation_id=" in desc, f"no representation_id in header: {desc}"
    assert "unreadable" not in desc and "absent" not in desc, desc

    manifest = json.loads((REPO / "datasets" / "representation_manifest.json").read_text())
    assert manifest["representation_id"] in desc, (manifest["representation_id"], desc)
    print(f"PASS: test_describe_reports_real_provenance")
    print(f"      {desc}")


def test_describe_absent_manifest_is_explicit():
    with tempfile.TemporaryDirectory() as d:
        desc = manage.describe_dataset_dir(Path(d))
        assert "representation_manifest=absent" in desc, desc
    print("PASS: test_describe_absent_manifest_is_explicit")


# ---------------------------------------------------------------------------
# CLI behaviour (the DoD's missing-dir case)
# ---------------------------------------------------------------------------

def test_cli_missing_dir_fails_early_exit_2():
    r = _run("gate", "dataset", "--datasets", "datasets_NOPE")
    assert r.returncode == 2, f"expected exit 2, got {r.returncode}\n{r.stdout}\n{r.stderr}"
    assert "not found" in r.stderr, r.stderr
    # Must fail BEFORE any gate output, not after a partial run.
    assert "Gate 3" not in r.stdout, f"a gate ran despite the bad path:\n{r.stdout}"
    print("PASS: test_cli_missing_dir_fails_early_exit_2")


def test_cli_header_is_self_evidencing():
    r = _run("gate", "dataset")
    assert "GATE TARGET" in r.stdout, r.stdout[:400]
    # Must be the RESOLVED ABSOLUTE path, not a bare relative string like "datasets".
    assert f"datasets={REPO / 'datasets'}" in r.stdout, \
        f"header does not show the resolved absolute path:\n{r.stdout[:400]}"
    assert "representation_id=" in r.stdout, r.stdout[:400]
    print("PASS: test_cli_header_is_self_evidencing")


def test_cli_from_other_cwd_uses_repo_tree():
    """Running from elsewhere must gate the REPO tree, not fail on a missing
    cwd-relative 'datasets'. (Pre-fix, cmd_invariants resolved against cwd.)"""
    with tempfile.TemporaryDirectory() as d:
        r = subprocess.run(
            [sys.executable, str(MANAGE), "invariants"],
            capture_output=True, text=True, cwd=d)
    assert str(REPO / "datasets") in r.stdout, \
        f"did not resolve to the repo tree from {d}:\n{r.stdout[:400]}"
    print("PASS: test_cli_from_other_cwd_uses_repo_tree")


def test_cli_missing_runs_dir_fails_early():
    r = _run("gate", "dataset", "--runs", "runs_NOPE")
    assert r.returncode == 2, f"expected exit 2, got {r.returncode}\n{r.stderr}"
    assert "runs" in r.stderr and "not found" in r.stderr, r.stderr
    print("PASS: test_cli_missing_runs_dir_fails_early")


def main() -> int:
    tests = [
        test_relative_resolves_against_repo_not_cwd,
        test_absolute_path_is_honoured,
        test_missing_dir_raises,
        test_file_instead_of_dir_raises,
        test_describe_reports_real_provenance,
        test_describe_absent_manifest_is_explicit,
        test_cli_missing_dir_fails_early_exit_2,
        test_cli_header_is_self_evidencing,
        test_cli_from_other_cwd_uses_repo_tree,
        test_cli_missing_runs_dir_fails_early,
    ]
    passed = failed = 0
    for t in tests:
        try:
            t()
            passed += 1
        except AssertionError as e:
            print(f"FAIL: {t.__name__}: {e}")
            failed += 1
        except Exception as e:
            print(f"ERROR: {t.__name__}: {type(e).__name__}: {e}")
            failed += 1
    print()
    print("=" * 40)
    print(f"Tests: {passed} passed, {failed} failed")
    print("=" * 40)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())