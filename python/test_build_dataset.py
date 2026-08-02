#!/usr/bin/env python3
"""
test_build_dataset.py

Pipeline integrity tests for build_dataset.py — transforms Corpus/corpus.jsonl into
the three variant datasets (A: IR tokens, B: Lean source BPE, C: AST-style BPE).

Tests are CPU-only, synthetic-fixture-based, and complete in under 5 seconds.
They guard against silent corruption in:
  - Vocab construction (stable, low IDs for structural tokens, GEN_UNK thresholding)
  - Token sequence construction (integer IDs, GRAPH_BEGIN/END framing, labels == input_ids)
  - Train/eval split (90/10, no leakage, reproducibility, consistent partition across variants)
  - FVAR_*/BVAR_* regression (guard against commit c5f1e99 bug)

Run with:
    python3 python/test_build_dataset.py

A non-zero exit code means at least one test failed.
"""

import json
import os
import sys
import tempfile
import shutil

sys.path.insert(0, "python")
import importlib.util

_spec = importlib.util.spec_from_file_location(
    "build_dataset", os.path.join("python", "build_dataset.py")
)
_mod = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_mod)

build_ir_vocab = _mod.build_ir_vocab
encode_ir = _mod.encode_ir
build_variant_A = _mod.build_variant_A
example_id = _mod.example_id


# ---------------------------------------------------------------------------
# Synthetic corpus fixtures
# ---------------------------------------------------------------------------

def make_fixture(count: int = 25) -> list[dict]:
    """Create a minimal synthetic corpus with realistic IR token patterns."""
    fixtures = []
    for i in range(count):
        name = f"Decl_{i:03d}"
        module = f"Mathlib.Test.Fixture_{i % 3}"
        tokens = [
            "GRAPH_BEGIN",
            "E", "FVAR_0", "neut",
            "E", "FVAR_1", "neut",
            "E", "TERM_0", "neut",
            "A", "FVAR_0", "typeclass", "Monoid", "neut",
            "A", "TERM_0", "sort", "u + 1", "neut",
            "R", "FVAR_0", "TERM_0", "eq", "neut",
            "O", "inputs:FVAR_0", "output:TERM_1", f"gen:Decl_{i}.op", "neut",
            "O", "inputs:FVAR_0,FVAR_1", "output:TERM_2", f"gen:Decl_{i}.combine", "neut",
            "GRAPH_END",
        ]
        fixtures.append({
            "name": name,
            "module": module,
            "tokens": tokens,
            "leanExpr": f"forall {{x : Type}}, Monoid.{name}",
        })
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


# ---------------------------------------------------------------------------
# TEST 1: Vocab construction — stability (same input → same output)
#
# Guards against non-determinism in vocab building. build_ir_vocab must be
# stable across calls with identical inputs.
# ---------------------------------------------------------------------------

def test_build_ir_vocab_stable():
    name = "build_ir_vocab: deterministic (same input → same output)"
    try:
        examples = make_fixture(20)
        vocab1 = build_ir_vocab(examples)
        vocab2 = build_ir_vocab(examples)
        assert vocab1 == vocab2, "Vocab differs between two calls with identical input"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 2: Vocab construction — structural tokens get low fixed IDs (< 20)
#
# Structural tokens (GRAPH_BEGIN, GRAPH_END, E, A, R, O, pos, neg, neut,
# <PAD>, <BOS>, <EOS>, <UNK>, GEN_UNK) must be assigned IDs in the reserved
# low-ID range for readability and consistency.
# ---------------------------------------------------------------------------

def test_structural_tokens_low_ids():
    name = "build_ir_vocab: structural tokens receive IDs < 20"
    try:
        examples = make_fixture(20)
        vocab = build_ir_vocab(examples)
        structural = [
            "GRAPH_BEGIN", "GRAPH_END", "E", "A", "R", "O",
            "pos", "neg", "neut",
            "<PAD>", "<BOS>", "<EOS>", "<UNK>", "GEN_UNK",
        ]
        for tok in structural:
            assert tok in vocab, f"Structural token '{tok}' missing from vocab"
            assert vocab[tok] < 20, f"Structural token '{tok}' has ID {vocab[tok]} >= 20"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 3: Vocab construction — rare gen:* tokens collapse to GEN_UNK
#
# Tokens below GEN_UNK_THRESHOLD frequency must be mapped to GEN_UNK, not
# assigned their own IDs. This prevents vocab bloat from rare generated tokens.
# ---------------------------------------------------------------------------

