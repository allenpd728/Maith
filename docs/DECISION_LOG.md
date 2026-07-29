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

- References:
  - Matched-batch rerun results 2026-07-28: A=1.39, B=1.15, C=1.13
  - 3-epoch result 2026-07-28: A=1.2598
  - docs/PHASE_5_RESULTS.md
  - python/train.py (`--epochs` flag, `epochs_override` parameter)

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
