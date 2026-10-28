# Critique: H1 high-Q² SIDIS per-charge TUnfold pipeline v3 (960-bin gen)
**Date:** 2026-05-31
**Reviewer:** h1-analysis-critic
**Material reviewed (local copies):**
- `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/{README.md, binning_v3.py, build_migration_th2_per_charge.py, merge_migration_per_charge.py, unfold_tunfold_single_tau.C, postprocess_single_tau.py, summarize_tau_scan.py}`
- Six `tau_scan_summary_<mode>_pos.json` in `H1/sidis_highQ2/data/tunfold_tree_v3_percharge/run_960bin_pos_v2/`
- Reference: `H1/sidis_highQ2/docs/research/research-tunfold-cxx-port-state-2026-05-30.md` (Britzger pattern)
- Diagnostic: `H1/sidis_highQ2/data/diagnostics/charge_confusion_v3/charge_confusion_summary.json`

**Verdict family:** SIDIS unfolding methodology + bin construction. Final integrated verdict on the pipeline as a producer of publication numbers: **CONCERN** with one **FATAL** item (the τ-selection score function chooses unphysical solutions, see §1.2 and §3.1). No bug found in the bin-axis construction itself (Concern 1).

---

## 1. Verdicts on the two user concerns

### 1.1 Concern 1 — pT_h shows ρ ≈ 1 always; possible bin-axis off-by-one bug

**VERDICT: NOT-A-BUG (axis construction is correct; ρ≈1 is a methodology consequence, see §2.G).**

**Justification.**

(a) *digitize/edge behavior is consistent reco↔gen.* `binning_v3.py:159-164` (`zp_local_reco`) and `:170-176` (`zp_local_gen`) both use `np.digitize(x, EDGES) - 1` followed by the same `(iz<0)|(iz>=NZ)|(ip<0)|(ip>=NPT)` bad-mask. With NumPy default `right=False`, an exact value at the **lowest** edge (z=0.0, pT=0.0) maps to bin 0 (`digitize` returns 1 → −1 = 0); an exact value at the **highest** edge (z=1.0, pT=8.0) returns `Nbins` → −1 = `Nbins-1+1`, which the bad-mask correctly throws out (verified locally with NumPy). Reco and gen edges share their reco parents bit-identically — `binning_v3.py:339-345` asserts that every reco edge appears in `Z_EDGES_GEN`/`PTH_EDGES_GEN`. So a track at z=0.0 lands in `iz_reco=0` AND `iz_gen=0`, which `binmap_gen_to_reco` aggregates back to `iz_reco=0`. No mirroring asymmetry.

(b) *numpy→TH2D bin conversion is correct.* In `build_migration_th2_per_charge.py:557-559` the TH2D is constructed as `TH2D(name, title, NB_R_EXT, 0.0, float(NB_R_EXT), NB_G_EXT, 0.0, float(NB_G_EXT))` — so the underlying X axis has `NB_R_EXT=241` bins of unit width over `[0, 241]`. ROOT bin 1 covers `[0, 1)`, ROOT bin 241 covers `[240, 241]`. The fill at `:566` does `SetBinContent(i+1, j+1, R[i,j])` with `i` the numpy 0-indexed reco bin and `j` the numpy 0-indexed gen bin. ROOT bin 1 ↔ numpy 0 and ROOT bin 241 ↔ numpy 240. The miss row (numpy index `RECO_OVERFLOW_IDX = NB_R = 240`, set at `:74`) goes into ROOT bin 241, the gen-overflow column (numpy `NB_G = 960`, `:75`) into ROOT bin 961. Correct.

(c) *Macro reads bin numbers correctly.* `unfold_tunfold_single_tau.C:431-439` loops `for (int i = 1; i <= NB_G; ++i) unfolded[i-1] = h_unf->GetBinContent(i);` — pulls ROOT bins 1..960 into numpy indices 0..959, leaving the gen-overflow at ROOT bin 961 out of the unfolded vector. Refold loop `:443-451` does the same for `NB_R=240` reco bins. Aggregation loop `:466-476` uses `for (i=1; i<=NB_G; ++i) for (j=1; j<=NB_G; ++j) ... cov_pub(binmap[i-1], binmap[j-1]) += h_cov->GetBinContent(i,j)` — same convention. All consistent.

