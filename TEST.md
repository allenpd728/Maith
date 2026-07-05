# Maith IR Test Suite

## Overview

The Maith IR test suite validates the core intermediate representation (IR) components used for transformer-based proof assistance. The tests are structured using **Lake** (Lean's package manager) and organized into multiple modules for testing specific components.

## Test Infrastructure Status

### ✅ Passing Tests (22 tests)

The following core IR components have been successfully tested and compile without errors:

- **Polarity** (3 tests) — Semantic direction markers (pos, neut, neg)
- **EntityId** (2 tests) — Entity identifiers (var, term)
- **Entity** (1 test) — Atomic IR units
- **Attribute** (1 test) — Entity metadata
- **RelationOp** (3 tests) — Semantic relation operators (eq, add, sub, etc.)
- **Relation** (1 test) — Semantic edges between entities
- **OperationOp** (2 tests) — Constructive operation operators (add, mul, etc.)
- **Operation** (1 test) — Constructive steps that build entities
- **Graph** (2 tests) — Complete IR representation
- **Encoder Tests** (1 placeholder test) — Skipped due to module issues
- **Decoder Tests** (1 placeholder test) — Skipped due to module issues
- **Pipeline Tests** (2 tests) — Graph type verification

### ⚠️ Compilation Issues in Utility Modules

The following modules have existing compilation errors that need fixing before full integration testing can begin:

#### Encoder.lean
- **Issue**: Syntax error on line 63 — unexpected token `\`
- **Cause**: List syntax incompatibility with Lean 4 (array/list construction)
- **Impact**: Cannot test IR → Token encoding pipeline

#### Decoder.lean
- **Issue**: Syntax error on line 69 — unexpected token `\`
- **Cause**: Same as Encoder (list syntax)
- **Impact**: Cannot test Token → IR decoding pipeline

#### Normalizer.lean
- **Issues**:
  - Invalid pattern matching on line 74
  - Missing `ToString` instances for `RelationOp` and `OperationOp`
  - Missing `LT` instance for `EntityId`
- **Impact**: Cannot run graph normalization

#### GraphEquivalence.lean
- **Issue**: Invalid field projection syntax
- **Cause**: Structures don't have the expected fields or projections
- **Impact**: Cannot verify graph equivalence

#### DSLHelper.lean
- **Issue**: Duplicate `Operation` declaration
- **Impact**: Helper functions not available

#### RewriteEngine.lean
- **Issue**: List syntax errors on lines 78, 90, 116
- **Cause**: Same as Encoder/Decoder
- **Impact**: Rewrite engine not functional

#### Examples.lean
- **Issue**: Depends on broken modules
- **Impact**: Cannot load example graphs

## Running the Tests

### Prerequisites

- Lean 4 (v4.31.0 or compatible)
- Lake (v5.0.0 or compatible)
- Elan (for Lean toolchain management)

### Build the Test Suite

```bash
cd /Users/philipallen/Desktop/Maith
lake build tests
```

### Run the Tests

```bash
./.lake/build/bin/tests
```

### Expected Output

```
╔════════════════════════════════════════════════════╗
║        Maith IR Pipeline Test Suite                ║
╚════════════════════════════════════════════════════╝

=== Polarity Tests ===
✓ Polarity.pos is defined
✓ Polarity.neut is defined
✓ Polarity.neg is defined

Results: 3/3 passed

[... more test results ...]

╔════════════════════════════════════════════════════╗
║         Test suite complete                        ║
╚════════════════════════════════════════════════════╝
```

## Test Structure

```
Tests/
├── Harness.lean              # Test framework & utilities
├── ComponentTests.lean       # Core IR component tests
├── EncoderTests.lean         # Encoder tests (placeholder)
├── DecoderTests.lean         # Decoder tests (placeholder)
├── PipelineTests.lean        # End-to-end tests
└── Main.lean                 # Test orchestration entry point
```

### Harness.lean

Provides a lightweight testing framework:

- `TestResult` — inductive type for pass/fail results
- `runTest` — execute a boolean assertion
- `assertEqual` — compare two values for equality
- `assertNotEqual` — verify two values differ
- `printTestResult` — display results
- `runTestSuite` — orchestrate multiple tests with summary reporting

### ComponentTests.lean

Validates individual IR components verify types and constructors compile correctly:

- Polarity variants
- EntityId variants
- Core entity, attribute, relation, operation types
- RelationOp and OperationOp variants
- Graph type

### PipelineTests.lean

Tests higher-level IR functionality (currently placeholder tests due to broken encoder/decoder modules).

## Next Steps for Full Functionality

### Priority 1: Fix Encoder/Decoder

1. **Encoder.lean line 63** — Replace `\[...]` with `[...]` for list literals
2. **Decoder.lean line 69** — Same fix
3. Verify token format is consistent
4. Add encoder/decoder roundtrip tests

### Priority 2: Fix Normalizer

1. Implement missing `ToString` instances for `RelationOp`, `OperationOp`
2. Implement `LT` instance for `EntityId`
3. Fix pattern matching syntax
4. Add normalization tests

### Priority 3: Fix Graph Equivalence

1. Verify field projections work
2. Implement graph comparison logic
3. Add equivalence verification tests

### Priority 4: Enable Example Loading

1. Fix DSLHelper duplicate declaration
2. Load Examples.lean
3. Create tests validating example graphs

### Priority 5: Generate Training Corpus

Once encoder/decoder work, generate a dataset of IR-encoded examples for SLM training:

```lean
def generateCorpus : IO Unit := do
  let graphs := Examples.exampleGraphs
  let tokens := graphs.map (fun g => defaultEncoder.encodeGraph g)
  -- Write tokens to file for training
```

## Testing Known Issues

1. **Field notation** — Lean 4 struct syntax `{...}` requires careful handling in tests
2. **Module dependencies** — Some modules have circular import issues due to old code style
3. **Linter warnings** — Many files use deprecated `structure ... :=` syntax (should use `structure ... where`)

## Performance Notes

- Test suite compiles in ~30 seconds
- Test execution takes <1 second
- Each test module builds independently
- Lake caches successfully built modules

## Troubleshooting

### "bad import" errors

Ensure all imported modules are declared in `lakefile.lean`:

```lean
lean_lib Maith
lean_lib Tests
lean_exe tests where
  root := `Tests.Main
```

### "unknown module prefix" errors

Check that module imports use the full qualified name (e.g., `Maith.Polarity`).

### Linker errors

Ensure `main : IO Unit` is defined at the top level (not in a namespace).

## Architecture

```
Maith Library (Maith/ directory)
├── Core Types
│   ├── Polarity.lean
│   ├── EntityId.lean
│   ├── Entity.lean
│   ├── Attribute.lean
│   ├── RelationOp.lean
│   ├── OperationOp.lean
│   ├── Relation.lean
│   ├── Operation.lean
│   └── Graph.lean  ← Central type
├── Codec (broken)
│   ├── Encoder.lean  ✗ Syntax errors
│   └── Decoder.lean  ✗ Syntax errors
├── Utilities (broken)
│   ├── DSLHelper.lean  ✗ Duplicate decl
│   ├── Transpiler.lean  (depends on Encoder/Decoder)
│   ├── RewriteEngine.lean  ✗ Syntax errors
│   ├── Normalizer.lean  ✗ Type errors
│   └── GraphEquivalence.lean  ✗ Field errors
└── Init.lean → Imports all modules

Test Suite (Tests/ directory)
├── Harness.lean
├── ComponentTests.lean  ✓ Passing
├── EncoderTests.lean  ⚠ Placeholder
├── DecoderTests.lean  ⚠ Placeholder
├── PipelineTests.lean  ✓ Passing
└── Main.lean  ✓ Entry point
```

## References

- README.md — Project overview and motivation
- Design.md — Architectural principles
- Lean 4 Documentation — https://lean-lang.org
- Lake Documentation — https://github.com/leanprover/lake
