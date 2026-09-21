# Prior Art and Related Work

> **Scope.** This document surveys the research landscapes Maith intersects, identifies the
> closest prior art, and states honestly where Maith converges with existing work versus
> where it occupies a gap. It is written to accompany the experimental record in
> [`docs/decisions/LOG.md`](../decisions/LOG.md); decision IDs (DEC-0xx) refer to entries there.
>
> **Two tracks.** §1–8 cover the IR/training track (shelved as a research direction,
> DEC-036; last revised 2026-08-16 — the DEC-031/032 invalidation caveat in §1 still
> applies to those numbers). **§9 covers the active track, axiom discovery**, and was added
> 2026-09-18 because the active track had inherited no grounding from the rest of the
> document. If you are working on axiom discovery, read §9 first; §1–8 are background.

## 1. Maith's position in one paragraph

Recent progress in neural theorem proving has focused on two levers: model scale and data
quantity (e.g. ReProver's ~99K theorems, LeanNavigator's 4.7M, DeepSeek-Prover's RL
training). Representation-as-a-variable has been studied in formal math, but along a
narrower axis: prior work tests **structural-format variants** of a given representation
(tree-with-names vs. graph-without-names; text-only vs. dependency-graph) on
retrieval/premise-selection tasks (Wang et al., 2017; Paliwal et al., 2020; the 2025
textual+structural Lean premise-selection work — see §3). **Maith tests something narrower
and different: whether a *canonical semantic graph IR extracted from elaborated Lean `Expr`*** —
a representation that resolves implicits, canonicalizes ordering, and exposes
declaration-level semantics — improves *prediction* and *retrieval* over a raw BPE
baseline, with size-matched controls. The closest analogues test format variants, not
elaboration-grounded semantic canonicalization, and they do so on retrieval, not
prediction. (IRCoder and FAIR test the same thesis in the *programming-language* domain —
see §2 — and find it holds; Maith tests it for formal math and finds it does not hold at
this scale.) That is the gap this project fills, and the experimental record to date is a
nuanced negative result. Across the full control grid (DEC-026/027), a
semantic IR extracted from elaborated Lean terms *encodes* semantics (DEC-025: 75.4%
linear-probe module-classification accuracy vs. 13.6% for a shape-only ablation, a 62-point
gap) but does *not* improve next-token prediction or completion accuracy over a
size-matched BPE baseline — even after the cold-start embedding confound is ruled out
(DEC-021) and the model-size confound is controlled (DEC-027: B-small at matched 358M /
601-vocab achieves 90.5% top-1 vs. A's 90.0%).

> **⚠ These numbers are from invalidated runs (DEC-031, 2026-08-12).** They are
> retained as the historical record of the finding's logic but are NOT current evidence.
> Clean retrains are complete; hypothesis tests pending under verified quality gates.

The interpretation: the IR encodes real
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
| Scale | ~4M files, 1.1B–7.3B models (small tier) | ~3.5K examples, 358M model (toy tier) |
| Outcome | Positive (gains across tasks) | Null-to-negative on perplexity; positive on probing |

Two non-exclusive readings of the contrast, both open in Maith's decision log:

1. **Scale.** IR grounding shows task-dependent, non-monotonic gains across
   1.1B–7.3B parameters in code — no clean threshold is established (several 7.3B cells
   in IRCoder's results table are negative), and it has never been tested in formal math
   at any scale. Maith's 358M / 3.5K (toy tier) may be below the regime where IR
   grounding pays off — and the gap is in *transformer capacity*, not just
   embedding-table size (see
   [`EXPERIMENT_DESIGN`](../experiments/EXPERIMENT_DESIGN.md#model-size-taxonomy)).
   This is DEC-024's open question (a): would semantic content help with 10K+ examples?
   See [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md) §H4/H8
   for the full non-monotonicity analysis.
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

### Representation-as-a-variable in formal math — the closest lineage to Maith

The above systems keep the representation as source/tactics/state. A separate, smaller
lineage *does* vary the representation and measure the effect — and it is the closest
prior art to Maith's representation comparison. None of it, however, tests
elaboration-grounded canonical semantic graph IR vs. raw BPE, which is Maith's specific
contribution.

- **Wang et al., "Premise Selection for Theorem Proving by Deep Graph Embedding"
  (NeurIPS 2017; arXiv:1709.09994).** Builds a graph representation of first-order
  formulas and runs an ablation comparing representation variants — tree-with-names vs.
  graph-without-names vs. their full FormulaNet graph — on premise selection. This is a
  controlled test of representation structuring in formal math, on a retrieval task. It
  is the same *shape* of experiment as Maith's A/B/C comparison; the differences are
  domain (first-order ATP, not Lean), representations tested (format variants, not
  elaboration-grounded semantic IR), and task (retrieval, not prediction). Maith's §1
  claim is narrowed against this work, not against the source/tactics systems above.
- **Paliwal et al., "Graph Representations for Higher-Order Logic and Theorem Proving"
  (AAAI 2020).** Graph neural networks over HOL Light expression graphs for proof
  search; structure-aware beats non-structured representations, with subexpression
  sharing. Representation-as-a-variable in higher-order logic, in a different proof
  assistant.
- **"Combining Textual and Structural Information for Premise Selection in Lean" (2025;
  arXiv:2510.23637).** Adds an RGCN over the inter-theorem dependency graph to
  ReProver's ByT5 text embeddings, comparing text-only vs. text+structure for premise
  retrieval in Lean 4. The closest Lean-specific representation comparison: it tests
  *adding* structural edges to text embeddings, where Maith tests *replacing* text with a
  canonical semantic graph IR. Also a retrieval task, not a prediction task.
- **Crouse et al., "Improving GNN Representations of Logical Formulae with Subgraph
  Pooling" (DLG-KDD 2020).** GNN representations of logical formulae for premise
  selection; an adjacent representation-design study in the same task family.

The pattern across this literature: representation-as-a-variable *has* been tested in
formal math, on retrieval/premise-selection, with structural-format variants. What remains
unexamined by this lineage is elaboration-grounded canonical semantic graph IR vs. raw BPE,
under prediction metrics, with size-matched controls — which is the specific corner Maith
occupies.

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
   reflects an objective-mismatch rather than a representation failure. *Novelty narrowed
   (per [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md) §H10/H11):*
   the perplexity-vs-probe dissociation in a formal-semantics LM setting was demonstrated
   first by "Meaning in Language Models: A Formal Semantics..." (WFVML 2023), which trains
   a Transformer on program traces and shows a linear probe recovers semantic state while
   perplexity/BLEU diverges from it. That paper is Maith's methodology with the same
   finding, in a sibling domain, pre-dating this work. Maith's contribution is therefore
   *not* the dual-evaluation methodology itself; it is demonstrating the dissociation
   *for a canonical semantic graph IR in formal mathematics specifically*, paired with a
   flat-IR ablation that isolates the semantic content from structural scaffolding — a
   decomposition the program-trace paper does not perform.

2. **Math-domain IR designed from elaborated terms.** Code-IR work uses existing compiler
   IRs (LLVM). Lean-prover work uses source. Maith designs a *new* semantic IR extracted
   from the elaborator's ground-truth semantics (`MetaExtractor.lean` over elaborated
   `Expr`) — the "elaborator as oracle" move. The extraction-source move is *shared* with
   PACT (§3), which extracts self-supervised signal from kernel-level proof terms in Lean;
   what is Maith's own is the representation design — a canonical semantic graph IR with
   normalized entity/relation/operation structure — and the *replacement* of source with
   that IR, rather than PACT's co-training. The representation-comparison lineage (Wang et
   al., Paliwal et al., the 2025 Lean graph-retrieval work) varies *format* of a
   representation; Maith varies the *source* of the representation (elaborated semantics)
   and canonicalizes it. AMR performs the analogous extraction for natural language, but
   via manual annotation rather than an oracle.

A note on what is *not* claimed here: the controlled experiments that make Maith's null
result interpretable (DEC-021 isolating cold-start, DEC-027 isolating model size) are
valuable for *experimental validity* — they distinguish "the IR representation is not
helping" from "a confound is masking a benefit" — but confound isolation is good practice,
not a research contribution, and is not claimed as novelty.

## 7. Open questions the prior art does not resolve for Maith

- **Scale regime.** IR grounding shows task-dependent, non-monotonic gains across
  1.1B–7.3B in code (no clean threshold; several 7.3B cells negative) and has never been
  tested in formal math at any scale. Whether Maith's null holds at larger scale (the
  A-large cell in the
  [`V2_COMPARISON_MATRIX`](../experiments/V2_COMPARISON_MATRIX.md), unrun) remains the most
  important open scale question. DEC-027 resolved the *parameter-count* confound at 358M
  (B-small vs. A), but did not test whether IR grounding pays off at IRCoder-scale model
  size. See [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md) §H4/H8.
- **Training objective.** DEC-027's interpretation points at objective mismatch rather
  than representation failure: the IR encodes semantics (DEC-025) but next-token
  prediction does not reward it. Whether a masked-reconstruction or proof-completion
  objective would recover an IR advantage is untested and is the most direct next lever
  flagged by the experimental record. *Objective precedent:* the masked-subterm
  reconstruction objective itself has been tested on source/proof terms — skip-tree
  (Rabe et al., 2021) and PACT's skip-proof (§3) — and helps. What is untested is pairing
  that objective with a *canonical semantic graph IR* as the prediction target; Maith's H5
  novelty is the representation×objective interaction, not the objective alone. See
  [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md) §H5.
- **Model architecture.** The IR is a graph; the model is a sequence transformer. The
  linearization (graph → token stream) is a lossy step that forces the transformer to
  reconstruct graph structure from positional patterns. A GNN or GraphTransformer would
  consume the graph directly, with IR edges as message-passing paths. Architecture
  alignment is an untested dimension — see §9. *Concrete realization now exists:*
  Nazrin/ExprGraph (Aniva et al., 2026; arXiv:2602.18767) is a GNN-based Lean 4 prover
  that converts Lean expressions to a minimal graph and consumes them directly — the
  aligned-architecture case §8 predicts should work, now implemented in Lean. See
  [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md) §H4/H8.
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
- **Retrieval on the canonical semantic graph IR specifically.** Representation has been tested
  on retrieval/premise-selection in formal math — but with structural-format variants
  (Wang et al., 2017; Paliwal et al., 2020; the 2025 Lean graph-retrieval work, §3), not
  with an elaboration-grounded canonical semantic graph IR vs. raw BPE. Whether Maith's IR
  improves *dependency/premise retrieval* over BPE at Maith's scale is the untested cell
  most aligned with where the field's positive results live (ReProver's wins are on
  retrieval, not perplexity). This is the [`HYPOTHESIS_GRID`](../experiments/HYPOTHESIS_GRID.md)
  H6 cell, unrun. *As of 2026, this is the most clearly-open cell in the field:* the
  2025–26 Lean retrieval wave (LeanSearch v2, Lean Finder, LeanPremise/LeanHammer,
  LeanProgress) all use BPE/text embeddings, sometimes augmented with structural edges,
  and none tests an elaboration-grounded canonical semantic graph IR as the retrieval
  representation. See [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md)
  §H6.

