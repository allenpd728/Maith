/-

RewriteEngine.lean

Simple rewrite engine for IR graphs.

Provides at least one real rewrite rule so the project does something non-trivial.

-/

import Maith.Graph
import Maith.Entity
import Maith.Relation
import Maith.Operation

namespace Lean.DSL

/--

A RewriteRule transforms one Graph into another.

For now, rules are simple functions; later they can be

pattern-based and compositional.

-/

structure RewriteRule where

(name    : String)

(apply   : Graph → Graph)

/--

RewriteEngine holds a list of rewrite rules and applies them

sequentially to a graph.

-/

structure RewriteEngine where

(rules : List RewriteRule)

/-- Apply all rules in order to a graph. -/

def RewriteEngine.rewrite (eng : RewriteEngine) (g : Graph) : Graph :=

eng.rules.foldl (fun acc rule => rule.apply acc) g

/-

Example concrete rule:

Simplify operations of the form x + 0 → x

(on the Operation layer only, as a first real rewrite).

-/

/-- Check if an operation is of the form x + 0. -/

def isAddZero (o : Operation) : Bool :=

match o.inputs with

| [xId, zeroId] =>

  match zeroId with

  | EntityId.term 0 =>

      match o.op with

      | OperationOp.add => true

      | _ => false

  | _ => false

| _ => false

/-- Rewrite x + 0 → x at the operation level. -/

def rewriteAddZeroOp (o : Operation) : Operation :=

if isAddZero o then

match o.inputs with
| [xId, _] =>
  -- Preserve the original operation's polarity rather than hardcoding
  -- `Polarity.pos`, so the rewrite doesn't silently change semantics.
  { inputs := [xId],
    output := o.output,
    op := OperationOp.add,
    polarity := o.polarity }
| _ => o

else

o

/-- Apply x + 0 → x to all operations in a graph. -/

def rewriteAddZeroGraph (g : Graph) : Graph :=

{ g with operations := g.operations.map rewriteAddZeroOp }

/-- A default rewrite engine with a single real rule: x + 0 → x. -/

def defaultRewriteEngine : RewriteEngine :=

{ rules := [

  { name := "add_zero", apply := rewriteAddZeroGraph }

] }

end Lean.DSL
