# Qwen Pretraining Prior Analysis

**Analysis Date:** 2026-08-03 (post-DEC-021)

## Executive Summary

**NOT CREDIBLE** — The Qwen pretraining prior explanation is weak based on the available evidence.

## Part 1: Vocabulary Overlap Check

### Method
For each `gen:<Name>` token in the IR vocabulary, we checked whether the token string or its components appear in Qwen's BPE tokenizer vocabulary.

### Results

| Metric | Value |
|--------|-------|
| Total gen:* tokens in IR vocab | 628 |
| Exact matches (token == Qwen vocab) | 0 |
| Partial matches (content in Qwen vocab) | 162 |
| No matches | 466 |
| Direct match rate | 0.0% |
| Any match rate | 25.8% |

### Interpretation

Only **25.8%** of gen:* tokens have matches in Qwen's vocabulary. The vocabulary overlap is weaker than expected.

**Sample unmatched tokens (first 10):**
```
gen:OfNat.ofNat
gen:PartialOrder.toPreorder
gen:Subgroup
gen:AddMonoid.toAddZeroClass
gen:instOfNatNat
gen:DivInvMonoid.toMonoid
gen:HEq
gen:Iff
gen:SubNegMonoid.toAddMonoid
gen:AddSubgroup
```

---

## Part 2: Lean Token Frequency in Qwen Pretraining (Proxy)

### Method
Computed the tokens-per-character ratio for 100 Lean expressions sampled from the corpus, compared to a 200-word English reference paragraph. A lower ratio for Lean indicates Qwen has built efficient multi-character merges for Lean syntax during pretraining.

### Results

| Metric | Lean Source | English Reference |
|--------|-------------|-------------------|
| Mean tokens/char | 0.4505 | 0.1708 |
| Median tokens/char | 0.4593 | 0.1708 |
| Min | 0.1818 | 0.1708 |
| Max | 0.5527 | 0.1708 |

### Interpretation

Lean source is **163.7%** less token-efficient than English. Lean is being treated as unknown text; the pretraining prior is **weaker than expected**.

---

## Part 3: Per-Layer Gradient Norm Proxy

**SKIPPED:** No checkpoint path provided

This analysis requires:
- A GPU or MPS device
- The Variant B checkpoint (`runs/variant_B_phase6/checkpoint-final`)
- The eval dataset (`datasets/eval_B.jsonl`)

---

## Conclusion

The Qwen pretraining prior explanation is **NOT CREDIBLE** based on current evidence.

Other explanations for Variant A's underperformance should be investigated, particularly the normalizer hypothesis (see `docs/NORMALISATION_AUDIT.md`).

---

## References

- DECISION_LOG.md (DEC-006 through DEC-021)
- python/qwen_prior_analysis.py
- datasets/vocab_A.json
- Corpus/corpus.jsonl
