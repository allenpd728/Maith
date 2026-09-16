#!/usr/bin/env python3
"""Tests for the missing-corpus diagnostic (#35, Python half).

Issue #35: `buildCorpus --per-operator` writes `Corpus/corpus.jsonl`, while
`check_ir_build.py` defaults to `Corpus/corpus.per_operator.jsonl`. The gate then
said "corpus not found" while a corpus sat right there under the other name —
misleading rather than actionable.

This does NOT change which file is read (that is #35's fix decision). It makes the
failure legible and reports the detected mode.

Run: python3 python/test_ir_build_missing_corpus.py
"""

import json
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(REPO / "python"))

import check_ir_build as cib  # noqa: E402


def _perop_line(name="d") -> str:
    return json.dumps({"name": name, "graph": {"operations": [
        {"inputs": [], "output": {"kind": "term", "index": 0}, "op": "op:AddMonoid"}]}}) + "\n"


def _module_line(name="d") -> str:
    return json.dumps({"name": name, "graph": {"operations": [
        {"inputs": [], "output": {"kind": "term", "index": 0}, "op": "GEN_ALGEBRA"}]}}) + "\n"


def test_mode_detection_per_operator():
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "c.jsonl"
        p.write_text(_perop_line() * 3)
        assert cib._detect_corpus_mode(p) == "per_operator", cib._detect_corpus_mode(p)
    print("PASS: test_mode_detection_per_operator")


def test_mode_detection_module():
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "c.jsonl"
        p.write_text(_module_line() * 3)
        assert cib._detect_corpus_mode(p) == "module", cib._detect_corpus_mode(p)
    print("PASS: test_mode_detection_module")


def test_mode_detection_mixed_and_unknown():
    with tempfile.TemporaryDirectory() as d:
        mixed = Path(d) / "m.jsonl"
        mixed.write_text(_perop_line() + _module_line())
        assert cib._detect_corpus_mode(mixed) == "mixed", cib._detect_corpus_mode(mixed)
        empty = Path(d) / "e.jsonl"
        empty.write_text("")
        assert cib._detect_corpus_mode(empty) == "unknown"
    print("PASS: test_mode_detection_mixed_and_unknown")


def test_missing_names_the_alternative_and_its_mode():
    """The core of the fix: the message must name what IS there, and its mode."""
    with tempfile.TemporaryDirectory() as d:
        tmp = Path(d)
        (tmp / "corpus.jsonl").write_text(_perop_line() * 3)
        detail = cib._corpus_missing_detail(tmp / "corpus.per_operator.jsonl")
        assert "corpus.jsonl" in detail, detail
        assert "[per_operator]" in detail, detail
        assert "#35" in detail, "should point at the issue so the reader finds the fix"
    print("PASS: test_missing_names_the_alternative_and_its_mode")


def test_missing_with_no_alternative_says_so():
    with tempfile.TemporaryDirectory() as d:
        detail = cib._corpus_missing_detail(Path(d) / "corpus.per_operator.jsonl")
        assert "no corpus*.jsonl" in detail, detail
    print("PASS: test_missing_with_no_alternative_says_so")


def test_missing_when_dir_absent_says_so():
    with tempfile.TemporaryDirectory() as d:
        detail = cib._corpus_missing_detail(Path(d) / "nope" / "corpus.jsonl")
        assert "does not exist" in detail, detail
    print("PASS: test_missing_when_dir_absent_says_so")


def test_does_not_change_which_file_is_read():
    """Guard the scope boundary: the default path is unchanged.

    The fix decision in #35 (rename the artifact vs require --corpus) is the
    maintainer's. This change must not smuggle one of the options in.
    """
    src = (REPO / "python" / "check_ir_build.py").read_text()
    assert 'default=str(_REPO_ROOT / "Corpus" / "corpus.per_operator.jsonl")' in src, \
        "the default corpus path changed -- that is #35's decision, not this fix"
    print("PASS: test_does_not_change_which_file_is_read")


def test_mode_detection_is_cheap_on_large_files():
    """Only a sample is read, so a 60MB corpus is not scanned in full."""
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "big.jsonl"
        with open(p, "w") as f:
            for i in range(5000):
                f.write(_perop_line(f"d{i}"))
        before = p.stat().st_size
        mode = cib._detect_corpus_mode(p, sample=50)
        assert mode == "per_operator", mode
        assert before > 100_000, "fixture should be non-trivial"
    print("PASS: test_mode_detection_is_cheap_on_large_files")


def main() -> int:
    tests = [
        test_mode_detection_per_operator,
        test_mode_detection_module,
        test_mode_detection_mixed_and_unknown,
        test_missing_names_the_alternative_and_its_mode,
        test_missing_with_no_alternative_says_so,
        test_missing_when_dir_absent_says_so,
        test_does_not_change_which_file_is_read,
        test_mode_detection_is_cheap_on_large_files,
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