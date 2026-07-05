/-

TrainingCorpus.lean

Generate training data for SLM (small language model) from IR examples.

Produces IR → Token pairs suitable for model training.

-/

import Maith.Graph
import Maith.Encoder
import Maith.Token

namespace Lean.DSL

/-- A training example: IR graph paired with its token encoding -/

structure TrainingExample :=
  (graph : Graph)
  (tokens : List Token)
  deriving Repr

/-- Generate a training example from an IR graph -/

def generateTrainingExample (g : Graph) : TrainingExample :=
  let tokens := encodeGraph g
  { graph := g, tokens := tokens }

/-- Generate multiple training examples -/

def generateTrainingCorpus (graphs : List Graph) : List TrainingExample :=
  graphs.map generateTrainingExample

/-- Serialize a training example to a format suitable for SLM training -/

def serializeTrainingExample (ex : TrainingExample) : String :=
  let tokenStr := String.intercalate " " ex.tokens
  s!"EXAMPLE\n{tokenStr}\nEND_EXAMPLE"

/-- Serialize a full training corpus to a text file format -/

def serializeTrainingCorpus (examples : List TrainingExample) : String :=
  let serialized := examples.map serializeTrainingExample
  String.intercalate "\n\n" serialized

/-- Count total tokens in a corpus -/

def corpusTokenCount (examples : List TrainingExample) : Nat :=
  examples.foldl (fun acc ex => acc + ex.tokens.length) 0

/-- Statistics about a training corpus -/

structure CorpusStats :=
  (exampleCount : Nat)
  (totalTokens : Nat)
  (avgTokensPerExample : Float)
  deriving Repr

/-- Compute corpus statistics -/

def computeCorpusStats (examples : List TrainingExample) : CorpusStats :=
  let exCount := examples.length
  let totalToks := corpusTokenCount examples
  let avg := if exCount > 0 then totalToks.toFloat / exCount.toFloat else 0
  { exampleCount := exCount, totalTokens := totalToks, avgTokensPerExample := avg }

end Lean.DSL
