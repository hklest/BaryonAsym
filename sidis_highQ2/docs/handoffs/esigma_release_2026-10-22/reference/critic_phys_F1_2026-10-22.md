# Hostile critic follow-up: F1 nominal-consistent shifted kinematics, physics at nonzero shift (2026-10-22)

Scope: `H1ChargedSidis_HighQ2/scripts/unfolding/final_detlegs_optA_2026-10-21/f1_nominal_consistent.py` (apply_f1), the
leg-vector construction it feeds (`hfs_empz_n2_2026-10-21/n2_leg_vectors.py`, budget19 STEP-6), and the stored trees. NAF read-only.
New checks: `scripts/critic_phys/{f1_xf_residual,leg_decomposition,envelope_edges,frozen_elec_cuts}.py`.

## Verdict: **DISPATCH-WITH-FIXES**

F1 is physically sound at nonzero shift; (a)–(c) below are all answered positively.

Item (d), however, turned up a **methodological error in how every detector leg is turned into a systematic**. The leg is
IBU(D_leg − B; **M_leg**) − IBU(D; M): data AND MC are shifted in the same direction, so the calibration effect largely cancels.
The production still produces every input the correct definition needs (shifted responses M_leg, plus the shifted DVs as cross-checks), so this
does **not** block dispatch. It **blocks quoting** until the leg-vector step is redefined.

One cheap builder addition is recommended before dispatch: the frozen E′ > 11 GeV threshold (D2).

## (a) Is "stored + [g(shift) − g(zero)]" the right estimator? Yes.
At nonzero shift, the delta form equals an absolute recompute with the nominal definition, up to the residual (stored − g(zero)).
- **Q²/x/y residual:** float32 rounding. Calculator closure is ≤6.6e-7, verified earlier with 27.6/920 and Σ_h = 2·27.6·Yh.
- **xF residual in data:** |x0 − stored| has median 1.5e-9 and max 5.4e-6 on non-sentinel rows.
- **What the form buys:** it removes the float32/float64 edge flips at zero and keeps the −999 sentinels (Δ is added to −999, so they still pass xF<0, as in the nominal).
- **What it costs:** nothing physical. The derivative is the nominal-definition derivative evaluated at the nominal point, which is exactly what a scale systematic needs.

## (b) xF transfer assumptions: exact for data; one MC caveat (MINOR)
- **Energy shift:** E′_ES = Q2e/(2E_b(1+cosθ)) with calculator Q2e = 2·27.6·E′(1+cosθ). Hence E′_ES = (27.6/E_b)·E′. I measured E′_ES/scatEE = 0.998264 = 27.6/E_b (median, ep0607). So E′_ES ∝ (1+δ) exactly.
- **θ shift:** both the calculator Q2e and the Stage-2 denominator carry (1+cosθ′), so E′_ES is θ-independent and only the direction rotates. Exact.
- **MC caveat:** the Stage-2 formula does **not** reproduce the stored MC xF_SIDIS_es for **16% of selected MC rows** (|x0 − stored| > 1e-3; p99 1.8e-2, max 0.59; RAPGAP 7094 file). In data it reproduces to 5e-6. Some MC-specific Stage-2 input differs (beam 4-vector, electron or hadron 4-vector).
  - Consequence: the MC Δ is computed with a formula that is not MC Stage-2's, while data Δ is exact.
  - Impact is tiny: only xF sign flips matter (45 of 254,685 rows at +1% E′, 0 at ±1 mrad).
  - Recommend: identify the MC input before quoting, or record the bound.

## (c) HAT vs calibrated E′ in the deltas: not an issue
The stored scatE* is the calculator's electron. Q²_e = 2·27.6·scatEE·(1+cosθ) reproduces Q2es/Q2e, and Yes and Xes close to ≤6.6e-7. ΔQ² therefore uses the same E′ as the calculator. Even if it did not, a calibration factor c would enter as δ·(c−1), which is second order (~1e-4 relative).

## (d) Before dispatch / before quoting

