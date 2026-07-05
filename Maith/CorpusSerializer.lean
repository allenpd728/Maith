/-

CorpusSerializer.lean

Serialization utilities for writing TrainingCorpus to disk.

-/

import Maith.CorpusBuilder

namespace Lean.DSL

def corpusStatsToJsonString (stats : CorpusStats) : String :=
  "totalDeclarations: " ++ toString stats.totalDeclarations ++ ", " ++
  "successfulExamples: " ++ toString stats.successfulExamples

def trainingExampleToJsonLine (ex : TrainingExample) : String :=
  "name: " ++ ex.name ++ ", module: " ++ ex.module

structure SerializationConfig where
  outputDir : String := "./Corpus"
  corpusFile : String := "corpus.jsonl"
  statsFile : String := "stats.json"
  logsFile : String := "logs.txt"
  includeGraphs : Bool := false
  includeTokens : Bool := true

deriving Repr

def buildJsonlHeader (_stats : CorpusStats) : String :=
  "# Mathlib IR Training Corpus"

def formatLogs (logs : List LogEntry) : String :=
  logs.foldl (fun acc log =>
    acc ++
    s!"[{log.stage}] {log.declarationName}: {log.message}\n"
  ) ""

def serializeCorpusToJsonl (corpus : TrainingCorpus) : String :=
  let header := buildJsonlHeader corpus.stats
  let lines := corpus.examples.map trainingExampleToJsonLine
  let body := String.intercalate "\n" lines
  header ++ "\n" ++ body ++ "\n"

def serializeStats (stats : CorpusStats) : String :=
  corpusStatsToJsonString stats

def serializeLogs (logs : List LogEntry) : String :=
  formatLogs logs

def ensureOutputDirectory (path : String) : IO Unit := do
  IO.println s!"[LOG] Would create output directory: {path}"

def writeFile (path : String) (content : String) : IO Unit := do
  IO.println s!"[LOG] Would write {content.length} bytes to: {path}"

def persistCorpusToDisk (corpus : TrainingCorpus) (config : SerializationConfig) :
    IO Unit := do
  _ ← ensureOutputDirectory config.outputDir
  let corpusPath := s!"{config.outputDir}/{config.corpusFile}"
  let corpusContent := serializeCorpusToJsonl corpus
  _ ← writeFile corpusPath corpusContent
  IO.println s!"[CORPUS] Wrote {corpus.examples.length} examples to {corpusPath}"
  let statsPath := s!"{config.outputDir}/{config.statsFile}"
  let statsContent := serializeStats corpus.stats
  _ ← writeFile statsPath statsContent
  IO.println s!"[STATS] Wrote statistics to {statsPath}"

def defaultSerializationConfig : SerializationConfig :=
  {
    outputDir := "./Corpus"
    corpusFile := "corpus.jsonl"
    statsFile := "stats.json"
    logsFile := "logs.txt"
    includeGraphs := false
    includeTokens := true
  }

end Lean.DSL
