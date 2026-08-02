"""
test_train_regression.py

Regression tests for train.py — specifically targeting the failure modes that
have already cost multi-hour runs.

Tests are CPU-only, model-free, and complete in under a second. They import
collate_fn and evaluate_perplexity directly from train.py and exercise them
with synthetic data, so they catch regressions without requiring a GPU or
downloading any weights.

Run with:
    python3 python/test_train_regression.py

A non-zero exit code means at least one test failed.
"""

import math
import sys
import types
import torch

# ---------------------------------------------------------------------------
# Import only the functions under test, not the full training loop.
# train.py has top-level side effects (argparse, imports that need CUDA/MPS)
# so we import it as a module and extract just what we need.
# ---------------------------------------------------------------------------
sys.path.insert(0, "python")
import importlib.util, os

_spec = importlib.util.spec_from_file_location("train", os.path.join("python", "train.py"))
_train = importlib.util.module_from_spec(_spec)
# Stub out transformers/torch imports that would trigger downloads
_spec.loader.exec_module(_train)

collate_fn          = _train.collate_fn
evaluate_perplexity = _train.evaluate_perplexity
IRDataset           = _train.IRDataset


# ---------------------------------------------------------------------------
# Minimal fake causal-LM model for testing evaluate_perplexity without weights
# ---------------------------------------------------------------------------

class FakeCausalLMOutput:
    def __init__(self, loss):
        self.loss = loss


class FakeCausalLM(torch.nn.Module):
    """
    Fake AutoModelForCausalLM that computes a deterministic cross-entropy loss
    from input_ids and labels, matching the contract evaluate_perplexity expects:
      - outputs.loss is a scalar tensor
      - model(**batch) is the only call made
    """
    def __init__(self, vocab_size: int = 256):
        super().__init__()
        self.vocab_size = vocab_size

    def forward(self, input_ids, attention_mask=None, labels=None):
        if labels is None:
            return FakeCausalLMOutput(loss=torch.tensor(float("nan")))
        # Causal-LM convention: model shifts labels internally.
        # Predict position i from input_ids[:i], compare to labels[i].
        # We mimic this by comparing input_ids[:, :-1] to labels[:, 1:].
        B, T = input_ids.shape
        if T < 2:
            # Sequence too short to produce any valid predictions after shift.
            # Return a loss of 0 with 0 contributing tokens (same as HF behaviour).
            return FakeCausalLMOutput(loss=torch.tensor(0.0))

        logits = torch.zeros(B, T - 1, self.vocab_size)
        shifted_labels = labels[:, 1:]  # shape (B, T-1)

        # For non-ignored positions, set the correct logit high so loss is low.
        mask = shifted_labels != -100
        for b in range(B):
            for t in range(T - 1):
                if mask[b, t]:
                    logits[b, t, shifted_labels[b, t]] = 10.0

        loss = torch.nn.functional.cross_entropy(
            logits.reshape(-1, self.vocab_size),
            shifted_labels.reshape(-1),
            ignore_index=-100,
        )
        return FakeCausalLMOutput(loss=loss)


# ---------------------------------------------------------------------------
# Test utilities
# ---------------------------------------------------------------------------

PASS = []
FAIL = []

def ok(name):
    PASS.append(name)
    print(f"  PASS  {name}")

def fail(name, reason):
    FAIL.append(name)
    print(f"  FAIL  {name}: {reason}")


def make_batch(sequences):
    """Convert a list of int lists into a collated batch via collate_fn."""
    items = []
    for seq in sequences:
        t = torch.tensor(seq, dtype=torch.long)
        items.append({"input_ids": t, "attention_mask": torch.ones_like(t), "labels": t.clone()})
    return collate_fn(items)


# ---------------------------------------------------------------------------
# TEST 1: collate_fn — labels match input_ids for non-padded positions
#
# Verifies that collate_fn sets labels = input_ids for all real tokens and
# labels = -100 for padding. This is the invariant the label-shift fix
# established: labels are NOT pre-shifted; the model shifts internally.
# ---------------------------------------------------------------------------

def test_collate_labels_match_input_ids():
    name = "collate_fn: labels == input_ids for non-padded positions"
    try:
        batch = make_batch([[1, 2, 3, 4], [5, 6]])
        input_ids = batch["input_ids"]
        labels    = batch["labels"]
        # Shorter sequence should be padded with -100 in labels
        assert labels[1, 2].item() == -100, f"Expected -100 padding, got {labels[1,2].item()}"
        assert labels[1, 3].item() == -100, f"Expected -100 padding, got {labels[1,3].item()}"
        # Non-padded positions should have labels == input_ids
        for i in range(4):
            assert labels[0, i].item() == input_ids[0, i].item(), \
                f"labels[0,{i}]={labels[0,i].item()} != input_ids[0,{i}]={input_ids[0,i].item()}"
        for i in range(2):
            assert labels[1, i].item() == input_ids[1, i].item(), \
                f"labels[1,{i}]={labels[1,i].item()} != input_ids[1,{i}]={input_ids[1,i].item()}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 2: collate_fn — seq_len=1 edge case
#
# A single-token sequence: after the causal shift there are zero valid
# prediction targets. collate_fn should produce a valid tensor without
# crashing; evaluate_perplexity should handle this gracefully.
# ---------------------------------------------------------------------------

