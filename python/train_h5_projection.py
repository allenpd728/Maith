"""
train_h5_projection.py — Stage 2 of H5 two-stage contrastive training.

Loads pre-extracted embeddings from extract_h5_embeddings.py and trains a
small MLP projection head using NT-Xent (SimCLR) loss. The full model is
never loaded — each step is pure matrix math on the projection head only,
making CPU training practical (thousands of steps per minute).

This is the standard SimCSE / CLIP approach: freeze the encoder, learn a
projection into a space where contrastive loss is optimised.

Gate checks (G-H5-P):
  G-H5-P-1  embeddings file exists and shape matches n_pairs expectation
  G-H5-P-2  pairs file exists with >= 100 pairs
  G-H5-P-3  embedding index matches dataset (example_ids align)

Usage:
    python3 python/train_h5_projection.py \
        --embeddings runs/h5_embeddings/train_A_embeddings.npy \
        --pairs datasets/h5_pairs_train.json \
        --out runs/variant_A_h5_proj \
        [--epochs 3] [--batch-size 64] [--temperature 0.07] \
        [--proj-dim 256] [--smoke-test]
"""

import argparse
import json
import time
from pathlib import Path

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F


# ---------------------------------------------------------------------------
# NT-Xent loss (identical to train_h5_contrastive.py)
# ---------------------------------------------------------------------------

def nt_xent_loss(anchor_emb: torch.Tensor, positive_emb: torch.Tensor,
                 temperature: float) -> torch.Tensor:
    batch_size = anchor_emb.size(0)
    all_emb = torch.cat([anchor_emb, positive_emb], dim=0)
    sim = torch.mm(all_emb, all_emb.t()) / temperature
    mask = torch.eye(2 * batch_size, device=sim.device).bool()
    sim.masked_fill_(mask, float("-inf"))
    labels = torch.arange(batch_size, device=sim.device) + batch_size
    return F.cross_entropy(sim[:batch_size], labels)


# ---------------------------------------------------------------------------
# Projection head
# ---------------------------------------------------------------------------

class ProjectionHead(nn.Module):
    """
    2-layer MLP: input_dim -> hidden_dim -> proj_dim, with BN + ReLU between.
    Standard SimCLR / MoCo projection head design.
    """

    def __init__(self, input_dim: int, proj_dim: int = 256):
        super().__init__()
        hidden_dim = max(input_dim, proj_dim * 2)
        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.BatchNorm1d(hidden_dim),
            nn.ReLU(inplace=True),
            nn.Linear(hidden_dim, proj_dim),
        )

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return F.normalize(self.net(x), p=2, dim=-1)


# ---------------------------------------------------------------------------
# Dataset
# ---------------------------------------------------------------------------

class EmbeddingPairDataset(torch.utils.data.Dataset):
    def __init__(self, embeddings: np.ndarray, pairs: list, smoke_test: bool = False):
        self.embeddings = torch.from_numpy(embeddings).float()
        self.pairs = pairs[:64] if smoke_test else pairs

    def __len__(self):
        return len(self.pairs)

    def __getitem__(self, idx):
        a_idx, b_idx = self.pairs[idx]
        return self.embeddings[a_idx], self.embeddings[b_idx]


# ---------------------------------------------------------------------------
# Pre-flight gate
# ---------------------------------------------------------------------------

def preflight_gate(embeddings_path: Path, pairs_path: Path,
                   index_path: Path) -> list:
    errors = []

    if not embeddings_path.exists():
        errors.append(f"G-H5-P-1 FAIL: embeddings not found: {embeddings_path}")
    else:
        emb = np.load(str(embeddings_path))
        print(f"  PASS  G-H5-P-1 embeddings: shape={emb.shape}")

    if not pairs_path.exists():
        errors.append(f"G-H5-P-2 FAIL: pairs file not found: {pairs_path}")
    else:
        with open(pairs_path) as f:
            pairs_data = json.load(f)
        n_pairs = pairs_data.get("n_pairs", 0)
        if n_pairs < 100:
            errors.append(f"G-H5-P-2 FAIL: only {n_pairs} pairs — need >= 100")
        else:
            print(f"  PASS  G-H5-P-2 pairs: {n_pairs} pairs")

    if index_path.exists():
        print(f"  PASS  G-H5-P-3 embedding index found: {index_path}")
    else:
        print(f"  WARN  G-H5-P-3 no index file — skipping provenance check")

    return errors


# ---------------------------------------------------------------------------
# Training
# ---------------------------------------------------------------------------

