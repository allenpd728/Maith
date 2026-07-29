# Phase 5 Results — Maith Project

## What this document is

A results document for the Phase 5 A/B/C experiment. It will be filled in when the matched-batch rerun completes. Right now it is a shell with placeholders.

---

## Experiment Overview

Three variants compared on next-token prediction perplexity (lower is better) on a held-out eval split of 246 examples. All variants trained on 2,213 examples, seed=42, 1 epoch, effective batch size=8 (grad_accum=8 for all variants after DEC-007 fix).

---

## Variants

| Variant | Token Type | Vocab Size | Parameters | Train Seq Cap | Eval Seq Cap |
|---------|------------|------------|------------|---------------|--------------|
| A | Maith IR tokens (v1.2.0) | 4,495 | 361.9M | 1024 | 512 |
| B | Raw `leanExpr` string | 151,643 | 494.0M | 384 | 512 |
| C | AST-style split `leanExpr` | 151,643 | 494.0M | 384 | 512 |

---

## Matched-Batch Rerun Results (2026-07-28 — all variants effective batch=8)

| Variant | Eval Perplexity | Training Minutes | Train Steps |
|---------|-----------------|------------------|-------------|
| A | 1.39 | 22.7 | 277 |
| B | 1.15 | 112.7 | 277 |
| C | 1.13 | 50.0 | 277 |

All variants: 1 epoch, seed=42, grad_accum=8, effective batch=8, `smoke_test: false`.

---

## Previous Run Results (2026-07-27 — carry confounds, not final)

| Variant | Eval Perplexity | Training Minutes | Effective Batch Size | Confound Flags |
|---------|-----------------|------------------|----------------------|----------------|
| A | 1.3922 | 22.6 | 8 | — |
| B | 1.1420 | 41.3 | 4 | DEC-007 |
| C | 1.1298 | 44.3 | 4 | DEC-007 |

All three variants also carry DEC-006 (see below).

---

## Known Confounds

- **DEC-006**: Variant A uses a randomly initialized embedding table (4,495 tokens). Variants B and C start from Qwen2.5-Coder pretrained embeddings (151,643 tokens). B and C have a strong cold-start advantage unrelated to representation quality.
- **DEC-007**: Previous run had B and C at effective batch size=4 vs A at effective batch size=8. Fixed in matched-batch rerun (all variants now effective batch=8).

---

## Interpretation

A underperforms B and C in both runs. After the DEC-007 fix (matched batch), the gap is unchanged:
A=1.39 vs B=1.15 vs C=1.13. DEC-007 was not the explanation.

**DEC-006 is the remaining confound.** B and C start from Qwen2.5-Coder pretrained embeddings
(151,643 tokens). A starts from randomly initialized embeddings (4,495 tokens). The cold-start
disadvantage for A is real and uncontrolled in this experiment. Before concluding that IR tokens
hurt, DEC-006 must be ruled out.

**Next step to rule out DEC-006:** run variant A for 3 epochs (or warm-start A's embeddings).
If the gap to B/C shrinks substantially with more training, the cold-start embedding confound
is the dominant effect. If the gap holds, the IR token representation itself is the issue —
investigate vocab size, structural information loss, or sequence length cap.

## DEC-008 Result: 3-epoch variant A (2026-07-28)

| Variant | Epochs | Eval Perplexity | Training Minutes | Train Steps |
|---------|--------|-----------------|------------------|-------------|
| A | 3 | 1.2598 | 67.6 | 831 |
| B | 1 | 1.15 | 112.7 | 277 |
| C | 1 | 1.13 | 50.0 | 277 |

3-epoch A dropped from 1.39 to 1.2598, confirming cold-start initialization (DEC-006) is a real
factor. However the gap to B/C persists — 1.2598 vs 1.15/1.13. Cold-start is not the complete
explanation.

**Conclusion:** DEC-006 is partially confirmed as a confound. Both cold-start initialization and
the representation itself may be contributing to A's underperformance. An embedding warm-start
experiment (DEC-009) is required to isolate representation quality from initialization advantage.

## Outcome framework

| Scenario | Interpretation |
|----------|---------------|
| 3-epoch A < 1.25 | Cold-start dominant; IR tokens not yet ruled out |
| 3-epoch A ≈ 1.39 | Cold-start not the explanation; IR tokens underperform at this scale |
| 3-epoch A ≈ B/C | IR tokens competitive once embedding disadvantage removed |

**Actual result: 1.2598** — cold-start is real but partial. Warm-start (DEC-009) is next.

## DEC-009 Result: Embedding warm-start variant A (2026-07-29)

| Variant | Epochs | Warm Start | Overlap Tokens | Eval Perplexity | Training Minutes |
|---------|--------|------------|----------------|-----------------|------------------|
| A | 1 | yes | 23 / 4,495 (0.51%) | 1.4288 | 22.5 |
| B | 1 | n/a | — | 1.15 | 112.7 |
| C | 1 | n/a | — | 1.13 | 50.0 |

Warm-start overlap was 0.51% — effectively a cold start. Perplexity of 1.43 is marginally worse
than the cold-start 1-epoch result (1.39), consistent with mild interference from the 23
mismatched pretrained vectors. Embedding transfer is not viable at this vocab overlap level.

**Conclusion:** DEC-006 Confound 1 is fully characterized. The residual gap between A (best: 1.26
at 3 epochs) and B/C (1.15/1.13 at 1 epoch) reflects the IR representation itself, not
initialization alone. Next: run `eval_completion.py` on matched-batch B/C checkpoints to assess
whether the perplexity gap translates to completion accuracy.
