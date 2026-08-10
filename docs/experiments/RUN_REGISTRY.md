# Run Registry — Checkpoint Provenance Reference

> **Purpose.** Single source of truth for what each checkpoint was trained on,
> what config it used, and which comparisons it belongs to. This exists because
> the manifest system was built but never wired into the training scripts, so
> provenance must be reconstructed from results.json files and commit history.
>
> **Update protocol.** Every new training run adds a row here BEFORE the run
> starts, with a designated run ID. After the run, fill in the actual values.
> Never delete rows — superseded runs are marked SUPERSEDED.

## Checkpoint inventory

### Variant A (semantic IR)

| Run ID | Dir | Vocab | Train n | Epochs | Corpus mode | LR | Perplexity | Notes |
|---|---|---|---|---|---|---|---|---|
| A-dec006 | variant_A_dec006 | 8144 | 3375 | 2 | per-op (gen:FullName) | 2e-4 | 1.2978 | Early per-op run |
| A-phase6 | variant_A_phase6 | 8144 | 3375 | 3 | per-op (gen:FullName) | 2e-4 | 1.2792 | 3-epoch per-op |
| A-v1.3.0 | variant_A_v1_3_0 | 4554 | 2213 | 2 | v1.3.0 (pre-C4) | 2e-4 | 1.4325 | Old corpus, small |
| A-v1.3.0-ep | variant_A_v1_3_0_ep | 4554 | 2213 | 2 | v1.3.0 | 2e-4 | 1.4226 | Duplicate of above |
| A-v1.3.0-final | variant_A_v1_3_0_final | 8144 | 3375 | 2 | per-op | 2e-4 | 1.3016 | |
| A-v1.3.0-proj | variant_A_v1_3_0_proj | 8221 | 3491 | 2 | per-op, old (leaky) | 2e-4 | 1.3717 | Trained on leaky data |
| A-v1.3.0-true | variant_A_v1_3_0_true | 8221 | 3491 | 2 | per-op, old (leaky) | 2e-4 | 1.3896 | Trained on leaky data |
| A-v1.4.0 | variant_A_v1_4_0 | 1236 | 3491 | 2 | v1.4.0 (IO markers) | 2e-4 | 1.2751 | Trained on leaky data |
| **A-v2** | **variant_A_v2_full** | **601** | **3491** | **2** | **v2 module (GEN_ buckets)** | **2e-4** | **1.2361** | **DEC-026/027, trained on leaky data** |
| A-v2.0.0-smoke | variant_A_v2_0_0 | 601 | 50 | 1 | v2 module | 2e-4 | 16.06 | Smoke test |
| **A-v3** | **variant_A_v3** | **8102** | **3375** | **3** | **per-op (gen:FullName)** | **2e-4** | **1.2812** | **DEC-029, 3 epochs** |

### Variant B (raw BPE)

| Run ID | Dir | Vocab | Train n | Epochs | LR | Perplexity | Notes |
|---|---|---|---|---|---|---|---|
| B2 | variant_B2 | 151643 | 3375 | 3 | 5e-5 | 1.107 | 3-epoch, dedup'd train |
| B-phase6 | variant_B_phase6 | 151643 | 3375 | 2 | 5e-5 | 1.1295 | 2-epoch |
| **B-v2-2ep** | **variant_B_v2_2ep** | **151643** | **3491** | **2** | **5e-5** | **1.099** | **Trained on leaky data** |

### Variant B_small (BPE truncated to 601)

| Run ID | Dir | Vocab | Train n | Epochs | LR | Perplexity | Notes |
|---|---|---|---|---|---|---|---|
| **B-small** | **variant_B_small** | **601** | **3491** | **2** | **2e-4** | **1.1294** | **DEC-027, trained on leaky data** |

### Variant C (AST-split BPE)

| Run ID | Dir | Vocab | Train n | Epochs | LR | Perplexity | Notes |
|---|---|---|---|---|---|---|---|
| C-phase6 | variant_C_phase6 | 151643 | 3375 | 2 | 5e-5 | 1.1102 | |
| **C-v2** | **variant_C_v2** | **151643** | **3375** | **3** | **5e-5** | **1.098** | **3-epoch** |

### Variant flat (shape-only IR)

