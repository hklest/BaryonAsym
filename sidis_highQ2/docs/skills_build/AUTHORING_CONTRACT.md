# SKILL.md authoring contract — high-Q2 charged SIDIS NAF skills

## Where you write
Local mirror only: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/.opencode/skills/<skill-name>/SKILL.md`
Optional overflow: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/.opencode/skills/<skill-name>/references/<topic>.md`

Never use `ssh`. Never touch `/data/dust/`, `/afs/`, `/pnfs/`, `/cvmfs/` with local tools —
those strings appear ONLY as text inside the files you write.

## Where you read
Your manifest lists every source file you own:
`/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/skills_build/manifests/<skill-name>.txt`
Paths in the manifest are relative to `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/`.
Also read `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/STATE.md` (do not contradict it).

## Path translation — critical
You READ from `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/<rel>`.
You WRITE the NAF path `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/<rel>`.
The local Mac path must NEVER appear inside a SKILL.md. Same relative structure, different root.

## Required file shape
```
---
name: <skill-name>
description: <one sentence, <=25 words. Must say the topic AND that it is high-Q2
             charged-hadron SIDIS, AND when to load it. Front-load trigger keywords.>
---

## Authoritative facts
## Procedures
## Traps
## Canonical references
## Ownership boundary
```

## Section rules
- **Authoritative facts** — numbers, paths, conventions that must never be re-derived. Every
  number traceable to a source file you read. Never invent. If sources conflict, state both
  values, both file paths, and which wins.
- **Procedures** — ordered runnable steps, absolute NAF paths, real script names from the sources.
- **Traps** — each trap states the WRONG value/action AND the RIGHT one. Superseded values must be
  explicitly named and negated, never silently dropped.
- **Canonical references** — a table `NAF absolute path | what it contains | read it when`.
  EVERY file in your manifest must appear in this table exactly once. Group tightly-related files
  onto one row only if you list every path in that row's path cell.
- **Ownership boundary** — what belongs to a sibling skill or a different analysis.

## Hard limits
- SKILL.md body <= 400 lines. If you exceed it, move detail into `references/<topic>.md` inside
  your own skill directory and point at it from the body. Do not drop content.
- Terse. Tables and one-line bullets. No prose paragraphs, no preamble, no restating the question.
- Never contradict `STATE.md`.

## The 14 skills (for writing your Ownership boundary section)
sidis-hq2-state, sidis-hq2-docmap, sidis-hq2-normalization, sidis-hq2-unfolding,
sidis-hq2-systematics, sidis-hq2-radiative, sidis-hq2-backgrounds, sidis-hq2-builder,
sidis-hq2-operations, sidis-hq2-mc, sidis-hq2-aziasym, sidis-hq2-plotting, sidis-hq2-lessons,
sidis-hq2-results

## Environment facts you may cite
- opencode binary: `/data/dust/group/h1/klesthen/BaryonAsym/opencode/install/opencode` (not on PATH)
- H1OO: `source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh`, `make bin`
- Login node: single-file interactive checks only, 5 min wall cap; everything else via Condor
- Sessions run ON NAF — there is no SSH step, execution is local

## Final summary contract
Return ONLY: files written with line counts; count of manifest files covered vs total; any
conflicts found (with both values and both paths); any manifest file you could not place and why.
No command logs, no pasted content, no narration.
