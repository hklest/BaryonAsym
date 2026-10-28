# Critic Review: `electron_phi` systematic source (Scheme-B' 480-bin, v3p1)

**Date**: 2026-05-26
**Reviewer**: h1-analysis-critic
**Source**: `data/systematics_sources/source_electron_phi.json` + `cov_electron_phi.npz`
**Memo**: `docs/research/research-electron-systematics-2026-05-25.md` (§§3.2, 3.3, 4)

---

## VERDICT: **FATAL — the reported σ_phi_e is consistent with TUnfold/MC-stat noise, not a measured systematic, in essentially the entire bulk; the single-bin "headline" σ values are dominated by a pre-existing pathological row (cell 5, z=5) that already carries negative nominal yields, and the rank-1 fully-correlated covariance assigned to this noise is structurally indefensible.**

If shipped as currently constructed, the cov contributes random unfolding scatter to the total covariance with a fully-correlated correlation matrix — which both inflates and biases the budget. Two issues are individually killing; together they require either re-derivation with a noise/MC-stat subtraction or downgrading φ_e to an analytic upper bound.

---

## Material Reviewed

| Layer | Source | Verified |
|---|---|---|
| Convention/method (Andreev 2022/2023, ±1 mrad) | memo §§2.2, 2.3, 4 | yes (verbatim quotes; matches PRL 128 132002 & PLB 844 138101) |
| Implementation claim (shift φ_e by 0.001 rad, recompute scatEX/Y/Z, propagate Option B) | JSON header + memo §5.2 | partially — only the *outputs* (unfolded_nom/up/dn) are in the JSON; the actual rescale script, the recomputed CSV, and the response build/unfold steps are not on local disk |
| Cov structure (rank-1, fully correlated) | NPZ | yes — verified `cov == outer(delta, delta)` to machine precision (residual 0.0 / 1.4e6) |
| Per-bin numbers | NPZ + JSON | yes — 480 bins; nominal, up, down, delta, sigma, fractional sigma, by-stratum headline all recomputed and reproduce the brief's headlines |

I did NOT have access to the per-track CSV, the rescale script, or the response/unfold code — those live on NAF. All findings below come from the unfolded triples themselves, which is sufficient for the noise / signal discrimination.

---

## Fatal Issues

### F1. The reported σ is statistically indistinguishable from common-mode TUnfold noise across the bulk

**Claim** (memo §3.2, §1, §4): "Lepton(ϕ) sub-dominant for cross-section means in bulk bins, comparable to HFS for ⟨Nc⟩"; expected per-mille on bulk yields.

**Evidence** (decomposition of (up, down) shifts into antisymmetric = signal and symmetric = common-mode):

For 480 bins, with δup = up − nom, δdn = dn − nom, the antisymmetric component (δup − δdn)/2 ≡ δ_signed (this is what the cov uses) and the symmetric component (δup + δdn)/2 measure two different things:
- antisymmetric → the linear response to a +δφ shift (signal)
- symmetric → curvature OR common-mode noise (in particular, independent TUnfold realisations dressed up as up/down)

| stratum | median \|δ_antisym\| | median \|δ_sym\| | ratio sym/antisym | bins with \|sym\|>\|antisym\| |
|---|---:|---:|---:|---:|
| S0 [150,300) | 2.41 | 2.64 | **1.09** | 57% |
| S1 [300,700) | 2.58 | 2.48 | **0.96** | 39% |
| S2 [700,1500) | 2.34 | 3.58 | **1.53** | 58% |
| S3 [1500,20000) | 2.88 | 3.00 | **1.04** | 51% |

For a genuine, linear systematic the antisymmetric component should dominate (sym is at most O(δ²); here both up and dn shifts are ~1 mrad apart so curvature is negligible). For pure independent TUnfold noise on each variation, antisym and sym are both ~σ_noise/√2 with the *same* magnitude. The observed ratio ~1 in every stratum is the classic signature of **noise-dominated half-difference**, not of a measured ±1 mrad response.

Confirming with sign structure: across the 480 bins, n(δ>0)=256, n(δ<0)=224. Under H0 (random sign) the std is √480/2 ≈ 11; the observed excess of 16 is at most 1.5σ — fully consistent with sign-random. A genuine ϕ rotation of the lepton-scattering plane should leave a coherent sign pattern across bins (since the response of the CM-frame φ_h binning to a +ϕ shift has a specific structure correlated with the bin index). There is no such pattern.

