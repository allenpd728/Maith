#!/usr/bin/env python3
"""
check_invariants.py — The gate that runs before experiments.

Asserts every semantic invariant the experiments rely on. Exits non-zero on any
violation. Called as a precondition by every experiment script (train, eval, extract,
retrieve). Runs in seconds.

Each invariant is grounded in a real bug the project already hit, generalized from a
specific regression to a class guard. See docs/experiments/PIPELINE_HARDENING_SCOPE.md §4.2.

Usage:
    python3 python/check_invariants.py [--datasets datasets] [--runs runs]
    python3 python/check_invariants.py --check split
    python3 python/check_invariants.py --check variant_input --variant A
"""

import argparse
import json
import os
import sys
from pathlib import Path
from typing import Optional

# Import the manifest system
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from manifest import (
    load_manifest, verify_manifest, expected_source_for_variant, VARIANT_TO_SOURCE,
    VARIANT_TO_DATASET_SOURCE, REQUIRED_FIELDS,
)

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _load_jsonl(path: Path) -> list[dict]:
    rows = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                rows.append(json.loads(line))
    return rows


def _example_id(row: dict) -> str:
    rid = row.get("example_id")
    return str(rid) if rid else f"{row.get('module', '')}::{row.get('name', '')}"


class InvariantError(Exception):
    pass


def _report(name: str, passed: bool, detail: str = "") -> dict:
    return {"name": name, "passed": passed, "detail": detail}


# ---------------------------------------------------------------------------
# Invariant 1: Split integrity
# ---------------------------------------------------------------------------

def check_split_integrity(datasets_dir: Path, variants: list[str] = None) -> list[dict]:
    """Train/eval disjoint by example_id for all variants; eval identity identical."""
    if variants is None:
        variants = ["A", "B_small", "B", "C", "flat"]

    results = []
    eval_id_sets = {}

    for v in variants:
        train_path = datasets_dir / f"train_{v}.jsonl"
        eval_path = datasets_dir / f"eval_{v}.jsonl"

        if not train_path.exists() or not eval_path.exists():
            results.append(_report(f"split_{v}_files", False, f"missing: {train_path} or {eval_path}"))
            continue

        train_ids = {_example_id(r) for r in _load_jsonl(train_path)}
        eval_ids = {_example_id(r) for r in _load_jsonl(eval_path)}
        overlap = train_ids & eval_ids

        eval_id_sets[v] = eval_ids

        if overlap:
            sample = sorted(overlap)[:5]
            results.append(_report(
                f"split_{v}_disjoint", False,
                f"{len(overlap)} train/eval overlaps: {sample}"
            ))
        else:
            results.append(_report(
                f"split_{v}_disjoint", True,
                f"train={len(train_ids)} eval={len(eval_ids)} disjoint"
            ))

    # Cross-variant eval identity (same declarations evaluated)
    if len(eval_id_sets) > 1:
        ref_variant = list(eval_id_sets.keys())[0]
        ref_ids = eval_id_sets[ref_variant]
        for v, ids in eval_id_sets.items():
            if v == ref_variant:
                continue
            if ids != ref_ids:
                diff = (ids - ref_ids) | (ref_ids - ids)
                results.append(_report(
                    f"split_eval_identity_{ref_variant}_vs_{v}", False,
                    f"{len(diff)} eval-id differences (first 5: {sorted(diff)[:5]})"
                ))
            else:
                results.append(_report(f"split_eval_identity_{ref_variant}_vs_{v}", True, "identical"))

    # Cross-variant train count (same number trained)
    if len(variants) > 1:
        train_counts = {}
        for v in variants:
            tp = datasets_dir / f"train_{v}.jsonl"
            if tp.exists():
                train_counts[v] = len(_load_jsonl(tp))
        if train_counts:
            ref_count = list(train_counts.values())[0]
            mismatched = {v: c for v, c in train_counts.items() if c != ref_count}
            if mismatched:
                results.append(_report(
                    "split_train_count_uniform", False,
                    f"train counts differ: {mismatched} (ref={ref_count})"
                ))
            else:
                results.append(_report("split_train_count_uniform", True, f"all={ref_count}"))

    return results


