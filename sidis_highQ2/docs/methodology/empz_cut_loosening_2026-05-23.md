# Empz and y-cut loosening for H1 high-Q² charged-particle SIDIS

- **Date:** 2026-05-24
- **Status:** CURRENT methodology / production-state memo
- **Scope:** Documents the cut-state transition that became current on 2026-05-23: Stage 1 Empz widened to `[35,70]`, downstream extractors updated to `0.1 < y < 0.7`, and the current interpretation boundaries for those changes.

## 1. What changed

### Stage 1

`PhysicsConstants.h` in the SIDIS-local Stage 1 source now uses:
- `EMPZ_MIN = 35`
- `EMPZ_MAX = 70`

This supersedes the older high-Q² default inherited from the g1j_ES / v5f era:
- `EMPZ_MIN = 45`
- `EMPZ_MAX = 65`

The Stage 1 production binary still keeps wider headroom on y:
- `Y_MIN = 0.1`
- `Y_MAX = 0.9`

That headroom is intentional so downstream analyses can apply their own
analysis-level y recut without forcing a new Stage 1 production every time.

### Downstream extractors

The SIDIS downstream extractors were updated to:
- `CUT_Y_HI = 0.7` (was 0.6)
- `CUT_EMPZ_LO = 35`
- `CUT_EMPZ_HI = 70`

A total of **23 SIDIS downstream extractors** were updated to the widened cut state.

## 2. Observed effect

Smoke-level bookkeeping showed:
- about **+7.7%** more data events passing under the widened Empz cut
- about **+7.4%** more data events surviving after the downstream `y < 0.7` cut

These gains are large enough that any document or result derived under the
older `y < 0.6` / tighter-Empz regime must be treated as belonging to the
superseded v5f-era analysis state.

## 3. Interpretation

These changes are **not** just a plotting-level cosmetic recut.
They affect:
- event yields
- period accounting
- bin occupancies
- response / acceptance inputs
- radiative-correction inputs
- all downstream observables (`M`, `σ`, `A_h`, `A_BC`, `R_BC`, `A_n`)

Therefore the widened-cut state is part of the reason all existing v5f-era
results are now labeled **PRELIMINARY (pre-v5g)**.

## 4. Relationship to current production state

The current high-Q² SIDIS production state is defined by the combination of:
1. SIDIS-local v5g binaries,
2. widened Empz `[35,70]`,
3. downstream `0.1 < y < 0.7`,
4. per-period GRL injection,
5. ep0304 production re-inclusion,
6. pending adoption of Scheme C / `HQ-v5g-r1` binning.

Do not describe any earlier cut state as “current” without an explicit
historical qualifier.
