# Maith IR Pipeline — Testing Summary

## Current status

- `lake build tests`: passes, 0 failures
- `./.lake/build/bin/tests`: all tests pass
- Last verified: 2026-07-26

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
| Decoder | 12 |
| Graph Type | 2 |
| Normalizer | 7 |
| Injectivity | 10 |
| Problem Generator | 9 |
| Corpus Serializer | 3 |
| **Subtotal** | **62** |

Additional pipeline checks run via `Tests/CorpusPipelineTests.lean` (not counted above):
`Data structures compile`, `Transpiler formatting`, `ProcessingResult type`, `Graph normalization`,
`Injectivity checking`, `Enumeration configuration`, `Serialization configuration`,
`Forall body with arithmetic ops`, `Metadata extraction`, `Projection extraction`,
`Let expression extraction`, `HOF application extraction`, `Scoped binder injectivity`.

## Decoder tests (v1.2.0 state)

- `Decoder round-trips EntityId.bound (v0.1.0 legacy)` — legacy `b(<scope>)` format
- `Decoder round-trips BVAR_N token (v1.2.0 lambda)` — positional lambda binder IDs (`λ:BVAR_N`)
- `Decoder round-trips FVAR_N token (v1.2.0 forall)` — positional forall binder IDs (`∀:FVAR_N`)
- `Decoder round-trips TERM_N token (v1.0.0)` — positional term IDs
- `Decoder round-trips Graph with bound entity (v1.2.0 positional)` — full graph round-trip

## Golden examples fixture (v1.2.0)

`python3 python/test_golden_examples.py` — 4/4 passed.

Covers four structural cases with exact expected token sequences:
- `LeftCancelSemigroup.toIsLeftCancelMul` — forall-only binders (FVAR_N only)
- `IsAddCommutative.instDivisionAddCommMonoid` — mixed FVAR_N + BVAR_N
- `IsAddCommutative.instAddCommSemigroup._proof_1` — semantic `add` op token
- `npow_eq_pow` — EntityId.var (named constant alongside FVAR_N)

Fails immediately if the encoder changes token output. Update `GOLDEN` dict and bump
`encoderVersion` in `Corpus/stats.json` when a change is intentional.

## Remaining decoder extension

- Add one mixed-binder graph round-trip test (`FVAR_N` + `BVAR_N` in the same graph) for extra
  coverage of dual-counter behavior.

## Python corpus validation

`python/validate_roundtrip.py --all` — 2,554/2,554 passed (v1.2.0 corpus).
Reports FVAR_*/BVAR_*/TERM_* token counts in vocab snapshot.

## Files modified in most recent test update (2026-07-25 session 2)

- `Maith/Encoder.lean` — v1.2.0 FVAR/BVAR split
- `Maith/Decoder.lean` — v1.2.0 FVAR_N/BVAR_N parsing, `∀:`/`λ:` scope prefix
- `Maith/MetaExtractor.lean` — `∀:`/`λ:` scope tagging
- `python/validate_roundtrip.py` — FVAR_* token reporting
