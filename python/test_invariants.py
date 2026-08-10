#!/usr/bin/env python3
"""
test_invariants.py — Tests for check_invariants.py.

Confirms the invariant checker passes on valid fixtures and fails on
intentionally-broken ones. Each test corresponds to a real bug class.

Run: python3 python/test_invariants.py
"""

import json
import os
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from check_invariants import (
    check_split_integrity,
    check_variant_input_consistency,
    check_vocab_range,
    check_checkpoint_results_consistency,
    check_comparison_validity,
    check_provenance_hash,
    COMPARISON_CLAIMS,
)
from manifest import (
    write_dataset_manifest, write_results_manifest, write_checkpoint_manifest,
    verify_manifest, expected_source_for_variant,
)


# ---------------------------------------------------------------------------
# Fixture helpers
# ---------------------------------------------------------------------------

def _make_dataset(path: Path, variant: str, n_train: int = 10, n_eval: int = 5):
    """Create a valid dataset JSONL with correct source field.

    example_id is the SAME across variants (the invariant: all variants evaluate
    the same declarations) — only source and input_ids differ per variant.
    """
    def _row(i, split):
        return {
            "source": variant,
            "representation_id": "semantic_graph_ir_v2_0_0",
            "example_id": f"Test.Mod::DECL_{split}_{i}",  # same across variants
            "name": f"DECL_{split}_{i}",
            "module": f"Test.Mod",
            "input_ids": [i % 100, (i + 1) % 100, (i + 2) % 100],
            "labels": [0, 1, 2],
            "seq_len": 3,
        }
    with open(path / f"train_{variant}.jsonl", "w") as f:
        for i in range(n_train):
            f.write(json.dumps(_row(i, "train")) + "\n")
    with open(path / f"eval_{variant}.jsonl", "w") as f:
        for i in range(n_eval):
            f.write(json.dumps(_row(i, "eval")) + "\n")


def _make_broken_source_dataset(path: Path, variant: str):
    """Create a dataset where the source field doesn't match the variant."""
    with open(path / f"train_{variant}.jsonl", "w") as f:
        for i in range(3):
            f.write(json.dumps({
                "source": "WRONG_VARIANT",
                "representation_id": "semantic_graph_ir_v2_0_0",
                "example_id": f"Test.Mod::{variant}_train_{i}",
                "name": f"{variant}_train_{i}",
                "module": "Test.Mod",
                "input_ids": [1, 2, 3],
                "labels": [0, 1, 2],
                "seq_len": 3,
            }) + "\n")


def _make_overlap_dataset(path: Path, variant: str):
    """Create a dataset where train and eval overlap by example_id."""
    shared_id = f"Test.Mod::SHARED_DECL"
    for split, n in [("train", 10), ("eval", 5)]:
        with open(path / f"{split}_{variant}.jsonl", "w") as f:
            # First row is shared
            f.write(json.dumps({
                "source": variant,
                "representation_id": "semantic_graph_ir_v2_0_0",
                "example_id": shared_id,
                "name": "SHARED_DECL",
                "module": "Test.Mod",
                "input_ids": [1, 2, 3],
                "labels": [0, 1, 2],
                "seq_len": 3,
            }) + "\n")
            for i in range(n - 1):
                f.write(json.dumps({
                    "source": variant,
                    "representation_id": "semantic_graph_ir_v2_0_0",
                    "example_id": f"Test.Mod::{variant}_{split}_{i}",
                    "name": f"{variant}_{split}_{i}",
                    "module": "Test.Mod",
                    "input_ids": [1, 2, 3],
                    "labels": [0, 1, 2],
                    "seq_len": 3,
                }) + "\n")


def _make_out_of_vocab_dataset(path: Path, variant: str, vocab_size: int):
    """Create a dataset with input_ids exceeding vocab_size."""
    with open(path / f"train_{variant}.jsonl", "w") as f:
        for i in range(3):
            f.write(json.dumps({
                "source": variant,
                "representation_id": "semantic_graph_ir_v2_0_0",
                "example_id": f"Test.Mod::{variant}_train_{i}",
                "name": f"{variant}_train_{i}",
                "module": "Test.Mod",
                "input_ids": [vocab_size + 100],  # out of range
                "labels": [0],
                "seq_len": 1,
            }) + "\n")
    with open(path / f"eval_{variant}.jsonl", "w") as f:
        for i in range(2):
            f.write(json.dumps({
                "source": variant,
                "representation_id": "semantic_graph_ir_v2_0_0",
                "example_id": f"Test.Mod::{variant}_eval_{i}",
                "name": f"{variant}_eval_{i}",
                "module": "Test.Mod",
                "input_ids": [1, 2],
                "labels": [0, 1],
                "seq_len": 2,
            }) + "\n")


# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

def test_split_integrity_passes():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_dataset(tmp, "A")
        _make_dataset(tmp, "B")
        results = check_split_integrity(tmp, ["A", "B"])
        failures = [r for r in results if r["passed"] is False]
        assert not failures, f"Expected pass, got failures: {failures}"
    print("PASS: test_split_integrity_passes")


def test_split_integrity_catches_overlap():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_overlap_dataset(tmp, "A")
        results = check_split_integrity(tmp, ["A"])
        overlap_checks = [r for r in results if "disjoint" in r["name"]]
        failures = [r for r in overlap_checks if r["passed"] is False]
        assert failures, "Expected failure on overlapping split, but all passed"
    print("PASS: test_split_integrity_catches_overlap")


def test_variant_input_passes():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_dataset(tmp, "A")
        results = check_variant_input_consistency(tmp, "A")
        failures = [r for r in results if r["passed"] is False]
        assert not failures, f"Expected pass, got failures: {failures}"
    print("PASS: test_variant_input_passes")


