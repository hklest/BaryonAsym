# QA verdict record — data-vs-MC reco-level yield deficit, v6/z005 kinematic control plots (reco_kinematics_dist_2026-10-08)

**Date:** 2026-10-09 · **Agent:** h1-qa-specialist (read-only QA) · **Scope:** high-Q2 charged-particle SIDIS (Q² > 150 GeV²), central v6/z005, reco-level kinematic control-plot aggregates (`H1ChargedSidis_HighQ2/output_v6/studies/reco_kinematics_dist_2026-10-08/aggregate/`, schema v2, Condor 5434861, aggregate `overall_pass = True` for data and both MC scopes).

## VERDICT (final)

**PASS on the audit question — the deficit is a REAL model-level (generator physics) discrepancy, not a selection, weighting, or scaling bug.**

- The ~6% event-level and ~12–13% track-level MC deficit relative to data is the **same effect already on record** at the unfolded/grid level (2026-09-25 4-generator comparison: DJANGOH14 0.864×, RAPGAP31 0.880×) and at Born level (2026-10-08: 0.818×/0.844×), reproduced here at reco level through an **independent code path**.
- Code-path identity between data and MC is **confirmed byte-identical** (shared loader; §1).
- No single scale factor — luminosity, global weight, or scale constant — can produce the observation, because the event-level and track-level ratios **differ** by 8.8% (dj) / 6.4% (rg), and that difference is **scale-free** (§4.3).
- The deficit is **strongly shape-dependent** (per-bin data/MC ratios span 0.23 → 1.02, differently for each generator), which is the signature of generator modeling differences, not of a cut or normalization error (§4.5).
- Residual uncertainty: the **event-level** −6% is only ~1.5–2× the combined normalization uncertainty (±2.5% MC ⊕ ±2.7% data lumi) and data is raw (no background subtraction), so a normalization-plus-background component of up to a few % cannot be excluded at event level. The **track-level** deficit and the **scale-free multiplicity deficit** (−8.1% dj / −6.0% rg) are robust against all normalization questions.

Confidence: **high** for "not a selection/weighting/scaling bug"; **high** that the dominant effect is model-level; **medium** on the exact decomposition of the event-level −6% (model vs normalization vs residual data background).

---

## 0. Inputs audited (all existing products; no new batch runs)

