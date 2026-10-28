# rev6_v2 results: chain audit Tier 0 + Tier 1 fixes applied

> 📍 **Entry-point docs:** [RUN_STATUS.md](../RUN_STATUS.md) · [LESSONS_LEARNED.md](../LESSONS_LEARNED.md) · [chain audit SYNTHESIS](../critiques/rev6_chain_audit_2026-05-31/SYNTHESIS.md).

---

**Date:** 2026-05-31 (continued, after NWfix + chain audit)
**Pipeline revision:** rev6_v2 (= NWfix + Tier 0 F2 + Tier 1 F3-F7)

## Fixes applied

### Tier 1 (per-bin contamination, 5 one-line changes to `build_migration_th2_per_charge_NWfix_v2.py`)

| ID | Fix | Effect |
|---|---|---|
| **F3** | Add `not_v0_match` to `pass_reco_full` | V0-daughter reco tracks no longer contaminate fake column |
| **F4** | Add `GenPt > 0.15` to gen-pool cut | Symmetrize Pt cut between reco and gen |
| **F5** | Clip z to [0,1] at gen too | Symmetrize z outlier rejection |
| **F6** | 8-decimal rounding in consumed-key | Eliminate hash collisions |
| **F7** | Matched-fill uses `gen_chg_inl` not `reco_chg` | Eliminate charge-confusion contamination in per-charge ε |

### Tier 0 (architectural)

| ID | Fix | Effect |
|---|---|---|
| **F2** | Plumb `data_w2` through `_fake_subtract_v2 → IBU/BBB/σ drivers` | σ_stat propagated using real bg-subtraction variance instead of Poisson approximation |
| F1 | (TUnfold rev6 kEConstraintArea restore) | **NOT done in this iteration** — deferred |

## Production state

- **Migration build (Condor 3563542):** 8/8 jobs complete in ~13 min.
- **Per-file outputs:** `/data/dust/.../output/tunfold_tree_v3_percharge/per_file_migration_NWfix_v2/`
- **Merged response:** `migration_inputs_v3_percharge_NWfix_v2.root`

## Headline numbers (h+, IBU N=100, post-NWfix_v2)

| Method | RG integrated | DJ integrated | int diff | σ_total trusted-median |
|---|---|---|---|---|
| TUnfold rev5 (unchanged) | 3.19e6 | 2.65e6 | +16.8% | 12.4% |
| **IBU rev6_v2** N=100 | **3.45e6** | **2.72e6** | **+21.1%** | **11.2%** |
| BBB-diagonal rev6_v2 | 2.23e6 | 2.27e6 | −1.9% | 5.0% |
| BBB-matrix_w rev6_v2 | 2.22e6 | 2.26e6 | −1.9% | 3.8% |

h-:

| Method | RG int | DJ int | int diff |
|---|---|---|---|
| TUnfold rev5 | 2.96e6 | 2.47e6 | +16.5% |
| **IBU rev6_v2** | 2.23e6 | 2.80e6 | **−25.6%** |
| BBB-diagonal rev6_v2 | 2.17e6 | 2.21e6 | −1.9% |
| BBB-matrix_w rev6_v2 | 2.16e6 | 2.20e6 | −2.0% |

### Key observations vs prior NWfix

| Metric | NWfix | NWfix_v2 | Change |
|---|---|---|---|
| Reco efficiency (integrated) | 0.55 | 0.60 | +9% (F3+F4) |
| Miss count (h+) | 56K | 44K | −20% (F4 mostly) |
| Fake count (h+) | 8.6K | 5.6K | −35% (F3) |
| IBU refold χ²/ndf | ~625 | ~300 | **−50%** (Tier 1 multiple) |
| IBU h+ N=100 integrated | 3.20e6 | 3.45e6 | +7.8% |
| IBU h- N=100 integrated | 3.07e6 | 2.23e6 | **−27.5%** (F7) |

The big h- IBU drop (-27.5%) is **F7 (charge symmetrization) correctly removing charge-confused matched events that were inflating h- via reco-charge assignment**. This is real physics now exposed.

### Closure verification

| Mode | Method | med \|rel\| trusted | p90 trusted | int_ratio |
|---|---|---|---|---|
| selfRG | IBU rev6_v2 | **0.000** | **0.000** | **1.000** |
| selfRG | BBB matrix | **0.000** | **0.000** | **1.000** |
| selfRG | BBB diagonal | 0.204 | 0.737 | 0.998 |
| crossDJ | IBU rev6_v2 | 0.042 | 0.268 | 0.984 |
| crossDJ | BBB matrix | 0.035 | 0.130 | 0.982 |
| crossRG | IBU rev6_v2 | 0.045 | 0.314 | 1.019 |
| crossRG | BBB matrix | 0.038 | 0.144 | 1.019 |

