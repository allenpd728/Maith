# OpenHands Task: Step 2 — IR Coverage Statistics

## Context

You are working on the Maith project, a research codebase that trains small
language models on Lean 4 / Mathlib proof IR graphs. The project lives in the
sandbox repo at:

```
~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/
```

The active branch is `kit/ir-design-research`. Check it out before starting:

```
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
git checkout kit/ir-design-research
```

The research goal is to diagnose why the semantic IR (variant A, 1,236 tokens)
yields *higher* perplexity than an 11-token flat baseline — meaning the
semantic layer is hurting rather than helping. Step 1 (AST exploration) is
complete. **Your task is Step 2: coverage statistics.**

Read `ir-research/RESEARCH_PLAN.md` in full before writing any code.
Read `ir-research/findings/ast_explorer_output.json` for Step 1 findings.

---

## Key constraint — data source

**All measurements must read from `.graph` JSON fields in `Corpus/corpus.jsonl`
and from `datasets/vocab_A.json` and `datasets/train_A.jsonl`.**

Do NOT parse the `leanExpr` string field in `corpus.jsonl`. That field is a
pretty-printed display string — Step 1 proved its string parser has multiple
inconsistent failure modes (subscript splitting, binder swallowing, punctuation
orphaning). Any measurement derived from parsing `leanExpr` is untrustworthy.

The correct template is `collect_graph_entity_stats()` in
`ir-research/scripts/ast_explorer.py` — it reads `entry["graph"]["entities"]`
directly from the parsed JSON. Follow that pattern throughout.

---

## What to build

**Script**: `ir-research/scripts/ir_coverage.py`  
**Output**: `ir-research/findings/ir_coverage.json`  
**Python**: `/Library/Frameworks/Python.framework/Versions/3.14/bin/python3`

The script must be runnable as:
```
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/ir_coverage.py \
    --corpus Corpus/corpus.jsonl \
    --vocab datasets/vocab_A.json \
    --train datasets/train_A.jsonl \
    --output ir-research/findings/ir_coverage.json
```

---

## Measurements (all from `.graph` JSON, no leanExpr parsing)

### 1. Polarity distribution
For each of the four graph arrays (`entities`, `attributes`, `relations`,
`operations`), count how often `polarity` is `"neut"`, `"pos"`, or `"neg"`.
Report per-array and overall. This quantifies the noise budget — if `neut`
dominates everywhere, the polarity field carries no information.

### 2. Entity ID frequency distribution
From `datasets/vocab_A.json` (a `{token: id}` dict, 1,237 lines):
- Compute token frequency by counting how often each token ID appears across
  all `input_ids` sequences in `datasets/train_A.jsonl`
- Produce a frequency histogram: tokens appearing 1×, 2–10×, 11–100×, 100+×
- Count how many tokens map to `GEN_UNK` (token ID 4) — these are gen:*
  operation names that didn't make the vocabulary cutoff
- Report the top 20 most frequent tokens and bottom 20 (rarest non-zero)

### 3. Attribute row density
From `.graph.attributes` in `corpus.jsonl`:
- For each declaration, count how many distinct entities have at least one
  attribute row
- Compute: % of entities with zero attribute rows (per declaration, then
  aggregated across corpus)
- Distribution: histogram of per-entity attribute row counts (0, 1, 2, 3+)
- Flag the most common attribute keys and their value distributions

### 4. Normalisation collision rate
From `.graph` and `tokens` fields in `corpus.jsonl`:
- A normalisation collision = two declarations whose `tokens` sequence is
  identical but whose `name` differs. These are structurally equivalent
  declarations that the canonicaliser (casesOn→recOn, mk._flat_ctor→mk) maps
  to the same IR.
- Count: how many such collision pairs exist?
- Also count declarations whose `name` ends in `.casesOn`, `.recOn`,
  `._flat_ctor`, or `._flat` — these are the normalisation targets.

### 5. Hard-fail path confirmation
From `.graph` in `corpus.jsonl`:
- **bvar scope**: for each declaration, find the maximum De Bruijn index that
  would be referenced. A rough proxy: count `bound`-kind entities (these are
  the binder context). If any `var`-kind entity name looks like a bvar
  reference that exceeded context depth, flag it. Also simply confirm
  `stats.json` `irConstructionFailed == 0`.
