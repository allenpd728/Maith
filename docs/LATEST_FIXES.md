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
  is not yet implemented (`Transpiler.lean` is debug-only).

---

## 2026-07-27 — Matched-batch fix, documentation hardening

- Full matched-batch A/B/C rerun initiated after DEC-007 fix.
- Results tables updated in `README.md` and `docs/EXPERIMENT_DESIGN.md`.
- `docs/PHASE_5_RESULTS.md` created with `[PENDING]` placeholders for clean rerun numbers.
- `PHASE_5_COMPLETION_CHECKLIST.md` updated to reflect current rerun status.
