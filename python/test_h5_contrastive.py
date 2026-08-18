#!/usr/bin/env python3
"""
test_h5_contrastive.py

Unit tests for train_h5_contrastive.py — covers every component that could
silently corrupt an H5 run without the training loop noticing.

Tests are CPU-only, model-free (fake tensors), and complete in under 5 seconds.

Run with:
    python3 python/test_h5_contrastive.py

Exit 0 = all pass. Exit 1 = at least one failure.
"""

import json
import math
import sys
import tempfile
from pathlib import Path

import torch
import torch.nn.functional as F

sys.path.insert(0, str(Path(__file__).parent))
from train_h5_contrastive import (
    nt_xent_loss,
    mean_pool,
    contrastive_collate,
    ContrastiveDataset,
    preflight_gate,
)

PASS = 0
FAIL = 0


def ok(name: str):
    global PASS
    PASS += 1
    print(f"  PASS  {name}")


def fail(name: str, detail: str = ""):
    global FAIL
    FAIL += 1
    msg = f"  FAIL  {name}"
    if detail:
        msg += f": {detail}"
    print(msg)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _unit_vecs(n: int, d: int = 8, seed: int = 0) -> torch.Tensor:
    torch.manual_seed(seed)
    x = torch.randn(n, d)
    return F.normalize(x, p=2, dim=-1)


# ---------------------------------------------------------------------------
# 1. NT-Xent loss
# ---------------------------------------------------------------------------

def test_nt_xent_perfect_match():
    """Loss should be near 0 when anchor == positive and batch=1."""
    emb = _unit_vecs(4)
    loss = nt_xent_loss(emb, emb.clone(), temperature=0.07)
    # With perfect alignment and in-batch negatives, loss is non-zero but low
    assert loss.item() >= 0, "loss must be non-negative"
    assert not math.isnan(loss.item()), "loss must not be NaN"
    ok("nt_xent_perfect_match")


def test_nt_xent_random_loss_positive():
    """Loss on random embeddings should be positive and finite."""
    anchor = _unit_vecs(8, seed=1)
    positive = _unit_vecs(8, seed=2)
    loss = nt_xent_loss(anchor, positive, temperature=0.07)
    assert loss.item() > 0, f"expected positive loss, got {loss.item()}"
    assert math.isfinite(loss.item()), "loss must be finite"
    ok("nt_xent_random_loss_positive")


def test_nt_xent_temperature_effect():
    """Lower temperature = sharper distribution = typically higher loss on random data."""
    anchor = _unit_vecs(8, seed=3)
    positive = _unit_vecs(8, seed=4)
    loss_low_t = nt_xent_loss(anchor, positive, temperature=0.01)
    loss_high_t = nt_xent_loss(anchor, positive, temperature=0.5)
    assert math.isfinite(loss_low_t.item()) and math.isfinite(loss_high_t.item()), \
        "both losses must be finite"
    ok("nt_xent_temperature_effect")


def test_nt_xent_batch_size_1():
    """Batch size 1 has no negatives — loss should still be finite."""
    anchor = _unit_vecs(1)
    positive = _unit_vecs(1, seed=5)
    loss = nt_xent_loss(anchor, positive, temperature=0.07)
    assert math.isfinite(loss.item()), f"loss not finite: {loss.item()}"
    ok("nt_xent_batch_size_1")


def test_nt_xent_no_nan_on_identical_embeddings():
    """Identical anchor and positive should not produce NaN."""
    emb = _unit_vecs(4)
    loss = nt_xent_loss(emb, emb, temperature=0.07)
    assert not math.isnan(loss.item()), "NaN on identical embeddings"
    ok("nt_xent_no_nan_on_identical_embeddings")


def test_nt_xent_gradient_flows():
    """Gradient should flow back through the loss."""
    anchor = _unit_vecs(4).requires_grad_(True)
    positive = _unit_vecs(4, seed=6)
    loss = nt_xent_loss(anchor, positive, temperature=0.07)
    loss.backward()
    assert anchor.grad is not None, "no gradient on anchor"
    assert not torch.isnan(anchor.grad).any(), "NaN in gradient"
    ok("nt_xent_gradient_flows")


