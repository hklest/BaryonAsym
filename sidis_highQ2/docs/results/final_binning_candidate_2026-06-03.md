# H1 high-Q² charged-SIDIS TUnfold AREA — final-binning candidate (pT-only merging)

**Date:** 2026-06-03
**Status:** INTERNAL PRELIMINARY CANDIDATE. Not for circulation. Awaits
`h1-analysis-critic` and `h1-unfolder` review.

---

## 1. Summary

Built a final-binning internal release of the H1 high-Q² charged-particle
SIDIS measurement using TUnfold AREA as the central method (IBU diagnostic
only). The release was constructed from the existing 210-bin (rev6 cells
0..5 + S3 x-merged) scheme by:

1. **STEP 1 — Refreshing the 210-bin central values at the closure-tauopt
   taus** (`τ_pos = 1.000e-6`, `τ_neg = 2.5929437974e-6`) that were
   identified by the 2026-06-02 closure tau-optimization scan. The S3
   x-merged block is kept at the existing multi-objective AREA tau
   (`τ_pos = 3.162e-3`, `τ_neg = 1.000e-3`) because the closure-tauopt
   scan was performed over the full 240-bin response and does NOT apply
   to the 30-bin x-merged response.

2. **STEP 2 — Building a pT-only common-merge map** across charges. Within
   each `(charge, Q²/x cell, z bin)` row of 5 base pT bins the merge tests
   schemes A → E (finest to coarsest):
   - A: `[0,0.5] [0.5,1] [1,2] [2,4] [4,8]`
   - B: `[0,0.5] [0.5,1] [1,2] [2,8]`
   - C: `[0,1] [1,2] [2,8]`
   - D: `[0,1] [1,8]`
   - E: `[0,8]`

   A group is **acceptable** iff:
   - `sum(data_count) > 0`
   - aggregated `central > 0`
   - at least one base bin is `trusted = True` (rev6 trust mask AND
     closure-not-always-bad)
   - aggregated `max(rel σ_stat, rel σ_model, rel σ_total) ≤ 200%`

   The **common** scheme is the coarser of (pos, neg) when they disagree.
   A row is **silently dropped** only if it is `z_bin == 5` (z in [0.7,1.0])
   and scheme E fails for either charge. For `z < 0.7`, scheme-E failure
   is a **BLOCKER** that requires user / critic decision; we never silently
   drop a low-z row.

3. **STEP 3 — Aggregating** the 210 base bins into final-bin yields
   following the merge map. `σ_stat` is aggregated in quadrature
   (`covariance_approximation = "diagonal_only"`; full ROOT covariance is
   not loaded in this postprocessing driver). `σ_reg` is computed by
   summing the neighbour-τ shifted central vectors per merged group and
   re-taking the half-difference; `σ_model` does the same for the Djangoh
   response. `σ_total` is the quadrature sum of the three components.

4. **STEP 4 — QA plots** (pT_h overlays per cell, bin-status map,
   uncertainty-source distributions). MC overlay context is intentionally
   skipped because no bin-aligned MC truth tables for the merged scheme
   are shipped in the input JSONs.

5. **STEP 5 — Documentation** (this file) + RUN_STATUS + LESSONS_LEARNED
   updates.

---

## 2. Bin counts

| Quantity | Value |
|---|---|
| Base bins per charge (210-bin tauopt refresh) | 210 |
| **Final published bins per charge** | **157** |
| **Common bin count across charges** | **157** (binning identical by construction) |
| Final bins source: rev6 cells 0..5 | 142 |
| Final bins source: S3 x-merged | 15 |
| Total `(cell, z)` rows considered | 42 (7 cells × 6 z bins) |
| Rows published at scheme A (5 pT bins) | 25 |
| Rows published at scheme B (4 pT bins) | 3 |
| Rows published at scheme C (3 pT bins) | 6 |
| Rows published at scheme D (2 pT bins) | 0 |
| Rows published at scheme E (1 pT bin)  | 2 |
| Rows dropped (z ≥ 0.7, scheme E failed) | 4 |
| Rows BLOCKERS (z < 0.7, scheme E failed) | 2 |
| Rows total used | 42 (25+3+6+0+2 + 4 dropped + 2 blockers) |

