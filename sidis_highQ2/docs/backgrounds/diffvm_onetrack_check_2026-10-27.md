# DIFFVM ρ⁰ with one decay pion lost: topology and one-track control region (2026-10-27)

**Scope.** This is a check for the high-Q² charged-hadron SIDIS eΣ release. The data-tuned DIFFVM scale s = 0.080 ± 0.146 was fitted only in clean two-track events (`diffvm_datatuned_subtraction_2026-10-26.md`). This note asks four things: how much of DIFFVM's selected yield comes from events where only one ρ pion is selected, whether a one-track control region in data constrains s, whether the one-to-two-track ratio is robust, and whether the ±42% DIFFVM term needs a one-track part. Nothing installed was changed: not the note, the release or the budget.

Path shorthand: `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies`, `T` = `H/diffvm_onetrack_2026-10-27/truth`, `R` = `H/diffvm_onetrack_2026-10-27/cr`.

## Short answer

- **One-pion fraction.** 9.2 ± 0.6% of DIFFVM's 2196 selected tracks come from events where only one ρ pion is selected (201 tracks). The fraction is 11.0% for h⁺ and 7.2% for h⁻. In gen bin 30 it is 14.5 ± 3.1% (h⁺) and 4.1 ± 1.8% (h⁻). In the 7 bins with more than 10% DIFFVM, summed, it is 9.5 ± 1.2%.
- **Why the pion is lost.** In 65% of cases the pion is inside |η| < 1.6 and pT > 0.15 GeV but has no reconstructed track (MC tracking loss, median gen pT 2.8 GeV). In 26% it has a reco track that falls outside the z/pT grid (median gen pT ≈ 12 GeV). Only 6% are lost at |η| ≥ 1.6, and 3% below the pT cut. No track-quality cut removes one.
- **One-track control region.** The control region is built and fitted, but it cannot constrain s at the 0.1 level. The design sensitivity (Asimov, true s = 0.10) is σ_s = 0.66.
  - The data fit gives s = 3.2 (+1.4/−1.1 stat, ±2.4 syst) with χ²/ndf = 33/11. The 95% UL is 5.6 (stat) and 9.5 (with systematics).
  - The fit is driven by an excess at ΔE < 0 (19 data events, 3.8 RAPGAP; local, post-hoc Poisson p ≈ 2×10⁻⁸, MC stat only). Without the ΔE < 0 bins the fit sits at its lower bound (s = −4), with UL95 = 4.1.
  - The data favour neither the two-track s nor the world expectation. They are consistent with both and exclude neither.
- **Ratio robustness.** Under all 17 SDME, Q² and R(Q²) reweights, the overall one-pion fraction stays within 0.088–0.097 (±5%). For the 7 bins summed it moves by up to 12%, and for single bins by 12–56%, within their 20–100% stat errors. The decay angles therefore do not limit the extrapolation. The MC tracking-loss rate sets the ratio, and the reweights do not test it.
- **The ±42% term.** It is adequate and needs no separate one-track component. The ±42% scales the full DIFFVM template, which already contains the 9% one-pion part. If the one-pion part were doubled, the effective scale would rise by at most ×1.15 (h⁺ bin 30). At the Gaussian 95% limit that is 0.32 → 0.37, still below the 0.42 used. Inferred: the ΔE < 0 excess is unlikely to be elastic ρ⁰. Explaining it would need s ≈ 5, which is at least 12 times the two-track 95% UL.

## 1. DIFFVM one-pion topology (Q1)

**Method.** A new Condor truth pass over the 404 DIFFVM files (`T/scripts/ctl_onetrack.py`, cluster 5902059, 8 jobs) extends the SDME truth pass with the lost-pion kinematics and per-track failure flags. The merged output matches `H/diffvm_sdme_2026-10-26/truth/merged_truth_diffvm.npz` exactly in run, event, n_sel, flat980, charge and gen mother (`T/merge_validation.json`). The gen bin is the release `i_gen_inlined` of the matched gen particle.

**Classes.** Class 1 means one ρ pion is selected, class 2 means both are, and class 0 is other (fakes and events with three or more ρ-matched tracks). Raw counts are from `T/q1_fractions.csv`.

| set | N | class 1 | class 2 | f1 | class 1 with 1 / 2 / ≥3 loose tracks |
|---|---|---|---|---|---|
| all | 2196 | 201 | 1976 | 0.092 ± 0.006 | 109 / 72 / 20 |
| h⁺ | 1131 | 124 | 994 | 0.110 | 77 / 34 / 13 |
| h⁻ | 1065 | 77 | 982 | 0.072 | 32 / 38 / 7 |
| h⁺ bin 30 | 131 | 19 | 109 | 0.145 ± 0.031 | 17 / 2 / 0 |
| h⁻ bin 30 | 122 | 5 | 116 | 0.041 ± 0.018 | 3 / 1 / 1 |
| h⁺ 31 / 65 | 38 / 143 | 7 / 17 | 31 / 125 | 0.18 / 0.12 | |
| h⁻ 31 / 65 / 100 | 36 / 115 / 33 | 2 / 8 / 1 | 34 / 106 / 32 | 0.06 / 0.07 / 0.03 | |
| 7 bins summed | 618 | 59 | 553 | 0.095 ± 0.012 | 52 / 6 / 1 |

