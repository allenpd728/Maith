/-

Encoder.lean

IR → Token sequence encoder. Format version 1.3.0.

See docs/ENCODER_FORMAT.md for the full token grammar specification.

Changes in v1.3.0:
- Removed polarity tokens from E, A, R, O row emission (no discriminative signal)

Changes in v1.2.0:
- Forall binders (∀:) → FVAR_N tokens (separate counter from BVAR_N)
- Lambda binders (λ:) → BVAR_N tokens (unchanged)
- Legacy untagged scopes → BVAR_N (backward compat)

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

Format version 1.3.0 (see docs/ENCODER_FORMAT.md):
- `EntityId.var`   → bare constant name (stable across corpus)
- `EntityId.term`  → `TERM_N` (capped at TERM_63; beyond → TERM_MANY)
- `EntityId.bound` with "∀:" prefix → `FVAR_N` (forall binder, counter starts at 0 per graph)
- `EntityId.bound` with "λ:" prefix → `BVAR_N` (lambda binder, counter starts at 0 per graph)
- `EntityId.bound` untagged (legacy) → `BVAR_N` (backward compat)
- Operation ops    → `gen:*` (rare ops mapped to GEN_UNK at dataset-build time)
- Graph structure  → GRAPH_BEGIN / E / A / R / O / GRAPH_END
- Polarity is no longer emitted as a token (v1.3.0 breaking change)
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

-- Assign positional FVAR/BVAR indices to all EntityId.bound values in a graph,
-- in order of first appearance across entities, attributes, relations, and operations.
-- Scope strings tagged with "∀:" prefix → FVAR_N (forall binders).
-- Scope strings tagged with "λ:" prefix → BVAR_N (lambda binders).
-- Untagged legacy scopes → BVAR_N (backward compat with pre-v1.2.0 corpus data).
-- Returns a mapping from scope string → token.
private def buildBvarMap (g : Graph) : List (String × Token) :=
  let allIds : List EntityId :=
    (g.entities.map (·.id)) ++
    (g.attributes.flatMap (fun a => [a.target])) ++
    (g.relations.flatMap (fun r => [r.src, r.tgt])) ++
    (g.operations.flatMap (fun o => o.inputs ++ [o.output]))
  -- Separate counters for forall and lambda binders so each starts from 0.
  let (_, _, mapping) := allIds.foldl
    (fun (state : Nat × Nat × List (String × Token)) id =>
      let (nextFvar, nextBvar, acc) := state
      match id with
      | .bound scope =>
        if acc.any (fun (s, _) => s == scope) then (nextFvar, nextBvar, acc)
        else if scope.startsWith "∀:" then
          let tok := if nextFvar ≤ maxPositional then s!"FVAR_{nextFvar}" else "FVAR_MANY"
          (nextFvar + 1, nextBvar, acc ++ [(scope, tok)])
        else
          -- "λ:" prefix or legacy untagged → BVAR_N
          let tok := if nextBvar ≤ maxPositional then s!"BVAR_{nextBvar}" else "BVAR_MANY"
          (nextFvar, nextBvar + 1, acc ++ [(scope, tok)])
      | _ => (nextFvar, nextBvar, acc)
    ) (0, 0, [])
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
  ["E", resolveId bvarMap e.id]

private def encodeAttributeWith (bvarMap : List (String × Token)) (a : Attribute) : List Token :=
  ["A", resolveId bvarMap a.target, a.key, a.value]

private def encodeRelationWith (bvarMap : List (String × Token)) (r : Relation) : List Token :=
  ["R", resolveId bvarMap r.src, resolveId bvarMap r.tgt, toString r.op]

private def encodeOperationWith (bvarMap : List (String × Token)) (o : Operation) : List Token :=
  let inputTokens := o.inputs.map (resolveId bvarMap)
  [ "O"
  , "inputs:" ++ String.intercalate "," inputTokens
  , "output:" ++ resolveId bvarMap o.output
  , toString o.op
  ]

-- Per-component functions for the Encoder struct (graph-level context not available).
-- These use the raw toString fallback and are provided for compatibility.
-- Prefer encodeGraph for training corpus output.
def encodeEntity (e : Entity) : List Token :=
  ["E", toString e.id]

def encodeAttribute (a : Attribute) : List Token :=
  ["A", toString a.target, a.key, a.value]

def encodeRelation (r : Relation) : List Token :=
  ["R", toString r.src, toString r.tgt, toString r.op]

def encodeOperation (o : Operation) : List Token :=
  let inputTokens := o.inputs.map (fun id => toString id)
  [ "O"
  , "inputs:" ++ String.intercalate "," inputTokens
  , "output:" ++ toString o.output
  , toString o.op
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
