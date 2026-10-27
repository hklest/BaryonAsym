# WP-C binning record: X-frame observables, boost, resolutions, grid and blinded mask (2026-10-27)

**Scope.** This record covers WP-C of the program plan
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
(D6, D7, D8; the WP-C block; Assumption 2). It contains the operational X-frame definition and
its gen-level equivalent, resolutions from the existing v6dif signal MC, per-bin purity and stability, the
bin proposal, the blinded-mask procedure, the T0-to-data acceptance-chain reconciliation, and the
`engine_shape` test. It contains no data histograms: no data ROOT file was opened.

**Paths.** `D/` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/`, `P/` =
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`. WP-C products sit under
`D/{docs,scripts,condor,data}/restart_2026-10-27/wp-c/`. The supervisor brief named `wp-c`, while
the plan's WP-C block writes `wpC`. This record follows the brief (decision C0 below).

**Tags.** **[V]** marks a value computed by the cited script or read from the cited file. **[I]** marks an inference.

## 1. Observable and X-frame definition

**Observable (binding, plan D6).** For charged hadrons (h⁺, h⁻ separately) the record measures
(1/N_D) dn/dx_F and (1/N_D) dn/dp_T*² in the rest frame of X, in M_X bins. The fiducial phase space is
Q² > 150 GeV², 0.1 < y < 0.7, x_IP < 0.03, M_Y < 1.6 GeV, |t| < 1 GeV² and 4 ≤ M_X ≤ 40 GeV. The hadron
fiducial is |η_lab| < 1.6 and 0.15 ≤ p_T ≤ 15 GeV for π/K/p, with V0 and conversion/Dalitz daughters excluded
(`D/scripts/t1_particle_level/particle_level.py:76-100`). ⟨β⟩ is quoted per M_X bin, and there is no β axis.

**Frame and axis.** The frame is the rest frame of p_X. The axis u is the direction opposite to the γ*
direction boosted into that frame (u = −q̂*). In the X rest frame γ* and IP are back to back by
construction, because p_X = q + p_IP, so u is the Pomeron direction whatever the Pomeron's transverse
momentum. Then x_F = 2 p*·u / M_frame (positive along the Pomeron/proton direction, the parent's
proton-positive convention, opposite to PLB 428 (1998) 206), and p_T*² = |p* − (p*·u)u|². Implementation:
`D/scripts/restart_2026-10-27/wp-c/study/xframe_obs.py` (`frame_obs` :87-105, u = −q̂* at :96, x_F at :99).

**Reco photon and masses.** The reco photon is the eSigma photon, `reco_photon` imported from
`P/scripts/unfolding/esigma_final_2026-10-24/esigma_xf.py:106`, and the reco hadron mass is the pion.
The gen photon is the ISR-only true γ* from `gen_truth` (same file :183), and the gen hadron mass is the PDG
mass from `gen_mass` (:60). The stored `xF_SIDIS_es`, `xFGen` and `z_SIDIS_es` are never read.

**Reco p_X: two estimators were studied.**

| Frame | p_X | frame mass | comment |
|---|---|---|---|
| A (D6 as written) | stored HFS `phE, phPx, phPy, phPz` (`GetAllFsLessElectron`, `P/src/ChargedSidisTreeBuilderHighQ2_v6dif.C:2535-2540`) | √(p_X²) | not corrected for forward losses; carries measured transverse imbalance |
| B (collinear) | q_es + ξP, with ξ the exact root of (q_es + ξP)² = M_X,rec² (`xframe_obs.py:65-77`, keeps m_p) | M_X,rec = DiffTot `mx` = √(phM2·Ys/yh) (writer :2545-2547) | longitudinal boost along the γ*p axis; uses the y/y_h-corrected mass |

**Gen-level equivalent.** The target definition is the rest frame of the true p_X = q_true + (P − p_Y).
The largest-gap split is the one the writer mirrors (`mir*`, writer :2730-2810) and that
`particle_level.py` defines. The true gen p_X 4-vector is not in the v6dif trees; WP-E adds it (plan D5).
Until then the interim proxy is G2: p_X = q_true + ξP, with ξ such that the mass equals `mirMX`.
G1, which uses ξ = `mirXip`, is a variant with mass² ≈ M_X² − t. G1 and G2 give identical p_T*² and differ
in x_F by RMS 0.022 (core 0). G1 fails `xf_range` strictly (1.1×10⁻⁴ of tracks are outside [−1, 1], up to x_F =
6.1), so G2 is the interim gen proxy **[V]** (`D/data/restart_2026-10-27/wp-c/study/diag1.txt`).

