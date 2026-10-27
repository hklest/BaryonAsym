# WP-A2 S1: definition audit, Plug DB probe and pre-registration freeze

Date 2026-10-27. Child plan `D/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md` (sha256 at freeze
`c553d2007df95fbd78d386a96636328d6434cf21c1171b717dd9a67eb2225b0d`), slice S1. `D` =
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`, `H1OO` =
`/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo`.
Paper line numbers ("l.") refer to `pdftotext` output of the arXiv PDFs 1203.4495 and 1412.0928, fetched 2026-10-27.

## 1. Published reference definitions (D1)

| item | arXiv:1203.4495 (EPJC 72 (2012) 2074) | arXiv:1412.0928 (HERA-II dijets) |
|---|---|---|
| η_max | §3.3: "The pseudorapidity η_max of the most forward energy deposit in the LAr calorimeter above a noise threshold of 800 MeV is required to be less than 3.3" (l.522-523) | η_max < 3.2 with the 800 MeV LAr threshold (l.419-420, 468) |
| FMD | §3.3: "At most one hit pair should be present in the first two layers of the FMD" (l.523-524) | — |
| Plug | §3.3: "The energy measured in the Plug calorimeter is required to be smaller than 7 GeV" (l.524-525) | Plug extends the gap (l.470) |
| FTS | §3.3, 2004-2007: "no hits in the 26 m and 28 m stations of the FTS" (l.526-527) | 28 m station only (l.390) |
| noise losses | §3.3: "determined using randomly triggered events which are overlaid with simulated events" (l.546-548) | — |
| E−p_z | §3.3: Σ(E−p_z) > 37 GeV (l.494) | — |
| uncertainties | §4: FMD efficiency 4% (2004-07), FTS 7%, Plug energy scale 10%, noise energy 10% (l.617-623) | — |
| Q² range | HERA-II LRG sample up to Q² = 105 GeV² (l.440); no η_max control plot shown | — |

Vertex vs nominal-origin η and electron exclusion are stated in neither paper. No published control plot is a target (D1).

## 2. Implemented definitions (D2), with pointers

| quantity | implementation | pointer |
|---|---|---|
| `etaMaxClus08` | writer stores `gH1Calc->Diff()->GetEtaMax(1)` = HAT `EtaMaxClus[1]` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6dif.C:1147-1148,2495`; `H1OO/H1Calculator/H1CalcDiff.C:199-210` |
| HAT `EtaMaxClus[i]` | per cluster and calorimeter, E_final-weighted centre, η from the origin (z = 0); LAr and SpaCal only (Iron, Plug and BPlug have E_min = 1000 GeV, i.e. excluded); E > 0.4 GeV (i = 0) or 0.8 GeV (i = 1); no electron exclusion | `H1OO/H1HatFiller/H1HatDiffractiveFiller.C:38-42,100-147` |
| `ePlug` | writer stores `ForwDets()->GetEPlug()` = HAT `HfsClusPlugE` (× (1+shift) only under a systematic variation) | writer `:2452`; `H1OO/H1Calculator/H1CalcForwDets.C:249-268` |
| HAT `HfsClusPlugE` | HERA-II: a Plug cell enters if E_cell (× 1/EnCor for MC) > Thresh; the MC sum carries × 1/EnCor | `H1OO/H1HatFiller/H1HatCaloFiller.C:243-244,272-292`; Plug cluster threshold 0 (`:39`), Plug excluded from the HFS sum (`:47`) |
| `etaMaxVtx08`, `etaMaxVtxExcl08` | data and ODS samples only; −999 for the noODS samples 6671, 6677, 7071, 7091, 7094, 7096, 7097 | `D/docs/restart_2026-10-27/wp-d/mc_gap_list.md:85-100` |

Differences from D1: (i) the implemented η_max also takes SpaCal clusters; they are backward, so they set η_max only
when no LAr cluster passes 0.8 GeV; (ii) the published cut is 3.3, the analysis cut is 3.2 (the 1412.0928 value);
(iii) η is taken from the nominal origin, which neither paper excludes. None of these can be recomputed from the ND
trees (HAT+MODS, no cells).

## 3. Plug calibration database (H5 observable (i))

