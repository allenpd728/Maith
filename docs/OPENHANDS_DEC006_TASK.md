# OpenHands Task: DEC-006 Pre-Experiment Hardening

## Context

This is the Maith project — a research codebase testing whether a custom
intermediate representation (IR) for Lean/Mathlib theorem proving helps small
language models learn mathematical structure more efficiently than raw BPE
tokenization.

**Repository:** https://github.com/allenpd728/Maith  
**Branch:** `kit/dev`  
**Repo path on host machine:** `~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo`

The next experiment is **DEC-006**: isolating the cold-start embedding confound
for Variant A. Variants B and C use Qwen2.5-Coder-0.5B's pretrained BPE
embeddings unchanged. Variant A resizes the embedding table from 151,643 tokens
to 8,144 IR tokens — those new embeddings are randomly initialized, giving B/C
an unfair head start unrelated to representation quality.

The fix is to initialize Variant A's embedding table by **projecting Qwen's
pretrained embeddings** into the IR vocab space, rather than starting from
random or using the existing `--embed-pretrain` warm-start (which only
partially mitigates the confound).

`train.py` already has `--embed-pretrain` (embed-only training for 1 epoch
before full fine-tune). It does NOT yet have `--embed-project`. That is what
you are building.

The existing Python test suite has 27 tests, all passing. Do not break them.
Run `python3 python/test_train_regression.py` and the other test files before
and after your changes to confirm.

---

## Your Tasks

Work on the `kit/dev` branch. Commit each task separately with a descriptive
message. Push to `origin kit/dev` when done.

---

### Task 1 — Write `python/embed_project.py`

Create a standalone script that builds the embedding projection matrix and
saves it to disk. `train.py` will load this matrix at startup when
`--embed-project` is passed.

**What it must do:**

1. Load the base model (`Qwen/Qwen2.5-Coder-0.5B`) and extract its embedding
   matrix `E_qwen` of shape `(151643, hidden_dim)`.

2. Load `datasets/vocab_A.json` (the IR vocabulary: a dict mapping token
   string → int ID).

3. For each IR token in `vocab_A.json`, find the best matching Qwen BPE token
   using one of these strategies (implement both, select via `--strategy` flag):
   - **`exact`** — if the IR token string exists verbatim in Qwen's tokenizer
     vocabulary, use that token's embedding directly.
   - **`mean`** — tokenize the IR token string with Qwen's BPE tokenizer, then
     take the mean of the resulting subword embeddings. This handles IR tokens
     like `HMul.hMul` that BPE splits into multiple pieces.

4. Build the projected matrix `E_proj` of shape `(vocab_A_size, hidden_dim)`.
   For tokens with no match, fall back to the mean of the full Qwen embedding
   matrix (not zeros — zeros produce degenerate behavior).

5. Save `E_proj` as a `.pt` file (using `torch.save`) to a path specified by
   `--output` (default: `datasets/embed_proj_A.pt`).

