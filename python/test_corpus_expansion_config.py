#!/usr/bin/env python3
"""
test_corpus_expansion_config.py

Tests for DEC-019 corpus expansion config validation.
Verifies that Scripts/module_expansion_targets.json has the expected structure
and the expansion candidates don't overlap with baseline modules.

Run with:
    python3 python/test_corpus_expansion_config.py

A non-zero exit code means at least one test failed.
"""

import json
import os
import sys


# ---------------------------------------------------------------------------
# Helpers
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
# Config path
# ---------------------------------------------------------------------------

CONFIG_PATH = os.path.join("Scripts", "module_expansion_targets.json")


# ---------------------------------------------------------------------------
# TEST 1: Config file exists
# ---------------------------------------------------------------------------

def test_config_exists():
    name = "config file exists"
    if os.path.exists(CONFIG_PATH):
        ok(name)
    else:
        fail(name, f"{CONFIG_PATH} not found")


# ---------------------------------------------------------------------------
# TEST 2: Config is valid JSON
# ---------------------------------------------------------------------------

def test_config_valid_json():
    name = "config is valid JSON"
    try:
        with open(CONFIG_PATH) as f:
            data = json.load(f)
        ok(name)
    except json.JSONDecodeError as e:
        fail(name, f"Invalid JSON: {e}")


# ---------------------------------------------------------------------------
# TEST 3: Config has required keys
# ---------------------------------------------------------------------------

def test_config_has_required_keys():
    name = "config has required keys (baseline_modules, expansion_candidates)"
    required_keys = {"baseline_modules", "expansion_candidates"}
    try:
        with open(CONFIG_PATH) as f:
            data = json.load(f)
        
        missing = required_keys - set(data.keys())
        if missing:
            fail(name, f"Missing keys: {missing}")
        else:
            ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 4: expansion_candidates is a non-empty list
# ---------------------------------------------------------------------------

def test_expansion_candidates_non_empty():
    name = "expansion_candidates is a non-empty list"
    try:
        with open(CONFIG_PATH) as f:
            data = json.load(f)
        
        candidates = data.get("expansion_candidates", [])
        if not isinstance(candidates, list):
            fail(name, f"expansion_candidates is not a list: {type(candidates)}")
        elif len(candidates) == 0:
            fail(name, "expansion_candidates is empty")
        else:
            ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 5: Expected topology/order modules are present
# ---------------------------------------------------------------------------

def test_expected_modules_present():
    name = "expected topology/order modules are present"
    expected_modules = [
        "Mathlib.Topology.Basic",    # topological spaces, open sets
        "Mathlib.Order.Lattice",      # lattice structures
    ]
    try:
        with open(CONFIG_PATH) as f:
            data = json.load(f)
        
        candidates = data.get("expansion_candidates", [])
        missing = [m for m in expected_modules if m not in candidates]
        
        if missing:
            fail(name, f"Missing expected modules: {missing}")
        else:
            ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# TEST 6: No collision between baseline and expansion candidates
# ---------------------------------------------------------------------------

def test_no_collision():
    name = "no module appears in both baseline_modules and expansion_candidates"
    try:
        with open(CONFIG_PATH) as f:
            data = json.load(f)
        
        baseline = set(data.get("baseline_modules", []))
        candidates = set(data.get("expansion_candidates", []))
        
        overlap = baseline & candidates
        if overlap:
            fail(name, f"Modules in both sets: {overlap}")
        else:
            ok(name)
    except Exception as e:
        fail(name, str(e))


# ---------------------------------------------------------------------------
# Run all tests
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    print("Running corpus_expansion_config tests...\n")

    test_config_exists()
    test_config_valid_json()
    test_config_has_required_keys()
    test_expansion_candidates_non_empty()
    test_expected_modules_present()
    test_no_collision()

    print(f"\n{len(PASS)} passed, {len(FAIL)} failed.")
    if FAIL:
        print("Failed tests:")
        for name in FAIL:
            print(f"  - {name}")
        sys.exit(1)
    sys.exit(0)
