# Coherent-prior IBU-native uncertainties: ibu_stat + ibu_reg, mixed-basis final table

**Study date:** 2026-10-07. **Status:** PROVISIONAL INTERNAL DIAGNOSTIC — read-only against all
central products (migration ROOT sha256 `53234e21…cab6019` verified identical before/after every
run); raw/no-bgsub, no `C_rad`, not Born level. No central product modified; nothing here is
physics-approved.

**Study directory:** `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/coherent_prior_ibu_uncertainties_z005_20261007/` (README in `ibu_uncertainties/`)
**Summary JSON:** `…/ibu_uncertainties/summary_coherent_ibu_uncertainties.json`
**Final table:** `…/ibu_uncertainties/coherent_prior_final_results_mixedbasis_2026-10-07.csv` (490 rows, 38 columns, plotting-ready)
**Basis:** validated coherent-prior fork study `coherent_prior_ibu_z005_20261005` (report:
`docs/unfolding/coherent_prior_fix_results_2026-10-05.md`) — prior := response truth marginal
`body+miss` in both IBU roles, exact release IBU equations, k=4, L=351000 nb^-1. The IBU kernel is
the fork's, with the iteration count generalized; no machinery reinvented.
**Scope (user-approved):** IBU-native components only (`ibu_stat`, `ibu_reg`); detector/systematic
sources stay on the existing old-prior basis. **Execution:** NAF login node, ~1 s per stage
(200 toy chains ≈ 1 s in-process) — far under the 5-min interactive cap; no Condor needed.

## 1. Definitions

* **`ibu_reg` (NEW, coherent basis, SIGNED):** `d_reg = (N[k=5] − N[k=3])/2 / N[k=4]`, coherent
  prior, central data, both charges. Quadrature uses `|d_reg|`.
* **`ibu_reg` (OLD, release):** `0.5·|IBU_plain(k=5) − IBU_plain(k=3)|`, unsigned — reproduced by
  this study's plain-prior arms to `9.3e-12` (definition confirmed, not assumed).
* **`ibu_stat` (NEW, coherent basis):** N=100 Poisson-replica unfolds per charge through
  coherent-prior IBU k=4; per-bin relative uncertainty = sample std (ddof=1) / coherent `N[k=4]`.
  Toy convention = old-basis release convention (`ibu_toy_covariance_v4`): `d_toy ~
  Poisson(round(d_i))` per physical reco bin on the fake-NOT-subtracted measured vector, fixed row
  fake fraction, same response/prior, fixed k. Per-toy RNG `default_rng([seed, toy_id])` (seeds
  pos `20261007`, neg `20261008`) — batch-position independent, so restarts resume bit-identically;
  outputs persisted per batch of 10.
* **`total_mixedbasis_prov_rel`:** `sqrt(others_oldbasis² + ibu_stat_coherent² + ibu_reg_coherent²)`,
  where `others_oldbasis` is the quadrature of the 9 old-basis non-IBU-native sources
  (ibu_model, hfs_jes, hfs_rces, hfs_theta, elec_energy, elec_theta, lumi 2.7%,
  tracking_eff_m0p5, phi_h_gauss).

**MIXED-BASIS CAVEAT (explicit):** `total_mixedbasis_prov_rel` combines NEW coherent-basis
IBU-native components with OLD-basis detector/systematic sources and is **not a homogeneous
uncertainty budget**; the old-basis relative components are defined against the old plain-prior
nominal cross sections. It is provisional pending the coherent re-release that must regenerate
the downstream detector/systematic chain.

## 2. Validation gates — all PASS

| gate | result |
|---|---|
| Convergence/machinery | coherent k=4 reproduces fork study to `1.2e-15`; plain k=4 reproduces central release to `3.3e-15`; old-basis reg definition reproduced to `9.3e-12`; toy-mean/central median `1.00002` (h+) / `1.00013` (h−) |
| Rows | 490 (245 h+ then 245 h−, charge-major) in every product |
| Finite | all k=3/4/5 yields, stat/reg columns, totals finite |
| Negative cross sections | 0 (central unfolds and final table) |
| Toys | 100/100 valid per charge (finite, non-negative) |
| Determinism | rerun of toys 0 and 57, both charges: **bit-identical** (`np.array_equal`) |
| Central integrity | ROOT sha256 unchanged before/after; all outputs under `output_v6/studies/` |

## 3. Prior-invariance finding (exact, both components)

For `p_init = p_eff = P` the IBU output factors as `N_k[j] = P[j]·q_k[j]` with `q_k`
prior-independent (first-iteration smearing `A·n_0 = B` exactly; inductively `A·n_k = B·q_k`).
Therefore the per-bin **relative** `ibu_stat` (same toys) and `ibu_reg` are exactly prior-basis
invariant. Verified directly: unfolding the same 100 toys with the plain prior gives relative
stat agreeing with the coherent result to `≤1.4e-16` (h+) / `≤1.1e-16` (h−); the reg half-spreads
agree to `≤5.5e-12`. Consequences: (i) the coherent-prior switch changes cross sections (~1%
median) but **not** their relative IBU-native uncertainties; (ii) any new-vs-old difference in
`ibu_stat` is toy resampling, not prior physics.

