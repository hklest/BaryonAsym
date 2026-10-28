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
Amended 2026-10-28 (evening): G1″ passed at the D15 edge (wave 1d); the measurement grid is the user's (D16-D22) and the
signal MC moves to 8402-8405 (D23). Added: WP-D2 (840x adoption, ≤ 1 Condor-day plus the tally of the running
production) and WP-A56′ (re-derivation of the veto correction and the leakage fit on the new mixture, ≈ 0.1 Condor-days);
WP-C is re-scoped to the user grid; WP-E grows by the 840x re-production in the writer fork (cost from the WP-D2 tally).

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

Amended 2026-10-28 (evening): the observable grid is hemisphere × β × {z*, p_T*²}, Q² integrated (user; D16-D22). The
weakest link after wave 1d is the leakage estimate at M_X > 10 GeV: chain-dependent
(`D/docs/restart_2026-10-27/wp-a6/g2_verdict_edge30.md:52`) and MC-starved (9012 raw ≈ 0.5k / 1.2k / 0.9k reco events per
M_X bin, `mc_candidates_assessment_2026-10-28.md:40`); the remedy is statistics (840x, ≈ 25× the 9012 templates) and
fit units that remove the per-cell normalisation freedom (Q²-integrated, per β bin), not a new gate. Order: adopt 840x
(WP-D2) → re-derive q and f_ND on the new mixture (WP-A56′) in parallel with the user-grid record (WP-C C1) and the
selection record (WP-B) → Q4 return → WP-E onward. Rejected: deriving the user-grid mask on the 9012 cocktail (the
raw-9012 floor is 57-82 per bin, `user_grid_stats.md:19-20`, and the mask would be re-derived anyway); keeping 9012 in the
nominal mixture (adds 4 %, different SW and generator version); an M_X-binned second unfolding (Q5 (c)).

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
| R29 | Q5 answer: bin in M_X, quote ⟨β⟩, no β axis (Log 2026-10-27); x_F as a measurement axis (D6) | superseded (user 2026-10-28) | the user's grid has two β bins and the x_F sign as hemisphere only; M_X bins stay derivation strata | D16, D20 |
| R30 | Signal MC = 9012 (handoff "Mixture nominal"; WP-D gap list) | change (lead, conditional) | 8402-8405: ≈ 25× statistics, all four periods, DST on disk, generator-level gate passed; adoption gates in WP-D2 | D23, WP-D2 |
| R31 | D7 purity floor 0.35 under unfolding (D7) | keep | the floor is what IBU needs; the user-grid failures move with grid and frame choice, not with the rule | D19, D22 |
| R32 | Frame not frozen, B provisional nominal (`binning_record.md:59,375`, C1) | change (lead) | A nominal, B cross-check: A has 0 D7 failures at q4 on the user grid and carries the t-recoil B ignores by construction (`binning_record.md:53-56`) | D21, WP-C C2 confirms |

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
- D16 **User grid (user, 2026-10-28).** The measurement grid is hemisphere (x_F > 0 Pomeron side P, x_F < 0 photon side G)
  × β (2 bins) × {z* (2 bins) | p_T*² (≤ 5 bins)}, per charge, Q² integrated over Q² > 150 GeV²; the x_F and M_X axes of D6
  are replaced (R29). Primary observables: (1/N_D) dn^±/dz* and (1/N_D) dn^±/dp_T*² per (hemisphere, β) cell; charge sum and
  h⁺−h⁻ derived. Evidence base `D/docs/restart_2026-10-27/wp-c/user_grid_2026-10-28/user_grid_stats.md` (UG): 15,296 data
  events, 104,459 tracks at the D15 selection (UG:6). Everything else in D6 (frame axis, lab fiducial, eSigma γ*, masses,
  per-event normalisation, x_F sign) stands.
- D17 **Q² integrated; D cells are strata (user "we can integrate over Q²").** D cells and M_X bins remain derivation and
  systematic strata only: q per (M_X bin, D cell) (D14), the η_max fit per β bin with cells combined (D24), per-cell and
  per-M_X fits as cross-checks; no measurement bin is Q²- or M_X-differential.
- D18 **z = z* = 2E*/M_X** in the D21 frame (UG:8). Lab z = P·p_h/P·q is ≈ 0 on the Pomeron side (median 0.013 vs 0.077 on
  the photon side, UG:14; a 0.2 split holds 0-4 tracks, UG:25) and cannot carry two bins. Split at z* = 0.2 (fixed,
  leading-hadron edge; 0 D7 failures, raw-9012 min 57, UG:19,26); the per-hemisphere median split (0.094 G / 0.083 P) is the
  alternative presentation (fails P high-β low-z*, purity 0.33 in both charges, UG:18,26). Pending user confirmation (Q9).
- D19 **p_T*² binning rule.** Data-quantile edges, equal population over all tracks in both hemispheres (q5
  0/0.042/0.103/0.222/0.547, q4 0.055/0.152/0.424, q3 0.079/0.291 GeV², last bin open, UG:14); fixed grids rejected (f5 fails
  10/40 per-charge bins, raw-9012 min 11, UG:21,26). Per (hemisphere, β) cell the grid is the finest of {q5, q4, q3} whose
  every bin passes D7 for both charges on the frozen cocktail, the same grid for h⁺ and h⁻; start value q4 (frame A: 0
  failures; frame B: 5), q5 where it passes (it fails only the G high-β bins 1-4, purity 0.23-0.33, UG:26). The failures are
  migration purity, not MC statistics (UG:26-27): more MC does not move them, grid and frame do. — amended 2026-10-28 (Q-C1-3): the per-cell pass criterion is the C1-7 rule (D7+ plus MC-stat ≥ 500/≥ 20, inter-cell migration < 0.5, bin width ≥ 1.5(σ_core+|bias|)), see the WP-C C1 child plan.
- D20 **β bins.** Two, split at the data event median 0.523 (low β ≈ M_X 13-32 GeV, high β ≈ 5-18 GeV at 5/95 %, UG:14); reco
  β = DiffTot `beta`, gen β from true Q² and `mirMX` (UG:10). The three M_X bins [4,10), [10,20), [20,40] stay the
  pre-registered units in which G1″(a) and G2 were ruled and are carried as a derivation stratum and continuity cross-check
  (q, f_ND per M_X); no M_X-binned unfolding.
- D21 **Frame: A nominal, B cross-check (lead; supersedes C1 "no freeze").** Reco p_X = stored HFS 4-vector (frame A,
  `binning_record.md:44-62`); gen p_X = the WP-E truth X 4-vector, its exact counterpart (the proxy q + x_IP·P until then).
  Grounds: on the user grid A has 0 D7 failures at q4 and 5 at q5 against B's 5 and 8 (UG:26); B's p_T*² ignores the HFS
  transverse imbalance and the t-recoil by construction, which the critic ruled not negligible at large |x_F|
  (`binning_record.md:53-56`), and B carries a +0.10…+0.41 GeV² bias at M_X 4-10 (`:149-165`), the high-β cells where the
  failures sit; A's 1.5-2.6× worse p_T*² core resolution at M_X > 10 (`:80-147`) is absorbed by the quantile grid and the
  unfolding. B is the frame systematic. Confirmed at the WP-C mask freeze with the true gen X; if B then has fewer failures
  on the same grid, WP-C reports instead of flipping. Consequence: WP-E (gen X/Y branches) stays required and includes 840x.
