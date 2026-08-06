#!/usr/bin/env python3
"""
Step 1 — AST Explorer
Branch: kit/ir-design-research
Output: ir-research/findings/ast_explorer_output.json

Reads corpus.jsonl, parses each leanExpr string into an annotated AST tree,
then produces:
  1. Tree views for 10 sampled declarations (diverse kinds)
  2. Full corpus node-type frequency table
  3. List of node types present in leanExpr but absent in the IR graph

Usage:
    python3 ir-research/scripts/ast_explorer.py \
        --corpus Corpus/corpus.jsonl \
        --output ir-research/findings/ast_explorer_output.json
"""

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path


# ---------------------------------------------------------------------------
# Lean Expr string parser
# ---------------------------------------------------------------------------
# leanExpr is a pretty-printed elaborated term, not JSON.  We tokenise it
# into a tree of nodes using a recursive-descent approach that handles:
#   - Named application:   f a b c
#   - Forall / fun:        forall (x : T), body  |  fun (x : T) => body
#   - Sort / Prop / Type:  Sort.{u}  Prop  Type.{u}
#   - Constants:           Foo.bar.{u_1 u_2}
#   - Variables:           inst._@... (hygCtx names)
#   - OfNat / let:         OfNat.ofNat ...  let x := ...
#   - Parenthesised:       (expr)
#   - Brackets:            {expr} [expr]
#
# We DON'T try to produce a full Lean elaboration — we extract node types
# for coverage analysis, so a conservative tokeniser is fine.

# Regexp for the leading keyword/head of any sub-expression
_KEYWORD_RE = re.compile(
    r"""
    (?P<forall>  \bforall\b ) |
    (?P<fun>     \bfun\b    ) |
    (?P<let>     \blet\b    ) |
    (?P<sort>    \bSort\b(?:\.\{[^}]*\})? ) |
    (?P<prop>    \bProp\b   ) |
    (?P<type_kw> \bType\b(?:\.\{[^}]*\})? ) |
    (?P<nat_lit> \b\d+\b    ) |
    (?P<ident>   [A-Za-z_@][A-Za-z0-9._@'?\-]* (?:\.\{[^}]*\})? )
    """,
    re.VERBOSE,
)


def _classify_head(token: str) -> str:
    """Return a coarse node-type label for a head token."""
    if token == "forall":
        return "Expr.forallE"
    if token == "fun":
        return "Expr.lam"
    if token == "let":
        return "Expr.letE"
    if token.startswith("Sort"):
        return "Expr.sort"
    if token in ("Prop", "Type") or token.startswith("Type.{"):
        return "Expr.sort"
    if re.fullmatch(r"\d+", token):
        return "Expr.lit.nat"
    # Heuristics for common Lean names
    if token.startswith("@"):
        return "Expr.const.explicit"
    if re.match(r"[A-Z]", token):
        return "Expr.const"
    if token.startswith("inst."):
        return "Expr.fvar.inst"
    if token.startswith("_") or "._hyg" in token or "._@" in token:
        return "Expr.fvar.hyg"
    return "Expr.app_head"


def _split_top_level(s: str) -> list[str]:
    """
    Split a string on spaces/commas at depth 0 (ignoring content inside
    balanced parens/braces/brackets).
    """
    parts = []
    depth = 0
    cur = []
    for ch in s:
        if ch in "({[":
            depth += 1
            cur.append(ch)
        elif ch in ")}]":
            depth -= 1
            cur.append(ch)
        elif ch in " ,\n\t" and depth == 0:
            tok = "".join(cur).strip()
            if tok:
                parts.append(tok)
            cur = []
        else:
            cur.append(ch)
    tok = "".join(cur).strip()
    if tok:
        parts.append(tok)
    return parts


def parse_expr(expr: str, depth: int = 0, max_depth: int = 6) -> dict:
    """
    Recursively parse a leanExpr string into a lightweight node dict:
      { "node_type": str, "head": str, "depth": int,
        "arity": int, "children": [...] }
    Stops recursing at max_depth to keep output manageable.
    """
    expr = expr.strip()
    if not expr:
        return {"node_type": "Expr.unknown", "head": "", "depth": depth, "arity": 0, "children": []}

    # Strip outer parens/braces/brackets
    if (
        (expr.startswith("(") and expr.endswith(")"))
        or (expr.startswith("{") and expr.endswith("}"))
        or (expr.startswith("[") and expr.endswith("]"))
    ):
        inner = expr[1:-1].strip()
        # Only unwrap if the brackets are balanced (i.e. the outer pair matches)
        depth_check = 0
        balanced = True
        for i, ch in enumerate(expr[1:-1]):
            if ch in "({[":
                depth_check += 1
            elif ch in ")}]":
                depth_check -= 1
            if depth_check < 0:
                balanced = False
                break
        if balanced:
            expr = inner

    # Identify head token
    m = _KEYWORD_RE.match(expr)
    if not m:
        return {"node_type": "Expr.unknown", "head": expr[:40], "depth": depth, "arity": 0, "children": []}

    head = m.group(0)
    node_type = _classify_head(head)
    rest = expr[m.end():].strip()

    if depth >= max_depth or not rest:
        return {"node_type": node_type, "head": head, "depth": depth, "arity": 0, "children": []}

    # Split rest into top-level arguments and recurse
    args = _split_top_level(rest)
    children = [parse_expr(a, depth + 1, max_depth) for a in args[:8]]  # cap at 8 children

    return {
        "node_type": node_type,
        "head": head,
        "depth": depth,
        "arity": len(args),
        "children": children,
    }


