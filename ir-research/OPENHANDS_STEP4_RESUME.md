# OpenHands Resume: Step 4 — Compression Gate Fix

## Status

Your previous Step 4 submission (`fe2fcc3`) was reviewed and rejected.
A rework brief was committed at `4588036` (`OPENHANDS_STEP4_REWORK.md`) but
no fix was pushed. This prompt supersedes the rework brief — read it in full
before touching any code.

---

## The problem with the previous submission

`measure_v2_C1C2C4` applies a hard-coded multiplier to the current BPT:

```python
bpt_after = bpt * (1 - c1_reduction * 0.5)  # Conservative estimate
```

This is circular — the formula guarantees a PASS regardless of what the data
says. The gate result is meaningless. The identical numbers (BPT 5.33) in your
follow-up report confirm nothing was changed.

---

## What you must fix

### 1. Build a simulated v2 token stream from real data

The v2 encoder doesn't exist yet, so you can't measure it directly. Instead,
modify the existing `train_A.jsonl` token sequences to reflect what C2 and C4
would produce, then compute BPT/redundancy/coverage from that simulated stream.

**C1 — no effect:**
The current encoder never serializes polarity (Step 2 confirmed 100% `neut`
across all rows — the encoder silently skips it). Removing polarity from the
schema changes nothing about the existing token sequences. Do NOT apply any
token reduction multiplier for C1. Add this note to the JSON output:
```
"c1_note": "No effect on current encoder output — polarity was never serialized"
```

**C2 — simulate +2 tokens per typeclass declaration:**
- Load `datasets/train_A.jsonl`. Each line is `{"input_ids": [...]}`.
- Load `Corpus/corpus.jsonl`. Each line has a `.name` field matching the
  declaration order in `train_A.jsonl` (same index, same 4,029 declarations).
- For each declaration, check its `.graph.entities` for entities where the
  attributes include a typeclass marker. Use the Step 3 count: 3,149
  declarations are affected. If exact identification from the graph is
  complex, mark the first 3,149 declarations (by corpus index) as affected —
  the distribution approximation is acceptable for a gate measurement.
- For each affected declaration's `input_ids`, append 2 synthetic token IDs
  that are not in the existing vocab (e.g. use IDs 9999 and 9998 as
  placeholder `typeclass_name` tokens).
- This produces a modified token list. Compute metrics from it.

**C4 — simulate GEN_UNK redistribution:**
- From `schema_comparison.json`: 1,955 GEN_UNK tokens total, 1,564 resolvable.
- Load `datasets/vocab_A.json` to find the token ID for `GEN_UNK`.
- In the (already C2-modified) token list, find all positions with the GEN_UNK
  token ID. Take the first 1,564 occurrences and replace each with one of 20
  synthetic namespace-bucket token IDs (IDs 9980–9999, cycling through them in
  round-robin order so each bucket gets roughly equal share ~78 tokens each).
- The remaining 391 GEN_UNK tokens stay unchanged.
- Compute final metrics from this fully-modified token list.

### 2. Compute metrics from the simulated token list

Use the existing `compute_bpt`, `compute_redundancy`, and `compute_coverage`
functions — they are correct. Just pass them the simulated token list instead
of the formula-derived values.

For vocab size: current vocab (1,236) + 2 typeclass tokens + 20 bucket tokens
= 1,258.

### 3. Delete the formula-based approach entirely

Remove `bpt_after = bpt * (1 - c1_reduction * 0.5)` and all similar multiplier
lines from `measure_v2_C1C2C4`. The function must load data and simulate, not
multiply.

### 4. Update tests

Add a test (or update an existing one) that:
- Calls `measure_v2_C1C2C4` with a known synthetic `train_A` dataset
- Verifies the returned BPT is computed from the modified token stream
- Confirms v2 BPT is NOT simply `current_BPT * some_constant`

---

## Expected shape of results after fix

After a correct simulation:
- **C1** contributes zero improvement (encoder already omits polarity)
- **C2** adds ~6,298 tokens across 3,149 declarations — slight BPT increase
  from new token types, slight coverage improvement
- **C4** redistributes 1,564 GEN_UNK into 20 buckets — lower redundancy,
  better coverage, small BPT improvement from more uniform distribution

The gate may PASS, PARTIAL, or FAIL. That is fine — an honest result is the
point. A result derived from real simulation is acceptable even if it FAILS;
a formula-guaranteed PASS is not acceptable.

---

## Files to modify

- `ir-research/scripts/compression_gate.py` — fix `measure_v2_C1C2C4`
- `ir-research/scripts/test_compression_gate.py` — add/update simulation test
- `ir-research/findings/compression_gate.json` — regenerate from fixed script

Do NOT modify any other findings files from Steps 2 or 3.

---

## Running the script

```bash
cd /Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/compression_gate.py \
    --train-a datasets/train_A.jsonl \
    --train-flat datasets/train_flat.jsonl \
    --vocab-a datasets/vocab_A.json \
    --vocab-flat datasets/vocab_flat.json \
    --output ir-research/findings/compression_gate.json
```

Run tests before committing:
```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    ir-research/scripts/test_compression_gate.py
```

---

## Commit and push

```bash
git add ir-research/scripts/compression_gate.py \
        ir-research/scripts/test_compression_gate.py \
        ir-research/findings/compression_gate.json
git commit -m "fix: Step 4 — compute v2 BPT from simulated token stream, not formula"
git push origin openhands/ir-compression-gate
```

Do NOT open a PR. Kit reviews the branch directly.
