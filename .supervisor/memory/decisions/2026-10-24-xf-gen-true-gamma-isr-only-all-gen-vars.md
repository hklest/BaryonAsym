---
domain: science
summary: eSigma generator level uses one true gamma* (E_eff subtracts ISR photons only) for all gen kinematics: x_F, Q2, y, x, z, pT, fiducial cuts, Q2 cell
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md]
verified: 2026-10-24
source: user 2026-10-24
supersedes: .supervisor/archive/memory/decisions/2026-10-24-xf-gen-true-gamma-isr-only-all-vars.md
---
User extension, verbatim: "It should be for z and pt as well I believe."
Earlier user decision (U2), verbatim: "Yes, use the true photon for all of them. Also, if we subtract every photon from the beam energy it is indeed wrong. Apply the correction. I agree, b with correction."
The true gamma* uses E_eff = beam electron energy minus initial-state radiation photons ONLY, not final-state ones. One true gamma* defines all generator-level kinematics: x_F, Q2, y, x, z, pT, the fiducial cuts and the Q2 cell. This replaces "gen z and pT keep their current photon".
The generator-level phi_h appendix decision (memory/decisions/2026-10-24-gen-phih-appendix-keeps-truth-photon.md) was stated separately and is left unchanged; the user has not said whether it follows this extension.
Also supersedes the earlier E_eff = BeamElecE - max(GenEnPhoton,0) formula (every photon subtracted, judged wrong) and the 2026-10-19 "Option A" cell choice.
