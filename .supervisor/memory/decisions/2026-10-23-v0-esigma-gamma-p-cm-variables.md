---
domain: science
summary: V0 SIDIS: eSigma cuts/binning; gamma*p-CM/Breit variables (pT vs boosted q axis, xF, z=P.Ph/P.q); dsigma/deta and lab pT dropped
affects: [lambda/docs/sidis_v0/, lambda/scripts/sidis_v0/]
verified: 2026-10-23
source: supervisor 2026-10-23
supersedes: .supervisor/archive/memory/decisions/2026-10-23-v0-esigma-conventions.md
---
eSigma used consistently for cuts, binning and z. The measurement uses gamma*p-CM/Breit-frame variables per sidis_highQ2 conventions: p_T^h relative to the boosted q axis, x_F, z = P.P_h/P.q. d(sigma)/d(eta) and lab-frame p_T are dropped. Lambda+anti-Lambda summed, anti-Lambda kept as cross-check. MC generator files from v3p2 only; never mix with gen_v2. Replaces the earlier "no Breit-frame observables" clause.
