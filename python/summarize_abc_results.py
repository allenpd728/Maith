#!/usr/bin/env python3
"""
summarize_abc_results.py

Print a formatted A/B/C comparison table from the run dirs + completion eval
output, including perplexity, completion accuracy, and the DEC-026 gate
pass/fail. Lets you drop final numbers into docs without manual formatting.

Reads:
  - runs/variant_*/results.json            -> eval_perplexity, epochs, vocab
  - runs/completion_accuracy.json (optional) -> top-1 completion accuracy
  - or a completion eval log path passed via --completion-log

Usage:
    python3 python/summarize_abc_results.py
    python3 python/summarize_abc_results.py \\
        --A runs/variant_A_v2_full --B runs/variant_B2 --C runs/variant_C_v2 \\
        --completion-log eval_c_correct.log

Exit 0 always (this is a reporting tool, not a test).
"""

import argparse
import json
import os
import re
import sys
from pathlib import Path


def load_results(run_dir: Path):
    """Return (perplexity, epochs, vocab, smoke) from a run's results.json, or None."""
    rp = run_dir / "results.json"
    if not rp.exists():
        return None
    try:
        r = json.load(open(rp))
        return {
            "ppl": r.get("eval_perplexity"),
            "epochs": r.get("epochs"),
            "vocab": r.get("vocab_size"),
            "smoke": r.get("smoke_test", False),
            "train": r.get("train_examples"),
        }
    except Exception:
        return None


def parse_completion_log(path: Path) -> dict:
    """Parse top-1 accuracy per variant from an eval_completion.py log."""
    if not path or not path.exists():
        return {}
    text = path.read_text()
    out = {}
    # match lines like: "    Top-1 accuracy: 91.7%  (1816/1980 tokens)"
    # under a "── Variant X ──" header
    cur = None
    for line in text.splitlines():
        m = re.search(r"Variant ([ABC])", line)
        if m and "──" in line:
            cur = m.group(1)
        m2 = re.search(r"Top-1 accuracy:\s*([\d.]+)%", line)
        if m2 and cur:
            out[cur] = float(m2.group(1))
            cur = None
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--A", default="runs/variant_A_v2_full")
    ap.add_argument("--B", default="runs/variant_B2")
    ap.add_argument("--C", default="runs/variant_C_v2")
    ap.add_argument("--completion-log", default="runs/completion_accuracy.json",
                    help="eval_completion.py log or results json")
    ap.add_argument("--gate-baseline", type=float, default=1.2458,
                    help="DEC-026 C1+C2 partial baseline to beat (default 1.2458)")
    ap.add_argument("--bc-baseline", type=float, default=1.11,
                    help="B/C perplexity baseline to beat for the hypothesis (default 1.11)")
    args = ap.parse_args()

    rows = {}
    for v, d in (("A", args.A), ("B", args.B), ("C", args.C)):
        rows[v] = load_results(Path(d))

    # completion accuracy: try json first, then log parse
    comp = {}
    cpath = Path(args.completion_log)
    if cpath.exists() and cpath.suffix == ".json":
        try:
            comp = json.load(open(cpath))
            # normalize: expect {variant: {top1_accuracy: ...}} or {variant: pct}
            norm = {}
            for k, val in comp.items():
                v = val.get("top1_accuracy") if isinstance(val, dict) else val
                norm[k] = v
            comp = norm
        except Exception:
            comp = parse_completion_log(cpath)
    else:
        comp = parse_completion_log(cpath)

    print("═" * 72)
    print("  A/B/C Results Summary")
    print("═" * 72)
    print(f"  {'Variant':<10}{'PPL':>10}{'Epochs':>8}{'Vocab':>8}{'Top-1':>10}{'Dir':>22}")
    print(f"  {'-'*10}{'-'*10}{'-'*8}{'-'*8}{'-'*10}{'-'*22}")
    dirs = {"A": args.A, "B": args.B, "C": args.C}
    for v in ("A", "B", "C"):
        r = rows[v]
        if not r:
            print(f"  {v:<10}{'(missing)':>10}")
            continue
        smoke = " (smoke)" if r["smoke"] else ""
        ppl = f"{r['ppl']:.4f}" if r["ppl"] is not None else "-"
        top1 = f"{comp.get(v, '-')}%" if v in comp else "-"
        print(f"  {v:<10}{ppl+smoke:>10}{str(r['epochs']):>8}{str(r['vocab']):>8}{top1:>10}{dirs[v]:>22}")
    print("═" * 72)

    # DEC-026 gate
    a = rows.get("A")
    if a and a["ppl"] is not None and not a["smoke"]:
        ppl = a["ppl"]
        print()
        print("  DEC-026 gate:")
        print(f"    A perplexity           = {ppl:.4f}")
        print(f"    C1+C2 partial baseline = {args.gate_baseline:.4f}  -> "
              f"{'PASS ✓ (C4 helped)' if ppl < args.gate_baseline else 'no improvement vs C1+C2'}")
        print(f"    B/C baseline           = {args.bc_baseline:.4f}  -> "
              f"{'PASS ✓ (beats baselines — hypothesis supported)' if ppl < args.bc_baseline else 'still trails B/C'}")
    print()


if __name__ == "__main__":
    main()
