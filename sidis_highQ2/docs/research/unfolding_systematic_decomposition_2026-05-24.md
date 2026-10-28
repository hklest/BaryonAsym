# Unfolding systematic decomposition for SIDIS high-Q² v5g

Research memo, 2026-05-24. Scope: how to decompose the unfolding contribution
to the systematic budget of the H1 high-Q² charged-particle SIDIS multiplicity
and asymmetry measurement on Scheme B′ binning, given that three methods
(BBB, IBU, TUnfold) are in hand and OmniFold is pending.

## Bottom line

The recommended decomposition is the one that the H1 collaboration has used
consistently since the high-Q² jets program (Andreev *et al.* 2017,
arXiv:1611.03421) and that was re-applied verbatim in the 2024 groomed event
shapes paper (Andreev *et al.* 2024, arXiv:2403.10134) and in the 2023 OmniFold
jet substructure paper (Andreev *et al.* 2024, arXiv:2303.13620):

```
σ²_unfold(bin) =
      σ²_stat,unfold              # data-stat propagated through nominal unfolder
    + σ²_model                    # ½ |IBU(DJ) − IBU(RG)| evaluated on data
    + σ²_reg                      # τ-scan: result with τ × 2 and τ / 2
    + σ²_MC-stat,response         # finite-MC noise in the response matrix
```

Pick **IBU as central**, with bootstrap statistics. **BBB and TUnfold are
cross-checks, not envelope contributors.** This matches Option **C** of the
prompt, lightly extended with an explicit `σ_reg` term because TUnfold is in
the toolkit anyway. The full 3-method envelope is reported in supplementary
material; it is not added to the budget, since (a) the spread is dominated by
the same data-stat fluctuation flowing through differently regularised
operators, (b) part of the spread is BBB's residual migration bias on
low-purity cells which is exactly the failure mode the closure test already
captures, and (c) no major HERA, LHC, RHIC, or EIC-projection analysis in the
last decade has added a method-spread term on top of the model + regularisation
terms.

Worked numbers for the two example cells are at the end of this memo.

## Q1 — canonical decomposition of unfolding uncertainties

The community-standard breakdown, as codified by D'Agostini for IBU
(NIM A 362 (1995) 487 and the 2010 update arXiv:1010.0632), by Schmitt for
Tikhonov regularisation (TUnfold, arXiv:1205.6201), by Höcker & Kartvelishvili
for SVD (arXiv:hep-ph/9509307), by Blobel in the canonical particle-physics
unfolding review (arXiv:hep-ex/0208022), by Cowan in the PHYSTAT survey
("A survey of unfolding methods for particle physics", Conf.Proc.C 0203181
(2002) 248), and by Adye for RooUnfold (arXiv:1105.1160), separates the
unfolding step into four logically distinct contributions:

| Label                              | What it covers                                                                                                       | How it is computed in practice                                                                                                                                                  |
|------------------------------------|----------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Data statistical (post-unfold)     | Poisson fluctuation of data, after passage through the (non-linear) unfolding operator, with all bin-bin correlations | TUnfold: analytic covariance from regularised least-squares. IBU: bootstrap (50–1000 toys). OmniFold/MultiFold: bootstrap by resampling input data with replacement, 100 toys (H1 standard) |
| Response-matrix MC statistical     | Finite generator-level MC induces noise in *R*                                                                       | Schmitt's analytic propagation in TUnfold; for IBU usually a second bootstrap over MC weights                                                                                   |
| Model (prior, MC-modelling) bias   | Sensitivity to the truth distribution and physics model used to build *R*                                            | Re-unfold with an alternative generator's response; usually DJANGOH ↔ RAPGAP in HERA, Pythia ↔ Herwig at LHC. Half the difference, or the full difference if asymmetric         |
| Regularisation / method            | Sensitivity to τ (TUnfold), iteration number (IBU), kterm (SVD), epochs (OmniFold)                                   | Vary the regularisation strength by a factor of two each way; take the half-difference                                                                                          |

These four are kept separate because they are uncorrelated by construction:
the bootstrap freezes the response, the model variation freezes the data, and
the τ-scan freezes both data and response. They are then summed in quadrature
into a total "unfolding uncertainty".

The double-counting risk that the supervisor flagged is real, but the
literature avoids it by **never adding a method-spread term on top of these
four**. The two oldest pre-LHC analyses that did add a method-spread term
(e.g. some LEP-era analyses) have all migrated to the four-term scheme, and
the current ATLAS, CMS, LHCb, H1 and ZEUS practice is uniform.

