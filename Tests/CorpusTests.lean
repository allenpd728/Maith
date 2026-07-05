/-
Tests.TrainingCorpusTests

Tests for the TrainingCorpus module.

-/

import Tests.Harness
import Maith.TrainingCorpus

namespace Tests.TrainingCorpus

open Tests
open Lean.DSL

def corpusTests : List TestResult := [
  runTest "Generate training example from empty graph"
    (let ex := generateTrainingExample { entities := [], attributes := [], relations := [], operations := [] }
     ex.tokens.length ≥ 2)
    "Should generate tokens for empty graph (GRAPH_BEGIN, GRAPH_END)",
  
  runTest "Training example has matching graph"
    (let g := { entities := [], attributes := [], relations := [], operations := [] }
     let ex := generateTrainingExample g
     ex.graph = g)
    "Training example should preserve graph",
  
  runTest "Generate corpus from multiple graphs"
    (let g1 := { entities := [], attributes := [], relations := [], operations := [] }
     let g2 := { entities := [], attributes := [], relations := [], operations := [] }
     let corpus := generateTrainingCorpus [g1, g2]
     corpus.length = 2)
    "Should generate one example per graph",
  
  runTest "Serialize training example produces string"
    (let ex := generateTrainingExample { entities := [], attributes := [], relations := [], operations := [] }
     let serialized := serializeTrainingExample ex
     serialized.contains "EXAMPLE" && serialized.contains "END_EXAMPLE")
    "Serialization should include markers",
  
  runTest "Compute corpus statistics"
    (let ex := generateTrainingExample { entities := [], attributes := [], relations := [], operations := [] }
     let stats := computeCorpusStats [ex]
     stats.exampleCount = 1 && stats.totalTokens ≥ 2)
    "Statistics should reflect corpus contents"
]

def runAllCorpusTests : IO Unit := do
  runTestSuite "Training Corpus Tests" corpusTests

end Tests.TrainingCorpus
