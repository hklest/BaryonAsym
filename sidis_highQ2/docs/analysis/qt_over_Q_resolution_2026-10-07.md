# qT/Q resolution design study — 2026-10-07

**Status:** PROVISIONAL INTERNAL DIAGNOSTIC; corrected one-file RAPGAP31 smoke, not a release input or an unfolding product.

## Observable, selection, and matching

`qT/Q = pT_h^CM/(z Q)`, where `pT_h^CM` is transverse to the virtual-photon axis after boosting the hadron and photon into the gamma*-p CM (`W=q+P`) rest frame and `z=(P.p_h)/(P.q)`. Reco uses the candidate momentum, scattered electron, and `Q=sqrt(Q2es)`; gen uses the inlined matched partner, inlined generator electron, per-event `ChargedTotGen` beams, and `Q=sqrt(Q2_es_gen)`. The hadron mass is selected by `|GenPDG|`: pi 0.13957, K 0.49368, p 0.93827 GeV (pion default).

The verified computational references are `extract_weighted_bin_centers_v6.py:41-62` and `build_migration_th2_per_charge_v4.py:296-337`. The prior citation to `_compute_pTh_cm_and_z at src/ChargedSidisTreeBuilderHighQ2_v6.C:311-336` was incorrect: that builder region is argument parsing, the builder stores no `pT_h^CM`, and its relevant z definition is at `:1844-1872`.

Selection is the frozen final-candidate reco selection, including `0.05 <= z_reco < 1` (SUPERSEDED 2026-10-07: this lower bound was a study-only cut, not part of the frozen release selection — see the correction section below; the released scope is `0 <= z_reco < 1` and is now the default), the reco-only lab-frame electron-cone veto `alpha(track,e) >= 0.2`, and the stated reco event/candidate requirements. Gen uses the truth-fiducial cuts and pi/K/p-only pool, without an alpha veto. The duplicate inlined-partner key rejection remains enabled: all claimants of a duplicate rounded `(GenP,GenEta,wrapped GenPhi,GenCharge)` key within a file/event are removed.

## QA-corrected qT/Q axes and resolution definitions

The former `[0,0.6)` gen range truncated the measured tail. The new command-line defaults (`--qt-gen-edges`, `--qt-reco-edges`) are variable-width axes:

* gen: `0, 0.01, 0.02, ..., 0.10, 0.12, 0.14, 0.16, 0.18, 0.20, 0.25, 0.30, 0.40, 0.50, 0.60, 0.80, 1.00, 1.50, 2.00, 3.00, 4.00, 5.00`;
* reco: `-0.05, 0, 0.01, 0.02, ..., 0.10, 0.12, 0.14, 0.16, 0.18, 0.20, 0.25, 0.30, 0.40, 0.50, 0.60, 0.80, 1.00, 1.50, 2.00, 3.00, 4.00, 5.00`.

Thus the core remains 0.01-wide through 0.10, then widths are 0.02, 0.05, 0.10, 0.20, 0.50, or 1.00 as specified by adjacent edges. No overflow bin is used: values outside 5 remain explicitly counted. The primary resolution quantities are unweighted `abs_sigma68`, the central-68%-interval half-width of `Delta=(qT/Q)_reco-(qT/Q)_gen`, and `abs_rms=sqrt(mean(Delta^2))`, both dimensionless. `abs_mean_bias=mean(Delta)` is the signed absolute bias. Legacy relative quantities retain `dfrac=Delta/(qT/Q)_gen`. `n_pairs_unweighted` aliases `n_pairs`; `n_pairs_weighted` aliases `sum_w`.

## Corrected smoke validation

Input: `sidistree_rapgap31_7094_ep0607_0091.root`. Counters reproduce exactly: `kept=59267`, `matched=63667/64405=0.98854`, `n_evt_pass=248579`, and `n_zcut_pass=64405`. The matched fraction is consistent with the documented roughly 10% response fake fraction once denominators are distinguished: about 82% of that fake category is matched-but-gen-out-of-fiducial rather than genuinely unmatched. In this file the fake fractions are 10.09% (h+) and 9.92% (h-).

