#!/usr/bin/env python3
"""
train_h5_contrastive.py

H5 contrastive training: fine-tune a checkpoint with SimCSE-style in-batch
negatives to test whether a retrieval-oriented objective produces better
embeddings than next-token prediction.

The central question (H5): does the training objective — not the representation —
explain the weak retrieval performance seen in H6?

Design:
  - Anchor: embedding of declaration i (mean-pool final hidden layer)
  - Positive: embedding of a declaration sharing >= 1 dependency with i
  - Negatives: all other anchors in the batch (in-batch negatives)
  - Loss: NT-Xent (normalized temperature-scaled cross-entropy) a.k.a. SimCSE loss
  - Base: A_v3_2ep checkpoint (already fine-tuned on IR; we add contrastive fine-tuning)
  - Eval metric: retrieval Recall@10 on the H6 clean benchmark

Gate contract (enforced before any training):
  - Pairs file exists and has n_pairs >= 100
  - Base checkpoint exists and has a config.json
  - Dataset file used to build pairs is the same as the checkpoint's training data
  - results.json written atomically at the end; partial runs leave no results.json

Output layout:
    <out_dir>/
        checkpoint-final/       # model weights
        results.json            # eval metrics + config record
        loss_curve.json         # per-step NT-Xent loss

Usage:
    python3 python/train_h5_contrastive.py \\
        --pairs datasets/h5_pairs_train.json \\
        --dataset datasets/train_A.jsonl \\
        --checkpoint runs/variant_A_v3_2ep/checkpoint-final \\
        --out runs/variant_A_h5 \\
        --epochs 3 \\
        --batch-size 32 \\
        --temperature 0.07 \\
        --lr 2e-5

    # Smoke test (fast, no GPU needed):
    python3 python/train_h5_contrastive.py --smoke-test \\
        --pairs datasets/h5_pairs_train.json \\
        --dataset datasets/train_A.jsonl \\
        --checkpoint runs/variant_A_v3_2ep/checkpoint-final \\
        --out runs/smoke_h5
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
    from transformers import AutoModel, AutoConfig, AutoTokenizer, set_seed as hf_set_seed
except ImportError:
    missing.append("transformers")

try:
    from torch.utils.data import Dataset, DataLoader
except ImportError:
    pass

try:
    from torch.utils.tensorboard import SummaryWriter
    _tb_available = True
except ImportError:
    _tb_available = False

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    sys.exit(1)


# ---------------------------------------------------------------------------
# NT-Xent loss (SimCSE)
# ---------------------------------------------------------------------------

def nt_xent_loss(anchor_emb: "torch.Tensor", positive_emb: "torch.Tensor",
                 temperature: float = 0.07) -> "torch.Tensor":
    """
    Normalized temperature-scaled cross-entropy loss.

    anchor_emb:   (N, D) L2-normalized embeddings of anchors
    positive_emb: (N, D) L2-normalized embeddings of positives
    temperature:  scalar, typically 0.05–0.1

    For each anchor i, the positive is positive_emb[i] and all other
    (anchor_emb[j], positive_emb[j]) for j != i are treated as negatives.

    Returns scalar loss.
    """
    batch_size = anchor_emb.size(0)
    # Concatenate: [anchor_0..N, positive_0..N] → (2N, D)
    all_emb = torch.cat([anchor_emb, positive_emb], dim=0)  # (2N, D)

    # Cosine similarity matrix: (2N, 2N)
    sim = torch.mm(all_emb, all_emb.t()) / temperature

    # Mask self-similarity (diagonal)
    mask = torch.eye(2 * batch_size, device=sim.device).bool()
    sim.masked_fill_(mask, float("-inf"))

    # Labels: anchor i's positive is at index i + batch_size
    labels = torch.arange(batch_size, device=sim.device) + batch_size

    # Cross-entropy over the top half (anchor rows)
    loss = F.cross_entropy(sim[:batch_size], labels)
    return loss


# ---------------------------------------------------------------------------
# Dataset
# ---------------------------------------------------------------------------

class ContrastiveDataset(Dataset):
    """
    Dataset of (anchor_input_ids, positive_input_ids) pairs.

    Loads the full training JSONL, then uses the pairs index to serve pairs.
    """

    def __init__(self, dataset_path: Path, pairs_path: Path,
                 max_seq_len: int = 512):
        with open(pairs_path) as f:
            pairs_data = json.load(f)

        self.pairs = pairs_data["pairs"]
        self.max_seq_len = max_seq_len

        # Load all declarations
        self.rows = []
        with open(dataset_path) as f:
            for line in f:
                line = line.strip()
                if line:
                    self.rows.append(json.loads(line))

        n = len(self.rows)
        for a, b in self.pairs:
            assert 0 <= a < n, f"anchor index {a} out of range (n={n})"
            assert 0 <= b < n, f"positive index {b} out of range (n={n})"

    def __len__(self):
        return len(self.pairs)

    def __getitem__(self, idx):
        a_idx, b_idx = self.pairs[idx]
        anchor_ids = self.rows[a_idx]["input_ids"][:self.max_seq_len]
        positive_ids = self.rows[b_idx]["input_ids"][:self.max_seq_len]
        return anchor_ids, positive_ids


def contrastive_collate(batch):
    """
    Collate variable-length sequences by padding to the max length in the batch.
    Returns (anchor_ids, anchor_mask, positive_ids, positive_mask).
    """
    anchor_seqs, positive_seqs = zip(*batch)

    def pad(seqs):
        max_len = max(len(s) for s in seqs)
        ids = torch.zeros(len(seqs), max_len, dtype=torch.long)
        mask = torch.zeros(len(seqs), max_len, dtype=torch.long)
        for i, s in enumerate(seqs):
            ids[i, :len(s)] = torch.tensor(s, dtype=torch.long)
            mask[i, :len(s)] = 1
        return ids, mask

    a_ids, a_mask = pad(anchor_seqs)
    p_ids, p_mask = pad(positive_seqs)
    return a_ids, a_mask, p_ids, p_mask


# ---------------------------------------------------------------------------
# Embedding extraction
# ---------------------------------------------------------------------------

def mean_pool(hidden_states: "torch.Tensor",
              attention_mask: "torch.Tensor") -> "torch.Tensor":
    """Mean pool hidden states over non-padding tokens, then L2-normalize."""
    mask = attention_mask.unsqueeze(-1).float()
    summed = (hidden_states * mask).sum(dim=1)
    counts = mask.sum(dim=1).clamp(min=1)
    pooled = summed / counts
    return F.normalize(pooled, p=2, dim=-1)


# ---------------------------------------------------------------------------
# Pre-flight gate
# ---------------------------------------------------------------------------

def preflight_gate(pairs_path: Path, dataset_path: Path,
                   checkpoint_path: Path) -> list[str]:
    """
    Run pre-flight checks. Returns list of error strings (empty = all pass).
    Mirrors the gate design in PIPELINE_QUALITY_GATES.md Station 4/5.
    """
    errors = []

    # G-H5-1: pairs file exists and is valid
    if not pairs_path.exists():
        errors.append(f"G-H5-1 FAIL: pairs file not found: {pairs_path}")
    else:
        try:
            with open(pairs_path) as f:
                pairs_data = json.load(f)
            n_pairs = pairs_data.get("n_pairs", 0)
            if n_pairs < 100:
                errors.append(f"G-H5-1 FAIL: only {n_pairs} pairs — need >= 100")
            else:
                print(f"  PASS  G-H5-1 pairs file: {n_pairs} pairs")
        except Exception as e:
            errors.append(f"G-H5-1 FAIL: could not read pairs file: {e}")

    # G-H5-2: dataset exists
    if not dataset_path.exists():
        errors.append(f"G-H5-2 FAIL: dataset not found: {dataset_path}")
    else:
        print(f"  PASS  G-H5-2 dataset exists: {dataset_path}")

    # G-H5-3: checkpoint exists and has config
    config_path = checkpoint_path / "config.json"
    if not checkpoint_path.exists():
        errors.append(f"G-H5-3 FAIL: checkpoint not found: {checkpoint_path}")
    elif not config_path.exists():
        errors.append(f"G-H5-3 FAIL: checkpoint missing config.json: {checkpoint_path}")
    else:
        print(f"  PASS  G-H5-3 checkpoint exists: {checkpoint_path}")

    # G-H5-4: pairs dataset file matches the training dataset
    if pairs_path.exists() and dataset_path.exists():
        try:
            with open(pairs_path) as f:
                pairs_meta = json.load(f)
            recorded = Path(pairs_meta.get("dataset_file", "")).name
            actual = dataset_path.name
            if recorded and recorded != actual:
                errors.append(
                    f"G-H5-4 FAIL: pairs built from '{recorded}' "
                    f"but training uses '{actual}' — rebuild pairs"
                )
            else:
                print(f"  PASS  G-H5-4 dataset provenance matches")
        except Exception:
            pass  # already caught above

    return errors


# ---------------------------------------------------------------------------
# Training loop
# ---------------------------------------------------------------------------

def train(
    pairs_path: Path,
    dataset_path: Path,
    checkpoint_path: Path,
    out_dir: Path,
    epochs: int = 3,
    batch_size: int = 32,
    temperature: float = 0.07,
    lr: float = 2e-5,
    max_seq_len: int = 512,
    seed: int = 42,
    smoke_test: bool = False,
):
    hf_set_seed(seed)
    random.seed(seed)
    torch.manual_seed(seed)

    device = (
        torch.device("mps") if torch.backends.mps.is_available()
        else torch.device("cuda") if torch.cuda.is_available()
        else torch.device("cpu")
    )
    print(f"Device: {device}")

    # Load model (encoder only — we use hidden states, not logits)
    print(f"Loading model from {checkpoint_path}...")
    model = AutoModel.from_pretrained(str(checkpoint_path))
    model = model.to(device)
    model.train()

    # Dataset + DataLoader
    dataset = ContrastiveDataset(dataset_path, pairs_path, max_seq_len)
    if smoke_test:
        from torch.utils.data import Subset
        dataset = Subset(dataset, range(min(64, len(dataset))))

    loader = DataLoader(
        dataset,
        batch_size=batch_size,
        shuffle=True,
        collate_fn=contrastive_collate,
        drop_last=True,  # NT-Xent requires consistent batch sizes
    )

    optimizer = torch.optim.AdamW(model.parameters(), lr=lr, weight_decay=0.01)
    total_steps = len(loader) * epochs
    scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=total_steps)

    # TensorBoard
    tb_dir = out_dir / "tb"
    writer = None
    if _tb_available:
        tb_dir.mkdir(parents=True, exist_ok=True)
        writer = SummaryWriter(log_dir=str(tb_dir))

    loss_curve = []
    global_step = 0
    t0 = time.time()

    print(f"Training: {len(dataset)} pairs, {len(loader)} steps/epoch, {epochs} epochs")
    print(f"Batch size: {batch_size}, LR: {lr}, Temperature: {temperature}")

    for epoch in range(1, epochs + 1):
        epoch_loss = 0.0
        n_batches = 0

        for a_ids, a_mask, p_ids, p_mask in loader:
            a_ids, a_mask = a_ids.to(device), a_mask.to(device)
            p_ids, p_mask = p_ids.to(device), p_mask.to(device)

            # Forward pass
            a_out = model(input_ids=a_ids, attention_mask=a_mask)
            p_out = model(input_ids=p_ids, attention_mask=p_mask)

            a_emb = mean_pool(a_out.last_hidden_state, a_mask)
            p_emb = mean_pool(p_out.last_hidden_state, p_mask)

            loss = nt_xent_loss(a_emb, p_emb, temperature)

            if torch.isnan(loss) or torch.isinf(loss):
                print(f"WARNING: loss is {loss.item()} at step {global_step} — skipping")
                optimizer.zero_grad()
                continue

            optimizer.zero_grad()
            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            optimizer.step()
            scheduler.step()

            loss_val = loss.item()
            epoch_loss += loss_val
            n_batches += 1
            global_step += 1

            loss_curve.append({"step": global_step, "loss": loss_val})

            if writer:
                writer.add_scalar("train/nt_xent_loss", loss_val, global_step)
                writer.add_scalar("train/lr", scheduler.get_last_lr()[0], global_step)

            if global_step % 50 == 0:
                elapsed = time.time() - t0
                print(f"  step {global_step}/{total_steps} "
                      f"loss={loss_val:.4f} elapsed={elapsed:.0f}s")

        avg_loss = epoch_loss / max(n_batches, 1)
        print(f"Epoch {epoch}/{epochs} — avg loss: {avg_loss:.4f}")

    if writer:
        writer.close()

    # Save checkpoint
    ckpt_out = out_dir / "checkpoint-final"
    ckpt_out.mkdir(parents=True, exist_ok=True)
    model.save_pretrained(str(ckpt_out))
    print(f"Checkpoint saved: {ckpt_out}")

    # Save loss curve
    loss_curve_path = out_dir / "loss_curve.json"
    with open(loss_curve_path, "w") as f:
        json.dump({"steps": loss_curve}, f)

    # Build results.json — written last so partial runs leave no results.json
    final_loss = loss_curve[-1]["loss"] if loss_curve else None
    initial_loss = loss_curve[0]["loss"] if loss_curve else None
    results = {
        "hypothesis": "H5",
        "objective": "nt_xent_contrastive",
        "base_checkpoint": str(checkpoint_path),
        "dataset": str(dataset_path),
        "pairs_file": str(pairs_path),
        "n_pairs": len(dataset) if not smoke_test else 64,
        "epochs": epochs,
        "batch_size": batch_size,
        "temperature": temperature,
        "learning_rate": lr,
        "max_seq_len": max_seq_len,
        "seed": seed,
        "total_steps": global_step,
        "initial_loss": initial_loss,
        "final_loss": final_loss,
        "loss_descended": (
            (initial_loss - final_loss) / initial_loss > 0.05
            if initial_loss and final_loss else None
        ),
        "smoke_test": smoke_test,
        "device": str(device),
        "duration_s": round(time.time() - t0, 1),
    }

    results_path = out_dir / "results.json"
    with open(results_path, "w") as f:
        json.dump(results, f, indent=2)
    print(f"Results saved: {results_path}")

    return results


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="H5 contrastive training")
    parser.add_argument("--pairs", default="datasets/h5_pairs_train.json")
    parser.add_argument("--dataset", default="datasets/train_A.jsonl")
    parser.add_argument("--checkpoint",
                        default="runs/variant_A_v3_2ep/checkpoint-final")
    parser.add_argument("--out", default="runs/variant_A_h5")
    parser.add_argument("--epochs", type=int, default=3)
    parser.add_argument("--batch-size", type=int, default=32)
    parser.add_argument("--temperature", type=float, default=0.07)
    parser.add_argument("--lr", type=float, default=2e-5)
    parser.add_argument("--max-seq-len", type=int, default=512)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--smoke-test", action="store_true",
                        help="Quick run with 64 pairs, no GPU needed")
    parser.add_argument("--skip-gate", action="store_true",
                        help="Skip pre-flight gate (not recommended)")
    args = parser.parse_args()

    pairs_path = Path(args.pairs)
    dataset_path = Path(args.dataset)
    checkpoint_path = Path(args.checkpoint)
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    # Pre-flight gate
    if not args.skip_gate:
        print("\nPre-flight gate (H5):")
        errors = preflight_gate(pairs_path, dataset_path, checkpoint_path)
        if errors:
            print("\nGATE FAILED — fix the following before running H5:")
            for e in errors:
                print(f"  {e}")
            sys.exit(1)
        print("  All gates pass — proceeding to training.\n")

    results = train(
        pairs_path=pairs_path,
        dataset_path=dataset_path,
        checkpoint_path=checkpoint_path,
        out_dir=out_dir,
        epochs=args.epochs,
        batch_size=args.batch_size,
        temperature=args.temperature,
        lr=args.lr,
        max_seq_len=args.max_seq_len,
        seed=args.seed,
        smoke_test=args.smoke_test,
    )

    print(f"\nH5 training complete.")
    print(f"  Final loss:     {results['final_loss']:.4f}" if results.get("final_loss") else "")
    print(f"  Loss descended: {results['loss_descended']}")
    print(f"  Duration:       {results['duration_s']}s")
    print(f"\nNext step: extract embeddings and run retrieval eval:")
    print(f"  manage.py run-alias extract-h5")
    print(f"  manage.py run-alias eval-h5")


if __name__ == "__main__":
    main()
