/-
Tests.RoundTripTests

Phase 8b — Lean-native Encoder ↔ Decoder round-trip validation.

For each golden example (token sequence hard-coded from the v1.3.0 corpus):
  1. Decode the token list into a Graph via decodeGraph
  2. Re-encode the graph back to tokens via encodeGraph
  3. Assert the re-encoded token list equals the original

If any test fails, the mismatch is shown as expected vs actual token list.

Format: v1.3.0 (no polarity tokens — 2-token E rows, 4-token A/R/O rows).

All examples extracted from Corpus/corpus.jsonl (2026-08-03, lake exe buildCorpus, 14 modules).
-/

import Tests.Harness
import Maith.Encoder
import Maith.Decoder

namespace Tests.RoundTrip

open Tests
open Lean.DSL

-- Round-trip a token list: decode → encode, assert equality.
def roundTripTest (name : String) (toks : List String) : TestResult :=
  let g := decodeGraph toks
  let reEncoded := encodeGraph g
  if reEncoded == toks then
    TestResult.pass name
  else
    -- Find first mismatch by zipping and scanning
    let pairs := toks.zip reEncoded
    let mismatchPos := pairs.findIdx? (fun (a, b) => a != b)
    let posMsg := match mismatchPos with
      | some i =>
          let expected := (toks.drop i).headD "?"
          let actual   := (reEncoded.drop i).headD "?"
          s!" (first mismatch at position {i}: expected \"{expected}\", got \"{actual}\")"
      | none =>
          s!" (length mismatch: original {toks.length}, re-encoded {reEncoded.length})"
    TestResult.fail name s!"Round-trip mismatch{posMsg}"

-- ──────────────────────────────────────────────────────────────────────────────
-- Golden examples (v1.3.0, zero polarity tokens)
-- ──────────────────────────────────────────────────────────────────────────────

-- E + A + R only, FVAR_0, 20 tokens
private def ex_CancelMonoid : List String :=
  ["GRAPH_BEGIN", "E", "FVAR_0", "E", "TERM_0", "E", "TERM_1",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_1", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "GRAPH_END"]

-- Same shape, different typeclass — tests that different graphs still round-trip
private def ex_CommMonoid : List String :=
  ["GRAPH_BEGIN", "E", "FVAR_0", "E", "TERM_0", "E", "TERM_1",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_1", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "GRAPH_END"]

private def ex_SubNegMonoid : List String :=
  ["GRAPH_BEGIN", "E", "FVAR_0", "E", "TERM_0", "E", "TERM_1",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_1", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "GRAPH_END"]

-- O rows with gen: ops, multiple FVAR, 44 tokens
private def ex_MulOne_mk : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3",
   "A", "FVAR_1", "typeclass", "One",
   "A", "FVAR_2", "typeclass", "Mul",
   "A", "TERM_0", "sort", "u_2 + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_2", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_3", "GEN_ALGEBRA",
   "GRAPH_END"]

-- Single O row with arity-1 input, 32 tokens
private def ex_IsDedekindFiniteMonoid : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2",
   "A", "FVAR_1", "typeclass", "MulOne",
   "A", "TERM_0", "sort", "u_2 + 1",
   "A", "TERM_2", "sort", "0",
   "R", "FVAR_0", "TERM_0", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "GRAPH_END"]

-- O rows with `neg` op (tests that neg is handled as OperationOp, not polarity), 48 tokens
private def ex_AddGroup_proof : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3",
   "A", "FVAR_1", "typeclass", "AddGroup",
   "A", "TERM_0", "sort", "u_1 + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_2", "FVAR_0", "eq",
   "R", "TERM_3", "FVAR_2", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_2", "neg",
   "O", "IN_1", "OUT_3", "neg",
   "GRAPH_END"]

