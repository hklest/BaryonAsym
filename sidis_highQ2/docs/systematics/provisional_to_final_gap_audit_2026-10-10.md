# PROVISIONAL→final gap audit and rederivation plan — 13-component z005 budget (2026-10-10)

Read-only gate audit before any rederivation. No reruns were launched; no central or
budget product was modified. Companion records: `budget_13components_derivation_2026-10-10.md`
(component derivations), `plotted_band_vs_budget_2026-10-10.csv` (band audit).

**Executive verdict: zero of the 13 components are STALE by chain drift.** The analysis
changes since the `_rel` components were produced (coherent-prior swap 2026-10-07;
releasebin C_rad rederivation 2026-10-09 + emfix) either predate the 2026-10-07
regeneration of every IBU-based component or touched only the C_rad tables, whose two
budget components were rebuilt the same day. The promotion problem is **prescription-gap
closure** (one-sided detector legs, diagnostic phi_h, B1 flag, lumi memo), **not**
re-derivation on a changed chain. One chain-hygiene defect was found (HFS shifted
input-list mismatch, §3.4) that must be cured inside the paired reruns.

## 1. Current canonical chain basis (with freeze dates)

| Basis element | Definition | Frozen / current since |
|---|---|---|
| Binning | finalcandidate S3-merged bin map: gen z edges `[0.0,0.05,0.1,0.2,0.3,0.5,0.7,1.0]`; S3 cell `1500<=Q2<20000`, `0.021<=x<1.5`; 245 gen bins/charge, 980-entry data vector, 490 published rows. `zlow005` = first-bin edge, not a cut. | 2026-06-10 (finalcandidate v4); unchanged through all releases |
| Selection (reco) | `v4_fid4cut_2026-06-06_reco_only`: `150<Q2es<20000`; `0.1<Yes<0.7`; `Empz>45` (no upper bound); `xF_SIDIS_es<0`; cleanup `|PVz|<35, Pt>0.15, |Eta|<1.6, NHitDedx>=5, RadLength>=5, |DcaPrime/cosh(Eta)|<2`, primary, not e-matched, not V0 daughter, `0<=z<=1`; Stage-1 reco-only alpha cone 0.2 | 2026-06-06 (fid4cut); data vector file dated 2026-06-10 |
| Selection (gen / RAD-sample truth) | `150<Q2_es_gen<20000`; `0.1<Y_es_gen<0.7`; `GenEmpz>45` (no upper bound, retained by user decision 2026-10-09); `xFGen<0` (current hemisphere); `|GenPDG| in {211,321,2212}`; gen-x binned with rad-safe **`x_es_gen`** (response builder `build_migration_th2_per_charge_v4.py:145,353,363,503-504`) | Predates all component production; unchanged |
| Data vector | `data_vector_..._percharge.json`: **658 files, four periods incl. ep0304** (ep0304:83, em0405:152, em06:100, ep0607:323), raw/no-bgsub, 3,884,097 raw tracks | 2026-06-10; unchanged through the coherent swap (release README: "only RAPGAP prior consumption changed") |
| Response | finalcandidate v4 migration ROOTs (RAPGAP31 central, DJANGOH14 branch); coherent rebuild 2026-10-06 reproduced the central body (prior extraction only) | 2026-06-10 body; 2026-10-06 prior extraction |
| Prior / unfolding | coherent `body+miss`, IBU k=4, `L=351000.0 nb^-1` | central swap 2026-10-07 |
| C_rad | releasebin tables (DJANGOH14 applied primary, RAPGAP31 formal alternative), `(Q2esGen, x_es_gen)` binning, `GenEmpz>45`, `xFGen<0`; Born release 2026-10-09 | 2026-10-09 (emfix same day) |
| Budget | `total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09/` (supersedes the non-emfix package; only `c_rad_gen_rel` differs) | 2026-10-09 |

## 2. Candidate analysis changes — checked and dispositioned

