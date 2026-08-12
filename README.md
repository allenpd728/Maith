# Maith: Semantic IR for Lean Mathematics

> **v2 era (2026-08-09).** Current IR: `semantic_graph_ir_v2_0_0` (C1 polarity removal,
> C2 typeclass enrichment, C4 GEN module bucketing; vocab 601). The v2 control grid is
> complete: Variant A v2 = perplexity **1.2361** / top-1 **90.0%**; B-small (size-matched
> BPE control, DEC-027) = **1.1294** / **90.5%** — essentially tied with A at matched
> params. The IR encodes real semantics (DEC-025: 62pp probe gap) but doesn't improve
> prediction over BPE at toy scale (358M, 3.5K examples). Prediction metrics are
> structurally biased against the IR's canonicalization goal (H11 ◐ — partly fundamental,
> partly contingent on IR design quality) — the primary evaluation should be semantic-task
> metrics (retrieval, ATP), which are untested.
>
> Key docs: [`HYPOTHESIS_GRID`](docs/experiments/HYPOTHESIS_GRID.md) (sub-claim status) ·
> [`V2_COMPARISON_MATRIX`](docs/experiments/V2_COMPARISON_MATRIX.md) (control grid) ·
> [`EXPERIMENT_DESIGN`](docs/experiments/EXPERIMENT_DESIGN.md) (evaluation framework) ·
> [`PRIOR_ART`](docs/reference/PRIOR_ART.md) (related work) ·
> [`LITERATURE_REVIEW_2026_08`](docs/experiments/LITERATURE_REVIEW_2026_08.md) (field check of open sub-claims) ·
> [`DECISION_LOG`](docs/decisions/LOG.md) (DEC-026/027).

Maith is a Lean 4 project for extracting a canonical semantic representation of formal mathematics from elaborated Lean terms (`Expr`), then serializing that representation into token sequences for downstream language-model training.

## 1) Research Question

Can language models become better theorem provers if trained on a representation of mathematics that exposes semantic structure rather than source syntax?

## 2) Hypothesis

Our central hypothesis is that language models trained on a canonical semantic representation of formal mathematics — rather than raw proof assistant source code — will learn more useful mathematical representations, resulting in improved theorem prediction, proof search, and proof completion.

Maith is a **candidate representation** for testing that hypothesis; effectiveness is not assumed and remains to be demonstrated empirically.

## 3) Why This Matters

Recent progress in automated theorem proving with language models has focused heavily on model scale and data quantity. We hypothesize that representation choice is also a major bottleneck: if training data exposes semantic structure more directly, models may need less capacity to learn equivalent mathematical patterns. Maith exists to make that hypothesis testable in a Lean/Mathlib setting.

## 4) Approach

Maith does **not** parse Lean source strings into IR. Instead, it runs inside Lean and extracts IR from the elaborated environment (`Environment`, `ConstantInfo`, `Expr`), where implicit arguments, notation expansion, and typing information are already resolved.

High-level flow:

- Extract declaration semantics from elaborated `Expr` trees (`MetaExtractor.lean`)
- Build IR Graph (`Entity`, `Attribute`, `Relation`, `Operation`)
- Canonicalize graph ordering (`Normalizer.lean`)
- Encode canonical graph to tokens (`Encoder.lean`) — v2.0.0, 601-token vocabulary
- Serialize examples to JSONL (`CorpusSerializer.lean`)
- Consume JSONL in `python/` for vocab/tokenizer, splits, and dataset objects
- Decode tokens back to graph (`Decoder.lean`)
- Decompile graph to Lean syntax (`Transpiler.lean` — `Decompile.decompileGraph`)

The full pipeline is implemented and validated end-to-end: 2,554/2,554 declarations round-trip cleanly through encode → decode → decompile (see `python/validate_roundtrip.py` and `docs/reference/DECOMPILER_HANDOVER.md`), confirming token/graph losslessness and Lean syntax reconstruction. The current v2 corpus covers 14 Mathlib modules (4,029 extracted examples; 3,491 train / 388 eval after filtering).

## 5) Why Not Train Directly on Lean Source?

This section is motivation, not a claim of proven superiority.

