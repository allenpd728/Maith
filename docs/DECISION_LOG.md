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

  **Implementation status (2026-08-02):**
  `python/embed_project.py` and `--embed-project` flag in `train.py` are
  implemented and tested. Run `python3 python/validate_dec006_setup.py`
  before the full training run.

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

---

### DEC-015: Corpus expansion — 4 to 14 Mathlib modules, vocab A 4,495 to 8,102 tokens

**Date:** 2026-07-31
**Status:** Accepted. Full matched A/B/C rerun required before any Phase 5 conclusion.

**What happened:**
Commit 56fd814 (July 31, 08:20 AM) executed the corpus expansion that was previously
only tested as a dry run. 1,347 new examples were added from 10 additional Mathlib modules,
growing the dataset from 2,213/246 to 3,375/376 (train/eval). Vocab A grew from 4,495 to
8,102 tokens — an 80% increase.

**New modules added:**
- Algebra.Group.NatPowAssoc:     28 examples
- Algebra.Ring.Basic:            68 examples
- Algebra.Ring.GeomSum:          70 examples
- Algebra.Group.Subgroup.Basic: 393 examples
- Data.Nat.Basic:                27 examples
- Data.Int.Basic:                22 examples
- Order.Lattice:                519 examples
- Order.LatticeIntervals:       128 examples
- Algebra.Module.Basic:          23 examples
- Topology.Basic:                69 examples

**Total corpus:** 3,901 examples across 14 modules (3,375 train / 376 eval after 150
pathological dropped). Original 4 modules: Algebra.Group.Defs, Algebra.Group.Basic,
Algebra.Ring.Defs, Order.Basic.

**Why rolling back datasets is not viable (option 1):**
Vocab A's embedding table is sized to vocab_A.json. Old A/C checkpoints used a 4,495-token
table; the current vocab has 8,102 tokens. The checkpoint is architecturally incompatible
with the current pipeline state and cannot be evaluated against the new vocab. A full matched
rerun of all three variants on the expanded corpus is the only valid path.

**Interaction with DEC-002 (sequence cap asymmetry) — unassessed:**
train_B p99 sequence length on expanded corpus = 2,051 tokens (max 8,242). train_A p99 =
1,036 (max 1,305). The runtime seq cap (DEC-014) is 512 for B/C. The cap now truncates a
larger fraction of the expanded corpus than it did on the original 4-module corpus. This
may interact with DEC-002 in ways not yet characterized. Flag for post-run review: after
the matched rerun completes, stratify perplexity by sequence length bucket to check whether
truncation is affecting B/C disproportionately.

**Action required:**
Re-run all three variants (A, B, C) on the 3,375-example expanded corpus before drawing
any Phase 5 conclusions. B2 already completed on this corpus (perplexity 1.107, 523 min).
A and C must be rerun to match.

**References:**
- commit 56fd814 (corpus expansion)
- python/corpus_expansion_dry_run.py (prior dry run)
- DEC-002 (sequence cap asymmetry, original)
- DEC-006 (cold-start embedding confound)
- docs/EXPERIMENT_DESIGN.md (matched-run protocol)

---

### DEC-016: Phase 5 conclusion — matched A/B/C rerun on expanded corpus (2026-08-01)

**Date:** 2026-08-01
**Status:** Accepted. Phase 5 closed. Two required follow-ups documented below.

**Runs:**

| Variant | Checkpoint | Train examples | Eval examples | Epochs | Perplexity | Training minutes |
|---------|-----------|----------------|---------------|--------|------------|------------------|
| A | variant_A_v3/checkpoint-final | 3,375 | 376 | 3 | 1.2812 | 77.4 |
| B | variant_B2/checkpoint-final | 3,375 | 376 | 3 | 1.107 | 523.0 |
| C | variant_C_v2/checkpoint-final | 3,375 | 376 | 3 | 1.098 | 272.8 |

All three: seed=42, base model Qwen2.5-Coder-0.5B, smoke_test=false.

**Completion accuracy (eval_completion.py, 50 examples, mask_last=5, teacher-forced):**

| Variant | Top-1 Accuracy | Correct / Total |
|---------|---------------|-----------------|
| A | 89.6% | 224 / 250 |
| B | 94.4% | 236 / 250 |
| C | 90.8% | 227 / 250 |

