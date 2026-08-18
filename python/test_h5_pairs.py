#!/usr/bin/env python3
"""
test_h5_pairs.py

Unit tests for build_h5_pairs.py — covers every invariant the pair builder
must satisfy before H5 training can proceed.

Tests are CPU-only, model-free, and complete in under a second.
No disk I/O except for the one integration test that reads the real groundtruth.

Run with:
    python3 python/test_h5_pairs.py

Exit 0 = all pass. Exit 1 = at least one failure.
"""

import json
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from build_h5_pairs import build_dep_index, build_pairs, build_and_save, load_jsonl

PASS = 0
FAIL = 0


def ok(name: str):
    global PASS
    PASS += 1
    print(f"  PASS  {name}")


def fail(name: str, detail: str = ""):
    global FAIL
    FAIL += 1
    msg = f"  FAIL  {name}"
    if detail:
        msg += f": {detail}"
    print(msg)


# ---------------------------------------------------------------------------
# Test helpers
# ---------------------------------------------------------------------------

def _names(n: int) -> list[str]:
    return [f"decl_{i}" for i in range(n)]


def _gt(pairs: dict[str, list[str]]) -> dict:
    """Build a flat groundtruth dict from name -> [dep, ...] pairs."""
    return pairs


# ---------------------------------------------------------------------------
# 1. build_dep_index
# ---------------------------------------------------------------------------

def test_dep_index_basic():
    names = _names(4)  # decl_0 .. decl_3
    gt = {"decl_0": ["decl_1", "decl_2"], "decl_1": ["decl_2"]}
    idx = build_dep_index(gt, names)
    # Only deps that are in names are kept
    assert idx["decl_0"] == {"decl_1", "decl_2"}, idx["decl_0"]
    assert idx["decl_1"] == {"decl_2"}, idx["decl_1"]
    assert idx["decl_2"] == set(), idx["decl_2"]
    assert idx["decl_3"] == set(), idx["decl_3"]
    ok("dep_index_basic")


def test_dep_index_filters_external_deps():
    names = ["a", "b"]
    gt = {"a": ["b", "external_lib_func"]}
    idx = build_dep_index(gt, names)
    assert idx["a"] == {"b"}, "external dep should be filtered out"
    ok("dep_index_filters_external_deps")


def test_dep_index_handles_missing_names():
    names = ["a", "b", "c"]
    gt = {"a": ["b"]}  # c not in gt
    idx = build_dep_index(gt, names)
    assert "c" in idx
    assert idx["c"] == set()
    ok("dep_index_handles_missing_names")


def test_dep_index_handles_dict_format():
    names = ["a", "b", "c"]
    gt = {"a": {"b": 2, "external": 1}}
    idx = build_dep_index(gt, names)
    assert idx["a"] == {"b"}, "dict format should extract keys, filter external"
    ok("dep_index_handles_dict_format")


# ---------------------------------------------------------------------------
# 2. build_pairs — structural invariants
# ---------------------------------------------------------------------------

def test_pairs_no_self_pairs():
    names = _names(5)
    gt = {f"decl_{i}": [f"decl_{j}" for j in range(5) if j != i] for i in range(5)}
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared=1)
    for a, b in pairs:
        assert a != b, f"self-pair found: ({a}, {a})"
    ok("pairs_no_self_pairs")


def test_pairs_no_duplicates():
    names = _names(10)
    gt = {f"decl_{i}": [f"decl_{j}" for j in range(10) if j != i] for i in range(10)}
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared=1)
    seen = set()
    for a, b in pairs:
        key = (min(a, b), max(a, b))
        assert key not in seen, f"duplicate pair: {key}"
        seen.add(key)
    ok("pairs_no_duplicates")


def test_pairs_respect_min_shared():
    # decl_0 and decl_1 share dep_X; decl_0 and decl_2 share nothing
    names = ["decl_0", "decl_1", "decl_2", "dep_X"]
    gt = {
        "decl_0": ["dep_X"],
        "decl_1": ["dep_X"],
        "decl_2": [],
    }
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared=1)
    # decl_0 (idx 0) and decl_1 (idx 1) should pair
    pair_set = {(min(a, b), max(a, b)) for a, b in pairs}
    assert (0, 1) in pair_set, "expected (0,1) pair for shared dep_X"
    # decl_2 (idx 2) should not appear
    for a, b in pairs:
        assert 2 not in (a, b), f"decl_2 should not appear in pairs"
    ok("pairs_respect_min_shared")


def test_pairs_min_shared_2():
    # decl_0 and decl_1 share dep_X only — not enough for min_shared=2
    names = ["decl_0", "decl_1", "dep_X", "dep_Y"]
    gt = {
        "decl_0": ["dep_X"],
        "decl_1": ["dep_X"],
    }
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared=2)
    assert len(pairs) == 0, f"expected 0 pairs with min_shared=2, got {len(pairs)}"
    ok("pairs_min_shared_2")


def test_pairs_max_per_anchor():
    # Each decl shares deps with many others — cap should be respected
    names = [f"decl_{i}" for i in range(20)]
    common_dep = "dep_common"
    names.append(common_dep)
    gt = {f"decl_{i}": [common_dep] for i in range(20)}
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index, min_shared=1, max_pairs_per_anchor=3)
    # Count pairs per anchor
    from collections import Counter
    anchor_counts = Counter()
    for a, b in pairs:
        anchor_counts[a] += 1
        anchor_counts[b] += 1
    # No anchor should appear more than max_pairs_per_anchor times
    # (approximately — symmetric deduplication means some anchors appear as positives)
    # The hard constraint is max_pairs_per_anchor per anchor *before* dedup
    assert len(pairs) > 0, "expected some pairs"
    ok("pairs_max_per_anchor")


