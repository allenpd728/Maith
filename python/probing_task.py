#!/usr/bin/env python3
"""
probing_task.py

Train linear probes on saved representations and report classification accuracy.

Task 1 — Module Classification (primary):
    11-class prediction from module labels

Task 2 — Arity Bin Prediction (secondary, --task arity):
    4-class prediction from arity_bin labels

Usage:
    python3 python/probing_task.py \
        --representations runs/probing \
        --labels runs/probing/labels.json \
        --out runs/probing \
        --task module

    # Run arity prediction:
    python3 python/probing_task.py \
        --representations runs/probing \
        --labels runs/probing/labels.json \
        --out runs/probing \
        --task arity
"""

import argparse
import json
import os
import sys
from collections import Counter

import numpy as np
import torch
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset
from sklearn.model_selection import train_test_split
from sklearn.metrics import f1_score, balanced_accuracy_score, confusion_matrix

# ---------------------------------------------------------------------------
# Dependency check
# ---------------------------------------------------------------------------

missing = []
try:
    import torch
except ImportError:
    missing.append("torch")
try:
    import numpy as np
except ImportError:
    missing.append("numpy")
try:
    from sklearn.model_selection import train_test_split
    from sklearn.metrics import f1_score, balanced_accuracy_score
except ImportError:
    missing.append("scikit-learn")

if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    print("Install with: pip install scikit-learn")
    sys.exit(1)

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

HIDDEN_SIZE = 896
PROBE_LR = 1e-3
PROBE_EPOCHS = 100
PROBE_BATCH_SIZE = 32
PROBE_SEEDS = [42, 43, 44]
SPLIT_SEED = 42
SPLIT_RATIO = 0.8

# Interpretation thresholds from Kit_Proposal.md Step 7
GAP_THRESHOLD_STRONG = 10  # pp
GAP_THRESHOLD_WEAK = 5    # pp

# Variants to probe
VARIANT_NAMES = ["A", "B", "C", "flat", "random"]

# ---------------------------------------------------------------------------
# Linear Probe
# ---------------------------------------------------------------------------

class LinearProbe(nn.Module):
    """Simple linear classifier for probing."""

    def __init__(self, input_dim: int, num_classes: int):
        super().__init__()
        self.linear = nn.Linear(input_dim, num_classes)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return self.linear(x)


# ---------------------------------------------------------------------------
# Label processing
# ---------------------------------------------------------------------------

def load_labels(labels_path: str, task: str) -> tuple[list[dict], list[int], list[str]]:
    """
    Load labels and build class mapping.
    
    Returns:
        labels: List of label dicts
        y: List of integer class labels
        class_names: List of class names in label order
    """
    with open(labels_path, "r") as f:
        labels = json.load(f)

    if task == "module":
        # Build 11-class mapping: top 10 modules + "Other"
        module_counts = Counter(lbl["module"] for lbl in labels)
        
        # Sort by count descending
        sorted_modules = sorted(module_counts.items(), key=lambda x: -x[1])
        
        # Top 10 become named classes
        top_10 = sorted_modules[:10]
        
        # Build mapping
        class_mapping = {}
        class_names = []
        for i, (module, count) in enumerate(top_10):
            short_name = module.split(".")[-3:] if "." in module else [module]
            short_name = ".".join(short_name) if isinstance(short_name, list) else short_name
            class_mapping[module] = (i, short_name)
            class_names.append(short_name)
        
        # Remaining modules → "Other" (index 10)
        other_modules = set(m for m, c in sorted_modules[10:])
        class_mapping["_other"] = (10, "Other")
        class_names.append("Other")
        
        # Convert labels to integers
        y = []
        for lbl in labels:
            module = lbl["module"]
            if module in class_mapping:
                y.append(class_mapping[module][0])
            else:
                y.append(10)  # Other
        
        return labels, y, class_names

    elif task == "arity":
        # 4-class prediction from arity_bin (already integers 0-3)
        y = [lbl["arity_bin"] for lbl in labels]
        class_names = ["0-5 ops", "6-15 ops", "16-40 ops", "40+ ops"]
        return labels, y, class_names

    else:
        raise ValueError(f"Unknown task: {task}")


def stratified_split(X: np.ndarray, y: np.ndarray, seed: int) -> tuple:
    """Stratified 80/20 train/eval split."""
    return train_test_split(
        X, y,
        test_size=1 - SPLIT_RATIO,
        stratify=y,
        random_state=seed,
    )


# ---------------------------------------------------------------------------
# Probe training
# ---------------------------------------------------------------------------

