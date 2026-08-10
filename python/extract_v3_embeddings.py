#!/usr/bin/env python3
"""
extract_v3_embeddings.py — Extract embeddings for variant A v3 (per-operator mode).

A v3 was trained by joining IR token strings with spaces and encoding via the
Qwen BPE tokenizer (not custom input_ids). So we need to feed the IR token
strings from the corpus, not the per-op dataset's input_ids.

This is correct for A v3 because A v3 IS an IR variant (trained on IR tokens,
just without C4 GEN_ bucketing — gen:<FullName> tokens are preserved as BPE
subwords in the Qwen tokenizer).
"""
import gc
import json
import os
import sys
from pathlib import Path

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

REPO = Path(__file__).resolve().parent.parent
HIDDEN_SIZE = 896
MAX_SEQ_LEN = 512
SEED = 42

CKPT = REPO / "runs" / "variant_A_v3" / "checkpoint-final"
CORPUS = REPO / "Corpus" / "corpus.jsonl"
DATASETS = REPO / "datasets"
OUT = REPO / "runs" / "retrieval_embeddings"

def get_device():
    if torch.cuda.is_available():
        return "cuda"
    elif torch.backends.mps.is_available():
        return "mps"
    return "cpu"

def patch_tokenizer_config(path):
    cfg_path = os.path.join(path, "tokenizer_config.json")
    if not os.path.exists(cfg_path):
        return
    with open(cfg_path) as f:
        cfg = json.load(f)
    if isinstance(cfg.get("extra_special_tokens"), list):
        cfg["extra_special_tokens"] = {}
        with open(cfg_path, "w") as f:
            json.dump(cfg, f, indent=2)

def main():
    device = get_device()
    print(f"Device: {device}")

    # Load A v3 model
    print(f"Loading A v3 checkpoint: {CKPT}")
    patch_tokenizer_config(str(CKPT))
    model = AutoModelForCausalLM.from_pretrained(str(CKPT), dtype=torch.float32, trust_remote_code=True).to(device)
    tokenizer = AutoTokenizer.from_pretrained(str(CKPT), trust_remote_code=True)
    model.eval()
    for p in model.parameters():
        p.requires_grad = False

    # Need to null bos/eos (151643 is out of range for 8102-vocab)
    model.config.bos_token_id = None
    model.config.eos_token_id = None

    # Load the same train/eval split as other variants (from datasets/train_A.jsonl)
    # to get the example_id ordering, then match to corpus tokens
    train_ids = set()
    eval_ids = set()
    train_order = []
    eval_order = []
    for split, path, ids_set, order in [
        ("train", DATASETS / "train_A.jsonl", train_ids, train_order),
        ("eval", DATASETS / "eval_A.jsonl", eval_ids, eval_order),
    ]:
        with open(path) as f:
            for line in f:
                d = json.loads(line)
                eid = d["example_id"]
                ids_set.add(eid)
                order.append(eid)

    # Load corpus to get tokens by example_id
    corpus_by_id = {}
    with open(CORPUS) as f:
        for line in f:
            d = json.loads(line)
            eid = f"{d.get('module','')}::{d.get('name','')}"
            corpus_by_id[eid] = d

    torch.manual_seed(SEED)

    for split_name, order in [("train", train_order), ("eval", eval_order)]:
        all_embeddings = []
        all_names = []

        print(f"Extracting {split_name} embeddings for {len(order)} examples...")

        for i, eid in enumerate(order):
            if (i + 1) % 500 == 0 or i == 0:
                print(f"  {split_name} {i+1}/{len(order)}...")

            ex = corpus_by_id.get(eid)
            if not ex:
                print(f"  WARNING: {eid} not in corpus, using zeros")
                all_embeddings.append(torch.zeros(1, HIDDEN_SIZE))
                all_names.append(eid.split("::")[-1] if "::" in eid else eid)
                continue

            tokens = ex.get("tokens", [])
            input_str = " ".join(tokens)
            encoded = tokenizer(input_str, return_tensors="pt", truncation=True,
                               max_length=MAX_SEQ_LEN, add_special_tokens=False)
            input_ids = encoded["input_ids"].to(device)
            attn_mask = encoded["attention_mask"].to(device)

            with torch.no_grad():
                outputs = model(input_ids=input_ids, attention_mask=attn_mask,
                               output_hidden_states=True)
                final_hidden = outputs.hidden_states[-1]
                pooled = final_hidden.mean(dim=1)

            all_embeddings.append(pooled.cpu())
            all_names.append(ex.get("name", ""))

        embeddings = torch.cat(all_embeddings, dim=0)
        out_path = OUT / f"embeddings_A_v3_{split_name}.pt"
        torch.save(embeddings, out_path)
        print(f"  Saved {embeddings.shape} -> {out_path}")

        names_path = OUT / f"names_A_v3_{split_name}.json"
        with open(names_path, "w") as f:
            json.dump(all_names, f)

    del model, tokenizer
    gc.collect()
    if device == "mps":
        torch.mps.empty_cache()
    print("Done.")

if __name__ == "__main__":
    main()
