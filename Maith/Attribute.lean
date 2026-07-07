/-

Attribute.lean

Canonical IR attribute representation.

Matches all usage across DSLHelpers, Examples, Encoder, Decoder, and Graph.

-/

import Maith.EntityId
import Maith.Polarity

namespace Lean.DSL

/--

An Attribute attaches metadata to an entity inside the IR graph.

It consists of:

* `target`   — the EntityId this attribute belongs to

* `key`      — the attribute name (e.g. "value", "type")

* `value`    — the attribute value (stored as a string)

* `polarity` — semantic polarity (constructive, neutral, reductive)

Attributes allow the IR to encode:

* literal values (e.g. `value = 3`)

* types (e.g. `type = nat`)

* annotations

* metadata used by the transpiler or rewrite engine

-/

structure Attribute where

(target   : EntityId)

(key      : String)

(value    : String)

(polarity : Polarity)

deriving Repr, DecidableEq, Inhabited

end Lean.DSL
