#!/usr/bin/env python3
"""
Unit tests for ir_coverage.py

Tests:
1. Polarity counter handles missing polarity field gracefully (defaults to neut)
2. Token frequency bucket logic is correct for boundary values (1, 2, 10, 11, 100, 101)
3. Attribute density calculation is correct for a synthetic 3-entity graph
4. Normalisation collision detection works on a synthetic collision pair
5. Operation arity check correctly flags a neg op with 2 inputs and passes one with 1
"""

import unittest
import sys
from pathlib import Path

# Add the scripts directory to the path
sys.path.insert(0, str(Path(__file__).parent))

from ir_coverage import (
    count_polarities,
    compute_token_frequency,
    compute_attribute_density,
    compute_normalisation_collisions,
    check_hard_fail_paths,
)


class TestPolarityCounter(unittest.TestCase):
    """Test 1: Polarity counter handles missing polarity field gracefully"""
    
    def test_missing_polarity_defaults_to_neut(self):
        """Missing polarity field should default to 'neut'"""
        entries = [
            {
                "graph": {
                    "entities": [
                        {"id": {"kind": "var", "name": "x"}, "polarity": "pos"},
                        {"id": {"kind": "var", "name": "y"}},  # Missing polarity
                        {"id": {"kind": "var", "name": "z"}, "polarity": "neg"},
                    ],
                    "attributes": [],
                    "relations": [],
                    "operations": [],
                }
            }
        ]
        result = count_polarities(entries)
        
        # entity polarity should have 1 pos, 1 missing (defaults to neut), 1 neg
        self.assertEqual(result["entities"]["pos"], 1)
        self.assertEqual(result["entities"]["neut"], 1)  # default for missing
        self.assertEqual(result["entities"]["neg"], 1)
    
    def test_empty_entries(self):
        """Empty entries should produce zeros"""
        entries = [{"graph": {"entities": [], "attributes": [], "relations": [], "operations": []}}]
        result = count_polarities(entries)
        self.assertEqual(result["overall"]["neut"], 0)
        self.assertEqual(result["overall"]["pos"], 0)
        self.assertEqual(result["overall"]["neg"], 0)


class TestTokenFrequencyBuckets(unittest.TestCase):
    """Test 2: Token frequency bucket logic is correct for boundary values"""
    
    def test_bucket_boundaries(self):
        """Test exact boundary values: 1, 2, 10, 11, 100, 101"""
        vocab = {"<PAD>": 0, "<BOS>": 1, "t1": 2, "t2": 3, "t3": 4, "t4": 5}
        
        # Create training entries with specific frequencies
        train_entries = [
            {"input_ids": [2]},  # t1 appears once -> bucket 1x
            {"input_ids": [3] * 2},  # t2 appears twice -> bucket 2_10x
            {"input_ids": [4] * 10},  # t3 appears 10 times -> bucket 2_10x
            {"input_ids": [5] * 11},  # t4 appears 11 times -> bucket 11_100x
        ]
        
        result = compute_token_frequency(vocab, train_entries)
        buckets = result["frequency_buckets"]
        
        # Boundary checks
        self.assertEqual(buckets["1x"], 1)  # t1 once
        self.assertEqual(buckets["2_10x"], 2)  # t2 (2x) + t3 (10x)
        self.assertEqual(buckets["11_100x"], 1)  # t4 11 times
        self.assertEqual(buckets["100plus_x"], 0)
    
    def test_100_versus_101(self):
        """Test 100 vs 101 boundary"""
        vocab = {"<PAD>": 0, "t1": 1, "t2": 2}
        
        # t1: exactly 100 times (bucket 11_100x)
        # t2: 101 times (bucket 100plus_x)
        train_entries = [
            {"input_ids": [1] * 100},
            {"input_ids": [2] * 101},
        ]
        
        result = compute_token_frequency(vocab, train_entries)
        buckets = result["frequency_buckets"]
        
        self.assertEqual(buckets["11_100x"], 1)  # 100
        self.assertEqual(buckets["100plus_x"], 1)  # 101


