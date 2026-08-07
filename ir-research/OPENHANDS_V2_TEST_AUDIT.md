# KIT Orchestrator — IR v2 Test Audit + ExtractionFaithfulnessTests Fix

Branch: `openhands/ir-v2-test-audit`
Base: `kit/dev` (current HEAD: ad57c7d)
Repo: `/Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo`
Python: `/Library/Frameworks/Python.framework/Versions/3.14/bin/python3`

## Context

IR v2 (C2 typeclass_name, C3 const_name, C4 GEN_* bucketing) is implemented on `kit/dev`.
The Lean test suite currently fails to compile due to a signature change in
`extractGraphFromConstantInfo`. There are also zero tests covering the new v2 token
behaviours. This task fixes the compilation break and adds v2 faithfulness tests.

Do NOT modify `Maith/MetaExtractor.lean` or any other production Lean file.
Do NOT run `build_dataset.py` or start a training run.
Changes are confined to `Tests/ExtractionFaithfulnessTests.lean` only.

---

## Task 1 — Fix compilation break

### What broke

`extractGraphFromConstantInfo` now requires a `module : String` parameter (added in the
C4 fix). The test helper `extractDeclGraph` in `Tests/ExtractionFaithfulnessTests.lean`
calls it with the old one-argument signature (line ~49):

```lean
match extractGraphFromConstantInfo info with
```

This produces:
```
error: Type mismatch — ProcessingResult.ok g has type ProcessingResult ?m.10
  but is expected to have type String → ProcessingResult Graph
```

### Fix

`extractDeclGraph` already receives `env : Environment`. Use the environment to look up
the module for the declaration, then pass it to `extractGraphFromConstantInfo`.

The function `moduleNameForDecl` in `Maith/MetaExtractor.lean` does exactly this but is
`private`. Instead, replicate the lookup inline in `extractDeclGraph` using the public
Lean API — `env.getModuleIdxFor?` — which is already used internally:

```lean
def extractDeclGraph (env : Environment) (declName : String) : Option Graph := do
  let info ← getConstant? env declName
  -- Resolve the declaration's module for v2 GEN_* bucketing
  let n := declName.splitOn "." |>.foldl (fun acc s => Name.str acc s) Name.anonymous
  let moduleName : String :=
    match env.getModuleIdxFor? n with
    | some midx =>
      if midx.toNat < env.header.modules.size then
        env.header.modules[midx.toNat]!.module.toString
      else
        env.header.mainModule.toString
    | none => env.header.mainModule.toString
  match extractGraphFromConstantInfo info moduleName with
  | .ok g => pure g
  | .fail _ => none
```

### Verify the fix compiles

```bash
cd /Users/philipallen/Library/Caches/com.spotify.studio/.studio/artifacts/maith-repo
~/.elan/bin/lake build tests 2>&1 | tail -10
```

Must complete with `Build completed successfully`. Warnings are OK, errors are not.

Then run the tests:

```bash
~/.elan/bin/lake exe tests 2>&1 | tail -20
```

All existing tests must pass (or be marked as skipped due to Mathlib availability).

---

## Task 2 — Add v2 IR faithfulness tests

Add the following tests to `extractionFaithfulnessTests` in
`Tests/ExtractionFaithfulnessTests.lean`, after the existing test blocks.

These use the same pattern as the existing tests: guard with `declExists?`, skip
gracefully if Mathlib isn't loaded.

### C2 — typeclass_name attribute

```lean
  /- ============================================
     v2 C2: typeclass_name attribute
     instImplicit binders should carry typeclass_name in addition to typeclass
     ============================================ -/
  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has typeclass_name A row (v2 C2)" "mul_assoc"
      (hasAttributeKey · "typeclass_name")
      "mul_assoc has a typeclass binder — should emit typeclass_name attribute (C2)"
  else
    TestResult.pass "mul_assoc has typeclass_name A row (v2 C2)" "Skipped: mul_assoc not available",

  if declExists? env "AddZero.mk" then
    runGraphTest env "AddZero.mk has typeclass_name A row (v2 C2)" "AddZero.mk"
      (hasAttributeKey · "typeclass_name")
      "AddZero.mk is a typeclass constructor — should emit typeclass_name (C2)"
  else
    TestResult.pass "AddZero.mk has typeclass_name A row (v2 C2)" "Skipped: AddZero.mk not available",
```

