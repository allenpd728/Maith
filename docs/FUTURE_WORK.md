# Future Work

> **This document is superseded.** See `docs/PHASE_7_ROADMAP.md` for the current
> prioritised plan to outperform the AST baseline (Variant C), including the decision
> tree, remaining IR improvements, and domain generalisation track.
>
> This file is retained for git history. Do not update it — update PHASE_7_ROADMAP.md.

---

## Historical context (as of Phase 6 closure, 2026-08-02)

Phase 6 confirmed the A/B/C gap is stable at ~0.17pp. DEC-006 (cold-start embedding
confound) was subsequently closed by DEC-021 (embedding projection experiment, 2026-08-03).
The IR was audited post-DEC-021 and v1.3.0 implemented polarity removal and casesOn/recOn
normalisation. A v1.3.0 retraining run is in progress.

For the full current roadmap, open items, and decision gates, see:

- `docs/PHASE_7_ROADMAP.md` — prioritised path to beating Variant C
- `docs/IR_V2_FIX_SPEC.md` — Fix 1+2 specification and implementation notes
- `docs/IR_V2_PROPOSAL.md` — full v2 IR design proposal
- `docs/DECISION_LOG.md` — DEC-001 through DEC-021+
