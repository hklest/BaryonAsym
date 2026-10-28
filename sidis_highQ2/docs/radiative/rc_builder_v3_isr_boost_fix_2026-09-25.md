# RC gen-only v3 ISR-beam-boost check — 2026-09-25

## Verdict

**REFUTED — no source, binary, steering, or ntuple changes made.**

Bounded one-file audit of the existing v2 RAD ep output:

- Input: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/rad_retained_v2_20260923/ntuples/ep/rad_ep_0642.root`
- Tree: `ChargedGenRC`; weighted with `weight1*weight2`; finite non-negative `pT_h` rows only.
- `GenEnPhoton < 0.5 GeV`: 2,100,409 rows, weighted `pT_h` p99 = 18.15285 GeV; weighted fraction `pT_h >= 8 GeV` = 0.02392404.
- `GenEnPhoton >= 0.5 GeV`: 716,264 rows, weighted `pT_h` p99 = 7.54779 GeV; weighted fraction `pT_h >= 8 GeV` = 0.00936680.
- All rows: weighted p99 = 15.58791 GeV; fraction `pT_h >= 8 GeV` = 0.01971459.

The high-`pT_h` tail is largest in the low-photon-energy category rather than being concentrated at high `GenEnPhoton`; therefore the requested ISR-effective-beam explanation is refuted by the prescribed test. Per instruction, no `RC_treebuilder_genonly_v3.C` was created and no v2 source/binary/ntuple was modified.

## Photon-energy bins

| `GenEnPhoton` [GeV] | rows | weighted `pT_h` p99 [GeV] | weighted fraction `pT_h >= 8 GeV` |
|---|---:|---:|---:|
| [0, 0.5) | 2,100,409 | 18.15285 | 0.02392404 |
| [0.5, 1) | 168,360 | 12.37900 | 0.01525530 |
| [1, 2) | 157,668 | 10.71403 | 0.01363877 |
| [2, 5) | 185,425 | 6.60217 | 0.00799149 |
| [5, 10) | 104,532 | 5.65600 | 0.00658753 |
| [10, 27.5) | 98,847 | 4.48900 | 0.00428641 |
| [27.5, inf) | 1,432 | 2.06400 | 0.00175263 |

## If later approved after a new root-cause test

The retained RAD v2 campaign consists of 1,373 files (731 ep and 642 em). Its historical Condor submit file is `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/condor/rad_retained_v2_20260923/rad_retained_v2.sub`, with a 10,800 s / 3 h runtime request and 2 GB memory per file. A v3 rebuild would require a distinct v3 queue, wrapper, output root, and binary identity guard; it must not reuse or overwrite the v2 paths.
