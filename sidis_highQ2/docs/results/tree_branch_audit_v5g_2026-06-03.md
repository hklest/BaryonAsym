# v5g sidistree branch audit — rev6_v2 / AREA 210-bin pipeline

_Generated_: `2026-06-03T15:12:13Z`  (`build_audit_markdown.py` + `audit_tree_branches_v5g.py`)

**Goal**: identify which branches in the v5g `sidistree_*.root` output are unused
by the current rev6_v2 / TUnfold AREA 210-bin pipeline and quantify the file-size
saving available before launching systematic-production passes.

**Mode**: read-only inspection of three representative files using `uproot.compressed_bytes`
and `uncompressed_bytes` per branch. No ROOT files were modified.

---

## 1. Consumer scripts (authoritative)

The classification below is built from the verbatim branch lists in two scripts:

- `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix_v2.py`
- `scripts/analysis/build_data_side_systematics.py`

Downstream rev6_v2 / 210-bin TUnfold consumers operate on the **merged migration ROOT**
and bg-subtracted JSONs, never on `sidistree_*.root` directly. Old pertrack / v3final / IBU
diagnostic scripts and the lowpt-refined branch are not treated as production requirements.

### CT_BRANCHES (`ChargedTot`, build_migration_th2_per_charge_NWfix_v2.py)

```
Pt, Eta, Px, Py, Pz, Charge, NHitDedx, RadLength, DcaPrime, IsFromPrimary, IsCentralTrk, TrackType, scatEX, scatEY, scatEZ, scatEE, BeamElecE, BeamProtP, Q2e, Q2es, Yes, Xes, Empz, PVz, weight1, weight2, RunNumber, EventNumber, IsMatched, IsMatchedElectron, IsMatchedHadron, IsMatchedConvOrDalitz, IsMatchedV0Daughter, xF_SIDIS_es, GenPt, GenEta, GenPhi, GenP, GenCharge, GenPDG, zGen, xFGen, Q2_es_gen, Y_es_gen, x_es_gen, GenElecE, GenElecPhi, GenElecTheta
```

### CT_OPT

```
NucliaWeight, GenMatchIdx
```

### CTG_BRANCHES (`ChargedTotGen`)

```
GenIdx, GenCharge, GenPDG, GenPt, GenEta, GenPhi, GenElecE, GenElecPx, GenElecPy, GenElecPz, GenBeamElecE, GenBeamProtP, Q2esGen, x_es_gen, Y_es_gen, Q2eGen, YeGen, YesGen, zGen, IsV0DaughterGen, weight1, weight2, RunNumber, EventNumber, xFGen, GenEmpz
```

### DATA_BRANCHES (`ChargedTot`, build_data_side_systematics.py)

```
Pt, Eta, Px, Py, Pz, Charge, NHitDedx, RadLength, DcaPrime, IsFromPrimary, IsCentralTrk, TrackType, scatEX, scatEY, scatEZ, scatEE, BeamElecE, BeamProtP, Q2es, Yes, Xes, Empz, PVz, xF_SIDIS_es
```

---

## 2. Sample files

