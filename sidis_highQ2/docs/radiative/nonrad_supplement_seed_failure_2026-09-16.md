# DJANGOH14 NORAD e-p supplement: identical-seed generation failure (2026-09-16)

## Purpose

This study compared the OLD (Britzger-era) DJANGOH14 NORAD baseline MC with NEW supplement MC generated in July and September 2026. It formed, separately for h+ and h-, the per-bin ratio `R = sigma_nonrad(OLD, baseline only) / sigma_nonrad(NEW, baseline + supplement)`, using `sigma_proxy = sum(weight1*weight2)/L_MC` in the frozen zlow005 flattening: 245 bins per charge (`7 Q2/x cells x 7 z bins x 5 pT_h bins`). The intent was to establish statistical consistency of the new generation with the old sample.

## Verdict

**FAIL. The e-p (EMINUSP) supplement campaign is invalid.** All 500 Stage-3 jobs in `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_500x10k_prod_20260708/` generated the identical event stream. The campaign therefore contains approximately 9,998 independent generated events, not 4,999,000. The e+p (EPLUSP) supplement campaign at `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/` is unaffected and passes the same checks.

## Root cause and direct confirmation

Every one of the 500 per-job Stage-1 steering cards hardcodes `RNIN 845991`; for example, this is line 122 of `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_500x10k_prod_20260708/stage1_djangoh/jobs/job_0000/steering/stdjan_0000.steer`, and is equivalent across `stage1_djangoh/jobs/job_*/steering/stdjan_*.steer`. A full scan found exactly 1 unique RNIN value. Stage-1 logs report `H1GEN: ISEED = 845991 from RNIN bank` with seed vector `845991,0,0`. No active `MSTA(39)` card exists, and `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_500x10k_prod_20260708/stage1_djangoh/jobs/job_0000/condor/run_stage1_0000.sh` supplies no alternate seed. The card-generation script is not retained in the campaign tree.

Three separated e-p Stage-3 files (job_0000, job_0250, job_0499) each have `GenDIS = 9,998`, bit-identical first-five-row kinematics, `sum(Q2sGen) = 4391217.458518982`, `sum(weight1) = 23567.29173195362`, and byte size 8,527,772. By contrast, e+p has 530 unique RNIN values spanning 900000-900529 plus retry seeds 900530 and 900531; `scripts/patch_verify_submit_0001_0529.py` enforces uniqueness. Its controls (job_0000, job_0265, job_0529) have distinct kinematics, `sum(Q2sGen)` values 4173191.8107681274 / 4009079.402908325 / 4014814.3607559204, and byte sizes 8,544,871 / 8,515,270 / 8,534,061.

## Aggregate evidence and ratio consequence

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/nonrad_base_vs_supp_audit_2026-09-12.json` records e- supplement `event_n = 4,999,000 = 500 x 9,998`, `had_n = 3,413,000 = 500 x 6,826`, 9,998 unique `(RunNumber, EventNumber)` pairs across all 500 files, and every histogram bin as an exact multiple of 500. e+p instead has `event_n = 5,298,940` and `had_n = 3,445,782`, with no duplication pattern.

The e- component of `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/nonrad_ratio_v1/` (`nonrad_ratio_v1_pos_245bins.csv`, `nonrad_ratio_v1_neg_245bins.csv`, `nonrad_ratio_v1_summary.json`) and `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/radiative/nonrad_ratio_v1_7col_2026-09-12/` is **INVALID**. These artifacts used `L_eff(supp e-) = 1,608,930.470 nb^-1` from 4,999,000 nominal events; true independent exposure is smaller by about 500. Thus `R(h+) = 1.0000639710634291`, `R(h-) = 1.0002384352974953`, and the pulls (`n = 475`, RMS = 586703.17, 374 of 475 beyond 3 sigma) are duplicate-statistics artifacts. The h- disjoint discrepancy `-0.2169% +- 0.0235%` and z > 0.7 e- deficit (supp/base = 0.3397 and 0.0 in the top two z bins) reflect the tiny true e- sample, not generator physics.

## Missed validation and required actions

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/norad_supplement_reconciliation_2026-09-11.md` and `manifests/stage3_validation_status.json` checked counts, OK markers, nonzero sizes, ROOT magic, and common cross section, but no seed field or uniqueness. Identical 8,527,772-byte outputs passed as size consistency rather than a duplicate-stream alarm.

1. Regenerate e-p as a separately versioned campaign with unique per-job RNIN, a `job_index -> steering path -> RNIN` manifest, active-`MSTA(39)` rejection, and pre-submit uniqueness verification following e+p.
2. Require a seed-independence smoke gate: verify log seed echo and nonidentical event-content fingerprints for at least three jobs before production.
3. Quarantine or clearly mark this e-p campaign and all derived ratio tables and plots as invalid provenance.
4. Recompute OLD-versus-NEW NONRAD only after e-p regeneration. e+p needs no regeneration.

## Standing blockers

This record changes no radiative correction. `P-CRAD-SPARSE` and `P-CRAD-LUMI` remain live; the central release remains raw with no `C_rad` applied and is not Born-level.
