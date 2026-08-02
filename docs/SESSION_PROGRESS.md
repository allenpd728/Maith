# Maith IR Pipeline — Session Progress

## 2026-07-28 (session 7: regression tests, pre-flight check, matched-batch rerun)

### What was done

**Regression tests** — `python/test_train_regression.py` written: 6 tests covering the label-shift edge case (seq_len=2 that produces all-ignored labels after the internal causal-LM shift — the exact case that caused the original bug), NaN loss detection, and `VARIANT_BATCH_CONFIG` effective batch size matching across A/B/C. All 6 pass, CPU-only, no GPU needed.

**Pre-flight check** — `python/preflight_check.py` written: 7 checks — effective batch size alignment across all variants, vocab size consistency (tokenizer vs model config vs embedding rows) for each variant, and finite loss verification on a small real forward pass. Passed 7/7 on the current checkout before the matched-batch rerun started.

**Matched-batch rerun started** — all variants now `grad_accum=8` (effective batch=8). Currently in progress.

**PHASE_5_RESULTS.md created** — shell document with `[PENDING]` placeholders for matched-batch results, previous run numbers with confound flags, known confounds section (DEC-006, DEC-007), and interpretation framework.

**Documentation updates** — `PHASE_5_COMPLETION_CHECKLIST.md` updated to reflect rerun status and post-rerun steps; `LATEST_FIXES.md` updated with full 2026-07-28 entry; `PYTHON_PIPELINE.md` updated with `preflight_check.py`, `test_train_regression.py`, and `eval_completion.py` added to files table; `Modelfile` added to `.gitignore`.

---

## 2026-07-27 (session 6: DEC-007 fix, --trace flag, documentation)

### What was done

**DEC-007 identified and fixed** — B and C had `grad_accum=4` (effective batch 4) vs A at effective batch 8. An uncontrolled variable across A/B/C. Fixed `VARIANT_BATCH_CONFIG` in `python/train.py`: B and C `grad_accum` bumped from 4 to 8. Decision: bumped B/C rather than dropping A's `batch_size` — A was already stable, no reason to retest A's memory profile. Removed misleading `(all variants matched)` from the batch-size print. Documented in `DECISION_LOG.md` as DEC-007.

**Full matched-batch run** — executed A/B/C with effective batch=8 across all variants (`smoke_test: false`). Results: A=1.3922 perplexity, 22.6 min, 277 steps; B=1.1420, 41.3 min, 554 steps; C=1.1298, 44.3 min, 554 steps. These carry DEC-006 (embedding cold-start) and DEC-007 confounds and are not final.

**`--trace` flag** — added `--trace <declName>` to `Scripts/BuildCorpus.lean`, `Maith/MathlibCorpusBuilder.lean`, and `Maith/ProcessingPipeline.lean`. When passed, prints three representations of the named declaration's elaborated `Expr` before pipeline runs: `[TRACE:leanExpr]`, `[TRACE:dbgToString]`, `[TRACE:reprStr]` (full constructor tree). Used to capture real live `--trace neg_neg` output for Stage 2 of `EXAMPLE_ROUNDTRIP.md`.

**EXAMPLE_ROUNDTRIP.md** — created: full pipeline walkthrough for `neg_neg` with explicit provenance labels on every stage (direct corpus read / live Lean run / illustrative). Stage 2 populated with real elaborated `Expr` data from `--trace` output.

**Round-trip completeness** — additive edits to `EXAMPLE_ROUNDTRIP.md`, `Design.md`, and `README.md`: token↔graph losslessness is verified (2,554/2,554); graph→Lean-syntax reconstruction is now implemented via `Transpiler.lean`'s `Decompile.decompileGraph` function.

**Results tables updated** — `README.md` and `docs/EXPERIMENT_DESIGN.md` updated with real 2026-07-27 full-run numbers and confound flags.

---

## 2026-07-25 (session 4: 100% corpus coverage + pipeline hardening)

### What was done

**`EntityId.bound` round-trip fix** — `Decoder.lean` was silently falling through
`b(<scope>)` tokens to `.var`, corrupting decoded graphs. Fixed `parseEntityIdToken`
to strip the `b(...)` wrapper and reconstruct `.bound`. Python `corpus_loader.py`
`_validate_entity_id` also rejected `bound` as an unknown kind — fixed to accept
and validate the `declName/depth/binderName` scope format. 776 of 792 corpus entries
contain bound IDs; all now pass the validator and token round-trip.

**HOF application extraction** — `extractExprEntityId` was calling
`failUnsupported "HOF application"` for any non-constant function head (`bvar`,
`fvar` applied as a function). Fixed: variable-headed applications now emit an
`Operation` with `op = .generic "hof"`, head entity as first input, arg entities
following. Coverage jump: **70% → 93%** (792 → 1055 declarations).

**Projection extraction** — `.proj typeName idx struct` now emits
`Operation (.generic "proj:TypeName/idx")` applied to the struct entity.

**Let-expression extraction** — `.letE name type value body` now pushes a scoped
`bound` entity for the binding, emits a `let-binding` attribute and equality
relation to the value entity, then recurses into the body.

**HEq arity fallback** — `HEq` has 4 args (`HEq α a β b`), not 3 like `Eq`.
Added explicit branch; any other unexpected arity falls back to a generic operation
rather than `failUnsupported`. Coverage: **99.6% → 100%** (1125 → 1129).

