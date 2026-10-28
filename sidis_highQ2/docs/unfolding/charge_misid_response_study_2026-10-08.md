# Charge-misID response-routing quantification (Critic-A A-F3) — 2026-10-08

**Status:** STUDY COMPLETE. Study-only artifacts; no central release, response
product, or canonical script was modified. This document closes the dispatched
quantification for finding **A-F3** ("gen-charge routing of matched rows",
`docs/critiques/critic-response-born-release_2026-10-08.md`, status OPEN at
dispatch).

**Release basis of all quotes below:** canonical RAW central release
`H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
(coherent `body+miss` prior, IBU `k=4`, `L_data=351000.0 nb^-1`), and its
Born-level child (per-bin `C_rad`, DJANGOH14 xFcut primary). Every cross-section
quote below states its level.

---

## 1. Finding under test

The production response builder `build_migration_th2_per_charge_v4.py` (F7
routing, lines 909–918) routes **matched** response rows by **GEN** charge
while the per-charge data vectors are **RECO**-charge selected. The per-charge
folding identity `d = B·n + F` is therefore violated at the charge-misID rate:

- the h+ data space contains gen−→reco+ tracks that the h+ body `B_pos` does
  not model (they were routed to `B_neg`), and
- `B_pos` contains gen+→reco− rows whose reco tracks are not in the h+ data.

Historical context: F7 was a **deliberate** fix (2026-05-31 chain audit,
finding C2P2 — `rev6_chain_audit_2026-05-31/ROUND2_ITERATION.md`,
`SYNTHESIS.md`) that made per-charge efficiency a pure geometric acceptance;
the same audit required the residual charge-confusion systematic to be
"quantified per bin from MC truth vs reco charge correlation". Pre-F7 the
charge-confused matched rows contaminated the fake column (noted by
`critic-ibu-1/2`, 2026-05-31); post-F7 they sit in the gen-charge body. This
study is that required quantification, at the current v6 release chain.

## 2. Inputs, method, and why a new scan script

- **MC input:** the complete v6 central RAPGAP31 sidistree set — 364 files,
  four periods (ep0304/em0405/em06/ep0607; reqIDs 7097/7096/7071/7094),
  ~50.8 GB, under
  `output_v6/central/sidistree/mc/rapgap31/{7097,7096,7071,7094}/`.
- **Response input (read-only):** central
  `migration_inputs_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.root`
  plus the four per-period `mig_v4_rapgap31_<PERIOD>.root` files.
- **Yield input (read-only):** release `ibu/cross_sections.csv`
  (`yield_unfolded`, 490 rows).
- **Selection:** inlined verbatim from the **production** v4 builder (v4
  fiducial contract: 150<Q2<20000 both legs, 0.1<y<0.7, Empz>45, xF<0 both
  legs, |η|<1.6, RadLength≥5, NHitDedx≥5, Pt>0.15, GenEmpz>45 per-event
  first-occurrence). The canonical-snapshot directory holds only v6/allfake
  variants with different, superseded broad-gen-envelope cuts; the v4
  production contract is what built the central release — confirmed by
  `meta_fiducial_contract` inside the central response ROOT.
- **Why a new scan script was required:** canonical builders accumulate only
  routing-charge-keyed response histograms; no canonical script records the
  per-row reco-vs-gen charge comparison, the off-diagonal X/Y matrices, or the
  misID kinematic profiles this study needs. Validation of the new script is
  by exact reproduction of the central response (Section 3).
- **Execution:** Condor cluster 5428224, 4 jobs (one per period), LCG_109a
  environment, 364/364 files processed. Merge/propagation ran on the login
  node (0.8 s; single central ROOT reads, small arrays).

Scan conventions: `X` = gen+→reco− off-diagonal block (lands in the pos body
at the reco kinematic bin), `Y` = gen−→reco+ (lands in the neg body);
both `(490, 245)` reco-major, matching the response-body indexing. Weighted
quantities use `w_reco = w1·w2·NuclIAWeight` as in the response body legs.

## 3. Validation — ALL PASS

| Gate | Result |
|---|---|
| Per-period body+fake reproduction vs `mig_v4_rapgap31_<P>.root` | max rel dev **0.0** (exact), all 4 periods, both charges |
| Total body+fake reproduction vs central ROOT | max rel dev **≤ 3.9e-16**; zero-structure exact (0/0 mismatches; 22,500 / 21,563 nonzero body cells pos/neg; 487/484 fake cells) |
| Central IBU k=4 coherent-prior recompute vs release yields (canonical `crosscheck_unfold_methods_v4.py` imported unchanged) | max rel dev **3.7e-15** (h−), 2.1e-15 (h+); median ~1.5e-16 |
| Off-diagonal bookkeeping: X/Y column sums vs per-gen-bin misID weights | max rel dev **≤ 2.0e-15** |
| JSON↔npz scalar cross-checks (per-period and total X/Y/matched counts) | exact match, all entries |

The scan therefore reproduces the exact population of matched rows entering
the central v6 RAPGAP31 response body, and the propagation machinery
reproduces the released yields before any correction is applied.

## 4. Charge-misID rates in the response body

**Overall (all matched body rows):**

| Quantity | Value |
|---|---|
| Matched body rows | 59,213,848 |
| Matched body weight | 80,182,218.16 |
| MisID rows (reco charge ≠ gen charge) | 199,404 |
| MisID weight | 268,843.13 |
| **MisID rate — unweighted** | **0.3368 %** |
| **MisID rate — weighted** | **0.3353 %** |

**Direction asymmetry (stable across periods):**

| Block | Count | Rate within its gen-charge body (cnt / w) |
|---|---|---|
| X: gen+ → reco− | 52,087 | 0.1737 % / 0.1720 % |
| Y: gen− → reco+ | 147,317 | 0.5039 % / 0.5027 % |
| **Y/X ratio** | **2.83 (cnt), 2.85 (w)** | per-period 2.79–2.87 |

The misID is **not symmetric**: gen− hadrons are charge-confused into the h+
sample ~2.8× more often than the reverse. The 2026-05-31 "symmetric at the
integrated level" assumption is empirically false, and this asymmetry — not
the overall rate — drives the net bias below.

Per-period: ep0304 0.3532 %, em0405 0.3115 %, em06 0.3649 %, ep0607 0.3399 %
(unweighted). Reco-space view: the h+ data space contains 0.4898 % misID-in
rows (147,317 / 30,075,052), the h− data space 0.1788 % (52,087 / 29,138,796).

**Matching-tier structure:** ALL 199,404 misID rows are Tier 0 (truth-link).
Tier 2 greedy matching carries a same-charge veto and contributes 94,837
matched rows with **zero** misID; Tier 1 is empty. The misID classification is
therefore complete — no matching path can produce an uncounted charge flip.

**Loose sample (no fiducial cuts, matched pool only):** 1,444,933 /
102,964,224 = **1.4033 % unweighted / 1.4857 % weighted**. The v4 fiducial
contract suppresses the raw charge-confusion rate by ~4.2×.

## 5. Kinematic dependence

U-shaped in reco pT (weighted and unweighted agree):

| Regime | Rate |
|---|---|
| pT ∈ [0.15, 0.172) GeV | 2.96 % (1,328,119 matched rows) |
| pT ∈ [1, 3) GeV (minimum) | 0.03–0.05 % |
| pT ∈ [29, 43) GeV | 2.3–3.5 % (populations 10³) |
| pT > 57 GeV | 5.9–33 % (populations ≤ 10²; irrelevant to totals) |

- Low-pT material region dominates the integral: **31.6 % of all misID rows
  (62,948/199,404) lie below pT = 0.198 GeV**; 26.9 % below p = 0.25 GeV.
- High-momentum rise is the classic curvature-sign-confusion regime; the
  extreme bins hold ≤ 10² matched rows and do not affect any total.
- `pT_h` (CM) profile: monotone fall from 1.36 % at 0.02 GeV to ~0.10 % at
  ~2.5 GeV, mild rise to ~0.25 % at ~8 GeV.
- Per-gen-bin on the published 245×2 grid: median **0.225 % (gen+) / 0.477 %
  (gen−)**; max **4.69 %** (S1_x1, z[0,0.05), pT[4,8), n=123) and **8.30 %**
  (S1_x1, z[0,0.05), pT[4,8), n=138) — i.e. the largest per-bin contamination
  sits in the high-pT published bins of the lowest-z row, not at low pT.

## 6. Propagated unfolding bias

**Method 1 (primary; identity-consistent data-vector correction).** Using the
release unfolded yields `n` as truth proxy and the coherent prior
`P = body+miss`:

```
Delta_pos[i] = sum_j X[i,j]·n_pos[j]/P_pos[j] − sum_j Y[i,j]·n_neg[j]/P_neg[j]
Delta_neg    = −Delta_pos   (identically)
d_sig_ideal  = d_sig ± Delta·(1−f_i)          (fake-subtracted, release convention)
bias         = n_central − n_ideal            (IBU k=4, UNCHANGED central response)
```

| Per-bin \|bias\| | h+ | h− |
|---|---|---|
| Median | **0.0464 %** | **0.0507 %** |
| Mean | 0.141 % | 0.198 % |
| p95 | 0.565 % | 0.725 % |
| Max | +1.669 % (S3_x0, z[0.7,1.0), pT[4,8), yield 45.8) | −4.558 % (S3_x0, z[0.7,1.0), pT[4,8), yield 7.9) |
| Bins > 0.1 % | 83/245 | 90/245 |
| Bins > 1 % | 3/245 | 8/245 |

All 11 bins above 1 % are low-yield (median yield 122 vs 1610 all-bin h+;
12.9 vs 1383 h−) and sit in the pT[4,8) high-z corners and the z[0,0.05)
pT[1,2) edge — the same kinematics as the per-gen-bin rate maxima.

**Integrated shifts** (RAW release scale, coherent prior, k=4):

- h+ **overestimated by +0.335 %**: 11.483372 → 11.444978 nb (−0.0384 nb).
- h− **underestimated by −0.344 %**: 11.190383 → 11.228980 nb (+0.0386 nb).
- Equal and opposite within 0.01 pp — a pure inter-charge migration, as
  expected since misID neither creates nor destroys tracks.

**Folding-identity violation at reco level (before unfolding):** median
0.048 % (h+) / 0.053 % (h−); p95 0.66 % / 0.77 %; max 6.01 % / 9.38 %.
IBU smoothing reduces the worst per-reco-bin violations to the per-gen-bin
biases above.

**Method 2 (cross-check; reco-routed response recompute).** Rebuild the body
as `B1 = B − X + Y` (h+) / `B − Y + X` (h−) with rebuilt prior and `f_i`, and
unfold the unchanged data. This solves the identity violation but also
**relabels truth charge**, so it is quoted only as a consistency check:
integrated −0.197 % (h+) / +0.204 % (h−); median 0.149 % / 0.171 %; max
2.01 % / 4.43 %. Sign structure and bin localization agree with Method 1;
magnitudes are ~60 % of Method 1 at the integrated level, consistent with the
known caveat that relabeling absorbs part of the effect into the truth
definition.

## 7. Comparison against the canonical 13-component budget

Budget basis: `output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_2026-10-07/`
(per-bin `total_with_crad13_rel`; medians reproduce STATE.md's 5.713 % / 
6.150 % exactly — basis verified).

| Quantity | Value |
|---|---|
| Per-bin \|bias\| / per-bin budget, median | **0.0062** |
| … p95 | 0.141 |
| … max | **0.229** (h+, S2_x1, z[0,0.05), pT[1,2): 1.07 % vs 4.67 %) |
| Bins with \|bias\| > per-bin budget | **0 / 490** |
| Bins with \|bias\| > 0.5 × per-bin budget | **0 / 490** |
| Quadrature increment if added as a 14th component | median 0.00016 pp, max 0.178 pp |

Even the single worst bin (h−, −4.56 %) carries a 58.3 % per-bin budget in
the crad13 table; every >1 % bias bin has a budget ≥ 4.7 %.

## 8. Charge-asymmetry impact — the one material consequence

Because Y ≈ 2.83·X while the h± integrated yields are nearly equal, the
central release systematically inflates h+ and deflates h−:

- Integrated `A = (h+ − h−)/(h+ + h−)` (RAW yields): **0.012922 (central) →
  0.009526 (identity-consistent); ΔA = −0.0034 absolute**, i.e. the central
  chain overstates the integrated charge asymmetry by ~36 % of its corrected
  value.
- Integrated h+/h− ratio inflated by **+0.68 % relative** (1.026182 → 
  1.019236).

This confirms and supersedes the rev5-era estimate ("up to ~2×0.35 % = 0.7 %
per published bin", `rev5_iteration_2026-05-31/critic-1`): the actual
per-bin effect is far smaller (median 0.05 %), but the asymmetry-level effect
concentrates exactly where that critic pointed — in `A_h` itself.

## 9. Born-level propagation

`sigma_Born(bin) = C_rad(bin) × Unfold(data; R)` is per-bin multiplicative,
applied post-unfold, and common to the central and corrected yields. **All
relative biases in Sections 6–8 therefore propagate unchanged to the Born
release** (`born_crad_dj14xfcut_..._2026-10-07/`). Absolute nb shifts scale
by the per-bin C_rad; at the integrated level (C_rad ≈ 1.1419 / 1.1414) the
Born-scale shifts are ≈ −0.0438 nb (h+, on 13.150141 nb) and +0.0441 nb
(h−, on 12.813870 nb).

## 10. Verdict

- **Per-bin h+/h− cross sections and multiplicities (RAW and Born):
  NEGLIGIBLE.** 0/490 bins exceed their per-bin 13-component budget; the
  median bias/budget ratio is 0.006 (≈160× subdominance); the quadrature
  increment of adding this as a 14th component is ≤ 0.18 pp everywhere. No
  correction to the release is required on this basis. Recommended
  disposition: register as a documented subdominant source using
  `unfolding_bias_per_bin.csv` (Method 1), not as a correction.
- **Charge-asymmetry observables (`A_h`, h+/h− ratios): MATERIAL relative to
  the observable.** The integrated asymmetry is overstated by +0.0034
  absolute; any published `A_h`-type quantity must either carry this as an
  explicit systematic (per-bin ±|bias| from the CSV; sign correlated between
  charges by construction) or be produced with the identity-consistent
  correction. A raw asymmetry quote without this term is not defensible.
- **A-F3 can be CLOSED AS QUANTIFIED** on the basis of this study, with the
  asymmetry-observable caveat carried forward to any future `A_h` product.

## 11. Caveats and blockers

- **MC-based estimate.** The bias uses the RAPGAP31 charge-confusion model.
  No real-data charge-confusion constraint exists in v6; the direction
  asymmetry (Y/X ≈ 2.83) is empirical in MC and its data counterpart is
  unvalidated. First-order scaling: if the true misID rate differs by a
  factor f, the biases scale approximately linearly in f.
- **Method 2 truth-relabeling caveat** (Section 6): the cross-check changes
  the meaning of the unfolded truth charge; it bounds consistency, not truth.
- **Yield-level scope.** This study propagates relative yield biases only.
  The absolute nb figures quote the release's own normalization
  (`L_data=351000.0 nb^-1`, carried by the release's normalization verdict);
  no new normalization is derived here.
- **Tier coverage.** The misID census covers all matching paths (Tier 0 +
  Tier 2; Section 4), so the rate cannot be underestimated by an uncounted
  matching channel.
- **Low-pT published bins.** After the (population-irrelevant) high-pT tail,
  the largest per-gen-bin contamination sits in pT[4,8) bins of the z[0,0.05)
  row (up to 8.3 %); these are exactly the low-yield bins where the budget is
  tens of percent, which is why the effect stays subdominant everywhere.

## 12. Artifacts (study-only; absolute paths)

Study root:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/charge_misid_response_2026-10-08/`

