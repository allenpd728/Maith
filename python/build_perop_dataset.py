#!/usr/bin/env python3
"""
build_perop_dataset.py — Build a per-operator A dataset that preserves gen:<FullName>
tokens (no C4 GEN_ bucketing). This matches what the variant_A_v3 checkpoint was
trained on (vocab_size=8102, gen:<FullName> tokens).

Needed because build_dataset.py's encode_ir always buckets gen: tokens to GEN_<AREA>
regardless of --bucket-mode.
"""
import json
import os
import random
from pathlib import Path
from collections import Counter

REPO = Path(__file__).resolve().parent.parent
CORPUS = REPO / "Corpus" / "corpus.jsonl"
OUT = REPO / "datasets_perop"
SEED = 42
TERM_MANY_THRESHOLD = 50

def example_id(ex):
    return f"{ex.get('module','')}::{ex.get('name','')}"

def main():
    # Load corpus
    with open(CORPUS) as f:
        examples = [json.loads(line) for line in f if line.strip()]
    print(f"Loaded {len(examples)} examples")

    # Filter TERM_MANY
    filtered = [ex for ex in examples if ex.get("tokens",[]).count("TERM_MANY") <= TERM_MANY_THRESHOLD]
    print(f"Filtered: {len(filtered)} (dropped {len(examples)-len(filtered)})")

    # Dedup
    seen = set()
    deduped = []
    for ex in filtered:
        eid = example_id(ex)
        if eid in seen:
            continue
        seen.add(eid)
        deduped.append(ex)
    print(f"Deduped: {len(deduped)} (removed {len(filtered)-len(deduped)})")

    # Split (same logic as build_dataset.py: first 90% train, last 10% eval)
    random.seed(SEED)
    shuffled = deduped[:]
    random.shuffle(shuffled)
    split = int(len(shuffled) * 0.9)
    train_examples = shuffled[:split]
    eval_examples = shuffled[split:]
    print(f"Split: {len(train_examples)} train / {len(eval_examples)} eval")

    # Build vocab from training set — preserve gen:<FullName> tokens, no bucketing
    token_counts = Counter()
    for ex in train_examples:
        for t in ex.get("tokens", []):
            if isinstance(t, str):
                token_counts[t] += 1

    # Build vocab: structural tokens first, then by frequency
    STRUCTURAL = ["<PAD>", "<BOS>", "<EOS>", "<UNK>", "GRAPH_BEGIN", "GRAPH_END",
                  "E", "A", "R", "O", "FVAR_MANY", "BVAR_MANY", "TERM_MANY"]
    vocab = {}
    for i, t in enumerate(STRUCTURAL):
        vocab[t] = i
    next_id = len(STRUCTURAL)

    # Add all tokens from training set in frequency order (including gen:<FullName>)
    # Use threshold 1 (keep all tokens) to match A v3's 8102-vocab training
    for tok, count in token_counts.most_common():
        if tok not in vocab and count >= 1:  # keep all tokens to match v3
            vocab[tok] = next_id
            next_id += 1

    print(f"Vocab size: {len(vocab)}")
    gen_count = sum(1 for k in vocab if k.startswith("gen:"))
    print(f"gen:<FullName> tokens in vocab: {gen_count}")

    # Save vocab
    OUT.mkdir(parents=True, exist_ok=True)
    with open(OUT / "vocab_A_perop.json", "w") as f:
        json.dump(vocab, f, indent=2)

    # Encode — no bucketing, direct lookup
    unk_id = vocab.get("<UNK>", 3)

    def encode(tokens):
        result = []
        for t in tokens:
            if t in vocab:
                result.append(vocab[t])
            else:
                result.append(unk_id)
        return result

    def build_rows(examples_list):
        rows = []
        for ex in examples_list:
            ids = encode(ex.get("tokens", []))
            rows.append({
                "source": "A",
                "representation_id": "semantic_graph_ir_perop_v1",
                "example_id": example_id(ex),
                "name": ex.get("name", ""),
                "module": ex.get("module", ""),
                "input_ids": ids,
                "labels": ids[:],
                "seq_len": len(ids),
            })
        return rows

    train_rows = build_rows(train_examples)
    eval_rows = build_rows(eval_examples)

    for split_name, rows in [("train", train_rows), ("eval", eval_rows)]:
        path = OUT / f"{split_name}_A.jsonl"
        with open(path, "w") as f:
            for r in rows:
                f.write(json.dumps(r) + "\n")
        print(f"Wrote {len(rows)} rows -> {path}")

if __name__ == "__main__":
    main()
