# Phase 5 Completion Checklist

## Current Status (as of 2026-07-28)

- **Matched-batch rerun complete**: A=1.39, B=1.15, C=1.13 (all `smoke_test: false`, effective batch=8)
- **DEC-007 resolved**: all variants at grad_accum=8; gap between A and B/C holds after fix
- **DEC-006 outstanding**: A uses randomly initialized embeddings (4,495 tokens); B/C use pretrained Qwen embeddings (151,643 tokens); this cold-start disadvantage is uncontrolled

## Completed

- [x] Full matched-batch A/B/C run (2026-07-28)
- [x] Results tables updated in `README.md` and `docs/EXPERIMENT_DESIGN.md`
- [x] `docs/PHASE_5_RESULTS.md` filled in with real numbers and interpretation
- [x] DEC-007 documented and resolved

## Next Step

Rule out DEC-006 (embedding cold-start confound) by running variant A for 3 epochs:

```bash
# In python/train.py, temporarily set VARIANT_EPOCHS["A"] = 3, then:
python3 python/run_full_experiment.py --variant A
```

If the gap to B/C (1.15/1.13) shrinks substantially, DEC-006 is the primary cause.
If it holds, the IR token representation itself is the issue — investigate vocab size
or representation design.
