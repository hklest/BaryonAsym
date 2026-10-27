# WP-A2 child plan: root-cause diagnosis of the η_max / Plug failure and gate G1′

Parent: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(WP-A2 block, gate G1′, Scope Notes bind). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`,
`P` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`,
`H1OO` = `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo`.
Output roots: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a2/`. WP-A inputs: `D/docs/restart_2026-10-27/wp-a/`
(records), `D/data/restart_2026-10-27/wp-a/` (partials, `period_map.csv`, `condor_tally.csv`),
`D/scripts/restart_2026-10-27/wp-a/` (`n1_wpa.py`, `payload_wpa.py`, `fit_7p1_wpa.py`, `g1_eval_wpa.py`, `wpa_common.py`).
Decision-and-outline only; `executor` writes the code.

## Goal

Decide, from ≤ 6 pre-registered hypotheses tested on the existing v6dif trees, whether the G1 failure
(`D/docs/restart_2026-10-27/wp-a/g1_verdict.md`: Plug N−1 MC/data 1.35 at 0 GeV and 0.29-0.44 across 1-7 GeV; FMD/FTS N−1
0.68-0.92 best cocktail; §7.1 η_max fit at the toy floor for 7071, 7091, 7094 and nd4cat, D/ndf ≈ 5900-6500/358) is
definitional (η_max convention, Plug threshold and scale, overlay noise, fit structure, simulation flags) or physical, fix any
justified definitional correction from control material, and rule gate G1′ on a disjoint test split. Done when
`D/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md` exists with a PASS or KILL ruling by `h1-analysis-critic`.

Non-goals: new SimRec or any MC request; writer (`v6dif`) changes; FPS/VFPS or proton tagging; generator retuning; any fit
whose parameters are tuned on the test split; changing G1′ thresholds after S1; editing the parent plan or `.supervisor/memory/`.

Expected change size: four parameterised forks of the WP-A scripts (`n1_wpa2.py`, `payload_wpa2.py`, `fit_7p1_wpa2.py`,
`g1prime_eval_wpa2.py`, ≈ 400 lines of Python in total), one H1OO macro of ≈ 60 lines (`plugdb_probe.C`), one CSV join
helper, two `.sub` files with queue lists, ≈ 12 JSON/CSV products, five Markdown records, one plot set. No ROOT trees are
written. Compute: the whole WP-A chain used 0.027 Condor-days (175 jobs); WP-A2 adds one 17-file diagnostic pass (≈ 0.01
Condor-days), one payload pass and ≤ 48 fits of 1000 toys (≈ 4 min each): ≈ 0.2 Condor-days against a 5 Condor-day cap.

## Approach

Pre-register, measure on the derivation split, correct, then gate on the test split. S1 freezes the hypothesis matrix and
the G1′ text, audits the implemented definitions against the published ones and reads the Plug calibration database for data
and MC runs. S2 runs one Condor pass over the 17 merged files that fills every discriminating histogram of H1-H6 at once, on
both splits, so no hypothesis is tested twice. S3 derives, from the derivation split and the published reference only, the
definitional corrections the S2 outcomes justify. S4 applies the frozen correction set and evaluates G1′ (a)-(c) on the test
split. S5 rules. The split is by `EventNumber` parity over the whole A_pre sample (even = derivation, odd = test), so control
and test are disjoint by construction and the test population is the Stage-B population itself.

Why this ordering: the WP-A records already localise the §7.1 failure (η_max 3.2-3.5 carries ≈ 52% of D; data above the
fit at 3.15-3.35 by 13-39% and below it at 3.40-3.55 by ≈ 44%, computed from
`D/data/restart_2026-10-27/wp-a/s3_gof/fits/per_bin_data_prediction_deviance_7071_fine_ALL.csv`) and show it is identical for
RAPGAP ME+PS (7071), CDM (7091) and the four-catalogue mix (`g1_verdict.md`, G1(c) table). A feature at the LAr forward edge
that two generator classes share points first at the detector-level definition of η_max and the forward response, which the
cheap tests (H1, H5, H6, H4-edge) address before any composition or physics argument (H3, H4-flow, H2).

Rejected alternatives (one line each):
- Full MC-response campaign (forward-detector response matrices, new SimRec): not chosen by the user; days-scale diagnosis first.
- Reproduce the published η_max control plots: EPJC 72 (2012) 2074 (arXiv:1203.4495) shows none, and its HERA-II LRG sample
  covers Q² 10-105 GeV² only (its Table 1); Q² > 150 GeV² there comes from the 1999-2000 data of EPJC 48 (2006) 715. The plan
  reproduces the published *definitions* and noise method instead (D1).
- Re-use the WP-A control/test split (deep gap ∧ even vs complement): the test side then contains even-parity events outside
  the deep gap, which S3 would use for the edge-geometry derivation; a parity-only split keeps derivation and test disjoint.
- Procure the random-trigger pools and re-derive the overlay rate: FPACK files need libdcap/ODS tooling absent on Alma9 and
  carry no forward-detector pool (`D/docs/restart_2026-10-27/wp-a/s1c_mc_expert_lookup.md:6-13`); H2 is tested on run-level
  data proxies instead.
- Add shape nuisances to the §7.1 fit: tuning on the test sample; the three-template structure test (H3) runs on the
  derivation split only and enters G1′ as a frozen template choice, never as a free parameter.

## Decisions

- **D1 Published reference = definitions, not plots.** The reference definitions are those of arXiv:1203.4495 §3.3:
  "The pseudorapidity η_max of the most forward energy deposit in the LAr calorimeter above a noise threshold of 800 MeV is
  required to be less than 3.3"; "At most one hit pair should be present in the first two layers of the FMD"; "The energy
  measured in the Plug calorimeter is required to be smaller than 7 GeV"; FTS "no hits in the 26 m and 28 m stations"; noise
  losses "determined using randomly triggered events which are overlaid with simulated events"; Σ(E−p_z) > 37 GeV; |z_vtx| <
  35 cm; and the quoted forward-detector uncertainties (FMD efficiency 4%, FTS 7%, Plug energy scale 10%, noise energy 10%).
  The HERA-II dijet paper (arXiv:1412.0928) uses η_max < 3.2 with the same 800 MeV and the 28 m FTS station only. Vertex vs
  nominal-origin η and electron exclusion are not stated in either paper. S1 records this table; no published control plot
  is claimed as a target.
- **D2 Implemented definitions (audit baseline).** `etaMaxClus08` is the HAT `EtaMaxClus[1]`: E_final cell sums per cluster
  over `MixedClusters` + `SpacalClusters`, η of the energy-weighted centre relative to the origin (z = 0), E > 0.8 GeV, no
  electron exclusion (`H1OO/H1HatFiller/H1HatDiffractiveFiller.C:100-147`; `EtaMaxClus[0]` is the 0.4 GeV variant, stored as
  `etaMaxClus04`). `ePlug` is the HAT `HfsClusPlugE`: for HERA-II runs each Plug cell passes a run-dependent database threshold
  `H1CalibPlug::GetThresh()` and, **for MC only**, the cell energy is divided by `H1CalibPlug::GetEnCor()` before the
  threshold and in the sum (`H1OO/H1HatFiller/H1HatCaloFiller.C:272-292`). Neither can be recomputed from the ND trees
  (HAT+MODS, no cells, `D/docs/restart_2026-10-27/wp-d/mc_gap_list.md:85-100`); the vertex-corrected
  `etaMaxVtx08`/`etaMaxVtxExcl08` exist only for ODS samples (9012, 9024, 9033, 9155, 9157, 9158, 9013) and data.
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
- **D4 Split rule.** Derivation split = even `EventNumber`, test split = odd, over the whole A_pre sample of every file,
  data and MC. The parity basis is `EventNumber` after the (Run, Event) uniqueness check of `n1_wpa.py:173-176`; the fork
  replaces that check's entry-index fallback by an abort (duplicate keys fail closed), and the disjointness check of
  `:183-190` is kept. Every number used to choose or size a
  correction comes from the even split or the published reference; G1′ is evaluated once on the odd split.
- **D5 Correction admissibility.** A correction enters the frozen set only if (i) its hypothesis is supported by its D3
  criterion, (ii) it is a definition (threshold, cut value, η convention, fit range, fiducial cut, DB rescale, vertex
  reweight, published-range template bound) and not a free parameter, and (iii) its value is fixed in
  `s3_correction_record.md` with its derivation-split evidence before any S4 job is submitted. The record's sha256 is
  quoted in the S4 submit log. Changes that alter the physics scope are not corrections: abandoning the Plug veto, an x_IP
  window beyond 0.03, a new MC request (`user:`; Open Questions Q1-Q2, x_IP belongs to WP-B).
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
- **D7 ND alternatives.** 7071 (RAPGAP ME+PS, em06 sim), 7091 (CDM, ep0607 sim) and nd4cat (7071 + 7094 + 7096 + 7097);
  7094 alone only as a diagnostic in the H1/H4 tables. No new sample.
- **D8 Normalisation in the no-gap control.** MC is normalised to data per D cell within the H4 sample (shape test). The
  H4 sample's selection is the parent's (Q²_es > 150, 0.1 < y < 0.9, 35 < E−p_z < 70, |z_vtx| < 35,
  `P/src_stage1/PhysicsConstants.h:36-39,65-67`), so no parent tree is read.
- **D9 Reuse, no new machinery.** All histograms come from one fork of `n1_wpa.py` with additive blocks (identity test
  against the WP-A partials with default flags), the payload from a fork of `payload_wpa.py` with `--empz`, `--cand`,
  `--eta-range`, `--split` flags, the fit from a fork of `fit_7p1_wpa.py` with `--templates 2|3`, `--pdiss-bound`, and the
  gate from a fork of `g1_eval_wpa.py`. The WP-A originals stay untouched (md5 list in `t4_originals_md5.txt` extended).

## Outline

- `D/scripts/restart_2026-10-27/wp-a2/plugdb_probe.C` (new, H1OO macro, login node): for a list of run numbers, print
  `H1CalibPlug::GetThresh()` and `GetEnCor()` via `H1DBManager::Instance()->GetDBEntry(H1CalibPlug::Class())` after
  `H1Tree::Instance()->SetRunNumber`-equivalent positioning on one event of one file per sample; pattern
  `H1OO/H1HatFiller/H1HatCaloFiller.C:243-244, 278-279`. Output CSV `run, period_or_sample, is_mc, thresh, encor`. Gotcha:
  the DB entry is run-dependent; probe the first and last run of each data period and each MC sim run (sim `RunNumber` from
  the tree, e.g. 398376 for the 04_05 stream).
- `D/scripts/restart_2026-10-27/wp-a2/n1_wpa2.py` (fork of `n1_wpa.py`): new flags `--split even|odd|all` (parity over
  A_pre, replacing the deep-gap control definition at `:183-190` when set), `--empz-join <ChargedTot path>`; new additive
  partial blocks `wpa2.{eta_thr04, eta_thr08, deta_vtx, vtxz, eplug_fine, edge_plugtag, incl_nogap, fmd_layers, fts_stations,
  per_run}`; η_max edges as `ETA_EDGES` (`:105-110`), `ePlug` fine edges 0-10 GeV / 0.1. Invariant: default flags reproduce
  the WP-A `partial.json` byte-for-byte on `9155.root`.
- `D/scripts/restart_2026-10-27/wp-a2/payload_wpa2.py` (fork of `payload_wpa.py:1-8` basis): flags `--split`, `--empz`,
  `--cand` (x_IP < 0.04 ∧ M_X > 3), `--eta-max-cut 3.4 --edge-merge` (fit range correction of H4), `--eta-branch
  etaMaxClus04|etaMaxClus08`.
- `D/scripts/restart_2026-10-27/wp-a2/fit_7p1_wpa2.py` (fork of `fit_7p1_wpa.py:15` (`DIFF` tuple), `:22` (`fit`)): `--templates 3` splits
  `DIFF` into {9012, 9024, 9033} and {9155, 9157, 9158, 6671, 6677} with the second's scale bounded to [0.5, 2] × its v3
  ratio to the first; toy method unchanged (`toy_gof`, `:83-94`).
- `D/scripts/restart_2026-10-27/wp-a2/g1prime_eval_wpa2.py` (fork of `g1_eval_wpa.py`): reads the frozen correction set
  from `s3_correction_record.json`, applies the Plug rescale and vertex reweight to the odd-split partials, emits
  `g1prime_table.csv` with clauses (a)-(c) per ND alternative.
- `D/condor/restart_2026-10-27/wp-a2/diag_wpa2/diag_wpa2.sub` and `fit_wpa2/fit_wpa2.sub`: copies of the WP-A `.sub` files
  (1 CPU; per-sample memory, runtime and disk as in the `queue_s1.txt` columns); queue files per slice.
- Data shapes: `s3_correction_record.json` = list of `{hypothesis, supported: bool, correction: {kind, value, sample_scope},
  evidence: path}`; `g1prime_table.csv` columns `alt, clause, cell, value, limit, pass`.

## Slices

Specialists: `h1-naf-operator` validates every Condor submission, log and the tally; `h1-mc-expert` validates the DB
semantics (S1), the overlay/noise reading (S2 H2, H5) and the generator-class argument (S2 H4); `h1-analysis-critic` signs
the pre-registration (S1), the correction record (S3) and rules the verdict (S5). `h1-data-validator` is not needed: no
merged file is produced (the 17 inputs are the validated WP-A set); if any slice writes a merged file it is validated
before use. Condor for every multi-file step; login node only for the one-file DB macro and numpy-on-JSON/CSV steps.

### S1. Definition audit, DB probe and pre-registration freeze
Delivers: the published-vs-implemented definition table, the Plug DB values per data period and MC sample (H5 observable
(i)), and the frozen hypothesis matrix and G1′ text.
Inputs: D1-D3, D6; `H1OO` sources cited in D2; one event of one file per sample from the 17-file queue
`D/condor/restart_2026-10-27/wp-a/n1_wpa/queue_s1.txt`; `period_map.csv`.
Units: (a) `plugdb_probe.C` and its CSV; (b) `s1_definition_audit.md`.
Runs: login node only (one file per invocation, < 5 min each; ≤ 17 invocations spread as needed).
Outputs: `D/data/restart_2026-10-27/wp-a2/s1_db/plug_calib.csv`;
`D/docs/restart_2026-10-27/wp-a2/s1_definition_audit.md` (D1 table, D2 pointers, DB table, the D3 matrix, a `## G1′ text` section quoting D6
verbatim, sha256 of the plan at freeze).
Tests:
- `db_probe_complete`: rows for 2 runs × 4 data periods + 1 run × 13 MC samples → 21 rows, no NaN.
- `gate_text_frozen`: D6 text in the audit record → identical to this plan's D6 (diff empty).
- `definitions_cited`: every D1/D2 claim → carries a paper section or `file:line` that `sed -n` confirms.
Checkpoint: `diff <(sed -n '/^- \*\*D6/,/^- \*\*D7/p' wp-a2_plan.md) <(sed -n '/^## G1′ text/,/^## /p' s1_definition_audit.md)`
is empty and `test $(wc -l < plug_calib.csv) -eq 22` (header + 21 rows); then `h1-mc-expert` signs the DB-value reading
(what `EnCor ≠ 1` means for the simulated Plug scale) and `h1-analysis-critic` signs the matrix as complete and
non-tunable; the first Condor submission is allowed only after both.

