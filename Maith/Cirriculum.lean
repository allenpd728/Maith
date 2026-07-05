/-

Curriculum.lean

Curriculum generation for the Lean DSL IR project.

Produces structured training examples for SLM development.

-/

import Maith.Graph
import Maith.Examples
import Maith.Normalizer
import Maith.Transpiler
import Maith.RewriteEngine

namespace Lean.DSL

/--

A curriculum item represents a single training example:

- the original graph

- its normalized form

- the list of rewrite rule names applied

- the Lean syntax representation

-/

structure CurriculumItem :=

(inputGraph      : Graph)

(normalizedGraph : Graph)

(rewriteSteps    : List String)

(leanSyntax      : String)

deriving Repr, DecidableEq

/--

Curriculum generator interface.

Given a graph, produce a curriculum item.

-/

structure CurriculumGenerator :=

(generate : Graph → CurriculumItem)

/--

Default curriculum generator.

Integrates:

- Normalizer

- Transpiler

- RewriteEngine

This produces:

- Graph → Normalized Graph

- Graph → Rewrite Step Names

- Graph → Lean Syntax

-/

def defaultCurriculumGenerator

(norm : Normalizer)

(trans : Transpiler)

(rew : RewriteEngine)

: CurriculumGenerator :=

{

generate := fun g =>

  let normalized := norm.normalize g

  let rewritten := rew.rules.map (fun r => r.name)

  let leanForm := trans.toLeanGraph g

  {

    inputGraph := g,

    normalizedGraph := normalized,

    rewriteSteps := rewritten,

    leanSyntax := leanForm

  }

}

end Lean.DSL
