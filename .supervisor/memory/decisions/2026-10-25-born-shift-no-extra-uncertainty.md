---
domain: science
summary: Final eSigma: no extra uncertainty and no ibu_model enlargement for the Born shift vs previous release; 3 h+ bins exceed total uncertainty, 195 exceed ibu_model alone, accepted
affects: [sidis_highQ2/docs/plans/esigma_xf_bg_revision_2026-10-24.md, H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/]
verified: 2026-10-25
source: user 2026-10-25
supersedes: archive/memory/decisions/2026-10-25-high-z-born-shift-accepted-model-dependence.md
---
User decision refining the earlier acceptance of the high-z Born shift (+0.6 to +2.6% at z >= 0.2, unfolding model dependence): add no extra uncertainty term and do not enlarge ibu_model for the Born shift against the previous release. Per-bin coverage: 3 h+ bins (z rows 2, 3 and 4, last bin of each row) exceed the total quoted uncertainty; in 195 bins the shift exceeds ibu_model alone. Both are accepted because the observable definition changed and the old release was wrong; the earlier "covered by ibu_model and total uncertainty" claim is withdrawn as a gate or expectation. Coverage stays a diagnostic only.
User verbatim: "This is still a shift vs. previous. I expect we may have some that are greater than our uncertainty band, since we're changing the definitions of the observables in a sense. The new version should be correct. The old version was incorrect, and the uncertainties weren't capturing it. That's fine."
Evidence: H1ChargedSidis_HighQ2/output_v6/studies/esigma_final_2026-10-24/s4/born_shift_vs_ibu_model.csv; plot sidis_highQ2/plots/diagnostics/born_shift_coverage_2026-10-25/born_shift_coverage.png (both verified to exist).
Related: .supervisor/memory/conventions/2026-10-25-no-old-release-agreement-blockers.md (consistent).
