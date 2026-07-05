/-

MathlibLoader.lean

Utilities for enumerating and extracting declarations from Mathlib modules.

Provides filtering, size checking, and declaration enumeration.

-/

import Maith.CorpusBuilder

namespace Lean.DSL

/--
Metadata about a Mathlib declaration for filtering and preprocessing.
-/
structure DeclarationMetadata :=
  (name : String)
  (module : String)
  (sizeBytes : Nat := 0)
  (tacticDensity : Float := 0.0)  -- 0.0 = no tactics, 1.0 = all tactics
  (isProof : Bool := false)
  (isStructure : Bool := false)
  (isInductive : Bool := false)

deriving Repr

/--
Configuration for filtering declarations during enumeration.
-/
structure EnumerationConfig :=
  (maxSizeBytes : Nat := 100_000)        -- Skip declarations > 100KB
  (maxTacticDensity : Float := 0.8)      -- Skip if > 80% tactics
  (includeProofs : Bool := true)
  (includeDefinitions : Bool := true)
  (includeStructures : Bool := true)
  (includeInductives : Bool := true)
  (targetModules : List String := [])

deriving Repr

/--
Result of enumerating a single Mathlib module.
-/
structure EnumerationResult :=
  (module : String)
  (declCount : Nat := 0)
  (declarations : List DeclarationMetadata := [])
  (filtered : Nat := 0)  -- Number of declarations filtered out
  (errors : List String := [])

deriving Repr

/--
Helper: Check if a declaration should be included based on metadata and config.
-/
def shouldIncludeDeclaration (metadata : DeclarationMetadata) (config : EnumerationConfig) :
    Bool :=
  let sizeOk := metadata.sizeBytes ≤ config.maxSizeBytes
  let tacticOk := metadata.tacticDensity ≤ config.maxTacticDensity
  let typeOk :=
    (metadata.isProof && config.includeProofs) ||
    (!metadata.isProof && config.includeDefinitions) ||
    (metadata.isStructure && config.includeStructures) ||
    (metadata.isInductive && config.includeInductives)
  sizeOk && tacticOk && typeOk

/--
Helper: Create a DeclarationMetadata from basic info.

This is a placeholder that will be extended with proper Lean environment API
calls to extract declaration information from the actual Lean environment.
-/
def createDeclarationMetadata (name : String) (module : String)
    (source : String) : DeclarationMetadata :=
  let sizeBytes := source.length
  -- Simple heuristic: count tactic keywords
  let tacticKeywords := ["by", "exact", "intro", "apply", "rw", "simp", "sorry"]
  let words := source.split (· = ' ')
  let tacticCount := tacticKeywords.foldl (fun count keyword =>
    count + (words.filter (fun w => toString w = keyword) |>.length)
  ) 0
  let totalKeywords := words.filter (fun w => (toString w).isEmpty = false) |>.length
  let tacticDensity := if totalKeywords = 0 then 0.0
                       else Float.ofNat tacticCount / Float.ofNat totalKeywords
  {
    name := name
    module := module
    sizeBytes := sizeBytes
    tacticDensity := tacticDensity
    isProof := source.contains "proof" || source.contains "by"
    isStructure := source.contains "structure"
    isInductive := source.contains "inductive"
  }

/--
Enumerate declarations from a single module.

This is a placeholder that will be extended with proper environment API usage.
For now, it returns an empty list with a note about needing environment access.
-/
def enumerateModule (module : String) (config : EnumerationConfig)
    (declarations : List (String × String)) :
    EnumerationResult :=
  let filtered := declarations.filter (fun (name, source) =>
    let declMeta := createDeclarationMetadata name module source
    shouldIncludeDeclaration declMeta config
  )
  let declMetas := filtered.map (fun (name, source) =>
    createDeclarationMetadata name module source
  )
  {
    module
    declCount := declarations.length
    declarations := declMetas
    filtered := declarations.length - filtered.length
  }

/--
Enumerate declarations from multiple modules.

Combines results from all target modules into a single flat list.
-/
def enumerateModules (modules : List String) (config : EnumerationConfig)
    (moduleDeclarations : List (String × List (String × String))) :
    List EnumerationResult :=
  moduleDeclarations.filter (fun (m, _) => modules.contains m) |>.map (fun (m, decls) =>
    enumerateModule m config decls
  )

/--
Extract all declarations from enumeration results into a flat list.

Returns: List (String × String × String) = (name, module, source)
-/
def extractAllDeclarations (results : List EnumerationResult) :
    List (String × String × String) :=
  results.foldl (fun acc result =>
    acc ++ result.declarations.map (fun declMeta =>
      (declMeta.name, declMeta.module, "")  -- Source would be populated from env
    )
  ) []

/--
Statistics for enumeration across all modules.
-/
structure EnumerationStats :=
  (totalEnumerated : Nat := 0)
  (totalFiltered : Nat := 0)
  (resultsByModule : List (String × Nat × Nat) := [])  -- (module, count, filtered)
  (errors : List String := [])

deriving Repr

/--
Build enumeration statistics from results.
-/
def buildEnumerationStats (results : List EnumerationResult) :
    EnumerationStats :=
  {
    totalEnumerated := results.foldl (· + ·.declCount) 0
    totalFiltered := results.foldl (· + ·.filtered) 0
    resultsByModule := results.map (fun r => (r.module, r.declCount, r.filtered))
    errors := results.foldl (· ++ ·.errors) []
  }

/--
Predefined standard Mathlib modules for corpus building.
-/
def standardMathlibModules : List String :=
  [
    "algebra.group.basic",
    "algebra.ring.basic",
    "data.nat.basic",
    "data.int.basic",
    "data.real.basic"
  ]

/--
Default enumeration config for Mathlib corpus building.
-/
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
