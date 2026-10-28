# G4 R_int Expectation Recalibration — Decision Record (2026-10-04)

**Author role:** h1-radiative (radiative-correction specialist)
**Product:** `plots/phi_h/rad_vs_nonrad_v3_cm_2026-10-03/` (CM-frame RAD-vs-NONRAD phi_h,
cluster 5323782 90/90, finalize 5323786 PASS, worker sha256
`d379625c28d5e86af78ebfbdf05c6e0c419730c7c3de67cfc4ee8752cc74e7f4`)
**Question put to this role:** final gating decision on G4 — DJANGOH integrated-ratio
anchor R_int = 0.893823/0.894107 falls below the spec's "0.899–0.903" figures while
RAPGAP (0.899539/0.899886) sits inside them.
**Decision:** APPROVED WITH A DOCUMENTED CAVEAT. **No gate threshold is changed.**
The 0.899–0.903 figures were never the G4 gate; they are the spec's expected-value
annotation and are hereby recalibrated (superseded) as recorded below.

---

## 1. What G4 actually gates (and what it does not)

Spec `spec_nonrad_rad_phih_v3_2026-10-02.md` (G4 definition, lines 426–445) and the
finalize manifest G4 block (lines 16804–16930) implement exactly:

- **Absolute anchor:** R_int per generator-charge inside `[0.96, 1.06] × 1/C_rad_FINAL(>45)`
  with C_rad_FINAL from `CRAD_FINAL_PROVENANCE_2026-09-30.md`:
  DJ 1.1375979129031535 / 1.1366708715220577; RG 1.1328903059596749 / 1.1319063751575906.
  Implemented bands: DJ h+ [0.843883, 0.931788], DJ h− [0.844572, 0.932548],
  RG h+ [0.847390, 0.935660], RG h− [0.848127, 0.936473].
- **Cross-generator band [0.99, 1.01]:** measured RG/DJ = 1.0063948529431672 / 1.0064638253294644.
- **Cross-charge band [0.99, 1.01]:** measured plus/minus = 0.9996819774883412 (DJ) / 0.9996134698581705 (RG).
- **Status: PASS** (`failed_gates: []`, manifest line 6). Measured multipliers
  R_int × C_rad_FINAL(>45) = 1.016811 / 1.016305 / 1.019079 / 1.018587 — all comfortably in [0.96, 1.06].

