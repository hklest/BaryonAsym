# Method specification and implementation plan — σ(γ\*p) derived product via the Hand-convention virtual-photon flux (high-Q² charged-hadron SIDIS)

**Date:** 2026-10-16 · **Type:** binding method specification + implementation plan (read-only on all existing products; this file is the only artifact written by this task)
**Product specified:** the derived σ(γ\*p) sidecar on the current cone+S2′ RAW release — σ^{γ\*p}(bin) = σ^{ep,Born,γ-only}(bin) × ⟨1/Γ⟩(bin) = RAW(bin) × C_gamma_born(bin) × ⟨1/Γ⟩(bin), 490 bins (7 (Q²,x) cells × 7 z × 5 p_T, two charges).

**GOVERNING STATUS — OPTIONAL AUXILIARY (user decision 2026-10-16, `docs/methodology/ep_reporting_level_user_decision_2026-10-16.md`; STATE.md line 1):** the reporting level of this analysis is the **ep cross section**. The γ\*p program is optional auxiliary material; its pending items are **NOT blockers, NOT gated, and NOT required**. This specification freezes the method for (a) the already-built derived product and (b) any optional completion of the pending steps (§9, §10). No γ\*p blocker may be recorded anywhere.

**Consistency statement:** this spec contradicts none of: `docs/research/research-gammap-hand-convention_2026-10-15.md` (formula source of truth), `docs/radiative/note_crad_vs_gammaflux_and_R_2026-10-15.md` (C_rad/flux commute verdict, NoZ inventory, promotion path), `docs/methodology/map_ep_xsec_pipeline_touchpoints_2026-10-15.md` (producers/consumers/labels, Option-B design, risks R1–R9), `docs/audits/audit_gammaflux_inputs_2026-10-15.md` (branch availability), `docs/normalization/beam_energy_period_composition_2026-10-15.md` (s, beam config, per-cell y/ε), `docs/research/gammap-systematics-budget-2026-10-15.md` (§10 SWAP + §11 M2), `docs/research/gammap-conversion-prescription-2026-10-15.md` (estimator + rejections), or RUN_STATUS.md/STATE.md.

---

## 1. Scope and non-scope

**In scope.**
- ONE derived product: a post-unfold, per-bin multiplicative conversion of the current central **cone+S2′ RAW release** (2026-10-13) into a σ(γ\*p) release, implemented as a **derived sidecar in a NEW directory** (touchpoint-map Option B): `sidis_highQ2/data/gammap_derived/gammap_hand_cone_s2prime_photonborn_2026-10-15/`.
- Its two inputs built first as independent dated artifacts: the **per-bin effective-flux table** (`data/flux/hand_gammap_flux_releasebin_2026-10-15/`) and the **γ\*p systematic budget** (`systematics_budget_gammap_16comp.csv`, 17 components).
- The exact frozen conventions (§2), the frozen input set (§3), the build procedures (§4–§5), the uncertainty bookkeeping (§6), the labelling/provenance rules (§7), the validation gates G1–G12 (§8), and the ordered implementation steps with owners (§9).

**Out of scope / explicitly NOT done.**
- The ep chain is **NOT rebuilt, NOT repointed, NOT superseded**: the RAW release, the withZ Born release, the response, the unfolding, the ep legenv budget, the covariance toys, and the charge asymmetry are untouched (read-only inputs). The ep Born release remains the **primary product**.
- **No re-unfolding, no response rebuild, no y-binning** (the effective-flux identity converts each released bin exactly; explicit y-slicing was evaluated and rejected — it would fragment the 490-bin product and require a full re-unfolding chain for zero physics gain).
- The **withZ Born release is never divided by Γ** — that would produce a mislabelled "(γ\*+Z)p" object (integrated ≈ −8.8% Z/loop content); the label and the operation are both FORBIDDEN.
- **Multiplicities remain blocked** (no DIS denominator exists or is created); the γ\*p product is not a step toward them (R9).
- **No L/T separation, no σ_T extraction** (single beam energy ⇒ no Rosenbluth lever; R unmeasured; the ε-weighted-total convention makes R unnecessary — §2).
- **No absolute-normalization validation**: P-ABS-NORM (absolute hadron cross-section normalisation chain provisional) is inherited verbatim.
- MC generator curve conversions are governed by §6 rules (caveat-only for PYTHIA8/SHERPA3); no generator EW-mode audit is performed by this spec.

---

## 2. Exact definitions (frozen)

### 2.1 The conversion relation

> **σ^{γ\*p}(bin) = σ^{ep,Born,γ-only}(bin) × ⟨1/Γ⟩(bin) = RAW(bin) × C_gamma_born(bin) × ⟨1/Γ⟩(bin)  [nb·GeV², bin-integrated]**

- `RAW(bin) = N_Unf(bin)/L_data`, `L_data = 351000.0 nb⁻¹` (release driver, bin-integrated nb; no bin-width division at release level).
- `C_gamma_born(bin) = C_rad(bin) × F_EWgamma(bin)` — the pure-photon (γ-only) Born ep level at fixed α(0) in one step; C_rad contains **no flux factor** (verified: pure σ_nonrad/σ_rad yield ratio), so the Γ division is the **first and only** flux division in the chain — no double counting (commute verdict).
- σ^{γ\*p} is the **ε-weighted total**: **σ^{γ\*p} ≡ σ_T + ε̄ σ_L = σ_T(1 + ε̄R)**. R = σ_L/σ_T **drops out of the central value exactly**; no R leg exists (statement duty only: converting to σ_T costs 3.4–8.5% at plausible R — §6).

### 2.2 The flux (Hand / H1-house convention)

> **Γ(y,Q²) = (α_em(0)/π) · (1 − y + y²/2) / (y · Q²)  [GeV⁻²]** — density in the (y,Q²) measure

- Source of truth: H1, "Diffractive Electroproduction of ρ and φ Mesons at HERA", JHEP 1005 (2010) 032, arXiv:0910.5831, §5.1 Eqs. (9)–(10) (verified verbatim against the primary source). Algebraically identical to (α/2π)(1+(1−y)²)/(yQ²). ZEUS arXiv:hep-ex/0504010 §4 uses the identical convention.
- **Polarization parameter:** ε = (1−y)/(1−y+y²/2) ≡ 2(1−y)/(1+(1−y)²). Endpoints over the fiducial y window: ε(0.1) = 0.99448, ε(0.7) = 0.55046 → populated ε ∈ [0.550, 0.994]. The Trento γ²-exact ε (Bacchetta Eq. 8) differs by ≤ 5×10⁻⁶; all target-mass/electron-mass/convention terms (γ², Q²/ν², (1+γ²/2x), m_e) are ≤ 0.03% at Q² ≥ 150 GeV² — a documented bound, **not** a systematic leg.
- **α_em(0):** this spec freezes **α_em(0) = 1/137.035999084** (CODATA 2018). **As-built value:** 1/137.0359895 (generator-matched, user-locked 2026-10-15, recorded in the worker `ALPHA_EM_INVERSE` and the product `flux_convention`). Difference: the as-built α is larger by Δ = +6.99×10⁻⁸ relative ⇒ all γ\*p values shift by −6.99×10⁻⁸ relative (−0.000007%), a uniform factor that cancels in every internal identity gate and sits far below every threshold. **Record-only; no rebuild required.** Any future re-build uses the spec-frozen value and records the change.
- **σ-normalization bookkeeping (frozen once):** the H1-house (Q²-convention) normalization — σ_T = (4π²α/Q²)·2xF₁, σ_L = (4π²α/Q²)·F_L, hence σ_T + σ_L = (4π²α/Q²)·F₂. Hand-K_H-normalized tables differ by the pure factor 1/(1−x) (up to +17% at populated x ≈ 0.15 in S2_x1; ~+4% at x ≈ 0.034); any comparison against Hand-K_H tables must apply that rescale. This is a convention statement, not an uncertainty.
- **s:** fixed H1OO constant **s = 101,568.0 GeV²** (√s = 318.697 GeV; 4·27.6·920) — the self-consistent choice because the release eΣ kinematics (`Yes`/`Q2es`/`Xes`) are themselves defined at the fixed constant. Single beam configuration (e± 27.6 × 920 GeV); **no MER (575)/LER (460) period in the sample** (0 of 20,788 GRL runs) — the mixed-beam-energy flux problem is moot. The worker evaluates per-event s = 4·`GenBeamElecE`·`GenBeamProtP`; the residual beam-spread effect on ⟨1/Γ⟩ (≤ ±0.5%) is absorbed by `flux_model_rel` (§6).

