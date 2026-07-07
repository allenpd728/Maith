/-

Transpiler.lean

Lean syntax ↔ IR graph conversion layer.

Minimal compiling scaffold aligned with the full IR stack.

-/

import Maith.Graph
import Maith.Entity

namespace Lean.DSL

private def formatEntityId (id : EntityId) : String :=
  match id with
  | .var s   => s!"var:{s}"
  | .term n  => s!"term:{n}"
  | .bound s => s!"bound:{s}"

/--

Transpiler converts between Lean syntax (as strings) and the IR graph.

This is a minimal scaffold: each function uses placeholder logic so the

project compiles cleanly and Copilot can begin extending the transpiler

automatically.

-/

structure Transpiler where

(toLeanEntity    : Entity → String)

(toLeanAttribute : Attribute → String)

(toLeanRelation  : Relation → String)

(toLeanOperation : Operation → String)

(toLeanGraph     : Graph → String)

/--

A default transpiler implementation that produces simple,

human-readable Lean-like strings. This keeps the project compiling

and provides a foundation for future Lean→IR and IR→Lean conversions.

-/

def defaultTranspiler : Transpiler :=
  let toLeanEntity := fun e => s!"entity {formatEntityId e.id} @ {e.polarity}"
  let toLeanAttribute := fun a => s!"attr {formatEntityId a.target} {a.key} := {a.value} @ {a.polarity}"
  let toLeanRelation := fun r => s!"rel {formatEntityId r.src} {formatEntityId r.tgt} {r.op} @ {r.polarity}"
  let toLeanOperation := fun o =>
    let ins := String.intercalate ", " (o.inputs.map formatEntityId)
    s!"op ({ins}) -> {formatEntityId o.output} using {o.op} @ {o.polarity}"
  {
    toLeanEntity := toLeanEntity
    toLeanAttribute := toLeanAttribute
    toLeanRelation := toLeanRelation
    toLeanOperation := toLeanOperation
    toLeanGraph := fun g =>
      let ents := g.entities.map toLeanEntity
      let attrs := g.attributes.map toLeanAttribute
      let rels := g.relations.map toLeanRelation
      let ops := g.operations.map toLeanOperation
      String.intercalate "\n" (["-- GRAPH BEGIN"] ++ ents ++ attrs ++ rels ++ ops ++ ["-- GRAPH END"])
  }

end Lean.DSL
