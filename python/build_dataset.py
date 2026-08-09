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

For variants B and C the tokenizer is Qwen2.5-Coder-0.5B BPE.
For variant A we build a custom vocab from the corpus and map tokens to integer IDs.

Also outputs:
  - datasets/vocab_A.json        — IR token → int ID mapping (variant A)
  - datasets/split_train.jsonl   — 90% train split (same declarations across A/B/C)
  - datasets/split_eval.jsonl    — 10% eval split

Task: next-token prediction. input_ids = full sequence; labels = input_ids.
(`AutoModelForCausalLM` performs the causal shift internally during loss computation.)

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
    # v2 IR tokens: typeclass_name (C2) and GEN_* bucket tokens (C4)
    structural = [
        "<PAD>", "<BOS>", "<EOS>", "<UNK>", "GEN_UNK",
        "GRAPH_BEGIN", "GRAPH_END",
        "E", "A", "R", "O",
        "pos", "neg", "neut",
        "eq", "add", "sub", "mul", "div", "le", "ge", "lt", "gt", "pow",
        "typeclass", "sort", "literal",
        # v2 IR tokens (C2: typeclass_name, C4: gen buckets)
        "typeclass_name",
        "GEN_MATHLIB", "GEN_ALGEBRA", "GEN_ORDER", "GEN_TOPOLOGY", "GEN_ANALYSIS",
        "GEN_LOGIC", "GEN_DATA", "GEN_LEAN", "GEN_INIT", "GEN_STD", "GEN_UNKNOWN",
    ] + [f"FVAR_{i}" for i in range(64)] + ["FVAR_MANY"] \
      + [f"BVAR_{i}" for i in range(64)] + ["BVAR_MANY"] \
      + [f"TERM_{i}" for i in range(64)] + ["TERM_MANY"] \
      + [f"IN_{i}" for i in range(10)] + ["IN_MANY"] \
      + [f"OUT_{i}" for i in range(64)] + ["OUT_MANY", "OUT_VAR"]

    vocab = {tok: i for i, tok in enumerate(structural)}
    next_id = len(vocab)

    # Add remaining tokens in frequency order (stable: sort by (-freq, token))
    for tok, count in sorted(vocab_counts.items(), key=lambda x: (-x[1], x[0])):
        if tok in vocab:
            continue
        # v1 gen:* tokens (gen:FullyQualifiedName) are replaced by stable bucket tokens
        # in v2 (GEN_ALGEBRA, GEN_ORDER, etc.). Skip any residual gen:* entries.
        if tok.startswith("gen:"):
            continue
        vocab[tok] = next_id
        next_id += 1

    return vocab


def bucket_from_module(module: str) -> str:
    """Map a declaration's module to a GEN_* bucket.

    Replicates `bucketFromModule` in Maith/MetaExtractor.lean exactly so the
    Python-side token encoding stays consistent with the Lean-side design (C4).
    Bucketing is by the *declaration's* module (declModule), not the gen token's
    own namespace — matching the Lean extractor's behaviour.
    """
    parts = module.split(".")
    if len(parts) >= 2 and parts[0] == "Mathlib":
        second = parts[1].upper()
        if second in ("ALGEBRA",):
            return "GEN_ALGEBRA"
        if second in ("ORDER",):
            return "GEN_ORDER"
        if second in ("TOPOLOGY",):
            return "GEN_TOPOLOGY"
        if second in ("ANALYSIS",):
            return "GEN_ANALYSIS"
        if second in ("LOGIC", "TACTIC"):
            return "GEN_LOGIC"
        if second in ("DATA", "COMBINATORICS"):
            return "GEN_DATA"
        return "GEN_MATHLIB"
    if parts:
        first = parts[0].upper()
        if first == "LEAN":
            return "GEN_LEAN"
        if first == "INIT":
            return "GEN_INIT"
        if first == "STD":
            return "GEN_STD"
    return "GEN_MATHLIB"


def encode_ir(tokens: list[str], vocab: dict[str, int], module: str = "") -> list[int]:
    unk_id = vocab.get("<UNK>", 3)
    gen_unk_id = vocab.get("GEN_UNK", 4)
    # C4: bucket gen:* tokens by the declaration's module instead of collapsing
    # them all to GEN_UNK. Falls back to GEN_UNK if the bucket is not in vocab.
    bucket_id = vocab.get(bucket_from_module(module), gen_unk_id) if module else gen_unk_id
    result = []
    for tok in tokens:
        if tok in vocab:
            result.append(vocab[tok])
        elif tok.startswith("gen:"):
            result.append(bucket_id)
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
    """Causal LM labels mirror input_ids; the model applies the shift internally."""
    return input_ids[:]


def example_id(ex: dict) -> str:
    return f"{ex.get('module', '')}::{ex.get('name', '')}"


def build_variant_A(examples: list[dict], vocab: dict[str, int], representation_id: str) -> list[dict]:
    rows = []
    for ex in examples:
        ids = encode_ir(ex.get("tokens", []), vocab, ex.get("module", ""))
        rows.append({
            "source": "A",
            "representation_id": representation_id,
            "example_id": example_id(ex),
            "name": ex.get("name", ""),
            "module": ex.get("module", ""),
            "input_ids": ids,
            "labels": make_labels(ids),
            "seq_len": len(ids),
        })
    return rows