### 2.3 The effective-flux estimator (and its weight)

> **⟨1/Γ⟩(bin) = Σᵢ (wᵢ / Γ(yᵢ, Q²ᵢ)) / Σᵢ wᵢ  [GeV²]**, with **wᵢ = weight1ᵢ × weight2ᵢ**

- Population: the **DJANGOH14 NoZ photon-only pure-Born e⁻ truth sample** (the C_gamma_born numerator sample — level-matched to the σ_ep being divided; at γ-only Born tree level e⁻/e⁺ are charge-symmetric, so the e⁻ truth validly weights the mixed-beam data).
- Γ is evaluated at the **rad-safe gen triple (Y_es_gen, Q2_es_gen)**; the branch-choice sensitivity vs the eΣ-native triple (`YesGen`, `Q2esGen`) is ≤ 0.014% (pure-Born sample).
- **Per gen charged-hadron row** (~22 rows/event), **no event deduplication** — the bin is a hadron-level (Q²,x,z,p_T) bin and σ_γ\*p(B) = σ_ep(B)·Σ_rows w/Γ / Σ_rows w holds exactly with σ_ep(B) in the CγB numerator convention (per-bin n/sumw bit-identical to the campaign tables, gated).
- Selection **mirrors `crad_v3_worker_releasebin.py` lines 118–142 exactly**: w = weight1×weight2; DIS fiducial 150 ≤ Q²_es_gen < 20000 and 0.1 < Y_es_gen < 0.7; `GenEmpz > 45`; π/K/p-only hadron mask (truth pool discipline); **recomputed xF < 0** (q-nominal, current hemisphere); cell assignment via (`Q2esGen`, `x_es_gen`) — the release-safe binning of the C_rad/CγB campaign. Worker invocation: `--sample nonrad --variants nominal --empz-modes empz_gt45` (the exact 2026-10-09 NoZ campaign mode).
- This estimator makes the conversion an **exact identity** under the one-photon factorization dσ_ep = Γ·dσ_γ\*p (per dy dQ² dz dp_T at fixed (Q²,x)) — the only residual systematics are the weighting model (`flux_model_rel`) and its MC statistics (`flux_stat_rel`), §6.

### 2.4 Quantified rejections (frozen; no re-litigation)

| Alternative | Bias vs ⟨1/Γ⟩ | Verdict |
|---|---|---|
| Bin-center Γ (geometric centers) | −14% … +72% (per cell −13.9/+69.0/−22.5/+70.6/−18.8/+71.8/−0.7%) | REJECTED — quantified out (R4/R11) |
| Bin-center Γ (log centers) | −29% … +56% | REJECTED |
| 1/Γ at the cell's measured effective (⟨Q²⟩,⟨y⟩) | Jensen gap +4% … +14% (⟨1/Γ⟩/1/Γ(eff) = 1.040–1.142) | REJECTED as central value; retained as the G3 band |
| Count-weighting (Σn instead of Σw) | up to **+12.1%** (S0_x1; per cell +6.98/+12.14/+2.75/+10.25/+0.00/+1.63/+0.00%) | REJECTED — the single biggest trap (R10); computed only as the anti-trap diagnostic |
| Explicit y-binning (6 campaign y-bins × 7 cells) | kinematically empty slices; full re-unfolding required | REJECTED — zero physics gain |
| σ_T-only product | needs unmeasured R (3.4–8.5% penalty) | REJECTED for the central product; ε̄ sidecar + documented penalty instead |
| Trento structure-function normalization (Bacchetta Eq. 7) | different observable/units, not a photoabsorption σ | REJECTED as the release convention |
| Dressed NOZ1 level instead of pure Born α(0) | ≈ +8.6–9% integrated (NOZ1/NOZ = 7.2859/6.7086 nb) | Convention substitution, recorded decision, never a systematic |

### 2.5 The observable, its measure, and the ε̄ sidecar

- **Binned observable (primary released form):** σ_gammap(bin) in **nb·GeV²** — the γ\*p photoabsorption cross section σ_γ\*p(z,p_T;Q²,x) integrated over the bin region **in the frozen (y,Q²,z,p_T) measure** (the measure in which the H1 VM formula divides by Γ). Freezing the measure is mandatory: the (x,Q²)-measure variant needs the ⟨x/(yΓ)⟩ factor — an O(1) difference; the two measures must never be mixed (R1/R6). The (x,Q²)-measure factor is carried per bin **only** as the labelled diagnostic column `x_over_y_invGamma_mean_gev2`.
- **4D differential density (emitted):** d⁴σ^{γ\*p}/dy dQ² dz dp_T **[nb/GeV]** = σ_gammap(bin) / (A_yq2_clipped · Δz · Δp_T), with **A_yq2_clipped = ∫∫_{R; 0.1<y<0.7; x<1} Q²/(x²s) dx dQ² [GeV²]** (exact closed form; s = 101,568.0 GeV²). A per-(x,Q²)-volume density [nb/GeV³] is **NOT emitted** — it would silently change the measure convention (R6). **x-cap rule:** if an (x,Q²)-measure render is ever produced, the S3_x0 bookkeeping width (x_max = 1.5) must be capped — **Δx = 1.0 − x_min (0.979)** — exactly as the ep LaTeX/table generators enforce; the binned nb·GeV² form sidesteps the split entirely and is the recommended presentation.
- **ε̄ sidecar (mandatory, shipped in the flux table):** per-bin/per-cell effective kinematics — ⟨Q²⟩, ⟨y⟩, ⟨x⟩, ε̄, Γ min/max/spread, ⟨s⟩ (as-built `eff_q2_gev2`, `eff_y`, `eff_x`, `eff_eps`, `eff_s_gev2`, `gamma_*`, `eps_min/max`). Production per-cell values (w-weighted, full sample): ε̄ = 0.816/0.975/0.824/0.976/0.815/0.975/0.881 (S0_x0…S3_x0, charge-averaged); ⟨Q²⟩ = 197.7/214.9/412.8/467.3/936.0/1036.7/3034 GeV²; ⟨y⟩ = 0.458/0.188/0.446/0.184/0.459/0.190/0.348. This sidecar is the only mitigation for S3's 137× in-bin Γ spread (R13) and the required input for any consumer σ_T conversion.
- **F₂ route identity (cross-check basis):** σ^{γ\*p} = σ_T + εσ_L = (4π²α/Q²)·[F₂ − (1−ε)F_L] = **(4π²α/Q²)·σ̃_r** — exact at γ→0; the H1 Eq. (10) flux is nothing but the reduced-cross-section factorization (gate G8).

