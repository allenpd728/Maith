#!/usr/bin/env python3
"""
train.py

Fine-tune Qwen2.5-Coder on one of the A/B/C dataset variants and report
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
from pathlib import Path
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
    from transformers import AutoModelForCausalLM, AutoTokenizer, TrainingArguments, Trainer
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
TRAIN_MAX_SEQ_LEN     = 1024
TRAIN_MAX_SEQ_LEN_C   = 512   # BPE on IR text inflates ~2.6x; reduce to avoid OOM on MPS
EVAL_MAX_SEQ_LEN      = 512   # fixed across A/B/C for apples-to-apples perplexity
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
    PyTorch Dataset. Truncates sequences to the provided max_len.
    """

    def __init__(self, jsonl_path: str, max_len: int = TRAIN_MAX_SEQ_LEN, limit: Optional[int] = None):
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

def load_model_for_variant(variant: str, vocab_path: Optional[str]):
    """
    Load the configured Qwen2.5-Coder base model and adapt it for the chosen variant.

    Variant A: resize embeddings to custom IR vocab size (Option 1 from EXPERIMENT_DESIGN.md).
    Variants B/C: use native BPE tokenizer and embedding table unchanged.
    """
    print(f"Loading base model: {BASE_MODEL}")

    if variant == "A":
        assert vocab_path and os.path.exists(vocab_path), f"vocab_A.json not found at {vocab_path}"
        with open(vocab_path) as f:
            vocab = json.load(f)
        custom_vocab_size = len(vocab)
        print(f"  Variant A: custom IR vocab, {custom_vocab_size} tokens")

        # Load tokenizer as a reference (we use integer IDs directly, not the tokenizer)
        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)

        # Load model and resize embedding + lm_head to IR vocab size
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        model.resize_token_embeddings(custom_vocab_size)
        print(f"  Resized embedding table: {tokenizer.vocab_size} → {custom_vocab_size}")

        bos_id = vocab.get("<BOS>")
        eos_id = vocab.get("<EOS>")
        pad_id = vocab.get("<PAD>")
        assert bos_id is not None, "Custom vocab missing <BOS>"
        assert eos_id is not None, "Custom vocab missing <EOS>"
        assert pad_id is not None, "Custom vocab missing <PAD>"

        model.config.bos_token_id = bos_id
        model.config.eos_token_id = eos_id
        model.config.pad_token_id = pad_id
        model.generation_config.bos_token_id = bos_id
        model.generation_config.eos_token_id = eos_id
        model.generation_config.pad_token_id = pad_id
        print(f"  Remapped bos_token_id={bos_id}, eos_token_id={eos_id}, pad_token_id={pad_id}")

        embedding_rows = int(model.get_input_embeddings().weight.shape[0])
        assert embedding_rows == custom_vocab_size, (
            f"Embedding rows ({embedding_rows}) != custom vocab size ({custom_vocab_size})"
        )
        assert model.config.vocab_size == custom_vocab_size, (
            f"Config vocab_size ({model.config.vocab_size}) != custom vocab size ({custom_vocab_size})"
        )

        return model, tokenizer, {
            "tokenizer_mode": "custom_ir_vocab",
            "custom_vocab_size": custom_vocab_size,
            "tokenizer_vocab_size": int(tokenizer.vocab_size),
            "model_vocab_size": int(model.config.vocab_size),
            "embedding_rows": embedding_rows,
        }

    else:
        # Variants B and C: native BPE, no resizing needed
        print(f"  Variant {variant}: native Qwen2.5-Coder BPE tokenizer")
        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        embedding_rows = int(model.get_input_embeddings().weight.shape[0])
        assert model.config.vocab_size == embedding_rows, (
            f"Config vocab_size ({model.config.vocab_size}) != embedding rows ({embedding_rows})"
        )
        print(
            "  Native vocab check: "
            f"tokenizer.vocab_size={tokenizer.vocab_size}, "
            f"model.config.vocab_size={model.config.vocab_size}, "
            f"embedding_rows={embedding_rows}"
        )
        return model, tokenizer, {
            "tokenizer_mode": "qwen_native_bpe",
            "custom_vocab_size": None,
            "tokenizer_vocab_size": int(tokenizer.vocab_size),
            "model_vocab_size": int(model.config.vocab_size),
            "embedding_rows": embedding_rows,
        }


def _example_id(row: dict) -> str:
    if row.get("example_id"):
        return str(row["example_id"])
    return f"{row.get('module', '')}::{row.get('name', '')}"


def _load_example_ids(path: str) -> list[str]:
    ids = []
    with open(path) as f:
        for line in f:
            row = json.loads(line)
            ids.append(_example_id(row))
    return ids


