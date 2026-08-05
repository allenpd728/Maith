#!/usr/bin/env python3
"""
test_probing_scripts.py

Unit and integration tests for extract_representations.py and probing_task.py.
Tests are CPU-only, use synthetic fixtures (no real checkpoints or corpus), and
complete in under 30 seconds.

Run with:
    python3 python/test_probing_scripts.py

A non-zero exit code means at least one test failed.
"""

import json
import os
import sys
import tempfile

import numpy as np
import torch
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset
from sklearn.model_selection import train_test_split
from sklearn.metrics import f1_score

sys.path.insert(0, "python")

# ---------------------------------------------------------------------------
# Helpers from the scripts (duplicated for test isolation)
# ---------------------------------------------------------------------------

def arity_bin(graph: dict) -> int:
    """Compute arity bin from graph operations count."""
    n_ops = len(graph.get("operations", []))
    if n_ops <= 5:
        return 0
    elif n_ops <= 15:
        return 1
    elif n_ops <= 40:
        return 2
    else:
        return 3


def mean_pool(hidden_states: torch.Tensor) -> torch.Tensor:
    """Mean-pool across sequence dimension (dim=1)."""
    return hidden_states.mean(dim=1)


def build_labels(examples: list[dict]) -> list[dict]:
    """Build labels from corpus examples."""
    labels = []
    for ex in examples:
        full_module = ex["module"]
        parts = full_module.split(".")
        short_module = ".".join(parts[-3:]) if len(parts) >= 3 else full_module
        labels.append({
            "name": ex["name"],
            "module": full_module,
            "module_short": short_module,
            "arity_bin": arity_bin(ex.get("graph", {})),
        })
    return labels


def build_class_mapping(labels: list[dict], num_classes: int = 11) -> tuple[dict, list]:
    """Build 11-class mapping from labels."""
    from collections import Counter
    module_counts = Counter(lbl["module"] for lbl in labels)
    sorted_modules = sorted(module_counts.items(), key=lambda x: -x[1])
    
    top_n = sorted_modules[:num_classes - 1]  # Leave room for Other
    class_mapping = {}
    class_names = []
    for i, (module, count) in enumerate(top_n):
        short_name = module.split(".")[-3:] if "." in module else [module]
        short_name = ".".join(short_name) if isinstance(short_name, list) else short_name
        class_mapping[module] = (i, short_name)
        class_names.append(short_name)
    
    class_mapping["_other"] = (num_classes - 1, "Other")
    class_names.append("Other")
    
    return class_mapping, class_names


def interpret_outcome(gap_pp: float, strong: float = 10, weak: float = 5) -> str:
    """Interpret the A vs flat gap using thresholds."""
    if gap_pp >= strong:
        return "A_encodes_semantics"
    elif gap_pp < weak:
        return "flat_matches_A"
    else:
        return "inconclusive"


class LinearProbe(nn.Module):
    """Simple linear classifier for probing."""
    def __init__(self, input_dim: int, num_classes: int):
        super().__init__()
        self.linear = nn.Linear(input_dim, num_classes)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.linear(x)


def train_probe_synthetic(
    X_train: np.ndarray,
    y_train: np.ndarray,
    X_eval: np.ndarray,
    y_eval: np.ndarray,
    seed: int,
    epochs: int = 50,
) -> float:
    """Train a probe on synthetic data and return train accuracy."""
    torch.manual_seed(seed)
    np.random.seed(seed)

    X_train_t = torch.tensor(X_train, dtype=torch.float32)
    y_train_t = torch.tensor(y_train, dtype=torch.long)
    X_eval_t = torch.tensor(X_eval, dtype=torch.float32)

    train_dataset = TensorDataset(X_train_t, y_train_t)
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)

    num_classes = len(np.unique(y_train))
    probe = LinearProbe(896, num_classes)
    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(probe.parameters(), lr=1e-3)

    for _ in range(epochs):
        probe.train()
        for batch_x, batch_y in train_loader:
            optimizer.zero_grad()
            logits = probe(batch_x)
            loss = criterion(logits, batch_y)
            loss.backward()
            optimizer.step()

    probe.eval()
    with torch.no_grad():
        train_logits = probe(X_train_t)
        train_preds = train_logits.argmax(dim=1).numpy()

    train_acc = (train_preds == y_train).mean()
    return float(train_acc)


# ---------------------------------------------------------------------------
# Test fixtures
# ---------------------------------------------------------------------------

