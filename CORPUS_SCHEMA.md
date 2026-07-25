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
- `{ "kind": "term", "index": <nat> }` — anonymous intermediate term (counter is monotonically increasing within a declaration, reset across declarations)
- `{ "kind": "bound", "scope": "<declName>/<depth>/<binderName>" }` — scoped bound variable from a `∀`/`Π`/`λ` binder; the `scope` field embeds the declaration name so that identically-named binders in different declarations produce distinct tokens

Token serialisation (as emitted by `Encoder.lean` and expected by `Decoder.lean`):

- `var`   → the name string directly (e.g. `"HMul.hMul"`)
- `term`  → `"t<index>"` (e.g. `"t0"`, `"t3"`)
- `bound` → `"b(<scope>)"` (e.g. `"b(mul_assoc/0/a)"`)

The Python validator in `corpus_loader.py` accepts all three kinds. The `scope` field must contain exactly two `/` separators (format: `declName/depth/binderName`); the validator enforces this.

`stats.json` additionally records aggregate counters and
`failureStats.failureReasons`, which stores explicit unsupported/failure reasons.