(d) *`kHistMapOutputVert` orientation matches the build.* Per the research doc §3c (`research-tunfold-cxx-port-state-2026-05-30.md:416-424`), Britzger's canonical orientation is `kHistMapOutputVert` = "reco on x (input binning), gen on y (output binning)". The build in `merge_migration_per_charge.py:163` constructs the TH2 with `(NX_reco, NY_gen)` order matching that orientation, and the macro `unfold_tunfold_single_tau.C:382-386` constructs `TUnfoldDensity(h_R, TUnfold::kHistMapOutputVert, ...)`. Self-consistent with the merged file's `meta_orientation` string at `merge_migration_per_charge.py:403-405`. Note also that `h_R->GetNbinsY()-1` is used at `:327` to derive `NB_G`, confirming the macro expects gen on Y.

(e) *Why ρ≈1 then?* The ρ≈1 saturation is real but is a **regularization-coupling artifact, not a bin-axis bug**. Two reasons reinforce this:
  1. *Under-determined system.* 240 reco bins constrain 960 gen bins. TUnfold + `kRegModeCurvature` then drives nearby gen bins toward a smooth interpolation; the four gen bins inside the same reco parent (the `N_GEN_PER_RECO = 4` aggregation, `binning_v3.py:131`) are almost perfectly co-determined, so their aggregated covariance row is dominated by a single common mode → off-diagonal ρ_pub between pT-adjacent reco bins inside a (cell, z) group saturates near 1.
  2. *The supervisor's own slicing already shows this.* `mean_abs_rho_pub_offdiag ≈ 0.06` and `frac_abs_rho_pub_gt_0p9 ≈ 0.6%` are exactly what one expects when only the few-nearest pairs along the regularization neighbor list saturate while everything else is healthy. The per-mode summary `tau_scan_summary_data_pos.json` recommended-τ row has `mean=0.16`, `max=0.998`, consistent with this physical picture.

This is a **methodology FLAG** — see §2.G for the missing 4D-aware L matrix that would fix it — not a bin-axis bug.

---

### 1.2 Concern 2 — lowest-τ in scan always wins

**VERDICT: FATAL methodology flaw in the score function. The score function rewards numerically pathological solutions and the recommendation logic blindly takes the argmin.**

**Justification.**

(a) *The score function does NOT penalize unphysical unfolded totals or wrong sign.* `unfold_tunfold_single_tau.C:810-817`:
```cpp
double score_tau = mean_abs_rho_pub_offdiag
                 + 0.5 * frac_pub_gt09
                 + 0.2 * std::fabs(std::log10(std::max(refold_chi2_per_ndf, 1e-3)))
                 + 0.5 * (double(n_negative_pub) / 240.0)
                 + 1.0 * (pathology_flag ? 1.0 : 0.0);
if (is_closure && std::isfinite(closure_max_abs_rel)) {
  score_tau += 0.5 * closure_max_abs_rel;
}
```
There is no term involving `unfolded_total_pub`, `data_total`, the *sign* of `unfolded_total_pub`, or any comparison `|unfolded_total_pub − data_total| / data_total`. The data-mode `data_pos` JSON shows the recommended τ=1e-5 with `unfolded_total_pub = -1.51e5` while `data_total ≈ +1.48e6` — a sign-flipped, **physically impossible** solution. Yet its score (1.43) is the lowest among 21 scanned τ. This is FATAL: any downstream consumer that takes the recommended-τ unfolded vector as the publication central value will produce a negative differential cross-section.

(b) *The `pathology_flag` term is degenerate across the scan.* Per `unfold_tunfold_single_tau.C:798-809`, `pathology_flag` is true whenever ANY of {gate_max_rho>0.95, gate_mean_rho>0.5, gate_n_negative>12, gate_n_zsigma, gate_refold_hi>5, gate_refold_lo<0.05, gate_neg_eig>0, gate_cond>1e6, gate_closure_max>0.20} trips. Inspection of all six summary JSONs: **every single scanned τ in every mode has `pathology_flag = True`** — `max_abs_rho_pub_offdiag` saturates at ≈1 everywhere (the `gate_max_rho` gate alone is enough). So the `+1.0 * pathology_flag` term is a **constant +1** across the entire scan and contributes nothing to the ranking. The score function is effectively `mean_abs_rho_pub + 0.5*frac_gt09 + 0.2*|log10(refold)| + 0.5*nneg/240` for ranking purposes — none of which penalize negative totals.

