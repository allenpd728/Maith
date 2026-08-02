#!/usr/bin/env python3
"""
eval_completion.py

Next-token prediction accuracy on the held-out eval split.

Perplexity measures average surprise across all tokens. Completion accuracy
measures something sharper: given the first K tokens of a sequence, does the
model's top-1 prediction match the actual next token? This is closer to the
practical claim — that IR tokens make next-step prediction easier.

Methodology:
  - For each example in the eval split, mask the last N tokens (default: 5).
  - Feed the prefix to the model, take the argmax of the logits at each
    masked position, compare to the ground truth.
  - Report top-1 accuracy per variant side by side.

Works across all three variants (A uses the saved custom-vocab model;
B and C use the saved native-BPE models). Loads models from runs/ directory.

Usage:
    # Compare all three variants
    python3 python/eval_completion.py

    # Single variant
    python3 python/eval_completion.py --variants A

    # More examples, more masked tokens
    python3 python/eval_completion.py --samples 100 --mask-last 10

    # Use a specific runs directory
    python3 python/eval_completion.py --runs-dir runs/
"""

import argparse
import json
import math
import os
import sys
from pathlib import Path
from typing import Optional

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
    sys.exit(1)


# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

BASE_MODEL   = "Qwen/Qwen2.5-Coder-0.5B"
DEFAULT_RUNS = "runs/"
DATASETS_DIR = "datasets/"
SEED         = 42


# ---------------------------------------------------------------------------
# Data loading
# ---------------------------------------------------------------------------