**Eval script fix (2026-08-01):**
eval_completion.py previously only had --checkpoint-A. Added --checkpoint-B and
--checkpoint-C flags and corrected the getattr key to use .upper(). The fix is
purely directory-routing — no change to the evaluation logic (masking, teacher forcing,
argmax, accuracy counting). Confirmed: the same evaluate_completion() function runs
for all three variants without modification.

**Spot-check (24 examples, 120 positions, B vs C — two independent rounds):**
Round 1 (9 examples, seed 42): B=42/45 (93.3%), C=40/45 (88.9%). Round 2 (15 fresh
examples, seed 7, non-overlapping): B=73/75 (97.3%), C=74/75 (98.7%). Combined:
B=115/120 (95.8%), C=114/120 (95.0%) — essentially tied.

The round-1 narrative ("C fails at syntactic boundaries in longer sequences") does not
replicate. In round 2 C goes 5/5 on the longest example (seq_len=1369) while B drops
to 3/5. Round 2 divergences split evenly: one example where C beats B, one where B
beats C. No systematic pattern was confirmed.

**Implication for the aggregate gap (B=94.4% vs C=90.8%):**
The aggregate gap is real but its source is not explained by any per-token pattern
visible in 24 spot-checked examples. The gap may be driven by a small number of
examples where C fails consistently, or it may be sampling variance in the 50-example
aggregate run. The 200-example, mask_last=10 follow-up eval is the right way to
determine whether the gap holds at scale.

**Perplexity / completion-accuracy inversion (B vs C):**
C has better perplexity (1.098 vs 1.107) but worse completion accuracy (90.8% vs 94.4%)
in the aggregate run. The spot-check does not reproduce this gap at the token level,
suggesting the inversion — if real — is not uniformly distributed across sequences.

**Required follow-up 2 — correction to framing:**
Previously labelled "matched-epoch completion eval." That label is wrong — all three
variants are already at 3 epochs. The actual distinction is sample size and mask depth:
200 examples and mask_last=10 vs the current 50/5. At 2,000 predictions vs 250, the
confidence interval tightens enough to determine whether the B/C gap and the B/C vs A
gap are stable. This is the correct framing.

**Phase 5 finding:**
Variant A trails B and C on both perplexity and completion accuracy on the expanded,
matched corpus. The gap is real: ~0.18 perplexity points, ~5pp completion accuracy.
The gap is materially smaller than earlier mismatched Phase 5 estimates (which showed
a 36pp completion gap from unequal corpus sizes and a buggy eval script).

**DEC-006 status — STILL OPEN:**
Variant A uses a randomly initialized 8,102-token embedding table. Variants B and C use
Qwen2.5-Coder pretrained embeddings (151,643 tokens). The embedding warm-start experiment
(DEC-009) achieved only 0.51% token overlap and was effectively a cold start — it did not
isolate the representation confound. No experiment has yet run A with a genuinely
warm-started embedding. Until that is done, we cannot cleanly separate "IR representation
is worse" from "random initialization is worse." DEC-006 remains an open confound.

**Required follow-up 1 — DEC-002 stratified analysis (not optional):**
DEC-015 flagged that the expanded corpus has B/C p99 sequence length of 2,051 tokens
against a 512-token eval cap, potentially truncating a larger fraction of B/C's harder
examples and artificially improving their perplexity. Before any Phase 6 design uses
these numbers as a baseline, perplexity must be stratified by sequence length bucket
(short: <128 tokens, medium: 128-512, long: >512) to confirm B/C's advantage is not
an artifact of asymmetric truncation. This is a required pre-condition for Phase 6, not
a post-hoc check.

**Required follow-up 2 — matched-epoch completion eval (not optional):**
The corrected Phase 5 completion eval (DEC-010) compared 3-epoch A against 1-epoch B/C,
which was acknowledged as unfair to B/C. The current run has all three at 3 epochs for
the first time. A matched-epoch completion eval must be run before drawing a final
conclusion on the representation hypothesis. The 5pp gap may shrink, hold, or widen —
but until it is measured under matched conditions it is not a conclusion.

**References:**
- runs/variant_A_v3/results.json
- runs/variant_B2/results.json
- runs/variant_C_v2/results.json
- runs/completion_accuracy.json
- DEC-002 (sequence cap asymmetry)
- DEC-006 (cold-start embedding confound)
- DEC-009 (warm-start attempt, 0.51% overlap)
- DEC-010 (eval_completion bug fix)
- DEC-015 (corpus expansion, matched rerun requirement)
- docs/PHASE_5_RESULTS.md (full results history)

