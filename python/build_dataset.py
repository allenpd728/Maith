from __future__ import annotations

import argparse
from pathlib import Path

from corpus_loader import CorpusDataset, build_vocabulary, load_corpus, train_eval_split


def main() -> None:
    parser = argparse.ArgumentParser(description="Load Maith corpus.jsonl and build datasets.")
    parser.add_argument("--corpus", type=Path, required=True, help="Path to corpus.jsonl")
    parser.add_argument("--eval-ratio", type=float, default=0.2, help="Eval split ratio")
    parser.add_argument("--seed", type=int, default=0, help="Shuffle seed")
    args = parser.parse_args()

    examples = load_corpus(args.corpus)
    vocab = build_vocabulary(examples)
    train_examples, eval_examples = train_eval_split(
        examples,
        eval_ratio=args.eval_ratio,
        seed=args.seed,
    )

    train_dataset = CorpusDataset(train_examples, vocab)
    eval_dataset = CorpusDataset(eval_examples, vocab)

    print(f"Loaded examples: {len(examples)}")
    print(f"Vocabulary size: {len(vocab)}")
    print(f"Train examples: {len(train_dataset)}")
    print(f"Eval examples: {len(eval_dataset)}")


if __name__ == "__main__":
    main()
