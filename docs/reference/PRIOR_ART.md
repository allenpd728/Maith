# Prior Art and Related Work

> **Scope.** This document surveys the research landscapes Maith intersects, identifies the
> closest prior art, and states honestly where Maith converges with existing work versus
> where it occupies a gap. It is written to accompany the experimental record in
> [`docs/decisions/LOG.md`](../decisions/LOG.md); decision IDs (DEC-0xx) refer to entries there.

## 1. Maith's position in one paragraph

Recent progress in neural theorem proving has focused on two levers: model scale and data
quantity (e.g. ReProver's ~99K theorems, LeanNavigator's 4.7M, DeepSeek-Prover's RL
training). **In formal mathematics / theorem proving, everyone trains on
source/tactics/state and scales data. Maith is the only project systematically testing
whether the *representation* (a canonical semantic IR vs. raw source) is the bottleneck.**
(IRCoder and FAIR test the same hypothesis in the *programming-language* domain — see
§2 — and find it holds; Maith tests it for formal math and finds it does not hold at this
scale.) That is the gap this project fills, and the experimental record to date is a
nuanced negative result. Across the full control grid (DEC-026/027), a
semantic IR extracted from elaborated Lean terms *encodes* semantics (DEC-025: 75.4%
linear-probe module-classification accuracy vs. 13.6% for a shape-only ablation, a 62-point
gap) but does *not* improve next-token prediction or completion accuracy over a
size-matched BPE baseline — even after the cold-start embedding confound is ruled out
(DEC-021) and the model-size confound is controlled (DEC-027: B-small at matched 358M /
601-vocab achieves 90.5% top-1 vs. A's 90.0%). The interpretation: the IR encodes real
semantic structure, but that structure is not rewarded by the next-token objective at this
scale. This dissociation lands inside an active, contested debate in the broader ML
literature, mapped below.

> **Epoch confound note.** The B (91.7%) and C (93.0%) figures are 3-epoch v1-era runs,
> while A (90.0%) and B-small (90.5%) are 2-epoch v2 runs. The A-vs-B-small comparison is
> epoch-matched and is the clean representation test; the A-vs-B/C comparison is not. If
> B/C are re-run at 2 epochs, these numbers will change and this document will need
> updating. See [`V2_COMPARISON_MATRIX`](../experiments/V2_COMPARISON_MATRIX.md) for per-run
> epoch counts.

## 2. Code intermediate representations — the closest prior art, and the opposite result

The single most relevant line of work applies Maith's thesis to *programming languages*
rather than formal mathematics: ground a language model in a compiler's intermediate
representation rather than surface syntax, on the hypothesis that the IR exposes semantic
structure the surface hides.

### IRCoder / SLTrans (Paul et al., ACL 2024; arXiv:2403.03894)

IRCoder compiles a parallel dataset of ~4M source files paired with their LLVM IR
(`SLTrans`), then continued-pretrains six code LMs (StarCoderBase, DeepSeek-Coder,
CodeLlama; 1.1B–7.3B parameters) on source↔IR pairs. **Result: consistent gains across
prompt robustness, multilingual code completion, code understanding, and instruction
following.**

This is the same thesis as Maith, tested in a different domain, with a positive outcome.
The contrast is the contribution-relevant fact:

| | IRCoder | Maith |
|---|---|---|
| Domain | Programming languages | Formal mathematics (Lean/Mathlib) |
| IR type | Existing compiler IR (LLVM) | Designed semantic graph from elaborated `Expr` |
| Scale | ~4M files, 1.1B–7.3B models | ~3.5K examples, 365M model |
| Outcome | Positive (gains across tasks) | Null-to-negative on perplexity; positive on probing |

Two non-exclusive readings of the contrast, both open in Maith's decision log:

1. **Scale.** IRCoder's gains appear at 1.1B+ parameters on millions of examples. Maith's
   365M / 3.5K may be below the regime where IR grounding pays off. This is DEC-024's
   open question (a): would semantic content help with 10K+ examples?
2. **IR type.** LLVM IR is a lossy, operational lowering designed to be compact and
   regular. Maith's IR is a semantic graph that preserves more structure but is harder to
   predict token-by-token. DEC-024's flat-IR ablation supports this: the semantic tokens
   add prediction entropy without commensurate signal under next-token loss.

### Related code-IR line

- **FAIR — Flow Type-Aware Pre-Training of Compiler IRs** (Niu et al., 2023;
  arXiv:2309.04828). Graph Transformer over IR with flow-type pretraining tasks;
  state-of-the-art on code tasks. Explicitly notes that "IR-specific training may be
  essential to close the gap between source-level and IR-level reasoning" — directly
  relevant to Maith's A-vs-B/C gap.
- **IR2Vec** (VenkataKeerthy et al., 2020). Hierarchical vector representations of LLVM IR.
- **Meta LLM Compiler** (Cummins et al., 2024). Pretrained IR models for code optimization.
- **AST-T5** (Gong et al., 2024). Structure-aware pretraining over ASTs. Analogous to
  Maith's Variant C (AST-split `leanExpr`).
- **"Can LLMs Understand Intermediate Representations in Compilers?"** (arXiv:2502.06854,
  2025). Finds Code Llama leads on source (131 pass@1) while IR-specific training is
  needed to close the IR-level reasoning gap — mirrors Maith's A-trails-B/C finding.

IRCoder and FAIR are the positive-control counterparts to Maith's negative result and
belong at the center of any related-work claim. The honest framing: Maith tests the
IRCoder thesis in the formal-math domain and finds it does not hold at this scale;
whether the cause is domain, IR type, or scale remains open.

## 3. Lean/math theorem-proving LLMs — the domain Maith complements

This line trains on Lean *source, tactics, or proof state*, not a semantic IR. Maith is
the representation-research complement to it.

- **ReProver / LeanDojo** (Yang et al., NeurIPS 2023). Tactic generation augmented with
  premise retrieval; ~99K Mathlib theorems. The retrieval-augmented baseline.
- **LeanNavigator** (arXiv:2503.04772, 2025). 4.7M theorems with proofs via state-transition
  graph exploration; outperforms prior SOTA. The "just add data" approach at scale.
- **DeepSeek-Prover V1/V2** (Xin et al., 2024/2025). V2 uses reinforcement learning for
  subgoal decomposition.
- **PACT — Proof Artifact Co-training** (Han, Rute, Wu, Ayers, Polu; ICLR 2022;
  arXiv:2102.06203). Extracts self-supervised data from kernel-level proof terms and
  co-trains alongside tactic prediction; improved proving success 32%→48%. **PACT is
  Maith's closest methodological kin** — both extract training signal from
  elaborated/kernel-level structure rather than surface source. The key design difference:
  PACT *co-trains* the structural objective alongside the source objective rather than
  *replacing* source with IR. PACT's gains where Maith's replacement does not is a real
  design lesson worth carrying forward (see §7 and the candidate direction noted in
  [`V2_NEXT_STEPS.md`](../experiments/V2_NEXT_STEPS.md)).
- **Lean Copilot** (Song et al., 2024). Human-in-the-loop LLM proof assistance.
- **Lean Workbook** (arXiv:2406.03847, 2024). Synthetic NL↔Lean formalization pipeline.

The pattern across this literature: scale data and models, keep the representation as
source/tactics/state. Maith is the systematic test of the representation variable that
this literature leaves unexamined.

## 4. JEPA and the tokens-vs-representation debate — Maith's results are evidence here

### LeCun (2022), "A Path Towards Autonomous Machine Intelligence" (OpenReview position paper)

The position paper that frames the broader debate. LeCun argues that generative prediction
in observation/token space is the wrong objective for learning useful representations: the
model wastes capacity on surface detail and misses structure. Proposes JEPA — predict in
*latent* space, not token space — and explicitly contrasts "word models" with "world
models."

**Maith's DEC-024/DEC-025 result pair is consistent with the LeCun position,
demonstrated in a new domain.** The probing result (Variant A encodes semantics, 75.4%)
alongside the perplexity result (Variant A is penalized for semantics, +0.27 bits/token)
shows precisely the dissociation LeCun predicts: token-prediction quality ≠ representation
quality. Maith arrived at this dissociation empirically through ablation and probing;
JEPA arrives at it architecturally. **This is an interpretive connection, not a proven
link:** Maith does not test a JEPA architecture, and the dissociation could have causes
beyond the one LeCun identifies (e.g. data volume, IR token format). The convergence is
worth noting as compatible evidence, not as validation of the JEPA position.

### The JEPA family — operational instantiations

- **I-JEPA** (Assran et al., CVPR 2023). Image self-supervised learning via masked latent
  prediction; matched masked-autoencoder quality with no pixel-level loss.
- **V-JEPA** (Bardes et al., ICLR 2024). Video; learns motion structure without
  reconstruction.
- **V-JEPA 2** (Assran et al., 2025). Internet-scale video JEPA post-trained as an
  action-conditioned latent world model for robot planning. The "JEPA as world model"
  milestone.
- **LeJEPA / LeWorldModel** (Balestriero & LeCun, 2025; Maes et al., 2026). Removes the
  heuristic anti-collapse tricks (EMA target encoder, VICReg) via SIGReg, an isotropic
  Gaussian objective.

The JEPA line validates, architecturally, the claim that prediction-in-representation-space
captures structure that token-prediction does not. Maith's contribution is the
empirical observation that this dissociation also appears *within* a token-prediction
model: the structure is present in the hidden representations (DEC-025) but unrewarded by
the next-token objective (DEC-024).

### Tokens-vs-states framing

- **"From Tokens to States: LLMs as a Special Case of World Models"** (arXiv:2606.28127,
  2026). Argues LLMs are a degenerate special case of world models (state space = token
  sequences, action = append token) and cites OthelloGPT/chess models showing world-state
  structure is encoded *linearly in hidden activations, not in tokens*. This is the same
  dissociation Maith's DEC-025 probing result demonstrates for Lean: the tokens are the
  action space; the semantic model lives in the hidden states.

## 5. Semantic representations and interlingua — the classical ancestors

### Abstract Meaning Representation (AMR, Banarescu et al., 2013)

Rooted directed graphs encoding sentence-level semantics (concepts as nodes, relations as
edges), designed to be language-neutral. Has supervised parse (text→AMR) and generate
(AMR→text) tasks. **AMR is the natural-language analog of Maith's IR graph.** Maith's
round-trip validation (2,554/2,554 declarations through encode→decode→decompile; see
[`docs/reference/EXAMPLE_ROUNDTRIP.md`](EXAMPLE_ROUNDTRIP.md)) is the formal-math
equivalent of AMR's parse/generate round-trip. A 2025 survey (arXiv:2505.03229) notes AMR
is increasingly explored for integration into LLMs to improve the semantic quality of
generated text — the same hypothesis space Maith tests for mathematics.

### Neural interlingua

- **Google GNMT zero-shot** (Schuster et al., 2016). Multilingual NMT spontaneously
  developed a language-independent intermediate representation enabling unseen language
  pairs (Japanese↔Korean). The canonical instance of an "agnostic central representation";
  notably *latent and internal*, not human-readable.
- **Variable-length neural interlingua** (Mao et al., 2023). Explicit neural interlingua
  module between encoder and decoder for zero-shot MT; argues fixed-length interlingua
  limits representation ability.
- **UNL (Universal Networking Language)**. The serious symbolic attempt at a universal,
  language-neutral representation; largely abandoned because designing a representation
  that captures everything proved as hard as the translation itself.

The lesson this literature holds for Maith: explicit, human-readable interlinguas have a
history of encoding semantics well but underperforming implicit/latent alternatives on the
prediction task. Maith's IR is an explicit, human-readable representation (closer to
UNL/AMR than to GNMT's latent space), and Maith's results echo the historical pattern —
the explicit representation encodes semantics (DEC-025) but does not beat the implicit
alternative at next-token prediction (DEC-021/024).

## 6. Where Maith is distinct

Despite the convergences above, two things are genuinely Maith's own:

1. **Dual-evaluation methodology (perplexity + probing).** IRCoder, FAIR, and the
   Lean-prover line evaluate with task metrics alone. Maith's DEC-024/DEC-025 pairing — a
   flat-IR ablation that isolates structure from semantics, followed by a linear probe
   testing whether semantics is *encoded* despite not helping *prediction* — is a
   methodological choice that surfaces a dissociation the prior art does not typically
   measure. It motivates (but does not prove) the interpretation that the null result
   reflects an objective-mismatch rather than a representation failure.

2. **Math-domain IR designed from elaborated terms.** Code-IR work uses existing compiler
   IRs (LLVM). Lean-prover work uses source. Maith designs a *new* semantic IR extracted
   from the elaborator's ground-truth semantics (`MetaExtractor.lean` over elaborated
   `Expr`) — the "elaborator as oracle" move. No prior art does exactly this for Lean.
   AMR performs the analogous extraction for natural language, but via manual annotation
   rather than an oracle.