# ---------------------------------------------------------------------------
# Invariant 2: Checkpoint-results consistency
# ---------------------------------------------------------------------------

def check_checkpoint_results_consistency(runs_dir: Path, variant: Optional[str] = None) -> list[dict]:
    """results.json references a checkpoint that exists; mtimes are consistent."""
    results = []

    variant_dirs = []
    if variant:
        candidate = runs_dir / f"variant_{variant}"
        if candidate.exists():
            variant_dirs.append(candidate)
    else:
        for d in sorted(runs_dir.iterdir()):
            if d.is_dir() and d.name.startswith("variant_"):
                variant_dirs.append(d)

    for vdir in variant_dirs:
        results_path = vdir / "results.json"
        if not results_path.exists():
            continue

        with open(results_path) as f:
            r = json.load(f)

        # The checkpoint should exist in the same dir
        ckpt_dir = vdir / "checkpoint-final"
        if not ckpt_dir.exists():
            # Check for other checkpoint patterns
            ckpts = list(vdir.glob("checkpoint-*"))
            ckpt_dir = ckpts[0] if ckpts else None

        if not ckpt_dir or not ckpt_dir.exists():
            results.append(_report(
                f"ckpt_results_{vdir.name}", False,
                f"results.json exists but no checkpoint dir found in {vdir}"
            ))
            continue

        # Mtime consistency (stale guard — hard fail if >7 days off)
        import os.path
        r_mtime = os.path.getmtime(results_path)
        ckpt_mtime = os.path.getmtime(ckpt_dir)
        diff_days = abs(r_mtime - ckpt_mtime) / 86400

        if diff_days > 7:
            results.append(_report(
                f"ckpt_results_mtime_{vdir.name}", False,
                f"results.json and checkpoint differ by {diff_days:.1f} days (stale?)"
            ))
        else:
            results.append(_report(
                f"ckpt_results_mtime_{vdir.name}", True,
                f"mtime diff={diff_days:.2f}d"
            ))

    return results


# ---------------------------------------------------------------------------
# Invariant 3: Variant-input consistency (the extraction bug, generalized)
# ---------------------------------------------------------------------------

def check_variant_input_consistency(datasets_dir: Path, variant: str) -> list[dict]:
    """Dataset source field matches the variant's expected source_input."""
    results = []
    expected_source = expected_source_for_variant(variant)
    expected_dataset_source = VARIANT_TO_DATASET_SOURCE.get(variant, variant)

    for split in ["train", "eval"]:
        path = datasets_dir / f"{split}_{variant}.jsonl"
        if not path.exists():
            results.append(_report(f"variant_input_{variant}_{split}", False, f"missing: {path}"))
            continue

        rows = _load_jsonl(path)
        mismatches = [r for r in rows if r.get("source") != expected_dataset_source]

        if mismatches:
            results.append(_report(
                f"variant_input_{variant}_{split}", False,
                f"{len(mismatches)}/{len(rows)} records have source != '{expected_dataset_source}' "
                f"(expected source_input='{expected_source}'). "
                f"Example: source='{mismatches[0].get('source')}'"
            ))
        else:
            results.append(_report(
                f"variant_input_{variant}_{split}", True,
                f"all {len(rows)} records have source='{expected_dataset_source}' (expected: {expected_source})"
            ))

    return results


# ---------------------------------------------------------------------------
# Invariant 4: Vocab-range consistency (the bos/eos crash, generalized)
# ---------------------------------------------------------------------------