- **operation arity**: for each operation in `.graph.operations`, check ops
  whose `op` field is `"add"`, `"sub"`, `"mul"`, `"div"`, `"pow"` (expect 2
  inputs) or `"neg"` (expect 1 input). Count any with fewer inputs than
  expected.
- **mvar count**: count `var`-kind entities whose `name` starts with `"?"`.
  (Already confirmed 0 in Step 1 — record it here for completeness.)

---

## Output schema

`ir-research/findings/ir_coverage.json` must contain:

```json
{
  "corpus_size": 4029,
  "polarity": {
    "entities":   {"neut": N, "pos": N, "neg": N, "pct_neut": 0.0},
    "attributes": {"neut": N, "pos": N, "neg": N, "pct_neut": 0.0},
    "relations":  {"neut": N, "pos": N, "neg": N, "pct_neut": 0.0},
    "operations": {"neut": N, "pos": N, "neg": N, "pct_neut": 0.0},
    "overall":    {"neut": N, "pos": N, "neg": N, "pct_neut": 0.0}
  },
  "token_frequency": {
    "vocab_size": 1237,
    "gen_unk_collapses": N,
    "frequency_buckets": {"1x": N, "2_10x": N, "11_100x": N, "100plus_x": N},
    "top_20": [["token", count], ...],
    "bottom_20_nonzero": [["token", count], ...]
  },
  "attribute_density": {
    "total_entities": 153118,
    "entities_with_zero_attributes": N,
    "pct_zero_attributes": 0.0,
    "per_entity_count_distribution": {"0": N, "1": N, "2": N, "3plus": N},
    "top_attribute_keys": [["key", count], ...]
  },
  "normalisation": {
    "collision_pairs": N,
    "casesOn_count": N,
    "recOn_count": N,
    "flat_ctor_count": N,
    "examples": [{"name_a": "...", "name_b": "...", "token_len": N}, ...]
  },
  "hard_fail_paths": {
    "ir_construction_failed": 0,
    "operation_arity_violations": N,
    "mvar_entities": 0,
    "notes": "..."
  }
}
```

---

## Tests

Write `ir-research/scripts/test_ir_coverage.py` with at least 5 unit tests:
- Polarity counter handles missing `polarity` field gracefully (defaults to neut)
- Token frequency bucket logic is correct for boundary values (1, 2, 10, 11, 100, 101)
- Attribute density calculation is correct for a synthetic 3-entity graph
- Normalisation collision detection works on a synthetic collision pair
- Operation arity check correctly flags a neg op with 2 inputs and passes one with 1

Run tests before committing:
```
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/test_ir_coverage.py
```

---

## Useful file locations (all in the sandbox repo)

| File | Contents |
|---|---|
| `Corpus/corpus.jsonl` | 4,029 declarations, each with `.graph` and `.leanExpr` |
| `Corpus/stats.json` | Extraction summary — use to confirm irConstructionFailed=0 |
| `datasets/vocab_A.json` | `{token: id}` dict, 1,237 entries |
| `datasets/train_A.jsonl` | Training examples with `input_ids` sequences |
| `ir-research/RESEARCH_PLAN.md` | Full research plan and step definitions |
| `ir-research/findings/ast_explorer_output.json` | Step 1 findings |
| `ir-research/scripts/ast_explorer.py` | Step 1 script — see `collect_graph_entity_stats()` for the correct data access pattern |
| `python/corpus_loader.py` | Existing corpus loader — check before reimplementing |

---

## Commit instructions

1. Run the test suite — all tests must pass before committing.
2. Run the script against the full corpus — output must be valid JSON.
3. Commit both files to `kit/ir-design-research`:
   ```
   git add ir-research/scripts/ir_coverage.py \
           ir-research/scripts/test_ir_coverage.py \
           ir-research/findings/ir_coverage.json
   git commit -m "ir-research: Step 2 — ir_coverage.py + findings"
   git push origin kit/ir-design-research
   ```

Do not open a PR — Kit will review the commit directly on the branch.

---

## Review gate

Kit will review `ir_coverage.json` before Step 3 starts. The review checks:
- Polarity pct_neut values are consistent with the known ~99.8% figure
- Token frequency buckets sum to vocab_size
- Attribute density pct_zero is in the expected ~85% range
- Collision pairs reflect actual normalisation (casesOn/recOn, mk._flat_ctor)
- All hard-fail counts are 0 or justified

Do not proceed to Step 3 without explicit sign-off.
