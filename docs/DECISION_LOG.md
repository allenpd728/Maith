# Experiment Decision Log

Canonical record of experiment-critical decisions that affect interpretation and reproducibility.

## Usage

- Add a new entry whenever an experiment assumption, protocol, or interpretation rule changes.
- Keep entries append-only; supersede by adding a newer entry that references the old one.
- Use explicit decision IDs so tooling/docs can reference stable anchors.

## Entries

### DEC-001: A/B/C variant semantics
- Date: 2026-07-26
- Status: accepted
- Scope: dataset/build + training/evaluation
- Decision:
  - Variant A uses Maith IR tokens (`Encoder.lean` v1.2.0) with custom `vocab_A.json`.
  - Variant B uses raw `leanExpr` text with native Qwen2.5-Coder BPE.
  - Variant C uses AST-style split `leanExpr` with native Qwen2.5-Coder BPE.
- Rationale:
  - Separates representation effect (A/B/C) from tokenizer-family effect (B/C share tokenizer family).
- References:
  - `docs/Design.md#ab-c-variant-definitions-current-experiment`
  - `docs/EXPERIMENT_DESIGN.md#variants`

### DEC-002: Shared eval cap for apples-to-apples perplexity
- Date: 2026-07-26
- Status: accepted
- Scope: evaluation/comparison
- Decision:
  - Eval perplexity comparisons across A/B/C must use a shared fixed eval cap (`eval_seq_len_cap=512`).
- Rationale:
  - Prevents sequence-length truncation differences from confounding perplexity comparisons.
- References:
  - `docs/EXPERIMENT_DESIGN.md#evaluation`
  - `python/check_results_gate.py`

### DEC-003: Embedding-table confound handling
- Date: 2026-07-26
- Status: accepted
- Scope: model architecture interpretation
- Decision:
  - Use fixed base architecture as primary comparison strategy, allowing A's embedding table to resize
    to custom vocab while B/C retain Qwen tokenizer/embedding size.
- Rationale:
  - Keeps most architecture components constant while testing representation directly.
- References:
  - `docs/EXPERIMENT_DESIGN.md#known-confound-embedding-table-size`

### DEC-004: Decision-grade publication gate
- Date: 2026-07-26
- Status: accepted
- Scope: reporting/publication
- Decision:
  - Results are publishable only after strict full-run checks pass (non-smoke A/B/C, gate pass, readiness pass).
- Rationale:
  - Avoids over-interpreting smoke runs.
- References:
  - `python/run_postrun_pipeline.py`
  - `python/check_publish_readiness.py`
  - `docs/EXPERIMENT_DESIGN.md#finalization-checklist-publishability-gate`

### DEC-005: Context-pack sidecar protocol for proof-context gap
- Date: 2026-07-26
- Status: accepted
- Scope: dataset/context scaffolding
- Decision:
  - Address cross-declaration proof-context risk via additive sidecar artifacts (`context_refs`,
    `dependency_refs`, `typeclass_context`) rather than changing core A/B/C rows immediately.
- Rationale:
  - Preserves current experiment comparability while enabling controlled context-aware variants.
- References:
  - `docs/CONTEXT_PACK_PROTOCOL.md`
  - `python/scaffold_context_artifacts.py`
  - `python/build_dependency_manifest.py`

### DEC-006: Known confounds that disadvantage A's absolute perplexity
- Date: 2026-07-27
- Status: accepted
- Scope: model architecture interpretation / training protocol
- Decision:
  - Two structural confounds must be ruled out before interpreting A vs B/C perplexity differences
    as evidence for or against the representation hypothesis:

  **Confound 1 — Randomly initialized embeddings (primary).**
  Variant A's embedding and output (unembedding) layers are randomly initialized due to the vocab
  resize from 151,643 to 4,495 tokens. Variants B and C retain Qwen's pretrained embeddings in
  full. This gives B/C a substantial head start on next-token prediction that is unrelated to
  representation quality, and the disadvantage is largest at small training-data scale where the
  model has limited opportunity to recover. If A underperforms B/C on perplexity, random
  initialization must be ruled out as the primary cause before concluding the representation
  hypothesis is false.

  **Confound 2 — Differing train sequence caps (secondary).**
  Variant A trains with a 1,024-token sequence cap; variants B/C train with a 384-token cap. This
  asymmetry is intentional — the tighter B/C cap was introduced as a memory-pressure fix for MPS
  (BPE on IR text inflates sequence length ~2.6x). Eval uses a shared 512-token cap across all
  three variants, so the final comparison metric is apples-to-apples. However, A receives longer
  training context per example than B/C do, which is an uncontrolled variable in training exposure.
  This is on record so it is not mistaken for a neutral design choice in future analysis.

