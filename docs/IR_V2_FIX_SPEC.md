# IR v2 Fix Specification

**Date:** 2026-08-03  
**Status:** Ready for implementation  
**Scope:** `Maith/MetaExtractor.lean`, `Maith/Normalizer.lean`, `Maith/Polarity.lean`, `Maith/Encoder.lean`  
**Do not touch:** Training pipeline, dataset files, existing corpus

---

## Overview

Three targeted fixes to the current IR (`semantic_graph_ir_v1_2_0`) before any new
corpus build or training run. Each fix is independent and can be implemented and
validated in sequence. Together they address the two highest-priority findings from
the post-DEC-021 analysis:

1. **casesOn/recOn normalisation gap** — structurally identical declarations producing
   different IR token sequences (normalisation audit: 6.5% of alpha-equivalent pairs fail)
2. **Polarity token removal** — `neut` in 99.8% of rows, consuming 24% of all tokens
   with near-zero discriminative signal

A third fix (IO marker simplification) is specified but lower priority — it requires
a vocabulary-breaking change and should be validated after fixes 1 and 2 are confirmed.

---

## Fix 1: casesOn / recOn Normalisation Gap

### Problem

`MetaExtractor.lean` scopes binder entity IDs using the declaration name as a prefix:

```lean
let scopedName := s!"∀:{st.declName}/{depth}/{binderName}"
let binderId   := EntityId.bound scopedName
```

Lean generates several structurally identical but differently-named declarations for
the same type:

| Lean-generated name | Meaning |
|---|---|
| `Foo.casesOn` | Case analysis eliminator |
| `Foo.recOn` | Recursion eliminator (identical structure) |
| `Foo.mk._flat_ctor` | Flattened constructor alias for `Foo.mk` |

Because the scoped ID embeds `declName`, `AddCommSemigroup.recOn` and
`AddCommSemigroup.casesOn` produce different binder entity IDs (`∀:AddCommSemigroup.recOn/...`
vs `∀:AddCommSemigroup.casesOn/...`) even though their elaborated `Expr` types are
alpha-equivalent. After encoding, these become different FVAR token sequences.

### Fix: Canonical Declaration Name in Scope Strings

In `MetaExtractor.lean`, before constructing any scoped name, canonicalise the
declaration name by stripping the auto-generated suffix:

```lean
-- Add this helper near the top of the private section:
private def canonicaliseDeclName (name : String) : String :=
  -- Strip known Lean auto-generated suffixes that produce structurally identical decls.
  -- casesOn and recOn are alpha-equivalent eliminators; mk._flat_ctor is an alias for mk.
  let name := if name.endsWith ".casesOn" then
                name.dropRight ".casesOn".length ++ ".recOn"
              else name
  let name := if name.endsWith ".mk._flat_ctor" then
                name.dropRight "._flat_ctor".length
              else name
  name
```

Then apply it when initialising `ExtractionState.declName`. In `graphFromExpr`:

```lean
def graphFromExpr (declName : String) (expr : Lean.Expr) : ProcessingResult Graph :=
  let canonName := canonicaliseDeclName declName   -- ADD THIS LINE
  let initState : ExtractionState := { declName := canonName }   -- USE canonName
  ...
```

And in `extractGraphFromConstantInfo`, change:

```lean
let declName := info.name.toString
let initState : ExtractionState := { declName }
```

to:

```lean
let declName := canonicaliseDeclName info.name.toString
let initState : ExtractionState := { declName }
```

The `valueInitState` block further down also uses `declName` — it will inherit the
fix automatically since both use the same `declName` binding.

### Validation

After implementing, run the normalisation audit script against a freshly extracted
corpus and confirm the alpha-equivalence identical rate rises from 93.5% toward 100%:

```bash
python3 python/normalisation_audit.py
```

Also run the existing Lean tests to confirm no regressions:

```bash
lake test
```

---

## Fix 2: Remove Polarity Tokens

### Problem

Every `E`, `A`, `R`, `O` row emits a trailing polarity token (`pos`, `neg`, `neut`).
Corpus-wide, `neut` appears in 99.8% of rows. Polarity accounts for 24% of all IR
tokens with essentially no discriminating signal.

The `Normalizer.lean` polarity functions are no-ops for `neut` (they rewrite `neut`
to `neut`), confirming that polarity carries no information in the normalised graph.

### Fix: Three-file change

**File 1: `Maith/Polarity.lean`**

No change to the `Polarity` type itself — keep `pos`, `neg`, `neut` as valid values.
The type is used internally for graph construction and may be needed for future work.

**File 2: `Maith/Encoder.lean`**

Locate the token emission functions for each row type. Remove the polarity token from
the emitted sequence for all four row types (E, A, R, O).

The encoder currently emits something of the form:
```
"E" :: encodeEntityId id :: [encodePolarity polarity]
```

Change to:
```
"E" :: [encodeEntityId id]
```

Apply the same removal to A, R, and O row emission. The `Polarity` field on the
underlying structs is unchanged — only the encoder stops emitting it as a token.

If `pos` or `neg` genuinely need to be preserved for a specific row type, emit them
as a prefix modifier on the row token instead (`E_pos`, `R_neg`) rather than as a
trailing token. Given the 0.2% occurrence rate, this is an edge case.