A note on what is *not* claimed here: the controlled experiments that make Maith's null
result interpretable (DEC-021 isolating cold-start, DEC-027 isolating model size) are
valuable for *experimental validity* — they distinguish "the IR representation is not
helping" from "a confound is masking a benefit" — but confound isolation is good practice,
not a research contribution, and is not claimed as novelty.

## 7. Open questions the prior art does not resolve for Maith

- **Scale regime.** IRCoder's positive result is at 1.1B+ parameters and millions of
  examples. Whether Maith's null holds at larger scale (the A-large cell in the
  [`V2_COMPARISON_MATRIX`](../experiments/V2_COMPARISON_MATRIX.md), unrun) remains the most
  important open scale question. DEC-027 resolved the *parameter-count* confound at 358M
  (B-small vs. A), but did not test whether IR grounding pays off at IRCoder-scale model
  size.
- **Training objective.** DEC-027's interpretation points at objective mismatch rather
  than representation failure: the IR encodes semantics (DEC-025) but next-token
  prediction does not reward it. Whether a masked-reconstruction or proof-completion
  objective would recover an IR advantage is untested and is the most direct next lever
  flagged by the experimental record.
- **Co-training vs. replacement.** PACT co-trains structural signal alongside source and
  gains; Maith replaces source with IR and does not. Whether a co-training design would
  recover IRCoder-style gains for Maith's IR is untested.
