# M1 closure (re-scope): the env100 budget builder

Status: CLOSED 2026-10-24 for the eΣ (esigmaphoton 2026-10-23) release, by user decision (re-scope). Still open, and not done, for the older OPT-A, v5 and nw budgets.

## What the blocker was
The env100 term is the systematic uncertainty from the background that is subtracted and then unfolded away. The script that produced the 2026-10-19 dated-budget table for it cannot be found. The acceptance test was to regenerate the table and match the column `total_with_bgsub19_rel` exactly in three bins: pos/109, pos/179 and neg/240 (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md:425`). Without that match, the quoted budget was marked as blocked.

## User decision (2026-10-24)
Close by re-scope. The eΣ budget computes env100 with `components_groupC.py` and `esph_downstream.py`, and no longer uses the 10-19 builder. The literal M1 fix is not carried out.

## Evidence
- Quoted budget: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphoton_2026-10-23/esph_budget_v2.csv`, sha256 cbac4b573aa3a5da8127e6640c52cf8d40fe1bb393c3dc39ee3d307e490108b9 (INSTALL_RECORD `budget_v2_sha256`), column `total_quoted_esph_rel`.
- Built by `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/esph_downstream.py`, sha256 0e41b4ba96e01e26fba72bc07f0f2ecb64a4a7d28f0ad41997f84f935f3982c5. This was already pinned (INSTALL_RECORD, about L141); the live copy is identical.
- `step_groupC` (`esph_downstream.py` L281-298) imports `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/final_rerelease_2026-10-22/budget_groupC/components_groupC.py` and runs it on the MC cache. Outputs are in `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/downstream/budget_groupC_v2/`.
- New pin: `components_groupC.py` sha256 d3297b7ef4b04a3f6841ce40c3cd9d84f6d2309f00d01582cd6e519cb44191a3 (32965 bytes, mtime 2026-10-22 16:45:02). Also `build_mc_cache_groupC.py` sha256 d7e759dfa61fe932123a8058eb693ca276249f9c98fd6b819f7f38abf81bc3d0 (mtime 16:42:29).
- The run recorded the same module hash in `budget_groupC_v2/groupC_new.json` (inputs block, about L524-532), so the pin is the module that actually ran.
- Outputs: `groupC_new_columns.npz` sha 5a442059f52458cc9f5d3a657a880232873310582d38e73174b3d780afa7d437 and `groupC_new.json` sha f0bf47032b51ac5a17c086f2e4cf2c110570f520c0a9e9227f4302fe0a459746, both written 2026-10-23 05:07:11. The budget CSV followed 0.3 s later, and the module is older than the run.
- Method: `components_groupC.py` L19-25 computes env100 as a full joint re-unfold (user decision 2026-10-22 #4). A grep of `budget_groupC/*.py` for `term_490` and `bgsub19` gives 0 hits, so the unlocatable table is not an input. L364 reads the installed 10-19 reco-ratio vector only for the gate and "mixed" comparison columns, not for the quoted `bgsub_env100_hyp_rel` or `bgsub_env100_rel`.
- The pin is recorded in a new sidecar, `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphoton_2026-10-23.M1_groupC_pin_2026-10-24.json`. The installed INSTALL_RECORD (sha 1ab34a98bb07dfc62eadcf7fa85a761296b8c459c5397bfc4cd46239f01da06a) is not edited. Registry lines go into both RUNLIST.md copies and the H1 SEVEN_CATEGORY_SCRIPT_INDEX.md.

## What this does not change
- The literal M1 acceptance test (exact match in the three bins) is not done. It remains open for the older budgets `systematics_bgsub_env100_optA_c1fix_2026-10-20`, `..._nonprimary_v5...` and `..._nonprimary_nw_optA_2026-10-21`, and applies only if one of them is quoted again.
- `components_groupC.py` is not in the package MANIFEST. Its provenance is the run-recorded hash plus this pin.
- The MC cache contents were not traced file by file.
- An optional bitwise re-run of groupC is one Condor job. It was not done.

## Pointers
- Audit: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/blockers/quoting_blockers_audit_2026-10-24.md` (follow-up, M1 section).
- Memory: `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/memory/decisions/2026-10-24-m1-n2-closed-d2-eprime-cut-shifted-legs.md`.
- Script that applies the registry and status edits: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_release_2026-10-23/s18_close_quoting_blockers_2026-10-24.py`.
