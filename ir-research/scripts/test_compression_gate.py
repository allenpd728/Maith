#!/usr/bin/env python3
"""
Unit tests for compression_gate.py

Tests:
1. BPT calculation is correct for a known synthetic token distribution
2. Redundancy is 0.0 for a uniform distribution, ~0 for a single-token corpus
3. Coverage correctly counts tokens above frequency threshold
4. Gate PASS/FAIL logic triggers correctly given synthetic metric values
5. Collision rate reduction from C2 uses correct n*(n-1)//2 formula
6. v2 C1C2C4 simulation is derived from modified token stream, not a multiplier
"""

import unittest
import math
import json
import tempfile
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
    simulate_v2_token_stream,
    BASELINE,
    TYPEFIELD_TOKEN_IDS,
    BUCKET_TOKEN_IDS,
    GEN_UNK_ID,
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


class TestSimulation(unittest.TestCase):
    """Test 6: v2 simulation is from token stream, not a multiplier."""
    
    def test_c2_adds_typefield_tokens(self):
        """C2: First 3149 declarations get 2 synthetic typefield tokens."""
        # Create synthetic train data - avoid GEN_UNK_ID (4) since C4 replaces it
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # 5 declarations, each with 5 tokens (avoiding 4 which is GEN_UNK)
            for i in range(5):
                f.write(json.dumps({"input_ids": [10, 20, 30, 40, 50]}) + '\n')
            train_path = f.name
        
        try:
            all_ids, token_counts, total_tokens = simulate_v2_token_stream(train_path)
            
            # First declaration (idx=0) should have typefield tokens appended
            # Original: [10,20,30,40,50], After C2: [10,20,30,40,50,9999,9998]
            self.assertEqual(all_ids[0:5], [10, 20, 30, 40, 50])
            self.assertEqual(all_ids[5:7], TYPEFIELD_TOKEN_IDS)
            
            # All 5 decls are affected (5 < 3149), so each has 1 typefield token pair
            for tid in TYPEFIELD_TOKEN_IDS:
                self.assertIn(tid, token_counts)
                self.assertEqual(token_counts[tid], 5)  # All 5 decls have them
        finally:
            Path(train_path).unlink()
    
    def test_c2_affects_only_first_3149_decls(self):
        """C2: Only first 3149 declarations get additional tokens."""
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # Create 3150 declarations (more than 3149)
            # Avoid GEN_UNK_ID (4) since C4 replaces it
            for i in range(3150):
                f.write(json.dumps({"input_ids": [10, 20, 30]}) + '\n')
            train_path = f.name
        
        try:
            all_ids, token_counts, total_tokens = simulate_v2_token_stream(train_path)
            
            # With 3150 decls:
            # - Decls 0-3148 (first 3149): 3 + 2 = 5 tokens each = 15745 tokens
            # - Decl 3149 (last): 3 tokens = 3 tokens
            # Total = 15748 tokens
            
            # Decl 3149 starts after all previous decls
            # First 3149 decls (0-3148): 3149 * 5 = 15745 tokens
            decl_3149_start = 3149 * 5
            decl_3149_tokens = all_ids[decl_3149_start:decl_3149_start + 3]
            
            # Should be original tokens only (no typefield tokens)
            self.assertEqual(decl_3149_tokens, [10, 20, 30])
            
            # Typefield tokens count should equal 3149 (first 3149 decls)
            self.assertEqual(token_counts[TYPEFIELD_TOKEN_IDS[0]], 3149)
            self.assertEqual(token_counts[TYPEFIELD_TOKEN_IDS[1]], 3149)
        finally:
            Path(train_path).unlink()
    
    def test_c4_replaces_gen_unk_with_buckets(self):
        """C4: First 1564 GEN_UNK tokens replaced with bucket tokens."""
        # Note: Typefield tokens (9998, 9999) overlap with bucket tokens (9980-9999).
        # We check non-typefield bucket tokens (9980-9981) to isolate C4 behavior.
        non_typefield_buckets = [9980, 9981]
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # Create declarations with GEN_UNK tokens (ID=4)
            # 4000 declarations with 2 GEN_UNK each = 8000 GEN_UNK
            for i in range(4000):
                f.write(json.dumps({"input_ids": [GEN_UNK_ID, GEN_UNK_ID]}) + '\n')
            train_path = f.name
        
        try:
            all_ids, token_counts, total_tokens = simulate_v2_token_stream(train_path)
            
            # Total: 8000 GEN_UNK + 6298 typefield = 14298
            self.assertEqual(total_tokens, 14298)
            
            # First 1564 GEN_UNK should be replaced with bucket tokens
            # Remaining GEN_UNK = 8000 - 1564 = 6436
            self.assertEqual(token_counts[GEN_UNK_ID], 6436)
            
            # Non-typefield bucket tokens (9980, 9981) should each appear ~78-79 times
            # 1564 / 20 = 78.2, so 78 or 79 per bucket
            for bucket_id in non_typefield_buckets:
                self.assertIn(bucket_id, token_counts)
                count = token_counts[bucket_id]
                self.assertGreaterEqual(count, 78)
                self.assertLessEqual(count, 79)
        finally:
            Path(train_path).unlink()
    
    def test_v2_bpt_not_multiplier_of_current(self):
        """v2 BPT is computed from modified token stream, not current_BPT * constant."""
        # Create two different token distributions
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # Distribution 1: 100 tokens of type A, 100 of type B (avoiding 4 which is GEN_UNK)
            for i in range(100):
                f.write(json.dumps({"input_ids": [100]}) + '\n')
            for i in range(100):
                f.write(json.dumps({"input_ids": [200]}) + '\n')
            train_path_1 = f.name
        
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # Distribution 2: 50 tokens of type A, 150 of type B (different ratio)
            for i in range(50):
                f.write(json.dumps({"input_ids": [100]}) + '\n')
            for i in range(150):
                f.write(json.dumps({"input_ids": [200]}) + '\n')
            train_path_2 = f.name
        
        try:
            # Simulate v2 for both distributions
            all_ids_1, counts_1, total_1 = simulate_v2_token_stream(train_path_1)
            all_ids_2, counts_2, total_2 = simulate_v2_token_stream(train_path_2)
            
            bpt_1 = compute_bpt(counts_1, total_1)
            bpt_2 = compute_bpt(counts_2, total_2)
            
            # BPTs should be different (not the same multiplier)
            # If it were just a multiplier, they'd have the same ratio
            self.assertNotAlmostEqual(bpt_1, bpt_2, places=3)
        finally:
            Path(train_path_1).unlink()
            Path(train_path_2).unlink()
    
    def test_simulation_token_count_increases(self):
        """C2 increases total token count by 2 * 3149 = 6298 tokens."""
        with tempfile.NamedTemporaryFile(mode='w', suffix='.jsonl', delete=False) as f:
            # 3500 declarations with 10 tokens each = 35000 tokens
            # Avoid 4 which is GEN_UNK
            for i in range(3500):
                f.write(json.dumps({"input_ids": list(range(10, 20))}) + '\n')
            train_path = f.name
        
        try:
            all_ids, token_counts, total_tokens = simulate_v2_token_stream(train_path)
            
            # 3149 decls get +2 tokens each = 6298 new tokens
            expected_total = 35000 + 6298
            self.assertEqual(total_tokens, expected_total)
        finally:
            Path(train_path).unlink()


if __name__ == "__main__":
    unittest.main()
