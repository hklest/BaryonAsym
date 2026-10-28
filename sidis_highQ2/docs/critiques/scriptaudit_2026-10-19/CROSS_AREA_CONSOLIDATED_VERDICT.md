# Script Audit 2026-10-19 — Cross-Area Consolidated Verdict (12/12 loops complete)

Scope: canonical high-Q2 charged-hadron SIDIS scripts, four areas, three critique loops each, read-only.
Basis audited: cone+S2'+Set-R' clean release (2026-10-17 FINAL) + 2026-10-19 Env100 freeze.

## Area verdicts

| Area | Loops | Verdict | Blocking defects | Total confirmed findings |
|---|---|---|---|---|
| Treebuilder (+central chain) | 3/3 | CONDITIONAL-PASS | N1, M2, N2 (+B1 prevention) | 20 open (2 TRUTH-DEFINITION, 5 BUILDER-PROVENANCE, 2 MATCHING, 2 DENOMINATOR-TRAP, 9 ROBUSTNESS, 1 SYS-UNDERRESPONSE; N4 resolved) |
| Unfolding procedure | 3/3 | PASS | none | 8 minor (1 UNFOLD-NUMBER, 2 PROVENANCE, 1 MAINTENANCE, 4 DIAGNOSTIC-ONLY) |
| Systematics evaluation | 3/3 | CONDITIONAL-PASS (quoting BLOCKED) | C1, M1, m1, m3, n1 | 12 (1 QUADRATURE-DEFECT, 3 PROVENANCE, 3 SUPERSESSION-HYGIENE, 1 GATE-DESIGN, 4 DOCUMENTATION) |
| Plotting | 3/3 | FAIL | 24 defects (3 PLOT-NUMBER, 13 PROVENANCE, 7 DISPLAY, 1 robustness) | 24 |

## Top physics-content defects (ranked by impact on final results)

1. **TB-N1 (TRUTH-DEFINITION, material, per-cell bias)** — response bins gen prior/miss by `Q2esGen`, matched body by rad-safe `Q2_es_gen`; 1.94% straddle (per-cell up to 8.74%), 100% one-directional; sign-definite per-(Q2,x)-cell cross-section bias NOT absorbed by IBU. Integrated values robust. Fix: harmonize gen-Q2 index, rebuild response, re-release.
2. **TB-M2 (TRUTH-DEFINITION, material, per-z-bin bias)** — gen z on nominal beams (ISR-biased); 3.25% of gen rows change z bin, all upward; rad-safe z computable offline (formula validated). Fix: rebuild gen-z axis on `z_rad` or documented truth-contract + z-shift systematic.
3. **SYS-C1 (QUADRATURE-DEFECT, conservative)** — `total_with_env100_rel` double-counts detint_removal (proven exactly, 3 independent re-derivations); propagates verbatim into public Born tables and env100 covariance diagonals; inflation <= ~171 ppm, never deflates. Fix: regenerate on declared 19-active basis; acceptance: empty-D bins (pos/109, pos/179, neg/240) total == `total_with_bgsub19_rel` EXACTLY.
4. **SYS-M1/n1 (PROVENANCE)** — env100 builder unlocatable (hunt final); registries have zero 2026-10-18/19 entries; quoted budget not regenerable from pinned provenance. Fix: reconstruct thin appender (19-comp CSV + env100 term CSV, detint once), pin sha + run record, register override.
5. **TB-N2 (SYSTEMATIC-UNDERRESPONSE)** — HFS legs leave Empz bit-identical; HFS->Empz>45 channel absent from budget; quoted HFS-leg errors under-cover. Fix: add HFS-Empz channel, re-freeze budget.
6. **TB-M1 (BUILDER-PROVENANCE)** — Stage-2 stores production-HAT-frozen Q2s/Ys (0/65 identical vs calibrated sidecar; Q2>150 flips 7/65); released fiducial on calculator Q2es/Yes (exposure zero there) but z/pT_h/alpha axes and Empz/PVz cuts on frozen state; method description must be corrected; numbers stand.
7. **PLOT-C2 (PLOT-NUMBER, fatal to S3_x0 densities)** — bgsub-diagnostic plotter uncapped dx (1.479 vs 0.979) -> all S3_x0 densities x0.662 low.
8. **PLOT-C1/R1/R2 (PROVENANCE)** — registered Born plotter reads superseded 2026-10-13 release + superseded budget; local-mirror registry staler; clean release ships frozen plotter source embedding ruled-out convention.

## Minimal fix set (cross-area, priority order)

1. Rebuild response with harmonized gen-Q2 cell index (rad-safe) + rad-safe gen z; re-run IBU k=4; re-release; quantify per-cell/per-z shifts (fixes TB-N1+M2 in one pass; full-sample Condor).
2. Reconstruct env100 builder, regenerate total on declared basis, propagate to covariance diagonals + public tables + STATE, re-freeze; empty-D acceptance test (SYS-C1+M1+n1).
3. Add HFS-Empz systematic channel; re-freeze budget (TB-N2).
4. Re-point registered Born plotter to current clean release + current quoted budget; fix uncapped dx; regenerate 7-panel plot set (PLOT-C1/C2/R1/R2 + user decision env100 vs bgsub19 error bands).
5. Supersession hygiene: S2 edition note, checkpoint-5 snapshot, plotting registry entries (SYS-m1/m3; PLOT-R3).
6. Quarantine `submit_full_production_v6.py` wrong-input glob (TB-B1).
7. Hardening batch: output finiteness guards (UNF-1), sidecar-exhaustion hard-fail (TB-M5), covlegs docstring (UNF-5), stale-prose sweep (UNF-2, TB-N5, m2/m4), doc notes on sub-unity C_rad bins (UNF-4), freeze-gate declared-basis re-derivation (SYS-i1), README/config nits (SYS-n2/n3, phi_h).

## Statements of protection

- Released clean-release integrated cross-sections: robust under every confirmed finding (N1/M2 are redistribution-level; C1 is uncertainty-side and conservative).
- Quoted 1000-toy statistical sidecar: untouched by every confirmed finding.
- Per-cell and per-z-bin central values: PROVISIONAL until fix 1 lands.
- Quoted systematic / public Born tables: BLOCKED until fix 2 lands.
- Publication-facing plots: BLOCKED until fix 4 lands (plotting area FAIL).

Reports: loop{1,2,3}_{treebuilder,unfolding,systematics,plotting}.md + loop1_M1_kinematic_provenance_check.md + n1_m2_radgap_q2gen_straddle_rapgap31_7071_em06_0060.txt (all in this directory).