- D22 **Unfolding on the user grid (D8 adapted).** IBU per charge with a 2D response (cell × observable bin → cell × reco
  bin), reco split 2× along the observable axis only (Q6 option (c)), cells = hemisphere × β so that migration across
  hemispheres and β bins is inside the response, plus the event-level β response for N_D: 8 → 16 (z*) and ≤ 20 → 40 (p_T*²)
  gen → reco bins per charge. D7 (weighted n ≥ 50, purity ≥ 0.35, stability ≥ 0.35) stays the pre-registered mask floor for
  unfolded bins; unfolding is what makes a 0.35-purity bin measurable, it does not relax the floor; bin-by-bin is used only
  for the G3 gate. The D7+ extras (`binning_record.md:222-244`) apply as written. The ≥ 5× signal-MC request (Q6) is met by
  840x (D23). — amended 2026-10-28 (Q-C1-1): reconstructed β is split 2× finer than the gen β bins when C2/WP-H build the response matrices (h1-unfolder consult: +0.054 β bias).
- D23 **Signal MC = 8402/8403/8404/8405 (lead, conditional; user may override, Q13).** RAPGAP 3.1043 elastic uds, gens
  5643 (e⁻)/5644 (e⁺), 10 M each, L_eq 330-331 pb⁻¹ per period, SW77 with tilt and run-dependent z-vtx, DST on disk
  (`D/docs/restart_2026-10-27/wp-d/mc_candidates_assessment_2026-10-28.md:40-49`, cited as A); generator-level gate passed
  (ratio to 9012 flat in β, x_IP, Q², |t|; +6-8 % global, A:45-47). Rules: (i) one floating normalisation per generator stream,
  no (β, x_IP) reweight (A:47); (ii) per-period weights: each data period takes its own 840x period at L_data/L_eq times the
  stream factor; (iii) 8402≡8405 and 8403≡8404 are the same generated events (A:49), two independent truth streams, so
  MC-stat bootstraps are by generated event; (iv) 9012 leaves the nominal mixture (adds 4 %, other SW and version) and is the
  SW84 anchor and shape cross-check; nominal mixture = 840x + charm 9024/9033 + PDISS 9155/9157/9158 + Reggeon 6671/6677,
  PDISS ratio per R11; (v) adoption gates open in WP-D2: the 8405 ODS smoke record (run, undocumented), the production tally
  (odsLoaded 100 %, DiffTot = evls, rc=134 retries), the SW77→SW84 transfer check — first 840x vs 9012 on the shared 06/07
  period at detector level, the djangoh14 gen 5624 same-event pair (A:52) only if that shows a difference beyond σ_q,ALL or
  the chain spread. Roles of the user-named samples: 9022/9023 (gen 4490, charm BGF only, 71.6 pb⁻¹ each, correlated twins) = x_IP-shape
  cross-check of 9024/9033 only (A:7-10); 8529/8530/8531 (gens 5717-5719, p̂_T hard subsets with `weight1`, SW80 04/05 e⁻) =
  high-p_T-tail shape cross-checks, never in the σ-normalised mixture (A:14-19); gen 5719 = ReqID 8531, the Reggeon member of
  that family (A:17; `mc_gap_list.md:72`): not an elastic signal candidate; its
  one use is a detector-level (DST, SW80) Reggeon shape cross-check of the HAT/MODS-only 6671/6677 in the hard tail, after the
  `h1-mc-db` steering confirmation (the 5719 log was read for σ only, A:17; PTCU, subset and periods unconfirmed). 6670/6676/8221/7448 have no DST and
  are not signal (8221 vs 6676, IFPS only, remains a candidate WP-J hadronisation leg).
- D24 **WP-A5′/WP-A6′ re-derivation on the new mixture (lead).** Both corrections depend on 9012 through the DIFF8 template,
  f_MC,full in q and the propagation sample (`D/docs/restart_2026-10-27/wp-a5/g1pp_a_verdict.md:41-43`), so WP-A56′ re-derives
  them under the same freeze protocol (even derives, odd rules). Order: (1) 840x skims; (2) first-pass η_max fit with the new
  DIFF8′ for the stream scales; (3) q per (M_X bin, D cell) from the deep-gap view (D14); (4) the η_max(0.4) < 3.0 two-template
  fit per β bin, Q² integrated, D cells combined with the MC relative cell populations (the parent's inclusive data/MC Q²
  agreement is the justification), per-cell and per-M_X fits as cross-checks; (5) f_ND per β bin, G2 ceiling 0.30 on the
  largest chain, σ_G2 = fit ⊕ chain spread, δ_edge beside; one scale iteration between (3) and (4). 6670 is not used for
  templates: 840x already gives ≈ 25× the 9012 template statistics and 6670 would add ≈ 20 % at the price of a HAT-vs-tree
  η_max parity validation. **Leakage target:** σ_G2/(1 − f_ND) ≤ 0.10 per β bin (present M_X values 0.21 / 0.13 for [10,20) /
  [20,40], from `g2_verdict_edge30.md:15-35`), equivalently a propagated leakage term ≤ 10 % per measurement bin: the figure
  G2's own justification assumed, and under half the G5 median ceiling once the parent-style detector legs (7-8 % median) and
  the veto leg (≤ 5 %) are added. Expected gains: the fit σ (0.044-0.080) shrinks with ≈ 25× templates and ≈ 3× data per unit;
  the chain spread (0.151 at [10,20)) shrinks when the per-cell normalisation freedom goes (`h1-mc-expert`), but it is a model
  term and may stay above the target. If a β bin misses it, WP-A56′ reports beside it the pre-registered variant selection
  η_max(0.4) < 3.0 (the fit variable; removes the residual band; costs 10-35 % of events) and the user chooses variant, mask or
  acceptance under G5 (Q11); no lead switches it.
