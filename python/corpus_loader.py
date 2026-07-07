from __future__ import annotations

import json
import random
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, Iterable, List, Sequence, Tuple


class SchemaError(ValueError):
    pass


@dataclass(frozen=True)
class TrainingExample:
    name: str
    module: str
    lean_expr: str
    graph: Dict[str, Any]
    tokens: List[str]


def _expect(condition: bool, message: str) -> None:
    if not condition:
        raise SchemaError(message)


def _validate_entity_id(entity_id: Any, path: str) -> None:
    _expect(isinstance(entity_id, dict), f"{path} must be an object")
    kind = entity_id.get("kind")
    _expect(kind in {"var", "term"}, f"{path}.kind must be 'var' or 'term'")
    if kind == "var":
        _expect(isinstance(entity_id.get("name"), str), f"{path}.name must be a string")
    else:
        _expect(isinstance(entity_id.get("index"), int), f"{path}.index must be an integer")


def _validate_graph(graph: Any, path: str) -> None:
    _expect(isinstance(graph, dict), f"{path} must be an object")
    for field in ("entities", "attributes", "relations", "operations"):
        _expect(isinstance(graph.get(field), list), f"{path}.{field} must be an array")

    for i, entity in enumerate(graph["entities"]):
        _expect(isinstance(entity, dict), f"{path}.entities[{i}] must be an object")
        _validate_entity_id(entity.get("id"), f"{path}.entities[{i}].id")
        _expect(isinstance(entity.get("polarity"), str), f"{path}.entities[{i}].polarity must be a string")

    for i, attr in enumerate(graph["attributes"]):
        _expect(isinstance(attr, dict), f"{path}.attributes[{i}] must be an object")
        _validate_entity_id(attr.get("target"), f"{path}.attributes[{i}].target")
        _expect(isinstance(attr.get("key"), str), f"{path}.attributes[{i}].key must be a string")
        _expect(isinstance(attr.get("value"), str), f"{path}.attributes[{i}].value must be a string")
        _expect(isinstance(attr.get("polarity"), str), f"{path}.attributes[{i}].polarity must be a string")

    for i, rel in enumerate(graph["relations"]):
        _expect(isinstance(rel, dict), f"{path}.relations[{i}] must be an object")
        _validate_entity_id(rel.get("src"), f"{path}.relations[{i}].src")
        _validate_entity_id(rel.get("tgt"), f"{path}.relations[{i}].tgt")
        _expect(isinstance(rel.get("op"), str), f"{path}.relations[{i}].op must be a string")
        _expect(isinstance(rel.get("polarity"), str), f"{path}.relations[{i}].polarity must be a string")

    for i, op in enumerate(graph["operations"]):
        _expect(isinstance(op, dict), f"{path}.operations[{i}] must be an object")
        _expect(isinstance(op.get("inputs"), list), f"{path}.operations[{i}].inputs must be an array")
        for j, input_id in enumerate(op["inputs"]):
            _validate_entity_id(input_id, f"{path}.operations[{i}].inputs[{j}]")
        _validate_entity_id(op.get("output"), f"{path}.operations[{i}].output")
        _expect(isinstance(op.get("op"), str), f"{path}.operations[{i}].op must be a string")
        _expect(isinstance(op.get("polarity"), str), f"{path}.operations[{i}].polarity must be a string")


def validate_line(obj: Dict[str, Any], line_number: int) -> TrainingExample:
    prefix = f"line {line_number}"
    _expect(isinstance(obj.get("name"), str), f"{prefix}: name must be a string")
    _expect(isinstance(obj.get("module"), str), f"{prefix}: module must be a string")
    _expect(isinstance(obj.get("leanExpr"), str), f"{prefix}: leanExpr must be a string")
    _validate_graph(obj.get("graph"), f"{prefix}: graph")
    tokens = obj.get("tokens")
    _expect(isinstance(tokens, list), f"{prefix}: tokens must be an array")
    _expect(all(isinstance(token, str) for token in tokens), f"{prefix}: each token must be a string")
    return TrainingExample(
        name=obj["name"],
        module=obj["module"],
        lean_expr=obj["leanExpr"],
        graph=obj["graph"],
        tokens=tokens,
    )


def load_corpus(path: str | Path) -> List[TrainingExample]:
    path = Path(path)
    examples: List[TrainingExample] = []
    with path.open("r", encoding="utf-8") as handle:
        for line_number, raw_line in enumerate(handle, start=1):
            line = raw_line.strip()
            if not line:
                continue
            obj = json.loads(line)
            examples.append(validate_line(obj, line_number))
    return examples


def build_vocabulary(examples: Sequence[TrainingExample], min_freq: int = 1) -> Dict[str, int]:
    counts: Dict[str, int] = {}
    for example in examples:
        for token in example.tokens:
            counts[token] = counts.get(token, 0) + 1

    vocab = {"<PAD>": 0, "<UNK>": 1}
    for token, count in sorted(counts.items(), key=lambda item: (-item[1], item[0])):
        if count >= min_freq:
            vocab[token] = len(vocab)
    return vocab


def train_eval_split(
    examples: Sequence[TrainingExample],
    eval_ratio: float = 0.2,
    seed: int = 0,
) -> Tuple[List[TrainingExample], List[TrainingExample]]:
    if not 0.0 < eval_ratio < 1.0:
        raise ValueError("eval_ratio must be between 0 and 1")

    shuffled = list(examples)
    rng = random.Random(seed)
    rng.shuffle(shuffled)
    eval_size = max(1, int(len(shuffled) * eval_ratio)) if shuffled else 0
    eval_set = shuffled[:eval_size]
    train_set = shuffled[eval_size:]
    return train_set, eval_set


class CorpusDataset:
    def __init__(self, examples: Sequence[TrainingExample], vocab: Dict[str, int]) -> None:
        self._examples = list(examples)
        self._vocab = vocab
        self._unk_id = vocab["<UNK>"]

    def __len__(self) -> int:
        return len(self._examples)

    def __getitem__(self, index: int) -> Dict[str, Any]:
        example = self._examples[index]
        token_ids = [self._vocab.get(token, self._unk_id) for token in example.tokens]
        return {
            "name": example.name,
            "module": example.module,
            "token_ids": token_ids,
            "graph": example.graph,
        }
