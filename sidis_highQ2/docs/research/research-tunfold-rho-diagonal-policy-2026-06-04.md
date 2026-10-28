# TUnfold ρ / global correlation: definitions and diagonal-inclusion policy

**Scope.** Clarify, for the H1 high-Q² charged-SIDIS analysis, the exact
definitions of (a) the ordinary correlation matrix ρ_ij returned by
`TUnfold::GetRhoIJ` and (b) the global correlation coefficient ρ_i returned
by `TUnfold::GetRhoI`; state the diagonal-inclusion policy for our
"max / mean off-diagonal |ρ|" diagnostics; audit our current
adaptive-vs-nominal rho comparison plots; and recommend the next concrete
check.

**Sources consulted**
- ROOT TUnfold reference, `https://root.cern.ch/doc/master/classTUnfold.html`
  (TUnfold/TUnfoldDensity is the same code we run against H1OO; the V17.9
  Schmitt code is the upstream of ROOT's TUnfold). Saved local extract at
  `/Users/hklest/.local/share/opencode/tool-output/tool_e940e1e71001x0EUWK0lXhSimm`.
- S. Schmitt, JINST 7 (2012) T10003 [arXiv:1205.6201] (the canonical TUnfold
  paper) — already summarised in
  `H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`.
- Our existing code:
  - `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`
  - `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/postprocess_single_tau.py`
  - `H1/sidis_highQ2/scripts/unfolding/postprocess_tunfold_official.py`
  - `H1/sidis_highQ2/scripts/unfolding/unfold_TUnfold_4d.py`
  - `H1/sidis_highQ2/scripts/unfolding/plot_unfold4d_QA.py`
  - `H1/sidis_highQ2/scripts/plotting/plot_adaptive_vs_nominal_unfolding_qa_2026_06_04.py`
  - `H1/sidis_highQ2/scripts/analysis/select_S3_tau_multiobjective.py`

---

## 1. Two distinct ρ objects in TUnfold

TUnfold exposes two genuinely different correlation-like objects. Conflating
them is the most common diagnostic error.

### 1.1 Ordinary (Pearson) correlation matrix ρ_ij — `GetRhoIJ`

`void TUnfold::GetRhoIJ(TH2* rhoij, const Int_t* binMap=nullptr)` (declared in
`TUnfold.h`, defined in `TUnfold.cxx`; see ROOT reference, "Member Function
Documentation → GetRhoIJ"). It returns the standard normalised covariance

    ρ_ij = V_ij / sqrt(V_ii · V_jj)

built from the output covariance V = `GetVxx()`. This is just the ordinary
Pearson correlation matrix of the unfolded vector x.

**Diagonal behaviour.** By construction ρ_ii ≡ 1 whenever V_ii > 0. The
diagonal carries *no information*: it is the identity statement
"each bin is perfectly correlated with itself". Any health metric whose
purpose is to detect bin-to-bin coupling MUST exclude the diagonal. Including
it forces the metric to be bounded below by 1, which (a) is independent of
the unfolding, and (b) makes every well-behaved result look pathological.

Off-diagonal range: ρ_ij ∈ [−1, +1], with negative values typical of
area-constrained TUnfold (anti-correlation between neighbours that share a
constrained sum) and positive values typical inside a regularisation
neighbourhood from `kRegModeCurvature`.

### 1.2 Global correlation coefficient ρ_i — `GetRhoI`

`Double_t TUnfold::GetRhoI(TH1* rhoi, const Int_t* binMap=nullptr, TH2*
invEmat=nullptr)`. ROOT reference quotes verbatim:

> for a given bin, the global correlation coefficient is defined as
>
>     ρ_i = sqrt( 1 − 1 / ( V_ii · (V⁻¹)_ii ) )
>
> such that the calculation of global correlation coefficients possibly
> involves the inversion of a covariance matrix.
> return value: maximum global correlation coefficient

This is the **Eadie-Drijard-James-Roos-Sadoulet "global correlation
coefficient"** (PDG-style). It is the multiple-correlation of bin i with
**all other bins jointly**, expressed as one scalar per bin, derived from
the full V *and* its inverse. Key properties:

1. ρ_i ∈ [0, 1) by construction. It is **not** a Pearson correlation; it
   measures how much of the variance of x_i is explained by a linear
   combination of all other x_{j≠i}.
2. ρ_i = 0 iff x_i is uncorrelated with every other bin (V and V⁻¹ are both
   diagonal in row i).
3. ρ_i → 1 iff the covariance matrix becomes degenerate at bin i (some
   linear combination of x_j explains x_i perfectly — the symptom of
   over-regularisation / rank deficiency / collinear bins).
4. **There is no diagonal-inclusion question for ρ_i.** It is already a
   per-bin scalar that quantifies "bin i vs. the rest". The companion
   summary scalars exposed by TUnfold are:
   - `GetRhoMax()` — max_i ρ_i, returned also from `DoUnfold()` and stored
     internally as `fRhoMax`.
   - `GetRhoAvg()` — mean_i ρ_i, stored as `fRhoAvg`.
   These are the figures of merit minimised by `ScanTau` mode
   `kEScanTauRhoAvg` / `kEScanTauRhoMax` (the Schmitt-paper §4.2
   convention; see
   `research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
   line 68).

### 1.3 One-line cheat sheet

| Object | Source | Range | Diagonal? | Diagnostic role |
|--------|--------|-------|-----------|-----------------|
| ρ_ij Pearson | `GetRhoIJ` / our `h_rho_pub` | [−1, +1] | ≡ 1 (uninformative) | **off-diagonal** entries are the diagnostic; diagonal MUST be excluded from max/mean |
| ρ_i global | `GetRhoI` / `GetRhoMax` / `GetRhoAvg` | [0, 1) | n/a — already per-bin scalar | per-bin scalar; one value per bin, used directly |

---

## 2. Audit of current H1 high-Q² SIDIS rho code

I checked every call site that touches ρ in this analysis.

### 2.1 Canonical per-tau macro — CORRECT

`scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`,
lines 621–666. Pseudocode:

```cpp
// build full 240x240 rho with rho[i][i]=1 for storage
for (r1=0; r1<NB_PUB; ++r1) rho_pub[r1*NB_PUB + r1] = 1.0;
for (r1=0; r1<NB_PUB; ++r1)
  for (r2=r1+1; r2<NB_PUB; ++r2)
    rho_pub[r1*NB_PUB + r2] = cov(r1,r2)/(σ_r1*σ_r2);

// metrics computed STRICTLY on upper triangle (r2 = r1+1, …)
for (r1=0; r1<NB_PUB; ++r1)
  for (r2=r1+1; r2<NB_PUB; ++r2)            //  <-- diagonal excluded
    accumulate ar = |rho_pub[r1][r2]|
```

`max_abs_rho_pub_offdiag`, `mean_abs_rho_pub_offdiag`,
`frac_abs_rho_pub_gt_0p5`, `frac_abs_rho_pub_gt_0p9` are all written with
the diagonal excluded. **OK**.

### 2.2 Postprocess plot — CORRECT

`scripts/unfolding/tunfold_official_v3_percharge/postprocess_single_tau.py`,
lines 113–151. Uses `iu = np.triu_indices(rho.shape[0], k=1)` for the
off-diagonal histogram (`k=1` strictly excludes the diagonal). The
displayed `mean / max / frac>0.5 / frac>0.9` values are read from the macro
JSON, which is already off-diagonal-only (§2.1). **OK**.

### 2.3 4-D TUnfold driver — CORRECT

`scripts/unfolding/unfold_TUnfold_4d.py`, lines 145–155:

```python
Roff = Rho.copy()
np.fill_diagonal(Roff, 0.0)
max_off_rho = float(np.max(np.abs(Roff)))
```

Diagonal explicitly zeroed before the max. **OK**. Note this style is
safer than `triu_indices` if any future caller forgets which slice they
took.

### 2.4 Reduced/aggregated official postprocess — CORRECT

`scripts/unfolding/postprocess_tunfold_official.py`, lines 104–109: builds
rho from cov, then `np.fill_diagonal(rho, 1.0)`. Diagonal is forced to the
trivial value (so plotting and saving show 1 on the diagonal), but no max
is taken from this object inside this file. **OK**.

### 2.5 Adaptive-vs-nominal QA plotter — CORRECT (uses upstream metrics)

`scripts/plotting/plot_adaptive_vs_nominal_unfolding_qa_2026_06_04.py`,
lines 251 and 625–648: reads `mean_abs_rho_pub_offdiag` /
`max_abs_rho_pub_offdiag` from the per-tau JSONs produced in §2.1. It does
not recompute ρ from a matrix, so it inherits the (correct) off-diagonal
convention. **OK** — provided the per-tau JSONs were produced by the
current canonical macro (see §3).

### 2.6 4-D plot QA — CORRECT

`scripts/unfolding/plot_unfold4d_QA.py`, lines 230–256: the cross-stratum
histogram `cross_rhos` is built explicitly from `Rho[i_a, i_b]` with
`i_a ≠ i_b` by construction (different stratum indices), and the summary
line `tunfold_json["max_off_diag_correlation"]` is the upstream value
produced in §2.3. **OK**.

### 2.7 S3 τ multi-objective selector — CORRECT

`scripts/analysis/select_S3_tau_multiobjective.py` reads
`max_abs_rho_pub_offdiag` / `max_abs_rho_pub` from per-tau JSONs (lines
240–241). Same provenance as §2.5. **OK**.

### 2.8 Bottom line

No current production-path code computes a max / mean over a correlation
matrix that includes the diagonal. All off-diagonal scalars consumed by
the adaptive-vs-nominal plot trace back to the macro convention in §2.1,
which excludes the diagonal correctly.

**Therefore the user's concern — "max=1 because diagonal was included" —
is not realised in the present code path.** Any saturation of
`max_abs_rho_pub_offdiag` near 1.0 in the JSONs (e.g. the
0.97 / 0.98 / 0.998 values in
`data_release_rev6_TUnfold_AREA_210bin_INTERNAL_PRELIMINARY/release.json`
or in `tau_scan_summary_data_pos.json` cited in
`docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md`) is a *real*
off-diagonal saturation, not a diagonal-inclusion artefact.

The physical mechanism is documented in
`docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md` §lines 31–32:
because the publication-axis cov is the aggregation of a 4× over-determined
gen layout (`N_GEN_PER_RECO = 4`), the 4 gen bins inside one reco parent
are co-determined; their aggregated row collapses onto a single common
mode, so off-diagonal ρ between pT-adjacent reco bins inside a (cell, z)
group saturates at ~1. That is a **real** regularisation pathology of the
chosen binning, not a bug in the diagnostic.

---

## 3. Policy for H1 high-Q² SIDIS rho diagnostics

Adopt the following explicit policy. None of it requires code changes
beyond §5; this section is the canonical reference for downstream agents.

### 3.1 Ordinary correlation matrix ρ_ij (from `GetRhoIJ` or built locally)

| Metric | Definition |
|--------|------------|
| `max_abs_rho_offdiag` | max over {(i,j) : i < j} of \|ρ_ij\| — **diagonal excluded** |
| `mean_abs_rho_offdiag` | mean over {(i,j) : i < j} of \|ρ_ij\| |
| `frac_abs_rho_gt_t` | fraction of {(i,j) : i < j} pairs with \|ρ_ij\| > t |
| `nearest_neighbour_rho` | ρ_ij restricted to physical Δ=1 in one axis (z, pT, x, or Q²), signed. Already implemented in `postprocess_single_tau.py` `_enumerate_nbr_far`. |
| `far_field_rho` | \|ρ_ij\| restricted to L1 distance ≥ 4 across all 4 axes |

The diagonal is set to 1.0 only **for storage and visualisation** (a
matrix plot is unreadable with zeros on the diagonal). It is **never**
included in any reduction (max / mean / fraction / percentile).

### 3.2 Global correlation ρ_i (from `GetRhoI`, `GetRhoMax`, `GetRhoAvg`)

| Metric | Definition |
|--------|------------|
| `rho_i_global[i]` | TUnfold global correlation coefficient per bin i, ρ_i = sqrt(1 − 1/(V_ii (V⁻¹)_ii)) |
| `rho_max_global` | `GetRhoMax()` = max_i ρ_i |
| `rho_avg_global` | `GetRhoAvg()` = mean_i ρ_i |

ρ_i is **not** a Pearson off-diagonal entry. There is no "diagonal vs
off-diagonal" distinction. Each ρ_i is a single number for bin i. Do not
form a max over a "diagonal vs off-diagonal" partition; just report
`rho_max_global` and `rho_avg_global` directly.

The two object classes (Pearson off-diagonal vs global) carry different
information and should be reported **side by side**, never substituted
for each other:

- Pearson off-diagonal max/mean → "how strongly do specific neighbour
  pairs (or any pair) lock together?"
- Global ρ_i max/mean → "is any individual bin de-facto a linear
  combination of the others (rank-deficiency / over-regularisation
  warning)?"

For τ scans, the Schmitt-paper minimisation is on `rho_avg_global` (the
`ScanTau`-rhoavg mode used by the C++ port; see
`research-tunfold-cxx-port-state-2026-05-30.md` lines 59 and 902).

### 3.3 Adaptive-vs-nominal binning comparison: required tuple

When comparing adaptive low-pT vs nominal 4× binning at the τ of each
candidate, the comparison must report:

1. `max_abs_rho_offdiag` and `mean_abs_rho_offdiag` (per §3.1).
2. `frac_abs_rho_gt_0p5`, `frac_abs_rho_gt_0p9` (per §3.1).
3. `nearest_neighbour_rho` distributions per Δ axis (signed, so positive
   regularisation-induced and negative area-constraint-induced lobes can
   be told apart).
4. `rho_max_global`, `rho_avg_global` (per §3.2), if available.
5. Eigenvalue spectrum + condition number of V (already computed in §2.1).

(1)–(3) are already reported by the current pipeline. (4) is **not yet
reported in the adaptive-vs-nominal product**; see §5.

---

## 4. Where a bug *could* have existed (not in current code)

For completeness, document the failure modes we are NOT exposed to but
that adjacent code patterns invite. Use this checklist when reviewing
any new ρ diagnostic.

**Bug pattern A — full-matrix `np.max(np.abs(rho))`.** If anyone writes
`max_rho = float(np.abs(rho).max())` on a TH2D loaded as a NumPy array,
the result is exactly 1.0 because the diagonal is 1. **Cure:** use
`np.triu_indices(n, k=1)` or `np.fill_diagonal(rho.copy(), 0.0)` before
the max. Grep all new code for the pattern:

    rg -n 'np\.max\(np\.abs\(rho' H1/sidis_highQ2/scripts
    rg -n 'np\.abs\(rho\)\.max'    H1/sidis_highQ2/scripts
    rg -n 'rho\.max\(\)'           H1/sidis_highQ2/scripts

Current scan: zero hits (verified during this audit).

**Bug pattern B — `np.fill_diagonal(rho, 1.0)` followed by max.** The
trivial-1 fill is a fine visualisation step but **must** be followed by
either a triu-only reduction or a `np.fill_diagonal(rho.copy(), 0.0)`
prior to max/mean. `postprocess_tunfold_official.py:109` does the fill;
no subsequent reduction is taken from that matrix in that file. **Safe
today; flag if reused.**

**Bug pattern C — ρ_i mistaken for an off-diagonal slice.** If the
plotting code labels `rho_max_global` as "max off-diagonal", the reader
gets confused. We currently do not store ρ_i at all (no `GetRhoI` call in
our macros — see §5), so this confusion has not arisen, but flag for any
future change.

**Bug pattern D — using ρ_ij averaged over all (i,j) including diagonal
as a closure-quality metric.** With N bins, that mean is artificially
raised by N/N² = 1/N. For N=240 this is 0.4%, small but non-zero. Not a
current issue (we use `i<j` upper triangle).

---

## 5. Recommended next action for adaptive-vs-nominal ρ plots

Two concrete deliverables in priority order. Both are small additions to
existing files; neither needs new infrastructure.

### 5.1 (Priority 1, ≤ 1 day) Add the TUnfold global ρ to the per-tau output

Modify `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`
to call

```cpp
TH1D h_rho_i_global("h_rho_i_global", ";gen bin;rho_i (global)",
                    NB_G + 1, -0.5, NB_G + 0.5);
double rho_max_global = unfold.GetRhoI(&h_rho_i_global);
double rho_avg_global = unfold.GetRhoAvg();
double rho_max_root    = unfold.GetRhoMax();
```

right after the existing cov/ρ_ij computation. Write three scalars to
the JSON:

```
"rho_max_global":  rho_max_root,
"rho_avg_global":  rho_avg_global,
"rho_max_global_from_GetRhoI": rho_max_global,
```

and write `h_rho_i_global` (241-bin TH1D; the gen-axis underflow stays in
bin 0 as for the other histograms) to the per-tau ROOT for the postprocess
plotter to consume. Cost: O(20 lines of C). The matrix inversion inside
`GetRhoI` is a one-shot, negligible compared with the existing
eigendecomposition.

Add a panel to `postprocess_single_tau.py` that plots the per-bin ρ_i
distribution (histogram of `h_rho_i_global` values) alongside the existing
off-diagonal ρ_ij histogram. This gives the two complementary views
required by §3.2.

### 5.2 (Priority 2, ≤ 1 day) Promote ρ_max_global + ρ_avg_global to the comparison plot

Update `scripts/plotting/plot_adaptive_vs_nominal_unfolding_qa_2026_06_04.py`
to read the new scalars (§5.1) and overlay them on the rho-vs-τ panel
that currently shows only `mean_abs_rho_pub_offdiag` and
`max_abs_rho_pub_offdiag`. Recommended layout:

- Solid lines: existing Pearson off-diagonal mean / max.
- Dashed lines (different colour family, e.g. Okabe-Ito vermillion vs
  blue): `rho_avg_global` and `rho_max_global`.
- Y-axis label: "ρ summary (off-diagonal Pearson vs. global, per τ)".
- Add a one-line caption explaining the two families differ (Pearson:
  pair-level; global: per-bin scalar from §3.2).

If §5.1 is not landed yet, the plot should still render and just show
the existing two curves with a note "ρ_global not yet stored" — never
fabricate values.

### 5.3 (Priority 3, sanity) Spot-check existing JSONs

For each per-tau JSON consumed by the adaptive-vs-nominal plot, verify
that the reported max is **not** exactly 1.0 across many τ values (which
would be the diagonal-inclusion smoking gun). The values cited in
`docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md` line 32
(`mean=0.16, max=0.998`) are evidence of a real saturation pattern, not
a bug — the mean is far from 1, and the max varies with τ. The same
spot check should be applied to the adaptive low-pT and refined low-pT
summary JSONs once they are produced.

A useful one-liner (run on NAF or locally on retrieved JSONs) is:

    jq '.max_abs_rho_pub_offdiag, .mean_abs_rho_pub_offdiag' \
        path/to/unfold_tau*.json

Both should vary smoothly with τ; if `max` is bit-exactly 1.0 across
many τ, look for a regression that includes the diagonal. (No such
regression is present today.)

---

## 6. Summary for the user's exact question

> Should diagonal components be included when testing unfolding
> correctness? Each bin is 100% correlated with itself; including the
> diagonal in max would always give 1.

**For the ordinary correlation matrix ρ_ij (our `h_rho_pub`,
`GetRhoIJ`): NO.** The diagonal is identically 1 by construction and
carries no information about the unfolding. All off-diagonal scalars
(max, mean, fraction above threshold, percentile, nearest-neighbour
histograms) must exclude i = j. Our current pipeline (§2) does this
correctly everywhere.

**For TUnfold's global correlation coefficient ρ_i (`GetRhoI`,
`GetRhoMax`, `GetRhoAvg`): the question does not arise.** ρ_i is a
per-bin scalar bounded in [0, 1) defined by
ρ_i = sqrt(1 − 1/(V_ii (V⁻¹)_ii)). It already aggregates "bin i vs.
all others" into one number; there is nothing to exclude. Report it
as `rho_max_global` and `rho_avg_global` directly, side by side with
(not in place of) the Pearson off-diagonal summary.

**The two are different objects and should both appear in the
adaptive-vs-nominal comparison.** Concrete next action: §5.1 + §5.2.

---

## 7. Risks, open questions, gaps

- **Not yet computed.** TUnfold ρ_i (global) is not currently emitted by
  `unfold_tunfold_single_tau.C`. Until §5.1 is done, the adaptive-vs-
  nominal comparison has only the Pearson off-diagonal family. This is
  a coverage gap, not a correctness bug.
- **`GetRhoIJ` would deliver the *aggregated* matrix only if a `binMap`
  is supplied** (default `nullptr` returns the full unaggregated matrix
  in TUnfold's internal indexing). Our macro builds the publication-bin
  240×240 cov by manual aggregation (T·V·Tᵀ) and then derives ρ from it;
  the `binMap` argument of `GetRhoIJ` / `GetRhoI` would do the same job
  natively and is the canonical TUnfold-recommended path. Documented as
  an open opportunity, not a defect.
- **Schmitt-paper "ρ_avg has no minimum in τ" caveat** (see
  `research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
  lines 151, 535) means `rho_avg_global` is a *monitoring* curve, not a
  unique τ-selection target on our binning. Use it together with the
  L-curve and closure, never alone.
- **Aggregation aliasing.** Because our publication ρ is built from the
  T·V·Tᵀ aggregation of a 4× over-determined gen layout, even with the
  diagonal correctly excluded the off-diagonal max saturates near 1.0
  by *physical* mechanism (co-determined gen bins inside one reco
  parent). This is the real story behind the JSON values cited in
  critique-tunfold-v3-percharge-2026-05-31.md, and is the motivation for
  the binning-scan / refined-bin work already underway. Diagnostic
  policy here is orthogonal to that physical issue.

## 8. Artefacts referenced

- `H1/sidis_highQ2/docs/research/research-tunfold-rho-diagonal-policy-2026-06-04.md` (this file)
- `H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md` (Schmitt eq./§4.2 summary)
- `H1/sidis_highQ2/docs/research/research-tunfold-cxx-port-state-2026-05-30.md` (ScanTau-rhoavg lineage)
- `H1/sidis_highQ2/docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md` (saturation mechanism)
- ROOT reference: `https://root.cern.ch/doc/master/classTUnfold.html` (verbatim ρ_i formula in `GetRhoI` doc block)
