# Latest Fixes — 2026-07-06

Note: Encoder/Decoder/Transpiler were scaffolded, not fully implemented, as of this status update — see README Limitations.

## Summary

This session completed the architectural pivot from string-based Lean parsing
to real Lean 4 metaprogramming extraction, and drove corpus success on
`Mathlib.Algebra.Group.Defs` from **0/1,129** to **792/1,129 (70%)**.

## What changed

### Architecture pivot (sessions 1–3, cumulative)

The original `Transpiler.fromLean*` functions attempted to re-derive IR
structure from Lean source strings. This was replaced entirely with
`Maith/MetaExtractor.lean`, which walks `ConstantInfo`/`Expr` trees from
the live Lean `Environment` directly. No string-based Lean parser exists
anywhere in the codebase.

### `EntityId.bound` — scoped De Bruijn safety

Added `EntityId.bound : String → EntityId` with format
`"declName/depth/binderName"`. Two declarations that both bind a variable
named `x` now produce distinct entity IDs and non-colliding tokens.
`testScopedBinderInjectivity` covers this property.

### `forallE` / `lam` / `bvar` handling

`forallE` and `lam` binders are now processed symmetrically: push a scoped
entity onto `binderCtx`, optionally emit a type annotation (or `"typeclass"`
attribute for `instImplicit`), recurse into body, pop. `bvar n` resolves via
`binderCtx[n]?`.

### `OperationOp.generic` fallback

Any constant-headed application (`f a b` where `f` is a known `const`) that
isn't in the semantic core (add/sub/mul/div/neg/pow/eq/lt/le) now becomes
`Operation (.generic "f.fullName")` with token `gen:f.fullName`. This
prevents the IR vocabulary from growing per-symbol forever.

### `.thmInfo` value skip

Proof term values (theorem witnesses) are now skipped: only the TYPE of a
theorem is extracted, since the type is the statement. This eliminated
267 spurious "binder lambda in proof term" failures.

### `mergeGraphs` entity deduplication (2026-07-06)

When merging the type graph and value graph for a definition, entities are
now deduplicated by ID before concatenation. Forall-binders in the type and
lambda-binders in the definition body share scoped names — they are the same
semantic variable. Naïve concatenation was producing 280 injectivity failures;
this fix recovered all of them.

### `Decoder.lean` `gen:` prefix fix (2026-07-06)

`String.drop` in Lean 4.31.0 returns `Substring`, not `String`. Fixed using
`(s.drop 4).toString`.

## Corpus results (2026-07-06)

Module: `Mathlib.Algebra.Group.Defs`, 1,129 declarations

| | Count | % |
|---|---|---|
| **Success** | **792** | **70%** |
| Failure | 337 | 30% |

| Count | Failure reason |
|---|---|
| 217 | type: HOF application (non-constant application head) |
| 62 | value: projection expression |
| 48 | value: HOF application |
| 6 | value: let expression |
| 4 | value: `Eq` arity (heterogeneous equality) |

Note: validated against one module only. The failure taxonomy will likely grow
with broader corpus coverage (exotic notation, tactic blocks, numeral encodings).

## Test status (2026-07-06)

- `lake build tests`: 66 jobs, 0 failures
- `./.lake/build/bin/tests`: all tests pass
- New/fixed tests this session:
  - `testForallBodyWithOps` — `∀ x y : Nat, x+y = y+x` extracts ≥2 ops + ≥1 rel
  - `testScopedBinderInjectivity` — De Bruijn collision safety across declarations
  - `testCreateDeclarationMetadata` — real assertions on extracted metadata fields

