# Blockers

Open and closed blocker files live here, per `docs/MULTI_AGENT_WORKFLOW.md`.
Naming: `open_YYYYMMDD-HHMMSS_<short-slug>.md` -> `closed_YYYYMMDD-HHMMSS_<short-slug>.md`

An agent that cannot start or finish a task writes one of these rather than guessing,
labels the issue `status:blocked-needs-input`, and moves on to other work.
