# Next-Session Handoff: Unfolding Method Tuning

**Date:** 2026-05-31
**Audience:** Supervisor / specialist picking up the H1 high-Q² SIDIS analysis
to **tune and finalize the unfolding** across TUnfold, IBU, BBB, and OmniFold.
**Read this BEFORE you touch any unfolding code or merged ROOT.**

This is a focused operational handoff. The broader `H1/sidis_highQ2/context-handoff.md`
covers everything else. This file collects what you need to make immediate
progress on unfolding tuning.

---

## 1. Working state in one paragraph

The high-Q² SIDIS unfolding pipeline at
`scripts/unfolding/tunfold_official_v3_percharge/` (TUnfold) and `scripts/ibu/`
(IBU) and `scripts/omnifold/` (OmniFold) is operational at **rev5**. Rev0→rev5
fixed (a) a broken `GenMatchIdx`-based MC↔reco matching (reco efficiency went
from 0.001 → 0.55), (b) inverted reco/gen binning roles (reco is now FINE at
960 bins, gen is COARSE at 240 published bins), (c) an F1 area-constraint
formula that was injecting `data/reco_eff²` instead of `data/reco_eff`,
(d) an OmniFold data-extract bug that required an MC-only flag on real data.
Rev5 dropped the F1 hack entirely: data overflow bin = 0 and TUnfold uses
`kEConstraintNone`. **At τ = 1.44 × 10⁻⁶ on h+, TUnfold gives unfolded total
≈ 3.19 × 10⁶, IBU gives 1.47 × 10⁶, BBB gives 2.77 × 10⁶**, and the
data-vs-MC `(RG − DJ)/RG` integrated model difference is **+16.8 %**. IBU at
rev5 effectively does NOT do acceptance correction because we removed the
miss-row data input it was using; this is the main known bug to fix.

## 2. Pipeline position (Step 8, multi-method comparison)

- ✅ Build per-event response from MC TTrees (validated, reco_eff ≈ 0.55)
- ✅ Merge → single response ROOT (with F1 fix at rev4, dropped at rev5)
- ✅ TUnfold rev5: `kEConstraintNone`, data overflow = 0
- ✅ IBU rev5 (rapgap + djangoh × pos + neg) — **acceptance broken**
- ✅ BBB analytic computation
- 🟡 OmniFold rev5 — re-submitted after data extract fix; running 4–12 h
- ⏳ Trust mask: defined and applied to pT_h plots; 230/240 trusted pos, 225/240 neg
- ⏳ σ_stat + σ_reg + σ_model breakdown at τ = 1.44e-6 (done for pos and neg)
- ❌ IBU acceptance correction at rev5 — needs fix
- ❌ More differential BBB (capture intra-bin migration) — open question
- ❌ Three-way method-systematics comparison once OmniFold done
- ❌ Final τ + iteration choice frozen and re-summarized

## 3. The merged ROOT and what's in it (CRITICAL)

NAF path:
```
/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge.root
```

Per charge `<chg> ∈ {pos, neg}` and per MC `<mc> ∈ {rapgap, djangoh, nominal}`
(nominal = rapgap alias):

| Histogram | Shape | Meaning |
|---|---|---|
| `h_R_<chg>_<mc>` | 961 × 241 TH2D | Fine reco × coarse gen response. Row 960 = reco-overflow (Britzger miss row); col 240 = gen-overflow (fakes). Inner 960 × 240 = matched. Values are absolute MC weights `weight1 × weight2 × NucliaWeight` (matched) or `weight1 × weight2` (miss/prior). |
| `h_R_<chg>_<mc>_w2` | 961 × 241 | sum-w² for variance |
| `h_prior_gen_<chg>_<mc>` | 240 TH1D | Per-MC gen-pool prior at MC luminosity; column sums of `h_R` over inner reco = matched fraction × prior |
| `h_matched_reco_marginal_<chg>_<mc>` | 960 TH1D | Per-MC matched-reco marginal at MC luminosity |
| `h_data_bgsub_<chg>` | 961 TH1D | Bg-subtracted real data, bin 960 = 0 in rev5 (was MC-predicted miss yield in rev4) |
| `h_data_bgsub_<chg>_w2` | 961 TH1D | Variance |
| `h_binmap_gen_to_reco` | 960 TH1I | **MISLEADING NAME**: this is reco→gen aggregation map (4 fine reco bins per coarse gen bin). Length 960, values in [0, 240). Used to aggregate fine-reco quantities to coarse-gen for BBB. |
| `h_mc_count_<chg>{,_djangoh}` | 240 TH1I | Unweighted MC track count per gen bin; defaults are rapgap (no suffix) and djangoh (`_djangoh`) |
| `meta_*` (TNamed) | scalar | Version tags + sums |

