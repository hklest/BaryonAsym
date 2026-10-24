---
domain: science
summary: eSigma generator level uses the true gamma* (E_eff subtracts ISR photons only) for x_F, Q2, y, x, fiducial cuts and Q2 cell; gen z and pT keep current photon
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md]
verified: 2026-10-24
source: user 2026-10-24
supersedes: .supervisor/archive/memory/decisions/2026-10-24-xf-gen-true-gamma-star-option-a.md
---
User decision (U2), verbatim: "Yes, use the true photon for all of them. Also, if we subtract every photon from the beam energy it is indeed wrong. Apply the correction. I agree, b with correction."
The true gamma* uses E_eff = beam electron energy minus initial-state radiation photons ONLY, not final-state ones. It defines generator-level x_F, Q2, y, x, the fiducial cuts and the Q2 cell. Generator z and pT keep their current photon.
Supersedes the earlier formula (E_eff = BeamElecE - max(GenEnPhoton,0), every photon subtracted, which the user judged wrong) and reverses the 2026-10-19 "Option A" cell choice (not stored as an entry).
