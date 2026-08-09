# AGENTS.md

> Condensed project reference — dense by design, not narrative prose. For human-readable
> introductions see `README.md`; for term definitions see `docs/reference/GLOSSARY.md`.

## Project overview

Maith is a Lean 4 project that extracts a canonical semantic intermediate representation
(IR) from formal mathematics and tests whether training language models on that IR —
rather than raw source syntax — improves performance on formal-math tasks.

- **Domain:** formal mathematics (Lean 4 / Mathlib)
- **Base model:** Qwen2.5-Coder-0.5B (all variants share this transformer base)
- **Hardware constraint:** consumer M4 Mac, 16GB unified memory (MPS) — limits all
  experiments to the toy tier (<1B params); see model-size taxonomy below
- **Central hypothesis:** a canonical semantic representation improves formal math
  tooling. Decomposed into 10 testable sub-claims (H1–H10); see hypothesis grid below.

## Current state (v2 era, 2026-08-09)

- **IR version:** `semantic_graph_ir_v2_0_0` (C1 polarity removal, C2 typeclass
  enrichment, C4 GEN module bucketing). Vocab: 601 tokens.
- **Corpus:** 14 Mathlib modules (algebra, order, topology), ~4,029 extracted
  declarations → 3,491 train / 388 eval after filtering.
- **Latest decision:** DEC-027 (B-small size control — complete).
- **Result summary:** The IR encodes semantic structure (DEC-025: 75.4% linear-probe
  accuracy vs 13.6% for shape-only ablation, 62-point gap) but does not improve
  next-token prediction or completion accuracy over size-matched BPE at toy scale
  (DEC-027). Whether it helps under non-prediction metrics (retrieval, ATP) or at
  larger transformer capacity is open.

## Key terms (compressed — see `docs/reference/GLOSSARY.md` for full definitions)

| Term | One-line meaning |
|---|---|
| **A (Variant A)** | IR tokens (semantic graph), 601-vocab, 358M params |
| **B (Variant B)** | Raw leanExpr via Qwen BPE, 151K-vocab, 494M params |
| **C (Variant C)** | AST-split leanExpr via Qwen BPE, 151K-vocab, 494M params |
| **B-small** | BPE truncated to 601 tokens, 358M params (size-matched control for A) |
| **IR** | Intermediate Representation — semantic graph (Entity/Attribute/Relation/Operation) extracted from Lean's elaborated `Expr` |
| **Flat-IR** | DEC-024 ablation: IR with all semantic content replaced by SLOT (shape only) |
| **C1/C2/C4** | v2 IR changes: polarity removal / typeclass enrichment / GEN module bucketing |
| **DEC-0XX** | Decision-log entry in `docs/decisions/LOG.md` |
| **H1–H10** | Hypothesis-grid sub-claims in `docs/experiments/HYPOTHESIS_GRID.md` |

## Experiment structure

The 2×2 control grid (representation × vocabulary size). All variants share the same
Qwen2.5-Coder-0.5B transformer base; the parameter difference between columns is
**embedding-table size**, not transformer capacity.

