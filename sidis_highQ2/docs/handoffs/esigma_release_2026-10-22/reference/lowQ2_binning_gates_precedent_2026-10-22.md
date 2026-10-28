# Low-Q² charged SIDIS binning: precedent survey and recommended gates (WS5), 2026-10-22

**Scope.** This is a literature survey only; no NAF access was used. The target measurement is H1 low-Q² charged-hadron SIDIS: 5 < Q² < 100 GeV², 0.1 < y < 0.6, HERA-II 2005–07, binned in (Q², x, z, pT_h) with pT_h in the γ*p CM (HCM) frame, current hemisphere, h+ and h− separately. It is unfolded with IBU, with TUnfold as a cross-check.

**Definitions used here** (identical to the WS4 plan):
- purity P = N(rec ∧ gen in bin) / N(rec in bin);
- stability S = N(rec ∧ gen in bin) / N(gen in bin).

**Method.** Every number below was read from the arXiv full text (pdftotext of arxiv.org/pdf/<id>), and the section or table is given. "Not stated" means I searched the text (purity/stability/resolution/bin/iteration/statistics) and found no number. In that case I did not infer one.

---

## (a) Precedent table

| # | Source (arXiv) | Measurement / phase space | Variables (unfolded) | Purity floor | Stability floor | k·σ rule | Min stats | Unfolding method and validation | Citation location |
|---|---|---|---|---|---|---|---|---|---|
| 1 | **H1, 1302.1321** | Charged-particle spectra, 5<Q²<100 GeV², 0.05<y<0.6, 2006 e+p, 88.6 pb⁻¹, HCM frame, 0<η*<5, 0<p*T<10 GeV | (x,Q²) × p*T or η* | **>75% in all bins** (particle-level purity, same definition as ours) | Not stated | Not stated. CJC σ(pT)/pT ≈ 0.2%·pT/GeV ⊕ 1.5% (Sec. 3.1) | Not stated | **Bin-by-bin** correction factors (DJANGOH nominal, RAPGAP difference = syst). Factors are 1–1.8 (current η*), up to 2.6 (central η*, high p*T) | Sec. 4, para. 1 and footnote 1; Table 1; Sec. 3.3 |
| 2 | **H1, 2011.01812** | Charged-particle multiplicity P(N), 5<Q²<100, 0.0375<y<0.6, 2006–07, 136 pb⁻¹, incl. current hemisphere 0<η*<4 | (N, Q², y) 3D | Not stated | Not stated | Not stated. Reco bins in Q², y, N are **finer than truth** "to better resolve migration effects" | Not stated | **TUnfold**, phase space enlarged at reco against boundary migrations; "robust against variations of the regularisation scheme". **Truth N bins widened for N>3 "to avoid large negative bin-to-bin correlations"** | Sec. 4.3; Table 1 |
| 3 | **H1, 1611.03421** | Jets, 5<Q²<100 (extended 3<Q²<120), 2005–07, 290 pb⁻¹ | Q², y, PT (multi-D) | Achieved: NC DIS ≈70%; jets typ. 40–45%, **35% at lowest PT accepted**. No floor stated | Acceptance ≈80% (60–70% at Q² edges); no floor stated | Not stated | MC stat on result ≈ **1/3 of data stat** (achieved, not a floor) | TUnfold, **τ = 10⁻⁶** (no τ dependence observed); extended phase space via extra rows and columns. Adjacent-Q² correlation ≈ −0.2; final PT bins = merged finer unfolded bins | Sec. 3 (purity/acceptance), Sec. 4 (τ), Sec. 6 (δMCstat), Sec. 8.1 (correlations) |
| 4 | **H1, 0911.5678** | Jets at low Q² (5<Q²<100, 0.2<y<0.7), 1999–2000, 43.5 pb⁻¹ | Q², PT, ξ | **typ. 70%, >50% in all bins** | **typ. 60%, >40% in all bins** | Not stated | Not stated | Bin-by-bin; mean of RAPGAP/DJANGOH, half the difference = model syst | Sec. 2.3, last para. |
| 5 | **H1, 1406.4709** | Multijets, 150<Q²<15000 | Q², y, PT (4D, 1370 truth × 4562 reco bins) | Not stated | Not stated | Not stated | Not stated | TUnfold, τ = 10⁻⁶. **L-curve gives τ = 7.8·10⁻⁵ with consistent results.** 148 truth bins combined to 64 published. **Adjacent-PT correlations down to −0.5 accepted** | Sec. 3.2–3.4; Sec. 6 |
| 6 | **H1, 0904.0929** | Inclusive σ_r at low Q² and x | (x, Q²) or (y, Q²) | **Typ. >50%; bin merged if P<25%** | **Typ. >50%; bin merged if S<25%** | Not stated | Not stated | Bin-by-bin MC correction, iterative MC reweighting | Sec. 4.2 |
| 7 | **H1, 0904.3513** | Inclusive σ_r (precision) | (x, Q²) | **>40% all bins**, typ. >50% | **>40% all bins**, typ. >50% | Not stated | Not stated | Bin-by-bin | Sec. 2.3 |
| 8 | **H1, 1012.4355** | Inclusive σ_r, low Ep runs | (x, Q²) | typ. 70% (high y) to 50% (low y); no floor stated | same as purity | Not stated | Not stated | Bin-by-bin | Sec. 4.1 |
| 9 | **H1, 1206.7007** | Inclusive NC/CC at high Q² | (x, Q²) | **>30%** (defines kinematic reach) | **>30%** | Not stated | Not stated | Bin-by-bin, A = S/P | Sec. 5.5 |
| 10 | **H1, 2108.12376** | Lepton-jet TMD-sensitive correlations, Q²>150 | 8 observables, unbinned | n/a (unbinned) | n/a | n/a | Not stated | **MultiFold, 5 iterations, "chosen such that the closure tests … do not dominate the total uncertainty".** Bias = RAPGAP vs DJANGOH unfolding difference; same-generator split-sample closure "unbiased within MC statistical uncertainties". Non-closure ≤ few %, 10% at low qT/Q. No binned IBU cross-check found in the text | Main text, unfolding and uncertainty paragraphs; Suppl. Fig. 6 (response matrices "not used"), Fig. 7 (non-closure) |
| 11 | **H1, 2412.14092** | Lepton-jet azimuthal asymmetry harmonics, Q²>100 | unbinned (MultiFold) | n/a | n/a | n/a. ϕ resolution ≈1 rad drives the model-bias systematic | **MC ≈ 100× data** (MC stat negligible) | MultiFold; model bias = RAPGAP vs DJANGOH; 100 Poisson bootstraps for stat | Sec. 5 |
| 12 | **ZEUS, hep-ex/9903056** | Breit-frame multiplicity/momentum spectra, current and target regions, Q²≥10 | (x,Q²) × xp, η | Not stated | Not stated | **"bin sizes … commensurate with the measurement resolution"** (qualitative, k≈1). (x,Q²) bins sized "to give good statistics … and to limit the migrations" | Not stated (qualitative) | Bin-by-bin, factors <1.3 (current), <2.0 (target). **Current↔target track migration: 7% out; 12% in on average for 10<Q²<320, up to 25% for Q²<40 at low y** | Sec. 3 (binning), Sec. 5 |
| 13 | **ZEUS, 1001.4026** | Scaled-momentum spectra, 10<Q²<41000 | Q² × xp | Not stated | Not stated | Not stated | Not stated | Bin-by-bin, factors typ. <1.2 (<1.5 at highest Q²) | Sec. 5 |
| 14 | **ZEUS, hep-ex/9910043** | Forward jets, ET²/Q² | ET²/Q² | Not stated as floor. Achieved 40→80%; **~20% accepted in lowest bin** ("statistical errors are large") | Efficiency 35→55%; ~20% in lowest bin | Not stated | Not stated | Bin-by-bin, factors 0.8–1.4 | Sec. 2 |
| 15 | **HERMES, 1212.5407** | π±, K± multiplicities, p and d targets, Q²>1 | 3D: (x or Q²) × z × Ph⊥ | Not stated | Not stated | Not stated. Model independence is argued only "in the limit of infinitely small bins" | Not stated. φh binning omitted "because of limited statistical precision" | **Unregularised inversion of a 3D smearing matrix S(i,j) = n(i,j)/n_Born(j)**, with j=0 = migration from outside acceptance. Finite-bin-width model dependence via JETSET-parameter scan. Results correlated; covariance published | Sec. III.C intro (3D binning rationale); Sec. III.C.5, Eqs. 4–6; Sec. III.D.4; Sec. IV, Tabs. III–VI |
| 16 | **COMPASS, 1604.02695** | π±, h± multiplicities, isoscalar target | (x, y, z) | n/a | n/a | Not stated. Correction done "in narrow kinematic bins" to reduce generator dependence | Not stated | Bin-by-bin acceptance A = rec/gen multiplicity; **A always >40%** (≈70% y<0.3, ≈50% y>0.3) | Sec. 3.3 |
| 17 | **COMPASS, 1608.06760** | K± multiplicities | (x, y, z) | n/a | n/a | Not stated | Not stated | Same method; **A always >35%**, average ≈65% | Sec. 2.3 |
| 18 | **COMPASS, 1709.07374** | TMD (PhT²-dependent) h± multiplicities, 4918 points | (x, Q², z, PhT²), 4D | n/a | n/a | Not stated | Not stated as a number: "a few (x,Q²) bins are **discarded** in the lowest and highest z bins because of low statistical precision as well as large acceptance correction factors" | 4D acceptance in narrow bins; **A >0.4** at high x and PhT²>1; h+/h− acceptance agree to 0.02–0.04 | Sec. 3.3; Sec. 5, para. 1 |
| M1 | **Schmitt, TUnfold, 1205.6201** | method | — | Not stated | Not stated | Not stated as k. **Requires n_reco ≥ n_truth (n − m > 0 d.o.f.)**: "data … measured in finer bins than are extracted" | Not stated | τ by L-curve (Sec. 4.1) or **minimising average or maximum global correlation ρ_i** (Sec. 4.2). Bins may be merged before computing ρ_i. Too small τ gives "large negative correlations of adjacent bins"; too large τ biases toward x₀ | Sec. 1; Sec. 4, 4.1, 4.2; Sec. 7.2 (bin-width-aware regularisation) |
| M2 | **D'Agostini, 1010.0632** | method (IBU) | — | — | — | Not stated | Zero MC counts: "best pragmatic solution is re-binning" | **"just two or three steps are sufficient"** in normal cases; too many iterations make the result "crazy" (positive feedback from fluctuations). Optional smoothing between iterations (not after the last) | Sec. 4; App. B.1, footnote 12 |
| M3 | **Adye, RooUnfold, 1105.1160** | method | — | — | — | — | — | IBU: number of iterations "has to be tuned according to the sample statistics and binning … **four iterations are usually sufficient**". The full error propagation is needed for >1 iteration | Sec. 3.1; Sec. on Bayes errors (Fig. 5) |
| M4 | **Brenner et al., RooFitUnfold, 1910.14654** | method comparison | — | — | — | — | Binning and low-stat bins: "responsibility of the user" | Picks τ or the iteration count such that **coverage P_cov = 68.3% − 1%**. Bias, variance and coverage from toys | Sec. 2.3; Sec. 3.1; Sec. 3.3 |
| M5 | **Blobel, hep-ex/0208022** | method | — | — | Implicit: tridiagonal A with diagonal 1−2ε; **"for values above ε = 0.20 the condition number κ is very rapidly increasing"** (diagonal ≤ 0.6). Example ε=0.22 gives κ=7.9, and the unregularised result still oscillates | "Some resolution is lost if bins with a width as large as … the final resolution" are used. **Recommends m = 2·ndf truth bins initially, n > m reco bins, then combining pairs to reach a full-rank covariance** | Large MC needed for n×m matrix | Regularised LS with rank / effective-d.o.f. cut | Sec. 2 (κ vs ε); Sec. 3 (binning, final example) |
| M6 | **Blobel, TU Dresden seminar, 25 June 2015** (slides, not arXiv: desy.de/~sschmitt/blobel/dresden.pdf) | method review | — | Quotes H1 0904.0929 rule (P,S <25% → merge) | same | **"Folklore: bins width should be close to the resolution (one standard deviation)"**, and flags this as an open question | **"Folklore: a factor of 10 in statistic for Monte Carlo, compared to the data, should be sufficient"** | — | Slide p. 6; p. 7; p. 59 |

