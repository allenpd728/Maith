# Phase 7 Roadmap: Path to Outperforming the AST Baseline

**Written:** 2026-08-03  
**Status:** Active — v1.3.0 retraining in progress  
**Baseline to beat:** Variant C (AST-split BPE), perplexity 1.1102, completion accuracy 91.4%

---

## Where We Stand

Six phases of A/B/C experiments plus DEC-021–024 have established:

| Finding | Status |
|---|---|
| IR trails BPE by ~0.17pp across Phase 5, Phase 6, DEC-021 | Confirmed |
| Gap is not due to cold-start embedding initialisation | Closed — DEC-021 |
| Gap is not due to sequence length (IR is 2x shorter than BPE) | Confirmed |
| Qwen pretraining prior is weaker than expected (25.8% vocab overlap) | Confirmed — QWEN_PRIOR_ANALYSIS.md |
| ~40–50% of v1.2.0 IR tokens were structural noise (polarity, IO markers) | Confirmed — IR_SCHEMA_AUDIT.md |
| 93% alpha-equivalence rate is accurate, not a defect | Confirmed — DEC-021 addendum |
| Fix 2 (polarity removal) regresses perplexity at 3.5k examples | Confirmed — DEC-022; neut = positional anchor |
| Fix 3 (IO marker simplification) improves perplexity to 1.2751 (new best) | Confirmed — DEC-023 |
| Flat-IR (shape only, 11 tokens) achieves PPL 1.0551 / 0.077 bits/tok | Confirmed — DEC-024 |
| Semantic content adds 0.27 bits/tok cost not recovered at current scale | Confirmed — DEC-024 |
| Whether more data or different objective would close this gap | Open question |

**Current state (2026-08-04):** Variant A v1.4.0 perplexity 1.2751 (best to date).
Flat-IR ablation complete. Next decision: IR pretraining, corpus expansion, or probing task experiment.

---

## What "Outperforming the AST" Requires

There are two targets. They are not the same:

**Variant B (raw BPE, 1.1295)** — easier bar. Beating B means structured tokenisation
of any kind outperforms raw text. This is a weaker claim: it confirms that *some*
structure helps but does not distinguish IR from simpler structural approaches.

**Variant C (AST-split BPE, 1.1102)** — the meaningful bar. C is also a structured
representation — it splits Lean at punctuation and keyword boundaries before BPE.
Beating C is evidence that *semantic* structure (entity/relation/operation graphs,
positional binder encoding, named constants) outperforms *syntactic* structure
(AST-shaped tokenisation). This is the hypothesis.

Current gap to C: ~0.17pp (1.2978 → 1.1102). Target: close it.

---

## Remaining Work, In Priority Order

### 1. v1.3.0 Retraining (In Progress)

**What it tests:** Whether polarity removal alone narrows the gap.  
**Expected outcome:** Perplexity toward 1.20, completion accuracy above 87%.  
**Decision gate:** If gap narrows to <0.10pp, proceed to IO marker simplification.
If gap is unchanged, the Qwen prior (item 4) becomes the priority.

---

### 2. IO Marker Simplification (Fix 3 — Next Encoder Change)

**What it is:** Replace `inputs:FVAR_0,FVAR_1` patterns with arity count tokens
(`ARITY_2`, `OUTPUT_3`). Collapses 7,014 unique IO marker tokens into ~10.  
**Why it matters:** IO markers are 13.5% of all tokens with 7,014 unique types —
the largest remaining vocabulary dilution source. Each unique IO marker pattern is
a token the model must learn to predict; many appear fewer than 5 times in training.
Collapsing to arity counts removes this dilution without losing structural information
(the model can still learn that a 2-input operation is different from a 3-input one).  
**Files to change:** `Maith/Encoder.lean`, `Maith/Decoder.lean`,
`python/build_dataset.py` (structural token list), `docs/ENCODER_FORMAT.md` (→ v1.4.0)  
**Decision gate:** Run after v1.3.0 results confirm direction. Single-variable: change
only IO markers, keep all other hyperparameters identical.

---

### 3. Type-Role Prefixes for Cross-Graph Identity

**What it is:** Add semantic role tokens (`ROLE:CARRIER`, `ROLE:INSTANCE`,
`ROLE:ARGUMENT`) to binder entity declarations in the IR.  
**Why it matters:** Currently `FVAR_0` in one graph has no relationship to `FVAR_0`
in another — the index is positional within a single graph only. The model cannot learn
"the first forall binder in a ring axiom usually represents the carrier type" because
that cross-graph semantic anchor doesn't exist. BPE doesn't have this problem:
`HMul.hMul` always means the same thing regardless of which sequence it appears in.  
**Implementation:** Derive role from binder scope string analysis in `MetaExtractor.lean`:
- Top-level `∀:` binders with type-level sort → `ROLE:CARRIER`
- `instImplicit` binders → `ROLE:INSTANCE`  
- Value-level `∀:` and `λ:` binders → `ROLE:ARGUMENT`  
**New vocab tokens:** ~5 (small addition).  
**Spec:** `docs/IR_V2_PROPOSAL.md` Section 3.

---

