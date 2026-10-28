# Research: Frozen H1-Published-Precedent Systematics Menu for v6sys High-Q² Charged-SIDIS Release

**Date:** 2026-06-03
**Author role:** h1-physics-analyst (charged-hadron SIDIS)
**Workspace:** `H1/sidis_highQ2/`
**Scope:** Decide and freeze, item-by-item, which H1-published-precedent systematics are required in the high-Q² charged-SIDIS v6sys release; classify each as already-present / post-unfolding-diagonal-now / requires-Stage1+Stage2-shifted-MC / handled-elsewhere / NA; pin syskeys and magnitudes where literature allows; mark BLOCKED rather than invent magnitudes that are not pinned. **Read-only research; no code edits, no remote commands.**
**Audience:** `h1-sidis-supervisor`, `h1-systematics`, `h1-builder-engineer`, `h1-analysis-critic`, `code-editor` for implementation, `h1-analysis-critic` for sign-off.

> **Bottom line.** The H1-precedent systematics menu for this analysis is fully enumerable today. Six items are already present in the rev6 pipeline in some form; one is already implemented as a post-unfolding diagonal source (`tracking_central_h1_1302_1321`); two are pure normalization/background sources that belong outside v6sys and are already represented as analytic placeholders; six are H1Calc electron-side / HFS-side / noise-side migration-affecting sources that REQUIRE the v6sys paired Stage1+Stage2 shifted-MC production before any per-source covariance can be emitted; one (combined/forward 10% tracking) is NOT APPLICABLE because the high-Q² fiducial sits inside |η|<1.3 central CTD acceptance. Until the v6sys shifted-MC response runs land, the electron/HFS/noise sources MUST be emitted only as metadata stubs (BLOCKED state, with magnitudes pinned where literature permits) and MUST NOT be filled with rank-1 reweight covariances on the 480-bin grid — that pattern has already been flagged critic-fatal in the rev6_v2 pipeline.

---

## 0. Source-Anchoring (read first)

This memo synthesizes the following local artifacts, all of which I read in full:

- `sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md` — H1 low-Q² charged-particle spectra paper, full §5 systematics budget transcribed (SpaCal 1%, SpaCal θ 1 mrad, HFS 2%, model DJ/RG, central tracking 1%, combined 10%, K0S 0.2%, γp 30%).
- `sidis_highQ2/docs/research/research-tracking-systematics-2026-05-25.md` — H1 high-Q² literature survey (Aktas 2007 PLB 654, Aaron 2009 PLB 681); pins 2.5% per-track inclusive tracking and 2.5% per-track anti-correlated charge-dependent for A_h.
- `sidis_highQ2/docs/research/research-HFS-systematic-2026-05-25.md` — H1 high-Q² HFS convention: JES ±1% (in-jet) + RCES ±1% (out-of-jet), independently varied, propagated through full unfold (Andreev 2014/2017/2023/2024). Full Option A vs Option B comparison; the "2%" framing inherited from low-Q² 1302.1321 is wrong for the high-Q² regime — replace with two ±1% components.
- `sidis_highQ2/docs/research/research-g1jES-electron-LAr-systematics-2026-06-01.md` — Canonical electron-side LAr systematic recipe from g1j_ES: `eElecEn` ±0.5% per LAr wheel (5 wheels), `eElecTh` +1 mrad per θ region (3 regions) plus matched `eElecTrTh`, applied via full response-matrix rebuild per iSys.
- `sidis_highQ2/docs/research/unfolding_systematic_decomposition_2026-05-24.md` — Canonical four-term unfolding decomposition: σ²_unfold = σ²_stat + σ²_model(DJ↔RG) + σ²_reg(τ×2) + σ²_MC-stat,response. Method spread is a coverage check, NOT a systematic.
- `sidis_highQ2/docs/research/research-v6sys-smoke-physics-review-2026-06-03.md` — Current v6sys smoke state: Stage2-only PASS for `central` and `electron_energy_up` on DJANGOH MC; Stage1 event-processing smoke not yet run; `electron_theta_up` mapped not yet smoked; `electron_energy_down` / `electron_phi_*` / `hfs_*` / `noise_energy_*` all blocked without `--sys-allow-default`.
- `sidis_highQ2/plan-v6sys-treebuilder.md` — Frozen v6sys plan: H1Calc whitelist enum values (`eHadEn=0`, `eHadEnRCES=1`, `eHadEnCor=2`, `eHadTh=4`, `eHadPh=5`, `eNoiseEn=6`, `eElecEn=7`, `eElecEnCor=8`, `eElecEnUnc=9`, `eElecTh=10`, `eElecPh=11`, `eElecTrEn=12`, `eElecTrTh=13`, `eElecTrPh=14`); Stage1+Stage2 paired rerun required for all final kinematic detector systematics; Stage2-only acceptable only for previews.
- `sidis_highQ2/data/systematics_sources/` — Current source-file inventory (electron_energy/theta/phi, tracking_efficiency/chargedep/resolution, NucliaWeight, trigger, lumi, unfolding_IBU/TUnfold, nominal); critic-fatal rank-1 status documented for electron_* in the related electron critique notes (2026-05-26).
- `sidis_highQ2/data/systematics_sources/tracking_central_h1_1302_1321/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/source_tracking_central_h1_1302_1321.json` — Just-emitted post-unfolding diagonal source, `sigma_rel = sqrt(0.010² + 0.002²) = 0.01019803902718557`, explicitly excludes combined/forward 10% term because of |η|<1.3 fiducial.

I read these in full before writing this menu; cross-references are flagged inline.

---

## 1. Frozen Decision Table — H1-Precedent Source Menu for v6sys/High-Q² SIDIS

Each row pins the syskey (production-side name), the H1Calc enum if any, the canonical magnitude (with provenance citation), the bin-migration character, the v6sys implementation route, the current state, and the validation gate that must close before the source can be filled with real numbers.

Categories used:
- **A** = already present in the rev6 / v3final pipeline (in some form, with health caveats noted).
- **B** = implementable now as post-unfolding analytic / diagonal source (no shifted MC needed).
- **C** = REQUIRES v6sys paired Stage1+Stage2 shifted-MC response production; BLOCKED until production runs land.
- **D** = normalization / background / model source handled outside v6sys (lumi, trigger, model, regularization, etc.).
- **E** = NOT APPLICABLE to high-Q² SIDIS (excluded by fiducial or by regime).

### 1.1 Detector systematics (LAr, electron, HFS, noise)

