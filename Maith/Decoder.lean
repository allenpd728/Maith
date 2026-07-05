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

/--

Decoder transforms linear token sequences back into IR structures.

This is a minimal scaffold: each function uses placeholder logic

so the project compiles cleanly and Copilot can begin extending

the decoder automatically.

-/

structure Decoder :=

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
      let id := EntityId.var idStr
      let pol := if polStr = "pos" then Polarity.pos
                 else if polStr = "neut" then Polarity.neut
                 else Polarity.neg
      { id := id, polarity := pol }
  | _ =>
      { id := EntityId.var "ERR", polarity := Polarity.neut }

def decodeAttribute (toks : List Token) : Attribute :=
  match toks with
  | ["A", tgtStr, key, val, polStr] =>
      let tgt := EntityId.var tgtStr
      let pol := if polStr = "pos" then Polarity.pos
                 else if polStr = "neut" then Polarity.neut
                 else Polarity.neg
      { target := tgt, key := key, value := val, polarity := pol }
  | _ =>
      { target := EntityId.var "ERR", key := "ERR", value := "ERR", polarity := Polarity.neut }

def decodeRelation (toks : List Token) : Relation :=
  match toks with
  | ["R", srcStr, tgtStr, opStr, polStr] =>
      let src := EntityId.var srcStr
      let tgt := EntityId.var tgtStr
      let op :=
        if opStr = "eq" then RelationOp.eq
        else if opStr = "add" then RelationOp.add
        else if opStr = "sub" then RelationOp.sub
        else if opStr = "mul" then RelationOp.mul
        else if opStr = "div" then RelationOp.div
        else if opStr = "le" then RelationOp.le
        else if opStr = "ge" then RelationOp.ge
        else if opStr = "lt" then RelationOp.lt
        else RelationOp.gt
      let pol := if polStr = "pos" then Polarity.pos
                 else if polStr = "neut" then Polarity.neut
                 else Polarity.neg
      { src := src, tgt := tgt, op := op, polarity := pol }
  | _ =>
      { src := EntityId.var "ERR", tgt := EntityId.var "ERR", op := RelationOp.eq, polarity := Polarity.neut }

def decodeOperation (toks : List Token) : Operation :=
  match toks with
  | ["O", inputsStr, outputStr, opStr, polStr] =>
      let trimmed := if inputsStr.startsWith "inputs:" then inputsStr.drop 7 else inputsStr
      -- `String.split` already returns `List String`; the previous `.toList`
      -- call had nothing valid to resolve against and was removed.
      let inputs := (trimmed.split (· = ',')).map (fun s => EntityId.var s)
      let output := EntityId.var (if outputStr.startsWith "output:" then (outputStr.drop 7) else outputStr)
      let op :=
        if opStr = "add" then OperationOp.add
        else if opStr = "sub" then OperationOp.sub
        else if opStr = "mul" then OperationOp.mul
        else if opStr = "div" then OperationOp.div
        else if opStr = "neg" then OperationOp.neg
        else OperationOp.pow
      let pol := if polStr = "pos" then Polarity.pos
                 else if polStr = "neut" then Polarity.neut
                 else Polarity.neg
      { inputs := inputs, output := output, op := op, polarity := pol }
  | _ =>
      { inputs := [], output := EntityId.var "ERR", op := OperationOp.add, polarity := Polarity.neut }

def decodeGraph (toks : List Token) : Graph :=
  let sections := toks.dropWhile (fun t => t ≠ "GRAPH_BEGIN")
  let _body := sections.drop 1 |>.takeWhile (fun t => t ≠ "GRAPH_END")
  -- Placeholder: real implementation will parse structured segments.
  { entities := [],
    attributes := [],
    relations := [],
    operations := [] }

def defaultDecoder : Decoder :=
{
  decodeEntity := decodeEntity
  decodeAttribute := decodeAttribute
  decodeRelation := decodeRelation
  decodeOperation := decodeOperation
  decodeGraph := decodeGraph
}

end Lean.DSL
