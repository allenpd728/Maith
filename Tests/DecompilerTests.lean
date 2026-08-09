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
Test 8: Verify decompiled Lean code actually type-checks via `lean`.

Decompiles a small graph, wraps the output in `#check (...)`, writes it to a
temp file with the needed `import`, and runs `lean` on it. Pass iff `lean`
exits 0 (the decompiled term elaborates as a valid Lean expression).

This is the real elaboration gate (replaces the old placeholder). It is
EXPECTED TO FAIL until the decompiler produces elaborator-accepted Lean:
the `Eq` application form, implicit-argument handling, and BVAR/lambda
binders (Task 2) all need work. When it fails, the captured `lean` stderr is
in the failure message so the gap is visible.

NOTE: unverified — pending `lake build` on Kit's machine.
-/
def decompTest8 : IO TestResult := do
  -- A minimal graph: ∀ (a : Nat), Eq a a
  let g : Graph := {
    entities := [
      { id := EntityId.bound "∀:test/0/a", polarity := Polarity.neut },
      { id := EntityId.term 0, polarity := Polarity.neut }
    ]
    attributes := [
      { target := EntityId.bound "∀:test/0/a", key := "typeclass", value := "Nat", polarity := Polarity.neut }
    ]
    relations := [
      { src := EntityId.bound "∀:test/0/a", tgt := EntityId.term 0, op := RelationOp.eq, polarity := Polarity.neut }
    ]
    operations := []
  }
  let decompiled := Decompile.decompileGraph g
  -- Wrap as a #check of the decompiled type expression.
  let leanSrc := s!"import Mathlib.Data.Nat.Basic\n\n#check ({decompiled})\n"
  let tmpPath := "/tmp/maith_decomp_test8.lean"
  IO.FS.writeFile tmpPath leanSrc
  -- Run `lean` on the temp file; capture exit code + stderr.
  let out ← IO.Process.output { cmd := "lean", args := #[tmpPath] }
  -- Clean up the temp file (best-effort; ignore failure).
  try IO.FS.removeFile tmpPath catch _ => pure ()
  if out.exitCode == 0 then
    return TestResult.pass "Decompiled Lean code type-checks (lean exit 0)"
      s!"Decompiled output elaborated: {decompiled}"
  else
    return TestResult.fail "Decompiled Lean code type-checks (lean exit 0)"
      s!"lean exited {out.exitCode} on decompiled output:\n{decompiled}\nstderr:\n{out.stderr}"

/--
Test 7: FULL neg_neg graph from docs/EXAMPLE_ROUNDTRIP.md Stage 3.
This is the exact graph structure from the documentation.
-/
def decompTest7 : TestResult :=
  let g : Graph := {
    entities := [
      { id := EntityId.bound "∀:neg_neg/0/G", polarity := Polarity.neut },
      { id := EntityId.bound "∀:neg_neg/1/inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3", polarity := Polarity.neut },
      { id := EntityId.bound "∀:neg_neg/2/a", polarity := Polarity.neut },
      { id := EntityId.term 0, polarity := Polarity.neut },
      { id := EntityId.term 1, polarity := Polarity.neut },
      { id := EntityId.term 2, polarity := Polarity.neut },
      { id := EntityId.term 3, polarity := Polarity.neut }
    ]
    attributes := [
      { target := EntityId.bound "∀:neg_neg/1/inst._@.Mathlib.Algebra.Group.Defs.567151492._hygCtx._hyg.3", key := "typeclass", value := "InvolutiveNeg", polarity := Polarity.neut },
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
  -- Verify the output contains the key components
  let hasG := result.contains "G"
  let hasA := result.contains "a"
  let hasEq := result.contains "Eq"
  let hasNeg := result.contains "Neg.neg"
  -- Force fail to show decompilation result
  runTest "Decompile full neg_neg from Stage 3 graph"
    (hasG && hasA && hasEq && hasNeg)
    s!"FULL neg_neg decompilation: {result}"

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
Test 9: Verify output doesn't contain malformed "Eq.G" fragment.
The Eq emission was previously malformed, emitting "Eq.G" where G is a variable.
This test verifies the fix: Eq should be emitted as "Eq lhs rhs" without the type.
-/
def decompTest9 : TestResult :=
  -- Build a graph that produces an Eq expression
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "G",
      mkBoundEntity 1 "a",
      mkTermEntity 0,
      mkTermEntity 1,
      mkTermEntity 2,
      mkTermEntity 3
    ]
    attributes := [
      mkAttr (EntityId.bound "∀:test/0/G") "typeclass" "Mul",
      mkAttr (EntityId.term 0) "sort" "u"
    ]
    relations := [
      mkRel (EntityId.bound "∀:test/0/G") (EntityId.term 0) RelationOp.eq,
      mkRel (EntityId.bound "∀:test/1/a") (EntityId.term 1) RelationOp.eq,
      mkRel (EntityId.term 3) (EntityId.term 1) RelationOp.eq  -- Creates an Eq from term3 to term1 (a)
    ]
    operations := [
      mkOp [EntityId.bound "∀:test/0/G", EntityId.term 1] (EntityId.term 2) OperationOp.mul,
      mkOp [EntityId.term 2] (EntityId.term 3) OperationOp.neg  -- Result = neg(mul(G, a))
    ]
  }
  let result := Decompile.decompileGraph g
  let hasEq := result.contains "Eq"
  let hasMalformed := result.contains "Eq."
  runTest "No malformed Eq.G in output"
    (hasEq && !hasMalformed)
    s!"Output should contain 'Eq' but not 'Eq.': {result}"

/--
Test 10: Verify ∀ keyword is present for forall binders.
Previously, forall binders were emitted as "(name : type), body" (tuple syntax).
This test verifies the fix: forall binders now use the ∀ keyword.
-/
def decompTest10 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "x",
      mkBoundEntity 1 "y"
    ]
    attributes := []
    relations := []
    operations := []
  }
  let result := Decompile.decompileGraph g
  let hasForall := result.contains "∀"
  runTest "∀ keyword present for forall binders"
    hasForall
    s!"Output should contain ∀ keyword: {result}"

