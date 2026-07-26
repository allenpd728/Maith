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
| `train.py` | Fine-tuning script — runs one A/B/C variant, reports eval perplexity |
| `compare_results.py` | Reads all three `runs/variant_*/results.json` and prints comparison table |
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

## train.py

Fine-tunes Qwen2.5-Coder-1.5B on one variant and reports eval perplexity.

```bash
pip install torch transformers trl datasets

# Smoke test — 1 epoch, 50 examples
python3 python/train.py --variant A --smoke-test

# Full runs (~20-40 min each on M-series Mac)
python3 python/train.py --variant A --out runs/variant_A
python3 python/train.py --variant B --out runs/variant_B
python3 python/train.py --variant C --out runs/variant_C
```

Saves `runs/variant_*/results.json` with perplexity, training time, and config. After all three
runs, the script prints a comparison table automatically.

Key design choices (from `docs/EXPERIMENT_DESIGN.md`):
- Variant A: embedding table resized to 4,233 IR tokens (Option 1 — hold architecture fixed)
- Variants B/C: native Qwen2.5-Coder BPE, no resizing
- Fixed seed 42, cosine LR schedule, 3 epochs, batch size 8 (effective)

## Known gaps

- No PyTorch `DataLoader` collate function in `corpus_loader.py` — `train.py` uses its own collate.
- Results not yet collected — smoke test in progress.

## Next steps

1. Complete smoke test, then run all three full variants
2. Record perplexity results in `docs/EXPERIMENT_DESIGN.md`
3. Expand corpus to more Mathlib modules once baseline results are in
