# DJANGOH14 NONRAD Supplement Pipeline — 2026-07-08 Validated Chain

Supersedes and extends `djangoh14_norad_supplement_pipeline_2026-07-07.md` for the current validated Condor chain.

## Purpose

Produce supplemental DJANGOH14 NONRAD MC for the high-Q2 charged-SIDIS `C_rad` study without re-discovering the environment/bootstrap failures from the 2026-07-08 validation session.

Use the remote short root:
`/data/dust/group/h1/klesthen/Djangoh14`

Do not write local or remote ad hoc wrapper-copy variants as canonical provenance. Use the maintained remote scripts below.

## Canonical Remote Scripts

All three validated stages are driven from:
`/data/dust/group/h1/klesthen/Djangoh14/scripts/`

| Stage | Canonical script |
|---|---|
| 1: DJANGOH generation | `submit_stage1_djangoh_500x10k.sh` |
| 2: dst2all conversion | `submit_stage2_dst2all_from_stage1.sh` |
| 3: C_rad treebuilder | `submit_stage3_crad_treebuilder_from_stage2.sh` |

## SSH / NAF Safety Reminders

Before any remote command:

```bash
ssh -O check naf-hone.desy.de 2>&1
```

Rules:
- If the ControlMaster check fails, stop and ask the user to re-establish SSH manually.
- Use Condor for production, multi-file work, and anything beyond a one-file sub-5-minute check.
- Do not run production on the login node.
- Keep monitoring and any `condor_q` / `condor_rm` actions scoped to the specific campaign path; do not use broad account-wide Condor operations on the shared `klesthen` account.
- Treat `/data/dust/...` and `/afs/...` as remote-only paths.

## Validated One-Job Campaign from 2026-07-08

| Item | Validated value |
|---|---|
| Campaign dir | `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_envpatch_submit1_20260708` |
| Stage 1 cluster | `4242693` |
| Stage 2 cluster | `4242698` |
| Stage 3 cluster | `4242700` |
| Stage 1 status | PASS: BOS/LOG produced |
| Stage 2 status | PASS: `ODS=1000`, `MODS=1000`, `HAT=1000` |
| Stage 3 status | PASS: `crad_genonly_0000.root`, size `840K`, `GenDIS=1000`, `ChargedGenRC=22027` |

### Required validated branches in Stage 3 output

`weight1`, `weight2`, `Q2sGen`, `YsGen`, `XsGen`, `GenPDG`, `GenCharge`, `Mother2`, `IsGtr`, `GenEta_lab`, `zGen`, `pT_h`, `xFGen`

## Current Production State at Time of Writing

Stage 1 production launch is in progress; do **not** claim completion unless later monitoring proves it.

Current production launch:

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage1_djangoh_500x10k.sh --campaign hk_0123_500x10k_prod_20260708 --njobs 500 --nevents 10000 --submit
```

If a suffix was needed at submission time, use the actual created campaign directory name. This note does **not** certify production completion.

## Exact Commands

### Stage 1: DJANGOH generation

Remote script:
`/data/dust/group/h1/klesthen/Djangoh14/scripts/submit_stage1_djangoh_500x10k.sh`

Important behavior:
- Default mode is **prepare-only**.
- Use `--submit` to actually submit to Condor.

One-job smoke:

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage1_djangoh_500x10k.sh --campaign <CAMPAIGN> --njobs 1 --nevents 1000 --submit
```

Production:

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage1_djangoh_500x10k.sh --campaign <CAMPAIGN> --njobs 500 --nevents 10000 --submit
```

### Stage 2: dst2all conversion

Remote script:
`/data/dust/group/h1/klesthen/Djangoh14/scripts/submit_stage2_dst2all_from_stage1.sh`

Prepare campaign conversion jobs:

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage2_dst2all_from_stage1.sh --campaign <CAMPAIGN>
```

For one-job validation, submit the generated file directly:

```bash
condor_submit <campaign_dir>/stage2_dst2all/jobs/job_0000/condor/submit_stage2_0000.sub
```

### Stage 3: treebuilder

Remote script:
`/data/dust/group/h1/klesthen/Djangoh14/scripts/submit_stage3_crad_treebuilder_from_stage2.sh`

Prepare campaign treebuilder jobs:

```bash
cd /data/dust/group/h1/klesthen/Djangoh14
scripts/submit_stage3_crad_treebuilder_from_stage2.sh --campaign <CAMPAIGN>
```

For one-job validation, submit the generated file directly:

```bash
condor_submit <campaign_dir>/stage3_treebuilder/jobs/job_0000/condor/submit_stage3_0000.sub
```

## Expected Directory Layout

Assume:
`<campaign_dir> = /data/dust/group/h1/klesthen/Djangoh14/campaigns/<CAMPAIGN>`

### Stage 1 outputs

