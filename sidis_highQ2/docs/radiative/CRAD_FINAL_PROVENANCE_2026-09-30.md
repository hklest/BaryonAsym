# C_rad FINAL provenance — high-Q2 charged-hadron SIDIS (2026-09-30)

> **SUPERSEDED 2026-10-06:** the blind (`xf_cut: False`) FINAL documented here is superseded for all Born-level use by the xFGen<0 xFcut baseline — see `CRAD_BASELINE_XFCUT_PROVENANCE_2026-10-06.md`; its tables are quarantined under `data/radiative/quarantine/` and its 7-column plot package is archived under `plots/old/crad_v4_djangoh_vs_rapgap_7col_2026-09-30/`. Retained unchanged below as the historical record.

**STATUS: COMPLETE AND FINAL for DJANGOH14 and RAPGAP31.** `c_rad_applied=false` — values are final but NOT applied to the central v6 release, which remains raw/no-`C_rad` and not Born-level.

`C_rad = sigma_nonrad / sigma_rad`, `sigma_proxy = sum(weight1*weight2)/L_MC`. EMPZ>45, variant `nominal`, 490 bins (245/charge).

| Generator | h+ | h- |
|---|---|---|
| DJANGOH14 | 1.1375979129031535 | 1.1366708715220577 |
| RAPGAP31 | 1.1328903059596749 | 1.1319063751575906 |

Cross-generator agreement `0.41%` both charges. RAPGAP 490/490 finite; per-bin min/median/max `0.8940`/`1.1402`/`1.5301`.

## DJANGOH14 chain (pre-existing, unchanged this session)

- Trees: `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/rad/{ep,em}/` (1373), `output_v3/nonrad_old/{ep,em}/` (520), `output_v3/nonrad_new/{ep,em}/` (1060)
- Exposure (nb^-1): RAD ep `16206393.0`, em `13534017.0`; NONRAD_OLD ep `12920115.855`, em `12841572.442`; NONRAD_NEW ep `1717376.821`, em `1699702.771`
- Result: `sidis_highQ2/data/radiative/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_empz_gt45_nominal_490rows.csv`

## RAPGAP31 chain (built this session)

Sample: ReqIDs `8947-8954` mirror DJANGOH `8955-8962`; release `4.0.25`, `RAD.Q2GT60.CTEQ6L`, `S39600.R97905`, under `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/rapgap31/<reqid>/`.
Counts: 8947 ep 188, 8948 ep 201, 8949 ep 201, 8950 ep 101, 8951 em 201, 8952 em 166, 8953 em 201, 8954 em 14 = ep 691 + em 582 = **1273**.

### Step 1 — RAD trees
Campaign `.../studies/radiative_correction_genonly_v1/condor/genonly_v3_rapgap_rad_20260929/` (`build_campaign.py`, `run_one_v3.sh`, `rapgap_rad_queue.tsv`, `rapgap_rad_v3_20260929.sub`).
Binary `bin/x86_64-el9-gcc14-opt/RC_treebuilder_genonly_v3` sha256 `ba21ee5f95395a8607190e5936bb56b880eb3c5ad5d43089ee0e0813c9cda931`; source `src/RC_treebuilder_genonly_v3.C` sha256 `deef89f6a847d1c3569e88a3abd657913087820e8dbdde448664e4661fa73f18`.
Output `output_v3/rapgap_rad/{ep,em}/`, 1273/1273, 208.8 GiB. Clusters: smoke `5276091` (10/10), production `5276093` (1233 exit 0 + 30 exit 127), retry `5276095` (30/30).

### Step 2 — NONRAD trees
Source `/data/dust/group/h1/britzger/norad_prod21/dst2all_rapgap/` — 520 files, `RAPGAP31.NC.{EPLUSP,EMINUSP}.NORAD.Q2GT100.CTEQ6L.{A-H}.C00000NN`, 264 ep + 256 em.
Campaign `.../condor/genonly_v3_rapgap_nonrad_20260930/`. Output `output_v3/rapgap_nonrad/{ep,em}/`, 520/520, 98.7 GiB. Clusters: smoke `5303262` (6/6), production `5303263` (514/514).

### Step 3 — exposures (`oomclumi`)
Formula `L[nb^-1] = 1000 * sum_chunk_lumi[pb^-1]`; weighted convention `sumW = SIGMT[pb] * Lumi[pb^-1]`. Identical to DJANGOH reference `sidis_highQ2/data/radiative/rad_oomclumi_exact_v2_20260924/rad_oomclumi_exact_v2_20260924.json`.
Binary `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/bin/x86_64-el9-gcc14-opt/oomclumi`.

- RAD: worker `sidis_highQ2/data/radiative/rapgap_rad_exposure_2026-09-29/run_oomclumi_chunk.sh`, clusters `5303258`+`5303259`, 1273/1273. ep `17536474.0` + em `14461067.0` = **`31997541.0 nb^-1`**. Artifact `.../rapgap_rad_oomclumi_exact_20260930.json`.
- NONRAD: worker `sidis_highQ2/data/radiative/rapgap_nonrad_exposure_2026-09-30/run_oomclumi_chunk.sh`, clusters `5303264`/`5303265`/`5303266`, 503/520. ep `13954768.0` + em `13416991.0` = **`27371759.0 nb^-1`**. Artifact `.../rapgap_nonrad_oomclumi_exact_20260930.json`.

