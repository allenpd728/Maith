/-

RewritePipeline.lean

Multi-pass rewrite pipeline for the Lean DSL IR project.

Applies rewrite rules repeatedly until a fixpoint is reached,

integrating normalization for stable canonical output.

-/

import Maith.RewriteEngine
import Maith.Graph
import Maith.Normalizer

namespace Lean.DSL

/--

Apply one rewrite pass:

Run all rewrite rules in sequence over the graph.

-/

def rewritePass (eng : RewriteEngine) (g : Graph) : Graph :=

eng.rules.foldl (fun acc rule => rule.apply acc) g

/--

Fixpoint detection:

A fixpoint is reached when applying a rewrite pass produces no change.

-/

def isFixpoint (g1 g2 : Graph) : Bool :=

g1 = g2

/--

Multi-pass rewrite pipeline loop.

This is intentionally a partial definition because fixpoint iteration
is not structurally recursive on `Graph`.

-/

partial def runPipelineLoop (eng : RewriteEngine) (norm : Normalizer) (current : Graph) : Graph :=

let next := rewritePass eng current

let normalized := norm.normalize next

if isFixpoint current normalized then

 normalized

else

 runPipelineLoop eng norm normalized

/--

Multi-pass rewrite pipeline:

Repeatedly apply rewrite passes, normalize the result,
and stop when a fixpoint is reached.

-/

def runPipeline (eng : RewriteEngine) (norm : Normalizer) (g : Graph) : Graph :=

runPipelineLoop eng norm g

/--

RewritePipeline interface:

Provides a single `run` function that rewrites a graph to fixpoint.

-/

structure RewritePipeline :=

(run : Graph → Graph)

/--

Default rewrite pipeline:

Uses the provided rewrite engine and normalizer.

-/

def defaultRewritePipeline (eng : RewriteEngine) (norm : Normalizer) : RewritePipeline :=

{ run := fun g => runPipeline eng norm g }

end Lean.DSL