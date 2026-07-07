/-
ProcessingPipeline.lean

Pipeline stages for converting elaborated Lean declarations → IR → Tokens.
-/

import Maith.CorpusBuilder
import Maith.Graph
import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.Encoder
import Maith.EntityId
import Maith.Normalizer
import Maith.MetaExtractor

namespace Lean.DSL

def transpileLeanToGraph (decl : ExtractedDeclaration) : ProcessingResult Graph :=
  extractGraphFromDeclaration decl

def canonicalizeEntities (entities : List Entity) : List Entity :=
  normalizeEntities entities

def canonicalizeAttributes (attributes : List Attribute) : List Attribute :=
  normalizeAttributes attributes

def canonicalizeRelations (relations : List Relation) : List Relation :=
  normalizeRelations relations

def canonicalizeOperations (operations : List Operation) : List Operation :=
  normalizeOperations operations

def pipelineNormalizeGraph (graph : Graph) : Graph :=
  normalizeGraph graph

def deduplicateIds (ids : List EntityId) : List EntityId :=
  ids.foldl (fun acc id =>
    if acc.contains id then acc else acc ++ [id]
  ) []

def checkGraphInjectivity (graph : Graph) : ProcessingResult Graph :=
  let ids := graph.entities.map (·.id)
  let uniqueIds := deduplicateIds ids
  if List.length ids = List.length uniqueIds then
    ProcessingResult.ok graph
  else
    ProcessingResult.fail "injectivity: Graph has duplicate entity IDs"

def encodeGraphToTokens (graph : Graph) (encoder : Encoder) :
    ProcessingResult (List Token) :=
  let tokens := encoder.encodeGraph graph
  ProcessingResult.ok tokens

def processDeclaration (decl : ExtractedDeclaration) (encoder : Encoder) :
    ProcessingResult TrainingExample :=
  match transpileLeanToGraph decl with
  | ProcessingResult.fail msg => ProcessingResult.fail msg
  | ProcessingResult.ok g =>
    let normalizedGraph := pipelineNormalizeGraph g
    match checkGraphInjectivity normalizedGraph with
    | ProcessingResult.fail msg => ProcessingResult.fail msg
    | ProcessingResult.ok checkedGraph =>
      match encodeGraphToTokens checkedGraph encoder with
      | ProcessingResult.fail msg => ProcessingResult.fail s!"encoding: {msg}"
      | ProcessingResult.ok tokens =>
        ProcessingResult.ok {
          name := decl.name.toString
          module := decl.module
          leanExpr := toString decl.info.type
          graph := checkedGraph
          tokens := tokens
        }

private def upsertFailureReason (reasons : List (String × Nat)) (reason : String) :
    List (String × Nat) :=
  let rec go (remaining : List (String × Nat)) (acc : List (String × Nat)) :=
    match remaining with
    | [] => acc ++ [(reason, 1)]
    | (r, count) :: rest =>
      if r = reason then
        acc ++ [(r, count + 1)] ++ rest
      else
        go rest (acc ++ [(r, count)])
  go reasons []

private def updateFailureStats (stats : FailureStats) (msg : String) : FailureStats :=
  let withReason := { stats with failureReasons := upsertFailureReason stats.failureReasons msg }
  if msg.startsWith "unsupported:" || msg.startsWith "type extraction failed:" ||
      msg.startsWith "value extraction failed:" then
    { withReason with irConstructionFailed := withReason.irConstructionFailed + 1 }
  else if msg.startsWith "injectivity:" then
    { withReason with injectivityCheckFailed := withReason.injectivityCheckFailed + 1 }
  else if msg.startsWith "encoding:" then
    { withReason with encodingFailed := withReason.encodingFailed + 1 }
  else
    { withReason with transpilationFailed := withReason.transpilationFailed + 1 }

def updateCorpusStats (stats : CorpusStats) (result : ProcessingResult TrainingExample) :
    CorpusStats :=
  match result with
  | ProcessingResult.ok _ =>
    { stats with successfulExamples := stats.successfulExamples + 1 }
  | ProcessingResult.fail msg =>
    { stats with failureStats := updateFailureStats stats.failureStats msg }

def processBatch (declarations : List ExtractedDeclaration) (encoder : Encoder) :
    ProcessingResult (List TrainingExample × CorpusStats) :=
  let rec loop (remaining : List ExtractedDeclaration)
      (examples : List TrainingExample) (stats : CorpusStats) :
      ProcessingResult (List TrainingExample × CorpusStats) :=
    match remaining with
    | [] => ProcessingResult.ok (examples, stats)
    | decl :: rest =>
      let result := processDeclaration decl encoder
      let updatedExamples := match result with
        | ProcessingResult.ok ex => examples ++ [ex]
        | ProcessingResult.fail _ => examples
      let updatedStats := updateCorpusStats stats result
      loop rest updatedExamples updatedStats

  let initialStats : CorpusStats := { totalDeclarations := declarations.length }
  loop declarations [] initialStats

end Lean.DSL