- D25 **Housekeeping defaults (lead, user let stand 2026-10-28).** σ_q = max(|q|/2, odd/even spread, σ_stat) (closes WP-A5
  Q-A; Q-B is moot at 3.0, `g1pp_a_verdict_edge30.md:33`); G2's GOF branch retired (closes Q7); δ_edge reported beside the G2
  uncertainty, not folded in (closes WP-A6 Q1), with the condition-5 band-ρ values per bin
  (`D/docs/restart_2026-10-27/wp-a6/g2_edge30_conditions_2_5.md:21-28`). Binding `h1-analysis-critic` conditions at the 3.0
  edge, relayed by the supervisor (only conditions 2 and 5 are on disk; WP-A56′ step 0 writes the list to
  `D/docs/restart_2026-10-27/wp-a6/g2_edge30_critic_conditions.md`): the leakage uncertainty is carried to G5 and shown net
  favourable against the η_max-cut efficiency systematic (η_max correlates with x_F, so the cut efficiency does not cancel);
  purity budgets use largest-chain f_ND; the `nobb` stability flag ([20,40] 7091 0.353) is carried; the frozen "[10,20) stays
  masked unless WP-F unmasks it" (`D/docs/restart_2026-10-27/wp-a6/g2_verdict.md:222`, `g2_verdict_edge30.md:51`) is
  discharged by WP-A56′ on the new mixture, not by D15.

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
  Amended 2026-10-28 (evening, D16-D24): G1″(a)'s ≤ 0.05 per measurement bin and G2's ≤ 0.30 ceiling are re-ruled by WP-A56′ on
  the 840x mixture, G1″(a) on the user grid (D16) and G2 per β bin (D20), the M_X bins as continuity cross-check; D24's leakage
  target σ_G2/(1−f_ND) ≤ 0.10 per β bin is a target with a user return (Q11), not a kill. Wave-1d state at the 3.0 edge
  (mechanical re-rules; critic ruling relayed): G1″(a) PASS, max σ_rel 0.038 A / 0.025 B, 37 / 34 retained bins, 4 / 2 fragile
  cancellation bins carried in joint and quadrature form (`g1pp_a_verdict_edge30.md:6,26-32,41`; D7 masks not re-evaluated,
  `:50-51`); G2 largest-chain f_ND 0.040 / 0.243 / 0.221, σ_G2 0.032 / 0.157 / 0.099, no mask, with the D25 conditions
  (`g2_verdict_edge30.md:15-35`).
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
  f_reg, f_fake, purity, stability, masked` shared by WP-C, WP-F, WP-H, WP-J. Amended 2026-10-28 (evening): the key becomes
  `charge, hemisphere, beta_bin, axis (zstar|pt2s), bin`, with `mx_bin` rows kept for the D20 cross-check strata.
- 840x (D23): stage-2 chain `D/condor/v6dif_mc840x_stage2/2026-10-28/mc840{2,3,4,5}/` (prep scripts
  `prepare_mc840x_*.py`, `prod_merge_job.sh`), output `D/output_v6dif/mc/840{2,3,4,5}/2026-10-28/`, smoke
  `D/output_v6dif/mc/8405/smoke_2026-10-28/`; tally and merge pattern =
  `D/docs/restart_2026-10-27/wp-d/mc_candidates/production_2026-10-28.md` (per-chunk gates `:3`, rc=134 retry policy `:4`),
  merged target `D/data/t2_detector/mc840x_merged_2026-10-28/<R>.root` with a README and tally CSV as in
  `D/data/t2_detector/mcnew_merged_2026-10-28/`.
- Skims (WP-D2 delivers both for 840x): event-level `D/scripts/restart_2026-10-27/wp-a3/skim_wpa3.py` →
  `D/data/restart_2026-10-27/wp-a3/skims/<label>.npz` (17 labels, `g1pp_a_verdict.md:54-76`); track-level study npz
  `D/scripts/restart_2026-10-27/wp-c/study/wpc_build.py` with labels in `wpc_lib.L.LABELS`. Gotcha: `ug_tables.py` takes its
  signal list from `propagate_wpa5.sample()` (`D/scripts/restart_2026-10-27/wp-a5/propagate_wpa5.py:38`), not from an
  argument; the 840x labels and per-period weights go into `wpc_lib` and the wp-a5 cells, and 9012 is dropped from the list.
- WP-A56′ reuses `D/scripts/restart_2026-10-27/wp-a5/{veto_corr_hist.py,derive_veto_corr.py,propagate_wpa5.py,veto_weight.py}`
  and `D/scripts/restart_2026-10-27/wp-a6/{build_payload_wpa6.py,fit_bblite_wpa6.py,fnd_wpa6.py,purity_fill_wpa6.py}` plus
  `edge30/`; frozen range `D/data/restart_2026-10-27/wp-a6/range_frozen.json`; payload pattern
  `D/data/restart_2026-10-27/wp-a6/edge30/payloads/<split>_<chain>_sel30.json`. Invariant: the even-split freeze and sha
  record precede any odd-split read, as in `g1pp_a_verdict.md:40-41`.
- Truth η_max recomputation (WP-A7 item 1) exists in
  `D/scripts/restart_2026-10-27/wp-d/mc_candidates/pkg_xscan/src/McCandidateXScan.C:8-10,57-90` (status-0 `H1PartMC`,
  largest-gap X/Y split in the γ*p cms, M_X, M_Y, η_max of X); WP-E ports it into the v6dif2 writer fork as the branch.

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

Amended 2026-10-28 (evening); waves 2-6 above are superseded by this sequence:
1d. Wave 1d (D15): WP-A5 and WP-A6 re-ruled at 3.0 — done (`g1pp_a_verdict_edge30.md`, `g2_verdict_edge30.md`).
1e. Wave 1e (now): WP-D2. Checkpoint: `D/data/t2_detector/mc840x_merged_2026-10-28/<R>.root` validated,
   `D/docs/restart_2026-10-27/wp-d/mc_candidates/production_840x_2026-10-28.md`, `D/docs/restart_2026-10-27/wp-d2/adoption_record.md`,
   840x labels in both skim sets.
1f. Wave 1f (after WP-D2): WP-A56′ and WP-C C1 (user-grid record on the 840x cocktail, proxy gen; the edge-30 f_ND as
   stand-in, re-filled from WP-A56′) in parallel, then WP-B (decision) on both.
   Checkpoint: `D/docs/restart_2026-10-27/wp-a56/g1pp_a_verdict_v2.md`, `g2_verdict_v2.md` (per β bin),
   `D/docs/restart_2026-10-27/wp-c/user_grid_record.md`, `D/docs/restart_2026-10-27/wp-b/selection_record.md`; then the Q4 return
   (Q10) with the WP-D2 cost tally.
2′. Wave 2 (on Q4): WP-E (v6dif2 for data, 840x and the other MC), WP-I execute (gen level; needs only C1).
   Checkpoint: validator record; `crad_record.md`.
3′. Wave 3: WP-F (G2 re-confirmed per β bin on v6dif2), WP-C C2 (frame confirmation, mask freeze with sha256), then WP-G (G3).
4′-6′. WP-H (G4); WP-J (G5); WP-K and WP-L, as before.

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
- 840x is SW77 where the data-period simulation elsewhere is SW84 and the forward response is the known weak point: if the
  WP-D2 transfer check fails, a transfer leg is needed or the 9012 anchor limits what 840x can carry.
- Two truth streams in four periods (8402≡8405, 8403≡8404): MC-stat correlation across periods, handled by bootstraps by
  generated event (D23 iii); the +7 % unattributed normalisation (DPDF/flux, A:51) is bounded by the flat ratio in β and
  x_IP (A:45) and re-checked on the final fit.
- The D24 leakage target may be out of reach in the low-β bin (M_X 13-32 GeV) if the chain spread is a model term: user return
  Q11, with the η_max(0.4) < 3.0 variant costed beside it.
- The user grid's p_T*² failures sit in the G high-β cell in both frames; if the true-gen-X purity (WP-C C2) does not recover
  q4 there, that cell is measured with q3 or z* only (D19), not with a relaxed D7.

## Scope Notes

In: everything in `## Work Packages`. Out: FPS/VFPS; parent release changes; generator retuning;
editing `.supervisor/memory/` (curator only). User decisions `executor` must not reopen: the pivot to
X-frame x_F/p_T*² in M_X/β bins; narrow Plug probe first; re-base on parent conventions; restart
authority for leads (user 2026-10-27, recorded in the supervisor brief); parent-side memory entries
R21/R22; the 2026-10-28 reframing (veto efficiency, not Plug energy shape), the G1″ re-gating, correction +
systematic for the forward-veto excess, and no reuse of the remedy-1 η_max < 1.5 control region (D12-D14); the D15 edge
3.0; the user grid of D16 (two z bins, ≤ 5 p_T bins, hemispheres, two β bins, Q² integrated) and the five named samples'
consideration (D23). Lead decisions the user may override are marked in D18, D21, D23, D24 (Q9, Q11, Q13). Workspace rules per `CLAUDE.md`; `.sub` files under `D/condor/restart_2026-10-27/`.

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
- Status: done
- Result: G1″(a) PASS at 3.2 (2026-10-28, `g1pp_a_verdict.md`) and at the D15 edge 3.0 (mechanical re-rule, max σ_rel 0.038 A /
  0.025 B, `g1pp_a_verdict_edge30.md:6,29`); q frozen even sha c026604e; superseded for the measurement by WP-A56′ (D24)
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
- Status: done
- Result: G2 PASS with masks at 3.3 (`g2_verdict.md`); at the D15 edge 3.0 largest-chain f_ND 0.040 / 0.243 / 0.221, no mask,
  critic conditions carried (`g2_verdict_edge30.md:15-35`, `g2_edge30_conditions_2_5.md`); superseded for the measurement by
  WP-A56′ (D24)
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
- Deps: item (1) WP-E (writer fork); item (2) WP-A56′ (correction JSON)
- Stages: execute
- Status: blocked: execute: parked — opened only by user request or by WP-E for item (1)
- Lead: `executor`; `h1-mc-expert` (truth banks), `h1-systematics` (item 2 as an optional leg)
- Condor: yes if opened (item 1 rides the WP-E production; item 2 ≈ 0.05 Condor-days)
- Scope Notes: (1) unbiased truth η_max: recompute from H1PartMC status-0 particles (0.4 and 0.8 GeV thresholds) as
  a WP-E writer-fork branch; definition of the biased HAT `EtaMaxGen` per the `h1-mc-expert` fact (`## Log`
  2026-10-28). Updated 2026-10-28 (evening): the recomputation exists in
  `D/scripts/restart_2026-10-27/wp-d/mc_candidates/pkg_xscan/src/McCandidateXScan.C:8-10,57-90`; WP-E ports it, item (1)
  reduces to the `etamax_gen_fwd_vs_hat` test on the v6dif2 trees. (2) Plug threshold scan 7/10/15 GeV as an
  optional systematic leg on the WP-A5 correction; the user chose re-gating without it. Not parked here but
  recorded for continuity (both owned by WP-D2): (3) the gen 4489 bookkeeping ("Events 1,645,032" vs 1,200,002 records, A:48),
  WP-D2 step (6), before any absolute 9012 normalisation or G3 comparison; (4) the rc=134 stage-2 exit abort
  (`production_2026-10-28.md:4`), counted in the WP-D2 tally, investigated only if the rerun-once policy fails a chunk or the
  count exceeds the 9/40 of the first production. Tests:
  `etamax_gen_fwd_vs_hat: on 9012 the new branch ≥ hatEtaMaxGen event by event -> fraction with new > hat
  reported`; `threshold_scan_table: q per threshold per (M_X, cell) -> written`. Checkpoint:
  `D/docs/restart_2026-10-27/wp-a7/followups.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-a7/followups.md`