### Derived (not cited): what k·σ means for P and S

Take Gaussian smearing σ and a true density that is flat across one bin of width w = k·σ. Then the fraction of true entries that stays in the bin is S(k) = P(k) (computed here, not from a paper):

| k = w/σ | 0.5 | 1.0 | 1.5 | 2.0 | 2.5 | 3.0 | 4.0 | 5.0 |
|---|---|---|---|---|---|---|---|---|
| in-bin fraction | 0.20 | 0.37 | 0.51 | 0.61 | 0.68 | 0.73 | 0.80 | 0.84 |

Two consequences matter for the gates:

- **Combining dimensions.** In a 4D cell with roughly independent smearing in each dimension, the cell-level P and S are roughly the **product** of the 1D values. For example, two dimensions at k=2 and two "wide" dimensions (≈0.9 each) give ≈0.30.
- **WS2 candidate family.** At k=1.0 the 1D retention alone is only 0.37, so only k ≥ 1.5 can reach a 4D cell floor of 0.3. Falling spectra (pT_h, z) and migration out of the fiducial or current-hemisphere region lower purity further at the steep side.

---

## (b) Recommended gate set for this analysis

Each gate is evaluated **per final truth cell (Q², x, z, pT_h), per charge**, on the nominal MC (DJANGOH 1.4) and repeated on RAPGAP 3.1. A cell that fails is merged (rule G8) and is not dropped silently. Where precedent varies, gates are tiered **strict / typical / loose**. *Recommended* marks what I propose to adopt.

