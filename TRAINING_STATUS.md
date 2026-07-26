# A/B/C Full Run Status

## Current State (Jul 26, 16:05 UTC)

| Variant | Status | Progress | ETA | Notes |
|---------|--------|----------|-----|-------|
| **A** | ✅ Complete | 831/831 steps (3 epochs) | — | 69.0 min, **1.297 perplexity** |
| **B** | 🔄 Running | 3/831 steps (~0.4%) | ~04:00 Jul 27 UTC | BS=1, GA=8, clean training (6.6 sec/step) |
| **C** | ⏳ Queued | — | ~16:00 Jul 27 UTC | Will start after B completes |

## Key Changes

**Batch Size Fix:** Addressed OOM by using variant-specific batch configs while keeping effective batch size constant.

| Variant | Batch Size | Grad Accum | Effective | Reason |
|---------|-----------|-----------|-----------|--------|
| A | 2 | 4 | 8 | Small 4.5K vocab, fits easily |
| B | 1 | 8 | 8 | Large 151K vocab, needs smaller BS to fit |
| C | 1 | 8 | 8 | Large 151K vocab, needs smaller BS to fit |

## Monitoring

Watch training in real-time:
```bash
tail -f runs/full_abc_runs.log
```

Quick status check:
```bash
python3 python/watch_full_runs.py --log runs/full_abc_runs.log
```

## When B/C Complete

Run the post-run pipeline:
```bash
python3 python/run_postrun_pipeline.py --runs-dir runs/ --allow-incomplete
```

This will:
1. Strict comparison of A/B/C results
2. Check publication gate
3. Add decision log entry
4. Validate all artifact schemas

## Interview Talking Points

- **Problem:** B crashed due to embedding table memory pressure (33× larger than A)
- **Solution:** Adaptive batch sizes with constant effective batch (good experimental hygiene)
- **Insight:** IR representation (A) is not just semantically better—it's also computationally lighter
