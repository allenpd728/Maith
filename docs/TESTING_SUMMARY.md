# Maith IR Pipeline — Testing Summary

## Current status

- `lake build tests`: passes, 0 failures
- `./.lake/build/bin/tests`: all tests pass
- Last verified: 2026-07-25

## Test counts

| Suite | Tests |
|-------|-------|
| Polarity | 2 |
| EntityId | 2 |
| Entity | 1 |
| Attribute | 1 |
| RelationOp | 2 |
| Relation | 1 |
| OperationOp | 2 |
| Operation | 1 |
| Graph | 2 |
| Encoder | 5 |
| Decoder | 11 |
| Graph Type | 2 |
| Normalizer | 7 |
| Injectivity | 10 |
| Problem Generator | 9 |
| Corpus Serializer | 3 |
| **Subtotal** | **61** |

Additional pipeline checks run via `Tests/CorpusPipelineTests.lean` (not counted above):
`Data structures compile`, `Transpiler formatting`, `ProcessingResult type`, `Graph normalization`,
`Injectivity checking`, `Enumeration configuration`, `Serialization configuration`,
`Forall body with arithmetic ops`, `Metadata extraction`, `Projection extraction`,
`Let expression extraction`, `HOF application extraction`, `Scoped binder injectivity`.

## Decoder tests (v1.0.0 additions)

- `Decoder round-trips EntityId.bound (v0.1.0 legacy)` — legacy `b(<scope>)` format
- `Decoder round-trips BVAR_N token (v1.0.0)` — positional bound IDs
- `Decoder round-trips TERM_N token (v1.0.0)` — positional term IDs
- `Decoder round-trips Graph with bound entity (v1.0.0 positional)` — full graph round-trip

## Files modified in most recent test update (2026-07-25)

- `Tests/DecoderTests.lean` — BVAR/TERM v1.0.0 tests
- `Tests/CorpusPipelineTests.lean` — scoped binder injectivity updated for v1.0.0 behavior