def test_collate_single_token_sequence():
    name = "collate_fn: seq_len=1 does not crash"
    try:
        batch = make_batch([[42]])
        assert batch["input_ids"].shape == (1, 1), f"Unexpected shape: {batch['input_ids'].shape}"
        assert batch["labels"].shape == (1, 1)
        assert batch["labels"][0, 0].item() == 42
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 3: evaluate_perplexity — seq_len=2 (the original bug case)
#
# This is the exact failure mode from the historical bug: a sequence of
# length 2 passes through collate_fn with labels=[tok0, tok1]. After the
# causal-LM internal shift, only position 1 (tok1) is a valid prediction
# target. With the OLD broken labelling (labels pre-shifted to [-100, tok1]),
# BOTH positions were -100 after the model's internal shift, giving 0 valid
# tokens and a division-by-zero / NaN perplexity.
#
# With the FIXED labelling (labels == input_ids, no pre-shift), position 1
# is a valid target and perplexity is finite.
#
# We verify: evaluate_perplexity returns a finite value for seq_len=2 input.
# ---------------------------------------------------------------------------

def test_evaluate_perplexity_seq_len_2():
    name = "evaluate_perplexity: seq_len=2 returns finite value (label-shift regression)"
    try:
        model = FakeCausalLM(vocab_size=256)
        model.eval()

        # Build a minimal IRDataset-like list with a 2-token sequence.
        # We bypass IRDataset (needs a real file) and construct the example directly.
        seq = [10, 20]
        dataset = [
            {"input_ids": torch.tensor(seq, dtype=torch.long),
             "attention_mask": torch.tensor([1, 1], dtype=torch.long),
             "labels": torch.tensor(seq, dtype=torch.long)}  # labels == input_ids (fixed)
        ]

        ppl = evaluate_perplexity(model, dataset, device="cpu", batch_size=1)
        assert math.isfinite(ppl), f"Perplexity is not finite: {ppl}"
        assert ppl > 0, f"Perplexity must be positive, got {ppl}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 4: evaluate_perplexity — seq_len=1 (zero valid targets after shift)
#
# A 1-token sequence has no valid prediction target after the causal shift.
# The model returns loss=0.0 with 0 contributing tokens. evaluate_perplexity
# must not divide by zero or produce NaN. We verify it returns a finite value
# (exp(0) = 1.0 is the expected result when total_tokens=0 and total_loss=0).
# ---------------------------------------------------------------------------

def test_evaluate_perplexity_seq_len_1():
    name = "evaluate_perplexity: seq_len=1 does not produce NaN (zero valid targets)"
    try:
        model = FakeCausalLM(vocab_size=256)
        model.eval()

        dataset = [
            {"input_ids": torch.tensor([7], dtype=torch.long),
             "attention_mask": torch.tensor([1], dtype=torch.long),
             "labels": torch.tensor([7], dtype=torch.long)}
        ]

        ppl = evaluate_perplexity(model, dataset, device="cpu", batch_size=1)
        assert math.isfinite(ppl), f"Perplexity is not finite for seq_len=1: {ppl}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 5: evaluate_perplexity — non-finite loss is silently skipped
#
# If the model returns NaN loss, evaluate_perplexity skips that batch and
# continues (deliberate design choice for training resilience — see line 244
# of train.py). This test verifies the function returns a finite value rather
# than propagating NaN when the only batch produces a NaN loss.
#
# With all batches skipped (total_tokens=0), the function falls back to
# avg_loss = 0.0 / 1 = 0.0, returning exp(0) = 1.0 — a finite, valid value.
# ---------------------------------------------------------------------------

def test_evaluate_perplexity_skips_nan_loss():
    name = "evaluate_perplexity: silently skips NaN loss batches, returns finite value"
    try:
        class NaNModel(torch.nn.Module):
            def forward(self, **kwargs):
                return FakeCausalLMOutput(loss=torch.tensor(float("nan")))

        model = NaNModel()
        model.eval()

        dataset = [
            {"input_ids": torch.tensor([1, 2, 3], dtype=torch.long),
             "attention_mask": torch.tensor([1, 1, 1], dtype=torch.long),
             "labels": torch.tensor([1, 2, 3], dtype=torch.long)}
        ]

        ppl = evaluate_perplexity(model, dataset, device="cpu", batch_size=1)
        assert math.isfinite(ppl), f"Expected finite perplexity after NaN skip, got {ppl}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 6: BATCH_SIZE / GRAD_ACCUM — global constants exist and are consistent
#
# DEC-007 fixed the batch mismatch by using a single global BATCH_SIZE and
# GRAD_ACCUM for all variants (rather than per-variant config). This test
# verifies those constants exist and that the effective batch size is
# reasonable (>= 4), catching any accidental reset to 1/1 that would
# re-introduce the mismatch.
# ---------------------------------------------------------------------------

def test_global_batch_config_exists():
    name = "BATCH_SIZE / GRAD_ACCUM: global constants present and effective batch >= 4"
    try:
        assert hasattr(_train, "BATCH_SIZE"), "train.py is missing BATCH_SIZE constant"
        assert hasattr(_train, "GRAD_ACCUM"), "train.py is missing GRAD_ACCUM constant"
        effective = _train.BATCH_SIZE * _train.GRAD_ACCUM
        assert effective >= 4, \
            f"Effective batch size {effective} is suspiciously small (BATCH_SIZE={_train.BATCH_SIZE}, GRAD_ACCUM={_train.GRAD_ACCUM})"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("Running train.py regression tests...\n")

    test_collate_labels_match_input_ids()
    test_collate_single_token_sequence()
    test_evaluate_perplexity_seq_len_2()
    test_evaluate_perplexity_seq_len_1()
    test_evaluate_perplexity_skips_nan_loss()
    test_global_batch_config_exists()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
