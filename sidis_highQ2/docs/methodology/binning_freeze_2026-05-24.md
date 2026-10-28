# Binning freeze decision: HQ-v5g-Bprime (FINAL)

**Date:** 2026-05-24
**Status:** FROZEN — no further changes to the (Q²,x), z, pT_h, or φ_h edges
**Decision by:** Supervisor sign-off

## Bin definition

### (Q², x) binning — Scheme B′

| Stratum | Q² range (GeV²) | x edges |
|---|---|---|
| 0 | [150, 300) | 0.0021, 0.0071, 0.030 |
| 1 | [300, 700) | 0.0042, 0.016, 0.069 |
| 2 | [700, 1500) | 0.0099, 0.034, 0.15 |
| 3 | [1500, 20000) | 0.021, 0.087, 1.5 |

### z binning

Edges: `[0.0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` → 6 bins.

### pT_h binning

Edges: `[0.0, 0.5, 1.0, 2.0, 4.0, 8.0]` → 5 bins.

### φ_h binning

16 equal bins in `[-π, +π]`.

### Charge and cell count

- Charge states: `{+, -}`
- Total 4D cells: `8 × 6 × 5 × 2 = 480`
- Total with 16-bin `φ_h` axis: `8 × 6 × 5 × 16 × 2 = 7680`
- Production: `v5g` 4-period (`ep0304 + em0405 + em06 + ep0607`)
- Canonical extractor: `extract_pertrack_v5g_fixedFrame_v3.py`
- Canonical frame: `γ*+p` CM (extractor v3)

## Justification

- 80.4% of 480 4D cells are BBB-valid (purity ≥ 0.5).
- 9.0% are red (purity < 0.15), but 30/32 of these have N_data = 0 (empty data) and the other 2 have N_data ≤ 2. They contribute nothing to any published cross section.
- Geometric acceptance MC-model spread (DJANGOH vs RAPGAP) is ≤ 9% in any (Q²,x) cell and ~3.6% integrated — small enough to be absorbed by the unfolding closure systematic.

## Implications

- Downstream pipelines (extractors, BBB, IBU, TUnfold, OmniFold, systematics, plotting) all build on this binning.
- Future cross-checks may use alternative binnings as systematic studies, but the **published** result uses HQ-v5g-Bprime.

## Cross-references

- `H1/sidis_highQ2/docs/methodology/binning_adoption_HQ-v5g-Bprime_2026-05-24.md` — binning adoption memo, now superseded in status by this freeze decision
- `H1/sidis_highQ2/docs/methodology/binning_finalization_HQ-v5g-Bprime_v2_2026-05-24.md` — supporting evidence for the freeze decision
- `H1/sidis_highQ2/docs/methodology/pTh_phih_frame_bug_2026-05-24.md` — frame-fix memo; extractor v3 is required for this binning to be physically meaningful
