#!/usr/bin/env python3
"""
build_dataset.py

Builds three dataset variants from corpus.jsonl for the A/B/C experiment:
  A) IR tokens    — Maith v1.0.0 token sequences (our representation)
  B) Lean source  — raw leanExpr strings (BPE baseline)
  C) AST-style    — leanExpr tokenized with a simple structural splitter
                    (splits on whitespace + Lean punctuation: . ( ) @ _ #)

Each variant is exported as a HuggingFace-compatible JSONL with fields:
  { "input_ids": [...], "labels": [...], "source": "A"|"B"|"C",
    "name": "<decl name>", "module": "<module>" }

For variants B and C the tokenizer is Qwen2.5-Coder-1.5B BPE.
For variant A we build a custom vocab from the corpus and map tokens to integer IDs.

Also outputs:
  - datasets/vocab_A.json        — IR token → int ID mapping (variant A)
  - datasets/split_train.jsonl   — 90% train split (same declarations across A/B/C)
  - datasets/split_eval.jsonl    — 10% eval split

Task: next-token prediction. input_ids = full sequence; labels = input_ids shifted
left by 1 (standard causal LM format).

Usage:
    python3 python/build_dataset.py [--corpus Corpus/corpus.jsonl] [--out datasets/]
"""

import json
import argparse
import random
import os
import re
from collections import Counter
from typing import Optional

try:
    from transformers import AutoTokenizer
    HAS_TRANSFORMERS = True
except ImportError:
    HAS_TRANSFORMERS = False


# ---------------------------------------------------------------------------
# Variant A: IR token vocab
# ---------------------------------------------------------------------------

GEN_UNK_THRESHOLD = 5  # gen:* tokens below this frequency → GEN_UNK

def build_ir_vocab(examples: list[dict], freq_threshold: int = GEN_UNK_THRESHOLD) -> dict[str, int]:
    """Build a stable integer vocab from IR token frequencies."""
    vocab_counts = Counter(tok for ex in examples for tok in ex.get("tokens", []))

    # Structural tokens always get fixed low IDs for readability
    structural = [
        "<PAD>", "<BOS>", "<EOS>", "<UNK>", "GEN_UNK",
        "GRAPH_BEGIN", "GRAPH_END",
        "E", "A", "R", "O",
        "pos", "neg", "neut",
        "eq", "add", "sub", "mul", "div", "le", "ge", "lt", "gt", "pow",
        "typeclass", "sort", "literal",
    ] + [f"BVAR_{i}" for i in range(32)] + ["BVAR_MANY"] \
      + [f"TERM_{i}" for i in range(32)] + ["TERM_MANY"]

    vocab = {tok: i for i, tok in enumerate(structural)}
    next_id = len(vocab)

    # Add remaining tokens in frequency order (stable: sort by (-freq, token))
    for tok, count in sorted(vocab_counts.items(), key=lambda x: (-x[1], x[0])):
        if tok in vocab:
            continue
        # Rare gen:* tokens collapse to GEN_UNK (already in vocab)
        if tok.startswith("gen:") and count < freq_threshold:
            continue
        vocab[tok] = next_id
        next_id += 1

    return vocab


def encode_ir(tokens: list[str], vocab: dict[str, int]) -> list[int]:
    unk_id = vocab.get("<UNK>", 3)
    gen_unk_id = vocab.get("GEN_UNK", 4)
    result = []
    for tok in tokens:
        if tok in vocab:
            result.append(vocab[tok])
        elif tok.startswith("gen:"):
            result.append(gen_unk_id)
        else:
            result.append(unk_id)
    return result


# ---------------------------------------------------------------------------
# Variant C: simple AST-style tokenizer
# ---------------------------------------------------------------------------

def ast_tokenize(expr: str) -> list[str]:
    """
    Split a Lean expression string into structural tokens without BPE.
    Splits on whitespace and Lean punctuation: . ( ) @ _ # : ,
    Preserves the pieces as tokens. Empty pieces are dropped.
    """
    pieces = re.split(r'([\s.()\[\]@#:,]+)', expr)
    return [p for p in pieces if p.strip()]


# ---------------------------------------------------------------------------
# Dataset builder
# ---------------------------------------------------------------------------

def make_labels(input_ids: list[int]) -> list[int]:
    """Standard causal LM: labels = input_ids shifted left, last position = -100."""
    return input_ids[1:] + [-100]


def build_variant_A(examples: list[dict], vocab: dict[str, int]) -> list[dict]:
    rows = []
    for ex in examples:
        ids = encode_ir(ex.get("tokens", []), vocab)
        rows.append({
            "source": "A",
            "name": ex.get("name", ""),
            "module": ex.get("module", ""),
            "input_ids": ids,
            "labels": make_labels(ids),
            "seq_len": len(ids),
        })
    return rows