| Gate | Strict | Typical (**recommended**) | Loose | One-line justification |
|---|---|---|---|---|
| **G1 bin width vs resolution** (per variable, local σ68 at gen level) | w ≥ 3σ | **w ≥ 2σ in z and pT_h; w ≥ 1.5σ allowed in x where Q²-strata force it; Q² bins typically ≫σ** | w ≥ 1σ (Blobel "folklore", ZEUS "commensurate") | No HERA paper states k numerically (rows 1–18). k=1 alone caps 1D retention at 0.37 (derived table), incompatible with a 4D floor ≥0.3. k=2 gives 0.61 per dimension. Treat k as a **generator of candidates**; P and S are the binding gates |
| **G2 reco binning** | n_reco = 2·n_truth in every dimension that is unfolded | **n_reco ≥ n_truth globally; 2:1 in pT_h (and in x if cheap); 1:1 in z acceptable** (as in the WS2 plan) | n_reco = n_truth (square, IBU only) | TUnfold requires n ≥ m (M1, Sec. 1). Blobel recommends 2× finer then combining (M5, Sec. 3). H1 2011.01812 and 1406.4709 use finer reco grids |
| **G3 purity floor** (cell level) | P ≥ 0.50 | **P ≥ 0.30 in every reported cell; median P ≥ 0.5 per Q² stratum** | P ≥ 0.25 (merge trigger) | Matrix-unfolded H1 jets accept 35–45% (row 3). Bin-by-bin H1 analyses demand 40–75% (rows 1, 4, 7) because there purity *is* the method's validity. H1 high-Q² inclusive uses >30% (row 9), and H1 low-Q² inclusive merges <25% (row 6) |
| **G4 stability floor** (cell level, full gen denominator incl. misses) | S ≥ 0.40 | **S ≥ 0.30** | S ≥ 0.25 | Same precedent as G3 (rows 4, 6, 7, 9). S is what keeps the response diagonal-dominant (see G6) |
| **G5 acceptance × efficiency** (reco-matched / gen in cell) | ≥ 0.40 | **≥ 0.30**; flag cells < 0.40 for an extra model-dependence check | ≥ 0.25 | COMPASS keeps A >0.35–0.40 everywhere (rows 16–18). H1 1302.1321 tolerates correction factors up to 2.6 (A ≈ 0.38) with bin-by-bin |
| **G6 response conditioning** | In each 1D slice of R (truth→reco, efficiency-normalised), diagonal ≥ 0.6 (Blobel ε ≤ 0.2) | **Report cond(R) and the per-cell diagonal; require diagonal ≥ S-floor (G4). Investigate (do not auto-fail) if cond(R) of the purity-normalised square truth×truth sub-matrix in a (Q², x) slice exceeds ~10** | diagnostic only | Blobel (M5, Sec. 2): κ grows "very rapidly" for ε>0.2, and κ=7.9 already oscillates unregularised. No HERA paper quotes a κ limit, and IBU regularises anyway. So κ is a **warning**, with P/S and closure as the pass/fail criteria |
| **G7 adjacent-bin correlations of the unfolded result** (stat) | ρ_adj ≥ −0.4 | **ρ_adj ≥ −0.5** for every adjacent pair | ≥ −0.6 | H1 published −0.5 in PT (1406.4709 Sec. 6) and −0.2 in Q² (1611.03421 Sec. 8.1). H1 2011.01812 widened truth bins precisely to avoid "large negative" correlations. TUnfold guidance: strongly negative ρ means under-regularised (M1, Sec. 4) |
| **G8 merging rule** | — | **If a cell fails G3/G4/G5/G7, merge it with its neighbour along the dimension with the worst σ/w (usually pT_h or x), within the same (Q², x) stratum first; publish at the merged level** | — | H1 0904.0929 merges when P or S <25% (Sec. 4.2). H1 1406.4709/1611.03421 publish merged sums of finer unfolded bins. Blobel combines pairs to full rank (M5) |
| **G9 data statistics** | ≥ 400 reco tracks per reco cell (≤5% Poisson) | **≥ 100 reco tracks per reco cell (≤10% Poisson) AND unfolded stat ≤ 15% per published cell** | ≥ 50 per reco cell | **No surveyed paper states a numeric minimum** (all rows "not stated"). COMPASS 1709.07374 discards bins for "low statistical precision" without a number. These thresholds are a proposal: 100 matches the WS3 provisional gate, and the unfolded-stat cap guards against IBU noise amplification in high-k cells |
| **G10 MC statistics** | MC ≥ 30× data lumi-equivalent; δMCstat ≤ 0.2·δstat | **MC ≥ 10× data (Blobel folklore); propagated response-MC stat ≤ ⅓ of data stat per cell (H1 1611.03421 achieved); Kish n_eff ≥ 100 gen and ≥ 50 matched per cell (WS3 plan)** | δMCstat ≤ ½ δstat | Blobel slides p. 6 (10×). H1 1611.03421 Sec. 6 (≈⅓). H1 2412.14092 Sec. 5 (≈100×, negligible). Kish floors are the plan's, with no published precedent |
| **G11 IBU iteration count** | k fixed by coverage: P_cov ≥ 67.3% (RooFitUnfold rule) | **Smallest k ≥ 2 such that (i) iteration-to-iteration change < 0.2·σ_stat in ≥ 95% of cells, and (ii) the cross-generator non-closure does not dominate the total uncertainty in any Q² stratum; expect k = 2–4. Use one global k for both charges** | k = 4 fixed (RooUnfold default) | D'Agostini: 2–3 steps (M2, Sec. 4). Adye: 4 (M3, Sec. 3.1). H1 2108.12376 picks the iteration count so closure does not dominate. RooFitUnfold uses coverage (M4, Sec. 2.3). The 0.2σ convergence threshold is a proposal |
| **G12 closure: same generator, independent halves** | |mean pull| ≤ 0.1, pull width 0.9–1.1, coverage within 68±3% | **Per-cell |bias| ≤ 0.5·σ_stat(cell) in ≥ 95% of cells, none > 1σ; pull mean within ±0.2, width 0.8–1.2 over 200 toys; refold χ²/ndf ≤ 1.5; no negative unfolded cells** | |bias| ≤ 1σ_stat | H1 2108.12376: self-closure "unbiased within MC statistical uncertainties" (qualitative only). **No surveyed paper gives numeric pull or bias limits.** These are proposals consistent with that qualitative statement |
| **G13 closure: cross generator (DJ ↔ RG)** | Non-closure < ½ total syst per cell | **Non-closure is booked as a model/method systematic, not a gate. Flag cells where it exceeds the data stat by >2× for merging review** | book only | H1 standard: generator difference = model syst (1302.1321 Sec. 4; 0911.5678 Sec. 2.3; 2011.01812 Sec. 4.3; 2108.12376) |
| **G14 TUnfold cross-check** | Same result within 1σ_stat in all cells | **Run TUnfold with τ from ScanLcurve and from min-avg global ρ; require |IBU−TUnfold| ≤ 1σ_stat in ≥ 90% of cells; do not use for selection** | qualitative | H1 1406.4709: L-curve τ consistent with fixed τ (Sec. 3.4). H1 2011.01812: robust to regularisation choice. TUnfold Sec. 4.1–4.2 |
| **G15 boundary/side bins** | — | **Truth: under/overflow in pT_h and z, plus out-of-fiducial (Q², y) and target-hemisphere (η*<0) "miss" rows. Reco: extended box (e.g. 3<Q²<120, y slightly beyond 0.1–0.6) so migrations into the measurement box are unfolded, not subtracted** | — | H1 1611.03421 Sec. 3 (3<Q²<120 extended); H1 2011.01812 Sec. 4.3; HERMES j=0 bin (Sec. III.C.5). ZEUS hep-ex/9903056 Sec. 5: current/target migration reaches 25% at Q²<40, so the η*>0 boundary needs explicit rows |