- Rationale:
  - Naming confounds before results arrive prevents them from becoming convenient post-hoc
    explanations in either direction. Consistent with the discipline established in DEC-002/003.
- References:
  - `docs/EXPERIMENT_DESIGN.md#known-confound-embedding-table-size`
  - `python/train.py` (MAX_SEQ_LEN / MAX_SEQ_LEN_C constants)

### DEC-007: Effective batch size differs across variants (unmatched)
- Date: 2026-07-27
- Status: accepted
- Scope: training protocol / experiment validity
- Decision:
  - VARIANT_BATCH_CONFIG sets batch_size=2 for A and batch_size=1 for B/C, producing effective
    batch sizes of 8 vs 4 respectively. The print statement "(all variants matched)" in train.py
    was incorrect and has been removed.

  **Consequence for the full run (2026-07-27):**
  Variant A received ~277 gradient steps; B/C received ~554 steps
  (2,213 examples / 8 = 277; / 4 = 554). A received half the parameter updates of B/C
  regardless of representation quality. This is an additional confound — alongside DEC-006's
  embedding-init confound — that must be ruled out before concluding that A's higher perplexity
  (1.39 vs 1.13-1.14) reflects a failure of the IR representation hypothesis.

  **Rationale for the asymmetry:**
  A uses a 4,495-token vocab with smaller embeddings, which allows a larger per-device batch size
  within MPS memory limits. B/C carry 151k-token embedding tables (494M params) and require
  batch_size=1.

  **Resolution for next run:**
  Match gradient steps explicitly. Recommended: set B/C to grad_accum=8 (batch_size=1, effective 8)
  to match A's effective batch of 8. Alternatively train for multiple epochs to give A equivalent
  update exposure. Do not interpret perplexity differences as hypothesis evidence until step counts
  are matched or the asymmetry is explicitly discounted.

- References:
  - python/train.py (VARIANT_BATCH_CONFIG, lines 88-90)
  - Full-run results 2026-07-27: A=1.39, B=1.14, C=1.13

### DEC-008: DEC-006 cold-start test — 3-epoch variant A run
- Date: 2026-07-28
- Status: complete
- Scope: experiment interpretation / confound elimination
- Decision:
  - After the matched-batch rerun (2026-07-28: A=1.39, B=1.15, C=1.13), the A/B/C gap holds.
    DEC-007 (unmatched batch sizes) is ruled out as the primary cause. DEC-006 (randomly
    initialized embeddings for A vs pretrained for B/C) remains unaddressed.

  **Test protocol:**
  Run variant A for 3 epochs, all other hyperparameters unchanged, output to `runs/variant_A_3ep`.
  Compare the 3-epoch A perplexity against the 1-epoch B/C baseline (1.15/1.13).

  ```
  python3 python/train.py --variant A --datasets datasets/ --out runs/variant_A_3ep --epochs 3
  ```

  **Result (2026-07-28):**
  3-epoch variant A eval perplexity = **1.2598** (67.6 minutes, 831 train steps).

  **Interpretation:**
  1.2598 is a meaningful drop from 1.39 (1-epoch), confirming that cold-start embedding
  initialization is a real factor. However, it remains above the "cold-start dominant" threshold
  of ~1.25, and the gap to B/C (1.15/1.13) persists. Cold-start accounts for a significant
  portion of A's underperformance but not all of it. The representation may also be contributing.

  **Conclusion:**
  DEC-006 is partially confirmed as a confound. The cold-start penalty is real but not the
  complete explanation. An embedding warm-start experiment (DEC-009) is required to isolate
  representation quality from initialization advantage more cleanly.

  **Why 3 epochs (not embedding warm-start):**
  3-epoch test was zero-code-change, self-contained, and interpretable as a first probe.
  Embedding warm-start is the cleaner structural fix and is the recommended next step.

  **Perplexity summary (on record before completion-eval result):**
  - 1-epoch A (matched-batch): 1.39
  - 3-epoch A: 1.2598
  - Delta: −0.13 (~9% improvement)
  - 1-epoch B/C baseline: 1.15 / 1.13
  - Residual gap after 3 epochs: A still 0.11–0.13 above B/C
  - Status: partial closure. Cold-start is a real factor. Gap persists. Representation
    contribution cannot yet be separated from remaining initialization disadvantage.
  - Completion accuracy against the 3-epoch checkpoint (checkpoint-831) is pending and
    may change the interpretation of how much of the residual gap is representation vs
    training exposure.

