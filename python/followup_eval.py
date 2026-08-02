#!/usr/bin/env python3
"""
followup_eval.py

Two follow-ups in one model-load pass per variant:

  1. DEC-002 stratified perplexity — per-example loss computed at the
     capped sequence length (512 for B/C, 1024 for A), then binned by
     the UNCAPPED sequence length so we can see whether truncation is
     helping B/C on examples that would have been long.

  2. Higher-confidence completion accuracy — 200 examples, mask_last=10,
     teacher-forced autoregressive (same methodology as eval_completion.py).

Loads each model once and runs both evaluations before freeing memory.

Usage:
    cd <maith-repo-root>
    python3 python/followup_eval.py
"""

import json
import math
import pathlib
import sys

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
# Config
# ---------------------------------------------------------------------------

VARIANTS = {
    "A": {
        "ckpt":      "runs/variant_A_v3/checkpoint-final",
        "eval":      "datasets/eval_A.jsonl",
        "seq_cap":   1024,   # train cap used for A
    },
    "B": {
        "ckpt":      "runs/variant_B2/checkpoint-final",
        "eval":      "datasets/eval_B.jsonl",
        "seq_cap":   512,
    },
    "C": {
        "ckpt":      "runs/variant_C_v2/checkpoint-final",
        "eval":      "datasets/eval_C.jsonl",
        "seq_cap":   512,
    },
}

COMPLETION_SAMPLES = 200
MASK_LAST          = 10
SEED               = 42
DATASETS_DIR       = pathlib.Path("datasets")
RUNS_DIR           = pathlib.Path("runs")

LENGTH_BUCKETS = [
    ("short",  0,   128),
    ("medium", 128, 512),
    ("long",   512, 999999),
]

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def load_examples(path: pathlib.Path):
    return [json.loads(l) for l in path.read_text().strip().splitlines() if l.strip()]