Per-ReqID RAD (nb^-1): 8947 `4776992.0`, 8948 `5099485.0`, 8949 `5096293.0`, 8950 `2563704.0`, 8951 `5056942.0`, 8952 `4177497.0`, 8953 `5054884.0`, 8954 `171744.0`.

### Step 4 — histogramming
Worker `.../scripts/histogramming_v3/crad_v3_worker.py`, **unmodified** (generator-agnostic; `--sample rad|nonrad --empz-modes empz_gt45`).
Partials `output_v3/rapgap_hist_20260930/partials/{rad,nonrad}/{ep,em}/`, 1777/1777. Clusters `5303267`+`5303268`.

### Step 5 — finalize
**New fork** `.../scripts/histogramming_v3/crad_v3_finalize_rapgap.py`, from `crad_v3_finalize.py`, three changes:
1. lines 8-10 exposures replaced with RAPGAP values; `NEW` set to zero (RAPGAP has a single NONRAD set)
2. partial glob `nonrad_old` -> `nonrad`, `nonrad_new` -> `_absent`
3. contract string `crad_rapgap31_fork_2026-09-30`, `generator: RAPGAP31`

Outputs `sidis_highQ2/data/radiative/rapgap_crad_final_2026-09-30/`: `crad_v3_empz_gt45_nominal_490rows.csv`, `..._yresolved_2940rows.csv`, `crad_v3_summary.json`.

## Mandatory caveats

1. **17 NONRAD chunks (all `C0000032`) are degenerate** — 8448 entries vs ~156000 typical; `oomclumi` returns `Sigma=0.000, Lumi=-nan`, reproduced interactively. Excluded from BOTH exposure and histogramming so numerator and denominator stay matched. Any rerun must exclude them identically.
2. **`projection_regression_pass=False` in the RAPGAP summary is a fork artifact**, not a data defect: the `_absent` glob is empty and the check returns False on an empty list. Per-partial projection verified directly — `sumw == sumw_y.sum(axis=0)` True, totals match at `838524`.
3. **ReqID 8951 SIGMT is not constant**: 106 chunks at `15030.704 pb`, 95 at `15033.014 pb` (0.015%). Does not affect the luminosity sum; indicates two generator sub-runs.
4. **Numerator-only `Q2Gki>100` cut applied to RAD** because NONRAD is `Q2GT100` and RAD is `Q2GT60`. In-fiducial effect ~0.09% of RAD events.
5. Exposure provenance label `DIAGNOSTIC_MANIFEST_LEVEL` for both generators.

## Operational traps hit this session

- `thish1.sh` exits non-zero and references an unset `GROUPROOT`; wrappers must NOT use `set -e`/`set -u` around it.
- `retry_until` is STOP-on-true; AFS flakes need `(ExitCode != 127)`, never `== 127`.
- A worker missing its executable bit gives exit 126 on every job (login-node `bash script.sh` test will not catch it).
- An `oomclumi` scratch dir keyed only on `(REQID, FILE_INDEX)` collides across beams; the path must include beam.

## SUPERSEDED — DO NOT USE

- RAPGAP `fLumi-v3` diagnostic `.../output/diagnostics/rapgap_fLumi_diagnostic_2026-06-24/full_nonrad_map_v3_with_unc/rerun_mapping_fixed/diagnostic_bins_v3.csv` (490 rows, 476 finite). Silently produced wrong plots this session via a stale plotter default.
- Quarantined RAPGAP aggregates `RAD 5,293,265 nb^-1`, `NONRAD 3,397,124 nb^-1`, and the anomalous `C_rad ~ 1.405`. **RESOLVED:** the NONRAD value belonged to a different 520-chunk Britzger set; the mismatched pairing gave `L_rad/L_nonrad = 1.5581606677` where the correct ratio is `1.168998`. This retires blocker `P-RAPGAP`.
- Older DJANGOH candidates: `full_crad_v1`, `full_crad_v2`, the 2026-09-21 candidate, the run3 truth-v2 product (h+ `0.6770210`, h- `0.8509564`; omitted the `IsGtr!=0` gate), and the V0/IsGtr candidate (h+ `1.14063313`, h- `1.14017023`).

## Blocker status

- `P-CRAD-SPARSE`: **CLOSED** — 490/490 finite both generators.
- `P-CRAD-LUMI`: **DOWNGRADED** — both generators traced to `oomclumi` at `DIAGNOSTIC_MANIFEST_LEVEL`. Remaining for raw-generator certification: reconcile `oomclumi` per-chunk sums against MCDB/generator-log records for ReqIDs `8947-8954` and the Britzger NONRAD set, including a duplicate/overlap audit.
- `P-RAPGAP`: **CLOSED** — see resolution above.

## Remaining open work

1. Applying `C_rad` to the central release — gated on the separate release/approval workflow.
2. The 7-column comparison plots — deferred for styling. `sidis_highQ2/plots/radiative/crad_v4_djangoh_vs_rapgap_7col_2026-09-30/plot_crad_djangoh_vs_rapgap_fLumi_7col.py` now has all CLI values hardcoded as defaults and runs bare (`python3 plot_crad_djangoh_vs_rapgap_fLumi_7col.py`, add `--variant wide`). Plots are correct as of final regeneration but **have NOT been visually reviewed**.