**Current version tag (TNamed):** `meta_f1_formula_version = "rev5_kEConstraintNone_no_overflow"`.

**Per-charge numbers** (from rev5 merge metadata; pos / neg):
- `data_bgsub_<chg>_coarse` (data physical reco total): **1.476 × 10⁶ / 1.426 × 10⁶**
- `prior_<chg>_rapgap`: 6.71 × 10⁷ / 6.59 × 10⁷
- `prior_<chg>_djangoh`: 7.57 × 10⁷ / 7.45 × 10⁷
- `matched_total_<chg>_rapgap`: 3.66 × 10⁷ / 3.55 × 10⁷
- `reco_eff_<chg>_rapgap_MC`: **0.546 / 0.539**
- Expected acceptance-corrected gen-pool total `data / reco_eff`: **2.71 × 10⁶ / 2.64 × 10⁶**

## 4. Binning (Scheme B', inverted layout, rev3+)

- **Reco (fine)**: NB_R = 960. Layout `i_reco = cell × 120 + iz_fine × 10 + ipt_fine`, with cell ∈ [0, 8), iz_fine ∈ [0, 12), ipt_fine ∈ [0, 10). 2× subdivision of z and pT_h vs published.
- **Gen (coarse, PUBLISHED)**: NB_G = 240. Layout `i_pub = cell × 30 + iz × 5 + ipt`, with cell ∈ [0, 8), iz ∈ [0, 6), ipt ∈ [0, 5).
- Cells (8): `S0a, S0b, S1a, S1b, S2a, S2b, S3a, S3b` = 4 Q² strata × 2 x sub-bins.
- Z edges: `[0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]`
- pT_h edges (CM frame): `[0.0, 0.5, 1.0, 2.0, 4.0, 8.0]` GeV
- Aggregation: 4 fine reco bins per published gen bin (2 × 2 in (z, pT_h)).

## 5. Numerical headlines you should know cold

**At τ = 1.44 × 10⁻⁶ (rev5), pos charge**, per-bin systematic medians on 240 published bins:

| Source | median | p90 |
|---|---|---|
| σ_stat / n (TUnfold cov diag) | 11.6 % | 110 % |
| σ_reg / n (τ × 2 / ÷ 2) | 0.2 % | 0.4 % |
| σ_model / n (RG vs DJ unfolds) | 15.9 % | 126 % |
| **σ_total / n** | **21.0 %** | 180 % |

`σ_model` dominates `σ_stat` (1.37 × at median); `σ_reg` is negligible.

**Multi-method model-uncertainty comparison** at same point (per-bin `|RG − DJ|/RG`):

| Method | median | integrated diff |
|---|---|---|
| **BBB** | **2.4 %** | −2.2 % |
| **TUnfold** | 15.9 % | +16.8 % |
| **IBU** | 30.6 % | +0.02 % (broken, see §7) |

Pos / neg numbers are highly symmetric: median σ_total = 21.0 % / 21.0 %; integrated diff = +16.8 % / +16.5 %.

Trust mask criterion: untrusted if `cnt_rap < 50` OR (`cnt_rap < 500` AND `reco_eff < 0.05`). Result: **230 / 240 trusted pos, 225 / 240 trusted neg.** Untrusted bins concentrate in S3b high-z high-pT (acceptance < 1 % in some bins).

## 6. Where things live (LOCAL + NAF)

### Code (deployed to both local and NAF)

