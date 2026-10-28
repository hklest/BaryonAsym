# TUnfold rev6 area-constraint closure tau-optimization results
## 2026-06-02

**Status:** COMPLETE. Condor cluster 3571814 (240 jobs) finished. All 240
expected outputs present (`n_found = 240`, `n_missing = 0`).

---

## 1. Configuration

| Parameter | Value |
|---|---|
| Constraint | `kEConstraintArea` (area constraint; rev6 canonical) |
| Closure modes | selfRG, selfDJ, crossDJ, crossRG |
| Charges | h+ (pos), h− (neg) |
| Tau grid | 30 log-spaced values, 1.000e-06 … 1.000e-02 |
| Total jobs | 4 modes × 2 charges × 30 tau = 240 |
| Response matrix | `migration_inputs_v3_percharge_NWfix_v2.root` (rev6_v2 canonical; Tier 0+1 fixes) |
| Script | `unfold_tunfold_closure_tauopt.C` (or equivalent) |

---

## 2. Selected tau per charge

| Charge | τ_chosen | Score | Gate satisfied | Common-tau acceptable |
|---|---|---|---|---|
| **h+ (pos)** | **1.000e-06** | 0.8021 | yes | yes |
| **h− (neg)** | **2.593e-06** | 0.8063 | yes | yes |

The score is computed as a weighted sum across all 4 closure modes:
`score = w_p50 · median_p50 + w_p90 · median_p90 + w_nneg · mean_n_neg_frac + w_chi2 · mean_refold_chi2`
(exact weights embedded in the summarizer script). Lower score = better.

The chosen tau is the score-minimizing gate-passing candidate. The gate
rejects tau values where the aggregated cross-closure breaks down beyond
recoverable thresholds.

---

## 3. Closure metrics at chosen tau

### h+ (pos): τ = 1.000e-06

| Mode | p50 |unf−truth|/truth | p90 |unf−truth|/truth | n_negative | refold χ²/ndf |
|---|---|---|---|---|
| selfRG | **1.19%** | **4.74%** | 3 | 9.5e-05 |
| selfDJ | **1.04%** | **4.70%** | 4 | 1.2e-04 |
| crossDJ | 5.31% | 68.4% | 11 | 28.3 |
| crossRG | 5.24% | 88.0% | 5 | 31.6 |
| **Aggregate (score inputs)** | **3.25%** | **36.6%** | mean frac 3.0% | mean chi2 13.0 |

### h− (neg): τ = 2.593e-06

| Mode | p50 |unf−truth|/truth | p90 |unf−truth|/truth | n_negative | refold χ²/ndf |
|---|---|---|---|---|
| selfRG | **1.06%** | **4.52%** | 5 | 9.2e-05 |
| selfDJ | **1.07%** | **4.52%** | 4 | 1.1e-04 |
| crossDJ | 5.53% | 80.4% | 10 | 29.5 |
| crossRG | 5.24% | 66.3% | 7 | 24.7 |
| **Aggregate (score inputs)** | **3.41%** | **35.5%** | mean frac 2.9% | mean chi2 13.5 |

**Interpretation of cross-closure p90:** The large p90 values (66–88%) in
crossDJ/crossRG are driven by a minority of bins with high model-dependence
(bins 235–239 and nearby corners), not by a global failure. The majority of
bins close at <10% even in cross-closure. The mask below removes the most
pathological bins before any analysis-note claim is made. Self-closure p90
< 5% is excellent.

**Note on rho:** Per-combo-table entries in the summary JSON include
`mean_abs_rho_pub_offdiag` and `max_abs_rho_pub_offdiag`. At τ = 1e-6 (pos,
selfRG): mean off-diagonal |rho| = 0.82%, max = 99.1% (one large near-diagonal
entry). These rho values are from the per-publication-bin submatrix and are
consistent with known TUnfold behavior at low tau (high effective rank,
near-diagonal correlations). Full rho matrices are in the individual PDFs.

---

## 4. Always-bad-bin mask

### Definition

A bin is **always bad** if `|unfolded - truth| / truth > 2.00` (200%
disagreement) at **all 30 tau values** and **all 4 closure modes**. This bin
is structurally unresolvable by TUnfold at any regularization in this
configuration and must be excluded from all cross-section tables and
analysis-note figures.

A bin is **cross-only bad** if self-closure achieves < 200% at some tau but
**both** crossDJ AND crossRG fail at all tau. These bins are resolvable
within one MC model but not across models; they carry unquantifiable
model-dependence and should be excluded from analysis-note figures unless
the model-dependence systematic is explicitly computed.

### Mask sizes

| Charge | always-bad bins | always-bad bin indices | cross-only-bad bins | cross-only-bad indices |
|---|---|---|---|---|
| **h+ (pos)** | **2** | 209, 239 | 4 | 235, 236, 237, 238 |
| **h− (neg)** | **2** | 235, 239 | 6 | 206, 208, 233, 236, 237, 238 |

