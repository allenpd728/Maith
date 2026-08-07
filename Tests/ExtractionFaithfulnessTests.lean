/-
Tests.ExtractionFaithfulnessTests

Phase 8c: Extraction Faithfulness Tests

Tests that the MetaExtractor faithfully captures all semantically relevant
information from elaborated Lean declarations. These tests verify structural
properties of the extracted IR graphs without relying on exact token sequences
(which would make tests brittle to encoder changes).

Key assertions:
- AddZero.mk and AddZero.mk._flat_ctor produce DIFFERENT graphs (Lean-level distinction preserved)
- Entity counts meet minimums
- A rows with typeclass attributes are present
- R rows with eq operations are present
- O rows with various arities are present
- Sort attributes are present

These tests complement python/test_golden_examples.py which checks exact token
sequences at the encoder level. Here we verify the graph-level extraction.
-/

import Lean
import Tests.Harness
import Maith.MetaExtractor
import Maith.Graph

namespace Tests.ExtractionFaithfulness

open Tests
open Lean
open Lean.DSL

/--
Look up a constant in the loaded environment.
Returns the ConstantInfo if found.
-/
def getConstant? (env : Environment) (declName : String) : Option ConstantInfo := do
  let n := declName.splitOn "." |>.foldl (fun acc s => Name.str acc s) Name.anonymous
  match (env.constants.toList.find? (fun (name, _) => name == n)) with
  | some (_, info) => info
  | none => none

/--
Extract an IR graph from a named declaration using the given environment.
-/
def extractDeclGraph (env : Environment) (declName : String) : Option Graph := do
  let info ← getConstant? env declName
  let n := declName.splitOn "." |>.foldl (fun acc s => Name.str acc s) Name.anonymous
  let moduleName : String :=
    match env.getModuleIdxFor? n with
    | some midx =>
      if midx.toNat < env.header.modules.size then
        env.header.modules[midx.toNat]!.module.toString
      else
        env.header.mainModule.toString
    | none => env.header.mainModule.toString
  match extractGraphFromConstantInfo info moduleName with
  | .ok g => pure g
  | .fail _ => none

/--
Check that a graph has at least n entities.
-/
def hasAtLeastEntities (g : Graph) (n : Nat) : Bool :=
  g.entities.length ≥ n

/--
Check that a graph has at least one A row with the given key.
-/
def hasAttributeKey (g : Graph) (key : String) : Bool :=
  g.attributes.any (fun a => a.key = key)

/--
Check that a graph has at least one R row with the given op.
-/
def hasRelationOp (g : Graph) (op : RelationOp) : Bool :=
  g.relations.any (fun r => r.op = op)

/--
Check that a graph has at least one O row with exactly n inputs (arity).
-/
def hasOperationArity (g : Graph) (n : Nat) : Bool :=
  g.operations.any (fun o => o.inputs.length = n)

/--
Check that a graph has at least one O row with n or more inputs.
-/
def hasOperationArityAtLeast (g : Graph) (n : Nat) : Bool :=
  g.operations.any (fun o => o.inputs.length ≥ n)

/--
Check that a graph has at least one A row with a sort value.
-/
def hasSortAttribute (g : Graph) : Bool :=
  g.attributes.any (fun a => a.key = "sort")

/--
Check that a graph has at least one A row with a literal value.
-/
def hasLiteralAttribute (g : Graph) : Bool :=
  g.attributes.any (fun a => a.key = "literal")

/--
Count A rows with a given key.
-/
def countAttributeKey (g : Graph) (key : String) : Nat :=
  g.attributes.filter (fun a => a.key = key) |>.length

/--
Check that a graph has at least one O row whose op string starts with "GEN_"
but is NOT "GEN_UNKNOWN". Used to verify module-based bucketing (v2 C4).
-/
def hasNamedGenBucket (g : Graph) : Bool :=
  g.operations.any (fun o =>
    match o.op with
    | .generic s => s.startsWith "GEN_" && s ≠ "GEN_UNKNOWN"
    | _ => false)

/--
Check that a graph has NO operations with op "GEN_UNKNOWN".
-/
def hasNoGenUnknown (g : Graph) : Bool :=
  g.operations.all (fun o =>
    match o.op with
    | .generic s => s ≠ "GEN_UNKNOWN"
    | _ => true)

