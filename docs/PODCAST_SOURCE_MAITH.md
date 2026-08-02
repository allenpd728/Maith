# Maith: Representation as an Optimization Lever in AI

## Podcast Source Document for NotebookLM

**Purpose:** Comprehensive source material for generating a podcast discussion about Maith's research into semantic representations for mathematical AI.

**Target Audience:** Technical listeners familiar with AI/ML concepts, interested in mathematical AI and formal verification.

**Tone:** Curious, explanatory, forward-looking. Avoids jargon where possible; explains concepts from first principles when necessary.

---

## Section 1: The Big Idea

### The Core Hypothesis

Language models can become better theorem provers if they're trained on a representation of mathematics that exposes semantic structure rather than raw source syntax.

This is the central question Maith is designed to test: **Does representation choice matter as much as model size?**

### Why This Might Be True

Think about two examples that illustrate how representation shapes what we can do:

**Example 1: Language and Color Perception**
Some languages have distinct words for light blue and dark blue, while others use a single word for both. Speakers of languages with separate color terms can distinguish those shades faster. The naming system affects cognitive categorization.

**Example 2: Leibniz vs. Newton Calculus Notation**
Leibniz's calculus notation — with d/dx operators and integral symbols — proved more usable and extensible than Newton's dot notation. Leibniz's version made relationships easier to express, easier to build upon, and easier to teach. It reduced cognitive load and unlocked new mathematical operations.

The pattern in both cases: **Better representations reduce cognitive load and unlock new capabilities.**

### The Bet

If this pattern holds for language models working with formal mathematics, then:

- A **smaller model** trained on a **canonical semantic representation** could outperform a larger model trained on raw source syntax
- The representation itself removes work the model would otherwise have to do
- We could get more capable AI with less compute

---

## Section 2: What is Lean and Why Does It Matter?

### Lean 4 as a Proof Assistant

Lean is a modern interactive theorem prover — a system where mathematicians and computer scientists can write formal proofs that are mechanically verified for correctness.

Think of it as a programming language for mathematics where:
- Every theorem must be proven
- Every proof is checked by the kernel for logical consistency
- The system prevents you from stating false theorems

### Why Lean 4 for This Experiment?

Lean 4 is particularly well-suited for representation research because:

1. **Elaborated Environment**: When Lean processes code, it fills in all implicit details — implicit arguments, typeclass instances, notation expansion. This "elaborated" form already removes some syntactic noise.

2. **Rich Type System**: Lean's type system handles universe polymorphism, dependent types, and typeclasses — structures that encode real mathematical semantics.

3. **Active Ecosystem**: Mathlib is one of the largest libraries of formally verified mathematics in existence, with over 100,000 theorems.

### The Constraint That Makes This Hard

Here's the interesting problem: even though Lean already elaborates code, there's still a gap between what the elaborator produces and what's optimal for model training.

The elaborator's job is correctness, not model friendliness. So Maith asks: **Can we transform Lean's output into something even better for learning?**

---

## Section 3: The Three Representations (A/B/C)

### Variant A: The Maith IR (Semantic Representation)

This is Maith's candidate representation — a **canonical semantic graph** that captures the true mathematical structure.

What makes it special:
- **Canonical form**: The same mathematical concept always looks the same, regardless of how it was written originally
- **Explicit structure**: Type relationships, operations, and typeclass instances are all visible as edges in a graph
- **Noise removed**: Syntactic sugar, notation variations, and implicit details are transformed into explicit semantic elements

Think of it like reducing a messy handwritten equation to a clean, standardized form where all the mathematical relationships are crystal clear.

### Variant B: Raw Source Syntax

This is the baseline: **Lean's elaborated source code** tokenized directly.

What it contains:
- All the explicit type information from Lean's elaborator
- Full namespaced identifiers like `Mathlib.Algebra.Group.Defs.neg_neg`
- Universe level annotations like `.{u_1}`
- Typeclass instances fully spelled out

The advantage: It's what Lean actually produces, with minimal transformation. The disadvantage: It's still text designed for human reading, not model training.

### Variant C: AST-Style Representation

This is a middle ground: **Lean's elaborated type expressions** tokenized in an abstract syntax tree style.

It's more structured than raw text but less semantic than Maith's graph. Think of it as breaking down the syntax tree without fully analyzing the mathematical semantics.

### Why Three Variants?

By training identical model architectures on these three representations, we can isolate whether **representation choice** affects performance — or whether model size is the only thing that matters.

If A outperforms B and C, the hypothesis is supported. If all three perform equally, representation doesn't matter — only scale does.

---

## Section 4: How Maith's Pipeline Works

### The Extraction Process

Maith runs inside the Lean environment, where it can directly access Lean's internal data structures. This is crucial — rather than parsing source text, Maith reads the actual elaborated expressions.

```
Lean Source Code
       ↓
   Lean's Elaborator
       ↓
   Elaborated Expr Trees
       ↓
   MetaExtractor (Leans reads internal structures)
       ↓
   IR Graph (Maith's semantic representation)
```

### The IR Graph Structure

Maith's representation consists of four main components:

**1. Entities**
These are the variables and terms:
- Bound variables (variables introduced by `forall` or `lambda`)
- Term entities (computed values like `neg(neg(a))`)

**2. Attributes**
Properties attached to entities:
- Sort attributes: "this type lives in Type.{u_1}"
- Typeclass attributes: "this instance proves InvolutiveNeg"

**3. Relations**
Type membership and equality:
- "Variable a has type G"
- "The result equals the input"

