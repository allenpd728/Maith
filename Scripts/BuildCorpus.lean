import Maith.MathlibCorpusBuilder

open Lean.DSL in
def main (args : List String) : IO Unit := do
  -- Parse --trace <declName> flag.
  -- When present, prints the raw elaborated Expr for that declaration and exits.
  let traceDecl : Option String :=
    match args with
    | ["--trace", name] => some name
    | _ => none
  buildMathlibIRCorpusCustomModules
    [ "Mathlib.Algebra.Group.Defs"
    , "Mathlib.Algebra.Group.Basic"
    , "Mathlib.Algebra.Ring.Defs"
    , "Mathlib.Order.Basic"
    , "Mathlib.Algebra.Group.NatPowAssoc"
    , "Mathlib.Algebra.Ring.Basic"
    , "Mathlib.Algebra.Ring.GeomSum"
    , "Mathlib.Algebra.Group.Subgroup.Basic"
    , "Mathlib.Data.Nat.Basic"
    , "Mathlib.Data.Int.Basic"
    , "Mathlib.Order.Lattice"
    , "Mathlib.Order.LatticeIntervals"
    , "Mathlib.Algebra.Module.Basic"
    , "Mathlib.Topology.Basic"
    ]
    (encoder := Lean.DSL.defaultEncoder)
    (traceDecl := traceDecl)
