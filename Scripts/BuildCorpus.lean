import Maith.MathlibCorpusBuilder

open Lean.DSL in
def main : IO Unit :=
  buildMathlibIRCorpusCustomModules [
    "Mathlib.Algebra.Group.Defs",
    "Mathlib.Algebra.Group.Basic",
    "Mathlib.Algebra.Ring.Defs",
    "Mathlib.Order.Basic"
  ]
