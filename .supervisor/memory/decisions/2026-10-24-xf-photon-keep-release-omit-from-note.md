---
domain: science
summary: xF<0 hemisphere cut keeps electron-only photon; keep installed release, no eSigma rerun, omit x_F photon choice from note and paper
affects: [H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/]
verified: 2026-10-24
source: user 2026-10-24
supersedes:
---
User decision, verbatim: "1. Seems this is fine. I would prefer we just don't mention anything about it in the note, since it seems to not matter at all."
Options were keep the release or rerun the chain with eSigma x_F. Chosen: keep the installed release, no rerun. Why: the two-way check (Condor 5868500) shows eSigma x_F changes 0/3.88M data, 0/76.7M DJ and 4/64.3M RG selected tracks, so the effect is negligible.
Consequence: the note and the paper do not mention the x_F photon choice at all (no text fix, no systematic). Evidence: H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/summary.json.
