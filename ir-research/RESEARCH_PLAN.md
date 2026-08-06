# IR Design Research Plan

## Goal

Redesign the Maith IR so that semantic tokens carry measurable signal rather
than noise. The current A variant (1,236 tokens) yields higher perplexity than
the 11-token flat baseline, indicating the semantic layer is hurting rather
than helping. This research investigates *why* and proposes a v2 IR that fixes
the root causes.

## Workflow

- **Branch**: `kit/ir-design-research`
- **Directory**: `ir-research/`
  - `scripts/`   — analysis and tooling scripts
  - `findings/`  — output files and summaries from each step
  - `proposals/` — IR schema proposals generated from findings
- **OpenHands** has SSH access to the sandbox and can run all steps locally
- **Kit** reviews findings at each stage before merge into `kit/dev`
- OpenHands works on feature branches (`openhands/ir-*`), Kit merges after review

---

## Steps

### Step 1 — AST Explorer (Kit, local) ✅ COMPLETE
**Script**: `scripts/ast_explorer.py`  
**Output**: `findings/ast_explorer_output.json`  
**Committed**: `7e6210e`

For a sample of declarations, pull `leanExpr` from `corpus.jsonl` and render
the elaborated AST as an annotated tree: node type, depth, arity, and child
count. Also extract and count all distinct AST node type prefixes across the
full corpus.

**Key findings:**

1. **Parser bug confirmed, not IR gap — but the parser is more broken than
   initially described.** The 52,709 `Expr.unknown` nodes were a bug in the
   string parser, but the failure modes are multiple and inconsistent:
   - Punctuation (`->`, `=>`, `:`) splits into orphan sibling nodes
   - Unicode subscripts (`₁`, `₂`, `₃`) detach from their identifier — `G₁`
     becomes two sibling nodes `G` and `₁` rather than one token
   - Multi-token binder groups (`α : Sort.{u}`, `{n : Nat}`) are sometimes
     swallowed whole as a single `Expr.unknown` leaf and sometimes split
   The parser's behavior is inconsistent across examples, not a single clean
   failure mode. **This parser is retired. It must not be reused or patched
   for any downstream measurement step.** `MetaExtractor.lean` has explicit
   match arms for every constructor. **Roundtrip is not catastrophically broken.**

2. **Extraction success rate: 4,029/4,029 (100%).** `stats.json` reports zero
   `irConstructionFailed`, zero `encodingFailed`. Both hard-fail paths
   (`bvar` out-of-scope, operation arity) never fire in this corpus.

3. **mvar frequency: 0.** No `?`-prefixed var entities appear in any of the
   153,118 graph entities across the corpus. Verified two independent ways:
   (a) `collect_graph_entity_stats()` in `ast_explorer.py` iterating
   `entry["graph"]["entities"]` parsed from JSON, and (b) a direct `jq`
   query on `corpus.jsonl` with no Python involved — both return 0. The mvar
   semantic bug is not consequential for this Mathlib subset. **Reprioritised
   below the four IR design issues.**

4. **Graph entity distribution (ground truth):**
   - `term`: 113,683 (74.2%) — anonymous fresh terms
   - `bound`: 37,741 (24.6%) — binder-scoped entities
   - `var`: 1,694 (1.1%) — named constants

**Confirmed real problems (ranked by likely impact):**
1. Polarity always `neut` — 24% of tokens are zero-information noise
2. Entity ID sparsity — rare `gen:*` tokens collapsed to `GEN_UNK`
3. Normalisation gaps — `casesOn`/`recOn`, `mk`/`mk._flat_ctor`
4. Attribute row density — 85% of entities have zero A rows
5. mvar mapping — not consequential for this corpus (0 occurrences)

Review gate: ✅ Passed — diagnosis confirmed coherent, Step 2 scoped.

---

### Step 2 — Coverage Statistics (Kit, local)
**Script**: `scripts/ir_coverage.py`  
**Output**: `findings/ir_coverage.json`

**Data source: `.graph` JSON only.** All measurements read `entry["graph"]`
fields parsed from `corpus.jsonl` directly. The `leanExpr` string parser from
`ast_explorer.py` is retired and must not be used or extended here — it has
multiple inconsistent failure modes (subscript splitting, binder swallowing)
that make any string-level parse of `leanExpr` untrustworthy for coverage
statistics. The mvar check in Step 1 is the correct template: structured JSON
field access, no string parsing of pretty-printed Lean.

