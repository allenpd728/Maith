/-

RelationOp.lean

Canonical semantic relation operators for the IR.

Matches all constructors referenced in DSLHelpers and Examples.

-/

namespace Lean.DSL

/--

RelationOp represents semantic relationships between entities.

These operators appear in algebraic expressions, equations,

comparisons, and structural relations inside IR graphs.

This definition matches all usage across DSLHelpers, Examples,

and the rewrite engine.

-/

inductive RelationOp

| eq     -- equality (=)

| add    -- addition (+)

| sub    -- subtraction (-)

| mul    -- multiplication (×)

| div    -- division (÷)

| le     -- ≤

| ge     -- ≥

| lt     -- <

| gt     -- >

deriving Repr, DecidableEq

end Lean.DSL
