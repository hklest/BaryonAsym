# Binning adoption: HQ-v5g-Bprime (Scheme B′)

**Date:** 2026-05-24
**Status:** FROZEN (FINAL) by supervisor decision, 2026-05-24
**Supersedes:** all prior v5g binning candidates (Scheme A through F, HQ-v5g-r1)
**Production tag:** v5g 4-period (ep0304 + em0405 + em06 + ep0607)

## (Q², x) binning — Scheme B′ — 8 cells, charge-separated downstream

| Stratum | Q² range (GeV²) | x edges |
|---|---|---|
| 0 | [150, 300) | 0.0021, 0.0071, 0.030 |
| 1 | [300, 700) | 0.0042, 0.016, 0.069 |
| 2 | [700, 1500) | 0.0099, 0.034, 0.15 |
| 3 | [1500, 20000) | 0.021, 0.087, 1.5 |

## z binning (universal, unchanged from v5f-r2)

Edges: `[0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` → 6 bins.

## pT_h binning (universal, unchanged from v5f-r2)

Edges: `[0.0, 0.5, 1.0, 2.0, 4.0, 8.0]` → 5 bins.

## φ_h binning (newly adopted)

16 equal bins from −π to +π for harmonics analysis.

## Rationale

- Simplest of the candidate schemes evaluated in `binning_study_2026-05-24.md`
- Scheme B′ rebalancing (top stratum lower boundary moved 2000 → 1500 GeV²) already addresses the top-cell statistics concern that motivated finer Q² strata
- Scheme C is preserved as alternative for cross-checks

## Sample

4-period (ep0304 INCLUDED). The prior "ANALYSIS-DEFERRED" policy for ep0304 is no longer in effect.

## Canonical extractor

`extract_pertrack_v5g_fixedFrame_v3.py` — γ*+p CM-frame pT_h + φ_h, per-event BeamProtP.

## Downstream pipeline

- Acceptance
- Purity
- Migration
- `C_rad`
- Multiplicities
- `A_h`
- `φ_h` harmonics
- Systematics

## Cross-references

- `H1/sidis_highQ2/docs/methodology/binning_study_2026-05-24.md` (study report, with Scheme C as the planner's runner-up)
- `H1/sidis_highQ2/docs/methodology/binning_finalization_HQ-v5g-Bprime_v2_2026-05-24.md` (freeze-supporting evidence: per-cell data counts confirm red-band cells are overwhelmingly empty in data)
- `H1/sidis_highQ2/docs/methodology/binning_freeze_2026-05-24.md` (formal freeze memo)
- `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md` (frame fix; extractor v3)
- `H1/sidis_highQ2/docs/data/binning_v5f_frozen.json` (legacy v5f-r1/r2 binning specs plus frozen HQ-v5g-Bprime spec, preserved for audit)

## Finalization (2026-05-24)

The HQ-v5g-Bprime binning is now **FROZEN** and is the **FINAL** canonical binning for the current high-Q² charged-particle SIDIS analysis. No further changes to the `(Q², x)`, `z`, `pT_h`, or `φ_h` edges are to be made for the published-analysis pipeline.

Justification for the freeze is now stronger than at the time of the initial adoption: the v2 finalization study confirmed that the low-purity red-band cells (`P < 0.15`) overwhelmingly correspond to empty data phase space. Of the 32 mergeable red cells, 30 have `N_data = 0`, and the remaining 2 have only `N_data = 1` and `N_data = 2` respectively.

At the same time, **80.4%** of the 480 4D cells are BBB-valid (`purity ≥ 0.5`). The **9.0%** red-band cells are therefore understood as kinematic phase-space holes rather than active measurement bins: they contribute zero to any published cross section because there is nothing measurable there.

Supporting evidence is documented in `H1/sidis_highQ2/docs/methodology/binning_finalization_HQ-v5g-Bprime_v2_2026-05-24.md`, and the formal freeze decision is recorded in `H1/sidis_highQ2/docs/methodology/binning_freeze_2026-05-24.md`.
