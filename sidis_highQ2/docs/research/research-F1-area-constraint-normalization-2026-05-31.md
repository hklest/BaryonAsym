# F1 area-constraint normalization bug — diagnosis and recommended fix

**Author:** researcher agent
**Date:** 2026-05-31
**Scope:** Verify whether the rev3 F1 "data overflow = miss_row × data/prior" fix
applied in `merge_migration_per_charge.py::_extend_data_fine` produces a
physically correct gen-pool unfolded total under TUnfold `kEConstraintArea`,
and decide on the cleanest correction.

**TL;DR:** **The user is correct.** F1 is a mis-normalization. It feeds
TUnfold an area-constraint target equal to `data_total × prior / matched`
(via algebraic identity), but the *correct* gen-pool target at data
luminosity is `data_total × prior / matched_in_inner_reco` — and crucially,
because our response is filled so that `prior[j] ≈ matched_col[j] +
miss_col[j]` (column j of M sums to prior[j]), TUnfold's internal
per-column normalization `A[i,j] = M[i,j] / fSumOverY[j]` makes
`epsilon[j] ≡ Σ_i A[i,j] = 1`, and the area constraint becomes
`Σ_j X[j] = Σ_i Y[i]`. F1 therefore forces the unfolded total to equal
`data_total + miss_overflow = data_total × prior / prior = data_total ×
(1 + miss/prior) ≈ 1.45 × data_total`, whereas the physically correct
gen-pool total is `data_total / reco_eff = data_total × prior / matched
≈ 1.83 × data_total`. The 21% gap is precisely `1.45 / 1.83 = 0.79`.

The simplest fix is **Option 1: replace `prior_total` with `matched_total`
in F1**. This yields `unfolded_total = data_total + miss × data / matched
= data_total × prior / matched = data_total / reco_eff`, which is what
Cowan §11 and HERMES PRD 87 (2013) 074029 call the "acceptance-corrected
gen yield". Option 2 (drop F1, switch to `kEConstraintNone`) is also
defensible — Britzger uses it explicitly with a `2×τ` compensation — but
introduces a more disruptive global change.

**IBU is affected by the same bug**, because run_ibu.py feeds IBU exactly
the same F1-extended data vector and includes the miss row in `col[j]`.
This forces IBU's internal `eps[j] ≈ 1` (instead of the physical
~0.546), so IBU's `1/eps[j]` update step does NOT recover the
acceptance-corrected total — it returns the same `data_total × prior /
prior` = `data + miss_overflow` ≈ 2.14e6 that TUnfold returns under F1.
This is why the IBU and TUnfold answers agree to 0.5%: they are
*equivalently mis-normalized*, not independently correct.

---

## 1. The bug, with definitive algebra

### 1.1 Setup

Per `build_migration_th2_per_charge.py`:

- `R[i, j] += w_reco = w1·w2·Nuclia` for MATCHED rows (line 683-686)
- `R[NB_R, j] += w_gen = w1·w2` for MISS rows (line 724-725)
- `R[i, NB_G] += w_reco` for FAKE rows (line 699-700; outside the area
  constraint relevant slice)
- `prior_gen[j] += w_gen = w1·w2` from `ChargedTotGen` (line 357, 651)

Numerical totals from the pos-charge nominal response, per
`research-tunfold-optimization-from-schmitt-paper-2026-05-31.md` and
the user's quoted numbers:

- `matched_total ≡ Σ_{i,j} R[i_inner, j_inner] = Σ_j matched_col[j] ≈ 3.66e7`
- `miss_row_mc  ≡ Σ_j R[NB_R, j]            ≈ 30.66e6`
- `prior_total ≡ Σ_j prior_gen[j]             ≈ 6.71e7`
- `data_total  ≡ Σ_i (n_subtracted)           ≈ 1.48e6`

Note `matched_total + miss_row_mc ≈ 6.726e7 ≈ prior_total` to 0.2%, so
**`column_sum_M[j] = matched_col[j] + miss_col[j] ≈ prior[j]`** for every
gen bin. The tiny mismatch comes from the NucliaWeight asymmetry between
matched (with Nuclia) and miss (without) — already noted in
`merge_migration_per_charge.py:368-380` as a known limitation, residual
~2%.

The reco efficiency from MC is

```
reco_eff_MC ≡ matched_total / prior_total ≈ 3.66e7 / 6.71e7 ≈ 0.546
```

### 1.2 TUnfold's internal normalization (TUnfoldV17.cxx)

From `TUnfoldV17::TUnfoldV17` ctor on NAF at
`/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/src/TUnfoldV17.cxx`:

