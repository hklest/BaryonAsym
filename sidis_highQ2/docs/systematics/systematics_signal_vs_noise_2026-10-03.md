# Signal vs noise in the v6/z005 per-bin systematic components — 2026-10-03

**Question.** Are the per-bin systematic components in the v6/z005 budget picking up
statistical fluctuations, or do they contain real bin-to-bin variation?

**Status.** PROVISIONAL INTERNAL DIAGNOSTIC analysis on existing artifacts only (read-only,
no new production). Verdicts that rest on MAD-based noise floors are provisional pending P1
replica floors; the tracking verdict is definitive (measured 16-seed floor).
**UPDATE 2026-10-04 (appended, Section 8):** the P1 Phase A replica floors are measured
(N=100, FLOORS_READY). The five detector-source verdicts and the ibu_reg per-bin status are
now DEFINITIVE and supersede the provisional floor-based numbers below; the provisional
results are retained for audit.

**Inputs.** S2M smoothing sidecar (`sidis_highQ2/data/systematics/smoothing_s2m_2026-10-03/
s2m_sidecar_per_bin.csv`), the tracking 16-seed ensemble + 16 per-seed releases
(`output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/ensemble_ibu_data4p_v2_20260703/`),
and the central total table (2026-07-03 with_tracking). Metrics JSONs:
`sidis_highQ2/data/systematics/signal_vs_noise_2026-10-03/{metrics,cell_level_metrics}.json`;
scripts: `sidis_highQ2/scripts/systematics/signal_vs_noise_2026-10-03/`.

---

## 1. Executive answer

Both, cleanly separated by scale — and the separation matters:

1. **Every detector source except elec_energy carries a REAL, coherent cell-level response**
   (per-(Q2,x)-cell offsets significant at 3–24 sigma against the floor, with 7/7 or 6/7
   sign agreement between charges). This is real bin-to-bin variation of the systematic itself.
2. **The fine-grained (adjacent-pT) structure is noise-dominated for all sources except
   tracking**, whose small-scale wiggles are also mostly real (chi2/dof = 3.9 against a
   validated floor). The fine-grained jitter carries negatively autocorrelated statistical
   errors (lag-1 residual correlation −0.18…−0.29 for all eight sources), the IBU
   sum-constraint signature: the per-bin shift estimates are compensating fluctuations.
3. **Tracking is definitively REAL STRUCTURE** (reference case, measured floor): 238/490 bins
   have |mean shift| >= 3 SEM vs 1.3 expected under pure noise; 85 kinematic bins are >= 3 sigma
   in both charges simultaneously; ~79% of its per-bin variance is real.
4. **ibu_model is a genuine model difference** (charge correlation +0.51, 8 sigma, robust to
   outliers) whose per-bin values are contaminated by sparse-bin noise blowups.
5. In a typical populated bin, all these components sit at 5–45% of the per-bin data
   statistical uncertainty (median |d|/ibu_stat: 0.05–0.45); only in sparse high-Q2 bins do
   they exceed it.

The earlier design-phase "74–100% noise variance" fractions measured only the
within-pT-row (small-scale) component and are not contradicted: they missed the coherent
cell-level response underneath, which the charge-correlation and cell-median tests below
isolate floor-free.

---

## 2. Reference case: tracking (DEFINITIVE — measured 16-seed floor)

Noise model validation first (this is what makes the verdict definitive):
- The stored SEM (`seed_mean_unc = seed_rms/sqrt(16)`) reproduces the empirical per-bin
  standard error from the 16 per-seed releases exactly (median ratio 1.00).
- The pooled standardized per-seed shifts have excess kurtosis **+0.11** (Gaussian; per-bin
  median −0.43 is n=16 sampling noise) — no heavy tails, so z-values are trustworthy.

Evidence:
- **238/490 bins (48.6%) with |mean shift| >= 3 SEM** (pure-noise expectation: 1.3 bins);
  303/490 (61.8%) >= 2 SEM (expectation 22.4). Median z = 2.87; max z = 23.3
  (neg/58, S0_x1, z4 p3, d = −0.77%). The >= 3 sigma bins span all 14 (charge, cell) grids
  (8–28 of 35 bins each); **85 bin indices are >= 3 sigma in both charges at once**
  (e.g. bin 47 S0_x1: z = 19.6 pos / 16.0 neg). Full list: `metrics.json` → `tracking_deep`.
- **Coherent, charge-identical cell response:** cell medians −0.127…−0.273% in S0–S2
  (12/14 cells at |z| up to 24), with a REAL sign flip to +0.107/+0.145% in S2_x1 in both
  charges; pos/neg cell-median correlation +1.00, 7/7 sign agreement.
- **Floor-free confirmation:** bin-level pos-vs-neg correlation of the signed shift
  rho = +0.63 (9.9 sigma; Spearman +0.57).