def sample_for_completion(examples, n, seed):
    """Deterministic even-stride sample of n examples."""
    if len(examples) <= n:
        return examples
    import random
    rng = random.Random(seed)
    step = max(1, len(examples) // n)
    pool = examples[::step][:n]
    # top up if stride was too coarse
    if len(pool) < n:
        extras = [e for e in examples if e not in pool]
        rng.shuffle(extras)
        pool += extras[:n - len(pool)]
    return pool[:n]


def bucket_label(raw_len):
    for label, lo, hi in LENGTH_BUCKETS:
        if lo <= raw_len < hi:
            return label
    return "long"


# ---------------------------------------------------------------------------
# Per-example perplexity (full sequence, capped at seq_cap)
# ---------------------------------------------------------------------------

def compute_per_example_perplexity(model, examples, seq_cap, device):
    """
    Returns list of dicts: {name, raw_len, capped_len, perplexity, truncated}
    raw_len  = original sequence length (before cap)
    capped_len = min(raw_len, seq_cap)
    """
    results = []
    n = len(examples)
    with torch.no_grad():
        for i, ex in enumerate(examples):
            ids = ex["input_ids"]
            raw_len = len(ids)
            ids_capped = ids[:seq_cap]
            capped_len = len(ids_capped)
            if capped_len < 2:
                continue
            print(f"    ppl [{i+1}/{n}] {ex.get('name', '?')!r:.40s} (len={capped_len})", flush=True)
            inp = torch.tensor(ids_capped, dtype=torch.long).unsqueeze(0).to(device)
            out = model(input_ids=inp, labels=inp)
            if device == "mps":
                torch.mps.synchronize()
            loss = out.loss.item()
            if not math.isfinite(loss):
                continue
            results.append({
                "name":       ex.get("name", "?"),
                "raw_len":    raw_len,
                "capped_len": capped_len,
                "truncated":  raw_len > seq_cap,
                "perplexity": round(math.exp(loss), 4),
            })
    return results


# ---------------------------------------------------------------------------
# Completion accuracy (mask_last tokens, teacher-forced, single-pass)
# ---------------------------------------------------------------------------

def compute_completion_accuracy(model, examples, mask_last, device):
    """
    Teacher-forced completion accuracy using a single forward pass per example.

    For each example we feed the full sequence (prefix + targets) to the model
    once, then read the argmax predictions at the mask_last positions from the
    returned logits.  This replaces the original token-by-token loop (which
    required mask_last forward passes + mask_last MPS syncs per example) with
    a single forward pass + one sync, reducing MPS command-buffer pressure by
    ~10x and eliminating the primary source of queue-overflow stalls.

    Semantics are identical to the original: each predicted token is conditioned
    only on the ground-truth prefix up to that position (teacher forcing), which
    is exactly what logits[:, prefix_len-1 : prefix_len+mask_last-1, :] gives
    from a single full-sequence forward pass.
    """
    correct = 0
    total   = 0
    per_example = []
    n = len(examples)
    with torch.no_grad():
        for i, ex in enumerate(examples):
            ids = ex["input_ids"]
            if len(ids) <= mask_last:
                continue
            prefix_len = len(ids) - mask_last
            targets    = ids[prefix_len:prefix_len + mask_last]
            if not targets:
                continue
            print(f"    completion [{i+1}/{n}] {ex.get('name', '?')!r:.40s} (len={len(ids)})", flush=True)

            # Single forward pass over the full sequence
            inp    = torch.tensor(ids, dtype=torch.long).unsqueeze(0).to(device)
            logits = model(input_ids=inp).logits   # [1, seq_len, vocab]
            if device == "mps":
                torch.mps.synchronize()

            # logits[:, t, :] predicts token t+1, so predictions for positions
            # prefix_len..prefix_len+mask_last-1 live at logits[:, prefix_len-1..prefix_len+mask_last-2, :]
            pred_logits = logits[0, prefix_len - 1 : prefix_len + mask_last - 1, :]  # [mask_last, vocab]
            preds       = pred_logits.argmax(dim=-1).tolist()                          # [mask_last]

            ex_correct = sum(p == t for p, t in zip(preds, targets))
            correct   += ex_correct
            total     += mask_last
            per_example.append({
                "name":     ex.get("name", "?"),
                "correct":  ex_correct,
                "total":    mask_last,
                "accuracy": ex_correct / mask_last,
            })

    acc = correct / total if total else 0.0
    return {
        "top1_accuracy":       round(acc, 4),
        "correct":             correct,
        "total":               total,
        "examples_evaluated":  len(per_example),
        "per_example":         per_example,
    }


# ---------------------------------------------------------------------------
# Stratification report
# ---------------------------------------------------------------------------

def stratify_perplexity(ppl_results, label):
    print(f"\n  {label} — perplexity by sequence-length bucket (raw, pre-cap length)")
    print(f"  {'Bucket':<14} {'N':>5} {'Truncated':>10} {'Mean PPL':>10} {'Median PPL':>11}")
    print(f"  {'-'*14} {'-'*5} {'-'*10} {'-'*10} {'-'*11}")

    by_bucket = {b[0]: [] for b in LENGTH_BUCKETS}
    for r in ppl_results:
        by_bucket[bucket_label(r["raw_len"])].append(r)

    all_ppls = [r["perplexity"] for r in ppl_results]
    truncated_total = sum(1 for r in ppl_results if r["truncated"])

    for label_b, lo, hi in LENGTH_BUCKETS:
        rows = by_bucket[label_b]
        if not rows:
            print(f"  {label_b:<14} {'0':>5} {'-':>10} {'-':>10} {'-':>11}")
            continue
        ppls = sorted(r["perplexity"] for r in rows)
        mean_ppl   = sum(ppls) / len(ppls)
        median_ppl = ppls[len(ppls) // 2]
        trunc      = sum(1 for r in rows if r["truncated"])
        print(f"  {label_b:<14} {len(rows):>5} {trunc:>10} {mean_ppl:>10.3f} {median_ppl:>11.3f}")

    overall_mean = sum(all_ppls) / len(all_ppls) if all_ppls else float("nan")
    print(f"  {'OVERALL':<14} {len(all_ppls):>5} {truncated_total:>10} {overall_mean:>10.3f}")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    if not pathlib.Path("lakefile.lean").exists():
        print("ERROR: run from the Maith repo root")
        sys.exit(1)

    device = "mps"  if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Completion eval: {COMPLETION_SAMPLES} examples, mask_last={MASK_LAST}")
    print()

    all_ppl_results        = {}
    all_completion_results = {}

    for variant, cfg in VARIANTS.items():
        ckpt_path = pathlib.Path(cfg["ckpt"])
        if not ckpt_path.exists():
            print(f"[{variant}] checkpoint not found at {ckpt_path}, skipping")
            continue

        print(f"{'='*60}")
        print(f"  Variant {variant}  (seq_cap={cfg['seq_cap']})")
        print(f"{'='*60}")

        # Load model
        print(f"  Loading tokenizer/model from {ckpt_path} ...", flush=True)
        model = AutoModelForCausalLM.from_pretrained(
            str(ckpt_path), trust_remote_code=True, dtype=torch.float32
        ).to(device)
        model.eval()
        print(f"  Model loaded. params={sum(p.numel() for p in model.parameters())/1e6:.1f}M", flush=True)

        # Load eval examples
        all_examples = load_examples(pathlib.Path(cfg["eval"]))
        print(f"  {len(all_examples)} eval examples loaded", flush=True)

        # --- Follow-up 1: per-example perplexity (all 376 examples) ---
        print(f"\n  [1/2] Computing per-example perplexity (n={len(all_examples)}) ...", flush=True)
        ppl_results = compute_per_example_perplexity(
            model, all_examples, cfg["seq_cap"], device
        )
        all_ppl_results[variant] = ppl_results
        stratify_perplexity(ppl_results, f"Variant {variant}")

        # --- Follow-up 2: completion accuracy (200 examples, mask_last=10) ---
        comp_examples = sample_for_completion(all_examples, COMPLETION_SAMPLES, SEED)
        print(f"\n  [2/2] Completion accuracy ({len(comp_examples)} examples, mask_last={MASK_LAST}) ...", flush=True)
        comp_result = compute_completion_accuracy(model, comp_examples, MASK_LAST, device)
        all_completion_results[variant] = comp_result
        print(f"  Top-1 accuracy: {comp_result['top1_accuracy']:.1%}  "
              f"({comp_result['correct']}/{comp_result['total']} tokens, "
              f"{comp_result['examples_evaluated']} examples)")

        # Free memory
        del model
        if device == "mps":
            torch.mps.empty_cache()
        elif device == "cuda":
            torch.cuda.empty_cache()
        print()

    # -----------------------------------------------------------------------
    # Summary tables
    # -----------------------------------------------------------------------

    print(f"\n{'='*60}")
    print("  FOLLOW-UP 1: Perplexity stratified by sequence length")
    print(f"{'='*60}")
    print()
    print(f"  {'Variant':<10} {'Bucket':<14} {'N':>5} {'Truncated':>10} {'Mean PPL':>10} {'Median PPL':>11}")
    print(f"  {'-'*10} {'-'*14} {'-'*5} {'-'*10} {'-'*10} {'-'*11}")
    for variant in ["A", "B", "C"]:
        if variant not in all_ppl_results:
            continue
        rows = all_ppl_results[variant]
        by_bucket = {b[0]: [] for b in LENGTH_BUCKETS}
        for r in rows:
            by_bucket[bucket_label(r["raw_len"])].append(r)
        for label_b, lo, hi in LENGTH_BUCKETS:
            bucket_rows = by_bucket[label_b]
            if not bucket_rows:
                print(f"  {'Var '+variant:<10} {label_b:<14} {'0':>5}")
                continue
            ppls = sorted(r["perplexity"] for r in bucket_rows)
            mean_p   = sum(ppls) / len(ppls)
            median_p = ppls[len(ppls) // 2]
            trunc    = sum(1 for r in bucket_rows if r["truncated"])
            print(f"  {'Var '+variant:<10} {label_b:<14} {len(bucket_rows):>5} "
                  f"{trunc:>10} {mean_p:>10.3f} {median_p:>11.3f}")
        print()

    print(f"\n{'='*60}")
    print("  FOLLOW-UP 2: Completion accuracy (200 examples, mask_last=10)")
    print(f"{'='*60}")
    print(f"  {'Variant':<10} {'Top-1 Acc':>10} {'Correct':>9} {'Total':>8} {'Examples':>10}")
    print(f"  {'-'*10} {'-'*10} {'-'*9} {'-'*8} {'-'*10}")
    for variant in ["A", "B", "C"]:
        if variant not in all_completion_results:
            continue
        r = all_completion_results[variant]
        print(f"  {'Var '+variant:<10} {r['top1_accuracy']:>9.1%} "
              f"{r['correct']:>9} {r['total']:>8} {r['examples_evaluated']:>10}")

    # Save results
    out = {
        "completion": {v: {k: val for k, val in r.items() if k != "per_example"}
                       for v, r in all_completion_results.items()},
        "stratified_perplexity": {
            v: {
                bucket: {
                    "n": len([r for r in rows if bucket_label(r["raw_len"]) == bucket]),
                    "truncated": sum(1 for r in rows if bucket_label(r["raw_len"]) == bucket and r["truncated"]),
                    "mean_ppl": round(
                        sum(r["perplexity"] for r in rows if bucket_label(r["raw_len"]) == bucket) /
                        max(1, len([r for r in rows if bucket_label(r["raw_len"]) == bucket])), 4),
                }
                for bucket in ["short", "medium", "long"]
            }
            for v, rows in all_ppl_results.items()
        }
    }
    out_path = RUNS_DIR / "followup_eval_results.json"
    out_path.write_text(json.dumps(out, indent=2))
    print(f"\n  Results saved → {out_path}")


if __name__ == "__main__":
    main()
