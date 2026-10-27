---
domain: science
summary: The detint +-100% normalization leg and its rank-1 covariance are removed from the eSigma budget as a double count of the hyperon and nonprimary legs
affects: [H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-27b/]
verified: 2026-10-27
source: .supervisor/archive/working/plans/2026-10-27-sidis-highq2-finalization.md (user "Fix it", 2026-10-27)
supersedes: .supervisor/archive/memory/decisions/2026-10-23-esigma-detint-rank1-correlated.md
---
The detint norm leg and cov_detint_norm_rank1 are absent from the budget and covariance (26 of 27 terms remain). The hyperon and material shares were counted twice, in the detint norm leg and in the env100 hyperon and nonprimary legs.
Why: the user chose removal ("Fix it") after the ratio was quantified: totals only decrease, median change below 0.003 pp.
Rejected: keeping it as conservative.
