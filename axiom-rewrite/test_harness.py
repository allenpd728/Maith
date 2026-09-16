#!/usr/bin/env python3
"""Tests for the five-gate harness (#29).

The DoD's three requirements, each pinned:

  (a) the harness runs a spec end-to-end and emits a verdict per gate with the Lean
      obligation it discharged;
  (b) a fixture candidate that passes gate 1 and fails gate 2 (Unit-collapse) is
      correctly rejected with no partial credit;
  (c) gate 4 refuses to emit a compression number for a candidate that failed
      gate 3.

Plus the control that makes (b) meaningful: without a candidate that PASSES, a
harness that rejects everything would look identical to one that rejects correctly.

These tests need the Lean toolchain. They SKIP cleanly when `lake` is unavailable
(the sandbox loses it between sessions) rather than failing -- same pattern as the
corpus-dependent tests in #28.

Run: python3 axiom-rewrite/test_harness.py
"""

import json
import shutil
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent
sys.path.insert(0, str(HERE))

import harness as H  # noqa: E402

SPECS = HERE / "specs"
LEAN_AVAILABLE = shutil.which("lake") is not None


def _run(spec_name):
    spec = H.Spec.load(SPECS / spec_name)
    h = H.Harness(timeout=600)
    try:
        return spec, h.run(spec)
    finally:
        h.cleanup()


def _by_gate(results):
    return {r.gate: r.outcome for r in results}


# ---------------------------------------------------------------------------
# (a) verdict per gate, with the obligation recorded
# ---------------------------------------------------------------------------

def test_spec_loads_and_reports_every_gate():
    spec = H.Spec.load(SPECS / "fixture_unit_collapse.json")
    assert spec.candidate_id, "spec must carry an id"
    assert spec.prelude, "spec must carry a prelude (the Lean source)"
    assert 1 in spec.obligations and 2 in spec.obligations, spec.obligations
    print("PASS: test_spec_loads_and_reports_every_gate")


def test_every_gate_gets_a_verdict_and_obligation():
    if not LEAN_AVAILABLE:
        print("SKIP: test_every_gate_gets_a_verdict_and_obligation (no lake)")
        return
    _spec, results = _run("fixture_unit_collapse.json")
    gates = [r.gate for r in results]
    assert gates == [1, 2, 3, 4, 5], f"expected all five gates, got {gates}"
    for r in results:
        assert r.outcome in ("pass", "fail", "skip"), r.outcome
        assert r.obligation, f"gate {r.gate} recorded no obligation"
    print("PASS: test_every_gate_gets_a_verdict_and_obligation")


# ---------------------------------------------------------------------------
# (b) the DoD fixture: gate 1 pass, gate 2 fail, no partial credit
# ---------------------------------------------------------------------------

def test_unit_collapse_passes_gate1_fails_gate2():
    if not LEAN_AVAILABLE:
        print("SKIP: test_unit_collapse_passes_gate1_fails_gate2 (no lake)")
        return
    _spec, results = _run("fixture_unit_collapse.json")
    g = _by_gate(results)
    assert g[1] == "pass", f"gate 1 must PASS (it genuinely is a homomorphism): {g}"
    assert g[2] == "fail", f"gate 2 must FAIL (collapses everything): {g}"
    assert g[3] != "pass", "no partial credit: a gate-2 failure must not let gate 3 pass"
    print("PASS: test_unit_collapse_passes_gate1_fails_gate2")


def test_unit_collapse_is_rejected_overall():
    if not LEAN_AVAILABLE:
        print("SKIP: test_unit_collapse_is_rejected_overall (no lake)")
        return
    _spec, results = _run("fixture_unit_collapse.json")
    s = H.Harness.summary(results)
    assert s["all_passed"] is False, "the degenerate candidate must be rejected"
    assert s["highest_gate_passed"] == 1, s
    print("PASS: test_unit_collapse_is_rejected_overall")


# ---------------------------------------------------------------------------
# (c) gate 4 refuses compression when gate 3 failed
# ---------------------------------------------------------------------------