The macro `D/scripts/restart_2026-10-27/wp-a2/plugdb_probe.C` (login node, < 1 min) reads `H1CalibPlug` through
`H1DBManager::GetDBEntry(H1CalibPlug::Class(), run)` from the release snapshot
`$H1DIST/releases/4.2.1/share/H1OOSnapshot.root` (`H1OO/H1Geom/H1DBManager.C:208-233`). The run list is
`D/data/restart_2026-10-27/wp-a2/s1_db/runs.csv`: the first and last run of each data period
(`D/data/restart_2026-10-27/wp-a/period_map.csv`) and, per MC sample, the most populated `RunNumber` of its merged file
(`D/data/restart_2026-10-27/wp-a2/s1_db/mc_runnumbers.txt`). The MC files are run-dependent: their `RunNumber` values
span the data runs of the represented periods (7071: 444307-466997), so the MC-side DB entry is that of the data run
the MC event is assigned to. Output `D/data/restart_2026-10-27/wp-a2/s1_db/plug_calib.csv`, 21 rows:

| run | period / sample | MC | Thresh [GeV] | EnCor | validity |
|---|---|---|---|---|---|
| 357160 | ep0304 | 0 | 0.3 | 1 | 303906-511079 |
| 392213 | ep0304 | 0 | 0.3 | 1 | 303906-511079 |
| 399629 | em0405 | 0 | 0.3 | 1 | 303906-511079 |
| 436893 | em0405 | 0 | 0.3 | 1 | 303906-511079 |
| 444312 | em06_true | 0 | 0.3 | 1 | 303906-511079 |
| 466997 | em06_true | 0 | 0.3 | 1 | 303906-511079 |
| 468532 | ep0607_true | 0 | 0.3 | 1 | 303906-511079 |
| 500611 | ep0607_true | 0 | 0.3 | 1 | 303906-511079 |
| 487272 | 9012 | 1 | 0.3 | 1 | 303906-511079 |
| 479971 | 9024 | 1 | 0.3 | 1 | 303906-511079 |
| 423833 | 9033 | 1 | 0.3 | 1 | 303906-511079 |
| 411482 | 9155 | 1 | 0.3 | 1 | 303906-511079 |
| 464849 | 9157 | 1 | 0.3 | 1 | 303906-511079 |
| 499114 | 9158 | 1 | 0.3 | 1 | 303906-511079 |
| 411482 | 6671 | 1 | 0.3 | 1 | 303906-511079 |
| 499114 | 6677 | 1 | 0.3 | 1 | 303906-511079 |
| 464849 | 7071 | 1 | 0.3 | 1 | 303906-511079 |
| 499114 | 7091 | 1 | 0.3 | 1 | 303906-511079 |
| 499114 | 7094 | 1 | 0.3 | 1 | 303906-511079 |
| 411482 | 7096 | 1 | 0.3 | 1 | 303906-511079 |
| 385957 | 7097 | 1 | 0.3 | 1 | 303906-511079 |

Reading: the snapshot holds a single `H1CalibPlug` entry valid for runs 303906-511079, with Thresh = 0.3 GeV per cell
and EnCor = 1 for every HERA-II data and MC run probed. The HAT filler therefore applies the same 0.3 GeV cell
threshold to data and MC and no MC rescale (1/EnCor = 1). Caveat: the HAT files were produced by the official
production releases, whose DB snapshot is not recorded in the trees; this probe reads the 4.2.1 snapshot.

## 4. Pre-registered hypothesis matrix (D3, frozen verbatim)

