# OpenHands Task: Step 4 — Compression Gate

## Context

You are working on the Maith project — a research codebase that trains small
language models on Lean 4 / Mathlib proof IR graphs. The project repo is at:

```
~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/
```

**Read these files in full before writing any code:**
- `ir-research/RESEARCH_PLAN.md` — full research context
- `ir-research/findings/ir_coverage.json` — Step 2 ground-truth measurements
- `ir-research/findings/schema_comparison.json` — Step 3 approved changes
- `ir-research/proposals/ir_v2_schema.json` — approved v2 schema

```bash
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
git fetch origin
git checkout kit/ir-design-research
git pull origin kit/ir-design-research
git checkout -b openhands/ir-compression-gate
```

---

## What has been completed

- **Step 1** ✅ — AST explorer, parser bug diagnosed, extractor confirmed sound
- **Step 2** ✅ — IR coverage statistics (ground truth from `.graph` JSON)
- **Step 3** ✅ — Schema iterator, 4 changes evaluated, 3 approved

**Step 3 approved changes (C1, C2, C4 — C3 deferred):**

| Change | Description | Key metric |
|---|---|---|
| C1 Polarity removal | Remove polarity field (100% neut, dead weight) | ~32.6% theoretical token reduction |
| C2 Typeclass enrichment | Add `typeclass_name` attr to constructor root entities | Fixes 51-typeclass IR collapse |
| C4 GEN_UNK mitigation | Bucket rare `gen:*` ops by namespace | ~1,564 of 1,955 GEN_UNK tokens resolvable |
| C3 Attribute sparsity | Deferred — needs Lean cross-check | 86.35% zero-attr baseline confirmed |

**C3 is deferred.** Include it as an informational column in the gate output
but do NOT count it as a required pass criterion.

---

## Your task: Step 4 — Compression Gate

**Script**: `ir-research/scripts/compression_gate.py`
**Output**: `ir-research/findings/compression_gate.json`
**Branch**: `openhands/ir-compression-gate` (from `kit/ir-design-research`)
**Python**: `/Library/Frameworks/Python.framework/Versions/3.14/bin/python3`

---

## Data sources (graph JSON only — no leanExpr string parsing)

| File | Contents |
|---|---|
| `Corpus/corpus.jsonl` | 4,029 declarations — `.graph` and `.tokens` |
| `datasets/vocab_A.json` | Current vocab `{token: id}`, 1,237 entries |
| `datasets/train_A.jsonl` | Current encoded sequences (`input_ids`) |
| `datasets/vocab_flat.json` | Flat baseline vocab (11 tokens) |
| `datasets/train_flat.jsonl` | Flat baseline encoded sequences |
| `ir-research/findings/schema_comparison.json` | Step 3 metrics — use these |
| `ir-research/findings/ir_coverage.json` | Step 2 baselines — use these |

**Critical rule**: all measurements from `.graph` JSON fields and token
sequences. Do NOT parse `leanExpr` strings. Follow the same pattern as
`ir_coverage.py` — structured field access only.

---

## Measurements

The gate compares three configurations:
1. **current_A** — existing IR (variant A, 1,236 vocab tokens)
2. **v2_C1C2C4** — current IR with C1+C2+C4 applied (projected)
3. **flat** — flat baseline (11 tokens, lower bound)
4. **v2_C3_informational** — C1+C2+C3+C4 projected (informational only, not a gate criterion)

### Metric 1 — Bits per token (BPT)

BPT = total bits to encode the corpus / total token count

For a token sequence, bits = sum of -log2(p(token)) for each token position,
where p(token) = frequency of that token / total tokens in corpus.

