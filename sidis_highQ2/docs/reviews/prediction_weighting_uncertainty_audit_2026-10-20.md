# Prediction weighting & per-bin uncertainty audit — 5-prediction high-Q² charged-SIDIS comparison

**Date:** 2026-10-20
**Scope:** Read-only audit (NAF login node, single-file checks < 5 min each; no Condor; no existing file edited).
**Question 1:** Does the SHERPA3 unit-count weight-discarding defect class affect the other predictions in the canonical 5-prediction comparison (`data_vs_mc_z005_5generator_born_releasebin_sherpa3sumw_2026-10-20/`)?
**Question 2:** Which per-bin uncertainties exist, where, are they statistically valid, and can the plotter render them on ratio panels?
**Companion document:** `sherpa_weighting_shapebias_study_2026-10-20.md` (SHERPA3 defect proof and sumW rebuild spec).

---

## 1. Executive verdicts

| Prediction | Weighting/aggregation verdict | Evidence (one line) |
|---|---|---|
| PYTHIA8 10M ep+em | **CLEAN** — unit-count is exact; merge correct | `meanW = 1.000000` exactly on `_EVTCOUNT`, cutflow, DIS/Q² and all Z005 bins (both legs); merge arithmetic 0/980 mismatches |
| SHERPA3 10M (reference case) | **DEFECT → FIXED** | Unit-count CSVs discarded MC@NLO weights; canonical package now uses the sumW rebuild (25.206769 nb; ΣnumEntries = 59,210,379 matches dedup anchor) |
| RAPGAP31 NORAD | **CLEAN** — weights carried | `sigma_nonrad = sum(weight1·weight2)/L_MC` from the weighted releasebin campaign; no unit counting, no clamping |
| DJANGOH14 NORAD | **CLEAN** — weights carried | same pipeline; 490/490 finite, 0 alignment mismatches |
| MAPTMD24 | **CLEAN** — no generator weights involved | density × published-bin volume with `dx_eff = 1.0 − x_min` cap; band ordering enforced; 230/230 provider rows matched |
| Data side (NoZ photon-Born sidecar) | **CLEAN** — no weighting-class operation | `sigma_photonBorn = C_gamma_born × sigma_raw(S2′ IBU)`; per-bin multiplicative correction on unfolded yields; strict edge-validated matching |

No second instance of the SHERPA3 defect class exists in the canonical comparison. The defect was unique to the SHERPA3 CSV producers that read the `numEntries` column of weighted MC@NLO YODA histograms.

---

## 2. Part 1 — weighting/aggregation audit per prediction

### 2a. PYTHIA8 10M ep+em unit-count chain — CLEAN

**Unit-weight premise: CONFIRMED empirically.** Inspected `ep_seed830001_n5000.yoda` and `em_seed840001_n5000.yoda` (`pythia8_rivet_z005_xfpos_20260702/outputs/`):

- `/RAW/_EVTCOUNT`: sumW = 5000 = numEntries, meanW = 1.000000 (both legs).
- All 7 cells × 35 bins of `Z005/hplus/.../yield_zpT_flat`: sumW = numEntries to machine precision (max |sumW − n| = 0).
- Cutflow and `DIS/Q2`: sumW = numEntries everywhere.
- Campaign-local `H1_2026_I_CHARGED_SIDIS_HIGHQ2.cc` is byte-identical to the smoke routine, so Rivet-3 auto-weighting applies — and the per-event weight is exactly 1.0.

For strictly unweighted events, `numEntries`-based aggregation is *exact*, and `sqrt(N)` statistics are valid. The SHERPA3 hazard cannot occur here.

**ep+em luminosity-weighted merge: CONSISTENT.** The producer (`aggregate_pythia8_yoda_unitcount_10M.py` + `run_pythia8_10M_aggregation.py`) aggregates each leg separately (1000 seeds × 5000 events = 5M per leg; per-leg `sigma_bin = sigma_gen(leg) · n_bin / N_gen(leg)`, with `sigma_gen(leg)` the N-weighted mean of per-seed provenance values), then merges:

- `w_ep = 194.012/356.613 = 0.544041`, `w_em = 162.601/356.613 = 0.455959` (sum = 1 exactly);
- `sigma_epem = w_ep·sigma_ep + w_em·sigma_em` — weights applied **once**, to cross sections (correct mixed-beam composition), no event-count renormalization;
- `stat_epem = sqrt((w_ep·stat_ep)² + (w_em·stat_em)²)` — correct quadrature for independent legs.

