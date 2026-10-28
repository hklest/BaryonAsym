# Synthesis — 2-round chain audit of treebuilder → unfolding

**Date:** 2026-05-31 (after rev6 NWfix complete)
**Method:** 10 round-1 critics × 5 problem areas × 2 roles (implementation + physics) = 79 findings. Round 2 cross-examination converged to 17 actionable items.

**Composite verdict: CONCERN with high-confidence fix paths.**

The pipeline is internally consistent (closure tests pass, NW asymmetry fixed) but has **structural mismatches** between methods (TUnfold vs IBU fake handling) and **missing systematics** (data_w2 propagation, σ_chargeconf, σ_NW, σ_hadron_mass).

---

## The 5 problem areas

| Area | Topic | Critics | Round-1 findings | Round-2 confirmed |
|---|---|---|---|---|
| 1 | Event + track cuts | impl + physics | 21 | 5 |
| 2 | Gen-reco matching + fid | impl + physics | 17 | 4 |
| 3 | Frame computation + kinematics | impl + physics | 12 | 2 |
| 4 | Weights + normalization | impl + physics | 13 | 4 |
| 5 | Unfolding semantics + propagation | impl + physics | 16 | 2 (high-impact) |

Round-1 critic files: `area<N>-{cuts,matching,frames,weights,unfolding}-critic-{impl,physics}.md`.
Round-2 synthesis: [ROUND2_ITERATION.md](ROUND2_ITERATION.md).

---

## Tier 0 — Production-blocking fixes (1-2 days work + Condor)

### F1. Harmonize TUnfold/IBU fake-handling semantics (C5F1)

**Problem:** TUnfold rev5 keeps R_full and handles fakes internally via the response's fake column; IBU rev6 slices off the fake column and pre-subtracts fakes via `_fake_subtract.py`. Both happen to give 3.19-3.20e6 integrated for h+, but this is COINCIDENTAL given the different treatments.

**Fix:** Restore `kEConstraintArea` in TUnfold rev6 (per rev6 audit TB1) AND consume `data_inner` from `_fake_subtract`. Both methods then use the same fake handling. Re-run TUnfold rev6 → quantify the resulting headline shift.

**Impact:** Method-comparison plots become physically meaningful (currently misleading).

### F2. Plumb `data_w2` through fake-subtract → IBU → BBB (C4F1)

**Problem:** `_fake_subtract.py:108` uses Poisson on `data_fine` for σ_stat propagation. Real `data_fine` has variance encoded in `h_data_bgsub_<chg>_w2` (correlations from upstream bg subtraction). Using Poisson under-estimates σ_stat by 10-30% per bin.

**Fix:** Add `data_w2` parameter to `fake_subtract_data()`. Use as variance instead of Poisson. Wire through IBU bootstrap (use Gaussian with `data_inner_var` instead of Poisson on `data_inner`) and BBB.

**Impact:** σ_stat headline is currently UNDER-estimated by 10-30%. After fix, σ_stat headline rises accordingly.

---

## Tier 1 — Per-bin contamination fixes (1-line changes, 1-day work total)

### F3. Add `not_v0_match` to `pass_reco_full` (C2F2)

V0-daughter reco tracks currently fall to the fake column. Fake-subtraction then over-subtracts by ~1-2% globally. **Fix:** add `not_v0_match` to the `pass_reco_full` AND in line 447 of `build_migration_th2_per_charge_NWfix.py`. Rebuild migration ROOTs.

### F4. Add `GenPt > 0.15` to gen cuts (C1M2)

Reco cuts Pt > 0.15 GeV but gen has no Pt cut → low-Pt gen tracks enter prior but cannot match → forced to miss → ε[j] artificially low in lowest-Pt bin (5-10% bias). **Fix:** add `(GenPt > TRACK_CUTS_RECO["Pt_min"])` to gen-side cuts. Rebuild.

### F5. Clip z to [0,1] symmetrically at gen and reco (C1M1)

Reco currently drops `z<0 OR z>1` outliers; gen has no such cut. Asymmetric → 0.5% bias at z=1 edge bin. **Fix:** clip both sides instead of dropping at reco.

### F6. Increase consumed-key rounding to 8 decimals (C2F1/C2P1)

Current 5-decimal rounding can cause hash collisions in dense kinematic regions, causing wrong gen-bin assignments. **Fix:** change `round(GenPt, 5)` to `round(GenPt, 8)` (and similarly for GenEta) at 4 locations in `build_migration_th2_per_charge_NWfix.py`.

### F7. Symmetrize matched-fill charge assignment to gen_chg (C2P2 = rev6 CC2 mechanism)

**Important discovery:** Currently matched fill uses `reco_chg`, miss/prior uses `gen_chg`. Charge-confused matched events go to wrong-charge col but their gen partners stay in right-charge prior → per-charge ε[j] can exceed 1 in bins where charge confusion is asymmetric. **Fix:** change line 675 from `rc = int(reco_chg[k])` to `rc = int(gen_chg_inl[k])` for matched events (keep `reco_chg` for fakes since they have no gen). One line.

**Impact:** ε[j] becomes pure geometric acceptance, free of charge confusion. Charge confusion then enters as separate σ_chargeconf systematic (Tier 2).

---

## Tier 2 — Missing systematics (~1 week total)

