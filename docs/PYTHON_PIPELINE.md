# Python Pipeline

The `python/` directory provides tooling to load, validate, and transform the JSONL corpus
produced by the Lean pipeline into training-ready datasets for the A/B/C experiment.

## Files

| File | Purpose |
|------|---------|
| `corpus_loader.py` | Schema validation, loading, vocab building, splitting, dataset class |
| `build_dataset.py` | A/B/C dataset builder — IR vocab + BPE variants, train/eval splits |
| `validate_roundtrip.py` | Decoder round-trip validator — confirms BVAR/TERM token stability |
| `tokenizer_study.py` | BPE fragmentation study — compares IR tokens vs Qwen2.5-Coder BPE |
| `train.py` | Fine-tuning script — runs one A/B/C variant, reports eval perplexity |
| `compare_results.py` | Reads all three `runs/variant_*/results.json` and prints comparison table |
| `run_full_experiment.py` | Sequential full/smoke A/B/C runner with shared log output |
| `check_results_gate.py` | Fails unless A/B/C outputs are full (non-smoke) and eval-aligned |
| `publish_results_summary.py` | Produces publish-ready summary JSON + markdown-style output block |
| `run_postrun_pipeline.py` | Runs strict compare → gate → publish → readiness → schema validation |
| `finalize_full_experiment.py` | Runs strict compare, gate, publish, and representation checks in one flow |
| `scaffold_theorem_eval.py` | Generates Phase 6 theorem-eval config/result templates |
| `validate_module_targets.py` | Preflight-check module importability for corpus expansion |
| `corpus_expansion_dry_run.py` | Runs module expansion to temp output and prints failure summary |
| `representation_audit.py` | Audits representation metadata consistency across datasets/runs |
| `representation_matrix_status.py` | Reports readiness/state across registered representation candidates |
| `register_representation.py` | Registers new representation candidates in the family registry |
| `backfill_representation_metadata.py` | Backfills missing representation IDs into existing artifacts |
| `watch_full_runs.py` | Monitors `runs/full_abc_runs.log` and reports current full-run progress |
| `estimate_full_run_eta.py` | Estimates completion ETA from full-run progress history |
| `run_status_dashboard.py` | Runs monitor/ETA/snapshot/report/history/docs checks in one command |
| `full_run_status_snapshot.py` | Exports run/gate/ETA snapshot to `runs/full_run_status.json` |
| `full_run_status_report.py` | Exports a markdown run-status brief to `runs/full_run_status.md` |
| `full_run_status_history.py` | Appends deduplicated run snapshots to `runs/full_run_status_history.jsonl` |
| `check_publish_readiness.py` | Verifies full-run artifacts and emits `runs/publish_readiness.json` |
| `validate_experiment_artifacts.py` | Validates schema of key run artifacts and writes `runs/artifact_schema_validation.json` |
| `check_docs_consistency.py` | Detects stale README/docs values and writes `runs/docs_consistency_report.json` |
| `corpus_report.py` | Node-type frequency report — merges top-N token counts into `Corpus/stats.json` |
| `spot_check.py` | Manual corpus spot-checking helper |

## Requirements

- Python 3.8+
- `transformers` library (for `build_dataset.py` variants B/C and `tokenizer_study.py`):
  `pip install transformers`

`corpus_loader.py` and `validate_roundtrip.py` use only the standard library.

## build_dataset.py

Builds three dataset variants for the controlled A/B/C experiment:

| Variant | Representation | Tokenizer |
|---------|---------------|-----------|
| A | Maith IR tokens (v1.2.0) | Custom vocab (`vocab_A.json`) |
| B | Raw `leanExpr` string | Qwen2.5-Coder BPE |
| C | AST-style split `leanExpr` | Qwen2.5-Coder BPE |

```bash
cd ~/Projects/Maith
python3 python/build_dataset.py [--corpus Corpus/corpus.jsonl] [--out datasets/] [--seed 42]
# Optional: tag the dataset with an explicit representation candidate ID
python3 python/build_dataset.py --representation-id semantic_graph_ir_v1_2_0
```

Outputs under `datasets/`:
- `vocab_A.json` — IR token → integer ID mapping (built from training split only)
- `train_A.jsonl`, `eval_A.jsonl` — IR token variant
- `train_B.jsonl`, `eval_B.jsonl` — Lean source BPE variant
- `train_C.jsonl`, `eval_C.jsonl` — AST-style BPE variant
- `train_manifest.json`, `eval_manifest.json` — ordered declaration IDs shared across A/B/C
- `representation_manifest.json` — representation metadata (`representation_id`, split sizes, seed)

Each row: `{ "source", "example_id", "name", "module", "input_ids", "labels", "seq_len" }`.
`labels` = `input_ids` shifted left by 1 (standard causal LM format, last position = −100).
Rows now also include `representation_id` so future representation variants can share the same
dataset/training tooling without ambiguity.

Split: 90% train / 10% eval, fixed seed for reproducibility across all three variants.

### IR vocab design

- Structural tokens (`GRAPH_BEGIN`, `E`, `A`, `R`, `O`, polarities, ops) get fixed low IDs.
- `BVAR_0`–`BVAR_31` + `BVAR_MANY` and `TERM_0`–`TERM_31` + `TERM_MANY` are always in vocab.
- `gen:*` tokens below frequency threshold (default 5) map to `GEN_UNK`.
- Vocab is built from the training split only — no test-set leakage.

## validate_roundtrip.py

Confirms that the Python-side decoder correctly round-trips every corpus token sequence.

```bash
python3 python/validate_roundtrip.py                   # sample 200
python3 python/validate_roundtrip.py --all             # full 2554
python3 python/validate_roundtrip.py --sample 500
```

