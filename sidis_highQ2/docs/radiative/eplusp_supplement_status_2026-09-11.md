# e+ p DJANGOH14 NORAD supplement — submission status (2026-09-11)

- **Smoke:** Condor cluster `5109124`, `job_0000`, PASSED with 9,999 generated events, `sigma = 7.2879 nb`, and a 52,113,600-byte output.
- **Wrapper environment:** jobs `0001-0529` were reconciled to the validated `job_0000` configuration: `desygroupset h1`, H1BUILD/H1LCG `PATH` and `LD_LIBRARY_PATH` precedence, and shell ordering around the LCG and legacy-H1 setup. All 529 wrappers were verified; 528 required patching during reconciliation.
- **Steering:** no active `MSTA(39)` card remains. RNIN values are unique and span `900001-900529` for production (`900000` belongs to the smoke).
- **Submission:** 529 production jobs (`0001-0529`) were submitted in Condor cluster `5109125` using `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/stage1_djangoh/submit_stage1_0001_0529.sub`.
- **Initial scheduler snapshot:** 194 running, 0 held; all 529 Cmd paths matched the intended campaign. No completion polling or output validation was performed.
- **Expected statistics:** 5,289,471 generated events from the 529 production jobs, plus 9,999 smoke events = **5,299,470 total**. At `sigma = 7.2879 nb`, expected effective luminosity is **727,160.09 nb^-1**.
- **Manifest:** `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/manifests/stage1_manifest.tsv` (real cluster IDs and scheduler states recorded).

## Terminal monitor and downstream staging decision (2026-09-12)

- **Terminal condition:** cluster `5109125` drained on the first poll; `condor_q` returned no live records. `condor_history` records `527` exit-0 and `2` exit-2 jobs; no jobs were Held.
- **Stage 1 audit:** `530/530` BOS files and `530/530` official LOG files are non-zero, but only `528/530` jobs (including smoke `job_0000`) carry `stage1.ok` and exit 0. `job_0085` and `job_0404` wrote partial BOS files then aborted with `hsgenm.F:366` Fortran formatted-transfer error (expected INTEGER, got REAL). They are truncated/suspect outputs, not good production jobs.
- **Downstream:** Stage 2 and Stage 3 were deliberately not prepared/submitted: their required Stage-1 inputs are incomplete/corrupt for two jobs. No Stage-3 ROOT ntuple directory or ROOT output exists for this campaign.
- **Actual good statistics:** `528 × 9,999 = 5,279,472` generated events. With `sigma = 7.2879 nb`, `L_eff = 5,279,472 / 7.2879 = 724,416.0869386242950644218499 nb^-1`.
- **Durable audit:** `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/status/stage1_terminal_audit_20260912.txt`; manifest terminal states were updated at the path above. First blocker is the two Stage-1 generator aborts; route repair/validation before any downstream staging.

## Repair, downstream completion, and final validation (2026-09-12)

- **Cause:** `job_0085` and `job_0404` used the same validated steering/runtime as 528 completed peers and diverged only by RNIN; both hit the seed-dependent `hsgenm.F:366` formatted-transfer type mismatch after thousands of events, not a systematic configuration failure.
- **Retry:** fresh non-overlapping RNIN seeds `900530` and `900531` were submitted in cluster `5109129` via `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/stage1_djangoh/submit_stage1_retry_0085_0404_20260912.sub`; both completed exit 0 within the 3-minute poll interval. Final Stage-1: `530/530` good jobs, `0` held.
- **Stage 2:** the first nested-submit attempt (`5109130`) exited 127 and produced no outputs; the durable direct replacement `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/stage2_dst2all/submit_stage2_direct_0000_0529_20260912.sub` (cluster `5109131`) completed 526 jobs. Four transient `dst2all` aborts were quarantined and recovered in cluster `5109132`; all `530/530` conversion triplets are present.
- **Stage 3:** cluster `5109133`, submitted with `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/stage3_treebuilder/submit_stage3_direct_0000_0529_20260912.sub`, completed `530/530` exit 0. Ntuples: `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/stage3_treebuilder/jobs/`.
- **Stage-3 validation:** 530 non-zero ROOT files, 4,522,861,241 bytes total, zero zero-size/truncated files; `GenDIS=5,298,940`, `ChargedGenRC=118,109,262`. Thus `N_generated=5,298,940` and `L_eff=727,087.3639868824 nb^-1` using 7.2879 nb. Durable validation/status: `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911/status/stage3_20260912/stage3_validation.json` and `status/final_terminal_manifest_20260912.txt`. No remaining blocker.
