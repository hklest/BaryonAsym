# WP-A2 S2: diagnostic pass and hypothesis readings (derivation split)

Date 2026-10-27. Child plan `D/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md` (sha256 `c553d200…b0d`), S1 record
`s1_definition_audit.md` (sha256 `bb95183d…bd23`, readings R1-R8 bind). `D` =
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Every number below is from the **even**
(derivation) split; the odd split was filled in the same pass and has not been read.

## 1. Production and tests

- Condor cluster 5902113 (`D/condor/restart_2026-10-27/wp-a2/diag_wpa2/`, submit log `submit_log_s2.txt`, which quotes
  both pre-registration hashes): 35 jobs (17 files × {even, odd} with the `ChargedTot` E−p_z join, plus one identity
  job), all exit 0, no resubmission; 1856 s wall = 0.0215 Condor-days. Worker `D/scripts/restart_2026-10-27/wp-a2/n1_wpa2.py`
  (md5 at submission `7614c1a6a3e816fa41d62dc2391004c3`). 7091 peaked at 12208 of 12288 MiB.
- `fork_default_identity`: PASS. The default-flag partial for 9155 equals the WP-A `s1_split/partials/all/9155` partial
  exactly (0 differences in counts, hists, dcell_eta, my_tables, conservation, wpa); the only extra key is
  `wpa.eta35_eplug`, which `n1_wpa.py` gained after WP-A S1 ran (`identity_check_wpa2.py`).
- `split_disjoint_complete`: PASS (`s2_tests_wpa2.py`). Even/odd key overlap 0 in all 17 files; even + odd raw counts
  equal the WP-A split=all counts for every mask and D cell.
- `empz_join_coverage`: below 0.999 for 14 of 17 files, as the plan anticipates. Data 0.987-0.988, MC 0.996-0.9997.
  The unmatched events are zero-track events with no stored `Empz` (`D/docs/t2/empz_reconstructibility_2026-10-18.md` §6).
  Per the plan, H4 and the H3 E−p_z variants run on the matched subset.
- `edge_localisation_reproduced`: PASS. Baseline two-template fit, even split, 7071: DATA/fit = 1.229 in [3.15, 3.35)
  and 0.630 in [3.40, 3.55) in D0 (D1 1.196 / 0.660, D2 1.120 / 0.685), D/ndf = 1894/118 in D0.
- `h4_control_is_inclusive`: **FAIL** in D0 and D1. The ND share of the H4 sample under the 7071 cocktail is
  0.849 / 0.875 / 0.931 (D0 / D1 / D2; threshold 0.90). The H4 reading is kept with this caveat (§5).
- After S2, `n1_wpa2.py` gained one flag, `--eta-cut` (default 3.2), needed by the H1 correction (§3). With default
  flags the output is unchanged: the 9155 partial is identical to the original's.

Products: partials `D/data/restart_2026-10-27/wp-a2/s2_diag/partials/<label>/<split>/partial.json`; analysis
`D/data/restart_2026-10-27/wp-a2/s2_diag/analysis/{s2_analysis_even.json, s2_tests.json}` from `s2_analyze_wpa2.py`
(whose docstring fixes the operational criteria, written before any output was read) and `s2_tests_wpa2.py`; plots
`D/plots/restart_2026-10-27/wp-a2/s2/*.png` (`plots_s2_wpa2.py`).

## 2. H5: Plug threshold / energy scale (DB treatment). **Not supported.**

- Criterion (i): EnCor_MC ≠ 1, or Thresh differing between data and MC by more than the cell-noise width.
  Value: EnCor = 1 and Thresh = 0.3 GeV for all 21 runs (S1 §3). (i) fails, so H5 cannot be true (R7).
- Observable (ii): the fine `ePlug` spectrum for A_pre ∧ η_max > 3.5, unit area. Per-period MC is each component's
  full content projected onto its represented periods by luminosity share (see §6 for the projection fix):

| period | DATA 0-0.1 GeV share | MC share 7071 / 7091 / nd4cat | MC/DATA in 1-7 GeV, 7071 / 7091 / nd4cat |
|---|---:|---|---|
| ep0304 | 0.087 | 0.187 / 0.178 / 0.198 | 0.903 / 0.890 / 0.891 |
| em0405 | 0.082 | 0.187 / 0.178 / 0.196 | 0.876 / 0.864 / 0.870 |
| em06 | 0.084 | 0.186 / 0.177 / 0.186 | 0.867 / 0.855 / 0.867 |
| ep0607 | 0.093 | 0.187 / 0.178 / 0.193 | 0.885 / 0.873 / 0.881 |

  MC has about twice DATA's share of events with no Plug energy, and 10-15% too few events in 1-7 GeV. The deficit
  survives, so the false reading is met. No correction is made: × EnCor = × 1. `h1-mc-expert` reads this as an
  occupancy effect, not a scale effect. A 10% Plug scale leaves ePlug = 0 events at 0 and moves the 1-7 GeV integral by
  a few percent at most.