- References:
  - Matched-batch rerun results 2026-07-28: A=1.39, B=1.15, C=1.13
  - 3-epoch result 2026-07-28: A=1.2598
  - docs/PHASE_5_RESULTS.md
  - python/train.py (`--epochs` flag, `epochs_override` parameter)

### DEC-011: eval_completion.py fix verification — corrected eval confirmed valid
- Date: 2026-07-29
- Status: accepted
- Scope: evaluation pipeline / result validity

**Bug recap (DEC-010):**
The original `evaluate_completion` loop used a logit-position index (`logit_pos = prefix_len - 1 + k`)
that immediately exceeded `logits.shape[0]` at k=1, causing a break after the first token.
Every run evaluated exactly 1 prediction per example regardless of `--mask-last`.

**Fix — autoregressive teacher-forcing loop:**
The corrected loop iterates directly over `targets = input_ids[prefix_len : prefix_len + mask_last]`.
At each step it:
1. Feeds `current_ids` (initially `input_ids[:prefix_len]`) to the model
2. Takes `logits[0, -1].argmax()` — the prediction for the next position
3. Compares to `target_id` (ground truth)
4. Appends `target_id` (not the prediction) to `current_ids` for the next step (teacher forcing)

**Indexing math:**
For a sequence of length L with `mask_last=10`:
- `prefix_len = L - 10`
- `targets = input_ids[L-10 : L]` — exactly 10 tokens
- Loop runs 10 times; `ex_total` increments to 10
- Grand total across N examples = N × 10 (minus examples with len ≤ mask_last)

**Prediction scaling confirmed against actual output (2026-07-29 run):**
- A: 200 examples × 10 = 2,000 total predictions reported ✓
- B: 189 examples × 10 = 1,890 total predictions reported ✓  (11 examples too short, skipped)
- C: 197 examples × 10 = 1,970 total predictions reported ✓  (3 examples too short, skipped)

This matches exactly. The old buggy runs reported total_predictions = examples_evaluated (200/189/197),
i.e., 1 prediction per example. The fixed runs report total_predictions = examples × 10. The scaling
is confirmed.

**Corrected results (89.7/94.1/94.6) — provisional status:**
These results are valid under the fixed eval but carry one remaining caveat: Variant A used the
3-epoch checkpoint (checkpoint-831) while B and C used 1-epoch checkpoints (checkpoint-554). The
comparison is not matched on training budget. B and C at 3 epochs may score higher, potentially
widening the gap. These numbers are the best available result for A vs B/C but must be treated as
provisional until B and C are evaluated at matched epoch count.

- References:
  - python/eval_completion.py (corrected loop, lines ~216–230)
  - DEC-010 (bug documentation)
  - Corrected run output 2026-07-29: A=89.7%, B=94.1%, C=94.6%

### DEC-012: Regression test for DEC-010 eval_completion.py indexing bug
- Date: 2026-07-29
- Status: accepted
- Scope: evaluation pipeline / test coverage

**What the test covers:**
`python/test_eval_regression.py` adds 6 tests targeting the two bugs documented in DEC-010:

  1. **total_predictions scales with mask_last** — the single most direct regression guard.
     With the old buggy code, total_predictions == n_examples always. With the fix,
     total_predictions == n_examples * mask_last. This test asserts the correct invariant
     at mask_last = 1, 3, 5, and 10.

  2. **Short sequences skipped correctly** — examples with len(input_ids) <= mask_last
     must be excluded entirely from total_predictions and examples_evaluated.

  3. **Perfect predictor scores 100%** — end-to-end sanity check with a controlled model
     that always predicts the correct next token.

  4. **Worst predictor scores 0%** — complementary sanity check with a model that always
     predicts token 0 against sequences starting at token 10.

  5. **Teacher forcing uses ground truth** — RecordingModel logs every input_ids call.
     Verifies that at each step the prefix is extended by the ground-truth token (not the
     prediction), confirming that errors do not compound across the mask_last steps.

  6. **mask_last=1 backward compat** — with mask_last=1 the fixed eval and old buggy eval
     produce the same result (1 token per example). Confirms the fix is not a regression
     for the single-token case.