/--
Test 11: Verify universe level normalization.
Previously, universe levels like "u_1 + 1" were emitted directly.
This test verifies the fix: levels are normalized to "succ u_1".
-/
def decompTest11 : TestResult :=
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "G",
      mkTermEntity 0
    ]
    attributes := [
      mkAttr (EntityId.term 0) "sort" "u_1 + 1"
    ]
    relations := [
      mkRel (EntityId.bound "∀:test/0/G") (EntityId.term 0) RelationOp.eq
    ]
    operations := []
  }
  let result := Decompile.decompileGraph g
  let hasSucc := result.contains "succ"
  let hasMalformed := result.contains "u_1 + 1"
  runTest "Universe level normalized to succ"
    (hasSucc && !hasMalformed)
    s!"Level should be 'succ u_1' not 'u_1 + 1': {result}"

/--
Test 12: Verify Eq emission doesn't include type as suffix.
This is a regression test for the Eq.G issue where the type variable
was incorrectly appended to the Eq constructor name.
-/
def decompTest12 : TestResult :=
  -- Build a graph with a term entity that produces an Eq expression
  let g : Graph := {
    entities := [
      mkBoundEntity 0 "α",
      mkBoundEntity 1 "x",
      mkBoundEntity 2 "y",
      mkTermEntity 0,
      mkTermEntity 1,
      mkTermEntity 2,
      mkTermEntity 3
    ]
    attributes := [
      mkAttr (EntityId.term 0) "sort" "u"
    ]
    relations := [
      mkRel (EntityId.bound "∀:test/0/α") (EntityId.term 0) RelationOp.eq,
      mkRel (EntityId.bound "∀:test/1/x") (EntityId.term 1) RelationOp.eq,
      mkRel (EntityId.bound "∀:test/2/y") (EntityId.term 2) RelationOp.eq,
      mkRel (EntityId.term 3) (EntityId.term 1) RelationOp.eq  -- Creates Eq(x, term3)
    ]
    operations := [
      mkOp [EntityId.bound "∀:test/0/α", EntityId.term 1, EntityId.term 2] (EntityId.term 3) OperationOp.mul
    ]
  }
  let result := Decompile.decompileGraph g
  -- Check that Eq is followed by space, not by a dot
  let hasEq := result.contains "Eq"
  let hasMalformed := result.contains "Eq.α" || result.contains "Eq.α"
  runTest "Eq followed by space, not dot"
    (hasEq && !hasMalformed)
    s!"Eq should be 'Eq lhs rhs' not 'Eq.α lhs rhs': {result}"

/--
Collect all decompiler tests (excluding IO tests).
-/
def decompilerTests : List TestResult := [
  decompTest1,
  decompTest2,
  decompTest3,
  decompTest4,
  decompTest5,
  decompTest6,
  decompTest7,
  decompTest9,
  decompTest10,
  decompTest11,
  decompTest12
]

def runAllDecompilerTests : IO Unit := do
  -- Run sync tests
  runTestSuite "Decompiler Tests" decompilerTests
  
  -- Run IO test (Lean validity check)
  IO.println ""
  IO.println "=== Lean Validity Check ==="
  let testResult <- decompTest8
  match testResult with
  | TestResult.pass name msg => 
    IO.println s!"✓ {name}"
    if not msg.isEmpty then IO.println s!"  {msg}"
  | TestResult.fail name msg =>
    IO.println s!"✗ {name}: {msg}"

end Tests.Decompiler