## 8. Theoretical grounding: algorithmic alignment and disentanglement

Maith's central hypothesis — that a canonical semantic representation improves learning —
connects to two established research traditions that, taken together, explain both why the
hypothesis is theoretically sound and why the experimental null (DEC-027) is expected under
the current conditions.

### Algorithmic alignment (Xu et al., ICML 2021)

Xu et al. proved that a neural network learns a target reasoning task with lower sample
complexity when its architecture's modules correspond to the target algorithm's subroutines.
They demonstrated this with GNNs aligning with Bellman-Ford (shortest path): GNN
message-passing mirrors the dynamic programming update, so the network only has to learn
simple linear functions, and it extrapolates. The theorem: better alignment ⇒ lower sample
complexity ⇒ better generalization, including out-of-distribution.

This is the closest formal statement to "a representation isomorphic to the generative
process makes learning easy" — except it's about the *architecture* mirroring the
*algorithm*, not the *input representation* mirroring the *generative process*. Same
principle, applied at a different layer of the stack.

### Disentangled representation learning (Bengio et al., 2013)

Bengio et al. defined a good representation as one that makes the explanatory factors of
variation in the data explicit and independent. "Factors of variation" *is* the generative
process — the latent variables that, when combined, produce the observed data. So
"disentangle the factors of variation" is literally "make the representation isomorphic to
the generative process."