**Test design notes:**
  - CPU-only, model-free (FakeCausalLM / RecordingModel). Runs in < 1 second.
  - Same treatment as test_train_regression.py: imports only the function under test,
    no GPU, no dataset files required.
  - Test 1 would catch a DEC-010 regression immediately: if the break-on-first-token
    bug re-appeared, total_predictions would equal n_examples (10) instead of
    n_examples * mask_last (10, 30, 50, 100) for the four mask_last values tested.
  - Test 5 (teacher forcing) catches the subtler failure mode where the prefix is
    extended with the predicted token instead of the ground truth — which would make
    accuracy sensitive to error compounding rather than per-position accuracy.

**Run:**
  python3 python/test_eval_regression.py

**Result (2026-07-29):** 6/6 passed.

- References:
  - python/test_eval_regression.py
  - DEC-010 (bug documentation)
  - python/eval_completion.py (corrected evaluate_completion function)

### DEC-009: Embedding warm-start to isolate representation quality
- Date: 2026-07-28
- Status: complete
- Scope: experiment interpretation / confound elimination
- Decision:
  - DEC-008 established that cold-start embedding initialization partially explains A's
    underperformance but does not fully account for the gap to B/C. An embedding warm-start
    experiment is the next step to separate representation quality from initialization advantage.

  **Proposed protocol:**
  Before training variant A, copy pretrained Qwen2.5-Coder embedding vectors for any tokens
  whose string representation appears in both the Qwen vocabulary and the custom IR vocab
  (e.g., numeric literals, common identifiers). Randomly initialize the remainder. All other
  hyperparameters unchanged from the matched-batch rerun. Output to `runs/variant_A_warmstart`.

  **Interpretation framework:**
  - If warm-start A perplexity approaches B/C (≤1.15): embedding initialization was the
    primary cause; IR representation is competitive once initialized fairly.
  - If warm-start A perplexity remains above 1.25: the representation itself underperforms
    at this scale; investigate vocab size (4,495 tokens may be too small), sequence cap
    asymmetry, or structural information loss in the IR encoding.
  - If warm-start A perplexity is between 1.15 and 1.25: both factors contribute; further
    ablations needed.

  **Prerequisites:**
  - Identify token overlap between `vocab_A.json` and Qwen2.5-Coder BPE vocabulary.
  - Implement warm-start weight copy in `python/train.py` (new `--warm-start-embeddings` flag).
  - Verify that warm-started embeddings are trainable (not frozen) so A can still adapt.

  **Result (2026-07-29):**
  Warm-start overlap: **23 tokens out of 4,495 (0.51%)**.
  Warm-start variant A eval perplexity = **1.4288** (22.5 minutes, 1 epoch).

  **Interpretation:**
  The 0.51% overlap means the warm-start was effectively a cold start — 4,472 of 4,495 embedding
  rows remained randomly initialized. Perplexity is 1.43, marginally *worse* than the cold-start
  1-epoch result (1.39), consistent with mild interference from the 23 mismatched pretrained vectors.

  The warm-start approach cannot address DEC-006 at this vocab overlap level. The Qwen BPE
  vocabulary and Maith's custom IR vocabulary are structurally too different for embedding transfer
  to be meaningful.

  **Conclusion:**
  DEC-006 Confound 1 (randomly initialized embeddings) is now fully characterized:
  - Cold-start is a real factor (DEC-008: 3 epochs improved A from 1.39 → 1.26).
  - Warm-start cannot close the gap due to near-zero vocab overlap.
  - The residual gap between A (1.26 at 3 epochs) and B/C (1.15/1.13 at 1 epoch) is
    attributable to the IR representation itself, not initialization alone.

  **Next step:** Run `eval_completion.py` against the matched-batch B and C checkpoints to
  evaluate whether B/C's perplexity advantage translates to completion accuracy. Perplexity
  is an average-case metric; completion accuracy is a sharper, task-relevant signal.

