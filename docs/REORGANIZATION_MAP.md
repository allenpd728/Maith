# Repo Reorganization Path Map

> Use this file when reorganizing `docs/` and the repo root.
> Every old path maps to its new path. After moving files, run:
>
> ```bash
> grep -r "docs/" docs/ --include="*.md" -l
> ```
>
> Then for each match, replace old paths with new paths using this table.
> Delete this file once the reorganization is complete and verified.

---

## docs/ → subdirectory mapping

### docs/decisions/
| Old path | New path | Action |
|---|---|---|
| `docs/DECISION_INDEX.md` | `docs/decisions/INDEX.md` | Move |
| `docs/DECISION_LOG.md` | `docs/decisions/LOG.md` | Move |

### docs/experiments/
| Old path | New path | Action |
|---|---|---|
| `docs/EXPERIMENT_DESIGN.md` | `docs/experiments/EXPERIMENT_DESIGN.md` | Move |
| `docs/V2_COMPARISON_MATRIX.md` | `docs/experiments/V2_COMPARISON_MATRIX.md` | Move |
| `docs/V2_NEXT_STEPS.md` | `docs/experiments/V2_NEXT_STEPS.md` | Move |
| `docs/PROBING_TASK_FINAL.md` | `docs/experiments/PROBING_TASK_FINAL.md` | Move |
| `docs/PROBING_TASK_GUIDE.md` | `docs/experiments/PROBING_TASK_GUIDE.md` | Move |

### docs/reference/
| Old path | New path | Action |
|---|---|---|
| `docs/ENCODER_FORMAT.md` | `docs/reference/ENCODER_FORMAT.md` | Move |
| `docs/PYTHON_PIPELINE.md` | `docs/reference/PYTHON_PIPELINE.md` | Move |
| `docs/REPRESENTATION_EVOLUTION.md` | `docs/reference/REPRESENTATION_EVOLUTION.md` | Move |
| `docs/IR_SCHEMA_AUDIT.md` | `docs/reference/IR_SCHEMA.md` | Move + rename |
| `docs/IR_V2_PROPOSAL.md` | `docs/reference/IR_V2_PROPOSAL.md` | Move |
| `docs/IR_V2_FIX_SPEC.md` | `docs/reference/IR_V2_FIX_SPEC.md` | Move |
| `docs/EXAMPLE_ROUNDTRIP.md` | `docs/reference/EXAMPLE_ROUNDTRIP.md` | Move |
| `docs/NORMALISATION_AUDIT.md` | `docs/reference/NORMALISATION_AUDIT.md` | Move |
| `docs/QWEN_PRIOR_ANALYSIS.md` | `docs/reference/QWEN_PRIOR_ANALYSIS.md` | Move |
| `docs/DECOMPILER_HANDOVER.md` | `docs/reference/DECOMPILER_HANDOVER.md` | Move |
| `CORPUS_SCHEMA.md` (root) | `docs/reference/CORPUS_SCHEMA.md` | Move from root |

### docs/history/
| Old path | New path | Action |
|---|---|---|
| `docs/PHASE_5_RESULTS.md` | `docs/history/PHASE_5_RESULTS.md` | Move |
| `docs/PHASE_6_RESULTS.md` | `docs/history/PHASE_6_RESULTS.md` | Move |
| `docs/PHASE_7_ROADMAP.md` | `docs/history/PHASE_7_ROADMAP.md` | Move |
| `docs/PHASE_7_DESIGN.md` | `docs/history/PHASE_7_DESIGN.md` | Move |
| `docs/PHASE_8_VALIDATION_PLAN.md` | `docs/history/PHASE_8_VALIDATION_PLAN.md` | Move |

### docs/scratch/
| Old path | New path | Action |
|---|---|---|
| `docs/runs_audit.md` | `docs/scratch/runs_audit.md` | Move |
| `docs/variant_c_eval_bug.md` | `docs/scratch/variant_c_eval_bug.md` | Move |
| `docs/v2_token_analysis.md` | `docs/scratch/v2_token_analysis.md` | Move |
| `docs/resume_notes_v2.md` | `docs/scratch/resume_notes_v2.md` | Move |
| `docs/REPO_AUDIT.md` | `docs/scratch/REPO_AUDIT.md` | Move |
| `docs/AUDIT_2026_08_04.md` | `docs/scratch/AUDIT_2026_08_04.md` | Move |

### Delete (stale or redundant)
| Old path | Reason |
|---|---|
| `docs/FUTURE_WORK.md` | Superseded by PHASE_7_ROADMAP.md (self-declared) |
| `docs/CHANGELOG.md` | Git log supersedes this |
| `docs/TEST.md` | Stub, no content |
| `docs/TESTING_SUMMARY.md` | Covered by PHASE_7_ROADMAP and test scripts |
| `docs/Kit_Proposal.md` | Historical, no active use |
| `docs/Design.md` | Check contents first — may be worth keeping in reference/ |
| `docs/REORGANIZATION_MAP.md` | This file — delete after reorganization complete |

---

## Root → new location mapping

| Old path | New path | Action |
|---|---|---|
| `CORPUS_SCHEMA.md` | `docs/reference/CORPUS_SCHEMA.md` | Move |
| `check_phase5_status.sh` | `Scripts/check_phase5_status.sh` | Move |
| `run_abc.sh` | `Scripts/run_abc.sh` | Move |
| `elan-init.sh` | `Scripts/elan-init.sh` | Move |
| `eval_a_v2_final.log` | `logs/eval_a_v2_final.log` | Move |
| `eval_c_correct.log` | `logs/eval_c_correct.log` | Move |
| `run_A_v2_full.log` | `logs/run_A_v2_full.log` | Move |
| `run_A_v2_full.pid` | — | Delete |
| `test_write.tmp` | — | Delete |
| `nohup.out` | — | Delete |
| `Modelfile.tmp` | — | Delete |
| `datasets_flat/` | `datasets/archive/flat/` | Move or delete |
| `datasets_phase5_backup/` | `datasets/archive/phase5_backup/` | Move or delete |
| `datasets_phase5_v1/` | `datasets/archive/phase5_v1/` | Move or delete |
| `doc/` | Merge into `docs/` or delete | Check contents first |
| `ir-research/` | `docs/scratch/ir-research/` | Move or delete |
| `local-models/` | Add to `.gitignore` if not tracked | Keep in place |

---

## .gitignore additions

Add these lines to `.gitignore` after reorganization:

```
logs/*.log
logs/*.pid
*.pid
*.tmp
nohup.out
local-models/
```

---

## Cross-reference sweep (run after all moves)

```bash
# Find all docs still referencing old paths
grep -r "docs/DECISION_INDEX\|docs/DECISION_LOG\|docs/EXPERIMENT_DESIGN\|docs/PHASE_7_ROADMAP\|docs/ENCODER_FORMAT\|docs/PYTHON_PIPELINE\|docs/IR_SCHEMA_AUDIT\|docs/REPRESENTATION_EVOLUTION" docs/ --include="*.md" -l

# Find all docs referencing root-level files that moved
grep -r "CORPUS_SCHEMA\|runs_audit\|variant_c_eval_bug\|v2_token_analysis" docs/ --include="*.md" -l
```

For each file returned, update the path using the mapping table above.