The median `z_gen=0.1033` and median `Q=18.0 GeV`, explaining qT/Q medians near 0.46. Tier-0 same-event `ChargedTotGen` partner presence is `58976/59204=99.615%`; the 228 absences are all STR-class and statistically indistinguishable from the bulk. The remaining matching caveat is unchanged: a guaranteed unique Tier-0 key requires a builder change.

| reco cell, charge | valid pairs | gen in-range | both axes in-range |
|---|---:|---:|---:|
| S0_x0 h+ | 8428 | 99.739% | 99.620% |
| S0_x0 h- | 7948 | 99.660% | 99.623% |
| S0_x1 h+ | 5283 | 99.962% | 99.943% |
| S0_x1 h- | 5092 | 99.961% | 99.961% |
| S1_x0 h+ | 6970 | 99.957% | 99.943% |
| S1_x0 h- | 6787 | 99.897% | 99.882% |
| S1_x1 h+ | 4377 | 100.000% | 100.000% |
| S1_x1 h- | 3979 | 100.000% | 100.000% |
| S2_x0 h+ | 2217 | 100.000% | 100.000% |
| S2_x0 h- | 2012 | 100.000% | 100.000% |
| S2_x1 h+ | 1957 | 100.000% | 100.000% |
| S2_x1 h- | 1666 | 100.000% | 100.000% |
| S3_x0 h+ | 1362 | 100.000% | 100.000% |
| S3_x0 h- | 1189 | 100.000% | 100.000% |

Overall gen-axis retention is `59204/59267 = 99.894%`; both-axis retention is `59188/59267 = 99.867%`. This exceeds the 95% gate in every cell/charge. `abs_sigma68` and `abs_rms` are present and finite in every summary bin with `n_pairs >= 2` (zero failures).

## Representative gen-binned resolution

Each entry is `gen qT/Q: abs_sigma68, relative sigma68 [N]`; the final value is the first N>=10 point where relative sigma68 is below 0.20.

| cell, charge | near 0.10 | near 0.30 | near 0.50 | first relative <0.20 |
|---|---|---|---|---:|
| S0_x0 h+ | 0.095: 0.0370, 0.3908 [92] | 0.275: 0.0488, 0.1746 [518] | 0.450: 0.0575, 0.1299 [743] | 0.225 |
| S0_x0 h- | 0.095: 0.0484, 0.5037 [97] | 0.275: 0.0428, 0.1545 [480] | 0.450: 0.0496, 0.1127 [732] | 0.225 |
| S0_x1 h+ | 0.095: 0.0860, 0.8875 [68] | 0.275: 0.0876, 0.3199 [339] | 0.450: 0.1022, 0.2328 [493] | 0.550 |
| S0_x1 h- | 0.095: 0.0877, 0.8881 [55] | 0.275: 0.0752, 0.2729 [339] | 0.450: 0.1070, 0.2366 [495] | 0.550 |
| S1_x0 h+ | 0.095: 0.0392, 0.4152 [154] | 0.275: 0.0451, 0.1652 [524] | 0.450: 0.0459, 0.1019 [586] | 0.225 |
| S1_x0 h- | 0.095: 0.0481, 0.4983 [126] | 0.275: 0.0477, 0.1695 [534] | 0.450: 0.0408, 0.0912 [584] | 0.225 |
| S1_x1 h+ | 0.095: 0.0800, 0.8380 [114] | 0.275: 0.0792, 0.2961 [303] | 0.450: 0.0921, 0.1994 [344] | 0.450 |
| S1_x1 h- | 0.095: 0.0699, 0.7333 [93] | 0.275: 0.0805, 0.2959 [275] | 0.450: 0.0839, 0.1956 [394] | 0.450 |
| S2_x0 h+ | 0.095: 0.0299, 0.3178 [71] | 0.275: 0.0359, 0.1311 [161] | 0.450: 0.0536, 0.1259 [138] | 0.190 |
| S2_x0 h- | 0.095: 0.0331, 0.3578 [60] | 0.275: 0.0454, 0.1688 [150] | 0.450: 0.0484, 0.1102 [121] | 0.190 |
| S2_x1 h+ | 0.095: 0.0984, 1.0507 [67] | 0.275: 0.0793, 0.2949 [139] | 0.450: 0.1067, 0.2423 [116] | 0.550 |
| S2_x1 h- | 0.095: 0.0579, 0.6055 [41] | 0.275: 0.0722, 0.2563 [134] | 0.450: 0.0984, 0.2221 [108] | 0.550 |
| S3_x0 h+ | 0.095: 0.0467, 0.4966 [58] | 0.275: 0.0346, 0.1290 [77] | 0.450: 0.0418, 0.0880 [62] | 0.190 |
| S3_x0 h- | 0.095: 0.0203, 0.2099 [37] | 0.275: 0.0384, 0.1443 [87] | 0.450: 0.0412, 0.0931 [59] | 0.190 |