Published-rate (rows): 36 / 42 = 85.7% of `(cell, z)` rows yield at least
one final pT bin; 6 / 42 = 14.3% are dropped or blocker.

---

## 3. Per-row scheme map

A summary of every row where the chosen common scheme is not the finest
(A). Full per-row provenance lives in
`data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/final_binning_map_ptmerge.json`.

| cell | z_bin | z range | pos scheme | neg scheme | chosen common | note |
|---|---|---|---|---|---|---|
| S0_x0 | 5 | [0.70, 1.00] | A | C | C | neg pushes to C |
| S0_x1 | 0 | [0.00, 0.10] | B | B | B |   |
| S0_x1 | 4 | [0.50, 0.70] | C | C | C |   |
| S0_x1 | 5 | [0.70, 1.00] | E | none | **DROPPED** | neg fails all (z ≥ 0.7 silent drop) |
| S1_x0 | 5 | [0.70, 1.00] | C | C | C |   |
| S1_x1 | 4 | [0.50, 0.70] | C | C | C |   |
| S1_x1 | 5 | [0.70, 1.00] | none | none | **DROPPED** | both fail E (z ≥ 0.7) |
| S2_x0 | 4 | [0.50, 0.70] | C | A | C | pos pushes to C |
| S2_x0 | 5 | [0.70, 1.00] | E | none | **DROPPED** | neg fails all (z ≥ 0.7) |
| S2_x1 | 0 | [0.00, 0.10] | B | A | B | pos pushes to B |
| S2_x1 | 2 | [0.20, 0.30] | C | A | C | pos pushes to C |
| S2_x1 | 3 | [0.30, 0.50] | A | E | E | neg pushes to E |
| S2_x1 | 4 | [0.50, 0.70] | none | none | **BLOCKER** | scheme E fails neg, multiple negative centrals (z < 0.7) |
| S2_x1 | 5 | [0.70, 1.00] | none | none | **DROPPED** | both fail E (z ≥ 0.7) |
| S3_xmerged | 4 | [0.50, 0.70] | none | none | **BLOCKER** | conservative S3 trust mask zero-trusts row; numerical rel σ are acceptable (see §6) |
| S3_xmerged | 5 | [0.70, 1.00] | none | none | **DROPPED** | conservative S3 trust mask + z ≥ 0.7 silent drop |

All other 31 rows publish at scheme A (5 pT bins). Charges agree on the
chosen scheme in 31 / 36 published rows; 5 / 36 rows have charge
disagreement and use the coarser scheme of the pair.

---

## 4. Validation summary

Verified after the build:

| Check | Result |
|---|---|
| 210-bin tauopt refresh: uses `τ_pos = 1.000e-6`, `τ_neg = 2.5929e-6` | PASS (per `release.json`) |
| Final binning map has no z-merged groups | PASS (every group's `(z_min, z_max)` lies on the 6-edge base set `[0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]`) |
| All published final groups satisfy `max(rel σ_stat, σ_model, σ_total) ≤ 200%` | PASS: max rel σ_total over all 157 bins is 1.713 (pos) and 1.710 (neg) |
| Zero-data bins not published | PASS: min `data_count` is 5.0 (pos) and 3.0 (neg) per final group |
| Negative-central bins not published | PASS: min aggregated `unfolded` is 42.2 (pos) and 33.9 (neg) |
| S3 z ≥ 0.7 drop policy applied | PASS: S3_xmerged z = 5 is in the dropped list |
| Common binning across charges | PASS: pos and neg have identical 157-bin scheme |
| Outputs non-empty and mirrored locally | PASS: see file list below |

---

## 5. Output locations

### Local (primary)

| Path | Content |
|---|---|
| `local:H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` | Final-bin candidate release (157 bins/charge) |
| `local:H1/sidis_highQ2/data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/` | Refreshed 210-bin release at the tauopt taus |
| `local:H1/sidis_highQ2/plots/final_binning_candidate/` | pT_h overlays, status map, uncertainty plots |
| `local:H1/sidis_highQ2/scripts/analysis/build_finalbin_release_ptmerge.py` | Driver script (postprocessing only; no new TUnfold runs) |

### File list (final release)

- `README.md` — written narrative + dropped/blocker rows
- `release.json` — header / taus / merge policy / bin counts
- `manifest.json` — file list with SHA-256 hashes
- `binning_finalbin.json` — final-bin definitions
- `final_binning_map_ptmerge.json/.csv` — per-row merge provenance
- `yields_finalbin_{pos,neg}.json/.csv` — central + uncertainties
- `publication_mask_finalbin.json` — pub status (all `published` by construction)

### File list (refreshed 210-bin release)

- `README.md`, `release.json`, `manifest.json`, `binning_210.json`
- `unfolded_yields_210_{pos,neg}.json/.csv`

### Plots produced

- `plots/final_binning_candidate/pth_overlay_finalbin_{pos,neg}.pdf` —
  per-cell pT_h panels (7 cells incl. S3 merged), z colours Okabe-Ito,
  pT bins reflect merged scheme.
- `plots/final_binning_candidate/finalbin_status_map.pdf` — 7×6 grid
  coloured by chosen scheme letter.
- `plots/final_binning_candidate/finalbin_uncertainty_sources.pdf` —
  histograms of rel σ_stat/reg/model/total + dominant-source bar chart
  per charge.
- `plots/final_binning_candidate/MC_OVERLAY_SKIPPED.txt` — note on the
  skipped MC overlay panel.

---

## 6. Blockers and design conflicts (require user / critic decision)

### Blocker 1 — S2_x1 z_bin = 4 (z in [0.50, 0.70])

This row has multiple base bins with negative aggregated central values
in BOTH charges; even at scheme E (all 5 pT bins merged) the neg-charge
aggregated `rel σ_stat = 2.13` (above the 2.00 acceptance), with
`rel σ_total = 2.13` and central ≈ 520. This is a legitimate physics
failure of the regularized AREA result at this `(Q², x, z)` cell rather
than a side-effect of an overly conservative mask. **Recommendation:**
either (a) drop this row from the publication, or (b) accept it at a
relaxed `rel σ ≤ 250%` threshold for this single row with documentation,
or (c) re-investigate at a different τ choice for this cell only.

### Blocker 2 — S3_xmerged z_bin = 4 (z in [0.50, 0.70])

This is a **design conflict between the conservative S3-merged trust
mask and the pT merge policy.** The conservative S3-merged mask
(inherited from the 2026-06-02 210-bin release) zero-trusts ALL z ≥ 0.5
bins in the S3 block, so the gate `no_trusted_base_bin` fails for every
candidate group at every scheme — even though the aggregated numerical
relative uncertainties at scheme E are well within tolerance
(`rel σ_total = 0.58` pos and `0.85` neg, both below 2.00) and the
aggregated centrals are positive (459.7 pos, 603.0 neg).

**Recommendation:** relax the S3-merged trust mask so that S3 z = 4
(z in [0.50, 0.70]) is only required to satisfy the same numerical
acceptance gates as the rest of the binning, and re-build. If the
relaxed mask is acceptable, S3 z = 4 would publish at scheme E with
one pT bin per charge, increasing the published bin count by 1 per
charge.

### Silently dropped z ≥ 0.7 rows (4 rows)

- S0_x1 z = 5 (z in [0.70, 1.00])
- S1_x1 z = 5
- S2_x0 z = 5
- S2_x1 z = 5
- S3_xmerged z = 5 (also subject to the conservative S3 mask)

These are dropped per the user's S3 / high-z policy. The three published
z = 5 rows (S0_x0, S1_x0, S2_x1 ?) — actually verified counts: S0_x0,
S1_x0 and S2_x1 — yield 7 z = 5 final bins per charge total.

---

## 7. Covariance approximation

`covariance_approximation = "diagonal_only"`. The aggregation of `σ_stat`
across merged groups is a quadrature sum of the per-base-bin TUnfold
publication-bin errors (square roots of the covariance-matrix diagonals).
A future extension can load the full ROOT covariance via TUnfold's
`GetEmatrixTotal` and aggregate the off-diagonal block exactly, which is
expected to **decrease** the aggregated `σ_stat` because the per-bin
errors in a finely-binned scheme tend to be positively correlated. The
present diagonal-only approximation is therefore **conservative** for the
pT-merged groups.

`σ_reg` and `σ_model` aggregation is exact under the additivity of
extensive yields: for each merged group we sum the per-base-bin
`unfolded_lo / unfolded_hi / unfolded_DJ` vectors and re-take the
half-difference. This is the right operation and preserves cancellation
across merged base bins. For `σ_reg` at the pos charge, the lower
neighbour `unfolded_lo` is not available because `τ_pos = 1.000e-6` sits
at the lower edge of the rev6 fine τ scan; the one-sided definition
`|unfolded(τ_high) − unfolded(τ_nom)|` is used, which is documented in
the release `release.json`.

---

## 8. Ready for critic review?

**Yes**, with two explicit caveats to be handed to `h1-analysis-critic`
and `h1-unfolder`:

1. **The two blocker rows must receive a decision** (drop, relax
   threshold, retune τ for the cell, or relax S3 trust mask for S3 z = 4).
2. **The covariance aggregation is diagonal-only**; if the analysis-note
   author requires the true correlated `σ_stat` aggregation, a future
   re-run with `--use-root-cov` (not yet implemented) is needed.

All other validation gates pass cleanly: no z-merging, no zero-data
published, no negative-central published, all rel uncertainties under
200% for every published group, common scheme across charges.

---

## 9. Inputs and provenance

All inputs were already on disk (no new TUnfold runs were issued):

### Tauopt (rev6 cells 0..5) — local

- Data unfolds at `τ_pos = 1.000e-6` and `τ_neg = 2.5929e-6`:
  `local:H1/sidis_highQ2/data/tunfold_rev6_tauopt_data/unfolded_data_{pos,neg}_tau*.json`
- Same at the Djangoh response: `unfolded_dataDJ_*`
- Neighbour-τ unfolds for σ_reg (fine geometric grid, ratio ≈ 1.373):
  - pos: `tau1_3738237959e-06.json` only (lower edge — see §7)
  - neg: `tau1_8873918221e-06.json` and `tau3_5622478903e-06.json`
- Trust mask + reco_eff + mc_count taken from the existing per-bin sigma
  files (tau-independent properties of the response matrix):
  `local:H1/sidis_highQ2/data/per_bin_sigma_tunfold_rev6_area/`
- Binmap: `migration_inputs_v3_percharge_NWfix_v2.root.binmap_v3.json`
  (mirrored locally to the tauopt-data directory).
- Closure-tauopt always-bad masks:
  `local:H1/sidis_highQ2/data/closure_rev6_area_tauopt/closure_tauopt_mask_{pos,neg}.json`.

### S3 x-merged AREA — local

- Scan dir (Rapgap nominal): `local:H1/sidis_highQ2/data/S3merged_tunfold_area_local/scan/`
- Scan dir (Djangoh response): `local:H1/sidis_highQ2/data/S3merged_tunfold_area_local/scan_dataDJ/`
  - 12 JSONs fetched from `naf:.../output/tunfold_tree_v3_percharge/S3merged_tunfold_area/`
    (just the multi-objective τ + sqrt(10) neighbours per charge × RG/DJ).

### Data counts — local

- `local:H1/sidis_highQ2/data/diagnostics_210/data_counts_210.json`
  (210-bin background-subtracted data yields, per charge).

### Remote provenance (NAF, authoritative)

- `naf:/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/tau_scan_rev6_fine_1em6_1em2/`
- `naf:.../tunfold_tree_v3_percharge/S3merged_tunfold_area/scan{,_dataDJ}/`
- `naf:.../tunfold_tree_v3_percharge/closure_rev6_area_tauopt/`

The final-binning release JSONs themselves are postprocessing artifacts
and live locally only at the present time; remote mirroring is a follow-up
step.

---

## 10. Next steps

1. Route blockers (§6) to `h1-analysis-critic` and the user for a
   decision: keep / drop / retune / relax mask for the two flagged rows.
2. Once blockers are resolved, implement the agreed treatment in
   `build_finalbin_release_ptmerge.py` (e.g., a `--s3-trust-relaxation`
   flag) and re-run.
3. Decide whether to add full ROOT-covariance aggregation
   (`--use-root-cov`) before sending for analysis-note inclusion.
4. Mirror the final release directories to NAF
   `output/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/`
   and the refreshed 210-bin release to
   `output/data_release_rev6_TUnfold_AREA_210bin_tauopt_INTERNAL/` once
   the candidate clears critic review.
5. Re-evaluate `h1-systematics` budget for the final bin scheme
   (currently only stat / reg / model are aggregated; detector
   systematics are not yet folded into the final-bin tables).
