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
- **Kit** does Steps 1–2 locally (corpus access required)
- **OpenHands** does Step 3 (script authoring, SSH access for Lean + local runs)
- **Kit** reviews findings at each stage before merge into `kit/dev`

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

1. **Parser bug confirmed, not IR gap.** The 52,709 `Expr.unknown` nodes from
   the initial run were a bug in the string parser — `_KEYWORD_RE` didn't
   handle `->`, `=>`, `:`, unicode subscripts (`₁ ₂`), or Greek variable names.
   These are pretty-print artefacts with no corresponding `Lean.Expr`
   constructor. `MetaExtractor.lean` has explicit match arms for every
   constructor. **Roundtrip is not catastrophically broken.**

2. **Extraction success rate: 4,029/4,029 (100%).** `stats.json` reports zero
   `irConstructionFailed`, zero `encodingFailed`. Both hard-fail paths
   (`bvar` out-of-scope, operation arity) never fire in this corpus.

3. **mvar frequency: 0.** No `?`-prefixed var entities appear in any of the
   153,118 graph entities across the corpus. The mvar semantic bug is
   not consequential for this Mathlib subset — either these declarations don't
   use metavariables, or mvar-containing decls were filtered before corpus
   build. **Reprioritised below the four IR design issues.**

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

Operate directly on the IR graph data in `corpus.jsonl` (ground truth,
parser-independent). Do NOT use the leanExpr string parser for coverage
measurements.

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

### Step 3 — IR Schema Iterator (OpenHands, SSH)
**Script**: `scripts/ir_schema_iterator.py`  
**Output**: `proposals/ir_v2_schema.json`, `findings/schema_comparison.json`

Using the findings from Steps 1–2, OpenHands authors an iterator that:
1. Takes a proposed schema change (new row type, normalisation rule, polarity
   collapse, etc.)
2. Applies it to a sample of corpus entries
3. Measures: token count delta, vocabulary coverage delta, redundancy ratio
4. Writes a comparison entry to `findings/schema_comparison.json`

OpenHands also has Lean access via SSH — it can cross-check proposed AST
mappings against `pp.all` output for representative declarations.

Review gate: Kit reviews `schema_comparison.json` and selects the changes that
improve density without losing coverage before Step 4.

---

### Step 4 — Compression Gate (Kit, local)
**Script**: `scripts/compression_gate.py`  
**Output**: `findings/compression_gate.json`

Run the approved v2 schema across the full corpus and measure:
- Bits-per-token (BPT) vs current IR and flat baseline
- Vocabulary coverage (% of token positions filled by high-frequency tokens)
- Redundancy ratio (entropy of token distribution)

Gate criteria (all must pass before a training run is scheduled):
- BPT(v2) < BPT(A_current)
- Redundancy(v2) < Redundancy(A_current)
- Coverage(v2) >= Coverage(A_current)

If gate passes, produce the OpenHands prompt to update `MetaExtractor.lean`
and regenerate datasets. If gate fails, return to Step 3 with a diagnosis.

---

## Known IR Problems (from current findings)

| Problem | Evidence | Impact |
|---|---|---|
| Polarity always `neut` | 99.8% of rows | 24% of tokens are noise |
| Entity ID sparsity | IDs like `FVAR_0` appear rarely | Model can't build stable reps |
| Normalisation gaps | `casesOn` vs `recOn`, `mk` vs `mk._flat_ctor` | Same structure, different tokens |
| Attribute row density | 85% of entities have zero attribute rows | A rows mostly noise |
| AST nodes dropped | Unknown — Step 1 will quantify | Unknown information loss |

---

## Merge Policy

- Each step's outputs are committed to `kit/ir-design-research` before the
  next step starts.
- OpenHands works on `openhands/ir-schema-v2` branched from
  `kit/ir-design-research`.
- Kit reviews OpenHands PRs before merge into `kit/ir-design-research`.
- `kit/ir-design-research` merges into `kit/dev` only after Step 4 gate passes
  and a training run confirms perplexity improvement.