| Artifact | Path (relative to study root) |
|---|---|
| Scan script (selection inlined verbatim from production v4 builder) | `scripts/scan_charge_misid_rapgap31_v6.py` (md5 `180f2c5dea738fca53d16a15e1af2c41`, matches per-period JSON records) |
| Merge / validation / rates / propagation script (canonical IBU machinery imported unchanged) | `scripts/merge_and_analyze_charge_misid.py` (md5 `ca2bb34cec84d9bbdea7b4e157418ecc`) |
| Per-period scan outputs | `scan_outputs/scan_{ep0304,em0405,em06,ep0607}.{npz,json}` |
| Condor submission + wrapper + logs (cluster 5428224, 4/4) | `condor/` |
| Merged totals (all scan arrays, 4 periods summed) | `merged/merged_scan_totals.npz` |
| Validation report (all gates of Section 3) | `merged/validation_report.json` |
| Overall rates (Section 4) | `merged/misid_rates_overall.json` |
| Per-gen-bin rates, 245×2, with into/out-of body flows | `merged/misid_rate_per_genbin.csv` |
| Per-reco-bin rates, 490×2 | `merged/misid_rate_per_recobin.csv` |
| Kinematic profiles (P, Pt, pTh; matched and loose samples) | `merged/misid_kinematic_profiles.csv` |
| Per-bin bias table (Method 1 + Method 2 cross-check) | `merged/unfolding_bias_per_bin.csv` |
| Bias summary (Section 6 numbers) | `merged/bias_summary.json` |

Provenance: scan selection = production
`scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`
(F7 routing lines 909–918; v4 fiducial contract lines 231–284); IBU machinery =
`crosscheck_unfold_methods_v4.py` (same directory) imported unchanged;
binning = `binning_v3.py` (md5 `ab35ee80cbb3cff0e636fbbe38f8e310`, matches
scan records). The pre-v6 `migration_inputs_v3_percharge_NWfix_v2.root` was
NOT used. Budget comparison basis = canonical crad13 budget (Section 7).
