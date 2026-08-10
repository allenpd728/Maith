#!/usr/bin/env python3
"""
retrieval_eval.py

Evaluate dependency retrieval for H6: given each eval declaration's embedding,
rank the training-set declarations by cosine similarity and measure whether the
declaration's true dependencies are retrieved near the top.

Compares IR (variant A) vs BPE (B_small, B) vs flat-IR ablation vs random.
The primary gate: does A beat B_small on Recall@10?

Two modes:
  Mode 1 (eval → train): query = eval declarations, pool = train declarations
  Mode 2 (train → train): query = train declarations, pool = train (leave-one-out)

Metrics: Recall@{1,5,10,20}, MRR, with bootstrap 95% CIs.

Usage:
    python3 python/retrieval_eval.py \\
        --embeddings-dir runs/retrieval_embeddings \\
        --groundtruth datasets/dependency_groundtruth_eval_to_train.json \\
        --out runs/h6_retrieval/results.json
"""

import argparse
import json
import os
import sys
from pathlib import Path

import numpy as np
import torch

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def load_embeddings(embeddings_dir: Path, variant: str, split: str) -> tuple[torch.Tensor, list[str]]:
    """Load embeddings and names for a variant/split."""
    emb_path = embeddings_dir / f"embeddings_{variant}_{split}.pt"
    names_path = embeddings_dir / f"names_{variant}_{split}.json"
    if not emb_path.exists():
        return None, None
    embeddings = torch.load(emb_path, weights_only=True)
    with open(names_path) as f:
        names = json.load(f)
    return embeddings, names


def cosine_similarity_matrix(queries: torch.Tensor, pool: torch.Tensor) -> torch.Tensor:
    """Compute cosine similarity [n_queries, n_pool]."""
    # Normalize
    q = queries / (queries.norm(dim=1, keepdim=True) + 1e-8)
    p = pool / (pool.norm(dim=1, keepdim=True) + 1e-8)
    return q @ p.T  # [n_queries, n_pool]


def compute_recall_at_k(sim: np.ndarray, gt_deps: dict, pool_names: list[str], k_values: list[int]) -> dict:
    """Compute Recall@k and MRR for each query.

    sim: [n_queries, n_pool] cosine similarities
    gt_deps: {query_name: [dep_name, ...]}
    pool_names: list of declaration names in the pool (aligned with sim columns)
    """
    pool_idx = {name: i for i, name in enumerate(pool_names)}
    results = {k: [] for k in k_values}
    mrrs = []

    for query_name, deps in gt_deps.items():
        # Find which pool indices are true deps
        dep_indices = set()
        for d in deps:
            if d in pool_idx:
                dep_indices.add(pool_idx[d])

        if not dep_indices:
            continue  # no in-pool deps, skip

        # Get similarities for this query
        # Find the query index in the sim matrix
        # (sim is indexed by position, we need to map query_name → row)
        # This is handled by the caller passing aligned arrays

    # Actually, we need the query names too to align. Let me restructure.
    return None  # will use the function below instead


def evaluate_retrieval(
    query_embeddings: torch.Tensor,
    query_names: list[str],
    pool_embeddings: torch.Tensor,
    pool_names: list[str],
    gt_deps: dict,
    k_values: list[int] = None,
) -> dict:
    """Evaluate retrieval for one variant.

    Returns dict with Recall@k, MRR, and per-query details.
    """
    if k_values is None:
        k_values = [1, 5, 10, 20]

    # Compute cosine similarity [n_queries, n_pool]
    sim = cosine_similarity_matrix(query_embeddings, pool_embeddings).numpy()

    pool_idx = {name: i for i, name in enumerate(pool_names)}

    per_query = {k: [] for k in k_values}
    mrrs = []
    n_queryable = 0

    for qi, query_name in enumerate(query_names):
        if query_name not in gt_deps:
            continue
        deps = gt_deps[query_name]

        # Find which pool indices are true deps
        dep_indices = []
        for d in deps:
            if d in pool_idx:
                dep_indices.append(pool_idx[d])

        if not dep_indices:
            continue

        n_queryable += 1
        dep_set = set(dep_indices)
        n_deps = len(dep_set)

        # Rank pool by similarity (descending)
        sim_row = sim[qi]
        ranked_indices = np.argsort(-sim_row)

        # Recall@k: fraction of deps in top-k
        for k in k_values:
            top_k = set(ranked_indices[:k].tolist())
            hits = len(dep_set & top_k)
            recall = hits / n_deps
            per_query[k].append(recall)

        # MRR: reciprocal rank of first dep hit
        for rank, idx in enumerate(ranked_indices, 1):
            if idx in dep_set:
                mrrs.append(1.0 / rank)
                break
        else:
            mrrs.append(0.0)

    # Aggregate
    results = {}
    for k in k_values:
        vals = per_query[k]
        if vals:
            results[f"recall@{k}"] = {
                "mean": float(np.mean(vals)),
                "std": float(np.std(vals)),
                "n": len(vals),
            }

    if mrrs:
        results["mrr"] = {
            "mean": float(np.mean(mrrs)),
            "std": float(np.std(mrrs)),
            "n": len(mrrs),
        }

    results["n_queryable"] = n_queryable
    results["pool_size"] = len(pool_names)
    return results


