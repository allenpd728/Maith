/-
Tests.DecompilerTests

Tests for the Decompiler module.

-/

import Tests.Harness
import Maith.Transpiler
import Maith.Decoder
import Maith.Encoder

namespace Tests.Decompiler

open Tests
open Lean.DSL

/--
Test helper: create a simple forall-bound entity.
-/
private def mkBoundEntity (idx : Nat) (name : String) : Entity :=
  { id := EntityId.bound s!"∀:test/{idx}/{name}", polarity := Polarity.neut }

/--
Test helper: create a term entity.
-/
private def mkTermEntity (n : Nat) : Entity :=
  { id := EntityId.term n, polarity := Polarity.neut }

/--
Test helper: create an attribute.
-/
private def mkAttr (target : EntityId) (key value : String) : Attribute :=
  { target := target, key := key, value := value, polarity := Polarity.neut }

/--
Test helper: create a relation.
-/
private def mkRel (src tgt : EntityId) (op : RelationOp) : Relation :=
  { src := src, tgt := tgt, op := op, polarity := Polarity.neut }

/--
Test helper: create an operation.
-/
private def mkOp (inputs : List EntityId) (output : EntityId) (op : OperationOp) : Operation :=
  { inputs := inputs, output := output, op := op, polarity := Polarity.neut }

/--
Test 1: Decompile a simple graph with one forall binder.
-/
def decompTest1 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "G"
    ]
    attributes := [
      mkAttr (EntityId.bound "∀:test/0/G") "typeclass" "Type"
    ]
    relations := []
    operations := []
  }
  let result := Decompile.decompileGraph g
  runTest "Decompile simple forall binder"
    (not result.isEmpty && result.contains "G")
    "Should produce Lean syntax with binder name G"

/--
Test 2: Decompile a graph with a typeclass instance.
-/
def decompTest2 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "G",
      mkBoundEntity 1 "inst"
    ]
    attributes := [
      mkAttr (EntityId.bound "∀:test/1/inst") "typeclass" "InvolutiveNeg"
    ]
    relations := [
      mkRel (EntityId.bound "∀:test/0/G") (EntityId.term 0) RelationOp.eq
    ]
    operations := [
      mkOp [EntityId.bound "∀:test/0/G"] (EntityId.term 0) (OperationOp.generic "Type")
    ]
  }
  let result := Decompile.decompileGraph g
  runTest "Decompile typeclass instance"
    (not result.isEmpty)
    "Should produce Lean syntax with typeclass"

/--
Test 3: Round-trip test - encode then decode.
-/
def decompTest3 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "x",
      mkBoundEntity 1 "y",
      mkTermEntity 0
    ]
    attributes := []
    relations := [
      mkRel (EntityId.bound "∀:test/0/x") (EntityId.term 0) RelationOp.eq,
      mkRel (EntityId.bound "∀:test/1/y") (EntityId.bound "∀:test/0/x") RelationOp.eq
    ]
    operations := [
      mkOp [EntityId.bound "∀:test/0/x", EntityId.term 0] (EntityId.term 1) OperationOp.add
    ]
  }
  let tokens := encodeGraph g
  let decoded := defaultDecoder.decodeGraph tokens
  let recompiled := Decompile.decompileGraph decoded
  runTest "Round-trip encode-decode-decompile"
    (not recompiled.isEmpty)
    "Should produce valid output after round-trip"

/--
Test 4: Decompile neg_neg example (simplified).
This tests the core use case from the example round-trip documentation.
-/
def decompTest4 : TestResult :=
  -- This is a simplified version of the neg_neg graph structure
  let g : Graph := {
    entities := [
      { id := EntityId.bound "∀:neg_neg/0/G", polarity := Polarity.neut },
      { id := EntityId.bound "∀:neg_neg/1/inst", polarity := Polarity.neut },
      { id := EntityId.bound "∀:neg_neg/2/a", polarity := Polarity.neut },
      { id := EntityId.term 0, polarity := Polarity.neut },
      { id := EntityId.term 1, polarity := Polarity.neut },
      { id := EntityId.term 2, polarity := Polarity.neut },
      { id := EntityId.term 3, polarity := Polarity.neut }
    ]
    attributes := [
      { target := EntityId.bound "∀:neg_neg/1/inst", key := "typeclass", value := "InvolutiveNeg", polarity := Polarity.neut },
      { target := EntityId.term 0, key := "sort", value := "u_1 + 1", polarity := Polarity.neut }
    ]
    relations := [
      { src := EntityId.bound "∀:neg_neg/0/G", tgt := EntityId.term 0, op := RelationOp.eq, polarity := Polarity.neut },
      { src := EntityId.bound "∀:neg_neg/2/a", tgt := EntityId.bound "∀:neg_neg/0/G", op := RelationOp.eq, polarity := Polarity.neut },
      { src := EntityId.term 3, tgt := EntityId.bound "∀:neg_neg/2/a", op := RelationOp.eq, polarity := Polarity.neut }
    ]
    operations := [
      { inputs := [EntityId.bound "∀:neg_neg/0/G"], output := EntityId.term 1, op := OperationOp.generic "InvolutiveNeg", polarity := Polarity.neut },
      { inputs := [EntityId.bound "∀:neg_neg/2/a"], output := EntityId.term 2, op := OperationOp.neg, polarity := Polarity.neut },
      { inputs := [EntityId.term 2], output := EntityId.term 3, op := OperationOp.neg, polarity := Polarity.neut }
    ]
  }
  let result := Decompile.decompileGraph g
  -- The result should contain G, inst, and a
  let hasG := result.contains "G"
  let hasInst := result.contains "inst"
  let hasA := result.contains "a"
  let hasNeg := result.contains "Neg.neg"
  -- The neg_neg graph produces a forall body that is just the computed expression
  -- (not wrapped in an explicit Eq, since that's implicit in the body)
  runTest "Decompile neg_neg graph"
    (hasG && hasInst && hasA && hasNeg && not result.isEmpty)
    s!"Should produce valid Lean syntax with all components. Got: {result}"

/--
Test 5: Verify decompiler output is valid Lean-like syntax.
-/
def decompTest5 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "x",
      mkBoundEntity 1 "y",
      mkTermEntity 0
    ]
    attributes := []
    relations := [
      mkRel (EntityId.bound "∀:test/0/x") (EntityId.term 0) RelationOp.eq
    ]
    operations := [
      mkOp [EntityId.bound "∀:test/0/x", EntityId.term 0] (EntityId.term 1) OperationOp.add
    ]
  }
  let result := Decompile.decompileGraph g
  -- Result should contain parentheses (binders) and comma (forall structure)
  let hasParens := result.contains "("
  let hasComma := result.contains ","
  runTest "Decompile produces valid forall structure"
    (hasParens && hasComma)
    "Should produce syntax with forall structure"

/--
Test 6: Default decompiler instance works.
-/
def decompTest6 : TestResult :=
  let g : Graph := {
    entities := [mkBoundEntity 0 "a"]
    attributes := []
    relations := []
    operations := []
  }
  let result := Decompile.defaultDecompiler.decompileGraph g
  runTest "Default decompiler instance works"
    (not result.isEmpty)
    "Default decompiler should produce output"

/--
Collect all decompiler tests.
-/
def decompilerTests : List TestResult := [
  decompTest1,
  decompTest2,
  decompTest3,
  decompTest4,
  decompTest5,
  decompTest6
]

def runAllDecompilerTests : IO Unit := do
  runTestSuite "Decompiler Tests" decompilerTests

end Tests.Decompiler
