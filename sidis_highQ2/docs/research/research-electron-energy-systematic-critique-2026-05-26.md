# Critique: source_electron_energy systematic (HQ-v5g-Bprime 480-bin grid)

**Date**: 2026-05-26
**Reviewer**: h1-analysis-critic
**Target**: `data/systematics_sources/source_electron_energy.json` + `cov_electron_energy.npz`
**Reference**: `docs/research/research-electron-systematics-2026-05-25.md`
**Implementation (on NAF, not directly read)**: `scripts/systematics/source_electron_shift.py` + `run_electron_systematic.py`

---

## VERDICT: **FATAL**

The source as delivered should **not** be used as the canonical scattered-electron-energy systematic for any cross-section, multiplicity, or asymmetry table that is to leave this workspace. Two independent killing issues plus four concrete major concerns are documented below. The 244/480 same-side bin count is not a damping artifact — it is the rank-1 covariance construction silently throwing away half of the actual electron-scale uncertainty.

This is *not* a verdict that the analysis can never produce a defensible electron-scale systematic — it is a verdict that **the present artifact does not faithfully quantify the H1 ±0.5 %/±1 % scattered-electron energy scale on the unfolded h± yields**. The fixes are mechanical and laid out in §F.

---

## What was reviewed

* The full source JSON (480 nominal, up, down, signed-delta, sigma, fractional-sigma arrays, plus headline + stratum stats). Read end-to-end.
* The NPZ companion: shape, eigenstructure, correlation matrix, bin metadata.
* The canonical convention memo: §1, §2.1–§2.5, §3.1–§3.5, §5.1–§5.2, §7.A, §8 read in full.
* Independent re-derivation of:
  - half-difference formula `δ_i = (up_i − dn_i)/2` against stored `delta_signed_per_bin` and `sigma_per_bin` — **CONFIRMED**.
  - cov as outer product `δ ⊗ δ` — **CONFIRMED bit-identical** (`np.allclose` true; second eigenvalue 5e−8 vs leading 1.1e9; |corr| ≡ 1 everywhere).
  - same-side count (`(up−nom)·(dn−nom) > 0`): **244/480, matches**.
  - the §3.5 algebra `δQ²/Q² = δ(2±y)`: independent derivation below — **memo §3.5 has a sign error; correct is δ(2−y); smoke is right; old formula is wrong**. Memo §3.5 *did* note this internally as a "correction" but the corrected statement also went the wrong way (it kept "δ(2+y_es)"). See §C2.

The implementation script on NAF was not read directly. All claims about its behavior in this critique are constrained to what the dispatcher asserted plus what is necessarily true from the numeric outputs.

---

## A. Fatal Issues

### A1. The same-side construction (244/480 bins) is silently throwing away the systematic in half the grid — and this is *not* a TUnfold τ artifact.

**Claim under review**: "n_same_side=244 is likely TUnfold τ=0.01 regularization damping the up & down perturbations toward the smooth-prior fixed point; rank-1 cov from |up−down|/2 is mathematically fine."

**Counter-evidence**:

1. Numerical reconstruction (independent):
   - Median of the "asymmetry metric" `|((up−nom)+(dn−nom))| / (|up−nom|+|dn−nom|)` across the 244 same-side bins is **exactly 1.000**. That means in *every* same-side bin, up and down do not partially co-move — they fully co-move. There is no partial-damping signature anywhere.
   - In opposite-side bins this metric is 0.476 (i.e. up and down are roughly mirror images with ~50 % residual size asymmetry — the expected behavior of a coherent shift on a noisy estimator).
   - Same-side fraction increases monotonically with Q²: **S0 38/120 → S1 55/120 → S2 70/120 → S3 81/120** (statistics gradient: where stats die, both shifts get noise-dominated and co-move). Also rises with z and pT_h. This is exactly the pattern of a **noise-dominated, not regularization-dominated**, response.