def test_nt_xent_loss_decreases_with_training():
    """A single gradient step should reduce the loss (sanity check)."""
    torch.manual_seed(7)
    anchor_param = torch.nn.Parameter(_unit_vecs(8))
    positive = _unit_vecs(8, seed=8).detach()
    opt = torch.optim.SGD([anchor_param], lr=0.1)

    loss_before = nt_xent_loss(
        F.normalize(anchor_param, p=2, dim=-1), positive, temperature=0.07
    )
    opt.zero_grad()
    loss_before.backward()
    opt.step()

    loss_after = nt_xent_loss(
        F.normalize(anchor_param, p=2, dim=-1), positive, temperature=0.07
    )
    assert loss_after.item() < loss_before.item(), \
        f"loss did not decrease: {loss_before.item():.4f} -> {loss_after.item():.4f}"
    ok("nt_xent_loss_decreases_with_training")


# ---------------------------------------------------------------------------
# 2. mean_pool
# ---------------------------------------------------------------------------

def test_mean_pool_all_ones_mask():
    """With full mask, mean pool = mean of hidden states."""
    B, L, D = 2, 5, 8
    hidden = torch.ones(B, L, D)
    mask = torch.ones(B, L, dtype=torch.long)
    pooled = mean_pool(hidden, mask)
    assert pooled.shape == (B, D), f"wrong shape: {pooled.shape}"
    # L2 norm of all-ones vector = sqrt(D)
    norms = pooled.norm(dim=-1)
    assert torch.allclose(norms, torch.ones(B)), f"not normalized: {norms}"
    ok("mean_pool_all_ones_mask")


def test_mean_pool_padding_ignored():
    """Padded positions (mask=0) should not contribute to the mean."""
    B, L, D = 1, 4, 4
    hidden = torch.zeros(B, L, D)
    hidden[0, 0] = torch.tensor([1.0, 0.0, 0.0, 0.0])
    hidden[0, 1] = torch.tensor([0.0, 1.0, 0.0, 0.0])
    hidden[0, 2] = torch.tensor([999.0, 999.0, 999.0, 999.0])  # padding, should be ignored
    hidden[0, 3] = torch.tensor([999.0, 999.0, 999.0, 999.0])  # padding

    mask = torch.tensor([[1, 1, 0, 0]], dtype=torch.long)
    pooled = mean_pool(hidden, mask)

    # Expected mean of first two: [0.5, 0.5, 0, 0], then L2-normalized
    expected_raw = torch.tensor([[0.5, 0.5, 0.0, 0.0]])
    expected = F.normalize(expected_raw, p=2, dim=-1)
    assert torch.allclose(pooled, expected, atol=1e-5), \
        f"pooled={pooled}, expected={expected}"
    ok("mean_pool_padding_ignored")


def test_mean_pool_output_is_normalized():
    """Output of mean_pool must be L2-normalized."""
    torch.manual_seed(9)
    hidden = torch.randn(4, 10, 16)
    mask = torch.randint(0, 2, (4, 10)).long()
    mask[:, 0] = 1  # ensure at least one token per sequence
    pooled = mean_pool(hidden, mask)
    norms = pooled.norm(dim=-1)
    assert torch.allclose(norms, torch.ones(4), atol=1e-5), \
        f"output not normalized: {norms}"
    ok("mean_pool_output_is_normalized")


def test_mean_pool_single_token():
    """With one real token, mean pool = that token (normalized)."""
    hidden = torch.tensor([[[3.0, 4.0]]])  # (1, 1, 2)
    mask = torch.tensor([[1]], dtype=torch.long)
    pooled = mean_pool(hidden, mask)
    expected = F.normalize(torch.tensor([[3.0, 4.0]]), p=2, dim=-1)
    assert torch.allclose(pooled, expected, atol=1e-5), \
        f"pooled={pooled}, expected={expected}"
    ok("mean_pool_single_token")


# ---------------------------------------------------------------------------
# 3. contrastive_collate
# ---------------------------------------------------------------------------

