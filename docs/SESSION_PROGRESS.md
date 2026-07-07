# Maith IR Pipeline — Session Progress

## 2026-07-06 (sessions 2–3: metaprogramming extraction + corpus run)

Note: Encoder/Decoder/Transpiler were scaffolded, not fully implemented, as of this status update — see README Limitations.

### What was done

**Architecture pivot** — replaced `Transpiler.fromLean*` string parsers with
`Maith/MetaExtractor.lean`, a real Lean 4 metaprogram that walks
`ConstantInfo`/`Expr` trees from the live `Environment`. The motivation: Lean's
elaborated representation already contains the fully-resolved type information;
re-deriving it from source text is impossible in the general case (macros,
notation, implicit arguments, typeclass resolution all disappear from source).

**Scoped entity IDs** — added `EntityId.bound : String → EntityId` using the
format `"declName/depth/binderName"`. This prevents De Bruijn index collisions
across declarations. Covered by `testScopedBinderInjectivity`.

**`forallE`/`lam`/`bvar` handling** — both binder forms push a scoped entity
onto `binderCtx`, optionally emit a type-annotation attribute (or `"typeclass"`
for `instImplicit`), recurse into the body, then pop. `bvar n` resolves via
list indexing.

**`OperationOp.generic` fallback** — any constant-headed application not in
the semantic core (add/sub/mul/div/neg/pow/eq/lt/le) emits
`Operation (.generic "FullName")` with token `gen:FullName`. This keeps the IR
vocabulary stable as Mathlib coverage grows.

**`.thmInfo` skip** — proof terms are skipped; only the type (the statement)
is extracted for theorems. This eliminated 267 spurious failures.

**`mergeGraphs` dedup** — type-graph and value-graph entities are deduplicated
by ID on merge. Forall/lambda binders for the same declaration at the same depth
produce identical scoped IDs and are the same entity; naive concatenation was
causing 280 injectivity failures.

**Decoder `gen:` fix** — `String.drop 4` returns `Substring` in Lean 4.31.0;
fixed with `.toString`.

**Added Mathlib dependency** — `mathlib4` v4.31.0 via Lake manifest.

### Corpus results — `Mathlib.Algebra.Group.Defs` (1,129 declarations)

| | Count | % |
|---|---|---|
| **Success** | **792** | **70%** |
| Failure | 337 | 30% |

| Count | Failure reason |
|---|---|
| 217 | type: HOF application (non-constant head) |
| 62 | value: projection expression |
| 48 | value: HOF application |
| 6 | value: let expression |
| 4 | value: `Eq` arity (heterogeneous equality) |

Validated against one module. The failure taxonomy will grow with broader coverage.

Non-trivial successes confirm real content is being extracted:
`mul_assoc` → 11 ents, 5 rels, 5 ops; `DivisionMonoid.mk` → 34 ents, 14 rels, 21 ops.

### Tests

- `lake build tests`: 66 jobs, 0 failures
- All tests pass
- New: `testScopedBinderInjectivity`, `testForallBodyWithOps`, `testCreateDeclarationMetadata` (real assertions)

### Remaining IR gaps (next milestones, by failure count)

1. **HOF application** (265): `f a` where `f` is `bvar`/`fvar` — variable-headed application has no IR representation yet
2. **Projection** (62): `Expr.proj` — structure field access
3. **`letE`** (6): dependent let-binding
4. **Heterogeneous `Eq` arity** (4): guard on arity in `extractApplication`

---

## 2026-07-05 (session 1: baseline fixes)

- Fixed Lean 3-style syntax in `Examples.lean`, `Cirriculum.lean`
- Implemented `Transpiler` round-trip (Lean-like graph text → parse → Graph)
- Implemented `Decoder` token round-trip
- Made `CorpusSerializer` write real files to disk
- Fixed `ProblemGenerator` to reject `a = 0`
- Delegated normalization to `Maith.Normalizer` in `ProcessingPipeline`
- Build: 66 jobs, 0 failures; 54/54 tests passing
