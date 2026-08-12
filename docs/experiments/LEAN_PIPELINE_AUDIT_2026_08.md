# Lean Pipeline Code Audit — 2026-08

> **Scope.** Audit of the Lean files in the IR-creation + corpus-build + pipeline path:
> `MetaExtractor.lean`, `Encoder.lean`, `Normalizer.lean`, `CorpusBuilder.lean`,
> `ProcessingPipeline.lean`, `CorpusSerializer.lean`, `MathlibCorpusBuilder.lean`,
> `MathlibLoader.lean`, `Decoder.lean`, and the core data-model files
> (`EntityId/Entity/Attribute/Relation/Operation/Polarity/Graph/Token`).
>
> **Provenance.** Conducted 2026-08-12 by reading every file in the path end-to-end.
> Findings are ranked by severity. Unambiguous code bugs (typo, stale version strings,
> provably-dead functions) are fixed in the same change set; structural refactors
> (dedup, polarity removal, dead-module pruning) are documented as recommendations
> because they change behavior or surface area and warrant explicit sign-off.
>
> **Headline.** The pipeline is functionally correct for what it does (the 2,554/2,554
> round-trip holds, and the De Bruijn indexing is right). The problems are the classic
> signatures of AI-assisted generation: copy-pasted near-duplicate handlers, dead code
> retained across redesigns, stale version provenance, and a latent correctness bug in
> a helper that is currently masked by its call sites. No finding invalidates the
> experimental record, but several erode maintainability and one (version strings) feeds
> the corpus/dataset provenance confusion flagged in the architecture-doc review.

---

## Findings

### F1 — CRITICAL: `Init.lean` imports a non-existent file (`Maith.Cirriculum`)

`Maith/Init.lean` line: `import Maith.Cirriculum`. The actual file is `Maith/Curriculum.lean`
— there is no `Cirriculum.lean`. This is a typo.

**Why it isn't a build break today:** nothing imports bare `Maith` (Init). Every consumer
(`Scripts/BuildCorpus.lean`, `Tests/*`) imports specific submodules (`Maith.MathlibCorpusBuilder`,
`Maith.Graph`, etc.). `lake build` compiles reachable roots, and Init.lean is unreachable, so
the typo is silent. Init.lean is effectively dead code.

**Fix applied:** correct the typo to `Maith.Curriculum`. (Init.lean's reachability/deadness is
a separate issue — see F6.)

### F2 — HIGH: corpus stats write stale, wrong version strings

`MathlibCorpusBuilder.lean` lines 129–130 hardcode:
```
encoderVersion := "1.2.0"
irVersion := "0.1.0"
```
These are wrong on two counts:
- `Encoder.lean`'s header says **v1.4.0** (the v1.4.0 IO-marker simplification landed). So
  even the Lean-side encoder version is stale.