def test_variant_input_catches_mismatch():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_broken_source_dataset(tmp, "A")
        # need eval file too
        with open(tmp / "eval_A.jsonl", "w") as f:
            for i in range(2):
                f.write(json.dumps({
                    "source": "WRONG_VARIANT",
                    "representation_id": "semantic_graph_ir_v2_0_0",
                    "example_id": f"Test.Mod::A_eval_{i}",
                    "name": f"A_eval_{i}",
                    "module": "Test.Mod",
                    "input_ids": [1, 2],
                    "labels": [0, 1],
                    "seq_len": 2,
                }) + "\n")
        results = check_variant_input_consistency(tmp, "A")
        failures = [r for r in results if r["passed"] is False]
        assert failures, "Expected failure on source mismatch, but all passed"
    print("PASS: test_variant_input_catches_mismatch")


def test_vocab_range_passes():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_dataset(tmp, "A")
        # input_ids are [i%100, (i+1)%100, (i+2)%100] — all < 101
        results = check_vocab_range(tmp, "A", vocab_size=101)
        failures = [r for r in results if r["passed"] is False]
        assert not failures, f"Expected pass, got failures: {failures}"
    print("PASS: test_vocab_range_passes")


def test_vocab_range_catches_out_of_range():
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        _make_out_of_vocab_dataset(tmp, "A", vocab_size=100)
        results = check_vocab_range(tmp, "A", vocab_size=100)
        failures = [r for r in results if r["passed"] is False]
        assert failures, "Expected failure on out-of-range ids, but all passed"
    print("PASS: test_vocab_range_catches_out_of_range")


def test_comparison_validity_matched_size():
    m_a = {
        "artifact_type": "results", "variant": "A",
        "representation_id": "semantic_graph_ir_v2_0_0",
        "source_input": "ir_tokens", "seed": 42, "epochs": 2,
        "train_examples": 3491, "eval_examples": 388,
    }
    m_bsmall = {
        "artifact_type": "results", "variant": "B_small",
        "representation_id": "semantic_graph_ir_v2_0_0",
        "source_input": "bpe_601", "seed": 42, "epochs": 2,
        "train_examples": 3491, "eval_examples": 388,
    }
    results = check_comparison_validity([m_a, m_bsmall], "representation_at_matched_size")
    failures = [r for r in results if r["passed"] is False]
    assert not failures, f"Expected pass for matched-size comparison, got: {failures}"
    print("PASS: test_comparison_validity_matched_size")


def test_comparison_validity_catches_epoch_mismatch():
    m_a = {
        "artifact_type": "results", "variant": "A",
        "representation_id": "semantic_graph_ir_v2_0_0",
        "source_input": "ir_tokens", "seed": 42, "epochs": 2,
        "train_examples": 3491, "eval_examples": 388,
    }
    m_b = {
        "artifact_type": "results", "variant": "B",
        "representation_id": "semantic_graph_ir_v2_0_0",
        "source_input": "bpe_full", "seed": 42, "epochs": 3,  # EPOCH MISMATCH
        "train_examples": 3491, "eval_examples": 388,
    }
    results = check_comparison_validity([m_a, m_b], "representation_at_matched_size")
    failures = [r for r in results if r["passed"] is False]
    assert failures, "Expected failure on epoch mismatch (2 vs 3), but comparison passed"
    print("PASS: test_comparison_validity_catches_epoch_mismatch")


def test_manifest_verify_catches_missing_fields():
    bad_manifest = {"artifact_type": "results", "variant": "A"}
    errors = verify_manifest(bad_manifest)
    assert len(errors) > 0, "Expected errors for missing fields"
    assert any("missing field" in e for e in errors)
    print("PASS: test_manifest_verify_catches_missing_fields")


def test_manifest_verify_catches_invalid_values():
    bad_manifest = {
        "artifact_type": "WRONG", "variant": "ALSO_WRONG",
        "representation_id": "x", "source_input": "nope",
        "seed": 42, "epochs": 2, "train_examples": 1, "eval_examples": 1,
        "created_at": "x", "created_by_commit": "x",
        "pipeline_version": "x", "provenance_hash": "x",
    }
    errors = verify_manifest(bad_manifest)
    assert any("invalid artifact_type" in e for e in errors), f"Expected artifact_type error: {errors}"
    assert any("invalid variant" in e for e in errors), f"Expected variant error: {errors}"
    assert any("invalid source_input" in e for e in errors), f"Expected source_input error: {errors}"
    print("PASS: test_manifest_verify_catches_invalid_values")


def test_expected_source_mapping():
    assert expected_source_for_variant("A") == "ir_tokens"
    assert expected_source_for_variant("B") == "bpe_full"
    assert expected_source_for_variant("C") == "ast_split"
    assert expected_source_for_variant("B_small") == "bpe_601"
    print("PASS: test_expected_source_mapping")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    tests = [
        test_split_integrity_passes,
        test_split_integrity_catches_overlap,
        test_variant_input_passes,
        test_variant_input_catches_mismatch,
        test_vocab_range_passes,
        test_vocab_range_catches_out_of_range,
        test_comparison_validity_matched_size,
        test_comparison_validity_catches_epoch_mismatch,
        test_manifest_verify_catches_missing_fields,
        test_manifest_verify_catches_invalid_values,
        test_expected_source_mapping,
    ]
    passed = 0
    failed = 0
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
    print(f"{'='*40}")
    print(f"Tests: {passed} passed, {failed} failed")
    print(f"{'='*40}")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
