---
name: naf-h1-sidis-access
description: "Where the H1 high-Q2 charged SIDIS analysis lives on NAF and how to navigate it (STATE.md, opencode skills)"
metadata:
  node_type: memory
  type: reference
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-20T17:28:59.941Z
---

`ssh klesthen@naf-hone.desy.de` works non-interactively (BatchMode). Paths:
- Docs/state: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2`
- Products: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6`

Navigation: `STATE.md` is huge, so grep its `##` override sections. Topical facts live in `.opencode/skills/sidis-hq2-*/SKILL.md`. On the login node, run one-file checks only (≤5 min); everything else goes through Condor. The local `CLAUDE.md` in the H1_SIDIS folder summarizes this. Related: [[h1-sidis-detint-uncertainty-gap]].
