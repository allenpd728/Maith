# Agent B Task: TensorBoard Integration

> **You are Agent B.** Your task is to add TensorBoard logging to
> `python/train_v2_resume.py` — the canonical training script. This is Phase 4
> of the manage.py + TensorBoard scope.
>
> **You work in parallel with Agent A** (building `python/manage.py`). You do
> not depend on Agent A's work. You touch different files (you modify
> `python/train_v2_resume.py`, Agent A creates `python/manage.py`).
>
> **Full spec:** `docs/experiments/MANAGE_PY_TENSORBOARD_SCOPE.md` — read §4
> for the TensorBoard logging spec. This document is the condensed task
> assignment.

---

## What you're building

Add ~15 lines of TensorBoard SummaryWriter logging to `python/train_v2_resume.py`
so that every training run produces TensorBoard event files that can be
visualized in a browser.

TensorBoard is already installed — it ships with torch. No new dependencies.

## What to log

| Metric | When to log | TensorBoard visualization |
|---|---|---|
| `train/loss` | Every logging step (every N batches) | Loss curve over steps |
| `eval/perplexity` | End of each epoch | Perplexity over epochs |
| `config/variant` | Start of run (as text) | Run metadata |
| `config/epochs` | Start of run (as text) | Run metadata |
| `config/vocab_size` | Start of run (as text) | Run metadata |
| `config/learning_rate` | Start of run (as text) | Run metadata |

## Where to log

TensorBoard event files go in `runs/{run_id}/tb/`. The `--logdir runs/` flag
makes TensorBoard discover all runs and overlay their loss curves for comparison.

## Code to add

### 1. Import (top of file, with other imports)

```python
from torch.utils.tensorboard import SummaryWriter
```

### 2. Initialize writer (at training start, after model is loaded)

Find the section where training begins (after the model, tokenizer, and dataset
are loaded, before the training loop). Add:

```python
writer = SummaryWriter(log_dir=os.path.join(out_dir, "tb"))
writer.add_text("config/variant", str(variant), 0)
writer.add_text("config/epochs", str(epochs), 0)
writer.add_text("config/vocab_size", str(vocab_size), 0)
writer.add_text("config/learning_rate", str(lr_for_variant(variant)), 0)
```

### 3. Log loss during training (in the training loop)

Find where the training loss is computed/logged. After each logging interval
(where loss is printed), add:

```python
writer.add_scalar("train/loss", loss.item(), global_step)
```

You need to find or create a `global_step` counter that increments per batch.
If one exists (e.g., `step`, `global_step`, `total_steps`), use it. If not,
add a counter:

```python
global_step = 0  # before the training loop
# inside the loop:
global_step += 1
```

### 4. Log perplexity after each epoch (where perplexity is computed)

Find where `eval_perplexity` or `ppl` is computed (after each epoch or at the
end of training). Add:

```python
writer.add_scalar("eval/perplexity", ppl, epoch)
```

### 5. Close writer (at the end, after training is done)

```python
writer.close()
```

## Important notes

- **Do NOT modify `train.py`** — it's deprecated. Only modify
  `train_v2_resume.py`.
- **Do NOT modify any other file.** Your change is self-contained in
  `train_v2_resume.py`.
- The `out_dir` variable in `train_v2_resume.py` is the run directory (e.g.,
  `runs/variant_A_v3_2ep_20260811/`). The `tb/` subdirectory goes inside it.
- If `torch.utils.tensorboard` import fails, the tensorboard package may need
  installation: `/usr/local/bin/python3 -m pip install tensorboard`. But it
  should already be available since it ships with torch.
- Handle the case where the SummaryWriter can't write (e.g., disk full) —
  wrap in a try/except so training doesn't crash if TensorBoard logging fails.

## Files you modify

| File | Action |
|---|---|
| `python/train_v2_resume.py` | MODIFY — add SummaryWriter logging (~15 lines) |

## Files you read (do NOT modify)

| File | What to read |
|---|---|
| `python/train_v2_resume.py` | The script you're modifying — find the training loop, loss logging, and perplexity computation |
| `docs/experiments/MANAGE_PY_TENSORBOARD_SCOPE.md` §4 | Full TensorBoard spec |

## Testing

1. **Smoke test:** Run a 1-epoch, 50-example smoke training and verify:
   - `runs/{id}/tb/` directory is created
   - Event files appear in it
   - `tensorboard --logdir runs/ --port 6006` shows the run
   - Loss curve and perplexity are visible in the browser

```bash
# On the M4:
cd /Users/openhands-demo/maith-repo
/usr/local/bin/python3 python/train_v2_resume.py --variant A \
    --datasets datasets_perop --out runs/smoke_tb_test \
    --smoke-test --epochs 1

# Verify:
ls runs/smoke_tb_test/tb/

# Launch TensorBoard:
tensorboard --logdir runs/ --port 6006
# Then open http://localhost:6006 in a browser (or SSH tunnel)
```

2. **Verify existing functionality still works:** the results.json should
   still be written, the perplexity should still be computed, and the
   training should complete without errors.

## Acceptance criteria

- [ ] `train_v2_resume.py` writes TensorBoard event files to `runs/{id}/tb/`
- [ ] Loss is logged as `train/loss` scalar per step
- [ ] Perplexity is logged as `eval/perplexity` scalar per epoch
- [ ] Config (variant, epochs, vocab_size, learning_rate) is logged as text
- [ ] Smoke test produces visible charts in TensorBoard
- [ ] Training still completes and writes results.json correctly
- [ ] No new dependencies (tensorboard ships with torch)
- [ ] `train.py` is NOT modified

## Git workflow

1. `git checkout kit/dev`
2. `git pull origin kit/dev`
3. Modify `python/train_v2_resume.py`
4. Test on the M4 (SSH access required — see below)
5. Commit: `impl: TensorBoard logging in train_v2_resume.py (Phase 4)`
6. Push to `kit/dev`

## SSH access to M4

The M4 is at `4.tcp.ngrok.io:15643`, user `openhands-demo`. You need the SSH
key set up by the user. Test with:
```bash
ssh -i <key_path> -p 15643 openhands-demo@4.tcp.ngrok.io 'echo CONNECTED'
```

The repo on the M4 is at `/Users/openhands-demo/maith-repo/`.
Python with torch is at `/usr/local/bin/python3`.

## Launching TensorBoard (for testing)

```bash
# On the M4:
tensorboard --logdir /Users/openhands-demo/maith-repo/runs/ --port 6006

# From your machine, SSH tunnel:
ssh -L 6006:localhost:6006 -i <key_path> -p 15643 openhands-demo@4.tcp.ngrok.io

# Then open http://localhost:6006 in a browser
```
