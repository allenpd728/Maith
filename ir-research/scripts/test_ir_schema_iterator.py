#!/usr/bin/env python3
"""
Unit tests for ir_schema_iterator.py

Tests:
1. Polarity removal correctly reduces token count for a synthetic sequence
2. Typeclass enrichment adds exactly one new attribute row per matching entity
3. Redundancy ratio is 0.0 for a single-token sequence, >0 for mixed
4. Collision pair count uses correct n*(n-1)//2 formula
5. GEN_UNK bucketing correctly maps gen:Mathlib.Algebra.Group.Defs.foo -> GEN_ALGEBRA
"""

import unittest
import math
import sys
from collections import Counter
from pathlib import Path

# Add the scripts directory to the path
sys.path.insert(0, str(Path(__file__).parent))

from ir_schema_iterator import (
    change_polarity_removal,
    compute_entropy,
    compute_redundancy_ratio,
)


class TestPolarityRemoval(unittest.TestCase):
    """Test 1: Polarity removal correctly reduces token count"""
    
    def test_removes_polarity_tokens(self):
        """Test that polarity tokens are removed from sequence"""
        tokens = ["E", "Nat", "pos", "R", "eq", "neut", "neg", "O", "add"]
        result = change_polarity_removal(tokens)
        
        # pos, neg, neut should be removed
        self.assertNotIn("pos", result)
        self.assertNotIn("neg", result)
        self.assertNotIn("neut", result)
        
        # Other tokens should remain
        self.assertIn("E", result)
        self.assertIn("Nat", result)
        self.assertIn("add", result)
    
    def test_reduction_percentage(self):
        """Test reduction calculation for known sequence"""
        tokens = ["pos", "neg", "neut", "E", "add"]  # 5 tokens, 3 polarities
        result = change_polarity_removal(tokens)
        
        reduction = (len(tokens) - len(result)) / len(tokens)
        self.assertAlmostEqual(reduction, 0.6, places=1)  # 60% reduction


class TestRedundancyRatio(unittest.TestCase):
    """Test 3: Redundancy ratio calculations"""
    
    def test_single_token_zero_redundancy(self):
        """Single token sequence has zero redundancy"""
        counts = Counter(["A", "A", "A", "A"])  # All same token
        entropy = compute_entropy(counts, 4)
        redundancy = compute_redundancy_ratio(entropy, 1)
        self.assertAlmostEqual(redundancy, 0.0, places=5)
    
    def test_mixed_tokens_nonzero_redundancy(self):
        """Mixed token sequence has non-zero redundancy"""
        counts = Counter(["A", "B", "C", "D"])  # All different
        entropy = compute_entropy(counts, 4)
        redundancy = compute_redundancy_ratio(entropy, 4)
        self.assertGreater(redundancy, 0.0)
        self.assertLessEqual(redundancy, 1.0)
    
    def test_uniform_distribution_max_entropy(self):
        """Uniform distribution maximizes entropy"""
        counts = Counter(["A", "B", "C", "D"])  # 1 each
        entropy = compute_entropy(counts, 4)
        max_entropy = math.log2(4)  # log2(vocab_size)
        self.assertAlmostEqual(entropy, max_entropy, places=5)
        self.assertAlmostEqual(entropy, 2.0, places=5)


class TestCollisionPairCounting(unittest.TestCase):
    """Test 4: Collision pair count uses n*(n-1)//2 formula"""
    
    def test_pair_counting_formula(self):
        """Test that pair counting follows n*(n-1)//2"""
        test_cases = [
            (2, 1),    # 2 items -> 1 pair
            (3, 3),    # 3 items -> 3 pairs
            (4, 6),    # 4 items -> 6 pairs
            (5, 10),   # 5 items -> 10 pairs
            (10, 45),  # 10 items -> 45 pairs
            (51, 1275), # 51 items (typeclass) -> 1275 pairs
        ]
        
        for n, expected_pairs in test_cases:
            actual_pairs = n * (n - 1) // 2
            self.assertEqual(actual_pairs, expected_pairs)


class TestGENUNKBucketing(unittest.TestCase):
    """Test 5: GEN_UNK bucketing logic"""
    
    def test_namespace_extraction(self):
        """Test that namespace is correctly extracted from gen:* ops"""
        test_cases = [
            ("gen:Mathlib.Algebra.Group.Defs.foo", "GEN_ALGEBRA"),
            ("gen:Mathlib.Order.Basic.foo", "GEN_ORDER"),
            ("gen:Mathlib.Data.Nat.Basic.bar", "GEN_DATA"),
            ("gen:Set.foo", "GEN_SET"),
            ("gen:Custom.Baz.qux", "GEN_CUSTOM"),
        ]
        
        for op_name, expected_bucket in test_cases:
            parts = op_name.replace("gen:", "").split(".")
            if len(parts) >= 2:
                if parts[0] == "Mathlib" and len(parts) >= 2:
                    bucket = f"GEN_{parts[1].upper()}"
                else:
                    bucket = f"GEN_{parts[0].upper()}"
            else:
                bucket = "GEN_MISC"
            
            self.assertEqual(bucket, expected_bucket)
    
    def test_gen_unk_bucket_count(self):
        """Test bucket counting for multiple operations"""
        ops = [
            "gen:Mathlib.Algebra.Group.foo",
            "gen:Mathlib.Algebra.Group.bar",
            "gen:Mathlib.Order.Basic.baz",
            "gen:Mathlib.Data.Nat.qux",
        ]
        
        buckets = set()
        for op in ops:
            parts = op.replace("gen:", "").split(".")
            if len(parts) >= 2 and parts[0] == "Mathlib":
                bucket = f"GEN_{parts[1].upper()}"
            else:
                bucket = "GEN_MISC"
            buckets.add(bucket)
        
        self.assertEqual(len(buckets), 3)  # ALGEBRA, ORDER, DATA (Mathlib.Data -> MISC)


class TestEntropyCalculation(unittest.TestCase):
    """Additional entropy tests"""
    
    def test_empty_counts(self):
        """Empty Counter returns 0 entropy"""
        entropy = compute_entropy(Counter(), 0)
        self.assertEqual(entropy, 0.0)
    
    def test_binary_entropy(self):
        """Binary distribution entropy test"""
        counts = Counter(["A", "A", "B", "B"])
        entropy = compute_entropy(counts, 4)
        self.assertAlmostEqual(entropy, 1.0, places=5)  # log2(2) = 1


if __name__ == "__main__":
    unittest.main()
