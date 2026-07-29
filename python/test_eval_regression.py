"""
test_eval_regression.py

Regression tests for eval_completion.py — targeting the two bugs documented
in DEC-010.

  Bug 1 (primary): The logit indexing loop used logit_pos = prefix_len - 1 + k,
    where logits only had prefix_len rows (indices 0..prefix_len-1). At k=1,
    logit_pos == prefix_len, immediately triggering the break guard. Every run
    evaluated exactly ONE token per example regardless of --mask-last.

  Bug 2 (secondary): Targets were read from the labels array (which is
    input_ids shifted by one internally), rather than directly from input_ids.
    This caused an off-by-one in what the eval was actually checking.

The fix replaced both with an autoregressive teacher-forcing loop that:
  - Iterates over targets = input_ids[prefix_len : prefix_len + mask_last]
  - Feeds an incrementally extended prefix at each step
  - Appends the ground-truth token (not the prediction) after each step

These tests verify:
  1. total_predictions == examples * mask_last (the key scaling invariant)
  2. Skipping is correct for sequences shorter than mask_last
  3. Perfect-predictor model scores 100%
  4. Worst-predictor model scores 0%
  5. Teacher forcing: errors do not compound (each step sees ground truth prefix)
  6. mask_last=1 is equivalent to single-token eval (regression baseline)

All tests are CPU-only, model-free (use FakeCausalLM), and complete in < 1 s.

Run with:
    python3 python/test_eval_regression.py

A non-zero exit code means at least one test failed.
"""

import sys
import os
import math

sys.path.insert(0, "python")
import importlib.util

import torch

# ---------------------------------------------------------------------------
# Load evaluate_completion from eval_completion.py
# ---------------------------------------------------------------------------

_spec = importlib.util.spec_from_file_location(
    "eval_completion", os.path.join("python", "eval_completion.py")
)
_eval = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_eval)

evaluate_completion = _eval.evaluate_completion


# ---------------------------------------------------------------------------
# Minimal fake causal-LM for controlled testing
# ---------------------------------------------------------------------------

class FakeLMOutput:
    def __init__(self, logits):
        self.logits = logits


class PerfectPredictor(torch.nn.Module):
    """
    Always predicts the token immediately following the last input token.
    Requires that input_ids contain the answer: it echoes input_ids[t+1]
    by always setting logit for that position to 1.0 and all others to 0.

    Since teacher forcing passes the ground truth at each step, input_ids
    grows by one token per step and the 'next' token is always known.
    """
    def __init__(self, vocab_size: int = 256):
        super().__init__()
        self.vocab_size = vocab_size

    def forward(self, input_ids, **kwargs):
        B, T = input_ids.shape
        # Predict: last token in sequence + 1 (mod vocab) — deterministic, not random
        # This simulates a model that perfectly knows what comes next because
        # the test sequences are constructed so that target == last_id + 1.
        last_id = input_ids[0, -1].item()
        next_id = (last_id + 1) % self.vocab_size
        logits = torch.zeros(B, T, self.vocab_size)
        logits[0, -1, next_id] = 100.0
        return FakeLMOutput(logits)


class WorstPredictor(torch.nn.Module):
    """Always predicts token 0, regardless of context."""
    def __init__(self, vocab_size: int = 256):
        super().__init__()
        self.vocab_size = vocab_size

    def forward(self, input_ids, **kwargs):
        B, T = input_ids.shape
        logits = torch.zeros(B, T, self.vocab_size)
        logits[:, :, 0] = 100.0  # always predict token 0
        return FakeLMOutput(logits)


class RecordingModel(torch.nn.Module):
    """
    Records every input_ids it receives. Used to verify teacher forcing:
    each call should extend the prefix by exactly one ground-truth token.
    Always predicts token 0 (accuracy doesn't matter here, only call shapes).
    """
    def __init__(self, vocab_size: int = 256):
        super().__init__()
        self.vocab_size = vocab_size
        self.calls = []  # list of input_ids tensors received

    def forward(self, input_ids, **kwargs):
        self.calls.append(input_ids.cpu().clone())
        B, T = input_ids.shape
        logits = torch.zeros(B, T, self.vocab_size)
        return FakeLMOutput(logits)


# ---------------------------------------------------------------------------
# Test helpers
# ---------------------------------------------------------------------------

