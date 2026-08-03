# Phase 8: Bidirectional Pipeline Validation

**Created:** 2026-08-03  
**Status:** 🟡 In Progress — Phase 8a underway  
**Goal:** Confirm at every stage that no information is silently lost before making any further IR changes (Fix 3 IO markers, Fix 4 type-role prefixes).

Fix 3 does not happen until Phases 8a and 8b pass. Fix 4 does not happen until all four phases pass.

---

## Why This Matters

`validate_roundtrip.py` reports 200/200 pass, but that result is narrower than it sounds:

- It tests **Python decoder ↔ Python encoder only** — a self-consistency check, not a pipeline check
- It was run against the **v1.2.0 corpus** (4-module, 2,554 examples with polarity tokens). v1.3.0 format removed polarity tokens and the script has not been updated
- It has **no test of `MetaExtractor.lean`** — the extraction step where information could be silently lost at the Lean `Expr` level
- The **decompiler (`Maith/Transpiler.lean`) is broken** — it produces structural output that does not parse as valid Lean (known failures: `Eq` emission malformed, universe syntax invalid, `∀` keyword missing)

The full pipeline has two directions that must both be validated:

```
Lean Expr
    │  MetaExtractor.lean (forward)         ← Phase 8c tests this
    ▼
IR Graph
    │  Encoder.lean (forward)               ← Phase 8b tests this (Lean-native)
    ▼
Token sequence
    │  Decoder.lean (reverse)               ← Phase 8b tests this (Lean-native)
    ▼
IR Graph
    │  Transpiler.lean (reverse)            ← Phase 8d fixes and tests this
    ▼
Lean syntax (parseable)
```

Currently only the middle two steps (Encoder ↔ Decoder) are tested, and only via a Python reimplementation.

---

## Phase 8a — Fix Python round-trip for v1.3.0

**Owner:** Kit  
**Status:** 🔴 Not started  
**Estimated effort:** 2–3 hours  
**Blocked by:** Nothing — start immediately

### What's broken

`validate_roundtrip.py` expects polarity tokens (`neut`) after every E/A/R/O row (v1.2.0 format). v1.3.0 removed polarity tokens. The script misparsed the v1.3.0 corpus silently — the 200/200 pass result was against the old 4-module v1.2.0 `corpus.jsonl`.

Specific broken lines:
- `decode_tokens()` reads `tokens[i + 2]` as polarity for E rows (line ~98)
- `reencode_graph()` appends polarity to every row (lines 143–151)
- No encoder version detection — script assumes v1.2.0 unconditionally

### Changes required

1. Update `decode_tokens()` to handle v1.3.0 format (no polarity after E/A/R/O rows)
2. Update `reencode_graph()` to emit v1.3.0 format (no polarity tokens)
3. Add encoder version detection — read `encoderVersion` from corpus or manifest; branch for v1.2.0 vs v1.3.0 so historical corpora still validate
4. Increase default sample from 200 to all examples (`--all` as default)

### Pass criterion

✅ 3,901/3,901 examples round-trip on the full v1.3.0 corpus  
✅ Zero polarity tokens in re-encoded output  
✅ v1.2.0 corpus (corpus.jsonl.bak) still passes under v1.2.0 mode

### Files to change

- `python/validate_roundtrip.py`

---

## Phase 8b — Lean-native Encoder ↔ Decoder round-trip

**Owner:** OpenHands  
**Status:** 🔴 Not started  
**Estimated effort:** 1–2 days  
**Blocked by:** Phase 8a complete

### What's missing

The Python decoder in `validate_roundtrip.py` is a manual reimplementation of `Decoder.lean`. If they diverge, the Python validator passes even when the real Lean decoder is wrong. There is no test that runs the actual Lean decoder against real corpus examples.

### What to build

A Lean test harness `Tests/RoundTripTests.lean` that for each test example:
1. Deserialises the stored token sequence using `Decoder.lean`
2. Re-encodes using `Encoder.lean`
3. Compares token-for-token to the original stored sequence

Test coverage requirements:
- Minimum 500 examples from `Corpus/corpus.jsonl`
- Must cover all row types: E rows, A rows (typeclass + sort + literal), R rows (eq), O rows (various arities)
- Must cover both FVAR (forall) and BVAR (lambda) binder kinds
- Must include at least 10 examples with IO arity ≥ 3

Any mismatch must report: example name, token position, expected token, actual token, surrounding context (5 tokens either side).

### Pass criterion

✅ 500/500 Lean-native round-trips pass  
✅ No mismatch between `Encoder.lean` output and stored corpus tokens  
✅ Test suite runnable via `lake test`

### Files to create/change

- `Tests/RoundTripTests.lean` (new)
- `Maith.lean` (add RoundTripTests import if needed)

### Do not touch

- `Maith/MetaExtractor.lean`
- `Maith/Encoder.lean` (read-only unless a bug is found; if a bug is found, document it and fix it — do not silently work around it)
- `Corpus/corpus.jsonl`

---

## Phase 8c — IR graph faithfulness: Expr extraction preserves semantic content

**Owner:** OpenHands  
**Status:** 🔴 Not started  
**Estimated effort:** 2–3 days  
**Blocked by:** Phase 8b complete

### What's missing

No test confirms that `MetaExtractor.lean` captures all semantically relevant information from the Lean `Expr`. Silent losses here — dropped universe levels, collapsed implicit arguments, missed binder annotations — would not be caught by any current test.

### What to build