| Path | What |
|---|---|
| `scripts/unfolding/tunfold_official_v3_percharge/binning_v3.py` | Per-charge bin definitions + binmap |
| `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge.py` | Per-event response builder (uproot, ~730 lines) |
| `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge.py` | Per-MC + data merger; **F1 was dropped at line 394 (rev5)** |
| `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C` | The actual TUnfold C++ macro; `TUnfoldDensity(h_R, kHistMapOutputVert, kRegModeNone, kEConstraintNone, kDensityModeNone)` + custom L matrix (304 triplets) + ScanLcurve / ScanTau / manual scan |
| `scripts/unfolding/tunfold_official_v3_percharge/postprocess_single_tau.py` | Per-τ diagnostic PDFs |
| `scripts/unfolding/tunfold_official_v3_percharge/summarize_tau_scan.py` | Per-mode τ-scan summary |
| `scripts/unfolding/tunfold_official_v3_percharge/tau_scan_grid.py` | Argfile generator |
| `scripts/unfolding/tunfold_official_v3_percharge/compare_*.py` | Cross-MC + data-vs-dataDJ comparisons |
| `scripts/ibu/ibu_core.py` | IBU implementation (D'Agostini iterative Bayes) |
| `scripts/ibu/run_ibu.py` | IBU driver per (charge, mc-tag) |
| `scripts/ibu/wrap_ibu_rev5_mctag.sh` | NEW wrap for `--mc-tag` argument (rev5) |
| `scripts/ibu/submit_ibu_rev5.condor` | NEW 4-job submit (pos/neg × rapgap/djangoh) |
| `scripts/omnifold/extract_omnifold_arrays.py` | Per-event extractor (FIXED at rev5: data-mode no longer requires `is_mat_h`) |
| `scripts/omnifold/omnifold_core.py` | PyTorch MultiFold algorithm |
| `scripts/omnifold/run_omnifold_*.py` | Drivers |
| `scripts/analysis/per_bin_systematics_at_tau.py` | NEW σ_stat/σ_reg/σ_model breakdown at fixed τ |
| `scripts/analysis/tau_selection_metrics.py` | NEW multi-metric τ-selection script |
| `scripts/analysis/compute_bbb_and_compare.py` | NEW BBB computation + 3-method comparison |
| `scripts/analysis/plot_pT_h_per_cell.py` | NEW pT_h plots with trust mask |

### NAF outputs

| Path | Contents |
|---|---|
| `/data/dust/.../output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge.root` | rev5 merged response (4.5 MB) |
| `/data/dust/.../output/tunfold_tree_v3_percharge/tau_scan/` | TUnfold rev5 per-τ JSONs/PDFs/ROOTs (data + dataDJ × 20 τ ∈ [1e-9, 10]) |
| `/data/dust/.../output/tunfold_tree_v3_percharge/closure/` | TUnfold rev5 closure-mode JSONs (selfRG, selfDJ, crossDJ, crossRG × 20 τ) |
| `/data/dust/.../output/tunfold_tree_v3_percharge/ibu_rev5_rapgap/` | IBU rev5 with rapgap response (pos + neg) |
| `/data/dust/.../output/tunfold_tree_v3_percharge/ibu_rev5_djangoh/` | IBU rev5 with djangoh response (pos + neg) |
| `/data/dust/.../output/omnifold/omnifold_inputs_{rapgap,djangoh,data}.npz` | OmniFold merged inputs (2.5 GB each) |
| `/data/dust/.../output/omnifold/data_unfold_rapgap_iter5/` | OmniFold data unfold dirs (status: re-submitted as cluster 3559797, still running) |

### Local data + plots

| Path | Contents |
|---|---|
| `data/tunfold_tree_v3_percharge/run_rev5/sigma_breakdown/` | σ breakdown JSON + 4 per-τ JSONs for pos |
| `data/tunfold_tree_v3_percharge/run_rev5/sigma_breakdown_neg/` | same for neg |
| `data/tunfold_tree_v3_percharge/run_rev5/tau_sweep_pos/` | 10 JSONs (data + dataDJ × 5 τ near 1.44e-6) |
| `data/tunfold_tree_v3_percharge/run_rev5/tau_scan/` | All rev5 per-τ data JSONs (pos) |
| `data/tunfold_tree_v3_percharge/run_rev5/closure/` | All rev5 per-τ closure JSONs (pos) |
| `data/tunfold_tree_v3_percharge/run_rev5/ibu_rapgap/`, `ibu_djangoh/` | IBU rev5 per-iter JSONs |
| `data/tunfold_tree_v3_percharge/run_rev5/bbb_compare/` | BBB vs IBU vs TUnfold comparison JSONs (pos + neg) |
| `data/tunfold_tree_v3_percharge/run_rev5/tau_selection_metrics.json` | Multi-metric τ ranking |
| `plots/tunfold_tree_v3_percharge/run_rev5/pT_h_per_cell.pdf` | 4-page pT_h distributions with trust mask + σ_total bars |
| `plots/tunfold_tree_v3_percharge/run_rev5/per_bin_systematics_tau1p44em6.pdf` | σ_stat/σ_reg/σ_model breakdown plot (pos) |
| `plots/tunfold_tree_v3_percharge/run_rev5/per_bin_systematics_neg_tau1p44em6.pdf` | same (neg) |
| `plots/tunfold_tree_v3_percharge/run_rev5/tau_selection_metrics.pdf` | 6-page multi-metric τ-selection PDF |
| `plots/tunfold_tree_v3_percharge/run_rev5/tau_sweep_model_systematic.pdf` | σ_model vs τ |
| `plots/tunfold_tree_v3_percharge/run_rev5/bbb_compare_pos.pdf`, `bbb_compare_neg.pdf` | 4-page BBB vs IBU vs TUnfold model uncertainty |

## 7. KNOWN OPEN ISSUES (the work for the next session)

### Issue A — IBU acceptance correction broken at rev5

**Symptom**: IBU rev5 with rapgap response gives `unfolded_total = 1.470 × 10⁶`;
with djangoh response gives `1.470 × 10⁶` (identical to 6 digits). Both equal
the bg-subtracted data total `data_phys = 1.476 × 10⁶`. The MC choice has zero
effect on the integrated answer; per-bin median model uncertainty is 30.6 %
but driven by a single bin with IBU_rapgap ≈ 0 producing 10¹²⁴ divide-by-zero
explosions in p90/max.

**Cause**: IBU's update formula `p_{n+1}[j] = (1/ε[j]) × Σ_i d[i] × P(j|i, p_n)`
expects an `ε[j]` that represents per-bin reco efficiency. In `ibu_core.py:100`
we compute `eps[j] = col[j] / prior[j]` with `col[j]` including the miss row
(which we set to non-zero at fill time per Britzger Encoding B). At rev5 the
data overflow bin is 0, so the data input total is `data_phys` not
`data_phys + miss_yield`. The IBU then enforces `Σ p_unfolded = data_phys`
regardless of `ε[j]` because `ε[j] ≈ 1` (col includes miss row) — the
acceptance correction is neutralized.

**Two paths to fix** (next session decision):

1. **Restore F1-style data overflow ONLY for IBU**: have IBU's `run_ibu.py` add
   `data[NB_R] = miss_yield_data_scaled = miss_row × data / matched` to the data
   it reads before feeding into `IBU.fit()`. TUnfold's data input stays at
   overflow=0 (rev5). One-line change in run_ibu.py.

2. **Re-define `ε[j]` for IBU as matched_reco_marginal[j] / prior[j]**, exclude
   the miss row from the col computation. This makes `ε[j]` the true per-bin
   acceptance, and IBU's `1/ε[j]` factor correctly applies acceptance correction
   even with data overflow = 0. Cleaner but requires editing `ibu_core.py`.

Either fix should bring IBU's integrated unfolded total to ~2.71 × 10⁶ (matching
the expected acceptance-corrected gen-pool scale), and IBU per-bin model
uncertainty should drop into the BBB regime (~few % median).

