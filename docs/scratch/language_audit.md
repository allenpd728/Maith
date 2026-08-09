# Language & Framing Audit — v1/v2/Future IR

**Auditor role:** Technical writer
**Date:** 2026-08-09
**Scope:** All `docs/` + `README.md`. Assessed how v1 IR, v2 changes, and future
candidates are described, and whether the framing is consistent, accurate, and tells
a coherent story.

## The story the docs should tell (in one paragraph)

Maith's v1 IR was a **graph-based semantic representation** — declarations extracted
from elaborated Lean as entity/relation/operation (E/R/O) graphs, serialized to token
sequences with per-entity polarity, granular per-constant operation names (`gen:FullName`),
and structural IO markers. Analysis (DEC-021–024) showed ~40–50% of those tokens were
structural noise or vocabulary dilution that didn't help prediction. **v2 strips that
noise and collapses granular tokens into coarse, high-frequency buckets** (C1 drops
polarity, C4 collapses ~2,000 rare `gen:FullName` tokens into ~10 `GEN_<area>` buckets)
while **adding one targeted semantic signal** (C2 typeclass names) — trading graph
granularity for a leaner 601-token vocab that trains more sample-efficiently (1.2751 →
1.2361). Future work is a **search for an optimized IR**: incremental v2.x tweaks
(revised buckets, C3 attribute pruning) or a v3 redesign (different graph structure,
cross-graph identity), evaluated against the same fixed protocol and 2×2 control grid.

No single doc currently states this story end-to-end. The pieces are scattered across
`Design.md` (graph framing), `IR_V2_PROPOSAL` (keep/strip framing), `ENCODER_FORMAT`
(mechanics), and `EXPERIMENT_DESIGN` (protocol). The narrative gap is the main issue.

## Assessment by dimension

### 1. How v1 IR is described — strong in places, weak in others

**Good:** `Design.md` and `IR_V2_PROPOSAL` frame v1 explicitly as a graph:
"semantic graph → tokens," "E/R/O row structure is the core hypothesis," "graph
representation." `README.md` calls it a "canonical semantic representation."

**Weak:** `EXPERIMENT_DESIGN.md` and `V2_COMPARISON_MATRIX.md` say "Maith IR tokens"
without "graph" — a reader doesn't learn that the IR *is* a serialized graph, not just
a custom tokenization. `ENCODER_FORMAT.md` documents E/A/R/O rows but never says "this
is a graph being linearized" — the graph framing is implicit.

**Fix:** Add one sentence to `EXPERIMENT_DESIGN` and `V2_COMPARISON_MATRIX` variant-A
descriptions: "Variant A serializes Maith's **semantic IR graph** (entity/relation/
operation rows extracted from elaborated Lean) into token sequences."

### 2. How v2 is described — mechanics without the "stripping" narrative

**Good:** `IR_V2_PROPOSAL` frames v2 correctly as "streamlined IR that removes
identified noise while preserving core semantic content," with explicit "What to Keep"
vs "What to Remove/Collapse" sections. This is the right framing — v2 as a
simplification/compression of the v1 graph.

**Weak:** `ENCODER_FORMAT.md`'s v2 section describes C1/C2/C4 by their mechanics
(polarity removal, typeclass attr, GEN bucketing) but doesn't connect them into the
"v2 strips graph granularity for training efficiency" story. Specifically:
- **C1 is undersold:** described as "schema cleanup with no token-stream effect beyond
  v1.3.0" — technically accurate (v1.3.0 already dropped polarity) but a reader thinks
  "then why is it a v2 change?" Should say: "C1 formalises the v1.3.0 polarity removal
  as part of the v2 IR — polarity was ~24% of v1 tokens and carried no signal."
- **C4 isn't framed as granularity collapse:** described as "bucketing" but not as
  "collapses ~2,000 granular per-constant `gen:FullName` tokens (vocabulary dilution)
  into ~10 coarse area buckets." The "trading graph granularity for frequency" framing
  is missing.
- **C2 isn't framed as the one addition:** should be clear that C2 is the *only* v2
  change that *adds* semantic signal (typeclass names), while C1/C4 *remove/collapse*.

**Fix:** Add a 2-sentence v2 narrative lead-in to `ENCODER_FORMAT.md`'s v2 section and
`V2_COMPARISON_MATRIX.md`'s variant-A description: "v2 simplifies the v1 graph
representation for training: C1/C4 strip structural noise and collapse granular
tokens (polarity, per-constant `gen:` names) into coarse high-frequency buckets,
reducing vocab 1,236 → 601; C2 adds one targeted semantic signal (typeclass names)."