def test_gate4_refuses_when_gate3_fails():
    if not LEAN_AVAILABLE:
        print("SKIP: test_gate4_refuses_when_gate3_fails (no lake)")
        return
    _spec, results = _run("fixture_transfer_fails.json")
    g = _by_gate(results)
    assert g[1] == "pass" and g[2] == "pass", f"this fixture must reach gate 3: {g}"
    assert g[3] == "fail", f"gate 3 is deliberately false here: {g}"
    gate4 = next(r for r in results if r.gate == 4)
    assert gate4.outcome == "fail", gate4
    assert "refused" in gate4.obligation, gate4.obligation
    assert "no number emitted" in gate4.obligation, (
        "the refusal must be OBSERVABLE, not implied: " + gate4.obligation)
    print("PASS: test_gate4_refuses_when_gate3_fails")


def test_gate4_refuses_when_gate3_absent():
    """Gate 3 skipped (not failed) must also refuse: absence is not a pass."""
    unit = H.Harness._gate4(
        H.Spec.load(SPECS / "fixture_unit_collapse.json"),
        [H.GateResult(1, "pass", "x"), H.GateResult(2, "fail", "y")],
    )
    assert unit.outcome == "fail", unit
    assert "refused" in unit.obligation, unit.obligation
    print("PASS: test_gate4_refuses_when_gate3_absent")


# ---------------------------------------------------------------------------
# Control: the harness can PASS things
# ---------------------------------------------------------------------------

def test_control_passes_all_gates():
    if not LEAN_AVAILABLE:
        print("SKIP: test_control_passes_all_gates (no lake)")
        return
    _spec, results = _run("control_valid.json")
    g = _by_gate(results)
    assert g == {1: "pass", 2: "pass", 3: "pass", 4: "pass", 5: "pass"}, g
    assert H.Harness.summary(results)["all_passed"] is True
    print("PASS: test_control_passes_all_gates")


def test_gate4_permits_after_gate3_passes():
    spec = H.Spec.load(SPECS / "control_valid.json")
    r = H.Harness._gate4(spec, [H.GateResult(3, "pass", "x")])
    assert r.outcome == "pass", r
    assert "permitted" in r.obligation, r.obligation
    print("PASS: test_gate4_permits_after_gate3_passes")


# ---------------------------------------------------------------------------
# The oracle is the kernel, not a grep
# ---------------------------------------------------------------------------

def test_sorry_does_not_pass_a_gate():
    """A smuggled `sorry` must fail: `warningAsError` makes it an error.

    This is the harness's own integrity property -- if `sorry` counted as success,
    every gate would be trivially passable.
    """
    if not LEAN_AVAILABLE:
        print("SKIP: test_sorry_does_not_pass_a_gate (no lake)")
        return
    h = H.Harness(timeout=300)
    try:
        ok, detail = h.discharge("theorem smuggled : (0 : Nat) = 1 := by sorry",
                                 "sorry probe")
    finally:
        h.cleanup()
    assert ok is False, f"a `sorry` proof PASSED a gate (detail: {detail})"
    print("PASS: test_sorry_does_not_pass_a_gate")


def test_true_also_does_not_pass_when_obligation_is_false():
    """Sanity: a genuinely false obligation fails, so gates are not vacuous."""
    if not LEAN_AVAILABLE:
        print("SKIP: test_true_also_does_not_pass_when_obligation_is_false (no lake)")
        return
    h = H.Harness(timeout=300)
    try:
        ok, _ = h.discharge("theorem bogus : (0 : Nat) = 1 := rfl", "false probe")
    finally:
        h.cleanup()
    assert ok is False, "a false obligation PASSED -- gates would be vacuous"
    print("PASS: test_true_also_does_not_pass_when_obligation_is_false")


