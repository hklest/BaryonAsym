# v6/z005 covariance package v4 — 2026-09-23

- Plotting script: `plotting/plot_covariance_z005_2026-09-23.py` in the canonical bundle.
- Plots: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/covariance/z005_2026-09-23/` (17 figures; review PDF `covariance_z005_review_2026-09-23.pdf`).
- Package: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/`
- Supplement (model/reg rank-1): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/covariance/z005_supplement_modelreg_2026-09-23/`
- Status: ACCEPTED-PROVISIONAL

## Product

The provisional global-fit covariance package is at:

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/`

It contains 490-bin float64 absolute covariance matrices in nb², corresponding fractional matrices (`rel_*.npy`), and three total correlation matrices. The bin ordering is exactly 245 h+ bins followed by 245 h- bins; `bin_map.csv` is the alignment authority through `(charge, bin_index)`.

`cov_total_all_diagonalB.npy` is the recommended **provisional** working total. `cov_total_all_rank1B.npy` is an unvalidated upper-bound correlation scenario, not a recommended nominal covariance.

## v1 → v4 history

- **v1:** first audit package; superseded because its absolute basis predated the repaired release consistency checks.
- **v2:** audit iteration; superseded after further packaging and validation refinements.
- **v3:** established source/tier structure but built while the release and total-summary absolute fields had reverted to the stale `297657.8 nb^-1` basis. Its relative source matrices remain audit-relevant where they derive only from invariant fractional inputs, but it fails the repaired-basis statistical closure.
- **v4:** rebuild on the authoritative `L_data=351000.0 nb^-1` four-period S67 basis. v1/v2/v3 remain untouched as audit history and are superseded for use.

## Luminosity regression and repair

The 2026-06-11 rescale correctly converted nb-space release fields from `297657.8` to `351000.0 nb^-1`, preserving yield-space and relative quantities. A 2026-06-14 producer rerun for the 1000-toy update retained a stale hardcoded `LUMI=297657.8` and silently overwrote the corrected nb payload. The 2026-09-23 repair restored the authoritative basis: nominal rows now agree with the release exactly, while yield-space content, relative fields, and the 1000-toy provenance remain intact.

The toy covariance inputs are yield² and byte-identical through the repair. v4 transports them by `1/(351000 nb^-1)^2`. Thus the correct test is not a hand rescale of the toy covariance but closure against repaired `stat_unc_nb/cross_section_nb`.

## Corrected validation gates and closure

- **S1:** `sqrt(diag(cov_stat))/sigma` versus repaired release `stat_unc_nb/cross_section_nb`: maximum relative difference `5.26076954427986e-16` (PASS; requirement `<1e-10`).
- **T:** tracking diagonal versus `tracking_eff_m0p5_rel`: maximum relative difference `7.63370254364098e-13` (PASS; requirement `<1%`).
- **X:** `cov_total_all_diagonalB` checked against the repaired historical total CSV after replacing v4's 1000-toy statistical diagonal with historical 100-toy `ibu_stat_rel`: maximum relative difference `3.4285237610270342e-15`, 0/490 beyond 1% (PASS).
- **S2:** 1000-toy versus 100-toy comparison assessed statistically with `sigma_r=0.074505`: 0 bins beyond `3 sigma_r`; bin-index correlation `0.0041873`, so no kinematic structure (PASS). The roughly 19.6% endpoint spread is benign finite-toy variation; 100-toy subsampling gave the established `21.66% ± 2.10%` behavior.
- **ALIGN:** all input artifacts align on `(charge, bin_index)` (PASS).
- **REGRESS:** luminosity-invariant source relative matrices agree with v3 to maximum `5.551115123125783e-17` (floating precision). v3 `rel_cov_stat` and totals are explicitly excluded because v3 transported toys with the stale basis and therefore differs by `0.02258265143759966`; this is the expected diagnosed v3 defect, not a new source variation.
- **Matrix integrity:** all emitted covariance matrices are `(490,490)`, finite, symmetric to zero numerical difference, non-negative diagonal, and PSD within the documented numerical tolerance. All three totals are PSD.
- No numerical input payload used in the build carries `297657.8`; historical provenance references are not numerical inputs.

## Source treatment

Tier A: 1000-toy data statistics, rank-one 2.7% luminosity, and charge-independent diagonal tracking. The h+/h- statistical cross-block is **ZERO BY ASSUMPTION**, unmeasured.

Tier B: HFS JES, HFS RCES, HFS theta are one-sided provisional vectors; electron energy and electron theta are provisional post-hoc vectors. Both rank-one and diagonal forms are supplied, without choosing a correlation model.

Tier C: model and regularization are diagonal fallbacks from `model_unc_nb/cross_section_nb` and `reg_unc_nb/cross_section_nb`. Response-MC-statistical covariance is **ABSENT**; no matrix is emitted.

The two-sided HFS-theta construction is quarantined: audited signed means are +0.25681140% (up) and +0.25880271% (down), an even/quadratic response. Its rank-one and diagonal alternatives are outside all totals. Centered 16-seed tracking scatter is retained only as `qa_only_cov_tracking_seedscatter.npy` and is excluded from all totals.

## Caveats and prioritized follow-up

1. Produce response-MC-statistical replicas and the full 490-bin covariance.
2. Run paired shifted-response reruns and noise-floor/migration-coherence validation for Tier-B detector sources; choose a validated correlation model only then.
3. Replace the electron +1% post-hoc vector with the frozen ±0.5%-per-LAr-wheel prescription. Electron energy is about twice the frozen magnitude; both electron sources lack paired shifted-response reruns.
4. Resolve the IBU/BBB versus native-v6 TUnfold nominal method decision.
5. Complete C_rad provenance, sparse-bin policy, and method review; it remains unapplied.
6. Measure charge-dependent tracking before any `A_h` publication; the current charge-independent tracking covariance is insufficient for that observable.

This package is therefore **ACCEPTED-PROVISIONAL** for covariance-interface work only, not a final physics-release covariance.