### C3 — const_name attribute

```lean
  /- ============================================
     v2 C3: const_name attribute on .const entities
     Every constant-headed entity should carry a const_name attribute
     ============================================ -/
  if declExists? env "Nat.add" then
    runGraphTest env "Nat.add has const_name A row (v2 C3)" "Nat.add"
      (hasAttributeKey · "const_name")
      "Nat.add involves .const exprs — should emit const_name attribute (C3)"
  else
    TestResult.pass "Nat.add has const_name A row (v2 C3)" "Skipped: Nat.add not available",

  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc has const_name A rows (v2 C3)" "mul_assoc"
      (fun g => countAttributeKey g "const_name" ≥ 1)
      "mul_assoc references constants — should have at least one const_name attribute (C3)"
  else
    TestResult.pass "mul_assoc has const_name A rows (v2 C3)" "Skipped: mul_assoc not available",
```

### C4 — GEN_* bucket tokens, no GEN_UNKNOWN for Mathlib declarations

Add a helper at the top of the test helpers section (after `countAttributeKey`):

```lean
/--
Check that a graph has at least one O row whose op string starts with "GEN_"
but is NOT "GEN_UNKNOWN". Used to verify module-based bucketing (C4).
-/
def hasNamedGenBucket (g : Graph) : Bool :=
  g.operations.any (fun o =>
    match o.op with
    | .generic s => s.startsWith "GEN_" && s ≠ "GEN_UNKNOWN"
    | _ => false)

/--
Check that a graph has NO operations with op "GEN_UNKNOWN".
-/
def hasNoGenUnknown (g : Graph) : Bool :=
  g.operations.all (fun o =>
    match o.op with
    | .generic s => s ≠ "GEN_UNKNOWN"
    | _ => true)
```

Then add the tests:

```lean
  /- ============================================
     v2 C4: GEN_* module-based bucketing
     Mathlib declarations should produce named GEN_* buckets, not GEN_UNKNOWN
     ============================================ -/
  if declExists? env "mul_assoc" then
    runGraphTest env "mul_assoc generic ops use named GEN_* buckets (v2 C4)" "mul_assoc"
      (fun g => g.operations.isEmpty || hasNoGenUnknown g)
      "mul_assoc is in Mathlib.Algebra — generic ops should NOT be GEN_UNKNOWN (C4)"
  else
    TestResult.pass "mul_assoc generic ops use named GEN_* buckets (v2 C4)" "Skipped: mul_assoc not available",

  if declExists? env "Nat.add" then
    runGraphTest env "Nat.add generic ops use named GEN_* buckets (v2 C4)" "Nat.add"
      (fun g => g.operations.isEmpty || hasNoGenUnknown g)
      "Nat.add is in Init — generic ops should be GEN_INIT, not GEN_UNKNOWN (C4)"
  else
    TestResult.pass "Nat.add generic ops use named GEN_* buckets (v2 C4)" "Skipped: Nat.add not available",
```

---

## Final verification

After all changes:

```bash
~/.elan/bin/lake build tests 2>&1 | tail -5
~/.elan/bin/lake exe tests 2>&1 | tail -30
```

All tests must either PASS or be marked SKIPPED (Mathlib availability). Zero failures.

---

## Commit and push

```bash
git checkout -b openhands/ir-v2-test-audit
git add Tests/ExtractionFaithfulnessTests.lean
git commit -m "tests: fix ExtractionFaithfulnessTests for v2 API + add C2/C3/C4 coverage

- Fix extractDeclGraph to pass module string to extractGraphFromConstantInfo
- Add typeclass_name tests (C2): mul_assoc, AddZero.mk
- Add const_name tests (C3): Nat.add, mul_assoc
- Add GEN_* bucket tests (C4): mul_assoc, Nat.add — verify no GEN_UNKNOWN
- Add hasNamedGenBucket and hasNoGenUnknown helpers"
git push origin openhands/ir-v2-test-audit
```

Do NOT push to `kit/dev`. Kit reviews before merging.

---

## Done signal

When all tests pass and the branch is pushed, output exactly:

## Task Completed Successfully

Include the full test run output in your completion message.
