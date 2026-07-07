# Maith IR Pipeline

A Lean 4 pipeline that extracts IR graphs from real Mathlib declarations using
Lean's own metaprogramming APIs, then encodes them as token sequences for
machine-learning training corpora.

## Current status (2026-07-06)

- **Build**: `lake build tests` — 66 jobs, 0 failures
- **Tests**: all pass (54 unit tests + corpus-pipeline and serializer integration checks)
- **Corpus extraction** against `Mathlib.Algebra.Group.Defs` (1,129 declarations):

| | Count | % |
|---|---|---|
| **Successful extractions** | **792** | **70%** |
| Failed | 337 | 30% |

**Failure breakdown (exhaustive, not rounded):**

| Count | Reason |
|---|---|
| 217 | type extraction failed: HOF application (non-constant head) |
| 62 | value extraction failed: projection expression |
| 48 | value extraction failed: HOF application (non-constant head) |
| 6 | value extraction failed: let expression |
| 4 | value extraction failed: `Eq` arity (heterogeneous equality) |

This breakdown is the honest measure of where the IR stands. A 70% success rate on
one module is meaningful, but other modules will likely surface new failure categories.
This has been validated against exactly one module; the failure taxonomy will likely
grow with broader corpus coverage.

**Example non-trivial successes:**

| Declaration | Entities | Relations | Operations |
|---|---|---|---|
| `mul_assoc` | 11 | 5 | 5 |
| `mul_comm` | 8 | 4 | 3 |
| `mul_one` | 11 | 3 | 6 |
| `DivisionMonoid.mk` | 34 | 14 | 21 |

---

## Architecture

### The pivot: why metaprogramming instead of string parsing

The original design attempted to re-derive IR structure from Lean source text
(a `String → Graph` approach). This was fundamentally wrong: it would require
reimplementing Lean's parser, macro expander, elaborator, implicit argument
inference, and typeclass resolution from scratch — with none of Lean's actual
type information available. That cannot reach real Mathlib content regardless
of how much the string-parsing logic is improved.

**The fix is architectural**: extract IR structure directly from Lean's own
elaborated representation using Lean 4's metaprogramming API (`Environment`,
`ConstantInfo`, `Expr`). Lean already has the fully parsed, fully elaborated form
in memory — use it instead of re-deriving it from text.

### Pipeline

```
Lean environment (real declarations, real Expr trees)
        │
        ▼   MetaExtractor.lean  ←── walks ConstantInfo / Expr directly
   IR Graph (Entity / Attribute / Relation / Operation)
        │
        ▼   Normalizer.lean
   Sorted, canonical Graph
        │
        ▼   Encoder.lean
   Token sequence
        │
        ▼   CorpusSerializer.lean
   corpus.jsonl  (one line per declaration, schema in CORPUS_SCHEMA.md)
        │
        ▼   python/  (separate from Lean code)
   Vocab / tokenizer / train-eval split / dataset
```

**Division of responsibility**: Lean owns extraction — only Lean understands
Lean's elaborated terms. Python owns everything downstream of the JSONL: token
ID assignment, vocabulary construction, data splits, and training. There is no
Lean-source string parser anywhere in the codebase.

### Scoped entity IDs and De Bruijn safety

A non-obvious but load-bearing design decision: Lean's elaborated terms use De
Bruijn indices for bound variables (binders are nameless positions, 0 = innermost).
Two different declarations can both have a bound variable at index 0 — they mean
entirely different things. Naïvely assigning `EntityId` by De Bruijn index alone
causes token collisions across declarations.

The solution is `EntityId.bound : String → EntityId`, where the string is formed
as `"declName/depth/binderName"`. This makes every scoped entity globally unique:
the same binder name `x` in `mul_assoc` and `mul_comm` produces different entity
IDs and thus different tokens. `testScopedBinderInjectivity` in
`Tests/CorpusPipelineTests.lean` covers this property directly — two declarations
both quantifying over a variable named `x` must produce non-colliding token streams.