**M_Y and the proton are unmeasured.** Neither frame uses Y. The forward vetoes and η_max < 3.2 select
events in which Y escaped down the beam pipe. M_Y < 1.6 GeV and |t| < 1 GeV² are gen-level fiducial cuts only.
Events with M_Y ≥ 1.6 GeV or |t| ≥ 1 GeV² that pass the reco selection are fakes, handled as per-reco-bin
fractions from the PDISS MC (plan D8). Frame B assumes the Pomeron is collinear with P; this neglects
the t-recoil. The Pomeron p_T is ~√|t| ≈ 0.3-0.5 GeV, which tilts the axis by ~√|t|/M_X and shifts p_T*² by up to ~0.1-0.25 GeV²
for hadrons at |x_F| ~ 0.5. That is comparable to the first p_T*² bin, so it is **not** negligible at large |x_F| (h1-analysis-critic) **[I]**.

**Frame recommendation.** No frame is frozen (decision C1). B is the provisional nominal for the D7 grid; A keeps more structure under D7+ (§3). The evidence is in §2 and §3:
- B has a 1.5-2.6× smaller p_T*² core resolution at M_X > 10 GeV. Part of this is by construction: in B, q, P and p_X are collinear, so B's p_T*² is
  exactly the hadron p_T² about the γ*p axis in the hadronic CM frame and does not depend on ξ or M_X,rec (G1 and G2 give identical p_T*² for the same reason).
  B therefore ignores the HFS transverse imbalance and the t-recoil that A carries, and it was measured against a proxy with the same collinear definition.
- B has a smaller x_F core resolution at M_X > 10 GeV.
- Under D7, B yields more bins (28/charge against 26 for A). Under D7+ at reco x_IP < 0.03, A keeps 17 bins against 15 for B (§3).
- B's absolute biases are about 2-3× A's. The unfolding absorbs a bias, but a bias costs purity.
- Conditions (h1-analysis-critic, SOUND-WITH-CONDITIONS):
  - (a) the unfolding truth is the true X frame from WP-E; B's resolution and purity are re-derived against it before the grid is frozen;
  - (b) a t-slope (and PDISS-t) variation is a response-model systematic;
  - (c) A versus B tests response-model dependence only, not the truth definition;
  - (d) comparability with PLB 428 needs the true γ*-IP frame at particle level, never a B-defined particle level.

**Unit tests [V]** (`D/scripts/restart_2026-10-27/wp-c/study/wpc_tests.py`):
- `boost_closure` (p_X in its own frame → (0, 0, 0, M) within 1e-6): PASS. The maximum residuals are
  4.6e-13 (A, 10,186 events), 4.3e-14 (B; |p_X² − M²| ≤ 5.7e-12) and 1.1e-13 (G1).
- `xf_range` (gen, all tracks, −1 ≤ x_F ≤ 1): PASS for G2, with 0 tracks outside. G1 fails, as described above.
  At reco, |x_F| > 1 for 1×10⁻⁴ (A) and 2×10⁻⁴ (B) of the weighted tracks; these go to the edge bins.
- Sign: the weighted mean x_F is −0.045 (A), −0.034 (B) and −0.048 (G1). The photon (current) hemisphere
  dominates, which is consistent with Pomeron-positive x_F.

## 2. Resolutions (existing signal MC)

