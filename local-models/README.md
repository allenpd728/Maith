# Maith Local Models

Four narrow-purpose Ollama models that extend the Maith workflow without replacing the main agent.
Each model has a single well-defined role and must not be wired into automated pipelines that act
without human review. All output goes to a human or a log before any next step is taken.

## Base Model Decision

All four models use `qwen3:8b` as their base.

**Original spec vs actual:**

| Model | Originally specified | Actual base | Reason for change |
|---|---|---|---|
| maith-coder | qwen2.5-coder:7b | qwen3:8b | Storage constraint; boilerplate Python doesn't require a code-fine-tuned model |
| maith-docs | qwen3.5:9b | qwen3:8b | Storage constraint; documentation drafting is well within qwen3:8b capability |
| maith-checker | qwen3-embedding:4b | qwen3:8b | Embedding-only models cannot run system prompts or produce text; role reframed as reasoning auditor |
| maith-verifier | qwen3:8b | qwen3:8b | No change — already local |

If storage constraints are lifted, maith-coder is the best candidate to upgrade to
`qwen2.5-coder:7b` for higher code quality. maith-docs could benefit from `qwen3.5:9b`
for longer document reformatting tasks.

## Models

### maith-coder
**Role:** Boilerplate Python scripts, test scaffolding, pre-flight checks.

**Hard exclusions** (system prompt enforces these — do not ask it to touch these files):
- `train.py`
- `build_dataset.py`
- Any `.lean` file
- `eval_completion.py`, `eval_checkpoint.py`, or any evaluation logic

**When to use:** generating pytest fixtures, smoke test stubs, environment validation scripts,
corpus inspection utilities, result file parsers.

**When NOT to use:** anything touching the training loop, dataset construction, Lean pipeline,
or eval logic. Those stay with the main agent.

---

### maith-docs
**Role:** Drafts and reformats documentation from facts explicitly given in the prompt.

**Hard constraints** (system prompt enforces these):
- Never invents, estimates, or infers a number or claim not given in the prompt
- Will decline any request to determine current state ("what does the README say?") — redirect those to the main agent or maith-verifier

**When to use:** drafting README sections, changelog entries, doc page rewrites — when you already
have all the facts and just need them formatted clearly.

**When NOT to use:** generating documentation when you don't have the source facts yet. Feed it
the facts first, then ask it to write.

---

### maith-checker
**Role:** Consistency auditor. Flags mismatches between key files when their content is pasted in.

**Files it audits:** `docs/DECISION_LOG.md`, `README.md`, `docs/PHASE_5_RESULTS.md`, dashboard JSON files.

**Hard constraints** (system prompt enforces these):
- Read-only. Never modifies or resolves anything.
- Never concludes which source is correct — only flags the discrepancy.
- Every response ends with: "All items above require human review before any action is taken."

**Original design vs actual:** Was intended to use vector embeddings (qwen3-embedding:4b) to detect
semantic similarity mismatches. Now uses line-by-line reasoning instead. The output contract is the
same: a numbered list of potential mismatches for human review. Embedding-based detection would have
caught more subtle paraphrasing inconsistencies; the reasoning approach requires you to paste the
relevant file sections explicitly.

**When to use:** before publishing results, after editing multiple docs, or any time you want a
second pass over consistency between the decision log, README, and results files.

---

### maith-verifier
**Role:** Strict evidence-based verifier. Only reports what it directly confirmed from evidence
provided in the prompt.

**Hard constraints** (system prompt enforces these):
- Never restates or summarises a claim without independently checking it against provided evidence
- Never fills gaps with inference or plausible-sounding assumptions
- Every response is structured as CONFIRMED / DENIED / UNVERIFIABLE with source citations

**When to use:** spot-checking specific factual claims before committing to a report or doc update,
verifying that a git log entry matches what the decision log says happened, confirming a metric
matches the results file.

**When NOT to use:** asking it to summarise a situation — it will only verify discrete claims
against evidence you give it.

## Usage

Always use `maith-run.py` — never call `ollama run` directly. The wrapper acquires a
lock file before starting the model and releases it on exit, ensuring only one maith-*
model runs at a time.

**Interactive mode:**
```bash
python3 local-models/maith-run.py maith-coder
python3 local-models/maith-run.py maith-docs
python3 local-models/maith-run.py maith-checker
python3 local-models/maith-run.py maith-verifier
```

**Single prompt (non-interactive):**
```bash
python3 local-models/maith-run.py maith-verifier "Verify this claim: variant B perplexity was 2.14. Evidence: [paste results.json content]"
```

**Check whether a model is currently running:**
```bash
python3 local-models/maith-run.py --status
```

**If a model crashed and left a stale lock that wasn't auto-cleared:**
```bash
rm /tmp/maith-model.lock
```

### How the lock works

- On start: writes `/tmp/maith-model.lock` containing the model name and PID
- On exit (normal, error, or Ctrl-C): removes the lock file automatically
- On blocked start: prints which model is active and exits immediately — nothing starts
- **Stale lock detection:** when a new run checks an existing lock, it uses `ps -o comm=` to verify the locked PID is still running *as a Python process* (our wrapper). If the PID is dead, or alive but running as something else (e.g. `llama-server` reused the PID after a kill -9), the lock is declared stale and cleared automatically before the new model starts.

This approach handles the hard case: after `kill -9` on the wrapper, the OS can reuse the wrapper's PID within milliseconds — often for the `llama-server` child that was spawned by ollama. A simple liveness check (`os.kill(pid, 0)`) would incorrectly treat that as a live lock. The command name check catches this and unblocks the next run without any manual intervention.

### Testing the lock

`test-lock.py` automates the crash-cleanup test sequence:

```bash
python3 local-models/test-lock.py
```

It starts maith-coder in the background, sends `kill -9` to the wrapper as soon as the lock appears, immediately runs maith-verifier, and prints a clear PASS/FAIL verdict. Run this any time you want to verify the stale lock path is working correctly.

## Recreating the Models

If models need to be recreated (e.g. after an ollama reinstall):
```
cd local-models/
ollama create maith-coder -f Modelfile.maith-coder
ollama create maith-docs -f Modelfile.maith-docs
ollama create maith-checker -f Modelfile.maith-checker
ollama create maith-verifier -f Modelfile.maith-verifier
```

All four share `qwen3:8b` as their base, so only one model download is required.

## Operating Rules

- **Always use `maith-run.py`, never `ollama run` directly.** The wrapper enforces the lock. Calling ollama directly bypasses it.
- **One model at a time.** The lock file at `/tmp/maith-model.lock` enforces this at the process level — a second invocation will be blocked and print which model is active.
- **No chaining without review.** The output of one model must be read and approved by a human before being passed as input to another. No automated pipelines that pipe one model's output directly into the next.
- **No parallel invocation.** Even if the tasks seem independent, run them sequentially. This avoids compounding errors and keeps MPS load predictable on a constrained machine.

## What These Models Do Not Do

- They do not trigger downstream pipeline steps on their own
- They do not write to any project file
- They do not communicate with each other
- They do not run training, evaluation, or dataset construction
- They produce output for human review only