| Change | Date | What it actually touched | Budget impact |
|---|---|---|---|
| (a) Releasebin C_rad rederivation (A-F1) | 2026-10-09 | **C_rad tables only.** F1 root-cause doc: the only semantic diff in the C_rad workers is `bq,bx = Q2esGen,XesGen -> Q2esGen,x_es_gen`; DIS gate, `Q2Gki>100`, safe-xF, hadron/V0 selection, z/pT, weights byte-identical. Response/unfolding legs untouched — the response has always binned gen-x with `x_es_gen` (builder source lines above); A-F1 existed precisely because the *old C_rad tables* mismatched the response convention | `c_rad_gen_rel`/`c_rad_stat_rel` rebuilt same day on the new tables; emfix corrected `c_rad_gen_rel` only (RG31 e- exposure repair, medians h+ 0.5505→0.5844%, h− 0.5166→0.5804%); other 11 components byte-identical (0/14,700 mismatches) |
| (a′) RG31 em exposure repair (emfix) | 2026-10-09 | RG31 NONRAD e- denominator only (+53,773.7 nb^-1); applied DJ14 primary unchanged | `c_rad_gen_rel` only (above) |
| (b) Cut changes (radlen5/nhit5/pT5/eta1p6/zlow005, EMPZ, ep0304) | — | **None found.** Release tag constant across the 2026-06-10 → 2026-10-09 releases; `SELECTION.md` documents the finalcandidate v4 chain as release-effective; data vector unchanged since 2026-06-10. EMPZ>45 convention decision (2026-10-09) = retain, no applied change. ep0304 was **already included** in the 658-file data vector at 2026-06-10; the 2026-09-23 normalization-basis repair changed only the lumi **constant** (297657.8 → 351000.0 nb^-1), not data content | None; relative components invariant under the lumi-constant repair (regime-B invariance, normalization gate 2026-10-09) |
| (c) RAD-sample truth-variable changes beyond C_rad binning | — | **None.** The response truth contract (`Q2_es_gen, Y_es_gen, x_es_gen`, `GenEmpz>45`, `xFGen<0`) predates every component; the only truth-variable change in the window is the C_rad x-bin assignment (row a), and the affected components were rebuilt on it. The z<0.05 xF-definition sensitivity (DJ14 max 9.925% h+) is CAVEAT-ONLY since 2026-10-09 (user decision, no budget component) | None outstanding |
| Coherent-prior swap | 2026-10-06/07 | IBU prior initialization + efficiency only (response body, data vector, fake handling, k=4, binning unchanged) | All IBU-based components regenerated 2026-10-07 on the coherent lineage (stat toys, model/reg, detector re-unfolds, tracking ensemble, B1 floors) |
| fLumi pb^-1 correction | 2026-09-25 | MC/background lumi bookkeeping (DjBkgd sums) | Voided the old `background_rel` retirement rationale (register row 17); no `_rel` component change |

## 3. Per-component validity table

Verdicts: **VALID** = produced on the current chain, no invalidating change since
production. **STALE** = produced on a superseded basis. **UNVERIFIABLE** = provenance gap.
Final-gate caveats are prescription/decision items, not staleness.

