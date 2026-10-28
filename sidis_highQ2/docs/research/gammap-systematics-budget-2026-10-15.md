# Systematic-error budget specification for the derived sigma(gamma\*p) release product (2026-10-15)

> **USER DECISION 2026-10-16: this γ*p program is OPTIONAL auxiliary material — reporting level is the ep cross section; the pending items below (§10 rebuild, §11 M2 fold, builder phases) are NOT blockers and NOT required. See `docs/methodology/ep_reporting_level_user_decision_2026-10-16.md`.**


**Author:** h1-systematics (embedded run; the `sidis-hq2-systematics` skill is not loadable in this runtime — precedent `docs/research/gammap-electroweak-level-2026-10-15.md` header; STATE.md, the canonical budget release, and the authoritative 2026-10-15 design memos were read directly).
**Type:** READ-ONLY design specification. No release, table, budget, plot, script, or note file was modified; no production was run; the 2026-10-13 cone triad and the 2026-10-14 leg-envelope budget are untouched. The only file created by this task is this memo.
**Amended later 2026-10-15:** §10 (QA decision request on the built release) appended after read-only adjudication of `gammap_hand_cone_s2prime_photonborn_2026-10-15`; supersession pointers added at §3.2 items 13′/17 and §3.4. No release artifact was modified.
**Amended 2026-10-16:** §11 (M2 landed: final measured flux_model_rel fold + builder specification) appended; supersession pointers added at §3.2 item 16, §10.4, §10.5 item 5. Still no release artifact modified.
**Product specified:** the derived virtual-photon–proton cross section `sigma_gammap(bin) = RAW(bin) x C_gamma_born(bin) x <1/Gamma>(bin)` (Hand/H1 flux, arXiv:0910.5831 Eq. 10; `(y,Q2,z,pT)` measure; binned nb·GeV²; post-unfold derived sidecar, Option B of the touchpoint map — no re-unfolding).
**Parent basis (verified this session):** cone triad of 2026-10-13/14 — RAW `.../data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ 11.412233 / h− 11.123626 nb), Born `.../data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/` (13.026987 / 12.697511 nb), canonical budget `.../data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/` (`total_with_cone15_rel` medians h+ 5.8032 % / h− 6.1919 %). Photon-Born level on this basis: 11.875722 / 11.571280 nb; γ\*p prototype totals 1 170 035 / 1 129 842 nb·GeV² (single-file flux, ≲1 % full-pass shift expected). L = 351000.0 nb⁻¹, flat `lumi_rel` 2.7 %.

---

## 0. Verdict summary

1. **Of the existing 15 budget components, 13 carry UNCHANGED (bit-identical relative values), 2 are INVALIDATED as constructed and are SWAP-replaced** (`c_rad_gen_rel` → `c_gamma_gen_rel` κ-proxy; `c_rad_stat_rel` → `c_gamma_stat_rel` NoZ numerator stat); none is merely relabeled; none is dropped. One NEW component is required (`flux_model_rel`), plus one conditional component (`flux_stat_rel`, gate-decided, expected subdominant).
2. **The per-bin multiplicative conversion cancels in every relative leg** (exact identity for ratios at a common level), so the 13 carried legs need no re-derivation; the experimental covariance is carried by exact diagonal congruence `Cov_γp[i][j] = f_i·Cov_ep[i][j]·f_j`, `f_i = C_gamma_born(i)/(L_data·Γ_i)` — correlation matrix unchanged, no toy rerun.
3. **Combination rules:** the two radiative-side replacements are **SWAPs, never additions** (shared content: the κ-proxy's first term *is* the WithZ emfix spread; the RAD-mixed denominator stat is common to both correction factors). `flux_model_rel` is **added in quadrature** to the swapped legs (distinct object: the in-bin y-population model vs the EW projection factor).
4. **Projected medians** (median-level quadrature model; exact per-bin recomputation is the builder's job): swaps only 5.8176 % / 6.2043 % (+0.014 / +0.012 pp — consistent with the radiative reviewer's ≲ +0.05 pp); with the interim per-cell flux leg ≈ 5.89–5.90 % / 6.27–6.28 %; with the conservative flat 1.75 % flux bound 6.0751 % / 6.4464 % (+0.27 / +0.25 pp).
5. **PYTHIA8/SHERPA3 conversion status:** labeled caveat + theory-side band on the MC curves only; **never** an experimental uncertainty leg.
6. **One Condor campaign** (one accumulator extension of the existing worker; DJ14 NoZ Born 260-file pass + RG31 RAD emfix pass) makes the budget final; the SWAP legs need no new measurement.
7. **Defensible today as a labelled PROVISIONAL-DERIVED release: YES** — with the interim flux bound, the inherited provisional-status caveats, and the extended never-mix quote discipline.

---

## 1. Q1 — The existing 15 components: exact enumeration and disposition

**Authoritative source of the names** (read this session): the canonical budget release
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/`
— `components_rel_columns` key of `total_uncertainty_summary_cone_variant_15comp.json` (lines 6–22), identically the CSV column headers of `total_uncertainty_summary_cone_variant_15comp.csv` and the `values` keys of `systematics_charged_sidis_highQ2.json` (490 bins = 245 h+ / 245 h−; medians below recomputed read-only from the canonical CSV this session).

| # | Component (exact name) | h+ med % | h− med % | Disposition under γ\*p | Reasoning |
|---|---|---|---|---|---|
| 1 | `ibu_stat_rel` | 3.6314 | 4.0114 | **UNCHANGED** | Data statistical (1000-toy IBU diagonal). The per-bin conversion factor `f(b)` multiplies nominal and absolute uncertainty alike; the relative leg is invariant. |
| 2 | `ibu_model_rel` | 1.3623 | 1.4304 | **UNCHANGED** | Response-model leg (B1-floored). Ratio of varied-to-nominal at a common level; `f(b)` cancels exactly. |
| 3 | `ibu_reg_rel` | 0.5140 | 0.5470 | **UNCHANGED** | Regularization (k-choice) leg; same cancellation. |
| 4 | `hfs_jes_rel` | 0.3038 | 0.3444 | **UNCHANGED** | Single HFS nuisance, whole-vector leg; relative. |
| 5 | `hfs_rces_rel` | 0.2432 | 0.2463 | **UNCHANGED** | Same. |
| 6 | `hfs_theta_rel` | 0.2398 | 0.2795 | **UNCHANGED** | Same. |
| 7 | `elec_energy_rel` | 1.4820 | 1.6971 | **UNCHANGED** | Leg-envelope elec-E column (construction (c), 2026-10-14); relative; the electron scale acts upstream of the conversion and is level-agnostic. |
| 8 | `elec_theta_rel` | 0.4382 | 0.5135 | **UNCHANGED** | Same. |
| 9 | `lumi_rel` | 2.7000 | 2.7000 | **UNCHANGED** | `L_data` enters only `RAW = N_Unf/L_data`; `C_gamma_born` (MC ratio) and `<1/Gamma>` (normalized weighted mean) carry no `L_data`. The 2.7 % propagates 1:1; only the absolute expression changes units (nb → nb·GeV²). Remains the flat fully correlated leg, reported separately in publication-style tables. |
| 10 | `tracking_eff_m0p5_rel` | 0.4937 | 0.4984 | **UNCHANGED** | Single tracking nuisance, data-side conservative; relative. |
| 11 | `phi_h_gauss_rel` | 1.0522 | 1.0550 | **UNCHANGED** | φ_h model leg (floored Gaussian envelope); relative. |
| 12 | `c_rad_gen_rel` | 0.5844 | 0.5804 | **INVALIDATED → SWAP** | The leg's referent is the generator-scheme uncertainty of the **applied factor**. Under γ\*p the applied factor is `C_gamma_born` (DJANGOH-only), not `C_rad`; the WithZ DJ/RG releasebin spread is no longer the right model leg. Replaced by `c_gamma_gen_rel` = the κ-proxy (§3). Never additive — the κ-proxy's first term *is* this leg's content. |
| 13 | `c_rad_stat_rel` | 0.4272 | 0.4429 | **INVALIDATED → SWAP** | Same referent argument: the statistical uncertainty of the applied factor is now that of `C_gamma_born`, dominated by the NoZ numerator (`relative_stat_unc` median 0.551 %, max 7.84 %). Replaced by `c_gamma_stat_rel` (§3). Never additive — the RAD-mixed denominator stat is common to both factors. |
| 14 | `detint_removal_rel` | 0.0386 | 0.0340 | **UNCHANGED** | Response-internal det.int. removal leg (model/stat subcolumns carried); relative, response-side, level-agnostic. |
| 15 | `fakecol_mcstat_rel` | 0.3443 | 0.3495 | **UNCHANGED** | Fake-column MC statistics; relative. |

**Counts: 13 unchanged / 0 merely relabeled / 2 invalidated (SWAP-replaced) / 0 dropped.** Current totals for anchoring: `total_with_cone15_rel` medians 5.8032 % / 6.1919 %, max 80.396 % / 84.499 %; dominant at median `ibu_stat_rel` both charges; 4 bins > 50 %.

Two descriptive (non-component) budget columns do change **scale and level labels**: `nominal_cross_section_nb` and the `tracking_eff_m0p5_abs_nb` / tracking-source absolute columns must be rescaled by `f(b)` in the γ\*p budget sidecar (relative columns byte-identical) — the touchpoint map §5 "optional derived sidecar only if absolute-nb budget columns are wanted in gamma\*p units" is hereby specified as REQUIRED for the sidecar, since the budget contract carries `nominal_cross_section_nb`.

---

## 2. Q2 — Correlation structure: how C_rad, F_EWgamma, and `<1/Gamma>` enter

### 2.1 Roles of the three factors

- **`C_rad(b) = sigma_nonrad_mixed(b)/sigma_rad_mixed(b)`** — ep-level QED radiative-distortion ratio; no flux content (verified: `docs/radiative/note_crad_vs_gammaflux_and_R_2026-10-15.md` §1). Under γ\*p it enters only through the composed factor.
- **`F_EWgamma(b) = sigma_gamma_born(b)/sigma_nonrad_mixed(b)`** — EW projection: removes Z exchange, γ–Z interference, and the full one-loop content (of the integrated −8.8 %: ≈ −0.35 % Z, ≈ −7.9 % loop; per-cell (1−F) grows 0.080 → 0.153 with Q²). `C_gamma_born = C_rad × F_EWgamma` hard-gated (G4, 1e-9).
- **`<1/Gamma>(b)`** — per-bin effective Hand flux, cross-section-weighted over the NoZ photon-Born e⁻ gen population with the mirrored CγB selection. Because Γ varies by 8.9×–137× inside the cells (in-bin spread > 10 % in effectively all 490 bins), `<1/Gamma>` is a **model-weighted average**: its model dependence is precisely the new systematic `flux_model_rel`.

All three are per-bin **diagonal multiplicative** factors, charge-blind at the event level (Γ depends only on (y,Q²); photon Born is exactly charge-symmetric at tree level; h± `F_EWgamma` integrated difference ≤ 0.04 %).

### 2.2 Correlation classes (nuisance-driven; cell structure lives in magnitude, not correlation)

