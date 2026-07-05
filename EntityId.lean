/-

EntityId.lean

Canonical identifiers for entities in the IR.

Matches all constructors referenced in DSLHelpers and Examples.

-/

namespace Lean.DSL

/--

EntityId represents the identity of an entity in the IR.

It has two forms:

* `var "x"`   — a named variable (used for algebraic expressions)

* `term 0`    — a numbered term (used for intermediate or anonymous nodes)

This matches all usage across DSLHelpers, Examples, and the rewrite engine.

-/

inductive EntityId

| var  : String → EntityId   -- named variable, e.g. "x"

| term : Nat → EntityId      -- numbered term, e.g. 0, 1, 2

deriving Repr, DecidableEq

end Lean.DSL