- **Real fine-grained variation:** residuals around the S2M smooth give chi2/dof = 3.9 and
  pT-roughness 2.1x the SEM expectation — with a Gaussian, validated floor this is real
  sub-smooth bin-to-bin variation (IBU amplification of the 0.5% track loss genuinely
  changes with local spectrum shape), not fluctuation.
- Variance decomposition against the measured floor: f_noise = 0.21 → **~79% of the per-bin
  variance is real structure**, ~21% seed noise.

**Verdict: the tracking systematic is REAL STRUCTURE, not fluctuation** — a coherent
negative offset (−0.13…−0.27%) in S0–S2 with a real sign flip in S2_x1, plus real
fine-grained bin-to-bin variation; median |d| among the >= 3 sigma bins 0.23%, tail to 2.8%.

---

## 3. Per-source evidence (490 bins, both charges; floor-based numbers provisional except tracking)

| metric | hfs_jes | hfs_rces | hfs_theta | elec_energy | elec_theta | ibu_model | ibu_reg* | tracking |
|---|---|---|---|---|---|---|---|---|
| f_noise (var vs floor) | 0.13 | 0.40 | 0.31 | 0.09† | 0.14 | 0.28 | 0.11† | **0.21** |
| lag-1 pT autocorr (raw) | +0.01 | +0.04 | +0.06 | −0.04 | −0.10 | −0.08 | −0.21* | +0.07 |
| lag-1 residual autocorr | −0.24 | −0.21 | −0.18 | −0.26 | −0.29 | −0.26 | −0.28 | −0.27 |
| sign-flip rate (noise: 0.50) | 0.27 | 0.17 | 0.17 | 0.47 | 0.37 | 0.47 | 0.00* | 0.25 |
| roughness ratio (noise: 1.0) | 0.80 | 0.60 | 0.56 | 0.96 | 0.84 | 0.80 | 0.77 | 2.13 |
| chi2/dof vs smooth (noise: 1.0) | 3.3 | 0.87 | 0.93 | 2.9 | 2.2 | 1.5 | 2.2 | **3.9** |
| bins >= 2 sigma (exp. 22.4) | 109 | 111 | 130 | 113 | 112 | 86 | 145 | **303** |
| bins >= 3 sigma (exp. 1.3) | 66 | 46 | 56 | 77 | 73 | 49 | 99 | **238** |
| frac bins < 2 sigma | 0.78 | 0.77 | 0.73 | 0.77 | 0.77 | 0.82 | 0.70 | **0.38** |
| charge rho, bin level (Pearson) | +0.27 | +0.06 | −0.07 | −0.43† | +0.01 | +0.51 | +0.38* | **+0.63** |
| — Spearman / trimmed (|d|<=10%) | +0.24/+0.21 | +0.25/+0.06 | +0.05/−0.07 | +0.20/+0.35 | +0.06/−0.09 | +0.52/+0.49 | +0.68/+0.59* | +0.57/+0.63 |
| cell medians >= 3 sigma (of 14) | 5 | 12 | 10 | 0 | 5 | 0† | 14 | **12** |
| charge sign agreement (cells) | 7/7 | 7/7 | 7/7 | 5/7 | 6/7 | 6/7 | 7/7 | **7/7** |
| charge rho, cell medians | +0.77 | +0.73 | +0.04 | +0.11 | +0.66 | +0.87 | +0.62 | **+1.00** |
| coherent amplitude (cell medians) | +0.23…+1.25% | +0.15…+0.32% | +0.02…+0.32% | none | +0.03…+0.41% | −1.35…+1.13% | +0.22…+1.19% | −0.27…+0.15% |
| median abs(d)/ibu_stat | 0.26 | 0.09 | 0.09 | 0.45 | 0.15 | 0.45 | 0.19 | 0.05 |

*ibu_reg is on the abs fallback basis (no sign information; sign-flip/lag-1 on |d|).
†Unreliable: heavy-tailed (elec_energy, 38 bins with |d|>10%, one +141%) or folded-distribution
floor (ibu_reg) biases the MAD decomposition; per-bin classes for these two are indicative only.

Pure-noise references: sign-flip 50%; lag-1 of independent noise 0 ± 1/sqrt(392 rows) = 0.05;
>= 2 sigma in 4.6% (22.4 bins), >= 3 sigma in 0.27% (1.3 bins); charge rho 0 ± 0.064.

---

## 4. Per-source verdicts

Bin-class fractions are floor-based (< 2 sigma NOISE-DOMINATED, 2–3 sigma MIXED,
>= 3 sigma REAL-per-floor) and provisional for every source except tracking.

- **tracking — REAL STRUCTURE (definitive).** Noise 21% of variance / real 79%;
  38% of bins noise-consistent, 13% mixed, 49% real (>= 3 sigma). Real amplitude: coherent
  −0.13…−0.27% (S0–S2) with sign flip +0.11…+0.15% (S2_x1), fine structure median 0.23%.