def collect_node_types(node: dict, counter: Counter) -> None:
    counter[node["node_type"]] += 1
    for child in node.get("children", []):
        collect_node_types(child, counter)


# ---------------------------------------------------------------------------
# IR node-type extractor
# ---------------------------------------------------------------------------
# Known IR row/token prefixes that the current encoder emits

IR_NODE_PREFIXES = {
    # Structural tokens
    "SEP", "PAD", "UNK",
    # Row types
    "E:",    # entity
    "R:",    # relation
    "O:",    # operation
    "A:",    # attribute
    # Value tokens
    "N:", "S:", "B:",
    # Kind labels
    "def", "thm", "axiom", "opaque", "abbrev", "instance",
    "forall", "fun", "app", "const", "fvar", "bvar",
    "mvar", "sort", "lit", "proj", "letE", "mdata",
    # Relation types
    "arg", "ret", "body", "type", "inst", "param",
    "field", "alt", "cond", "recCase", "major",
    # Polarity (legacy)
    "pos", "neg", "neut",
}

EXPR_TO_IR_MAPPING = {
    "Expr.forallE":       "forall",
    "Expr.lam":           "fun",
    "Expr.app_head":      "app",
    "Expr.const":         "const",
    "Expr.const.explicit": "const",
    "Expr.fvar.inst":     "fvar",
    "Expr.fvar.hyg":      "fvar",
    "Expr.sort":          "sort",
    "Expr.lit.nat":       "lit",
    "Expr.letE":          "letE",
    "Expr.unknown":       None,
}


# ---------------------------------------------------------------------------
# Sampling: pick 10 declarations spanning interesting kinds
# ---------------------------------------------------------------------------

TARGET_KINDS = [
    "simple_lemma",        # short leanExpr, starts with forall
    "recursive_def",       # contains rec or recOn
    "typeclass_instance",  # contains 'inst' in name or expr
    "dependent_type",      # nested forall with type universes
    "inductive_recursor",  # name ends with .rec
    "lambda_term",         # starts with fun
    "let_binding",         # contains let
    "sort_expr",           # just Sort/Type/Prop
    "application",         # long chain of app
    "nat_literal",         # contains nat literal
]


def score_declaration(entry: dict) -> dict[str, int]:
    """Score a corpus entry for each target kind."""
    name = entry.get("name", "")
    expr = entry.get("leanExpr", "")
    scores = {k: 0 for k in TARGET_KINDS}

    if expr.startswith("forall") and len(expr) < 300:
        scores["simple_lemma"] += 1
    if "rec" in name or "recOn" in expr or "casesOn" in expr:
        scores["recursive_def"] += 1
    if "inst" in name.lower() or "inst." in expr:
        scores["typeclass_instance"] += 1
    if expr.count("forall") > 2 and "Sort.{" in expr:
        scores["dependent_type"] += 1
    if name.endswith(".rec"):
        scores["inductive_recursor"] += 1
    if expr.startswith("fun"):
        scores["lambda_term"] += 1
    if "let " in expr:
        scores["let_binding"] += 1
    if expr.strip() in ("Prop", "Type") or re.match(r"Sort\.\{", expr):
        scores["sort_expr"] += 1
    if expr.count("(") > 10:
        scores["application"] += 1
    if re.search(r"\b\d+\b", expr):
        scores["nat_literal"] += 1

    return scores


def select_samples(entries: list[dict], n: int = 10) -> list[dict]:
    """Greedily pick one best entry per target kind, up to n total."""
    selected = []
    covered_kinds = set()
    kind_best: dict[str, tuple[int, dict]] = {}

    for entry in entries:
        scores = score_declaration(entry)
        for kind, score in scores.items():
            if score > 0 and kind not in covered_kinds:
                prev_score, _ = kind_best.get(kind, (0, None))
                if score > prev_score:
                    kind_best[kind] = (score, entry)

    for kind in TARGET_KINDS:
        if kind in kind_best and len(selected) < n:
            _, entry = kind_best[kind]
            if entry not in selected:
                selected.append(entry)
                covered_kinds.add(kind)

    # Fill remaining slots with longest exprs not already selected
    remaining = [e for e in entries if e not in selected]
    remaining.sort(key=lambda e: len(e.get("leanExpr", "")), reverse=True)
    while len(selected) < n and remaining:
        selected.append(remaining.pop(0))

    return selected[:n]