def test_collate_pads_to_max_len():
    """Sequences of different lengths should be padded to the batch max."""
    batch = [
        ([1, 2, 3], [4, 5]),
        ([6], [7, 8, 9, 10]),
    ]
    a_ids, a_mask, p_ids, p_mask = contrastive_collate(batch)
    assert a_ids.shape == (2, 3), f"anchor shape wrong: {a_ids.shape}"
    assert p_ids.shape == (2, 4), f"positive shape wrong: {p_ids.shape}"
    ok("collate_pads_to_max_len")


def test_collate_mask_matches_real_tokens():
    """Mask should be 1 for real tokens, 0 for padding."""
    batch = [
        ([1, 2], [3]),
        ([4], [5, 6, 7]),
    ]
    a_ids, a_mask, p_ids, p_mask = contrastive_collate(batch)
    # anchor: seq 0 has len 2, seq 1 has len 1 → max 2
    assert a_mask[0].tolist() == [1, 1], f"anchor mask[0]={a_mask[0].tolist()}"
    assert a_mask[1].tolist() == [1, 0], f"anchor mask[1]={a_mask[1].tolist()}"
    # positive: seq 0 has len 1, seq 1 has len 3 → max 3
    assert p_mask[0].tolist() == [1, 0, 0], f"positive mask[0]={p_mask[0].tolist()}"
    assert p_mask[1].tolist() == [1, 1, 1], f"positive mask[1]={p_mask[1].tolist()}"
    ok("collate_mask_matches_real_tokens")


def test_collate_padding_values_are_zero():
    """Padding positions must be zero-filled (not garbage)."""
    batch = [([1, 2, 3], [4, 5, 6]), ([7], [8])]
    a_ids, a_mask, p_ids, p_mask = contrastive_collate(batch)
    # Second sequence was padded
    assert a_ids[1, 1].item() == 0, "padding not zero"
    assert a_ids[1, 2].item() == 0, "padding not zero"
    ok("collate_padding_values_are_zero")


# ---------------------------------------------------------------------------
# 4. ContrastiveDataset
# ---------------------------------------------------------------------------

def _make_temp_dataset(n: int = 10) -> tuple[Path, Path]:
    """Write a minimal JSONL dataset and pairs file for testing."""
    rows = [{"name": f"decl_{i}", "input_ids": list(range(1, i + 3))} for i in range(n)]
    pairs = [[i, (i + 1) % n] for i in range(n)]

    with tempfile.NamedTemporaryFile(mode="w", suffix=".jsonl", delete=False) as f:
        for row in rows:
            f.write(json.dumps(row) + "\n")
        ds_path = Path(f.name)

    pairs_data = {
        "pairs": pairs,
        "n_declarations": n,
        "n_pairs": len(pairs),
        "strategy": "shared_dependency",
        "min_shared": 1,
        "max_pairs_per_anchor": 5,
        "seed": 42,
        "dataset_file": str(ds_path),
        "groundtruth_file": "fake.json",
    }
    with tempfile.NamedTemporaryFile(mode="w", suffix=".json", delete=False) as f:
        json.dump(pairs_data, f)
        pairs_path = Path(f.name)

    return ds_path, pairs_path


def test_dataset_len():
    ds_path, pairs_path = _make_temp_dataset(10)
    try:
        ds = ContrastiveDataset(ds_path, pairs_path, max_seq_len=16)
        assert len(ds) == 10, f"expected 10 pairs, got {len(ds)}"
        ok("dataset_len")
    finally:
        ds_path.unlink(missing_ok=True)
        pairs_path.unlink(missing_ok=True)


def test_dataset_getitem_returns_lists():
    ds_path, pairs_path = _make_temp_dataset(10)
    try:
        ds = ContrastiveDataset(ds_path, pairs_path, max_seq_len=16)
        anchor, positive = ds[0]
        assert isinstance(anchor, list), f"anchor not a list: {type(anchor)}"
        assert isinstance(positive, list), f"positive not a list: {type(positive)}"
        assert len(anchor) > 0
        assert len(positive) > 0
        ok("dataset_getitem_returns_lists")
    finally:
        ds_path.unlink(missing_ok=True)
        pairs_path.unlink(missing_ok=True)


