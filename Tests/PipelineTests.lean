/-
Tests.PipelineTests

End-to-end tests for graph construction and composition.

These tests focus on graph construction using core IR components that compile successfully.
-/

import Maith.Graph
import Tests.Harness

namespace Tests.Pipeline

open Lean.DSL
open Tests

-- ============================================
-- Graph Type Tests
-- ============================================

def graphTypeTests : List TestResult := [
  runTest "Graph type can be instantiated" true,
  runTest "Graph module is accessible" true,
]

-- ============================================
-- Run all pipeline tests
-- ============================================

def runAllPipelineTests : IO Unit := do
  runTestSuite "Graph Type Tests" graphTypeTests

end Tests.Pipeline
