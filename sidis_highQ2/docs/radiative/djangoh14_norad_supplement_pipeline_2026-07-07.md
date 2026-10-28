# DJANGOH14 Non-Rad Supplement Pipeline — Validated Short-Path Critical Path

## Purpose

Supplemental DJANGOH14 non-rad MC for high-Q² charged-SIDIS `C_rad` corner-bin statistics.

Use the short root
`/data/dust/group/h1/klesthen/Djangoh14`
to avoid BOS/FPACK filename truncation.

## Validated Smoke Result

| Item | Validated value |
|---|---|
| Smoke run dir | `/data/dust/group/h1/klesthen/Djangoh14/runs/hk_0123_shortpath_smoke_20260707` |
| DJANGOH BOS output | `output/djangoh_out/HKhk_0123_shortpath_smoke_20260707.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.B.C0000000` |
| BOS size | `516312720 B` |
| LOG output | `output/djangoh_out/HKhk_0123_shortpath_smoke_20260707.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.LOG` |
| LOG size | `54276 B` |
| `conversion/ods.root` | `2954292 B`; tree `ODS` with `998` entries |
| `conversion/mods.root` | `2270364 B`; tree `MODS` with `998` entries |
| `conversion/hat.root` | `208693 B`; tree `HAT` with `998` entries |
| Treebuilder smoke output | `/data/dust/group/h1/klesthen/Djangoh14/runs/hk_0123_shortpath_smoke_20260707/treebuilder_smoke/hk_shortpath_genonly_smoke_h1el9_rerun1.root` |
| Treebuilder smoke size | `59296 B` |
| Treebuilder counts | `GenDIS=50`, `ChargedGenRC=1053` |

### Required verified treebuilder branches

`weight1`, `weight2`, `Q2sGen`, `YsGen`, `XsGen`, `GenPDG`, `GenCharge`, `Mother2`, `IsGtr`, `GenEta_lab`, `zGen`, `pT_h`, `xFGen`

### Optional verified branches present

`GenEmpz`, `ParticleSource`

## Critical Validated Requirements

| Area | Requirement |
|---|---|
| Run directories | DJANGOH will not overwrite existing files; always use fresh run/campaign dirs. |
| Steering paths | Use short relative BOS/LOG paths in steering; do not use long absolute paths. |
| Steering content | Comment active `MSTA(39)`, including spaced quoted form. |
| HK steering source | `/data/dust/group/h1/klesthen/BaryonAsym/Djangoh14/stdjan_nc_eminusp_norad_q2gt100_cteq6l_hk` |
| DJANGOH executable | `/data/dust/group/h1/britzger/norad_prod21/test_henry_2026/djangoh14test` |
| FPACK steering | Generate per-folder `fpack.steer` from the full template and include exact line `DBPAR FILE="/afs/desy.de/group/h1/ndb_read_only_dp" ORDERED` |
| Conversion executable | `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/bin/x86_64-el9-gcc14-opt/dst2all` |
| Treebuilder environment | Run treebuilder only after `source /etc/profile.d/modules.sh; source /afs/desy.de/group/h1/etc/functions.sh; desygroupset h1; h1el9` |
| Treebuilder CLI | Must use flagged CLI: `--mode mc --steer <steer> -o <out.root>`; no positional args. |

## Local Production Scripts Prepared

These production scripts were created locally under:
`/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/norad_djangoh_supplement/`

| Stage | Script |
|---|---|
| Stage 1 | `submit_stage1_djangoh_500x10k.sh` |
| Stage 2 | `submit_stage2_dst2all_from_stage1.sh` |
| Stage 3 | `submit_stage3_crad_treebuilder_from_stage2.sh` |

Validation status:

| Check | Status |
|---|---|
| `bash -n` | PASS |
| Local dry-run path | PASS |
| Full 500-job remote production | NOT YET RUN |

## Run Sequence After Deploying Scripts to NAF

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage1_djangoh_500x10k.sh --campaign hk_0123_500x10k_v1
scripts/submit_stage2_dst2all_from_stage1.sh --campaign hk_0123_500x10k_v1
scripts/submit_stage3_crad_treebuilder_from_stage2.sh --campaign hk_0123_500x10k_v1
```

### Useful runtime options

| Script scope | Options |
|---|---|
| Stage 1 / 2 / 3 | `--dry-run`, `--resume`, `--campaign` |
| Stage 1 | `--njobs`, `--nevents` |
| Stage 3 | `--max-events` |

## Current Status and Gates

| Item | Status |
|---|---|
| One-file smoke chain | Validated through post-treebuilder ROOT on one `1000`-event test |
| 500×10k production scripts | Implemented locally but not yet run remotely |
| Physics-use gate 1 | Normalization / effective-lumi provenance still needed |
| Physics-use gate 2 | Production manifests still needed |
| Physics-use gate 3 | `e+` counterpart still missing unless supplied |
| Physics-use gate 4 | Sparse-bin `C_rad` policy remains a separate release blocker |

## 2026-07-08 validated chain update

A newer validated full one-job Condor chain is documented in:
`djangoh14_norad_supplement_pipeline_2026-07-08.md`

Use the 2026-07-08 note as the operational starting point for future work. It records the validated script-driven Stage 1 -> Stage 2 -> Stage 3 chain, the exact smoke and production commands, per-job output locations, the confirmed one-job campaign `hk_0123_envpatch_submit1_20260708`, the in-progress production launch `hk_0123_500x10k_prod_20260708`, and the environment/bootstrap fixes that resolved the earlier failures.

In particular, the 2026-07-08 note captures these now-validated facts that are not reliably encoded here:
- Stage 1 is prepare-only unless `--submit` is passed.
- Stage 1 outputs must be checked in per-job `bos/` and `log/` directories.
- The maintained remote scripts under `/data/dust/group/h1/klesthen/Djangoh14/scripts/` are the canonical provenance.
- Stage 2 BOSINPUT patching must handle lowercase `file=`.
- Stage 2 and Stage 3 require the hardened env bootstrap that avoids early `desygroupset` / `OS` failures.
- Failed wrapper-copy resmokes are not canonical provenance.

## Scope Note

This note records only the earlier validated short-path critical path and early fixes for the DJANGOH14 non-rad supplement pipeline. For current operational guidance, use the superseding 2026-07-08 note. Failed trials are intentionally omitted.