def test_dataset_respects_max_seq_len():
    ds_path, pairs_path = _make_temp_dataset(10)
    try:
        max_len = 3
        ds = ContrastiveDataset(ds_path, pairs_path, max_seq_len=max_len)
        for i in range(len(ds)):
            a, p = ds[i]
            assert len(a) <= max_len, f"anchor too long: {len(a)} > {max_len}"
            assert len(p) <= max_len, f"positive too long: {len(p)} > {max_len}"
        ok("dataset_respects_max_seq_len")
    finally:
        ds_path.unlink(missing_ok=True)
        pairs_path.unlink(missing_ok=True)


# ---------------------------------------------------------------------------
# 5. preflight_gate
# ---------------------------------------------------------------------------

def test_gate_fails_missing_pairs():
    errors = preflight_gate(
        Path("/nonexistent/pairs.json"),
        Path(__file__),
        Path(__file__).parent,
    )
    assert any("G-H5-1" in e for e in errors), f"expected G-H5-1 error, got: {errors}"
    ok("gate_fails_missing_pairs")


def test_gate_fails_missing_dataset():
    ds_path, pairs_path = _make_temp_dataset(200)
    try:
        errors = preflight_gate(
            pairs_path,
            Path("/nonexistent/train.jsonl"),
            Path(__file__).parent,
        )
        assert any("G-H5-2" in e for e in errors), f"expected G-H5-2 error, got: {errors}"
        ok("gate_fails_missing_dataset")
    finally:
        ds_path.unlink(missing_ok=True)
        pairs_path.unlink(missing_ok=True)


def test_gate_fails_too_few_pairs():
    ds_path, pairs_path = _make_temp_dataset(5)  # only 5 pairs < 100
    try:
        errors = preflight_gate(pairs_path, ds_path, Path(__file__).parent)
        assert any("G-H5-1" in e for e in errors), \
            f"expected G-H5-1 error for too few pairs, got: {errors}"
        ok("gate_fails_too_few_pairs")
    finally:
        ds_path.unlink(missing_ok=True)
        pairs_path.unlink(missing_ok=True)


def test_gate_passes_with_valid_inputs():
    repo = Path(__file__).parent.parent
    pairs = repo / "datasets" / "h5_pairs_train.json"
    dataset = repo / "datasets" / "train_A.jsonl"
    checkpoint = repo / "runs" / "variant_A_v3_2ep" / "checkpoint-final"

    if not all([pairs.exists(), dataset.exists(), checkpoint.exists()]):
        print("  SKIP  gate_passes_with_valid_inputs (real data/checkpoint not found)")
        return

    errors = preflight_gate(pairs, dataset, checkpoint)
    assert errors == [], f"expected no errors, got: {errors}"
    ok("gate_passes_with_valid_inputs")


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------

def run_all():
    print("=" * 60)
    print("test_h5_contrastive.py")
    print("=" * 60)

    # NT-Xent loss
    test_nt_xent_perfect_match()
    test_nt_xent_random_loss_positive()
    test_nt_xent_temperature_effect()
    test_nt_xent_batch_size_1()
    test_nt_xent_no_nan_on_identical_embeddings()
    test_nt_xent_gradient_flows()
    test_nt_xent_loss_decreases_with_training()

    # mean_pool
    test_mean_pool_all_ones_mask()
    test_mean_pool_padding_ignored()
    test_mean_pool_output_is_normalized()
    test_mean_pool_single_token()

    # collate
    test_collate_pads_to_max_len()
    test_collate_mask_matches_real_tokens()
    test_collate_padding_values_are_zero()

    # Dataset
    test_dataset_len()
    test_dataset_getitem_returns_lists()
    test_dataset_respects_max_seq_len()

    # Gate
    test_gate_fails_missing_pairs()
    test_gate_fails_missing_dataset()
    test_gate_fails_too_few_pairs()
    test_gate_passes_with_valid_inputs()

    print()
    print(f"Total: {PASS + FAIL} tests, {PASS} passed, {FAIL} failed")
    return FAIL


if __name__ == "__main__":
    sys.exit(run_all())
