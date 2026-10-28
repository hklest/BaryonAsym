# C_rad baseline provenance — xFGen<0 re-derivation (2026-10-06)

**STATUS: PROVISIONAL DIAGNOSTIC until reviewed. `c_rad_applied=false`.** This baseline
supersedes the blind (`xf_cut: False`) FINAL of 2026-09-30 for all Born-level use, per the
BLK-7.1 ruling (hemisphere convention: the blind tables under-correct `+0.35/+0.38%` (RAPGAP31) and `+0.37/+0.41%` (DJANGOH14) integrated;
the bias is confined to z<0.05 bins).

## Definition

Per-bin, gen-level `C_rad = sigma_nonrad/sigma_rad`, `sigma_proxy = sum(weight1*weight2)/L_MC`,
current-hemisphere cut `xFGen<0` applied on BOTH the RAD and NONRAD legs, EMPZ>45 nominal
variant, weights `weight1*weight2`. xF is recomputed from `(GenPx,GenPy,GenPz,GenE)` with the
q-nominal construction (stored RAD `xFGen` branch not used; nominal-beam-q bug under ISR,
bounded +0.039% integrated). Exposures reused verbatim from the 2026-09-30 FINAL finalizers
(`oomclumi`, `DIAGNOSTIC_MANIFEST_LEVEL`).

## Tables and values (490 bins = 245 per charge; both 490/490 finite)

| Generator | Table | Integrated h+ | Integrated h- |
|---|---|---|---|
| RAPGAP31 (nominal) | `data/radiative/rapgap_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` | `1.1368045` | `1.1362373` |
| DJANGOH14 (cross-check) | `data/radiative/djangoh14_crad_final_xfcut_2026-10-05/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` | `1.1418534` | `1.1413756` |

Cross-generator agreement `+0.444%` (h+) / `+0.452%` (h-). z>=0.05 bins are stable vs the
blind FINAL in aggregate; the shift is confined mostly to the z<0.05 row.

## Campaign and validation

Condor clusters `5339645`/`5339646` (histogramming initial), `5339679`/`5339680` (retry),
`5341298`/`5341299` (finalize). Validation and provenance artifacts (G1–G6 gates, worker-fork
sha256, queue provenance): `crad_rapgap31_xfcut.json`, `crad_djangoh14_xfcut.json`, and
`xfcut_campaign_manifest.json` in the same two directories. Current 7-column comparison
package: `plots/radiative/crad_v4_xfcut_djangoh_vs_rapgap_7col_2026-10-06/`.

## Supersession

Blind FINAL products are quarantined (history only, do not use):
`data/radiative/quarantine/rapgap_crad_final_2026-09-30/` and
`data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/`; blind plot
package archived at `plots/old/crad_v4_djangoh_vs_rapgap_7col_2026-09-30/`. Their record:
`docs/radiative/CRAD_FINAL_PROVENANCE_2026-09-30.md`. Pending: the 2026-10-05 chain born
tables (`H1ChargedSidis_HighQ2/output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/c_rad_sidecars/`)
still carry the blind C_rad and need an S3 re-attach with this baseline.

Historical leftovers, retained deliberately as executed-run records (supersession markers, not live claims): the dated g4/spec/closure-plan docs (`g4_rint_expectation_recalibration_2026-10-04.md`, `spec_nonrad_rad_phih_v3*`, `CRAD_CLOSURE_PLAN_2026-09-30.md`) and the campaign JSONs' `blind_final_tables` provenance lines keep their blind-era path/value references.