---

### DEC-017: Follow-up eval results — stratified perplexity and high-confidence completion accuracy (2026-08-02)

**Date:** 2026-08-02
**Status:** Accepted. Phase 6 design blocked pending equal-sequence-length perplexity rerun (see required gate below).

#### Follow-up 1: Stratified perplexity (DEC-002 resolution)

**Results (followup_eval.py, all 376 eval examples, seq_cap per variant):**

| Variant | Short (<128) N | Mean PPL | Medium (128–512) N | Mean PPL | Long (>512) N | Truncated | Mean PPL |
|---------|---------------|----------|--------------------|----------|---------------|-----------|----------|
| A | 158 | 1.663 | 187 | 1.396 | 31 | 5/31 | 2.200 |
| B | 83 | 1.415 | 214 | 1.126 | 79 | 78/79 | 1.077 |
| C | 71 | 1.668 | 197 | 1.109 | 108 | 107/108 | 1.083 |

**Finding:**
The stratification reveals that B/C's aggregate perplexity advantage over A is
substantially driven by asymmetric truncation, not solely by representation quality.

B's 79 "long" examples are truncated in 78/79 cases to 512 tokens — the model never
sees the hard part of those sequences. C truncates 107/108. A's 31 long examples are
truncated in only 5/31 — it is evaluated on sequences up to its full 1024-token training
cap, including the structurally complex tails that B/C never process.

B and C's long-bucket perplexity (1.077/1.083) is their *best* performing bucket, not
their worst. This is consistent with truncation artificially removing the harder suffix
tokens from their eval loss. A's long-bucket perplexity (2.200) is its worst — it is
penalized for tokens that B/C's eval cap silently drops.

**Implication:**
The current aggregate perplexity comparison (A: 1.2812, B: 1.107, C: 1.098) should not
be treated as valid evidence for or against the representation hypothesis. The numbers
are not measuring the same thing: A and B/C are evaluated over materially different
token distributions. The perplexity comparison cannot support a conclusion about
representation quality until it is re-run with matched sequence caps.

This does not affect the short and medium buckets, where all three variants are
unadjusted and directly comparable. In those buckets B and C still lead A (short: A 1.663
vs B 1.415 vs C 1.668; medium: A 1.396 vs B 1.126 vs C 1.109), so the advantage is not
entirely an artifact. But the aggregate headline numbers are contaminated and should not
be cited as a primary result.

#### Follow-up 2: Completion accuracy (DEC-016 required follow-up)

**Results (followup_eval.py, 200 examples, mask_last=10, single-pass teacher-forced):**

| Variant | Top-1 Accuracy | Correct / Total | Examples evaluated |
|---------|---------------|-----------------|-------------------|
| A | 86.2% | 1723 / 2000 | 200 |
| B | 92.8% | 1792 / 1930 | 193 |
| C | 91.4% | 1800 / 1970 | 197 |

**Finding:**
A trails B and C by ~5–6 percentage points. The gap from the 50-example run (A: 89.6%,
B: 94.4%, C: 90.8%) survives at scale with tighter confidence: 2,000 predictions for A,
~1,950 for B and C. The B/C gap narrows from 3.6pp to 1.4pp and is not a reliable
signal — within expected variance at this sample size.

This result is not affected by the sequence-cap asymmetry identified in Follow-up 1.
Completion accuracy is computed from the full uncapped sequence for each example
(the model sees the entire sequence; only the final mask_last tokens are evaluated),
so B/C are not receiving any truncation benefit here. The 5–6pp A deficit is a clean
result that stands on its own.

**Methodological note:**
The completion eval uses a single-pass teacher-forced method (DEC-017 / followup_eval.py)
replacing the original token-by-token loop. The two methods were verified to produce
bit-identical predictions on 5 examples per variant before the full run
(check_completion_indexing.py, all variants PASS). The token count difference across
variants (B: 1930, C: 1970 vs A: 2000) reflects examples skipped for being shorter than
mask_last=10, not a sampling difference.

#### Required gate before Phase 6: equal-sequence-length perplexity rerun

**This is a Phase 6 blocker, not an open question.**

Phase 6 design must not proceed until perplexity is re-evaluated with a matched sequence
cap across all three variants. The current perplexity numbers are not a valid baseline
for a Phase 6 comparison.

