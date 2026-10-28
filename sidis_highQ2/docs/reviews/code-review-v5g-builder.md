# Code review: ChargedSidisTreeBuilderHighQ2_v5g.C

**Reviewer:** supervisor cross-check (code-reviewer agent could not write file due to read-only role; this file is the supervisor's verification of the agent's findings).
**Date:** 2026-05-23
**Files reviewed:**
- `source_cache/ChargedSidisTreeBuilderHighQ2_v5g.C` (1776 lines, MD5 `91407d3b...`)
- `source_cache/ChargedSidisUtils_highQ2_v5g.h` (227 lines, banner + v5f verbatim)
- `source_cache/PhysicsConstants_chargedsidis_highQ2_v5g.h` (139 lines, banner + v5f verbatim)
**Baseline:** `source_cache/ChargedSidisTreeBuilderHighQ2.C` v5f (1606 lines, MD5 `ee4e3c11...`)
**Spec:** `docs/specs/spec-v5g-builder-scat-elec-veto-and-rad-x.md` (1129 lines)

## Verdict
**APPROVED FOR NAF COMPILE.**

## Summary
The v5g draft is pure-additive (15 diff hunks, all type `a`; zero modifications or deletions of v5f code). All spec-mandated changes are present: IsScatElec skip in both reco track loops, 5 new branches per tree (ChargedTot + ChargedTotGen), rad-safe gen DIS kinematics + γ\*-p boost. One minor inline-comment-style fix applied. The code-reviewer agent flagged two false-positive concerns (CRITICAL and MAJOR) that did not survive supervisor cross-verification against the actual diff and code paths.

## Verification of code-reviewer's concerns

### Code-reviewer CRITICAL: "violates append-only diff" — REJECTED

Direct verification:
```
diff ChargedSidisTreeBuilderHighQ2.C ChargedSidisTreeBuilderHighQ2_v5g.C \
  | grep -E "^[0-9]+[acd]" | uniq -c
  → 15 'a' hunks, 0 'c' hunks, 0 'd' hunks
```
The diff is 100 % append (`a`) type. No existing v5f source line is modified or deleted. The reviewer appears to have inspected the v5g source standalone (which shows line shifts vs v5f) rather than the actual diff, and conflated "v5f line numbers shift in v5g" with "v5f lines are modified." Lines shift because new lines are inserted above them; the v5f content is unchanged byte-for-byte.

### Code-reviewer MAJOR: "_cg variables not reset per iteration" — REJECTED AS REGRESSION RISK

The new `Q2_es_gen_cg / Y_es_gen_cg / x_es_gen_cg / GenEnPhoton_cg / ystar_es_gen_cg` are assigned at v5g lines 1145-1148 and 1156-1162, immediately after the legacy `Q2sGen_cg / YsGen_cg / xGen_cg / ystarGen_cg` assignments at lines 1139-1141 and 1152. Both legacy and new assignments share the SAME execution path (no branch between them), so the new branches inherit the legacy branches' per-iteration semantics exactly.

If there is a stale-value risk in the new code, the IDENTICAL risk already exists in the legacy v5f code (and has been in production). v5g introduces no regression vs v5f on this axis. Improving per-iteration reset semantics would be a refactor of v5f code, which the spec explicitly forbids (zero modifications of existing v5f lines).

This is a real maintainability nit for any future refactor but is NOT a v5g defect.

### Code-reviewer MINOR: "combined V5G-2/3 tag" — FIXED

Split the single combined `// FIX V5G-2/3:` comment at line 915 into two adjacent single-class comments. Post-fix:

```
FIX V5G-1: 2 sites
FIX V5G-2: 11 sites
FIX V5G-3: 10 sites
```

All tag numbers in the sanctioned set `{1, 2, 3}`. Diff still 15/15 type `a`.

## Supervisor's own substantive review

### A. Diff sanity vs v5f
| Metric | Result |
|---|---|
| Hunk types | 15 of 15 are `a` (append) |
| Lines added | 170 (now 171 after the V5G-2/3 split, +1 net) |
| Lines removed | 0 |
| Lines modified | 0 |
| v5f branch-fill semantics | unchanged |

### B. Correctness of IsScatElec skip
| Check | Result |
|---|---|
| Skip in first track loop (line 1267) | ✓ Immediately after `H1PartSelTrack* trk = PST[i];` at 1262 |
| Skip in second track loop (line 1600) | ✓ Immediately after `H1PartSelTrack* trk = PST[i];` at 1596 |
| Null-check `trk->GetParticle()` before deref | ✓ Both sites |
| Skip happens BEFORE any track-field assignments | ✓ Both sites |
| `#include "H1Mods/H1PartCand.h"` present | ✓ Carried in from v5f (line 102) |

### C. Rad-x event-scope computation
| Check | Result |
|---|---|
| `H1FloatPtr hatGenEnPhoton("GenEnPhoton")` declared at module scope | ✓ Line 232 |
| `s_eff = 4 * GenBeamProtP * (GenBeamElecE - max(GenEnPhoton, 0))` | ✓ Line 1027 (uses per-event `_evt` vars) |
| Per-event beams, not nominal 27.6 / 920 constants | ✓ Lines 1018-1019 (mirrors `BeamElecE` / `BeamProtP`) |
| `GenEnPhoton < 0` guarded to 0 | ✓ Line 1024 |
| `E_e_eff > 0` and `s_eff > 1` and `Y_es_gen > 1e-6` guarded with sentinel | ✓ Lines 1037-1043 |
| NoRad case (GenEnPhoton=0) reduces to standard formula | ✓ By construction: `E_e_eff = GenBeamElecE` |