**Stats aggregation fix** — `processBatch` was leaving `tokenDistribution` and
`graphStats` at zero defaults. Fixed: both are computed from the collected examples
at the end of the loop using `Float.ofNat` for explicit Nat→Float coercions.

**`buildCorpus` executable** — added `Scripts/BuildCorpus.lean` and a `lean_exe`
entry to `lakefile.lean` so the corpus can be re-generated with:
```
lake build buildCorpus
lake env ./.lake/build/bin/buildCorpus
```

**`decodeGraph` panics removed** — both `panic!` calls replaced with graceful
fallbacks: missing `GRAPH_BEGIN` returns an empty graph; unknown tokens are skipped.
`decodeGraph` is now total.

**Stale comments updated** — `Encoder.lean`, `Decoder.lean`, and `Transpiler.lean`
all carried "minimal scaffold / placeholder" doc comments that no longer reflected
the actual implementations. Updated to document what the code actually does, and
added an explicit warning that `Transpiler` is debug-only with a different ID format
than `Encoder`.

**Deprecated API fixes** — `String.dropRight` → `dropEnd().toString`,
`String.trim` → `trimAscii().toString`.

**`parseRelationOpToken` fallback fixed** — previously defaulted to `.gt` for
unknown tokens, making `.gt` unreachable. Now all nine `RelationOp` values are
explicit branches; fallback is `.eq`.

**Python spot-check script** — `python/spot_check.py` validates the full corpus
through `corpus_loader.py` and verifies token round-trips for all bound entries.

### Corpus results — `Mathlib.Algebra.Group.Defs` (1,129 declarations)

| | Count | % |
|---|---|---|
| **Success** | **1129** | **100%** |
| Failure | 0 | 0% |

Token distribution: min 13, max 7509, avg 318 tokens/graph, total 359,310 tokens.
Graph structure: avg 35.8 entities, 6.8 attributes, 13.3 relations, 21.7 operations; max graph size 1,859 nodes.

### Tests

- `lake build tests`: all jobs, 0 failures
- All tests pass (including new: `testProjectionExtracts`, `testLetExpressionExtracts`,
  `testHOFApplicationExtracts`, `Decoder round-trips EntityId.bound`,
  `Decoder round-trips Graph with bound entity`)

### Commits on `kit/dev` this session

| Hash | Description |
|---|---|
| `fb44d6d` | fix: replace decodeGraph panics with graceful fallbacks; update stale placeholder comments |
| `b6ec4f8` | fix: use Float.ofNat for explicit Nat→Float coercions in processBatch |
| `43a3730` | fix: correct Nat/Float casts and zipWith usage in processBatch stats |
| `6a31c5e` | fix: populate tokenDistribution and graphStats in processBatch |
| `0e96f5e` | fix: handle HEq arity and unexpected relation arity as generic operations |
| `19ead0a` | fix: match letE 5th Bool argument in MetaExtractor |
| `0fbb7e9` | fix: implement projection and let-expression extraction in MetaExtractor |
| `3505874` | fix: open Lean.DSL namespace in BuildCorpus script |
| `7e88935` | feat: add buildCorpus executable for re-running corpus generation |
| `e633b86` | fix: add .toString to String.Slice results from dropEnd and trimAscii |
| `0e94b83` | fix: replace deprecated String.dropRight/trim with dropEnd/trimAscii in Decoder |
| `f515285` | fix: HOF application extraction in MetaExtractor; add tests and corpus spot-check script |
| `b1e4404` | fix: bound EntityId round-trip in Decoder, Python validator, and schema docs |
| `55ff7c0` | docs: update pipeline status, design notes, fix typo, add Python pipeline doc |

### Next milestones (completed in session 5)

1. ✅ **Expand corpus** — 4 modules, 2,554/2,554 (100%)
2. ✅ **Python training pipeline** — `train.py`, `compare_results.py`, A/B/C splits built
3. ✅ **`mathlibCommitHash`** — resolved via `jq` from `lake-manifest.json` (`fabf563a`)
4. ✅ **Encoder v1.2.0** — FVAR_N/BVAR_N split, 7,867 unique tokens, 2,554/2,554 round-trip

---

## 2026-07-06 (session 3: metaprogramming extraction + corpus run)

**Architecture pivot** — replaced `Transpiler.fromLean*` string parsers with
`Maith/MetaExtractor.lean`, a real Lean 4 metaprogram that walks
`ConstantInfo`/`Expr` trees from the live `Environment`.

**Scoped entity IDs** — added `EntityId.bound : String → EntityId` using the
format `"declName/depth/binderName"`.

**`forallE`/`lam`/`bvar` handling** — binder forms push a scoped entity onto
`binderCtx`, recurse into the body, then pop. `bvar n` resolves via list indexing.

**`OperationOp.generic` fallback** — any constant-headed application not in the
semantic core emits `Operation (.generic "FullName")`.

**`.thmInfo` skip** — proof terms skipped; only the statement type extracted for
theorems.

**Corpus results at session end**: 792/1129 (70%). Failure taxonomy documented above.

---

## 2026-07-05 (session 1: baseline fixes)

- Fixed Lean 3-style syntax in `Examples.lean`, `Curriculum.lean`
- Implemented `Transpiler` round-trip
- Implemented `Decoder` token round-trip
- Made `CorpusSerializer` write real files to disk
- Fixed `ProblemGenerator` to reject `a = 0`
- Build: 66 jobs, 0 failures; 54/54 tests passing