Verified numerically against the released 490-row CSVs: **0/980 mismatches** (sigma and stat, all bins); totals reproduce exactly: ep = 19.186345 nb, em = 18.997978 nb, epem = **19.100458 nb** (published value).

**Input actually used by the canonical package:** `prediction_pythia8_epem_unitcount_xfpos_10M_normfix_clean_plotter.csv` (2026-07-04 normfix-clean package). Its provenance JSON records derivation from the producer epem CSV with **identical `sum_sigma_nb = 19.10045770254032`** (14-digit equality) — the normfix only appended plotter columns (`generator`, `tag`, `valid_prediction`, `notes`); no value changed. Per-leg component columns (`stat_bin_nb_ep/em`, `lumi_weight_ep/em`) are carried through unchanged.

*Note (not a hazard):* the luminosity fractions (54.4%/45.6%) encode the generator-campaign ep/em composition; the data side is normalized by total L only (351.0 pb in the canonical invocation). Any residual ep/em composition mismatch would be a small level effect, not a shape bias.

### 2b. RAPGAP31 / DJANGOH14 NORAD legs — CLEAN (weights carried)

**Source campaign is genuinely weighted.** The releasebin C_rad workers (`crad_v3_worker_releasebin.py`) compute per-entry `w = weight1 × weight2` (finite-checked) and accumulate `sumw`, `sumw2`, and `n` per bin. The finalizer (`finalize_releasebin.py`) combines beams by luminosity fraction (DJANGOH14: `BEAM = {ep: 0.5140930272, em: 0.4859069728}`) as `sigma = Σ_b BEAM_b·sumw_b/L_b` with variance `Σ_b BEAM_b²·sumw2_b/L_b²` — i.e. `sigma = sum(w1·w2)/L_MC` exactly as the 2026-09-25 handoff states, with proper weighted errors.

**Package builder mapping (`build_nonrad_truth_predictions_releasebin_2026-10-13.py`):**

- `sigma_bin_nb = sigma_nonrad(charge, global_bin_index)` — taken **directly** from the weighted table; no unit counting, no renormalization by event counts.
- `stat_bin_nb = sigma_nonrad/sqrt(nonrad_n)` when `nonrad_n > 0`, else 0.0 — honestly labeled in the CSV notes and validation JSON as *"approximate Poisson statistical uncertainty from unweighted NONRAD counts"* (see §3 for validity).
- **No clamping:** no `max(0, ·)` anywhere; `valid_prediction` is finiteness-only. All 490/490 bins per generator have finite `sigma_nonrad` and positive `nonrad_n` (no zero-count bins).
- Alignment: `(charge, global_bin_index)` **plus** an independent 8-edge comparison per row (tolerance 1e-9) **plus** an independent edge-tuple identification; 3,920 edge comparisons per generator, **0 mismatches**; ABORT on any mismatch.
- Totals reproduce the supervisor-pinned constants: RAPGAP31 = 21.861163 nb, DJANGOH14 = 21.222921 nb; integrated nonrad/rad ratios match to 1e-6.

**Verdict: no unit-count hazard, no clamping hazard.** The only caveat is that the published per-bin stat is the count-based approximation, not the exact weighted error (§3.2).

### 2c. MAPTMD24 — CLEAN (pure theory input; conversion verified)

Provider CSV (`data/MAPTMD/MAPTMD24_H1.csv`, 230 rows): columns `charge, Q2/x/z/PhT edges, xsec_low[nb/GeV3], xsec_high[nb/GeV3]` — densities in nb/GeV³; per the theorist's statement (2026-10-13) the two columns are the lower/upper edges of a **68% band**.

Builder (`build_maptmd_predictions_2026-10-13.py`) verified line by line:

- `volume = (Q2_max−Q2_min) · dx_eff · (z_max−z_min) · (pT_max−pT_min)` with `dx_eff = 1.0 − x_min` **only** for the capped highest-x cell (release `x_max = 1.5` vs provider `x_max = 1.0` — same physical bin), literal `x_max − x_min` everywhere else. Units: GeV² × GeV = GeV³ ✓ matches the density unit. ABORT on non-positive volume.
- `sigma_bin_nb = midpoint × volume` with `midpoint = (xsec_low + xsec_high)/2`; `band_low/high_bin_nb = xsec_low/high × volume` — the same volume multiplies value and band, so the band is the 68% envelope in bin-integrated units.
- Band ordering enforced (`low ≤ mid ≤ high`, ABORT on violation); matching by edges with the documented cap rule; **230/230 matched, 0 unmatched, 0 duplicate targets**; cap rule exercised on 46 rows (23+23), all rows of the Q² 1500–20000, x 0.021 cell.
- `stat_bin_nb = 0.0` **by construction** (the 68% band *is* the uncertainty; stated in CSV notes and validation JSON).
- **No normalization hazard:** no generator `sigma_gen`, no luminosity, no event counts enter anywhere; the prediction is absolute Born-level theory × published bin volume.
- Coverage caveat (correctly propagated): 230/490 bins covered (no z < 0.05 row; sparse pT at higher z); integrated MAPTMD totals are not comparable to full-coverage totals.

