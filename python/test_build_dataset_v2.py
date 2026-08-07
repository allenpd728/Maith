#!/usr/bin/env python3
"""
test_build_dataset_v2.py

Tests for v2 IR token handling in build_dataset.py:
1. typeclass_name is in the structural vocab with a stable low ID
2. All 11 GEN_* bucket tokens are in the structural vocab
3. gen:* tokens are dropped from vocab regardless of frequency
4. typeclass_name:Group token (freq-driven) is added to vocab if it appears
5. encode_ir correctly encodes GEN_ALGEBRA to its vocab ID
6. encode_ir falls back to <UNK> for unknown typeclass_name:RareClass
"""

import sys
from pathlib import Path
import unittest

# Add python directory to path
sys.path.insert(0, str(Path(__file__).parent))

from build_dataset import build_ir_vocab, encode_ir


class TestStructuralVocab(unittest.TestCase):
    """Test that v2 structural tokens are in the structural vocab."""

    def test_typeclass_name_in_structural(self):
        """typeclass_name is in the structural vocab with a stable low ID."""
        examples = [{"tokens": ["typeclass_name"]}]
        vocab = build_ir_vocab(examples)
        assert "typeclass_name" in vocab, "typeclass_name should be in vocab"
        # Should be early in vocab (after PAD, BOS, EOS, UNK, GEN_UNK, GRAPH_BEGIN, GRAPH_END, E, A, R, O, pos, neg, neut, eq, add, sub, mul, div, le, ge, lt, gt, pow, typeclass, sort, literal)
        assert vocab["typeclass_name"] < 50, f"typeclass_name ID {vocab['typeclass_name']} should be < 50"

    def test_gen_bucket_tokens_in_structural(self):
        """All 11 GEN_* bucket tokens are in the structural vocab."""
        gen_buckets = [
            "GEN_MATHLIB", "GEN_ALGEBRA", "GEN_ORDER", "GEN_TOPOLOGY",
            "GEN_ANALYSIS", "GEN_LOGIC", "GEN_DATA", "GEN_LEAN",
            "GEN_INIT", "GEN_STD", "GEN_UNKNOWN"
        ]
        examples = [{"tokens": gen_buckets}]
        vocab = build_ir_vocab(examples)
        
        for bucket in gen_buckets:
            assert bucket in vocab, f"{bucket} should be in vocab"
            assert vocab[bucket] < 50, f"{bucket} ID {vocab[bucket]} should be < 50"

    def test_gen_bucket_tokens_have_distinct_ids(self):
        """GEN_* bucket tokens have distinct stable IDs."""
        gen_buckets = [
            "GEN_MATHLIB", "GEN_ALGEBRA", "GEN_ORDER", "GEN_TOPOLOGY",
            "GEN_ANALYSIS", "GEN_LOGIC", "GEN_DATA", "GEN_LEAN",
            "GEN_INIT", "GEN_STD", "GEN_UNKNOWN"
        ]
        examples = [{"tokens": gen_buckets}]
        vocab = build_ir_vocab(examples)
        
        ids = [vocab[bucket] for bucket in gen_buckets]
        assert len(ids) == len(set(ids)), "GEN_* tokens should have distinct IDs"


class TestGenTokenDropping(unittest.TestCase):
    """Test that v1 gen:* tokens are dropped from vocab."""

    def test_gen_prefix_tokens_dropped_regardless_of_frequency(self):
        """gen:* tokens are dropped from vocab regardless of frequency."""
        # High frequency gen:* token - should still be dropped
        examples = [{"tokens": ["gen:Foo.bar", "gen:Foo.bar"] * 1000}]
        vocab = build_ir_vocab(examples)
        assert "gen:Foo.bar" not in vocab, "gen:* tokens should be dropped"
        
        # Low frequency gen:* token
        examples = [{"tokens": ["gen:LowFreq.token"]}]
        vocab = build_ir_vocab(examples)
        assert "gen:LowFreq.token" not in vocab, "gen:* tokens should be dropped"

    def test_gen_unk_still_in_vocab(self):
        """GEN_UNK remains in vocab."""
        examples = [{"tokens": ["GEN_UNK"]}]
        vocab = build_ir_vocab(examples)
        assert "GEN_UNK" in vocab, "GEN_UNK should be in vocab"

    def test_gen_bucket_tokens_not_dropped(self):
        """GEN_* bucket tokens (without gen: prefix) are NOT dropped."""
        examples = [{"tokens": ["GEN_ALGEBRA", "GEN_MATHLIB"]}]
        vocab = build_ir_vocab(examples)
        assert "GEN_ALGEBRA" in vocab, "GEN_ALGEBRA should be in vocab"
        assert "GEN_MATHLIB" in vocab, "GEN_MATHLIB should be in vocab"