**Summary of the recommended (typical) set:**
- **Resolution and binning:** w ≥ 2σ68 in z and pT_h (1.5σ in x where forced); n_reco ≥ n_truth, 2:1 in pT_h.
- **Per-cell floors:** P ≥ 0.30, S ≥ 0.30, A·ε ≥ 0.30; ρ_adj ≥ −0.5.
- **Statistics:** data ≥ 100 reco tracks per reco cell and unfolded stat ≤ 15%; MC ≥ 10× data and δMCstat ≤ ⅓ δstat.
- **IBU:** smallest k ≥ 2 with convergence < 0.2σ (expect 2–4).
- **Closure:** self-closure |bias| ≤ 0.5σ in ≥ 95% of cells; pull mean ±0.2, width 0.8–1.2.
- **Cross-checks:** the cross-generator difference is booked as a systematic; TUnfold agrees within 1σ in ≥ 90% of cells; cond(R) is a warning only.

---

## (c) Open questions

1. **4D cell vs 1D floors.** All surveyed purity/stability floors apply to ≤3D cells (most are 2D). Our 4D cells multiply the per-dimension losses. Should G3/G4 be applied to the full 4D cell, which is what is published, or to 1D projections at ≥0.5–0.6 each? I recommend the 4D cell, with 1D projections reported as diagnostics. Precedent does not settle this.
2. **Track-level vs event-level migration.** H1 1302.1321 defines purity per particle. An (x, Q²) event migration moves *all* tracks of the event coherently. Should the stratum-level (x, Q²) purity also be gated at event level (the inclusive H1 rules, rows 6–9) in addition to the per-track cell purity?
3. **Is the 75% purity of H1 1302.1321 the expectation an H1 referee will apply?** It was a bin-by-bin requirement. With a full-matrix IBU, 30–45% has H1 precedent (1611.03421), but only for jets. No H1 matrix-unfolded hadron-spectrum paper with a stated floor was found.
4. **z and pT_h resolution at low Q².** Both depend on the event kinematics (ν/y and the HCM boost), not only on track resolution. At low y the electron-method y resolution degrades, so the local σ68 may be dominated by the event kinematics. WS2 should split σ into "event" and "track" parts before applying G1.
5. **Current-hemisphere boundary.** ZEUS measured 12–25% migration into the current region at Q²<40 (hep-ex/9903056 Sec. 5). Does our S2' response carry η*<0 truth as a miss/fake row, or subtract it? This interacts with G4 and G15.
6. **No published numeric closure/pull limits or cond(R) limit** was found in any HERA, HERMES or COMPASS paper surveyed. G6, G11 (0.2σ) and G12 are proposals, and they need user sign-off as policy.
7. **Minimum statistics** (G9) has no numeric precedent either. Should the floor be on reco counts, on unfolded relative stat, or on both?
8. **Normalisation.** Multiplicities are normalised to N_DIS(x, Q²). HERMES unfolds numerator and denominator consistently (Eq. 6). Should the gates also apply to the inclusive N_DIS(x, Q²) unfolding (event-level P/S ≥ 0.3–0.5, per rows 6–9)?
9. **Charge symmetry of gates.** COMPASS h+/h− acceptances agree to 0.02–0.04. Should a cell that passes for one charge but fails for the other be merged for both, keeping a common binning? I recommend a common binning.
10. **H1 2108.12376 / 2412.14092** are unbinned (MultiFold) at Q²>100–150. They give iteration-choice and model-bias practice but no binned purity/stability numbers. No H1 low-Q² hadron-level TMD measurement with a binned IBU cross-check was found in this survey.

---

### Source list (arXiv IDs as read)
H1: 1302.1321, 2011.01812, 1611.03421, 0911.5678, 1406.4709, 0904.0929, 0904.3513, 1012.4355, 1206.7007, 2108.12376, 2412.14092.
ZEUS: hep-ex/9903056, 1001.4026, hep-ex/9910043; also checked with no numbers found: hep-ex/0608053, 1010.6167, hep-ex/0604032, hep-ex/0701039, 0812.2864.
HERMES: 1212.5407.
COMPASS: 1604.02695, 1608.06760, 1709.07374.
Methods: 1205.6201 (TUnfold), 1010.0632 (IBU), 1105.1160 (RooUnfold), 1910.14654 (RooFitUnfold), hep-ex/0208022 (Blobel); Blobel Dresden 2015 slides (https://www.desy.de/~sschmitt/blobel/dresden.pdf).