def load_eval_split(variant: str, datasets_dir: str, limit: Optional[int]) -> list[dict]:
    path = Path(datasets_dir) / f"eval_{variant}.jsonl"
    if not path.exists():
        raise FileNotFoundError(f"Eval split not found: {path} — run build_dataset.py first")
    examples = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                examples.append(json.loads(line))
    if limit:
        # Deterministic sample — take every Nth example for even coverage
        step = max(1, len(examples) // limit)
        examples = examples[::step][:limit]
    return examples


# ---------------------------------------------------------------------------
# Model loading
# ---------------------------------------------------------------------------

def find_checkpoint_dir(model_dir: Path) -> Path:
    """
    Return the best checkpoint directory inside model_dir.

    Priority order:
      1. Highest-numbered checkpoint-<step>/ subdir (legacy step-based saves).
      2. checkpoint-final/ — written by train.py v2+ (final-only save, no step number).
      3. model_dir itself if no checkpoint subdirs exist.
    """
    # Step-based checkpoints (checkpoint-277, checkpoint-554, etc.)
    numeric_checkpoints = sorted(
        [
            d for d in model_dir.iterdir()
            if d.is_dir()
            and d.name.startswith("checkpoint-")
            and d.name.split("-")[-1].isdigit()
        ],
        key=lambda d: int(d.name.split("-")[-1]),
    )
    if numeric_checkpoints:
        return numeric_checkpoints[-1]

    # Final-only checkpoint written by train.py v2+
    final = model_dir / "checkpoint-final"
    if final.exists():
        return final

    return model_dir


def load_model_and_vocab(variant: str, runs_dir: str, datasets_dir: str, device: str,
                         checkpoint_override: str = None):
    """
    Load a fine-tuned model from runs/variant_X/ (or its latest checkpoint subdir).
    Pass checkpoint_override to use a different run directory for this variant
    (e.g. runs/variant_A_3ep instead of runs/variant_A).
    For variant A, also loads vocab_A.json to get the vocab size.
    Returns (model, vocab_size).
    """
    if checkpoint_override:
        model_dir = Path(checkpoint_override)
    else:
        model_dir = Path(runs_dir) / f"variant_{variant}"
    if not model_dir.exists():
        raise FileNotFoundError(
            f"No saved model at {model_dir} — run train.py --variant {variant} first"
        )
    model_dir = find_checkpoint_dir(model_dir)

    if variant == "A":
        vocab_path = Path(datasets_dir) / "vocab_A.json"
        with open(vocab_path) as f:
            vocab = json.load(f)
        vocab_size = len(vocab)
    else:
        vocab_size = None  # native BPE, unchanged

    print(f"  Loading variant {variant} from {model_dir} ...")
    model = AutoModelForCausalLM.from_pretrained(
        str(model_dir),
        trust_remote_code=True,
        dtype=torch.float32,
    )
    model = model.to(device)
    model.eval()

    if vocab_size:
        print(f"    vocab_size={vocab_size}")
    print(f"    params={sum(p.numel() for p in model.parameters())/1e6:.1f}M")
    return model, vocab_size


# ---------------------------------------------------------------------------
# Evaluation
# ---------------------------------------------------------------------------

def evaluate_completion(
    model,
    examples: list[dict],
    device: str,
    mask_last: int = 5,
) -> dict:
    """
    For each example, feed the prefix (all tokens except the last `mask_last`)
    to the model and check whether the top-1 prediction at each position
    matches the ground truth.

    Returns a dict with top1_accuracy, total_predictions, correct_predictions,
    and per-example results.
    """
    correct = 0
    total = 0
    per_example = []

    with torch.no_grad():
        for ex in examples:
            input_ids = ex["input_ids"]
            labels    = ex["labels"]

            # Need at least mask_last + 1 tokens to do anything meaningful
            if len(input_ids) <= mask_last:
                continue

            # Prefix: everything up to the last mask_last tokens
            prefix_len = len(input_ids) - mask_last

            # Ground truth: the mask_last tokens that follow the prefix.
            # Read directly from input_ids (not labels) to avoid the labels-shift confusion.
            # The last token in input_ids is always EOS; targets won't include it because
            # prefix_len + mask_last == len(input_ids), so targets = input_ids[prefix_len:-0]
            # which is input_ids[prefix_len:], stopping before the implicit EOS position.
            targets = input_ids[prefix_len:prefix_len + mask_last]
            if not targets:
                continue

            # Autoregressive multi-token eval with teacher forcing.
            # At each step: feed current prefix, predict next token, compare to ground truth,
            # then append the ground truth token (not the prediction) so errors don't compound.
            # This measures per-position accuracy at each of the mask_last positions
            # independently, which is what was intended.
            ex_correct = 0
            ex_total   = 0
            current_ids = input_ids[:prefix_len]
            for target_id in targets:
                inp = torch.tensor(current_ids, dtype=torch.long).unsqueeze(0).to(device)
                outputs = model(input_ids=inp)
                pred = int(outputs.logits[0, -1].argmax().item())
                if pred == target_id:
                    ex_correct += 1
                ex_total += 1
                current_ids = current_ids + [target_id]  # teacher forcing

            correct += ex_correct
            total   += ex_total
            per_example.append({
                "name":     ex.get("name", "?"),
                "correct":  ex_correct,
                "total":    ex_total,
                "accuracy": ex_correct / ex_total if ex_total else 0.0,
            })

    top1 = correct / total if total else 0.0
    return {
        "top1_accuracy":         round(top1, 4),
        "correct_predictions":   correct,
        "total_predictions":     total,
        "examples_evaluated":    len(per_example),
        "per_example":           per_example,
    }


# ---------------------------------------------------------------------------
# Reporting
# ---------------------------------------------------------------------------

BOLD  = "\033[1m"
GREEN = "\033[32m"
RED   = "\033[31m"
CYAN  = "\033[36m"
RESET = "\033[0m"


def print_results_table(all_results: dict[str, dict], mask_last: int) -> None:
    print()
    print(f"{'═' * 62}")
    print(f"{BOLD}  Completion Accuracy — top-1, last {mask_last} tokens masked{RESET}")
    print(f"{'═' * 62}")
    print(f"  {'Variant':<12} {'Top-1 Acc':>10} {'Correct':>9} {'Total':>8} {'Examples':>10}")
    print(f"  {'-'*12} {'-'*10} {'-'*9} {'-'*8} {'-'*10}")

    best_acc = max(r["top1_accuracy"] for r in all_results.values())

    for variant in ["A", "B", "C"]:
        if variant not in all_results:
            continue
        r = all_results[variant]
        acc = r["top1_accuracy"]
        flag = f" {GREEN}←{RESET}" if acc == best_acc else ""
        print(
            f"  {'Variant ' + variant:<12}"
            f" {acc:>9.1%}{flag}"
            f" {r['correct_predictions']:>9,}"
            f" {r['total_predictions']:>8,}"
            f" {r['examples_evaluated']:>10,}"
        )

    print(f"{'═' * 62}")

    # Interpretation
    variants_run = list(all_results.keys())
    if len(variants_run) == 3:
        acc_A = all_results["A"]["top1_accuracy"]
        acc_B = all_results["B"]["top1_accuracy"]
        acc_C = all_results["C"]["top1_accuracy"]
        print()
        if acc_A > acc_B and acc_A > acc_C:
            lift_vs_B = (acc_A - acc_B) / acc_B * 100 if acc_B > 0 else float("inf")
            print(f"  {GREEN}{BOLD}✓ Variant A leads{RESET} — {lift_vs_B:.1f}% relative improvement over B.")
            print(f"    IR tokens make next-step prediction easier at this scale.")
            print(f"    Supports the representation hypothesis.")
        elif acc_A < acc_B and acc_A < acc_C:
            print(f"  {RED}✗ Variant A trails{RESET} — IR tokens hurt completion accuracy here.")
            print(f"    Possible causes: vocab too small, cross-graph identity loss,")
            print(f"    or insufficient training data for custom-vocab generalisation.")
        else:
            print(f"  ~ Mixed: A={acc_A:.1%}, B={acc_B:.1%}, C={acc_C:.1%}")
            print(f"    No clear winner. Consider expanding corpus or training longer.")
    elif len(variants_run) < 3:
        missing_v = [v for v in ["A", "B", "C"] if v not in variants_run]
        print(f"\n  {len(variants_run)}/3 variants evaluated. Missing: {', '.join(missing_v)}")
    print()


def save_results(all_results: dict[str, dict], runs_dir: str, mask_last: int) -> None:
    out = {
        "mask_last": mask_last,
        "variants":  all_results,
    }
    out_path = Path(runs_dir) / "completion_accuracy.json"
    with open(out_path, "w") as f:
        json.dump(out, f, indent=2)
    print(f"  Results saved → {out_path}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Next-token completion accuracy across A/B/C variants."
    )
    parser.add_argument("--variants",    default="A,B,C",
                        help="Comma-separated variants to evaluate (default: A,B,C)")
    parser.add_argument("--samples",     type=int, default=50,
                        help="Number of eval examples to use (default: 50)")
    parser.add_argument("--mask-last",   type=int, default=5,
                        help="Number of tokens to mask at end of each sequence (default: 5)")
    parser.add_argument("--runs-dir",    default=DEFAULT_RUNS)
    parser.add_argument("--datasets",    default=DATASETS_DIR)
    parser.add_argument("--checkpoint-A", default=None,
                        help="Override checkpoint directory for variant A (e.g. runs/variant_A_v3)")
    parser.add_argument("--checkpoint-B", default=None,
                        help="Override checkpoint directory for variant B (e.g. runs/variant_B2)")
    parser.add_argument("--checkpoint-C", default=None,
                        help="Override checkpoint directory for variant C (e.g. runs/variant_C_v2)")
    args = parser.parse_args()

    if not Path("lakefile.lean").exists():
        print("ERROR: run from the Maith repo root")
        sys.exit(1)

    device = "mps"  if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Samples per variant: {args.samples}  |  Masking last {args.mask_last} tokens")
    print()

    variants = [v.strip().upper() for v in args.variants.split(",")]
    all_results: dict[str, dict] = {}

    for variant in variants:
        print(f"── Variant {variant} ──")
        try:
            ckpt_override = getattr(args, f"checkpoint_{variant.upper()}", None)
            model, _ = load_model_and_vocab(variant, args.runs_dir, args.datasets, device,
                                            checkpoint_override=ckpt_override)
            examples  = load_eval_split(variant, args.datasets, limit=args.samples)
            print(f"    {len(examples)} eval examples loaded")
            result = evaluate_completion(model, examples, device, mask_last=args.mask_last)
            all_results[variant] = result
            print(f"    Top-1 accuracy: {result['top1_accuracy']:.1%}  "
                  f"({result['correct_predictions']}/{result['total_predictions']} tokens)")
            # Free memory before loading next model
            del model
            if device == "mps":
                torch.mps.empty_cache()
            elif device == "cuda":
                torch.cuda.empty_cache()
        except FileNotFoundError as e:
            print(f"    {RED}Skipping — {e}{RESET}")
        print()

    if not all_results:
        print("No variants could be evaluated. Run train.py first.")
        sys.exit(1)

    print_results_table(all_results, args.mask_last)
    save_results(all_results, args.runs_dir, args.mask_last)


if __name__ == "__main__":
    main()