class TestFreqDrivenTokens(unittest.TestCase):
    """Test that freq-driven tokens are handled correctly."""

    def test_typeclass_name_qualified_in_vocab(self):
        """typeclass_name:Group token (freq-driven) is added to vocab if it appears."""
        examples = [{"tokens": ["typeclass_name:Group", "typeclass_name:Ring"]}]
        vocab = build_ir_vocab(examples)
        assert "typeclass_name:Group" in vocab, "typeclass_name:Group should be in vocab"
        assert "typeclass_name:Ring" in vocab, "typeclass_name:Ring should be in vocab"


class TestEncodeIR(unittest.TestCase):
    """Test encode_ir function with v2 tokens."""

    def test_gen_bucket_encodes_correctly(self):
        """encode_ir correctly encodes GEN_ALGEBRA to its vocab ID."""
        examples = [{"tokens": ["GEN_ALGEBRA"]}]
        vocab = build_ir_vocab(examples)
        encoded = encode_ir(["GEN_ALGEBRA"], vocab)
        assert len(encoded) == 1, "Should encode to one token"
        assert encoded[0] == vocab["GEN_ALGEBRA"], "Should encode to GEN_ALGEBRA's vocab ID"

    def test_gen_unk_encodes_correctly(self):
        """encode_ir correctly encodes GEN_UNK to its vocab ID."""
        examples = [{"tokens": ["GEN_UNK"]}]
        vocab = build_ir_vocab(examples)
        encoded = encode_ir(["GEN_UNK"], vocab)
        assert len(encoded) == 1, "Should encode to one token"
        assert encoded[0] == vocab["GEN_UNK"], "Should encode to GEN_UNK's vocab ID"

    def test_unknown_typeclass_name_falls_back_to_unk(self):
        """encode_ir falls back to <UNK> for unknown typeclass_name:RareClass."""
        examples = [{"tokens": ["typeclass_name:Group"]}]
        vocab = build_ir_vocab(examples)
        encoded = encode_ir(["typeclass_name:RareClass"], vocab)
        assert len(encoded) == 1, "Should encode to one token"
        assert encoded[0] == vocab["<UNK>"], "Unknown typeclass_name should fall back to <UNK>"

    def test_v1_gen_prefix_falls_back_to_gen_unk(self):
        """encode_ir falls back to GEN_UNK for v1 gen:* tokens (not in vocab)."""
        examples = [{"tokens": ["GEN_ALGEBRA"]}]  # v2 token
        vocab = build_ir_vocab(examples)
        # v1 gen:* token would not be in vocab
        encoded = encode_ir(["gen:Old.token"], vocab)
        assert len(encoded) == 1, "Should encode to one token"
        assert encoded[0] == vocab["GEN_UNK"], "v1 gen:* should fall back to GEN_UNK"

    def test_typeclass_name_in_vocab_encodes_correctly(self):
        """encode_ir correctly encodes known typeclass_name:Group."""
        examples = [{"tokens": ["typeclass_name:Group"]}]
        vocab = build_ir_vocab(examples)
        encoded = encode_ir(["typeclass_name:Group"], vocab)
        assert len(encoded) == 1, "Should encode to one token"
        assert encoded[0] == vocab["typeclass_name:Group"], "Should encode to typeclass_name:Group's vocab ID"


if __name__ == "__main__":
    import unittest
    unittest.main(verbosity=2)
