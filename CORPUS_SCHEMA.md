# corpus.jsonl schema

Each line is one JSON object with this structure:

```json
{
  "name": "Nat.zero",
  "module": "Init.Prelude",
  "leanExpr": "Nat",
  "graph": {
    "entities": [
      { "id": { "kind": "var", "name": "Nat" }, "polarity": "neut" }
    ],
    "attributes": [],
    "relations": [],
    "operations": []
  },
  "tokens": ["GRAPH_BEGIN", "E", "Nat", "neut", "GRAPH_END"]
}
```

Notes:

- `name`: declaration name from Lean environment.
- `module`: module owning the declaration.
- `leanExpr`: serialized elaborated declaration type (`ConstantInfo.type`).
- `graph`: extracted IR graph.
- `tokens`: encoder output for the normalized graph.

`graph.id` values:

- `{ "kind": "var", "name": "<string>" }` — named free variable or constant reference
- `{ "kind": "term", "index": <nat> }` — anonymous intermediate term
- `{ "kind": "bound", "scope": "<declName>/<depth>/<binderName>" }` — scoped bound variable from a `∀`/`Π` binder; the `scope` field embeds the declaration name so that identically-named binders in different declarations produce distinct tokens

`stats.json` additionally records aggregate counters and
`failureStats.failureReasons`, which stores explicit unsupported/failure reasons.