def test_run_records_to_ledger_with_the_ledgers_own_rules():
    """The harness -> ledger bridge (the gap left when #29 first closed).

    Records the DoD fixture and asserts the ledger's rules held: no compression for
    a candidate that did not pass gate 3, and not reusable without gate 5.
    """
    if not LEAN_AVAILABLE:
        print("SKIP: test_run_records_to_ledger_with_the_ledgers_own_rules (no lake)")
        return
    import tempfile, json as _json
    sys.path.insert(0, str(HERE))
    with tempfile.TemporaryDirectory() as d:
        ledger = Path(d) / "candidates.jsonl"
        spec = H.Spec.load(SPECS / "fixture_unit_collapse.json")
        h = H.Harness(timeout=600)
        try:
            results = h.run(spec)
        finally:
            h.cleanup()
        H.record(spec, results, ledger)

        rec = _json.loads(ledger.read_text().strip())
        assert rec["gates"]["2"] == "fail", rec["gates"]
        assert rec["compression"] is None, \
            f"compression recorded for a candidate that failed gate 3: {rec['compression']}"
        assert rec["reusable"] is False, "marked reusable without gate 5"
        assert rec["status"] == "failed_gate_2", rec["status"]
    print("PASS: test_run_records_to_ledger_with_the_ledgers_own_rules")


def test_control_records_as_reusable():
    if not LEAN_AVAILABLE:
        print("SKIP: test_control_records_as_reusable (no lake)")
        return
    import tempfile, json as _json
    with tempfile.TemporaryDirectory() as d:
        ledger = Path(d) / "candidates.jsonl"
        spec = H.Spec.load(SPECS / "control_valid.json")
        h = H.Harness(timeout=600)
        try:
            results = h.run(spec)
        finally:
            h.cleanup()
        H.record(spec, results, ledger)
        rec = _json.loads(ledger.read_text().strip())
        assert rec["reusable"] is True, \
            "a gate-5 survivor should be marked reusable"
        assert rec["status"] == "passed_gate_5", rec["status"]
    print("PASS: test_control_records_as_reusable")


def test_recording_declines_compression_for_a_failed_transfer():
    """A candidate that failed gate 3 cannot carry compression, even if offered."""
    if not LEAN_AVAILABLE:
        print("SKIP: test_recording_declines_compression_for_a_failed_transfer (no lake)")
        return
    import tempfile, json as _json
    with tempfile.TemporaryDirectory() as d:
        ledger = Path(d) / "candidates.jsonl"
        spec = H.Spec.load(SPECS / "fixture_transfer_fails.json")
        h = H.Harness(timeout=600)
        try:
            results = h.run(spec)
        finally:
            h.cleanup()
        # Offer a compression number the caller should not get to record.
        H.record(spec, results, ledger, compression={"nodes_before": 10, "nodes_after": 3})
        rec = _json.loads(ledger.read_text().strip())
        assert rec["compression"] is None, (
            "compression was recorded despite gate 3 failing -- the bridge must "
            f"not forward it: {rec['compression']}")
    print("PASS: test_recording_declines_compression_for_a_failed_transfer")


def main() -> int:
    tests = [
        test_spec_loads_and_reports_every_gate,
        test_every_gate_gets_a_verdict_and_obligation,
        test_unit_collapse_passes_gate1_fails_gate2,
        test_unit_collapse_is_rejected_overall,
        test_gate4_refuses_when_gate3_fails,
        test_gate4_refuses_when_gate3_absent,
        test_control_passes_all_gates,
        test_gate4_permits_after_gate3_passes,
        test_sorry_does_not_pass_a_gate,
        test_true_also_does_not_pass_when_obligation_is_false,
        test_run_records_to_ledger_with_the_ledgers_own_rules,
        test_control_records_as_reusable,
        test_recording_declines_compression_for_a_failed_transfer,
    ]
    passed = failed = 0
    for t in tests:
        try:
            t()
            passed += 1
        except AssertionError as e:
            print(f"FAIL: {t.__name__}: {e}")
            failed += 1
        except Exception as e:
            print(f"ERROR: {t.__name__}: {type(e).__name__}: {e}")
            failed += 1
    print()
    print("=" * 40)
    print(f"Tests: {passed} passed, {failed} failed")
    if not LEAN_AVAILABLE:
        print("(Lean unavailable -- gate-executing tests skipped)")
    print("=" * 40)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())