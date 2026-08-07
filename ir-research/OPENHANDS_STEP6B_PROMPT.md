# OpenHands Task: Step 6b — Update build_dataset.py for v2 IR tokens

## Context

You are working on the Maith project. Repo path on sandbox:

```
~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo/
```

**Read these files before writing any code:**
- `ir-research/RESEARCH_PLAN.md` — research context
- `ir-research/findings/schema_comparison.json` — approved schema changes C1/C2/C4
- `python/build_dataset.py` — the file you are modifying
- `datasets/vocab_A_v1.json` — current (v1) vocab for reference

```bash
cd ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
git fetch origin
git checkout kit/dev
git pull origin kit/dev
git checkout -b openhands/build-dataset-v2
```

---

## Background

`MetaExtractor.lean` has been updated (Steps 1–5, now on `kit/dev`) with three
schema changes:

**C1 — polarity removal**: no effect on tokens (encoder already omitted polarity).

**C2 — typeclass_name attribute**: instImplicit binders now emit a second
attribute with key `typeclass_name` and value = the short unqualified typeclass
name (e.g. `"Group"`, `"Ring"`, `"Semiring"`). The encoder serializes this as a
token like `typeclass_name:Group`. These are brand new token types not in the
current vocab.

**C4 — gen:* namespace bucketing**: all generic operation fallbacks now emit a
stable bucket name instead of the full qualified name:
- `GEN_MATHLIB`, `GEN_ALGEBRA`, `GEN_ORDER`, `GEN_TOPOLOGY`, `GEN_ANALYSIS`
- `GEN_LOGIC`, `GEN_DATA`, `GEN_LEAN`, `GEN_INIT`, `GEN_STD`, `GEN_UNKNOWN`

Projection tokens are now `proj:GEN_MATHLIB/0` style instead of
`proj:Semigroup.toMul/0`.

The v2 corpus (`corpus.jsonl`) is being rebuilt locally. Your job is to update
`python/build_dataset.py` so it handles these new tokens correctly when it runs
against the new corpus.

---

## Your task: update `python/build_dataset.py`

**File**: `python/build_dataset.py`
**Branch**: `openhands/build-dataset-v2` (from `kit/dev`)

### Change 1 — Add v2 structural tokens to `build_ir_vocab`

In `build_ir_vocab`, the `structural` list defines tokens that always get stable
low integer IDs regardless of frequency. Add the new v2 token types here so they
are never treated as rare and never fall through to GEN_UNK.

Current structural list (lines ~56–68):
```python
structural = [
    "<PAD>", "<BOS>", "<EOS>", "<UNK>", "GEN_UNK",
    "GRAPH_BEGIN", "GRAPH_END",
    "E", "A", "R", "O",
    "pos", "neg", "neut",
    "eq", "add", "sub", "mul", "div", "le", "ge", "lt", "gt", "pow",
    "typeclass", "sort", "literal",
] + [f"FVAR_{i}" for i in range(64)] + ["FVAR_MANY"] \
  + [f"BVAR_{i}" for i in range(64)] + ["BVAR_MANY"] \
  + [f"TERM_{i}" for i in range(64)] + ["TERM_MANY"] \
  + [f"IN_{i}" for i in range(10)] + ["IN_MANY"] \
  + [f"OUT_{i}" for i in range(64)] + ["OUT_MANY", "OUT_VAR"]
```

Add the following **after** `"typeclass", "sort", "literal"`:

```python
# v2 IR tokens (C2: typeclass_name, C4: gen buckets)
"typeclass_name",
"GEN_MATHLIB", "GEN_ALGEBRA", "GEN_ORDER", "GEN_TOPOLOGY", "GEN_ANALYSIS",
"GEN_LOGIC", "GEN_DATA", "GEN_LEAN", "GEN_INIT", "GEN_STD", "GEN_UNKNOWN",
```

These ensure the 11 bucket tokens and the `typeclass_name` key token always have
stable IDs in every vocab built from a v2 corpus.

### Change 2 — Handle `typeclass_name:*` tokens in `encode_ir`

