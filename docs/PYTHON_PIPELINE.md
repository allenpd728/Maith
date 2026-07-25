# Python Pipeline

The `python/` directory provides tooling to load the JSONL corpus produced by the Lean pipeline,
build a token vocabulary, split into train/eval sets, and expose dataset objects for model
training.

## Files

| File | Purpose |
|------|---------|
| `corpus_loader.py` | Schema validation, loading, vocab building, splitting, dataset class |
| `build_dataset.py` | CLI entry point — loads corpus and prints dataset summary |

## Requirements

Python 3.8+. No third-party dependencies — only the standard library.

## Usage

```bash
cd python
python build_dataset.py --corpus ../Corpus/corpus.jsonl
```

Optional flags:

| Flag | Default | Description |
|------|---------|-------------|
| `--corpus` | (required) | Path to `corpus.jsonl` |
| `--eval-ratio` | `0.2` | Fraction of examples held out for eval |
| `--seed` | `0` | Shuffle seed for reproducible splits |

Example output:

```
Loaded examples: 792
Vocabulary size: 1143
Train examples: 633
Eval examples: 159
```

## Schema validation

`corpus_loader.py` validates every line against the schema defined in `CORPUS_SCHEMA.md` before
returning examples. Any schema violation raises `SchemaError` with the line number and field path.

Entity ID kinds validated: `var`, `term`, `bound`. Note: `corpus_loader.py` currently validates
only `var` and `term` — the `bound` kind (scoped De Bruijn IDs added July 6) is not yet in the
validator. Update `_validate_entity_id` to add `"bound"` to the accepted kind set and check
`entity_id.get("scope")` is a string.

## Key classes

### `TrainingExample`

```python
@dataclass(frozen=True)
class TrainingExample:
    name: str        # Declaration name (e.g. "mul_assoc")
    module: str      # Source Mathlib module
    lean_expr: str   # Serialized elaborated declaration type
    graph: dict      # Normalized IR graph (entities/attributes/relations/operations)
    tokens: list     # Token sequence from Encoder
```

### `CorpusDataset`

Wraps a list of `TrainingExample` and a vocabulary dict. Supports `len()` and index access,
returning `{"name", "module", "token_ids", "graph"}` per item. Token IDs use `<UNK>` (id 1) for
out-of-vocabulary tokens. Compatible with PyTorch `DataLoader` via a standard `collate_fn`.

### `build_vocabulary`

Builds `{token: int}` from corpus token frequencies. Special tokens: `<PAD>` (0), `<UNK>` (1).
Accepts a `min_freq` argument (default 1) to filter rare tokens.

## Known gaps

- `_validate_entity_id` does not yet handle the `bound` entity ID kind (scope string format:
  `"declName/depth/binderName"`). Validation passes for existing corpus output but will need
  updating as extraction coverage grows.
- No PyTorch `Dataset` subclass or `collate_fn` is provided yet — `CorpusDataset` is framework-
  agnostic and needs a thin wrapper for use with `torch.utils.data.DataLoader`.
- No export format for HuggingFace `datasets` library yet.

## Next steps

1. Fix `bound` kind validation in `_validate_entity_id`
2. Add a `collate_fn` for batched token-id tensors
3. Add vocabulary export (`vocab.json`) alongside the dataset summary
4. Wire into a training script once the Encoder produces full token sequences