**Inputs.** The signal cocktail is 9012 + 9024 + 9033 + 9155/9157/9158, from the merged files under
`D/data/t2_detector/mc9012_merged_2026-10-18/` and `D/data/t2_detector/mc_merged_2026-10-18/`. 9012 ran
on the login node (one file). The other five ran on Condor, one job per file. The original build was cluster 5902088 with
`D/condor/restart_2026-10-27/wp-c/study/wpc_build.sub` (5/5 normal terminations). The current `s<ReqID>.npz` files, which feed every `_G2`
table and §2a, come from the round-1 rebuild: cluster 5902089 with `wpc_build_r1.sub` (5/5 return value 0), plus a login-node re-run of 9012.
The old fields are unchanged. (The `.sub` files sit in the plan's `condor/` tree.) Each event is weighted by w_event ×
scale_to_data × exposure_factor from `D/docs/mc/composite_scale_table_v3_2026-10-20.csv`, including the
PDISS ×5.3611 truth-in-cut rule and the charm g21 slice weights. The builder is `wpc_build.py`, and per-sample
outputs are `D/data/restart_2026-10-27/wp-c/study/s<ReqID>.{npz,json}`.

**Selection used [V].**
- Reco event:
  - ChargedTot Q2es > 150 GeV² and 0.1 < Yes < 0.7.
  - 35 < E−p_z < 70 GeV and |PVz| < 35 cm.
  - Stack C: etaMaxClus08 < 3.2, selFmd, ePlug < 7, selFts (`D/scripts/t4/build_localization_n1_mytbl_2026-10-20.py:115,140-141`).
  - DiffTot 0 < xIp < 0.04 and mx > 3 GeV (plan D4).
- Reco track: the parent release cuts (`P/scripts/unfolding/cone_s2prime_optA_2026-10-19/build_migration_th2_per_charge_cone_s2prime_optA_esxf.py:665-669,1350-1360,1423`):
  - p_T > 0.15 GeV and |η| < 1.6.
  - NHitDedx ≥ 5, RadLength ≥ 5 and |DcaPrime/cosh η| < 2.
  - IsFromPrimary, and central or combined track.
  - Electron-track opening angle ≥ 0.2 rad.

**Event-level resolution [V].** The table gives (reco − gen)/gen for selected, gen-fiducial events,
weighted, with gen = `mir*`. The core σ is a 2σ-clipped Gaussian width.

| Quantity | M_X,gen 4-10 bias / core σ / RMS | 10-20 | 20-40 |
|---|---|---|---|
| M_X (`mx`, y/y_h-corrected) | −0.040 / 0.152 / 0.464 | −0.116 / 0.145 / 0.182 | −0.156 / 0.124 / 0.135 |
| M_X (√phM2 alone) | −0.028 / 0.170 / 0.494 | −0.109 / 0.161 / 0.198 | −0.156 / 0.138 / 0.151 |
| x_IP | +0.011 / 0.105 / 0.229 | −0.082 / 0.120 / 0.152 | −0.170 / 0.141 / 0.157 |
| β | +0.025 / 0.049 / 0.128 | +0.121 / 0.141 / 0.193 | +0.255 / 0.203 / 0.305 |

- In absolute terms, x_IP has a bias of −0.0019 and a core σ of 0.0020, and β has a bias of +0.054 and a core σ of 0.062.
- M_X is biased low by 12-16% at M_X > 10 GeV because of forward losses. The y/y_h correction helps (core σ 0.138
  against 0.152 overall), and `mx2Ye` is marginally better (0.135).
- `hatMXGen` equals `mirMX` to 1e-3 in 100% of events. `hatXpom` agrees with `mirXip` to within 1e-3 in only 0.3% of events,
  so the two x_IP definitions differ. The fiducial uses `mirXip` (`particle_level.py` definition).
- The bins [4,10), [10,20) and [20,40] are 5-7σ_core wide in M_X, so the M_X binning is not resolution-limited. Its purity is
  set by the ~12-16% low bias at the 10 and 20 GeV edges and by the RMS tail at low M_X **[I]**.

**Track-level x_F resolution [V]** (matched tracks, reco − G1 in x_F units, M_X bin from gen):

| M_X bin | A bias / core σ / RMS | B bias / core σ / RMS |
|---|---|---|
| 4-10 | +0.004 / 0.021 / 0.078 | +0.028 / 0.021 / 0.099 |
| 10-20 | +0.004 / 0.013 / 0.050 | +0.015 / 0.010 / 0.054 |
| 20-40 | +0.003 / 0.008 / 0.031 | +0.010 / 0.006 / 0.026 |

**Track-level p_T*² resolution [V]** (relative core σ in the start-grid p_T*² bins
[0,0.25), [0.25,0.75), [0.75,2), [2,10) GeV²; `diag1.txt:61-85`):

| M_X bin | A | B | B absolute bias (GeV²) |
|---|---|---|---|
| 4-10 | 0.29 / 0.23 / 0.21 / (n=5) | 0.23 / 0.18 / 0.36 / (n=5) | +0.10 / +0.19 / +0.41 |
| 10-20 | 0.24 / 0.21 / 0.22 / 0.17 | 0.10 / 0.12 / 0.13 / 0.11 | +0.06 / +0.09 / +0.22 / +0.10 |
| 20-40 | 0.18 / 0.15 / 0.19 / 0.16 | 0.07 / 0.07 / 0.09 / 0.10 | +0.02 / +0.05 / +0.04 / −0.09 |

- B's p_T*² is biased upward at low M_X (+0.10 GeV² in the lowest bin at 4-10 GeV, the full width of the
  proposed first bin). It cannot come from `mx`, because B's p_T*² is independent of ξ (above). The candidate sources are the eSigma
  photon direction and the track or pion-mass treatment; the decomposition is in §2a.
- Gen x_F is concentrated near zero. The 1/50/99 % quantiles are −0.64/−0.03/+0.53 at M_X 4-10 and
  −0.47/−0.02/+0.16 at M_X 20-40, and most p_T*² values lie below 0.3 GeV² **[V]**. This is why the proposal concentrates bins around x_F ≈ 0.

**Track matching [V].** `GenMatchIdx` is filled only for tier-2 matches (115 of 84,502 rows in 9012;
writer ~:1985-2056). The analysis therefore uses the matched-gen kinematics stored on the reco row
(`GenPt/GenEta/GenPhi/GenPDG`, `IsMatched`, `IsMatchedHadron`) and does not join to ChargedTotGen.

### 2a. Origin of the frame-B p_T*² bias [V]

Each row below changes one ingredient on matched, gen-valid tracks. The bias is reco − G2 in GeV², shown as the mean over all tracks
and as the mean in the gen p_T*² < 0.25 GeV² bin. The script is `wpc_r1_diag.py` and the output is
`D/data/restart_2026-10-27/wp-c/study/diag_r1_F3F4F5.txt`.

