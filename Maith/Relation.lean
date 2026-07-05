/-

Relation.lean

Canonical IR relation representation.

Matches all usage across DSLHelpers, Examples, Encoder, Decoder, and Graph.

-/

import Maith.EntityId
import Maith.RelationOp
import Maith.Polarity

namespace Lean.DSL

/--

A Relation represents a semantic edge between two entities in the IR graph.

It consists of:

* `src`      — source EntityId

* `tgt`      — target EntityId

* `op`       — the relation operator (e.g. eq, add, le)

* `polarity` — semantic polarity (constructive, neutral, reductive)

Relations encode:

* equations (e.g. `x = y`)

* algebraic relationships (e.g. `x + y`)

* comparisons (e.g. `x ≤ y`)

* structural dependencies between entities.

-/

structure Relation :=

(src      : EntityId)

(tgt      : EntityId)

(op       : RelationOp)

(polarity : Polarity)

deriving Repr, DecidableEq

end Lean.DSL