| | Narrow-vocab (601 tokens, 358M) | Full-vocab (151K tokens, 494M) |
|---|---|---|
| **IR vocab** | **A** ✅ | **A-large** ❌ N/A (601-token IR can't meaningfully fill 151K embedding rows) |
| **BPE vocab** | **B-small** ✅ | **B** ✅ |
| **AST BPE vocab** | **C-small** ❌ untested | **C** ✅ |

## Results table

| Variant | Representation | Vocab | Params | Epochs | Perplexity | Top-1 Acc | Era |
|---|---|---|---|---|---|---|---|
| A | Semantic IR graph | 601 | 358M | 2 | 1.2361 | 90.0% | v2 (authoritative) |
| B-small | BPE truncated to 601 | 601 | 358M | 2 | 1.1294 | 90.5% | v2 (authoritative) |
| B | Raw BPE | 151,643 | 494M | 3 | 1.107 | 91.7% | v1-era (epoch confound) |
| C | AST-split BPE | 151,643 | 494M | 3 | 1.098 | 93.0% | v1-era (epoch confound) |

**Epoch note:** B and C are 3-epoch v1-era runs; A and B-small are 2-epoch v2 runs. The
A-vs-B-small comparison is epoch-matched and is the clean representation test. B/C 2-epoch
re-runs are in progress.

## Model-size taxonomy

All Maith variants fall in the **toy** tier. See `docs/experiments/EXPERIMENT_DESIGN.md`
section "Model-size taxonomy" for the full table.

| Tier | Params | Maith tested? |
|---|---|---|
| Toy / micro | <1B | ✅ All variants (358M–494M) |
| Small | 1B–8B | ❌ (IRCoder positive results start at 1.1B) |
| Medium | 8B–30B | ❌ |
| Large | 30B–100B | ❌ |
| Frontier | 100B+ | ❌ |

"Open at larger scale" means open at larger **transformer capacity** (a 1B+ base model),
not just a bigger embedding table on the same 0.5B base.

## Hypothesis grid summary

See `docs/experiments/HYPOTHESIS_GRID.md` for full detail (question, evidence, closure
condition per row).

| # | Sub-claim | Status |
|---|---|---|
| H1 | IR encodes semantic structure | ✅ Closed (positive) |
| H2 | IR improves next-token prediction over BPE | ❌ Closed (negative) |
| H3 | Cold-start embedding init explains the null | ❌ Closed (ruled out) |
| H4 | Model size explains the null | ◐ Partial (ruled out at 358M; open at 1B+) |
| H5 | Training objective is the bottleneck | ⬜ Open (mechanism supported, not directly tested) |
| H6 | IR improves retrieval / similarity tasks | ⬜ Open (highest-value next experiment) |
| H7 | IR improves proof completion / ATP | ⬜ Open (downstream research claim) |
| H8 | IR advantage appears only above a scale threshold | ⬜ Open (blocked on compute) |
| H9 | Co-training recovers gains where replacement didn't | ⬜ Open (PACT precedent) |
| H10 | IR beats AST on semantic probing beyond module classification | ◐ Partial (DEC-025 tested module class only) |

## File map

| Path | Contents |
|---|---|
| `README.md` | Human-readable project introduction, results, roadmap |
| `docs/decisions/LOG.md` | Chronological experiment record (DEC-001 through DEC-027) |
| `docs/decisions/INDEX.md` | Navigation index for the decision log |
| `docs/experiments/HYPOTHESIS_GRID.md` | Decomposed sub-claims (H1–H10) with status, evidence, closure conditions |
| `docs/experiments/V2_COMPARISON_MATRIX.md` | Current 2×2 control grid (v2 era) |
| `docs/experiments/V1_COMPARISON_MATRIX.md` | Historical v1 grid (pre-size-control) |
| `docs/experiments/EXPERIMENT_DESIGN.md` | Experiment protocol, hyperparameters, model-size taxonomy |
| `docs/experiments/V2_NEXT_STEPS.md` | Phase tracker and candidate directions |
| `docs/reference/GLOSSARY.md` | Term definitions for cross-domain readers |
| `docs/reference/PRIOR_ART.md` | Related-work survey (IRCoder, PACT, JEPA, AMR, interlingua) |
| `docs/reference/Design.md` | Architecture and IR design decisions |
| `docs/reference/ENCODER_FORMAT.md` | Canonical token format spec (v2.0.0) |
| `docs/reference/EXAMPLE_ROUNDTRIP.md` | Full pipeline walkthrough (neg_neg) |
| `docs/history/PHASE_7_ROADMAP.md` | Current roadmap and open items |

## Build and test commands

**Lean (corpus extraction + tests):**
```bash
lake build tests
./.lake/build/bin/tests          # 66+ unit tests + integration checks

# Corpus rebuild:
lake build buildCorpus
lake env ./.lake/build/bin/buildCorpus   # outputs to Corpus/
```

**Python (training + evaluation):**
```bash
python3 python/train.py --variant A --datasets datasets/ --embed-project datasets/embed_proj_A.pt
python3 python/eval_completion.py --variants A B_SMALL --samples 200 --mask-last 10
python3 python/validate_roundtrip.py     # decoder round-trip validator
python3 python/summarize_abc_results.py  # 2×3 grid comparison table
```

**Python test suite:**
```bash
python3 python/test_train_regression.py
python3 python/test_build_dataset.py
python3 python/test_variant_config.py
python3 python/test_pipeline_integration.py
```

## Conventions

**Decision log (`docs/decisions/LOG.md`):**
- Append-only; supersede by adding a newer entry that references the old one
- Format: `### DEC-0XX — Title` with Date, Status, Scope, Decision, Rationale, References
- Name confounds *before* results arrive, then run controlled experiments to isolate them

**Hypothesis grid (`docs/experiments/HYPOTHESIS_GRID.md`):**
- H-numbers are stable; add rows for new sub-claims, don't delete closed rows
- Each row: Question (plain language) → Status (✅/❌/◐/⬜) → Sub-claim → Evidence → Closure condition
- Update status when a DEC entry resolves a sub-claim

**Negative-claim scoping:**
- Every instance of "not a representation deficit" or "not doing measurable work" must
  carry the qualifier: *under prediction-family metrics (perplexity, completion accuracy)
  at this scale*
- The evidence supports a scoped negative (prediction metrics, toy scale), not a broad one
- Non-prediction metrics (retrieval, ATP) are open (H6/H7)

**Grid axis terminology:**
- Use "narrow-vocab" / "full-vocab" for the 2×2 grid columns (not "small model" /
  "large model" — both variants are toy-scale; the difference is embedding-table size,
  not transformer capacity)
- Variant letters (A, B, C, B-small) are retained; relabeling applies to axes only

**IR versions:**
- v1.2.0 → v1.3.0 → v1.4.0 → v2.0.0 (current). Each version has a different vocab size.
- "0.5B" in Qwen2.5-Coder-0.5B is the model name, not the param count after resize.
  Base ships at ~494M; after vocab resize to 601 tokens, Variant A is ~358M.

## Pitfalls to avoid

- Don't state the hypothesis is disproven based on toy-scale results — it hasn't been
  tested at meaningful scale yet, and that's a real gap, not an excuse. But don't use
  "still open" to imply the current evidence is neutral or encouraging either: three of
  four tested angles are null, and the pattern so far does not favor the hypothesis.
  State both facts together, every time. If "test at scale" is invoked, it must actually
  be run, not held as a permanent escape hatch.
- Don't use "small model" / "large model" for grid axes — both are toy-scale; use
  narrow-vocab / full-vocab.
- Don't cite B/C numbers (91.7% / 93.0%) without the epoch-confound caveat (3-epoch v1
  vs 2-epoch v2). The clean comparison is A vs B-small (epoch-matched) — and that
  comparison is also a loss for A (90.0% vs 90.5% top-1, 1.2361 vs 1.1294 perplexity).
- Don't inflate the probing result into task-level evidence. DEC-025 shows the IR's
  semantics are encoded in model representations — a necessary condition for the
  hypothesis, not evidence of task benefit. Encoding has not translated to any measured
  task advantage. Don't use "it helps under probing" to imply the representation is
  working; encoding ≠ helping.
- Don't conflate embedding-table size with transformer capacity — the 136M parameter gap
  is embedding rows, not attention/FFN layers.
