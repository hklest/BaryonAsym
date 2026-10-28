# Plan: diffractive SIDIS at Q² > 150 GeV² — restart from scratch (2026-10-27)

Program plan (parent). `D/` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/`,
`P/` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`. Restart root for every new
product: `D/docs/restart_2026-10-27/<wp>/`, `D/data/restart_2026-10-27/<wp>/`,
`D/scripts/restart_2026-10-27/<wp>/`, `D/condor/restart_2026-10-27/<wp>/`.
Primary input: `D/docs/research/revival_assessment_2026-10-27.md` (cited as `RA`). "plan §n" in the
register means the prior plan `D/H1DiffractiveHighQ2SIDISPlan.md`.

## Goal

Deliver the first unfolded, multidifferential diffractive charged-hadron SIDIS measurement at
Q² > 150 GeV²: per-event-normalised x_F and p_T*² spectra of charged hadrons in the X (γ*IP) rest
frame, in M_X bins, for h⁺ and h⁻, from the certified HERA-II LRG sample (317,212.8 nb⁻¹), with a
full systematic budget, an analysis-note section and updated handoff/memory.

Non-goals: FPS/VFPS leading-proton cross-check; a (Q²,x,z,pT) grid like the parent's; generator
retuning; any change to the parent inclusive release.

Expected change size: one bounded probe (WP-A, ≤10 Condor-days) then, if it passes, a campaign of
roughly 15-30 Condor-days over 2-3 months: a writer fork with 2 new branch groups, 4-6 downstream
Python forks, ~12 new doc records, one note section. All prior diffractive products stay as provenance.
Amended 2026-10-28: G1 and G1′ were killed; two further bounded packages on the existing WP-A3 skims (WP-A5,
WP-A6; ≤ 1 Condor-day together, 2-3 doc records each) rule the replacement gate G1″ before the campaign;
the campaign size is unchanged.

## Approach

Gate first, then build. WP-A probes the one localized blocker (Plug veto deficit,
`D/docs/t4/localization_verdict_2026-10-20.md:46-48,68-74`) with a pre-registered kill criterion
and re-runs the N−1 and η_max GOF gates. WP-B/C/D run in parallel with WP-A because they need no
new production. WP-E onward is blocked until gate G1 passes. The observable pivots to X-rest-frame
x_F/p_T*² in M_X bins (H1 PLB 428 (1998) 206 style, RA §4), integrated over Q² > 150 GeV², which
(a) is coarse enough for the ~47k weighted reco signal tracks per charge (WP-C), (b) integrates over the Q² cells
whose ND leakage grows from 4% to 41% (`D/docs/t4/localization_verdict_2026-10-20.md:62`), and
(c) cancels normalisation systematics by per-event normalisation.

Rejected alternatives (one line each):
- Keep the 45-bin parent-frame grid: tens-of-events bins, 26-45% envelope effects (RA §1.5).
- Fund the full response campaign first: the adjudication's own §3(ii) requires localization and a
  kill criterion first (`D/docs/t4/final_path_adjudication_2026-10-20.md:152`, §3(ii); the
  missing-evidence list is §7, `:303-316`).
- Avoid re-production by boosting to the collinear q + x_IP·P frame at gen level: kept only as the
  WP-C interim proxy; the gen X 4-vector is not in the ntuples and a gen/reco frame mismatch would
  enter the response (worker finding; writer `P/src/ChargedSidisTreeBuilderHighQ2_v6dif.C:978-1056`).

Amended 2026-10-28: the campaign gate is G1″ (D12-D14), ruled by WP-A5 and WP-A6 on the existing skims; WP-E
onward waits on both records and the Q4 user return.

## Decision Re-evaluation Register

Every prior binding decision, re-judged under the user's 2026-10-27 authorisation. `keep` = carried
as a start value, `change` = replaced now, `defer` = re-decided by the named package. `user:` marks
material items (also in `## Open Questions`).

| # | Prior decision (source) | Verdict | Rationale | Re-decided by |
|---|---|---|---|---|
| R1 | Observable = parent (Q²,x,z,pT) grid, 45 bins/charge (`D/docs/binning/diffractive_binning_baseline_2026-10-17.md`) | change | user pivot to X-frame x_F/p_T*² in M_X bins; parent grid starved | WP-C |
| R2 | Masks h⁺ 16 / h⁻ 14, rule N_pred<10 (baseline §2) | change | void with R1; new blinded mask rule (D7) | WP-C, ratified by user at WP-K |
| R3 | x_IP < 0.03 fiducial, split undecided (plan §12.1) | keep, unsplit | statistics; Reggeon handled by subtraction with a per-bin ceiling (D4) | WP-B, WP-F |
| R4 | M_Y < 1.6 GeV, \|t\| < 1 GeV² (plan §2, FIXED) | keep | H1 LRG standard; needed for the σ_r^D(3) gate against EPJC 72 (2012) 2074 | — |
| R5 | M_X > 2 GeV fiducial (plan §12.3) | change → M_X ∈ [4, 40] GeV | hadron spectra need multiplicity; removes exclusive VM by construction; costs ≈3% of signal (gen M_X < 5 GeV is 2.6%, worker from `D/data/t1_particle_level/analysis_fullbatch_hists3d_2026-10-18/9012.json`) | WP-B confirms edges |
| R6 | β < 0.8 subsample as possible primary (plan §12.3) | user: | the user's pivot names "M_X / β bins"; lead recommends M_X bins with ⟨β⟩ quoted per bin (Open Q5) | WP-C after Q5 |
| R7 | η_max < 3.2 with 0.8 GeV clusters (plan §4.1) | keep as START, vary 3.1/3.3 | HERA-II dijet convention; gated by the LAr N−1 | WP-B |
| R8 | E_Plug < 7 GeV veto (plan §4.1) | defer | the localized deficit; three pre-registered remedies in WP-A | WP-A |
| R9 | FMD ≤ 1 hit pair layers 1-2; FTS 26/28 m empty (plan §4.1) | keep as START | composition-reachable failures; noise correction from the WP-A control method | WP-A, WP-B |
| R10 | Reco extended ranges x_IP < 0.04, M_X > 1.5 (plan §4.1) | keep concept; M_X reco > 3 GeV | migrations across the fiducial boundary must be in the response | WP-C |
| R11 | Signal = Y = p + dissociation M_Y < 1.6; 83:17 mix, ±7.2% (handoff "Mixture nominal") | keep | H1 standard; mix re-derived on the new selection, same ratio 1.20 | WP-F |
| R12 | Background baseline subtraction vs cocktail (plan §12.4) | keep subtraction | H1 practice; cocktail as control-plot cross-check | WP-F |
| R13 | Certified run list FROZEN, 317,212.80 nb⁻¹ (handoff B5) | keep | 9/9 sign-off; Plug gate condition satisfied by it | — |
| R14 | FPS/VFPS cross-check (plan §12.6) | change → no | statistics-limited at high Q²; out of scope | — |
| R15 | Composite scale table v3 + exposure convention (handoff) | keep as luminosity facts | per-sample weights recomputed on the new selection | WP-E |
| R16 | ENVELOPE-ONLY leakage end state; FATAL verdict | superseded | user restart; the verdict's §7 items become gates G1-G2 | this plan |
| R17 | "Do not retune generators" (plan §6.2) | change (lead) | data-driven forward-detector noise/efficiency corrections are detector response, not generator tuning; generator physics stays untouched | WP-A |
| R18 | Trap "no random-trigger stream" (handoff) | change wording | MC carries the overlay (`D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md:24-41`); data-side rates come from the deep-gap control (D2) | WP-L |
| R19 | DF-SYS-01/02 registered UNEVALUATED | re-evaluate | new selection and grid; evaluated as WP-F outputs | WP-F |
| R20 | C_rad = parent DJANGOH14 gen-level pair | change | no diffractive non-rad SimRec; gen-level RAPGAP pair or bound (D9) | WP-I |
| R21 | Parent memory: no diffractive term in the inclusive analysis (`.supervisor/memory/decisions/2026-10-26-no-diffractive-term-or-mixed-response.md`) | keep | parent-side; untouched | — |
| R22 | Parent: observable strict, no VM veto (`.supervisor/memory/decisions/2026-10-26-observable-strict-no-vm-veto.md`) | keep | M_X ≥ 4 GeV removes exclusive VM; no veto, no one-track cut | WP-B |
| R23 | T5 σ_r^D(3) gate vs EPJC 72 (2012) 2074 (plan §10.2, never run) | keep as gate G3 | the only external credibility check of the LRG selection | WP-G |
| R24 | 6671/6677 Reggeon usable at detector level (signed table) vs "not usable" (semantics record) | defer | SW=70/no-ODS validity question | WP-D via `h1-mc-db` |
| R25 | Re-base on OPT-A Q2esGen and eSigma photon-frame conventions | keep (user) | like-for-like with the parent; lives in offline forks (D5) | WP-E |
| R26 | New official MC requests (non-rad diffractive, Fit A, ARIADNE, SW=84 Reggeon, ep0304 PDISS) | user: | external cost; recommendation none before G1 | WP-D → Open Q3 |
| R27 | G1/G1′ clause (a): Plug N−1 energy shape ≤ 0.10 and \|Δf_fail\| ≤ 0.02 as a kill gate (this plan, 2026-10-27) | superseded (user 2026-10-28) | the gate is veto efficiency, not Plug energy; the excess becomes a correction + systematic (G1″(a), D12-D14) | WP-A5 |
| R28 | G1/G1′ clause (c): two-template η_max toy p ≥ 0.05 over the full range to 3.4 as a kill gate | superseded (user 2026-10-28) | the LAr-edge 3.15-3.4 mismatch is simulation response (`D/docs/restart_2026-10-27/wp-a4/forward_response_fix_assessment.md` §1.1); fit confined to η_max(0.4) < 3.0, generator spread as systematic, G2 kept (G1″(c)) | WP-A6 |

## Decisions

- D1 **Gate order.** WP-E through WP-K are blocked on G1 (WP-A). WP-B, WP-C, WP-D, WP-I(research)
  run in parallel with WP-A on existing ntuples. Reason: the adjudication's revival preconditions
  and budget realism. Amended 2026-10-28: the campaign gate is G1″ (WP-A5 for (a), WP-A6 for (c)/G2);
  WP-E onward is blocked on both records, and the Q4 user return happens there.