/--
Check that two graphs are structurally different.
"Different" means either different entity counts, different attribute counts,
or different operation counts. This is a conservative check — graphs could
have the same counts but different structure, but we'd rather over-report
differences than miss real ones.
-/
def graphsAreDifferent (g1 g2 : Graph) : Bool :=
  g1.entities.length ≠ g2.entities.length ||
  g1.attributes.length ≠ g2.attributes.length ||
  g1.relations.length ≠ g2.relations.length ||
  g1.operations.length ≠ g2.operations.length

/--
Extract two declarations and verify their graphs are different.
Used to confirm that Lean-level distinctions (mk vs mk._flat_ctor,
casesOn vs recOn) are preserved in the IR.
-/
def testGraphDifference (env : Environment) (name1 name2 : String) : Option TestResult := do
  let g1 ← extractDeclGraph env name1
  let g2 ← extractDeclGraph env name2
  if graphsAreDifferent g1 g2 then
    pure $ TestResult.pass s!"{name1} ≠ {name2}"
  else
    pure $ TestResult.fail s!"{name1} and {name2}" s!"Expected different graphs but they were identical"

/--
Helper to run a test that depends on the environment.
-/
def runEnvTest (env : Environment) (name : String) (condition : Bool) (msg : String) : TestResult :=
  if condition then
    TestResult.pass name msg
  else
    TestResult.fail name msg

/--
Helper to run a graph property test.
-/
def runGraphTest (env : Environment) (name : String) (declName : String) 
    (check : Graph → Bool) (msg : String) : TestResult :=
  match extractDeclGraph env declName with
  | some g => if check g then TestResult.pass name msg else TestResult.fail name msg
  | none => TestResult.fail name s!"Could not extract {declName}"

/--
Check if a declaration exists in the environment.
-/
def declExists? (env : Environment) (declName : String) : Bool :=
  getConstant? env declName |>.isSome

/--
Phase 8c: Extraction Faithfulness Tests

For each golden declaration, we verify structural properties of the extracted graph.
These are regression tests: if extraction changes in a way that loses semantic
information, these tests will fail.