- **D3 Hypotheses, pre-registered, in test order (cheapest and most discriminating first).** Each names its observable, the
  derivation-split criterion and the reading under true/false. A hypothesis is "supported" only by its own criterion; a
  reading between a hypothesis's true and false thresholds counts as not supported.
  - **H5 Plug threshold / energy scale per period (DB treatment).** Observable (i): `H1CalibPlug` `Thresh` and `EnCor` read
    from the H1OO database for one data run per period (357160-500611 ranges of
    `D/data/restart_2026-10-27/wp-a/period_map.csv`) and for the sim run of every MC sample of the 17-file queue (9012, 9024,
    9033, 9155, 9157, 9158, 6671, 6677, 7071, 7091, 7094, 7096, 7097); (ii) the fine `ePlug` spectrum 0-10 GeV in 0.1 GeV bins, derivation split, A_pre ∧ η_max
    > 3.5 (ND-dominated), data per period vs MC per sim period. True if `EnCor_MC ≠ 1` or `Thresh` differs between the data
    periods and the MC sim runs by more than the cell-noise width, **and** the ratio of the first non-zero bins to the 0 GeV
    bin in (ii) is reproduced by rescaling and re-thresholding a smooth fit to the data spectrum with the DB values (χ² p ≥
    0.05 on 1-7 GeV). False if `EnCor = 1`, thresholds agree and the 1-7 GeV deficit survives. If true, the correction is an
    analytic rescale of MC `ePlug` by × `EnCor` (undoing the filler's per-cell 1/`EnCor`, which the stored value already
    carries; direction confirmed by the `h1-mc-expert` S1 reading) with the veto re-evaluated at 7 GeV (the veto sits far above
    the cell threshold, so the threshold itself is not corrected); the sub-threshold shape clause G1′(a) is then evaluated on
    the rescaled spectrum. Cost: login node (DB macro, < 5 min) plus histograms from the S2 pass.
  - **H1 η_max definition mismatch (threshold, cut value, vertex, electron).** Observables, derivation split, A_pre with the
    Plug/FMD/FTS vetoes (the §7.1 cut-removed basis): the η_max spectrum per D cell for `etaMaxClus08` vs `etaMaxClus04`, data
    vs the 7071 and 7091 cocktails each normalised in 3.5 ≤ η < 5; and, data and 9012 only, the shift Δη = `etaMaxVtx08` −
    `etaMaxClus08` for events with `etaMaxClus08` ∈ [3.0, 3.6). True if the 3.15-3.35 excess and the 3.40-3.55 deficit move by
    ≥ 50% of their size between the 0.4 and 0.8 GeV thresholds (response-driven), or if the data Δη distribution differs from
    9012's by a median shift ≥ 0.03 (geometry-driven; a 10 cm vertex offset gives ≈ 0.04 at θ ≈ 4.5°). False if the pattern
    is threshold- and Δη-invariant. If true: adopt the published 800 MeV / η_max < 3.3 definition for the N−1 basis (R7's
    3.3 variant) and, for the fit, the threshold variant with the smaller edge residual, chosen on the derivation split.
    Cost: S2 histograms only.
  - **H6 Simulation flags (beam tilt, z-vertex) per sample.** Observables: the `hatVtxZ` distribution per sample vs data per
    period, and the H1 Δη test restricted to 9012 (tilt and z-vertex simulated, SW 84) against 6671/6677 (neither simulated,
    SW 70, `mc_gap_list.md:103-104`; cluster η only). True if the Δη median shift of H1 is reproduced by the z-vertex
    distribution difference alone (reweighting 9012 to the data `hatVtxZ` moves the shift by ≥ 50%). False otherwise. If
    true: a `hatVtxZ` reweight per sample becomes part of the frozen correction set; the SW 70 Reggeon samples keep their
    WP-D z_vtx systematic. Cost: S2 histograms only; lowest discriminating power for the edge (expected Δη ≲ 0.04).
  - **H4 ND forward energy flow vs forward-edge response, no-gap inclusive control.** Sample: A_pre with 35 < E−p_z < 70 GeV
    (joined from `ChargedTot` on (RunNumber, EventNumber), `D/docs/t2/empz_reconstructibility_2026-10-18.md:31-45`), no
    forward veto, derivation split; ND MC normalised to data per D cell. Two observables: (edge) the `etaMaxClus08` spectrum
    in [3.0, 3.6) for events with `ePlug` > 20 GeV, where forward activity is guaranteed and the most-forward-cluster position
    is detector-dominated; (flow) the full `etaMaxClus08` spectrum and the `ePlug` spectrum of the inclusive sample. True-detector
    if the edge sample shows the same data-above-3.15-3.35 / below-3.40-3.55 pattern with |MC/data − 1| ≥ 0.15 in the worst
    0.05 bin (the LAr forward edge is sculpted differently in data and simulation: dead or masked cells, noise suppression);
    true-physics if the edge sample agrees within 0.10 but the inclusive η_max and `ePlug` spectra disagree in the same
    direction for both 7071 and 7091. False if the inclusive sample agrees within the G1′(a) window. If true-detector: the
    fit range is restricted to η_max < 3.4 plus one merged edge bin [3.4, 5) (a definitional correction, frozen in S3); if
    true-physics: no correction is admissible without a new ND model (`user:` item, Open Q2). Both ND classes fail G1(c)
    identically, so true-physics is a priori disfavoured. Cost: S2 histograms plus the E−p_z join.
  - **H3 Fit template structure, x_IP window, E−p_z, out-of-fiducial admixture.** Observables, derivation split, §7.1 fit
    with the same binning: (i) the baseline two-template fit; (ii) the payload with 35 < E−p_z < 70 GeV added; (iii) (ii)
    plus the candidate cuts x_IP < 0.04 ∧ M_X > 3 GeV; (iv) (ii) with three templates (elastic + charm, PDISS + Reggeon, ND)
    whose PDISS ratio is bounded to [0.5, 2] × the v3 ratio, the published variation range. True if toy p rises by ≥ 10×
    from (i) to any of (ii)-(iv) **and** the edge pattern of H4 is absent; false if every variant stays within 3× of (i). If
    true: the variant becomes the frozen G1′(c) fit definition (E−p_z and candidate cuts are the analysis fiducial already,
    WP-C C6; the PDISS bound is the published range, not a tuned value). The 32.5% out-of-fiducial admixture
    (`D/docs/restart_2026-10-27/wp-c/binning_record.md:190-193`) is reported per variant as the gen-x_IP ≥ 0.03 share of the
    9012 template; it is a purity fact for WP-B/WP-F, not a correction here. Cost: ≤ 16 fits of 1000 toys.
  - **H2 Random-trigger overlay rate / forward noise vs data per period.** Observables: per-run Plug, FMD and FTS fail rates
    in the deep-gap control (η_max < 1.5) from the per-run `veto_fail_rate_PLUG7` and `mean_PLUG7` columns of
    `D/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv` and the S2 per-run FMD/FTS counters,
    against run order and per-run luminosity within each period; and the per-layer FMD (`nFmuLayer[6]`) and per-station FTS
    (`nFtsLayer[16]`) fail rates, derivation split, data vs MC per sim period. True if the control excess
    (`s2_remedy1_record.md`: Plug ≈ 0.065-0.075, FMD 0.054-0.101, FTS 0.036-0.073) varies within a period by more than 3σ
    with run order or luminosity (beam-related activity absent from a 2-4-run overlay snapshot), or if the excess sits in
    specific layers/stations. False if the excess is flat in time and uniform across layers. If true: the WP-A remedy-1
    removal q per period (already derived on control) is kept as the noise correction with a per-sub-period refinement;
    this cannot repair the sub-threshold Plug shape (shown in WP-A) and bears on G1′(b) only. Cost: login-node CSV work plus
    S2 counters; run last because it cannot move G1′(a) or (c).

