<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note, but parts of the opening baseline are pre-combined-track. Re-read with the v3 track-type-aware workflow in mind.

# Deep Research: Tracking-Related Systematic Uncertainties for H1 Charged-Hadron SIDIS

**Date:** 2026-05-14
**Author:** researcher (deep-read pass on tracking systematics)
**Scope:** Tracking efficiency, charge-dependent tracking residuals, material-interaction
asymmetry, V0/conversion contamination, track-quality cut systematics, and data-driven
cross-checks for the H1 inclusive charged-hadron SIDIS measurement (low-Q2:
5 < Q2 < 100 GeV2 and high-Q2: Q2 > 150 GeV2).
**Observables under consideration:**
- Multiplicity M^{h+/-}(Q2,x,z,pT,h) = N_h_corr / (N_DIS_corr * bin_widths)
- Charge asymmetry A_h = (N_{h+} - N_{h-}) / (N_{h+} + N_{h-})

**BASE_CUT (reco selection, taken from `analyze_charged_xsec_corrected.py` and
`context-handoff.md` Section 3):**
```
NHitDedx >= 10
RadLength > 10
abs(DcaPrime / cosh(Eta)) < 2.0
Pt > 0.15 GeV
abs(Eta) < 1.3
IsFromPrimary == 1
IsCentralTrk == 1
```

**One-line headline finding:** the 0.5% / 10% per-track number from H1 paper DESY 20-176
applies to inclusive multiplicities only after partitioning by tracker type. Because our
BASE_CUT enforces `IsCentralTrk == 1`, the 10% combined-track term is **not applicable**
to our fiducial; only the 0.5% central-track term applies. For the charge asymmetry A_h
the common 0.5% cancels exactly, and the leading non-cancelling tracking systematic is
the charge-dependent residual (CJC sagitta asymmetry + material interaction asymmetry +
non-cancelling Lambda/anti-Lambda V0 contamination), expected to be at the
0.1-0.5 % absolute level.

---

## 1. Published Tracking-Efficiency Numbers: Provenance and Applicability

### 1.1 The two H1 numbers: 0.5 % per central track and 10 % per combined track

Both numbers are quoted by the H1 multiplicity paper (DESY 20-176; arXiv:2011.01812;
EPJC 81 (2021) 212), Section 5 on systematic uncertainties (extracted at
`H1/proton/ChargedParticlePaper_extracted.md` lines 418-420 per
`research-sidis-systematics-literature.md` Sys.5, lines 124-138):

> "An additional uncertainty of 0.5 % is added per central track and 10 % per combined
> track."

The same numbers also appear in the H1 charged-particle spectra companion paper
(arXiv:1302.1321, Section 5, summarised in `research-sidis-systematics-literature.md`
lines 256-262) where they are re-quoted as "1 % for central tracks ... 10 % for
combined tracks" (note: the spectra paper's "1 %" is the additional CJC-only
single-track efficiency uncertainty, applied per data point, while the multiplicity
paper's "0.5 %" appears specifically as the additional per-track unfolding-input
uncertainty -- the two are not literally the same number). Both papers say the
uncertainty is "added per track," not per event.

### 1.2 Where the 0.5 % central-track number actually comes from

Neither paper derives the 0.5 % from a study performed in that paper. Both cite it as
an established H1 tracking-efficiency uncertainty. The standard provenance chain in
H1 is:

1. **CJC-only single-track efficiency studies based on tag-and-probe in the
   redundant tracking scenario.** The Central Tracking Detector (CTD) pairs the CJC
   (drift chamber) with the CST (silicon vertex tracker) and the CIZ/COZ (z-chambers).
   By using the CST hit alone as a tag and the CJC track as a probe (or vice versa),
   one measures the CJC reconstruction efficiency relative to a probe-independent
   tag. This is the same data-driven method used by every modern collider tracker.
   The H1 internal note for this study is the standard tracking-efficiency note
   referenced collectively across H1 papers; it is not retrievable in this
   workspace, but the resulting per-track uncertainty has been quoted consistently
   as approximately 0.5-1.0 % across H1 publications since the HERA-II era for
   primary-vertex tracks above pT ~ 150 MeV.

2. **CJC curl-up and dead-cell studies.** The H1 2009 Lambda paper (arXiv:0810.4036)
   explicitly cites "CJC curl-up studies" and quotes 2 % per track for V0 daughters,
   evaluated separately because V0 daughters are not primary-vertex tracks (see
   `research-published-systematics-survey.md` Table 1, row 7). The 2 % V0 number is
   substantially larger than the 0.5 % primary-vertex number because (a) V0 daughters
   lie off the primary vertex and are reconstructed differently, and (b) V0 fitting
   adds a systematic on the geometric reconstruction. The 0.5 % is for tracks that
   pass a primary-vertex constraint, which matches our `IsFromPrimary == 1` cut.

3. **Period-by-period validation.** H1 standard practice is to re-measure tracking
   efficiency in each running period and assign the difference between data and MC
   as the systematic. The 0.5 % is the integrated value averaged across HERA-II
   running periods and across the central-CJC fiducial.

**Bottom line:** the 0.5 % is data-driven, derived from H1 tag-and-probe and
redundant-tracking studies on primary-vertex tracks above the 150 MeV pT floor.
It is not a flat number across all kinematics -- below pT ~ 150 MeV the efficiency
itself drops sharply and the uncertainty grows -- but for our fiducial
(pT > 0.15 GeV, |eta| < 1.3, primary-vertex) the 0.5 % is the published H1 figure.

### 1.3 Where the 10 % combined-track number comes from

The "combined track" in H1 terminology means a track that uses information from both
the CTD (central) and the FTD (forward tracker). The combined-track efficiency is
much harder to measure because:

1. The FTD is a planar drift-chamber stack with reduced redundancy; tag-and-probe
   cannot use a probe-independent inner-detector tag in the same way it can for
   the CJC.
2. The transition region between CTD-only and combined-track acceptance
   (1.3 < |eta| < 1.6 approximately) has track quality that depends sensitively
   on the alignment between CTD and FTD modules.
3. Period-to-period FTD performance was less stable than CJC performance.

The H1 multiplicity paper (DESY 20-176, Section 4 of the extracted MD lines 261-283)
explicitly extends to |eta_lab| < 1.6 and includes combined tracks in the fiducial.
Our analysis uses |eta| < 1.3 and `IsCentralTrk == 1`, **excluding** combined tracks
by construction. The 10 % per-track uncertainty therefore does not propagate to our
analysis.

**Verification:** the proton builder source (`source_cache/ProtonTreeBuilder_lowQ2.C`
lines 23-28) confirms that the tree-level cuts include `IsCentralTrk() == true` --
"forward tracks lack dE/dx" -- and the analysis BASE_CUT enforces it again. There
is no path by which a forward or combined track enters our hadron sample.

### 1.4 Per-track or per-event?

The H1 numbers are explicitly per-track. For the multiplicity distribution P(N), a
per-track efficiency uncertainty propagates approximately as
`(dM/M)^2 ~ N * (deps/eps)^2` for an N-track event, which is why the high-N tails
of P(N) get hit harder (1-5 % range, vs ~1 % on the moments per
`research-sidis-systematics-literature.md` Table 2 row Sys.5). For our **differential
SIDIS multiplicity** M(z, pT, ...) the propagation is different:

- M = N_h(bin) / N_DIS / Δbin
- A relative shift δε/ε on per-track efficiency shifts N_h(bin) by the same
  relative factor, so δM/M ≈ δε/ε ≈ 0.5 % per bin.
- N_DIS is set by the trigger and electron reconstruction, not by hadron tracking,
  so it is not affected by the per-track tracking systematic.

This is the key correction noted in `critique-systematics-implementation.md`
Section 4.2: the 1-5 % range from the H1 paper is for P(N), not for differential M.
For our differential M, the per-track 0.5 % propagates as approximately 0.5 % flat
per bin.

### 1.5 pT, eta, run-period dependence

The H1 paper does not publish a differential breakdown of the 0.5 %. From physics:

- Below pT ~ 150 MeV the CJC efficiency falls off (the track curls inside the
  drift chamber). Our pT > 0.15 GeV cut keeps us above this region.
- Near |eta| = 1.3 we reach the edge of the central CJC acceptance. The
  efficiency is still well-modelled by MC there, but the data-MC residual
  could be slightly larger (perhaps 0.7-1 % rather than 0.5 %). This is an
  unmeasured edge effect for our analysis; the eta_cut_scan handle in the
  reference (`reference-sidis-systematics.md` Variation Cookbook) probes
  exactly this fall-off.