| Run ID | Dir | Vocab | Train n | Epochs | LR | Perplexity | Notes |
|---|---|---|---|---|---|---|---|
| flat | variant_flat | 11 | 3627 | 2 | 2e-4 | 1.0551 | Old split (3627, unmatched) |

---

## Comparison matrix — what's valid to compare

### Valid comparisons (confound-free)

| Comparison | A side | B side | Matched on | Differs on | Status |
|---|---|---|---|---|---|
| A-v2 vs B-small | A-v2 (2ep, 601v, leaky) | B-small (2ep, 601v, leaky) | epochs, vocab, train_n, corpus | representation | ✅ VALID (DEC-027) |
| A-v1.4.0 vs flat | A-v1.4.0 (2ep, 1236v) | flat (2ep, 11v, old split) | epochs | representation, split | ⚠️ SPLIT MISMATCH |

### Confounded comparisons (NOT valid as representation tests)

| Comparison | Confound | Why it matters |
|---|---|---|
| A-v2 vs B-v2-2ep | Size (601v vs 151Kv) | 136M param difference |
| A-v2 vs C-v2 | Size + epochs (2 vs 3) | Two variables changed |
| A-v3 vs B-small | **Epochs (3 vs 2)** | A-v3 had 50% more training |
| A-v3 vs A-v2 | Epochs (3 vs 2) + corpus mode | Two variables changed |
| A-v3 vs B-v2-2ep | Size + epochs + corpus mode | Three variables changed |
| Any vs flat | Split mismatch (3627 vs 3375/3491) | Different eval declarations |

### Needed: clean A-v3 comparison

To isolate C4 bucketing as the only variable, need:
- **A-v3-2ep**: A-v3 retrained at 2 epochs (matching B-small)
- Then compare A-v3-2ep vs B-small (both 2ep, 601-vocab-ish, same split)
- OR: B-small-3ep: B-small retrained at 3 epochs (matching A-v3)

---

## Dataset version history

| Version | Dir | Train n | Eval n | Dedup'd | Split matches A/B/C | Notes |
|---|---|---|---|---|---|---|
| Original (leaky) | datasets/ (overwritten) | 3491 | 388 | No | Yes (A/B/C shared it) | 128 duplicates, 13 train/eval overlaps |
| Rebuilt (dedup'd) | datasets/ (current) | 3375 | 376 | Yes | Yes | Invariant checker: 50/50 pass |
| Per-operator | datasets_perop/ | 3375 | 376 | Yes | Yes (same split) | For A-v3; gen:FullName tokens |
| Old flat | (overwritten) | 3627 | 402 | No | No (different split) | SUPERSEDED by rebuilt flat |

---

## Embedding extraction history

| Extraction | Dir | Datasets used | Variants | Bug? | Notes |
|---|---|---|---|---|---|
| DEC-025 probing | runs/probing/ | Original (4029) | A, flat (IR tokens to all) | Yes — IR tokens to B/C | Only A and flat extracted |
| H6 first (invalid) | runs/retrieval_embeddings/ (overwritten) | Mixed (A=dedup'd, others=leaky) | A, B_small, B, C, flat | No (per-variant input_ids) | INVALID — mismatched dataset versions |
| H6 validated | runs/retrieval_embeddings/ (current) | Rebuilt (3375/376) | A, B_small, B, C, flat | No | All shapes match |
| A-v3 | runs/retrieval_embeddings/ (current) | Per-op corpus + dedup'd split | A_v3 | No | IR tokens via Qwen BPE tokenizer |

---

## Rules to prevent contamination

1. **Never overwrite a dataset directory.** New builds go to `datasets_v{N}/` or `datasets_{mode}/`. The `datasets/` dir is the current canonical version; previous versions are preserved with a suffix.

2. **Never overwrite an embeddings directory.** Each extraction round goes to `runs/retrieval_embeddings_v{N}/`. The current one is preserved.

3. **Every new training run gets a new directory.** `runs/variant_{V}_{config}/`, not overwriting an existing one. Example: `variant_A_v3_2ep/` for the 2-epoch retrain.

4. **Run the invariant checker before every extraction and eval.** Not optional.

5. **Record the run in this registry before starting.** Assign a run ID, note the config, then run.

6. **Manifests must be written by the training script.** Until the manifest system is wired in, manually create a manifest after each run using `manifest.py`.
