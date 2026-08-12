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

def transpileLeanToGraph (decl : ExtractedDeclaration) (bucketMode : BucketMode := .module) : ProcessingResult Graph :=
  extractGraphFromDeclaration decl bucketMode

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

def processDeclaration (decl : ExtractedDeclaration) (encoder : Encoder) (bucketMode : BucketMode := .module) :
    ProcessingResult TrainingExample :=
  match transpileLeanToGraph decl bucketMode with
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

/--
Variant of processDeclaration that, when `traceDecl` matches this declaration's
name, prints the raw elaborated Expr before any IR extraction runs.

Three representations are printed:
  [TRACE:leanExpr]   — `toString decl.info.type`, the same string stored in
                        corpus.jsonl.  Readable but loses universe/mvar detail.
  [TRACE:dbgToString] — `Expr.dbgToString decl.info.type`, the internal Lean
                        kernel representation with de Bruijn indices and
                        universe levels fully explicit.
  [TRACE:reprStr]    — `reprStr decl.info.type`, Lean's Repr instance for Expr,
                        showing the exact constructor tree (Expr.forallE,
                        Expr.app, Expr.const, etc.).

After printing the trace the declaration is processed normally and its result
is returned, so --trace can be used without suppressing corpus output.
-/
def processDeclarationWithTrace
    (decl : ExtractedDeclaration) (encoder : Encoder) (traceDecl : Option String) :
    IO (ProcessingResult TrainingExample) := do
  match traceDecl with
  | some name =>
    if decl.name.toString == name then do
      IO.println s!"\n=== EXPR TRACE: {name} ==="
      IO.println "[TRACE:leanExpr]"
      IO.println (toString decl.info.type)
      IO.println ""
      IO.println "[TRACE:dbgToString]"
      IO.println (Expr.dbgToString decl.info.type)
      IO.println ""
      IO.println "[TRACE:reprStr]"
      IO.println (reprStr decl.info.type)
      IO.println "=== END TRACE ==="
      pure (processDeclaration decl encoder)
    else
      pure (processDeclaration decl encoder)
  | none => pure (processDeclaration decl encoder)

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

def processBatch (declarations : List ExtractedDeclaration) (encoder : Encoder) (bucketMode : BucketMode := .module) :
    ProcessingResult (List TrainingExample × CorpusStats) :=
  let rec loop (remaining : List ExtractedDeclaration)
      (examples : List TrainingExample) (stats : CorpusStats) (bucketMode : BucketMode := .module) :
      ProcessingResult (List TrainingExample × CorpusStats) :=
    match remaining with
    | [] =>
      -- Compute token and graph statistics from the collected examples.
      let tokenLengths  : List Nat := examples.map (·.tokens.length)
      let totalTokens   : Nat      := tokenLengths.foldl (· + ·) 0
      let minLength     : Nat      := tokenLengths.foldl Nat.min (tokenLengths.headD 0)
      let maxLength     : Nat      := tokenLengths.foldl Nat.max 0
      let n             : Float    := Float.ofNat examples.length
      let avgLength     : Float    :=
        if examples.isEmpty then 0.0 else Float.ofNat totalTokens / n
      let entityCounts  : List Nat := examples.map (·.graph.entities.length)
      let attrCounts    : List Nat := examples.map (·.graph.attributes.length)
      let relCounts     : List Nat := examples.map (·.graph.relations.length)
      let opCounts      : List Nat := examples.map (·.graph.operations.length)
      let graphSizes    : List Nat :=
        List.zipWith (· + ·)
          (List.zipWith (· + ·) entityCounts attrCounts)
          (List.zipWith (· + ·) relCounts opCounts)
      let maxGraphSize  : Nat      := graphSizes.foldl Nat.max 0
      let sumNat (counts : List Nat) : Nat := counts.foldl (· + ·) 0
      let avgOf (counts : List Nat) : Float :=
        if examples.isEmpty then 0.0 else Float.ofNat (sumNat counts) / n
      let finalStats := { stats with
        tokenDistribution := {
          minLength  := minLength
          maxLength  := maxLength
          avgLength  := avgLength
          totalTokens := totalTokens
        }
        graphStats := {
          avgEntities   := avgOf entityCounts
          avgAttributes := avgOf attrCounts
          avgRelations  := avgOf relCounts
          avgOperations := avgOf opCounts
          maxGraphSize  := maxGraphSize
        }
        -- Per-module success counts derived from example metadata.
        moduleStats :=
          let modules := (examples.map (·.module)).eraseDups
          modules.map fun m =>
            let modExamples := examples.filter (·.module == m)
            { moduleName := m
              totalDeclarations := modExamples.length  -- approximation: only counts successes
              successfulExamples := modExamples.length }
      }
      ProcessingResult.ok (examples, finalStats)
    | decl :: rest =>
      let result := processDeclaration decl encoder bucketMode
      let updatedExamples := match result with
        | ProcessingResult.ok ex => examples ++ [ex]
        | ProcessingResult.fail _ => examples
      let updatedStats := updateCorpusStats stats result
      loop rest updatedExamples updatedStats bucketMode

  let initialStats : CorpusStats := { totalDeclarations := declarations.length }
  loop declarations [] initialStats bucketMode

end Lean.DSL
