# Hostile implementation critic: FINAL detector-leg + C_rad production (2026-10-21/22)

Scope: the code and campaign setup in `scripts/unfolding/final_detlegs_optA_2026-10-21/` and `scripts/radiative/final_crad_consistent_2026-10-21/`, plus the smoke products. I made no remote writes. My scratch scripts are in `scripts/critic_impl/`: diff_of_diffs.py, merge_dod.py, check_args.py, verify_shim.py, check_crad_queue.py, check_dv.py, check_shard_gen.py, check_leg_branches.py, yh_ratio.py.

**Verdict: DISPATCH-WITH-FIXES.** The physics code is correct as far as I could reproduce it: the shim, the electron OPT-A clone, the merge pins, the args and file sets, and the C_rad fork all check out. Nothing I found forces a second production. Dispatch is still blocked by one procedural item (B1): **the production submit files do not exist yet**. On top of that, the campaign lacks completeness gates that would catch silently dropped files or jobs.

## Findings

| # | Sev | Finding | Evidence | Fix |
|---|---|---|---|---|
| B1 | **BLOCKER** (process) | **No production submit files exist**, so what will actually be dispatched cannot be reviewed. The only `.sub` files are smoke ones (`smoke2.sub`, `smoke2_dvs.sub`, `smoke_elec.sub`, `mergetest.sub`, crad `smoke.sub`, `finalize_gate*.sub`). There is no `.sub` for `shards.args` (1914), `dvs.args` (18), `queue_djangoh14.tsv` (2953) or `queue_rapgap31.tsv` (1777). The production merge job (22 merges plus gen-side checks) and the production finalize job are also unwritten. | `ls condor/` in both dirs | Write them before dispatch, taking the smoke2 subs as template. Use `queue a1,…,a6 from shards.args` and `queue a1,…,a5 from dvs.args`. For C_rad use `queue input,sample,target from queue_*.tsv` (tab-separated is fine; there are 0 commas in any args file). Memory: 4 GB is enough for shards and DVs (measured peak 0.4–1.0 GB in smoke; the OPT-A nominal shards peaked at 1.5 GB). C_rad needs 8 GB, the precedent `submit_8gb.sub`. Add `max_retries` / `on_exit_hold = ExitCode != 0`. The C_rad worker returns exit 2 on BLOCKED. Submit from a clean shell, or drop `getenv=True`: the jobs call `/usr/bin/python3` explicitly, and an inherited LCG `PYTHONPATH` would mix numpy/uproot stacks. |
| M1 | MAJOR | **Silent file loss inside a job.** All builders catch open/read failures, return False and carry on. The response builder only prints `processed k/N files OK` (OPT-A builder L2769–2790). `meta_n_input_files` is `len(files)`, not the number of files OK (L2448). The DV builders only count `n_files_failed`. A transient dust I/O error therefore yields a shard or DV that is short by a file and exits 0. Retries do not help. | builder code; smoke logs show `5/5 OK` and `failed=0`, but nothing enforces it | Add a post-run gate. Every shard `.out` must contain `processed N/N files OK` with N equal to the shard-list length. Every DV JSON must have `n_files_failed==0` and `n_files_opened==660`. Every HFS `*.n2shim.json` must have `n_files` equal to the number of files and unmatched-event fractions ≲1e-3. |
| M2 | MAJOR | **Merges accept any number of shards.** The OPT-A merge and the electron clone both `glob` and merge whatever exists; the clone's L403 only refuses an empty set. A failed job silently produces a response that is short by a shard. | merge code | The merge driver must assert 73 shards for RG and 101 for DJ (`meta_shard_count`). It must also hard-check the gen side against the OPT-A nominal merged responses: `h_prior_gen_*` and `h_mc_count_*` bit-identical for all 22 merged responses. I verified this identity on the smoke shards (elec zero RG, elec p1pct DJ, HFS jes_up RG vs OPT-A nominal shard 0000). `h_prior_gen_*_NWeff` legitimately differs (it is weighted by the NW of the reco partner), so exclude it from the identity check. |
| M3 | MAJOR | **The C_rad finalizers do not check partial counts.** The DJ finalizer globs `rad/{ep,em}`, `nonrad_old/*` and `nonrad_new/*` with no count assertion. The RG finalizer only reports `input_file_counts`. A missing partial biases σ, because the full-sample luminosity stays in the denominator. | `finalize_final_consistent_djangoh14.py` L72; RG L36 | Before finalizing, assert the exact counts. DJ: rad/em 642, rad/ep 731, nonrad_old/em 260, nonrad_old/ep 260, nonrad_new/em 530, nonrad_new/ep 530. RG: rad/em 582, rad/ep 691, nonrad/em 248, nonrad/ep 256. Also grep every worker `.out` for `"verdict": "PASS"`. |
| m1 | MINOR | The production RG finalize command must pass the **emfix** exposures. The fork takes them on the CLI: `--non-em 13470764.7`, `--rad-ep 17536474.0 --rad-em 14461067.0 --non-ep 13954768.0`. Pass `CRAD_F_EP=0.5439017406824048` (from `lumi_eff_final.json`, which I verified: e+p = 0304+0607 periods = 0.54390). | `run_finalize_gate_rg.sh` uses the right values, but no production script exists | Write it with these constants, and record the sha of `lumi_eff_final.json`. |
| m2 | MINOR | The DJ finalize gate prints `DJ gate rows identical: False`. The cause is **not "sum order"** as the plan says. `BEAM["em"] = 1.0 - f` gives 0.48590697279999995, while the applied literal is 0.4859069728 (1 ulp apart). The ≤7.7e-16 agreement is genuine. | local check | Correct the wording in the plan or record. |
| m3 | MINOR | Shim traps that are not reachable today: `arrays(library="ak")` returns **uncorrected** Empz silently; `tree["Empz"]` raises a loud TypeError; `uproot.iterate` / `concatenate` bypass the proxy. None of the three builders uses these paths (each reads ChargedTot once, via `.arrays(list, library="np")`). | verify_shim.py | Make the shim raise if `library` is not `"np"`, so a future builder cannot silently skip the correction. |
| m4 | MINOR | Concurrency: jobs import unmodified files that live in other campaign dirs (OPT-A builder md5 3e399a4b, cone DV builder 83dc63f1, shifted DV builder 2108365c, legacy `shifted_electron_kinematics_v4.py` 8c007695). Peer sessions write to this tree. | | Add an md5 pin check to `run_job.sh`, or copy the files into `$F` and record their md5s. |
| m5 | MINOR | Audit E ("shifted trees differ only in calculator kinematics") covered jes_down, jes_up-rebuild, rces_down and theta_up. I spot-checked theta_down and jes_down on data d06_0001: only `Q2da, Yda, Xda, Yes, Xes, Q2h, Yh, Xh` differ (100 branches), as claimed. Row counts differ at the stated Stage-1 level (MC em06_0003: jes_down −181 / theta_down −34 / rces_up +92 rows of 338,408; data rces_up +1). | check_leg_branches.py | None; consistent with the known ≈1e-4–5e-4 event-set differences. Document that rces_up and theta_down are now covered. |
| m6 | MINOR | The electron merge clone omits the OPT-A manifest annotation block `opta_additions_vs_2026_10_11_cone_merge`. It is informational only; the code-level OPT-A additions (variant pin, 5 counters, 3 constants) are all present (see merge diff-of-diffs). | merge_dod.py | Optional. |
| m7 | MINOR / physics note | Set R' is not shifted in the electron legs. For the electron **θ** legs, the α-cone itself moves (shift is applied before the cone), so the proposed bound (scale templates by the per-bin reco-yield change) does not capture the cone geometry for `e_cone`. | audit §D | Physics critic to rule on this. It does not need new response jobs, but if it is not accepted it needs the template producer (~10k jobs). |
| m8 | MINOR (recommendation) | No OPT-A-frame **null-mode** (frozen-Empz) HFS responses are produced. The "fileset vs n2" decomposition of the HFS change therefore exists natively only for jes_down (frame test 5816032). The other legs would need the cone frame plus re-denomination. | plan §1 | If reviewers will want to isolate the Empz effect per leg in the native frame, add 6×174 null shards now: ~6 CPU-h, +0.6 GB, same wrapper with `N2_MODE=null`. That avoids a second production. |

