import Maith.MathlibCorpusBuilder

open Lean.DSL in
def main : IO Unit :=
  buildMathlibIRCorpusCustomModules ["Mathlib.Algebra.Group.Defs"]
