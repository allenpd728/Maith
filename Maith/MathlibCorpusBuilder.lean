/-

MathlibCorpusBuilder.lean

Top-level orchestration for the Mathlib → DSL → IR → Tokens → Corpus pipeline.

Provides the main entry point: buildMathlibIRCorpus

-/

import Maith.CorpusBuilder
import Maith.ProcessingPipeline
import Maith.MathlibLoader
import Maith.CorpusSerializer

namespace Lean.DSL

/--
Main entry point: Build a complete Mathlib IR training corpus.

Pipeline:
1. Enumerate declarations from selected Mathlib modules
2. Filter by size and tactic density
3. Process each declaration: Lean → DSL → IR → Tokens
4. Accumulate TrainingExample records
5. Build CorpusStats
6. Serialize to disk

Parameters:
- `modules`: List of Mathlib module names (e.g., ["algebra.group.basic", ...])
- `enumerationConfig`: Filtering and enumeration settings
- `serializationConfig`: Output directory and file settings
- `transpiler`: Lean↔DSL converter
- `encoder`: IR→Token converter
- `moduleSources`: Pre-loaded module declarations (name, source pairs)

Returns: IO Unit (writes corpus to disk)
-/
def buildMathlibIRCorpus
    (modules : List String)
    (enumerationConfig : EnumerationConfig := defaultEnumerationConfig)
    (serializationConfig : SerializationConfig := defaultSerializationConfig)
    (transpiler : Transpiler := Lean.DSL.defaultTranspiler)
    (encoder : Encoder := Lean.DSL.defaultEncoder)
    (moduleSources : List (String × List (String × String)) := []) :
    IO Unit := do
  IO.println "=== Mathlib IR Training Corpus Builder ==="
  IO.println s!"Target modules: {String.intercalate ", " modules}"
  IO.println ""

  -- Stage 1: Enumerate declarations from modules
  IO.println "[Stage 1] Enumerating Mathlib declarations..."
  let enumerationResults := enumerateModules modules enumerationConfig moduleSources
  let enumerationStats := buildEnumerationStats enumerationResults
  IO.println s!"Enumerated {enumerationStats.totalEnumerated} declarations"
  IO.println s!"Filtered {enumerationStats.totalFiltered} declarations (size/tactics)"
  IO.println ""

  -- Extract all declarations to process
  let allDeclarations := extractAllDeclarations enumerationResults
  IO.println s!"Processing {allDeclarations.length} declarations..."
  IO.println ""

  -- Stage 2-9: Process all declarations through the pipeline
  IO.println "[Stage 2-9] Running processing pipeline..."
  let processingResult := processBatch allDeclarations transpiler encoder
  match processingResult with
  | .fail msg =>
    IO.println s!"[ERROR] Pipeline failed: {msg}"
    pure ()
  | .ok (examples, stats) =>
    IO.println s!"[SUCCESS] Processed {stats.successfulExamples} declarations"
    let tokenMsg := s!"[STATS] Token range: {stats.tokenDistribution.minLength}" ++
                    s!" - {stats.tokenDistribution.maxLength}" ++
                    s!" (avg: {stats.tokenDistribution.avgLength})"
    IO.println tokenMsg
    IO.println ""

    -- Build final corpus
    let corpus : TrainingCorpus := {
      examples
      stats := {
        stats with
        totalDeclarations := enumerationStats.totalEnumerated
        mathlibCommitHash := "unknown"
        encoderVersion := "0.1.0"
        irVersion := "0.1.0"
      }
    }

    -- Stage 10: Serialize to disk
    IO.println "[Stage 10] Serializing corpus to disk..."
    _ ← persistCorpusToDisk corpus serializationConfig
    IO.println ""
    IO.println "=== Corpus building complete! ==="

/--
Convenient overload for building corpus with standard Mathlib modules.

Uses default enumeration and serialization configs.
-/
def buildMathlibIRCorpusStandard
    (transpiler : Transpiler := Lean.DSL.defaultTranspiler)
    (encoder : Encoder := Lean.DSL.defaultEncoder)
    (moduleSources : List (String × List (String × String)) := []) :
    IO Unit :=
  buildMathlibIRCorpus
    standardMathlibModules
    defaultEnumerationConfig
    defaultSerializationConfig
    transpiler
    encoder
    moduleSources

/--
Build corpus with custom modules and default settings.
-/
def buildMathlibIRCorpusCustomModules
    (modules : List String)
    (transpiler : Transpiler := Lean.DSL.defaultTranspiler)
    (encoder : Encoder := Lean.DSL.defaultEncoder) :
    IO Unit :=
  buildMathlibIRCorpus
    modules
    defaultEnumerationConfig
    defaultSerializationConfig
    transpiler
    encoder
    []

/--
Detailed execution trace for debugging and metrics.

Logs each stage's input/output for troubleshooting.
-/
structure ExecutionTrace where
  (stage : PipelineStage)
  (inputCount : Nat)
  (outputCount : Nat)
  (failureCount : Nat)
  (averageProcessingTimeMs : Float := 0.0)