| Leg | Across bins | Across charges | Across (Q²,x) cells |
|---|---|---|---|
| `lumi_rel` | 100 % (flat 2.7 %) | 100 % (one `L_data`) | 100 % |
| `elec_energy_rel`, `elec_theta_rel`, `hfs_jes/rces/theta_rel`, `tracking_eff_m0p5_rel` | 100 % (single nuisance each; whole-vector leg convention preserves the per-bin correlation of one chain) | 100 % (charge-common detector/tracking nuisance; per-charge propagation) | 100 % |
| `ibu_model_rel`, `ibu_reg_rel`, `phi_h_gauss_rel`, `detint_removal_rel` (model subcolumn) | 100 % (one model/k choice each) | 100 % | 100 % |
| `c_gamma_gen_rel` (κ-proxy; SWAP successor of `c_rad_gen_rel`) | 100 % (one generator-scheme decision) | 100 % (EW factor charge-blind to ≤0.04 %) | 100 % — the κ-term grows with Q² via (1−F): cell-structured **magnitude**, single decision |
| `flux_model_rel` (NEW) | 100 % (one flux-weighting-model decision; whole-vector) | 100 % (flux is (y,Q²)-only; both charges share the same beam mixture and DIS population; residual = hadron-population weighting, second order, gated in Phase 1) | 100 % — worst in S3 (137× in-bin Γ spread) in **magnitude** |
| `ibu_stat_rel` | 0 (binwise diagonal; the migration off-diagonals live in the 1000-toy covariance sidecar, carried by congruence) | 0 | 0 |
| `c_gamma_stat_rel` (NoZ numerator stat; SWAP successor of `c_rad_stat_rel`) | 0 (MC Poisson per bin) | ≈0 (disjoint per-charge hadron pools over shared events; treat as uncorrelated — conservative) | 0 |
| `fakecol_mcstat_rel`, `detint_removal_stat` subcolumn | 0 | ≈0 | 0 |
| `flux_stat_rel` (conditional, if carried) | 0 | ≈0 | 0 |

**Mixed-beam note:** the RAD-mixed denominator is shared between the h+ and h− `C_gamma_born` columns (same MC events, disjoint hadron pools); the charge-antisymmetric γZ content is diluted by (f_em − f_ep) ≈ 0.028 to ≲0.01 % and vanishes as a physics concern at the photon target (exact tree-level charge symmetry) — the "mixed-beam-only" scope limitation becomes a scope statement, not a systematics class.

### 2.3 Diagonal congruence × per-bin multiplicative flux leg

- **Experimental (toy) covariance:** exact diagonal congruence from the yield-unit sidecar, `Cov_γp[i][j] = f_i · Cov_yield[i][j] · f_j` with `f_i = C_gamma_born(i)/(L_data · Γ_i)` (one-step extension of the existing `born_covariance_recipe` key). A diagonal congruence preserves the correlation matrix; **no toy re-derivation, no re-solve**. The per-bin factor cancels in every relative leg, which is why the 13 carried components are bit-identical.
- **The flux systematic is not a rescaling of the toy covariance** — it is an uncertainty **on the diagonal factor itself**. In any full systematics covariance its contribution is the outer product `ΔCov[i][j] = u_flux_i · u_flux_j · σ_i σ_j` (fully correlated construction), exactly the rank-1 scheme-leg pattern the existing `covariance_cradstat/` sidecars implement; the conditional flux **statistical** leg is diagonal (binwise). Practical implementation: the derived budget sidecar stores relative legs (13 carried + 2 swapped + 1 new) and the covariance sidecar documents the extended recipe — nothing else changes.
- **Do not** re-derive the 13 carried legs "on the γ\*p basis": the recorded table-flip invariance (relative products invariant under any per-bin table flip; the cone promotion re-derived relative products only because the *observable basis* changed, which a flux division does not) governs. The γ\*p product is a level/units change, not an observable-basis change.

---

## 3. Q3 — The new γ\*p budget: components, formulas, combination rules

### 3.1 Definition and carried chain

```
sigma_gammap(b) = RAW(b) x C_gamma_born(b) x G(b),   G(b) = <1/Gamma>(b)
C_gamma_born(b) = C_rad(b) x F_EWgamma(b)            [hard-gated G4, 1e-9]
```

Sequential total chain (mirrors the parent op-for-op; first three stages byte-identical):

```
T9  = total_quadrature_rel        = sqrt(sum of legs 1-9)                    [unchanged]
T10 = total_with_tracking_rel     = sqrt(T9^2  + tracking_eff_m0p5_rel^2)    [unchanged]
T11 = total_with_phih_rel         = sqrt(T10^2 + phi_h_gauss_rel^2)          [unchanged]
T13'= total_with_photon13_rel     = sqrt(T11^2 + c_gamma_gen_rel^2 + c_gamma_stat_rel^2)   [SWAP pair]
T15'= total_with_cone15'_rel      = sqrt(T13'^2 + detint_removal_rel^2 + fakecol_mcstat_rel^2)
T16 = total_with_gammap16_rel     = sqrt(T15'^2 + flux_model_rel^2)          [NEW total]
      (+ flux_stat_rel^2 inside T16 if the conditional leg is carried)
```

### 3.2 Component list (16 + 1 conditional)

**Carried bit-identical (13):** `ibu_stat_rel`, `ibu_model_rel`, `ibu_reg_rel`, `hfs_jes_rel`, `hfs_rces_rel`, `hfs_theta_rel`, `elec_energy_rel`, `elec_theta_rel`, `lumi_rel`, `tracking_eff_m0p5_rel`, `phi_h_gauss_rel`, `detint_removal_rel` (with `detint_removal_model_rel`/`detint_removal_stat_rel` subcolumns), `fakecol_mcstat_rel`.

**Swapped (2):**

12′. **`c_gamma_gen_rel`(b) = sqrt( v3_emfix_rel(b)² + [κ·(1 − F_EWgamma(b))]² )**, κ = 0.02 central (κ = 0.03 conservative variant retained).
    Source: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_gen_unc_proxy_v3emfix_490rows.csv` (+ validation JSON). Expected magnitudes: medians h+ 0.6223 % / h− 0.6189 %; max 5.77 % / 5.67 %; κ-term alone median 0.185 % (κ=0.02) / 0.278 % (κ=0.03), per-cell mean 0.16 % (Q² 150–300) rising to 0.31 % / 0.46 % (cell 6). **SWAP for `c_rad_gen_rel`** (WithZ emfix medians 0.5844 % / 0.5804 %).

13′. **`c_gamma_stat_rel`(b) = NoZ-numerator per-bin `relative_stat_unc`(b)**. *(CORRECTED 2026-10-15, §10.2: the source column is the FULL per-bin ratio stat `C_gamma_born_stat_unc/C_gamma_born` (identity verified), numerator-dominated — so the swap's coverage is exact, not numerator-only; the "NoZ-numerator" phrasing understates the leg.)*
    Source: `cgammaborn_empz_gt45_nominal_490rows.csv` per-bin stat column. Expected magnitudes: median 0.551 %, max 7.84 % (sparse high-Q²/high-pT bins; min count 237, median 50 537 weighted entries). **SWAP for `c_rad_stat_rel`** (medians 0.4272 % / 0.4429 %; max 4.41 % / 6.04 %).

**New (1):**

16. **`flux_model_rel`(b)** — model dependence of the in-bin (y,Q²)-population weighting of `<1/Gamma>`. Final form (freeze before measurement):

```
flux_model_rel(b) = max( u_level(b), u_gen(b) )        [ENVELOPE — see rules]