-- FVAR and BVAR together, var EntityId "Nat", 58 tokens
private def ex_AddCancelMonoid_ctorIdx : List String :=
  ["GRAPH_BEGIN",
   "E", "Nat", "E", "BVAR_0", "E", "BVAR_1",
   "E", "FVAR_0", "E", "FVAR_1",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3", "E", "TERM_4",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_2", "sort", "u + 1",
   "A", "TERM_4", "literal", "Lean.Literal.natVal 0",
   "R", "BVAR_0", "TERM_2", "eq",
   "R", "BVAR_1", "TERM_3", "eq",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_1", "TERM_1", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_3", "GEN_ALGEBRA",
   "GRAPH_END"]

-- Arity-2 O rows (inputs:FVAR_0,FVAR_1 compound token), 56 tokens
private def ex_AddLeftCancelMonoid_mk : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3", "E", "TERM_4", "E", "TERM_5",
   "A", "FVAR_1", "typeclass", "AddMonoid",
   "A", "FVAR_2", "typeclass", "IsLeftCancelAdd",
   "A", "TERM_0", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_2", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_3", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_4", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_5", "GEN_ALGEBRA",
   "GRAPH_END"]

-- Arity-3 O row (inputs:FVAR_0,FVAR_1,FVAR_3), 50 tokens
private def ex_IsLeftCancelMul : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2", "E", "FVAR_3",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3",
   "A", "FVAR_2", "typeclass", "IsLeftCancelMul",
   "A", "TERM_0", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_1", "TERM_1", "eq",
   "R", "FVAR_3", "FVAR_0", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_2", "GEN_ALGEBRA",
   "O", "IN_3", "OUT_3", "GEN_ALGEBRA",
   "GRAPH_END"]

-- BVAR + FVAR, same structure as AddCancelMonoid.ctorIdx for a different decl, 58 tokens
private def ex_AddRightCancelMonoid_ctorIdx : List String :=
  ["GRAPH_BEGIN",
   "E", "Nat", "E", "BVAR_0", "E", "BVAR_1",
   "E", "FVAR_0", "E", "FVAR_1",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3", "E", "TERM_4",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_2", "sort", "u + 1",
   "A", "TERM_4", "literal", "Lean.Literal.natVal 0",
   "R", "BVAR_0", "TERM_2", "eq",
   "R", "BVAR_1", "TERM_3", "eq",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_1", "TERM_1", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_3", "GEN_ALGEBRA",
   "GRAPH_END"]

-- Large example: 8 O rows, mixed arity, 104 tokens
private def ex_AddRightCancelSemigroup_rec : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2", "E", "FVAR_3",
   "E", "FVAR_4", "E", "FVAR_5", "E", "FVAR_6",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3",
   "E", "TERM_4", "E", "TERM_5", "E", "TERM_6", "E", "TERM_7",
   "E", "TERM_8", "E", "TERM_9",
   "A", "FVAR_4", "typeclass", "AddSemigroup",
   "A", "FVAR_6", "typeclass", "IsRightCancelAdd",
   "A", "TERM_0", "sort", "u + 1",
   "A", "TERM_2", "sort", "u_1",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_1", "TERM_2", "eq",
   "R", "FVAR_2", "TERM_1", "eq",
   "R", "FVAR_3", "TERM_7", "eq",
   "R", "FVAR_5", "TERM_8", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_3", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_4", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_5", "GEN_ALGEBRA",
   "O", "IN_3", "OUT_6", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_7", "hof",
   "O", "IN_1", "OUT_8", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_9", "hof",
   "GRAPH_END"]

