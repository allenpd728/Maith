Design Notes
============

These notes document the architectural reasoning behind the Lean‑based transformer‑centric IR. They explain why each layer exists, how the components interact, and what constraints shaped the design.

Core Principles
---------------

### **1\. Transformers prefer structure over syntax**

Lean syntax is optimized for humans. Transformers prefer:

*   fixed vocabularies
    
*   predictable token boundaries
    
*   shallow nesting
    
*   reversible structure
    
*   uniform patterns
    

The IR is designed to minimize entropy and maximize locality.

### **2\. Reversibility is non‑negotiable**

Every IR token must map back to a Lean construct. This ensures:

*   round‑trip correctness
    
*   verifiable SLM outputs
    
*   safe rewriting
    

### **3\. Graphs are the universal representation**

Math expressions, proofs, and definitions all reduce to graphs. Graphs provide:

*   explicit dependency structure
    
*   typed edges
    
*   uniform encoding
    
*   transformer‑friendly traversal
    

DSL Design
----------

The DSL is intentionally minimal. It avoids:

*   syntactic sugar
    
*   implicit coercions
    
*   overloaded notation
    

Instead, it uses:

*   explicit polarity
    
*   explicit operators
    
*   explicit entity IDs
    

This makes the DSL predictable for both humans and models.

IR Vocabulary
-------------

The IR vocabulary is small and canonical. It includes:

*   **Entity** — atomic units
    
*   **Attribute** — metadata
    
*   **Relation** — semantic edges
    
*   **Operation** — constructive steps
    
*   **Graph** — container
    

Each component has a fixed shape and a fixed set of fields.

Encoding Strategy
-----------------

The encoder produces token lists with:

*   stable ordering
    
*   reversible structure
    
*   no ambiguity
    

Tokens are designed for next‑token prediction.

Decoding Strategy
-----------------

The decoder reconstructs IR graphs from token lists. It must:

*   detect malformed sequences
    
*   enforce type correctness
    
*   preserve graph invariants
    

Transpiler Constraints
----------------------

The transpiler bridges Lean ↔ DSL ↔ IR. It must:

*   preserve semantics
    
*   avoid lossy transformations
    
*   support round‑trip equivalence
    

Rewrite Engine Philosophy
-------------------------

Rewrites operate on graphs, not syntax. This enables:

*   algebraic simplification
    
*   structural normalization
    
*   symbolic reasoning
    

Rewrites must be:

*   deterministic
    
*   terminating
    
*   composable
    

Future Directions
-----------------

*   richer operator vocabularies
    
*   probabilistic rewrite rules
    
*   SLM‑driven autocompletion
    
*   VS Code integration
    
*   IR‑native proof search
    

These notes serve as the conceptual backbone of the project, guiding future contributors and ensuring architectural consistency.