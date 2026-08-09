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

Token serialisation (Encoder v1.2.0, as emitted by `Encoder.lean` and expected by `Decoder.lean`):

- Free variables (named entities, constants) → the name string directly (e.g. `"HMul.hMul"`)
- Forall-bound variables → `"FVAR_<N>"` (e.g. `"FVAR_0"`, `"FVAR_1"`) — positional, cap 63, overflow → `"FVAR_MANY"`
- Lambda-bound variables → `"BVAR_<N>"` (e.g. `"BVAR_0"`, `"BVAR_1"`) — positional, cap 63, overflow → `"BVAR_MANY"`
- Term entities → `"TERM_<N>"` (e.g. `"TERM_0"`, `"TERM_3"`)
- Generic operations → `"gen:<FullName>"` (e.g. `"gen:HAdd.hAdd"`)

The `scope` field in `EntityId.bound` must contain exactly two `/` separators (format: `declName/depth/binderName`); the scope encodes forall vs lambda via prefix (`∀:` or `λ:`). The encoder maps these to positional FVAR/BVAR tokens; the decoder reconstructs scoped names for lossless round-trip.

`stats.json` additionally records aggregate counters and
`failureStats.failureReasons`, which stores explicit unsupported/failure reasons.