u_level(b) = |G_DJ14,RAD-mixed(b) − G_DJ14,NoZ-photonBorn(b)| / G_DJ14,NoZ-photonBorn(b)
u_gen(b)   = |G_RG31,RAD-emfix(b)     − G_DJ14,RAD-mixed(b)|     / G_DJ14,RAD-mixed(b)
```

    - `u_level` is measurable **today** from the existing full-sample y-resolved table `cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` (per-cell values +0.93 / +1.00 / +0.54 / +0.51 / +0.46 / +0.56 / +1.75 %, cells 0–6, worst S3); the per-bin vector is derivable from the same table in the builder (read-only arithmetic, no Condor).
    - `u_gen` **requires the Phase-1 RG31 RAD emfix pass**: no RAPGAP NoZ sample exists and the IPRO=1200 HERACLES interface has no Z-off switch, so the generator comparison must be made at RAD level with the level term disentangling level from generator. **Currently UNEVALUATED — carried as an interim bound, never zero, never silently dropped** (failure-handling rule). *(MEASURED 2026-10-15/16: the RG31 RAD emfix flux pass landed — final fold and formulas in §11; this interim-bound paragraph is superseded.)*
    - **Interim leg (recommended):** the per-bin/per-cell `u_level` vector, documented with the ≤1.75 % cap; **conservative variant:** flat 1.75 % in all 490 bins. Either variant must be flagged `generator_component_unevaluated_interim_bound` in the budget metadata.

**Conditional (1):**

17. **`flux_stat_rel`(b)** — per-bin MC statistics of `<1/Gamma>` (free byproduct of the Phase-1 pass; the weighted-mean variance `sqrt(Σw²(1/Γ − ⟨1/Γ⟩)²)/Σw` per bin). **Carry only if any bin exceeds 0.1 %** (gate at build time); expected subdominant (median ≲0.2 %); binwise uncorrelated. Not in the handed leg list; specified here so the build does not silently omit a measurable stat. *(GATE OUTCOME 2026-10-15, §10.2: FIRED on the landed full-sample M1 pass — medians h+ 0.2653 % / h− 0.2786 %, max 4.1866 % / 7.1646 %; the leg is MANDATORY, not informational.)*

### 3.3 Combination rules for every overlapping pair — and why

| Pair | Rule | Why |
|---|---|---|
| `c_gamma_gen_rel` vs `c_rad_gen_rel` | **SWAP** (never add) | The κ-proxy's first term `v3_emfix_rel` **is** the WithZ emfix spread — the exact content of `c_rad_gen_rel`. Adding would double-count the C_rad generator scheme spread. The κ-term is the *anchored* EW-generator dependence (unmeasurable: no RG NoZ); it is quadrature-summed with the scheme term **inside one leg**, and that leg replaces the old one because the applied factor's referent changed (C_rad → C_gamma_born). |
| `c_gamma_stat_rel` vs `c_rad_stat_rel` | **SWAP** (never add) | Both quantify MC statistics of the *same correction chain* at different levels; the RAD-mixed denominator statistical content is **common** to C_rad and C_gamma_born — addition double-counts it. The NoZ numerator stat dominates at median (0.551 % > 0.4272/0.4429 %) and carries the same sparse-bin structure (max 7.84 %); the swap is conservative at the median. |
| `flux_model_rel` vs `c_gamma_gen_rel` | **ADD in quadrature** | Distinct objects: the in-bin y-population model driving `G(b)` vs the EW projection factor `F_EWgamma`. No shared content; the partial physical correlation (both respond to generator-model differences) is acknowledged and is numerically irrelevant at these magnitudes (≤1.75 % vs ~0.62 %: quadrature vs envelope differs by ≲0.01 pp on the total). |
| `u_level` vs `u_gen` inside `flux_model_rel` | **ENVELOPE (max), not quadrature** | Both probe the **same** object — the in-bin y-population model — along correlated axes (a generator y-shape difference also changes the radiative distortion); the envelope matches the budget's FULL ABS single-sided convention. Must be frozen before the Phase-1 measurement (no post-hoc rule shopping). |
| `flux_model_rel` vs `flux_stat_rel` | **ADD** | Model vs statistics; independent. |
| `flux_model_rel` vs `lumi_rel` | **ADD** in the quadrature total; lumi reported separately in publication tables | Independent sources; `lumi_rel` is the one flat fully correlated leg (2.7 %), and the separate-reporting duty applies at the note/table level exactly as for the parent budget. |
| Flux legs vs the 13 carried legs | **ADD** | No overlap: the carried legs are unfolding/detector/normalization uncertainties at a common level; the conversion factor cancels in all of them. |

**Items that are explicitly NOT legs (never budgeted):**

- **Flux-convention differences** (Hand vs Gilman/Williams vs Trento-exact): ≤0.03 % over the whole phase space (γ² ≤ 2.6×10⁻⁴, Q²/ν² ≤ 2.6×10⁻⁴, |ε_H1 − ε_Trento| ≤ 5×10⁻⁶) — documented once in the flux-table README.
- **y/bin-center convention:** the analytic bin-center route is quantified out (−14 %…+72 % bias, cell-dependent); the MC-integrated effective flux is mandatory, and its residual model dependence **is** `flux_model_rel` — no separate leg.
- **α(0)-pure-Born vs dressed NOZ1** (≈ +8.6–9 % integrated, NOZ1/NOZ = 7.2859/6.7086 nb): a **convention substitution**, recorded as a decision (pure photon Born at fixed α(0), consistent with the α(0)-based Hand flux), never a systematic.
- **R = σ_L/σ_T:** drops out of the central value entirely under the ε-weighted-total convention σ_γ\*p = σ_T + ε̄σ_L; statement duty only.
- **Per-beam / per-hadron-charge photon legs:** NONE at the photon target — exact tree-level charge symmetry (NoZ e± = 218.84216 pb both); mixed γZ residue ≈ 0.007 %; covered by the κ anchor.

### 3.4 Projected totals (median-level quadrature model; the exact per-bin recomputation is the Phase-2 builder's job — the legenv precedent showed a median-level model can deviate O(0.1–0.2 pp) from the exact replacement, so these are design estimates, not quotes)

| Scenario | h+ median | h− median |
|---|---|---|
| Current legenv budget (anchor) | 5.8032 % | 6.1919 % |
| + two SWAPs only | 5.8176 % (+0.014 pp) | 6.2043 % (+0.012 pp) |
| + SWAPs + interim per-cell flux (median bin in S0, ~0.93–1.0 %) | ≈ 5.89–5.90 % | ≈ 6.27–6.28 % |
| + SWAPs + conservative flat 1.75 % flux | 6.0751 % (+0.27 pp) | 6.4464 % (+0.25 pp) |

The swap-only increment is inside the radiative reviewer's ≲ +0.05 pp estimate (the max bins grow a little more via the NoZ-stat max 7.84 %, landing in the same sparse bins already > 50 % through `ibu_stat_rel`). Validation guidance is preserved: total remains comparable to stat (total/stat ≈ 1.6), dominant component at median remains `ibu_stat_rel` (not luminosity), and the > 50 % instability bin set is expected to be the same 4 bins — the build-time instability investigation must verify bin-set equality.

*(SUPERSEDED 2026-10-15 by the §10.2 per-bin design estimates, which also supersede the §0.4 projections: swaps only 5.8151 / 6.2122 %; swaps + per-cell `flux_model_rel` + gated `flux_stat_rel` 5.8558 / 6.2857 %; flat-1.75 variant 6.0813 / 6.4605 %. The "median bin in S0" assumption was wrong for h+ — the h+ median-total bin is S3_x0, the worst flux cell (u_level +1.75 %); h− is S0_x0 (+0.93 %) — and the estimates now include the fired `flux_stat_rel` gate. The exact per-bin recomputation remains the builder's job; deviations from the §10.2 anchors are REPORT items.)*

---

## 4. Q4 — How to represent the approximate PYTHIA8.317 / SHERPA3.0 conversions

**Decision: a labeled caveat, plus a theory-side band rendered only on the MC curves. Never an explicit experimental uncertainty.**

1. **Mandatory labeled caveat** on every γ\*p-level render or table that includes the PYTHIA8.317 / SHERPA3.0 curves: "conversion approximate — the DJANGOH14-derived `F_EWgamma × <1/Gamma>` has been applied; the generators' internal α_EM / loop conventions are generator-internal and not audited; Z-content unverified" (touchpoint-map risk R12). Until the generator EW-mode audit (Z content + α convention) passes, these two curves are either excluded from γ\*p-level ratio statements or shown caveat-only.
2. **Theory-side band (when shown):** rendered **only on the MC curves** — never on the data points, never in the data budget. Minimum width: the conservative κ = 0.03 EW-convention term `[κ(1−F_EWgamma)]` (medians 0.278 %, max 0.646 %), widened if the generator audit quantifies a larger mismatch. Label verbatim: "theory-side convention band — not included in the experimental uncertainties".
3. **Why not an explicit uncertainty:** the mismatch is a property of the *prediction's level statement*, not of the measurement. Leaking it into the experimental budget would (i) misattribute theory-side convention risk to the data, (ii) contaminate every downstream consumer (tables, exports, note totals, quadrature sums), and (iii) blur the level-flag discipline the two-convention structure already enforces. RAPGAP31 / DJANGOH14 NORAD take `F_EWgamma` legitimately (HERACLES-family machinery, auditable); MAPTMD24 takes `<1/Gamma>` alone (already photon-only); the caveat is specific to PYTHIA8/SHERPA3.

---

## 5. Q5 — What must be measured on Condor before the budget is final; cheapest job; interim defensibility

**Required measurements (all share one worker extension):**

- **M1 — Full-sample DJ14 NoZ photon-Born flux pass (product-critical):** extend `crad_v3_worker_releasebin.py` (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py`) with one accumulator (`sum_w_invGamma` per global bin, per charge; optionally per-(bin,y) diagnostic — the y-resolved array structure already exists) and run over the 260 `ChargedGenRC` trees under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/`. Emits `gamma_flux_hand_490rows.csv` + per-cell effective-kinematics sidecar (⟨Q²⟩, ⟨y⟩, ⟨x⟩, ε̄) + per-bin flux stat (feeds the `flux_stat_rel` gate) + the count-weighted anti-trap variant. Gates: file-0 cutflow = 777 094 selected entries; 490/490 finite positive; per-cell `<1/Gamma>` within 1 % of the prototype (62 982 / 22 128 / 128 576 / 47 553 / 299 933 / 108 816 / 735 729 GeV²); count-weighted variant rejected by gate (bias up to +12.1 %); charge-split h± difference gate (expected ≪1 %).
- **M2 — RG31 RAD emfix flux pass (the only currently-unevaluated budget leg):** same extended worker over the RG31 RAD emfix sample (the formal-alternative C_rad leg sample) → per-bin `u_gen`. This delivers the measured `flux_model_rel`. It must be a RAD-level comparison because no RAPGAP NoZ sample exists and IPRO=1200 has no Z-off switch; `u_level` (computable today from the existing 2940-row y-resolved CSV) disentangles level from generator.
- **M3 (optional, same campaign):** DJ14 RAD `<1/Gamma>` per-bin pass — refines `u_level` from per-cell to per-bin granularity (per-cell values already available without Condor).

**Cheapest job that delivers the missing measurement:** ONE Condor campaign — a single accumulator extension of the existing worker, two-to-three sample sets (260 DJ NoZ Born files; RG31 RAD emfix set; optionally DJ14 RAD set), reusing the crad campaign submission/merge/finalize machinery. **The budget becomes final when M2 lands** (M1 is required for the product central values in any case); the two SWAP legs require **no** new measurement — they come from the existing NoZ 490-row table and κ-proxy CSV.

**Defensible to publish meanwhile (interim):** the labelled derived sidecar with the 16-component budget — 13 carried legs bit-identical, the two SWAP legs from existing tables, and `flux_model_rel` at the interim bound (per-bin/per-cell `u_level` vector recommended; flat 1.75 % conservative variant) — with the generator component explicitly flagged UNEVALUATED/interim-bound in the metadata. Projected medians ≈ 5.89–5.90 % / 6.27–6.28 % (per-cell interim) or 6.08 % / 6.45 % (flat 1.75 %). **Not defensible meanwhile:** calling the budget final; quoting `flux_model_rel` as measured; or omitting the flux leg on the ground that the effective-flux conversion is an exact identity — the identity is exact, but the weighting model is not, and the in-bin Γ spread (>10 % in essentially all bins, up to 137× in S3) makes the model dependence a genuine ~1 %-class systematic.

---

## 6. Q6 — Impact on the charge asymmetry A_h (if and when released)

A_h is CLOSED in this analysis; this section is conditional. For A_h(b) = (σ⁺−σ⁻)/(σ⁺+σ⁻) at the γ\*p level:

- **Cancel exactly:** `lumi_rel` (single `L_data`); the α(0) convention (global); and the **entire beam-charge/Z class** — at the photon target the charge-antisymmetric γZ term that burdens an ep-Born A_h does not exist (exact tree-level charge symmetry; NoZ e± identical to all printed digits); no per-beam F(i) bookkeeping applies.
- **Cancel to first order (charge-correlated; only the charge-antisymmetric residue survives):** the conversion factor `<1/Gamma>` and its model leg `flux_model_rel` — Γ depends only on (y,Q²), and the two charges share the same beam mixture and the same DIS event population per bin, so both the flux factor and its model shift are charge-common up to the hadron-population-weighting residual (second order; bounded at build time by the Phase-1 charge-split flux gate). Likewise `C_gamma_born` and `c_gamma_gen_rel` (h± F integrated difference ≤ 0.04 %), and the charge-common nuisances (`elec_*`, `hfs_*`, `tracking_eff_m0p5_rel`, `ibu_model_rel`, `ibu_reg_rel`, `phi_h_gauss_rel`, detint model). The A-F3 charge-asymmetry systematic (mandatory before any A_h quote) must evaluate these correlated cancellations explicitly.
- **Do not cancel:** the statistical legs — `ibu_stat_rel` (independent per-charge data statistics), `c_gamma_stat_rel` (disjoint per-charge NoZ numerator hadron pools), `fakecol_mcstat_rel`, `detint_removal_stat`.
- **Net:** a γ\*p-level A_h would be statistics-dominated and structurally cleaner than an ep-Born A_h (the whole beam-charge systematics class vanishes by construction). The flux leg's y-dependence does not break the cancellation because y is a DIS-level variable shared by the two charges — only the hadron-population weighting residual survives, and it is gated, not assumed.

---

## 7. Q7 — Verdict

**Is the γ\*p product defensible as a labelled derived release today? YES**, with this discipline:

- Label **PROVISIONAL-DERIVED**, inheriting verbatim: the NoZ `C_gamma_born` PROVISIONAL DIAGNOSTIC / unapplied parent status (promotion to applied factor of the derived product is a recorded user decision, per the EW memo §3.3), `P-CRAD-LUMI` FILE-SET-RECONCILED (generator-log closure permanently blocked externally — not publication-grade, ships as documented caveat exactly as the ep Born release), the PROVISIONAL releasebin baseline, raw/no-bgsub with response fake-fraction subtraction only, and the absolute-normalization-provisional open item. The γ\*p product must never be presented as more final than its ep parent.
- Budget: 16 components (13 carried + 2 SWAP + `flux_model_rel` interim; `flux_stat_rel` conditional), totals per §3.4.
- Quote discipline: four-axis never-mix rule — prior basis, Born-vs-raw level, observable basis (cone-basis S2′), and now γ\*p level with flux convention (Hand/H1 arXiv:0910.5831 Eq. 10), measure ((y,Q²,z,pT)), EW level (pure photon Born at fixed α(0)), and R-treatment (ε-weighted total; no F_L separation). Binned nb·GeV² form (sidesteps the x_max = 1.5 density split, risk R6). Full-Born ep release remains the primary product; the fallback hybrid "(full-NC Born)/Γ" must never be called σ(γ\*p).
- The arithmetic is verified end-to-end on the cone basis (RAW → Born exact → photon-Born 11.875722/11.571280 nb → γ\*p prototype 1 170 035/1 129 842 nb·GeV², single-file flux, ≲1 % full-pass shift expected).

**Shortest list of measurements that would make it publication-grade:**

1. **M1** — the full-sample DJ14 NoZ Born `<1/Gamma>` pass (260 files; one accumulator).
2. **M2** — the RG31 RAD emfix `<1/Gamma>` pass (measured `flux_model_rel`).
   → one Condor campaign, one worker extension. No other budget measurement is required: the SWAP legs come from existing tables, the carried legs are bit-identical, and every non-leg (convention items) is a statement, not a measurement.
3. (Workflow, not measurement, but on the critical path: the recorded NoZ promotion decision + cone-basis derived build + checksummed install, and the generator EW-mode audit before any PYTHIA8/SHERPA3 γ\*p comparison — theory-side.)
4. Residual ceiling: P-CRAD-LUMI cannot be closed internally; "publication-grade" is bounded by the parent's status and ships with the documented caveat.

---

## 8. Budget artifact contract (specification for the Phase-2 build)

Deliverable: `systematics_gammap_16comp_<basis>_<date>/` **outside** `output_v6/central/data_release/` (Option B), containing:

- `total_uncertainty_summary_gammap_16comp.csv` / `.json` — 490 rows; 16 relative component columns (13 carried names verbatim + `c_gamma_gen_rel`, `c_gamma_stat_rel`, `flux_model_rel`; + `flux_stat_rel` if gated in); carried descriptive columns; absolute-nb columns rescaled by `f(b)`; totals per §3.1 with `errors = total_with_gammap16_rel`.
- `systematics_charged_sidis_highQ2_gammap.json` — release-contract schema: `step: "systematics"`, `channel`, `status` (PASS/FAIL), `input_files`, `output_file`, `bins: 490`, `values` (per-source per-bin per-charge), `errors`, `validation`.
- `validation` block must contain: dominant-at-median check (must remain `ibu_stat_rel`, both charges — not luminosity); completeness (16/16 finite 490/490; the interim flux leg flagged `generator_component_unevaluated_interim_bound`); instability (>50 % bin set equal to the parent's 4 bins; any new bin investigated); comparability (total/stat within [0.5, 5.0]); bit-identity gates for the 13 carried columns vs the parent CSV; per-swap provenance keys (`replaced_leg`, `replacement_basis`, `swap_rule`); expectation verification (median-level quadrature model vs exact per-bin recomputation — deviations are REPORT items per the legenv precedent); the count-weighted anti-trap rejection record.
- `provenance.json`, `README.md` (conventions, interim status, never-mix statement), `validation_checks.json`.

---

## 9. Evidence index (absolute paths; ★ = read directly this session)

- ★ Canonical budget (component names, medians, construction): `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/` — `total_uncertainty_summary_cone_variant_15comp.json` (`components_rel_columns`), `total_uncertainty_summary_cone_variant_15comp.csv` (medians recomputed this session), `systematics_charged_sidis_highQ2.json`, `README.md`, `validation_checks.json`, `comparison_vs_installed_and_rejected_env_2026-10-14.json`.
- ★ Parent releases: `.../data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/`; `.../data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/`.
- ★ NoZ / photon-Born tables: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/` — `cgammaborn_empz_gt45_nominal_490rows.csv` (C_gamma_born, F_EWgamma, per-bin stat), `cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` (level term), `cgammaborn_gen_unc_proxy_v3emfix_490rows.csv` + `.json` (κ-proxy, SWAP rule, mc-expert validation), `README_NOZBORN_RELEASEBIN.md`.
- ★ Applied C_rad pair: `.../data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`; `.../data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`; worker to extend: `.../djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py`.
- ★ Design memos consumed: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/gammap-conversion-prescription-2026-10-15.md` (conversion identity, flux table, per-cell ⟨1/Γ⟩, level term, phases); `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/gammap-electroweak-level-2026-10-15.md` (EW level, F_EWgamma audit, κ-proxy, SWAP rules, +0.05 pp estimate); `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/note_crad_vs_gammaflux_and_R_2026-10-15.md` (no flux double-count; R not needed; NoZ inventory; budget-commutation verdict); `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/methodology/map_ep_xsec_pipeline_touchpoints_2026-10-15.md` (Option B, diagonal congruence, risks R1–R9).
- ★ `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md` (cone triad, blockers, carried caveats, A_h closed).
- NoZ truth trees (M1 input): `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/` (260 files).
- Consolidated baseline reference: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/` (`RELEASE_2026-10-10.md`).

