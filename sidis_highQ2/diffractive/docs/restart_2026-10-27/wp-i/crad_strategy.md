# WP-I research: QED radiative-correction strategy for the diffractive high-Q² SIDIS restart (2026-10-27)

**Scope.** This note decides how QED radiative corrections are handled for per-event-normalised x_F and p_T*² spectra of
charged hadrons. The spectra are in the X (γ*IP) rest frame, in M_X bins, for elastic LRG diffraction at Q² > 150 GeV²
and 0.1 < y < 0.7. It covers the research stage of WP-I in
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(block lines 420-434, rule D9 lines 128-132). The execute stage is out of scope; it is gated on G1 and on the WP-C grid.
No Condor job was submitted. One login-node generator check was run in the session scratchpad (§3), and nothing was
written to the repository except this file.

Abbreviations: HC = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, SH = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2`,
RG = `/afs/desy.de/group/h1/h1gen/rapgap33/rapgap-3.305`, TMP = the session scratchpad `timing/` directory
(`/tmp/claude-37770/-data-dust-group-h1-klesthen-BaryonAsym/04487394-3d8f-4347-8414-f174c1478184/scratchpad/timing/`). TMP is not durable.

## 1. Ruling: the RAPGAP 3.305 diff_rad / diff_nonrad pair

**The existing pair is NOT USABLE as it stands. A corrected pair is USABLE-WITH-CONDITIONS.** `h1-radiative-correction`
and `h1-mc-expert` both reached this verdict independently.

| Item | Finding | Status |
|---|---|---|
| Existing pair | `diff_nonrad` uses IPRO=12; `diff_rad` uses IPRO=1200 with LPIN(4)=1, INT3(1)=5 and ISM3(1)=1 (`HC/MC/rapgap/cmnd/rapgap_diff_{nonrad,rad}.steer:36,323,337,356`). Because the process code differs as well as the QED setting, the README calls it "not a clean toggle" (`SH/MC/rapgap/README.md:27-34`). | verified |
| Clean toggle exists | IPRO=1200 is the HERACLES interface to the IPRO=12 process. Change log: "1200(nrad) and 12 give same results" (`RG/manual/rapgap32.tex:2687`). The inclusive pair already uses IPRO=1200 in both legs (`rapgap_{nonrad,rad}_inclusive.steer:36`). | verified |
| Candidate partner built | `TMP/diff_nonrad1200.steer` is `diff_rad.steer` with only LPIN(4)=0 and INT3(1)/ISM3(1) commented. It runs and gives σ = 140.84 pb, compared with 138.92 pb for IPRO=12. The 1.4 % gap is probably electroweak LPIN(7-11) terms and DPDF grid interpolation. | run verified; cause inferred (`h1-mc-expert`) |
| Rad leg is misconfigured | σ_rad = 119.89 pb = soft+virtual 84.90 + ISR 34.99. FSR is integrated (23.89 pb) but not sampled (ISM3(2) is commented). Compton is off (INT3(3) and ISM3(3) are commented). The soft+virtual term carries the negative counterterm for FSR as well, so the missing FSR events bias both normalisation and shape. The full integrated total is 143.79 pb (`TMP/rapgap_rad.log:517-568`). **The 0.851 rad/nonrad ratio is therefore a configuration defect, not physics.** | verified |
| Generator cuts | ICUT=3 applies the cuts to lepton variables. In the rad leg 116/2000 events have true Q² < 150 and 134/2000 have true y outside 0.1-0.7. Below QMIN the DPDF is frozen at the grid edge (`RG/src/rapgap/rystfu_lhapdf6.F:490-504`). | verified (`h1-mc-expert` awk pass) |
| DPDF | INGG = −30 with IPOM = −30 gives H1 2006 **Fit A**, Pomeron only (`RG/src/h1qcd/h1qcd2006.F:36-40`; both logs print "Fit A"). The nominal 9012 is labelled Fit B, but its steering cannot be recovered (§2). | verified |

**Conditions for the execute stage.** These are the agreed union of the two specialist rulings. Each applies identically
to both legs except where the toggle itself differs.
- C1. Rad leg: copy the QED channel set of `rapgap_rad_inclusive.steer`, i.e. INT3(1),(2),(3)=5 and ISM3(1),(2),(3)=1.
  Non-rad leg: LPIN(4)=0, every ISM3 commented, INT3 as in `rapgap_nonrad_inclusive.steer`. Then check that σ_rad
  agrees with the INT total.
- C2. Both legs: IPRO=1200, the same IPOM/INGG/NFLA/RRXF and the same seeds policy.
- C3. Loosen the generator cuts to QMIN≈100, YMIN 0.05 and YMAX 0.8, and apply the fiducial at analysis level on true
  variables (§4). This removes the lepton-variable cut ambiguity and the frozen-grid edge.
- C4. Generate e⁺p and e⁻p separately. The parent per-event ratios already differ between them, 1.0555 and 1.0677.
- C5. DPDF: use INGG=−31 (Fit B) with IPOM=−30 (Pomeron only; −32 if the Reggeon is wanted) to match the nominal signal model, and use Fit A as a variation. This is a lead
  inference; `h1-mc-expert` rates Fit A versus B as second order for a ratio.
- C6. Born check: compare IPRO=12 with IPRO=1200, LPIN(4)=0 at ≥100k events. A residual of ≥1 % that cannot be
  explained becomes an uncertainty leg.
- C7. Verify the FSR record layout once ISM3(2)=1 is set. The rad-leg FSR layout is provisional because the current
  sample contains no FSR events.

## 2. Provenance (`h1-mc-db`)

- **No non-radiative diffractive DIS sample exists at Q² > 150 GeV²**, either in official MC or on DUST. Sources: the
  2019 DB dump `/afs/desy.de/group/h1/iww2/imc/h1db.js` (every POMUDS/POMC gen row is RAD=Y, rows 2314-2331 and 2637)
  and the saved generator logs. The live MC DB is disabled, so the result is verified only for these sources.
- All known diffractive samples are radiative: 9012 (gen 4489; GenRad 0/1/2 = 57.1/25.1/17.8 %,
  `SH/diffractive/docs/mc/elastic_uds_replacement_search_2026-10-17.md:73`), 9024/9033, 9155/9157/9158 and 6671/6677.
  The logs show INT2/INT3 on.
- **9012 cannot be replicated "except QED".** Its steering block is empty (`logfiles/rapgap31/gen_4489.txt:19-23`), it
  ran RAPGAP 3.01/01 rather than 3.305, and its GEN file is tape-only. The 3.305 pair is therefore a stand-in model of the
  QED effect and not a twin of the signal MC. This is acceptable for a ratio but must be stated in the record.
- The parent's non-rad legs are Britzger's private NORAD productions under `/data/dust/group/h1/britzger/norad_prod21/`.
  None of them is diffractive.

## 3. Timing shard and cost

**Login-node runs.** Host naf-hone21, 2026-10-27, binary `/afs/desy.de/group/h1/h1gen/rapgap33/standalone/bin/rapgap_hepmc`
(RAPGAP 3.305), seed 9101, 2000 events per run.

| Run | Wall | σ [pb] | HepMC size |
|---|---|---|---|
| diff_nonrad1200 | 7.2 s | 140.84 | 16.9 MB |
| diff_rad (current card) | 2.8 s | 119.89 ± 0.08 | 16.7 MB |
| diff_rad_full (corrected C1/C3 card: + INT3(3), ISM3(2),(3); QMIN 100, y 0.05-0.8; seed 9102) | 5.0 s | 339.03 ± 0.19 | — |

The corrected card is `TMP/diff_rad_full.steer`. Its σ is not comparable with the other rows because the generator cuts
are looser. With looser cuts, only part of the generated sample lands in the Q² > 150, 0.1 < y < 0.7 fiducial. Taking the
σ ratio as a rough guide (≈140/339), about 2.4× more events must be generated per fiducial event (inference). The rad sample has 241 of 2000 events with a collinear photon (θ > 179°, E > 1 GeV); the non-rad sample has none. The
historical Condor rate is 25-52 s per 2000 events including BASES (`SH/MC/rapgap/README.md:57-64`).

**Cost of a full pair.** The specialist recommendation is 2M fiducial events per leg and per lepton charge: 4 legs, 8M
fiducial events, or about 20M generated with the loosened cuts. That is roughly 1000 jobs of 20k events. Two figures set
the generator cost:
- Condor rate: (25-52 s / 2000 events) × 20M events ≈ 70-145 core-hours. This is an upper bound, because BASES is
  amortised over 20k-event jobs.
- Login-node rate: 2.5-3.6 ms per event, about 14-20 core-hours.

The ≥100k Born check adds less than 1 core-hour. The HepMC→ROOT reduction (`HC/MC/scripts/reduce_hepmc_to_root.py`) was
not timed and may dominate. Transient HepMC is about 8.4 GB per 1M events and is deleted per job.

**Cost line.** About 1-6 core-days, a few hours wall at 400 slots, a few GB of ROOT output, no new official MC.
This fits the plan assumption of ≤ 1 Condor-day per mode (plan :258-259). The `.sub` file and wrapper belong to the
execute stage, under `SH/diffractive/scripts/restart_2026-10-27/wp-i/` and `SH/diffractive/condor/restart_2026-10-27/wp-i/`.

## 4. Definition, and what eSigma plus the ISR-only convention already removes

**Definition** (`h1-radiative-correction`). Use the per-event ratio, not the parent's yield ratio:

C_rad(bin) = [(1/N_ev) dN/dO]_nonrad / [(1/N_ev) dN/dO]_rad, for each M_X bin, each hadron charge and each lepton
charge, with O = x_F or p_T*².

The parent yield ratio, with DJANGOH14 median 1.146, contains a DIS event-rate factor of about 1.08. That factor cancels
in a per-event-normalised spectrum (derived from the numbers in `SH/docs/radiative/crad_machinery_guide_2026-09-28.md` §8 item 5, §10).

**Gen-level kinematics.** True q is the pdg 23 line outgoing at HepMC vertex −1. On the current ISR-only records it equals
the electron after ISR minus the scattered electron, so it already has ISR removed. The FSR case is condition C7. The IP four-vector is the pdg 990 line at vertex −2, which matches
p − p′ component by component. The X system is X = q + IP and M_X² = X². All momenta must be rescaled by 1000 (the
known HepMC2 GeV/1000 defect). This reading was verified on records by `h1-mc-expert`.

This convention is the parent's ISR-only true γ* (memory decision `2026-10-24-gen-all-kinematics-true-gamma-isr-only-incl-phih`).
The fiducial on true variables is identical in both legs: Q² > 150, 0.1 < y < 0.7, plus the WP-C x_IP, |t| and M_X cuts.

**Already removed** by the ISR-only true γ* at gen level, with eSigma at reco: ISR shifts of Q², x and y, and therefore of
the X-frame boost, M_X, β and x_IP.

**Residual effects that C_rad must carry:**
- (i) the bin mix inside an M_X bin, because ISR lowers the effective √s;
- (ii) FSR, which the parent leaves in k′ because it does not subtract final-state photons;
- (iii) Compton events;
- (iv) the soft, virtual and vacuum-polarisation weights acting on the event mix.

**Expected size** (inference from the parent). The per-event multiplicity ratio is 1.047-1.068
(`crad_machinery_guide_2026-09-28.md` §10), so shape effects of up to about 5-7 % are plausible. This is larger than the
D9 3 % floor.

## 5. Fallback: raw level plus a bound, and how it enters WP-J

- **Source of "<5 %".** The local plan line `SH/diffractive/H1DiffractiveHighQ2SIDISPlan.md:249` cites nothing. The
  published origin is H1, Eur. Phys. J. C72 (2012) 2074, arXiv:1203.4495: "Radiative corrections are calculated bin by
  bin using the HERACLES program interfaced to RAPGAP. They are smaller than 5% for all measured data points." It also
  states "The uncertainty on radiative corrections is 1%" (verified from the arXiv full text). EPJ C48 (2006) 715 also
  corrects to Born level via HERACLES and gives no size.
- **Limits of the bound.** H1 *applied* the correction and quoted 1 %. The 5 % figure bounds the size of a correction to
  an inclusive cross section, not an uncertainty on uncorrected hadron spectra. It does not cover boost or M_X
  migration.
- **Fallback rule**, used only if the pair is not produced or fails C1-C7: no correction, plus one symmetric leg
  max(|1−C_rad,est|, f). The leg is fully correlated across bins within a charge as a single nuisance, and is added in
  quadrature to the other WP-J legs. Its normalisation part cancels per event; the shape part survives.
- **The floor f.** The D9 value f = 3 % is below the parent per-event multiplicity effect of 5-7 %. Lacking a diffractive
  estimate, the lead recommends f = 6 %, the parent e⁺ per-event value rounded (inference). This
departs from plan rule D9 and needs supervisor sign-off. This floor would be one of
  the larger legs against the G5 median ceiling of 25 %.
- **WP-J legs if the pair is used:**
  - (i) statistical;
  - (ii) QED model, |C(full ISR+FSR+Compton) − C(ISR-only)|, where the ISR-only card has FSR and Compton off in both
    INT3 and ISM3;
  - (iii) Born residual from C6, if any;
  - (iv) closure of the diffractive leptonic δ against the inclusive pairs in the same true variables, a leg above 2 %;
  - (v) a 1 % floor, following the H1 2012 precedent.

  This matches the parent's separate C_rad stat and gen columns (`SH/docs/status/radiative-corrections.md:29`).

## 6. What transfers from the inclusive parent

- **Transfers.** The parent applies C_rad post-unfold: σ_Born = C_rad × Unfold. The pipeline is
  `HC/scripts/radiative/final_crad_consistent_2026-10-21/` with `crad_v3_worker_final_consistent.py` and
  `finalize_final_consistent_djangoh14.py`. These parts carry over: the `ratio()` stat propagation (line 47), the
  manifest/partials/finalize Condor flow, the y-resolved sums, the per-beam fraction (`CRAD_F_EP`) and the
  file-scoped event joins (guide §8.1).
- **Does not transfer.** The (Q²,x,z,pT) grid constants (CELLS, ZED, PTED, `bindex()`, 245/490 offsets,
  `np.zeros(245)`), the xF_gen < 0 cut, the hadron-yield weighting and the H1OO HAT/MODS input format. The diffractive
  pair is HepMC→ROOT. The older `HC/scripts/canonical_highQ2_sidis_2026-06-30/radiative_corrections/full_crad_finalize.py`
  is superseded.
- **Inputs.** The parent's DJANGOH/RAPGAP31 pairs are non-diffractive and cannot be reused as inputs. They serve only as
  the closure reference in leg (iv).

## 7. Recommendation

**Produce the corrected generator-level RAPGAP 3.305 pair (conditions C1-C7), compute the per-event C_rad per M_X bin, and
apply it post-unfold.** Use the legs listed in §5.

This reverses revival-assessment item 5 (`SH/diffractive/docs/research/revival_assessment_2026-10-27.md:239-240`), which
recommended raw level plus a bound. Two reasons:
- (a) the pair costs about 1-6 core-days, with no official MC;
- (b) the only sourced bound is a 1 % uncertainty on an *applied* inclusive correction, while the expected per-event
  shape effect, at 5-7 %, would exceed a 3 % floor.

Keep the fallback in §5 (f = 6 %) in case the pair fails its conditions.

## 8. Dependence on WP-C's final binning

- **Independent of the binning:** the pair ruling, the steering conditions, the cost, the definition and the fallback
  rule.
- **Depends on the binning:**
  - the per-bin C_rad values;
  - whether 2M events per leg and charge meets ≤1 % ratio stat in core bins and ≤3 % in tail bins (x_F → ±1, high p_T*²);
  - the fiducial x_IP, |t| and M_X range;
  - the requirement that the WP-H response truth uses the same q = pdg 23 + IP definition, without which C_rad
    double-counts or misses ISR.

## 9. Verified vs inferred

- **Verified:** every steering line, log number and timing in §1 and §3; the DB and log provenance in §2; the parent
  numbers in §4 and §6; the H1 2012 quotes.
- **Inferred:**
  - the cause of the 1.4 % Born gap;
  - Fit A versus B being second order;
  - the 5-7 % size expectation and f = 6 %;
  - the cost of the reducer, and the ≈2.4× generated-to-fiducial factor;
  - the FSR record layout.

## 10. Risks, open questions, gaps

- **FSR convention.** The parent leaves FSR photons in k′. `h1-radiative-correction` proposes adding them back into k′
  (q pre-FSR). The two choices define different Born observables. The execute stage must take the parent convention
  unless the user changes it.
- **Born gap.** The 1.4 % gap at 2000 events was not resolved at high statistics (condition C6).
- **9012 model.** The DPDF fit and the reggeon content of 9012 are unknown. A Reggeon contribution matters at large
  x_IP, since RRXF allows x_IP up to 0.2.
- **No second generator.** No second generator with diffractive QED exists, so the model leg is RAPGAP-internal only.
- **Papers not read.** The H1 1998 hadron-final-state papers and the ZEUS papers were not read for radiative statements.
- **Path mismatch.** The plan's `Owns` field names `D/.../restart_2026-10-27/wpI/`. This brief used `wp-i/`. The
  execute stage should pick one.
