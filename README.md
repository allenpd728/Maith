# Maith IR Pipeline

## Status

- `lake build tests` succeeded on 2026-07-05.
- Build completed with **64 jobs** and **0 failures**.
- `./.lake/build/bin/tests` completed with **54/54 counted tests passing**.
- The suite also runs corpus-pipeline validation checks plus an IO regression that writes `corpus.jsonl`, `stats.json`, and `logs.txt` to disk.
- The build emits a small number of deprecation warnings from the existing string-trim helpers, but they do not block the build.

## Verified fixes on 2026-07-05

- `Maith/Examples.lean`: fixed Lean 3-style list syntax in `mkGraph`.
- `Maith/Cirriculum.lean`: added the missing imports for `Maith.Normalizer`, `Maith.Transpiler`, and `Maith.RewriteEngine`.
- `Maith/Transpiler.lean`: implemented Lean-like graph parsing and round-tripping.
- `Maith/Decoder.lean`: implemented token → graph reconstruction and round-trip tests.
- `Maith/ProblemGenerator.lean`: now rejects `a = 0` and only emits solvable problems.
- `Maith/CorpusSerializer.lean`: now writes real JSON/JSONL output to disk.
- `Maith/MathlibLoader.lean`: preserves declaration source text through extraction.
- `Maith/ProcessingPipeline.lean`: delegates normalization to `Maith.Normalizer`.

## Known issues resolved

| Date | File | Resolution |
| --- | --- | --- |
| 2026-07-05 | `Maith/Examples.lean` | Replaced escaped list syntax with valid Lean 4 syntax. |
| 2026-07-05 | `Maith/Cirriculum.lean` | Added missing imports and removed corrupted field-access text. |
| 2026-07-05 | `Maith/Transpiler.lean` | Replaced escaped brackets / corrupted field accesses and removed self-reference. |
| 2026-07-05 | `Maith/GraphEquivalence.lean` | Verified boolean equality helpers compile against the imported normalizer and rewrite pipeline. |

## Architecture

`Lean source -> Transpiler -> DSL -> IR Graph -> Normalizer -> Encoder -> Tokens -> Training corpus`

Supporting modules:

- `RewriteEngine` and `RewritePipeline` for graph rewriting.
- `GraphEquivalence` for structural, normalized, and rewrite-closure comparisons.
- `Examples`, `Cirriculum`, and `ProblemGenerator` for training data generation.

## Known limitations

- `buildMathlibIRCorpus` is functional when given module source tuples, but the repo still does not enumerate the live Lean environment automatically.
- The synthetic `Maith.ProblemGenerator` corpus remains the easiest zero-setup training source.

## Directory structure

```text
Maith/
  Polarity.lean
  EntityId.lean
  Entity.lean
  Attribute.lean
  RelationOp.lean
  OperationOp.lean
  Relation.lean
  Operation.lean
  Graph.lean
  Token.lean
  Encoder.lean
  Decoder.lean
  Normalizer.lean
  Transpiler.lean
  RewriteEngine.lean
  RewritePipeline.lean
  GraphEquivalence.lean
  TrainingCorpus.lean
  CorpusBuilder.lean
  CorpusSerializer.lean
  ProcessingPipeline.lean
  ProblemGenerator.lean
  MathlibLoader.lean
  MathlibCorpusBuilder.lean
  DSLHelper.lean
  Examples.lean
  TokenSpec.lean
  Cirriculum.lean
  Init.lean
Tests/
  ...
```

## Reproducing the verified result

```bash
cd /Users/philipallen/Desktop/Maith
lake clean
lake build tests
./.lake/build/bin/tests
```