| M_X bin (GeV) | baseline | (i) gen photon | (ii) gen track | (iii) gen mass on reco track |
|---|---|---|---|---|
| 4-10 | +0.130 / +0.101 | +0.003 / +0.002 | +0.131 / +0.103 | +0.131 / +0.101 |
| 10-20 | +0.073 / +0.059 | −0.001 / +0.002 | +0.074 / +0.058 | +0.073 / +0.058 |
| 20-40 | +0.028 / +0.020 | −0.003 / +0.001 | +0.031 / +0.020 | +0.029 / +0.020 |

- The bias comes entirely from the reco eSigma photon direction. Track momentum and mass play no part.
- The medians are ≤ 0.005 GeV², so the mean comes from tails. These are most likely radiative events, in which the reco photon differs
  from the ISR-only true γ*. That is **[I]**: the tails were not split by ISR tag (open item W5).
- The same photon enters frame A, so the photon tails are a common reco-photon issue, not specific to B **[I]**.

## 3. Statistics, purity, stability and the bin proposal

**The "~84k reco signal tracks per charge" assumption does not hold [V].** The 9012 ChargedTot has 84,502
rows in total before any cut, and 15.7k tracks survive the selection (7,885 h⁺ / 7,801 h⁻). The full cocktail has
79.6k raw / 79.1k weighted selected reco tracks per charge. Of these, **30.8k raw / 47.2k weighted (h⁺)** and
30.5k / 46.9k (h⁻) are gen-valid signal tracks. The acceptance chain (§5) independently gives ~40-44k
(elastic acceptance applied to the full cocktail). The predicted sample is therefore ~47k signal tracks per charge at
data luminosity, about 55% of the planned 84k. The weighted composition is ~60% elastic 9012, 28% charm and 11%
PDISS. N_D (selected events, weighted / raw) by reco M_X bin is 4,562 / 3,537 (4-10), 11,623 / 9,657 (10-20) and
7,041 / 8,889 (20-40).

**Definitions (as D7 is applied here).**
- Purity: N(reco and gen in the same 2D (M_X, axis) bin) / N(all reco-selected tracks in the bin). Fakes are in the
  denominator, which makes this the conservative reading of D7.
- Stability: N(same bin) / N(gen-valid tracks in the bin that are reco-selected anywhere).
- n_sig_pred: the weighted count of gen-valid reco tracks at 317,212.8 nb⁻¹.
- n_mc_raw: the unweighted count of the same tracks, pooled over samples.

**Start grid (plan WP-C block) [V].** Under D7 the start grid masks 26 of 60 (frame B) or 23 of 60 (frame A)
gen bins (both charges) (`D/data/restart_2026-10-27/wp-c/mask/mask_start_frame{A,B}_G2_D7.json`; the G1 version gives the same counts).
- Purity is the binding criterion. It is 0.25-0.50 in x_F (one empty bin excluded) and 0.03-0.47 in p_T*², and the top two p_T*² bins fail.
- Stability is 0.38-1.0.
- The outermost x_F bins fail N ≥ 50 at high M_X.

**Fakes limit purity [V].** 32.5% of selected signal-event weight lies outside the gen fiducial. Of those events,
82% have mirXip ≥ 0.03, 18.5% have M_X > 40 GeV, 11% have true Q² < 150 GeV², 9% have M_Y ≥ 1.6 GeV and 2% have |t| ≥ 1 GeV²
(the categories overlap). The fake fraction per bin is 0.15-0.35 at M_X < 20 GeV and 0.5-0.6 at 20-40 GeV. The cause is the
reco x_IP < 0.04 window against the 0.03 fiducial, combined with the −8 to −17% x_IP bias at M_X > 10 GeV. This is a
WP-B input: tightening reco x_IP would raise purity (open item W1).

**Gen proxy used for the tables.** All proposal and mask tables below use the interim proxy G2 (critic round 1 found that the first
tables used G1). The `_G2` CSVs supersede the un-suffixed ones, which are kept as provenance. The resolution tables in §2 are against G1.
G1 and G2 give identical p_T*² and an x_F difference with RMS 0.022 and core 0, so those tables are unaffected for p_T*² and nearly so for x_F.

**Two proposal families [V]** (`wpc_r1_propose.py`; printout `diag_r1_propose.txt`; use only its last six blocks, since the earlier ones are stale). Both use a greedy
merge of a fine candidate grid. The CSVs add three columns:
- `p_mig` = N(same bin)/N(gen-valid reco in bin), the migration purity;
- `n_mc_raw_minreco` = the minimum raw MC count over the four reco sub-bins (2× split in M_X at edges 4, 7, 10, 15, 20, 30, 40 GeV, and 2× in the observable), over both charges;
- `pass_D7plus`.

**(P1) D7 literal, reco x_IP < 0.04, frame B: 28 gen bins per charge** (`binstats_proposal_frameB_G2.csv`).