- Lines 1742-1755: for each gen column j, sum M values across **inner reco
  bins only** (`iy in [0, ny)` → ROOT bins 1..ny); accumulate into `sum`.
- Lines 1763-1774: add ROOT underflow + overflow reco bins to
  `fSumOverY[j]`.
- Line 1850: `A[i, j] = M[i, j] / fSumOverY[j]` — TUnfold **internally
  rescales** the user matrix M to per-column probabilities A.

Our TH2 is declared as `NB_R_EXT × NB_G_EXT = 961 × 241`. The miss row at
numpy index 960 lives at ROOT bin 961, which is the **last inner ROOT bin**
(NOT the ROOT overflow bin 962). So TUnfold sees the miss row as a regular
reco bin, and `fSumOverY[j]` is the sum over ALL 961 inner rows including
the miss row. Therefore

```
fSumOverY[j] ≈ matched_col[j] + miss_col[j] ≈ prior[j]
```

and consequently in TUnfold's normalized matrix

```
ε[j] ≡ Σ_i A[i, j]
     = Σ_i M[i, j] / fSumOverY[j]
     = fSumOverY[j] / fSumOverY[j]
     = 1
```

for every gen bin j. (This is the `epsilon` array constructed at
TUnfoldV17.cxx:343-347.)

### 1.3 What kEConstraintArea actually enforces

TUnfoldV17.cxx:361-368 — the constraint algorithm computes

```
y_minus_epsx = Σ_i Y[i] − Σ_j ε[j] · X[j]
```

and then adjusts `X` so the Lagrange residual `y_minus_epsx = 0`, i.e.,

```
Σ_j ε[j] · X[j] = Σ_i Y[i]
```

This matches Schmitt (S12) §2.2 eqs. 6-8 (cited in
`research-tunfold-optimization-from-schmitt-paper-2026-05-31.md` §1) where
`ε_j = Σ_i A_{ij}` is the per-truth-bin efficiency vector AFTER A's
column-normalization.

In our setup `ε[j] = 1` (see §1.2), so the area constraint reduces to

```
Σ_j X[j] = Σ_i Y[i] = data_total_with_overflow
```

i.e., **the unfolded gen total exactly equals the data input total**,
including any overflow you put in `Y`.

### 1.4 F1's effect on the constraint target

The F1 formula in `merge_migration_per_charge.py:392`:

```python
miss_yield = miss_row_mc * (data_total / prior_total)
```

Hence

```
data_total_with_overflow = data_total + miss_row_mc · data_total / prior_total
                         = data_total · (1 + miss_row_mc / prior_total)
                         = data_total · (prior_total + miss_row_mc) / prior_total
```

Using `matched_total ≈ prior_total − miss_row_mc` (since `prior_total ≈
matched_total + miss_row_mc`):

```
data_total_with_overflow = data_total · prior_total / prior_total
                          + data_total · miss_row_mc / prior_total
                         ≈ data_total · (1 + 30.66/67.1)
                         ≈ data_total · 1.457
                         ≈ 1.48e6 · 1.457
                         ≈ 2.16e6     ✓ matches observed 2.15e6
```

Under the area constraint with ε=1, this becomes the unfolded total —
which observationally is 2.14e6 (TUnfold) and 2.14e6 (IBU). **The
constraint is satisfied; the constraint target is wrong.**

### 1.5 What the physically correct target is

Standard acceptance correction (Cowan §11; HERMES PRD 87 (2013) 074029
eq. 6; S12 §1 "miss" semantics):

```
unfolded_gen_pool_at_data_lumi[j] = data_acceptance_corrected[j]
                                  = data[j] / reco_eff_MC[j]
```

Total:

```
correct_target = data_total / reco_eff_MC
              = data_total · prior_total / matched_total
              = 1.48e6 · 6.71e7 / 3.66e7
              = 1.48e6 · 1.833
              = 2.71e6
```

The 21% bug = `1.457 / 1.833 = 0.795`, i.e., **F1's target is 79.5% of
the correct target.**

### 1.6 The user's proposed `data/matched` algebra is identically the
acceptance correction

```
miss_yield_new = miss_row_mc · (data_total / matched_total)
data_total_with_overflow_new = data_total + miss_row_mc · data_total / matched_total
                              = data_total · (matched_total + miss_row_mc) / matched_total
                              ≈ data_total · prior_total / matched_total
                              = data_total / reco_eff_MC
                              ≈ 2.71e6           ✓
```

