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

## Matched-Batch Rerun Results (current run — numbers pending)

| Variant | Eval Perplexity | Training Minutes |
|---------|-----------------|------------------|
| A | [PENDING] | [PENDING] |
| B | [PENDING] | [PENDING] |
| C | [PENDING] | [PENDING] |

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

## Interpretation Framework

- **A < B and A < C**: preliminary support for IR token hypothesis
- **A ≈ B or A ≈ C**: no clear advantage at this scale or corpus size
- **A > B or A > C**: IR tokens hurt; investigate vocab size or structural information loss
- **DEC-006 note**: if B/C still outperform A after matched batch fix, the embedding cold-start confound (DEC-006) must be ruled out before drawing conclusions — this requires a separate 3-epoch A run or embedding warm-start test