(c) *`summarize_tau_scan.py` does not recompute the score, just sorts.* `summarize_tau_scan.py:192-194` sets `r['score_for_ranking'] = r['score_tau']` (taken verbatim from the per-τ JSON). `:276-282` does `order = np.argsort(np.where(valid, scores, np.inf)); recommended = int(top5_idx[0])`. No physicality filter, no constraint that recommended τ must have `unfolded_total_pub > 0`. The bug is in the score function itself, not in the summarizer; the summarizer just propagates it.

(d) *Should the scan extend below 1e-5?* No — the question is moot because the entire low-τ tail produces negative `unfolded_total_pub`. Looking at `tau_scan_summary_data_pos.json`, the top-5 are all at τ ≤ 1e-3 and all five have `unfolded_total_pub` in `[-1.7e5, -6.3e4]` (negative). Going lower in τ will make this worse, not better (less regularization → more variance in the under-determined system → larger negative excursions). For `dataDJ`, top-5 are all at τ ≤ 1.1e-3 with `unfolded_total_pub` in `[-2.3e5, +8.1e3]` (mostly negative). The system is genuinely under-determined and the L-curve corner / ScanTau RhoAvg minimum should be inserted as **independent cross-checks** (Britzger's canonical recipe uses `ScanTau` with `kEScanTauRhoAvg` — research doc §3d, `:451-457`). But neither cross-check will recover physicality on its own; the structural fix is to (i) refuse τ that yield negative totals or grossly violate area constraint, and (ii) reduce the gen binning back toward the reco binning until the system is at least square (see §3.1).

**Side observation that strengthens the verdict.** The `selfRG_pos` and `selfDJ_pos` closure modes have `closure_max_abs_rel = 795.9` and `255.3` at the *recommended* τ — meaning the per-bin closure (unfolded−truth)/truth misses by factors of 200×–800× in at least one bin. The current score additively absorbs `0.5 * closure_max_abs_rel` (`unfold_tunfold_single_tau.C:815-817`), so `selfRG_pos` recommended score = 399 is *dominated* by that 0.5×796 = 398 term — meaning the closure term is so dominant that the relative ranking IS basically by closure for closure modes, but with no upper-bound penalty for unphysical totals. The pipeline cannot reliably pick a τ today.

---

## 2. Broader findings (sections A–J)

### A. Fiducial cut mirroring — STATUS: WARN (cannot fully verify locally)

**What I CAN confirm locally:**
- `build_migration_th2_per_charge.py:81-92` (reco event + track cuts) and `:93-97` (gen event + Option C gen fid) mirror across the boundary as follows:
  - Q² > 150 at both reco (`Q2_min`=150 at `:83`) and gen (`Q2_min`=150 at `:93`).
  - 0.1 < y < 0.7 at both reco (`y_lo/hi` at `:84`) and gen (`:93`).
  - 35 < Empz < 70 at reco (`:85`) AND at gen (`GenEmpz_lo/hi` at `:96-97`).
  - xF < 0 at reco (`xF_SIDIS_es < 0` at `:354`) and at gen (`xFGen < 0` at `:267`).
  - PDG pool {211, 321, 2212} at gen (`:98`).
  - Reco track cuts (`Pt_min/max`, `Eta_abs`, `NHitDedx_min`, `RadLength_min`, `DcaCosh_abs`, `IsFromPrimary`, `track_class_ok`) applied at `:358-368`.
  - z-outlier reco cut `(z_comp<0)|(z_comp>1)` at `:389-390`.
- The `Q²` cut variable choice: at reco, the event cut uses `Q2e` (`:334`) but the *binning* uses `Q2es` and `Xes` (`:392-393`). At gen, the event cut uses `Q2esGen` (or `Q2_es_gen` if present, `:245-256`), and binning prefers `Q2esGen`/`x_es_gen` (`:277-286`). Critic note: the reco *event* cut on `Q2e` (electron-method) while the *binning* uses `Q2es` (Σ-method) is a deliberate inconsistency you should make sure was intentional — this matters because a track can pass `Q2e>150` while sitting in `Q2es<150` (would put it into cell stratum S0 incorrectly or get an OOF mark). The mirror at gen uses `Q2_es_gen` consistently. **Flag**: confirm with the canonical extractor whether `Q2e` vs `Q2es` for the reco event cut is the same convention; the research/handoff docs reference Σ-method.

**What I CANNOT verify locally:**
- The canonical extractor `extract_pertrack_v5g_fixedFrame_v3p2.py` is on NAF only and not in this workspace. I cannot diff line-by-line against it.

### B. Weight handling — STATUS: PASS

- Reco-side: `build_migration_th2_per_charge.py:402-410`:
  ```python
  w1 = batch["weight1"].astype(float)
  w2 = batch["weight2"].astype(float)
  wn = batch["NucliaWeight"].astype(float)   # clipped to [0, 10]
  w_reco = w1 * w2 * wn
  ```
  Correct: `weight1 * weight2 * NucliaWeight` with NucliaWeight clipped to `NUCLIA_CLIP = 10.0` (`:99`, `:407`).
- Gen-side: `:316-318`:
  ```python
  w1 = batch["weight1"].astype(float) if "weight1" in batch else np.ones(n)
  w2 = batch["weight2"].astype(float) if "weight2" in batch else np.ones(n)
  w_gen = w1 * w2
  ```
  Correct: `weight1 * weight2` (no NucliaWeight at gen, per convention).
- One **minor concern**: the `if "weight1" in batch else np.ones(n)` fallback at gen would silently use 1.0 if the branches are missing. The branch list `CTG_BRANCHES` at `:181` does list both, and the `arrays(use_ctg, …)` call at `:541` filters to branches actually present in the file. If a file is missing `weight1`/`weight2` for any reason, that file would be silently weighted to 1.0. Recommend a hard assert at gen-pool entry that both branches exist.

### C. Acceptance encoding (Britzger Encoding B) — STATUS: PASS

- Miss row at reco-axis bin index `RECO_OVERFLOW_IDX = NB_R = 240` (`build_migration_th2_per_charge.py:74`). Filled at `:489-505` from gen tracks for which `gen_matched_flag` is False, with weight `w_gen` and dispatched by **gen** charge (`acc = self._acc_for_charge(gc)` at `:498`). Confirmed: `(i_gen >= 0) & (~gen_matched_flag)` is exactly the "ChargedTotGen tracks that have NO entry in ChargedTot's GenMatchIdx for that (Run, Event, GenIdx)" condition because `gen_matched_flag[grow]` is set only when the reco loop successfully finds the gen row via the `(Run, Event, GenMatchIdx)` key (`:471-473, :481`).
- Fake column at gen-axis bin index `GEN_OVERFLOW_IDX = NB_G = 960` (`:75`). Filled at `:482-487` from reco tracks whose `(Run, Event, GenMatchIdx)` key has no entry in the gen row dictionary, weight `w_reco`, dispatched by **reco** charge (`acc = self._acc_for_charge(rc)` at `:467`).
- Data TH1 has 241 bins, bin 241 (1-indexed ROOT) = 0 (the gen-overflow column has no real-data entry). `merge_migration_per_charge.py:268-275` constructs `ne` of length 241, copies `n_pos`/`n_neg` into `ne[:NB_R]`, and leaves `ne[NB_R]` = 0. Variance for the overflow bin is set to the *mean of non-zero variances* (`:274-275`), which is **not** zero. This is consistent with `unfold_tunfold_single_tau.C:281-291` (pseudo-data path) but is a non-trivial choice — it gives the overflow bin a non-zero σ. That's defensible (avoids singular weight on that bin in the χ² for TUnfold internal solve) but should be documented; the alternative is to use a Poisson-floor σ = √(mean(N)).

### D. Matched-event routing — STATUS: PASS (correct convention)

- `build_migration_th2_per_charge.py:474-481`:
  ```python
  ig = int(i_gen[grow])
  acc.R   [ir, ig] += wr     # wr = w_reco
  acc.R_w2[ir, ig] += wr * wr
  acc.matched_reco_marginal   [ir] += wr
  ```
  Matched entries are filled with the **reco weight** `wr = w_reco = weight1*weight2*NucliaWeight`. This is correct for TUnfold: the response matrix `R[reco, gen]` represents *the probability that a gen-pool event ends up in reco bin r, in the same units as the reco data input*. NucliaWeight is the reco-side detector/efficiency reweighting that brings MC reco into agreement with data reco; it MUST be on the reco side of the matrix because the data input that TUnfold sees is the reco-weighted, NucliaWeight-equivalent data distribution. Using `w_gen` here would create a column-normalization mismatch: the diagonals would be `w_gen` while the miss-row would be `w_gen` (consistent column-sum), but the data input on the reco axis carries the full NucliaWeight-equivalent rate. So `w_reco` is right.
- Consequence: the gen-pool prior used as a per-mode reference (`prior_gen` at `:439`) is filled with `w_gen`, but the *matched portion* of the column-sum is `Σ_r R[r, ig] = w_reco_matched`, which differs from `prior_gen[ig] = w_gen` by the NucliaWeight ratio. The miss row is filled with `w_gen` (`:502`). This means **column-sums do not equal prior_gen** (matched contributes `w_reco`, miss contributes `w_gen`). This is technically inconsistent with the strict "column-sum-of-the-response = prior" identity that TUnfold's area-constraint internal mathematics assumes (see §F). **Minor FLAG**, see §F.

### E. Per-charge dispatch — STATUS: PASS (with explicit caveat for charge confusion)

- Matched → reco charge (`:466-467`); miss → gen charge (`:498`); fake → reco charge (`:466-467`). Documented in the build script header (`:26-39`) and the README (`:279-285`).
- This is consistent with the data-side ground truth: the **data** input to the unfolder is split by reco charge (the only charge observable in data); therefore the response matrix must take reco-charge as the row coordinate. A matched track in which `Charge ≠ GenCharge` (0.35% per the diagnostic) sits in the reco-charge column-bin's `h_R_{rc}` and produces an `(i_reco, i_gen)` entry where `i_gen` belongs to the *opposite* gen charge population. So `h_R_pos` is technically NOT a strict "h+ → h+" matrix; it is "any matched reco h+ ← gen of either charge". This is the right convention given the dispatch choice. **The methodological cost** is that the per-charge unfolds implicitly assume the 0.35% gen-opposite-charge admixture in each `h_R_chg` is small enough to not bias the unfolded answer; the integrated 0.35% is fine but the diagnostic flags low-pT_h/low-z corners with higher rates which should be checked per-bin (see §I).
- An alternative consistent dispatch would be: matched → **gen** charge (so `h_R_pos` is strictly "h+ at gen produces some reco track"). That would make the response matrix represent the truth-level h+ asymmetry but would create a mismatch with the reco-axis data split. The current convention is correct.

### F. TUnfold configuration — STATUS: WARN

- `kHistMapOutputVert`: PASS, matches Britzger (research doc §3c `:416-424`) and matches build orientation.
- `kRegModeCurvature`: PASS for "smoothness on the flat index", but see §G for the structural issue with flat-index curvature.
- `kEConstraintArea`: **WARN.** With Encoding B's miss row, area-constraint enforces `Σ_{r=1..240} unfolded_refold_r + miss_refold_240 = Σ_{r=1..240} data_r + 0`, i.e., the total *including* the miss row equals the data total. The unfolded physical-sum is then `data_total × (1 − miss_flow_fraction)`. As long as `prior_gen[g]` is consistent with `Σ_r R[r,g]`, area-constraint is mathematically clean. But per §D, those two are NOT equal in this build because matched fills use `w_reco` while miss fills use `w_gen` — a mismatch by the NucliaWeight ratio that integrates to ~1 but is bin-dependent. The likely sign of this is small (NucliaWeight is order 1 on most events), but it does mean TUnfold's "area" is being enforced against an effectively non-physical column-sum target. **Recommend either (i) use `kEConstraintNone` (Britzger's diagnostic-sweep choice) to remove this implicit assumption, or (ii) reconcile the miss-row weighting with the matched weighting** (e.g., reweight miss row to a NucliaWeight-equivalent rate).
- `kDensityModeNone`: PASS for unit-width index binning. Matches Britzger's effective choice per research doc (`:432-436`).

### G. L matrix curvature on flat index — STATUS: FLAG (methodology flaw)

`kRegModeCurvature` on the flat gen index applies a second-derivative penalty across **consecutive flat-index gen bins**. With our layout `i_gen = cell * 120 + iz*10 + ipt`, "consecutive" means:
- Within the inner pT_h block (ipt: 0→9): physically adjacent pT_h bins inside the same (cell, z) — **physically meaningful smoothing**.
- At iz boundaries (every 10 gen bins): jump from `(cell, z=k, ipt=9)` to `(cell, z=k+1, ipt=0)` — *not* physically smooth (highest pT in z-bin k coupled to lowest pT in z-bin k+1 as if they were adjacent).
- At cell boundaries (every 120 gen bins): jump from one (Q², x) cell to a totally different physical phase-space point — **physically meaningless**.

The L matrix therefore "smooths" across cell and z boundaries, producing exactly the kind of over-coupled pT-adjacent rows we see in §1.1. The supervisor's observation that the saturated-ρ pairs are "pT_h-adjacent within (cell, z) groups" is consistent with the legitimate-smoothing part; but Curvature on a flat-index ordering ALSO smooths across the meaningless boundaries, which contaminates the regularization. This is a real methodology issue.

**Recommended fix:** construct a custom L via `TUnfold::RegularizeCurvature(i, i+1, i+2)` triplet-by-triplet, calling it ONLY for triplets where all three bins share the same `(cell, z)` block for pT-curvature smoothing, and ONLY for triplets that share the same `(cell, ipt)` for z-curvature smoothing. Do not regularize across cell boundaries at all. This is the TUnfold-supported way to encode 4D structure. Britzger does not face this because his binning is more 1D-shape-like at the index level.

### H. Score function design — STATUS: FAIL (covered as fatal in §1.2)

Specific to the items asked:
- *Does it penalize unphysical unfolded totals?* No. There is no `|Σ unfolded − Σ data| / Σ data` term, no positivity guard, no sign check. FAIL.
- *Does `pathology_flag` work?* No: it is constantly True across the scan (all 21–80 τ trip `gate_max_rho` because `max_abs_rho_pub > 0.95` everywhere — see all six summary JSONs). The `+1.0 * pathology_flag` term is a constant offset and contributes zero to ranking. FAIL — needs to be replaced by graded contributions from each gate or by hard exclusion of failing τ from the recommended set.
- *Is `0.5 × closure_max_abs_rel` a sensible weight?* No — `closure_max_abs_rel` values seen in the data are 200–800; 0.5 × that overwhelms every other term and effectively turns the closure-mode score into "pick the lowest closure error", which (combined with the under-determined system) tends to converge on the same low-τ corner where the system is most flexible. Should be normalized — e.g., `closure_max_abs_rel / (1 + closure_max_abs_rel)` or simply a hard reject above 0.5 — to avoid one term dominating.
- *Should it include `cond_pub` or `effective_rank_pub`?* Yes. The `condition_number_pub` gate is at >1e6 which is far too permissive for a 240×240 matrix in publication context. A graded contribution `log10(cond/1e3)_+ * weight` would help.

### I. Charge confusion (0.35%) propagation — STATUS: FLAG

The 0.35% integrated charge-confusion was measured (per `charge_confusion_summary.json`) and used as the JUSTIFICATION for splitting into per-charge unfolds, but **no systematic source is wired into the current pipeline** for it. The README explicitly states (`:283`) "propagated as a separate ~1% systematic in low-pT_h / low-z / S3b corners" — but this is a forward statement, not a current implementation. The merged ROOT does not include a charge-swapped response variation, the macro does not consume one via `AddSysError`, and `summarize_tau_scan.py` does not surface a per-bin charge-confusion error column. **FLAG**: this systematic must be assembled before the final unfolded result is published, otherwise the per-charge results carry an undocumented bias in low-pT_h / low-z corners where the rate is higher than 0.35%.

### J. MC-truth pool (prior_gen) definition — STATUS: PASS

`build_migration_th2_per_charge.py:430-442`:
```python
good_gen = (i_gen >= 0)
...
np.add.at(acc.prior_gen, idx[m], wg[m])
np.add.at(acc.prior_gen_w2, idx[m], wg[m] * wg[m])
```
where `i_gen` is built only for gen tracks that pass `pass_fid & (i_flat >= 0) & (gen_chg >= 0)` (`:320`), with `pass_fid` enforcing event cuts + Option C gen fid + V0-daughter veto + POOL_PDG {π, K, p}. So `prior_gen` per charge per MC is exactly the full gen-pool charged-hadron count in the Option C fiducial, weighted by `weight1 × weight2`. This is the correct denominator for "differential rate in (Q², x, z, pT_h) bin per charge in the fiducial". 

One subtle note matching §D/§F: `prior_gen[g]` uses `w_gen` while the matched part of `Σ_r R[r,g]` uses `w_reco` (NucliaWeight-inflated). For an *acceptance-only* downstream calculation (`acc[g] = Σ_r_matched R[r,g] / prior_gen[g]`), this means acceptance carries an implicit NucliaWeight in the numerator that isn't in the denominator — biased acceptance unless `<NucliaWeight>` ≈ 1 per gen bin. Since `prior_gen` is what feeds `kEConstraintArea` consistency, this should be checked numerically.

---

## 3. Top blocking issues (would invalidate the final unfolded result)

1. **Score function selects unphysical solutions (FATAL, §1.2/§2.H).** Today, the recommended τ in `data_pos` gives `unfolded_total_pub = -1.51e5` versus `data_total = +1.48e6`. Any downstream consumer that takes this τ at face value produces a negative differential cross-section. Must be fixed before any publication number is generated.
2. **The system is under-determined by a factor of 4 (CONCERN, related to §1.1 and §3.1).** With 240 reco vs 960 gen and no useful regularization across cell/z boundaries (§G), TUnfold cannot recover gen-level structure finer than reco resolution; the fine binning provides no information that isn't already in the regularization. The right diagnosis is that the gen binning should be reduced to at most 1:1 with reco, or 2:1 with a structure-aware L matrix, until a re-scan can produce a τ whose `unfolded_total_pub` is positive and within ~10% of `data_total`.
3. **No charge-confusion systematic in pipeline (FLAG, §2.I).** The per-charge architecture was justified by the 0.35% diagnostic but the corresponding systematic is not wired in. Without it, per-charge unfolded results carry undocumented bias in low-pT_h / low-z / S3b corners.

Secondary but important:
4. **Flat-index curvature regularization smooths across physically meaningless cell/z boundaries (§2.G).** Drives the saturated near-ρ=1 pairs and contaminates the L-curve. Custom L via triplet-by-triplet `RegularizeCurvature` calls.
5. **`kEConstraintArea` with miss-row weighted by `w_gen` while matched fills use `w_reco` (§2.F/§2.D).** Mathematically inconsistent area target. Either switch to `kEConstraintNone` or reconcile the weighting.

---

## 4. Recommended next actions (concrete tasks)

For `h1-script-runner` / `h1-builder-engineer`:

1. **Rewrite the score function** (`unfold_tunfold_single_tau.C:810-817`):
   - Add a hard reject: if `unfolded_total_pub <= 0` OR `|unfolded_total_pub − data_total| / data_total > 0.5`, set `score_tau = +infinity` and `pathology_flag = true` with a new gate `gate_unphysical_total`.
   - Replace `+ 1.0 * pathology_flag` (constant +1 across scan) with graded sums of per-gate contributions.
   - Cap or normalize the closure term (`0.5 × closure_max_abs_rel`) — use `min(closure_max_abs_rel, 1.0)` to bound the weight.
   - Add a `log10(max(cond, 1e3) / 1e3)` term to penalize ill-conditioned solutions.
   - Add a hard gate on `gate_neg_eig` (any negative eigenvalue should be a HARD reject for a covariance matrix that must be PSD).

2. **In `summarize_tau_scan.py`** (`:276-282`), filter `valid` to additionally require `unfolded_total_pub > 0` AND `n_negative_pub < 12` AND `n_neg_eigenvalues_pub == 0` before computing `recommended_idx`. Add a new field `recommended_subject_to_physicality: true|false` so downstream consumers see that no physical τ was found.

3. **Add ScanTau/ScanLcurve as independent cross-checks** in the macro: call `unfold.ScanTau(50, 1e-8, 1e-1, &scanResult, TUnfoldDensity::kEScanTauRhoAvg, ...)` and write the τ minimum into the JSON; same for `ScanLcurve`. Compare against the score-function recommendation. This is Britzger's canonical approach (research doc §3d).

4. **Implement structure-aware L matrix** (`unfold_tunfold_single_tau.C` after TUnfoldDensity construction, before `DoUnfold`):
   - After constructing `TUnfoldDensity` with `kRegModeNone`, manually call `unfold.RegularizeCurvature(i, i+1, i+2)` only for triplets within the same (cell, z) — i.e., when `_decompose(i).cell == _decompose(i+2).cell && _decompose(i).z == _decompose(i+2).z`. Repeat for z-direction triplets within the same (cell, ipt).
   - Document the L-matrix construction in the JSON metadata.

5. **Re-evaluate the gen binning ratio.** The 4× over-binning vs reco produces an under-determined system that no τ can rescue physically. Drop back to the v2 1:1 gen=reco baseline OR keep 4× only with structure-aware L. Run a binning-comparison study before committing to v3 for publication.

6. **Wire the charge-confusion systematic.** Build a "charge-swapped response" variant of `h_R_pos` (where for each matched track with `Charge ≠ GenCharge`, swap the charge dispatch) and pass it via `TUnfoldSys::AddSysError(..., kSysErrModeMatrix)` once `TUnfoldSys` is used instead of `TUnfoldDensity` for the final canonical run.

For `h1-mc-expert` (if needed): confirm the NucliaWeight semantics — is it strictly a reco-side detector reweight, or does it include any gen-level component? This determines whether the §D/§F weighting mismatch is physical or a true bug.

---

## 5. Open questions (require additional diagnostics or remote access)

1. **Does `extract_pertrack_v5g_fixedFrame_v3p2.py` on NAF use exactly the same fiducial cuts as `build_migration_th2_per_charge.py`?** I cannot diff this from the local workspace; need a remote `diff` or a copy of the extractor into `H1/sidis_highQ2/scripts/extraction/` for audit. Specifically the `Q2e` vs `Q2es` choice for the reco event cut (§A).
2. **What is `<NucliaWeight>` per gen bin?** If close to 1.0 uniformly, the §D/§F weighting mismatch is harmless; if it varies by 10%+ across (Q², x, z, pT_h), the area-constraint target is biased by the same amount per column. Needs a per-bin diagnostic.
3. **What would the L-curve or ScanTau(RhoAvg) recommend?** Not currently computed. Adding them as in §4.3 is the most diagnostic next step.
4. **What does the unfolded distribution look like at a "physical" τ?** I have only the metric summaries; the per-τ ROOT files contain `h_unfolded_pub` per bin. A quick post-hoc filter of all τ rows by `unfolded_total_pub > 0` would tell us whether any τ in the current scan produces a physically reasonable solution, even if the score function doesn't pick it.
5. **Per-bin charge-confusion rate vs the integrated 0.35%.** The diagnostic was integrated; the README references low-pT_h / low-z / S3b corners as higher. Need that per-bin map to size the systematic correctly.

---

**End of critique.** Cross-references: research doc §3 (Britzger pattern), `binning_v3.py:131,255-275` (aggregation map), `build_migration_th2_per_charge.py:382-414` (reco weight assembly), `unfold_tunfold_single_tau.C:382-400` (TUnfold construction), `summarize_tau_scan.py:276-293` (recommendation logic).