### D. ystar_es_gen
| Check | Result |
|---|---|
| Boost uses per-event gen beams + gen γ\*, not nominal | ✓ Lines 1055-1062 (uses `eBeam4_gen` from per-event `GenBeamElecE_evt/ProtP_evt` + `genScatElec4_evt`) |
| `validBoostGen_es` guard before applying boost to gen hadron | ✓ Line 1156 |
| Sentinel `-999.0f` when invalid | ✓ Line 1161 |

### E. Branch declarations and registrations
| Check | Result |
|---|---|
| Exactly 5 new `chargedTree->Branch(...)` | ✓ Lines 560, 561, 562, 563, 565 |
| Exactly 5 new `chargedGenTree->Branch(...)` | ✓ Lines 608, 609, 610, 611, 613 |
| Branch names match spec exactly | ✓ Q2_es_gen, Y_es_gen, x_es_gen, GenEnPhoton, ystar_es_gen |
| Float_t reset to sentinel near existing declarations | ✓ Lines 322-331 (_ch), 355-361 (_cg) |
| Sentinel = -999.0f (-/Q²/Y/x/ystar) or 0.0f (GenEnPhoton) | ✓ Matches spec |

### F. FIX V5G-N inline tags
| Tag | Count | Spec class | Status |
|---|---|---|---|
| V5G-1 | 2 | skip-scat-elec | ✓ |
| V5G-2 | 11 | rad-safe gen DIS | ✓ |
| V5G-3 | 10 | rad-safe γ\*-p boost | ✓ |
All tag numbers in `{1, 2, 3}` per spec Appendix A.

### G. C++ syntax sanity
| Check | Result |
|---|---|
| Brace balance preserved | ✓ delta +8 / +8 (new scope blocks) |
| Paren balance preserved | ✓ delta 0 |
| New `Float_t` typing consistent with existing pattern | ✓ |
| No C++11/14/auto/lambda/using namespace added | ✓ |
| Includes well-formed | ✓ (no new include needed; `H1PartCand.h` already in v5f) |

### H. Backward compatibility
| Check | Result |
|---|---|
| Events with no `IsScatElec()`-tagged track produce identical output to v5f | ✓ By skip-only semantics; if no track is skipped, every v5f branch is unchanged |
| Legacy `xGen_ch = Q2sGenVal / (4.0 * pBeamP * eBeamE * YsGenVal)` formula intact | ✓ At v5g line 1089 (was v5f line 938; shifted but unmodified) |
| Legacy `xGen / Q2sGen / YsGen / ystarGen` branches still registered and filled | ✓ All registrations preserved; all assignments preserved |
| New branches sit alongside, not replacing, legacy | ✓ |

### I. The controlled deviation (`genScatElec4_evt` mirror)
| Check | Result |
|---|---|
| `genScatElec4_evt` default-initialized at event scope | ✓ Line 919, `(0.0, 0.0, 0.0, 0.0)` |
| Mirrored from `genScatElec4` inside `if (validGenElec)` | ✓ Line 1011 |
| Consumed only by new v5g code | ✓ Line 1058 (`gamma_gen_es = eBeam4_gen - genScatElec4_evt`); legacy code still uses `genScatElec4` |
| Functionally equivalent to hoisting per spec §6.3 intent | ✓ Sentinel default means downstream consumer sees zero 4-vector when invalid; consumer is guarded by `validBoostGen_es` |

### J. Red flags
None identified. The skip-then-fill flow is clean; the per-event scratch state is initialized at event scope (lines 915-925 area); sentinels are propagated through guards.

## CRITICAL defects
**None.**

## MAJOR concerns
**None.**

## MINOR concerns
**None remaining post-fix.** (The reviewer's "_cg per-iteration reset" concern is a real maintainability nit for a future refactor but is identical to v5f behaviour and is NOT a v5g defect.)

## What v5g got right
- Pure-additive diff (15 of 15 hunks type `a`).
- IsScatElec skip is the minimum-disruption, maximum-correctness solution per the researcher's H1OO finding.
- Per-event beam-energy policy (frozen P9) respected in `s_eff` and the γ\*-p boost.
- Defensive sentinels and guards (Float_t -999 for kinematics, 0.0 for GenEnPhoton, validBoostGen_es gate on ystar).
- Per-event scratch state (`_evt` suffix) cleanly initialized per event, never leaks across events.
- Legacy code byte-identical: the controlled deviation introducing `genScatElec4_evt` is the right choice to preserve v5f outputs exactly.
- Documentation: every change cluster carries a `// FIX V5G-N:` comment; the comments explain the physics, not just the mechanics.
- Inline cross-references to spec sections (§6.2, §6.4, §7.2) so the C++ traces back to the design.

## Sign-off conditions
**Approved as-is for NAF compile.** Proceed to:
1. scp `_v5g.C`, `_v5g.h`, `_v5g.h` files to NAF `src/` directory.
2. Update NAF Makefile to add `_v5g` build target (per spec §10.2).
3. `make bin` and confirm clean compile.
4. Submit Condor smoke campaign on 5-10 reference MC files (mix of RAPGAP rad + DJANGOH rad).
5. Run validation tests from spec §8 against the smoke output:
   - Legacy branches byte-identical (regression check vs v5f).
   - IsScatElec bit fires for ~1 track per event at Q² > 150 (positive test).
   - x_es_gen finite on rad events where legacy xGen is inf.
   - x_es_gen ≈ legacy xGen on NoRad events (within float precision).
6. If smoke passes, schedule full-stat rebuild on the Condor batch system.

**Do NOT launch the full-stat rebuild on the login node.** Use Condor.

## What was NOT reviewed
- NAF compile (requires H1OO build environment).
- Runtime behavior (requires test ROOT files; we can validate after the smoke build).
- Performance impact (the IsScatElec check is O(1) per track; the rad-x branches add ~5 Float_t writes per row; negligible).
