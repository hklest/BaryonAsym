# G1 RAD gate recalibration — decision record (2026-10-04)

**Author role:** `h1-radiative` (radiative-correction specialist). This document is the
manifest-recorded reason required by `spec_nonrad_rad_phih_v3_2026-10-02.md` item 8
("Gate thresholds may be changed only by `h1-normalization`/`h1-radiative` with a
manifest-recorded reason"). No code was changed and no production was run for this
document.

**Product:** RAD-vs-NONRAD truth-level phi_h overlay, fork
`plot_phi_rad_nonrad_gen_v3_compact.py`, output basis
`plots/phi_h/rad_vs_nonrad_v3_cm_2026-10-03/` (smoke cluster 5323776,
`smoke_1file/manifest.json`, status QUARANTINED, failed_gates=[G1] only).

**Decision:** split gate G1 into G1-N (NONRAD, Born-calibrated, UNCHANGED) and G1-R
(RAD, radiation-calibrated, thresholds below), add one new hard one-sidedness check
(R6) and record-level radiative-tail counters. Production is UNBLOCKED under this
design. Verdict: **GATE_APPROVED_FOR_PRODUCTION** (conditional on implementing the
G1-N/G1-R split with exactly the thresholds in Section 3).

---

## 1. Diagnosis: CONFIRMED

The RAD G1 failures are genuine QED radiation smearing the electron-method kinematics
(`Q2_es_gen`/`Y_es_gen`/`x_es_gen`, built from the post-radiation scattered electron
with ISR-subtracted beam energy, `RC_treebuilder_genonly_v3.C` line 82) relative to the
true generated kinematics (`Q2sGen`/`YsGen`/`XsGen`). They are NOT a v1-style
wrong-formula defect, and the Born-calibrated G1 thresholds (purity>=99.5%,
completeness>=99.5%, wrong-cell<=0.5%, corr>=0.999) are structurally inapplicable to a
RAD sample: they encode the identity "ES == true", which holds only without radiation.

Evidence, in decreasing order of strength:

1. **The formula is verified correct where an independent standard exists.** Both
   NONRAD samples pass G1 through the same builder code path with
   corr = 0.99999992-0.99999999, purity 99.997-100%, completeness 99.982-99.989%,
   wrong-cell 0.001-0.004%, and zero flag-consistency mismatches in all four samples.
   A wrong-sign formula is generator-independent and would fail NONRAD identically.
2. **The RAD deviations are exactly the radiation-predicted size, and are REQUIRED by
   the FINAL C_rad** (closure, Section 2): row-level ES-selection retention
   S/T = completeness/purity = 0.9439-0.9493, together with the published
   C_rad(EMPZ 35-70) ~= 1.1127, implies a true-level radiative depletion of
   4.78-4.88% — the standard HERA ISR soft+virtual+collinear radiator scale
   (alpha/2pi)*ln(Q2/m_e2) ~= 0.023-0.026 at Q2 = 150-2000 GeV2, times an O(2)
   logarithmic structure factor. Had the RAD sample passed the Born-calibrated
   completeness threshold (>=99.5%), the published C_rad would require a ~9.7%
   true-level depletion — about twice the ISR scale, i.e. physically inconsistent.
   The ~6% completeness loss is therefore not merely acceptable: it is necessary for
   C_rad = 1.11-1.14 to hold.
3. **The radiative tail is independently measured in our own artifacts.**
   `data/radiative/genenphoton_radiation_fraction_2026-09-29/summary.json`:
   39.4% of DJ RAD events carry GenEnPhoton>0 (26.2% >0.5 GeV, 20.2% >1 GeV, 8.0%
   >2 GeV unweighted), vs GenEnPhoton identically 0 in all NONRAD sets; inside the
   ES fiducial the photon fraction is still 10.9% (>0), 5.5% (>1 GeV), 1.25%
   (>2 GeV). `data/radiative/crad_empz_scan_20260925/agg_sums.json`: NONRAD sumw is
   bit-identical at every EMPZ threshold 0-50 GeV (retention 1.000000), while RAD
   loses 1.10% (ep h+, weighted) at EMPZ>35, 3.03% at >45, 5.59% at >50 — a pure
   RAD-side E-pz tail. The strict theta>175deg HepMC diagnostic found photons in
   23.29% of RAD vs 0.76% of NONRAD events.
4. **Two independent generator stacks fail identically.** DJANGOH14 and RAPGAP31 RAD
   differ in completeness by 0.45-0.51 pts, wrong-cell by 0.10 pts, corrY by 0.0005,
   with the same sign and structure; the direction of the small DJ/RG difference
   matches the small C_rad difference (RG 1.1329 vs DJ 1.1376: less radiative loss ->
   higher retention). A shared builder defect is excluded by point 1; a per-generator
   defect is excluded by the agreement.

v1-defect reference (audit `data/radiative/v1_kincut_audit_2026-10-02/results/`):
purity 72.21-72.53%, completeness 9.16-9.82%, wrong-cell 63.06-63.28%,
corrQ2 0.01713, corrY 0.01714, raw x-corr -0.3630. The RAD smoke values are 1-2
orders of magnitude away from the defect on every witness (Section 3 table).

## 2. Quantitative closure (the numeric core of the confirmation)

Retention S/T = completeness/purity (row-level, smoke 5323776). T-ratio = true-level
RAD/NONRAD fiducial rate implied by (1/C_rad)/S/T.

| sample-charge | purity % | completeness % | S/T | C_rad (35-70) | 1/C_rad | implied T | depletion |
|---|---|---|---|---|---|---|---|
| DJ RAD h+ | 99.627 | 94.036 | 0.94388 | 1.1127 | 0.89871 | 0.95215 | -4.78% |
| DJ RAD h- | 99.625 | 94.133 | 0.94487 | 1.1127 | 0.89871 | 0.95116 | -4.88% |
| RG RAD h+ | 99.583 | 94.487 | 0.94883 | ~1.1080 (est.) | 0.90253 | 0.95121 | -4.88% |
| RG RAD h- | 99.595 | 94.548 | 0.94932 | ~1.1070 (est.) | 0.90333 | 0.95153 | -4.85% |

C_rad references: FINAL (EMPZ>45) DJ 1.13760/1.13667, RG 1.13289/1.13191
(`CRAD_FINAL_PROVENANCE_2026-09-30.md`); DJ EMPZ 35-70 scan value 1.1127 (spec item 8);
RG 35-70 estimated by mirroring the DJ EMPZ-window shift (-0.0249), RG value
unpublished. Caveats: row-level retention proxies the weight-level retention used in
C_rad (agreement of the four T values at the 0.1-0.2% level is the meaningful result);
smoke is 1 file per sample (per-file statistical precision <0.1 pt on ~3x10^5 rows).

The four implied depletions agree at 0.10 pts with no free parameters. Wrong-cell
2.80-2.91% is consistent with the same smearing against the published cell boundaries
(Q2 edges 150/300/700/1500/20000): v1 defect wrong-cell is 63%, 22x higher.

## 3. Approved gate design: (a) split-threshold G1-N/G1-R, plus new R6

Option (b) is REFUTED on branch-list evidence: the 84-branch v3 `ChargedGenRC` schema
(verified in `genenphoton_radiation_fraction_2026-09-29/summary.json` schema block)
contains no Born-level pre-FSR electron-method kinematics. Every electron-method
family is post-radiation by construction: `Q2_es_gen` (post-radiation electron,
ISR-subtracted beam), `Q2esGen` (framework eSigma, also post-radiation; it is the
C_rad binning variable, not an independent witness), `*_v1bug` (forbidden),
`*_v3` (identical to `Q2_es_gen` when GenRad==1, i.e. on RAD). The `GenElecUnc*`
branches are the bare post-shower electron, not pre-radiation. The only Born-level
truth is `Q2sGen`/`YsGen`/`XsGen` — already the G1 witness. Option (c) is adopted at
record level only (quantiles, no hard shape gate: no independent reference quantiles
exist, and thresholds capture the discrimination). Option (d) is retained through the
existing G4 anchor and new record-level tail counters below.

**G1-N (NONRAD samples: `djangoh14_nonrad`, `rapgap31_nonrad`) — UNCHANGED:**
purity >= 99.5%; completeness >= 99.5%; wrong-cell <= 0.5%; corr(Q2_es,Q2s) >= 0.999
AND corr(Y_es,Ys) >= 0.999 AND corr(log10 x_es, log10 Xs) >= 0.999; flag-consistency
100%. This remains the maximally sensitive anti-v1 gate because ES == true holds
exactly on NONRAD (measured corr 0.99999992+).

**G1-R (RAD samples: `djangoh14_rad`, `rapgap31_rad`), per sample and charge:**

| # | witness | hard threshold | smoke value | v1 defect | defect separation |
|---|---|---|---|---|---|
| R1 | purity | >= 99.0% | 99.583-99.627 | 72.21-72.53% | 27+ pts |
| R2 | completeness | in [90.0%, 97.5%] | 94.036-94.548 | 9.16-9.82% | ~80 pts below floor |
| R3 | wrong-cell fraction | <= 5.0% | 2.800-2.907 | 63.06-63.28% | 12.6x |
| R4 | corr(Q2_es,Q2s), corr(Y_es,Ys), corr(log10 x_es,log10 Xs) | each >= 0.98 | 0.98786-0.99470 | 0.01713-0.01714 | 57x |
| R5 | flag-consistency | 100% | 0 mismatches x4 | — | — |
| R6 (NEW) | one-sidedness over true-fiducial rows (Q2sGen>=150 & 0.1<YsGen<0.7 & hadron-level conditions): frac(Q2_es_gen > 1.05*Q2sGen) AND frac(Y_es_gen < YsGen-0.05) | each <= 5.0% (warn at 2.0%) | not yet measured; radiatively expected <~1-2% | ~30-50% (symmetric scatter) | >= 6x |

Rationale for R2's two sides: the floor excludes any formula corruption (defect
9-10%); the ceiling enforces that the RAD sample is actually radiative — a NONRAD
file set fed as RAD gives completeness 99.98-99.99% and fails the ceiling by >2.4 pts
(earlier and more local than G4's aggregate R_int catch). The ceiling is also required
by physics: retention >0.979 would force the true-level depletion below 8.2%, in
tension with the ISR radiator scale and the 0.41% DJ/RG C_rad agreement.

Rationale for R6: FSR degrades the scattered-electron energy, pushing Q2_es DOWN and
Y_es UP; ISR subtraction acts in the same directions. The "forbidden" sides
(Q2_es > 1.05*Q2s; Y_es < Ys - 0.05) are radiatively suppressed (populated only by
wide-angle/multi-photon radiation not captured by GenEnPhoton and by combiner
effects); any symmetric formula corruption puts O(half the sample) there. This is the
one-sided version of option (c) rendered as a threshold, and it is the only new
witness not already dominated by R1-R5. Thresholds are set ~3-5x above the radiative
expectation and >= 6x below the defect level; they are to be measured for the first
time in the production pass (recorded, warn at 2.0%, hard at 5.0%).

All G1-R metrics remain computed exactly as G1 does today (against
`Q2sGen`/`YsGen`/`XsGen`, same masks); only the thresholds are sample-keyed. The
plotter must stamp the G1-R thresholds and this document's path into `manifest.json`.

## 4. Observable decision (item 3): keep the electron-method selection for BOTH samples

Both RAD and NONRAD must be selected on `Q2_es_gen`/`Y_es_gen`/`x_es_gen` (plus
`pass_truth_event_fiducial`, `pass_truth_hadron_fiducial`, `35<=GenEmpz<=70`) — the
frozen flag/ES convention — NOT on `Q2sGen`/`YsGen`. Reasons:

1. **C_rad consistency.** The FINAL C_rad selected BOTH samples on the ES family
   (`crad_v3_worker.py` lines 88-90: fiducial on `Q2_es_gen`/`Y_es_gen`/`x_es_gen`,
   EMPZ on `GenEmpz`) and binned on framework eSigma. Under the ES selection the
   per-panel integral ratios are the phi_h projection of 1/C_rad under the plot
   fiducial (expected R_int ~ 0.899-0.903), keeping gate G4 and the published per-bin
   C_rad CSV cross-reference valid. Under a Born (`Q2sGen`) selection the integral
   ratio would instead approximate the TRUE-level radiator ratio (~0.95 implied),
   which is NOT C_rad; G4 would need full recalibration and the product would no
   longer be the phi_h projection of the published correction.
2. **Measured-space consistency.** The central v6 analysis selects and bins in the
   electron method, and this fork computes phi_h/pT_h in the gamma*p CM frame built
   from the post-radiation electron with the canonical v6 boost recipe (G7). The
   product therefore shows the radiative distortion of the observable the analysis
   actually measures — the quantity a future C_rad-like phi_h correction would need.
3. **A Born-selected comparison is not implementable on this basis.** The stored and
   recomputed phi_h, pT_h, zGen all use q = k_in(nominal) - k_out(post-radiation
   electron). Selecting on `Q2sGen`/`YsGen` while keeping this frame would compare
   Born-binned shapes still defined by the radiative electron — an inconsistent
   hybrid. A true fixed-Born-kinematics comparison requires rebuilding the Trento
   frame from vertex kinematics (a `sigma_photon`-style q from `Q2sGen`/`YsGen`, cf.
   the C_rad worker's q-variant machinery) — a separate builder/plotter variant,
   out of scope for this unblock and not required by the product's purpose.

**Consequence for the ratio interpretation (record in the manifest):** under the ES
selection the RAD and NONRAD curves in a given (Q2_es,x_es) cell correspond to
slightly different true-kinematics mixtures; R(phi)-1 therefore contains the
bin-migration (kinematic) component of the radiative correction in addition to any
intrinsic azimuthal dynamics. This is intended: it is the correction in measured
space, and its panel integral is anchored by C_rad through G4. It must NOT be quoted
as "QED azimuthal dynamics at fixed true kinematics". If that question is wanted
later, it requires the separate vertex-frame, Born-selected variant of point 3.

## 5. Additional RAD-specific gates and records (item 4)

1. **R6 one-sidedness** (hard, Section 3) — the primary new RAD-specific gate.
2. **R2 completeness ceiling 97.5%** (hard, Section 3) — "is the RAD sample actually
   radiative" / wrong-sample-feed catch, complementing G4's aggregate R_int anchor.
3. **Record-level (warn, not hard), per RAD sample and charge, into the manifest:**
   - `GenRad` distribution: frac(GenRad==1), frac(GenRad_valid==0). Do not hard-gate:
     event-level GenRad semantics are not established as a fraction expectation.
   - `GenEnPhoton` tail among selected rows: frac(E_gamma>1 GeV), frac(E_gamma>2 GeV)
     (2026-09-29 study expectation ~5.5% / ~1.25% unweighted inside the ES fiducial).
   - Implied true-level depletion T = R_int * purity / completeness per
     generator-charge, expected band [0.93, 0.97] (smoke-implied 0.951-0.952);
     outside the band -> investigate (this documents the Section 2 closure every run).
   - Migration quantiles over true-fiducial rows: median, p05, p95 of
     (Q2_es-Q2s)/Q2s and (Y_es-Ys) — the option-(c) shape evidence; seeds future
     tightening of R6.
4. The specced Q2Gki domain-leak counter (expected ~0.09% of RAD rows, warn-level)
   stays unchanged.

## 6. Verdict

G1 as written (Born-calibrated thresholds applied to RAD) is REJECTED as a gate
design — it is physically unsatisfiable for any genuine RAD sample consistent with
the FINAL C_rad. The amended design (G1-N unchanged + G1-R per Section 3 + R6 +
Section 5 records) is approved.

**GATE_APPROVED_FOR_PRODUCTION** — conditional on implementing the G1-N/G1-R split
with exactly the Section 3 thresholds and recording this document in the run manifest;
G0/G2/G3/G5/G6/G7 already pass in smoke, G4 evaluates on the production basis with its
specced bands unchanged.