---

## Next milestones (prioritized by failure count)

These are the three remaining IR gaps, each with a concrete failure count as a
size estimate. They are listed in recommended implementation order.

### 1. HOF application support — 265 failures (largest)

**What it is**: expressions of the form `f a b` where `f` is a bound or free
variable (a `bvar` or `fvar` applied as a function), not a known constant.
Examples: `Function.Injective f` expands to `∀ a b, f a = f b → a = b`, where
`f a` has a `bvar` as the application head. The current IR can represent
constant-headed operations (`HMul.hMul a b`) but not variable-headed ones.

**What it needs**: either a new `OperationOp.hof` constructor representing
"apply the entity at position X to these arguments", or a relational encoding
where `f a` becomes a `Relation` edge rather than an `Operation` node.
This is likely the largest single lift because it touches the structural
representation of function application — the most common thing in type theory.

### 2. Projection expressions (`.proj`) — 62 failures

**What it is**: structure field access, e.g. `s.field` encoded as `Expr.proj`.
These appear in the values of definitions that destructure structures.

**What it needs**: a new IR node or `RelationOp` variant representing
"field access from entity X under field name Y". Relatively self-contained.

### 3. `letE` (dependent let-binding) — 6 failures

**What it is**: `let x := e₁; e₂`, encoded as `Expr.letE`. Low frequency now
but will grow with more complex definitions.

**What it needs**: an additional binder-context extension similar to `forallE`/`lam`,
plus an IR representation for the bound value (an `Attribute` or new node type).
Small scope, good warmup before tackling HOF.

### 4. Heterogeneous `Eq` arity — 4 failures

**What it is**: `HEq` / heterogeneous equality applied with unexpected arity.
The current `extractRelation` for `Eq` assumes exactly two arguments; some
elaborated forms supply type arguments too.

**What it needs**: a guard or arity normalization step in the `Eq` case of
`extractApplication`. Small and surgical.

---

## Test suite

```bash
lake build tests
./.lake/build/bin/tests
```

Tests are organized by module in `Tests/`. Key tests:

- `Tests/InjectivityTests.lean` — normalization does not collapse distinct graphs
- `Tests/CorpusPipelineTests.lean` — end-to-end extraction including:
  - `testScopedBinderInjectivity`: De Bruijn collision safety across declarations
  - `testForallBodyWithOps`: non-trivial `∀ x y, x+y = y+x` extracts ≥2 ops
  - `testCreateDeclarationMetadata`: real assertions on extracted metadata fields
- `Tests/DecoderTests.lean` — token round-trips including `gen:` prefix
- `Tests/NormalizerTests.lean`, `Tests/EncoderTests.lean` — component correctness

---

## JSONL schema

See `CORPUS_SCHEMA.md`. The Python consumer (`python/`) reads that file as its
stable contract and does not parse any Lean source text.

---

## Directory structure

```text
Maith/
  EntityId.lean          ← EntityId.var | .bound | .term; scoped naming
  MetaExtractor.lean     ← walks ConstantInfo/Expr; core extraction logic
  ProcessingPipeline.lean
  CorpusSerializer.lean
  MathlibCorpusBuilder.lean
  OperationOp.lean       ← includes generic : String → OperationOp fallback
  Normalizer.lean
  Encoder.lean
  Decoder.lean
  ... (other IR types: Entity, Attribute, Relation, Operation, Graph, Token)
Tests/
  CorpusPipelineTests.lean
  InjectivityTests.lean
  DecoderTests.lean
  ...
python/
  loader.py              ← validates corpus.jsonl against schema
  vocab.py               ← vocab/token-ID construction
  dataset.py             ← train/eval split, PyTorch Dataset
Corpus/
  corpus.jsonl           ← output; not committed
  stats.json             ← extraction statistics; not committed
CORPUS_SCHEMA.md         ← stable contract between Lean output and Python input
```