The **0.899–0.903 figures are the spec's expected-value annotation**:
`E[R_int] ≈ 1.022 × 1/C_rad_FINAL(>45)`, where 1.022 is the EMPZ window-shift ratio
C_rad(>45)/C_rad(35–70) = 1.022149 (chain `empz_35_70` nominal:
DJ 1.1129471463113847 / 1.1123662584135976; the "~1.1127" in the spec is the charge mean
of the C_rad chain's own `empz_35_70` variant, `data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_summary.json`).
Reproduced per curve: E[DJ] = 0.898384/0.899117, E[RG] = 0.902117/0.902902 → the quoted
"0.899–0.903" spans DJ-low to RG-high. The RG expectation anchor mirrors the DJ window
ratio (1.1082395/1.1076018).

**Measured gaps vs the annotation:** DJ −0.5223% (h+) / −0.5425% (h−);
RG −0.3096% (h+) / −0.3284% (h−).

## 2. Frame consistency (item 1)

R_int is the phi_h-**integrated** RAD/NONRAD ratio, so the LAB→CM frame change cannot
move it: any per-bin migration cancels in the integral. Frame-coupled exclusions are
exactly zero rows (`cm_nonfinite_excluded: 0`, `phi_sentinel_excluded: 0`; manifest
`phi_cm_definition` and selection string, lines 136–138). The annotation predates the
frame change and is frame-blind (it is built from C_rad values, which are phi-integrated
by construction). **The band was frame-consistent — frame-invariant by construction.**

## 3. Gap decomposition (item 2) — no listed candidate closes it; two unlisted terms do

Basis definitions used below (all verified by direct reads this session):
- **Chain reference basis** (`crad_v3_worker.py`, lines 86–101): finite weight →
  RAD-only `Q2Gki>100` domain (default on) → ES-gen DIS fiducial
  (`Q2_es_gen∈[150,20000)`, `Y_es_gen∈(0.1,0.7)`) → GenEmpz mode → hadron provenance
  (|PDG|∈{211,321,2212}, IsGtr, Mother2=−1, v3 Veto veto) → 490-bin grid.
  **No truth-kinematics event cut; no lab-eta cut** (`truth_eta_cut: False`, `xf_cut: False`).
- **Product basis** (manifest selection string, line 136):
  `pass_truth_event_fiducial==1 & pass_truth_hadron_fiducial==1` & ES-gen fiducial &
  `35<=GenEmpz<=70` & finite CM observables; **no Q2Gki cut** ("recorded diagnostic only").
- **Direct-sum scan basis** (`scripts/radiative/crad_empz_scan_20260925/stage1.py`):
  same trees/files/lumis, ES-gen fiducial, `xFGen<0` + z∈[0,1] + pT∈[0,8],
  GenEmpz>T threshold, **no Q2Gki, no truth cut**, v2 Veto branch only.

| candidate (as listed) | effect on product R_int vs chain-derived expectation | verdict |
|---|---|---|
| EMPZ window (35–70 vs >45) | **0.000%** — the annotation already used the 35–70 window (ratio 1.022149). NONRAD side of every EMPZ cut is vacuous (scan per-file NONRAD decomposition identically zero: no NONRAD row has GenEmpz≤45; `nonrad_retained = 1.0000` at all thresholds). Upper edge ≤70 vacuous: chain `empz_45_70` ≡ `empz_gt45` to 2.5e-8 (1.1375978877640025 vs 1.1375979129031535). So product 35–70 ≡ chain 35–70 ≡ scan T=35 in EMPZ semantics. | does not contribute |
| Q2Gki>100 domain cut | **+0.090% DJ / ~0.000% RG** — WRONG SIGN. The chain applies it to the RAD numerator (provenance line 60: in-fiducial effect ~0.09% of RAD events); the product does not. Keeping those rows predicts the product ABOVE the chain-derived expectation, widening the gap to ~−0.61%. | does not close; wrong way |
| OLD/NEW NONRAD pooling | **0.000%** — the product's normalization pools OLD+NEW with the identical oomclumi exposures as the chain (norm verification 2026-10-02: 1373 RAD = 731 ep + 642 em; 520 OLD + 1060 NEW; scales = 1/L; COMBINE matched to chain). The addendum's ±0.007–0.034% figures are vs OLD-only, a basis choice both sides share here. | does not contribute |
| Beam mixture (e±p/e∓p) | **0.000%** — the product consumes the chain's own per-beam w/L mixture factors (ep fraction 0.5141/0.5449-family as recorded); composition identical by construction. | does not contribute |
| ES-boundary migration | **0.000%** — both bases use the same ES-gen boundaries and the frozen flag convention (G1-gated); no reco-level boundary enters this gen-only product. | does not contribute |
| LAB→CM frame change | **0.000%** — item 1. | does not contribute |
| **Truth-event fiducial (unlisted; product-only)** | **−0.3623% / −0.3645% DJ; −0.3736% / −0.3761% RG** (net of the ~0.002% NONRAD removal). Measured from the finalize purity numbers: RAD 99.6361/99.6338/99.6261/99.6236%, NONRAD 99.99834/99.99831/99.99974/99.99974%. Confirmed product-only by the worker read (chain has no truth-kinematics event cut). The product's `pass_truth_hadron_fiducial` (lab-eta class, absent from the chain, `truth_eta_cut: False`) belongs to the same product-only selection family and is absorbed in the residual below. | **dominant identified term** |
| **Reference-side residual (unlisted)** | **−0.250% / −0.268% DJ; +0.064% / +0.048% RG.** Attributed to the C_rad chain reference's own sub-percent selection/aggregation-coverage sensitivity (evidence in §4). | closes the remainder |

**Closure check (fractional shifts on R_int):**
- DJ h+: −0.5223% = +0.090% (Q2Gki) − 0.3623% (truth) − 0.2500% (residual)
- DJ h−: −0.5425% = +0.090% − 0.3645% − 0.2680%
- RG h+: −0.3096% = 0.000% − 0.3736% + 0.0640%
- RG h−: −0.3284% = 0.000% − 0.3761% + 0.0477%

Note the RG curves are fully reconciled by the truth-cut term alone (residual at the
per-mille level); only DJ carries a ~−0.25% reference-side residual, which is the same
curve whose chain reference is demonstrably the low outlier (§4).

## 4. Evidence that the residual is reference-side

Three independent computations over the **same 1373 RAD + 1580 pooled NONRAD files with
the same oomclumi exposures** (DJ h+/h−):

| computation | C_rad(35–70 equivalent) | vs product implied C |
|---|---|---|
| product implied (1/R_int) | 1.118790 / 1.118434 | — |
| direct-sum scan T=35 (`scan_results.csv`) | 1.118301 / 1.117954 | −0.0437% / −0.0430% |
| chain `empz_35_70` aggregated (`crad_v3_summary.json`) | 1.112947 / 1.112366 | −0.5201% / −0.5425% |

- The **chain reference is the low outlier**: it sits 0.4788%/0.4998% below the scan at
  matched EMPZ semantics, and 0.545% below it at empz-none (chain 1.099952/1.099904 vs
  scan 1.105969/1.105814).
- The chain-vs-scan difference is **not pure aggregation**: the scan's hadron-level
  selection differs (`xFGen<0` discards ~62% of low-Empz RAD weight — per-file
  decomposition rad_0: 18138.9 removed of 29427.2 — plus no Q2Gki and the v2 Veto
  branch), and the difference is EMPZ-mode-dependent (−0.545% none → −0.479% T=35 →
  −0.272% T=45), the signature of a selection×EMPZ interaction rather than a constant
  aggregation bias. Sub-percent C_rad sensitivity to hadron-level selection is therefore
  **empirically real** in this sample set, and the chain-derived expectation annotation
  inherits it.
- The product's raw per-mille agreement with the scan (+0.044%) is **partially a
  cancellation**: the product's truth cut depresses its R_int by ~0.36%, the scan's
  xF<0 cut depresses the scan's R_int by an inferred ~0.3% (from the residual after the
  measured truth term; not independently measurable from the scan artifacts because the
  scan's NONRAD decomposition is empty by design). Honest reading: after known selection
  differences, product, scan, and chain agree within a few tenths of a percent, with the
  chain low. A −0.25% reference-side residual sits comfortably inside this demonstrated
  scale and is **not evidence of a product defect**.

## 5. DJ-vs-RG separation (item 3) — not a red flag

- The measured same-basis DJ/RG R_int separation is **0.640% / 0.646%**
  (manifest cross_generator 1.0063948529431672 / 1.0064638253294644) — not 0.57%;
  0.57–0.58% was the DJ-vs-annotation-edge distance.
- The published cross-generator C_rad agreement at EMPZ>45 is **0.416% / 0.421%**
  (chain gt45: DJ 1.1375979/1.1366709 vs RG 1.1328903/1.1319064).
- Same sign (DJ below RG), amplified ~×1.5 in the 35–70 window, which transmits the ISR
  E−pz radiative tail — exactly the regime where DJANGOH and RAPGAP parton-shower/ISR
  modeling differs most. The growth is expected, monotone with the removal of the
  EMPZ>45 shield, and passes the cross-generator band [0.99, 1.01] with margin.
- **Disposition: carry 0.64% as the generator systematic** (as already planned for the
  generator-difference budget); it does not invalidate either generator's internal
  consistency and is not a gating red flag.

## 6. Correct G4 band (item 4) and superseded annotation

- **The implemented spec band is the correct band and is unchanged**:
  [0.96, 1.06] × 1/C_rad_FINAL(>45) per generator-charge (absolute numbers in §1),
  cross-generator [0.99, 1.01], cross-charge [0.99, 1.01].
  Derivation of why it is right: G4 is a gross-consistency anchor at the −4/+6% level,
  deliberately wider than the intentional basis differences it must absorb — the EMPZ
  window shift (+2.21%), the product-only truth-selection terms (±0.4%), and the
  reference-side sub-percent sensitivity (±0.5%, §4) — which sum to ~±3.3%. The measured
  multipliers 1.0163–1.0191 sit inside with the expected ~1.02 structure intact.
- **Superseded expectation annotation:** the spec's "~1.022 × 1/C_rad → 0.899–0.903"
  becomes **DJ ≈ 0.894 / RG ≈ 0.900** (multiplier ≈ 1.017–1.019) once the product's
  truth-event fiducial (−0.36/−0.37%) and the reference-side offset (−0.25% DJ) are
  accounted. The finalize's own `implied_true_level_depletion` diagnostic
  (R_int ÷ the RAD ES-selection efficiency implied by purity/completeness) gives
  0.946180/0.945825/0.947218/0.946893 — all `in_band: true` in its [0.93, 0.97] warn
  band, an independent internal confirmation.

## 7. Verdict and caveat sentence

**Verdict: APPROVED WITH A DOCUMENTED CAVEAT.** G4 PASS stands; no threshold changes;
the product remains INTERNAL DIAGNOSTIC class (c_rad_applied=false); the 0.64% DJ/RG
separation is carried as the generator systematic.

**Caveat sentence (verbatim, for quotation):**

> G4 passes its specced [0.96, 1.06]×1/C_rad_FINAL(>45) bands with the spec's
> 0.899–0.903 expectation annotation superseded by the measured DJ 0.8938/0.8941 and
> RG 0.8995/0.8999, the DJ shortfall being fully decomposed into the product-only
> truth-event fiducial (−0.36%) and a −0.25% chain-reference-side offset — with the
> EMPZ-window, pooling, beam-mixture, ES-boundary, and frame terms all exactly 0.000%
> and the Q2Gki term of the wrong sign (+0.09%) — and the 0.64% DJ/RG R_int separation
> to be carried as the generator systematic.

## 8. Provenance of every number used (reads only; no reruns)

- Manifest `plots/phi_h/rad_vs_nonrad_v3_cm_2026-10-03/manifest.json`: line 6
  (`failed_gates: []`); line 136 (selection string, truth fiducials, no Q2Gki,
  35<=GenEmpz<=70); line 138 (phi_cm_definition); lines 16293–16342
  (implied_true_level_depletion, purity/completeness per curve); lines 16804–16930
  (G4 reference, bands, per-curve values, status PASS); lines 16021/16089/16157/16225
  (RAD purity 99.6361/99.6338/99.6261/99.6236) and 15870/15895/15920/15945 (NONRAD
  purity 99.99834/99.99831/99.99974/99.99974).
- Spec `docs/radiative/spec_nonrad_rad_phih_v3_2026-10-02.md` lines 426–445 (G4 definition
  and the 0.899–0.903 expected-value annotation).
- Chain worker `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py`
  lines 83, 86–101, 108, 111–112 (selection family; RAD-only Q2Gki default on;
  no truth event cut; `truth_eta_cut: False`, `xf_cut: False`).
- Chain values `data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_summary.json`
  (empz_gt45, empz_45_70, empz_35_70, empz_none integrated C_rad by charge; oomclumi
  exposures identical to the product's normalization).
- Scan `data/radiative/crad_empz_scan_20260925/`: `scan_results.csv` (T=35
  1.1183013244554414/1.1179537189590913; none 1.1059693429774702/1.1058138439343579;
  nonrad_retained ≡ 1.0), `agg.py` (oomclumi E dict identical to chain), per-file
  decompositions (NONRAD identically zero; RAD xF removal 18138.9/29427.2 on rad_0),
  worker `scripts/radiative/crad_empz_scan_20260925/stage1.py` (selection family,
  xFGen<0, no Q2Gki, per-mode GenEmpz threshold).
- Provenance `docs/radiative/CRAD_FINAL_PROVENANCE_2026-09-30.md` line 60 (numerator-only
  Q2Gki>100 on RAD, in-fiducial ~0.09%).
- Normalization `data/radiative/nonrad_rad_phih_norm_2026-10-02/nonrad_rad_phih_norm_verification_2026-10-02.json`
  (scales = 1/L, COMBINE pooling, per-beam mixture factors, verified file counts).
- Prior records `docs/radiative/g1_rad_gate_recalibration_decision_2026-10-04.md` and
  `docs/radiative/g1_r6y_recalibration_addendum_2026-10-04.md` (G1 split context;
  their "scan" citations refer to the chain's EMPZ-mode values, not the 2026-09-25
  direct-sum scan distinguished here).

**Refinement note vs prior session wording:** the 2026-09-25 direct-sum scan is an
independent basis with a *different* hadron-level selection (xFGen<0, no Q2Gki), not a
like-for-like replication of the chain; the chain-vs-scan 0.48–0.55% differences
therefore demonstrate reference-side selection/aggregation-coverage sensitivity rather
than a pure aggregation bias. This refinement strengthens (not weakens) the conclusion:
the −0.25% DJ residual is inside the demonstrated sensitivity scale, and the product is
consistent with the independent direct-sum basis after the measured selection terms.