**Provenance:** read-only session (direct reads plus one single-file median computation from the canonical budget CSV on the login node, within the ≤1-file/≤5-min policy). No release artifact, table, budget, plot, script, STATE.md row, or note file was modified; no Condor job was submitted; the only file created is this memo.

---

## 10. DECISION 2026-10-15 (QA decision request on the built γ\*p budget): SWAP MUST REPLACE — relabel rejected; per-cell interim flux leg; REBUILD REQUIRED

**Adjudicated:** the QA findings on `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/gammap_derived/gammap_hand_cone_s2prime_photonborn_2026-10-15/` (`systematics_budget_gammap_transferred.csv`, `metadata.json`, `validation_gammap.json`, `README.md`). All QA numbers reproduce read-only this session: 13 carried legs byte-identical to the 2026-10-14 cone budget; nb-rebasing exact (max dev 1.55e-15); `lumi_rel` flat 0.027 carried; totals byte-equal to the ep parent (h+ 5.8032 % / h− 6.1919 % medians); `c_gamma_born_stat_rel_available` / `flux_stat_rel_available` exact on 490/490; the two swap legs ABSENT; the inherited legs present relabelled `level_denominator_gen_rel__inherited_c_rad_gen_rel` / `level_denominator_stat_rel__inherited_c_rad_stat_rel`; the deviation documented only in `metadata.json` (`level_leg_relabeling`). **Two additional violations QA did not list:** (i) the §3.2-item-17 `flux_stat_rel` gate has FIRED (full-sample medians h+ 0.2653 % / h− 0.2786 %, max 4.1866 % / 7.1646 % — far above the 0.1 % fold threshold) yet the release carries the leg as "information, not folded in"; (ii) the §8 release-contract JSON `systematics_charged_sidis_highQ2_gammap.json` is absent from the release.

### 10.1 Q1 — Verdict: the SWAP must actually replace the values in the total. The as-built relabel-plus-informational form is NOT a legitimate implementation of the swap rule.