| Artifact | Path |
|---|---|
| Study script | `H1ChargedSidis_HighQ2/scripts/reco_kinematics_dist_2026-10-08/reco_kinematics_dist.py` |
| Canonical selection | `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_data_vector_v4.py` |
| Data aggregate | `output_v6/studies/reco_kinematics_dist_2026-10-08/aggregate/reco_kinematics_dist_hQ2_v6data_2026-10-08.json` (+CSV) |
| DJANGOH14 aggregate | `.../reco_kinematics_dist_mc_djangoh14_L351000_2026-10-08.json` (+CSV) |
| RAPGAP31 aggregate | `.../reco_kinematics_dist_mc_rapgap31_L351000_2026-10-08.json` (+CSV) |
| Per-file records | `.../per_file/<period>/` (658), `.../per_file_mc/<mc>/<reqid>/` (501+364) — used for weight statistics and per-ReqID stability |
| Normalization authority | `sidis_highQ2/data/backgrounds/normalization_verification_recokin_mc_2026-10-08/verification_record.{md,json}` (verdict PASS) |
| Stage-1 tree builder | `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis.C` |
| Stage-2 v6 builder | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` |
| Prior comparison basis | `sidis_highQ2/STATE.md` lines 39–40 (4-generator raw-level 2026-09-25 and Born-level 2026-10-08 totals) |
| Older audit | `output_v6/studies/data_mc_reco_audit_20260729/` (single-file unit-area smoke, w1·w2 only) |

The task's quoted aggregate numbers were re-read from the products and reproduced exactly (536,807 / 3,902,480; dj 506,706.379 / 3,386,983.512; rg 504,560.333 / 3,448,218.399).

---

## 1. Code-path identity (question a) — CONFIRMED IDENTICAL

**Structure.** One shared loader `_open_and_run_selection(path, mc_mode)` (script lines 655–889) is called by both `process_file` (data, line 897) and `process_file_mc` (line 966). `mc_mode` affects only: (i) the branch read list (`+ weight1, weight2, NucliaWeight`, line 722–724), (ii) the per-event constancy check list (`+ weight1/weight2`, line 848–849), and (iii) the weight computation `w_ev_row = w1*w2`, `w_trk_row = w1*w2*nw` (lines 874–887). **All selection masks are computed before and independently of the mc_mode branch.**

**Mechanical diff.** The study's selection block (lines 742–813) was diffed against the canonical `build_data_vector_v4._select_and_bin` block (lines 332–405). Result: **identical line-for-line** modulo (a) three comment lines, (b) the canonical's counters bookkeeping line, (c) the `bdv.` prefix on cut constants (the study imports the canonical module and uses the same constant objects — no constant re-derived), and (d) naming: the canonical reuses `pass_reco` for `pass_reco & z_ok`; the study assigns it to `pass_reco_zok` — same value.

**Conventions verified identical in both paths:**
- float64 upcast `astype(float)` **before** every gate comparison for all kinematic branches (Pt, Eta, Px/Py/Pz, scatE*, BeamE/BeamProtP with 27.6/920 fallback, Q2es, Yes, Empz, PVz, Xes, xF_SIDIS_es, DcaPrime, Charge); integer flags (NHitDedx, RadLength, IsFromPrimary, IsCentralTrk, TrackType, IsMatchedElectron, IsMatchedV0Daughter) compared in native types.
- `evt_fid` = finite Q2es ∧ 150<Q2es<20000 ∧ finite Yes ∧ 0.1<Yes<0.7 ∧ finite Empz ∧ Empz>45; `evt_cleanup` = finite PVz ∧ |PVz|<35; `trk` = Pt>0.15 ∧ |η|<1.6 ∧ NHitDedx≥5 ∧ RadLength≥5 ∧ |Dca′/cosh η|<2 ∧ IsFromPrimary==1 ∧ (IsCentralTrk==1 ∨ TrackType∈{1,2}) ∧ IsMatchedElectron==0 ∧ IsMatchedV0Daughter==0 ∧ finite xF ∧ xF<0; `z_ok` = finite z_comp ∧ finite pTh_cm from `bdv._compute_pTh_cm_and_z` (photon-axis z = P·h/P·q, CM pT_h).
- Event identification: unique `(RunNumber, EventNumber)` per file, first-row value, fid = `evt_fid & evt_cleanup` at first row — identical logic both modes; the ≥1-track-candidate representability of ChargedTot is **common-mode** (same tree structure data and MC; DisTot not written in v6).
- The only deliberate difference from canonical — the (z,pT)/(Q2,x) grid cap — is **not applied to either data or MC** (symmetric; documented).

**Conclusion (a):** cuts are applied by byte-identical code to data and MC; only the weighting differs, exactly as claimed. There is no place in this study where a data/MC selection asymmetry could enter.

**Tree-implicit (stage-1) selection asymmetries — checked, all favor MC:**
`src_stage1/TreeBuilderHighQ2_chargedsidis.C`: CUT 1 (good-run list + detector status, lines 510–524) and CUT 2 (S67 L1 trigger bit, lines 526–530) are **data-only**; the reco-envelope cuts 3–11 (electron E/θ, Empz 30–75, vertex, Zimpact, φ-crack, fidvol, Q2/y eSigma, yₑ rejection) apply to both data and MC. The v6 stage-2 builder fills ChargedTot only for `passRecoEnv` events in **both** modes (envelope-OR keep; gen-only MC events get zero reco rows). Consequences:
- The data-only trigger/run requirements **remove data events only**, biasing MC/data **upward** — they *mask* part of the true MC deficit and **cannot create** it.
- `TrigWeightAC` is stored but not applied anywhere (open placeholder, central chain convention) — same direction of effect (unweighted data are trigger-suppressed; S67 LAr-electron efficiency at Eₑ>11 GeV, Q²>150 is high, so the mask is bounded at the few-% level).

---

## 2. Per-cut pass-fraction analysis (question b)

Counters available per file/aggregate: `n_events_unique` (events with ≥1 candidate row), `n_ev_fid` (fid+cleanup events), `n_entries` (candidate rows), `n_ps` (rows passing full track selection), `n_trk_sel` (× z_ok), `n_trk_zsan`, z-tail decomposition. Per-stage pass fractions:

| Stage (per input unit) | Data | DJANGOH14 | RAPGAP31 | Comment |
|---|---|---|---|---|
| Candidate rows → full trk selection (`n_ps/n_entries`) | 3,902,480/7,538,865 = **0.51763** | 76,964,449/149,089,165 = **0.51623** | 64,485,897/122,694,617 = **0.52558** | data sits **between** the two MCs (dj −0.14 pp, rg +0.80 pp) → quality cuts (NHitDedx, RadLength, DCA, class, e/V0 veto, xF) are effectively symmetric |
| `z_ok` losses among `pass_reco` | **0** (n_ps = n_trk_sel) | **0** | **0** | finite-z/finite-pT gate is a pass-through on all three samples — no float/edge pathology |
| z==0 exact, charge-zero | 0 / 0 | 0 / 0 | 0 / 0 | identical sentinel-free behavior |
| ≥1-candidate events → fid events (`n_ev_fid/n_events_unique`) | 536,807/845,143 = **0.63508** | 10,613,090/14,729,072 = **0.72055** (unweighted) | 8,614,339/11,784,302 = **0.73100** (unweighted) | **not** a selection asymmetry: the denominators differ in stream composition (data candidate stream contains non-fiducial-kinematics and non-DIS events; MC streams are generated DIS over the generation phase space, q2Gen>150 at stage-1). MC fractions are *higher* — the fid cut does not suppress MC |
| Tracks per fid event (unweighted) | **7.2725** | **7.2518** (0.997×) | **7.4859** (1.030×) | unweighted MC multiplicities **bracket** the data |
| Tracks per fid event (weighted, scale-free) | — | **6.6843** (0.919×) | **6.8341** (0.940×) | the w1·w2 event reweighting shifts the model prediction below data (§4.4) |

Cut-edge checks: fid edges are exact bin edges; the first populated bins above y=0.1, Q2=150, Empz=45 continue the neighboring-bin ratio trends smoothly in both MCs (no step, no spike, no hole); Pt/Eta under/overflow structurally empty in all samples; Empz[44,45) empty in all three. **No individual cut stage contributes a data/MC asymmetry.** The deficit is not localized at any cut boundary; it is distributed smoothly across all kinematic variables (§4.5).

Per-ReqID stability (no anomalous sample): fid fraction dj 0.716–0.723 / rg 0.727–0.733; row pass dj 0.512–0.521 / rg 0.522–0.530; mean w1·w2 dj 1.4221–1.4289 / rg 1.5190–1.5294; mean NW 0.9893–0.9898 everywhere. Data per-period multiplicities 7.21–7.43 (ep0304 7.430, em0405 7.284, em06 7.248, ep0607 7.207) — homogeneous.

---

## 3. Reco-level ratios vs the known unfolded-level ratios (question c)

| Quantity | DJANGOH14 | RAPGAP31 |
|---|---|---|
| Reco-level event ratio (scaled fid events / data) | 506,706.379/536,807 = **0.9439** | 504,560.333/536,807 = **0.9401** |
| Reco-level track ratio (scaled tracks / data) | 3,386,983.512/3,902,480 = **0.8679** | 3,448,218.399/3,902,480 = **0.8836** |
| Reco-level, z_sanity(0,1)-compatible subset | **0.8689** | **0.8846** |
| **Known grid/unfolded-level 490-bin totals (2026-09-25, central `prior_gen` × 1/L_MC — independent code path)** | 19.559339/22.645664 = **0.8637** | 19.924621/22.645664 = **0.8798** |
| Known Born-level 4-generator totals (2026-10-08, gen-level σ=Σw1w2/L_MC, third path) | 21.222921/25.964011 = **0.8175** | 21.903831/25.964011 = **0.8437** |

**Interpretation.** The reco-level track ratios agree with the independently-produced grid-level ratios to within ~0.5 pp; the residual is exactly the remaining selection differences (pT_h<8 grid cap removes relatively more data tail tracks; (Q2,x) grid containment; per-charge vs charge-combined). The Born-level comparison (no reco selection, no NucliaWeight, different trees/code) shows the **same sign and a larger magnitude**. Four independent measurement paths — this reco study, the central response prior (grid level), the Born-level C_rad-campaign tables, and the 2026-07-29 single-file shape audit (χ²/ndof ≈ 217–482 in unit-area mode with w1·w2 only) — all show the deficit. **The reco-level deficit is the known model-level effect, not a new selection bug.** (Reminder from STATE.md: RAPGAP31 is the central-response generator and therefore not independent of the unfolded data; DJANGOH14 carries the independent-model comparison weight.)

---

## 4. Weighting and normalization checks (question d)

### 4.1 Weight health
- `weight2 ≡ 1.0` verified on every file (both generators, aggregate `weight2_all_unity_all_files: true`).
- 0 non-finite event weights, 0 non-finite track weights, 0 non-finite NW among selected tracks; NW clipped-low (negative sentinels): dj 115, rg 98 out of 77.0M/64.5M selected tracks (negligible); clipped-high: 0.
- Stale `ChargedTot;1` cycle (223,440 entries) never read; default-cycle audit passed on all 865 MC files (497 dj + 364 rg multicycle files handled correctly).

### 4.2 Mean w1·w2 (quantified)
- DJANGOH14: mean w1·w2 per fid event = 15,139,830.82/10,613,090 = **1.42652** (range 1.0–148.67).
- RAPGAP31: 13,128,474.42/8,614,339 = **1.52403** (range 1.0–149.38).
- Far from 1 → the reweighting does real work (expected for CTEQ6L-generated steep spectra reweighted to the generator model); per-ReqID stable (§2). The L_MC construction (ΣW/SIGMT, oomclumi) is self-consistent with these weights, so Σw × L_data/L_MC is the correct model expectation. Without w1 reweighting the MC would be off by far more (dj unweighted scaled events = 355,204 → 0.66× data), i.e. the weighting machinery is essential and functioning; its correctness is independently anchored by the C_rad campaign σ tables and the oomclumi L_MC verification (record verdict PASS).

### 4.3 Luminosity sensitivity — single-scale hypothesis is IMPOSSIBLE
A luminosity (or any global scale/weight) error shifts the event and track ratios **equally**. Observed:

| | event ratio | track ratio | ratio-of-ratios (scale-free) |
|---|---|---|---|
| DJANGOH14 | 0.9439 | 0.8679 | **0.9195** |
| RAPGAP31 | 0.9399 | 0.8836 | **0.9400** |

To fix events, dj would need L_MC = 9,899,425 nb⁻¹; to fix tracks, L_MC = 9,102,154 nb⁻¹ — **mutually inconsistent by 8.76%**. RAPGAP31: 8,584,267 vs 8,069,792 — inconsistent by 6.38%. No single scale factor exists that removes the deficit. Additionally, both generators' L_MC are independent oomclumi measurements (865 chunks, G14 tolerance 2.5%, verified PASS); the event-level fix would require a −5.6%/−6.0% common error in both, and would still leave the track deficit untouched. Scales used are bit-exact vs the verification record (dj 0.03346843073584302, rg 0.03843251824641288); rejected values (356613.0, 297657.8) confirmed absent.

### 4.4 NucliaWeight effect size (quantified)
- Pooled mean NW over selected tracks: dj **0.989723**, rg **0.989486** (per-ReqID 0.9893–0.9898; NW ≤ 1 by construction, ~95.7% of tracks < 1).
- NW-off counterfactual (divide track sumw by mean NW): dj track ratio 0.8679 → **0.8769**; rg 0.8836 → **0.8930**. NW explains only **~0.9–1.0 pp of the 13.2/11.6 pp track deficit (~7–8%)**; the deficit remains ≥11% with NW off. The event level carries no NW by convention (per-track object), so the event/track split is unaffected.
- The 2026-07-29 audit used w1·w2 only (no NW) and already showed large shape disagreement — the deficit predates the NW convention.

### 4.5 Shape structure — the decisive evidence
Per-bin scaled-MC/data ratios (from the aggregate CSVs; representative bins):

| Variable, bin | dj/data | rg/data |
|---|---|---|
| Yes [0.10,0.11) | 0.939 | 0.940 |
| Yes [0.69,0.70) | 1.009 | 1.022 |
| Empz [45,46) | 0.879 | 0.921 |
| Empz [69,70) | 0.806 | 0.785 |
| Empz ≥ 70 (overflow) | 0.718 | 0.706 |
| Pt [0.15,0.164) | 0.677 | 0.740 |
| Pt [3.16,3.49) | 0.879 | 0.871 |
| Pt [55.3,61.1) | 0.490 | 0.392 |
| z [0,0.02) | 0.839 | 0.874 |
| z [0.98,1.0) | 0.471 | 0.426 |
| z ≥ 1 (overflow) | 0.231 | 0.227 |
| pTh_cm [2.06,2.28) | 0.963 | 0.828 |
| pTh_cm [7.44,8.21) | 0.724 | 0.568 |
| pTh_cm ≥ 10 (overflow) | 0.274 | 0.245 |

- The ratio is **not flat**: it spans 0.23 → 1.02 within one generator. A scale error is flat; a cut bug produces a step at a boundary. Neither pattern is present.
- The deficit **grows into the hard/fragmentation tails** (high Pt, high z, high pT_h, high Empz) and **inverts at high y** — a coherent physics pattern (parton-shower hardness, current-region high-z fragmentation, soft-track and nuclear-interaction modeling, Empz tail).
- **DJANGOH14 and RAPGAP31 differ from each other in shape** (pT_h 2–4 GeV: dj 0.94–0.96 vs rg 0.75–0.83; Empz tail: dj 0.81 vs rg 0.78) **despite identical selection code** — a code bug common to the MC path would impose a common distortion; generator-specific patterns are the signature of genuine model differences.
- Unweighted MC multiplicities bracket the data (dj 0.997×, rg 1.030×) while the weighted (model) multiplicities are 0.919×/0.940×: the w1·w2 kinematic reweighting shifts weight toward lower-multiplicity events — i.e. **the generator's own model, after reweighting, predicts fewer tracks per fiducial event than observed**. This is a model statement, luminosity-independent by construction.
- The Q2 high-edge bin [18.65k,20.5k) shows 1.13/1.14 on 15 data events — a statistics fluctuation, not signal.

---

## 5. Final verdict, confidence, residual uncertainties (question e)

**REAL model-level discrepancy.** The MC deficit is not a selection, weighting, or scaling artifact:

1. Selection code is byte-identical between data and MC (one shared loader; §1); track-quality pass fractions agree to <1.6 pp with data between the two generators; z_ok, sentinel, charge-zero, and cut-edge behavior identical (§2).
2. No single scale can explain events (−5.6%/−6.0%) and tracks (−13.2%/−11.6%) simultaneously — 8.8%/6.4% mutually inconsistent required luminosities; the multiplicity deficit is scale-free (§4.3).
3. The reco-level ratios reproduce the independently-produced grid-level ratios (dj 0.868 vs 0.864; rg 0.884 vs 0.880) to ~0.5 pp, and the Born-level path shows the same deficit with a third method (§3).
4. The per-bin ratio structure is strongly shape-dependent and generator-specific (§4.5).
5. NW contributes only ~1 pp; the deficit persists at ≥11% with NW off, and existed in the w1·w2-only 2026-07-29 audit (§4.4).
6. All tree-implicit asymmetries (data-only S67 trigger, run list, detector status) act to **suppress data**, i.e. they mask part of the deficit rather than create it (§1).

**Residual uncertainties (state with the result):**
- Event-level −6%: comparable to only ~1.5–2× the combined normalization uncertainty (±2.5% MC L_MC ⊕ ±2.7% data lumi_rel); a normalization component of a few % cannot be excluded at event level. Do not over-interpret the event-level number as pure model deficit.
- Data are raw: no background subtraction at reco level. Residual non-DIS contamination (photoproduction/fake electrons at the fid level) inflates data and contributes to the apparent deficit; not quantified for this selection (the alpha0p2/bgsub chain addresses backgrounds under a different fiducial variant).
- Trigger: TrigWeightAC not applied (open placeholder); the data-only S67 requirement masks an unknown few-% fraction of the true model deficit at event level.
- RAPGAP31 is the central-response generator — not independent of the unfolded data; DJANGOH14 is the independent-model comparison. The two generators agreeing on the deficit (while differing in shape) is the substantive model-comparison statement.
- The event-level yields are "fiducial events with ≥1 charged track candidate" (DisTot not written in v6) — common-mode, but do not label them "per fiducial DIS event".

## 6. Recommended figure-README caveats and follow-ups (question f)

**Wording for the README of the data-vs-MC control-plot figure:**

> Curves: DJANGOH14 / RAPGAP31 reco-level RAD-signal MC, raw sums of w1·w2 (event level) / w1·w2·NucliaWeight (track level; NW non-finite→1, clip [0,10]) scaled to L = 351000.0 nb⁻¹ with pooled per-generator scales 351000/L_MC (L_MC = 10,487,495.0 / 9,132,891.0 nb⁻¹, oomclumi-verified). Data: unweighted raw counts. RAW/UNCORRECTED reco level — no background subtraction, no acceptance correction, no C_rad/Born, no unfolding; trigger weights not applied. Event-level curves show fiducial events with ≥1 charged track candidate. MC underpredicts data by ≈6% at event level and ≈12–13% at track level; per QA record `docs/qa/DATA_MC_RECO_YIELD_DISCREPANCY_QA_2026-10-09.md` this is the known model-level deficit (same as the 4-generator comparison: 0.86×/0.88× at grid level, 0.82×/0.84× at Born level), **not** a selection or normalization bug; the deficit is shape-dependent (largest at high pT_h, high z, high Pt, high Empz). Normalization uncertainty ±2.5% (MC) ⊕ ±2.7% (data lumi) does not cover the track-level deficit; **do not tune the MC normalization to the data**. RAPGAP31 is the central-response generator (not an independent prediction); show generators separately, never merged. PROVISIONAL INTERNAL DIAGNOSTIC — not physics-approved, not for publication.

**Follow-ups (none block this verdict; no re-run of the study needed):**
1. Quantify the residual-background component of the event-level deficit using the existing background machinery (alpha0p2/bgsub chain), noting the fiducial-variant difference, if the event-level number is to be quoted.
2. Bound the S67 trigger-efficiency mask (TrigWeightAC placeholder is open) if the event-level deficit magnitude matters downstream.
3. Keep per-period MC-vs-data scaling out of figures (pooled per-generator scale only; per-period L_data scheme is superseded).
4. When the note quotes MC comparisons, use the Born-level 4-generator package (2026-10-08) as the comparison basis per STATE.md; these reco-level control plots are consistent with it and stay diagnostic.

## 7. Evidence index (key line references)

- Shared loader + mc_mode scope: `reco_kinematics_dist.py` 655–889 (weights 874–887; data call 897; MC call 966).
- Verbatim selection block: `reco_kinematics_dist.py` 742–813 vs `build_data_vector_v4.py` 332–405 (mechanically diffed; see §1); cut constants `build_data_vector_v4.py` 168–181.
- Data-only trigger/run cuts: `src_stage1/TreeBuilderHighQ2_chargedsidis.C` 510–530; envelope-OR stage-2 gating: `src/ChargedSidisTreeBuilderHighQ2_v6.C` 1–31.
- Weight conventions evidence: verification record §2 (build_migration_th2_per_charge_v4 690–698; prior_gen = w1·w2), §3 (L_MC provenance).
- Aggregate totals/counters/weight checks: the three aggregate JSONs (`totals`, `self_check`, `normalization` blocks; `aggregate_overall_pass: True`).
- Per-file weight statistics: 865 `per_file_mc/*/*/*.recokin.json` records (mean w1·w2, mean/min/max NW, per-ReqID stability).
- 4-generator totals: `sidis_highQ2/STATE.md` lines 39–40; `plots/mc_predictions/data_vs_mc_z005_4generator_2026-09-25/manifest_4generator.json` (data 22.645664218179906 nb).

**Blockers:** none. All verdict items are supported by existing products; no re-run or new batch evidence is required.