## 3. H1: η_max definition. **Supported (response branch).** The geometry branch is not supported.

- Response criterion (R5): the 3.15-3.35 excess and the 3.40-3.55 deficit, as Σ(DATA − MC)/DATA with MC normalised
  in 3.5 ≤ η < 5, scope ALL, both move by ≥ 50% between 0.8 and 0.4 GeV, for both 7071 and 7091.

| alt | excess 0.8 → 0.4 GeV | deficit 0.8 → 0.4 GeV | move excess / deficit |
|---|---|---|---|
| 7071 | 0.150 → 0.013 | −0.014 → −0.073 | 0.91 / 4.23 |
| 7091 | 0.149 → 0.009 | −0.017 → −0.078 | 0.94 / 3.67 |

- Geometry criterion: the Δη = `etaMaxVtx08` − `etaMaxClus08` median shift between DATA ep0607 and 9012 (the only
  period 9012 represents) is ≥ 0.03. Value: 0.0004 (medians 0.0001 vs −0.0003; 10775 and 1315 events).
- Caveat (R5 metric): LAr cluster η_max ends near 3.6, so the "3.5 ≤ η < 5" normalisation window holds in practice
  only the two bins [3.5, 3.6). The response reading follows the pre-registered metric and is not reinterpreted. The
  plot `h1_etamax_edge_threshold.png` shows the full 2.5-4 ratio.
- Correction (D3 H1, response branch): (a) the N−1 basis adopts the published 800 MeV / η_max < 3.3 definition
  (`n1_wpa2.py --eta-cut 3.3`, `etaMaxClus08`); (b) the fit uses the threshold variant with the smaller edge residual on
  the derivation split. That residual is the deviance over [3.15, 3.55) in the ALL fine fit, summed over 7071, 7091 and
  nd4cat: 6153 for 0.8 GeV and 4070 for 0.4 GeV, so `etaMaxClus04` is chosen
  (`D/data/restart_2026-10-27/wp-a2/s3_corr/analysis/h1_threshold_choice.json`).

## 4. H6: simulation flags (beam tilt, z-vertex). **Not supported.**

- Criterion: the H1 geometry shift exists (≥ 0.03), and reweighting 9012 to the DATA ep0607 `hatVtxZ` moves it by ≥ 50%.
- Value: the 9012 median moves from −0.0003 to 0.0000; the geometry shift is 0.0004, far below 0.03, so the
  precondition fails. Mean `hatVtxZ` in A_pre: DATA 0.24-1.46 cm by period; 9012 0.72; 6671 1.55; 6677 0.37 cm.
  No reweight enters the frozen set. 6671/6677 keep their WP-D z_vtx systematic.

## 5. H4: ND forward flow vs forward-edge response. **True-detector (R1).**

- Criterion: the edge sample is H4 (A_pre ∧ 35 < E−p_z < 70, no veto) with ePlug > 20 GeV, η in [3.0, 3.6), bins with
  ≥ 20 DATA events, MC normalised per D cell. True-detector requires the sign pattern (DATA > MC in 3.15-3.35,
  DATA < MC in 3.40-3.55) and worst |MC/DATA − 1| ≥ 0.15 for both 7071 and 7091.
- Value: the pattern holds in every cell for both alternatives. Worst deviation: 7071 0.711 / 0.725 / 0.734; 7091
  0.887 / 0.961 / 1.025. DATA/MC is 1.2-2.8 over [3.0, 3.35) and 0.5-0.6 over [3.40, 3.55) (26479 / 15111 / 6648
  DATA events). Inclusive η_max shape metric (7071): 1.21 / 0.73 / 0.99.
- Reading: true-detector overrides false (R1), so true-physics is not read. The pattern is the same for RAPGAP ME+PS
  and CDM, in a sample with guaranteed forward activity.
- Caveat: the non-ND admixture is 15% / 12.5% in D0 / D1 (test threshold 10%). A 12-15% diffractive share cannot make
  a 70-100% edge deviation in both ND models unless the diffractive templates carry the same edge feature. That is
  the same detector-level reading.
- Correction (D3 H4, true-detector): the fit range is restricted to η_max < 3.4 plus one merged edge bin [3.4, 5)
  (`payload_wpa2.py --eta-max-cut 3.4 --edge-merge`).

## 6. H2: random-trigger overlay rate / forward noise. **Supported by its criterion (layer/station-specific); the specialist disputes that remedy-1 is a sound correction.**

