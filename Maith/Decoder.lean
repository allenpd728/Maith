/-

Decoder.lean

Token sequence → IR decoder.

Minimal, compiling scaffold aligned with the full IR stack.

-/

import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.Graph
import Maith.Polarity
import Maith.EntityId
import Maith.RelationOp
import Maith.OperationOp
import Maith.Token

namespace Lean.DSL

private def parseEntityIdToken (s : String) : EntityId :=
  -- v1.0.0/v1.1.0: TERM_N positional token → EntityId.term N; TERM_MANY → .term 64 (sentinel > maxPositional 63)
  if s == "TERM_MANY" then .term 64
  else if s.startsWith "TERM_" then
    let suffix := (s.drop 5).toString
    match suffix.toNat? with
    | some n => .term n
    | none   => .term 64  -- malformed → sentinel
  -- v1.2.0: FVAR_N positional token → EntityId.bound "∀:FVAR_N" (forall binder)
  else if s == "FVAR_MANY" then .bound "∀:FVAR_MANY"
  else if s.startsWith "FVAR_" then .bound s!"∀:{s}"
  -- v1.0.0/v1.1.0: BVAR_N positional token → EntityId.bound "λ:BVAR_N" (lambda binder)
  else if s == "BVAR_MANY" then .bound "λ:BVAR_MANY"
  else if s.startsWith "BVAR_" then .bound s!"λ:{s}"
  -- v0.1.0 legacy: EntityId.term serialised as "t<n>" (e.g. "t0", "t12")
  else if s.startsWith "t" then
    match (s.drop 1).toString.toNat? with
    | some n => .term n
    | none   => .var s
  -- v0.1.0 legacy: EntityId.bound serialised as "b(<scope>)"
  else if s.startsWith "b(" && s.endsWith ")" then
    let inner := (s.drop 2).toString.dropEnd 1 |>.toString
    .bound inner
  -- EntityId.var: bare constant name
  else
    .var s

private def parsePolarityToken (s : String) : Polarity :=
  if s = "pos" then .pos else if s = "neut" then .neut else .neg

private def parseRelationOpToken (s : String) : RelationOp :=
  if s = "eq"  then .eq
  else if s = "add" then .add
  else if s = "sub" then .sub
  else if s = "mul" then .mul
  else if s = "div" then .div
  else if s = "le"  then .le
  else if s = "ge"  then .ge
  else if s = "lt"  then .lt
  else if s = "gt"  then .gt
  else .eq  -- fallback: unknown token → eq (avoids silent wrong-case in .gt default)

private def parseOperationOpToken (s : String) : OperationOp :=
  if s = "add" then .add
  else if s = "sub" then .sub
  else if s = "mul" then .mul
  else if s = "div" then .div
  else if s = "neg" then .neg
  else if s = "pow" then .pow
  else if s.startsWith "gen:" then
    .generic (s.drop 4).toString
  else .pow

/--

Decoder transforms linear token sequences back into IR structures.

Supports both encoder format versions:
- v1.0.0: `TERM_N` → `.term N`, `BVAR_N` → `.bound "BVAR_N"` (positional)
- v0.1.0: `t<n>` → `.term N`, `b(<scope>)` → `.bound scope` (legacy)

`decodeGraph` is total: missing markers and unknown tokens produce an
empty or partial graph rather than a panic.

-/

structure Decoder where

(decodeEntity    : List Token → Entity)

(decodeAttribute : List Token → Attribute)

(decodeRelation  : List Token → Relation)

(decodeOperation : List Token → Operation)

(decodeGraph     : List Token → Graph)

/--

A default decoder implementation that mirrors the default encoder.
Handles all three `EntityId` forms (`var`, `term`, `bound`), all nine
`RelationOp` values, and all `OperationOp` values including `generic`.
Round-trip fidelity is verified by the test suite in `Tests.DecoderTests`.

-/

-- v1.3.0: E rows are 2 tokens (E, id) — no polarity token emitted.
-- v1.2.0: E rows were 3 tokens (E, id, pol) — polarity was emitted.
-- Both forms are accepted here for backward compatibility during corpus transition.
def decodeEntity (toks : List Token) : Entity :=
  match toks with
  | ["E", idStr] =>
      -- v1.3.0 format: no polarity token
      let id := parseEntityIdToken idStr
      { id := id, polarity := Polarity.neut }
  | ["E", idStr, polStr] =>
      -- v1.2.0 format: polarity token present
      let id := parseEntityIdToken idStr
      let pol := parsePolarityToken polStr
      { id := id, polarity := pol }
  | _ =>
      { id := EntityId.var "ERR", polarity := Polarity.neut }