- **hfs_jes — MIXED: real coherent cell response, noise-dominated fine structure (fine-grain provisional).**
  78% / 9% / 13%. Real component: +0.23…+1.25% cell offsets (rising with Q2), all-positive,
  7/7 charge agreement, cell rho +0.77, bin-level charge rho +0.27 (4 sigma, floor-free).
  Fine-grained excess (chi2/dof 3.3) is real-structure-or-floor-miscalibration — P1 decides.
- **hfs_rces — MIXED, closest to a pure coherent response: real cell offsets, fine structure consistent with noise.**
  77% / 13% / 9%. Real component: +0.15…+0.32% (12/14 cells >= 3 sigma, 7/7 agreement,
  cell rho +0.73). chi2/dof 0.87 and roughness 0.60: adjacent-pT jitter fully consistent
  with the floor. Cleanest "smooth real signal + noise" case after tracking.
- **hfs_theta — MIXED: real coherent cell response; the bin-to-bin varying part is charge-specific.**
  73% / 15% / 11%. Real component: +0.02…+0.32% cell offsets (10/14 >= 3 sigma, all positive,
  7/7 agreement). Bin-level charge rho ~ 0: the varying component does not repeat between
  charges — real charge-specific structure and/or noise (chi2/dof 0.93: consistent with floor).
- **elec_energy — NOISE-DOMINATED at bin level, sub-dominant real correlated component, heavy-tailed sparse-bin blowups (provisional).**
  77% / 7% / 16% (floor-based; unreliable under heavy tails). No coherent cell offsets
  (0/14 >= 3 sigma; medians −0.52…+0.40%, inconsistent). Bulk shows real shared structure
  (trimmed charge rho +0.35, Spearman +0.20); the raw Pearson −0.43 is driven by
  anti-correlated sparse-bin outliers (e.g. bin 179: +141% pos vs −61% neg). The > 50%
  excursions are noise blowups (kept raw by the S2M exclusion rule). Typical |d| 0.88%
  sits at the noise scale.
- **elec_theta — MIXED: real low-Q2 coherent response, noise-dominated elsewhere (provisional).**
  77% / 8% / 15%. Real component: +0.2…+0.41% cell offsets confined to S0/S1 (5/14 cells
  >= 3 sigma, 6/7 agreement, cell rho +0.66); high-Q2 cells consistent with zero.
  Bin-level charge rho ~ 0; chi2/dof 2.2.
- **ibu_model — REAL model difference contaminated by sparse-bin noise (case (c)).**
  82% / 8% / 10% (floor-based, provisional). The DJANGOH-vs-RAPGAP response difference is
  genuinely charge-correlated (bin rho +0.51, Spearman +0.52, trimmed +0.49 — 8 sigma,
  floor-free) and cell-patterned (cell-median rho +0.87, 6/7 sign agreement, sign varying
  by cell: −1.35%…+1.13%): a real, signed, cell-dependent model difference at the ~1–2%
  level in populated cells. Per-bin values in sparse cells are noise-blowups (27 bins
  |d|>10%, one excluded at 53.8%; smooth std(m) 3.9% dominated by S3_x0).
- **ibu_reg — REAL cell-level magnitude structure; per-bin status INDETERMINATE (provisional, abs-fallback floor).**
  70% / 9% / 20% (floor-based; the abs-basis MAD floor is biased low on the folded
  distribution, so per-bin classes cannot be certified). The magnitude pattern is real and
  charge-shared: 14/14 cell medians >= 3 sigma (+0.22…+1.19%, rising with Q2/x), 7/7
  agreement, cell rho +0.62, bin-level rank rho +0.68 — the strongest rank correlation of
  all sources. Needs P2 signed tau x2 / tau / 2 values plus P1 floors for per-bin verdicts.

---

## 5. Separating the three things asked

**(a) Real bin-to-bin variation of the systematic itself.** Present in tracking
(definitively, including sub-smooth scales), hfs_jes, hfs_rces, hfs_theta, elec_theta (low Q2),
ibu_model, and the ibu_reg magnitude — as coherent per-cell offsets (0.15–1.25%) plus, for
tracking, genuine fine-grained variation. Demonstrated floor-free by charge correlation
and cell-median sign agreement.

**(b) Statistical fluctuation of the shift estimate.** Dominates the adjacent-pT structure of
all detector sources: residuals around the smooth are at the floor scale with NEGATIVE
lag-1 autocorrelation (−0.18…−0.29) — the IBU sum-constraint signature (compensating
bin-to-bin errors). This is why raw per-bin values jitter and dip, and why the S2M
second-moment treatment (floor-lifted smooth + excursion preservation) is the right
budget representation. For elec_energy the fluctuations are heavy-tailed and blow up in
sparse bins to tens of percent.