**Mechanism**: the analysis runs TUnfold with τ = 0.01 on a 480-bin response matrix per variation. Each independent unfold is a separate stochastic realisation (the regularisation interacts with statistical fluctuations in the response and in the input). Three calls (nominal, up, down) produce three different noise realisations on top of an extremely small (per memo §3.2: ~1 mrad-rotation, on a yield observable that integrates over φ_h, expected ≲ per-mille) signal. The half-difference (up − dn)/2 is therefore dominated by `noise_up − noise_dn` ≈ √2 · σ_unfold, **not** by the genuine 2 × (linear response to δφ).

**Why this is killing**: a sigma that is in fact noise has none of the properties the cov is asserting:
- It is **not** correlated across bins. The cov here asserts ρ_ij = sign(δ_i) sign(δ_j) ∈ {−1, +1}, but if δ is just `noise_up − noise_dn` per bin, the signs are independent across bins. The rank-1 outer-product structure is wrong by construction.
- It is **not** signed in a physically meaningful way. The H1 convention (Andreev 2024 §6: "signed difference between nominal and shifted upwards") presumes the shift induces a coherent response; when the response is below the unfolding noise floor, the sign is random.
- It will **add into the total covariance** at face value, inflating the budget without representing any physical degree of freedom.

**Killing follow-up**: do at least one of —
1. **MC-stat / bootstrap subtraction**. Run the unfolder ≥30 times with bootstrapped pseudo-data on the *nominal* response; measure σ_unfold(i) per bin. The true φ-systematic per bin is the quadrature-difference √(σ_phi_e_measured² − 2σ_unfold²)_+ (the factor √2 because up and dn are independent realisations). Bins where this is consistent with zero get σ_phi_e set to a zero-or-bound value (e.g. an analytic per-mille bound from §3.2 of the memo), not to the noisy half-difference.
2. **N-replica re-unfold**. Re-run the unfold a few times with different seeds on the *same* shifted CSV; the spread across replicas is σ_unfold; the φ-induced shift only appears as the bin-by-bin mean offset across replicas.
3. **Drop the bin-resolved cov and quote an analytic upper bound** for φ_e: from §3.2 of the memo, the per-bin yield shift from a ±1 mrad rotation in a φ-integrated observable is ≤ per-mille. A diagonal cov with σ_phi_e(i) = 0.001 × |N_nom(i)| would be physically defensible; the current cov is not.

### F2. Six bins on the cell-5, z=5 row carry 50% of the total |σ| budget and sit on a known pre-existing artefact

**Claim** (headline): "max fractional σ: 446% (sparse-corner artifact)" treats the extreme bin as a curiosity. In fact the entire S2 anomaly that the brief asks about ("S2 is 30× S0 in Σ|σ|/Σ|N|") is concentrated in **6 bins**.

**Evidence**:
- 6 bins reach 50% of the global Σ|σ|; 152 bins reach 90%. The cov is therefore not a measurement of 480 bins, it is a measurement of ~6 bins inflated by 474 bins of noise.
- The top-6 are **all** in cell-5 (`S2b: Q² ∈ [700,1500), upper-x`), z=5 (0.7 < z ≤ 1.0), pT_h ∈ {0..4}, both charges:

  | rank | charge | cell | z | pT | nominal | δ | σ/\|N\| |
  |---:|:--:|:--:|:--:|:--:|---:|---:|---:|
  | 1 | h⁻ | 5 | 5 | 3 | **−38080** | +1196 | 0.031 |
  | 2 | h⁻ | 5 | 5 | 2 | **−22193** | +709 | 0.032 |
  | 3 | h⁺ | 5 | 5 | 3 | +20647 | −389 | 0.019 |
  | 4 | h⁺ | 5 | 5 | 2 | +16139 | −246 | 0.015 |
  | 5 | h⁻ | 5 | 5 | 1 | **−6224** | +207 | 0.033 |
  | 6 | h⁺ | 5 | 5 | 4 | +7706 | −123 | 0.016 |