### 2d. Data side — NoZ photon-Born sidecar — CLEAN (no weighting-class operation)

`build_data_side_noz_photonborn_2026-10-13.py` constructs `sigma_photonBorn(bin) = C_gamma_born(bin) × sigma_raw(bin)`:

- `sigma_raw` = the S2′ detint-fake RAW IBU unfolded cross sections (`bbb_ibu_detints2prime_..._2026-10-09/ibu/cross_sections.json`), taken read-only;
- `C_gamma_born` = per-bin factor from the DJANGOH14 NoZ photon-Born table (`cgammaborn_empz_gt45_nominal_490rows.csv`);
- matching by `(charge, global bin_index)` + independent 8-edge check with the same x_max cap rule; ABORTS on mismatch, duplicate, non-finite value, or > 1e-4 deviation from supervisor pre-computed totals;
- sidecar carries `lumi_nb = 351000.0`, `c_rad_applied = false`, `photon_born_c_gamma_applied = true`, and the full RAW uncertainty schema unchanged (§3.5).

This is a per-bin multiplicative correction applied to already-unfolded yields — there is no event-weight aggregation step in which weights could be discarded. The unfolding itself was **not** re-derived (per audit instructions). The sidecar's own caveat (level inconsistency: generator curves are full-Born/NORAD, only MAPTMD24 is naturally photon-only; DJANGOH-only correction) is a physics-level caveat, not a weighting defect.

---

## 3. Part 2 — per-bin uncertainty inventory

### 3.1 Summary table

| Side | Per-bin unc. exists? | Where (file → column) | Statistically valid? | Carried in ratio table? | Rendered today? |
|---|---|---|---|---|---|
| PYTHIA8 | Yes (MC stat) | producer + normfix-clean plotter CSV → `stat_bin_nb` (plus `stat_bin_nb_ep/em`, `lumi_weight_ep/em`) | **Valid** — `sigma_gen·sqrt(N)/N` is exact for the confirmed unit-weight sample | `mc_stat_unc_nb` (490/490 nonzero) | No |
| SHERPA3 (old CSVs: correctednorm, normfix_clean, dedup) | Yes but **INVALID** | old CSVs → `stat_bin_nb` = `sigma_gen·sqrt(N)/N` | **Invalid** for weighted MC@NLO events — ignores the ×630 weight spread; underestimates stat by ~2.4–2.8× (S0–S2) and ~4.1× (S3) on average | superseded packages' tables only | No |
| SHERPA3 (new sumW rebuild — canonical) | Yes | `prediction_sherpa3_sumw_xfpos_10M_sumw_2026-10-20_bins.csv` → `sumW_total, sumW2_total, numEntries_total, sigma_bin_nb, stat_bin_nb` (+ dedup cross-check cols); plotter CSV → `sigma_bin_nb, stat_bin_nb` | **Valid** — `sigma = S/T_tot/1000`, `stat = sqrt(S2)/T_tot/1000` (self-normalizing; sumw2-based) | `mc_stat_unc_nb` (490/490, sumw2-based — spot-verified) | No |
| RAPGAP31 NORAD | Yes (MC stat) | `prediction_rapgap31_nonrad_truth_v6z005_releasebin_plotter.csv` → `stat_bin_nb` | **Approximately valid** — `sigma_nonrad/sqrt(nonrad_n)`, labeled "approximate Poisson from unweighted counts"; exact weighted per-bin error not published in the 490-row table (only the C_rad **ratio** error `relative_stat_unc` is) | `mc_stat_unc_nb` (490/490 nonzero) | No |
| DJANGOH14 NORAD | Yes (MC stat) | `prediction_djangoh14_nonrad_truth_v6z005_releasebin_plotter.csv` → `stat_bin_nb` | same as RAPGAP31 | `mc_stat_unc_nb` (490/490 nonzero) | No |
| MAPTMD24 | Yes (theory 68% band; stat = 0 by construction) | `prediction_maptmd24_truth_v6z005_releasebin_plotter.csv` → `band_low_bin_nb, band_high_bin_nb` (`stat_bin_nb` = 0) | **Valid as a 68% band** per theorist statement; central = band midpoint | `mc_band_low_nb / mc_band_high_nb` (490/490; 0 for the 260 uncovered bins) | **Yes — absolute panels only** (fill_between), not on ratio panels |
| Data side | Yes (full set) | sidecar `data_side_noz_photonborn/ibu/cross_sections.json` → `stat_unc_nb` (toy), `analytic_stat_unc_nb`, `model_unc_nb`, `reg_unc_nb`, `toy_stat_unc_nb`, `total_unc_nb` (diag), `total_unc_rel`; toy covariance/correlation JSONs referenced per charge | **Valid** — IBU unfolding uncertainties (toy stat, response-model, regularization) | `data_stat_unc_nb`, `data_total_unc_nb`, `data_ratio_stat_frac_unc`, `data_ratio_total_frac_unc` | **Yes** — error bars on absolute panels (total) and ratio panels (total/data as fractional bars at y=1) |

