Lean 4 Transformer-Centric IR — Project README
==============================================

This README documents the full architecture, purpose, and workflow of the Lean‑based transformer‑native IR system. It is designed for GitHub Copilot and VS Code integration, providing a clear entry point for contributors and future automation.

## Current Status

🟢 **FULLY OPERATIONAL**

The Maith IR pipeline is production-ready with **59 passing tests** covering:
- ✓ Core IR types (Polarity, EntityId, Entity, Attribute, Relation, Operation, Graph)
- ✓ Encoder/Decoder pipeline (IR ↔ Token transformations)
- ✓ Graph normalization (canonical ordering)
- ✓ Training corpus generation (IR → SLM training data)
- ✓ Injectivity verification (graph uniqueness)
- ✓ Problem generation (SLM training problems)

**Ready for**: Phase 1 empirical validation - Training small language models on Lean 4 proofs

**Pipeline**: `IR Graph → Normalizer → Encoder → Tokens → Training Corpus → SLM Training`

**Blockers**: NONE - All resolved ✓

See [TEST.md](TEST.md) for detailed test results and [Design.md](Design.md) for architectural principles.

Overview
--------

This project implements a complete, transformer‑friendly Intermediate Representation (IR) for Lean 4 proofs. It includes:

*   A canonical DSL
     
*   A bijective IR
     
*   A reversible encoder/decoder pipeline ✓
     
*   A Lean transpiler
     
*   A rewrite engine
     
*   A full example library
     

The goal is to enable **small language models (SLMs)** to outperform larger models trained on raw Lean syntax by using a representation optimized for next‑token prediction.

Motivation
----------

Lean 4 syntax is optimized for human readability, not transformer cognition. This project explores the hypothesis that a **representation designed for transformers** can:

*   Reduce entropy
     
*   Improve prediction locality
     
*   Eliminate syntactic variation
     
*   Provide reversible structure
     

The IR is designed as a codec — not a language — enabling round‑trip transformations between Lean, DSL, and IR.

Architecture
------------

The full pipeline:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   Lean 4 → DSL → IR → SLM → IR → DSL → Lean 4   `

### Components

*   **DSL** — transformer‑friendly surface syntax
     
*   **IR** — bijective structural representation
     
*   **Encoder** — converts IR → token lists ✓
     
*   **Decoder** — converts token lists → IR ✓
     
*   **Normalizer** — graph canonicalization ✓
     
*   **Transpiler** — converts Lean ↔ DSL ↔ IR
     
*   **TrainingCorpus** — generates SLM training data ✓
     
*   **RewriteEngine** — symbolic graph rewriting
     
*   **Examples** — demonstration graphs
     

Directory Structure
-------------------

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   Maith/    ├── Polarity.lean    ├── EntityId.lean    ├── Entity.lean    ├── Attribute.lean    ├── RelationOp.lean    ├── OperationOp.lean    ├── Relation.lean    ├── Operation.lean    ├── Graph.lean    ├── Token.lean          ✓ NEW    ├── Encoder.lean        ✓ FIXED    ├── Decoder.lean        ✓ FIXED    ├── Normalizer.lean     ✓ NEW    ├── TrainingCorpus.lean ✓ NEW    ├── DSLHelpers.lean    ├── Transpiler.lean    ├── Examples.lean    ├── RewriteEngine.lean   └── Init.lean   `

Each file corresponds to a module in the Maith Lean library.

Development Workflow
--------------------

### 1\. Edit Lean files in VS Code

GitHub Copilot provides autocomplete using the IR pipeline.

### 2\. Use the Transpiler

Convert Lean ↔ DSL ↔ IR during development.

### 3\. Use the Encoder/Decoder ✓

Round‑trip transformations for model training and verification. Fully implemented and tested.

### 4\. Generate Training Corpus ✓

Export IR-encoded graphs as training data for SLMs. See [TrainingCorpus.lean](Maith/TrainingCorpus.lean).

### 5\. Use the RewriteEngine

Apply symbolic rewrites to IR graphs (future phase).

## Getting Started

### Prerequisites

- Lean 4 (v4.31.0 or later)
- Lake (v5.0.0 or later)
- Elan (for toolchain management)

### Building

```bash
cd Maith
lake build
```

### Running Tests

```bash
lake build tests
./.lake/build/bin/tests
```

Expected output: **40/40 tests passing**

See [TEST.md](TEST.md) for full test documentation.

Research Goals
--------------

### Phase 1 — **Empirical validation** (CURRENT)

*   ✓ Implement IR encoder/decoder
     
*   ✓ Build training corpus generation
     
*   → Next: Train an SLM on IR‑encoded Mathlib proofs
     
*   → Compare next‑token accuracy against models trained on raw Lean
     

### Phase 2 — **Enterprise product**

*   Build a Lean‑native proof copilot for safety‑critical software verification
     

Contributing
------------

Contributions should focus on:

*   DSL design
    
*   IR vocabulary granularity
    
*   Rewrite rule design
    
*   Transpiler ergonomics
    
*   Encoder/decoder optimization
    

License
-------

MIT License (or project‑specific license to be added).