### Issue B — More-differential BBB to capture intra-bin migration

Standard BBB is a pure per-bin acceptance correction (`prior[j] / matched[j]`
applied to data aggregated to bin j). It does NOT capture migration — events
that move between bins are corrected away by the diagonal factor only.

Three options to make BBB more differential:

1. **BBB with bin-wise migration probabilities**: instead of summing data over
   the 4 fine reco bins that map to each coarse gen bin, weight each fine reco
   bin by the per-fine-reco MC migration probability into that gen bin. For
   each gen bin `j`, BBB becomes `Σ_r data[r] × P(gen=j | reco=r)` then divided
   by `acceptance[j]`. This is "weighted BBB" or "matrix BBB" — uses the same
   binmap that TUnfold uses but applies the MC migration model bin-by-bin
   rather than globally.

2. **Two-step BBB**: first unfold migrations within (cell, z) groups using a
   small 5×5 (or 4×4 with subdivision) matrix per group, then apply the
   acceptance correction per coarse gen bin. This factorizes the problem into
   intra-group migration + bin-level acceptance.

3. **Use the fine reco binning explicitly**: BBB[fine] = `data_fine × prior_fine_implied / matched_fine`,
   then aggregate to coarse gen. This requires a fine-gen prior which we
   don't directly have — would need to subdivide `prior_gen` by the response
   matrix's column structure.

