# Numerical audit: individual covariance matrices — clean-release build (cold critic)
Date: 2026-10-18 | Auditor: h1-analysis-critic (numerical forensics; session could not write files — content delivered inline and transcribed verbatim by the supervisor; binary .npy spectra not independently recomputable in the critic sandbox — those verdicts rest on recorded gates plus cross-verification)

**VERDICT (package level): ANOMALY (MAJOR) — 1 MAJOR, 4 MINOR, 0 BLOCKER.**

| Matrix | Verdict | min eig | rank | max asym | unit check |
|---|---|---|---|---|---|
| C_stat h+ 245x245 (`ibu_toy_covariance_pos`) | SOUND (sampled) | >=0 by constr. (1000-toy sample cov; not computed) | <=999 by constr., gen. 245 (n/r) | 0.0 sampled, exact | counts^2 OK exact: diag(0)=1355.5271377^2, diag(100)=6.5127745^2; /351000^2 closure exact |
| C_stat h- 245x245 | SOUND (sampled) | >=0 by constr. | same | 0.0 sampled, exact | counts^2 OK exact: diag(0)=1308.730898^2, diag(244)=3.6192911^2 |
| corr twins h+/h- | SOUND (sampled) | n/a | n/a | 0.0 sampled; diag=1.0 exact (3 entries) | corr=cov/sigma(x)sigma exact at (0,1),(0,35); |rho|<=1 sampled |
| C_MCstat 490x490 (+rel twin) | SOUND (caveats; k100 seed-repeat diag +2.4%/+2.1% OK) | -1.86e-12/-9.79e-13, 0 neg @tol (recorded) | 199/245 per charge (=K-1, expected) | 0.0 (recorded) | counts^2 + dimensionless rel twin OK; integrated closes: 3,688,592.60/351000=10.5088 nb, 3,586,343.40/351000=10.2175 nb |
| S2 joint gen cov 490x490 | SOUND | +0.2654 (recorded, rel 2.3e-8) | n/r (deferred S9; N=1000>490) | 0.0 by constr. | counts^2 OK reconstruction: rho[0][0]=0.745 <= rec. max 0.778; sigma-excess +61% <= rec. max +86.2% |
| S2 cross-block 245x245 (+toy_se) | **ANOMALY (MAJOR)** — sound standalone; BLOCKER if S9 assembles raw under D4 | n/a (block of PSD 490x490) | n/m | 0.0 by constr. | counts^2 (no explicit units field); D4 assembly -> rho=1.93 @bin0 |
| S2 reco cov 980x980 | SOUND (recorded-level only) | n/r | n/r | n/r | counts^2; recorded cross-charge |rho| med 0.0214/max 0.6093, diag excess +3.5%/+114.6% |
| rank-1 signed deltas 490x12 (+10 mag, 2 env pairs, flags) | SOUND (sampled; elec D1 caveat) | n/a (rank-1 by constr.) | 12 cols; 21/21 budget cols <=1e-12 OK | n/a | nb=rel x nominal exact in 6-row census (~130 pairs); lumi flat 0.027 OK; re-anchor 1.8e-15 OK |

Anomaly counts: BLOCKER 0 - MAJOR 1 - MINOR 4.

MINOR findings:
1. `delta_elec_*_sym` = mean of +/-legs (D1 curvature-type), NOT the budget systematic — median 0.000427 vs env 0.017021 (40x collapse if S9 consumes sym as the elec systematic).
2. env one-leg-per-charge convention drops the large leg at edge bins (pos bin 179: selected -1% leg +5.0%, unselected +1% leg implied ~ +132%).
3. C_MCstat D5 zero cross-charge assumption unmeasured (same RAPGAP sample feeds both charges) + 12,207/11,834 fixed low-Neff body cells unfluctuated -> possible undercoverage, bounded by 0.85-0.92% diag.
4. mixed units counts^2/nb/relative across package; cross-block JSON lacks a units field.

SINGLE WORST FINDING (MAJOR): the D4 mixed-basis assembly is provably non-PSD. `cross[0][0] = 3,422,449.87 counts^2` (18.9 sigma vs its toy_se 181,254.50) seated next to sidecar marginals sqrt(1,837,453.82 x 1,712,776.56) = 1,774,225 -> **rho_assembled = 1.929**; 2x2 minor det = -8.57e12, eigenvalues (-1.65e6, +5.20e6) counts^2; asymmetry variance at bin 0 = Var+ + Var- - 2Cov = **-3.29e6 counts^2**. Root cause: joint event-cluster toys over-disperse marginals vs the Poisson-only sidecar (+5.1%/+4.9% median, +86.2%/+84.2% max on sigma scale = up to 3.5x variance); the toy machinery itself is consistent (joint-basis rho[0][0]=0.745 <= recorded 0.778). Consequences: S9 eigen/inversion/chi2 failure or negative variances, and the sidecar TIER_A diagonal understates stat uncertainty up to +86% sigma-scale in cluster-heavy bins (bin 0: +61%). FIX BEFORE S9: re-base the cross-block via the joint correlation onto sidecar marginals (`cross' = rho (o) sigma_sc sigma_sc^T`), or supersede D4 with the joint per-charge blocks (moves published diag ~+10% var-scale median). Decisive next check: run the S9 pre-assembly PSD gate on the D4-assembled 490x490 — it will fail at bin 0.
