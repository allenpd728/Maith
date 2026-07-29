#!/usr/bin/env python3
"""
tokenizer_study.py

Tokenizer fragmentation study: Maith IR tokens vs Qwen2.5-Coder BPE.

Compares three representations of the same Mathlib declarations:
  A) Maith IR tokens (our format, v1.0.0)
  B) Raw Lean source expression string (leanExpr field)
  C) Structured IR token string (IR tokens joined as space-separated text)

For each, measures:
  - Sequence length distribution (min/median/p95/max)
  - Vocabulary size
  - Fragmentation ratio (BPE tokens per IR token)

Requires:
  pip install transformers

Usage:
  python3 python/tokenizer_study.py [--corpus Corpus/corpus.jsonl] [--sample 500]
"""

import json
import argparse
import statistics
import sys
from collections import Counter

try:
    from transformers import AutoTokenizer
except ImportError:
    print("ERROR: transformers not installed. Run: pip install transformers")
    sys.exit(1)


MODEL = "Qwen/Qwen2.5-Coder-0.5B"


def load_sample(corpus_path: str, n: int) -> list[dict]:
    import random
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]
    random.seed(42)
    return random.sample(examples, min(n, len(examples)))


def seq_stats(lengths: list[int]) -> dict:
    s = sorted(lengths)
    return {
        "min":    s[0],
        "median": s[len(s) // 2],
        "p95":    s[int(len(s) * 0.95)],
        "max":    s[-1],
        "mean":   round(statistics.mean(s), 1),
        "total":  sum(s),
    }


def print_stats(label: str, lengths: list[int]) -> None:
    st = seq_stats(lengths)
    print(f"  {label}")
    print(f"    min={st['min']}  median={st['median']}  p95={st['p95']}  max={st['max']}  mean={st['mean']}")
    print(f"    total tokens across sample: {st['total']:,}")


def run(corpus_path: str, sample_size: int) -> None:
    print(f"Loading sample of {sample_size} examples from {corpus_path} ...")
    examples = load_sample(corpus_path, sample_size)
    n = len(examples)
    print(f"Loaded {n} examples.")
    print()

    print(f"Loading Qwen2.5-Coder tokenizer ({MODEL}) ...")
    print("  (first run downloads ~1MB vocab file)")
    tokenizer = AutoTokenizer.from_pretrained(MODEL, trust_remote_code=True)
    print("  Tokenizer loaded.")
    print()

    # --- Representation A: Maith IR token sequences (already tokenized) ---
    ir_lengths = [len(ex["tokens"]) for ex in examples]
    ir_vocab = Counter(tok for ex in examples for tok in ex["tokens"])

    # --- Representation B: Raw Lean expression string → BPE ---
    lean_texts = [ex.get("leanExpr", "") for ex in examples]
    lean_bpe_lengths = [len(tokenizer.encode(t)) for t in lean_texts]

    # --- Representation C: IR tokens joined as space-separated string → BPE ---
    ir_as_text = [" ".join(ex["tokens"]) for ex in examples]
    ir_bpe_lengths = [len(tokenizer.encode(t)) for t in ir_as_text]

    # --- Fragmentation: how many BPE tokens per IR token? ---
    # For each example: bpe_len / ir_len
    lean_frag = [b / i for b, i in zip(lean_bpe_lengths, ir_lengths) if i > 0]
    ir_frag   = [b / i for b, i in zip(ir_bpe_lengths,  ir_lengths) if i > 0]

    # --- Spot-check: show BPE tokenization of a few IR-specific tokens ---
    probe_tokens = [
        "HMul.hMul", "gen:OfNat.ofNat", "Lean.Literal.natVal 0",
        "gen:hof", "BVAR_0", "TERM_3", "GRAPH_BEGIN",
        "inputs:BVAR_0,BVAR_1", "output:TERM_0",
    ]

    print("=" * 60)
    print("RESULTS")
    print("=" * 60)
    print()
    print(f"Sample size: {n} examples")
    print()

    print("--- Sequence length distribution ---")
    print_stats("A) Maith IR tokens (our format)", ir_lengths)
    print_stats("B) Lean expr string → Qwen2.5 BPE", lean_bpe_lengths)
    print_stats("C) IR tokens as text → Qwen2.5 BPE", ir_bpe_lengths)
    print()

    print("--- Fragmentation ratio (BPE tokens / IR tokens) ---")
    print(f"  B/A (Lean source → BPE vs IR):  median={statistics.median(lean_frag):.2f}x  mean={statistics.mean(lean_frag):.2f}x")
    print(f"  C/A (IR-as-text → BPE vs IR):   median={statistics.median(ir_frag):.2f}x   mean={statistics.mean(ir_frag):.2f}x")
    print()
    print("  Inflation ratio < 1.0 means IR is more token-efficient than BPE.")
    print("  Inflation ratio > 1.0 means BPE produces more tokens for the same content.")
    print()

    print("--- Vocabulary ---")
    print(f"  Maith IR unique tokens (full corpus):  5,577")
    print(f"  Maith IR unique tokens (this sample):  {len(ir_vocab):,}")
    print(f"  Qwen2.5-Coder vocab size:              {tokenizer.vocab_size:,}")
    print()

    print("--- BPE fragmentation of IR-specific tokens ---")
    for tok in probe_tokens:
        bpe = tokenizer.encode(tok)
        pieces = tokenizer.convert_ids_to_tokens(bpe)
        print(f"  {tok!r:35s} → {len(bpe)} BPE tokens: {pieces}")
    print()

    print("--- Top 10 IR tokens by frequency (sample) ---")
    for tok, count in ir_vocab.most_common(10):
        bpe_len = len(tokenizer.encode(tok))
        print(f"  {count:>6}x  {tok!r:30s}  BPE: {bpe_len} token(s)")
    print()

    # Summary verdict
    median_inflation = statistics.median(lean_frag)
    print("--- Summary ---")
    if median_inflation > 2.0:
        verdict = "HIGH fragmentation: BPE struggles significantly with Lean identifiers."
    elif median_inflation > 1.2:
        verdict = "MODERATE fragmentation: BPE handles some tokens well, fragments others."
    else:
        verdict = "LOW fragmentation: BPE handles Lean source reasonably well."
    print(f"  Lean source BPE inflation: {median_inflation:.2f}x — {verdict}")
    print()
    print("  IR token format advantages:")
    print("  - Fixed vocab of ~5,577 tokens (vs 150k for Qwen2.5-Coder)")
    print("  - Structural tokens (GRAPH_BEGIN, E, R, O) are always single tokens")
    print("  - BVAR_N / TERM_N are single tokens; BPE may split them")
    print("  - No subword fragmentation of mathematical identifiers")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--sample", type=int, default=500)
    args = parser.parse_args()
    run(args.corpus, args.sample)
