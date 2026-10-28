# Session Handoff — 4-Generator MC Comparison Plots, 2026-09-25

Scope: high-Q2 inclusive charged-hadron SIDIS (`Q2 > 150 GeV^2`), v6/z005.
Status of everything below: **PROVISIONAL INTERNAL DIAGNOSTIC**. Nothing here is approved physics.

---

## 1. TL;DR for the next session

1. Built a new 4-generator version of the canonical 7-column data-vs-MC comparison plot: PYTHIA8, SHERPA3, **plus new RAPGAP31 and DJANGOH14** truth-level curves. Package is complete and visually reviewed.
2. Added 8 figure PDFs to the analysis-note repo and **pushed them successfully** (commit `bf5e39d`).
3. Wrote the LaTeX to reference those figures in `sections/results.tex`, committed locally as `dadf971`, but **the push was REJECTED**: the user concurrently edited the same region in Overleaf. **This is the open item — see §5.**
4. Discovered that the previous 2-generator data-vs-MC plots used the **superseded luminosity** `297657.8 nb^-1`. See §4.

Highest-value next action: **resolve the `results.tex` merge conflict in §5.** Nothing else is blocked.

---

## 2. What was built

Package directory:
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_4generator_2026-09-25/`

Contents: 6 PDFs, 9 PNG page images at 150 dpi in `page_images_150dpi/`, 2 new prediction CSVs, the modified plotter copy `compare_data_vs_mc_z005_4generator.py`, `manifest_4generator.json`, `validation_4generator.json`, and `README.md` carrying the full caveat text.

The canonical plotter at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005.py` was **NOT modified**. A copy was made into the package directory and edited there.

---

## 3. Where the RAPGAP and DJANGOH curves come from — IMPORTANT

The obvious source, the gen-only `C_rad` study tables, is **BLOCKED and must not be used** for an absolute overlay:

- `.../studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/{djangoh14,rapgap31}_full_crad_v1_allcharges_490bins.csv`
- Their `*_sigma_proxy_nb` uses hard-coded aggregate exposures that are not publication-grade, and a wider truth fiducial (`GenEmpz` 35-70, `|GenEta_lab|<4`).
- v1 NONRAD totals are `154.354 nb` (DJANGOH) and `178.008 nb` (RAPGAP) against data `22.65 nb`, i.e. 6-7x. Not usable.
- RAPGAP v1 additionally has a quarantined RAD/NONRAD aggregate-lumi mismatch.

**The source actually used instead** is the central v6/z005 detector-MC generator-level truth prior, already in the exact release gen binning because it is the IBU prior and model cross-check:

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/_run/bbb_ibu_crosscheck_{rapgap,djangoh}_{pos,neg}.json`
- JSON path `response_aux.prior_gen`, 245 entries per charge.
- Equivalently `h_prior_gen_{pos,neg}_{rapgap,djangoh}` in
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/migration_inputs_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.root`

`prior_gen` is the **raw sum of MC truth weights** (`weight1*weight2`), NOT luminosity-scaled. Convert with a single generator-wide scale:

```
sigma_bin_nb = prior_gen_bin / L_MC_total
```

| Generator | L_MC_total (nb^-1) | s_gen | Periods (all four, verified) |
|---|---:|---|---|
| RAPGAP31 | `9132891.0` | `1.0949435397838428e-07` | 7097 ep0304, 7096 em0405, 7071 em06, 7094 ep0607 |
| DJANGOH14 | `10487495.0` | `9.535165451807129e-08` | 7093 ep0304, 7092 em0405, 7072 em06, 7091 ep0607 |

All four periods per generator were confirmed present in the response (`meta_n_rapgap_files=4`, `meta_n_djangoh_files=4`), so the single generator-wide scale is valid. Per-period luminosities: `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`.

Weighted-not-counts confirmed: prior/count integral ratios are RAPGAP `1.40794/1.40824`, DJANGOH `1.33176/1.33080`, distinctly non-unity.

### Validated 490-bin totals

| Source | Total (nb) |
|---|---:|
| Data (v6/z005 IBU, `L_data=351000.0 nb^-1`) | `22.645664` |
| PYTHIA8 (Rivet 10M) | `19.100458` |
| SHERPA3 (Rivet 10M) | `28.961821` |
| RAPGAP31 (response prior) | `19.924621` |
| DJANGOH14 (response prior) | `19.559339` |