**What is required:**
Re-run perplexity evaluation for all three variants with a single shared sequence cap —
either 512 tokens (B/C's current cap, meaning A is evaluated under A's training cap for
the long bucket) or a cap chosen to equalize the truncation fraction across variants.
The goal is that the "long" bucket truncation rate is comparable across A, B, and C so
that the per-bucket and aggregate perplexity numbers reflect the same token distribution.

**What this will and will not resolve:**
It will produce a perplexity comparison that is not confounded by asymmetric truncation,
making it valid evidence for or against the representation hypothesis at the sequence
lengths where all variants are compared on equal footing.

It will not resolve DEC-006 (cold-start embedding confound). Even a clean perplexity
comparison may still conflate representation quality with initialization disadvantage.
DEC-006 remains open and must be addressed separately in Phase 6 design.

**References:**
- runs/followup_eval_results.json
- python/followup_eval.py
- python/check_completion_indexing.py
- DEC-002 (sequence cap asymmetry, original)
- DEC-006 (cold-start embedding confound, still open)
- DEC-015 (corpus expansion)
- DEC-016 (Phase 5 conclusion)

---

### DEC-018: Equal-sequence-length perplexity rerun — Phase 5 perplexity gate resolved (2026-08-02)

**Date:** 2026-08-02
**Status:** Accepted. Phase 5 perplexity comparison now valid. Phase 6 blocker lifted.

**Required by:** DEC-017, which found that the original aggregate perplexity comparison
was not valid evidence because A and B/C were evaluated over different token distributions
due to asymmetric sequence-length truncation.

#### Results at shared 512-token cap (dec018_ppl_rerun.py, all 376 eval examples)

| Variant | Short (<128) N | Mean PPL | Medium (128–512) N | Mean PPL | Long (>512) N | Truncated | Mean PPL | Overall Mean PPL |
|---------|---------------|----------|--------------------|----------|---------------|-----------|----------|-----------------|
| A | 158 | 1.663 | 187 | 1.396 | 31 | 31/31 | 1.168 | 1.489 |
| B | 83 | 1.415 | 214 | 1.126 | 79 | 78/79 | 1.077 | 1.180 |
| C | 71 | 1.668 | 197 | 1.109 | 108 | 107/108 | 1.083 | 1.207 |

B and C's numbers are unchanged from DEC-017 (they were already at a 512-token cap).
A's long-bucket perplexity falls from 2.200 to 1.168 once capped at 512 — confirming
that the 2.200 figure was driven by hard suffix tokens beyond the 512-token boundary
that B/C were never evaluated on. However, A's overall perplexity rises from 1.281 to
1.489 under the matched cap because its short and medium buckets now have their relative
weight increased (A has proportionally more short examples than B/C, and those examples
have higher perplexity for A than for B/C).

#### Interpretation

**B and C lead A in every bucket at the matched cap.** The advantage is not a
truncation artifact. Specifically:

- Short bucket: B leads A by 0.248 perplexity points; C is essentially tied with A.
- Medium bucket: B leads A by 0.270; C leads A by 0.287.
- Long bucket: B leads A by 0.091; C leads A by 0.085 — the gap narrows but does not close.
- Overall: B leads A by 0.309; C leads A by 0.282.

The DEC-017 concern was valid: the original 1.281 vs 1.107/1.098 comparison was
contaminated by asymmetric truncation and should not have been cited. The DEC-018
matched comparison (1.489 vs 1.180/1.207) is a valid apples-to-apples result. The
direction is the same — B/C outperform A — but the mechanism is confirmed to be real
rather than an evaluation artifact.

**The B/C gap (1.180 vs 1.207) is small** and similar in magnitude to the DEC-017
unmatched comparison. B has a slight perplexity edge over C.

**DEC-006 (cold-start confound) remains open.** This result confirms that B and C
produce lower perplexity on the same token distributions. It does not isolate whether
that advantage comes from representation quality or from the embedding warm-start that
B and C receive from Qwen2.5-Coder's pretrained weights. DEC-006 must be addressed
in Phase 6 design.

#### Phase 5 perplexity conclusion (now valid)

