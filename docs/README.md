# Maith Documentation

Maith extracts a canonical semantic intermediate representation (IR) from
elaborated Lean 4 terms and serializes it into token sequences for downstream
language-model training. The central research question: **can language models
become better theorem provers if trained on a representation of mathematics that
exposes semantic structure rather than source syntax?** Maith is a candidate
representation for testing that hypothesis; effectiveness is not assumed and
remains to be demonstrated empirically.

This directory is organized into five subdirectories:

| Directory | Contents |
|---|---|
| [`decisions/`](decisions/) | Decision log and index (DEC-001+) — the experiment record |
| [`experiments/`](experiments/) | Experiment designs, comparison matrix, probing task specs, next-steps |
| [`reference/`](reference/) | IR schema/format specs, encoder/normalisation audits, design notes, corpus schema, [glossary](reference/GLOSSARY.md) |
| [`history/`](history/) | Phase 5–8 results, roadmaps, and validation plans (historical, per-phase) |
| [`scratch/`](scratch/) | Session analyses, audits, and bug investigations (runs audit, variant C eval bug, v2 token analysis) |

Start with [`decisions/INDEX.md`](decisions/INDEX.md) for a navigational guide to
the decision log, and [`experiments/V2_COMPARISON_MATRIX.md`](experiments/V2_COMPARISON_MATRIX.md)
for the current controlled-experiment grid.