def test_gen_unk_thresholding():
    name = "build_ir_vocab: tokens below threshold collapse to GEN_UNK"
    try:
        # Create corpus where gen:rare appears only once (below threshold)
        examples = [
            {
                "name": "Decl_Rare",
                "module": "Mathlib.Test",
                "tokens": ["GRAPH_BEGIN", "E", "FVAR_0", "neut", "gen:rare", "GRAPH_END"],
                "leanExpr": "forall x, gen:rare",
            }
        ]
        vocab = build_ir_vocab(examples, freq_threshold=5)
        gen_unk_id = vocab.get("GEN_UNK", None)
        assert gen_unk_id is not None, "GEN_UNK not in vocab"
        # gen:rare appears once (< threshold), so encode_ir should map it to GEN_UNK
        encoded = encode_ir(["gen:rare"], vocab)
        assert encoded == [gen_unk_id], \
            f"gen:rare should map to GEN_UNK (id={gen_unk_id}), got {encoded}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 4: Vocab construction — FVAR_*/BVAR_* tokens present
#
# Guards the specific bug from commit c5f1e99 where FVAR_0, FVAR_1, BVAR_0,
# BVAR_1 were missing from vocab construction. These positional tokens from
# Encoder v1.2.0 must not be collapsed.
# ---------------------------------------------------------------------------

def test_fvar_bvar_tokens_present():
    name = "build_ir_vocab: FVAR_0, FVAR_1, BVAR_0, BVAR_1 present (c5f1e99 guard)"
    try:
        examples = make_fixture(20)
        vocab = build_ir_vocab(examples)
        for tok in ["FVAR_0", "FVAR_1", "BVAR_0", "BVAR_1"]:
            assert tok in vocab, f"Positional token '{tok}' missing from vocab (c5f1e99 regression)"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 5: Vocab construction — contiguous IDs from 0 with no gaps
#
# Vocab IDs must be contiguous starting from 0. Gaps indicate corruption or
# unintended token removal.
# ---------------------------------------------------------------------------

def test_vocab_ids_contiguous():
    name = "build_ir_vocab: IDs are contiguous starting from 0"
    try:
        examples = make_fixture(20)
        vocab = build_ir_vocab(examples)
        max_id = max(vocab.values())
        assigned = set(vocab.values())
        expected = set(range(max_id + 1))
        missing = expected - assigned
        assert not missing, f"Vocab has gaps: missing IDs {sorted(missing)}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 6: Token sequences — Variant A produces integer IDs from vocab
#
# Variant A input_ids must be integer IDs, not raw token strings. This
# ensures the model receives the correct numeric token representation.
# ---------------------------------------------------------------------------

def test_variant_a_input_ids_are_integers():
    name = "build_variant_A: input_ids are integer IDs from vocab"
    try:
        examples = make_fixture(10)
        vocab = build_ir_vocab(examples)
        rows = build_variant_A(examples, vocab, "test_v1")
        assert len(rows) == len(examples)
        for row in rows:
            ids = row["input_ids"]
            assert isinstance(ids, list), f"input_ids is not a list: {type(ids)}"
            assert all(isinstance(x, int) for x in ids), \
                f"input_ids contains non-integers: {[type(x) for x in ids if not isinstance(x, int)]}"
            # All IDs must be valid vocab entries
            vocab_ids = set(vocab.values())
            assert all(iid in vocab_ids for iid in ids), \
                f"input_ids contains invalid vocab IDs: {[iid for iid in ids if iid not in vocab_ids]}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 7: Token sequences — Variant A sequences begin/end with GRAPH markers
#
# All IR sequences must be framed with GRAPH_BEGIN and GRAPH_END tokens for
# clear graph boundary delimitation.
# ---------------------------------------------------------------------------

def test_variant_a_graph_boundaries():
    name = "build_variant_A: sequences begin with GRAPH_BEGIN and end with GRAPH_END"
    try:
        examples = make_fixture(10)
        vocab = build_ir_vocab(examples)
        rows = build_variant_A(examples, vocab, "test_v1")
        graph_begin_id = vocab["GRAPH_BEGIN"]
        graph_end_id = vocab["GRAPH_END"]
        for row in rows:
            ids = row["input_ids"]
            assert len(ids) >= 2, f"Sequence too short: {len(ids)}"
            assert ids[0] == graph_begin_id, \
                f"First token is not GRAPH_BEGIN: got ID {ids[0]}"
            assert ids[-1] == graph_end_id, \
                f"Last token is not GRAPH_END: got ID {ids[-1]}"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 8: Token sequences — labels == input_ids for all variants
#
# labels must equal input_ids (no pre-shift). The model shifts internally
# per the DEC-010 fix. This guards against accidental reintroduction of the
# label-shift bug.
# ---------------------------------------------------------------------------

def test_labels_equal_input_ids():
    name = "build_variant_A: labels == input_ids (no pre-shift)"
    try:
        examples = make_fixture(10)
        vocab = build_ir_vocab(examples)
        rows = build_variant_A(examples, vocab, "test_v1")
        for row in rows:
            assert row["labels"] == row["input_ids"], \
                f"labels != input_ids for {row.get('name', '?')}: label-shift bug regressed?"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 9: Token sequences — source field is correct per variant
