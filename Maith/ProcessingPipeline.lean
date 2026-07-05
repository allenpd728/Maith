/-

ProcessingPipeline.lean

Pipeline stages for converting Lean declarations → DSL → IR → Tokens.

-/

import Maith.CorpusBuilder
import Maith.Graph
import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.Encoder
import Maith.Transpiler
import Maith.EntityId

namespace Lean.DSL

def transpileLeanToGraph (leanExpr : String) (transpiler : Transpiler) :
    ProcessingResult Graph :=
  let graph := transpiler.fromLeanGraph leanExpr
  ProcessingResult.ok graph

def canonicalizeEntities (entities : List Entity) : List Entity :=
  entities

def canonicalizeAttributes (attributes : List Attribute) : List Attribute :=
  attributes

def canonicalizeRelations (relations : List Relation) : List Relation :=
  relations

def canonicalizeOperations (operations : List Operation) : List Operation :=
  operations

def pipelineNormalizeGraph (graph : Graph) : Graph :=
  let entities := canonicalizeEntities graph.entities
  let attributes := canonicalizeAttributes graph.attributes
  let relations := canonicalizeRelations graph.relations
  let operations := canonicalizeOperations graph.operations
  ⟨entities, attributes, relations, operations⟩

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
    ProcessingResult.fail "Graph has duplicate entity IDs"

def encodeGraphToTokens (graph : Graph) (encoder : Encoder) :
    ProcessingResult (List Token) :=
  let tokens := encoder.encodeGraph graph
  ProcessingResult.ok tokens

def processDeclaration (name : String) (module : String) (leanExpr : String)
    (transpiler : Transpiler) (encoder : Encoder) :
    ProcessingResult TrainingExample :=
  match transpileLeanToGraph leanExpr transpiler with
  | ProcessingResult.fail msg => ProcessingResult.fail msg
  | ProcessingResult.ok g =>
    let normalizedGraph := pipelineNormalizeGraph g
    match checkGraphInjectivity normalizedGraph with
    | ProcessingResult.fail msg => ProcessingResult.fail msg
    | ProcessingResult.ok checkedGraph =>
      match encodeGraphToTokens checkedGraph encoder with
      | ProcessingResult.fail msg => ProcessingResult.fail msg
      | ProcessingResult.ok tokens =>
        ProcessingResult.ok {
          name := name
          module := module
          leanExpr := leanExpr
          graph := checkedGraph
          tokens := tokens
        }

def updateCorpusStats (stats : CorpusStats) (result : ProcessingResult TrainingExample)
    (_stage : PipelineStage) : CorpusStats :=
  match result with
  | ProcessingResult.ok _example =>
    { stats with successfulExamples := stats.successfulExamples + 1 }
  | ProcessingResult.fail _ =>
    stats

def processBatch (declarations : List (String × String × String))
    (transpiler : Transpiler) (encoder : Encoder) :
    ProcessingResult (List TrainingExample × CorpusStats) :=
  let rec loop (remaining : List (String × String × String))
      (examples : List TrainingExample) (stats : CorpusStats) :
      ProcessingResult (List TrainingExample × CorpusStats) :=
    match remaining with
    | [] => ProcessingResult.ok (examples, stats)
    | (name, module, expr) :: rest =>
      let result := processDeclaration name module expr transpiler encoder
      let updatedExamples := match result with
        | ProcessingResult.ok ex => examples ++ [ex]
        | ProcessingResult.fail _ => examples
      let updatedStats := updateCorpusStats stats result PipelineStage.transpilation
      loop rest updatedExamples updatedStats

  let initialStats : CorpusStats := { totalDeclarations := declarations.length }
  loop declarations [] initialStats

end Lean.DSL
