# V2 IR Comparison Matrix

> **⚠ RESET NOTICE (2026-08-12):** All numbers in this matrix are from invalidated
> runs (split leakage, epoch confounds, dataset contamination — see DEC-031/032).
> They are retained as a historical record but are NOT valid evidence. Clean retrains
> are complete (A_v3_2ep, B_small_clean, flat_clean — all 3375/376, 2 epochs) but
> hypothesis tests have not been re-run under verified quality gates. See
> [`HYPOTHESIS_GRID`](HYPOTHESIS_GRID.md) for current status.

> Tracks all controlled experiments comparing IR representations and model sizes.
> Update this document whenever a new cell is filled in. Each row is a single
> training run; the grid below is the target structure for a fair comparison.

> See also: [V1_COMPARISON_MATRIX.md](V1_COMPARISON_MATRIX.md) — the historical v1 grid
> that motivated the v2 redesign (no size control, epoch confound, 1,236-vocab IR).

## Why this document exists

The v2 IR (C1+C2+C4) experiment produced a result — A at 90.0% completion accuracy
vs B at 91.7% and C at 93.0% — that was initially confounded by model size: Variant A
uses a 601-token embedding table (358M params total) while Variants B and C use a
151k-token table (494M params). This matrix tracks the controlled experiments needed to
isolate representation from size. **The size confound is now resolved (DEC-027):** the
B-small cell (BPE truncated to 601 tokens / 358M params) ties A at 90.5% vs. 90.0%,
confirming the A-vs-B/C gap is a parameter-count effect, not a representation deficit
*under prediction-family metrics (perplexity, completion accuracy) at this scale*.

## The 2×2 control grid

