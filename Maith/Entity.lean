/-

Entity.lean

Canonical IR entity representation.

Matches all usage across DSLHelpers, Examples, Encoder, Decoder, and Graph.

-/

import Maith.EntityId
import Maith.Polarity

namespace Lean.DSL

/--

An Entity is the atomic unit of the IR graph.

It consists of:

* `id`       — an EntityId (var or term)

* `polarity` — semantic polarity (constructive, neutral, reductive)

Attributes, relations, and operations reference entities by their IDs.

-/

structure Entity where

(id       : EntityId)

(polarity : Polarity)

deriving Repr, DecidableEq

end Lean.DSL
