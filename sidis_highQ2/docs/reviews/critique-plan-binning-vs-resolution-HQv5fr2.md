# Critique: plan-binning-vs-resolution-HQv5fr2

**Critic:** planning-critic
**Date:** 2026-05-22
**Plan reviewed:** `docs/plans/plan-binning-vs-resolution-HQv5fr2.md` (972 lines, 19 sections)

## Verdict
**MAJOR REVISIONS REQUIRED**

## Summary
The plan is strong on overall scope discipline, output completeness, and explicit handling of the high-Q² reco-tree gen-electron caveat on `ChargedTot`. However, it has several correctness and feasibility gaps against the current high-Q² codebase and authoritative context: incorrect input file globs, deprecated artifact treated as active anchor, under-specified selection semantics (`ParticleSource`, charge-agreement, weighting), and incomplete encoding of the z-edge supervisor caveats. Close to implementable in structure, not yet safe to execute without another annotation cycle.

## CRITICAL defects (block implementation)

1. **Wrong MC input paths.** Plan §§2.1, 5.2, 6.1 reference `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/charged_HighQ2_{rapgap31,djangoh14}_*.root`. The authoritative high-Q² inventory is `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/{rapgap31,djangoh14}/` (legacy per-run outputs use `proton_highQ2_*.root` naming) plus merged outputs that were written to `/pnfs/desy.de/dphep/scratch/h1/klesthen/sidis/highQ2_merged/` (dCache not reliably accessible as of 2026-05-22; verify readability before use). The existing `migration_matrices_v5f_r2_default.json` points into the legacy path. The plan's globs would match zero files.

2. **Deprecated artifact treated as active anchor.** Plan §§2.1, 5.2, 6.5, 19 use `docs/data/unfolding_compare_v5f_HQv5fr2_analyzed.json` as a comparison reference. The file declares itself `status: DEPRECATED`, `scheme: HQ-v5f-r1`, `retained_for: audit trail; do not consume for downstream analysis`. Plan must either drop this anchor or label it explicitly as archival-only with a note that comparisons against it carry no authority.

3. **Beam-energy scanner invocation wrong.** Plan §13.2 invokes `verify_beam_energy_usage.py scripts/study_binning_resolution_HQv5fr2.py`. The scanner takes no positional argument; it walks `scripts/*.py`. Correct invocation is `python3 scripts/verify_beam_energy_usage.py` and then verify the new script is absent from FAIL output.

## MAJOR concerns

4. **Weighting prescription not frozen.** Plan §§2.2, 7.1.1, 11 say "same nominal matched-MC weighting prescription as the current high-Q² pipeline" without spelling it out. High-Q² pipeline uses `weight1 * weight2 * TrigWeightAC * NucliaWeight` in some scripts and `weight1 * weight2` in others. Reproducing the existing 4D purity median 0.626 requires choosing one; the plan must freeze it explicitly. Recommend `weight1 * weight2` for the resolution study (mirrors low-Q² precedent and `migration_matrices_v5f_r2_default.json`) and document the choice.

5. **Charge handling underspecified.** Authoritative context says "480 4D cells per charge". Plan §7.5.1 says charge is metadata not part of the 480-cell count. Existing per-charge unfolding artifacts are charge-separated. Plan must explicitly choose: charge-separated 480-cell audit (matches existing artifacts) OR charge-combined 480-cell + per-charge supplementary, and justify.

6. **ParticleSource semantics unresolved.** Plan freezes `ParticleSource != 4` without elevating the ambiguity. Authoritative warning: `ParticleSource` historically defaults to 0. The plan must require (a) a `ParticleSource` value-frequency audit on the matched-MC sample as a pre-condition, (b) explicit documentation of whether 0 means "acceptable default" or "background-like contamination", (c) reconciliation to the migration JSON under the exact same policy. This is needed before the residuals stage runs.

7. **z-edge caveats not operationalised.** Supervisor memo flags `z ∈ [0.0, 0.1]` (soft/target fragmentation) and `z ∈ [0.7, 1.0]` (exclusive VMD + diffractive contamination) as edge bins with different physics treatments. Plan mentions edge bins conceptually but JSON output contract in §11 has no dedicated `headline_z_bins` vs `edge_z_bins` split, no edge-bin worst-purity summary, no edge-bin disagreement summary. Required additions to JSON:
   - `summary.headline_z_purity_4d_median` (z ∈ [0.1, 0.7], 4 bins)
   - `summary.edge_z_purity_4d_median` (z ∈ [0,0.1] and [0.7,1.0], 2 bins)
   - `per_z_bin.flag` ∈ {"headline", "edge_low", "edge_high"}
   - `flagged_cells.in_headline` and `.in_edge`