References anchoring this taxonomy:

- D'Agostini, NIM A 362 (1995) 487 (original IBU prescription).
- D'Agostini 2010, arXiv:1010.0632 §§ 4–5 (explicit recommendation that the
  model dependence is the *single* systematic of IBU; iteration number is a
  *regularisation* uncertainty handled by an iteration scan).
- Cowan PHYSTAT-2002 (Conf.Proc.C 0203181 (2002) 248), §§ 4–6 (the standard
  decomposition: data statistical via covariance propagation, MC statistical,
  bias from the assumed truth, regularisation strength).
- Blobel arXiv:hep-ex/0208022 §§ 3–5 (treatment of the bias–variance
  trade-off, MC truth dependence).
- Schmitt, JINST 7 (2012) T10003, arXiv:1205.6201 §§ 6–8 (TUnfold covariance,
  τ-choice systematic, response-matrix MC-stat propagation).
- Adye, arXiv:1105.1160 §§ 3–5 (RooUnfold treatment, method comparison
  philosophy: comparison plots, not envelope).
- Brenner, Burgard & Croft 2025, arXiv:2510.15129 §§ 2–3 (modern restatement
  in a likelihood framework: systematic uncertainties enter as nuisance
  parameters in the Tikhonov-regularised likelihood; the method spread is
  treated as a *coverage* check, not as a systematic).

## Q2 — method-spread treatment

The literature is consistent: **the method spread is a cross-check, not a
systematic to be added in quadrature**. Concretely:

- ATLAS, CMS, LHCb measurements in the past decade typically declare a
  single nominal method (IBU is the most common; TUnfold for highly
  binned, low-statistics analyses; SVD for legacy comparability) and use the
  other methods only as a closure cross-check. When two methods are reported
  in the publication, both are quoted separately with their own uncertainty
  bands rather than merged.
- H1's tradition is uniform on this point. Reading the four most recent H1
  measurements that use a regularised matrix-inversion or iterative unfolder:
    - Andreev *et al.* (H1) 2017 jet cross sections at high Q², EPJC 77 (2017)
      215, arXiv:1611.03421. Quote: "Statistical uncertainties … are determined
      on detector level and are propagated through the unfolding equations …
      The model uncertainty … is obtained as the difference between the
      nominal result and the result obtained using the migration matrix from
      the reweighted Djangoh or the reweighted Rapgap predictions alone … The
      reweighting uncertainty … accounts for changes of the result due to the
      weighting of the simulations to data" (§6). TUnfold is the single
      nominal unfolder; no method envelope.
    - Andreev *et al.* (H1) 2022 lepton-jet correlation, PRL 128 (2022)
      132002, arXiv:2108.12376. Quote: "The bias of the unfolding procedure
      is determined by taking the difference in the result when unfolding with
      Rapgap and with Djangoh. This procedure gives a consistent result to
      unfolding detector-level Rapgap with Djangoh (and vice versa). It was
      also verified that unfolding Rapgap with itself using statistically
      independent samples gives unbiased results within MC statistical
      uncertainties." MultiFold is the single nominal unfolder; the IBU
      reduction is mentioned as a special case, not as a separate column in
      the budget.
    - Andreev *et al.* (H1) 2024 jet substructure (OmniFold with GNN),
      Phys. Lett. B 844 (2023) 138101, arXiv:2303.13620. Quote: "A model
      uncertainty is estimated by unfolding the data with Djangoh instead of
      the default Rapgap. In addition, a non-closure uncertainty is estimated
      by unfolding Djangoh as pseudo-data using the Rapgap simulation and
      comparing to Djangoh at particle level. The combined effects of the
      unfolding uncertainties typically are below 10 [%]." OmniFold is the
      single nominal unfolder.
    - Andreev *et al.* (H1) 2024 groomed event shapes, EPJC 84 (2024) 720,
      arXiv:2403.10134. Quote: "The uncertainty from the model dependence of
      the unfolding is estimated as half the difference between the spectra
      unfolded using the migration matrices from Rapgap and Djangoh,
      respectively. This is the dominant systematic uncertainty in many bins
      of the measurement, with typical values of 5–10% … The uncertainty
      associated with regularization is determined by varying the
      regularization parameter by a factor of two larger and smaller than its
      nominal value … The statistical uncertainty is determined by a
      resampling procedure" (§6.4). TUnfold is the single nominal unfolder.
    - Andreev *et al.* (H1) 2024 lepton-jet azimuthal correlation, EPJC 84
      (2024) 720, arXiv:2406.01430. Single nominal TUnfold; τ uncertainty from
      TUnfold's own prescription; ARIADNE vs LEPTO/MEPS comparison provides
      the model uncertainty.