## 4. ibu_stat, old vs new (old = central release 1000-rep column)

| charge | NEW median / max | OLD median / max | new/old ratio median (range) |
|---|---|---|---|
| h+ | **3.568% / 31.44%** | 3.590% / 31.34% | 0.988 (0.722–1.191) |
| h− | **3.819% / 57.55%** | 3.831% / 55.70% | 1.004 (0.790–1.194) |

Magnitudes cross-check: medians agree within ±1.2%. The per-bin scatter (up to ±20–28% in the
lowest-yield high-z pT-edge bins) is the expected sampling noise of a 100-toy std estimate and
matches the historical 100↔1000-rep comparison pattern (2026-06-14 audit: per-bin deltas up to
~20%, median 0.1%). Max-stat bins are the smallest cross sections (h− bin 244, S3_x0, z=0.7–1,
pT=4–8: 57.6% at xs = 2.2e-05 nb).

## 5. ibu_reg, old vs new (SIGNED vs ABS basis change explicit)

| charge | NEW \|signed\| median / max | OLD (unsigned) median / max | \|new/old\| ratio |
|---|---|---|---|
| h+ | **0.577% / 29.77%** | 0.577% / 29.77% | 1.000000 (dev ≤ 5.5e-12) |
| h− | **0.566% / 13.64%** | 0.566% / 13.64% | 1.000000 (dev ≤ 8e-12) |

The magnitudes are **identical** on the two prior bases (exact cancellation, §3). The basis
change is conventional only: the new column is **signed** — h+ min/median/max
`−29.77% / −0.059% / +14.95%`, h− `−13.64% / −0.070% / +7.67%` — while the release stores the
unsigned half-spread. The extremes sit in low-yield edge bins (h+ bin 179, S2_x1, z<0.05,
pT=4–8: −29.8% at xs = 8.2e-05 nb, where stat is 8.0%).

## 6. Mixed-basis provisional totals

| charge | OLD total (`total_with_phih_rel`) median | MIXED (`total_mixedbasis_prov_rel`) median | per-bin delta median (range) |
|---|---|---|---|
| h+ | 6.047% | **6.130%** | −0.005 pp (−3.02 … +1.80 pp) |
| h− | 6.389% | **6.296%** | +0.011 pp (−5.82 … +4.55 pp) |

Changes are small and dominated by which toy set populates `ibu_stat` (the old budget total
embeds the 100-rep stat lineage — see §7). `others_oldbasis` reconstructs the exact complement
of the budget's IBU-native pair inside `total_with_phih_rel` (quadrature identity to `2.4e-15`),
and the mixed-total identity was re-verified bin-by-bin in the written table (0 violations).

## 7. Pre-existing budget lineage finding (not introduced here)

The adopted floored budget's `ibu_stat_rel` column (from
`total_uncertainty_z005_2026-07-03_with_tracking`, continued by the 2026-10-03 floor) matches the
**100-rep backup** release (`_backup_100rep_stat_2026-06-14`) exactly (245/245, ≤1e-12), while the
current central release `stat_unc_nb` carries the **1000-rep** update (2026-06-14, cluster
3692701). Max deviation 0.0345 (h+) / 0.0624 (h−) absolute in relative units, concentrated in
low-yield high-z bins. This study's OLD-basis comparison columns use the **central release**
values; `total_with_phih_rel_oldbasis` (kept as a reference column) still embeds the budget's
100-rep stat. Flagged for the budget owners; not resolved here (out of scope).

## 8. Are the coherent-prior shifts covered by the updated uncertainties?

Yes. The coherent-prior cross-section shifts (vs plain-prior baseline: median 1.03%/1.10%,
max 3.58%/3.46%, integrated +0.128%/+0.120%) are below `total_mixedbasis_prov_rel` in **all 490
bins**: median |shift|/total = 11.7% (h+) / 10.9% (h−), max 78.7% / 73.8% (0 bins with
|shift| > total). The shift exceeds the IBU-native stat+reg quadrature alone in 33 (h+) / 34 (h−)
bins — expected, since it is a prior/efficiency effect, not a data-statistical fluctuation; it is
covered by the model/detector-dominated total.

## 9. Artifacts

All under the study directory (see `ibu_uncertainties/README.md`): `ibu_reg_coherent.csv`,
`toys/toys_{pos,neg}_batch{00..09}.csv` (incremental, resume-safe), `ibu_stat_coherent.csv`,
`coherent_prior_final_results_mixedbasis_2026-10-07.csv`, `stage{1,2,3}_gates.json`,
`summary_coherent_ibu_uncertainties.json`, `PROGRESS.md`, driver
`run_coherent_ibu_uncertainties.py`. Integrated coherent cross sections re-confirmed:
h+ `11.483372` nb, h− `11.190383` nb, combined `22.673755` nb (fork values).