def bootstrap_ci(values: list[float], n_resamples: int = 1000, confidence: float = 0.95) -> tuple[float, float]:
    """Bootstrap confidence interval for the mean."""
    if not values:
        return (0.0, 0.0)
    arr = np.array(values)
    means = []
    for _ in range(n_resamples):
        sample = np.random.choice(arr, size=len(arr), replace=True)
        means.append(np.mean(sample))
    alpha = (1 - confidence) / 2
    lower = float(np.percentile(means, alpha * 100))
    upper = float(np.percentile(means, (1 - alpha) * 100))
    return (lower, upper)


def evaluate_with_ci(
    query_embeddings: torch.Tensor,
    query_names: list[str],
    pool_embeddings: torch.Tensor,
    pool_names: list[str],
    gt_deps: dict,
    k_values: list[int] = None,
    n_bootstrap: int = 1000,
) -> dict:
    """Evaluate retrieval with bootstrap confidence intervals."""
    if k_values is None:
        k_values = [1, 5, 10, 20]

    sim = cosine_similarity_matrix(query_embeddings, pool_embeddings).numpy()
    pool_idx = {name: i for i, name in enumerate(pool_names)}

    # Collect per-query metrics for bootstrap
    per_query_recall = {k: [] for k in k_values}
    per_query_mrr = []
    n_queryable = 0

    for qi, query_name in enumerate(query_names):
        if query_name not in gt_deps:
            continue
        deps = gt_deps[query_name]
        dep_indices = [pool_idx[d] for d in deps if d in pool_idx]
        if not dep_indices:
            continue

        n_queryable += 1
        dep_set = set(dep_indices)
        n_deps = len(dep_set)

        sim_row = sim[qi]
        ranked_indices = np.argsort(-sim_row)

        for k in k_values:
            top_k = set(ranked_indices[:k].tolist())
            hits = len(dep_set & top_k)
            per_query_recall[k].append(hits / n_deps)

        for rank, idx in enumerate(ranked_indices, 1):
            if idx in dep_set:
                per_query_mrr.append(1.0 / rank)
                break
        else:
            per_query_mrr.append(0.0)

    # Bootstrap CIs
    results = {}
    for k in k_values:
        vals = per_query_recall[k]
        if vals:
            mean = float(np.mean(vals))
            ci_lo, ci_hi = bootstrap_ci(vals, n_bootstrap)
            results[f"recall@{k}"] = {
                "mean": mean,
                "ci_lo": ci_lo,
                "ci_hi": ci_hi,
                "n": len(vals),
            }

    if per_query_mrr:
        mean = float(np.mean(per_query_mrr))
        ci_lo, ci_hi = bootstrap_ci(per_query_mrr, n_bootstrap)
        results["mrr"] = {
            "mean": mean,
            "ci_lo": ci_lo,
            "ci_hi": ci_hi,
            "n": len(per_query_mrr),
        }

    results["n_queryable"] = n_queryable
    results["pool_size"] = len(pool_names)
    return results


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="H6 retrieval evaluation")
    parser.add_argument("--embeddings-dir", default="runs/retrieval_embeddings")
    parser.add_argument("--groundtruth", default="datasets/dependency_groundtruth_eval_to_train.json")
    parser.add_argument("--out", default="runs/h6_retrieval/results.json")
    parser.add_argument("--variants", default="A,B_small,B,C,flat")
    parser.add_argument("--mode", choices=["eval_to_train", "train_to_train"], default="eval_to_train")
    parser.add_argument("--k-values", default="1,5,10,20")
    args = parser.parse_args()

    embeddings_dir = Path(args.embeddings_dir)
    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)

    variants = [v.strip() for v in args.variants.split(",")]
    k_values = [int(k) for k in args.k_values.split(",")]

    # Load ground truth
    with open(args.groundtruth) as f:
        gt_data = json.load(f)

    if args.mode == "eval_to_train":
        gt_deps = gt_data["eval_queries"]
        pool_names = gt_data["train_pool"]
        query_split = "eval"
        pool_split = "train"
    else:
        # Mode 2: train → train (leave-one-out)
        # Need full dependency ground truth, not just eval→train
        full_gt_path = Path("datasets/dependency_groundtruth.json")
        if not full_gt_path.exists():
            print(f"ERROR: {full_gt_path} not found. Run build_dependency_groundtruth.py first.")
            return
        with open(full_gt_path) as f:
            full_gt = json.load(f)
        gt_deps = full_gt
        pool_names = gt_data["train_pool"]
        query_split = "train"
        pool_split = "train"

    print(f"Mode: {args.mode}")
    print(f"Query split: {query_split}, Pool split: {pool_split}")
    print(f"Queryable declarations: {len(gt_deps)}")
    print(f"Pool size: {len(pool_names)}")
    print()

    all_results = {}

    for variant in variants:
        print(f"{'='*60}")
        print(f"VARIANT: {variant}")
        print(f"{'='*60}")

        # Load embeddings
        query_emb, query_names = load_embeddings(embeddings_dir, variant, query_split)
        pool_emb, pool_names_loaded = load_embeddings(embeddings_dir, variant, pool_split)

        if query_emb is None or pool_emb is None:
            print(f"  Missing embeddings for {variant}, skipping.")
            all_results[variant] = {"error": "missing_embeddings"}
            continue

        # Verify names alignment with ground truth pool
        if set(pool_names_loaded) != set(pool_names):
            print(f"  WARNING: pool names in embeddings don't match ground truth pool")
            print(f"  Embeddings pool: {len(pool_names_loaded)} names")
            print(f"  GT pool: {len(pool_names)} names")
            # Use the embeddings' pool names (they're the actual pool)
            pool_names = pool_names_loaded

        # Evaluate with bootstrap CIs
        results = evaluate_with_ci(
            query_emb, query_names, pool_emb, pool_names,
            gt_deps, k_values, n_bootstrap=1000
        )

        all_results[variant] = results

        for k in k_values:
            r = results.get(f"recall@{k}", {})
            if r:
                print(f"  Recall@{k}: {r['mean']:.4f} [{r['ci_lo']:.4f}, {r['ci_hi']:.4f}] (n={r['n']})")
        mrr = results.get("mrr", {})
        if mrr:
            print(f"  MRR: {mrr['mean']:.4f} [{mrr['ci_lo']:.4f}, {mrr['ci_hi']:.4f}] (n={mrr['n']})")
        print()

    # Summary comparison
    print(f"{'='*60}")
    print("SUMMARY (Mode: " + args.mode + ")")
    print(f"{'='*60}")
    print(f"{'Variant':<12} {'Recall@10':>20} {'MRR':>20}")
    print("-" * 54)
    for variant in variants:
        r = all_results.get(variant, {})
        r10 = r.get("recall@10", {})
        mrr = r.get("mrr", {})
        if r10 and mrr:
            print(f"{variant:<12} {r10['mean']:.4f} [{r10['ci_lo']:.4f}, {r10['ci_hi']:.4f}]"
                  f"  {mrr['mean']:.4f} [{mrr['ci_lo']:.4f}, {mrr['ci_hi']:.4f}]")
        else:
            print(f"{variant:<12} {'N/A':>20} {'N/A':>20}")

    # Primary gate: A vs B_small on Recall@10
    a_r10 = all_results.get("A", {}).get("recall@10", {})
    bsmall_r10 = all_results.get("B_small", {}).get("recall@10", {})
    if a_r10 and bsmall_r10:
        diff = a_r10["mean"] - bsmall_r10["mean"]
        print()
        print(f"PRIMARY GATE: A vs B_small on Recall@10")
        print(f"  A:       {a_r10['mean']:.4f} [{a_r10['ci_lo']:.4f}, {a_r10['ci_hi']:.4f}]")
        print(f"  B_small: {bsmall_r10['mean']:.4f} [{bsmall_r10['ci_lo']:.4f}, {bsmall_r10['ci_hi']:.4f}]")
        print(f"  Diff:    {diff:+.4f}")
        if diff > 0.02 and a_r10["ci_lo"] > bsmall_r10["ci_hi"]:
            print("  VERDICT: H6 POSITIVE — IR beats B-small on retrieval by a meaningful margin")
        elif diff < -0.02 and a_r10["ci_hi"] < bsmall_r10["ci_lo"]:
            print("  VERDICT: H6 NEGATIVE — IR loses to B-small on retrieval")
        else:
            print("  VERDICT: INCONCLUSIVE — CIs overlap, no significant difference")

    # Save results (convert all numpy values to native Python types)
    def to_native(obj):
        if isinstance(obj, (np.float32, np.float64)):
            return float(obj)
        if isinstance(obj, (np.int32, np.int64)):
            return int(obj)
        if isinstance(obj, dict):
            return {k: to_native(v) for k, v in obj.items()}
        if isinstance(obj, list):
            return [to_native(v) for v in obj]
        return obj

    with open(out_path, "w") as f:
        json.dump({
            "mode": args.mode,
            "variants": to_native(all_results),
            "primary_gate": "A vs B_small on Recall@10",
            "n_bootstrap": 1000,
            "k_values": k_values,
        }, f, indent=2)
    print(f"\nResults saved → {out_path}")


if __name__ == "__main__":
    main()
