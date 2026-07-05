/-
Tests.NormalizerTests

Tests for the Normalizer module.

-/

import Tests.Harness
import Maith.Normalizer

namespace Tests.Normalizer

open Tests
open Lean.DSL

def normalizerTests : List TestResult := [
  runTest "Normalize empty graph"
    (let g := { entities := [], attributes := [], relations := [], operations := [] }
     let normalized := normalizeGraph g
     normalized.entities = [] && normalized.attributes = [])
    "Empty graph should remain empty",
  
  runTest "Normalize single entity"
    (let ent := { id := EntityId.var "x", polarity := Polarity.pos }
     let g := { entities := [ent], attributes := [], relations := [], operations := [] }
     let normalized := normalizeGraph g
     normalized.entities.length = 1)
    "Should preserve single entity",
  
  runTest "Normalize polarity to neutral"
    (let ent := { id := EntityId.var "x", polarity := Polarity.neut }
     let g := { entities := [ent], attributes := [], relations := [], operations := [] }
     let normalized := normalizeGraph g
     match normalized.entities.head? with
     | some e => e.polarity = Polarity.neut
     | none => false)
    "Should preserve neutral polarity",
  
  runTest "Sort entities by EntityId"
    (let e1 := { id := EntityId.term 2, polarity := Polarity.pos }
     let e2 := { id := EntityId.var "x", polarity := Polarity.pos }
     let g := { entities := [e1, e2], attributes := [], relations := [], operations := [] }
     let normalized := normalizeGraph g
     match normalized.entities.head? with
     | some e => e.id = EntityId.var "x"
     | none => false)
    "Variables should come before terms",
  
  runTest "Sort attributes by target then key"
    (let a1 := { target := EntityId.var "b", key := "k1", value := "v", polarity := Polarity.pos }
     let a2 := { target := EntityId.var "a", key := "k1", value := "v", polarity := Polarity.pos }
     let g := { entities := [], attributes := [a1, a2], relations := [], operations := [] }
     let normalized := normalizeGraph g
     match normalized.attributes.head? with
     | some a => a.target = EntityId.var "a"
     | none => false)
    "Attributes should be sorted by target",
  
  runTest "Sort relations by src, op, then tgt"
    (let r1 := { src := EntityId.var "x", tgt := EntityId.var "y", op := RelationOp.eq, polarity := Polarity.pos }
     let r2 := { src := EntityId.var "a", tgt := EntityId.var "b", op := RelationOp.eq, polarity := Polarity.pos }
     let g := { entities := [], attributes := [], relations := [r1, r2], operations := [] }
     let normalized := normalizeGraph g
     match normalized.relations.head? with
     | some r => r.src = EntityId.var "a"
     | none => false)
    "Relations should be sorted by source",
  
  runTest "Sort operations by output, op, then inputs"
    (let o1 := { inputs := [], output := EntityId.var "z", op := OperationOp.add, polarity := Polarity.pos }
     let o2 := { inputs := [], output := EntityId.var "a", op := OperationOp.add, polarity := Polarity.pos }
     let g := { entities := [], attributes := [], relations := [], operations := [o1, o2] }
     let normalized := normalizeGraph g
     match normalized.operations.head? with
     | some o => o.output = EntityId.var "a"
     | none => false)
    "Operations should be sorted by output"
]

def runAllNormalizerTests : IO Unit := do
  runTestSuite "Normalizer Tests" normalizerTests

end Tests.Normalizer