### 4. IR Pretraining Pass (If Gap Persists After Items 2–3)

**What it is:** Build a large unlabelled IR corpus from all of Mathlib (not just the
14 algebra modules) and run masked language modelling on it before fine-tuning.  
**Why it matters:** B/C benefit from Qwen's pretraining on enormous amounts of Lean
source text — the attention layers carry Lean-specific structure before any fine-tuning
begins. A's IR tokens have zero pretraining coverage: the attention layers start from
random weights on an unfamiliar token vocabulary. The embedding projection (DEC-021)
gave A warm embeddings but left the attention layers cold. An IR pretraining pass would
give A's attention layers IR-specific priors equivalent to what Qwen gives B/C.  
**Scale required:** Likely 50k–100k Mathlib declarations to be meaningful. Current
corpus is 2,554 declarations across 14 modules. Full Mathlib has ~100k declarations.  
**Prerequisite:** Fix 3 (IO markers) should be done first to ensure pretraining uses
the cleanest possible IR format.  
**Decision gate:** Pursue only if items 2–3 leave a gap of >0.05pp vs C.

---

### 5. Flat-IR Ablation — Variant D

**What it tests:** Is the graph structure itself contributing signal, or is the
head symbol + arity enough?  
**What it is:** Strip the E/R/O graph structure entirely. Encode only:
`FLAT_HEAD <gen:token> FLAT_ARITY <n> FLAT_DEPTH <n> FLAT_UNIV <level>`.  
Example: a 22-token IR sequence → 7 flat tokens.  
**Why it matters:** If flat-IR matches the full graph IR on perplexity, the graph
structure is adding tokens without adding signal and the design should be simplified.
If it's worse, the graph structure is carrying real information and cross-graph
identity (item 3) is the right direction.  
**Decision gate:** Run after v1.3.0 results are in. This is a design-validation
experiment, not a performance-optimisation experiment — its value is clarifying the
design, not necessarily improving numbers.  
**Spec:** `docs/IR_V2_PROPOSAL.md` Section 5.

---

### 6. Domain Generalisation — Topology and Order (DEC-019 Diversity Track)

**What it tests:** Does the IR transfer to mathematical domains outside its training
distribution?  
**Candidates:** `Mathlib.Topology.Basic`, `Mathlib.Order.Lattice`  
**Why it matters:** All current training is algebra. BPE generalises trivially because
it encodes surface text and mathematical identifiers transfer. If the IR fails on
topology while BPE handles it fine, the `gen:*` token coverage is too narrow and the
IR is over-fit to algebraic structure.  
**Prerequisite:** v1.3.0 results should be in hand. Domain generalisation is a
Phase 7 question, not a prerequisite for the performance experiments above.

---

## Decision Tree

```
v1.3.0 results
    │
    ├── perplexity ≤ 1.20 (gap narrowed significantly)
    │       → Proceed to Fix 3 (IO markers) as next single-variable run
    │       → Then type-role prefixes (item 3)
    │       → Hypothesis is viable; keep iterating
    │
    ├── perplexity 1.20–1.28 (gap unchanged or minimal improvement)
    │       → Run flat-IR ablation to determine if graph structure helps at all
    │       → If flat-IR ≈ full IR: simplify representation, revisit design
    │       → If flat-IR < full IR: graph structure is helping; pursue IR pretraining (item 4)
    │
    └── perplexity > 1.28 (regression)
            → Audit dataset rebuild and training config before proceeding
            → DEC-022 (2026-08-03): regression confirmed as real finding, not a bug
              neut tokens were functioning as positional anchors; removal cost 0.074pp
              New baseline: v1.3.0 + embed_project = 1.3717
              Fix 2 deferred until corpus > ~10k examples
            → Next: Phase 8b (Lean round-trip validation) then Fix 3 (IO markers)
```

---

## What Is Already Documented

| Topic | Location |
|---|---|
| IR token grammar and version history | `docs/ENCODER_FORMAT.md` |
| IR schema noise analysis | `docs/IR_SCHEMA_AUDIT.md` |
| Fix 1+2 implementation spec | `docs/IR_V2_FIX_SPEC.md` |
| Full v2 IR design proposal (IO markers, type-role prefixes, flat-IR) | `docs/IR_V2_PROPOSAL.md` |
| Normalisation audit findings | `docs/NORMALISATION_AUDIT.md` |
| Qwen pretraining prior analysis | `docs/QWEN_PRIOR_ANALYSIS.md` |
| Representation registration and evolution workflow | `docs/REPRESENTATION_EVOLUTION.md` |
| Full experimental history DEC-001 to DEC-024 | `docs/DECISION_LOG.md` |
| Phase 7 proof-term completion design (separate from this roadmap) | `docs/PHASE_7_DESIGN.md` |

---

## References

- DEC-020 (Phase 6 conclusion)
- DEC-021 (embedding projection — DEC-006 closed)
- DEC-022 (tokenizer study — IR 2x more efficient than BPE)
- docs/IR_V2_PROPOSAL.md
- docs/IR_V2_FIX_SPEC.md
- runs/variant_A_v1_3_0/ (in progress)
