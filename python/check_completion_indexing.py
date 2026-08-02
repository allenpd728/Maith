#!/usr/bin/env python3
"""
check_completion_indexing.py

Sanity-check that the single-pass teacher-forced completion (new method)
produces bit-identical predicted tokens to the original token-by-token loop
(old method) on the same 5 examples from each variant.

Prints each example's predictions side-by-side and flags any mismatch.
Exits with code 1 if any mismatch is found.
"""

import json
import pathlib
import sys

missing = []
try:
    import torch
except ImportError:
    missing.append("torch")
try:
    from transformers import AutoModelForCausalLM
except ImportError:
    missing.append("transformers")
if missing:
    print(f"ERROR: missing dependencies: {', '.join(missing)}")
    sys.exit(1)

VARIANTS = {
    "A": {
        "ckpt": "runs/variant_A_v3/checkpoint-final",
        "eval": "datasets/eval_A.jsonl",
    },
    "B": {
        "ckpt": "runs/variant_B2/checkpoint-final",
        "eval": "datasets/eval_B.jsonl",
    },
    "C": {
        "ckpt": "runs/variant_C_v2/checkpoint-final",
        "eval": "datasets/eval_C.jsonl",
    },
}

MASK_LAST   = 10
CHECK_N     = 5   # examples per variant


def load_examples(path):
    return [json.loads(l) for l in pathlib.Path(path).read_text().strip().splitlines() if l.strip()]


def old_method(model, ids, mask_last, device):
    """Original token-by-token loop."""
    prefix_len = len(ids) - mask_last
    current    = ids[:prefix_len]
    preds      = []
    with torch.no_grad():
        for tgt in ids[prefix_len:prefix_len + mask_last]:
            inp    = torch.tensor(current, dtype=torch.long).unsqueeze(0).to(device)
            logits = model(input_ids=inp).logits
            if device == "mps":
                torch.mps.synchronize()
            preds.append(int(logits[0, -1].argmax().item()))
            current = current + [tgt]
    return preds


def new_method(model, ids, mask_last, device):
    """Single-pass teacher-forced method."""
    prefix_len = len(ids) - mask_last
    with torch.no_grad():
        inp         = torch.tensor(ids, dtype=torch.long).unsqueeze(0).to(device)
        logits      = model(input_ids=inp).logits   # [1, seq_len, vocab]
        if device == "mps":
            torch.mps.synchronize()
        pred_logits = logits[0, prefix_len - 1 : prefix_len + mask_last - 1, :]
        return pred_logits.argmax(dim=-1).tolist()


def main():
    if not pathlib.Path("lakefile.lean").exists():
        print("ERROR: run from the Maith repo root")
        sys.exit(1)

    device = "mps"  if torch.backends.mps.is_available() else \
             "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Checking {CHECK_N} examples per variant, mask_last={MASK_LAST}")
    print()

    any_mismatch = False

    for variant, cfg in VARIANTS.items():
        ckpt_path = pathlib.Path(cfg["ckpt"])
        if not ckpt_path.exists():
            print(f"[{variant}] checkpoint not found, skipping")
            continue

        print(f"{'='*60}")
        print(f"  Variant {variant}")
        print(f"{'='*60}")
        print(f"  Loading model...", flush=True)
        model = AutoModelForCausalLM.from_pretrained(
            str(ckpt_path), trust_remote_code=True, dtype=torch.float32
        ).to(device)
        model.eval()
        print(f"  Model loaded.", flush=True)

        examples = load_examples(cfg["eval"])
        # Pick CHECK_N examples spread evenly through the eval set
        step     = max(1, len(examples) // CHECK_N)
        selected = examples[::step][:CHECK_N]

        variant_ok = True
        for i, ex in enumerate(selected):
            ids  = ex["input_ids"]
            name = ex.get("name", "?")
            if len(ids) <= MASK_LAST:
                print(f"  [{i+1}] {name!r:.40s}: too short, skipping")
                continue

            prefix_len = len(ids) - MASK_LAST
            targets    = ids[prefix_len:prefix_len + MASK_LAST]

            print(f"  [{i+1}] {name!r:.40s} (seq_len={len(ids)}, prefix={prefix_len})", flush=True)

            old_preds = old_method(model, ids, MASK_LAST, device)
            new_preds = new_method(model, ids, MASK_LAST, device)

            match = old_preds == new_preds
            status = "OK" if match else "MISMATCH"
            print(f"       old: {old_preds}")
            print(f"       new: {new_preds}")
            print(f"       tgt: {targets}")
            print(f"       --> {status}")

            if not match:
                variant_ok   = False
                any_mismatch = True
                for pos, (o, n, t) in enumerate(zip(old_preds, new_preds, targets)):
                    if o != n:
                        print(f"       DIFF at position {pos}: old={o} new={n} target={t}")

        # Free memory
        del model
        if device == "mps":
            torch.mps.empty_cache()
        elif device == "cuda":
            torch.cuda.empty_cache()

        print(f"  Variant {variant}: {'ALL MATCH' if variant_ok else 'MISMATCHES FOUND'}")
        print()

    if any_mismatch:
        print("RESULT: FAIL — methods diverge, do not trust followup_eval.py results until fixed")
        sys.exit(1)
    else:
        print("RESULT: PASS — both methods produce identical predictions on all checked examples")
        sys.exit(0)


if __name__ == "__main__":
    main()
