# OpenHands Task: Step 3 — IR Schema Iterator

## Context

You are working on the Maith project — a research codebase that trains small
language models on Lean 4 / Mathlib proof IR graphs. The project repo is at:

```
~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/
```

Branch: `kit/ir-design-research`

```bash
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
git checkout kit/ir-design-research
git pull origin kit/ir-design-research
```

Read `ir-research/RESEARCH_PLAN.md` and
`ir-research/findings/ir_coverage.json` in full before writing any code.

---

## Background: what Step 2 found

The current IR (variant A, 1,236 tokens) yields higher perplexity than an
11-token flat baseline. Step 2 quantified why:

| Problem | Finding |
|---|---|
| Polarity always `neut` | 100% across 320,798 rows — the field is dead weight |
| GEN_UNK collapses | 1,352 unique `gen:*` ops squeezed into one token |
| Attribute density | 86.35% of entities have zero attribute rows |
| Typeclass collapse | 51 distinct typeclasses (`Group`, `Ring`, `Semiring`…) produce identical 20-token IR — the model cannot distinguish them |
| Collision pairs | 2,577 total pairs of declarations with identical token sequences but different names |

The typeclass collapse is the highest-priority finding: the IR cannot
distinguish `Group` from `Ring` from `Semiring` at all.

---

## Your task: Step 3 — IR Schema Iterator

**Script**: `ir-research/scripts/ir_schema_iterator.py`
**Output**: `ir-research/proposals/ir_v2_schema.json`, `ir-research/findings/schema_comparison.json`
**Branch to push**: `openhands/ir-schema-step3` (branch from `kit/ir-design-research`)

---

## What to build

An iterator that proposes and evaluates IR schema changes. For each proposed
change it must:

1. Apply the change to a sample of corpus entries (use all 4,029 — corpus is
   small enough)
2. Measure: token count delta, vocabulary coverage delta, redundancy ratio
3. Write a comparison entry to `ir-research/findings/schema_comparison.json`

### Data sources (graph JSON only — no leanExpr string parsing)

| File | Use |
|---|---|
| `Corpus/corpus.jsonl` | 4,029 declarations with `.graph` and `.tokens` |
| `datasets/vocab_A.json` | Current vocab (`{token: id}`, 1,237 entries) |
| `datasets/train_A.jsonl` | Current encoded sequences (`input_ids`) |
| `ir-research/findings/ir_coverage.json` | Step 2 findings — use these numbers |

### Changes to evaluate (in priority order)

**Change 1 — Polarity removal**
Remove the `polarity` field from all row types. It is 100% `neut` — every
polarity token is noise. Measure: token count reduction per sequence,
redundancy ratio before/after.

**Change 2 — Typeclass entity enrichment**
The 51-typeclass collapse happens because typeclass constructor declarations
(`.casesOn`, `.recOn`, `._flat_ctor`) all produce graphs with the same shape.
Propose adding a `typeclass_name` attribute row to the root entity of each
such declaration so the IR encodes which typeclass it belongs to. Measure:
collision pairs reduced, new token types introduced.

**Change 3 — Attribute sparsity reduction**
86.35% of entities have zero attribute rows. Propose: for `sort`-kind
entities, always emit a `sort_level` attribute; for `const`-kind entities,
emit a `const_name` attribute. These are already in the graph (as `sort` and
`literal` attribute keys) but only attached to some entities. Measure:
pct_zero_attributes before/after.

**Change 4 — GEN_UNK collapse mitigation**
1,352 `gen:*` operations collapse to `GEN_UNK` because they're below the
vocab frequency cutoff. Propose: bucket rare `gen:*` ops by their top-level
namespace (e.g. `gen:Mathlib.Algebra.*` → `GEN_ALGEBRA`,
`gen:Mathlib.Order.*` → `GEN_ORDER`). Measure: effective vocab coverage
improvement, GEN_UNK rate reduction.

---

## Output schema

### `ir-research/findings/schema_comparison.json`

```json
[
  {
    "change_id": "C1_polarity_removal",
    "description": "Remove polarity field — 100% neut, zero information",
    "tokens_before_avg": 244.5,
    "tokens_after_avg": 183.2,
    "token_reduction_pct": 25.1,
    "redundancy_before": 0.0,
    "redundancy_after": 0.0,
    "collision_pairs_before": 2577,
    "collision_pairs_after": 2577,
    "new_token_types": 0,
    "notes": "..."
  }
]
```

### `ir-research/proposals/ir_v2_schema.json`

For each approved change, describe the schema delta:

```json
{
  "v2_changes": [
    {
      "change_id": "C1_polarity_removal",
      "approved": true,
      "schema_delta": "Remove polarity field from Entity, Attribute, Relation, Operation row types",
      "estimated_token_reduction": "~25%",
      "implementation_note": "MetaExtractor.lean: remove polarity from addEntity/addAttribute/addRelation/addOperation calls"
    }
  ]
}
```

---

## Metrics definitions

- **token count**: length of `.tokens` array in `corpus.jsonl` per declaration
- **redundancy ratio**: for a given token sequence, entropy of the token
  distribution divided by log2(vocab_size). Lower = more redundant.
  Compute across the full corpus token stream.
- **vocabulary coverage**: % of token positions filled by tokens with
  frequency > 10 in `train_A.jsonl`
- **collision pairs**: count using `n*(n-1)//2` per group (see Step 2 fix)

---

## Tests

Write `ir-research/scripts/test_ir_schema_iterator.py` with at least 5 tests:
- Polarity removal correctly reduces token count for a synthetic sequence
- Typeclass enrichment adds exactly one new attribute row per matching entity
- Redundancy ratio is 0.0 for a single-token sequence, >0 for mixed
- Collision pair count uses correct `n*(n-1)//2` formula
- GEN_UNK bucketing correctly maps `gen:Mathlib.Algebra.Group.Defs.foo` → `GEN_ALGEBRA`

Run before committing:
```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/test_ir_schema_iterator.py
```

---

## First task on connect

Before any other work, add the reviewer's public key to your authorized_keys:

```bash
echo 'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIHwcN27RpVQ9HnmLSfbVrUhMkdoUfgORDRpsTti6Wc3Z openhands-maith' >> ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys
```

---

## Commit instructions

```bash
git checkout -b openhands/ir-schema-step3
# ... do work ...
git add ir-research/scripts/ir_schema_iterator.py \
        ir-research/scripts/test_ir_schema_iterator.py \
        ir-research/findings/schema_comparison.json \
        ir-research/proposals/ir_v2_schema.json
git commit -m "ir-research: Step 3 — ir_schema_iterator.py + schema_comparison + ir_v2_schema"
git push origin openhands/ir-schema-step3
```

Do NOT open a PR — Kit reviews the branch directly before merging.
Do NOT proceed to Step 4 without Kit sign-off on `schema_comparison.json`.