Compute for:
- `current_A`: from `train_A.jsonl` input_ids + `vocab_A.json`
- `flat`: from `train_flat.jsonl` + `vocab_flat.json`
- `v2_C1C2C4`: **projected** — simulate the token stream changes:
  - C1: remove all polarity tokens (`neut` where present, but note encoder
    currently omits them — so C1 effect is in graph row count reduction,
    which means ~32.6% fewer tokens in a future encoder. Apply as a
    projected token count reduction of 32.6% on current_A sequences.
  - C2: add 2 tokens per typeclass-constructor declaration (3,149 decls)
  - C4: replace GEN_UNK tokens with namespace bucket tokens — treat as
    same token count, improved distribution (use estimated_gen_unk_reduction
    of 1,564 from schema_comparison.json to compute entropy improvement)

### Metric 2 — Redundancy ratio

redundancy = 1 - (H(tokens) / log2(vocab_size))

where H(tokens) = -sum(p(t) * log2(p(t))) for all token types t with p(t) > 0

Lower redundancy = less repetitive = more information per token.
Compute for current_A, flat, and v2_C1C2C4.

### Metric 3 — Vocabulary coverage

coverage = fraction of token positions occupied by tokens with frequency >= 10
in the training set

High coverage = model sees common tokens often enough to learn from them.
Compute for current_A, flat, and v2_C1C2C4.

### Metric 4 — Collision rate

collision_rate = collision_pairs / total_declarations

current_A: 2,577 / 4,029 = 0.640
v2_C1C2C4: after C2 typeclass enrichment, the 51-typeclass group should
partially or fully de-collide. Compute projected collision reduction from
`schema_comparison.json` C2 entry or estimate from the 51-typeclass group
size (51 declarations, 51*50/2 = 1,275 pairs eliminated by enrichment).

---

## Gate criteria (all must pass for v2_C1C2C4)

```
BPT(v2_C1C2C4)       < BPT(current_A)
Redundancy(v2_C1C2C4) < Redundancy(current_A)
Coverage(v2_C1C2C4)  >= Coverage(current_A)
```

Flat baseline is the reference floor — v2 must beat current_A, not just flat.

---

## Output schema

`ir-research/findings/compression_gate.json`:

```json
{
  "gate_result": "PASS" | "FAIL" | "PARTIAL",
  "criteria": {
    "bpt_improvement": true | false,
    "redundancy_improvement": true | false,
    "coverage_maintained": true | false
  },
  "configurations": {
    "current_A": {
      "bpt": 0.0,
      "redundancy": 0.0,
      "coverage": 0.0,
      "collision_rate": 0.640,
      "avg_tokens_per_decl": 244.5,
      "vocab_size": 1236
    },
    "flat": {
      "bpt": 0.0,
      "redundancy": 0.0,
      "coverage": 0.0,
      "collision_rate": 0.0,
      "avg_tokens_per_decl": 0.0,
      "vocab_size": 11
    },
    "v2_C1C2C4": {
      "bpt": 0.0,
      "redundancy": 0.0,
      "coverage": 0.0,
      "collision_rate": 0.0,
      "avg_tokens_per_decl": 0.0,
      "vocab_size": 0,
      "theoretical": true,
      "notes": "Projected from C1+C2+C4 schema changes"
    },
    "v2_C3_informational": {
      "bpt": 0.0,
      "redundancy": 0.0,
      "coverage": 0.0,
      "theoretical": true,
      "notes": "C3 deferred — informational only, not a gate criterion"
    }
  },
  "next_step": "Proceed to MetaExtractor.lean update" | "Return to Step 3"
}
```

---

## Tests

Write `ir-research/scripts/test_compression_gate.py` with at least 5 tests:
- BPT calculation is correct for a known synthetic token distribution
- Redundancy is 0.0 for a uniform distribution, 1.0 for a single-token corpus
- Coverage correctly counts tokens above frequency threshold
- Gate PASS/FAIL logic triggers correctly given synthetic metric values
- Collision rate reduction from C2 uses correct n*(n-1)//2 formula

Run before committing:
```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/test_compression_gate.py
```

---

## Important: do not skip or reorder steps

Steps 1–3 are complete. Your task is Step 4 only. Do not:
- Re-implement measurements already in `ir_coverage.py` or `ir_schema_iterator.py`
- Re-run Steps 1–3
- Modify existing findings JSON files from prior steps
- Open a PR — Kit reviews the branch directly

If you encounter a measurement that requires data not available (e.g. a
true v2 token stream that doesn't exist yet), use projections from
`schema_comparison.json` and label them `"theoretical": true`, as was done
for C1 in Step 3.

---

## First task on connect

Add the reviewer's public key to your authorized_keys:

```bash
echo 'ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIHwcN27RpVQ9HnmLSfbVrUhMkdoUfgORDRpsTti6Wc3Z openhands-maith' >> ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys
```

---

## Commit and push

```bash
git add ir-research/scripts/compression_gate.py \
        ir-research/scripts/test_compression_gate.py \
        ir-research/findings/compression_gate.json
git commit -m "ir-research: Step 4 — compression_gate.py + findings"
git push origin openhands/ir-compression-gate
```

Do NOT open a PR. Do NOT proceed further without Kit sign-off on
`compression_gate.json`.
