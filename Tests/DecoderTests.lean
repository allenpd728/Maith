/-
Tests.DecoderTests

Tests for the Decoder module.

-/

import Tests.Harness
import Maith.Decoder

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
  
  runTest "Decoder decodes empty Graph correctly"
    ((defaultDecoder.decodeGraph ["GRAPH_BEGIN", "GRAPH_END"]).entities = [])
    "Should decode empty graph placeholder correctly"
]

def runAllDecoderTests : IO Unit := do
  runTestSuite "Decoder Tests" decoderTests

end Tests.Decoder