| M_X bin (GeV) | x_F edges | p_T*² edges (GeV²) |
|---|---|---|
| [4, 10) | −1, −0.4, −0.2, −0.05, 1 | 0, 10 (one bin) |
| [10, 20) | −1, −0.5, −0.4, −0.3, −0.2, −0.15, −0.1, −0.05, 0.05, 0.2, 1 | 0, 0.05, 0.15, 0.3, 10 |
| [20, 40] | −1, −0.3, −0.15, −0.1, −0.05, 0.05, 1 | 0, 0.3, 0.75, 10 |

- Every bin passes D7: minimum purity 0.350, minimum stability 0.468. The grid is fragile, though: 35 of 56 bins sit below 0.40, and
  f_fake reaches 0.59 (0.50-0.59 in most M_X 20-40 bins).
- Many edge bins have 0-5 raw MC tracks in at least one reco sub-bin.
- 40 of 56 bins fail D7+.
- **h1-unfolder verdict: REJECT as it stands.** The f_fake ≥ 0.5 bins violate the plan's own WP-H test `fake_fraction_bounds: 0 ≤ f_i < 0.5`,
  and the reco-bin MC statistics are too low for a stable response.
- Frame A under the same rule gives 26 bins (`binstats_proposal_frameA_G2.csv`).

**(P2) D7+ (h1-unfolder conditions).** A bin passes when all of these hold:
- n ≥ 50;
- stability ≥ 0.35;
- D7 purity ≥ 0.35;
- p_mig ≥ 0.40;
- f_fake ≤ 0.35;
- ≥ 100 raw MC tracks in every reco sub-bin;
- width ≥ 2σ_core.

At most 6 x_F and 4 p_T*² bins are allowed per M_X bin. The rule was run for three reco x_IP windows.

Counts are read from the hashed per-charge masks (C7).

| Variant | signal tracks/charge (raw / weighted) | gen bins/charge | passing h⁺ / h⁻ | f_fake range | CSV |
|---|---|---|---|---|---|
| B, x_IP < 0.040 | 30.8k / 47.2k | 11 | 4 / 4 | 0.22-0.59 | `binstats_proposal_D7plus_xip040_frameB_G2.csv` |
| B, x_IP < 0.030 | 30.4k / 46.8k | 15 | 5 / 5 | 0.19-0.52 | `..._xip030_frameB_G2.csv` |
| B, x_IP < 0.025 | 28.5k / 44.6k | 15 | 8 / 7 | 0.17-0.38 | `..._xip025_frameB_G2.csv` |
| A, x_IP < 0.030 | 30.4k / 46.8k | 17 | 7 / 7 | 0.08-0.52 | `..._xip030_frameA_G2.csv` |

- The reco x_IP cut removes mostly fakes. On weighted counts, 0.030 costs 0.9% of signal and 14% of all selected tracks, and 0.025 costs 5.6% of signal
  and 25% of all tracks. The raw signal costs are 1.3% and 7.5%.
- Under D7+ the x_F axis collapses to one bin at M_X < 20 GeV in frame B (both tighter windows). Only M_X 20-40 keeps
  [−1, −0.05, 0, 0.05, 1].
- Frame A at x_IP < 0.03 keeps the most structure: x_F [−1, 0.1, 0.3, 1] at M_X 10-20 and [−1, 0, 0.05, 1] at M_X 20-40, plus p_T*² [0, 0.05, 0.15, 0.3, 10] at 10-20.
- **What binds [V].** Three things fail in practice:
  - `n_mc_raw_minreco` in the x_F tails and at high p_T*² (0-17 raw tracks in the M_X 4-7 or 30-40 sub-bins);
  - f_fake > 0.35 at M_X 20-40;
  - p_mig only at the x_F/p_T*² edges.
  
  The response MC statistics are the main limit. 9012 has L_MC = 52,273 nb⁻¹, 0.16 × the data luminosity, and the whole cocktail provides
  ~30k raw gen-valid tracks per charge.

**Lead assessment [I].** Neither family is a defensible frozen grid yet.
- P1 has the statistics on paper but fails the unfolding conditions.
- P2 can be unfolded where it passes, but it loses the x_F shape below M_X = 20 GeV, which is the core of the observable.
- The options go to the user (U1).

## 4. Blinded-mask procedure

1. **Rule.** The pre-registered rule is plan D7: a bin is masked if n_sig_pred < 50, or purity < 0.35, or stability < 0.35, with all
   inputs from the signal-cocktail MC at the ruled exposure. D7+ (§3) is optional and is not adopted without U1.
2. **Order.** The mask is derived and hashed before any data histogram in that bin is filled. WP-C opened no data ROOT file
   (`data_read: false` in every JSON).
3. **Artefact.** `D/scripts/restart_2026-10-27/wp-c/mask/make_blind_mask.py <per-bin CSV> <mask.json> [D7|D7plus]` writes the JSON and a
   `.sha256` sidecar. The JSON records the rule string, the source table's sha256, the creation time, and for each bin the masked flag
   with reasons and a `marginal` flag (passes D7 with purity or stability < 0.40).
4. **Current masks [V]** under `D/data/restart_2026-10-27/wp-c/mask/`. All are on G2. The first-round G1-based masks
   (`mask_{start,proposal}_frame{A,B}.json`) are superseded and kept as provenance.

