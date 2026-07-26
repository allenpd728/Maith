# Python Pipeline

The `python/` directory provides tooling to load, validate, and transform the JSONL corpus
produced by the Lean pipeline into training-ready datasets for the A/B/C experiment.

## Files

| File | Purpose |
|------|---------|
| `corpus_loader.py` | Schema validation, loading, vocab building, splitting, dataset class |
| `build_dataset.py` | A/B/C dataset builder — IR vocab + BPE variants, train/eval splits |
| `validate_roundtrip.py` | Decoder round-trip validator — confirms BVAR/TERM token stability |
| `tokenizer_study.py` | BPE fragmentation study — compares IR tokens vs Qwen2.5-Coder BPE |
| `spot_check.py` | Manual corpus spot-checking helper |

## Requirements

- Python 3.8+
- `transformers` library (for `build_dataset.py` variants B/C and `tokenizer_study.py`):
  `pip install transformers`

`corpus_loader.py` and `validate_roundtrip.py` use only the standard library.

## build_dataset.py

Builds three dataset variants for the controlled A/B/C experiment:

| Variant | Representation | Tokenizer |
|---------|---------------|-----------|
| A | Maith IR tokens (v1.0.0) | Custom vocab (`vocab_A.json`) |
| B | Raw `leanExpr` string | Qwen2.5-Coder BPE |
| C | AST-style split `leanExpr` | Qwen2.5-Coder BPE |

```bash
cd ~/Projects/Maith
python3 python/build_dataset.py [--corpus Corpus/corpus.jsonl] [--out datasets/] [--seed 42]
```

Outputs under `datasets/`:
- `vocab_A.json` — IR token → integer ID mapping (built from training split only)
- `train_A.jsonl`, `eval_A.jsonl` — IR token variant
- `train_B.jsonl`, `eval_B.jsonl` — Lean source BPE variant
- `train_C.jsonl`, `eval_C.jsonl` — AST-style BPE variant

Each row: `{ "source", "name", "module", "input_ids", "labels", "seq_len" }`.
`labels` = `input_ids` shifted left by 1 (standard causal LM format, last position = −100).

Split: 90% train / 10% eval, fixed seed for reproducibility across all three variants.

### IR vocab design

- Structural tokens (`GRAPH_BEGIN`, `E`, `A`, `R`, `O`, polarities, ops) get fixed low IDs.
- `BVAR_0`–`BVAR_31` + `BVAR_MANY` and `TERM_0`–`TERM_31` + `TERM_MANY` are always in vocab.
- `gen:*` tokens below frequency threshold (default 5) map to `GEN_UNK`.
- Vocab is built from the training split only — no test-set leakage.

## validate_roundtrip.py

Confirms that the Python-side decoder correctly round-trips every corpus token sequence.

```bash
python3 python/validate_roundtrip.py                   # sample 200
python3 python/validate_roundtrip.py --all             # full 2554
python3 python/validate_roundtrip.py --sample 500
```

Expected output on v1.0.0 corpus:
```
Results: 2554/2554 passed
  All examples round-trip cleanly ✓
  BVAR_* tokens: 33   TERM_* tokens: 33   Legacy b(): 0   Legacy t<n>: 0
```

## tokenizer_study.py

Measures BPE fragmentation of IR-specific tokens and sequence length inflation.

```bash
python3 python/tokenizer_study.py [--corpus Corpus/corpus.jsonl] [--sample 500]
```

Results on 2,554-example corpus (500-example sample):
- Lean source → BPE: **1.69x** median sequence length inflation vs IR tokens
- IR-as-text → BPE: **2.63x** inflation (BPE actively harms the structured IR format)
- Key fragmentation: `HMul.hMul` → 4 BPE tokens, `gen:OfNat.ofNat` → 6, `BVAR_0` → 4

## corpus_loader.py

Low-level schema validation and loading. Validates every line against the schema in
`CORPUS_SCHEMA.md`. Handles both v0.1.0 (`b(...)` / `t<n>`) and v1.0.0 (`BVAR_N` / `TERM_N`)
entity ID formats.

Key classes:
- `TrainingExample` — frozen dataclass: `name`, `module`, `lean_expr`, `graph`, `tokens`
- `CorpusDataset` — wraps examples + vocab dict; supports `len()` and index access
- `build_vocabulary` — builds `{token: int}` from corpus frequencies

## Known gaps

- No PyTorch `DataLoader` collate function yet — `CorpusDataset` is framework-agnostic.
- Training script (`train.py`) not yet written — `build_dataset.py` produces the datasets,
  the fine-tuning step is the next milestone.
- `mathlibCommitHash` field in `stats.json` is still `"unknown"` — Mathlib version is not
  automatically captured during corpus build.

## Next steps

1. Write `python/train.py` — fine-tune Qwen2.5-Coder-1.5B on each variant with fixed
   seeds/splits/hparams for the controlled A/B/C comparison
2. Evaluate perplexity on eval splits for each variant
3. Add `mathlibCommitHash` capture to `Scripts/BuildCorpus.lean`