---

## 3. Required inputs (absolute paths; all read-only)

| # | Input | Absolute path | Count / status |
|---|---|---|---|
| 1 | **Parent RAW release (cone+S2′)** — the σ base | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` | `ibu/` + `bbb/` cross_sections CSV/JSON (IBU h+ 11.412233 / h− 11.123626 nb); `covariance/` **yield-unit** 100-toy matrices + dimensionless correlations; `stat_sidecar_1000toy/` (staging, NOT installed); `bin_map.csv` 490 rows (charge, bin_index, global_bin_index, cell_label, q2/x/z/pT edges — **no y, no bin centers**); metadata/summary. sha256 of `ibu/cross_sections.json` recorded in the derived metadata (`cedeed10…`). |
| 2 | **withZ Born release** — alignment/identity gates ONLY, never divided by Γ | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` | `ibu/`+`bbb/` (IBU h+ 13.026987 / h− 12.697511 nb); **`alternative_scheme_rapgap31_relbin_emfix/`** twin (RG31-emfix Born alternative, same schema); `covariance/` yield-unit sidecars with `born_covariance_recipe` `Cov_Born[i][j] = C_i·D_ij·C_j`, `C_i = C_rad(bin_i)/L_data`; `bin_map.csv`; `SELECTION.md` (contract). |
| 3 | **NoZ C_gamma_born table** (the applied EW factor) | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_490rows.csv` | 490 rows (245/charge), release-safe (`Q2esGen`,`x_es_gen`), GenEmpz>45, recomputed xF<0 both legs; columns incl. `C_gamma_born`, `F_EWgamma`, `C_rad_mixed_published`, `sigma_gamma_born`, `sigma_rad_mixed`, `relative_stat_unc`, `gamma_born_n`, `identity_residual`. Integrated h+ 1.040960540463156 / h− 1.0401468080806677; per-bin stat **median 0.551% (charge-pooled), max 7.84%** (the flux-stat scale reference, §6); F_EWgamma per-bin 0.785–0.991, integrated 0.9116/0.9113. Status: **PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`, DJANGOH-only** — applied here as the derived product's own factor with the caveat carried. sha256 `7ec901af…`. Companions in the same dir: `..._yresolved_2940rows.csv` (6 y-bins × 490; feeds u_level), `cgammaborn_gen_unc_proxy_v3emfix_490rows.csv` (`u_model_rel_k002`; feeds `c_gamma_gen_rel`), validation JSONs, exact-exposure provenance (`L_MC = 14,119,125.013920162 nb⁻¹`, EXACT_OOMCLUMI_SWEEP PASS). |
| 4 | **DJANGOH14 NoZ truth ROOT files** (flux-build input) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/nozborn_em_0000.root … nozborn_em_0259.root` | **260 files** (count verified). Tree `ChargedGenRC` (84 branches) + `GenDIS` (event level). **Branch availability verdict (audit 2026-10-15): every required branch present — no builder change.** Weights `weight1`, `weight2` (weight2 ≡ 1, hard-gated); rad-safe triple `Q2_es_gen`/`Y_es_gen`/`x_es_gen`; eΣ-native `Q2esGen`/`YesGen`/`XesGen` (cell assignment); `GenBeamElecE`/`GenBeamProtP` (per-event s); `GenElecPx/Py/Pz/E` (alternative q); `GenEmpz`; hadron `GenPx/Py/Pz/E`, `GenPDG`, `GenCharge`, `IsGtr`, `Mother2`, `IsV0DaughterGen_v3`, `VetoAncestorPDG_v3`. **No** ε/Γ/W/√s branch exists anywhere (release tables carry no y either) — per-event Γ must be computed, which these branches fully support. Input semantics (gated in the worker): per-gen-charged-hadron rows; two `ChargedGenRC` cycles per file (latest read, `multi_cycle_files` = 260); four chunk-trailer files (0064/0129/0194/0259) with a −999 sentinel `GenDIS` entry and zero hadron rows (sentinel pattern hard-gated out); L_MC cross-check Σw1w2 = 94,718,960.74 over 40,000,004 GenDIS events → 14,119,121.7 nb⁻¹ (−2.4×10⁻⁷ vs the exposure JSON). |
| 5 | **ep legenv budget** (source of the 13 carried legs) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/total_uncertainty_summary_cone_variant_15comp.csv` | 490 rows; medians h+ 5.8032% / h− 6.1919%; sha256 `568040fe…`. |
| 6 | **RG31 RAD emfix sample** (M2 `u_gen` measurement) | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/flux/hand_gammap_flux_rg31_rad_emfix_releasebin_2026-10-15/` | 1273 files (582 e⁻ + 691 e⁺), 128 shards, cluster 5671056, 437.7M selected entries, 490/490 bins, 0 fallbacks; comparison JSON vs the DJ table (per-bin \|RG−DJ\| median 1.04%, max 12.30%). |
| 7 | **Normalization constants** | — | `L_data = 351000.0 nb⁻¹` (four-period S67 target; flat 2.7% `lumi_rel`); s = 101,568.0 GeV²; single beam config 27.6 × 920 GeV (no MER/LER). |

Release-table fact governing the design: the tables carry **no y, ε, W, s columns** — the per-bin Γ is necessarily MC-derived (audit §1), which is why input #4 exists.

---

## 4. Flux-table build procedure

**Algorithm (worker `compute_inverse_flux_releasebin.py`, smoke-validated).** For each of the 260 NoZ files: read the latest `ChargedGenRC` cycle; apply the mirrored selection (§2.3); per selected gen charged-hadron row compute w = weight1×weight2, s = 4·GenBeamElecE·GenBeamProtP, y = `Y_es_gen`, Q² = `Q2_es_gen`, and Γ(y,Q²) with the frozen α; accumulate per global bin (490): `n`, `sum_w`, `sum_w_invGamma` (plus second-moment terms for the stat, and per-(bin,y) arrays for the y-resolved diagnostic sidecar); hard-gate weight2≡1, sentinel patterns, non-finite kinematics. Merge: ⟨1/Γ⟩ = Σ(w/Γ)/Σw per bin; effective kinematics (w-weighted ⟨Q²⟩,⟨y⟩,⟨x⟩,ε̄,⟨s⟩); Γ min/max/spread; count-weighted and bin-center variants as **diagnostics only**.

**File list / sharding.** 260 files → **26 shard manifests × 10 files** (`queues/manifest_shard00..25.tsv`).

**CPU / Condor shape — Condor is MANDATORY** (260 files ≫ the login-node ≤1-file/≤5-min policy). Measured: 11.3 s/file (NoZ), ≈100 s per 10-file job, peak RSS 2.2–2.3 GB. Submit parameters: `request_cpus=1`, **`request_memory = 4000`**, **`+RequestRuntime = 1800`**, `retry_until = (ExitCode != 127)`, `max_retries = 5`, `should_transfer_files = NO`. **HTCondor 25 trap:** `manifest` is a **reserved submit command** — the RUN_PLAN B.2 verbatim queue variables were rejected by HTCondor 25.0.8; the repaired pattern renames the queue variables (`mfile/outdir/tag`) with everything else byte-identical. Production clusters: 5670068 (10-file smoke, 1 job) + 5670130 (25 jobs); 26/26 exit 0.

**Merge gates (hard; exit 2 on failure).**
1. **490/490 bins measured** — no `--allow-empty-fallback` in production (single-file fixture only). The two bins empty in the single-file fixture (global 209, 453) are populated in the full sample.
2. **n-identity:** per-bin n == `gamma_born_n` of the CγB table, exact integer identity, 0 mismatches (a mismatch is a selection mismatch → hard stop, escalate).
3. **Cell-mean gate:** per-cell ⟨1/Γ⟩ within **2%** of the committed single-file anchors 63,296.63 / 22,023.99 / 128,791.52 / 47,753.01 / 301,942.47 / 107,718.98 / 734,613.15 GeV² (S0_x0…S3_x0). Never silently loosen.
4. **Anti-trap:** emission identity ≤ 1e-12; count-weighted and bin-center variants present as diagnostics only.
5. **Census:** Σ selected entries == **200,825,440** (h+ 101,222,484 / h− 99,602,956); Σ input rows == 890,876,981; `multi_cycle_files` == 260; `weight2_not1_rows` = `sentinel_rows` = `nonfinite_*` = 0.

**Output schema — one row per global_bin_index × charge (490 rows).** Required content (spec name → as-built column):

| Required | As-built column | Definition |
|---|---|---|
| global_bin_index + charge (+ cell, q2/x/z/pT edges) | `bin_index` (charge-local 0–244) + `charge` (h+ 0–244 / h− 245–489 global) + `cell` + edge columns | join key, identical to the CγB/bin_map key |
| **Gamma_bar** [GeV⁻²] | derived = 1/`invGamma_mean_gev2` (exact reciprocal; MUST be present or documented as such) | Γ̄(bin) = 1/⟨1/Γ⟩(bin) |
| **inv_Gamma_bar** [GeV²] | `invGamma_mean_gev2` | ⟨1/Γ⟩ = Σ(w/Γ)/Σw |
| **epsilon_bar** | `eff_eps` | w-weighted ε̄(bin) |
| **effective y** (+⟨Q²⟩, ⟨x⟩, ⟨s⟩) | `eff_y`, `eff_q2_gev2`, `eff_x`, `eff_s_gev2` | w-weighted |
| **n_events** | `n` | selected **gen charged-hadron entries** (per-hadron rows ≈ 22/event — NOT DIS events; state this wherever quoted) |
| **w_sum** | `sum_w` | Σ weight1×weight2 |
| **rel MC stat error** | `invGamma_stat_rel` | weighted-mean variance of 1/Γ (feeds `flux_stat_rel`) |
| diagnostics | `invGamma_count_mean_gev2` (anti-trap), `x_over_y_invGamma_mean_gev2` ((x,Q²)-measure factor), `gamma_min/max_1_per_gev2`, `gamma_spread`, `eps_min/max`, `invGamma_source` | never central values |

**Location:** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/flux/hand_gammap_flux_releasebin_2026-10-15/` — `gamma_flux_hand_inverse_fullsample_nozborn_em_260files_2026-10-15.csv` (+ `.json`, `_yresolved.csv`, `STATUS_flux_batch_2026-10-15.json`, `partials/`, `logs/`, `queues/`). **Production values (landed 2026-10-15):** per-cell ⟨1/Γ⟩ = 63,107.6 / 22,121.5 / 128,577.1 / 47,608.9 / 300,548.9 / 108,402.1 / 724,820.1 GeV² (max dev vs single-file 1.33%, inside the 2% gate); per-bin span ≈ 1.6×10⁴ – 2.9×10⁶ GeV²; ε̄ per cell 0.814–0.976.