Per-job outputs live under:
- `<campaign_dir>/stage1_djangoh/jobs/job_####/bos/`
- `<campaign_dir>/stage1_djangoh/jobs/job_####/log/`

Important:
- Monitor the per-job `bos/` and `log/` directories.
- A top-level scan can falsely suggest zero output even when jobs succeeded.

### Stage 2 outputs

Per-job conversion outputs live under:
- `<campaign_dir>/stage2_dst2all/jobs/job_####/conversion/ods.root`
- `<campaign_dir>/stage2_dst2all/jobs/job_####/conversion/mods.root`
- `<campaign_dir>/stage2_dst2all/jobs/job_####/conversion/hat.root`

### Stage 3 outputs

Per-job final ROOT outputs live under:
- `<campaign_dir>/stage3_treebuilder/jobs/job_####/root/crad_genonly_####.root`

## Validation Checklist

### Stage 1 validation

PASS if:
- Condor cluster is created.
- Per-job BOS file exists under `stage1_djangoh/jobs/job_####/bos/`.
- Per-job LOG file exists under `stage1_djangoh/jobs/job_####/log/`.
- Wrapper env patch is active and jobs produce BOS/LOG.

Non-blocking note:
- `module: command not found` may appear in wrappers and is nonfatal if BOS and LOG are produced.

### Stage 2 validation

PASS if for the tested job:
- `ods.root`, `mods.root`, and `hat.root` all exist.
- ROOT entry counts are consistent.
- Validated one-job reference is `1000` entries in each of ODS, MODS, and HAT.

### Stage 3 validation

PASS if for the tested job:
- `crad_genonly_####.root` exists.
- Validated one-job reference file is `crad_genonly_0000.root` with size about `840K`.
- `GenDIS=1000`.
- `ChargedGenRC=22027`.
- Required branches are present:
  `weight1, weight2, Q2sGen, YsGen, XsGen, GenPDG, GenCharge, Mother2, IsGtr, GenEta_lab, zGen, pT_h, xFGen`

## Critical Fixes Now Baked into the Remote Scripts

### Stage 1

The generated wrappers now source the needed runtime before `djangoh14test`, including:
- LCG_107a `x86_64-el9-gcc14-opt`
- H1 `thish1`
- H1BUILD paths

This is the fix for the previous missing runtime problem.

### Stage 2

The conversion script now includes the critical fixes:
- BOSINPUT patch handles lowercase `file=` as well as the expected form.
- Validation checks the active BOSINPUT only, not commented or irrelevant lines.
- Environment bootstrap is hardened against:
  - `desygroupset: command not found`
  - `OS: unbound variable`
  - wrong H1 env source order

### Stage 3

The treebuilder script now includes the critical fixes:
- robust environment bootstrap before treebuilder execution
- no early unconditional `desygroupset`

## Failure Lessons to Preserve

- **Do not add arbitrary MSTA guards.** The original NONRAD steering is the validated baseline. If checking `MSTA`, do not block submission unless an active uncommented `MSTA(39)=2` truly remains. Normal agents should not add new guards.
- **Stage 1 is prepare-only unless `--submit` is passed.** Missing `--submit` is a common false-success trap.
- **Stage 1 outputs are per-job.** Top-level scans can falsely report zero BOS/LOG even when jobs succeeded.
- **`GLIBCXX_3.4.32 not found` means the wrapper environment is missing the LCG_107a setup.** Current Stage 1 script should already handle this.
- **Stage 2 `active BOSINPUT FILE not patched` came from lowercase `file=` in the template.** Current Stage 2 script handles this.
- **Stage 2 `desygroupset: command not found` and `OS: unbound variable` were environment-bootstrap failures.** Current Stage 2 script handles both before H1 setup.
- **Stage 3 had the same early `desygroupset` issue.** Current Stage 3 script handles it.
- **Do not globally retag copied wrappers or steering.** Use the maintained scripts.
- **Do not use failed wrapper-copy resmoke attempts as provenance.** The validated provenance is the script-driven chain above.

## Recommended Minimal Operating Sequence

1. Check SSH ControlMaster.
2. Launch Stage 1 smoke with `--submit`.
3. Validate BOS/LOG in per-job directories.
4. Prepare Stage 2 for the same campaign and submit one generated `submit_stage2_0000.sub`.
5. Validate `ods.root`, `mods.root`, `hat.root` and their entry counts.
6. Prepare Stage 3 for the same campaign and submit one generated `submit_stage3_0000.sub`.
7. Validate `crad_genonly_0000.root`, tree counts, and required branches.
8. Only after the one-job chain passes, launch larger production.

## Relationship to the 2026-07-07 Note

The 2026-07-07 note remains useful as earlier short-path context, but the current validated Condor chain and failure fixes are captured here and should be treated as the operational starting point for future agents.