## What I reproduced or verified independently (PASS)

1. **Shim.**
   - I installed the shim, read ChargedTot through the proxy, and compared row by row against an independent dict-based recomputation of `Empz + 2·BeamElecE·(Yh_leg − Yh_twin)` on reco rows (0<Ys<1) with matched (Run, Event). Result: **exact equality** on:
     - data `d06_0145_hfs_jes_down`, recovered-twin fallback: 9,599 rows, 0 unmatched;
     - MC `djangoh14_7091_ep0607_0162_hfs_jes_up`, rebuild root: 305,275 rows, 42 unmatched reco rows (10 events, 3.4e-4); 263 rows cross 45 inward.
   - All other branches are untouched, and non-reco rows are unchanged.
   - Non-reco rows have Ys=1 and Empz=0, and none of them can pass the reco selection.
   - The corrected Empz is constant per event (0 violations).
   - dtype goes from float32 to float64. The builders cast with `.astype(float)` anyway, and null mode leaves the array untouched.
   - Yh-ratio guard headroom: max |Yh_leg/Yh_nom − 1| = 1.41% (rces_down), 1.36% (theta_down), 1.40% (jes_down) on a full DJ file, against the 10% bound. Min Yh_nom is 0.017, so spurious aborts are unlikely.
   - A file with 0 entries returns before `.arrays()`, so it is safe.
   - A key collision would trip the constancy check and fail loudly, not silently.
