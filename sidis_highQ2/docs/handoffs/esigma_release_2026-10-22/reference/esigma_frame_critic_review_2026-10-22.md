# eΣ reco-photon frame: second critic round (2026-10-22)

The user asked: "make sure that we're not forgetting something". Two independent read-only critics reviewed the work:
one on physics, radiative corrections and the observable definition, one on analysis-chain completeness. Neither was
given the author's list of concerns.

## Verdicts
- **Physics: PROCEED-WITH-CONDITIONS.** The chain "reco eΣ → unfold to the gen target → C_rad → Born" is self-consistent.
  **C_rad needs no rerun, and the gen target should NOT change.**
  - The target is a radiative-level hybrid: z, pT and Q² use the nominal-beam lepton photon; y and x are the true values.
  - For ISR events, the eΣ reco z matches this target well:
    - f = E_γ/E_e > 0.05: median ln(z_eΣ/zGen) = +0.004, against −0.29 versus the true photon;
    - z-bin agreement is 0.915 with zGen and 0.719 with z_true.
  - A true-photon target would make the response worse, and would cost a C_rad rerun (4,730 jobs) plus the full ~4,300.
- **Chain: NOT-YET.**
  - The production and re-pointing are clean, and purity improves.
  - But 3–4 budget terms are not truly in the eΣ frame, several frame-keyed rules break silently, and covariance and ratios
    do not exist yet.
  - Nothing argues for reconsidering the switch.

## Fixed immediately
- **phi_h_gauss / tracking_eff hybrid.** This was a bug in `esph_downstream.step_groupB`. The response re-pointing happened
  before the module's `_old_pairing`, which therefore unfolded the old electron-frame vectors through the eΣ response
  (neg/240: 16% → 37%).
  - Both columns are now carried verbatim from the electron-frame final budget, which was the intended "carry".
  - phi_h still needs a native eΣ recompute before quoting.
- **bgsub_fakefrac** is now computed natively: the eΣ release f_i plus the eΣ OPT-A npz, 0 jobs. Before, it was carried.

## Open items before quoting

| # | item | source | cost |
|---|---|---|---|
| 1 | Φ_h native recompute: eΣ fork of the flat-φ reweight response builder | both | 174 shards + 1 IBU job |
| 2 | ISR / QED leg: response reweight × (1 ± κ) for GenEnPhoton/E_e > 0.01, with κ from data/MC (y_e/y_eΣ, the E−pz 45–52 GeV tail at low y) | physics B1, chain 7 | 174 RG shards + 1 job; **new term, user decision** |
| 3 | Σ_h resolution / LAr-noise proxy leg (z/pT now depend on Σ_h ≈ 5 GeV at low y; d ln z / d ln Σ_h = 2y − 1) | physics M1, chain 6 | 174–348 shards; **new term, user decision** |
| 4 | JES leg form: budget single leg vs covariance (up−down)/2 differ; leg looks MC-stat noisy (p95 1.1 → 3.4%) | chain 6 | 0 jobs (convention + significance test) |
| 5 | B1 floor recompute on the eΣ responses (it binds in ~60% of bins; old floor is pre-cone, electron-frame, status FAIL/PROVISIONAL) | chain 3 | ~100 light jobs |
| 6 | Validation: data/MC control plots (Σ_h, E−pz, y_eΣ/y_e, z_eΣ/z_e, hadron share of Σ_h at high z); closure (DJ through RG, half-sample); k-scan; S7 prior-reweight rerun; significance of the per-bin frame shifts (paired bootstrap via the s02a fork) | physics M2/M3, chain 5 | 1–few jobs each |
| 7 | Covariance in the eΣ frame: s02a (fork exists), s02b, s03, s08/s09, v5/NW editions; h+/h− and A_h columns. The edition gates are incremental, so the base package needs its own gates | chain 2 | s02a over 660 files + single jobs |
| 8 | Empty-D bin set changed ({pos/109, pos/179, neg/240} → none): EMPTY_D_BINS pin, M1 criterion, the pos/b179 exemption and the straddle/bgsub guards must be re-specified | chain 4 | records |
| 9 | m2_zshift "justified zero" rationale (relied on QED common mode) and the C_rad contract text: rewrite | physics M4, chain 7 | records |
| 10 | Provenance: eΣ ids in the release metadata and the merged response names; one promotion record covering all un-installed final-re-release changes + eΣ, with swap-one attribution; canonical edits (STATE, RUN_STATUS, current-release, ~6 skills); note text and figures (q definition, purity, closure, control plots) | chain 8 | records + plot reruns |
| 11 | Minor: xF(eΣ) vs xF(e) sign flips at z < 0.1; FSR/Compton and the HAT bare-electron definition; lowQ2 template z-shape between frames; ±0.5% E′ legs (S6 linearity FROZEN/FAILED, 348 shards); a per-track cross-check vs photon_options B; mislabelled groupC gate; weak gates documented | both | small |
| 12 | Parity with the low-Q² analysis (still electron photon, same 1/y lever arm) and the AziAsym convention | both | **user decision** |
