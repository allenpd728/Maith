# Maith Documentation

Maith extracts a canonical semantic intermediate representation (IR) from
elaborated Lean 4 terms and serializes it into token sequences for downstream
language-model training. The central research question: **can language models
become better theorem provers if trained on a representation of mathematics that
exposes semantic structure rather than source syntax?** Maith is a candidate
representation for testing that hypothesis; effectiveness is not assumed and
remains to be demonstrated empirically.

This directory is organized into six subdirectories plus top-level protocol docs:

| Directory | Contents |
|---|---|
| [`decisions/`](decisions/) | Decision log and index (DEC-001+) — the experiment record |
| [`experiments/`](experiments/) | Experiment designs, comparison matrix, probing task specs, next-steps, and the active [axiom-discovery track](experiments/AXIOM_DISCOVERY.md) + [benchmark corpus plan](experiments/BENCHMARK_CORPUS_PLAN.md) |
| [`reference/`](reference/) | IR schema/format specs, encoder/normalisation audits, design notes, corpus schema, [glossary](reference/GLOSSARY.md) |
| [`history/`](history/) | Phase 5–8 results, roadmaps, and validation plans (historical, per-phase) |
| [`scratch/`](scratch/) | Session analyses, audits, and bug investigations (runs audit, variant C eval bug, v2 token analysis) |

Top-level protocol docs:

| Doc | Contents |
|---|---|
| [`TOOLCHAIN_AND_CI.md`](TOOLCHAIN_AND_CI.md) | The PleaNP-derived CI, toolchain bootstrap, two-tier integrity gates, and single-`dev`-branch protocol |
| [`MULTI_AGENT_WORKFLOW.md`](MULTI_AGENT_WORKFLOW.md) | The PleaNP-derived task protocol — run-ids, atomic claims, sweeps, `blocked by` lineages, done-evidence, blockers |
| [`../tooling/gates/README.md`](../tooling/gates/README.md) | The integrity scanners (hygiene, vacuity, lethality) and their fixtures |
| [`../blockers/README.md`](../blockers/README.md) | Agent blocker files (`open_*` → `closed_*`) |

Start with [`decisions/INDEX.md`](decisions/INDEX.md) for a navigational guide to
the decision log, and [`experiments/V2_COMPARISON_MATRIX.md`](experiments/V2_COMPARISON_MATRIX.md)
for the current controlled-experiment grid. For the active research direction, read
[`experiments/AXIOM_DISCOVERY.md`](experiments/AXIOM_DISCOVERY.md).