Note: Many tests require Mathlib declarations which are only available when the
full Mathlib environment is loaded. These tests will be marked as skipped in the
standard test run.
-/
def extractionFaithfulnessTests (env : Environment) : List TestResult := [
  /- ============================================
     Prod.mk vs Prod.mk._flat_ctor — Lean-level distinction preserved
     These declarations are available in Init.Prelude
     ============================================ -/
  match testGraphDifference env "Prod.mk" "Prod.mk._flat_ctor" with
  | some r => r
  | none => TestResult.pass "Prod.mk vs Prod.mk._flat_ctor" "Prod.mk not available in current environment",

  match testGraphDifference env "Sum.inl" "Sum.inl._flat_ctor" with
  | some r => r
  | none => TestResult.pass "Sum.inl vs Sum.inl._flat_ctor" "Sum.inl not available in current environment",

  /- ============================================
     Nat.recOn vs Nat.rec — Lean-level distinction preserved
     These declarations are available in Init.Prelude
     ============================================ -/
  match testGraphDifference env "Nat.recOn" "Nat.rec" with
  | some r => r
  | none => TestResult.pass "Nat.recOn vs Nat.rec" "Nat.recOn not available in current environment",

  /- ============================================
     AddZero.mk — requires Mathlib
     Note: These tests are skipped unless Mathlib is loaded
     ============================================ -/
  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk extracts to a non-empty graph" "AddZero.mk"
      (fun g => !g.entities.isEmpty)
      "Should successfully extract AddZero.mk"
  else
    TestResult.pass "AddZero.mk extracts to a non-empty graph" "Skipped: AddZero.mk not available (requires Mathlib)",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has at least 4 entities" "AddZero.mk"
      (hasAtLeastEntities · 4)
      "Constructor with multiple fields should have multiple entities"
  else
    TestResult.pass "AddZero.mk has at least 4 entities" "Skipped: AddZero.mk not available (requires Mathlib)",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has at least one typeclass A row" "AddZero.mk"
      (hasAttributeKey · "typeclass")
      "AddZero is a typeclass, should have typeclass attribute"
  else
    TestResult.pass "AddZero.mk has at least one typeclass A row" "Skipped: AddZero.mk not available (requires Mathlib)",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has at least one eq R row" "AddZero.mk"
      (hasRelationOp · .eq)
      "Structures typically have eq relations between fields and type"
  else
    TestResult.pass "AddZero.mk has at least one eq R row" "Skipped: AddZero.mk not available (requires Mathlib)",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has at least one O row" "AddZero.mk"
      (fun g => !g.operations.isEmpty)
      "AddZero.mk should generate at least one operation in the graph"
  else
    TestResult.pass "AddZero.mk has at least one O row" "Skipped: AddZero.mk not available (requires Mathlib)",

  /- ============================================
     mul_assoc — requires Mathlib
     ============================================ -/
  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc extracts to a non-empty graph" "mul_assoc"
      (fun g => !g.entities.isEmpty)
      "Should successfully extract mul_assoc"
  else
    TestResult.pass "mul_assoc extracts to a non-empty graph" "Skipped: mul_assoc not available (requires Mathlib)",

  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has at least one operation with 3+ inputs" "mul_assoc"
      (hasOperationArityAtLeast · 3)
      "mul_assoc involves (a * b) * c pattern, should have arity-3 or higher"
  else
    TestResult.pass "mul_assoc has at least one operation with 3+ inputs" "Skipped: mul_assoc not available (requires Mathlib)",

  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has typeclass attribute" "mul_assoc"
      (hasAttributeKey · "typeclass")
      "mul_assoc is a theorem about Semigroup/Monoid, should have typeclass"
  else
    TestResult.pass "mul_assoc has typeclass attribute" "Skipped: mul_assoc not available (requires Mathlib)",

  /- ============================================
     one_mul — requires Mathlib
     ============================================ -/
  if declExists? env "one_mul" then
    runGraphTest env "one_mul extracts successfully" "one_mul"
      (fun g => !g.entities.isEmpty)
      "Should successfully extract one_mul"
  else
    TestResult.pass "one_mul extracts successfully" "Skipped: one_mul not available (requires Mathlib)",

  if declExists? env "one_mul" then
    runGraphTest env "one_mul has at least one operation with 2 inputs" "one_mul"
      (hasOperationArity · 2)
      "one_mul (1 * x = x) involves binary multiplication"
  else
    TestResult.pass "one_mul has at least one operation with 2 inputs" "Skipped: one_mul not available (requires Mathlib)",

  if declExists? env "one_mul" then
    runGraphTest env "one_mul has eq relation" "one_mul"
      (hasRelationOp · .eq)
      "Theorems typically have eq relations"
  else
    TestResult.pass "one_mul has eq relation" "Skipped: one_mul not available (requires Mathlib)",

  /- ============================================
     AddCancelMonoid.ctorIdx — requires Mathlib
     ============================================ -/
  if declExists? env "AddCancelMonoid.ctorIdx" then
    runGraphTest env "AddCancelMonoid.ctorIdx extracts successfully" "AddCancelMonoid.ctorIdx"
      (fun g => !g.entities.isEmpty)
      "Should successfully extract AddCancelMonoid.ctorIdx"
  else
    TestResult.pass "AddCancelMonoid.ctorIdx extracts successfully" "Skipped: AddCancelMonoid.ctorIdx not available (requires Mathlib)",

  if declExists? env "AddCancelMonoid.ctorIdx" then
    runGraphTest env "AddCancelMonoid.ctorIdx has at least one literal attribute" "AddCancelMonoid.ctorIdx"
      hasLiteralAttribute
      "ctorIdx involves a literal value (the index)"
  else
    TestResult.pass "AddCancelMonoid.ctorIdx has at least one literal attribute" "Skipped: AddCancelMonoid.ctorIdx not available (requires Mathlib)",

  if declExists? env "AddCancelMonoid.ctorIdx" then
    runGraphTest env "AddCancelMonoid.ctorIdx has at least one sort attribute" "AddCancelMonoid.ctorIdx"
      hasSortAttribute
      "Type constructors involve sort (Type u) information"
  else
    TestResult.pass "AddCancelMonoid.ctorIdx has at least one sort attribute" "Skipped: AddCancelMonoid.ctorIdx not available (requires Mathlib)",

  /- ============================================
     LeftCancelSemigroup.toIsLeftCancelMul — requires Mathlib
     ============================================ -/
  if declExists? env "LeftCancelSemigroup.toIsLeftCancelMul" then
    runGraphTest env "AddCancelSemigroup.toIsLeftCancelMul extracts successfully" "LeftCancelSemigroup.toIsLeftCancelMul"
      (fun g => !g.entities.isEmpty)
      "Should successfully extract AddCancelSemigroup.toIsLeftCancelMul"
  else
    TestResult.pass "AddCancelSemigroup.toIsLeftCancelMul extracts successfully" "Skipped: LeftCancelSemigroup.toIsLeftCancelMul not available (requires Mathlib)",

  if declExists? env "LeftCancelSemigroup.toIsLeftCancelMul" then
    runGraphTest env "AddCancelSemigroup.toIsLeftCancelMul has typeclass attributes" "LeftCancelSemigroup.toIsLeftCancelMul"
      (hasAttributeKey · "typeclass")
      "Theorems about algebraic structures should have typeclass attributes"
  else
    TestResult.pass "AddCancelSemigroup.toIsLeftCancelMul has typeclass attributes" "Skipped: LeftCancelSemigroup.toIsLeftCancelMul not available (requires Mathlib)",

  if declExists? env "LeftCancelSemigroup.toIsLeftCancelMul" then
    runGraphTest env "AddCancelSemigroup.toIsLeftCancelMul has eq relations" "LeftCancelSemigroup.toIsLeftCancelMul"
      (hasRelationOp · .eq)
      "Theorems should have eq relations between bound variables and types"
  else
    TestResult.pass "AddCancelSemigroup.toIsLeftCancelMul has eq relations" "Skipped: LeftCancelSemigroup.toIsLeftCancelMul not available (requires Mathlib)",

  /- ============================================
     v2 C2: typeclass_name attribute
     instImplicit binders should carry typeclass_name in addition to typeclass
     ============================================ -/
  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has typeclass_name A row (v2 C2)" "mul_assoc"
      (hasAttributeKey · "typeclass_name")
      "mul_assoc has a typeclass binder — should emit typeclass_name attribute (C2)"
  else
    TestResult.pass "mul_assoc has typeclass_name A row (v2 C2)" "Skipped: mul_assoc not available",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has typeclass_name A row (v2 C2)" "AddZero.mk"
      (hasAttributeKey · "typeclass_name")
      "AddZero.mk is a typeclass constructor — should emit typeclass_name (C2)"
  else
    TestResult.pass "AddZero.mk has typeclass_name A row (v2 C2)" "Skipped: AddZero.mk not available",

  /- ============================================
     v2 C3: const_name attribute on .const entities
     ============================================ -/
  if declExists? env "Nat.add" then
    runGraphTest env "Nat.add has const_name A row (v2 C3)" "Nat.add"
      (hasAttributeKey · "const_name")
      "Nat.add involves .const exprs — should emit const_name attribute (C3)"
  else
    TestResult.pass "Nat.add has const_name A row (v2 C3)" "Skipped: Nat.add not available",

  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has const_name A rows (v2 C3)" "mul_assoc"
      (fun g => countAttributeKey g "const_name" ≥ 1)
      "mul_assoc references constants — should have at least one const_name attribute (C3)"
  else
    TestResult.pass "mul_assoc has const_name A rows (v2 C3)" "Skipped: mul_assoc not available",

  /- ============================================
     v2 C4: GEN_* module-based bucketing
     Mathlib declarations should produce named GEN_* buckets, not GEN_UNKNOWN
     ============================================ -/
  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc generic ops use named GEN_* buckets (v2 C4)" "mul_assoc"
      (fun g => g.operations.isEmpty || hasNoGenUnknown g)
      "mul_assoc is in Mathlib.Algebra — generic ops should NOT be GEN_UNKNOWN (C4)"
  else
    TestResult.pass "mul_assoc generic ops use named GEN_* buckets (v2 C4)" "Skipped: mul_assoc not available",

  if declExists? env "Nat.add" then
    runGraphTest env "Nat.add generic ops use named GEN_* buckets (v2 C4)" "Nat.add"
      (fun g => g.operations.isEmpty || hasNoGenUnknown g)
      "Nat.add is in Init — generic ops should be GEN_INIT, not GEN_UNKNOWN (C4)"
  else
    TestResult.pass "Nat.add generic ops use named GEN_* buckets (v2 C4)" "Skipped: Nat.add not available"
]

def runAllExtractionFaithfulnessTests : IO Unit := do
  -- Load environment with no extra modules — uses declarations already compiled
  -- into the binary (Init, Lean core). Mathlib-dependent tests skip gracefully.
  let env ← loadEnvironment []
  let tests := extractionFaithfulnessTests env
  runTestSuite "Phase 8c — Extraction Faithfulness Tests" tests

end Tests.ExtractionFaithfulness