### 3. How future changes are framed — improved but still mixed

**Good:** `EXPERIMENT_DESIGN.md` (post-reframe) says "search for a reasonably optimized
IR" / "IR candidate — a v2.x tweak, a v3 redesign, or an experimental tweak." This is
the right framing.

**Weak — residual version-centric language:**
- `ENCODER_FORMAT.md` line ~132: "so v3 implementers do not re-introduce polarity
  tokens" — still says "v3 implementers" (the reframe caught "v3 implementer warning"
  but missed this second occurrence in the C1 section).
- `V2_COMPARISON_MATRIX.md` line 104: "a future v3 IR" — version-centric.
- `V2_NEXT_STEPS.md` C3 section: framed as a v2 cleanup item, not as a candidate
  iteration in the search.

**Fix:** Replace "v3" with "a future IR candidate" in the residual spots. Reframe the
C3 section as "a candidate iteration (prune attribute noise) to evaluate against the
control grid, if B-small shows the IR is doing real work."

### 4. Terminology inconsistencies

| Issue | Where | Fix |
|---|---|---|
| "Fix N" (v1 era: Fix 2 = polarity, Fix 3 = IO markers) vs "CN" (v2 era: C1 = polarity, C4 = buckets) — mapping never stated | `DECISION_LOG` uses Fix N; `ENCODER_FORMAT`/`V2_*` use CN | Add a one-line mapping note in `ENCODER_FORMAT` v2 section: "C1 corresponds to the v1-era 'Fix 2' (polarity removal, DEC-022); C4 is new in v2." |
| "v2" / "v2.0.0" / `semantic_graph_ir_v2_0_0` used interchangeably | all docs | Acceptable; pick `semantic_graph_ir_v2_0_0` as the formal id, "v2" as shorthand. State once in ENCODER_FORMAT. |
| "representation" vs "tokenization" vs "IR format" sometimes conflated | EXPERIMENT_DESIGN, ENCODER_FORMAT | "Representation" = what the model sees (conceptual); "IR format" / "tokenization" = the mechanism. Use "representation" for the hypothesis-level framing, "format" for the spec. |
| `Design.md` architecture diagram says "Encoder.lean (v1.2.0)" — stale | Design.md line 30, 60 | Update to "Encoder.lean (v2.0.0)". |
| `IR_V2_PROPOSAL` says "gen:* Named Constants — Keep unchanged (with GEN_UNK threshold)" — but v2 CHANGED this (C4 buckets them) | IR_V2_PROPOSAL "Keep" section | Add a note: "Update: C4 (DEC-026) bucketed gen:* tokens by module rather than keeping them granular." |

## Recommended canonical framing (adopt across docs)

**v1 IR:** "a graph-based semantic representation — declarations extracted from
elaborated Lean as entity/relation/operation graphs, serialized to token sequences."

**v2 IR:** "simplifies the v1 graph for training: strips structural noise (C1 polarity)
and collapses granular semantic tokens into coarse high-frequency buckets (C4 GEN
module buckets, vocab 1,236 → 601), while adding one targeted semantic signal (C2
typeclass names). The graph structure (E/R/O) is preserved; what changes is which
graph attributes become tokens and how granularly."

**Future candidates:** "an IR-candidate search — each candidate (a v2.x tweak or a v3
redesign) is evaluated against the same fixed protocol and 2×2 control grid (with
B-small as the size control). The goal is a reasonably optimized IR that supports the
representation hypothesis."

## Prioritized edits

1. **Add the v2 narrative lead-in** to `ENCODER_FORMAT` v2 section + `V2_COMPARISON_MATRIX`
   variant-A description (the "strips noise / collapses granularity / adds typeclass"
   sentence).
2. **Fix the C1 undersell** in `ENCODER_FORMAT` (not "no token-stream effect" — "formalises
   the v1.3.0 polarity removal as part of v2; polarity was ~24% of v1 tokens").
3. **Fix residual "v3" language** in `ENCODER_FORMAT` C1 section, `V2_COMPARISON_MATRIX`
   line 104, `V2_NEXT_STEPS` C3 section → "future IR candidate."
4. **Add the Fix-N → CN mapping note** in `ENCODER_FORMAT` v2 section.
5. **Add graph framing** to `EXPERIMENT_DESIGN` + `V2_COMPARISON_MATRIX` variant-A
   descriptions ("serializes the semantic IR graph").
6. **Update `Design.md`** stale "v1.2.0" → "v2.0.0" + add the v2 IR note to the
   architecture description.
7. **Add the "gen:* Keep → C4 changed" note** to `IR_V2_PROPOSAL`.
