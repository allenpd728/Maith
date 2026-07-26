#!/usr/bin/env python3
"""
check_decision_log.py

Validate that docs/DECISION_LOG.md contains required canonical decisions.
"""

import argparse
from pathlib import Path


REQUIRED_DECISIONS = (
    "DEC-001",
    "DEC-002",
    "DEC-003",
    "DEC-004",
    "DEC-005",
)


def main() -> int:
    parser = argparse.ArgumentParser(description="Validate canonical decision-log coverage.")
    parser.add_argument("--path", default="docs/DECISION_LOG.md")
    args = parser.parse_args()

    path = Path(args.path)
    if not path.exists():
        print(f"Decision log not found: {path}")
        return 1

    text = path.read_text(errors="replace")
    missing = [d for d in REQUIRED_DECISIONS if d not in text]
    if missing:
        print(f"FAILED: missing required decision entries: {', '.join(missing)}")
        return 1

    print(f"PASS: decision log includes required entries ({', '.join(REQUIRED_DECISIONS)}).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
