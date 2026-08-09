# IR Schema Audit: semantic_graph_ir_v1_2_0

**Date:** 2026-08-03  
**Reference:** DEC-021 (post-Phase 6 analysis)  
**Author:** IR Candidate Analysis

> **v2 update (2026-08-09):** This audit was written for v1.2.0. The v2 IR (v2.0.0) changed three things audited here: polarity tokens are removed (C1), gen:FullName tokens are bucketed to GEN_<area> (C4), and typeclass attributes use short names (C2). The audit's signal/noise findings remain valid as the rationale for those changes. See [`ENCODER_FORMAT.md`](ENCODER_FORMAT.md) v2.0.0 for the current token format.

This document audits each token type in the v1.2.0 IR schema (`semantic_graph_ir_v1_2_0`) to determine whether it carries predictive signal or functions as noise.

---

## Token Type Summary

| Token Type | Count (train) | % of Total | Unique Types | Signal Assessment |
|------------|---------------|-------------|--------------|-------------------|
| polarity | 187,362 | 24.57% | 2 | ⚠ NEAR-CONSTANT |
| structural_row_type | 186,914 | 24.51% | 4 | ✓ REQUIRED |
| io_marker | 102,954 | 13.50% | 7,014 | ⚠ HIGH NOISE |
| term_ref | 89,897 | 11.79% | 65 | ✓ STRONG SIGNAL |
| fvar_binder | 57,451 | 7.53% | 49 | ✓ STRONG SIGNAL |
| gen_named_constant | 44,679 | 5.86% | 628 | ✓ STRONG SIGNAL |
| rel_op | 36,776 | 4.82% | 9 | ✓ STRONG SIGNAL |
| bvar_binder | 17,832 | 2.34% | 38 | ⚠ CONTEXT-DEPENDENT |
| other | 16,407 | 2.15% | 280 | ⚠ MIXED |
| attribute_key | 13,647 | 1.79% | 3 | ✓ STRONG SIGNAL |
| structural_graph | 6,750 | 0.89% | 2 | ✓ REQUIRED |
| gen_unk | 1,953 | 0.26% | 1 | ⚠ INFORMATION LOSS |

---

## Detailed Token Analysis

### 1. `GRAPH_BEGIN` / `GRAPH_END`

**Description:** Graph boundary markers.  
**Completion Decision:** Marks sequence start/end; informs the model when to attend to graph structure.  
**Evidence of Signal:** Required for structural parsing. Always present at positions 0 and N-1 of every sequence.  
**Distribution:** Uniform across corpus (appears in 100% of examples).  
**Verdict:** ✓ REQUIRED — Serves as structural framing, not discriminative signal but necessary for parsing.

---

### 2. `E` (Entity Row)

**Description:** Declares an entity with ID and polarity.  
**Completion Decision:** Informs the model about the vocabulary of entities in the graph.  
**Evidence of Signal:** Appears in 100% of sequences (every entity must be declared).  
**Distribution:** Count varies with graph size; position within sequence is informative.  
**Verdict:** ✓ REQUIRED — Establishes entity vocabulary for subsequent relation/operation rows.

---

### 3. `A` (Attribute Row)

**Description:** Assigns attributes to entities (typeclass, sort, literal).  
**Completion Decision:** Informs type information for entities; typeclass attributes distinguish class instances.  
**Evidence of Signal:** Appears in 81% of examples (3,332 of 3,375 train).  
**Distribution:** Three variants: `typeclass`, `sort`, `literal`. `typeclass` appears most frequently (4,731 times).  
**Verdict:** ✓ STRONG SIGNAL — Type information is essential for semantic completion. However, see concern below.

**⚠ Schema Concern:** `A` rows appear to duplicate information already available in the `O` operation tokens (which include the head symbol). For example:
```
A BVAR_1 typeclass CommMagma neut
O inputs:BVAR_0,BVAR_1 output:TERM_3 gen:CommMagma.toMul neut
```
The typeclass attribute on `BVAR_1` could be inferred from the operation that consumes it. This redundancy may inflate sequence length without adding discriminative signal.

