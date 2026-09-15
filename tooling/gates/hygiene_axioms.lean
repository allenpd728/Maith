/-!
# Gate 6 (Tier 2) — axiom-tracking hygiene

This is a *spec*, not an implementation. It requires `lake build` and the
Lean toolchain, so it is the local agent's responsibility. Ported from the
sibling project PleaNP (2026-09-15).

## What Tier 2 catches that Tier 1 cannot

`hygiene_scan.py` (Tier 1) greps source for literal `sorry`/`admit`/`axiom`.
It CANNOT catch:

- **`sorry` smuggled via a meta-program.** A custom tactic, macro, or
  `elaborator` can internally synthesize a sorry term; the call site shows
  only the tactic name. This is the "compiles but means nothing" failure the
  OpenAI Navier–Stokes episode made concrete.
- **A proof that uses a custom axiom declared elsewhere** (in a dependency,
  or transitively).

## What the local agent runs

For each claim Maith asserts (currently: the transfer results the
axiom-discovery track produces — see `docs/experiments/AXIOM_DISCOVERY.md`):

```lean
-- For each frozen theorem T:
#print axioms T
```

Acceptance rule:

- **Accept** iff the axiom set is exactly the trusted standard set:
  `{propext, Classical.choice, Quot.sound, funext, Quot.ind}`. No custom
  axioms, no `sorry`-derived axioms.
- **Reject** if the set contains anything else — most importantly
  `sorryAx` or any `axiom` declared outside Mathlib core.

Runner: `tooling/gates/axiom_check.py` (retarget its imports and declaration
names to the current claim before use).

## How the two tiers compose

A claim is "Gate 6 passed" only when BOTH tiers run:

- Tier 1 (`hygiene_scan.py`): no literal sorry/admit, no custom axiom
  declarations in source.
- Tier 2 (this file, `#print axioms`): the proof's axiom set is exactly the
  trusted standard set.

Tier 1 is a fast pre-check; Tier 2 is the trust anchor. Neither alone
suffices. See `docs/TOOLCHAIN_AND_CI.md` §3.
-/