| label | path | on-disk |
|---|---|---:|
| `mc_rapgap31` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root` | 60.84 MB |
| `data_ep0304` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/ep0304/sidistree_ep0304_d03_0000.root` | 103.28 KB |
| `mc_djangoh14` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/djangoh14/7072/sidistree_djangoh14_7072_em06_0000.root` | 45.25 MB |

## 3. Per-tree branch counts and entry counts

| sample | tree | n_branches | n_entries |
|---|---|---:|---:|
| mc_rapgap31 | `ChargedTot` | 101 | 258,537 |
| mc_rapgap31 | `ChargedTotGen` | 60 | 517,853 |
| mc_rapgap31 | `DisTot` | 49 | 24,032 |
| data_ep0304 | `ChargedTot` | 101 | 743 |
| data_ep0304 | `DisTot` | 49 | 76 |
| mc_djangoh14 | `ChargedTot` | 101 | 187,087 |
| mc_djangoh14 | `ChargedTotGen` | 60 | 397,484 |
| mc_djangoh14 | `DisTot` | 49 | 17,989 |

## 4. Per-file totals and projected savings (compressed)

| sample | on-disk | trees total comp | trees total uncomp | drop-candidate comp | savings |
|---|---:|---:|---:|---:|---:|
| `mc_rapgap31` | 60.84 MB | 60.75 MB | 215.00 MB | 28.41 MB | **46.8%** |
| `data_ep0304` | 103.28 KB | 89.04 KB | 317.15 KB | 48.12 KB | **54.0%** |
| `mc_djangoh14` | 45.25 MB | 45.19 MB | 160.62 MB | 21.14 MB | **46.8%** |

> The data file is unrepresentative on absolute scale (it is a tiny 743-track
> chunk used purely as a schema sample). The percentage savings are nonetheless
> consistent with the MC files.

## 5. Per-classification byte budget (mc_rapgap31, representative)

| classification | n_branches | compressed | uncompressed |
|---|---:|---:|---:|
| `required_mc_only` | 49 | 19.31 MB | 69.78 MB |
| `legacy_or_pre_rev6_only` | 33 | 16.11 MB | 36.09 MB |
| `unused_safe_to_drop` | 67 | 12.30 MB | 47.34 MB |
| `required_current_pipeline` | 27 | 8.96 MB | 26.19 MB |
| `diagnostic_keep_optional` | 34 | 4.07 MB | 35.60 MB |

## 6. Top drop candidates (ranked by mc_rapgap31 compressed bytes)

Branches classified as either `unused_safe_to_drop` or `legacy_or_pre_rev6_only`.

| # | tree | branch | classification | rapgap_c | djangoh_c | data_c |
|---:|---|---|---|---:|---:|---:|
| 1 | `ChargedTotGen` | `ystar_es_gen` | `legacy_or_pre_rev6_only` | 1.83 MB | 1.41 MB | 0 B |
| 2 | `ChargedTotGen` | `ystarGen` | `legacy_or_pre_rev6_only` | 1.83 MB | 1.41 MB | 0 B |
| 3 | `ChargedTotGen` | `yBreitGen` | `legacy_or_pre_rev6_only` | 1.80 MB | 1.38 MB | 0 B |
| 4 | `ChargedTotGen` | `GenP` | `unused_safe_to_drop` | 1.80 MB | 1.38 MB | 0 B |
| 5 | `ChargedTotGen` | `ylab` | `unused_safe_to_drop` | 1.78 MB | 1.36 MB | 0 B |
| 6 | `ChargedTot` | `Phi` | `unused_safe_to_drop` | 938.15 KB | 678.95 KB | 2.82 KB |
| 7 | `ChargedTot` | `xF_SIDIS` | `legacy_or_pre_rev6_only` | 931.72 KB | 674.47 KB | 2.79 KB |
| 8 | `ChargedTot` | `yBreit_es` | `legacy_or_pre_rev6_only` | 929.43 KB | 672.29 KB | 2.74 KB |
| 9 | `ChargedTot` | `ylab` | `unused_safe_to_drop` | 928.96 KB | 671.75 KB | 2.76 KB |
| 10 | `ChargedTot` | `z_SIDIS` | `legacy_or_pre_rev6_only` | 928.90 KB | 672.34 KB | 2.78 KB |
| 11 | `ChargedTot` | `yBreit` | `legacy_or_pre_rev6_only` | 928.71 KB | 671.83 KB | 2.73 KB |
| 12 | `ChargedTot` | `z_SIDIS_es` | `legacy_or_pre_rev6_only` | 919.78 KB | 665.43 KB | 2.71 KB |
| 13 | `ChargedTot` | `P` | `unused_safe_to_drop` | 919.57 KB | 665.60 KB | 2.74 KB |
| 14 | `ChargedTot` | `yBreitGen` | `legacy_or_pre_rev6_only` | 918.61 KB | 664.70 KB | 135 B |
| 15 | `ChargedTot` | `Theta` | `unused_safe_to_drop` | 908.89 KB | 657.78 KB | 2.72 KB |
| 16 | `ChargedTot` | `ystar` | `legacy_or_pre_rev6_only` | 905.96 KB | 656.18 KB | 2.71 KB |
| 17 | `ChargedTot` | `ystar_es` | `legacy_or_pre_rev6_only` | 897.28 KB | 649.54 KB | 2.65 KB |
| 18 | `ChargedTot` | `ystar_es_gen` | `legacy_or_pre_rev6_only` | 889.15 KB | 643.82 KB | 138 B |
| 19 | `ChargedTot` | `ystarGen` | `legacy_or_pre_rev6_only` | 889.04 KB | 643.73 KB | 134 B |
| 20 | `ChargedTotGen` | `inFiducialCharged` | `unused_safe_to_drop` | 214.22 KB | 162.60 KB | 0 B |
| 21 | `ChargedTot` | `L1L2L3ac` | `legacy_or_pre_rev6_only` | 170.86 KB | 130.22 KB | 446 B |
| 22 | `ChargedTot` | `L1L2L3rw` | `legacy_or_pre_rev6_only` | 170.86 KB | 130.22 KB | 639 B |
| 23 | `ChargedTot` | `xGen` | `unused_safe_to_drop` | 132.78 KB | 99.51 KB | 123 B |
| 24 | `ChargedTot` | `YsGen` | `unused_safe_to_drop` | 131.65 KB | 98.68 KB | 124 B |
| 25 | `ChargedTot` | `Q2sGen` | `unused_safe_to_drop` | 131.59 KB | 98.56 KB | 125 B |
| 26 | `ChargedTotGen` | `XdaGen` | `legacy_or_pre_rev6_only` | 128.79 KB | 96.82 KB | 0 B |
| 27 | `ChargedTotGen` | `XesGen` | `unused_safe_to_drop` | 128.77 KB | 96.81 KB | 0 B |
| 28 | `ChargedTotGen` | `XhGen` | `unused_safe_to_drop` | 128.73 KB | 96.75 KB | 0 B |
| 29 | `ChargedTotGen` | `XeGen` | `unused_safe_to_drop` | 128.67 KB | 96.75 KB | 0 B |
| 30 | `ChargedTotGen` | `xGen` | `unused_safe_to_drop` | 128.62 KB | 96.69 KB | 0 B |
| 31 | `ChargedTotGen` | `Q2_es_gen` | `unused_safe_to_drop` | 127.65 KB | 96.00 KB | 0 B |
| 32 | `ChargedTotGen` | `YdaGen` | `legacy_or_pre_rev6_only` | 127.54 KB | 95.95 KB | 0 B |
| 33 | `ChargedTotGen` | `Q2daGen` | `legacy_or_pre_rev6_only` | 127.51 KB | 95.90 KB | 0 B |
| 34 | `ChargedTotGen` | `YsGen` | `unused_safe_to_drop` | 127.48 KB | 95.87 KB | 0 B |
| 35 | `ChargedTotGen` | `Q2sGen` | `unused_safe_to_drop` | 127.43 KB | 95.82 KB | 0 B |
| 36 | `ChargedTotGen` | `Q2hGen` | `unused_safe_to_drop` | 127.42 KB | 95.82 KB | 0 B |
| 37 | `ChargedTotGen` | `YhGen` | `unused_safe_to_drop` | 127.16 KB | 95.65 KB | 0 B |
| 38 | `ChargedTot` | `Xh` | `unused_safe_to_drop` | 120.52 KB | 90.25 KB | 528 B |
| 39 | `ChargedTot` | `Xda` | `legacy_or_pre_rev6_only` | 120.33 KB | 90.04 KB | 528 B |
| 40 | `ChargedTot` | `xBj` | `unused_safe_to_drop` | 120.29 KB | 90.09 KB | 513 B |

### Headline: top 10 branches to drop

1. `ChargedTotGen/ystar_es_gen` — legacy_or_pre_rev6_only (1.83 MB compressed)
2. `ChargedTotGen/ystarGen` — legacy_or_pre_rev6_only (1.83 MB compressed)
3. `ChargedTotGen/yBreitGen` — legacy_or_pre_rev6_only (1.80 MB compressed)
4. `ChargedTotGen/GenP` — unused_safe_to_drop (1.80 MB compressed)
5. `ChargedTotGen/ylab` — unused_safe_to_drop (1.78 MB compressed)
6. `ChargedTot/Phi` — unused_safe_to_drop (938.15 KB compressed)
7. `ChargedTot/xF_SIDIS` — legacy_or_pre_rev6_only (931.72 KB compressed)
8. `ChargedTot/yBreit_es` — legacy_or_pre_rev6_only (929.43 KB compressed)
9. `ChargedTot/ylab` — unused_safe_to_drop (928.96 KB compressed)
10. `ChargedTot/z_SIDIS` — legacy_or_pre_rev6_only (928.90 KB compressed)

## 7. Full per-tree branch listings with classification

### mc_rapgap31 — `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root`

#### `ChargedTot` (101 branches, 258,537 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `DcaPrime` | `required_current_pipeline` | 957.89 KB | 1012.28 KB |
| `DcaPrimeSinTheta` | `diagnostic_keep_optional` | 955.69 KB | 1012.52 KB |
| `Py` | `required_current_pipeline` | 952.56 KB | 1012.03 KB |
| `Px` | `required_current_pipeline` | 952.52 KB | 1012.03 KB |
| `Pz` | `required_current_pipeline` | 948.80 KB | 1012.03 KB |
| `Phi` | `unused_safe_to_drop` | 938.15 KB | 1012.06 KB |
| `xF_SIDIS` | `legacy_or_pre_rev6_only` | 931.72 KB | 1012.28 KB |
| `yBreit_es` | `legacy_or_pre_rev6_only` | 929.43 KB | 1012.31 KB |
| `ylab` | `unused_safe_to_drop` | 928.96 KB | 1012.09 KB |
| `z_SIDIS` | `legacy_or_pre_rev6_only` | 928.90 KB | 1012.25 KB |
| `yBreit` | `legacy_or_pre_rev6_only` | 928.71 KB | 1012.15 KB |
| `Eta` | `required_current_pipeline` | 928.59 KB | 1012.06 KB |
| `xF_SIDIS_es` | `required_current_pipeline` | 922.68 KB | 1012.37 KB |
| `Pt` | `required_current_pipeline` | 922.66 KB | 1012.03 KB |
| `z_SIDIS_es` | `legacy_or_pre_rev6_only` | 919.78 KB | 1012.34 KB |
| `P` | `unused_safe_to_drop` | 919.57 KB | 1012.01 KB |
| `GenPhi` | `required_mc_only` | 919.14 KB | 1012.15 KB |
| `yBreitGen` | `legacy_or_pre_rev6_only` | 918.61 KB | 1012.31 KB |
| `xFGen` | `required_mc_only` | 912.93 KB | 1012.12 KB |
| `GenEta` | `required_mc_only` | 910.33 KB | 1012.15 KB |
| `zGen` | `required_mc_only` | 910.21 KB | 1012.09 KB |
| `Theta` | `unused_safe_to_drop` | 908.89 KB | 1012.12 KB |
| `ystar` | `legacy_or_pre_rev6_only` | 905.96 KB | 1012.12 KB |
| `GenPt` | `required_mc_only` | 903.58 KB | 1012.12 KB |
| `GenP` | `required_mc_only` | 901.75 KB | 1012.09 KB |
| `ystar_es` | `legacy_or_pre_rev6_only` | 897.28 KB | 1012.28 KB |
| `ystar_es_gen` | `legacy_or_pre_rev6_only` | 889.15 KB | 1012.40 KB |
| `ystarGen` | `legacy_or_pre_rev6_only` | 889.04 KB | 1012.28 KB |
| `RadLength` | `required_current_pipeline` | 879.52 KB | 1012.31 KB |
| `NucliaWeight` | `required_mc_only` | 781.02 KB | 1012.40 KB |
| `GenMotherPDG` | `diagnostic_keep_optional` | 350.63 KB | 1012.40 KB |
| `NHitCJC` | `diagnostic_keep_optional` | 289.36 KB | 1012.25 KB |
| `NHitCJC1` | `diagnostic_keep_optional` | 250.91 KB | 1012.28 KB |
| `NHitDedx` | `required_current_pipeline` | 225.38 KB | 506.22 KB |
| `GenPDG` | `required_mc_only` | 217.27 KB | 1012.15 KB |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 170.86 KB | 3.95 MB |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 170.86 KB | 3.95 MB |
| `GenCharge` | `required_mc_only` | 134.13 KB | 1012.31 KB |
| `xGen` | `unused_safe_to_drop` | 132.78 KB | 1012.09 KB |
| `Y_es_gen` | `required_mc_only` | 132.17 KB | 1012.28 KB |
| `YsGen` | `unused_safe_to_drop` | 131.65 KB | 1012.12 KB |
| `Q2sGen` | `unused_safe_to_drop` | 131.59 KB | 1012.15 KB |
| `Q2_es_gen` | `required_mc_only` | 131.57 KB | 1012.31 KB |
| `x_es_gen` | `required_mc_only` | 131.44 KB | 1012.28 KB |
| `GenElecPhi` | `required_mc_only` | 122.76 KB | 1012.34 KB |
| `PVz` | `required_current_pipeline` | 122.53 KB | 1012.06 KB |
| `scatEY` | `required_current_pipeline` | 122.11 KB | 1012.15 KB |
| `scatEX` | `required_current_pipeline` | 122.09 KB | 1012.15 KB |
| `Xh` | `unused_safe_to_drop` | 120.52 KB | 1012.03 KB |
| `Xda` | `legacy_or_pre_rev6_only` | 120.33 KB | 1012.06 KB |
| `xBj` | `unused_safe_to_drop` | 120.29 KB | 1012.06 KB |
| `Xes` | `required_current_pipeline` | 120.24 KB | 1012.06 KB |
| `Xe` | `unused_safe_to_drop` | 120.20 KB | 1012.03 KB |
| `Q2h` | `unused_safe_to_drop` | 120.08 KB | 1012.06 KB |
| `Q2da` | `legacy_or_pre_rev6_only` | 119.10 KB | 1012.09 KB |
| `Yes` | `required_current_pipeline` | 119.09 KB | 1012.06 KB |
| `scatEZ` | `required_current_pipeline` | 119.09 KB | 1012.15 KB |
| `Q2s` | `unused_safe_to_drop` | 119.05 KB | 1012.06 KB |
| `Yda` | `legacy_or_pre_rev6_only` | 119.03 KB | 1012.06 KB |
| `Ys` | `unused_safe_to_drop` | 119.02 KB | 1012.03 KB |
| `Ye` | `unused_safe_to_drop` | 119.01 KB | 1012.03 KB |
| `Q2es` | `required_current_pipeline` | 118.96 KB | 1012.09 KB |
| `Q2e` | `required_current_pipeline` | 118.93 KB | 1012.06 KB |
| `GenElecTheta` | `required_mc_only` | 118.92 KB | 1012.40 KB |
| `Yh` | `unused_safe_to_drop` | 118.86 KB | 1012.03 KB |
| `Ws` | `unused_safe_to_drop` | 118.10 KB | 1012.03 KB |
| `GenElecE` | `required_mc_only` | 117.88 KB | 1012.28 KB |
| `scatEE` | `required_current_pipeline` | 117.73 KB | 1012.15 KB |
| `Empz` | `required_current_pipeline` | 116.86 KB | 1012.09 KB |
| `Charge` | `required_current_pipeline` | 109.92 KB | 1012.15 KB |
| `VetoAncestorPDG` | `diagnostic_keep_optional` | 93.33 KB | 1012.49 KB |
| `EventNumber` | `required_current_pipeline` | 92.02 KB | 1012.37 KB |
| `weight1` | `required_mc_only` | 87.88 KB | 1012.25 KB |
| `IsGtr` | `unused_safe_to_drop` | 80.45 KB | 1012.12 KB |
| `GenEnPhoton` | `diagnostic_keep_optional` | 71.44 KB | 1012.37 KB |
| `IsStr` | `unused_safe_to_drop` | 41.27 KB | 253.09 KB |
| `IsMatchedHadron` | `required_mc_only` | 40.27 KB | 253.25 KB |
| `IsMatchedConvOrDalitz` | `required_mc_only` | 35.39 KB | 253.30 KB |
| `IsMatchedElectron` | `required_mc_only` | 31.18 KB | 253.27 KB |
| `ParticleSource` | `diagnostic_keep_optional` | 30.33 KB | 1012.46 KB |
| `inFiducialChargedMatch` | `unused_safe_to_drop` | 29.83 KB | 1012.69 KB |
| `IsMatched` | `required_mc_only` | 27.00 KB | 1012.31 KB |
| `IsCentralTrk` | `required_current_pipeline` | 26.33 KB | 1012.40 KB |
| `TrackType` | `required_current_pipeline` | 25.78 KB | 1012.31 KB |
| `IsCombinedTrk` | `diagnostic_keep_optional` | 25.03 KB | 1012.43 KB |
| `IsMatchedV0Daughter` | `required_mc_only` | 23.26 KB | 253.29 KB |
| `MatchTier` | `diagnostic_keep_optional` | 16.74 KB | 253.20 KB |
| `IsMatchedMuon` | `diagnostic_keep_optional` | 12.51 KB | 253.23 KB |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 11.66 KB | 1012.40 KB |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 11.66 KB | 1012.40 KB |
| `MatchAngle` | `diagnostic_keep_optional` | 10.74 KB | 1012.34 KB |
| `IsMatchedDIFMuon` | `diagnostic_keep_optional` | 9.84 KB | 253.26 KB |
| `RunNumber` | `required_current_pipeline` | 9.60 KB | 1012.31 KB |
| `GenMatchIdx` | `required_mc_only` | 9.27 KB | 1012.37 KB |
| `StartRadius` | `diagnostic_keep_optional` | 8.49 KB | 1012.37 KB |
| `IsFromPrimary` | `required_current_pipeline` | 8.47 KB | 1012.43 KB |
| `EndRadius` | `diagnostic_keep_optional` | 8.43 KB | 1012.31 KB |
| `BeamElecE` | `required_current_pipeline` | 8.43 KB | 1012.31 KB |
| `BeamProtP` | `required_current_pipeline` | 8.43 KB | 1012.31 KB |
| `weight2` | `required_mc_only` | 8.35 KB | 1012.25 KB |
| `IsMatchedK0LDaughter` | `diagnostic_keep_optional` | 2.95 KB | 253.29 KB |

#### `ChargedTotGen` (60 branches, 517,853 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `xFGen` | `required_mc_only` | 1.88 MB | 1.98 MB |
| `zGen` | `required_mc_only` | 1.86 MB | 1.98 MB |
| `ystar_es_gen` | `legacy_or_pre_rev6_only` | 1.83 MB | 1.98 MB |
| `ystarGen` | `legacy_or_pre_rev6_only` | 1.83 MB | 1.98 MB |
| `GenPhi` | `required_mc_only` | 1.81 MB | 1.98 MB |
| `yBreitGen` | `legacy_or_pre_rev6_only` | 1.80 MB | 1.98 MB |
| `GenP` | `unused_safe_to_drop` | 1.80 MB | 1.98 MB |
| `ylab` | `unused_safe_to_drop` | 1.78 MB | 1.98 MB |
| `GenEta` | `required_mc_only` | 1.77 MB | 1.98 MB |
| `GenPt` | `required_mc_only` | 1.77 MB | 1.98 MB |
| `GenIdx` | `required_mc_only` | 611.96 KB | 1.98 MB |
| `GenMotherPDG` | `diagnostic_keep_optional` | 514.44 KB | 1.98 MB |
| `GenPDG` | `required_mc_only` | 338.84 KB | 1.98 MB |
| `GenCharge` | `required_mc_only` | 220.57 KB | 1.98 MB |
| `inFiducialCharged` | `unused_safe_to_drop` | 214.22 KB | 1.98 MB |
| `GenElecPhi` | `diagnostic_keep_optional` | 131.29 KB | 1.98 MB |
| `GenVtxZ` | `diagnostic_keep_optional` | 131.14 KB | 1.98 MB |
| `GenScatEX` | `diagnostic_keep_optional` | 130.82 KB | 1.98 MB |
| `GenElecPx` | `required_mc_only` | 130.82 KB | 1.98 MB |
| `GenScatEY` | `diagnostic_keep_optional` | 130.82 KB | 1.98 MB |
| `GenElecPy` | `required_mc_only` | 130.82 KB | 1.98 MB |
| `x_es_gen` | `required_mc_only` | 129.46 KB | 1.98 MB |
| `XdaGen` | `legacy_or_pre_rev6_only` | 128.79 KB | 1.98 MB |
| `XesGen` | `unused_safe_to_drop` | 128.77 KB | 1.98 MB |
| `XhGen` | `unused_safe_to_drop` | 128.73 KB | 1.98 MB |
| `XeGen` | `unused_safe_to_drop` | 128.67 KB | 1.98 MB |
| `xGen` | `unused_safe_to_drop` | 128.62 KB | 1.98 MB |
| `Y_es_gen` | `required_mc_only` | 128.18 KB | 1.98 MB |
| `GenScatEZ` | `diagnostic_keep_optional` | 127.87 KB | 1.98 MB |
| `GenElecPz` | `required_mc_only` | 127.87 KB | 1.98 MB |
| `Q2_es_gen` | `unused_safe_to_drop` | 127.65 KB | 1.98 MB |
| `YesGen` | `required_mc_only` | 127.63 KB | 1.98 MB |
| `YdaGen` | `legacy_or_pre_rev6_only` | 127.54 KB | 1.98 MB |
| `Q2daGen` | `legacy_or_pre_rev6_only` | 127.51 KB | 1.98 MB |
| `YsGen` | `unused_safe_to_drop` | 127.48 KB | 1.98 MB |
| `Q2esGen` | `required_mc_only` | 127.47 KB | 1.98 MB |
| `YeGen` | `required_mc_only` | 127.45 KB | 1.98 MB |
| `Q2sGen` | `unused_safe_to_drop` | 127.43 KB | 1.98 MB |
| `Q2hGen` | `unused_safe_to_drop` | 127.42 KB | 1.98 MB |
| `Q2eGen` | `required_mc_only` | 127.41 KB | 1.98 MB |
| `YhGen` | `unused_safe_to_drop` | 127.16 KB | 1.98 MB |
| `GenElecTheta` | `diagnostic_keep_optional` | 126.96 KB | 1.98 MB |
| `GenScatEE` | `diagnostic_keep_optional` | 126.22 KB | 1.98 MB |
| `GenElecE` | `required_mc_only` | 126.16 KB | 1.98 MB |
| `GenEmpz` | `required_mc_only` | 119.35 KB | 1.98 MB |
| `EventNumber` | `required_mc_only` | 102.47 KB | 1.98 MB |
| `weight1` | `required_mc_only` | 100.22 KB | 1.98 MB |
| `GenEnPhoton` | `diagnostic_keep_optional` | 82.53 KB | 1.98 MB |
| `VetoAncestorPDG` | `diagnostic_keep_optional` | 43.30 KB | 1.98 MB |
| `ParticleSource` | `diagnostic_keep_optional` | 26.22 KB | 1.98 MB |
| `IsConvOrDalitzGen` | `diagnostic_keep_optional` | 25.09 KB | 507.26 KB |
| `RunNumber` | `required_mc_only` | 18.92 KB | 1.98 MB |
| `GenBeamElecE` | `required_mc_only` | 17.89 KB | 1.98 MB |
| `GenBeamProtP` | `required_mc_only` | 17.89 KB | 1.98 MB |
| `weight2` | `required_mc_only` | 17.50 KB | 1.98 MB |
| `IsGtr` | `unused_safe_to_drop` | 17.25 KB | 1.98 MB |
| `IsV0DaughterGen` | `required_mc_only` | 4.26 KB | 507.23 KB |
| `IsK0LDaughter` | `diagnostic_keep_optional` | 4.23 KB | 507.19 KB |
| `IsDIFMuon` | `diagnostic_keep_optional` | 4.16 KB | 507.13 KB |
| `IsStr` | `unused_safe_to_drop` | 4.09 KB | 507.06 KB |

#### `DisTot` (49 branches, 24,032 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `VtxZ` | `diagnostic_keep_optional` | 87.52 KB | 94.16 KB |
| `scatEX` | `diagnostic_keep_optional` | 86.92 KB | 94.17 KB |
| `scatEY` | `diagnostic_keep_optional` | 86.92 KB | 94.17 KB |
| `Xh` | `unused_safe_to_drop` | 85.60 KB | 94.15 KB |
| `XhGen` | `unused_safe_to_drop` | 85.34 KB | 94.16 KB |
| `XdaGen` | `legacy_or_pre_rev6_only` | 85.34 KB | 94.17 KB |
| `XesGen` | `unused_safe_to_drop` | 85.33 KB | 94.17 KB |
| `Xda` | `legacy_or_pre_rev6_only` | 85.32 KB | 94.16 KB |
| `Xs_gen` | `unused_safe_to_drop` | 85.30 KB | 94.17 KB |
| `XeGen` | `unused_safe_to_drop` | 85.29 KB | 94.16 KB |
| `Xes` | `unused_safe_to_drop` | 85.25 KB | 94.16 KB |
| `Xe` | `unused_safe_to_drop` | 85.25 KB | 94.15 KB |
| `Q2h` | `unused_safe_to_drop` | 85.11 KB | 94.16 KB |
| `Xs` | `unused_safe_to_drop` | 85.10 KB | 94.15 KB |
| `Q2da` | `legacy_or_pre_rev6_only` | 84.14 KB | 94.16 KB |
| `Q2daGen` | `legacy_or_pre_rev6_only` | 84.08 KB | 94.17 KB |
| `YesGen` | `unused_safe_to_drop` | 84.06 KB | 94.17 KB |
| `YsGen` | `unused_safe_to_drop` | 84.05 KB | 94.16 KB |
| `YdaGen` | `legacy_or_pre_rev6_only` | 84.04 KB | 94.17 KB |
| `Q2hGen` | `unused_safe_to_drop` | 84.03 KB | 94.17 KB |
| `Yes` | `unused_safe_to_drop` | 84.02 KB | 94.16 KB |
| `Q2esGen` | `unused_safe_to_drop` | 84.01 KB | 94.17 KB |
| `Q2sGen` | `unused_safe_to_drop` | 84.01 KB | 94.17 KB |
| `Q2eGen` | `unused_safe_to_drop` | 84.00 KB | 94.17 KB |
| `YeGen` | `unused_safe_to_drop` | 83.99 KB | 94.16 KB |
| `Q2es` | `unused_safe_to_drop` | 83.98 KB | 94.16 KB |
| `Q2e` | `unused_safe_to_drop` | 83.98 KB | 94.16 KB |
| `Ye` | `unused_safe_to_drop` | 83.95 KB | 94.15 KB |
| `Yh` | `unused_safe_to_drop` | 83.94 KB | 94.15 KB |
| `Yda` | `legacy_or_pre_rev6_only` | 83.93 KB | 94.16 KB |
| `Q2s` | `unused_safe_to_drop` | 83.85 KB | 94.16 KB |
| `scatEZ` | `diagnostic_keep_optional` | 83.84 KB | 94.17 KB |
| `Ys` | `unused_safe_to_drop` | 83.78 KB | 94.15 KB |
| `YhGen` | `unused_safe_to_drop` | 82.13 KB | 94.16 KB |
| `scatEE` | `diagnostic_keep_optional` | 82.05 KB | 94.17 KB |
| `Empz` | `unused_safe_to_drop` | 79.12 KB | 94.16 KB |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 67.29 KB | 376.48 KB |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 67.29 KB | 376.48 KB |
| `weight1` | `unused_safe_to_drop` | 54.33 KB | 94.17 KB |
| `EventNumber` | `unused_safe_to_drop` | 46.36 KB | 94.19 KB |
| `nChargedReco` | `unused_safe_to_drop` | 24.29 KB | 94.19 KB |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 2.67 KB | 94.19 KB |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 2.67 KB | 94.19 KB |
| `RunNumber` | `unused_safe_to_drop` | 1.89 KB | 94.18 KB |
| `BeamElecE` | `unused_safe_to_drop` | 921 B | 94.18 KB |
| `BeamProtP` | `unused_safe_to_drop` | 920 B | 94.18 KB |
| `weight2` | `unused_safe_to_drop` | 909 B | 94.17 KB |
| `nChargedGen` | `unused_safe_to_drop` | 859 B | 94.19 KB |
| `isMC` | `unused_safe_to_drop` | 211 B | 23.54 KB |

### data_ep0304 — `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/ep0304/sidistree_ep0304_d03_0000.root`

#### `ChargedTot` (101 branches, 743 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `DcaPrimeSinTheta` | `diagnostic_keep_optional` | 2.88 KB | 2.99 KB |
| `DcaPrime` | `required_current_pipeline` | 2.88 KB | 2.98 KB |
| `Px` | `required_current_pipeline` | 2.85 KB | 2.98 KB |
| `Py` | `required_current_pipeline` | 2.85 KB | 2.98 KB |
| `Pz` | `required_current_pipeline` | 2.83 KB | 2.98 KB |
| `Phi` | `unused_safe_to_drop` | 2.82 KB | 2.98 KB |
| `xF_SIDIS` | `legacy_or_pre_rev6_only` | 2.79 KB | 2.98 KB |
| `z_SIDIS` | `legacy_or_pre_rev6_only` | 2.78 KB | 2.98 KB |
| `Eta` | `required_current_pipeline` | 2.77 KB | 2.98 KB |
| `ylab` | `unused_safe_to_drop` | 2.76 KB | 2.98 KB |
| `Pt` | `required_current_pipeline` | 2.75 KB | 2.98 KB |
| `P` | `unused_safe_to_drop` | 2.74 KB | 2.97 KB |
| `yBreit_es` | `legacy_or_pre_rev6_only` | 2.74 KB | 2.98 KB |
| `yBreit` | `legacy_or_pre_rev6_only` | 2.73 KB | 2.98 KB |
| `Theta` | `unused_safe_to_drop` | 2.72 KB | 2.98 KB |
| `xF_SIDIS_es` | `required_current_pipeline` | 2.72 KB | 2.98 KB |
| `ystar` | `legacy_or_pre_rev6_only` | 2.71 KB | 2.98 KB |
| `z_SIDIS_es` | `legacy_or_pre_rev6_only` | 2.71 KB | 2.98 KB |
| `RadLength` | `required_current_pipeline` | 2.66 KB | 2.98 KB |
| `ystar_es` | `legacy_or_pre_rev6_only` | 2.65 KB | 2.98 KB |
| `NHitCJC` | `diagnostic_keep_optional` | 987 B | 2.98 KB |
| `NHitCJC1` | `diagnostic_keep_optional` | 864 B | 2.98 KB |
| `NHitDedx` | `required_current_pipeline` | 855 B | 1.53 KB |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 639 B | 11.69 KB |
| `scatEY` | `required_current_pipeline` | 530 B | 2.98 KB |
| `Xda` | `legacy_or_pre_rev6_only` | 528 B | 2.98 KB |
| `Xh` | `unused_safe_to_drop` | 528 B | 2.98 KB |
| `Q2h` | `unused_safe_to_drop` | 526 B | 2.98 KB |
| `Ye` | `unused_safe_to_drop` | 522 B | 2.98 KB |
| `scatEX` | `required_current_pipeline` | 522 B | 2.98 KB |
| `Yda` | `legacy_or_pre_rev6_only` | 521 B | 2.98 KB |
| `Yes` | `required_current_pipeline` | 521 B | 2.98 KB |
| `Xes` | `required_current_pipeline` | 521 B | 2.98 KB |
| `Yh` | `unused_safe_to_drop` | 521 B | 2.98 KB |
| `Xe` | `unused_safe_to_drop` | 519 B | 2.98 KB |
| `xBj` | `unused_safe_to_drop` | 513 B | 2.98 KB |
| `scatEE` | `required_current_pipeline` | 513 B | 2.98 KB |
| `scatEZ` | `required_current_pipeline` | 510 B | 2.98 KB |
| `Q2s` | `unused_safe_to_drop` | 508 B | 2.98 KB |
| `Ys` | `unused_safe_to_drop` | 508 B | 2.98 KB |
| `Empz` | `required_current_pipeline` | 505 B | 2.98 KB |
| `Q2da` | `legacy_or_pre_rev6_only` | 504 B | 2.98 KB |
| `Q2es` | `required_current_pipeline` | 504 B | 2.98 KB |
| `Q2e` | `required_current_pipeline` | 503 B | 2.98 KB |
| `Ws` | `unused_safe_to_drop` | 501 B | 2.98 KB |
| `PVz` | `required_current_pipeline` | 498 B | 2.98 KB |
| `EventNumber` | `required_current_pipeline` | 472 B | 2.98 KB |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 446 B | 11.69 KB |
| `Charge` | `required_current_pipeline` | 440 B | 2.98 KB |
| `RunNumber` | `required_current_pipeline` | 233 B | 2.98 KB |
| `BeamProtP` | `required_current_pipeline` | 163 B | 2.98 KB |
| `IsCentralTrk` | `required_current_pipeline` | 150 B | 2.99 KB |
| `IsCombinedTrk` | `diagnostic_keep_optional` | 146 B | 2.99 KB |
| `TrackType` | `required_current_pipeline` | 145 B | 2.98 KB |
| `inFiducialChargedMatch` | `unused_safe_to_drop` | 141 B | 3.00 KB |
| `BeamElecE` | `required_current_pipeline` | 139 B | 2.98 KB |
| `NucliaWeight` | `required_mc_only` | 138 B | 2.99 KB |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 138 B | 2.99 KB |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 138 B | 2.99 KB |
| `ystar_es_gen` | `legacy_or_pre_rev6_only` | 138 B | 2.99 KB |
| `GenElecTheta` | `required_mc_only` | 138 B | 2.99 KB |
| `StartRadius` | `diagnostic_keep_optional` | 137 B | 2.98 KB |
| `IsFromPrimary` | `required_current_pipeline` | 136 B | 2.99 KB |
| `MatchAngle` | `diagnostic_keep_optional` | 136 B | 2.98 KB |
| `GenElecPhi` | `required_mc_only` | 136 B | 2.98 KB |
| `EndRadius` | `diagnostic_keep_optional` | 135 B | 2.98 KB |
| `yBreitGen` | `legacy_or_pre_rev6_only` | 135 B | 2.98 KB |
| `Q2_es_gen` | `required_mc_only` | 135 B | 2.98 KB |
| `VetoAncestorPDG` | `diagnostic_keep_optional` | 134 B | 2.99 KB |
| `ystarGen` | `legacy_or_pre_rev6_only` | 134 B | 2.98 KB |
| `Y_es_gen` | `required_mc_only` | 134 B | 2.98 KB |
| `x_es_gen` | `required_mc_only` | 134 B | 2.98 KB |
| `GenElecE` | `required_mc_only` | 134 B | 2.98 KB |
| `weight1` | `required_mc_only` | 133 B | 2.98 KB |
| `weight2` | `required_mc_only` | 133 B | 2.98 KB |
| `ParticleSource` | `diagnostic_keep_optional` | 133 B | 2.99 KB |
| `GenEta` | `required_mc_only` | 132 B | 2.98 KB |
| `GenPhi` | `required_mc_only` | 132 B | 2.98 KB |
| `GenMotherPDG` | `diagnostic_keep_optional` | 131 B | 2.99 KB |
| `GenPt` | `required_mc_only` | 131 B | 2.98 KB |
| `xFGen` | `required_mc_only` | 131 B | 2.98 KB |
| `GenMatchIdx` | `required_mc_only` | 130 B | 2.98 KB |
| `GenP` | `required_mc_only` | 130 B | 2.98 KB |
| `zGen` | `required_mc_only` | 130 B | 2.98 KB |
| `GenEnPhoton` | `diagnostic_keep_optional` | 130 B | 2.98 KB |
| `IsMatched` | `required_mc_only` | 128 B | 2.98 KB |
| `GenCharge` | `required_mc_only` | 128 B | 2.98 KB |
| `GenPDG` | `required_mc_only` | 125 B | 2.98 KB |
| `Q2sGen` | `unused_safe_to_drop` | 125 B | 2.98 KB |
| `IsGtr` | `unused_safe_to_drop` | 124 B | 2.98 KB |
| `YsGen` | `unused_safe_to_drop` | 124 B | 2.98 KB |
| `xGen` | `unused_safe_to_drop` | 123 B | 2.98 KB |
| `IsMatchedConvOrDalitz` | `required_mc_only` | 120 B | 837 B |
| `IsMatchedK0LDaughter` | `diagnostic_keep_optional` | 119 B | 836 B |
| `IsMatchedV0Daughter` | `required_mc_only` | 118 B | 835 B |
| `IsMatchedElectron` | `required_mc_only` | 116 B | 833 B |
| `IsMatchedDIFMuon` | `diagnostic_keep_optional` | 115 B | 832 B |
| `IsMatchedHadron` | `required_mc_only` | 114 B | 831 B |
| `IsMatchedMuon` | `diagnostic_keep_optional` | 112 B | 829 B |
| `MatchTier` | `diagnostic_keep_optional` | 108 B | 825 B |
| `IsStr` | `unused_safe_to_drop` | 104 B | 821 B |

#### `DisTot` (49 branches, 76 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `scatEX` | `diagnostic_keep_optional` | 379 B | 379 B |
| `scatEY` | `diagnostic_keep_optional` | 379 B | 379 B |
| `scatEZ` | `diagnostic_keep_optional` | 379 B | 379 B |
| `scatEE` | `diagnostic_keep_optional` | 379 B | 379 B |
| `Empz` | `unused_safe_to_drop` | 377 B | 377 B |
| `VtxZ` | `diagnostic_keep_optional` | 377 B | 377 B |
| `Q2es` | `unused_safe_to_drop` | 377 B | 377 B |
| `Q2da` | `legacy_or_pre_rev6_only` | 377 B | 377 B |
| `Q2e` | `unused_safe_to_drop` | 376 B | 376 B |
| `Q2s` | `unused_safe_to_drop` | 376 B | 376 B |
| `Yes` | `unused_safe_to_drop` | 376 B | 376 B |
| `Xes` | `unused_safe_to_drop` | 376 B | 376 B |
| `Yda` | `legacy_or_pre_rev6_only` | 376 B | 376 B |
| `Xda` | `legacy_or_pre_rev6_only` | 376 B | 376 B |
| `Q2h` | `unused_safe_to_drop` | 376 B | 376 B |
| `Ye` | `unused_safe_to_drop` | 375 B | 375 B |
| `Xe` | `unused_safe_to_drop` | 375 B | 375 B |
| `Ys` | `unused_safe_to_drop` | 375 B | 375 B |
| `Xs` | `unused_safe_to_drop` | 375 B | 375 B |
| `Yh` | `unused_safe_to_drop` | 375 B | 375 B |
| `Xh` | `unused_safe_to_drop` | 375 B | 375 B |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 371 B | 1.26 KB |
| `EventNumber` | `unused_safe_to_drop` | 352 B | 384 B |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 251 B | 1.26 KB |
| `nChargedReco` | `unused_safe_to_drop` | 212 B | 385 B |
| `RunNumber` | `unused_safe_to_drop` | 182 B | 382 B |
| `BeamProtP` | `unused_safe_to_drop` | 132 B | 382 B |
| `BeamElecE` | `unused_safe_to_drop` | 109 B | 382 B |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 107 B | 385 B |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 107 B | 385 B |
| `nChargedGen` | `unused_safe_to_drop` | 103 B | 384 B |
| `weight1` | `unused_safe_to_drop` | 102 B | 380 B |
| `weight2` | `unused_safe_to_drop` | 102 B | 380 B |
| `Q2esGen` | `unused_safe_to_drop` | 99 B | 380 B |
| `Q2daGen` | `legacy_or_pre_rev6_only` | 99 B | 380 B |
| `Q2eGen` | `unused_safe_to_drop` | 98 B | 379 B |
| `Q2sGen` | `unused_safe_to_drop` | 98 B | 379 B |
| `Xs_gen` | `unused_safe_to_drop` | 98 B | 379 B |
| `YesGen` | `unused_safe_to_drop` | 98 B | 379 B |
| `XesGen` | `unused_safe_to_drop` | 98 B | 379 B |
| `YdaGen` | `legacy_or_pre_rev6_only` | 98 B | 379 B |
| `XdaGen` | `legacy_or_pre_rev6_only` | 98 B | 379 B |
| `Q2hGen` | `unused_safe_to_drop` | 98 B | 379 B |
| `YeGen` | `unused_safe_to_drop` | 97 B | 378 B |
| `XeGen` | `unused_safe_to_drop` | 97 B | 378 B |
| `YsGen` | `unused_safe_to_drop` | 97 B | 378 B |
| `YhGen` | `unused_safe_to_drop` | 97 B | 378 B |
| `XhGen` | `unused_safe_to_drop` | 97 B | 378 B |
| `isMC` | `unused_safe_to_drop` | 94 B | 149 B |

### mc_djangoh14 — `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/djangoh14/7072/sidistree_djangoh14_7072_em06_0000.root`

#### `ChargedTot` (101 branches, 187,087 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `DcaPrime` | `required_current_pipeline` | 693.41 KB | 732.71 KB |
| `DcaPrimeSinTheta` | `diagnostic_keep_optional` | 691.90 KB | 732.89 KB |
| `Px` | `required_current_pipeline` | 689.74 KB | 732.57 KB |
| `Py` | `required_current_pipeline` | 689.59 KB | 732.57 KB |
| `Pz` | `required_current_pipeline` | 686.33 KB | 732.57 KB |
| `Phi` | `unused_safe_to_drop` | 678.95 KB | 732.59 KB |
| `xF_SIDIS` | `legacy_or_pre_rev6_only` | 674.47 KB | 732.71 KB |
| `z_SIDIS` | `legacy_or_pre_rev6_only` | 672.34 KB | 732.68 KB |
| `yBreit_es` | `legacy_or_pre_rev6_only` | 672.29 KB | 732.73 KB |
| `yBreit` | `legacy_or_pre_rev6_only` | 671.83 KB | 732.66 KB |
| `ylab` | `unused_safe_to_drop` | 671.75 KB | 732.61 KB |
| `Eta` | `required_current_pipeline` | 671.49 KB | 732.59 KB |
| `Pt` | `required_current_pipeline` | 667.76 KB | 732.57 KB |
| `xF_SIDIS_es` | `required_current_pipeline` | 667.63 KB | 732.78 KB |
| `P` | `unused_safe_to_drop` | 665.60 KB | 732.54 KB |
| `z_SIDIS_es` | `legacy_or_pre_rev6_only` | 665.43 KB | 732.75 KB |
| `GenPhi` | `required_mc_only` | 665.08 KB | 732.66 KB |
| `yBreitGen` | `legacy_or_pre_rev6_only` | 664.70 KB | 732.73 KB |
| `xFGen` | `required_mc_only` | 660.68 KB | 732.64 KB |
| `zGen` | `required_mc_only` | 658.84 KB | 732.61 KB |
| `GenEta` | `required_mc_only` | 658.17 KB | 732.66 KB |
| `Theta` | `unused_safe_to_drop` | 657.78 KB | 732.64 KB |
| `ystar` | `legacy_or_pre_rev6_only` | 656.18 KB | 732.64 KB |
| `GenPt` | `required_mc_only` | 653.79 KB | 732.64 KB |
| `GenP` | `required_mc_only` | 652.62 KB | 732.61 KB |
| `ystar_es` | `legacy_or_pre_rev6_only` | 649.54 KB | 732.71 KB |
| `ystar_es_gen` | `legacy_or_pre_rev6_only` | 643.82 KB | 732.80 KB |
| `ystarGen` | `legacy_or_pre_rev6_only` | 643.73 KB | 732.71 KB |
| `RadLength` | `required_current_pipeline` | 637.02 KB | 732.73 KB |
| `NucliaWeight` | `required_mc_only` | 563.92 KB | 732.80 KB |
| `GenMotherPDG` | `diagnostic_keep_optional` | 254.80 KB | 732.80 KB |
| `NHitCJC` | `diagnostic_keep_optional` | 210.61 KB | 732.68 KB |
| `NHitCJC1` | `diagnostic_keep_optional` | 181.65 KB | 732.71 KB |
| `NHitDedx` | `required_current_pipeline` | 163.97 KB | 366.35 KB |
| `GenPDG` | `required_mc_only` | 159.32 KB | 732.66 KB |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 130.22 KB | 2.86 MB |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 130.22 KB | 2.86 MB |
| `xGen` | `unused_safe_to_drop` | 99.51 KB | 732.61 KB |
| `Y_es_gen` | `required_mc_only` | 99.01 KB | 732.71 KB |
| `YsGen` | `unused_safe_to_drop` | 98.68 KB | 732.64 KB |
| `GenCharge` | `required_mc_only` | 98.62 KB | 732.73 KB |
| `Q2sGen` | `unused_safe_to_drop` | 98.56 KB | 732.66 KB |
| `Q2_es_gen` | `required_mc_only` | 98.50 KB | 732.73 KB |
| `x_es_gen` | `required_mc_only` | 98.42 KB | 732.71 KB |
| `GenElecPhi` | `required_mc_only` | 91.80 KB | 732.75 KB |
| `PVz` | `required_current_pipeline` | 91.75 KB | 732.59 KB |
| `scatEY` | `required_current_pipeline` | 91.46 KB | 732.66 KB |
| `scatEX` | `required_current_pipeline` | 91.43 KB | 732.66 KB |
| `Xh` | `unused_safe_to_drop` | 90.25 KB | 732.57 KB |
| `xBj` | `unused_safe_to_drop` | 90.09 KB | 732.59 KB |
| `Xda` | `legacy_or_pre_rev6_only` | 90.04 KB | 732.59 KB |
| `Xes` | `required_current_pipeline` | 90.04 KB | 732.59 KB |
| `Q2h` | `unused_safe_to_drop` | 89.96 KB | 732.59 KB |
| `Xe` | `unused_safe_to_drop` | 89.95 KB | 732.57 KB |
| `Yes` | `required_current_pipeline` | 89.18 KB | 732.59 KB |
| `Ye` | `unused_safe_to_drop` | 89.18 KB | 732.57 KB |
| `Ys` | `unused_safe_to_drop` | 89.16 KB | 732.57 KB |
| `Q2s` | `unused_safe_to_drop` | 89.14 KB | 732.59 KB |
| `Q2da` | `legacy_or_pre_rev6_only` | 89.14 KB | 732.61 KB |
| `Yda` | `legacy_or_pre_rev6_only` | 89.08 KB | 732.59 KB |
| `scatEZ` | `required_current_pipeline` | 89.03 KB | 732.66 KB |
| `Q2es` | `required_current_pipeline` | 89.03 KB | 732.61 KB |
| `Yh` | `unused_safe_to_drop` | 89.01 KB | 732.57 KB |
| `Q2e` | `required_current_pipeline` | 89.00 KB | 732.59 KB |
| `GenElecTheta` | `required_mc_only` | 88.86 KB | 732.80 KB |
| `Ws` | `unused_safe_to_drop` | 88.42 KB | 732.57 KB |
| `GenElecE` | `required_mc_only` | 88.16 KB | 732.71 KB |
| `scatEE` | `required_current_pipeline` | 88.07 KB | 732.66 KB |
| `Empz` | `required_current_pipeline` | 87.33 KB | 732.61 KB |
| `Charge` | `required_current_pipeline` | 79.94 KB | 732.66 KB |
| `VetoAncestorPDG` | `diagnostic_keep_optional` | 69.30 KB | 732.87 KB |
| `EventNumber` | `required_current_pipeline` | 68.66 KB | 732.78 KB |
| `weight1` | `required_mc_only` | 62.39 KB | 732.68 KB |
| `IsGtr` | `unused_safe_to_drop` | 59.75 KB | 732.64 KB |
| `GenEnPhoton` | `diagnostic_keep_optional` | 53.63 KB | 732.78 KB |
| `IsStr` | `unused_safe_to_drop` | 30.19 KB | 183.16 KB |
| `IsMatchedHadron` | `required_mc_only` | 29.31 KB | 183.22 KB |
| `IsMatchedConvOrDalitz` | `required_mc_only` | 25.94 KB | 183.25 KB |
| `ParticleSource` | `diagnostic_keep_optional` | 23.19 KB | 732.85 KB |
| `inFiducialChargedMatch` | `unused_safe_to_drop` | 22.96 KB | 733.04 KB |
| `IsMatchedElectron` | `required_mc_only` | 22.84 KB | 183.23 KB |
| `IsMatched` | `required_mc_only` | 20.48 KB | 732.73 KB |
| `IsCentralTrk` | `required_current_pipeline` | 20.06 KB | 732.80 KB |
| `TrackType` | `required_current_pipeline` | 19.58 KB | 732.73 KB |
| `IsCombinedTrk` | `diagnostic_keep_optional` | 19.12 KB | 732.82 KB |
| `IsMatchedV0Daughter` | `required_mc_only` | 16.98 KB | 183.24 KB |
| `MatchTier` | `diagnostic_keep_optional` | 12.55 KB | 183.18 KB |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 8.62 KB | 732.80 KB |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 8.62 KB | 732.80 KB |
| `IsMatchedMuon` | `diagnostic_keep_optional` | 8.51 KB | 183.21 KB |
| `MatchAngle` | `diagnostic_keep_optional` | 7.86 KB | 732.75 KB |
| `RunNumber` | `required_current_pipeline` | 7.30 KB | 732.73 KB |
| `GenMatchIdx` | `required_mc_only` | 6.88 KB | 732.78 KB |
| `IsMatchedDIFMuon` | `diagnostic_keep_optional` | 6.65 KB | 183.22 KB |
| `StartRadius` | `diagnostic_keep_optional` | 6.43 KB | 732.78 KB |
| `IsFromPrimary` | `required_current_pipeline` | 6.41 KB | 732.82 KB |
| `EndRadius` | `diagnostic_keep_optional` | 6.39 KB | 732.73 KB |
| `BeamElecE` | `required_current_pipeline` | 6.39 KB | 732.73 KB |
| `BeamProtP` | `required_current_pipeline` | 6.39 KB | 732.73 KB |
| `weight2` | `required_mc_only` | 6.32 KB | 732.68 KB |
| `IsMatchedK0LDaughter` | `diagnostic_keep_optional` | 2.07 KB | 183.25 KB |

#### `ChargedTotGen` (60 branches, 397,484 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `xFGen` | `required_mc_only` | 1.44 MB | 1.52 MB |
| `zGen` | `required_mc_only` | 1.43 MB | 1.52 MB |
| `ystar_es_gen` | `legacy_or_pre_rev6_only` | 1.41 MB | 1.52 MB |
| `ystarGen` | `legacy_or_pre_rev6_only` | 1.41 MB | 1.52 MB |
| `GenPhi` | `required_mc_only` | 1.38 MB | 1.52 MB |
| `GenP` | `unused_safe_to_drop` | 1.38 MB | 1.52 MB |
| `yBreitGen` | `legacy_or_pre_rev6_only` | 1.38 MB | 1.52 MB |
| `GenEta` | `required_mc_only` | 1.36 MB | 1.52 MB |
| `ylab` | `unused_safe_to_drop` | 1.36 MB | 1.52 MB |
| `GenPt` | `required_mc_only` | 1.35 MB | 1.52 MB |
| `GenIdx` | `required_mc_only` | 493.07 KB | 1.52 MB |
| `GenMotherPDG` | `diagnostic_keep_optional` | 407.38 KB | 1.52 MB |
| `GenPDG` | `required_mc_only` | 262.37 KB | 1.52 MB |
| `GenCharge` | `required_mc_only` | 169.69 KB | 1.52 MB |
| `inFiducialCharged` | `unused_safe_to_drop` | 162.60 KB | 1.52 MB |
| `GenElecPhi` | `diagnostic_keep_optional` | 98.67 KB | 1.52 MB |
| `GenVtxZ` | `diagnostic_keep_optional` | 98.58 KB | 1.52 MB |
| `GenScatEX` | `diagnostic_keep_optional` | 98.37 KB | 1.52 MB |
| `GenElecPx` | `required_mc_only` | 98.37 KB | 1.52 MB |
| `GenScatEY` | `diagnostic_keep_optional` | 98.37 KB | 1.52 MB |
| `GenElecPy` | `required_mc_only` | 98.37 KB | 1.52 MB |
| `x_es_gen` | `required_mc_only` | 97.30 KB | 1.52 MB |
| `XdaGen` | `legacy_or_pre_rev6_only` | 96.82 KB | 1.52 MB |
| `XesGen` | `unused_safe_to_drop` | 96.81 KB | 1.52 MB |
| `XhGen` | `unused_safe_to_drop` | 96.75 KB | 1.52 MB |
| `XeGen` | `unused_safe_to_drop` | 96.75 KB | 1.52 MB |
| `xGen` | `unused_safe_to_drop` | 96.69 KB | 1.52 MB |
| `Y_es_gen` | `required_mc_only` | 96.46 KB | 1.52 MB |
| `Q2_es_gen` | `unused_safe_to_drop` | 96.00 KB | 1.52 MB |
| `GenScatEZ` | `diagnostic_keep_optional` | 95.99 KB | 1.52 MB |
| `GenElecPz` | `required_mc_only` | 95.99 KB | 1.52 MB |
| `YdaGen` | `legacy_or_pre_rev6_only` | 95.95 KB | 1.52 MB |
| `YesGen` | `required_mc_only` | 95.93 KB | 1.52 MB |
| `Q2daGen` | `legacy_or_pre_rev6_only` | 95.90 KB | 1.52 MB |
| `YsGen` | `unused_safe_to_drop` | 95.87 KB | 1.52 MB |
| `YeGen` | `required_mc_only` | 95.87 KB | 1.52 MB |
| `Q2sGen` | `unused_safe_to_drop` | 95.82 KB | 1.52 MB |
| `Q2hGen` | `unused_safe_to_drop` | 95.82 KB | 1.52 MB |
| `Q2esGen` | `required_mc_only` | 95.79 KB | 1.52 MB |
| `Q2eGen` | `required_mc_only` | 95.74 KB | 1.52 MB |
| `YhGen` | `unused_safe_to_drop` | 95.65 KB | 1.52 MB |
| `GenElecTheta` | `diagnostic_keep_optional` | 95.45 KB | 1.52 MB |
| `GenScatEE` | `diagnostic_keep_optional` | 94.88 KB | 1.52 MB |
| `GenElecE` | `required_mc_only` | 94.83 KB | 1.52 MB |
| `GenEmpz` | `required_mc_only` | 94.38 KB | 1.52 MB |
| `EventNumber` | `required_mc_only` | 77.17 KB | 1.52 MB |
| `weight1` | `required_mc_only` | 72.49 KB | 1.52 MB |
| `GenEnPhoton` | `diagnostic_keep_optional` | 62.67 KB | 1.52 MB |
| `VetoAncestorPDG` | `diagnostic_keep_optional` | 34.41 KB | 1.52 MB |
| `ParticleSource` | `diagnostic_keep_optional` | 20.61 KB | 1.52 MB |
| `IsConvOrDalitzGen` | `diagnostic_keep_optional` | 20.41 KB | 389.35 KB |
| `RunNumber` | `required_mc_only` | 14.58 KB | 1.52 MB |
| `GenBeamElecE` | `required_mc_only` | 13.74 KB | 1.52 MB |
| `GenBeamProtP` | `required_mc_only` | 13.74 KB | 1.52 MB |
| `weight2` | `required_mc_only` | 13.45 KB | 1.52 MB |
| `IsGtr` | `unused_safe_to_drop` | 13.25 KB | 1.52 MB |
| `IsV0DaughterGen` | `required_mc_only` | 3.26 KB | 389.32 KB |
| `IsK0LDaughter` | `diagnostic_keep_optional` | 3.24 KB | 389.30 KB |
| `IsDIFMuon` | `diagnostic_keep_optional` | 3.19 KB | 389.25 KB |
| `IsStr` | `unused_safe_to_drop` | 3.14 KB | 389.20 KB |

#### `DisTot` (49 branches, 17,989 entries)

| branch | classification | compressed | uncompressed |
|---|---|---:|---:|
| `VtxZ` | `diagnostic_keep_optional` | 65.50 KB | 70.48 KB |
| `scatEY` | `diagnostic_keep_optional` | 65.08 KB | 70.49 KB |
| `scatEX` | `diagnostic_keep_optional` | 65.07 KB | 70.49 KB |
| `Xh` | `unused_safe_to_drop` | 64.09 KB | 70.48 KB |
| `XdaGen` | `legacy_or_pre_rev6_only` | 63.88 KB | 70.49 KB |
| `Xs_gen` | `unused_safe_to_drop` | 63.87 KB | 70.49 KB |
| `XhGen` | `unused_safe_to_drop` | 63.86 KB | 70.49 KB |
| `XesGen` | `unused_safe_to_drop` | 63.86 KB | 70.49 KB |
| `Xda` | `legacy_or_pre_rev6_only` | 63.81 KB | 70.48 KB |
| `XeGen` | `unused_safe_to_drop` | 63.81 KB | 70.49 KB |
| `Xes` | `unused_safe_to_drop` | 63.78 KB | 70.48 KB |
| `Xe` | `unused_safe_to_drop` | 63.76 KB | 70.48 KB |
| `Q2h` | `unused_safe_to_drop` | 63.71 KB | 70.48 KB |
| `Xs` | `unused_safe_to_drop` | 63.68 KB | 70.48 KB |
| `YsGen` | `unused_safe_to_drop` | 62.93 KB | 70.49 KB |
| `YdaGen` | `legacy_or_pre_rev6_only` | 62.92 KB | 70.49 KB |
| `YesGen` | `unused_safe_to_drop` | 62.92 KB | 70.49 KB |
| `Q2da` | `legacy_or_pre_rev6_only` | 62.90 KB | 70.48 KB |
| `Q2hGen` | `unused_safe_to_drop` | 62.89 KB | 70.49 KB |
| `Q2sGen` | `unused_safe_to_drop` | 62.88 KB | 70.49 KB |
| `Q2daGen` | `legacy_or_pre_rev6_only` | 62.88 KB | 70.49 KB |
| `Yes` | `unused_safe_to_drop` | 62.88 KB | 70.48 KB |
| `Ye` | `unused_safe_to_drop` | 62.86 KB | 70.48 KB |
| `YeGen` | `unused_safe_to_drop` | 62.86 KB | 70.49 KB |
| `Q2esGen` | `unused_safe_to_drop` | 62.85 KB | 70.49 KB |
| `Q2eGen` | `unused_safe_to_drop` | 62.85 KB | 70.49 KB |
| `Yh` | `unused_safe_to_drop` | 62.84 KB | 70.48 KB |
| `Q2es` | `unused_safe_to_drop` | 62.81 KB | 70.48 KB |
| `Q2e` | `unused_safe_to_drop` | 62.80 KB | 70.48 KB |
| `Yda` | `legacy_or_pre_rev6_only` | 62.78 KB | 70.48 KB |
| `Q2s` | `unused_safe_to_drop` | 62.72 KB | 70.48 KB |
| `Ys` | `unused_safe_to_drop` | 62.70 KB | 70.48 KB |
| `scatEZ` | `diagnostic_keep_optional` | 62.56 KB | 70.49 KB |
| `YhGen` | `unused_safe_to_drop` | 61.52 KB | 70.49 KB |
| `scatEE` | `diagnostic_keep_optional` | 61.32 KB | 70.49 KB |
| `Empz` | `unused_safe_to_drop` | 59.21 KB | 70.48 KB |
| `L1L2L3ac` | `legacy_or_pre_rev6_only` | 52.37 KB | 281.83 KB |
| `L1L2L3rw` | `legacy_or_pre_rev6_only` | 52.37 KB | 281.83 KB |
| `weight1` | `unused_safe_to_drop` | 38.19 KB | 70.49 KB |
| `EventNumber` | `unused_safe_to_drop` | 34.39 KB | 70.50 KB |
| `nChargedReco` | `unused_safe_to_drop` | 18.04 KB | 70.51 KB |
| `TrigWeightAC` | `legacy_or_pre_rev6_only` | 1.94 KB | 70.51 KB |
| `TrigWeightRW` | `legacy_or_pre_rev6_only` | 1.94 KB | 70.51 KB |
| `RunNumber` | `unused_safe_to_drop` | 1.45 KB | 70.50 KB |
| `BeamElecE` | `unused_safe_to_drop` | 701 B | 70.50 KB |
| `BeamProtP` | `unused_safe_to_drop` | 701 B | 70.50 KB |
| `weight2` | `unused_safe_to_drop` | 692 B | 70.49 KB |
| `nChargedGen` | `unused_safe_to_drop` | 652 B | 70.50 KB |
| `isMC` | `unused_safe_to_drop` | 184 B | 17.64 KB |

---

## 8. Recommended reduced schema for systematic-production passes

These schemas are derived from sections 1, 5, 6. The 'minimal' lines list
the branches the rev6_v2/AREA 210-bin pipeline must read; the 'diagnostic'
line lists branches that should be kept around for foreseeable future
detector/systematics work (track-quality, electron-quality, vertex,
background-veto), per Section 9.

### 8a. Minimal DATA schema (`ChargedTot` only; `ChargedTotGen` absent on data)

```
Pt, Eta, Px, Py, Pz, Charge, NHitDedx, RadLength, DcaPrime, IsFromPrimary, IsCentralTrk, TrackType, scatEX, scatEY, scatEZ, scatEE, BeamElecE, BeamProtP, Q2e, Q2es, Yes, Xes, Empz, PVz, RunNumber, EventNumber, xF_SIDIS_es
```

Justification: every branch in `DATA_BRANCHES` + the matching `CT_BRANCHES` entries
for `Q2e` (used as a cross-check on Q2es) and the event/run numbers. `weight1`/`weight2`
are not stored on data.

### 8b. Minimal MC schema (`ChargedTot` reco-side; required)

```
BeamElecE, BeamProtP, Charge, DcaPrime, Empz, Eta, EventNumber, GenCharge, GenElecE, GenElecPhi, GenElecTheta, GenEta, GenMatchIdx, GenP, GenPDG, GenPhi, GenPt, IsCentralTrk, IsFromPrimary, IsMatched, IsMatchedConvOrDalitz, IsMatchedElectron, IsMatchedHadron, IsMatchedV0Daughter, NHitDedx, NucliaWeight, PVz, Pt, Px, Py, Pz, Q2_es_gen, Q2e, Q2es, RadLength, RunNumber, TrackType, Xes, Y_es_gen, Yes, scatEE, scatEX, scatEY, scatEZ, weight1, weight2, xFGen, xF_SIDIS_es, x_es_gen, zGen
```

### 8c. Minimal MC schema (`ChargedTotGen`)

```
EventNumber, GenBeamElecE, GenBeamProtP, GenCharge, GenElecE, GenElecPx, GenElecPy, GenElecPz, GenEmpz, GenEta, GenIdx, GenPDG, GenPhi, GenPt, IsV0DaughterGen, Q2eGen, Q2esGen, RunNumber, Y_es_gen, YeGen, YesGen, weight1, weight2, xFGen, x_es_gen, zGen
```

Note that `Q2_es_gen` is duplicated by `Q2esGen` on this tree — keep only `Q2esGen`
on this tree. The inlined `Q2_es_gen` is retained on `ChargedTot` (different role).

### 8d. Optional diagnostic schema (KEEP for future systematics)

```
NHitCJC, NHitCJC1, DcaPrimeSinTheta, StartRadius, EndRadius, MatchTier, MatchAngle, GenMotherPDG, VetoAncestorPDG, IsMatchedMuon, IsMatchedDIFMuon, IsMatchedK0LDaughter, ParticleSource, GenEnPhoton, IsCombinedTrk, IsConvOrDalitzGen, IsK0LDaughter, IsDIFMuon, GenVtxZ, GenScatEX, GenScatEY, GenScatEZ, GenScatEE, GenElecPhi, GenElecTheta
```

### 8e. `DisTot` tree — drop entire tree from production output

`DisTot` is not consumed by the rev6_v2 / 210-bin AREA pipeline at all. All 49 of its
branches duplicate event-level quantities that are also stored per-track on `ChargedTot`.
If a slim event-level handle is desired for fast event-loop QA, write only:
`RunNumber, EventNumber, VtxZ, scatEX, scatEY, scatEZ, scatEE, BeamElecE, Empz, isMC` —
but the recommendation is **drop `DisTot` entirely** from systematic-production output.

---

## 9. Warnings — branches NOT to drop now even if currently unused

The following branches are flagged `diagnostic_keep_optional` because they are needed
for foreseeable detector/analysis systematics that are not yet implemented but are part
of the planned Phase-2 systematic budget. Do not propose dropping them without first
re-checking the systematic plan.

- **Electron quality / four-vector**: `GenElecPhi`, `GenElecTheta`, `GenScatE*` — needed
  for any electron-energy / theta posthoc shift that wants a gen-truth comparison.
- **Track quality**: `NHitCJC`, `NHitCJC1`, `DcaPrimeSinTheta`, `StartRadius`, `EndRadius`
  — needed for the planned tracking-efficiency systematic (variation of NHit/DCA cuts).
- **Gen-matching diagnostics**: `MatchTier`, `MatchAngle`, `GenMotherPDG`,
  `VetoAncestorPDG`, `ParticleSource` — needed for the secondary/decay-contamination
  systematic and for an eventual matching-stability check.
- **Background-veto flags**: `IsMatchedMuon`, `IsMatchedDIFMuon`,
  `IsMatchedK0LDaughter`, `IsConvOrDalitzGen` (and the gen-side `IsK0LDaughter`,
  `IsDIFMuon`) — needed for charged-particle background subtraction systematics.
- **Radiative-photon truth**: `GenEnPhoton` — needed for the radiative-correction
  systematic (already required as scalar on ChargedTot is consumed by the radiative-safe
  gen-electron-energy correction baked into v5g).
- **Vertex diagnostics on `ChargedTotGen`**: `GenVtxZ` — needed if vertex-z cut systematic
  is varied on the gen side.

These are NOT recommended for removal in the reduced schema.

---

## 10. Headline numbers and recommended next action

| sample | on-disk | reachable savings (compressed) |
|---|---:|---:|
| `mc_rapgap31` | 60.84 MB | 28.41 MB (**46.8%**) |
| `data_ep0304` | 103.28 KB | 48.12 KB (**54.0%**) |
| `mc_djangoh14` | 45.25 MB | 21.14 MB (**46.8%**) |

- Projected savings on a representative MC file are **~46.8 %** of the on-disk
  compressed sidistree size. For an ~80,000-job systematic production at the current
  v5g per-file size, this translates into roughly half of the previously-required DUST
  footprint per systematic variant.
- The savings are dominated by per-track `ystar*`, `yBreit*`, `ylab`, `Phi`, `P`,
  `Theta`, `z_SIDIS*`, `xF_SIDIS` on `ChargedTot`, the equivalent gen quantities on
  `ChargedTotGen`, and the entire `DisTot` tree.
- All of these branches are either derivable on-the-fly from `Px/Py/Pz`/scatE*/
  generator scalar inputs, or are pre-rev6 SIDIS-frame quantities superseded by the
  CM-frame recomputation in `build_migration_th2_per_charge_NWfix_v2.py`.

**Recommended next action**:

1. Have `h1-builder-engineer` cut a v5h Stage 2 builder that removes the
   `legacy_or_pre_rev6_only` branches above plus the explicit `unused_safe_to_drop`
   list, and drops the `DisTot` tree entirely.
2. Run a 1-file smoke test on rapgap, djangoh, ep0304 and re-run this audit; verify
   that the resulting compressed size shrinks by ~47 % and that
   `build_migration_th2_per_charge_NWfix_v2.py` and `build_data_side_systematics.py`
   still complete with identical reco-efficiency / counts on the smoke file.
3. Only after the smoke validation, kick the systematic-production pass with the
   trimmed schema.

---

_End of audit._  Generated by `audit_tree_branches_v5g.py` + `build_audit_markdown.py`.
