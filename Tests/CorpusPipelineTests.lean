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
import Maith.MetaExtractor

namespace Tests.CorpusPipeline

open Lean.DSL

/--
Test data structures compile correctly.
-/
def testDataStructuresCompile : Bool :=
  let ex : TrainingExample := {
    name := "test.decl"
    module := "test.module"
    leanExpr := "Nat"
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

  let _corpus : TrainingCorpus := {
    examples := [ex]
    stats := stats
  }

  ex.tokens.length = 2 && _corpus.stats.successfulExamples = 8

/--
Test that the transpiler rendering still emits graph markers.
-/
def testTranspilerFormatting : Bool :=
  let graph : Graph := {
    entities := [
      { id := EntityId.var "x", polarity := Polarity.pos },
      { id := EntityId.term 2, polarity := Polarity.neut }
    ]
    attributes := [
      { target := EntityId.var "x", key := "value", value := "42", polarity := Polarity.pos }
    ]
    relations := [
      { src := EntityId.var "x", tgt := EntityId.term 2, op := RelationOp.eq, polarity := Polarity.neg }
    ]
    operations := [
      { inputs := [EntityId.var "x", EntityId.term 2], output := EntityId.var "y", op := OperationOp.add, polarity := Polarity.pos }
    ]
  }
  let rendered := defaultTranspiler.toLeanGraph graph
  rendered.contains "-- GRAPH BEGIN" && rendered.contains "-- GRAPH END"

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
      { id := EntityId.term 2, polarity := Polarity.neg },
      { id := EntityId.var "a", polarity := Polarity.pos },
      { id := EntityId.var "a", polarity := Polarity.neut }
    ]
    attributes := [
      { target := EntityId.term 1, key := "z", value := "2", polarity := Polarity.neg },
      { target := EntityId.var "a", key := "a", value := "1", polarity := Polarity.pos }
    ]
    relations := [
      { src := EntityId.term 3, tgt := EntityId.var "b", op := RelationOp.gt, polarity := Polarity.neg },
      { src := EntityId.var "a", tgt := EntityId.var "c", op := RelationOp.eq, polarity := Polarity.pos }
    ]
    operations := [
      { inputs := [EntityId.var "b", EntityId.var "a"], output := EntityId.term 0, op := OperationOp.add, polarity := Polarity.neg }
    ]
  }

  let normalized := pipelineNormalizeGraph graph
  normalized.entities.map (·.id) = [EntityId.var "a", EntityId.var "a", EntityId.term 2] &&
  (match normalized.attributes.head? with
   | some a => a.target = EntityId.var "a"
   | none => false) &&
  (match normalized.relations.head? with
   | some r => r.src = EntityId.var "a"
   | none => false) &&
  (match normalized.operations.head? with
   | some o => o.inputs = [EntityId.var "a", EntityId.var "b"]
   | none => false)

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
Test that a `∀ x y : Nat, x + y = y + x` Expr produces a non-trivial graph
with ≥ 2 operations (HAdd.hAdd) and ≥ 1 relation (Eq) — confirming that
forallE recursion reaches a real arithmetic body and produces structural IR,
not just a resolved binder chain.
This is the "lam/forall body is non-trivial" acceptance check.
-/
def testForallBodyWithOps : Bool :=
  -- Build the Expr for ∀ (x : Nat) (y : Nat), x + y = y + x manually.
  -- Indices in the body: bvar(0)=y, bvar(1)=x (De Bruijn order, innermost first).
  let natConst  := Lean.Expr.const `Nat []
  let hAdd      := Lean.Expr.const `HAdd.hAdd []
  let eqConst   := Lean.Expr.const `Eq []
  let xBvar     := Lean.Expr.bvar 1   -- x after pushing both binders
  let yBvar     := Lean.Expr.bvar 0   -- y is innermost
  -- x + y  =  app(app(HAdd.hAdd, x), y)  ; takeLast 2 args = [x, y] ✓
  let addXY := Lean.Expr.app (Lean.Expr.app hAdd xBvar) yBvar
  -- y + x
  let addYX := Lean.Expr.app (Lean.Expr.app hAdd yBvar) xBvar
  -- Eq Nat (x+y) (y+x) — getAppArgs.length=3, Eq branch takes last 2 = [addXY, addYX] ✓
  let eqExpr    := Lean.Expr.app (Lean.Expr.app (Lean.Expr.app eqConst natConst) addXY) addYX
  let forallY   := Lean.Expr.forallE `y natConst eqExpr .default
  let forallX   := Lean.Expr.forallE `x natConst forallY .default

  match graphFromExpr "testDecl" forallX with
  | .fail msg => dbg_trace "testForallBodyWithOps FAIL: {msg}"; false
  | .ok g =>
    -- Must have at least 2 operations (addXY and addYX) and 1 relation (Eq).
    g.operations.length ≥ 2 &&
    g.relations.length  ≥ 1 &&
    -- The entity IDs for x and y must be distinct scoped bound entities.
    g.entities.any (fun e => match e.id with | .bound s => s.contains "/x" | _ => false) &&
    g.entities.any (fun e => match e.id with | .bound s => s.contains "/y" | _ => false)

/--
Test metadata extraction on a real inline `ConstantInfo`.
-/
def testCreateDeclarationMetadata : Bool :=
  let natType := Lean.Expr.const `Nat []
  let defnVal : Lean.DefinitionVal := {
    name        := `myTestDecl
    levelParams := []
    type        := natType
    value       := natType
    hints       := .abbrev
    safety      := .safe
  }
  let decl : ExtractedDeclaration := {
    name   := `myTestDecl
    module := "Test.Module"
    info   := .defnInfo defnVal
  }
  let metadata := createDeclarationMetadata decl
  metadata.name   = "myTestDecl"   &&
  metadata.module = "Test.Module"  &&
  metadata.sizeBytes > 0           &&
  !metadata.isProof                &&
  !metadata.isInductive

