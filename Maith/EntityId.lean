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

| var   : String → EntityId   -- named variable, e.g. "x"

| term  : Nat → EntityId      -- numbered term, e.g. 0, 1, 2

| bound : String → EntityId   -- scoped bound variable: "declName/depth/binderName"

deriving Repr, DecidableEq, Inhabited

instance : ToString EntityId :=
  ⟨fun id => match id with
    | EntityId.var s   => s
    | EntityId.term n  => s!"t{n}"
    | EntityId.bound s => s!"b({s})"⟩

-- Sort order: var < bound < term
instance : LT EntityId :=
  ⟨fun id1 id2 => match id1, id2 with
    | EntityId.var s1,   EntityId.var s2   => s1 < s2
    | EntityId.var _,    EntityId.bound _  => true
    | EntityId.var _,    EntityId.term _   => true
    | EntityId.bound _,  EntityId.var _    => false
    | EntityId.bound s1, EntityId.bound s2 => s1 < s2
    | EntityId.bound _,  EntityId.term _   => true
    | EntityId.term _,   EntityId.var _    => false
    | EntityId.term _,   EntityId.bound _  => false
    | EntityId.term n1,  EntityId.term n2  => n1 < n2⟩

def EntityId.compare (id1 id2 : EntityId) : Bool :=
  match id1, id2 with
  | EntityId.var s1,   EntityId.var s2   => s1 < s2
  | EntityId.var _,    EntityId.bound _  => true
  | EntityId.var _,    EntityId.term _   => true
  | EntityId.bound _,  EntityId.var _    => false
  | EntityId.bound s1, EntityId.bound s2 => s1 < s2
  | EntityId.bound _,  EntityId.term _   => true
  | EntityId.term _,   EntityId.var _    => false
  | EntityId.term _,   EntityId.bound _  => false
  | EntityId.term n1,  EntityId.term n2  => n1 < n2

end Lean.DSL
