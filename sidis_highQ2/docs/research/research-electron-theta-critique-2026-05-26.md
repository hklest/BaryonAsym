# Critic Review: `electron_theta` 4D Systematic (HQ-v5g-Bprime, 480 bins)

**Date**: 2026-05-26
**Reviewer role**: `h1-analysis-critic` (read-only, adversarial)
**Source ID under review**: `electron_theta`
**Files reviewed**:
- `H1/sidis_highQ2/data/systematics_sources/source_electron_theta.json`
- `H1/sidis_highQ2/data/systematics_sources/cov_electron_theta.npz`
- `H1/sidis_highQ2/docs/research/research-electron-systematics-2026-05-25.md` (memo, §§2.1, 2.5, 3.1, 3.2, 3.3, 4, 8)

---

## VERDICT: **CONCERN** (bordering on FATAL for tail bins; SOUND for S0/S1 bulk only)

The intended Andreev-2024 §6.1 prescription (±1 mrad polar shift, full re-unfold, signed-difference convention) is implemented faithfully at the JSON level and the cov matches `outer(δ,δ)` to machine precision. Bulk magnitudes (S0/S1) sit within the published H1 priors (~per-mille, comparable to Andreev 2023 Tables 2–3 for ⟨Nc⟩-like observables). However, three findings rise above "cosmetic":

1. **The shift is grossly non-antisymmetric.** Median `|(up+down)/2|/σ ≈ 1.0–1.5` per stratum, with S2 reaching 300×. A linear-response ±1 mrad shift around the nominal should give `|(up+down)/2|/σ ≪ 1`. The unfolder is delivering an essentially monotone bias, not a symmetric bracket — meaning the "signed half-difference" σ is not what it is being labelled as.
2. **S2 σ-budget is dominated by ~5 catastrophic bins (83.4 % of Σ|σ|).** This is not a smooth cot(θ_e) growth; it is a small set of pathological negative- or near-zero-nominal cells driving the entire stratum-level fractional metric. The §8.4 memo prior of "≤1.5 % tail" is broken at the bin level by a factor of 100–500, and the rank-1 covariance lets these pathologies propagate fully correlated to every other bin.
3. **The rank-1 / fully-correlated cov structure misrepresents the bin–bin correlation.** 263/480 (55 %) of bins are same-side; this is a TUnfold-damping signature identical to the one flagged for `electron_energy`. The implied claim "one underlying nuisance, one direction" is contradicted by both the same-side fraction and the linearity-ratio diagnostic.