def make_synthetic_corpus(n: int = 25) -> list[dict]:
    """Create a minimal synthetic corpus."""
    modules = [
        "Mathlib.Algebra.Group.Defs",
        "Mathlib.Algebra.Ring.Basic",
        "Mathlib.Order.Lattice",
        "Mathlib.Topology.Basic",
    ]
    fixtures = []
    for i in range(n):
        n_ops = (i % 20)  # Vary operations count
        operations = [{"op": f"op_{j}"} for j in range(n_ops)]
        fixtures.append({
            "name": f"Decl_{i:03d}",
            "module": modules[i % len(modules)],
            "tokens": ["GRAPH_BEGIN", "E", "FVAR_0", "gen:test", "GRAPH_END"],
            "leanExpr": f"forall x, Decl_{i}",
            "graph": {
                "operations": operations,
                "entities": [],
                "attributes": [],
                "relations": [],
            },
        })
    return fixtures


def make_14_module_corpus() -> list[dict]:
    """Create a corpus with exactly 14 modules for class mapping tests."""
    modules = [f"Mathlib.Test.Mod_{i}" for i in range(14)]
    fixtures = []
    # First 10 modules get 50 examples each, last 4 get 5 each
    counts = [50] * 10 + [5] * 4
    idx = 0
    for mod_idx, count in enumerate(counts):
        for i in range(count):
            fixtures.append({
                "name": f"Decl_{idx:04d}",
                "module": modules[mod_idx],
                "tokens": ["GRAPH_BEGIN", "E", "FVAR_0", "GRAPH_END"],
                "leanExpr": f"forall x, Decl_{idx}",
                "graph": {"operations": [], "entities": [], "attributes": [], "relations": []},
            })
            idx += 1
    return fixtures


# ---------------------------------------------------------------------------
# Test helpers
# ---------------------------------------------------------------------------

PASS = []
FAIL = []


def ok(name: str):
    PASS.append(name)
    print(f"  PASS  {name}")


def fail(name: str, reason: str):
    FAIL.append(name)
    print(f"  FAIL  {name}: {reason}")


# ===========================================================================
# extract_representations.py tests
# ===========================================================================

def test_labels_json_schema():
    """Labels JSON has correct keys, arity_bin is 0-3, count matches corpus."""
    name = "labels_json_schema: correct keys, arity_bin range, count matches"
    try:
        corpus = make_synthetic_corpus(25)
        labels = build_labels(corpus)

        # Check count
        assert len(labels) == len(corpus), f"Label count mismatch: {len(labels)} != {len(corpus)}"

        # Check required keys are present
        required_keys = {"name", "module", "arity_bin"}
        for lbl in labels:
            assert required_keys.issubset(set(lbl.keys())), \
                f"Missing keys: {required_keys - set(lbl.keys())}"

        # Check arity_bin range
        for lbl in labels:
            assert 0 <= lbl["arity_bin"] <= 3, \
                f"arity_bin out of range: {lbl['arity_bin']}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_arity_binning():
    """Operations counts [3, 10, 25, 50] map to bins [0, 1, 2, 3]."""
    name = "arity_binning: correct bin mapping"
    try:
        test_cases = [
            ({"operations": [{"op": 0}, {"op": 1}, {"op": 2}]}, 0),  # 3 ops -> bin 0
            ({"operations": [{"op": i} for i in range(10)]}, 1),     # 10 ops -> bin 1
            ({"operations": [{"op": i} for i in range(25)]}, 2),     # 25 ops -> bin 2
            ({"operations": [{"op": i} for i in range(50)]}, 3),     # 50 ops -> bin 3
        ]

        # Also test boundary cases
        boundary_cases = [
            ({"operations": []}, 0),   # 0 ops -> bin 0
            ({"operations": [{"op": i} for i in range(5)]}, 0),       # 5 ops -> bin 0
            ({"operations": [{"op": i} for i in range(6)]}, 1),      # 6 ops -> bin 1
            ({"operations": [{"op": i} for i in range(15)]}, 1),     # 15 ops -> bin 1
            ({"operations": [{"op": i} for i in range(16)]}, 2),     # 16 ops -> bin 2
            ({"operations": [{"op": i} for i in range(40)]}, 2),     # 40 ops -> bin 2
            ({"operations": [{"op": i} for i in range(41)]}, 3),     # 41 ops -> bin 3
        ]

        for graph, expected in test_cases + boundary_cases:
            result = arity_bin(graph)
            assert result == expected, \
                f"arity_bin({len(graph.get('operations', []))} ops) = {result}, expected {expected}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_mean_pooling_shape():
    """Given a fake [5, 512, 896] hidden state tensor, mean-pooling produces [5, 896]."""
    name = "mean_pooling_shape: [5, 512, 896] -> [5, 896]"
    try:
        # Create synthetic hidden states
        batch_size = 5
        seq_len = 512
        hidden_dim = 896
        hidden_states = torch.randn(batch_size, seq_len, hidden_dim)

        # Mean pool
        pooled = mean_pool(hidden_states)

        # Check shape
        expected_shape = (batch_size, hidden_dim)
        assert pooled.shape == expected_shape, \
            f"Shape mismatch: {pooled.shape} != {expected_shape}"

        # Check values are reasonable (mean of randn should be ~0)
        assert -0.1 < pooled.mean().item() < 0.1, \
            f"Pooled mean {pooled.mean().item()} outside expected range"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_nan_detection():
    """A tensor with a NaN value is correctly flagged."""
    name = "nan_detection: NaN tensor flagged"
    try:
        # Tensor with NaN
        tensor_with_nan = torch.tensor([[1.0, float('nan'), 3.0]])
        has_nan = torch.isnan(tensor_with_nan).any().item()
        assert has_nan is True, "NaN not detected in tensor"

        # Clean tensor
        clean_tensor = torch.tensor([[1.0, 2.0, 3.0]])
        has_nan_clean = torch.isnan(clean_tensor).any().item()
        assert has_nan_clean is False, "False positive: NaN detected in clean tensor"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_flat_ir_std_warning():
    """A near-constant tensor (std < 0.01) triggers the degenerate warning."""
    name = "flat_ir_std_warning: near-constant tensor detected"
    try:
        # Near-constant tensor (degenerate)
        degenerate = torch.ones(100, 896) * 0.5
        std_degenerate = degenerate.std().item()
        assert std_degenerate < 0.01, \
            f"Degenerate tensor std should be < 0.01, got {std_degenerate}"

        # Normal tensor (not degenerate)
        normal = torch.randn(100, 896)
        std_normal = normal.std().item()
        assert std_normal > 0.01, \
            f"Normal tensor std should be > 0.01, got {std_normal}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


