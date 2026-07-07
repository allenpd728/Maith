# Latest Fixes - July 5, 2026

## Summary

The IR pipeline now round-trips real graphs, writes corpus files to disk, and passes the checked test suite.

## Verified changes

- `Maith/Transpiler.lean`
  - Parses the Lean-like graph text emitted by `toLeanGraph`
  - Round-trip test added for a representative graph

- `Maith/Decoder.lean`
  - Reconstructs graphs from token streams instead of returning an empty graph
  - Round-trip test added for non-empty graphs

- `Maith/MathlibLoader.lean`
  - Preserves declaration source text through extraction

- `Maith/ProcessingPipeline.lean`
  - Uses `Maith.Normalizer.normalizeGraph` directly

- `Maith/CorpusSerializer.lean`
  - Writes `corpus.jsonl`, `stats.json`, and `logs.txt` to disk
  - Serializes full JSON payloads, including tokens and optional graph data

- `Maith/ProblemGenerator.lean`
  - Rejects `a = 0`
  - Generates only solvable linear-equation problems

## Test status

- `lake build tests`: success
- Build jobs: **64**
- `./.lake/build/bin/tests`: success
- Counted tests passing: **54/54**
- Additional corpus pipeline and serializer checks: passing

## Current scope

- The pipeline is functional for supplied module-source lists.
- Automatic live Lean environment enumeration is still manual.
