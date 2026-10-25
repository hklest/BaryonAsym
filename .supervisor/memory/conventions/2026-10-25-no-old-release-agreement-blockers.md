---
domain: science
summary: final eSigma reruns - agreement with the old (flawed) release is a diagnostic, never a blocker; ask the user only if byte-identical a priori
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md, sidis_highQ2/]
verified: 2026-10-25
source: user 2026-10-25 (plan sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md)
supersedes:
---
User: "RU-2, yes, we don't expect that to be the same. So you can proceed with what you have but with cross checks. Actually, all the questions you're asking me here are mostly related to deviations from the previous (incorrect) analysis, but since that analysis had severe flaws, we don't expect them to be the same. Remove any blockers related to the two being the same, unless they should be byte identical a priori."

Rule for final-campaign reruns:
- Gates requiring agreement with the previous (flawed) release are diagnostics, not blockers: the born_shift row-integral limit, the RU-2 fake-fraction window, the generator phi_h G1/G4 old-anchored bands.
- Hard blockers remain only for checks identical by construction (null tests, same-MC file lists, unchanged-input identities, md5 pins) and internal-consistency gates.
- Independent cross-checks are still required before install: an MC-expert estimate of the z migration and of the RU-2 drop.
- Do not ask the user about deviations from the old analysis unless the quantity should be byte-identical a priori.