PASS = []
FAIL = []


def ok(name):
    PASS.append(name)
    print(f"  PASS  {name}")


def fail(name, reason):
    FAIL.append(name)
    print(f"  FAIL  {name}: {reason}")


def make_examples(n: int, seq_len: int, start_token: int = 1) -> list[dict]:
    """
    Build n examples each of length seq_len.
    input_ids = [start_token, start_token+1, ..., start_token+seq_len-1]
    labels    = input_ids (same, no pre-shift — matches train.py fixed behaviour)
    """
    examples = []
    for i in range(n):
        base = (start_token + i * seq_len) % 200
        ids = list(range(base, base + seq_len))
        examples.append({
            "input_ids": ids,
            "labels":    ids[:],
            "name":      f"decl_{i}",
        })
    return examples


# ---------------------------------------------------------------------------
# TEST 1: total_predictions scales with mask_last (the DEC-010 invariant)
#
# With the buggy eval: total_predictions == n_examples always.
# With the fixed eval: total_predictions == n_examples * mask_last.
#
# This is the single most direct test of the DEC-010 primary bug.
# ---------------------------------------------------------------------------

def test_total_predictions_scale_with_mask_last():
    name = "total_predictions == n_examples * mask_last (DEC-010 primary invariant)"
    try:
        n = 10
        seq_len = 20
        for mask_last in [1, 3, 5, 10]:
            examples = make_examples(n, seq_len)
            model = WorstPredictor(vocab_size=256)
            result = evaluate_completion(model, examples, device="cpu", mask_last=mask_last)
            expected = n * mask_last
            actual = result["total_predictions"]
            assert actual == expected, (
                f"mask_last={mask_last}: expected total_predictions={expected}, "
                f"got {actual}. "
                f"If this equals {n} (n_examples), the DEC-010 bug has regressed."
            )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 2: Short sequences are skipped correctly
#
# Examples with len(input_ids) <= mask_last must be skipped entirely.
# They should contribute 0 to total_predictions and 0 to examples_evaluated.
# ---------------------------------------------------------------------------

def test_short_sequences_skipped():
    name = "sequences shorter than mask_last are skipped"
    try:
        mask_last = 5
        examples = [
            {"input_ids": [1, 2, 3, 4, 5], "labels": [1, 2, 3, 4, 5], "name": "exact"},   # len == mask_last: skipped
            {"input_ids": [1, 2, 3],        "labels": [1, 2, 3],        "name": "short"},   # len < mask_last: skipped
            {"input_ids": [1, 2, 3, 4, 5, 6], "labels": [1, 2, 3, 4, 5, 6], "name": "ok"}, # len > mask_last: evaluated
        ]
        model = WorstPredictor(vocab_size=256)
        result = evaluate_completion(model, examples, device="cpu", mask_last=mask_last)
        assert result["examples_evaluated"] == 1, (
            f"Expected 1 example evaluated (only 'ok'), got {result['examples_evaluated']}"
        )
        assert result["total_predictions"] == mask_last, (
            f"Expected total_predictions={mask_last} for the one valid example, "
            f"got {result['total_predictions']}"
        )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 3: Perfect predictor scores 100%
#
# Uses PerfectPredictor with sequences where each token is previous+1.
# Teacher forcing ensures each step receives the correct prefix, so
# a perfect predictor should get every token right.
# ---------------------------------------------------------------------------

def test_perfect_predictor_scores_100():
    name = "perfect predictor scores 100% accuracy"
    try:
        # Build sequences where target[k] == target[k-1] + 1 (mod 256)
        # PerfectPredictor predicts last_input_id + 1, which matches.
        n = 5
        seq_len = 15
        mask_last = 5
        examples = make_examples(n, seq_len, start_token=10)

        model = PerfectPredictor(vocab_size=256)
        result = evaluate_completion(model, examples, device="cpu", mask_last=mask_last)

        assert result["top1_accuracy"] == 1.0, (
            f"Expected 100% accuracy, got {result['top1_accuracy']:.1%}. "
            f"Correct: {result['correct_predictions']}/{result['total_predictions']}"
        )
        assert result["total_predictions"] == n * mask_last, (
            f"Expected {n * mask_last} total predictions, got {result['total_predictions']}"
        )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 4: Worst predictor scores 0%
