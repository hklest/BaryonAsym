# pT_h and phi_h frame bug — discovery, diagnosis, fix

**Date:** 2026-05-24  
**Status:** pT_h FIXED in v2; phi_h FIXED in v3; downstream re-derivation pending  
**Severity:** CRITICAL — invalidates all pre-2026-05-24 pT_h-binned and phi_h-binned results

## The bug

The downstream charged-SIDIS kinematic helper `compute_pth_phih_z()` computed both `pT_h` and `phi_h` from lab-frame 3-vectors while claiming to return `\gamma*+p`-frame quantities. In practice the implementation evaluated `pT_h` as `|p × q̂|` using the hadron and virtual-photon lab 3-vectors, and evaluated the Trento angle from lab-frame cross products built from the same inputs. That is not the hadronic-CM observable definition used for SIDIS at H1 high Q².

The mislabeling appeared in the Python-side extraction chain, including `extract_phih_moments_v5f.py` and `extract_phi_moments_data.py`, whose docstrings described the result as if it were a Lorentz-invariant `\gamma*+p` quantity. It is not. `|p × q̂|` is only preserved under boosts along `q⃗`, and Trento `phi_h` is likewise not invariant under the generic HERA lab→CM boost.

## How it was detected

A single-file diagnostic on `sidistree_em0405_d05_0142.root` compared `pT_h` three ways: lab `Pt`, the reference-script formula, and the proper `\gamma*+p` CM result after an explicit Lorentz boost. The reference formula tracked lab `Pt` far more strongly than the true CM quantity: correlation 0.935 with lab `Pt`, but only 0.59 with the boosted-CM `pT_h`. The central values told the same story: median `pT_h = 0.73` GeV in the broken formula versus `0.47` GeV in the proper CM computation.

The same diagnostic on `phi_h` showed an even larger failure in the harmonic moments. Using the lab-frame formula gave `<cos phi_h> = -0.675`, while the proper CM-frame evaluation gave `-0.067`. For `<cos 2phi_h>`, the broken lab result was `+0.385` and the proper CM result was `+0.031`. That is an order-of-magnitude distortion, so all previously extracted `phi_h` harmonics using the lab-frame formula are wrong.

## Why it bites at HERA

The shortcut fails because the boost from the HERA lab frame to the `\gamma*+p` center-of-mass frame is not aligned with the virtual-photon direction. In symmetric or nearly collinear situations one can sometimes get away with treating `|p × q̂|` or Trento-style cross products as effectively unchanged, but that logic does not apply here.

At HERA-II the beams are strongly asymmetric (`E_e = 27.6` GeV, `E_p = 920` GeV nominally), and the incoming lepton has a substantial component transverse to `q⃗` in the lab. As a result, the lab→CM boost has a large component not parallel to `q⃗`. Therefore lab-frame `|p × q̂|` is not the CM-frame transverse momentum, and lab-frame Trento cross products do not return the CM-frame `phi_h`.

## The fix (pT_h)

The correct procedure is to construct the event-level hadronic-CM four-vector

`W = q + P`,

where `q = k - k'` is built from the beam and scattered-electron four-vectors and `P` is the incoming proton four-vector. Then Lorentz-boost the hadron, lepton, and relevant reference vectors into the rest frame of `W`, and only there evaluate `pT_h` as the component transverse to the boosted `q` axis.

The first corrected implementation for `pT_h` was `extract_pertrack_v5g_fixedFrame_v2.py`. The current canonical implementation is `extract_pertrack_v5g_fixedFrame_v3.py`, which keeps the v2 CM-frame `pT_h` fix, uses the per-event `BeamProtP` branch rather than a hardcoded 920 GeV proton beam, and extends the schema with both `phi_h_old` (lab-frame Trento, intentionally retained as the broken audit reference) and `phi_h` (correct `\gamma*+p` CM-frame Trento angle). The extractor also carries through `Pt_lab` and `Eta_lab` for downstream cuts and writes a `z_outlier` flag for the rare tracks with invariant `z = (P·p_h)/(P·q)` outside `[0,1]`.

## Validation

The primary validation is the expected `pT_h` compression after the proper boost. The median `pT_h` dropped from `0.73 → 0.47` GeV, matching the qualitative expectation that the physical distribution should be concentrated near the low-`pT_h` region rather than peaking in the several-GeV range.