| # | Sev | Finding (evidence) | Action |
|---|---|---|---|
| D1 | **BLOCKER for quoting (not for dispatch)** | **The detector legs shift data and MC together, which cancels the calibration effect.** `n2_leg_vectors.py`/STEP-6 compute IBU(D_leg − B; M_leg) vs IBU(D; M), and the N2 note itself says "largely cancels data vs MC". A calibration uncertainty is the *relative* data/MC scale, so exactly one side must be shifted. Decomposition on the installed 2026-10-14 cone-frame legs (`leg_decomposition.py`, RAPGAP, k=4) is in the table below. Additional evidence: **MC-only ± legs anti-correlate as a scale effect must (−0.81); the installed "both" ± legs do not (−0.13 / +0.10).** The installed integrated values (+0.15…+0.21%) have the **same sign for +1% and −1%**, i.e. they are offset and noise, not scale response. The installed electron and HFS components (budget and covariance S5f deltas) therefore **under-state** the electron-energy systematic by ×2.5–3 per bin. | Redefine the leg as **IBU(D; M_leg) − IBU(D; M)** (standard H1 practice: shift the MC, unfold the nominal data). Keep data-only IBU(D_leg; M) as a cross-check: it should ≈ −(MC-only); corr −0.73 here, with the old zero-offset still inside. Apply the same to the HFS legs and the covariance deltas. No new jobs: the M_leg shards are exactly what is being dispatched. **Expect the quoted total to rise:** elecE ±1% MC-only medians are about 3.6–4.0% vs 1.3–1.7% installed. **User input needed:** whether ±1% is the right E′ scale. H1 high-Q² LAr practice is typically 0.5–1%, θ-dependent. |
| D2 | MAJOR (pre-dispatch, cheap) | **Frozen upstream E′ > 11 GeV cut.** min scatEE = 11.00–11.06 GeV in every data and MC tree. Q², y and E−pz are not truncated: Q2es down to 75–80, Yes to 0.04, Empz to 30. 0.06–0.29% of selected data tracks (0.15–0.18% MC) lie in the 1% band above 11 GeV, concentrated at high y (median y 0.64–0.68, Q² ~180–250): **0.3–1.5% of all y > 0.5 tracks**. No shifted builder re-applies the threshold to E′_shift, so the −1% leg never loses these events and the +1% leg cannot gain the absent sub-threshold events. In the MC-only form (D1) this acceptance edge is part of the very effect being measured. | Add `E′_shift > 11.0 GeV` (the upstream value) to the shifted response builder and the shifted DV builder. It is a no-op at zero shift, so F1 bit-identity holds. For the + leg, take the gain as the mirror of the − leg loss or document it as one-sided. |
| D3 | MINOR | Electron θ edges: within 1 mrad of the observed θ extremes there are only 0.002–0.13% of selected tracks, and they are not evidently hard cuts. | Record. |
| D4 | MINOR | MC xF formula mismatch (see (b)). | Identify the input or bound it (flips ≤0.02%). |
| D5 | note | Under D1's MC-only definition, the zero-offset question (F1) only needs MC zero ≡ MC nominal. F1 delivers that bitwise; the zero DV becomes a cross-check only. | none |

**D1 decomposition (RAPGAP, k=4, installed 2026-10-14 cone-frame legs):**

| leg | per-bin median \|Δ\|: installed "both" | MC-only IBU(D; M_leg) | data-only IBU(D_leg; M) | corr(MC-only, data-only) |
|---|---|---|---|---|
| elecE +1% | 1.33 / 1.71% | **3.63 / 3.84%** | 4.78 / 5.07% | −0.73 / −0.78 |
| elecE −1% | 1.65 / 1.62% | **4.04 / 4.05%** | 4.77 / 5.33% | −0.78 / −0.71 |
| elecθ +1 mrad | 0.46 / 0.52% | 0.24 / 0.26% | 0.55 / 0.59% | −0.07 / −0.17 |
| elecθ −1 mrad | 0.43 / 0.48% | 0.40 / 0.42% | 0.74 / 0.75% | −0.11 / −0.18 |
| hfs_jes_down | 0.31 / 0.35% | 0.43 / 0.44% | 0.38 / 0.40% | −0.55 / −0.52 |

Values are h+ / h−; "both" ≈ MC-only + data-only to 0.001–0.3%.

## Checklist
- [ ] D2: add the E′_shift > 11 GeV cut to both shifted builders, then re-run the zero bit-identity gate.
- [ ] D1: leg-vector step = MC-only (D nominal, M_leg), with data-only as a cross-check. Apply to budget and covariance for electron AND HFS. Get a user decision on the ±1% E′ magnitude before the budget is regenerated.
- [ ] D4: MC Stage-2 xF input identified or bounded.
