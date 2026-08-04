"""
build_flat_ir_dataset.py

Flat-IR ablation dataset builder for Phase 7 design-validation experiment.

Transforms v1.4.0 IR token sequences into shape-only sequences where every
content token is replaced with SLOT. Preserves row structure (E/A/R/O counts
and positions) but removes all semantic content (entity IDs, gen: ops, etc.)

Vocab: 7 tokens only — GRAPH_BEGIN, GRAPH_END, E, A, R, O, SLOT
(+ <PAD>, <BOS>, <EOS>, <UNK> for training compatibility)

If flat-IR ≈ Variant A perplexity: graph content is noise, only shape matters.
If flat-IR >> Variant A perplexity: graph content carries real signal.

Usage:
    python3 python/build_flat_ir_dataset.py --corpus Corpus/corpus.jsonl --out datasets_flat/
"""

import argparse
import json
import random
from collections import Counter
from pathlib import Path


# ---------------------------------------------------------------------------
# Flat-IR vocab (intentionally minimal)
# ---------------------------------------------------------------------------

FLAT_VOCAB = {
    "<PAD>": 0,
    "<BOS>": 1,
    "<EOS>": 2,
    "<UNK>": 3,
    "GRAPH_BEGIN": 4,
    "GRAPH_END": 5,
    "E": 6,
    "A": 7,
    "R": 8,
    "O": 9,
    "SLOT": 10,
}


# ---------------------------------------------------------------------------
# Flatten a v1.4.0 token sequence → shape-only sequence
# ---------------------------------------------------------------------------

ROW_WIDTHS = {"E": 2, "A": 4, "R": 4, "O": 4}  # marker + N content slots

def flatten_tokens(tokens: list[str]) -> list[str]:
    """
    Replace all content tokens with SLOT, preserving structural row markers.
    Output length equals input length.
    """
    result = []
    i = 0
    while i < len(tokens):
        t = tokens[i]
        if t in ("GRAPH_BEGIN", "GRAPH_END"):
            result.append(t)
            i += 1
        elif t in ROW_WIDTHS:
            width = ROW_WIDTHS[t]
            result.append(t)
            result.extend(["SLOT"] * (width - 1))
            i += width
        else:
            # Unexpected token — pass through as SLOT
            result.append("SLOT")
            i += 1
    return result


def encode_flat(tokens: list[str]) -> list[int]:
    unk = FLAT_VOCAB["<UNK>"]
    return [FLAT_VOCAB.get(t, unk) for t in tokens]


def make_labels(ids: list[int]) -> list[int]:
    """Causal LM labels: shift by 1, mask first token with -100."""
    return [-100] + ids[1:]


# ---------------------------------------------------------------------------
# Dataset split (reuse same split logic as build_dataset.py)
# ---------------------------------------------------------------------------

def split_examples(examples: list[dict], eval_frac: float = 0.1, seed: int = 42):
    rng = random.Random(seed)
    shuffled = examples[:]
    rng.shuffle(shuffled)
    n_eval = max(1, int(len(shuffled) * eval_frac))
    return shuffled[n_eval:], shuffled[:n_eval]


def example_id(ex: dict) -> str:
    return f"{ex.get('module', '')}::{ex.get('name', '')}"


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def run(corpus_path: str, out_dir: str, seed: int = 42) -> None:
    corpus_path = Path(corpus_path)
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"Loading corpus from {corpus_path} ...")
    with open(corpus_path) as f:
        examples = [json.loads(l) for l in f]
    print(f"  {len(examples)} examples loaded")

    train_examples, eval_examples = split_examples(examples, seed=seed)
    print(f"  Split: {len(train_examples)} train / {len(eval_examples)} eval")

    def build(exs: list[dict]) -> list[dict]:
        rows = []
        for ex in exs:
            flat = flatten_tokens(ex.get("tokens", []))
            ids = encode_flat(flat)
            rows.append({
                "source": "flat_ir",
                "representation_id": "flat_ir_v1",
                "example_id": example_id(ex),
                "name": ex.get("name", ""),
                "module": ex.get("module", ""),
                "input_ids": ids,
                "labels": make_labels(ids),
                "seq_len": len(ids),
            })
        return rows

    train_rows = build(train_examples)
    eval_rows  = build(eval_examples)

    # Sequence length stats
    train_lens = sorted(r["seq_len"] for r in train_rows)
    n = len(train_lens)
    median = train_lens[n // 2]
    max_len = train_lens[-1]

    def write_jsonl(rows, path):
        with open(path, "w") as f:
            for r in rows:
                f.write(json.dumps(r) + "\n")

    write_jsonl(train_rows, out_dir / "train_flat.jsonl")
    write_jsonl(eval_rows,  out_dir / "eval_flat.jsonl")

    # Write vocab
    with open(out_dir / "vocab_flat.json", "w") as f:
        json.dump(FLAT_VOCAB, f, indent=2)

    print(f"\nWriting dataset files ...")
    print(f"  Wrote {len(train_rows)} rows → {out_dir}/train_flat.jsonl")
    print(f"  Wrote {len(eval_rows)} rows  → {out_dir}/eval_flat.jsonl")
    print(f"  Wrote vocab ({len(FLAT_VOCAB)} tokens) → {out_dir}/vocab_flat.json")
    print(f"\n=== Flat-IR dataset summary ===")
    print(f"  train: {len(train_rows)} examples  median_len={median}  max_len={max_len}")
    print(f"  eval:  {len(eval_rows)} examples")
    print(f"  vocab size: {len(FLAT_VOCAB)} tokens")
    print(f"\nNext step: train with --variant flat --datasets {out_dir}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build flat-IR ablation dataset")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--out", default="datasets_flat/")
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    run(args.corpus, args.out, args.seed)
