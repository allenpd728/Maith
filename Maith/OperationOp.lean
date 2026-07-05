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

deriving Repr, DecidableEq

end Lean.DSL