Measurements:

**IR design quality:**
- Polarity distribution — count `neut` vs `pos` vs `neg` across all entities,
  operations, relations, and attributes
- Entity ID frequency distribution — token frequency histogram, long-tail
  count, GEN_UNK collapse rate from `vocab_A.json`
- Attribute row density — % of entities with zero A rows; distribution of
  A-row counts per entity
- Normalisation collision rate — count graph pairs where leanExpr differs but
  token sequence is identical (`casesOn`/`recOn`, `mk`/`mk._flat_ctor`)

**Hard-fail path quantification (added from Step 1 review):**
- bvar-out-of-scope potential: for each graph, compare max bvar index referenced
  in operations/relations against binder depth (bound entity count) — flag any
  declaration where a bvar index could exceed context depth
- Operation arity: for each operation with op in {neg, add, sub, mul, div, pow},
  count input entities and flag if below expected minimum arity

**mvar tracking:**
- Confirm 0 `?`-prefixed var entities across full corpus (already measured in
  Step 1; Step 2 should record the confirmed count for completeness)

Review gate: Kit reviews coverage report and drafts the list of highest-impact
fixes before handing to OpenHands.

---

### Step 3 — IR Schema Iterator (OpenHands, SSH) ✅ COMPLETE
**Script**: `scripts/ir_schema_iterator.py`  
**Output**: `proposals/ir_v2_schema.json`, `findings/schema_comparison.json`
**Branch merged**: `openhands/ir-schema-step3`

4 changes evaluated, 3 approved:

| Change | Result | Status |
|---|---|---|
| C1 Polarity removal | ~32.6% theoretical token reduction | ✅ Approved |
| C2 Typeclass enrichment | 134 typeclasses, 3,149 decls enriched, fixes 51-typeclass collapse | ✅ Approved |
| C3 Attribute sparsity | 86.35% → ~0% zero-attr (projected) | ❌ Deferred — needs Lean cross-check |
| C4 GEN_UNK mitigation | ~1,564 of 1,955 GEN_UNK tokens resolvable via namespace buckets | ✅ Approved |

Review gate: ✅ Passed — Kit reviewed schema_comparison.json, C1/C2/C4 approved.

---

### Step 4 — Compression Gate (OpenHands, SSH)
**Script**: `scripts/compression_gate.py`  
**Output**: `findings/compression_gate.json`  
**Prompt**: `OPENHANDS_STEP4_PROMPT.md`
**Branch**: `openhands/ir-compression-gate`

Run the approved v2 schema (C1+C2+C4) across the full corpus and measure:
- Bits-per-token (BPT) vs current IR and flat baseline
- Vocabulary coverage (% of token positions filled by high-frequency tokens)
- Redundancy ratio (entropy of token distribution)
- Collision rate (projected reduction from C2 typeclass enrichment)

C3 included as an informational column — not a required pass criterion.

Gate criteria (all must pass for v2_C1C2C4):
- BPT(v2) < BPT(current_A)
- Redundancy(v2) < Redundancy(current_A)
- Coverage(v2) >= Coverage(current_A)

If gate passes → produce prompt to update `MetaExtractor.lean` and regenerate
datasets. If gate fails → return to Step 3 with diagnosis.

Review gate: Kit reviews `compression_gate.json` before any training run.

---

## Known IR Problems (from current findings)

| Problem | Evidence | Impact |
|---|---|---|
| Polarity always `neut` | 99.8% of rows | 24% of tokens are noise |
| Entity ID sparsity | IDs like `FVAR_0` appear rarely | Model can't build stable reps |
| Normalisation gaps | `casesOn` vs `recOn`, `mk` vs `mk._flat_ctor` | Same structure, different tokens |
| Attribute row density | 85% of entities have zero attribute rows | A rows mostly noise |
| AST nodes dropped | Step 1 complete — string parser retired, graph-only approach confirmed | Step 2 measures from .graph JSON |

---

## Merge Policy

- Each step's outputs are committed to `kit/ir-design-research` before the
  next step starts.
- OpenHands works on `openhands/ir-schema-v2` branched from
  `kit/ir-design-research`.
- Kit reviews OpenHands PRs before merge into `kit/ir-design-research`.
- `kit/ir-design-research` merges into `kit/dev` only after Step 4 gate passes
  and a training run confirms perplexity improvement.