A Lean test `Tests/ExtractionFaithfulnessTests.lean` that:

1. Extracts a set of golden declarations using `MetaExtractor.lean`
2. For each, verifies the extracted IR graph against a fully-specified expected structure

Golden declarations to cover (chosen for known structural properties):

| Declaration | Key property to verify |
|---|---|
| `AddZero.mk` | FVAR entities with typeclass A rows; R rows capture `[Zero M] [Add M]` constraints |
| `AddZero.mk._flat_ctor` | BVAR entities with explicit lambda args; structurally different from `AddZero.mk` |
| `mul_assoc` | 3-input O row; both FVAR and TERM entities present |
| `one_mul` | Simple 2-input O row; single typeclass constraint |
| `AddCancelMonoid.ctorIdx` | Literal A row (`Lean.Literal.natVal 0`); sort A row |

For each golden declaration verify:
- Entity count matches expected (± known TERM_MANY overflow exceptions)
- All expected `gen:` tokens present in O rows
- R rows capture all type equalities (count and op type)
- A rows capture all typeclass constraints and sort levels
- `casesOn` and `recOn` variants of the same type produce **different** IR graphs (confirms the extractor is faithfully representing the Lean-level distinction established in DEC-021)
- `mk` and `mk._flat_ctor` variants produce **different** IR graphs (same reason)

### Pass criterion

✅ All golden declarations extract with expected graph structure  
✅ `casesOn` vs `recOn` graphs confirmed structurally distinct  
✅ `mk` vs `mk._flat_ctor` graphs confirmed structurally distinct  
✅ Any deviation from expected structure is documented as either (a) known acceptable simplification with justification, or (b) a bug to fix

### Files to create/change

- `Tests/ExtractionFaithfulnessTests.lean` (new)
- If extraction bugs found: `Maith/MetaExtractor.lean` (only with explicit justification per finding)

---

## Phase 8d — Fix decompiler to produce parseable Lean output

**Owner:** OpenHands  
**Status:** 🔴 Not started  
**Estimated effort:** 1 week  
**Blocked by:** Phase 8b complete (can run in parallel with 8c)

### Why this matters

A working decompiler is the only way to confirm the IR is **complete** — that the original Lean expression could be reconstructed from the graph alone. Without it, passing round-trip tests only confirm self-consistency, not completeness. If the extractor drops information, the encoder/decoder round-trip still passes (the dropped information isn't there to fail on).

### Known failures (from `docs/DECOMPILER_HANDOVER.md`)

1. **`Eq` emission is malformed** — `Eq.G` is not valid Lean syntax
2. **Universe level syntax is invalid** — current output doesn't match Lean's `Type.{u}` / `Sort.{u+1}` forms
3. **`forall` binders emitted without `∀` keyword** — produces unparseable output

### What to build

1. Fix the three known failures in `Maith/Transpiler.lean`
2. Add `Tests/DecompilerTests.lean` that runs `decompileGraph` on 10 golden examples and verifies the output parses in Lean without syntax errors
3. The bar is **parseable** — Lean can read the output without syntax errors. Full re-elaboration (type-checking) is not required for Phase 8d.

Golden examples for decompiler testing: same set as Phase 8c (`AddZero.mk`, `mul_assoc`, `one_mul`, `AddCancelMonoid.ctorIdx` — simple enough to decompile, complex enough to exercise all row types).

### Pass criterion

✅ 10/10 golden declarations decompile to output that parses in Lean without syntax errors  
✅ Test runnable via `lake test`  
✅ `DECOMPILER_HANDOVER.md` updated to reflect resolved vs remaining issues

### Files to change

- `Maith/Transpiler.lean`
- `Tests/DecompilerTests.lean` (new or extend existing)
- `docs/DECOMPILER_HANDOVER.md` (update status)

---

## Sequencing and gates

```
Phase 8a  Python round-trip v1.3.0          Kit, ~2-3h      🔴 Not started
    │
    ▼ (8a must pass before proceeding)
Phase 8b  Lean Encoder ↔ Decoder            OpenHands #1    🔴 Not started
    │
    ├──▶ Fix 3: IO marker simplification    Kit             🔴 Blocked on 8a+8b
    │        (can run in parallel with 8c/8d once 8a+8b pass)
    │
    ├──▶ Phase 8c  Expr extraction faithfulness  OpenHands #2    🔴 Blocked on 8b
    │
    └──▶ Phase 8d  Decompiler fix               OpenHands #3    🔴 Blocked on 8b
             │
             ▼
         Fix 4: Type-role prefixes         Kit             🔴 Blocked on 8d
```

**Fix 3 gate:** Phases 8a + 8b  
**Fix 4 gate:** All four phases complete

---

## Progress log

| Date | Phase | Action | Result |
|---|---|---|---|
| 2026-08-03 | — | Plan written | This document |

---

## References

- `docs/DECOMPILER_HANDOVER.md` — known decompiler failures and handoff notes
- `docs/PHASE_7_ROADMAP.md` — IR improvement roadmap (Fix 3, Fix 4, flat-IR ablation)
- `docs/ENCODER_FORMAT.md` — IR token grammar v1.3.0
- `python/validate_roundtrip.py` — Python round-trip validator (currently v1.2.0 only)
- `Maith/Encoder.lean` — token encoder
- `Maith/Decoder.lean` — token decoder (if it exists) / decoder spec
- `Maith/Transpiler.lean` — graph→Lean decompiler (broken, Phase 8d target)
- `Tests/` — existing Lean test suite