- **Corpus volume.** All Maith results are at ~3.5K examples. IRCoder's gains appear at
  millions of files. Corpus expansion beyond 10K examples is an open lever; v2's 601-token
  vocabulary is well-suited to scale.
- **Task-metric evaluation.** Maith's primary results are perplexity, completion accuracy,
  and probing. The downstream claim — better theorem prediction, proof search, proof
  completion — requires the theorem-proving evaluation scaffolded in
  [`docs/history/PHASE_7_ROADMAP.md`](../history/PHASE_7_ROADMAP.md) but not yet run.

## 8. References

- Assran, M., Duval, Q., Misra, I., Bojanowski, P., Vincent, P., Rabbat, M., LeCun, Y., &
  Ballas, N. (2023). Self-Supervised Learning from Images with a Joint-Embedding Predictive
  Architecture (I-JEPA). *CVPR 2023*. arXiv:2301.08243.
- Banarescu, L., et al. (2013). Abstract Meaning Representation (AMR) 1.0 specification.
  *Parsing na Noite da Lua*.
- Bardes, A., et al. (2024). V-JEPA: Latent Video Prediction for Visual Representation
  Learning. *ICLR 2024*.
- Cummins, C., et al. (2024). Meta Large Language Model Compiler.
- Gong, L., Elhoushi, M., & Cheung, A. (2024). AST-T5: Structure-Aware Pretraining for
  Code Generation and Understanding. arXiv:2401.03003.
