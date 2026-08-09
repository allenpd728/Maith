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
    ap.add_argument("--B", default="runs/variant_B_v2_2ep",
                    help="B run dir (default: v2 2-epoch re-run; use runs/variant_B2 for v1-era)")
    ap.add_argument("--C", default="runs/variant_C_v2_2ep",
                    help="C run dir (default: v2 2-epoch re-run; use runs/variant_C_v2 for v1-era)")
    ap.add_argument("--B-small", default="runs/variant_B_small",
                    help="B-small size-control run dir")
    ap.add_argument("--completion-log", default="runs/completion_accuracy_abc_final.json",
                    help="eval_completion.py log or results json")
    ap.add_argument("--gate-baseline", type=float, default=1.2458,
                    help="DEC-026 C1+C2 partial baseline to beat (default 1.2458)")
    ap.add_argument("--bc-baseline", type=float, default=1.11,
                    help="B/C perplexity baseline to beat for the hypothesis (default 1.11)")
    args = ap.parse_args()

    # Load all variants including B-small
    variant_dirs = {"A": args.A, "B": args.B, "C": args.C, "B-small": args.B_small}
    rows = {}
    for v, d in variant_dirs.items():
        rows[v] = load_results(Path(d))

    # completion accuracy: try json first, then log parse
    comp = {}
    cpath = Path(args.completion_log)
    if cpath.exists() and cpath.suffix == ".json":
        try:
            raw = json.load(open(cpath))
            norm = {}
            for k, val in raw.items():
                v = val.get("top1_accuracy") if isinstance(val, dict) else val
                norm[k] = v
            comp = norm
        except Exception:
            comp = parse_completion_log(cpath)
    else:
        comp = parse_completion_log(cpath)

    # Print the 2x3 control grid
    print("═" * 80)
    print("  A/B/C Results — 2×3 Control Grid")
    print("═" * 80)
    print()
    print(f"  {'Variant':<12}{'Repr':>16}{'Vocab':>8}{'Params':>8}{'Epochs':>8}"
          f"{'PPL':>10}{'Top-1':>10}{'Status':>14}")
    print(f"  {'-'*12}{'-'*16}{'-'*8}{'-'*8}{'-'*8}{'-'*10}{'-'*10}{'-'*14}")

    grid_order = [
        ("A",       "IR graph",     "601",     "358M"),
        ("B-small", "BPE trunc",    "601",     "358M"),
        ("B",       "BPE full",     "151k",    "494M"),
        ("C",       "AST BPE",      "151k",    "494M"),
    ]

    for v, repr_label, vocab_label, params_label in grid_order:
        r = rows.get(v)
        if not r:
            print(f"  {v:<12}{repr_label:>16}{vocab_label:>8}{params_label:>8}"
                  f"{'-':>8}{'(missing)':>10}{'-':>10}{'-':>14}")
            continue
        smoke = " (smoke)" if r["smoke"] else ""
        ppl = f"{r['ppl']:.4f}" if r["ppl"] is not None else "-"
        # completion accuracy: B-small stored as "B_SMALL" in some logs
        acc_key = v
        if v == "B-small" and "B_SMALL" in comp:
            acc_key = "B_SMALL"
        top1 = f"{comp.get(acc_key, '-')}%" if acc_key in comp else "-"
        # status: v2-authoritative vs v1-era
        ep = r.get("epochs")
        if v in ("A", "B-small"):
            status = "v2 (auth)" if ep == 2 else "check epochs"
        elif v in ("B", "C"):
            status = "v2 re-run" if ep == 2 else "v1-era (3ep)"
        else:
            status = "-"
        print(f"  {v:<12}{repr_label:>16}{vocab_label:>8}{params_label:>8}"
              f"{str(ep):>8}{ppl+smoke:>10}{top1:>10}{status:>14}")

    print("═" * 80)
    print()

    # DEC-026/027 gate analysis
    a = rows.get("A")
    bs = rows.get("B-small")
    if a and a["ppl"] is not None and not a["smoke"]:
        ppl_a = a["ppl"]
        print("  Gate analysis:")
        print(f"    A perplexity           = {ppl_a:.4f}")
        print(f"    C1+C2 partial baseline = {args.gate_baseline:.4f}  -> "
              f"{'PASS (C4 helped)' if ppl_a < args.gate_baseline else 'no improvement vs C1+C2'}")
        print(f"    B/C baseline           = {args.bc_baseline:.4f}  -> "
              f"{'PASS (beats baselines)' if ppl_a < args.bc_baseline else 'still trails B/C'}")
        if bs and bs["ppl"] is not None and not bs["smoke"]:
            ppl_bs = bs["ppl"]
            print(f"    B-small perplexity     = {ppl_bs:.4f}  -> "
                  f"{'A beats B-small (rep helps)' if ppl_a < ppl_bs else 'A does not beat B-small (size confound)'}")
            # DEC-027 gate: B-small accuracy vs A
            acc_a = comp.get("A")
            acc_bs = comp.get("B_SMALL", comp.get("B-small"))
            if acc_a is not None and acc_bs is not None:
                print(f"    A top-1 = {acc_a}%, B-small top-1 = {acc_bs}%  -> "
                      f"{'IR leads' if acc_a > acc_bs else 'BPE leads' if acc_bs > acc_a else 'tied'} "
                      f"(DEC-027 gate: B-small >= 90% = size confound)")
        print()



if __name__ == "__main__":
    main()
