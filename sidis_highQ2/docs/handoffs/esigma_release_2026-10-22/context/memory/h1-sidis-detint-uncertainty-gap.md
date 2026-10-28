---
name: h1-sidis-detint-uncertainty-gap
description: 2026-10-20 audit finding — H1 high-Q2 SIDIS det.int. removal has no rate/normalization uncertainty; awaiting user choice of κ
metadata:
  node_type: memory
  type: project
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-20T17:28:57.080Z
---

Audit 2026-10-20 (local doc `docs/detint_uncertainty_audit_2026-10-20.md` in the H1_SIDIS folder): det.int. is removed response-internally via S2' `f_i` (never externally subtracted), but no budget term varies its rate. The ±100% `bgsub_env100_rel` (added 2026-10-19) covers e+V0 only. `detint_removal_rel` (~0.03% median) is DJ–RG spread, which shares GEANT. `bgsub_fakefrac_rel` is fc0-only. The unpriced content is ~0.7% integrated, ~0.3% per-bin median, ≤2.6%.

**Why:** the user believed e/V0 had 100% uncertainty and asked whether det.int. was blindly subtracted; the answer was effectively yes, in the uncertainty sense.

**How to apply:** next steps are a user decision on κ, then an fc3-scaled `f_i` variant through IBU k=4 during the OPT-A budget regeneration. Also verify the NucliaWeight convention mismatch. See [[naf-h1-sidis-access]].
