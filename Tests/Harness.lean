/-
Tests.Harness

Simple test framework for the Maith IR pipeline.
Provides utilities for assertion-based testing with clear reporting.
-/

namespace Tests

/-- Test result: either pass or fail with a message -/
inductive TestResult
  | pass (name : String) (message : String := "")
  | fail (name : String) (message : String)
  deriving Repr

/-- Run a test given a name and a boolean condition -/
def runTest (name : String) (condition : Bool) (msg : String := "") : TestResult :=
  if condition then
    TestResult.pass name msg
  else
    TestResult.fail name msg

/-- Assert two values are equal -/
def assertEqual {α : Type} [DecidableEq α] [Repr α]
    (name : String) (expected : α) (actual : α) : TestResult :=
  if expected == actual then
    TestResult.pass name
  else
    TestResult.fail name s!"Expected {repr expected}, got {repr actual}"

/-- Assert two values are not equal -/
def assertNotEqual {α : Type} [DecidableEq α] [Repr α]
    (name : String) (val1 : α) (val2 : α) : TestResult :=
  if val1 ≠ val2 then
    TestResult.pass name
  else
    TestResult.fail name s!"Expected values to be different, but both were {repr val1}"

/-- A test that is known to fail, with the issue tracking its fix.

Issue #25: the suite previously exited 0 no matter what, so failures were
invisible to CI. Making the exit code meaningful requires resolving the existing
failures; while they are being fixed they are listed here *explicitly* rather than
deleted or silently skipped, so the signal is not lost.

A name in this list that starts *passing* is reported as a problem, so the list
cannot rot: fixing a bug forces removing its entry. -/
structure KnownFailure where
  /-- Exact test name as passed to `runTest`. -/
  name : String
  /-- What tracks its resolution. -/
  issue : String

/-- Pre-existing failures (see `docs/experiments/V2_NEXT_STEPS.md`). -/
def knownFailures : List KnownFailure := [
  { name := "Decoder round-trips mixed FVAR/BVAR graph (v1.2.0 positional)",
    issue := "PRE-EXISTING: decoder input-format drift (V2_NEXT_STEPS.md §1)" },
  { name := "Non-commutative operations do not collapse",
    issue := "PRE-EXISTING: normalizer ordering (V2_NEXT_STEPS.md §2)" },
  { name := "Decompiled Lean code type-checks (lean exit 0)",
    issue := "PRE-EXISTING: decompiler output (V2_NEXT_STEPS.md §3)" },
]

/-- Look up a known-failure entry by test name, if present. -/
def knownFailureFor (name : String) : Option KnownFailure :=
  knownFailures.find? (fun k => k.name == name)

/-- Print a test result. Known failures are labelled SKIP and do not count as
failures; a known failure that now passes is flagged so the list gets pruned. -/
def printTestResult (result : TestResult) : IO Unit := do
  match result with
  | TestResult.pass name msg =>
    match knownFailureFor name with
    | some k =>
      IO.println s!"? {name}"
      IO.println s!"  FIXED? listed as a known failure ({k.issue}) but now passes — remove it from Tests.Harness.knownFailures."
    | none =>
      IO.println s!"✓ {name}"
      if not msg.isEmpty then IO.println s!"  {msg}"
  | TestResult.fail name msg =>
    match knownFailureFor name with
    | some k => IO.println s!"⊘ {name} — SKIPPED (known failure: {k.issue})"
    | none => IO.println s!"✗ {name}: {msg}"

/-- Count results that should make the run fail: unexpected `fail`s, plus any
known failure that unexpectedly *passed* (so the allowlist cannot silently rot). -/
def countFailures (tests : List TestResult) : Nat :=
  tests.foldl (fun acc t =>
    match t with
    | TestResult.fail name _ => if (knownFailureFor name).isSome then acc else acc + 1
    | TestResult.pass name _ => if (knownFailureFor name).isSome then acc + 1 else acc
    ) 0

/-- Run a list of tests and report results. Returns the number of *unexpected*
failures (0 = clean) so callers can propagate a meaningful exit code. -/
def runTestSuiteCounted (name : String) (tests : List TestResult) : IO Nat := do
  IO.println ""
  IO.println s!"=== {name} ==="
  for test in tests do
    printTestResult test
  let unexpected := countFailures tests
  let passed := tests.filter (fun t => match t with | TestResult.pass _ _ => true | _ => false)
  let skipped := tests.filter (fun t => match t with
    | TestResult.fail n _ => (knownFailureFor n).isSome
    | _ => false)
  IO.println s!"\nResults: {passed.length}/{tests.length} passed"
  if skipped.length > 0 then
    IO.println s!"  ({skipped.length} known failure(s) skipped — Tests.Harness.knownFailures)"
  if unexpected != 0 then
    IO.println s!"⚠ {unexpected} unexpected test failure(s)"
  else
    IO.println "All tests passed!"
  return unexpected

/-- Run a list of tests and report results (discarding the count). -/
def runTestSuite (name : String) (tests : List TestResult) : IO Unit := do
  let _ ← runTestSuiteCounted name tests
  pure ()

end Tests
