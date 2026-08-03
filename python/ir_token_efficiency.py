#!/usr/bin/env python3
"""
ir_token_efficiency.py

Token efficiency audit for Maith IR (Variant A) vs BPE (Variant B).

Computes for each variant:
- Mean, median, p95, p99 token sequence length
- Distribution of sequence lengths in 5 buckets
- For Variant A: frequency breakdown of token types
  (structural tokens vs FVAR_N/BVAR_N/TERM_N vs gen:* named constants vs GEN_UNK)

Prints a side-by-side comparison table.
"""

import json
import argparse
import statistics
from collections import Counter
from pathlib import Path


def load_dataset(path: str) -> list[dict]:
    """Load a dataset JSONL file."""
    rows = []
    with open(path) as f:
        for line in f:
            if line.strip():
                rows.append(json.loads(line))
    return rows


def load_vocab(path: str) -> dict[str, int]:
    """Load vocab JSON file (token -> int ID)."""
    with open(path) as f:
        return json.load(f)


def reverse_vocab(vocab: dict[str, int]) -> dict[int, str]:
    """Reverse vocab to get int ID -> token."""
    return {v: k for k, v in vocab.items()}


def compute_length_stats(lengths: list[int]) -> dict:
    """Compute summary statistics for sequence lengths."""
    if not lengths:
        return {}
    return {
        "count": len(lengths),
        "mean": statistics.mean(lengths),
        "median": statistics.median(lengths),
        "p95": sorted(lengths)[int(len(lengths) * 0.95)] if len(lengths) > 1 else lengths[0],
        "p99": sorted(lengths)[int(len(lengths) * 0.99)] if len(lengths) > 1 else lengths[0],
        "min": min(lengths),
        "max": max(lengths),
    }


def bucket_distribution(lengths: list[int], buckets: list[tuple]) -> dict[str, int]:
    """
    Count sequences in each bucket.
    buckets: list of (label, min_exclusive, max_inclusive)
    """
    counts = {b[0]: 0 for b in buckets}
    for length in lengths:
        for label, lo, hi in buckets:
            if lo < length <= hi:
                counts[label] += 1
                break
        else:
            # If no bucket matched, count in the last bucket
            counts[buckets[-1][0]] += 1
    return counts


def classify_variant_a_token(token_str: str) -> str:
    """Classify a Variant A token into a type category."""
    if token_str in ("GRAPH_BEGIN", "GRAPH_END"):
        return "structural_graph"
    if token_str in ("E", "A", "R", "O"):
        return "structural_row_type"
    if token_str in ("pos", "neg", "neut"):
        return "polarity"
    if token_str in ("eq", "add", "sub", "mul", "div", "le", "ge", "lt", "gt", "pow"):
        return "rel_op"
    if token_str in ("typeclass", "sort", "literal"):
        return "attribute_key"
    if token_str.startswith("FVAR_"):
        return "fvar_binder"
    if token_str.startswith("BVAR_"):
        return "bvar_binder"
    if token_str.startswith("TERM_"):
        return "term_ref"
    if token_str.startswith("gen:"):
        return "gen_named_constant"
    if token_str in ("GEN_UNK",):
        return "gen_unk"
    if token_str.startswith("inputs:") or token_str.startswith("output:"):
        return "io_marker"
    if token_str.startswith("inputs:") or token_str.startswith("output:"):
        return "io_marker"
    if token_str.startswith("inputs:"):
        return "io_marker"
    if token_str.startswith("output:"):
        return "io_marker"
    return "other"


def analyze_variant_a_tokens(rows: list[dict], vocab_path: str) -> dict:
    """Analyze token type distribution in Variant A."""
    vocab = load_vocab(vocab_path)
    rev_vocab = reverse_vocab(vocab)
    
    token_counts = Counter()
    token_examples = Counter()  # count of examples containing each token type
    
    type_examples = Counter()
    
    for row in rows:
        input_ids = row.get("input_ids", [])
        tokens = [rev_vocab.get(tid, f"<ID:{tid}>") for tid in input_ids]
        
        seen_in_example = set()
        
        for tok in tokens:
            token_counts[tok] += 1
            category = classify_variant_a_token(tok)
            token_examples[(category, tok)] += 1
            if category not in seen_in_example:
                type_examples[category] += 1
                seen_in_example.add(category)
    
    # Summarize by category
    category_total = Counter()
    category_unique = Counter()
    for (cat, tok), count in token_examples.items():
        category_total[cat] += count
        category_unique[cat] += 1
    
    return {
        "total_tokens": sum(token_counts.values()),
        "unique_tokens": len(token_counts),
        "by_category": {
            cat: {
                "total_count": category_total[cat],
                "unique_tokens": category_unique[cat],
                "examples_with": type_examples[cat],
                "pct_of_total": 100 * category_total[cat] / sum(token_counts.values()) if token_counts else 0,
            }
            for cat in sorted(category_total.keys())
        },
        "top_tokens": dict(token_counts.most_common(20)),
    }


