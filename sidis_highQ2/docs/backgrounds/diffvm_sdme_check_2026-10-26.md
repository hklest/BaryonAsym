# DIFFVM ρ⁰ decay angles vs measured SDMEs, and the decay-model systematic on the subtraction (2026-10-26)

**Scope.** This study checks whether the helicity structure of the DIFFVM elastic ρ⁰ MC (reqIDs 8445–8448) matches the measured spin-density matrix elements (SDMEs) at our Q² > 150 GeV². The MC is the one subtracted with the data-tuned scale s = 0.08 ± 0.15 (see `diffvm_datatuned_subtraction_2026-10-26.md`). The study then quantifies what a decay-angle reweight, and a Q²-slope reweight, do to:
- the reco-980 template;
- the fitted s;
- the subtracted yields.

Study outputs only: no release, budget, note, plan or git change. The `diffvm_datatuned_2026-10-26` study was read-only; its scripts were copied before modification.

Path shorthand: `S` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/diffvm_sdme_2026-10-26`, `O` = `.../output_v6/studies/diffvm_datatuned_2026-10-26`, `DV` = `/afs/desy.de/group/h1/h1gen/diffvm/i386_suse63/ftn`.

## Short answer

- **r⁰⁴₀₀ and R agree.**
  - DIFFVM truth in our fiducial range (Q²_gen > 150, 0.1 < y < 0.7, N = 1895) gives r⁰⁴₀₀ = 0.916 ± 0.016, or R = 12.0 ± 2.5.
  - The measurements stop at Q² ≈ 25–40. H1's highest point is R = 6.5 at Q² = 23, and ZEUS's is R = 6.1 at Q² = 40.5. Extrapolated to Q² ≈ 200, the band is R ≈ 6–20, or r⁰⁴₀₀ ≈ 0.84–0.95, with a power-law central value r⁰⁴₀₀ ≈ 0.91.
  - DIFFVM is at the centre of this band. Reweighting cos θ* to the central extrapolation changes the template total by 0.04%.
- **DIFFVM has no interference terms and no SCHC violation.**
  - Its decay is a pure cos θ* distribution. It is flat in φ, Φ and ψ: r¹₁₋₁ = Re r⁵₁₀ = r⁵₀₀ = 0.
  - The measured values are r⁵₀₀ ≈ 0.13, Re r⁵₁₀ ≈ 0.08, and a cos 2ψ term with r¹₁₋₁ ≈ −0.02 (H1) or +0.10 (ZEUS).
  - The steering requests the interference (IANDEC = 1, cos δ = 0.9), but the H1 build cannot read that card.
- **Effect on s.** Reweighting DIFFVM to the full measured 15-SDME sets moves s by at most +0.006. The Q²-slope variant n = 3.5 instead of 2.4 moves it by +0.007. Both are 4–5% of σ_s = 0.146 and about 10% of the stat error.
- **Effect on the subtraction.** These are changes of the subtracted yield, with s refitted.
  - SDME variants: at most 0.39% (h⁻ bin 30) and 0.28% (h⁺ bin 30).
  - Q² slope: at most 0.69% (h⁻ 30) and 0.36% (h⁺ 30).
  - For comparison, the nominal subtraction itself moves bin 30 by −2.6% / −1.7%, and the σ_s term is 5.0% / 3.2%.
- **Recommendation:**
  - Keep DIFFVM unreweighted for the nominal subtraction. It matches the central R extrapolation.
  - Quote a decay-and-Q²-model term as the envelope of the refit-s variants (§5). It adds about 0.02% absolute in quadrature to the bin-30 totals (§5), so it can also be absorbed as a stated sub-dominant item. The user decides which.

## 1. What DIFFVM generates (source and truth)

**Source** (h1-mc-expert, verified from `DV`):
- **Helicity choice.** The photon helicity is picked with longitudinal probability ε/(1+ε) (`gephot.f:450-452,628`, `iheli.f:30-36`). Longitudinal events are then weighted by R = ξQ²/(m_ρ² + ξχQ²), with ξ = 0.30 and χ = 0.07 (`gengam.f:640-642`; log RPAR22/23; Λ = 0 means "use the VM mass").
- **Resulting fraction.** The longitudinal fraction is r = εR/(1+εR), with ε = 2(1−y)/(1+(1−y)²).
- **Decay.** `decrho.f:127-133,215` accept-rejects cos²θ for helicity 0 and sin²θ for ±1. θ is the π⁺ angle in the ρ rest frame relative to the ρ direction in the γ*p CMS, i.e. the helicity frame (`decrho.f:149,201-204,246`).
  - φ comes from RAMBO and is uniform. The electron azimuth is uniform (`gephot.f:596`).
  - No phase δ and no off-diagonal SDME exist.
  - π⁺π⁻γ (1%) is isotropic phase space.
- **Steering versus build.**
  - `gen_5679.txt:289-319` requests IANDEC = 1 (the SCHC W(θ,ψ) with cos δ = 0.90), MZERO/MONE = 0.5 and IRACOR = 2 (HERACLES).
  - The H1 library `/afs/desy.de/group/h1/h1gen/diffvm/amd64_rhel70/lib/libdiffvm11004.a` has NIPAR = 9 and no IANDEC, COSDEL or HERACLES symbols, so these cards are dropped. The 11001 source is not on disk; that it behaves the same is inferred.
  - The truth confirms it: GenEmpz = 55.2 exactly and there are no radiated photons.

**Truth extraction** (Condor, 8 jobs + merge; `S/truth/merged_truth_diffvm.npz`, scripts `S/scripts/truth/`):
- **Sample.** 25,785 gen events. 25,582 have π⁺π⁻ (GenMotherPDG 113); the other 203 are neutral modes with no selected tracks. The outgoing proton is in the bank, and four-momentum closes to 5×10⁻⁴ GeV.
- **Parity.** The per-event flat-980 counts reproduce the release `counts_980` bin by bin (sum 2196), and all 1054 events of `O/pass/merged_diffvm.npz` are matched.
- **Q² coverage.** The trees hold essentially no events below Q² ≈ 60, so DIFFVM cannot be compared with the data at the measured Q² directly. The comparison has to use DIFFVM's formula.
- **Frame.**
  - z is the ρ direction in the γ*p CMS (cos(z, p′) = −1 exactly).
  - y = q̂ × ẑ and x = y × z. This is the Schilling–Wolf convention, with y along q × v.
  - A geometric toy (`S/scripts/reweight/convention_toy.py`) fixes the mapping to H1: H1 (φ, ϕ) = (−Φ, φ_dec).

**SDMEs from DIFFVM truth** (moment method, all generated events, stat errors; `S/analysis/sdme/sdme_table.txt`, plots `angles_truth.png`):

| Q² bin | N | ⟨ε⟩ | r⁰⁴₀₀ | DIFFVM formula εR/(1+εR) | R | r¹₁₋₁ | r⁵₀₀ / Re r⁵₁₀ |
|---|---|---|---|---|---|---|---|
| 50–100 | 17374 | 0.924 | 0.893(5) | 0.902 | 9.0 ± 0.5 | −0.003(6) | ≈ 0 |
| 100–150 | 5019 | 0.923 | 0.907(10) | 0.911 | 10.5 ± 1.3 | −0.012(12) | ≈ 0 |
| 150–300 | 2596 | 0.920 | 0.922(14) | 0.916 | 12.9 ± 2.5 | −0.003(17) | ≈ 0 |
| > 300 | 591 | 0.903 | 0.903(28) | 0.918 | 10.3 ± 3.3 | −0.090(35) | ≈ 0 |
| FID (Q² > 150, 0.1 < y < 0.7) | 1895 | 0.907 | 0.916(16) | 0.919 | 12.0 ± 2.5 | −0.001(17) | ≈ 0 |

- **Other elements.** All the other accessible elements (r⁰⁴₁₋₁, Re r⁰⁴₁₀, r¹₀₀, r¹₁₁, r⁵₀₀, Im r²₁₋₁, Im r⁶₁₀) are consistent with 0, except two. r¹₁₋₁ = −0.09 ± 0.035 at Q² > 300 (2.6σ) reads as a fluctuation among about 15 elements per bin. r¹₀₀ is negative in every bin (about 2σ combined); DIFFVM generates 0 for it, so it does not affect the reweight.
- **Im r³₁₋₁.** It needs a polarised beam, and neither paper gives it. DIFFVM has 0.
- **Fit of the cos θ shape.** A fit to the per-event W_DV gives χ²/ndf = 18.8/20 (FID), and a uniform shift in r of −0.002 ± 0.011.
- **Selected events.** Events with ≥ 1 selected reco track have r⁰⁴₀₀ = 0.889 ± 0.021 at generator level. Acceptance thus slightly prefers sin²θ (transverse) decays, which is why the decay model matters at all.

## 2. Measured SDMEs and extrapolation

Sources: H1 arXiv:0910.5831 and ZEUS arXiv:0708.1478. Text and PDF copies with full tables are in `S/lit/`, in `sdme_lit.json` and `sdme_lit.md`.

**Verified (read from the tables):**
- **H1, Table 37** (15 SDMEs, elastic plus p-diss with M_Y < 5, ⟨Q²⟩ = 3.1–23.1): r⁰⁴₀₀ = 0.597, 0.680, 0.789, 0.793, 0.877.
  - R from Table 49: 6.52 (+1.27/−0.98 stat, +1.23/−0.96 syst) at Q² = 23.1.
  - r⁵₀₀ = 0.08–0.17, with no Q² trend.
  - cos δ = 0.91–0.98 (Table 48).
  - r¹₁₋₁ = −0.019 ± 0.021 at Q² = 23.1.
- **ZEUS, Table 1** (bins up to 10–100, |t| < 1, p-diss subtracted): r⁰⁴₀₀ = 0.59–0.81, and r⁵₀₀ = 0.085–0.168.
  - Table 7 (W = 90): R = 6.10 (+2.75/−1.56 stat, +2.15/−0.85 syst) at Q² = 40.5, i.e. r⁰⁴₀₀ = 0.86 ± 0.04.
- **Definitions.** Both papers use r⁰⁴₀₀ = εR/(1+εR) (H1 Eq. 28, ZEUS Eq. 4). H1 Eq. 38 is the full 15-SDME W(θ, ϕ, φ), and Eq. 47 is the SCHC form.
- **R's dependence on other variables.** Neither paper fits R(Q²). H1 calls the rise "tamed at large Q²". R is flat in W and |t| and falls with M_ππ.

**Inferred (extrapolation fits, `S/lit/sdme_lit.json`):**
- **Power law.** R = a(Q²/m_ρ²)^b fitted to Q² ≥ 5 gives b = 0.42 ± 0.12, so R(200) = 12 ± 4 and R(450) = 17 ± 8.
- **Envelope.** It runs from R flat at 6.3 to the all-Q² power law (b = 0.60, R(200) = 20).
- **ε.** ⟨ε⟩ is 0.85–0.92 for 0.1 < y < 0.7. This gives r⁰⁴₀₀(200) ≈ 0.84–0.95 (central 0.91) and r⁰⁴₀₀(450) ≈ 0.85–0.97 (central 0.94).
- **SCHC violation at our Q².** Nothing measured indicates that r⁵₀₀ ≈ 0.1–0.17 vanishes at high Q². It is relevant because it produces a sin 2θ cos ϕ term that DIFFVM lacks.

## 3. Reweights (decay angles at fixed production; production Q² shape)

Code and outputs: scripts in `S/scripts/reweight/`; outputs `S/reweight/weights_<variant>.npz` (fields `w_truth`, `w_ctl`, `T980`), `summary.json`, `closure_checks.json` and `README.md`.

**Weight construction.**
- The weight is w = W_target / W_DV. W_DV = (3/4)[(1−r) sin²θ + 2r cos²θ] per event, with r = εR_DV/(1+εR_DV).
- The variants are:
  - R_flat, R_log, R_pow5 (central) and R_powall: cos θ only, with a per-event r⁰⁴₀₀(Q², ε).
  - SCHC_full: H1 Eq. 47 with R_pow5 and cos δ = 0.978.
  - H1_full and ZEUS_full: all 15 SDMEs from the highest-Q² bin, with r⁰⁴₀₀ replaced per event by R_pow5. `_scaled` also rescales the transverse elements.
  - Each full variant also has an `_xflip` version (ϕ → ϕ + π). The x-axis orientation relative to the H1 text could not be pinned down, so the sign of the interference terms is carried as a variant. The geometry-derived mapping is the central one.
  - Q2_n3.0 and Q2_n3.5 change the production slope from (Λ²/(Q²+Λ²))^2.4 to n = 3.0 or 3.5, normalised over the fiducial events. The ZEUS local slopes above Q² ≈ 40 are 3.46 and 3.89.

**Checks.**
- The decay-variant mean weights are 0.99–1.03 on the fiducial sample. They are not renormalised, so part of each SDME-variant shift is weight-sample noise.
- The H1 and ZEUS full sets go negative on 1.6–2.4% of events, and are clipped to 0. SCHC_full stays positive.
- Weighted moments recover the targets:
  - R_pow5 on the fiducial sample: r⁰⁴₀₀ = 0.918 ± 0.016 (target 0.919).
  - H1_full on all events: r⁵₀₀ = 0.135 ± 0.006 (target 0.135) and Re r⁵₁₀ = 0.083 ± 0.003 (target 0.085).
  - On the fiducial sample, the H1_full and ZEUS_full r⁰⁴₀₀ closure is 1–2σ low (0.900 ± 0.019 and 0.885 ± 0.021, target 0.919), because of the clipping.

**Reco-980 template.**

| variant | total T ratio | shape: max per-bin change (bins ≥ 5 counts) |
|---|---|---|
| R_pow5 | 1.000 (−0.04%) | 3.6% (±1.9% stat) |
| R_flat / R_log / R_powall | 1.008 / 1.005 / 0.996 | 47 / 29 / 20% (±22 / 13 / 9% stat) |
| SCHC_full (±xflip) | 1.010 / 0.997 | 90 / 61% (±44 / 36%) |
| H1_full (±xflip) | 1.015 / 1.003 | 83 / 59% (±37 / 30%) |
| ZEUS_full (±xflip) | 1.025 / 1.003 | 132 / 74% (±72 / 44%) |
| Q2_n3.0 / n3.5 | 0.962 / 0.932 | 36 / 56% (±18 / 27%) |

- The per-bin maxima sit in sparse bins and are dominated by noise. The effect after unfolding into gen bins, which is the quantity that matters, is in §4.

## 4. Effect on s and on the subtracted yields

Code and outputs:
- Copied `fit_s.py`, with an added `--diffvm-weighted` option. The original used raw DIFFVM counts, so only the template is weighted; the fit logic is unchanged.
- Copied `subtract_unfold.py`, with an added `--T-ref-file` option.
- Both ran on Condor. Outputs are in `S/fit/<variant>/`, `S/unfold/<variant>_{sfix,srefit}/`, and `S/fit_unfold_summary.{json,csv}`.

**Identity check.** The identity weights reproduce s = 0.0795202 (F2_lowM) and 0.140229 (F2_M11) exactly. They also reproduce `O/unfold/dt_global_r2/per_bin.csv` with a relative difference of 0.

**Table.**
- The columns are:
  - s (F2_lowM), with Δs relative to 0.0795;
  - the subtracted-yield shift y_s(var)/y_s(nom) − 1 with s refitted, in %;
  - Δ(subtracted amount) in h⁻ 30, in %.
- In every variant χ²/ndf stays at 46.0–46.7/27, and the stat errors change by less than 0.002.

| variant | s | Δs | h⁻30 | h⁺30 | h⁺31 | h⁻31 | h⁻65 | h⁻100 | h⁺65 | max all bins | Δamount h⁻30 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| R_pow5 | 0.0793 | −0.0002 | +0.01 | +0.01 | 0.00 | 0.00 | 0.00 | 0.00 | 0.00 | 0.01 | −0.4 |
| R_flat | 0.0784 | −0.0011 | +0.14 | +0.11 | +0.07 | +0.06 | +0.05 | +0.05 | +0.04 | 0.14 | −5.0 |
| R_powall | 0.0799 | +0.0004 | −0.06 | −0.05 | −0.03 | −0.03 | −0.02 | −0.03 | −0.02 | 0.06 | +2.1 |
| SCHC_full | 0.0817 | +0.0023 | −0.24 | −0.09 | −0.03 | −0.08 | −0.02 | −0.13 | −0.05 | 0.24 | +8.8 |
| SCHC_full_xflip | 0.0762 | −0.0033 | +0.28 | +0.11 | +0.04 | +0.09 | +0.02 | +0.11 | +0.05 | 0.28 | −10.4 |
| H1_full | 0.0848 | +0.0053 | −0.39 | −0.21 | −0.08 | −0.11 | −0.07 | −0.19 | −0.09 | 0.39 | +14.5 |
| H1_full_xflip | 0.0822 | +0.0027 | +0.10 | −0.08 | −0.07 | +0.03 | −0.05 | −0.02 | 0.00 | 0.12 | −3.7 |
| ZEUS_full | 0.0858 | +0.0064 | −0.37 | −0.28 | −0.11 | −0.13 | −0.06 | −0.25 | −0.12 | 0.37 | +13.5 |
| ZEUS_full_xflip | 0.0810 | +0.0016 | +0.14 | −0.09 | −0.05 | +0.05 | −0.08 | +0.07 | −0.01 | 0.14 | −5.2 |
| Q2_n3.0 | 0.0832 | +0.0038 | −0.40 | −0.22 | −0.16 | −0.17 | −0.03 | +0.21 | −0.05 | 0.40 | +14.8 |
| Q2_n3.5 | 0.0864 | +0.0069 | −0.69 | −0.36 | −0.27 | −0.29 | −0.04 | +0.36 | −0.06 | 0.69 | +25.3 |

**Readings from the table.**
- **Fixed-s (template-only) shifts** are about half of the values above: at most 0.20% for the SDME variants and 0.42% for n = 3.5 in h⁻ 30.
- **Integrated yield.** It moves by less than 10⁻⁵ in every variant.
- **F2_M11.** Its Δs follow the same pattern, at most +0.0094 (n = 3.5).
- **Q² dependence (n).** It matters about as much as the SDMEs, through the Q² split of the template between the 150–300 and > 300 cells.
  - The study's Q² slices (s = 0.12 ± 0.07 at 150–300 and 0.005 ± 0.12 above 300) are mildly consistent with a steeper fall than n = 2.4. This is inferred, at about 1σ.
  - The effect is still below 0.7% in every gen bin.

## 5. Recommendation

1. **Nominal: DIFFVM as generated, with no decay reweight.**
   - Its r⁰⁴₀₀ (0.916 ± 0.016 in the fiducial range) equals the central extrapolation of the measurements, and the R_pow5 reweight changes nothing (≤ 0.01%).
   - Reweighting to the full measured SDME sets would rest on SDMEs from Q² ≤ 40 carried to Q² ≈ 250, with an undetermined interference sign. That is not a better central value.
2. **Systematic: a decay-and-Q²-model term.**
   - Define it as the per-bin envelope of the refit-s shifts over {R_flat, R_powall, SCHC_full ± xflip, H1_full ± xflip, ZEUS_full ± xflip} ⊕ {Q2_n3.5}, added in quadrature.
   - Its size is about 0.8% in h⁻ 30 (0.39 ⊕ 0.69) and 0.46% in h⁺ 30. Every other bin is at most 0.44% (h⁻ 100). The SDME-variant shifts are 1–2σ of the weight-sample statistics, so the envelope is a conservative bound.
   - Against the σ_s term (5.0% and 3.2%), it raises the bin-30 totals by less than 0.02% absolute: 20.5% stays 20.5% in h⁻ and 18.6% stays 18.6% in h⁺.
   - The per-bin vectors already exist in `S/unfold/*_srefit/per_bin.csv`.
   - Option A: carry it as one extra rank-one term, signed by the H1_full and n = 3.5 directions, which agree in bin 30. Option B: state that it is below 1% and covered by σ_s. A is cleaner for publication of the subtracted amount; B needs no rerun. This is a user decision.
3. **The published subtracted amount.** The decay model moves the amount by up to about 15% (SDMEs) or 25% (n = 3.5) in bin 30. If the amount itself is published, quote it as s·T ± σ_s·T with a ±25% model note. σ_s/s ≈ 180% dominates in any case.
4. **s needs no change.** All variants keep s within 0.076–0.086, against σ_s = 0.146.

## Verified vs inferred

**Verified (file-backed):**
- the DIFFVM decay model, from source and library (§1);
- the truth parity and SDME moments, and the closure of the weighted moments;
- the literature table values (§2);
- the identity reproductions of the fit and the unfold;
- all numbers in §3 and §4.

**Inferred:**
- 11001 behaving like 1.07/1.08 and 1.10/04 (the source is not on disk; the truth agrees);
- the R(Q²) extrapolation and its band (no measurement above Q² = 40);
- that SDMEs from the Q² ≈ 23 bin apply at Q² ≈ 250;
- the x-axis sign of the interference terms, carried as `_xflip`;
- that the H1 SDMEs, which include p-diss with M_Y < 5, apply to the elastic sample;
- the n = 3.0–3.5 range, taken from the ZEUS local slopes.

## Gaps

- No measurement exists at Q² > 40, so the ρ SDMEs at our Q² are extrapolated.
- The selected-event sample is small: 1129 events with a reco track. The per-reco-bin template changes are statistics-limited, and only the unfolded and integrated effects are robust.
- A small offset remains, likely from the isotropic 1% π⁺π⁻γ channel, which the two-body W_DV does not describe: the weighted R_pow5 closure on all events gives r⁰⁴₀₀ 0.008 (1.6σ) below the target. It does not matter at the size of these effects.
- The Q²-shape variants also change the template total (by −4% or −7%). The fit absorbs that through s, so part of the n effect is normalisation.
