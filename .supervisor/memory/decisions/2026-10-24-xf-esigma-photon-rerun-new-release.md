---
domain: science
summary: x_F<0 cut moves to the eSigma photon (reco and gen) with chain rerun and new release; reverses the keep-release decision
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md, H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/]
verified: 2026-10-24
source: user 2026-10-24
supersedes: .supervisor/archive/memory/decisions/2026-10-24-xf-photon-keep-release-omit-from-note.md
---
User decision, verbatim, replying to whether to recompute the inherited background uncertainty terms in eSigma: "Yes, the background uncertainties should be recalculated for eSigma photon. While you do that, try to fix the xF also while we're at it."
Reverses the earlier keep-release choice. The x_F < 0 hemisphere cut moves to the eSigma photon at reconstructed and generator level, with a chain rerun and a new release. The two-way check (Condor 5868500) found the effect negligible (0/3.88M data, 0/76.7M DJ, 4/64.3M RG tracks), but the user wants it consistent. The note may now need to state the x_F photon choice (the old omit-from-note instruction no longer holds). Plan: sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md.
