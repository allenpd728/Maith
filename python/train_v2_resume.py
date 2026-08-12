#!/usr/bin/env python3
"""
train.py

Fine-tune Qwen2.5-Coder-0.5B on one of the A/B/C dataset variants and report
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

Output directory layout (final-only, no intermediate checkpoints):
    <out_dir>/
        checkpoint-final/       # model weights + config only (~1.4 GB, no optimizer state)
        loss_curve.json
        results.json
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
        DataCollatorForLanguageModeling, TrainerCallback,
    )
    from transformers import set_seed as hf_set_seed
except ImportError:
    missing.append("transformers")
try:
    from torch.utils.data import Dataset
except ImportError:
    pass  # covered by torch above

# TensorBoard SummaryWriter (ships with torch). Guarded so a missing or broken
# tensorboard install degrades to no-op logging instead of crashing training.
_tb_available = True
try:
    from torch.utils.tensorboard import SummaryWriter
except ImportError:
    _tb_available = False

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    print("Run: pip install transformers torch trl datasets")
    sys.exit(1)


# ---------------------------------------------------------------------------
# Constants (from EXPERIMENT_DESIGN.md)
# ---------------------------------------------------------------------------

BASE_MODEL      = "Qwen/Qwen2.5-Coder-0.5B"  # 494M params — fits MPS 20 GB; swap to 1.5B for CUDA
MAX_SEQ_LEN     = 1024
BATCH_SIZE      = 1
GRAD_ACCUM      = 8   # effective batch = BATCH_SIZE * GRAD_ACCUM = 8 (unchanged)
LEARNING_RATE   = 2e-4
EPOCHS          = 2
WEIGHT_DECAY    = 0.01
WARMUP_RATIO    = 0.05  # converted to warmup_steps at runtime
SEED            = 42

# B/C use the full 151k embedding table and are unstable at 2e-4; use a reduced LR.
# This is a legitimate consequence of vocab size, not a confound (documented in
# docs/experiments/EXPERIMENT_DESIGN.md hyperparameters table).
def lr_for_variant(variant: str) -> float:
    return 5e-5 if variant in ("B", "C") else LEARNING_RATE


# ---------------------------------------------------------------------------
# Dataset sample logger
# ---------------------------------------------------------------------------

def log_dataset_samples(
    train_path: str,
    variant: str,
    tokenizer,
    vocab_path: Optional[str] = None,
    n_samples: int = 3,
    out_dir: Optional[str] = None,
) -> None:
    """
    Print and optionally save n_samples decoded examples from the raw JSONL
    (before IRDataset truncation) so training inputs can be visually validated.

    For variant A, decodes token IDs using vocab_A.json (id → token string).
    For B/C, decodes using the Qwen BPE tokenizer.
    """
    # Build id→token map for variant A / B_small (custom-vocab variants)
    id_to_token = {}
    if variant in ("A", "B_small") and vocab_path and os.path.exists(vocab_path):
        with open(vocab_path) as f:
            vocab = json.load(f)
        # vocab is token→id; invert it
        id_to_token = {v: k for k, v in vocab.items()}

    samples = []
    with open(train_path) as f:
        for i, line in enumerate(f):
            if i >= n_samples:
                break
            row = json.loads(line)
            ids = row["input_ids"]
            if variant in ("A", "B_small"):
                decoded = " ".join(id_to_token.get(tid, f"<UNK:{tid}>") for tid in ids)
            else:
                decoded = tokenizer.decode(ids, skip_special_tokens=False)
            samples.append({
                "index": i,
                "name": row.get("name", "?"),
                "module": row.get("module", "?"),
                "seq_len": row.get("seq_len", len(ids)),
                "decoded": decoded,
            })

    sep = "=" * 60
    print(f"\n{sep}")
    print(f"DATASET SAMPLE VALIDATION — Variant {variant}")
    print(f"Source: {train_path}")
    print(sep)
    for s in samples:
        print(f"\n[{s['index']}] {s['name']} ({s['module']}) — {s['seq_len']} tokens")
        print(f"  {s['decoded'][:300]}{'...' if len(s['decoded']) > 300 else ''}")
    print(f"{sep}\n")

    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
        out_path = os.path.join(out_dir, f"sample_validation_{variant}.json")
        with open(out_path, "w") as f:
            json.dump({"variant": variant, "samples": samples}, f, indent=2)
        print(f"  Sample log saved → {out_path}")
    else:
        print("  (out_dir not set — sample log not saved to disk)")


# ---------------------------------------------------------------------------
# Dataset
# ---------------------------------------------------------------------------

class IRDataset(Dataset):
    """
    Loads a pre-built JSONL split (from build_dataset.py) and wraps it as a
    PyTorch Dataset. Truncates sequences to MAX_SEQ_LEN.
    """

    def __init__(self, jsonl_path: str, max_len: int = MAX_SEQ_LEN, limit: Optional[int] = None, expected_source: Optional[str] = None):
        self.examples = []
        with open(jsonl_path) as f:
            for i, line in enumerate(f):
                if limit and i >= limit:
                    break
                row = json.loads(line)
                # Guard: confirm each row's source field matches the expected variant.
                # A mismatch means the wrong dataset file was loaded — catch it early
                # before any training steps run on mismatched token spaces.
                if expected_source is not None:
                    row_source = row.get("source")
                    assert row_source == expected_source, (
                        f"Source mismatch in {jsonl_path} at line {i}: "
                        f"expected source='{expected_source}' but got '{row_source}'. "
                        f"Declaration: {row.get('name', '?')}. "
                        f"Check that build_dataset.py wrote the correct files."
                    )
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

def evaluate_perplexity(model, dataset: IRDataset, device: str, batch_size: int = 1) -> float:
    """Compute perplexity on a dataset. Lower is better."""
    model.eval()
    total_loss = 0.0
    total_tokens = 0

    with torch.no_grad():
        for start in range(0, len(dataset), batch_size):
            batch = [dataset[i] for i in range(start, min(start + batch_size, len(dataset)))]
            batch_tensors = collate_fn(batch)
            batch_tensors = {k: v.to(device) for k, v in batch_tensors.items()}

            # Count non-padding label tokens; skip degenerate examples (<2 tokens)
            n_tokens = (batch_tensors["labels"] != -100).sum().item()
            if n_tokens < 2:
                continue

            outputs = model(**batch_tensors)
            loss = outputs.loss
            loss_val = loss.item()
            if math.isnan(loss_val) or math.isinf(loss_val):
                continue

            total_loss   += loss_val * n_tokens
            total_tokens += n_tokens

    avg_loss = total_loss / max(total_tokens, 1)
    return math.exp(avg_loss)


# ---------------------------------------------------------------------------
# Model setup
# ---------------------------------------------------------------------------

def load_model_for_variant(variant: str, vocab_path: Optional[str], device: str,
                                  embed_project_path: str = ""):
    """
    Load Qwen2.5-Coder-0.5B and adapt it for the chosen variant.

    Variant A: resize embeddings to custom IR vocab size (Option 1 from EXPERIMENT_DESIGN.md).
    Variants B/C: use native BPE tokenizer and embedding table unchanged.

    If embed_project_path is provided and exists, load the embedding projection matrix
    and apply it to initialize Variant A's embedding table.
    """
    print(f"Loading base model: {BASE_MODEL}")

    if variant in ("A", "flat", "B_small"):
        assert vocab_path and os.path.exists(vocab_path), f"vocab not found at {vocab_path}"
        with open(vocab_path) as f:
            vocab = json.load(f)
        vocab_size = len(vocab)
        print(f"  Variant {variant}: custom vocab, {vocab_size} tokens")

        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        model.resize_token_embeddings(vocab_size)
        print(f"  Resized embedding table: {tokenizer.vocab_size} → {vocab_size}")

        # Variant A uses a custom vocab (e.g. 601/633 tokens), so the base Qwen
        # bos/eos token ids (151643) are out of range and crash eval on load.
        # Null them out so the saved config is eval-safe.
        model.config.bos_token_id = None
        model.config.eos_token_id = None

        # DEC-006: Load embedding projection matrix if provided
        if embed_project_path and os.path.exists(embed_project_path):
            print(f"  Loading embedding projection from {embed_project_path} ...")
            E_proj = torch.load(embed_project_path, map_location="cpu")
            assert E_proj.shape == (vocab_size, model.config.hidden_size), \
                f"Projection shape mismatch: {E_proj.shape} vs ({vocab_size}, {model.config.hidden_size})"
            with torch.no_grad():
                model.model.embed_tokens.weight.copy_(E_proj)
                # Initialize lm_head to the transpose of the embedding projection
                # (weight tying convention — same as Qwen's original design)
                if model.lm_head.weight.shape == E_proj.shape:
                    model.lm_head.weight.copy_(E_proj)
            print(f"  Embedding projection applied. Norm: {E_proj.norm():.2f}")

        return model, tokenizer, vocab_size

    else:
        print(f"  Variant {variant}: native Qwen2.5-Coder BPE tokenizer")
        tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
            dtype=torch.float32,
        )
        return model, tokenizer, tokenizer.vocab_size


# ---------------------------------------------------------------------------
# Thermal guard callback
# ---------------------------------------------------------------------------

class ThermalGuardCallback(TrainerCallback):
    """
    Monitors step time during training. If a step takes longer than
    THROTTLE_THRESHOLD_SEC, the chip is likely thermal-throttling.
    Inserts a COOLDOWN_SEC pause to let it recover before continuing.
    Logs all step times so slowdowns are visible in the training output.
    """
    THROTTLE_THRESHOLD_SEC = 90   # flag if a step exceeds this (normal variance is 17-60s; 90s signals real pressure)
    COOLDOWN_SEC           = 10   # pause this long when throttling detected
    LOG_EVERY_N_STEPS      = 10   # print step time at this interval

    def __init__(self):
        self._step_start = None
        self._throttle_count = 0

    def on_step_begin(self, args, state, control, **kwargs):
        self._step_start = time.time()

    def on_step_end(self, args, state, control, **kwargs):
        if self._step_start is None:
            return
        elapsed = time.time() - self._step_start
        step = state.global_step

        if step % self.LOG_EVERY_N_STEPS == 0:
            print(f"  [thermal] step {step}: {elapsed:.1f}s/step", flush=True)

        if elapsed > self.THROTTLE_THRESHOLD_SEC:
            self._throttle_count += 1
            print(
                f"  [thermal] ⚠ step {step} took {elapsed:.1f}s — throttling detected "
                f"(#{self._throttle_count}). Pausing {self.COOLDOWN_SEC}s to cool down ...",
                flush=True,
            )
            time.sleep(self.COOLDOWN_SEC)
            print(f"  [thermal] Resuming.", flush=True)


# ---------------------------------------------------------------------------
# TensorBoard logging callback
# ---------------------------------------------------------------------------

class TensorBoardCallback(TrainerCallback):
    """
    Logs train/loss and eval/perplexity to TensorBoard via a SummaryWriter.
    All logging is wrapped in try/except so a TensorBoard failure never
    crashes training.
    """

    def __init__(self, writer):
        self.writer = writer

    def on_log(self, args, state, control, logs=None, **kwargs):
        if self.writer is None or logs is None:
            return
        if "loss" in logs:
            try:
                self.writer.add_scalar("train/loss", logs["loss"], state.global_step)
            except Exception:
                pass

    def on_evaluate(self, args, state, control, metrics=None, **kwargs):
        if self.writer is None or metrics is None:
            return
        if "eval_loss" in metrics:
            try:
                ppl = math.exp(float(metrics["eval_loss"]))
                self.writer.add_scalar("eval/perplexity", ppl, state.epoch)
            except Exception:
                pass


# ---------------------------------------------------------------------------
# Training loop
# ---------------------------------------------------------------------------

def run(variant: str, datasets_dir: str, out_dir: str, smoke_test: bool,
         resume: bool = False, embed_pretrain: bool = False,
         embed_project: str = "", epochs_override: int = None,
         lr_override: float = None) -> None:
    hf_set_seed(SEED)
    random.seed(SEED)

    # DEC-006: Mutual exclusivity check for embed_project and embed_pretrain
    if embed_project and embed_pretrain:
        print("ERROR: --embed-project and --embed-pretrain are mutually exclusive.")
        print("  Use --embed-project for projection-based initialization,")
        print("  or --embed-pretrain for embedding-only pretraining, not both.")
        sys.exit(1)

    device = "mps" if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Variant: {variant}")
    if embed_project:
        print(f"Embed project: {embed_project}")
    print(f"Output: {out_dir}")
    print()

    # flat-IR ablation uses its own dataset directory and vocab
    is_flat = (variant == "flat")
    if is_flat:
        train_path = os.path.join(datasets_dir, "train_flat.jsonl")
        eval_path  = os.path.join(datasets_dir, "eval_flat.jsonl")
        vocab_path = os.path.join(datasets_dir, "vocab_flat.json")
        expected_source = "flat_ir"
    else:
        train_path = os.path.join(datasets_dir, f"train_{variant}.jsonl")
        eval_path  = os.path.join(datasets_dir, f"eval_{variant}.jsonl")
        # A and B_small both use a custom (truncated) vocab + resized embedding table.
        vocab_path = os.path.join(datasets_dir, f"vocab_{variant}.json") if variant in ("A", "B_small") else None
        expected_source = variant

    assert os.path.exists(train_path), f"Missing: {train_path} — run build_dataset.py first"
    assert os.path.exists(eval_path),  f"Missing: {eval_path}"

    limit = 50 if smoke_test else None
    seq_len = 512
    print(f"Sequence length cap: {seq_len} tokens")
    print(f"Loading datasets{' (smoke test: 50 examples)' if smoke_test else ''} ...")
    train_dataset = IRDataset(train_path, max_len=seq_len, limit=limit, expected_source=expected_source)
    eval_dataset  = IRDataset(eval_path,  max_len=seq_len, limit=limit, expected_source=expected_source)
    print(f"  Train: {len(train_dataset)} examples")
    print(f"  Eval:  {len(eval_dataset)} examples")
    print()

    model, tokenizer, vocab_size = load_model_for_variant(
        variant, vocab_path, device, embed_project_path=embed_project)
    # DEC-013: gradient checkpointing enabled to reduce peak MPS memory and prevent
    # thermal throttling. use_reentrant=False is the MPS-safe setting. Note: this
    # introduces non-determinism on MPS. "Statistically equivalent" is a general
    # claim about the technique — NOT verified empirically for this setup (361M
    # random-init variant A vs 494M pretrained B/C at ~2,200 examples on Apple
    # Silicon). The non-determinism may manifest differently across variants given
    # the asymmetric initialization. Logged here rather than asserted as settled.
    model.gradient_checkpointing_enable(gradient_checkpointing_kwargs={"use_reentrant": False})
    model = model.to(device)
    n_params = sum(p.numel() for p in model.parameters()) / 1e6
    print(f"  vocab_size={vocab_size}")
    print(f"  params={n_params:.1f}M")
    print(f"  gradient_checkpointing=True (use_reentrant=False, MPS-safe — see DEC-013)")

    # -------------------------------------------------------------------------
    # Phase 6 / DEC-006: Option 2 — embedding-only pretrain pass
    # -------------------------------------------------------------------------
    # Before the standard fine-tune, run a 1-epoch pass with all transformer
    # layers frozen. Only the embedding table and lm_head train. This gives the
    # embedding layer a meaningful initialisation on the IR distribution before
    # the full model starts training. Neutralises the cold-start confound so that
    # any remaining gap to B/C is attributable to representation quality rather
    # than initialisation.

    epochs = 1 if smoke_test else (epochs_override if epochs_override is not None else EPOCHS)
    lr = lr_override if lr_override is not None else lr_for_variant(variant)

    if embed_pretrain and not smoke_test:
        print()
        print("=" * 60)
        print("PHASE 6 — DEC-006: Embedding-only pretrain (Option 2)")
        print("=" * 60)
        print("Freezing all transformer layers; training embeddings/lm_head for 1 epoch ...")

        # Freeze everything
        for name, param in model.named_parameters():
            param.requires_grad = False

        # Unfreeze embedding and lm_head
        model.model.embed_tokens.requires_grad_(True)
        model.lm_head.requires_grad_(True)

        embed_params = sum(p.numel() for p in model.parameters() if p.requires_grad) / 1e6
        print(f"  Trainable params (embed-only): {embed_params:.1f}M")

        # 1-epoch embed-only trainer
        embed_trainer_tmp = os.path.join(out_dir, "_embed_pretrain_tmp")
        embed_args = TrainingArguments(
            output_dir=embed_trainer_tmp,
            num_train_epochs=1,
            per_device_train_batch_size=BATCH_SIZE,
            per_device_eval_batch_size=BATCH_SIZE,
            gradient_accumulation_steps=GRAD_ACCUM,
            learning_rate=lr,
            weight_decay=WEIGHT_DECAY,
            warmup_steps=1,
            lr_scheduler_type="linear",
            eval_strategy="no",
            save_strategy="no",
            logging_steps=20,
            seed=SEED,
            report_to="none",
            fp16=False,
            bf16=False,
            dataloader_pin_memory=False,
        )

        embed_trainer = Trainer(
            model=model,
            args=embed_args,
            train_dataset=train_dataset,
            eval_dataset=eval_dataset,
            data_collator=lambda batch: collate_fn(batch),
        )

        t0 = time.time()
        embed_trainer.train()
        elapsed_ep = time.time() - t0
        print(f"  Embedding pretrain complete in {elapsed_ep/60:.1f} min.")

        # Quick eval perplexity after embed pretrain
        print("  Eval perplexity after embed pretrain ...")
        if device == "mps":
            import gc; gc.collect(); torch.mps.empty_cache()
        embed_ppl = evaluate_perplexity(model, eval_dataset, device)
        print(f"  Embed-pretrain eval perplexity: {embed_ppl:.3f}")

        # Unfreeze everything for fine-tune
        for param in model.parameters():
            param.requires_grad = True
        print("  Unfrozen all layers — ready for fine-tune.")
        print("=" * 60)
        print()
    elif embed_pretrain and smoke_test:
        print("  Note: --embed-pretrain skipped in smoke-test mode.")
        print()

    os.makedirs(out_dir, exist_ok=True)

    # Log decoded training samples for visual validation before training starts.
    log_dataset_samples(
        train_path=train_path,
        variant=variant,
        tokenizer=tokenizer,
        vocab_path=vocab_path,
        n_samples=3,
        out_dir=out_dir,
    )
    total_steps = max(1, (len(train_dataset) // (BATCH_SIZE * GRAD_ACCUM)) * epochs)
    warmup_steps = max(1, int(WARMUP_RATIO * total_steps))

    # TensorBoard SummaryWriter — event files go in runs/{out_dir}/tb/
    writer = None
    if _tb_available:
        try:
            writer = SummaryWriter(log_dir=os.path.join(out_dir, "tb"))
            writer.add_text("config/variant", str(variant), 0)
            writer.add_text("config/epochs", str(epochs), 0)
            writer.add_text("config/vocab_size", str(vocab_size), 0)
            writer.add_text("config/learning_rate", str(lr), 0)
        except Exception as e:
            print(f"  Warning: TensorBoard writer init failed ({e}); logging disabled.")
            writer = None
    # Checkpoint ~twice per epoch so a mid-epoch stall (sleep/disconnect) can be
    # resumed via --resume instead of restarting epoch 0 from scratch.
    steps_per_epoch = max(1, len(train_dataset) // (BATCH_SIZE * GRAD_ACCUM))
    save_steps = max(1, steps_per_epoch // 2)

    # Use a temp subdir for HF Trainer internals; we do our own final save below.
    trainer_tmp = os.path.join(out_dir, "_trainer_tmp")

    training_args = TrainingArguments(
        output_dir=trainer_tmp,
        num_train_epochs=epochs,
        per_device_train_batch_size=BATCH_SIZE,
        per_device_eval_batch_size=BATCH_SIZE,
        gradient_accumulation_steps=GRAD_ACCUM,
        learning_rate=lr,
        weight_decay=WEIGHT_DECAY,
        warmup_steps=warmup_steps,
        lr_scheduler_type="cosine",
        eval_strategy="epoch",
        save_strategy="steps",        # checkpoint mid-epoch so stalls can be resumed
        save_steps=save_steps,        # ~2 checkpoints per epoch (see computation above)
        save_total_limit=3,           # rotate: keep the 3 most recent
        load_best_model_at_end=False,
        logging_steps=10,
        seed=SEED,
        report_to="none",
        fp16=False,
        bf16=False,
        dataloader_pin_memory=False,
    )

    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        data_collator=lambda batch: collate_fn(batch),
        callbacks=[ThermalGuardCallback(), TensorBoardCallback(writer)],
    )

    # Resolve resume checkpoint — find the latest epoch checkpoint in out_dir.
    resume_from = None
    if resume:
        import glob
        ckpt_glob = glob.glob(os.path.join(out_dir, "_trainer_tmp", "checkpoint-*"))
        # Sort by the numeric step in the dir name (checkpoint-200 < checkpoint-1000);
        # plain lexicographic sort misorders once step counts exceed 3 digits.
        ckpt_dirs = sorted(ckpt_glob, key=lambda p: int(os.path.basename(p).split("-")[-1]))
        if ckpt_dirs:
            resume_from = ckpt_dirs[-1]
            print(f"Resuming from checkpoint: {resume_from}")
        else:
            print("--resume set but no checkpoint found in out_dir — starting fresh.")

    print(f"Training variant {variant} for {epochs} epoch(s) ...")
    t0 = time.time()
    trainer.train(resume_from_checkpoint=resume_from)
    elapsed = time.time() - t0
    print(f"Training complete in {elapsed/60:.1f} minutes.")
    print()

    # Save final checkpoint — weights + config only, no optimizer state.
    final_ckpt = os.path.join(out_dir, "checkpoint-final")
    print(f"Saving final checkpoint → {final_ckpt}")
    model.save_pretrained(final_ckpt)
    tokenizer.save_pretrained(final_ckpt)

    # Clean up HF Trainer temp dir (contains no useful artefacts).
    import shutil
    if os.path.exists(trainer_tmp):
        shutil.rmtree(trainer_tmp)

    # Save loss curve from trainer log history.
    loss_curve = {
        "train": [
            {"step": e["step"], "loss": e["loss"]}
            for e in trainer.state.log_history
            if "loss" in e and "eval_loss" not in e
        ],
        "eval": [
            {"step": e["step"], "eval_loss": e["eval_loss"]}
            for e in trainer.state.log_history
            if "eval_loss" in e
        ],
    }
    loss_curve_path = os.path.join(out_dir, "loss_curve.json")
    with open(loss_curve_path, "w") as f:
        json.dump(loss_curve, f, indent=2)
    print(f"Loss curves saved → {loss_curve_path}")
    print(f"  Train loss points: {len(loss_curve['train'])} | Eval loss points: {len(loss_curve['eval'])}")
    print()

    print("Computing final perplexity on eval split ...")
    if device == "mps":
        import gc; gc.collect(); torch.mps.empty_cache()
    ppl = evaluate_perplexity(model, eval_dataset, device)
    print(f"  Variant {variant} eval perplexity: {ppl:.2f}")
    print()

    if writer is not None:
        try:
            writer.add_scalar("eval/perplexity", ppl, epochs)
        except Exception:
            pass

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
        "embed_pretrain": embed_pretrain,
        "embed_project": embed_project,
        "learning_rate": lr,
        "effective_batch_size": BATCH_SIZE * GRAD_ACCUM,
        "max_seq_len": MAX_SEQ_LEN,
    }
    if embed_pretrain and not smoke_test:
        results["embed_pretrain_eval_ppl"] = round(embed_ppl, 4)
    results_path = os.path.join(out_dir, "results.json")
    with open(results_path, "w") as f:
        json.dump(results, f, indent=2)
    print(f"Results saved → {results_path}")

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

    if writer is not None:
        try:
            writer.close()
        except Exception:
            pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--variant",    required=True, choices=["A", "B", "C", "flat", "B_small"])
    parser.add_argument("--datasets",   default="datasets/")
    parser.add_argument("--out",        default=None)
    parser.add_argument("--smoke-test", action="store_true",
                        help="Quick 1-epoch run on 50 examples to verify the pipeline")
    parser.add_argument("--resume",     action="store_true",
                        help="Resume from the latest epoch checkpoint in --out if one exists")
    parser.add_argument("--embed-pretrain", action="store_true",
                        help="Phase 6 DEC-006: 1-epoch embedding-only pretrain before fine-tune. "
                             "Freezes transformer layers; trains embeddings/lm_head for 1 epoch, "
                             "then unfreezes and trains normally. Neutralises the cold-start "
                             "embedding confound for Variant A.")
    parser.add_argument("--embed-project", type=str, default="",
                        help="Path to embedding projection matrix (.pt file) for Variant A. "
                             "If provided, loads the projection and applies it to initialize "
                             "the embedding table. Mutually exclusive with --embed-pretrain.")
    parser.add_argument("--epochs", type=int, default=None,
                        help="Override the number of training epochs (default: 2). "
                             "Used by launch_run.py to pass the configured epoch count.")
    parser.add_argument("--lr", type=float, default=None,
                        help="Override the learning rate (default: 2e-4 for A/B_small/flat, "
                             "5e-5 for B/C). Used by launch_run.py to pass the configured LR.")
    args = parser.parse_args()

    # Default output goes directly into the Studio artifacts directory so Kit
    # can read results without any copying or syncing.
    artifacts = os.environ.get("SC_ARTIFACTS_DIR", "")
    if args.out:
        out = args.out
    elif artifacts:
        out = os.path.join(artifacts, "maith-runs", f"variant_{args.variant}")
    else:
        out = f"runs/variant_{args.variant}"

    run(args.variant, args.datasets, out, args.smoke_test, resume=args.resume,
        embed_pretrain=args.embed_pretrain, embed_project=args.embed_project,
        epochs_override=args.epochs, lr_override=args.lr)