2. **Electron OPT-A clone, diff-of-diffs.** diff(shifted → shifted_optA) and diff(cone → OPT-A) each have 24 hunks. 22 are byte-identical. The other 2 differ only in docstring and comment text (the hand-applied hunks 1 and 15; the smoke CLI knobs are verbatim). The non-comment line multisets differ only in docstring lines.
   - The presets in `shifted_electron_kinematics_v4.py` are zero=(0,0), E±1% = (±0.01, 0), θ±1 mrad = (0, ±0.001). The stamps `.12g` give "0.01"/"-0.01"/"0.001"/"-0.001"/"1"/"-1"/"0", which match `SHIFT_PINNED` for all 5 legs and the signsym_minus precedent pins.
   - Smoke shards report `n_opta_q2esgen_lookup_miss=0` and `body_vs_pool_mismatch=0`.
3. **Merge.**
   - Coverage is 196 keys for elec and 188 for base.
   - The positive zero test passed; the negative test (cone-frame shard) failed on the 8 missing OPT-A keys, as required; HFS via the OPT-A merge passed.
   - HFS shard sidecars (`*.root.n2shim.json`) do not match the `*_<leg>.root` globs.
4. **Args and lists.**
   - 1914 shard lines (6 tokens each) and 18 DV lines (5 tokens). 0 commas or quotes.
   - All 1932 outputs are unique, under `studies/final_detlegs_optA_2026-10-21/`, and none exists yet.
   - Each leg×gen has 73 RG / 101 DJ shards.
   - **All 1044 HFS shard lists map file-by-file and in order onto the nominal OPT-A shard lists** (`cone_s2prime_2026-10-11/shard_lists`, which `optA_response_rebuild` itself used): 364 RG / 501 DJ files. jes_up uses its 2026-10-17 rebuild root.
   - All 7 DV lists have 660 entries, all exist, and the stems are in identical order. (The DV builders re-sort internally; counts do not depend on order.)
   - **New nominal DV(660) − recovered 2-file DV == the installed 658-file DV `cone_alpha_central_2026-10-11/production/data_vector_cone_alpha_central_2026-10-11.json`, bitwise.** The recovered share is 0.2502%. zero/nominal is +0.3053% (data).
5. **Condor plumbing.**
   - `run_job.sh` dispatches on 6 vs 5 arguments correctly.
   - It uses explicit `/usr/bin/python3`, so it needs no thish1 and has no `set -u` hazard.
   - A 660-path argv is ~112 KB, well under ARG_MAX.
   - Smoke runtimes: ~1 min per shard, ~1 min per DV.
6. **C_rad.**
   - The worker fork differs from the applied worker (md5 2c333932, identical for DJ and RG) only in the docstring, the veto line (the VetoAncestorPDG_v3 term moves into the `anc` counter), the `eta_jpsi_descendants_kept` counter, and the config flag `short_lived_ancestor_veto: False`.
   - `--no-rad-q2gki-gt100` disables `use_domain` for rad; nonrad never had the cut.
   - Queues: the input lists are **identical in order** to the applied `djangoh14_crad_releasebin_2026-10-09/input_queue.tsv` (2953) and to the emfix `input_queue.tsv` (1777). The emfix queue's targets point into the non-emfix partials dir; in the fork the targets are remapped 1:1 into `studies/final_crad_consistent_2026-10-21/{djangoh14,rapgap31}/partials/`, and none exists yet. All inputs exist. Sample labels are consistent with the filenames.
   - Emfix was a denominator-only fix, so reusing the same inputs is correct.
   - The finalizer forks differ from the originals only in BEAM, via the `CRAD_F_EP` environment variable (asserted to lie in (0.4, 0.7)). The exposures are unchanged.
   - The RAD exposure does not depend on removing the Q2Gki>100 selection, so keeping RAD exposures unchanged is correct.

## Luminosity update (coordinator, 2026-10-22)
L_eff = 350,890.85 nb⁻¹ (f_proc = 0.9996890, HAT-run based) and f_ep = 0.5439017. These are consistent with `lumi_eff_final.json`, and e+p equals the 0304+0607 periods. Neither enters any job of this campaign: the leg vectors are ratios, and C_rad takes f_ep only at finalize. Downstream, the same L_eff must scale the Set R' templates in **both** the nominal and every leg (D_leg − B_R'), or a (1 − L_eff/351,000) = 0.03% offset leaks into the legs.

## Answers to plan §5 Q5 (second-production risk)
- No missing leg, DV, or wrong file set found.
- The schema traps are covered by the full-key merges.
- The only optional additions worth making now are the m8 null shards.
- The real risk is **silent incompleteness** (M1–M3), not definitions. Close it with gates, not with reruns.