### S2. Diagnostic pass (one Condor sweep, both splits)
Delivers: every H1-H6 histogram and counter on the even split (H3 fits excluded), plus the same blocks on the odd split for
S4, and the per-hypothesis readings against the D3 criteria.
Inputs: the 17 files, v3 scale tables and nd4cat sidecar, the frozen run list, `ChargedTot` trees for the E−p_z join
(data and MC paths per `empz_reconstructibility_2026-10-18.md`).
Units: (a) `n1_wpa2.py` + `diag_wpa2.sub` (17 files × 2 splits = 34 jobs); (b) `s2_analyze_wpa2.py` (numpy on JSON: ratios,
Δη medians, edge residuals, per-layer tables; login node); (c) per-run H2 tables from the run-list CSV (login node).
Runs: Condor for (a); login node for (b), (c).
Outputs: `D/data/restart_2026-10-27/wp-a2/s2_diag/partials/<label>/<split>/partial.json`, `s2_diag/analysis/*.json`;
`D/docs/restart_2026-10-27/wp-a2/s2_diagnostic_record.md` with one subsection per hypothesis (criterion, numbers, reading);
`D/plots/restart_2026-10-27/wp-a2/s2/` (η_max edge overlays per threshold, Δη, `ePlug` fine spectrum, edge-sample
overlays, per-layer rates).
Tests:
- `fork_default_identity`: `n1_wpa2.py` default flags on `9155.root` → `partial.json` equal to
  `D/data/restart_2026-10-27/wp-a/s1_split/partials/all/9155/partial.json` (counts, hists, dcell_eta).
