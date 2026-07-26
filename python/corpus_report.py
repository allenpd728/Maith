#!/usr/bin/env python3
"""
corpus_report.py

Generate a reproducible node-type frequency report from corpus.jsonl and
merge it into Corpus/stats.json. Run once per corpus version to freeze
the analysis alongside the data.

Usage:
    python3 python/corpus_report.py
    python3 python/corpus_report.py --corpus Corpus/corpus.jsonl --stats Corpus/stats.json --top 30
"""

import argparse
import json
import re
from collections import Counter
from pathlib import Path


def load_corpus(path: str) -> list[dict]:
    examples = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                examples.append(json.loads(line))
    return examples


def analyse(examples: list[dict], top_n: int = 20) -> dict:
    """
    Count token-level frequencies across the full corpus and return a report dict.
    Separates structural tokens, gen:* operation types, FVAR/BVAR/TERM usage,
    and TERM_MANY/BVAR_MANY/FVAR_MANY overflow counts.
    """
    token_counter: Counter = Counter()
    gen_counter: Counter = Counter()
    fvar_max = 0
    bvar_max = 0
    term_max = 0
    term_many_count = 0
    bvar_many_count = 0
    fvar_many_count = 0
    total_tokens = 0
    total_examples = len(examples)

    for ex in examples:
        tokens = ex.get("tokens", [])
        total_tokens += len(tokens)
        for tok in tokens:
            token_counter[tok] += 1
            if tok.startswith("gen:"):
                gen_counter[tok] += 1
            elif tok == "TERM_MANY":
                term_many_count += 1
            elif tok == "BVAR_MANY":
                bvar_many_count += 1
            elif tok == "FVAR_MANY":
                fvar_many_count += 1
            elif m := re.fullmatch(r"TERM_(\d+)", tok):
                term_max = max(term_max, int(m.group(1)))
            elif m := re.fullmatch(r"BVAR_(\d+)", tok):
                bvar_max = max(bvar_max, int(m.group(1)))
            elif m := re.fullmatch(r"FVAR_(\d+)", tok):
                fvar_max = max(fvar_max, int(m.group(1)))

    top_gen = [{"token": tok, "count": cnt} for tok, cnt in gen_counter.most_common(top_n)]
    top_all = [{"token": tok, "count": cnt} for tok, cnt in token_counter.most_common(top_n)]

    return {
        "total_examples": total_examples,
        "total_tokens": total_tokens,
        "unique_tokens": len(token_counter),
        "top_gen_tokens": top_gen,
        "top_tokens_overall": top_all,
        "overflow_counts": {
            "TERM_MANY": term_many_count,
            "BVAR_MANY": bvar_many_count,
            "FVAR_MANY": fvar_many_count,
        },
        "max_positional_index_seen": {
            "TERM": term_max,
            "BVAR": bvar_max,
            "FVAR": fvar_max,
        },
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--stats",  default="Corpus/stats.json")
    parser.add_argument("--top",    type=int, default=20, help="Top N tokens to report")
    args = parser.parse_args()

    corpus_path = Path(args.corpus)
    stats_path  = Path(args.stats)

    if not corpus_path.exists():
        print(f"ERROR: corpus not found at {corpus_path}")
        print("Run: lake env ./.lake/build/bin/buildCorpus")
        return

    print(f"Loading corpus from {corpus_path} ...")
    examples = load_corpus(str(corpus_path))
    print(f"  {len(examples)} examples loaded.")

    print(f"Analysing token frequencies (top {args.top}) ...")
    report = analyse(examples, top_n=args.top)

    # Merge into stats.json
    if stats_path.exists():
        with open(stats_path) as f:
            stats = json.load(f)
    else:
        stats = {}

    stats["nodeTypeReport"] = report

    with open(stats_path, "w") as f:
        json.dump(stats, f, indent=2)
    print(f"Report merged into {stats_path}")

    # Human-readable summary
    print()
    print(f"=== Node-type frequency report ===")
    print(f"  Total examples:  {report['total_examples']:,}")
    print(f"  Total tokens:    {report['total_tokens']:,}")
    print(f"  Unique tokens:   {report['unique_tokens']:,}")
    print()
    print(f"  Overflow counts:")
    for k, v in report["overflow_counts"].items():
        pct = v / report["total_tokens"] * 100
        print(f"    {k:<12} {v:>8,}  ({pct:.2f}% of all tokens)")
    print()
    print(f"  Max positional index seen:")
    for k, v in report["max_positional_index_seen"].items():
        print(f"    {k:<6} {v}")
    print()
    print(f"  Top {args.top} gen:* operation tokens:")
    for row in report["top_gen_tokens"][:args.top]:
        print(f"    {row['count']:>8,}  {row['token']}")


if __name__ == "__main__":
    main()
