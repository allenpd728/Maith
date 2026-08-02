"""
Standalone perplexity evaluator for a saved checkpoint.
Usage: python3 python/eval_checkpoint.py --variant B --checkpoint runs/variant_B/checkpoint-final --datasets datasets/
"""
import argparse, gc, json, math, os, sys
from pathlib import Path

import torch
from transformers import AutoModelForCausalLM, AutoTokenizer

# Reuse dataset/collate helpers from train.py
sys.path.insert(0, str(Path(__file__).parent))
from train import IRDataset, collate_fn

def evaluate_perplexity(model, dataset, device, batch_size=1):
    model.eval()
    total_loss = 0.0
    total_tokens = 0
    with torch.no_grad():
        for start in range(0, len(dataset), batch_size):
            batch = [dataset[i] for i in range(start, min(start + batch_size, len(dataset)))]
            batch_tensors = collate_fn(batch)
            batch_tensors = {k: v.to(device) for k, v in batch_tensors.items()}
            outputs = model(**batch_tensors)
            loss = outputs.loss
            n_tokens = (batch_tensors["labels"] != -100).sum().item()
            total_loss += loss.item() * n_tokens
            total_tokens += n_tokens
    avg_loss = total_loss / max(total_tokens, 1)
    return math.exp(avg_loss)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--variant", required=True)
    parser.add_argument("--checkpoint", required=True)
    parser.add_argument("--datasets", required=True)
    parser.add_argument("--seq-len", type=int, default=512)
    args = parser.parse_args()

    device = "mps" if torch.backends.mps.is_available() else "cpu"
    print(f"Device: {device}")
    print(f"Loading checkpoint: {args.checkpoint}")

    model = AutoModelForCausalLM.from_pretrained(args.checkpoint, trust_remote_code=True)
    model = model.to(device)

    eval_file = Path(args.datasets) / f"eval_{args.variant}.jsonl"
    print(f"Eval dataset: {eval_file}")
    eval_dataset = IRDataset(str(eval_file), max_len=args.seq_len)
    print(f"  {len(eval_dataset)} examples")

    print("Flushing MPS cache ...")
    if device == "mps":
        gc.collect()
        torch.mps.empty_cache()

    print("Computing perplexity ...")
    ppl = evaluate_perplexity(model, eval_dataset, device)
    print(f"\nVariant {args.variant} checkpoint perplexity: {ppl:.4f}")

    # Patch results.json with the 3-epoch result
    results_path = Path(args.checkpoint).parent / "results.json"
    if results_path.exists():
        with open(results_path) as f:
            results = json.load(f)
        results["eval_perplexity_3ep"] = round(ppl, 4)
        results["eval_perplexity_3ep_checkpoint"] = args.checkpoint
        with open(results_path, "w") as f:
            json.dump(results, f, indent=2)
        print(f"Patched {results_path} with eval_perplexity_3ep: {ppl:.4f}")

if __name__ == "__main__":
    main()
