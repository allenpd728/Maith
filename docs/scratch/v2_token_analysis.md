# v2 IR Token Analysis — C2 present, C4 NOT implemented in data

**Analyzed:** 2026-08-08 (during the `variant_A_v2_full` training run)
**Question:** Does the rebuilt v2 dataset actually contain the C1/C2/C4 IR changes
that DEC-025 triggered and DEC-026 approved?

## Verdict

| Change | In token stream? | Evidence |
|---|---|---|
| **C1** polarity removal | ✅ Yes | no `neut`/`pos`/`neg` tokens in `train_A.jsonl` |
| **C2** typeclass enrichment | ✅ Yes | `typeclass` attribute key + value present (e.g. `A FVAR_1 typeclass Ring`) |
| **C4** GEN_* module bucketing | ❌ **No** | all generic operations collapse to a single `GEN_UNK`; **zero** `GEN_ALGEBRA`/`GEN_ORDER`/etc. |

So the v2 dataset is effectively **C1 + C2 only** — the C4 module-based GEN
bucketing (the change that was supposed to redistribute ~1,564 tokens out of the
single `GEN_UNK` pool into ~20 module buckets) is **not present**.

## C4 root cause

- The corpus (`Corpus/corpus.jsonl`, rebuilt Aug 7) still emits v1-style
  `gen:<FullName>` tokens for generic operations (e.g. `gen:AddMonoid`,
  `gen:HSMul.hSMul`). The Lean `MetaExtractor`/`Encoder` did **not** emit
  `GEN_<bucket>` tokens, despite `bucketFromModule` existing in
  `MetaExtractor.lean`.
- `python/build_dataset.py`:
  - `build_ir_vocab` **skips** any `tok.startswith("gen:")` from the vocab
    (`if tok.startswith("gen:"): continue`).
  - `encode_ir` maps **every** `gen:*` token to `GEN_UNK`:
    ```python
    elif tok.startswith("gen:"):
        result.append(gen_unk_id)   # <- no module-bucket mapping
    ```
- The `GEN_ALGEBRA`/`GEN_ORDER`/… tokens are defined in the `structural` vocab
  list but **never used** — nothing translates `gen:AddMonoid` → `GEN_ALGEBRA`.

## Magnitude

- `GEN_UNK` occupies **49,893 positions = 8.27%** of all training tokens.
- These 49,893 positions are a single undifferentiated token — exactly the
  vocabulary-dilution problem C4 was designed to fix (DEC-026: "1,564 tokens
  that were pooled into one bucket now spread across 20").
- v2 vocab is 601; unique token types in train_A: 537; top-100 tokens cover
  92.3% of positions.

## Impact on the running v2 A training (`variant_A_v2_full`)

The run currently in progress is a **partial v2 (C1+C2, missing C4)**, not the
full C1+C2+C4 v2 that DEC-026 specifies. Therefore:

- Its final perplexity is informative (it tests polarity removal + typeclass
  enrichment) but does **not** fully answer DEC-026's gate for the complete v2 IR.
- A follow-up is required: implement the `gen:* → GEN_<bucket>` mapping (either
  in `build_dataset.py`'s `encode_ir` using the declaration's `module` field, or
  by fixing the Lean encoder to emit `GEN_<bucket>`), rebuild the dataset, and
  re-run A. Only then is the DEC-026 "full v2" comparison valid.

## Recommended fix (Python-side, fastest)

`encode_ir` needs the module → bucket mapping. Since `encode_ir` only receives
the token list (not the declaration's module), the cleanest spot is to bucket
the `gen:<FullName>` token by its namespace prefix at encode time. Sketch:

```python
def gen_bucket(full_name: str) -> str:
    # full_name like "Mathlib.Algebra.Group.Basic.add" -> "GEN_ALGEBRA"
    top = full_name.split(".")
    # map top-level namespace to a bucket; default GEN_UNKNOWN
    ...
    return bucket  # one of GEN_ALGEBRA, GEN_ORDER, GEN_TOPOLOGY, ...

# in encode_ir:
elif tok.startswith("gen:"):
    bucket = gen_bucket(tok[4:])
    result.append(vocab.get(bucket, gen_unk_id))
```

This must be kept consistent with `bucketFromModule` in `MetaExtractor.lean`
(ideally share the mapping logic / a single source of truth) to avoid skew
between the Lean-side and Python-side bucketing.

## Note on an earlier misreading

An initial pass searched for the literal token `"typeclass_name"` and found 0 —
that was a wrong token name. The actual attribute key token is `"typeclass"`
(followed by its value, e.g. `"Ring"`). C2 is present; only C4 is missing.
