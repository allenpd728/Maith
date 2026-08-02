#!/usr/bin/env python3
"""
embed_project.py

Builds an embedding projection matrix that maps Qwen2.5-Coder-0.5B's pretrained
BPE embeddings into the Maith IR vocabulary space for Variant A.

This addresses the cold-start embedding confound in DEC-006: Variant A resizes the
embedding table from 151,643 tokens to 8,144 IR tokens. Without initialization,
these new embeddings are random, giving B/C an unfair pretraining head start.

The projection strategy:
  1. For IR tokens that exist verbatim in Qwen's vocab, use their embeddings directly.
  2. For IR tokens that BPE-tokenize into multiple subwords, take the mean.
  3. For tokens with no match, fall back to the global mean of Qwen embeddings.

Usage:
    python3 python/embed_project.py \
        --vocab datasets/vocab_A.json \
        --output datasets/embed_proj_A.pt \
        --strategy mean

    python3 python/embed_project.py \
        --vocab datasets/vocab_A.json \
        --output datasets/embed_proj_A_exact.pt \
        --strategy exact
"""

import argparse
import json
import os
import sys
from typing import Optional

import torch


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def parse_args():
    parser = argparse.ArgumentParser(
        description="Build embedding projection matrix for Variant A (IR vocab)"
    )
    parser.add_argument(
        "--vocab",
        type=str,
        default="datasets/vocab_A.json",
        help="Path to IR vocab JSON (token → int ID)",
    )
    parser.add_argument(
        "--output",
        type=str,
        default="datasets/embed_proj_A.pt",
        help="Output path for the projected embedding matrix (.pt file)",
    )
    parser.add_argument(
        "--strategy",
        type=str,
        choices=["exact", "mean"],
        default="mean",
        help=(
            "Matching strategy: "
            "'exact' uses token strings that exist verbatim in Qwen vocab; "
            "'mean' BPE-tokenizes and takes mean of subword embeddings. "
            "'mean' is recommended as it handles composite tokens like HMul.hMul."
        ),
    )
    parser.add_argument(
        "--model",
        type=str,
        default="Qwen/Qwen2.5-Coder-0.5B",
        help="HuggingFace model to load embeddings from",
    )
    parser.add_argument(
        "--cache-dir",
        type=str,
        default=None,
        help="Cache directory for HuggingFace models",
    )
    return parser.parse_args()


# ---------------------------------------------------------------------------
# Embedding projection
# ---------------------------------------------------------------------------

def get_hf_token():
    """Load HF_TOKEN from environment or .env file."""
    hf_token = os.environ.get("HF_TOKEN", "")
    if not hf_token:
        env_path = os.path.join(os.path.dirname(__file__), "..", ".env")
        if os.path.exists(env_path):
            with open(env_path) as f:
                for line in f:
                    if line.strip().startswith("HF_TOKEN="):
                        hf_token = line.strip().split("=", 1)[1].strip()
                        break
    return hf_token if hf_token else None


def load_qwen_embeddings(model_name: str, cache_dir: Optional[str] = None):
    """Load Qwen2.5-Coder-0.5B and extract its embedding matrix."""
    from transformers import AutoModelForCausalLM, AutoTokenizer

    hf_token = get_hf_token()

    print(f"Loading model: {model_name} ...")
    tokenizer = AutoTokenizer.from_pretrained(
        model_name,
        trust_remote_code=True,
        cache_dir=cache_dir,
        token=hf_token,
    )
    model = AutoModelForCausalLM.from_pretrained(
        model_name,
        trust_remote_code=True,
        cache_dir=cache_dir,
        token=hf_token,
    )

    # Extract embedding matrix (same as how train.py accesses it)
    embed_matrix = model.model.embed_tokens.weight.detach().cpu()
    hidden_dim = embed_matrix.shape[1]
    vocab_size = embed_matrix.shape[0]

    print(f"  Qwen vocab size: {vocab_size:,}")
    print(f"  Hidden dim: {hidden_dim}")

    # Build token → ID map for Qwen tokenizer
    qwen_vocab = tokenizer.get_vocab()

    return {
        "embed_matrix": embed_matrix,
        "vocab": qwen_vocab,
        "tokenizer": tokenizer,
        "hidden_dim": hidden_dim,
        "vocab_size": vocab_size,
    }


