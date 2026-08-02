# Future Work (Phase 6+)

> Phase 5 is complete (DEC-018 resolved, 2026-08-02). This document contains scaffold designs
> for Phase 6 and beyond. Phase 6 design may now proceed.

This document consolidates scaffold designs for work that has not yet started. Nothing here
is implemented. These notes exist to preserve design thinking and schema contracts for when
Phase 6 begins.

---

## Phase 6: Theorem-proving evaluation

### Overview

Phase 5 measures next-token prediction perplexity. Phase 6 would measure downstream
theorem-proving performance: whether models trained on Maith IR are better at proof search,
proof completion, or ATP success than models trained on raw Lean source.

### Planned metrics

- Proof completion rate
- ATP success rate
- Proof search steps
- Proof search time (seconds)

### Artifact schemas (not yet produced)

#### `runs/theorem_eval_config.json`

Produced by: `python/scaffold_theorem_eval.py` (template only — not yet run)

Required top-level keys: `schema_version`, `description`, `dataset_manifest`, `benchmarks`,
`variants`, `metrics`, `run_metadata`.

Each variant (`A`, `B`, `C`) requires: `name`, `model_dir`, `representation_id`.

#### `runs/theorem_eval_results.json`

Produced by: evaluation harness (not yet implemented).

Required top-level keys: `schema_version`, `status`, `notes`, `benchmark`,
`dataset_manifest`, `run_metadata`, `results` (with `A`, `B`, `C`).

Each variant result requires: `proof_completion_rate`, `atp_success_rate`,
`proof_search_steps`, `proof_search_time_seconds`.

### Validation command (once artifacts exist)

```bash
python3 python/validate_theorem_eval_artifacts.py
```

---

## Representation evolution

Phase 5 results (A=1.489 vs B=1.180 vs C=1.207 at 512-token cap) show A trails B/C on both
perplexity and completion accuracy. DEC-006 (cold-start embedding confound) remains open — we
cannot yet separate representation quality from initialization advantage.

Alternate representations can be explored as Phase 6 work if the confound is resolved and the
gap persists. Design notes are in `docs/REPRESENTATION_EVOLUTION.md`.

---

## Proof-context sidecars

A scaffold exists for attaching cross-declaration context to training examples without
rewriting existing A/B/C datasets. The protocol keeps context as additive sidecars
so A/B/C comparability is preserved. Scripts: `python/scaffold_context_artifacts.py`,
`python/build_dependency_manifest.py`, `python/validate_context_artifacts.py`.

This is not implemented — the scaffold is a design template only.

---

## Experiment artifact schemas (Phase 5 monitoring)

The following JSON artifacts are produced by Phase 5 monitoring scripts under `runs/`:

| File | Produced by |
|------|-------------|
| `full_run_status.json` | `python/full_run_status_snapshot.py` |
| `full_results_gate.json` | `python/check_results_gate.py` |
| `publish_results_summary.json` | `python/publish_results_summary.py` |
| `publish_readiness.json` | `python/check_publish_readiness.py` |
| `artifact_schema_validation.json` | `python/validate_experiment_artifacts.py` |

Schema validation:

```bash
python3 python/validate_experiment_artifacts.py --runs-dir runs/
```
