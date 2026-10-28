# NONRAD phi_h compact plots (gen-level, DJANGOH14 vs RAPGAP31) — 2026-10-02

> **STATUS 2026-10-03: WITHDRAWN.** Both the 2026-10-01 and 2026-10-02 `nonrad_gen_compact` sets were built on `RC_treebuilder_genonly_v1` trees, whose `Q2_es_gen`/`Y_es_gen`/`x_es_gen` use a wrong-signed swapped-angle formula. Kinematic audit (Condor cluster 5323363, 3140 files): purity ~72%, completeness ~9%, 63% of retained rows in the wrong (Q2,x) cell, 5 of 7 cells 100% mislabeled, true Q2 confined to 914-2750 GeV^2. The phi_h wrap fix is valid and is reused; the kinematic basis of these figures is not.
> Directories moved to `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/quarantine/withdrawn_v1_kinematics/` (see its WITHDRAWN.md). Audit: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/v1_kincut_audit_2026-10-02/`. Corrected replacement basis `RC_treebuilder_genonly_v3`: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/spec_nonrad_rad_phih_v3_2026-10-02.md`.

## Purpose

Generator-level NON-radiative (NORAD) Trento phi_h distributions for DJANGOH14 vs RAPGAP31: all 7 (Q2,x) cells x 7 z x 5 pT_h bins, charge-separated, MC-only diagnostic (`include_data=false`). Not a systematic, not central, not theory-facing.

## Inputs

- ChargedGenRC gen-only ntuples: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/ntuples/{DJANGOH14,RAPGAP31}/nonrad/`, 520 files each.
- Weights: `weight1*weight2`.
- Truth fiducial: `pass_truth_event_fiducial && pass_truth_hadron_fiducial`, `Q2_gen` 150-20000, `0.1<Y_gen<0.7`, `35<=GenEmpz<=70`. No reco cuts.

## Plotter

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/plotting/phi_reco_luminorm_2026-06-12/plot_phi_reco_luminorm_v6_compact.py`

## Defect (found 2026-10-02)

- ChargedGenRC stores phi_h on [0,2pi) (audit: 0.0000% negative entries in both generators); the plotter bins [-pi,pi]. First set therefore showed the entire negative half empty and silently dropped ~49.4% of entries.
- Root cause: missing wrap, NOT a data or generator property.

## Fix

- Single line added at plotter line 118: `phi = np.where(phi > np.pi, phi - 2.0 * np.pi, phi)`.
- Re-run filled-weighted counts exactly doubled:
  - DJANGOH14: plus 11845689.2 -> 23694483.0; minus 11621089.8 -> 23251223.9
  - RAPGAP31: plus 13370567.9 -> 26739728.6; minus 13060538.6 -> 26124488.1

## Status

- SUPERSEDED/INVALID for interpretation (empty negative phi_h half): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/phi_h/nonrad_gen_compact_2026-10-01/`
- Superseded (withdrawn 2026-10-03) set: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/phi_h/nonrad_gen_compact_2026-10-02/` (plus/minus PDFs, 7 pages each; `manifest.json`; `condor/run_full.sh` provenance; `png_plus/`, `png_minus/` review images).
- h1-plot-reviewer ACCEPT on all 14 pages. Non-blocking notes: empty panels only in extreme high-z/low-pT corners; crowded left-margin y labels.

## Lesson

Any plotting or ratio script consuming ChargedGenRC phi_h must wrap (pi,2pi) -> (-pi,0] or bin [0,2pi) explicitly.