Option 1 is the textbook "extended BBB" and is probably the right move.

### Issue C — OmniFold third leg still pending

OmniFold cluster 3559797 (unfold, 2 jobs, 32 GB) + 3559798 (closures, 4 jobs,
48 GB) re-submitted after the data-extract fix. Expected wall 4-12 h. Once
done:
- Pull `output/omnifold/data_unfold_<mc>_iter5/` + `output/omnifold/closure_*_iter5/`
- Add OmniFold as the third leg in `scripts/analysis/compute_bbb_and_compare.py`
- Re-run the 3-method (now 4-method) per-bin model-uncertainty comparison

The OmniFold pipeline is at `scripts/omnifold/` with full Condor wraps.

### Issue D — Frozen τ choice not yet quoted

Best τ by majority-of-bins (8/240 pass all gates): **1.44 × 10⁻⁶**.
Best by composite score: 1.62 × 10⁻⁵.
Best by smoothness alone: 7.85 × 10⁻². Per the user's instinct, **1.44e-6 is
the working choice** but should be cross-checked against the model-uncertainty
trajectory: σ_model is monotonically decreasing from τ = 1e-7 to τ = 1.6e-5
(15.98 % → 13.97 % median). The sweet spot may actually be 1e-5 to 5e-6 — but
it's a marginal choice and 1.44e-6 is defensible.

### Issue E — S3b cell (high Q², high x)

10 untrusted bins (pos), 15 untrusted (neg) — concentrated in iz=3-5 high-z
high-pT_h corners where MC reco efficiency drops to < 1 % and MC unweighted
count drops to ~200. Quarantine these bins from publication or extend MC
sample if more statistics are available.

## 8. Working τ value and the four σ components

```
τ_canonical    = 1.4384498883 × 10⁻⁶
τ_up           = 2.8768997766 × 10⁻⁶  (for σ_reg)
τ_down         = 7.1922494415 × 10⁻⁷  (for σ_reg)
σ_stat         = TUnfold cov diagonal at τ_canonical
σ_reg          = max(|n(τ_up) - n(τ_can)|, |n(τ_down) - n(τ_can)|)
σ_model        = |n_rapgap_response(τ_can) - n_djangoh_response(τ_can)|
σ_total        = √(σ_stat² + σ_reg² + σ_model²)
```

## 9. Forbidden actions

- **Do NOT restore the F1 area-constraint hack** for TUnfold (`merge_migration_per_charge.py`).
  The rev5 convention (data overflow = 0, kEConstraintNone) is the correct one
  per the user's "let the unfolding do its job" directive. Restoring F1 sends
  TUnfold's unfolded total to `data / reco_eff²` again.
- **Do NOT use `kEConstraintArea` in the TUnfold macro** with the current R
  (column sums = `prior_gen[j]` in absolute MC weight units). The combination
  is exactly what produced the 5.06 × 10⁶ overshoot in rev4.
- **Do NOT change the matching code** (`build_migration_th2_per_charge.py`,
  inlined-gen matching via signed-PDG consumed-key) — the 0.55 reco efficiency
  is correct per the |η_gen|<4 vs |η_reco|<1.3 mismatch.
- **Do NOT change the fiducial cuts** (Option C: `xF_Trento > 0` +
  `|GenEta| < 4` + `35 < Empz < 70` + Q² > 150 + 0.1 < y < 0.7). The published
  cross-section depends on this exact choice.

## 10. Cross-references

- **Full session iteration history**: `H1/sidis_highQ2/context-handoff.md` § "Unfolding pipeline v2 (rev0→rev5)" (added 2026-05-31)
- **TUnfold-paper-based optimization research**: `H1/sidis_highQ2/docs/research/research-tunfold-optimization-from-schmitt-paper-2026-05-31.md`
- **F1 normalization research artifact**: `H1/sidis_highQ2/docs/research/research-F1-area-constraint-normalization-2026-05-31.md`
- **Critic verdict on rev3 pipeline**: `H1/sidis_highQ2/docs/critiques/critique-tunfold-v3-percharge-2026-05-31.md`
- **Cross-analysis handoff to low-Q² supervisor**: `H1/sidis_lowQ2/docs/handoffs/tunfold_official_cxx_handoff_from_highQ2_2026-05-31.md`
- **Charge confusion diagnostic**: `H1/sidis_highQ2/data/diagnostics/charge_confusion_v3/charge_confusion_summary.json` (0.35 % integrated)