| # | H1-precedent source | High-Q² magnitude (citation) | syskey (v6sys) | H1Calc enum | Migration? | Category | Current state | Gate to fill |
|---|---|---|---|---|---|---|---|---|
| 1 | LAr scattered-electron energy scale, per wheel | ±0.5% per LAr wheel (5 wheels: BBE, CB1+2, CB3, FB1, FB2+IF) — g1j_ES `AnalysisBase.C:108–129`, anchored to arXiv:0904.3513 | `electron_energy_up`, `electron_energy_down` | `eElecEn=7` | YES (Q², x, y, Empz, acceptance) | **C** | Critic-FATAL rank-1 `source_electron_energy.json` exists; Stage2-only DJANGOH MC smoke for `electron_energy_up` PASSED; `electron_energy_down` blocked; Stage1 not smoked | **Paired Stage1+Stage2 shifted-MC for ±0.5% per all 5 wheels**, response rebuild, full unfold per side; only then emit per-source covariance |
| 2 | LAr scattered-electron polar angle, per impact region | +1 mrad per region (3 regions: θ>135°, 120–135°, <120°) PLUS matched `eElecTrTh` +1 mrad — g1j_ES `AnalysisBase.C:130–141` (g1j_ES uses 1 mrad; arXiv:0904.3513 quotes 0.2–0.5 mrad; **adopt 1 mrad as conservative envelope to match g1j_ES; flag for re-tightening to 0.2 mrad once SIDIS calibration revisited**) | `electron_theta_up`, `electron_theta_down` | `eElecTh=10` co-shifted with `eElecTrTh=13` | YES | **C** | Critic-FATAL rank-1 `source_electron_theta.json` exists; v6sys Stage2-only `electron_theta_up` call site mapped, not yet smoked | **Paired Stage1+Stage2 ±1 mrad per all 3 regions + matched `eElecTrTh`**, response rebuild, full unfold per side |
| 3 | LAr scattered-electron azimuth | **BLOCKED — magnitude not pinned.** No g1j_ES analogue (g1j_ES does not vary `eElecPh`); arXiv:0904.3513 carries no published HERA-II prescription for the SIDIS regime | `electron_phi_up`, `electron_phi_down` | `eElecPh=11` | YES (frame boost, φ_h) | **C / BLOCKED** | Critic-FATAL rank-1 `source_electron_phi.json` exists with no provenance | **Decide first: drop or rebuild.** Default recommendation: DROP from v6sys until a published HERA-II `eElecPh` magnitude is found (`h1-software` to confirm). If retained, BLOCKED until magnitude pinned and Stage1+Stage2 paired run completes |
| 4 | HFS energy scale, in-jet (JES) | **±1.0%** on HFS objects inside high-pT jets (k_T, R=1.0, pT>5 GeV, η_lab∈[−1.5, 2.75]) — Andreev 2023 PLB 844 §4; Andreev 2024 EPJC 84 §6.2; canonical calibration Andreev 2014 EPJC 75 65 | `hfs_jes_up`, `hfs_jes_down` | `eHadEn=0` (operating on in-jet subset, OR a dedicated JES-class enum if SIDIS builder distinguishes) | YES | **C** | Not yet implemented in rev6 pipeline; placeholder envelope `perbin_LAr_HFS_beam.json` exists but does not split JES/RCES | **Paired Stage1+Stage2 ±1% applied per-object inside in-jet HFS class.** Requires in-jet/out-of-jet classifier matching Andreev 2023 jet definition (verify v5g jet collection matches; if not, build classifier collection in v6sys builder) |
| 5 | HFS energy scale, out-of-jet (RCES) | **±1.0%** on HFS objects outside high-pT jets — Andreev 2023 §4; Andreev 2024 §6.2 | `hfs_rces_up`, `hfs_rces_down` | `eHadEnRCES=1` (per audited v6sys plan enum) | YES | **C** | Not yet implemented | **Paired Stage1+Stage2 ±1% applied to out-of-jet HFS subset**, independent variation from JES |
| 6 | HFS polar angle | **+1 mrad** alignment uncertainty — Andreev 2024 §6.1 ("HFS polar-angle alignment 1 mrad → typically ~1%") | `hfs_theta_up`, `hfs_theta_down` | `eHadTh=4` | YES | **C** | Not yet implemented | Paired Stage1+Stage2 ±1 mrad |
| 7 | HFS azimuth | **±20 mrad** — Andreev 2023 §4; Andreev 2024 §6.1 | `hfs_phi_up`, `hfs_phi_down` | `eHadPh=5` | YES (φ_h, frame) | **C** | Not yet implemented | Paired Stage1+Stage2 ±20 mrad |
| 8 | LAr cluster noise (residual non-HFS energy) | **BLOCKED — magnitude not pinned in SIDIS regime.** g1j_ES does not vary `eNoiseEn`; 1302.1321 has no explicit noise term; Andreev 2024 absorbs it into RCES | `noise_energy_up`, `noise_energy_down` | `eNoiseEn=6` | YES | **C / BLOCKED-OR-DROP** | Not implemented | **Decide first: drop or pin.** Default recommendation: DROP from v6sys baseline; if retained, BLOCKED until magnitude is pinned by `h1-software` from H1Calc default or by published H1 reference |

### 1.2 Tracking systematics (central CTD, vertex, weak decays, nuclear interactions)

| # | H1-precedent source | High-Q² magnitude (citation) | syskey | H1Calc enum | Migration? | Category | Current state | Gate to fill |
|---|---|---|---|---|---|---|---|---|
| 9 | Central-tracking efficiency (incl. vertex reco, weak decays, nuclear interactions) — fiducial-inside-CTD form | **1.0%** central tracking ⊕ **0.2%** K0S contamination = √(0.010² + 0.002²) = **1.0198%** post-unfolding diagonal, per charge, independent — arXiv:1302.1321 §5; matches Aaron 2009 PLB 681 §6 "2.5% on D±" envelope **down-scaled** because the dense-jet inflation does NOT apply to the high-Q² SIDIS fiducial |η_h|<1.3 inside central CTD | `tracking_central_h1_1302_1321` | none (analytic post-unfold) | NO (per-track weight, bin-boundary preserving) | **B (already implemented)** | `source_tracking_central_h1_1302_1321.json` EMITTED 2026-06-03 to FINALBIN release; per-bin covariances `cov_tracking_central_h1_1302_1321_{pos,neg}.npz` present | **DONE.** No further action; ready to flow into combined covariance |
| 10 | Aaron-2009 high-Q² envelope (2.5% per-track, dense-jet inflation in highest-Q² bin) | **NOT ADOPTED as separate source.** The Aaron 2009 2.5% is the upper envelope for D±; our 1302.1321 1.0% inside fiducial is the appropriate ceiling and is already booked. If a critic insists on adopting Aaron 2009's full 2.5%, replace `tracking_central_h1_1302_1321` σ_rel from 0.0102 → 0.025; document as the alternative and re-emit. **Conservative-vs-best-fit decision pending; default = 1302.1321 1.0198%.** | n/a (alternative form of #9) | n/a | NO | **B (alternative)** | Not emitted; would supersede #9 if adopted | Supervisor decision: adopt Aaron-2009 2.5% conservative ceiling? If yes, re-emit #9 with σ_rel=0.025 |
| 11 | Charge-dependent tracking residual on A_h (fully anti-correlated h⁺ vs h⁻) | **2.5% on A_h** (conservative, Aaron 2009 PLB 681 §6) — implemented per-track via anti-correlated weight reweight, NO re-unfold required (per-track weight, bin-boundary preserving) | `tracking_chargedep` | none | NO | **B** | `source_tracking_chargedep.json` and `cov_tracking_chargedep.npz` already present in `data/systematics_sources/` | **Sanity-check existing source** against the Aaron 2009 2.5% envelope per the formula in `research-tracking-systematics-2026-05-25.md` §4.2. If matches within 30%, ACCEPT; if not, regenerate |
| 12 | Combined / forward (CTD+FTD) tracking efficiency | **10% per-track** — arXiv:1302.1321 §5 | n/a | n/a | n/a | **E (NOT APPLICABLE)** | Explicitly excluded in `source_tracking_central_h1_1302_1321.json` `explicitly_excludes` field | NOT APPLICABLE: high-Q² SIDIS fiducial |η_h|<1.3 ⊂ central CTD acceptance; no forward / combined tracks used. Document the exclusion in the analysis note |
| 13 | Tracking precision (per-track pT resolution) | **BLOCKED — methodology not literature-anchored.** No H1 paper publishes an explicit tracking-precision systematic; `research-tracking-systematics-2026-05-25.md` §4.3 proposes a ±50% smearing study (one-sided, +50%, on `σ(pT)/pT = 0.006·pT ⊕ 0.02`). This is migration-affecting and requires response rebuild | `tracking_resolution` | none (extractor-level smearing) | YES | **C** | Critic-FATAL rank-1 `source_tracking_resolution.json` exists; `cov_tracking_resolution.npz` and `_rapgap.npz` present | **Paired smeared-MC reco → response rebuild → re-unfold.** Or DROP and explicitly state the absence of an H1-precedent prescription. Recommendation: DROP from v6sys baseline; tracking precision is implicit in every H1 publication |