6. Print a summary: how many tokens matched exactly, how many used mean
   subword, how many fell back to global mean, and the L2 norm of the
   projection matrix (sanity check that it's not all zeros).

**CLI interface:**
```
python3 python/embed_project.py \
    --vocab datasets/vocab_A.json \
    --output datasets/embed_proj_A.pt \
    --strategy mean
```

**Important:** This script downloads the base model weights (HF Hub). The
`HF_TOKEN` env var is set in the repo's `.env` file if needed. The script
should handle the case where the model is already cached.

---

### Task 2 — Add `--embed-project` flag to `train.py`

Wire up the projection matrix into `train.py`'s Variant A initialization path.

**Changes needed in `load_model_for_variant()`** (around line 258):

After `model.resize_token_embeddings(vocab_size)`, add a branch:

```python
if embed_project_path and os.path.exists(embed_project_path):
    print(f"  Loading embedding projection from {embed_project_path} ...")
    E_proj = torch.load(embed_project_path, map_location="cpu")
    assert E_proj.shape == (vocab_size, model.config.hidden_size), \
        f"Projection shape mismatch: {E_proj.shape} vs ({vocab_size}, {model.config.hidden_size})"
    with torch.no_grad():
        model.model.embed_tokens.weight.copy_(E_proj)
        # Initialize lm_head to the transpose of the embedding projection
        # (weight tying convention — same as Qwen's original design)
        if model.lm_head.weight.shape == E_proj.shape:
            model.lm_head.weight.copy_(E_proj)
    print(f"  Embedding projection applied. Norm: {E_proj.norm():.2f}")
```

**Changes needed in `run()` and `main()`:**

- Add `embed_project: str = ""` parameter to `run()`.
- Pass `embed_project_path=embed_project` to `load_model_for_variant()`.
- Add `--embed-project` CLI argument (string path, default `""`).
- Log `embed_project` path to `results.json` alongside `embed_pretrain`.
- `--embed-project` and `--embed-pretrain` should be mutually exclusive: if
  both are passed, print an error and exit.

**Smoke-test behavior:** When `--smoke-test` is passed alongside
`--embed-project`, still apply the projection (it's initialization, not
training) but skip the actual training as normal.

---

### Task 3 — Write `python/test_embed_project.py`

Unit tests for the projection script. CPU-only, no GPU required, complete in
under 5 seconds. Use the same self-contained pattern as
`python/test_train_regression.py` (no pytest dependency — just a `main()`
with `ok()/fail()` helpers and `sys.exit(1)` on failure).

**Tests to include:**

1. **Shape test** — run `embed_project.py` on a tiny synthetic vocab (10
   tokens, fabricated strings) against a tiny synthetic embedding matrix
   (not the real Qwen model — use a fixture). Verify output shape is
   `(10, hidden_dim)`.

2. **Exact match test** — if a token string maps exactly to a known index in
   the source vocab, verify the projected embedding equals the source
   embedding for that row.

3. **Mean strategy test** — for a token that splits into 2 subword pieces,
   verify the projected embedding equals the mean of those 2 subword
   embeddings (within floating-point tolerance).

4. **Fallback test** — for an unknown token with no subword match, verify the
   projected embedding equals the global mean of the source matrix (not zeros).

5. **Norm sanity test** — verify the output matrix has a non-zero L2 norm
   (catches the degenerate all-zeros case).

6. **Shape mismatch detection in `train.py`** — using the existing
   `FakeCausalLM` pattern from `test_train_regression.py`, verify that if a
   projection file with wrong shape is passed to the `load_model_for_variant`
   path, it raises `AssertionError` cleanly rather than silently corrupting
   the embedding table.

Run the tests at the bottom:
```python
if __name__ == "__main__":
    print("Running embed_project tests...\n")
    test_shape()
    test_exact_match()
    test_mean_strategy()
    test_fallback_to_global_mean()
    test_norm_nonzero()
    test_shape_mismatch_raises()
    ...
```

---

### Task 4 — Add a DEC-006 smoke-test validation script

Write `python/validate_dec006_setup.py`. This is a pre-flight check to run
before committing GPU time. It should:

1. Confirm `datasets/vocab_A.json` exists and is valid JSON with the expected
   structure (dict of str → int).

2. Confirm `datasets/embed_proj_A.pt` exists (or accept a `--proj-path`
   override). Load it and verify shape `(N, 1024)` where N matches
   `len(vocab_A.json)`. Print the matrix norm.

3. Confirm `datasets/train_A.jsonl` and `datasets/eval_A.jsonl` exist and
   are non-empty.

4. Do a 1-step smoke run of `train.py` with `--embed-project` using
   `--smoke-test`, capturing stdout. Verify the output contains:
   - `"Embedding projection applied"`
   - `"Variant A"`
   - No `"Error"` or `"Traceback"` strings.

5. Print a final `✅ DEC-006 setup verified` or `❌ N checks failed` summary.

**CLI interface:**
```
python3 python/validate_dec006_setup.py --proj-path datasets/embed_proj_A.pt
```

---

### Task 5 — Verify `corpus_expansion_dry_run.py` still works

`python/corpus_expansion_dry_run.py` is required for DEC-019 (the diversity
track) but hasn't been run since the corpus expansion work. Without running a
full Lean build:

1. Read the script and confirm it parses correctly (no import errors).
2. Check that the config file it reads exists and has the expected keys
   (`baseline_modules`, `expansion_candidates`).
3. Confirm the candidate module list in that config matches what's documented
   in `docs/FUTURE_WORK.md` (Mathlib.Topology.Basic, Mathlib.Order.Lattice).
4. Add a `python/test_corpus_expansion_config.py` test that:
   - Loads the config JSON.
   - Verifies `expansion_candidates` is a non-empty list.
   - Verifies the two expected topology/order modules are present.
   - Verifies no module appears in both `baseline_modules` and
     `expansion_candidates` (collision check).

---

### Task 6 — Update documentation

After all the above is working and tested:

1. **`docs/DECISION_LOG.md`** — find the DEC-006 entry and add an
   "Implementation status" note at the bottom:
   ```
   **Implementation status (2026-08-02):**
   `python/embed_project.py` and `--embed-project` flag in `train.py` are
   implemented and tested. Run `python3 python/validate_dec006_setup.py`
   before the full training run.
   ```

2. **`README.md`** — update the Python test suite inventory table to include
   the new test files:
   - `python/test_embed_project.py` — N tests — embedding projection shape, match, fallback, norm
   - `python/test_corpus_expansion_config.py` — N tests — DEC-019 corpus config validation

3. **`docs/FUTURE_WORK.md`** — update item 1 (DEC-006) status from
   `Not started` to `Implementation complete — ready to run`. Update the
   implementation notes to reference the new script and flag.

---

## Constraints and conventions

- **CPU-only tests.** All test files must run without a GPU, without
  downloading model weights, and in under 10 seconds. Use synthetic fixtures
  (small random tensors, fabricated vocabs) rather than the real Qwen model.
- **No pytest dependency.** Use the self-contained pattern from
  `test_train_regression.py` — `ok()/fail()` helpers, `sys.exit(1)` on
  failure, runnable directly with `python3`.
- **Do not modify existing tests.** Add new test files; do not change
  `test_train_regression.py`, `test_build_dataset.py`, `test_variant_config.py`,
  or `test_pipeline_integration.py`.
- **Do not modify the training datasets or corpus.** All changes are in
  `python/` and `docs/`.
- **Commit each task separately** with a short descriptive message. Push to
  `origin kit/dev` at the end.
- **Run the full existing test suite before pushing** and confirm 27/27 pass:
  ```
  python3 python/test_train_regression.py
  python3 python/test_build_dataset.py
  python3 python/test_variant_config.py
  python3 python/test_pipeline_integration.py
  ```

## Definition of done

- `python/embed_project.py` exists and runs to completion on the real vocab.
- `--embed-project` flag works in `train.py` for Variant A.
- `--embed-project` and `--embed-pretrain` are mutually exclusive.
- `python/test_embed_project.py` passes (all tests green).
- `python/validate_dec006_setup.py` runs and reports ✅.
- `python/test_corpus_expansion_config.py` passes.
- All 27 existing tests still pass.
- Docs updated in DECISION_LOG.md, README.md, and FUTURE_WORK.md.
- All changes committed to `kit/dev` and pushed to origin.
