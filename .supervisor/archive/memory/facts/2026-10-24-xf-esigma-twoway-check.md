---
domain: science
summary: xF hemisphere cut uses electron-only photon at reco and gen; eSigma xF would change ~0 tracks (two-way check); keep-vs-rerun decision pending
affects: [H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/]
verified: 2026-10-24
source: Condor 5868500, user report 2026-10-24
supersedes:
---
The xF hemisphere cut uses the electron-only photon at reco and gen. Two-way check (Condor 5868500): switching to eSigma changes 0/3.88M data, 0/76.7M DJ, 4/64.3M RG (NaN, 6e-8) selected tracks. Gen flips ~0.25%, >99% of them at z_gen<0.05.
Record: H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/.
Open: user decision pending, keep the release and fix the note text, or rerun the chain with eSigma xF.
superseded by: memory/facts/2026-10-24-xf-esigma-twoway-check-2.md
