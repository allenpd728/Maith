import Maith.MathlibCorpusBuilder

open Lean.DSL in
def main (args : List String) : IO Unit := do
  -- Parse --trace <declName> flag.
  -- When present, prints the raw elaborated Expr for that declaration and exits.
  let traceDecl : Option String :=
    match args with
    | ["--trace", name] => some name
    | _ => none
  -- Parse --per-operator flag
  let usePerOperator : Bool := args.foldl (fun acc a => acc || a == "--per-operator") false
  let bucketMode : Lean.DSL.BucketMode :=
    if usePerOperator then Lean.DSL.BucketMode.per_operator else Lean.DSL.BucketMode.module
  if usePerOperator then IO.println "[CORPUS] Bucket mode: per_operator (op:<shortName>)"
  else IO.println "[CORPUS] Bucket mode: module (GEN_* buckets)"

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
    (traceDecl := traceDecl) (bucketMode := bucketMode)
