/-

Encoder.lean

IR → Token sequence encoder. Format version 1.0.0.

See docs/ENCODER_FORMAT.md for the full token grammar specification.

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

Format version 1.0.0 (see docs/ENCODER_FORMAT.md):
- `EntityId.var`   → bare constant name (stable across corpus)
- `EntityId.term`  → `TERM_N` (capped at TERM_31; beyond → TERM_MANY)
- `EntityId.bound` → `BVAR_N` positional within graph (capped at BVAR_31; beyond → BVAR_MANY)
- Operation ops    → `gen:*` (rare ops mapped to GEN_UNK at dataset-build time)
- Graph structure  → GRAPH_BEGIN / E / A / R / O / GRAPH_END
-/
structure Encoder where
  encodeEntity    : Entity → List Token
  encodeAttribute : Attribute → List Token
  encodeRelation  : Relation → List Token
  encodeOperation : Operation → List Token
  encodeGraph     : Graph → List Token

-- Maximum positional index before the MANY fallback token.
-- Set to 63: covers p95+ of real Mathlib graphs (p90 = 32, p95 = 32 at current corpus size)
-- without exploding vocab size. Adds 32 new TERM_N/BVAR_N tokens vs the previous cap of 31.
private def maxPositional : Nat := 63

private def termToken (n : Nat) : Token :=
  if n ≤ maxPositional then s!"TERM_{n}" else "TERM_MANY"

-- Assign positional BVAR indices to all EntityId.bound values in a graph,
-- in order of first appearance across entities, attributes, relations, and operations.
-- Returns a mapping from scope string → BVAR_N token.
private def buildBvarMap (g : Graph) : List (String × Token) :=
  let allIds : List EntityId :=
    (g.entities.map (·.id)) ++
    (g.attributes.flatMap (fun a => [a.target])) ++
    (g.relations.flatMap (fun r => [r.src, r.tgt])) ++
    (g.operations.flatMap (fun o => o.inputs ++ [o.output]))
  -- Walk in order, assign a fresh index to each new bound scope.
  let (_, mapping) := allIds.foldl (fun (state : Nat × List (String × Token)) id =>
    let (nextIdx, acc) := state
    match id with
    | .bound scope =>
      if acc.any (fun (s, _) => s == scope) then (nextIdx, acc)
      else
        let tok := if nextIdx ≤ maxPositional then s!"BVAR_{nextIdx}" else "BVAR_MANY"
        (nextIdx + 1, acc ++ [(scope, tok)])
    | _ => (nextIdx, acc)
  ) (0, [])
  mapping

private def resolveId (bvarMap : List (String × Token)) (id : EntityId) : Token :=
  match id with
  | .var s   => s
  | .term n  => termToken n
  | .bound scope =>
    match bvarMap.find? (fun (s, _) => s == scope) with
    | some (_, tok) => tok
    | none          => "BVAR_MANY"  -- should not happen if map was built from same graph

private def encodeEntityWith (bvarMap : List (String × Token)) (e : Entity) : List Token :=
  ["E", resolveId bvarMap e.id, toString e.polarity]

private def encodeAttributeWith (bvarMap : List (String × Token)) (a : Attribute) : List Token :=
  ["A", resolveId bvarMap a.target, a.key, a.value, toString a.polarity]

private def encodeRelationWith (bvarMap : List (String × Token)) (r : Relation) : List Token :=
  ["R", resolveId bvarMap r.src, resolveId bvarMap r.tgt, toString r.op, toString r.polarity]

private def encodeOperationWith (bvarMap : List (String × Token)) (o : Operation) : List Token :=
  let inputTokens := o.inputs.map (resolveId bvarMap)
  [ "O"
  , "inputs:" ++ String.intercalate "," inputTokens
  , "output:" ++ resolveId bvarMap o.output
  , toString o.op
  , toString o.polarity
  ]

-- Per-component functions for the Encoder struct (graph-level context not available).
-- These use the raw toString fallback and are provided for compatibility.
-- Prefer encodeGraph for training corpus output.
def encodeEntity (e : Entity) : List Token :=
  ["E", toString e.id, toString e.polarity]

def encodeAttribute (a : Attribute) : List Token :=
  ["A", toString a.target, a.key, a.value, toString a.polarity]

def encodeRelation (r : Relation) : List Token :=
  ["R", toString r.src, toString r.tgt, toString r.op, toString r.polarity]

def encodeOperation (o : Operation) : List Token :=
  let inputTokens := o.inputs.map (fun id => toString id)
  [ "O"
  , "inputs:" ++ String.intercalate "," inputTokens
  , "output:" ++ toString o.output
  , toString o.op
  , toString o.polarity
  ]

/--
Encode a full graph using v1.0.0 positional ID mapping.
Builds the BVAR map from the graph, then encodes all components consistently.
This is the function used by the corpus builder.
-/
def encodeGraph (g : Graph) : List Token :=
  let bvarMap := buildBvarMap g
  let ents  := g.entities.flatMap   (encodeEntityWith    bvarMap)
  let attrs := g.attributes.flatMap (encodeAttributeWith bvarMap)
  let rels  := g.relations.flatMap  (encodeRelationWith  bvarMap)
  let ops   := g.operations.flatMap (encodeOperationWith bvarMap)
  ["GRAPH_BEGIN"] ++ ents ++ attrs ++ rels ++ ops ++ ["GRAPH_END"]

def defaultEncoder : Encoder :=
  { encodeEntity    := encodeEntity
    encodeAttribute := encodeAttribute
    encodeRelation  := encodeRelation
    encodeOperation := encodeOperation
    encodeGraph     := encodeGraph
  }

end Lean.DSL