### The impossibility result (Locatello et al., NeurIPS 2019)

Locatello et al. proved that **without inductive biases, unsupervised disentanglement is
impossible** — provably, not just hard. Multiple generative processes can produce identical
observed data, so no representation learned only from data can be guaranteed to recover the
true factors. They showed empirically that across thousands of models and six benchmark
datasets, none reliably disentangled without explicit inductive bias.

This is the dark twin of the principle: the generative process is real, but *recovering it
from data alone is not guaranteed to be possible.*

### Why Maith sidesteps the impossibility result

**Formal mathematics is one of the few domains where the generative process is actually
known.** The generative process of a Lean theorem is the elaborator's construction of the
`Expr` tree — typeclass resolution, implicit insertion, binder scoping, notation expansion.
That's not ambiguous the way "what factors generated this face" is ambiguous. Maith has
ground-truth access to the generative process via Lean's elaborated environment. It extracts
from `Expr`, not from surface syntax.

This means the disentanglement impossibility result **doesn't apply** to Maith's domain.
The impossibility is about *recovering* the process from data — Maith doesn't have to
recover it; the elaborator provides it for free. What remains is the *other* limitation:
even with the generative process exposed, the model has to be able to exploit it.

### The alignment-fragility barrier (where Maith's null is predicted by theory)