| Mask | rule | sha256 | masked / bins (both charges) |
|---|---|---|---|
| `mask_proposal_frameB_G2_D7.json` (P1) | D7 | `73cd9efdd3ee0bde7baf75ce2a3bba5a79d8a34fa8ebc1b5025f21208bc7b4f6` | 0 / 56 (35 marginal) |
| `mask_proposal_frameB_G2_D7plus.json` (P1 under D7+) | D7+ | `281661419a3e175706bd85b40015c6249ff1bb3a4eef578892c60994906e14fe` | 40 / 56 |
| `mask_D7plus_xip025_frameB_G2.json` (P2) | D7+ | `d887f9766694c22739478262b31c6c6eae52b9deb9108bcc1e0845d2d1b8e277` | 15 / 30 |
| `mask_D7plus_xip030_frameB_G2.json` | D7+ | `c89fd2b65c7062d88fc18219552286880776d14373d08bd07bccda410bcb83d1` | 20 / 30 |
| `mask_D7plus_xip030_frameA_G2.json` | D7+ | `d83271394abf51250c985542ece0d1f80c643685ea2000d9103378bd060c2c05` | 20 / 34 |
| `mask_D7plus_xip040_frameB_G2.json` | D7+ | `f103d3b678813073237d3b4a0142ee644e3243573f57239918c0adb449290d1f` | 14 / 22 |
| `mask_proposal_frameA_G2_D7.json` | D7 | `c48a5225b8f1b12e36affe9099bd7f3266561a36d0c0f68dfb5207ad74e6c888` | 0 / 52 |
| `mask_start_frameB_G2_D7.json` (start grid) | D7 | `78b8a5c0901e5066f132b957412d41d2545cfff534a28e7027d020cfbfece8ca` | 26 / 60 |
| `mask_start_frameA_G2_D7.json` (start grid) | D7 | `ad61ce7a9eef0a84638f308fae7f43bbc895a1afca4905451d85ca2395ff45d8` | 23 / 60 |

5. **Later stages.** WP-F adds f_ND and f_reg to the same rows, and the G2 gate ceiling masks further bins. WP-H/J add the closure and
   systematic masks. Every re-derivation writes a new dated JSON and hash and never overwrites an old one. The final mask is ratified
   by the user at WP-K (plan D7).
6. **Status.** `mask_blind` passes for every mask above: each hash was recorded with no data read. None of the masks is frozen. The
   grid is re-derived on the WP-B selection and after item U1 (decision C4).


## 5. Acceptance-chain reconciliation: T0 (~130.6k) to LRG data (12,583)

The script is `D/scripts/restart_2026-10-27/wp-c/accept/cutflow_9012.py` and the output is `D/data/restart_2026-10-27/wp-c/accept/cutflow_9012.json`.

**The two numbers count different things [V].**
- **130.6k** (`D/docs/t0/t0_yield_prediction_2026-10-18.md:37-43`) counts particle-level fiducial signal **events**
  (M_X > 2 GeV, before any detector cut). It sums four periods: elastic uds 93,469 + charm 32,291 + PDISS 4,805. It uses
  the superseded equal-period fill 4 × 6.714705 = 26.8588. The binding exposure ruling
  (`D/docs/mc/exposure_convention_ruling_2026-10-19.md` §1-2) gives 6.0683, a factor 0.2259, so 130.6k corresponds to
  **~29.5k** certified four-period events.
- **12,583** (`D/docs/t2/lrg_selection_study_2026-10-18.md:74-78`) counts **raw data events** in **em0405 only** (e⁻), before run
  certification (103,398 nb⁻¹ snapshot). The cuts are the parent proxy (Q² > 150 GeV², 0.1 < y < 0.9, |vtx| < 35 cm, **no E−p_z**) and the
  four LRG vetoes, with no x_IP or M_X cut. It is not a signal yield.

**Chain for 9012 elastic, em0405 pre-certification exposure [V].** The weight is w = 103,398.4 / 52,273.3 = 1.978 per 9012 event.

| Step | N_MC | em0405 events | factor |
|---|---|---|---|
| gen fiducial (from the T0 doc: 93,469/26.8588; not a script output) | 3,480 | 6,884 | — |
| reco envelope + parent proxy (electron, trigger, vertex, Q²/y migration) | 2,154 | 4,261 | 0.619 |
| + η_max < 3.2 | 1,933 | 3,824 | 0.897 |
| + FMD | 1,729 | 3,420 | 0.894 |
| + Plug < 7 | 1,719 | 3,400 | 0.994 |
| + FTS (LRG candidate and gen fiducial) | 1,673 | 3,309 | 0.973 |
| LRG candidate, no truth cut | 2,863 | 5,663 | ×1.711 (fakes and migration in; 58.4% truth-fiducial) |