2. Mechanism: τ regularization toward the prior could in principle shrink **both** `up` and `down` differentials toward zero, producing small |δ|. It cannot produce the observed pattern where up and down are not mirror images at all (asymmetry metric = 1). A symmetric τ damps both ends symmetrically; it preserves the relative *sign* of the perturbation. The observed same-side bins have lost sign-coherence entirely. That is **noise on the unfolded answer dominating the signed-shift signal**, not regularization shrinkage.

3. Consequence for the rank-1 cov: in a bin where the true coherent shift is δ_true but the (up, dn) noise envelope is σ_noise > |δ_true|, the half-difference `(up−dn)/2` is a noise estimator with expectation ~0 and width ~σ_noise/√2. The cov entry then is rank-1 with a *random* sign and a typical-noise magnitude, fully correlated by construction to all other bins' noise. That is **wrong by construction**: the truth is "this bin's electron-scale systematic is bounded by the noise floor and uncorrelated to other bins' truth shift"; the artifact says "this bin's electron-scale systematic is δ_noise and 100 % correlated with the coherent shift in every other bin." The two propagate to *completely different* downstream errors on any linear combination (cross-section integrated over z, for example, or A_h).

4. **Concrete bias**: in the 244 same-side bins, σ_per_bin is *not* an underestimate of the true electron systematic in the bulk-magnitude sense — it is roughly the noise floor of the unfold response to a small perturbation. The actual electron systematic in those bins, from the published H1 priors in memo §8.1, should be ≈ 0.5–1 % of |N_nom|. The recorded σ in same-side bulk bins (median |σ|=15 vs median |N|=922 ⇒ 1.6 %) is in the right *magnitude ballpark* by accident, but the sign assignment is random — so any signed sum (e.g. an asymmetry) sees the wrong correlation structure.

**Physical mechanism of bias on downstream products**:
- For per-bin σ on yields: the recorded σ is approximately the unfold-noise envelope under a small reco shift. It happens to be ~comparable to the published prior in the bulk, so per-bin yield uncertainty quoted from this source will be roughly right *in magnitude*. **However**, the bin-to-bin sign pattern is randomized noise, not the true coherent electron-scale response.
- For any signed linear combination (cross-section integrated over z; multiplicity ratio; charge asymmetry A_h): the rank-1 cov is full-rank-1 with sign = sign(δ_recorded). Because sign(δ_recorded) in 244 bins is essentially a coin flip, the integrated-yield error is computed using a *random* sign pattern and will randomly cancel or add. This is the worst possible structural bias for asymmetry-type observables.
- For the cross-bin combined error: 244 bins are forced into rank-1 lockstep with the opposite-side bins' signs, which is physically incorrect — the truth is that the coherent shift dominates the opposite-side bins and noise dominates the same-side bins. These are two different physical states; one rank-1 matrix cannot represent both.

**Killing question**: with τ=0.01 fixed, what is the unfold's per-bin noise floor under a *zero* shift (e.g. two statistically independent halves of the same nominal data, unfolded separately)? If that noise floor is comparable to or larger than the recorded |δ| in the same-side bins, the same-side σ is noise, not signal. This is the test that has to pass before this source is usable.

**Likely fix**: either (a) take the *symmetric component* `(|up−nom| + |dn−nom|)/2` as σ (loses sign, but is bounded above by the noise floor and not below), with σ assigned via Andreev-2024-style "signed difference under up-shift only" convention; (b) refuse to publish the rank-1 cov structure and use a diagonal cov of `σ_i^2` instead (still wrong, but at least does not lock 244 noise bins into coherent rank-1 with 236 signal bins); (c) run a noise-floor closure (same input, two unfolds with different RNG seeds or different τ neighborhoods) to subtract the noise floor in quadrature before reporting σ.

### A2. Rank-1 cov with |corr| ≡ 1 forces 244 noise-sign bins into full coherence with 236 signal bins. This is structurally incorrect.

**Claim under review**: "Cov[i,j] = δ_i × δ_j with δ_i = (N_up − N_dn)/2. Mathematically fine."

