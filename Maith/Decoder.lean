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
  if s.startsWith "t" then
    match s.drop 1 |>.toNat? with
    | some n => .term n
    | none => .var s
  else
    .var s

private def parsePolarityToken (s : String) : Polarity :=
  if s = "pos" then .pos else if s = "neut" then .neut else .neg

private def parseRelationOpToken (s : String) : RelationOp :=
  if s = "eq" then .eq
  else if s = "add" then .add
  else if s = "sub" then .sub
  else if s = "mul" then .mul
  else if s = "div" then .div
  else if s = "le" then .le
  else if s = "ge" then .ge
  else if s = "lt" then .lt
  else .gt

private def parseOperationOpToken (s : String) : OperationOp :=
  if s = "add" then .add
  else if s = "sub" then .sub
  else if s = "mul" then .mul
  else if s = "div" then .div
  else if s = "neg" then .neg
  else .pow

/--

Decoder transforms linear token sequences back into IR structures.

This is a minimal scaffold: each function uses placeholder logic

so the project compiles cleanly and Copilot can begin extending

the decoder automatically.

-/

structure Decoder where

(decodeEntity    : List Token → Entity)

(decodeAttribute : List Token → Attribute)

(decodeRelation  : List Token → Relation)

(decodeOperation : List Token → Operation)

(decodeGraph     : List Token → Graph)

/--

A default decoder implementation that mirrors the default encoder.

This is NOT a full reversible codec — it is a compiling placeholder

that ensures the IR pipeline is structurally complete.

-/

def decodeEntity (toks : List Token) : Entity :=
  match toks with
  | ["E", idStr, polStr] =>
      let id := parseEntityIdToken idStr
      let pol := parsePolarityToken polStr
      { id := id, polarity := pol }
  | _ =>
      { id := EntityId.var "ERR", polarity := Polarity.neut }

def decodeAttribute (toks : List Token) : Attribute :=
  match toks with
  | ["A", tgtStr, key, val, polStr] =>
      let tgt := parseEntityIdToken tgtStr
      let pol := parsePolarityToken polStr
      { target := tgt, key := key, value := val, polarity := pol }
  | _ =>
      { target := EntityId.var "ERR", key := "ERR", value := "ERR", polarity := Polarity.neut }

def decodeRelation (toks : List Token) : Relation :=
  match toks with
  | ["R", srcStr, tgtStr, opStr, polStr] =>
      let src := parseEntityIdToken srcStr
      let tgt := parseEntityIdToken tgtStr
      let op := parseRelationOpToken opStr
      let pol := parsePolarityToken polStr
      { src := src, tgt := tgt, op := op, polarity := pol }
  | _ =>
      { src := EntityId.var "ERR", tgt := EntityId.var "ERR", op := RelationOp.eq, polarity := Polarity.neut }

def decodeOperation (toks : List Token) : Operation :=
  match toks with
  | ["O", inputsStr, outputStr, opStr, polStr] =>
      let trimmed : String :=
        if inputsStr.startsWith "inputs:" then (inputsStr.drop 7).toString else inputsStr
      let inputs := (trimmed.splitOn ",").map (fun s => parseEntityIdToken s.trim)
      let outputText : String :=
        if outputStr.startsWith "output:" then (outputStr.drop 7).toString else outputStr
      let output :=
        parseEntityIdToken outputText.trim
      let op := parseOperationOpToken opStr
      let pol := parsePolarityToken polStr
      { inputs := inputs, output := output, op := op, polarity := pol }
  | _ =>
      { inputs := [], output := EntityId.var "ERR", op := OperationOp.add, polarity := Polarity.neut }

def decodeGraph (toks : List Token) : Graph :=
  let sections := toks.dropWhile (fun t => t ≠ "GRAPH_BEGIN")
  match sections with
  | [] => panic! "decodeGraph: missing GRAPH_BEGIN marker"
  | _ :: body =>
    let body := body.takeWhile (fun t => t ≠ "GRAPH_END")
    let rec go (remaining : List Token) (acc : Graph) : Graph :=
      match remaining with
      | [] => acc
      | "E" :: id :: pol :: rest =>
          go rest { acc with entities := acc.entities ++ [decodeEntity ["E", id, pol]] }
      | "A" :: tgt :: key :: value :: pol :: rest =>
          go rest { acc with attributes := acc.attributes ++ [decodeAttribute ["A", tgt, key, value, pol]] }
      | "R" :: src :: tgt :: op :: pol :: rest =>
          go rest { acc with relations := acc.relations ++ [decodeRelation ["R", src, tgt, op, pol]] }
      | "O" :: inputs :: output :: op :: pol :: rest =>
          go rest { acc with operations := acc.operations ++ [decodeOperation ["O", inputs, output, op, pol]] }
      | _ => panic! s!"decodeGraph: malformed token stream: {remaining}"
    go body { entities := [], attributes := [], relations := [], operations := [] }

def defaultDecoder : Decoder :=
{
  decodeEntity := decodeEntity
  decodeAttribute := decodeAttribute
  decodeRelation := decodeRelation
  decodeOperation := decodeOperation
  decodeGraph := decodeGraph
}

end Lean.DSL