---

### 4. `R` (Relation Row)

**Description:** Declares a relation between two entities (typically `eq` for type annotations).  
**Completion Decision:** Informs structural relationships between entities.  
**Evidence of Signal:** Appears in 99% of examples (3,347 of 3,375 train).  
**Distribution:** `eq` is dominant (30,411 of 36,776 = 83%). Other relations (lt, le, gt, ge) are rare.  
**Verdict:** ✓ STRONG SIGNAL — Relations establish entity connections. However, the near-uniformity of `eq` suggests most `R` rows are boilerplate type annotations.

**⚠ Schema Concern:** If 83% of `R` rows are `eq` relations on type annotations, they may be considered structural noise. The model could learn type structure from `A` rows alone.

---

### 5. `O` (Operation Row)

**Description:** Declares an operation with inputs, output, operator, and polarity.  
**Completion Decision:** **Primary predictive signal.** Head symbol prediction is the main completion task.  
**Evidence of Signal:** Appears in 99% of examples.  
**Distribution:** Variable; the `gen:*` operator tokens carry the most semantic content.  
**Verdict:** ✓ STRONG SIGNAL — This is the core of the IR; the model must predict operation tokens.

**⚠ Schema Concern:** The `inputs:FVAR_0,FVAR_1` format encodes positional references as strings, creating 7,014 unique `io_marker` tokens. Many of these are simple positional patterns (e.g., `inputs:FVAR_0` appears 4,943 times) that could be more efficiently encoded.

---

### 6. Polarity: `pos`, `neg`, `neut`

**Description:** Polarity of entity/relation/operation in the graph.  
**Completion Decision:** Informs about positive/negative occurrences of subexpressions.  
**Evidence of Signal:**  
- `neut`: 186,914 (99.7% of all polarity tokens)  
- `pos`: ~400  
- `neg`: ~48  
**Distribution:** ⚠ NEAR-CONSTANT — `neut` dominates to an extreme degree.  
**Verdict:** ⚠ NEAR-CONSTANT — Polarity carries almost no discriminative information. The few `pos`/`neg` tokens may be noise from edge cases.

**Recommendation:** Polarity tokens inflate sequence length by ~24% without meaningful signal. Consider:
1. Removing polarity entirely (set to `neut` always)
2. Collapsing polarity into the row type (`E_neut`, `E_pos`, `E_neg`)

---

### 7. `FVAR_N` (Forall Binder Variables)

**Description:** Positional references to forall-bound variables.  
**Completion Decision:** Informs the model about binder scope and argument positions.  
**Evidence of Signal:** 49 unique tokens (FVAR_0 through FVAR_48+), appears in 99% of examples.  
**Distribution:** FVAR_0 dominates (13,981 occurrences), declining with higher indices.  
**Verdict:** ✓ STRONG SIGNAL — Binder variables are essential for completion; the model must know which variable is being referenced.

**⚠ Cross-Graph Identity Problem:** Per ENCODER_FORMAT.md, `FVAR_0` in one graph has no relationship to `FVAR_0` in another. This is a known limitation (DEC-021 context).

---

### 8. `BVAR_N` (Lambda Binder Variables)

**Description:** Positional references to lambda-bound variables.  
**Completion Decision:** Same as FVAR_N but for lambda scope.  
**Evidence of Signal:** 38 unique tokens, appears in 39% of examples (1,322 of 3,375).  
**Distribution:** Much less frequent than FVAR tokens; mostly in proof terms.  
**Verdict:** ⚠ CONTEXT-DEPENDENT — Signal is strong when present, but absent in most statement-only declarations.

---

### 9. `TERM_N` (Term References)

**Description:** Positional references to intermediate terms in the expression.  
**Completion Decision:** Informs about term dependencies and argument ordering.  
**Evidence of Signal:** 65 unique tokens (TERM_0 through TERM_63+), appears in 99.9% of examples.  
**Distribution:** Higher frequency than binder variables; terms are the primary output of operations.  
**Verdict:** ✓ STRONG SIGNAL — Terms are the primary building blocks of completions.

