/-
Tests.EncoderTests

Tests for the Encoder module.

-/

import Tests.Harness
import Maith.Encoder

namespace Tests.Encoder

open Tests
open Lean.DSL

def encoderTests : List TestResult := [
  runTest "Encoder encodes Entity correctly" 
    ((defaultEncoder.encodeEntity { id := EntityId.var "x", polarity := Polarity.pos }).length = 3)
    "Entity should encode to 3 tokens",
  
  runTest "Encoder encodes Attribute correctly"
    ((defaultEncoder.encodeAttribute { target := EntityId.var "y", key := "key1", value := "val1", polarity := Polarity.neut }).length = 5)
    "Attribute should encode to 5 tokens",
  
  runTest "Encoder encodes Relation correctly"
    ((defaultEncoder.encodeRelation { src := EntityId.var "a", tgt := EntityId.var "b", op := RelationOp.eq, polarity := Polarity.pos }).length = 5)
    "Relation should encode to 5 tokens",
  
  runTest "Encoder encodes Operation correctly"
    ((defaultEncoder.encodeOperation { inputs := [EntityId.var "x"], output := EntityId.var "z", op := OperationOp.add, polarity := Polarity.pos }).length = 5)
    "Operation should encode to 5 tokens",
  
  runTest "Encoder graph produces tokens"
    ((defaultEncoder.encodeGraph { entities := [], attributes := [], relations := [], operations := [] }).length ≥ 2)
    "Graph encoding should produce at least GRAPH_BEGIN and GRAPH_END"
]

def runAllEncoderTests : IO Unit := do
  runTestSuite "Encoder Tests" encoderTests

end Tests.Encoder
