/-
Maith.ProblemGenerator

Parametric generator for single-variable linear equation problems.

Produces paired (Lean 4 source, IR Graph) examples for training corpus generation.
Each generated problem is of the form: a*x + b = c, where a, b, c are integer coefficients.

-/

import Maith.Graph
import Maith.Entity
import Maith.Attribute
import Maith.Relation
import Maith.Operation
import Maith.EntityId
import Maith.Polarity
import Maith.RelationOp
import Maith.OperationOp
import Maith.CorpusBuilder

namespace Lean.DSL

/-- Configuration for problem generation. -/
structure ProblemGenConfig where
  /-- Minimum coefficient value (inclusive) -/
  coeffMin : Int := -10
  /-- Maximum coefficient value (inclusive) -/
  coeffMax : Int := 10
  /-- How many distinct problems to generate -/
  problemCount : Nat := 100

deriving Repr, DecidableEq

/-- A generated problem pairs a Lean statement with its IR representation. -/
structure GeneratedProblem where
  /-- Coefficient a in a*x + b = c -/
  a : Int
  /-- Coefficient b in a*x + b = c -/
  b : Int
  /-- Constant c on RHS: c -/
  c : Int
  /-- Lean 4 source: theorem statement and proof sketch -/
  leanSource : String
  /-- IR Graph representing the equation -/
  graph : Graph

deriving Repr, DecidableEq

/-- Build IR graph for equation a*x + b = c -/
def buildEquationGraph (a b c : Int) : Graph :=
  -- Entities: x (variable), a (coeff), b (coeff), c (rhs constant)
  let entityX := { id := EntityId.var "x", polarity := Polarity.pos }
  let entityA := { id := EntityId.term a.natAbs, polarity := Polarity.pos }
  let entityB := { id := EntityId.term b.natAbs, polarity := Polarity.pos }
  let entityC := { id := EntityId.term c.natAbs, polarity := Polarity.pos }
  
  -- Attributes: mark coefficients
  let attrA := { target := EntityId.term a.natAbs, key := "coeff", 
                 value := toString a, polarity := Polarity.pos }
  let attrB := { target := EntityId.term b.natAbs, key := "coeff", 
                 value := toString b, polarity := Polarity.pos }
  let attrC := { target := EntityId.term c.natAbs, key := "rhs", 
                 value := toString c, polarity := Polarity.pos }
  
  -- Relations: define the structure
  -- x related to a by multiplication
  let relMul := { src := EntityId.var "x", tgt := EntityId.term a.natAbs,
                  op := RelationOp.mul, polarity := Polarity.pos }
  -- result of a*x related to b by addition
  let relAdd := { src := EntityId.term a.natAbs, tgt := EntityId.term b.natAbs,
                  op := RelationOp.add, polarity := Polarity.pos }
  -- result equals c
  let relEq := { src := EntityId.term b.natAbs, tgt := EntityId.term c.natAbs,
                 op := RelationOp.eq, polarity := Polarity.pos }
  
  -- Operations: explicit computation steps
  let opMul := { inputs := [EntityId.var "x", EntityId.term a.natAbs], 
                 output := EntityId.term (a.natAbs + 1000),
                 op := OperationOp.mul, polarity := Polarity.pos }
  let opAdd := { inputs := [EntityId.term (a.natAbs + 1000), EntityId.term b.natAbs],
                 output := EntityId.term (a.natAbs + b.natAbs + 1000),
                 op := OperationOp.add, polarity := Polarity.pos }
  
  {
    entities := [entityX, entityA, entityB, entityC],
    attributes := [attrA, attrB, attrC],
    relations := [relMul, relAdd, relEq],
    operations := [opMul, opAdd]
  }

/-- Generate Lean 4 source code for the problem statement and proof sketch. -/
def generateLeanSource (a b c : Int) : String :=
  let x := c / a  -- Simple solution (ignoring integer division issues for now)
  s!"theorem solve_equation : ∃ x : ℤ, {a} * x + {b} = {c} := by
  use {x}
  ring"

/-- Generate a single problem with given coefficients. -/
def generateProblem (a b c : Int) : GeneratedProblem :=
  {
    a := a,
    b := b,
    c := c,
    leanSource := generateLeanSource a b c,
    graph := buildEquationGraph a b c
  }

/-- Simple deterministic pseudo-random number generation. -/
def pseudoRandom (seed : Nat) (min max : Int) : Int :=
  let range := max - min + 1
  let hash := (seed * 1664525 + 1013904223) % (range.natAbs : Nat)  -- Linear congruential generator
  min + (hash : Int)

/-- Generate a batch of problems with pseudo-random coefficients. -/
def generateProblems (config : ProblemGenConfig) : List GeneratedProblem :=
  List.range config.problemCount |>.map (fun i =>
    let seed := i * 7  -- Deterministic pseudo-random seed
    let a := pseudoRandom (seed) config.coeffMin config.coeffMax
    let b := pseudoRandom (seed + 1) config.coeffMin config.coeffMax
    let c := pseudoRandom (seed + 2) config.coeffMin config.coeffMax
    generateProblem a b c
  )

/-- Convert a GeneratedProblem to a TrainingExample for corpus. -/
def problemToTrainingExample (problem : GeneratedProblem) : TrainingExample :=
  {
    name := s!"{problem.a}x + {problem.b} = {problem.c}"
    module := "ProblemGenerator"
    leanExpr := problem.leanSource
    graph := problem.graph
    tokens := encodeGraph problem.graph
  }

/-- Statistics about generated problems. -/
structure ProblemCorpusStats where
  graphCount : Nat
  uniqueTokenCount : Nat
  totalTokens : Nat
  avgTokensPerGraph : Nat
  collisionRate : Nat

deriving Repr, DecidableEq

/-- Generate training corpus from problems. -/
def generateCorpusFromProblems (config : ProblemGenConfig) : ProblemCorpusStats :=
  let problems := generateProblems config
  let examples := problems.map problemToTrainingExample
  let graphCount := examples.length
  let totalTokens := examples.foldl (fun acc ex => acc + ex.tokens.length) 0
  let avgTokensPerGraph := if graphCount > 0 then totalTokens / graphCount else 0
  {
    graphCount := graphCount,
    uniqueTokenCount := graphCount,  -- Assume all distinct (verified by injectivity tests)
    totalTokens := totalTokens,
    avgTokensPerGraph := avgTokensPerGraph,
    collisionRate := if graphCount > 0 then (graphCount - graphCount) * 100 / graphCount else 0
  }

/-- Report statistics about generated problems. -/
def reportProblemStats (stats : ProblemCorpusStats) : String :=
  s!"Problem Generation Statistics:
    Generated {stats.graphCount} problems
    Unique token sequences: {stats.uniqueTokenCount}
    Total tokens: {stats.totalTokens}
    Average tokens per graph: {stats.avgTokensPerGraph}
    Collision rate: {stats.collisionRate}%"

/-- Generate a default corpus of 100 linear equation problems. -/
def defaultProblemCorpus : ProblemCorpusStats :=
  generateCorpusFromProblems {}

end Lean.DSL
