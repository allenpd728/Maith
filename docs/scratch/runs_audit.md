# `runs/` Directory Audit — Checkpoint/Results Consistency

**Audited:** 2026-08-08 (while v2 A training in progress in `runs/variant_A_v2_full/`)
**Method:** for each `runs/variant_*/` dir, compare the checkpoint `model.safetensors`
mtime against `results.json` mtime. A run is **STALE/INCONSISTENT** when the
`results.json` (perplexity) was written days after the checkpoint — meaning the
reported perplexity does not describe the checkpoint actually on disk.

This is the class of bug that produced Variant C's 1.5% completion-accuracy
(see `docs/scratch/variant_c_eval_bug.md`): the eval loads the on-disk checkpoint, not the
model that produced `results.json`.

## Summary table

| variant dir | ppl | smoke | ep | status | ckpt date | results date |
|---|---|---|---|---|---|---|
| variant_A | 1.2812 | – | 3 | **STALE/INCONSISTENT (3d)** | 07-30 08:10 | 08-01 22:28 |
| variant_A_dec006 | 1.2978 | – | 2 | OK | 08-02 21:45 | 08-02 21:45 |
| variant_A_phase6 | 1.2792 | – | 3 | OK | 08-02 01:25 | 08-02 01:26 |
| variant_A_phase6_smoke | 16.58 | yes | 1 | SMOKE | 08-01 23:12 | 08-01 23:12 |
| variant_A_phase6_smoke2 | 17.12 | yes | 1 | SMOKE | 08-01 23:14 | 08-01 23:15 |
| variant_A_v1_3_0 | 1.4325 | – | 2 | OK | 08-03 07:42 | 08-03 07:42 |
| variant_A_v1_3_0_ep | 1.4226 | – | 2 | OK | 08-03 08:18 | 08-03 08:19 |
| variant_A_v1_3_0_final | 1.3016 | – | 2 | OK | 08-03 10:45 | 08-03 10:46 |
| variant_A_v1_3_0_proj | 1.3717 | – | 2 | OK | 08-03 18:22 | 08-03 18:22 |
| variant_A_v1_3_0_true | 1.3896 | – | 2 | OK | 08-03 17:02 | 08-03 17:03 |
| variant_A_v1_4_0 | 1.2751 | – | 2 | OK | 08-03 21:23 | 08-03 21:23 |
| variant_A_v2 | – | – | – | EMPTY/IN-FLIGHT | – | – |
| variant_A_v2_0_0 | 16.06 | yes | 1 | SMOKE | 08-07 07:36 | 08-07 07:36 |
| variant_A_v2_full | – | – | – | EMPTY/IN-FLIGHT (current run) | – | – |
| variant_A_v3 | 1.2812 | – | 3 | OK | 08-01 08:10 | 08-01 08:11 |
| variant_B | 1.107 | – | 3 | **STALE/INCONSISTENT (2d)** | 07-30 18:46 | 08-01 22:28 |
| variant_B2 | 1.107 | – | 3 | OK | 08-01 00:35 | 08-01 00:38 |
| variant_B_phase6 | 1.1295 | – | 2 | OK | 08-02 12:17 | 08-02 12:30 |
| variant_C | 1.098 | – | 3 | **STALE/INCONSISTENT (2d)** | 07-31 09:46 | 08-01 22:28 |
| variant_C_phase6 | 1.1102 | – | 2 | OK | 08-02 17:34 | 08-02 17:37 |
| variant_C_v2 | 1.098 | – | 3 | OK | 08-01 19:53 | 08-01 19:55 |
| variant_flat | 1.0551 | – | 2 | OK | 08-03 22:28 | 08-03 22:28 |

## Findings

### Three stale/inconsistent legacy dirs — the dangerous ones

`variant_A`, `variant_B`, `variant_C` all have `results.json` timestamped
**08-01 22:28** (the "Phase 5 final" batch) while their checkpoints are 2–3 days
older. Their `results.json` perplexities (A=1.2812, B=1.107, C=1.098) describe a
*later* run whose checkpoint is **not** in these dirs. Loading any of these
checkpoints gives a model that does **not** match the reported perplexity.

- `variant_C` is the confirmed bad one → 1.5% completion accuracy.
- `variant_B` happened to score 91.7% only because `eval_completion.py` picks
  `checkpoint-277` (a numeric step checkpoint) rather than the stale
  `checkpoint-final`, and that step checkpoint is valid.
- `variant_A` is similarly inconsistent; treat its 1.2812 as untrustworthy
  for any checkpoint loaded from that dir.

### Authoritative run per variant (use these for eval)

| variant | authoritative dir | ppl | notes |
|---|---|---|---|
| A (v1.4.0) | `runs/variant_A_v1_4_0` | 1.2751 | best pre-v2 A |
| A (phase6) | `runs/variant_A_phase6` | 1.2792 | |
| A (DEC-006 proj) | `runs/variant_A_dec006` | 1.2978 | the DEC-026 baseline to beat |
| A (v2) | `runs/variant_A_v2_full` | (in progress) | this session's run |
| B | `runs/variant_B2` (1.107) or `runs/variant_B_phase6` (1.1295) | | avoid stale `variant_B` |
| C | `runs/variant_C_v2` (1.098) or `runs/variant_C_phase6` (1.1102) | | avoid stale `variant_C` |
| Flat | `runs/variant_flat` | 1.0551 | DEC-024 ablation |

### Smokes / empties (not results)

`variant_A_phase6_smoke`, `variant_A_phase6_smoke2`, `variant_A_v2_0_0` are
50-example smoke tests (ppl ~16–17) — not real results. `variant_A_v2` and
`variant_A_v2_full` are empty/in-flight.

## Recommendations

1. **Never eval against the default `runs/variant_{X}/`** without verifying the
   checkpoint matches `results.json`. Always pass the explicit authoritative dir,
   e.g. `--checkpoint-C runs/variant_C_v2`.
2. **Quarantine the three stale dirs** (`variant_A`, `variant_B`, `variant_C`) —
   rename to `runs/_stale_variant_A` etc. or delete, so the default path no longer
   silently loads a mismatched checkpoint. (Defer this mutation until the v2 A run
   finishes, to avoid any I/O contention.)
3. **Add a consistency guard** to `eval_completion.py` / `find_checkpoint_dir`:
   warn (or refuse) when `results.json` mtime differs from the checkpoint mtime by
   more than a threshold — this would have flagged C immediately.
4. **Record provenance in `results.json`** — add the checkpoint mtime / training
   step so future runs are self-describing.
