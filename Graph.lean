/-

Graph.lean

Canonical IR graph representation.

Matches all usage across DSLHelpers, Examples, Encoder, Decoder, and RewriteEngine.

-/

namespace Lean.DSL

/--

A Graph is the complete IR representation of a mathematical expression,

proof step, or algebraic structure.

It consists of:

* `entities`    — the atomic nodes of the graph

* `attributes`  — metadata attached to entities

* `relations`   — semantic edges between entities

* `operations`  — constructive steps that build new entities

This is the central structure used by:

* the encoder/decoder

* the transpiler

* the rewrite engine

* the SLM training pipeline

-/

structure Graph :=

(entities    : List Entity)

(attributes  : List Attribute)

(relations   : List Relation)

(operations  : List Operation)

deriving Repr, DecidableEq

end Lean.DSL
