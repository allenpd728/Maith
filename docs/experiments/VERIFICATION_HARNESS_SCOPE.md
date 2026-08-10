# Verification Harness — Scoping Document for Implementation

> **Purpose.** Before running H6 (or any new experiment), build a verification harness
> that catches the bug classes this project has already hit. This document fills in
> the file-specific details that a non-local agent needs to implement the harness.
>
> **Context.** The project has been bitten by: stale checkpoints (variant C eval bug),
> wrong-input extraction (IR tokens fed to B/C variants in H6), epoch confounds
> (B/C 3-epoch v1-era vs A/B-small 2-epoch v2), and untracked split name collisions
> (13 train/eval name overlaps). A verification harness catches these before results
> are trusted.

---

## 1. Checkpoint provenance — assert checkpoint matches results.json

### What exists

`python/eval_completion.py` (line 146) has `warn_if_checkpoint_stale()`. It compares
the modification timestamps of the checkpoint directory and `results.json` and **warns**
(print) if the gap exceeds 1 day. This is a warning, not a hard fail — the eval proceeds
with potentially stale weights.

### What's missing

The guard checks *timestamps*, not *content*. A checkpoint and results.json could have
matching timestamps but describe different training runs (e.g., if both were touched by a
`cp -r` or a re-run that didn't overwrite). The guard should also:

- **Verify `results.json` fields against the checkpoint's config.json.** The checkpoint
  directory contains `config.json` with model architecture details (vocab_size,
  hidden_size, num_layers). `results.json` records `vocab_size` and `n_params_M`. Assert
  these match:
  - `results.json.vocab_size` == checkpoint `config.json.vocab_size` (for A/B_small)
  - `results.json.n_params_M` is consistent with `config.json` architecture

- **Hash the checkpoint weights.** Compute a SHA256 of the checkpoint's
  `model_weights.safetensors` (or `pytorch_model.bin`) and store the hash in
  `results.json` at training time. At eval time, recompute and assert equality. This
  catches any post-training weight corruption or accidental overwrite.

- **Make it a hard fail, not a warning.** Change `warn_if_checkpoint_stale()` to
  `assert_checkpoint_provenance()` that raises on mismatch. Provide a `--force` flag to
  override for known-edge cases (e.g., manual checkpoint moves).

### Files to modify

- `python/eval_completion.py` — upgrade `warn_if_checkpoint_stale()` to
  `assert_checkpoint_provenance()` with content checks + hash verification
- `python/train.py` — add checkpoint hash to `results.json` at training time (line 628,
  where `results.json` is written)

### What results.json currently contains (for reference)

```json
{
  "variant": "A",
  "vocab_size": 601,
  "n_params_M": 358.4,
  "train_examples": 3491,
  "eval_examples": 388,
  "epochs": 2,
  "eval_perplexity": 1.2361,
  "training_minutes": 105.4,
  "seed": 42,
  "base_model": "Qwen/Qwen2.5-Coder-0.5B",
  "smoke_test": false,
  "embed_pretrain": false,
  "embed_project": false
}
```

Fields to add: `checkpoint_sha256`, `config_vocab_size`, `config_hidden_size`.

---

## 2. Input-variant consistency — assert input matches the variant's training data

### What exists

Nothing. This is the gap that caused the H6 bug.

`python/extract_representations.py` (line 219) feeds `ex["tokens"]` (the IR token list
from `Corpus/corpus.jsonl`) to every variant. It joins the tokens with spaces,
re-tokenizes via the variant's tokenizer, and forward-passes. This is correct for
Variant A and flat (both trained on IR tokens), but **wrong for B, C, and B_small**
(those trained on `leanExpr` text or AST-split text via their own `input_ids`).

### What's needed

A pre-extraction assertion that verifies the input source matches the variant:

- **For extraction scripts:** before forward-passing, load the variant's dataset file
  (`datasets/eval_{variant}.jsonl`) and assert that the `input_ids` field exists. Then
  feed `input_ids` directly to the model (no re-tokenization, no string round-trip).
  This is what PR #16's H6 scope doc (§4.2) specifies.

- **For training scripts:** `python/train.py` already loads from per-variant dataset
  files via `IRDataset(jsonl_path)`. The `IRDataset.__init__` (line 167) loads
  `input_ids` from each record. This is correct — the bug is only in
  `extract_representations.py`, not in training.

- **For evaluation scripts:** `python/eval_completion.py` also loads from per-variant
  dataset files. Correct.

- **Add a `source` field check.** Each dataset record has a `source` field
  (`"A"`, `"B"`, `"C"`, `"flat"`, `"B_small"`). Before feeding input to a variant's
  model, assert `record["source"]` matches the expected variant. This is a cheap,
  deterministic check that catches the entire class of input-variant mismatch bugs.

### Specific assertion to add

```python
# In any script that feeds data to a variant's model:
expected_source = variant  # "A", "B", "C", "B_small", "flat"
for record in dataset:
    assert record.get("source") == expected_source, (
        f"Input-variant mismatch: record source={record.get('source')} "
        f"but expected variant={expected_source}. "
        f"Feeding the wrong variant's data to this model produces invalid embeddings."
    )
```

### Files to modify

- `python/extract_representations.py` — add source assertion; or better, write a new
  `python/extract_retrieval_embeddings.py` that loads from per-variant dataset files
  (per H6 scope doc §4.2) and includes the source assertion
- Any future extraction/eval script — add the same assertion

### Dataset field reference (verified from actual files)

All variant dataset files (`datasets/{train,eval}_{A,B,C,B_small,flat}.jsonl`) have
identical field structure:
```
source, representation_id, example_id, name, module, input_ids, labels, seq_len
```

The `input_ids` field contains the exact integer token sequences each model was trained
on. These should be fed directly to the model (no re-tokenization).

---

## 3. Split integrity — assert no train/eval name overlap (or document it)

### What exists

`python/build_dataset.py` (line 295-301) does a deterministic 90/10 split with seed=42.
It shuffles with `random.seed(seed)` and splits at the 90% mark. There is no name-overlap
check.

### What was found

**13 names overlap between train and eval.** These are distinct declarations in
different modules that share a short name (e.g., `Set.Icc.instOrderTopElemOfFactLe._proof_1`
appears in both splits as different declarations). This is a pre-existing property of
the split, not a bug — the split is by shuffled index, not by name.

Overlap examples (first 5):
- `Set.Icc.instOrderTopElemOfFactLe._proof_1`
- `Set.Ici.lattice._proof_3`
- `_private.Mathlib.Order.LatticeIntervals.0.Set.Iic.complementedLattice_iff._simp_1_6`
- `Set.Ico.orderBot._proof_1`
- `Set.Icc.instOrderTopElemOfFactLe`

### What's needed

- **Add a split-integrity check to `build_dataset.py`** that runs after the split and
  reports any name overlaps. If overlaps exist, print a warning with the count and
  examples, but do not fail (the overlaps are known and handled in retrieval by matching
  on full `name` + de-duplicating the candidate pool).

- **For retrieval specifically:** match dependencies by full `name` field (which is the
  full declaration name, e.g., `Mathlib.Algebra.Group.Defs.mul_comm`), not by the
  short `name`. De-duplicate the candidate pool by `name` so each declaration appears
  once as a candidate. This is already specified in H6 scope doc §3.3.

- **Add to the verification harness:** a `check_split_integrity()` function that:
  1. Loads train and eval names
  2. Reports overlap count
  3. Asserts the overlap is documented (either in the harness output or in a known-issues
     doc)
  4. For retrieval: asserts the candidate pool is de-duplicated by `name`

### Files to modify

- `python/build_dataset.py` — add post-split overlap report (warning, not error)
- Verification harness — add `check_split_integrity()` function

### Split counts (verified from actual data)

- Train: 3,378 unique names
- Eval: 386 unique names
- Overlap: 13 names (distinct declarations in different modules)

---

## 4. Epoch/hyperparameter provenance — assert compared variants share protocol

### What exists

`python/train.py` records the following in `results.json`:
- `epochs`: number of training epochs (currently hardcoded to 2)
- `seed`: 42
- `base_model`: "Qwen/Qwen2.5-Coder-0.5B"
- `smoke_test`: boolean
- `embed_pretrain`: boolean
- `embed_project`: boolean

Global hyperparameters (hardcoded in train.py, lines 81-87):
```python
BATCH_SIZE      = 1
GRAD_ACCUM      = 8   # effective batch = 8
LEARNING_RATE   = 2e-4
EPOCHS          = 2
WEIGHT_DECAY    = 0.01
WARMUP_RATIO    = 0.05
SEED            = 42
```

### The problem

**Per-variant hparam overrides are not recorded in results.json.** The code has
`VARIANT_BATCH_CONFIG` (referenced in DEC-007) that sets different batch sizes per
variant, and the learning rate was changed for B/C (commit `83f18a8`: "variant-specific
LR — B/C use 5e-5 for 151k embedding stability"). But `results.json` records the global
`LEARNING_RATE` (2e-4), not the per-variant override. So the provenance record is
incomplete.

Additionally, B and C were trained at 3 epochs in the v1 era, while A and B_small were
trained at 2 epochs in the v2 era. This epoch confound is documented (V2_NEXT_STEPS,
DEC-027 notes) but not machine-checkable.

### What's needed

- **Record actual hparams per variant in results.json.** Add fields:
  - `actual_learning_rate`: the LR used for this variant (may differ from global)
  - `actual_batch_size`: the effective batch size for this variant
  - `actual_epochs`: the actual number of epochs run (may be overridden via CLI)
  - `actual_max_seq_len`: the sequence length cap used for this variant
  - `train_config_hash`: a hash of the full training configuration for deduplication

- **Add a comparison assertion.** Before comparing two variants' results, assert:
  ```python
  def assert_comparable(v1_results, v2_results, strict=True):
      """Assert two variants are comparable (same protocol)."""
      fields = ["seed", "base_model", "epochs", "actual_learning_rate",
                "actual_batch_size", "actual_max_seq_len"]
      for field in fields:
          if v1_results.get(field) != v2_results.get(field):
              msg = f"Protocol mismatch on {field}: {v1_results.get(field)} vs {v2_results.get(field)}"
              if strict:
                  raise AssertionError(msg)
              else:
                  print(f"  ⚠ {msg}")
  ```

- **For the existing v1-era B/C runs:** these have `epochs=3` while A/B_small have
  `epochs=2`. The assertion would flag this. The correct behavior is to warn (not fail)
  and direct the user to the epoch-confound documentation. A v2 re-run at 2 epochs
  (in progress per V2_NEXT_STEPS) would resolve this.

### Files to modify

- `python/train.py` — add `actual_*` fields to results.json at training time (line 610-628)
- Verification harness — add `assert_comparable()` function
- `python/eval_completion.py` — call `assert_comparable()` before reporting cross-variant
  comparison tables

### Current per-variant hparam state (from commit history and DEC entries)

| Variant | Epochs | LR | Batch | Max seq | Vocab | Params | Notes |
|---|---|---|---|---|---|---|---|
| A (v2) | 2 | 2e-4 | 8 | 1024 | 601 | 358M | embed-project |
| B_small | 2 | 2e-4 | 8 | 1024 | 601 | 358M | DEC-027 control |
| B (v1-era) | 3 | 5e-5 | 4 | 512 | 151K | 494M | epoch confound |
| C (v1-era) | 3 | 5e-5 | 4 | 512 | 151K | 494M | epoch confound |
| flat | 2 | 2e-4 | 8 | 1024 | 11 | 358M | DEC-024 ablation |

Note: B/C's LR was reduced to 5e-5 for stability (commit 83f18a8), but this is not
recorded in their results.json. The v2 2-epoch re-runs (in progress) would use 2e-4
matching A/B_small.

---

## 5. Implementation deliverables

1. `python/verify_experiment.py` — the verification harness itself:
   - `assert_checkpoint_provenance(run_dir, variant)` — hash + config checks
   - `assert_input_variant_consistency(dataset_path, variant)` — source field check
   - `check_split_integrity(train_path, eval_path)` — name overlap report
   - `assert_comparable(v1_results, v2_results)` — hparam protocol check
   - `run_all_checks(variants, runs_dir, datasets_dir)` — orchestrator

2. `python/train.py` — add `actual_*` fields and `checkpoint_sha256` to results.json

3. `python/eval_completion.py` — upgrade stale warning to hard assertion with content
   checks; call `assert_comparable()` before cross-variant comparisons

4. Tests: `python/test_verify_experiment.py` — unit tests for each assertion

---

## 6. What "done" looks like

- Running `python/verify_experiment.py` on the existing checkpoints produces a report
  that catches: the epoch confound (B/C 3-epoch vs A/B_small 2-epoch), the stale
  checkpoint dirs (runs/_stale_*), and the split overlap (13 names).
- Any new extraction script that feeds data to a variant's model hits the
  `assert_input_variant_consistency()` check and fails hard if the input source doesn't
  match the variant.
- `results.json` for future runs includes `actual_*` fields and `checkpoint_sha256`.
- The H6 experiment (per PR #16 scope doc) runs after the harness passes, not before.

---

## 7. What this does NOT do

- Does not fix the existing invalid H6 results (those are documented as a known issue).
- Does not re-train B/C at 2 epochs (that's in progress separately).
- Does not change the split (13 name overlaps are a known property, not a bug).
- Does not verify model architecture alignment (sequence vs graph) — that's a future
  experimental dimension, not a correctness check.