N>=10 populated gen bins (and N<10 unreliable markers) are respectively: S0_x0 26/1 and 26/1; S0_x1 25/2 and 25/2; S1_x0 26/1 and 25/2; S1_x1 24/3 and 24/3; S2_x0 24/3 and 24/3; S2_x1 23/4 and 23/4; S3_x0 23/4 and 23/4, in h+/h- order.

## SUPERSEDED-SCOPE CORRECTION (2026-10-07, QA adjudication) — released z scope

The study previously applied a study-only `z_reco >= 0.05` cut that the central release does **not** apply: the release reco and gen scope are both `0 <= z < 1`, and `zlow005` in the release path is only a binning/path tag (the `[0,0.05)` bin is a first-class released bin). Consequently **all z>=0.05-only results produced before 2026-10-07 are superseded for physics use**: the `kept=59267` 3D smoke result, the per-cell pair table and resolution tables above, and any purity/stability gate derived from them were computed on the z-restricted sample. They are retained here as history; `--z-min-reco 0.05` reproduces them exactly (kept=59267, matched=63667, zcut_pass=64405, evt_pass=248579 — verified 2026-10-07 in the provenance run below).

The default is now the released scope (FIX 1), the 4D ledger was rebuilt with central-response-equivalent fiducials and exact per-series closure (FIX 2/3), and a one-file central-selection comparison plus a z-scope audit were added (FIX 4). One-file RAPGAP31 (`sidistree_rapgap31_7094_ep0607_0091.root`) results under the released scope:

* Selection scale-up: `zcut_pass` 64,405 -> 176,381; kept pairs 59,267 -> 161,128 (x2.72); every (cell,charge) selected count increases (x2.08-x3.20).
* Sliced 98-series ledger (`(cell, z[7 released bins], charge)`), per series: `reco_total`, `gen_total`, `body`, `fake`, `miss`, `qt_reco_out`, `qt_gen_out`. Closure identities `body+fake+qt_reco_out=reco_total` and `body+miss+qt_gen_out=gen_total` hold exactly for all 98 series (asserted in code; worst residual 0 in both the provenance and released-scope runs). `reco_total`/`gen_total` are full physical marginals; `reco_inaxis`/`gen_inaxis` are the qT/Q in-axis marginals; they are reported separately and never mixed.
* Efficiency `body/(body+miss)` is in (0,1] for all 98 populated series (min 0.1250 at `S2_x1_z6_neg`, max 0.8000 at `S2_x0_z6_neg`); the z0 bin, structurally zero (body) before, is now populated in all 14 (cell,charge) combinations: body 1,765-13,174 per series, z0 efficiency 0.289-0.452.
* z-scope audit (pooled): 63.5% of reco candidates (111,976/176,381) and 73.4% of fiducial in-grid truth (263,601/358,959) sit in z `[0,0.05)`; 61.9% of the in-axis reco yield is in z0.
* z0 qT/Q distribution (pooled reco): median 1.356, q90 4.252, q99 8.072; **6.64% of z0 reco candidates have qT/Q >= 5.0** (the current axis ceiling; no non-finite values). Gen side: median 1.858, q90 6.822, q99 12.93, 17.74% >= 5.0. Per-(cell,charge) the z0 frac>=5.0 reaches 15.5-15.7% in S0_x0 and the z0 q99 reaches 9.45 there. **Verdict: the 5.0 axis ceiling does not contain the z0 population** (q99 8.07 reco / 12.93 gen; 6.6%/17.7% beyond the ceiling); the closure ledger carries these as `qt_reco_out`/`qt_gen_out`, and any z0-inclusive unfolding use of these matrices requires either an extended axis or an explicit out-of-axis treatment.
* Study-vs-central comparison (one file, read-only mirror of `build_migration_th2_per_charge_v4.py` finalcandidate conventions: |PVz|<35 cleanup, no alpha veto, fixed pion mass, no one-to-one duplicate drop, F7 gen-charge rule, NucliaWeight weights): central mirror counters 176,561 reco PS rows / 162,568 matched / 13,744 fake / 198,113 miss; per-series sums reproduce the counters exactly. At z0 the study (central-equivalent, slice-local) efficiency is within -0.070..+0.028 of the central mirror's own (mean -0.021); study fake fractions at z0 (0.18-0.25) are systematically higher than the central mirror's (0.05-0.10) because the study's slice-local fake absorbs cross-slice z/charge migration and duplicate-key drops, while the central matched fill books cross-z-bin pairs as matched. Over all 98 series the study efficiency is lower on average (mean -0.13), driven by the same slice-local miss convention at high z. Both conventions are now quantified side by side in the manifest.
* Tier-0 partner presence is unchanged in nature (absent fraction 0.71% at the released scope, 1,220/171,995 unique Tier-0 keys); 913 body candidates had no same-series in-axis pool partner and are counted as fake per series (`bodycand_partner_not_in_truth_pool`).

Artifacts for this correction (old smoke outputs in the same directory are retained unmodified as provenance of the superseded scope):

* Provenance run (reproduces the superseded scope exactly): `data/qt_over_Q_resolution/2026-10-07/zdifferential_smoke/z005_provenance_2026-10-07/` (`qtres_z005_provenance.root`, `.json`, `stdout.log`)
* Released-scope run with central comparison: `data/qt_over_Q_resolution/2026-10-07/zdifferential_smoke/zscope_released_2026-10-07/` (`qtres_zscope_released.root`, `.json` with `zdifferential_4d` and `central_convention_comparison`, `stdout.log`, `plots/`)
* Plotter re-run on the released-scope output exits 0 (no crash on the new manifest keys or the populated z0 bin); full z-differential plotting is deferred.

The 364-file Condor pass remains clear to launch for this diagnostic extractor only, now under the released z scope by default (`--central-compare` off for multi-file passes; the 4D block is single-file-only), and it must remain separate from the central release and response/unfolding chain.

## Artifacts