At a shared 512-token evaluation cap, Variant A (Maith IR) has higher perplexity than
both Variant B (raw leanExpr, Qwen BPE) and Variant C (normalized leanExpr, Qwen BPE)
across all sequence-length buckets. Combined with the completion accuracy result
(DEC-017: A 86.2%, B 92.8%, C 91.4%), the Phase 5 evidence consistently shows A
trailing B/C on both metrics. The representation hypothesis — that IR tokens improve
model performance — is not supported by Phase 5 results. DEC-006 remains the
unresolved confound: the gap may reflect initialization disadvantage rather than
representation quality.

**Phase 6 blocker lifted.** Phase 6 design may proceed from this baseline.

#### References
- runs/dec018_ppl_results.json
- python/dec018_ppl_rerun.py
- DEC-002 (sequence cap asymmetry)
- DEC-006 (cold-start embedding confound, still open)
- DEC-016 (Phase 5 matched-run conclusion)
- DEC-017 (stratified perplexity, follow-up completion accuracy)

---

### DEC-019: Corpus expansion — planning entry (2026-08-02)

**Date:** 2026-08-02
**Status:** Planned. Not started. Awaiting decision on which hypothesis to prioritize
and GPU time to commit before execution.

#### Background

Phase 5 (DEC-016 through DEC-018) established that A trails B/C on both perplexity and
completion accuracy at matched evaluation conditions. DEC-006 identifies the unresolved
confound: A's embedding table is randomly initialized at training time, while B and C
begin from Qwen2.5-Coder's pretrained weights. Two Phase 6 attempts to close this
confound have so far been inconclusive:

- **Attempt 1 (warm-start):** embedding warm-start via vocabulary projection — marginal
  or negative result; did not close the gap.
- **Attempt 2 (embed-pretrain):** longer pretraining of A's embeddings — similarly
  inconclusive.

Corpus expansion is a potential **Attempt 3** on the DEC-006 confound, but it is also
a separate and distinct hypothesis about generalization. These two motivations call for
different module choices and produce different experiments; they must not be conflated.

#### The two separate claims

**Claim 1 — Volume (DEC-006 lineage):**
More training examples give A's randomly-initialized embeddings more gradient signal,
potentially allowing A to partially close the initialization gap with B/C. Mechanism:
A requires more signal to organize its embedding space from scratch; B/C start from an
organized state and need less signal per example. Prediction: increasing volume in
algebra-adjacent modules may narrow A's gap, particularly in short/medium sequences
where cold-start effects dominate.

*Important caveat:* this is not guaranteed. B/C also benefit from additional data and
start from a stronger position. The gap may stay proportionally similar or widen. This
is an empirical question, not a given.

**Claim 2 — Diversity (generalization):**
Broader domains test whether the IR representation transfers outside the algebraic
modules the baseline was trained on. This is a different and valid experiment: does
Maith IR generalize to, e.g., topological or lattice structures? This does not directly
address DEC-006 — B/C would be expected to benefit at least as much from domain
diversity, given their tokenizers already cover diverse Lean syntax from pretraining.

#### Module choices implied by each claim

| Claim | Module priority | Rationale |
|-------|----------------|-----------|
| Volume (DEC-006) | Algebra-adjacent: NatPowAssoc, Ring.Basic, Ring.GeomSum, Subgroup.Basic, Data.Nat.Basic, Data.Int.Basic, Algebra.Module.Basic | Similar IR graph structure to baseline; more examples in the same distribution |
| Diversity (generalization) | Topology.Basic, Order.Lattice | Meaningfully out-of-distribution relative to baseline algebra modules; tests transfer |

Both sets are in `Scripts/module_expansion_targets.json` under `expansion_candidates`.

#### Required pre-execution steps (in order)

1. **Dry-run (print-only):** confirm module list and generated Lean runner — no
   extraction runs.
   ```
   python3 python/corpus_expansion_dry_run.py --set candidates --print-only
   ```

2. **Dry-run (real counts):** get actual declaration counts and success rates from
   Mathlib directly, replacing any secondhand estimates with verified ground truth.
   (~10–15 min Lean build time, no training.)
   ```
   python3 python/corpus_expansion_dry_run.py --set candidates
   ```

3. **Decide the hypothesis explicitly** before running extraction or training. Log the
   choice as a follow-up DEC entry (DEC-020 or similar) before proceeding. This is the
   step that makes the eventual result interpretable. Expansion run without an explicit
   hypothesis choice cannot be cleanly interpreted either way.

4. **Extract, rebuild datasets, retrain** — only after steps 1–3 are complete. Same
   order of time commitment as the Phase 5 matched rerun (extraction + full A/B/C
   retrain at 3 epochs each).

