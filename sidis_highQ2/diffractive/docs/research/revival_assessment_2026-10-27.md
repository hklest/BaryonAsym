# Revival assessment: diffractive high-Q² charged-SIDIS extension (2026-10-27)

**Scope.** Why the LRG diffractive extension was parked, what is reusable, what is missing, prior art, and the
user-only decisions. Input for `planner`; no plan or WBS. Relative paths resolve under
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/` (abbreviated `D/`) unless they start
`sidis_highQ2/` or `H1ChargedSidis_HighQ2/` (workspace root). Tags: **[V]** verified in the cited file;
**[I]** inference by this assessment.

**Premise correction.** The request lists "defining binning, identifying the MC samples needed" as open steps.
Both were done on 2026-10-16/20. The binning is DECIDED with user sign-off: a 45-bin grid per charge and
pre-registered masks (`D/docs/binning/diffractive_binning_baseline_2026-10-17.md:14-21`). The MC inventory,
luminosities and composite scale table are closed and signed (`D/context-handoff.md:54-68`); the C_rad pair and
model-variant samples are known gaps (§3.4-3.5). The analysis
stopped at T4 (backgrounds), before unfolding. The blocker is not missing steps. It is a FATAL
data-vs-MC shape failure (§1). A partial revival on 2026-10-20 already ran its first stage and returned
NO-GO (`D/docs/t4/localization_verdict_2026-10-20.md:1-5`).

## 1. Why the extension was parked

**Verdict [V].** "FATAL — for any unfolded diffractive SIDIS result under the current evidence register"
(`D/docs/t4/final_path_adjudication_2026-10-20.md:28-35`). Unfolding was ruled NOT PERMISSIBLE, and the
systematic-viability stop condition was TRIGGERED (`…adjudication…md:49-82, 241-266`). The user accepted the
ENVELOPE-ONLY end state and parked the extension (`D/context-handoff.md:22-40`).

**Selection and masks under which it was evaluated [V].**
- Fiducial region: x_IP<0.03, M_Y<1.6 GeV, |t|<1 GeV², M_X>2 GeV, on top of the parent Q²≥150 GeV²,
  y∈[0.1,0.7] (`D/H1DiffractiveHighQ2SIDISPlan.md:18-43`; `D/docs/binning/…baseline…md:82-92`).
- Detector-level LRG cuts: η_max<3.2 (LAr clusters >0.8 GeV), E_Plug<7 GeV, FMD ≤1 hit pair in layers 1-2,
  no FTS hits at 26/28 m (`D/H1DiffractiveHighQ2SIDISPlan.md:80-91`). Stack C (cluster `etaMaxClus08`
  authoritative) and stack R were both tested (`D/docs/t4/localization_verdict_2026-10-20.md:26`).
- Binding pre-registered masks: h⁺ 16 / h⁻ 14 bins masked. Scenario B (+ full D2) is the leading candidate
  but NOT ratified (`D/context-handoff.md:38-41`; `D/docs/t4/t7_mask_review_proposal_2026-10-20.md`).
  The verdict holds "under any mask scenario" (`…adjudication…md:30-31`).
- Data: certified forward-detector run list, 317,212.80 nb⁻¹, FROZEN 2026-10-21 (`D/context-handoff.md:60, 82-90`).

**Quantitative blockers [V].** The adjudication finds the failure "at least three independent failures deep"
(`…adjudication…md:106-117`):
1. *ND leakage shape (background vector).* The η_max template fit with two free templates (plan §7.1) fails
   on 79,008 cut-removed events (D0/D1/D2 = 44,583/23,947/10,404). Fine D/ndf is 5925.8/358 with the
   RAPGAP31-7071 ND template and 6457.8/358 with DJANGOH14-7091. Coarse D/ndf is 5248.5/132 and 5832.3/132.
   The template-statistics-aware toy p-value is ≤0.0033, the 300-toy floor, in every cell. 51-60% of the
   data sit at η≥3.2 (`…adjudication…md:59-76`). The DF-SYS-01 nuisance refit and the five-scale
   per-component fit also fail (`D/docs/diffractive_feasibility_note_2026-10-20.md:52-61`). The two ND
   generators also disagree with each other: RAPGAP is higher by ~1.31× after correction, and neither is a
   defensible nominal (`D/context-handoff.md:28-32`).
2. *Response (purity and migration).* Plan §7.2 handles M_Y/|t| migrations through the dissociation MC. That
   MC's η_max description fails with mixed sign: +32% in D0, −27% in D2. Both η branches fail
   (2051.59/179 with vertex, 1827.13/180 with cluster). The signal-only shortfall is 0.714-0.759
   (`…adjudication…md:108-112`). DF-SYS-01 remains UNEVALUATED, with response-matrix usability unproven
   (`…adjudication…md:112-114`).
3. *Selection (forward vetoes).* DF-SYS-02 (FMD nFmu12) has a maximum deviation of 0.366-0.459 under every
   cocktail (`…adjudication…md:64-66`). The later N−1 study confirms that **all three** forward elements fail.
   Max |MC/DATA−1| is FMD 0.79-1.77, Plug 0.97-1.00 and FTS 0.73-0.82; Plug is the one localized, composition-independent failure
   (`D/docs/t4/localization_verdict_2026-10-20.md:40-42`). The data Plug veto-fail fraction is
   0.207/0.230/0.283 (D0/D1/D2). The largest rate in any MC component is 0.146, so no non-negative mixture
   can reproduce it (`…localization…md:46`).
4. *Gate.* The T5 inclusive σ_r^D(3) gate (plan §10.2) was never run, and T6/T7 are unreachable in the
   plan's own ordering (`…adjudication…md:71-73`).
5. *Statistics on top of systematics.* Retained bins carry median 26-45% envelope effects (worst 50-75%) on
   a grid that is already "tens-of-events fragile" (`…adjudication…md:256-258`).

**What would overturn it [V]** (adjudication §7, `…adjudication…md:303-316`; repeated in the feasibility note
§5, `D/docs/diffractive_feasibility_note_2026-10-20.md:309-359`):
(1) a leakage constraint that passes fine, coarse and template-statistics-aware GOF on the v3 payload for at
least one ND alternative; (2) a DF-SYS-01 treatment that passes its own validation and does not project onto
the (D,z,pT) migration structure; (3) DF-SYS-02 resolved with validated FMD/Plug/FTS N−1 distributions;
(4) the T5 gate outcome; (5) period-matched generator pairs, **delivered 2026-10-20, no gate change**
(`D/context-handoff.md:42-52`); (6) a noise stream or model.
The revival route, adjudication §3(ii), has five preconditions: attribution to a named simulable defect,
localization first, a resolvable noise prerequisite, governance (a plan amendment because §6.2 says "Do not
retune generators"), and budget realism (months of work, success not guaranteed)
(`D/docs/diffractive_feasibility_note_2026-10-20.md:314-342`).

**Status of the revival preconditions after parking [V].**
- Localization ran on 2026-10-20 and found **one** named element: a composition-independent Plug
  forward-energy/response deficit, "most parsimoniously read as detector-side". FMD, FTS and the ODS/LAr η
  failures remain response-or-composition ambiguous. The stage-1 go/no-go was **NO-GO** for a full campaign.
  Only a narrow Plug pre-campaign investigation was authorized (`…localization…md:66-72`).
- The low-M_Y signal itself probes forward modelling. At the elastic peak, only 58-65% of MC events pass the
  vetoes (`…localization…md:64`).
- **Noise (B3), now partly resolved:** MC overlays real random-trigger events (`RT_NOISE=Y` on every v6dif
  ReqID; OVERLAY PRESENT) (`D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md:24-41`). The
  fidelity of the overlaid rates against data is not determined, and the handoff trap
  (`D/context-handoff.md:92`) still says "no random-trigger stream" for the data side **[V/I: the handoff
  predates the 2026-10-22 record]**.

**Plan §12 open human decisions** (`D/H1DiffractiveHighQ2SIDISPlan.md:370-379`), near-verbatim, with status:
1. *x_IP range:* x_IP<0.03 (more statistics, growing Reggeon contribution above ≈0.01) versus x_IP<0.01
   (cleaner Pomeron physics), or a split into two bins. **Status:** START <0.03 in use. The split was kept as an
   option (it doubles the grid to 90 bins/charge) and never decided (`D/docs/binning/…baseline…md:454-456`).
2. *Signal definition:* Y=p plus dissociation with M_Y<1.6 GeV (H1 standard), or correct to Y=p with the
   1.20 ratio (≈7% extra normalisation). **Status:** in practice Y=p+diss. The 83:17 mixture is frozen with a
   ±7.2% ratio systematic (`D/context-handoff.md:67`; `…baseline…md:208-211, 379`). No explicit user
   sign-off of §12.2 was found **[I]**.
3. *Low-mass and high-β region:* keep M_X>2 GeV as the main cut, and decide whether the β<0.8 subsample is the
   primary result for DPDF comparisons. **Status:** M_X>2 in use. No β<0.8 decision record was found by grep
   of `D/docs` **[V negative search]**.
4. *Background baseline:* subtraction (H1 practice) versus a full cocktail. **Status:** subtraction per plan
   §8.2. The decision is moot while no admissible ND normalisation exists (`…adjudication…md:51-56`).
5. *Data periods:* the parent periods and charges minus runs without working forward detectors.
   **Status:** done (certified list, 18,052 ACCEPT runs; `D/context-handoff.md:60`).
6. *Leading-proton (FPS/VFPS) cross-check:* likely too statistics-limited at high Q²; decide whether to spend
   effort on it. **Status:** undecided. The VFPS-period sets are listed only as outside the four-period
   bookkeeping (`D/docs/mc/diffractive_mc_inventory_2026-10-15.md:140`).

## 2. What exists and is reusable

### 2a. v6dif ntuples (`D/output_v6dif/`) [V unless tagged]
Data: `data/{ep0304,em0405,em06,ep0607}` (true-period unmixed; `D/docs/diffractive_feasibility_note…md:34-35`).
Scale, weight and roles come from `D/docs/mc/composite_scale_table_v3_2026-10-20.csv` (rows 2-11) and
`D/docs/mc/luminosity_diffractive_mc_2026-10-16.csv`. Radiative content comes from
`D/docs/mc/diffractive_mc_semantics_2026-10-16.md:308-310`.

| ReqID (size) | Generator / process | Rad | Weight / lumi status | Use in t0-t4 |
|---|---|---|---|---|
| 9012 (66M) | RAPGAP31 POMUDS elastic uds diffractive, Fit B, ep0607 sim; projected to all 4 periods (fully correlated) | HERACLES rad | w≡1; L_MC 52,273 nb; CLOSED | nominal elastic signal and response (T0-T4) |
| 9024 (292M) / 9033 (195M) | RAPGAP diffractive charm elastic, e+/e− | rad [I: family] | x_IP-slice flux weights; L closed; correlated streams | signal cocktail (T0-T4) |
| 9155/9157/9158 (195/99/194M) | RAPGAP PDISS uds (proton dissociation, DIFFVM-like M_Y), em0405/em06/ep0607; 9158 borrowed for ep0304 | rad | w1·w2≡1 at Q²>150; ×5.3611 on truth-in-cut signal only | dissociative signal, M_Y migrations |
| 6671/6677 (108M each) | RAPGAP Reggeon elastic, e−/e+ | rad | native weights; reported separately, never in the signal cocktail | Reggeon background |
| 7071 (9.6G) | RAPGAP31 non-diffractive DIS, em06 (parent central v6 reuse) | parent RAD MC [I] | scale row re-ruled 2026-10-20 (6.0277× defect corrected) | ND leakage alternative A |
| 7091 (32G) | DJANGOH14 non-diffractive DIS, ep0607 (parent reuse) | parent RAD MC [I] | L 4,379,935 nb; CLOSED | ND leakage alternative B |
| 7094/7096/7097 (20/20/9.6G) | RAPGAP31 ND four-catalog, 06_07e+/04_05e−/2004e+ | [I] | own-catalog L 3.05/3.03/1.53 ×10⁶ nb; sidecar scale rows, not in the signed v3 table | nd4cat fit and localization extension (`D/docs/t4/localization_verdict…md:52`) |

DJANGOH14 7072 (em06) was used only gendump-style in the pair record, with no v6dif tree
(`D/docs/mc/period_matched_pair_record_2026-10-20.md:18-34`). DIFFVM samples do not reach Q²>150 and are
background-only (`D/docs/mc/diffractive_mc_inventory_2026-10-15.md:26-27`). Every v6dif ReqID carries the
random-trigger overlay (`D/docs/t2/mc_randomtrigger_overlay…md:24-30`).

**Other reusable diffractive assets [V]** (`D/context-handoff.md:58-68`): the certified run list and frozen
lumi table; the exposure convention ruling; composite response v3 (`D/data/t4/composite_response_v3_2026-10-20/`);
the §7.1 payload (79,008 events, sha256 `341e5a1e…`); leakage-fit, datamc and mask-review products; the
systematics ledger (DF-SYS-01/02 registered UNEVALUATED); and the particle-level §2.1 implementation
(`D/scripts/t1_particle_level/particle_level.py`).

### 2b. Inclusive v6 pipeline components and what a diffractive port would change
- **Binning.** Parent: 245 bins/charge, 7 (Q²,x) cells × 7 z × 5 pT, reco pT split 2×, response (491,246)
  (`sidis_highQ2/docs/RUN_STATUS.md:90-91`). The diffractive 45-bin grid is a lossless merge of the parent
  cells (`D/docs/binning/…baseline…md:24-79`). *Change:* the grid has no x_IP, β or M_X axis (they "remain
  unbinned", `…baseline…md:10-12`). A diffractive observable in (β, x_IP) or M_X needs a new axis, which
  doubles the grid or more under already-starved statistics.
- **Unfolding.** Response builder `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`.
  IBU uses k=4 with the coherent body+miss prior (`RUN_STATUS.md:92`). The current engine call is
  `eng.ibu_unfold` (`H1ChargedSidis_HighQ2/scripts/unfolding/esigma_final_2026-10-24/esph_downstream.py:170-173`).
  Fakes are subtracted as a fraction f_i on the data vector (same line). *Change:* train on the diffractive
  cocktail only. Fakes must include M_Y>1.6, |t|>1, x_IP>0.03 and M_X<2. ND leakage is subtracted before
  unfolding (plan §8.2, `D/H1DiffractiveHighQ2SIDISPlan.md:261-270`). The empty-bin guard and masks are
  already specified (`…baseline…md:305-367`).
- **C_rad.** The parent definition is C_rad=σ_nonrad/σ_rad, gen-only and post-unfold per bin. DJANGOH14 is the
  applied primary and RAPGAP31 the formal alternative (`RUN_STATUS.md:169-190`). The finalizer is
  `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/radiative_corrections/full_crad_finalize.py`.
  *Change:* this needs a diffractive rad/non-rad pair. All official diffractive SimRec samples are radiative only (§3).
- **Systematics.** Budget v2 has 26 quoted sources: IBU stat/model/reg, HFS JES/RCES/θ, electron E/θ, lumi,
  tracking, φ_h, C_rad gen/stat, background-subtraction terms, DIFFVM ρ, non-primary, NucliaWeight
  (`H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphotonxf_2026-10-24/esph_budget_v2.csv`
  header; `RUN_STATUS.md:501`). Detector legs work as MC-only response swaps, IBU(D_sub;M_leg)−IBU(D_sub;M_nom)
  (`esph_downstream.py:177-186`). *Change:* the mechanism is reusable as is. The plan §9 adds 16 diffraction
  sources (`D/H1DiffractiveHighQ2SIDISPlan.md:286-303`), but its model-systematic bands assume an MC that
  describes the data, and that assumption failed (`D/docs/diffractive_feasibility_note…md:330-336`).
- **Background subtraction.** Parent Set R' = e_cone_applied + V0_genavg + lowQ2 (`RUN_STATUS.md:350`). The
  code is in `H1ChargedSidis_HighQ2/scripts/background_subtraction/` and the fit-norm code in
  `H1ChargedSidis_HighQ2/scripts/bgsub_fitnorm_2026-10-19/`. *Change:* the dominant diffractive background is
  ND leakage, which needs a validated normalisation. This is the failed item.
- **Convention drift [V/I].** The parent has since moved to the OPT-A Q2esGen truth contract (2026-10-20) and
  to eSigma photon-frame x_F with ISR-only gen kinematics (2026-10-24) (`RUN_STATUS.md:358, 491-503`). The
  diffractive products predate both (grid frozen 2026-10-17 from `EVENT_CUTS_RECO`, `…baseline…md:82-89`). A
  revival must re-base on the current parent conventions, or the diffractive/parent ratio is not like for like **[I]**.

## 3. Gaps on the way to unfolded cross sections
1. **Validated ND-leakage model** (the blocker). Neither ND generator passes GOF, and no third
   detector-level ND generator exists in the four-period bookkeeping **[V/I]** (`…adjudication…md:59-76`).
   The ND fraction in the η window grows with Q² cell: 3.7% / 10.8% / 40.6% (D0/D1/D2) (`…localization…md:62`).
2. **Forward-detector response.** The Plug deficit is localized but not corrected. FMD/FTS are unlocalized.
   The overlay rates are not yet compared with data (plan §4.3 table never produced; §10.1 checkbox unticked,
   `D/H1DiffractiveHighQ2SIDISPlan.md:314`).
3. **LRG-selection efficiency for signal.** At the elastic peak only 58-65% of MC events pass the vetoes, and
   the data disagree (`…localization…md:46, 64`). Signal efficiency is therefore model-dependent until item 2 is fixed.
4. **C_rad for diffraction.** There is no non-radiative diffractive SimRec. The standalone RAPGAP 3.305
   `diff_nonrad` (IPRO=12) versus `diff_rad` (IPRO=1200+HERACLES) is "not a clean QED rad on/off toggle".
   The two have different σ (138.92 vs 119.87 pb) (`sidis_highQ2/MC/rapgap/README.md:11-12, 22-34`). This is
   generator-level only, and no local SimRec exists (`D/docs/mc/diffractive_mc_inventory…md:236-240`). The
   plan quotes H1 at <5% for inclusive diffraction (`…Plan.md:249`).
5. **Model-variant samples.** The ARIADNE/CDM Pomeron sets 8221/7448 are HAT+MODS only, with DST on unmounted
   /acs, so no ODS and no η_max (`…inventory…md:151-161, 230`). No detector-level Fit A sample was identified
   by the inventory **[V negative search]**. The plan §9 shower and DPDF systematics therefore lack inputs.
6. **Period coverage.** There is no ep0304 PDISS sample (9158 is borrowed) and no light-flavour Reggeon
   SimRec at detector level for 6671 (`D/docs/mc/diffractive_mc_semantics…md:35-42`). **Source conflict:** that record calls 6671 "not usable at
   detector level (hat-only, no DST)", yet a v6dif 6671 tree exists and entered the localization stack
   (`…localization…md:30`). Reconcile this with `h1-mc-db`. The 9012 elastic stream
   is one truth stream for 4 periods, giving correlated MC stat of 16% in D2
   (`D/docs/t0/t0_yield_prediction_2026-10-18.md:58-66`).
7. **Not done:** T3 M_X/x_IP/β reconstruction resolution and c(η_max) beyond partial work; T5 inclusive gate;
   T6-T9; all 11 §10.1 checks and the 10 §11 task boxes are unticked; the checkboxes were not maintained, and T0-T4 were executed (`…Plan.md:311-367`).
8. **Handoff Priority Actions** (`D/context-handoff.md:95-112`): (1) stay parked; (2-3) done; (4) re-open
   only on an explicit revival decision, via the §3(ii) preconditions. Any mask change requires fresh user
   ratification from GOF-passing inputs. (5) The parent proceeds independently.
9. **Possible contradiction to check [I].** The T0 particle-level fiducial prediction is ~130.6k signal
   events before acceptance (`D/docs/t0/…2026-10-18.md:37-43`). The raw LRG-selected data count in a
   preliminary study is 12,583 events (`D/docs/t2/lrg_selection_study_2026-10-18.md:74-78`). The two counts are defined differently (the T2 study is a
   preliminary run before certification), so this needs an explicit acceptance-chain
   reconciliation before any revival budget.

## 4. Physics prior art (H1/ZEUS hadron final states in diffractive DIS)
- H1, *Hadron production in diffractive DIS*, PLB 428 (1998) 206, [hep-ex/9803032](https://arxiv.org/abs/hep-ex/9803032).
  1994 data at low Q². Measures energy flow, charged-particle x_F, p_T*² and ⟨p_T*²⟩(x_F) in the **X (γ*IP)
  rest frame**, in M_X ranges, compared with EMC γ*p results.
- H1, *Multiplicity structure of the hadronic final state in diffractive DIS*,
  [hep-ex/9804012](https://arxiv.org/abs/hep-ex/9804012). Multiplicities and rapidity spectra in the X rest frame versus M_X.
- H1, *Thrust jet analysis of DIS large-rapidity-gap events*, EPJC 1 (1998) 495,
  [hep-ex/9711006](https://arxiv.org/abs/hep-ex/9711006). 10<Q²<100 GeV². Thrust and P_t versus M_X.
- ZEUS, *Event shape analysis of DIS events with a large rapidity gap*, PLB 421 (1998) 368,
  [hep-ex/9710027](https://arxiv.org/abs/hep-ex/9710027). 5≤Q²≤185 GeV². Shapes in the γ*-IP frame versus M_X.
- ZEUS, *Properties of hadronic final states in diffractive DIS* (LPS-tagged),
  [hep-ex/0107052](https://arxiv.org/abs/hep-ex/0107052). 4<M_X<35 GeV. Thrust and sphericity in X.
- H1 diffractive dijets HERA II, JHEP 03 (2015) 092 ([1412.0928](https://arxiv.org/abs/1412.0928)), and
  inclusive LRG, EPJC 72 (2012) 2074. These are the selection, unfolding and σ_r^D(3) references already in
  the plan (`…Plan.md:383-387`).

**Implication [I].** The established observables are x_F (or z) and p_T* in the **X rest frame**, binned in
M_X (or β), mostly at Q²≲100 GeV², with no fully unfolded multidifferential (Q²,x,z,pT) diffractive SIDIS. A
"first" claim is defensible only in a qualified form: first high-Q² (Q²>150) unfolded multidifferential
diffractive SIDIS. The present grid uses parent γ*p-frame z and pT, not X-frame variables
(`…baseline…md:10-12`). Searches found no H1 high-Q² diffractive charged-hadron spectrum; H1 PLB 654 (2007) 148
([0706.2456](https://arxiv.org/abs/0706.2456)) is non-diffractive.

## 5. Supervisor-facing summary
**Decisions only the user can make**
1. *Revive at all?* Options: (a) stay parked, publish the parent (recommended by the adjudication,
   `…adjudication…md:200-218`); (b) a narrow Plug pre-campaign investigation only (authorized scope,
   `…localization…md:70-72`); (c) fund the full MC-response campaign (months, a plan amendment against "do
   not retune generators"). **Recommendation:** (b) as a bounded probe with a pre-registered kill criterion.
   Move to (c) only if Plug, FMD and FTS all localize to simulable defects.
2. *Scope of a revived measurement:* keep the 45-bin parent-frame grid, or pivot to an X-rest-frame x_F/p_T*
   measurement in M_X (or β) bins, integrated over the high-Q² range (fewer bins, matches §4 prior art).
   **Recommendation:** the pivot, because it is coarser and more comparable with published work.
3. *Re-base on current parent conventions* (OPT-A Q2esGen, eSigma photon frame) before any new production:
   yes or no. **Recommendation:** yes.
4. *Plan §12.1/12.3/12.6* (x_IP split, β<0.8 primary, FPS/VFPS cross-check). **Recommendation:** x_IP<0.03
   unsplit; β<0.8 as a subsample; no FPS/VFPS.
5. *Accept a non-clean C_rad* (standalone RAPGAP rad/non-rad, gen-level) or quote at raw level with a
   systematic. **Recommendation:** raw level plus a quoted bound, given the <5% expected size.
6. *Mask ratification* from GOF-passing inputs (required for any mask change, `D/context-handoff.md:108-109`).

**Risks that could re-FATAL the analysis**
1. The ND-leakage shape remains GOF-failing with every available generator (composition-side root cause, no simulation fix).
2. The FMD/FTS/ODS-η failures do not localize, or prove composition-side (`…localization…md:68-69`).
3. The overlay noise rates disagree with data, and there is no data random-trigger stream to correct them.
4. Signal veto efficiency at the elastic peak is model-dependent at the 35-42% loss level, so the response is
   not validatable.
5. The T5 σ_r^D(3) gate fails against EPJC 72 (2012) 2074 (`…Plan.md:325-353`).
6. Statistics: D2 and high-z bins are starved (16% MC stat in D2 already). Adding x_IP/β/M_X axes worsens this.