### 3.2 SHERPA3 old vs new stat — quantified

The bins CSV allows the exact comparison `rel_stat_sumw / rel_stat_unitcount = sqrt(N·S2)/S` per bin. Mean underestimate factor of the old count-based stat (old/new):

| Cell | mean factor | bins |
|---|---|---|
| S0_x0 | 2.79× | 70 |
| S0_x1 | 2.38× | 70 |
| S1_x0 | 2.60× | 70 |
| S1_x1 | 2.37× | 70 |
| S2_x0 | 2.67× | 70 |
| S2_x1 | 2.80× | 70 |
| S3_x0 | 4.10× | 70 |

Q²-dependent, worst at S3 — the same Q²-dependence as the weight spread. The old stat values survive only in superseded CSVs/packages; the canonical 2026-10-20 package carries the valid sumw2-based stat.

**Plotter CSV vs bins CSV (confirmed):** the plotter CSV (`..._plotter.csv`) carries **exactly the same** `sigma_bin_nb` and `stat_bin_nb` values as the bins CSV — verified by exact float equality over all 490 rows (0/490 mismatches for both columns; e.g. bin 0: sigma = 2.108751184873771 nb, stat = 0.004038761427961454 nb). The bins CSV additionally carries the raw per-bin sums (`sumW_total`, `sumW2_total`, `numEntries_total`) and the dedup cross-check columns (`dedup_numEntries`, `dedup_sigma_bin_nb`, `ratio_sigma_vs_dedup`, `a_sumw_over_unit_bin`); the plotter CSV instead carries the plotter schema (`charge_sign`, `sigma_gen_nb` = 4.47433519882836 derived, `N_generated` = 10⁷, `source_hist_path`, etc.). Chain anchors: Σ sigma = **25.206769 nb**, Σ numEntries = **59,210,379** (= dedup released-CSV anchor).

### 3.3 NORAD stat approximation — magnitude

The exact weighted per-bin relative error of `sigma_nonrad` is not a published column (the campaign accumulates `sumw2` and publishes only the C_rad **ratio** error `relative_stat_unc`). At bin 0 the campaign's weighted ratio error is 2.505e-4 vs the naive `1/sqrt(n) ⊕ 1/sqrt(r)` = 2.366e-4 — a ~6% level difference, i.e. the H1OO weights are near-uniform and the count-based approximation is good at the few-percent level. It is *approximately* valid, not invalid — and it is labeled as approximate in three places (CSV notes, validation JSON, builder docstring).

### 3.4 MAPTMD24 band

`band_low/high_bin_nb = xsec_low/high × volume` — the 68% theory envelope in the same bin-integrated units as the central value; band ordering enforced at build time. Rendered as a filled band on absolute panels (see §4). Uncovered bins (260/490) carry 0.0 with `valid_prediction`/coverage bookkeeping in the validation JSON.

### 3.5 Data side

The sidecar preserves the full RAW unfolding uncertainty schema per bin: toy stat (identical to `stat_unc_nb`), analytic stat, model (response-variation), regularization, and the diagonal total (`total_unc_nb` = quadrature of stat+model+reg, verified at bin 0), plus `total_unc_rel` and references to toy covariance/correlation JSONs. The plotter consumes `stat_unc_nb` and `total_unc_nb` and writes both fractional forms to the ratio table. No weighting-class issue in the construction path (§2d).

---

## 4. Plotter uncertainty support

Two versions exist and differ materially.

### 4.1 Canonical script — `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py` (612 lines)

**Exact prediction-CSV columns accepted today:**

- Required (validated, missing → exception): `generator`, `tag`, `charge`, `bin_index`, `global_bin_index`, `sigma_bin_nb`, `stat_bin_nb`, `valid_prediction`.
- Also extracted at merge (must be present in practice): `notes`.
- Row count enforced: 490 (245 pos + 245 neg); one-to-one merge on `(charge, bin_index, global_bin_index)`.