1. **Referent.** A budget leg quantifies the uncertainty of the *applied* factor. Here the applied factor is `C_gamma_born = C_rad × F_EWgamma`, not `C_rad`. The relabelled total carries the ep-level `c_rad_gen_rel` (the WithZ DJ/RG emfix spread — the same object as the κ-proxy's first term `v3_emfix_rel` by construction; medians identical to four decimals both charges, 0.5844/0.5804 %) and therefore assigns **zero** generator-scheme uncertainty to the `F_EWgamma` projection — an integrated −8.8 % effect whose per-cell (1−F) grows 0.080→0.153 with Q², with **no generator cross-check measurable** (no RAPGAP NoZ sample; IPRO=1200 has no Z-off switch). Zeroing an unevaluated component is exactly the "assume zero" pattern the failure-handling rule forbids. The dropped κ-term (medians 0.181/0.189 %, max 0.431/0.405 %, growing with Q²; proxy medians 0.6223/0.6189 %) is the *only* coverage of the EW-generator dependence of the applied projection — the very content that distinguishes this product's correction from the ep one. That the median-level shortfall is small (≈0.04 pp on the leg) is not a defense: budget correctness is a referent property, not a median-size property.
2. **Statistics.** The applied factor's per-bin statistical uncertainty is the NoZ-table `relative_stat_unc` = `C_gamma_born_stat_unc/C_gamma_born` (identity verified) — the **full ratio stat** of the applied factor, numerator-dominated (NoZ e− pool: min 237, median 50 537 weighted entries; medians h+ 0.5442 % / h− 0.5721 %, max 5.874 % / 7.837 %). The inherited `c_rad_stat_rel` (0.4272/0.4429 %, max 4.41/6.04 %) quantifies a different ratio (mixed-nonrad/mixed-rad) that this product does not apply; its overlap with the applied factor's stat is the common RAD-mixed *denominator* Poisson only. The builder's "common denominator" premise is TRUE — and it is precisely why the rule is SWAP, never ADD: the two stat legs share the denominator content, so quadrature-adding double-counts it, while *replacing* counts every pool exactly once with full coverage. The as-built uses the true premise to draw the false conclusion (keep the old leg, drop the new content): the released total carries the denominator ⊕ mixed-nonrad-numerator Poisson and leaves the applied factor's numerator pool — the stat-limiting side — at zero. An "available" column that does not enter the quadrature is, from the budget's standpoint, zero; the metadata's "avoid double counting" rationale is a category error (double counting arises only under ADD, and the specified rule was a SWAP).
3. **Schema assertion.** The relabelled column names assert that the inherited ep-level legs *are* the level legs of the γ\*p product. They are a proper subset of the applied factor's uncertainty (the denominator-side part). Totals byte-equal to the ep parent present the γ\*p budget as the ep budget — the exact confusion the four-axis never-mix discipline exists to prevent, now baked into the artifact rather than remaining a quote risk.

### 10.2 Q2 — Exact final column specification for the rebuild (read-only arithmetic over existing tables; NO Condor, NO new measurement)

**Columns added (budget legs):**

| Column | Formula / exact source | Replaces | Nature / correlation | Verification anchors |
|---|---|---|---|---|
| `c_gamma_gen_rel`(b) | `u_model_rel_k002`(b) ≡ sqrt(`v3_emfix_rel`(b)² + [0.02·(1−`F_EWgamma`(b))]²), from `data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_gen_unc_proxy_v3emfix_490rows.csv` (same bin join the as-built informational columns already implement; h− rows global-indexed 245–489) | `level_denominator_gen_rel__inherited_c_rad_gen_rel` (the relabelled `c_rad_gen_rel`) | per-bin (490, charge-split); fully correlated (one generator-scheme decision; magnitude cell-structured via (1−F)) | medians h+ 0.6223 % / h− 0.6189 %; max 5.770 % / 5.674 %; κ-term medians 0.181/0.189 %, max 0.431/0.405 %. κ=0.03 variant (`u_model_rel_k003`, medians 0.6581/0.6528 %) retained as documented variant only |
| `c_gamma_stat_rel`(b) | `relative_stat_unc`(b) = `C_gamma_born_stat_unc`(b)/`C_gamma_born`(b), from `.../cgammaborn_empz_gt45_nominal_490rows.csv` — full per-bin stat of the applied factor (numerator-dominated) | `level_denominator_stat_rel__inherited_c_rad_stat_rel` (the relabelled `c_rad_stat_rel`) | per-bin; binwise uncorrelated (MC Poisson; per-charge pools disjoint — treat uncorrelated, conservative) | medians h+ 0.5442 % / h− 0.5721 %; max 5.874 % / 7.837 %; min 0.0316 % |
| `flux_model_rel`(b) | u_level(cell(b)), per-cell constant (interim): S0_x0 +0.93 %, S0_x1 +1.00 %, S1_x0 +0.54 %, S1_x1 +0.51 %, S2_x0 +0.46 %, S2_x1 +0.56 %, S3_x0 +1.75 % (FULL ABS single-sided). Builder recomputes from `.../cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` + analytic Hand flux; recorded values are the anchor | NEW (replaces nothing) | per-cell constant (interim); fully correlated (one flux-weighting-model decision); final form max(u_level(b), u_gen(b)) per-bin (envelope rule frozen) when M2 lands | the seven recorded values; ≤1.75 % cap; metadata flag `generator_component_unevaluated_interim_bound` MANDATORY |
| `flux_stat_rel`(b) | `invGamma_stat_rel`(b) from `data/flux/hand_gammap_flux_releasebin_2026-10-15/gamma_flux_hand_inverse_fullsample_nozborn_em_260files_2026-10-15.csv` (values already present, exactly reproduced, in the as-built informational column) | NEW; the §3.2-item-17 gate has FIRED, so folding is MANDATORY | per-bin; binwise uncorrelated | medians h+ 0.2653 % / h− 0.2786 %; max 4.1866 % / 7.1646 %; fired-gate record in `validation` |

**Columns removed:** `level_denominator_gen_rel__inherited_c_rad_gen_rel`, `level_denominator_stat_rel__inherited_c_rad_stat_rel` (replaced — a budget CSV must not carry a column whose name asserts a leg it does not budget; provenance moves to metadata keys), `c_gamma_born_stat_rel_available` and `flux_stat_rel_available` (their content becomes the legs). **Carried verbatim:** the 13 carried legs and all descriptive/absolute columns exactly as in the as-built transfer (QA-verified byte-identical, rebasing exact; nothing else changes).

**Totals (recomputed):** `total_quadrature_rel`, `total_with_tracking_rel`, `total_with_phih_rel` unchanged (carried legs only) → `total_with_photon13_rel` = sqrt(`total_with_phih_rel`² + `c_gamma_gen_rel`² + `c_gamma_stat_rel`²), REPLACING `total_with_crad13_rel` (rename mandatory — content changed) → `total_with_cone15prime_rel` = sqrt(`total_with_photon13_rel`² + `detint_removal_rel`² + `fakecol_mcstat_rel`²), REPLACING `total_with_cone15_rel` (rename mandatory) → `total_with_gammap16_rel` = sqrt(`total_with_cone15prime_rel`² + `flux_model_rel`² + `flux_stat_rel`²) — NEW headline, and the `errors` array of the contract JSON. Percent-echo and increment columns follow their renamed totals.

**The 7.84 % NoZ-stat tail: per-bin values stand — NO cap, NO merge with `ibu_stat_rel`.** The max is the applied factor's genuine worst-bin MC stat (n≈237 weighted entries); a cap would understate the product's uncertainty in exactly the sparse high-Q²/high-pT bins. `ibu_stat_rel` is 1000-toy DATA statistics; `c_gamma_stat_rel` is applied-factor MC statistics — distinct objects, uncorrelated, both must remain visible for the dominant-source validation and the A_h non-cancellation structure. Verified in the per-bin design model: the tail lands in bins already `ibu_stat`-dominated (max total 80.40→80.41 % / 84.50→84.52 %) and the >50 % instability bin set remains exactly the parent's 4 bins (1 h+ / 3 h−) under every rebuild scenario; the build-time check must confirm bin-set equality on the exact rebuild.

**Expected total-median shift (per-bin design model from the as-built CSV + source tables; exact recomputation is the builder's job; deviations from these anchors are REPORT items per the legenv precedent):** as-built relabelled (REJECTED) 5.8032 / 6.1919 % → swaps only 5.8151 / 6.2122 % (+0.012 / +0.020 pp) → **swaps + per-cell `flux_model_rel` + gated `flux_stat_rel` (THE SPEC): h+ 5.8558 % (+0.053 pp) / h− 6.2857 % (+0.094 pp)**; conservative flat-1.75 variant 6.0813 / 6.4605 % (+0.278 / +0.269 pp). Validation expectations preserved: dominant-at-median `ibu_stat_rel` both charges; total/stat ≈ 1.6 (within [0.5, 5.0]); >50 % set = parent's 4 bins.

**Contract additions:** emit the §8 release-contract JSON `systematics_charged_sidis_highQ2_gammap.json` (step "systematics", channel, status, `input_files`, `output_file`, bins 490, `values` per-source per-bin per-charge, `errors` = `total_with_gammap16_rel`, `validation` per §8: bit-identity gates for the 13 carried columns, per-swap provenance keys `replaced_leg`/`replacement_basis`/`swap_rule`, the fired-gate record for `flux_stat_rel`, the interim flag for `flux_model_rel`, dominant/completeness/instability/comparability checks). Metadata: `level_leg_relabeling` REMOVED, replaced by `level_leg_swaps`; the "systematics-owner decision" deferral on `c_gamma_born_numerator_stat` is CLOSED (decided: SWAP, this section). File naming: the rebuilt budget is renamed per the §8 family (`systematics_budget_gammap_16comp…`), since "transferred" now mislabels a swap-implementing budget; the superseded transferred CSV is retained under a `_superseded_` marker with a one-line pointer, per workspace convention.

### 10.3 Q3 — moot; the label duty transfers to the rebuild

The as-built form is not defensible, so the memo is NOT amended to match the implementation; instead this §10 corrects the memo's own numbers (pointers at §3.2 items 13′/17 and §3.4). The never-mix label duty survives on the REBUILD. Verbatim label text required in the release README and the analysis note, on every table/figure showing this budget:

> "γ\*p-level systematic budget: 17 components — 13 legs carried bit-identical from the ep cone-basis legenv budget; two swap legs (`c_gamma_gen_rel`, `c_gamma_stat_rel`) REPLACING the ep-level `c_rad_gen_rel`/`c_rad_stat_rel` because the applied factor here is `C_gamma_born = C_rad × F_EWgamma`; `flux_model_rel` at the interim per-cell level bound (+0.46…+1.75 %; generator component unevaluated pending the RAPGAP31 RAD-emfix measurement); `flux_stat_rel` gated in (>0.1 %). This is NOT the ep budget: the ep cone-basis 15-component budget (medians h+ 5.8032 % / h− 6.1919 %) remains the parent product's budget; γ\*p totals (medians ≈5.86 % / ≈6.29 %, interim) must never be quoted against or mixed with ep totals (four-axis never-mix rule: prior basis, level, observable basis, flux convention)."

### 10.4 Q4 — `flux_model_rel` interim treatment: PER-CELL u_level APPLIED NOW (flagged); flat 1.75 % documented as the conservative variant; exclusion FORBIDDEN *(SUPERSEDED 2026-10-16 by §11: M2 landed; the leg is now the MEASURED per-bin envelope max(u_level, u_gen); the interim per-cell form and the `generator_component_unevaluated_interim_bound` flag are retired)*

- **Excluded with named caveat: REJECTED.** The in-bin Γ spread exceeds 10 % in effectively all 490 bins (up to 137× in S3), so the weighting-model dependence is a genuine ~1 %-class systematic of the product's own conversion factor; omitting it zeroes a measured effect and violates the failure-handling rule. (Impact if taken anyway: medians would stay at the swap-only 5.8151 / 6.2122 % with a known ~1 %-class leg missing — not acceptable even internally.)
- **Flat 1.75 % now: acceptable conservative variant, NOT chosen.** Impact: h+ 6.0813 % (+0.278 pp) / h− 6.4605 % (+0.269 pp). It overstates six of seven cells (up to ~3.8× on the leg) when a measured per-cell structure exists.
- **Per-cell +0.46…+1.75 % now: CHOSEN.** Impact: **h+ 5.8558 % (+0.053 pp) / h− 6.2857 % (+0.094 pp)**, both including the newly gated `flux_stat_rel`. It applies the measured component with its measured structure, does not overstate low-spread cells, keeps the generator component explicitly flagged `generator_component_unevaluated_interim_bound` (never silently zero), and is implementable today from existing tables. The ≤1.75 % worst-cell bound is documented as the envelope cap; M2 (RG31 RAD emfix pass) replaces the leg with max(u_level, u_gen) when it lands — required for publication (10.5).

### 10.5 Q5 — Blocking classification

**Blocking for INTERNAL showing:**
1. Swap non-implementation (relabelled inherited legs in the totals; κ-term and applied-factor stat at zero) — the released budget misstates the applied factor's uncertainties under a γ\*p label; no internal showing until rebuilt.
2. `flux_model_rel` absent from the total — a measured ~1 %-class systematic of the product's own conversion factor cannot be shown as absent; the interim per-cell leg is required for any internal display.
3. `flux_stat_rel` gate violation (information-only despite medians 0.265/0.279 %, max 4.19/7.16 % ≫ 0.1 %) — must be folded for any internal display.
4. Release-contract JSON + the 10.3 label text — required on every internally shown table/figure of this budget.

**Blocking for PUBLICATION (not blocking internal):**
5. `flux_model_rel` interim form — publication requires the measured M2 leg (generator component); internal showing is fine with the interim flag. *(CLOSED 2026-10-16: M2 landed and adjudicated — see §11.5.)*
6. Inherited provisional-status caveats (P-CRAD-LUMI file-set-reconciled; `C_gamma_born` PROVISIONAL DIAGNOSTIC DJANGOH-only; absolute-normalization provisional; 100-toy incumbent covariance) — publication-blocking, inherited from the ep parent; internal showing is fine with labels.
7. PYTHIA8/SHERPA3 conversion approximation (theory-side caveat + band on MC curves only, never on data) — publication-blocking for any γ\*p-level comparison statement involving those curves; internal showing is fine caveat-only.

### 10.6 Rebuild scope and provenance

**The current release CSV MUST be rebuilt: YES.** Scope: two swap columns in as legs, two flux columns in as legs, four columns out, three totals recomputed plus one added, metadata `level_leg_relabeling` → `level_leg_swaps`, README label text (10.3), release-contract JSON added. NO Condor, NO new measurement — every leg comes from existing tables, and the as-built release already joins them exactly for the informational columns. The 13 carried legs, the nb-rebasing, the covariance sidecars, and the cross-section products are untouched. **Provenance of this decision:** read-only adjudication; single-file login-node checks only (the release budget CSV, the κ-proxy CSV, the NoZ nominal CSV, the flux CSV header, metadata/README/validation JSONs — each command one input file, seconds of wall); no release artifact, table, plot, script, STATE.md row, or note file modified; no Condor job submitted; the only file modified is this memo (§10 appended plus three supersession pointers).

---

## 11. DECISION 2026-10-16 (M2 landed): FINAL flux_model_rel = MEASURED per-bin envelope max(u_level, u_gen) with LEVEL-CORRECTED u_gen — builder specification

**Adjudicated (read-only, verified this session):** the RG31 RAD emfix flux campaign (cluster 5671056, 128/128 jobs PASS, 490/490 bins measured, 0 fallback bins, 437.7M selected entries; `data/flux/hand_gammap_flux_rg31_rad_emfix_releasebin_2026-10-15/`) and its comparison vs the DJ NoZ-Born production flux table. Verified against `comparison_vs_dj_nozborn_2026-10-15.json`: per-bin |RG−DJ| median 1.04 %, mean 1.77 %, p95 6.21 %, max 12.30 % (bin 189); 253 bins >1 %, 127 >2 %; per-cell charge-pooled |diff| 0.35–1.28 % (S2_x0 largest). Both tables share the flux-table schema and global bin indexing (join exact). Per-bin flux stat read directly: RG `invGamma_stat_rel` median 0.192 %, p95 1.257 %, max 4.484 %; DJ median 0.269 %, p95 1.554 %, max 7.165 % (151 bins >0.5 %, 64 >1 %, 18 >2 %). DJ y-resolved table: 490 bins × 6 y-slices, median 5 populated slices per bin per population, **0 degenerate bins** — per-bin level synthesis feasible everywhere. An exact y-resolved sidecar exists for RG too (per-(slice,bin) `sum_w`, `invGamma_mean_gev2` — an exact decomposition of the 490-row means).

### 11.1 Q1 — The fold: option (a), the FROZEN envelope max(u_level, u_gen) PER BIN, with u_gen level-corrected. No post-hoc rule change.

The rule was frozen before measurement (§3.3: both terms probe the SAME object — the in-bin y-population model of ⟨1/Γ⟩ — along correlated axes; envelope, not quadrature). The measurement gives no grounds to re-open it. Quantitative disposition of the alternatives:

- **(c) raw difference as the leg: REJECTED.** D_raw = δ_level ⊕ δ_gen (exactly multiplicative, additively to first order). The envelope's other argument already carries u_level at full size; taking D_raw would carry the level content twice and roughly doubles the median leg (raw median 1.04 % vs level-corrected u_gen ~0.5 %; per-cell raw 0.35–1.28 % vs level 0.46–1.75 % — same order, so the correction is material, not cosmetic). D_raw survives only as the documented conservative upper variant (§11.4 metadata).
- **(b) quadrature after level removal: REJECTED.** Violates the freeze, and quadrature of correlated same-object axes double-ish counts (a generator y-shape difference also shifts the radiative distortion — that correlation is why the envelope was frozen).
- **(d) per-cell: REJECTED as the central form.** The per-bin structure is RESOLVED (see §11.3: the five worst bins are 3.6–21σ above the comparison's combined statistical noise), and within-cell per-bin |raw| medians (0.69–2.61 %) far exceed the pooled cell means (0.35–1.28 %) — real z/pT-dependent model structure that per-cell pooling would discard. Per-cell values survive as the δ_level fallback and as validation anchors.
- **The measured per-cell decomposition (charge-pooled; the quantitative justification):** with signed raw = (G_RG−G_DJ)/G_DJ per cell and δ_level = +u_level per cell, δ_gen = (1+raw)/(1+δ_level) − 1:

| Cell | u_level % | raw signed % | u_gen = \|δ_gen\| % | envelope max % | interim leg % |
|---|---|---|---|---|---|
| S0_x0 | +0.93 | +0.87 | 0.06 | **0.93** | 0.93 |
| S0_x1 | +1.00 | +0.50 | 0.50 | **1.00** | 1.00 |
| S1_x0 | +0.54 | −0.65 | 1.19 | **1.19** | 0.54 |
| S1_x1 | +0.51 | −0.85 | 1.36 | **1.36** | 0.51 |
| S2_x0 | +0.46 | −1.28 | 1.73 | **1.73** | 0.46 |
| S2_x1 | +0.56 | −1.18 | 1.73 | **1.73** | 0.56 |
| S3_x0 | +1.75 | +0.35 | 1.38 | **1.75** | 1.75 |

The generator component is NEGATIVE in six of seven cells (RG31's flux weighting sits systematically below DJ14's at common RAD level) and **GOVERNS the envelope in S1/S2**, where the interim level-only leg underestimated the model dependence by up to ~3.8× (S2_x0: 0.46 → 1.73 %). The M2 measurement was necessary, not decorative: the interim budget under-covered a real, coherent, one-nuisance generator effect — exactly the fully-correlated whole-vector class the correlation table assigns.

### 11.2 Q2 — Level correction: REQUIRED. Exact builder form (all columns exist; no interpretation needed)

1. **Per-bin signed level ratio (synth-vs-synth, discretization cancels):**
   `δ_level(b) = G_DJ_RAD_synth(b) / G_DJ_NoZBorn_synth(b) − 1`,
   `G_DJ_X_synth(b) = Σ_y σ_X(b,y)·κ(b,y) / Σ_y σ_X(b,y)`, X ∈ {`rad_mixed`, `gamma_born`},
   from `data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` (columns `sigma_rad_mixed`, `sigma_gamma_born`, joined on `charge`+`bin_index`+`y_index`; 0 degenerate bins verified), with κ(b,y) = the analytic Hand inverse flux under the SAME (y-slice, Q²) convention that produced the validated per-cell values (prescription-memo recipe; the seven recorded values are the anchor).
2. **Per-bin generator component (measured-vs-measured, event-level both sides):**
   `u_gen(b) = | G_RG(b) / ( G_DJ(b) · (1 + δ_level(b)) ) − 1 |`,
   `G_RG(b)` = `invGamma_mean_gev2` from `.../hand_gammap_flux_rg31_rad_emfix_releasebin_2026-10-15/gamma_flux_hand_inverse_fullsample_rg31_rad_emfix_1273files_2026-10-15.csv`; `G_DJ(b)` = `invGamma_mean_gev2` from `.../hand_gammap_flux_releasebin_2026-10-15/gamma_flux_hand_inverse_fullsample_nozborn_em_260files_2026-10-15.csv` (the APPLIED flux). Join on global `bin_index` (0–489, h+ 0–244 / h− 245–489 — verified identical in both tables).
3. **Per-bin level component:** `u_level(b) = |δ_level(b)|` (per-bin refinement of the per-cell vector).
4. **Why this construction is clean:** the level ratio is computed synth-vs-synth (identical discretization of both DJ populations → cancels to first order); the generator ratio is measured-vs-measured (event-level on both sides). Residual systematics: (i) second-order within-slice y-shape differences between the two DJ populations — bounded by the self-check gate below; (ii) beam composition — NEGLIGIBLE, ≲0.05 %: after level correction the comparison is beam-MATCHED (the DJ RAD reference `sigma_rad_mixed` is itself the mixed-beam pool; the RG table pools 582 e− + 691 e+ files with the campaign's lumi-consistent weights), the Hand kernel Γ(Q²,y) is charge-blind, and the residual is the pool-fraction mismatch × the γZ-induced y-shape difference (mixed-beam charge-antisymmetric content ≲0.01 % per the §2.2 dilution argument; ≲0.05 % with shape effects, conservative). An exact per-beam verification (re-run over `{em,ep}` separately) is available if ever queried but is NOT required.
5. **Gates and fallbacks (bins where the correction is unavailable or unstable):**
   - **Anchor gate:** the per-cell charge-pooled mean of per-bin `u_level(b)` must reproduce +0.93/+1.00/+0.54/+0.51/+0.46/+0.56/+1.75 % within ±0.10 pp; likewise the cell-pooled level-corrected `u_gen` must reproduce 0.06/0.50/1.19/1.36/1.73/1.73/1.38 % within ±0.10 pp. Deviations are REPORT items.
   - **Synthesis self-check:** r(b) = G_DJ_NoZBorn_synth(b)/G_DJ_NoZBorn_meas(b) — report the distribution; bins with |r(b)−1| > 0.02 use the CELL-level δ_level (fallback, flagged).
   - **Degenerate-slice fallback:** bins with <2 populated y-slices for either population use the cell-level δ_level (none exist today, verified; if any appear, flagged). The fallback bin list is a mandatory `validation` entry.
   - **Cross-check (recommended, REPORT item):** reconstruct G_RG(b) from the RG y-resolved sidecar (Σ_y `sum_w`·`invGamma_mean_gev2` / Σ_y `sum_w` — an exact identity) and confirm equality with the 490-row value; and compute the convention-matched variant `u_gen_synth(b) = |G_RG_RAD_synth/G_DJ_RAD_synth − 1|` — agreement with the primary estimator documents the discretization control.

### 11.3 Q3 — Instability test and verdict: the tail is REAL model dependence, not flux statistics. Per-bin leg DEFENSIBLE. *(PARTIALLY SUPERSEDED 2026-10-16 by §12: the resolution test moves from |D_raw| to the level-corrected |D_corr| — the |D_raw| test mis-selects level/gen cancellation bins; unresolved bins now take the pooled cell value u_gen_cell, reversing the keep-per-bin policy for the noise-dominated subset ONLY; every resolved value remains uncapped; the "real tail" verdict itself STANDS)*

- **Hard test (builder MUST run and record):** per bin, `σ_comb(b) = sqrt(σ_RG(b)² + σ_DJ(b)²)`, the quadrature of the two tables' `invGamma_stat_rel` columns; resolution flag = (`|D_raw(b)| > 2·σ_comb(b)`).
- **Measured verdict (spot-checked at the five worst bins this session):** bin 189: |D| = 12.30 % vs σ_comb = 0.58 % → **21σ**; bin 434: 11.64 % vs 0.61 % → 19σ; bin 209: 11.54 % vs 3.23 % → 3.6σ; bin 224: 10.85 % vs 0.92 % → 12σ; bin 194: 10.49 % vs 0.74 % → 14σ. All resolved. The 12.3 % tail is a genuine in-bin population-weighting difference (in-bin 1/Γ spreads 12–360×; both samples populous there, e.g. bin 189 n_RG = 39,224 / n_DJ = 14,474) — precisely the `flux_model_rel` effect as defined. NOT RG flux statistics, NOT bin-population instability.
- **Acceptance criterion:** the per-bin leg stands if every bin with |D_raw| > 2 % that FAILS the 2σ test is individually explained in `validation` (expected count ≈ 0 from the spot checks; the unresolved population is expected O(10–50) bins confined to the DJ-stat tail — 151 bins >0.5 %, 64 >1 %, 18 >2 % — mostly with |D_raw| ≤ 2 %). The builder reports the full resolved/unresolved partition.
- **Policy for unresolved bins (|D_raw| ≤ 2σ_comb):** KEEP the level-corrected |u_gen(b)| as the leg value — never zero, never cell-shrunk. Unresolved ≠ small: the true model difference is merely bounded, and substituting the cell value (≤1.73 %) would understate sparse-bin model structure where the bound is several %. Flag these bins `flux_model_stats_limited` (informational column + `validation` list), with the documented caveat that the value contains the comparison's MC-statistical noise (dominated by the DJ flux stat, separately carried as `flux_stat_rel` — a small, documented, conservative overlap). This honors the failure-handling rule: investigated, recorded, not silently replaced.
- **No cap anywhere.** The resolved tail values stand as measured; they land in bins already `ibu_stat`-dominated, and the >50 % instability bin-set equality check (§10.2) guards the totals.

### 11.4 Q4 — FINAL flux-leg column set and formulas (builder-implementable) *(SUPERSEDED 2026-10-16 by §12: u_level retired to per-cell constants; resolution test corrected to |D_corr| > 2σ_comb with cell-value fallback; revised column set; the §11.4 expected-medians block is RETRACTED as design-estimate errors — see §12.1/§12.4)*

| Column (exact name) | Formula (exact) | Status |
|---|---|---|
| `flux_model_rel`(b) | max( u_level(b), u_gen(b) ) with u_level, u_gen per §11.2 | **LEG** — replaces the interim per-cell constant; per-bin magnitudes; fully correlated whole-vector (one model-decision family, the §2.2 correlation class unchanged); FINAL form of §3.2 item 16 |
| `flux_model_u_level_rel`(b) | \|δ_level(b)\| (§11.2 item 3) | informational |
| `flux_model_u_gen_rel`(b) | \|G_RG(b)/(G_DJ(b)·(1+δ_level(b))) − 1\| (§11.2 item 2) | informational |
| `flux_model_resolved_flag`(b) | 1 if \|D_raw(b)\| > 2·sqrt(σ_RG(b)²+σ_DJ(b)²) else 0; σ from the two `invGamma_stat_rel` columns | informational (§11.3 diagnostic) |
| `flux_stat_rel`(b) | `invGamma_stat_rel`(b), DJ 260-file table | **LEG — UNCHANGED** from the 2026-10-15 rebuild (applied-flux MC stat; the RG table's stat is NOT a leg — the RG flux is not applied; it enters only σ_comb) |

- **Totals:** `total_with_gammap16_rel` = sqrt(`total_with_cone15prime_rel`² + `flux_model_rel`² + `flux_stat_rel`²) — the only recomputed total; everything else in the 2026-10-15 rebuilt budget is untouched. Covariance sidecar: the flux contribution remains the rank-1 outer product u_i·u_j·σ_iσ_j (§2.3), now with the measured per-bin vector.
- **Interim per-cell u_level values:** superseded as the leg; retained as (i) the §11.2 anchor-gate values and (ii) the δ_level fallback values.
- **Flag retirement:** REMOVE `generator_component_unevaluated_interim_bound` — the generator component is MEASURED. Add to metadata: `flux_model_status: "measured_envelope_ulevel_ugen_M2_2026-10-15"`, the frozen-rule record (envelope; level-corrected u_gen; no post-hoc change), the conservative upper variant `flux_model_upper_raw_rel(b) = |D_raw(b)|` (documented, NOT the central leg), the beam-mix bound note (≲0.05 %), and the stats-limited bin list.
- **README/analysis-note label** (replaces the corresponding sentence of the §10.3 verbatim text): "flux_model_rel MEASURED per bin: envelope max(u_level, u_gen) — the DJ-internal level dependence of the in-bin flux weighting and the level-corrected RAPGAP31-vs-DJANGOH14 generator difference (RG31 RAD emfix, 1273 files, mixed beam; M2, 2026-10-15); no interim bound remains. flux_stat_rel is the applied-flux MC statistics (DJANGOH14 NoZ), gated in at >0.1 %. This is NOT the ep budget; the four-axis never-mix rule stands."
- **Caveat-text update:** the `gammap_caveat` "the flux level has no generator cross-check" is SUPERSEDED (u_gen is that cross-check, measured). The `C_gamma_born` DJANGOH-only caveat REMAINS in force — no RAPGAP NoZ sample exists for the EW projection; the κ-anchor inside `c_gamma_gen_rel` stays, and the two legs must NOT be merged (distinct objects: EW projection factor vs flux weighting model; add-in-quadrature per §3.3).
- **Expected headline medians (design estimates; exact per-bin recomputation is the builder's job; deviations from these anchors are REPORT items):** the flux-leg median rises ~0.56 % → ~1.0 % (plausible range 0.9–1.4 %), but both median-TOTAL bins sit in cells where the envelope equals u_level — the interim value (h+ median bin in S3_x0, envelope 1.75 %; h− in S0_x0, envelope 0.93 %) — so the headline medians move only through median migration from the S2-cell growth: **h+ ≈ 5.86 % (5.8550 + 0.00…+0.03 pp); h− ≈ 6.29 % (6.2845 + 0.00…+0.03 pp)**. Validation expectations unchanged: dominant-at-median `ibu_stat_rel` both charges; >50 % bin set equal to the parent's 4 bins (the flux tail lands in `ibu_stat`-dominated sparse bins; bin-set equality MUST be re-verified on the final rebuild); total/stat within [0.5, 5.0].

### 11.5 Q5 — Blocker status after M2 *(UPDATED 2026-10-16 by §12.5: #5 STAYS CLOSED; new internal item G-8, the §12 v2 estimator rebuild; and per the 2026-10-16 user decision recorded in `sidis_highQ2/STATE.md` the γ\*p program is OPTIONAL auxiliary material — NO γ\*p item blocks or gates the ep deliverable)*

- **CLOSED by this measurement:** publication blocker **#5** (flux_model_rel interim form / generator component unevaluated) — the leg is now measured with full diagnostics (resolution partition, anchor gates, fallback list). Internal blockers **#1–4** remain closed by the 2026-10-15 rebuild, subject to the same validation suite passing on the final-leg rebuild.
- **REMAIN publication-blocking:** **#6** — inherited provisional-status caveats: P-CRAD-LUMI (file-set-reconciled, generator-log closure blocked externally); `C_gamma_born` PROVISIONAL DIAGNOSTIC DJANGOH-only (the EW-projection part stands; only the flux-weighting part of the DJANGOH-only caveat is superseded by u_gen); absolute-normalization provisional (P-ABS-NORM); 100-toy incumbent covariance. **#7** — PYTHIA8/SHERPA3 conversion approximation (theory-side caveat + band on MC curves only). Neither is affected by M2.

### 11.6 Provenance

Read-only adjudication; single-file login-node checks only (the RG 490-row flux CSV, the DJ 490-row flux CSV, the DJ y-resolved 2940-row CSV, the RG y-resolved sidecar header, the two flux-directory listings, `STATUS_flux_rg31rad_batch_2026-10-15.json`, `comparison_vs_dj_nozborn_2026-10-15.json` — each command one input file, seconds of wall; per-cell decomposition arithmetic performed on numbers already read). No release artifact, table, budget, plot, script, STATE.md row, or note file modified; no Condor job submitted; the only file modified is this memo (§11 appended plus supersession pointers at §3.2 item 16, §10.4, §10.5 item 5).

## 12. DECISION 2026-10-16 (builder return on §11, five deviations): the median rises are PREDOMINANTLY REAL — per-bin u_level RETIRED to cell constants; resolution test CORRECTED to |D_corr| > 2σ_comb with pooled-cell fallback; §11.4 design anchors RETRACTED

The builder implemented §11 exactly; every reported deviation traces to MY §11 design estimates or MY §11.3 test specification, not to builder error. All five reported deviations verified read-only against the installed release and reproduce exactly (§12.6). This section is the binding v2 estimator specification.

### 12.0 Ruling summary

1. **Diagnosis:** the +0.462/+0.247 pp headline-median rises are ≈70–90 % REAL per-bin level-corrected generator structure (referenced to the corrected-test sweep: +0.40/+0.18 pp real, +0.06/+0.07 pp |noise|-max bias); the bias is REAL but TAIL-localized (sparse-bin legs, S0_x0, unresolved S3), not the median-rise explanation. The builder's proposed bias mechanism is correct as a mechanism and wrong as the median explanation; the separating evidence is in §12.1/§12.6.
2. **Estimator (Q2):** option (c) — `flux_model_rel(b) = max(|δ_level_cell(cell(b))|, u_gen_eff(b))`, with `u_gen_eff(b) = |D_corr(b)|` where `|D_corr(b)| > 2·σ_comb(b)` (CORRECTED test) and the pooled cell value `u_gen_cell(cell(b))` otherwise. Envelope rule unchanged; no cap.
3. **Level term (Q3):** PER-CELL constants; the §11.2 per-bin refinement is RETIRED (failed its own gate 5/7, all-upward, sparsity-scaled — the signature of |noise|, not model structure).
4. **Revised expected medians (Q4):** h+ ≈ 6.2 % (window 6.10–6.30), h− ≈ 6.44 % (window 6.36–6.50); the installed v1 budget is DEFENSIBLE for internal showing at HEADLINE level only, with the mandatory caveat label (§12.4 verbatim text); NOT defensible per-bin.
5. **Ledger (Q5):** #5 STAYS CLOSED; new internal item G-8 (this rebuild); #6/#7 unchanged; per the 2026-10-16 user decision (`sidis_highQ2/STATE.md`), NO γ\*p item blocks or gates the ep deliverable — this is a qualifications ledger on optional auxiliary material.

### 12.1 Q1 — Mechanism, measured; the bias/real separation

- **REAL (dominant at the medians).** Both migrated median bins' v1 legs are per-bin `u_gen`, not `u_level`: pos bin 125 (S1_x1) u_gen = 2.4487 %, flag = 1 (resolved even under the D_raw test); neg bin 120 (S1_x1) u_gen = 1.9022 %, flagged unresolved ONLY through level/gen cancellation in |D_raw| (per-bin level +1.5141 % against a negative generator difference ⇒ small |D_raw|); it resolves under the corrected test. Under v2 both bins KEEP their legs — the headline rise does not collapse.
- **BIAS, tail-localized (the level term).** Per-cell means of per-bin |δ_level(b)| exceed the anchors in 5/7 cells, ALL UPWARD, scaling with cell sparsity: +0.100/+0.040 (S0_x0/S0_x1) → +0.231/+0.769 (S1) → +0.366/+1.199 (S2) → +2.171 pp (S3_x0). Single-bin excursions reach 7.6–12.3 % in S3_x0 (g224: u_level 12.34 % vs cell 1.75 %, u_gen 1.70 %; g234: 7.56 % vs u_gen 0.26 %). The level synthesis carries per-bin MC statistics (σ_δlevel ≈ 0.3 % dense to several % sparse; no per-slice stat errors are recorded) that match or exceed the physical signal (0.46–1.75 %). At the leg MEDIAN this term contributes only +0.045 pp (h+) / 0.000 (h−); it governs the envelope in 37 (h+) / 40 (h−) bins and dominates the >5 % tail in ~6 bins per charge.
- **BIAS + MISCLASSIFICATION (the flag).** §11.3 tested |D_raw| = |level + gen| — not the quantity the leg carries. Measured consequence (the builder's proposed test, adopted): unresolved-bin u_gen means 1.775 % (h+) / 1.942 % (h−) ≈ resolved-bin means 1.837/1.839 % — the unresolved set is NOT noise-selected; it is dominated by cancellation bins carrying real cell-level signal (unresolved S1_x1 u_gen mean 1.326/1.227 vs pooled truth 1.355). The genuine noise is localized: unresolved S0_x0 u_gen mean 0.898/1.041 % vs truth 0.063 %; unresolved S3_x0 3.714/4.768 % vs truth 1.379 %. The 177-count and the 24 big-|D_raw|-unresolved bins are artifacts of the wrong test plus the real stat tail — my §11.3 expectations (O(10–50); ≈0) were design-estimate errors.
- **VERDICT on the rises.** Referenced to the corrected-test sweep (medians 6.2588/6.4654 %, partition-insensitive): h+ +0.462 pp = +0.40 real + 0.06 bias; h− +0.247 = +0.18 real + 0.07 bias. Leg median 0.56 → 1.69/1.61: essentially ALL real (v2 leg median ≈ 1.73/1.61). The |noise|-max bias manifests as: legs >5 % 21→13 (h+) and 15→10 (h−) under v2; the 24 listed bins' mean leg 5.14→3.70 / 5.88→4.70.
- **ROOT CAUSE of the failed §11.4 anchors — mine, three errors, all RETRACTED:** (a) sign-handling bug in my per-cell design proxy — I computed |median-of-|D_raw|| − level per cell, losing the SIGN of the cell raw difference (S1_x0 signed raw ≈ −0.65 %), understating the true cell generator envelopes (1.19/1.36/1.73/1.73 % for S1_x0…S2_x1 vs my 0.54/0.51/0.99/1.40 proxy); (b) per-bin ≈ cell assumption — neglected real resolved per-bin dispersion (median-bin u_gen 2.45 vs cell 1.36); (c) unresolved-count estimates that ignored the cancellation bins selected by the |D_raw| test. The dominant-at-median = `ibu_stat_rel` expectation assumed no median migration; the migration is REAL (to dense S1_x1 bins with small ibu_stat and large ibu_reg/elec_energy values), PERSISTS under v2, and that expectation is retracted (the not-luminosity hard check stands).

### 12.2 Q2 — Estimator ruling: option (c) with the CORRECTED test. Exact formulas.

**CHOSEN (v2, binding):**

Constants per cell c ∈ {S0_x0, S0_x1, S1_x0, S1_x1, S2_x0, S2_x1, S3_x0}:
- `δ_level_cell(c)` (SIGNED; the builder's `level_term_anchor_check` recomputation, agreement ≤0.05 pp with the recorded anchors; charge-pooled): S0_x0 +0.009274572804495618; S0_x1 +0.009960480172857222; S1_x0 +0.005370930907010241; S1_x1 +0.005096304843307697; S2_x0 +0.004617423585718594; S2_x1 +0.005502114174376516; S3_x0 +0.017337179821288302.
- `u_gen_cell(c)` (ABSOLUTE; the `anchor_gate_u_gen` values, HARD-gated ±0.10 pp; charge-pooled): S0_x0 0.00062961737150391; S0_x1 0.004983346719885984; S1_x0 0.011876178590037756; S1_x1 0.013548380244515434; S2_x0 0.017348958284617133; S2_x1 0.017269344558473398; S3_x0 0.013787135619949709.

Per bin b (global 0–489; cell(b) from the release `cell_label`):
1. `u_level(b) := |δ_level_cell(cell(b))|` — a cell constant (§12.3).
2. `D_corr(b) := G_RG(b)/(G_DJ(b)·(1+δ_level_cell(cell(b)))) − 1` — G_RG = M2 RG31 RAD emfix `invGamma_mean_gev2`, G_DJ = applied DJANGOH14 NoZ-Born `invGamma_mean_gev2`, join on global bin (sources unchanged from §11.2). The CELL correction replaces §11.2's per-bin δ_level(b) here too: the per-bin correction injected the level-synthesis noise into u_gen (v1 u_gen noise ≈ sqrt(σ_comb² + σ_δlevel²)); the cell correction leaves u_gen with σ_comb only.
3. `u_gen(b) := |D_corr(b)|`.
4. `σ_comb(b) := sqrt(σ_RG(b)² + σ_DJ(b)²)` (unchanged; the two `invGamma_stat_rel` columns).
5. `resolved(b) := 1 if |D_corr(b)| > 2·σ_comb(b) else 0` — **CORRECTED TEST**: on the level-corrected difference the leg actually carries, replacing the |D_raw| test.
6. `u_gen_eff(b) := u_gen(b) if resolved(b)=1 else u_gen_cell(cell(b))`.
7. `flux_model_rel(b) := max(u_level(b), u_gen_eff(b))` — envelope rule UNCHANGED (frozen §11.1 option (a)); fully correlated whole-vector; NO CAP anywhere.
8. `flux_stat_rel`, the other 15 components, and `total_with_gammap16_rel` = sqrt(`total_with_cone15prime_rel`² + `flux_model_rel`² + `flux_stat_rel`²): UNCHANGED. Covariance sidecar: same rank-1 outer-product rule with the v2 vector. Release contract JSON: add the v2 spec string and the rebuilt label medians.

Informational columns: `flux_model_u_level_rel` = per-bin |δ_level(b)| synthesis value (RETAINED for audit; NO leg role); `flux_model_u_gen_rel` = u_gen(b) as defined above (cell-corrected); `flux_model_resolved_flag` = resolved(b) (recomputed); **NEW** `flux_model_u_gen_source(b)` ∈ {"perbin","cell"}. Metadata: `flux_model_status: "measured_envelope_ulevelcell_ugen_correctedtest_v2_2026-10-16"`; the conservative upper variant `flux_model_upper_raw_rel(b) = |D_raw(b)|` stays documented; the beam-mix bound note (≲0.05 %) stays; the stats-limited listing is re-derived under the corrected test.

**REJECTED:** (a) keep v1 — retains the noise-enshrined level term and |noise| in genuinely unresolved bins (S0_x0: 0.90–1.04 % vs 0.063 % truth; S3 sparse: 3.7–4.8 % vs 1.379 %). (b) quadrature-subtract + floor at zero — floors at zero (violates never-zero; discards the pooled measured signal in cancellation bins); subtracts the WRONG noise under v1 (u_gen noise there is sqrt(σ_comb²+σ_δlevel²), not σ_comb); biased low for |γ| ≲ σ; the pooled cell value is a strictly better shrinkage target than zero because it is itself measured and HARD-gated. (d) smooth/fit across cells — the seven pooled values ARE the resolved structure (a smooth curve would smear the real S0_x0→S1_x0 discontinuity 0.06→1.19 % and create a fitted-object provenance problem); smoothing touches only the fallback values, where the pooled cell value is already the natural estimator. (e) full |D_raw| envelope — double-carries the level term and the noise; REMAINS the documented conservative upper variant only.

**EXPLICIT REVERSAL of §11.3's "never cell-shrunk" policy for u_gen, and why it is principled:** §11.3 guarded against replacing a measured value with something smaller. The corrected test restores that guard where it matters: EVERY bin whose level-corrected difference is resolved keeps its per-bin value, uncapped — including the tails that stay resolved (≈5–6 of the 24 listed bins keep legs ~8–13 %). Only bins with |D_corr| ≤ 2σ_comb fall back, and the replacement is not zero, not a cap, not a shrink below measured content — it is the pooled cell MEASUREMENT `u_gen_cell` (the same object §11.2 already HARD-gated), the statistically correct shrinkage target when the per-bin reading is uninformative. The old policy, on the measured evidence, enshrines MC statistics as model uncertainty precisely where the comparison cannot resolve model from noise — a worse defect than the understatement risk it guarded against. Never-zero is preserved structurally: `flux_model_rel(b) ≥ |δ_level_cell(cell(b))| ≥ 0.46 %` everywhere (HARD gate). The no-fallback bound remains available as the documented |D_raw| upper variant.

**Winners'-curse caveat (recorded, accepted):** marginally-resolved sparse bins (|D_corr| just above 2σ_comb) retain a selection-inflated component. Accepted — conservative direction, bounded by the upper variant; no cap, no further correction.

### 12.3 Q3 — Level-term ruling: PER-CELL constants; per-bin refinement RETIRED

- `u_level(b) := |δ_level_cell(cell(b))|` — the seven constants in §12.2. The §11.2 per-bin refinement is RETIRED from the leg.
- Why (measured, not taste): (i) the per-bin values failed their own §11.2 anchor gate in 5/7 cells, ALL UPWARD — the one-directional signature of E|X+n| ≥ |E X| with the pooled value the truth; the gate fired exactly as designed; (ii) the deviations scale with cell sparsity — i.e. with the known per-slice Poisson statistics of the y-resolved synthesis, not with any physical gradient; 9–12 % single-bin level excursions in S3_x0 adjacent to the 1.75 % cell value are not credible level dependence; (iii) the SAME tables pooled per cell reproduce the anchors at ≤0.05 pp — the machinery is sound, the per-bin refinement is noise-dominated (σ_δlevel ≳ signal); (iv) no per-slice stat errors exist in the 2940-row table, so no per-bin resolution test for the level term is even definable — per-bin retention is undefendable.
- The per-bin synthesis value stays as the informational column `flux_model_u_level_rel` (audit only). `level_term_anchor_check` (pooled, ±0.05 pp) remains a HARD gate for the constants. The §11.2 "per-bin refinement where the synthesis supports it" clause is retracted: the synthesis does not support it per bin; this ruling resolves the gate's REPORT item.

### 12.4 Q4 — Revised expected medians; internal-showing verdict for the installed v1 budget

**v2 expected anchors (design estimates with honest error bars; deviations outside windows are REPORT items, not hard gates):**
- Headline medians (`total_with_gammap16_rel`): **h+ ≈ 6.2 % (window 6.10–6.30); h− ≈ 6.44 % (window 6.36–6.50).**
- Flux-leg median ≈ 1.6–1.75 % both charges (v1 measured 1.694/1.606 — the leg MEDIAN barely moves; v2 corrects the tail and the per-bin composition).
- n_unresolved(corrected test) ≈ 55–80 per charge (v1: 83/94 under the D_raw test).
- Legs >5 %: ≈13 (h+) / ≈10 (h−) (v1: 21/15). The 24-bin listing re-derived: ≈5–6 bins stay resolved with large per-bin values; the rest take the cell fallback.
- >50 % bin-set equality with the parent's 4 bins: expected to HOLD (1 h+ / 3 h− in every bracketing variant); re-verify on the rebuild.
- Basis: single-file bracketing arithmetic on the installed CSV — all-cell floor 5.979/6.288; D_raw-flag partition 6.093/6.393; per-bin-everywhere 6.259/6.465; corrected-test sweep (thresholds 2.0–2.4·`flux_stat_rel`) FLAT at 6.2588/6.4654 — the median is partition-INSENSITIVE (fallbacks sit in the sparse upper tail); the remaining ±0.1 pp covers the cell-level-correction recompute (removes the level-synthesis noise the §11.2 per-bin correction injected into u_gen; downward in S1_x1/S2_x1). Median bins migrate again under v2 (sweep: S2_x0) — no fixed median-bin expectation is set.
- The §11.4 anchors (h+ ≈ 5.86 / h− ≈ 6.29, +0.00…+0.03 pp; leg median ~1.0 %; O(10–50) unresolved; ≈0 big-unresolved; dominant-at-median = `ibu_stat_rel`) are RETRACTED and replaced by the above. Dominant-at-median becomes a recorded diagnostic with NO fixed expected leg; the not-luminosity hard check STANDS.

**INTERNAL-SHOWING VERDICT (installed v1 budget):** DEFENSIBLE for internal showing at HEADLINE level ONLY, with the mandatory caveat label; NOT defensible for per-bin or leg-level quoting, note figures, or external circulation. Basis: v1's headline medians exceed the v2 expectation by ≈ +0.05…+0.15 pp (h+) / +0.03…+0.14 pp (h−) — order 1 % relative, conservative direction; all measured objects are present; every deviation is honestly recorded in `validation_gammap.json`. The defects are tail-localized. This is categorically different from the §10 relabel build, which MISSTATED the referent and was rejected outright. **Mandatory caveat text (verbatim, appended to the README label):** "flux_model_rel estimator v1 (per-bin u_level + per-bin u_gen, |D_raw| resolution test): the level term and unresolved generator values carry |noise| inflation — headline medians are biased upward ≈ +0.05…+0.15 pp vs the v2 estimator (memo §12, 2026-10-16); sparse-tail legs are overstated (legs >5 %: 21/15 bins vs ≈13/10 expected under v2); per-bin values must not be quoted. v2 rebuild pending."

The v2 rebuild is single-pass arithmetic over existing artifacts (the two flux tables + the release budget CSV + the 14 constants in §12.2); no MC production, no Condor. Schedule before any bin-level use of the γ\*p budget. Builder-side execution.

### 12.5 Q5 — Blocker ledger, updated

- **#5 (flux_model_rel interim / generator component unevaluated): STAYS CLOSED.** The generator component is MEASURED (M2, 490/490, pooled gates 7/7 at ≤0.005 pp; per-bin resolved values exist and are kept under v2). An estimator correction is not a reopening of the measurement.
- **NEW internal item G-8:** the §12 v2 estimator rebuild — binding for any bin-level/leg-level use of the γ\*p budget; the v1 headline-only showing privilege (§12.4) expires when v2 lands.
- **#6** (P-CRAD-LUMI; `C_gamma_born` DJANGOH-only EW projection; P-ABS-NORM; 100-toy incumbent covariance) and **#7** (PYTHIA8/SHERPA3 conversion approximation): UNCHANGED.
- **SCOPE RE-FRAME per the 2026-10-16 user decision recorded in `sidis_highQ2/STATE.md`:** the reporting level is the ep cross section; the γ\*p derivative program is OPTIONAL auxiliary material; NO γ\*p item — including G-8, #6, #7 — blocks or gates the deliverable. This ledger is hereby a QUALIFICATIONS ledger on an optional auxiliary product, binding only if that product is quoted. No γ\*p blocker may be recorded against the ep release.

### 12.6 Evidence (single-file checks, 2026-10-16; all numbers reproduce the builder's report)

- **Per-cell decomposition** (charge-pooled, n=70/cell; from `systematics_budget_gammap_16comp.csv`):

| cell | u_level mean vs anchor (pp) | u_gen mean / median vs pooled truth | v1 leg mean / median | cell-truth envelope | frac level-governed | n_unres (D_raw) |
|---|---|---|---|---|---|---|
| S0_x0 | 1.0295 vs 0.93 (+0.100) | 0.587 / 0.485 vs 0.063 | 1.060 / 0.972 | 0.930 | 0.86 | 23 |
| S0_x1 | 1.0399 vs 1.00 (+0.040) | 0.840 / 0.596 vs 0.498 | 1.343 / 1.323 | 1.000 | 0.61 | 18 |
| S1_x0 | 0.7708 vs 0.54 (+0.231) | 1.458 / 1.063 vs 1.188 | 1.515 / 1.110 | 1.188 | 0.07 | 23 |
| S1_x1 | 1.2790 vs 0.51 (+0.769) | 1.799 / 1.782 vs 1.355 | 1.904 / 1.812 | 1.355 | 0.29 | 33 |
| S2_x0 | 0.8264 vs 0.46 (+0.366) | 1.986 / 1.857 vs 1.735 | 2.016 / 1.857 | 1.735 | 0.04 | 22 |
| S2_x1 | 1.7588 vs 0.56 (+1.199) | 2.618 / 2.203 vs 1.727 | 2.749 / 2.312 | 1.727 | 0.17 | 30 |
| S3_x0 | 3.9208 vs 1.75 (+2.171) | 3.644 / 1.844 vs 1.379 | 5.174 / 4.443 | 1.750 | 0.57 | 28 |

- **Builder's test (adopted):** u_gen means resolved/unresolved = 1.837/1.775 % (h+, 162/83 bins), 1.839/1.942 % (h−, 151/94) — the flag is not noise-selected. Per-cell unresolved u_gen means vs truths: S0_x0 0.898/1.041 vs 0.063 (noise); S1_x1 1.326/1.227 vs 1.355 (real signal, mis-flagged); S3_x0 3.714/4.768 vs 1.379 (noise).
- **Median bins:** pos g125 (S1_x1): total 6.3170, flux 2.4487 = u_gen (u_level 1.1449), flag 1, ibu_stat 2.3869, ibu_reg 3.3269, elec_energy 1.3863; same-bin total under the interim leg 5.8454. neg bin 120 → g365 (S1_x1): total 6.5311, flux 1.9022 = u_gen (u_level 1.5141), flag 0 (cancellation), ibu_stat 2.1256, ibu_reg 2.6126, elec_energy 3.9596; same-bin interim total 6.2688.
- **Tail composition:** level-governed excursions g224 12.34 % (u_gen 1.70), g234 7.56 % (u_gen 0.26), g469 10.15 % (u_gen 2.16), g474 8.11 % (u_gen 3.60); gen-governed resolved tails g235 10.86 %, g236 9.85 %, g481 12.49 % (all kept under v2).
- **Brackets:** all-cell 5.9790/6.2875; D_raw-flag partition 6.0926/6.3932; per-bin-everywhere 6.2588/6.4654; corrected-test sweep flat at 6.2588/6.4654 across thresholds 2.0–2.4·`flux_stat_rel`; n>50 % = 1 (h+) / 3 (h−) in EVERY variant. Of the 24 big-|D_raw| unresolved bins, 5 (h+) / 6 (h−) resolve under the corrected test; their mean leg 5.14→3.70 / 5.88→4.70.

### 12.7 Provenance

Read-only adjudication; single-file login-node checks only (`validation_gammap.json`; two single-input-file python passes over `systematics_budget_gammap_16comp.csv`, seconds of wall; `sidis_highQ2/STATE.md` read for the 2026-10-16 user decision). The skill `sidis-hq2-systematics` was again not loadable in this runtime (precedent: memo header). No release artifact, table, budget, plot, script, STATE.md row, or note file modified; no Condor job submitted; the only file modified is this memo (§12 appended plus supersession pointers at §11.3, §11.4, §11.5).
