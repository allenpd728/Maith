"""
preflight_check.py

Pre-flight checks to run before any A/B/C training run.

Catches the three failure modes that have already cost multi-hour runs:
  1. Vocab size mismatch — tokenizer.vocab_size vs model.config.vocab_size vs
     embedding rows (the resize step for variant A can silently fail to update
     model.config, causing silent OOB lookups or wrong-shape lm_head outputs).
  2. NaN/Inf on a forward pass — feeds a small real batch through each variant's
     model and checks that loss is finite before committing to a full run.
  3. Effective batch size mismatch — confirms A/B/C produce equal gradient steps
     per epoch (the DEC-007 confound that invalidated the 2026-07-27 run).

All three checks are fast: they load the model once per variant (~10-20 s each
on MPS/CPU) and run a single forward pass on a 4-example synthetic batch.
No training, no optimizer, no grad computation.

Usage:
    python3 python/preflight_check.py [--datasets datasets/] [--variants A B C]

Exit codes:
    0 — all checks passed, safe to start training
    1 — at least one check failed; do NOT start training
"""

import argparse
import json
import math
import os
import sys

import torch

sys.path.insert(0, "python")
import importlib.util

_spec = importlib.util.spec_from_file_location("train", os.path.join("python", "train.py"))
_train = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_train)

load_model_for_variant = _train.load_model_for_variant
collate_fn             = _train.collate_fn
VARIANT_BATCH_CONFIG   = _train.VARIANT_BATCH_CONFIG
IRDataset              = _train.IRDataset
EVAL_MAX_SEQ_LEN       = _train.EVAL_MAX_SEQ_LEN


PASS = []
FAIL = []


def ok(name, detail=""):
    PASS.append(name)
    suffix = f"  ({detail})" if detail else ""
    print(f"  PASS  {name}{suffix}")


def fail(name, reason):
    FAIL.append(name)
    print(f"  FAIL  {name}")
    print(f"        {reason}")


# ---------------------------------------------------------------------------
# Check 1: Effective batch sizes match across all requested variants
#
# Does not load any model — purely a config read. Fast.
# ---------------------------------------------------------------------------

def check_batch_config(variants):
    name = "Effective batch size matches across variants"
    cfg = VARIANT_BATCH_CONFIG
    missing = [v for v in variants if v not in cfg]
    if missing:
        fail(name, f"Variants not in VARIANT_BATCH_CONFIG: {missing}")
        return

    eff = {v: cfg[v]["batch_size"] * cfg[v]["grad_accum"] for v in variants}
    values = list(eff.values())
    if len(set(values)) != 1:
        fail(name, f"Mismatch: {eff}  — fix VARIANT_BATCH_CONFIG before running")
        return

    detail = "  |  ".join(
        f"{v}: batch={cfg[v]['batch_size']} × accum={cfg[v]['grad_accum']} = {eff[v]}"
        for v in variants
    )
    ok(name, detail)


# ---------------------------------------------------------------------------
# Check 2: Vocab size consistency for each variant
#
# Loads the model for each variant and verifies:
#   - model.config.vocab_size == embedding_rows (resize updated config)
#   - For variant A: embedding_rows == len(vocab_A.json)
#   - For B/C: tokenizer.vocab_size <= model.config.vocab_size
#     (Qwen's tokenizer reports 151,643; the model's embedding table has
#      151,936 rows due to padding for hardware alignment — that gap is
#      expected and not a bug)
# ---------------------------------------------------------------------------

def check_vocab(variant, datasets_dir, device):
    name = f"Variant {variant}: vocab size consistency"
    vocab_path = os.path.join(datasets_dir, "vocab_A.json") if variant == "A" else None

    try:
        model, tokenizer, meta = load_model_for_variant(variant, vocab_path)
    except Exception as e:
        fail(name, f"load_model_for_variant raised: {e}")
        return None, None, None

    embedding_rows = int(model.get_input_embeddings().weight.shape[0])
    config_vocab   = int(model.config.vocab_size)
    tok_vocab      = int(tokenizer.vocab_size)

    issues = []

    if config_vocab != embedding_rows:
        issues.append(
            f"model.config.vocab_size={config_vocab} != embedding_rows={embedding_rows}"
        )

    if variant == "A":
        if vocab_path and os.path.exists(vocab_path):
            with open(vocab_path) as f:
                custom_size = len(json.load(f))
            if embedding_rows != custom_size:
                issues.append(
                    f"embedding_rows={embedding_rows} != vocab_A.json size={custom_size}"
                )
    else:
        # For B/C the tokenizer vocab (151,643) is smaller than the padded embedding
        # table (151,936). This is expected. Flag only if tokenizer vocab exceeds table.
        if tok_vocab > embedding_rows:
            issues.append(
                f"tokenizer.vocab_size={tok_vocab} > embedding_rows={embedding_rows} "
                f"(tokenizer IDs would be out of range)"
            )

    if issues:
        fail(name, "; ".join(issues))
        return None, None, None

    detail = (f"tokenizer={tok_vocab}, config={config_vocab}, "
              f"embedding_rows={embedding_rows}")
    ok(name, detail)
    return model.to(device), tokenizer, meta