class TestAttributeDensity(unittest.TestCase):
    """Test 3: Attribute density calculation is correct for synthetic graph"""
    
    def test_synthetic_3_entity_graph(self):
        """Test with 3 entities: 1 with attr, 1 with 2 attrs, 1 with 0"""
        entries = [
            {
                "graph": {
                    "entities": [
                        {"id": {"kind": "term", "name": "x"}},
                        {"id": {"kind": "term", "name": "y"}},
                        {"id": {"kind": "term", "name": "z"}},
                    ],
                    "attributes": [
                        # Entity 0 has 1 attribute
                        {"target": {"kind": "term", "index": 0}, "key": "sort", "value": "Nat"},
                        # Entity 1 has 2 attributes
                        {"target": {"kind": "term", "index": 1}, "key": "sort", "value": "Int"},
                        {"target": {"kind": "term", "index": 1}, "key": "type", "value": "Number"},
                    ],
                    "relations": [],
                    "operations": [],
                }
            }
        ]
        
        result = compute_attribute_density(entries)
        
        self.assertEqual(result["total_entities"], 3)
        self.assertEqual(result["entities_with_zero_attributes"], 1)  # Entity 2
        self.assertEqual(result["pct_zero_attributes"], 33.33)
        
        # Distribution: 1 zero, 1 one, 1 two+
        dist = result["per_entity_count_distribution"]
        self.assertEqual(dist.get("0", 0), 1)
        self.assertEqual(dist.get("1", 0), 1)
        self.assertEqual(dist.get("2", 0), 1)
        self.assertEqual(dist.get("3plus", 0), 0)
    
    def test_all_entities_have_attributes(self):
        """All entities with attributes should have zero=0"""
        entries = [
            {
                "graph": {
                    "entities": [
                        {"id": {"kind": "term", "name": "x"}},
                        {"id": {"kind": "term", "name": "y"}},
                    ],
                    "attributes": [
                        {"target": {"kind": "term", "index": 0}, "key": "a", "value": "1"},
                        {"target": {"kind": "term", "index": 1}, "key": "b", "value": "2"},
                    ],
                    "relations": [],
                    "operations": [],
                }
            }
        ]
        
        result = compute_attribute_density(entries)
        self.assertEqual(result["entities_with_zero_attributes"], 0)
        self.assertEqual(result["pct_zero_attributes"], 0.0)


class TestNormalisationCollision(unittest.TestCase):
    """Test 4: Normalisation collision detection works on synthetic collision pair"""
    
    def test_collision_pair_detected(self):
        """Declarations with same tokens but different names should be detected"""
        # Create entries with identical token sequences
        tokens_a = ["GRAPH_BEGIN", "E", "Nat", "GRAPH_END"]
        tokens_b = ["GRAPH_BEGIN", "E", "Nat", "GRAPH_END"]  # Same tokens
        tokens_c = ["GRAPH_BEGIN", "E", "Int", "GRAPH_END"]  # Different
        
        entries = [
            {"name": "Decl.A", "tokens": tokens_a},
            {"name": "Decl.B", "tokens": tokens_b},  # Collision with A
            {"name": "Decl.C", "tokens": tokens_c},
        ]
        
        result = compute_normalisation_collisions(entries)
        
        # Should have 1 collision pair (A, B)
        self.assertEqual(result["collision_pairs"], 1)
    
    def test_no_collision_for_different_tokens(self):
        """Declarations with different tokens should not be a collision"""
        entries = [
            {"name": "Decl.A", "tokens": ["A", "B", "C"]},
            {"name": "Decl.B", "tokens": ["X", "Y", "Z"]},
        ]
        
        result = compute_normalisation_collisions(entries)
        self.assertEqual(result["collision_pairs"], 0)
    
    def test_caseson_recOn_counting(self):
        """Declarations ending in .casesOn and .recOn should be counted"""
        entries = [
            {"name": "Foo.casesOn", "tokens": ["A"]},
            {"name": "Bar.recOn", "tokens": ["B"]},
            {"name": "Baz.rec", "tokens": ["C"]},  # Not casesOn/recOn
            {"name": "Qux._flat_ctor", "tokens": ["D"]},
            {"name": "Quux._flat", "tokens": ["E"]},
        ]
        
        result = compute_normalisation_collisions(entries)
        
        self.assertEqual(result["casesOn_count"], 1)
        self.assertEqual(result["recOn_count"], 1)
        self.assertEqual(result["flat_ctor_count"], 2)  # _flat_ctor + _flat