8. **Reconciliation of 0.626 anchor not tied to existing sample policy.** Plan says "reproduce or explain 0.626" but does not require matching the existing JSON's sample policy: `n_files_per_gen: 50`, weighting, population threshold, per-charge handling. A full-stat rerun may legitimately differ. Plan needs an explicit reconciliation matrix that runs the new script under the same 50-file subset first as a strict reproduction check, THEN runs full-stat and reports both.

9. **Sparse-bin statistical uncertainty missing from 4D output.** The risk register acknowledged sparse cells inflating medians but the JSON schema in §11 stores only `purity_value` and `n_eff` per cell, not `purity_stat_unc` (which is binomial-on-effective-N). Without per-cell stat uncertainty, the audit cannot distinguish "purity = 0.3 with 1000 events" from "purity = 0.3 with 5 events" — both have very different implications for unfold-vs-bbb gating.

10. **Gen-denominator event cuts loose language.** §7.1.3 says "gen event-level DIS cuts mapped to the frozen high-Q² definitions". Plan must state explicitly: gen-side denominator applies only cuts derivable from gen branches; no `Empz`/`PVz` cut on ChargedTotGen because builder does not write them there; event-cut parity with reco is partial by construction; the missing Empz/PVz cut is a documented limitation, not a defect.

## MINOR concerns

11. **SSH procedure under-specified.** §13.1 correctly mentions `ssh -O check` but §13.2 then issues multiple follow-on `ssh naf-hone.desy.de "..."` commands. Per workspace rules: one check per dispatch, any later failure means stop, no retries. Plan should add explicit "if any follow-on command fails, stop and report; do not retry" guidance.

12. **Alternative-binning variants not edge-deterministic.** §7.7 defines "narrower-by-half" and "double-width" abstractly but does not give the exact edge-list construction. Mapping from `[0, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` to "half-width" is ambiguous (do we split each interval evenly, or apply a uniform 0.05 step?). Plan must provide the deterministic edge lists.

13. **Unfolding-required flag wording drifts toward method recommendation.** §§7.5.2, 9, 10, 17 introduce `unfolding_required` flags. Acceptable if purely informational. Plan should add one sentence: "these flags are informational inputs to the existing CG-2.3 gating in `plan-finalization-r1.md` and do not constitute fresh method decisions made by this audit."

14. **Aux √s plot placement.** Open question #3 from the planner. Recommend keep as `fig_aux01_per_event_sqrts.png` and reference it from the policy doc; do not promote to `fig_r0x` numbering (would diverge from low-Q² naming).

## What the plan got right

- Correctly identifies the high-Q² ChargedTot schema caveat: no `GenElecE/GenElecTheta` polar; use inverse electron method.
- Treats φ_h as periodic with wrapped residuals via `atan2(sinΔφ, cosΔφ)` (§§7.2.1, 9).
- 2×RMS criterion in the right space: log for Q²/x, linear for z/pT_h, no forced pass/fail for φ_h.
- Explicit audit-not-rebin scope guard, repeated in §§1, 14, 17, 19.
- Output contract: JSON path, md path, plot dir, ~30 JSON keys catalogued.
- Risk register present.
- Critic gates CG-1..CG-6 reasonable.
- Beam-energy policy correctly cites helper and scanner.

## Open questions for the supervisor

These genuinely require supervisor decision (not implementer):

1. **Weight formula** (defect 4): `weight1 * weight2` (mirrors low-Q² precedent) vs `weight1 * weight2 * TrigWeightAC * NucliaWeight` (mirrors high-Q² production scripts)?
2. **Charge handling** (defect 5): charge-separated 480-cell audit, or charge-combined with per-charge supplementary?
3. **ParticleSource selection** (defect 6): is `ParticleSource != 4` the right cut, or also exclude 0? Requires the `ParticleSource` value-frequency audit on the actual high-Q² matched-MC sample before deciding.

The other planner-flagged open questions are implementer decisions and can be resolved in the plan revision without supervisor input.

## Final recommendation

**REVISE then implement.** The plan structure is sound and the methodology is correct. Fix the 3 CRITICALs (paths, deprecated anchor, scanner command) and the 7 MAJORs (weighting, charge, ParticleSource, z-edge outputs, anchor reconciliation, stat uncertainty, gen-cut language) in one annotation cycle. The 4 MINORs can be folded into the same revision or deferred. After revision the plan should be implementation-ready in one shot.

Estimated revision effort: 2-3 hours by the planner agent. Three supervisor decisions block full revision (weight formula, charge handling, ParticleSource policy).