def print_comparison_table(stats_a: dict, stats_b: dict):
    """Print a side-by-side comparison table."""
    print("=" * 80)
    print("TOKEN EFFICIENCY COMPARISON: Variant A (IR) vs Variant B (BPE)")
    print("=" * 80)
    
    # Length statistics
    print("\n### Sequence Length Statistics")
    print("-" * 60)
    print(f"{'Metric':<20} {'Variant A (IR)':<20} {'Variant B (BPE)':<20}")
    print("-" * 60)
    
    for key in ["count", "mean", "median", "p95", "p99", "min", "max"]:
        val_a = stats_a.get(key, "N/A")
        val_b = stats_b.get(key, "N/A")
        if isinstance(val_a, float):
            val_a = f"{val_a:.1f}"
        if isinstance(val_b, float):
            val_b = f"{val_b:.1f}"
        print(f"{key:<20} {str(val_a):<20} {str(val_b):<20}")
    
    # Bucket distribution
    print("\n### Sequence Length Distribution (buckets)")
    print("-" * 60)
    print(f"{'Bucket':<30} {'Variant A':<12} {'Variant B':<12} {'A %':<10} {'B %':<10}")
    print("-" * 60)
    
    total_a = stats_a.get("count", 1)
    total_b = stats_b.get("count", 1)
    
    for label in ["1-50", "51-100", "101-200", "201-500", ">500"]:
        count_a = stats_a.get("buckets", {}).get(label, 0)
        count_b = stats_b.get("buckets", {}).get(label, 0)
        pct_a = 100 * count_a / total_a if total_a else 0
        pct_b = 100 * count_b / total_b if total_b else 0
        print(f"{label:<30} {count_a:<12} {count_b:<12} {pct_a:<10.1f} {pct_b:<10.1f}")


def print_variant_a_token_breakdown(analysis: dict):
    """Print detailed token type breakdown for Variant A."""
    print("\n### Variant A Token Type Breakdown")
    print("-" * 80)
    print(f"Total tokens: {analysis['total_tokens']:,}")
    print(f"Unique token types: {analysis['unique_tokens']}")
    print()
    print(f"{'Category':<30} {'Total':<10} {'%':<8} {'Unique':<10} {'Examples':<10}")
    print("-" * 80)
    
    by_cat = analysis["by_category"]
    for cat in sorted(by_cat.keys(), key=lambda x: -by_cat[x]["total_count"]):
        info = by_cat[cat]
        print(f"{cat:<30} {info['total_count']:<10,} {info['pct_of_total']:<8.2f} {info['unique_tokens']:<10} {info['examples_with']:<10}")
    
    print("\n### Top 20 Most Frequent Tokens")
    print("-" * 40)
    for tok, count in analysis["top_tokens"].items():
        print(f"  {tok:<35} {count:>10,}")


def main():
    parser = argparse.ArgumentParser(description="Token efficiency audit for Maith IR vs BPE")
    parser.add_argument("--dataset-dir", default="datasets/", help="Directory containing dataset files")
    parser.add_argument("--train-a", default="datasets/train_A.jsonl", help="Path to train_A.jsonl")
    parser.add_argument("--train-b", default="datasets/train_B.jsonl", help="Path to train_B.jsonl")
    parser.add_argument("--vocab-a", default="datasets/vocab_A.json", help="Path to vocab_A.json")
    args = parser.parse_args()
    
    print(f"Loading Variant A dataset: {args.train_a}")
    rows_a = load_dataset(args.train_a)
    print(f"  Loaded {len(rows_a)} examples")
    
    print(f"Loading Variant B dataset: {args.train_b}")
    rows_b = load_dataset(args.train_b)
    print(f"  Loaded {len(rows_b)} examples")
    
    # Compute length statistics
    lengths_a = [r.get("seq_len", len(r.get("input_ids", []))) for r in rows_a]
    lengths_b = [r.get("seq_len", len(r.get("input_ids", []))) for r in rows_b]
    
    stats_a = compute_length_stats(lengths_a)
    stats_b = compute_length_stats(lengths_b)
    
    # Bucket distribution
    buckets = [("1-50", 0, 50), ("51-100", 50, 100), ("101-200", 100, 200), 
               ("201-500", 200, 500), (">500", 500, float("inf"))]
    stats_a["buckets"] = bucket_distribution(lengths_a, buckets)
    stats_b["buckets"] = bucket_distribution(lengths_b, buckets)
    
    # Print comparison
    print_comparison_table(stats_a, stats_b)
    
    # Variant A token breakdown
    print(f"\nAnalyzing Variant A token types...")
    analysis_a = analyze_variant_a_tokens(rows_a, args.vocab_a)
    print_variant_a_token_breakdown(analysis_a)
    
    # Summary
    print("\n" + "=" * 80)
    print("SUMMARY")
    print("=" * 80)
    
    ratio = stats_a.get("mean", 0) / stats_b.get("mean", 1) if stats_b.get("mean", 0) else 0
    print(f"\nVariant A mean length: {stats_a.get('mean', 0):.1f}")
    print(f"Variant B mean length: {stats_b.get('mean', 0):.1f}")
    print(f"A/B length ratio: {ratio:.2f}x")
    
    if ratio > 1.0:
        print(f"\n⚠ IR sequences are {ratio:.2f}x longer than BPE on average.")
        print("  This suggests the IR may be expanding noise rather than compressing signal.")
        print("  The extra tokens could be:")
        print("  - Repeated structural markers (GRAPH_BEGIN, row types, etc.)")
        print("  - Redundant polarity tokens (mostly 'neut')")
        print("  - Over-specified attribute annotations")
    else:
        print(f"\n✓ IR sequences are shorter than BPE (ratio: {ratio:.2f}x)")


if __name__ == "__main__":
    main()