---

## 5. Conversion procedure (the derived release)

**Builder:** `sidis_highQ2/scripts/gammap_2026-10-15/build_gammap_release.py` (derived-sidecar pattern per `build_data_side_noz_photonborn_2026-10-13.py` + the exact-text Born-builder precedent). Inputs read-only; output a NEW directory **outside** `output_v6/central/data_release/` and `data/release/`.

**5.1 Per-bin factor.** `gammap_factor_nb_gev2(bin) = C_gamma_born(bin) × ⟨1/Γ⟩(bin)` — shared by all (z,p_T) bins of a cell only through Γ's cell kinematics; strictly per-bin in the product.

**5.2 Columns scaled — the 8 absolute-nb columns, 1:1, round_trip exact-text.** `cross_section_nb, stat_unc_nb, analytic_stat_unc_nb, model_unc_nb, reg_unc_nb, toy_stat_unc_nb, total_unc_nb, total_unc_nb_diag` → `sigma_gammap_bin_nb_gev2, stat_unc_gammap_nb_gev2, analytic_stat_unc_gammap_nb_gev2, model_unc_gammap_nb_gev2, reg_unc_gammap_nb_gev2, toy_stat_unc_gammap_nb_gev2, total_unc_gammap_nb_gev2, total_unc_gammap_nb_gev2_diag`. The **ep columns are carried byte-identical in the same CSV** (the ep release itself is untouched); the γ\*p columns are added alongside.

**5.3 Columns unchanged, and why.** All yields (`yield_unfolded`, `*_unc_yield`) **byte-identical** (YIELD POLICY CHOICE-A): the relation `yield = cross_section_nb × L_data` is already deliberately broken in the Born release and is **triply broken** here (`yield ≠ σ_gammap × L_data × Γ̄`) — the `raw_yield_vs_gammap_cross_section_warning` metadata key is mandatory. All **relative** columns (`total_unc_rel`, the 13 carried budget legs, acceptance/prior sidecars) unchanged — the per-bin factor f cancels in every ratio; the **charge asymmetry is unchanged** (Γ depends only on DIS kinematics, not hadron charge). Statistical treatment: per-bin propagated stat = `toy_stat_unc_gammap_nb_gev2` (the release-authoritative in-tree 100-toy stat leg × f; the 1000-toy sidecar is staging, not installed); relative statistical uncertainty unchanged.

**5.4 Density.** `d4sigma_gammap_dy_dq2_dz_dpt_nb_per_gev` = σ_gammap(bin)/(A_yq2_clipped·Δz·Δp_T) with the §2.5 clipped area (`yq2_area_clipped_gev2`, `bin_volume_yq2zpt_gev3` columns). **No nb·GeV⁻³ column on any measure** (validation-recorded): σ_gammap carries ⟨1/Γ⟩ [GeV²], so every 4D density of it is nb/GeV; the only nb·GeV⁻³ object recoverable is the ep-level photon-Born density, which is the parent's, not a γ\*p quantity.

**5.5 Covariance — diagonal congruence on the yield-unit basis.** The parent `covariance/` matrices are in **unfolded-yield² units** (`units: "unfolded_yield_counts"`; `born_covariance_recipe` C_i = C_rad(bin_i)/L_data). The derived covariance is

> **Cov_γ\*p[i][j] = g_i · Cov_ep,RAW[i][j] · g_j**, with **g_i = C_gamma_born(bin_i) · ⟨1/Γ⟩(bin_i) / L_data**  [(nb·GeV²)²]

— i.e. the one-step touchpoint-map notation **C′_i = C_gamma_born(bin_i)/(L_data·Γ_i)** applied to both indices (the C_i/(Γ_iΓ_j) congruence), on the yield-unit parent basis. The **1/L_data factor is mandatory** (QA remediation 2026-10-15 finding 1: its omission inflated the absolute scale by L_data² = 1.23×10¹¹; correlation and the seven CSV uncertainty columns were unaffected). Correlation matrix unchanged (copied verbatim). Hard gates: diag == `toy_stat_unc_gammap_nb_gev2`² (≤1e-12 rel), correlation preserved (≤1e-10), ordering vs the release stat column (≤1e-9), lumi hard-gated against the parent payload (351000.0). Rank deficiency (numerical rank 99/245, negative eigenvalues ~1e-17 rel) is inherited from the 100-toy parent: **inversion/fitting use FORBIDDEN until the 1000-toy sidecar is installed; correlation-only for shape comparisons.**

