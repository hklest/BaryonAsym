# bgsub_fitnorm campaign spec — data-driven e/V0 template-normalization fit (bgsub_fitnorm_rel)

- **Date**: 2026-10-19
- **Status**: AUTHORIZED SPECIFICATION, not yet executed. User decision 2026-10-19 (option 1):
  data-driven e/V0 template-normalization fit campaign authorized; flat-30% and e-envelope
  options rejected; tightened-chain stability test NOT authorized.
- **Basis**: cone+S2′+Set-R′ clean release (promoted 2026-10-17, canonical). Set R′ =
  e_cone_applied + V0_genavg + lowQ2, subtracted from the cone data vector at RAW reconstructed
  level, pre-unfold. Budget denominators: official_reunfold IBU k=4 on data_sub′
  (RAW integrated h+ 10.508811 nb / h− 10.217503 nb, L 351000.0 nb⁻¹).
- **Deliverable**: one new systematic component `bgsub_fitnorm_rel` on the 490-bin release grid,
  assembled into a 20-component candidate budget. Promotion to `data_release/` is user-gated and
  out of scope.
- **All paths and numbers below were verified against the workspace on 2026-10-19.** Design-stage
  estimates carried over from the (unpersisted) 2026-10-18 design report are labeled as such.

Absolute path prefix used throughout: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`
(abbreviated `$PKG`).

---

## 1. Fit model

**Observable.** x = |DcaPrime| (branch `DcaPrime`, cm; use absolute value) for all selected tracks
of a fit group, pooled over all (x, z, pT_h) grid cells inside the group. Selection is mirrored
verbatim from the canonical vector producer (Section 3). Motivation (design-stage measurements):
in-selection |Dca| medians signal 0.012–0.056 cm, e-class 0.096–0.195 cm, V0-class 0.126–0.203 cm;
the tail carries the discrimination (data |Dca|>0.5 tail ≈ 7.3%).

**Fit binning (pre-registered).** 36 bins: 20 × 0.01 cm over [0, 0.20]; 6 × 0.05 over (0.20, 0.50];
5 × 0.10 over (0.50, 1.00]; 4 × 0.25 over (1.00, 2.00]; 1 overflow bin (2.00, +∞). The selection
permits |DcaPrime| < 2.0·cosh(η) (≤ 5.15 cm at |η|→1.6); the overflow bin absorbs the remainder.

**Components and parameters** (per fit group g = charge × Q2 stratum):

| Component | Class definition (stored branches) | Normalization | Free parameter |
|---|---|---|---|
| kept | selected ∧ in-grid ∧ NOT e ∧ NOT V0 (includes matched hadrons, detint, unmatched) | N_kept | **free** (≥ 0) |
| e | `IsMatchedElectron` == 1 | E_g (pooled RG+DJ predicted, fixed) | multiplier **μ_e free** (≥ 0) |
| V0 | `IsMatchedV0Daughter` == 1 AND `IsMatchedElectron` == 0 | V_g (fixed) | multiplier **μ_V0 free** (≥ 0) |
| lowQ2 | leakage tracks from 7091 source (Section 3) | L_g (fixed, pinned to released Σ lowq2) | none — fixed |

- Signal normalization answer: **N_kept is free**, not profiled against an external constraint and
  not fixed. It must absorb data/MC differences of the ~92% kept component so μ_e, μ_V0 are
  unbiased. The fitted N_kept is diagnostic only; no released quantity changes because of it.
- kept deliberately includes detint and unmatched tracks: the release does NOT subtract them
  (detint non-removal is carried by `detint_removal_rel`; unmatched ≈ 0.07%). The fit model must
  mirror the subtraction set exactly: what is subtracted is e + V0 + lowQ2, everything else is kept.
- e∩V0 overlap: the release counts e and V0 independently with the overlap IGNORED (user decision
  2026-10-08; 112.2093 cone-applied scaled tracks integrated = 0.0029% of D_cone). Fit templates
  must be disjoint, so V0 excludes e-flagged tracks; the overlap is recorded per group
  (`e_v0_overlap`). μ_V0 is applied to the RELEASED `v0_avg` as-is; the 0.085% inconsistency vs
  the disjoint template is recorded, not corrected.
- lowQ2 is **never fitted** (0.0054% of D_cone). Its uncertainty remains carried by the existing
  bgsub components per the release convention.

**Likelihood.** Binned Poisson (extended ML):
`L = Π_k Pois(n_k | ν_k) · Π_k Pen(γ_k)` with
`ν_k = γ_k · ( N_kept·s_k + μ_e·E_g·e_k + μ_V0·V_g·v_k + L_g·l_k )`,
where s, e, v, l are unit-normalized template shapes and n_k are unweighted data counts.

**MC-stat template errors.** Barlow–Beeston LITE: one γ_k per bin;
`N_eff,k = (Σ w_k)² / Σ w²_k` pooled over the four classes; penalty `Σ_k N_eff,k (γ_k − 1)²`.
Pre-approved fallback if minimization is unstable: 200 Poisson-bootstrap toy refits of the
templates, σ(μ) = RMS; record which treatment was used.

**Weights.** MC: `weight1 * weight2` (NucliaWeight EXCLUDED from nominal, producer convention)
× generator combined scale (Section 8). Data: unweighted raw counts.

**Minimizer.** iminuit preferred, scipy bounded L-BFGS fallback; must return the covariance of
(μ_e, μ_V0) (N_kept and γ profiled/fit alongside; the 2×2 μ-block is what propagates).

**Goodness of fit.** Post-fit Pearson χ² with total variance (data Poisson + template term via
γ̂), ndf = nbins − 3, p-value per group. Guard in Section 5 (R6).

**Per-group yield bookkeeping** (all recorded in the results CSV): N_data (= Σ D_cone over the
group's release bins, exact integer — see Section 7 gate), E_g, V_g, L_g, K_g (kept), overlap_g,
N_kept_fit, and the validation deltas vs the released vectors.

---

## 2. Fit groups

**Eight groups**: charge ∈ {h+, h−} × stratum ∈ {S0, S1, S2, S3}, edges from the authoritative
binmap (`Q2_STRATA_RECO`, applied to `Q2es`):

| Stratum | Q2 range (GeV²) |
|---|---|
| S0 | [150, 300) |
| S1 | [300, 700) |
| S2 | [700, 1500) |
| S3 | [1500, 20000) |

Group assignment at projection time: stratum from `Q2es` via these edges; charge from
`sign(Charge)`. Cross-check against the 490 CSV `q2_stratum` column of the corresponding cells.

**Application rule onto the 490-bin grid.** Each release bin i carries (charge_i, q2_stratum,i) in
the released 490 vector CSV. The group's μ and σ(μ) apply to ALL bins of that group:

- `μ_comb,g = (E_g·μ_e,g + V_g·μ_V0,g) / (E_g + V_g)`
- `σ(μ_comb,g) = sqrt(J C Jᵀ)`, `J = (E_g/(E_g+V_g), V_g/(E_g+V_g))`, C = 2×2 covariance of (μ_e, μ_V0).

**Granularity assumption (pre-registered).** The data/MC normalization mis-modeling is a uniform
multiplicative factor within a group. (Q2,z) refinement — feasible in about half the groups by the
design study — stays optional and out of this campaign. Per-490-bin fits are infeasible
(σ(μ_comb) ≈ 11%; 122/126 bins < 1000 tracks).

**Feasibility anchors.** Design-stage estimates (toy Fisher information; the campaign supersedes
them): σ(μ_comb) ≈ 0.9% (S0) → 3.6% (S3) per charge-stratum; σ(μ_e) ≈ 2.8% → 11%. Group content
from the released 490 CSV (verified 2026-10-19): per-group Σ f_i over release bins with
data_sub′ > 0 — pos S0 2.82 / S1 2.49 / S2 2.51 / S3 1.18; neg S0 2.91 / S1 2.68 / S2 2.92 /
S3 1.41 (69–70 bins each; S3 34–35), f_i = (e_avg+v0_avg)/data_sub′.

**Fallback guard (pre-registered).** If a group has N_data < 5000 tracks, or σ(μ_comb) > 0.20, or
the fit does not converge: merge that stratum with the adjacent LOWER stratum (S3→S2 first, then
S1→S0), refit, record `merged=true`. If merging still fails → STOP (R6).

---

## 3. Templates and inputs

Trees live under `$PKG/output_v6/central/sidistree/`; tree name is **`ChargedTot`** in every file.

| Role | Path | Files |
|---|---|---|
| Data | `$PKG/output_v6/central/sidistree/data/{em0405,em06,ep0304,ep0607}/` | 152+100+83+323 = **658**; names `sidistree_<period>_d<NN>_<NNNN>.root` |
| RAPGAP31 templates | `$PKG/output_v6/central/sidistree/mc/rapgap31/{7071,7094,7096,7097}/` | 61+121+121+61 = **364**; `sidistree_rapgap31_<reqid>_<period>_<NNNN>.root` |
| DJANGOH14 templates | `$PKG/output_v6/central/sidistree/mc/djangoh14/{7072,7091,7092,7093}/` | 88+201+141+71 = **501**; `sidistree_djangoh14_<reqid>_<period>_<NNNN>.root`. **NEVER include `merged_7091.root` in this set** (duplicates ReqID 7091) |
| lowQ2 shape source | `$PKG/output_v6/central/sidistree/mc/djangoh14/merged_7091.root` | 1 file, 29.2 GB. djangoh14/7091 RAD ep0607, L_MC = 4,379,935 nb⁻¹, net scale 0.08013817556653238 = 351000/4379935 |
| Released Set-R′ vectors | `$PKG/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/vectors/` | `bgsub_setrprime_vectors_490.csv` (canonical application grid; columns incl. charge `pos`/`neg`, `q2_stratum`, `e_avg`, `v0_avg`, `lowq2`, `B_R_prime`, `data_sub_prime`, `D_cone`), `..._980.csv` (fine-grid audit view, charge `h+`/`h-`), `..._vectors.json` (gate/anchors) |
| 19-comp budget (reassembly parent) | `$PKG/output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/bgsub_setrprime_budget19_cleandenom.csv` | 490 rows; columns incl. `bgsub_stat_rel`, `bgsub_model_rel`, `bgsub_stage_rel`, `bgsub_fakefrac_rel`, `total_with_bgsub19_rel` |
| Packaged template bank (validation anchors ONLY) | `$PKG/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/` | npz keys verified: `yield_980, var_980, sumw_980, sumw2_980, per-charge variants` — **yields only, no shape histograms** |

**Do templates come from existing stage2 artifacts?** No. The packaged npz and the vector CSVs
contain per-bin yields only (verified keys above), so the |DcaPrime| (and variation) shape
templates **must be re-projected from the trees**. The packaged/stage2 artifacts serve as
normalization anchors, e.g. RG electron `yield_980[0] = 9287.3767 = sumw 241654.129 ×
0.03843251824641288`, matching the 980 CSV `e_rg` (h+, bin 0) exactly.

**Signal/kept shape source**: RG31+DJ14 pooled (all 8 ReqID dirs), kept class as defined in
Section 1. **lowQ2 shape**: project `merged_7091.root` tracks passing the full standard selection
(the leaked population). Normalization is pinned to the released Σ lowq2 per group in all cases.
Pre-registered sensitivity guard: refit (h+, S0) once with the lowQ2 shape replaced by the kept
shape; |Δμ_comb| must be < 0.001 (lowQ2 is 0.0054% of D_cone; the historical template's alpha0p1
radius mismatch is bounded << 0.01% by the carried caveat). `mc_bg/djangoh14_lowQ2bkgd/` (7087,
800 files) is NOT the source of the released lowQ2 vector and is NOT used by this campaign.

**Selection mirror (authoritative implementation).** Import the producer module —
`$PKG/scripts/canonical_highQ2_sidis_2026-06-30/systematics/produce_reco_background_vector_alpha0p2_v2.py`
— and use its constants/functions directly (`EVENT_CUTS_RECO`, `EVENT_CLEANUP_RECO`,
`TRACK_CUTS_RECO`, `ELECTRON_ALPHA_VETO_RAD`, `compute_reco_electron_alpha`,
`compute_pth_cm_and_z`, `load_binmap`, indexing, `combined_scale`, `L_MC_PERIOD_NB`), running its
`ProvenanceVerifier` and `_self_check_canonical_ptz` at driver start. Do not re-type constants.
Semantics (verified in source 2026-10-19):

- Event: `isfinite(Q2es) & 150 < Q2es < 20000`; `isfinite(Yes) & 0.1 < Yes < 0.7`;
  `isfinite(Empz) & Empz > 45`; `isfinite(PVz) & |PVz| < 35`.
- Track: `Pt > 0.15`; `|Eta| < 1.6`; `NHitDedx >= 5`; `RadLength >= 5.0`;
  `|DcaPrime / cosh(Eta)| < 2.0`; `IsFromPrimary == 1`;
  `(IsCentralTrk == 1) | (TrackType == 1) | (TrackType == 2)`; `isfinite(xF_SIDIS_es) & xF_SIDIS_es < 0`.
- Cone: alpha = arccos of (Px,Py,Pz)·(scatEX,scatEY,scatEZ)/(|p||ê|); keep alpha finite and
  `>= 0.2` rad (nominal).
- Observables: (pT_h in CM, z) via `compute_pth_cm_and_z` with `BeamElecE` (fallback 27.6) and
  `BeamProtP` (fallback 920); require `isfinite(z) & 0 <= z <= 1`.
- Grid membership: fine reco index `cell*70 + iz*10 + ipt` per charge, cells from (Q2es, Xes)
  over the 7 cells `S0_x0, S0_x1, S1_x0, S1_x1, S2_x0, S2_x1, S3_x0`; `iz` from
  `Z_EDGES_RECO = [0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` (7 bins); `ipt` from
  `PTH_EDGES_RECO = [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0]` (10 bins);
  in-range = index ∈ [0, 490). (The release 490 grid merges pT pairs → release pT edges
  [0,0.5), [0.5,1.0), [1.0,2.0), [2.0,4.0), [4.0,8.0).)
- Data rows: identical selection minus MC-only classifiers. MC weights: `weight1*weight2 ×
  combined_scale(generator)`; data weight 1.

Binmap (authoritative): `$PKG/output_v6/studies/electron_cone_alpha0p2_z005_20260623/
migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json`.

---

## 4. Template-variation refit set (no builder change)

All variations read stored branches — verified present in the v6 tree schema: `MatchAngle`,
`MatchTier`, `VetoAncestorPDG`, `IsMatchedK0LDaughter`, `IsMatchedElectron`,
`IsMatchedV0Daughter`, `RadLength`, `DcaPrime`. Each variation is a full refit of all 8 groups;
output Δμ per group in the variations CSV.

| ID | Variation | What it probes |
|---|---|---|
| V1 | Observable swap: 2D \|DcaPrime\| × RadLength. Binning: Dca 12 bins (8×0.025 over [0,0.20], 2×0.05 (0.20,0.30], 1×(0.30,0.50], overflow (0.50,∞)) × RadLength 5 bins ([5,10), [10,15), [15,25), [25,40), [40,∞) cm; selection floor is RadLength ≥ 5.0 | Shape-choice dependence of μ (weighting of the discriminating tail) |
| V2a | V0 class restricted to short-lived ancestors: `VetoAncestorPDG ∈ {310, 3122, −3122}` | V0 class composition/boundary sensitivity |
| V2b | V0 class extended to include K0L daughters (`IsMatchedK0Daughter == 1` or `VetoAncestorPDG == 130`) | same |
| V3 | Tier-2 tighten: reclassify `MatchTier == 2 AND MatchAngle > 1.0°` tracks from their truth class into kept (data unchanged). Smoke-step check: unique(`MatchAngle` | `MatchTier==2`) must be ≤ 2.0° (the Tier-2 fallback constant; Tier-0 thresholds are H1REC-internal and NOT tunable; Tier-2 content ≈ 0.134% of matched, design-stage) | Marginal-match contamination of the e/V0 classes |
| V4a/V4b | Cone-radius rescan: `ELECTRON_ALPHA_VETO_RAD` = 0.15 / 0.25 (nominal 0.2), full re-projection of data AND templates, refit. Motivation (design-stage): in-cone data/MC excess ≈ 2.1× after signal subtraction; official in-cone counts 19,024 in-grid / 28,582 in-PS (vectors JSON IV-2) | Cone-boundary modeling of the e class |

**Generator swap (X1_rg / X1_dj) — CROSS-CHECK ONLY.** Refit with RG31-only and DJ14-only
templates. Verdict: **double-counts `bgsub_model_rel`** (that component is built from the RG↔DJ
half-spread of B′), so Δμ is reported with `folded=false` and is never folded.

**MC template statistical noise — not a variation.** Covered in-fit by BB-lite and externally by
`bgsub_stat_rel` (SA-3 distinctness: `ibu_stat_rel` = data-Poisson; `bgsub_stat_rel` =
template-MC variance; the fit σ(μ) is a data-side normalization measurement error — the bounded
overlap with `bgsub_stat_rel` is < 0.2% relative and accepted, recorded in the component record).

**Folding rule (house max-rule, pre-authorized).** For each release bin i in group g:

```
model_term_i = max over V ∈ {V1, V2a, V2b, V3, V4a, V4b} of |μ_comb,g^V − μ_comb,g^nom| × f_i
bgsub_model_rel_new,i = max( bgsub_model_rel_old,i , model_term_i )     # NO quadrature add
```

with f_i = (e_avg,i + v0_avg,i)/data_sub_prime,i. Record per bin which side won and the fraction
of updated bins; if the update moves the budget medians by > 0.1 pp, flag in the report.
**Caution:** the max-rule updates the BUDGET column `bgsub_model_rel` (relative, through-chain
component). Do NOT touch the vector CSV's `bgsub_model_term` (a RAW-level per-bin term in the
released vectors file, which stays read-only).

---

## 5. Decision rules (pre-registered)

Definitions per group g: μ_comb,g and σ(μ_comb,g) per Section 2; `z_g = |μ_comb,g − 1| / σ(μ_comb,g)`.

- **R1 (2σ rule).** If ANY group has z_g > 2 → the campaign STOPS before budget reassembly and
  escalates to the user. User decision point:
  - **(A) Central re-derivation**: `B′_i ← e_avg,i·μ_e,g(i) + v0_avg,i·μ_V0,g(i) + lowq2,i` per
    release bin, data_sub′ recomputed; the official re-unfold and budget re-derivation then run
    through the promotion machinery as a SEPARATELY authorized follow-up (not executed by this
    campaign's implementer). Afterwards `bgsub_fitnorm_rel,i = σ(μ_comb,g(i)) × f_i` with the NEW
    data_sub′.
  - **(B) Keep central (μ = 1)**: the default component formula below (envelope style).
  - Decision-support number: the per-bin coherent shift is `Δ_i = |μ_g − 1| · f_i`; integrated it
    is ≈ `|μ_comb − 1| × 7.84%` — the e+v0 share of data_sub′ (e 4.16% + V0 3.69%; 7.85% is the
    B′/data_sub′ figure including the fixed lowQ2, which is NOT shifted).
- **R2 (default component).**
  `bgsub_fitnorm_rel,i = sqrt( (μ_comb,g(i) − 1)² + σ²(μ_comb,g(i)) ) × (e_avg,i + v0_avg,i) / data_sub_prime,i`
  with e_avg, v0_avg, data_sub_prime from the released 490 CSV (RAW reco level, cone+S2′+Set-R′
  basis). lowQ2 is NOT multiplied by μ. A species-resolved variant
  `sqrt( (e_i(μ_e−1))² + (e_i σ_e)² + (v0_i(μ_V0−1))² + (v0_i σ_V0)² ) / (e_i + v0_i)`
  (no cross-covariance) is recorded for audit only; report its max relative deviation from R2
  per group. R2 stands unless R1 fires and the user requests otherwise.
- **R3 (empty/sparse bins).** Bins with `data_sub_prime,i ≤ 0` get component 0 (house release
  convention f = 0 on empty-D bins). The three known bins: (pos, 109), (pos, 179), (neg, 240).
  Flag any bin with f_i > 0.5 (none currently; measured max f = 28.39% pos / 29.93% neg).
- **R4 (>100% check).** After 20-comp reassembly, bins with `total_with_bgsub20_rel > 1.0` must
  remain 0 (current: 0; max total 73.65% pos / 86.99% neg). Any excess → STOP, report, no clipping.
- **R5 (floor/plausibility).** If the per-charge median of `bgsub_fitnorm_rel` is < 0.01%
  (degenerate fit) or > 5% (pathological) → flag for user review before reassembly. Expected
  scale on the default path: σ(μ_comb) 0.9–3.6% × f median 3.17%/3.56% → component medians
  O(0.03–0.13%), up to O(0.2–0.9%) if |μ−1| ≈ σ.
- **R6 (convergence/GOF).** Any group failing convergence after the Section-2 merge fallback, or
  post-fit p-value < 0.001 → STOP and escalate with that group's residual distribution (shape
  mis-modeling can bias μ).

Barlow / nested-subset toy machinery is NOT part of this campaign (tightened-selection stability
test not authorized). The `official_reunfold` SA-3 toy lineage
(`$PKG/scripts/unfolding/cone_s2prime_2026-10-11/.../ibu_toy_covariance_v4.py` ancestry,
`run_record_sub.json`) is referenced only for a potential R1-branch-A re-derivation, which is
separately authorized work.

---

## 6. Outputs

**Script directory (create):** `$PKG/scripts/bgsub_fitnorm_2026-10-19/`

- `bgsub_fitnorm_project.py` — tree → per-group class histograms (npz). Args: `--source
  {data,rapgap31,djangoh14,lowq2_7091}`, `--input-list|--input-root`, `--output-npz`, `--smoke`.
- `bgsub_fitnorm_fit.py` — merged histograms → fit results. Args: `--hist-dir`, `--output-csv`,
  `--output-json`, `--variation {nominal,V1,V2a,V2b,V3,V4a,V4b,X1_rg,X1_dj}`.
- `merge_fitnorm_results.py`, `apply_fitnorm_to_budget.py`, `run_fitnorm_smoke.sh`,
  `run_fitnorm_condor.sh`, `submit_fitnorm.condor`.

**Output tree (create):** `$PKG/output_v6/studies/bgsub_fitnorm_2026-10-19/` with
`fitinputs/` (per-shard npz + `fitinputs_validation.json`), `fits/` (`fitnorm_results.csv`,
`fitnorm_results.json`), `variations/` (`fitnorm_variations.csv`), `budget/`, `logs/`.

**`fitnorm_results.csv`** — one row per (charge, stratum), columns:
`schema_version (=bgsub_fitnorm_results_2026-10-19_v1), charge (h+/h-), q2_stratum (S0..S3),
q2_lo, q2_hi, n_tracks_data, n_bins, E_pred, V_pred, L_pred, K_pred, e_v0_overlap, N_kept_fit,
mu_e, mu_e_err, mu_v0, mu_v0_err, corr_mu_e_mu_v0, mu_comb, mu_comb_err, z_score, chi2, ndf,
p_value, converged, n_bb_gamma, min_neff_bin, merged, flags`.

**`fitnorm_variations.csv`** — one row per (variation, charge, stratum), columns:
`variation, charge, q2_stratum, mu_e, mu_e_err, mu_v0, mu_v0_err, mu_comb,
delta_mu_comb_vs_nom, folded, note`.

**Budget candidate:** `budget/bgsub_setrprime_budget20_cleandenom.csv` — all 19-comp columns
bit-carried except `bgsub_model_rel` (possibly max-rule updated); new columns `bgsub_fitnorm_rel`,
`total_with_bgsub20_rel`, `total_with_bgsub20_percent`, `total_bgsub20_increment_rel`,
`total_bgsub20_increment_percent_points`. `total_with_bgsub20_rel` is recomputed from the full
component set (never by patching totals). Plus `fitnorm_component_record.json` (per-bin f_i, μ, σ,
R1–R6 outcomes, max-rule bookkeeping, overlap notes), `README`, `provenance.json` (sha256 of all
inputs). The candidate stays under `studies/`; installation to `data_release/` is a user-gated
promotion.

---

## 7. Implementer checklist (ordered)

1. **Write the drivers.** Import the producer module; no re-typed selection constants; run its
   provenance verifier + canonical self-check at start. Assert the binmap loads with NB_R=490,
   NB_G=245 and 7 cell labels.
2. **Login-node smoke (< 5 min each, single file).**
   - Data: `$PKG/.../sidistree/data/em0405/sidistree_em0405_d05_0000.root` — projection runs,
     all 8 group histograms fill, selected count > 0, max |DcaPrime| ≤ 5.2, no NaNs.
   - MC: `$PKG/.../sidistree/mc/rapgap31/7096/sidistree_rapgap31_7096_em0405_0000.root` —
     classes fill; unique(`MatchAngle` | `MatchTier==2`) ≤ 2.0°; e∩V0 overlap counter exercised.
   - Micro-fit of (h+, S0) on the smoke histograms — code-path/convergence check only.
   - Smoke outputs under `fitinputs/smoke/` with `SMOKE_` in filenames (house convention).
3. **Condor production projection.** `submit_fitnorm.condor`, shards of ~25 files: data 658 → 27
   jobs; rapgap31 364 → 15; djangoh14 501 → 21; `merged_7091.root` → 1 dedicated job
   (`+RequestRuntime = 10800`; if it exceeds 3 h, split by entry ranges into 4–8 jobs). Wrapper
   mirrors the proven template: `unset PYTHONHOME; unset PYTHONPATH; exec /usr/bin/python3
   <driver> "$@"` (cluster 5379695 lesson; `getenv = True` in the .sub is acceptable ONLY with
   this wrapper). No `JobBatchName`-based duplicate guards (absent from classads); query cluster
   IDs directly.
4. **Merge + projection validation gates** (vs released artifacts, same trees ⇒ near-machine
   precision expected):
   - Per group: projected data track count == Σ `D_cone` over the group's release bins,
     **exact integer** (hard gate; any mismatch = selection-mirror bug).
   - Per group: |E_proj − 2·Σ e_avg(490)| / (2·Σ e_avg) < 1e-6;
     |V_proj + overlap_g − 2·Σ v0_avg(490)| / (2·Σ v0_avg) < 1e-6;
     L_proj pinned to Σ lowq2 (record deviation, must be 0 by construction).
5. **Nominal fits.** 8 groups, BB-lite, covariance, GOF (R6 guards).
6. **Variation refits.** V1, V2a, V2b, V3, V4a, V4b + X1_rg, X1_dj cross-checks.
7. **Interpretation.** Apply R1–R6; stop-or-proceed.
8. **Budget reassembly to 20 components** against the 19-comp cleandenom CSV, then validate:
   - **A1 (bit-identity anchor)**: inject μ=1, σ=0 → component ≡ 0 and
     `total_with_bgsub20_rel` string-identical to `total_with_bgsub19_rel`; medians
     **5.9764% (pos) / 6.2968% (neg)**; 0 bins > 100%.
   - **A2 (μ=1.25 synthetic anchor)**: inject μ_comb = 1.25, σ = 0 for all groups → component
     medians **0.7896% (pos) / 0.8909% (neg)**; total20 medians **6.0483% (pos) / 6.3096% (neg)**;
     0 bins > 100%. (Verified against the released CSVs on 2026-10-19; the design memo's
     6.0485/6.3102 and 0.798/0.893 differ at rounding level — the CSV-verified values here are
     authoritative.)
   - **A3 (carry check)**: with the max-rule a no-op, the 4-comp bgsub quadrature per bin has
     medians **0.4445% (pos) / 0.4604% (neg)**.
9. **Report** the per-group μ table, R1 outcome, and dominant-source statement to the user.
   Do NOT install anything to `data_release/`.

---

## 8. Constraints

- **L_data = 351000.0 nb⁻¹ only** (four-period S67 total). The released vectors use
  generator-total combined scaling; never per-period normalization in this campaign.
- **MC scales (combined):** rapgap31 `0.03843251824641288` = 351000/9132891; djangoh14
  `0.03346843073584302` = 351000/10487495; lowQ2 source (djangoh14/7091, `merged_7091.root`)
  net `0.08013817556653238` = 351000/4379935. **Forbidden:** `22.1963651` and `15813.4`
  (belong to djangoh14/7110 DjBkgd — wrong sample); `0.08141970143392539` (superseded
  356613.0 nb⁻¹ direct-monitor normalization — already baked into packaged npz, never re-apply);
  data-lumi values 297657.8 / 356613.0.
- **No builder change.** All variations read stored branches (verified present, Section 4). The
  V0 ancestor-walk depth-6 knob is NOT accessible without a builder fork → out of scope.
- **Response NOT rebuilt.** No unfolding rerun inside this campaign; an unfolding rerun happens
  only under R1-branch-A with separate user authorization.
- **Read-only inputs.** Trees, vector CSVs/JSON, budget CSV, and the npz bank are never modified.
- **Level discipline.** Every released-value quote states level and basis. Fit inputs and the
  `bgsub_fitnorm_rel` construction are at RAW reconstructed level (cone+S2′+Set-R′ selection,
  pre-unfold Set-R′ subtraction). Budget totals are relative uncertainties on the RAW-level
  official_reunfold IBU k=4 result on data_sub′. Born-level values (h+ 11.996558 / h− 11.663956 nb,
  Born triad) are not modified by this campaign.
- **Environment.** Login node: ≤ 5 min, single file, interactive checks only; everything else on
  Condor. Wrapper unsets PYTHONHOME/PYTHONPATH and execs `/usr/bin/python3`; never `getenv = True`
  without that wrapper; no `JobBatchName` guards; `regexp()` does not evaluate in this pool's
  classad engine (use `stringListMember` if needed).
- **Carried caveats inherited unchanged:** e∩V0 overlap ignored (0.0029% of D_cone; user decision
  2026-10-08); lowQ2 alpha0p1 radius mismatch (bounded << 0.01%); DjBkgd exact-exposure
  provenance open (≤ 0.074% of B_R′); lumi 2.7% flat never rescaled; NucliaWeight excluded from
  nominal weights (producer convention).
