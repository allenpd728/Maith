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

## Outcome framework

- **A gap shrinks at 3 epochs**: DEC-006 (cold-start) is the primary cause; IR tokens not yet ruled out
- **A gap holds at 3 epochs**: IR tokens hurt at this scale; investigate vocab size or representation design
- **A ≈ B/C at 3 epochs**: IR tokens are competitive once the embedding disadvantage is removed