- LeCun, Y. (2022). A Path Towards Autonomous Machine Intelligence. OpenReview position
  paper. https://openreview.net/forum?id=BZ5a1r-kVsf
- Mao, Z., Song, H., Dabre, R., Chu, C., & Kurohashi, S. (2023). Variable-length Neural
  Interlingua Representations for Zero-shot Neural Machine Translation. *Multi3Generation*.
- Niu, Y., et al. (2023). FAIR: Flow Type-Aware Pre-Training of Compiler Intermediate
  Representations. arXiv:2309.04828.
- Paul, I., Glavaš, G., & Gurevych, I. (2024). IRCoder: Intermediate Representations Make
  Language Models Robust Multilingual Code Generators. *ACL 2024*. arXiv:2403.03894.
  (SLTrans dataset.)
- Han, J. M., Rute, J., Wu, Y., Ayers, E., & Polu, S. (2022). Proof Artifact Co-training
  for Theorem Proving with Language Models (PACT). *ICLR 2022*. arXiv:2102.06203.
- Schuster, M., Johnson, M., & Thorat, N. (2016). Zero-Shot Translation with Google's
  Multilingual Neural Machine Translation System. Google Research Blog.
- Song, P., Yang, K., & Anandkumar, A. (2024). Lean Copilot: Large Language Models as
  Copilots for Theorem Proving in Lean. arXiv:2404.12534.
- VenkataKeerthy, S., et al. (2020). IR2Vec: LLVM IR Based Program Embeddings.
- Xin, H., et al. (2024). DeepSeek-Prover: Advancing Theorem Proving via Large Language
  Models. (V2: arXiv:2504.21801, 2025.)
- Yang, K., Sui, Y., He, N., & Anandkumar, A. (2023). LeanDojo: Theorem Proving with
  Retrieval-Augmented Language Models (ReProver). *NeurIPS 2023 Datasets and Benchmarks*.
- "Can Large Language Models Understand Intermediate Representations in Compilers?"
  arXiv:2502.06854 (2025).
- "From Tokens to States: LLMs as a Special Case of World Models and the Continuous Path
  Beyond." arXiv:2606.28127 (2026).
- Survey of Abstract Meaning Representation: Then, Now, Future. arXiv:2505.03229 (2025).
