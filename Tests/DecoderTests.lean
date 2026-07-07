/-
Tests.DecoderTests

Tests for the Decoder module.

-/

import Tests.Harness
import Maith.Decoder
import Maith.Encoder

namespace Tests.Decoder

open Tests
open Lean.DSL

def decoderTests : List TestResult := [
  runTest "Decoder decodes Entity correctly"
    (let entity := defaultDecoder.decodeEntity ["E", "x", "pos"]
     entity.id = EntityId.var "x" && entity.polarity = Polarity.pos)
    "Should decode entity from tokens correctly",
  
  runTest "Decoder handles invalid Entity gracefully"
    ((defaultDecoder.decodeEntity ["E"]).id = EntityId.var "ERR")
    "Should produce error entity for invalid tokens",
  
  runTest "Decoder decodes Attribute correctly"
    ((defaultDecoder.decodeAttribute ["A", "y", "key", "value", "neut"]).target = EntityId.var "y")
    "Should decode attribute from tokens correctly",
  
  runTest "Decoder decodes Relation correctly"
    ((defaultDecoder.decodeRelation ["R", "a", "b", "eq", "pos"]).src = EntityId.var "a")
    "Should decode relation from tokens correctly",
  
  runTest "Decoder decodes Operation correctly"
    ((defaultDecoder.decodeOperation ["O", "inputs:x,y", "output:z", "add", "pos"]).op = OperationOp.add)
    "Should decode operation from tokens correctly",
  
  runTest "Decoder round-trips a non-empty Graph"
    (let graph : Graph := {
      entities := [
        { id := EntityId.var "x", polarity := Polarity.pos },
        { id := EntityId.term 1, polarity := Polarity.neut }
      ]
      attributes := [
        { target := EntityId.var "x", key := "value", value := "42", polarity := Polarity.pos }
      ]
      relations := [
        { src := EntityId.var "x", tgt := EntityId.term 1, op := RelationOp.eq, polarity := Polarity.neg }
      ]
      operations := [
        { inputs := [EntityId.var "x", EntityId.term 1], output := EntityId.var "y", op := OperationOp.add, polarity := Polarity.pos }
      ]
    }
    defaultDecoder.decodeGraph (encodeGraph graph) = graph)
    "Should decode encoded graph back to the original graph",

  runTest "Decoder decodes empty Graph correctly"
    (defaultDecoder.decodeGraph ["GRAPH_BEGIN", "GRAPH_END"] = { entities := [], attributes := [], relations := [], operations := [] })
    "Should decode an empty graph"
]

def runAllDecoderTests : IO Unit := do
  runTestSuite "Decoder Tests" decoderTests

end Tests.Decoder
