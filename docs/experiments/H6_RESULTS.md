# H6 Retrieval Results — A_v3_2ep vs B_small

> **Date:** 2026-08-12
> **DEC:** DEC-030
> **Status:** ❌ Closed (negative) — the IR does not improve retrieval over size-matched BPE at toy scale, under the prediction-trained objective.

---

## The comparison

This is the clean, epoch-matched retest of the H6 retrieval experiment. The initial run was
invalid (dataset contamination, epoch confound — see `H6_RETEST_PLAN.md`). This run fixes
both:

- **A_v3_2ep:** per-operator IR (gen:FullName tokens), 2254-vocab, 359.9M params, 2 epochs,
  clean 3375/376 split, perplexity 1.2928
- **B_small:** BPE truncated to 601-vocab, 358M params, 2 epochs, same split

Both variants share the same train/eval split (3375/376), the same epochs (2), and are
size-matched (~359M). The comparison is methodologically valid.

## Results

### Mode 1 (eval → train) — the headline test

| Variant | Recall@1 | Recall@5 | Recall@10 | Recall@20 | MRR |
|---|---|---|---|---|---|
| A_v3_2ep | 0.0009 [0, 0.0026] | 0.0009 [0, 0.0026] | 0.0052 [0.0009, 0.0113] | 0.0096 [0.0035, 0.0168] | 0.0095 [0.0046, 0.0187] |
| B_small | 0.0026 [0, 0.0078] | 0.0042 [0.0007, 0.0087] | 0.0064 [0.0017, 0.0122] | 0.0111 [0.0037, 0.0212] | 0.0150 [0.0066, 0.0256] |

n_queryable = 287, pool_size = 3375, 1000 bootstrap resamples.

**Primary gate (A Recall@10 > B_small Recall@10 by a meaningful margin):** ❌ Not met.
A_v3_2ep Recall@10 = 0.0052 vs B_small Recall@10 = 0.0064. The CIs overlap
([0.0009, 0.0113] vs [0.0017, 0.0122]). A is numerically *below* B_small.

### Mode 2 (train → train) — triangulation

| Variant | Recall@1 | Recall@5 | Recall@10 | Recall@20 | MRR |
|---|---|---|---|---|---|
| A_v3_2ep | 0.0001 [0, 0.0004] | 0.0008 [0.0002, 0.0014] | 0.0012 [0.0006, 0.0019] | 0.0034 [0.0020, 0.0050] | 0.0051 [0.0040, 0.0064] |
| B_small | 0.0001 [0, 0.0004] | 0.0005 [0.0001, 0.0010] | 0.0028 [0.0016, 0.0041] | 0.0044 [0.0029, 0.0061] | 0.0062 [0.0052, 0.0074] |

n_queryable = 2641, pool_size = 3375.

**Mode 2 agrees with Mode 1:** A_v3_2ep does not beat B_small. CIs overlap on every
metric. B_small is consistently numerically higher.

## Verdict

**H6 is closed (negative) at toy scale under the prediction-trained objective.** The
canonical semantic IR (per-operator, gen:FullName) does not produce more useful retrieval
embeddings than size-matched BPE (B_small) for dependency/premise retrieval. Both modes
agree, so the result is robust to the eval-split choice.

This is the same null as the confounded run (DEC-029 era results). The epoch confound was
not the explanation — the clean 2-epoch comparison reproduces the null.

## The confound to acknowledge (§5.4 of the scope)

A's IR tokens contain dependency names as literal tokens (`gen:AddMonoid` is a token in
the declaration that depends on `AddMonoid`). BPE embeddings encode surface substrings,
which include the name but not as a discrete semantic unit. This means the test is
*somewhat loaded in IR's favor* for directly-referenced dependencies: A can retrieve deps
partly because the dep name is a clean token, while B must match surface substrings.

**A positive would have been real but should have been read with this grain of salt.** A
*negative* — which is what we observe — is a strong signal: if the IR can't win even with
the dep name as a clean token, the representation isn't producing useful semantic
embeddings for retrieval under the next-token prediction objective.

## What this does not settle

- **Scale (H4/H8):** same 0.5B model, same 3.5K corpus. IRCoder's positive results start at
  1.1B. The null is settled at toy scale only.
- **Objective (H5):** the weights are trained with next-token prediction. A retrieval-specific
  objective (e.g., contrastive) might produce different embeddings from the same checkpoint.
- **Co-training (H9):** joint IR + source training might recover retrieval gains where
  replacement didn't (PACT precedent).
- **ATP success (H7):** retrieval is a proxy for premise selection, not a direct ATP test.

## Files

- Results: `runs/h6_retrieval/results_v3_2ep.json` (Mode 1), `results_v3_2ep_mode2.json` (Mode 2)
- Checkpoint: `runs/variant_A_v3_2ep/checkpoint-final`
- Embeddings: `runs/retrieval_embeddings/embeddings_A_v3_2ep_{train,eval}.pt`
- Training log: `runs/train_A_v3_2ep.log`
