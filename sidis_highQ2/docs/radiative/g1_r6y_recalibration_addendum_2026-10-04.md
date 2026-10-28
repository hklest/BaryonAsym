# G1-R R6 one-sidedness gate: first production-basis measurement and R6-y recalibration — addendum (2026-10-04)

**Author role:** `h1-radiative` (radiative-correction specialist). This is the follow-up to
`g1_rad_gate_recalibration_decision_2026-10-04.md` and, together with it, constitutes the
manifest-recorded reason required by `spec_nonrad_rad_phih_v3_2026-10-02.md` item 8
("Gate thresholds may be changed only by `h1-normalization`/`h1-radiative` with a
manifest-recorded reason"). No builder code was changed; the plotter gate-eval section must
be updated to the Section 4 thresholds before the production re-smoke.

**Basis of this addendum:**
- Production basis: 90 shard partials of cluster 5323782 + finalize 5323784,
  `plots/phi_h/rad_vs_nonrad_v3_cm_2026-10-03/` (`manifest.json`, status QUARANTINED,
  failed_gates=[G1] under the ORIGINAL R6-y; 945,730,605 true-fiducial RAD rows).
- Re-smoke: cluster 5323780, `smoke_1file/manifest.json`.
- Plotter `plot_phi_rad_nonrad_gen_v3_compact.py`, sha256
  `fac9a06f9ebeab1289ac05710d9bee8387ad64021e9031524fc97851be7da330` (identical in smoke,
  production, and finalize records).
- Single-file row-level diagnostics: `rad_ep_0000.root` (65,598 events / 2,816,908
  `ChargedGenRC` rows), uproot, scripts `/tmp/opencode/crad_r6_diag*.py`.
- Builder `RC_treebuilder_genonly_v3.C` lines 78/82/84/85.

**Decision in one line:** the R6-y failure is a THRESHOLD_MISCALIBRATION of my own gate
(the expected side and rate were wrong for this builder's ES convention), not a defect and
not a wrong formula; R6 is recalibrated to a 4-check set (Section 4) that the full
production basis already passes 4/4; verdict **PROCEED_AFTER_RESMOKE** (Section 9).

---

## 1. First production-basis measurement of R6

Per RAD sample-charge, over true-fiducial rows (Q2sGen>=150 & 0.1<YsGen<0.7 & IsGtr==1 &
Mother2==-1 & |GenEta_lab|<4 & 0<=zGen<=1 & xFGen<0), tolerance 0.05, original thresholds:

| witness | original threshold | re-smoke 5323780 (1 file/sample-charge) | production 5323782 (90 shards) | verdict |
|---|---|---|---|---|
| frac(Q2_es_gen > 1.05*Q2sGen) (r6_q2_up) | <= 5.0% hard, warn > 2.0% | 0.0000% (all 4) | 0.0000% (all 4; 0 rows) | PASS |
| frac(Y_es_gen < YsGen-0.05) (r6_y_down) | <= 5.0% hard, warn > 2.0% | DJ h+ 7.7537%, DJ h- 7.6832%, RG h+ 7.6549%, RG h- 7.5867% | DJ h+ 7.4841%, DJ h- 7.4477%, RG h+ 7.3575%, RG h- 7.3141% | **FAIL** (only failure; G1 -> QUARANTINED) |

Production denominators (true-fiducial rows): DJ h+ 232,260,497; DJ h- 228,664,182;
RG h+ 244,686,289; RG h- 240,119,637; total 945,730,605. R1-R5 all pass on both bases
(re-smoke: R1 99.583-99.627, R2 94.036-94.548, R3 2.800-2.907, R4 0.98786-0.99470,
R5 zero mismatches). The Q2 side passes at exactly 0.0000% and the Y side fails at
7.3-7.8% — Section 2 shows these are the same physics.

## 2. Sign physics re-derivation (item 1): both sides are one-sided DOWN, by construction

My original Section 3 rationale assumed the naive electron method (bare post-FSR electron
energy against the nominal beam), under which "FSR degrades the scattered-electron energy,
pushing Q2_es DOWN and Y_es UP". **That assumption is false for this builder.** The frozen
ES convention (`RC_treebuilder_genonly_v3.C`):

- line 78: `eeOld = GenBeamElecE - max(0, GenEnPhoton)` — the photon energy is subtracted
  from the **beam**, unconditionally (no GenRad condition);
- line 82: `Y_es_gen = 1 - (GenElecE/eeOld)*sin^2(theta/2)`,
  `Q2_es_gen = 2*eeOld*GenElecE*(1+cos(theta))`.

Three row-level facts measured on `rad_ep_0000.root` fix the semantics:

1. `GenElecE >= GenElecUncE` always; on `GenRad==0 & GenEnPhoton>0` rows (FSR),
   `GenElecE - GenElecUncE == GenEnPhoton` exactly (correlation 1.000). The H1OO electron
   combiner **merges the FSR photon back into `GenElecE`** (combined electron): the
   electron energy is restored to its pre-FSR value.
2. `GenRad==1` iff ISR (`GenElecE == GenElecUncE` on 100% of such rows; never both).
3. Consequently, for FSR the photon is subtracted from the **wrong side** (the beam did not
   lose it; the outgoing electron did, and the combiner already gave the energy back), while
   for ISR the subtraction is exactly right (the beam really did lose `GenEnPhoton` before
   the scattering). The builder itself documents the intended per-side semantics: the
   `_v3` variant (line 85) subtracts `GenEnPhoton` from the beam **only when `GenRad==1`**;
   the frozen line-78/82 ES convention — shared by the RAD/NONRAD selections, the C_rad
   worker, and the central-v6 eSigma algebra — subtracts it unconditionally.

The exact row-level law that follows (and is measured, med |dy - predicted| = 0.0000,
p90 = 0.0006 over all FSR rows):

```
FSR:  Q2_es = Q2s * (1 - Eg/E0)                      -> dq2 = -Eg/E0 < 0
      Y_es = 1 - (1-Ys)/(1-Eg/E0)                    -> dy = -(1-Ys)*Eg/(E0-Eg) < 0
ISR:  ES == Born exactly (beam correctly reduced)    -> dy = dq2 = 0
Eg==0: ES == Born                                    -> dy = dq2 = 0
```

with (1-Ys) median 0.689 among FSR rows, so any FSR photon with Eg >~ 1.87 GeV
(= 0.05*(E0-Eg)/(1-Ys) at the median) drives dy beyond -0.05; measured FSR rows with
Eg>2 GeV: 40.4%, measured FSR rows beyond dy=-0.05: 40.2%.

**Therefore both Q2_es and Y_es are one-sided DOWN, and the up sides are structurally
empty.** Measured on the full 945,730,605-row production basis (all four sample-charges):
dy > +0.05: **0 rows**; dy > +0.02: **0 rows**; dq2 > +0.05: **0 rows**; dy overflow 0;
max dy on the 2.8M-row file +0.00023. The Q2-side PASS at exactly 0.0000% and the Y-side
FAIL at 7.3-7.8% are one shared mechanism (wrong-side beam subtraction of the FSR photon)
projected onto two variables — a wrong-formula defect would instead break the structure on
both sides (nonzero up-side; see Section 3).

Answering the two posed sub-cases directly: a **missed scattered photon** plays no role —
every row with dy < -0.05 has GenEnPhoton>0, Eg==0 rows are clean to 1e-5, and the
combiner captures essentially all electron-adjacent FSR (fact 1); **hard ISR is exactly
compensated** (fact 3, ES==Born on ISR rows; only 0.78% of ISR rows drift beyond -0.05,
from sub-threshold soft radiation). The entire Y-side tail is merged-FSR + wrong-side beam
subtraction.

## 3. Closure and satisfiability (item 2): 7.3-7.8% is the expected value

Intended-row composition on the file: 63.2% Eg==0, 17.5% ISR, 19.3% FSR. Fraction with
dy < -0.05 per class: Eg==0 -> 0; ISR -> 0.78%; FSR -> 40.2%. Closure:

```
0.632*0% + 0.175*0.78% + 0.193*40.2% = 7.90%   (measured on the file: 7.841-7.891%)
```

The production value (7.314-7.484%) and re-smoke value (7.587-7.754%) are the same
quantity on different file samples; the spread is file sampling, not gate pathology. The
shape is also internally consistent with the selection: of the 6.05% completeness loss
(R2), 99.9% of lost rows have Eg>0 and 82.1% are FSR (failing Y_es<=0.1 in 69% of cases,
Q2_es<150 in 32%, EMPZ in 17% — overlapping), i.e. the ES selection preferentially drops
the deepest-dy FSR rows, which is why selected-row photon fractions (Section 5) sit
slightly below the true-fiducial ones.

**Satisfiability:** the old <= 5.0% ceiling would require the FSR-beyond-(-0.05) fraction
to be ~25% (measured 40.2%) or the FSR row fraction to be ~12% (measured 19.3%). Neither
is reachable for any genuine RAD sample under this frozen ES convention: the FSR fraction
is set by the generator's radiative engine, and the dy law of Section 2 sends every
Eg >~ 1.87 GeV FSR photon beyond the band. The 7.3-7.8% is therefore not merely allowed —
it is required. Conversely, a symmetric corruption (v1-style wrong formula) scatters
~30-50% of rows to **both** sides (v1 audit: wrong-cell 63.06-63.28%, corr 0.017); the
measured up side is empty at 0/945,730,605, which no corruption can produce.
**Verdict: THRESHOLD_MISCALIBRATION** — my original expectation ("<~1-2%") was wrong for
this convention; the witness itself was measuring the right thing on the right side.

## 4. Corrected R6 (approved; supersedes the R6 row of the original decision record)

Per RAD sample-charge, over true-fiducial rows, tolerance 0.05 unchanged; quantile
convention = plotter `hist_quantiles` (bin-center, under/overflow included, dy edges
`linspace(-0.5,0.5,201)`, dq2 edges `linspace(-1,2,301)`):

| # | check | hard threshold | warn | measured (production basis) | re-smoke |
|---|---|---|---|---|---|
| R6-q2up | frac(Q2_es_gen > 1.05*Q2sGen) | <= 5.0% | > 2.0% | 0.0000% (0 rows) | 0.0000% |
| R6-yup (NEW) | frac(Y_es_gen > YsGen + 0.05) | <= 1.0% | > 0.5% | 0 rows / 945,730,605 | 0 rows |
| R6-ydown (RECALIBRATED) | frac(Y_es_gen < YsGen - 0.05) | in [3.0%, 12.0%] | outside [4.0%, 10.0%] | 7.314 / 7.358 / 7.448 / 7.484% (RG h- / RG h+ / DJ h- / DJ h+) | 7.587-7.754% |
| R6-quant (NEW) | median(dy) in [-0.010, +0.010] AND median(dq2/Q2sGen) in [-0.015, +0.015] AND p05(dy) in [-0.160, -0.040] | all three required | — | median(dy) -0.0025 (x4); median(dq2) -0.0050 (x4); p05(dy) -0.0975 (DJ) / -0.0925 (RG); p05(dq2) -0.1350 (x4) | p05(dy) -0.0975..-0.1025 |

Rationale:

- **Band floor 3.0% (hard):** a NONRAD file fed as RAD gives r6_y_down = 3 / 5 / 0 / 0 rows
  over 215-238M rows per sample-charge (measured on the production NONRAD blocks) —
  ~0.000%, failing the floor by 3 pts. The floor is the "sample is actually radiative"
  cross-check, replacing the role my original one-sided design implicitly assigned to the
  (unsatisfiable) ceiling, and it complements the R2 completeness ceiling. Measured RAD
  margin: >= 4.3 pts.
- **Band ceiling 12.0% (hard):** symmetric corruption puts ~30-50% there (>= 2.5x above);
  genuine over-radiation or double-counted FSR would also exceed it. Measured margin:
  >= 4.1 pts against the worst single-file value (7.89%).
- **R6-yup at 1.0% (hard):** the up side is structurally empty (Section 2) — 0 rows in
  945,730,605. 1.0% tolerates unmodeled wide-angle/multi-photon effects while catching any
  sign inversion or symmetric corruption (O(10-50%) there) at >= 10x margin. This, with
  R6-q2up, preserves the one-sidedness spirit of the original design — the empty up sides
  are the sharpest anti-defect witnesses in the whole gate set.
- **R6-quant:** pins the bulk (medians; the ~80% no-photon+ISR spike sits at |dy| < 0.005)
  and the tail depth (p05) independently of the band edges; a v1-style defect (corr 0.017,
  wrong-cell 63%) fails all three windows. Seeds future tightening.
- **Q2 side stays <= 5.0% UNCHANGED** (measured 0.0000%); R1-R5 unchanged; the R6
  evaluation mask and the plotter's existing counters/histograms are sufficient — all four
  checks are evaluable at finalize from `r6_truefid_rows`, `r6_q2_up_rows`, `r6_y_down_rows`,
  `dy_hist`(+under/over), `dq2_hist`(+under/over), and `migration_quantiles_truefid`
  (edges align exactly with the +-0.05 and +-0.02 thresholds). No shard-side changes are
  required; only the finalize-side gate-eval thresholds change.

All four checks PASS on the full production basis and on the re-smoke.

## 5. GenEnPhoton>2 GeV at ~9.9% among selected rows (item 4): EXPECTED — and an erratum

**Answer: expected, not a red flag for the EMPZ 35-70 window.** Direct reproduction:
frac(Eg>2 GeV) among selected rows = 9.79-9.82% on the single file and 9.638-9.776% on the
full production basis (manifest `genenphoton_tail_selected`; frac(Eg>1 GeV) =
15.732-15.851%). The 9.9% figure is the same physics as the R6-y recalibration:

- The EMPZ 35-70 window removes **hard ISR** and transmits **hard FSR**: GenEmpz
  (builder line 84) uses the combined electron, so FSR leaves it unchanged
  (corr(FSR Eg, EMPZ) = 0.125), while ISR reduces E-pz by ~2*Eg
  (corr(ISR Eg, EMPZ) = -0.986; ISR rows with Eg>8 GeV have GenEmpz median 29.45 < 35,
  i.e. are excluded). The selected >2 GeV population is therefore FSR-dominated — exactly
  the population that drives R6-y: FSR fraction 19.3% x FSR Eg>2 GeV 40.4% ~= 7.8% ~=
  the R6-y rate, and the selected >2 GeV fraction (9.6-9.8%) is the parent rate (14.6%)
  minus the EMPZ-removed ISR part.
- **Erratum to the original decision record, Section 5.3:** the cited expectation
  "(2026-09-29 study expectation ~5.5% / ~1.25% unweighted inside the ES fiducial)" is
  WRONG and is retracted. In `data/radiative/genenphoton_radiation_fraction_2026-09-29/summary.json`
  the `unweighted_gt` / `fiducial_unweighted_gt` threshold ladders are (>0, >0.5, >1, **>5**)
  GeV, not (>0, >0.5, >1, >2): entries 1-3 reproduce exactly from the study's own spectrum
  (39.39% / 26.16% / 20.21%), but the spectrum sums to **14.58%** for >2 GeV, while the
  quoted "8.00%" 4th entry sits between frac(>4 GeV) = 9.48% and frac(>8 GeV) = 5.20%
  (i.e. it is frac(> ~5 GeV)); likewise the fiducial "1.25%" is a >5 GeV number under the
  study's own (non-ES) fiducial mask. Corrected reference for the record-level counter:
  among ES-selected rows expect **~16% (>1 GeV) and ~10% (>2 GeV)**, as now directly
  measured on the full basis. The 2026-09-29 file needs no rewrite for this addendum —
  its spectra are correct; only its threshold labels were misread by me.

## 6. Stale production submission (cluster 5323782, "71 shards", script SHA d21e0e9e...): DISCARD — moot

Forensics (local time = UTC+2):

- The stale-submission record cites script SHA
  `d21e0e9e633195081fadf3c20479afc03fdd3736b5f627ed19db09c29e2e5faa` (pre G1-N/G1-R split).
  The script is untracked in git and that SHA is unresolvable — but `wrap_shard.sh`
  executes the **live script path**, so the submit-side SHA is not what runs.
- The live script (sha256 `fac9a06f...`, mtime Oct 4 01:23 local) is the post-split
  version and is identical in the smoke, production, and finalize manifests.
- `shards.submit` says `queue 90`; the number "71" appears nowhere in the submit files.
  The condor log shows 90 jobs submitted 01:30:13 local; all 90 on-disk partials carry
  `created_utc` 23:31-23:44Z (01:31-01:44 local), all are post-split schema (R6 counters
  and dy/dq2 histograms present, 0 schema mismatches across 90 shards); finalize 5323784
  ran 01:49 local.

**Ruling:** no pre-split partial exists on disk — the stale-submission concern is moot;
discard the stale submission record (and remove any still-queued stale cluster before the
re-smoke, since an identical partial stem would be overwritten). The 90-shard on-disk set
is a valid post-split production basis and carries a valid G1-R verdict under the
corrected R6 (PASS 4/4, Section 4). Non-blocking recommendation for future runs: stamp
`script_sha256` into each shard's `meta` block so submit-side and run-side provenance
cannot diverge silently.

## 7. What changes for the re-smoke (gate list, item 5)

- **G0** — unchanged.
- **G1-N (NONRAD)** — Born-calibrated, UNCHANGED.
- **G1-R (RAD)** — R1-R5 unchanged (all passing: R1 99.583-99.627, R2 94.036-94.548,
  R3 2.800-2.907, R4 0.98786-0.99470, R5 zero mismatches); **R6 = the corrected 4-check
  set of Section 4**; Section-5 record-level counters unchanged except the GenEnPhoton
  reference, updated to ~16% / ~10% among selected rows (Section 5); Q2Gki domain-leak
  warn (~0.09%) unchanged. The plotter's gate-eval thresholds (and only those) must be
  updated; the manifest must stamp this addendum's path + sha256 next to the original
  decision record.
- **G2-G7** — unchanged, including the three smoke-driven scope fixes already recorded in
  the production manifest (G3 exclusion classes on CM-recomputed pT_h; G7 boost-invariance
  maxima gated over selected rows; G2 at-2pi band pooled per sample over charges). No
  threshold values change.
- **G4** evaluates on the production basis with its specced bands unchanged; C_rad
  consistency is unaffected — the ES convention is shared by the RAD/NONRAD selections and
  the frozen central-v6 eSigma algebra, and the Section-2 closure of the original decision
  record (implied true-level depletion -4.78..-4.88%) relied only on R1/R2, which are
  unchanged and passing.

**Production may proceed once the re-smoke passes this gate list.** The corrected R6
already passes 4/4 on the full 945,730,605-row basis; the re-smoke is procedural
confirmation that the finalize-side gate-eval implements the corrected thresholds.

## 8. Measurement index (for the manifest and audit)

Production basis (this addendum's own aggregation of the 90 partials; hist-sum and
row-counter agree exactly everywhere):

| sample-charge | truefid rows | r6_y_down rows | frac | dy>+0.05 | dy>+0.02 | dq2>+0.05 | dy underflow (<-0.5) | dq2 underflow (<-1) |
|---|---|---|---|---|---|---|---|---|
| djangoh14_rad h+ | 232,260,497 | 17,382,617 | 7.4841% | 0 | 0 | 0 | 2,630,424 (1.13%) | 206,118 (0.089%) |
| djangoh14_rad h- | 228,664,182 | 17,030,163 | 7.4477% | 0 | 0 | 0 | 2,548,142 (1.11%) | 191,239 (0.084%) |
| rapgap31_rad h+ | 244,686,289 | 18,002,881 | 7.3575% | 0 | 0 | 0 | 2,693,724 (1.10%) | 233,503 (0.095%) |
| rapgap31_rad h- | 240,119,637 | 17,562,670 | 7.3141% | 0 | 0 | 0 | 2,593,444 (1.08%) | 216,328 (0.090%) |

NONRAD control (same counters): r6_y_down rows = 3 (DJ h+), 5 (DJ h-), 0 (RG h+), 0 (RG h-);
r6_q2_up = 0; dy > +0.05 = 0 — the band floor's wrong-sample-feed reference.

Single-file diagnostics (`rad_ep_0000.root`): r6_y 7.891% / 7.841% (h+ / h-); composition
63.2% / 17.5% / 19.3% (Eg==0 / ISR / FSR); beyond -0.05: 0% / 0.78% / 40.2%; closure 7.90%;
dy law residual med 0.0000 / p90 0.0006; (1-Ys) median 0.689; FSR Eg>2 GeV 40.4%; max dy
+0.00023; Eg>1 / Eg>2 among selected rows on the file 16.6% / 10.5% (ES-fiducial mask) and
9.79-9.82% >2 GeV (plotter selection); parent per-event Eg>0 / >1 / >2 = 39.5% / 20.3% /
14.7% (study cross-check: 39.39% / 20.21% / 14.58%).

## 9. Verdict

**THRESHOLD_MISCALIBRATION** — not a defect, not a wrong formula: the R6-y witness measured
the correct quantity, but my original sign model ("FSR pushes Y_es UP") did not describe
the frozen ES convention (merged-FSR electron + unconditional wrong-side beam subtraction),
and the expected rate (<~1-2%) was wrong; the physically required rate under this
convention is 7-8%, now derived (Section 2), closed (Section 3), and gated (Section 4).

**PROCEED_AFTER_RESMOKE** under the Section 7 gate list.