def build_projection(
    ir_vocab: dict[str, int],
    qwen_data: dict,
    strategy: str = "mean",
) -> tuple[torch.Tensor, dict]:
    """
    Build projected embedding matrix for the IR vocab.

    Args:
        ir_vocab: dict mapping IR token string → int ID
        qwen_data: output of load_qwen_embeddings
        strategy: 'exact' or 'mean'

    Returns:
        E_proj: tensor of shape (len(ir_vocab), hidden_dim)
        stats: dict with matching statistics
    """
    embed_matrix = qwen_data["embed_matrix"]
    qwen_vocab = qwen_data["vocab"]
    tokenizer = qwen_data["tokenizer"]
    hidden_dim = qwen_data["hidden_dim"]

    vocab_size = len(ir_vocab)
    E_proj = torch.zeros(vocab_size, hidden_dim)

    stats = {
        "exact_matches": 0,
        "mean_subword": 0,
        "global_mean_fallback": 0,
    }

    # Global mean fallback for tokens with no match
    global_mean = embed_matrix.mean(dim=0)

    for token, token_id in ir_vocab.items():
        if strategy == "exact":
            if token in qwen_vocab:
                qwen_id = qwen_vocab[token]
                E_proj[token_id] = embed_matrix[qwen_id]
                stats["exact_matches"] += 1
            else:
                # Try BPE tokenization as fallback for 'exact' too
                # (some tokens might BPE-match even if not exact)
                sub_ids = tokenizer.encode(token, add_special_tokens=False)
                if sub_ids and all(sid < len(embed_matrix) for sid in sub_ids):
                    sub_embeds = embed_matrix[sub_ids]
                    E_proj[token_id] = sub_embeds.mean(dim=0)
                    stats["mean_subword"] += 1
                else:
                    E_proj[token_id] = global_mean
                    stats["global_mean_fallback"] += 1
        else:  # strategy == "mean"
            # BPE-tokenize the IR token and take mean of subword embeddings
            sub_ids = tokenizer.encode(token, add_special_tokens=False)

            if sub_ids and all(sid < len(embed_matrix) for sid in sub_ids):
                sub_embeds = embed_matrix[sub_ids]
                E_proj[token_id] = sub_embeds.mean(dim=0)
                stats["mean_subword"] += 1
            else:
                # No valid subword match → global mean fallback
                E_proj[token_id] = global_mean
                stats["global_mean_fallback"] += 1

    return E_proj, stats


def save_projection(E_proj: torch.Tensor, output_path: str) -> None:
    """Save the projection matrix to disk."""
    os.makedirs(os.path.dirname(output_path) or ".", exist_ok=True)
    torch.save(E_proj, output_path)
    print(f"  Saved projection matrix to {output_path}")


def print_summary(
    E_proj: torch.Tensor,
    stats: dict,
    ir_vocab_size: int,
) -> None:
    """Print a summary of the projection results."""
    print()
    print("=== Projection Summary ===")
    print(f"  IR vocab size: {ir_vocab_size:,}")
    print(f"  Exact matches: {stats['exact_matches']:,} ({100*stats['exact_matches']/ir_vocab_size:.1f}%)")
    print(f"  Mean subword:  {stats['mean_subword']:,} ({100*stats['mean_subword']/ir_vocab_size:.1f}%)")
    print(f"  Global mean fallback: {stats['global_mean_fallback']:,} ({100*stats['global_mean_fallback']/ir_vocab_size:.1f}%)")

    norm = E_proj.norm().item()
    print(f"  L2 norm of projection matrix: {norm:.4f}")
    print(f"  Mean embedding norm: {E_proj.norm(dim=1).mean().item():.4f}")

    if norm < 1e-6:
        print("  WARNING: Matrix norm is near zero — check for initialization errors.")
    print()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    args = parse_args()

    # Load IR vocab
    print(f"Loading IR vocab from {args.vocab} ...")
    if not os.path.exists(args.vocab):
        print(f"ERROR: IR vocab not found at {args.vocab}")
        sys.exit(1)

    with open(args.vocab) as f:
        ir_vocab = json.load(f)

    print(f"  IR vocab size: {len(ir_vocab):,}")

    # Load Qwen embeddings
    qwen_data = load_qwen_embeddings(args.model, cache_dir=args.cache_dir)

    # Build projection
    print(f"Building projection matrix (strategy={args.strategy}) ...")
    E_proj, stats = build_projection(ir_vocab, qwen_data, strategy=args.strategy)

    # Save
    save_projection(E_proj, args.output)

    # Print summary
    print_summary(E_proj, stats, len(ir_vocab))

    print("Done.")


if __name__ == "__main__":
    main()