The downstream multiplicities comparison also behaved as expected. `v5g_multiplicities_v3.pdf` shows the dominant peak moving into the `[0, 0.5)` GeV bin in all eight `(Q², x)` cells, including the high-Q² S3a/S3b cells. Cell totals shifted only modestly in most bins, from `<2%` in S0a up to about `+17%` in S3b, with the largest changes appearing where tracks previously leaked beyond the broken `pT_h < 8` cap and are now recovered by the corrected CM-frame definition.

## Affected results

All pre-2026-05-24 results that bin in `pT_h` or derive `phi_h` from the downstream lab-frame helper are affected. This wrong-frame issue is independent of, and additive to, the already-known v5f/v5g production-state and binning-staleness caveats.

- `H1/sidis_highQ2/docs/results/multiplicities_cross_sections_A_h_v5f_no_ep0304_v2.md` — **SUPERSEDED** already for pre-v5g / stale-sample reasons; also wrong-frame for `pT_h`.
- `H1/sidis_highQ2/docs/results/phih_harmonics_v5f_no_ep0304_v2.md` — **SUPERSEDED** already; also wrong-frame for `phi_h`.
- `H1/sidis_highQ2/docs/results/phih_harmonics_v5f_no_ep0304_v3_coarse.md` — **SUPERSEDED** already; also wrong-frame for `phi_h`.
- `H1/sidis_highQ2/docs/results/phih_harmonics_v5f_no_ep0304_v4_unbinnedML.md` — **SUPERSEDED** already; also wrong-frame for `phi_h`.
- Any pre-2026-05-24 v5g interim `pT_h`-differential plots or tables derived with `extract_pertrack_v5g.py` or the v1 fixed-frame extractor — superseded by the v2 CM-frame extractor.
- Any pre-2026-05-24 `phi_h` harmonic diagnostic or result using `compute_pth_phih_z()` in its lab-frame form — invalid until rerun with a CM-frame implementation.

## phi_h fix (landed 2026-05-24)

The production `phi_h` fix landed on 2026-05-24 in the canonical extractor `extract_pertrack_v5g_fixedFrame_v3.py` at NAF path `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/extract_pertrack_v5g_fixedFrame/extract_pertrack_v5g_fixedFrame_v3.py`.

The v3 schema is v2 plus two `phi_h` audit/fix columns:
- `phi_h_old` = lab-frame Trento angle from the broken formula, retained unchanged for audit closure
- `phi_h` = correct `\gamma*+p` CM-frame Trento angle

Full-sample weighted `phi_h` moments after the standard audit cuts (`z_outlier==0` and `Pt_lab < 15 GeV`, 3,403,119 tracks weighted by `TrigWeightAC`) are:

| Moment | Lab (broken) | CM (correct) |
|---|---:|---:|
| ⟨cos φ_h⟩ | −0.6675 | **−0.0905** |
| ⟨cos 2φ_h⟩ | +0.3688 | **+0.0332** |
| ⟨sin φ_h⟩ | +0.0010 | +0.0013 |
| ⟨sin 2φ_h⟩ | −0.0012 | −0.0003 |

These lab-frame moments reproduce the audited `extract_phih_moments_v5f.py` / `extract_phi_moments_data.py` behavior essentially exactly, confirming that those scripts remain the frozen broken reference and were NOT modified. The corrected CM-frame values sit in the physically reasonable HERA-II high-Q² SIDIS range, with small `cos φ_h` and `cos 2φ_h` moments consistent with modest Cahn / Boer-Mulders contributions rather than the order-one artifacts produced by the wrong frame.

Smoke validation on the reference file `sidistree_em0405_d05_0142.root` matched the diagnostic expectations: `⟨cos φ_h⟩ = -0.078`, `⟨cos 2φ_h⟩ = +0.035`, and `⟨cos φ_h⟩_old = -0.669`, all within the expected windows. The old and corrected angles remain correlated (`corr(phi_h, phi_h_old) = 0.92`), but the wrapped median shift is still large (`median |Δphi_h| = 31.5°`), which is why the harmonics move by about an order of magnitude.

## Permanent trap for future agents

> **TRAP: pT_h and phi_h must be computed in the γ*+p CM frame, not the lab frame.** The H1OO tree ChargedTot stores raw lab-frame 4-vectors (Px, Py, Pz, scatEX/Y/Z/EE, BeamElecE, BeamProtP) but does NOT precompute pT_h or phi_h. Computing pT_h as `|p × q̂|` or phi_h with Trento cross products using lab 3-vectors gives WRONG answers at HERA-II asymmetric beams (boost lab→CM is not along q̂). The canonical extractor is `extract_pertrack_v5g_fixedFrame_v3.py` which performs the full Lorentz boost W=q+P (per-event BeamProtP), retains `phi_h_old` for audit, and evaluates `pT_h`/`phi_h` in the CM frame. The C++ TreeBuilder does this correctly for V0/proton subtrees but not for ChargedTot.