### 1.3 Background, model, regularization, normalization (handled outside v6sys)

| # | H1-precedent source | Magnitude (citation) | syskey | Migration? | Category | Current state | Gate to fill |
|---|---|---|---|---|---|---|---|
| 14 | Photoproduction (γp) background contamination | **±30%** of remaining γp template (Phojet) — arXiv:1302.1321 §5; standard at high y. **At high-Q² SIDIS y<0.7 + Empz∈[35,70] selection, residual γp is ≲ a few × 0.1%**, so the propagated effect on multiplicities is at most O(0.1%) per bin (Andreev 2023 §4 explicitly characterizes background as "negligible") | `gammap_bkg_30pct` | NO (analytic on subtracted template) | **D** | Not separately emitted; folded into background subtraction pipeline | Re-emit only if a γp template is being subtracted from data; otherwise document as "negligible at high-Q²" in the analysis note |
| 15 | Model dependence (DJANGOH vs RAPGAP unfolded result) | **Half-difference** σ_model = ½\|IBU(R_DJ) − IBU(R_RG)\| — Andreev 2024 §6.4; canonical H1 high-Q² prescription | `model_DJvsRG` | YES (already re-unfolded) | **A / D** | Existing in pipeline as `unfolded_4d_v3final_data_DJvsRG.json`, `v5g_unfolding_*_closure_crossMC.json` | Already present; reformat into `source_model_DJvsRG.json` if the combiner expects per-source JSON. Source-level emission is a `code-editor` task |
| 16 | Unfolding regularization (τ-scan) | τ × 2 and τ / 2; half-difference — Andreev 2024 §6.4 | `unfolding_reg` | YES (re-unfolded) | **A / D** | `sigma_reg_TUnfold_4d_v3final.json`, `tunfold_sigma_reg/`, `ibu_sigma_reg/` exist | Already computed; ready to reformat into `source_unfolding_reg.json` |
| 17 | Unfolding statistical (bootstrap on data) | 100-toy bootstrap, IBU or TUnfold central — Andreev 2403.10134 §6.4 | `unfolding_stat` | YES (re-unfolded per toy) | **A / D** | Present as IBU `cov` matrices, `unfolded_4d_v3*_IBU_covariance.npz`, `unfolded_4d_v3*_TUnfold_covariance.npz` | Already present; reformat as `source_unfolding_stat.json` |
| 18 | Response-matrix MC-stat | MC bootstrap of response — Andreev 2017 §6 | `unfolding_MCstat` | YES | **A / D** | `sigma_MCstat_TUnfold_4d_v3final.json`, `mcstat_replicas_tau0p1/` present | Already present; reformat |
| 19 | Method spread (BBB vs IBU vs TUnfold) | **NOT A SYSTEMATIC.** Per `unfolding_systematic_decomposition_2026-05-24.md` and Brenner-Burgard-Croft 2025 (arXiv:2510.15129): method spread is a coverage diagnostic, not a systematic. Report in supplementary material only | n/a | n/a | n/a (cross-check) | Not emitted as a source | Document explicitly that method-spread is NOT in the budget; show comparison plot in supplementary material only |
| 20 | Luminosity normalization | **2.7%** (HERA-II combined) — Andreev 2024 §6.3 | `lumi` | NO (normalization) | **D** | `source_lumi.json` EXISTS (σ_rel = 0.027); cancels in A_h | DONE; applies only to absolute cross sections, not to multiplicities or A_h |
| 21 | Trigger efficiency | **NEGLIGIBLE at high-Q²**, conservative ≤0.5% — `source_trigger.json` notes T67 inclusive-DIS trigger > 99% efficient | `trigger` | NO | **D** | `source_trigger.json` EXISTS (σ_rel = 0.005, placeholder ceiling); cancels in A_h | DONE; replace with measured value if rate-vs-lumi study performed |
| 22 | Nuclear interaction weight (`NucliaWeight`) systematic | Conservative envelope from NucliaWeight distribution; cancels in A_h — `source_NucliaWeight.json` | `NucliaWeight` | NO (per-event weight) | **D** | `source_NucliaWeight.json` EXISTS; cancels in A_h | DONE; supervisor-decision pending on adoption in M^h budget |
| 23 | Radiative correction (C_rad per bin) | Explicit per-bin C_rad from DJANGOH radiative vs non-radiative; magnitude ≲ few % — see `research-crad-for-charged-sidis.md`, `research-rapgap-crad.md` | `crad` | NO (per-bin correction) | **D** | `crad_v3_HQv5gBprime_2026-05-24.json` exists | Already computed; emit as `source_crad.json` if combiner requires |

### 1.4 Inapplicable to high-Q² SIDIS

| # | H1-precedent source | Why inapplicable to high-Q² SIDIS |
|---|---|---|
| 24 | Backward-SpaCal scattered-electron energy scale (low-Q² SpaCal cluster 1% from 1302.1321) | At high-Q² (Q²>150 GeV²), scattered electron lands in **LAr**, not SpaCal. The LAr equivalent is item #1 (`eElecEn`). Document the regime difference in the analysis note |
| 25 | Backward-SpaCal angular resolution (1 mrad from 1302.1321) | Same as above; LAr equivalent is item #2 (`eElecTh`) |
| 26 | eΣ-method specific low-y / high-y resolution systematics | High-Q² SIDIS uses eΣ (or Σ) per existing analysis; standard kinematic-method uncertainties are folded into items #1–#7 via H1Calc shifts. No separate eΣ syst needed |
| 27 | Combined/forward (CTD+FTD) tracking efficiency 10% | See item #12; |η_h|<1.3 fiducial inside CTD |

---

## 2. H1Calc syskey & enum freeze for v6sys electron / HFS sources

Per the v6sys plan (`plan-v6sys-treebuilder.md` §"How H1CalcSystematic Should Be Applied") and the g1j_ES recipe (`research-g1jES-electron-LAr-systematics-2026-06-01.md` §2), the canonical electron + HFS H1Calc applications for the SIDIS-linked H1OO build (`relvol10/4.2.1`) are:

| syskey | H1Calc enum | Setter call (g1j_ES recipe) | Magnitude | Side |
|---|---|---|---|---|
| `central` | n/a (sentinel `-9999`) | `ResetAllShifts(); Reset()` only | n/a | none |
| `electron_energy_up` | `eElecEn=7` | per wheel w∈[0..4]: `SetShiftElecEnWheel(w, +0.005)`; then `ShiftUp(eElecEn)` | +0.5% per LAr wheel | up |
| `electron_energy_down` | `eElecEn=7` | per wheel w∈[0..4]: `SetShiftElecEnWheel(w, -0.005)`; then `ShiftDo(eElecEn)` | −0.5% per LAr wheel | down |
| `electron_theta_up` | `eElecTh=10` + `eElecTrTh=13` co-shift | per region r∈[0..2]: `SetShiftElecTheta(r, +0.001)`; `SetShift(eElecTrTh, +0.001)`; `ShiftUp(eElecTh); ShiftUp(eElecTrTh)` | +1 mrad per θ region + matched track θ | up |
| `electron_theta_down` | `eElecTh=10` + `eElecTrTh=13` co-shift | per region r∈[0..2]: `SetShiftElecTheta(r, -0.001)`; `SetShift(eElecTrTh, -0.001)`; `ShiftDo(eElecTh); ShiftDo(eElecTrTh)` | −1 mrad per θ region | down |
| `electron_phi_up` | `eElecPh=11` | **BLOCKED — no g1j_ES recipe, no published magnitude.** Default v6sys plan magnitude (placeholder) cannot be defended | **BLOCKED** | up |
| `electron_phi_down` | `eElecPh=11` | **BLOCKED** | **BLOCKED** | down |
| `hfs_jes_up` | `eHadEn=0` (in-jet subset) | `SetShift(eHadEn, +0.01)`; apply only to HFS objects inside k_T R=1.0 pT>5 GeV jets (η_lab∈[−1.5, 2.75]); `ShiftUp(eHadEn)` | +1% in-jet HFS | up |
| `hfs_jes_down` | `eHadEn=0` (in-jet) | same with −0.01; `ShiftDo` | −1% in-jet HFS | down |
| `hfs_rces_up` | `eHadEnRCES=1` (out-of-jet) | `SetShift(eHadEnRCES, +0.01)`; `ShiftUp` | +1% out-of-jet HFS | up |
| `hfs_rces_down` | `eHadEnRCES=1` (out-of-jet) | same with −0.01; `ShiftDo` | −1% out-of-jet HFS | down |
| `hfs_theta_up` | `eHadTh=4` | `SetShift(eHadTh, +0.001)`; `ShiftUp` | +1 mrad HFS θ | up |
| `hfs_theta_down` | `eHadTh=4` | same with −0.001; `ShiftDo` | −1 mrad HFS θ | down |
| `hfs_phi_up` | `eHadPh=5` | `SetShift(eHadPh, +0.020)`; `ShiftUp` | +20 mrad HFS φ | up |
| `hfs_phi_down` | `eHadPh=5` | same with −0.020; `ShiftDo` | −20 mrad HFS φ | down |
| `noise_energy_up` | `eNoiseEn=6` | **BLOCKED — no pinned magnitude for SIDIS regime.** Default-shift fallback is `--sys-allow-default` only | **BLOCKED** | up |
| `noise_energy_down` | `eNoiseEn=6` | **BLOCKED** | **BLOCKED** | down |

### 2.1 Magnitude-source freeze

- **`eElecEn` ±0.5% per wheel:** PINNED. Provenance: arXiv:0904.3513 (electron-energy scale at HERA-II), g1j_ES `AnalysisBase.C:108–129`. Per-wheel granularity is mandatory; a single global `SetShift(eElecEn)` loses the per-wheel response and degenerates to a single-amplitude rescale (flagged in `research-v6sys-smoke-physics-review-2026-06-03.md` line 159).
- **`eElecTh` +1 mrad per region:** PINNED with caveat. g1j_ES uses 1 mrad; arXiv:0904.3513 lower bound is 0.2 mrad (HERA-II jets) and 0.5 mrad (low-Q² FL). **Adopt 1 mrad as the conservative envelope for v6sys initial production**; flag for revisitation to 0.2 mrad in a follow-up systematic-reduction pass after the v6sys baseline is in place.
- **`eElecPh`:** NOT PINNED. Drop from v6sys unless `h1-software` or supervisor produces a magnitude. Do NOT use `--sys-allow-default` for a publication-grade result; default-shift fallback is for infrastructure smoke only.
- **`eHadEn` (JES, in-jet) ±1%:** PINNED. Andreev 2023 PLB 844 §4 ("In both cases, the energy-scale uncertainty is ±1%"), Andreev 2024 EPJC 84 §6.2.
- **`eHadEnRCES` (RCES, out-of-jet) ±1%:** PINNED. Same citations.
- **`eHadTh` ±1 mrad:** PINNED. Andreev 2024 §6.1.
- **`eHadPh` ±20 mrad:** PINNED. Andreev 2023 §4 ("uncertainty of ±20 mrad … on the azimuthal angle determination of HFS objects"), Andreev 2024 §6.1.
- **`eNoiseEn`:** NOT PINNED. Drop or block.

### 2.2 In-jet / out-of-jet classifier for JES vs RCES

Andreev 2023 §1 jet definition (which the H1 JES calibration was tuned against): **k_T algorithm, R = 1.0, pT > 5 GeV, lab frame, on HFS objects with −1.5 < η_lab < 2.75**. The v6sys builder must apply `eHadEn` ±1% only to HFS objects whose parent jet passes this cut, and `eHadEnRCES` ±1% only to HFS objects outside any such jet. If the existing v5g/v6sys jet collection differs (different algorithm, different R, different pT threshold), the JES vs RCES split is degraded; in that case either (a) build a parallel JES-classifier jet collection in the v6sys builder, or (b) collapse JES + RCES into a single ±1% uniform HFS shift as a fallback (then quadrature gives the same envelope but loses the per-bin correlation structure).

---

## 3. Implementation route: stub now or fill now?

This is the most important policy question and the answer is uniform across all H1Calc-shift sources.

### Policy

**Until the v6sys paired Stage1+Stage2 shifted-MC production lands, emit electron / HFS / noise sources as METADATA STUBS ONLY. Do NOT fill them with numerical covariances on the 480-bin grid.** 

Rationale, source-anchored:

1. **Rank-1 reweight covariances on the 480-bin grid have been flagged critic-FATAL three times** (`source_electron_energy.json`, `source_electron_theta.json`, `source_electron_phi.json`) in the rev6_v2 pipeline. See `research-electron-energy-systematic-critique-2026-05-26.md`, `research-electron-theta-critique-2026-05-26.md`. The pattern (form `cov = δ δᵀ` from a posthoc per-bin shift δ on already-unfolded data) is mathematically a rank-1 outer product that does NOT respect noise floors on low-population cells. Re-introducing the same pattern in v6sys with new magnitudes does not fix the underlying error; it relabels it.