**(c) ibu_model: genuine model difference + sparse-bin contamination.** The coherent
signed pattern (charge rho +0.51, cell rho +0.87) is the real DJANGOH-vs-RAPGAP response
difference; the sparse-cell values (S3_x0, high-z corners) are dominated by statistical
blowups. Any per-bin use of ibu_model should carry the standing truth-conservation caveat
and treat sparse-cell excursions as noise-suspect until P1 MC-replica floors exist.

---

## 6. What is provisional, and the minimal P1 recipe

**Will NOT change with P1** (floor-free evidence): tracking REAL verdict and its numbers;
the existence of coherent cell-level responses for hfs_jes/hfs_rces/hfs_theta/elec_theta/ibu_reg;
the ibu_model real-model-difference verdict; the IBU anti-correlated-noise characterization.

**WILL change / become definitive with P1:** every floor-based per-bin class fraction in
Section 4 (all MAD-floor sources); the interpretation of chi2/dof > 1 fine-grained excesses
(real sub-smooth structure vs floor miscalibration) for hfs_jes (3.3), elec_energy (2.9),
elec_theta (2.2), ibu_reg (2.2), ibu_model (1.5); the elec_energy heavy-tail handling; the
ibu_reg per-bin certification (also needs P2 signed values).

**Minimal P1 recipe (not run).** For each of the five one-sided detector shifts
(hfs_jes_down, hfs_rces_up, hfs_theta_up, elecE_p1pct, elecTheta_p1mrad) plus the nominal:
produce N = 100 paired event-split (or bootstrap) data replicas, re-unfold each
(nominal, shifted) pair with the frozen central IBU settings (tau fixed, k = 4), and take
the per-bin spread of (shifted − nominal)/nominal across replicas as the measured per-bin
noise floor (pairing removes the common data-statistics component; N = 100 gives ~7%
precision per sigma_i, N = 50 is the minimum); extend the same machinery with response-matrix
bootstrap replicas for ibu_model and with signed tau x2 / tau / 2 unfolds (P2) for ibu_reg;
then re-run this analysis and the S2M smoother with measured floors. The existing 16-seed
tracking ensemble is the exact template.

---

## 7. Caveats

1. All floor-based fractions for the seven MAD sources are PROVISIONAL (P1 pending); the
   MAD floor is estimated from the same field it judges, is biased under heavy tails
   (elec_energy) and on the folded abs distribution (ibu_reg).
2. The dof = n − 3 approximation for chi2/dof against the per-target local-linear S2M
   smooth is rough; values near 1 are robust, values 2–4 indicate order-of-magnitude excess.
3. The tracking z-values assume iid seeds; the pooled Gaussianity check (excess kurtosis
   0.11) supports this; its SEM is empirically validated.
4. Roughness ratios assume independent noise; the measured residual anti-correlation
   (−0.2 to −0.3) biases them low by ~10–15% — direction favours the noise interpretation
   even less being needed, i.e. conclusions are conservative.
5. Nothing here modifies any central product; the central budget remains raw /
   no-bgsub / no-C_rad / not Born level.

---

# 8. UPDATE 2026-10-04 — measured P1 replica floors: definitive verdicts

**What happened since.** P1 Phase A ran to completion: five one-sided detector shifts
(hfs_jes_down, hfs_rces_up, hfs_theta_up, elecE_p1pct, elecTheta_p1mrad) + nominal, N=100
paired Poisson data-vector bootstrap replicas through the frozen central IBU (k=4)
(`output_v6/studies/p1_replica_floors_2026-10-03/`, FLOORS_READY: 100/100 replicas valid,
0 excluded, 490/490 finite floors per source, replica-mean vs working-point bias
0.066–0.075 of floor — unbiased; floor precision ~7%). Phase B item B2 (signed ibu_reg
k±1 floors from the stored iteration vectors) is also complete. This section re-runs the
floor-based analysis with the MEASURED floors and supersedes every provisional floor-based
number in Sections 3–5 for those sources. The original provisional numbers above are kept
for audit. Tracking (Section 2) was already definitive and is unchanged. Artifacts:
`data/systematics/signal_vs_noise_2026-10-03/{metrics_measured_floors,mc_flip_noise_bound,
ibu_reg_signed_floors_b2}.json`, scripts `scripts/systematics/signal_vs_noise_2026-10-03/
{rerun_with_measured_floors,mc_flip_noise_bound,b2_ibu_reg_signed_floors}.py`.

## 8.1 Headline: the provisional noise-dominated fractions were a MAD artifact

