# Phase 7 Design: Proof-Term Completion

**Status:** Future direction. Not started. Do not touch MetaExtractor.lean, the tactic density
filter, or any training pipeline until Phase 5/6 are complete and this design is revisited.

**Logged:** 2026-07-31  
**Source:** Analysis during Phase 5 corpus expansion planning.

---

## Motivation

The ultimate test of the Maith hypothesis is whether an IR-trained model can assist with
formal proof work — not just predict statement structure, but generate valid proof terms
given a theorem statement. This document records the design analysis for that capability
as a fully separate future phase.

---

## Why Next-Tactic Prediction Is Ruled Out

Next-tactic prediction is the approach used by tools like LeanDojo and COPRA: given a
tactic proof state, predict the next tactic string. It is explicitly ruled out for Maith
for the following reason.

Tactics live in the `Syntax` layer — the surface language the user writes. Lean's
elaborator consumes `Syntax` and produces elaborated `Expr` terms. The current IR
extracts from elaborated `Expr` only. By the time `MetaExtractor.lean` sees a declaration,
the tactic syntax is gone: `by ring`, `by simp [h]`, `by exact Finset.sum_comm` have all
been elaborated into their proof-term equivalents (`Ring.proof_1`, a simp normal-form term,
an application of `Finset.sum_comm`). There is no tactic string to recover.

Supporting next-tactic prediction would require a completely different extraction path:
intercepting the elaborator before it discards `Syntax`, capturing tactic proof states
mid-elaboration, and building a separate training schema around tactic→tactic transitions.
That is a different project, not an extension of this one.

**This decision is final absent a specific new reason to reopen it.** The Syntax-layer
gap is not a solvable configuration choice — it is architectural.

---

## The Right Path: Proof-Term Completion

Proof-term completion is tractable within the current IR architecture. The training task is:

> Given a theorem statement (type), generate the elaborated proof term (value).

This is a sequence-to-sequence problem over IR token sequences. The current IR already
represents types correctly. What is missing is the proof-term side.

---

## The Three Blockers

These are the changes required to support full Mathlib extraction for proof-term completion
training. They are **not** to be made until this phase is formally started.

| # | Blocker | Current state | Required change | Effort |
|---|---------|--------------|-----------------|--------|
| 1 | Proof terms skipped | `constantValueExpr?` returns `none` for `.thmInfo` — proof witnesses are never extracted | Remove the `.thmInfo` exclusion; extract and merge proof-term graphs alongside type graphs | Small (1-line change + validation) |
| 2 | Tactic density filter | `maxTacticDensity := 0.8` in `MathlibLoader.lean` drops most of advanced Mathlib | Raise to 1.0 or remove; replace with size-only cap | Trivial (config change) |
| 3 | `mvar` is opaque | Metavariables mapped to a generic `var` entity, losing unification context | Represent `mvar` with expected-type context or unification role; requires new IR node types | Large |

### Notes on each blocker

**Blocker 1** is the most impactful and lowest risk. The comment in `MetaExtractor.lean`
explains the original rationale: proof terms were skipped because they caused 267 "binder
lambda" failures and produced noisy signal for the statement-learning task. For proof-term
completion, noise in proof terms is the signal — this tradeoff inverts completely.

**Blocker 2** is configuration-only. The density filter exists to keep training signal
clean for statement learning. For proof-term completion it becomes an obstacle. The size
cap (`maxSizeBytes := 100_000`) remains useful as a guard against pathologically large
terms and should be kept.

**Blocker 3** is the hardest. Metavariables in proof terms represent open unification goals
— they appear wherever typeclass inference or implicit argument resolution is deferred. A
model that sees `?mvar_42` as a featureless token cannot learn the structural role of
unification in Lean proofs. Properly representing `mvar` requires extending `EntityId`,
`Attribute`, and potentially the token vocabulary. This should not be done until the
simpler blockers are validated.

---

## IR Changes Required (When This Phase Starts)

1. **New `OperationOp` variants** for common proof-term combinators:
   - `recursor` — induction/recursion eliminators (`Nat.rec`, `List.rec`, etc.)
   - `cast` — type coercions (`Eq.mpr`, `cast`)
   - `congr` — congruence lemmas (`congr_arg`, `congr_fun`)
   - `sorry` — placeholder proofs (should be filtered, but worth representing for detection)

2. **Separate type and proof token sequences** in the training schema. Currently a single
   `tokens` array covers both. For proof-term completion, the input is the type sequence
   and the target is the proof sequence. The `corpus.jsonl` schema needs a `proof_tokens`
   field alongside `tokens`.

3. **`mvar` type attribution** (Blocker 3 above) — lower priority, can be deferred to
   a sub-phase.

---

## Relationship to Current Work

- **Phase 5** (complete): A/B/C representation comparison on statement-only IR.
- **Phase 6** (planned): Corpus expansion — more algebra modules + novel module. Uses
  current IR unchanged. Extraction targets `Mathlib.Algebra.GroupPower.Basic`,
  `Mathlib.Algebra.Ring.Basic`, `Mathlib.Algebra.Field.Defs`, `Mathlib.Data.Nat.Basic`.
- **Phase 7** (this document): Proof-term completion. Starts only after Phase 6 results
  are in and the corpus expansion is validated.

Do not conflate Phase 6 corpus expansion with Phase 7 IR changes. The Phase 6 modules
were chosen specifically because they extract cleanly under the current IR. Phase 7
requires IR changes first, then re-extraction of a broader Mathlib slice.

---

## Open Questions (To Be Resolved When Phase 7 Starts)

- What is the right sequence-to-sequence framing? Encoder-decoder vs. decoder-only with
  a type/proof separator token?
- How large does the proof-term corpus need to be for meaningful signal? Phase 5 used
  ~2,500 declarations. Proof-term training likely needs an order of magnitude more.
- Should proof terms be normalized before extraction (e.g. beta-reduced, eta-expanded)?
  The current normalizer was designed for statement structure — its behaviour on proof
  terms has not been characterized.
- Is the 4,495-token IR vocabulary large enough to cover proof-term combinators, or does
  the vocabulary need to grow?
