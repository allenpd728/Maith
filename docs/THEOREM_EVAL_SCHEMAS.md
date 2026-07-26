# Theorem Evaluation Artifact Schemas

Schema contract for Phase 6 theorem-proving evaluation artifacts.

## 1) `runs/theorem_eval_config.template.json` (or concrete config)

Produced by: `python/scaffold_theorem_eval.py`

Required top-level keys:
- `schema_version` (string, currently `theorem_eval_config.v1`)
- `description` (string)
- `dataset_manifest` (object)
- `benchmarks` (array)
- `variants` (array)
- `metrics` (array)
- `run_metadata` (object)

Required nested fields:
- `dataset_manifest.split` (string)
- `dataset_manifest.expected_examples` (int)
- `dataset_manifest.source` (string)
- each benchmark: `name` (string), `split` (string), `examples` (int)
- variants must include `A`, `B`, `C`, each with:
  - `name` (string)
  - `model_dir` (string)
  - `representation_id` (string)
- metrics must include:
  - `proof_completion_rate`
  - `atp_success_rate`
  - `proof_search_steps`
  - `proof_search_time_seconds`

## 2) `runs/theorem_eval_results.template.json` (or concrete results)

Produced by: `python/scaffold_theorem_eval.py` (template), later filled by evaluation harness.

Required top-level keys:
- `schema_version` (string, currently `theorem_eval_results.v1`)
- `status` (string)
- `notes` (string)
- `benchmark` (string)
- `dataset_manifest` (object)
- `run_metadata` (object)
- `results` (object with `A`, `B`, `C`)

Required nested fields:
- `dataset_manifest.split` (string)
- `dataset_manifest.example_count` (int)
- `dataset_manifest.manifest_path` (string)
- `run_metadata.seed` (int)
- `run_metadata.model_family` (string)
- `run_metadata.timestamp_utc` (string|null)
- for each variant (`A`, `B`, `C`):
  - `proof_completion_rate` (number|null)
  - `atp_success_rate` (number|null)
  - `proof_search_steps` (number|null)
  - `proof_search_time_seconds` (number|null)

## Validation command

```bash
python3 python/validate_theorem_eval_artifacts.py
```

Writes `runs/theorem_eval_schema_validation.json` and exits non-zero on schema mismatch.
