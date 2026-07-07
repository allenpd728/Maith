/-
MathlibLoader.lean

Enumerate declarations from a real loaded Lean environment.
-/

import Maith.CorpusBuilder
import Maith.MetaExtractor

namespace Lean.DSL

/--
Metadata about an environment declaration for filtering and preprocessing.
-/
structure DeclarationMetadata where
  name : String
  module : String
  sizeBytes : Nat := 0
  tacticDensity : Float := 0.0
  isProof : Bool := false
  isStructure : Bool := false
  isInductive : Bool := false

deriving Repr

/--
Configuration for filtering declarations during enumeration.
-/
structure EnumerationConfig where
  maxSizeBytes : Nat := 100_000
  maxTacticDensity : Float := 0.8
  includeProofs : Bool := true
  includeDefinitions : Bool := true
  includeStructures : Bool := true
  includeInductives : Bool := true
  targetModules : List String := []

deriving Repr

/--
Result of enumerating a single module.
-/
structure EnumerationResult where
  module : String
  declCount : Nat := 0
  declarations : List ExtractedDeclaration := []
  filtered : Nat := 0
  errors : List String := []

private def declarationKindFlags (info : ConstantInfo) : Bool × Bool × Bool :=
  match info with
  | .thmInfo _ => (true, false, false)
  | .inductInfo _ => (false, false, true)
  | .ctorInfo _ => (false, true, false)
  | .recInfo _ => (false, true, false)
  | _ => (false, false, false)

def createDeclarationMetadata (decl : ExtractedDeclaration) : DeclarationMetadata :=
  let declTypeSize := toString decl.info.type |>.length
  let (isProof, isStructure, isInductive) := declarationKindFlags decl.info
  {
    name := decl.name.toString
    module := decl.module
    sizeBytes := declTypeSize
    tacticDensity := 0.0
    isProof := isProof
    isStructure := isStructure
    isInductive := isInductive
  }

def shouldIncludeDeclaration (metadata : DeclarationMetadata) (config : EnumerationConfig) :
    Bool :=
  let sizeOk := metadata.sizeBytes ≤ config.maxSizeBytes
  let tacticOk := metadata.tacticDensity ≤ config.maxTacticDensity
  let typeOk :=
    (metadata.isProof && config.includeProofs) ||
    (metadata.isStructure && config.includeStructures) ||
    (metadata.isInductive && config.includeInductives) ||
    ((!metadata.isProof && !metadata.isStructure && !metadata.isInductive) && config.includeDefinitions)
  sizeOk && tacticOk && typeOk

def enumerateModule (env : Environment) (module : String) (config : EnumerationConfig) :
    EnumerationResult :=
  let allDecls := enumerateDeclarationsFromEnvironment env [module]
  let kept :=
    allDecls.filter fun decl =>
      let metadata := createDeclarationMetadata decl
      shouldIncludeDeclaration metadata config
  {
    module := module
    declCount := allDecls.length
    declarations := kept
    filtered := allDecls.length - kept.length
  }

def enumerateModules (env : Environment) (modules : List String) (config : EnumerationConfig) :
    List EnumerationResult :=
  modules.map (fun module => enumerateModule env module config)

def extractAllDeclarations (results : List EnumerationResult) :
    List ExtractedDeclaration :=
  results.foldl (fun acc result => acc ++ result.declarations) []

structure EnumerationStats where
  totalEnumerated : Nat := 0
  totalFiltered : Nat := 0
  resultsByModule : List (String × Nat × Nat) := []
  errors : List String := []

deriving Repr

def buildEnumerationStats (results : List EnumerationResult) :
    EnumerationStats :=
  {
    totalEnumerated := results.foldl (· + ·.declCount) 0
    totalFiltered := results.foldl (· + ·.filtered) 0
    resultsByModule := results.map (fun r => (r.module, r.declCount, r.filtered))
    errors := results.foldl (· ++ ·.errors) []
  }

def standardMathlibModules : List String :=
  [
    "Mathlib.Algebra.Group.Basic"
  ]

def defaultEnumerationConfig : EnumerationConfig :=
  {
    maxSizeBytes := 100_000
    maxTacticDensity := 0.8
    includeProofs := true
    includeDefinitions := true
    includeStructures := true
    includeInductives := true
    targetModules := standardMathlibModules
  }

end Lean.DSL
