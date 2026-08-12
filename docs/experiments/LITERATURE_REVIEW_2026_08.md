# Literature Review — August 2026

> **Purpose.** A point-in-time literature review checking whether Maith's open
> sub-claims (per [`HYPOTHESIS_GRID`](HYPOTHESIS_GRID.md)) have been answered by
> existing research. Conducted 2026-08-12 against Google Scholar / arXiv / venue
> proceedings. Retained for historical value: subsequent experiments may close some
> of the open cells flagged here, but this record documents the state of the field
> *as Maith's experiment program understood it* at this date.
>
> **Companion to** [`../reference/PRIOR_ART.md`](../reference/PRIOR_ART.md). Where this
> review surfaces work that should be cited going forward, PRIOR_ART.md has been
> updated in the same change set. This document is the *review*; PRIOR_ART.md is the
> *living bibliography*.
>
> **Verdict summary.** Of Maith's open sub-claims, only one — H11's dual-evaluation
> methodology — has a direct precedent that materially narrows a novelty claim
> ("Meaning in Language Models," WFVML 2023). The representation-as-a-variable cell
> Maith occupies (canonical semantic IR vs. BPE, in formal mathematics) remains
> untested by the field across every metric family: prediction, retrieval, and ATP.
> The 2025–26 Lean retrieval wave confirms H6 as the most clearly-open and most
> active frontier.

---

## Summary table

| Sub-claim | Status after review | Key finding |
|---|---|---|
| H4/H8 (scale threshold) | Still open — and messier than prior framing | IRCoder gains are non-monotonic across 1.1B–7.3B; no clean threshold |
| H5 (objective redesign) | Partially answered in code/HOL domain; open for Lean semantic IR | Skip-tree (Rabe 2021) + PACT skip-proof already do masked-subterm prediction |
| H6 (retrieval/similarity) | Still open — the most active, untested gap | Every 2025–26 Lean retriever uses BPE/text; none uses a canonical semantic IR |
| H7 (proof completion / ATP) | Still open | All SOTA provers train on tactics/proof-state, not IR |
| H9 (co-training) | Answered in code domain; open for formal-math semantic IR | IRCoder & PACT both effectively co-train; gains confirmed |
| H10 (richer probing) | Open, but a direct methodological precedent exists (uncited pre-review) | "Meaning in Language Models" (WFVML 2023) does the same probe + perplexity-divergence methodology |
| H11 (perplexity validity) | Reinforced by new independent evidence | Formal-math survey + program-semantics probing both corroborate the dissociation |

---

## H4 / H8 — Does the IR advantage appear only above a scale threshold?

### Prior framing
[`PRIOR_ART`](../reference/PRIOR_ART.md) §2 and [`HYPOTHESIS_GRID`](HYPOTHESIS_GRID.md) H4/H8
frame IRCoder's positive result as appearing "at 1.1B+ parameters (small tier)."

### What the literature shows
IRCoder's per-model results table (arXiv:2403.03894) shows the gains are **not monotonic
with scale**, and there is no clean "above 1B" threshold:

| Model | Representative task deltas |
|---|---|
| IRCoder 1.1B | +0.88, +0.78, +1.88 (present) |
| IRCoder 1.3B | +3.58, +2.70, +4.45 (often largest) |
| IRCoder 3.1B | +0.23, +6.94, +0.47 (mixed) |
| IRCoder 7.3B | −0.13, −2.64, −1.39 (sometimes negative) |

IRCoder's own framing is that "the largest improvements observed for the strongest
Code-LMs" — referring to *base model quality*, not a clean parameter threshold, and
several 7.3B cells are negative. The authors note header-mangling gains are
"substantially smaller" due to benchmark artifacts.

A directly relevant graph-native result not previously cited: **Nazrin / ExprGraph**
(Aniva, Oikawa, Dill, Barrett; arXiv:2602.18767). A GNN-based Lean 4 theorem prover
that converts Lean expressions to a minimal graph ("ExprGraph") and consumes them
directly — no linearization to tokens. It argues expressions "can naturally and
compactly be represented as graphs" and that the GNN "exploits fundamental symmetries
of mathematics." This is the *aligned-architecture* case Maith's §8 theoretical
grounding predicts should work, now realized in Lean.