**Data-side JSON columns consumed:** `charge, bin_index, global_bin_index, q2_min/max, x_min/max, z_min/max, pT_min/max, cross_section_nb, stat_unc_nb, total_unc_nb, supported`.

**Flags:** `--release-dir`, `--prediction-csv` (repeatable), `--out-dir`, `--lumi-pb`, `--differentiate-pth`, `--differentiate-4d`, and `--use-covariance` — the last is **accepted for CLI compatibility but explicitly "currently not implemented."**

**What it does with MC stat:** `stat_bin_nb` is renamed `mc_stat_unc_nb`, divided by the same density scale as the value (`bin_volume_4d` or `pTh_width`) in differentiated modes, and **written to the ratio table** — but **never rendered**: there is no `fill_between`, no MC `yerr` anywhere. Only data error bars are drawn (absolute panels: `data_total_unc_nb`; ratio panels: data reference at y=1 with `data_ratio_total_frac_unc` bars; global-bin ratio figure: data bars only).

**Verdict:** rendering MC uncertainty bands on ratio panels with the canonical script requires a **code change** — there is no existing flag, and no band columns exist in its schema.

### 4.2 Package-local extended plotter — `.../data_vs_mc_z005_5generator_born_releasebin_2026-10-13/compare_data_vs_mc_z005_4generator_born.py` (used for the canonical sherpa3sumw package, per `invocation_2026-10-20.sh`)

