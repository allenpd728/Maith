#!/usr/bin/env python3
"""
train_h9_cotrain.py

H9 co-training: joint next-token prediction + NT-Xent contrastive auxiliary loss,
trained from the pretrained base (not from A_v3_2ep).

The central question (H9): does applying the contrastive signal *during* next-token
learning produce a better representation than sequential post-hoc fine-tuning (H5)?

Design:
  - Base: Qwen2.5-Coder-0.5B pretrained (same starting point as A_v3_2ep)
  - Variant: A (IR tokens, 2254-vocab, same dataset as A_v3_2ep)
  - Objective: loss = (1 - λ) * ntp_loss + λ * nt_xent_loss  [λ = 0.1]
  - NTP loss: standard causal LM next-token prediction on IR tokens
  - NT-Xent loss: SimCSE-style in-batch negatives on dependency pairs
  - Epochs: 2 (matched to A_v3_2ep for comparison validity)
  - Batch: 1, grad_accum=8 (effective batch 8 — standard protocol)
  - LR: 2e-4 (standard A-variant lr)
  - Pairs: datasets/h5_pairs_train.json (already built for H5)

Batch construction:
  Each step samples one batch of NTP examples. If a pair is available for any example
  in the batch, a contrastive forward pass also runs on that pair. Steps with no
  available pairs contribute NTP loss only. The contrastive loss is computed on up to
  CONTRASTIVE_BATCH_SIZE pairs per step regardless of effective NTP batch size.

Gate contract (enforced before any training):
  - Pairs file exists and has n_pairs >= 100
  - Base checkpoint is pretrained Qwen (not A_v3_2ep — H9 must not start from converged IR)
  - Dataset is train_A.jsonl, expected_source='A'
  - results.json written atomically at end; partial runs leave no results.json

Comparison validity (per EXPERIMENT_DESIGN.md):
  - epochs=2, seed=42, train_examples=3375, max_seq_len=512
  - Must match A_v3_2ep on all four parameters for a valid comparison

Output layout:
    runs/variant_A_h9/
        checkpoint-final/       # model weights
        results.json            # config + metrics (incl. ntp_loss, contrastive_loss)
        loss_curve.json         # per-step breakdown: total, ntp, contrastive

Usage:
    python3 python/train_h9_cotrain.py \\
        --dataset datasets/train_A.jsonl \\
        --eval-dataset datasets/eval_A.jsonl \\
        --vocab datasets/vocab_A.json \\
        --pairs datasets/h5_pairs_train.json \\
        --out runs/variant_A_h9

    # Smoke test (fast, 50 examples, 1 epoch):
    python3 python/train_h9_cotrain.py --smoke-test \\
        --dataset datasets/train_A.jsonl \\
        --eval-dataset datasets/eval_A.jsonl \\
        --vocab datasets/vocab_A.json \\
        --pairs datasets/h5_pairs_train.json \\
        --out runs/smoke_h9
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
    import torch.nn.functional as F
except ImportError:
    missing.append("torch")

try:
    from transformers import (
        AutoConfig, AutoModelForCausalLM, AutoTokenizer,
        set_seed as hf_set_seed,
    )
except ImportError:
    missing.append("transformers")

try:
    from torch.utils.data import Dataset, DataLoader
except ImportError:
    pass

_tb_available = True
try:
    from torch.utils.tensorboard import SummaryWriter
except ImportError:
    _tb_available = False

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    sys.exit(1)


# ---------------------------------------------------------------------------
# Constants (matched to EXPERIMENT_DESIGN.md standard protocol)
# ---------------------------------------------------------------------------

BASE_MODEL              = "Qwen/Qwen2.5-Coder-0.5B"
EPOCHS                  = 2
BATCH_SIZE              = 1         # per-device
GRAD_ACCUM              = 8         # effective batch = 8
LEARNING_RATE           = 2e-4
WEIGHT_DECAY            = 0.01
WARMUP_RATIO            = 0.05
SEED                    = 42
MAX_SEQ_LEN             = 512
CONTRASTIVE_LAMBDA      = 0.1       # weight of contrastive loss in joint objective
CONTRASTIVE_TEMPERATURE = 0.07      # NT-Xent temperature
CONTRASTIVE_BATCH_SIZE  = 16        # number of pairs per contrastive step


# ---------------------------------------------------------------------------
# NT-Xent loss (identical to H5 — tested in test_h5_contrastive.py)
# ---------------------------------------------------------------------------

def nt_xent_loss(anchor_emb: "torch.Tensor", positive_emb: "torch.Tensor",
                 temperature: float = CONTRASTIVE_TEMPERATURE) -> "torch.Tensor":
    """
    Normalized temperature-scaled cross-entropy loss (SimCSE-style).

    anchor_emb:   (N, D) L2-normalized embeddings
    positive_emb: (N, D) L2-normalized embeddings
    Returns scalar loss.
    """
    batch_size = anchor_emb.size(0)
    all_emb = torch.cat([anchor_emb, positive_emb], dim=0)   # (2N, D)
    sim = torch.mm(all_emb, all_emb.t()) / temperature       # (2N, 2N)
    mask = torch.eye(2 * batch_size, device=sim.device).bool()
    sim.masked_fill_(mask, float("-inf"))
    labels = torch.arange(batch_size, device=sim.device) + batch_size
    return F.cross_entropy(sim[:batch_size], labels)


def mean_pool(hidden_states: "torch.Tensor",
              attention_mask: "torch.Tensor") -> "torch.Tensor":
    """Mean pool over non-padding tokens, L2-normalize."""
    mask = attention_mask.unsqueeze(-1).float()
    summed = (hidden_states * mask).sum(dim=1)
    counts = mask.sum(dim=1).clamp(min=1)
    return F.normalize(summed / counts, p=2, dim=-1)


# ---------------------------------------------------------------------------
# NTP Dataset (identical pattern to train_v2_resume.IRDataset)
# ---------------------------------------------------------------------------

class NTPDataset(Dataset):
    """Pre-built JSONL split → next-token prediction examples."""

    def __init__(self, jsonl_path: Path, max_len: int = MAX_SEQ_LEN,
                 limit: Optional[int] = None, expected_source: str = "A"):
        self.examples = []
        with open(jsonl_path) as f:
            for i, line in enumerate(f):
                if limit and i >= limit:
                    break
                row = json.loads(line)
                src = row.get("source")
                assert src == expected_source, (
                    f"Source mismatch at line {i}: expected '{expected_source}' "
                    f"got '{src}'. Wrong dataset file loaded."
                )
                ids    = row["input_ids"][:max_len]
                labels = row["labels"][:max_len]
                self.examples.append({
                    "input_ids":      ids,
                    "attention_mask": [1] * len(ids),
                    "labels":         labels,
                    "name":           row.get("name", ""),
                })

    def __len__(self):
        return len(self.examples)

    def __getitem__(self, idx):
        ex = self.examples[idx]
        return {k: torch.tensor(v, dtype=torch.long)
                for k, v in ex.items() if k != "name"}

    def names(self):
        return [ex["name"] for ex in self.examples]


def ntp_collate(batch: list[dict], pad_id: int = 0) -> dict:
    max_len = max(ex["input_ids"].shape[0] for ex in batch)
    B = len(batch)
    input_ids      = torch.zeros(B, max_len, dtype=torch.long)
    attention_mask = torch.zeros(B, max_len, dtype=torch.long)
    labels         = torch.full((B, max_len), -100, dtype=torch.long)
    for i, ex in enumerate(batch):
        n = ex["input_ids"].shape[0]
        input_ids[i, :n]      = ex["input_ids"]
        attention_mask[i, :n] = ex["attention_mask"]
        labels[i, :n]         = ex["labels"]
    return {"input_ids": input_ids, "attention_mask": attention_mask, "labels": labels}


# ---------------------------------------------------------------------------
# Pair sampler
# ---------------------------------------------------------------------------

class PairSampler:
    """
    Wraps the h5_pairs_train.json pairs for on-demand contrastive batches.
    Loads train_A rows by index for fast lookup.
    """

    def __init__(self, pairs_path: Path, dataset_path: Path,
                 max_seq_len: int = MAX_SEQ_LEN, seed: int = SEED):
        with open(pairs_path) as f:
            data = json.load(f)
        self.pairs = data["pairs"]          # list of [anchor_idx, positive_idx]
        self.n_pairs = len(self.pairs)

        # Load all rows indexed by position
        self.rows = []
        with open(dataset_path) as f:
            for line in f:
                line = line.strip()
                if line:
                    self.rows.append(json.loads(line))

        self.max_seq_len = max_seq_len
        self._rng = random.Random(seed)
        self._order = list(range(self.n_pairs))
        self._rng.shuffle(self._order)
        self._cursor = 0

    def sample_batch(self, batch_size: int) -> Optional[tuple]:
        """
        Draw batch_size pairs. Returns (anchor_ids, anchor_mask, pos_ids, pos_mask)
        as tensors, or None if fewer than batch_size pairs remain in this epoch.
        Re-shuffles automatically when the epoch wraps.
        """
        if self.n_pairs < batch_size:
            return None

        indices = []
        for _ in range(batch_size):
            if self._cursor >= self.n_pairs:
                self._rng.shuffle(self._order)
                self._cursor = 0
            indices.append(self._order[self._cursor])
            self._cursor += 1

        anchor_seqs, pos_seqs = [], []
        for idx in indices:
            a_i, p_i = self.pairs[idx]
            anchor_seqs.append(self.rows[a_i]["input_ids"][:self.max_seq_len])
            pos_seqs.append(self.rows[p_i]["input_ids"][:self.max_seq_len])

        def pad(seqs):
            max_len = max(len(s) for s in seqs)
            ids  = torch.zeros(len(seqs), max_len, dtype=torch.long)
            mask = torch.zeros(len(seqs), max_len, dtype=torch.long)
            for i, s in enumerate(seqs):
                ids[i, :len(s)]  = torch.tensor(s, dtype=torch.long)
                mask[i, :len(s)] = 1
            return ids, mask

        a_ids, a_mask = pad(anchor_seqs)
        p_ids, p_mask = pad(pos_seqs)
        return a_ids, a_mask, p_ids, p_mask

    def reset_epoch(self):
        self._rng.shuffle(self._order)
        self._cursor = 0


# ---------------------------------------------------------------------------
# Pre-flight gate
# ---------------------------------------------------------------------------

def preflight_gate(pairs_path: Path, dataset_path: Path,
                   vocab_path: Path) -> list[str]:
    """
    Pre-flight checks before training. Returns list of error strings (empty = pass).
    Mirrors PIPELINE_QUALITY_GATES Station 4 contract.
    """
    errors = []

    # G-H9-1: pairs file exists and is valid
    if not pairs_path.exists():
        errors.append(f"G-H9-1 FAIL: pairs file not found: {pairs_path}")
    else:
        try:
            with open(pairs_path) as f:
                d = json.load(f)
            n = d.get("n_pairs", len(d.get("pairs", [])))
            if n < 100:
                errors.append(f"G-H9-1 FAIL: only {n} pairs — need >= 100")
            else:
                print(f"  PASS  G-H9-1 pairs file: {n} pairs")
        except Exception as e:
            errors.append(f"G-H9-1 FAIL: could not read pairs file: {e}")

    # G-H9-2: training dataset exists and has expected source
    if not dataset_path.exists():
        errors.append(f"G-H9-2 FAIL: dataset not found: {dataset_path}")
    else:
        # Spot-check first 10 rows for source='A'
        try:
            with open(dataset_path) as f:
                for i, line in enumerate(f):
                    if i >= 10:
                        break
                    row = json.loads(line)
                    if row.get("source") != "A":
                        errors.append(
                            f"G-H9-2 FAIL: dataset row {i} has source='{row.get('source')}'"
                            f" — expected 'A'. Wrong dataset file?"
                        )
                        break
            if not errors or "G-H9-2" not in errors[-1]:
                print(f"  PASS  G-H9-2 dataset exists, source='A': {dataset_path}")
        except Exception as e:
            errors.append(f"G-H9-2 FAIL: could not read dataset: {e}")

    # G-H9-3: vocab file exists
    if not vocab_path.exists():
        errors.append(f"G-H9-3 FAIL: vocab not found: {vocab_path}")
    else:
        try:
            with open(vocab_path) as f:
                vocab = json.load(f)
            print(f"  PASS  G-H9-3 vocab exists: {len(vocab)} tokens")
        except Exception as e:
            errors.append(f"G-H9-3 FAIL: could not read vocab: {e}")

    # G-H9-4: pairs index within bounds of dataset
    if pairs_path.exists() and dataset_path.exists():
        try:
            with open(pairs_path) as f:
                pairs_data = json.load(f)
            with open(dataset_path) as f:
                n_rows = sum(1 for _ in f)
            bad = [(a, b) for a, b in pairs_data.get("pairs", [])
                   if a >= n_rows or b >= n_rows]
            if bad:
                errors.append(
                    f"G-H9-4 FAIL: {len(bad)} pairs have out-of-range indices "
                    f"(dataset has {n_rows} rows). Example: {bad[0]}"
                )
            else:
                print(f"  PASS  G-H9-4 all pair indices within dataset bounds (n={n_rows})")
        except Exception as e:
            errors.append(f"G-H9-4 FAIL: index check error: {e}")

    return errors


# ---------------------------------------------------------------------------
# Perplexity evaluation (identical to train_v2_resume)
# ---------------------------------------------------------------------------

def evaluate_perplexity(model, dataset: NTPDataset, device: str,
                        batch_size: int = 1) -> float:
    model.eval()
    total_loss, total_tokens = 0.0, 0
    with torch.no_grad():
        for start in range(0, len(dataset), batch_size):
            batch = [dataset[i] for i in range(start, min(start + batch_size, len(dataset)))]
            tensors = ntp_collate(batch)
            tensors = {k: v.to(device) for k, v in tensors.items()}
            n_tokens = (tensors["labels"] != -100).sum().item()
            if n_tokens < 2:
                continue
            out = model(**tensors)
            val = out.loss.item()
            if math.isnan(val) or math.isinf(val):
                continue
            total_loss   += val * n_tokens
            total_tokens += n_tokens
    avg = total_loss / max(total_tokens, 1)
    return math.exp(avg)


# ---------------------------------------------------------------------------
# Main training loop
# ---------------------------------------------------------------------------

def run(dataset_path: Path, eval_dataset_path: Path, vocab_path: Path,
        pairs_path: Path, out_dir: Path, smoke_test: bool,
        lam: float = CONTRASTIVE_LAMBDA,
        temperature: float = CONTRASTIVE_TEMPERATURE,
        contrastive_batch: int = CONTRASTIVE_BATCH_SIZE) -> None:

    hf_set_seed(SEED)
    random.seed(SEED)

    device = ("mps"  if torch.backends.mps.is_available()  else
              "cuda" if torch.cuda.is_available()           else "cpu")
    epochs = 1 if smoke_test else EPOCHS
    limit  = 50 if smoke_test else None

    print("=" * 60)
    print("H9 Co-Training — Joint NTP + NT-Xent")
    print("=" * 60)
    print(f"  Device:              {device}")
    print(f"  Base model:          {BASE_MODEL}")
    print(f"  Epochs:              {epochs}")
    print(f"  Batch:               {BATCH_SIZE} (grad_accum={GRAD_ACCUM}, effective={BATCH_SIZE * GRAD_ACCUM})")
    print(f"  LR:                  {LEARNING_RATE}")
    print(f"  Lambda (contrastive):{lam}")
    print(f"  Temperature:         {temperature}")
    print(f"  Contrastive batch:   {contrastive_batch} pairs/step")
    print(f"  Max seq len:         {MAX_SEQ_LEN}")
    print(f"  Seed:                {SEED}")
    print(f"  Smoke test:          {smoke_test}")
    print(f"  Output:              {out_dir}")
    print()

    # Pre-flight gate
    print("Running pre-flight gate ...")
    errors = preflight_gate(pairs_path, dataset_path, vocab_path)
    if errors:
        for e in errors:
            print(f"  {e}")
        print("\nPRE-FLIGHT FAILED — aborting. Fix errors above before training.")
        sys.exit(1)
    print("Pre-flight gate passed.\n")

    # Load vocab and resize model
    with open(vocab_path) as f:
        vocab = json.load(f)
    vocab_size = len(vocab)

    print(f"Loading base model: {BASE_MODEL} ...")
    model = AutoModelForCausalLM.from_pretrained(
        BASE_MODEL, trust_remote_code=True, dtype=torch.float32)
    tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)
    model.resize_token_embeddings(vocab_size)
    model.config.bos_token_id = None
    model.config.eos_token_id = None
    model.gradient_checkpointing_enable(
        gradient_checkpointing_kwargs={"use_reentrant": False})
    model = model.to(device)

    n_params = sum(p.numel() for p in model.parameters()) / 1e6
    print(f"  vocab_size={vocab_size}, params={n_params:.1f}M")
    print()

    # Datasets
    print("Loading datasets ...")
    train_ds = NTPDataset(dataset_path,      limit=limit, expected_source="A")
    eval_ds  = NTPDataset(eval_dataset_path, limit=limit, expected_source="A")
    print(f"  Train: {len(train_ds)} examples")
    print(f"  Eval:  {len(eval_ds)} examples")

    train_loader = DataLoader(
        train_ds, batch_size=BATCH_SIZE, shuffle=True,
        collate_fn=ntp_collate, drop_last=False)

    pair_sampler = PairSampler(pairs_path, dataset_path, seed=SEED)
    print(f"  Pairs: {pair_sampler.n_pairs} available")
    print()

    # Optimizer and scheduler
    optimizer = torch.optim.AdamW(
        model.parameters(), lr=LEARNING_RATE, weight_decay=WEIGHT_DECAY)

    total_steps   = max(1, len(train_loader) * epochs // GRAD_ACCUM)
    warmup_steps  = max(1, int(WARMUP_RATIO * total_steps))

    def lr_lambda(step):
        if step < warmup_steps:
            return step / warmup_steps
        progress = (step - warmup_steps) / max(1, total_steps - warmup_steps)
        return max(0.0, 0.5 * (1.0 + math.cos(math.pi * progress)))

    scheduler = torch.optim.lr_scheduler.LambdaLR(optimizer, lr_lambda)

    # TensorBoard
    writer = None
    if _tb_available:
        try:
            writer = SummaryWriter(log_dir=str(out_dir / "tb"))
        except Exception:
            pass

    os.makedirs(out_dir, exist_ok=True)

    # Training loop
    loss_curve = []      # {step, total_loss, ntp_loss, contrastive_loss}
    global_step = 0
    accum_step  = 0
    accum_ntp   = 0.0
    accum_con   = 0.0
    t_start     = time.time()

    initial_ntp_loss = None
    final_ntp_loss   = None

    print("Starting training ...")
    for epoch in range(epochs):
        pair_sampler.reset_epoch()
        model.train()

        for batch in train_loader:
            batch = {k: v.to(device) for k, v in batch.items()}

            # --- NTP forward pass ---
            out = model(**batch)
            ntp_loss = out.loss

            # --- Contrastive forward pass ---
            pair_batch = pair_sampler.sample_batch(contrastive_batch)
            if pair_batch is not None:
                a_ids, a_mask, p_ids, p_mask = [t.to(device) for t in pair_batch]

                with model.no_sync() if hasattr(model, "no_sync") else _null_ctx():
                    a_out = model(input_ids=a_ids, attention_mask=a_mask,
                                  output_hidden_states=True)
                    p_out = model(input_ids=p_ids, attention_mask=p_mask,
                                  output_hidden_states=True)

                a_emb = mean_pool(a_out.hidden_states[-1], a_mask)
                p_emb = mean_pool(p_out.hidden_states[-1], p_mask)
                con_loss = nt_xent_loss(a_emb, p_emb, temperature)
            else:
                con_loss = torch.tensor(0.0, device=device)

            # Combined loss
            loss = (1.0 - lam) * ntp_loss + lam * con_loss
            loss = loss / GRAD_ACCUM
            loss.backward()

            accum_ntp += ntp_loss.item()
            accum_con += con_loss.item()
            accum_step += 1

            if accum_step >= GRAD_ACCUM:
                torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
                optimizer.step()
                scheduler.step()
                optimizer.zero_grad()

                avg_ntp = accum_ntp / GRAD_ACCUM
                avg_con = accum_con / GRAD_ACCUM
                avg_tot = (1.0 - lam) * avg_ntp + lam * avg_con

                if initial_ntp_loss is None:
                    initial_ntp_loss = avg_ntp
                final_ntp_loss = avg_ntp

                loss_curve.append({
                    "step":             global_step,
                    "total_loss":       round(avg_tot, 6),
                    "ntp_loss":         round(avg_ntp, 6),
                    "contrastive_loss": round(avg_con, 6),
                })

                if global_step % 10 == 0:
                    elapsed = time.time() - t_start
                    steps_done  = global_step + 1
                    steps_total = total_steps
                    eta = (elapsed / steps_done) * (steps_total - steps_done) if steps_done > 0 else 0
                    print(
                        f"  epoch {epoch+1}/{epochs} step {global_step}/{total_steps} "
                        f"ntp={avg_ntp:.4f} con={avg_con:.4f} tot={avg_tot:.4f} "
                        f"elapsed={elapsed:.0f}s eta={eta:.0f}s",
                        flush=True,
                    )

                if writer:
                    try:
                        writer.add_scalar("train/ntp_loss",         avg_ntp, global_step)
                        writer.add_scalar("train/contrastive_loss",  avg_con, global_step)
                        writer.add_scalar("train/total_loss",        avg_tot, global_step)
                    except Exception:
                        pass

                global_step += 1
                accum_step   = 0
                accum_ntp    = 0.0
                accum_con    = 0.0

        print(f"Epoch {epoch+1}/{epochs} complete.")

        # Mid-run checkpoint
        if not smoke_test and epoch == 0 and epochs > 1:
            mid_ckpt = out_dir / f"checkpoint-step{global_step}"
            model.save_pretrained(mid_ckpt)
            print(f"  [mid-run checkpoint saved: {mid_ckpt}]")

    # Final checkpoint
    elapsed_total = time.time() - t_start
    print(f"\nTraining complete in {elapsed_total:.0f}s ({elapsed_total/3600:.1f}h)")

    final_ckpt = out_dir / "checkpoint-final"
    print(f"Saving final checkpoint → {final_ckpt}")
    model.save_pretrained(final_ckpt)
    tokenizer.save_pretrained(final_ckpt)

    # Perplexity
    print("Computing final perplexity ...")
    if device == "mps":
        import gc; gc.collect(); torch.mps.empty_cache()
    ppl = evaluate_perplexity(model, eval_ds, device)
    print(f"  Eval perplexity: {ppl:.4f}")

    # Loss descended check
    loss_descended = (
        final_ntp_loss is not None and
        initial_ntp_loss is not None and
        final_ntp_loss < initial_ntp_loss
    )

    # Save loss curve
    lc_path = out_dir / "loss_curve.json"
    with open(lc_path, "w") as f:
        json.dump(loss_curve, f, indent=2)
    print(f"Loss curve saved → {lc_path}")

    # Save results (comparison validity fields required by EXPERIMENT_DESIGN.md)
    results = {
        "hypothesis":          "H9",
        "objective":           "ntp_plus_nt_xent",
        "base_model":          BASE_MODEL,
        "base_checkpoint":     "pretrained (not A_v3_2ep)",
        "variant":             "A_h9",
        "dataset":             str(dataset_path),
        "pairs_file":          str(pairs_path),
        "n_pairs":             pair_sampler.n_pairs,
        "vocab_size":          vocab_size,
        "n_params_M":          round(n_params, 1),
        "train_examples":      len(train_ds),
        "eval_examples":       len(eval_ds),
        "epochs":              epochs,
        "seed":                SEED,
        "batch_size":          BATCH_SIZE,
        "grad_accum":          GRAD_ACCUM,
        "effective_batch_size": BATCH_SIZE * GRAD_ACCUM,
        "learning_rate":       LEARNING_RATE,
        "max_seq_len":         MAX_SEQ_LEN,
        "lambda_contrastive":  lam,
        "temperature":         temperature,
        "contrastive_batch":   contrastive_batch,
        "total_steps":         global_step,
        "initial_ntp_loss":    round(initial_ntp_loss, 6) if initial_ntp_loss else None,
        "final_ntp_loss":      round(final_ntp_loss, 6)   if final_ntp_loss   else None,
        "loss_descended":      loss_descended,
        "eval_perplexity":     round(ppl, 4),
        "duration_s":          round(elapsed_total, 1),
        "device":              device,
        "smoke_test":          smoke_test,
    }
    results_path = out_dir / "results.json"
    with open(results_path, "w") as f:
        json.dump(results, f, indent=2)
    print(f"Results saved → {results_path}")

    print()
    print("=== H9 Training Summary ===")
    print(f"  Initial NTP loss:  {initial_ntp_loss:.4f}" if initial_ntp_loss else "  Initial NTP loss: N/A")
    print(f"  Final NTP loss:    {final_ntp_loss:.4f}"   if final_ntp_loss   else "  Final NTP loss:   N/A")
    print(f"  Loss descended:    {loss_descended}")
    print(f"  Eval perplexity:   {ppl:.4f}")
    print(f"  Duration:          {elapsed_total:.0f}s")
    print()
    print("Next step: extract embeddings and run retrieval eval:")
    print("  manage.py run-alias extract-h9")
    print("  manage.py run-alias eval-h9")
    print("  manage.py run-alias eval-h9-mode2")

    if writer:
        try:
            writer.add_scalar("eval/perplexity", ppl, epochs)
            writer.close()
        except Exception:
            pass


# ---------------------------------------------------------------------------
# Null context manager (for no_sync fallback)
# ---------------------------------------------------------------------------

from contextlib import contextmanager

@contextmanager
def _null_ctx():
    yield


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    REPO = Path(__file__).resolve().parent.parent

    parser = argparse.ArgumentParser(description="H9 co-training: joint NTP + NT-Xent")
    parser.add_argument("--dataset",      default=str(REPO / "datasets" / "train_A.jsonl"))
    parser.add_argument("--eval-dataset", default=str(REPO / "datasets" / "eval_A.jsonl"))
    parser.add_argument("--vocab",        default=str(REPO / "datasets" / "vocab_A.json"))
    parser.add_argument("--pairs",        default=str(REPO / "datasets" / "h5_pairs_train.json"))
    parser.add_argument("--out",          default=str(REPO / "runs" / "variant_A_h9"))
    parser.add_argument("--smoke-test",   action="store_true")
    parser.add_argument("--lambda",       type=float, default=CONTRASTIVE_LAMBDA,
                        dest="lam", metavar="LAMBDA")
    parser.add_argument("--temperature",  type=float, default=CONTRASTIVE_TEMPERATURE)
    parser.add_argument("--contrastive-batch", type=int, default=CONTRASTIVE_BATCH_SIZE)
    args = parser.parse_args()

    run(
        dataset_path      = Path(args.dataset),
        eval_dataset_path = Path(args.eval_dataset),
        vocab_path        = Path(args.vocab),
        pairs_path        = Path(args.pairs),
        out_dir           = Path(args.out),
        smoke_test        = args.smoke_test,
        lam               = args.lam,
        temperature       = args.temperature,
        contrastive_batch = args.contrastive_batch,
    )