- `split_disjoint_complete`: even ∩ odd (Run, Event) keys → empty per file; even + odd counts → equal the WP-A `all` counts
  per mask and D cell.
- `empz_join_coverage`: fraction of A_pre events with a `ChargedTot` match → ≥ 0.999 per file (else the file is listed
  and H4 runs on the matched subset with the fraction quoted).
- `edge_localisation_reproduced`: even-split baseline two-template residual pattern → data/fit > 1 in [3.15, 3.35) and < 1 in
  [3.40, 3.55) in D0 (the WP-A localisation, before any correction).
- `h4_control_is_inclusive`: ND share of the H4 sample under the 7071 cocktail → ≥ 0.90 in every D cell.
Checkpoint: `s2_analyze_wpa2.py --root D/data/restart_2026-10-27/wp-a2/s2_diag` prints a supported/not-supported line for
H1, H2, H4, H5, H6 with the criterion value and exits 0; `h1-naf-operator` appends the S2 line to `condor_tally.csv`;
`h1-mc-expert` validates the H2/H5 readings.

### S3. Correction derivation and H3 structure test (derivation split only)
Delivers: the frozen correction set and the frozen G1′(c) fit definition.
Inputs: S2 readings; `plug_calib.csv`; even-split payloads.
Units: (a) `payload_wpa2.py` even-split payloads for the H3 variants (i)-(iv) and for the H1/H4 definitional variants
(threshold, `--eta-max-cut 3.4 --edge-merge`); (b) `fit_7p1_wpa2.py` via `fit_wpa2.sub` (≤ 16 fits × 1000 toys);
(c) `s3_correction_record.md` + `.json` (D5 fields), `h1-analysis-critic` sign-off.
Runs: Condor for (a) when it touches > 1 file and for (b); login node for (c).
Outputs: `D/data/restart_2026-10-27/wp-a2/s3_corr/{payloads,fits}/`, `s3_corr/s3_correction_record.json`;
`D/docs/restart_2026-10-27/wp-a2/s3_correction_record.md` (per hypothesis: supported?, correction, value, evidence path;
the H3 fit table; the gen-x_IP ≥ 0.03 share per variant for WP-B/WP-F).
Tests:
- `corrections_are_definitions`: every `correction.kind` → in {threshold, cut, eta_convention, fit_range, fiducial_cut,
  db_rescale, vtx_reweight, template_bound}; none carries a value fitted on the test split.