The encoder serializes `typeclass_name` attributes as `typeclass_name:<value>`
tokens (e.g. `typeclass_name:Group`, `typeclass_name:Ring`). These are
frequency-driven and will appear in the freq-sorted portion of the vocab. No
special handling needed in the vocab builder — they'll be added by the existing
frequency loop.

However, `encode_ir` currently has this fallback:
```python
elif tok.startswith("gen:"):
    result.append(gen_unk_id)
```

Add a parallel guard so that `typeclass_name:*` tokens that don't appear in the
vocab (e.g. very rare typeclass names in the eval set but not the train set) fall
back to `<UNK>` rather than silently disappearing:

```python
# No special fallback needed — typeclass_name:* tokens use the standard <UNK> path
# (this comment replaces nothing; just confirm the existing else clause handles it)
```

Actually no change is needed here — the existing `else: result.append(unk_id)`
already handles unknown `typeclass_name:*` tokens correctly. Just verify this is
the case and add a comment confirming it.

### Change 3 — Update `GEN_UNK` collapse logic for gen:* tokens

Currently:
```python
# Rare gen:* tokens collapse to GEN_UNK (already in vocab)
if tok.startswith("gen:") and count < freq_threshold:
    continue
```

With v2, the MetaExtractor no longer emits `gen:FullyQualifiedName` tokens —
it emits bucket names like `GEN_ALGEBRA` directly (without the `gen:` prefix).
The `gen:*` prefix tokens from the v1 corpus won't appear in the v2 corpus.

Update this block to also skip `gen:*` tokens entirely (they are v1 artifacts),
and add a comment explaining why:

```python
# v1 gen:* tokens (gen:FullyQualifiedName) are replaced by stable bucket tokens
# in v2 (GEN_ALGEBRA, GEN_ORDER, etc.). Skip any residual gen:* entries.
if tok.startswith("gen:"):
    continue
```

This replaces the frequency threshold check for `gen:*` — all `gen:*` tokens
are dropped regardless of frequency, since the v2 encoder never emits them.

### Change 4 — Update representation_id default

In the `run()` function and the argparse default, update:
```python
# OLD:
representation_id: str = "semantic_graph_ir_v1_4_0"
# NEW:
representation_id: str = "semantic_graph_ir_v2_0_0"
```

And in argparse:
```python
# OLD:
parser.add_argument("--representation-id", default="semantic_graph_ir_v1_4_0")
# NEW:
parser.add_argument("--representation-id", default="semantic_graph_ir_v2_0_0")
```

---

## Tests

Write `python/test_build_dataset_v2.py` with at least 5 tests:

1. `typeclass_name` is in the structural vocab with a stable low ID
2. All 11 GEN_* bucket tokens are in the structural vocab
3. `gen:*` tokens are dropped from vocab regardless of frequency
4. `typeclass_name:Group` token (freq-driven) is added to vocab if it appears
   in training data
5. `encode_ir` correctly encodes `GEN_ALGEBRA` to its vocab ID
6. `encode_ir` falls back to `<UNK>` for unknown `typeclass_name:RareClass`

Run before committing:
```bash
/Library/Frameworks/Python.framework/Versions/3.14/bin/python3 \
    python/test_build_dataset_v2.py
```

All tests must pass.

---

## Important: do NOT run build_dataset.py against the corpus

The v2 corpus (`corpus.jsonl`) is being rebuilt locally and is not ready yet.
Your task is to update the script and tests only. Do not run `build_dataset.py`
against the real corpus — it would produce a v2 vocab from a v1 corpus and that
would be wrong.

You can verify correctness using the tests alone.

---

## Commit and push

```bash
git add python/build_dataset.py python/test_build_dataset_v2.py
git commit -m "feat: build_dataset.py v2 — typeclass_name + GEN_* bucket tokens, drop gen:* v1 artifacts"
git push origin openhands/build-dataset-v2
```

Do NOT open a PR. Kit reviews the branch directly.

Output your final summary as:

## Task Completed Successfully
[summary]
