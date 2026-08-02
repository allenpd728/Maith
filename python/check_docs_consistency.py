#!/usr/bin/env python3
"""
check_docs_consistency.py

Detect stale documentation values that have caused experiment/reporting drift.
Read-only with respect to training runs.
"""

import argparse
from typing import Optional, List
import json
import re
from dataclasses import dataclass
from pathlib import Path


DOC_GLOBS = ("README.md", "docs/**/*.md")
BANNED_PATTERNS = (
    re.compile(r"\b4,233\b"),
    re.compile(r"\b151643\b"),
)
EMBED_TABLE_RE = re.compile(r"Variant A uses a ([0-9,]+)-token embedding table")


@dataclass
class Finding:
    path: str
    line: int
    message: str


def list_doc_files(repo_root: Path) -> List[Path]:
    files: list[Path] = []
    for pattern in DOC_GLOBS:
        files.extend(repo_root.glob(pattern))
    return sorted(set(p for p in files if p.is_file()))


def load_vocab_size(vocab_path: Path) -> Optional[int]:
    if not vocab_path.exists():
        return None
    with open(vocab_path) as f:
        payload = json.load(f)
    if not isinstance(payload, dict):
        raise ValueError(f"Expected dict in vocab file: {vocab_path}")
    return len(payload)


def scan_docs(files: list[Path], expected_vocab_size: int | None) -> List[Finding]:
    findings: list[Finding] = []
    for path in files:
        text = path.read_text(errors="replace")
        lines = text.splitlines()
        for i, line in enumerate(lines, start=1):
            for pat in BANNED_PATTERNS:
                if pat.search(line):
                    findings.append(Finding(str(path), i, f"banned stale value matched: {pat.pattern}"))

            m = EMBED_TABLE_RE.search(line)
            if m and expected_vocab_size is not None:
                reported = int(m.group(1).replace(",", ""))
                if reported != expected_vocab_size:
                    findings.append(
                        Finding(
                            str(path),
                            i,
                            f"Variant A embedding-table size mismatch: docs={reported}, vocab_A.json={expected_vocab_size}",
                        )
                    )
    return findings


def main() -> int:
    parser = argparse.ArgumentParser(description="Check docs for stale experiment values.")
    parser.add_argument("--repo-root", default=".")
    parser.add_argument("--vocab", default="datasets/vocab_A.json")
    parser.add_argument("--report-out", default="runs/docs_consistency_report.json")
    args = parser.parse_args()

    repo_root = Path(args.repo_root).resolve()
    vocab_path = (repo_root / args.vocab).resolve()
    report_path = (repo_root / args.report_out).resolve()

    expected_vocab_size = load_vocab_size(vocab_path)
    files = list_doc_files(repo_root)
    findings = scan_docs(files, expected_vocab_size=expected_vocab_size)

    report = {
        "repo_root": str(repo_root),
        "checked_files": len(files),
        "expected_variant_a_vocab_size": expected_vocab_size,
        "findings": [
            {"path": f.path, "line": f.line, "message": f.message}
            for f in findings
        ],
        "ok": len(findings) == 0,
    }

    report_path.parent.mkdir(parents=True, exist_ok=True)
    with open(report_path, "w") as f:
        json.dump(report, f, indent=2)
        f.write("\n")

    print(f"Checked {len(files)} markdown files.")
    if expected_vocab_size is not None:
        print(f"Expected variant A vocab size from {args.vocab}: {expected_vocab_size}")
    else:
        print(f"Vocab file not found: {args.vocab} (size check skipped)")

    if findings:
        print(f"FAILED: {len(findings)} findings")
        for finding in findings:
            print(f"- {finding.path}:{finding.line}: {finding.message}")
        return 1

    print("PASS: docs consistency checks passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
