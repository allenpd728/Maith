"""
extract_h5_embeddings.py — Stage 1 of H5 two-stage contrastive training.

Runs one forward pass over every row in a training JSONL using the frozen
checkpoint and saves mean-pooled L2-normalised embeddings to a .npy file.

Only needs to run once per checkpoint. The output is the input to
train_h5_projection.py.

Usage:
    python3 python/extract_h5_embeddings.py \
        --dataset datasets/train_A.jsonl \
        --checkpoint runs/variant_A_v3_2ep/checkpoint-final \
        --out runs/h5_embeddings/train_A_embeddings.npy \
        [--batch-size 8] [--max-seq-len 512] [--device cpu]
"""

import argparse
import json
import time
from pathlib import Path

import numpy as np
import torch
import torch.nn.functional as F
from transformers import AutoModel


# ---------------------------------------------------------------------------
# Mean pooling (identical to train_h5_contrastive.py)
# ---------------------------------------------------------------------------

def mean_pool(hidden_states: torch.Tensor, attention_mask: torch.Tensor) -> torch.Tensor:
    mask = attention_mask.unsqueeze(-1).float()
    summed = (hidden_states * mask).sum(dim=1)
    counts = mask.sum(dim=1).clamp(min=1)
    pooled = summed / counts
    return F.normalize(pooled, p=2, dim=-1)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Extract frozen embeddings for H5 Stage 1")
    parser.add_argument("--dataset", required=True, help="Path to training JSONL")
    parser.add_argument("--checkpoint", required=True, help="Path to model checkpoint dir")
    parser.add_argument("--out", required=True, help="Output .npy path for embeddings")
    parser.add_argument("--batch-size", type=int, default=8)
    parser.add_argument("--max-seq-len", type=int, default=512)
    parser.add_argument("--device", type=str, default=None)
    args = parser.parse_args()

    dataset_path = Path(args.dataset)
    checkpoint_path = Path(args.checkpoint)
    out_path = Path(args.out)

    # Gate checks
    print("Pre-flight checks:")
    assert dataset_path.exists(), f"Dataset not found: {dataset_path}"
    assert checkpoint_path.exists(), f"Checkpoint not found: {checkpoint_path}"
    out_path.parent.mkdir(parents=True, exist_ok=True)
    print(f"  PASS  dataset: {dataset_path}")
    print(f"  PASS  checkpoint: {checkpoint_path}")
    print(f"  OUT   {out_path}")

    # Device — MPS is fine for inference (no backward pass, no OOM risk)
    if args.device:
        device = torch.device(args.device)
    elif torch.backends.mps.is_available():
        device = torch.device("mps")
        # MPS inference batch size: keep low to avoid memory pressure
        if args.batch_size > 4:
            print(f"  MPS: capping batch size at 4 (was {args.batch_size})")
            args.batch_size = 4
    else:
        device = torch.device("cpu")
    print(f"Device: {device}")

    # Load rows
    rows = []
    with open(dataset_path) as f:
        for line in f:
            line = line.strip()
            if line:
                rows.append(json.loads(line))
    n = len(rows)
    print(f"Loaded {n} rows")

    # Load model frozen (bfloat16 everywhere)
    print(f"Loading model from {checkpoint_path}...")
    model = AutoModel.from_pretrained(str(checkpoint_path), dtype=torch.bfloat16)
    model.to(device)
    model.eval()
    hidden_dim = model.config.hidden_size
    print(f"Model loaded — hidden_dim={hidden_dim}, dtype=bfloat16")

    # Extract embeddings in batches
    all_embeddings = np.zeros((n, hidden_dim), dtype=np.float32)
    t0 = time.time()

    with torch.no_grad():
        for batch_start in range(0, n, args.batch_size):
            batch_rows = rows[batch_start: batch_start + args.batch_size]
            batch_ids_raw = [r["input_ids"][: args.max_seq_len] for r in batch_rows]

            # Pad batch
            max_len = max(len(ids) for ids in batch_ids_raw)
            input_ids = torch.zeros(len(batch_rows), max_len, dtype=torch.long)
            attention_mask = torch.zeros(len(batch_rows), max_len, dtype=torch.long)
            for i, ids in enumerate(batch_ids_raw):
                input_ids[i, : len(ids)] = torch.tensor(ids, dtype=torch.long)
                attention_mask[i, : len(ids)] = 1

            input_ids = input_ids.to(device)
            attention_mask = attention_mask.to(device)

            out = model(input_ids=input_ids, attention_mask=attention_mask)
            emb = mean_pool(out.last_hidden_state, attention_mask)  # (B, D) float32 after norm

            all_embeddings[batch_start: batch_start + len(batch_rows)] = emb.float().cpu().numpy()

            if (batch_start // args.batch_size + 1) % 10 == 0 or batch_start + args.batch_size >= n:
                done = min(batch_start + args.batch_size, n)
                elapsed = time.time() - t0
                eta = (elapsed / done) * (n - done) if done > 0 else 0
                print(f"  {done}/{n} rows — elapsed={elapsed:.0f}s eta={eta:.0f}s", flush=True)

    # Save
    np.save(str(out_path), all_embeddings)
    elapsed = time.time() - t0
    print(f"Saved {all_embeddings.shape} embeddings to {out_path} ({elapsed:.0f}s)")

    # Also save an index file mapping row index → example_id for traceability
    index_path = out_path.with_suffix(".index.json")
    index = [r.get("example_id", str(i)) for i, r in enumerate(rows)]
    with open(index_path, "w") as f:
        json.dump({"n": n, "checkpoint": str(checkpoint_path),
                   "dataset": str(dataset_path), "index": index}, f)
    print(f"Index saved: {index_path}")


if __name__ == "__main__":
    main()