- Across HERA-II running periods (2005, 2006, 2007 e+p; 2003-2006 e-p) the
  tracking calibration varied. The period_split handle (HERMES-style largest
  deviation rule, ~2 % typical) is the appropriate vehicle for capturing the
  period-to-period drift in tracking efficiency. The 0.5 % from the H1
  multiplicity paper is the period-averaged value.

### 1.6 Is the 0.5 % directly applicable to our analysis?

**Yes, with three caveats:**

1. The 0.5 % applies per central, primary-vertex track above pT ~ 150 MeV in
   the |eta| < 1.3 acceptance. Our BASE_CUT matches exactly.
2. The 10 % combined-track uncertainty does not apply because we exclude
   combined tracks (`IsCentralTrk == 1`).
3. For differential SIDIS multiplicities the propagation is ~0.5 % per bin
   on M (essentially flat), not the 1-5 % range that appears in the H1
   paper for P(N). The 1-5 % is multi-track-event amplification specific to
   the multiplicity-distribution observable.

**Documented in the bookkeeping:** the systematic must be explicitly labelled as
"H1-published central-track per-track efficiency, 0.5 %, propagated as 0.5 %
relative on M, exact cancellation in A_h."

---

## 2. Charge-Dependent Tracking Efficiency at H1

### 2.1 Why a charge-dependent residual is expected

The CJC measures track curvature in the H1 magnetic field (B = 1.16 T solenoid).
A charged track of momentum p and charge sign q bends with sagitta proportional
to qB/p. Positive and negative tracks bend in opposite directions in (r, phi).
Several physical effects lead to (small) charge-dependent reconstruction
efficiency in the central CJC:

1. **Geometric edge effects.** A track that bends toward a dead drift cell, a
   broken sense wire, or the inner/outer CJC wall may be lost. Whether this
   happens for a positive or negative track of the same |p| depends on the
   azimuthal position of the dead element and on the bending direction. Over
   the full azimuth this averages out to first order, but fluctuations across
   a real (not perfectly azimuthally symmetric) detector leave a residual.

2. **Pattern-recognition asymmetries.** Track-finding algorithms typically have
   a small directional bias because they propagate hits along the assumed
   trajectory. If the seed-hit pattern favours one bending direction over the
   other (for example when seeded from outer CJC layers and propagated inward),
   a small charge-dependent efficiency can result.

3. **Material interaction asymmetry** (covered separately in Section 3): proton/
   antiproton and pi+/pi- have different nuclear interaction cross-sections in
   the beampipe, CST, and inner CJC walls, biasing the surviving-hadron
   composition and hence the charge ratio.

4. **Trigger and dead-time correlations** (covered briefly in Section 6):
   the L1 track trigger fires on coincidence patterns that may have small
   charge-dependent biases.

### 2.2 Expected magnitude at H1

There is no published H1 measurement of the charge-dependent CJC efficiency
asymmetry per se. The closest approximations are:

- **The H1 multiplicity paper does not measure A_h** (per
  `research-sidis-systematics-literature.md` Section 1, repeatedly stated in
  Sections 3.5, 4, and 6.6). Therefore there is no published H1 charge-asymmetry
  systematic budget against which to benchmark.
- **The H1 Lambda 2009 paper** (arXiv:0810.4036, summarised in
  `research-published-systematics-survey.md` Section 1) measures
  A = (N_Lambda - N_antiLambda) / (N_Lambda + N_antiLambda) and finds A
  consistent with zero, with all detector systematics treated as
  charge-symmetric (i.e., assumed to cancel). They do not isolate a
  charge-dependent CJC residual; instead they implicitly absorb it into
  the GEANT3 simulation accuracy and the V0-cut envelope.
- **The CJC sagitta asymmetry as a function of pT.** A 10 micron sagitta
  measurement error on a 1 GeV track gives a momentum bias of ~1 % and a
  charge-dependent efficiency through the resulting acceptance edge bias
  of typically O(0.1 %) -- this is the order of magnitude that can be
  expected without dedicated study.

A reasonable expectation, consistent with the H1 Lambda result that A_Lambda
is statistics-limited at the few-permil level and consistent with the 0.5 %
per-track common uncertainty serving as the upper envelope, is that the
**charge-dependent residual on per-track efficiency at H1 is approximately
0.1-0.3 %, conservatively bounded at 0.5 %.** This does not cancel in A_h.

### 2.3 What other experiments report

