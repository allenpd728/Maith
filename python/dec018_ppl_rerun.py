#!/usr/bin/env python3
"""
dec018_ppl_rerun.py  —  DEC-018: equal-sequence-length perplexity rerun

Evaluates perplexity for all three variants with a shared 512-token cap so
that the comparison is not confounded by asymmetric truncation (DEC-017).

All three variants are evaluated over the same token-length distribution:
sequences longer than 512 are truncated identically for A, B, and C.

Outputs:
  runs/dec018_ppl_results.json   — machine-readable summary
  Printed stratified table       — short / medium / long buckets with
                                   truncation counts per variant

Usage:
    cd <maith-repo-root>
    python3 python/dec018_ppl_rerun.py
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
    from transformers import AutoModelForCausalLM
except ImportError:
    missing.append("transformers")
if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    sys.exit(1)

# ---------------------------------------------------------------------------
# Config — shared cap applied to ALL three variants
# ---------------------------------------------------------------------------

SHARED_SEQ_CAP = 512   # DEC-018: equal cap for A, B, C

VARIANTS = {
    "A": {
        "ckpt": "runs/variant_A_v3/checkpoint-final",
        "eval": "datasets/eval_A.jsonl",
    },
    "B": {
        "ckpt": "runs/variant_B2/checkpoint-final",
        "eval": "datasets/eval_B.jsonl",
    },
    "C": {
        "ckpt": "runs/variant_C_v2/checkpoint-final",
        "eval": "datasets/eval_C.jsonl",
    },
}

RUNS_DIR = pathlib.Path("runs")

LENGTH_BUCKETS = [
    ("short",  0,   128),
    ("medium", 128, 512),
    ("long",   512, 999999),
]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def load_examples(path):
    return [json.loads(l) for l in pathlib.Path(path).read_text().strip().splitlines() if l.strip()]


def bucket_label(raw_len):
    for label, lo, hi in LENGTH_BUCKETS:
        if lo <= raw_len < hi:
            return label
    return "long"


# ---------------------------------------------------------------------------
# Per-example perplexity at shared cap
# ---------------------------------------------------------------------------

def compute_perplexity(model, examples, seq_cap, device):
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
            print(f"    ppl [{i+1}/{n}] {ex.get('name', '?')!r:.40s} (raw={raw_len}, cap={capped_len})", flush=True)
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
# Stratification table
# ---------------------------------------------------------------------------

def stratify(results, variant_label):
    by_bucket = {b[0]: [] for b in LENGTH_BUCKETS}
    for r in results:
        by_bucket[bucket_label(r["raw_len"])].append(r)

    print(f"\n  {variant_label} — perplexity @ shared {SHARED_SEQ_CAP}-token cap")
    print(f"  {'Bucket':<14} {'N':>5} {'Truncated':>10} {'Mean PPL':>10} {'Median PPL':>11}")
    print(f"  {'-'*14} {'-'*5} {'-'*10} {'-'*10} {'-'*11}")

    summary = {}
    for label, lo, hi in LENGTH_BUCKETS:
        rows = by_bucket[label]
        if not rows:
            print(f"  {label:<14} {'0':>5}")
            summary[label] = None
            continue
        ppls = sorted(r["perplexity"] for r in rows)
        mean_p   = sum(ppls) / len(ppls)
        median_p = ppls[len(ppls) // 2]
        trunc    = sum(1 for r in rows if r["truncated"])
        print(f"  {label:<14} {len(rows):>5} {trunc:>10} {mean_p:>10.3f} {median_p:>11.3f}")
        summary[label] = {"n": len(rows), "truncated": trunc, "mean_ppl": round(mean_p, 4), "median_ppl": round(median_p, 4)}

    all_ppls = [r["perplexity"] for r in results]
    trunc_total = sum(1 for r in results if r["truncated"])
    overall_mean = sum(all_ppls) / len(all_ppls) if all_ppls else float("nan")
    print(f"  {'OVERALL':<14} {len(all_ppls):>5} {trunc_total:>10} {overall_mean:>10.3f}")
    summary["overall"] = {"n": len(all_ppls), "truncated": trunc_total, "mean_ppl": round(overall_mean, 4)}
    return summary


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
    print(f"DEC-018: equal-sequence-length perplexity rerun")
    print(f"Shared seq_cap = {SHARED_SEQ_CAP} tokens (applied identically to A, B, C)")
    print()

    all_results = {}
    all_summaries = {}

    for variant, cfg in VARIANTS.items():
        ckpt_path = pathlib.Path(cfg["ckpt"])
        if not ckpt_path.exists():
            print(f"[{variant}] checkpoint not found at {ckpt_path}, skipping")
            continue

        print(f"{'='*60}")
        print(f"  Variant {variant}")
        print(f"{'='*60}")
        print(f"  Loading model from {ckpt_path} ...", flush=True)
        model = AutoModelForCausalLM.from_pretrained(
            str(ckpt_path), trust_remote_code=True, dtype=torch.float32
        ).to(device)
        model.eval()
        print(f"  Model loaded. params={sum(p.numel() for p in model.parameters())/1e6:.1f}M", flush=True)

        examples = load_examples(cfg["eval"])
        print(f"  {len(examples)} eval examples loaded", flush=True)
        print(f"  Computing perplexity (seq_cap={SHARED_SEQ_CAP}) ...", flush=True)

        results = compute_perplexity(model, examples, SHARED_SEQ_CAP, device)
        all_results[variant] = results
        all_summaries[variant] = stratify(results, f"Variant {variant}")

        del model
        if device == "mps":
            torch.mps.empty_cache()
        elif device == "cuda":
            torch.cuda.empty_cache()
        print()

    # -----------------------------------------------------------------------
    # Cross-variant summary
    # -----------------------------------------------------------------------

    print(f"\n{'='*60}")
    print(f"  DEC-018 SUMMARY: Perplexity at shared {SHARED_SEQ_CAP}-token cap")
    print(f"{'='*60}")
    print(f"  {'Variant':<10} {'Bucket':<14} {'N':>5} {'Truncated':>10} {'Mean PPL':>10}")
    print(f"  {'-'*10} {'-'*14} {'-'*5} {'-'*10} {'-'*10}")
    for variant in ["A", "B", "C"]:
        if variant not in all_summaries:
            continue
        s = all_summaries[variant]
        for bucket in ["short", "medium", "long", "overall"]:
            if s.get(bucket) is None:
                continue
            b = s[bucket]
            print(f"  {'Var '+variant:<10} {bucket:<14} {b['n']:>5} {b['truncated']:>10} {b['mean_ppl']:>10.3f}")
        print()

    # Save
    out = {
        "seq_cap": SHARED_SEQ_CAP,
        "summaries": all_summaries,
    }
    out_path = RUNS_DIR / "dec018_ppl_results.json"
    out_path.write_text(json.dumps(out, indent=2))
    print(f"  Results saved → {out_path}")


if __name__ == "__main__":
    main()