/--
Two declarations that both introduce a binder named `x` must produce
different encoded token sequences — the scoped `EntityId.bound` encoding
must embed the declaration name so De Bruijn-0 from `DeclA` never collides
with De Bruijn-0 from `DeclB`.
-/
def testScopedBinderInjectivity : Bool :=
  -- ∀ (x : Prop), x  — same Expr for both declarations
  let propType  := Lean.Expr.sort Lean.Level.zero
  let xBody     := Lean.Expr.bvar 0
  let forallExpr := Lean.Expr.forallE `x propType xBody .default

  match graphFromExpr "DeclA" forallExpr, graphFromExpr "DeclB" forallExpr with
  | .ok gA, .ok gB =>
    let eidsA := gA.entities.map (·.id)
    let eidsB := gB.entities.map (·.id)
    -- Entity ID lists must differ (different scoped binder names).
    eidsA ≠ eidsB &&
    -- Both graphs must be non-empty (extraction succeeded).
    gA.entities.length ≥ 1 &&
    gB.entities.length ≥ 1 &&
    -- Encoded token sequences must differ.
    encodeGraph gA ≠ encodeGraph gB
  | _, _ => false

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

  if testTranspilerFormatting then
    IO.println "    ✓ Transpiler formatting"
  else
    IO.println "    ✗ Transpiler formatting FAILED"

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

  if testForallBodyWithOps then
    IO.println "    ✓ Forall body with arithmetic ops (non-trivial graph)"
  else
    IO.println "    ✗ Forall body with arithmetic ops FAILED"

  if testCreateDeclarationMetadata then
    IO.println "    ✓ Metadata extraction (real assertions)"
  else
    IO.println "    ✗ Metadata extraction FAILED"

  if testScopedBinderInjectivity then
    IO.println "    ✓ Scoped binder injectivity"
  else
    IO.println "    ✗ Scoped binder injectivity FAILED"

end Tests.CorpusPipeline
