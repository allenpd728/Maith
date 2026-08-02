#!/usr/bin/env python3
"""
compare_results.py

Read results.json from each A/B/C training run and print a comparison table.

Usage:
    python3 python/compare_results.py
    python3 python/compare_results.py --runs-dir runs/
"""

import argparse
import json
from typing import Optional
import os
import sys


def run_quality(result: dict) -> str:
    return "SMOKE" if result.get("smoke_test") else "FULL"


def load_result(runs_dir: str, variant: str) -> Optional[dict]:
    path = os.path.join(runs_dir, f"variant_{variant}", "results.json")
    if not os.path.exists(path):
        return None
    with open(path) as f:
        return json.load(f)


def load_loss_curve(runs_dir: str, variant: str) -> dict:
    curve_path = os.path.join(runs_dir, f"variant_{variant}", "loss_curve.json")
    if os.path.exists(curve_path):
        with open(curve_path) as f:
            data = json.load(f)
        return {
            "source": "loss_curve.json",
            "train_loss_curve": data.get("train_loss_curve", []),
            "eval_loss_curve": data.get("eval_loss_curve", []),
        }

    variant_dir = os.path.join(runs_dir, f"variant_{variant}")
    checkpoint_dirs = sorted(
        (
            d for d in os.listdir(variant_dir)
            if d.startswith("checkpoint-") and os.path.isdir(os.path.join(variant_dir, d))
        ),
        key=lambda name: int(name.split("-")[-1]),
    ) if os.path.isdir(variant_dir) else []
    trainer_state_path = (
        os.path.join(variant_dir, checkpoint_dirs[-1], "trainer_state.json")
        if checkpoint_dirs
        else ""
    )
    if os.path.exists(trainer_state_path):
        with open(trainer_state_path) as f:
            state = json.load(f)
        history = state.get("log_history", [])
        return {
            "source": "trainer_state.json",
            "train_loss_curve": [
                {"step": h["step"], "epoch": h.get("epoch"), "loss": h["loss"]}
                for h in history
                if "loss" in h and "step" in h
            ],
            "eval_loss_curve": [
                {"step": h["step"], "epoch": h.get("epoch"), "eval_loss": h["eval_loss"]}
                for h in history
                if "eval_loss" in h and "step" in h
            ],
        }

    return {"source": "none", "train_loss_curve": [], "eval_loss_curve": []}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument(
        "--strict-full",
        action="store_true",
        help="Exit non-zero unless all A/B/C results are present and non-smoke",
    )
    args = parser.parse_args()

    results = {v: load_result(args.runs_dir, v) for v in ["A", "B", "C"]}
    any_found = any(r is not None for r in results.values())

    if not any_found:
        print("No results found yet. Run train.py for at least one variant first.")
        print(f"  Expected: {args.runs_dir}variant_A/results.json, etc.")
        return 1 if args.strict_full else 0

    print()
    print("=== A/B/C Experiment Results ===")
    print()
    print(
        f"  {'Variant':<10} {'Perplexity':>12} {'Vocab':>8} {'Params':>10} "
        f"{'Minutes':>9} {'Quality':>8} {'Tokenizer':>17} {'Caps(T/E)':>10} {'Repr':>24}"
    )
    print(f"  {'-'*10} {'-'*12} {'-'*8} {'-'*10} {'-'*9} {'-'*8} {'-'*17} {'-'*10} {'-'*24}")

    present_results = [r for r in results.values() if r is not None]
    has_mixed_quality = (
        len({run_quality(r) for r in present_results}) > 1
        if present_results
        else False
    )
    if has_mixed_quality:
        print("  ⚠ Mixed run quality detected (SMOKE + FULL). Do not compare winners yet.")
        print()

    best_ppl = min(
        (r["eval_perplexity"] for r in results.values() if r is not None),
        default=None
    )

    for variant in ["A", "B", "C"]:
        r = results[variant]
        if r is None:
            print(f"  {'Variant ' + variant:<10} {'(not run yet)':>12}")
            continue
        ppl = r["eval_perplexity"]
        flag = " ←" if (not has_mixed_quality and ppl == best_ppl) else ""
        quality = run_quality(r)
        tokenizer_mode = r.get("tokenizer_mode", "unknown")
        train_cap = r.get("train_seq_len_cap", "?")
        eval_cap = r.get("eval_seq_len_cap", "?")
        representation_id = r.get("representation_id", "unknown")
        print(
            f"  {'Variant ' + variant:<10}"
            f" {ppl:>12.2f}{flag}"
            f" {r['vocab_size']:>8,}"
            f" {r['n_params_M']:>9.0f}M"
            f" {r['training_minutes']:>9.1f}"
            f" {quality:>8}"
            f" {tokenizer_mode:>17}"
            f" {str(train_cap) + '/' + str(eval_cap):>10}"
            f" {representation_id:>24}"
        )

    print()

    print("Loss curves:")
    combined_curves = {}
    for variant in ["A", "B", "C"]:
        r = results[variant]
        if r is None:
            continue
        curve = load_loss_curve(args.runs_dir, variant)
        train_curve = curve["train_loss_curve"]
        eval_curve = curve["eval_loss_curve"]
        combined_curves[variant] = curve
        if train_curve:
            first_train = train_curve[0]["loss"]
            last_train = train_curve[-1]["loss"]
            summary = f"train points={len(train_curve)} ({first_train:.4f}→{last_train:.4f})"
        else:
            summary = "train points=0"
        if eval_curve:
            last_eval = eval_curve[-1]["eval_loss"]
            summary += f", eval points={len(eval_curve)} (last={last_eval:.4f})"
        else:
            summary += ", eval points=0"
        print(f"  Variant {variant}: {summary} [source: {curve['source']}]")

    curves_path = os.path.join(args.runs_dir, "loss_curves.json")
    with open(curves_path, "w") as f:
        json.dump(combined_curves, f, indent=2)
    print(f"  Saved combined curves → {curves_path}")
    print()

    # Interpretation
    completed = [v for v, r in results.items() if r is not None and not r.get("smoke_test")]
    if has_mixed_quality:
        print("Interpretation:")
        print("  Mixed SMOKE/FULL results in table; winner interpretation is suppressed.")
        print("  Wait until A/B/C are all FULL runs for decision-grade comparison.")
    elif len(completed) == 3:
        ppl_A = results["A"]["eval_perplexity"]
        ppl_B = results["B"]["eval_perplexity"]
        ppl_C = results["C"]["eval_perplexity"]
        print("Interpretation:")
        if ppl_A < ppl_B and ppl_A < ppl_C:
            improvement_vs_B = round((ppl_B - ppl_A) / ppl_B * 100, 1)
            print(f"  ✓ Variant A outperforms B and C — IR tokens improve sample efficiency.")
            print(f"    A is {improvement_vs_B}% lower perplexity than B (raw Lean source).")
            print(f"    Preliminary support for the representation hypothesis.")
        elif ppl_A > ppl_B and ppl_A > ppl_C:
            print(f"  ✗ Variant A underperforms — IR tokens hurt at this scale.")
            print(f"    Investigate: custom vocab too small? Cross-graph identity loss?")
            print(f"    See docs/ENCODER_FORMAT.md — Known design constraints.")
        else:
            print(f"  ~ Mixed results. A={ppl_A:.2f}, B={ppl_B:.2f}, C={ppl_C:.2f}")
            print(f"    Corpus may be too small to show a clear signal (n=2,459 training examples).")
            print(f"    Consider expanding to more Mathlib modules before drawing conclusions.")
        print()
        print(f"  Base model:  {results['A']['base_model']}")
        print(f"  Epochs:      {results['A']['epochs']}")
        print(f"  Seed:        {results['A']['seed']}")
        print(f"  Train split: {results['A']['train_examples']} examples")
        print(f"  Eval split:  {results['A']['eval_examples']} examples")
    elif completed:
        remaining = [v for v in ["A", "B", "C"] if v not in completed]
        print(f"  {len(completed)}/3 full runs complete. Still waiting on: {', '.join('Variant ' + v for v in remaining)}")
    else:
        print("  All results are from smoke tests — run full 3-epoch experiments for meaningful comparison.")

    print()
    if args.strict_full:
        missing = [v for v, r in results.items() if r is None]
        smoke = [v for v, r in results.items() if r is not None and r.get("smoke_test")]
        if missing:
            print(f"STRICT_FULL: FAIL — missing results for: {', '.join(missing)}")
            return 1
        if smoke:
            print(f"STRICT_FULL: FAIL — smoke results present for: {', '.join(smoke)}")
            return 1
        print("STRICT_FULL: PASS — all A/B/C results are full runs.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