**5.6 Integrated-value recomputation (acceptance anchors, all verified as-built).**
RAW 11.412233 / 11.123626 nb → ×`C_rad_mixed_published` = **13.026987 / 12.697511 nb** (reproduces the withZ Born release exactly — the parent-alignment gate) → ×`C_gamma_born` = **11.875722 / 11.571280 nb** (photon-only ep Born; integrated CγB_eff 1.0406/1.0402; F_EWgamma σ-weighted 0.911532/0.911221) → ×⟨1/Γ⟩ = **1,163,513.70 / 1,125,476.59 nb·GeV²** (γ\*p, full-sample flux). σ-weighted ⟨1/Γ⟩ = 97,974.1 / 97,264.7 GeV². Single-file prototype anchors 1,170,035 / 1,129,842 nb·GeV² (gate ±1.5%; full-sample shift ≲1.3%). **The γ\*p analogue of the ep 13.03/12.70 nb sums is the per-bin-first sum above — never (13.03 nb)÷Γ** (three independent reasons: withZ content, per-cell Γ span 2.2×10⁴–7.4×10⁵ GeV², nb·GeV² measure).

**5.7 Output location.** `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/gammap_derived/gammap_hand_cone_s2prime_photonborn_2026-10-15/` — `gammap_cross_sections.csv`/`.json`, `covariance/gammap_toy_covariance_{pos,neg}.json`, `systematics_budget_gammap_16comp.csv`, `systematics_charged_sidis_highQ2_gammap.json` (release contract), `bin_map.csv`, `metadata.json`, `README.md`, `validation_gammap.json`, `superseded_budget_transferred_2026-10-15/` (superseded pointer per workspace convention).

---

## 6. Uncertainty bookkeeping

**The γ\*p budget is a 17-component budget (13 carried + 2 swap + 2 flux) — it is NOT the ep 15-component budget and must never be mixed with it.**

**6.1 Carried bit-identical (13 legs).** `ibu_stat_rel, ibu_model_rel, ibu_reg_rel, hfs_jes_rel, hfs_rces_rel, hfs_theta_rel, elec_energy_rel, elec_theta_rel, lumi_rel` (flat 2.7%, separately reported in publication tables), `tracking_eff_m0p5_rel, phi_h_gauss_rel, detint_removal_rel` (+`detint_removal_model_rel`/`_stat_rel` subcolumns), `fakecol_mcstat_rel` — bit-identical from the ep legenv budget (the per-bin conversion factor cancels in every relative leg; gated: 6370 cells, 0 mismatches). Absolute-nb budget columns rebase exactly into γ\*p units (max dev 1.55e-15).

