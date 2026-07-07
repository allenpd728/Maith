/-
CorpusSerializer.lean

Serialization utilities for writing TrainingCorpus to disk.
-/

import Maith.CorpusBuilder

namespace Lean.DSL

structure SerializationConfig where
  outputDir : String := "./Corpus"
  corpusFile : String := "corpus.jsonl"
  statsFile : String := "stats.json"
  logsFile : String := "logs.txt"
  includeGraphs : Bool := false
  includeTokens : Bool := true

deriving Repr

private def jsonEscape (s : String) : String :=
  s.foldl (fun acc c =>
    acc ++
      (if c = '"' then "\\\""
       else if c = '\\' then "\\\\"
       else if c = '\n' then "\\n"
       else if c = '\r' then "\\r"
       else if c = '\t' then "\\t"
       else String.push "" c)
  ) ""

private def jsonString (s : String) : String :=
  "\"" ++ jsonEscape s ++ "\""

private def jsonArray (items : List String) : String :=
  "[" ++ String.intercalate ", " items ++ "]"

private def jsonField (name value : String) : String :=
  jsonString name ++ ": " ++ value

private def jsonObject (fields : List (String × String)) : String :=
  "{" ++ String.intercalate ", " (fields.map (fun (k, v) => jsonField k v)) ++ "}"

private def jsonEntityId (id : EntityId) : String :=
  match id with
  | .var s   => jsonObject [("kind", jsonString "var"),   ("name",  jsonString s)]
  | .term n  => jsonObject [("kind", jsonString "term"),  ("index", toString n)]
  | .bound s => jsonObject [("kind", jsonString "bound"), ("scope", jsonString s)]

private def jsonPolarity (p : Polarity) : String := jsonString (toString p)
private def jsonRelationOp (op : RelationOp) : String := jsonString (toString op)
private def jsonOperationOp (op : OperationOp) : String := jsonString (toString op)

private def jsonEntity (e : Entity) : String :=
  jsonObject [
    ("id", jsonEntityId e.id),
    ("polarity", jsonPolarity e.polarity)
  ]

private def jsonAttribute (a : Attribute) : String :=
  jsonObject [
    ("target", jsonEntityId a.target),
    ("key", jsonString a.key),
    ("value", jsonString a.value),
    ("polarity", jsonPolarity a.polarity)
  ]

private def jsonRelation (r : Relation) : String :=
  jsonObject [
    ("src", jsonEntityId r.src),
    ("tgt", jsonEntityId r.tgt),
    ("op", jsonRelationOp r.op),
    ("polarity", jsonPolarity r.polarity)
  ]

private def jsonOperation (o : Operation) : String :=
  jsonObject [
    ("inputs", jsonArray (o.inputs.map jsonEntityId)),
    ("output", jsonEntityId o.output),
    ("op", jsonOperationOp o.op),
    ("polarity", jsonPolarity o.polarity)
  ]

def corpusStatsToJsonString (stats : CorpusStats) : String :=
  jsonObject [
    ("totalDeclarations", toString stats.totalDeclarations),
    ("successfulExamples", toString stats.successfulExamples),
    ("failureStats", jsonObject [
      ("transpilationFailed", toString stats.failureStats.transpilationFailed),
      ("irConstructionFailed", toString stats.failureStats.irConstructionFailed),
      ("normalizationFailed", toString stats.failureStats.normalizationFailed),
      ("injectivityCheckFailed", toString stats.failureStats.injectivityCheckFailed),
      ("encodingFailed", toString stats.failureStats.encodingFailed),
      ("skippedTooLarge", toString stats.failureStats.skippedTooLarge),
      ("skippedTacticHeavy", toString stats.failureStats.skippedTacticHeavy),
      ("failureReasons", jsonArray (stats.failureStats.failureReasons.map (fun (reason, count) =>
        jsonObject [("reason", jsonString reason), ("count", toString count)]
      )))
    ]),
    ("tokenDistribution", jsonObject [
      ("minLength", toString stats.tokenDistribution.minLength),
      ("maxLength", toString stats.tokenDistribution.maxLength),
      ("avgLength", toString stats.tokenDistribution.avgLength),
      ("totalTokens", toString stats.tokenDistribution.totalTokens)
    ]),
    ("graphStats", jsonObject [
      ("avgEntities", toString stats.graphStats.avgEntities),
      ("avgAttributes", toString stats.graphStats.avgAttributes),
      ("avgRelations", toString stats.graphStats.avgRelations),
      ("avgOperations", toString stats.graphStats.avgOperations),
      ("maxGraphSize", toString stats.graphStats.maxGraphSize)
    ]),
    ("mathlibCommitHash", jsonString stats.mathlibCommitHash),
    ("encoderVersion", jsonString stats.encoderVersion),
    ("irVersion", jsonString stats.irVersion)
  ]

def trainingExampleToJsonLine (config : SerializationConfig) (ex : TrainingExample) : String :=
  let _ := config
  jsonObject [
    ("name", jsonString ex.name),
    ("module", jsonString ex.module),
    ("leanExpr", jsonString ex.leanExpr),
    ("graph", jsonObject [
      ("entities", jsonArray (ex.graph.entities.map jsonEntity)),
      ("attributes", jsonArray (ex.graph.attributes.map jsonAttribute)),
      ("relations", jsonArray (ex.graph.relations.map jsonRelation)),
      ("operations", jsonArray (ex.graph.operations.map jsonOperation))
    ]),
    ("tokens", jsonArray (ex.tokens.map jsonString))
  ]

def serializeLogs (logs : List LogEntry) : String :=
  logs.foldl (fun acc log =>
    acc ++ s!"[{log.stage}] {log.declarationName}: {log.message}\n"
  ) ""

def serializeCorpusToJsonl (corpus : TrainingCorpus) (config : SerializationConfig) : String :=
  let lines := corpus.examples.map (trainingExampleToJsonLine config)
  String.intercalate "\n" lines ++ if lines.isEmpty then "" else "\n"

def serializeStats (stats : CorpusStats) : String :=
  corpusStatsToJsonString stats

def ensureOutputDirectory (path : String) : IO Unit := do
  IO.println s!"[LOG] Creating output directory: {path}"
  IO.FS.createDirAll path

def writeFile (path : String) (content : String) : IO Unit := do
  IO.println s!"[LOG] Writing {content.length} bytes to: {path}"
  IO.FS.writeFile path content

def persistCorpusToDisk (corpus : TrainingCorpus) (config : SerializationConfig) :
    IO Unit := do
  _ ← ensureOutputDirectory config.outputDir
  let corpusPath := s!"{config.outputDir}/{config.corpusFile}"
  let corpusContent := serializeCorpusToJsonl corpus config
  _ ← writeFile corpusPath corpusContent
  IO.println s!"[CORPUS] Wrote {corpus.examples.length} examples to {corpusPath}"

  let statsPath := s!"{config.outputDir}/{config.statsFile}"
  let statsContent := serializeStats corpus.stats
  _ ← writeFile statsPath statsContent
  IO.println s!"[STATS] Wrote statistics to {statsPath}"

  let logsPath := s!"{config.outputDir}/{config.logsFile}"
  let logsContent := s!"Wrote {corpus.examples.length} examples\nWrote stats for {corpus.stats.successfulExamples} successful examples\n"
  _ ← writeFile logsPath logsContent
  IO.println s!"[LOGS] Wrote logs to {logsPath}"

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