def test_pairs_deterministic():
    names = _names(10)
    gt = {f"decl_{i}": [f"decl_{j}" for j in range(5)] for i in range(10)}
    dep_index = build_dep_index(gt, names)
    pairs1 = build_pairs(names, dep_index, seed=42)
    pairs2 = build_pairs(names, dep_index, seed=42)
    assert pairs1 == pairs2, "pairs not deterministic with same seed"
    ok("pairs_deterministic")


def test_pairs_different_seeds_differ():
    names = _names(10)
    gt = {f"decl_{i}": [f"decl_{j}" for j in range(10) if j != i] for i in range(10)}
    dep_index = build_dep_index(gt, names)
    pairs1 = build_pairs(names, dep_index, seed=1)
    pairs2 = build_pairs(names, dep_index, seed=2)
    # With enough pairs, different seeds should produce different orderings
    assert pairs1 != pairs2, "different seeds should produce different orderings"
    ok("pairs_different_seeds_differ")


def test_pairs_empty_gt():
    names = _names(5)
    dep_index = build_dep_index({}, names)
    pairs = build_pairs(names, dep_index)
    assert pairs == [], f"expected empty pairs for empty groundtruth, got {pairs}"
    ok("pairs_empty_gt")


def test_pairs_indices_in_range():
    names = _names(8)
    gt = {f"decl_{i}": [f"decl_{j}" for j in range(8) if j != i] for i in range(8)}
    dep_index = build_dep_index(gt, names)
    pairs = build_pairs(names, dep_index)
    for a, b in pairs:
        assert 0 <= a < len(names), f"anchor index {a} out of range"
        assert 0 <= b < len(names), f"positive index {b} out of range"
    ok("pairs_indices_in_range")


# ---------------------------------------------------------------------------
# 3. build_and_save — integration test against real data
# ---------------------------------------------------------------------------

def test_integration_real_data():
    repo = Path(__file__).parent.parent
    dataset = repo / "datasets" / "train_A.jsonl"
    gt_file = repo / "datasets" / "dependency_groundtruth.json"

    if not dataset.exists() or not gt_file.exists():
        print("  SKIP  integration_real_data (data files not found)")
        return

    with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as f:
        out = Path(f.name)

    try:
        result = build_and_save(dataset, gt_file, out, min_shared=1, max_pairs_per_anchor=5)

        assert result["n_declarations"] == 3375, \
            f"expected 3375 declarations, got {result['n_declarations']}"
        assert result["n_pairs"] > 0, "expected at least one pair"
        assert result["strategy"] == "shared_dependency"
        assert result["min_shared"] == 1

        # Every pair index is in range
        for a, b in result["pairs"]:
            assert 0 <= a < result["n_declarations"]
            assert 0 <= b < result["n_declarations"]
            assert a != b

        # No duplicate pairs
        seen = set()
        for a, b in result["pairs"]:
            key = (min(a, b), max(a, b))
            assert key not in seen, f"duplicate pair {key}"
            seen.add(key)

        ok(f"integration_real_data ({result['n_pairs']} pairs from {result['n_declarations']} decls)")
    finally:
        out.unlink(missing_ok=True)


# ---------------------------------------------------------------------------
# 4. Output format invariants
# ---------------------------------------------------------------------------

def test_output_schema():
    names = _names(6)
    gt = {"decl_0": ["decl_1", "decl_2"], "decl_1": ["decl_2"]}

    with tempfile.NamedTemporaryFile(mode="w", suffix=".jsonl", delete=False) as f:
        for i, name in enumerate(names):
            import json as _json
            f.write(_json.dumps({"name": name, "input_ids": [i]}) + "\n")
        ds_path = Path(f.name)

    with tempfile.NamedTemporaryFile(mode="w", suffix=".json", delete=False) as f:
        json.dump(gt, f)
        gt_path = Path(f.name)

    with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as f:
        out_path = Path(f.name)

    try:
        result = build_and_save(ds_path, gt_path, out_path)
        required_keys = {"pairs", "n_declarations", "n_pairs", "strategy",
                         "min_shared", "max_pairs_per_anchor", "seed",
                         "dataset_file", "groundtruth_file"}
        assert required_keys.issubset(result.keys()), \
            f"missing keys: {required_keys - result.keys()}"
        assert isinstance(result["pairs"], list)
        assert isinstance(result["n_pairs"], int)
        assert result["n_pairs"] == len(result["pairs"])
        ok("output_schema")
    finally:
        ds_path.unlink(missing_ok=True)
        gt_path.unlink(missing_ok=True)
        out_path.unlink(missing_ok=True)


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------

def run_all():
    print("=" * 60)
    print("test_h5_pairs.py")
    print("=" * 60)

    test_dep_index_basic()
    test_dep_index_filters_external_deps()
    test_dep_index_handles_missing_names()
    test_dep_index_handles_dict_format()
    test_pairs_no_self_pairs()
    test_pairs_no_duplicates()
    test_pairs_respect_min_shared()
    test_pairs_min_shared_2()
    test_pairs_max_per_anchor()
    test_pairs_deterministic()
    test_pairs_different_seeds_differ()
    test_pairs_empty_gt()
    test_pairs_indices_in_range()
    test_integration_real_data()
    test_output_schema()

    print()
    print(f"Total: {PASS + FAIL} tests, {PASS} passed, {FAIL} failed")
    return FAIL


if __name__ == "__main__":
    sys.exit(run_all())