**File 3: `docs/ENCODER_FORMAT.md`**

Update the token grammar to remove polarity from the E/A/R/O row format:

```
-- Before:
( E <entity_id> <polarity> )*
( A <entity_id> <key> <value> <polarity> )*
( R <entity_id> <entity_id> <rel_op> <polarity> )*
( O "inputs:<id,...>" "output:<id>" <op> <polarity> )*

-- After:
( E <entity_id> )*
( A <entity_id> <key> <value> )*
( R <entity_id> <entity_id> <rel_op> )*
( O "inputs:<id,...>" "output:<id>" <op> )*
```

Bump the encoder version from `1.2.0` to `1.3.0` and note this is a breaking change
(existing datasets built with v1.2.0 are incompatible with v1.3.0 encoder output).

### Validation

After implementing, rebuild the dataset and confirm:
1. Mean sequence length drops by approximately 20–25% (from ~226 to ~170–180 tokens)
2. The `neut`/`pos`/`neg` tokens no longer appear in `datasets/train_A.jsonl`
3. `lake test` passes

```bash
python3 python/build_dataset.py --corpus Corpus/corpus.jsonl --out datasets/
python3 python/tokenizer_study.py --corpus Corpus/corpus.jsonl --sample 500
```

---

## Fix 3: IO Marker Simplification (Lower Priority)

### Problem

The `inputs:FVAR_0,FVAR_1` format in O rows generates 7,014 unique token types for
what are structurally simple positional patterns. `inputs:FVAR_0` alone appears 4,943
times. This is a vocabulary explosion that dilutes the signal from `gen:*` tokens.

### Fix: Arity count tokens

Replace the `inputs:<id,...>` string with a pair of fixed-format tokens:

```
-- Before:
O inputs:FVAR_0,FVAR_1 output:TERM_2 gen:Semigroup.toMul

-- After:
O ARITY_2 OUTPUT_2 gen:Semigroup.toMul
```

Where:
- `ARITY_N` — number of inputs (0–9; `ARITY_MANY` for 10+)
- `OUTPUT_N` — positional index of the output term (0–63; `OUTPUT_MANY` for 64+)

The specific input and output entity IDs are already declared in the preceding E rows.
The O row tells the model *which operation* is applied and *how many* inputs it takes —
the exact input/output IDs are recoverable from context.

**Why lower priority:** This is a vocabulary-breaking change that requires updates to
both `Encoder.lean` and the Python vocab builder in `build_dataset.py`. It should be
validated in a separate corpus rebuild after fixes 1 and 2 are confirmed stable, so
that each change's effect on perplexity and completion accuracy can be measured
independently.

### Files to change when ready
- `Maith/Encoder.lean` — change O row emission format
- `Maith/Decoder.lean` — update decoder to match
- `python/build_dataset.py` — update `build_ir_vocab` structural token list
- `docs/ENCODER_FORMAT.md` — bump to version `1.4.0`

---

## Implementation Sequence

Do these in order. Validate each before starting the next.

1. **Fix 1 (casesOn/recOn)** — `MetaExtractor.lean` only, no vocab change, re-extract corpus, run normalisation audit
2. **Fix 2 (polarity removal)** — `Encoder.lean` + `ENCODER_FORMAT.md`, rebuild dataset, confirm sequence length drop, retrain Variant A
3. **Fix 3 (IO markers)** — only after Fix 2 training results are in hand

After Fix 2 retraining, compare the new Variant A perplexity and completion accuracy
against the Phase 6 baseline (1.2792 / 84.2%). If the gap to B/C has narrowed, the
noise reduction is working and Fix 3 is worth implementing. If the gap is unchanged,
the problem lies elsewhere and Fix 3 should be deprioritised.

---

## What Success Looks Like

| Metric | Current (v1.2.0) | Target after Fix 1+2 |
|--------|-----------------|----------------------|
| Alpha-equiv normalisation rate | 93.5% | >99% |
| Mean sequence length | 226 tokens | ~170–180 tokens |
| Polarity token share | 24% | 0% |
| Variant A perplexity | 1.2978 | < 1.25 (meaningful improvement) |
| Completion accuracy | 84.2% | > 87% |

If Fix 1+2 bring Variant A to within 0.10 perplexity points of B/C (currently ~0.17
gap), the hypothesis is viable and Fix 3 plus the type-role prefix additions from
IR_V2_PROPOSAL.md become the next design iteration.

---

## References
- `Maith/MetaExtractor.lean` — binder scoping logic (Fix 1)
- `Maith/Normalizer.lean` — polarity normalisation (confirms Fix 2 is safe)
- `Maith/Encoder.lean` — token emission (Fix 2 and 3)
- `docs/ENCODER_FORMAT.md` — token grammar spec
- `docs/NORMALISATION_AUDIT.md` — audit findings (casesOn/recOn gap, polarity distribution)
- `docs/IR_SCHEMA_AUDIT.md` — schema noise analysis (IO marker token explosion)
- `docs/IR_V2_PROPOSAL.md` — broader v2 design proposal
- DEC-021, DEC-022 — experimental baselines this fix targets