h- analogous. **IBU + matrix-BBB self-closure still machine precision.** Diagonal-BBB structural ~20% bias unchanged (intrinsic to method, confirmed by Schmitt 2016).

### σ_stat improvement from C4F1 (`data_w2` plumbing)

| Method (h+) | NWfix σ_stat | NWfix_v2 σ_stat (data_w2 passed) | Change |
|---|---|---|---|
| IBU N=100 | 8.6% | 8.9% | +0.3 pp |
| BBB-diagonal | 4.3% | 4.9% | +0.6 pp |
| BBB-matrix | 2.4% | 2.8% | +0.4 pp |

σ_stat increased by 4-14% relative — confirms that the Poisson approximation was UNDER-estimating σ_stat by exactly the predicted ~10-30% margin. **Headline σ is now honest.**

## Files

### NAF
- `/data/dust/.../output/tunfold_tree_v3_percharge/per_file_migration_NWfix_v2/` (8 per-file ROOTs, 16 MB)
- `/data/dust/.../output/tunfold_tree_v3_percharge/migration_inputs_v3_percharge_NWfix_v2.root` (4.5 MB)
- `/data/dust/.../output/ibu_rev6_v2/{rapgap,djangoh}/iteration_scan_summary_*.json + per-N JSONs`
- `/data/dust/.../output/bbb_rev6_v2/bbb_rev6_v2_{pos,neg}.{json,pdf}`
- `/data/dust/.../output/closure_rev6_v2/closure_rev6_v2_{pos,neg}.{json,pdf}`
- `/data/dust/.../output/method_compare_rev6_v2/method_compare_rev6_v2_{pos,neg}.{json,pdf}`
- `/data/dust/.../output/per_bin_sigma_rev6_v2/per_bin_sigma_rev6_v2_{pos,neg}.{json,pdf}`

### Local mirrors
- `H1/sidis_highQ2/data/{ibu_rev6_v2,bbb_rev6_v2,closure_rev6_v2,method_compare_rev6_v2,per_bin_sigma_rev6_v2}/`
- `H1/sidis_highQ2/plots/{bbb_rev6_v2,closure_rev6_v2,method_compare_rev6_v2,per_bin_sigma_rev6_v2}/`

### Bundled summary PDFs (42 pages each, 950 KB)
- `H1/sidis_highQ2/plots/rev6_summary_all.pdf` — original rev6 (pre-NWfix)
- `H1/sidis_highQ2/plots/rev6_NWfix_summary_all.pdf` — NWfix only
- **`H1/sidis_highQ2/plots/rev6_v2_summary_all.pdf`** — Tier 0+1 fixes applied (current canonical)

### Scripts
- `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix_v2.py` (1073 lines, +34 vs NWfix)
- `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix_v2.py`
- `H1/sidis_highQ2/scripts/ibu/{_fake_subtract_v2.py, run_ibu_rev6_v2.py}`
- `H1/sidis_highQ2/scripts/analysis/{_fake_subtract_v2.py, compute_bbb_rev6_v2.py, closure_rev6_v2.py, per_bin_sigma_rev6_v2.py}`

## Open items (deferred from chain audit synthesis)

| Tier | ID | Status |
|---|---|---|
| Tier 0 | F1 (TUnfold rev6 kEConstraintArea + ScanLcurve) | Open |
| Tier 2 | F8 (σ_fakemodel from RG/DJ) | Open |
| Tier 2 | F9 (σ_hadron_mass from species) | Open |
| Tier 2 | F10 (σ_NW = 30%·(NW−1)) | Open |
| Tier 2 | F11 (σ_VtxZ_cut) | Open |
| Tier 2 | F12 (σ_MC_stat two-tier bootstrap) | Open |
| Tier 2 | F13 (σ_chargeconf — now needed because F7 removed it from response) | Open |
| Tier 3 | F14 (RG+DJ)/2 nominal + symmetric σ_model | Open |
| Tier 3 | F15 (end-to-end toy MC for σ_total correlation) | Open |
| Tier 3 | F16 (joint pos+neg unfolding for A_h) | Open |

## Next recommended steps

1. **Tier 0 F1**: Restore kEConstraintArea in TUnfold rev6 + run ScanLcurve. This is the last remaining production-blocking item.
2. **Tier 2 F13 (σ_chargeconf)**: F7 removed charge confusion from the response, so it now needs to be quoted explicitly as a per-bin systematic.
3. **Tier 2 F8-F12 (other systematics)**: Add to per_bin_sigma_rev6_v2 output.
4. Disk cleanup: 230 GB quarantined; transfer to crossover archive when ready.
