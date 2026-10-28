# Regularization-strength tuning: IBU rev6 vs TUnfold rev5

> 📍 **Entry-point docs:** [RUN_STATUS.md](../RUN_STATUS.md) (what's current) · [LESSONS_LEARNED.md](../LESSONS_LEARNED.md) (structural insights) · [rev6_audit/SYNTHESIS.md](../critiques/rev6_audit_2026-05-31/SYNTHESIS.md) (open issues).

---


**Date:** 2026-05-31 (continued)
**Question:** Did IBU's N-iter uncertainty really beat TUnfold's τ uncertainty, or was the previous σ_reg estimate using inconsistent definitions?

## Headline

The earlier rev6 σ_reg numbers ("IBU 0.2% trusted-median, TUnfold 0.1% trusted-median") were misleading. Both used "narrow neighbor" definitions in their own scan grids, but the two grids have very different step sizes (IBU N=30→50 is a factor 1.67; TUnfold τ=7e-7→3e-6 is a factor 4). **An apples-to-apples comparison says TUnfold has a much wider stable plateau than IBU**, but at their respective canonical operating points the per-bin σ_reg numbers are comparable.

## Three σ_reg definitions, all consistent across methods

For both IBU (regularization strength = number of iterations N) and TUnfold (regularization strength = Tikhonov parameter τ):

| Definition | What it measures |
|---|---|
| **(A) narrow neighbor** | \|unf(strength+1step) − unf(strength−1step)\| / 2 around canonical |
| **(B) stability window** | (max−min)/2 of unfolded across the ±factor-of-N or ±decade-in-τ window around canonical |
| **(C) rms over full scan** | std of unfolded across the entire bias-to-variance trajectory |

## Per-bin σ_reg trusted-median results (h+)

| Method | Definition (A) | Definition (B) | Definition (C) |
|---|---|---|---|
| IBU rev6 (canonical N=50) | 0.35% | 0.37% (N∈[30,80]) | 6.94% (N∈[1,130]) |
| TUnfold rev5 (canonical τ=1.4e-6) | 0.11% | 0.32% (τ∈[1.4e-7,1.4e-5]) | 42.8% (τ∈[1e-9,10]) |

## What this means

**The rms-over-full-scan numbers (definition C) are NOT honest σ_reg.** They include both methods' transitions through their pathological bias and variance regimes. Both are large because both methods scan past their stable plateau into the matrix-inversion-oscillation regime (large variance) and the over-regularized regime (large bias).

**The honest σ_reg is definition (A) at a chosen canonical point**: how much does the answer change for a small perturbation around the operating point.

## The real comparison: where does the stable plateau live?

A bin is "stable" if σ_reg/|unf| < 1% AND integrated total varies by < 2% across ±2 scan steps:

| Method | Stable plateau width (h+) | Stable plateau width (h-) |
|---|---|---|
| IBU rev6 | N ∈ [80, 130] (one decade) | N = 130 only (insufficient — needs more iterations) |
| TUnfold rev5 | τ ∈ [1e-9, 7e-7] (9 decades) | τ ∈ [1e-9, 1.4e-6] (4 decades) |

**TUnfold has a much wider stable plateau than IBU on both charges.** TUnfold pos sits flat at integrated total 3.19e6 for nine decades of τ before σ_reg rises above 1%. IBU's stable region is just one factor of ~2 in N around the converged value.

## Integrated total trajectory (h+)

| Method | At edge of plateau | At canonical | Beyond canonical |
|---|---|---|---|
| IBU N→1 | 2.37e6 (bias-dominated) | N=50: **3.20e6** | N=130: 3.21e6 (asymptotic) |
| TUnfold τ→0 | 3.19e6 (matrix-inversion plateau) | τ=1.4e-6: **3.19e6** | τ→∞: 0.10e6 (full bias to zero) |

**TUnfold canonical is INSIDE the plateau** (τ=1.4e-6 → 3.19e6, identical to τ=1e-9 → 3.19e6).
**IBU canonical is at the EDGE of the plateau** (N=50 → 3.20e6, still 0.5% below asymptotic 3.21e6 at N=130).

## Tuning gain available

### IBU rev6
1. **Move canonical N from 50 to 100-130.** This shifts IBU into its actual stable plateau. Expected gain: σ_reg drops from 0.35% to <0.1% per bin (trusted-median).
2. **Per-charge N tuning.** h- IBU has not converged by N=50 (only by N≥80). h+ is mostly converged at N=50 but the long-tail bins need more. Use the per-bin convergence diagnostic to set N where 95% of trusted bins have stabilized to 1%.
3. **Cost of larger N**: subsecond per IBU run, no operational issue.

### TUnfold rev5
1. **Current canonical τ = 1.4e-6 is at the L-curve transition.** The plateau extends to τ ≈ 1e-7 with the same integrated total. Choosing canonical τ in the plateau center (e.g., τ ≈ 1e-7) would give σ_reg < 0.01% (matrix-inversion-like, but stable here).
2. **kEConstraintArea restore** (the rev6 TUnfold path I haven't yet rerun): would tighten normalization but shift the plateau position. Worth running.
3. **L-curve scan with TUnfold's built-in ScanLcurve** would identify the kink automatically. Recommended.

### Both methods
4. **Quote σ_reg via definition (A) consistently** in all systematics tables. Definition (C) is misleading; definition (B) is informative for window-stability documentation but not for the "uncertainty due to regularization choice" headline.

## Updated recommendation

- **Use IBU rev6 at N=100** (not N=50) as the headline iteration count.
- **Use TUnfold τ = 1.4e-6** as the headline τ, but quote the L-curve-scan-derived τ as cross-check.
- **Quote σ_reg using definition (A) (narrow neighbor in the chosen scan)** for both methods in tables.
- **Cite the stable plateau width** as a robustness metric: TUnfold is more forgiving than IBU.

## Files

- `H1/sidis_highQ2/scripts/analysis/ibu_sigma_reg_scan.py` — IBU N-scan + σ_reg
- `H1/sidis_highQ2/scripts/analysis/tunfold_sigma_reg_scan.py` — TUnfold τ-scan + σ_reg
- `H1/sidis_highQ2/scripts/analysis/reg_strength_comparison.py` — apples-to-apples
- `H1/sidis_highQ2/data/ibu_sigma_reg/ibu_sigma_reg_{pos,neg}.json` — per-N unfolded vectors
- `H1/sidis_highQ2/data/tunfold_sigma_reg/tunfold_sigma_reg_{pos,neg}.json` — per-τ unfolded vectors
- `H1/sidis_highQ2/data/reg_strength_compare/reg_strength_{pos,neg}.json` — comparison summary
- `H1/sidis_highQ2/plots/reg_strength_all.pdf` — bundled PDF (6 docs, ~20 pages)
- NAF Condor cluster 3560441 (completed) — 25 new τ values for h-

## Open items

- **TUnfold rev6 (kEConstraintArea) full τ scan**: not yet run. Expected to confirm same plateau width with corrected normalization.
- **L-curve scan via TUnfold internals**: would auto-pick τ. Worth a single-shot dispatch.
- **IBU at N=100 + per-bin sigma decomp re-run**: trivial follow-up once canonical N is moved.