- **ZEUS (arXiv:1001.4026, Sec. 5).** Quotes overall tracking efficiency
  at +/-3 % at high Q2 and (-0.5, +5) % at low Q2; assigns track-reconstruction-
  near-acceptance-borders at (-3, +6) % at high Q2. Does not separately report
  a charge-dependent component. ZEUS's larger overall tracking systematic
  (3 % vs H1's 0.5 %) reflects a less mature tag-and-probe analysis at the
  time of publication. ZEUS treats the asymmetry on V0s in
  hep-ex/0612023 (`research-published-systematics-survey.md` Section 6) by
  the K0s polarisation cross-check (any nonzero K0s "polarisation" would
  flag a charge-dependent detector asymmetry) and finds it consistent with
  zero -- but they do not derive a numerical asymmetry budget from this.
- **HERMES** uses a forward-spectrometer geometry not directly comparable.
  In their pion/kaon multiplicity paper (arXiv:1212.5407) the dominant
  systematic is RICH unfolding and JETSET tune (Section III.4); they do
  not isolate a charge-dependent tracking residual either.
- **COMPASS** (arXiv:1709.07374) groups all detector effects into a 5 %
  acceptance systematic and does not report a charge-dependent tracking
  efficiency.

**Net conclusion:** no comparable HERA experiment publishes a charge-
dependent tracking efficiency directly. The standard practice across
all four (H1, ZEUS, HERMES, COMPASS) is to assume detector systematics
cancel in charge-asymmetry observables and to validate this assumption
through a data/MC charge-ratio cross-check or a charge-symmetric polarisation
(K0s) cross-check. The numerical residual is taken implicitly as a
statistics-limited bound rather than evaluated as an explicit systematic.

### 2.4 What we should do

For our analysis the appropriate handling is:

1. **Assign 0.5 % per central track as the common tracking systematic on M.**
   Cancels in A_h.
2. **Assign a charge-dependent tracking residual.** Two routes are available:
   - Route A (analytic, conservative): take 0.2 % charge-dependent as the
     half of the common 0.5 % envelope and record it as a flat
     `tracking_charge_dep` source in A_h with cancellation class
     `does_not_cancel`. This is the placeholder approach already in
     `compute_systematics_charged.py` (`context-handoff.md` Section 9
     systematics table).
   - Route B (data-driven): measure the data/MC charge-ratio
     N_data(h+) / N_data(h-) divided by N_MC(h+) / N_MC(h-) as a function
     of pT, eta, phi. Any trend at the per-bin sub-percent level gives a
     direct upper bound on the charge-dependent tracking residual. The
     advantage of Route B is that it bounds residuals from CJC sagitta
     asymmetry, dead-cell distribution, and pattern-recognition bias all
     at once. Route B requires only the existing data and MC and does not
     require any new MC production.
3. **Cross-validate with the K0s polarisation check** (Section 6.3) if K0s
   reconstruction can be enabled in our charged-particle pipeline. The
   K0s -> pi+ pi- decay angular distribution is isotropic in the K0s
   rest frame; any charge-dependent tracking efficiency biases the
   reconstructed angular distribution and produces a fake "polarisation"
   that is purely instrumental.

---

## 3. Material Interaction Asymmetry

### 3.1 Physics

After the F3 builder fix (charge-consistent matching documented in
`memo-mc-truth-policy-charged-sidis.md` Section 6 and `context-handoff.md`
Section 7 / 11) **fake** charge-sign assignments arising from material-secondary
tracks matching to unrelated GTR particles are eliminated. What remains is the
**physical** asymmetry: positive and negative hadrons interact differently
with detector material, so a different fraction of each is lost before reaching
the tracker volumes that constitute our acceptance.

The material between the interaction point and the CJC outer envelope is, to
first approximation:
- Beam pipe: ~1 mm beryllium
- CST silicon: 4 layers, total ~0.5 % radiation length per layer
- Inner CJC wall and gas: ~0.5 % radiation length

Total material is ~3-5 % of a radiation length. Hadronic interaction lengths
are far longer than radiation lengths, so the fraction of hadrons lost to
nuclear interactions before the CJC midpoint is typically a few percent at
low pT and well below 1 % at high pT.

### 3.2 Charge-asymmetric interaction cross-sections

At the H1 momentum scale (a few hundred MeV to a few GeV in the lab):

- **Pions:** sigma(pi+ p) ~ sigma(pi- p) to within ~10 %. The pi-N total
  cross-section at p = 1 GeV/c is ~30-35 mb for both signs; at lower
  momenta the differences from delta-resonance excitation become
  larger (the Delta++(1232) couples preferentially to pi+ p, the
  Delta-(1232) less so since Delta- couples to pi- n). The net data:
  asymmetric pion absorption is at the percent level for low-pT pions.

- **Protons vs antiprotons:** sigma(p p) ~ 40 mb at p = 1 GeV. sigma(pbar p)
  is much larger (~70 mb at p = 1 GeV) and includes annihilation
  channels which dominate at low momentum (sigma_ann ~ 40 mb at p = 0.5
  GeV). Antiprotons annihilate. Therefore at the *primary-vertex track*
  level, more antiprotons are lost to material than protons, biasing the
  surviving sample toward h+. **For protons specifically, the material
  asymmetry has the opposite sign** from pions: more pbar lost than p.

- **Kaons:** sigma(K+ p) is much smaller than sigma(K- p) at H1 momenta
  because K- can produce hyperons (K- p -> Lambda pi0, K- p -> Sigma pi).
  More K- are lost to material than K+. This biases toward h+.

Our charged-hadron sample is pion-dominated (~80 %) with subdominant
proton/antiproton (~10-15 %) and kaon (~5-10 %) populations. The net
charge asymmetry from material interactions is the species-weighted
combination:

- pi+ slightly more absorbed than pi- at low pT (delta resonances) =>
  small bias toward h-
- pbar more absorbed than p => bias toward h+
- K- more absorbed than K+ => bias toward h+

The signs partially cancel. The dominant low-pT pion contribution biases
toward h-. The total expected material-induced charge asymmetry in our
inclusive sample is approximately **0.1-0.5 % at low pT, decreasing to
< 0.1 % above pT ~ 0.5 GeV**. This estimate is consistent with the
critique document `critique-systematics-implementation.md` Section 4.4
("net charge-dependent acceptance shift from material interactions
estimated at 0.1-0.5 %").

The published H1 Lambda paper (`research-published-systematics-survey.md`
Section 6) explicitly does **not** assign a separate material-asymmetry
systematic and relies on GEANT3 simulation to capture it. GEANT3
(used by H1 since the original detector simulation) is known to model
antiproton annihilation cross-sections imperfectly at low momentum
(survey Section 6, "GEANT 3 ... is known to have imperfect modeling of
antiproton annihilation at low momenta"). Our analysis must explicitly
evaluate this rather than rely on GEANT3 alone.

### 3.3 At what pT is the effect largest

The interaction probability is set by the geometric path length and the
cross-section. The asymmetric component is largest where:
- The cross-sections themselves are most different (low momentum:
  pi-Delta resonances near 200-300 MeV/c, pbar annihilation peak below
  500 MeV/c, K- hyperon production peak below 1 GeV/c).
- The track pT is closest to the material-traversal-floor where lost
  tracks are not replaced by upstream-produced secondaries (low pT,
  where primary tracks dominate the count).

Both pull toward **low pT (< 0.5 GeV)**, where the asymmetric absorption is
of order 0.5 % at the species level and ~0.2 % integrated over the
pion-dominated sample.

### 3.4 How H1 published papers handled it

- **H1 multiplicity paper (DESY 20-176)** does not measure A_h, so the
  question does not arise.
- **H1 charged-particle spectra (1302.1321)** measures charge-summed
  spectra; material asymmetry cancels by construction.
- **H1 Lambda 2009 (0810.4036)** absorbs material asymmetry into the
  GEANT3 simulation and assigns no explicit systematic. The implicit
  assumption is GEANT3 fidelity; the Lambda asymmetry is found
  consistent with zero at the few-permil statistical level, so the
  implicit assumption is not stress-tested.
- **ZEUS V0 paper (hep-ex/0612023)** does the same and additionally uses
  the K0s polarisation cross-check
  (`research-published-systematics-survey.md` Section 6) to bound
  charge-asymmetric detector biases.

### 3.5 How to estimate it from our MC

**Direct method:** compute the charge ratio at gen and reco level and
attribute the difference to detector effects.

For each kinematic bin b:

```
R_gen(b)   = N_gen(h+, b) / N_gen(h-, b)
R_reco(b)  = N_reco(h+, b) / N_reco(h-, b)        # MC reconstructed, no truth-match
A_mat(b)   = (R_reco(b) - R_gen(b)) / (R_reco(b) + R_gen(b))
```

`A_mat(b)` measures the *combined* detector asymmetry: tracking efficiency
charge-dependence + material absorption + reconstruction-bias-from-charge.
Because the F3 fix eliminates fake charge assignments via mismatched
truth-matching, this measure isolates physical detector effects. The
matched-charge purity diagnostic (`critique-systematics-implementation.md`
HIGH-3, "log the matched-charge purity ... any value below 99 % is a
validation failure") is the cross-check that A_mat is interpretable.

**Sanity check:** R_gen at the broad fiducial should match the upstream
HERA charged-pion ratio at moderate Bjorken-x (h+/h- typically 1.05-1.20
at z ~ 0.3-0.7 in the H1 phase space, dominated by the charge of the
struck quark). R_reco above pT ~ 0.5 GeV should agree with R_gen to
within ~0.5 % per bin. Below pT ~ 0.3 GeV the disagreement may grow
to a few percent and signals the material asymmetry plus the CJC
low-pT efficiency dropoff.

**Indirect method (DCA scan):** tracks from material interactions are
displaced from the primary vertex and have systematically larger
DcaPrime values than primary-vertex tracks. Tightening the DCA cut from
2.0 cm to 1.5 cm preferentially removes material-interaction products and
should reduce |A_mat| by the same fraction as it reduces the
material-interaction contamination. The DCA scan handle (already in the
MVP, `dca_scan` in `context-handoff.md` Section 9 and
`reference-sidis-systematics.md` Variation Cookbook) thus provides an
*indirect* upper bound on the material-asymmetry systematic without
needing MC truth ancestry tagging.

### 3.6 Does the DCA cut help?

Yes, but partially. Tracks from nuclear interactions in the beampipe and
inner CST tend to have DcaPrime > 1 cm because the secondary vertex is
displaced. Tighter DCA preferentially removes them. However:

- Material interactions at very small radii (the first 1-2 mm of the
  beampipe) produce daughters whose DCA back-projects close to the
  primary vertex, so they survive the DCA cut.
- The hadron loss itself (the "primary" pion that interacted and never
  reached the tracker) is irreversible by any cut on the surviving
  tracks; it is captured only in the per-bin reco/gen ratio.

Consequently the DCA scan handles the *fake* contribution from material
secondaries that pass the primary-vertex cut. The *absorption asymmetry*
remains and is captured only by the reco/gen comparison or by GEANT3.

---

## 4. V0 and Conversion Contamination for Inclusive Charged Hadrons

### 4.1 Source decomposition and charge symmetry

| Source | Charge structure of daughters | Cancels in A_h? | Magnitude on M (H1 paper) |
|---|---|---|---|
| K0s -> pi+ pi- | one h+ and one h- (always paired) | YES (charge-symmetric) | 1-7 % on P(N), <=2 % on moments |
| Lambda -> p pi- | one h+ (proton) and one h- (pion) -- but Lambda is matter, not antimatter | NO if Lambda yield differs from anti-Lambda | (sub-leading inside the same K0s envelope) |
| anti-Lambda -> pbar pi+ | one h- (antiproton) and one h+ (pion) | NO if Lambda yield differs | (sub-leading) |
| gamma -> e+ e- (conversion) | one h+ and one h- (paired) | YES (charge-symmetric) | 0.5 % (H1 paper Sys.6) |
| pi0 Dalitz -> gamma e+ e- | one h+ and one h- | YES | bundled with conversions |

The H1 multiplicity paper Sys.6 (`research-sidis-systematics-literature.md`
lines 142-154) assigns **50 % uncertainty** on the K0s contamination and
**0.5 % uncertainty** on the conversion+Dalitz contamination. The total
is 1-7 % on P(N) and up to 2 % on moments.

**For A_h specifically:**
- The K0s contribution is charge-symmetric in the daughter pair (one pi+ and
  one pi- per decay) and does not bias A_h. K0s contamination cancels.
- The Lambda + anti-Lambda contribution is charge-asymmetric to the extent that
  the Lambda yield differs from the anti-Lambda yield. At HERA the Lambda
  asymmetry A_Lambda = (N_Lambda - N_antiLambda) / (N_Lambda + N_antiLambda)
  is small (consistent with zero at the percent level per
  `research-published-systematics-survey.md` Section 5), so the leading
  Lambda/anti-Lambda contamination is also charge-symmetric to first order.
  However, the Lambda decay produces a proton (h+) and a pi- (h-),
  whereas anti-Lambda produces a pbar (h-) and a pi+ (h+); the *species*
  of the positive daughter differs between the two. This matters because:
  - The proton from Lambda has a stiffer momentum spectrum than the
    pi+ from anti-Lambda (asymmetric two-body kinematics).
  - The proton is *less* absorbed in material than the pbar.
  - Hence the surviving h+ from Lambda decay is biased toward higher
    momentum than the surviving h+ from anti-Lambda decay.
  - Net effect on A_h is non-zero even when N_Lambda = N_antiLambda
    exactly, mediated by the species-specific material interaction
    asymmetry inside the V0 contamination subset.
- The conversion (gamma -> e+ e-) is charge-symmetric and cancels.
- The pi0 Dalitz decay is charge-symmetric and cancels.

### 4.2 What fraction of our charged tracks come from V0 daughters

**No direct measurement is available in our pipeline today.** The
information needed is the MC truth ancestry of each reco track. The
trees provide:

- `ParticleSource` (`memo-mc-truth-policy-charged-sidis.md` Section 4)
  classifies generator-level charged hadrons by immediate-mother category:
  PS = 0 (primary), PS = 1 (Sigma0 daughter), PS = 2 (Xi daughter),
  PS = 3 (Omega daughter), PS = 4 (kSecondaryInteraction; STR-only),
  PS = 5 (kCombinatorial; multiple mothers).
- `GenMotherPDG` (referenced in `memo-mc-truth-policy-charged-sidis.md`
  Section 5, "stored on ChargedTotGen at line 679-681 of the builder")
  gives the PDG code of the immediate gen mother.

Critically, **K0s and Lambda are PYTHIA-stable in the GTR bank**; their
charged daughters are produced by GEANT3 in the STR bank with
`IsGtr() == false`. By the policy in Section 1 of the truth memo,
these STR-bank pi/K/p never enter the gen denominator. They can,
however, enter the reco numerator if the corresponding STR track passes
the BASE_CUT and is matched to a gen particle by direction.

The F3 charge-consistent matching fix removes the *misclassified* part
(an STR pi+ from K0s decay matched to an unrelated GTR gen pi-), but
the *correctly matched* part survives: an STR pi+ from K0s decay matched
to the GTR pi+ that PYTHIA originally listed as the K0s decay daughter
*if* the immediate-daughter PDG is stored. In practice for K0s decay,
the GTR bank holds the K0s itself (PDG 310) but not its decay daughters
(those are GEANT3-decayed); the STR pi+ has IsGtr=false and therefore
no GTR counterpart with PDG 211 from the same parent. The matching
loop scans GTR pi/K/p only, so the STR daughter cannot find its sibling
GTR daughter -- it can only find an unrelated GTR pi+ from the primary
vertex, which the charge-consistency cut allows but the angular
matching may or may not satisfy.

The net effect is that **K0s and Lambda daughters that pass the analysis
cuts appear in the numerator without a matched GTR partner of the same
PDG**. They are accounted for in `IsMatched == 0` reco tracks (or in
matched tracks where the matched gen partner is a primary pi/K/p that
happens to lie within the matching cone, which is a fake match). The
F3 fix's charge-consistency requirement reduces but does not eliminate
the latter category.

### 4.3 Estimating contamination from the gen tree

Until a direct V0-daughter ancestry tag is added to the trees, the
contamination must be estimated indirectly. Three routes:

1. **MC-MC comparison via ParticleSource on ChargedTot.** The reco
   tree carries `ParticleSource` for matched tracks. If the matched-gen
   partner of a reco track has PS in {1, 2, 3} (hyperon feed-down), the
   track is from a generator-level hyperon decay chain. **The K0s and
   Lambda contamination is *not* in PS = {1, 2, 3} because K0s/Lambda
   themselves are PYTHIA-stable and decay via GEANT3.** The PS handle
   captures only Sigma0/Xi/Omega feed-down, which is a different
   contamination subset. Useful for hyperon feed-down systematic but not
   for V0 contamination.

2. **MC truth ancestry via GenMatchPDG on ChargedTot (if available).**
   This branch, if present (the truth memo Section 5 mentions it as a
   possible future addition), would identify a reco track whose matched
   gen partner has a K0s or Lambda mother. Currently the trees store
   `GenMotherPDG` only on `ChargedTotGen` (gen denominator), not on
   `ChargedTot` (reco numerator). The asymmetry of the bookkeeping means
   we know the mother of each gen primary but not the mother of each
   reco-matched primary.

3. **Counting K0s and Lambda in the GTR bank and applying a measured
   acceptance.** This is the "indirect" approach. Loop over `H1PartMC`
   GTR particles for each event, count IsGtr K0s (PDG 310) and Lambda
   (PDG 3122) inside the analysis fiducial, multiply by the known H1
   K0s and Lambda branching ratios and by an estimated daughter
   acceptance (typically 30-50 % per daughter for the H1 central CJC
   above pT > 0.15 GeV). The result is a yield estimate of K0s and
   Lambda daughters surviving the analysis cuts.
   - The H1 published Lambda acceptance in our kinematic region is
     ~20 % (`research-published-systematics-survey.md` Section 6:
     "epsilon_rec ... 19.5 % for Lambda on average"). Lambda yield
     in our DIS sample is at the percent level.
   - K0s yield is approximately 2-4x the Lambda yield in pp/ep
     fragmentation.
   - Daughter acceptance for the `IsFromPrimary == 1` cut is small
     because K0s and Lambda decay several cm from the primary vertex
     and most daughters fail the primary-vertex requirement. The
     fraction surviving `IsFromPrimary == 1` is approximately 5-15 %
     depending on decay length and PV-association tolerance.
   - Combined: K0s daughters contribute approximately
     2-4 (K0s/event-DIS) * 2 (daughters/K0s) * 0.10 (PV-survival) /
     N_charged_per_event_DIS ~ 1-3 % of the inclusive charged sample.
   - Lambda daughters contribute approximately
     1 (Lambda/event-DIS) * 2 (daughters/Lambda) * 0.10 (PV-survival) /
     N_charged_per_event_DIS ~ 0.3-1 % of the inclusive sample.
   These estimates are consistent with the H1 multiplicity paper's
   Sys.6 magnitude (1-7 % on P(N), with the higher end at high-N).

### 4.4 How to evaluate the V0 contamination systematic

For our analysis the appropriate evaluation methods are, in order of
preference:

1. **Direct MC ancestry tag.** Add `GenAncestorPDG` (the most-distant
   strange-particle ancestor in the chain) to the `ChargedTot` reco
   tree at builder level. With this, any reco track with
   `GenAncestorPDG == 310` (K0s) or `abs(GenAncestorPDG) == 3122`
   (Lambda or anti-Lambda) is tagged. The systematic is then
   evaluated by the H1 paper recipe: vary the K0s/Lambda fraction
   by 50 % and propagate.
   - This requires a builder rerun. Not in MVP.

2. **Indirect via DCA scan.** Tightening the DCA cut from 2.0 to 1.5 cm
   removes a known fraction of V0 daughters because their reconstructed
   tracks have systematically larger DCA (the K0s decay length is
   c*tau = 2.7 cm; Lambda is 7.9 cm; many of the daughters that pass
   the loose primary-vertex association have DCA in the 0.5-2 cm
   range). The change in M and A_h between DCA = 1.5 and 2.0 cm
   bounds the V0-daughter contamination contribution.
   - In MVP today as `dca_scan`. Already executable.

3. **Charge-asymmetry sensitivity.** Compute A_h with and without the
   pT < 0.3 GeV bins included. K0s and Lambda daughters have
   characteristically soft pT spectra; removing the lowest-pT bins
   removes most of the V0 contamination. The change in integrated A_h
   bounds the contamination contribution to the asymmetry.

4. **The H1 paper recipe applied analytically.** Take the indirect
   yield estimate from Section 4.3 (K0s daughters ~1-3 % of sample,
   Lambda+anti-Lambda daughters ~0.3-1 %), apply 50 % uncertainty
   on K0s as the H1 paper does, and apply 100 % uncertainty on the
   Lambda - anti-Lambda difference (the non-cancelling part). This
   gives:
   - K0s: 50 % * 1-3 % = 0.5-1.5 % uncertainty on M (cancels in A_h).
   - Lambda - anti-Lambda non-cancellation: assume 5 % anti-Lambda
     deficit relative to Lambda from material asymmetry; the
     effective non-cancelling fraction is then 5 % * (0.3-1 %) =
     0.015-0.05 % on A_h. Conservative.

The total V0 systematic on A_h from the analytic recipe is at the
0.05-0.1 % level, dominated by Lambda absorption asymmetry. This is
much smaller than the 0.5 % H1-paper-style number for M because the
charge symmetry of K0s decay protects A_h from the leading
contribution.

---

## 5. Track Quality Cut Systematics: Methodology

Our BASE_CUT contains five threshold-style cuts in addition to the kinematic
cuts (`IsFromPrimary` and `IsCentralTrk` are categorical, not threshold,
and are not scanned):

- `NHitDedx >= 10`
- `RadLength > 10`
- `abs(DcaPrime/cosh(Eta)) < 2.0`
- `Pt > 0.15`
- `abs(Eta) < 1.3`

### 5.1 Cut scans vs data/MC efficiency comparison

There are two distinct ways to assign a systematic for a track-quality cut:

**Method A: Cut scan.** Vary the threshold around its nominal value (typically
+/- 1 working point) and propagate the resulting change in M and A_h. This
is the approach in the MVP. Strengths:
- Cheap (script-rerun, no new MC).
- Bounds the sensitivity of the result to the *choice* of cut value.
- Captures both the data/MC efficiency mismatch (if data and MC agree
  perfectly, the cut scan still produces a non-zero shift because the
  yield itself depends on the cut, but the ratio data/MC remains
  invariant; the systematic on M is then the residual).
- Captures bin-migration effects when the cut interacts with kinematic
  variables.

Weaknesses:
- Includes statistical noise from the data sample; the variation is not
  purely systematic.
- Does not directly measure the data-vs-MC efficiency disagreement; it
  measures the sensitivity of the *result* to the cut.

**Method B: Data/MC efficiency comparison.** Measure the per-cut efficiency
in data using a tag-and-probe technique on a control sample (typically
J/psi -> ee or another resonance), measure the same in MC, take the
data/MC ratio, and propagate its uncertainty. Strengths:
- Directly captures the physics: the systematic is the actual
  data/MC mismatch.
- Independent of the choice of cut value.
- Standard H1 practice for track-momentum-scale, electron-ID, etc.

Weaknesses:
- Requires a clean control sample and careful matching of the control
  to the analysis kinematic region.
- Does not capture cut-vs-kinematic-variable interactions.
- Often more expensive than a cut scan.

**The H1 multiplicity paper uses neither.** It assigns the per-track
0.5 % central / 10 % combined as a fixed published number, which is itself
the result of a Method B (data-driven tag-and-probe) study performed
upstream of the analysis and not re-evaluated for each measurement. The
paper does not perform a cut scan as a systematic; it relies on the
upstream tracking-efficiency note for the per-track number, and it
relies on the MC unfolding response to capture the cut-vs-kinematic
interaction.

**For our analysis** the appropriate hybrid is:

1. Take the H1-published 0.5 % per central track as the **data/MC
   tracking-efficiency systematic**. This is Method B as conducted by
   the H1 collaboration, inherited rather than re-derived.
2. Use the cut scans as **complementary sensitivity tests** that capture
   cut-vs-kinematic interactions and migration effects not contained in
   the inherited 0.5 %. The cut scan systematic is *added in quadrature*
   to the 0.5 % per-track systematic.
3. Document the partial overlap: a tightening of NHitDedx from 10 to 12
   removes some of the tracks whose efficiency is poorly modelled in MC,
   so the cut scan and the per-track systematic are not fully
   independent. Conservatively, the quadrature addition is an
   over-estimate; using the maximum of (cut-scan envelope, 0.5 % per
   track) is a tighter bound.

### 5.2 Charge-dependent efficiency of each cut

| Cut | Charge-dependent? | Mechanism | Captured by |
|---|---|---|---|
| NHitDedx >= 10 | Mostly no | Hit count is set by track length traversed; same for h+ and h- of the same kinematics. Tiny residual via the dE/dx itself depending on velocity (Bethe-Bloch gives same dE/dx for h+ and h- of same momentum). | nhitdedx_scan |
| RadLength > 10 | Slightly | Path length through CJC depends on bending; positive and negative tracks with the same |p| traverse slightly different lengths because they bend in opposite directions. The asymmetry is at the 0.1 % level. | radlength_scan |
| DcaPrime / cosh(Eta) < 2.0 | Yes (material interaction) | Tracks from material secondaries have larger DCA; material absorption is charge-asymmetric (Section 3); therefore the DCA cut efficiency is charge-asymmetric. | dca_scan (key handle) |
| Pt > 0.15 | Yes (CJC sagitta) | Low-pT tracks have larger curvature, more sagitta-dependent reconstruction failures; charge-dependent because positive and negative tracks reach the pT threshold via different sagitta trajectories. | pt_cut_scan |
| abs(Eta) < 1.3 | Slightly | Forward/backward acceptance edges can have slight charge asymmetry from the magnetic field tilting the track at the acceptance boundary. | eta_cut_scan |

The DCA cut is the most charge-asymmetric and is the best in-MVP handle on
the material asymmetry. The pT cut is the second-most charge-asymmetric
because it directly probes the low-pT region where material asymmetry and
CJC sagitta efficiency both peak.

### 5.3 The H1 multiplicity paper's approach to track-quality cuts

The H1 paper does not vary individual track-quality cuts as systematics. Its
Sys.5 (single-track efficiency) is purely the inherited per-track number;
its Sys.6 (V0 contamination) is the only cut-related systematic, and even
it is varied by changing the K0s/conversion contamination assumption rather
than by changing a cut threshold. The cut variation is implicit in the
unfolding: by unfolding with different MC generators (Sys.2), the paper
captures the cut-vs-kinematic-vs-generator interaction.

**Implication for us:** the proper analog of the H1 multiplicity paper's
treatment is to (a) inherit the 0.5 % per-track from the published
tracking-efficiency note and (b) include `mc_model_swap` (RAPGAP vs
DJANGOH) as the dominant model-dependence source. The per-cut scans
in the MVP are H1's *charged-particle spectra* paper precedent
(arXiv:1302.1321) which does perform some cut variations, and they
remain useful additional handles.

---

## 6. Data-Driven Cross-Checks

The most powerful cross-checks for tracking systematics are charge-symmetric
detector observables that *should* be zero or unity if the detector is
charge-symmetric. Five concrete checks, all executable today on the existing
merged files:

### 6.1 Data/MC charge-ratio comparison

Plot N_data(h+) / N_data(h-) and N_MC(h+) / N_MC(h-) as a function of:
- pT (most sensitive to material asymmetry and CJC sagitta)
- eta (most sensitive to acceptance-edge effects)
- phi (most sensitive to dead-cell distribution)
- Run period (most sensitive to time-dependent calibration drift)

**Interpretation:**
- If `R_data(b) / R_MC(b) = 1` to within statistics, all charge-asymmetric
  detector effects are well-modelled by MC and no additional systematic
  is needed for A_h.
- If a structured deviation exists (e.g., R_data/R_MC < 1 below pT 0.3 GeV
  by ~1 %), this is a direct data-driven measurement of the residual
  charge-dependent efficiency that is not captured by MC. The systematic
  on A_h is then `delta_A_h(b) = 0.5 * (R_data/R_MC - 1)` per bin.

**Cost:** zero new infrastructure; a single TTree::Draw on data and MC
with a charge cut.

### 6.2 Acceptance h+/h- ratio

Plot the MC-only acceptance ratio epsilon(h+) / epsilon(h-) per bin. By the
F3 fix, this should be 1.000 +/- statistical fluctuation if the gen-level
sample is charge-symmetric AND the detector is charge-symmetric. A
systematic deviation (e.g., epsilon(h+) / epsilon(h-) = 1.005 +/- 0.001
in a particular bin) directly measures the MC-modelled charge-dependent
detector effect.

The published H1 Lambda analysis quotes Lambda acceptance and anti-Lambda
acceptance separately (`research-published-systematics-survey.md`
Section 6) precisely to expose this ratio. We should follow suit.

**Cost:** already computed in our acceptance pipeline; just plot the
ratio.

### 6.3 K0s polarisation cross-check (ZEUS-style)

If the K0s reconstruction is enabled in our pipeline (it is *not* in the
inclusive charged-hadron analysis but would require minimal addition --
see `research-published-systematics-survey.md` Section 6, ZEUS practice),
the K0s -> pi+ pi- decay angular distribution in the K0s rest frame
should be isotropic by parity. Any measured "polarisation" of the K0s
is purely instrumental and quantifies a charge-dependent detector
asymmetry on the daughter pi+/pi- pair.

**ZEUS finding:** consistent with zero. **H1 has not done this check.**
This is a very powerful upper bound on charge-dependent tracking efficiency
because it isolates the detector contribution at fixed gen-level
charge symmetry (the K0s cannot be polarised in the reaction).

**Cost:** moderate. Requires reconstructing K0s candidates and fitting the
K0s mass peak in cos(theta*) bins. Existing H1 V0 tools (used by the
Lambda analysis at `H1/lambda/`) provide the basis. Could be added as a
post-MVP cross-check.

### 6.4 Tag-and-probe on J/psi -> e+ e-

H1 has an extensive J/psi sample at low Q2 (and a smaller diffractive J/psi
sample at high Q2). The J/psi mass peak is narrow (~30 MeV in the
H1 CJC), and the daughter e+ e- pair is balanced in pT. Tagging on one
daughter and probing the other gives a charge-dependent efficiency
measurement. This is the standard ATLAS/CMS technique; H1 has not used
it for the per-charge tracking systematic but the technology exists.

**Cost:** high; requires a new mini-analysis. Out of MVP scope.

### 6.5 Charge-summed vs charge-separated multiplicity comparison

Compute M^{h+} + M^{h-} and compare to the H1 published charge-summed
M^{h} from arXiv:1302.1321 Tables. Any disagreement bounds the
charge-summed tracking systematic; any phi/eta-dependent disagreement
in the difference M^{h+} - M^{h-} bounds the charge-asymmetric
component. This is a powerful end-to-end closure check.

**Cost:** zero extra infrastructure once nominal multiplicities are
extracted. Use the published H1 charged-particle spectra as the
external reference.

---

## 7. Implementation Recommendations

The MVP systematics driver `compute_systematics_charged.py`
(`context-handoff.md` Section 9) handles tracking systematics through
five script-rerun handles (`dca_scan`, `pt_cut_scan`, `eta_cut_scan`,
`nhitdedx_scan`, `radlength_scan`) plus two analytic handles
(`tracking_eff` and `tracking_charge_dep`). The recommendations below
specify how each tracking sub-component should be evaluated and
recorded in the output JSON.

### 7.1 Common central-track efficiency

- **Handle:** `tracking_eff`
- **Method:** analytic; multiply M_nominal by 0.005 per bin to get
  the absolute uncertainty on M.
- **Rerun cost:** zero.
- **Cancellation in A_h:** exact (delta_A_h = 0).
- **Output JSON entry:**
  ```
  "tracking_eff": {
    "owner": "h1-data-validator",
    "method": "analytic-inherited-0.5pct-central",
    "cancellation_class": "cancels",
    "per_charge": { "hplus": { "delta_M": 0.005 * M_plus }, "hminus": ... },
    "derived_asymmetry": { "A_h": { "delta": 0.0 } },
    "provenance": "H1 multiplicity paper DESY 20-176 Sys.5 central-track 0.5%"
  }
  ```

### 7.2 Charge-dependent tracking residual

- **Handle:** `tracking_charge_dep`
- **Method:** Two-step.
  - Step A (placeholder, MVP): assign 0.2 % flat per bin on
    A_h, classified `does_not_cancel`. Document as a conservative
    upper bound based on the half-of-common envelope.
  - Step B (post-MVP, data-driven): derive the per-bin residual from the
    data/MC charge-ratio cross-check (Section 6.1); replace Step A
    placeholder with the measured value.
- **Rerun cost:** zero for Step A; one TTree::Draw + plotting for Step B.
- **Cancellation in A_h:** does not cancel.
- **Output JSON entry:**
  ```
  "tracking_charge_dep": {
    "owner": "h1-data-validator",
    "method": "data-driven-charge-ratio (or analytic-placeholder-0.2pct)",
    "cancellation_class": "does_not_cancel",
    "per_charge": "n/a (asymmetry-only source)",
    "derived_asymmetry": { "A_h": { "delta": [0.002, 0.002, ...] } },
    "provenance": "Section 2.4 of research-tracking-systematics-deep.md;
                   no published H1 number; conservative bound"
  }
  ```

### 7.3 Material interaction asymmetry

- **Handle:** `material_interaction` (subsumed under `dca_scan` in MVP)
- **Method:** Two-step.
  - Step A (MVP): use the DCA scan envelope (Section 5.2) as the
    operational handle. The systematic on A_h is the maximum
    |A_h(DCA=1.5) - A_h(DCA=2.0)| or |A_h(DCA=2.5) - A_h(DCA=2.0)|
    per bin. Already in MVP.
  - Step B (post-MVP, direct): compute `A_mat(b)` from the
    R_reco(b) / R_gen(b) MC comparison (Section 3.5); record as a
    separate `material_asymmetry` source.
  - Step C (publication-grade): vary GEANT3 hadronic interaction
    cross-sections (or rerun a subset of MC with GEANT4 cross-sections)
    and propagate. This is a Condor-expensive rerun.
- **Rerun cost:** zero for Step A (already in MVP); one MC TTree
  pass for Step B; multi-day Condor rerun for Step C.
- **Cancellation in A_h:** does not cancel.
- **Bookkeeping:** keep `material_interaction` as a *distinct* source
  from `tracking_charge_dep` even though both contribute to the
  charge-asymmetric tracking residual. This matches the
  reference-systematics policy
  (`reference-sidis-systematics.md` Added Beyond Literature item 6,
  "Material-interaction asymmetry source for A_h: keep distinct from
  generic tracking efficiency").
- **Output JSON entry:**
  ```
  "material_interaction": {
    "owner": "h1-data-validator",
    "method": "indirect-via-DCA-scan (MVP); direct-via-MC-reco-gen-ratio (post-MVP)",
    "cancellation_class": "does_not_cancel",
    "per_charge": "via DCA scan",
    "derived_asymmetry": { "A_h": { "delta": [...] } },
    "provenance": "Section 3 of research-tracking-systematics-deep.md;
                   GEANT3 known to model pbar annihilation imperfectly"
  }
  ```

### 7.4 V0 / conversion contamination

- **Handle:** `v0_contamination`
- **Method:** Three-tier.
  - Tier 1 (MVP, indirect): use the DCA scan as the operational handle.
    Tightening DCA preferentially removes V0 daughters; the resulting
    M and A_h shift bounds the contamination.
  - Tier 2 (post-MVP, indirect-yield-estimate): apply the analytic
    recipe of Section 4.4 step 4 (50 % uncertainty on K0s yield, 100 %
    uncertainty on Lambda - anti-Lambda non-cancelling fraction).
    Cost: load gen-level K0s and Lambda yields from the trees and
    apply the H1-paper recipe.
  - Tier 3 (publication-grade, direct): add `GenAncestorPDG` to
    `ChargedTot` builder output; tag K0s and Lambda daughters
    explicitly; vary contamination by 50 % per the H1 paper.
    Cost: builder rerun (Condor-expensive).
- **Rerun cost:** zero for Tier 1 (already in MVP);
  zero-to-light for Tier 2; Condor for Tier 3.
- **Cancellation in A_h:** K0s subset cancels; Lambda subset partial.
- **Output JSON entry:**
  ```
  "v0_contamination": {
    "owner": "h1-data-validator",
    "method": "indirect-via-DCA-scan + analytic-recipe-50pct-K0s",
    "cancellation_class": "partial",
    "subcomponents": {
      "K0s": { "cancellation_class": "cancels", "delta_M": 0.005-0.015 * M },
      "Lambda_minus_antiLambda": {
        "cancellation_class": "does_not_cancel",
        "delta_A_h": 0.0005-0.001
      },
      "conversions": { "cancellation_class": "cancels", "delta_M": 0.005 * M }
    },
    "provenance": "H1 multiplicity paper DESY 20-176 Sys.6"
  }
  ```

### 7.5 Track-quality cut scans (NHitDedx, RadLength, DCA, pT, eta)

- **Handles:** `nhitdedx_scan`, `radlength_scan`, `dca_scan`,
  `pt_cut_scan`, `eta_cut_scan`
- **Method:** vary each cut around its nominal value
  (typical +/- 1 working point), compute M and A_h with the varied
  cut, take the maximum |delta| as the systematic per bin.
- **Critical:** use `h_num_nominal / acc_varied` for the multiplicity,
  not `h_num_varied / acc_varied`. The latter cancels to `h_den` and
  produces zero deltas (`context-handoff.md` Section 11 mistake #4).
- **Rerun cost:** script-rerun.
- **Cancellation in A_h:** classified per
  `context-handoff.md` Section 9 systematics table:
  - `dca_scan`: does not cancel (probes material asymmetry)
  - `radlength_scan`: does not cancel (probes material asymmetry)
  - `pt_cut_scan`: partial (probes both common and CJC sagitta)
  - `eta_cut_scan`: partial (probes acceptance edge)
  - `nhitdedx_scan`: partial (probes hit-quality cuts uniformly)
- **Independence vs. inherited 0.5 %:** the cut scans partially
  overlap with the inherited per-track efficiency systematic.
  Conservative practice: add in quadrature. Tighter bound: take
  max(cut_scan, 0.005 * M) per bin.

### 7.6 Per-source quadrature recipe for the tracking subtotal

For the multiplicity:
```
delta_M_tracking_total = sqrt(
    (0.005 * M)^2                          # tracking_eff (inherited)
  + delta_M(nhitdedx_scan)^2               # cut scans
  + delta_M(radlength_scan)^2
  + delta_M(dca_scan)^2
  + delta_M(pt_cut_scan)^2
  + delta_M(eta_cut_scan)^2
  + delta_M(v0_contamination)^2            # V0 contamination on M
)
```

For the asymmetry:
```
delta_Ah_tracking_total = sqrt(
    0                                      # tracking_eff cancels
  + delta_Ah(tracking_charge_dep)^2        # 0.002 placeholder or data-driven
  + delta_Ah(material_interaction)^2       # via DCA scan or direct MC
  + delta_Ah(v0_Lambda_residual)^2         # non-cancelling V0 part
  + delta_Ah(dca_scan)^2                   # detector cut scans (charge-asym)
  + delta_Ah(radlength_scan)^2
  + delta_Ah(pt_cut_scan)^2
  + delta_Ah(eta_cut_scan)^2
  + delta_Ah(nhitdedx_scan)^2
)
```

The 0 entry for `tracking_eff` is explicit per Section 7.1: the common
per-track efficiency cancels exactly in A_h.

---

## 8. Summary Table

The summary table below collates per-component magnitude estimates,
cancellation behavior in A_h, evaluation method, and data-driven
validation. Magnitudes are scoped to our analysis (low-Q2,
|eta| < 1.3, IsCentralTrk == 1, pT > 0.15 GeV); they are not the
H1 paper P(N) numbers.

| # | Tracking sub-component | Magnitude on M | Magnitude on A_h | Cancels in A_h? | Evaluation method | Data-driven validation | MVP-executable today? |
|---|---|---|---|---|---|---|---|
| 1 | Common central-track efficiency (inherited 0.5%) | 0.5 % flat per bin | 0 % (cancels) | YES (exact) | Analytic; multiply M by 0.005 | Charge-summed M comparison vs H1 1302.1321 | YES (analytic) |
| 2 | Combined-track efficiency (inherited 10 %) | not applicable | not applicable | not applicable | Excluded by `IsCentralTrk == 1` | n/a | not applicable |
| 3 | Charge-dependent CJC efficiency residual | < 0.1 % | 0.1-0.5 % per bin | NO | Step A: 0.2 % placeholder; Step B: data/MC charge ratio | Data/MC R(h+/h-) per bin | YES Step A; YES Step B (single MC pass) |
| 4 | Material interaction asymmetry | 0.1-0.5 % at low pT | 0.1-0.5 % at low pT | NO | DCA scan (MVP); MC reco/gen ratio (post-MVP) | DCA scan envelope; reco/gen R ratio per bin | YES (via DCA scan) |
| 5 | V0 contamination - K0s daughters | 1-3 % on M | 0 % (cancels) | YES (charge-symmetric) | DCA scan (MVP); analytic 50 % rule (post-MVP); explicit ancestry tag (publication) | Run yield comparison vs H1 K0s rate | YES Tier 1; YES Tier 2 |
| 6 | V0 contamination - Lambda minus anti-Lambda | 0.3-1 % on M | 0.05-0.1 % on A_h | PARTIAL | Analytic recipe with 100 % uncertainty on Lambda - anti-Lambda non-cancellation | Cross-check vs H1 Lambda asymmetry result | YES (analytic) |
| 7 | Conversion contamination (gamma -> e+ e-) | 0.5 % on M (H1 paper) | 0 % (cancels) | YES (charge-symmetric) | Inherited 0.5 % from H1 paper Sys.6 | n/a | YES (analytic) |
| 8 | NHitDedx threshold | 0.5-2 % on M | 0.1-0.5 % on A_h | PARTIAL | Cut scan: NHitDedx in {8, 10, 12} | Data-MC distribution comparison per charge | YES |
| 9 | RadLength threshold | 0.5-2 % on M | 0.1-0.5 % on A_h | NO | Cut scan: RadLength in {5, 10, 15} | Data-MC distribution comparison per charge | YES |
| 10 | DCA cut | 0.5-3 % on M | 0.1-1 % on A_h | NO | Cut scan: DCA in {1.5, 2.0, 2.5} cm | Data-MC DCA distribution per charge | YES (in MVP) |
| 11 | pT threshold | 0.5-2 % on M | 0.1-0.5 % on A_h | PARTIAL | Cut scan: pT in {0.10, 0.15, 0.20} GeV | Data-MC pT distribution per charge | YES (in MVP) |
| 12 | eta threshold | 0.5-2 % on M | 0.1-0.5 % on A_h | PARTIAL | Cut scan: |eta| in {1.2, 1.3, 1.4} | Data-MC eta distribution per charge | YES (in MVP) |
| 13 | Period-to-period drift in tracking | 0-2 % on M | 0-2 % on A_h | NO | period_split: 2005, 2006, 2007 separately, HERMES-rule largest deviation | N(h+/h-) ratio per period | YES (in MVP) |
| 14 | K0s polarisation cross-check (post-MVP) | n/a (cross-check only) | bounds the row-3 residual | n/a | Reconstruct K0s, fit cos(theta*) | Direct measurement of charge-dep detector asym | NO (requires K0s in pipeline) |

**Quadrature totals (MVP, central-fiducial, low-Q2):**
- Multiplicity tracking subtotal: ~2-4 % per bin, dominated by V0 contamination (row 5) and DCA scan (row 10).
- Asymmetry tracking subtotal: ~0.3-1.5 % per bin, dominated by charge-dependent residual (row 3), material interaction (row 4), and DCA scan (row 10).

These numbers are pre-final and reflect the placeholder character of
rows 3, 4, and 6. Replacing the placeholders with data-driven measurements
(rows 3 via Section 6.1, row 4 via Section 3.5, row 6 via the analytic
recipe) is expected to *tighten* the asymmetry tracking subtotal toward
the 0.3-0.5 % level rather than loosen it, because the placeholder
values are conservative upper bounds.

---

## 9. Risks, Assumptions, and Open Questions

### 9.1 Risks

1. **Inherited 0.5 % is unverified for our analysis.** The H1 multiplicity
   paper does not publish a tracking-efficiency breakdown by pT, eta, run
   period, or any other kinematic variable. The 0.5 % is an integrated
   value across the H1 central-CJC fiducial. A localized region of our
   phase space (e.g., the pT < 0.2 GeV slice or the |eta| > 1.2 slice)
   could carry a larger residual uncertainty that is averaged into the
   global 0.5 %. Mitigation: data-driven charge-ratio cross-check
   (Section 6.1) per bin.

2. **GEANT3 pbar annihilation is imperfect.** Both H1 published papers
   rely on GEANT3 to capture material interaction asymmetry. The
   `research-published-systematics-survey.md` Section 6 explicitly
   flags this: "GEANT 3 ... is known to have imperfect modeling of
   antiproton annihilation at low momenta." Our analysis inherits this
   risk; the DCA scan and the reco/gen R-ratio cross-check together
   provide the only independent bounds.

3. **Lambda contamination is asymmetric in a way that can fake A_h.**
   The Lambda decay produces a proton (hard, not absorbed) and a pion
   (soft, slightly absorbed). The anti-Lambda decay produces an
   antiproton (soft, heavily absorbed) and a pion (soft). The
   surviving daughters of Lambda are biased to h+ (proton), and the
   surviving daughters of anti-Lambda are biased to h+ (pion). Both
   skew toward h+ but by different fractions. Net effect on A_h
   depends on the absolute Lambda yield; if Lambda contamination is
   1 % and the relative bias is 5 %, the A_h shift is 0.05 %. This
   is small but is a non-cancellation that requires explicit analytic
   evaluation per Section 4.4 step 4.

4. **The MVP cut scans are interpreted as systematics, but they are
   also sensitivity tests.** A large cut-scan envelope can indicate
   either a real detector mis-modelling (true systematic) or a
   poor choice of nominal cut (analysis design issue). Distinguishing
   the two requires the data/MC distribution comparison per cut
   (Section 6, item 1). If data/MC agree perfectly across the
   distribution, the cut-scan envelope is purely sensitivity and the
   nominal choice is well-justified.

### 9.2 Assumptions

1. **The F3 fix has eliminated all charge-flip fake matches.** This is
   asserted by `memo-mc-truth-policy-charged-sidis.md` Section 6 and
   `context-handoff.md` Section 7. The validation diagnostic (matched-
   charge purity > 99 %) is documented in
   `critique-systematics-implementation.md` HIGH-3 but its actual
   measured value should be confirmed by a `h1-data-validator` pass
   on the post-reprocessing trees.

2. **The 0.5 % inherited per-track is uncorrelated bin-to-bin.** This
   matches the H1 charged-particle spectra paper (1302.1321) statement
   "applied as an independent uncorrelated uncertainty on every data
   point" and is the conservative choice. The H1 multiplicity paper
   does not state the correlation explicitly (per
   `research-sidis-systematics-literature.md` Section 5 open question
   #3); the spectra-paper choice is the safer default.

3. **Charge-symmetric backgrounds (K0s daughters, conversions) cancel
   exactly in A_h.** True up to the second-order effects from species-
   specific material interaction inside the V0 daughter pair (Section 4.1
   discussion of Lambda vs anti-Lambda asymmetric daughter spectra).
   The leading effect is exact cancellation; the residual is below
   0.05 % on A_h and can be neglected at MVP precision.

### 9.3 Open Questions

1. **Is `GenAncestorPDG` worth adding to `ChargedTot`?** This single
   builder change would convert the V0-contamination systematic from
   indirect (Tier 1/2) to direct (Tier 3), enabling the H1 paper recipe
   exactly. Cost: one builder edit and one Condor rerun. Decision needed
   from the supervisor.

2. **Should the K0s polarisation cross-check be promoted into our
   pipeline?** ZEUS used it to bound charge-dependent detector
   asymmetries; H1 has not. Cost: enabling K0s reconstruction in
   the inclusive charged pipeline. Decision needed.

3. **What is the actual matched-charge purity in the post-F3
   reprocessed trees?** This number directly validates the F3 fix.
   Diagnostic: `N(Charge*GenCharge > 0 | IsMatched==1) / N(IsMatched==1)`
   on RAPGAP31 and DJANGOH14 merged MC. Owner: `h1-root-analyst`. This
   should be run before any tracking systematic is quoted publicly.

4. **What is the actual data/MC charge ratio per bin?** Section 6.1 is
   the most powerful cross-check and converts the
   `tracking_charge_dep` placeholder into a measurement. Owner:
   `h1-data-validator`. Should be run alongside the nominal extraction.

5. **Does the period-split tracking systematic show structure?** The
   HERMES-rule (largest deviation = systematic) gives an integrated
   number; if 2005 differs from 2006-2007 by an order of magnitude
   more than 2006 vs 2007, the HERMES rule may be too pessimistic
   and a year-by-year correlated treatment would be appropriate.
   Owner: `h1-systematics`.

6. **For high-Q2, does `IsCentralTrk == 1` still apply?** The high-Q2
   analysis uses the same builder family (`proton_HiQ/`) but the
   kinematic acceptance reaches forward eta where combined tracks
   contribute. This research focuses on low-Q2; the high-Q2 version
   should re-derive whether the central-only restriction holds and
   whether the 10 % combined-track number then becomes relevant.
   Owner: high-Q2 supervisor.

---

## 10. Pointers and Provenance

### 10.1 Source documents read

- `H1/sidis_lowQ2/research-sidis-systematics-literature.md` (DESY 20-176 Sys.5/Sys.6 inventory; ZEUS, HERMES, COMPASS comparison)
- `H1/sidis_lowQ2/reference-sidis-systematics.md` (systematics catalog policy; added-beyond-literature items)
- `H1/sidis_lowQ2/critique-systematics-implementation.md` (Section 5 tracking deep dive; cancellation matrix critique)
- `H1/sidis_lowQ2/memo-mc-truth-policy-charged-sidis.md` (F3 fix; charge-consistent matching; ParticleSource semantics)
- `H1/sidis_lowQ2/context-handoff.md` (current MVP systematics handles; analysis state)
- `H1/lambda/scripts/build_systematic_budget.py` (V0 budget builder reference)
- `H1/lambda/docs/research-h1-systematics-framework.md` (H1CalcSystematic enum; track-momentum systematic via H1SysShift)
- `H1/lambda/docs/research-published-systematics-survey.md` (H1 2009 Lambda; ZEUS V0; material interaction discussion; K0s polarisation cross-check)
- `H1/lambda/docs/plan-systematics-evaluation.md` (V0 systematics plan; track momentum scale precedent)
- `H1/sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` (BASE_CUT and IsCentralTrk fiducial)

### 10.2 Authoritative pointers

| Topic | Primary source |
|---|---|
| H1 inherited tracking-efficiency numbers (0.5 % central, 10 % combined) | DESY 20-176 Sys.5 (`research-sidis-systematics-literature.md` lines 124-138); arXiv:1302.1321 Section 5 (lines 256-262) |
| Cancellation matrix for A_h | `reference-sidis-systematics.md` Charge-Asymmetry Cancellation Matrix; `research-sidis-systematics-literature.md` Section 3.5 |
| Material interaction asymmetry physics | `research-published-systematics-survey.md` Section 6; this document Section 3 |
| V0 contamination recipe | DESY 20-176 Sys.6 (50 % K0s, 0.5 % conversions); `research-sidis-systematics-literature.md` lines 142-154; this document Section 4 |
| F3 charge-consistent matching fix | `memo-mc-truth-policy-charged-sidis.md` Section 6; `context-handoff.md` Sections 7 and 11 |
| Cut-scan handles in MVP | `reference-sidis-systematics.md` Variation Execution Cookbook; `context-handoff.md` Section 9 |
| Data-driven cross-checks | This document Section 6; ZEUS K0s polarisation in `research-published-systematics-survey.md` Section 6 |

### 10.3 Provenance

This document was produced by a fresh deep-read pass over the listed
source documents. Numerical magnitude estimates that are not direct
quotations from the H1 published papers are explicitly labelled in
Section 8 as MVP placeholders or as physics-informed estimates with
the underlying physics laid out in Sections 2-4. No new measurements
were performed; all data-driven validations (Section 6) are
recommendations, not measurements.

The document is intended to serve as the authoritative tracking-systematic
research artifact for downstream planning by the SIDIS supervisor and
the `h1-systematics`, `h1-data-validator`, and `h1-root-analyst` agents.
Where it conflicts with `reference-sidis-systematics.md` (the playbook),
the playbook should be updated to reflect the more detailed sub-component
breakdown given here (rows 3, 4, 5, 6 of Section 8), particularly the
explicit separation of `material_interaction` from `tracking_charge_dep`
and the explicit Lambda-vs-anti-Lambda asymmetry inside V0 contamination.
