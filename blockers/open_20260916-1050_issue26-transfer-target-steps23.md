# Blocker: #26 — transfer-target list needs maintainer steps 2-3 (and step 4 is blocked upstream)

**Opened:** 2026-09-16T11:0x UTC by openhands `run=20260916-1050-e8a2`
**Issue:** Maith #26 (`status:blocked-needs-input`)
**Deliverable so far:** `docs/experiments/TRANSFER_TARGETS.md` (DEC-042)

## What was completed

Step 1 of `BENCHMARK_CORPUS_PLAN.md` Part 2 — the research pass. Executed with web
research (Tavily, primary sources preferred) because the plan's named tool (DeepSeek)
was not available in this environment. Output: 10 targets (T1-T10) plus 2 screening
results (N1/N2), each with citation, status, documented hardness, and a per-target
verification note.

## What is missing — three distinct gaps, all outside agent authority

### Gap 1 — Step 2 (filtering): the plan assigns this to the maintainer

`BENCHMARK_CORPUS_PLAN.md` step 2 applies three plain-language criteria (specific?
hardness-documented? restricted-enough model?) and is explicitly "your job, plain
English only". I recorded a **preliminary** table in the doc so the maintainer can
check rather than start cold, but it is not the sign-off and the doc says so.

**Decision needed within step 2:** whether the list should contain **only OPEN**
targets. If yes, it shrinks to T2, T3, T4-gap, T7-general, T8, T9 — every one of
which is *believed to require* the circuit lower bounds that are currently blocked.
That changes what a candidate φ could realistically reach, so it should be decided
before targets are frozen, not discovered afterwards.

### Gap 2 — Step 3 (cross-check): the plan says a second, independent model's pass

The plan is explicit that this is mandatory given how load-bearing the list is.

**What I did instead (mitigation, not the step):** multiple independent sources per
target (course notes, surveys, arXiv, Dagstuhl LIPIcs, proceedings), and I verified
the two load-bearing claims against their **primary** sources — which caught a real
error (T6 was drafted from a snippet as "bipartite matching 2^{n^{1/3−o(1)}}"; the
arXiv abstract shows the headline theorem is **perfect** matching at
**2^{n^{Ω(1)}}`, bipartite separate). Recorded in the doc's cross-check section.

**What is still needed:** a genuinely independent pass by a different model,
differently prompted. I cannot be the independent check on my own research.

### Gap 3 — Step 4 (formalization): blocked upstream on PleaNP #102

Maith cannot `require PleaNP` until PleaNP's root `lakefile` shim lands (PleaNP's
Lake package is at `lean/`; Lake resolves a dependency's root at the repo root).
Verified empirically in DEC-040.

**New scope finding recorded in the doc:** even after #102 lands, **T8/T9
(proof complexity: Frege, AC⁰-Frege) and T7's communication-complexity half likely
need substrate `PleaNP.Circuits` does not have.** `PleaNP.Circuits` is
Boolean-circuit-shaped (gates, families, AC⁰, monotone). Proof systems and
communication protocols are different models. So some targets need *new substrate*,
not just import — this is a scope decision for the maintainer, not an implementation
choice, which is why it is recorded here rather than absorbed into the estimate.

### Related gap — the doc-quality follow-through

The doc contains a `Verification:` note per target, but three secondary citations
rest on summaries rather than the papers themselves (T3's Arora-Barak addendum, T8's
Feige-Filmus cross-reference, T10's Smolensky via the CKK survey). They are marked
as such. A maintainer-run cross-check is the natural place to promote them to primary.

## What unblocks each gap

| Gap | Unblocked by |
|---|---|
| 1 (filtering) | maintainer applies the three criteria; decides OPEN-only vs open+resolved |
| 2 (cross-check) | maintainer runs an independent model pass |
| 3 (formalization) | PleaNP #102 lands **and** maintainer decides the proof/communication substrate question |

## Why this is a blocker and not an implementation detail

Per `docs/MULTI_AGENT_WORKFLOW.md` §Blockers, a blocker is for spec-level ambiguity
only the human can resolve. All three gaps qualify: the plan assigns steps 2-3 to the
maintainer by role, and Gap 3's substrate question changes scope rather than
mechanism.

The alternative — writing stand-in `Maith/Benchmark/` definitions to complete the
formalization table — was **deliberately rejected**. It would violate the point of
option (a) (import the *real* PleaNP models) and produce statements type-checking
against the wrong substrate, which is the failure mode the integrity gates exist to
catch. #26's DoD explicitly permits "a recorded reason it could not be formalized",
so the honest record is the correct output.

## Not done / next agent should not redo

- Do **not** re-run the step-1 research; it is complete and dated.
- Do **not** claim #26 again until at least Gaps 1-2 are resolved; it is
  `status:blocked-needs-input`.
- The Part 2 work already unblocks nothing downstream on its own: #30 (first
  candidate batch) needs #26 **and** #27 **and** #29, and no φ has been proposed yet.