# ---------------------------------------------------------------------------
# Check 3: Forward pass produces finite loss on a small synthetic batch
#
# Builds a 4-example batch of short sequences (length 8) drawn from the
# valid token range for this variant, runs a forward pass, and checks that:
#   - loss is not NaN
#   - loss is not Inf
#   - loss is positive (a loss of exactly 0.0 would also be suspicious)
#
# Uses torch.no_grad() — no optimizer, no gradient computation.
# ---------------------------------------------------------------------------

def check_forward_pass(variant, model, datasets_dir, device):
    name = f"Variant {variant}: forward pass produces finite loss"
    if model is None:
        fail(name, "Skipped — model failed to load in vocab check")
        return

    try:
        # Use real eval data if available — more representative than random tokens.
        eval_path = os.path.join(datasets_dir, f"eval_{variant}.jsonl")
        if os.path.exists(eval_path):
            dataset = IRDataset(eval_path, max_len=EVAL_MAX_SEQ_LEN, limit=4)
            if len(dataset) == 0:
                fail(name, f"eval_{variant}.jsonl loaded 0 examples")
                return
            batch = collate_fn([dataset[i] for i in range(min(4, len(dataset)))])
        else:
            # Fall back to synthetic batch if datasets not built yet.
            # Use IDs safely within the model's vocab range.
            vocab_size = int(model.get_input_embeddings().weight.shape[0])
            safe_id = min(10, vocab_size - 1)
            seq = [safe_id] * 8
            items = []
            for _ in range(4):
                t = torch.tensor(seq, dtype=torch.long)
                items.append({
                    "input_ids": t,
                    "attention_mask": torch.ones_like(t),
                    "labels": t.clone(),
                })
            batch = collate_fn(items)

        batch = {k: v.to(device) for k, v in batch.items()}

        model.eval()
        with torch.no_grad():
            outputs = model(**batch)

        loss = outputs.loss
        loss_val = loss.item()

        if not math.isfinite(loss_val):
            fail(name, f"loss={loss_val} (not finite) — do NOT start training")
            return
        if loss_val <= 0:
            fail(name, f"loss={loss_val:.6f} (zero or negative — suspicious)")
            return

        ok(name, f"loss={loss_val:.4f}")

    except Exception as e:
        fail(name, f"Exception during forward pass: {e}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Pre-flight checks before A/B/C training")
    parser.add_argument("--datasets", default="datasets/", help="Path to datasets directory")
    parser.add_argument("--variants", nargs="+", default=["A", "B", "C"],
                        help="Variants to check (default: A B C)")
    args = parser.parse_args()

    device = "mps" if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"

    print(f"Pre-flight checks — device: {device}")
    print(f"Variants: {args.variants}")
    print(f"Datasets: {args.datasets}")
    print()

    # --- Check 1: batch config (no model load needed) ---
    print("[ Check 1: Effective batch sizes ]")
    check_batch_config(args.variants)
    print()

    # --- Checks 2 + 3 per variant ---
    for variant in args.variants:
        print(f"[ Check 2+3: Variant {variant} ]")
        model, tokenizer, meta = check_vocab(variant, args.datasets, device)
        check_forward_pass(variant, model, args.datasets, device)
        # Free model memory before loading next variant
        if model is not None:
            del model
            if device == "mps":
                torch.mps.empty_cache()
            elif device == "cuda":
                torch.cuda.empty_cache()
        print()

    # --- Summary ---
    total = len(PASS) + len(FAIL)
    print(f"Results: {len(PASS)}/{total} passed.")
    if FAIL:
        print("\nFailed checks — do NOT start training until these are resolved:")
        for name in FAIL:
            print(f"  ✗ {name}")
        sys.exit(1)
    else:
        print("\nAll checks passed. Safe to start training.")
        sys.exit(0)


if __name__ == "__main__":
    main()