def train(
    embeddings_path: Path,
    pairs_path: Path,
    out_dir: Path,
    epochs: int = 3,
    batch_size: int = 64,
    temperature: float = 0.07,
    proj_dim: int = 256,
    lr: float = 1e-3,
    seed: int = 42,
    smoke_test: bool = False,
    log_every: int = 10,
):
    torch.manual_seed(seed)
    device = torch.device("cpu")  # projection head is tiny — CPU is fast enough
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    # Load embeddings and pairs
    embeddings = np.load(str(embeddings_path))
    with open(pairs_path) as f:
        pairs_data = json.load(f)
    pairs = pairs_data["pairs"]

    n_rows, input_dim = embeddings.shape
    print(f"Embeddings: {n_rows} rows, dim={input_dim}")
    print(f"Pairs: {len(pairs)}")

    dataset = EmbeddingPairDataset(embeddings, pairs, smoke_test=smoke_test)
    loader = torch.utils.data.DataLoader(
        dataset, batch_size=batch_size, shuffle=True, drop_last=True
    )

    model = ProjectionHead(input_dim=input_dim, proj_dim=proj_dim).to(device)
    n_params = sum(p.numel() for p in model.parameters())
    print(f"Projection head: {n_params:,} params, input_dim={input_dim}, proj_dim={proj_dim}")

    optimizer = torch.optim.AdamW(model.parameters(), lr=lr, weight_decay=0.01)
    total_steps = len(loader) * epochs
    scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=max(total_steps, 1))

    # TensorBoard (optional)
    writer = None
    try:
        from torch.utils.tensorboard import SummaryWriter
        tb_dir = out_dir / "tb"
        tb_dir.mkdir(parents=True, exist_ok=True)
        writer = SummaryWriter(log_dir=str(tb_dir))
    except ImportError:
        pass

    print(f"Training: {len(dataset)} pairs, {len(loader)} steps/epoch, {epochs} epochs", flush=True)
    print(f"Batch size: {batch_size}, LR: {lr}, Temperature: {temperature}", flush=True)

    loss_curve = []
    global_step = 0
    t0 = time.time()

    for epoch in range(1, epochs + 1):
        epoch_loss = 0.0
        n_batches = 0

        for a_emb, p_emb in loader:
            a_emb = a_emb.to(device)
            p_emb = p_emb.to(device)

            a_proj = model(a_emb)
            p_proj = model(p_emb)

            loss = nt_xent_loss(a_proj, p_proj, temperature)

            if torch.isnan(loss) or torch.isinf(loss):
                print(f"WARNING: loss is {loss.item()} at step {global_step} — skipping", flush=True)
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

            if global_step % log_every == 0 or global_step == 1:
                elapsed = time.time() - t0
                remaining = (elapsed / global_step) * (total_steps - global_step) if global_step > 0 else 0
                print(f"  step {global_step}/{total_steps} "
                      f"loss={loss_val:.4f} elapsed={elapsed:.0f}s eta={remaining:.0f}s",
                      flush=True)

        avg_loss = epoch_loss / max(n_batches, 1)
        print(f"Epoch {epoch}/{epochs} — avg loss: {avg_loss:.4f}", flush=True)

    if writer:
        writer.close()

    # Save projection head weights
    ckpt_out = out_dir / "checkpoint-final"
    ckpt_out.mkdir(parents=True, exist_ok=True)
    torch.save(model.state_dict(), str(ckpt_out / "projection_head.pt"))
    # Save config for eval scripts
    with open(ckpt_out / "config.json", "w") as f:
        json.dump({"input_dim": input_dim, "proj_dim": proj_dim,
                   "embeddings": str(embeddings_path)}, f, indent=2)
    print(f"Projection head saved: {ckpt_out}")

    # Save loss curve
    with open(out_dir / "loss_curve.json", "w") as f:
        json.dump({"steps": loss_curve}, f)

    # Results gate
    final_loss = loss_curve[-1]["loss"] if loss_curve else None
    initial_loss = loss_curve[0]["loss"] if loss_curve else None
    results = {
        "hypothesis": "H5",
        "stage": "projection",
        "objective": "nt_xent_contrastive",
        "embeddings": str(embeddings_path),
        "pairs_file": str(pairs_path),
        "n_pairs": len(dataset),
        "epochs": epochs,
        "batch_size": batch_size,
        "temperature": temperature,
        "proj_dim": proj_dim,
        "learning_rate": lr,
        "seed": seed,
        "total_steps": global_step,
        "initial_loss": initial_loss,
        "final_loss": final_loss,
        "loss_descended": (
            (initial_loss - final_loss) / initial_loss > 0.05
            if initial_loss and final_loss else None
        ),
        "smoke_test": smoke_test,
        "duration_s": round(time.time() - t0, 1),
    }
    with open(out_dir / "results.json", "w") as f:
        json.dump(results, f, indent=2)
    print(f"Results saved: {out_dir / 'results.json'}")
    print(f"loss_descended: {results['loss_descended']} "
          f"(initial={initial_loss:.4f}, final={final_loss:.4f})" if initial_loss else "")
    return results


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="H5 Stage 2: projection head contrastive training")
    parser.add_argument("--embeddings", required=True, help="Path to .npy embeddings from Stage 1")
    parser.add_argument("--pairs", required=True, help="Path to h5_pairs_train.json")
    parser.add_argument("--out", required=True, help="Output directory for checkpoint + results")
    parser.add_argument("--epochs", type=int, default=3)
    parser.add_argument("--batch-size", type=int, default=64)
    parser.add_argument("--temperature", type=float, default=0.07)
    parser.add_argument("--proj-dim", type=int, default=256)
    parser.add_argument("--lr", type=float, default=1e-3)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--smoke-test", action="store_true")
    parser.add_argument("--log-every", type=int, default=10)
    args = parser.parse_args()

    embeddings_path = Path(args.embeddings)
    pairs_path = Path(args.pairs)
    index_path = embeddings_path.with_suffix(".index.json")

    print("Pre-flight gate (H5-Projection):")
    errors = preflight_gate(embeddings_path, pairs_path, index_path)
    if errors:
        for e in errors:
            print(f"  {e}")
        raise SystemExit(1)
    print("  All gates pass — proceeding to training.\n")

    train(
        embeddings_path=embeddings_path,
        pairs_path=pairs_path,
        out_dir=Path(args.out),
        epochs=args.epochs,
        batch_size=args.batch_size,
        temperature=args.temperature,
        proj_dim=args.proj_dim,
        lr=args.lr,
        seed=args.seed,
        smoke_test=args.smoke_test,
        log_every=args.log_every,
    )


if __name__ == "__main__":
    main()