MC stat uncertainty on the two new curves is an approximate `sqrt(N)` from the unweighted `h_mc_count_*` histograms, **not** a true `sumw2`. Recorded in the manifest.

### Mandatory caveats for these two curves

- **RAPGAP31 is the central unfolding response**, so its curve is **NOT statistically independent** of the unfolded data. DJANGOH14 is the model-response cross-check.
- RAPGAP31 is a diagnostic overlay only: `P-RAPGAP` remains open. Not nominal, not a generator-envelope systematic.
- Data release is raw: no background subtraction, no `C_rad`, not Born level.

---

## 4. Discovered defect — old plots used the superseded luminosity

The previous 2-generator comparison at
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/plots/data_vs_mc_z005_20260705_10M_rivet_unitcount_xfpos_normfix_clean_differential4d_xcap1p0_correctedinputs_with_panelratio_dataunc/`
consumed a release snapshot normalized with the **superseded 2005-2007-only** `297657.8 nb^-1`.

- Its data total reads `26.7039134892 nb`; the correct current value is `22.645664 nb`.
- The data denominator is low by exactly `297657.8 / 351000.0 = 0.84802792` in **all 490 bins**.
- PYTHIA8 and SHERPA3 MC values were **unaffected**: max abs difference `0.000e+00`, distinct new/old MC ratio set exactly `{1.0}`.
- Cause: the release was re-run on 2026-09-23 with the correct four-period S67 `351000.0 nb^-1` including ep0304.

**Consequence:** the three note figures `fig_results_data_vs_mc_z005_4d_*_20260705.pdf` are superseded. Do not quote `26.7039 nb`.

---

## 5. OPEN ITEM — analysis-note push is blocked on a merge conflict

### What is already pushed and safe

Commit `bf5e39d` on `development`, **pushed successfully**, added 8 figure PDFs to `figures/`:

- `fig_results_data_vs_mc_z005_4d_4gen_absolute_and_panelratio_combined_20260925.pdf` (4 pages)
- `fig_results_data_vs_mc_z005_4d_4gen_pTh_ratio_{hplus,hminus}_20260925.pdf`
- `fig_results_data_vs_mc_z005_4d_4gen_absolute_{hplus,hminus}_20260925.pdf`
- `fig_results_data_vs_mc_z005_4d_4gen_ratio_by_global_bin_20260925.pdf`
- `fig_results_xsec_density_release351_expanded_7panel_{hplus,hminus}_z005_20260924.pdf`

### What is stuck

Local commit `dadf971` edits `sections/results.tex` to reference those figures. **Push rejected.** The user pushed Overleaf commits `ca092d3` and `05e9e1e` touching the same lines.

Repo state left clean and safe:
- Local branch `development` at `dadf971`.
- `origin/development` at `05e9e1e`.
- Backup branch **`backup-premerge-dadf971`** points at `dadf971`. The work is not lost.
- A trial merge produced **9 conflict hunks** in `sections/results.tex` and was **aborted**. Working tree is clean apart from the user's own unrelated in-progress `sections/systematics.tex` and 6 untracked `fig_syst_uncertainty_sources_expanded_*` PDFs, which were deliberately never staged.

### Why it conflicts — these are semantic, not mechanical

| Overleaf change (`05e9e1e`) | Conflicting local change (`dadf971`) |
|---|---|
| Deleted the combined `fig:results:4d-data-mc-combined` figure block entirely | Repointed it to the new 4-generator combined PDF |
| Repointed the two ratio figures to `fig_results_data_vs_mc_z005_4d_pTh_ratio_*_20260705.pdf` (the **old 2-generator** plots) | Repointed them to the new 4-gen `..._4gen_..._20260925.pdf` |
| Renamed subsection to `\subsection{Cross Sections}` | Edited prose inside that subsection |
| Replaced the long caveat paragraph with one line about `Q^2<60 GeV^2` background and radiative correction | Expanded the caveats |
| Deleted subsections `Interpretation of Present Tables` and `Low-Q^2 Companion Status` | Not touched locally |

Note the Overleaf version currently points at the **old 2-generator figures**, which carry the superseded-luminosity data denominator described in §4.

### Resolution options put to the user (not yet answered)

- **(A) Recommended.** Overleaf structure wins. Only swap the two ratio figure paths to the 4-gen `20260925` files and add the release351 subsection. Keeps the user's trimmed prose and deletions.
- **(B)** As A, plus restore the combined 4-gen figure Overleaf deleted.
- **(C)** Local `dadf971` wins wholesale. Reverts the user's caveat trims and section deletions. Not recommended.
- **(D)** Walk the 9 conflict hunks one by one.

### Content of the blocked commit, for reuse

`dadf971` did four things to `sections/results.tex` (`+106 / -24`):

1. Repointed the three data-vs-MC figures to the 4-gen `20260925` PDFs, keeping labels `fig:results:4d-data-mc-{combined,hplus,hminus}`.
2. Rewrote the four-dimensional subsection prose and captions for four generators, including the `L_MC` values, period lists, and the five 490-bin totals from §3.
3. Fixed two pre-existing broken alpha0p2 paths by adding the missing `_z005` suffix, with no prose change, since that subsection is specific to the near-electron veto.
4. Added a new subsection `sec:results:release351-expanded-xsec-density` with the two release351 expanded-systematics density figures.

That commit was verified: all 34 figure references across all sections resolved, and the note built with CVMFS TeX Live 2025 to **71 pages with zero undefined references**.

Build requirement, unchanged: `export PATH=/cvmfs/sft.cern.ch/lcg/external/texlive/2025/bin/x86_64-linux:$PATH`. The system `/usr/bin/pdflatex` lacks `tcolorbox.sty` and `cleveref.sty`.

Pre-existing defect note: `results.tex` previously referenced 5 figure paths that omitted the `_z005` infix and resolved to missing files. Overleaf independently fixed the two alpha0p2 ones in `05e9e1e`.

---

## 6. Reproducing the plots

```
cd /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_4generator_2026-09-25/
MPLBACKEND=Agg python3 compare_data_vs_mc_z005_4generator.py \
  --release-dir /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10 \
  --prediction-csv <pythia8 plotter csv> \
  --prediction-csv <sherpa3 plotter csv> \
  --prediction-csv prediction_rapgap31_response_prior_gen_truth_v6z005_L_MC_scaled_plotter.csv \
  --prediction-csv prediction_djangoh14_response_prior_gen_truth_v6z005_L_MC_scaled_plotter.csv \
  --out-dir . --lumi-pb 351.0 --differentiate-4d
