/-

TrainingCorpus.lean

Minimal corpus definitions for Maith IR.

-/

import Maith.Graph
import Maith.Encoder
import Maith.Token

namespace Lean.DSL

/-- Statistics about a training corpus -/

structure SimpleCorpusStats :=
  (exampleCount : Nat)
  (totalTokens : Nat)
  (avgTokensPerExample : Float)
  deriving Repr

end Lean.DSL