**6.2 The two C_rad legs — SWAP (never ADD), relabel REJECTED.** The applied factor here is C_gamma_born = C_rad × F_EWgamma, so the budget must quantify **that** factor:
- `c_gamma_gen_rel`(b) = sqrt(`v3_emfix_rel`² + [κ·(1−`F_EWgamma`(b))]²), **κ = 0.02** central (κ = 0.03 variant documented only, never budgeted) — **REPLACES `c_rad_gen_rel`** (medians h+ 0.6223% / h− 0.6189%, max 5.77/5.67%). SWAP, never ADD: the proxy's first term IS the WithZ DJ/RG emfix spread (the exact content of `c_rad_gen_rel`); the κ-term is the anchored EW-generator dependence of the projection (unmeasurable — no RAPGAP NoZ sample; IPRO=1200 has no Z-off switch). Fully correlated (one generator-scheme decision).
- `c_gamma_stat_rel`(b) = the NoZ-table `relative_stat_unc` = `C_gamma_born_stat_unc`/`C_gamma_born` (the FULL per-bin stat of the applied factor, numerator-dominated: NoZ e⁻ pool min 237 / median 50,537 weighted entries; medians h+ 0.5442% / h− 0.5721%, max 5.87/7.84%) — **REPLACES `c_rad_stat_rel`** (medians 0.4272/0.4429%). SWAP, never ADD: the RAD-mixed denominator Poisson is common to both ratios; addition double-counts it, replacement counts every pool once. Binwise uncorrelated. **Tail policy: the 7.84% sparse-bin tail stands — NO cap, NO merge into `ibu_stat_rel`** (1000-toy DATA statistics vs applied-factor MC statistics are distinct, uncorrelated objects).
- History (binding): the initial as-built "relabel + informational" form was **REJECTED** by the 2026-10-15 QA adjudication (budget-spec §10: a relabelled leg asserting the ep-level content zeroes the F_EWgamma projection's generator uncertainty — the "assume zero" pattern); the budget was rebuilt in place the same day with the true SWAP.

**6.3 `flux_model_rel` — model dependence of the ⟨1/Γ⟩ weighting. FINAL form (budget-spec §11, M2 measured 2026-10-15/16):**
> **flux_model_rel(b) = max( u_level(b), u_gen(b) )** [per-bin ENVELOPE — frozen before measurement, no post-hoc rule change]
> u_level(b) = |δ_level(b)|, δ_level from the DJ-internal synth-vs-synth y-resolved level ratio (`cgammaborn_..._yresolved_2940rows.csv`; per-cell anchors +0.93/+1.00/+0.54/+0.51/+0.46/+0.56/+1.75%)
> u_gen(b) = | G_RG31(b) / (G_DJ(b)·(1+δ_level(b))) − 1 | (level-corrected generator component; RG31 RAD emfix table vs the applied DJ table, global-index join)

- Measured per-cell envelope: **0.93 / 1.00 / 1.19 / 1.36 / 1.73 / 1.73 / 1.75%** (S0_x0…S3_x0); the generator component governs in S1/S2 (the interim level-only leg under-covered by up to ~3.8×). Raw |RG−DJ|: median 1.04%, max 12.30% (bin 189, 21σ above the combined flux-stat noise — real model dependence, resolved). Fully correlated whole-vector (one flux-weighting-model decision). NO cap anywhere; unresolved (stats-limited) bins KEEP |u_gen|, flagged `flux_model_stats_limited`. Informational columns: `flux_model_u_level_rel`, `flux_model_u_gen_rel`, `flux_model_resolved_flag`; conservative upper variant `flux_model_upper_raw_rel` = |D_raw| (documented, never central).
- **AS-BUILT state:** the current `systematics_budget_gammap_16comp.csv` still carries the **interim per-cell u_level** (7 distinct values 0.46–1.75%, flag `generator_component_unevaluated_interim_bound`); the §11 final per-bin envelope rebuild is **PENDING — optional, not gated** (§10 open items). The beam-spread effect (≤±0.5%) is absorbed here.
- Distinctness duty: `flux_model_rel` (in-bin y-population model of G) vs `c_gamma_gen_rel` (EW projection factor) are distinct objects — **ADD in quadrature, never merge** (the "no generator cross-check" caveat is superseded for the flux-weighting part by u_gen; the CγB DJANGOH-only caveat REMAINS for the EW projection).

**6.4 `flux_stat_rel` — MC statistics of ⟨1/Γ⟩ (gate FIRED → mandatory leg).** Per-bin `invGamma_stat_rel` from the 260-file table; the >0.1% carry-gate **FIRED** (medians h+ 0.2653% / h− 0.2786%, max 4.19/7.16%), so folding into the total is MANDATORY. **Scale reference:** the same NoZ sample's CγB per-bin stat — charge-pooled **median 0.551%, max 7.84%** — i.e. the flux-stat medians sit at ~half the same-sample applied-factor stat and the max below its max, consistent with shared-sample statistics; `flux_stat_rel` (flux-weighting stat) and `c_gamma_stat_rel` (applied-factor stat) are distinct objects, **ADD in quadrature**. Binwise uncorrelated.

**6.5 The R-term — absent by construction.** σ_γ\*p ≡ σ_T + ε̄σ_L makes R drop out of the central value exactly; **no R leg exists**. Statement duty: any σ_T conversion by a consumer uses the ε̄ sidecar at a documented penalty (3.4–8.5% at R = 0.15±0.10 or R = 0.25±0.05; expected R_SIDIS ~ 0.03–0.10 at our Q²). σ_T-only requests are declined for the central product.

**6.6 Totals.** `total_with_photon13_rel` = sqrt(`total_with_phih_rel`² + `c_gamma_gen_rel`² + `c_gamma_stat_rel`²) (REPLACES `total_with_crad13_rel`; rename mandatory — content changed) → `total_with_cone15prime_rel` (REPLACES `total_with_cone15_rel`) → **`total_with_gammap16_rel` = sqrt(`total_with_cone15prime_rel`² + `flux_model_rel`² + `flux_stat_rel`²)** — the headline and the contract-JSON `errors` array. As-built (2026-10-15 §10 rebuild, interim flux leg): medians **h+ 5.8550% / h− 6.2845%** (design anchors 5.8558/6.2857; deviations −0.0008/−0.0012 pp — REPORT-level agreement). §11 design estimate for the final-leg rebuild: h+ ≈ 5.86% / h− ≈ 6.29% (median migration only). Validation expectations: dominant-at-median `ibu_stat_rel` both charges; total/stat ∈ [0.5, 5.0] (≈1.6); >50% instability bin set == the parent's 4 bins.

**6.7 Not legs (never budgeted).** Flux-convention differences (Hand vs Gilman/Williams vs Trento-exact: ≤0.03%); y/bin-center convention (quantified out — its residual IS `flux_model_rel`); α(0)-pure vs dressed NOZ1 (convention substitution); R (statement duty); per-beam/per-charge photon legs (exact tree-level charge symmetry, NoZ e± = 218.84216 pb both).

**6.8 MC-curve conversion rules (consumer side).** All four generator curves' γ\*p partner is **F_EWgamma(bin) × ⟨1/Γ⟩(bin) ONLY** — applying C_rad or C_gamma_born to ANY MC curve is FORBIDDEN (CγB pairs the DATA RAW side only). PYTHIA8.317/SHERPA3.0: conversion APPROXIMATE (generator-internal α/loop conventions; Z-content unverified) — mandatory caveat on every plot/table, theory-side band (κ = 0.03 term) on MC curves only, never on data, never in the data budget. MAPTMD24: photon-only ep level — ×⟨1/Γ⟩ alone (the ~10⁵ GeV² rescaling is mandatory).

**6.9 Inherited status caveats (carried verbatim).** C_gamma_born PROVISIONAL DIAGNOSTIC (DJANGOH-only, e⁻ numerator / mixed-beam denominator, `c_rad_applied=false` in the source); P-CRAD-LUMI (file-set-reconciled; generator-log closure blocked externally); P-ABS-NORM; 100-toy incumbent covariance. Publication-blocking if ever promoted (§10.5 items 6–7 of the budget spec); internal showing fine with labels.

---

## 7. Provenance and labelling rules

**7.1 Mandatory metadata keys** (in the derived `metadata.json`; as-built values in parentheses):
- `schema`, `method` ("ibu"), `variant`, `release_label`, `response_variant_id`, `created_date`
- **`level` / `derived_level`**: "gamma\*p reduced (derived sidecar): sigma_gammap = RAW × C_gamma_born × ⟨1/Γ⟩; the ep Born release remains the primary product"
- **`flux_convention`**: "Hand/H1 arXiv:0910.5831 Eq. 10: Gamma = (alpha_em/pi)\*(1−y+y²/2)/(y\*Q2)" **plus the α(0) value and its source** (as-built "alpha(0)=1/137.0359895"; spec-frozen 1/137.035999084, CODATA 2018 — §2.2)
- **`parent_basis`**: `parent_release` + `raw_parent_release` (absolute path of the cone+S2′ RAW release, **no placeholder**) + `parent_release_sha256`
- **`observable_basis`**: cone alphaveto RAW data vector (label `raw_data_no_bgsub_alphaveto_cone`); **`prior_basis`**: coherent `body+miss` response truth marginal from the CONE+S2′ variant rapgap31 response (the central and no-cone S2′ marginals are FORBIDDEN); `cone_status`
- **`c_gamma_born_applied`**: true (this derived product), with the source-table status recorded verbatim (PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`, DJANGOH-only)
- `sigma_gammap_definition` (the §2.1 line + estimator + sample); `unit_statement` (measure, nb·GeV², the A_yq2_clipped density form, the no-nb/GeV³ rule); `gammap_covariance_recipe` (the §5.5 congruence with g, the 1/L_data, the hard gate); `raw_yield_vs_gammap_cross_section_warning` (CHOICE-A)
- `lumi_nb` (351000.0, hard-gated against the parent payload); `stat_error_statement`
- `cgammaborn_csv` + sha256; `flux_csv` + sha256; `budget_csv` + sha256; `kappa_csv` + sha256; `noz_yresolved_csv` + sha256; `flux_yresolved_csv` + sha256
- `gammap_caveats` (array; includes the MC-curve rule, the measure rule, the provisional-status inheritance); `level_leg_swaps` (the two SWAP records with `replaced_leg`/`replacement_basis`/`swap_rule`); `flux_model_rel`/`flux_stat_rel` records (form, flag, gate, per-cell anchors, stats); `gammap_budget_label` (the §7.2 never-mix text verbatim); `budget_contract`
- `qa_remediation_*` records; superseded pointers.

**7.2 Quote-rule additions (the four-axis never-mix rule).** The chain already enforces prior basis, Born-vs-raw level, and observable basis. γ\*p adds the fourth axis: **flux convention**. Any γ\*p quote (table, figure, note, talk) MUST state: (i) the flux convention — Hand/H1-house, arXiv:0910.5831 Eq. (10), with the α(0) value; (ii) the parent basis — cone+S2′ RAW release 2026-10-13; (iii) the EW level — photon-only Born at fixed α(0) via C_gamma_born; (iv) the measure — (y,Q²,z,p_T), binned nb·GeV². **Quotes omitting the flux convention and the parent basis are FORBIDDEN.** γ\*p totals must never be quoted against or mixed with ep totals. The "(γ\*+Z)p" mislabel (dividing the withZ Born release by Γ) is FORBIDDEN. Measure mixing ((y,Q²) vs (x,Q²)) is FORBIDDEN. The verbatim never-mix label text (budget-spec §10.3, updated by §11.4 for the measured flux leg) ships in the release README, the budget label, and on every table/figure showing the budget.

**7.3 Supersession discipline.** Superseded artifacts are retained under `_superseded_`/`superseded_*` markers with one-line pointers (as-built: `superseded_budget_transferred_2026-10-15/POINTER.txt`); the ep Born release's own superseded bases remain untouched provenance.

---

## 8. Validation gates G1–G12 (pass criteria; as-built outcome)

| Gate | Criterion (PASS requires) | As-built (2026-10-15/16) |
|---|---|---|
| **G1 — Schema & key alignment** | 490 rows (245/charge) in RAW, CγB, flux, budget, and derived tables; 8-edge alignment 0 mismatches; x-cap bookkeeping rule respected ({1.0, 1.5} in bin_map, 1.0-cap in any density); budget `nominal_cross_section_nb` == RAW per bin (≤1e-9) | PASS (edge mismatches 0; budget-vs-RAW max dev 1.55e-15) |
| **G2 — Census & selection identity** | per-bin n == `gamma_born_n` (490/490 exact); Σ selected == 200,825,440; Σ input rows == 890,876,981; `multi_cycle_files` == 260; `weight2_not1_rows` == `sentinel_rows` == `nonfinite_*` == 0; file-0 cutflow 777,094 selected entries | PASS (0 mismatches; all census values exact) |
| **G3 — Effective-flux Jensen band & anti-trap** | per-cell ⟨1/Γ⟩ / 1/Γ(effective kinematics) ∈ **[1.04, 1.14]** (the measured Jensen gap band); count-weighted variant recorded as diagnostic only (bias ≤ +12.1%); bin-center variants recorded as rejected diagnostics (geometric −14…+72%, log −29…+56%); emission identity ≤ 1e-12 | PASS (emission identity 0.0; count/bin-center diagnostics emitted) |
| **G4 — ε̄ bounds** | per-cell ε̄ ∈ **[0.55, 0.99]**; per-bin `eps_min`/`eps_max` within [0.550, 0.995] | PASS (ε̄ = 0.814–0.976; S3 0.880–0.882) |
| **G5 — Flux-stat threshold** | per-charge `invGamma_stat_rel` **median ≤ 0.30% AND max < 8.0%** (scale reference: the same-sample CγB numerator stat, charge-pooled median 0.551%, max 7.84%); the >0.1% carry-gate FIRED → the leg is folded into the total | PASS (medians 0.2653/0.2786%; max 4.19/7.16%; folded) |
| **G6 — Positivity & monotonicity** | all 490 σ_gammap finite > 0; all RAW positive; all ⟨1/Γ⟩/Γ̄ finite positive; per-charge cell-mean ⟨1/Γ⟩ strictly increasing across Q² strata within each x-subbin family (x0: 63.1k→128.6k→300.5k→724.8k GeV²; x1: 22.1k→47.6k→108.4k GeV²) and ⟨1/Γ⟩(x0) > ⟨1/Γ⟩(x1) within each stratum | PASS (verified on the production table) |
| **G7 — Per-bin factor identity** | σ_gammap(bin) == RAW(bin)·CγB(bin)·⟨1/Γ⟩(bin) ≤ 1e-12 rel; round-trip σ_ep,γBorn(bin) == σ_gammap(bin)·Γ̄(bin) ≤ 1e-12; integrated chain RAW→Born→noZ reproduces the release totals (≤1e-6 rel) | PASS (identity 0.0; 11.412233/11.123626 → 13.026987/12.697511 → 11.875722/11.571280 exact) |
| **G8 — F₂/reduced-xsec route** | the algebraic identity σ^{γ\*p} = (4π²α/Q²)·σ̃_r (σ̃_r = F₂ − (1−ε)F_L) verified exact at γ→0; numeric DIS-level anchor at the cell effective kinematics from published F₂/F_L (R band 0.05–0.30) as a REPORT item — no external γ\*p charged-hadron SIDIS target exists (first-of-kind) | Identity verified (design-level); numeric anchor optional REPORT, not gated |
| **G9 — Covariance congruence** | g includes 1/L_data (hard-gated vs parent payload lumi 351000.0); diag == `toy_stat_unc_gammap_nb_gev2`² ≤ 1e-12; correlation preserved ≤ 1e-10; ordering vs the release stat column ≤ 1e-9; inherited rank deficiency documented; inversion/fitting forbidden until the 1000-toy sidecar | PASS (5.6e-16 / 6.7e-16; correlation 4.4e-16; rank 99/245 documented) |
| **G10 — Budget completeness, swaps, totals** | 17 components × 490 finite, 0 negatives; 13 carried legs bit-identical (6370 cells, 0 mismatches); 30 descriptive columns bit-identical (14,700 cells); the two SWAP legs IN the totals under the renamed totals; headline medians within ±0.02 pp of the design anchors; dominant-at-median `ibu_stat_rel`; total/stat ∈ [0.5, 5.0]; >50% instability set == the parent's 4 bins; contract JSON present | PASS (medians 5.8550/6.2845% vs anchors 5.8558/6.2857, dev ≤0.0012 pp; all bit-identity gates 0 mismatches) |
| **G11 — M2 flux-model anchors** (applies to the §11 final-leg rebuild) | per-cell u_level anchor within ±0.10 pp of +0.93/+1.00/+0.54/+0.51/+0.46/+0.56/+1.75%; level-corrected u_gen anchor within ±0.10 pp of 0.06/0.50/1.19/1.36/1.73/1.73/1.38%; synthesis self-check \|r(b)−1\| ≤ 0.02 (else cell-level δ_level fallback, flagged); degenerate-slice fallback list mandatory (none exist); resolution partition recorded (\|D_raw\| > 2σ_comb); unresolved bins KEEP \|u_gen\| flagged `flux_model_stats_limited`; NO cap | M2 measured (cluster 5671056; comparison JSON verified); **final-leg rebuild PENDING — optional, not gated** |
| **G12 — Labelling/provenance completeness** | all §7.1 metadata keys present; the never-mix label verbatim in README + `gammap_budget_label` + contract JSON; sha256 chain complete (parent, CγB, flux, budget, κ, both y-resolved tables); superseded pointers present; no "(γ\*+Z)p" string anywhere; `raw_parent_release` absolute (no placeholder) | PASS, one label-hygiene item open (stale `level_leg_relabeling` key in `gammap_cross_sections.json` — §10) |

**Rollback rule (unchanged from the RUN_PLAN):** if any Stage-B/C/D gate fails, the candidate derived directory must not be populated or quoted anywhere; partials/logs retained for diagnosis; the failing gate escalated to `h1-sidis-supervisor`. An n-identity mismatch is a selection mismatch — hard stop.

---

## 9. Ordered implementation steps with owners

All steps are **executed as-built** except where marked PENDING; per the 2026-10-16 decision, the PENDING steps are optional and ungated. Owners: `h1-cross-section` (derived-product arithmetic/method owner), `h1-script-runner` (deploy/run mechanics), `h1-naf-operator` (Condor/NAF operations), `h1-qa-specialist` (QA), `h1-plotting-naf` (rendering/packaging), `h1-systematics` (budget spec/adjudication); `h1-sidis-supervisor` adjudicates and records.

| # | Step | Owner | Output path / flags | Status |
|---|---|---|---|---|
| 1 | Freeze conventions + this spec | h1-cross-section (supervisor adjudication) | this file | DONE |
| 2 | Flux worker (`compute_inverse_flux_releasebin.py`) + single-file smoke (login node ≤1 file/≤5 min) | h1-cross-section (define) / h1-script-runner (run) | `scripts/gammap_2026-10-15/`; smoke: 777,094 selected entries, 11.3 s/file | DONE |
| 3 | Shard manifests + submit files (26×10) — HTCondor 25 repair: rename queue vars (`mfile/outdir/tag`), never `manifest` | h1-script-runner | `data/flux/.../queues/` | DONE |
| 4 | Condor campaign — `request_memory=4000`, `+RequestRuntime=1800`, `retry_until=(ExitCode!=127)`, `max_retries=5`; per-job verdict PASS mode shard | h1-naf-operator | clusters 5670068 (smoke) + 5670130 (25 jobs); `data/flux/.../partials/` | DONE (26/26 exit 0) |
| 5 | Merge + hard gates — `--merge-partials --partial-list ... --cgammaborn-csv ... --expect-cell-means 63296.63,22023.99,128791.52,47753.01,301942.47,107718.98,734613.15 --expect-cell-tol 0.02`; NO `--allow-empty-fallback` | h1-script-runner | `gamma_flux_hand_inverse_fullsample_nozborn_em_260files_2026-10-15.csv` (+json, yresolved, STATUS) | DONE (exit 0, G1–G5 flux gates PASS) |
| 6 | Derived-release build — `build_gammap_release.py --flux-csv ... --output-dir ... --release-name ...`; NO `--allow-flux-fallback`, NO `--selftest` in production | h1-cross-section (builder) per RUN_PLAN Stage D | `data/gammap_derived/gammap_hand_cone_s2prime_photonborn_2026-10-15/` | DONE |
| 7 | QA remediation — covariance unit defect (add 1/L_data to g; hard gate) + parent-path placeholder + plot-label determination | h1-qa-specialist (findings) / h1-cross-section (rebuild) | rebuilt `covariance/gammap_toy_covariance_{pos,neg}.json`; record in `validation_gammap.json qa_remediation_2026_10_15` | DONE 2026-10-15 |
| 8 | Budget rebuild (SWAP MUST REPLACE; contract JSON; never-mix label) | h1-systematics (binding spec §10) / h1-cross-section (implementation) | `systematics_budget_gammap_16comp.csv`, `systematics_charged_sidis_highQ2_gammap.json`; superseded pointer | DONE 2026-10-15 (medians 5.8550/6.2845%) |
| 9 | M2 measurement — RG31 RAD emfix flux pass (1273 files, 128 shards) + comparison vs the DJ table | h1-naf-operator (Condor, cluster 5671056) / h1-cross-section | `data/flux/hand_gammap_flux_rg31_rad_emfix_releasebin_2026-10-15/` + `comparison_vs_dj_nozborn_2026-10-15.json` | DONE |
| 10 | §11 final `flux_model_rel` per-bin envelope rebuild (G11) | h1-systematics (spec §11) / h1-cross-section (builder) | rebuilt budget CSV + metadata keys (retire the interim flag; add `flux_model_status`, upper-raw variant, stats-limited list) + README/label text | **PENDING — OPTIONAL** |
| 11 | γ\*p renders/tables (only on request): binned nb·GeV² form recommended; corrected nb/GeV (y,Q²,z,p_T) density label; S3 spread statement; PYTHIA8/SHERPA3 caveat | h1-plotting-naf | `plots/` category dirs (per-topic, never loose) | **PENDING — OPTIONAL** |
| 12 | Promotion to any quoted product (note tables, exports) | user decision, recorded by h1-sidis-supervisor | — | **NOT REQUIRED** (2026-10-16: ep is the reporting level) |
| 13 | QA acceptance of any new artifact from steps 10–11 | h1-qa-specialist | validation JSONs | on demand |

---

## 10. Open items (explicit; none is a blocker or gated — 2026-10-16 decision)

1. **User promotion decision: NOT REQUIRED.** The ep cross section is the reporting level; the γ\*p program is optional auxiliary; if a theorist ever requests a γ\*p table, the machinery of this spec provides it; until then, quote ep.
2. **§11 final per-bin envelope `flux_model_rel` rebuild: PENDING, optional.** The current budget CSV carries the interim per-cell u_level (0.46–1.75%); the measured M2 ingredients (u_level per bin, level-corrected u_gen per bin) all exist. Rebuild scope: flux legs + `total_with_gammap16_rel` + metadata (`generator_component_unevaluated_interim_bound` retired; `flux_model_status: "measured_envelope_ulevel_ugen_M2_2026-10-15"`; upper-raw variant; stats-limited list) + README/label caveat text (the "no generator cross-check" caveat is superseded for the flux-weighting part).
3. **α value bookkeeping (cosmetic, record-only):** spec-frozen α(0) = 1/137.035999084 (CODATA 2018) vs as-built 1/137.0359895 (generator-matched, user-locked 2026-10-15); Δ = −6.99×10⁻⁸ relative on all γ\*p values — uniform, below every gate, no rebuild required; any future rebuild adopts the spec value and records the change.
4. **Stale `level_leg_relabeling` key in `gammap_cross_sections.json`** (the authoritative `metadata.json` carries `level_leg_swaps`): label hygiene; fold into the next rebuild of any release artifact.
5. **100-toy vs 1000-toy covariance staging (inherited):** the derived covariance uses the in-tree 100-toy incumbent (rank 99/245); inversion/fitting use FORBIDDEN until the 1000-toy sidecar is installed; correlation-only for shape comparisons.
6. **Inherited publication-blockers (relevant only if ever promoted):** P-CRAD-LUMI (file-set-reconciled; generator-log closure blocked externally); C_gamma_born PROVISIONAL DIAGNOSTIC DJANGOH-only (EW-projection part; no RAPGAP NoZ sample exists); P-ABS-NORM (absolute normalization provisional); PYTHIA8/SHERPA3 conversion approximation (theory-side caveat + band on MC curves only).
7. **S3 presentation statement:** a single σ_γ\*p number per S3 bin is exact but hard to interpret at "the" bin kinematics (in-bin Γ spread up to 137×; ε̄ ≈ 0.881 over y ∈ [0.1, 0.7]) — any S3 render carries an explicit spread statement plus the effective-kinematics sidecar.
8. **σ_T-only requests: declined** for the central product; provide the ε̄ sidecar + the documented conversion penalty (3.4–8.5% at plausible R).
9. **x-cap render split (R6):** the binned nb·GeV² form is the recommended presentation; if an (x,Q²)-volume density render is ever produced, Δx = 1.0−x_min is MANDATORY (S3_x0 bookkeeping x_max = 1.5 must never enter a density division uncapped); the (y,Q²,z,p_T) density is nb/GeV, never nb/GeV³.
10. **MAPTMD24 coverage:** 230/490 bins (no z<0.05 row; sparse p_T at higher z); covered-bin midpoint totals are not comparable to full-coverage totals; the full-grid provider CSV remains open.
11. **HTCondor 25 'manifest' reserved-word trap:** documented; any re-run uses the repaired submit-file pattern (renamed queue variables).
12. **MC-curve EW audit (PYTHIA8/SHERPA3 Z-content + α convention):** unverified; their γ\*p conversions stay caveat-only until audited.
13. **No per-period decomposition of L_data = 351,000.0 nb⁻¹ exists** (only the GRL inventory sum 356,613.59 nb⁻¹) — irrelevant to this product (single beam configuration; Γ is charge-blind), noted for completeness.
14. **MER/LER: moot** — excluded by the GRL boundary (0 of 20,788 runs); no mixed-√s flux treatment is needed.

---

*Specification produced read-only; the only file written by this task is this document. All numbers were verified against the as-built artifacts (`data/flux/hand_gammap_flux_releasebin_2026-10-15/`, `data/flux/hand_gammap_flux_rg31_rad_emfix_releasebin_2026-10-15/`, `data/gammap_derived/gammap_hand_cone_s2prime_photonborn_2026-10-15/`, `scripts/gammap_2026-10-15/`) and the named authoritative documents on 2026-10-16.*
