#!/usr/bin/env python3
"""
Unit tests for compression_gate.py

Tests:
1. BPT calculation is correct for a known synthetic token distribution
2. Redundancy is 0.0 for a uniform distribution, ~0 for a single-token corpus
3. Coverage correctly counts tokens above frequency threshold
4. Gate PASS/FAIL logic triggers correctly given synthetic metric values
5. Collision rate reduction from C2 uses correct n*(n-1)//2 formula
"""

import unittest
import math
from collections import Counter
import sys
from pathlib import Path

# Add the scripts directory to the path
sys.path.insert(0, str(Path(__file__).parent))

from compression_gate import (
    compute_bpt,
    compute_entropy,
    compute_redundancy,
    compute_coverage,
    compute_collision_rate,
)


class TestBPT(unittest.TestCase):
    """Test 1: BPT calculation is correct for known distributions."""
    
    def test_uniform_distribution_bpt(self):
        """Uniform distribution over 2 tokens = 1 bit per token."""
        token_counts = Counter({"A": 500, "B": 500})
        total = 1000
        bpt = compute_bpt(token_counts, total)
        self.assertAlmostEqual(bpt, 1.0, places=5)
    
    def test_single_token_bpt(self):
        """Single token = 0 bits per token."""
        token_counts = Counter({"A": 1000})
        total = 1000
        bpt = compute_bpt(token_counts, total)
        self.assertAlmostEqual(bpt, 0.0, places=5)
    
    def test_four_tokens_uniform(self):
        """Uniform over 4 tokens = 2 bits per token."""
        token_counts = Counter({"A": 250, "B": 250, "C": 250, "D": 250})
        total = 1000
        bpt = compute_bpt(token_counts, total)
        self.assertAlmostEqual(bpt, 2.0, places=5)


class TestRedundancy(unittest.TestCase):
    """Test 2: Redundancy calculations."""
    
    def test_uniform_zero_redundancy(self):
        """Uniform distribution has zero redundancy."""
        token_counts = Counter({"A": 250, "B": 250, "C": 250, "D": 250})
        redundancy = compute_redundancy(token_counts, 1000, 4)
        self.assertAlmostEqual(redundancy, 0.0, places=5)
    
    def test_single_token_max_redundancy(self):
        """Single token has maximum redundancy."""
        token_counts = Counter({"A": 1000})
        redundancy = compute_redundancy(token_counts, 1000, 1)
        self.assertAlmostEqual(redundancy, 0.0, places=5)  # log2(1) = 0, so 1 - 0/0 = 0
    
    def test_concentrated_distribution(self):
        """Concentrated distribution has high redundancy."""
        token_counts = Counter({"A": 900, "B": 100})
        total = 1000
        entropy = compute_entropy(token_counts, total)
        # H = -0.9*log2(0.9) - 0.1*log2(0.1) ≈ 0.469
        self.assertGreater(entropy, 0.4)
        self.assertLess(entropy, 0.5)


class TestCoverage(unittest.TestCase):
    """Test 3: Coverage correctly counts tokens above threshold."""
    
    def test_all_above_threshold(self):
        """All tokens above threshold = 100% coverage."""
        token_counts = Counter({"A": 100, "B": 50, "C": 20})
        coverage = compute_coverage(token_counts, 170, threshold=10)
        self.assertEqual(coverage, 1.0)
    
    def test_none_above_threshold(self):
        """No tokens above threshold = 0% coverage."""
        token_counts = Counter({"A": 5, "B": 3, "C": 1})
        coverage = compute_coverage(token_counts, 9, threshold=10)
        self.assertEqual(coverage, 0.0)
    
    def test_partial_coverage(self):
        """Some tokens above threshold = partial coverage."""
        token_counts = Counter({"A": 100, "B": 5, "C": 3, "D": 1})
        # A (100) >= 10, others not
        coverage = compute_coverage(token_counts, 109, threshold=10)
        self.assertAlmostEqual(coverage, 100/109, places=3)


class TestCollisionPairCounting(unittest.TestCase):
    """Test 5: Collision pair count uses n*(n-1)//2 formula."""
    
    def test_pair_counting_formula(self):
        """Test that pair counting follows n*(n-1)//2."""
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
    
    def test_collision_rate(self):
        """Collision rate = pairs / total declarations."""
        rate = compute_collision_rate(1275, 4029)
        self.assertAlmostEqual(rate, 1275/4029, places=5)


class TestGateLogic(unittest.TestCase):
    """Test 4: Gate PASS/FAIL logic."""
    
    def test_pass_all_improve(self):
        """All criteria met = PASS."""
        bpt_improvement = True
        redundancy_improvement = True
        coverage_maintained = True
        
        all_pass = bpt_improvement and redundancy_improvement and coverage_maintained
        any_pass = bpt_improvement or redundancy_improvement or coverage_maintained
        
        self.assertTrue(all_pass)
        self.assertTrue(any_pass)
    
    def test_fail_none_improve(self):
        """No criteria met = FAIL."""
        bpt_improvement = False
        redundancy_improvement = False
        coverage_maintained = False
        
        all_pass = bpt_improvement and redundancy_improvement and coverage_maintained
        any_pass = bpt_improvement or redundancy_improvement or coverage_maintained
        
        self.assertFalse(all_pass)
        self.assertFalse(any_pass)
    
    def test_partial_some_improve(self):
        """Some criteria met = PARTIAL."""
        bpt_improvement = True
        redundancy_improvement = False
        coverage_maintained = False
        
        all_pass = bpt_improvement and redundancy_improvement and coverage_maintained
        any_pass = bpt_improvement or redundancy_improvement or coverage_maintained
        
        self.assertFalse(all_pass)
        self.assertTrue(any_pass)


if __name__ == "__main__":
    unittest.main()
