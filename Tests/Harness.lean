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

/-- Print a test result (always shows message if provided) -/
def printTestResult (result : TestResult) : IO Unit := do
  match result with
  | TestResult.pass name msg => 
    IO.println s!"✓ {name}"
    if not msg.isEmpty then IO.println s!"  {msg}"
  | TestResult.fail name msg => IO.println s!"✗ {name}: {msg}"

/-- Run a list of tests and report results -/
def runTestSuite (name : String) (tests : List TestResult) : IO Unit := do
  IO.println ""
  IO.println s!"=== {name} ==="
  for test in tests do
    match test with
    | TestResult.pass name msg => 
      IO.println s!"✓ {name}"
      if not msg.isEmpty then IO.println s!"  {msg}"
    | TestResult.fail name msg => 
      IO.println s!"✗ {name}: {msg}"
  let passed := tests.filter (fun t => match t with | TestResult.pass _ _ => true | _ => false)
  let total := tests.length
  IO.println s!"\nResults: {passed.length}/{total} passed"
  if passed.length ≠ total then
    IO.println "⚠ Some tests failed!"
  else
    IO.println "All tests passed!"

end Tests
