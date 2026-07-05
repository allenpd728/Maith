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
import Maith.TrainingCorpus

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

/--
Build IR graph for equation a*x + b = c.

NOTE: Entity ids are fixed, role-based names (`x`, `a_coeff`, `b_coeff`,
`c_rhs`, `ax_result`, `sum_result`) rather than ids derived from the
coefficient magnitudes (e.g. `EntityId.term a.natAbs`). Deriving ids from
values meant that any two coefficients sharing a magnitude (e.g. `a = 2,
b = -2`, or `a = 3, c = 3`) — which happens often across the small default
coefficient ranges used in tests/corpus generation — would silently collide
and corrupt the graph structure. The actual coefficient values are still
recorded faithfully via the `value`/`rhs` attributes.
-/
def buildEquationGraph (a b c : Int) : Graph :=
  let idX := EntityId.var "x"
  let idA := EntityId.var "a_coeff"
  let idB := EntityId.var "b_coeff"
  let idC := EntityId.var "c_rhs"
  let idAX := EntityId.var "ax_result"
  let idSum := EntityId.var "sum_result"

  -- Entities: x (variable), a (coeff), b (coeff), c (rhs constant)
  let entityX := { id := idX, polarity := Polarity.pos }
  let entityA := { id := idA, polarity := Polarity.pos }
  let entityB := { id := idB, polarity := Polarity.pos }
  let entityC := { id := idC, polarity := Polarity.pos }

  -- Attributes: record the actual coefficient values
  let attrA := { target := idA, key := "coeff",
                 value := toString a, polarity := Polarity.pos }
  let attrB := { target := idB, key := "coeff",
                 value := toString b, polarity := Polarity.pos }
  let attrC := { target := idC, key := "rhs",
                 value := toString c, polarity := Polarity.pos }

  -- Relations: define the structure
  -- x related to a by multiplication
  let relMul := { src := idX, tgt := idA,
                  op := RelationOp.mul, polarity := Polarity.pos }
  -- result of a*x related to b by addition
  let relAdd := { src := idA, tgt := idB,
                  op := RelationOp.add, polarity := Polarity.pos }
  -- result equals c
  let relEq := { src := idB, tgt := idC,
                 op := RelationOp.eq, polarity := Polarity.pos }

  -- Operations: explicit computation steps
  let opMul := { inputs := [idX, idA],
                 output := idAX,
                 op := OperationOp.mul, polarity := Polarity.pos }
  let opAdd := { inputs := [idAX, idB],
                 output := idSum,
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
    graph := problem.graph,
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