#
# WorstPredictor always predicts token 0. Sequences use tokens starting at 10,
# so target tokens are never 0 and accuracy should be exactly 0%.
# ---------------------------------------------------------------------------

def test_worst_predictor_scores_0():
    name = "worst predictor scores 0% accuracy (targets never == predicted token)"
    try:
        n = 5
        seq_len = 15
        mask_last = 5
        examples = make_examples(n, seq_len, start_token=10)  # tokens 10..24, never 0

        model = WorstPredictor(vocab_size=256)
        result = evaluate_completion(model, examples, device="cpu", mask_last=mask_last)

        assert result["top1_accuracy"] == 0.0, (
            f"Expected 0% accuracy, got {result['top1_accuracy']:.1%}. "
            f"Correct: {result['correct_predictions']}/{result['total_predictions']}"
        )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 5: Teacher forcing verified — each step receives ground truth prefix
#
# This directly tests the "errors do not compound" property of teacher forcing.
# RecordingModel logs every input_ids it receives. After mask_last steps for
# one example of seq_len=10 with mask_last=4:
#   - Call 0: input_ids has length prefix_len = 6
#   - Call 1: input_ids has length 7 (prefix + ground truth token 0)
#   - Call 2: input_ids has length 8
#   - Call 3: input_ids has length 9
#
# The last token of each call's input_ids must be the ground truth token
# at that position (not whatever was predicted), confirming teacher forcing.
# ---------------------------------------------------------------------------

def test_teacher_forcing_uses_ground_truth():
    name = "teacher forcing: each step extends prefix with ground truth (not prediction)"
    try:
        seq_len   = 10
        mask_last = 4
        prefix_len = seq_len - mask_last  # = 6
        # Sequence: [10, 11, 12, 13, 14, 15, 16, 17, 18, 19]
        ids = list(range(10, 10 + seq_len))
        examples = [{"input_ids": ids, "labels": ids[:], "name": "test"}]

        model = RecordingModel(vocab_size=256)
        evaluate_completion(model, examples, device="cpu", mask_last=mask_last)

        assert len(model.calls) == mask_last, (
            f"Expected {mask_last} model calls, got {len(model.calls)}"
        )
        # Verify each call's prefix length and last token
        for step, call_input in enumerate(model.calls):
            expected_len = prefix_len + step
            actual_len   = call_input.shape[1]
            assert actual_len == expected_len, (
                f"Step {step}: expected prefix length {expected_len}, got {actual_len}"
            )
            # Last token should be ground truth token at position prefix_len + step - 1
            # (for step 0, the prefix ends at prefix_len-1, so last token = ids[prefix_len-1])
            if step > 0:
                expected_last = ids[prefix_len + step - 1]
                actual_last   = call_input[0, -1].item()
                assert actual_last == expected_last, (
                    f"Step {step}: last token of prefix should be ground truth "
                    f"ids[{prefix_len + step - 1}]={expected_last}, got {actual_last}. "
                    f"If this is the predicted token, teacher forcing is broken."
                )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 6: mask_last=1 is equivalent to single-token eval
#
# With mask_last=1, the fixed eval and the old buggy eval produce the same
# result (both evaluate exactly 1 token per example). This validates that
# the fix is backward-compatible at mask_last=1.
# ---------------------------------------------------------------------------

def test_mask_last_1_is_single_token_eval():
    name = "mask_last=1 evaluates exactly 1 token per example (backward compat)"
    try:
        n = 8
        seq_len = 12
        mask_last = 1
        examples = make_examples(n, seq_len)

        model = WorstPredictor(vocab_size=256)
        result = evaluate_completion(model, examples, device="cpu", mask_last=mask_last)

        assert result["total_predictions"] == n, (
            f"mask_last=1 should give total_predictions={n}, got {result['total_predictions']}"
        )
        assert result["examples_evaluated"] == n, (
            f"Expected {n} examples evaluated, got {result['examples_evaluated']}"
        )
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("Running eval_completion.py regression tests (DEC-010)...\n")

    test_total_predictions_scale_with_mask_last()
    test_short_sequences_skipped()
    test_perfect_predictor_scores_100()
    test_worst_predictor_scores_0()
    test_teacher_forcing_uses_ground_truth()
    test_mask_last_1_is_single_token_eval()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
