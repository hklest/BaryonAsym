# Hostile implementation/reproduction review: `nucliaweight_primaryloss_rel` (2026-10-21 candidate)

**Verdict: PASS-WITH-CONDITIONS.** The critic wrote their own code for every check and used the official engine plus
their own IBU implementation; they did not re-run `make_nucliaweight_term.py`. Every headline number reproduces to
≤ 2e-15. There is no BLOCKER; all conditions are about documentation or scope.

## Claim table

| # | Claim | Result | Evidence |
|---|---|---|---|
| 1 | Builder diff is only the κ scaling plus a meta key | REPRODUCED | `diff` against the source (md5 c9af59df… confirmed) shows 3 hunks (l.635, l.1424–1425, l.2582). wn enters only through `w_reco` (l.1426). `_nw_partner` (l.2095/2158) feeds only the audit-only `prior_gen_NWeff`. `h_prior_gen`, `h_mc_count` and pool rows are identical on all 174 shards. The tree NW is already species-refined: \|PDG\|=2212 has NW ≡ 1. |
| 2 | 174 shards, κ=1, same frozen lists, clean exits | REPRODUCED | 73 RG + 101 DJ. Meta κ = {'1.0'} everywhere. The shard-list mapping is identical to the nominal `cone_s2prime_optA_2026-10-19/condor/*_rebuild.args`. All non-sumw meta keys are identical. 174/174 rv 0, .err files empty. Term job 5815199 rv 0. |
| 3 | κ=0 null, linearity, D only in body + fake column | REPRODUCED | smoke k0 against official: 68/68 histograms bit-identical. Linearity is 8.7e-15 over all non-w2 histograms. The miss row and corner cell are unchanged in 348/348 shard-charges. D ≥ 0 on the summed responses. |
| 4 | Integrated shifts and per-bin values | REPRODUCED | h+ +1.1792% / −1.1473%; h− +1.1834% / −1.1514%. Native/l2m 0.611, fake-leg +0.1483%, coherence +0.4530%. The nominal matches the official release to 1.3e-15. The per-bin term matches the CSV to ≤ 1.8e-15 for both charges and generators. |
| 5 | Candidate CSV | REPRODUCED | 490 unique keys. All 98 v5 columns are string-identical, and each v5 line is a byte prefix of the candidate line. The new total is exact (max abs diff 0.0). No y0 ≤ 0 bins. The MC-stat column is not in the total. Medians are as claimed. |
| 6 | Ordering, charge, bootstrap | no defect | Charge-major layout verified from `per_bin`. The budget `nominal_cross_section_nb` equals y0/351000 to 1e-15. |

## Findings

- **MINOR-1: clip before scale.** wn is clipped to [0, 10] (l.1421) before scaling (l.1425), so tracks with wn < 0.5 get
  negative weights at κ=+1. Raw NW goes down to −2.58 (very-low-p K).
  - Only 1–2 negative body cells survive summation (−1.1 / −2.3 vs |D| ≈ 4.7e5).
  - Capping them changes the term by ≤ 1.7e-5 absolute and the integrated value by 7e-8.
  - Document this, or clip after scaling in any future rebuild.
- **MINOR-2: (anti)protons are not covered.** NW ≡ 1 for p/p̄, so the h+/h− ±0.004% and A_h ±0.00002 statements hold
  only for the π/K/e/μ loss that NW models. The caveat must be attached directly to them.
- **MINOR-3: convention dependence.** The loss-to-miss form keeps P fixed and is correct to first order under either
  nominal convention.
  - If the +0.453% coherence fix is adopted, regenerate the term (expected change < 0.01%).
  - The covariance edition must state which signed vector it uses and how the κ=−1 asymmetry is handled.
- **MINOR-4: wording.** The linearity figure is 8.7e-15, not 2.6e-15. The largest per-bin values are in sparse S3_x0
  high-z bins (≤ 5.56%), where v5 is already 29–61%, so they are understood.
- **Not defects (user decisions):** the fake-leg double count with `bgsub_fakefrac_rel` (92% of the fake-leg deficit is in
  `h_fake_existing_other`); the possible tracking overlap; independence from `nonprimary_norm_v5`.

## Install conditions

1. Attach the (anti)proton caveat directly to the ratio and A_h statements.
2. Record the clip-before-scale behaviour and its impact (≤ 1.7e-5).
3. Build a rank-1 covariance block with ρ = 1 across charges from `nucliaweight_primaryloss_signed_rel`, state the κ=−1
   handling, and re-run the gates.
4. Regenerate the term if the coherence fix is adopted.
5. Install via the sha-guarded canonical-edit path with fresh re-reads, and only with user sign-off.

The critic made no remote writes outside a `/tmp` scratch directory, which was deleted, and submitted no Condor jobs.