We hypothesize that source-token training may be less sample-efficient because:

- Lean notation can hide semantic equivalences that elaborate to similar terms.
- Elaboration inserts implicit arguments that are absent in surface syntax.
- Macros and coercions can map many textual forms to related elaborated structures.
- Multiple syntactic encodings of the same idea may collapse into a smaller set of semantic patterns after elaboration.

We expect a canonical semantic representation to be easier for sequence models to learn from than raw source in at least some theorem-prediction/proof-search settings. **This is not yet supported by the experimental record:** the v2 control grid (DEC-026/027) finds the IR encodes semantics (DEC-025) but does not outperform a size-matched BPE baseline on next-token prediction or completion accuracy. See [`docs/reference/PRIOR_ART.md`](docs/reference/PRIOR_ART.md) for the related work that motivates this hypothesis and the closest positive-result counterpart (IRCoder).

## 6) Architecture

```mermaid
flowchart LR
    A[Lean source / Mathlib modules] --> B[Lean elaborator]
    B --> C[Expr + Environment]
    C --> D[MetaExtractor.lean]
    D --> E[IR Graph]
    E --> F[Normalizer.lean]
    F --> G[Canonical graph]
    G --> H[Encoder.lean v1.2.0]
    H --> I[Token sequence — v1-style strings]
    I --> J[CorpusSerializer.lean]
    J --> K[Corpus/corpus.jsonl — v1 tokens, NOT committed]
    K --> L[python/build_dataset.py]
    L --> L1[v2 re-encode: C1/C2/C4 + integer mapping via vocab_A.json]
    L1 --> M[datasets/train_*.jsonl + eval_*.jsonl — integer input_ids]
    M --> N[python/train.py variants A/B/C]
    N --> O[runs/variant_*/results.json + loss_curve.json]
```

> **Where v2 encoding and integer mapping actually happen (diagram correction).**
> The Lean-side `Encoder.lean` emits **v1-style string tokens** (`neut`, `gen:<FullName>`)
> into `corpus.jsonl`. The **v2 token stream** (C1 polarity removal, C2 typeclass short
> names, C4 GEN bucketing) and the **token→integer mapping** are both produced in
> `python/build_dataset.py`'s `encode_ir` + `build_ir_vocab` at dataset-build time — not
> in Lean, and not in `train.py`. So `corpus.jsonl` holds v1-style strings; the v2
> integers exist only in `datasets/train_*.jsonl` (`input_ids`/`labels`). See
> [`CORPUS_SCHEMA.md`](docs/reference/CORPUS_SCHEMA.md) and
> [`build_dataset.py`](python/build_dataset.py) `encode_ir`/`bucket_from_module`.
> `Corpus/corpus.jsonl` itself is gitignored (not committed); `Corpus/corpus_manifest.json`
> records its hash and regeneration command.

Concrete pipeline in this repo:

`Lean environment -> MetaExtractor.lean -> IR Graph -> Normalizer.lean -> canonical graph -> Encoder.lean (v1 strings) -> corpus.jsonl -> build_dataset.py (v2 encode + int map) -> datasets/train_*.jsonl (int ids) -> train.py`

