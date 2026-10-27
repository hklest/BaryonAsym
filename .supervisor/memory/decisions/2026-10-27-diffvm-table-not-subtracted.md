---
domain: science
summary: DIFFVM rho0 is neither subtracted nor given an uncertainty by default; a per-bin table with sign column and recommended 1-sigma is published as supplementary material
affects: [sidis_highQ2/docs/backgrounds/diffvm_perbin_table_esigmaphotonxf_2026-10-27.md, sidis_highQ2/data/backgrounds/diffvm_perbin_esigmaphotonxf_2026-10-27/, sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/backgrounds.tex]
verified: 2026-10-27
source: .supervisor/archive/working/plans/2026-10-27-sidis-highq2-finalization.md (user 2026-10-27)
supersedes: .supervisor/archive/memory/decisions/2026-10-26-diffvm-not-subtracted-negligible-rho0.md
---
User, verbatim: "I think that we will just mention these numbers in a table. Some theory frameworks may include diffractive rho, meaning we shouldn't just subtract it or even put an uncertainty on it by default." The table lists every bin with any diffractive-rho contribution, the track fraction, a sign column and the recommended 1-sigma uncertainty if theorists subtract it. It is published as supplementary material cited by the paper. The quoted budget has no DIFFVM row (the old 100% row is removed, not replaced).
Why: theory frameworks may already include diffractive rho, so a default subtraction or uncertainty could double count.
Rejected: a default data-limited uncertainty term (the former supervisor default).