**4. Operations**
The actual computations:
- "Apply negation to x"
- "Generate an InvolutiveNeg instance from G"

### Canonicalization

Before encoding, the graph is normalized into a canonical form:

- Entities sorted by type then identifier
- Attributes sorted by target then key
- Operations sorted by output then inputs

This ensures that mathematically equivalent statements produce identical representations, regardless of original source variation.

### Encoding to Tokens

The canonical graph is then converted to a token sequence suitable for transformer models:

- Bound variables become `FVAR_0`, `FVAR_1`, etc.
- Term entities become `TERM_0`, `TERM_1`, etc.
- Operations and attributes get their own tokens

This produces the training data for the models.

### The Round-Trip

Importantly, Maith is designed to be **bidirectional**:

```
Lean → IR Graph → Tokens → (Model Training)
                              ↑
                     Tokens → IR Graph → Lean (Decompilation)
```

This round-trip capability means:
- We can verify information isn't lost in encoding
- We can verify model outputs decode correctly
- The decompiler reconstructs Lean syntax from the IR

---

## Section 5: The Worked Example — neg_neg

### What We're Proving

The theorem `neg_neg` states that negation is its own inverse: negating something twice returns you to where you started.

In mathematical notation: **∀(a : G), -(-a) = a**

### What Lean's Elaborator Produces

When Lean processes this theorem, it fills in all the details:

```
forall {G : Type.{u_1}}
  [inst : InvolutiveNeg.{u_1} G]
  (a : G),
  Eq.{succ u_1} G
    (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst) 
      (Neg.neg.{u_1} G (InvolutiveNeg.toNeg.{u_1} G inst) a))
    a
```

Notice all the explicit details: universe levels, the actual negation function, the typeclass instance, the equality type.

### What Maith's IR Captures

Maith transforms this into a semantic graph where the structure is explicit:

- Three bound variables: G (the type), inst (the InvolutiveNeg instance), a (the element)
- Four term entities: the type universe, the InvolutiveNeg instance, neg(a), and neg(neg(a))
- Relations showing: G is a type, a is in G, neg(neg(a)) equals a
- Operations showing: neg applied to a produces neg(a), neg applied to neg(a) produces neg(neg(a))

The graph makes the mathematical structure — the actual involution property — crystal clear.

### Why This Matters for Models

With the graph representation, a model can directly "see":
- The relationship between negation operations
- That applying negation twice returns the original value
- The type structure without syntactic noise

This is the kind of semantic understanding that might allow a smaller model to reason effectively.

---

## Section 6: Implications for Mathematical AI

### Why This Matters for Theorem Proving

Automated theorem proving has historically relied on:
- Hand-crafted search strategies
- Extensive domain knowledge encoding
- Massive compute for proof search

If representation helps, we could shift some of that burden to **better data representation** rather than more compute.

### Potential Benefits

**1. Sample Efficiency**
A model that doesn't need to learn to parse syntax implicitly could learn mathematical reasoning with fewer examples.

**2. Better Generalization**
Canonical representations might help models transfer learning from known theorems to novel ones.

**3. Improved Proof Search**
Structured semantic representations could make it easier for models to suggest relevant lemmas or tactics.

**4. Smaller, Faster Models**
If representation does the heavy lifting, we might get capable theorem-proving AI that runs on modest hardware.

### The Broader Vision

The implications extend beyond mathematics:

**Software Correctness**: Similar representation techniques could help AI reason about code — proving properties of programs, finding bugs, suggesting fixes.

**Scientific Discovery**: Mathematical AI that can reason about formal structures might assist with novel proofs and conjectures.

**Education**: Better representations of mathematical knowledge could power smarter tutoring systems.

---

## Section 7: Looking Forward

### What Comes Next

The Maith experiment is building the infrastructure to test the hypothesis:

1. ✅ Built the IR representation and extraction pipeline
2. ✅ Implemented encoding and decoding with full round-trip verification
3. ✅ Created the A/B/C model comparison framework
4. 🔄 Running experiments to measure representation effects
5. 🔄 Expanding to broader mathematical domains

### The Open Questions

The experiment is designed to answer:
- Does representation affect next-token prediction quality?
- Does representation affect theorem-proving performance?
- Which mathematical domains benefit most from semantic representation?
- What IR design choices matter most?

### The Stakes

If the hypothesis holds, it would suggest that **representation is an underutilized optimization lever** in AI development.

Just as better notation unlocked new mathematics for humans, better representations might unlock new capabilities for AI — potentially with more impact than simply scaling model size.

---

## Appendix: Key Terms and Concepts

**Elaboration**: Lean's process of filling in implicit arguments, resolving notation, and typeclass instances.

**Typeclass**: A type-level constraint (like "has involutive negation") that can have multiple implementations.

**Universe Level**: Lean's mechanism for handling polymorphism over types of different "sizes."

**Canonical Form**: A standardized representation where equivalent expressions always look the same.

**De Bruijn Indices**: A nameless representation for bound variables where `bvar n` refers to the nth enclosing binder.

**IR (Intermediate Representation)**: A data structure that captures program or mathematical structure in an abstract, analysis-friendly form.

**Round-Trip**: The property that encoding followed by decoding produces the original input — critical for lossless transformations.

---

## Supporting Materials

- **Maith Repository**: https://github.com/allenpd728/Maith
- **Research Slides**: https://maith-representation-xzja7a7.gamma.site/

---

*Document prepared as source material for NotebookLM podcast generation. All content focuses on the positive hypothesis and potential of semantic representation research. No discussion of failure modes or hypothesis disproval.*