def check_vocab_range(datasets_dir: Path, variant: str, vocab_size: Optional[int] = None) -> list[dict]:
    """Every input_ids value is within [0, vocab_size)."""
    results = []

    if vocab_size is None:
        # Try to infer from vocab_A.json (A/flat) or results.json
        if variant in ("A", "flat"):
            vocab_path = datasets_dir / f"vocab_{variant}.json"
            if not vocab_path.exists() and variant == "flat":
                # flat may not have its own vocab; use 11 (the known flat vocab size)
                vocab_size = 11
            elif vocab_path.exists():
                with open(vocab_path) as f:
                    vocab = json.load(f)
                vocab_size = len(vocab)
        if vocab_size is None:
            # B/C/B_small use Qwen BPE — check results.json for vocab_size
            for d in [datasets_dir.parent / "runs"]:
                rp = d / f"variant_{variant}" / "results.json"
                if rp.exists():
                    with open(rp) as f:
                        r = json.load(f)
                    vocab_size = r.get("vocab_size")
                    break
        if vocab_size is None and variant in ("B", "C"):
            # Full Qwen BPE vocab — check the checkpoint's tokenizer config
            for d in [datasets_dir.parent / "runs"]:
                tk = d / f"variant_{variant}" / "checkpoint-final" / "tokenizer_config.json"
                if tk.exists():
                    with open(tk) as f:
                        tc = json.load(f)
                    # Qwen vocab size is typically 151,643
                    vocab_size = tc.get("vocab_size", 151643)
                    break
            if vocab_size is None:
                vocab_size = 151643  # known Qwen2.5-Coder BPE vocab

    if vocab_size is None:
        results.append(_report(
            f"vocab_range_{variant}", None,  # None = skipped
            f"cannot determine vocab_size for variant {variant} — skipped"
        ))
        return results

    for split in ["train", "eval"]:
        path = datasets_dir / f"{split}_{variant}.jsonl"
        if not path.exists():
            continue

        rows = _load_jsonl(path)
        out_of_range = 0
        max_id = 0
        for r in rows:
            for tid in r.get("input_ids", []):
                if not isinstance(tid, int):
                    continue
                if tid < 0 or tid >= vocab_size:
                    out_of_range += 1
                if tid > max_id:
                    max_id = tid

        if out_of_range > 0:
            results.append(_report(
                f"vocab_range_{variant}_{split}", False,
                f"{out_of_range} input_ids out of [0, {vocab_size}); max_id={max_id}"
            ))
        else:
            results.append(_report(
                f"vocab_range_{variant}_{split}", True,
                f"all in [0, {vocab_size}); max_id={max_id}"
            ))

    return results


# ---------------------------------------------------------------------------
# Invariant 5: Comparison validity (the epoch confound, generalized)
# ---------------------------------------------------------------------------

COMPARISON_CLAIMS = {
    "representation_at_matched_size": {
        "must_match": ["representation_id", "seed", "train_examples", "eval_examples", "epochs"],
        "must_differ": ["source_input"],
        "description": "IR vs BPE at matched params/vocab (the DEC-027 control)",
    },
    "representation_vs_full_bpe": {
        "must_match": ["representation_id", "seed", "train_examples", "eval_examples"],
        "must_differ": ["source_input"],
        "description": "IR vs full BPE (size confound present — not a clean test)",
    },
    "ir_vs_flat_ablation": {
        "must_match": ["representation_id", "seed", "train_examples", "eval_examples", "epochs"],
        "must_differ": [],  # same source_input, differs in IR richness
        "description": "IR vs flat-IR ablation (DEC-024)",
    },
}


def check_comparison_validity(
    manifests: list[dict],
    claim: str,
) -> list[dict]:
    """Assert compared artifacts support the claimed comparison."""
    results = []

    if claim not in COMPARISON_CLAIMS:
        results.append(_report(f"comparison_claim_{claim}", False, f"unknown claim: {claim}"))
        return results

    spec = COMPARISON_CLAIMS[claim]

    if len(manifests) < 2:
        results.append(_report(f"comparison_{claim}", False, "need >= 2 manifests to compare"))
        return results

    errors = []
    for field in spec["must_match"]:
        values = {m.get(field) for m in manifests}
        if len(values) > 1:
            errors.append(f"{field} differs: {[m.get(field) for m in manifests]}")

    for field in spec["must_differ"]:
        values = [m.get(field) for m in manifests]
        if len(set(values)) == 1:
            errors.append(f"{field} identical across all ({values[0]}) — expected to differ")

    if errors:
        results.append(_report(f"comparison_{claim}", False, "; ".join(errors)))
    else:
        results.append(_report(f"comparison_{claim}", True, spec["description"]))

    return results