Follow-up work on algorithmic alignment (Veličković & Dudzik, NeurIPS 2022 — "Graph Neural
Networks are Dynamic Programmers") found that alignment is fragile: even in the cleanest
alignment settings, GNNs "overfit to clever hacks and sidestep the actual procedure," and
OOD generalization requires *additional* careful inductive biases beyond just architecture
alignment. Alignment is necessary but not sufficient.

**This is exactly the barrier Maith's experimental record shows.** Maith has cleared the
impossible hurdle (the elaborator provides the generative process) and hit the possible-but-
hard one (getting a small model under a biased objective to exploit exposed structure).
The null result (DEC-027: IR doesn't improve prediction at toy scale) is *predicted by the
theory*: alignment is fragile, especially when:

1. **Architecture is misaligned:** the IR is a graph; the model is a sequence transformer.
   The linearization forces the model to reconstruct graph structure from positional
   patterns — an expensive inference the theory says should be fragile.
2. **Objective is misaligned:** next-token prediction rewards surface predictability, not
   semantic structure — formally a gradient-signal-to-noise (GSNR) problem. GSNR, the ratio
   of squared mean to variance of per-sample gradients (Michalkiewicz et al., 2023), is low
   for parameters tied to rare, high-information tokens: their mean gradient is small
   relative to the variance from frequent structural tokens, so they underlearn even when
   the information is present (as DEC-025's probe shows it is). A canonical IR spreads
   semantic content across many rare tokens while concentrating frequency in a few
   structural ones, giving semantic parameters low GSNR by construction. This predicts an
   asymmetry the current specs under-weight: rebalancing the token distribution (DEC-028
   `per_operator`) raises retrievable information but need not improve prediction — more
   rare tokens may worsen per-token GSNR (a theoretical prediction pending empirical check) — whereas up-weighting conclusion/type tokens (H5)
   raises GSNR directly.
3. **Capacity is insufficient:** at 358M, the model is capacity-bound on structural
   scaffolding (DEC-024) before it can fully exploit semantic content.

### What the theory predicts would work

The principle predicts that the IR should help under conditions where the alignment
barriers are addressed:

- **Graph architecture** (GNN/GraphTransformer on the IR's edges) — consumes the graph
  directly, with edges as message-passing paths. No linearization loss.
- **Reconstruction objective** mirroring the elaborator's term-building process — rewards
  understanding the generative structure, not predicting surface tokens.
- **Sufficient capacity** (1B+) — enough to learn both structural scaffolding and semantic
  content (addresses DEC-024's capacity-bound finding).
- **Semantic-task evaluation** (retrieval, ATP) — measures whether the exposed structure
  translates to task advantage, which is what the isomorphism is designed to provide.

Each of these is an experiment in Maith's open research front (H5, H6, H7, H8). The theory
predicts they are the conditions under which the principle should hold — and that the
current null (misaligned conditions) doesn't refute the principle, it confirms the theory's
prediction about what happens under misalignment.

### Implication for the experiment-scope matrix

The experiment-scope matrix in
[`HYPOTHESIS_GRID`](../experiments/HYPOTHESIS_GRID.md) is missing a dimension: **model
architecture** (sequence vs. graph). The current experiments all use sequence transformers,
which is the misaligned case. A graph-native architecture is the aligned case, and the
theory predicts it should be where the IR's advantage appears. This dimension should be
added to the matrix and tracked as an open experimental question.

## 9. The active track (axiom discovery) — added 2026-09-18

> **Why this section exists.** Sections 1–8 were written for the IR/training track and
> were last revised 2026-08-16. The active track was promoted on 2026-09-15 (DEC-036) and
> inherited no grounding from them: on the pre-existing document, `homomorphism`,
> `axiom discovery`, `transfer`, `conjecture`, `causal abstraction`, `analogy`, and
> `circuit complexity` each had **zero** mentions, while `IR` had 96. This section is the
> field check for the track that is actually running. It is not a novelty claim — per the
> project's standing rule, nothing here authorizes calling anything novel.

The active track (`docs/experiments/AXIOM_DISCOVERY.md`) searches for a map φ from a
domain of interest into a known algebraic structure such that φ is a genuine
homomorphism, φ is non-trivial (not a collapse), and pulling existing theorems back
through φ yields new or substantially shorter proofs. Five gates check candidates, each a
real Lean obligation. That shape has prior art on four fronts.

### 9.1 Transport of structure — the classical lineage of φ

The move "prove a structure-preserving map exists, then read theorems across it" is
classical mathematics, not a new method:

- **Transport of structure** (Bourbaki) — the standard practice of carrying a structure
  across a bijection and re-deriving its theorems in the new carrier.
- **Transfer principle / Łoś's theorem** — the model-theoretic case: a statement true in
  one structure transfers to another under the appropriate elementary equivalence. This
  is the closest classical analogue to "pull an existing theorem back through φ," and it
  is also where the *collapse* failure mode is well characterized (the map must be
  structure-embedding, not merely structure-preserving into a trivial target).
- **Categorical / Morita equivalence** — the modern statement of when two structures have
  the same theory, and the natural home of gate 2's "non-trivial, not a collapse"
  condition.

**Position.** Maith's specific instantiation is narrower: a *search* over candidate φ's,
kernel-checked, on a fixed benchmark domain, with proof-size accounting. The general
principle is standard mathematics. What is not standard is treating φ-discovery as a
search problem with an admission-control pipeline; §9.4 is where that comparison belongs.

### 9.2 Structure-mapping and analogical retrieval — the #28 layer

`docs/reference/STRUCTURAL_SIMILARITY.md` (issue #28, phase 1) is a structural
fingerprinting and ranking layer over IR graphs: alpha-equivalence modulo naming and
ordering, plus a graded similarity score
(`0.7 * weighted_jaccard(facts) + 0.3 * jaccard(ops)`). That is analogical *retrieval*
and *mapping*, a field with decades of work and, importantly, established evaluation
practice:

- **Gentner, D. (1983). *Structure-mapping: A theoretical framework for analogy*** —
  the foundational account of analogy as alignment of relational structure rather than
  surface features. This is the closest cognitive-science ancestor of the homomorphism
  obligation, and it supplies vocabulary Maith currently re-derives (the doc's own
  "presentation vs. content" boundary is structure-mapping's *systematicity* and
  *relational* emphasis).
- **Kang et al. (2022), *Augmenting Scientific Creativity with an Analogical Search
  Engine*** (ACM TOCHI 29(6), Article 57) — retrieval + mapping over scientific corpora,
  with user studies on what makes a retrieved analogy useful. Directly relevant to
  deciding what the #28 score should optimize.
- **Literature-based discovery** — Swanson's **ABC model** and the **ARROWSMITH** system
  (Smalheiser & Swanson), which take two disjoint corpora and surface *bridging terms*
  for human assessment. Structurally the same candidate-surfacing shape as the
  Ephapse-style handoff, and the parent of **bisociative** cross-domain discovery using
  word embeddings (New Generation Computing, 2020).

**The borrowable methodological point.** The #28 doc's own "Known limits" states:

> **No ground truth for semantic similarity.** Structural identity is decidable and
> tested; "these two graphs mean related things" is not claimed and is not tested. Any
> future semantic notion needs its own labelled data.

The analogy literature has already confronted this. The standard construction is to rank
candidates by **surprise = structural_similarity x semantic_distance** — i.e. reward
pairs that are structurally alike but semantically far apart — rather than by raw
similarity, which favors trivially-similar pairs. Maith's current score has no
distance term, so it would rank `CancelMonoid`/`CommMonoid`/`SubNegMonoid` (its own
largest group, 51×) highly, which is correct for dedup but exactly wrong for *candidate
generation*: the interesting φ is the one bridging unrelated structures, not the one
joining near-identical ones. **This is a live design question for #28 phase 2, not a
defect in phase 1**, but it should be decided against the prior art rather than
re-derived.

### 9.3 Automated conjecture generation with formal validation — the closest pipeline analogue

The comparison that matters most for the active track's shape:

- **LLM Framework for Discovering Major Mathematical Conjectures** (arXiv:2607.28632) —
  a three-stage pipeline: region search from explicit local evidence modules →
  reflective validation for foundationality/novelty/significance → formal validation in
  Lean 4 + Mathlib. Reported: 20/20 candidates pass Lean parse and typecheck, 20/20 not
  absorbed by `exact?`, 20/20 not discharged by `aesop`, no duplicates. That is Maith's
  gate shape (search → admission control → kernel check → quality filter), with published
  numbers.
- **LeanConjecturer** (arXiv:2506.22005), **STP: Self-play LLM Theorem Provers with
  Iterative Conjecturing and Proving** (ICML 2025), **ATG: Benchmarking Automated Theorem
  Generation** (NAACL 2024 Findings), and the survey ***Automated Conjecturing and Theorem
  Finding*** (JCST) — the broader conjecture-generation line.
- **MOOSE-Chem** (ICLR 2025) — hypothesis *rediscovery* from background only, which is
  the evaluation design discussed in §9.5.

**Position.** Maith's distinguishing element here is not formal validation in Lean —
that is now standard practice in this line — but that the generator is a *structural
search over an IR graph*, with the map φ required to be a proven homomorphism rather
than a conjectured-looking statement. The distinguishing claim is the search mechanism
and the obligation, not the pipeline architecture.

### 9.4 Causal abstraction — the rigorous form of the gates-1–2 question

Gates 1–2 ask: does a map preserve structure, and is it non-trivial? That question has a
formalized literature with a graded metric:

- **Geiger et al. (2021), *Causal Abstractions of Neural Networks*** (NeurIPS 2021) —
  alignment of neural representations with variables in an interpretable causal model,
  verified by **interchange interventions**.
- **Geiger et al. (2022), *Inducing Causal Structure for Interpretable Neural Networks***
  (ICML 2022) — interchange intervention training.
- **Geiger et al. (2023), *Finding Alignments Between Interpretable Causal Variables and
  Distributed Neural Representations*** (arXiv:2303.02536) — **distributed alignment
  search** (DAS), finding alignments by gradient descent, and introducing **interchange
  intervention accuracy (IIA)** as a graded notion.
- **Causal Abstraction in Model Interpretability: A Compact Survey** (arXiv:2410.20161).
- **Sutter et al. (2025), *The Non-Linear Representation Dilemma*** (arXiv:2507.08802,
  NeurIPS 2025 spotlight) — the boundary condition: if the alignment map is allowed to be
  arbitrarily expressive, any network aligns to any algorithm, and the test goes vacuous.

> **Correction to an earlier cross-repo note.** A sibling-repo review (Ephapse
> `DEC-010`) initially flagged Sutter et al. as a risk to Maith's gate 1, on the reading
> that a homomorphism obligation could be passed vacuously. **That was wrong and has been
> corrected there.** Gate 1 is a written Lean term whose complexity is fixed by its
> author; it is not a learned or capacity-selected map, so the Sutter vacuity result does
> not apply to it. Gate 2 additionally rejects the Unit-collapse by name (see the #29
> starter in `AGENT_HANDOFF.md`). The correct reading is the one used above: this
> literature is a **strengthening to borrow**, not a defect in Maith's gates.

**What is specifically borrowable.** IIA gives a *graded* answer to a question Maith
currently answers binarily ("typechecks → pass"). More usefully, the Sutter boundary
condition sharpens gate 2: for a non-injective φ into a target whose operations are weak
enough that the homomorphism law holds for uninteresting reasons, "prove **or
characterize** φ's kernel" is the only part of the gate doing real work. The
causal-abstraction literature states precisely what such a characterization must
establish for the pass to carry information.

### 9.5 Evaluation design — the missing ground-truth recovery setting

Every Maith gate evaluates a candidate that already exists. Nothing tests whether the
*proposal mechanism* can find candidates that are known to be there. Adjacent work does:

- **MOOSE-Chem** (ICLR 2025) recovers a known chemistry hypothesis from background alone —
  a rediscovery setting with ground truth.
- Mechanistic-interpretability practice injects a known correlation into a background
  corpus and measures detector recovery against a naive baseline (method and numbers in
  the sibling repo `philipdallen/ephapse`, `docs/reference/PRIOR_ART.md` §2).

Without an equivalent, "the proposal mechanism ran and found no candidates" is
**uninterpretable**: it cannot distinguish "no candidate φ exists on this benchmark" from
"the structural search cannot see the ones that do." This is the same ground-truth gap
the #28 doc admits for semantic similarity (§9.2), appearing at the level of the whole
proposal step. A small injected-fixture design would close it — synthetic φ's planted in
the corpus that the search is expected to recover — and is far cheaper than a full
benchmark.

#### 9.5.1 Preferred design: retrospective time-cut validation (added 2026-09-18)

The injected-fixture design above tests whether the search mechanism can find *planted*
candidates. A stronger and more established design tests whether it can anticipate *real*
ones.

**The method.** Freeze the knowledge base at time *t*. Pose a genuinely open question from
time *t*. Generate and rank candidate hypotheses using only evidence available before *t*.
Then compare the top-ranked candidates against what was later discovered or supported.
Success is measured as: did the system recover, anticipate, or move measurably toward the
later-supported mechanism?

This is the standard design in the current literature, not a proposal:

- **ProjectionBench** (arXiv:2605.30284) — progressive information disclosure, starting
  from a paper's topic and research question and revealing technical detail in stages;
  models must generate hypotheses at each stage.
- **IdeaBench** (PMC11923747) — 2,374 target papers published after 2024-01-01, paired with
  ~23K filtered references, explicitly chosen to postdate model training data.

**The load-bearing caveat — and it is fatal if missed.** The cut must be relative to the
**model's training cutoff**, not to wall-clock "recent." If a model has already memorized
the later discovery, the benchmark measures recall, not hypothesis generation. Both
benchmarks above select post-cutoff material specifically to prevent this; the leakage
concern is why IdeaBench dates its targets rather than taking the most recent papers.
Any Maith implementation must state the model's cutoff and verify candidate targets
postdate it. A time-cut design without this check is not evidence.

**Why this ranks above the injected-fixture design.** Planted fixtures test the search
mechanism's sensitivity under controlled conditions. Time-cut validation tests the same
property against real mathematical structure, where candidates are not shaped to be
findable. It is also the only design here that could support a claim about the *end* of
the pipeline — whether gate-3-surviving transfers correspond to anything the field later
found valuable — rather than only about the search step.

**What it still would not establish.** Anticipating a known result is not discovering a
new one, and the design cannot rule out that the model is retrieving a near-sensitive
paraphrase of the discovery rather than reasoning to it. Record it as a benchmark of
recovery, not a demonstration of discovery. Both source benchmarks frame it this way.

#### 9.5.2 The protocol is older than the LLM literature — and its known weaknesses transfer

The time-cut design is not new to the LLM era. Literature-based discovery has used the
**replication** method for decades: given a known discovery at time *t*, provide the
literature available before *t*, produce a ranked candidate list, and score the system by
how highly the known target ranks. Swanson's **fish-oil / Raynaud's syndrome** (1986) and
**magnesium / migraine** (1988) links are the canonical targets and remain the most
commonly used benchmark for LBD systems.

Two documented weaknesses are worth importing rather than rediscovering:

1. **The target set is tiny.** Evaluation has relied on the same handful of confirmed
   discoveries for over three decades. Anyone building a Maith analogue must state the
   target set and its selection process, not inherit "a few famous hits" as if it were a
   benchmark.
2. **Target-selection bias.** The canonical discoveries were made by a researcher who
   personally experienced the conditions involved — a documented concern about scientific
   neutrality in the protocol. Target selection is a source of bias, not a neutral step.

**Why this matters for Maith specifically.** The active track's benchmark domain is
circuit complexity, and #26 builds a transfer-target list (T1–T10 plus screening results).
That list *is* a target set under this protocol, and the same question applies: how were
the targets chosen, and could the selection make recovery easy or hard for reasons
unrelated to the search mechanism? The LBD literature says to ask this explicitly.

**The generalizable point.** A time-cut benchmark measures the *search mechanism* only if
the target set was not selected with knowledge of what the mechanism can find. Otherwise
it measures the selection. Same failure shape as the #28 ground-truth gap (§9.2), one
level up.

### 9.6 Where the active track is distinct

Following §6's pattern, and claiming only what the above supports:

1. **The obligation is a kernel obligation, and it is the *first* gate.** The conjecture-
   generation line filters for parseability, non-triviality via `exact?`/`aesop`, and
   novelty; it does not require a proven structure-preserving map as the admission
   condition for a candidate. Maith's gate 1 does. That is the closest thing to a
   distinguishing methodological choice here, and it is a *strengthening of admission
   control*, not a new result.
2. **Search is over a canonical semantic IR graph**, with presentation-invariance pinned
   by tests (and — importantly — a deliberate boundary: free-variable names are treated as
   content, with `test_free_variable_name_change_must_not_match` guarding the boundary).
   The analogy literature operates on text, embeddings, or knowledge graphs, not on an
   elaboration-grounded canonical graph IR.
3. Nothing here is claimed about *problem taste* or significance. The closest prior
   pipeline (arXiv:2607.28632) explicitly targets "high problem taste"; Maith's stated
   non-goals deliberately decline that target in favour of a falsifiable general method.
   That is a scoping difference worth stating so it is not read as a capability gap.

### 9.7 What the prior art does not resolve for the active track

- **Whether homomorphism-first admission control is productive.** Requiring a proven
  homomorphism before any transfer attempt is stricter than the conjecture line. The
  prior art gives no evidence either way on whether that strictness costs more in
  candidate yield than it gains in precision. This is the active track's central
  untested methodological bet.
- **Whether structural search over an IR graph finds anything the text-based line
  misses.** No comparison exists. The `#28` results (4,029 graphs, 539 shared
  fingerprints, real structural families recovered) show the mechanism runs and finds
  families; they do not show it finds *transferable* φ's.
- **What the correct ranking objective is** (see §9.2): similarity alone cannot
  distinguish dedup from discovery, and no ground truth exists to calibrate a
  surprise-style score on this corpus.
- **How a found φ should be checked for significance** beyond typechecking — `#34`
  ("adversarial significance check via independent external models") is the open question
  here, and the causal-abstraction literature (§9.4) is the closest existing formal
  treatment.
- **Whether a challenge-before-admission stage is worth adding.** The gates verify a
  candidate that already exists; there is no step that tries to *defeat* a candidate
  before it enters. #34 is the nearest thing, and it operates only on gate-5 survivors.
  A pre-admission critique stage ("is this a relabeled standard result? does the mapping
  preserve the homomorphism or only the vocabulary? which preconditions are absent?") is
  a candidate design, not a committed one. Note the ordering difference: post-hoc
  verification is strong here, pre-admission critique is thin.
- **Partly resolved by §9.5.1:** the recovery-setting gap now has a preferred design
  (retrospective time-cut validation). What remains open is the training-cutoff question
  — which model, and whether its cutoff can actually be established — plus the fact that
  a recovery benchmark still would not demonstrate new discovery.

## 10. References

- Assran, M., Duval, Q., Misra, I., Bojanowski, P., Vincent, P., Rabbat, M., LeCun, Y., &
  Ballas, N. (2023). Self-Supervised Learning from Images with a Joint-Embedding Predictive
  Architecture (I-JEPA). *CVPR 2023*. arXiv:2301.08243.
- Banarescu, L., et al. (2013). Abstract Meaning Representation (AMR) 1.0 specification.
  *Parsing na Noite da Lua*.
- Bardes, A., et al. (2024). V-JEPA: Latent Video Prediction for Visual Representation
  Learning. *ICLR 2024*.
- "Combining Textual and Structural Information for Premise Selection in Lean."
  arXiv:2510.23637 (2025).
- Crouse, M., Abdelaziz, I., Cornelio, C., Thost, V., Wu, L., Forbus, K., Fokoue, A.
  (2020). Improving Graph Neural Network Representations of Logical Formulae with
  Subgraph Pooling. *DLG-KDD 2020*.
- Cummins, C., et al. (2024). Meta Large Language Model Compiler.
- Gong, L., Elhoushi, M., & Cheung, A. (2024). AST-T5: Structure-Aware Pretraining for
  Code Generation and Understanding. arXiv:2401.03003.
- LeCun, Y. (2022). A Path Towards Autonomous Machine Intelligence. OpenReview position
  paper. https://openreview.net/forum?id=BZ5a1r-kVsf
- Mao, Z., Song, H., Dabre, R., Chu, C., & Kurohashi, S. (2023). Variable-length Neural
  Interlingua Representations for Zero-shot Neural Machine Translation. *Multi3Generation*.
- Michalkiewicz, M., et al. (2023). Domain Generalization Guided by Gradient Signal to
  Noise Ratio of Networks. *ICCV 2023*. arXiv:2310.07361. (Formal GSNR definition and its
  tie to the generalization gap; grounds the objective-misalignment mechanism in §8.)
- Niu, Y., et al. (2023). FAIR: Flow Type-Aware Pre-Training of Compiler Intermediate
  Representations. arXiv:2309.04828.
- Paliwal, A., Loos, S., Rabe, M., Bansal, K., & Szegedy, C. (2020). Graph Representations
  for Higher-Order Logic and Theorem Proving. *AAAI 2020*.
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
- Wang, M., Tang, Y., Wang, J., & Deng, J. (2017). Premise Selection for Theorem Proving by
  Deep Graph Embedding. *NeurIPS 2017*. arXiv:1709.09994.
- Xin, H., et al. (2024). DeepSeek-Prover: Advancing Theorem Proving via Large Language
  Models. (V2: arXiv:2504.21801, 2025.)
- Yang, K., Sui, Y., He, N., & Anandkumar, A. (2023). LeanDojo: Theorem Proving with
  Retrieval-Augmented Language Models (ReProver). *NeurIPS 2023 Datasets and Benchmarks*.
- "Can Large Language Models Understand Intermediate Representations in Compilers?"
  arXiv:2502.06854 (2025).
- "From Tokens to States: LLMs as a Special Case of World Models and the Continuous Path
  Beyond." arXiv:2606.28127 (2026).
- Survey of Abstract Meaning Representation: Then, Now, Future. arXiv:2505.03229 (2025).
- Xu, K., Hu, W., Leskovec, J., & Jegelka, S. (2021). How Neural Networks Extrapolate:
  From Feedforward to Graph Neural Networks. *ICML 2021*.
- Bengio, Y., Courville, A., & Vincent, P. (2013). Representation Learning: A Review and
  New Perspectives. *IEEE TPAMI*.
- Locatello, F., et al. (2019). Challenging Common Assumptions in the Unsupervised Learning
  of Disentangled Representations. *NeurIPS 2019*.
- Veličković, P., & Dudzik, W. (2022). Graph Neural Networks are Dynamic Programmers.
  *NeurIPS 2022*.

### References added by the [`LITERATURE_REVIEW_2026_08`](../experiments/LITERATURE_REVIEW_2026_08.md)

- Aniva, L., Oikawa, I., Dill, D., & Barrett, C. (2026). Nazrin: Atomic Tactics for
  Graph Neural Networks for Theorem Proving in Lean 4. arXiv:2602.18767.
- Rabe, M. N., Lee, D., Bansal, K., & Szegedy, C. (2021). Mathematical reasoning via
  self-supervised skip-tree training. *ICLR 2021*.
  https://openreview.net/forum?id=YmqAnY0CMEy
- "Meaning in Language Models: A Formal Semantics..." WFVML 2023. (ETH, paper_26.pdf.)
- "LeanTree: Accelerating White-Box Proof Search with Factorized States in Lean 4."
  arXiv:2507.14722 (2025).
- "LeanSearch v2: Global Premise Retrieval for Lean 4 Theorem Proving." arXiv:2605.13137
  (2026).
- "Lean Finder: Semantic Search for Mathlib That Understands User Intents."
  arXiv:2510.15940 (ICLR 2026).
- "Premise Selection for a Lean Hammer." arXiv:2506.07477 (2025).
- "LeanProgress: Guiding Search for Neural Theorem Proving via Proof Progress Prediction."
  arXiv:2502.17925 (2025).
- "SorryDB: Can AI Provers Complete Real-World Lean Theorems?" arXiv:2603.02668 (2026).
- "Thinking Machines: Mathematical Reasoning in the Age of LLMs." MDPI AI (2025).
- "Canonical Autoregressive Generation." arXiv:2506.06446 (2025).
- "Language Models over Canonical Byte-Pair Encodings." ICML 2025.
  https://icml.cc/virtual/2025/poster/44596
- "LLM Knowledge is Brittle: Truthfulness Representations Rely on Superficial
  Resemblance." arXiv:2510.11905 (2025).
- Math information retrieval representation study. CEUR Vol-2696, paper_235.

### References added by §9 (active track, 2026-09-18)

**Transport of structure / transfer**
- Bourbaki, N. *Éléments de mathématique* — transport of structure as standard practice.
- Łoś, J. (1955). Quelques remarques, théorèmes et problèmes sur les classes définissables
  d'algèbres. (Transfer principle for ultraproducts; the model-theoretic transfer case.)
- Morita equivalence — the categorical statement of shared theory between structures.

**Structure-mapping and analogical retrieval**
- Gentner, D. (1983). Structure-mapping: A theoretical framework for analogy.
  *Cognitive Science* 7(2), 155–170.
- Kang, H. B., Qian, X., Hope, T., Shahaf, D., Chan, J., & Kittur, A. (2022). Augmenting
  Scientific Creativity with an Analogical Search Engine. *ACM TOCHI* 29(6), Article 57.
- Swanson, D. R. & Smalheiser, N. R. — the ABC model of literature-based discovery and
  the ARROWSMITH system (bridging-term identification across disjoint corpora).
- "Bisociative Literature-Based Discovery: Lessons Learned and New Word Embedding
  Approach." *New Generation Computing* (2020). doi:10.1007/s00354-020-00108-w.

**Automated conjecture generation with formal validation**
- Wong, A., Zeng, Z., Tan, Y., Li, W., Chen, X., Lai, X., Shi, Y., Lu, L., & Chen, Y.
  (2026). LLM Framework for Discovering Major Mathematical Conjectures: AI's Quest for
  the Next Riemann Hypothesis. arXiv:2607.28632.
- Onda, N., Kasaura, K., Oriike, Y., Taniguchi, M., Sannai, A., & Sonoda, S. (2025).
  LeanConjecturer: Automatic generation of mathematical conjectures for theorem proving.
  arXiv:2506.22005.
- Dong, K. & Ma, T. (2025). STP: Self-play LLM Theorem Provers with Iterative Conjecturing
  and Proving. *ICML 2025*, 14114–14136.
- Lin, X., Cao, Q., Huang, Y., Yang, Z., Liu, Z., Li, Z., & Liang, X. (2024). ATG:
  Benchmarking Automated Theorem Generation for Generative Language Models. *NAACL 2024
  Findings*. doi:10.18653/V1/2024.FINDINGS-NAACL.279.
- "Automated conjecturing and theorem finding: A survey." *JCST* (2026).
  doi:10.1007/s11390-026-6040-0.
- Yang, Z., Liu, W., et al. (2025). MOOSE-Chem: Large Language Models for Rediscovering
  Unseen Chemistry Scientific Hypotheses. *ICLR 2025*.

**Causal abstraction / interchange interventions**
- Geiger, A., Lu, H., Icard, T., & Potts, C. (2021). Causal Abstractions of Neural
  Networks. *NeurIPS 2021*.
- Geiger, A., Wu, Z., Lu, H., Rozner, J., Kreiss, E., Icard, T., Goodman, N., & Potts, C.
  (2022). Inducing Causal Structure for Interpretable Neural Networks. *ICML 2022*.
- Geiger, A., Ibeling, D., Zur, A., et al. (2023). Finding Alignments Between
  Interpretable Causal Variables and Distributed Neural Representations. arXiv:2303.02536.
- "Causal Abstraction in Model Interpretability: A Compact Survey." arXiv:2410.20161.
- Sutter, T., et al. (2025). The Non-Linear Representation Dilemma: Is Causal Abstraction
  Enough for Mechanistic Interpretability? arXiv:2507.08802, *NeurIPS 2025* (spotlight).

**Retrospective / time-cut benchmark design (§9.5.1, added 2026-09-18)**
- ProjectionBench: Evaluating Scientific Hypothesis Generation in LLMs Under Progressive
  Information Disclosure. arXiv:2605.30284.
- IdeaBench: benchmark dataset for LLM hypothesis generation — 2,374 target papers
  published after 2024-01-01 with ~23K filtered references, dated to prevent training
  leakage. PMC11923747 ("Embracing Foundation Models for Advancing Scientific Discovery").

**Literature-based discovery / replication protocol (§9.5.2, added 2026-09-18)**
- Swanson, D. R. (1986). Fish oil, Raynaud's syndrome, and undiscovered public knowledge.
  *Perspectives in Biology and Medicine* 30(1), 7–18. doi:10.1353/pbm.1986.0087.
- Swanson, D. R. (1988). Migraine and magnesium: eleven neglected connections.
  (The second canonical LBD target.)
- Smalheiser, N. R. (2017). Rediscovering Don Swanson: the past, present and future of
  literature-based discovery. *Journal of Data and Information Science* 2(4), 43–64.
  doi:10.1515/jdis-2017-0019.
- "Literature-based discovery: addressing the issue of the subpar evaluation methodology."
  PMC9945845. (The target-set and selection-bias critique cited in §9.5.2.)
- ARROWSMITH two-node search interface — Smalheiser et al. (2006), *Journal of Biomedical
  Discovery and Collaboration* 1(1).
