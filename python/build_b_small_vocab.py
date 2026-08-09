#!/usr/bin/env python3
"""
build_b_small_vocab.py

Build the B-small control experiment vocab + datasets (DEC-027).

B-small tests whether Variant A's competitiveness comes from the semantic IR
structure or merely from a small vocabulary / small embedding table. It truncates
the BPE vocabulary to the 601 most frequent tokens (matching A's 601), resizes the
embedding table to 601 rows (→ 358M params, matching A), and remaps the B datasets
to the compact 0..600 id range. The BPE representation (raw leanExpr, no semantic
structure) is otherwise unchanged.

Outputs:
  datasets/vocab_B_small.json   — {token: id} map, size 601
  datasets/train_B_small.jsonl  — train_B with input_ids remapped to 0..600
  datasets/eval_B_small.jsonl   — eval_B with input_ids remapped to 0..600

Coverage is verified to be ~99.9% (this Lean corpus uses only 874 distinct BPE ids,
so truncating to 601 is near-lossless). See docs/V2_COMPARISON_MATRIX.md.

Run from repo root:
    python3 python/build_b_small_vocab.py
"""

import json
from collections import Counter
from pathlib import Path

DATASETS = Path("datasets")
TARGET_VOCAB = 601
SPECIALS = {"<PAD>": 0, "<BOS>": 1, "<EOS>": 2, "<UNK>": 3}
UNK_ID = SPECIALS["<UNK>"]


def main():
    # 1. Count BPE token-id frequency across the TRAINING split (vocab built from train only,
    #    matching how build_dataset.py builds vocab_A from train_A).
    counts = Counter()
    n_tok = 0
    with open(DATASETS / "train_B.jsonl") as f:
        for line in f:
            for tid in json.loads(line)["input_ids"]:
                counts[tid] += 1
                n_tok += 1
    n_unique = len(counts)
    print(f"train_B: {n_tok} tokens, {n_unique} unique BPE ids")

    # 2. Take the top (TARGET_VOCAB - len(SPECIALS)) most frequent ids; assign compact ids.
    keep_n = TARGET_VOCAB - len(SPECIALS)
    top = counts.most_common(keep_n)
    # build remapping: old BPE id (int) -> new compact id (4..600)
    remap = {old_id: (i + len(SPECIALS)) for i, (old_id, _) in enumerate(top)}

    # 3. Vocab_B_small.json (string keys for JSON): specials + str(old_id) -> new_id
    vocab = dict(SPECIALS)  # 0..3
    for old_id, new_id in remap.items():
        vocab[str(old_id)] = new_id
    assert len(vocab) == TARGET_VOCAB, f"vocab size {len(vocab)} != {TARGET_VOCAB}"
    with open(DATASETS / "vocab_B_small.json", "w") as f:
        json.dump(vocab, f, indent=2)
    print(f"Wrote datasets/vocab_B_small.json ({len(vocab)} tokens)")

    # 4. Coverage check (how much of train+eval falls outside the top-601 -> UNK)
    coverage = sum(c for old_id, c in counts.items() if old_id in remap)
    print(f"  coverage on train_B: {coverage / n_tok * 100:.2f}% "
          f"(UNK rate {100 - coverage / n_tok * 100:.2f}%)")
    top10 = [old_id for old_id, _ in top[:10]]
    print(f"  top-10 BPE ids kept: {top10}")

    # 5. Remap datasets: train_B / eval_B -> train_B_small / eval_B_small (source="B_small")
    for split in ("train", "eval"):
        src_path = DATASETS / f"{split}_B.jsonl"
        out_path = DATASETS / f"{split}_B_small.jsonl"
        n_rows = 0
        n_unk = 0
        n_total = 0
        with open(src_path) as fin, open(out_path, "w") as fout:
            for line in fin:
                row = json.loads(line)
                new_ids = [remap.get(tid, UNK_ID) for tid in row["input_ids"]]
                n_unk += sum(1 for i in new_ids if i == UNK_ID)
                n_total += len(new_ids)
                row["input_ids"] = new_ids
                row["labels"] = list(new_ids)
                row["source"] = "B_small"
                row["representation_id"] = "bpe_small_601"
                # keep name/module/example_id/seq_len; recompute seq_len
                row["seq_len"] = len(new_ids)
                fout.write(json.dumps(row) + "\n")
                n_rows += 1
        print(f"Wrote {out_path} ({n_rows} rows; UNK {n_unk}/{n_total} = "
              f"{n_unk / max(n_total, 1) * 100:.2f}%)")

    print("\nB-small vocab + datasets ready.")
    print("Next: train with  python/python/train_v2_resume.py --variant B_small --out runs/variant_B_small")


if __name__ == "__main__":
    main()