## Canonical extractor

The canonical per-track extractor is now `H1/sidis_highQ2/scripts/extract_pertrack_v5g_fixedFrame_v3.py` (NAF: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/extract_pertrack_v5g_fixedFrame/extract_pertrack_v5g_fixedFrame_v3.py`). It supersedes both `extract_pertrack_v5g_fixedFrame_v2.py` and the earlier v1 fixed-frame prototype: v2 remains the first correct `pT_h` repair, while v3 is the canonical combined `pT_h` + `phi_h` CM-frame extractor.

The corresponding merged CSV is `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame_v3/pertrack_v5g_fixedFrame_v3_merged.csv.gz` on NAF, with local copy `H1/sidis_highQ2/data/pertrack_v5g_fixedFrame_v3_merged.csv.gz`. Production used Condor cluster `3429468` with 660 jobs, ~60 s wall, 100% success, producing 3,409,486 rows (126 MB compressed). Per-period totals are ep0304 521,080 / em0405 1,000,336 / em06 547,068 / ep0607 1,341,002.

## References

- Diagnostic plots: `H1/sidis_highQ2/plots/diag_high_pTh_2026-05-24/` (pT_h)
- Diagnostic plots: `H1/sidis_highQ2/plots/diag_phih_frame_2026-05-24/` (phi_h)
- Canonical CM-frame extractor: `H1/sidis_highQ2/scripts/extract_pertrack_v5g_fixedFrame_v3.py` (Python)
- Superseded first `pT_h` fix: `H1/sidis_highQ2/scripts/extract_pertrack_v5g_fixedFrame_v2.py` (Python)
- Frozen broken audit references: `extract_phih_moments_v5f.py` and `extract_phi_moments_data.py` (`compute_pth_phih_z()` intentionally NOT modified)
- Multiplicities PDF comparison: `H1/sidis_highQ2/plots/v5g_multiplicities.pdf` (broken, v1) vs `v5g_multiplicities_v2.pdf` (fixed, hardcoded Ep) vs `v5g_multiplicities_v3.pdf` (fixed, per-event Ep)

## 2026-05-24 follow-up: BBB normalization bug

While diagnosing why IBU integrated yields disagreed with bin-by-bin (BBB) integrated yields by ~20% on high-purity cells (S0a–S3b, h+ and h-), an algebra bug was identified in `compute_xsec_v3_HQv5gBprime.py`. The BBB acceptance was being computed as `eps_h = n_reco_and_gen / n_gen`, which is the diagonal-only quantity acceptance × purity, not pure acceptance. Pure acceptance is `eps_h = n_reco_match / n_gen`, where `n_reco_match` is the MC reco-matched-hadron count in the reco bin regardless of where the matched gen track falls. The old formula systematically inflated `1/eps_h` and over-corrected the data BBB yields uniformly by 12–22% across all 16 (Q²,x,charge) cells. Purity remains the decision rule for `bbb_valid` (use BBB only where purity ≥ 0.5) but is NOT part of the correction formula itself. The fix was a one-line change in `compute_xsec_v3_HQv5gBprime.py` (line 417). The old JSON has been renamed to `xsec_v3_HQv5gBprime_2026-05-24.OLD-WRONG-acc-x-purity.json` for audit; the new corrected JSON overwrites the canonical path. After regeneration, IBU/BBB integrated yields agree within 4–7% on the eight largest cells (S0a–S2a) as expected, while TUnfold still shows the previously-documented ~50% over-regularization. Downstream regeneration: `source_00_nominal.json` and `systematics_combined_v3_HQv5gBprime.json` were re-emitted from the corrected xsec JSON. The IBU and TUnfold per-source JSONs (`source_unfolding_IBU.json`, `source_unfolding_TUnfold.json`) still reference OLD BBB ratios in their `integrated_bbb` fields; the unfolded values themselves do not depend on BBB, so their `status="deferred_to_unfolding"` keeps them out of the totals and no regeneration of the underlying IBU/TUnfold CSV outputs is required. For background, see the planned diagnostic memo at `H1/sidis_highQ2/docs/methodology/BBB_vs_unfolding_normalization_diagnosis_2026-05-24.md`.