- `irVersion := "0.1.0"` is meaningless — the representation ID is
  `semantic_graph_ir_v2_0_0` (per `datasets/representation_manifest.json`), and the v2
  token stream is produced in Python (`build_dataset.py`), not here. The Lean-side corpus
  is v1-style strings (see the [architecture-diagram correction](../../README.md#6-architecture)).

**Impact:** `Corpus/stats.json` records `encoderVersion: 1.2.0` / `irVersion: 0.1.0`, so any
provenance check against the actual representation ID fails. This feeds the same corpus-vs-dataset
confusion the doc review surfaced.

**Fix applied:** set `encoderVersion := "1.4.0"` (the actual Lean-side encoder version) and
`irVersion := "semantic_graph_ir_v1_4_0"` (the actual Lean-side corpus format — v1-style
strings; v2 is a Python-side transform). A note is added that v2 is produced downstream.

### F3 — HIGH: `forallE` and `lam` handlers are ~90% duplicated

`MetaExtractor.lean` lines 348–381 (`.forallE`) and 382–409 (`.lam`) are nearly identical:
same scoped-name construction (only the `∀:`/`λ:` prefix differs), identical `binderInfo`
match arms (`instImplicit` / `implicit|strictImplicit` / `default`), identical
`addAttribute`/`addRelation` calls, identical `withBinder` recursion. ~30 lines copied.

**Risk:** a fix applied to one handler but not the other is a likely source of subtle
asymmetry. The open injectivity failure ("`sub(a,b)` and `sub(b,a)` produce identical
tokens", per `V2_NEXT_STEPS`) is exactly the kind of bug duplicated handlers breed.

**Recommendation (not applied):** extract `extractBinder (prefix : String) (binderName …)`,
called as `extractBinder "∀:" …` / `extractBinder "λ:" …`. Halves the code and guarantees
the two binder kinds stay in sync. Behavior-preserving; should be paired with the existing
round-trip + injectivity tests as a regression guard.

### F4 — MEDIUM: `jsonEscape` emits invalid JSON for control chars < U+0020

`CorpusSerializer.lean` `jsonEscape` handles only `"`, `\`, `\n`, `\r`, `\t`. RFC 8259
**requires** all control characters U+0000–U+001F to be escaped (e.g. as `\uXXXX`). Lean
`Expr` `toString` can in principle contain arbitrary characters; if a Mathlib declaration's
name or stringified type contains a control char, the emitted `corpus.jsonl` line is
not valid JSON and the Python loader will reject it.

**Likelihood:** low for current Mathlib, but it is a latent correctness gap with no guard.
**Secondary issue:** the `foldl` with `acc ++ String.push "" c` is O(n²) — it rebuilds the
accumulator on every character. On a 60 MB corpus this is a real cost.

**Recommendation (not applied):** add a control-char escape arm (`if c.toNat < 0x20 then
s!"\\u{c.toNat.toHex}"`), and switch to a `String.Builder` accumulator. Low-risk, but
changes the serialized bytes, so should ship with a corpus rebuild + round-trip re-verify.

### F5 — MEDIUM: `takeLast` has Nat-subtraction underflow + wrong semantics when `n > length`

`MetaExtractor.lean` line 234:
```
private def takeLast (n : Nat) (xs : List α) : List α := xs.drop (xs.length - n)
```
In Lean, `Nat` subtraction saturates at 0. When `xs.length < n`, `xs.length - n = 0`, so
`drop 0` returns the **whole list**, not the last `n`. So `takeLast 5 [a,b,c]` returns
`[a,b,c]` (drop 0) rather than `[a,b,c]` (the last 3, which here coincides) — but
`takeLast 5 []` returns `[]` and the general contract is broken.

**Why it's currently safe:** every call site guards with `if args.length = 3 then takeLast 2`
/ `if args.length ≥ 2`, so `n ≤ length` always holds today. But the function is a latent
trap for any future caller, and its name promises "last n" while its impl is "drop (len−n)".

**Recommendation (not applied):** reimplement as `xs.reverse.take n |>.reverse`, or add an
`if xs.length ≤ n then xs else xs.drop (xs.length - n)` guard. Behavior-preserving at
current call sites; safe to ship.

### F6 — MEDIUM: dead-code subtree (rewrite engine, curriculum, examples, problem generator)

`Init.lean` imports a cluster of modules that the corpus/training pipeline never calls:
- `ProblemGenerator.lean` — no usage outside its own test file
- `RewriteEngine.lean` / `RewritePipeline.lean` — only referenced by each other and
  `Curriculum`/`GraphEquivalence`
- `GraphEquivalence.lean` — no usage outside Init
- `Curriculum.lean` — no usage outside Init
- `Examples.lean` / `DSLHelper.lean` — only used by `Curriculum`

These form a self-referential dead subtree (an abandoned "rewrite engine + problem
generation" design direction) that `Init.lean` re-exports but no live code path reaches.
`Tests/ProblemGeneratorTests.lean` tests `ProblemGenerator`, so that file has a live test,
but the functionality is not in the corpus/training path.

**Impact:** bloats compile time, misleads readers about what the pipeline depends on, and
makes the "what does Maith actually consist of" question harder to answer.

**Recommendation (not applied):** either delete the subtree (if the rewrite-engine
direction is abandoned) or move it to a `Maith/Experimental/` subdirectory and drop it
from `Init.lean`. `ProblemGenerator`'s test would need a decision: keep (as an experiment
harness) or remove. This changes the public surface, so it needs a call.

### F7 — LOW: provably-dead functions

- `MetaExtractor.lean` `bucketGenName` / `bucketComponent` (lines 112–154): the
  name-parsing bucketing heuristic. Replaced by `bucketFromModule` (module mode) and
  `shortNameFromLeanName` (per_operator mode). `bucketGenName` is `private` and has zero
  call sites. **Fix applied:** deleted.
- `Normalizer.lean` `normalizeEntityId : EntityId → Int` (line 26): defined, never called.
  (`normalizeEntity` is used by the mergeSort comparator, but `normalizeEntityId` is not.)
  **Fix applied:** deleted.

### F8 — LOW: `Polarity` is dead weight carried through the entire data model

`Polarity.lean` defines `pos/neut/neg`. Every core structure (`Entity`, `Attribute`,
`Relation`, `Operation`) has a `polarity : Polarity` field. `CorpusSerializer` serializes
`"polarity"` for every entity/attr/rel/op. But:
- `Encoder.lean` v1.3.0 stopped emitting polarity tokens.
- `Normalizer.lean` has no polarity logic (the `normalizePolarity*` no-ops were deleted,
  per `V2_NEXT_STEPS`).
- Polarity is ~24% of v1 tokens and carries no signal (99.8% `neut`, per IR_SCHEMA audit).

So polarity is retained in the data model and serialized corpus but does nothing — it's
"kept for potential future use" (ENCODER_FORMAT) but is dead in the current pipeline.

**Impact:** inflates corpus size, clutters every structure, and is a recurring source of
the "why is this field here" question. The IR_SCHEMA audit already flagged it as noise.

**Recommendation (not applied):** remove `polarity` from the four structures and from the
serializer. This is a v1-corpus-format break (the serialized JSON loses a field), so it
must ship with a corpus rebuild + round-trip re-verify + a `representation_id` bump. Not
a quick fix, but the cleanest single move to reduce noise.

### F9 — LOW: `standardMathlibModules` is stale and unused

`MathlibLoader.lean` defines `standardMathlibModules := ["Mathlib.Algebra.Group.Basic"]`
(one module). `BuildCorpus.lean` hardcodes 14 modules and never references
`standardMathlibModules`. The "standard" list is stale and misleading.

**Recommendation (not applied):** either delete it or make `BuildCorpus.lean` use it.
Currently it's a footgun (someone might call `buildMathlibIRCorpusStandard` expecting the
14-module set and get 1 module).

### F10 — NON-ISSUE (verified correct): De Bruijn indexing

For the record, because it looked suspicious at first glance: `MetaExtractor.lean` resolves
`Expr.bvar n` via `st.binderCtx[n]?` where `binderCtx` is built by `withBinder` prepending
to the head (`id :: st.binderCtx`). `List.get?` is 0-indexed from the head, and the head
is the innermost binder (De Bruijn 0). So `bvar 0 → binderCtx[0] = innermost`. This matches
Lean's De Bruijn convention. **Correct — no action.**

---

## Summary of fixes applied in this change set

| Finding | Fix | File |
|---|---|---|
| F1 | `Maith.Cirriculum` → `Maith.Curriculum` | `Maith/Init.lean` |
| F2 | version strings `1.2.0`/`0.1.0` → `1.4.0`/`semantic_graph_ir_v1_4_0` + note | `Maith/MathlibCorpusBuilder.lean` |
| F7 | delete dead `bucketGenName`/`bucketComponent` | `Maith/MetaExtractor.lean` |
| F7 | delete dead `normalizeEntityId` | `Maith/Normalizer.lean` |

## Recommended follow-ups (not applied — need sign-off)

| Finding | Change | Why deferred |
|---|---|---|
| F3 | extract shared `extractBinder` for forallE/lam | behavior-preserving but touches the extraction core; pair with injectivity-test re-run |
| F4 | control-char JSON escaping + `String.Builder` | changes serialized bytes; ship with corpus rebuild |
| F5 | reimplement `takeLast` | behavior-preserving at call sites; trivial but in extraction core |
| F6 | prune/move dead rewrite-engine subtree | changes public surface; needs a decision on whether the direction is abandoned |
| F8 | remove `Polarity` from data model + serializer | v1-corpus-format break; needs corpus rebuild + `representation_id` bump |
| F9 | delete or repoint `standardMathlibModules` | minor, but touches the public builder API |

---

## Verification

The fixes applied (F1, F2, F7) are textual and do not change runtime behavior of the live
pipeline: F1 fixes a typo in an unreachable file; F2 changes only the version strings
written to `stats.json`; F7 deletes provably-unreferenced `private` functions. The live
corpus-build path (`Scripts.BuildCorpus.lean` → `MathlibCorpusBuilder` → `MetaExtractor`/
`Normalizer`/`Encoder`) is unchanged. `lake build` should be re-run to confirm F7's
deletions don't break a transitive reference the grep missed; this audit was conducted
without a Lean toolchain available in the review environment.
