#!/usr/bin/env python3
"""
test_pipeline_integration.py

End-to-end smoke test for the full pipeline: synthetic corpus → build_dataset →
collate_fn → valid batches. No GPU, no model weights.

This catches the class of bug where build_dataset.py produces well-formed JSON
that train.py then silently misreads. Runs in under 10 seconds.

Run with:
    python3 python/test_pipeline_integration.py

A non-zero exit code means at least one test failed.
"""

import json
import os
import sys
import tempfile
import shutil
import math

sys.path.insert(0, "python")
import importlib.util

# ---------------------------------------------------------------------------
# Load modules under test
# ---------------------------------------------------------------------------

_spec_build = importlib.util.spec_from_file_location(
    "build_dataset", os.path.join("python", "build_dataset.py")
)
_mod_build = importlib.util.module_from_spec(_spec_build)
_spec_build.loader.exec_module(_mod_build)

_spec_train = importlib.util.spec_from_file_location(
    "train", os.path.join("python", "train.py")
)
_mod_train = importlib.util.module_from_spec(_spec_train)
_spec_train.loader.exec_module(_mod_train)

build_ir_vocab = _mod_build.build_ir_vocab
build_variant_A = _mod_build.build_variant_A
filter_examples = _mod_build.filter_examples
collate_fn = _mod_train.collate_fn


# ---------------------------------------------------------------------------
# Synthetic corpus generator
# ---------------------------------------------------------------------------

def make_synthetic_corpus(n: int = 25) -> list[dict]:
    """Create a synthetic corpus with token sequences of length 5-30."""
    fixtures = []
    for i in range(n):
        # Vary sequence length between 5 and 30 tokens
        seq_len = 5 + (i % 26)  # 5 to 30
        tokens = ["GRAPH_BEGIN"]
        for j in range(seq_len - 2):
            if j % 4 == 0:
                tokens.append("E")
                tokens.append(f"FVAR_{j % 4}")
                tokens.append("neut")
            elif j % 4 == 1:
                tokens.append("A")
                tokens.append(f"TERM_{j % 8}")
                tokens.append("typeclass")
                tokens.append(f"gen:Class_{j % 3}")
                tokens.append("neut")
            else:
                tokens.append(f"TERM_{j % 16}")
                tokens.append("neut")
        tokens.append("GRAPH_END")

        fixtures.append({
            "name": f"SyntheticDecl_{i:03d}",
            "module": f"Mathlib.Synthetic.Module_{i % 3}",
            "tokens": tokens,
            "leanExpr": f"synthetic expr {i}",
            "term_many_count": 0,
        })
    return fixtures


# ---------------------------------------------------------------------------
# Test helpers
# ---------------------------------------------------------------------------

PASS = []
FAIL = []


def ok(name: str):
    PASS.append(name)
    print(f"  PASS  {name}")


def fail(name: str, reason: str):
    FAIL.append(name)
    print(f"  FAIL  {name}: {reason}")


# ---------------------------------------------------------------------------
# TEST 1: End-to-end pipeline — corpus → build_dataset → collate_fn → valid batches
#
# Full pipeline smoke test:
# 1. Create synthetic corpus
# 2. Run build_dataset (Variant A only for simplicity)
# 3. Load resulting train_A.jsonl
# 4. Run collate_fn on batch of 4 examples
# 5. Assert: input_ids shape (4, max_seq_len), labels == input_ids for non-padded,
#    no NaN, all IDs within vocab range
# ---------------------------------------------------------------------------

def test_full_pipeline():
    name = "full pipeline: corpus → build_dataset → collate_fn → valid batches"
    try:
        # Step 1: Create synthetic corpus
        examples = make_synthetic_corpus(25)

        # Step 2: Build datasets (Variant A only)
        with tempfile.TemporaryDirectory() as tmpdir:
            # Build vocab
            vocab = build_ir_vocab(examples)
            vocab_path = os.path.join(tmpdir, "vocab_A.json")
            with open(vocab_path, "w") as f:
                json.dump(vocab, f)

            # Filter and split
            filtered = filter_examples(examples, threshold=50)

            import random
            random.seed(42)
            shuffled = filtered[:]
            random.shuffle(shuffled)
            split = int(len(shuffled) * 0.9)
            train_examples = shuffled[:split]
            eval_examples = shuffled[split:]

            # Build variant A datasets
            train_A = build_variant_A(train_examples, vocab, "test_v1")
            eval_A = build_variant_A(eval_examples, vocab, "test_v1")

            # Write to temp JSONL
            train_path = os.path.join(tmpdir, "train_A.jsonl")
            eval_path = os.path.join(tmpdir, "eval_A.jsonl")

            with open(train_path, "w") as f:
                for row in train_A:
                    f.write(json.dumps(row) + "\n")
            with open(eval_path, "w") as f:
                for row in eval_A:
                    f.write(json.dumps(row) + "\n")

            # Step 3: Load train_A.jsonl
            loaded_rows = []
            with open(train_path) as f:
                for line in f:
                    loaded_rows.append(json.loads(line))

            assert len(loaded_rows) > 0, "train_A.jsonl is empty"
            print(f"  (loaded {len(loaded_rows)} examples from train_A.jsonl)")

            # Step 4: Run collate_fn on batch of 4
            batch_size = 4
            batch_examples = loaded_rows[:batch_size]

            import torch
            batch_items = []
            for row in batch_examples:
                ids = row["input_ids"]
                t = torch.tensor(ids, dtype=torch.long)
                batch_items.append({
                    "input_ids": t,
                    "attention_mask": torch.ones_like(t),
                    "labels": t.clone(),
                })

            # Override labels to match what build_dataset produces
            for i, row in enumerate(batch_examples):
                batch_items[i]["labels"] = torch.tensor(row["labels"], dtype=torch.long)

            collated = collate_fn(batch_items)

            # Step 5: Assertions

            # 5a: input_ids shape is (batch_size, max_seq_len)
            expected_shape = (batch_size, max(len(ex["input_ids"]) for ex in batch_examples))
            assert collated["input_ids"].shape == expected_shape, \
                f"input_ids shape mismatch: got {collated['input_ids'].shape}, expected {expected_shape}"

            # 5b: labels == input_ids for non-padded positions
            for i in range(batch_size):
                seq_len = len(batch_examples[i]["input_ids"])
                for j in range(seq_len):
                    assert collated["labels"][i, j].item() == collated["input_ids"][i, j].item(), \
                        f"labels[{i},{j}] != input_ids[{i},{j}]"

            # 5c: No NaN in any tensor
            assert not torch.isnan(collated["input_ids"]).any(), "input_ids contains NaN"
            assert not torch.isnan(collated["labels"]).any(), "labels contains NaN"
            assert not torch.isnan(collated["attention_mask"]).any(), "attention_mask contains NaN"

            # 5d: All IDs within vocab range
            vocab_size = len(vocab)
            all_ids = collated["input_ids"].tolist()
            flat_ids = [id_ for row in all_ids for id_ in row]
            max_id = max(flat_ids)
            assert max_id < vocab_size, \
                f"Found ID {max_id} >= vocab size {vocab_size}"

            ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("Running pipeline integration smoke test...\n")
    print("This test exercises the full pipeline without GPU or model weights.\n")

    test_full_pipeline()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
