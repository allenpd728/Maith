#!/usr/bin/env python3
"""
publish_results_summary.py

Build a publish-ready summary from full A/B/C run artifacts.
Reads:
  - runs/variant_{A,B,C}/results.json
  - runs/variant_{A,B,C}/loss_curve.json (if present)
  - runs/full_results_gate.json (if present)

Writes:
  - runs/publish_results_summary.json
"""

import argparse
from typing import Optional
import json
from typing import Optional
from pathlib import Path


def load_json(path: Path) -> Optional[dict]:
    if not path.exists():
        return None
    with open(path) as f:
        return json.load(f)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate publish-ready A/B/C result summary.")
    parser.add_argument("--runs-dir", default="runs/")
    parser.add_argument("--out", default=None, help="Output JSON path")
    args = parser.parse_args()

    runs_dir = Path(args.runs_dir)
    out_path = Path(args.out) if args.out else runs_dir / "publish_results_summary.json"

    variants = ["A", "B", "C"]
    results = {v: load_json(runs_dir / f"variant_{v}" / "results.json") for v in variants}
    curves = {v: load_json(runs_dir / f"variant_{v}" / "loss_curve.json") for v in variants}
    gate = load_json(runs_dir / "full_results_gate.json")

    missing = [v for v, r in results.items() if r is None]
    smoke = [v for v, r in results.items() if r is not None and r.get("smoke_test")]
    decision_grade = bool(gate and gate.get("decision_grade"))

    ppl = {
        v: (results[v].get("eval_perplexity") if results[v] is not None else None)
        for v in variants
    }
    full_variants = [v for v in variants if results[v] is not None and not results[v].get("smoke_test")]
    winner = None
    if len(full_variants) == 3:
        winner = min(variants, key=lambda v: ppl[v])

    curve_summary = {}
    for v in variants:
        c = curves[v] or {}
        train = c.get("train_loss_curve", [])
        evalc = c.get("eval_loss_curve", [])
        curve_summary[v] = {
            "train_points": len(train),
            "eval_points": len(evalc),
            "train_first": train[0]["loss"] if train else None,
            "train_last": train[-1]["loss"] if train else None,
            "eval_last": evalc[-1]["eval_loss"] if evalc else None,
        }

    summary = {
        "decision_grade": decision_grade,
        "missing_results": missing,
        "smoke_variants": smoke,
        "winner_by_perplexity": winner,
        "variants": {
            v: {
                "eval_perplexity": ppl[v],
                "training_minutes": results[v].get("training_minutes") if results[v] else None,
                "quality": ("FULL" if results[v] and not results[v].get("smoke_test") else "SMOKE") if results[v] else "MISSING",
                "tokenizer_mode": results[v].get("tokenizer_mode") if results[v] else None,
                "train_seq_len_cap": results[v].get("train_seq_len_cap") if results[v] else None,
                "eval_seq_len_cap": results[v].get("eval_seq_len_cap") if results[v] else None,
                "loss_curve": curve_summary[v],
            }
            for v in variants
        },
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w") as f:
        json.dump(summary, f, indent=2)

    print(f"Summary written: {out_path}")
    print()
    print("=== Publish Results Summary ===")
    print(f"Decision-grade: {'YES' if decision_grade else 'NO'}")
    if missing:
        print(f"Missing: {', '.join(missing)}")
    if smoke:
        print(f"Smoke variants: {', '.join(smoke)}")
    for v in variants:
        r = summary["variants"][v]
        print(
            f"Variant {v}: ppl={r['eval_perplexity']} "
            f"quality={r['quality']} "
            f"time={r['training_minutes']}m "
            f"caps={r['train_seq_len_cap']}/{r['eval_seq_len_cap']}"
        )
    if winner:
        print(f"Winner (lower perplexity): Variant {winner}")
    print()
    print("Markdown block:")
    print("```text")
    print(f"Decision-grade: {'YES' if decision_grade else 'NO'}")
    for v in variants:
        r = summary["variants"][v]
        print(f"- Variant {v}: ppl={r['eval_perplexity']} ({r['quality']})")
    if winner:
        print(f"- Winner: Variant {winner}")
    print("```")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