> **Scale note.** All variants share the same Qwen2.5-Coder-0.5B transformer base
> (toy-scale by field standards; see the [model-size taxonomy](EXPERIMENT_DESIGN.md#model-size-taxonomy)
> in EXPERIMENT_DESIGN). The parameter difference between the two columns is
> **embedding-table size** (601 vs 151K tokens), not transformer capacity (attention
> heads, FFN layers, hidden dims are identical). "Narrow-vocab" and "Full-vocab" name
> the actual variable; neither column is "large" in absolute terms.

| | Narrow-vocab (601 tokens, 358M) | Full-vocab (151K tokens, 494M) |
|---|---|---|
| **IR vocab (601 tokens)** | **A** ✅ 90.0% acc / 1.2361 ppl | **A-large** ❌ untested |
| **BPE vocab (151k tokens)** | **B-small** ✅ 90.5% acc / 1.1294 ppl (v2, 2ep) | **B** ✅ 91.7% acc / 1.107 ppl (v1-era, 3ep; re-run in progress) |
| **BPE variant vocab (151k tokens)** | **C-small** ❌ untested | **C** ✅ 93.0% acc / 1.098 ppl (v1-era, 3ep; re-run queued) |

**Priority:** ~~B-small is the most important missing cell.~~ **B-small complete (DEC-027).**
B-small = 90.5% acc / 1.1294 ppl — essentially tied with A (90.0% / 1.2361) at matched
params. The size confound is confirmed: A's deficit vs B/C is a parameter-count effect,
not a representation deficit *under prediction-family metrics at this scale*. The v2 IR is
not yet doing measurable work *under prediction metrics (perplexity, completion accuracy)*
beyond small-vocab BPE at this scale; whether it helps under non-prediction metrics
(retrieval, ATP) is open (H6/H7 in [`HYPOTHESIS_GRID`](HYPOTHESIS_GRID.md)). See DEC-027
for the full interpretation and next steps.

## Completed runs

### Variant A — IR v2 (C1+C2+C4), narrow-vocab
- **Run dir:** `runs/variant_A_v2_full/checkpoint-final`
- **IR version:** `semantic_graph_ir_v2_0_0` — Variant A serializes Maith's **semantic IR graph** (entity/relation/operation rows from elaborated Lean) into token sequences. v2 simplifies the v1 graph: C1 strips polarity noise, C4 collapses granular `gen:FullName` into `GEN_<area>` buckets (vocab 1,236→601), C2 adds typeclass names.
- **Base model:** Qwen2.5-Coder-0.5B
- **Vocab size:** 601 tokens (`datasets/vocab_A.json`)
- **Params:** 358.4M
- **Training data:** 3,491 examples, 14 Mathlib modules, seed=42
- **Epochs:** 2
- **Eval perplexity:** 1.2361
- **Completion accuracy (top-1, last 10 tokens, 200 samples):** 90.0% (1,799/2,000)
- **Date:** 2026-08-08
- **Notes:** First full C1+C2+C4 run. C4 improved over C1+C2 partial (1.2458 → 1.2361).

### Variant B — BPE, full-vocab
- **Run dir:** `runs/variant_B2/checkpoint-final`
- **IR version:** raw `leanExpr` string, Qwen BPE tokenizer
- **Base model:** Qwen2.5-Coder-0.5B (full embedding table = 494M)
- **Vocab size:** 151,643 tokens
- **Params:** 494M
- **Training data:** 3,491 examples, 14 Mathlib modules, seed=42
- **Epochs:** 3
- **Eval perplexity:** 1.107
- **Completion accuracy (top-1, last 10 tokens, 200 samples):** 91.7% (1,816/1,980)
- **Date:** 2026-08-01
- **Notes:** Authoritative B run. Avoid stale `runs/variant_B/` (checkpoint/results mismatch).

### Variant C — BPE variant, full-vocab
- **Run dir:** `runs/variant_C_v2/checkpoint-final`
- **IR version:** AST-style split `leanExpr`, Qwen BPE tokenizer
- **Base model:** Qwen2.5-Coder-0.5B (full embedding table = 494M)
- **Vocab size:** 151,643 tokens
- **Params:** 494M
- **Training data:** 3,491 examples, 14 Mathlib modules, seed=42
- **Epochs:** 3
- **Eval perplexity:** 1.098
- **Completion accuracy (top-1, last 10 tokens, 200 samples):** 93.0% (1,850/1,990)
- **Date:** 2026-08-01
- **Notes:** Authoritative C run. Avoid stale `runs/variant_C/` (checkpoint/results mismatch).
  C's 1.5% earlier result was a stale checkpoint bug — see `docs/scratch/variant_c_eval_bug.md`.

## Planned runs

### B-small — BPE, narrow-vocab ❌ (PRIORITY)
- **Purpose:** Control for model size. If B-small ≈ A (90%), size explains the gap.
  If B-small << A, the IR representation is doing real work.
- **Run dir:** `runs/variant_B_small/` (to be created)
- **IR version:** raw `leanExpr` string, Qwen BPE tokenizer
- **Base model:** Qwen2.5-Coder-0.5B **with embedding table truncated to 601 tokens**
  (or alternatively: freeze embedding table at 601 tokens, pad remaining param budget
  into FFN layers to match 358M total)
- **Vocab size:** 601 tokens (most frequent 601 BPE tokens from training corpus)
- **Params:** 358M (to match A exactly)
- **Training data:** same 3,491 examples, seed=42
- **Epochs:** 2 (to match A)
- **Gate:** B-small completion accuracy relative to A's 90.0%
- **Decision:** If B-small < 88%, IR structure is carrying A. If B-small ≥ 90%, size is the confound.
- **Status:** Not started. Requires vocab truncation script and model resize.

### B-small — BPE, narrow-vocab ✅ (DEC-027)
- **Run dir:** `runs/variant_B_small/checkpoint-final`
- **IR version:** raw `leanExpr` string, Qwen BPE tokenizer truncated to 601 tokens
- **Base model:** Qwen2.5-Coder-0.5B (embedding table truncated to 601)
- **Vocab size:** 601 tokens (`datasets/vocab_B_small.json`, top-601 BPE, 99.92% coverage)
- **Params:** 358.4M (matching A exactly)
- **Training data:** 3,491 examples (remapped to compact 0..600 IDs), seed=42
- **Epochs:** 2
- **Eval perplexity:** 1.1294
- **Completion accuracy (top-1, last 10 tokens, 200 samples):** 90.5% (1,792/1,980)
- **Date:** 2026-08-09
- **Notes:** Size control for DEC-027. B-small ≈ A (90.5% vs 90.0%) at matched params →
  size explains the gap, not the representation. Vocab builder: `python/build_b_small_vocab.py`.

### A-large — IR v2, full-vocab ❌ (lower priority)
- **Purpose:** Shows the ceiling of the IR approach at B/C's parameter count.
- **Run dir:** `runs/variant_A_large/` (to be created)
- **IR version:** `semantic_graph_ir_v2_0_0`
- **Base model:** Qwen2.5-Coder-0.5B with embedding table expanded to match 494M params
- **Vocab size:** 601 tokens
- **Params:** 494M
- **Training data:** same 3,491 examples, seed=42
- **Epochs:** 2
- **Status:** Not started. Lower priority than B-small.

### C-small — BPE variant, narrow-vocab ❌ (optional, closes the grid)
- **Purpose:** Completes the 2×2. Paired with C to isolate size effect on the BPE variant.
- **Status:** Not started. Lower priority than B-small.

## How to add a new IR candidate

When testing a new IR candidate (e.g. C3 attribute sparsity, a v2.x tweak, or a v3 redesign):

1. Add a new row section under "Completed runs" when training finishes.
2. Record: IR version, vocab size, params, eval perplexity, completion accuracy, run dir, date.
3. Update the 2×2 grid above (or extend it with a new row for the new IR).
4. Write a DEC-0XX decision record in `docs/decisions/LOG.md` covering the gate condition and result.
5. Ensure `datasets/representation_manifest.json` is updated to the new IR version before rebuilding.

## Eval protocol (reproducible)

All completion accuracy numbers in this document use:
```bash
python3 python/eval_completion.py \
  --variants <X> \
  --checkpoint-<X> runs/<authoritative_dir> \
  --samples 200 \
  --mask-last 10
```

Always pass explicit `--checkpoint-*` overrides — never rely on the default
`runs/variant_X/` path, which may be stale. See `docs/scratch/runs_audit.md` for the
full stale-checkpoint history and `docs/scratch/variant_c_eval_bug.md` for the C case study.

## Current interpretation (2026-08-08)

A is within 1.7pp of B and 3pp of C on completion accuracy, using a 250× smaller
vocabulary and 136M fewer parameters. The result is competitive but not conclusive:

- **Perplexity gap** (A: 1.24, B/C: 1.10) may reflect model size more than representation quality.
- **Completion accuracy gap** (A: 90%, B: 91.7%, C: 93%) is consistent with a size effect.
- **B-small is the deciding experiment.** Until it runs, the hypothesis that IR structure
  improves learning efficiency cannot be cleanly separated from the size confound.

The representation hypothesis is incrementally supported (A is competitive at smaller scale)
but not yet proven. DEC-027 will formally scope the B-small experiment.
