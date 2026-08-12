#!/usr/bin/env python3
"""
extract_retrieval_embeddings.py

Extract per-declaration embeddings by feeding each variant's ACTUAL training input
through its trained checkpoint, mean-pooling the final hidden layer.

This fixes the bug in extract_representations.py (which feeds IR tokens to ALL
variants, including B/C that were trained on leanExpr/AST text — see
KNOWN_ISSUES.md issue 4 and H6_RETRIEVAL_SCOPE.md §4.1).

Each variant's embeddings come from the input_ids field of its own dataset file
(datasets/{train,eval}_{variant}.jsonl), which contains the exact integer token
sequences each model was trained on. No re-tokenization, no string round-trip.

Usage:
    python3 python/extract_retrieval_embeddings.py \\
        --variants A,B_small,B,C,flat,random \\
        --out runs/retrieval_embeddings

    # Single variant:
    python3 python/extract_retrieval_embeddings.py --variants A --out runs/retrieval_embeddings
"""

import argparse
import gc
import json
import os
import sys
from pathlib import Path

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

HIDDEN_SIZE = 896
MAX_SEQ_LEN = 512
EXTRACTION_SEED = 42

_REPO_ROOT = Path(__file__).resolve().parent.parent
_RUNS_DIR = Path(os.environ.get(
    "MAITH_RUNS_DIR",
    str(_REPO_ROOT / "runs"),
))
_DATASETS_DIR = Path(os.environ.get(
    "MAITH_DATASETS_DIR",
    str(_REPO_ROOT / "datasets"),
))

CHECKPOINTS = {
    "A":       _RUNS_DIR / "variant_A_v2_full" / "checkpoint-final",
    "B_small": _RUNS_DIR / "variant_B_small" / "checkpoint-final",
    "B":       _RUNS_DIR / "variant_B_phase6" / "checkpoint-final",
    "C":       _RUNS_DIR / "variant_C_v2" / "checkpoint-final",
    "flat":    _RUNS_DIR / "variant_flat" / "checkpoint-final",
    "random":  None,  # Loaded from HuggingFace cache, no fine-tuning
}

BASE_MODEL = "Qwen/Qwen2.5-Coder-0.5B"

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def get_device() -> str:
    if torch.cuda.is_available():
        return "cuda"
    elif torch.backends.mps.is_available():
        return "mps"
    else:
        return "cpu"


def load_dataset_split(variant: str, split: str) -> list[dict]:
    """Load a dataset split (train or eval) for a variant."""
    path = Path(_DATASETS_DIR) / f"{split}_{variant}.jsonl"
    if not path.exists():
        print(f"WARNING: {path} not found, skipping {variant}/{split}")
        return []
    rows = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                rows.append(json.loads(line))
    return rows


def patch_tokenizer_config(ckpt_path: str) -> None:
    """Fix extra_special_tokens field if it is a list (saved by older transformers)."""
    cfg_path = os.path.join(ckpt_path, "tokenizer_config.json")
    if not os.path.exists(cfg_path):
        return
    with open(cfg_path) as f:
        cfg = json.load(f)
    if isinstance(cfg.get("extra_special_tokens"), list):
        cfg["extra_special_tokens"] = {}
        with open(cfg_path, "w") as f:
            json.dump(cfg, f, indent=2)
        print(f"  Patched extra_special_tokens list→dict in {cfg_path}")