- Three of the top-6 nominals are large **negative** yields — these are the pre-existing-pathology bins (n_negative_nominal_bins=33 across the grid). The unfolding is producing oscillating sign-flipped yields in this row (h⁻ at z=5 has nominals of {+4803, −6224, −22193, **−38080**, +28} across pT_h, with a sign flip every bin), which is a TUnfold regularisation artefact, not a physical yield.
- The Σ|σ|/Σ|N| breakdown within S2:
  - cell 4 (S2a, lower-x): ratio = 0.00047 (perfectly normal, per-mille as memo predicts).
  - cell 5 (S2b, upper-x): ratio = **0.00800** — **17× cell 4**.
  - S2 cell-5 carries 3401 of the 3570 total |σ| units in S2 (95%).

**Mechanism**: cell-5/z=5 is a row where the response matrix is degenerate or strongly off-diagonal (high-z, mid-pT, upper-x in S2 has very low MC statistics and large bin-to-bin migration), and the τ=0.01 regularisation produces large oscillating values. *Any* small perturbation of the response or the input data re-shuffles the regularised solution within that row by O(1000s) yield units — not because the input physics shifted, but because the regularisation has multiple near-equivalent solutions. The "systematic σ" measured here is the **regularisation instability**, not the φ_e response.

The same applies to the S3 cell-7 row (next-most-pathological), the row 6, 7 from the second-largest concentration in the top-20.

**Why this is killing**: feeding a 1000-unit "systematic" originating from regularisation instability into a *fully correlated* rank-1 cov means:
- The cov asserts: "if you knew the true φ_e was +1 mrad instead of 0, **every** bin would shift coherently in the direction encoded in `delta_signed_per_bin`." That's false for these bins — the sign and magnitude in cell-5/z=5 is set by the regulariser's choice among degenerate solutions, not by the physics.
- In bin-summed observables (cross-sections, A_h, harmonics), these bins will pull the result in arbitrary directions with weight σ², and the rank-1 outer product will assign them as **fully correlated** to bins in the bulk that have nothing to do with the regularisation instability.

**Killing follow-up**:
- Exclude the 33 negative-nominal bins from the cov (set σ=0, ρ=0) on the basis that they are pre-existing TUnfold artefacts and the analysis already treats them specially downstream (the brief acknowledges this). The current `sigma_per_bin` includes them.
- Separately, exclude the cell-5/z=5 row from the rank-1 structure (or split the cov as block-diagonal: one block for the bulk bins, one block for the pathological row, and assign the latter zero off-diagonal correlation).
- Alternatively, mask the entire z=5 row for *all* systematic sources, declare it not physically interpretable, and quote results only on z ∈ {0,1,2,3,4}.

### F3. Rank-1, fully-correlated cov is structurally inconsistent with the noise-dominated δ

Verified: `cov_electron_phi.npz['cov'] == outer(delta_signed, delta_signed)` to 0.0 residual. So `corr[i,j] = sign(δ_i) sign(δ_j)` exactly.

But §F1 shows δ_signed is not the linear response — it is `noise_up − noise_dn` per bin. The signs of δ_signed are therefore independent across bins (confirmed by the near-50/50 sign split, 256/224). Asserting ρ_ij = ±1 for all (i,j) pairs based on random signs gives a covariance whose correlation matrix is *not* the physically true correlation matrix — it is a random ±1 pattern that will alias as a coherent shift downstream.

The downstream consequence is well-defined: when this cov is summed into a 1D observable (e.g. integrated cross-section per Q² bin), the contribution is `(Σ_i s_i δ_i)²` where s_i is the bin weight. For random-sign δ_i with magnitudes ~σ_noise, this scales as Σ s_i² σ_noise² — the same as a *diagonal* cov of the same per-bin magnitudes. So the immediate downstream damage in scalar summaries is the same as treating each bin as uncorrelated. **But** in matrix-valued downstream uses (eigendecomposition for pull tests, profiling, BLUE combination across observables), a rank-1 cov with the wrong principal direction is much worse than a diagonal cov of the same magnitudes — it forces all the "uncertainty" into one direction in observable space that is physically arbitrary.

**Killing follow-up**: do not ship a rank-1 cov for a systematic whose per-bin response is noise-dominated. The defensible options are:
1. After the F1 noise subtraction, if any bins survive with statistically significant shifts, keep them with rank-1 structure and zero out the rest.
2. Quote σ_phi_e as a diagonal cov bounded by the analytic per-mille estimate from memo §3.2 and call out in the systematics table that φ_e is bounded, not measured.

---

## Major Concerns

### C1. Q² invariance under φ_e shift is NOT clean at the unfolded level — cell 5 breaks badly