def build_variant_B(examples: list[dict], tokenizer, representation_id: str) -> list[dict]:
    rows = []
    for ex in examples:
        text = ex.get("leanExpr", "")
        ids = tokenizer.encode(text)
        rows.append({
            "source": "B",
            "representation_id": representation_id,
            "example_id": example_id(ex),
            "name": ex.get("name", ""),
            "module": ex.get("module", ""),
            "input_ids": ids,
            "labels": make_labels(ids),
            "seq_len": len(ids),
        })
    return rows


def build_variant_C(examples: list[dict], tokenizer, representation_id: str) -> list[dict]:
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
            "representation_id": representation_id,
            "example_id": example_id(ex),
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


TERM_MANY_FILTER_THRESHOLD = 50  # drop examples with more TERM_MANY tokens than this


def filter_examples(examples: list[dict], threshold: int, drop_log_path: Optional[str] = None) -> list[dict]:
    """
    Drop examples where TERM_MANY appears more than `threshold` times.
    These are pathological sequences (._f flat constructors, large auto-generated match arms)
    that would inject hundreds of ambiguous tokens into single training sequences.

    Logs dropped declaration names to `drop_log_path` if provided.
    """
    kept, dropped = [], []
    for ex in examples:
        count = ex.get("tokens", []).count("TERM_MANY")
        if count <= threshold:
            kept.append(ex)
        else:
            dropped.append({"name": ex.get("name", ""), "module": ex.get("module", ""), "term_many_count": count})

    dropped.sort(key=lambda x: -x["term_many_count"])
    print(f"  Filter (TERM_MANY > {threshold}): kept {len(kept)}, dropped {len(dropped)}")
    if dropped:
        print(f"  Top 5 dropped: {[d['name'] for d in dropped[:5]]}")

    if drop_log_path and dropped:
        os.makedirs(os.path.dirname(drop_log_path), exist_ok=True)
        with open(drop_log_path, "w") as f:
            json.dump(dropped, f, indent=2)
        print(f"  Full drop log → {drop_log_path}")

    return kept


def run(corpus_path: str, out_dir: str, seed: int = 42, representation_id: str = "semantic_graph_ir_v2_0_0") -> None:
    print(f"Loading corpus from {corpus_path} ...")
    with open(corpus_path) as f:
        examples = [json.loads(line) for line in f]
    print(f"Loaded {len(examples)} examples.")

    # Filter pathological examples before splitting
    print("Filtering pathological examples ...")
    drop_log = os.path.join(out_dir, "filtered_dropped.json")
    examples = filter_examples(examples, TERM_MANY_FILTER_THRESHOLD, drop_log_path=drop_log)
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
        model = "Qwen/Qwen2.5-Coder-0.5B"
        print(f"Loading tokenizer ({model}) ...")
        tokenizer = AutoTokenizer.from_pretrained(model, trust_remote_code=True)
        print(f"  Tokenizer vocab size: {tokenizer.vocab_size:,}")
        print()

    # Build datasets
    print("Building variant A (IR tokens) ...")
    train_A = build_variant_A(train_examples, vocab, representation_id)
    eval_A  = build_variant_A(eval_examples,  vocab, representation_id)

    if tokenizer:
        print("Building variant B (Lean source → BPE) ...")
        train_B = build_variant_B(train_examples, tokenizer, representation_id)
        eval_B  = build_variant_B(eval_examples,  tokenizer, representation_id)

        print("Building variant C (AST-style → BPE) ...")
        train_C = build_variant_C(train_examples, tokenizer, representation_id)
        eval_C  = build_variant_C(eval_examples,  tokenizer, representation_id)
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

    train_manifest_path = os.path.join(out_dir, "train_manifest.json")
    eval_manifest_path = os.path.join(out_dir, "eval_manifest.json")
    with open(train_manifest_path, "w") as f:
        json.dump([example_id(ex) for ex in train_examples], f, indent=2)
    with open(eval_manifest_path, "w") as f:
        json.dump([example_id(ex) for ex in eval_examples], f, indent=2)
    print(f"  Wrote train/eval manifests → {train_manifest_path}, {eval_manifest_path}")

    representation_manifest_path = os.path.join(out_dir, "representation_manifest.json")
    with open(representation_manifest_path, "w") as f:
        json.dump(
            {
                "representation_id": representation_id,
                "encoderVersion": "1.3.0",
                "seed": seed,
                "train_examples": len(train_examples),
                "eval_examples": len(eval_examples),
            },
            f,
            indent=2,
        )
    print(f"  Wrote representation manifest → {representation_manifest_path}")

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
    print("  Recommended: Qwen2.5-Coder-0.5B (variant A uses custom vocab; B/C use native BPE)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--out", default="datasets/")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--representation-id", default="semantic_graph_ir_v2_0_0")
    args = parser.parse_args()
    run(args.corpus, args.out, args.seed, args.representation_id)
