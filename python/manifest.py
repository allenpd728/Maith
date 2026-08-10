#!/usr/bin/env python3
"""
manifest.py — Provenance manifests for Maith pipeline artifacts.

Every artifact (checkpoint, dataset, embeddings, results, ground_truth) carries a
manifest recording what it is and how it was made. The manifest is the foundation for
check_invariants.py: invariants are checked *between* manifests, not by re-deriving
provenance from the artifacts themselves.

Manifest schema (shared by all artifact types):
    artifact_type:   checkpoint | dataset | embeddings | results | ground_truth
    variant:          A | B_small | B | C | flat | random | multi
    representation_id: e.g. semantic_graph_ir_v2_0_0
    source_input:     ir_tokens | leanExpr | ast_split | bpe_601 | bpe_full
    seed:              int
    epochs:            int
    train_examples:    int
    eval_examples:     int
    created_at:        ISO timestamp
    created_by_commit: git SHA (or "unknown")
    pipeline_version:  str
    provenance_hash:   content hash of the inputs that produced this artifact
    extra fields per artifact_type (see write_*_manifest functions)

Usage:
    from manifest import write_results_manifest, load_manifest, verify_manifest
    write_results_manifest(results_path, ...)
    m = load_manifest("runs/variant_A_v2_full/manifest.json")
    verify_manifest(m)  # raises on missing/invalid fields
"""

import hashlib
import json
import os
import subprocess
from datetime import datetime, timezone
from pathlib import Path
from typing import Optional

MANIFEST_FILENAME = "manifest.json"

REQUIRED_FIELDS = [
    "artifact_type",
    "variant",
    "representation_id",
    "source_input",
    "seed",
    "epochs",
    "train_examples",
    "eval_examples",
    "created_at",
    "created_by_commit",
    "pipeline_version",
    "provenance_hash",
]

ARTIFACT_TYPES = {"checkpoint", "dataset", "embeddings", "results", "ground_truth"}

VARIANTS = {"A", "B_small", "B", "C", "flat", "random", "multi"}

SOURCE_INPUTS = {"ir_tokens", "leanExpr", "ast_split", "bpe_601", "bpe_full"}

VARIANT_TO_SOURCE = {
    "A": "ir_tokens",
    "B_small": "bpe_601",
    "B": "bpe_full",
    "C": "ast_split",
    "flat": "ir_tokens",
    "random": "bpe_full",
}


def _git_sha(repo_root: Optional[str] = None) -> str:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "HEAD"],
            cwd=repo_root or os.getcwd(),
            capture_output=True, text=True, timeout=5,
        )
        return result.stdout.strip() or "unknown"
    except Exception:
        return "unknown"


def _now_iso() -> str:
    return datetime.now(timezone.utc).isoformat(timespec="seconds")


def _file_content_hash(path: str | Path, max_bytes: int = 256 * 1024 * 1024) -> str:
    """SHA256 of file content, capped at max_bytes for large files (checkpoints)."""
    h = hashlib.sha256()
    n = 0
    with open(path, "rb") as f:
        while n < max_bytes:
            chunk = f.read(1 << 20)
            if not chunk:
                break
            h.update(chunk)
            n += len(chunk)
    return f"sha256:{h.hexdigest()}"