- D2 **Noise prerequisite resolved by a control region, not a stream.** Data-side forward-detector
  noise/threshold rates are measured in a deep-gap control region (only η_max < 1.5 required, M_X
  and x_IP in range) where physics forward energy is absent, as the combined forward-stack
  (Plug, FMD, FTS) fail probability, data versus overlaid MC; per-detector rates are diagnostics for
  attribution. The excess is applied to MC as one random event-removal probability per period
  (EPJC 72 (2012) 2074 method), derived in the control region and tested on the disjoint Stage-B
  N−1 sample; its dependence on the PDISS M_Y spectrum is a WP-J leg. Reason: no data
  random-trigger stream in the DST chain (`D/data/t2_detector/randomtrigger_probe_2026-10-22/probe_record_2026-10-22.md:23`).
  Superseded 2026-10-28 by D14: the per-detector deep-gap N−1 view (`etaMaxClus08` < 2.5, per (M_X, D cell))
  replaces the η_max < 1.5 combined-stack control region; the remedy-1 q̄ ≈ 0.18 overshoots the deep-gap Plug
  data by 0.11 (`veto_efficiency_view.md`, remedy-1 section) and must not be reused.
- D3 **Plug remedies, pre-registered and ordered.** (i) noise correction per D2 (one stack
  probability; per-detector rates as diagnostics); (ii) threshold raise
  to the value at which the control-region fail rates agree (scan 3/5/7/10/15 GeV); (iii) drop the
  Plug veto and take the M_Y > 1.6 GeV migration from the PDISS MC. The first remedy that passes
  G1 is nominal; the next passing one is a systematic leg. No tuning on the test sample.
  Superseded 2026-10-28 (D12-D14): remedy (i) becomes the per-detector, per-(M_X, cell) correction of WP-A5;
  (ii) the threshold scan is optional (WP-A7, 7/10/15 GeV); (iii) dropping the Plug veto is not chosen.
- D4 **Selection start values** (WP-B confirms): parent event selection; η_max < 3.3 (0.8 GeV
  clusters, electron excluded, vertex-relative); FMD ≤ 1 hit pair in layers 1-2; FTS 26/28 m empty;
  Plug per WP-A; reco x_IP < 0.04, reco M_X > 3 GeV; fiducial x_IP < 0.03, M_Y < 1.6 GeV, \|t\| <
  1 GeV², 4 ≤ M_X ≤ 40 GeV; no VM veto, no multiplicity cut. Reggeon fraction per M_X bin ≤ 0.15 or
  x_IP is tightened to 0.02 in that bin's study. — superseded by D15 (edge 3.0) on 2026-10-28
- D5 **Re-base = downstream forks, plus one writer fork.** OPT-A gen cell (`Q2esGen`, already in
  the writer, `P/src/ChargedSidisTreeBuilderHighQ2_v6dif.C:999,1112`) and eSigma ISR-only gen
  kinematics (`P/scripts/unfolding/esigma_final_2026-10-24/esigma_xf.py`) are offline conventions.
  The writer fork adds only the gen X and Y 4-vectors (and gen largest-gap M_Y, t) so the gen
  frame equals the reco frame definition; then data (658 jobs, ≈30 min) and the 13 MC samples are
  re-produced once.
- D6 **Observable.** Frame: rest frame of p_X with the z axis along the γ* direction; x_F =
  2 p_L*/M_X with x_F > 0 along the Pomeron (proton) direction, the parent's proton-positive
  convention (`.supervisor/memory/decisions/2026-10-24-xf-gamma-p-frame-not-lab.md`) and the
  opposite sign to PLB 428 (1998) 206 (sign confirmed in Open Q2); p_T*² transverse to that axis. Reco p_X = stored HFS
  4-vector `phE/phPx/phPy/phPz` (`…_v6dif.C:2535-2540`): two reco estimators are carried (A: stored HFS 4-vector; B: collinear q_eSigma + ξP with mass = mx), frame not frozen until WP-E delivers the gen X 4-vector (WP-C C-decisions); γ* from the eSigma photon
  (`esigma_photon.py:19 reco_photon_electron`); reco track mass = pion, gen mass = PDG (parent
  convention). Gen p_X = new writer branch; interim proxy q + x_IP·P. Hadron fiducial = parent
  tracker phase space \|η_lab\| < 1.6, 0.15 ≤ pT ≤ 15 GeV, π/K/p from X, V0 and conversion daughters
  excluded (`D/scripts/t1_particle_level/particle_level.py:76-100`). Primary observables:
  (1/N_D) dn^±/dx_F and (1/N_D) dn^±/dp_T*² in three M_X bins; charge sum and h⁺−h⁻ derived.
  Absolute dσ/dM_X dx_F only after G3 (user: Open Q2).
- D7 **Blinded mask rule.** A bin is masked when the MC-predicted reco signal track count is
  < 50, or purity < 0.35, or stability < 0.35; derived from the cocktail before any data histogram
  in that bin is filled; recorded with sha256 before unblinding; ratified by the user at WP-K.
  Reason: the old N_pred < 10 rule had no purity criterion and the new grid can afford one.
- D8 **Unfolding.** IBU k=4 with the parent engine `eng.ibu_unfold`
  (`P/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py:623`),
  2D response (M_X × x_F) and (M_X × p_T*²) per charge with reco bins split 2×, plus the event-level
  M_X response for N_D; fakes (truth outside fiducial M_Y/\|t\|/x_IP/M_X) as per-reco-bin fractions;
  ND and Reggeon subtracted before unfolding; TUnfold as cross-check only.
- D9 **C_rad.** Computed at generator level on the per-event-normalised observable with a
  ≥1M-event standalone RAPGAP 3.305 pair (`diff_nonrad` IPRO=12 vs `diff_rad` IPRO=1200+HERACLES,
  setup under `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/MC/rapgap/`) only if
  `h1-radiative-correction` and `h1-mc-expert` rule the pair usable for a ratio; otherwise no
  correction and a systematic of max(\|1−C_rad,est\|, 3%). No official non-rad SimRec request now.
- D10 **Specialists are the authority.** Physics/H1OO/MC/Condor steps name the H1 specialist;
  pack leads coordinate. Verdicts on gates G1-G5 are issued by `h1-analysis-critic`.
- D11 **Budget realism.** WP-A is capped (Open Q1); if the cap is reached without G1, the restart
  stops with a negative-result addendum and the parent publishes alone.
