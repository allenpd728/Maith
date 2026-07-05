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

namespace Lean.DSL

/-- Token type used by the encoder and decoder. -/

abbrev Token := String

/--

Encoder transforms IR structures into linear token sequences.

This is a minimal scaffold: each function is implemented with

simple placeholder logic so the project compiles cleanly and

Copilot can begin extending the encoder automatically.

-/

structure Encoder :=

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

def defaultEncoder : Encoder :=

{

encodeEntity := fun e =>

  \[

    "E",

    toString [e.id](https://e.id),

    toString e.polarity

  \],

encodeAttribute := fun a =>

  \[

    "A",

    toString a.target,

    a.key,

    a.value,

    toString a.polarity

  \],

encodeRelation := fun r =>

  \[

    "R",

    toString r.src,

    toString r.tgt,

    toString r.op,

    toString r.polarity

  \],

encodeOperation := fun o =>

  let inputTokens := o.inputs.map (fun id => toString id)

  \[

    "O",

    "inputs:" ++ String.intercalate "," inputTokens,

    "output:" ++ toString o.output,

    toString o.op,

    toString o.polarity

  \],

encodeGraph := fun g =>

  let ents := g.entities.bind (fun e => defaultEncoder.encodeEntity e)

  let attrs := g.attributes.bind (fun a => defaultEncoder.encodeAttribute a)

  let rels := g.relations.bind (fun r => defaultEncoder.encodeRelation r)

  let ops := g.operations.bind (fun o => defaultEncoder.encodeOperation o)

  \["GRAPH_BEGIN"\] ++ ents ++ attrs ++ rels ++ ops ++ \["GRAPH_END"\]

}

end Lean.DSL