| # | Component | Produced | Chain basis it was produced on | Verdict | Final-gate caveats (not staleness) |
|---|---|---|---|---|---|
| 1 | `ibu_stat_rel` | 2026-10-07 (1000 toys, cluster 5380017, seeds 12345/22345) | coherent release, fixed RAPGAP response, coherent prior, k=4, 658-file four-period data | **VALID** | h+/h− stat cross-block is zero-by-assumption; must be measured/justified for final `A_h` (spec row 1, decision 11) |
| 2 | `ibu_model_rel` | 2026-10-07 base (cluster 5379991) + B1 floor (cluster 5380196) | coherent lineage, replica-own `body+miss` priors, B1 replica floor `max(base, floor)` | **VALID** | P-RAPGAP (RAPGAP response/provenance disposition) OPEN; B1 3-SEM working-point FAIL on 1 h+ / 3 h− supported bins (report-only); covariance representation open |
| 3 | `ibu_reg_rel` | 2026-10-07 (k-scan, cluster 5379991) | coherent lineage, `0.5·|IBU(k=5)−IBU(k=3)|` | **VALID** | final covariance needs a signed sidecar on the v6 basis (spec row 3) |
| 4 | `hfs_jes_rel` | 2026-10-07 re-unfold of legacy one-sided `hfs_jes_down` artifacts (finalcandidate v4 chain, 2026-06-25 shifted vectors) | same selection/binning/response family as central; coherent prior from the shifted response | **VALID** (chain) — flagged defect §3.4 | PROVISIONAL prescription: one-sided down, not paired ±1% in-jet; jet-classifier audit missing (spec row 4) |
| 5 | `hfs_rces_rel` | 2026-10-07 re-unfold of legacy `hfs_rces_up` | same as #4 | **VALID** (chain) — flagged defect §3.4 | one-sided up; ±1% out-of-jet paired required; `eHadEnRCES` relvol10 enum audit (row 5) |
| 6 | `hfs_theta_rel` | 2026-10-07 re-unfold of legacy `hfs_theta_up` | same as #4 | **VALID** (chain) — flagged defect §3.4 | one-sided up; audited two-sided response is even/quadratic (signed means +0.2568%/+0.2588%) and quarantined (row 6, decision 3) |
| 7 | `elec_energy_rel` | 2026-10-07 re-unfold of legacy `elecE_p1pct` (+1%) | same chain; shifted vector on the exact 658-file list | **VALID** (chain) | +1% ≈ 2× the frozen ±0.5%-per-LAr-wheel amplitude; five-wheel explicit paired rerun required; "do not rescale the existing vector" (row 8, §2.2-1) |
| 8 | `elec_theta_rel` | 2026-10-07 re-unfold of legacy `elecTheta_p1mrad` | same chain; 658-file list | **VALID** (chain) | +1 mrad alone; frozen source is three-region `eElecTh` **co-shifted with matched-track `eElecTrTh`**; setter audit + paired rerun (row 9, §2.2-4) |
| 9 | `lumi_rel` | 2026-06-13 (flat assignment) | normalization-only; chain-independent; "L_data does not enter any budget arithmetic" | **VALID** | derivation-artifact gap (§6.3); rank-1, excluded from multiplicities/`A_h` |
| 10 | `tracking_eff_m0p5_rel` | 2026-10-07 coherent regeneration (16/16 seeds, per-seed legacy responses + four-period seed vectors) | coherent lineage, seed-own `body+miss` prior | **VALID** | baseline choice: delivered −0.5% ensemble vs frozen 1.0%⊕0.2% analytic (register rule 4, decision 8); no silent substitution |
| 11 | `phi_h_gauss_rel` | 2026-10-06 coherent rederivation (floored; flat unfolds from the 2026-09-25 study, `body+miss` consistent) | coherent nominal; response-truth-marginal flat priors | **VALID** (chain) | diagnostic, not approved; adoption caveats mandatory (§6.1); P-PLOTS open |
| 12 | `c_rad_gen_rel` | 2026-10-09 (releasebin V3; emfix) | current releasebin tables (post A-F1, post emfix) | **VALID** (freshest) | C_rad itself NOT publication-grade (P-CRAD-LUMI generator-log-open; releasebin baseline PROVISIONAL); z<0.05 xF-def sensitivity caveat-only |
| 13 | `c_rad_stat_rel` | 2026-10-09 (applied DJ14 releasebin table `relative_stat_unc`) | current tables | **VALID** | rank-1 covariance is a 100%-coherence envelope assumption, not measured; DJ e- RAD 4.67% count shortfall is precision-only and carried here |

**Census: VALID 13 / STALE 0 / UNVERIFIABLE 0.**

### 3.4 Flagged chain-hygiene defect (does not rise to STALE)

The three HFS shifted data vectors were produced from a **660-file** list — `ep0607: 325`
vs the central 323 (two extra files) — dated 2026-06-25; shifted/central total raw counts =
1.000456 (+0.046%). The selection contract, binning, and response chain are identical
(`v4_fid4cut_2026-06-06`), so this is an input-list inconsistency, not a superseded basis;
its magnitude is sub-dominant to the one-sided prescription problem. The electron shifted
vectors used the exact 658-file list (verified for both tags). **Action:** all paired
reruns must pin inputs to the canonical central 658-file list; the one-sided HFS values
should not be promoted as-is regardless (they fail the paired prescription).

## 4. Detector legs 4–8: frozen final prescriptions and rerun scope

