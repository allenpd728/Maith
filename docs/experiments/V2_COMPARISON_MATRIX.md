# V2 IR Comparison Matrix

> Tracks all controlled experiments comparing IR representations and model sizes.
> Update this document whenever a new cell is filled in. Each row is a single
> training run; the grid below is the target structure for a fair comparison.

## Why this document exists

The v2 IR (C1+C2+C4) experiment produced a result — A at 90.0% completion accuracy
vs B at 91.7% and C at 93.0% — but the comparison is confounded by model size.
Variant A uses a 601-token embedding table (358M params total); Variants B and C
use a 151k-token table (494M params). Two variables changed at once: representation
*and* model size. This matrix tracks the experiments needed to isolate each variable.

## The 2×2 control grid

| | Small model (358M params) | Large model (494M params) |
|---|---|---|
| **IR vocab (601 tokens)** | **A** ✅ 90.0% acc / 1.2361 ppl | **A-large** ❌ untested |
| **BPE vocab (151k tokens)** | **B-small** ❌ untested | **B** ✅ 91.7% acc / 1.107 ppl |
| **BPE variant vocab (151k tokens)** | **C-small** ❌ untested | **C** ✅ 93.0% acc / 1.098 ppl |

**Priority:** B-small is the most important missing cell. It directly answers whether
A's competitiveness comes from the IR representation or simply from both A and B-small
being small models on a narrow task.

## Completed runs

### Variant A — IR v2 (C1+C2+C4), small model
- **Run dir:** `runs/variant_A_v2_full/checkpoint-final`
- **IR version:** `semantic_graph_ir_v2_0_0` (C1 polarity removal, C2 typeclass enrichment, C4 GEN module bucketing)
- **Base model:** Qwen2.5-Coder-0.5B
- **Vocab size:** 601 tokens (`datasets/vocab_A.json`)
- **Params:** 358.4M
- **Training data:** 3,491 examples, 14 Mathlib modules, seed=42
- **Epochs:** 2
- **Eval perplexity:** 1.2361
- **Completion accuracy (top-1, last 10 tokens, 200 samples):** 90.0% (1,799/2,000)
- **Date:** 2026-08-08
- **Notes:** First full C1+C2+C4 run. C4 improved over C1+C2 partial (1.2458 → 1.2361).

### Variant B — BPE, large model
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

### Variant C — BPE variant, large model
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

### B-small — BPE, small model ❌ (PRIORITY)
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

### A-large — IR v2, large model ❌ (lower priority)
- **Purpose:** Shows the ceiling of the IR approach at B/C's parameter count.
- **Run dir:** `runs/variant_A_large/` (to be created)
- **IR version:** `semantic_graph_ir_v2_0_0`
- **Base model:** Qwen2.5-Coder-0.5B with embedding table expanded to match 494M params
- **Vocab size:** 601 tokens
- **Params:** 494M
- **Training data:** same 3,491 examples, seed=42
- **Epochs:** 2
- **Status:** Not started. Lower priority than B-small.

### C-small — BPE variant, small model ❌ (optional, closes the grid)
- **Purpose:** Completes the 2×2. Paired with C to isolate size effect on the BPE variant.
- **Status:** Not started. Lower priority than B-small.

## How to add a new IR candidate

When testing a new IR representation (e.g. C3 attribute sparsity, or a future v3 IR):

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