- Criterion (R2): even-split counters only. H2 needs either a run-order or per-run-luminosity trend within a period
  (5 equal-count blocks, chi² p < 0.0027 or |slope z| > 3), or an excess that sits in specific layers or stations.
- Trend: none. Over periods × {Plug, FMD, FTS} × {run order, per-run A_pre count}, the smallest block p is 0.063 and
  no |z| exceeds 3. The run list's `lumi_per_run_nb` column is empty for every run, so the per-run A_pre count is used
  as the luminosity proxy (code review C1; the first pass had silently ordered at random). The run-list columns
  `veto_fail_rate_PLUG7` / `mean_PLUG7` are context only.
- **Projection fix.** The first analysis pass grouped MC by `RunNumber`, but an MC sample carries the run numbers of its
  simulation period only (7071: em06; 7091: ep0607). This put the whole ND weight into one period. `h1-mc-expert`
  found the error. The per-period MC tables (H2 layers, H5 (ii)) now project each component's full content onto its
  represented periods by luminosity share (`s2_analyze_wpa2.proj_share`, the remedy-1 convention). H1, H4 and H6 do not
  group by period and are unchanged.
- Layers and stations, after the fix (deep-gap control; DATA vs DIFF8 + ND cocktail):
  - The uniformity chi² gives p = 1e-71 to 2.5e-7 in every period, for both 7071 and 7091. It includes the MC
    statistical error (code review C2; the first pass had DATA errors only: 1e-240 to 1e-10) and treats correlated
    layers as independent.
  - The pattern has both signs. FMD layers 1-3: DATA 0.22-0.36 vs MC 0.21-0.26. FMD layer 4 is low in DATA in every
    period (0.15-0.24 vs 0.25-0.28). FTS indices 0-3: DATA about 2× MC (0.08-0.12 vs 0.04-0.05). ep0607 FTS indices
    4-7: DATA 0.005-0.03 vs MC 0.06-0.08. Per period there are 0-7 layers with a > 3σ excess and 0-6 with a > 3σ deficit.
  - The literal criterion is met.
- `h1-mc-expert` reading (SIGNED no for H2). The structure is mostly efficiency or dead channels: unsimulated FMD
  layer-4 inefficiency, and dead or masked FTS channels in ep0607 data. A genuine positive excess remains only in the
  pre-toroid FMD layers 1-3 and the FTS indices 0-3. That excess fits beam-induced activity or forward flow.
  Remedy-1 q is a uniform event removal, q = (f_d − f_MC)/(1 − f_MC). It cannot represent deficits, so the expert
  calls it an effective occupancy correction (systematic leg), not a noise correction.
- **Superseded by S3** (`s3_correction_record.md` §4): `h1-analysis-critic` excluded remedy-1 from the frozen set
  on D5 (i)/(ii). The disposition below is the S2-time proposal.
- Disposition (S2 proposal): D3 binds. "If true: the WP-A remedy-1 removal q per period … is kept as the noise correction". The
  criterion was fixed before the data and is not re-read after the fact, so the frozen set keeps remedy-1. The q comes
  from `D/data/restart_2026-10-27/wp-a/s2_remedy1/noise_table.json`, derived on the WP-A even-parity deep-gap control,
  inside the derivation split. The per-sub-period refinement is the identity, because no trend is present. The expert
  dissent goes to `h1-analysis-critic` at the S3 sign-off. Remedy-1 cannot repair the sub-threshold Plug shape (WP-A),
  and it enters G1′(a) and (b) (R2).

## 7. H3: fit structure. Fits are pending in S3.

By R3, "the edge pattern of H4 is absent" means "H4 true-detector not supported". H4 true-detector is supported, so
H3 cannot be supported whatever the fits give. The 12 pre-registered fits still run, for the record and for
`h3_variants_complete`.

## 8. Specialist validation (`h1-mc-expert`, 2026-10-27)

- H5: signed (not supported; occupancy, not scale; production-DB caveat from S1 stands).
- H4: signed (detector reading). The shared feature is the full-range edge pile-up in MC at [3.40, 3.50): D0 DATA
  3914 / 3796 per bin vs 7071 4707 / 6315 and 7091 5275 / 7162. Geometry is excluded (H1 Δη, H6), and the H1
  response points to the LAr forward-edge cluster response. The non-ND admixture cannot produce the shared deficit,
  and the ND-share check is not part of the D3 criterion. Caveat: the worst-bin value is set by the 3.0-3.3 slope,
  which depends on the generator (DATA/MC at 3.0 is 1.7 for 7071 and 2.8 for 7091). The frozen correction should
  rest on the shared [3.40, 3.55) feature. It does: the H4 correction merges everything from 3.4 upward.
- H2: SIGNED no. The criterion reading rests on mixed-sign efficiency effects (§6). The first-pass projection
  artefact the expert found is fixed, and the literal criterion is still met.
