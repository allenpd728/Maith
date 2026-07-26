# Maith IR Test Suite

## Running tests

```bash
cd ~/Projects/Maith
lake build tests
./.lake/build/bin/tests
```

## Current result

- Build: 0 failures
- Suite: all tests pass (61 counted + corpus pipeline checks)
- Last verified: 2026-07-25

## Counted test breakdown

- Polarity: 2
- EntityId: 2
- Entity: 1
- Attribute: 1
- RelationOp: 2
- Relation: 1
- OperationOp: 2
- Operation: 1
- Graph: 2
- Encoder: 5
- Decoder: 11
- Graph Type: 2
- Normalizer: 7
- Injectivity: 10
- Problem Generator: 9
- Corpus Serializer: 3

**Total: 61**

## Notes

- `Tests/CorpusPipelineTests.lean` runs 13 additional named checks outside the counted suite.
- `Tests/CorpusSerializerTests.lean` includes both pure JSON checks and an IO integration test.
- Decoder count grew from 7 → 11 with the addition of v1.0.0 BVAR/TERM round-trip tests.