**Counter-evidence**:
- `np.allclose(cov, np.outer(δ, δ))` is **true bit-for-bit**. `np.abs(corr)` is identically 1.0 in every off-diagonal entry. Second eigenvalue of cov is 5e−8 vs leading 1.1e9 — pure rank-1.
- This structure is the correct one for a single coherent physics shift only when the per-bin δ is the **signal** response of that bin to that coherent shift, not noise. For 244 of 480 bins, δ is noise (see A1). The cov then claims that the noise in bin i is 100 % correlated with the signal in bin j ≠ i. This is provably false for any sensible noise model.
- For any downstream summary that contracts the cov with a non-trivial vector w (e.g. integrating over z to get σ^h(Q², x, pT_h): w_i ∈ {1, 0}; or computing A_h = (N+ − N−)/(N+ + N−): w has signed entries), the quadratic form w^T C w under the rank-1 cov is `(w · δ)^2`. With sign(δ) random in 244 bins, `w · δ` is a random walk whose expectation has nothing to do with the true uncertainty on w^T N.

**Physical mechanism of bias**:
- *Sum* observables (Σ_i N_i over z and pT_h within a (Q², x) cell): the rank-1 cov gives error `|Σ_i δ_i|`. The signal-bin contribution adds coherently and correctly; the 244 noise-bin contributions add as a random walk and accidentally cancel by ~√N_noise. **The reported integrated systematic is biased low** by roughly the fraction `√(N_signal² + N_noise) / (N_signal + N_noise)` ≈ 0.5–0.7 depending on cell.
- *Difference* observables (h+ minus h−): the rank-1 cov gives error `|δ_h+ − δ_h−|`. The reviewer measured `<|δ_h+ − δ_h−|> / <|δ_h+|> ≈ 0.99` across matched (cell, z, pT) bins. That means the cov has no charge-cancellation built in — *for an asymmetry observable, this source contributes roughly the full per-bin magnitude with no cancellation*. The physical truth (memo §4) is that electron systematics are charge-symmetric to leading order and should cancel almost completely in A_h. **The cov structure forbids this cancellation**, overstating σ on A_h by a factor of ≈ 10 (memo §4: true σ_electron(A_h) ≤ 0.1 %; recorded structure permits ≈ 1 %).

**Killing question**: is the rank-1 cov structure the right propagation for an asymmetry analysis in this analysis? If the supervisor's pipeline contracts this cov against a charge-difference operator, the answer is **no** by construction.

**Likely fix**: either (a) use a *signed* propagation convention (Andreev 2024 §6: "signed quantities corresponding to the difference between the nominal angles and the angles after the systematic shift") with the *single up-shift* δ as the signed input, propagated coherently across bins and across charges — this gives the correct cancellation in asymmetries; (b) split the cov into signal-bin block (rank-1 with the coherent δ) and noise-bin block (diagonal with bounded σ). Both require re-deriving the source from up-only or rerunning with a noise-suppressed unfold.

---

## B. Major Concerns

### B1. The §3.5 algebra in the memo is wrong (it had a sign error, internally flagged, but the "correction" also went the wrong direction). Implementation smoke is right.

**Memo statement**: §3.5 paragraph "**Correction to §3.1 above…**", final line: `δQ²_es / Q²_es ≈ δ_E · (2 + y_es)`, giving +1.25 % at y=0.5.

**Independent re-derivation** (Σ-method, fixed θ_e, ΔE_e' = δ·E_e'):
```
Σ_e_new = (1+δ) Σ_e
y_new   = Σ_had / (Σ_had + (1+δ)Σ_e)
1 − y_new = (1+δ) Σ_e / (Σ_had + (1+δ) Σ_e)
(1 − y_new)/(1 − y_old) = (1+δ) · (Σ_had + Σ_e) / (Σ_had + (1+δ) Σ_e)
                        = (1+δ) / [1 + δ · Σ_e/(Σ_had+Σ_e)]
                        = (1+δ) / [1 + δ · (1−y)]
                        ≈ 1 + δ − δ(1−y)
                        = 1 + δ·y                (to first order)

Q²_new / Q²_old = (1+δ)² · (1 − y_old)/(1 − y_new)
                = (1+δ)² / (1 + δ·y)
                ≈ 1 + 2δ − δ·y
                = 1 + δ·(2 − y)
```

