# Phase 5 Completion Checklist

## Current Status (as of 2026-07-28)

- **Matched-batch rerun** (all variants `grad_accum=8`, effective batch size=8) is in progress.
- **Previous run** (2026-07-27) produced real full-run results (`smoke_test: false`):
  - A: 1.3922 perplexity, 22.6 min
  - B: 1.1420 perplexity, 41.3 min
  - C: 1.1298 perplexity, 44.3 min
  - These results carry confounds DEC-006 and DEC-007 and are not the final numbers.
- **Pre-flight check** (`python/preflight_check.py`) passed 7/7 checks before this rerun started.

## When Complete

1. Confirm `effective_batch_size=8` in all three `results.json` files.
2. Run `python3 python/preflight_check.py --datasets datasets/` to validate post-rerun state.
3. Update results tables in `README.md` and `docs/EXPERIMENT_DESIGN.md` with the new numbers.
4. Rebuild datasets and fix `eval_completion.py` label bug, then run completion accuracy.