### Verdict
**H4/H8 remain open.** The literature does *not* establish a clean scale threshold —
IRCoder's gains are task-dependent and non-monotonic, and the prior "1.1B+ is where it
pays off" framing is an over-simplification of a messy empirical picture. The more
important untested dimension is now **architecture** (Nazrin's graph-native approach),
which the experiment-scope matrix flags as missing and which now has a concrete Lean
realization to compare against.

---

## H5 — Is the training objective the bottleneck?

### What the literature shows
The masked-reconstruction / proof-completion objective Maith proposes has substantial
direct precedent — partially traced in PRIOR_ART via PACT but not fully:

1. **Rabe, Lee, Bansal & Szegedy, "Mathematical reasoning via self-supervised
   skip-tree training" (ICLR 2021).** The origin of the **skip-tree task**: mask a
   subterm of a theorem statement and predict it. PACT (already cited) explicitly
   builds on this: "Unlike skip-tree training (Rabe et al., 2021), which focuses solely
   on predicting masked subterms of theorem statements, PACT derives its self-supervised
   training data from far more complex proofs."

2. **PACT's skip-proof task** (Han et al., ICLR 2022, already cited): "Given the partial
   proof term, predict the masked subterm τ′." Plus type prediction and next-lemma
   prediction. PACT co-trained these alongside tactic prediction and improved proving
   32%→48%.

3. **LeanTree** (arXiv:2507.14722, 2025): factorizes proof states by masking nested
   tactic blocks, yielding "simpler proof steps." A recent masked-subgoal decomposition
   objective.

### Verdict
**H5 is partially answered at the objective level, but open for Maith's specific
question.** The masked-subterm reconstruction objective *itself* has been tested —
skip-tree (Rabe 2021) and skip-proof (PACT) — and it helps. But these were applied to
**source/proof terms**, not to a **canonical semantic IR extracted from elaborated
Expr**. The specific open question — "would a masked-reconstruction objective reward
the IR's semantic structure that next-token loss doesn't?" — is **not answered**,
because no one has paired the masked-subterm objective with a canonical semantic IR as
the prediction target. H5 remains novel *as a representation×objective interaction*,
even though the objective alone is not new. PRIOR_ART should acknowledge skip-tree/
skip-proof as the objective precedent so the novelty claim is precise: it's the
*pairing*, not the objective.

---

## H6 — Does the IR improve retrieval / similarity tasks?

### What the literature shows
This is the most active 2025–26 front, and it makes H6 both the most important and most
clearly-open question. The Lean retrieval/premise-selection literature expanded rapidly:

| System | Year | Representation used | Tests canonical semantic IR? |
|---|---|---|---|
| ReProver (LeanDojo) | 2023 | BPE text embeddings | No |
| Piotrowski et al. | 2023 | Hand-crafted n-gram features (random forest) | No |
| Graph-augmented LeanDojo (arXiv:2510.23637) | 2025 | BPE text + RGCN over dependency graph | No — adds structural edges to text |
| LeanPremise / LeanHammer (arXiv:2506.07477) | 2025 | BPE encoder-only transformer, InfoNCE | No |
| LeanSearch v2 (arXiv:2605.13137) | 2026 | Hierarchy-informalized corpus + embedding–reranker | No — NL-informalized text |
| Lean Finder (arXiv:2510.15940, ICLR 2026) | 2026 | Dense embeddings + DPO alignment | No |
| LeanExplore (Asher, 2025) | 2025 | Dense + sparse + PageRank hybrid | No |
| LeanProgress (arXiv:2502.17925) | 2025 | DeepSeek-Coder 1.3B fine-tune | No |

**The pattern is striking and directly confirms H6 is open:** *every* 2025–26 Lean
retriever uses BPE/text embeddings, sometimes augmented with structural edges, and *none*
tests an elaboration-grounded canonical semantic IR as the retrieval representation. The
closest neighbor — the graph-augmented LeanDojo work (arXiv:2510.23637, already cited) —
*adds* an RGCN over dependency edges to BPE text and beats ReProver by 25%. That's
"text + structure," not "canonical IR replacing text."

LeanSearch v2 reports nDCG@10 of 0.62 vs. 0.53 for the next-best system *without
domain-specific fine-tuning*, using a "hierarchy-informalized Mathlib corpus." This
suggests representation/format choices in the retrieval index matter substantially —
which is exactly the variable Maith's H6 isolates, but no one has isolated *Maith's*
representation.

### Verdict
**H6 is open and is the most clearly-unanswered question in this review.** The retrieval
front is the field's most active area, and representation-as-a-variable is visibly
under-explored at exactly the axis Maith occupies (canonical semantic IR vs. BPE). The
DEC-025 `extract_representations.py` infrastructure is reusable here. This should be the
highest-priority next experiment — the literature is moving fast on retrieval and leaving
this exact cell empty.

### Adjacent evidence
A math-information-retrieval representation study (CEUR Vol-2696, paper_235) tested
LaTeX / MathML / Operator Tree / prefix / infix representations for *informal* math
retrieval and found prefix/infix notations improved accuracy, while "removing math
tokens" was worst. This is representation-as-a-variable for informal math (LaTeX),
adjacent to Maith's formal-math IR question, and supports H6's premise that
representation choice matters for math retrieval.

---

## H7 — Does the IR improve proof completion / ATP success?

### What the literature shows
The SOTA has moved to scale + RL, not representation:

- **Goedel-Prover-V2** (Princeton, Aug 2025): 88.1% MiniF2F pass@32, 80× smaller than
  competitors.
- **Kimina-Prover** (Moonshot, 2025): 92.2% MiniF2F, first to show clear performance
  scaling with model size.
- **BFS-Prover** (ByteDance Seed), **HunyuanProver** (Tencent), **SeedProver**
  (ByteDance), **Aristotle** (Harmonic): all RL-driven.
- **SorryDB** (arXiv:2603.02668, 2026): a benchmark of *real-world* Lean theorems (vs.
  competition-math-heavy MiniF2F), finding competition-trained provers struggle on real
  Mathlib proofs — a representation/distribution gap.
- **AlphaProof** (DeepMind, 2025, Nature): RL + Lean, IMO silver level.

All of these train on **tactics, proof states, or proof terms** — none on a canonical
semantic IR. The representation lever is untouched; the field pulled the *scale* and *RL*
levers instead.

The one exception is **Nazrin** (above): a GNN on ExprGraph for tactic prediction. But
Nazrin compares graph-native vs. autoregressive, not IR vs. BPE, and it is low-resource/
tactic-focused, not ATP-at-scale.

### Verdict
**H7 is open.** The ATP field has moved hard toward RL + scale and has *not* tested
representation-as-a-variable for downstream proving. SorryDB is a useful addition: it
shows competition-math proxies (MiniF2F) don't transfer to real Mathlib, reinforcing
Maith's decision to ground in real Mathlib declarations rather than synthetic/
competition data. H7 remains the actual downstream research claim and is untouched.

---

## H9 — Would co-training (IR alongside source) recover gains?

### What the literature shows
Co-training / multi-view IR+source is established and effective in the code domain:

- **IRCoder** effectively co-trains: it concatenates source + IR and does causal LM over
  both, "aligning the IR constructs with respective constructs of various programming
  languages." Gains are consistent.
- **PACT** (already cited): co-trains structural objectives (skip-proof, type
  prediction) alongside tactic prediction; 32%→48%.
- The broader code-IR survey literature explicitly states: "Multi-view or paired
  training (using IRs and source code) improves robustness and cross-lingual semantic
  grasp (Paul et al. 2024, Li et al. 2022, Hasija et al. 2023)."

### Verdict
**H9 is answered in the code domain but open for formal-math semantic IR.** The design
lesson (co-train, don't replace) is well-supported — PACT and IRCoder both confirm it.
What's untested is whether co-training *Maith's canonical semantic IR* alongside Lean
source recovers IRCoder-style gains in the formal-math domain. PRIOR_ART already flags
this correctly as a candidate direction. The novelty is the *domain + representation
combination*, not the co-training idea itself.

---

## H10 — Does the IR beat AST on richer semantic probing?

### What the literature shows — a direct precedent not previously cited
**"Meaning in Language Models: A Formal Semantics..."** (WFVML 2023) is the closest
methodological analog to DEC-025 — and was not cited in PRIOR_ART pre-review. It does
*exactly* Maith's dual-evaluation methodology, in a program-synthesis setting:

- Trains a Transformer LM on program traces (a formal/program-semantic domain, like
  Maith's Lean IR).
- Trains a **linear probe** to extract the "semantic state" (facing direction) from the
  LM's hidden states.
- Finds the semantic state **emerges as a byproduct of next-token prediction** — the
  probe recovers it.
- Explicitly notes: "BLEU score... is not a good predictor of quality, which complements
  our results regarding perplexity on the training corpus," and shows the **divergence
  between perplexity/loss and semantic content**.

This is Maith's DEC-024/DEC-025 dissociation (semantics encoded but unrewarded by
prediction) demonstrated independently in a different formal domain — published *before*
this review. PRIOR_ART §6 previously claimed the dual-evaluation methodology as
"genuinely Maith's own" (contribution #1). This paper weakens that novelty claim: the
perplexity-vs-probe dissociation in a formal-semantics LM setting was demonstrated first
here. It should be cited and the novelty claim narrowed — Maith's contribution is
demonstrating the dissociation *for a canonical semantic IR in formal mathematics
specifically*, not the dual-evaluation methodology itself.

Adjacent probing work:
- **"LLM Knowledge is Brittle"** (arXiv:2510.11905): probes truthfulness
  representations; finds linear probes degrade OOD as measured by perplexity — uses
  perplexity as an OOD metric, tangential to Maith's question.
- **OthelloGPT / chess-GPT linear-probe work** (cited via "From Tokens to States"):
  world-state structure encoded linearly in hidden activations, not tokens. Same
  dissociation family.

### Verdict
**H10 is open at the Maith-specific level, but the methodology has a direct precedent
that must be cited.** The "Meaning in LMs" paper is the most important addition to
prior art from this review — it's Maith's methodology with the same finding, in a sibling
domain. The richer-probe experiments (typeclass arity, theorem-vs-definition, semantic
category) remain untested.

---

## H11 — Is perplexity a valid primary metric?

### What the literature shows — strong new independent support
Maith's H11 "perplexity is structurally biased toward redundancy" argument gets three
independent reinforcements:

1. **"Meaning in Language Models" (WFVML 2023)** — directly demonstrates perplexity/
   BLEU diverging from semantic content in a formal-semantics LM. Empirical, not just
   theoretical. The strongest direct support.

2. **"Thinking Machines: Mathematical Reasoning in the Age of LLMs"** (MDPI, 2025
   survey) — on formal mathematics specifically: "the space of valid expressions is far
   narrower, and even small deviations can render a proof completely invalid. For a
   language model trained to predict the next token, this rigidity is difficult to
   internalize without external feedback." An independent, domain-specific statement of
   Maith's structural-bias argument: formal math's canonicality (narrow
   valid-expression space) is intrinsically hard for next-token objectives.

3. **Canonicality-at-the-tokenization-level work**: "Canonical Autoregressive
   Generation" (arXiv:2506.06446) and "Language Models over Canonical Byte-Pair
   Encodings" (ICML 2025) — these prove non-canonical token sequences "waste modeling
   power" and "divert probability mass away from plausible outputs." This is about
   *tokenization* canonicality (BPE strings), not *representation* canonicality (Maith's
   IR). A nuance worth noting: canonicalizing the *tokenization* layer helps perplexity
   (fewer wasted strings); canonicalizing the *semantic* layer hurts it (removes
   redundancy). Different layers, opposite effects — and the contrast clarifies that
   Maith's H11 claim is specifically about *semantic* canonicalization, not tokenization
   canonicality.

### Verdict
**H11's core finding (perplexity ≠ representation quality) is reinforced, not refuted.**
The "Meaning in LMs" paper is the key addition — it is empirical prior art for the
dissociation. Maith's two-component framing (fundamental vs. contingent) stands. The
contingent component (could a v3 IR with semantic redundancy narrow the gap?) remains
open and untested.

---

## Other notes for prior art

- **DeepSeek-Prover V2** (arXiv:2504.21801, 2025): PRIOR_ART cites V1; V2 uses RL for
  subgoal decomposition. Minor reference update.
- **LeanDojo Benchmark 4** (122,517 theorems / 259,580 tactics / 167,779 premises from
  Mathlib4): an expanded benchmark over the 99K-theorem v3 cited in PRIOR_ART. Relevant
  if the corpus is expanded (experiment-scope "corpus size" dimension).

---

## What this means for next steps

1. **Update PRIOR_ART with three additions** (done in this change set): (a) "Meaning in
   Language Models" (WFVML 2023) — narrows the dual-evaluation novelty claim; (b)
   Nazrin/ExprGraph (arXiv:2602.18767) — the graph-native Lean precedent for the
   architecture dimension; (c) the 2025–26 Lean retrieval wave (LeanSearch v2, Lean
   Finder, LeanPremise, LeanProgress) — establishes H6 as the clearly-open cell.

2. **Sharpen H4/H8 framing** (done in this change set): drop the "IRCoder gains appear
   at 1.1B+" simplification — the gains are non-monotonic. The honest framing is "IR
   grounding shows task-dependent, non-monotonic gains across 1.1B–7.3B in code; no
   clean threshold is established, and it has never been tested in formal math at any
   scale."

3. **Acknowledge H5's objective precedent** (done in this change set): skip-tree (Rabe
   2021) and skip-proof (PACT) already do masked-subterm prediction. H5's novelty is
   the *pairing* of that objective with a canonical semantic IR, not the objective
   itself.

4. **Priority remains H6 then H10**: H6 is the most clearly-open and most active field
   frontier; H10 has a methodological precedent to cite but the richer probes are
   untested. Both are tractable on current hardware, consistent with
   [`V2_NEXT_STEPS`](V2_NEXT_STEPS.md).

5. **The "Meaning in LMs" paper is the one finding that materially changes Maith's
   novelty claims** — it's the methodology + finding in a sibling domain, pre-dating
   Maith's work. Engage with it honestly in PRIOR_ART §6 before any publication.

---

## References added by this review

- Aniva, L., Oikawa, I., Dill, D., & Barrett, C. (2026). Nazrin: Atomic Tactics for
  Graph Neural Networks for Theorem Proving in Lean 4. arXiv:2602.18767.
- Rabe, M. N., Lee, D., Bansal, K., & Szegedy, C. (2021). Mathematical reasoning via
  self-supervised skip-tree training. *ICLR 2021*.
  OpenReview: https://openreview.net/forum?id=YmqAnY0CMEy
- "LeanTree: Accelerating White-Box Proof Search with Factorized States in Lean 4."
  arXiv:2507.14722 (2025).
- "LeanSearch v2: Global Premise Retrieval for Lean 4 Theorem Proving." arXiv:2605.13137
  (2026).
- "Lean Finder: Semantic Search for Mathlib That Understands User Intents."
  arXiv:2510.15940 (ICLR 2026).
- "Premise Selection for a Lean Hammer." arXiv:2506.07477 (2025).
- "LeanProgress: Guiding Search for Neural Theorem Proving via Proof Progress
  Prediction." arXiv:2502.17925 (2025).
- "SorryDB: Can AI Provers Complete Real-World Lean Theorems?" arXiv:2603.02668 (2026).
- "Meaning in Language Models: A Formal Semantics..." WFVML 2023.
  (ETH, paper_26.pdf.)
- "Thinking Machines: Mathematical Reasoning in the Age of LLMs." MDPI AI 2025.
- "Canonical Autoregressive Generation." arXiv:2506.06446 (2025).
- "Language Models over Canonical Byte-Pair Encodings." ICML 2025.
  https://icml.cc/virtual/2025/poster/44596
- "LLM Knowledge is Brittle: Truthfulness Representations Rely on Superficial
  Resemblance." arXiv:2510.11905 (2025).
- Math information retrieval representation study. CEUR Vol-2696, paper_235.
- Goedel-Prover-V2 (Princeton, 2025); Kimina-Prover (Moonshot, 2025); AlphaProof
  (DeepMind, 2025, Nature).
- LeanDojo Benchmark 4 (122,517 theorems / 259,580 tactics / 167,779 premises,
  Mathlib4).