**Comparison with the data count [V].**
- Elastic candidates scaled to the full cocktail with the elastic share 0.7159, plus the Reggeon (+2.7%), give **~8.1k**
  em0405 candidates against **12,583** in data. Prediction/data is **0.65**.
- The deficit grows with the Q² cell: prediction/data is 0.71 / 0.57 / 0.36 in D0/D1/D2. This follows the ND-leakage trend
  (3.7/10.8/40.6%, `D/docs/t4/localization_verdict_2026-10-20.md:62`) qualitatively.
- The independent certified stage-B check gives signal-only MC/data = 0.738 (ruling §4).
- The lumi bookkeeping closes: 12,583 × 317,213/103,398 = 38.6k against 37,302 certified stage-B, a 3.5% difference.

**Conclusion.**
- There is no contradiction in kind. The ×10.4 gap (130.6k/12,583) decomposes as follows:
  - exposure convention ×1/4.43 (130.6k → 29.5k);
  - em0405 share of the four periods ×0.326 (→ 9.62k = 6,884/0.7159);
  - candidate acceptance including fakes ×0.82 (= 5,663/6,884);
  - Reggeon ×1.027 (→ 8.1k);
  - data/prediction ×1.55 (prediction/data 0.65).
- A real residual of **~30-35% of the raw LRG data** is not covered by signal + Reggeon. The most likely sources are ND
  leakage and the background left by the missing E−p_z cut in that study (data y < 0.9 against the gen y < 0.7 fiducial) **[I]**.
  The chain scales the elastic fake ratio (×1.711) to the cocktail, so it under-counts PDISS events with M_Y > 1.6 GeV that pass the
  vetoes. In the full cocktail, selected PDISS events with M_Y ≥ 1.6 GeV are 21% of PDISS and ≈ 2.6% of all selected events
  (`D/data/restart_2026-10-27/wp-c/study/diag_r1_F3F4F5.txt`, F4). That is small next to the residual **[I]**.
- The residual rises with Q² and so is not a flat signal normalisation offset. It is the problem that plan gates G1/G2 already
  address (gates, not the gen proxies of §1), not a new one.
- The chain is model-dependent: charm and PDISS acceptance are set equal to elastic. The ±~10% this introduces is not separated.

**Fiducial caveat [V].** The chain's truth fiducial in `cutflow_9012.py:11` uses `hat*` variables (hatMYGen, hatXpomGki, hatMXGen > 2, hatTGki) with
no true-Q²/y cut. It is not the analysis fiducial of §1 (`mir*`, true γ*, M_X ∈ [4,40] GeV), so the 0.619 step mixes the T0 fiducial with the
script fiducial, and the 58.4% and 14.2k figures are approximate.

**Implied signal yield [I, approximate].** ~14.2k LRG-selected, truth-fiducial signal events in the four certified periods, and
~40-44k reco signal tracks per charge from the chain. This agrees with the 47k weighted count of §3. The chain applies elastic
acceptance throughout, while charm and PDISS have higher multiplicity **[I]**.

## 6. `engine_shape` test (plan Assumption 2) — PASS [V]

- **Engine.** `cbo.import_engine()` resolves to `P/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`
  (md5 `6f735fb6088237db77ff7514bd158f92`; `derive_response_blocks` :516, `ibu_unfold` :623).
- **Orientation.** `M_ext` is (NB_R+1, NB_G+1): reco rows, gen columns, the miss row last and the fake column last.
- **Hard-coded sizes.** The two functions and their helpers contain none. The 240 constants (:177, :192, :195) feed only the publication layout.
- **Shape string.** The plan's Tests line "(31,61)" is transposed. The correct shape for 30 gen / 60 reco bins is **(61, 31)**. The
  Assumption text "(NB_R+1, NB_G+1)" is correct.
- **Toy results.** These are Asimov toys (efficiency ~0.6, 10% fakes, purity 0.64-0.76).

| Case | Shape | k=4 | k=50 |
|---|---|---|---|
| (a) MC prior = truth | (61, 31) | 6.7e-16 | 6.7e-16 |
| (b) flat MC, truth sloped ±30% | (61, 31) | 2.1e-3 | 7.3e-7 |
| (c) MC prior = truth | (45, 17) | 4.4e-16 | 4.4e-16 |
| (c) flat MC, truth sloped ±30% | (45, 17) | 1.9e-3 | 2.5e-7 |

- **Pass criterion.** The plan's criterion is case (a), and it passes. Case (b) is regularisation bias at k=4, reported only.
- **Engine assumption.** The engine assumes the data fake fraction equals the MC f_i. With f_fake up to 0.6 (§3), the f_fake model
  becomes a leading systematic for WP-H/J **[I]**.
- **Files.** The script is `D/scripts/restart_2026-10-27/wp-c/engine_shape/engine_shape_test.py` and the result is
  `D/data/restart_2026-10-27/wp-c/engine_shape/engine_shape_result.json`.

## 7. Decision log (WP-C lead)

