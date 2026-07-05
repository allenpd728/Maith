/-

Encoder.lean

IR → Token sequence encoder.

Minimal, compiling scaffold aligned with the full IR stack.

-/

import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.Graph
import Maith.Polarity
import Maith.EntityId
import Maith.Token

namespace Lean.DSL

/--

Encoder transforms IR structures into linear token sequences.

This is a minimal scaffold: each function is implemented with

simple placeholder logic so the project compiles cleanly and

Copilot can begin extending the encoder automatically.

-/

structure Encoder where

(encodeEntity    : Entity → List Token)

(encodeAttribute : Attribute → List Token)

(encodeRelation  : Relation → List Token)

(encodeOperation : Operation → List Token)

(encodeGraph     : Graph → List Token)

/--

A default encoder implementation that produces simple,

human-readable tokens. This keeps the project compiling

and provides a foundation for future SLM‑ready encoding.

-/

def encodeEntity (e : Entity) : List Token :=
  [
    "E",
    toString e.id,
    toString e.polarity
  ]

def encodeAttribute (a : Attribute) : List Token :=
  [
    "A",
    toString a.target,
    a.key,
    a.value,
    toString a.polarity
  ]

def encodeRelation (r : Relation) : List Token :=
  [
    "R",
    toString r.src,
    toString r.tgt,
    toString r.op,
    toString r.polarity
  ]

def encodeOperation (o : Operation) : List Token :=
  let inputTokens := o.inputs.map (fun id => toString id)
  [
    "O",
    "inputs:" ++ String.intercalate "," inputTokens,
    "output:" ++ toString o.output,
    toString o.op,
    toString o.polarity
  ]

def encodeGraph (g : Graph) : List Token :=
  let ents := g.entities.flatMap encodeEntity
  let attrs := g.attributes.flatMap encodeAttribute
  let rels := g.relations.flatMap encodeRelation
  let ops := g.operations.flatMap encodeOperation
  ["GRAPH_BEGIN"] ++ ents ++ attrs ++ rels ++ ops ++ ["GRAPH_END"]

def defaultEncoder : Encoder :=
{
  encodeEntity := encodeEntity
  encodeAttribute := encodeAttribute
  encodeRelation := encodeRelation
  encodeOperation := encodeOperation
  encodeGraph := encodeGraph
}

end Lean.DSL
