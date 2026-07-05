/-

CorpusPipelineTests.lean

Tests for the Mathlib → DSL → IR → Tokens → Corpus pipeline.

Validates corpus building, serialization, and statistics tracking.

-/

import Maith.CorpusBuilder
import Maith.ProcessingPipeline
import Maith.MathlibLoader
import Maith.CorpusSerializer
import Maith.MathlibCorpusBuilder
import Maith.Transpiler
import Maith.Encoder

namespace Tests.CorpusPipeline

open Lean.DSL

/--
Test data structures compile correctly.
-/
def testDataStructuresCompile : Bool :=
  let ex : TrainingExample := {
    name := "test.decl"
    module := "test.module"
    leanExpr := "λ x => x"
    graph := { entities := [], attributes := [], relations := [], operations := [] }
    tokens := ["token1", "token2"]
  }

  let stats : CorpusStats := {
    totalDeclarations := 10
    successfulExamples := 8
    failureStats := {
      transpilationFailed := 1
      irConstructionFailed := 0
      normalizationFailed := 0
      injectivityCheckFailed := 0
      encodingFailed := 1
      skippedTooLarge := 0
      skippedTacticHeavy := 0
    }
  }

  let corpus : TrainingCorpus := {
    examples := [ex]
    stats := stats
  }

  true

/--
Test pipeline result type works correctly.
-/
def testProcessingResult : Bool :=
  let ok_result : ProcessingResult String := .ok "success"
  let fail_result : ProcessingResult String := .fail "error"

  match ok_result with
  | .ok msg => msg = "success"
  | .fail _ => false

  &&

  match fail_result with
  | .ok _ => false
  | .fail msg => msg = "error"

/--
Test graph normalization functions.
-/
def testGraphNormalization : Bool :=
  let graph : Graph := {
    entities := [
      { id := EntityId.var "x", polarity := Polarity.pos },
      { id := EntityId.var "y", polarity := Polarity.neut }
    ]
    attributes := []
    relations := []
    operations := []
  }

  let normalized := pipelineNormalizeGraph graph
  normalized.entities.length = 2

/--
Test injectivity checking.
-/
def testInjectivityCheck : Bool :=
  let graph : Graph := {
    entities := [
      { id := EntityId.var "a", polarity := Polarity.pos }
    ]
    attributes := []
    relations := []
    operations := []
  }

  match checkGraphInjectivity graph with
  | .ok _ => true
  | .fail _ => false

/--
Test Mathlib loader enumeration configuration.
-/
def testEnumerationConfig : Bool :=
  let config := defaultEnumerationConfig
  config.maxSizeBytes = 100_000 &&
  config.maxTacticDensity < 1.0 &&
  config.targetModules.length > 0

/--
Test serialization configuration.
-/
def testSerializationConfig : Bool :=
  let config := defaultSerializationConfig
  config.outputDir = "./Corpus" &&
  config.corpusFile = "corpus.jsonl"

/--
Run all corpus pipeline tests.
-/
def runAllCorpusPipelineTests : IO Unit := do
  IO.println ""
  IO.println "  Testing Corpus Pipeline..."

  if testDataStructuresCompile then
    IO.println "    ✓ Data structures compile"
  else
    IO.println "    ✗ Data structures FAILED"

  if testProcessingResult then
    IO.println "    ✓ ProcessingResult type works"
  else
    IO.println "    ✗ ProcessingResult type FAILED"

  if testGraphNormalization then
    IO.println "    ✓ Graph normalization"
  else
    IO.println "    ✗ Graph normalization FAILED"

  if testInjectivityCheck then
    IO.println "    ✓ Injectivity checking"
  else
    IO.println "    ✗ Injectivity checking FAILED"

  if testEnumerationConfig then
    IO.println "    ✓ Enumeration configuration"
  else
    IO.println "    ✗ Enumeration configuration FAILED"

  if testSerializationConfig then
    IO.println "    ✓ Serialization configuration"
  else
    IO.println "    ✗ Serialization configuration FAILED"

end Tests.CorpusPipeline
