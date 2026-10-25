---
domain: science
summary: Float-edge pT_h bin mismatches (within ~1e-7 GeV of an edge, bitwise-equal operands) are benign; benign-mismatch certification rule extended to DJANGOH files
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md, sidis_highQ2/]
verified: 2026-10-25
source: user 2026-10-25 (plan sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md)
supersedes:
---
User: "We don't care about such small differences" (confirmed the quantity is pT_h). Rows whose pT_h lies within about 1e-7 GeV of a bin edge with bitwise-equal operands are benign. The benign-mismatch certification rule now covers DJANGOH files as well; the per-row checks are unchanged.