* ROOT: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/qt_over_Q_resolution/2026-10-07/qt_over_Q_resolution_rapgap31_7094_ep0607_0091.root`
* Manifest: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/qt_over_Q_resolution/2026-10-07/qt_over_Q_resolution_rapgap31_7094_ep0607_0091.json`
* PDFs: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/qt_over_Q_resolution/2026-10-07/`

The 364-file Condor pass is clear to launch for this diagnostic extractor only; it must remain separate from the central release and response/unfolding chain.

## FULL-POOL CORRECTED VERDICT (2026-10-08) — supersedes every 3D, z-integrated, and pre-fix number above

### Scope and provenance corrections (binding)
- `zlow005` is a PATH TAG, not a cut. Released z scope is `0 <= z < 1`; no study may apply `z >= 0.05`. Filenames and labels must never carry a `_z005` suffix.
- Migration-matrix orientation was inverted in the first 4D build and is FIXED: `x = RECO`, `y = GEN`, FAKE in the final gen column carrying physical reco indices, MISS in the final reco row carrying physical gen indices (`build_qt_over_Q_resolution.py:949-979`).
- Coarsened purity had a real defect: it summed FINE diagonal cells, which is grouping-invariant and understated coarse purity by 0.25-0.35. Fixed to the matched coarse-block diagonal `B[np.ix_(rx,ix)].sum()`. The headline `-0.279` purity deficit is WITHDRAWN.
- `derive_4d_purity_acceptance.py` md5 history `15c7afef80316d91449069a69e0f46de` -> `522b5ff1b2018eca6728f800b02a617f` (the later edit only fixes the informational `N_fake_reco` index at `:51`; all seven metric/count columns bit-identical across 4,116 rows per pool).
- Resolution CANNOT be taken from a merged body matrix; the merged-42x43 reconstruction failed (worst deviation 1.67 mean, 103.24 RMS, 1023 sigma68). Per-pair extraction is the only resolution source, gated exactly against published single-file truth.

### Published products
- Per-series 4D, released z scope, corrected: RAPGAP31 md5 `d4ae34c6b7bddaea1305cef02622fa5e` (364/364 files, kept 58,653,117, body 49,570,043, fake 14,836,155, miss 79,658,571); DJANGOH14 md5 `e4373cef853e665e1b4b12ef96c49c88` (500/500, kept 69,520,545, body 58,517,564, fake 18,188,573, miss 95,264,765). Clusters 5386259 / 5386260.
- Per-pair resolution: `.../2026-10-07/fullpass_pairs_rapgap31/pairs_resolution_rapgap31_merged.root` md5 `a2411c7a2c02eddd1556fa131c6fc765`; in-axis 58,652,892; kept-in-axis gap 225 pairs.
- Release-keyed GLOBAL (convention-matched, RAW UNWEIGHTED): RAPGAP31 `.../2026-10-08/global_release_keyed_rapgap31/` md5 `1771fc4b415d63fbaca9c472cda02ab3`; DJANGOH14 `.../global_release_keyed_djangoh14/` md5 `947bbff8471f090ba026c9e3cff8b342`. Clusters R 5387372/5387742, D 5387373/5387743, 0 held, 364 and 500 files exactly once, closure exact, titles verbatim `RECO flattened (final: MISS)` / `GEN flattened (final: FAKE)`.
- Booking is unweighted by construction: `np.add.at(...,1)` at `build_qt_over_Q_resolution.py:1557` (body), `:1565` (fake), `:1572` (miss). Absolute values are NOT comparable with released weighted absolutes; only the qT/Q-to-pT_h difference inside one product is meaningful.

### Block-keying artefact (the reason the first comparison was invalid)
The release keys gen by its own kinematics and reco by its own, with NO cell/z equality, so cross-block matched pairs stay body (`build_migration_th2_per_charge_v4.py:510-542`, `:679-683`, `:908-930`, `:956-977`). The per-series 4D extractor required `gcell == rc` and `gzbin == rz` (`build_qt_over_Q_resolution.py:1388-1391`, `:1407-1421`), charging cross-block migration to fake and miss. Measured penalty, full pool: fake 2.3445x (RAPGAP31) / 2.2973x (DJANGOH14), miss 1.1196x / 1.1208x.

### Purity, acceptance, stability verdict
- Partition-dependence is the dominant effect, not the variable. Per-block purity is NOT a property of the partition (regrouping invariant), and ratio-of-sums aggregate purity IS invariant under regrouping of the same fine partition, so earlier weighted-mean comparisons were meaningless for bin-placement questions.
- 5-bin equal-statistics qT/Q aggregate 0.6516 (RAPGAP31 h+) against yield-structure-matched 0.7045 and equal-width 0.7535; equal-statistics quantiles are the WORST permissible choice.
- Convention-matched, full pool, like-for-like qT/Q minus pT_h: delta purity -0.000337 / -0.000233 (h+ / h- RAPGAP31), -0.000350 / -0.000259 (DJANGOH14); delta acceptance +0.000577 / +0.000529, +0.000668 / +0.000642; delta stability +0.0003. All 36 aggregate rows |delta| <= 0.0007. GENERATOR-STABLE.
- WITHDRAWN: the earlier claimed acceptance deficit of 0.122 (convention artefact, 53-55% attributed to keying, >=99.45% removed like-for-like) and the ARM 3 claim that qT/Q beats pT_h by +0.0194..+0.0258 at matched placement (leg-weighting artefact: weighted release leg 0.6794 against unweighted qT/Q leg; unweighted same-placement pT_h is 0.7072, flipping the delta to -0.003..-0.005).
- VERDICT: at matched structure and matched convention, qT/Q and pT_h are PURITY- AND ACCEPTANCE-EQUIVALENT.

### Resolution verdict
- Published per-z exact relative-RMS ratio qT/Q over pT_h: 1.6572, 1.0879, 1.0599, 1.0067, 0.7790, 0.6993, 0.3949, overall 1.0365. Per-z medians from the ARM 3 control (0.91, 0.73, 0.77, 0.62, 0.47, 0.35) reproduce exactly as medians over `N>=1000` h+ bins; the z0 sign difference is a SUPPORT-CUT artefact, not a data error.
- Relative RMS is NOT meaningful at z0: bins holding 0.021% of pairs carry ~51% of the relative second moment. The ABSOLUTE RMS ratio decides, z0 median 2.39 (6/7 cells > 1, pooled ~4.0), i.e. qT/Q clearly worse at low z.
- From z2 upward qT/Q wins 7/7 blocks on absolute RMS, medians 0.367, 0.200, 0.121, 0.063. Corrected crossover is z ~ 0.05-0.1, NOT 0.2.
- `1/(z*Q)` rescaling factor half-68 spread 0.091234 on 58,466,839 pairs.
- Support, not purity, is the binding constraint: only 4 of 49 blocks retain >=5 resolvable bins under the regularised-IU effective-rank gate (FEASIBLE-AFTER-MERGING everywhere, positivity order 2, generator-robust to <=1 bin). z6 has no `N>=1000` qT/Q gen-bin support at all.
- Known defects, parked, non-blocking: `derive_4d_resolution_pairs.py:129` OVERWRITES rather than sums `matched_gen_z_same`, and the pairs merge DROPS the `pair_z_migration` tree that exists in the per-file partials (e.g. `partial_chunk_08.root`).

### Open-ended highest qT/Q bin (2026-10-08 investigation, NOT adopted)
- Release convention audited: pT_h truth edges `[0,0.5,1,2,4,8]` GeV with a HARD 8.0 edge, above-axis gen DROPPED (`binning_v3.py:108,110-111`; builder `:541-546`, `:795`, `:951`), in-grid reco of an out-of-axis partner routed to FAKE. Our treatment is identical. Therefore no absorb-versus-drop asymmetry exists and the parity conclusion is unaffected (bias exactly zero).
- Option implemented anyway, `--qt-open-last-bin` with `--qt-open-sentinel` (default 1e6), extractor md5 `327407ab733107ed03d95e81660a5c7c` -> `2d89aa277e4e49a14202217d3973f8bb`; single-file products in `.../2026-10-08/open_last_bin_test/`.
- Effect confined to the gen side of the last bin, all 262 moved truths UNMATCHED (kept-pair gen max 15.97 < 16.90): top-bin h+ gen population 347 -> 475, h- 370 -> 504, h+ acceptance 0.00576 -> 0.00421, purity undefined before and after (top reco bin empty), body migration unchanged, neighbour bin bit-identical, ledger closes with `qt_gen_out` 262 -> 0.
- Aggregates: delta purity unchanged to six decimals; delta acceptance improves +0.000493 -> +0.000177; delta stability +0.000260 -> -0.000010. ARM 3 clipping is eliminated (49/49 blocks representable; mapped edges reached 55.49 against a 16.90 ceiling). Real gen reach exceeds 16.90 only in `S0_x0 z0` (99.99th percentile 20.44, max 21.19, 0.277%/0.291% of pairs); all other 96 blocks stay below 14.9.
- DISPOSITION: keep the hard 16.90 edge with drop, i.e. release-identical. The open last bin is retained only as a diagnostic that makes the z0 ARM 3 blocks representable; adopting it would break convention parity for a gain under 4e-4.

### FINAL AXIS RECOMMENDATION
Do NOT replace the released `pT_h` axis with qT/Q. Purity, acceptance and stability are equivalent at matched structure, so the axis is not purity-limited; it is LIMITED BY RESOLVABLE-BIN SUPPORT (>=5 bins in 4 of 49 blocks, none at z6) and by a large absolute-resolution penalty at low z. The defensible upgrade path is a Z-SPLIT HYBRID: keep `pT_h` below z ~ 0.1, where qT/Q absolute RMS is ~2.4x worse, and consider qT/Q above z ~ 0.2, where it is better in 7/7 blocks, and only with WIDE HIGH-qT/Q-WEIGHTED partitions, never equal-statistics quantiles, and only with regularised iterative unfolding, never bin-by-bin correction.

## CORRECTED RECO-MASS CONVENTION AND HYBRID z>0.1 UNFOLD (2026-10-09)

**SUPERSEDED:** every purity, acceptance, stability, resolution, and like-for-like number derived in this document before 2026-10-09 — the smoke and released-scope tables, the full-pool corrected-verdict aggregates and deltas, the per-z RMS ratios, and the like-for-like equivalence rows — used a RECO leg built from the truth `|GenPDG|` species mass. A data vector carries no truth species and cannot reproduce that leg, so none of those numbers are reproducible on data. They are retained above as history only.

- The FINAL AXIS VERDICT still stands: qT/Q is NOT a replacement for the released `pT_h` axis. The verdict was drawn at matched structure and matched convention, and the corrected re-derivation under the fixed convention keeps it.
- The crossover recommendation — `pT_h` below z ~ 0.1, qT/Q above — is now IMPLEMENTED, not proposed: the hybrid z>0.1 measurement is complete.
- CONVENTION FIX: the central release computes both legs with `M_PION` fixed (`build_migration_th2_per_charge_v4.py:287`, `:320`, inside `_compute_pTh_cm_and_z:311-336`). The corrected products apply the fix to the reco leg only; the gen leg deliberately keeps the true species mass.
- DATA-VECTOR CONVENTION CORRECTION: the released data vector convention has NO alpha veto, while the corrected response reco leg does. The study data vector therefore deliberately uses its own convention (alpha veto ON, `|PVz|<35` OFF, `pT_h<8` grid OFF, pion reco mass, unweighted `+1` counts; spec `data/qt_over_Q_resolution/2026-10-09/data_side_spec/data_side_spec.json`). The two totals — `3,884,097` released in-grid and `780,199` in-axis here — are NOT comparable and must never be quoted against each other.

The authoritative record is the STATE.md section "qT/Q hybrid z>0.1 measurement, corrected reco-mass convention (2026-10-09)" and the artifact directories under `data/qt_over_Q_resolution/2026-10-09/` — authoritative corrected responses `global_weighted_pi_rapgap31/` and `global_weighted_pi_djangoh14/`, data vector `data_vector/`, re-freeze `refreeze_pi_fullpool/`, unfolding `driver_repair/`, `unfolding_review/`, `unfold_final/`; the same-day truth-mass products `global_weighted_rapgap31/` and `global_weighted_djangoh14/` are superseded and not-for-unfolding; figures `plots/qt_over_Q_resolution/2026-10-09/qtspec_final/`.
