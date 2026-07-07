# Maith IR Test Suite

## Verified run

```bash
cd Maith
lake build tests
./.lake/build/bin/tests
```

## Result

- `lake build tests` completed successfully.
- Build completed with **66 jobs** and **0 failures** (clean build and incremental build).
- The main suite reported **54/54 counted tests passing**.
- Additional corpus pipeline checks passed.
- Serializer integration verified real file output.

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
- Decoder: 7
- Graph Type: 2
- Normalizer: 7
- Injectivity: 10
- Problem Generator: 9

Subtotal: **54**

## Notes

- `Tests/CorpusPipelineTests.lean` runs additional validation checks outside the counted suite.
- `Tests/CorpusSerializerTests.lean` includes both pure JSON checks and an IO integration test.