- C0: outputs go under `wp-c/`, as in the supervisor brief, not the plan's `wpC/`. The brief's path is binding; WP-E/F/H must reference `wp-c`.
- C1 (revised in round 1): neither reco frame is frozen. B (collinear) is the provisional nominal for the D7-literal grid P1; A (HFS
  4-vector) keeps more x_F structure under D7+. Both are carried, and the choice is made against the true WP-E X frame. B's p_T*² advantage is
  partly circular (h1-analysis-critic), and A is the better D7+ grid at x_IP < 0.03. This departs from D6's "Reco p_X = stored HFS 4-vector"
  only in carrying B; the gen target is unchanged.
- C2: the interim gen proxy is G2 (mass matched to `mirMX`), not G1 (ξ = x_IP), because G1 violates `xf_range`. All grids and masks were
  regenerated on G2 in round 1. G2 is used for x_F binning scouting only, with no p_T*² closure claim. Gen diffractive variables come from
  `mir*`, not `hat*`.
- C3 (revised): D7 purity keeps fakes in the denominator, as pre-registered. p_mig and f_fake are reported beside it, and D7+ is offered as
  an extension. A stricter rule changes a pre-registered threshold and therefore goes to the user (U1).
- C4: no grid or mask is frozen. Re-derivation follows the WP-B selection and U1. Each hash proves blindness, not adoption.
- C5: N_D, n_sig_pred and the weights use the ruled exposure convention (scale × exposure_factor, 6.0683 for 9012), not the recipe text's
  26.8588, because the ruling supersedes that convention.
- C6: E−p_z 35-70 GeV is used as briefed. The parent release builder uses E−p_z > 45 GeV with no upper cut (~6% fewer events), so the
  choice is left to WP-B (W2).
- C7: D7+ masks are applied per charge, not as a common pass for both charges, because D7 is a per-bin rule. The worker's
  common-pass flag differs by one bin.

## 8. Open items, risks and coverage gaps

- **U1 (user: binning and mask rule).** The options are:
  - (a) the D7-literal 28-bin grid P1 (h1-unfolder REJECT; f_fake up to 0.59 violates WP-H `fake_fraction_bounds`);
  - (b) D7+ with a tighter reco x_IP (≈ 7-8 bins/charge pass, with x_F integrated below M_X 20 GeV in frame B);
  - (c) D7+ with reco bins split only along the observable axis, plus a ≥ 5× elastic signal-MC request (Open Q3) — not yet evaluated;
  - (d) a coarser fixed physics grid of about 3 x_F × 3 p_T*² per M_X bin, evaluated under D7+.
  
  The lead recommends (c), with (d) as the fallback. The binding constraint is response-MC statistics (9012 has 0.16 × the data
  luminosity), not resolution.
- **W1 (WP-B).** The reco x_IP window (< 0.04) against the 0.03 fiducial drives f_fake to 0.15-0.6. A window of 0.025-0.03 removes mostly fakes
  at a weighted signal cost of ≤ 6%.
- **W2 (WP-B).** The E−p_z window differs between the plan (35-70 GeV) and the parent release (> 45 GeV).
- **W3 (WP-H).**
  - The per-reco-sub-bin f_fake was not evaluated.
  - Track NucliaWeight and TrigWeight were not applied.
  - The PDISS recipe's `calcQ2s/calcYs` were taken as `Q2sGen/YsGen` without confirmation.
  - Systematics are needed per source (h1-unfolder): the x_IP 0.03-0.04 band and Reggeon, PDISS normalisation ±50%, the M_Y spectrum and t-slope,
    M_X > 40 GeV feed-down, and Q² < 150 GeV² feed-in. f_i should be constrained from reco x_IP sidebands.
  - N_D is unfolded with its own event-level response and the ratio taken afterwards, with an event-level bootstrap shared by numerator and
    denominator. Fiducial events with zero tracks count in N_D.
- **W4 (plan text, for the supervisor).** Three statements need correcting:
  - "~84k reco signal tracks per charge" (~47k weighted, ~30k raw predicted);
  - the `engine_shape` shape "(31,61)" (should be (61,31));
  - D6's reco p_X (both frames carried, per C1).
- **W5 (WP-E/H).** The frame-B p_T*² tails come from the reco eSigma photon (§2a). They should be split by ISR tag, and the same check applied to frame A.
- **Risk R-a.** t-recoil effects reach ~0.1-0.25 GeV² in p_T*² at |x_F| ~ 0.5, which is comparable to the first bins. Every p_T*² purity number
  here is against a collinear proxy and must be redone against the true X frame from WP-E.
- **Risk R-b.** About 30-35% of the raw LRG data is not covered by signal + Reggeon (§5). Plan gates G1/G2 already address this.
- **Gaps.**
  - X/Y membership of matched gen hadrons was checked only through the gap midpoint.
  - The true gen X frame awaits WP-E.
  - No resolution plots were produced. The numbers are in `D/data/restart_2026-10-27/wp-c/study/diag{1,2}.txt`, `diag_r1_F3F4F5.txt` and `diag_r1_propose.txt`.