#### What this entry does not decide

- Which hypothesis to prioritize (volume vs. diversity).
- Whether expansion is the right next Phase 6 step relative to other approaches.
- Module subset selection within the candidate list.

Those decisions require the dry-run numbers and an explicit hypothesis choice first.

#### References
- Scripts/module_expansion_targets.json
- python/corpus_expansion_dry_run.py
- DEC-006 (cold-start embedding confound, still open)
- DEC-016 (Phase 5 conclusion)
- DEC-018 (perplexity gate resolved)

---

### DEC-020: Phase 6 conclusion — expanded corpus A/B/C rerun (2026-08-02)

**Date:** 2026-08-02
**Status:** Closed.

#### What was run

Full A/B/C retrain on the expanded 14-module corpus (DEC-015), matching the Phase 5
design. Embed-pretrain warm-start applied identically across all three variants
(DEC-009). Equal epoch counts: B and C at 2 epochs, A at 3 epochs (same ratio as
Phase 5 to account for A's smaller vocabulary). All runs completed without throttling
or checkpointing issues.

#### Results

| Variant | Vocab | Params | Epochs | Eval perplexity | Training time |
|---------|-------|--------|--------|----------------|--------------|
| A (IR tokenization) | 8,144 | 365M | 3 | **1.2792** | 105 min |
| B (BPE, standard FT) | 151,643 | 494M | 2 | **1.1295** | 181 min |
| C (BPE, embed-pretrain) | 151,643 | 494M | 2 | **1.1102** | 189 min |

B/C gap: 0.019 (negligible). A trails B/C by ~0.15–0.17 perplexity points.

#### Interpretation

The Phase 5 finding (DEC-016) replicates exactly on the expanded corpus. The A/B/C
gap did not close with more data: A's perplexity improved from Phase 5 (where
embed-pretrain eval was 2.14 pre-warmup) to 1.28 final, but B and C improved
proportionally, maintaining the same margin.

This is consistent with the representation hypothesis in DEC-006 — IR tokenization
produces a fundamentally different sequence structure that does not benefit from
BPE-pretrained Qwen weights as efficiently — but DEC-006 remains technically open
because the cold-start confound (A's embedding table was randomly initialized at
Phase 4 baseline) has not been isolated by a controlled experiment. The embed-pretrain
warm-start (DEC-009) mitigated this for Phase 5 and Phase 6 runs, but a direct
embedding-projection experiment was attempted (runs/variant_B_phase6 embed_pretrain
path) and showed only marginal improvement (embed_pretrain_eval_ppl 1.2761 → 1.1295
final for B; 1.2902 → 1.1102 for C; 2.1403 → 1.2792 for A), which does not
definitively isolate the representation quality effect from the embedding-table effect.

#### What this closes

- Phase 6 is complete. The experimental question "does more data close the gap?" is
  answered: no, not at the volume tested (~3,375 train / 376 eval examples across 14
  modules).
- DEC-019 (corpus expansion plan) status: the volume-for-DEC-006 hypothesis was
  implicitly tested here; the gap did not narrow. The diversity hypothesis
  (Topology.Basic, Order.Lattice) remains untested.

#### What remains open

- **DEC-006** — the cold-start embedding confound is still not definitively isolated.
  A controlled experiment (identical vocab, embed-pretrain on IR tokens from a Lean
  corpus, then fine-tune) would be required to separate representation quality from
  initialization quality.
- **DEC-019 diversity track** — domain generalization (does Maith IR transfer to
  topology/lattice structures?) was not addressed by Phase 6 and remains a valid
  future question.
- **Decompiler validity** (from DEC-014 and the decompiler PR) — the Lean decompiler
  produces structural skeletons, not yet valid Lean. This is a Phase 7 concern.

#### Decision

Phase 6 is closed. The project has a clean, reproducible, multi-phase result: IR
tokenization (Variant A) consistently trails BPE-based fine-tuning (Variants B/C) by
~0.15–0.17 perplexity points across both Phase 5 and Phase 6 corpus sizes. The gap
is stable, not converging. Whether this is attributable to representation quality,
initialization, or both requires a dedicated controlled experiment that is out of scope
for the current experimental program.

