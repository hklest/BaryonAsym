# Cone-basis S2' Born-level cross-section tables (public product, 2026-10-16)

Config-driven LaTeX cross-section tables for the canonical **cone-basis S2'
observable** (alphaveto-cone promotion 2026-10-13, CURRENT; leg-envelope budget
2026-10-14; `sidis_highQ2/STATE.md` cone-baseline promotion override). Generated
by `sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09/generate_latex_cross_section_tables.py`
**v1.2.2** from `table_config_cone_public_2026-10-16.json` (public profile,
extends `table_config_cone_2026-10-16.json`, extends `table_config.json`).
This product **supersedes
`sidis_highQ2/data/tables/highQ2_z005_s2prime_born_public_2026-10-11b/`**
(no-cone S2' basis) for the analysis-note appendix tables. The layout, column
set, file formats, and extensions are identical to 2026-10-11b; the numbers
move to the cone basis, the leg-envelope budget, and — for IBU — the 1000-toy
statistical sidecar chain (see **Statistical column**, the one deliberate
convention deviation). The repoint is purely config-driven.

## Basis (quoting rule: prior basis + level + observable basis)

- **Observable basis**: cone-basis S2' — S2' detint-fake response variant
  (det.int. Class 2 removed response-internally via enlarged per-reco-bin
  `f_i`; k_parented charged-K DIF kept as signal; external det.int.
  subtraction FORBIDDEN) PLUS the `alpha >= 0.2 rad` laboratory-frame
  electron-cone veto on the reco legs (after the e/V0 truth vetoes, before
  the z-guard / matched-fake classification / det.int. reroute; gen side
  unchanged; cone-rejected matched legs flow to the miss row).
- **Level**: Born (`c_rad_applied=true`), `sigma_Born = C_rad x Unfold(data;R)`;
  raw parent = the cone raw release. noZ set = photon-only Born
  (`C_gamma_born x sigma_raw`).
- **Prior**: coherent `body+miss` computed FROM the cone+S2' variant rapgap31
  response (R10-asserted), IBU `k=4`, `L_data = 351000.0 nb^-1`.
- **Inputs** (full sha256 and row counts in `provenance.json`; generator
  sha256 `ad4ae2c028001f13...`, config chain sha256s recorded there):
  - Born release `born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/ibu/cross_sections.csv`
    (`ab53e9b6695dd85f...`, 490 rows);
  - raw parent `bbb_ibu_detints2prime_alphaveto_cone_..._2026-10-13/ibu/cross_sections.csv`
    (`53bd0b568e7a9f09...`, 490 rows);
  - 15-component leg-envelope budget `systematics_cone_s2prime_15comp_shiftedlegs_legenv_..._2026-10-14/total_uncertainty_summary_cone_variant_15comp.csv`
    (`568040fefb868680...`, 490 rows; published total column
    `total_with_cone15_rel`);
  - C_rad tables UNCHANGED: `djangoh14_crad_releasebin_2026-10-09`
    (`cf810be44a59ed79...`) and `djangoh14_nozborn_crad_releasebin_2026-10-09`
    (`7ec901af36d93b7f...`);
  - previous-product sidecars (factor gate reference, tol 0.0):
    `highQ2_z005_s2prime_born_public_2026-10-11/numbers_{withZ,noZ}.csv`.
- Integrated IBU (asserted by the `release_meta` gate against the releases'
  own `summary_totals.json`): withZ h+ `13.026987 nb` / h- `12.697511 nb`;
  raw h+ `11.412233 nb` / h- `11.123626 nb`; noZ (self-derived, gate tol
  1e-9) h+ `11.875722 nb` / h- `11.571280 nb`. BBB branch (verify-only):
  withZ h+ `13.004519 nb` / h- `12.682479 nb`.

## Column set (identical to 2026-10-11b)

`z range | pT_h range (GeV) | sigma (nb) | stat. unc. (nb) | syst. unc. (nb)
| C_rad +/- unc | C_gamma_Born +/- unc | Delta V (GeV^3)` — both factors in
both sets, fixed order; own-convention factor applied, other reference-only;
`C_gamma_born / C_rad = F_EWgamma` gated per bin. Digit rule unchanged
(4 significant digits; factors value 3 / uncertainty 2; `tabcolsep 1pt`).
Sidecars `numbers_withZ.csv` / `numbers_noZ.csv`: same 65-column schema as
2026-10-11b (verified identical header).

## Statistical column — DEVIATION from the superseded 2026-10-11b set

**IBU stat now uses `toy_stat_unc_nb`** = (1000-toy sidecar / `L_data`) x
`C_rad` — the C4b covariance drop-in chain, equal per bin to the budget
`ibu_stat_rel` (max deviation 5.6e-17) — **not** the release-job
`stat_unc_nb` that 2026-10-11b used for both methods. This matches the
analysis-note systematics text ("IBU statistical uncertainty & sigma of 1000
toy replicas") and the budget component. BBB keeps `stat_unc_nb` (the BBB
toy column is all-empty; no BBB toy chain exists). On this release the two
IBU columns agree to -16.9% .. +27.8% per bin (median -0.5%); vs the shipped
2026-10-11b stat values the per-bin change is -31.4% .. +25.6% (median
-0.1%; 98/490 bins beyond +/-10%, 4 beyond +/-25%) — flagged, not
investigated (source change + cone basis; the noZ set's stat is the raw-level
chain, `stat_raw = Born stat / C_rad`, so the relative stat is identical
between the sets, worst deviation 3.7e-16). Recorded per run in
`provenance.json` `stat_column`.

## Systematic column convention

Default systematic = quadrature of 13 of the 15 leg-envelope budget
components (EXCLUDED: `ibu_stat_rel`, carried by the stat column;
`c_rad_stat_rel`, carried by the factor column). The budget's five detector
components are FULL-CHAIN from the shifted-legs campaign (RAPGAP31 governs;
leg-level envelope), so the medians DROP vs 2026-10-11b's data-side budget:
h+ `4.4745%` / h- `4.4660%` (were `6.394%` / `6.540%`); per-bin syst change
-88.9% .. +119.0% (263/490 bins beyond +/-25%) — a known budget-convention
change, flagged, not investigated. `tracking_eff_m0p5_rel` remains
data-side conservative. `--sys-mode published_total` switches to the
budget's own `total_with_cone15_rel` (medians h+ `5.8032%` / h- `6.1919%`).

## Bin-volume convention (unchanged from 2026-10-11/11b, stated explicitly)

The FINAL column of every table is the four-dimensional bin volume
`dV = dQ2 * dx * dz * dpT_h` (GeV^3) from the release edge columns. For S3_x0
(`x_max = 1.5` bookkeeping) the x width is capped at the physical limit:
`dx = 1.0 - x_min` (never the bookkeeping width). `bin_volume_gev3` is
bit-identical to 2026-10-11b per bin (max deviation 0.0); captions state
that entries are integrated over the bin volume and that predictions must be
integrated over the same volume. First z bin 0.0-0.05 and first pT_h bin
0.0-0.5 GeV (path tags `zlow005`/`pT5` denote first-bin edges, not cuts).

## Validation

Run-time gates (hard; details in `validation.json`):

- Public binned (top level): **16/16 gates PASS** (1 report-only),
  `pdflatex` compile PASS, 0 overfull boxes (text width margin 455.24 pt).
- Internal audit (`internal/`): **15/15 gates PASS** (no token gate in
  internal style), compile PASS, 0 overfull.
- Density (`density/`, public profile): **16/16 gates PASS**, compile PASS,
  2 overfull worst 5.42 pt (report-only; same irreducible density-cell
  width as 2026-10-11b).
- BBB branch verify-only (public profile): **16/16 gates PASS**
  (withZ BBB totals reproduce the release metadata; factor own-deviation 0.0).
- `factor_columns_consistency`: own-convention factor value AND total
  uncertainty bit-identical to the 2026-10-11 sidecars (tolerance 0.0, worst
  deviation 0.0 on 980 rows) — the C_rad / C_gamma_born tables are unchanged
  by the cone promotion; `C_gamma_born / C_rad == F_EWgamma` worst 2.6e-16.
- `release_meta`: computed withZ integrals reproduce
  13.026987454844365 / 12.697511232683578 (worst 9.3e-16); raw integrals
  reproduce 11.412233 / 11.123626; `ibu_selected_iter=4`; lumi 351000.0.

External post-generation checks (30/30 PASS; script + log retained in the
generating session): 245+245 rows per mode in all three sets; all sigma /
stat / syst finite, no negatives, no zero-sigma rows (IBU); sidecar withZ
sums reproduce the Born integrals (worst 4.2e-16) and `sigma_raw_nb` sums
reproduce 11.412233 / 11.123626 (exact); per-bin
`budget_published_total_rel` == legenv CSV `total_with_cone15_rel` (max
deviation 0.0; medians 5.8032% / 6.1919%); stat == release
`toy_stat_unc_nb` bitwise; noZ stat rel == withZ stat rel (3.7e-16); syst ==
13-component legenv quadrature (1.1e-16); density == binned / volume
(exact); bin volumes and block inventory (28 per set) identical to
2026-10-11b.

Change scan vs 2026-10-11b (FLAG ONLY, not investigated): sigma -47.5% ..
+4.3% (median -0.0%; 20/490 bins beyond +/-10%, 6 beyond +/-25%; largest
drops neg bin 304 and pos bin 59 — the cone veto's per-bin effect,
integrated -0.29%/-0.31%); stat and syst as quoted above.

## Reproduction

```bash
cd sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09
export PATH=/cvmfs/sft.cern.ch/lcg/external/texlive/2025/bin/x86_64-linux:$PATH
PROD=/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/results/cone_s2prime_born_public_tables_2026-10-16
python3 generate_latex_cross_section_tables.py --config table_config_cone_public_2026-10-16.json            # R1: public binned -> $PROD
python3 generate_latex_cross_section_tables.py --config table_config_cone_2026-10-16.json                   # R2: internal    -> $PROD/internal
python3 generate_latex_cross_section_tables.py --config table_config_cone_public_2026-10-16.json --sigma-form density --out $PROD/density  # R3
python3 generate_latex_cross_section_tables.py --config table_config_cone_public_2026-10-16.json --verify-only --method bbb --out <scratch>  # R4: BBB verify
# then remove the sub-run reports (one report set at top level), as shipped here:
rm -f $PROD/internal/{validation.json,provenance.json,compile_check.*} $PROD/density/{validation.json,provenance.json,compile_check.*}
```

Pure-Python stdlib over single CSVs; login-node execution (no production
ROOT files touched, no Condor needed).

## Provenance and supersession

- Supersedes `sidis_highQ2/data/tables/highQ2_z005_s2prime_born_public_2026-10-11b/`
  (no-cone S2' basis; retained untouched — its sidecars remain the factor
  gate reference). Pre-S2' products remain historical/audit only.
- Intended consumer: the analysis-note appendix cross-section tables
  (`sections/appendix_cross_section_tables.tex`); install mechanically from
  `blocks/` (28 per-charge files, same names as the note's current set).
- Status: PROVISIONAL / not physics-final. P-CRAD-LUMI FILE-SET-RECONCILED
  (C_rad not publication-grade); releasebin baseline provisional; EMPZ>45
  convention CLOSED 2026-10-09; raw/no-bgsub with response fake-fraction
  subtraction only (cone+S2' RG-internal integrated `f_i` 11.2335% h+ /
  10.8698% h-); no species PID; multiplicities blocked; mixed-beam validity
  only; no external det.int. subtraction on top of this basis.