def _dataset_hash(dataset_path: str | Path) -> str:
    """Hash the semantic content of a dataset: name + module + input_ids per example."""
    h = hashlib.sha256()
    with open(dataset_path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            rec = json.loads(line)
            # Hash identity + token sequence, not cosmetic fields
            identity = f"{rec.get('example_id', '')}|{rec.get('name', '')}|{rec.get('module', '')}"
            ids = json.dumps(rec.get("input_ids", []))
            h.update(identity.encode() + b"\n" + ids.encode() + b"\n")
    return f"sha256:{h.hexdigest()}"


def write_manifest(
    artifact_path: str | Path,
    manifest: dict,
    filename: str = MANIFEST_FILENAME,
) -> Path:
    """Write a manifest.json alongside an artifact."""
    artifact_path = Path(artifact_path)
    if artifact_path.is_dir():
        manifest_path = artifact_path / filename
    else:
        manifest_path = artifact_path.parent / filename
    with open(manifest_path, "w") as f:
        json.dump(manifest, f, indent=2)
    return manifest_path


def load_manifest(artifact_dir: str | Path, filename: str = MANIFEST_FILENAME) -> Optional[dict]:
    """Load a manifest from an artifact directory. Returns None if missing."""
    manifest_path = Path(artifact_dir) / filename
    if not manifest_path.exists():
        return None
    with open(manifest_path) as f:
        return json.load(f)


def verify_manifest(manifest: dict) -> list[str]:
    """Validate a manifest has all required fields with valid values.
    Returns a list of error strings (empty = valid)."""
    errors = []
    for field in REQUIRED_FIELDS:
        if field not in manifest:
            errors.append(f"missing field: {field}")

    if manifest.get("artifact_type") and manifest["artifact_type"] not in ARTIFACT_TYPES:
        errors.append(f"invalid artifact_type: {manifest['artifact_type']}")

    if manifest.get("variant") and manifest["variant"] not in VARIANTS:
        errors.append(f"invalid variant: {manifest['variant']}")

    if manifest.get("source_input") and manifest["source_input"] not in SOURCE_INPUTS:
        errors.append(f"invalid source_input: {manifest['source_input']}")

    return errors


def expected_source_for_variant(variant: str) -> str:
    """Return the expected source_input for a variant."""
    if variant not in VARIANT_TO_SOURCE:
        raise ValueError(f"unknown variant: {variant}")
    return VARIANT_TO_SOURCE[variant]


# ---------------------------------------------------------------------------
# Per-artifact-type manifest writers
# ---------------------------------------------------------------------------

def write_results_manifest(
    results_path: str | Path,
    *,
    variant: str,
    representation_id: str,
    seed: int,
    epochs: int,
    train_examples: int,
    eval_examples: int,
    checkpoint_path: Optional[str | Path] = None,
    actual_learning_rate: Optional[float] = None,
    actual_batch_size: Optional[int] = None,
    actual_max_seq_len: Optional[int] = None,
    vocab_size: Optional[int] = None,
    n_params_M: Optional[float] = None,
    eval_perplexity: Optional[float] = None,
    pipeline_version: str = "1.0",
    repo_root: Optional[str] = None,
) -> Path:
    """Write a manifest for a results.json + its checkpoint."""
    results_path = Path(results_path)
    repo_root = repo_root or os.getcwd()

    manifest = {
        "artifact_type": "results",
        "variant": variant,
        "representation_id": representation_id,
        "source_input": expected_source_for_variant(variant),
        "seed": seed,
        "epochs": epochs,
        "train_examples": train_examples,
        "eval_examples": eval_examples,
        "created_at": _now_iso(),
        "created_by_commit": _git_sha(repo_root),
        "pipeline_version": pipeline_version,
        "provenance_hash": _file_content_hash(results_path),
        "checkpoint_path": str(checkpoint_path) if checkpoint_path else None,
        "actual_learning_rate": actual_learning_rate,
        "actual_batch_size": actual_batch_size,
        "actual_max_seq_len": actual_max_seq_len,
        "vocab_size": vocab_size,
        "n_params_M": n_params_M,
        "eval_perplexity": eval_perplexity,
    }

    manifest_path = write_manifest(results_path, manifest)
    # Also write a copy in the checkpoint dir if provided
    if checkpoint_path and Path(checkpoint_path).is_dir():
        write_manifest(checkpoint_path, manifest)
    return manifest_path


def write_dataset_manifest(
    dataset_path: str | Path,
    *,
    variant: str,
    representation_id: str,
    seed: int,
    epochs: int,
    train_examples: int,
    eval_examples: int,
    pipeline_version: str = "1.0",
    repo_root: Optional[str] = None,
) -> Path:
    """Write a manifest for a dataset file (train_V.jsonl or eval_V.jsonl)."""
    dataset_path = Path(dataset_path)
    manifest = {
        "artifact_type": "dataset",
        "variant": variant,
        "representation_id": representation_id,
        "source_input": expected_source_for_variant(variant),
        "seed": seed,
        "epochs": epochs,
        "train_examples": train_examples,
        "eval_examples": eval_examples,
        "created_at": _now_iso(),
        "created_by_commit": _git_sha(repo_root or os.getcwd()),
        "pipeline_version": pipeline_version,
        "provenance_hash": _dataset_hash(dataset_path),
    }
    return write_manifest(dataset_path, manifest)


def write_embeddings_manifest(
    embeddings_path: str | Path,
    *,
    variant: str,
    representation_id: str,
    seed: int,
    epochs: int,
    train_examples: int,
    eval_examples: int,
    checkpoint_path: Optional[str | Path] = None,
    source_dataset_path: Optional[str | Path] = None,
    extraction_method: str = "mean_pool_final_hidden",
    pipeline_version: str = "1.0",
    repo_root: Optional[str] = None,
) -> Path:
    """Write a manifest for extracted embeddings."""
    embeddings_path = Path(embeddings_path)
    manifest = {
        "artifact_type": "embeddings",
        "variant": variant,
        "representation_id": representation_id,
        "source_input": expected_source_for_variant(variant),
        "seed": seed,
        "epochs": epochs,
        "train_examples": train_examples,
        "eval_examples": eval_examples,
        "created_at": _now_iso(),
        "created_by_commit": _git_sha(repo_root or os.getcwd()),
        "pipeline_version": pipeline_version,
        "provenance_hash": _file_content_hash(embeddings_path),
        "checkpoint_path": str(checkpoint_path) if checkpoint_path else None,
        "source_dataset_path": str(source_dataset_path) if source_dataset_path else None,
        "extraction_method": extraction_method,
    }
    return write_manifest(embeddings_path, manifest)


def write_ground_truth_manifest(
    gt_path: str | Path,
    *,
    representation_id: str,
    seed: int,
    train_examples: int,
    eval_examples: int,
    corpus_path: Optional[str | Path] = None,
    pipeline_version: str = "1.0",
    repo_root: Optional[str] = None,
) -> Path:
    """Write a manifest for a ground-truth file (e.g. dependency_groundtruth.json)."""
    gt_path = Path(gt_path)
    manifest = {
        "artifact_type": "ground_truth",
        "variant": "multi",
        "representation_id": representation_id,
        "source_input": "ir_tokens",
        "seed": seed,
        "epochs": 0,
        "train_examples": train_examples,
        "eval_examples": eval_examples,
        "created_at": _now_iso(),
        "created_by_commit": _git_sha(repo_root or os.getcwd()),
        "pipeline_version": pipeline_version,
        "provenance_hash": _file_content_hash(gt_path),
        "corpus_path": str(corpus_path) if corpus_path else None,
    }
    return write_manifest(gt_path, manifest)


def write_checkpoint_manifest(
    checkpoint_dir: str | Path,
    *,
    variant: str,
    representation_id: str,
    seed: int,
    epochs: int,
    train_examples: int,
    eval_examples: int,
    results_path: Optional[str | Path] = None,
    pipeline_version: str = "1.0",
    repo_root: Optional[str] = None,
) -> Path:
    """Write a manifest for a checkpoint directory."""
    checkpoint_dir = Path(checkpoint_dir)
    # Hash model.safetensors if present (the main weights file)
    weights = checkpoint_dir / "model.safetensors"
    if weights.exists():
        ph = _file_content_hash(weights)
    else:
        ph = f"sha256:dir:{hashlib.sha256(str(checkpoint_dir).encode()).hexdigest()[:16]}"

    manifest = {
        "artifact_type": "checkpoint",
        "variant": variant,
        "representation_id": representation_id,
        "source_input": expected_source_for_variant(variant),
        "seed": seed,
        "epochs": epochs,
        "train_examples": train_examples,
        "eval_examples": eval_examples,
        "created_at": _now_iso(),
        "created_by_commit": _git_sha(repo_root or os.getcwd()),
        "pipeline_version": pipeline_version,
        "provenance_hash": ph,
        "results_path": str(results_path) if results_path else None,
    }
    return write_manifest(checkpoint_dir, manifest)