For a fuller, stage-by-stage architecture and artifact map, see:
[`docs/reference/Design.md#detailed-pipeline-diagram`](docs/reference/Design.md#detailed-pipeline-diagram).

The full pipeline is implemented and validated. See section 7 for corpus results and section 8 for known limitations.

## 7) Current Status / Results

### Build and tests

- `lake build tests` passes (0 failures).
- All tests pass (**66+ unit tests + corpus-pipeline/serializer integration checks**).

**Python test suite** — all passing, 0 failures:

| File | Tests | Coverage |
|------|-------|----------|
| `python/test_train_regression.py` | 6 | `collate_fn`, `evaluate_perplexity`, `BATCH_SIZE`/`GRAD_ACCUM` constants |
| `python/test_build_dataset.py` | 14 | Dataset construction, tokenization, splits |
| `python/test_variant_config.py` | 6 | Variant A/B/C configuration validation |
| `python/test_pipeline_integration.py` | 1 | End-to-end pipeline smoke test |

Run with:
```bash
python3 python/test_train_regression.py
python3 python/test_build_dataset.py
python3 python/test_variant_config.py
python3 python/test_pipeline_integration.py
```

### While full A/B/C runs are active

Use these read-only monitoring helpers (safe during active training):

```bash
python3 python/run_status_dashboard.py
python3 python/watch_full_runs.py
python3 python/estimate_full_run_eta.py
python3 python/full_run_status_snapshot.py
python3 python/full_run_status_report.py
python3 python/full_run_status_history.py
```

To guard against stale experiment values in docs while results evolve:

```bash
python3 python/check_docs_consistency.py
```

After runs complete, check publish readiness in one command:

```bash
python3 python/check_publish_readiness.py --runs-dir runs/
```

Validate key artifact schemas:

```bash
python3 python/validate_experiment_artifacts.py --runs-dir runs/
```

Run the full post-run publication pipeline:

```bash
python3 python/run_postrun_pipeline.py --runs-dir runs/
```

Phase 5/6 artifact field contracts and the Phase 7 IR-candidate roadmap are documented in [`docs/history/PHASE_7_ROADMAP.md`](docs/history/PHASE_7_ROADMAP.md). Related work is surveyed in [`docs/reference/PRIOR_ART.md`](docs/reference/PRIOR_ART.md).

Canonical experiment assumptions/interpretation decisions are tracked in
[`docs/decisions/LOG.md`](docs/decisions/LOG.md).

### Corpus extraction result

The v2 corpus spans 14 Mathlib modules across algebra, order, and topology
(`Mathlib.Algebra.*`, `Mathlib.Order.*`, `Mathlib.Topology.Basic`). Total declarations:
**4,029** extracted; after filtering, **3,491 train / 388 eval** examples.

The original 4-module extraction (2,554 declarations, 100% coverage) is preserved below
for reference:

| Module | Declarations | Success |
|---|---|---|
| Algebra.Group.Defs | 1,129 | 1,129 (100%) |
| Algebra.Group.Basic | 548 | 548 (100%) |
| Algebra.Ring.Defs | 446 | 446 (100%) |
| Order.Basic | 431 | 431 (100%) |
| **Total** | **2,554** | **2,554 (100%)** |

All previously identified failure categories have been resolved — zero failures across all modules.

Token distribution (v2.0.0 encoder, 601-token vocab): see `Corpus/stats.json` for current
counts. The v1.2.0 distribution (7,867 raw tokens / 4,495 after filtering) is superseded.

Non-trivial extracted examples:

| Declaration | Entities | Relations | Operations |
|---|---|---|---|
| `mul_assoc` | 11 | 5 | 5 |
| `mul_comm` | 8 | 4 | 3 |
| `mul_one` | 11 | 3 | 6 |
| `DivisionMonoid.mk` | 34 | 14 | 21 |

Evidence artifact committed intentionally: [`Corpus/corpus.jsonl`](Corpus/corpus.jsonl) (current 7.2MB extraction output for the module above).

### Binder encoding

Lean uses De Bruijn indices for bound variables. Maith uses positional tokens assigned in
first-appearance order within each graph:

- Forall binders (`∀x, ...`) → `FVAR_0`, `FVAR_1`, ... (cap 63, overflow → `FVAR_MANY`)
- Lambda binders (`fun x => ...`) → `BVAR_0`, `BVAR_1`, ... (cap 63, overflow → `BVAR_MANY`)

Each counter resets to 0 per graph. Scoped names (`∀:declName/depth/x`) are used internally
during extraction to prevent cross-declaration collisions, then mapped to positional tokens by
the encoder. Round-trip verified 2,554/2,554 via `validate_roundtrip.py`.

## 8) Limitations

- **Corpus scale.** All training results are on 14 Mathlib algebra/topology/order modules
  (~3.5K examples). IRCoder's positive IR-grounding results appear at millions of files
  and 1.1B+ parameters; whether Maith's null holds at larger corpus/model scale is open.
- **Current v2 result (DEC-026/027, 2026-08-09).** Full control grid:

  | Variant | Representation | Vocab | Params | Perplexity | Top-1 Acc |
  |---|---|---|---|---|---|
  | A (v2 IR) | Semantic IR graph → tokens | 601 | 358M | 1.2361 | 90.0% |
  | B-small (BPE control) | Raw BPE truncated to 601 | 601 | 358M | 1.1294 | 90.5% |
  | B (full BPE) | Raw BPE, full vocab | 151,643 | 494M | 1.107 | 91.7% |
  | C (AST BPE) | AST-split BPE, full vocab | 151,643 | 494M | 1.098 | 93.0% |

  The A-vs-B/C gap is a **size effect** (B-small at matched params ties A), not a
  representation deficit *under prediction-family metrics at this scale*. The IR encodes
  real semantic content (DEC-025: 62pp probe gap vs. flat-IR), but that structure is not
  rewarded by the next-token objective at this scale. Whether it helps under non-prediction
  metrics (retrieval, ATP) is open (H6/H7). Cold-start (DEC-021) and model-size (DEC-027)
  confounds are both ruled out *for the prediction-metric null*.
  *(Epoch note: B and C are 3-epoch v1-era runs; A and B-small are 2-epoch v2 runs. The
  A-vs-B-small comparison is epoch-matched and is the clean representation test; A-vs-B/C
  is not. See `docs/experiments/V2_COMPARISON_MATRIX.md` for per-run epoch counts.)*
- **Objective mismatch is the leading hypothesis for the null.** Next-token prediction may
  not be the objective that rewards semantic structure; a downstream task (proof search,
  completion) or a masked-reconstruction objective might. This is untested.
- **No downstream task evaluation yet.** Current evidence is perplexity, completion
  accuracy, and probing — not theorem-proving success. See section 10.
- For the current authoritative experiment state, use:
  - `docs/decisions/LOG.md` (DEC-026, DEC-027)
  - `docs/experiments/V2_COMPARISON_MATRIX.md`
  - `python3 python/compare_results.py --runs-dir runs/`
- `Transpiler.lean` provides both debug formatting and Lean syntax decompilation via
  `Decompile.decompileGraph`. See `docs/reference/DECOMPILER_HANDOVER.md`.

## 9) Research Roadmap

1. **Phase 1: Build semantic IR** — ✅ done
2. **Phase 2: Extract Mathlib corpus** — ✅ done (2,554 declarations, 4 modules, 100% coverage; v2 corpus expanded to 14 modules / 4,029 examples)
3. **Phase 2.5: Stable encoder format + vocab** — ✅ done (v2.0.0, 601 tokens; decoder round-trip 2554/2554)
4. **Phase 3: Build token vocabulary + dataset** — ✅ done (`python/build_dataset.py`, A/B/C splits, `vocab_A.json`)
5. **Phase 4: Tokenizer fragmentation study** — ✅ done (1.69x BPE inflation on Lean source)
6. **Phase 5–6: Run A/B/C training + v2 control grid** — ✅ complete (2026-08-09). Full grid in section 8. Representation hypothesis not yet supported on perplexity/completion; cold-start (DEC-021) and size (DEC-027) confounds ruled out.
7. **Phase 7: IR-candidate search** — active. Next levers per DEC-027: corpus expansion (>10k examples), objective redesign (masked-reconstruction / proof-completion), IR pretraining, and downstream theorem-proving evaluation. See [`docs/history/PHASE_7_ROADMAP.md`](docs/history/PHASE_7_ROADMAP.md).

Related work is surveyed in [`docs/reference/PRIOR_ART.md`](docs/reference/PRIOR_ART.md); project terminology is defined in [`docs/reference/GLOSSARY.md`](docs/reference/GLOSSARY.md).

Remaining IR milestones with current size estimates:

All previously identified failure categories have been resolved:
- ✅ **HOF application** (`bvar`/`fvar` as function head): fixed, emits `gen:hof` operation
- ✅ **Projection expressions (`.proj`)**: fixed, emits `gen:proj:TypeName/idx` operation
- ✅ **`letE` support**: fixed, pushes scoped bound entity and recurses into body
- ✅ **Heterogeneous `Eq` arity**: fixed, `HEq` handled explicitly; other arities fall back to generic operation

## 10) Evaluation Plan

The core hypothesis is tested by training and comparing models on:

1. Lean source tokens
2. Lean AST serialization
3. Maith semantic IR tokens

Metrics:

- Next-token prediction quality — ✅ run (v2 control grid, DEC-026/027)
- Linear-probe semantic content — ✅ run (DEC-025: IR encodes semantics, 62pp gap)
- Completion accuracy (top-1) — ✅ run (DEC-026/027)
- Proof completion performance — ❌ not started
- Automated theorem-proving success rate — ❌ not started
- Proof-search efficiency (time/steps) — ❌ not started
- Embedding quality for mathematical similarity/retrieval — ❌ not started

**Current evidence state:** next-token prediction, completion accuracy, and probing are
complete. The representation hypothesis is **not yet supported** on these metrics: the IR
encodes semantics (DEC-025) but does not outperform a size-matched BPE baseline (DEC-027).
The leading interpretation is objective mismatch — next-token prediction may not reward
semantic structure. Downstream theorem-proving evaluation (the actual research claim) is
not started; it is the decisive test that would distinguish "the IR helps proving but not
perplexity" from "the IR does not help." See [`docs/history/PHASE_7_ROADMAP.md`](docs/history/PHASE_7_ROADMAP.md).

## 11) Building the Project

```bash
git clone <repo-url>
cd Maith
lake build tests
./.lake/build/bin/tests
```

To run the corpus build:

```bash
cd Maith
lake build buildCorpus
lake env ./.lake/build/bin/buildCorpus
```

Outputs are written under `Corpus/`:

- `Corpus/corpus.jsonl`
- `Corpus/stats.json`
- `Corpus/logs.txt`

This repository currently tracks these corpus artifacts intentionally so readers can inspect real output directly.

## 12) Repository Structure

```text
Maith/
  MetaExtractor.lean       # elaborated Lean Expr -> IR graph
  EntityId.lean            # includes EntityId.bound for scoped binders
  Normalizer.lean          # canonical ordering/normalization
  Encoder.lean             # graph -> token sequence (v2.0.0, 601-token vocab)
  Decoder.lean             # token -> graph parser (backward compat across versions)
  Transpiler.lean          # debug + Lean syntax decompilation via Decompile.decompileGraph
  CorpusSerializer.lean    # JSONL/stat serialization
  ProcessingPipeline.lean  # extraction + normalize + encode flow
  MathlibCorpusBuilder.lean
  ...
Tests/
  CorpusPipelineTests.lean
  InjectivityTests.lean
  DecoderTests.lean
  EncoderTests.lean
  ExtractionFaithfulnessTests.lean
  RoundTripTests.lean
  ...
python/
  corpus_loader.py         # schema validation, loading, vocab build, split, dataset class
  build_dataset.py         # A/B/C dataset builder: IR vocab + BPE variants, train/eval splits
  build_b_small_vocab.py   # DEC-027 B-small control: truncate BPE to 601 tokens
  build_flat_ir_dataset.py # DEC-024 flat-IR ablation: shape-only (SLOT) dataset
  embed_project.py         # DEC-021 embedding projection (warm-start IR vocab from Qwen BPE)
  train.py                 # fine-tuning script: one variant per run, reports eval perplexity
  eval_completion.py       # top-1 completion accuracy evaluation (A/B/C/B_SMALL)
  extract_representations.py  # DEC-025 probing: forward-pass to frozen hidden states
  probing_task.py          # DEC-025 probing: linear probe module classification
  compare_results.py       # prints A/B/C comparison table from runs/variant_*/results.json
  validate_roundtrip.py    # decoder round-trip validator (confirms BVAR/TERM stability)
  tokenizer_study.py       # BPE fragmentation study vs Qwen2.5-Coder
docs/
  decisions/LOG.md         # experiment decisions and confound documentation (DEC-001..027)
  decisions/INDEX.md       # navigation index for decision log
  experiments/             # experiment designs, comparison matrix, probing task
  reference/               # Design.md, ENCODER_FORMAT, PRIOR_ART, EXAMPLE_ROUNDTRIP, ...
  history/                 # PHASE_5/6/7 results, roadmap, validation plan
  scratch/                 # working notes (audits, analysis, resume notes)
  README.md                # docs index
```

## 13) License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
