# Maith IR Test Suite

## Verified run

Command:

```bash
cd /Users/philipallen/Desktop/Maith
lake clean
lake build tests
./.lake/build/bin/tests
```

Result:

- `lake build tests` completed successfully.
- Build completed with **62 jobs** and **0 failures**.
- The test executable reported **54/54 counted tests passing**.
- The executable also printed **6 corpus pipeline validation checks**, and those passed.
- Lean emitted deprecation warnings from existing `structure :=` declarations during the build.

## Test breakdown

- Polarity: 3
- EntityId: 2
- Entity: 1
- Attribute: 1
- RelationOp: 3
- Relation: 1
- OperationOp: 2
- Operation: 1
- Graph: 2
- Encoder: 5
- Decoder: 6
- Graph Type: 2
- Normalizer: 7
- Injectivity: 10
- Problem Generator: 8

Subtotal: **54 counted tests**

Additional output:

- Corpus pipeline validation checks: **6 passed**

## Resolved issues

- `Maith/Examples.lean`: fixed Lean 3 list syntax.
- `Maith/Cirriculum.lean`: added missing imports.
- `Maith/Transpiler.lean`: removed corrupted field-access text and self-reference.
- `Maith/Decoder.lean`: fixed String slice handling.
- `Maith/ProblemGenerator.lean`: mapped to the actual `TrainingExample` structure.
- `Maith/GraphEquivalence.lean`: verified the current boolean equality helpers build with `Maith.Normalizer` and `Maith.RewritePipeline`.

## Test structure

```text
Tests/
  Harness.lean
  ComponentTests.lean
  EncoderTests.lean
  DecoderTests.lean
  CorpusTests.lean
  NormalizerTests.lean
  InjectivityTests.lean
  ProblemGeneratorTests.lean
  PipelineTests.lean
  CorpusPipelineTests.lean
  Main.lean
```

## Unresolved issues

None affecting the build or tests.