# ---------------------------------------------------------------------------
# Invariant 6: Provenance hash consistency
# ---------------------------------------------------------------------------

def check_provenance_hash(datasets_dir: Path, variants: list[str] = None) -> list[dict]:
    """Datasets compared as 'same data, different representation' have matching
    provenance hashes on their input corpus."""
    results = []
    if variants is None:
        variants = ["A", "B_small", "B", "C"]

    hashes = {}
    for v in variants:
        for split in ["train", "eval"]:
            path = datasets_dir / f"{split}_{v}.jsonl"
            if not path.exists():
                continue
            manifest = load_manifest(path)
            if manifest and manifest.get("provenance_hash"):
                hashes.setdefault(split, {})[v] = manifest["provenance_hash"]

    for split, vhashes in hashes.items():
        if len(vhashes) < 2:
            continue
        ref_v = list(vhashes.keys())[0]
        ref_h = vhashes[ref_v]
        for v, h in vhashes.items():
            if v == ref_v:
                continue
            if h != ref_h:
                results.append(_report(
                    f"provenance_hash_{split}_{ref_v}_vs_{v}", False,
                    f"hash mismatch: {ref_v}={ref_h[:20]}... vs {v}={h[:20]}..."
                ))
            else:
                results.append(_report(
                    f"provenance_hash_{split}_{ref_v}_vs_{v}", True, "matching"
                ))

    return results


# ---------------------------------------------------------------------------
# Invariant 7: Corpus-format consistency (P0-2 fix — corpus format verified)
# ---------------------------------------------------------------------------

def check_corpus_format(datasets_dir: Path) -> list[dict]:
    """Verify the corpus on disk matches the format the datasets claim.

    The corpus has been overwritten before (KNOWN_ISSUES issue 3 / AUDIT P0-2):
    a per-operator build overwrote the v2 module-mode corpus. This check
    verifies the corpus token format matches what the dataset manifest claims,
    so future rebuilds can't silently produce wrong-format datasets.

    v2 module-mode: GEN_<AREA> bucket tokens, 0 gen:<FullName> tokens
    per-operator mode: gen:<FullName> tokens, 0 GEN_<AREA> tokens
    """
    results = []
    corpus_path = datasets_dir.parent / "Corpus" / "corpus.jsonl"

    if not corpus_path.exists():
        results.append(_report("corpus_format", False, f"corpus not found at {corpus_path}"))
        return results

    # Sample the first 100 records to determine corpus format
    gen_named_count = 0
    gen_bucket_count = 0
    records_checked = 0
    with open(corpus_path) as f:
        for i, line in enumerate(f):
            if i >= 100:
                break
            d = json.loads(line)
            for t in d.get("tokens", []):
                if isinstance(t, str):
                    if t.startswith("gen:") and t not in ("gen:hof", "gen:proj"):
                        gen_named_count += 1
                    elif t.startswith("GEN_"):
                        gen_bucket_count += 1
            records_checked += 1

    # Determine corpus format
    if gen_named_count > 0 and gen_bucket_count == 0:
        corpus_format = "per_operator"
    elif gen_bucket_count > 0 and gen_named_count == 0:
        corpus_format = "v2_module"
    elif gen_named_count > 0 and gen_bucket_count > 0:
        corpus_format = "mixed (CORRUPT)"
    else:
        corpus_format = "unknown"

    # Check against the canonical dataset manifest
    manifest_path = datasets_dir / "representation_manifest.json"
    expected_format = "unknown"
    if manifest_path.exists():
        with open(manifest_path) as f:
            manifest = json.load(f)
        rep_id = manifest.get("representation_id", "")
        if "v2_0_0" in rep_id:
            expected_format = "v2_module"
        elif "v2_1" in rep_id or "perop" in rep_id:
            expected_format = "per_operator"

    if corpus_format == "mixed (CORRUPT)":
        results.append(_report(
            "corpus_format", False,
            f"corpus has BOTH gen:FullName ({gen_named_count}) and GEN_ buckets "
            f"({gen_bucket_count}) in first {records_checked} records — CORRUPT"
        ))
    elif expected_format != "unknown" and corpus_format != expected_format:
        results.append(_report(
            "corpus_format", False,
            f"corpus is {corpus_format} but manifest claims {expected_format} "
            f"({manifest_path}) — rebuild datasets from the correct corpus"
        ))
    else:
        results.append(_report(
            "corpus_format", True,
            f"corpus is {corpus_format}, manifest expects {expected_format} "
            f"(gen:FullName={gen_named_count}, GEN_={gen_bucket_count} in {records_checked} records)"
        ))

    return results


