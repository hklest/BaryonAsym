# Covariance-definition audit — 19-component clean-release budget vs the expert prescription (2026-10-17)

**Audit date:** 2026-10-17 (read-only; no central product modified, regenerated, or overwritten).
**Audited object:** the CURRENT CANONICAL systematics basis of the high-Q2 charged-hadron SIDIS analysis:

- Budget: `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/` (25 files; 19 components; 490 rows; cluster `5721117`, run_key `d4c92d90…`, computed UTC 2026-10-17 00:51:57; validation 29/29 PASS).
- Stat sidecar: `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/covariance/` (full 245×245 per-charge bootstrap covariance + correlation).
- Nominal denominator: official_reunfold IBU `k=4` on `data_sub′` — integrated h+ `10.50881084` / h− `10.21750255` nb (RAW level; L 351000.0 nb⁻¹).

**Context loaded before audit:** `sidis_highQ2/STATE.md` (incl. the 2026-10-17 Clean-Release Promotion Override), `sidis_highQ2/context-handoff.md`, skills `sidis-hq2-systematics` and `sidis-hq2-unfolding`, promotion record `docs/promotions/bgsub_cleanrelease_promotion_2026-10-17.md`.

---

## 1. The expert prescription audited against (verbatim points)

1. Per-source **signed shift vectors** δ_k = U_k − U_nominal, published as signed per-bin columns.
2. Variations built from **response rebuilds on the same MC events** as nominal (no independent-MC noise).
3. Shift sizes at the **1-sigma** level of each source.
4. **Explicit up/down leg handling** — both shift directions run; symmetrization `(U_up − U_down)/2` (or an explicitly recorded equivalent convention).
5. **Significance tests vs MC noise** (bootstrap/significance testing, smoothing where a variation is noise-dominated).
6. `ibu_stat` delivered as a **full bootstrap covariance matrix** (toys), not only diagonal.
7. A **C_MCstat term** — the response (MC) statistical uncertainty propagated as covariance.
8. The **unfolding's own systematic** — prior dependence (truth-reweight to the unfolded result) and/or iteration and model (alternative generator) — as named sources.
9. **Uncorrelated per-bin terms** kept diagonal.
10. **Publish the components**, including the total `C_total = C_stat + C_MCstat + Σ_k δ_k δ_kᵀ`, with a **near-singularity caveat** when rank-1 correlated terms are included.

---

## 2. Audit basis and method

Every claim below was checked against on-disk artifacts: the budget JSON and its 12 re-derivation records + carried-components record, the four `component_cache/` files, the output-contract JSON `systematics_charged_sidis_highQ2.json`, `validation_checks.json` (29 checks, read in full), `provenance.json`, the release-sidecar covariance JSONs, the shifted-legs campaign plan and sign-symmetry record (`cone_s2prime_shiftedlegs_2026-10-14/`), the legenv budget README, the B1 replica-floor study, and the four existing covariance-product directories (one current-basis, three stale-basis). Nothing was re-derived numerically; this is a definition-and-custody audit.

**Global budget convention (governs all 19 components):** every component is an **UNSIGNED per-bin relative magnitude**, `comp = |IBU_leg_sub − IBU_nom_sub| / IBU_nom_sub` (detector legs) or the recorded equivalent, on the **single denominator** (IBU k=4 on `data_sub′`), FULL ABS single-sided (`component_convention_full_abs_single_sided`: 0 negative cells of 18,620). Quadrature closure to 1.11e-16; hierarchy total ≥ every component in all 490×2 bins.

---

## 3. Component-by-component verdict table

