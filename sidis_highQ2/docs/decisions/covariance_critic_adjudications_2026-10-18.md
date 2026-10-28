# Supervisor adjudications — critic wave 1 (covariance matrices), 2026-10-18

Governing critiques (all three persisted under sidis_highQ2/docs/critiques/):
- critic-covariance-matrices-numerical-cold_2026-10-18.md
- critic-rank1-columns-numerical_2026-10-18.md
- critic-covariance-matrices-construction-hot_2026-10-18.md

## Adjudications (binding for S8/S9)

A1 (D4 cross-block, MAJOR/UNSOUND): ADOPT fix (a). Cross-block re-based onto sidecar
marginals: cross' = rho_joint,cross (elementwise) sigma_sc,pos sigma_sc,neg^T. Sidecar
per-charge blocks stay the TIER_A C_stat marginals. MANDATORY S9 pre-assembly PSD gate on
the full 490x490 (eigvalsh, tolerance -1e-12 x scale). Pre-registered fallback order:
(i) scalar shrinkage of the cross-block toward zero until PSD, record factor; (ii) full-matrix
Schur re-base rho_joint,full o sigma_sc sigma_sc^T ONLY with new supervisor sign-off (changes
within-charge correlations). Fix (b) (joint per-charge blocks supersede sidecar) REJECTED for
this package. Guard: flag correlations involving the 45 ever-clipped fine bins at the gate.
Over-dispersion numbers (+5.06%/+4.89% diag median, +86.2%/+84.2% max sigma, 4.01%/4.00%
entries >3 sigma) go verbatim into the S9 assumptions register at the C_stat term
(publish-not-tune retained; convention change is a release-level decision, out of scope).

A2 (elec_theta_sym, BLOCKER): D6 ruling TAKEN — apply S4-recommended smoothing to the
published elec_theta_sym column per the phi_h Gaussian rolling-window precedent; raw column
retained in custody; no per-bin zeroing. S8 executes; S9 consumes the smoothed column.

A3 (elec_energy convention gap): governed by recorded S6 verdict — D2 NOT APPLIED, +/-1%
symmetrized unscaled, budget convention (env twin) primary for reconciliation. S8 documents
the super-linear upper-bound-convention reading (~1.5x a linear-scaled 0.5% response) and
attributes the sym-vs-env gap (median 0.74x, per-bin to 13.7x) to named convention causes.

A4 (S7 eps-role attribution): supervisor sign-off GRANTED with conditions. The attribution
(primary-variant FAIL dominated by the eps/p_eff normalization role; S_coh shape-role
variant ~0.60%/0.72%, frac_inside 0.710/0.722) is accepted as plausible, NOT proven.
Conditions: S9 unpropagated_sources carries the FULL variant bracket (primary 8.82%/9.54%,
frac_inside 0.0857/0.1102, integrated +0.480%/+0.560%) AND the explicit statement that the
primary medians exceed the total budget medians (5.9764%/6.2968%). ibu_model budget component
unchanged. Labelling hazard fix WITHOUT editing the original record: S9/S10 must read gate
status from a new additive addendum file (e.g. truthreweight/gate_status_addendum_2026-10-18.json
with gate_status=FAIL, job_status=PASS, pointer to original); original
truthreweight_closure.json stays untouched.

A5 (C_MCstat post-hoc bias bound 0.5): CONFIRMED as supervisor (measured bias/sigma medians
0.042/0.051; outcome insensitive to bound in [0.1,1]). Recorded as post-hoc-confirmed.
Fixed low-Neff cells (12,207/11,834, downward sparse-cell bias) and D5 zero cross-charge
block (unmeasured, conservative for h+/h- ratios) go into the S9 register with
direction-of-bias statements. S9 must apply a pre-registered rank-deficiency policy
(eigen-floor/pseudo-inverse) for the 199/245 per-charge rank.

A6 (S9 input list): PINNED and machine-enforced in the assembly script:
- rank-1: hfs_jes, hfs_rces, hfs_theta, elec_energy_sym, elec_theta_sym (smoothed per A2),
  ibu_model, ibu_reg, detint_model, c_rad_gen; lumi = rank-1 peel.
- diagonal-only: tracking (D7), all mag_* columns.
- excluded: c_rad_gen_rawhalf (D8 retained custody), elec_*_env twins (reconciliation only).

A7 (ibu_model/ibu_reg sign classification): extended shard bootstrap (seed 82345) in flight;
S8 must consume its outcome BEFORE the ibu_model/ibu_reg rank-1 columns freeze. If
noise-dominated per D6, S8 smooths the published sign pattern (magnitudes unchanged —
budget-exact) and records the decision; raw custody retained.

A8 (MINOR records into S8): the 6 unsubtracted-vs-subtracted sign-flip bins (jes pos
26/69/109/133, neg 113; theta pos b197); correction of the "S4-validated" detint_model
registry wording (no detint leg exists in S4; provenance = B1 aggregation machinery only);
S7 RNG-seed ledger gap to be recorded as missing evidence.

## Constraint reminders (unchanged)
- Budget public quote total_with_bgsub19_rel untouched; covariance package separate deliverable.
- Additive dated directories only; no central release, budget, sidecar, or 2026-10-14 edits.
- Single denominator: official_reunfold IBU k=4 on data_sub'; L=351000.0 nb^-1.

