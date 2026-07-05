/-

GraphEquivalence.lean

Canonical and structural equivalence checking for IR graphs.

This is the semantic backbone for rewrite correctness, normalization

validation, and SLM curriculum generation.

-/

namespace Lean.DSL

/--

Structural equality for entities.

Two entities are structurally equal if:

- their IDs match

- their polarity matches

- their type matches

-/

def entityStructEq (e1 e2 : Entity) : Bool :=

e1.id = e2.id ∧

e1.polarity = e2.polarity ∧

e1.type = e2.type

/--

Structural equality for attributes.

Two attributes are structurally equal if:

- target matches

- key matches

- value matches

- polarity matches

-/

def attrStructEq (a1 a2 : Attribute) : Bool :=

a1.target = a2.target ∧

a1.key = a2.key ∧

a1.value = a2.value ∧

a1.polarity = a2.polarity

/--

Structural equality for relations.

Two relations are structurally equal if:

- src matches

- op matches

- tgt matches

- polarity matches

-/

def relStructEq (r1 r2 : Relation) : Bool :=

r1.src = r2.src ∧

r1.op = r2.op ∧

r1.tgt = r2.tgt ∧

r1.polarity = r2.polarity

/--

Structural equality for operations.

Two operations are structurally equal if:

- inputs match

- output matches

- op matches

- polarity matches

-/

def opStructEq (o1 o2 : Operation) : Bool :=

o1.inputs = o2.inputs ∧

o1.output = o2.output ∧

o1.op = o2.op ∧

o1.polarity = o2.polarity

/--

Structural equality for entire graphs.

This checks raw structural equality WITHOUT normalization.

Useful for rewrite correctness tests.

-/

def graphStructEq (g1 g2 : Graph) : Bool :=

g1.entities.length = g2.entities.length ∧

g1.attributes.length = g2.attributes.length ∧

g1.relations.length = g2.relations.length ∧

g1.operations.length = g2.operations.length ∧

(List.zip g1.entities g2.entities).all (fun (e1,e2) => entityStructEq e1 e2) ∧

(List.zip g1.attributes g2.attributes).all (fun (a1,a2) => attrStructEq a1 a2) ∧

(List.zip g1.relations g2.relations).all (fun (r1,r2) => relStructEq r1 r2) ∧

(List.zip g1.operations g2.operations).all (fun (o1,o2) => opStructEq o1 o2)

/--

Normalized graph equivalence:

Two graphs are equivalent if their normalized forms are structurally equal.

This is the canonical equivalence relation used by rewrite pipelines.

-/

def graphNormEq (norm : Normalizer) (g1 g2 : Graph) : Bool :=

graphStructEq (norm.normalize g1) (norm.normalize g2)

/--

Rewrite-closure equivalence:

Two graphs are equivalent if running the rewrite pipeline on both

produces the same normalized graph.

This is the strongest equivalence relation and is used for:

- rewrite rule correctness

- rewrite pipeline correctness

- SLM curriculum correctness

-/

def graphRewriteEq (pipe : RewritePipeline) (norm : Normalizer) (g1 g2 : Graph) : Bool :=

let g1' := pipe.run g1

let g2' := pipe.run g2

graphNormEq norm g1' g2'

/--

GraphEquivalence interface.

Provides three levels of equivalence checking:

- structural

- normalized

- rewrite-closure

-/

structure GraphEquivalence :=

(structEq  : Graph → Graph → Bool)

(normEq    : Graph → Graph → Bool)

(rewriteEq : Graph → Graph → Bool)

/--

Default graph equivalence implementation.

-/

def defaultGraphEquivalence (norm : Normalizer) (pipe : RewritePipeline) : GraphEquivalence :=

{

structEq  := graphStructEq,

normEq    := fun g1 g2 => graphNormEq norm g1 g2,

rewriteEq := fun g1 g2 => graphRewriteEq pipe norm g1 g2

}

end Lean.DSL