Verdicts: **COMPLIANT** (definition sound per prescription, evidence on disk), **DEVIATION** (deliberate, documented departure from the prescription's form), **GAP** (required element missing/unevaluated), **NOT-APPLICABLE**. Medians are h+ / h− in percent, from the budget summary columns.

| # | Component | Definition as implemented | Key evidence | Verdict | What is missing |
|---|-----------|---------------------------|--------------|---------|-----------------|
| 1 | `ibu_stat_rel` (3.7299 / 4.0389) | 1000-toy bootstrap, Poisson-only `w2 = data_sub′` (SA-3), seeds 12345/22345; diagonal published in budget, **full matrix in the sidecar** (§4.1) | `rederivation_records/` ibu_stat record; release `covariance/ibu_toy_{covariance,correlation}_{pos,neg}.json`; check `ibu_stat_sidecar_consistency` (dev 1.11e-16) | **COMPLIANT** (matrix product) | Sidecar `nominal` key is an EMPTY list; no h+/h− cross-block (§4.1) |
| 2 | `ibu_model_rel` (1.3953 / 1.4255) | Unfolding model systematic: RG-vs-DJ **coherent half-difference** on data_sub′, `max(base, B1 prior-dependence floor)` — floor binds 152/167 bins | ibu_model rederivation record; check `b1_floor_mapping_vs_canonical_budget` (dev 0.0); B1 study `p1_replica_floors_coherentprior_2026-10-07/ensemble_b1/` (100 replicas, replica-own body+miss priors) | **COMPLIANT** (magnitude; the RG/DJ half-diff IS the prescription's alternative-generator form) | Signed δ column not published on the current basis (signed rank-1 exists only on the stale 2026-10-07 basis, §4.3); no truth-reweight-to-result iteration closure (§6) |
| 3 | `ibu_reg_rel` (0.5398 / 0.5604) | Iteration/regularization systematic: k5/k3 **half-difference** around nominal k=4 | ibu_reg rederivation record | **COMPLIANT** (magnitude) | Same signed-column publication gap as #2 |
| 4 | `hfs_jes_rel` (0.3081 / 0.3514) | Hadronic-scale leg: Stage-2 tree-level rerun (`ChargedSidisTreeBuilderHighQ2_v6 --sys-key`, eHadEn −0.01 JES) on **1:1-verified shifted twins of all 865 central MC files** → own shifted response + own coherent prior + own f_i → Set R′ subtraction → IBU k=4 → FULL ABS vs nominal | Campaign plan `CAMPAIGN_PLAN_cone_s2prime_shiftedlegs_2026-10-14.md`; `component_cache/detector*.json`; checks `stage3_custody_manifests_all_pass`, `unsubtracted_shifted_recompute_vs_stage3_csv` (28/28, dev 0.0) | **DEVIATION** | **Single-sided (plus-leg only); sign-symmetry UNSTUDIED** (deferred follow-up); unsigned — signed δ_k cannot be formed from existing products; no bootstrap significance test vs MC noise (§5) |
| 5 | `hfs_rces_rel` (0.2597 / 0.2638) | Same construction, eHadEnRCES +0.01 | Same | **DEVIATION** | Same as #4 |
| 6 | `hfs_theta_rel` (0.2520 / 0.2933) | Same construction, eHadTh +0.001 rad | Same | **DEVIATION** | Same as #4 |
| 7 | `elec_energy_rel` (1.6481 / 1.7059) | Electron-energy leg: **post-hoc kinematic substitution E′→1.01·E′ on the SAME central MC trees** (shifted builder) → shifted response/prior/f_i → Set R′ → IBU k=4; **leg-level envelope**: whole per-bin vector of the leg with larger robust median per charge (h+ ← minus 1.6481%, h− ← plus 1.7059%) | Campaign plan; `validation_checks.json` `elec_envelope_legenv_convention_fresh_selection` + `envelope_convention` block; sign-symmetry record 2026-10-14 (elecE_m1pct/pos flagged, minus median 1.4820% vs plus 1.2517%, +0.2303 pp; DJ chain does not reproduce) | **DEVIATION** | Envelope-of-abs **replaces** the prescription's `(U_up−U_down)/2` (supervisor-confirmed 2026-10-16, legenv precedent matched — but the up/down asymmetry is thereby absorbed, not symmetrized); **shift ±1% is ~2× the frozen ±0.5%/wheel menu 1-sigma** (documented in the stale v4 README; no two-size linearity check); electron legs are post-hoc substitutions, not paired tree reruns; no significance test (§5) |
| 8 | `elec_theta_rel` (0.4606 / 0.5189) | Same as #7 with ±1 mrad (both legs exist; envelope selects plus for both charges) | Same | **DEVIATION** | Same class as #7 (no leg-asymmetry flag recorded for theta); shift-size alignment with the frozen menu not separately documented |
| 9 | `lumi_rel` (2.7 flat) | 2.7% flat, **never rescaled** (multiplicative, denominator-invariant; loop-2 ruling); flagged bin-correlated in the output contract | `carried_components_record.json`; check `lumi_flat_0p027_never_rescaled`; `systematics_charged_sidis_highQ2.json` `correlated_sources` | **COMPLIANT** (as a fully correlated flat term) | For covariance use it must be **separated** from the per-bin quadrature into a rank-1 term (trivially constructible: 0.027·0.027·J); near-singularity caveat applies |
| 10 | `tracking_eff_m0p5_rel` (~0.51 / ~0.52 budget-column median; soft-median pins 0.4910/0.4929%) | **Data-side conservative form KEPT**: 16 no-cone seed DVs (tracking −0.5%) differenced through the nominal chain, `|mean − base_sub| / IBU_nom_sub`; caveat OPEN | `component_cache/tracking.json`; checks `tracking_canonical_form_vs_legenv_column` (dev 0.0), `trkcamp_csv_internal_consistency` (dev 0.0); STATE 2026-10-16 re-audit: **no full-chain tracking variant exists** | **DEVIATION** (documented, conservative) | Not a response rebuild on shifted MC (prescription point 2) — a data-side surrogate; charge-dependent tracking convention caveat carried OPEN; signed form exists in custody (`shift_mean − nominal`) but is published as abs |
| 11 | `phi_h_gauss_rel` (1.1024 / 1.0985) | Carried central ABS deltas re-denominated on the subtracted nominal (additive separability); **Gaussian smoothing**; 0.85× floor | Check `phi_h_central_recipe_reproduction` (dev 1.39e-17) | **COMPLIANT** (per-bin diagonal term; smoothing is the prescription's noise-mitigation form) | Smoothing width/floor provenance is carried (not re-derived on this basis); signed form not published |
| 12 | `c_rad_gen_rel` (0.5844 / 0.5804) | Generator spread of C_rad (WithZ sidecar v3, emfix, 490 rows); **level-invariant** (C_rad multiplicative) — carried bit-identical, re-verified dev 0.0 | Check `crad_v3_recompute_equals_carried` (0.0e+00 all three forms) | **COMPLIANT** | Signed/rank-1 covariance form exists only in the emfix-budget `covariance_cradstat/` package (relative forms level-invariant and valid; absolute forms on the stale coherent-prior raw nominal) |
| 13 | `c_rad_stat_rel` (0.4272 / 0.4429) | Statistical uncertainty of C_rad; same class as #12 | Same | **COMPLIANT** | Same as #12 |
| 14 | `detint_removal_rel` (0.0299 / 0.0287; subcolumns model/stat) | Removal-increment isolation recomputed on the subtracted chain: **model half-diff + pooled parentage stat**, `comp = hypot(model, stat)` | detint rederivation record; tail shrink p95 0.35→0.10 pp on the clean denominator (promotion record §3) | **COMPLIANT** (per-bin) | Parentage-stat pooling is an approximation (no full off-diagonal form); signed model half-diff in custody, not published |
| 15 | `fakecol_mcstat_rel` (0.3437 / 0.3494) | G12: σ(f_i) via per-reco-bin ±perturbations propagated through the **IBU k=4 Jacobian**, gen-bin quadrature — **diagonal only, fake column only** | fakecol rederivation record; check `g12_w2_partition_and_sumw2_pins` (dev 1.0e-15; pins 0.0) | **COMPLIANT** (as far as it goes) | Covers ONLY σ(f_i). The **body/matched response MC-stat remainder of C_MCstat is unevaluated** (row 20) |
| 16 | `bgsub_stat_rel` (0.0799 / 0.0954) | Template-MC variance of B_R′ — **distinct from `ibu_stat`** by the SA-3 w2 policy (no double count); carried bit-identical (already bgsub-denominated) | `carried_components_record.json`; check `carried_7comp_bitidentity_vs_budget15to19` (7840/7840 cells, 0 mismatches) | **COMPLIANT** | Diagonal magnitude only; no covariance form on the current basis |
| 17 | `bgsub_model_rel` (0.2016 / 0.2277) | Background-template model uncertainty; carried bit-identical | Same | **COMPLIANT** | Same as #16 |
| 18 | `bgsub_stage_rel` (0.2103 / 0.2453) | Subtraction-stage (template staging) uncertainty; carried bit-identical; **distinct from the detector legs** (double-count watch) | Same + budget double-count watch | **COMPLIANT** | Same as #16 |
| 19 | `bgsub_fakefrac_rel` (0.1828 / 0.1804) | Fake-fraction (f_i) uncertainty of the subtraction; carried bit-identical | Same | **COMPLIANT** | Same as #16 |
| 20 | **Response body/miss MC-stat (C_MCstat remainder)** — NOT a budget component | **Unevaluated.** `fakecol_mcstat_rel` covers only σ(f_i); the body/matched response MC statistics are unpropagated (G12(b) scope; response `sumw2` not propagated) | STATE.md P-METHOD OPEN items: gap OPEN with a **bounded estimate medians 1.4769%/1.5752%** (dossier basis; quadrature-closure totals 8.319%/8.554% stayed below the TUnfold cross-check totals); promotion record §"Carried open" | **GAP** | A full C_MCstat covariance term. Per failure-handling discipline this must be reported as **unevaluated, not zero** — the bounded ~1.5% estimate suggests it is NOT negligible next to `fakecol_mcstat_rel` (0.34%) |
| 21 | **Stat sidecar** (full bootstrap matrices) | 245×245 covariance + correlation per charge; 1000 toys; seeds 12345/22345; variant `bgsub_setrprime_conebase_reunfold_sub_full` = **current clean-release basis**; all 59,780 off-diagonals nonzero, symmetric; diagonal consistent with `ibu_stat_rel` to 1.11e-16 | `bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17/covariance/ibu_toy_{covariance,correlation}_{pos,neg}.json` | **COMPLIANT** (per charge) | `nominal` key is an EMPTY list in the sidecar; **no h+/h− cross-block** (independent seeds; supplement verdict NOT_MEASURABLE_FROM_RETAINED_TOYS); per-charge 245×245 only, no 490×490 |

**Tallies (21 rows): COMPLIANT 14 · DEVIATION 6 · GAP 1 · NOT-APPLICABLE 0.**

Notes on the tallies: the six DEVIATIONs are all *documented, deliberate* departures (supervisor sign-offs recorded), not defects of bookkeeping; the one GAP is a genuinely unevaluated required term. Several COMPLIANT rows carry publication-format gaps (signed columns, covariance forms) that are counted once in §8 rather than per row.

---

## 4. Statistical and MC-statistical covariance products

### 4.1 `ibu_stat` — full bootstrap matrix: COMPLIANT per charge, three caveats

The prescription's "ibu_stat as full bootstrap matrix" is **satisfied on the current basis**: the canonical release ships full 245×245 toy covariance and correlation matrices per charge (1000 toys, Poisson-only `w2 = data_sub′` per SA-3, so no template variance leaks into the stat term — and the budget's double-count watch confirms distinctness from `bgsub_stat_rel`). The budget column is the diagonal of this matrix (verified to 1.11e-16).

Caveats (all verified on disk):
1. The sidecar's `nominal` key is an **empty list** — the central vector must be taken from `ibu/cross_sections.csv`, not the sidecar.
2. **No h+/h− cross-block.** The two charges use independent toy seeds; the recorded supplement verdict is NOT_MEASURABLE_FROM_RETAINED_TOYS. Any 490×490 (or asymmetry-level) covariance must either regenerate toys with shared Poisson draws across charges or record an explicit zero-by-assumption (the stale v4 package used ZERO BY ASSUMPTION without measurement).
3. The sidecar stat band has a known **sidecar-represented under-coverage** in the worst bins (h+ bin 179 `0.013`, h− bin 235 `0.366` coverage figures carried in STATE) — a note-level coverage statement is required if per-bin intervals are published.

### 4.2 C_MCstat — PARTIAL: one diagonal slice done, the dominant part OPEN

- **Done:** `fakecol_mcstat_rel` (G12) propagates σ(f_i) through the IBU k=4 Jacobian — diagonal, fake column only, sumw2 pins verified (0.0 dev).
- **OPEN:** the **body/matched response MC statistics** are unpropagated (response `sumw2` not propagated; G12(b) scope). STATE carries this OPEN with a **bounded estimate: medians 1.4769% / 1.5752%** (computed on the pre-clean-release dossier basis). This bound is ~4× the published `fakecol_mcstat_rel` median, i.e. the missing part of C_MCstat is plausibly among the larger sub-percent systematics. It is correctly NOT assumed to be zero in the state records, but it is also NOT represented in the budget or any covariance product — a downstream consumer of the 19-component total would silently omit it.

### 4.3 Inventory of covariance products and their bases

| Product | Form | Basis | Status |
|---------|------|-------|--------|
| `bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17/covariance/` | Full 245×245 stat covariance + correlation, per charge | **Current clean-release basis** (`bgsub_setrprime_conebase_reunfold_sub_full`) | CURRENT — the only current-basis covariance product |
| `H1ChargedSidis_HighQ2/data/covariance/z005_supplement_modelreg_coherentprior_2026-10-07/` | 490×490 **signed** rank-1 + diagonal model/reg covariances (δ = 0.5·(IBU_DJ−IBU_RG), 0.5·(IBU_k3−IBU_k5)) | STALE coherent-prior basis (pre-S2′/cone/bgsub) | PROVISIONAL; definitions transferable, absolute vectors must be rebuilt on the clean basis |
| `.../total_uncertainty_z005_..._emfix_2026-10-10/covariance_cradstat/` | 490×490 rank-1 + diagonal `c_rad_stat` (and gen) | emfix budget; **relative forms level-invariant (valid)**, absolute forms on the stale coherent-prior raw nominal | Usable in relative form with basis flags |
| `sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/` | Full per-source rank-1+diagonal + TOTAL covariances (`cov_total_all_diagonalB.npy` was the recommended provisional total) | VERY STALE (pre-S2′, no cone, no bgsub, no C_rad) | Self-documented caveats: elecE shift ~2× frozen prescription; electron sources post-hoc without paired reruns; Tier-B rank-1 UNVALIDATED pending noise-floor test; response-MC-stat covariance ABSENT; h+/h− stat cross-block ZERO BY ASSUMPTION |
| **Current-basis C_total** | — | — | **DOES NOT EXIST** |

---

## 5. Detector legs — detailed audit (prescription points 2–5)

**Same-MC-events (point 2): satisfied, with an architecture note.**
- HFS legs (jes/rces/theta): Stage-2 tree-level reruns on **1:1-verified shifted twins of all 865 central MC files** — genuine same-events response rebuilds; shift→cone order signed off; custody manifests 14/14 runs, 28/28 recomputes dev 0.0.
- Electron legs: **post-hoc kinematic substitution** (E′→1.01·E′; θ→θ±1 mrad) on the SAME central MC trees via the shifted builder. Shared events: yes; but this is a substitution on frozen trees, not a paired detector-simulation rerun (the stale v4 README already flagged the electron sources as post-hoc lacking paired reruns). Accepted as the campaign design; recorded here because the prescription's "response rebuild" is only partially literal for these two sources.

**1-sigma shifts (point 3): one documented 2× deviation.**
- `elec_energy` ±1% vs the **frozen ±0.5%/wheel menu 1-sigma — a factor ~2**, documented in the stale v4 README and never revisited: no two-size linearity check exists, so the ~1.65/1.71% medians cannot be rescaled to the menu 1-sigma without an assumed linear response. HFS shifts (−1% JES, +1% RCES, +1 mrad) and elec_theta ±1 mrad are documented in the campaign plan; their menu-1-sigma alignment is not separately re-argued on record.
- The per-source shift sizes ARE recorded (DET_TAGS/sys-keys in the campaign plan and stage-3 custody) — the sizes are auditable; only the elecE normalization question is open.

**Up/down handling (point 4): DEVIATION, supervisor-signed.**
- HFS legs: **plus-leg only**; the minus legs were never produced; sign-symmetry is UNSTUDIED (legenv-install deferred follow-up (a); promotion record §Deferred items repeats it). A signed δ or a `(U_up−U_down)/2` cannot be formed from existing products.
- Electron legs: both legs exist, but the convention is the **leg-level envelope** — the whole per-bin vector of the leg with the larger robust median per charge — explicitly REJECTING per-bin max (v3, rejected as "noise-max") and replacing symmetrization. The recorded elecE asymmetry (minus median 1.4820% vs plus 1.2517%, +0.2303 pp, not reproduced by the DJ chain) is thereby absorbed rather than symmetrized; the S2-mechanism probe (why the minus shift deviates more) is deferred follow-up (b).
- The FULL ABS single-sided convention itself was explicitly confirmed by the supervisor (2026-10-14/16) — a deliberate, recorded choice, i.e. a DEVIATION from the prescription, not an oversight.

**Significance vs MC noise (point 5): partial.**
- No bootstrap significance test (e.g., toy-based z-tests of δ_k against the toy covariance) exists for any leg. Noise control is by convention: whole-leg-vector selection (robust median), rejection of per-bin-max v3, Gaussian smoothing for phi_h, and the >50% instability scan — which is done and PASSes: 5 clean >50% cells (`ibu_stat` 2, `elec_energy` 2, `elec_theta` 1), every one a carry-over of a known high-f_res′ tail bin already ≥40% in budget_15to19; per-bin noise spikes (e.g. bin 179 elec_energy) are documented in the sign-symmetry record. The `ibu_model` B1 floor has its own noise/mixed/real classification (z<2 noise, ≥3 real vs base) from the replica ensemble — the only source with a quantitative noise-classification.
- Net: the prescription's per-source significance test is implemented only for the model floor; for the detector legs the noise question is handled structurally, not statistically.

---

## 6. The unfolding's own systematic (prescription point 8)

- **Model:** `ibu_model_rel` = RG-vs-DJ coherent half-diff + B1 floor — this is the prescription's alternative-generator method, with the prior-dependence floor from a 100-replica parametric bootstrap of extended responses (replica-own body+miss priors; floor = replica SD; status FAIL on 4 supported bins failing the replica-mean working-point check, carried report-only). Magnitude COMPLIANT.
- **Iteration/regularization:** `ibu_reg_rel` = k5/k3 half-diff around nominal k=4. Magnitude COMPLIANT. (The method itself, IBU/BBB k=4 with coherent body+miss prior, is the user-declared FINAL nominal — P-METHOD CLOSED 2026-10-14.)
- **Prior dependence:** covered by the B1 floor (replica-prior bootstrap). **No truth-reweight-to-the-unfolded-result iteration** (re-unfold with the unfolded result as prior and quote the change) was found anywhere in the chain — the prescription's truth-reweight closure test is absent; the B1 replica ensemble is a related but distinct construction.
- **Signed form:** signed model/reg rank-1 covariances exist only on the STALE 2026-10-07 coherent-prior basis; on the current basis only |magnitudes| are published, though the signed half-diff inputs (e.g. `dj_v_coh_sub`) exist in custody and the signed columns are cheaply reconstructible.

---

## 7. Denominator discipline — COMPLIANT

This is the strongest part of the budget. The 2026-10-16 user decision ("clean release, single denominator") was executed as a full re-derivation, and the single-denominator claim is machine-checked:

- `single_denominator_statement`: all 19 components relative to ONE denominator (IBU k=4 on data_sub′; h+ 10.50881084 / h− 10.21750255 nb).
- Nominal recompute vs official IBU CSV: max rel dev **1.66e-15**.
- Carried 7 components: **7840/7840 cells** string-verbatim vs budget_15to19 (the bgsub_* components were already bgsub-denominated; lumi and c_rad are the recorded denominator-invariant exceptions with rulings).
- `c_rad` level-invariance re-verified: dev **0.0** in all three recompute forms.
- Quadrature closure (direct-19 vs chained, and on-disk re-parse): **1.11e-16**.
- Lumi NEVER rescaled (multiplicative, denominator-invariant — loop-2 ruling); re-denomination of the former mixed-denominator carry removed the ~×1.086 RAW-denominator bias found by critic loop 1.
- Double-count discipline: `ibu_stat` vs `bgsub_stat` distinct by SA-3 w2 policy; detector legs vs `bgsub_stage` distinct; exactly-once subtraction verified across cone + f_i + Set R′.

No mixed-denominator cell survives in the quoted budget. The only denominator-adjacent caveat is that relative components must be multiplied by the CURRENT nominal to form absolute δ vectors — safe precisely because of the single-denominator discipline.

---

## 8. Publication-format gaps (prescription points 1, 9, 10)

1. **No signed per-source columns δ_k on the current basis.** The budget publishes FULL ABS unsigned magnitudes only (`systematics_charged_sidis_highQ2.json` `values` are per-source per-bin magnitudes; `errors` the quadrature totals). Signed inputs exist in custody for: elec legs (both legs cached), model/reg half-diffs, tracking (mean−base), detint (model half-diff) — but NOT for the HFS legs (plus-only; the information does not exist). The output contract does flag `correlated_sources` (lumi, c_rad_gen/stat) and records `asymmetry_cancellation` expectations — good — but signed δ columns are a publication deliverable per the prescription and are absent.
2. **No current-basis C_total.** `C_total = C_stat + C_MCstat + Σ_k δ_kδ_kᵀ` cannot be assembled today: δ_k signed columns absent (1), C_MCstat body/miss unevaluated (§4.2), h+/h− cross-block unmeasured (§4.1). The only total covariances ever built are on the very stale 2026-09-23 basis (self-caveated). The near-singularity caveat (rank-1 lumi/c_rad/model terms) is therefore not yet statable for a current product — it must accompany any future C_total.
3. **Correlated vs uncorrelated separation.** The budget folds lumi (2.7% flat, fully correlated) into the per-bin quadrature. For magnitude quoting this is the recorded convention; for covariance consumers the correlated rank-1 terms must be peeled out — the contract JSON's `correlated_sources` flag supports this, but no peeled product exists.
4. **Sidecar hygiene:** empty `nominal` key; cross-block verdict NOT_MEASURABLE_FROM_RETAINED_TOYS (must be recorded as an explicit assumption if a 490×490 stat matrix is ever assembled from per-charge blocks).
5. **Stale covariance packages coexist with current magnitudes** (§4.3). All are labeled PROVISIONAL/stale in their READMEs, and STATE's superseded list governs, but none carries a README_SUPERSEDED pointer to a current successor — because no current successor exists. Until a current-basis covariance package is built, the only safe statement is: "current basis = per-bin magnitudes + per-charge stat matrices; all full covariance products are stale-basis."
6. **Live pointing trap (user-gated, recorded):** the canonical 7-panel plotter still reads the cone Born release and the SUPERSEDED budget column `total_with_cone15_rel` — repointing to `total_with_bgsub19_rel` is queued user-gated work, not executed.

---

## 9. Basis consistency — COMPLIANT (with the coexistence trap of §8.5)

- Budget and sidecar are both on the clean-release basis (sidecar variant string `bgsub_setrprime_conebase_reunfold_sub_full`; budget run_key/cluster pinned; promotion install 857/857 pre-existing files sha-identical, 0 modified).
- The budget's `package_class` string still says "NOT installed to central" — a staging-time relic explicitly superseded by the promotion record (R9: do not edit in place); auditors must cite the promotion record, not that string.
- All stale covariance products are date-stamped and self-caveated; STATE's superseded list names every prior basis. No product was found that silently mixes bases; the risk is consumers reaching for the stale total covariances because no current one exists.

---

## 10. Sources outside the 19-component budget (unevaluated / systematic-only — must not be assumed zero)

- **Response body/miss MC-stat** — the C_MCstat remainder (§4.2, the one GAP): bounded ~1.48/1.58% median; unevaluated as a component or covariance.
- **Trigger efficiency** — a PLACEHOLDER, carried open in every promotion's not-cured list; no data/MC comparison product exists; not in the budget.
- **DIFFVM exclusive rho / photoproduction** (register rows 18/19) — BLOCKED, never subtracted, systematic-only; measured elastic-rho fractions exist (z[0.7,1.0) row ~2.18%, worst cell ~4.53%) with an unquantified phi/omega/proton-dissociative caveat; no budget component.
- **e∩V0 overlap** — IGNORED by user decision (112.2093 tracks = 0.0029% of D_cone; systematic-only).
- **NucliaWeight r_min proxy; lowQ2 α≥0.2 rebuild; DjBkgd exact-exposure provenance (≤0.074% of B_R′)** — carried caveats, bounded, open.

These are correctly recorded as unevaluated/bounded rather than zero — consistent with the failure-handling discipline — but a covariance publication must list them explicitly as unpropagated terms.

---

## 11. Verdict summary and the single most important gap

- **21 audited rows** (19 budget components + stat sidecar + the C_MCstat remainder): **COMPLIANT 14, DEVIATION 6, GAP 1, NOT-APPLICABLE 0.**
- All six DEVIATIONs are documented, supervisor-signed convention choices (FULL ABS single-sided legs; leg-envelope for elec; conservative data-side tracking), each with its open follow-up (HFS sign-symmetry, S2 mechanism probe, full-chain tracking variant).
- **Single most important gap: no current-basis covariance construction exists.** The clean-release basis supports per-bin magnitudes and per-charge stat matrices only. The prescription's publishable set — signed δ_k columns and `C_total = C_stat + C_MCstat + Σ δ_kδ_kᵀ` — is available only on stale bases (2026-09-23 / 2026-10-07 / emfix-2026-10-10), and the one genuinely unevaluated required input, the **response body/miss MC-stat term (bounded ~1.5% median, ~4× the published fakecol slice)**, is still OPEN. Until these are closed, any covariance-consuming downstream use (fits, combinations, unfolding-level comparisons) would either mix bases or silently drop a ~1.5%-scale term.

**Validation-guidance cross-check (systematics-role guidance):** total/stat median 1.60/1.56 — systematics comparable to statistical, neither negligible nor dominant (PASS); dominant at median is `ibu_stat` — i.e., the systematic budget is statistics-limited at median, with lumi (2.7% flat, fully correlated) the largest single non-stat systematic and `elec_energy` (~1.65/1.71%) the largest structured one; the >50%-change investigation is done (5 cells, all known-tail carry-overs). For h+/h− ratio or asymmetry use: **lumi cancels exactly; c_rad nearly cancels (per-charge tables differ at the few-per-mille level); tracking partially (charge-dependent convention caveat OPEN); detector legs and ibu_stat do NOT cancel** (shared shifted responses but different data; independent toys).

---

## 12. Prioritized follow-ups

1. **P1 (cheap, custody-only):** rebuild **signed δ_k columns** on the clean basis from existing custody (elec both legs, model/reg half-diffs, tracking mean−base, detint model half-diff, fakecol Jacobian) and publish them alongside the magnitudes; peel lumi/c_rad into rank-1 correlated terms with the near-singularity caveat.
2. **P2 (analysis):** evaluate the **response body/miss MC-stat covariance** (e.g., split-sample or resampling bootstrap of the response through the IBU k=4 Jacobian, gen-bin quadrature à la G12) and either add it as component #20 or publish it as a bounded C_MCstat with its own record; the ~1.48/1.58% bound should be re-based to the clean release.
3. **P3 (production):** **HFS minus legs** (3 Stage-2 reruns on shifted twins) to close the sign-symmetry deferment and make signed HFS δ_k formable; plus an **elecE two-size linearity check** (±0.5% vs ±1%) to reconcile with the frozen menu 1-sigma.
4. **P4:** regenerate toys with **shared Poisson draws across charges** (or record an explicit cross-block assumption) and populate the sidecar `nominal` key; then assemble the current-basis **C_total** with the near-singularity caveat.
5. **P5 (hygiene):** add README_SUPERSEDED pointers on the three stale covariance packages; repoint the 7-panel plotter band to `total_with_bgsub19_rel` (already user-gated).
6. **P6 (recorded deferments to keep visible):** S2-mechanism probe (elecE minus-shift excess), full-chain tracking variant, trigger placeholder, truth-reweight closure test for the prior systematic.

---

## Appendix: key artifact paths

- Budget: `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/` (esp. `bgsub_setrprime_budget19_cleandenom.json`, `rederivation_records/`, `carried_components_record.json`, `component_cache/`, `systematics_charged_sidis_highQ2.json`, `validation_checks.json`)
- Stat sidecar: `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/covariance/ibu_toy_{covariance,correlation}_{pos,neg}.json`
- Shifted-legs campaign: `H1ChargedSidis_HighQ2/output_v6/studies/cone_s2prime_shiftedlegs_2026-10-14/` and `H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_shiftedlegs_2026-10-14/CAMPAIGN_PLAN_cone_s2prime_shiftedlegs_2026-10-14.md`
- B1 floors: `H1ChargedSidis_HighQ2/output_v6/studies/p1_replica_floors_coherentprior_2026-10-07/ensemble_b1/b1_ibu_model_floors.json`
- Stale covariance packages: `H1ChargedSidis_HighQ2/data/covariance/z005_supplement_modelreg_coherentprior_2026-10-07/`; `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/covariance_cradstat/`; `sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/`
- Governance: `sidis_highQ2/STATE.md` (2026-10-17 Clean-Release Promotion Override; P-METHOD OPEN items), `sidis_highQ2/docs/promotions/bgsub_cleanrelease_promotion_2026-10-17.md`

*Read-only audit; no central release, budget, or sidecar product was modified.*