-- v1.3.0: A rows are 4 tokens (A, target, key, value) — no polarity token.
-- v1.2.0: A rows were 5 tokens (A, target, key, value, pol).
def decodeAttribute (toks : List Token) : Attribute :=
  match toks with
  | ["A", tgtStr, key, val] =>
      -- v1.3.0 format: no polarity token
      let tgt := parseEntityIdToken tgtStr
      { target := tgt, key := key, value := val, polarity := Polarity.neut }
  | ["A", tgtStr, key, val, polStr] =>
      -- v1.2.0 format: polarity token present
      let tgt := parseEntityIdToken tgtStr
      let pol := parsePolarityToken polStr
      { target := tgt, key := key, value := val, polarity := pol }
  | _ =>
      { target := EntityId.var "ERR", key := "ERR", value := "ERR", polarity := Polarity.neut }

-- v1.3.0: R rows are 4 tokens (R, src, tgt, op) — no polarity token.
-- v1.2.0: R rows were 5 tokens (R, src, tgt, op, pol).
def decodeRelation (toks : List Token) : Relation :=
  match toks with
  | ["R", srcStr, tgtStr, opStr] =>
      -- v1.3.0 format: no polarity token
      let src := parseEntityIdToken srcStr
      let tgt := parseEntityIdToken tgtStr
      let op := parseRelationOpToken opStr
      { src := src, tgt := tgt, op := op, polarity := Polarity.neut }
  | ["R", srcStr, tgtStr, opStr, polStr] =>
      -- v1.2.0 format: polarity token present
      let src := parseEntityIdToken srcStr
      let tgt := parseEntityIdToken tgtStr
      let op := parseRelationOpToken opStr
      let pol := parsePolarityToken polStr
      { src := src, tgt := tgt, op := op, polarity := pol }
  | _ =>
      { src := EntityId.var "ERR", tgt := EntityId.var "ERR", op := RelationOp.eq, polarity := Polarity.neut }

-- v1.4.0: O rows are 4 tokens (O, IN_N, OUT_N, op) — arity+output-position tokens.
-- v1.3.0: O rows were 4 tokens (O, inputs:..., output:..., op) — compound strings.
-- v1.2.0: O rows were 5 tokens (O, inputs:..., output:..., op, pol) — with polarity.
-- Decode IN_N → N synthetic inputs (EntityId.term 0..N-1 as placeholders);
-- OUT_N → EntityId.term N; OUT_VAR/OUT_MANY → EntityId.term 64 (sentinel).
-- Note: the decoded inputs are positional sentinels only — the real entity identity
-- is carried by E/R rows. Downstream consumers should use the graph structure.
private def parseArityToken (s : String) : Nat :=
  if s == "IN_MANY" then 10
  else if s.startsWith "IN_" then
    match (s.drop 3).toString.toNat? with
    | some n => n
    | none   => 0
  else 0  -- fallback for compound "inputs:..." (v1.3.0 compat)

private def parseOutputToken (s : String) : EntityId :=
  if s == "OUT_MANY" || s == "OUT_VAR" then .term 64
  else if s.startsWith "OUT_" then
    match (s.drop 4).toString.toNat? with
    | some n => .term n
    | none   => .term 64
  else
    -- v1.3.0 compat: "output:TERM_N" compound string
    let text := if s.startsWith "output:" then (s.drop 7).toString else s
    parseEntityIdToken text