deriving Repr

/--
Build a detailed execution trace during corpus building.

Useful for performance profiling and debugging.
-/
def buildMathlibIRCorpusWithTrace
    (modules : List String)
    (enumerationConfig : EnumerationConfig := defaultEnumerationConfig)
    (serializationConfig : SerializationConfig := defaultSerializationConfig)
    (transpiler : Transpiler := Lean.DSL.defaultTranspiler)
    (encoder : Encoder := Lean.DSL.defaultEncoder) :
    IO Unit := do
  IO.println "=== Mathlib IR Training Corpus Builder (with Trace) ==="
  IO.println s!"Target modules: {String.intercalate ", " modules}"
  IO.println ""

  -- Trace: Enumeration
  IO.println "[TRACE] Stage 1: Enumeration"
  let enumerationResults := enumerateModules modules enumerationConfig []
  let enumerationStats := buildEnumerationStats enumerationResults
  IO.println s!"  Input:  0 (enumerate from environment)"
  IO.println s!"  Output: {enumerationStats.totalEnumerated}"
  IO.println s!"  Failed: {enumerationStats.totalFiltered} (filtered)"
  IO.println ""

  -- Extract all declarations
  let allDeclarations := extractAllDeclarations enumerationResults

  -- Trace: Full pipeline
  IO.println "[TRACE] Stages 2-9: Processing Pipeline"
  let processingResult := processBatch allDeclarations transpiler encoder
  match processingResult with
  | .fail msg =>
    IO.println s!"  ERROR: {msg}"
    pure ()
  | .ok (examples, stats) =>
    IO.println s!"  Input:  {allDeclarations.length}"
    IO.println s!"  Output: {stats.successfulExamples}"
    let totalFailed := stats.failureStats.transpilationFailed + stats.failureStats.irConstructionFailed + stats.failureStats.encodingFailed
    IO.println s!"  Failed: {totalFailed}"
    IO.println ""

    -- Trace: Serialization
    IO.println "[TRACE] Stage 10: Serialization"
    let corpus : TrainingCorpus := {
      examples
      stats := {
        stats with
        totalDeclarations := enumerationStats.totalEnumerated
      }
    }
    _ ← persistCorpusToDisk corpus serializationConfig
    IO.println s!"  Wrote {examples.length} examples"
    IO.println s!"  Wrote stats and logs"
    IO.println ""

    IO.println "=== Trace complete! ==="

/--
Summary report of corpus building results.
-/
structure CorpusBuildReport where
  (totalDeclarationsEnumerated : Nat)
  (successfulExamples : Nat)
  (successRate : Float)
  (failuresByStage : FailureStats)
  (tokenStats : TokenDistribution)
  (outputDirectory : String)

deriving Repr

/--
Generate a report from corpus building results.
-/
def buildReport (corpus : TrainingCorpus) (outputDir : String) :
    CorpusBuildReport :=
  let total := corpus.stats.totalDeclarations
  let successful := corpus.stats.successfulExamples
  let rate := if total = 0 then 0.0
              else Float.ofNat successful / Float.ofNat total
  {
    totalDeclarationsEnumerated := total
    successfulExamples := successful
    successRate := rate
    failuresByStage := corpus.stats.failureStats
    tokenStats := corpus.stats.tokenDistribution
    outputDirectory := outputDir
  }

/--
Print a human-readable report.
-/
def printCorpusBuildReport (report : CorpusBuildReport) : IO Unit := do
  IO.println "=== Corpus Build Report ==="
  IO.println s!"Total declarations enumerated: {report.totalDeclarationsEnumerated}"
  IO.println s!"Successful examples: {report.successfulExamples}"
  IO.println s!"Success rate: {(report.successRate * 100).toUInt32}%"
  IO.println ""
  IO.println "Failures by stage:"
  IO.println s!"  Transpilation:   {report.failuresByStage.transpilationFailed}"
  IO.println s!"  IR Construction: {report.failuresByStage.irConstructionFailed}"
  IO.println s!"  Normalization:   {report.failuresByStage.normalizationFailed}"
  IO.println s!"  Injectivity:     {report.failuresByStage.injectivityCheckFailed}"
  IO.println s!"  Encoding:        {report.failuresByStage.encodingFailed}"
  IO.println s!"  Skipped (size):  {report.failuresByStage.skippedTooLarge}"
  IO.println s!"  Skipped (tactics): {report.failuresByStage.skippedTacticHeavy}"
  IO.println ""
  IO.println "Token statistics:"
  IO.println s!"  Min length:   {report.tokenStats.minLength}"
  IO.println s!"  Max length:   {report.tokenStats.maxLength}"
  IO.println s!"  Average:      {report.tokenStats.avgLength}"
  IO.println s!"  Total tokens: {report.tokenStats.totalTokens}"
  IO.println ""
  IO.println s!"Output directory: {report.outputDirectory}"
  IO.println "=== End Report ==="

end Lean.DSL