- References:
  - DEC-006, DEC-008
  - docs/PHASE_5_RESULTS.md
  - python/train.py
  - runs/variant_A_warmstart/results.json

### DEC-010: eval_completion.py had two bugs — prior completion-accuracy results are invalid
- Date: 2026-07-29
- Status: accepted
- Scope: evaluation validity / result retraction

- Decision:
  The completion-accuracy results previously reported (A=56%/60.5%, B=94%/96.8%, C=100%/94.9%)
  are **invalid** and must not be used to support or refute the representation hypothesis.
  Two bugs were identified:

  **Bug 1 — Single-token-only evaluation (primary).**
  The logit indexing loop used logit_pos = prefix_len - 1 + k, but the forward pass only
  produced prefix_len logit rows (indices 0..prefix_len-1). At k=1, logit_pos = prefix_len,
  which immediately triggered the break guard. Every run — regardless of --mask-last value
  — evaluated exactly one token per example. The --mask-last flag changed only which
  single position was tested, not how many tokens were evaluated. "200 examples, mask-last 10"
  = 200 single-token predictions, not 2,000.

  **Bug 2 — Structurally biased test position (secondary, Variant C specific).**
  Token 13 (Qwen BPE structural delimiter) appeared at the tested position in 67/246 eval_C
  examples (27%) and is a high-frequency token (12,790 occurrences in eval_C, 15.2% of all
  tokens). Its preceding context is highly repetitive — 33/67 cases preceded by the exact same
  3-token pattern [1523, 48765, 23684]. A model could inflate accuracy on these positions by
  learning one n-gram. Variants A and B do not show equivalent concentration (A: token 98 at
  0.6% corpus frequency). This independently inflated C's score regardless of Bug 1.

  **Fix applied (2026-07-29):**
  - Replaced single-pass logit loop with autoregressive teacher-forcing loop evaluating each
    of the mask_last positions independently.
  - Added --checkpoint-A flag for direct evaluation against runs/variant_A_3ep.
  - Targets now read from input_ids[prefix_len:] directly, not the labels array.

  **Results retracted from PHASE_5_RESULTS.md:**
  - Completion accuracy, last 5 tokens, 50 examples: A=56%, B=94%, C=100%
  - Completion accuracy, last 10 tokens, 200 examples: A=60.5%, B=96.8%, C=94.9%
  - All interpretation and conclusions based on those numbers.
  - The "Phase 5 finding: IR representation hypothesis falsified" conclusion is premature
    and must be reassessed after the corrected eval.

  **Rerun protocol:**
  python3 python/eval_completion.py --checkpoint-A runs/variant_A_3ep --samples 200 --mask-last 10

  **Corrected result (2026-07-29):**
  Variant A (3-epoch, checkpoint-831): 89.7%  (1,794 / 2,000 tokens, 200 examples)
  Variant B (1-epoch, checkpoint-554): 94.1%  (1,779 / 1,890 tokens, 189 examples)
  Variant C (1-epoch, checkpoint-554): 94.6%  (1,863 / 1,970 tokens, 197 examples)

  **Key finding:**
  A trails B and C but the gap is far smaller than the buggy numbers suggested — 89.7% vs 94–95%,
  not 60% vs 95%. This materially changes the Phase 5 conclusion: the IR representation is
  competitive at this task, not strongly inferior. A's deficit over B/C is ~5 percentage points,
  which is consistent with the residual cold-start embedding penalty (DEC-006/008) rather than a
  fundamental failure of the representation hypothesis.

  **Comparison note:**
  A used 3-epoch checkpoint-831 (more training); B/C used 1-epoch checkpoint-554 (less training).
  This comparison still favors B/C — if A needed 3x the training to reach 89.7% while B/C hit
  94–95% at 1 epoch, the representation advantage of B/C is real. But "real and ~5pp" is a very
  different finding from "A is broken at 60%".

- References:
  - python/eval_completion.py (fix commit following 83ded87)
  - docs/PHASE_5_RESULTS.md (retracted results above, corrected results below)
  - DEC-008 (3-epoch A checkpoint)
  - DEC-009 (warm-start context)

