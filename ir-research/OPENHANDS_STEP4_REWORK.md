# Step 4 Rework: Compression Gate — Fix Required

## What was submitted

Commit `fe2fcc3` on `openhands/ir-compression-gate` is not acceptable. The gate
reported PASS but the result is not meaningful. See below.

---

## The core problem

`measure_v2_C1C2C4` does not simulate a token stream. It applies a hard-coded
multiplier to the current BPT:

```python
bpt_after = bpt * (1 - c1_reduction * 0.5)  # Conservative estimate
```

This is circular: the formula is constructed to guarantee improvement. The PASS
result is predetermined, not derived from data. A gate that always passes is not
a gate.

---

## What the script must do instead

The v2 token stream does not exist yet (the encoder hasn't been updated), so you
cannot measure it directly. That is expected and was acknowledged in the prompt
(`theoretical: true`). The correct approach is to **simulate the v2 token stream
from the existing corpus data** and compute BPT/redundancy/coverage from that
simulation. Here is how:

### How to simulate v2_C1C2C4

Work directly from `Corpus/corpus.jsonl` `.graph` fields and
`datasets/train_A.jsonl` + `datasets/vocab_A.json`. Do NOT parse `leanExpr`.

**C1 — polarity removal:**
- The current encoder silently skips `neut` polarity (Step 2 confirmed 100% neut
  across all rows). So C1 has no effect on the existing token stream.
- Do NOT apply a 32.6% BPT reduction multiplier. That number was a Step 3
  projection for a *future* encoder that would have emitted polarity tokens.
  The current encoder never emitted them, so removing polarity from the schema
  changes nothing about the existing `train_A.jsonl` sequences.
- C1's correct contribution to the gate: **token count unchanged, zero BPT
  impact on existing data**. Note this clearly in the output with
  `"c1_note": "No effect on current encoder output — polarity was never serialized"`.

**C2 — typeclass enrichment:**
- From `schema_comparison.json`: 3,149 declarations will gain 2 extra tokens each.
- Simulate by taking the current `train_A.jsonl` token sequences and inserting
  2 synthetic `typeclass_name` tokens for each of those 3,149 declarations.
- To identify which declarations are affected: load `Corpus/corpus.jsonl`,
  check each declaration's `.graph` for constructor root entities that have a
  typeclass attribute (use the `typeclass_count` from Step 3 findings, or
  approximate by checking `.graph.entities` for entities with `type == "const"`
  and attr keys that include typeclass markers). If exact identification is
  complex, use the Step 3 count (3,149) as the affected set and distribute the
  2 extra tokens across those declarations proportionally.
- Recompute BPT, redundancy, and coverage on the augmented token stream.

**C4 — GEN_UNK mitigation:**
- From `schema_comparison.json`: 1,564 of 1,955 GEN_UNK tokens are resolvable
  into ~20 namespace bucket tokens.
- Simulate by replacing 1,564 `GEN_UNK` tokens in the current token stream with
  20 new synthetic namespace-bucket token types distributed proportionally
  (e.g. ~78 tokens per bucket on average).
- The remaining 391 GEN_UNK tokens stay as GEN_UNK.
- Recompute BPT, redundancy, and coverage on the modified token stream.

### Expected result after correct simulation

Once you build the actual simulated stream:
- **C1**: no change (encoder already omits polarity — confirmed in Step 2)
- **C2**: slight token count increase (+2 per typeclass decl), slightly higher
  entropy from new token types → BPT likely increases marginally
- **C4**: 1,564 GEN_UNK tokens split into 20 buckets → improved distribution,
  lower redundancy

The gate may still PASS, PARTIAL, or FAIL — that is the point. The script must
derive the result from simulation, not from a formula that guarantees a
predetermined outcome.

---

## Summary of changes required

1. **Delete the `bpt_after = bpt * (1 - c1_reduction * 0.5)` line** and all
   similar formula-based BPT/redundancy multipliers in `measure_v2_C1C2C4`.

2. **Build a simulated v2 token list** by modifying the current `train_A.jsonl`
   sequences per C2 and C4 rules above. C1 contributes nothing to the current
   token stream.

3. **Compute BPT, redundancy, and coverage from the simulated token list**
   using the same functions already in the script (`compute_bpt`,
   `compute_redundancy`, `compute_coverage`). These functions are correct —
   only the input data is wrong.

4. **Update `compression_gate.json`** with the re-run results and regenerate
   the gate decision from the actual numbers.

5. **Tests**: add a test that verifies `measure_v2_C1C2C4` produces a result
   different from `measure_current_A` for reasons traceable to C2/C4 token
   stream changes — not just a multiplier applied to the same base values.

---

## What does NOT need to change

- The BPT, redundancy, coverage, and collision-rate functions are correct.
- The output schema is correct.
- The gate criteria are correct.
- `measure_current_A` and `measure_flat` are correct — they measure real data.
- The C3 informational column approach is fine.

---

## Branch

Continue on `openhands/ir-compression-gate`. Amend or add a new commit on top
of `fe2fcc3`. Push to origin when done.

## Commit message

```
fix: Step 4 — compute v2 BPT from simulated token stream, not formula
```

Do NOT open a PR. Ping Kit for review when pushed.