- Only 54% of class-1 tracks overall sit in events with exactly one loose track. In the 7 bins it is 52 of 59, and in h⁺ bin 30 it is 17 of 19. The one-loose-track region therefore captures most of the one-pion yield where it matters.
- The lost pion is π⁻ more often than π⁺ (110 against 66). This is why h⁺ carries more of the one-pion yield.

**Lost-pion reasons** (176 events, `T/q1_lost_pion.json`)

| reason | share |
|---|---|
| in acceptance, no reco track (92 of 114 are single-row events) | 65% |
| reco track outside the z/pT grid (gen pT ≈ 12 GeV, median) | 26% |
| gen \|η\| ≥ 1.6 | 6% |
| reco pT < 0.15 GeV | 3% |
| reco η cut | 1% |

**Reco topology** (`T/q1_topology.json`, median with 16–84% range)

| quantity | class 1 | class 2 |
|---|---|---|
| Σz of loose tracks | 0.89 (0.32–1.09) | 0.99 (0.93–1.05) |
| \|pT,e + pT,trk\| (GeV) | 1.74 (0.57–8.6) | 0.59 |
| E−pz, electron + tracks (GeV) | 52.6 (47.4–57.6) | 55.2 (53.6–56.7) |
| surviving-track pT (GeV) | 6.2 (0.74–11.6) | 6.3 |
| surviving-track η | −0.19 (−0.91–0.62) | −0.10 |

- In one-loose-track events the pT imbalance divided by the lost pion's gen pT is 0.98 (0.59–1.21). The imbalance is the missing pion.
- The surviving track typically has z ≈ 0.5–0.9, and the event has little other track activity. Activity outside the track is measured through ΔE = 2E_e·y_h − (E−pz)_track, with y_h from the HFS (§2).

## 2. One-track control region (Q2)

**Pass.** `R/scripts/onetrack_pass.py` ran on Condor (120 jobs plus a merge) over data (660 files), DIFFVM (404), rapgap31 (364) and djangoh14 (501).
- Per-file parity with the release builder `_select_and_bin` is asserted.
- The event-cut counts equal those of the two-track pass for all four samples, and the one-loose-track counts reproduce its n_loose = 1 bin (`R/pass/merge_summary.json`).
- Stored one-track events: data 10107, DIFFVM 118 (96.6% of the tracks have GenMotherPDG = 113), rapgap31 119786, djangoh14 160670.
- DIFFVM has 118 one-track events in total, which is 16 events at s = 1. This limits the sensitivity from the start.

**Region.** The region was fixed on MC only, before the data fit (`R/fit/region_scan_MConly.json`). It takes the release event cuts, exactly one loose track (primary, pT > 0.15 GeV, |η| < 1.6), that track release-selected, and z > 0.8. The control variable is ΔE (HFS E−pz outside the track) in 1 GeV bins from −2 to 12 GeV, with under- and overflow bins. DIFFVM peaks at ΔE ≈ 0, because at z > 0.8 the surviving track carries nearly all of the ρ⁰ E−pz.

**Yields at z > 0.8** (`R/fit/fit_results.json`)
- Data: 184 events.
- DIFFVM: 8.7 events at s = 1, 0.70 at s = 0.08 and 1.13 at s = 0.13. The purity is 4.7% at s = 1 and 0.4–0.6% at the expected s.
- SIDIS: 183.9 events at the inclusive normalisation and 157.4 at the fitted a.

**Fit.** The method is the two-track fit's (`fit_s.fit_core`, binned Poisson, Barlow–Beeston-lite, a free).

| variant | s | stat | UL95 (stat) | χ²/ndf |
|---|---|---|---|---|
| **nominal** (rapgap31, z > 0.8, ΔE) | **3.15** | +1.37/−1.06 | 5.6 | 33.2/11 |
| djangoh14 | 3.18 | +1.39/−1.07 | 5.7 | 27.2/11 |
| a fixed to inclusive | 2.46 | +1.05/−0.85 | 4.3 | 35.9/12 |
| z > 0.7 | 3.65 | +1.63/−1.22 | 6.6 | 27.9/11 |
| z > 0.9 | 4.60 | +2.41/−1.74 | n/a | 23.4/10 |
| coarse bins | 4.91 | +1.84/−1.42 | n/a | 6.9/5 |
| p_T,bal > 1 GeV | 2.77 | +1.72/−1.32 | 5.9 | 34.8/11 |

- The quadrature systematic is 2.38, so the UL95 with systematics is 9.5.
- Closure with 100 toys: injected s = 0.3 returns a mean of 0.23, and s = 0.1 returns 0.21. The pull RMS is 1.2–1.5, so the stat errors are underestimated at these counts.

