#!/usr/bin/env python3
"""Verify a RUN_REGISTRY entry's shared provenance inputs (issue #57).

The registry lets a Run ID resolve *within* the repo. This script checks the
other half: that the pinned inputs a figure depends on still match what the
registry claims, so a figure can be traced from its registry entry alone.

It verifies the four things issue #57 calls out:
  - the base model is pinned by revision in the training script, and the pinned
    revision matches the registry;
  - the corpus content_hash in corpus_manifest.json matches the corpus bytes;
  - version strings agree across stats.json / representation_manifest.json /
    Maith/Encoder.lean;
  - the dataset split hashes match the registry.

What it cannot verify from a fresh clone: `Corpus/*.jsonl` and `runs/` are
gitignored, so the corpus bytes and the trained checkpoints are absent. Those
checks report SKIP, not PASS — a figure is reproducible only to the extent its
inputs are tracked.

Usage:
    python3 python/verify_registry_inputs.py [--variant A]
"""

import argparse
import contextlib
import hashlib
import io
import re
import sys
from pathlib import Path
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parent))
from check_corpus import (  # noqa: E402  (reuse the Gate-2 helpers, don't fork them)
    check_g2_6_content_hash,
    check_g2_7_version_consistency,
)


def _silent(fn, arg):
    """Run a Gate-2 helper, swallowing the human-readable line it prints."""
    with contextlib.redirect_stdout(io.StringIO()):
        return fn(arg)

REPO = Path(__file__).resolve().parent.parent

# --- Values as recorded in docs/experiments/RUN_REGISTRY.md ----------------
REGISTRY = {
    "base_model": "Qwen/Qwen2.5-Coder-0.5B",
    "base_model_revision": "8123ea2e9354afb7ffcc6c8641d1b2f5ecf18301",
    "dataset_sha256": {
        "A": ("datasets/train_A.jsonl",
              "161b3defb62d89b797e403f92e506d97ec86b6ac1f1593760c0b44b9427580d7",
              "datasets/eval_A.jsonl",
              "7dd41021995875dd5d6fea5093f365249f267b8ed5e89abeb4623ba7432a7e66"),
        "B_small": ("datasets/train_B_small.jsonl",
                    "1178585446c36c8ba47b21fb28b0200abd7c46346603c90b4170d6fdb56fb556",
                    "datasets/eval_B_small.jsonl",
                    "ebd7b256d2f0ad0d8ea79c38b844df81b69569f1d0731fb4f330bb69ec582847"),
    },
    # Figures published in the registry; metric lives in the gitignored results.json.
    "figures": {
        "A": {"run_id": "A-v3", "metric": "eval_perplexity", "value": 1.2812},
        "B_small": {"run_id": "B-small", "metric": "eval_perplexity", "value": 1.1294},
    },
}


def _sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def check_base_model_revision():
    src = (REPO / "python" / "train_v2_resume.py").read_text()
    m_model = re.search(r'^BASE_MODEL\s*=\s*"([^"]+)"', src, re.M)
    m_rev = re.search(r'^BASE_MODEL_REVISION\s*=\s*"([^"]+)"', src, re.M)
    model = m_model.group(1) if m_model else ""
    rev = m_rev.group(1) if m_rev else ""
    problems = []
    if model != REGISTRY["base_model"]:
        problems.append(f"BASE_MODEL={model!r} != registry {REGISTRY['base_model']!r}")
    if rev != REGISTRY["base_model_revision"]:
        problems.append(
            f"BASE_MODEL_REVISION={rev!r} != registry {REGISTRY['base_model_revision']!r}")
    # The pin is only load-bearing if the loads and the results writer use it.
    loads = len(re.findall(r"revision=BASE_MODEL_REVISION", src))
    if loads < 4:
        problems.append(f"only {loads} load sites use revision=BASE_MODEL_REVISION")
    return problems


def check_dataset_hashes(variant):
    entry = REGISTRY["dataset_sha256"].get(variant)
    if entry is None:
        return [f"no registry hash entry for variant {variant!r}"]
    problems = []
    for path, expected in ((entry[0], entry[1]), (entry[2], entry[3])):
        p = REPO / path
        if not p.exists():
            problems.append(f"{path} missing (tracked file — should be present)")
            continue
        actual = _sha256(p)
        if actual != expected:
            problems.append(f"{path} sha256 {actual[:16]}… != registry {expected[:16]}…")
    return problems


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--variant", default="A",
                        choices=sorted(REGISTRY["dataset_sha256"]))
    args = parser.parse_args()

    gate_args = SimpleNamespace(
        corpus=str(REPO / "Corpus" / "corpus.jsonl"),
        manifest=str(REPO / "datasets" / "representation_manifest.json"),
        stats=str(REPO / "Corpus" / "stats.json"),
    )

    rows = []  # (name, status, detail)

    problems = check_base_model_revision()
    rows.append(("base_model_revision", not problems,
                 "pinned + used at all load sites" if not problems else "; ".join(problems)))

    if not (REPO / "Corpus" / "corpus.jsonl").exists():
        rows.append(("corpus content_hash", None,
                     "SKIP — Corpus/corpus.jsonl is gitignored (absent from a fresh clone)"))
    else:
        ok = _silent(check_g2_6_content_hash, gate_args)
        rows.append(("corpus content_hash", bool(ok), "matches corpus_manifest.json" if ok
                     else "does not match corpus_manifest.json"))

    if not (REPO / "Corpus" / "stats.json").exists():
        rows.append(("version consistency", None,
                     "SKIP — Corpus/stats.json absent (regenerated by `lake exe buildCorpus`)"))
    else:
        ok = _silent(check_g2_7_version_consistency, gate_args)
        rows.append(("version consistency", bool(ok),
                     "stats.json == representation_manifest.json == Encoder.lean" if ok
                     else "version strings disagree"))

    problems = check_dataset_hashes(args.variant)
    rows.append((f"dataset hashes ({args.variant})", not problems,
                 "match registry" if not problems else "; ".join(problems)))

    fig = REGISTRY["figures"].get(args.variant)
    if fig:
        rows.append(("figure value", None,
                     f"{fig['metric']}={fig['value']} (in gitignored runs/; "
                     f"cannot be recomputed without the checkpoint)"))

    print("=" * 60)
    print("RUN_REGISTRY PROVENANCE VERIFICATION")
    print("=" * 60)
    for name, ok, detail in rows:
        tag = "SKIP" if ok is None else ("PASS" if ok else "FAIL")
        print(f"  {tag}  {name}: {detail}")

    failed = [r for r in rows if r[1] is False]
    checked = [r for r in rows if r[1] is not None]
    print()
    if failed:
        print(f"FAILED — {len(checked) - len(failed)}/{len(checked)} checks passed.")
        return 1
    print(f"PASSED — {len(checked)}/{len(checked)} verifiable checks passed; "
          f"{len(rows) - len(checked)} skipped (inputs not tracked).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
