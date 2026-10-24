---
domain: science
summary: eSigma generator level uses one true gamma* (E_eff subtracts ISR photons only) for ALL gen kinematics incl. gen phi_h RAD/NONRAD appendix pages (90-shard rerun); reco stays eSigma
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md]
verified: 2026-10-24
source: user 2026-10-24
supersedes: .supervisor/archive/memory/decisions/2026-10-24-xf-gen-true-gamma-isr-only-all-gen-vars.md, .supervisor/archive/memory/decisions/2026-10-24-gen-phih-appendix-keeps-truth-photon.md
---
User, verbatim: "Yes, all should be the true gamma*, unless you see a reason for it not to be."
One true gamma* defines every generator-level kinematic: x_F, Q2, y, x, z, pT, phi_h, the fiducial cuts and the Q2 cell. E_eff = beam electron energy minus initial-state radiation photons ONLY (final-state photons not subtracted; subtracting every photon was judged wrong and is corrected).
This includes the generator-level phi_h RAD/NONRAD appendix pages, which no longer keep q = k_beam - k'_truth; they get a 90-shard rerun. The one-sentence photon statement on those pages must now describe the ISR-only true gamma*. Reconstructed-level quantities stay eSigma.
Prior user statements kept for history: "It should be for z and pt as well I believe."; "Yes, use the true photon for all of them ... I agree, b with correction."; phi_h appendix "YEs, keep them as they are." (now reversed).
Supersedes the earlier xf/all-gen-vars entry and the phi_h appendix keeps-truth-photon entry, and transitively the E_eff = BeamElecE - max(GenEnPhoton,0) formula and the 2026-10-19 "Option A" cell choice. Plan: sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md.