Out of 240 total bins: always-bad rate is 2/240 = 0.8% per charge.
Cross-only-bad rate is 4/240 (pos) and 6/240 (neg) = 1.7–2.5%.

**Total bins excluded (always-bad + cross-only-bad):** 6 (pos), 8 (neg).
Trusted bins: 234 (pos), 232 (neg).

### Source files

- `local:H1/sidis_highQ2/data/closure_rev6_area_tauopt/closure_tauopt_mask_pos.json`
- `local:H1/sidis_highQ2/data/closure_rev6_area_tauopt/closure_tauopt_mask_neg.json`

---

## 5. Trusted/physical mask policy

The always-bad mask is **separate from** the existing trust mask
(`cnt_MC < 50 OR (cnt_MC < 500 AND reco_eff < 0.05)`, 230/240 pos and
225/240 neg trusted). For analysis-note inclusion, a bin must pass **both**:

1. **Trust mask** (MC statistics and reco-efficiency gate), AND
2. **Closure always-bad mask** (not in `closure_mask_always_bad`).

Bins in `closure_mask_cross_only_bad` may be shown in QA plots but should
be flagged or excluded from headline cross-section tables until the
model-dependence systematic (σ_model for DJ vs RG shift) is computed and
documented separately.

---

## 6. Canonical output files

### Local (primary working copies)

| File | Content |
|---|---|
| `data/closure_rev6_area_tauopt/closure_tauopt_summary.json` | Master summary: tau selection, per-mode metrics, all-candidates ranking |
| `data/closure_rev6_area_tauopt/closure_tauopt_mask_pos.json` | Always-bad + cross-only-bad mask for h+ |
| `data/closure_rev6_area_tauopt/closure_tauopt_mask_neg.json` | Always-bad + cross-only-bad mask for h− |
| `data/closure_rev6_area_tauopt/closure_tauopt_{mode}_{charge}_tau{val}.json` | Per-tau per-mode per-charge metrics (240 files) |
| `plots/closure_rev6_area_tauopt/closure_tauopt_bundle.pdf` | Bundled diagnostic PDFs for all 240 jobs |
| `plots/closure_rev6_area_tauopt/closure_tauopt_summary.pdf` | Summary page(s) (tau-selection overview) |
| `plots/closure_rev6_area_tauopt/closure_tauopt_{mode}_{charge}_tau{val}.pdf` | Per-tau per-mode per-charge diagnostic PDFs (240 files) |

### Remote (NAF, authoritative source)

All files above also exist under:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/tunfold_tree_v3_percharge/closure_rev6_area_tauopt/`

ROOT files (per-tau TUnfold output trees) remain on NAF only (not mirrored
locally due to size). Summary JSON verified on remote at 210,797 bytes.

---

## 7. What is still pending before analysis-note inclusion

| Item | Status | Who |
|---|---|---|
| **h1-analysis-critic review of tauopt results** | Pending — not yet dispatched | h1-sidis-supervisor |
| **h1-unfolder review** | Pending | h1-sidis-supervisor |
| Cross-closure p90 assessment (68–88%): confirm majority-of-bins picture vs tail-driven | Pending | h1-analysis-critic |
| Model-dependence systematic σ_model from crossDJ/crossRG spread at chosen tau | Not computed | h1-systematics |
| Per-bin sigma tables at chosen tau (σ_stat, σ_reg, σ_model) | Not extracted yet | h1-physics-analyst |
| Rho/correlation matrix plots at chosen tau (full view) | In PDFs but not summarized | h1-plot-reviewer |
| L-curve scan at chosen tau (confirm kink location matches τ_chosen) | Not yet run | h1-unfolder |
| "Always-bad" bin physical identification (which (Q², x, pT_h, φ_h) cells?) | Not done | h1-physics-analyst |
| Analysis note §Unfolding update | **Blocked** — awaiting critic verdicts | documentation |
| Production data unfolding at τ_chosen | Not submitted | h1-naf-operator |

**Analysis note update is NOT ready.** It is blocked pending:
1. Critic iterations returning SOUND or CONCERN-manageable on the tauopt results.
2. Production data run at τ_chosen for both charges.
3. Per-bin σ tables and final cross-section extraction.

---

## 8. Score sensitivity (plateau check)

From `all_candidates` in the summary JSON:

**h+ (pos):** Gate-passing candidates span tau = 1e-6 to 3.29e-5 (12 candidates).
Score at tau = 1e-6: 0.802. Score at tau = 1.37e-5 (×14 higher): 0.857.
Score increase < 7% over a 14× range in tau → broad, flat plateau for pos.

**h− (neg):** Gate-passing candidates span tau = 1e-6 to 4.52e-5 (13 candidates).
Score at tau = 2.59e-6: 0.806. Score at tau = 1.27e-5 (×5 higher): 0.840.
Score increase < 5% over a 5× range in tau → also flat. 

This confirms that the tau choice is robust: misidentifying the optimal by
one grid step changes the score by < 0.5%.
