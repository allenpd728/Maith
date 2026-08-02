# Documentation Categorization

This document maps Maith's documentation to audience types and use cases.

---

## Audience Types

| Audience | Description |
|----------|-------------|
| **External Researchers** | ML/NLP researchers evaluating Maith as a representation approach |
| **Project Contributors** | Developers working on the Maith codebase |
| **Experimenters** | Researchers running A/B/C experiments |
| **Podcast/Outreach** | Content creators explaining Maith to broad audiences |
| **Internal Team** | Day-to-day project tracking and decision history |

---

## Document Map

### 🧪 External Researchers

| Document | Purpose | When to Read |
|----------|---------|--------------|
| `EXAMPLE_ROUNDTRIP.md` | Step-by-step walkthrough of one declaration through the pipeline | First contact; understand what Maith does |
| `Design.md` | Architecture and design decisions | Deep dive into how/why it works |
| `EXPERIMENT_DESIGN.md` | A/B/C experiment methodology | Understand the comparison framework |
| `PODCAST_SOURCE_MAITH.md` | Podcast-ready talking points | Preparing presentations or content |
| `FUTURE_WORK.md` | Phase 6+ designs | What's next in the research agenda |

### 🔧 Project Contributors

| Document | Purpose | When to Read |
|----------|---------|--------------|
| `Design.md` | Architecture overview | Getting oriented to codebase |
| `ENCODER_FORMAT.md` | Token format specification | Working on encoder/decoder |
| `CORPUS_PIPELINE_STATUS.md` | Extraction pipeline status | Understanding corpus generation |
| `TESTING_SUMMARY.md` | Test suite documentation | Adding or running tests |
| `TEST.md` | How to run tests | Quick test commands |

### 📊 Experimenters

| Document | Purpose | When to Read |
|----------|---------|--------------|
| `EXPERIMENT_DESIGN.md` | Full A/B/C methodology | Designing or validating experiments |
| `PHASE_5_RESULTS.md` | Previous experiment results | Baseline comparison |
| `DECISION_LOG.md` | All experiment decisions and their rationale | Understanding why things were done |
| `PYTHON_PIPELINE.md` | Python tooling reference | Running dataset building or training |

### 🎙️ Podcast / Outreach

| Document | Purpose | When to Read |
|----------|---------|--------------|
| `PODCAST_SOURCE_MAITH.md` | Ready-to-use podcast script source | Preparing podcast or video content |
| `EXAMPLE_ROUNDTRIP.md` | Concrete example with side-by-side math/Lean | Making the concept accessible |
| `Design.md` (intro sections) | High-level architecture | Framing the talk |

### 📝 Internal Team

| Document | Purpose | When to Read |
|----------|---------|--------------|
| `SESSION_PROGRESS.md` | Session-by-session development log | Catching up on recent work |
| `LATEST_FIXES.md` | Recent bug fixes and changes | Recent development context |
| `CHANGELOG.md` | Historical change record | Full history |
| `DECISION_LOG.md` | All design decisions with rationale | Understanding choices made |

---

## Use Case Flow

```
New to Maith?
├─→ Read EXAMPLE_ROUNDTRIP.md (concrete example)
├─→ Read PODCAST_SOURCE_MAITH.md (accessible overview)
└─→ Read Design.md (architecture)

Want to contribute code?
├─→ Read Design.md
├─→ Read ENCODER_FORMAT.md
└─→ Read TESTING_SUMMARY.md

Running experiments?
├─→ Read EXPERIMENT_DESIGN.md
├─→ Read PYTHON_PIPELINE.md
└─→ Check DECISION_LOG.md for context

Making a podcast/video?
├─→ Read PODCAST_SOURCE_MAITH.md
└─→ Use EXAMPLE_ROUNDTRIP.md for concrete examples

Catching up on project history?
├─→ Read SESSION_PROGRESS.md
├─→ Read LATEST_FIXES.md
└─→ Check CHANGELOG.md for full history
```

---

## Quick Reference: By Filename

| Document | Audience | Use Case |
|----------|----------|----------|
| `CHANGELOG.md` | Internal | Historical change tracking |
| `CORPUS_PIPELINE_STATUS.md` | Contributors | Understanding extraction |
| `DECISION_LOG.md` | Experimenters | Decision rationale |
| `DECOMPILER_HANDOVER.md` | Contributors | Decompiler implementation |
| `Design.md` | All | Architecture overview |
| `ENCODER_FORMAT.md` | Contributors | Token format reference |
| `EXAMPLE_ROUNDTRIP.md` | Researchers, Podcast | Concrete example walkthrough |
| `EXPERIMENT_DESIGN.md` | Researchers, Experimenters | A/B/C methodology |
| `FUTURE_WORK.md` | Researchers | Phase 6+ designs |
| `LATEST_FIXES.md` | Internal | Recent changes |
| `PHASE_5_RESULTS.md` | Researchers, Experimenters | Previous results |
| `PHASE_7_DESIGN.md` | Researchers | Next phase design |
| `PODCAST_SOURCE_MAITH.md` | Outreach | Podcast/video source |
| `PYTHON_PIPELINE.md` | Experimenters | Python tooling |
| `REPRESENTATION_EVOLUTION.md` | Researchers | IR candidate strategy |
| `SESSION_PROGRESS.md` | Internal | Development log |
| `TEST.md` | Contributors | Test commands |
| `TESTING_SUMMARY.md` | Contributors | Test suite docs |

---

## Summary Table

| Audience | Primary Docs | Secondary Docs |
|----------|--------------|----------------|
| External Researchers | EXAMPLE_ROUNDTRIP, Design, EXPERIMENT_DESIGN | FUTURE_WORK, PHASE_5_RESULTS |
| Project Contributors | Design, ENCODER_FORMAT, TESTING_SUMMARY | CORPUS_PIPELINE_STATUS, DECOMPILER_HANDOVER |
| Experimenters | EXPERIMENT_DESIGN, PYTHON_PIPELINE | DECISION_LOG, PHASE_5_RESULTS |
| Podcast/Outreach | PODCAST_SOURCE_MAITH | EXAMPLE_ROUNDTRIP |
| Internal Team | SESSION_PROGRESS, LATEST_FIXES, DECISION_LOG | CHANGELOG |