def _load_representation_id(path: str) -> str | None:
    with open(path) as f:
        first = f.readline().strip()
    if not first:
        return None
    row = json.loads(first)
    rid = row.get("representation_id")
    return str(rid) if rid is not None else None


def assert_shared_eval_examples(datasets_dir: str, eval_path: str) -> None:
    eval_manifest = Path(datasets_dir) / "eval_manifest.json"
    current_ids = _load_example_ids(eval_path)
    if eval_manifest.exists():
        with open(eval_manifest) as f:
            expected_ids = json.load(f)
        assert current_ids == expected_ids, (
            f"Eval split mismatch for {eval_path}; run build_dataset.py to regenerate aligned splits"
        )
        return

    ref_eval_a = Path(datasets_dir) / "eval_A.jsonl"
    if not ref_eval_a.exists():
        return
    expected_ids = _load_example_ids(str(ref_eval_a))
    assert current_ids == expected_ids, (
        f"Eval split mismatch for {eval_path} vs {ref_eval_a}; run build_dataset.py to align A/B/C"
    )


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

    assert_shared_eval_examples(datasets_dir, eval_path)
    train_representation_id = _load_representation_id(train_path)
    eval_representation_id = _load_representation_id(eval_path)
    if train_representation_id and eval_representation_id:
        assert train_representation_id == eval_representation_id, (
            f"Representation mismatch between train ({train_representation_id}) and eval ({eval_representation_id})"
        )

    limit = 50 if smoke_test else None
    train_seq_len = TRAIN_MAX_SEQ_LEN_C if variant == "C" else TRAIN_MAX_SEQ_LEN
    eval_seq_len = EVAL_MAX_SEQ_LEN
    print(f"Loading datasets{' (smoke test: 50 examples)' if smoke_test else ''} ...")
    print(f"  Train sequence cap: {train_seq_len}")
    print(f"  Eval sequence cap:  {eval_seq_len} (shared across A/B/C)")
    train_dataset = IRDataset(train_path, max_len=train_seq_len, limit=limit)
    eval_dataset  = IRDataset(eval_path,  max_len=eval_seq_len, limit=limit)
    print(f"  Train: {len(train_dataset)} examples")
    print(f"  Eval:  {len(eval_dataset)} examples")
    if train_representation_id:
        print(f"  Representation ID: {train_representation_id}")
    print()

    model, tokenizer, model_meta = load_model_for_variant(variant, vocab_path)
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
        logging_strategy="steps",
        logging_steps=1,
        seed=SEED,
        report_to="none",
        gradient_checkpointing=True,   # trade compute for memory — needed for C on MPS
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

    train_loss_curve = [
        {"step": int(h["step"]), "epoch": float(h.get("epoch", 0.0)), "loss": float(h["loss"])}
        for h in trainer.state.log_history
        if "loss" in h and "step" in h
    ]
    eval_loss_curve = [
        {"step": int(h["step"]), "epoch": float(h.get("epoch", 0.0)), "eval_loss": float(h["eval_loss"])}
        for h in trainer.state.log_history
        if "eval_loss" in h and "step" in h
    ]
    curve_path = os.path.join(out_dir, "loss_curve.json")
    with open(curve_path, "w") as f:
        json.dump(
            {
                "variant": variant,
                "train_loss_curve": train_loss_curve,
                "eval_loss_curve": eval_loss_curve,
            },
            f,
            indent=2,
        )
    print(f"Loss curves saved → {curve_path}")
    print(
        f"  Train loss points: {len(train_loss_curve)} | "
        f"Eval loss points: {len(eval_loss_curve)}"
    )
    print()

    # Final perplexity
    print("Computing final perplexity on eval split ...")
    ppl = evaluate_perplexity(model, eval_dataset, device)
    print(f"  Variant {variant} eval perplexity: {ppl:.2f}")
    print()

    # Save results
    results = {
        "variant": variant,
        "representation_id": train_representation_id or "unknown",
        "vocab_size": model_meta["model_vocab_size"],
        "tokenizer_mode": model_meta["tokenizer_mode"],
        "tokenizer_vocab_size": model_meta["tokenizer_vocab_size"],
        "model_vocab_size": model_meta["model_vocab_size"],
        "embedding_rows": model_meta["embedding_rows"],
        "n_params_M": round(n_params, 1),
        "train_examples": len(train_dataset),
        "eval_examples": len(eval_dataset),
        "train_seq_len_cap": train_seq_len,
        "eval_seq_len_cap": eval_seq_len,
        "epochs": epochs,
        "eval_perplexity": round(ppl, 4),
        "training_minutes": round(elapsed / 60, 1),
        "train_loss_points": len(train_loss_curve),
        "eval_loss_points": len(eval_loss_curve),
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
