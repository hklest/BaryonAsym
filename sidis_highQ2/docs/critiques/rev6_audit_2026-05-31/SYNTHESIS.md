# Rev6 audit synthesis — IBU + TUnfold critical issues

> 📍 **Entry-point docs:** [../../RUN_STATUS.md](../../RUN_STATUS.md) (what's current) · [../../LESSONS_LEARNED.md](../../LESSONS_LEARNED.md) (structural insights).

---


**Date:** 2026-05-31 (continued)
**Critics:**
- `critic-ibu-1-implementation.md` (5 fatal, 4 major, 6 latent)
- `critic-ibu-2-physics.md` (3 fatal, 5 major, 2 assumptions)
- `critic-tunfold-1-implementation.md` (4 fatal, 6 major, 5 latent)
- `critic-tunfold-2-physics.md` (3 fatal, 4 major, 3 defensibility)

**Composite verdict:** **CONCERN with critical normalization issues that affect both methods.** The rev6 IBU and TUnfold are both internally consistent and pass closure, but BOTH have an upstream normalization problem (NucliaWeight asymmetry between matched-reco and prior fills) that biases the per-bin acceptance correction.

---

## Cross-cutting issues (affect BOTH IBU and TUnfold)

### CC1. **NucliaWeight asymmetry in build_migration_th2_per_charge.py is THE root cause of multiple downstream issues**

**Flagged by:** IBU critic-1 IB3, IBU critic-2 IP-M1, TUnfold critic-2 TP2.

Build_migration applies NucliaWeight to matched-reco fill (line 486: `w_reco = w1 * w2 * wn`) but NOT to gen prior fill (line 651: just `w_gen = w1 * w2`). Consequences:
- IBU `ε[j] = col[j]/prior[j]` can exceed 1.0 silently in some bins (matched has extra `<NW>` ~ 1.02).
- TUnfold area-constraint target (rev4 F1 formula) produced 5.06e6 = data/reco_eff² because of this asymmetry — leading to the rev5 workaround of dropping the constraint entirely.

**The whole rev3→rev4→rev5 cascade was caused by this single asymmetry.** Three rounds of critics flagged it. It is still not fixed.

**Fix priority: HIGHEST.** Either:
- Add NucliaWeight to the prior fill (need to verify it can be read from ChargedTotGen consistently).
- Remove NucliaWeight from the matched-reco fill (loses the nuclear correction but harmonizes scales).

**Estimated effort:** 1 line code change in build_migration, plus Condor production rerun of all migration ROOTs (1-2 hr CPU).

### CC2. **Per-charge unfolding leaves 0.35% charge-confusion as unaccounted systematic**

**Flagged by:** IBU critic-2 IP-M2, TUnfold critic-2 TP-M2.

Documented in rev5 critique-2 M1. Persists in rev6. Add to systematics budget OR move to joint pos+neg unfolding architecture.

### CC3. **Cell-boundary effects in 8-cell layout not validated for either method**

**Flagged by:** IBU critic-1 IM1 (silent dropping of low-ε bins), TUnfold critic-1 TB4 (custom L excludes cell-boundary smoothness), TUnfold critic-2 TP3 (L matrix shape untested).

Both methods may have systematic biases at cell boundaries that the per-bin σ does not capture. Need a comparison study.

---

## IBU-specific issues

### **Fatal**

| # | File:line | Issue | Fix effort |
|---|---|---|---|
| **IB1** | `_fake_subtract.py:108-111` | Variance double-counts MC and data Poisson | 1-day rewrite of variance propagation |
| **IB2** | `_fake_subtract.py:88` | Closed-form scale assumes data/MC fake-fraction match (untested) | Add σ_fakescale systematic |
| **IB3** | `ibu_core_rev6.py:107` | ε[j] > 1.0 possible due to CC1 (NucliaWeight) | Fixed by CC1 |
| **IB4** | `ibu_core_rev6.py:88-91` | Silent clip of negative data inputs | Add n_clipped log |
| **IB5** | `run_ibu_rev6.py` | Refold formula documentation lies (correct by accident) | Doc rewrite + assertion |
| **IP1** | (physics) | Fake subtraction conflates combinatorial + V0 + secondaries | Document or split |
| **IP2** | (physics) | Global per-charge scale untested vs data shape | Per-bin scale validation |
| **IP3** | (physics) | 1/ε for ε~0.05-0.10 amplifies error 10-20× | Tighten trust mask |

### **Major (priority for headline)**

| # | Issue | Severity |
|---|---|---|
| **IP-M3** | Closure at N=100 (canonical per tuning) not tested | Re-run closure_rev6 at N=100 |
| **IP-M4** | Q²_reco<60 spillover double-count risk | Verify upstream bgsub semantics |
| **IP-M5** | Refold χ²/ndf = 750 reported — suspicious | Verify formula labeling vs implementation |

---

## TUnfold-specific issues

### **Fatal**

| # | File:line | Issue | Fix effort |
|---|---|---|---|
| **TB1** | `unfold_tunfold_single_tau.C:439` | `kEConstraintNone` is non-standard for inclusive multiplicity | Restore kEConstraintArea + fix F1 (depends on CC1) |
| **TB2** | `merge_migration_per_charge.py:407` | F1 logic dead-code with misleading diagnostics | Delete or flag |
| **TB3** | `unfold_tunfold_single_tau.C:959-993` | `area_residual_rel` score term active under kEConstraintNone (wrong) | Conditional disable |
| **TB4** | `unfold_tunfold_single_tau.C:447-482` | Custom L matrix shape untested | Run comparison study |
| **TP1** | (physics) | No physical normalization anchor (without kEConstraintArea) | Restore via TB1 |
| **TP3** | (physics) | L matrix shape untested vs alternatives | TB4 |

### **Major**

| # | Issue | Severity |
|---|---|---|
| **TP-D1** | τ chosen by ad-hoc gate metric, not L-curve | Run ScanLcurve |
| **TP-D2** | σ_reg definition not standardized | Use definition (A) uniformly |
| **TP-M3** | σ_model evaluated at canonical τ where regularization interacts | Quote at plateau center |
| **TP-M4** | Refold χ²/ndf number not in evidence — could be anomalous | Inspect |

---

## Prioritized action list

### Tier 0 — blocks everything else

1. **Fix CC1 (NucliaWeight asymmetry) in `build_migration_th2_per_charge.py`.** Either add NW to prior fill OR remove from matched fill. Decide which one (depends on whether NW is meaningful at gen level — physics question for `h1-mc-expert`).
2. **Rebuild all migration ROOTs on NAF via Condor.** 4 MCs × 2 charges × ~600 files each → ~2-3 hr CPU.

### Tier 1 — needed before headline

3. **Re-run IBU rev6 (after CC1 fix) with N=100 canonical.** Confirm ε[j] ≤ 1 for all populated bins.
4. **Re-run IBU closure at N=100.** Confirm trusted-median |rel| = 0.000.
5. **Restore kEConstraintArea in TUnfold rev6** (`unfold_tunfold_single_tau_rev6.C` to be derived from rev5 macro with line 439 changed). Re-run τ scan.
6. **Run TUnfold ScanLcurve** to identify L-curve kink τ. Compare to manual canonical 1.4e-6.
7. **Re-derive σ_model at the L-curve τ** (likely smaller than 1.4e-6 in plateau center) for the σ headline.

### Tier 2 — defensibility / systematics

8. **Add σ_fakescale systematic** (IB2): vary fake-subtraction scale by ±20%, propagate.
9. **Document the fake subtraction semantics** (IP1): which physics categories are subtracted, what's left over.
10. **Run cell-boundary comparison study** (TP3, CC3): L matrix shape variants on equal footing.
11. **Add σ_chargeconf systematic** (CC2): bound from per-charge integrated confusion.

### Tier 3 — verification

12. **Verify refold χ²/ndf labeling** (IP-M5, TP-M4): both methods report values that look anomalous; ensure formula and ndf are consistent.
13. **Verify upstream bg subtraction stage** (IP-M4, TP-M1): does the legacy bg-sub remove at event or track level? Determines double-count risk.
14. **Quote σ_reg via definition (A) consistently** with documented scan spacing (TP-D2).

---

## Open recommendation on rev6 publishability

**Current state: NOT publishable.** The IBU and TUnfold rev6 results both have the CC1 normalization issue that propagates through to per-bin acceptance correction. The headline integrated number 3.19e6 (TUnfold) vs 3.20e6 (IBU N=50) vs 3.21e6 (IBU N=130) agree at the 1% level, but they all share the same upstream NucliaWeight asymmetry. Until CC1 is fixed, every headline number carries an unknown 0-10% per-bin bias.

**Path to publishable:** Tier 0 + Tier 1, in order. Estimated wall time: 1-2 sessions of focused work, mostly waiting on Condor rebuild after CC1 fix.

**What's NOT broken:** The user's rev6 directive ("rec-no-gen = background, gen-no-reco = acceptance") is correctly implemented in IBU. The matrix-BBB and diagonal-BBB are correctly implemented (modulo Schmitt 2016's caveat about EM-n=0). The closure tests at the current canonical (N=50) pass perfectly for IBU rev6 and matrix-BBB. The rev6 fix of the original IBU degeneracy IS a real improvement over rev5.

**What IS broken:** The bridge between the response matrix and the data — the NucliaWeight asymmetry that has been documented for three iterations and never fixed. **Fix that one bug and most of the audit findings cascade to "OK".**

---

## EMPIRICAL VERIFICATION (added post-synthesis)

The CC1/IB3 claim was that NucliaWeight asymmetry could push ε[j] > 1. **Empirical check on the actual h+ Rapgap response:**

| Statistic | Value |
|---|---|
| min ε[j] | 0.000 (5 zero-prior bins) |
| median ε[j] | 0.664 |
| max ε[j] | 0.967 |
| bins with ε > 1.0 | **0/240** |
| bins with ε > 1.05 | 0/240 |
| bins with ε > 1.10 | 0/240 |

**The NucliaWeight asymmetry is real in the code (line 357 vs 486) but does NOT empirically violate ε ≤ 1 for the current rev6 sample.** Either NucliaWeight is close to 1 for surviving events, OR the asymmetry is partially compensated by other effects.

**Revised CC1 severity:** Major concern (not fatal). The asymmetry is a known-correct-physics violation, but does not numerically break the rev6 IBU canonical at N=50 (max ε = 0.967 < 1.0). However, it COULD violate ε ≤ 1 in unconverged corners or with different MC samples (e.g., if djangoh has higher NW than rapgap). Recommend fixing it for cleanliness but it does not block publication.

**Other findings (IB1, IB2, TB1, TB2, TB3) remain as stated.** The TUnfold normalization issue (TB1: kEConstraintNone) is now the highest-priority blocker, NOT the NucliaWeight asymmetry.

## Revised priority

### Tier 0 — blocks headline

1. **Restore kEConstraintArea in TUnfold** (TB1). Run ScanLcurve (TP-D1). Re-derive σ_model at L-curve τ.
2. **Run IBU rev6 closure at N=100** (IP-M3). Confirm trusted-median 0.
3. **Verify upstream bg subtraction is event-level** (IP-M4, TP-M1). Eliminates double-count risk.

### Tier 1 — defensibility for review

4. Fix NucliaWeight asymmetry for cleanliness (CC1) — not numerically required but eliminates a known-incorrect-formula.
5. Add σ_fakescale systematic (IB2).
6. Add σ_chargeconf systematic (CC2).
7. Quote σ_reg via definition (A) consistently with documented scan spacing (TP-D2).

### Tier 2 — long term

8. Cell-boundary L matrix shape study (TP3, CC3).
9. Compare to published HERA SIDIS multiplicities (TP-D3).