The S0/S1 bulk numbers individually are physically plausible (median 0.21–0.31 %, consistent with the memo's prior). But the artifact is being **stored as a single 480×480 fully-correlated covariance** with a long catastrophic tail. Downstream consumers will either (a) get a defensibly small electron-θ contribution if they truncate or pre-clean tail bins, or (b) get a totally wrong correlated-error budget if they pipe `cov_electron_theta.npz` through as-is. The current artifact is unsafe to ship without explicit downstream guidance.

---

## Material reviewed

- `source_electron_theta.json`: 480-bin nominal/up/down/δ/σ vectors, claimed convention "Andreev 2024 §6.1, ±1 mrad on θ_e, fully correlated rank-1".
- `cov_electron_theta.npz`: keys `cov, corr, sigma_per_bin, delta_signed_per_bin, nominal_unfolded, unfolded_up, unfolded_down, bin_meta, index_convention, cell_labels`.
- Verified: `σ = |δ| = |(up − down)/2|` exactly. `cov = δ δᵀ` to floating-point zero (rank-1 confirmed; effective rank = 1 with one eigenvalue 2.08e8, all others < 3e-9).
- Memo §2.1 (Andreev 2024 §6.1 quotation), §3.1 (eΣ Q² formula `ΔQ²/Q² = 2cot(θ_e)δθ + δy/(1−y)`), §3.2 (boost shifts on z, pT_h, φ_h), §3.3 (sensitivity table), §4 (charge-symmetry argument), §8.4 (per-bin prior).

---

## Fatal Issues

### F1. Linearity assumption is violated in 55 % of bins and median-broken in every stratum.

- **Claim under test.** The implementation propagates θ_e → θ_e ± 1 mrad and stores σ = |up − down|/2 with a rank-1 fully-correlated covariance. This procedure is only physically meaningful if the unfolded yield responds approximately linearly in the shift, i.e. if `up + down − 2·nom ≈ 0` to second order.
- **Evidence (computed from `cov_electron_theta.npz`).**
  | Stratum | median `\|(up+down)/2\|` | median `\|asym\|/σ` | p84 ratio | max ratio |
  |---|---:|---:|---:|---:|
  | S0 | 8.9 | 1.03 | 3.9 | 66.8 |
  | S1 | 6.4 | 1.14 | 4.4 | 46.4 |
  | S2 | 9.2 | 1.52 | 10.8 | **300.0** |
  | S3 | 6.7 | 0.86 | 2.7 | 160.6 |

  For a faithfully antisymmetric ±1 mrad shift on a linear response, the ratio `|(up+down)/2|/σ` should be small (≪1, ideally O(δθ) ≈ 0.001 of the response size). A median of ~1 across every stratum means the systematic is monotone or near-monotone in the shift direction in half the bins, not antisymmetric.
- **Mechanism.** This is the TUnfold-damping fingerprint flagged for `electron_energy`: at τ=0.01 with a 4D 480-bin response that has many near-zero or negative nominal cells, the regularizer drags both `up` and `down` toward the same biased point. The result is that the "half-difference" no longer estimates the linear-response Jacobian — it estimates 0 + a regularization bias. Whatever sign that bias has becomes the "σ", and downstream both up and down agree on it.
- **Killing follow-up.** Two options. (i) Switch to an asymmetric error specification: store `up − nom` and `down − nom` separately; let downstream symmetrize per its own convention (RooStats `asymError`, eigenvalue decomposition of `[up; down]`, etc.). (ii) Re-unfold with τ → 0 (or with the bias-correction Jacobian explicitly applied) and re-test the antisymmetry. Either way: the current σ does NOT estimate `|∂N/∂θ_e| · 1mrad`; it estimates a regularized scalar that has lost the sign structure. Do not ship the rank-1 cov as-is.

### F2. S2/S3 tail bins exceed the memo prior by factors of 100–500 and are concentrated in a handful of cells.

- **Claim under test.** Memo §8.4: "Tail bins (Q² > 2000 GeV², z > 0.7, pT_h > 4 GeV): σ_electron / yield ≈ 1.5 %." Memo §3.3: "ΔQ²/Q² = 2cot(θ_e)δθ + δy/(1−y) … smallest at θ_e = 90°, grows toward forward." This predicts a smooth, modest gradient.
- **Evidence.**
  - Top 5 bins in S2 carry **83.4 %** of Σ|σ_S2| = 29 444 / 35 309; in S3 top-5 = **71.4 %**. The stratum-level Σ|σ|/Σ|N| of 3.74 % (S2) and 2.63 % (S3) is not "physics gradient" — it is a few pathological cells.
  - Worst cases:
    - idx=410 (S2b, ch=h⁻, z-bin 4, pT-bin 0): nom = −1.30, up=4.24, down=24.13, σ = 9.94 → **f = 765 %**.
    - idx=479 (S3b, ch=h⁻, z-bin 5, pT-bin 4): nom = −6.79, up=27.85, down=−80.06, σ = 53.96 → **f = 795 %**.
    - idx=176 (S2b, ch=h⁺, z-bin 5, pT-bin 1): nom = 5237, up=−6639, down=−94, σ = 3272 → both up and down go negative; `(up+down)/2 − nom = −8603`, a **−164 % monotone drag** with σ quoted as 62.5 % — same-side, monotone, far from antisymmetric.
  - Crucially, **18 of the 33 negative-nominal bins are same-side**: the unfolder is bouncing negative-nominal cells around in a way that has nothing to do with cot(θ_e).
- **Mechanism.** Two compounding effects.
  - (a) The 33 negative-nominal-yield bins are a pre-existing pathology (memo confirms this). Dividing |σ| by |nom| ≈ 0 produces unphysical fractional uncertainties. Memo §8.4 prior was implicitly computed *excluding* pathological cells; the artifact does not.
  - (b) The Andreev 2024 cot(θ_e) prediction is a per-event Q²-resolution argument; it does NOT predict a 100 %-level shift on a *yield* in a (Q², x, z, pT_h) cell. Forward θ_e at fixed Q² is the same as high y at fixed Q², which is the low-statistics corner — TUnfold + bin sparsity, not physics, drives the 100 %-level shifts.
- **Killing follow-up.** Three actions, all required:
  1. Define a "physics-valid bin mask" (likely `nom > 0` AND `nom > N_min` for some statistical floor, e.g. N_min = 50 unfolded events) and report stratum-level summaries on that mask only. The current Σ|σ|/Σ|N| metric in the brief mixes signal cells with regularization noise.
  2. For the masked bins, redo the stratum gradient — if S2/S3 still exceeds S0/S1 by an order of magnitude, that's news worth investigating. My read: most of the 50× S1→S2 jump will evaporate once the top 5 S2 bins are masked.
  3. For the cells that fail the mask: do NOT propagate them with a rank-1 fully-correlated entry into the cov. They contaminate every other bin's effective error through `cov = δδᵀ`.

### F3. Rank-1 fully-correlated covariance is empirically false at the 55 % level.

- **Claim under test.** `correlation_structure: "fully_correlated_across_bins"`, implemented as `cov = outer(δ, δ)`. This rank-1 assertion is physically motivated as "one upstream nuisance (the 1 mrad alignment) → one coherent direction in 480-D yield space".
- **Evidence.**
  - 263/480 (54.8 %) bins are same-side (sign(up−nom) = sign(down−nom)). For a coherent rank-1 ±1 mrad shift, up and down should be on opposite sides of nominal in nearly every bin; same-side fraction should be near 0, not 55 %.
  - Same-side breakdown by Q² stratum: S0 51.7 %, S1 57.5 %, S2 **65.8 %**, S3 44.2 %. Worst in S2 — same stratum where the catastrophic bins live. This is not random.
  - The linearity ratio in F1 implies that "up direction" and "down direction" in 480-D yield space are not antiparallel — they have a large common component. A correct cov would have rank ≥ 2: one direction = linear-response Jacobian, one direction = quadratic bias / regularization drag. The rank-1 storage projects the bias direction onto the Jacobian direction and loses information.
- **Mechanism.** If you stored the *actual* (up − nom, down − nom) pair per bin and computed the cov as the proper symmetric construction (e.g. `cov_ij = ½ [(up_i − nom_i)(up_j − nom_j) + (down_i − nom_i)(down_j − nom_j)]`), you would get rank 2, with the second direction reflecting the common bias. The rank-1 cov hides that direction and reports only the antisymmetric projection.
- **Killing follow-up.** Build the correct rank-2 cov from `unfolded_up` and `unfolded_down` directly, diagonalize it, and report the top two eigenvalues per stratum. If λ₂/λ₁ ≪ 1 the rank-1 storage is justified; my prediction from the linearity ratio is λ₂/λ₁ ≳ 1 in S2 specifically, which is a fatal indictment of the storage format.

---

## Major Concerns

### M1. The `|p_e| fixed, scatEE fixed` shift implementation has a hidden consistency requirement.

- **Claim under test (brief).** "θ_e → θ_e ± 0.001 rad. |p_e| and φ_e held fixed; recompute (scatEX, scatEY, scatEZ) = |p_e| · (sin θ · cos φ, sin θ · sin φ, cos θ). scatEE unchanged (rotation conserves energy)."
- **Why this needs verification.** For massless electron, `scatEE` and `|p_e| = sqrt(scatEX² + scatEY² + scatEZ²)` are mathematically identical. The shift code uses TWO independent quantities: (a) `|p_e|` to set the magnitude of the new 3-vector, (b) `scatEE` unchanged. If the implementation derives `|p_e|` from `sqrt(scatEX² + scatEY² + scatEZ²)` (which equals `scatEE` to within float rounding only if the electron was treated as massless at extraction time), the up/down shifts conserve `scatEE` exactly. If `|p_e|` is derived from `scatEE` directly, the implementation is consistent by construction. If the extractor stored `scatEE` with the physical electron mass (i.e. `scatEE = sqrt(p² + m_e²)`), then `|p_e| ≠ scatEE` and there is a systematic ~511 keV / E_e energy offset (~5×10⁻⁵ at E_e = 11 GeV) on Σ_e via cos θ_e — well below 1 mrad effect, but check.
- **Evidence not in package.** The script `electron_systematic_csv_rescale.py` referenced in memo §7.A Phase 1 — I cannot verify the exact line that computes `|p_e|`. The memo §5.2 pseudocode uses `p_e = np.sqrt(df.scatEX**2 + df.scatEY**2 + df.scatEZ**2)` for the polar-shift branch and explicitly notes `scatEE_shift = scatEE` (unchanged). If implemented as written, `|p_e|` and `scatEE` will be bit-identical only when the extractor wrote `scatEE = sqrt(scatEX² + scatEY² + scatEZ²)`. Otherwise: latent inconsistency at the per-event level, biasing every shifted Σ_e by `(scatEE − |p_e|)·(1−cos θ_e)`.
- **Follow-up.** Pull the actual `electron_systematic_csv_rescale.py` from NAF and confirm. One-line check: `(df.scatEE − np.sqrt(df.scatEX**2 + df.scatEY**2 + df.scatEZ**2)).abs().max()` on the input CSV. If > 1e-3 GeV, the shift is internally inconsistent and the §3.1 cot(θ_e) derivation must be redone.

### M2. φ-rotation is correctly omitted from this source, BUT the assumed independence between Lepton(θ) and Lepton(φ) is unverified against Andreev 2024.

- **Claim under test (brief Q4).** Memo §2.1 quotes Andreev 2024 §6.1: "The HFS and electron polar angle uncertainties are considered separately, and each is passed through the unfolding procedure." Andreev 2024 §6.4 + Andreev 2023 §4 list Lepton(E) and Lepton(ϕ) as separate columns. The brief asserts: θ ↔ tracker alignment 1 mrad and φ ↔ separate 1 mrad are treated independently in H1 convention.
- **Evidence in memo.** Memo §2.1 and §2.5 are internally consistent: the H1 high-Q² convention varies Lepton(θ) and Lepton(ϕ) independently (independent ±1 mrad in θ; independent ±1 mrad in φ; one rotation per source). Andreev 2024 §6.1 explicitly separates HFS-polar and electron-polar. Andreev 2023 Tables 2–3 list `Lepton(ϕ)` as a separate column. So the φ-only rotation is correct under the published convention.
- **Subtle risk.** The published Andreev 2024 §6.1 wording "passed through the unfolding procedure" might apply *one shift to both* HFS-polar and electron-polar in the same variation (coherent rotation of the entire alignment), which would give a smaller combined uncertainty than treating them independently and summing in quadrature. This is the brief's known-issue #6 — see below.
- **Follow-up.** When the companion `HFS_polar` source is built, verify that Andreev 2024 §6.1 specifies independent vs. joint propagation. If joint, the current decomposition double-counts and overestimates the polar-alignment contribution by a factor of √2.

### M3. Memo §3.2 prediction `Δφ_h ≲ 1 mrad ≈ 0.06°` is far below the observed bin migration in S2/S3.

- **Claim under test.** Memo §3.2 predicts that a θ_e shift causes ΔpT_h/pT_h ≲ 0.1 % and Δφ_h ≲ 0.06° per track, with corresponding sub-percent yield shifts.
- **Evidence.** Stratum-summed Σ|σ|/Σ|N| of 3.74 % (S2) is incompatible with a sub-percent prediction even after excluding the catastrophic tail (M2 estimate: bulk-only S2 median ≈ 0.84 %, still 4× the memo prior of 0.21 % for S0). The §3.2 derivation may be missing the eΣ-form `δy/(1−y)` amplification at high y (memo §3.5 noted this for the energy-scale case but did not re-derive it for the polar case).
- **Follow-up.** Re-derive ΔQ²_es/Q²_es with the corrected eΣ formula `y_es = Σ_had/(Σ_had + E_e'(1−cos θ_e))` (memo §3.5 confirms this is the Andreev 2023 convention). For δθ = 1 mrad, δΣ_e = E_e'·sin(θ_e)·δθ. δy_es = −y_es·(1−y_es)·δΣ_e/Σ_e. The `δy/(1−y)` term in δQ²/Q² then becomes `−y·δΣ_e/Σ_e = −y·cot(θ_e/2)·δθ`. At y=0.5, θ_e=30°, δθ=1 mrad: contribution is 0.5·3.73·0.001 ≈ 0.19 %, in addition to the `2cot(θ_e)·δθ` direct term of 0.35 %. Total still well under 1 % per event — so the §3.2 prediction holds at the per-event level, but the bin-yield level can be 5–10× larger due to migration amplification when bin boundaries align with the shift direction. This is physically possible but not in the memo.

### M4. The 16-yield-unit median up/down asymmetry stated in the brief understates the problem.

- **Claim under test (brief).** "median up/down asymmetry: 16 yield units".
- **Evidence.** My recompute: median `|(up+down)/2|` across all 480 bins = 7.78 yield units, p84 = 47.6, max = 22 857 (idx=176, the catastrophic S2b cell). The brief's 16 is likely the mean, not the median.
- More importantly, the brief presents 16 yield units as if it were small; relative to nominal it is small only in bulk bins. In the catastrophic S2 cells the asymmetry is the *dominant* effect. The brief should report `|asym| / σ` not `|asym|` in absolute units.
- **Follow-up.** Re-quote: median `|(up+down−2nom)| / σ` = 1.03 (S0), 1.14 (S1), 1.52 (S2), 0.86 (S3). This is the headline linearity-failure number, and it should be in the brief.

### M5. Comparison with Andreev 2024 §6.1's "typically ~1 %" headline is favorable in bulk and unfavorable in tail — but the comparison is to a fundamentally different observable.

- **Claim under test.** Memo §2.1 cites Andreev 2024 §6.1 "typically ~1 %" for the combined HFS-polar + electron-polar effect on final distributions of groomed event shapes.
- **Evidence.** Andreev 2024 groomed event shapes are 1D unfolded observables with O(10) bins, large per-bin populations, no z×pT_h refinement. Our 4D 480-bin Scheme B' grid has O(50–1000) populated bins, many sparse. Comparing a 480-bin sparse 4D σ to a 10-bin populated 1D σ is not apples-to-apples; the Andreev 2024 "~1 %" is a *floor*, not a ceiling, when extrapolated to finer binning.
- **Follow-up.** State explicitly in any downstream documentation that the memo prior is a 1D ⟨observable⟩ uncertainty and the 4D yield uncertainty is expected to be larger per bin by factors of √(N_4D/N_1D) ≈ 7. The bulk S0 median of 0.21 % per bin (well below 1 %) is actually *better* than the Andreev 2024 floor suggests, which is suspicious — possibly indicating the rank-1 storage is artificially small in bulk because the regularization drags up and down together (F1).

---

## Non-Blocking Checks Confirmed

- **C1. JSON ↔ NPZ consistency.** `delta_signed_per_bin = unfolded_up − unfolded_down)/2` exactly (verified). `sigma_per_bin = |delta|` exactly. `cov = outer(delta, delta)` to floating-point zero. Storage is internally consistent.
- **C2. Bin indexing convention.** `idx = charge·240 + cell·30 + z·5 + pT` matches the JSON declaration and the `bin_meta` structured array. cell_labels = [S0a, S0b, S1a, S1b, S2a, S2b, S3a, S3b].
- **C3. Sign convention.** δ = (up − down)/2 matches Andreev 2024 §6 ("signed difference between nominal and shift-upward") modulo a factor of 2; the half-difference is a defensible H1 convention. Worth noting in the artifact metadata.
- **C4. n_negative_nominal_bins = 33** matches the brief and the memo (pre-existing pathology, not introduced by this source).
- **C5. Memo §2.1 vs. Andreev 2024 §6.1.** Verbatim quotation in memo matches the published H1 prescription. The convention is canonical.
- **C6. Memo §3.3 sensitivity table prediction at θ_e=90°.** `2·cot(90°) = 0` for the direct Q² term; correct. Bulk S0 median of 0.21 % is consistent with this prediction (central LAr, θ_e ≈ 90°, residual eΣ y-term dominates).
- **C7. Charge-symmetric cancellation in A_h (memo §4).** The mechanism is correct: a coherent θ_e rotation does not differentiate h⁺ from h⁻ at first order; A_h cancellation arises from common-mode subtraction. Bound of ≤0.1 % on A_h is plausible — but needs explicit re-derivation when h⁺/h⁻ are stored separately as in this artifact (charge_idx is part of the bin index here). The current source ships absolute yields, not A_h; downstream A_h propagation must use the full `cov` and the correct linear combination, not symmetrize by hand.

---

## Missing Evidence

- **E1. The shift implementation script.** Cannot verify M1 (`|p_e|` vs. `scatEE` bit-consistency) without reading `electron_systematic_csv_rescale.py` or equivalent. **Request**: dump the script.
- **E2. The unfolder configuration log.** TUnfold-4D at τ=0.01 is asserted in the JSON. The same-side fraction and linearity ratio strongly suggest τ-damping; without the actual TUnfold output (matrix condition number, L-curve, χ²/dof per variation) I cannot prove vs. merely suspect this. **Request**: τ-scan with σ measured at τ ∈ {0, 0.001, 0.01, 0.1}; show the linearity ratio as a function of τ.
- **E3. Per-bin (Q², x, z, pT_h, y, θ_e) coordinates.** The artifact stores bin indices, not the physical (Q², x, z, pT_h) edges. Without the edges I cannot map the catastrophic bins to specific θ_e values to test the cot(θ_e) hypothesis directly. **Request**: store the bin-edge tuple per index in the NPZ (or reference it via the binning name).
- **E4. The S2b cell definitions.** cell_labels show 8 cells but only 4 Q² strata. From context "S0a/S0b" appears to split each Q² stratum by x (low-x / high-x). Need confirmation: which is S2b, low-x or high-x? The catastrophic bins concentrate in S2b — if low-x, the sparsity hypothesis is supported; if high-x, the cot(θ_e) hypothesis gains ground.
- **E5. The companion `electron_energy` and `electron_phi` sources.** The same diagnostics (linearity ratio, same-side fraction, rank-2 cov) should be run on those to confirm this is a generic TUnfold-damping pattern, not specific to θ.
- **E6. Andreev 2024 §6.1 joint-vs-independent propagation for HFS-θ and electron-θ.** Memo §2.1 quote is ambiguous: "considered separately, and each is passed through the unfolding procedure" — but it's unclear if "separately" means "two independent shifts" or "two contributions extracted from one joint shift". This is the brief's known-issue #6 and the M2 above. **Request**: pull the Andreev 2024 ar5iv §6.1 text and accompanying figures to verify the variation procedure.

---

## Stratum-resolved findings (answering brief questions 1, 5, 7)

**Same-side bin distribution by Q²-stratum** (brief question 5):

| Stratum | same-side / total | fraction |
|---|---:|---:|
| S0 [150,300) | 62/120 | 51.7 % |
| S1 [300,700) | 69/120 | 57.5 % |
| **S2 [700,1500)** | **79/120** | **65.8 %** |
| S3 [1500,20000) | 53/120 | 44.2 % |

NOT evenly distributed. S2 is the worst, mirroring its catastrophic σ inflation. The τ-damping bias is **stratum-correlated**, not uniform. This bins ANY rank-1 cov to be wrong.

**Q²-stratum gradient — real physics vs. catastrophic cells** (brief question 7):

| Stratum | All bins p84 | Bulk-only (\|nom\|≥10) p84 | Top-5 / Σ\|σ\| |
|---|---:|---:|---:|
| S0 | 1.01 % | 1.01 % | 68.0 % |
| S1 | 2.82 % | 2.82 % | 36.4 % |
| **S2** | **9.20 %** | **9.13 %** | **83.4 %** |
| S3 | 6.98 % | 6.69 % | 71.4 % |

The S1→S2 jump in Σ|σ|/Σ|N| from 0.07 % to 3.74 % (50×) is **dominated by ~5 bins** (83.4 % of stratum |σ| budget). The cot(θ_e) prediction CANNOT explain a 50× stratum jump — at θ_e crossing from 60° to 30° (the physically reasonable range from low-Q² to high-Q² bulk), `2cot(θ_e)` goes from 1.15 to 3.46, a factor of 3, not 50. The 50× is a sparsity/regularization artifact.

**Memo §8.4 prior comparison** (brief question 1): The memo prior of "≤1.5 % tail" is broken at the bin level. Bulk S0 (0.21 %) is consistent. S1–S3 medians (0.31, 0.85, 1.96 %) exceed the bulk prior of 0.7 % and approach/exceed the tail prior of 1.5 %. Half of this is real physics (cot(θ_e) plus y-amplification); the other half is regularization noise. **It is not OK to attribute the 50× S1→S2 jump to "physically expected cot-θ dependence growing with Q²"** — that's a factor of 3, not 50.

---

## Recommended Next Check

**Build a rank-2 covariance directly from `unfolded_up` and `unfolded_down`** and compare to the stored rank-1. Specifically:

```python
import numpy as np
d = np.load('cov_electron_theta.npz')
up_dev  = d['unfolded_up']   - d['nominal_unfolded']
down_dev = d['unfolded_down'] - d['nominal_unfolded']
# Symmetric rank-2 cov:
cov_rk2 = 0.5 * (np.outer(up_dev, up_dev) + np.outer(down_dev, down_dev))
# Diagonalize:
w, _ = np.linalg.eigh(cov_rk2)
print(f"Top 2 eigs: {w[-1]:.3e}, {w[-2]:.3e}, ratio λ2/λ1 = {w[-2]/w[-1]:.3f}")
# Compare to stored rank-1:
rank1_sigma = np.sqrt(np.diag(d['cov']))
rank2_sigma = np.sqrt(np.diag(cov_rk2))
print(f"Per-bin σ_rank1 vs σ_rank2: median ratio = "
      f"{np.median(rank2_sigma/(rank1_sigma+1e-30)):.3f}")
# Stratum breakdown:
cell_idx = d['bin_meta']['cell_idx']
for s, cells in enumerate([(0,1),(2,3),(4,5),(6,7)]):
    m = np.isin(cell_idx, cells)
    print(f"S{s}: σ_rk2/σ_rk1 median = "
          f"{np.median(rank2_sigma[m]/(rank1_sigma[m]+1e-30)):.3f}")
```

If λ₂/λ₁ ≳ 0.1, the rank-1 storage loses ≳10 % of the variance — minimum requirement to switch to asymmetric error or rank-K storage. If the per-stratum σ_rank2/σ_rank1 median is ≳1.3 in S2 specifically, the current artifact underestimates the S2 systematic by ≳30 % even *after* the rank-1 inflation from the catastrophic cells. Either outcome forces a re-spec of the storage format before this source is admitted to the v3 systematics table.

**Secondary check**: τ-scan at τ ∈ {0, 0.001, 0.01, 0.1} on the unfolder; report `|asym|/σ` per stratum. If the ratio collapses to ≪1 at τ=0, the F1 verdict is confirmed: the current artifact is mostly storing regularization bias, not the physics shift.

---

*End of critique.*