---

### 10. `gen:*` Named Constants

**Description:** Qualified names for operations, types, and constants.  
**Completion Decision:** **Core semantic content.** The model predicts these to complete expressions.  
**Evidence of Signal:** 628 unique tokens, appears in 98% of examples.  
**Distribution:** Heavy-tailed; `gen:hof` (4,123), `gen:OfNat.ofNat` (1,739), `gen:Set` (642) dominate.  
**Verdict:** ✓ STRONG SIGNAL — This is the primary content the model must predict.

**⚠ GEN_UNK Issue:** 1,953 occurrences of `GEN_UNK` indicate rare tokens being collapsed. This represents information loss.

---

### 11. `GEN_UNK`

**Description:** Placeholder for named constants with frequency < 5 in training data.  
**Completion Decision:** Reduces vocabulary size at cost of semantic granularity.  
**Evidence of Signal:** 1,953 occurrences (~0.26% of tokens).  
**Distribution:** Collapses hundreds of rare operations into a single token.  
**Verdict:** ⚠ INFORMATION LOSS — Acceptable for rare tokens, but each `GEN_UNK` is a prediction the model cannot make accurately.

---

### 12. `typeclass`, `sort`, `literal` (Attribute Keys)

**Description:** Keys for attribute rows.  
**Completion Decision:** Informs the type of attribute being assigned.  
**Evidence of Signal:** 3 unique tokens, 13,647 total occurrences.  
**Distribution:** `sort` (6,353), `typeclass` (4,731), `literal` (2,563).  
**Verdict:** ✓ STRONG SIGNAL — These are semantically meaningful and help disambiguate entity roles.

---

### 13. Relational Operators: `eq`, `lt`, `le`, `gt`, `ge`, etc.

**Description:** Relation types for `R` rows.  
**Completion Decision:** Informs relational structure between entities.  
**Evidence of Signal:** 9 unique tokens, 36,776 occurrences.  
**Distribution:** `eq` dominates (83%), others are rare.  
**Verdict:** ✓ STRONG SIGNAL when non-`eq`; ⚠ BOILERPLATE when `eq` (type annotation).

---

## Noise Summary

Tokens identified as likely noise:

| Token Type | Issue | Est. Waste |
|-----------|-------|------------|
| `neut` polarity | 99.7% constant, inflates seq by ~24% | 186K tokens |
| `eq` relation | 83% of R rows are type annotations | 30K tokens |
| `A` rows | May duplicate O-row information | ~14K tokens |
| `io_marker` specificity | 7K unique tokens for simple patterns | ~50K tokens |
| `GEN_UNK` | Information loss on rare ops | 2K tokens |

**Total estimated noise:** ~40-50% of sequence length

---

## Recommendations

1. **Remove polarity tokens** — Replace with implicit `neut`; encode only `pos`/`neg` when present.

2. **Collapse `eq` relations** — Merge type annotation relations into entity declarations to eliminate the most common R-row pattern.

3. **Reduce `io_marker` granularity** — Instead of `inputs:FVAR_0,FVAR_1`, use a positional count: `I:2` (2 inputs). This collapses 7,014 tokens into ~10.

4. **Evaluate A-row redundancy** — Test whether removing A rows degrades completion accuracy. If not, remove them.

5. **Address cross-graph identity** — Implement type-role tokens or canonical naming to give the model semantic anchors across graphs.

---

## Conclusion

The current IR schema contains significant noise (~40-50% of sequence length) that does not contribute to predictive signal. The core signal is in:
- `O` rows (operations)
- `gen:*` tokens (named constants)
- `FVAR_N`/`BVAR_N`/`TERM_N` (positional references, to a degree)

The remaining tokens serve structural purposes but inflate sequence length without proportional discriminative power. A streamlined IR (proposed in `IR_V2_PROPOSAL.md`) could reduce sequence length by ~40% while preserving or improving completion quality.
