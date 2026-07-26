#!/usr/bin/env python3
"""
register_representation.py

Register a new representation candidate in Scripts/representation_families.json.
"""

import argparse
import json
from pathlib import Path


DEFAULT_REGISTRY = Path("Scripts/representation_families.json")


def main() -> int:
    parser = argparse.ArgumentParser(description="Register a new representation candidate.")
    parser.add_argument("--family-id", required=True)
    parser.add_argument("--representation-id", required=True)
    parser.add_argument("--status", default="planned", choices=["planned", "active", "deprecated"])
    parser.add_argument("--core-contract-version", default="core_v1")
    parser.add_argument("--notes", default="")
    parser.add_argument("--set-active", action="store_true", help="Also set active_representation_id")
    parser.add_argument("--registry", default=str(DEFAULT_REGISTRY))
    args = parser.parse_args()

    registry_path = Path(args.registry)
    if not registry_path.exists():
        print(f"Registry not found: {registry_path}")
        return 1

    with open(registry_path) as f:
        registry = json.load(f)

    families = registry.setdefault("families", [])
    for item in families:
        if item.get("representation_id") == args.representation_id:
            print(f"Representation already exists: {args.representation_id}")
            return 1

    new_item = {
        "family_id": args.family_id,
        "representation_id": args.representation_id,
        "status": args.status,
        "core_contract_version": args.core_contract_version,
        "notes": args.notes,
    }
    families.append(new_item)

    if args.set_active:
        registry["active_representation_id"] = args.representation_id
        for item in families:
            if item.get("representation_id") == args.representation_id:
                item["status"] = "active"
            elif item.get("status") == "active":
                item["status"] = "planned"

    with open(registry_path, "w") as f:
        json.dump(registry, f, indent=2)
        f.write("\n")

    print(f"Registered representation: {args.representation_id}")
    print(f"Registry updated: {registry_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
