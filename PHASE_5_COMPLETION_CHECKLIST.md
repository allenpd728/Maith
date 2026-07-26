# Phase 5 Completion Checklist

## Current Status (Jul 26, 16:28 UTC)
- Variant A: ✅ Complete (69.0 min, 1.297 perplexity, 361.9M params)
- Variant B: 🔄 Running (2% complete, expected finish ~04:00 UTC Jul 27)
- Variant C: ⏳ Queued (expected finish ~16:00 UTC Jul 27)

## What to Do When All Three Finish

### Step 1: Verify completion (run this at ~16:30 UTC Jul 27)
```bash
ls -lh runs/variant_{A,B,C}/results.json
cat runs/variant_{A,B,C}/results.json | jq -s '.[] | {variant, eval_perplexity, training_minutes, batch_size}'
```

### Step 2: Run comparison
```bash
python3 python/compare_results.py --full-results
```

### Step 3: Capture results
```bash
cp runs/variant_*/results.json runs/comparison_$(date +%Y%m%d_%H%M%S).json
mkdir -p artifacts/phase_5_results
cp runs/comparison_*.json artifacts/phase_5_results/
```

### Step 4: Document findings
- Create `docs/PHASE_5_RESULTS.md`
- Include: perplexity table, training time comparison, batch config notes
- Note: "This measures next-token prediction efficiency, not downstream task performance"

### Step 5: Future work section
Add to `docs/PHASE_5_RESULTS.md`:
```markdown
## Future Directions

### Context-Pack Protocol (Phase 6a)
Extract proof context (dependencies, scoped variables, type classes) 
from Mathlib declarations. Enables semantic-aware training for variants.

### Theorem-Proving Evaluation (Phase 6)
Build harness to measure actual proof-finding ability on held-out theorems.
Requires: Phase 5 baseline models + Phase 6a context packs (optional but recommended).

### Corpus Expansion
Current corpus: 4 Mathlib modules. Expand to full library (1000+ modules) 
to measure generalization and improve sample efficiency.
```

### Step 6: Commit
```bash
git add runs/ docs/PHASE_5_RESULTS.md artifacts/ 
git commit -m "Phase 5 complete: A/B/C perplexity comparison on canonical IR

Results: [summary]

Batch size tuning fixed OOM on variants B/C. All three variants trained
to completion with identical held-out evaluation set (512-token cap).

Primary finding: IR representation (A) shows [X]% better/worse perplexity
than raw Lean baselines (B/C).

This measures next-token prediction efficiency on isolated declarations.
Future work: context-pack protocol + theorem-proving evaluation harness.

Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>"
```

## Success Criteria
- [ ] B completes without OOM (check around 04:00 UTC Jul 27)
- [ ] C completes without OOM (check around 16:00 UTC Jul 27)
- [ ] All three results.json files present with eval_perplexity values
- [ ] Comparison shows clear A vs B/C delta (or no significant difference)
- [ ] Documentation captures findings honestly (no overinterpretation)
- [ ] Future work section positions context-packs + proving as separate projects