- `h3_variants_complete`: fits (i)-(iv) × {7071, 7091, nd4cat} → 12 rows with p, D/ndf, coarse and fine.
- `pdiss_bound_respected`: in variant (iv) the PDISS+Reggeon scale ÷ elastic scale → within [0.5, 2] × v3 ratio.
- `record_frozen_before_s4`: sha256 of `s3_correction_record.json` → quoted in the S4 submit log before any S4 job.
Checkpoint: `h1-analysis-critic` signs that each adopted correction meets D5 and that no `user:` item (D5) was applied.

### S4. G1′ evaluation on the test split
Delivers: the G1′ (a)-(c) numbers on the odd split with the frozen set applied.
Inputs: odd-split S2 partials, `s3_correction_record.json`, odd-split payloads with the frozen definition.
Units: (a) `payload_wpa2.py --split odd` with the frozen flags (Condor); (b) `fit_7p1_wpa2.py` 8 fits × ≤ 3 ND
alternatives × 1000 toys (Condor); (c) `g1prime_eval_wpa2.py` (login node) → `g1prime_table.csv`; (d) the G1′(b) cocktail scan
with the G1(b) grid (login node or one Condor job).
Runs: Condor for (a), (b), (d) if > 5 min; login node for (c).
Outputs: `D/data/restart_2026-10-27/wp-a2/s4_g1p/{payloads,fits,g1prime_table.csv}`;
`D/docs/restart_2026-10-27/wp-a2/s4_g1prime_record.md`; `D/plots/restart_2026-10-27/wp-a2/s4/`.
Tests:
- `single_evaluation`: the odd-split payload and fit queue → submitted once; any resubmission is logged with reason and the
  first result stands.
