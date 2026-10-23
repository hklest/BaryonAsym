---
domain: science
summary: V0 SIDIS: no lab-frame cuts; unfold to full gen-level invariant phase space with IBU; conventions inherited from sidis_highQ2
affects: [lambda/docs/sidis_v0/, lambda/scripts/sidis_v0/, sidis_highQ2/]
verified: 2026-10-23
source: user 2026-10-23
supersedes:
---
User, verbatim: "we also need to relax our cuts. For our SIDIS data to be interpretable, we can't have any lab-frame cuts. We need to unfold to the full gen-level phase space, only cuts on xF, z, pth, Q2, x, etc. are possible at gen-level. We can only put reco cuts that leave our gen-level acceptance mostly fine." And: "Look at the high Q2 SIDIS analysis to understand how we should define everything."
Consequences: gen-level cuts only on invariant variables; reco cuts must leave gen acceptance mostly intact; IBU unfolding is mandatory; definitions inherited from sidis_highQ2.
