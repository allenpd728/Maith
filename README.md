Lean 4 Transformer-Centric IR — Project README
==============================================

This README documents the full architecture, purpose, and workflow of the Lean‑based transformer‑native IR system. It is designed for GitHub Copilot and VS Code integration, providing a clear entry point for contributors and future automation.

Overview
--------

This project implements a complete, transformer‑friendly Intermediate Representation (IR) for Lean 4 proofs. It includes:

*   A canonical DSL
    
*   A bijective IR
    
*   A reversible encoder/decoder pipeline
    
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
    
*   **Encoder** — converts IR → token lists
    
*   **Decoder** — converts token lists → IR
    
*   **Transpiler** — converts Lean ↔ DSL ↔ IR
    
*   **RewriteEngine** — symbolic graph rewriting
    
*   **Examples** — demonstration graphs
    

Directory Structure
-------------------

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   lean-ir/    ├── Polarity.lean    ├── EntityId.lean    ├── Entity.lean    ├── Attribute.lean    ├── RelationOp.lean    ├── OperationOp.lean    ├── Relation.lean    ├── Operation.lean    ├── Graph.lean    ├── Encoder.lean    ├── Decoder.lean    ├── DSLHelpers.lean    ├── Transpiler.lean    ├── Examples.lean    └── RewriteEngine.lean   `

Each file corresponds to a page in the Copilot Project workspace.

Development Workflow
--------------------

### 1\. Edit Lean files in VS Code

GitHub Copilot provides autocomplete using the IR pipeline.

### 2\. Use the Transpiler

Convert Lean ↔ DSL ↔ IR during development.

### 3\. Use the Encoder/Decoder

Round‑trip transformations for model training and verification.

### 4\. Use the RewriteEngine

Apply symbolic rewrites to IR graphs.

Research Goals
--------------

Phase 1 — **Empirical validation**

*   Train an SLM on IR‑encoded Mathlib proofs
    
*   Compare next‑token accuracy against models trained on raw Lean
    

Phase 2 — **Enterprise product**

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