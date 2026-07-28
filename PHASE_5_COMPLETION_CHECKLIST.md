# Phase 5 Completion Checklist

## Current Status (as of 2026-07-28)

- **Matched-batch rerun complete**: A=1.39, B=1.15, C=1.13 (all `smoke_test: false`, effective batch=8)
- **DEC-007 resolved**: all variants at grad_accum=8; gap between A and B/C holds after fix
- **DEC-008 complete**: 3-epoch A=1.26 — cold-start is a real factor but does not fully explain the gap
- **DEC-009 in progress**: embedding warm-start experiment; code shipped in `train.py` (`--warm-start-embeddings`), run pending

## Completed

- [x] Full matched-batch A/B/C run (2026-07-28)
- [x] Results tables updated in `README.md` and `docs/EXPERIMENT_DESIGN.md`
- [x] `docs/PHASE_5_RESULTS.md` filled in with real numbers and interpretation
- [x] DEC-007 documented and resolved
- [x] DEC-008: 3-epoch variant A run complete (result: 1.2598)
- [x] `--epochs` flag added to `train.py`
- [x] `--warm-start-embeddings` flag added to `train.py` (DEC-009 implementation)
- [x] DEC-009 drafted in `docs/DECISION_LOG.md`

## In Progress

- [ ] **DEC-009**: run variant A with `--warm-start-embeddings`, compare to B/C baseline

  ```bash
  python3 python/train.py --variant A --datasets datasets/ --out runs/variant_A_warmstart \
    --warm-start-embeddings && \
  cp -r ~/Projects/Maith/runs/ ~/Library/Caches/com.spotify.studio/.studio/artifacts/maith-runs/
  ```

## Remaining after DEC-009

- [ ] Update `docs/DECISION_LOG.md` DEC-009 status to `complete`
- [ ] Add warm-start result to `docs/PHASE_5_RESULTS.md`
- [ ] Write final Phase 5 interpretation and update `README.md` roadmap entry to ✅
- [ ] Decide on Phase 6 (theorem-proving evaluation) based on DEC-009 outcome