class TestOperationArity(unittest.TestCase):
    """Test 5: Operation arity check correctly flags violations"""
    
    def test_neg_op_with_0_inputs_flags_violation(self):
        """neg with 0 inputs should be flagged (expected arity is 1)"""
        entries = [
            {
                "graph": {
                    "entities": [],
                    "attributes": [],
                    "relations": [],
                    "operations": [
                        {"op": "neg", "inputs": []},  # 0 inputs - fewer than expected
                    ],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        self.assertEqual(result["operation_arity_violations"], 1)
    
    def test_neg_op_with_1_input_no_violation(self):
        """neg with 1 input should NOT be flagged"""
        entries = [
            {
                "graph": {
                    "entities": [],
                    "attributes": [],
                    "relations": [],
                    "operations": [
                        {"op": "neg", "inputs": [{"kind": "x"}]},  # 1 input - correct
                    ],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        self.assertEqual(result["operation_arity_violations"], 0)
    
    def test_add_op_with_2_inputs_no_violation(self):
        """add with 2 inputs should NOT be flagged (expected arity is 2)"""
        entries = [
            {
                "graph": {
                    "entities": [],
                    "attributes": [],
                    "relations": [],
                    "operations": [
                        {"op": "add", "inputs": [{"kind": "x"}, {"kind": "y"}]},  # 2 inputs - correct
                    ],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        self.assertEqual(result["operation_arity_violations"], 0)
    
    def test_add_op_with_1_input_flags_violation(self):
        """add with 1 input should be flagged (expected arity is 2)"""
        entries = [
            {
                "graph": {
                    "entities": [],
                    "attributes": [],
                    "relations": [],
                    "operations": [
                        {"op": "add", "inputs": [{"kind": "x"}]},  # 1 input - wrong
                    ],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        self.assertEqual(result["operation_arity_violations"], 1)
    
    def test_non_checked_ops_no_violation(self):
        """Operations not in {neg, add, sub, mul, div, pow} should not be checked"""
        entries = [
            {
                "graph": {
                    "entities": [],
                    "attributes": [],
                    "relations": [],
                    "operations": [
                        {"op": "gen:CustomOp", "inputs": [{"kind": "x"}]},  # Not checked
                        {"op": "mul", "inputs": [{"kind": "x"}]},  # Should be checked
                    ],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        # Only mul with 1 input should be flagged (add, sub, mul, div, pow expect 2)
        self.assertEqual(result["operation_arity_violations"], 1)


class TestHardFailPaths(unittest.TestCase):
    """Additional tests for hard fail path checking"""
    
    def test_ir_construction_failed_from_stats(self):
        """irConstructionFailed should come from stats.json"""
        entries = [{"graph": {"entities": [], "attributes": [], "relations": [], "operations": []}}]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 42}})
        self.assertEqual(result["ir_construction_failed"], 42)
    
    def test_mvar_count(self):
        """var entities with ? prefix should be counted as mvar"""
        entries = [
            {
                "graph": {
                    "entities": [
                        {"id": {"kind": "var", "name": "?mvar1"}},
                        {"id": {"kind": "var", "name": "regular_var"}},
                        {"id": {"kind": "var", "name": "?mvar2"}},
                    ],
                    "attributes": [],
                    "relations": [],
                    "operations": [],
                }
            }
        ]
        
        result = check_hard_fail_paths(entries, {"failureStats": {"irConstructionFailed": 0}})
        self.assertEqual(result["mvar_entities"], 2)


if __name__ == "__main__":
    unittest.main()