def extract_embeddings_for_variant(
    variant: str,
    train_rows: list[dict],
    eval_rows: list[dict],
    device: str,
    out_dir: Path,
) -> None:
    """Extract embeddings for a single variant from its actual training inputs."""
    torch.manual_seed(EXTRACTION_SEED)

    # Load model + tokenizer
    if variant == "random":
        print(f"Loading pretrained {BASE_MODEL} (no fine-tuning)...")
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL, dtype=torch.float32, trust_remote_code=True
        ).to(device)
        tokenizer = AutoTokenizer.from_pretrained(
            BASE_MODEL, trust_remote_code=True
        )
    else:
        ckpt_path = CHECKPOINTS[variant]
        if not ckpt_path.exists():
            print(f"WARNING: checkpoint {ckpt_path} not found, skipping {variant}")
            return
        print(f"Loading checkpoint: {ckpt_path}...")
        patch_tokenizer_config(str(ckpt_path))
        model = AutoModelForCausalLM.from_pretrained(
            str(ckpt_path), dtype=torch.float32, trust_remote_code=True
        ).to(device)
        tokenizer = AutoTokenizer.from_pretrained(
            str(ckpt_path), trust_remote_code=True
        )

    model.eval()
    for param in model.parameters():
        param.requires_grad = False

    for split_name, rows in [("train", train_rows), ("eval", eval_rows)]:
        if not rows:
            continue

        all_embeddings = []
        all_names = []

        print(f"  Extracting {split_name} embeddings for {len(rows)} examples...")

        for i, row in enumerate(rows):
            if (i + 1) % 500 == 0 or i == 0:
                print(f"    {split_name} {i + 1}/{len(rows)}...")

            # THE FIX: use input_ids directly from the dataset, not IR tokens
            input_ids = row.get("input_ids", [])
            if not input_ids:
                print(f"    WARNING: empty input_ids for {row.get('name', '?')}, skipping")
                all_embeddings.append(torch.zeros(1, HIDDEN_SIZE))
                all_names.append(row.get("name", ""))
                continue

            # Truncate to max length
            input_ids = input_ids[:MAX_SEQ_LEN]
            ids_tensor = torch.tensor([input_ids], dtype=torch.long).to(device)

            # Build attention mask (all 1s for the actual tokens)
            attn_mask = torch.ones_like(ids_tensor)

            with torch.no_grad():
                outputs = model(
                    input_ids=ids_tensor,
                    attention_mask=attn_mask,
                    output_hidden_states=True,
                )
                # Final hidden layer: [1, seq_len, hidden_size]
                final_hidden = outputs.hidden_states[-1]

                # Mean-pool over non-padding positions
                # (all positions are real tokens since we truncated, not padded)
                pooled = final_hidden.mean(dim=1)  # [1, hidden_size]

            all_embeddings.append(pooled.cpu())
            all_names.append(row.get("name", ""))

        # Stack and save
        embeddings = torch.cat(all_embeddings, dim=0)  # [N, hidden_size]
        out_path = out_dir / f"embeddings_{variant}_{split_name}.pt"
        torch.save(embeddings, out_path)
        print(f"  Saved {embeddings.shape} → {out_path}")

        # Save names (ordering key for joining to ground truth)
        names_path = out_dir / f"names_{variant}_{split_name}.json"
        with open(names_path, "w") as f:
            json.dump(all_names, f)

    # Free memory
    del model, tokenizer
    gc.collect()
    if device == "mps":
        torch.mps.empty_cache()
    elif device == "cuda":
        torch.cuda.empty_cache()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Extract per-variant retrieval embeddings from actual training inputs."
    )
    parser.add_argument(
        "--variants", default="A,B_small,B,C,flat,random",
        help="Comma-separated variant names"
    )
    parser.add_argument(
        "--out", default=str(_RUNS_DIR / "retrieval_embeddings"),
        help="Output directory"
    )
    parser.add_argument(
        "--skip-invariant-check", action="store_true",
        help="Skip the invariant checker precondition (NOT RECOMMENDED)"
    )
    args = parser.parse_args()

    # ENFORCED PRECONDITION: invariant checker must pass before extraction
    if not args.skip_invariant_check:
        import subprocess as _sp
        print("=" * 60)
        print("PRECONDITION: INVARIANT CHECKER")
        print("=" * 60)
        _r = _sp.run([sys.executable, str(_REPO_ROOT / "python" / "check_invariants.py"),
                       "--datasets", str(_DATASETS_DIR), "--runs", str(_RUNS_DIR)])
        if _r.returncode != 0:
            print("INVARIANT CHECK FAILED — extraction blocked.")
            print("Fix the invariant violations before proceeding, or use --skip-invariant-check")
            sys.exit(1)
        print()

    variants = [v.strip() for v in args.variants.split(",")]
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    device = get_device()
    print(f"Device: {device}")
    print(f"Output: {out_dir}")
    print()

    for variant in variants:
        print(f"{'='*60}")
        print(f"VARIANT: {variant}")
        print(f"{'='*60}")

        # Load the variant's actual datasets
        train_rows = load_dataset_split(variant, "train")
        eval_rows = load_dataset_split(variant, "eval")

        if not train_rows and not eval_rows:
            print(f"No data for variant {variant}, skipping.")
            continue

        # Verify the source field matches the variant (invariant 3)
        expected_source_map = {
            "A": "A", "B_small": "B_small", "B": "B",
            "C": "C", "flat": "flat_ir", "random": "random",
        }
        expected_source = expected_source_map.get(variant, variant)
        for split_name, rows in [("train", train_rows), ("eval", eval_rows)]:
            if not rows:
                continue
            mismatches = [r for r in rows if r.get("source") != expected_source]
            if mismatches:
                print(f"  WARNING: {len(mismatches)}/{len(rows)} {split_name} records "
                      f"have source != '{expected_source}'. "
                      f"This may indicate an input-variant mismatch (KNOWN_ISSUES issue 4).")
                print(f"  Example: source='{mismatches[0].get('source')}'")
                # Don't abort — extract anyway, but warn loudly

        extract_embeddings_for_variant(variant, train_rows, eval_rows, device, out_dir)
        print()

    print("="*60)
    print("EXTRACTION COMPLETE")
    print("="*60)
    print(f"Embeddings saved to {out_dir}/")
    print(f"Files: embeddings_{{variant}}_{{train,eval}}.pt + names_{{variant}}_{{train,eval}}.json")


if __name__ == "__main__":
    main()