2. **Stage2-only response shifts are not migration-faithful** because Stage 1 preselection uses `gH1Calc`-dependent quantities (electron θ, Empz, Zimpact/PhiOctant, FidVolCut, Q²es, Yes). A Stage2-only run on a fixed Stage 1 event sample cannot recover events that fail central Stage 1 admission but would pass under the shift, and vice versa. This is the **Stage 1 admission-boundary bias**, explicitly stated in the v6sys plan §"Physics Policy Box" and `research-v6sys-smoke-physics-review-2026-06-03.md` Q4.

3. **The g1j_ES canonical implementation rebuilds the migration matrix per iSys** (`research-g1jES-electron-LAr-systematics-2026-06-01.md` §2.3): every iSys is applied by **rerunning the event reconstruction** through H1Calculator / H1BoostedJets, which gives a fully self-consistent migration matrix per source. There is NO posthoc weight-only or bin-shift shortcut anywhere in g1j_ES. The same physics-correctness rule applies to v6sys.

### What "metadata stub" means concretely

For each blocked source (#1, #2, #3 if retained, #4, #5, #6, #7, #8 if retained, #13 if retained), the v6sys release should emit a JSON of the following shape:

```json
{
  "source_id": "electron_energy",
  "schema_version": "v6sys_metadata_stub_v1",
  "stage": "pre_unfolding",
  "type": "migration_affecting_h1calc_shift",
  "status": "BLOCKED_pending_v6sys_paired_Stage1_Stage2_response_rerun",
  "h1calc_enum": "eElecEn",
  "h1calc_enum_value": 7,
  "magnitude_up": 0.005,
  "magnitude_down": -0.005,
  "magnitude_application": "per_LAr_wheel_5_wheels",
  "magnitude_provenance": "arXiv:0904.3513 + g1j_ES AnalysisBase.C:108-129",
  "implementation_route": "Stage1+Stage2 v6sys --sys electron_energy_{up,down} for all 5 wheels, per period; rebuild response, unfold each side; per-bin sigma = half-difference, per-bin cov to be characterized against noise floor before rank-1 cov is admitted",
  "blocks": [
    "v6sys Stage1 event-processing smoke not run",
    "Stage2 down-side variation not run",
    "response-matrix rebuild not done per iSys",
    "noise-floor characterisation outstanding"
  ],
  "do_not_fill_with_rank1_cov": true,
  "previous_critic_verdicts": [
    "research-electron-energy-systematic-critique-2026-05-26.md: rank-1 cov on noise-dominated bins, critic FATAL"
  ]
}
```

The combiner (`combine_systematics.py` and friends) MUST be patched to treat `status == BLOCKED_*` as a zero-contribution source with an explicit "not yet produced" entry in the systematic budget table — NOT as a missing source (which would silently drop the term) and NOT as a usable rank-1 source.

### What "fill now" means for the items that can be filled

- **#9 `tracking_central_h1_1302_1321`:** DONE 2026-06-03.
- **#11 `tracking_chargedep`:** existing source can be filled now (post-unfolding analytic, anti-correlated per-track weight, bin-boundary preserving). Sanity-check existing `source_tracking_chargedep.json` against Aaron 2009 2.5% envelope per the formula in `research-tracking-systematics-2026-05-25.md` §4.2.
- **#15 model dependence:** existing `unfolded_4d_v3final_data_DJvsRG.json` can be reformatted into `source_model_DJvsRG.json` now.
- **#16 unfolding regularization:** existing `sigma_reg_TUnfold_4d_v3final.json` reformatted.
- **#17 unfolding stat:** existing IBU/TUnfold cov NPZ reformatted.
- **#18 response MC-stat:** existing `sigma_MCstat_TUnfold_4d_v3final.json` reformatted.
- **#20 `lumi`:** DONE.
- **#21 `trigger`:** DONE (placeholder ceiling 0.5%).
- **#22 `NucliaWeight`:** DONE.
- **#23 `crad`:** existing `crad_v3_HQv5gBprime_2026-05-24.json` can be reformatted into `source_crad.json` now.
- **#14 γp 30%:** if no γp template is being subtracted, omit and document; if it is, emit with σ_rel propagated from 30% template variation.

### What should be DROPPED from v6sys (not blocked, not stubbed)

- **#3 `electron_phi_*`** unless a published magnitude surfaces. Current critic-FATAL `source_electron_phi.json` should be removed or marked DEPRECATED.
- **#8 `noise_energy_*`** unless a magnitude is pinned. No published HERA-II SIDIS magnitude exists.
- **#10 Aaron-2009 2.5% alternative** unless supervisor chooses the conservative ceiling.
- **#12 combined/forward 10%** NOT APPLICABLE.
- **#13 `tracking_resolution`** unless `h1-systematics` agrees the ±50% smearing methodology is defensible without H1-paper precedent. Current critic-FATAL `source_tracking_resolution.json` should be removed or marked DEPRECATED. Tracking precision is implicit in every H1 publication.
- **#19 method spread** NOT a systematic.
- **#24, #25 SpaCal-side variations** wrong regime.

---

## 4. Implementation Handoff (concrete)

### 4.A Sources to EMIT NOW locally (no Condor required)

For the FINALBIN release directory `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/`, emit / reformat the following per-source JSON + cov NPZ pairs:

| File | Action | Owner | Magnitude | Citation |
|---|---|---|---|---|
| `source_tracking_central_h1_1302_1321.json` (+ pos/neg cov) | DONE 2026-06-03 | n/a | σ_rel = 0.01019803902718557 (=√(0.010² + 0.002²)) | arXiv:1302.1321 §5 |
| `source_tracking_chargedep.json` (+ cov) | SANITY-CHECK existing vs 2.5% on A_h | `h1-systematics` | 2.5% on A_h, anti-correlated per-track | arXiv:0907.2666 §6 |
| `source_lumi.json` | DONE | n/a | 2.7% | Andreev 2024 §6.3 |
| `source_trigger.json` | DONE (placeholder) | n/a | ≤0.5% | placeholder until rate-vs-lumi study |
| `source_NucliaWeight.json` | DONE | n/a | per-event weight envelope; cancels in A_h | NucliaWeight distribution analysis |
| `source_crad.json` | REFORMAT from `crad_v3_HQv5gBprime_2026-05-24.json` | `code-editor` | per-bin | DJANGOH rad vs non-rad |
| `source_model_DJvsRG.json` | REFORMAT from `unfolded_4d_v3final_data_DJvsRG.json` | `code-editor` | ½\|IBU(R_DJ) − IBU(R_RG)\| | Andreev 2024 §6.4 |
| `source_unfolding_stat.json` | REFORMAT from `unfolded_4d_v3final_IBU_covariance.npz` | `code-editor` | 100-toy bootstrap | Andreev 2024 §6.4 |
| `source_unfolding_reg.json` | REFORMAT from `sigma_reg_TUnfold_4d_v3final.json` | `code-editor` | ½ \| τ×2 − τ/2 \| | Andreev 2024 §6.4 |
| `source_unfolding_MCstat.json` | REFORMAT from `sigma_MCstat_TUnfold_4d_v3final.json` | `code-editor` | response MC-stat | Andreev 2017 §6 |

### 4.B Sources to leave as BLOCKED metadata stubs (require Condor v6sys MC response)

Emit a stub JSON (schema in §3) for each of these, ensuring the combiner treats them as zero-contribution-not-yet-produced rather than missing:

| File | H1Calc enum | Magnitude | Run plan |
|---|---|---|---|
| `source_electron_energy.json` (REPLACE current critic-FATAL rank-1 file with stub) | `eElecEn=7` | ±0.5% per 5 LAr wheels | v6sys Stage1+Stage2 `electron_energy_{up,down}`, per period (em0405 + ep0405 + ep0607b + ep0607a), per DJANGOH + RAPGAP MC; ~3 h Condor wall per side per period, ~4 sides × 4 periods × 2 generators × ~30 min = ~16 h Condor wall total |
| `source_electron_theta.json` (REPLACE current critic-FATAL rank-1 file with stub) | `eElecTh=10` + `eElecTrTh=13` | ±1 mrad per 3 θ regions + matched track θ | Same as above with `electron_theta_{up,down}` |
| `source_hfs_jes.json` (NEW stub) | `eHadEn=0` (in-jet) | ±1% in-jet HFS | v6sys Stage1+Stage2 `hfs_jes_{up,down}`, requires in-jet/out-of-jet classifier in builder |
| `source_hfs_rces.json` (NEW stub) | `eHadEnRCES=1` (out-of-jet) | ±1% out-of-jet HFS | v6sys Stage1+Stage2 `hfs_rces_{up,down}` |
| `source_hfs_theta.json` (NEW stub) | `eHadTh=4` | ±1 mrad | v6sys Stage1+Stage2 `hfs_theta_{up,down}` |
| `source_hfs_phi.json` (NEW stub) | `eHadPh=5` | ±20 mrad | v6sys Stage1+Stage2 `hfs_phi_{up,down}` |

### 4.C Sources to DROP from v6sys baseline (with explicit deprecation note)

| File | Drop reason | Replacement |
|---|---|---|
| `source_electron_phi.json` (current critic-FATAL) | No g1j_ES recipe; no published HERA-II `eElecPh` magnitude for SIDIS | DROP or BLOCK; do not retain rank-1 critic-fatal cov |
| `source_tracking_resolution.json` (current critic-FATAL) | No H1-paper precedent for explicit tracking-precision systematic; absorbed in HFS/JES at H1 | DROP from v6sys; if reviewer insists, run ±50% smearing study with response rebuild as a separate non-precedent term |
| `source_tracking_efficiency.json` (current) | Supersedes by `source_tracking_central_h1_1302_1321.json` (item #9) which is the H1-precedent form | DEPRECATE; rename to `source_tracking_efficiency.json.DEPRECATED.md` documenting that #9 is the canonical replacement |
| Noise source | No pinned magnitude; not in published HERA-II SIDIS budget | DROP |

### 4.D Validation criteria before any v6sys source is filled

For each item moved from BLOCKED stub to filled covariance, the following gates MUST close in order:

1. **Stage 1 v6sys event-processing smoke passes** for the source's syskey on at least one period of DJANGOH MC + data (per-period admitted-event count delta vs central, Stage 1 → Stage 2 chargedtot row count delta, per-bin mean ΔQ²es / ΔYes on Scheme B′ grid). Owner: `h1-builder-engineer` + `h1-script-runner`.
2. **Per-wheel / per-region magnitude application audit** confirms the builder applies the array (`SetShiftElecEnWheel(w, ±0.005)` for all 5 wheels; `SetShiftElecTheta(r, ±0.001)` for all 3 regions) rather than a single global `SetShift`. Owner: `h1-builder-engineer`.
3. **Symmetric ± production** completed (both `up` and `down`). Owner: `h1-naf-operator`.
4. **Response matrix rebuilt per iSys** from shifted reco + nominal gen. Owner: `h1-unfolder`.
5. **Per-iSys TUnfold-4D unfold** of nominal data with shifted response completed. Owner: `h1-unfolder`.
6. **Noise floor characterized** for the source's per-bin shift δ on the 480-bin grid. Bins where σ_unfold ≳ \|δ\| must be MASKED or noise-floor-corrected before any rank-1 cov is admitted (`research-electron-energy-systematic-critique-2026-05-26.md`). Owner: `h1-systematics`.
7. **Critic gate (`h1-analysis-critic`)** signs off that the per-source covariance respects the noise floor and that no rank-1 outer-product cov is being shipped on noise-dominated bins.

Until all seven gates close, the source stays as a BLOCKED metadata stub.

### 4.E Final budget structure (post-v6sys completion)

The frozen budget structure for high-Q² SIDIS multiplicities M^h(Q², x, z, pT_h) and asymmetries A_h follows Andreev 2024 §6 + Andreev 2023 §4:

```
σ²_M(c) =
    σ²_stat,IBU(c)                                  # bootstrap, in-place
  + σ²_model_DJvsRG(c)                              # in-place
  + σ²_reg(c)                                       # in-place
  + σ²_MCstat,response(c)                           # in-place
  + σ²_eElecEn(c)                                   # BLOCKED → v6sys paired run
  + σ²_eElecTh(c)                                   # BLOCKED → v6sys paired run
  + σ²_eHadEn,in-jet(c)  ⊕  σ²_eHadEn,out-of-jet(c) # BLOCKED → v6sys paired run
  + σ²_eHadTh(c)                                    # BLOCKED → v6sys paired run
  + σ²_eHadPh(c)                                    # BLOCKED → v6sys paired run
  + σ²_tracking,central(c)                          # DONE (1.0198% diagonal, #9)
  + σ²_crad(c)                                      # in-place
  + σ²_NucliaWeight(c)                              # in-place
  + σ²_trigger(c)                                   # in-place placeholder
  # For absolute cross sections only:
  + σ²_lumi(c)                                      # in-place (2.7%)

σ²_A(c) =
    σ²_stat,IBU(c)
  + σ²_model_DJvsRG(c)                              # mostly cancels; small residual
  + σ²_reg(c)
  + σ²_MCstat,response(c)
  + σ²_tracking_chargedep(c)                        # in-place (2.5% on A_h, #11)
  + σ²_eElec/eHad asymmetry residuals (c)           # BLOCKED; mostly cancel
  # lumi, trigger, NucliaWeight, crad explicitly cancel in A_h
```

The DOMINANT terms in M^h are expected to be: σ_model_DJvsRG (5–10% per bin, Andreev 2024), σ_eHadEn ⊕ σ_eHadEnRCES (0.3–3% per bin, Andreev 2023 Tables 2–3), σ_eElecEn (≤2% per bin), σ_tracking,central (1.02% flat, item #9), σ_lumi (2.7% normalization).
The DOMINANT term in A_h is expected to be: σ_tracking_chargedep (2.5%, Aaron 2009).

---

## 5. Concrete answers to the four explicit questions

### Q1. Which H1-published systematics should be in v6sys/high-Q² release now, categorized?

See the decision table in §1. Summary by category:

- **Category A — already present in rev6 pipeline:** model dependence DJvsRG (#15), unfolding regularization (#16), unfolding statistical bootstrap (#17), response MC-stat (#18), C_rad (#23). All five exist in the pipeline today and only need to be reformatted into the `source_*.json` schema by `code-editor` for the combiner.
- **Category B — post-unfolding diagonal source, fillable now:** central tracking 1.0198% (#9, DONE 2026-06-03), charge-dependent tracking 2.5% on A_h (#11, exists, sanity-check needed).
- **Category C — REQUIRES v6sys paired Stage1+Stage2 shifted MC response:** electron_energy (#1), electron_theta (#2), HFS JES (#4), HFS RCES (#5), HFS theta (#6), HFS phi (#7). All six BLOCKED pending v6sys production; emit metadata stubs now with pinned magnitudes; do NOT fill with rank-1 cov.
- **Category D — handled elsewhere (normalization / background / model):** lumi (#20, DONE), trigger (#21, DONE placeholder), NucliaWeight (#22, DONE), γp 30% (#14, negligible at high-Q², emit only if subtracting), method spread (#19, NOT a systematic).
- **Category E — not applicable to high-Q² SIDIS:** combined/forward 10% tracking (#12, excluded by |η_h|<1.3 fiducial), SpaCal energy (#24) and SpaCal angular (#25) (electron lands in LAr at high Q²), eΣ-method-specific (#26, folded into items #1–#7).
- **DROP from v6sys baseline:** electron_phi (#3, no pinned magnitude), noise_energy (#8, no pinned magnitude), tracking_resolution (#13, no H1 precedent), Aaron 2.5% alternative (#10, decision pending).

### Q2. For electron/HFS H1Calc sources, exact syskeys, magnitudes, H1Calc default vs explicit?

See §2 for the full freeze. Highlights:

- **Use explicit magnitudes**, not H1Calc default shifts, for all PINNED sources. The g1j_ES recipe (`AnalysisBase.C`) uses explicit per-wheel `SetShiftElecEnWheel(w, ±0.005)` and per-region `SetShiftElecTheta(r, +0.001)` calls, not defaults. Defaults are reserved for `--sys-allow-default` infrastructure smoke only and MUST NOT be used in publication-grade runs.
- **PINNED magnitudes:** `eElecEn` ±0.5% per 5 wheels; `eElecTh` +1 mrad per 3 regions + matched `eElecTrTh`; `eHadEn` (JES) ±1% in-jet; `eHadEnRCES` (RCES) ±1% out-of-jet; `eHadTh` ±1 mrad; `eHadPh` ±20 mrad.
- **NOT PINNED (mark BLOCKED):** `eElecPh` (no g1j_ES recipe, no SIDIS-regime HERA-II prescription); `eNoiseEn` (no pinned SIDIS-regime magnitude).
- **Symmetric ± required** for SIDIS, NOT the g1j_ES one-sided convention. Asymmetry A_h needs symmetric ± to permit physical charge-cancellation. Sum both signs into per-source quadrature; do not drop the down-side.

### Q3. Stubs or numerical covariances now?

**Stubs now for all category C sources.** Source-anchored reasoning in §3.

The gate is unambiguous: do not generate numerical per-bin covariances for any migration-affecting source until the v6sys paired Stage1+Stage2 shifted-MC response production has run for that source. The currently existing rank-1 covariances (`source_electron_energy.json`, `source_electron_theta.json`, `source_electron_phi.json`, `source_tracking_resolution.json`) have all been flagged critic-FATAL and must be replaced by BLOCKED metadata stubs in the v6sys release, NOT carried forward.

Reformatting category A sources (unfolding stat / reg / MC-stat / model) into `source_*.json` schema is a `code-editor` task that does NOT require v6sys production; do it in parallel with the v6sys production campaign.

### Q4. Implementation handoff: emit now / leave BLOCKED / validation criteria

See §4 for the full handoff. Compact form:

- **EMIT NOW (no Condor):** tracking_central (DONE), tracking_chargedep (sanity-check), lumi (DONE), trigger (DONE placeholder), NucliaWeight (DONE), crad (reformat), model_DJvsRG (reformat), unfolding_stat (reformat), unfolding_reg (reformat), unfolding_MCstat (reformat).
- **LEAVE BLOCKED as metadata stubs, REQUIRES v6sys Condor production:** electron_energy (`eElecEn`), electron_theta (`eElecTh`+`eElecTrTh`), hfs_jes (`eHadEn` in-jet), hfs_rces (`eHadEnRCES` out-of-jet), hfs_theta (`eHadTh`), hfs_phi (`eHadPh`).
- **DROP (with deprecation note):** electron_phi (no magnitude), noise_energy (no magnitude), tracking_resolution (no H1 precedent), legacy tracking_efficiency (superseded by tracking_central_h1_1302_1321).
- **NOT APPLICABLE:** combined/forward 10% tracking, SpaCal energy, SpaCal angular.
- **Validation gates (per source before filling):** seven-gate sequence in §4.D — Stage 1 smoke → per-wheel/region audit → symmetric ± production → response rebuild → unfold per side → noise-floor characterization → critic sign-off.

---

## 6. Risks, assumptions, open questions

### Risks

1. **In-jet/out-of-jet classifier mismatch.** Andreev 2014 calibrated the JES 1% against the k_T R=1.0 pT>5 GeV jet collection. If the v5g/v6sys SIDIS builder uses a different jet collection (anti-k_T, different R, different pT threshold), the 1% calibration is not directly inheritable. Mitigation: confirm the v5g jet collection in implementation; if it differs, build a parallel JES-classifier collection inside v6sys for the in/out tagging. If still impossible, collapse JES + RCES into a single ±1% uniform HFS shift (gives the same quadrature envelope but loses per-bin correlation structure).
2. **`eHadEnRCES` enum value.** The v6sys plan lists `eHadEnRCES=1`. The H1OO release inspected for g1j_ES (`relvol5/3.4.13`) has `eHadEnCor=1`, not `eHadEnRCES`. The audited enum in the SIDIS-linked `relvol10/4.2.1` may use a different label. Mitigation: `h1-software` should print `int(H1CalcSystematic::eHadEnRCES)` against the SIDIS-linked H1OO build at implementation time to confirm the integer that ends up on disk.
3. **`eElecPh` and `eNoiseEn` magnitude search.** Both are mapped in the H1Calc enum but have no pinned SIDIS-regime magnitude. There is some probability that an internal H1 note or an unsurveyed paper pins them; if not, they stay dropped.
4. **Down-side coverage for HFS phi.** ±20 mrad is large; check that the response matrix doesn't go pathological at the edge of φ_h coverage.
5. **Background γp 30%.** If a γp template is actually being subtracted from data in the rev6 chain, the 30% template variation must propagate; if no template is subtracted, the 30% term is omitted and the residual γp is documented as "negligible" per Andreev 2023.
6. **Method-spread inclusion pressure.** A reviewer may push to include the 3-method (BBB/IBU/TUnfold) spread as a systematic. The literature is clear (Brenner-Burgard-Croft 2025; H1 collaboration practice in Andreev 2014/2017/2023/2024) that this is a coverage diagnostic, not a systematic. Mitigation: document this explicitly in the analysis note and show the 3-method comparison in supplementary material.

### Assumptions

1. The v6sys plan's H1Calc enum values (`eHadEn=0`, `eHadEnRCES=1`, `eHadEnCor=2`, `eHadTh=4`, `eHadPh=5`, `eNoiseEn=6`, `eElecEn=7`, `eElecEnCor=8`, `eElecEnUnc=9`, `eElecTh=10`, `eElecPh=11`, `eElecTrEn=12`, `eElecTrTh=13`, `eElecTrPh=14`) are correct for `relvol10/4.2.1` per the v6sys plan audit. Confirm before production runs.
2. The Stage 1 builder honors a `ResetAllShifts(); Reset()` discipline before evaluating `gH1Calc`-dependent preselection. Confirm in implementation.
3. The 480-bin Scheme B′ grid is the canonical bin grid for the v6sys release. Confirmed per AGENTS.md "Scheme B′ frozen as canonical (Q²,x) binning on 2026-05-24".
4. The eΣ DIS-kinematic method is the active reconstruction. Confirmed per `context-handoff.md`.
5. DJANGOH 1.4 is nominal MC; RAPGAP 3.1 is alternative MC for σ_model. Standard at H1 high-Q².

### Open questions

1. **Conservative vs best-fit central-tracking ceiling.** Adopt 1302.1321 1.0% (current, item #9) or Aaron 2009 2.5% (Aaron 2009 explicitly quotes 2.5% on D±)? The 1.0% form is the best-fit-inside-fiducial value; the 2.5% form is the conservative ceiling matching Aaron's per-track quote even when the dense-jet inflation does not apply. Supervisor decision; default = 1.0% (already emitted).
2. **Drop or block electron_phi and noise_energy?** Without a pinned magnitude, the cleanest decision is DROP. If supervisor wants to retain them as future expansion slots, BLOCK with NULL magnitude and document.
3. **Tracking resolution methodology decision.** The ±50% smearing approach is not literature-anchored. Either DROP (recommendation) or accept as an "in-house" non-precedent term with explicit caveat in the analysis note.
4. **HFS JES jet-classifier audit.** Match Andreev 2023 jet definition? Requires implementation-time verification.
5. **Aaron 2009 e⁺/e⁻ charge-bias cross-check on our 2005–2007 HERA-II sample.** Feasible (2005 e⁻ vs 2006–07 e⁺) but a separate study. Reduces the 2.5% A_h envelope if completed.

### Gaps in coverage

- I did not retrieve the full text of arXiv:0904.3513 (the original reference for the 0.5% / 0.2 mrad electron-side magnitudes); the values are inherited transitively via g1j_ES `AnalysisBase.C:82–84` and via Andreev 2014/2017/2023/2024 citation chains.
- I did not verify the v5g jet collection definition against Andreev 2023 §1. This is an implementation-time check.
- I did not retrieve any H1 internal calibration note for `eNoiseEn`; if one exists, it would un-block item #8.
- I did not survey the Aaron 2007 (arXiv:0706.2456) full systematics table beyond what is in `research-tracking-systematics-2026-05-25.md`. The tracking memo extracted the 2.5% per-track and the dense-jet inflation; the rest of the table (SpaCal-energy-equivalent LAr-energy for high Q²?) was not separately confirmed.

---

## 7. References

- arXiv:1302.1321 — H1, "Charged Particle Spectra in DIS at HERA" (EPJC 73 (2013) 2406). Source for central-tracking 1.0%, K0S 0.2%, SpaCal 1%, SpaCal angular 1 mrad, HFS 2% (low-Q² regime), model DJ vs RG, γp 30%.
- arXiv:0706.2456 — Aktas et al. (H1) PLB 654 (2007) 148. Source for high-Q² inclusive tracking 2.5% per-track + dense-jet inflation +2%/+5%.
- arXiv:0907.2666 — Aaron et al. (H1) PLB 681 (2009) 125. Source for charge-dependent tracking 2.5% on A_h anti-correlated.
- arXiv:1406.4709 — Andreev et al. (H1) EPJC 75 (2015) 65. Canonical reference for the JES + RCES ±1% calibration at high-Q² HERA-II.
- arXiv:1611.03421 — Andreev et al. (H1) EPJC 77 (2017) 215. JES + RCES ±1%; "1% uncorrelated everything-else" lump.
- arXiv:2108.12376 — Andreev et al. (H1) PRL 128 (2022) 132002. OmniFold + JES/RCES ±1%; no separate tracking systematic.
- arXiv:2303.13620 — Andreev et al. (H1) PLB 844 (2023) 138101. **Most directly comparable to our SIDIS analysis** (same dataset subset, same Q²>150 GeV², same Σ-method, same fiducial). JES/RCES ±1%, HFS φ ±20 mrad, per-bin systematics tables for ⟨Nc⟩.
- arXiv:2403.10134 — Andreev et al. (H1) EPJC 84 (2024) 718. Groomed event shapes. JES + RCES ±1%, HFS θ alignment 1 mrad, lumi 2.7%. Most recent H1 TUnfold high-Q² systematics statement.
- arXiv:0904.3513 — H1 calibration reference for electron energy 0.5% per wheel and electron θ 0.2–1 mrad. Cited by g1j_ES `AnalysisBase.C:82–84`.
- arXiv:1010.0632 — D'Agostini, "Improved iterative Bayesian unfolding". Source for IBU model + iteration regularization decomposition.
- arXiv:1205.6201 — Schmitt, "TUnfold". Source for τ-scan regularization systematic.
- arXiv:2510.15129 — Brenner, Burgard, Croft 2025. Modern likelihood restatement; method-spread is coverage, not systematic.
- Local artifacts cross-referenced (read in full):
  - `sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`
  - `sidis_highQ2/docs/research/research-tracking-systematics-2026-05-25.md`
  - `sidis_highQ2/docs/research/research-HFS-systematic-2026-05-25.md`
  - `sidis_highQ2/docs/research/research-g1jES-electron-LAr-systematics-2026-06-01.md`
  - `sidis_highQ2/docs/research/unfolding_systematic_decomposition_2026-05-24.md`
  - `sidis_highQ2/docs/research/research-v6sys-smoke-physics-review-2026-06-03.md`
  - `sidis_highQ2/plan-v6sys-treebuilder.md`
  - `sidis_highQ2/data/systematics_sources/tracking_central_h1_1302_1321/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/source_tracking_central_h1_1302_1321.json`

---

## 8. Status

**Written artifact:** `H1/sidis_highQ2/docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` (this file).
**Scope covered:** Frozen decision table for 27 H1-precedent systematic source slots; H1Calc syskey/enum/magnitude freeze for the 8 electron/HFS/noise sources scoped into v6sys; per-source emit-now / block / drop policy; concrete implementation handoff with seven-gate validation sequence; budget structure for M^h and A_h; risks, assumptions, open questions, and gaps explicitly flagged.
**Areas needing further investigation:**
- Confirm v5g jet collection definition vs Andreev 2023 (Section 6, risk 1).
- Confirm `eHadEnRCES` integer value in `relvol10/4.2.1` (Section 6, risk 2).
- Survey for any H1 internal calibration note pinning `eElecPh` and `eNoiseEn` (Section 6, risk 3).
- Aaron 2009 e⁺/e⁻ charge-bias cross-check feasibility on the 2005–2007 sample (Section 6, open question 5).
**No code was edited, no remote commands were run.**

*End of frozen menu.*
