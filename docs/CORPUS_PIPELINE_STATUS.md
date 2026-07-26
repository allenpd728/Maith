# Corpus Pipeline Implementation Status

**Last updated**: July 26, 2026
**Status**: ✅ **PRODUCTION READY — 100% coverage across 4 Mathlib modules (2554 declarations)**

Encoder, Decoder, and MetaExtractor are fully implemented. Transpiler is a debug-only
display utility — not in the training path. See `SESSION_PROGRESS.md` for full changelog.

## Overview

The pipeline extracts IR directly from Lean's elaborated environment via `MetaExtractor.lean`,
normalizes through `Maith.Normalizer`, encodes to tokens, and serializes to JSONL.

The original string-based `Transpiler.fromLean*` approach was replaced entirely in the July 6
session. No string-based Lean parser remains in the codebase. All extraction now happens by
walking `ConstantInfo`/`Expr` trees from the live `Environment`.

## Pipeline Architecture

```
1. Enumeration       → Load declarations from Lean Environment (MathlibLoader.lean)
2. Extraction        → Walk ConstantInfo/Expr trees (MetaExtractor.lean)
3. IR Construction   → Build IR Graph (Entity, Attribute, Relation, Operation)
4. Normalization     → Canonical ordering via Maith.Normalizer (ProcessingPipeline.lean)
5. Injectivity Check → Verify no duplicate entity IDs
6. Encoding          → IR Graph → Token sequence (Encoder.lean — scaffold)
7. Example Building  → Create TrainingExample records with metadata
8. Accumulation      → Build statistics and collect examples
9. Serialization     → Write JSONL + stats + logs to disk (CorpusSerializer.lean)
```

Step 6 uses Encoder v1.2.0: positional `FVAR_N`/`BVAR_N`/`TERM_N` tokens, cap 63, 2554/2554 round-trip verified.

## Implementation Summary

### Modules

| Module | Status | Purpose |
|--------|--------|---------|
| `MetaExtractor.lean` | ✅ Validated | Elaborated Lean Expr → IR graph |
| `EntityId.lean` | ✅ Complete | Scoped binder IDs (`EntityId.bound`) |
| `Normalizer.lean` | ✅ Complete | Canonical graph ordering |
| `CorpusBuilder.lean` | ✅ Complete | Core data structures |
| `ProcessingPipeline.lean` | ✅ Complete | Normalization wiring |
| `MathlibLoader.lean` | ✅ Complete | Declaration enumeration |
| `CorpusSerializer.lean` | ✅ Complete | JSONL/stat serialization to disk |
| `MathlibCorpusBuilder.lean` | ✅ Complete | Top-level orchestration |
| `Encoder.lean` | ✅ v1.2.0 | Graph → token sequence (positional FVAR_N/BVAR_N/TERM_N, cap 63) |
| `Decoder.lean` | ✅ Complete | Token → graph (v0.1.0 + v1.2.0 backward compat, total function) |
| `Transpiler.lean` | 🔧 Debug-only | Human-readable IR formatter, not in training path |

## Core Data Structures

### TrainingExample
```lean
structure TrainingExample where
  name : String           -- Declaration name
  module : String         -- Source Mathlib module
  leanExpr : String       -- Serialized elaborated declaration type
  graph : Graph           -- Normalized IR graph
  tokens : List Token     -- Encoded token sequence
```

### CorpusStats
```lean
structure CorpusStats where
  totalDeclarations : Nat
  successfulExamples : Nat
  failureStats : FailureStats
  tokenDistribution : TokenDistribution
  graphStats : GraphStats
  mathlibCommitHash : String
  encoderVersion : String
  irVersion : String
```

## Corpus Results (July 25, 2026)

4 modules — 2,554 declarations total

| Module | Declarations | Success |
|--------|-------------|---------|
| Algebra.Group.Defs | 1,129 | 1,129 (100%) |
| Algebra.Group.Basic | 548 | 548 (100%) |
| Algebra.Ring.Defs | 446 | 446 (100%) |
| Order.Basic | 431 | 431 (100%) |
| **Total** | **2,554** | **2,554 (100%)** |

Zero failures. All previously identified failure categories resolved (HOF, projection, letE, HEq).
Mathlib version: `fabf563a` (v4.31.0). Encoder version: 1.1.0.

## Key Design Decisions

### Why MetaExtractor, not Transpiler

Lean's elaborated representation already contains fully-resolved type information — implicit
arguments, notation expansion, typeclass resolution, and macro expansion are all done. Re-deriving
this from source strings is impossible in the general case. `MetaExtractor.lean` walks
`ConstantInfo`/`Expr` directly from the live `Environment`, avoiding that problem entirely.

### Encoder v1.1.0: positional bound IDs

Lean uses De Bruijn indices for bound variables. Early versions used scoped IDs
(`"declName/depth/binderName"`) — unique but producing 21k singleton tokens unusable for training.
Encoder v1.1.0 replaces them with positional `BVAR_N` tokens assigned in first-appearance order
within each graph (cap 63, overflow → `BVAR_MANY`). This collapsed vocabulary from 63,747 → 5,577
tokens (−91%) while leaving sequence lengths unchanged. See `docs/ENCODER_FORMAT.md`.

### `OperationOp.generic` fallback

Constant-headed applications outside the semantic core (add/sub/mul/div/neg/pow/eq/lt/le) emit
`Operation (.generic "FullName")` with token `gen:FullName`. This keeps the IR vocabulary stable
as Mathlib coverage grows.

### `.thmInfo` value skip

Proof term values are skipped; only the TYPE (the statement) is extracted for theorems. This
eliminated 267 spurious failures from proof-term lambda traversal.

## Build and Test Status

```
lake build tests: all jobs, 0 failures
./.lake/build/bin/tests: all tests passing
```

Corpus: 2554/2554 (100%) across 4 modules (Algebra.Group.Defs, Algebra.Group.Basic, Algebra.Ring.Defs, Order.Basic).
Token distribution: min 13, max 13112, avg 318, total 813,897.
Graph stats: avg 35.8 entities, 13.5 relations, 22.3 operations, max 3,142 nodes.

To regenerate: `lake build buildCorpus && lake env ./.lake/build/bin/buildCorpus`

## Next Steps

1. **Run A/B/C training experiment** — `python3 python/train.py --variant A/B/C`; compare eval perplexity
2. **Expand corpus** — add more Mathlib modules beyond the current 4
3. **Explicit binder markers** — distinguish forall vs lambda binders in token stream (future refinement)