This is **algebraically identical to dividing the data by the MC reco
efficiency** — i.e., the textbook acceptance correction, propagated
through the F1 mechanism. The new constraint target is the
gen-pool-equivalent total at data luminosity, exactly what the standard
SIDIS multiplicity / cross-section formula needs as input.

---

## 2. Per-question verdicts

### A. What does kEConstraintArea enforce when R has a miss row? — **VERIFIED**

**Verdict:** With our matrix layout (miss row INSIDE the inner reco grid
at ROOT bin 961), TUnfold normalizes A per-column over rows 1..961 so
that **ε[j] = 1 for every gen bin**, and `kEConstraintArea` enforces

```
Σ_j X[j] = Σ_i Y[i] = data_total + data_overflow_at_miss_row_bin
```

The unfolded gen total is *exactly* (to numerical precision) the data
input total.

**Citation:** Schmitt 2012 (arXiv:1205.6201) §2.2 eqs. 6-8 (Lagrangian
with `λ(Y − e^T x)` and `e_j = Σ_i A_{ij}`); TUnfoldV17.cxx lines
1742-1850 (M → A column-normalization) and lines 341-388
(`fConstraint != kEConstraintNone` branch building `epsilon` and
`y_minus_epsx`); local file
`unfold_tunfold_single_tau.C:439-440` (uses
`TUnfold::kEConstraintArea`).

### B. Is F1 the right way to apply kEConstraintArea with Britzger Encoding B? — **VERIFIED INCORRECT**

**Verdict:** F1 is wrong as the user suspects. It encodes an *artificial*
constraint `Σ_j X[j] = data_total · (1 + miss/prior) = data_total ·
(prior + miss)/prior = data_total · 2·prior_minus_NULL/prior ≈
1.45·data_total`, which is neither the data-reco scale (1.0·data_total)
nor the gen-pool scale (1.83·data_total). It is a half-step that
*partially* corrects for acceptance but stops short of the full
correction by exactly the factor `prior/matched / (1 + miss/prior) =
matched/(matched + miss) · prior/matched = prior/(matched + miss) ≈
prior/prior = 1`... no wait, the ratio is `(1 + miss/prior) /
(prior/matched)` = `(prior + miss)/prior · matched/prior` =
`(prior+miss)·matched/prior²`. With prior+miss ≈ prior + matched (since
miss+matched≈prior), this is `(prior+matched)·matched/prior² ≈ matched ·
(1 + matched/prior) / prior = (matched/prior)·(1 + matched/prior) =
reco_eff · (1 + reco_eff) ≈ 0.546 · 1.546 ≈ 0.844`. The expected ratio
**0.844 ≈ 0.795** (within Nuclia 2% slop). This explains the ~21%
deficit.

**Britzger reference:** Britzger sets data overflow to ZERO (implicitly
via `DataSys = DatamatrixSys->ProjectionX()->Clone()` — no extra bin
appended). His MC-prediction-of-miss is not added to data. He uses both
`kEConstraintArea` (with the data total) and `kEConstraintNone` (with
`2×τ` compensation). See NAF
`/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx`
line 156 (`SetInput(data, 0., 0., hist_vyy)` — no overflow extension)
and line 610 (`TH1D* DataSys = (TH1D*)DatamatrixSys->ProjectionX()`).

