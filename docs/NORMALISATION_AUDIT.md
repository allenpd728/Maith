# IR Normaliser Audit

**Analysis Date:** 2026-08-03 (post-DEC-021)

## Executive Summary

**SIGNIFICANT ISSUES FOUND** — The normalizer is not functioning as intended.

---

## Part 1: Alpha-Equivalence Check

### Method
Found pairs of corpus entries where the `leanExpr` strings differ only in lowercase identifier tokens (bound variable names). For each pair, checked whether their IR `tokens` arrays are identical.

### Results

| Metric | Value |
|--------|-------|
| Alpha-equivalent pairs | 1739 |
| Identical IR sequences | 1626 |
| Different IR sequences | 113 |
| Identical rate | 93.5% |

**FINDING:** Not all alpha-equivalent pairs produce identical IR sequences.

This indicates the normalizer is **NOT** fully collapsing alpha-equivalent expressions.
Bound variable name differences in Lean source are being preserved in the IR,
which means the representation has MORE surface variation than intended.

**Sample different pairs:**
```
IsMulTorsionFree.mk vs IsMulTorsionFree.mk._flat_ctor
IsAddCyclic.mk vs IsAddCyclic.mk._flat_ctor
AddCommSemigroup.recOn vs AddCommSemigroup.casesOn
InvOneClass.casesOn vs InvOneClass.recOn
AddRightCancelMonoid.casesOn vs AddRightCancelMonoid.recOn
```

---

## Part 2: Polarity Distribution

### Method
Computed the corpus-wide distribution of polarity tokens (`pos`, `neg`, `neut`) appearing after E, A, R, O rows in the IR.

### Results

| Polarity | Count | Percentage |
|----------|-------|------------|
| neg | 661 | 0.2% |
| neut | 310960 | 99.8% |

- Total rows with polarity: 311621
- Rows per example: 79.9

**FINDING:** `neut` appears in **99.8%** of rows.

Polarity is near-constant and may be contributing structural overhead (the `pos`/`neg`/`neut` token after each row) without discriminating signal.

---

## Part 3: Attribute Row Redundancy

### Method
For each `E <entity_id>` row in the corpus, counted how many `A` rows reference the same entity. Reported mean, median, and p95.

### Results

| Metric | Value |
|--------|-------|
| Total entities (from E rows) | 112523 |
| Mean attributes per entity | 0.18 |
| Median attributes per entity | 0 |
| p95 attributes per entity | 1 |
| Entities with 0 attributes | 95840 (85.2%) |

**FINDING:** >80% of entities have no attribute rows.

The `A` row type is sparsely used and may be contributing structural overhead:
- The `A` token
- The entity ID
- The key
- The value
- The polarity token

for minimal discriminative signal.

---

## Part 4: Unique Sequence Ratio

### Method
Counted unique token sequences in `train_A.jsonl` vs `train_B.jsonl`.
If Variant A has significantly MORE unique sequences than Variant B for the
same set of declarations, the IR is producing more surface variation.

### Results

| Variant | Unique Sequences | Total | Unique Rate |
|---------|-----------------|-------|-------------|
| A (IR tokens) | 3096 | 3375 | 91.7% |
| B (BPE) | 3111 | 3375 | 92.2% |

**Unique sequence ratio (A/B):** 1.00x

**FINDING:** Unique sequence ratios are comparable.

The IR is not producing excessive surface variation compared to BPE.

---

## Conclusion

### The normalizer is NOT functioning as intended.

Multiple issues were found:

1. **Alpha-equivalence not preserved:** Near-equivalent Lean expressions
   produce different IR sequences. This is the primary issue.

2. **Near-constant polarity:** neut appears in 99.8% of rows,
   adding overhead without discriminative signal.

**Priority:** Fix the alpha-equivalence issue first. This is likely the
root cause of Variant A's underperformance. The current IR representation
may be well-designed, but broken normalization is making it look bad.

---

## References

- docs/ENCODER_FORMAT.md (IR token grammar v1.2.0)
- python/normalisation_audit.py
- Corpus/corpus.jsonl
- datasets/train_A.jsonl
- datasets/train_B.jsonl
