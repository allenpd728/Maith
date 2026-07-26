# Experiment Artifact Schemas

This document defines the expected JSON structure for key experiment artifacts under `runs/`.

## 1) `runs/full_run_status.json`

Produced by: `python/full_run_status_snapshot.py`

Required top-level keys:
- `generated_at_utc` (string)
- `log_path` (string)
- `status` (object)
- `eta` (object)
- `gate` (object)
- `publish_summary` (object)

Important nested keys:
- `status.run_state` (string)
- `status.done_variants` (array)
- `status.pending_variants` (array)
- `status.progress` (object|null; if object: `percent`, `step`, `total_steps` ints)
- `eta.available` (bool)
- `gate.present` (bool), `gate.decision_grade` (bool|null)
- `publish_summary.present` (bool), `publish_summary.decision_grade` (bool|null), `publish_summary.winner_by_perplexity` (string|null)

## 2) `runs/full_results_gate.json`

Produced by: `python/check_results_gate.py`

Required top-level keys:
- `decision_grade` (bool)
- `checks` (array of check objects)

Each check object:
- `name` (string)
- `pass` (bool)
- `details` (object)

## 3) `runs/publish_results_summary.json`

Produced by: `python/publish_results_summary.py`

Required top-level keys:
- `decision_grade` (bool)
- `missing_results` (array)
- `smoke_variants` (array)
- `winner_by_perplexity` (string|null)
- `variants` (object with `A`, `B`, `C`)

Each `variants.<V>` object includes:
- `eval_perplexity` (number|null)
- `training_minutes` (number|null)
- `quality` (string)
- `tokenizer_mode` (string|null)
- `train_seq_len_cap` (int|null)
- `eval_seq_len_cap` (int|null)
- `loss_curve` (object)

`loss_curve` keys:
- `train_points` (int)
- `eval_points` (int)
- `train_first` (number|null)
- `train_last` (number|null)
- `eval_last` (number|null)

## 4) `runs/publish_readiness.json`

Produced by: `python/check_publish_readiness.py`

Required top-level keys:
- `ready_to_publish` (bool)
- `checks` (array of check objects)

Each check object:
- `name` (string)
- `pass` (bool)
- `details` (object)

## Schema validation command

Run:

```bash
python3 python/validate_experiment_artifacts.py --runs-dir runs/
```

This writes `runs/artifact_schema_validation.json` and exits non-zero on schema mismatch.