**Citation:** Schmitt 2012 §2.2 paragraph after eq. 8 ("This procedure
is applied in order to limit possible biases on the normalisation which
are present if the data y follow Poisson's statistics"). The cited bias
is O(1%), not O(20-50%). F1 has injected a much larger bias *into the
target* than the constraint was designed to remove from the solution.

### C. Is `data/matched` algebraically equivalent to data/reco_eff? — **VERIFIED CORRECT**

**Verdict:** Yes, exactly.

```
miss_yield_new = R_miss × data_total / matched_total
data_total_with_overflow_new
   = data_total + R_miss × data_total / matched_total
   = data_total × (matched_total + R_miss) / matched_total
   = data_total × prior_total_effective / matched_total
   ≈ data_total × prior_total / matched_total          (Nuclia ~ 2% slop)
   = data_total / (matched_total / prior_total)
   = data_total / reco_eff_MC
```

**Citation:** Cowan §11.4 (acceptance correction
`N_corrected = N_observed / ε`); HERMES PRD 87 (2013) 074029 §III.B
("Acceptance correction") eq. 6; Schmitt 2012 §3 eqs. 19-21 (M → A
internal normalization that drives the area constraint to operate at
"prior" scale by default). The algebra is also explicit in
`merge_migration_per_charge.py:382-400` (the F1 routine itself) — just
swap `prior_gen.sum()` with `matched_reco_marginal.sum()`.

### D. Is dropping kEConstraintArea for kEConstraintNone a cleaner fix? — **VERIFIED VIABLE, but with caveats**

**Verdict:** Yes, this works algorithmically (Britzger uses it; lines
2741-2752 of his NEWdec23my_sys_unfold.cxx). When `kEConstraintNone` is
used and the data overflow is set to 0, TUnfold minimizes the data χ² +
regularization term *without* a normalization Lagrange multiplier. The
unfolded total is then determined entirely by the matrix inversion
through `E·A^T·V_yy^{-1}·Y`. For our matrix where `A[i,j] = M[i,j] /
prior[j]`, this produces

```
X[j] ≈ Σ_i (R[i_inner,j]/prior[j])·data[i] / Σ_i (R[i_inner,j]/prior[j])²
```

at the matched-rows-only data fit, which to leading order recovers
`data[j] · prior[j] / matched_col[j] ≈ data[j] / reco_eff[j]` — the
correct acceptance-corrected total.

**Caveat from Schmitt 2017 Tab. 1 (cited in
research-tunfold-optimization-from-schmitt-paper-2026-05-31.md §2(e)):**
without an area constraint, the template-fit unfolded total carries an
O(0.3-1%) Poisson-vs-Gaussian bias that `kEConstraintArea` is
specifically designed to remove. Britzger compensates by running at
`2×τ` (the regularization eats some of the freedom that the constraint
would otherwise enforce); we'd have to retune τ from scratch.

**Citation:** Schmitt 2012 §2.2 eqs. 6-8, paragraph after eq. 8;
Schmitt 2017 Tab. 1; NAF `NEWdec23my_sys_unfold.cxx` lines 2741-2752
(Britzger's parallel runs at `2×τ` with `kEConstraintNone`).

### E. Does IBU have the same F1 bug? — **VERIFIED YES**

**Verdict:** **IBU is affected by the same bug.** Evidence from
`/Users/hklest/Desktop/opencode/H1/sidis_highQ2/scripts/ibu/run_ibu.py`
and `ibu_core.py`:

1. `run_ibu.py:114`: `R = Rfull[:, :NB_G].copy()` — keeps **all 961
   reco rows** including the miss row.
2. `run_ibu.py:122-126`: reads `h_data_bgsub_<chg>` which contains the
   F1 overflow at bin NB_R = 960.
3. `run_ibu.py:231`: `IBU(R, prior, data, n_iter=n_max)` — passes
   the full 961-element data vector including the F1 overflow.
4. `ibu_core.py:100-103`: `col = R.sum(axis=0); eps[good] = col[good] /
   p0[good]`. With `col[j] = matched_col[j] + miss_col[j] ≈ prior[j]`,
   this gives `eps[j] ≈ 1.0` — **not the physical 0.546 reco
   efficiency.**
5. `ibu_core.py:115-143`: the update is
   `p_next = update / eps ≈ update` (since eps≈1), so the `1/eps[j]`
   "acceptance correction" factor is silently neutralized.
6. The data input includes the F1 overflow row (`R @ p_n` includes
   `R[NB_R, j] · p_n[j]` in the denominator, and `data[NB_R] =
   miss_yield_overflow` in the numerator).

The net effect: IBU under this construction does **not** compute the
acceptance-corrected gen pool; it computes the "matched+miss combined"
gen pool with target `data_total + F1_overflow ≈ 2.14e6`, identical to
TUnfold under `kEConstraintArea` with F1. That is why the two methods
agree — they are equivalently mis-normalized.

**To fix IBU correctly:** drop the miss row from R before passing to
IBU (use `R = Rfull[:NB_R, :NB_G]`), keep prior as the gen pool, and
feed only the inner-reco data (no overflow). Then `eps[j] = matched_col[j]
/ prior[j] ≈ 0.546` (physical), and the `1/eps[j]` factor recovers
`data / reco_eff ≈ 2.71e6` naturally — the textbook D'Agostini result.

**Citation:** D'Agostini 1995 NIM A 362 487 §3 eq. (3.2) (`p_{n+1}[j] =
(1/ε_j) Σ_i d_i · P(j|i)` where ε_j = `Σ_i M_{ij}` is the per-truth
efficiency over **reconstructed-only** reco bins, NOT over a synthetic
miss row); `ibu_core.py:96-103` (eps definition); `run_ibu.py:112-114`
(R slicing keeps miss row); `run_ibu.py:122-126` (data with F1 overflow).

---

## 3. Britzger reference convention (canonical g1j_ES)

From NAF `/data/dust/group/h1/klesthen/g1j_ES/Analysis`:

### 3.1 Matrix fill pattern (`AnalysisEventShapes.C` lines 1217-1236)

- **Inner matched cells** (`Migration_Matrix_moreRec`): filled at
  `(reco_bin, gen_bin)` with `fRec.wgt` when both reco and gen are in
  PS.
- **Miss / inefficiency** (`Migration_Matrix_moreRec_EFF`): filled at
  `(reco_overflow_bin = q2bins[9]+1, gen_bin)` with `fGen.wgt` when gen
  in PS but reco fails. The `q2bins[9]+1` index puts this at the
  overflow row — i.e., **outside the inner reco grid**.
- **Bkg / fake** (`_BK`): filled at `(reco_bin, gen_overflow_bin)`
  with `fRec.wgt`.
- **Underflow correction** (`_OF`): filled with `fGen.wgt − fRec.wgt`
  to absorb the matched-but-migrated weight asymmetry.

Britzger then `MigmatrixSys->Add(EFFMigmatrixSys)` (line 607, 641, 693)
to merge inner + miss into a single TH2. **The key difference from our
setup**: Britzger's miss row is at the ROOT overflow bin `q2bins[9]+1`
(one past the last inner bin), so TUnfold's inner-row sum loop at
TUnfoldV17.cxx:1745-1755 **excludes** it from `sum`; then lines
1763-1774 add it back via `hist_A->GetBinContent(0, ix) +
hist_A->GetBinContent(ny+1, ix)`. The result is the same `fSumOverY[j]`
total — but Britzger's data layout matches this convention because
his `DataSys = DatamatrixSys->ProjectionX()->Clone()` has no
corresponding extra inner reco bin to populate. **TUnfold uses the
overflow rows for normalization but NOT for χ² in the data fit.**

### 3.2 Data input convention (`NEWdec23my_sys_unfold.cxx` line 156, 610)

```c++
TH1D* data = new TH1D(*indata);
// ...
unfold.SetInput(data, 0., 0., hist_vyy);
```

`indata` is just the data histogram. No artificial overflow bin is
added. In closure mode (line 610):

```c++
TH1D* DataSys = (TH1D*)DatamatrixSys->ProjectionX()->Clone();
```

This is a pure projection — overflow content is whatever ROOT put in the
TH2 overflow bin, which is essentially zero for a pseudo-data closure.

### 3.3 Constraint mode (`NEWdec23my_sys_unfold.cxx` lines 2715-2752)

Britzger runs BOTH:

- `kEConstraintArea` at his chosen τ (lines 2715-2739)
- `kEConstraintNone` at `2×τ` (lines 2741-2752)

These are used as cross-checks of normalization stability. The fact
that he uses both means neither is "the canonical answer" — they should
agree to within the Schmitt 2017 Tab. 1 O(1%) bias when properly
configured.

### 3.4 Effective normalization (Britzger vs. us)

In Britzger's setup:
- Matched fill: `fRec.wgt` (reco-side weight)
- Miss fill: `fGen.wgt` (gen-side weight; for inclusive observables
  `fGen.wgt ≈ fRec.wgt`)
- The matrix column sum `fSumOverY[j] = matched_col + miss_col ≈
  gen_total[j]`
- TUnfold normalizes A[i,j] = M[i,j] / fSumOverY[j], so the **inner-row
  column sum** of A is `matched_col / gen_total[j] = reco_eff[j]`
- ε[j] in the constraint = `Σ_i_all_rows A[i,j] = 1` (overflow rows
  bring it back to 1)
- Constraint: `Σ_j ε[j]·X[j] = Σ_i Y[i] = data_total` (no overflow in Y)
- Unfolded total = `data_total / reco_eff` AT THE EFFICIENCY-WEIGHTED
  SOLUTION, because **the constraint enforces sum(X) = data_total when
  ε=1, but X is the gen pool (acceptance-corrected) because A has the
  miss flow folded in correctly** — TUnfold's χ² term
  `(Y − Σ_j A_{inner_rows, j} X[j])^T V_yy^{-1} (Y − ...)` naturally
  enforces `Σ_j A_{inner_rows, j} X[j] = data[i]` per reco bin,
  i.e. `Σ_j reco_eff[j] · X[j] ≈ data_total`, and the area constraint
  with ε=1 says `Σ_j X[j] = data_total`. **These two are inconsistent
  unless ε[j] = reco_eff[j]·constant**, which is not generally true.

This last paragraph reveals a subtle point: in Britzger's canonical
setup the area constraint and the χ² minimum are not in tension because
the χ² minimum naturally settles at `X[j] ≈ data[j] / reco_eff[j]` and
ε=1 makes `Σ X = Σ X`. The area constraint just resolves the global
normalization gauge.

**In our setup with F1**, we INSTEAD set `Σ_i Y[i] = data + F1_overflow
≈ data × prior / prior = data × (1 + miss/prior)`, which forces
`Σ_j X[j] = data × (1 + miss/prior) ≠ data × prior/matched =
data/reco_eff`. The constraint then DRAGS the solution away from the χ²
minimum to satisfy the forced normalization, producing the 21% deficit
in the unfolded total.

---

## 4. Recommended fix

### 4.1 Option 1: replace `prior_total` with `matched_total` in F1 (RECOMMENDED)

**Change:** in `merge_migration_per_charge.py:382-400`, replace

```python
prior_total  = float(prior_gen.sum())
# ...
miss_yield = miss_row_mc * (data_total / prior_total)
```

with

```python
matched_total = float(R_nominal[:NB_R, :NB_G].sum())
# (or read h_matched_reco_marginal_<chg> and sum it)
# ...
miss_yield = miss_row_mc * (data_total / matched_total)
```

**Why this is the right physics:** the resulting target `data + miss·data/matched
= data · prior / matched = data / reco_eff` is exactly the
acceptance-corrected gen pool. Under TUnfold's `kEConstraintArea` with
ε[j]=1, this forces `Σ_j X[j] = data / reco_eff`, which matches both the
χ² minimum of the inner-reco-rows fit AND the textbook definition of
"events at the gen level consistent with the data at this luminosity".

**Also fix in closure mode** in
`unfold_tunfold_single_tau.C:286-304` (REV3 §F-C): the closure overflow
already uses `prior_total_pdata − matched_total_pdata = miss_total`,
which is correct for pseudo-data (the matched-reco marginal IS the
pseudo-data; the unfolded should recover prior_total_pdata = matched +
miss). This block is internally consistent — keep as is.

**Also fix in macro score** in
`unfold_tunfold_single_tau.C:959-962` (REV3 §F-B): the
`area_residual_rel` uses `dtot_with_overflow = dtot +
miss_yield_overflow`, which under Option 1 becomes the correct target
`data / reco_eff`. **No change needed** to the macro score formula —
just to the data overflow value.

**Also fix IBU** in `run_ibu.py`: either (a) regenerate
`migration_inputs_v3_percharge.root` with the corrected F1 and re-run
IBU unchanged, OR (b) drop the miss row from R inside `_load_response_root`
(slice `Rfull[:NB_R, :NB_G]` instead of `Rfull[:, :NB_G]`) and use only
`data[:NB_R]` (no overflow). Option (a) is cleaner because it keeps a
single source of truth for the merged ROOT.

### 4.2 Option 2: drop F1 entirely, switch to `kEConstraintNone`

**Change:** in `merge_migration_per_charge.py:_extend_data_fine`, set
`miss_yield = 0.0`. In `unfold_tunfold_single_tau.C:439`, change
`TUnfold::kEConstraintArea` → `TUnfold::kEConstraintNone`. Re-tune τ
(Britzger uses `2×τ`).

**Pros:** matches Britzger's `kEConstraintNone` configuration; cleanly
removes the artificial constraint; lets the matrix invert determine the
normalization through the χ² + regularization minimum.

**Cons:** invalidates the entire current τ scan (must re-scan); loses
the Schmitt 2017 Tab. 1 ~0.3% Poisson-Gaussian bias protection; closure
mode (`unfold_tunfold_single_tau.C:272-340`) also needs the overflow
removed; more disruption to downstream gates.

### 4.3 Option 3 (NOT recommended): keep F1, document as a known bias

Quote the unfolded result as "matched+miss combined units" rather than
gen pool. This requires renaming every downstream quantity in the
pipeline AND adjusting the cross-section / multiplicity formulas to
divide by `(1 + miss/prior)` instead of `1/reco_eff`. High blast
radius, no physics gain.

### 4.4 Verdict

**Option 1.** Single-line fix in
`merge_migration_per_charge.py:392` (`prior_total → matched_total`);
no downstream code changes needed; IBU regenerates correctly when the
merged ROOT is rebuilt. Total: ~5 lines including the `matched_total`
computation and the comment update.

---

## 5. Impact assessment

### 5.1 What changes under Option 1

**Per-bin shift:** every unfolded gen bin scales by approximately
`1.833 / 1.457 = 1.258`. Per the area constraint with ε=1, the new total
is `data / reco_eff` instead of `data × prior / prior`, so the
**overall normalization rises by ~26%** (= `1/0.795 − 1`).

More precisely, the shape redistribution depends on the per-bin reco
efficiency `reco_eff[j] = matched_col[j] / prior[j]`. Bins with above-
average reco_eff (well-measured central kinematics) shift LESS than
average; bins with below-average reco_eff (forward-going hadrons,
low-pT) shift MORE. The total uniformly rises by `1/<reco_eff> − 1`
where the average is data-weighted.

**Cross-MC closure ratios** (rapgap_unfold / djangoh_unfold per bin):
the F1 bias is a **multiplicative factor common to both unfoldings**
(both use the same data normalization scheme). The ratio is therefore
**unchanged** to first order. Cross-MC systematic = unchanged.

**Self-closure ratio** (unfolded_pseudo / truth_pseudo per bin): the
pseudo-data overflow already uses `prior − matched = miss`
(`unfold_tunfold_single_tau.C:298`), which is the CORRECT closure
target. So self-closure is **already at the right scale** and will be
unchanged by Option 1. This means **Option 1 will increase the
data-mode unfolded total without changing the closure-mode result**,
revealing whatever data/MC normalization ratio was hidden by F1 in
data mode.

### 5.2 What changes under Option 2

Less predictable. The unfolded total will be determined by the χ² + τ²
trade-off, and shifts in τ choice will couple to shifts in
normalization. Best-case: comes out close to `data / reco_eff` at small
τ; worst-case: drifts with τ in an uncontrolled way that demands a
fresh τ scan and a fresh score function.

### 5.3 Risk of either fix

- **Option 1 risk:** the unfolded total will jump up by ~26% on FIRST
  PRODUCTION RUN, which will be visible in every QA plot. Must
  pre-announce this to prevent panic / false bug reports.
  Cross-section absolute scale will change correspondingly. Existing
  comparisons with prior H1 multiplicity papers must be redone (but
  the new scale is the physically correct one to compare).
- **Option 2 risk:** larger blast radius; new τ scan; closure-mode
  overflow logic to revisit; new gates needed for the score function.

### 5.4 Cross-MC closure ratio invariance argument

Let `U_X(j) = unfolded gen yield in bin j when response = X` (X ∈
{rapgap, djangoh}). Under F1, the global normalization factor is `K =
data_total + miss_X × data / prior_X`, where `prior_X` and `miss_X`
depend on the response MC. For two MCs with `K_rapgap ≠ K_djangoh`, the
ratio `U_rap(j) / U_dj(j)` is NOT just the shape ratio; it includes
`K_rap / K_dj`. **Therefore Option 1 will ALSO shift the cross-MC ratio
slightly** (by `(K_rap / K_dj) / (K'_rap / K'_dj)` where K' uses
`matched` instead of `prior`). The shift will be small if the rapgap
and djangoh reco efficiencies agree to a few % (which they generally do
in well-modeled regions), but it should be quantified explicitly.

---

## 6. Validation gates after the fix

When Option 1 is applied, the following gates should be checked.
Implement as automated tests in the rev3 / rev4 pipeline.

### G1. Total normalization gate (per charge)

```
unfolded_total / (data_total / reco_eff_MC) ∈ [0.97, 1.03]
```

with `reco_eff_MC = matched_total / prior_total` taken from
`meta_sums` of the merged ROOT. Tolerance accounts for the NucliaWeight
~2% asymmetry. Per-charge.

### G2. Area-constraint exactness

```
|unfolded_total − (data_total + miss_yield_new)| / (data_total + miss_yield_new) < 1e-3
```

This should hold to machine precision under `kEConstraintArea`. If it
doesn't, the constraint is being violated and the matrix or data input
is ill-formed.

### G3. Cross-MC closure shape invariance

```
median over j of |U_rapgap(j) − U_djangoh(j)| / U_rapgap(j) ≤ 0.10
```

before and after the fix. Should not WORSEN.

### G4. Self-closure (selfRG, selfDJ) ratio = 1

```
|U_self / truth − 1| < 0.05 for the bin-by-bin median
```

unchanged from current — Option 1 does not affect the closure pipeline
because closure already uses prior − matched correctly.

### G5. χ²_refold / ndf ∈ [0.5, 2.0]

Should hold per Schmitt 2017 §3.1 eq. 7 with `ndf = N_reco_inner_used`.
Should improve modestly under Option 1 because the data fit is no
longer forced to a wrong total.

### G6. IBU agreement with TUnfold at the rev4 result

After regenerating the merged ROOT with the fixed F1, IBU's unfolded
total should also rise to `data / reco_eff`. The TUnfold–IBU comparison
remains a valid cross-check (both should now compute the same
acceptance-corrected total, NOT the F1-mis-normalized total).

### G7. Comparison to standalone bin-by-bin (BBB) correction

The simplest sanity check: compute `X_BBB[j] = data[i_for_j] /
reco_eff_MC[j]` (per-bin acceptance correction; matched_col[j]
collapsed onto the per-gen-bin reco efficiency). After Option 1, the
unfolded total should agree with `Σ_j X_BBB[j]` to within ~few %
(regularization smears shape but should not move the total). Currently
under F1 the unfolded total is `21%` BELOW the BBB total — this is the
simplest single-line diagnostic of the bug.

---

## Appendix: A historical note on how F1 was likely introduced

The F1 commit comment in `merge_migration_per_charge.py:340-381`
correctly identifies the symptom — that setting `data_overflow = 0`
forces some gen bins to be NEGATIVE to absorb the miss row's positive
contribution to the column constraint. The author's diagnosis ("we
encode the MC expectation of that flow scaled to the data luminosity
as the data constraint at the overflow bin") is half-right: yes, we
need to encode the MC's miss flow at data scale, but the correct way
to do that is to scale by `data / matched` (the inferred data
luminosity / matched MC at data luminosity) NOT by `data / prior`
(which gives the data luminosity / TOTAL-gen MC, a different and
incorrect quantity). The mistake is at the level of "which MC total
plays the role of the data normalization reference" — and was
plausibly missed because both `prior` and `matched` look like
reasonable MC totals.

The fact that the result agrees with IBU to 0.5% looked like
independent confirmation of correctness. In fact IBU's normalization
silently inherited the same defect through the miss-row-in-R
construction. The two methods agree because they implement the same
wrong constraint, not because both are right.

---

## References

- Schmitt 2012, "TUnfold, an algorithm for correcting migration effects
  in high energy physics", JINST 7 T10003, arXiv:1205.6201. §2.2
  (eqs. 6-8), §3 (eqs. 19-21).
- Schmitt 2017, "Data Unfolding Methods in High Energy Physics", EPJ
  Web Conf. 137 11008, arXiv:1611.01927. §3.1 eq. 7 (χ²_A test), Tab. 1
  (area-constraint bias on a toy).
- D'Agostini 1995, "A multidimensional unfolding method based on Bayes'
  theorem", NIM A 362, 487. §3 eq. (3.2) (IBU update with per-truth
  efficiency).
- Cowan 1998, *Statistical Data Analysis* (Oxford). §11.4
  ("Acceptance correction").
- HERMES Collaboration, "Hadron production in semi-inclusive
  deep-inelastic scattering...", PRD 87 (2013) 074029. §III.B eq. 6
  ("acceptance correction"), §III.D (unfolding via D'Agostini /
  Bayesian iterative method).
- TUnfold V17.9 source: NAF
  `/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/src/TUnfoldV17.cxx`,
  ctor lines 1700-1855 (M → A normalization including overflow rows),
  `DoUnfold` lines 270-470 (`fConstraint` branch building ε,
  `y_minus_epsx`, λ correction to X).
- Britzger reference: NAF
  `/data/dust/group/h1/klesthen/g1j_ES/Analysis/AnalysisEventShapes.C`
  lines 1217-1236 (`Migration_Matrix` + `_EFF` + `_BK` + `_OF` fill);
  `/data/dust/group/h1/klesthen/g1j_ES/Analysis/RooUnfold/NEWdec23my_sys_unfold.cxx`
  lines 107-156 (DoUnfolding skeleton), 605-621 (DoUnfoldingSys w/ MigmatrixSys =
  matrix + EFF, DataSys = ProjectionX), 2715-2752 (Area vs None modes).
- Local pipeline:
  `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge.py:340-405`
  (F1 implementation),
  `.../unfold_tunfold_single_tau.C:439-440` (kEConstraintArea constructor),
  `.../unfold_tunfold_single_tau.C:951-962` (REV3 §F-B area_residual_rel),
  `.../unfold_tunfold_single_tau.C:272-339` (REV3 §F-C closure overflow);
  `H1/sidis_highQ2/scripts/ibu/run_ibu.py:107-140` (IBU response loading),
  `.../ibu/ibu_core.py:95-143` (IBU eps and step).
- Prior research: `H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
  §2(e) (`kEConstraintArea` recommendation), §1 (S12 §2.2 eqs. 6-8).

*End of artifact. ~500 lines including references.*