```

The PYTHIA8 and SHERPA3 plotter CSVs are the current canonical 10M `normfix_clean` inputs under
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/plots/data_vs_mc_z005_20260704_10M_rivet_unitcount_xfpos_normfix_clean/producer_inputs_normalized/`.
Exact paths are recorded in `manifest_4generator.json`.

Note on SHERPA3: this package uses the 10M `normfix_clean` CSV totalling `28.961821 nb`. An older handoff quotes `26.578692 nb` for "Sherpa 1M corrected" — that is the superseded 1M sample, not this input.

Cosmetic state, per user request on 2026-09-25: figures carry only an `H1 Work In Progress` title and the shared legend. All caveat text was deliberately moved off-figure into `README.md` and `manifest_4generator.json`. Absolute panels use mathtext scientific y-tick labels capped at 3 ticks. The global-bin ratio figure is display-clipped to `[0, 3]` with `123` finite points off-scale.

---

## 7. Do-not-repeat list

- Do **not** use `full_crad_v1` or `full_crad_v2` tables as an absolute MC overlay. §3.
- Do **not** quote `26.7039 nb` as the data total. It encodes the superseded `297657.8 nb^-1`. §4.
- Do **not** reuse the `7110` DjBkgd luminosity `15813.4 nb^-1` or scale `22.2017` for sample `7091`.
- Do **not** scale each period to full data luminosity then sum. Sum raw weights per generator, then apply one generator-wide scale.
- Do **not** edit the canonical plotter `MC/scripts/compare_data_vs_mc_z005.py` in place. Copy it into the output package first.
- Do **not** stage the user's in-progress `sections/systematics.tex` or the 6 untracked `fig_syst_uncertainty_sources_expanded_*` PDFs.
- Do **not** present RAPGAP31 as independent validation of the unfolded data. It is the central response.
