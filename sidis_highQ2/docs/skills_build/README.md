# skills_build — provenance for the NAF skills port

Generated 2026-09-08. This directory documents how the 14 `sidis-hq2-*` skills
in `.opencode/skills/` were derived. It is not loaded at session start.

| File | Contents |
|------|----------|
| `COVERAGE_MANIFEST.md` | Every markdown artifact in the workspace and its owning skill. 424 entries. |
| `ownership.tsv` | Machine-readable `path -> owning_skill`. Regenerate checks against this. |
| `manifests/<skill>.txt` | Per-skill file list handed to each authoring pass. |
| `AUTHORING_CONTRACT.md` | The contract every SKILL.md was written against. |
| `OPEN_CONFLICTS_2026-09-08.md` | Source-document contradictions found during the port. |

## Invariant

Source documents were never modified, moved, or deleted. Skills are digests plus
reference indexes; the originals under `docs/`, `MC/`, `AziAsym/`, `data/`, and
`data_release/` remain authoritative. A wrong digest costs a slower lookup,
never a lost fact.

## When adding a new document

Add a row to `ownership.tsv` and a line to the owning skill's Canonical
references table. A document with no owner is invisible to the skill system.
