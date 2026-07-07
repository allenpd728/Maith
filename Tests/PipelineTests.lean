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
  runTest "Graph type can be instantiated" (
    let g : Graph := {
      entities := []
      attributes := []
      relations := []
      operations := []
    }
    g.entities.isEmpty && g.attributes.isEmpty && g.relations.isEmpty && g.operations.isEmpty
  ),
  runTest "Graph module preserves a constructed graph" (
    let g : Graph := {
      entities := [{ id := EntityId.var "n", polarity := Polarity.neut }]
      attributes := []
      relations := []
      operations := []
    }
    g.entities.length = 1 &&
    match g.entities.head? with
    | some e => e.id = EntityId.var "n"
    | none => false
  ),
]

-- ============================================
-- Run all pipeline tests
-- ============================================

def runAllPipelineTests : IO Unit := do
  runTestSuite "Graph Type Tests" graphTypeTests

end Tests.Pipeline