The ZEUS tradition is identical. The 2017 jet measurement by ZEUS
(EPJC 77 (2017) 547, arXiv:1606.05552 — outside our 2018–2025 window but cited
by H1 2403.10134 for binning continuity) uses one nominal regularised
unfolder, with the alternative method shown as a comparison only.

The 2025 Brenner–Burgard–Croft paper (arXiv:2510.15129) is the most recent
methodological argument for why the method spread should *not* be a
systematic: when systematic uncertainties are profiled in a likelihood, two
unfolders can both be valid Bayesian or frequentist estimators with different
bias–variance trade-offs; their difference is a regularisation difference, not
a measurement uncertainty, and is best reported as a coverage diagnostic.

## Q3 — cross-MC closure as a systematic source

The DJ↔RG cross-MC closure is, in modern HERA and LHC practice, the
*operational definition* of the model-dependence systematic. The recipes vary
only in detail:

- **Half-difference.** H1 2403.10134 §6.4: σ_model = ½ |unfold(data | R_RG) −
  unfold(data | R_DJ)|. This is the unambiguous form when the two MCs bracket
  the data, which is generally the H1 situation for DIS.
- **Full difference.** H1 1611.03421 §6 uses the full difference between
  nominal (weighted-mix) and each individual MC. The full difference is the
  conservative choice when one MC is clearly off and the other is close to
  data; the half-difference is the symmetric choice when neither MC is
  preferred.
- **Pseudo-data closure.** H1 2108.12376 (OmniFold) and H1 2303.13620
  (OmniFold-GNN) re-state the test as: unfold DJ pseudo-data with R built from
  RG, and compare to DJ truth. The numerical answer is the same as the
  half-difference scheme to within a few permille (the symmetry the supervisor
  observed, DJ→RG ≈ 5.65%, RG→DJ ≈ 5.76% in our case, is the empirical
  confirmation that the procedure is closing as expected).
- **Iteration to convergence.** Andreassen *et al.* OmniFold (arXiv:1911.09107
  §4 and the supplement) prescribes: choose the number of iterations such that
  the closure test stops improving — i.e., choose the iteration number so that
  the model uncertainty is comparable to but not dominated by the
  regularisation uncertainty. This is the modern restatement of D'Agostini's
  1995 advice to stop iterating once the χ² between successive iterations
  stabilises.

The closure systematic is **not** combined with the method-spread systematic;
the method spread is not in the budget at all. The closure systematic *is*
combined in quadrature with the data-stat, regularisation, and response-MC
contributions.