The MAD floors were estimated from the scatter of the same fields they judged, so they
absorbed real structure as "noise". The measured data-statistical floors are **9–60×
smaller** (per-bin ratio floor/MAD, median [quartiles]): hfs_jes 0.017 [0.006–0.057],
hfs_rces 0.028 [0.009–0.078], hfs_theta 0.029 [0.008–0.105], elec_energy 0.104
[0.033–0.428], elec_theta 0.054 [0.017–0.290]. Only 1.0–3.1% of bins (HFS) / 12.5–16.3%
(electron, the sparse heavy-tail bins) have MAD *below* the measured floor — the
"floor-too-small" miscalibration scenario is excluded except exactly where the heavy tails
live. 64–89% of bins have MAD >= 5× the measured floor.

Definitive per-bin fractions against the measured floors (490 bins, current-files working
point; classes: < 2σ NOISE-DOMINATED, 2–3σ MIXED, >= 3σ REAL), with the conservative
variant adding the first-order MC flip-noise bound in quadrature (Section 8.4):

| source | med floor | med \|d\| | med z | NOISE | MIXED | REAL | REAL (⊕ flip bound) |
|---|---|---|---|---|---|---|---|
| hfs_jes | 0.0147% | 0.33% | 21.2 | 9.2% | 4.1% | **86.7%** | 71.2% |
| hfs_rces | 0.0059% | 0.24% | 35.8 | 4.9% | 2.7% | **92.4%** | 85.9% |
| hfs_theta | 0.0069% | 0.26% | 33.8 | 6.1% | 3.5% | **90.4%** | 83.9% |
| elec_energy | 0.2015% | 1.45% | 6.8 | 21.2% | 8.0% | **70.8%** | 41.8% |
| elec_theta | 0.0293% | 0.46% | 13.2 | 15.5% | 5.7% | **78.8%** | 52.7% |

Pure-noise expectation: 4.6% >= 2σ, 0.27% >= 3σ. Observed >= 3σ counts are 347–453 of 490.
The provisional "74–78% noise-consistent" fractions (Section 3) are refuted: they measured
the MAD scale, not the noise.

## 8.2 Resolution of the chi2/dof question (Section 6, "WILL change" list)

The earlier MAD-based chi2/dof values (hfs_jes 3.3, elec_energy 2.9, elec_theta 2.2,
hfs_rces 0.87, hfs_theta 0.93) were **nearly uninformative**: the S2M smooth was
noise-calibrated to the same MAD scale, so both the "excesses" and the "consistencies"
reflected the MAD construction. Recomputed against the SAME S2M smooth with measured
floors, chi2/dof = 5131 (hfs_jes), 1508 (hfs_rces; 3378 vs cell median), 2668 (hfs_theta),
154 (elec_energy), 722 (elec_theta): the sub-smooth residuals are **12–72× the true
data-statistical noise in rms**. Verdict: the fine-grained excesses were **real sub-smooth
structure, understated by the MAD calibration** — not heavy-tail floor miscalibration.
The heavy-tail caveat survives only where it always applied: the sparsest bins, whose
measured floors are huge (elec_energy max 29.8%, elec_theta 17.7%) and which remain
noise-flagged. The IBU anti-correlation signature persists in the noise component itself
(residual lag-1 with measured floors −0.25…−0.33).

## 8.3 Definitive per-source verdicts (supersede Section 4 for these six)

- **hfs_jes — REAL STRUCTURE (definitive).** 86.7% of bins >= 3σ (425/490, med z 21),
  14/14 cell medians >= 3σ, 7/7 charge sign agreement, f_noise 3.6%. The provisional
  "noise-dominated fine structure" is overturned: the fine-grained field is real at
  ≫3σ. Basis note: the working point is the current-files delta; the published z005
  hfs_jes_down comparison column is a stale vintage (documented 2026-10-03) and must be
  regenerated before the next budget cycle. Current-basis charge rho +0.16 (published
  basis +0.27).
- **hfs_rces — REAL STRUCTURE (definitive); cleanest coherent case CONFIRMED.** 92.4%
  >= 3σ (453/490, med z 36), f_noise 1.6% (smallest of all sources), 14/14 cells, 7/7
  agreement. The provisional "fine structure consistent with noise" (chi2 0.87) was a
  MAD-scale artifact; with measured floors rces is the most strongly structured detector
  source. Survives the flip-noise bound at 85.9%.
- **hfs_theta — REAL STRUCTURE (definitive).** 90.4% >= 3σ (443/490, med z 34), 14/14
  cells, 7/7 agreement, f_noise 4.0%. The charge-specific varying component is real
  per-bin structure, not noise (provisional chi2 0.93 was uninformative).
- **elec_energy — OVERTURNED: REAL STRUCTURE in the bulk, noise-dominated only in the
  sparse tail (definitive).** 70.8% >= 3σ (347/490, med z 6.8), 11/14 cell medians >= 3σ;
  the bulk pattern is real response structure of the +1% electron-energy shift, not
  fluctuation (trimmed charge rho +0.35; the full rho −0.43 is driven by shared
  anti-correlated sparse-bin outliers). The provisional NOISE-DOMINATED bin-level verdict
  (0/14 cells, "typical |d| at the noise scale") is refuted: the typical bulk |d| (1.45%)
  is ~7× its measured floor (0.20%). The sparse-bin blowups (>50% excursions, kept raw by
  the S2M exclusion rule) remain noise-dominated — their floors reach 29.8%. Weakest
  survivor under the flip bound (41.8% real) — per-bin use should carry it.