def build_variant_B(examples: list[dict], tokenizer) -> list[dict]:
    rows = []
    for ex in examples:
        text = ex.get("leanExpr", "")
        ids = tokenizer.encode(text)
        rows.append({
            "source": "B",
            "name": ex.get("name", ""),
            "module": ex.get("module", ""),
            "input_ids": ids,
            "labels": make_labels(ids),
            "seq_len": len(ids),
        })
    return rows


def build_variant_C(examples: list[dict], tokenizer) -> list[dict]:
    rows = []
    for ex in examples:
        text = ex.get("leanExpr", "")
        pieces = ast_tokenize(text)
        # Re-join and BPE-tokenize each piece individually for cleaner boundaries
        ids = []
        for piece in pieces:
            ids.extend(tokenizer.encode(piece, add_special_tokens=False))
        rows.append({
            "source": "C",
            "name": ex.get("name", ""),
            "module": ex.get("module", ""),
            "input_ids": ids,
            "labels": make_labels(ids),
            "seq_len": len(ids),
        })
    return rows


def write_jsonl(path: str, rows: list[dict]) -> None:
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        for row in rows:
            f.write(json.dumps(row) + "\n")
    print(f"  Wrote {len(rows)} rows → {path}")


def run(corpus_path: str, out_dir: str, seed: int = 42) -> None:
    print(f"Loading corpus from {corpus_path} ...")
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]
    print(f"Loaded {len(examples)} examples.")
    print()

    # Deterministic split — fixed seed, same split used for A, B, C
    random.seed(seed)
    shuffled = examples[:]
    random.shuffle(shuffled)
    split = int(len(shuffled) * 0.9)
    train_examples = shuffled[:split]
    eval_examples  = shuffled[split:]
    print(f"Split: {len(train_examples)} train / {len(eval_examples)} eval (seed={seed})")
    print()

    # Build IR vocab from training set only (no test leakage)
    print("Building IR vocab (variant A) from training set ...")
    vocab = build_ir_vocab(train_examples, freq_threshold=GEN_UNK_THRESHOLD)
    vocab_path = os.path.join(out_dir, "vocab_A.json")
    os.makedirs(out_dir, exist_ok=True)
    with open(vocab_path, "w") as f:
        json.dump(vocab, f, indent=2)
    print(f"  Vocab size: {len(vocab)} tokens → {vocab_path}")
    print()

    # Load BPE tokenizer for B and C
    if not HAS_TRANSFORMERS:
        print("WARNING: transformers not installed — skipping variants B and C.")
        print("  Run: pip install transformers")
        tokenizer = None
    else:
        model = "Qwen/Qwen2.5-Coder-1.5B"
        print(f"Loading tokenizer ({model}) ...")
        tokenizer = AutoTokenizer.from_pretrained(model, trust_remote_code=True)
        print(f"  Tokenizer vocab size: {tokenizer.vocab_size:,}")
        print()

    # Build datasets
    print("Building variant A (IR tokens) ...")
    train_A = build_variant_A(train_examples, vocab)
    eval_A  = build_variant_A(eval_examples,  vocab)

    if tokenizer:
        print("Building variant B (Lean source → BPE) ...")
        train_B = build_variant_B(train_examples, tokenizer)
        eval_B  = build_variant_B(eval_examples,  tokenizer)

        print("Building variant C (AST-style → BPE) ...")
        train_C = build_variant_C(train_examples, tokenizer)
        eval_C  = build_variant_C(eval_examples,  tokenizer)
    else:
        train_B = eval_B = train_C = eval_C = []

    # Write splits — interleaved A/B/C so any random slice has all three
    print()
    print("Writing dataset files ...")
    write_jsonl(os.path.join(out_dir, "train_A.jsonl"), train_A)
    write_jsonl(os.path.join(out_dir, "eval_A.jsonl"),  eval_A)
    if tokenizer:
        write_jsonl(os.path.join(out_dir, "train_B.jsonl"), train_B)
        write_jsonl(os.path.join(out_dir, "eval_B.jsonl"),  eval_B)
        write_jsonl(os.path.join(out_dir, "train_C.jsonl"), train_C)
        write_jsonl(os.path.join(out_dir, "eval_C.jsonl"),  eval_C)

    # Summary stats
    print()
    print("=== Dataset summary ===")
    for label, rows in [("A train", train_A), ("A eval", eval_A),
                        ("B train", train_B), ("B eval", eval_B),
                        ("C train", train_C), ("C eval", eval_C)]:
        if not rows:
            continue
        lengths = [r["seq_len"] for r in rows]
        import statistics
        print(f"  {label}: {len(rows)} examples  "
              f"median_len={statistics.median(lengths):.0f}  "
              f"max_len={max(lengths)}")

    print()
    print("Done. Next step: fine-tune a small model on each variant and compare perplexity.")
    print("  Recommended: Qwen2.5-Coder-1.5B (variant A uses custom vocab; B/C use native BPE)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--out", default="datasets/")
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    run(args.corpus, args.out, args.seed)