## G1′ text
- **D6 Gate G1′ (binding, pre-registered here; the parent gates section cites this block).** On the odd-split Stage-B N−1
  sample, stack C, signed-v3 weights (nd4cat sidecar for 7094-7097), after the frozen S3 correction set:
  (a) Plug N−1 unit-area max |MC/DATA − 1| ≤ 0.10 in bins with ≥ 20 data entries, and |f_fail,MC − f_fail,DATA| ≤ 0.02
  absolute in each D cell; (b) FMD and FTS N−1 < 0.25 for at least one admissible cocktail (non-negative weights within ±50%
  of the signed-v3 scales; the G1(b) scan grid); (c) the §7.1 fit, with the frozen template and range definition, on the
  odd-split cut-removed payload reaches template-statistics-aware toy p ≥ 0.05, coarse and fine, in every D cell and ALL,
  with ≥ 1000 toys, for at least one of 7071, 7091, nd4cat (7094 alone is excluded: it represents ep0607 only,
  `g1_verdict.md` ‡). Thresholds are G1's, unchanged, so a pass is comparable with the KILL. **Kill:** (a) or (c) fails once
  the pre-registered matrix is exhausted (every H1-H6 test run, every admissible correction applied), or when 5 Condor-days
  or 7 calendar days from the first WP-A2 Condor submission are spent, whichever comes first; Condor-days = Σ(job wall ×
  request_cpus)/24 h from the logs, tallied by `h1-naf-operator` in `D/data/restart_2026-10-27/wp-a2/condor_tally.csv`.
  (b) failing alone downgrades to a systematic leg. A KILL recommends close-out to the user; a PASS unblocks WP-E with the
  frozen correction set as the WP-B start values.
## 5. Freeze checks

- Plan sha256 at freeze: `c553d2007df95fbd78d386a96636328d6434cf21c1171b717dd9a67eb2225b0d`.
- `gate_text_frozen`: `diff <(sed -n '/^- \*\*D6/,/^- \*\*D7/p' wp-a2_plan.md | sed '$d') <(sed -n '/^## G1′ text/,/^## /p' s1_definition_audit.md | sed '1d;$d')` is empty. The plan's literal command compares the D6 block including the D7 line against the section including both headings, which can never be empty; the two `sed` trims drop exactly those lines (deviation of form, intent unchanged).
- `db_probe_complete`: `plug_calib.csv` has 22 lines (header + 21 rows), no NaN.

## 6. Binding readings of D3 (pre-registered before any S2 job; thresholds unchanged)