- `clauses_complete`: `g1prime_table.csv` → (a) per D cell, (b) best admissible cocktail, (c) coarse and fine × 4 cells ×
  each alternative, no missing row.
- `toy_floor_reported`: every p → quoted with n_toys; p at floor marked.
- `tally_within_cap`: `condor_tally.csv` total → ≤ 5 Condor-days and calendar days ≤ 7 from the first S2 submission.
Checkpoint: `g1prime_eval_wpa2.py` prints PASS/FAIL per clause and alternative and exits 0; `h1-naf-operator` signs the tally.

### S5. Verdict
Delivers: `g1prime_verdict.md` and the lines the parent plan needs.
Inputs: S1-S4 records.
Units: (a) the verdict record (D6 text verbatim, numbers, deviations from pre-registration, the frozen correction set as
WP-B start values on PASS, close-out recommendation on KILL); (b) the parent `## Log` line and the `## Todo` ticks, returned
in the lead report (the parent is edited by `fixer`, not here).
Runs: login node.
Outputs: `D/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md`.
Tests:
- `threshold_pre_registered`: thresholds in the verdict → identical to D6 and to the parent gates section.
- `ruling_consistent`: ruling → PASS iff (a) and (c) pass for ≥ 1 alternative; (b) fail alone → "PASS with systematic leg".
- `user_items_listed`: every D5 `user:` change the diagnosis suggests → listed under open items, none applied.
Checkpoint: `grep -c '^\*\*Ruling: \(PASS\|KILL\)' g1prime_verdict.md` → 1; records S1-S4 cited with their sha256.