From `FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` rows 4–9, §2.2, §3.1–3.3, §5.3–5.8.
All five are migration-affecting and require **paired Stage1+Stage2** reruns: shifted re-reco
of data AND of MC with the nominal truth definition, shifted response rebuild, coherent-prior
unfold, 490-row finite output, closure/refold/stability checks, noise-floor/same-side tests.
Symmetrisation: coherent half-difference `|(X+−X−)/2|` only after the noise/coherence test;
approved max-side envelope only for a documented nonlinear response; one-sided values are
excluded from the final detector tier (register rule 1–3).

| Leg | Frozen final prescription | Reruns required | Scope per side |
|---|---|---|---|
| `hfs_jes` | `eHadEn` **±1% only inside kT R=1.0, pT>5 GeV, −1.5<η_lab<2.75 jets** (Andreev 2023 PLB 844 §4; 2024 EPJC 84 §6.2) | jet-classifier audit → paired ± Stage1+Stage2 → response rebuild → unfold → noise test | 658 data + 364 RAPGAP31 MC files ≈ 1,022 re-reco jobs (+502 if the DJANGOH14 branch is also shifted) |
| `hfs_rces` | `eHadEnRCES` **±1% outside those jets**, independent of JES | relvol10 enum/classifier verification → same paired machinery | same |
| `hfs_theta` | `eHadTh` **±1 mrad** | first resolve the quarantined even/quadratic two-sided response (diagnose vs rerun); then paired ± | same |
| `elec_energy` | `eElecEn` **±0.5% per each of five LAr wheels** (BBE, CB1+2, CB3, FB1, FB2+IF); all five explicit wheel setters per side; coherent half-difference only if noise-qualified; **replace, do not rescale** the +1% vector | paired ± five-wheel Stage1+Stage2 | same |
| `elec_theta` | `eElecTh` **±1 mrad per each of three regions co-shifted with matched-track `eElecTrTh` ±1 mrad** | setter-level co-shift audit → paired ± | same |
| (required, absent) `hfs_phi` | `eHadPh` **±20 mrad** (row 7; no current column) | new paired ± production | same |

**Total production scope for the detector tier:** 6 sources × 2 sides = **12 shifted
campaigns ≈ 12,300 re-reco jobs** (RAPGAP-only MC leg) plus 12 response-merge/build +
data-vector + unfold + comparison chains, the classifier/enum/setter audits, and the
noise-floor/bootstrap test batteries. Prerequisites per campaign: canonical 658-file input
list (§3.4 fix), single-file smoke test, normalization check (L=351000.0, relative-only
outputs), coherent `body+miss` prior, k=4, binmap v3, comparison vs the coherent nominal.

## 5. What can be promoted WITHOUT reruns

- `lumi_rel` — documentation-only (§6.3).
- `ibu_stat_rel`, `ibu_reg_rel` — numerically final-eligible for cross sections once the
  covariance decisions (signed sidecar; h+/h− cross-block measurement or approved proof)
  are made; no rederivation.
- `tracking_eff_m0p5_rel` — pending the baseline decision (delivered ensemble vs frozen
  analytic); whichever is chosen requires no production.
- `phi_h_gauss_rel` — pending disposition (§6.1); the pure-split option needs no new
  production (decomposition sidecar exists).
- `ibu_model_rel` — pending P-RAPGAP disposition + B1 follow-up; no rederivation of the
  base cross-closure.
- `c_rad_gen_rel`, `c_rad_stat_rel` — chain-current; gated on the separate C_rad
  publication-grade approval (P-CRAD-LUMI), not on any budget rerun.
- **No component requires rederivation because of chain drift.** The rerun program is
  entirely the detector-tier prescription upgrade (§4) plus the new `hfs_phi` source.

## 6. Other flagged open items — disposition

### 6.1 `phi_h_gauss` (diagnostic, not approved)
Adopted by user decision 2026-10-02 with **mandatory caveats**: (i) the envelope is taken
against the central nominal, so it **includes fork/prior and DJ-vs-RG generator-model
content (~1.6% median, already covered by `ibu_model_rel`)** — pure phi_h flat-vs-unity is
~0.6% median (decomposition sidecar `plots/systematics/flatphi_env_7panel_2026-10-02/
flatphi_env_decomposition.csv`); (ii) no noise guard (Gaussian smoothing suppresses 10/37
h+ and 13/29 h− of the >2σ excursions); (iii) diagnostic status, P-PLOTS open.
**Disposition path (decision, no production):** either (A) adopt the floored envelope as
final with the double-count documented as a conservative bound, or (B) re-derive the pure
phi_h component from the existing sidecar and move the fork/model content into the model
term. Option B changes the budget arithmetic only.