def train_probe(
    X_train: np.ndarray,
    y_train: np.ndarray,
    X_eval: np.ndarray,
    y_eval: np.ndarray,
    num_classes: int,
    seed: int,
    epochs: int = PROBE_EPOCHS,
) -> dict:
    """
    Train a linear probe and return metrics.
    
    Returns dict with accuracy, macro_f1, balanced_accuracy, and per-class f1.
    """
    torch.manual_seed(seed)
    np.random.seed(seed)

    # Convert to tensors
    X_train_t = torch.tensor(X_train, dtype=torch.float32)
    y_train_t = torch.tensor(y_train, dtype=torch.long)
    X_eval_t = torch.tensor(X_eval, dtype=torch.float32)
    y_eval_t = torch.tensor(y_eval, dtype=torch.long)

    train_dataset = TensorDataset(X_train_t, y_train_t)
    train_loader = DataLoader(train_dataset, batch_size=PROBE_BATCH_SIZE, shuffle=True)

    # Initialize probe
    probe = LinearProbe(HIDDEN_SIZE, num_classes)
    criterion = nn.CrossEntropyLoss()
    optimizer = torch.optim.Adam(probe.parameters(), lr=PROBE_LR)

    # Training loop
    for epoch in range(epochs):
        probe.train()
        for batch_x, batch_y in train_loader:
            optimizer.zero_grad()
            logits = probe(batch_x)
            loss = criterion(logits, batch_y)
            loss.backward()
            optimizer.step()

    # Evaluation
    probe.eval()
    with torch.no_grad():
        logits = probe(X_eval_t)
        preds = logits.argmax(dim=1).numpy()

    # Compute metrics
    accuracy = (preds == y_eval).mean()
    macro_f1 = f1_score(y_eval, preds, average="macro", zero_division=0)
    balanced_acc = balanced_accuracy_score(y_eval, preds)
    
    # Per-class F1
    per_class_f1 = {}
    for cls in range(num_classes):
        cls_mask = y_eval == cls
        if cls_mask.sum() > 0:
            cls_f1 = f1_score(
                (y_eval == cls).astype(int),
                (preds == cls).astype(int),
                zero_division=0,
            )
            per_class_f1[cls] = cls_f1

    return {
        "accuracy": float(accuracy),
        "macro_f1": float(macro_f1),
        "balanced_accuracy": float(balanced_acc),
        "per_class_f1": per_class_f1,
    }


def run_probe_training(
    X: np.ndarray,
    y: np.ndarray,
    num_classes: int,
    seeds: list[int] = PROBE_SEEDS,
) -> dict:
    """
    Run probe training across multiple seeds and aggregate results.
    """
    # Stratified split (same for all seeds)
    X_train, X_eval, y_train, y_eval = stratified_split(X, y, SPLIT_SEED)

    results = []
    for seed in seeds:
        result = train_probe(X_train, y_train, X_eval, y_eval, num_classes, seed)
        results.append(result)

    # Aggregate
    accs = [r["accuracy"] for r in results]
    f1s = [r["macro_f1"] for r in results]
    bal_accs = [r["balanced_accuracy"] for r in results]

    # Find best seed by accuracy
    best_idx = np.argmax(accs)
    best_result = results[best_idx]
    best_seed = seeds[best_idx]

    return {
        "seeds": seeds,
        "best_seed": best_seed,
        "accuracy_mean": float(np.mean(accs)),
        "accuracy_std": float(np.std(accs)),
        "macro_f1_mean": float(np.mean(f1s)),
        "macro_f1_std": float(np.std(f1s)),
        "balanced_accuracy_mean": float(np.mean(bal_accs)),
        "balanced_accuracy_std": float(np.std(bal_accs)),
        "best_seed_results": best_result,
        "all_results": results,
    }


def random_baseline(y_eval: np.ndarray, num_classes: int, n_samples: int = 1000) -> float:
    """
    Compute random baseline accuracy (uniform random assignment).
    """
    np.random.seed(42)
    random_preds = np.random.randint(0, num_classes, size=(n_samples, len(y_eval)))
    y_eval_expanded = np.tile(y_eval, (n_samples, 1))
    accuracies = (random_preds == y_eval_expanded).mean(axis=1)
    return float(np.mean(accuracies))


def interpret_outcome(gap_pp: float) -> str:
    """
    Interpret the A vs flat gap using Step 7 thresholds.
    
    Args:
        gap_pp: Difference in accuracy (A - flat) in percentage points
        
    Returns:
        outcome: "A_encodes_semantics" | "flat_matches_A" | "inconclusive"
    """
    if gap_pp >= GAP_THRESHOLD_STRONG:
        return "A_encodes_semantics"
    elif gap_pp < GAP_THRESHOLD_WEAK:
        return "flat_matches_A"
    else:
        return "inconclusive"


# ---------------------------------------------------------------------------
# Main pipeline
# ---------------------------------------------------------------------------

