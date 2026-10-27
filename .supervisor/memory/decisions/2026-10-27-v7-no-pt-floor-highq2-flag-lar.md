---
domain: science
summary: v7 tree keeps any daughter pT (no floor), SpaCal OR LAr electron with highQ2 flag; high-Q2 V0 SIDIS in scope
affects: [lambda/docs/sidis_v0/tree_spec_v7_2026-10-27.md, sidis_highQ2/]
verified: 2026-10-27
source: user 2026-10-27
supersedes:
---
User, 2026-10-27, verbatim: "I would say the pT should be looser than 0.1. Keep any pT at the loose ttree production. We have plenty of space. I would also say that you should include high Q2. In the TTree have a 'highQ2' flag that uses the LAr info (copy as much as possible about the scattered electron our high Q2 SIDIS analysis). I realize for SIDIS, the high Q2 phase space is nicer in terms of the struck quark going into the CJC region, so let's include that in our V0 SIDIS analysis."
Consequence: no daughter-pT floor; electron from SpaCal OR LAr with a highQ2 flag, copying the high-Q2 SIDIS electron info; high-Q2 V0 SIDIS is in scope.