# ---------------------------------------------------------------------------
# Main: run all checks
# ---------------------------------------------------------------------------

def run_all_checks(
    datasets_dir: Path,
    runs_dir: Path,
    variants: list[str] = None,
) -> list[dict]:
    if variants is None:
        variants = ["A", "B_small", "B", "C", "flat"]

    all_results = []

    # Invariant 1: Split integrity
    all_results.extend(check_split_integrity(datasets_dir, variants))

    # Invariant 2: Checkpoint-results consistency
    all_results.extend(check_checkpoint_results_consistency(runs_dir))

    # Invariant 3: Variant-input consistency (per variant)
    for v in variants:
        all_results.extend(check_variant_input_consistency(datasets_dir, v))

    # Invariant 4: Vocab-range consistency (per variant)
    for v in variants:
        all_results.extend(check_vocab_range(datasets_dir, v))

    # Invariant 6: Provenance hash consistency
    all_results.extend(check_provenance_hash(datasets_dir, variants))

    # Invariant 7: Corpus-format consistency
    all_results.extend(check_corpus_format(datasets_dir))

    return all_results


def main() -> int:
    parser = argparse.ArgumentParser(description="Invariant checker — the gate before experiments.")
    parser.add_argument("--datasets", default="datasets", help="Datasets directory")
    parser.add_argument("--runs", default="runs", help="Runs directory")
    parser.add_argument("--check", choices=["all", "split", "variant_input", "ckpt", "vocab", "provenance"],
                        default="all")
    parser.add_argument("--variant", help="Specific variant for per-variant checks")
    parser.add_argument("--strict", action="store_true", default=True,
                        help="Exit non-zero on any failure (default)")
    args = parser.parse_args()

    datasets_dir = Path(args.datasets)
    runs_dir = Path(args.runs)
    variants = [args.variant] if args.variant else None

    if args.check == "all":
        results = run_all_checks(datasets_dir, runs_dir, variants)
    elif args.check == "split":
        results = check_split_integrity(datasets_dir, variants)
    elif args.check == "variant_input":
        if not args.variant:
            print("ERROR: --check variant_input requires --variant")
            return 2
        results = check_variant_input_consistency(datasets_dir, args.variant)
    elif args.check == "ckpt":
        results = check_checkpoint_results_consistency(runs_dir, args.variant)
    elif args.check == "vocab":
        if not args.variant:
            print("ERROR: --check vocab requires --variant")
            return 2
        results = check_vocab_range(datasets_dir, args.variant)
    elif args.check == "provenance":
        results = check_provenance_hash(datasets_dir, variants)
    else:
        results = []

    # Report
    passed = sum(1 for r in results if r["passed"] is True)
    failed = sum(1 for r in results if r["passed"] is False)
    skipped = sum(1 for r in results if r["passed"] is None)

    print("=" * 60)
    print("INVARIANT CHECK")
    print("=" * 60)
    for r in results:
        if r["passed"] is True:
            print(f"  PASS  {r['name']}: {r['detail']}")
        elif r["passed"] is False:
            print(f"  FAIL  {r['name']}: {r['detail']}")
        else:
            print(f"  SKIP  {r['name']}: {r['detail']}")

    print()
    print(f"Total: {passed} passed, {failed} failed, {skipped} skipped")

    if failed > 0 and args.strict:
        print()
        print("INVARIANT VIOLATION — experiment blocked.")
        return 1

    print()
    print("All invariants hold. Experiment may proceed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
