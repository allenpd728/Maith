#!/usr/bin/env python3
"""
extract_h5_proj_embeddings.py

Apply the trained H5 projection head to existing retrieval embeddings
(A_v3_2ep eval/train splits) and save in the format expected by retrieval_eval.py.

Output variant name: A_h5_proj
Output format: embeddings_A_h5_proj_{eval,train}.pt + names_A_h5_proj_{eval,train}.json

Usage:
    python3 python/extract_h5_proj_embeddings.py \\
        --proj-checkpoint runs/variant_A_h5_proj/checkpoint-final \\
        --embeddings-dir runs/retrieval_embeddings_clean \\
        --source-variant A_v3_2ep \\
        --out runs/retrieval_embeddings_h5

Pre-flight gates:
  G-H5-E-1  proj checkpoint exists (projection_head.pt + config.json)
  G-H5-E-2  source embeddings exist for both eval and train splits
  G-H5-E-3  output dir is writable
"""

import argparse
import json
import sys
import time
from pathlib import Path

import torch
import torch.nn as nn
import torch.nn.functional as F


# ---------------------------------------------------------------------------
# Projection head (must match train_h5_projection.py exactly)
# ---------------------------------------------------------------------------

class ProjectionHead(nn.Module):
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
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Extract H5 projection embeddings for retrieval eval")
    parser.add_argument("--proj-checkpoint", required=True, help="Path to projection head checkpoint dir")
    parser.add_argument("--embeddings-dir", required=True, help="Dir containing source retrieval embeddings (.pt)")
    parser.add_argument("--source-variant", default="A_v3_2ep", help="Source variant name (default: A_v3_2ep)")
    parser.add_argument("--out", required=True, help="Output dir for projected embeddings")
    parser.add_argument("--out-variant", default="A_h5_proj", help="Output variant name (default: A_h5_proj)")
    args = parser.parse_args()

    ckpt_dir = Path(args.proj_checkpoint)
    emb_dir = Path(args.embeddings_dir)
    out_dir = Path(args.out)
    src = args.source_variant
    out_v = args.out_variant

    # --- Pre-flight gates ---
    print("Pre-flight gate (H5-ProjExtract):")
    errors = []

    proj_pt = ckpt_dir / "projection_head.pt"
    cfg_json = ckpt_dir / "config.json"
    if not proj_pt.exists() or not cfg_json.exists():
        errors.append(f"  FAIL  G-H5-E-1 proj checkpoint missing: {ckpt_dir}")
    else:
        print(f"  PASS  G-H5-E-1 proj checkpoint: {proj_pt}")

    splits = ["eval", "train"]
    for split in splits:
        ep = emb_dir / f"embeddings_{src}_{split}.pt"
        np_ = emb_dir / f"names_{src}_{split}.json"
        if not ep.exists() or not np_.exists():
            errors.append(f"  FAIL  G-H5-E-2 missing {ep} or {np_}")
        else:
            print(f"  PASS  G-H5-E-2 source embeddings ({split}): {ep}")

    out_dir.mkdir(parents=True, exist_ok=True)
    test_file = out_dir / ".write_test"
    try:
        test_file.touch()
        test_file.unlink()
        print(f"  PASS  G-H5-E-3 output dir writable: {out_dir}")
    except Exception as e:
        errors.append(f"  FAIL  G-H5-E-3 output dir not writable: {e}")

    if errors:
        for e in errors:
            print(e)
        sys.exit(1)
    print("  All gates pass — proceeding.\n")

    # --- Load projection head ---
    with open(cfg_json) as f:
        cfg = json.load(f)
    input_dim = cfg["input_dim"]
    proj_dim = cfg["proj_dim"]

    model = ProjectionHead(input_dim=input_dim, proj_dim=proj_dim)
    model.load_state_dict(torch.load(proj_pt, map_location="cpu", weights_only=True))
    model.eval()
    print(f"Projection head loaded: input_dim={input_dim}, proj_dim={proj_dim}")

    # --- Process each split ---
    for split in splits:
        t0 = time.time()
        src_emb_path = emb_dir / f"embeddings_{src}_{split}.pt"
        src_names_path = emb_dir / f"names_{src}_{split}.json"

        src_emb = torch.load(src_emb_path, map_location="cpu", weights_only=True).float()
        with open(src_names_path) as f:
            names = json.load(f)

        print(f"Processing {split}: {src_emb.shape[0]} embeddings, dim={src_emb.shape[1]}")

        # Apply projection head in one shot (CPU, small enough)
        with torch.no_grad():
            proj_emb = model(src_emb)  # (N, proj_dim), L2-normalised

        out_emb_path = out_dir / f"embeddings_{out_v}_{split}.pt"
        out_names_path = out_dir / f"names_{out_v}_{split}.json"

        torch.save(proj_emb, str(out_emb_path))
        with open(out_names_path, "w") as f:
            json.dump(names, f)

        elapsed = time.time() - t0
        print(f"  Saved {out_emb_path} ({proj_emb.shape}, {elapsed:.1f}s)")

    print(f"\nDone. Output variant '{out_v}' written to {out_dir}")
    print(f"\nNext step — run retrieval eval:")
    print(f"  python3 python/retrieval_eval.py \\")
    print(f"      --embeddings-dir {out_dir} \\")
    print(f"      --groundtruth datasets/dependency_groundtruth_eval_to_train.json \\")
    print(f"      --variants {out_v},A_v3_2ep,B_small_clean \\")
    print(f"      --mode eval_to_train \\")
    print(f"      --out runs/h5_retrieval/results.json")


if __name__ == "__main__":
    main()