- Output: `D/docs/restart_2026-10-27/wp-a7/`, `D/data/restart_2026-10-27/wp-a7/`
- Log: —

### WP-A56′: WP-A5′ + WP-A6′ — veto correction and leakage fit re-derived on the 840x mixture, per β bin
- Goal: re-derive q (per M_X bin × D cell) and f_ND (per β bin, Q² integrated, cells combined) on the D23 mixture with the
  WP-A5/WP-A6 machinery and freeze protocol, re-rule G1″(a) on the user grid and G2 per β bin, and report the D24 leakage
  target (with the η_max(0.4) < 3.0 variant beside it only where a β bin misses it); done when `D/docs/restart_2026-10-27/wp-a56/g2_verdict_v2.md` and
  `g1pp_a_verdict_v2.md` exist with `h1-analysis-critic` verdicts
- Owns: `D/{docs,data,scripts,condor,plots}/restart_2026-10-27/wp-a56/`
- Deps: WP-D2 (840x merged trees, per-period weights, both skim sets, adoption record)
- Stages: plan, execute
- Status: execute
- Lead: `executor` (child plan by `planner` at `.supervisor/working/plans/2026-10-28-wp-a56-rederivation.md`);
  `h1-mc-expert` (chain-spread reading if the [10,20)-type > 3σ_fit trigger recurs, `g2_verdict.md:135`), `h1-systematics`
  (σ_q and leakage-leg review), `h1-analysis-critic` (both verdicts)