**The ΔE < 0 excess** (post hoc, `R/fit/extras.json`)
- At z > 0.8 and ΔE < 0 there are 19 data events, against 3.8 ± 0.5 RAPGAP and 3.0 DIFFVM at s = 1. The p-value is local and post hoc, with MC stat only. In the window z = 0.5–0.8 at ΔE < 0 there are 0 data events, against 0.75 RAPGAP.
- Dropping ΔE < 0 puts the fit at its lower bound, s = −4 (UL95 4.1, χ²/ndf = 13.1/8).
- The 19 tracks fall in 15 reco bins, at most 1/13 of any bin. They have pT = 7–24 GeV, Q² = 150–1030 GeV², and either charge. Eight have HFS E−pz of only 4.5–6.8 GeV.

**Plot.** `R/plots/onetrack_dE_zgt0.8.{pdf,png}`: ΔE at z > 0.8 on a linear axis. It shows data, SIDIS at a = 0.035 (from the nominal s = 3.15 fit), DIFFVM at s = 0.10 (the world-data midpoint), and a dashed line for SIDIS + DIFFVM at s = 1.

## 3. Robustness of the one-to-two-track ratio (Q3)

The SDME weights from `H/diffvm_sdme_2026-10-26/reweight/weights_*.npz` (17 variants) were applied per truth event (`T/q3_summary.json`).

| set | central f1 (H1_full) | SDME max dev | Q² prod. max dev | R(Q²) max dev |
|---|---|---|---|---|
| all | 0.092 | +2.0% | +4.8% | −4.2% |
| h⁺ / h⁻ | 0.104 / 0.079 | +9.4% / −15.3% | +9.8% / −5.1% | +7.3% / −12.5% |
| bin 30, both charges | 0.100 | −13.7% | −5.3% | −6.7% |
| h⁺ bin 30 | 0.151 | −12.4% | −1.8% | −5.2% |
| 7 bins summed | 0.094 | +11.7% | +4.8% | +3.4% |

- The decay angles move the ratio by at most about 12% in the bin sums. That is far below ±42%, and below the ratio's own MC stat error (±12% in the 7 bins, ±21% in h⁺ bin 30).
- **Extrapolating the two-track s.** Both topologies come from the same events, so their ratio is fixed by MC acceptance and tracking efficiency. The angular part is now tested. The tracking part, mostly central pions of a few GeV lost without a track, depends on data/MC tracking efficiency. That is not measured here (close-pair merging or gen–reco matching could contribute), but even a factor of 2 is covered (§4).

## 4. Consequence for the ±42% term (Q4)

- The ±42% term is a rank-one scaling of the full DIFFVM template, so it already scales the one-pion part (9% overall, 15% in h⁺ bin 30) together with the two-track part.
- At s = 0.42, the whole one-pion DIFFVM yield in the release is 201 × 0.135913 × 0.42 ≈ 11.5 tracks, of which h⁺ bin 30 has about 1.1 tracks.
- A pessimistic case doubles the one-pion fraction through data/MC tracking. That raises the template by 9% overall and 15% in h⁺ bin 30. At the Gaussian 95% limit this is 0.32 × 1.15 ≈ 0.37, below 0.42. The term covers it through the gap between the Gaussian and the conservative linear-sum limits.
- The one-track data give no limit tighter than that (UL95 5.6–9.5), so they cannot replace or tighten the term.
- **Recommendation:** keep ±42% as is. Add no one-track component.
- The ΔE < 0 excess is outside the DIFFVM model. It is about 15 tracks spread over 15 reco bins, at most about 8% of a sparse bin and typically 1–3%. It is possibly related to the two-track high-mass excess (memory `decisions/2026-10-26-diffvm-high-mass-excess-no-study.md`), but it is unidentified. Whether to note it as such is a user call.

## Verified vs inferred

- **Verified (file-backed):**
  - all counts, fractions, reweight spreads, yields and fit results (paths above);
  - truth-pass and control-pass parity with the release selection;
  - the reco-bin spread of the 19 ΔE < 0 events.
- **Inferred:**
  - that the "no reco track" losses are tracking inefficiency (from single-row events);
  - that data/MC tracking differences stay within a factor of 2 (not measured);
  - that the ΔE < 0 excess is not elastic ρ⁰. This follows from the two-track limit and the fixed topology ratio. Its source is not identified.

## Open questions

- The ±42% term itself is still pending user confirmation (memory `decisions/2026-10-26-diffvm-not-subtracted-negligible-rho0.md`). This check does not change it.
- Should the ΔE < 0 one-track excess be mentioned in the note's background subsection alongside the high-mass excess?

## Outputs

- Truth: `T/scripts/{ctl_onetrack.py, merge_onetrack.py, analyze_onetrack.py, analyze_dR.py}`; `T/q1_*.json|csv`, `T/q3_*.json|csv`, `T/merge_validation.json`, `T/README.md`.
- Control region: `R/scripts/{onetrack_pass.py, merge_pass.py, choose_region.py, fit_onetrack.py, extras_and_plot.py}`; `R/pass/merged_*.npz`, `R/pass/merge_summary.json`; `R/fit/{region_scan_MConly.json, fit_results.json, extras.json, plot_inputs.json}`; `R/plots/onetrack_dE_zgt0.8.{pdf,png}`; `R/README.md`.
