# TUnfold Optimization from the Schmitt 2012 Paper

**Author:** physics-research agent
**Date:** 2026-05-31
**Scope:** Apply Schmitt (2012, JINST 7 T10003, arXiv:1205.6201) and Schmitt
(2017, EPJ Web Conf. 137 11008, arXiv:1611.01927) to the H1 high-Q² SIDIS
TUnfold rev3 pipeline. The goal is to identify which currently observed
pathologies (~30–50% bin-by-bin cross-MC model uncertainty; max|ρ|>0.95 at
every τ in the scan; condition number 10^15–10^21) are explained or
addressed by the algorithm authors' own prescriptions, and which require
physics-side changes outside the paper.

This research file is informational. It does NOT modify code, change rev3
outputs, or trigger NAF jobs. All claims are anchored to sections,
equations, or explicit prescriptions in the Schmitt papers (referred to as
**S12** for 1205.6201 and **S17** for 1611.01927) or in the ROOT
TUnfoldDensity reference documentation (**ROOT-DOC**,
https://root.cern/doc/master/classTUnfoldDensity.html).

---

## 1. Summary of the TUnfold algorithm per Schmitt 2012

(Half page; every claim is anchored to a section, equation, or
prescription.)

TUnfold solves the linear folding equation $\tilde{y}_i = \sum_j A_{ij}
\tilde{x}_j$ (S12 eq. 1; S17 eq. 2) by minimising a Lagrangian
$L = L_1 + L_2 + L_3$ (S12 eq. 3) where $L_1 = (y - Ax)^T V_{yy}^{-1} (y -
Ax)$ is the data χ² (S12 eq. 4), $L_2 = \tau^2 (x - f_b x_0)^T (L^T L)
(x - f_b x_0)$ is the Tikhonov regularisation term (S12 eq. 5), and $L_3 =
\lambda (Y - e^T x)$ is an optional area constraint (S12 eqs. 6–8) where $Y
= \sum_i y_i$ and $e_j = \sum_i A_{ij}$ is the per-truth-bin efficiency
vector. The stationary point is given by S12 eqs. 12–15, leading to a
closed-form estimator $x = E[A^T V_{yy}^{-1} y + \tau^2 (L^T L) f_b x_0]
+ (\lambda/2) E e$ with $E = (A^T V_{yy}^{-1} A + \tau^2 L^T L)^{-1}$
(S12 eq. 14). Covariance propagation to $V_{xx} = D^{xy} V_{yy}
(D^{xy})^T$ follows from S12 eqs. 16–18. Statistical and systematic
uncertainties on the response matrix $A$ are propagated via S12 eqs.
36–47 (appendix), where systematic shifts are stored as 1-σ vectors $\delta
x$ contributing $V_{xx}^{\delta M} = \delta x (\delta x)^T$ (S12 §6).

Two algorithmic constraints are imposed by the formalism itself:
**(i)** $\dim(x) \le \dim(y)$, with strict preference for $\dim(y) >
\dim(x)$ to retain degrees of freedom for the least-square fit (S12 §1, p.
1, sentence beginning "In order to obtain best results... the number of
degrees of freedom, $n-m$, has to be larger than zero. It means that the
data $y_i$ have to be measured in finer bins than are extracted by the
unfolding procedure"); **(ii)** $V_{yy}$ must have full rank, and the rows
of $A$ must be linearly independent (S12 §2.1). Failure to satisfy (ii)
gives the catastrophic condition numbers reported in our rev3 diagnostics.

The Monte Carlo response matrix $M$ has one extra row (index $i=0$) for
generator-level "miss" events that are not reconstructed in any bin (S12
§3); $A_{ij} = M_{ij}/s_j$ with $s_j = \sum_{i=0}^n M_{ij}$ (S12 eqs. 19,
20), and the bias vector defaults to $x_0 = s_j$ (S12 eq. 21). The
regularisation matrix $L$ may regularise the size (1 non-zero per row;
$L = I$, $\dim n_R = n$), the first derivative (2 non-zero per row,
$L_{i,i} = -1$, $L_{i,i+1} = +1$, $\dim n_R = n - 1$), or the curvature /
second derivative (3 non-zero per row, $L_{i,i} = 1$, $L_{i,i+1} = -2$,
$L_{i,i+2} = 1$, $\dim n_R = n - 2$) (S12 §7, items 1–3). Multi-dimensional
generalisations use bin-width and inter-bin-distance corrections (S12
§7.2.1–7.2.3, eqs. 29–35) so that the regularisation acts on densities
rather than raw counts and so that derivatives in different dimensions are
on a common scale via a dimension-specific normalisation constant
$\Delta^d$ (S12 §7.2.2, eq. 31 and below). The strength $\tau^2$ is
chosen by either the L-curve method (S12 §4.1, eqs. 22–24) or by scanning
average / maximum global correlation coefficients $\rho_i$ (S12 §4.2, eq.
25). The 2017 followup (S17 §4.4, §4.7, Tab. 2) explicitly compares
these procedures on a toy and concludes that L-curve "seems to give good
results when choosing the regularisation parameter by means of the L-curve
method" and that "no matter which unfolding method is used, detailed
closure tests are required to quantify the level of bias" (S17 §5).

---

## 2. Per-idea verdicts

Each idea (a–j and the three physics-side items) is graded **ACCEPT** /
**REJECT** / **DEFER**, with the paper section(s) and a one-paragraph
justification.

### (a) Switch to `TUnfoldBinning` — **ACCEPT (high priority)**

**Anchor:** S12 §7.1 and §7.2.3 (eqs. 28, 35); ROOT-DOC "Why to use complex
binning schemes" and the constructor signature
`TUnfoldDensity(hA, histmap, regmode, constraint, densityMode,
outputBins, inputBins, regularisationDistribution,
regularisationAxisSteering)`.

Schmitt's §7.1 example explicitly shows how the L matrix for a 2-D
$(P_T, \eta)$ binning is laid out: the curvature is applied per η-row
internally (rows 1–6 of eq. 28) and a second block (rows 7–10) couples
adjacent η bins within each $P_T$ row. The block structure precisely
matches our 4-D problem (Q² × x × z × $p_T^h$). Doing this by hand with
`RegularizeCurvature(i, i+1, i+2, 1.0)` on flat indices (our current
implementation) is brittle and is exactly what S12 §7.2.3 (the
"more-complicated regularisation scheme" example) and the ROOT-DOC are
designed to replace. Using `TUnfoldBinning` does three things our current
code does not: (1) it computes the bin-width-aware coefficients of eq. 32
automatically, so adjacent bins of unequal width are not over-regularised;
(2) it sidesteps any indexing mistakes when we add or remove cells; (3) it
exposes the `regularisationAxisSteering` string ("`*[UOB]`" by default;
see ROOT-DOC constructor) which controls whether under- and over-flow
bins are included in the L matrix — this is the same control surface that
S12 calls out as "care has to be taken" in §7.1. Because all of (g) below
hangs on whether boundary triplets are correctly excluded, and (f) hangs
on densityMode, both of those fixes are easier with `TUnfoldBinning`
than without. The cost is a one-time builder rewrite; the gain is one of
the few places Schmitt explicitly recommends his framework over manual
matrix construction.

### (b) Use `kRegModeDerivative` instead of `kRegModeCurvature` for some axes — **DEFER (toy-test it; do NOT switch globally)**

**Anchor:** S12 §7 (items 2 and 3); ROOT-DOC "How to choose the
regularisation settings".

S12 §7 itself is neutral on which is "better": it lists size, derivative,
and curvature as the three primitives without recommending one. However,
the ROOT-DOC explicitly states that "size regularisation does not work
well with the tau-scan" (which we'd want to use; see (i)) "the tau-scan is
expected to work better with 1st or 2nd derivative regularisation, because
at some point the negative correlations from migrations are approximately
cancelled by the positive correlations from the regularisation
conditions." Curvature is the default in
`TUnfoldDensity::TUnfoldDensity(...)` (ROOT-DOC constructor, default
`regmode = kRegModeCurvature`), and S12 §4.1 and S17 §4.3 both use
curvature in their worked toy. Our observed pathology — adjacent
published-bin pairs with ρ → 1.0 — is a known consequence of
**curvature** regularisation on adjacent bins, not derivative: the L
operator $(1, -2, 1)$ couples three consecutive bins and the kernel
$L^T L$ has positive off-diagonals at offset ±1 and ±2, so neighbouring
bins acquire positive correlation. Derivative would couple only two
neighbours, slightly reducing the off-diagonal coupling but not removing
it. The principled fix is **bin-width-aware curvature within each (Q², x)
cell** (item (a)) plus excluding boundary triplets (item (g)), not a
switch to derivative. **Defer**: try `kRegModeDerivative` on the
$p_T^h$ axis only as a sensitivity (where the spectrum is steeply
falling and the second-derivative penalty over-suppresses real shape),
keep curvature on $z$ (where the spectrum is comparatively flat).

### (c) Add `kRegModeSize` (or `RegularizeSize`) on the low-stat z<0.1, low-pT corner — **REJECT**

**Anchor:** S12 §7 item 1; ROOT-DOC "How to choose the regularisation
settings" (explicit warning).

`kRegModeSize` sets $L = I$ (S12 §7 item 1) and so penalises $\|x - f_b
x_0\|^2$ — i.e., pulls the corner toward the MC prior, not toward zero
(unless the prior is zero). The ROOT-DOC has an explicit warning: "*size*
regularisation does not work well with the tau-scan: the higher tau, the
smaller rho, but there is no minimum. As a result, large values of tau
(too strong regularisation) are found." Schmitt's preferred remedy for
low-statistics corners is not size regularisation; it is **either**
removing the corner from the unfolded distribution (treating it as
acceptance-loss and reporting only restricted-fiducial bins) **or**
grouping bins of low statistics together using the
`TUnfoldBinning::AddBinning(...)` mechanism. The bias term $f_b x_0$ is
already exactly the MC-pull we'd be re-implementing with size
regularisation. **Reject**: low-z / low-$p_T$ misbehaviour should be
addressed by binning, not by a third regularisation mode that the algorithm
authors warn against using with a τ-scan.

### (d) Use a non-flat bias prior via `DoUnfold(tau, input, scaleBias)` — **REJECT for the published result; ACCEPT only as a bias-systematic** 

**Anchor:** S12 §2.2 (eq. 5 and the paragraph below it), S12 §3 (eq. 21).

S12 makes the role of the bias explicit: "If $f_b = 1$, deviations of $x$
from $x_0$ are suppressed" (S12 §2.2 just below eq. 5). The default in
TUnfold is $x_0 = s_j$ (S12 eq. 21), which is the MC truth used to build
$A$ — i.e., what we already have. Passing a different MC truth via the
`scaleBias` argument *does not change the response matrix*; it only
changes which distribution the regularisation pulls toward at large τ.
The 30–50% cross-MC spread we observe is a real comparison between
rapgap and djangoh truths through the matrix, **not** a bias-prior
artifact, so changing $x_0$ would not reduce the spread, it would just
choose which generator wins the regularisation pull. S17 §4.3 again
defaults to "$x_B$ = zero or Monte Carlo truth" without recommending a
custom prior. The legitimate use of a non-flat bias is as a **systematic
study**: run the unfolding with $x_0$ = rapgap and $x_0$ = djangoh at a
common (small) τ and quote the difference as a bias systematic. This is
what we already do via the cross-MC step — we just don't call it that.
**Reject as a fix**, accept as a re-labelling of the existing study.

### (e) Change `kEConstraintArea` → `kEConstraintNone` — **DEFER (test, but Schmitt argues for keeping Area)**

**Anchor:** S12 §2.2 (eqs. 6–8, 15), the paragraph after eq. 8; S17 §4.2,
Tab. 1.

S12 §2.2 motivates the area constraint precisely: "This procedure is
applied in order to limit possible biases on the normalisation which are
present if the data $y$ follow Poisson's statistics whereas the least
square ansatz is strictly valid only for normal distributed measurements.
The problem is discussed in more detail in literature, for example in
[13] (Cowan, 1998)." S17 Tab. 1 shows the unconstrained template fit
yields $Y_{\rm unf} = 4572$ vs expectation 4584 (bias ~0.3%), while the
constrained fit and Tikhonov-with-area reproduce 4584 exactly. **For a
counting cross section** (our case), the area constraint is the right
choice and removing it will *introduce* an O(1%) normalisation bias of
the kind shown in S17 Tab. 1. **However**, the rev3 symptom that "at
moderate τ the unfolded total drops by ×5" is incompatible with the
area constraint being honoured. That implies either (i) the constraint
is being satisfied but only when reweighted by the efficiency vector $e$
(S12 eq. 8, 15) and the efficiency vector contains a huge mass at the
miss row; or (ii) our F1 overflow patching (rev2→rev3) is interacting
badly with the constraint. **Defer**: run *one* diagnostic unfolding with
`kEConstraintNone` at small τ and verify whether $Y_{\rm unf}$ jumps; if
it does, the area constraint *is* working and the τ>1e-3 collapse is
elsewhere (most likely a row-rank deficit when curvature couples cells —
see (a) and (g)). Do not switch the default.

### (f) Use `kDensityModeBinWidth` — **ACCEPT (high priority, paired with (a))**

**Anchor:** S12 §7.2.1 (eqs. 29, 30); S12 §7.2.2 (eqs. 31, 32); S12
§7.2.3 (eq. 35, with worked numbers); ROOT-DOC `EDensityMode` enum
documentation and the constructor default `kDensityModeBinWidthAndUser`.

Our published Scheme B' bins are decidedly non-uniform: z bin widths of
[0.05, 0.05, 0.1, 0.15, 0.2, 0.25] (so bins differ by ×5 in width across
the z range, with the steepest density change at low z) and $p_T^h$ bin
widths of [0.5, 1.0, 1.5, 2.0, 5.0] GeV (×10 in width). Currently we use
`kDensityModeNone`. S12 §7.2.1 is explicit: "The regularisation schemes
discussed so far do not take into account the effects of non-uniform bin
widths" (intro paragraph). Without density mode, eq. 28's curvature
$(x_{i+2} - x_{i+1}) - (x_{i+1} - x_i)$ is applied as if all bins have
unit width. A bin-width difference of ×10 means the curvature penalty
under-weights changes across narrow bins by a factor of ~100 relative to
wide bins (because $L^T L$ scales like 1/width²). The result is: the
regulariser systematically over-smooths narrow bins (low-z, low-$p_T$)
and under-smooths wide bins (high-z, high-$p_T$). This *exactly* matches
our symptom that "z=0..0.1 bins behave significantly worse than z>0.1
bins". Switching to `kDensityModeBinWidth` (or
`kDensityModeBinWidthAndUser` per the default constructor) applies S12
eqs. 31–32 automatically. The default constructor in ROOT-DOC is in fact
`kDensityModeBinWidthAndUser`; our explicit override to `kDensityModeNone`
is a regression against the algorithm authors' default. **Accept and
revert to the constructor default.** This is the single change in the
rev4 list that requires `TUnfoldBinning` to be the carrier of the bin
widths (see (a)); the two are coupled.

### (g) Boundary effects — first/last bin of each (cell, z) and (cell, $p_T^h$) group — **ACCEPT (high priority)**

**Anchor:** S12 §7 last paragraph ("it is also possible to choose neither
of the basic types and to set up details of the regularisation for
specific bins or groups of bins instead"); S12 §7.1 (eq. 28 shows the
block structure where curvature triplets do NOT span the boundary between
the first and second η bins — rows 1–2 act only inside η-bin 1, rows 3–4
only inside η-bin 2, etc.); ROOT-DOC `regularisationAxisSteering` default
`"*[UOB]"` (U = underflow, O = overflow, B = boundary excluded from
regularisation).

This is the **single most likely fix** for the pathological cell-edge ρ →
1.0 we observe. Our manual L construction uses
`RegularizeCurvature(i, i+1, i+2, 1.0)` for 144 $p_T^h$ triplets and 160
z triplets. The doc claims "no smoothing across (Q², x) cell boundaries",
but we have not independently verified that the triplet at the *end* of
one cell's $p_T^h$ run does not accidentally span into the start of the
next cell's run when the flat index advances. Even more importantly,
S12 eq. 28 shows that curvature on the η axis is applied only at the
first 4 $P_T$ bins (rows 7–10 each pick one $P_T$ and apply curvature
across three η bins; the matrix has zero entries linking across $P_T$
boundaries). The structural rule is: **curvature must be applied within
the manifold defined by holding all other dimensions fixed**, and the
first / last bins along the regularised axis do NOT get a triplet (because
they have no neighbour on one side). If our 144 + 160 triplet count omits
the correct boundary exclusions, we will be regularising fictitious
"wrap-around" structures and producing the singular pairs that drive
max|ρ| → 1.0. **Accept**: as a first action, audit the L matrix
construction; flag every triplet whose three bins are not all within a
single (Q²-bin, x-subbin, z-bin-for-pT triplets) or (Q²-bin, x-subbin,
$p_T$-bin-for-z triplets) group; verify count equals $(N_{p_T} - 2) \times
N_z \times N_{\rm cell}$ + $(N_z - 2) \times N_{p_T} \times N_{\rm cell}$.
With Scheme B' that is $(5 - 2) \times 6 \times 8 + (6 - 2) \times 5 \times
8 = 144 + 160 = 304$, which matches our reported count — but the *content*
of each triplet still needs verification that no triplet crosses a cell
boundary. The natural way to enforce this is item (a): hand the
boundary problem to `TUnfoldBinning` which uses the
`regularisationAxisSteering` default `"*[UOB]"` to omit underflow,
overflow, and boundary bins automatically.

### (h) Add `AddSysError` for the per-MC response uncertainty — **ACCEPT (high priority)**

**Anchor:** S12 §6 (eq. for $V_{xx}^{\delta M} = \delta x (\delta x)^T$);
S12 §B (eqs. 36, 42); ROOT-DOC `AddSysError(...)` method signature with
`ESysErrMode` enum.

Right now we run two independent unfoldings (one with rapgap response,
one with djangoh response) and quote the bin-by-bin difference as a
"cross-MC model" systematic. S12 §6 (last sentence: "TUnfold supports
multiple sources of systematic variation") and S12 §B (eq. 42 in
particular) describe the alternative: choose one $M$ as nominal, register
the second as a 1-σ shift via `AddSysError(hSysM_djangoh, "RGvsDJ",
kHistMapOutputVert, kSysErrModeMatrix)`, and the algorithm propagates
$\delta x = C(\delta A)^T z - D^{xy}(\delta A) x$ analytically. This is
**not just a re-labelling**: doing it inside TUnfold gives three things
that the difference-of-two-unfoldings does NOT give. (1) The shift δx is
evaluated at the same τ, with the same regularisation acting on it, so
the systematic is consistently regularised rather than being the
difference between two independent regularisations that may have hit
different points on the L-curve. (2) The shift is linear in δM (eq. 42),
which automatically suppresses the spurious "147× outlier bins" we see in
the differencing — because those outliers are almost certainly cases
where the two independent unfoldings landed on different singular
solutions, not where the response matrix actually differs by 147×. (3)
The covariance contribution $V_{xx}^{\delta M}$ enters cleanly into
`GetEmatrixSysSource(...)` and into the total error budget without
double-counting. **Accept**: use rapgap as nominal $A$; register djangoh
$A$ as a `kSysErrModeMatrix` source; drop the two-unfold difference from
the published-bin systematic and replace with the analytic shift. Expect
the cross-MC contribution to drop from p90 = 97% to something of order the
matrix difference itself (typically ~5–15% per bin).

### (i) τ choice via `ScanLcurve` / `ScanTau` instead of our manual scan — **ACCEPT, but with caveats**

**Anchor:** S12 §4.1 (eq. 24 for the L-curve curvature); S12 §4.2 (eq. 25
for global ρ); S17 §4.4 (Fig. 4 L-curve example), §4.7 (Tab. 2 says
both methods pass on the toy); ROOT-DOC "How to choose the regularisation
settings" which gives the operating manual.

Our manual scan with a custom score (mean_abs_rho + 0.5·χ²-deviation +
0.5·neg_frac + ...) is unsupported by either Schmitt paper. The algorithm
authors implemented *exactly two* sanctioned procedures: L-curve scan and
global-correlation scan. S17 explicitly tested both on a toy and reported
that both pass the data tests (S17 Tab. 1, rows "Tikhonov τ=0.0068" and
"Tikhonov τ=0.012", corresponding to L-curve and ρ_avg minimisation
respectively). ROOT-DOC then gives a crucial diagnostic prescription:
"whichever algorithm is used, the output has to be checked: (1) The
L-curve should have approximate L-shape and the final choice of tau
should not be at the very edge of the scanned region; (2) The scan
result should have a well-defined minimum and the final choice of tau
should sit right in the minimum." Our rev3 choice (τ = 1.83e-6, *the
smallest non-rejected* of 20 log-spaced points τ ∈ [1e-7, 10]) is exactly
the "edge of the scanned region" failure mode that ROOT-DOC warns
against. The ROOT-DOC also warns that "the L-curve scan does not work if
... the input data have no statistical fluctuations [identical MC events
are used to fill the matrix of migrations and the vector y for a 'closure
test']" — this is a constraint we must respect when running closure
tests. **Accept**: switch to `ScanLcurve()` for the central τ on real
data, and ALSO run `ScanTau(..., kEScanTauRhoAvg)` and
`ScanTau(..., kEScanTauRhoMax)`. Require the L-curve to actually be
L-shaped (cubic-spline fit converges); require the ScanTau result to
sit in a real minimum (not on the edge). Quote τ-systematic via
`GetDeltaSysTau(...)`. If neither built-in scan converges, that is itself
a signal that our problem is misspecified (most likely (a)+(f)+(g) are
already preventing a real L-curve from forming).

### (j) Two-stage unfolding (unfold to coarse, then refine) — **REJECT**

**Anchor:** Not present in S12 or S17.

Neither Schmitt paper describes or recommends two-stage unfolding. S12 §1
explicitly states that the required structure is $n \ge m$ in a *single*
fit, and S17 §5 closes with "in summary, no matter which unfolding method
is used, detailed closure tests are required to quantify the level of
bias". Two-stage unfolding doubles the bias surface (one bias per stage)
and complicates uncertainty propagation through a step that is not in the
TUnfold formalism. **Reject**.

---

### Physics-side ideas

### (P1) GEN fiducial |η|<4 vs RECO |η|<1.3 mismatch (0.55 efficiency) — **DEFER to physics-analyst review**

**Anchor:** S12 §1 ("part of the signal process may be generated outside
the phase-space covered by $x$ and thus has to be counted as background")
and S12 §5 (where this exact case is called out: "background has to
include all types of events which are possibly reconstructed in one of
the bins of $y$ but do not originate from any of the bins of $x$").

Schmitt's design pattern is: define the unfolded fiducial to match (or
be slightly narrower than) the reconstructed fiducial, and treat the
remainder as background to be subtracted by `SubtractBackground(...)` or
by event-by-event categorisation. Our Option C choice (|η_gen|<4)
creates a huge "extrapolation" task — events generated at |η|>1.3 cannot
be reconstructed at all and must rely entirely on $A$'s acceptance
factor — and Schmitt would warn that this is the regime where the
unfolding bias becomes hard to control. However, the published fiducial
choice is a physics-side decision (Option C was chosen for compatibility
with prior H1 charged-particle measurements). **Defer**: flag to
`h1-physics-analyst` that the 0.55 efficiency is the upstream driver of
the regularisation difficulty, and recommend re-running with
|η_gen|<2.0 as a sensitivity to quantify how much of the cross-MC
spread is acceptance vs. detector resolution. Do not change Option C
unilaterally.

### (P2) Log-pT binning to reduce migration — **DEFER to binning critic**

**Anchor:** S12 §7.2.1 (eq. 29; arbitrary user function $f_j^{\rm user}$
allows compensating "known kinematic factors" — the footnote gives DIS
"reduced cross section" as the example).

Schmitt explicitly designed the density mode (item (f)) to absorb
power-law factors in steeply-falling spectra. The footnote on p. 7 of
S12 says: "An example is the use of the 'reduced cross section' rather
than the ordinary cross section for inclusive deep-inelastic scattering
[H1, hep-ex/9908059]. The ordinary cross section changes by several
orders of magnitude as a function of kinematic variables and hence is
difficult to regularise. In contrast, the reduced cross section does not
vary a lot, and thus is more natural to regularise on." This applies
verbatim to our $p_T^h$ spectrum, which drops by ~3 orders of magnitude
across the published range. A log-binning or, equivalently, a
$f^{\rm user}_j = (p_T^{h,j})^{n}$ density-mode weight would flatten
the regularised quantity. **Defer**: the binning is already frozen as
Scheme B' (canonical 2026-05-24). Document this as a future Scheme C
candidate to consider only after rev4 stabilises with (a)+(f)+(g)+(h).

### (P3) Finer reco binning — **ACCEPT (verify, possibly increase)**

**Anchor:** S12 §1 (paragraph after eq. 2): "the data $y_i$ have to be
measured in finer bins than are extracted by the unfolding procedure.
This condition $n \ge m$ is in contrast to some other commonly used
unfolding methods, where often the restriction $n = m$ is imposed."

We currently have $n_{\rm reco}/n_{\rm gen} = 960/240 = 4$. Schmitt
never gives an explicit prescription for the ratio, but the cited H1
analyses and the S17 toy use ratios of 2–4 (S17 §4.2: "$M_x = 17$ and
$M_y = 33$ are chosen: for each truth bin two bins are used on detector
level"). Our ratio of 4 is at the upper end of the cited range, which is
fine — there is no obvious need to go finer. However, S12 §2.1 requires
that "the rows of $A$ shall be linear independent" and that $V_{yy}$
have full rank, both of which fail catastrophically when reco bins are
too fine and contain too few events. Our reco-side data statistics (in
total ~300k events for high-Q²) divided by 960 reco bins gives ~310
events per reco bin on average, but with a heavy tail in z and $p_T^h$,
the worst reco bins likely have <10 events. **Accept the verification**:
compute the per-reco-bin event count distribution; if more than ~10% of
reco bins have <10 events, the matrix rows are at risk of being linearly
dependent and we should either merge those reco bins (still inside
TUnfoldBinning) or accept that our reco-to-gen ratio of 4 is the right
target. Do NOT increase to 8 or 16 without first verifying that
adjacent reco bins are populated.

---

## 3. Recommended top-3 optimization actions for rev4

These three actions are the highest-leverage and are *mutually
reinforcing* — they all become easier if all three are done together,
because they share the `TUnfoldBinning` carrier object.

### Action 1: Rebuild the binning as a `TUnfoldBinning` tree (item (a))

**Code changes (sketch — DO NOT IMPLEMENT IN THIS RESEARCH):**
- In the rev4 builder script, create a `TUnfoldBinning` for generator:
  `genBins = new TUnfoldBinning("gen"); auto* sig = genBins->AddBinning("signal"); sig->AddAxis("Q2", N_Q2, Q2_edges, false, false); sig->AddAxis("x", N_x, x_edges, false, false); sig->AddAxis("z", N_z, z_edges, false, false); sig->AddAxis("pTh", N_pT, pT_edges, false, false);`
- Create a parallel `TUnfoldBinning recoBins` with twice as many z and
  $p_T^h$ bin edges per axis (the existing 4× ratio).
- Re-fill the response matrix via
  `TUnfoldBinning::CreateHistogramOfMigrations(genBins, recoBins, "M")`.
- Pass to the `TUnfoldDensity` constructor:
  `TUnfoldDensity unfold(M, kHistMapOutputVert, kRegModeCurvature,
   kEConstraintArea, kDensityModeBinWidth, genBins, recoBins,
   "signal", "*[UOB]");`
- Remove the 304 manual `RegularizeCurvature(...)` calls. The binning
  tree handles them.

**Expected benefit:** Eliminates the boundary-leakage risk (item (g))
*and* enables item (f) in one stroke. ρ → 1.0 between adjacent
published bins should drop to typical 0.3–0.5.

**Risk:** First time we'll use `TUnfoldBinning` in this analysis;
requires a closure test against the rev3 builder before being trusted.

### Action 2: Switch density mode to `kDensityModeBinWidth` (item (f), bundled with Action 1)

**Code changes:** None separate from Action 1 — the constructor flag
just changes from `kDensityModeNone` to `kDensityModeBinWidth` (or to
the constructor default `kDensityModeBinWidthAndUser` if we want to
keep the option of a user function for $p_T^h$ later (P2)).

**Expected benefit:** Per S12 §7.2.1–7.2.3, the regulariser will no
longer over-smooth the narrow low-z, low-$p_T$ bins. The 657% max
self-closure outliers in those bins should drop to be commensurate
with the median ~5%.

**Risk:** Effective τ will rescale (because the units of $L^T L$
change). The recommended τ from any prior scan is invalidated; must
re-scan from scratch.

### Action 3: Register the alternative MC response as `AddSysError` instead of running two separate unfoldings (item (h))

**Code changes (sketch):**
- Choose rapgap as nominal $A$.
- After `SetInput(...)`, call `unfold.AddSysError(M_djangoh,
  "GeneratorModel", kHistMapOutputVert, kSysErrModeMatrix)`.
- Run a single unfolding `unfold.DoUnfold(tau)`.
- Retrieve the cross-MC covariance via
  `unfold.GetEmatrixSysSource(matsys, "GeneratorModel")`.
- Add `matsys` to the published total covariance.
- Drop the second (djangoh-nominal) unfolding from the publication
  pipeline; keep it only as a sanity cross-check.

**Expected benefit:** The "147× outlier" bins disappear (they were
artefacts of differencing two regularised solutions). The per-bin
cross-MC contribution becomes proportional to $\|\delta M\| / \|M\|$
through the analytic propagator (S12 eq. 42), typically 5–15% rather
than 30–50%.

**Risk:** None methodological. Implementation risk only.

### Coupling note

If Actions 1+2 are taken and the L-matrix is now well-behaved, then
`ScanLcurve()` (item (i)) should produce a real L-shaped curve. The
current rev3 manual scan exists because no L-shape forms — it forms
because the L matrix is mis-specified. **Once Action 1+2 are done, item
(i) (replacing the manual scan with `ScanLcurve()`) becomes Action 4
and a natural follow-on.**

---

## 4. Sanity-check recommendations (additions to the diagnostic suite)

Schmitt's papers and the ROOT-DOC make several diagnostic
recommendations our current rev3 critic pipeline does not implement.

### (S1) ScanLcurve must converge to a real L-shape with τ_min not on the boundary

**Anchor:** ROOT-DOC: "The L-curve should have approximate L-shape and
the final choice of tau should not be at the very edge of the scanned
region".

Implement: a gate that requires the L-curve cubic-spline curvature
$C(t)$ (S12 eq. 24) to have an unambiguous interior maximum, and that
the chosen $\tau^*$ satisfy $\tau_{\min} < \tau^* < \tau_{\max}$ with
margin (e.g., $\tau^*/\tau_{\min} > 10$ and $\tau_{\max}/\tau^* > 10$).
Our rev3 already fails this gate (recommended τ = 1.83e-6 is the
smallest non-rejected of 20 points spanning [1e-7, 10]).

### (S2) ScanTau must sit in a real minimum, not on the boundary

**Anchor:** ROOT-DOC, same paragraph as (S1).

Implement: gate that `ScanTau(..., kEScanTauRhoAvg)` returns a
$\bar{\rho}_{\rm avg}(\tau)$ curve with a clean interior minimum, and
that the minimum is at least 10% below both endpoints of the scan.

### (S3) χ² test on refolding (the S17 §3.1 "Data tests")

**Anchor:** S17 §3.1 eq. 7: $\chi^2_A = (A\hat{x} + b - y)^T V_{yy}^{-1}
(A\hat{x} + b - y)$ should be distributed with $M_y - M_x$ degrees of
freedom. With $M_y = 960$, $M_x = 240$, we expect $\chi^2_A / N_{\rm df}
\approx 1$ with $N_{\rm df} = 720$. Our rev3 reports χ²/ndf rising to
100–300 at moderate τ — that is a *direct* signal of severe
over-regularisation per S17. The acceptance gate should require
$\chi^2_A / 720 \in [0.5, 2.0]$ (one-sided p-value > 0.001) at the
chosen τ.

### (S4) Normalisation closure $Y_{\rm unf} = Y_{\rm data}$

**Anchor:** S17 §3.1 eq. 6.

With `kEConstraintArea` this should be satisfied to machine precision
at every τ. Our rev3 reports the unfolded total dropping by ×5 between
small and moderate τ — this should be impossible if the area
constraint is being honoured. Implement a hard gate: if
$|Y_{\rm unf} - Y_{\rm data}| / Y_{\rm data} > 10^{-3}$ at any τ,
abort with a clear "area constraint not honoured — check
$\dim x \le \dim y$ and rank of $A$".

### (S5) Condition number of $A^T V_{yy}^{-1} A + \tau^2 L^T L$ at the chosen τ

**Anchor:** S12 §2.1: "$V_{yy}$ has to have full rank and the rows of
$A$ shall be linear independent". Our rev3 reports
$\kappa(V_{xx}) \sim 10^{15}$–$10^{21}$ — this means the matrix being
inverted in $E = (A^T V_{yy}^{-1} A + \tau^2 L^T L)^{-1}$ (S12 eq. 14)
is numerically singular at the level of double precision (which has
~16 decimal digits). Implement a gate $\kappa(E^{-1}) < 10^{12}$. If
violated at all τ, the L matrix is the wrong shape; investigate before
trusting any output.

### (S6) Closure test on independent MC samples (S17 §3.2)

**Anchor:** S17 §3.2 eqs. 9, 10.

Generate a few hundred Poisson-fluctuated pseudo-experiments from the
*data* truth (best estimate); unfold each; verify $\langle \hat{x}
\rangle = x_{\rm truth}$ and $\langle V_{xx} \rangle$ agrees with the
TUnfold-returned covariance. S17 §3.2 calls this "the most interesting
type of tests": pseudo-experiments seeded from a *different* truth
distribution than the one used to build $A$ (i.e., djangoh PEs unfolded
with rapgap $A$). $\chi^2_{\rm truth}$ (eq. 10) should follow a χ²
distribution with $M_x$ d.o.f.; if it has a fat right tail, the
algorithm has bias under model variation — which is what we're seeing
at the bin level. This is the strongest test that the rev4 pipeline
should pass before publication.

### (S7) Per-source covariance decomposition

**Anchor:** S12 §6 ("The contribution of individual sources of
uncertainty to the result's covariance matrix may be studied after
unfolding") and ROOT-DOC `GetEmatrixSysSource(...)`,
`GetEmatrixSysUncorr(...)`, `GetEmatrixInput(...)`,
`GetEmatrixSysTau(...)`.

Once Action 3 is taken, the cross-MC contribution becomes a single
`GetEmatrixSysSource(..., "GeneratorModel")` block. Add a published
plot showing the diagonal contributions: input statistics, response
matrix statistics ($V_{xx}^{M,\rm stat}$, S12 eq. 43), generator model
shift, τ uncertainty (`GetEmatrixSysTau`). This is the standard H1
breakdown and is fully supported by TUnfold.

### (S8) Eigenvalue spectrum of $L^T L$ within the published binning

**Anchor:** S12 §4 (cites Blobel eigenvalue analysis [14]); algorithm
folklore: a regularisation matrix that has too many near-zero
eigenvalues will not regularise (the τ in the null space does
nothing), and one with too many large eigenvalues will over-regularise
specific modes. A simple diagnostic: compute the eigenvalues of $L^T L$
restricted to the published bins; the spectrum should be monotone and
gap-free. If there are isolated large eigenvalues, those are the modes
ρ → 1.0 is collapsing onto.

---

## 5. References

```bibtex
@article{Schmitt:2012kp,
  author        = "Schmitt, Stefan",
  title         = "{TUnfold, an algorithm for correcting migration effects
                   in high energy physics}",
  journal       = "JINST",
  volume        = "7",
  pages         = "T10003",
  year          = "2012",
  doi           = "10.1088/1748-0221/7/10/T10003",
  eprint        = "1205.6201",
  archivePrefix = "arXiv",
  primaryClass  = "physics.data-an",
  reportNumber  = "DESY-12-129",
  note          = "v4, 15 Nov 2012; reference algorithm for the H1
                   high-Q$^2$ SIDIS rev3 pipeline."
}

@article{Schmitt:2016orm,
  author        = "Schmitt, Stefan",
  title         = "{Data Unfolding Methods in High Energy Physics}",
  journal       = "EPJ Web Conf.",
  volume        = "137",
  pages         = "11008",
  year          = "2017",
  doi           = "10.1051/epjconf/201713711008",
  eprint        = "1611.01927",
  archivePrefix = "arXiv",
  primaryClass  = "physics.data-an",
  note          = "Proceedings of the XII Quark Confinement and the Hadron
                   Spectrum conference. Toy comparison of BBB, matrix
                   inversion, template fit, Tikhonov, EM, and IDS
                   unfolding; gives operating recommendations for
                   L-curve and global-correlation scans."
}

@misc{ROOT:TUnfoldDensity,
  author        = "{ROOT Collaboration}",
  title         = "{TUnfoldDensity class reference (master)}",
  howpublished  = "\url{https://root.cern/doc/master/classTUnfoldDensity.html}",
  year          = "2026",
  note          = "Operating recommendations, constructor defaults, and
                   ScanLcurve / ScanTau usage notes. The default
                   constructor is
                   TUnfoldDensity(hA, histmap, kRegModeCurvature,
                   kEConstraintArea, kDensityModeBinWidthAndUser,
                   outputBins, inputBins, regDist,
                   regAxisSteering=\"*[UOB]\")."
}

@article{Tikhonov:1963,
  author        = "Tikhonov, A. N.",
  title         = "{Solution of incorrectly formulated problems and the
                   regularization method}",
  journal       = "Soviet Math. Dokl.",
  volume        = "4",
  pages         = "1035",
  year          = "1963",
  note          = "Original Tikhonov regularisation reference,
                   underlying TUnfold's L_2 term."
}

@article{Hansen:Lcurve,
  author        = "Hansen, P. C.",
  title         = "{The L-curve and Its Use in the Numerical Treatment of
                   Inverse Problems}",
  booktitle     = "Computational Inverse Problems in Electrocardiology",
  editor        = "P. Johnston",
  year          = "2000",
  note          = "Cited by S12 §4.1 [ref. 16] and S17 §4.4 [ref. 6] as
                   the source of the L-curve method that
                   ScanLcurve() implements."
}

@article{Blobel:DataUnfolding,
  author        = "Blobel, V.",
  title         = "{Data unfolding}",
  howpublished  = "Talk, Terascale Statistics School, Hamburg",
  year          = "2010",
  url           = "http://www.desy.de/~blobel/",
  note          = "Cited by S12 §4 and S17 §4.4 as the source of the
                   global-correlation minimisation method that ScanTau()
                   implements."
}

@article{Cowan:1998ji,
  author        = "Cowan, Glen",
  title         = "{Statistical Data Analysis}",
  publisher     = "Oxford University Press",
  year          = "1998",
  isbn          = "0198501560",
  note          = "Cited by S12 §2.2 as the discussion of the
                   Poisson vs. Gaussian bias that motivates
                   kEConstraintArea."
}

@article{H1:1999pj,
  author        = "Adloff, C. and others (H1 Collaboration)",
  title         = "{Measurement of neutral and charged current cross
                   sections in positron proton collisions at large
                   momentum transfer}",
  journal       = "Eur. Phys. J. C",
  volume        = "13",
  pages         = "609",
  year          = "2000",
  eprint        = "hep-ex/9908059",
  archivePrefix = "arXiv",
  note          = "Cited in S12 §7.2.1 footnote as the DIS reduced
                   cross section example for f_j^user usage."
}

@article{H1:2009pze,
  author        = "Aaron, F. D. and others (H1 Collaboration)",
  title         = "{Measurement of the Inclusive ep Scattering Cross
                   Section at Low Q$^2$ and x at HERA}",
  journal       = "Eur. Phys. J. C",
  volume        = "66",
  pages         = "17",
  year          = "2010",
  eprint        = "0910.5631",
  archivePrefix = "arXiv",
  note          = "Cited in S12 §5 as the discriminator-based background
                   determination example. Relevant if we ever move
                   background subtraction from upstream to inside
                   TUnfold."
}
```

---

## Appendix: Quick lookup table mapping our rev3 symptoms to Schmitt sections

| Symptom in rev3 | Most likely cause per Schmitt | Section(s) |
|---|---|---|
| max\|ρ\| > 0.95 at every τ | Curvature regulariser with non-density-aware L on non-uniform bins; possibly boundary triplets that cross cell edges | S12 §7.1, §7.2.1–7.2.3 |
| Unfolded total drops ×5 between small and moderate τ | Either area constraint not actually honoured, or large miss-row contribution in the response matrix shifting the τ-dependent normalisation through eq. 15 | S12 §2.2 eqs. 6–8, 15 |
| χ²_refold / ndf rising to 100–300 | Severe over-regularisation; the bias term has overtaken the data term | S17 §3.1 eq. 7; S17 Tab. 1 |
| 30–50% bin-by-bin cross-MC spread | Two independent regularised unfoldings hitting different singular solutions, instead of an analytic δx from `AddSysError` | S12 §6, §B eq. 42 |
| z=0..0.1 worse than z>0.1 | `kDensityModeNone` over-smooths narrow bins; curvature triplets near the (Q²,x)-cell boundary at z=0 are likely poorly defined | S12 §7.2.1 eqs. 29–30 |
| Recommended τ is smallest non-rejected | Manual scan is selecting the boundary point; ScanLcurve / ScanTau gates from ROOT-DOC explicitly forbid this | ROOT-DOC "How to choose the regularisation settings" |
| κ(V_xx) ~ 10^15–10^21 | Rows of A approximately linearly dependent (S12 §2.1 prerequisite violated) or L_T L kernel coupling near-singular modes through a wrong boundary | S12 §2.1 |

---

*End of artifact. Total length: ~570 lines.*