These readings resolve wording gaps raised by `h1-analysis-critic` in the S1 review. They add no threshold and
change no D6 text; each applies D4/D5 as written. The plan file itself is not edited (it is not executor-owned).

- R1 (H4 precedence). The edge sample is a subset of the inclusive sample, so the true-detector clause and the
  false clause can both hold. If the true-detector clause is met, it overrides the false clause. The true-physics
  clause is read only when the true-detector clause is not met.
- R2 (H2 derivation split). Any per-run or per-sub-period refinement of the noise removal q uses only the
  even-split S2 per-run counters (`wpa2.per_run`, `wpa2.control`); the all-parity `veto_fail_rate_PLUG7` /
  `mean_PLUG7` columns of the certified run list are shown as context and size nothing (D4). Sub-period boundaries,
  if any, are fixed in S3 from even-split data. Remedy-1 removal changes the Plug f_fail, so an adopted H2
  correction enters G1′(a) as well as (b); the plan's "bears on G1′(b) only" is read as "cannot repair the
  sub-threshold Plug shape".
- R3 (H3 floor and choice). "Toy p rises by ≥ 10×" is evaluated with p quoted as (count+1)/(n_toys+1), n_toys = 1000,
  so from a floor baseline (0.0010) the criterion is p ≥ 0.010 in every D cell and ALL, coarse and fine, for the same
  ND alternative. "The edge pattern of H4 is absent" means "H4 true-detector not supported". If several of (ii)-(iv)
  pass, the frozen variant is the one with the fewest added elements in the order (ii), (iii), (iv); ties are not
  broken by odd-split information.
- R4 (H3 iv bound provenance). The PDISS-to-elastic ratio range 0.5-2.0 is the published variation of
  arXiv:1203.4495 §4 ("variations in the assumed ratio of proton dissociation to elastic proton cross sections in
  the range 0.5 to 2.0", l.644-647). The bound is fixed; the ratio inside it is a fit parameter on both splits, as
  D5 admits for a published-range template bound.
- R5 (H1 branches). The size of the 3.15-3.35 excess and of the 3.40-3.55 deficit is the integrated unit-area
  (data − MC) over each window, MC normalised in 3.5 ≤ η < 5 as D3 states. A geometry-only H1 result (Δη median
  shift ≥ 0.03, threshold pattern invariant) is not corrected inside H1, because `etaMaxVtx08` is −999 for every ND
  sample; it is handed to H6 (hatVtxZ reweight). Only the response branch adopts the 800 MeV / 3.3 definition.
- R6 (H6 wording). The Δη test runs on 9012 only (6671/6677 carry no vertex η); 6671/6677 enter H6 through their
  `hatVtxZ` distributions only.
- R7 (H5). With EnCor = 1 and Thresh equal everywhere (§3), H5(i) fails and the correction (× EnCor = × 1) is the
  identity; observable (ii) is still filled and reported as the G1′(a) baseline and as a check on the snapshot caveat.
- R8 (D2 note). The 800 MeV in the implementation is a per-cluster (per calorimeter part) energy cut, not a
  per-deposit cut; the published wording does not distinguish the two. D1 also requires |z_vtx| < 35 cm, which A_pre
  applies (`n1_wpa.py`, `np.abs(hatVtxZ)<35`).

## 7. Sign-offs

- `h1-mc-expert` (2026-10-27): SIGNED yes. The D2 pointers and the §3 DB reading match the H1OO 4.2.1 source and
  snapshot; the rescale would be MC-only (EnCor > 1 lowers the MC Plug energy by 1/EnCor and raises the effective raw
  cell threshold to Thresh·EnCor); with EnCor = 1 there is none, and data and MC share the 0.3 GeV cell cut. H5(i)
  is false. The 4.1.1 snapshot is md5-identical to 4.2.1; the 3.4.15, 3.4.16, 4.0.6, 4.0.9, 4.0.23 and 4.0.24
  snapshots hold the same two-entry H1CalibPlug validity structure (runs from 76137 and from 303906), but their
  payloads cannot be decoded with the 4.2.1 libraries (schema change). Residual risk, judged low: the production-time
  PCLA payload is not decoded; no production record with other values was found.
- `h1-analysis-critic` (2026-10-27): round 1 SIGNED no (H4 precedence; H2 test-split leak and G1′(a) effect);
  round 2 SIGNED yes after §6 R1-R8, on condition that the sha256 of this record (with §6) is frozen and quoted in the
  S2 and S3 submit logs before the first S2 submission. `wpa2.control` exists in `n1_wpa2.py` (R2 wording kept).