def run_probing(
    representations_dir: str,
    labels_path: str,
    out_dir: str,
    task: str,
) -> None:
    """Main probing pipeline."""

    # Load labels
    print(f"Loading labels from: {labels_path}")
    labels, y, class_names = load_labels(labels_path, task)
    num_classes = len(class_names)
    print(f"Task: {task}")
    print(f"Number of classes: {num_classes}")
    print(f"Class names: {class_names}")
    print(f"Number of examples: {len(y)}")
    print()

    # Load representations
    variant_results = {}
    for variant in VARIANT_NAMES:
        repr_path = os.path.join(representations_dir, f"representations_{variant}.pt")
        if not os.path.exists(repr_path):
            print(f"WARNING: Representations not found for {variant}: {repr_path}")
            continue

        print(f"{'='*60}")
        print(f"PROBING VARIANT: {variant}")
        print(f"{'='*60}")

        # Load
        representations = torch.load(repr_path)
        X = representations.numpy()
        print(f"  Loaded representations: {X.shape}")

        # Run probe training
        print(f"  Training probes with seeds {PROBE_SEEDS}...")
        results = run_probe_training(X, np.array(y), num_classes)
        
        # Print summary
        print(f"  Results (mean ± std across {len(PROBE_SEEDS)} seeds):")
        print(f"    Accuracy:          {results['accuracy_mean']:.3f} ± {results['accuracy_std']:.3f}")
        print(f"    Macro-F1:          {results['macro_f1_mean']:.3f} ± {results['macro_f1_std']:.3f}")
        print(f"    Balanced Accuracy: {results['balanced_accuracy_mean']:.3f} ± {results['balanced_accuracy_std']:.3f}")
        print(f"    Best seed: {results['best_seed']}")

        variant_results[variant] = results
        print()

    # Random baseline
    print(f"{'='*60}")
    print("RANDOM BASELINE")
    print(f"{'='*60}")
    random_acc = random_baseline(np.array(y), num_classes)
    print(f"  Random baseline accuracy: {random_acc:.3f}")
    print()

    # Build output JSON
    output = {
        "task": f"{task}_classification",
        "n_classes": num_classes,
        "class_names": class_names,
        "random_baseline_accuracy": random_acc,
        "variants": {},
        "interpretation": {},
    }

    for variant, results in variant_results.items():
        output["variants"][variant] = {
            "seeds": results["seeds"],
            "accuracy_mean": results["accuracy_mean"],
            "accuracy_std": results["accuracy_std"],
            "macro_f1_mean": results["macro_f1_mean"],
            "macro_f1_std": results["macro_f1_std"],
            "balanced_accuracy_mean": results["balanced_accuracy_mean"],
            "balanced_accuracy_std": results["balanced_accuracy_std"],
            "best_seed_per_class_f1": results["best_seed_results"]["per_class_f1"],
        }

    # Compute interpretation (for task=module, compare A vs flat)
    if task == "module" and "A" in variant_results and "flat" in variant_results:
        acc_a = variant_results["A"]["accuracy_mean"]
        acc_flat = variant_results["flat"]["accuracy_mean"]
        gap_pp = (acc_a - acc_flat) * 100  # Convert to percentage points
        
        outcome = interpret_outcome(gap_pp)
        
        output["interpretation"] = {
            "A_vs_flat_gap_pp": round(gap_pp, 2),
            "outcome": outcome,
        }
        
        print(f"{'='*60}")
        print("INTERPRETATION")
        print(f"{'='*60}")
        print(f"  A accuracy: {acc_a:.3f}")
        print(f"  Flat accuracy: {acc_flat:.3f}")
        print(f"  Gap: {gap_pp:.1f} pp")
        print(f"  Outcome: {outcome}")
        print()

    # Save output
    os.makedirs(out_dir, exist_ok=True)
    if task == "module":
        out_path = os.path.join(out_dir, "task1_results.json")
    else:
        out_path = os.path.join(out_dir, "task2_results.json")
    
    with open(out_path, "w") as f:
        json.dump(output, f, indent=2)
    print(f"Results saved → {out_path}")

    print()
    print("="*60)
    print("PROBING COMPLETE")
    print("="*60)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Train and evaluate linear probes on saved representations."
    )
    parser.add_argument(
        "--representations",
        default="runs/probing",
        help="Directory containing representations (default: runs/probing)",
    )
    parser.add_argument(
        "--labels",
        default="runs/probing/labels.json",
        help="Path to labels JSON file (default: runs/probing/labels.json)",
    )
    parser.add_argument(
        "--out",
        default="runs/probing",
        help="Output directory for results (default: runs/probing)",
    )
    parser.add_argument(
        "--task",
        default="module",
        choices=["module", "arity"],
        help="Task to run: 'module' for module classification, 'arity' for arity bin prediction",
    )

    args = parser.parse_args()

    # Validate inputs
    if not os.path.exists(args.labels):
        print(f"ERROR: Labels file not found: {args.labels}")
        sys.exit(1)

    run_probing(args.representations, args.labels, args.out, args.task)


if __name__ == "__main__":
    main()
