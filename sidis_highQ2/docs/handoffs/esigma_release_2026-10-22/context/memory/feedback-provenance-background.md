---
name: feedback-provenance-background
description: "In the H1 SIDIS analysis, background/fake classes must be defined by truth-link provenance only, never by reco kinematics (resolution tails are unfolded)"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-21T01:47:36.132Z
---

Define reco-track background by truth-link provenance only. In practice that means STR-only (GEANT-created) links: det.int. and the K± keeper. Any track with a GTR primary π/K/p link is signal and is unfolded, however badly measured. That includes:
- P/GenP tails;
- large angles;
- charge flips;
- duplicates;
- Tier-2 matches.

**Why:** user ruling 2026-10-21. Using a P/GenP cut "mixes conventions" and turns the reco momentum resolution tail into background, when the response should unfold it.

**How to apply:** don't propose kinematic-consistency cuts or dedupe as background definitions. If a kinematic tail looks suspicious, treat it as a migration and modelling question for the response, not as background. Related: [[feedback-triplecheck-backgrounds]], [[h1-sidis-trackclass-impact]].