def decodeOperation (toks : List Token) : Operation :=
  match toks with
  | ["O", arityOrInputs, outputTok, opStr] =>
      let (inputs, output) :=
        if arityOrInputs.startsWith "IN_" then
          -- v1.4.0 format: IN_N + OUT_N
          let n := parseArityToken arityOrInputs
          let syntheticInputs := (List.range n).map (fun i => EntityId.term i)
          (syntheticInputs, parseOutputToken outputTok)
        else
          -- v1.3.0 format: inputs:... + output:...
          let trimmed : String :=
            if arityOrInputs.startsWith "inputs:" then (arityOrInputs.drop 7).toString else arityOrInputs
          let ins := (trimmed.splitOn ",").map (fun s => parseEntityIdToken (s.trimAscii.toString))
          let outputText : String :=
            if outputTok.startsWith "output:" then (outputTok.drop 7).toString else outputTok
          (ins, parseEntityIdToken (outputText.trimAscii.toString))
      let op := parseOperationOpToken opStr
      { inputs := inputs, output := output, op := op, polarity := Polarity.neut }
  | ["O", inputsStr, outputStr, opStr, polStr] =>
      -- v1.2.0 format: polarity token present, compound strings
      let trimmed : String :=
        if inputsStr.startsWith "inputs:" then (inputsStr.drop 7).toString else inputsStr
      let inputs := (trimmed.splitOn ",").map (fun s => parseEntityIdToken (s.trimAscii.toString))
      let outputText : String :=
        if outputStr.startsWith "output:" then (outputStr.drop 7).toString else outputStr
      let output := parseEntityIdToken (outputText.trimAscii.toString)
      let op := parseOperationOpToken opStr
      let pol := parsePolarityToken polStr
      { inputs := inputs, output := output, op := op, polarity := pol }
  | _ =>
      { inputs := [], output := EntityId.var "ERR", op := OperationOp.add, polarity := Polarity.neut }

-- An empty graph returned on any decode error — safe default, no crash.
private def emptyGraph : Graph :=
  { entities := [], attributes := [], relations := [], operations := [] }

def decodeGraph (toks : List Token) : Graph :=
  -- Find GRAPH_BEGIN; return empty graph gracefully if absent.
  let sections := toks.dropWhile (fun t => t ≠ "GRAPH_BEGIN")
  match sections with
  | [] => emptyGraph  -- missing GRAPH_BEGIN: return empty rather than panic
  | _ :: body =>
    let body := body.takeWhile (fun t => t ≠ "GRAPH_END")
    -- Detect format version by checking whether E rows include a polarity token.
    -- v1.3.0: E id (2 tokens after "E"); v1.2.0: E id pol (3 tokens after "E").
    -- We check the first E row to decide: if the token after id is a known polarity
    -- token, treat the stream as v1.2.0; otherwise treat as v1.3.0.
    let isV12 : Bool :=
      match body.dropWhile (· ≠ "E") with
      | "E" :: _ :: pol :: _ => pol == "pos" || pol == "neut" || pol == "neg"
      | _ => false
    let rec go (remaining : List Token) (acc : Graph) : Graph :=
      match remaining with
      | [] => acc
      -- v1.3.0: 2-token rows (no polarity)
      | "E" :: id :: rest =>
          if isV12 then
            -- v1.2.0: consume id + pol together
            match rest with
            | pol :: rest2 => go rest2 { acc with entities := acc.entities ++ [decodeEntity ["E", id, pol]] }
            | _ => acc
          else
            go rest { acc with entities := acc.entities ++ [decodeEntity ["E", id]] }
      | "A" :: tgt :: key :: value :: rest =>
          if isV12 then
            match rest with
            | pol :: rest2 => go rest2 { acc with attributes := acc.attributes ++ [decodeAttribute ["A", tgt, key, value, pol]] }
            | _ => acc
          else
            go rest { acc with attributes := acc.attributes ++ [decodeAttribute ["A", tgt, key, value]] }
      | "R" :: src :: tgt :: op :: rest =>
          if isV12 then
            match rest with
            | pol :: rest2 => go rest2 { acc with relations := acc.relations ++ [decodeRelation ["R", src, tgt, op, pol]] }
            | _ => acc
          else
            go rest { acc with relations := acc.relations ++ [decodeRelation ["R", src, tgt, op]] }
      | "O" :: inputs :: output :: op :: rest =>
          if isV12 then
            match rest with
            | pol :: rest2 => go rest2 { acc with operations := acc.operations ++ [decodeOperation ["O", inputs, output, op, pol]] }
            | _ => acc
          else
            go rest { acc with operations := acc.operations ++ [decodeOperation ["O", inputs, output, op]] }
      | _ :: rest =>
          -- Unknown or malformed token: skip rather than crash.
          go rest acc
    go body emptyGraph

def defaultDecoder : Decoder :=
{
  decodeEntity := decodeEntity
  decodeAttribute := decodeAttribute
  decodeRelation := decodeRelation
  decodeOperation := decodeOperation
  decodeGraph := decodeGraph
}

end Lean.DSL