- **elec_theta — REAL STRUCTURE majority (definitive).** 78.8% >= 3σ (386/490, med z 13),
  10/14 cells >= 3σ, 6/7 agreement. The provisional "real low-Q2 response only" widens:
  the bulk fine structure is real as well. Under the flip bound 52.7% real — still a
  majority.
- **ibu_reg — MIXED, now CERTIFIED per bin (B2, signed basis; supersedes "per-bin status
  INDETERMINATE").** Signed one-sided shifts d± = (N[k=5 or 3]−N[4])/N[4] from the stored
  Phase A iteration vectors (working point reproduced to 7e-16; published reg_unc_yield
  identified as the half-spread |N5−N3|/2, matched to 1e-11). Measured floors: +0.32% /
  −0.43% (pos), +0.35% / −0.45% (neg) median; half-spread floor 0.38/0.40%. Classes
  (max-side): 29–31% REAL, ~19% MIXED, 51–53% NOISE (half-spread basis: 26–27% REAL,
  53–56% NOISE); med z ≈ 1.9 vs 0.67 expected under pure noise; >= 3σ counts 146
  (max-side; 130 half-spread) vs 1.3 expected. The old MAD-abs floor was 1.8–1.9× the measured signed floor and ~51% of bins
  (124–127/245 per charge) change noise status — the abs-fallback suspicion is quantified
  and confirmed. The real component is the convergence-gradient pattern (cell medians
  +0.22…+1.19%, rising with Q2/x, rank rho +0.68, floor-free evidence unchanged); the
  ibu_reg field is genuinely half noise at bin level. MC note: k=3,4,5 share the frozen
  central response, so response MC noise is common-mode and cancels — these floors are
  data-statistics-dominated and need no flip bound.
- **tracking, ibu_model — unchanged.** Tracking was already definitive. ibu_model's
  field-level REAL-model-difference verdict was floor-free and stands; it acquires no
  measured floor in this update (B1 not run, Section 8.6).

## 8.4 MC-response statistics: same events confirmed, first-order flip bound

The P1 floors freeze the responses, so the MC-statistics component of each shift estimate
is not in them. Scope check on the six migration roots: every shifted root shares the
**same underlying MC events** as central (h_mc_count and prior bit-identical; weighted
response column totals agree to 1e-6–1e-4 relative) — the shifts are re-reconstructions,
so only re-reconstruction "flip" noise enters the shift estimates (common response noise
cancels in the paired difference, exactly like the data pairing). First-order net-flow
estimate per bin: sigma_flip = sqrt(wbar_j · Σ_i |dB_ij| dsig_i²)/W_j (flip fractions:
jes 0.17%, rces 0.03%, theta 0.02%, elecE 2.7%, elecT 0.3% median). Result: sigma_flip is
**2.4–4.1× the data floor at the median bin** (medians: jes 0.040%, rces 0.011%,
theta 0.008%, elecE 0.56%, elecT 0.12%) but only 3–34% of |d| — the REAL verdicts survive
total noise at the reduced fractions in the Section 8.1 table. Caveats: net-flow (gross
flips second order at ~1% shifts), uniform-weight approximation, own-column one-step
propagation. The definitive MC treatment is a B1-style response bootstrap.

## 8.5 Consequences for the S2M smoothing study (measured vs MAD verdict)

The S2M smoother was noise-calibrated to the MAD floors — 9–60× the true
data-statistical noise (Section 8.1). Its published conclusions **change materially**
under measured floors and must not be quoted as-is for the five detector sources:

- **Dip repair counts (85–144 lifted bins per source) would collapse in populated bins**:
  the lifts were driven by the overestimated noise scale (excursion floor ~0.42σ_c,
  lift envelope |d|−2σ_MAD). With measured floors the "dips" are >= 3σ real structure
  and must be tracked, not lifted; floor-lifting remains meaningful only in the sparse
  bins (where measured floors are large).
- **The smoothed values would approach the raw values** in populated bins (WLS weights
  1/v with v smaller by 80–3600×): the S2M "resolution floor" behavior disappears there.
- **The no-shrinkage guarantee weakens to irrelevance** (u >= σ/√N_eff with σ tiny) —
  harmless, but no longer doing useful work.
- **Excursion flags would fire almost everywhere** against the old MAD-calibrated smooth
  — the 0.42σ_c amendment-1 gate constant is an artifact of the MAD scale and must be
  re-derived.

**Recommendation (not executed in this task, per instruction):** re-run the S2M smoother
for the five detector sources with v_i = floor_P1,i² + sigma_flip,i² (both now available
per bin), hyperparameters unchanged, config re-frozen; expect smoothed ≈ raw in populated
bins with sparse-bin floor lifts; re-derive the dip/excursion gates afterward. ibu_reg
can be re-smoothed on the signed B2 basis with its measured floor (no abs fallback
needed). ibu_model smoothing stays MAD-based until B1. All S2M outputs remain PROVISIONAL
INTERNAL DIAGNOSTIC; the response truth-conservation caveat (±2.8%, 245/245 bins)
continues to travel with any smoothed ibu_model value. (2026-10-06: root-caused; coherent-prior fix validated, central adoption PENDING — see `docs/unfolding/coherent_prior_fix_results_2026-10-05.md`.)

## 8.6 B1 (ibu_model response bootstrap): still needed — yes

B1 is NOT needed to support the ibu_model field-level verdict (real model difference,
floor-free evidence, Section 4) — that stands without it. B1 remains necessary, with
unchanged design (phaseB_spec B1), for (a) per-bin ibu_model certification and (b)
budget-grade total ibu_model floors. The Section 8.4 finding reinforces this: RAPGAP and
DJANGOH are independent MC samples, so the crossclosure difference has no same-event
cancellation — MC statistics enters at first order and, at the measured detector-floor
scale (1e-4–2e-3), likely dominates the ibu_model per-bin noise. Do not use ibu_model
per-bin values (beyond the field-level statement) before B1 runs.

## 8.7 Updated caveats (delta on Section 7)

1. The measured floors are DATA-STATISTICAL floors (paired bootstrap); the MC-response
   component is bounded first-order (Section 8.4) and enters any budget-grade total in
   quadrature; the definitive treatment is B1-style response bootstrap.
2. hfs_jes per-bin numbers in this update use the current-files working point; the
   published z005 hfs_jes_down comparison column is stale (regenerate before the next
   budget cycle). The other four sources' published columns match the current files.
3. Bin-level Poisson bootstrap preserves per-bin marginals, not event-level cross-bin
   correlations (Phase A design caveat, unchanged); pairing is bin-aligned.
4. The dof = n−3 chi2 caveat (Section 7.2) is now moot for the measured-floor chi2
   values, which are orders of magnitude from the decision boundary.
5. Everything in this section remains PROVISIONAL INTERNAL DIAGNOSTIC; no central product
   is modified; the central release remains raw/no-bgsub/no-C_rad/not Born level.

# 9. UPDATE 2026-10-05 — B1 ibu_model response-bootstrap floors: per-bin certification

**What happened since.** B1 ran to completion: 100 cell-level parametric bootstrap
replicas of the RAPGAP31/DJANGOH14 responses (Poisson resampling on n_eff from the
`_w2` companions, seeds 2026100400+r, pinned published crossclosure recipe, IBU
k=4); drain check 100/100 replicas valid, 0 excluded. Floors = per-bin std (ddof=1)
of the signed half-spread d_model = 0.5*(bias_dj − bias_rg) across replicas:
`output_v6/studies/p1_replica_floors_2026-10-03/ensemble_b1/b1_ibu_model_floors.{json,csv}`
(schema b1_ibu_model_floors_v1). Median floor 1.218% (p95 6.58%, max 16.05%);
median working-point |d_model| 1.581% (the v2 comparison column median 1.594% is
the same field on the published-column basis). The aggregation printed
`status=FAIL` — that is a per-bin working-point verdict on exactly ONE supported
neg bin, not a production failure. (A NameError in b1_floor_aggregation.py — the
`problems` list was not initialized — was fixed mechanically; no other change.)
The crossclosure estimator is data-free, so the B1 floor is the COMPLETE
statistical floor of ibu_model: it includes the MC-response component that the
detector floors only bound (Section 8.4), and no flip bound is added to it.

## 9.1 The one working-point failure: statistically expected extreme; bin excluded from certification only

Failing bin: **neg, cell S1_x1, z_idx 5, pT_idx 0, bin_index 130,
global_bin_index 375.** Working point |d| = 2.933%; replica mean 3.598%;
|mean − wp| = 0.665% vs the 3-SEM gate 3*floor/sqrt(N) = 0.614% → **1.083× the
threshold** (3.25 SEM). Diagnosis — fluctuation, not pathology:

- The replica ensemble for this bin is symmetric (skew −0.07; std 2.047% = its
  floor) and the working point sits at the **33rd percentile** of it — a modestly
  low draw, not an outlier (67/100 replicas lie above wp).
- The near-threshold census over all 490 bins is a smooth continuum: dev/threshold
  = 0.985 (twice — including the SAME cell at pos charge, pos bin 130, and neg bin
  123), then 0.87, 0.78, 0.76 … — extreme-value structure, not a localized defect.
- Under unbiased replicas the expected number of >3σ working-point excursions
  among 490 bins is ~1.3; observed is 1, and only 8% over threshold.
- The ensemble is globally unbiased: median |dev|/SEM over all 490 bins = 0.653 vs
  0.674 expected under normality.
- No n_eff collapse: the bin's floor (2.05%) is the same order as its cell
  neighbors (S1_x1 floors 0.99–9.57%); nothing sparse-pathological about it.

**Disposition: the B1 floor set is RELIABLE** (489/490 bins pass; globally
unbiased; the single failure is the statistically expected extreme). The one bin
is **excluded from per-bin ibu_model certification** (flagged
working-point-unreliable), with its raw value and floor retained unmodified in all
outputs; its provisional class would have been MIXED (z = 1.43), so the REAL
fraction is insensitive to the disposition (20.7% certified vs 20.6% if included).
Do NOT flag the whole B1 floor set.

## 9.2 Per-bin ibu_model certification (B1 floors; classes < 2σ NOISE-DOMINATED, 2–3σ MIXED, >= 3σ REAL)

| basis | charge | n | med floor | med z | NOISE | MIXED | REAL |
|---|---|---|---|---|---|---|---|
| B1 (2026-10-05) | pos | 245 | 1.156% | 1.40 | 64.1% | 13.5% | **22.4%** |
| B1 (2026-10-05) | neg | 244 | 1.285% | 1.23 | 66.4% | 14.8% | **18.9%** |
| MAD (v2 = v1) | pos | 244 | 2.117% | 0.72 | 83.2% | 7.4% | 9.4% |
| MAD (v2 = v1) | neg | 245 | 2.220% | 0.70 | 81.6% | 7.8% | 10.6% |

Notes: B1 pos includes the >50% raw bin (S3_x0 z6 pT4, d = 53.8%), which is REAL
at z = 5.31 even against its 10.1% floor; the MAD rows exclude it (S2M sidecar
exclusion). B1 neg excludes the Section 9.1 gate-fail bin; MAD neg includes it.
Median |d| = 1.60% (pos) / 1.54% (neg) on the certified sets.

**Combined (489 certified bins): 65.2% NOISE-DOMINATED, 14.1% MIXED, 20.7% REAL**
— 101 bins >= 3σ vs ~1.3 expected under pure noise (~76×): the REAL bins are
decisively real. REAL fraction rises with Q2 stratum (S0 24%, S1 12%, S2 21%,
S3 30%).

**B1 vs MAD:** the B1 floor is 0.55× the MAD scale at the median (per-bin ratio
quartiles 0.26–1.14, n = 488 comparable bins) — for ibu_model the MAD was only
~1.8× too big, unlike the detector sources' 9–60×, because this field is genuinely
noise-dominated in the bulk. 182/488 bins (37%) change class: REAL roughly doubles
(71 NOISE→REAL, 12 MIXED→REAL, against 21 REAL→NOISE + 11 REAL→MIXED where the
B1 floor exceeds the local MAD).

**Field statement:** with median floor 1.218% vs median |d| ~1.58–1.59%, the
per-bin ibu_model field is noise-dominated in the bulk (65%) with a decisively
real ~21% minority. The field-level REAL-model-difference verdict (Section 4,
floor-free) stands and now carries per-bin certification; the Section 8.6
restriction ("do not use ibu_model per-bin values before B1") is lifted for
certification purposes, with the caveats below.

## 9.3 Residual caveat (delta on Section 8.7)

B1 resamples the RESPONSE and full-marginal MC statistics only (n_eff from the
`_w2` companions). The prior/truth histograms are FIXED published inputs — their
own MC statistical fluctuations are NOT resampled (no `_w2` exists for them). The
crossclosure truth-conservation caveat (±2.8%, 245/245 bins) continues to travel
with any per-bin ibu_model value. Everything in this section remains PROVISIONAL
INTERNAL DIAGNOSTIC; no central product is modified; the central release remains
raw/no-bgsub/no-C_rad/not Born level.

## 9.4 Recommendation for a future S2M v3 (not executed)

Switch ibu_model from the MAD basis to the B1 floors (sigma_i = floor_B1,i), with
d values and all v2 hyperparameters frozen and the config re-hashed; the MAD
caveat for ibu_model then closes. Treat the Section 9.1 gate-fail bin (neg 130)
with the S2M self-value rule — carry its raw value through unsmoothed
(`excluded_from_fit`) with a provenance flag, exactly like the >50% bin — rather
than dropping it. Leave the heavy floor tail (45 bins with floor > 5%, max
16.05%) uncapped: those bins enter the WLS with weight 1/v and the smoother
naturally preserves their raw values; capping would fabricate structure the
bootstrap does not support. Re-run the G3 toy closure under the ibu_model B1
noise model (heteroscedastic, like the v2 detector model) before quoting any v3
number.
