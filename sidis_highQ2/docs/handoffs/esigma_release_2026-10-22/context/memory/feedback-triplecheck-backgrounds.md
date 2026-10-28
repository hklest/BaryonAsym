---
name: feedback-triplecheck-backgrounds
description: "Before claiming a background/uncertainty size in the H1 SIDIS analysis, validate the background class with independent variables"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-20T23:16:30.439Z
---

Validate any truth-level background class with variables not used to define it before reporting its size. Examples:
- DCA′ templates, matched in pT and track length;
- physical constraints (a secondary can't exceed its parent's momentum);
- curvature pulls.

Report the misclassification fraction.

**Why:** on 2026-10-21 the user caught that my v1 "absorbed/Tier-2" background contained real primary tracks, after I had already reported a ±2.8% uncertainty from it. I had to withdraw it.

**How to apply:** apply this to every new fake/background category before propagating it to the unfolding or budget, and state the purity evidence alongside the number. Related: [[h1-sidis-trackclass-impact]].
