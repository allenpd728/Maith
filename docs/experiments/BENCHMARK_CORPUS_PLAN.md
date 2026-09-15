# Benchmark Corpus Plan

## Purpose

Companion to AXIOM_DISCOVERY.md. That document assumes a benchmark corpus
exists in two parts (transfer targets, conservativity corpus) without
specifying how to build either. This document is that plan.

## The hard constraint this plan is designed around

No personal Lean literacy. This means every step below is scoped so that
verification of correctness never depends on personally reading Lean code.
The division of labor is:

- **Research and selection of *what* the targets/corpus should contain** —
  literature-level work, doable by a non-Lean-literate researcher (you)
  plus an LLM literature-research pass (DeepSeek).
- **Formalizing/importing *that* content into Lean** — Lean-literate work,
  done by Claude Code or another Lean-capable assistant.
- **Correctness verification** — never "does this Lean code look right,"
  always "does `lake build` succeed" plus "does the statement, translated
  back into plain English by the assistant, match what the research pass
  said it should say." The type checker is the oracle; your job is
  checking that the *English description* of a formalized statement
  matches the literature, not checking Lean syntax.

This mirrors the project's existing non-negotiable design principle
(kernel as sole oracle) — it's not a new rule, just applied to a case
where you personally can't be the one reading the proof term.

## Part 1: Conservativity / compression corpus

**Goal:** a broad set of *already-proved* theorems, spanning genuinely
unrelated sub-areas of circuit and proof complexity, imported rather than
hand-formalized.

### Steps

1. **Inventory pass (Lean-side, no research needed).** Have Claude Code
   enumerate everything already formalized in PleaNP's circuit-complexity
   modules — a flat list of theorem names, statements in plain English, and
   which sub-area each belongs to (circuit basics, tree/decision-tree models,
   the invented-model round, the remaining lower-bound library, etc.). This is
   a mechanical extraction task, not a research task.

   > **Correction (2026-09-15).** The original text of this plan said to
   > inventory `complexitylib` and `descriptive-complexity`. That is no longer
   > the source. PleaNP closed issue #70 on 2026-09-13 with a decision to
   > **reject the `complexitylib` import** — its current head pins an unstable
   > `v4.34.0-rc2` toolchain plus a `cslib` dependency, while PleaNP pins stable
   > `v4.31.0`, so importing it would trade a stable pin for a prerelease. The
   > decision (recorded as PleaNP DEC-025) was to build a local
   > `PleaNP.Circuits` instead, which has since landed. The corpus source is
   > therefore **`PleaNP.Circuits`**, not an external library. See
   > "Dependencies" below.

2. **Domain-breadth check (your review, plain English only).** Review the
   plain-English list and confirm it actually spans multiple
   *unrelated-looking* sub-areas, per AXIOM_DISCOVERY's requirement that
   breadth be real (not several near-duplicate results dressed up as
   different areas). If it doesn't, flag which additional sub-areas are
   missing — this becomes a request to Claude Code, not something you
   source yourself.

3. **Import into the Maith IR pipeline.** Claude Code runs the existing
   extraction pipeline (corpus.jsonl generation) against the
   `PleaNP.Circuits` declarations, same as it already does for Mathlib. No new
   extraction logic needed — this is the existing HOF/projection-expression
   work applied to a new source library instead of new capability. The Lean-side
   representation of HOF application and `.proj` is **implemented and passing**
   (`Tests/CorpusPipelineTests.lean`: `testHOFApplicationExtracts`,
   `testProjectionExtracts`), so this path is known-good on synthetic
   declarations; the open question is only whether real Circuits declarations
   stay inside its coverage (see AXIOM_DISCOVERY's coverage-map step).

4. **Sign-off.** You never read the Lean. You review: (a) the plain-English
   theorem list from step 1, confirmed against the papers/textbook
   results they're supposed to represent (this is where DeepSeek helps,
   see Part 3), and (b) a `lake build` pass/fail signal from Claude Code.
   Both green is sufficient sign-off.

## Part 2: Transfer targets (open or hard lower bounds)

**Goal:** a short, curated list (5-15 items) of specific, well-defined,
citable lower-bound results in *restricted* models — hard or open
precisely because known techniques don't reach them, not because nobody's
tried. Explicitly not P vs NP or NP vs P/poly.

### Steps

1. **Literature research pass — DeepSeek's job.** Prompt DeepSeek for a
   research audit with a tightly scoped brief (draft brief below). Its
   output is a plain-English candidate list, not Lean — well within a
   research-audit LLM task, no Lean literacy required to commission or
   read it.

2. **Filtering pass — your job, plain English only.** For each candidate
   DeepSeek returns, check it against three plain-language criteria
   (no math background beyond what DeepSeek's summary gives you):
   - Is it specific (a named theorem/conjecture, not a vague research
     area)?
   - Does the source explicitly say *why* it's hard (barrier-documented
     or technique-documented, not just "unsolved")?
   - Is it in a restricted-enough model that a new technique has a
     plausible shot, per the earlier discussion of aiming narrow rather
     than at full P vs NP?
   Reject anything that fails any of the three; you don't need Lean
   knowledge to apply this filter, only DeepSeek's own summary of each
   item.