## Verification

End to end: `g1prime_verdict.md` cites the frozen `s3_correction_record.json` (sha256 matching the S4 submit log), the
`g1prime_table.csv` with every clause row, the `condor_tally.csv` within the D6 cap, and a per-hypothesis table
(supported / not supported / not run, with the criterion value); the WP-A2 scripts with default flags reproduce the WP-A
partials; and `h1-analysis-critic`'s ruling is PASS or KILL with no undecided clause.

## Risks

- H5 true but uncorrectable for ND samples: the DB rescale is applied to the stored `ePlug` scalar, which is the
  post-threshold sum; cells already cut by the MC-side threshold cannot be restored. Mitigation: G1′(a) is then evaluated on
  `ePlug` ≥ 1 GeV with the 0 GeV bin reported separately; if the residual still fails, KILL stands and the verdict names the
  Plug HAT treatment as the cause.
- H4 true-detector with the edge pattern also present in the diffractive template: the fit-range correction removes
  the most discriminating ND bins and G2's leakage uncertainty grows; recorded as a WP-F input.
- The odd split halves the data: G1′(a)'s ≥ 20-entry rule and the coarse ≥ 100-event merging drop bins; the calibrated
  toys keep (c) valid, but a borderline pass carries wider intervals. Recorded, not tuned.