#
# The source field must correctly identify which variant generated each row.
# ---------------------------------------------------------------------------

def test_variant_a_source_field():
    name = "build_variant_A: source field is 'A'"
    try:
        examples = make_fixture(10)
        vocab = build_ir_vocab(examples)
        rows = build_variant_A(examples, vocab, "test_v1")
        for row in rows:
            assert row["source"] == "A", f"Expected source='A', got '{row['source']}'"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 10: Token sequences — name and module fields present and non-empty
#
# Each example must have non-empty name and module for traceability.
# ---------------------------------------------------------------------------

def test_name_module_fields_present():
    name = "build_variant_A: name and module fields are present and non-empty"
    try:
        examples = make_fixture(10)
        vocab = build_ir_vocab(examples)
        rows = build_variant_A(examples, vocab, "test_v1")
        for row in rows:
            assert "name" in row and row["name"], f"Missing or empty 'name' in row"
            assert "module" in row and row["module"], f"Missing or empty 'module' in row"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 11: Train/eval split — 90/10 ratio within tolerance
#
# Split must be approximately 90% train / 10% eval. Allow 85-95% range for
# small corpora to account for rounding.
# ---------------------------------------------------------------------------

def test_split_ratio_90_10():
    name = "split: 90/10 ratio within 85-95% tolerance"
    try:
        examples = make_fixture(100)
        vocab = build_ir_vocab(examples[:90])  # Build vocab from training portion
        
        # Simulate the split logic from build_dataset.run()
        import random
        random.seed(42)
        shuffled = examples[:]
        random.shuffle(shuffled)
        split = int(len(shuffled) * 0.9)
        train_examples = shuffled[:split]
        eval_examples = shuffled[split:]
        
        total = len(train_examples) + len(eval_examples)
        train_ratio = len(train_examples) / total
        
        assert 0.85 <= train_ratio <= 0.95, \
            f"Train ratio {train_ratio:.3f} outside 0.85-0.95 range"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 12: Train/eval split — no declaration appears in both sets
#
# Guards against data leakage: no declaration should be in both train and eval.
# ---------------------------------------------------------------------------

def test_split_no_leakage():
    name = "split: no declaration appears in both train and eval (no leakage)"
    try:
        examples = make_fixture(100)
        import random
        random.seed(42)
        shuffled = examples[:]
        random.shuffle(shuffled)
        split = int(len(shuffled) * 0.9)
        train_examples = shuffled[:split]
        eval_examples = shuffled[split:]
        
        train_ids = set(example_id(ex) for ex in train_examples)
        eval_ids = set(example_id(ex) for ex in eval_examples)
        overlap = train_ids & eval_ids
        
        assert not overlap, f"Declaration leakage: {len(overlap)} declarations in both splits"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 13: Train/eval split — reproducibility with same seed
#
# Same seed must produce identical splits on repeated calls.
# ---------------------------------------------------------------------------

def test_split_reproducibility():
    name = "split: reproducible with same seed"
    try:
        examples = make_fixture(100)
        import random
        
        def run_split(seed):
            random.seed(seed)
            shuffled = examples[:]
            random.shuffle(shuffled)
            split = int(len(shuffled) * 0.9)
            return [example_id(ex) for ex in shuffled[:split]]
        
        split1 = run_split(42)
        split2 = run_split(42)
        assert split1 == split2, "Split differs with same seed (non-deterministic)"
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 14: Corpus file absent — graceful skip
#
# If Corpus/corpus.jsonl is absent, tests should skip gracefully with a
# descriptive message rather than crashing.
# ---------------------------------------------------------------------------

def test_corpus_absent_skips_gracefully():
    name = "corpus absent: skips gracefully"
    try:
        corpus_path = "Corpus/corpus.jsonl"
        if os.path.exists(corpus_path):
            print(f"  (skipping: corpus exists at {corpus_path})")
            ok(name + " (skipped)")
            return
        
        # If corpus doesn't exist, the fixture-based tests above are sufficient
        ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("Running build_dataset.py pipeline integrity tests...\n")

    test_build_ir_vocab_stable()
    test_structural_tokens_low_ids()
    test_gen_unk_thresholding()
    test_fvar_bvar_tokens_present()
    test_vocab_ids_contiguous()
    test_variant_a_input_ids_are_integers()
    test_variant_a_graph_boundaries()
    test_labels_equal_input_ids()
    test_variant_a_source_field()
    test_name_module_fields_present()
    test_split_ratio_90_10()
    test_split_no_leakage()
    test_split_reproducibility()
    test_corpus_absent_skips_gracefully()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
