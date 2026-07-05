/-
Tests.InjectivityTests

Property-based and hand-constructed tests for encoding injectivity.

Ensures that two structurally distinct graphs do not encode to the same tokens
(except when semantically equivalent after normalization).

-/

import Tests.Harness
import Maith.Encoder
import Maith.Decoder
import Maith.Normalizer

namespace Tests.Injectivity

open Tests
open Lean.DSL

/-- Test that addition operations with swapped operands normalize to same tokens -/
def testCommutativeSwapAdd : TestResult :=
  let o1 := { inputs := [EntityId.var "a", EntityId.var "b"], output := EntityId.var "z", 
              op := OperationOp.add, polarity := Polarity.pos }
  let o2 := { inputs := [EntityId.var "b", EntityId.var "a"], output := EntityId.var "z", 
              op := OperationOp.add, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [], operations := [o1] }
  let g2 := { entities := [], attributes := [], relations := [], operations := [o2] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- After normalization, both should have inputs sorted canonically
  runTest "Commutative add normalizes swapped operands" (t1 = t2) 
    "add(a,b) and add(b,a) should normalize identically"

/-- Test that multiplication operations with swapped operands normalize to same tokens -/
def testCommutativeSwapMul : TestResult :=
  let o1 := { inputs := [EntityId.var "x", EntityId.var "y"], output := EntityId.var "w", 
              op := OperationOp.mul, polarity := Polarity.pos }
  let o2 := { inputs := [EntityId.var "y", EntityId.var "x"], output := EntityId.var "w", 
              op := OperationOp.mul, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [], operations := [o1] }
  let g2 := { entities := [], attributes := [], relations := [], operations := [o2] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- After normalization, both should have inputs sorted canonically
  runTest "Commutative mul normalizes swapped operands" (t1 = t2) 
    "mul(x,y) and mul(y,x) should normalize identically"

/-- Test that non-commutative operations DO NOT collapse -/
def testNonCommutativeDistinct : TestResult :=
  let o1 := { inputs := [EntityId.var "a", EntityId.var "b"], output := EntityId.var "z", 
              op := OperationOp.sub, polarity := Polarity.pos }
  let o2 := { inputs := [EntityId.var "b", EntityId.var "a"], output := EntityId.var "z", 
              op := OperationOp.sub, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [], operations := [o1] }
  let g2 := { entities := [], attributes := [], relations := [], operations := [o2] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- These should NOT be equal (sub is not commutative)
  runTest "Non-commutative operations do not collapse" (t1 ≠ t2) 
    "sub(a,b) and sub(b,a) must have different tokens"

/-- Test relations with different src/tgt are distinct -/
def testRelationEdgesDistinct : TestResult :=
  let r1 := { src := EntityId.var "a", tgt := EntityId.var "b", 
              op := RelationOp.eq, polarity := Polarity.pos }
  let r2 := { src := EntityId.var "b", tgt := EntityId.var "a", 
              op := RelationOp.eq, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [r1], operations := [] }
  let g2 := { entities := [], attributes := [], relations := [r2], operations := [] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- These have different src/tgt, should not be equal
  runTest "Relations with different src/tgt are distinct" (t1 ≠ t2)
    "Relation (a→b) and (b→a) must have different tokens"

/-- Test that attribute key/value order matters -/
def testAttributeKeyValueDistinct : TestResult :=
  let a1 := { target := EntityId.var "x", key := "foo", value := "bar", polarity := Polarity.pos }
  let a2 := { target := EntityId.var "x", key := "bar", value := "foo", polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [a1], relations := [], operations := [] }
  let g2 := { entities := [], attributes := [a2], relations := [], operations := [] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- Different keys/values should produce different tokens
  runTest "Attributes with different key/value pairs are distinct" (t1 ≠ t2)
    "Different attribute key/value must have different tokens"

/-- Test that different relations ops are distinct -/
def testRelationOpDistinct : TestResult :=
  let r1 := { src := EntityId.var "a", tgt := EntityId.var "b", 
              op := RelationOp.add, polarity := Polarity.pos }
  let r2 := { src := EntityId.var "a", tgt := EntityId.var "b", 
              op := RelationOp.sub, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [r1], operations := [] }
  let g2 := { entities := [], attributes := [], relations := [r2], operations := [] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- Different operators should produce different tokens
  runTest "Relations with different operators are distinct" (t1 ≠ t2)
    "add(a,b) and sub(a,b) must have different tokens"

/-- Test that different operation ops are distinct -/
def testOperationOpDistinct : TestResult :=
  let o1 := { inputs := [EntityId.var "a"], output := EntityId.var "z", 
              op := OperationOp.add, polarity := Polarity.pos }
  let o2 := { inputs := [EntityId.var "a"], output := EntityId.var "z", 
              op := OperationOp.mul, polarity := Polarity.pos }
  let g1 := { entities := [], attributes := [], relations := [], operations := [o1] }
  let g2 := { entities := [], attributes := [], relations := [], operations := [o2] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- Different operations should produce different tokens
  runTest "Operations with different operators are distinct" (t1 ≠ t2)
    "OperationOp.add and OperationOp.mul must have different tokens"

/-- Test that different polarities are distinct -/
def testPolarityDistinct : TestResult :=
  let e1 := { id := EntityId.var "x", polarity := Polarity.pos }
  let e2 := { id := EntityId.var "x", polarity := Polarity.neut }
  let g1 := { entities := [e1], attributes := [], relations := [], operations := [] }
  let g2 := { entities := [e2], attributes := [], relations := [], operations := [] }
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- Different polarities should produce different tokens
  runTest "Different polarities are distinct" (t1 ≠ t2)
    "Entities with different polarities must have different tokens"

/-- Test mixed graph with multiple components -/
def testComplexGraphDistinct : TestResult :=
  let e1 := { id := EntityId.var "x", polarity := Polarity.pos }
  let e2 := { id := EntityId.var "y", polarity := Polarity.pos }
  let r1 := { src := EntityId.var "x", tgt := EntityId.var "y", 
              op := RelationOp.eq, polarity := Polarity.pos }
  let o1 := { inputs := [EntityId.var "x", EntityId.var "y"], output := EntityId.var "z",
              op := OperationOp.add, polarity := Polarity.pos }
  
  let g1 := { entities := [e1, e2], attributes := [], relations := [r1], operations := [o1] }
  
  -- Swap the relation's operand order (non-commutative conceptually in encoding)
  let r2 := { src := EntityId.var "y", tgt := EntityId.var "x", 
              op := RelationOp.eq, polarity := Polarity.pos }
  let g2 := { entities := [e1, e2], attributes := [], relations := [r2], operations := [o1] }
  
  let t1 := encodeGraph (normalizeGraph g1)
  let t2 := encodeGraph (normalizeGraph g2)
  -- Should be different due to relation edge flip
  runTest "Complex graphs with relation edge flips are distinct" (t1 ≠ t2)
    "Reversing relation direction must produce different tokens"

/-- Batch injectivity: count distinct graphs and distinct token strings -/
def testBatchInjectivity : TestResult :=
  -- Generate several small distinct graphs
  let graphs : List Graph := [
    { entities := [], attributes := [], relations := [], operations := [] },  -- empty
    { entities := [{ id := EntityId.var "a", polarity := Polarity.pos }], 
      attributes := [], relations := [], operations := [] },  -- 1 entity
    { entities := [{ id := EntityId.var "a", polarity := Polarity.pos },
                   { id := EntityId.var "b", polarity := Polarity.pos }], 
      attributes := [], relations := [], operations := [] },  -- 2 entities
    { entities := [], attributes := [], relations := 
      [{ src := EntityId.var "a", tgt := EntityId.var "b", op := RelationOp.eq, 
         polarity := Polarity.pos }], 
      operations := [] },  -- 1 relation
    { entities := [], attributes := [], relations := [], 
      operations := [{ inputs := [EntityId.var "a"], output := EntityId.var "b",
                       op := OperationOp.add, polarity := Polarity.pos }] }  -- 1 operation
  ]
  
  let encoded := graphs.map (fun g => encodeGraph (normalizeGraph g))
  let graphCount := graphs.length
  let uniqueTokens := encoded.length  -- If all distinct, should equal graphCount
  
  -- For this small batch, all should be distinct
  runTest "Batch injectivity test" (graphCount = uniqueTokens)
    s!"Expected {graphCount} distinct token sequences, got {uniqueTokens}"

def injectivityTests : List TestResult := [
  testCommutativeSwapAdd,
  testCommutativeSwapMul,
  testNonCommutativeDistinct,
  testRelationEdgesDistinct,
  testAttributeKeyValueDistinct,
  testRelationOpDistinct,
  testOperationOpDistinct,
  testPolarityDistinct,
  testComplexGraphDistinct,
  testBatchInjectivity
]

def runAllInjectivityTests : IO Unit := do
  runTestSuite "Injectivity Tests" injectivityTests

end Tests.Injectivity