- Condor: yes (skim forks and fits; ≈ 0.1 Condor-days)
- Scope Notes: step 0 writes the relayed critic conditions 1-8 to
  `D/docs/restart_2026-10-27/wp-a6/g2_edge30_critic_conditions.md` (D25). Steps per D24 (1)-(5); units: q strata (M_X bin,
  D cell) as D14; fit units β bin × {7071, 7091, nd4cat} × split, cells combined with the MC relative cell populations,
  per-cell and per-M_X units as cross-checks (continuity with `g2_verdict_edge30.md`); the scale fallback for units below
  100 FIT-sample data events or 100 raw ND events (`fit_record.md:6`) re-checked per β unit; 9012 excluded from DIFF8′ and fitted once as the anchor cross-check
  (its f_ND against DIFF8′'s within σ_G2). Propagation of q ± σ_q on the user grid (D16, frame A, q4 start grid, z* 0.2) gives the
  G1″(a) table per (charge, hemisphere, β, axis, bin), joint and quadrature forms for the cancellation bins
  (`g1pp_a_verdict.md:192,338`). Leakage target per β bin; only where a β bin misses it, the variant selection η_max(0.4) < 3.0 is
  run on the same skims and its event cost, f_ND and σ_G2 reported side by side (D24, Q11). Purity per measurement bin with largest-chain f_ND
  (D25) feeds WP-C C1. Not allowed: the remedy-1 control region; any Plug energy-shape criterion; relaxing D7. Tests:
  `even_frozen_first: q and scale JSON sha256 recorded before any odd-split read -> true`; `mixture_identity: DIFF8′ labels =
  {8402,8403,8404,8405,9024,9033,9155,9157,9158,6671,6677}, 9012 absent -> true`; `per_period_weights: Σ_R w_R L_eq,R per
  period = L_data,period × stream factor -> within 1e-6`; `g2_table_beta: f_ND, σ_fit, spread, δ_edge, ceiling flag, target
  flag per β bin and chain -> written`; `continuity_mx: per-M_X f_ND on the new mixture vs edge30 within σ_G2 -> reported`;
  `g1pp_a_table_user_grid: σ_rel ≤ 0.05 flag per user-grid bin, both forms -> written`; `variant_costed: if a β bin misses the D24 target -> η_max(0.4) < 3.0
  event fraction, f_ND and σ_G2 for that bin written`. Checkpoint: the two verdicts.
- Artifacts: child plan; `g1pp_a_verdict_v2.md`, `g2_verdict_v2.md`; `D/data/restart_2026-10-27/wp-a56/{veto_correction_v2.json,f_nd_per_beta.csv,purity_user_grid.csv}`
- Output: `D/docs/restart_2026-10-27/wp-a56/`, `D/data/restart_2026-10-27/wp-a56/`, `D/plots/restart_2026-10-27/wp-a56/`
- Log: —

### WP-B: selection and kinematic-range re-decision
- Goal: re-decide every selection and range item from scratch (register R3-R10, R22); done when the
  selection record is signed by `h1-analysis-critic`
- Owns: `D/docs/restart_2026-10-27/wp-b/`
- Deps: WP-A5 (veto corrections and their systematic), WP-A6 (η_max fit range, f_ND per M_X bin), WP-C (x_IP
  purity per M_X bin); the kinematic research proceeds in wave 1. Updated 2026-10-28 (evening): WP-A56′ (f_ND per β bin),
  WP-C C1 (x_IP purity per β bin); R7/W2 settled by D15; R5 edges and R3 x_IP remain
- Stages: research, plan
- Status: blocked: plan: WP-A56′ and WP-C C1 (wave 1f)
- Lead: `planner` (research by `planner-worker`s); `h1-analysis-critic` signs
- Condor: no (reads WP-A and WP-C products)
- Scope Notes: inputs D4 start values; outputs per item: value, alternatives, evidence, systematic
  variation. Includes the Reggeon-fraction check (D4) and the x_IP purity per M_X bin from WP-C.
  Test: `every_register_row_closed: R3-R10, R22 each have keep/change with evidence -> true`.
  Checkpoint: `D/docs/restart_2026-10-27/wp-b/selection_record.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-b/selection_record.md`
- Output: `D/docs/restart_2026-10-27/wp-b/`
- Log: —

### WP-C: observable definition, boost, binning and blinded mask (re-scoped 2026-10-28 evening to the user grid)
- Goal: fix D6 operationally, measure resolutions and propose the grid; done when the binning record
  with per-bin purity/stability and the blinded mask is written. Updated: fix D16-D22 operationally in two slices — C1 the
  user-grid record (observable definitions, bin edges per cell by the D19 rule, purity/stability per bin on the 840x
  cocktail with the proxy gen, provisional mask) and C2 the frame confirmation and mask freeze with sha256 on the v6dif2
  cocktail; done when `user_grid_record.md` (C1) and the frozen mask JSON (C2) exist
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-c/`
- Deps: none for the stat study (interim gen proxy q + x_IP·P); mask freeze: WP-A6 (f_ND per M_X bin enters the
  D7 purity) and the WP-B selection; final gen frame after WP-E. Updated: C1: WP-D2 (840x study npz, weights), WP-A56′ (q
  weights, f_ND and purity per β bin); C2: WP-E (true gen X), WP-B (selection record)
- Stages: research, plan, execute
- Status: execute (C1, with WP-A56′ re-filling f_ND; child plan at `.supervisor/working/plans/2026-10-28-wp-c-user-grid.md`); C2 after WP-E
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
  Checkpoint: `D/docs/restart_2026-10-27/wp-c/binning_record.md`. — the above is the 2026-10-27 scope, delivered
  (`binning_record.md`, user-grid statistics `user_grid_2026-10-28/`); the former block was blocked on user U1
  (binning_record.md §8, Open Q6), answered by the user grid and 840x (D19, D22, D23); superseded by:
  Updated 2026-10-28 (evening). C1 on the 840x cocktail (9012 dropped per D23, `wpc_lib.L.LABELS` and the wp-a5 cells
  extended by WP-D2): observables, frame (A nominal, B in the same tables), β and z* splits and the per-cell p_T*² grid rule
  per D16-D21, the β median re-read on the D15 selection, quantile edges from `user_grid_summary.json`; purity, stability,
  f_fake, n per bin per D7/D7+ with
  largest-chain f_ND from WP-A56′ (until it delivers, C1 is filled with the edge-30 largest-chain per-M_X f_ND,
  `g2_verdict_edge30.md:15-35`, mapped by event M_X, and re-filled when WP-A56′ lands); β and z* resolutions added to the T3 table (no z* entry exists, `binning_record.md:80-147`);
  the response shapes of D22 (8 → 16, ≤ 20 → 40 per charge, event-level β) checked with `engine_shape`. The provisional mask
  is derived blind and hashed but not frozen. C2 after WP-E: recompute purity with the true gen X in frames A and B on the C1
  grid, confirm D21 (report, do not flip), freeze the mask JSON with sha256 before any data fill. Tests (C1):
  `zstar_range: all tracks -> 0 < z* ≤ 1`; `beta_split_reproduced: data median at the D15 selection -> 0.523 ± 0.005 or the new
  value recorded`; `grid_rule_applied: per cell the chosen p_T*² grid is the finest of q5/q4/q3 with no D7 failure in either charge
  -> true`; `nine012_absent: cocktail labels contain no 9012 -> true`; `purity_source: purity uses largest-chain f_ND -> true`.
  Tests (C2): `frame_report: purity failures per frame on the C1 grid, true gen X -> written`; `mask_blind` as above.
  Checkpoints: `D/docs/restart_2026-10-27/wp-c/user_grid_record.md` (C1); `D/data/restart_2026-10-27/wp-c/mask_user_grid.json`
  + sha256 (C2).
- Artifacts: `D/docs/restart_2026-10-27/wp-c/binning_record.md`, mask JSON with sha256; updated: child plan
  `.supervisor/working/plans/2026-10-28-wp-c-user-grid.md`, `user_grid_record.md`, `mask_user_grid.json`
- Notes: C1 record at `D/docs/restart_2026-10-27/wp-c/c1/grid_freeze_record.md`; 5-line stub at `D/docs/restart_2026-10-27/wp-c/user_grid_record.md`.
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

### WP-D2: 840x adoption as signal MC
- Goal: close the D23 adoption gates and deliver 8402-8405 as validated, weighted, skimmed signal MC; done when
  `D/docs/restart_2026-10-27/wp-d2/adoption_record.md` is signed by `h1-analysis-critic` with the per-period weight table
- Owns: `D/docs/restart_2026-10-27/wp-d2/`, `D/data/t2_detector/mc840x_merged_2026-10-28/`,
  `D/docs/restart_2026-10-27/wp-d/mc_candidates/production_840x_2026-10-28.md`, the 840x entries of
  `D/data/restart_2026-10-27/wp-a3/skims/` and `D/data/restart_2026-10-27/wp-c/study/`, `D/condor/restart_2026-10-27/wp-d2/`
- Deps: none (the v6dif stage-2 production under `D/condor/v6dif_mc840x_stage2/2026-10-28/` is running; chunk output in
  `D/output_v6dif/mc/840{2,3,4,5}/2026-10-28/`)
- Stages: execute
- Status: done
- Result: 840x adopted (validation PASS; w_period 0.1324/0.2970/0.1551/0.3748 for 8402/8403/8404/8405; stream factor floated in WP-A56′; SW77→SW84 tier 1 passes at 2σ, tier 2 not run); record docs/restart_2026-10-27/wp-d2/adoption_840x_record.md; h1-analysis-critic signature pending
- Lead: `executor`; `h1-naf-operator` (tally, merge, Condor-day count), `h1-data-validator` (merged trees, branch parity with
  9012), `h1-mc-db` (gen 5624 pair availability; 4489 bookkeeping; 5719 steering), `h1-mc-expert` (SW77→SW84 reading),
  `h1-analysis-critic` (record)
- Condor: yes (tally, merge and skims ≈ 0.1 Condor-days; the gen-5624 pair chunks, ≈ 0.5-1 Condor-day, only if step (3) is
  triggered)
- Scope Notes: (1) tally per chunk with the `production_2026-10-28.md:3` gates, rc=134 count per sample (WP-A7 item 4), the
  Condor-days of the whole 840x production from the logs (input to Q10); the 8405 smoke record written from
  `D/output_v6dif/mc/8405/smoke_2026-10-28/` (odsLoaded, DiffTot = evls, branch list = 9012's); hadd to
  `mc840x_merged_2026-10-28/<R>.root` with README and tally CSV. (2) Per-period weight table: w_R = L_data,period / L_eq,R
  (330.3 / 331.0 pb⁻¹, A:43) × one stream factor (initial 1.0; fitted later by WP-A56′), periods 8402 = 03/04 e⁺, 8403 = 04/05
  e⁻, 8404 = 2006 e⁻, 8405 = 06/07 e⁺ (A:40,49); the z-vtx per period is simulated, no reweight. (3) SW77→SW84 transfer, two tiers: (a) 8405 (06/07, SW77) vs 9012 (06/07, SW84) on the WP-A3 skim
  variables after the stream factor (generator-level forward shapes agree, A:46): Plug/FMD/FTS fail probabilities and the
  η_max(0.4), η_max(0.8) shapes in the LRG-like region (x_IP < 0.04, reco M_X > 3); acceptance |Δf| per detector ≤ the WP-A5
  σ_q of the ALL cell (Plug 0.034, FTS 0.024, FMD 0.043, `g1pp_a_verdict.md:144-146`) and the η_max(0.4) < 3.0 shape
  difference inside the chain spread; (b) only if (a) fails: `h1-mc-db` confirms the djangoh14 gen 5624 same-event pairs
  (8457/8868 SW77 ↔ 8942/8944/9132/9135 SW84, A:52) and DST access, the first N chunks of one pair run through v6dif and are
  matched event by event, and a transfer weight per detector state is derived and carried as a WP-J leg; the record says which
  tier ruled. (4) 9012 leaves the mixture (D23 iv) once (3a) is recorded. (5) Skims: `skim_wpa3.py` and `wpc_build.py` over the four merged files, labels
  `8402..8405` added to `wpc_lib.L.LABELS` and the wp-a5 cells; 9022/9023, 8529-8531 skimmed as cross-check labels only.
  (6) Lookups: gen 5719 steering (PTCU, subset, periods) → confirm D23's role; gen 4489 bookkeeping closed (WP-A7 item 3).
  Not allowed: any reweight in (β, x_IP); adding 9012 to the nominal list; using 8529-8531 or 9022/9023 in the σ-normalised
  mixture. Tests: `chunks_complete: 100/100, 100/100, 101/101, 101/101 chunks with rc=0 and odsLoaded 100 % -> true`;
  `branch_parity_9012: every DiffTot/ChargedTot/ChargedTotGen branch of 9012 present in 8405 -> true`; `period_weights_sum:
  Σ_periods w_R L_eq,R = 317,212.8 nb⁻¹ -> within 1e-6`; `twin_identity: first 8 chunks of 8402 and 8405 match event by event
  on (event, Q², M_X) -> 100 %`; `transfer_acceptance: per-detector |Δf| 8405−9012 (06/07) -> ≤ σ_q,ALL, else tier (b) run and a transfer weight written`;
  `smoke_recorded: 8405 smoke record exists with the three gates -> true`. Checkpoint: `adoption_record.md`.
- Artifacts: `D/docs/restart_2026-10-27/wp-d2/adoption_record.md`; `production_840x_2026-10-28.md`;
  `D/data/restart_2026-10-27/wp-d2/period_weights.json`; merged trees; skims
- Output: `D/docs/restart_2026-10-27/wp-d2/`, `D/data/t2_detector/mc840x_merged_2026-10-28/`, `D/data/restart_2026-10-27/wp-d2/`
- Log: —

### WP-E: re-based production (writer fork, data and MC re-run)
- Goal: trees with gen X/Y 4-vectors and all v6dif branches for 4 data periods and 13 MC samples,
  validated; done when `h1-data-validator` passes entry counts against v6dif. Updated 2026-10-28 (evening): the MC list is
  the D23 mixture plus 9012 (anchor) and the ND chains, 840x included (402 chunks); the fork also carries the truth
  η_max branch ported from `McCandidateXScan.C` (WP-A7 item 1)
- Owns: `P/src/ChargedSidisTreeBuilderHighQ2_v6dif2.C`, `D/output_v6dif2/`,
  `D/condor/restart_2026-10-27/wp-e/`, `D/docs/restart_2026-10-27/wp-e/`
- Deps: WP-A5 and WP-A6 (gate G1″ records `g1pp_a_verdict.md`, `g2_verdict.md`; D1 and the Q4 user return), WP-C
  (branch list), WP-D (sample list). Updated 2026-10-28 (evening): WP-D2 (sample list, weights, transfer outcome), WP-A56′
  and WP-C C1 (the Q4 return, Q10); the branch list is fixed by D5, D21 and WP-A7 item 1, not by WP-C's completion
- Stages: plan, execute
- Status: blocked: plan: Q4 user return (Q10) after wave 1f
- Lead: `executor`; `h1-mc-expert` (truth-bank X/Y definition), `h1-naf-operator` (Condor, merges),
  `h1-data-validator`
- Condor: yes (data 658 jobs ≈ 30 min; MC evls + stage2 chains, est. 1-3 Condor-days; plus the 840x re-production at the
  WP-D2 tally cost)
- Scope Notes: D5; compile with `make bin`; one-file login-node smoke test only. Per-sample weights
  recomputed on the new selection with the v3 luminosities (R15) and the WP-D2 period weights for 840x. Tests: `branch_parity: every v6dif
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
- Deps: WP-E, WP-B, WP-A5 (correction applied to the signal MC before the fit), WP-A6 (fit range and method). Updated
  2026-10-28 (evening): WP-A56′ replaces WP-A5/WP-A6 as the method and start values; G2 re-confirmed per β bin (D20) with the
  M_X cross-check; the D24 target re-reported
- Stages: plan, execute
- Status: blocked: plan: WP-E
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
- Deps: WP-F; the 4489 bookkeeping closed (WP-A7 item 3) before the G3 comparison
- Stages: execute
- Status: blocked: execute: WP-F
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
- Deps: WP-G, WP-C C2 (frozen mask, frame A confirmed)
- Stages: plan, execute
- Status: blocked: plan: WP-G and WP-C C2
- Lead: `executor`; `h1-unfolder` (method, closure), `h1-analysis-critic`
- Condor: yes
- Scope Notes: D8 as adapted by D22 (cell × observable response per charge, event-level β response; bootstraps by generated
  event across the 840x twins, D23 iii). Tests: `split_closure`, `reweighted_closure` per G4; `fake_fraction_bounds: 0 ≤ f_i
  < 0.5 -> true`; `twin_bootstrap: resampling unit = generated event shared by 8402/8405 and 8403/8404 -> true`. Checkpoint: `D/docs/restart_2026-10-27/wp-h/g4_verdict.md`.
- Artifacts: plan-wpH-unfolding.md (default path); `g4_verdict.md`
- Output: `D/docs/restart_2026-10-27/wp-h/`, `D/data/restart_2026-10-27/wp-h/`
- Log: —

### WP-I: radiative correction strategy
- Goal: C_rad table or bound per D9; done when `h1-radiative-correction` signs the record
- Owns: `D/{docs,data,scripts,condor}/restart_2026-10-27/wp-i/`
- Deps: WP-C (observable); execute stage after gate G1″ (WP-A5, WP-A6; D1). Updated 2026-10-28 (evening): WP-C C1 (user grid,
  D16-D20) and the Q4 return; gen level only, runs in wave 2′
- Stages: research, execute
- Status: blocked: execute: WP-C C1 and Q10
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
- Deps: WP-H, WP-I, WP-A5 (veto-correction leg), WP-A6 (ND-spread leg). Updated 2026-10-28 (evening): WP-A56′ supplies both
  legs; WP-D2 the SW77→SW84 transfer leg (if derived) and the stream-factor leg; the η_max-cut efficiency systematic of D15 is
  shown net against the leakage gain (D25 condition)
- Stages: plan, execute
- Status: blocked: plan: WP-H
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
- Status: blocked: execute: WP-J
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
  — answered 2026-10-28 (evening): the user grid replaces U1 (D16-D22) and 840x meets the ≥ 5× request (D23); no request filed.
- Q7 — closed 2026-10-28 (evening): default (a) let stand (D25).
- Q8 (user:) selection edge for the measurement: keep η_max(0.8) < 3.3 (only M_X [4,10) survives G2, 2 bins per charge), move to η_max < 3.0 (WP-A6 in-fit f_ND 0.009–0.094 in all M_X bins; costs ~37 % of events; η_max-cut efficiency from MC becomes a systematic), or move to 2.5 (ND ≤ 0.12 before fitting, Plug Δ +0.065, keeps ~37 % of events). Supervisor recommendation: 3.0. Decision binds WP-B W2, WP-A5 propagation range and the WP-A6 re-ruling. — answered 2026-10-28: 3.0 (D15)

- **Q9 (user:) z definition.** Planned with z* = 2E*/M_X split at 0.2 (D18), because lab z = P·p_h/P·q is ≈ 0 on the Pomeron
  side (median 0.013) and a lab-z split at 0.2 holds 0-4 tracks there. Options: (a) z* at 0.2; (b) z* at the per-hemisphere
  median; (c) another z the user meant (say which). Recommendation: (a).
- **Q10 (user:) Q4 campaign authorisation**, due after wave 1f with the WP-D2 tally: options (a) authorise WP-E..WP-L at
  ≈ 15-30 Condor-days plus the 840x re-production in the writer fork (cost from the WP-D2 tally), (b) authorise WP-E only and
  return after WP-F's per-β G2. Recommendation: (a) if WP-A56′ meets the D24 target in both β bins, else (b).
- **Q11 (user:, conditional)** if a β bin misses the D24 leakage target σ_G2/(1−f_ND) ≤ 0.10: (a) selection on η_max(0.4) < 3.0
  (the fit variable; removes the residual band; costs 10-35 % of events; the η_max-cut efficiency systematic grows), (b) mask
  that β bin, (c) accept the larger leg if G5 still passes. Recommendation: decide on WP-A56′'s side-by-side numbers; default
  planned with (c) when the propagated term stays ≤ 15 %, else (a).
- **Q12 (user:) gen 5719 use.** It is ReqID 8531, the Reggeon hard-subset sample (D23), not an elastic signal; options (a) use
  it as the detector-level Reggeon shape cross-check of 6671/6677 in the hard tail, (b) drop it. Recommendation: (a), after the
  `h1-mc-db` steering confirmation in WP-D2.
- **Q13 (user:, may override a lead decision) 840x as signal MC** (D23) and 9012 out of the nominal mixture. Recommendation:
  keep, conditional on the WP-D2 gates; the alternative (9012 nominal) leaves the leakage estimate MC-starved. Default applied 2026-10-28 (840x adopted per WP-D2); user may override.

Future user touchpoint (not a question now): mask ratification at WP-K (D7); the Q4 return after wave 1c → now Q10 after
wave 1f.

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
- 2026-10-28 WP-A5/WP-A6 (wave 1d, mechanical re-rule at 3.0): G1″(a) PASS 37 (A) / 34 (B) bins, max σ_rel 0.038 / 0.025, q frozen reused (`g1pp_a_verdict_edge30.md`); G2 largest-chain f_ND 0.040 / 0.243 / 0.221, no mask, conditions 2 and 5 recorded (`g2_verdict_edge30.md`, `g2_edge30_conditions_2_5.md`); h1-analysis-critic ruling PASS with conditions 1-8 relayed by the supervisor; deferred: the full conditions list to disk (WP-A56′ step 0), D7 masks not re-evaluated at 3.0
- 2026-10-28 decision: measurement grid = two bins in z, up to five in p_T*, hemispheres x_F > 0 / x_F < 0, two bins in diffractive β, Q² integrated; samples gen 4490 / 9022, 8529-8531 offered; gen 5719 steering to be checked (D16, D17, D20); source: user
- 2026-10-28 decision: "integrate over Q²" → D cells and M_X bins are derivation and systematic strata only (D17); source: user
- 2026-10-28 decision: z = z* = 2E*/M_X split at 0.2, because lab z ≈ 0 on the Pomeron side; pending user confirmation (D18, Q9); source: lead
- 2026-10-28 decision: p_T*² data-quantile grid, finest of q5/q4/q3 passing D7 per (hemisphere, β) cell, start q4; β split at the median 0.523; frame A nominal with B as cross-check; IBU on the user grid with D7 unchanged (D19-D22); source: lead
- 2026-10-28 decision: 8402-8405 adopted as signal MC conditional on the 8405 smoke record, the production tally and the SW77→SW84 transfer check; one floating factor per stream, per-period weights; 9012 → SW84 anchor, out of the nominal mixture; 9022/9023 charm x_IP cross-check only; 8529-8531 hard-tail cross-checks only; gen 5719 = ReqID 8531 Reggeon, not signal (D23, Q12, Q13); source: lead
- 2026-10-28 decision: WP-A5/WP-A6 re-derived on the new mixture as WP-A56′, q per (M_X, D cell), f_ND per β bin, Q² integrated, cells combined, M_X cross-check; leakage target σ_G2/(1−f_ND) ≤ 0.10 per β bin with a user return on failure (Q11); 6670 not used for templates (D24); source: lead
- 2026-10-28 decision: housekeeping defaults let stand — σ_q = max(|q|/2, odd/even spread, σ_stat); G2 GOF branch retired; δ_edge reported beside, band-ρ values per bin (D25; closes Q7, WP-A5 Q-A/Q-B, WP-A6 Q1); source: lead
- 2026-10-28 planner (annotation): parent plan annotated for the wave-1d state; WP-D2 and WP-A56′ added, WP-C re-scoped to the user grid (C1/C2), WP-A7 items 3-4 added, WP-E..WP-L re-sequenced (waves 1e, 1f, 2′-6′); deferred: child plans for WP-A56′ and WP-C C1 (planner after WP-D2), critic conditions 1-8 to disk (WP-A56′ step 0)
- 2026-10-28 WP-D2: 840x adopted as signal (402/402 chunks, validation PASS, w = 0.1324/0.2970/0.1551/0.3748 for 8402/8403/8404/8405, stream factor floated in A56'); SW77->SW84 tier 1 passes at 2 sigma (|Δf|+2e = 0.007/0.025/0.011 vs σ_q 0.034/0.043/0.024; Δq/σ_q ≤ 0.65), tier 2 not run; 840x skims in both sets; deferred: gen-4489 Σweight1 check, rc-134 builder bug, cross-check-label skims (9022/9023, 8529-8531), 5719 steering lookup, h1-analysis-critic signature
- 2026-10-28 supervisor: wave 1e closed; wave 1f opened — planner child plans for WP-A56′ (`.supervisor/working/plans/2026-10-28-wp-a56-rederivation.md`) and WP-C C1 (`.supervisor/working/plans/2026-10-28-wp-c-user-grid.md`) dispatched in parallel; WP-D2 outputs committed; Q9 (z* default) and Q13 (840x adoption default) put to the user, non-blocking
- 2026-10-28 WP-A56′ (planner): child plan written and reviewed (critic PASS r1, scope-critic TRIM applied); deferred: Open Questions 1-2 to the user (override-only; neither blocks a start)
- 2026-10-28 WP-C C1 planner: done; child plan at .supervisor/working/plans/2026-10-28-wp-c-user-grid.md (critic PASS round 2); deferred: Q9 to the user (default z* at 0.2 applied); Q-C1-1/Q-C1-2/Q-C1-3 to the supervisor (D22 reco β split, closure-suite placement in C2, D19 criterion → D7+ plus consult criteria); slice 4b until WP-A56′ delivers f_nd_per_beta.csv
- 2026-10-28 decision: supervisor accepted Q-C1-1 (D22 amended: reco β split 2×), Q-C1-2 (h1-unfolder closure suite placed in WP-C C2 before the mask hash), Q-C1-3 (D19 amended: C1-7 criterion); lead defaults applied: WP-A56′ q strata (M_X, cell) nominal with qbeta beside, 6670 not used; executors dispatched for WP-A56′ (slices 0-5) and WP-C C1 (slices 1-3, 4a)

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
- [x] WP-A5 child plan approved; `veto_correction.json` frozen from the even split; per-bin systematic table; `g1pp_a_verdict.md` (PASS 2026-10-28; edge-3.0 re-rule `g1pp_a_verdict_edge30.md`)
- [x] WP-A6 child plan approved; restricted-range fits for three chains; `f_nd_per_mx.csv`; purity table; `g2_verdict.md` (PASS with masks 2026-10-28; edge-3.0 re-rule `g2_verdict_edge30.md`)
- [x] Q6 (U1 grid/mask rule, ≥ 5× signal-MC request) and Q7 (G2 GOF branch) answered by the user (user grid + 840x, D16-D23; Q7 default let stand, D25)
- [ ] ~~Q4 user return after wave 1c: campaign authorised → unblock WP-E~~ moved to Q10 after wave 1f
- [ ] WP-D2: 840x tally, smoke record, merge validated, period weights, SW77→SW84 transfer check, skims, 5719 and 4489 lookups; `adoption_record.md`
- [ ] WP-A56′ child plan approved; critic conditions 1-8 written to disk; q and f_ND re-derived on the 840x mixture; `g1pp_a_verdict_v2.md`, `g2_verdict_v2.md`; D24 target reported per β bin (variant costed where missed)
- [ ] WP-C C1 child plan approved; `user_grid_record.md` with per-cell grids by the D19 rule; Q9 (z*) confirmed by the user
- [ ] WP-B selection record (wave 1f)
- [ ] Q10 user return after wave 1f with the WP-D2 cost tally: campaign authorised → unblock WP-E
- [ ] WP-A7 items run or formally dropped in WP-L
- [ ] WP-E trees validated (840x included; truth η_max branch)
- [ ] WP-C C2: frame confirmed with the true gen X; mask frozen with sha256
- [ ] WP-F `g2_verdict.md`
- [ ] WP-G control plots reviewed; `g3_verdict.md`
- [ ] WP-H `g4_verdict.md`; unfolded spectra with covariance
- [ ] WP-I C_rad table or bound signed
- [ ] WP-J budget; `g5_verdict.md`
- [ ] WP-K mask ratified by user; results package; figures accepted
- [ ] WP-L note section, handoff, curator close-out