### 6.2 B1 `b1_aggregation_status: FAIL` (report-only)
The 3-SEM replica-mean working-point gate fails on **1 h+ / 3 h− supported bins**
(b1floor README); the floor binds 150 h+ / 163 h− bins; certification classes
NOISE<2 / MIXED 2–3 / REAL≥3 with corrected h+ fractions 88.16/7.76/4.08%.
**Resolution requires:** a B1 follow-up attribution study of the 4 failing bins (more
replicas for those bins, or a documented redefinition/acceptance of the working-point
check). Small study; no detector production. Carried as "B1 follow-up attribution items"
in every promotion's does-not-cure list.

### 6.3 `lumi_rel` derivation-artifact gap
The value is ratified in current bookkeeping (`docs/status/current-release.md`: "flat 2.7%,
`lumi_rel = 0.027`, included in quadrature"; normalization gate 2026-10-09: "carried
byte-identical; L_data does not enter any budget arithmetic") and anchored to H1 precedent
(spec row 14: Andreev 2024 §6.3). Missing: an in-workspace first-principles derivation memo.
**Disposition:** request a short normalization-owner derivation note (or formally accept the
precedent citation as the derivation record). Documentation-only.

### 6.4 BLOCKED background rows (separate decision chain — NOT in scope for this promotion)
- `background_rel` (row 17): unblocking requires regenerated alpha0p2 aggregates with
  `overlap_980` and `n_raw_980`, template exposure provenance closure, **P-ALPHA**
  response-consistency validation, and a subtraction-policy decision (current convention:
  raw/no-bgsub with fixed response fake-fraction subtraction only, f = 10.7141% h+ /
  10.4019% h−, no propagated uncertainty).
- `diffvm_rel` (row 18): rho assessed 2026-10-07 (306.31±6.45 expected tracks, 0.0074%
  integrated — negligible); phi/omega/proton-dissociative remain unassessed. Unblocking =
  complete the DIFFVM assessment or document sample unavailability.
- `gammap_bkg_30pct_rel` (row 19): no gamma-p template is subtracted; per the spec,
  document no-subtraction rather than emit zero. Moot if raw/no-bgsub is ratified as final.
These belong to the P-ALPHA/background decision chain and the completeness ledger; they are
**not** curable by the detector-tier reruns and must not be silently added or zeroed.

### 6.5 Blocked register rows outside the 13 (for the ledger)
`hfs_phi` (row 7 — scheduled in §4), `tracking_chargedep_rel` (row 11 — 2.5%-on-`A_h`
precedent; blocks any `A_h` release), `response_mcstat_rel` (row 12 — needs a 490-bin
response-bootstrap covariance), `crad_rel` (row 13 — superseded in practice by the applied
releasebin components 12/13, gated on C_rad approval), `trigger_rel` (row 15 — ≤0.5%
placeholder), `NucliaWeight_rel` (row 16 — supervisor decision). Spec census:
IMPLEMENTED 4 / PROVISIONAL 6 / BLOCKED 9 / DROPPED 5.

## 7. Prioritized plan

**P0 — documentation-only, no reruns (immediate):**
1. Lumi derivation memo (§6.3) → `lumi_rel` final.
2. Source-completeness ledger per spec §5.2 (all 24 register rows, statuses, artifact paths).
3. Analysis-note corrections per spec §5.17 (±0.5%/wheel electron energy, ±1% in/out-of-jet
   HFS, `eElecTrTh` co-shift scope, one-sided/provisional labels, missing-source claims).

**P1 — decisions/small studies, no detector production:**
4. Tracking baseline decision (rule 4 / decision 8).
5. phi_h disposition, option A vs B (§6.1).
6. B1 follow-up attribution for the 4 failing bins (§6.2).
7. Symmetrisation + Tier-B covariance model decisions (§3.1–3.2) — must precede
   interpretation of the P2 campaigns.

**P2 — detector-tier production campaigns (the PROVISIONAL→final reruns), ordered:**
8. `eElecEn` paired ± five-wheel ±0.5% (largest current detector component, medians
   1.35/1.66%, and the biggest amplitude mismatch — 2× frozen). 2 sides ≈ 2,044 jobs.
9. `hfs_jes` ±1% in-jet and `hfs_rces` ±1% out-of-jet (classifier/enum audits first).
   4 sides ≈ 4,088 jobs.
10. `hfs_theta` ±1 mrad (after the even/quadratic quarantine diagnosis). 2 sides ≈ 2,044 jobs.
11. `elec_theta` three-region + `eElecTrTh` co-shift (setter audit first). 2 sides ≈ 2,044 jobs.
12. `hfs_phi` ±20 mrad (new source). 2 sides ≈ 2,044 jobs.
    — every campaign: canonical 658-file list, smoke test, normalization check, coherent
    prior, k=4, closure/refold + noise-floor/same-side validation; then rebuild the budget
    with paired components (non-detector components carried byte-identical; `hfs_phi`
    becomes a 14th component) and construct Tier-B covariances per the P1 decisions.

**P3 — blocked / separate decision chains (enumerated, not scheduled here):**
13. Background family (P-ALPHA, `overlap_980`/`n_raw_980` build, DIFFVM completion,
    no-subtraction documentation) — §6.4.
14. `response_mcstat` covariance, trigger measurement, NucliaWeight decision,
    charge-dependent tracking (2.5%-on-`A_h`), h+/h− statistical cross-block,
    P-RAPGAP, P-METHOD, and C_rad publication-grade approval (P-CRAD-LUMI).
15. Full covariance package rebuild + final plotter resolution (P-PLOTS) after P2.

## 8. Evidence consulted (primary)

- Budget packages: `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_{2026-10-09,_emfix_2026-10-09}/` (READMEs, `README_SUPERSEDED_2026-10-09.md`, validation_checks, provenance)
- F1 root cause: `data/radiative/djangoh14_crad_releasebin_2026-10-09/F1_CROSSCHECK_ROOT_CAUSE_2026-10-09.md` (binning-only diff)
- Selection contract: `.../data_release/bbb_ibu_coherentprior_..._2026-10-06/SELECTION.md` + `metadata.json` (data-vector path)
- Response builder source: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_percharge/build_migration_th2_per_charge_v4.py` (`x_es_gen` consumption)
- Data vectors: central `.../central/tunfold_official_v4_finalcandidate_..._percharge/data_vector_...json` (658 files, four periods, `v4_fid4cut_2026-06-06`, 2026-06-10) vs shifted `.../systematics/hfs/<tag>/.../data_vector_<tag>.json` (660 files, ep0607:325, 2026-06-25); elec vectors 658; disk counts `output_v6/central/sidistree/data/{ep0304,em0405,em06,ep0607}` = 83/152/100/323
- Component packages: `stat_toys_coherentprior_2026-10-07/`, `ibu_native_coherentprior_2026-10-07/`, `detector_shifted_coherentprior_2026-10-07/` (inputs.json), `tracking_ensemble_coherentprior_2026-10-07/`, `phi_h_coherentprior_2026-10-06/`, `studies/p1_replica_floors_coherentprior_2026-10-07/`, `total_uncertainty_z005_with_phih_coherentprior_b1floor_2026-10-07/` (B1 3-SEM gate report)
- Prescriptions: `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` (rows 4–9, §2.2, §3, §5); runbook `docs/systematics/z005_shifted_systematics_pipeline_2026-06-13.md`
- Adoption/caveats: `docs/systematics/flatphi_phih_systematic_adoption_2026-10-02.md`
- State: `STATE.md`, `docs/RUN_STATUS.md`, `docs/status/current-release.md`, `docs/status/normalization-gate-releasebin-2026-10-09.md`, `docs/status/radiative-corrections.md`
- MC file counts: `output_v6/central/sidistree/mc/{rapgap31,djangoh14}` = 364/502

No reruns were launched; this record is the gate before execution.
