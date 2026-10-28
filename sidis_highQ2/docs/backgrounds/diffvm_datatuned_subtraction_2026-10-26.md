# DIFFVM ρ⁰: data-tuned scale factor and subtracted variant (2026-10-26)

**Scope.** This is a study for the high-Q² charged-hadron SIDIS eΣ release (`esigmaphotonxf_2026-10-24`). It asks two things: whether the DIFFVM elastic ρ⁰ MC can be normalised to data with a template fit, as COMPASS did in arXiv:2410.12005, and what subtracting s×DIFFVM does to the release. Nothing installed was changed: not the release, the budget_v2 outputs, the closure plan or the note. The study builds on `diffvm_large_bins_2026-10-25.md` and `exclusive_vm_precedent_2026-10-26.md` in this folder.

Path shorthand: `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, `O` = `H/output_v6/studies/diffvm_datatuned_2026-10-26`.

## Short answer

- **Data-tuned scale factor:** s = 0.08 ± 0.15, in units of the luminosity-scaled DIFFVM (×0.135913).
  - The uncertainty is 0.075 stat (scaled by √(χ²/ndf)) ⊕ 0.125 syst.
  - s is consistent with 0. The 95% CL upper limit is s < 0.32 (Gaussian, total σ), or s < 0.42 when the √(χ²/ndf)-scaled stat UL (0.22) and 1.645·σ_syst are added linearly (conservative). s = 1 is excluded.
- **Slices:** no significant dependence on Q² or z_max.
  - Q² 150–300: 0.12 ± 0.07; Q² > 300: 0.005 ± 0.12 (p = 0.41).
  - z_max > 0.7: 0.11 ± 0.06; z_max ≤ 0.7: 0.00 ± 0.09 (p = 0.29).
  - Corner (Q² < 300 and z_max > 0.7): 0.15 ± 0.07. All slice errors are stat only.
- **Subtracted variant:** subtracting s×DIFFVM and re-unfolding with the release IBU moves generator bin 30 by −1.7% (h⁺) and −2.6% (h⁻). The ±σ_s term replaces the ±100% term.
  - h⁻: the DIFFVM term goes from 34.6% to 5.0%, and the bin-30 total from 39.7% to 20.5%.
  - h⁺: the DIFFVM term goes from 22.0% to 3.2%, and the bin-30 total from 28.5% to 18.6%.
- **Veto cross-check in bin 30:** the clean-two-track veto removes 8 of 70 data tracks.
  - Predicted: 3.4–4.1 at the fitted s, 2.1–2.8 at s = 0, and 18–19 at s = 1. Full DIFFVM is excluded at about 3σ.
  - The excess implies s ≈ 0.36 ± 0.18. The σ_s term does not cover this.
- **New finding:** an exclusive-like two-track excess exists at M > 1.5 GeV. It does not have the DIFFVM shape (§4).
- **Recommendation:** keep the result unsubtracted and replace ±100% by **±42%**, the conservative data upper limit. This covers the fit limit and the bin-30 veto excess. Bin-30 totals become 24.1% (h⁻) and 20.3% (h⁺); the ±55% option gives 27.0% and 21.7%. Subtracting with s = 0.08 ± 0.15 is the alternative, but in bin 30 it under-covers the veto excess (§6).

## 1. Inputs and control-variable pass

- **Pass job:** `O/scripts/pass/ctl_pass.py` on Condor, 120 jobs plus one merge (cluster 5891066), about 3 minutes of wall time.
- **Samples:** 660 data files (the release list), 404 DIFFVM, and the response shards of rapgap31 (364) and djangoh14 (501).
- **Weights:** fit templates use the event weight w1·w2. The per-reco-bin vectors use w1·w2·NucliaWeight, as the release response does. DIFFVM is unweighted.
- **Selection:** the release event cuts and track selection. The pass uses a mask-returning replica of `_select_and_bin` from `build_data_vector_v4_alphaveto_esxf.py`, with eΣ z and pT_h.
- **Parity:** the per-file parity against the builder's own function is exact on every non-empty file (16 files are empty).
- **Closure with the release:** the merged data vector equals the release `n_raw` in all 980 bins (3,877,298 tracks), and DIFFVM equals the release `counts_980` (2196).
- **Outputs:** `O/pass/merged_{data,diffvm,rapgap31,djangoh14}.npz` and `O/pass/merge_summary.json`.
- **Control variables:**
  - loose tracks: primary, p_T > 0.15 GeV, |η| < 1.6;
  - z_sum = Σz over the loose tracks;
  - p_T,bal = |p_T,e + Σ p_T,trk|;
  - (E−p_z)_e + Σ(E−p_z)_trk;
  - M_ππ.
- **Difference from the 10-25 note:** z here is the release eΣ z, not the tree `z_SIDIS_es`. The ρ-window counts are therefore 72 (data) and 655 (raw DIFFVM), not 65 and 496.

## 2. Template fit

**Model.**
- Binned Poisson likelihood, μ = s·0.135913·D + a·S. D is the raw DIFFVM template. S is the rapgap31 template, the release response MC; its normalisation a is free and profiled.
- Template MC statistics enter via Barlow–Beeston-lite.
- The ±1σ interval is taken at Δ(−2lnL) = 1. The 95% UL is taken at Δ = 2.71 within s ≥ 0.
- Code: `O/scripts/fit/fit_s.py`. Results: `O/fit/fit_results.json`. Plots: `O/fit/plots/`.

**Closure test.**
- A synthetic test, run on the earlier nominal F2 (not F2_lowM), recovers injected s = 0.3 and 1 within errors.
- At injected s = 0 the estimate sits at the non-negativity wall: s ≈ −0.04, with an underestimated σ. The data nominal lies away from the wall; its lower 1σ edge is +0.026.

**Fits.** All fits use two-track opposite-sign events. The errors are stat only and unscaled.

| fit | control variables | s | stat ±1σ | UL95 (stat) | χ²/ndf | p |
|---|---|---|---|---|---|---|
| **F2_lowM (nominal)** | z_sum × p_T,bal, M_ππ < 1.5 GeV | **0.080** | −0.054/+0.061 | 0.18 | 46.5/27 | 0.011 |
| F2_M11 | same, M_ππ < 1.1 GeV | 0.140 | −0.057/+0.064 | 0.25 | 41.8/27 | 0.035 |
| F1_1D | M_ππ, 0.9 < z_sum < 1.1 | 0.01 | ±0.15 | 0.26 | 43.4/22 | 0.004 |
| F1_2D | M_ππ × z_sum | 0.00 | −0.10/+0.10 | 0.17 | 150.9/94 | 0.0002 |
| F2_epz | (E−p_z) closure × p_T,bal, all M | 0.185 | ±0.09 | 0.34 | 53.0/33 | 0.015 |
| F2 (all M) | z_sum × p_T,bal, all M | 0.325 | −0.083/+0.090 | 0.48 | 52.4/28 | 0.003 |

**How the nominal was chosen.**
- The nominal was first the all-mass F2. It was changed to F2_lowM after the first data fit, for two reasons.
  - The exclusive-like data events at M > 2 GeV drive F2's s = 0.33 (§4). DIFFVM has 14 of its 739 exclusive-like events above 1.5 GeV, and 1 above 2 GeV, so its shape cannot describe them.
  - F2_lowM keeps the exclusivity variables, which are not tied to the ρ peak.
- The ρ-peak fits (F1) agree with F2_lowM within 0.7σ.
- Because the choice was made after seeing the data, the mass-window variation is carried as a systematic.

**Systematics (|Δs|), in quadrature 0.125:**

| category | variation | Δs |
|---|---|---|
| SIDIS shape | a split at p_T,bal = 1 GeV | 0.061 |
| mass window | M < 1.1 GeV (F2_M11) | 0.061 |
| SIDIS normalisation | a fixed to the inclusive DIS count | 0.061 |
| binning | coarse / fine | 0.051 |
| window | z_sum and p_T,bal ranges | 0.043 |
| SIDIS generator | djangoh14 | 0.003 |

**Fit quality.**
- All fits have poor χ² (p from 0.0002 to 0.035). The misfit lies outside the exclusive corner, at low z_sum and large p_T,bal.
- The stat error is scaled by √(46.5/27) = 1.31, from 0.057 to 0.075. The SIDIS-shape and normalisation terms also cover the misfit.

**Acceptance.**
- The fit region holds 83.8% of DIFFVM's event-cut events and 88.1% of its reco-grid tracks.
- Applying s to the remaining part is an assumption.
- The Q² strata used by `dt_q2stratum` are S0 (Q² 150–300), with s = 0.12 ± 0.07, and S1 (300–700), with 0.007 ± 0.07. S2 and S3 take the global value.

## 3. Subtracted variant and comparison

**Code and method.**
- `O/scripts/unfold/subtract_unfold.py`, one Condor job per run. It reuses the release loaders and engine read-only: `components_groupC`, `lean_unfold`, IBU k = 4, rapgap response.
- Nominal: y_s = unfold(d − s·T), with T zeroed in empty-bin-guard bins, as in the release term.
- New rank-one term: max|y(s ± σ_s)/y_s − 1|, signed as `diffvm_rho_signed_rel` (ρ = +1).

**Validation.**
- s = 0 reproduces the release yields to 2·10⁻¹⁵.
- s = 0 with σ = 1 reproduces `diffvm_rho_rel` and its signed vector exactly.

**Approximations.**
- The other relative budget terms are unchanged.
- The absolute IBU stat error is kept.
- The e/V0 template repoint of the downstream step is not called; it does not affect y0 or the DIFFVM term.

**Outputs.** Each tag under `O/unfold/<tag>/` holds `per_bin.csv`, `summary.json` and `signed_vectors.npz`.
- `dt_global_r2`: the nominal, s = 0.0795, σ_s = 0.146.
- `unsub_k042`: unsubtracted with a ±42% term.
- `dt_global`: ±55% reference, together with the round-1 σ_s = 0.123.
- `dt_q2stratum` and `dt_global_q2shape`: the per-stratum s variant and the Q²-shape term.

**Bins with release DIFFVM > 10%.**
- "Term" is the DIFFVM uncertainty term. "Total" is the total quoted uncertainty, with the old term replaced.
- "Shift" is y_s/y0 − 1.

| bin | release: term | release: total | subtracted: shift | subtracted: term | subtracted: total | unsub ±55%: term | unsub ±55%: total | unsub ±42%: term | unsub ±42%: total |
|---|---|---|---|---|---|---|---|---|---|
| h⁻ 30 | 0.346 | 0.397 | −0.026 | 0.050 | **0.205** | 0.187 | 0.270 | 0.142 | **0.241** |
| h⁺ 30 | 0.220 | 0.285 | −0.017 | 0.032 | **0.186** | 0.120 | 0.217 | 0.091 | **0.203** |
| h⁺ 31 | 0.144 | 0.213 | −0.011 | 0.021 | 0.160 | 0.079 | 0.176 | 0.060 | 0.169 |
| h⁻ 31 | 0.138 | 0.211 | −0.011 | 0.020 | 0.162 | 0.075 | 0.177 | 0.057 | 0.170 |
| h⁻ 65 | 0.132 | 0.224 | −0.010 | 0.019 | 0.183 | 0.072 | 0.195 | 0.055 | 0.189 |
| h⁻ 100 | 0.112 | 0.256 | −0.009 | 0.016 | 0.232 | 0.061 | 0.239 | 0.047 | 0.235 |
| h⁺ 65 | 0.104 | 0.201 | −0.008 | 0.015 | 0.173 | 0.057 | 0.182 | 0.043 | 0.178 |

**Effect elsewhere.**
- Besides h±30, only three bins (h⁺31, h⁻31, h⁻65) shift by more than 1%. The integrated yield moves by under 10⁻⁵.
- Per-stratum s gives bin-30 shifts of −2.6% (h⁺) and −4.0% (h⁻).
- The Q²-shape term is at most 1.4%. It is not needed, since the slices agree.

## 4. Veto cross-check (generator bin 30)

- **Code and outputs:** `O/scripts/fit/veto_check.py`; results in `O/veto/veto_check.json` and `.csv`.
- **Vetoes:**
  - V1 removes events with exactly two opposite-charge loose tracks and 0.9 < z_sum < 1.1.
  - V2 adds p_T,bal < 1 GeV.
- **SIDIS normalisation:** SIDIS is normalised to the inclusive DIS count.

| gen bin 30 (70 data tracks) | V1 | V2 |
|---|---|---|
| data removed | 8 | 8 |
| predicted, s = 0 | 2.8 | 2.1 |
| predicted, s = 0.08 | 4.1 | 3.4 |
| predicted, s = 1 | 19.1 | 18.3 |
| implied s | 0.32 ± 0.18 | 0.36 ± 0.18 |

**Reading.**
- s = 1 is excluded at about 3σ.
- The excess over the fitted s is about 4.6 tracks, 6.6% of the bin. That is larger than the subtracted variant's 3–5% term.
- At event level the corner excess sits at M ≥ 1.5 GeV: 26 events in data against 15 predicted.
- With a low-mass veto (V2 and M < 1.5 GeV), the corner has 23 data events, against 18.7 predicted at s = 0.08 and 79.5 at s = 1. The implied s is then 0.14 ± 0.08.

**Mass spectrum of all V2 events.**
- In total there are 91 data events, against 66 predicted at s = 0.08.
- In 0.6–0.85 GeV: 15 in data, against 17 predicted. DIFFVM alone at s = 1 would give 62.
- Above 2 GeV: 44 in data, against 15.8 predicted. DIFFVM has 1 event there.
- Plot: `O/fit/plots/v2_mass.png`.

## 5. What the fit cannot constrain

- **Exclusive events outside the two-track topology.** This covers a third central track, ω → π⁺π⁻π⁰, and tracks lost outside |η| < 1.6. About 16% of DIFFVM falls outside the fit region; s is assumed to hold there.
- **Proton-dissociative ρ⁰ with central remnant tracks.** Pure forward dissociation stays inside the region. It is absorbed into s only as far as its p_T,bal shape resembles DIFFVM's.
- **φ, ρ′, J/ψ and the high-mass exclusive-like excess.** These are not DIFFVM-shaped. Where their tracks land in the reco bins was not checked.
- **A data-only filter removing clean two-track events.** This is still not ruled out (see the 10-25 note). It would bias s low.
- **The SIDIS template shape.** It is imperfect, and is covered by the two largest systematics.

## 6. Recommendation and cost

**Recommended: unsubtracted, with the DIFFVM term at ±42%** (s < 0.42, the conservative 95% limit).
- It covers the fit limit and the central value of the bin-30 veto excess (implied s ≈ 0.36 ± 0.18). The excess is not DIFFVM-shaped, so this is a conservative envelope.
- It needs no central change.

**Alternative: subtract s = 0.08 ± 0.15.**
- This gives the smallest totals.
- In bin 30 its 3–5% term does not cover the 6.6% veto excess. It would need an extra bin-30 term of that size, or a statement that the excess is outside the DIFFVM model.

The ±55% option is looser than the data now require.

**Cost of folding into the closure rerun** (`sidis_highQ2/docs/plans/closure_uncertainty_revision_2026-10-26.md`)

| | unsubtracted ±k | subtracted |
|---|---|---|
| change | T scaled by k in group C (`components_groupC.py:181-187`) | n_subtracted − s·T in the data vector |
| steps rerun | group C and the budget assemble, inside the plan's existing s08 | `esph_downstream.py` vector, release, born, detlegs, groupA, groupB, groupC and assemble, then s08/s09, closure term, gate and s15 on the new yields |
| extra Condor jobs | about 1–2 | about 8–10, minutes to 1 h each; no 660-job production |
| extra time | none | about half a day, plus a re-review of the numbers |

Either option changes the signed-off ±100% term (memory `decisions/2026-10-24-esigma-new-rank1-bg-terms-signed-off.md`) and the plan's Q2 answer, so it needs a user decision.

## Verified vs inferred

- **Verified (file-backed):**
  - pass parity and the release-vector equality;
  - all numbers for the fits, slices, veto and unfold (paths above);
  - the reproduction of the release y0 and the DIFFVM term.
- **Inferred:**
  - the nature of the high-mass exclusive-like events (candidate γγ → ℓℓ);
  - that s applies outside the control region;
  - the cost estimate (step names read from `esph_downstream.py`; job counts not run).
