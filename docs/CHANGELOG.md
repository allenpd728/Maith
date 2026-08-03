# Changelog

Historical record of fixes and changes. For current experiment decisions, see `docs/DECISION_LOG.md`.

---

## 2026-07-26 — Experiment operations + gating scaffolds

### Run observability and dashboarding

- Added `python/run_status_dashboard.py` to run monitor/ETA/snapshot/report/history/doc checks in one command.
- Added `python/full_run_status_snapshot.py` + `python/full_run_status_report.py` + `python/full_run_status_history.py`
  for machine-readable, markdown, and time-series run tracking.

### Publishability and schema gating

- Added `python/check_publish_readiness.py` to compute `runs/publish_readiness.json`.
- Added `python/validate_experiment_artifacts.py` to validate key run artifact schemas.
- Added `python/run_postrun_pipeline.py` wrapper for strict compare → gate → publish → readiness → schema checks.

### Decision and representation scaffolds

- Added canonical `docs/DECISION_LOG.md` plus `python/check_decision_log.py` coverage check.
- Added `python/check_representation_matrix_gate.py` and integrated it into post-run/finalization wrappers.
- Added theorem-eval schema contract/validation scaffolds.

---

## 2026-07-25 (session 2) — Encoder v1.2.0, FVAR/BVAR split, training pipeline

### Encoder v1.2.0 — forall/lambda binder distinction

`MetaExtractor.lean` now tags bound variable scope strings with a binder-kind prefix:
- Forall binders (`∀x, ...`) → scope `"∀:declName/depth/x"` → encodes as `FVAR_0`, `FVAR_1`, ...
- Lambda binders (`fun x => ...`) → scope `"λ:declName/depth/x"` → encodes as `BVAR_0`, `BVAR_1`, ...

Each counter resets to 0 per graph. Both capped at 63; overflow → `FVAR_MANY`/`BVAR_MANY`.
Total unique vocab: 7,867 tokens (up from 7,265). Round-trip 2,554/2,554 verified.

### Training pipeline complete

- `python/train.py` — fine-tune the configured Qwen2.5-Coder base model on one A/B/C variant, reports eval perplexity
- `python/compare_results.py` — reads all three `runs/variant_*/results.json`, prints comparison table
- `runs/` added to `.gitignore`
- `docs/EXPERIMENT_DESIGN.md` — results table stub ready to fill in after training

### Other fixes

- `mathlibCommitHash` now captured from `lake-manifest.json` via `jq` — `fabf563a` (Mathlib v4.31.0)
- `String.trim` → `trimAscii` deprecation fixed in `MathlibCorpusBuilder.lean`

---

## 2026-07-25 — Encoder v1.0.0, multi-module expansion, Python tooling

### Encoder format stabilization (v1.0.0)

Replaced unstable scoped entity IDs with positional tokens:

- `EntityId.bound "declName/0/x"` → `BVAR_0`, `BVAR_1`, ..., `BVAR_31`, `BVAR_MANY`
- `EntityId.term 3` → `TERM_3`, ..., `TERM_31`, `TERM_MANY`
- `EntityId.var "HMul.hMul"` → unchanged (semantically stable constants)

Result: vocabulary collapsed from **63,747 → 5,577 unique tokens** (−91%).
Format spec: `docs/ENCODER_FORMAT.md`.

### Decoder backward compatibility

`Decoder.lean` now handles both formats (v0.1.0 legacy `b(<scope>)` and v1.0.0 `BVAR_N`).
Fixed `TERM_MANY`/`BVAR_MANY` passthrough bug. Full 2,554/2,554 round-trip verified.

### Multi-module corpus expansion

`Scripts/BuildCorpus.lean` expanded to 4 modules. Coverage: **2,554/2,554 (100%)**.

### Python tooling

Added: `python/validate_roundtrip.py`, `python/tokenizer_study.py`, `python/build_dataset.py` (rewritten).

---

## 2026-07-06 — IR extraction: 0% → 100% coverage

### Architecture pivot

Replaced string-based `Transpiler.fromLean*` functions entirely with `MetaExtractor.lean`,
which walks `ConstantInfo`/`Expr` trees from the live Lean `Environment`.

### Coverage journey

| Fix | Coverage |
|-----|----------|
| Baseline (MetaExtractor) | 70% (792/1,129) |
| HOF application (`gen:hof`) | 93% (1,055) |
| Projection (`gen:proj:TypeName/idx`) | ~99% |
| `letE` support (5-arg match) | ~99.6% |
| `HEq` arity fallback | **100% (1,129/1,129)** |

### Key fixes

- HOF application, projection, `letE`, `HEq` arity — all resolved
- Stats aggregation fixed with `Float.ofNat`
- `buildCorpus` executable added
- `decodeGraph` panics replaced with graceful fallbacks
- `EntityId.bound` Python validation fixed
