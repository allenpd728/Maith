# H6 Retest Plan — Triangulated Verification

> **Status:** ACTIVE
> **Created:** 2026-08-10
> **Trigger:** Line-of-proof verification revealed the initial H6 results were INVALID
> (embeddings extracted from mismatched dataset versions — A from dedup'd 3375/376,
> others from old leaky 3491/388 — producing different n_queryable and pool sizes
> across variants, making the comparison meaningless).

## The problem

The initial H6 run was invalid because:

1. **Datasets on the M4 reverted to old (leaky) versions** after I rebuilt them.
   The dedup'd datasets (3375/376) were overwritten somehow — likely the flat/B_small
   rebuilds triggered by kit/dev pulls restored old-format files.

2. **Embeddings were extracted during a window when datasets were inconsistent:**
   - A: 3375/376 (dedup'd) — from rebuilt datasets
   - B_small, B, C, flat: 3491/388 or 3627/402 (old) — from pre-rebuild datasets
   - This produced embeddings of different sizes from different dataset versions.

3. **H6 eval compared variants on different query sets:**
   - A: n_queryable=276, pool_size=3375
   - B_small: n_queryable=294, pool_size=3491
   - Different queries, different pools → the comparison is invalid.

4. **The invariant checker was NOT run as a gate before extraction.**
   If it had been, it would have blocked the extraction (43 passed, 7 failed).

## The fix: enforced sequential verification

Every step has a gate. No step runs until the previous gate passes.

### Step 1 — Rebuild ALL datasets from the same corpus, in one pass

Run `build_dataset.py`, `build_flat_ir_dataset.py`, and `build_b_small_vocab.py`
in sequence, all reading from the same `Corpus/corpus.jsonl`, all writing to
`datasets/`. This ensures all variants are built from the same dedup'd corpus
with the same split.

**Gate:** all 5 variants have identical train count, identical eval count, and
identical eval example_ids (verified by invariant checker).

### Step 2 — Run invariant checker, require 0 failures

```
python3 python/check_invariants.py --datasets datasets --runs runs
```

Must report: `Total: 50 passed, 0 failed, 0 skipped` and
`All invariants hold. Experiment may proceed.`

If any failure: STOP. Fix the issue. Re-run. Do not proceed.

### Step 3 — Re-extract ALL embeddings from validated datasets

```
python3 python/extract_retrieval_embeddings.py --variants A,B_small,B,C,flat
```

**Gate:** every variant's embedding shape matches its dataset size:
- embeddings_A_train shape[0] == len(datasets/train_A.jsonl)
- embeddings_A_eval shape[0] == len(datasets/eval_A.jsonl)
- (same for B_small, B, C, flat)

### Step 4 — Rebuild dependency ground truth from the same corpus

```
python3 python/build_dependency_groundtruth.py
```

**Gate:** n_queryable > 200 for eval→train mode (verified by the script's output).

### Step 5 — Run H6 retrieval eval (Mode 1: eval→train)

```
python3 python/retrieval_eval.py --mode eval_to_train
```

**Gate:** all variants have the same n_queryable and pool_size.

### Step 6 — Triangulate: run Mode 2 (train→train)

```
python3 python/retrieval_eval.py --mode train_to_train
```

If Mode 1 and Mode 2 agree on the A-vs-B_small ordering, the result is robust
to the eval split choice. If they disagree, neither is trustworthy.

### Step 7 — Determinism check

Re-extract one variant's embeddings and verify they're identical (or within
float tolerance) to the first extraction.

If embeddings aren't deterministic, no result is reproducible.

## Acceptance criteria

- All invariant checks pass (0 failures)
- All embedding shapes match dataset sizes
- All variants in H6 results have the same n_queryable and pool_size
- Mode 1 and Mode 2 agree on A-vs-B_small ordering
- Embedding extraction is deterministic
- Results saved to `runs/h6_retrieval/results.json` (Mode 1) and
  `runs/h6_retrieval/results_mode2.json` (Mode 2)