The supervisor flagged: under a pure φ_e rotation, Q²_es should be invariant ⇒ no event leaves its (Q²,x) cell ⇒ the sum yield within each (charge, cell) should be near-invariant. I cannot inspect the per-event CSV, but I can test the unfolded-level analogue (sum over z, pT within each (charge, cell)):

| charge | cell | nom_sum | (up − nom)/nom | (dn − nom)/nom |
|:--:|:--:|---:|---:|---:|
| h⁺ | 0 (S0a) | 1150912 | +1.2e−5 | +1.1e−5 |
| h⁺ | 1 (S0b) | 929122 | +1.8e−6 | −1.3e−5 |
| h⁺ | 2 (S1a) | 624647 | −5.4e−6 | −0.3e−6 |
| h⁺ | 3 (S1b) | 478514 | −4.1e−5 | −4.9e−5 |
| h⁺ | 4 (S2a) | 180561 | −1.9e−4 | −3.6e−4 |
| h⁺ | 5 (S2b) | 200754 | **+1.0e−2** | **+1.8e−2** |
| h⁺ | 6 (S3a) | 87161 | +5.1e−4 | +1.1e−3 |
| h⁺ | 7 (S3b) | 197634 | +2.5e−3 | +4.1e−3 |
| h⁻ | 5 (S2b) | 87425 | **+4.2e−2** | −2.0e−3 |

Cells 0–4 are at the 10⁻⁵ to 10⁻⁴ level (consistent with TUnfold noise on the cell-sum). Cell 5 is at the **10⁻² level for both charges**, ~100× worse. This is the same z=5 pathology row dominating the (charge, cell) sum.

Two non-exclusive explanations:
(a) **Regularisation instability** in cell-5 redistributes O(10⁴) yield units within the z-slices on every unfold call → the (charge, cell) sum is not strictly conserved by the unfold even under a tiny input perturbation. This is the F2 mechanism applied at the cell level.
(b) **Acceptance/cut migration** under a φ shift through the LAr fiducial-φ cut, the Empz cut, or the (Q²,x) edge crossings via second-order θ_e mixing. The memo §3.2 says first-order none of this is relevant for a pure φ rotation; the cell-sum non-invariance here exceeds any plausible second-order estimate.

**Cannot distinguish (a) from (b) without inspecting the shifted CSV row count vs. nominal** — that is the killing follow-up. If the shifted CSV has the same number of rows per (Q²,x) cell as nominal (with only φ_h per row changed), then (a) is the explanation and the cell-5 instability is a TUnfold artefact. If the shifted CSV loses or gains rows in cell 5, the shift is leaking through a cut and there is a real (small) acceptance systematic that the rest of the propagation may or may not capture.

### C2. A_h cancellation is not happening — 54% of (cell, z, pT) bins exceed the memo's 0.001 absolute bound

Memo §4 promises: "σ_electron on A_h ≤ 0.1 % absolute per bin" based on the H1 jet-charge analog.

From the cov, propagating A_h = (N⁺ − N⁻)/(N⁺ + N⁻) using the per-bin δ values:

| metric | value |
|---|---:|
| median \|δA\| over (cell, z, pT) bins | 1.36e−3 |
| p84 \|δA\| | 1.02e−2 |
| max \|δA\| | 12.3 (pathological bin) |
| fraction of bins with \|δA\| > 0.001 (memo bound) | **54 %** |

The memo bound of 0.001 absolute on A_h is exceeded by half the bins. Most of this is the F1 noise (no cancellation between h⁺ and h⁻ noise — they are independent unfold calls in the brief's pipeline if h⁺ and h⁻ are unfolded jointly within the 480-bin grid; if they're unfolded separately, even more decorrelated). A genuine common-mode φ_e systematic would cancel in A_h to the per-mille level — the failure to cancel is direct evidence the per-bin δ is not a common-mode physical shift.

**Killing follow-up**: same as F1 — measure σ_unfold per bin, subtract it, and confirm the residual δ_phi_e is common-mode in (charge_idx 0, charge_idx 1) pairs as it must be for a per-event lepton shift.

### C3. Up/down median asymmetry of 5.5 yield units is consistent with `(noise_up − noise_dn)` of order √2 · σ_unfold

