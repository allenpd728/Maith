#!/usr/bin/env python3
"""
extract_representations.py

Forward-pass all corpus examples through each trained model checkpoint and save
mean-pooled final hidden layer representations.

Usage:
    python3 python/extract_representations.py \
        --corpus Corpus/corpus.jsonl \
        --out runs/probing

    # Run a subset of variants:
    python3 python/extract_representations.py \
        --corpus Corpus/corpus.jsonl \
        --out runs/probing \
        --variants A,C,flat
"""

import argparse
import gc
import json
import os
import sys

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

# ---------------------------------------------------------------------------
# Dependency check
# ---------------------------------------------------------------------------

missing = []
try:
    import torch
except ImportError:
    missing.append("torch")
try:
    from transformers import AutoModelForCausalLM, AutoTokenizer
except ImportError:
    missing.append("transformers")

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    print("Run: pip install torch transformers")
    sys.exit(1)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

HIDDEN_SIZE = 896
MAX_SEQ_LEN = 512
EXTRACTION_SEED = 42

# Checkpoint paths — resolved relative to the repo root, or override with MAITH_RUNS_DIR.
_RUNS_DIR = os.environ.get(
    "MAITH_RUNS_DIR",
    os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "runs"),
)
CHECKPOINTS = {
    "A":    os.path.join(_RUNS_DIR, "variant_A_v1_4_0", "checkpoint-final"),
    "C":    os.path.join(_RUNS_DIR, "variant_C_phase6",  "checkpoint-final"),
    "flat": os.path.join(_RUNS_DIR, "variant_flat",      "checkpoint-final"),
    "B":    os.path.join(_RUNS_DIR, "variant_B_phase6",  "checkpoint-final"),
    "random": None,  # Loaded from HuggingFace cache
}

BASE_MODEL = "Qwen/Qwen2.5-Coder-0.5B"

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def get_device() -> str:
    """Select the best available device."""
    if torch.cuda.is_available():
        return "cuda"
    elif torch.backends.mps.is_available():
        return "mps"
    else:
        return "cpu"


def arity_bin(graph: dict) -> int:
    """Compute arity bin from graph operations count."""
    n_ops = len(graph.get("operations", []))
    if n_ops <= 5:
        return 0
    elif n_ops <= 15:
        return 1
    elif n_ops <= 40:
        return 2
    else:
        return 3


def mean_pool(hidden_states: torch.Tensor) -> torch.Tensor:
    """Mean-pool across sequence dimension (dim=1)."""
    return hidden_states.mean(dim=1)


def check_tensor_valid(tensor: torch.Tensor, name: str) -> bool:
    """Check for NaN/Inf and print status."""
    has_nan = torch.isnan(tensor).any().item()
    has_inf = torch.isinf(tensor).any().item()
    if has_nan:
        print(f"  WARNING: {name} contains NaN values!")
        return False
    if has_inf:
        print(f"  WARNING: {name} contains Inf values!")
        return False
    return True


# ---------------------------------------------------------------------------
# Extraction logic
# ---------------------------------------------------------------------------

def load_corpus(corpus_path: str) -> list[dict]:
    """Load corpus from JSONL file."""
    examples = []
    with open(corpus_path, "r") as f:
        for line in f:
            examples.append(json.loads(line.strip()))
    return examples


def save_labels(examples: list[dict], out_dir: str) -> None:
    """Save labels JSON for probing task."""
    labels = []
    for ex in examples:
        # Extract short module suffix (last 3 components)
        full_module = ex["module"]
        parts = full_module.split(".")
        short_module = ".".join(parts[-3:]) if len(parts) >= 3 else full_module

        labels.append({
            "name": ex["name"],
            "module": full_module,
            "module_short": short_module,
            "arity_bin": arity_bin(ex.get("graph", {})),
        })

    os.makedirs(out_dir, exist_ok=True)
    labels_path = os.path.join(out_dir, "labels.json")
    with open(labels_path, "w") as f:
        json.dump(labels, f, indent=2)
    print(f"Saved labels ({len(labels)} examples) → {labels_path}")


def patch_tokenizer_config(ckpt_path: str) -> None:
    """
    Fix extra_special_tokens field if it is a list (saved by older transformers).
    Newer transformers expects a dict. Patches in-memory via a temp override file.
    """
    import json, os
    cfg_path = os.path.join(ckpt_path, "tokenizer_config.json")
    with open(cfg_path) as f:
        cfg = json.load(f)
    if isinstance(cfg.get("extra_special_tokens"), list):
        cfg["extra_special_tokens"] = {}
        with open(cfg_path, "w") as f:
            json.dump(cfg, f, indent=2)
        print(f"  Patched extra_special_tokens list→dict in {cfg_path}")