## A9 — D6 ruling on ibu_model published column (2026-10-19, from extended bootstrap seed 82345)

Evidence: shard_bootstrap/model_reg/significance_report_modelreg.json — ibu_model published
pos frac z>=2 0.5143 (intermediate, raw_ok) / neg 0.4816 (noise-dominated, smooth_recommended),
both threshold-adjacent (K=20: 0.5020/0.5224); z median 1.998 sits at the 2-sigma edge;
the raw RG/DJ half-diff that supplies D3 floor-bound signs is noise-dominated in BOTH charges
(frac z>=2 0.3102/0.2816). ibu_reg is REAL both charges (0.8286/0.7959 frac z>=2) — raw kept.

RULING: smooth the SIGN PATTERN of the published ibu_model column for BOTH charges
(phi_h Gaussian rolling-window precedent), uniform per-source treatment. Rationale:
(a) neg is smooth_recommended at the pre-registered K=50 adjudicator; (b) pos 0.5143 vs
K=20 0.5020 is within estimator noise of the threshold — not robust on either side;
(c) the sign-injection mechanism (sub-floor raw half-diff at 65% floor-bound bins) is
confirmed noise-dominated in both charges; (d) per-charge split treatment would create an
artificial charge asymmetry in one source's correlation structure. Magnitudes stay
bit-exact vs budget (floor/half rule unchanged) — only correlation sign coherence changes.
Raw custody columns retained; no per-bin zeroing. ibu_reg published column stays RAW (real).
Borderline caveat recorded verbatim in the S8 record.

## A10 — wave-2 ruling: fake-column MC-stat double count (2026-10-19, from S11 critics 1+2)

Evidence: S11 critic 1 (component census) + critic 2 (quantification). The published C_MCstat
(S3 response-replica toys, K=200, seed 52345, replica-own f_i) perturbs fake-column cells in
472/487 pos / 461/484 neg fake-cell bins — it already carries the fake-column MC-stat. The
package ALSO sums cov_fakecol_mcstat (budget G12 column, bitwise 390/490 vs G12, max rel
2.15e-16) as C_total term #17. Same effect enters both totals twice: duplicated variance
~12.74%/13.35% of C_MCstat median (up to 76.4%/71.8% worst bins); shipped total per-bin sigma
inflated ~0.1043%/0.1152% median (max 3.03%/2.27%). Conservative direction, but unadjudicated,
invisible to all 9 gates, and the g2 mcstat_addition annotation ("NO budget counterpart") is
false for the fake-cell component.

RULING: DROP cov_fakecol_mcstat from the 21-term composition of cov_total_with_lumi and
cov_total_no_lumi (totals become 20-term with lumi / 19-term without). Rationale: C_MCstat is
the authoritative toy-based carrier (full measured covariance incl. fake-cell stat per S3
census; the standalone G12 slice agrees with it — slice-vs-G12 ratio med 0.9989/1.0009);
disjointness principle (each uncertainty exactly once) per the expert prescription; dropping a
positive diagonal term cannot break PSD. The cov_fakecol_mcstat npy STAYS in the package as a
lineage/component product labelled consume=FALSE with the named reconciliation cause
"contained in C_MCstat (S3 census 472/461 perturbed fake cells); G12 budget term not re-added
— disjointness". Required with the rebuild: (i) correct the g2 mcstat_addition annotation;
(ii) add the overlap to the double-count-guards section; (iii) rerun gates g1/g2/g4/g6/g7/g8/g9
and refresh manifest/validation/record with an amendment history entry; (iv) decisive post-fix
check: G12/fakecol term provably absent from the corrected totals (quadrature identity) and
the corrected-total diagonal change matches the predicted 1−sqrt(1−(G12/sigma_tot)^2)
(med 0.1043%/0.1152%); (v) add a validation guard against toy-matrix-vs-budget-diagonal
duplication for future builds.

## A11 — doc fixes from S11 critic 3 (publication readiness) — apply in the same amendment
1. MAJOR: README grid line must read 2 x (7 (Q2,x) cells S0_x0,S0_x1,S1_x0,S1_x1,S2_x0,S2_x1,
   S3_x0 x 7 z sub-bins x 5 pT bins) = 245 per charge; remove "5 Q2 strata S0-S4" and "7 pT".
2. Add inherited release-status line verbatim from the release CSV caveat: central values are
   PROVISIONAL DIAGNOSTIC (no C_rad, not Born-level, no final physics approval, multiplicities
   blocked) — status inherited by this package.
3. Fix dest_inventory accounting (69 files[] + manifest.json self + s09_record.json = state
   the convention explicitly).
4. Add per-cell representative (Q2,x) table (or explicit do-not-use-midpoints ruling), a short
   chi2/nuisance usage snippet, and an "analysis note update pending" flag.
5. Add cov_stat borderline-PSD note (min eig −9.99999717598279e-13 vs −1e-12 tolerance,
   numerical rank 489, one effectively-null direction; use g8 inversion guidance); fix the
   census pointer to lineage/cov_mcstat_product.json; expand data_sub' at first use; state
   sigma_nb is bin-integrated; add the delta_k shift-vector usage line.