Expected output on v1.0.0 corpus:
```
Results: 2554/2554 passed
  All examples round-trip cleanly ✓
  BVAR_* tokens: 33   TERM_* tokens: 33   Legacy b(): 0   Legacy t<n>: 0
```

## tokenizer_study.py

Measures BPE fragmentation of IR-specific tokens and sequence length inflation.

```bash
python3 python/tokenizer_study.py [--corpus Corpus/corpus.jsonl] [--sample 500]
```

Results on 2,554-example corpus (500-example sample):
- Lean source → BPE: **1.69x** median sequence length inflation vs IR tokens
- IR-as-text → BPE: **2.63x** inflation (BPE actively harms the structured IR format)
- Key fragmentation: `HMul.hMul` → 4 BPE tokens, `gen:OfNat.ofNat` → 6, `BVAR_0` → 4

## corpus_loader.py

Low-level schema validation and loading. Validates every line against the schema in
`CORPUS_SCHEMA.md`. Handles both v0.1.0 (`b(...)` / `t<n>`) and v1.0.0 (`BVAR_N` / `TERM_N`)
entity ID formats.

Key classes:
- `TrainingExample` — frozen dataclass: `name`, `module`, `lean_expr`, `graph`, `tokens`
- `CorpusDataset` — wraps examples + vocab dict; supports `len()` and index access
- `build_vocabulary` — builds `{token: int}` from corpus frequencies

## train.py

Fine-tunes the configured Qwen2.5-Coder base model on one variant and reports eval perplexity.

```bash
pip install torch transformers trl datasets

# Smoke test — 1 epoch, 50 examples
python3 python/train.py --variant A --smoke-test

# Full runs (~20-40 min each on M-series Mac)
python3 python/train.py --variant A --out runs/variant_A
python3 python/train.py --variant B --out runs/variant_B
python3 python/train.py --variant C --out runs/variant_C
```

Saves `runs/variant_*/results.json` with perplexity, training time, and config. After all three
runs, the script prints a comparison table automatically.
Also saves `runs/variant_*/loss_curve.json` (train/eval loss points from Trainer logs).

To monitor the long-running sequential full A/B/C job:

```bash
python3 python/run_status_dashboard.py
python3 python/watch_full_runs.py
python3 python/estimate_full_run_eta.py
python3 python/full_run_status_snapshot.py
python3 python/full_run_status_report.py
python3 python/full_run_status_history.py
```

To run full A/B/C sequentially via a tracked script (instead of ad-hoc shell):

```bash
python3 python/run_full_experiment.py
```

To enforce decision-grade result quality before interpreting outcomes:

```bash
python3 python/check_results_gate.py --runs-dir runs/
```

To build a publish-ready summary once full runs finish:

```bash
python3 python/publish_results_summary.py --runs-dir runs/
```

To run all finalization checks in one command:

```bash
python3 python/finalize_full_experiment.py --runs-dir runs/
```

To run publish/finalization steps as one post-run pipeline:

```bash
python3 python/run_postrun_pipeline.py --runs-dir runs/
```

To verify publishability in one readiness check:

```bash
python3 python/check_publish_readiness.py --runs-dir runs/
```

To validate key artifact schemas:

```bash
python3 python/validate_experiment_artifacts.py --runs-dir runs/
```

To catch stale experiment values in markdown docs:

```bash
python3 python/check_docs_consistency.py
```

To scaffold Phase 6 theorem-proving evaluation artifacts:

```bash
python3 python/scaffold_theorem_eval.py
```

To audit representation metadata consistency:

```bash
python3 python/representation_audit.py
```

For stricter enforcement (fail if IDs are missing):

```bash
python3 python/representation_audit.py --strict-nonempty
```

To summarize status across all registered representation candidates:

```bash
python3 python/representation_matrix_status.py
```

To backfill representation IDs on older artifacts (dry-run by default):

```bash
python3 python/backfill_representation_metadata.py
python3 python/backfill_representation_metadata.py --apply
```

To register a new representation candidate:

```bash
python3 python/register_representation.py \
  --family-id semantic_graph_ir \
  --representation-id semantic_graph_ir_v2_candidate_1 \
  --status planned \
  --notes "First v2 candidate"
```

## Coverage expansion protocol (low-risk scaffold)

Target modules are tracked in:

```text
Scripts/module_expansion_targets.json
```

Recommended flow:

1. Validate that target modules import in the current Lean/Mathlib environment:

```bash
python3 python/validate_module_targets.py --set candidates
```

2. Run a dry-run extraction to temp output (no tracked corpus files touched):

```bash
python3 python/corpus_expansion_dry_run.py --set candidates --output-dir /tmp/maith-corpus-expansion-dryrun
```

3. Inspect dry-run summary (`totalDeclarations`, `successfulExamples`, failure buckets, per-module stats)
   and only then promote selected modules into the production corpus build path.

Key design choices (from `docs/EXPERIMENT_DESIGN.md`):
- Variant A: embedding table resized to `vocab_A.json` size (currently 4,495 IR tokens)
- Variants B/C: native Qwen2.5-Coder BPE, no resizing
- Fixed seed 42, cosine LR schedule, 3 epochs, batch size 8 (effective)
- Eval perplexity is computed at a fixed 512-token cap for all A/B/C runs

## Known gaps

- No PyTorch `DataLoader` collate function in `corpus_loader.py` — `train.py` uses its own collate.
- Current `runs/variant_*/results.json` are smoke-only (`smoke_test=true`), so they are not decision-grade.
- Full 3-epoch A/B/C runs on the complete train/eval splits are still pending.

## Next steps

1. Run all three full variants (A/B/C) with identical settings except representation.
2. Record full-run perplexity results in `docs/EXPERIMENT_DESIGN.md`.
3. Expand corpus to more Mathlib modules once baseline results are in.