Brief's headline: "median up/down asymmetry: 5.5 yield units." Given that the median |sym shift| = 2.77 and median |antisym shift| = 2.52, both around 2–3 yield units, this is exactly what `noise_up − noise_dn` looks like for σ_unfold ~ 2 units. Consistency check passes — but it is *consistency with noise*, not with a 1-mrad-shift signal.

---

## Non-Blocking Checks Confirmed

| Check | Status |
|---|---|
| `cov == outer(delta, delta)` (rank-1 implementation correct) | yes (residual 0.0) |
| Sigma_per_bin == \|delta_signed_per_bin\| | yes |
| 480-bin index convention matches JSON header | yes |
| Convention (Andreev 2022/2023 PRL/PLB, ±1 mrad on φ_e) | matches published H1 prescription verbatim |
| Memo prior (§3.2: φ_e shifts only φ_h, leaves Q²_es invariant first order) | physically correct |
| Headline numbers in JSON reproduce from the per-bin arrays | yes (median, p84, max, by-stratum all match) |
| n_negative_nominal_bins = 33 (matches the brief) | yes |
| n_same_side_bins = 246 (matches the brief) | yes |

---

## Missing Evidence

| Item | Why needed | Where it should live |
|---|---|---|
| The CSV rescale script (`electron_systematic_csv_rescale.py` per memo §7.A Phase 1) | confirm only `scatEX/Y/Z` rotated, |p_e|/θ_e/scatEE held fixed, Q²_es not recomputed from shifted scatE_i | scripts/ |
| Per-event row counts: nominal vs shifted CSV, per (Q²,x) cell | distinguish C1(a) regularisation from C1(b) cut migration | should be a logged QA in the rescale script |
| Bootstrap / multi-seed unfold replicas of the nominal unfold | derive σ_unfold(i) and let us subtract it from σ_phi_e | required input to fix F1 |
| Linear-response check: do up and dn shifts have opposite signs at the per-bin level outside cell-5/z=5? (sym/antisym test by bin index, not aggregate) | would show whether there is *any* coherent signal hiding in the bulk | derived from the same files I already have — see below |
| Comparison to the φ_e Option A spot-check the memo §7.B prescribes | establishes whether the Option B implementation is itself faithful | not produced yet |
| Inspection of the actual `pT_h_idx`, `z_idx`, `cell_idx` definitions for the shifted CSV (no row drops vs nominal?) | F2 / C1 root cause | NAF |

---

## Recommended Next Check

**Highest priority — do this before any other φ_e work proceeds**:

Run a **bootstrap σ_unfold characterisation**. Take the nominal v3p1 input, generate N≥30 Poisson-resampled pseudo-data replicas, unfold each with TUnfold-4D at τ=0.01, and per bin compute σ_unfold(i) = std across replicas. Then for every bin in `cov_electron_phi.npz`:

  σ_phi_e_corrected(i) = sqrt( max( σ_phi_e_measured(i)² − 2 · σ_unfold(i)², 0 ) )

The factor 2 comes from the half-difference of two independent realisations. Report:
  (a) the fraction of bins where σ_phi_e_corrected is consistent with zero;
  (b) the corrected per-stratum medians; and
  (c) the residual cov (rank-1 outer product of the corrected δ, **zeroed for any bin where corrected σ = 0 OR where nominal < 0**).

I expect that after subtraction, ≥80% of bins in S0/S1 and a large fraction in S2/S3 will fall to zero, leaving a much smaller, physically defensible φ_e cov dominated by a handful of high-z / high-pT_h bins where the boost rotation does have measurable response. If after subtraction essentially nothing is left, downgrade φ_e to a **diagonal analytic per-mille bound** (σ_phi_e(i) = 0.001 · |N_nom(i)|) and note in the systematics table that φ_e is bounded by the published convention rather than measured.

Until this characterisation is run, **do NOT ship the current `cov_electron_phi.npz`**. Treating noise as a fully-correlated systematic is worse than dropping the source: it injects a phantom coherent shift into every downstream bin-summed quantity.

Secondary checks (do after the bootstrap):
- Per-event row-count diff (shifted CSV vs nominal) per (Q²,x) cell → resolves C1.
- Verify rescale script reads `scatEE` and recomputes `scatEX/Y/Z` from |p|·(sinθ cosφ_sh, sinθ sinφ_sh, cosθ) — written naively this can pick up a (1 + O(δφ²)) factor that contaminates the energy.
- A_h propagation of the corrected cov; confirm the memo's 0.1% absolute bound is now satisfied.
