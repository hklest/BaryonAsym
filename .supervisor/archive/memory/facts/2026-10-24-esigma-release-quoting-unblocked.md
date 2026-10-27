---
domain: science
summary: eSigma release installed 2026-10-23, audit PASS; M1, N2, P-CRAD-LUMI all closed 2026-10-24, no quoting blockers remain
affects: [sidis_highQ2/docs/promotions/esigmaphoton_promotion_2026-10-23.md, sidis_highQ2/plots/systematics/esigmaphoton_2026-10-23/budget_7col/, sidis_highQ2/docs/blockers/]
verified: 2026-10-24
source: user 2026-10-24 (evidence sidis_highQ2/docs/blockers/M1_closure_rescope_2026-10-24.md, N2_closure_2026-10-24.md, P-CRAD-LUMI_closure_2026-10-24.md)
supersedes: .supervisor/archive/memory/facts/2026-10-23-esigma-release-installed.md
---
Installed 2026-10-23 (s15 Condor 5844143, 106/106 bitwise). Promoted to 21 canonical files (s16 plus 6 s17 additive fixes); backups in sidis_highQ2/docs/promotions/esigmaphoton_2026-10-23_preedit/. Audit PASS.
- INSTALL_RECORD: /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphoton_2026-10-23.json
- Promotion record: sidis_highQ2/docs/promotions/esigmaphoton_promotion_2026-10-23.md
- Note updated and built (commit d03fa6e); budget plots at sidis_highQ2/plots/systematics/esigmaphoton_2026-10-23/budget_7col/.
Quoting blockers, all closed 2026-10-24 (replaces the 10-23 "still BLOCKED" line):
- M1 env100 builder: re-scoped; see memory/decisions/2026-10-24-m1-n2-closed-d2-eprime-cut-shifted-legs.md.
- N2 HFS-Empz: closed, D2 dispositioned (E' > 11 GeV applies to shifted legs); effect size never measured; same decision entry.
- P-CRAD-LUMI: accepted with gaps; see memory/decisions/2026-10-24-p-crad-lumi-accepted-gaps.md. Revisit if MC DB access returns.
Open:
- Phase G (re-pointing plots and the analysis note) is in progress.
- Remaining eSigma plots need new plotters: covariance, Born 7-panel, h+/h- ratio, A_h, swap-one, control plots.
- 21 canonical docs and 3 note figures were untracked in git pending user OK (check current state before relying on this).
superseded by: sidis_highQ2/docs/promotions/esigmaphoton_promotion_2026-10-23.md
