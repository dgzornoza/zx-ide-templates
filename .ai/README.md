# .ai/ — Cross-Agent Guidance

> Maintainer-facing layout and conventions for the agent guidance surface.
> AI coding agents: prefer the root-level `AGENTS.md` as your entry point.
> This file documents the structure for humans and for agents that want depth.

## Layout

```text
.ai/
├── README.md           # this file (maintainer conventions)
├── skills/             # workflow templates (SKILL.md per skill)
│   └── <skill-name>/
│       ├── SKILL.md    # required
│       └── references/ # optional, for long-form reference material
└── rules/              # mandatory contracts (one .md file per rule)
    └── <rule-name>.md
```

## Skills

Skills live under `.ai/skills/<skill-name>/`. Each skill is a folder containing at minimum a `SKILL.md`. Skills are **workflows** — they describe how to do a thing.

### SKILL.md frontmatter (Anthropic-style, cross-agent)

```markdown
---
name: <kebab-case-name>
description: >-
  <one-paragraph description; mention when to load this skill>
user-invocable: true            # optional
argument-hint: "<hint text>"    # optional
---
```

- `name` MUST be kebab-case and match the folder name.
- `description` MUST be self-contained and SHOULD mention trigger conditions.
- `user-invocable` and `argument-hint` are optional and only meaningful for agents that support user-invoked skills.

### Adding a new skill

1. Create `.ai/skills/<skill-name>/`.
2. Add `SKILL.md` with the frontmatter above.
3. If the skill needs long-form reference material, add it under `.ai/skills/<skill-name>/references/`.
4. Add the skill to the index in the root `AGENTS.md` (skill index table).
5. Refresh `.atl/skill-registry.md` (`/skill-registry:refresh` if available).

## Rules

Rules live under `.ai/rules/<rule-name>.md`. Each rule is a single `.md` file. Rules are **mandatory contracts** — they describe constraints that MUST be honored.

### Rule frontmatter (cross-agent, extended)

```markdown
---
name: <kebab-case-name>
description: <one-line summary>
globs:                # optional; honored by Cursor/Codex, ignored elsewhere
  - "**/*.c"
  - "**/*.h"
---
```

- `name` MUST be kebab-case and match the filename without extension.
- `description` MUST summarize when the rule applies.
- `globs` is OPTIONAL. Include it when the rule only matters for specific file patterns. Consumers that do not honor `globs:` will ignore the field harmlessly.

### Adding a new rule

1. Create `.ai/rules/<rule-name>.md` with the frontmatter above.
2. Write the rule body as a list of constraints (use RFC 2119 keywords: `MUST`, `SHALL`, `SHOULD`, `MAY`).
3. Add the rule to the index in the root `AGENTS.md` (rule index table).
4. Refresh `.atl/skill-registry.md` if you want it indexed for delegators.

## Skill registry

`.atl/skill-registry.md` is the delegator index consumed by subagents in the gentle-ai / pi-subagents harness. It is **auto-generated** by scanning `.ai/skills/` (and user-level skill roots). After adding, moving, or removing a skill, run:

```bash
/skill-registry:refresh
```

If the command is unavailable, manually edit the `Sources scanned` line and the skill table to match.

## Precedence: rules vs skills

- **Rules** (`.ai/rules/`) are mandatory contracts. They MUST be honored.
- **Skills** (`.ai/skills/`) are workflow templates. They describe how to do work.
- If a skill output would violate a rule, the rule wins. Surface the conflict to the user.

## Migration history

This directory was introduced as part of the `agent-agnostic-guidance` SDD change. The previous GitHub-Copilot-shaped guidance under `.github/` has been retired. The OpenSpec record of the migration lives under `openspec/changes/agent-agnostic-guidance/`.