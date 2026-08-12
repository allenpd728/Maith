#!/usr/bin/env python3
"""
gen_corpus_manifest.py — Generate a tracked manifest for the corpus on disk.

The corpus (Corpus/corpus.jsonl) is a 60MB build artifact that should NOT be
tracked in git. This script generates a small manifest (Corpus/corpus_manifest.json)
that IS tracked, recording the corpus's content hash, format, stats, and
regeneration command. The invariant checker verifies the corpus on disk
matches this manifest.

Usage:
    python3 python/gen_corpus_manifest.py
    # Writes Corpus/corpus_manifest.json
"""
import hashlib
import json
import os
import subprocess
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
CORPUS = REPO / "Corpus" / "corpus.jsonl"
OUT = REPO / "Corpus" / "corpus_manifest.json"


def sha256_file(path: Path, max_bytes: int = 256 * 1024 * 1024) -> str:
    h = hashlib.sha256()
    n = 0
    with open(path, "rb") as f:
        while n < max_bytes:
            chunk = f.read(1 << 20)
            if not chunk:
                break
            h.update(chunk)
            n += len(chunk)
    return f"sha256:{h.hexdigest()}"


def detect_format(corpus_path: Path) -> str:
    """Determine corpus format from token inspection."""
    gen_named = 0
    gen_bucket = 0
    with open(corpus_path) as f:
        for i, line in enumerate(f):
            if i >= 100:
                break
            d = json.loads(line)
            for t in d.get("tokens", []):
                if isinstance(t, str):
                    if t.startswith("gen:") and t not in ("gen:hof", "gen:proj"):
                        gen_named += 1
                    elif t.startswith("GEN_"):
                        gen_bucket += 1
    if gen_named > 0 and gen_bucket == 0:
        return "per_operator"
    elif gen_bucket > 0 and gen_named == 0:
        return "v2_module"
    elif gen_named > 0 and gen_bucket > 0:
        return "mixed_corrupt"
    return "unknown"


def count_declarations(corpus_path: Path) -> int:
    n = 0
    with open(corpus_path) as f:
        for _ in f:
            n += 1
    return n


def git_sha() -> str:
    try:
        r = subprocess.run(["git", "rev-parse", "HEAD"], cwd=REPO,
                          capture_output=True, text=True, timeout=5)
        return r.stdout.strip() or "unknown"
    except Exception:
        return "unknown"


def main():
    if not CORPUS.exists():
        print(f"ERROR: {CORPUS} not found")
        return 1

    print(f"Hashing {CORPUS}...")
    content_hash = sha256_file(CORPUS)
    fmt = detect_format(CORPUS)
    n_decls = count_declarations(CORPUS)
    file_size = os.path.getsize(CORPUS)

    manifest = {
        "file": "Corpus/corpus.jsonl",
        "content_hash": content_hash,
        "format": fmt,
        "n_declarations": n_decls,
        "file_size_bytes": file_size,
        "generated_by": "lake exe buildCorpus",
        "regeneration_note": "Run `lake exe buildCorpus` (module mode) or "
                             "`lake exe buildCorpus --per-operator` (per-op mode). "
                             "Requires Mathlib built. See AUDIT_2026_08_10.md P0-2.",
        "backup": "Corpus/corpus.per_operator.jsonl",
        "created_by_commit": git_sha(),
    }

    with open(OUT, "w") as f:
        json.dump(manifest, f, indent=2)
    print(f"Wrote {OUT}")
    print(f"  hash: {content_hash[:30]}...")
    print(f"  format: {fmt}")
    print(f"  declarations: {n_decls}")
    print(f"  size: {file_size:,} bytes")
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main())
