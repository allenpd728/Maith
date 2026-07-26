#!/usr/bin/env python3
"""
train.py

Fine-tune Qwen2.5-Coder-1.5B on one of the A/B/C dataset variants and report
perplexity on the eval split. Run three times (once per variant) with identical
hyperparameters to produce a fair comparison.

Implements experiment design from docs/EXPERIMENT_DESIGN.md:
  - Option 1: hold architecture fixed, replace embedding/unembedding for variant A
  - Fixed seed, splits, and hparams across all three runs
  - Primary metric: perplexity on held-out eval split

Requirements:
    pip install transformers torch trl datasets

Usage:
    # Variant A — IR tokens with custom vocab
    python3 python/train.py --variant A --datasets datasets/ --out runs/variant_A

    # Variant B — Lean source BPE
    python3 python/train.py --variant B --datasets datasets/ --out runs/variant_B

    # Variant C — AST-style BPE
    python3 python/train.py --variant C --datasets datasets/ --out runs/variant_C

    # Quick smoke test (1 epoch, 50 examples)
    python3 python/train.py --variant A --datasets datasets/ --out runs/smoke --smoke-test
"""

import argparse
import json
import math
import os
import random
import sys
import time
from typing import Optional

# ---------------------------------------------------------------------------
# Dependency check
# ---------------------------------------------------------------------------

missing = []
try:
    import torch
except ImportError:
    missing.append("torch")
try:
    from transformers import (
        AutoConfig, AutoModelForCausalLM, AutoTokenizer,
        PreTrainedTokenizerFast, TrainingArguments, Trainer,
        DataCollatorForLanguageModeling,
    )
    from transformers import set_seed as hf_set_seed
except ImportError:
    missing.append("transformers")
try:
    from torch.utils.data import Dataset
except ImportError:
    pass  # covered by torch above

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    print("Run: pip install transformers torch trl datasets")
    sys.exit(1)


# ---------------------------------------------------------------------------
# Constants (from EXPERIMENT_DESIGN.md)
# ---------------------------------------------------------------------------

BASE_MODEL      = "Qwen/Qwen2.5-Coder-0.5B"  # 494M params — fits MPS 20 GB; swap to 1.5B for CUDA
MAX_SEQ_LEN     = 1024
BATCH_SIZE      = 2
GRAD_ACCUM      = 4
LEARNING_RATE   = 2e-4
EPOCHS          = 3
WEIGHT_DECAY    = 0.01
WARMUP_RATIO    = 0.05  # converted to warmup_steps at runtime
SEED            = 42


# ---------------------------------------------------------------------------
# Dataset
# ---------------------------------------------------------------------------

class IRDataset(Dataset):
    """
    Loads a pre-built JSONL split (from build_dataset.py) and wraps it as a
    PyTorch Dataset. Truncates sequences to MAX_SEQ_LEN.
    """

    def __init__(self, jsonl_path: str, max_len: int = MAX_SEQ_LEN, limit: Optional[int] = None):
        self.examples = []
        with open(jsonl_path) as f:
            for i, line in enumerate(f):
                if limit and i >= limit:
                    break
                row = json.loads(line)
                ids    = row["input_ids"][:max_len]
                labels = row["labels"][:max_len]
                self.examples.append({
                    "input_ids":      ids,
                    "attention_mask": [1] * len(ids),
                    "labels":         labels,
                })

    def __len__(self):
        return len(self.examples)

    def __getitem__(self, idx):
        ex = self.examples[idx]
        return {k: torch.tensor(v, dtype=torch.long) for k, v in ex.items()}


# ---------------------------------------------------------------------------
# Collator — pads a batch to the longest sequence in the batch
# ---------------------------------------------------------------------------

def collate_fn(batch: list[dict], pad_id: int = 0) -> dict:
    max_len = max(ex["input_ids"].shape[0] for ex in batch)
    input_ids      = torch.zeros(len(batch), max_len, dtype=torch.long)
    attention_mask = torch.zeros(len(batch), max_len, dtype=torch.long)
    labels         = torch.full((len(batch), max_len), -100, dtype=torch.long)

    for i, ex in enumerate(batch):
        n = ex["input_ids"].shape[0]
        input_ids[i, :n]      = ex["input_ids"]
        attention_mask[i, :n] = ex["attention_mask"]
        labels[i, :n]         = ex["labels"]

    return {"input_ids": input_ids, "attention_mask": attention_mask, "labels": labels}


# ---------------------------------------------------------------------------
# Perplexity evaluation
# ---------------------------------------------------------------------------

def evaluate_perplexity(model, dataset: IRDataset, device: str, batch_size: int = 4) -> float:
    """Compute perplexity on a dataset. Lower is better."""
    model.eval()
    total_loss = 0.0
    total_tokens = 0

    with torch.no_grad():
        for start in range(0, len(dataset), batch_size):
            batch = [dataset[i] for i in range(start, min(start + batch_size, len(dataset)))]
            batch_tensors = collate_fn(batch)
            batch_tensors = {k: v.to(device) for k, v in batch_tensors.items()}

            outputs = model(**batch_tensors)
            loss = outputs.loss

            # Count non-padding label tokens
            n_tokens = (batch_tensors["labels"] != -100).sum().item()
            total_loss   += loss.item() * n_tokens
            total_tokens += n_tokens

    avg_loss = total_loss / max(total_tokens, 1)
    return math.exp(avg_loss)


# ---------------------------------------------------------------------------
# Model setup
# ---------------------------------------------------------------------------