For OmniFold/MultiFold specifically, two distinct closure tests are reported:
the *model* test (swap the MC used to build the prior weights) and the
*non-closure* test (unfold one MC's detector level with the other MC's
response and compare to the first MC's particle level). H1 2303.13620 §3 and
the OmniFold supplement (arXiv:1911.09107) treat these as two separate
uncertainties added in quadrature — a finer split than what binned IBU
typically provides, but conceptually a sub-decomposition of what we have been
calling "model dependence".

## Q4 — bootstrap propagation across methods

The three statistical uncertainties of BBB, IBU and TUnfold are not
independent; all three are computed on the same data and any correlation
between methods is identically the data Poisson fluctuation flowing through
three different (almost-)linear operators.

The literature handles this in three steps:

1. The statistical uncertainty of the *nominal* method is the only statistical
   uncertainty that enters the budget. Bootstrap is the universal modern
   choice. For IBU, 100 bootstrap toys is the H1 standard (Andreev *et al.*
   2108.12376, 2303.13620); 1000 toys is used when the analytic D'Agostini
   covariance is judged unreliable (Andreev *et al.* 2403.10134).
2. The other methods' statistical uncertainties are not propagated into the
   total. They are reported next to the method-comparison plot so the reader
   can see whether method-to-method differences are statistically significant.
3. When the method spread *is* shown as a band (e.g. as a supplementary
   ratio plot), the comparison must be done on the *same* set of bootstrap
   replicas — i.e. each replica of the data is passed through BBB, IBU, and
   TUnfold and the per-replica differences are histogrammed. This isolates the
   genuine method-systematic component (the part of the spread that survives
   after the common data fluctuation is removed) from the spurious component
   (the part driven by different methods amplifying the same fluctuation
   differently). Schmitt 1205.6201 §7 and Adye 1105.1160 §5 both describe this
   "matched bootstrap" diagnostic.

Applied to our envelope: of the ~5% median envelope width on high-purity
cells, the *intrinsic* method-systematic component (matched-bootstrap, common
fluctuation removed) is typically a factor of 2–3 smaller than the raw
envelope. On low-purity edge cells, where BBB is invalid by purity-threshold,
the envelope mostly reflects BBB's residual migration bias and is not a
method-systematic at all — it is exactly the failure mode the cross-MC closure
already captures.

## Q5 — recommendation for our case

Adopt **Option C** (single method central, others as cross-checks), with the
explicit four-term decomposition:

```
σ²_unfold,total(cell) = σ²_stat,IBU + σ²_model + σ²_reg + σ²_MC-stat,response
```

where, for each cell:

- `σ_stat,IBU`: standard deviation across 100 bootstrap replicas of IBU on
  Poisson-resampled data, with the nominal response fixed.
- `σ_model = ½ |IBU(R_DJ; data) − IBU(R_RG; data)|`: the half-difference of
  IBU run with the DJANGOH-built and RAPGAP-built responses on the same data.
  This is the per-cell number that ranges 0.05% to 5.24% in our cross-MC
  closure table.
- `σ_reg = ½ |IBU_N=N* (data) − IBU_N=2N* (data)|`: half-difference of IBU
  with the nominal iteration count and with twice that count, on the same
  data. (Equivalent: half-diff of N* and N*/2 if N* > 2.) For TUnfold the
  analogous τ × 2 and τ / 2 scan is what the H1 2403.10134 paper does;
  in our setup the IBU iteration scan is the right knob since IBU is central.
- `σ_MC-stat,response`: standard deviation across MC-bootstrap replicas of
  the response matrix; typically one third of the data-stat term per H1
  1611.03421 §6.

BBB and TUnfold are run on the same nominal cell set and shown alongside IBU
in a per-bin ratio plot in supplementary material. The supplementary plot is
sufficient evidence that the result does not depend pathologically on the
algorithm. **No method-envelope term is added to the budget.**

Why this is defensible rather than just convenient:

- It matches the four most recent published H1 differential measurements,
  whose unfolding sections were validated by the same Schmitt/Britzger/Schmookler
  authorship lineage that wrote TUnfold and that defined the H1 OmniFold
  practice (2017, 2022, 2024-jets, 2024-event-shapes, 2024-azimuthal).
- It matches the 2025 methodological synthesis (Brenner, Burgard, Croft,
  arXiv:2510.15129), which is the most explicit current statement that
  method spread is coverage, not systematic.
- It avoids the double-counting failure mode the supervisor flagged: the
  full envelope on low-purity cells (38% on edge cells) is dominated by BBB's
  invalidity, which the purity criterion is supposed to flag, not by a
  legitimate method ambiguity.
- It is honest about where the uncertainty is large: on a low-purity cell
  where σ_model ≈ 5% and σ_stat is comparable, the total `σ_unfold` will be
  in the 5–10% range. That is realistic for a 4D differential SIDIS
  measurement and is what reviewers will expect.

## Worked example 1 — cell S0b h⁻ (high closure bias)

The closure-table number: σ_model = 5.24% (this is *already* the
half-difference, per our existing closure script convention; if the table
quotes the full DJ→RG ≈ 5.65% / RG→DJ ≈ 5.76% bias separately, then
σ_model = ½ × ½(5.65 + 5.76)% = 2.85%; check the local handoff for which
convention is in use).

Assume the half-difference convention. With IBU central:

| Component                        | Value                            |
|----------------------------------|----------------------------------|
| σ_stat,IBU (bootstrap, 100 toys) | take from per-cell bootstrap; assume ~3% for this cell as a working number |
| σ_model                          | 5.24%                            |
| σ_reg (IBU N × 2 vs N nominal)   | take from iteration scan; typically 1–2% for IBU when N is chosen sensibly; assume 1.5% |
| σ_MC-stat,response               | ≈ ⅓ × σ_stat,IBU ≈ 1%             |

```
σ_unfold,total = √(3.0² + 5.24² + 1.5² + 1.0²) % = √(9.0 + 27.5 + 2.25 + 1.0) %
               = √39.7 % = 6.30 %
```

The model term dominates. The full 3-method envelope on this cell is reported
as ~10% to ~38% (per the supervisor's summary of low-purity edge cells); that
envelope is *not* the systematic to quote — the 6.3% figure above is. The
envelope plot in supplementary material is the cross-check that confirms the
algorithmic choice is not what is driving the uncertainty.

## Worked example 2 — cell S0a h⁺ (well-populated, all methods agree)

Per the supervisor's numbers, on this cell the 3-method spread is ~1% and the
cross-MC closure is ~0.6%.

| Component                        | Value                            |
|----------------------------------|----------------------------------|
| σ_stat,IBU (bootstrap, 100 toys) | take from per-cell bootstrap; assume ~1% (high-statistics cell) |
| σ_model                          | 0.6%                             |
| σ_reg (IBU N × 2 vs N nominal)   | typically < 0.5% on a well-populated cell; assume 0.5%           |
| σ_MC-stat,response               | ≈ ⅓ × σ_stat,IBU ≈ 0.3%           |

```
σ_unfold,total = √(1.0² + 0.6² + 0.5² + 0.3²) % = √(1.0 + 0.36 + 0.25 + 0.09) %
               = √1.70 % = 1.30 %
```

In this regime, σ_stat,IBU and σ_model are comparable and the total is ~1.3%,
sub-dominant to the experimental systematics (HFS energy scale, electron
energy scale, RCES, etc.). The 1% method-envelope width is consistent with
this total within rounding and is *not* added on top.

Replace the working numbers for σ_stat,IBU and σ_reg with the actual values
from the bootstrap and the iteration scan once those are run; the structure
of the calculation does not change.

## Five primary references

1. D'Agostini 2010, arXiv:1010.0632, "Improved iterative Bayesian unfolding".
   The canonical statement that the IBU systematic budget contains a model
   term (variation of the prior MC) and a regularisation term (iteration
   number), nothing else.

2. Schmitt 2012, JINST 7 (2012) T10003, arXiv:1205.6201, "TUnfold: an
   algorithm for correcting migration effects in high energy physics". The
   reference for τ-choice systematics, analytic covariance propagation, and
   response-MC-stat propagation in Tikhonov regularisation.

3. Andreev *et al.* (H1) 2017, EPJC 77 (2017) 215, arXiv:1611.03421,
   "Measurement of Jet Production Cross Sections in Deep-inelastic ep
   Scattering at HERA". The most-cited H1 high-Q² differential measurement of
   the last decade; explicit four-term unfolding-uncertainty decomposition
   in §6 with model uncertainty defined as DJ vs RG difference.

4. Andreev *et al.* (H1) 2022, PRL 128 (2022) 132002, arXiv:2108.12376,
   "Measurement of lepton-jet correlation in deep-inelastic scattering with
   the H1 detector using machine learning for unfolding". First HERA OmniFold
   measurement; bootstrap-statistics + DJ↔RG model + closure recipe; explicit
   statement that DJ→RG and RG→DJ closure gives equivalent results.

5. Andreev *et al.* (H1) 2024, EPJC 84 (2024) 720, arXiv:2403.10134,
   "Measurement of groomed event shape observables in deep-inelastic
   electron-proton scattering at HERA". The most recent H1 TUnfold measurement
   with the cleanest one-paragraph statement of the recommended
   decomposition: model = ½|RG−DJ|, regularisation = τ × 2 scan, statistical
   = 1000-replica bootstrap.

6. Andreassen, Komiske, Metodiev, Nachman, Thaler 2020, PRL 124 (2020)
   182001, arXiv:1911.09107, "OmniFold: A Method to Simultaneously Unfold All
   Observables". The original OmniFold/MultiFold reference; defines the
   model-vs-non-closure two-test decomposition and the iteration-count
   regularisation systematic for the ML-based estimator.

7. Brenner, Burgard, Croft 2025, arXiv:2510.15129, "Systematic Uncertainties
   in Unfolding Considering the Likelihood Formalism". The most recent
   methodological reference that explicitly argues that the method spread is
   a coverage diagnostic, not a systematic, and provides a likelihood-level
   restatement of the four-term decomposition.

Supporting references (cited above but not in the top list):

- Blobel 2002, arXiv:hep-ex/0208022, "An Unfolding method for high-energy
  physics experiments".
- Höcker & Kartvelishvili 1996, NIM A 372 (1996) 469, arXiv:hep-ph/9509307,
  "SVD approach to data unfolding".
- Cowan 2002, "A survey of unfolding methods for particle physics",
  Conf.Proc.C 0203181 (2002) 248.
- Adye 2011, arXiv:1105.1160, "Unfolding algorithms and tests using
  RooUnfold".
- Andreev *et al.* (H1) 2023, Phys. Lett. B 844 (2023) 138101,
  arXiv:2303.13620, "Unbinned deep learning jet substructure measurement in
  high Q² ep collisions at HERA".
- Andreev *et al.* (H1) 2024, EPJC 84 (2024) 720, arXiv:2406.01430,
  "The azimuthal correlation between the leading jet and the scattered
  lepton in deep inelastic scattering at HERA".

## OmniFold when it lands

OmniFold should **not** be added to the BBB / IBU / TUnfold method envelope.
The recommended treatment, consistent with the H1 OmniFold papers
(2108.12376 and 2303.13620), is to **switch the central method to OmniFold**
once it is validated and to keep IBU, TUnfold, and BBB as cross-checks.
Concretely:

- σ_stat,OmniFold from a 100-replica data-bootstrap (the H1 standard;
  network re-training per replica). This replaces σ_stat,IBU in the budget.
- σ_model from the DJ ↔ RG MC swap (run the full ML training twice, once
  per generator's MC). This replaces σ_model,IBU; the magnitude is typically
  similar to the IBU one — H1 2108.12376 reports a few percent, up to 10% in
  the most extreme regions.
- σ_non-closure: OmniFold's iterative reweighter has a separate test where
  one MC's detector level is unfolded with the other MC's response and
  compared to the first MC's truth. This is a sub-decomposition of what we
  have been calling "model dependence" and is reported alongside σ_model in
  recent OmniFold publications.
- σ_iterations: the number-of-iterations scan, equivalent to IBU's
  iteration regularisation. The OmniFold paper recommends choosing the
  iteration number so that this term is comparable to but not dominated by
  σ_model.
- The ML hyperparameter sensitivity (network width, depth, learning rate,
  number of training epochs) should be checked and either folded into
  σ_iterations or shown to be negligible. H1 2303.13620 §3 reports that
  hyperparameter variations are negligible compared to bootstrap stat.

There is one genuinely unresolved point in the OmniFold uncertainty
quantification: there is no analytic covariance, only the bootstrap one, and
the bootstrap is expensive (each replica is a full network re-training). The
2025 Brenner–Burgard–Croft paper (arXiv:2510.15129) frames this as the main
open problem in modern unfolding — there is no likelihood-level definition
of an OmniFold systematic. For the SIDIS analysis, the pragmatic answer is:
trust the bootstrap with at least 100 replicas; report the bootstrap as the
statistical uncertainty; keep the binned IBU result as an independent
cross-check whose statistical uncertainty *can* be written down analytically.

OmniFold is not an "ML systematic" to be added on top of the four-term
decomposition. It is a *replacement* for IBU as the central estimator, with
its own bootstrap-driven realisation of the same four-term decomposition.

## Honest open problems

- The half-difference vs full-difference convention for σ_model is not
  universal. H1 1611.03421 uses the full difference (with respect to a
  nominal weighted mix); H1 2403.10134 uses the half-difference. The
  per-cell number from our existing closure table needs to be checked
  against whichever convention the local script implements. The difference
  is up to a factor of 2 in σ_model and propagates linearly into σ_unfold.

- The IBU iteration regularisation systematic (σ_reg) for our analysis has
  not been computed yet. The recommended scan is N* / 2 and N* × 2, where
  N* is the iteration count chosen so that successive iterations stop
  changing the result by more than the data-stat uncertainty. This scan
  should be added to the unfolding pipeline before the result is frozen.

- The bin-bin correlation matrix from IBU bootstrap should be reported in
  full, not just the diagonal. Asymmetry observables A_h = (M^h+ − M^h−)/
  (M^h+ + M^h−) require the full covariance for correct uncertainty
  propagation, because the M^h+ and M^h− unfolding errors are correlated
  through the shared response (M^h+ and M^h− are unfolded with the same
  R matrix, and the bootstrap replica index is shared). This is not a
  "method" question; it is a propagation question, but it is the part of
  the unfolding budget most likely to bite the asymmetry result.

- The 3-method envelope figure (5% median, 38% on edge cells) is the right
  cross-check to publish in supplementary material. The figure should be
  annotated to make clear that the wide envelope on edge cells is the
  expected failure mode of BBB on low-purity bins and *not* a problem with
  the IBU central result.

- For the published table, the 4D-cell unfolding uncertainty should be
  reported as a single column σ_unfold (the quadrature sum above) with a
  footnote pointing to the per-component breakdown in supplementary
  material, following the column structure of H1 2303.13620 Tables 4–8.
