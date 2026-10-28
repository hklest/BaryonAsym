# One-file high-x, high-z, lowest-pT_h radiation-migration diagnostic — 2026-09-22

> **DO_NOT_USE_FOR_CRAD_OR_PRODUCTION.** This bounded result uses only the biased one-file DJANGOH14/7091 RAD smoke output. It is forbidden for `C_rad`, acceptance, response/unfolding, normalization, central-release, or Born-level interpretation.

## Result

The validated diagnostic QA contract passed for this bounded use. Using the highest ROOT cycles (`ChargedTot;2` and `SidisRadiationMigrationEvent_DO_NOT_USE_FOR_CRAD_OR_PRODUCTION;1`), the exact RunNumber/EventNumber join was complete for 303,930 track rows and 95,054 unique event diagnostic rows.

The selector was the current finalcandidate eSigma-baseline reco method: `150 <= Q2s < 20000`, `0.1 < Ys < 0.7`, `|Eta| < 1.6`, `RadLength >= 5`, `NHitDedx >= 5`, finite `xF_SIDIS_es < 0`, and valid `0 <= z_SIDIS_es < 1`. The high-x cells were `S0_x1` (`150–300`, `0.0071–0.03`), `S1_x1` (`300–700`, `0.016–0.069`), `S2_x1` (`700–1500`, `0.034–0.15`), and `S3_x0` (`1500–20000`, `0.021–1.5`) in `(Q2s,xBj)`. The target was `0.70 <= z_SIDIS_es < 1.00` and `0 <= pT_h < 0.25 GeV`.

There are **2 selected tracks in 2 events**, with available raw `weight1*weight2` sum **2.0**. Both are h−, fiducial matched, `GenRad=0`, and `MsubprGevc%100=1`; hence the selected-track fractions are 100%, 100%, 100%, and 100%, respectively. There are no selected ISR or FSR rows, so photon-energy and ISR-loss conclusions are unavailable for this strict target in this file.

Both target tracks are outside the S3 high-x target in hard-GKI, dressed-observable, and `_es_gen` event coordinates (or invalid in those coordinates), while reconstructed coordinates are target by construction. This is an event-coordinate migration statement only. The file has no exact hard-level hadron `z` or photon-axis `pT_h`; do not infer a hadron-level source bin. For the two matched tracks only, the stored/reconstructed gen-observable summaries are median `zGen=0.70847` and median photon-axis `pT_h=0.59462 GeV`.

## Conventions and bounds

`pT_h` is **not** lab `Pt`: it was reconstructed from `(Px,Py,Pz)` with pion mass 0.13957 GeV, using the eSigma virtual photon from `E'_eSigma=Q2e/[2 E_e(1+cos(theta_e))]` (stored scattered-electron energy only as fallback). The hadron and photon were boosted by `-(q+P).BoostVector()` to the gamma-star–proton CM; `pT_h` is the component of the boosted hadron momentum perpendicular to the boosted photon axis. `z_SIDIS_es` is the existing builder eSigma SIDIS z branch. This is the current reco convention available in the diagnostic tree.

`z_SIDIS_es` sentinel/nonphysical exclusions before the target z/pT selection were: GenRad 0: 58/41,776 (0.1388%); GenRad 1: 12/14,002 (0.0857%); GenRad 2: 70/15,164 (0.4616%). The QA contract records five ISR event rows with `ISR_effective_s=-999`; do not infer its validity from finite values. The available generator-weighted figures are raw `weight1*weight2` sums and are not normalized.

The two bounded identifiers are in `bounded_event_identifiers.csv`; they are examples only and are not representative claims.

## Artifacts

- Machine-readable summary: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiation_migration/highx_highz_lowpt_onefile_2026-09-22/highx_highz_lowpt_onefile_summary.json`
- Compact tables: `breakdown.csv`, `event_coordinate_migration.csv`, `sentinel_exclusions.csv`, and `bounded_event_identifiers.csv` in the same directory.
