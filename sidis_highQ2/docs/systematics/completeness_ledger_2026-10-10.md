# z005 systematic-source completeness ledger (2026-10-10)

Living ledger per `FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` §5.2 ("publish a source
ledger containing every register row, status, applicability, reference, magnitude, and exact
artifact path; no missing result may be called zero"). Basis: the PROVISIONAL→final gap audit
(`docs/systematics/provisional_to_final_gap_audit_2026-10-10.md`, verdict: 13/13 components
VALID on the current chain, 0 STALE, 0 UNVERIFIABLE) and the component derivation record
(`docs/systematics/budget_13components_derivation_2026-10-10.md`). Canonical budget:
`H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/`
(13 components, totals median h+ 5.7093% / h− 6.1523%).

**Coverage:** all 24 frozen-spec register rows (register row 13 `crad_rel` is implemented
today as the two in-budget components `c_rad_gen_rel` + `c_rad_stat_rel`, see A12/A13),
the three post-spec in-budget components, the pending `hfs_phi_rel`, and five cross-cutting
gates/decisions — 31 rows total.

## Decision log (user decisions, 2026-10-10; recorded in the spec addendum)

- **D-2026-10-10-1** `phi_h_gauss_rel` ADOPTED, option A (floored envelope; no re-derivation;
  conservative ~1.6%-median model double-count caveat mandatory).
- **D-2026-10-10-2** `tracking_eff_m0p5_rel` baseline CONFIRMED (16-seed −0.5% ensemble;
  frozen 1.0198% analytic NOT adopted; no double-count).
- **D-2026-10-10-3** P2 12-campaign paired detector-leg sequence APPROVED: eElecEn ± →
  JES+RCES ± → hfs_theta ± → elec_theta ± → hfs_phi ±; each campaign gated on smoke test +
  normalization check + canonical 658-file list (cures the HFS 660-file defect, gap audit §3.4);
  `hfs_phi_rel` becomes the 14th component on completion.

## A. In-budget components of the canonical 13-component budget

| # | Source (spec row) | Median h+ / h− | Status | Blocker / decision | Disposition path | Primary artifact |
|---|---|---|---|---|---|---|
| A1 | `ibu_stat_rel` (row 1) | 3.5900 / 3.8314 % | **IN-BUDGET final-eligible** | h+/h− cross-block (E31) gates final `A_h` only | none for cross sections; measure cross-block for `A_h` | `output_v6/systematics/stat_toys_coherentprior_2026-10-07/` (1000 toys, cluster 5380017) |
| A2 | `ibu_model_rel` (row 2) | 1.4006 / 1.4255 % | **IN-BUDGET final-eligible** | P-RAPGAP (E27); B1 3-SEM working-point FAIL on 1 h+ / 3 h− supported bins (report-only) | P-RAPGAP decision + B1 follow-up attribution study (no rederivation) | `ibu_native_coherentprior_2026-10-07/` (cluster 5379991) + `studies/p1_replica_floors_coherentprior_2026-10-07/` (cluster 5380196) |
| A3 | `ibu_reg_rel` (row 3) | 0.5769 / 0.5662 % | **IN-BUDGET final-eligible** | final covariance needs signed sidecar | emit signed k±1 sidecar at covariance-build time | `ibu_native_coherentprior_2026-10-07/kscan/` |
| A4 | `hfs_jes_rel` (row 4) | 0.3052 / 0.3498 % | **IN-BUDGET provisional-pending-paired-rerun** | one-sided `hfs_jes_down`; jet-classifier audit; HFS 660-file input defect (gap audit §3.4) | P2 campaign 2: paired ±1% in-jet (kT R=1.0, pT>5, −1.5<η<2.75), pin 658-file list | legacy: `systematics/hfs/hfs_jes_down/tunfold_official_v4_finalcandidate_..._percharge/`; coherent: `detector_shifted_coherentprior_2026-10-07/` |
| A5 | `hfs_rces_rel` (row 5) | 0.2410 / 0.2446 % | **IN-BUDGET provisional-pending-paired-rerun** | one-sided `hfs_rces_up`; `eHadEnRCES` relvol10 enum audit; 660-file defect | P2 campaign 2: paired ±1% out-of-jet, pin 658 | as A4, tag `hfs_rces_up` |
| A6 | `hfs_theta_rel` (row 6) | 0.2406 / 0.2783 % | **IN-BUDGET provisional-pending-paired-rerun** | one-sided `hfs_theta_up`; two-sided response even/quadratic, quarantined (signed means +0.2568%/+0.2588%) | P2 campaign 3: quarantine diagnosis, then paired ±1 mrad | as A4, tag `hfs_theta_up` |
| A7 | `elec_energy_rel` (row 8) | 1.3537 / 1.6635 % | **IN-BUDGET provisional-pending-paired-rerun** | +1% one-sided ≈ 2× frozen ±0.5%-per-wheel; "replace, do not rescale" | P2 campaign 1 (first): paired ±, all five LAr-wheel setters per side (BBE, CB1+2, CB3, FB1, FB2+IF) | legacy: `elecE_p1pct` (658-file vector); coherent: `detector_shifted_coherentprior_2026-10-07/` |
| A8 | `elec_theta_rel` (row 9) | 0.4524 / 0.5000 % | **IN-BUDGET provisional-pending-paired-rerun** | +1 mrad alone; three-region `eElecTh` + `eElecTrTh` co-shift unproven | P2 campaign 4: setter-level co-shift audit, then paired ±1 mrad | legacy: `elecTheta_p1mrad` (658-file vector); coherent: as A7 |
| A9 | `lumi_rel` (row 14) | 2.70 / 2.70 % (flat, rank-1) | **IN-BUDGET final-eligible** | derivation-memo gap (P0 item 1) | normalization-owner derivation note; excluded from multiplicities and `A_h` | assignment in `total_uncertainty_z005_2026-06-13_with_electron_lumi/provenance.json`; ratified `docs/status/current-release.md`, `docs/status/normalization-gate-releasebin-2026-10-09.md` |
| A10 | `tracking_eff_m0p5_rel` (row 10) | 0.1806 / 0.1828 % | **DECIDED-ADOPTED (2026-10-10, D-…-2)** | none — baseline confirmed; frozen 1.0198% analytic NOT adopted (no double-count) | closed; no rerun | `tracking_ensemble_coherentprior_2026-10-07/` (16/16 seeds) |
| A11 | `phi_h_gauss_rel` (post-spec; adoption 2026-10-02) | 1.0482 / 1.0550 % | **DECIDED-ADOPTED (2026-10-10, D-…-1, option A)** | mandatory caveat: conservative envelope incl. ~1.6%-median model double-count; no noise guard | closed as adopted-with-caveat; caveat travels with any use | `phi_h_coherentprior_2026-10-06/` (`phi_h_gauss_rel_floored.csv`); adoption record `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md` |
| A12 | `c_rad_gen_rel` (implements row 13) | 0.5844 / 0.5804 % (emfix) | **IN-BUDGET final-eligible** | P-CRAD-LUMI (E29): C_rad applied but NOT publication-grade; z<0.05 xF-definition sensitivity caveat-only | publication-grade C_rad review (provenance closure); no component rerun | releasebin V3 on `data/radiative/{djangoh14,rapgap}_crad_releasebin_2026-10-09/`; emfix budget package |
| A13 | `c_rad_stat_rel` (implements row 13) | 0.4272 / 0.4429 % | **IN-BUDGET final-eligible** | P-CRAD-LUMI (E29); rank-1 covariance is a 100%-coherence envelope assumption, not measured | as A12; measured correlation if ever required | applied DJ14 releasebin table `relative_stat_unc` column; `covariance_cradstat/` |

## B. Pending new component

| # | Source (spec row) | Status | Blocker / decision | Disposition path | Primary artifact |
|---|---|---|---|---|---|
| B14 | `hfs_phi_rel` (row 7, required) | **NOT-EVALUATED** (production approved) | no artifact exists today | P2 campaign 5 (D-2026-10-10-3): paired ±20 mrad `eHadPh`; becomes the 14th budget component on completion | none yet; campaign per gap audit §4 |

## C. Blocked register rows (no value may enter a final total; none is zero)

| # | Source (spec row) | Status | Blocker / decision | Disposition path |
|---|---|---|---|---|
| C15 | `tracking_chargedep_rel` (row 11) | **BLOCKED** | existing 0.3%/track source fails the 2.5%-on-`A_h` Aaron sanity (0.003/0.025=0.12, outside 30%); must not support an `A_h` release | regenerate to the 2.5%-on-`A_h` envelope with charge-resolved validation, or omit `A_h`; fully anti-correlated h+/h− |
| C16 | `response_mcstat_rel` (row 12) | **BLOCKED** | no 490-bin response-MC statistical covariance emitted; response `sumw2`, fake-column uncertainty, replicas not propagated | response-bootstrap replica campaign + validated full covariance (spec decision 4); blocks publication-grade final |
| C17 | `trigger_rel` (row 15) | **BLOCKED** | ≤0.5% placeholder vs older 1.2% quote; no pinned value | S67 data/MC or monitor-trigger measurement; until then labelled placeholder, excluded from final claims |
| C18 | `NucliaWeight_rel` (row 16) | **BLOCKED** | supervisor adoption decision pending; no budget column (provisional per-event weight variation exists at `systematics/nuclint_absorption_provisional_20260909/`) | decision + column, or documented exclusion; cancels in `A_h` only after charge-resolved check |
| C19 | `background_rel` (row 17) | **BLOCKED** | P-ALPHA open (E30); alpha0p2 aggregates lack `overlap_980`/`n_raw_980`; template exposure provenance open; subtraction policy undecided — release is raw/no-bgsub, fixed response fake-fraction subtraction only (f = 10.7141% h+ / 10.4019% h−, **no propagated uncertainty**); former retirement voided by the 2026-09-25 fLumi correction | regenerate alpha0p2 aggregates with `overlap_980`/`n_raw_980`, close template provenance, P-ALPHA validation, then a subtraction-policy decision (subtract + uncertainty, or ratify no-subtraction convention) |
| C20 | `diffvm_rel` (row 18) | **BLOCKED** | assumed zero, not assessed zero: rho assessed 2026-10-07 (306.31±6.45 expected tracks, 0.0074% integrated — negligible); phi/omega/proton-dissociative unassessed | complete the DIFFVM assessment or document sample unavailability |
| C21 | `gammap_bkg_30pct_rel` (row 19) | **BLOCKED** | no gamma-p template is subtracted (central is raw/no-bgsub) | document no-subtraction per spec (never emit zero); moot if raw/no-bgsub is ratified as final |

## D. Dropped / diagnostic register rows (frozen spec status retained)

| # | Source (spec row) | Status | Reason (frozen spec) |
|---|---|---|---|
| D22 | `electron_phi_rel` (row 20) | **NOT-EVALUATED (DROPPED)** | no pinned HERA-II/SIDIS magnitude or g1j_ES recipe; no value may be invented |
| D23 | `noise_energy_rel` (row 21) | **NOT-EVALUATED (DROPPED)** | no pinned SIDIS/H1 precedent; no default shift |
| D24 | `tracking_resolution_rel` (row 22) | **NOT-EVALUATED (DROPPED)** | ±50% smearing proposal has no H1-precedent prescription; legacy rank-1 source critic-fatal |
| D25 | `combined_forward_tracking_10pct_rel` (row 23) | **NOT-EVALUATED (DROPPED)** | published hadron fiducial is within central tracking (|η_h|<1.3); do not add |
| D26 | `method_spread_BBB_IBU_TUnfold_rel` (row 24) | **DIAGNOSTIC** | method spread is coverage/validation, never a systematic; the nominal-method decision itself is P-METHOD (E28) |

## E. Cross-cutting gates and decisions (not magnitude sources; they gate rows above)

| # | Gate | Status | Gates which rows | Disposition path |
|---|---|---|---|---|
| E27 | **P-RAPGAP** — RAPGAP response/provenance disposition | **BLOCKED (decision)** | A2 (`ibu_model_rel`) | supervisor decision on the RAPGAP model-leg disposition; note RAPGAP31 shares its generator model with the central response (not independent of data) — documented in the 4-generator comparison package caveats |
| E28 | **P-METHOD** — nominal unfolding method | **BLOCKED (decision)** | nominal release; D26 | coherent `body+miss` adoption (2026-10-07) resolved the prior-conservation mismatch; remaining: IBU/BBB vs native-v6-TUnfold method validation and release decision |
| E29 | **P-CRAD-LUMI** — C_rad exposure provenance / publication grade | **BLOCKED (provenance)** | A12, A13; all Born-level claims | exposure provenance FILE-SET-RECONCILED (generator-log-open) → generator-log closure + publication-grade review; until then the releasebin baseline is PROVISIONAL and Born quotes must state basis |
| E30 | **P-ALPHA** — alpha-veto/background response-consistency validation | **BLOCKED (validation)** | C19 (`background_rel`) | validation campaign + policy decision; carried OPEN in every promotion's does-not-cure list |
| E31 | **h+/h− statistical cross-block** | **NOT-EVALUATED** (zero-by-assumption) | A1 for `A_h`; final asymmetry covariance | joint toy/bootstrap measurement from the same data events, or an approved zero proof; the current zero assumption forbids final `A_h` covariance |

## Plan status (per gap audit §7, updated 2026-10-10)

- **P0 (doc-only):** lumi derivation memo — OPEN; completeness ledger — THIS DOCUMENT (done);
  analysis-note corrections — OPEN.
- **P1 (decisions/small studies):** tracking baseline — CLOSED (D-2026-10-10-2); phi_h
  disposition — CLOSED (D-2026-10-10-1); B1 follow-up attribution — OPEN; symmetrisation +
  Tier-B covariance model decisions — OPEN (must precede P2 interpretation).
- **P2 (production):** APPROVED 2026-10-10 (D-2026-10-10-3), order eElecEn ± → JES+RCES ± →
  hfs_theta ± → elec_theta ± → hfs_phi ±; ~12,300 re-reco jobs total; per-campaign gates:
  smoke test, normalization check, canonical 658-file list; NOT YET LAUNCHED — this ledger
  is the pre-launch record.
- **P3 (blocked / separate chains):** C15–C21, E27–E31 per above; full covariance package
  rebuild and P-PLOTS after P2.

## Citations

- Gap audit: `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` (chain bases,
  dates, §3.4 HFS 660-file defect, §4 prescriptions/scope, §7 plan).
- Derivation record: `docs/systematics/budget_13components_derivation_2026-10-10.md`
  (per-component derivations, medians, provenance).
- Frozen spec + 2026-10-10 decision addendum:
  `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`.
- Canonical budget: `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/`.
- phi_h adoption record: `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md`.
- State: `STATE.md`, `docs/RUN_STATUS.md`, `docs/status/current-release.md`,
  `docs/status/radiative-corrections.md`, `docs/status/normalization-gate-releasebin-2026-10-09.md`.
