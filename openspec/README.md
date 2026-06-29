# OpenSpec directory

This directory holds the project's SDD/OpenSpec artifacts. It is the
authoritative source of truth for **why**, **what**, and **how** every
material change is planned, built, verified, and archived.

## Layout

```text
openspec/
├── README.md            # this file
├── config.yaml          # SDD configuration (store, phases, conventions)
├── specs/               # canonical (post-sync) delta specs
└── changes/             # active changes, one folder per change
    └── <change-name>/
        ├── proposal.md          # why, scope, non-goals
        ├── design.md            # technical approach
        ├── tasks.md             # ordered implementation tasks
        ├── specs/               # delta specs (deltas only)
        │   └── <area>/spec.md
        ├── apply-progress.md    # written by sdd-apply
        ├── verify-report.md     # written by sdd-verify
        └── sync-report.md       # written by sdd-sync
```

## Lifecycle

```text
explore → proposal → spec → design → tasks → apply → verify → sync → archive
```

Phases and conventions are defined in `config.yaml`. Read it before
starting any change.

## How to start a change

1. Pick a short, kebab-case `change-name` (see `naming.change_pattern`).
2. Create `openspec/changes/<change-name>/`.
3. Run `sdd-proposal` with the change name.
4. Proceed through `sdd-spec`, `sdd-design`, `sdd-tasks`.
5. Run `sdd-apply`, then `sdd-verify`.
6. Optionally `sdd-sync` and `sdd-archive`.

## Rules of thumb

- One change = one cohesive intent. If a change touches unrelated areas,
  split it.
- Specs are **deltas** against canonical specs in `openspec/specs/`,
  not full rewrites. Only the changed sections appear under
  `changes/<change>/specs/`.
- Tasks must include review-workload estimates. If total changed lines
  exceed `review_workload.budget_lines` (400 by default), split into
  chained PRs or get explicit user approval for a single PR.
- `strict_tdd` is **false** in this project. Do not invent TDD evidence.
  Validation is the Makefile build plus manual review.

## Relationship with `.ai/` and `.atl/`

- `.ai/AGENTS.md` and `.ai/skills/` are **cross-agent guidance** for any
  AI coding agent that consumes this repo.
- `.atl/skill-registry.md` is the **delegator index** for agents that
  spawn subagents. Refresh it after moving or adding SKILL.md files.
- `openspec/` is **not** an agent guidance surface. It is human-facing
  documentation of how changes are planned, executed, and verified.