-- Large example: multiple R rows between TERM nodes (non-FVAR relations), 88 tokens
private def ex_MulOneClass_ext : List String :=
  ["GRAPH_BEGIN",
   "E", "FVAR_0", "E", "FVAR_1", "E", "FVAR_2", "E", "FVAR_3",
   "E", "TERM_0", "E", "TERM_1", "E", "TERM_2", "E", "TERM_3",
   "E", "TERM_4", "E", "TERM_5", "E", "TERM_6", "E", "TERM_7", "E", "TERM_8",
   "A", "TERM_0", "sort", "u + 1",
   "R", "FVAR_0", "TERM_0", "eq",
   "R", "FVAR_1", "FVAR_2", "eq",
   "R", "FVAR_1", "TERM_1", "eq",
   "R", "FVAR_2", "TERM_2", "eq",
   "R", "FVAR_3", "TERM_5", "eq",
   "R", "TERM_5", "TERM_8", "eq",
   "O", "IN_1", "OUT_1", "GEN_ALGEBRA",
   "O", "IN_1", "OUT_2", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_3", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_4", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_5", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_6", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_7", "GEN_ALGEBRA",
   "O", "IN_2", "OUT_8", "GEN_ALGEBRA",
   "GRAPH_END"]

-- ──────────────────────────────────────────────────────────────────────────────
-- Structural invariant tests (decode-only assertions)
-- ──────────────────────────────────────────────────────────────────────────────

-- Verify entity counts after decode
private def entityCountTest (name : String) (toks : List String) (expected : Nat) : TestResult :=
  let g := decodeGraph toks
  assertEqual s!"{name}: entity count" expected g.entities.length

-- Verify that decoding an empty / malformed sequence does not crash
private def emptyDecodeTest : TestResult :=
  let g := decodeGraph []
  runTest "Empty token list decodes to empty graph" (g.entities.isEmpty && g.operations.isEmpty)

private def missingBeginTest : TestResult :=
  let g := decodeGraph ["E", "FVAR_0", "GRAPH_END"]
  runTest "Missing GRAPH_BEGIN produces empty graph" (g.entities.isEmpty)

-- ──────────────────────────────────────────────────────────────────────────────
-- Test suite
-- ──────────────────────────────────────────────────────────────────────────────

def roundTripTests : List TestResult :=
  -- Round-trip tests
  [ roundTripTest "CancelMonoid (20 tokens, E+A+R, FVAR)" ex_CancelMonoid
  , roundTripTest "CommMonoid (20 tokens, E+A+R, FVAR)" ex_CommMonoid
  , roundTripTest "SubNegMonoid (20 tokens, E+A+R, FVAR)" ex_SubNegMonoid
  , roundTripTest "MulOne.mk (44 tokens, O rows, gen: ops)" ex_MulOne_mk
  , roundTripTest "IsDedekindFiniteMonoid (32 tokens, single O row)" ex_IsDedekindFiniteMonoid
  , roundTripTest "AddGroup._proof_1 (48 tokens, neg op in O rows)" ex_AddGroup_proof
  , roundTripTest "AddCancelMonoid.ctorIdx (58 tokens, FVAR+BVAR+var Nat)" ex_AddCancelMonoid_ctorIdx
  , roundTripTest "AddLeftCancelMonoid.mk (56 tokens, arity-2 O rows)" ex_AddLeftCancelMonoid_mk
  , roundTripTest "IsLeftCancelMul (50 tokens, arity-3 O row)" ex_IsLeftCancelMul
  , roundTripTest "AddRightCancelMonoid.ctorIdx (58 tokens, BVAR+FVAR)" ex_AddRightCancelMonoid_ctorIdx
  , roundTripTest "AddRightCancelSemigroup.rec (104 tokens, 8 O rows)" ex_AddRightCancelSemigroup_rec
  , roundTripTest "MulOneClass.ext (88 tokens, TERM-TERM R rows)" ex_MulOneClass_ext
  -- Structural invariant tests
  , entityCountTest "CancelMonoid entity count" ex_CancelMonoid 3
  , entityCountTest "MulOne.mk entity count" ex_MulOne_mk 7
  , entityCountTest "AddCancelMonoid.ctorIdx entity count" ex_AddCancelMonoid_ctorIdx 10
  , entityCountTest "AddRightCancelSemigroup.rec entity count" ex_AddRightCancelSemigroup_rec 17
  -- Robustness tests
  , emptyDecodeTest
  , missingBeginTest
  ]

end Tests.RoundTrip