def extract_representations_for_variant(
    variant: str,
    examples: list[dict],
    device: str,
    out_dir: str,
) -> torch.Tensor:
    """
    Extract mean-pooled representations for a single variant.
    Returns tensor of shape [N, 896].
    """
    torch.manual_seed(EXTRACTION_SEED)

    if variant == "random":
        print(f"Loading pretrained {BASE_MODEL} (no fine-tuning)...")
        model = AutoModelForCausalLM.from_pretrained(
            BASE_MODEL,
            dtype=torch.float32,
            trust_remote_code=True,
        )
        model = model.to(device)
        tokenizer = AutoTokenizer.from_pretrained(
            BASE_MODEL,
            trust_remote_code=True,
        )
    else:
        ckpt_path = CHECKPOINTS[variant]
        print(f"Loading checkpoint: {ckpt_path}...")
        patch_tokenizer_config(ckpt_path)
        model = AutoModelForCausalLM.from_pretrained(
            ckpt_path,
            dtype=torch.float32,
            trust_remote_code=True,
        )
        model = model.to(device)
        tokenizer = AutoTokenizer.from_pretrained(
            ckpt_path,
            trust_remote_code=True,
        )

    model.eval()
    for param in model.parameters():
        param.requires_grad = False

    all_representations = []

    print(f"  Extracting representations for {len(examples)} examples...")
    for i, ex in enumerate(examples):
        if (i + 1) % 500 == 0 or i == 0:
            print(f"    Processing example {i + 1}/{len(examples)}...")

        # Join tokens with space as the input string
        tokens = ex.get("tokens", [])
        input_str = " ".join(tokens)

        # Tokenize and truncate
        encoded = tokenizer(
            input_str,
            return_tensors="pt",
            truncation=True,
            max_length=MAX_SEQ_LEN,
            add_special_tokens=False,
        )
        input_ids = encoded["input_ids"].to(device)

        # Forward pass with hidden states
        with torch.no_grad():
            outputs = model(
                input_ids,
                output_hidden_states=True,
            )
            # Final hidden layer (index -1 after removing embedding + 27 transformer layers)
            # hidden_states is tuple of length (num_layers + 1), last one is final
            final_hidden = outputs.hidden_states[-1]  # [1, seq_len, 896]

        # Mean-pool
        pooled = mean_pool(final_hidden)  # [1, 896]
        all_representations.append(pooled.cpu())

    # Stack all representations
    representations = torch.cat(all_representations, dim=0)  # [N, 896]

    # Free memory
    del model, tokenizer
    gc.collect()
    if device == "mps":
        torch.mps.empty_cache()
    elif device == "cuda":
        torch.cuda.empty_cache()

    return representations


def run_extraction(corpus_path: str, out_dir: str, variants: list[str]) -> None:
    """Main extraction pipeline."""
    device = get_device()
    print(f"Using device: {device}")
    print()

    # Load corpus
    print(f"Loading corpus from: {corpus_path}")
    examples = load_corpus(corpus_path)
    print(f"Loaded {len(examples)} examples\n")

    # Save labels (only once, after corpus is loaded)
    save_labels(examples, out_dir)
    print()

    # Extract for each variant
    for variant in variants:
        print(f"{'='*60}")
        print(f"EXTRACTING VARIANT: {variant}")
        print(f"{'='*60}")

        representations = extract_representations_for_variant(
            variant, examples, device, out_dir
        )

        # Verify shape
        print(f"  Shape: {representations.shape}")
        assert representations.shape == (len(examples), HIDDEN_SIZE), \
            f"Expected ({len(examples)}, {HIDDEN_SIZE}), got {representations.shape}"

        # Check for NaN/Inf
        valid = check_tensor_valid(representations, f"Variant {variant}")
        if not valid:
            print(f"  ERROR: Variant {variant} has invalid values!")
            sys.exit(1)

        # Compute statistics
        mean_val = representations.mean().item()
        std_val = representations.std().item()
        print(f"  Mean: {mean_val:.4f}, Std: {std_val:.4f}")

        # Degenerate warning for flat variant
        if variant == "flat":
            if std_val < 0.01:
                print(f"  WARNING: Flat-IR representations are near-degenerate (std={std_val:.6f} < 0.01)")
                print(f"  Probe results may be meaningless!")
            else:
                print(f"  Flat-IR std OK (>= 0.01)")

        # Save representations
        os.makedirs(out_dir, exist_ok=True)
        out_path = os.path.join(out_dir, f"representations_{variant}.pt")
        torch.save(representations, out_path)
        print(f"  Saved → {out_path}")
        print()

    print("="*60)
    print("EXTRACTION COMPLETE")
    print("="*60)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Extract mean-pooled representations from trained checkpoints."
    )
    parser.add_argument(
        "--corpus",
        default="Corpus/corpus.jsonl",
        help="Path to corpus JSONL file (default: Corpus/corpus.jsonl)",
    )
    parser.add_argument(
        "--out",
        default="runs/probing",
        help="Output directory for representations (default: runs/probing)",
    )
    parser.add_argument(
        "--variants",
        default="A,C,flat,B,random",
        help="Comma-separated list of variants to extract (default: A,C,flat,B,random)",
    )

    args = parser.parse_args()

    # Parse variants
    all_variants = ["A", "C", "flat", "B", "random"]
    requested = [v.strip() for v in args.variants.split(",")]
    variants = [v for v in all_variants if v in requested]

    if not variants:
        print(f"ERROR: No valid variants specified. Valid: {all_variants}")
        sys.exit(1)

    # Validate corpus exists
    if not os.path.exists(args.corpus):
        print(f"ERROR: Corpus file not found: {args.corpus}")
        sys.exit(1)

    run_extraction(args.corpus, args.out, variants)


if __name__ == "__main__":
    main()
