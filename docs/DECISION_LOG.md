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
