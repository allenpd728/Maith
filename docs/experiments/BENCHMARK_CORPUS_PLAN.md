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

1. **Inventory pass (Lean-side, no research needed).** Once PleaNP issue
   #70 (complexitylib toolchain reconciliation) lands, have Claude Code
   enumerate everything already formalized in complexitylib and
   descriptive-complexity — a flat list of theorem names, statements in
   plain English, and which sub-area each belongs to (circuit basics,
   Cook-Levin, reductions, descriptive-complexity classes, etc.). This is
   a mechanical extraction task, not a research task.

2. **Domain-breadth check (your review, plain English only).** Review the
   plain-English list and confirm it actually spans multiple
   *unrelated-looking* sub-areas, per AXIOM_DISCOVERY's requirement that
   breadth be real (not several near-duplicate results dressed up as
   different areas). If it doesn't, flag which additional sub-areas are
   missing — this becomes a request to Claude Code, not something you
   source yourself.

3. **Import into the Maith IR pipeline.** Claude Code runs the existing
   extraction pipeline (corpus.jsonl generation) against the imported
   complexitylib/descriptive-complexity declarations, same as it already
   does for Mathlib. No new extraction logic needed — this is the
   existing HOF/projection-expression work applied to a new source
   library instead of new capability.

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
   machine/circuit model definitions it needs from complexitylib. This
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
| Enumerating what's already formalized in complexitylib/descriptive-complexity | Claude Code |
| Extracting/importing into the Maith IR pipeline | Claude Code |
| Formalizing target statements in Lean | Claude Code |
| Verifying correctness | `lake build` (mechanical) + your plain-English cross-check against cited sources (never Lean review) |

## Non-goals

- Not writing any Lean yourself.
- Not treating a single LLM research pass as authoritative — the
  cross-check step is mandatory given how much of the project's validity
  rests on the transfer-target list being accurate.
- Not including any target whose only support is an unverified preprint
  claiming a full solution — per the earlier discussion of the
  crank-repo noise in this space, these should be explicitly filtered
  out, not included with a caveat.