def load_model_for_variant(variant: str, vocab_path: Optional[str], device: str):
    """
    Load Qwen2.5-Coder-1.5B and adapt it for the chosen variant.

    Variant A: resize embeddings to custom IR vocab size (Option 1 from EXPERIMENT_DESIGN.md).
    Variants B/C: use native BPE tokenizer and embedding table unchanged.
    """
    print(f"Loading base model: {BASE_MODEL}")

    if variant == "A":
        assert vocab_path and os.path.exists(vocab_path), f"vocab_A.json not found at {vocab_path}"
        with open(vocab_path) as f:
            vocab = json.load(f)
        vocab_size = len(vocab)
        print(f"  Variant A: custom IR vocab, {vocab_size} tokens")

        # Load tokenizer as a reference (we use integer IDs directly, not the tokenizer)
        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)

        # Load model and resize embedding + lm_head to IR vocab size
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        model.resize_token_embeddings(vocab_size)
        print(f"  Resized embedding table: {tokenizer.vocab_size} → {vocab_size}")
        return model, tokenizer, vocab_size

    else:
        # Variants B and C: native BPE, no resizing needed
        print(f"  Variant {variant}: native Qwen2.5-Coder BPE tokenizer")
        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        return model, tokenizer, tokenizer.vocab_size


# ---------------------------------------------------------------------------
# Training loop
# ---------------------------------------------------------------------------

def run(variant: str, datasets_dir: str, out_dir: str, smoke_test: bool) -> None:
    hf_set_seed(SEED)
    random.seed(SEED)

    device = "mps" if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Variant: {variant}")
    print()

    train_path = os.path.join(datasets_dir, f"train_{variant}.jsonl")
    eval_path  = os.path.join(datasets_dir, f"eval_{variant}.jsonl")
    vocab_path = os.path.join(datasets_dir, "vocab_A.json") if variant == "A" else None

    assert os.path.exists(train_path), f"Missing: {train_path} — run build_dataset.py first"
    assert os.path.exists(eval_path),  f"Missing: {eval_path}"

    limit = 50 if smoke_test else None
    print(f"Loading datasets{' (smoke test: 50 examples)' if smoke_test else ''} ...")
    train_dataset = IRDataset(train_path, limit=limit)
    eval_dataset  = IRDataset(eval_path,  limit=limit)
    print(f"  Train: {len(train_dataset)} examples")
    print(f"  Eval:  {len(eval_dataset)} examples")
    print()

    model, tokenizer, vocab_size = load_model_for_variant(variant, vocab_path, device)
    model = model.to(device)
    n_params = sum(p.numel() for p in model.parameters()) / 1e6
    print(f"  Model parameters: {n_params:.1f}M")
    print()

    os.makedirs(out_dir, exist_ok=True)
    epochs = 1 if smoke_test else EPOCHS
    total_steps = max(1, (len(train_dataset) // (BATCH_SIZE * GRAD_ACCUM)) * epochs)
    warmup_steps = max(1, int(WARMUP_RATIO * total_steps))

    training_args = TrainingArguments(
        output_dir=out_dir,
        num_train_epochs=epochs,
        per_device_train_batch_size=BATCH_SIZE,
        per_device_eval_batch_size=BATCH_SIZE,
        gradient_accumulation_steps=GRAD_ACCUM,
        learning_rate=LEARNING_RATE,
        weight_decay=WEIGHT_DECAY,
        warmup_steps=warmup_steps,
        lr_scheduler_type="cosine",
        eval_strategy="epoch",
        save_strategy="epoch",
        logging_steps=10,
        seed=SEED,
        report_to="none",
        fp16=False,  # MPS doesn't support fp16; set True for CUDA
        bf16=False,
        dataloader_pin_memory=False,
    )

    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        data_collator=lambda batch: collate_fn(batch),
    )

    print(f"Training variant {variant} for {epochs} epoch(s) ...")
    t0 = time.time()
    trainer.train()
    elapsed = time.time() - t0
    print(f"Training complete in {elapsed/60:.1f} minutes.")
    print()

    # Final perplexity
    print("Computing final perplexity on eval split ...")
    ppl = evaluate_perplexity(model, eval_dataset, device)
    print(f"  Variant {variant} eval perplexity: {ppl:.2f}")
    print()

    # Save results
    results = {
        "variant": variant,
        "vocab_size": vocab_size,
        "n_params_M": round(n_params, 1),
        "train_examples": len(train_dataset),
        "eval_examples": len(eval_dataset),
        "epochs": epochs,
        "eval_perplexity": round(ppl, 4),
        "training_minutes": round(elapsed / 60, 1),
        "seed": SEED,
        "base_model": BASE_MODEL,
        "smoke_test": smoke_test,
    }
    results_path = os.path.join(out_dir, "results.json")
    with open(results_path, "w") as f:
        json.dump(results, f, indent=2)
    print(f"Results saved → {results_path}")

    # Print comparison table hint
    print()
    print("=== Result ===")
    print(f"  Variant {variant}: perplexity = {ppl:.2f}")
    print()
    print("Compare all three variants:")
    for v in ["A", "B", "C"]:
        rpath = os.path.join(os.path.dirname(out_dir), f"variant_{v}", "results.json")
        if os.path.exists(rpath):
            r = json.load(open(rpath))
            flag = " ← this run" if v == variant else ""
            print(f"  Variant {v}: {r['eval_perplexity']:.2f}{flag}")
        elif v == variant:
            print(f"  Variant {v}: {ppl:.2f} ← this run")
        else:
            print(f"  Variant {v}: (not run yet)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--variant",    required=True, choices=["A", "B", "C"])
    parser.add_argument("--datasets",   default="datasets/")
    parser.add_argument("--out",        default=None)
    parser.add_argument("--smoke-test", action="store_true",
                        help="Quick 1-epoch run on 50 examples to verify the pipeline")
    args = parser.parse_args()

    out = args.out or f"runs/variant_{args.variant}"
    run(args.variant, args.datasets, out, args.smoke_test)