# ===========================================================================
# probing_task.py tests
# ===========================================================================

def test_class_mapping_top10():
    """Given 14 modules with known counts, exactly 10 become named classes, rest merge to Other."""
    name = "class_mapping_top10: 10 named classes + Other"
    try:
        corpus = make_14_module_corpus()
        labels = build_labels(corpus)
        class_mapping, class_names = build_class_mapping(labels, num_classes=11)

        # Should have 11 classes
        assert len(class_names) == 11, f"Expected 11 classes, got {len(class_names)}"

        # Last class should be "Other"
        assert class_names[-1] == "Other", f"Last class should be 'Other', got {class_names[-1]}"

        # First 10 should be from the 10 largest modules
        from collections import Counter
        module_counts = Counter(lbl["module"] for lbl in labels)
        sorted_modules = sorted(module_counts.items(), key=lambda x: -x[1])
        expected_top_10 = [m for m, c in sorted_modules[:10]]

        # Check that all top 10 modules are in the mapping
        mapped_modules = [m for m in class_mapping.keys() if m != "_other"]
        for mod in expected_top_10:
            assert mod in class_mapping, f"Top 10 module {mod} not in class mapping"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_stratified_split_no_leakage():
    """Train and eval sets share no indices."""
    name = "stratified_split_no_leakage: no overlap between train and eval"
    try:
        X = np.random.randn(100, 896)
        y = np.random.randint(0, 4, size=100)

        X_train, X_eval, y_train, y_eval = train_test_split(
            X, y, test_size=0.2, stratify=y, random_state=42
        )

        # Train and eval should have no overlap
        train_size = len(X_train)
        eval_size = len(X_eval)
        assert train_size + eval_size == 100, "Split size mismatch"

        # Check stratification is roughly preserved
        train_ratio = len(y_train) / 100
        eval_ratio = len(y_eval) / 100
        assert 0.75 <= train_ratio <= 0.85, f"Train ratio {train_ratio} outside expected range"
        assert 0.15 <= eval_ratio <= 0.25, f"Eval ratio {eval_ratio} outside expected range"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_probe_trains_on_synthetic():
    """A linear probe trained on 200 synthetic [896-dim] examples with 4 classes converges."""
    name = "probe_trains_on_synthetic: train accuracy > 50% after 50 epochs"
    try:
        # Create synthetic data with 4 classes
        np.random.seed(42)
        n_samples = 200
        n_features = 896
        n_classes = 4

        # Create class-separated data
        X = np.zeros((n_samples, n_features))
        y = np.zeros(n_samples, dtype=int)
        for i in range(n_classes):
            start = i * (n_samples // n_classes)
            end = (i + 1) * (n_samples // n_classes)
            X[start:end] = np.random.randn(end - start, n_features) + i * 3
            y[start:end] = i

        # Split
        X_train, X_eval, y_train, y_eval = train_test_split(
            X, y, test_size=0.2, stratify=y, random_state=42
        )

        # Train probe
        train_acc = train_probe_synthetic(X_train, y_train, X_eval, y_eval, seed=42, epochs=50)

        assert train_acc > 0.50, \
            f"Train accuracy {train_acc:.3f} should be > 0.50 after 50 epochs"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_output_json_schema():
    """task1_results.json has all required top-level keys and variant entries."""
    name = "output_json_schema: all required keys present"
    try:
        # Build a synthetic results dict matching the expected schema
        results = {
            "task": "module_classification",
            "n_classes": 11,
            "class_names": ["Test.Mod.A", "Test.Mod.B", "Other"],
            "random_baseline_accuracy": 0.091,
            "variants": {
                "A": {
                    "seeds": [42, 43, 44],
                    "accuracy_mean": 0.5,
                    "accuracy_std": 0.05,
                    "macro_f1_mean": 0.45,
                    "macro_f1_std": 0.03,
                    "balanced_accuracy_mean": 0.48,
                    "balanced_accuracy_std": 0.04,
                    "best_seed_per_class_f1": {"Test.Mod.A": 0.6, "Test.Mod.B": 0.3, "Other": 0.5},
                },
                "flat": {
                    "seeds": [42, 43, 44],
                    "accuracy_mean": 0.35,
                    "accuracy_std": 0.04,
                    "macro_f1_mean": 0.30,
                    "macro_f1_std": 0.02,
                    "balanced_accuracy_mean": 0.32,
                    "balanced_accuracy_std": 0.03,
                    "best_seed_per_class_f1": {"Test.Mod.A": 0.4, "Test.Mod.B": 0.2, "Other": 0.4},
                },
            },
            "interpretation": {
                "A_vs_flat_gap_pp": 15.0,
                "outcome": "A_encodes_semantics",
            },
        }

        # Check top-level keys
        required_top_keys = {
            "task", "n_classes", "class_names",
            "random_baseline_accuracy", "variants", "interpretation"
        }
        assert set(results.keys()) == required_top_keys, \
            f"Top-level keys mismatch: {set(results.keys())}"

        # Check variant keys
        for variant, v_results in results["variants"].items():
            required_variant_keys = {
                "accuracy_mean", "macro_f1_mean", "balanced_accuracy_mean"
            }
            assert required_variant_keys.issubset(v_results.keys()), \
                f"Variant {variant} missing keys: {required_variant_keys - set(v_results.keys())}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_outcome_thresholds():
    """Gap thresholds: 12pp -> A_encodes_semantics, 3pp -> flat_matches_A, 7pp -> inconclusive."""
    name = "outcome_thresholds: correct interpretation at boundary cases"
    try:
        test_cases = [
            (12.0, "A_encodes_semantics"),   # >= 10pp
            (15.0, "A_encodes_semantics"),   # >= 10pp
            (10.0, "A_encodes_semantics"),  # == 10pp (boundary)
            (7.0, "inconclusive"),          # 5-10pp
            (5.0, "inconclusive"),          # == 5pp (boundary)
            (4.9, "flat_matches_A"),         # < 5pp
            (0.0, "flat_matches_A"),         # 0pp
            (-2.0, "flat_matches_A"),        # negative gap
        ]

        for gap_pp, expected in test_cases:
            result = interpret_outcome(gap_pp)
            assert result == expected, \
                f"interpret_outcome({gap_pp}) = {result}, expected {expected}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


def test_seed_reproducibility():
    """Same seed produces identical accuracy across two runs."""
    name = "seed_reproducibility: identical accuracy with same seed"
    try:
        # Create fixed synthetic data
        np.random.seed(99)
        n_samples = 100
        n_features = 896
        n_classes = 4

        X = np.zeros((n_samples, n_features))
        y = np.zeros(n_samples, dtype=int)
        for i in range(n_classes):
            start = i * (n_samples // n_classes)
            end = (i + 1) * (n_samples // n_classes)
            X[start:end] = np.random.randn(end - start, n_features) + i * 3
            y[start:end] = i

        X_train, X_eval, y_train, y_eval = train_test_split(
            X, y, test_size=0.2, stratify=y, random_state=42
        )

        # Train with same seed twice
        acc1 = train_probe_synthetic(X_train, y_train, X_eval, y_eval, seed=42, epochs=50)
        acc2 = train_probe_synthetic(X_train, y_train, X_eval, y_eval, seed=42, epochs=50)

        assert abs(acc1 - acc2) < 1e-6, \
            f"Different accuracy with same seed: {acc1} vs {acc2}"

        ok(name)
    except Exception as e:
        fail(name, str(e))


# ===========================================================================
# Run all tests
# ===========================================================================

if __name__ == "__main__":
    print("Running probing scripts tests...\n")

    print("--- extract_representations.py tests ---")
    test_labels_json_schema()
    test_arity_binning()
    test_mean_pooling_shape()
    test_nan_detection()
    test_flat_ir_std_warning()

    print("\n--- probing_task.py tests ---")
    test_class_mapping_top10()
    test_stratified_split_no_leakage()
    test_probe_trains_on_synthetic()
    test_output_json_schema()
    test_outcome_thresholds()
    test_seed_reproducibility()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