# ---------------------------------------------------------------------------
# Tree rendering (for human-readable findings summary)
# ---------------------------------------------------------------------------

def render_tree(node: dict, prefix: str = "", is_last: bool = True) -> list[str]:
    connector = "└── " if is_last else "├── "
    label = f"{node['node_type']}  [{node['head'][:30]}]  arity={node['arity']}"
    lines = [prefix + connector + label]
    children = node.get("children", [])
    for i, child in enumerate(children):
        extension = "    " if is_last else "│   "
        lines.extend(render_tree(child, prefix + extension, i == len(children) - 1))
    return lines


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description="Step 1 AST Explorer")
    parser.add_argument("--corpus", default="Corpus/corpus.jsonl")
    parser.add_argument("--output", default="ir-research/findings/ast_explorer_output.json")
    parser.add_argument("--max-depth", type=int, default=6)
    args = parser.parse_args()

    corpus_path = Path(args.corpus)
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    print(f"Loading corpus from {corpus_path} ...", file=sys.stderr)
    entries = []
    with corpus_path.open() as f:
        for line in f:
            line = line.strip()
            if line:
                entries.append(json.loads(line))
    print(f"  Loaded {len(entries)} declarations", file=sys.stderr)

    # -------------------------------------------------------------------
    # Pass 1: full corpus node-type frequency
    # -------------------------------------------------------------------
    print("Pass 1: counting node types across full corpus ...", file=sys.stderr)
    corpus_node_counts: Counter = Counter()
    for entry in entries:
        expr = entry.get("leanExpr", "")
        if not expr:
            continue
        tree = parse_expr(expr, max_depth=args.max_depth)
        collect_node_types(tree, corpus_node_counts)

    # -------------------------------------------------------------------
    # Pass 2: IR coverage mapping
    # -------------------------------------------------------------------
    all_ast_types = set(corpus_node_counts.keys())
    covered_by_ir = {t for t in all_ast_types if EXPR_TO_IR_MAPPING.get(t) is not None}
    uncovered = sorted(all_ast_types - covered_by_ir)

    # -------------------------------------------------------------------
    # Pass 3: sample 10 declarations and build trees
    # -------------------------------------------------------------------
    print("Pass 3: selecting and parsing sample declarations ...", file=sys.stderr)
    samples = select_samples(entries, n=10)
    sample_trees = []
    for entry in samples:
        expr = entry.get("leanExpr", "")
        tree = parse_expr(expr, max_depth=args.max_depth)
        tree_lines = render_tree(tree)
        kind_scores = score_declaration(entry)
        best_kind = max(kind_scores, key=lambda k: kind_scores[k])
        sample_trees.append({
            "name": entry.get("name", ""),
            "module": entry.get("module", ""),
            "kind": best_kind,
            "leanExpr_length": len(expr),
            "leanExpr_preview": expr[:200],
            "tree": tree,
            "tree_text": "\n".join(tree_lines),
        })
        print(f"  Sampled: {entry.get('name', '')} ({best_kind})", file=sys.stderr)

    # -------------------------------------------------------------------
    # Output
    # -------------------------------------------------------------------
    output = {
        "corpus_size": len(entries),
        "node_type_frequency": dict(corpus_node_counts.most_common()),
        "ir_coverage": {
            "total_ast_node_types": len(all_ast_types),
            "covered_by_ir": len(covered_by_ir),
            "uncovered_count": len(uncovered),
            "uncovered_types": uncovered,
            "mapping": {t: EXPR_TO_IR_MAPPING.get(t) for t in sorted(all_ast_types)},
        },
        "samples": sample_trees,
    }

    with output_path.open("w") as f:
        json.dump(output, f, indent=2)

    print(f"\nOutput written to {output_path}", file=sys.stderr)

    # Print summary to stdout
    print("\n=== AST Node Type Frequency (top 20) ===")
    for nt, count in corpus_node_counts.most_common(20):
        ir_label = EXPR_TO_IR_MAPPING.get(nt, "UNMAPPED")
        covered = "✅" if ir_label else "❌"
        print(f"  {covered}  {nt:<30}  {count:>6}   IR: {ir_label or '(none)'}")

    print(f"\n=== IR Coverage ===")
    print(f"  AST node types found:    {len(all_ast_types)}")
    print(f"  Covered by IR:           {len(covered_by_ir)}")
    print(f"  NOT covered by IR:       {len(uncovered)}")
    if uncovered:
        print(f"  Uncovered types:")
        for t in uncovered:
            print(f"    - {t}  (count: {corpus_node_counts[t]})")

    print(f"\n=== Sampled Declarations ===")
    for s in sample_trees:
        print(f"\n  [{s['kind']}] {s['name']}")
        print(f"  expr length: {s['leanExpr_length']}")
        for line in s["tree_text"].split("\n")[:12]:
            print(f"    {line}")


if __name__ == "__main__":
    main()
