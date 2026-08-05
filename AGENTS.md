# Agent Guidelines

## Branch Workflow

```
main                    # Stable, production-ready
  └── kit/dev          # Integration branch (Kit's review before main)
       └── openhands/dev  # OpenHands staging branch (submit PRs here)
            └── feature/openhands-xxx  # Feature branches (your work)
```

**Rules:**
1. **Create feature branches from `openhands/dev`**: `git checkout -b feature/your-feature openhands/dev`
2. **When ready for review**: Open a PR from your feature branch → `openhands/dev`
3. **Kit will PR `openhands/dev` → `kit/dev`**: After batch review, Kit merges into kit/dev
4. **Never push directly to `kit/dev` or `main`**: These require human review

## Quick Start

```bash
# Start a new task
git checkout openhands/dev
git checkout -b feature/your-task-name

# After completing work
git push -u origin feature/your-task-name
# Then open PR: feature/your-task-name → openhands/dev
```

## Testing Before Submitting PR

Run all tests in the `python/` directory that exist:

```bash
python3 python/test_build_dataset.py
python3 python/test_train_regression.py
python3 python/test_variant_config.py
python3 python/test_probing_scripts.py
```

## Dependencies

See [`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md) for required packages.
