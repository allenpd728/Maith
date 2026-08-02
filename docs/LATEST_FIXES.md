# Latest Fixes

For recent sessions only. Older entries are in `docs/CHANGELOG.md`.
For the full session-by-session narrative log, see `docs/SESSION_PROGRESS.md`.

---

## 2026-07-28 (session 2) — DEC-008 result, DEC-009 implementation

### DEC-008 complete: 3-epoch variant A

- 3-epoch variant A run complete: perplexity = **1.2598** (67.6 min, 831 steps).
- Cold-start initialization (DEC-006) confirmed as a real factor — A improved 1.39 → 1.26.
- Gap to B/C (1.15/1.13) persists; cold-start is not the complete explanation.
- DEC-008 status updated to `complete` in `docs/DECISION_LOG.md`; result added to `docs/PHASE_5_RESULTS.md`.

### DEC-009: embedding warm-start implementation

- Added `--warm-start-embeddings` flag to `python/train.py`.
- New `_apply_warm_start_embeddings()` function: finds tokens whose string form appears in both
  the Qwen BPE vocabulary and `vocab_A.json`, copies pretrained embedding and lm_head vectors
  for those tokens into the resized variant-A embedding table. All weights remain trainable.
- Overlap count and fraction logged to `results.json` (`warm_start_overlap_count`,
  `warm_start_overlap_fraction`).
- DEC-009 drafted in `docs/DECISION_LOG.md` with full interpretation framework.

### Documentation

- `PHASE_5_COMPLETION_CHECKLIST.md` updated: DEC-008 marked complete, DEC-009 in progress.
- `README.md` roadmap Phase 5 entry updated to reflect current confound-elimination state.

---

## 2026-07-28 — Pre-flight checks, regression tests, --trace flag, documentation

### Experiment integrity

- Added `python/preflight_check.py` — pre-flight script to run before any A/B/C training run.
  Checks: (1) effective batch sizes match across all variants, (2) vocab size consistency
  (tokenizer vs model config vs embedding rows) for each variant, (3) forward pass produces
  finite loss on a small real batch. Passed 7/7 checks before the matched-batch rerun started.
- Added `python/test_train_regression.py` — regression tests for `train.py` targeting the
  failure modes that have already cost multi-hour runs. Covers: `collate_fn` label invariant,
  `seq_len=1` and `seq_len=2` edge cases (the original label-shift bug), NaN loss detection,
  and `VARIANT_BATCH_CONFIG` effective batch size matching. 6/6 pass, CPU-only, no GPU needed.

### Batch size fix (DEC-007)

- `VARIANT_BATCH_CONFIG` in `train.py`: B and C bumped from `grad_accum=4` to `grad_accum=8`,
  matching A's effective batch size of 8. Previous runs had B/C at effective batch 4 (twice as
  many gradient steps as A), which was an uncontrolled variable. Documented in DEC-007.
- Removed misleading `(all variants matched)` from the batch-size print in `train.py`.

### Lean Expr trace flag

- Added `--trace <declName>` flag to `Scripts/BuildCorpus.lean`. When passed, prints three
  representations of the named declaration's elaborated `Expr` before pipeline runs:
  `[TRACE:leanExpr]`, `[TRACE:dbgToString]`, `[TRACE:reprStr]` (full constructor tree).
  Implementation spans `Scripts/BuildCorpus.lean`, `Maith/MathlibCorpusBuilder.lean`, and
  `Maith/ProcessingPipeline.lean`.

### Documentation

- Added `docs/EXAMPLE_ROUNDTRIP.md` — full pipeline walkthrough for `neg_neg` with explicit
  provenance labels on every stage (direct corpus read / live run / illustrative).
- Updated `docs/DECISION_LOG.md` with DEC-007 (unmatched effective batch sizes).
- Updated `README.md` and `docs/EXPERIMENT_DESIGN.md` results tables with real 2026-07-27
  full-run numbers (A=1.3922, B=1.1420, C=1.1298, all `smoke_test: false`) and confound flags.
- Added round-trip scope clarification to `docs/EXAMPLE_ROUNDTRIP.md`, `docs/Design.md`,
  and `README.md`: token↔graph losslessness is verified; graph→Lean-syntax reconstruction
  is now implemented via `Transpiler.lean`'s `Decompile.decompileGraph` function.

---

## 2026-08-02 — DEC-018 resolved, Phase 5 closed, documentation cleanup

### DEC-018 complete: equal-sequence-length perplexity rerun

- Equal-sequence-length evaluation confirms B/C lead A is not a truncation artifact.
- Matched 512-token cap results: A=1.489, B=1.180, C=1.207.
- Long-bucket (512+ tokens): A=1.168, B=1.077, C=1.083 — gap persists.
- Provisional warnings removed from `README.md` and `docs/EXPERIMENT_DESIGN.md`.
- DEC-018 status updated to `resolved` in `docs/DECISION_LOG.md`.

### Phase 5 final picture

- **Perplexity (matched 512-token cap)**: A 1.489, B 1.180, C 1.207 — B/C lead in every bucket
- **Completion accuracy**: A 86.2%, B 92.8%, C 91.4% — same direction, same gap
- **DEC-006 still open**: whether the advantage reflects representation quality or warm-start is unresolved

### Documentation updates

- `README.md` Phase 5 entry marked ✅ complete.
- `PHASE_5_COMPLETION_CHECKLIST.md` deprecated, all items marked complete.
- `docs/PHASE_5_RESULTS.md` DEC-017/018 sections filled in.
- `docs/DECOMPILER_HANDOVER.md` created with full decompiler implementation details.

### Python compatibility fixes

- Fixed Python 3.9 compatibility across all Python scripts: replaced PEP 604 union types (`dict | None`) with `Optional[dict]`, added `List` imports, UTC fallback.
- Committed as `d486cd0`.

---

## 2026-08-01 — DEC-017 follow-up eval, DEC-016 corpus expansion

### DEC-017: stratified perplexity and completion accuracy

- Follow-up eval on expanded corpus checkpoints (variant_A_v3, variant_B2, variant_C_v2).
- Stratified perplexity by sequence length bucket (short/medium/long at 512 cap).
- Completion accuracy (200 examples, mask_last=10): A=86.2%, B=92.8%, C=91.4%.
- Results recorded in `docs/DECISION_LOG.md` and `docs/PHASE_5_RESULTS.md`.

### Corpus expansion (DEC-015)

- Expanded from 4 to 14 Mathlib modules (2,554 → ~8,000 declarations).
- Vocab A expanded from 4,495 to 8,102 tokens.
- Datasets rebuilt: `datasets/train_A.jsonl`, `datasets/eval_A.jsonl`, etc.
- DEC-015 status: complete.

---

## 2026-07-29 — DEC-010/011/012 eval_completion.py fixes

### DEC-010: eval_completion.py bugs identified and fixed

- Bug 1: logit indexing loop broke after first token — every run evaluated exactly one token.
- Bug 2: Variant C's tested position biased toward high-frequency delimiter token.
- Both bugs fixed in `python/eval_completion.py`.
- Prior results retracted; corrected results obtained.

### DEC-011/012: corrected completion accuracy

- Corrected results: A=89.7%, B=94.1%, C=94.6% (200 examples, mask_last=10).
- Gap reduced from ~35pp (buggy) to ~5pp (corrected).
- DEC-011/012 status: complete in `docs/DECISION_LOG.md`.

### DEC-013: training stability

- Added batch=1 edge case handling.
- Added epoch checkpointing (`--save-per-epoch`).
- Added gradient checkpointing support.
- Committed as `3a30019`.


