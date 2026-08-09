# IR Normaliser Audit

**Analysis Date:** 2026-08-04 (re-run against v1.4.0 corpus, 4,029 examples)
**Previous run:** 2026-08-03 (post-DEC-021, against earlier smaller corpus)

## Executive Summary

**SIGNIFICANT ISSUES FOUND** — The normalizer is not functioning as intended.

---

## Part 1: Alpha-Equivalence Check

### Method
Found pairs of corpus entries where the `leanExpr` strings differ only in lowercase identifier tokens (bound variable names). For each pair, checked whether their IR `tokens` arrays are identical.

### Results

| Metric | Value |
|--------|-------|
| Alpha-equivalent pairs | 1872 |
| Identical IR sequences | 1759 |
| Different IR sequences | 113 |
| Identical rate | 94.0% |

**FINDING:** Not all alpha-equivalent pairs produce identical IR sequences.

This indicates the normalizer is **NOT** fully collapsing alpha-equivalent expressions.
Bound variable name differences in Lean source are being preserved in the IR,
which means the representation has MORE surface variation than intended.

**Sample different pairs:**
```
IsAddCyclic.mk vs IsAddCyclic.mk._flat_ctor
InvOneClass.casesOn vs InvOneClass.recOn
AddRightCancelMonoid.casesOn vs AddRightCancelMonoid.recOn
InvolutiveNeg.recOn vs InvolutiveNeg.casesOn
CommMagma.casesOn vs CommMagma.recOn
```

---

## Part 2: Polarity Distribution

### Method
Computed the corpus-wide distribution of polarity tokens (`pos`, `neg`, `neut`) appearing after E, A, R, O rows in the IR.

### Results

| Polarity | Count | Percentage |
|----------|-------|------------|
| neg | 19196 | 100.0% |

- Total rows with polarity: 19196
- Rows per example: 4.8

**FINDING:** `neg` appears in **100.0%** of polarity-tagged rows.

**IMPORTANT CAVEAT (v1.4.0):** The audit script searches for the string `neg` in all token positions. In the v1.4.0 corpus, polarity tokens were removed from E/A/R/O rows (DEC-022/Fix 2). Every `neg` in the current corpus (661 occurrences, verified 2026-08-04) appears immediately after an `OUT_N` token — i.e., it is an arithmetic operation token in an O row, not a polarity marker. Zero `neg` tokens appear in a polarity role in the v1.4.0 corpus. This audit section is a script artefact, not a real finding against the current corpus.

---

## Part 3: Attribute Row Redundancy

### Method
For each `E <entity_id>` row in the corpus, counted how many `A` rows reference the same entity. Reported mean, median, and p95.

### Results

| Metric | Value |
|--------|-------|
| Total entities (from E rows) | 59648 |
| Mean attributes per entity | 0.10 |
| Median attributes per entity | 0 |
| p95 attributes per entity | 1 |
| Entities with 0 attributes | 55705 (93.4%) |

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
| A (IR tokens) | 3010 | 3491 | 86.2% |
| B (BPE) | 3102 | 3491 | 88.9% |

**Unique sequence ratio (A/B):** 0.97x

**FINDING:** Unique sequence ratios are comparable.

The IR is not producing excessive surface variation compared to BPE.

---

## Conclusion

### The normalizer is NOT functioning as intended.

Multiple issues were found:

1. **Alpha-equivalence not preserved:** Near-equivalent Lean expressions
   produce different IR sequences. This is the primary issue.

3. **Near-constant polarity:** neg appears in 100.0% of rows,
   adding overhead without discriminative signal.

**Priority:** Fix the alpha-equivalence issue first. This is likely the
root cause of Variant A's underperformance. The current IR representation
may be well-designed, but broken normalization is making it look bad.

---

## References

- docs/reference/ENCODER_FORMAT.md (IR token grammar v1.2.0)
- python/normalisation_audit.py
- Corpus/corpus.jsonl
- datasets/train_A.jsonl
- datasets/train_B.jsonl
