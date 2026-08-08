# Project Dependencies

**Last Updated:** 2026-08-05  
**Purpose:** Catalog all dependencies required to run experiments in this repository.

This document is for agents taking over work. It lists every library needed, organized by purpose, with installation commands.

---

## 1. Python Dependencies

### Core ML Stack

| Package | Version | Purpose | Used By |
|---------|---------|---------|---------|
| `torch` | latest | Neural network framework, tensor operations | `train.py`, `extract_representations.py`, `probing_task.py` |
| `transformers` | latest | HuggingFace models (Qwen2.5-Coder), tokenizers | `train.py`, `extract_representations.py` |
| `datasets` | latest | HF datasets utilities (optional, for train.py) | `train.py` |
| `trl` | latest | Training utilities from HF (Trainer) | `train.py` |
| `scikit-learn` | latest | ML utilities: train_test_split, f1_score, balanced_accuracy_score | `probing_task.py` |

**Install command:**
```bash
pip install torch transformers trl datasets scikit-learn
```

### Standard Library (No Install Required)

These are bundled with Python 3.8+:

| Module | Purpose |
|--------|---------|
| `json` | Reading/writing JSON files |
| `argparse` | CLI argument parsing |
| `pathlib` / `os` | File path operations |
| `sys` | System utilities |
| `random` | Randomization |
| `math` | Math operations |
| `statistics` | Statistical calculations |
| `datetime` | Date/time handling |
| `tempfile` | Temporary file management |
| `shutil` | File operations |
| `subprocess` | Running external commands |
| `gc` | Garbage collection |
| `ast` | Python AST parsing |
| `collections` | Container datatypes |
| `typing` | Type hints |
| `dataclasses` | Dataclass decorators |

### Optional/Niche

| Package | Purpose | Used By |
|---------|---------|---------|
| `numpy` | Numerical computing | Some analysis scripts |

---

## 2. Lean Dependencies

### Lake Packages

| Package | Source | Version | Purpose |
|---------|--------|---------|---------|
| `mathlib` | GitHub | `v4.31.0` | Lean mathematics library |

**lakefile.lean:**
```lean
require mathlib from git "https://github.com/leanprover-community/mathlib4" @ "v4.31.0"
```

### Local Libraries

| Library | Path | Purpose |
|---------|------|---------|
| `Maith` | `Maith/*.lean` | Core IR extraction and encoding |
| `Tests` | `Tests/*.lean` | Unit and integration tests |

---

## 3. External Models

### HuggingFace Models

| Model | Cache Location | Purpose |
|-------|---------------|---------|
| `Qwen/Qwen2.5-Coder-0.5B` | `~/.cache/huggingface/hub/` | Base model for all variants; random baseline for probing |

Download happens automatically on first use via `transformers`.

---

## 4. Environment Requirements

### Hardware

| Resource | Minimum | Recommended | Purpose |
|----------|---------|-------------|---------|
| RAM | 16 GB | 32 GB | Model loading, dataset processing |
| GPU (CUDA/MPS) | Optional | Yes (MPS/CUDA) | Training and extraction |
| Disk | 10 GB | 20 GB | Checkpoints, datasets, model cache |

### Software

| Software | Version | Purpose |
|----------|---------|---------|
| Python | 3.8+ | Runtime |
| Lean | 4.x (matches `lean-toolchain`) | Compilation, testing |
| Lake | Latest | Lean build system |
| Git | Any | Version control |

---

## 5. Quick Setup

### Full Environment

```bash
# 1. Install Python dependencies
pip install torch transformers trl datasets scikit-learn

# 2. Initialize Lake (installs Lean + mathlib)
./elan-init.sh

# 3. Build tests
lake build tests

# 4. Run all Python tests
python3 python/test_build_dataset.py
python3 python/test_train_regression.py
python3 python/test_variant_config.py
python3 python/test_probing_scripts.py
```

### Probing Scripts Only

```bash
pip install torch transformers scikit-learn
python3 python/extract_representations.py --corpus Corpus/corpus.jsonl --out runs/probing
python3 python/probing_task.py --representations runs/probing --labels runs/probing/labels.json --out runs/probing
```

---

## 6. Script-to-Dependency Mapping

### Training Scripts

| Script | Dependencies |
|--------|-------------|
| `python/train.py` | `torch`, `transformers`, `trl`, `datasets` |

### Probing Scripts

| Script | Dependencies |
|--------|-------------|
| `python/extract_representations.py` | `torch`, `transformers` |
| `python/probing_task.py` | `torch`, `scikit-learn` |

### Dataset Scripts

| Script | Dependencies |
|--------|-------------|
| `python/build_dataset.py` | Standard library only |
| `python/build_flat_ir_dataset.py` | Standard library only |
| `python/corpus_loader.py` | Standard library only |

### Test Scripts

| Script | Dependencies |
|--------|-------------|
| `python/test_build_dataset.py` | Standard library only |
| `python/test_probing_scripts.py` | `torch`, `scikit-learn`, `numpy` |
| `python/test_train_regression.py` | `torch` (optional) |

### Analysis Scripts

| Script | Dependencies |
|--------|-------------|
| `python/compare_results.py` | Standard library + `numpy` |
| `python/qwen_prior_analysis.py` | Standard library only |

---

## 7. Troubleshooting

### "No module named 'torch'"
```bash
pip install torch
```

### "No module named 'transformers'"
```bash
pip install transformers
```

### "No module named 'scikit-learn'"
```bash
pip install scikit-learn
```

### "ModuleNotFoundError: Maith"
The Lean libraries aren't built. Run:
```bash
lake build
```

### "Lake not found"
Initialize the Lean toolchain:
```bash
./elan-init.sh
```

---

## 8. Verification

### Check Python Dependencies
```python
import torch
import transformers
import sklearn
print(f"torch: {torch.__version__}")
print(f"transformers: {transformers.__version__}")
print(f"sklearn available")
```

### Check Lean/Lake
```bash
lake --version
lake build tests  # Should succeed
```

---

## 9. Cached Artifacts

Some scripts read pre-computed artifacts. These don't require dependencies but need to exist:

| Artifact | Location | Created By |
|----------|----------|------------|
| `Corpus/corpus.jsonl` | Repo root | Lean extraction |
| `datasets/*.jsonl` | `datasets/` | `build_dataset.py` |
| `datasets_flat/*.jsonl` | `datasets_flat/` | `build_flat_ir_dataset.py` |
| `runs/variant_*/checkpoint-final/` | `runs/` | `train.py` |
| `runs/probing/*` | `runs/probing/` | Probing scripts |

---

*For questions about specific experiment dependencies, see the experiment proposal documents in `docs/`.*
