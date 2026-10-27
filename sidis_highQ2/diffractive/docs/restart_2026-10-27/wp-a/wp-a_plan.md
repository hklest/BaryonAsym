# WP-A child plan: Plug forward-veto probe and gate G1

Parent: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(WP-A block, D2, D3, G1, Scope Notes bind). `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.
Output roots (brief): `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a/`.
Decision-and-outline only; `executor` writes the code.

## Goal

Attribute and correct the Plug veto deficit (data Plug fail fraction 0.207/0.230/0.283 in D0/D1/D2
versus ≤ 0.146 in any MC component, `D/docs/t4/localization_verdict_2026-10-20.md:46,52`) with the
three pre-registered remedies in order, re-run the Stage-B N−1 and the §7.1 η_max two-free GOF on
the rebuilt payload, and record the G1 ruling in
`D/docs/restart_2026-10-27/wp-a/g1_verdict.md`.

Non-goals: new MC, writer changes, FPS/VFPS, generator retuning, any change to G1 thresholds, any
FMD/FTS remedy beyond the combined stack probability of D2, editing the parent plan or
`.supervisor/memory/`.

Expected change size: four parameterised forks of existing t4 scripts plus one new derivation script
(≈ 500 lines of Python in total), three `.sub` files with queue lists, ≈ 8 JSON/CSV products, five
Markdown records. No ROOT trees are written. Compute: the whole t4 chain ran in ≈ 35 s wall
(14 N−1 jobs) plus 28 short builder jobs; the 1000-toy fits add ≈ 4 min each. WP-A as planned uses
≈ 0.1 Condor-day of the 10 Condor-day cap; the two-calendar-week clock is the binding limit.

## Approach

Measure first, correct second, gate third. S1 splits the frozen data Plug fail fraction by true
period and run range and defines the deep-gap control region (D2) with an event-parity split that
makes control and test sets disjoint by construction. S2 derives one combined forward-stack
removal probability per period from the control region, applies it to MC, and re-runs the N−1
chain. S3 rebuilds the cut-removed η_max payload with the same correction and runs the two-free fit
with ≥ 1000 toys. S4 runs the threshold scan and the no-Plug-veto variant only if remedy 1 fails
G1. S5 writes the verdict. Every pass reuses the t4 scripts through thin forks that add flags; no
cut, weight or binning changes beyond the flags.

Rejected alternatives:
- Derive the noise rate from the Stage-B sample itself: circular; D2 requires control/test disjoint.
- Stratify control (η_max < 1.5) and test (1.5 ≤ η_max < 3.2) by η_max instead of event parity:
  disjoint, but the test population then differs from the pre-registered Stage-B N−1 sample in
  composition (ND/PDISS leakage grows with η_max), confounding the noise test with composition.
- Procure the H1 random-trigger noise pools the MC overlay used: pnfs unreliable
  (`AGENTS.md`), and D2 settled on the control region; kept as the one `h1-mc-expert` lookup.
- Fit the noise rate as a nuisance in the §7.1 fit: tuning on the test sample, forbidden by D3.

## Decisions

- A0 **Gate text (binding, verbatim from the parent plan lines 142-155).** Every PASS/FAIL in the S2-S5
  records is ruled against this block and nothing else:

  > - **G1 (WP-A, kill gate).** On the Stage-B N−1 sample, stack C, signed-v3 weights, after the
  > nominal remedy: (a) Plug N−1 unit-area max |MC/DATA−1| ≤ 0.10 in bins with ≥ 20 data entries,
  > and |f_fail,MC − f_fail,DATA| ≤ 0.02 absolute in each D cell (data fractions 0.207/0.230/0.283
  > carry ≤ 0.007 stat; 0.02 ≈ the 10% PASS window of
  > `D/docs/t4/datamc_fail_diagnosis_2026-10-19.md:10` applied to a rate); (b) FMD and FTS N−1 < 0.25
  > (below the pre-registered FAIL boundary) for at least one admissible cocktail (non-negative weights within
  > ±50% of the signed-v3 scales); (c) the §7.1 two-free-template η_max fit on the rebuilt cut-removed
  > payload reaches template-statistics-aware toy p ≥ 0.05, coarse and fine, with ≥ 1000 toys, for at
  > least one ND alternative (the conventional 5% level; calibrated toys make a χ²/ndf ceiling
  > redundant; the prior fits sat at the 300-toy floor p ≤ 0.0033-0.01, so a pass is a ≥ 5-15× change,
  > not a marginal one). **Kill:** the WP-A budget (Open Q1; default 10
  > Condor-days, 2 calendar weeks, whichever comes first; Condor-days = Σ(job wall × request_cpus)/24 h
  > from the Condor logs, tallied by `h1-naf-operator` in the WP-A Log) is spent with (a) or (c) failing. (b) failing alone downgrades to a
  > systematic leg, not a kill.

- A1 **Periods.** Four true periods from the data files (ep0304, em0405, em06, ep0607;
  `D/data/t2_detector/merged_data_true_2026-10-18/README.md`), with ep0607 further split at
  run 492560 (fall-06 / 2007, `D/docs/t2/run_period_unmixing_2026-10-18.md:30-31`) as a
  diagnostic. The removal probability is per true period (4 values); it becomes per sub-period
  (5 values) only if the ep0607 halves differ by more than 2σ in S1. Reason: the MC noise overlay
  is run-number dependent through the `ANOF.<period>` cards
  (`D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md`, §0), so the excess can be
  period dependent; finer splits are statistics limited (data fractions carry ≤ 0.007 stat pooled).
- A2 **Control region (D2 instance).** `A_pre` (`build_localization_n1_mytbl_2026-10-20.py:113`)
  ∧ (`etaMaxClus08` < 1.5 or sentinel < −900) ∧ reco `xIp` < 0.04 ∧ reco `mx` > 3 GeV (D4 start
  values) ∧ `EventNumber` even (entry parity if `EventNumber` is not unique per run). Stack C cluster-η convention for every component (`:141`). Reason:
  the deep gap suppresses physics forward energy; the parity rule gives disjointness without
  changing the composition of the test sample. Both data and MC use the same rule.
- A3 **Test sample.** The Stage-B N−1 sample (`:143-147`) minus the control events, i.e. all events
  except even-`EventNumber` deep-gap events. Reason: D2 itself tests "on the disjoint Stage-B N−1
  sample" (parent D2), so excluding the control events is D2's wording, not a gate change; the
  parity rule keeps the test composition equal to the full sample. G1 thresholds are unchanged;
  the pre-registered fractions 0.207/0.230/0.283 are reproduced on the full frozen sample (test
  `fail_fraction_recomputed`) before the split.
- A4 **Noise quantity.** Per period, on the control region: f_stack = 1 − N(pass FMD ∧ Plug<7 ∧
  FTS)/N(control), for data and for the stack-C signed-v3 cocktail (9012, 9024, 9033, 9155, 9157,
  9158, 6671, 6677 plus one ND alternative, scales from
  `D/docs/mc/composite_scale_table_v3_2026-10-20.csv`). p_period = max(0, f_stack,data −
  f_stack,MC). The sample set is the 14 files of the t4 queues: data ep0304, em0405, em06_true,
  ep0607_true; MC 9012, 9024, 9033, 9155, 9157, 9158, 6671, 6677, 7071, 7091
  (`D/condor/t4/localization_diag_2026-10-20/localization_diag_queue.txt`). Per-detector excesses (Plug, FMD, FTS) and the η_max-stratified excess (η_max bins
  [−1,0), [0,1), [1,1.5)) are diagnostics only. If p_period ≤ 0 within 1σ in every period,
  remedy 1 is recorded as inapplicable and S4 starts. Reason: D2 verbatim; the official MC already
  overlays random-trigger noise (`RT_NOISE=Y`, overlay doc §0), so only the excess is applied.
- A5 **Application to MC, and the shape clause.** Each MC event in the test sample with run-mapped
  period k is dropped from the Stage-B (veto-passing) class with probability p_k, seeded per
  (sample, period). The dropped events are not discarded from the N−1 Plug histogram: their
  `ePlug` value is replaced by a draw from the per-period control-region excess spectrum
  (data − MC unit-lumi `ePlug` N−1 histogram, `EDGES["ePlug"]` grid `:64`, negative bins
  clipped to zero). The removal touches only the veto-passing (`ePlug` < 7) entries of the N−1
  Plug histogram; the replacement moves them to the measured excess spectrum instead of deleting
  them. `user:` D2 names removal only; A5 additionally replaces the removed events' `ePlug` with a
  control-region excess draw so the N−1 spectrum, not just the fail fraction, is corrected.
  Recommended: adopt A5. If the user keeps pure removal, the dropped entries are deleted from the
  N−1 histogram and G1(a) is ruled on that histogram.
- A6 **Which samples the correction touches.** All MC components, including ND; one p per period,
  independent of component, η_max and D cell. Reason: noise is detector-side (verdict `:48,66`);
  any component dependence would be tuning.
- A7 **GOF toys.** `n_toys` = 1000 for every (scope, binning, ND alternative) fit; ND alternatives
  7071 and 7091; seeds as in the v3 script (`run_section_7_1_twofree_v3_2026-10-20.py:122,130`).
  The four-catalogue alternative (7094/7096/7097, `D/data/t2_detector/mc_merged_nd4cat_2026-10-20/`,
  scale sidecar `D/docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv`; prior fit
  ΔD = −330, `D/docs/t4/leakage_fit_section_7_1_nd4cat_results_2026-10-20.md`) is run only if both
  fail G1(c): 3 N−1 jobs, 3 builder jobs (8 GiB/7200 s for 7094/7096, 4 GiB/2700 s for 7097, as in
  `D/condor/t4/localization_diag_nd4cat_2026-10-20/`) and 8 fits, before S4 starts. Pass needs toy
  p ≥ 0.05, coarse and fine, for at least one alternative (G1(c)); the scope is an open question.
- A8 **Remedy 2 (threshold scan).** Thresholds 3/5/7/10/15 GeV applied to the Plug literal in both
  the N−1 fork and the η_max builder fork; the nominal candidate is the smallest threshold at
  which the control-region data and MC Plug fail rates agree within 2σ in every period; G1 is
  evaluated at that threshold only. Reason: D3(ii); evaluating G1 at every threshold would be a
  look-elsewhere tuning.
- A9 **Remedy 3 (no Plug veto).** Stage-B and the cut-removed payload drop the Plug term; the
  M_Y > 1.6 GeV migration per D cell is taken from the PDISS truth tables already produced by the
  N−1 script (`my_tables`, `MY_EDGES :62`). `user:` G1(a) names a Plug N−1 test that does not
  exist without a Plug veto. Recommended reading: under remedy 3, G1(a) = FMD and FTS N−1 shape
  ≤ 0.10 plus the Stage-B data/MC yield ratio in each D cell within 1 ± 0.10 after the PDISS
  migration term; G1(c) unchanged.
- A10 **Nominal / systematic ordering.** Per D3: the first remedy passing G1 is nominal; a later
  passing remedy is a systematic leg. If remedy 1 passes, S4 is skipped and its remedies are
  deferred to WP-J. If no remedy passes within the budget, the ruling is KILL.
- A11 **Budget tally.** Condor-days = Σ(job wall × `request_cpus`)/24 h from the `.log` files under
  `D/condor/restart_2026-10-27/wp-a/*/logs/`, tallied by `h1-naf-operator` after each slice into
  `D/data/restart_2026-10-27/wp-a/condor_tally.csv` (columns `slice, cluster, n_jobs, wall_s,
  cpus, condor_days`). Login-node runs are listed with wall time but count zero Condor-days.
- A12 **Reused code is forked, not edited.** Originals under `D/scripts/t4/` stay byte-identical
  (md5 recorded once, in the S1 record); forks live under `D/scripts/restart_2026-10-27/wp-a/`.

## Outline

Scripts (`D/scripts/restart_2026-10-27/wp-a/`, each a fork with added flags, defaults equal to
the original behaviour):

- `n1_wpa.py` ← `D/scripts/t4/build_localization_n1_mytbl_nd4cat_2026-10-20.py` (identical to
  `build_localization_n1_mytbl_2026-10-20.py` except the `NOODS` set and the scale sidecar; line
  numbers below are the original's; add 17 from `:24` on for the nd4cat file). Add to the CLI
  (`:99-103`): `--plug-threshold` (float, default 7; replaces the literal at `:116` and the strings
  at `:73-75`), `--no-plug-veto`, `--split {all,control,test}`, `--noise-table <json>`,
  `--period-map <csv>`, `--seed`. Read `EventNumber`, `xIp`, `mx` with the branches at
  `:106-108`. Apply the split and the removal as masks before `:113` so the raw integer counts and
  the closures I1-I4 (`:224-260`) stay exact on the kept set; the `ePlug` replacement of A5 acts
  only on the `N1_PLUG` histogram fill. Add `period_counts` (per period: control/test counts,
  stack pass counts) to `partial.json`; if the v3 assembler rejects the extra key it goes to a
  `period_counts.json` sidecar instead. Invariant: with default flags the output equals the
  2026-10-20 partial (test `fork_default_identity`).
- `noise_table_wpa.py` (new, numpy/json only). Input: control partials for data and MC, the v3
  scale table. Output: `noise_table.json` {period: {p, p_err, f_data, f_mc, excess_eplug_hist,
  per_detector_excess, eta_strata}} with sha256, and `noise_table.md`. No tree access.
- `assemble_wpa.py`, `analyze_wpa.py` ← `assemble_localization_diag_2026-10-20.py`
  (`shape_metric :73-85`, `MIN_DATA_ENTRIES=20 :54`, roots `:38`) and
  `analyze_localization_diag_2026-10-20.py` (fail rates `:172-231`, roots `:43,179`): replace the
  hard-coded roots with `--root`; no metric change.
- `eta_builder_wpa.py` ← `D/scripts/t4/build_datamc_cutremoved_eta_7p1_2026-10-20.py`. Same
  flags as `n1_wpa.py`; Plug literal at `:96` and `:170`, MC event weight at `:99`. The assembler
  `D/scripts/t4/assemble_cutremoved_eta_7p1_v3_2026-10-20.py` is reused unchanged via
  `--partials-root`/`--outdir` (`:270-276`).
- `fit_7p1_wpa.py` ← `D/scripts/t4/run_section_7_1_twofree_v3_2026-10-20.py` (and the nd4cat
  fork for the four-catalogue alternative). Add `--payload`, `--outdir`, `--alt`, `--n-toys`
  (replaces the literals at `:122,:130`). Fit, deviance and toy recipe (`:19-30,:80-91`) unchanged.

Condor (`D/condor/restart_2026-10-27/wp-a/`): `n1_wpa/`, `eta_wpa/`, `fit_wpa/`, each with a
`.sub`, a queue file, `logs/`, patterned on
`D/condor/t4/localization_diag_2026-10-20/localization_diag.sub` (1 CPU; 2 GiB/1800 s; 7071
4 GiB/2700 s; 7091 8 GiB/7200 s) and
`D/condor/t4/datamc_cutremoved_eta_7p1_2026-10-20/datamc_cutremoved.sub`. Each job is one input
file. Fit jobs: one per (alt, binning, scope), 1 CPU, 1 GiB, 1800 s.

Data (`D/data/restart_2026-10-27/wp-a/`): `s1_split/`, `s1_control/`, `s2_remedy1/`,
`s3_gof/`, `s4_thr<N>/`, `s4_noplug/`, `condor_tally.csv`. Each run directory holds
`partials/<label>/partial.json`, `assembled/`, `analysis/`, and a `MANIFEST.json` (inputs with
sha256, flags, noise-table sha, git commit). Plots under `D/plots/restart_2026-10-27/wp-a/<slice>/`.

Docs (`D/docs/restart_2026-10-27/wp-a/`): `s1_period_split_record.md`, `s2_remedy1_record.md`,
`s3_gof_record.md`, `s4_remedies_record.md` (only if S4 runs), `g1_verdict.md`.

Gotchas: `ep0607_true.root` mixes fall-06 and 2007 (split by `RunNumber`); 9012 is one stream
projected onto all periods (`exposure_factor`), so its period is assigned from the projected run
number map, not the file; 9012 has event weight 1 (`:122`); the certified list is applied inside
the scripts to data only (`:111-113`); `7091.root` is 33 GB and must never be opened on the login
node.

## Slices

Specialists: `h1-naf-operator` validates every Condor submission, log and the tally;
`h1-mc-expert` validates overlay/noise semantics (S1, S2) and the PDISS migration reading (S4);
`h1-analysis-critic` rules the verdict (S5). Condor for any multi-file step; login node only for
single-file smokes and numpy-on-JSON steps.

### S1. Period/run-range split and control-region definition
Delivers: the data Plug fail fraction per period and run range, the control-region definition with
a disjointness proof, and the `h1-mc-expert` lookup result.
Inputs: the 14 files of A4, filtered to
`D/data/t2_detector/certified_runlist_2026-10-19/certified_runlist_2026-10-19.csv` (ACCEPT
rows; `sub_period`, `true_unit` columns for the period map); per-run lumi in
`.../certified_runlist_2026-10-19/lumi/`.
Units: (a) `n1_wpa.py` + `assemble_wpa.py` + `analyze_wpa.py` forks; (b) `period_map.csv`
(run range → period; from the run list) and `D/condor/restart_2026-10-27/wp-a/n1_wpa/`;
(c) `h1-mc-expert` lookup: are the random-trigger pools named in the overlay doc §2 (lines
101-112, `/pnfs/desy.de/dphep/online/h1/noise/`) readable, and
does `RT_NOISE=Y` imply Plug energy noise or only hit noise (recorded in the S1 record; informs
attribution, not a remedy).
Runs: Condor, 14 jobs × 3 splits (`all`, `control`, `test`) = 42 jobs; `analyze_wpa.py` on the
login node (JSON only).
Outputs: `D/data/restart_2026-10-27/wp-a/s1_split/` and `s1_control/`;
`D/docs/restart_2026-10-27/wp-a/s1_period_split_record.md` with per-period and per-sub-period
f_fail (data, each MC component, cocktail), control-region counts, η_max-stratified excess;
`D/plots/restart_2026-10-27/wp-a/s1/`.
Tests:
- `fork_default_identity`: `n1_wpa.py` with default flags on `9155.root` → `partial.json` equal
  to `D/data/t4/localization_diag_2026-10-20/partials/9155/partial.json` (counts and hists).
- `fail_fraction_recomputed`: split `all`, stack C → data f_fail per D cell within 0.002 of
  0.207/0.230/0.283.
- `control_disjoint`: (RunNumber, EventNumber) set of split `control` ∩ split `test` → empty, per
  file, data and MC.
- `split_complete`: control + test counts → equals `all` counts per mask and cell.
- `period_map_total`: Σ per-period certified lumi from the period map → 317,212.80 nb⁻¹, the
  frozen total (`D/docs/t2/b5_operations_signoff_2026-10-21.md:24`) within 0.01 nb⁻¹.
Checkpoint: `analyze_wpa.py --root D/data/restart_2026-10-27/wp-a/s1_split` prints the three
fractions and exits 0; `h1-naf-operator` appends the S1 line to `condor_tally.csv`.

### S2. Remedy 1 nominal pass and N−1 re-run
Delivers: `noise_table.json`, the corrected MC N−1 chain on the test sample, the G1(a) and G1(b)
numbers.
Inputs: S1 control partials; test-split inputs as in S1.
Units: (a) `noise_table_wpa.py` (login node); (b) `n1_wpa.py --split test --noise-table ...`
Condor run (14 jobs); (c) assemble/analyze on the result; (d) `h1-mc-expert` sign-off on the
A4/A5 semantics before (b) is submitted.
Outputs: `D/data/restart_2026-10-27/wp-a/s2_remedy1/{noise_table.json,partials,assembled,analysis}`;
`D/docs/restart_2026-10-27/wp-a/s2_remedy1_record.md` with: p_k ± σ per period, the Plug N−1
unit-area max|MC/DATA−1| (bins ≥ 20 data entries) and |Δf_fail| per D cell, FMD and FTS N−1 maxima
for the signed-v3 cocktail and for the admissible-cocktail scan (non-negative weights within
±50% of signed-v3, grid step 0.25), each ruled PASS/FAIL against A0; `D/plots/restart_2026-10-27/wp-a/s2/`.
Tests:
- `noise_table_bounds`: every p_k in [0, 1], p_err > 0, sha256 recorded → true.
- `removal_rate_closure`: on MC test sample per period, (dropped events)/(events before removal)
  → p_k within 3σ binomial.
- `no_test_tuning`: `noise_table.json` sha256 cited in the record equals the sha of the file
  produced from `s1_control` only (no field depends on `s1_split`/test inputs) → identical.
Checkpoint: record contains the G1(a)/(b) numbers with PASS/FAIL per clause; tally updated.

### S3. η_max GOF on the rebuilt payload
Delivers: the two-free fit with ≥ 1000 toys on the cut-removed payload rebuilt with remedy 1.
Inputs: `eta_builder_wpa.py` with the S2 noise table and the test split; assembler v3 with
`--partials-root`; the v1/v2/v3 scale tables it already takes.
Units: (a) builder Condor run (28 jobs; +3 nd4cat builder jobs per A7 if both alternatives fail,
with +3 N−1 jobs in the S1 chain); (b) assembler (login node, JSON only); (c)
`fit_7p1_wpa.py` Condor run: 2 alternatives × {coarse, fine} × {D0, D1, D2, ALL} = 16 jobs
(plus 8 for the four-catalogue fallback of A7 if both fail).
Outputs: `D/data/restart_2026-10-27/wp-a/s3_gof/{partials,assembled_v3,fits}`;
`D/docs/restart_2026-10-27/wp-a/s3_gof_record.md` with D/ndf, s_diff, s_ND, toy p per job, ruled
against A0, next to the 2026-10-20 baseline numbers; `D/plots/restart_2026-10-27/wp-a/s3/`.
Tests:
- `payload_asserts`: assembled JSON `pass` and `conservation.all_pass` → true
  (`run_section_7_1_twofree_v3_2026-10-20.py:94`).
- `baseline_reproduced`: `fit_7p1_wpa.py --n-toys 300` on the 2026-10-20 payload, alt 7071,
  fine ALL → D = 5925.8 ± 0.1, s_diff 0.9045, s_ND 1.3251
  (`D/docs/t4/leakage_fit_section_7_1_results_2026-10-20.md:19-30`).
- `toy_floor`: every fit reports `n_toys` ≥ 1000 and p_min = 1/(n_toys+1) → true.
Checkpoint: record contains G1(c) PASS/FAIL per alternative; tally updated.

### S4. Remedies 2 and 3 (only if S2 or S3 fails G1)
Delivers: the threshold scan and the no-Plug-veto variant through the same N−1 and GOF chain.
Inputs: as S2/S3 with `--plug-threshold {3,5,7,10,15}` (control split first for the A8 choice,
then test split at the chosen threshold) and `--no-plug-veto`.
Units: (a) control-split N−1 at 5 thresholds (70 jobs, Condor); (b) test-split N−1 and builder
at the chosen threshold and for no-Plug (2 × 42 jobs); (c) fits (2 × 16 jobs); (d) `h1-mc-expert`
validates the M_Y > 1.6 GeV migration reading from `my_tables` (A9).
Outputs: `D/data/restart_2026-10-27/wp-a/s4_thr<N>/`, `s4_noplug/`;
`D/docs/restart_2026-10-27/wp-a/s4_remedies_record.md`; `D/plots/restart_2026-10-27/wp-a/s4/`.
Tests:
- `threshold_choice_control_only`: the chosen threshold is derived from `s4_thr*/control` records
  only → record cites no test-split number before the choice.
- `threshold_scan_monotone`: data Plug fail fraction per cell decreases with threshold → monotone.
- `noplug_migration_reported`: PDISS M_Y > 1.6 GeV migration fraction per D cell → reported with
  its ±50% PDISS-normalisation envelope.
Checkpoint: record contains the G1 numbers for each remedy run; tally updated.

### S5. Verdict
Delivers: `D/docs/restart_2026-10-27/wp-a/g1_verdict.md`.
Inputs: S1-S4 records, `condor_tally.csv`, the parent plan's G1 text.
Units: (a) draft by `executor`; (b) ruling by `h1-analysis-critic` (PASS, KILL, or CONTINUE
only when the calendar budget remains and a pre-registered remedy is still unrun);
(c) `h1-naf-operator` signs the tally.
Tests:
- `threshold_pre_registered`: thresholds quoted in the verdict → identical to the parent plan's
  G1 text (diff of the quoted block → empty).
- `tally_present`: Condor-days and calendar days spent, with start date, → present and
  ≤ 10 / ≤ 14.
- `nominal_named`: the verdict names the nominal remedy and any systematic leg, or KILL → one of
  the three rulings, no other wording.
Checkpoint: `g1_verdict.md` exists with the G1(a)-(c) numbers, the ruling, and the tally.

## Verification

End to end: `g1_verdict.md` cites S1-S3 (and S4 if run) records whose numbers trace to
`MANIFEST.json` files with input sha256s; `fork_default_identity` and `baseline_reproduced` hold;
`control_disjoint` holds for every file; the t4 originals are md5-unchanged; `condor_tally.csv`
totals ≤ 10 Condor-days; no file outside the five `restart_2026-10-27/wp-a/` roots was written.

## Risks

- G1(c) is expected to be nearly invariant under remedy 1 (an η_max-independent removal cannot move
  a shape fit at D/ndf ≈ 5900/358); a KILL on (c) after remedy 1 is the likely outcome unless
  remedies 2/3 change the payload composition. Pre-registered here so it is not read as a surprise.
- The control-region excess may contain PDISS M_Y > 1.6 GeV leakage even at η_max < 1.5 (MC
  veto-pass at the elastic peak is only 0.58-0.65, verdict `:64`); the η_max-stratified excess is
  the diagnostic, the residual a WP-J systematic (parent Risks).
- A negative excess (MC noisier than data) in some period makes remedy 1 inapplicable there (A4).
- 9012's period assignment is a projection; a per-period p applied to one stream adds no new
  physics but can mis-weight if the projection map is wrong (checked by `removal_rate_closure`).
- The certified list is frozen (B5 satisfied 2026-10-21) but `B5` strings in the t4 script still
  say "NOT frozen"; forks must update the string, not the logic.
- Calendar: 2 weeks from first submission; the tally must record the start date.

## Scope Notes

In: S1-S5 above. Out (per brief): parent WP-A step (2) "ND-dominated region (η_max > 3.5) for
the energy scale" is not in the briefed slice contract; supervisor to confirm the drop or assign
it. Out: everything in the Goal non-goals; FMD/FTS remedies beyond D2; changes to the
§7.1 fit model, binning or templates; new scale tables; any edit under `D/scripts/t4/` or
`D/data/t4/`. User decisions `executor` must not reopen: D2 (control region, not a stream), D3
(remedy order, no tuning on the test sample), G1 thresholds and the 10 Condor-day / 2-week cap
(parent Log 2026-10-27), restart authority. Memory:
`.supervisor/memory/decisions/2026-10-26-no-diffractive-term-or-mixed-response.md` (parent side;
on KILL the parent publishes alone, nothing in WP-A changes it).

## Assumptions

- `EventNumber` is populated and unique per run in every data and MC `DiffTot` file (checked in
  S1 by a per-file uniqueness count on (RunNumber, EventNumber)).
- `uproot` and numpy on the NAF login node are the versions the t4 scripts ran with (checked by
  `fork_default_identity`).
- The `assemble_cutremoved_eta_7p1_v3` assembler accepts partials with the extra `period_counts`
  key (checked by `payload_asserts`).
- The Condor `.log` files record wall time and `request_cpus` for every job (checked by
  `h1-naf-operator` on the S1 logs).

## Open Questions

- `user:` A5 (control-region excess spectrum for removed MC events, an extension of D2's removal).
- `user:` G1(c) pass scope, which the parent does not fix: recommend `ALL` (coarse and fine).
- `user:` A9 (G1(a) reading under remedy 3, which has no Plug N−1).

## Log

- 2026-10-27 decision: supervisor — (1) A5 adopted: removed MC events get an `ePlug` drawn from the control-region excess spectrum; (2) G1(c) pass scope = ALL D cells, coarse and fine; (3) A9: under remedy 3, G1(a) = FMD/FTS N−1 ≤ 0.10 plus Stage-B yield ratio within 1 ± 0.10 after the PDISS M_Y > 1.6 migration term; (4) parent step "η_max > 3.5 energy-scale region" assigned to S1 as a diagnostic table (no gate role). Budget 10 Condor-days / 14 calendar days from 2026-10-27.
- 2026-10-27 executor: implementation choices (no gate change): fork base is the nd4cat N−1 builder (NOODS incl. 7094/7096/7097); MC period = represented data period from the composite-scale projection (unique decomposition of each exposure_factor into certified period lumis; nd4cat 7071 → em06), not the sim RunNumber (h1-mc-expert S1c: sound for lumi weighting; sim-period mismatch is a systematic); removal probability q = (f_data − f_MC)/(1 − f_MC) (exact control closure of the independent-noise model; A4's p = f_data − f_MC recorded as p_naive); excess spectrum = data − MC (MC normalised to the data control count) restricted to ePlug ≥ T plus overflow; the replaced ePlug enters every mask; assemble/analyze forks replaced by `g1_eval_wpa.py` (t4 assembler X1-X6 cross-checks are fixed to the full sample; registered shape_metric copied verbatim); `eta_builder_wpa.py` + v3 assembler replaced by `payload_wpa.py` reading the n1 partials' `dcell_eta.cutremoved` block (same basis, X1/X2-proven equal; re-proven on split=all).
- 2026-10-27 coordinator → pre-registered S3 input (before any S3 fit): ND alternatives are 7071 (RAPGAP ME+PS, verified by h1-mc-db, `../wp-d/mc3_7071_cascade_mode.md`), 7091 (CDM) and **7094** (ME+PS, same period/SW as 7091) as a third alternative; nd4cat (A7) stays the conditional fallback. G1(c) text unchanged.
- 2026-10-27 S1: Condor cluster 5902090, 51/51 rc 0, 0.0088 Condor-days. fork_default_identity PASS (9155), fail_fraction_recomputed PASS (0.2070/0.2297/0.2831, exact), control_disjoint PASS (17/17 files, 0 shared keys), split_complete PASS, period_map_total PASS (317,212.804 nb⁻¹), (Run,Event) unique in every file. ep0607 halves differ < 2σ → 4 periods.
- 2026-10-27 S2a: noise_table.json sha256 f93a6dd270f56ec1832efb1435877842ea77672667c4f870f7c7872d543fbcaf (inputs: s1_control only); q ≈ 0.155-0.191 ± 0.024-0.032 per period, remedy 1 applicable. h1-mc-expert S2d sign-off YES-WITH-CONDITIONS (`s2d_mc_expert_signoff.md`): stack excess is only ~1/3 Plug, so the A5 stack-to-Plug attribution over-shifts the Plug N−1 — escalated as `user:`.
- 2026-10-27 S2b: cluster 5902091 (43 jobs, rc 0). G1(a) FAIL every chain (Plug N−1 max dev 4.0-6.8; max |Δf_fail| 0.037-0.046, D2 low / 7094 D0 high); G1(b) FAIL (best admissible joint 0.68-0.76); removal_rate_closure: 76/77 rows within 3σ, one at −3.26σ (7071|ep0304; global p ≈ 0.08).
- 2026-10-27 S3: clusters 5902092 + 5902094 (nd4cat fallback, A7: all three primary alternatives failed). baseline_reproduced PASS (D 5925.826, toy p bit-identical); 32 fits at 1000 toys; G1(c) FAIL (max p 0.011; every ALL fit at the floor).
- 2026-10-27 S4: remedy 2 — no pre-registered threshold satisfies A8 on control data (Plug-fail |z| 2.2-3.5 at 15 GeV), no nominal threshold, G1 not evaluated at any other threshold; remedy 3 — cluster 5902093 + fits 5902095/5902096; A9 G1(a) FAIL (FMD 1.07-2.33, FTS 0.70-0.76, yield ratio 1.21-2.36), G1(c) FAIL (all 32 at floor). h1-mc-expert S4d migration reading YES-WITH-CONDITIONS (mirMY vs hatMYGen: 0.3% cross 1.6 GeV).
- 2026-10-27 tally: 175 jobs, 0.0267 Condor-days, day 1 of 14 (signed h1-naf-operator). Lead-submitted clusters 5902092-5902096 audited post hoc by h1-naf-operator (deviation from "validates every submission").
- 2026-10-27 S5: g1_verdict.md; h1-analysis-critic RULING KILL (all pre-registered remedies run, none passes G1(a)/(c); budget unspent, 0.0267 Condor-days, day 1 of 14); threshold_pre_registered PASS, tally_present PASS, nominal_named KILL. Code critic PASS (C1 7094 q inflation and C2 md5 provenance documented; C3/C4 dropped per scope-critic).

## Todo

- [x] S1a forks `n1_wpa.py`, `assemble_wpa.py`, `analyze_wpa.py`; `fork_default_identity` passes
- [x] S1b `period_map.csv`, `n1_wpa.sub` + queue; 42 jobs submitted and validated
- [x] S1c `h1-mc-expert` lookup recorded
- [x] S1 record written; `fail_fraction_recomputed`, `control_disjoint`, `split_complete` pass
- [x] S2a `noise_table_wpa.py`; `noise_table.json` with sha256; `h1-mc-expert` sign-off
- [x] S2b corrected test-split N−1 run; assemble/analyze; record with G1(a)/(b) numbers
- [x] S3a `eta_builder_wpa.py`; 28-job run; assembler v3; `payload_asserts` (done as `payload_wpa.py` from the n1 partials, no builder jobs; see Log)
- [x] S3b `fit_7p1_wpa.py`; `baseline_reproduced`; 16 fits at 1000 toys; record with G1(c)
- [x] S4 (conditional) threshold scan control-first, chosen-threshold test run, no-Plug run, record
- [x] S5 `g1_verdict.md` drafted; `h1-analysis-critic` ruling; tally signed
- [x] Parent Log line returned to the supervisor
