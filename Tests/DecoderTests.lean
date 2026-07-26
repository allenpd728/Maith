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
    "Should decode an empty graph",

  -- v0.1.0 legacy: EntityId.bound serialises as "b(<scope>)"
  runTest "Decoder round-trips EntityId.bound (v0.1.0 legacy)"
    (let decoded := defaultDecoder.decodeEntity ["E", "b(mul_assoc/0/a)", "neut"]
     decoded.id = EntityId.bound "mul_assoc/0/a" && decoded.polarity = Polarity.neut)
    "Should decode legacy b(<scope>) tokens back to EntityId.bound",

  -- v1.2.0: BVAR_N positional token decodes to a lambda-tagged bound ID.
  runTest "Decoder round-trips BVAR_N token (v1.2.0 lambda)"
    (let decoded := defaultDecoder.decodeEntity ["E", "BVAR_0", "neut"]
     decoded.id = EntityId.bound "λ:BVAR_0" && decoded.polarity = Polarity.neut)
    "Should decode BVAR_N positional tokens to lambda-tagged EntityId.bound",

  -- v1.2.0: FVAR_N positional token decodes to a forall-tagged bound ID.
  runTest "Decoder round-trips FVAR_N token (v1.2.0 forall)"
    (let decoded := defaultDecoder.decodeEntity ["E", "FVAR_0", "pos"]
     decoded.id = EntityId.bound "∀:FVAR_0" && decoded.polarity = Polarity.pos)
    "Should decode FVAR_N positional tokens to forall-tagged EntityId.bound",

  -- v1.0.0: EntityId.term encodes as TERM_N
  runTest "Decoder round-trips TERM_N token (v1.0.0)"
    (let decoded := defaultDecoder.decodeEntity ["E", "TERM_3", "pos"]
     decoded.id = EntityId.term 3 && decoded.polarity = Polarity.pos)
    "Should decode TERM_N positional tokens to EntityId.term",

  -- v1.2.0 graph round-trip with lambda-tagged source scopes:
  -- encodeGraph maps .bound "λ:..." → BVAR_N, decoder maps back to .bound "λ:BVAR_N".
  runTest "Decoder round-trips Graph with bound entity (v1.2.0 positional)"
    (let original : Graph := {
      entities := [
        { id := EntityId.bound "λ:mul_assoc/0/a", polarity := Polarity.neut },
        { id := EntityId.var "HMul.hMul",       polarity := Polarity.neut }
      ]
      attributes := []
      relations  := [{ src := EntityId.bound "λ:mul_assoc/0/a"
                       tgt := EntityId.var "HMul.hMul"
                       op  := RelationOp.eq
                       polarity := Polarity.neut }]
      operations := []
    }
    -- After encoding: bound "λ:mul_assoc/0/a" → BVAR_0; after decoding: bound "λ:BVAR_0"
    let expected : Graph := {
      entities := [
        { id := EntityId.bound "λ:BVAR_0",  polarity := Polarity.neut },
        { id := EntityId.var "HMul.hMul", polarity := Polarity.neut }
      ]
      attributes := []
      relations  := [{ src := EntityId.bound "λ:BVAR_0"
                       tgt := EntityId.var "HMul.hMul"
                       op  := RelationOp.eq
                       polarity := Polarity.neut }]
      operations := []
    }
    defaultDecoder.decodeGraph (encodeGraph original) = expected)
    "Should round-trip bound graph with positional BVAR_N tokens",

  -- v1.2.0 graph round-trip with mixed binder kinds:
  -- encodeGraph maps forall/lambda-tagged scopes to independent FVAR_N/BVAR_N counters.
  runTest "Decoder round-trips mixed FVAR/BVAR graph (v1.2.0 positional)"
    (let original : Graph := {
      entities := [
        { id := EntityId.bound "∀:mul_assoc/0/a", polarity := Polarity.pos },
        { id := EntityId.bound "λ:mul_assoc/1/b", polarity := Polarity.neut },
        { id := EntityId.term 0,                  polarity := Polarity.neut }
      ]
      attributes := []
      relations  := [
        { src := EntityId.bound "∀:mul_assoc/0/a"
          tgt := EntityId.bound "λ:mul_assoc/1/b"
          op := RelationOp.eq
          polarity := Polarity.pos }
      ]
      operations := [
        { inputs := [EntityId.bound "∀:mul_assoc/0/a", EntityId.term 0]
          output := EntityId.bound "λ:mul_assoc/1/b"
          op := OperationOp.add
          polarity := Polarity.neut }
      ]
    }
    let expected : Graph := {
      entities := [
        { id := EntityId.bound "∀:FVAR_0", polarity := Polarity.pos },
        { id := EntityId.bound "λ:BVAR_0", polarity := Polarity.neut },
        { id := EntityId.term 0,           polarity := Polarity.neut }
      ]
      attributes := []
      relations  := [
        { src := EntityId.bound "∀:FVAR_0"
          tgt := EntityId.bound "λ:BVAR_0"
          op := RelationOp.eq
          polarity := Polarity.pos }
      ]
      operations := [
        { inputs := [EntityId.bound "∀:FVAR_0", EntityId.term 0]
          output := EntityId.bound "λ:BVAR_0"
          op := OperationOp.add
          polarity := Polarity.neut }
      ]
    }
    defaultDecoder.decodeGraph (encodeGraph original) = expected)
    "Should round-trip a graph containing both FVAR_N and BVAR_N positional binders"
]

def runAllDecoderTests : IO Unit := do
  runTestSuite "Decoder Tests" decoderTests

end Tests.Decoder
