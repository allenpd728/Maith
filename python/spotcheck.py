#!/usr/bin/env python3
"""
spotcheck.py — decode and compare token-level predictions for B vs C on named examples.
Loads both models, runs the same masked-completion eval, then decodes every
predicted and actual token so we can see what's happening character-by-character.
"""
import json, pathlib, sys
import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

MASK_LAST   = 5
RUNS_DIR    = pathlib.Path("runs")
DATASETS    = pathlib.Path("datasets")
NAMES_FILE  = pathlib.Path("/tmp/spotcheck2_names.json")
BASE_MODEL  = "Qwen/Qwen2.5-Coder-0.5B"

device = "mps" if torch.backends.mps.is_available() else "cpu"
print(f"Device: {device}\n")

names = json.loads(NAMES_FILE.read_text())

# Load tokenizer (shared for B and C — both use Qwen BPE)
print("Loading tokenizer...")
tok = AutoTokenizer.from_pretrained(BASE_MODEL, trust_remote_code=True)

def load_examples(variant):
    path = DATASETS / f"eval_{variant}.jsonl"
    by_name = {}
    for line in path.read_text().strip().splitlines():
        ex = json.loads(line)
        by_name[ex["name"]] = ex
    return by_name

def load_model(variant, ckpt_dir):
    print(f"Loading variant {variant} from {ckpt_dir} ...")
    m = AutoModelForCausalLM.from_pretrained(
        str(ckpt_dir), trust_remote_code=True, dtype=torch.float32
    ).to(device)
    m.eval()
    return m

def run_example(model, ex):
    ids = ex["input_ids"]
    if len(ids) <= MASK_LAST:
        return None
    prefix_len = len(ids) - MASK_LAST
    targets = ids[prefix_len:prefix_len + MASK_LAST]
    current = ids[:prefix_len]
    preds = []
    with torch.no_grad():
        for tgt in targets:
            inp = torch.tensor(current, dtype=torch.long).unsqueeze(0).to(device)
            logits = model(input_ids=inp).logits[0, -1]
            pred = int(logits.argmax().item())
            preds.append((pred, tgt))
            current = current + [tgt]
    return preds

def decode(token_id):
    s = tok.decode([token_id], skip_special_tokens=False)
    return repr(s)

# Load data for both variants
examples_B = load_examples("B")
examples_C = load_examples("C")

ckpt_B = RUNS_DIR / "variant_B2" / "checkpoint-final"
ckpt_C = RUNS_DIR / "variant_C_v2" / "checkpoint-final"

model_B = load_model("B", ckpt_B)
model_C = load_model("C", ckpt_C)

print("\n" + "="*72)
print("SPOT-CHECK: B vs C token-level predictions (mask_last=5)")
print("="*72)

for name in names:
    ex_B = examples_B.get(name)
    ex_C = examples_C.get(name)
    if not ex_B or not ex_C:
        print(f"\n[{name}] — missing from one split, skipping")
        continue

    res_B = run_example(model_B, ex_B)
    res_C = run_example(model_C, ex_C)
    if res_B is None or res_C is None:
        print(f"\n[{name}] — too short, skipping")
        continue

    correct_B = sum(1 for p, t in res_B if p == t)
    correct_C = sum(1 for p, t in res_C if p == t)
    total = len(res_B)

    print(f"\n{'─'*72}")
    print(f"  {name}  (seq_len={len(ex_B['input_ids'])})")
    print(f"  B: {correct_B}/{total}  |  C: {correct_C}/{total}")
    print(f"  {'Pos':<4} {'Actual':<22} {'B pred':<22} {'C pred':<22} {'B✓':<4} {'C✓'}")
    print(f"  {'---':<4} {'------':<22} {'------':<22} {'------':<22} {'--':<4} {'--'}")
    for i, ((pb, tgt_b), (pc, tgt_c)) in enumerate(zip(res_B, res_C)):
        # tgt_b and tgt_c should be the same token (same eval sequence)
        actual_str  = decode(tgt_b)
        pred_b_str  = decode(pb)
        pred_c_str  = decode(pc)
        b_ok = "✓" if pb == tgt_b else "✗"
        c_ok = "✓" if pc == tgt_c else "✗"
        print(f"  {i:<4} {actual_str:<22} {pred_b_str:<22} {pred_c_str:<22} {b_ok:<4} {c_ok}")

print(f"\n{'='*72}")
print("Done.")
