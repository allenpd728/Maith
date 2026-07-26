Design Notes
============

These notes document the architectural reasoning behind the Lean‑based transformer‑centric IR. They explain why each layer exists, how the components interact, and what constraints shaped the design.

## Extraction Architecture

Maith extracts IR directly from Lean's elaborated environment — not from source strings.
`MetaExtractor.lean` walks `ConstantInfo`/`Expr` trees from the live `Environment`, where
implicit arguments, notation expansion, typeclass resolution, and macro expansion are already
resolved. This is the only extraction path; no string-based Lean parser exists in the codebase.

The high-level flow is:

```
Lean Environment → MetaExtractor.lean → IR Graph → Normalizer.lean → canonical graph
  → Encoder.lean (v1.0.0) → token sequence → CorpusSerializer.lean → corpus.jsonl
  → python/ → A/B/C dataset variants → fine-tuning
```

`Transpiler.lean` is debug-only: it formats IR as a human-readable string using a different
ID format (`var:/term:/bound:` prefixes) than the encoder. It is not in the training pipeline.

---

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

The IR vocabulary is small, canonical, and stable. Encoder v1.0.0 produces:

- **Structural tokens**: `GRAPH_BEGIN`, `GRAPH_END`, `E`, `A`, `R`, `O`, polarity markers
- **Positional bound IDs**: `BVAR_0`–`BVAR_31`, `BVAR_MANY` (replaces scoped `b(...)` strings)
- **Positional term IDs**: `TERM_0`–`TERM_31`, `TERM_MANY` (replaces `t<n>` strings)
- **Semantic constants**: `HMul.hMul`, `Eq`, etc. (stable across corpus)
- **Generic operations**: `gen:<FullName>` (rare ones map to `GEN_UNK` at dataset-build time)

Full specification: `docs/ENCODER_FORMAT.md`.

Current corpus vocabulary: **5,577 unique tokens** across 2,554 declarations (4 modules).

Encoding Strategy
-----------------

The encoder (`Encoder.lean`) produces token lists from a full graph in one pass:

1. Build a BVAR map: walk all entity IDs in the graph in order, assign `BVAR_N` to each new
   `EntityId.bound` scope encountered. This ensures positional consistency within a graph.
2. Encode entities, attributes, relations, and operations using the BVAR map.
3. Wrap with `GRAPH_BEGIN` / `GRAPH_END`.

Tokens are designed for next-token prediction. The positional encoding means structurally
identical graphs from different declarations produce identical token sequences — declaration-name
noise is removed by design.

Decoding Strategy
-----------------

The decoder (`Decoder.lean`) reconstructs IR graphs from token lists. It is total: unknown
tokens are skipped, missing `GRAPH_BEGIN` returns an empty graph rather than a panic.

Supports both v1.0.0 (`BVAR_N`/`TERM_N`) and v0.1.0 legacy (`b(...)`/`t<n>`) formats.


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