---

### DEC-013: Training stability — batch size reduction, epoch checkpointing, gradient checkpointing

**Date:** 2026-07-30
**Status:** Applied

**Context:**
Variant A repeatedly died from thermal throttling on Apple Silicon (16 GB MPS). Root cause:
BATCH_SIZE=2 with float32 forward+backward passes on a 361M-param randomly-initialized model
saturated MPS compute units within 3-5 steps. The effective batch size (BATCH_SIZE x GRAD_ACCUM)
was kept at 8 throughout to preserve training dynamics.

**Changes applied:**

1. BATCH_SIZE 2 to 1, GRAD_ACCUM 4 to 8 (effective batch unchanged at 8).
   Halves peak per-step memory with no validity impact. Applies equally to A, B, and C.

2. save_strategy "no" to "epoch", save_total_limit=3.
   Saves a checkpoint after each epoch. A crash mid-epoch loses at most one epoch of progress.
   A --resume flag was added to the CLI to pick up from the latest saved checkpoint.
   Does not affect model outputs or comparisons.

3. gradient_checkpointing_enable(use_reentrant=False).
   Recomputes activations during backward pass instead of holding them in memory. Reduces peak
   MPS memory at the cost of ~20-30% more compute per step. use_reentrant=False is the
   MPS-safe variant.

   WARNING - Unverified equivalence assumption: "statistically equivalent" is a general claim
   about gradient checkpointing drawn from CUDA-scale literature. It has NOT been empirically
   verified for this specific setup:
   - Variant A: 361M params, randomly initialized embedding table
   - Variants B/C: 494M params (Qwen2.5-Coder-0.5B), pretrained weights intact
   - Dataset scale: ~2,200 training examples
   - Hardware: Apple Silicon MPS (non-determinism characteristics differ from CUDA)

   The non-determinism introduced by gradient checkpointing may manifest differently across A vs
   B/C due to their asymmetric initialization. The comparison remains relative (all three variants
   use gradient checkpointing), so the playing field is level. But absolute perplexity values
   from DEC-013-onward runs are not directly comparable to prior float32 deterministic runs.

**Impact on experimental validity:**
- A/B/C remain comparable to each other (all three use identical training configuration).
- Results from DEC-013-onward are not directly comparable to pre-DEC-013 runs (batch size
  and gradient checkpointing both changed).
- Prior results (DEC-008, DEC-010, DEC-012) used BATCH_SIZE=2, no gradient checkpointing.

- References:
  - python/train.py (this commit)
  - DEC-007 (original batch-size mismatch fix)
  - DEC-008 (3-epoch A results, pre-DEC-013 config)

---

## DEC-014: Proof-Term Completion Deferred to Phase 7; Next-Tactic Prediction Ruled Out

**Date:** 2026-07-31
**Status:** Recorded. No action taken on codebase.

**Decision:**
Full Mathlib coverage for proof autocompletion is a Phase 7 concern, not a Phase 5/6
concern. The IR is intentionally left unchanged. Three blockers were identified and
documented. Next-tactic prediction is ruled out permanently on architectural grounds.

**Summary of blockers:** See docs/PHASE_7_DESIGN.md for the full three-blocker table
and IR change requirements.

**Why next-tactic prediction is ruled out:**
Tactics exist only in the Syntax layer, which is discarded before elaboration. By the
time MetaExtractor.lean runs, there is no tactic string to recover. Supporting it would
require a completely separate extraction path intercepting the elaborator mid-run. This is
not an extension of Maith — it is a different project.

**Why proof-term completion is the right path:**
The current IR already extracts type structure correctly. Proof-term completion (type to
proof term) requires only targeted IR additions: remove the .thmInfo skip, raise the
tactic density filter, extend OperationOp for proof combinators. These changes are scoped
and do not invalidate Phase 5/6 results.

**What is not changing now:**
- MetaExtractor.lean — no changes
- maxTacticDensity filter — remains at 0.8
- constantValueExpr? — .thmInfo skip remains in place
- Phase 6 corpus expansion targets — unchanged

**References:**
- docs/PHASE_7_DESIGN.md (full design doc)
- Maith/MetaExtractor.lean (extraction logic)
- Maith/MathlibLoader.lean (tactic density filter)