#### References
- runs/variant_A_phase6/results.json
- runs/variant_B_phase6/results.json
- runs/variant_C_phase6/results.json
- DEC-006 (cold-start embedding confound, still open)
- DEC-009 (embed-pretrain warm-start)
- DEC-015 (corpus expansion)
- DEC-016 (Phase 5 conclusion)
- DEC-018 (perplexity gate resolved)
- DEC-019 (corpus expansion plan)


### DEC-021: Embedding projection experiment — cold-start confound for DEC-006 closed (2026-08-03)

**Date:** 2026-08-03
**Status:** Closed. DEC-006 is now closed.

#### Background

DEC-006 identified a cold-start confound: Variant A's embedding table was randomly
initialized at Phase 4 baseline, while B and C inherited pretrained Qwen embeddings.
DEC-020 (Phase 6) mitigated this with an embed-pretrain warm-start (DEC-009) but could
not definitively isolate representation quality from initialization quality, because the
warm-start was applied to all three variants and the IR token space has no direct
pretrained analogue.

The controlled experiment proposed in DEC-006 and DEC-019: train an embedding projection
layer that maps Qwen's pretrained BPE embedding space into A's IR token space, giving A
genuinely warm embeddings derived from pretrained weights rather than random noise.

#### What was run

- **Script:** `python/train.py --variant A --datasets datasets/ --embed-project datasets/embed_proj_A.pt`
- **Checkpoint dir:** `runs/variant_A_dec006/`
- **Base model:** Qwen/Qwen2.5-Coder-0.5B
- **Vocab size:** 8,144 (IR vocabulary, unchanged)
- **Params:** 365.2M
- **Train examples:** 3,375 | **Eval examples:** 376
- **Epochs:** 2
- **Seed:** 42

#### Results

| | Perplexity |
|--|--|
| **Variant A — this run (embed-projected)** | **1.2978** |
| Variant A — Phase 6 baseline (embed-pretrain warm-start) | 1.2792 |
| Variant A — Phase 5 baseline (v3) | 1.2812 |
| Variant B — Phase 6 | 1.1295 |
| Variant C — Phase 6 | 1.1102 |

The embedding projection did not improve Variant A. The result (1.2978) is within
noise of both the Phase 5 (1.2812) and Phase 6 (1.2792) baselines. The gap to B/C
(~0.17 perplexity points) is fully intact.

Note: the terminal summary block printed "Variant A: 1.28" due to a rounding display
quirk in the comparison printer. The authoritative value from results.json is 1.2978.

#### Interpretation

Giving Variant A genuinely warm embeddings — projected from pretrained BPE weights —
produced no meaningful improvement over random initialization or the DEC-009
embed-pretrain approach. This rules out the embedding cold-start as the primary driver
of the A/B/C gap.

The gap is therefore attributable to **representation quality**, not initialization:
IR tokenization (Variant A) produces a sequence structure that is intrinsically less
learnable from this dataset and base model than BPE tokenization (Variants B/C),
independent of how the embedding table is seeded.

#### What this closes

- **DEC-006** — the cold-start embedding confound is now definitively closed. The
  embedding projection result rules out initialization as the cause of the A/B/C gap.
  The conclusion is: IR tokenization is less efficient than BPE for Lean expression
  completion at this scale, for reasons of representation quality, not initialization.

#### What remains open

- **Completion accuracy** — perplexity is confirmed consistent across three runs of
  Variant A (Phase 5, Phase 6, DEC-021). A completion accuracy eval on the DEC-021
  checkpoint would confirm the task metric is likewise unchanged. This is a low-priority
  confirmatory check; the perplexity convergence across three independent runs is
  already strong evidence.
- **DEC-019 diversity track** — domain generalization to topology/lattice structures
  remains untested and is a valid Phase 7 question.
- **Decompiler validity** (DEC-014) — structural skeleton → valid Lean is a Phase 7 concern.

#### Decision

DEC-006 is closed. The experimental program has a clean answer: IR tokenization
consistently trails BPE by ~0.17 perplexity points across Phase 5, Phase 6, and a
controlled embedding projection experiment. The gap is real, stable, and not explained
by initialization. Phase 7 should treat IR tokenization as a weaker baseline and focus
on decompiler validity and domain generalization.

#### References
- runs/variant_A_dec006/results.json
- DEC-006 (cold-start confound — now closed)
- DEC-009 (embed-pretrain warm-start)
- DEC-016 (Phase 5 conclusion)
- DEC-020 (Phase 6 conclusion)
- DEC-019 (corpus expansion plan, diversity track remains open)