This version **already implements the band schema extension** (anticipated by the MAPTMD builder's validation note "the plotter reader will be extended separately by the plotting specialist"):

- Optional prediction-CSV columns `band_low_bin_nb`, `band_high_bin_nb` are auto-detected, renamed `mc_band_low_nb` / `mc_band_high_nb`, filled with NaN when absent, density-scaled identically to the value, and written to the ratio table (490/490 populated for MAPTMD24; the four generator CSVs have no band columns → NaN).
- `MODEL_STYLES["MAPTMD24"]` carries `"band": True`; band generators get a `Patch` legend handle.
- Bands are rendered with `ax.fill_between(pT_min, mc_band_low_nb, mc_band_high_nb, step="post", alpha=0.22)` at exactly **two sites — both on ABSOLUTE panels**: the standalone/absolute pT panels and the top (`ax`) sub-panel of the 7-panel absolute+ratio figures.
- Additional differences vs canonical: `--standalone-only` flag (MAPTMD24 standalone renders), log-scale ratio axes with fixed ticks (0.5–20), 5-entry legend.
- **Ratio panels still draw MC as step lines only** — neither the per-cell ratio sub-panels (`rx`) nor the global-bin ratio figure render any MC uncertainty. `mc_stat_unc_nb` remains unrendered for all generators.

### 4.3 What rendering MC bands on ratio panels would take

1. **Stat bands for PYTHIA8 / SHERPA3 / RAPGAP31 / DJANGOH14:** *no schema change needed* — valid per-bin stat is already required input (`stat_bin_nb`) and already carried in the ratio table as `mc_stat_unc_nb` (the canonical package's SHERPA3 entry is now the valid sumw2-based value). Needed: a small code change adding, on each ratio panel, a ±1σ band `ratio ± mc_stat_unc_nb/data_cross_section_nb` (or equivalent error bars) per generator.
2. **Theory band for MAPTMD24 on ratio panels:** `mc_band_low_nb` / `mc_band_high_nb` are already carried; needed: `rx.fill_between(..., mc_band_low_nb/data_cross_section_nb, mc_band_high_nb/data_cross_section_nb, ...)` — the absolute-panel mechanism already exists to copy.
3. **Canonical script:** none of the band machinery exists there; either port the package-local extension + ratio rendering, or declare the package-local version the new canonical and re-sync `MC/scripts/`.
4. **Semantics note:** data-side ratio bars use *total* (stat+model+reg) uncertainty; MC bands would be *stat-only* for the generators (MAPTMD: 68% theory envelope). This asymmetry should be stated on the plots (the manifest's existing `panel_ratio_data_uncertainty_note` is the natural place).

---

## 5. Recommended route to get uncertainties onto ratio plots

1. Keep the canonical inputs as-is: all five predictions already carry valid per-bin uncertainties in their plotter CSVs (PYTHIA8 sqrt(N) — exact for unit weights; SHERPA3 sumw2-based from the sumW rebuild; NORAD approximate Poisson — labeled; MAPTMD24 68% band; data side full unfolding set).
2. Extend the **package-local** plotter (it already has the band-column mechanism): add ratio-panel band rendering per §4.3 items 1–2; for the four generators derive the band from the already-carried `mc_stat_unc_nb` (no CSV edits needed); for MAPTMD24 divide the existing band columns by `data_cross_section_nb`.
3. Port the extended plotter back to `H1ChargedSidis_HighQ2/MC/scripts/` (or re-declare canonical) so the capability is not package-local only.
4. Do **not** retrofit the old invalid SHERPA3 sqrt(N) stat from superseded CSVs; the 2026-10-20 package is the only SHERPA3 leg whose `mc_stat_unc_nb` is valid.
5. Optional accuracy upgrade (low priority): replace the NORAD `sigma/sqrt(nonrad_n)` stat with the exact weighted per-bin error by publishing `sqrt(sumw2)/L` from the C_rad campaign partials — the approximation is currently good at the few-percent level.

---

## 6. Chain-integrity anchors recorded during this audit

- PYTHIA8 epem total: 19.10045770254032 nb (producer CSV = normfix-clean CSV, 14-digit identity; merge arithmetic 0/980 mismatches).
- SHERPA3 sumW rebuild: Σ sigma = 25.206769 nb; Σ numEntries = 59,210,379 (= dedup released-CSV anchor); plotter CSV ≡ bins CSV on sigma/stat (0/490 + 0/490 exact-equality mismatches); derived sigma_gen = 4.47433519882836 nb.
- NORAD totals: RAPGAP31 = 21.861163 nb, DJANGOH14 = 21.222921 nb (builder validation, match supervisor constants to 1e-4/1e-6).
- MAPTMD24: 230/230 provider rows matched, 0 unmatched, 0 duplicates, 46 cap-rule rows; 230/490 bins covered.
- Data side: 490 rows, lumi 351,000 nb⁻¹, C_gamma_born applied, c_rad not applied; strict-match ABORT gates all passed (validation JSON).

## 7. Files examined (primary evidence)

- `pythia8_rivet_z005_xfpos_20260702/outputs/{ep_seed830001,em_seed840001}/...n5000.yoda`; campaign routine `.cc` (diff vs smoke routine: identical)
- `pythia8_10M_epem_unitcount_xfpos/producer/aggregate_pythia8_yoda_unitcount_10M.py`, `run_pythia8_10M_aggregation.py`, `prediction_pythia8_{ep,em,epem}_..._490rows.csv`, manifest provenance JSONs
- `data_vs_mc_z005_20260704_10M_rivet_unitcount_xfpos_normfix_clean/producer_inputs_normalized/` (plotter CSV + `provenance_normfix_clean.json`)
- `djangoh14_crad_releasebin_2026-10-09/scripts/{crad_v3_worker_releasebin.py,finalize_releasebin.py}`; `crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`
- `data_vs_mc_z005_5generator_born_releasebin_2026-10-13/build_nonrad_truth_predictions_releasebin_2026-10-13.py`, `validation_nonrad_truth_predictions_releasebin.json`, NORAD plotter CSVs
- `build_maptmd_predictions_2026-10-13.py`, `validation_maptmd_predictions.json`, `data/MAPTMD/MAPTMD24_H1.csv`
- `build_data_side_noz_photonborn_2026-10-13.py`, `data_side_noz_photonborn/ibu/cross_sections.json`
- `sherpa3_10M_unitcount_xfpos/producer/prediction_sherpa3_sumw_xfpos_10M_sumw_2026-10-20_{bins,plotter}.csv`, `run_sherpa3_10M_sumw_rebuild_2026-10-20.py`
- `MC/scripts/compare_data_vs_mc_z005_4generator_born.py` (canonical) vs package-local copy (diff); `data_vs_mc_z005_5generator_born_releasebin_sherpa3sumw_2026-10-20/{invocation_2026-10-20.sh,data_vs_mc_z005_ratio_table.csv}`

---

# Addendum (2026-10-20): Scale-variation uncertainty inventory

**Question:** beyond the statistical uncertainties inventoried above, do the predictions in the canonical 5-prediction comparison carry scale-variation uncertainties?
**Method:** read-only, single-file checks on the NAF login node (no Condor; no existing file edited other than this addendum).

## A.1 Per-prediction scale-variation availability

| Prediction | Scale variations available? | Where | Evidence |
|---|---|---|---|
| SHERPA3 | **Yes — already on disk in every production YODA** (14 variation streams; not yet propagated to the CSV) | 18 weight streams per YODA; Z005 objects exist per stream (RAW `HISTO1D_V3` + non-RAW copies) | 4 files verified directly (100k base + seeds 910001/910500/910990): identical 18 stream names, 28 Z005 objects per stream, 1,207 objects per file; remaining 987/991 **assumed** by production uniformity — assert in the future aggregation job |
| PYTHIA8 | **No** | — | 0 bracketed (multi-weight) paths in the production YODA (68 objects, single nominal stream, unit weights); no `Variations:` directive in the `.cmnd` cards |
| RAPGAP31 | **No** | — | releasebin campaign = single weighted samples (`weight1×weight2`); the only "variants" are photon-virtuality q-variants (nominal/sigma_photon/esigma_photon) and EMPZ modes — radiative-study legs, **not** mur/muf scale legs; 490-row tables have no variation columns; 2026-09-25 handoff documents weights/luminosity only |
| DJANGOH14 | **No** | — | same campaign structure as RAPGAP31 (verified in the same scripts/tables) |
| MAPTMD24 | **Partial** — a total-theory 68% band exists and is already rendered; its **composition is not locally documented** | provider CSV `xsec_low/xsec_high` → `band_low/high_bin_nb` | documented as "the lower and upper edges of the 68% uncertainty band per the theorist's statement 2026-10-13 … a theory envelope, NOT a statistical error"; no scale/PDF/twist decomposition recorded anywhere in the package docs, validation JSON, or RUN_STATUS |
| Data side | **Not applicable** — scale variation is a theory concept | — | data-side uncertainties are experimental: IBU stat (toy), response-model, regularization (sidecar), detector components in the systematics budgets; no analogous need |

## A.2 SHERPA3 stream inventory (18 streams, per production YODA)

Every stream carries the full object set (67–68 objects), **including the 14 RAW Z005 `yield_zpT_flat` `HISTO1D_V3` histograms (35 physical bins each) and their 14 non-RAW copies** — i.e. the scale-variation streams exist for the released Z005 490-bin histograms, not just for bookkeeping objects. The only default-only object is `/TMP/_BEAMPZ`.

| # | Stream (YODA path suffix) | What it is |
|---|---|---|
| 1 | *(no suffix)* | **Nominal** MC@NLO weight (`Weight = MEWeight × WeightNormalisation/NTrials`) |
| 2–8 | `MUR={0.707106781187, 1, 1.41421356237}__MUF={…}__LHAPDF=261000` — 7 combinations | **Full renormalization/factorization scale variations** (ME + PS), 7-point set with √2 factors: (μr,μf) ∈ {(½√2,½√2), (½√2,1), (1,½√2), (1,1), (1,√2), (√2,1), (√2,√2)}. The central `MUR=1__MUF=1` stream is **bit-identical to nominal** (ratio = 1.000000 at the sampled bins) — an internal consistency proof that these are genuine scale variations around nominal |
| 9–15 | `ME.MUR={…}__ME.MUF={…}__ME.LHAPDF=261000` — same 7 combinations | **ME-only scale variations** (matrix element varied, PS at nominal) — diagnostic streams; their central combination is **not** equal to nominal (ratio ≈ 0.956–0.981 at the sampled bins), so they are not self-normalized and must not enter the standard envelope |
| 16–18 | `EXTRA__MEWeight`, `EXTRA__NTrials`, `EXTRA__WeightNormalisation` | per-event bookkeeping counters (ME weight, number of phase-space trials, normalisation factor) — not physics variations |

All streams tag the **same PDF set (`LHAPDF=261000`)** → **no PDF variations are stored**; the LHAPDF tag only records the nominal PDF ID. Net: 1 nominal + 14 scale-variation streams (7 full + 7 ME-only) + 3 bookkeeping.

## A.3 Quantified envelope (100k released-chain base; ratio of stream sumW to nominal sumW, per bin)

| Bin | Entries (nominal) | 7-point FULL envelope | ME-only envelope |
|---|---|---|---|
| S0_x0, flat_bin 0 (z 0–0.05, pT 0–0.5) | 26,783 | **[0.963, 1.044] → −3.7% / +4.4%** | [0.974, 0.999] → −2.6% / −0.1% |
| S3_x0, flat_bin 0 (z 0–0.05, pT 0–0.5) | 31,049 | **[1.000, 1.024] → −0.0% / +2.4%** | [0.943, 0.961] → −5.7% / −3.9% |
| S0_x0 cell total (35 bins) | — | [0.969, 1.026] → −3.1% / +2.6% | — |
| S3_x0 cell total (35 bins) | — | [0.999, 1.025] → −0.1% / +2.5% | — |

Robust statement: in well-populated bins the MC@NLO scale envelope is a **few-percent** effect (−3.7%/+4.4% at S0 low-z low-pT; −0.0%/+2.4% at S3 low-z low-pT). Caveat: the per-bin envelope over all 241 populated hplus bins has a wide 5–95% spread (low 0.712–0.997, high 1.005–1.201, median ≈ 0.965/1.045) with extremes of −4.93 and +3.42 at bins holding only 214/268 entries — these extremes are 100k-statistics artifacts (weighted MC@NLO content dominated by few events), not physical scale sensitivity. A definitive per-bin envelope requires the 10M aggregation (below).

## A.4 Propagation into the prediction CSV — formula and effort

Extend the existing sumW rebuild aggregation (`run_sherpa3_10M_sumw_rebuild_2026-10-20.py` pattern) to the scale streams — **same Condor-scale job over the 991 gz YODAs; no Rivet re-run, no `.cc` change, no generator change** (the weights are already on disk):

1. Per stream *s* and bin *b*: `S_s(b) = Σ_files sumW_s(b)` from the RAW Z005 `HISTO1D_V3` bracketed paths; `T_tot = Σ_files Σ NTrials` (from `/RAW/_EVTCOUNT[EXTRA__NTrials]`, scale-independent).
2. `sigma_s(b) [nb] = S_s(b) / T_tot / 1000` — the same self-normalizing form as the rebuild; algebraically `sigma_nom(b) × S_s(b)/S_nom(b)` (the ratio form makes the normalization cancellation explicit).
3. Envelope columns: `scale_low_bin_nb = sigma_nom(b) · min_s[S_s(b)/S_nom(b)]`, `scale_high_bin_nb = sigma_nom(b) · max_s[...]`, taken over the **7 FULL streams** (central ≡ nominal, so effectively the 6 non-central); ME-only streams recorded as separate diagnostic columns, **not** in the nominal envelope (their central ≠ 1).
4. **No clamping** — negative weighted content is physical MC@NLO RS content; flag low-entry bins (e.g. `numEntries <` threshold) instead of clamping.
5. Add a per-file stream-completeness assert (18 streams × 28 Z005 objects) to the job — this converts the 987-file assumption into an assertion at zero extra cost.

Effort: one Condor aggregation campaign, ~15× the per-stream read volume of the sumW rebuild (1 nominal + 14 streams), plus two envelope columns in the output CSV; the plotter's band-column mechanism (§4.2) can then render the scale band with no further schema work.

## A.5 What would be needed for the other generators

- **PYTHIA8:** dedicated re-runs with varied generator settings (Pythia8's on-the-fly `Variations:` weights were not enabled at production time; no varied samples exist on disk). For an LO+PS standalone prediction any such envelope would probe PS-scale dependence only and would be a new production campaign, not a recovery from existing files.
- **RAPGAP31 / DJANGOH14:** dedicated generator re-runs with varied QCD-scale settings in the ME/PS; no such samples exist (the campaigns' only variation legs are the photon-virtuality q-variants and EMPZ modes of the radiative study, which serve a different purpose — C_rad — and are not μ-scale variations).
- **MAPTMD24:** nothing to run — the 68% provider band already serves as the total-theory-uncertainty analog and is rendered on absolute panels. If the user wants it *labeled* as a scale uncertainty, the theorist must confirm the band's composition (scale vs PDF vs higher-twist); no such decomposition is documented locally.
- **Data side:** no action — scale variation has no data-side analog; the sidecar's stat/model/reg uncertainties and the detector systematics budget already cover the experimental side.

## A.6 Files examined for this addendum

- `sherpa3_mepstest_20260628/rivet_smoke_20260629/work/run_reuse_p_pattern_gcc14_100k_z005_xfpos_20260629/Analysis.yoda.gz` (stream enumeration + envelope)
- `sherpa_rivet_z005_xfpos_to1M_20260703/outputs/seed{910001,910500,910990}_n10000/Analysis.yoda.gz` (stream-completeness spot checks)
- `pythia8_rivet_z005_xfpos_20260702/outputs/ep_seed830001/ep_seed830001_n5000.yoda` + `cards/*.cmnd` (single-stream confirmation)
- `djangoh14_crad_releasebin_2026-10-09/` and `rapgap_crad_releasebin_emfix_2026-10-09/` scripts + 490-row table header (no scale legs; q-variants only)
- `sidis_highQ2/docs/handoffs/SESSION_HANDOFF_mc_4generator_comparison_2026-09-25.md`, `sidis_highQ2/docs/RUN_STATUS.md`, 2026-10-13 package `README.md` + `validation_maptmd_predictions.json` (MAPTMD band provenance)
