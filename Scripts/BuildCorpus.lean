import Maith.MathlibCorpusBuilder

def main : IO Unit :=
  buildMathlibIRCorpusCustomModules ["Mathlib.Algebra.Group.Defs"]
