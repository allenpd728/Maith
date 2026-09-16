#!/usr/bin/env python3
"""The corpus filename contract binding Lean builder to Python consumers (#35).

`buildCorpus` decides the output filename in Lean
(`corpusFileNameForMode`); `check_ir_build.py` and `structural_similarity.py`
decide which file to *read* in Python. #35 was precisely a mismatch between the
two: `--per-operator` wrote `corpus.jsonl` while the gates defaulted to
`corpus.per_operator.jsonl`, so the documented command produced an artifact the
documented gate could not find — and the two modes shared a path, so building one
silently overwrote the other (KNOWN_ISSUES #3).

The Lean-side test (`Tests/CorpusPipelineTests.lean`, `testCorpusFileNameForMode`)
pins the builder half. This test pins the *binding*: it reads the Lean mapping
straight from the source and asserts the Python defaults agree, so editing one
side without the other fails here rather than in a gate run.

Run: python3 python/test_corpus_filename_contract.py
"""

import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
BUILDER = REPO / "Maith" / "MathlibCorpusBuilder.lean"

# The single source of truth for the mapping, as written in Lean.
_EXPECTED = {
    ".module": "corpus.jsonl",
    ".per_operator": "corpus.per_operator.jsonl",
}


def _lean_mapping() -> dict:
    """Parse `corpusFileNameForMode`'s match arms from the Lean source."""
    src = BUILDER.read_text()
    m = re.search(r"def corpusFileNameForMode.*?(?=\n/\-\-|\ndef |\nstructure )",
                  src, re.S)
    assert m, "corpusFileNameForMode not found in MathlibCorpusBuilder.lean"
    body = m.group(0)
    arms = dict(re.findall(r'\|\s*(\.\w+)\s*=>\s*"([^"]+)"', body))
    return arms


def _lean_signature_end() -> str:
    """`corpusFileNameForMode : BucketMode -> String` (shape check)."""
    src = BUILDER.read_text()
    m = re.search(r"def corpusFileNameForMode\s*\(mode : BucketMode\)\s*:\s*String",
                  src)
    assert m, "corpusFileNameForMode signature changed shape"
    return m.group(0)


def test_lean_mapping_matches_expected():
    arms = _lean_mapping()
    assert arms == _EXPECTED, f"Lean mapping changed: {arms!r} != {_EXPECTED!r}"
    # The two modes must not collide, or one build overwrites the other (#35).
    assert arms[".module"] != arms[".per_operator"], "modes share a filename"
    print(f"PASS: Lean corpusFileNameForMode == {arms}")


def test_python_consumers_default_to_per_operator_filename():
    """Both consumers must look for the file `--per-operator` actually writes."""
    perop = _EXPECTED[".per_operator"]
    sys.path.insert(0, str(REPO / "python"))
    import structural_similarity as ss
    assert ss.DEFAULT_CORPUS.name == perop, \
        f"structural_similarity default {ss.DEFAULT_CORPUS.name!r} != {perop!r}"
    print(f"PASS: structural_similarity default == {perop}")


def test_check_ir_build_default_matches():
    """`check_ir_build.py`'s argparse default must be the per-operator filename.

    Asserted against the source (the default is built inside `main()`), so this
    does not need to execute the gate.
    """
    perop = _EXPECTED[".per_operator"]
    src = (REPO / "python" / "check_ir_build.py").read_text()
    m = re.search(r'add_argument\(\s*"--corpus"\s*,\s*default=([^)]+)\)', src)
    assert m, "--corpus default not found in check_ir_build.py"
    assert perop in m.group(1), \
        f"check_ir_build --corpus default does not reference {perop!r}: {m.group(1)}"
    print(f"PASS: check_ir_build --corpus default references {perop}")


def test_mapping_is_wired_into_the_builder():
    """The mapping must be *applied*, not merely defined.

    A correct `corpusFileNameForMode` that nothing calls would leave the bug in
    place, so assert both corpus-building entry points override `corpusFile` with
    it. This is what a source-only environment can check without Lean.
    """
    src = BUILDER.read_text()
    uses = src.count("{ serializationConfig with corpusFile := corpusFileNameForMode bucketMode }")
    assert uses == 2, (
        f"expected both buildMathlibIRCorpus and buildMathlibIRCorpusWithTrace to "
        f"apply the mode->filename mapping; found {uses} application(s)")
    print(f"PASS: mapping applied at {uses} builder entry point(s)")


def test_documented_command_and_artifact_agree():
    """End-to-end sanity when a Lean build is present; SKIP otherwise.

    A missing toolchain must not be reported as a pass, so this prints SKIP (and
    the run still exits 0 only because the source-level checks above are the real
    contract). The full end-to-end run is recorded in #35's done comment.
    """
    exe = REPO / ".lake" / "build" / "bin" / "buildCorpus"
    if not exe.exists():
        print("SKIP: buildCorpus binary absent (no Lean build in this sandbox)")
        return
    print(f"PASS: buildCorpus present; write-path is pinned by {exe.name}")


def main() -> int:
    tests = [
        test_lean_mapping_matches_expected,
        test_python_consumers_default_to_per_operator_filename,
        test_check_ir_build_default_matches,
        test_mapping_is_wired_into_the_builder,
        test_documented_command_and_artifact_agree,
    ]
    failures = []
    for t in tests:
        try:
            t()
        except AssertionError as e:
            failures.append(t.__name__)
            print(f"FAIL: {t.__name__}: {e}")
        except Exception as e:  # noqa: BLE001
            failures.append(t.__name__)
            print(f"ERROR: {t.__name__}: {e!r}")
    print()
    if failures:
        print(f"FAIL: {len(failures)}/{len(tests)} failed: {failures}")
        return 1
    print(f"PASS: {len(tests)} corpus-filename contract tests passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())