- D12 **Veto criterion = efficiency, not energy shape (user, 2026-10-28: "I think we don't care about plug
  energy, just that it's an efficient veto").** The observables are per-event-normalised, so a veto
  inefficiency common to all events cancels; what enters is the data−MC difference in the probability that a
  true large-gap event fails a veto, and its dependence on (M_X, D cell, hadron kinematics). The Plug E
  spectrum shape (G1(a)/G1′(a)) is a diagnostic, not a gate. Evidence (`veto_efficiency_view.md`, odd split,
  `etaMaxClus08` < 2.5): Plug Δf = +0.065…+0.067 (ratio 4.0-4.3), FTS +0.045…+0.047, FMD −0.020…−0.034 and
  consistent with 0 on even; Plug pass-efficiency ratio over the M_X bins and D cells 0.946 → 0.898 (odd),
  0.951 → 0.854 (even).
- D13 **Re-gating (proposed by the lead, accepted by the user 2026-10-28).** G1″ replaces G1′ as the campaign
  gate: (a) the veto signal-loss correction and its systematic (WP-A5), acceptance ≤ 5 % per measurement bin;
  (c) the η_max template fit confined to η_max(0.4) < 3.0 with the ND-generator spread as systematic, G2 ≤ 0.30
  kept (WP-A6); the MC-statistics gate (D7 mask rule) and the ≥ 5× signal-MC request unchanged (Open Q6). The
  Plug/FMD/FTS leg of the G1′ KILL is withdrawn; the LAr-edge mismatch is a fit-range and systematic item, not a
  kill. Rejected: Plug threshold scan before re-gating (kept optional, WP-A7); dropping the Plug veto (D3 iii);
  close-out (WP-A4 recommendation); the modified-simulation campaign (WP-A4 §4, 6-12+ months).
- D14 **Forward-veto excess = correction + systematic (user, 2026-10-28).** Per detector (Plug, FTS, FMD) and per
  (M_X bin of the WP-C start grid, D cell): q = (f_data − f_MC,full)/(1 − f_MC,full) from the deep-gap N−1 view,
  derived on the even split and checked on the odd split, applied to the signal MC (DIFF8 components) as a
  random event-removal probability; systematic = max(|q|/2, odd/even-split spread). Binding (brief): the
  remedy-1 control definition (η_max(0.8) < 1.5, combined stack) is not reused. Lead additions, open to the
  WP-A5 child-plan review: the deep-gap q is applied uniformly over the selection η_max(0.8) < 3.2, and the
  excess of the [2.5, 3.0) Δf over the deep-gap value, weighted by the signal fraction above 2.5, is a further
  systematic term; the ≤ 5 % acceptance is evaluated by propagating q ± σ as event weights through the
  per-event-normalised spectra on the 9012 reco sample.
- D15 (user, 2026-10-28): LRG selection edge for the measurement is η_max(0.8 GeV clusters, `etaMaxClus08`) < 3.0,
  replacing 3.3 (D4). Grounds: WP-A6 odd-split f_ND at 3.3 is 0.372 / 0.332 for M_X [10,20) / [20,40] with 75–90 % of
  it in the unfitted 3.0–3.3 band, against in-fit parts 0.023–0.094; CJC-containment study: ~37 % of LRG events sit at
  3.0–3.3. Consequences: WP-B W2 is settled; WP-A5 propagation range and WP-A6 G2 ruling and purity table are
  re-evaluated at 3.0 (wave 1d); the η_max-cut signal efficiency from MC enters the systematics budget (WP-J) with the
  data/MC agreement of the fitted η_max shape below 3.0 as its bound.

## Gates and Kill Criteria

Thresholds are pre-registered here; changing one needs user sign-off before the affected run. G1 and G1′ are
history (both KILL, 2026-10-27); G1″ is the live campaign gate (user sign-off 2026-10-28, `## Log`).

- **G1 (WP-A, kill gate).** On the Stage-B N−1 sample, stack C, signed-v3 weights, after the
  nominal remedy: (a) Plug N−1 unit-area max \|MC/DATA−1\| ≤ 0.10 in bins with ≥ 20 data entries,
  and \|f_fail,MC − f_fail,DATA\| ≤ 0.02 absolute in each D cell (data fractions 0.207/0.230/0.283
  carry ≤ 0.007 stat; 0.02 ≈ the 10% PASS window of
  `D/docs/t4/datamc_fail_diagnosis_2026-10-19.md:10` applied to a rate); (b) FMD and FTS N−1 < 0.25
  (below the pre-registered FAIL boundary) for at least one admissible cocktail (non-negative weights within
  ±50% of the signed-v3 scales); (c) the §7.1 two-free-template η_max fit on the rebuilt cut-removed
  payload reaches template-statistics-aware toy p ≥ 0.05, coarse and fine, with ≥ 1000 toys, for at
  least one ND alternative (the conventional 5% level; calibrated toys make a χ²/ndf ceiling
  redundant; the prior fits sat at the 300-toy floor p ≤ 0.0033-0.01, so a pass is a ≥ 5-15× change,
  not a marginal one). **Kill:** the WP-A budget (Open Q1; default 10
  Condor-days, 2 calendar weeks, whichever comes first; Condor-days = Σ(job wall × request_cpus)/24 h
  from the Condor logs, tallied by `h1-naf-operator` in the WP-A Log) is spent with (a) or (c) failing. (b) failing alone downgrades to a
  systematic leg, not a kill.
- **G1′ (WP-A2, kill gate; KILL 2026-10-27; superseded 2026-10-28 by G1″, record and verdict kept as history;
  full text and hypothesis matrix in
  `D/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md` D3-D6).** The three G1 clauses with unchanged thresholds, evaluated on the
  odd-`EventNumber` test split after the definitional corrections frozen in
  `D/docs/restart_2026-10-27/wp-a2/s3_correction_record.md` from the even split and the published reference (EPJC 72 (2012)
  2074 definitions): (a) Plug N−1 shape ≤ 0.10 and |Δf_fail| ≤ 0.02 per D cell; (b) FMD and FTS N−1 < 0.25 for one
  admissible cocktail; (c) §7.1 toy p ≥ 0.05, coarse and fine, every D cell and ALL, ≥ 1000 toys, for one of 7071, 7091,
  nd4cat. **Kill:** (a) or (c) fails once the pre-registered H1-H6 matrix is exhausted, or 5 Condor-days / 7 calendar days
  from the first WP-A2 submission are spent. (b) alone downgrades to a systematic leg. Corrections are definitions only;
  abandoning the Plug veto, an x_IP window beyond 0.03 or a new MC request are `user:` items, never corrections.
- **G1″ (WP-A5 and WP-A6; replaces G1′ as the campaign gate, user 2026-10-28, D12-D14).** Efficiency-based, on
  the existing WP-A3 skims, signed-v3 weights and the fine G1′(c) scales:
  (a) **Veto signal loss (WP-A5).** On the deep-gap N−1 view (`etaMaxClus08` < 2.5, the other two vetoes
  applied; DIFF8 purity 0.83-0.97 across the three η_max bins), the data−MC fail-probability difference per
  detector and per (M_X bin, D cell) enters as the MC removal probability q of D14 with systematic
  max(|q|/2, odd/even spread). Acceptance: the propagated relative systematic on (1/N_D) dn/dx_F and
  (1/N_D) dn/dp_T*² is ≤ 0.05 in each measurement bin on the WP-C start grid; a bin above it is masked. Validation: the integrated
  Plug/FTS/FMD N−1 fail fractions reproduce fig2 and `veto_eff.json` (odd Plug D0/D1/D2 data 0.2724 /
  0.2996 / 0.3365; deep-gap Plug Δf +0.065…+0.067) to 1e-6.
  (b) FMD and FTS N−1 shapes: no longer gated; their fail-probability differences are part of (a).
  (c) **Leakage (WP-A6).** The two-template η_max fit on `etaMaxClus04` is confined to η_max < 3.0 (0.05 bins,
  no merged edge bin; the LAr-edge 3.15-3.4 region is excluded as simulation response, WP-A4 §1.1); toy GOF
  on the restricted range is reported (≥ 1000 toys) as a diagnostic, not gated; the ND-generator spread over
  7071 / 7091 / nd4cat is the systematic; the gate is G2.
  (d) The MC-statistics gate (D7 mask rule, WP-C) and the ≥ 5× signal-MC request (Open Q6) are unchanged.
  **Kill:** none beyond the per-bin masks; if every M_X bin is masked under (a) or G2, the restart returns to
  the user. The Q4 user return (campaign authorisation) follows the two records.
- **G2 (WP-A6 on the existing skims, re-confirmed by WP-F on the new production; leakage ceiling).** Per
  retained M_X bin, ND leakage fraction f_ND ≤ 0.30 under the largest of the three ND chains, with
  uncertainty = fit statistical ⊕ generator spread (max |f_ND(7091 or nd4cat) − f_ND(7071)|; nominal 7071 per
  the WP-D/MC-3 verification), fit range η_max(0.4) < 3.0 (G1″(c)). Amended 2026-10-28 (lead, Open Q7): the
  former GOF-dependent branch ("when G1(c) holds … without a GOF pass f_ND ≤ 0.10 with a ±100% correlated
  envelope") is retired, because GOF over the LAr edge is no longer a gate and the generator spread replaces
  the envelope. Bins above the ceiling are masked; if every M_X bin is masked
  the restart stops. Justification: the ceiling caps the leakage term at ≈ 10% per bin (0.30 × a
  combined uncertainty ≤ 1/3 of f_ND), the size of the parent's whole detector-side total
  (median 7.4-7.9%, `sidis_highQ2/docs/RUN_STATUS.md:7`), so one background term cannot dominate
  the budget before the G5 ceiling.
- **G3 (WP-G, σ_r^D(3) gate).** Bin-by-bin corrected x_IP·σ_r^D(3) in the overlapping (β, Q²,
  x_IP) points of EPJC 72 (2012) 2074 at Q² ≥ 150 GeV²: ratio within 1 ± 0.25 for ≥ 80% of the
  points and no point beyond 2σ (combined stat ⊕ syst of both). This replaces the prior plan's
  "within the quoted total uncertainty" (`D/H1DiffractiveHighQ2SIDISPlan.md:327`), a 1σ-per-point
  test that a correct measurement fails for ≈ 1/3 of the points; 0.25 is about twice the combined
  uncertainty of the high-Q² H1 LRG points. Failing G3 stops absolute cross
  sections; the per-event-normalised primary proceeds only if the failure is a pure normalisation
  offset (shape χ² p ≥ 0.05).
- **G4 (WP-H, closure).** Split-sample closure within 1σ_stat in ≥ 90% of retained bins;
  reweighted closure (M_X slope ±30%, x_F slope ±30%, PDISS fraction ±50%) bias ≤ max(0.5σ_stat, 5%).
- **G5 (WP-J, systematic viability).** Median total systematic ≤ 25% and ≤ 50% in every retained
  bin; otherwise mask or stop. Mirrors the adjudication's stop condition at the level the new grid
  can defend.

## Outline

Pointers only; `executor` writes the code in the child plans.

- `P/src/ChargedSidisTreeBuilderHighQ2_v6dif.C` → fork `…_v6dif2.C` (same build, `make bin`):
  add to `diffTree` the gen X and Y 4-vectors and gen largest-gap `M_Y`, `t` (truth banks per
  `h1-mc-expert`); keep every existing branch (`:1196-1300`). Gotcha: ND samples have no Y →
  p_X = full gen HFS less lepton-line photons; set a flag branch.
- `P/scripts/unfolding/esigma_final_2026-10-24/esigma_xf.py` (`xf_frame`, `_boost_to_W_rest`,
  `gen_truth`, `isr_tag`, `e_eff`, `gen_mass`) → fork `xframe_obs.py` under
  `D/scripts/restart_2026-10-27/wp-c/`: boost to the p_X rest frame; reuse `gen_truth` for the
  ISR-only γ*. Invariant: never read the stored `xF_SIDIS_es`/`z_SIDIS_es` (docstring `:1-30`).
- `P/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`
  (`NB_R/NB_G :162-165`, `EVENT_CUTS_RECO`, `TRACK_CUTS_RECO`, `POOL_PDG`) → fork with the
  (M_X, x_F) and (M_X, p_T*²) grids and the OPT-A gen cell; pattern: the additive forks in
  `P/scripts/unfolding/esigma_final_2026-10-24/make_esph_forks.py` (originals md5-pinned).
- `P/scripts/unfolding/esigma_final_2026-10-24/esph_downstream.py` (`:161-217` legs; sizes
  hard-coded `:67-77, :167, :299-302, :374-379`) → fork with grid sizes as parameters; leg names
  (`hfs_jes_*`, `hfs_rces_*`, `hfs_theta_*`, `elecE_*`, `elecTheta_*`) reused.
- `P/scripts/canonical_highQ2_sidis_2026-06-30/radiative_corrections/full_crad_finalize.py`
  (grid constants `Q2X`, `Z`, `P`, `np.zeros(245)`) → rewrite for the new grid under WP-I.
- WP-A reuses `D/scripts/t4/build_localization_n1_mytbl_2026-10-20.py` (cuts `:15-17, :73-75`,
  branches `:106-107`, `ePlug` grid `:64`), `D/scripts/t4/run_section_7_1_twofree_v3_2026-10-20.py`,
  and the Condor pattern `D/condor/t4/localization_diag_2026-10-20/localization_diag.sub`
  (1 CPU, 1800 s; 7091 7200 s). Inputs: `D/data/t2_detector/merged_data_true_2026-10-18/`,
  `D/data/t2_detector/mc_merged_2026-10-18/`, `D/data/t2_detector/mc9012_merged_2026-10-18/`,
  filtered to `D/data/t2_detector/certified_runlist_2026-10-19/` (per-run `veto_fail_rate_PLUG7`,
  `mean_PLUG7` columns exist for period/run-range splits).
- Data shapes: per-bin CSV rows `charge, mx_bin, axis (xF|pt2), bin, n_data, n_sig_pred, f_nd,
  f_reg, f_fake, purity, stability, masked` shared by WP-C, WP-F, WP-H, WP-J.

## Slices

A parent plan: each slice is one package wave; tests and checkpoints live in the package blocks and
their child plans.

1. Wave 1 (parallel): WP-A, WP-B (research), WP-C, WP-D, WP-I (research), WP-L (status line only).
   Checkpoint: G1 verdict record exists; WP-C bin proposal and WP-D gap list exist.
1b. Wave 1b (after G1 KILL, user decision 2026-10-27): WP-A2. Checkpoint: `g1prime_verdict.md` (KILL 2026-10-27).
1c. Wave 1c (after the G1′ KILL and the 2026-10-28 re-gating; parallel): WP-A5, WP-A6; WP-A7 parked.
   Checkpoint: `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md` and `D/docs/restart_2026-10-27/wp-a6/g2_verdict.md`;
   then the Q4 user return before wave 2.
2. Wave 2 (only on G1″(a) and G2 with ≥ 1 retained M_X bin, and user authorisation per Q4): WP-B (decision), WP-E.
   Checkpoint: re-produced trees validated by `h1-data-validator`.
3. Wave 3: WP-F then WP-G. Checkpoint: G2 and G3 verdict records.
4. Wave 4: WP-H, WP-I (execute). Checkpoint: G4 record; C_rad table or bound.
5. Wave 5: WP-J. Checkpoint: G5 record.
6. Wave 6: WP-K, WP-L. Checkpoint: user mask ratification, results package, note section, handoff.

## Verification

End to end: a results package under `D/data/restart_2026-10-27/wp-k/` whose README cites the G1″ records
(WP-A5, WP-A6) and passing records for G2-G5, with per-bin tables for both charges and both axes in all retained M_X bins, a
systematics CSV in the parent `esph_budget_v2.csv` column style, figures reviewed by
`h1-plot-reviewer`, a note section reviewed by `h1-analysis-critic`, and `D/context-handoff.md`
reporting status RESTART-COMPLETE with the trap list corrected.

## Risks

- G1 fails on (c) even with Plug fixed: the η_max shape failure is ND/PDISS modelling, not Plug;
  the G2 fallback (f_ND ≤ 0.10 bins only) may leave too few M_X bins.
- Control-region noise rates contaminated by PDISS M_Y > 1.6 leakage: mitigated by η_max < 1.5 and
  by cross-checking against the per-run `mean_PLUG7` proxy; residual becomes a systematic.
- x_IP purity 58.6% among candidates (`D/docs/t2/mc9012_detector_level_2026-10-18.md:50-51`):
  large fake fractions in the high-M_X bin; D7 may mask it.
- The lab fiducial sculpts x_F coverage; the result is not directly comparable to the 1994 H1
  full-acceptance spectra (stated as a fiducial measurement).
- 9012 is one truth stream for four periods: correlated MC stat; the response-MC stat enters the
  totals as in the parent (`.supervisor/memory/decisions/2026-10-26-response-mc-stat-in-quoted-totals.md`).
- 6671/6677 may be ruled unusable: Reggeon then enters only as a ±100% term on a fraction bounded
  from the elastic/PDISS flux fit; D4's 0.15 ceiling protects the bins.
- The WP-A5 correction is derived at `etaMaxClus08` < 2.5 and applied up to 3.2 (the WP-C track skims used for propagation were built at 3.2; the selection edge is 3.3 per D4; WP-B reconciles the two), where Plug purity falls to
  0.68 / 0.43 and the Plug Δf rises to +0.155 / +0.228 (fig6); if the signal fraction above 2.5 is large, the
  extrapolation term can exceed the 5 % acceptance and mask bins.
- Below η_max 3.0 the fit is still imperfect (χ²/bin 1.8 / 1.5 / 1.0 fine, D0/D1/D2, 7071; fig1 captions):
  f_ND carries a model uncertainty the generator spread may not cover.

## Scope Notes

In: everything in `## Work Packages`. Out: FPS/VFPS; parent release changes; generator retuning;
editing `.supervisor/memory/` (curator only). User decisions `executor` must not reopen: the pivot to
X-frame x_F/p_T*² in M_X/β bins; narrow Plug probe first; re-base on parent conventions; restart
authority for leads (user 2026-10-27, recorded in the supervisor brief); parent-side memory entries
R21/R22; the 2026-10-28 reframing (veto efficiency, not Plug energy shape), the G1″ re-gating, correction +
systematic for the forward-veto excess, and no reuse of the remedy-1 η_max < 1.5 control region (D12-D14). Workspace rules per `CLAUDE.md`; `.sub` files under `D/condor/restart_2026-10-27/`.

## Assumptions

- The merged t2 inputs are complete and still match `D/output_v6dif/` (checked by WP-A step 1 file
  counts and sha256 against `D/docs/t2/b5_operations_signoff_2026-10-21.md`).
- `eng.ibu_unfold` and `derive_response_blocks` accept arbitrary (NB_R+1, NB_G+1) shapes (checked by
  the `engine_shape` test in WP-C, wave 1).
- The standalone RAPGAP 3.305 setup can generate ≥ 1M events per mode in ≤ 1 Condor-day (checked by a
  one-shard timing run in WP-I).
- H1 truth banks allow the gen Y system to be identified in RAPGAP PDISS (checked by `h1-mc-expert`
  in WP-E slice 1).

## Work Packages

Block fields follow `plan-writing`; `Lead`, `Condor`, `Output` and `Log` are added per the brief.
Leads are pack leads; named H1 specialists are dispatched by them for the listed steps. WP-A3 (response-reweighting
scoping) and WP-A4 (forward-response fix assessment) were single research dispatches recorded in `## Log` only.

### WP-A: Plug forward-veto probe and gate G1
- Goal: attribute and correct the Plug veto deficit, re-run the N−1 and η_max GOF gates; done when
  the G1 verdict record exists (pass or kill)
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-a/`
- Deps: none
- Stages: plan, execute
- Status: done
- Result: G1 KILL 2026-10-27 — see sidis_highQ2/diffractive/docs/restart_2026-10-27/wp-a/g1_verdict.md
- Lead: `executor` (child plan by `planner`); specialists `h1-naf-operator` (Condor),
  `h1-mc-expert` (overlay semantics; one lookup whether H1 random-trigger event files of EPJC 72
  are accessible), `h1-analysis-critic` (verdict)
- Condor: yes (existing t4 scripts; ≤ 10 Condor-days)
- Scope Notes: steps: (1) period and run-range split of data Stage-B Plug fail fraction and E_Plug
  spectrum vs MC; (2) deep-gap control region per D2 and ND-dominated region (η_max > 3.5) for the
  energy scale; threshold scan; (3) remedies per D3, FMD/FTS noise correction by the same method;
  (4) N−1 re-run and §7.1 η_max fit on the rebuilt payload; (5) verdict. Tests: `control_disjoint:
  control-region event IDs ∩ test-sample IDs -> empty`; `fail_fraction_recomputed: data
  0.207/0.230/0.283 reproduced on the frozen list before any remedy -> |Δ| < 0.002`;
  `threshold_pre_registered: thresholds in the verdict equal this plan's G1 -> identical`.
  Checkpoint: `D/docs/restart_2026-10-27/wp-a/g1_verdict.md` with the numbers of G1(a)-(c).
- Artifacts: plan-wpA-plug-probe.md (default path); `D/docs/restart_2026-10-27/wp-a/g1_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-a/`, `D/data/restart_2026-10-27/wp-a/`
- Log: —

### WP-A2: root-cause diagnosis of the η_max / Plug failure and gate G1′
- Goal: decide whether the G1 failure is definitional (η_max convention, Plug DB threshold and scale, overlay noise, fit
  structure, simulation flags) or physical, fix justified definitional corrections from the even split and the published
  reference, and rule G1′ on the odd split; done when `D/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md` exists (pass or kill)
- Owns: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a2/`
- Deps: WP-A (done; its partials, scripts and records are the inputs)
- Stages: plan, execute
- Status: done
- Result: G1′ KILL 2026-10-27 — see sidis_highQ2/diffractive/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md
- Lead: `executor` (child plan by `planner`); specialists `h1-mc-expert` (Plug DB semantics, overlay/noise and
  generator-class readings), `h1-naf-operator` (Condor, tally), `h1-analysis-critic` (pre-registration, correction record,
  verdict); `h1-data-validator` only if a merged file is written
- Condor: yes (forks of the WP-A scripts over the 17 merged files; ≈ 0.2 of ≤ 5 Condor-days, ≤ 7 calendar days)
- Scope Notes: pre-registered hypotheses H5 (Plug DB threshold/`EnCor` per period), H1 (η_max threshold, cut, vertex,
  electron vs EPJC 72 (2012) 2074), H6 (beam tilt / z-vertex flags), H4 (no-gap inclusive control: forward-edge response vs
  ND forward flow), H3 (fit templates, E−p_z, x_IP window, out-of-fiducial admixture), H2 (overlay rate / forward noise per
  period), in that order; G1′ text in the child plan D6 (quoted in the gates section above); user items only for
  physics-scope changes. Tests: `fork_default_identity`, `split_disjoint_complete`, `corrections_are_definitions`,
  `record_frozen_before_s4`, `threshold_pre_registered` (child plan). Checkpoint: `g1prime_verdict.md` with the clause numbers.
- Artifacts: `D/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md`; `g1prime_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-a2/`, `D/data/restart_2026-10-27/wp-a2/`, `D/plots/restart_2026-10-27/wp-a2/`
- Log: —

### WP-A5: forward-veto signal-loss correction and gate G1″(a)
- Goal: derive the data−MC signal-event veto fail-probability difference for Plug, FTS and FMD per (M_X bin, D cell)
  from the deep-gap N−1 view, assign the systematic, propagate it to the measurement bins and validate against the
  fig2 integrated fail fractions; done when `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md` exists with the
  per-bin ≤ 0.05 table (pass per bin or mask)
- Owns: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a5/`
- Deps: none (inputs: the 17 WP-A3 skims, `D/scripts/restart_2026-10-27/gate_evidence/veto_eff_hist.py` and
  `D/scripts/restart_2026-10-27/wp-a3/fact_wpa3.py` (`masks`), `D/data/restart_2026-10-27/gate_evidence/veto_eff.json`;
  M_X edges = the WP-C start grid)
- Stages: plan, execute
- Status: execute
- Lead: `executor` (child plan by `planner`); `h1-naf-operator` (Condor), `h1-systematics` (systematic assignment
  review), `h1-analysis-critic` (verdict)
- Condor: yes (forks of `veto_eff_hist.py` over the 17 skims; ≈ 0.05 Condor-days)
- Scope Notes: steps: (1) deep-gap N−1 view (A_pre, `etaMaxClus08` < 2.5 or sentinel, the other two vetoes applied,
  x_IP < 0.04, reco M_X > 3 GeV) per detector, per (M_X bin [4,10), [10,20), [20,40] GeV, D cell), even and odd
  splits, three ND chains with the fine G1′(c) scales; Δf = f_data − f_MC,full and q = Δf/(1 − f_MC,full), frozen
  from the even split before the odd split is read; (2) systematic per cell = max(|q|/2, odd/even spread), chain
  spread reported as a diagnostic; (3) the [2.5, 3.0) extrapolation term of D14; (4) propagation: q ± σ as event
  weights on the DIFF8 reco events of the 9012-based cocktail, per-event-normalised x_F and p_T*² spectra per M_X
  bin in the WP-C interim frame, relative change = the G1″(a) systematic per measurement bin; (5) validation
  against fig2 and `veto_eff.json`; (6) verdict by `h1-analysis-critic`. Not allowed: the remedy-1 control region
  (η_max(0.8) < 1.5, combined stack, `D/docs/restart_2026-10-27/wp-a/s1_period_split_record.md`) in any step; any
  Plug energy-shape criterion. Tests: `fig2_consistency: integrated Plug N−1 f_fail, odd, D0/D1/D2 -> 0.2724 /
  0.2996 / 0.3365 within 1e-6 of summary.json`; `deep_gap_reproduced: Plug ALL odd Δf (7071) -> equals
  veto_eff.json deep_gap_eta_lt_2p5 within 1e-6`; `no_control_1p5: scripts contain no η_max < 1.5 control mask ->
  true`; `even_frozen_first: correction JSON sha256 recorded before any odd-split read -> true`; `syst_floor:
  every cell σ_q ≥ |q|/2 -> true`; `g1pp_a_table: relative σ on both spectra per (charge, M_X, bin) -> written
  with a ≤ 0.05 flag`. Checkpoint: `g1pp_a_verdict.md`. Wave 1d: re-rule at the D15 edge η_max(0.8) < 3.0; existing even-split freezes stand.
- Artifacts: plan-wpA5-veto-correction.md (default path); `D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md`;
  `D/data/restart_2026-10-27/wp-a5/veto_correction.json` (q, σ per detector × M_X × cell × split)
- Output: `D/docs/restart_2026-10-27/wp-a5/`, `D/data/restart_2026-10-27/wp-a5/`, `D/plots/restart_2026-10-27/wp-a5/`
- Log: —

### WP-A6: leakage re-fit at η_max < 3.0, gate G1″(c) and G2 on the existing skims
- Goal: re-run the two-template η_max fit confined to `etaMaxClus04` < 3.0 for 7071, 7091 and nd4cat per D cell,
  derive f_ND per M_X bin with fit ⊕ generator-spread uncertainty and purity per measurement bin, rule G2; done when
  `D/docs/restart_2026-10-27/wp-a6/g2_verdict.md` exists
- Owns: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a6/`
- Deps: none (inputs: `D/data/restart_2026-10-27/wp-a2/s4_g1p/payloads/odd_frozen_{7071,7091,nd4cat}.json`, the
  WP-A2 fit fork `D/scripts/restart_2026-10-27/wp-a2/fit_7p1_wpa2.py`, the WP-A3 skims; M_X edges = the WP-C
  start grid; WP-C `f_fake`)
- Stages: plan, execute
- Status: execute
- Lead: `executor` (child plan by `planner`); `h1-mc-expert` (ND forward-flow reading where the chains disagree),
  `h1-analysis-critic` (verdict)
- Condor: yes (fits per cell × chain × M_X bin with ≥ 1000 toys; ≈ 0.1 Condor-days)
- Scope Notes: fit basis as the WP-A2 frozen set (`etaMaxClus04`, cut-removed FIT sample, 0.05 bins) with the
  range changed to [−1, 3.0) and no merged edge bin; the [3.0, 3.4) and [3.4, 5) counts are reported as
  out-of-fit diagnostics. Fits per D cell and, where the FIT sample holds ≥ 100 data events per bin, per M_X bin;
  otherwise the M_X dependence comes from the templates under the per-cell scales. f_ND per M_X bin in the
  veto-passing η_max(0.8) < 3.3 selection (selection edge aligned to the WP-A2 S3 published definition, 2026-10-28; WP-B owns any later change) from the fitted s_ND; nominal 7071, systematic = max |f_ND(7091 or
  nd4cat) − f_ND(7071)|; G2 ceiling on the largest chain. Purity per measurement bin = 1 − f_ND − f_Reg − f_fake
  from the scaled ND track spectra in the WP-C interim frame, WP-C's f_fake and the D4 Reggeon fraction. Toy GOF
  reported, not gated. Tests: `range_frozen: fit edges -> [−1, 3.0), 0.05 bins, recorded before the odd-split
  fit`; `payload_identity: templates equal the WP-A2 odd_frozen payloads restricted to < 3.0 -> true`;
  `g2_table: every M_X bin has f_ND, fit σ, chain spread, ceiling flag -> true`; `spread_as_syst: quoted σ_syst =
  max chain deviation -> true`; `purity_table: purity per (charge, M_X, bin) in both axes -> written`.
  Checkpoint: `g2_verdict.md`. Wave 1d: re-rule at the D15 edge η_max(0.8) < 3.0; existing even-split freezes stand.
- Artifacts: plan-wpA6-leakage-refit.md (default path); `D/docs/restart_2026-10-27/wp-a6/g2_verdict.md`;
  `D/data/restart_2026-10-27/wp-a6/f_nd_per_mx.csv`
- Output: `D/docs/restart_2026-10-27/wp-a6/`, `D/data/restart_2026-10-27/wp-a6/`, `D/plots/restart_2026-10-27/wp-a6/`
- Log: —

### WP-A7: parked follow-ups (optional, non-gating)
- Goal: hold the deferred studies that gate nothing; done when each item is run or formally dropped in WP-L
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-a7/`
- Deps: item (1) WP-E (writer fork); item (2) WP-A5 (correction JSON)
- Stages: execute
- Status: blocked: execute: parked — opened only by user request or by WP-E for item (1)
- Lead: `executor`; `h1-mc-expert` (truth banks), `h1-systematics` (item 2 as an optional leg)
- Condor: yes if opened (item 1 rides the WP-E production; item 2 ≈ 0.05 Condor-days)
- Scope Notes: (1) unbiased truth η_max: recompute from H1PartMC status-0 particles (0.4 and 0.8 GeV thresholds) as
  a WP-E writer-fork branch; definition of the biased HAT `EtaMaxGen` per the `h1-mc-expert` fact (`## Log`
  2026-10-28). (2) Plug threshold scan 7/10/15 GeV as an
  optional systematic leg on the WP-A5 correction; the user chose re-gating without it. Tests:
  `etamax_gen_fwd_vs_hat: on 9012 the new branch ≥ hatEtaMaxGen event by event -> fraction with new > hat
  reported`; `threshold_scan_table: q per threshold per (M_X, cell) -> written`. Checkpoint:
  `D/docs/restart_2026-10-27/wp-a7/followups.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-a7/followups.md`
- Output: `D/docs/restart_2026-10-27/wp-a7/`, `D/data/restart_2026-10-27/wp-a7/`
- Log: —

### WP-B: selection and kinematic-range re-decision
- Goal: re-decide every selection and range item from scratch (register R3-R10, R22); done when the
  selection record is signed by `h1-analysis-critic`
- Owns: `D/docs/restart_2026-10-27/wp-b/`
- Deps: WP-A5 (veto corrections and their systematic), WP-A6 (η_max fit range, f_ND per M_X bin), WP-C (x_IP
  purity per M_X bin); the kinematic research proceeds in wave 1
- Stages: research, plan
- Status: research
- Lead: `planner` (research by `planner-worker`s); `h1-analysis-critic` signs
- Condor: no (reads WP-A and WP-C products)
- Scope Notes: inputs D4 start values; outputs per item: value, alternatives, evidence, systematic
  variation. Includes the Reggeon-fraction check (D4) and the x_IP purity per M_X bin from WP-C.
  Test: `every_register_row_closed: R3-R10, R22 each have keep/change with evidence -> true`.
  Checkpoint: `D/docs/restart_2026-10-27/wp-b/selection_record.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-b/selection_record.md`
- Output: `D/docs/restart_2026-10-27/wp-b/`
- Log: —

### WP-C: observable definition, boost, binning and blinded mask
- Goal: fix D6 operationally, measure resolutions and propose the grid; done when the binning record
  with per-bin purity/stability and the blinded mask is written
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-c/`
- Deps: none for the stat study (interim gen proxy q + x_IP·P); mask freeze: WP-A6 (f_ND per M_X bin enters the
  D7 purity) and the WP-B selection; final gen frame after WP-E
- Stages: research, plan, execute
- Status: blocked: plan: user U1 grid/mask rule (binning_record.md §8; re-surfaced 2026-10-28 with the ≥ 5× signal-MC request, Open Q6)
- Lead: `executor` (stat study), `planner` (binning record); `h1-unfolder` reviews purity/stability,
  `h1-analysis-critic` reviews the frame definition
- Condor: yes (reads 91 GB of v6dif MC; data on the login node is not allowed either: 658 files)
- Scope Notes: start grid (estimate, worker arithmetic from ~47k weighted (~30k raw MC) reco signal tracks/charge; the earlier 84k figure was the particle-level count (WP-C binning_record.md)): M_X [4,10),
  [10,20), [20,40] GeV; x_F 6 bins [−1,−0.5,−0.2,0,0.2,0.5,1]; p_T*² 4 bins [0,0.25,0.75,2,10] GeV²;
  30 gen bins per charge, reco 2× split. Deliver M_X, x_IP, β, x_F, p_T*² resolutions (T3 gap),
  purity/stability per bin, mask per D7, the T0→reco acceptance chain reconciliation (RA §3.9).
  Tests: `boost_closure: p_X boosted into its own frame -> (M_X, 0,0,0) within 1e-6`;
  `xf_range: all tracks -> −1 ≤ x_F ≤ 1`; `mask_blind: mask sha256 recorded before data fill -> true`;
  `engine_shape: ibu_unfold on a (61,31) toy -> converges, closure within 1e-3` (Assumption 2).
  Checkpoint: `D/docs/restart_2026-10-27/wp-c/binning_record.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-c/binning_record.md`, mask JSON with sha256
- Output: `D/docs/restart_2026-10-27/wp-c/`, `D/data/restart_2026-10-27/wp-c/`
- Log: —

### WP-D: MC inventory re-judgement and gap list
- Goal: close the 6671/6677 validity question, PDISS coverage, ND third generator, model variants,
  C_rad pair usability; done when the gap list is signed with owners
- Owns: `D/docs/restart_2026-10-27/wp-d/`
- Deps: none
- Stages: research
- Status: done
- Lead: `researcher`; `h1-mc-db` first (provenance: SW=70 hat/mods reco validity vs SW=84; SW=77
  re-sims 8329-8341; any SW=84 light-flavour Reggeon; ep0304 PDISS; third ND generator with ODS;
  /acs access to 8221/7448 DST), then `h1-mc-expert` (response-level judgement); the RAPGAP
  diff_rad/diff_nonrad usability ruling belongs to WP-I
- Condor: no
- Scope Notes: worker gap table (2026-10-27) is the starting list; luminosity CSV `family` column is
  known mislabelled (do not read it). Any new official MC request is a `user:` item (Open Q3), never
  filed by an agent. Test: `every_gap_has_owner_and_cost -> true`. Checkpoint:
  `D/docs/restart_2026-10-27/wp-d/mc_gap_list.md`.
- Artifacts: research-mc-gaps.md (default path); `D/docs/restart_2026-10-27/wp-d/mc_gap_list.md`
- Output: `D/docs/restart_2026-10-27/wp-d/`
- Log: —

### WP-E: re-based production (writer fork, data and MC re-run)
- Goal: trees with gen X/Y 4-vectors and all v6dif branches for 4 data periods and 13 MC samples,
  validated; done when `h1-data-validator` passes entry counts against v6dif
- Owns: `P/src/ChargedSidisTreeBuilderHighQ2_v6dif2.C`, `D/output_v6dif2/`,
  `D/condor/restart_2026-10-27/wp-e/`, `D/docs/restart_2026-10-27/wp-e/`
- Deps: WP-A5 and WP-A6 (gate G1″ records `g1pp_a_verdict.md`, `g2_verdict.md`; D1 and the Q4 user return), WP-C
  (branch list), WP-D (sample list)
- Stages: plan, execute
- Status: blocked: plan: gate G1″ records (WP-A5, WP-A6) and the WP-C branch list (re-opened 2026-10-28)
- Lead: `executor`; `h1-mc-expert` (truth-bank X/Y definition), `h1-naf-operator` (Condor, merges),
  `h1-data-validator`
- Condor: yes (data 658 jobs ≈ 30 min; MC evls + stage2 chains, est. 1-3 Condor-days)
- Scope Notes: D5; compile with `make bin`; one-file login-node smoke test only. Per-sample weights
  recomputed on the new selection with the v3 luminosities (R15). Tests: `branch_parity: every v6dif
  branch present in v6dif2 -> true`; `genX_closure: gen M_X from the new 4-vector vs hatMXGen on
  9012 -> median |Δ|/M_X < 0.02`; `entry_counts: per file equal v6dif -> true`. Checkpoint:
  validator record in `D/docs/restart_2026-10-27/wp-e/`.
- Artifacts: plan-wpE-production.md (default path); validator record
- Output: `D/output_v6dif2/`, `D/docs/restart_2026-10-27/wp-e/`
- Log: —

### WP-F: leakage model, fakes and forward-detector response; gate G2
- Goal: per-M_X-bin ND and Reggeon fractions with a validated normalisation, PDISS mixture and
  fake fractions, noise corrections applied; done when the G2 record exists
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-f/`
- Deps: WP-E, WP-B, WP-A5 (correction applied to the signal MC before the fit), WP-A6 (fit range and method)
- Stages: plan, execute
- Status: blocked: plan: WP-E (re-opened 2026-10-28)
- Lead: `executor`; `h1-mc-expert` (mixture, M_Y spectrum), `h1-analysis-critic` (G2 verdict)
- Condor: yes
- Scope Notes: η_max two-template fit per the §7.1 design with the WP-A6 frozen range (< 3.0) on the new selection,
  three ND chains, WP-A5 corrections applied first; re-confirms G2;
  mixture 1.20 ratio re-derived (R11); DF-SYS-01/02 re-evaluated here (R19). Tests:
  `fractions_sum: f_sig+f_nd+f_reg+f_fake per bin -> 1 ± 1e-6`; `g2_table: every M_X bin has f_ND
  and its uncertainty -> true`. Checkpoint: `D/docs/restart_2026-10-27/wp-f/g2_verdict.md`.
- Artifacts: plan-wpF-leakage.md (default path); `g2_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-f/`, `D/data/restart_2026-10-27/wp-f/`
- Log: —

### WP-G: detector-level distributions and the σ_r^D(3) gate G3
- Goal: data vs cocktail control plots and the inclusive gate; done when the G3 record exists
- Owns: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-g/`
- Deps: WP-F
- Stages: execute
- Status: blocked: execute: WP-F (re-opened 2026-10-28)
- Lead: `executor`; `h1-plot-reviewer`, `h1-analysis-critic` (G3 verdict)
- Condor: yes
- Scope Notes: distributions: η_max, E_Plug, nFmu12, nFts28, x_IP, M_X, β, Q², x, y, E−p_z, track
  multiplicity, reco x_F and p_T*² per M_X bin, per period. G3 per the gate section (bin-by-bin
  correction suffices for a gate). Tests: `plot_set_complete: every listed variable has a data/MC
  figure with ratio panel -> true`; `g3_points: ratio table vs EPJC 72 (2012) 2074 -> written`.
  Checkpoint: `D/docs/restart_2026-10-27/wp-g/g3_verdict.md`.
- Artifacts: `g3_verdict.md`; figures under `D/plots/restart_2026-10-27/wp-g/`
- Output: `D/docs/restart_2026-10-27/wp-g/`, `D/plots/restart_2026-10-27/wp-g/`
- Log: —

### WP-H: response matrices, unfolding and closure; gate G4
- Goal: unfolded per-event-normalised spectra per charge with statistical covariance; done when the
  G4 record exists
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-h/`, builder and downstream forks under
  `D/scripts/restart_2026-10-27/wp-h/`
- Deps: WP-G, WP-C
- Stages: plan, execute
- Status: blocked: plan: WP-G (re-opened 2026-10-28)
- Lead: `executor`; `h1-unfolder` (method, closure), `h1-analysis-critic`
- Condor: yes
- Scope Notes: D8. Tests: `split_closure`, `reweighted_closure` per G4; `fake_fraction_bounds: 0 ≤ f_i
  < 0.5 -> true`. Checkpoint: `D/docs/restart_2026-10-27/wp-h/g4_verdict.md`.
- Artifacts: plan-wpH-unfolding.md (default path); `g4_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-h/`, `D/data/restart_2026-10-27/wp-h/`
- Log: —

### WP-I: radiative correction strategy
- Goal: C_rad table or bound per D9; done when `h1-radiative-correction` signs the record
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-i/`
- Deps: WP-C (observable); execute stage after gate G1″ (WP-A5, WP-A6; D1)
- Stages: research, execute
- Status: blocked: execute: WP-C grid (Open Q6) and gate G1″ (re-opened 2026-10-28)
- Lead: `executor`; `h1-radiative-correction`, `h1-mc-expert`
- Condor: yes (standalone RAPGAP generation, ≤ 1 Condor-day)
- Scope Notes: research stage in wave 1 (pair usability, timing shard); execute after WP-C's grid.
  Tests: `event_count: ≥ 1M events per mode -> true`; `ratio_cancellation: C_rad on the per-event
  normalised x_F spectrum -> every bin within [0.9, 1.1] or flagged`. Checkpoint:
  `D/docs/restart_2026-10-27/wp-i/crad_record.md`.
- Artifacts: `crad_record.md`, C_rad CSV or bound statement
- Output: `D/docs/restart_2026-10-27/wp-i/`, `D/data/restart_2026-10-27/wp-i/`
- Log: —

### WP-J: systematics budget; gate G5
- Goal: per-bin budget in the parent column style; done when the G5 record exists
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-j/`
- Deps: WP-H, WP-I, WP-A5 (veto-correction leg), WP-A6 (ND-spread leg)
- Stages: plan, execute
- Status: blocked: plan: WP-H (re-opened 2026-10-28)
- Lead: `executor`; `h1-systematics` (budget), `h1-analysis-critic` (G5)
- Condor: yes
- Scope Notes: parent legs reused as MC-only response swaps (HFS JES/RCES/θ, electron E/θ, tracking,
  IBU stat/model/reg, response-MC stat, C_rad); diffractive legs: leakage (fit or envelope per G2),
  Reggeon ±50%, PDISS ratio ±7.2% and M_Y spectrum slope, forward-veto correction (WP-A5: max(|q|/2, odd/even
  spread)), ND-generator spread (WP-A6), η_max 3.1/3.3, optional Plug threshold 10/15 GeV (WP-A7), FMD
  hit-pair variation,
  M_X scale, x_IP-flux and β reweighting, t-slope. Lumi only for absolute results. Tests:
  `budget_columns_match_parent_style -> true`; `g5_ceilings: median ≤ 25%, max ≤ 50% -> per bin`.
  Checkpoint: `D/docs/restart_2026-10-27/wp-j/g5_verdict.md`.
- Artifacts: plan-wpJ-systematics.md (default path); budget CSV; `g5_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-j/`, `D/data/restart_2026-10-27/wp-j/`
- Log: —

### WP-K: results, tables and figures
- Goal: results package with tables, figures and MC comparisons; done when the user ratifies the
  mask and `h1-plot-reviewer` accepts the figures
- Owns: `D/{docs,data,plots}/restart_2026-10-27/wp-k/`
- Deps: WP-J
- Stages: execute, write
- Status: blocked: execute: WP-J (re-opened 2026-10-28)
- Lead: `executor` (tables, figures with the `dataviz` skill), `writer` (README); `h1-plot-reviewer`
- Condor: no
- Scope Notes: comparisons: RAPGAP Fit B elastic + PDISS cocktail; H1 1998 shapes qualitatively
  (fiducial caveat); absolute dσ only if G3 passed. Test: `tables_cover_retained_bins -> true`.
  Checkpoint: `D/data/restart_2026-10-27/wp-k/README.md` citing G1-G5 records.
- Artifacts: results package
- Output: `D/data/restart_2026-10-27/wp-k/`, `D/plots/restart_2026-10-27/wp-k/`
- Log: —

### WP-L: analysis note section, handoff and memory
- Goal: note section, handoff update and memory close-out; done when SAVE-CONTEXT.md steps are done
  and curator has closed out
- Owns: `D/context-handoff.md`, the note section, `D/docs/restart_2026-10-27/wp-l/`
- Deps: wave-1 slice: none (status line "RESTART under this plan" and the R18 trap correction);
  final slice: WP-K
- Stages: write
- Status: write
- Lead: `writer`; `h1-analysis-note-writer` (LaTeX note), `curator` (memory)
- Condor: no
- Scope Notes: `science-writing-style`; no new loose files; handoff edited by section. Test:
  `handoff_sections_intact: grep -n '^## ' before/after -> same headings plus RESTART status`.
  Checkpoint: curator close-out line in `.supervisor/history.md`.
- Artifacts: note section under
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/`,
  handoff diff
- Output: `D/docs/restart_2026-10-27/wp-l/`
- Log: —

## Open Questions

Material items only; each with options and a recommendation.

- **Q1 (user:) WP-A budget cap.** Options: (a) 10 Condor-days / 2 calendar weeks; (b) 20
  Condor-days / 4 weeks; (c) no cap. Recommendation: (a); it covers the three remedies with the
  existing t4 machinery (1 CPU, ≤ 2 h shards) and keeps the kill criterion meaningful.
- **Q2 (user:) Observable statement.** Options: (a) per-event-normalised spectra within the parent
  lab fiducial as primary, absolute cross sections secondary after G3; (b) absolute cross sections
  primary; (c) acceptance-corrected to the full X-frame phase space. Recommendation: (a); it is the
  prior-art form, it cancels normalisation systematics, and (c) would extrapolate with the MC whose
  forward description is the problem. Sub-item, x_F sign: (i) proton/Pomeron-positive as in the
  parent (D6 default); (ii) photon-positive as in PLB 428. Recommendation: (i); comparisons with
  PLB 428 flip the sign.
- **Q3 (user:) New official MC requests** (non-rad diffractive SimRec, SW=84 light-flavour Reggeon,
  Fit A or ARIADNE at detector level, ep0304 PDISS). Options: (a) none before G1; (b) file the
  Reggeon and ep0304 PDISS requests now; (c) file all. Recommendation: (a); every item has a
  bounded fallback (D9, D4 ceiling, 9158 borrow with its systematic) and the request procedure is
  undocumented in the workspace.
- **Q4 (user:) Campaign envelope after G1.** Options: (a) authorise WP-E..WP-L at ≈ 15-30
  Condor-days over 2-3 months on a G1 pass without further gating; (b) return to the user after
  G2 as well. Recommendation: (b); G2 is the second point where the analysis can still die cheaply.

- **Q5 (user:) Binning axis.** Options: (a) M_X bins with ⟨β⟩ quoted per bin; (b) β bins with
  ⟨M_X⟩ quoted; (c) both, as two presentations of the same unfolded sample. Recommendation: (a);
  X-frame hadron spectra scale with M_X (the e⁺e⁻ analogy of PLB 428), M_X is measured directly,
  and (c) doubles the unfolding and systematics work for one sample.

- **Q6 (user:) U1 grid/mask rule and the ≥ 5× elastic signal-MC request** (re-surfaced 2026-10-28; options
  (a)-(d) in `D/docs/restart_2026-10-27/wp-c/binning_record.md` §8). Recommendation: WP-C's, (c) D7+ with reco
  bins split only along the observable axis, (d) the coarser fixed grid as fallback; the MC request is an
  external-cost item (Open Q3) and is not filed by an agent. Needed before WP-C's mask freeze and WP-I execute.
- **Q7 (user:, may override a lead decision) G2 GOF branch.** Options: (a) retire the former no-GOF branch
  (f_ND ≤ 0.10, ±100 % envelope), as written in G2 above; (b) keep it, triggered by toy p < 0.05 on the
  restricted range. Recommendation: (a); the generator spread replaces the envelope and GOF over the LAr edge is
  no longer a gate.
- Q8 (user:) selection edge for the measurement: keep η_max(0.8) < 3.3 (only M_X [4,10) survives G2, 2 bins per charge), move to η_max < 3.0 (WP-A6 in-fit f_ND 0.009–0.094 in all M_X bins; costs ~37 % of events; η_max-cut efficiency from MC becomes a systematic), or move to 2.5 (ND ≤ 0.12 before fitting, Plug Δ +0.065, keeps ~37 % of events). Supervisor recommendation: 3.0. Decision binds WP-B W2, WP-A5 propagation range and the WP-A6 re-ruling. — answered 2026-10-28: 3.0 (D15)

Future user touchpoint (not a question now): mask ratification at WP-K (D7); the Q4 return after wave 1c.

## Log

- 2026-10-27 decision: user — restart from scratch; prior diffractive-branch decisions are inputs to re-judge, not binding; leads may take new decisions and log them (supervisor surfaces material ones)
- 2026-10-27 decision: user — revival path = narrow Plug probe first under kill gate G1
- 2026-10-27 decision: user — observables pivot to x_F / p_T*² in the X rest frame, M_X bins, Q² > 150 GeV²
- 2026-10-27 decision: supervisor (on user authorization) — Q1 WP-A budget cap 10 Condor-days / 2 calendar weeks
- 2026-10-27 decision: supervisor (on user authorization) — Q2 per-event-normalised spectra within the parent lab fiducial, absolute only after G3; x_F sign Pomeron-positive as in the parent
- 2026-10-27 decision: supervisor (on user authorization) — Q3 no new official MC request before G1
- 2026-10-27 decision: supervisor (on user authorization) — Q4 return to the user after G2 before committing the full campaign
- 2026-10-27 decision: supervisor (on user authorization) — Q5 bin in M_X, quote ⟨β⟩ per bin, no β axis
- 2026-10-27 supervisor: wave 1 dispatched — WP-A plan (planner), WP-C/WP-D/WP-I research (researcher), WP-L wave-1 slice (writer)
- 2026-10-27 WP-L (wave-1 slice): handoff RESTARTED status section added and R18 random-trigger trap corrected; deferred: note section (waits on WP-K)
- 2026-10-27 WP-I research: ruling USABLE-WITH-CONDITIONS on a corrected IPRO=1200 RAPGAP 3.305 pair (current diff_rad misconfigured; no non-rad diffractive sample exists), timing done (login node 2.8-7.2 s/2000 ev; full pair ≈1-6 core-days); recommend applied per-event C_rad with fallback max(|1−C|,6%); deferred: execute stage (gated on G1 and the WP-C grid)
- 2026-10-27 decision: supervisor (on user authorization) — WP-I: apply a computed per-event C_rad per M_X bin after unfolding (reverses revival-assessment item 5); fallback leg max(|1−C|, 6%); parent FSR convention kept
- 2026-10-27 WP-A: child plan written at sidis_highQ2/diffractive/docs/restart_2026-10-27/wp-a/wp-a_plan.md, critic PASS round 2; deferred: none (open questions resolved by supervisor below)
- 2026-10-27 decision: supervisor (on user authorization) — WP-A: A5 ePlug drawn from control-region excess spectrum for removed MC events; G1(c) pass scope = ALL D cells coarse+fine; A9 remedy-3 reading = FMD/FTS N−1 ≤ 0.10 plus Stage-B yield within 1 ± 0.10 after PDISS M_Y>1.6 term; η_max>3.5 energy-scale check assigned to S1 as diagnostic
- 2026-10-27 WP-D: done, mc_gap_list.md at restart_2026-10-27/wp-d/; deferred: Q3 user: items U1-U5 (no request before G1), production additions to WP-E (9013, 6674/8335, 8329-8341, 8221/7448, optional MC13)
- 2026-10-27 WP-D/MC-3: 7071 verified RAPGAP ME+PS (gen_5031.txt IFPS=3; DB author correction), 7091 CDM; prior envelope defect is period mismatch, not class; same-period pair 7091 vs 7094 added as pre-registered S3 alternative (mc3_7071_cascade_mode.md)
- 2026-10-27 WP-C: binning record, resolutions, two candidate grids and 9 hashed blinded masks (G2), acceptance chain reconciled, engine_shape PASS; signal MC stats are the binding limit (9012 = 0.16× data lumi); deferred: grid and mask freeze pending user U1 and the WP-B selection, final frame pending WP-E gen X frame
- 2026-10-27 supervisor: WP-A dispatched to executor; WP-C U1 and the ≥5× signal-MC request batched with the G1 verdict for the user
- 2026-10-27 WP-A: G1 KILL (h1-analysis-critic; remedies 1/2/3 + nd4cat run, (a) max|Δf| 0.037-0.046, (b) best cocktail 0.68-0.76, (c) max p 0.011, all ALL fits at floor, 0.027 Condor-days, day 1/14; commit 6c209e7); deferred: per-detector-noise and Plug energy-scale variants (not pre-registered); root cause of ND η_max template shape failure unknown
- 2026-10-27 supervisor: WP-E..WP-K blocked on user decision after G1 KILL; U1 grid/mask and the ≥5× signal-MC request deferred with them

- 2026-10-27 decision: user — after G1 KILL: root-cause diagnosis of η_max/Plug with new pre-registered gate G1′; full campaign only if G1′ passes; close-out, full campaign now, and proton tagging not chosen
- 2026-10-27 planner (WP-A2): program plan amended (WP-A2 block, G1′ gate, WP-E..K blocked on G1′); child plan written at sidis_highQ2/diffractive/docs/restart_2026-10-27/wp-a2/wp-a2_plan.md; deferred: none
- 2026-10-27 supervisor: WP-A2 child plan accepted on user authorization (G1′ thresholds = G1; conditional user: items Q1 Plug-veto abandonment / Q2 ND MC request surfaced only if triggered); WP-A2 dispatched to executor
- 2026-10-27 WP-A2: G1′ KILL by h1-analysis-critic ((a) Plug shape 0.95–0.98, Δf_fail −0.21…−0.24; (c) toy p ≤ 0.018, ALL at floor, for 7071/7091/nd4cat; frozen set η_max08<3.3 N−1, η_max04 fit, range <3.4+[3.4,5), record sha 4b2b5e26…d719; remedy-1 excluded on D5); H5/H6/H3 not supported, H1 response-only, H4 supported true-detector, H2 no admissible correction; 0.066 Condor-days; commit 208a99a; deferred: close-out decision to user (D6 KILL), remedy-1 systematic leg not run (non-gating)
- 2026-10-27 supervisor: both pre-registered gates killed; root cause reads as forward LAr-edge/Plug simulation response at high Q² (generator-independent); user asked to choose close-out vs detector-response campaign
- 2026-10-27 decision: user — after G1′ KILL, asked "Do you think we can get around this with reweighting?"; supervisor ran a pre-registered factorisation scoping (WP-A3) instead of close-out
- 2026-10-27 researcher (WP-A3 response-reweighting scoping): veto-state map does not factorise (rate closes for nd4cat ≤ 0.019; Plug shape ≥ 0.258, FMD/FTS 0.62, η04 edge p ≤ 6e-8; map inherits ND generator forward flow, 14–49 % chain dependence on signal efficiency; no well-simulated η_max plateau to retreat to) → close-out recommended, h1-analysis-critic signed, 0.00637 Condor-days, commit 4adaa4b; deferred: 4 user: scope items (η_max<2.6, drop FMD/FTS, Plug-rate map, new production)
- 2026-10-27 decision: user — not closing yet; asked what fixing the forward response and running new simulation would take, and whether it is reconstruction, simulation or both → WP-A4 assessment (researcher + h1-mc-expert + h1-mc-db) dispatched; output sidis_highQ2/diffractive/docs/restart_2026-10-27/wp-a4/forward_response_fix_assessment.md
- 2026-10-27 WP-A4 (researcher): forward-response attribution: LAr edge S-dominant, 2.5-3.3 rise P present, forward-active FMD/FTS and Plug occupancy S/R, deep-gap undetermined; no reconstruction-only fix (DSTs lack raw forward banks; 7071/7091 DSTs on unmounted /acs); re-sim feasible on h1simrec9 (3.9 s/ev) but needs a modified detector model (P(pass) ≈ 20-30 % with an H1 simulation expert, 6-12+ months; ND+PDISS ≈ 3,900 core-days); 0.0070 Condor-days; signed conditional by h1-analysis-critic; commit 676bb18; deferred: truth-η_max branch definition (h1-mc-expert pending), Plug layer test (needs DCEL reader)
- 2026-10-27 supervisor: WP-A4 decision table presented; recommendation close-out; awaiting user
- 2026-10-28 gate_evidence (supervisor-dispatched figures; no gate re-evaluated, no stored gate output changed): fig1-fig7 under `D/plots/restart_2026-10-27/gate_evidence/` with `captions.md` and `provenance.md`; `D/data/restart_2026-10-27/gate_evidence/{hists_ge.json,summary.json,veto_eff_hists.json,veto_eff.json}`; `D/docs/restart_2026-10-27/gate_evidence/veto_efficiency_view.md`; an explainer artifact (claude.ai page) exists for the user; deferred: none
- 2026-10-28 fact (h1-mc-expert): HAT `EtaMaxGen` = lab η of the X-system particle next to the largest γ*p-rapidity gap (H1HatDiffractiveFiller FillGen), not the most forward generated particle; `hatEtaMaxGen` and `etaMaxGen` are the same variable; an unbiased "most forward generated particle" needs recomputation from H1PartMC status-0 particles with a 0.4-0.8 GeV threshold (WP-A7 item 1)
- 2026-10-28 decision: user reframing — "I think we don't care about plug energy, just that it's an efficient veto": the gate criterion is the data−MC signal-event veto fail probability and its kinematic dependence, not the Plug energy shape (D12); source: user
- 2026-10-28 decision: re-gating accepted as proposed — G1″ replaces G1′: (a) veto signal-loss correction from the deep-gap N−1 view with systematic ≤ 5 % per measurement bin; (c) η_max fit confined to < 3.0 with ND-generator spread as systematic, G2 ≤ 0.30 kept; MC-stat gate and ≥ 5× signal-MC request unchanged; the Plug/FMD/FTS leg of the G1′ KILL withdrawn, the LAr edge a fit-range/systematic item (D13); source: user
- 2026-10-28 decision: Plug (and FTS/FMD) excess handling = correction + systematic, per (M_X bin, D cell), systematic max(half the correction, odd/even-split spread); not a threshold scan, not dropping the veto (D14); source: user
- 2026-10-28 decision: G2's former GOF-dependent branch (f_ND ≤ 0.10, ±100 % envelope) retired; generator spread replaces the envelope (Open Q7, user may override); source: lead
- 2026-10-28 supervisor: parent selection-edge wording aligned to η_max(0.8) < 3.3 in D4 and the WP-A6 block; R7 and the WP-A5 lines kept at 3.2 (existing WP-C track skims), noted for WP-B (WP-A6 child plan Decision 6 / 13); WP-A5 child plan at .supervisor/working/plans/2026-10-28-wp-a5-veto-signal-loss-correction.md and WP-A6 child plan at .supervisor/working/plans/2026-10-28-wp-a6-leakage-refit.md written (critic PASS round 1 each); executors dispatched for both; open to user: WP-A5 Q-A (σ_stat in σ_q), Q-B (η-stepped nominal), WP-A6 Q1 (δ_edge in G2 uncertainty), parent Q6 (U1 grid, ≥5× signal MC), Q7 (G2 GOF branch retired)
- 2026-10-28 WP-A5 (planner): child plan written and reviewed (critic PASS round 1, scope-critic TRIM applied); deferred: user answers to Open Q-A (σ_stat in σ_q) and Q-B (η-stepped nominal if the η term alone exceeds 0.05), non-blocking
- 2026-10-28 WP-A6 (planner): child plan written at .supervisor/working/plans/2026-10-28-wp-a6-leakage-refit.md, critic PASS round 1, scope-critic TRIM applied; deferred: σ_r^D(3) sanity figure to WP-G (no table or yield script in the workspace); 7091/nd4cat track builds (per-M_X spread used instead); open: Q1 edge-band term in the G2 uncertainty (user), parent 3.2/3.3 selection-edge wording (supervisor)
- 2026-10-28 executor (cjc_containment): done; the current hemisphere is CJC-contained in 94.7 % of LRG data and the losses are backward; the forward problems are unchanged in the subset (Plug Δ +0.137, purity 0.83, 37 % at η_max 3.0–3.3); deferred: nd4cat chain and neutral energy (no cluster list in the trees)
- 2026-10-28 WP-A5: G1″(a) PASS in both frames (A 37/37, B 34/34 D7-retained bins ≤0.05; max 0.049/0.035; masked_veto bins all already D7-masked), q frozen even sha c026604e, 5.8e-4 Condor-days; deferred: fragile η-cancellation bins (6A/3B) to WP-A6 FMD δ chain check, COMB over-correction to WP-E/F/H, Q-A/Q-B user answers; x_IP-cut variant of q (shifts up to −0.11/+0.29 in D2 high-M_X) to WP-J systematics
- 2026-10-28 WP-A6: G2 PASS with masks (odd split; M_X [4,10) retained, nd4cat f_ND 0.048 ± 0.040, δ_edge 0.013; [10,20) 0.372 and [20,40] 0.332 masked_G2; no flip), purity table and D7+ ∪ G2 mask written, 0.033 Condor-days; deferred: [4,10) robustness to the no-BB and lower-edge-1.0 variants and the [10,20) split dependence, to WP-F; 3.2/3.3 edge alignment to WP-B W2
- 2026-10-28 supervisor: wave 1c closed; G2 masks M_X > 10 GeV at the 3.3 selection edge because 75–90 % of f_ND sits in the unfitted η_max 3.0–3.3 band (in-fit parts 0.03–0.09); the selection-edge decision (keep 3.3 / move to 3.0 / move to 2.5) is put to the user before WP-B; WP-A5 outputs committed badb695, cjc_containment 2ab0579
- 2026-10-28 decision: user chose η_max(0.8) < 3.0 as the LRG selection edge (D15; AskUserQuestion, options 3.0 / 2.5 / keep 3.3); WP-A5/WP-A6 re-opened for wave 1d re-ruling at 3.0; WP-A6 outputs committed aac91a6
- 2026-10-28 decision: open to user, unanswered after two rounds: Q6 (U1 grid rule, recommendation (c); ≥5× signal-MC request), Q7 (G2 GOF branch retired, recommended), WP-A5 Q-A (σ_stat in σ_q, recommended yes), Q-B (moot), WP-A6 Q1 (δ_edge beside G2, recommended)

## Todo

- [x] WP-A child plan written and approved
- [x] WP-A steps 1-5 run; `g1_verdict.md` written
- [ ] WP-B selection record signed
- [ ] WP-C binning record, resolutions, blinded mask with sha256
- [x] WP-D gap list signed; Q3 answered (2026-10-27, Log)
- [x] WP-I research: pair usability ruling, timing shard (2026-10-27, Log)
- [x] WP-L wave-1 slice: handoff status line and R18 trap correction (2026-10-27, Log)
- [x] WP-A G1 verdict recorded (KILL 2026-10-27)
- [x] WP-A2 child plan approved; S1-S5 run; `g1prime_verdict.md` written (KILL 2026-10-27)
- [ ] ~~G1′ pass recorded → unblock WP-E~~ superseded 2026-10-28 by G1″ (D13)
- [ ] WP-A5 child plan approved; `veto_correction.json` frozen from the even split; per-bin systematic table; `g1pp_a_verdict.md`
- [ ] WP-A6 child plan approved; restricted-range fits for three chains; `f_nd_per_mx.csv`; purity table; `g2_verdict.md`
- [ ] Q6 (U1 grid/mask rule, ≥ 5× signal-MC request) and Q7 (G2 GOF branch) answered by the user
- [ ] Q4 user return after wave 1c: campaign authorised → unblock WP-E
- [ ] WP-A7 items run or formally dropped in WP-L
- [ ] WP-E trees validated
- [ ] WP-F `g2_verdict.md`
- [ ] WP-G control plots reviewed; `g3_verdict.md`
- [ ] WP-H `g4_verdict.md`; unfolded spectra with covariance
- [ ] WP-I C_rad table or bound signed
- [ ] WP-J budget; `g5_verdict.md`
- [ ] WP-K mask ratified by user; results package; figures accepted
- [ ] WP-L note section, handoff, curator close-out
