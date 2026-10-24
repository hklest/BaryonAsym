---
domain: science
summary: all background-related uncertainty terms (detector-interaction removal, background normalization, any other inherited) are recomputed in the eSigma frame
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md]
verified: 2026-10-24
source: user 2026-10-24
supersedes:
---
Decided with the x_F rerun (verbatim in .supervisor/memory/decisions/2026-10-24-xf-esigma-photon-rerun-new-release.md). Every background-related term, namely detector-interaction removal, background normalization and any other inherited term, is recomputed in the eSigma frame rather than inherited from the electron-photon release. This may change the installed Detint term (see .supervisor/memory/decisions/2026-10-23-esigma-detint-rank1-correlated.md and 2026-10-23-esigma-scope-exclusions-and-kappa.md), which stay in force until the recomputed values land.
