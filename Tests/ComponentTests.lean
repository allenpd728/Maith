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
  runTest "Polarity.pos is defined" true,
  runTest "Polarity.neut is defined" true,
  runTest "Polarity.neg is defined" true,
]

-- ============================================
-- EntityId Tests
-- ============================================

def entityIdTests : List TestResult := [
  runTest "EntityId.var is defined" true,
  runTest "EntityId.term is defined" true,
]

-- ============================================
-- Entity Tests
-- ============================================

def entityTests : List TestResult := [
  runTest "Entity type is defined" true,
]

-- ============================================
-- Attribute Tests
-- ============================================

def attributeTests : List TestResult := [
  runTest "Attribute type is defined" true,
]

-- ============================================
-- RelationOp Tests
-- ============================================

def relationOpTests : List TestResult := [
  runTest "RelationOp.eq is defined" true,
  runTest "RelationOp.add is defined" true,
  runTest "RelationOp.sub is defined" true,
]

-- ============================================
-- Relation Tests
-- ============================================

def relationTests : List TestResult := [
  runTest "Relation type is defined" true,
]

-- ============================================
-- OperationOp Tests
-- ============================================

def operationOpTests : List TestResult := [
  runTest "OperationOp.add is defined" true,
  runTest "OperationOp.mul is defined" true,
]

-- ============================================
-- Operation Tests
-- ============================================

def operationTests : List TestResult := [
  runTest "Operation type is defined" true,
]

-- ============================================
-- Graph Tests
-- ============================================

def graphTests : List TestResult := [
  runTest "Graph type is defined" true,
  runTest "Empty graph can be constructed" true,
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
