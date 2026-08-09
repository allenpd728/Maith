# Representation Evolution Strategy

Maith's current semantic graph IR (`semantic_graph_ir_v1_2_0`) is a **candidate representation**, not a guaranteed winner.
This document defines how we evolve representations if results do not support the current hypothesis.

## Goals

1. Preserve the ability to run clean A/B tests between representation variants.
2. Avoid breaking historical datasets and training artifacts.
3. Support iterative design (`IR-v1`, `IR-v2`, ...) with explicit versioning and contracts.

## Representation family model

- **Family**: high-level representation lineage (e.g., `semantic_graph_ir`, `typed_term_ir`, `hybrid_graph_term`).
- **Representation ID**: concrete versioned candidate (e.g., `semantic_graph_ir_v1_2_0`).
- **Core vocab contract**: stable token subset retained across family evolution.
- **Extension vocab**: additive tokens specific to a new candidate.

## Current baseline

- Family: `semantic_graph_ir`
- Representation ID: `semantic_graph_ir_v1_2_0`
- Encoding: positional `FVAR_N` / `BVAR_N` / `TERM_N` (cap 63)

## Evolution workflow

1. Register a new candidate in `Scripts/representation_families.json`.
2. Build dataset with explicit `--representation-id`.
3. Train and evaluate with results tagged by representation ID.
4. Run full A/B/C plus intra-family comparisons (v1 vs v2).
5. Gate with decision criteria before promoting the new candidate.

## Decision criteria (go/no-go)

A new representation candidate is promoted only if:

1. It passes extraction/round-trip validation at corpus scale.
2. It is not worse than baseline on core quality gates (full-run perplexity + completion metrics).
3. It improves at least one targeted weakness (e.g., cross-graph identity sensitivity) with acceptable tradeoffs.

If criteria fail, keep baseline and iterate next candidate.

## Experiment matrix extension

Current matrix:
- A: semantic graph IR
- B: Lean source BPE
- C: AST-style BPE

Evolution-ready matrix adds:
- A1: `semantic_graph_ir_v1_2_0`
- A2: `semantic_graph_ir_v2_x`
- A3: `typed_term_ir_v1_x` (future)

This allows separating:
- "IR vs non-IR" hypothesis
- "which IR design" hypothesis

## Tooling hooks in repo

- Dataset rows include `representation_id`.
- Training results include `representation_id`.
- Comparison scripts print `representation_id`.

These hooks let us compare multiple IR candidates without changing existing run mechanics.

## Operational commands

Audit current metadata consistency:

```bash
python3 python/representation_audit.py
```

Enforce non-empty metadata:

```bash
python3 python/representation_audit.py --strict-nonempty
```

See readiness across registered candidates:

```bash
python3 python/representation_matrix_status.py
python3 python/check_representation_matrix_gate.py --allow-incomplete
```

Backfill older artifacts that predate `representation_id` metadata (dry-run first):

```bash
python3 python/backfill_representation_metadata.py
python3 python/backfill_representation_metadata.py --apply
```

Register a new candidate in the family registry:

```bash
python3 python/register_representation.py \
  --family-id semantic_graph_ir \
  --representation-id semantic_graph_ir_v2_candidate_1 \
  --status planned \
  --notes "First v2 candidate"
```
