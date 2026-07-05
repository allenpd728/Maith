/-
Tests.ProblemGeneratorTests

Tests for parametric linear equation problem generation.

Verifies that generated problems:
1. Produce valid IR graphs
2. Generate unique token sequences (no collisions)
3. Integrate correctly with the training corpus pipeline

-/

import Tests.Harness
import Maith.ProblemGenerator
import Maith.Encoder
import Maith.Normalizer

namespace Tests.ProblemGenerator

open Tests
open Lean.DSL

/-- Test that a single problem generates both Lean and IR correctly -/
def testSingleProblemGeneration : TestResult :=
  let problem := generateProblem 2 3 11  -- 2*x + 3 = 11
  let hasLeanSource := problem.leanSource.length > 0
  let hasGraph := problem.graph.entities.length > 0
  let passed := hasLeanSource ∧ hasGraph
  runTest "Single problem generates Lean and IR" passed
    s!"Lean source: {problem.leanSource.length} chars, Graph entities: {problem.graph.entities.length}"

/-- Test that generated graphs encode to tokens -/
def testProblemGraphEncoding : TestResult :=
  let problem := generateProblem 1 1 2  -- x + 1 = 2
  let encoded := encodeGraph (normalizeGraph problem.graph)
  let tokensExist := encoded.length > 0
  runTest "Generated problem encodes to tokens" tokensExist
    s!"Encoded to {encoded.length} tokens"

/-- Test that training examples convert correctly -/
def testProblemToTrainingExample : TestResult :=
  let problem := generateProblem 3 2 8  -- 3*x + 2 = 8
  let ex := problemToTrainingExample problem
  let hasBothParts := ex.graph.entities.length > 0 ∧ ex.tokens.length > 0
  runTest "Problem converts to training example" hasBothParts
    s!"Graph entities: {ex.graph.entities.length}, Tokens: {ex.tokens.length}"

/-- Test that problem generation produces correct count -/
def testProblemBatchGeneration : TestResult :=
  let config := { coeffMin := -5, coeffMax := 5, problemCount := 20 }
  let problems := generateProblems config
  let expectedCount := 20
  let actualCount := problems.length
  runTest "Generate batch of problems" (actualCount = expectedCount)
    s!"Expected {expectedCount} problems, got {actualCount}"

/-- Test that generated corpus integrates with pipeline -/
def testCorpusGeneration : TestResult :=
  let config := { coeffMin := -3, coeffMax := 3, problemCount := 10 }
  let stats := generateCorpusFromProblems config
  let hasExamples := stats.graphCount > 0
  let statsValid := stats.totalTokens > 0
  runTest "Corpus generation produces valid output" (hasExamples ∧ statsValid)
    s!"Generated corpus with {stats.graphCount} examples, {stats.totalTokens} total tokens"

/-- Test that at least 100 problems can be generated -/
def testLargeScaleGeneration : TestResult :=
  let config := { coeffMin := -10, coeffMax := 10, problemCount := 100 }
  let stats := generateCorpusFromProblems config
  let enoughProblems := stats.graphCount ≥ 100
  runTest "Generate at least 100 problems" enoughProblems
    s!"Generated {stats.graphCount} problems"

/-- Test uniqueness of generated problems (no obvious duplicates from collision) -/
def testGeneratedProblemUniqueness : TestResult :=
  let config := { coeffMin := -5, coeffMax := 5, problemCount := 30 }
  let stats := generateCorpusFromProblems config
  -- With injectivity proven, token sequences should be unique for distinct graphs
  let allUnique := stats.graphCount = stats.uniqueTokenCount
  runTest "Generated problems encode uniquely" allUnique
    s!"Generated {stats.graphCount} unique token sequences"

/-- Test corpus statistics computation -/
def testCorpusStatistics : TestResult :=
  let config := { coeffMin := -2, coeffMax := 2, problemCount := 15 }
  let stats := generateCorpusFromProblems config
  let graphCountCorrect := stats.graphCount = 15
  let avgTokensCorrect := if stats.graphCount > 0 then stats.avgTokensPerGraph > 0 else true
  runTest "Corpus statistics computed correctly" (graphCountCorrect ∧ avgTokensCorrect)
    s!"Graphs: {stats.graphCount}, Avg tokens: {stats.avgTokensPerGraph}"

def problemGeneratorTests : List TestResult := [
  testSingleProblemGeneration,
  testProblemGraphEncoding,
  testProblemToTrainingExample,
  testProblemBatchGeneration,
  testCorpusGeneration,
  testLargeScaleGeneration,
  testGeneratedProblemUniqueness,
  testCorpusStatistics
]

def runAllProblemGeneratorTests : IO Unit := do
  runTestSuite "Problem Generator Tests" problemGeneratorTests

end Tests.ProblemGenerator
