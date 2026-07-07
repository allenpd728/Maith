/-

OperationOp.lean

Canonical constructive operators for IR graph operations.

Matches all constructors referenced in DSLHelpers and Examples.

-/

namespace Lean.DSL

/--

OperationOp represents constructive operations that build new entities

from existing ones. These appear in algebraic expressions, computational

steps, and structural transformations inside IR graphs.

This definition matches all usage across DSLHelpers, Examples,

and the rewrite engine.

-/

inductive OperationOp

| add    -- addition (+)

| sub    -- subtraction (-)

| mul    -- multiplication (×)

| div    -- division (÷)

| neg    -- unary negation

| pow    -- exponentiation (^)

| generic : String → OperationOp
  -- opaque fallback for any constant-headed application not in the semantic core;
  -- the string carries the head constant's name (e.g. "Iff", "Function.Injective").
  -- Token form: "gen:<name>". Vocabulary grows dynamically on the Python side.

deriving Repr, DecidableEq, Inhabited

instance : ToString OperationOp :=
  ⟨fun op => match op with
    | OperationOp.add       => "add"
    | OperationOp.sub       => "sub"
    | OperationOp.mul       => "mul"
    | OperationOp.div       => "div"
    | OperationOp.neg       => "neg"
    | OperationOp.pow       => "pow"
    | OperationOp.generic s => s!"gen:{s}"⟩

end Lean.DSL
