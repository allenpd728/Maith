/-
Tests.ComponentTests

Tests for individual IR components:
- EntityId
- Entity
- Polarity
- Attribute
- Relation
- Operation
- Graph

These tests validate core IR structures that compile successfully.
-/

import Maith.Polarity
import Maith.EntityId
import Maith.Entity
import Maith.Attribute
import Maith.RelationOp
import Maith.OperationOp
import Maith.Relation
import Maith.Operation
import Maith.Graph
import Tests.Harness

namespace Tests.Components

open Lean.DSL
open Tests

-- ============================================
-- Polarity Tests
-- ============================================

def polarityTests : List TestResult := [
  runTest "Polarity constructors are distinct" (Polarity.pos ≠ Polarity.neg),
  runTest "Polarity.toString is stable" (toString Polarity.neut = "neut"),
]

-- ============================================
-- EntityId Tests
-- ============================================

def entityIdTests : List TestResult := [
  runTest "EntityId.var and term are distinct" (EntityId.var "x" ≠ EntityId.term 0),
  runTest "EntityId.term renders with prefix" (toString (EntityId.term 3) = "t3"),
]

-- ============================================
-- Entity Tests
-- ============================================

def entityTests : List TestResult := [
  runTest "Entity preserves id and polarity" (
    let e : Entity := { id := EntityId.var "x", polarity := Polarity.pos }
    e.id = EntityId.var "x" && e.polarity = Polarity.pos
  ),
]

-- ============================================
-- Attribute Tests
-- ============================================

def attributeTests : List TestResult := [
  runTest "Attribute preserves fields" (
    let a : Attribute := {
      target := EntityId.var "x"
      key := "value"
      value := "42"
      polarity := Polarity.neut
    }
    a.target = EntityId.var "x" && a.key = "value" && a.value = "42"
  ),
]

-- ============================================
-- RelationOp Tests
-- ============================================

def relationOpTests : List TestResult := [
  runTest "Relation operators are distinct" (RelationOp.eq ≠ RelationOp.add),
  runTest "RelationOp.toString is stable" (toString RelationOp.mul = "mul"),
]

-- ============================================
-- Relation Tests
-- ============================================

def relationTests : List TestResult := [
  runTest "Relation preserves fields" (
    let r : Relation := {
      src := EntityId.var "a"
      tgt := EntityId.var "b"
      op := RelationOp.eq
      polarity := Polarity.pos
    }
    r.src = EntityId.var "a" && r.tgt = EntityId.var "b"
  ),
]

-- ============================================
-- OperationOp Tests
-- ============================================

def operationOpTests : List TestResult := [
  runTest "Operation operators are distinct" (OperationOp.add ≠ OperationOp.mul),
  runTest "OperationOp.toString is stable" (toString OperationOp.pow = "pow"),
]

-- ============================================
-- Operation Tests
-- ============================================

def operationTests : List TestResult := [
  runTest "Operation preserves fields" (
    let o : Operation := {
      inputs := [EntityId.var "a", EntityId.var "b"]
      output := EntityId.var "c"
      op := OperationOp.add
      polarity := Polarity.pos
    }
    o.inputs.length = 2 && o.output = EntityId.var "c"
  ),
]

-- ============================================
-- Graph Tests
-- ============================================

def graphTests : List TestResult := [
  runTest "Graph can hold one entity" (
    let g : Graph := {
      entities := [{ id := EntityId.var "x", polarity := Polarity.pos }]
      attributes := []
      relations := []
      operations := []
    }
    g.entities.length = 1 && g.entities.head?.isSome
  ),
  runTest "Graph preserves component counts" (
    let g : Graph := {
      entities := []
      attributes := [{ target := EntityId.var "x", key := "k", value := "v", polarity := Polarity.neut }]
      relations := []
      operations := []
    }
    g.attributes.length = 1 && g.relations.isEmpty
  ),
]

-- ============================================
-- Run all component tests
-- ============================================

def runAllComponentTests : IO Unit := do
  runTestSuite "Polarity Tests" polarityTests
  runTestSuite "EntityId Tests" entityIdTests
  runTestSuite "Entity Tests" entityTests
  runTestSuite "Attribute Tests" attributeTests
  runTestSuite "RelationOp Tests" relationOpTests
  runTestSuite "Relation Tests" relationTests
  runTestSuite "OperationOp Tests" operationOpTests
  runTestSuite "Operation Tests" operationTests
  runTestSuite "Graph Tests" graphTests

end Tests.Components