- The E−p_z join misses events (`ChargedTot` written for a subset): the `empz_join_coverage` test bounds it.
- Several hypotheses partially true: corrections are applied in the D3 order and each one's residual is re-read on the
  derivation split before the next is sized, so no correction absorbs another's effect twice.
- Calendar cap: specialist sign-offs in S1 and S3 are on the critical path; the clock starts at the first Condor submission.

## Scope Notes

In: S1-S5 on the 17 existing merged files and the `ChargedTot` join. Out: new SimRec or MC requests; writer changes;
proton tagging; FPS/VFPS; close-out work; any parent-plan edit (returned as lines); editing `.supervisor/memory/`. User
decisions `executor` must not reopen: root-cause diagnosis with gate G1′ rather than a full campaign, close-out or proton
tagging (user 2026-10-27, parent `## Log`); the observable pivot and Q1-Q5 answers of the parent; G1 thresholds reused
unchanged in G1′. Memory: no `.supervisor/memory/` entry binds this package (searched 2026-10-27 for diffractive, Plug,
η_max, overlay, noise, LRG; only `decisions/2026-10-26-no-diffractive-term-or-mixed-response.md`, a parent-analysis
decision, matched). Workspace rules per `CLAUDE.md`; `.sub` files under `D/condor/restart_2026-10-27/wp-a2/`.

## Assumptions

- The H1OO database is reachable from the login node with the standard `thish1.sh` setup and returns `H1CalibPlug` entries
  for both data and MC run numbers (checked by `db_probe_complete` in S1; if MC runs return no entry, the probe records the
  fallback `Thresh = 0, EnCor = 1` the filler uses and H5(i) reads "no MC-side rescale").
- `ChargedTot` exists for all 17 files with (RunNumber, EventNumber) keys (checked by `empz_join_coverage`).
- `EventNumber` is unique per (Run, Event) in every file (checked by the inherited uniqueness test; WP-A found it unique in all 17).
- The WP-A partial layout (`wpa.*` blocks) is stable, so additive blocks keep `fork_default_identity` (checked in S2).

## Open Questions

Material items only; each is a D5 `user:` change and is not applied in this package.
- **Q1 (user:) Plug veto abandoned permanently.** If H5 is true and uncorrectable (Risks, first item), options: (a) keep the
  veto and KILL; (b) replace the Plug veto by η_max + FMD + FTS with a Plug-free leakage model (changes the LRG definition
  against EPJC 72). Recommendation: decide only after the S5 ruling.
- **Q2 (user:) New ND MC request.** If H4 is true-physics (both ND classes wrong in forward flow): options: (a) close out;
  (b) request a DJANGOH/ARIADNE high-Q² ND SimRec with ODS. Recommendation: (a) unless the H4 flow discrepancy is < 20% and
  shape-only, in which case (b) with the WP-D request list.

## Log

(empty; `fixer` appends lead and decision lines)

## Todo

- [x] S1 `plugdb_probe.C` run; `plug_calib.csv` 21 rows; `s1_definition_audit.md` with D1-D3, D6 verbatim; both sign-offs
- [x] S2 `n1_wpa2.py` identity test PASS; 34 diagnostic jobs rc 0; `s2_diagnostic_record.md` with H1, H2, H4, H5, H6 readings
- [x] S3 even-split payload variants and ≤ 16 fits; `s3_correction_record.{md,json}` signed; sha256 recorded
- [x] S4 odd-split payloads and fits submitted once; `g1prime_table.csv` complete; tally ≤ 5 Condor-days / 7 days
- [x] S5 `g1prime_verdict.md` with the `h1-analysis-critic` ruling; parent Log and Todo lines returned