### F8. σ_fakemodel from |RG_fakefrac − DJ_fakefrac|/2 per bin (C4M1)

Closed-form `scale = data/(matched+fake)_MC` forces RAPGAP-prediction agreement with data. Real data-vs-MC fake-fraction can differ by 20%. **Fix:** compute per-bin σ_fakemodel and propagate.

### F9. σ_hadron_mass from species composition (C3P1)

All hadrons treated with M_PION; real hadrons are pi/K/p with different masses → z computed wrong by 1-3% in high-z. **Fix:** quantify with species composition uncertainty (RG vs DJ).

### F10. σ_NW = 30% × (NW − 1) per event (C4P2)

Nuclear-correction weight is itself an estimate with ~30% uncertainty. Currently not propagated. **Fix:** vary NW by ±30% × (NW−1), propagate.

### F11. σ_VtxZ_cut (C1F1)

VtxZ < 35 cm cut has no gen counterpart → 0.3% acceptance bias. **Fix:** vary cut to ±5 cm, propagate.

### F12. σ_MC_stat from two-tier bootstrap (C5M1)

Response matrix MC stat is currently NOT propagated. Adds ~1-3% per bin. **Fix:** add outer-loop bootstrap on R alongside inner-loop on data.

### F13. σ_chargeconf (= rev6 CC2)

After F7 (Tier 1), charge confusion is the residual systematic. **Fix:** quantify per bin from MC truth vs reco charge correlation.

---

## Tier 3 — Methodology improvements (depends on publication scope)

### F14. Switch to (RG+DJ)/2 nominal + symmetric σ_model (C5P3)

Currently RAPGAP is nominal, σ_model = |RG−DJ|/2. Treats both MCs as equally likely → over-estimates σ_model by up to 50%. **Fix:** use average as nominal, symmetric half-spread as σ_model. Matches HERA convention.

### F15. End-to-end toy MC for σ_total correlation validation (C5P1)

Quadrature ANSATZ (σ_total² = σ_stat² + σ_reg² + σ_model²) assumes zero correlation. Real correlations can shift σ_total by 20-50%. **Fix:** 100-toy MC end-to-end (data + response + prior fluctuations) → take std of unfolded → quote as benchmark vs quadrature.

### F16. Joint pos+neg unfolding for A_h headlines (C5P2)

Per-charge unfolding under-estimates σ correlations → over-estimates σ(A_h) by up to 2×. Required for A_h publication. **Fix:** re-architect IBU to handle joint pos+neg response.

---

## Tier 4 — Cosmetic / observability (~half-day each)

| # | Fix | File |
|---|---|---|
| F17 | Empz event-scalar uniqueness assertion | build_migration:603 |
| F18 | Log `n_clipped` count in `_fake_subtract` | _fake_subtract:104 |
| F19 | Bootstrap covariance use central not bootstrap mean | ibu_core_rev6:166 |
| F20 | Drop sentinel-row matched events from response (C2F3) | build_migration:557 |
| F21 | Log NW-clipped events (C2F4) | build_migration:483 |
| F22 | Document mixed sign convention (xF γ*-z vs pT_h Trento) | build_migration:432 |
| F23 | Document M_PION inclusive approximation | build_migration:129 |
| F24 | Replace b2_safe clipping with hard assert | build_migration:142 |

---

## NOT broken (round 2 confirmed)

- xF sign convention (verified via reading C++ stage 2 source code)
- Frame computation formulas (`_boost_to_W_rest`, `_compute_pTh_cm_and_z`)
- IBU rev6 self-closure (machine precision)
- BBB matrix self-closure (perfect)
- NW asymmetry fix (CC1 from previous audit)
- Trust mask criteria

---

## Recommended sequence

**This week (Tier 1 + Tier 0):**
1. Apply 5 one-line Tier 1 fixes (F3-F7) in `build_migration_th2_per_charge_NWfix.py`. Rebuild migration ROOTs on Condor. Re-merge, re-run IBU/BBB.
2. Implement Tier 0 fixes (F1, F2). Re-run TUnfold rev6 with kEConstraintArea + same fake handling as IBU.

**Next week (Tier 2):**
3. Add σ_fakemodel (F8), σ_hadron_mass (F9), σ_chargeconf (F13). These are the main missing systematics.
4. Optionally σ_NW (F10), σ_VtxZ (F11), σ_MC_stat (F12).

**Before publication (Tier 3):**
5. End-to-end toy MC (F15) for σ_total correlation validation.
6. Switch σ_model convention (F14).
7. If A_h is in scope: joint pos+neg unfolding (F16).

**Anytime (Tier 4):**
8. Documentation + observability improvements (F17-F24).

---

## Cross-references

- 10 round-1 critic reports: this directory, `area<N>-<topic>-critic-{impl,physics}.md`.
- Round-2 iteration synthesis: [ROUND2_ITERATION.md](ROUND2_ITERATION.md).
- Earlier rev6 audit synthesis: [../rev6_audit_2026-05-31/SYNTHESIS.md](../rev6_audit_2026-05-31/SYNTHESIS.md).
- NWfix results doc: [../../results/nucliaweight_fix_2026-05-31.md](../../results/nucliaweight_fix_2026-05-31.md).
- LESSONS_LEARNED: [../../LESSONS_LEARNED.md](../../LESSONS_LEARNED.md) (will be updated with Tier 1+2 findings post-fix).
