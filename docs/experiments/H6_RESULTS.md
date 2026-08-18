# H6 Retrieval Results — A_v3_2ep vs B_small_clean

> **Date:** 2026-08-18 (supersedes 2026-08-12 DEC-030 result)
> **Status:** ◐ Partial — IR numerically ahead of B_small_clean on Recall@10 but CIs
> fully overlap. H6 stays ⬜ in the grid pending a clean Mode 2 triangulation run.

> **⚠ DEC-030 (2026-08-12) was invalid:** compared A_v3_2ep against B_small trained on
> the old leaky split (3491/388). DEC-031 caught this; DEC-032 retrained B_small_clean
> on the correct split. Results below use verified clean embeddings.
>
> **Gate status:**
> - Invariant checker: 57/57 pass (0 failures)
> - Embedding shapes: all variants train=3375, eval=376 — matches datasets exactly
> - Invariant 5 (comparison validity): pass — A_v3_2ep and B_small_clean matched on
>   split (3375/376), epochs (2), size (~358–360M params)
> - Results file: `runs/h6_retrieval/results_clean.json`

---

## The comparison

- **A_v3_2ep:** IR per-operator, 2254-vocab, 359.9M params, 2 epochs, 3375/376, perplexity 1.2928
- **B_small_clean:** BPE 601-vocab, 358.4M params, 2 epochs, 3375/376, perplexity 1.1385
- **flat_clean:** flat-IR ablation, 11-vocab, 357.9M params, 2 epochs, 3375/376, perplexity 1.063

---

## Results — Mode 1 (eval → train)

| Variant | Recall@1 | Recall@5 | Recall@10 | Recall@20 | MRR |
|---|---|---|---|---|---|
| A_v3_2ep | 0.0009 [0, 0.0026] | 0.0009 [0, 0.0026] | 0.0052 [0, 0.0113] | 0.0096 [0.0033, 0.0168] | 0.0095 [0.0045, 0.0187] |
| B_small_clean | 0.0026 [0, 0.0070] | 0.0033 [0, 0.0078] | 0.0049 [0.0009, 0.0101] | 0.0070 [0.0026, 0.0126] | 0.0140 [0.0060, 0.0247] |
| flat_clean | 0.0009 [0, 0.0026] | 0.0009 [0, 0.0026] | 0.0035 [0.0004, 0.0079] | 0.0065 [0.0016, 0.0124] | 0.0091 [0.0043, 0.0171] |

n_queryable=287, pool_size=3375, 1000 bootstrap resamples.

**Primary gate:** ◐ Not met cleanly. A Recall@10=0.0052 vs B_small_clean=0.0049 — A
numerically ahead but CIs fully overlap. Not a statistically meaningful difference.

**Key shift from DEC-030:** original result showed B_small *beating* A (0.0064 vs 0.0052).
Against the correct B_small_clean comparator the direction flipped, but overlap is too
large to call.

---

## Mode 2 (train → train) — pending

Not yet run against clean embeddings. `results_v3_2ep_mode2.json` used old leaky B_small
and is invalid. Must run before H6 can close in either direction.

---

## Verdict

H6 cannot be closed yet. Mode 2 must be run against clean embeddings. If both modes
agree A ≥ B_small_clean, close as weak positive. If they disagree, result is ambiguous.

---

## What this does not settle

- **Mode 2:** must confirm direction before H6 closes
- **Scale (H4/H8):** toy scale only — IRCoder gains start at 1.1B
- **Objective (H5):** next-token prediction weights; contrastive objective untested
- **Co-training (H9):** joint IR + source training untested
- **ATP success (H7):** retrieval is a proxy, not a direct ATP test

---

## Files

- Results (clean, Mode 1): `runs/h6_retrieval/results_clean.json`
- Results (stale/invalid): `runs/h6_retrieval/results_v3_2ep.json`, `results_v3_2ep_mode2.json`
- Embeddings (clean): `runs/retrieval_embeddings_clean/`
- Checkpoint A: `runs/variant_A_v3_2ep/checkpoint-final`
- Checkpoint B_small_clean: `runs/variant_B_small/checkpoint-final` (retrained DEC-032)
