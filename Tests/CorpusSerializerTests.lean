/-
Tests.CorpusSerializerTests

Regression tests for corpus serialization and disk persistence.
-/

import Tests.Harness
import Maith.CorpusSerializer

namespace Tests.CorpusSerializer

open Tests
open Lean.DSL

def testJsonEscaping : TestResult :=
  let config : SerializationConfig := { includeGraphs := true, includeTokens := true }
  let ex : TrainingExample := {
    name := "decl\"1"
    module := "mod\\ule"
    leanExpr := "line1\nline2"
    graph := { entities := [], attributes := [], relations := [], operations := [] }
    tokens := ["tok\"1", "tok\\2"]
  }
  let line := trainingExampleToJsonLine config ex
  runTest "Serializer escapes JSON strings" (
    line.contains "\\\"" && line.contains "\\n" && line.contains "\\\\"
  ) line

def testStatsSerialization : TestResult :=
  let stats : CorpusStats := {
    totalDeclarations := 3
    successfulExamples := 2
    failureStats := { transpilationFailed := 1, irConstructionFailed := 0, normalizationFailed := 0, injectivityCheckFailed := 0, encodingFailed := 0, skippedTooLarge := 0, skippedTacticHeavy := 0 }
    tokenDistribution := { minLength := 1, maxLength := 4, avgLength := 2.5, totalTokens := 8 }
    graphStats := { avgEntities := 1.0, avgAttributes := 0.0, avgRelations := 0.0, avgOperations := 0.0, maxGraphSize := 1 }
    mathlibCommitHash := "abc123"
    encoderVersion := "1.0"
    irVersion := "1.0"
  }
  let json := corpusStatsToJsonString stats
  runTest "Serializer emits full stats JSON" (
    json.contains "\"failureStats\"" &&
    json.contains "\"tokenDistribution\"" &&
    json.contains "\"graphStats\"" &&
    json.contains "\"mathlibCommitHash\""
  ) json

def testPersistCorpusToDisk : IO TestResult := do
  let outputDir := "/tmp/maith-corpus-serializer-test"
  let config : SerializationConfig := {
    outputDir := outputDir
    corpusFile := "corpus.jsonl"
    statsFile := "stats.json"
    logsFile := "logs.txt"
    includeGraphs := true
    includeTokens := true
  }
  let corpus : TrainingCorpus := {
    examples := [{
      name := "example.decl"
      module := "example.module"
      leanExpr := "example"
      graph := {
        entities := [{ id := EntityId.var "x", polarity := Polarity.pos }]
        attributes := [{ target := EntityId.var "x", key := "value", value := "42", polarity := Polarity.pos }]
        relations := []
        operations := []
      }
      tokens := ["GRAPH_BEGIN", "E", "x", "pos", "GRAPH_END"]
    }]
    stats := {
      totalDeclarations := 1
      successfulExamples := 1
    }
  }
  persistCorpusToDisk corpus config
  let corpusPath := s!"{outputDir}/{config.corpusFile}"
  let statsPath := s!"{outputDir}/{config.statsFile}"
  let logsPath := s!"{outputDir}/{config.logsFile}"
  let corpusContent ← IO.FS.readFile corpusPath
  let statsContent ← IO.FS.readFile statsPath
  let logsContent ← IO.FS.readFile logsPath
  pure <| runTest "persistCorpusToDisk writes non-empty files" (
    corpusContent.length > 0 &&
    statsContent.length > 0 &&
    logsContent.length > 0 &&
    corpusContent.contains "\"tokens\"" &&
    corpusContent.contains "\"graph\"" &&
    statsContent.contains "\"successfulExamples\""
  ) s!"{corpusContent.length} / {statsContent.length} / {logsContent.length}"

def corpusSerializerTests : List TestResult := [
  testJsonEscaping,
  testStatsSerialization
]

def runAllCorpusSerializerTests : IO Unit := do
  runTestSuite "Corpus Serializer Tests" corpusSerializerTests
  let result ← testPersistCorpusToDisk
  printTestResult result

end Tests.CorpusSerializer
