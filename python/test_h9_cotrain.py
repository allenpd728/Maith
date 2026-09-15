#!/usr/bin/env python3
"""
test_h9_cotrain.py

Unit tests for train_h9_cotrain.py — covers every component that could
silently corrupt an H9 run without the training loop noticing.

Tests are CPU-only, model-free (fake tensors/files), complete in under 10 seconds.

Run with:
    python3 python/test_h9_cotrain.py

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
from train_h9_cotrain import (
    nt_xent_loss,
    mean_pool,
    ntp_collate,
    NTPDataset,
    PairSampler,
    preflight_gate,
    CONTRASTIVE_LAMBDA,
    CONTRASTIVE_TEMPERATURE,
    SEED,
    EPOCHS,
    BATCH_SIZE,
    GRAD_ACCUM,
    LEARNING_RATE,
    MAX_SEQ_LEN,
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


def _unit_vecs(n: int, d: int = 8, seed: int = 0) -> torch.Tensor:
    torch.manual_seed(seed)
    x = torch.randn(n, d)
    return F.normalize(x, p=2, dim=-1)


# ---------------------------------------------------------------------------
# 1. NT-Xent loss (same function as H5 — re-test here for isolation)
# ---------------------------------------------------------------------------

def test_nt_xent_non_negative():
    emb = _unit_vecs(4)
    loss = nt_xent_loss(emb, emb.clone())
    assert loss.item() >= 0, f"loss={loss.item()} is negative"
    assert not math.isnan(loss.item()), "loss is NaN"
    ok("nt_xent_non_negative")


def test_nt_xent_random_positive_finite():
    a = _unit_vecs(8, seed=1)
    p = _unit_vecs(8, seed=2)
    loss = nt_xent_loss(a, p)
    assert loss.item() > 0
    assert math.isfinite(loss.item())
    ok("nt_xent_random_positive_finite")


def test_nt_xent_temperature_sensitivity():
    """Lower temperature → sharper distribution → higher loss on random pairs."""
    a = _unit_vecs(8, seed=3)
    p = _unit_vecs(8, seed=4)
    loss_low  = nt_xent_loss(a, p, temperature=0.01)
    loss_high = nt_xent_loss(a, p, temperature=1.0)
    assert loss_low.item() >= loss_high.item(), (
        f"Expected low-temp loss ({loss_low.item():.4f}) >= "
        f"high-temp loss ({loss_high.item():.4f})"
    )
    ok("nt_xent_temperature_sensitivity")


def test_nt_xent_batch_size_1():
    """Batch size 1: loss should still be finite (edge case)."""
    a = _unit_vecs(1, seed=5)
    p = _unit_vecs(1, seed=6)
    loss = nt_xent_loss(a, p)
    assert math.isfinite(loss.item())
    ok("nt_xent_batch_size_1")


def test_nt_xent_gradient_flows():
    """Loss must be differentiable — .backward() should not raise."""
    a = _unit_vecs(4, seed=7).requires_grad_(True)
    p = _unit_vecs(4, seed=8).requires_grad_(True)
    loss = nt_xent_loss(a, p)
    loss.backward()
    assert a.grad is not None, "no gradient on anchor"
    assert p.grad is not None, "no gradient on positive"
    ok("nt_xent_gradient_flows")


# ---------------------------------------------------------------------------
# 2. mean_pool
# ---------------------------------------------------------------------------

def test_mean_pool_shape():
    hidden = torch.randn(4, 10, 8)
    mask   = torch.ones(4, 10)
    out = mean_pool(hidden, mask)
    assert out.shape == (4, 8), f"expected (4,8) got {out.shape}"
    ok("mean_pool_shape")


def test_mean_pool_normalized():
    hidden = torch.randn(4, 10, 8)
    mask   = torch.ones(4, 10)
    out = mean_pool(hidden, mask)
    norms = out.norm(dim=-1)
    assert torch.allclose(norms, torch.ones(4), atol=1e-5), f"not unit norm: {norms}"
    ok("mean_pool_normalized")


def test_mean_pool_ignores_padding():
    """Masked tokens should not affect the mean."""
    hidden = torch.ones(2, 5, 4)
    hidden[0, 3:] = 999.0          # garbage in padding positions
    mask = torch.tensor([[1,1,1,0,0],[1,1,1,1,1]], dtype=torch.float)
    out = mean_pool(hidden, mask)
    # Both rows should produce the same normalized direction
    assert torch.allclose(out[0], out[1], atol=1e-5), (
        f"padding not ignored: {out[0]} vs {out[1]}"
    )
    ok("mean_pool_ignores_padding")


# ---------------------------------------------------------------------------
# 3. ntp_collate
# ---------------------------------------------------------------------------

def _make_ntp_batch(lengths):
    return [
        {
            "input_ids":      torch.arange(n, dtype=torch.long),
            "attention_mask": torch.ones(n, dtype=torch.long),
            "labels":         torch.arange(n, dtype=torch.long),
        }
        for n in lengths
    ]


def test_ntp_collate_pads_to_max():
    batch = _make_ntp_batch([3, 7, 5])
    out = ntp_collate(batch)
    assert out["input_ids"].shape == (3, 7), f"shape={out['input_ids'].shape}"
    ok("ntp_collate_pads_to_max")


def test_ntp_collate_label_padding():
    """Short sequences should have -100 in the padded label positions."""
    batch = _make_ntp_batch([3, 7])
    out = ntp_collate(batch)
    assert (out["labels"][0, 3:] == -100).all(), "label padding incorrect"
    ok("ntp_collate_label_padding")


def test_ntp_collate_attention_mask():
    batch = _make_ntp_batch([4, 6])
    out = ntp_collate(batch)
    assert out["attention_mask"][0, 4:].sum() == 0, "mask not zero in padding"
    ok("ntp_collate_attention_mask")


# ---------------------------------------------------------------------------
# 4. NTPDataset
# ---------------------------------------------------------------------------

def _write_jsonl(path: Path, rows: list):
    with open(path, "w") as f:
        for r in rows:
            f.write(json.dumps(r) + "\n")


def test_ntp_dataset_loads():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "train_A.jsonl"
        _write_jsonl(p, [
            {"input_ids": [1,2,3], "labels": [1,2,3], "source": "A", "name": "x"},
            {"input_ids": [4,5],   "labels": [4,5],   "source": "A", "name": "y"},
        ])
        ds = NTPDataset(p, expected_source="A")
        assert len(ds) == 2
        ok("ntp_dataset_loads")


def test_ntp_dataset_source_check():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "train_A.jsonl"
        _write_jsonl(p, [
            {"input_ids": [1,2], "labels": [1,2], "source": "B", "name": "z"},
        ])
        raised = False
        try:
            NTPDataset(p, expected_source="A")
        except AssertionError:
            raised = True
        assert raised, "Should have raised AssertionError for wrong source"
        ok("ntp_dataset_source_check")


def test_ntp_dataset_truncates():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "train_A.jsonl"
        long_ids = list(range(1000))
        _write_jsonl(p, [
            {"input_ids": long_ids, "labels": long_ids, "source": "A", "name": "a"},
        ])
        ds = NTPDataset(p, max_len=10, expected_source="A")
        assert ds[0]["input_ids"].shape[0] == 10
        ok("ntp_dataset_truncates")


def test_ntp_dataset_limit():
    with tempfile.TemporaryDirectory() as td:
        p = Path(td) / "train_A.jsonl"
        _write_jsonl(p, [
            {"input_ids": [i], "labels": [i], "source": "A", "name": f"x{i}"}
            for i in range(20)
        ])
        ds = NTPDataset(p, limit=5, expected_source="A")
        assert len(ds) == 5
        ok("ntp_dataset_limit")


# ---------------------------------------------------------------------------
# 5. PairSampler
# ---------------------------------------------------------------------------

def _write_pairs(path: Path, rows_path: Path, n_rows: int = 10, n_pairs: int = 8):
    rows = [{"input_ids": list(range(i, i+3)), "labels": list(range(i, i+3)),
             "source": "A", "name": f"decl{i}"} for i in range(n_rows)]
    _write_jsonl(rows_path, rows)
    pairs_data = {
        "n_pairs": n_pairs,
        "pairs":   [[i % n_rows, (i+1) % n_rows] for i in range(n_pairs)],
    }
    with open(path, "w") as f:
        json.dump(pairs_data, f)


def test_pair_sampler_basic():
    with tempfile.TemporaryDirectory() as td:
        rows_p  = Path(td) / "train_A.jsonl"
        pairs_p = Path(td) / "pairs.json"
        _write_pairs(pairs_p, rows_p)
        ps = PairSampler(pairs_p, rows_p, seed=0)
        assert ps.n_pairs == 8
        ok("pair_sampler_basic")


def test_pair_sampler_batch_shape():
    with tempfile.TemporaryDirectory() as td:
        rows_p  = Path(td) / "train_A.jsonl"
        pairs_p = Path(td) / "pairs.json"
        _write_pairs(pairs_p, rows_p)
        ps = PairSampler(pairs_p, rows_p, seed=0)
        batch = ps.sample_batch(4)
        assert batch is not None
        a_ids, a_mask, p_ids, p_mask = batch
        assert a_ids.shape[0] == 4
        assert p_ids.shape[0] == 4
        ok("pair_sampler_batch_shape")


def test_pair_sampler_returns_none_when_too_few():
    """If n_pairs < batch_size, sample_batch returns None."""
    with tempfile.TemporaryDirectory() as td:
        rows_p  = Path(td) / "train_A.jsonl"
        pairs_p = Path(td) / "pairs.json"
        _write_pairs(pairs_p, rows_p, n_pairs=3)
        ps = PairSampler(pairs_p, rows_p, seed=0)
        result = ps.sample_batch(10)
        assert result is None
        ok("pair_sampler_returns_none_when_too_few")


def test_pair_sampler_wraps_epoch():
    """Sampler should wrap around and produce more batches than n_pairs."""
    with tempfile.TemporaryDirectory() as td:
        rows_p  = Path(td) / "train_A.jsonl"
        pairs_p = Path(td) / "pairs.json"
        _write_pairs(pairs_p, rows_p, n_rows=10, n_pairs=8)
        ps = PairSampler(pairs_p, rows_p, seed=0)
        # Draw 3 batches of 4 — requires 12 pairs from a pool of 8 (wraps once)
        results = [ps.sample_batch(4) for _ in range(3)]
        assert all(r is not None for r in results)
        ok("pair_sampler_wraps_epoch")


# ---------------------------------------------------------------------------
# 6. preflight_gate
# ---------------------------------------------------------------------------

def _make_preflight_fixture(tmp: Path):
    rows = [{"input_ids": [i], "labels": [i], "source": "A", "name": f"d{i}"}
            for i in range(200)]
    dataset_path = tmp / "train_A.jsonl"
    _write_jsonl(dataset_path, rows)

    pairs_data = {
        "n_pairs": 120,
        "pairs":   [[i % 200, (i+1) % 200] for i in range(120)],
    }
    pairs_path = tmp / "pairs.json"
    with open(pairs_path, "w") as f:
        json.dump(pairs_data, f)

    vocab_path = tmp / "vocab_A.json"
    with open(vocab_path, "w") as f:
        json.dump({str(i): i for i in range(100)}, f)

    return dataset_path, pairs_path, vocab_path


def test_preflight_passes():
    with tempfile.TemporaryDirectory() as td:
        dataset_p, pairs_p, vocab_p = _make_preflight_fixture(Path(td))
        errors = preflight_gate(pairs_p, dataset_p, vocab_p)
        assert errors == [], f"Unexpected errors: {errors}"
        ok("preflight_passes")


def test_preflight_missing_pairs():
    with tempfile.TemporaryDirectory() as td:
        dataset_p, _, vocab_p = _make_preflight_fixture(Path(td))
        errors = preflight_gate(Path(td) / "nonexistent.json", dataset_p, vocab_p)
        assert any("G-H9-1" in e for e in errors)
        ok("preflight_missing_pairs")


def test_preflight_missing_dataset():
    with tempfile.TemporaryDirectory() as td:
        _, pairs_p, vocab_p = _make_preflight_fixture(Path(td))
        errors = preflight_gate(pairs_p, Path(td) / "nonexistent.jsonl", vocab_p)
        assert any("G-H9-2" in e for e in errors)
        ok("preflight_missing_dataset")


def test_preflight_missing_vocab():
    with tempfile.TemporaryDirectory() as td:
        dataset_p, pairs_p, _ = _make_preflight_fixture(Path(td))
        errors = preflight_gate(pairs_p, dataset_p, Path(td) / "nonexistent.json")
        assert any("G-H9-3" in e for e in errors)
        ok("preflight_missing_vocab")


def test_preflight_out_of_range_pairs():
    with tempfile.TemporaryDirectory() as td:
        dataset_p, _, vocab_p = _make_preflight_fixture(Path(td))
        bad_pairs = Path(td) / "bad_pairs.json"
        with open(bad_pairs, "w") as f:
            json.dump({"n_pairs": 5, "pairs": [[0, 999], [1, 2], [3, 4], [5, 6], [7, 8]]}, f)
        errors = preflight_gate(bad_pairs, dataset_p, vocab_p)
        assert any("G-H9-4" in e for e in errors), f"errors={errors}"
        ok("preflight_out_of_range_pairs")


# ---------------------------------------------------------------------------
# 7. Constants match EXPERIMENT_DESIGN.md standard protocol
# ---------------------------------------------------------------------------

def test_constants_match_protocol():
    """Key training constants must match the fixed protocol from EXPERIMENT_DESIGN.md."""
    assert EPOCHS == 2,          f"EPOCHS={EPOCHS}, expected 2"
    assert BATCH_SIZE == 1,      f"BATCH_SIZE={BATCH_SIZE}, expected 1"
    assert GRAD_ACCUM == 8,      f"GRAD_ACCUM={GRAD_ACCUM}, expected 8"
    assert LEARNING_RATE == 2e-4, f"LEARNING_RATE={LEARNING_RATE}, expected 2e-4"
    assert SEED == 42,           f"SEED={SEED}, expected 42"
    assert MAX_SEQ_LEN == 512,   f"MAX_SEQ_LEN={MAX_SEQ_LEN}, expected 512"
    ok("constants_match_protocol")


def test_lambda_in_valid_range():
    assert 0.0 < CONTRASTIVE_LAMBDA < 1.0, (
        f"CONTRASTIVE_LAMBDA={CONTRASTIVE_LAMBDA} must be in (0, 1)")
    ok("lambda_in_valid_range")


def test_temperature_positive():
    assert CONTRASTIVE_TEMPERATURE > 0, (
        f"CONTRASTIVE_TEMPERATURE={CONTRASTIVE_TEMPERATURE} must be > 0")
    ok("temperature_positive")


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    tests = [
        # NT-Xent
        test_nt_xent_non_negative,
        test_nt_xent_random_positive_finite,
        test_nt_xent_temperature_sensitivity,
        test_nt_xent_batch_size_1,
        test_nt_xent_gradient_flows,
        # mean_pool
        test_mean_pool_shape,
        test_mean_pool_normalized,
        test_mean_pool_ignores_padding,
        # ntp_collate
        test_ntp_collate_pads_to_max,
        test_ntp_collate_label_padding,
        test_ntp_collate_attention_mask,
        # NTPDataset
        test_ntp_dataset_loads,
        test_ntp_dataset_source_check,
        test_ntp_dataset_truncates,
        test_ntp_dataset_limit,
        # PairSampler
        test_pair_sampler_basic,
        test_pair_sampler_batch_shape,
        test_pair_sampler_returns_none_when_too_few,
        test_pair_sampler_wraps_epoch,
        # preflight_gate
        test_preflight_passes,
        test_preflight_missing_pairs,
        test_preflight_missing_dataset,
        test_preflight_missing_vocab,
        test_preflight_out_of_range_pairs,
        # constants
        test_constants_match_protocol,
        test_lambda_in_valid_range,
        test_temperature_positive,
    ]

    print(f"Running {len(tests)} tests ...\n")
    for t in tests:
        try:
            t()
        except Exception as e:
            fail(t.__name__, str(e))

    print(f"\n{'=' * 40}")
    print(f"Results: {PASS} passed, {FAIL} failed")
    if FAIL:
        print("SOME TESTS FAILED — do not proceed to training.")
        sys.exit(1)
    else:
        print("All tests passed.")
        sys.exit(0)