Numerical check at δ=+0.005, y=0.5: exact ratio − 1 = +0.7506 %, first-order = +0.7500 %. Matches the smoke value (+0.77 % at +0.5 %). **The memo formula (2+y) is wrong by sign; the correct first-order coefficient is (2−y), confirmed by smoke.**

**Mechanism**: the memo derivation conflates the sign of `δy_es`. Under an *upward* E-scale shift, Σ_e grows ⇒ y decreases (less hadronic fraction relative to total) ⇒ (1−y) increases ⇒ Q² gets a *smaller* denominator boost than the naive (1+δ)² ⇒ net is δ(2−y), not δ(2+y).

**Severity**: this is a CONCERN, not FATAL, because (i) implementation smoke matches the correct formula, (ii) the propagated σ is computed from actual numeric unfolds, not from this formula, (iii) the formula appears only in the memo's *bound* derivation. But **the memo §3.3 summary table entry "±1.0 %" for Q²_es at y=0.5 is also slightly off** (should be 0.75 % at y=0.5, 1.0 % at y=0, growing to 1.5 % at y=−0.5 which is unphysical). At our typical y ∈ [0.1, 0.7], the per-event Q² shift is 0.65–0.95 %, not 1.0–1.25 %.

**Killing question**: does the implementation script actually use the (correct) Σ-method evaluation per event (with full numerical (1+δ)²/(1+δ·y) recompute, not a Taylor-truncated approximation)? Per the dispatch description it does — it recomputes y_es_sh, Q2e_sh, x_es_sh from shifted Σ_e — so the propagated σ is correct in principle, modulo A1/A2. The memo §3.5 algebra is a documentation bug, not a code bug.

**Required fix**: edit memo §3.5 paragraph "Correction to §3.1 above" and §3.3 table row for "Q²_es" to read `δ(2−y)` and `~0.75 % at y=0.5, 1 % at y=0`.

### B2. The piecewise η_e = 1.5 boundary is a placeholder; the H1 calibration boundary is θ-wheel-defined and may not correspond to η_e = 1.5 cleanly.

**Memo statement** (§5.2 script comment): `is_forward = eta_e > 1.5   # placeholder; the exact "forward" definition follows the LAr fiducial split`. Memo §9.5 and §10 item 4 explicitly flag this as needing verification against Andreev 2014 §6.

**Evidence**: Andreev 2024 §6.4 says "backward and central regions" (0.5 %) vs "forward region" (1 %), referring to the LAr wheel geometry. The H1 LAr forward wheel begins at θ ≈ 30° in the lab; the corresponding pseudorapidity is η ≈ 1.32 (using η = −ln(tan(15°))). η = 1.5 corresponds to θ ≈ 25.2°, which is *inside* the forward wheel. So the placeholder boundary is **conservatively biased**: events at 25° < θ < 30° (η between ~1.32 and ~1.5) are central-LAr in reality but get the *central* (smaller, 0.5 %) shift under the placeholder. They should get the *forward* (1 %) shift.

**Quantitative impact**: a single LAr-wheel boundary shift of |Δη| ~ 0.2 affects ~5–10 % of events in our sample (forward-LAr SIDIS, Q² > 150 GeV²). Under-shifting half of the forward-region events by half the magnitude (0.5 % instead of 1 %) under-quotes the forward-region contribution to the variation by ≈ 0.5 × 0.5 % = 0.25 % per event globally on forward events. The bin-by-bin under-shift is small (~10 % under-quote in forward-event-dominated bins) and bounded.

**Mechanism**: not bias in σ for central/backward-dominated bins; mild under-quote for forward-region-dominated bins (highest Q², highest x).