3. **Cross-check pass — second LLM opinion.** Run the filtered list past
   a second research pass (a different model, or a follow-up prompt
   asking specifically "is this list accurate and are these still open/
   hard as of [date]") before treating it as final — literature-audit
   tasks like this are exactly where a single LLM pass can miss a recent
   result or misstate a theorem's actual status, and a second, differently
   -prompted pass is a cheap check given how load-bearing this list is.

4. **Formalization — Claude Code's job.** For each surviving target,
   Claude Code formalizes the *statement* (not a proof — these are
   meant to be unsolved) as a Lean proposition, importing whatever
   machine/circuit model definitions it needs from `PleaNP.Circuits`. This
   is a statement-formalization task (translate an English theorem
   statement into a typed Lean proposition), which is checkable by
   `lake build` succeeding on the *statement* (it type-checks as a
   well-formed proposition) even though no proof exists yet.

5. **Sign-off.** Same pattern as Part 1: you review DeepSeek's plain-
   English description of each target against its cited source, and
   accept Claude Code's confirmation that the statement type-checks as a
   well-formed (unproved) proposition. You are never evaluating Lean
   syntax, only whether the English description matches the paper it's
   attributed to.

## Draft brief for DeepSeek (Part 2, step 1)

To be refined before use, but the shape should be:

> Identify specific, named lower-bound results or separations in circuit
> complexity, proof complexity, or communication complexity that are
> either open or were only resolved by unusually novel techniques, where
> the literature explicitly attributes the difficulty to a known barrier
> (relativization, natural proofs, algebrization) or to a specific
> documented limitation of existing technique families. Prefer restricted
> models (bounded-depth circuits, monotone circuits, specific proof
> systems) over the general P vs NP question. For each result, cite the
> paper(s) establishing its status and briefly state, in plain English,
> why it's considered hard. Exclude anything where the only source is a
> preprint claiming to have solved it (these are almost always
> unverified or wrong) — flag such claims separately as noise, not as
> targets.

## Division of labor summary

| Task | Who |
|---|---|
| Research which theorems/targets exist and why they're hard | DeepSeek (+ your plain-English filtering) |
| Second-opinion accuracy check on that research | A second LLM pass |
| Enumerating what's already formalized in `PleaNP.Circuits` | Claude Code |
| Extracting/importing into the Maith IR pipeline | Claude Code |
| Formalizing target statements in Lean | Claude Code |
| Verifying correctness | `lake build` (mechanical) + your plain-English cross-check against cited sources (never Lean review) |

## Dependencies and current corpus state (2026-09-15)

Both parts of this plan depend on PleaNP's circuit-complexity substrate, which
changed source on 2026-09-13. Current state, verified:

| Dependency | Status |
|---|---|
| `complexitylib` import into PleaNP | **Rejected** (PleaNP #70, its DEC-025) — pins `v4.34.0-rc2` + `cslib`; incompatible with PleaNP's stable `v4.31.0` pin |
| Local `PleaNP.Circuits` | **Exists, but is an early stub** — `lean/PleaNP/Circuits/Basic.lean` is a 17-line header listing *planned* coverage (boolean circuits, AC⁰/TC⁰/NC, switching lemma, Parity ∉ AC⁰, Razborov monotone lower bounds, Williams). The validation suite landed (PleaNP #71) covering the natural-property/largeness definitions; the lower-bound library is still open (e.g. PleaNP #72: parity ∉ AC⁰ via the switching lemma). |
| Extra circuits modules (#73, #37) | Landed — tree/decision-tree models and an "invented-model round" |

**Consequence for this plan: Part 1 (the conservativity corpus) cannot be built
yet.** The "enumerate everything already formalized" inventory pass would today
find a handful of modules, not a broad multi-sub-area corpus — and the plan's own
domain-breadth check (step 2) would fail on it. Part 1 is **blocked** on PleaNP's
Rung-4 lower-bound library growing (PleaNP #72 and successors).

**Part 2 (transfer targets) is not blocked** in the same way: the *research* and
*filtering* passes (DeepSeek + a second LLM pass) need no Lean, and the
formalization pass needs only whatever circuit-model definitions already exist.
Part 2 should proceed first, and Part 1 revisited once the substrate is broad
enough to be a real breadth test.

*This table is the doc-coherence fix required by the corpus plan having been
written against `complexitylib`, which will never be imported. Maith's
`docs/MULTI_AGENT_WORKFLOW.md` §1b makes keeping this current part of the
protocol.*

## Non-goals

- Not writing any Lean yourself.
- Not treating a single LLM research pass as authoritative — the
  cross-check step is mandatory given how much of the project's validity
  rests on the transfer-target list being accurate.
- Not including any target whose only support is an unverified preprint
  claiming a full solution — per the earlier discussion of the
  crank-repo noise in this space, these should be explicitly filtered
  out, not included with a caveat.