**Killing follow-up**: pull Andreev 2014 *EPJC* 74 (2014) 2814 §6 (or DESY-14-026) and locate the explicit θ-wheel boundary used for the LAr E-scale piecewise calibration. Replace `η_e > 1.5` with the θ_e-based cut (likely `θ_e < 30°` or similar). Re-run the shift script. The change will increase per-bin σ in forward-dominated bins by ≲ 10 %.

### B3. Same-side Q² gradient (38 % → 68 % across S0→S3) suggests the unfold-noise floor is the dominant effect at high Q², not the electron systematic itself.

Connecting A1 to the per-stratum numbers:
- S0 (lowest Q², highest stats): 38 % same-side. Most bins have a clean coherent up/down signature.
- S3 (highest Q², lowest stats): 67.5 % same-side. Most bins have lost sign coherence.

**Mechanism**: when statistical noise on the unfolded answer becomes comparable to the deterministic shift response, the random-noise component dominates and randomizes the up/down sign relative to the nominal. The published Andreev 2023 Table 2 Lepton(E) numbers in the high-Q² bin (2081–5000 GeV²) are 0.028 in absolute units, i.e. ~0.36 % on ⟨Nc⟩ — a tiny shift that is easily swamped by unfold noise in a stats-starved bin.

**Concrete diagnostic from the data**: the per-stratum Σ|σ|/Σ|N| are 0.40 %, 0.39 %, 8.48 %, 5.53 %. The 8.5 % in S2 is driven almost entirely by **2 bins out of 120** (top-2 bins contain 50 % of S2's Σ|σ|; top-9 contain 90 %). The two worst S2 bins are:
- bin 410: cell S2b, z=4 (pT=0), h−; nom=−1.30, up=−227.37, dn=4.83 ⇒ |δ|=116, frac=8935 % (this is the "max frac" in the headlines).
- bin 172: cell S2b, z=4 (pT=2), h+; nom=−42.5, up=1552, dn=−91.2 ⇒ |δ|=822, frac=1934 %.

In both bins the nominal is small or negative, and the up/down excursions are ~100–1500× larger than the nominal. These are bins where the unfold is producing **structurally meaningless point estimates** (negative yields, small magnitudes near the noise floor). The fractional sigma denominator |N_nom| is essentially noise, making the ratio meaningless — but the rank-1 cov contribution from these two bins is `δ_410² ≈ 13456` (entry C[410,410]) and `δ_172² ≈ 676000`, dominating the cov norm.

**Mechanism**: the cov has a few catastrophic-bin entries that dominate the rank-1 norm and propagate into every other bin's cross-correlation as ±|δ_410|·|δ_j| / sign. Any integrated quantity that touches these bins is contaminated.

**Required fix**:
- Mask or floor bins with |N_nom| below a statistical threshold (e.g. |N_nom| < 3·σ_stat) before constructing the rank-1 cov. Otherwise these two bins dominate the entire 480-bin cov.
- Quote σ on these bins as a separate "untrusted" category in the systematics table.

### B4. The Q2e ≡ Q2es trap is consistent within the systematic but inherits the underlying extractor bug.

**Issue (per dispatch)**: the v3p1 CSV's `Q2e` column is bit-identical to `Q2es` across 200k rows (verified upstream). The Q2_min=150 cut is on `Q2e` (claimed) but actually evaluates on Σ-method Q² values. The shifted CSV sets `Q2e_sh = Q2es_sh` to maintain consistency.

**Verdict**: this is internally consistent — the nominal cut is on Σ-method Q² and the shifted cut is also on Σ-method Q². The shift script is **not introducing** a new bias; it is faithfully reapplying the same cut definition. This is acceptable *for this systematic* (the trap cancels because nominal and shifted both use the same — albeit mislabeled — quantity).

However:
- If a downstream `extractor`-side bug fix later switches `Q2e` to actually mean electron-method Q², the **nominal** Q²-cut definition changes and *every* prior unfolded result becomes inconsistent. The current electron systematic would need to be re-run.
- The Andreev 2024 §6.4 prescription propagates the E-shift through the **electron-method** Q²_e (Q²_e = E_e'² sin²θ_e · 2 / (1 − cos θ_e_eq?)) for the *cut* but Σ-method Q²_es for the *binning*. If our binning and cut are both on Σ-method, we are slightly mismatched to the published convention. The bias is small (the Σ-method Q² is less sensitive to the electron E-scale than electron-method Q²) but real.

**Severity**: CONCERN. The systematic as reported is self-consistent. The extractor bug is a separate issue that the supervisor should treat as a "follow-up after this systematic is fixed" item.

**Killing question**: when the Q2e/Q2es extractor bug is fixed, will this systematic be re-derived? If yes, no action needed now. If no, the source should be marked "valid only under current extractor schema".

---

## C. Non-Blocking Checks Confirmed

### C1. Half-difference and sign conventions
- `delta_signed_per_bin == (unfolded_up − unfolded_down) / 2` to machine precision (480/480 bins).
- `sigma_per_bin == |delta_signed_per_bin|` to machine precision.
- This matches Andreev 2024 §6 sign convention (signed input, |·| for cov entry magnitude).

### C2. Cov is bit-identical to outer-product rank-1
- `np.allclose(cov, np.outer(δ, δ))`: True bit-for-bit.
- Second eigenvalue / first eigenvalue = 4.6e−17 (machine epsilon). Pure rank-1.
- `|corr_ij| ≡ 1.0` everywhere. The structure is what it claims to be.
- Severity: this is **what was specified**. The fatal issue (A1, A2) is that the specification itself is wrong for the underlying noise structure, not that the implementation departs from the spec.

### C3. Data-marginal shift is the correct kinematic choice for an electron-scale variation
- For an electron *detector*-scale uncertainty, the principled propagation is: shift the detector-level data (you are uncertain about how the detector measured the electron in the **data**), shift the MC-reco *consistently* (so the migration matrix reflects the same uncertainty), keep MC-truth unchanged (the truth is detector-independent).
- The implementation shifts both data and MC-reco. No double-application is in evidence from the numeric outputs.
- This is *different* from tracking systematics where the data is unchanged because the detector-level data measurement is anchored by a calibration we trust *for the cut definition* — for electron energy, the calibration is exactly what we are varying. Correct.

### C4. Empz update prescription
- `Empz_sh = Empz_nom − Σ_e_nom + Σ_e_sh`, with Σ_had untouched. Σ_had is the HFS-only E−pz sum and is by construction electron-independent. The update is the right kinematic re-bin.
- Verifiable from the propagated y_es_sh / Q2es_sh values once the per-event CSV is exported. The headline numbers (median frac 2 %, comparable to memo §8.1 priors of 0.7–1 %) are consistent with the right propagation modulo the noise contamination identified in A1.

### C5. Acceptance flip (events failing nominal cuts but passing shifted)
- Memo §5.2 cons: "few per mille" for 0.5 % shift, bounded.
- Numeric evidence: the Σ|σ|/Σ|N| in S0 and S1 (the high-stats strata) is 0.40 % and 0.39 %, comparable to the published Andreev 2023 bulk priors (0.5–1 %). If the acceptance-flip bias were large, S0 (lowest Q², highest stats) would show systematic offsets between up and dn integrated yields; it does not. This caveat is **bounded and small** in our numeric output. Confirmed.

### C6. Headlines arithmetic (median, p84, max) re-derive
- Median fractional σ: 2.06 % (matches).
- p84: 18.0 % (matches).
- Max: 8935 % at bin 410, cell S2b, z=4, pT=0, h− (matches: |N_nom|=1.30 is essentially numerical noise; this is bin 410 in the source).
- n_same_side: 244/480 (matches).
- n_negative_nominal: 33 (matches).

---

## D. Missing Evidence

### D1. Noise-floor closure test
- **Required to defend A1 quantitatively**: run two nominal unfolds with different RNG seeds (or different stat bootstrap samples of the same data) and compute the same `(N_a − N_b)/2` per bin. If that "zero-shift δ" has a per-bin distribution comparable to the recorded same-side `δ`, then ~244 bins are noise-dominated. If it is much smaller (say <30 % of recorded same-side |δ|), then the same-side response is real and the rank-1 cov is the right structure.
- This test has not been done. The supervisor should require it before signing off on this systematic.

### D2. Andreev 2014 θ-wheel boundary for piecewise E-scale
- Memo §10 item 4 explicitly flagged. Not done. Required for B2.

### D3. Cross-component covariance with Lepton(θ) and Lepton(φ)
- The published H1 prescription uses three independent components (E, θ, φ). The supervisor brief and source JSON are only the E component. The current source cov_electron_energy.npz cannot stand alone as "the scattered-electron systematic"; it must be added in quadrature with Lepton(θ) and Lepton(φ) sources. **If those sources are not also produced and combined, the analysis is missing 1.5–3 % of σ_electron in φ_h-binned observables** (memo §8.1 std-deviation table).

### D4. Closure spot-check against Option A
- Memo §7.B defined the closure protocol; per the source JSON this was not run (no `closure_vs_option_a` field). Without it, the analytic-CSV Option B implementation cannot be defended as "matches the published H1 prescription".

### D5. Background MC propagation
- Memo §10 item 7. The published prescription propagates the electron systematic through both signal MC and background subtraction. The source JSON does not state whether backgrounds (Pythia γp, DVCS, Compton, Grape, low-Q² Django, CC Django) were re-shifted. If backgrounds were not shifted, the systematic is incomplete on bins where background contamination is non-trivial (likely the high-y, low-Q² edge of S0).

---

## E. Direct answers to the dispatch's 8 known issues

| # | Issue | Verdict |
|---|---|---|
| 1 | n_same_side=244/480: TUnfold τ damping? | **No — it is the unfold noise floor.** Asymmetry metric of co-movement is exactly 1.000 in same-side bins (full co-movement); regularization shrinkage would partially damp both ends *symmetrically*, preserving sign. The observed pattern is statistical (gradient S0→S3 tracks stats). **The rank-1 cov on a noise floor is structurally wrong** (A1, A2). This is the dispatch's "biggest physics concern" — the verdict is FATAL. |
| 2 | Memo §3.5 δQ²/Q² = δ(2±y) | **Memo is wrong** (sign error; correct is `δ(2−y)`, confirmed by independent derivation and by smoke). Cosmetic-to-mildly-substantive: the propagated σ uses the numeric Σ-method recompute (correct), but documented expectations in the memo are biased high by ~25 % at y=0.5. Memo §3.3 and §3.5 must be edited. (C2/B1) |
| 3 | Piecewise η_e = 1.5 boundary | **Placeholder, unverified**. Likely under-shifts the forward region (true LAr forward wheel starts at θ ≈ 30° ⇒ η ≈ 1.32, not 1.5). Mild under-quote of forward σ. Resolve by pulling Andreev 2014 §6. (B2) |
| 4 | Data marginal shifts per variation | **Correct choice for electron-scale variation.** Different from tracking systematics by design — the electron calibration is what we are varying, so detector-level data must be re-calibrated alongside MC-reco. Implementation does this; no double-application. (C3) |
| 5 | Acceptance flip "events fail-nominal pass-shifted not in CSV" | **Bounded and small per memo §5.2; numerically consistent with S0/S1 priors.** The dispatch's "few per mille" assertion is consistent with what is observed (0.39–0.40 % in S0/S1). Not a blocker but should be noted as a residual Option-B-vs-Option-A bias source. (C5) |
| 6 | S2 Σ\|σ\|/Σ\|N\| = 8.48 % on integrated yield | **Driven by 2 catastrophic bins out of 120**: top-2 contain 50 % of Σ\|σ\|, top-9 contain 90 %. Both worst bins have \|N_nom\| < 50 (one is −1.30, near noise). **Mask bins with \|N_nom\| below a stats threshold before quoting integrated σ**. The bulk of S2 has σ comparable to the 1–1.5 % literature prior. (B3) |
| 7 | Q2e ≡ Q2es trap | **Self-consistent within this systematic** (cut is on Σ-method Q² both nominal and shifted; no new bias introduced). Inherits the underlying extractor schema bug; if the extractor is fixed, this source must be re-run. (B4) |
| 8 | Rank-1 cov with n_same_side=244 | **Structurally wrong** for the reason described in A1/A2. The same answer as #1. |

---

## F. Recommended Next Check (mandatory before this source is used)

**The single highest-priority check**: noise-floor closure test (D1).

Procedure:
1. Take the nominal v3p1 CSV. Split into two statistically independent halves (e.g. by event-ID parity, or with a bootstrap).
2. Unfold each half independently with τ=0.01.
3. Compute `δ_noise(i) = (N_unf_a(i) − N_unf_b(i)) / 2` per bin, rescaled by √2 to match the same-statistical-precision target as the up/down comparison.
4. Compare the distribution of |δ_noise(i)| to the recorded same-side |δ(i)|. Compute the same-side fraction within `|δ_noise|` distribution.
5. Decision:
   - If `|δ_noise|` ≳ recorded same-side |δ| (i.e. noise envelope subsumes the same-side response): **the same-side bins are noise; the present source is fatally biased on signed observables. Re-derive using an up-only signed convention or mask same-side bins as "below systematic floor" and assign σ from a literature prior.**
   - If `|δ_noise|` ≪ recorded same-side |δ|: **the same-side bins represent a real second-order non-linear electron-scale response; the rank-1 cov is acceptable as an *upper bound* on σ for non-asymmetry observables, but is still incorrect for charge-asymmetry observables (no cancellation, see A2).**

Secondary follow-ups (in priority order):
- **F2**: re-derive Q2e cut on actual electron-method Q² and confirm the bin-edge effect is < 0.1 % per bin (validates the B4 self-consistency caveat).
- **F3**: replace `η_e > 1.5` with the θ-wheel boundary from Andreev 2014 §6 and re-run. Expected effect: forward bins σ up by ~10 %.
- **F4**: produce the companion sources `source_electron_theta.json` and `source_electron_phi.json` per the published 3-component decomposition; combine in quadrature for the final electron envelope.
- **F5**: edit memo §3.5 correction paragraph and §3.3 table row 1 to read `δ(2−y)` consistent with smoke and independent derivation.
- **F6**: produce a closure spot-check against a single Option A reference variation (memo §7.B) at the +0.5 % central E-shift to confirm the analytic CSV pipeline reproduces the framework-level unfold within 30 % per bin in bulk.

---

## Summary

The source artifact `cov_electron_energy.npz` faithfully implements the specified rank-1 cov from half-differences of the (up, dn) unfolds. The specification is **wrong for the underlying statistical regime** of the present 480-bin grid: 244 bins (51 %) have an unfold-noise floor comparable to or larger than the electron-scale response, and the rank-1 outer-product cov forces those noise bins into 100 % correlation with the 236 signal bins. This (a) randomizes σ on signed observables (asymmetries especially), (b) prohibits the physical charge-cancellation that should reduce σ_electron(A_h) to ≤ 0.1 % per memo §4 (the cov as constructed permits ≈ 1 % with no cancellation), and (c) lets 2 catastrophic noise-dominated bins drive the S2 stratum Σ|σ| to 8.5 %.

A documentation bug (memo §3.5 sign error) and a placeholder η-boundary (memo §5.2) are unrelated cosmetic-to-mild concerns.

**Until the noise-floor closure (F1) is run, this source is FATAL for any signed-observable downstream use (A_h, integrated cross-sections, charge-difference multiplicities) and CONCERN-grade for per-bin σ on h+ or h− yields independently.**
