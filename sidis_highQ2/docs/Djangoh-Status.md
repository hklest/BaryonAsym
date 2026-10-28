# DJANGOH smoke-test handoff

## Goal
Produce a small matched **DJANGOH14+ARIADNE4.10** **RAD/NORAD** generator-level validation smoke on **Condor**, then use the validated sample for generator-level **C_rad** studies in the **high-Q² charged-SIDIS** analysis.

**Chosen C_rad convention:**

- `C_rad = sigma_norad / sigma_rad`
- This is a **cross-section ratio** convention, **not** a multiplicity ratio.

## Key paths

| Item | Path |
|---|---|
| Private NAF test root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/crad/generator_test_rad_norad_2026-06-15/` |
| Provenance / report root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/crad/full_v1/provenance_djangoh_cards_2026-06-15/` |
| Private dependency root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/_deps/djangoh14_ar410_readline6/lib/` |
| QA report for full_v1 recomputation | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/crad/full_v1/provisional_xsec_crad_2026-06-14/qa_report_retry2.json` |

### Card provenance found

| Type | Path |
|---|---|
| NORAD e− | `/data/dust/group/h1/britzger/norad_prod21/djangoh14_ar4.10/stdjan_nc_eminusp_norad_q2gt100_cteq6l_a` |
| NORAD e+ | `/data/dust/group/h1/britzger/norad_prod21/djangoh14_ar4.10/stdjan_nc_eplusp_norad_q2gt100_cteq6l_a` |
| RAD e+0607 | `/data/dust/group/h1/britzger/norad_prod21/guenterg/djangoh14_files/prod2/stdjan_nc_eplusp_0607_rad_q2gt60_cteq6l_a` |
| RAD e−05 | `/data/dust/group/h1/britzger/norad_prod21/guenterg/djangoh14_files/prod2/stdjan_nc_eminusp_05_rad_q2gt60_cteq6l_a` |
| Executable | `/data/dust/group/h1/britzger/norad_prod21/djangoh14_ar4.10/djangoh14-ar4.10` |

Useful provenance files already prepared in the provenance root:

- `card_diff_summary.json`
- `README_card_diff.txt`
- `generator_test_run_plan.md`

## What succeeded

1. **full_v1 C_rad recomputation completed** via Condor cluster **`3694896.0`** after increasing memory to **10 GB**.
2. Outputs were refreshed **2026-06-15 07:40**.
3. QA conclusion: this is **provisional full_v1 only**, **not** release-ready **z005**.
4. Card provenance search succeeded and identified the likely source cards and executable listed above.
5. Card-diff study concluded that RAD/NORAD cards are mostly aligned; the main historical mismatch is:
   - RAD uses **`Q2MI=60`**
   - NORAD uses **`Q2MI=100`**
6. Recommended matched-smoke convention already established:
   - use **common `Q2MI=60`**
   - use the **same executable**
   - keep **same PDF / weights / tune**
   - differ only in **radiation switches, output settings, and seeds**
7. Private cards were already prepared under the private test root and had path rewriting applied.
8. `/pnfs` and `/acs` paths were removed; validation passed in:
   - `reports/path_rewrite_report.json`
9. Initial dependency failure was diagnosed and fixed:
   - cluster **`3700320`** failed with **exit 127** because `libreadline.so.6` was missing
   - fixed by staging a real private `libreadline.so.6` under the dependency root
   - wrapper added: `condor/run_generator_smoke_fixeddeps.sh`
   - `ldd` passed after the fix
   - report: `reports/dependency_fix_report.json`
10. Stale held proc **`3700320.3`** was removed.

## What failed

| Cluster | Iteration | Result | Notes |
|---|---|---|---|
| `3705744` | fixed-deps | Failed | Wrapper ended with bare `exec`, ignored args, exited 0, produced no outputs |
| `3705854` | retry2 | Failed | Submit problem: `Args` undefined |
| `3705870` | retry3 | Failed | Generator ran, then failed `OPEN LOGOUTPUT ... IOS=17 STOP 333`; a few nonzero GEN outputs appeared, but `.LOG` files were zero-byte |
| `3705954` | retry4 | Failed | Fresh namespace still hit IOS=17; long absolute `FILE=` paths in cards still present |
| `3706153` | retry5 | Failed | Short relative path strategy attempted, but cards contained literal escaped quotes `FILE=\"job.bos\"`; parser rejected them and all jobs exited 77 |
| `3714554` | retry6 | Failed | Plain quotes fixed and jobs ran, but all still exited 77; retry6 reports missing; `.LOG` zero-byte; `gen/` outputs missing; Condor stdout is large (~85 KB/job) and likely contains the real failure detail |

## Current blocker
The likely blocker is still **card parse/runtime failure before useful log output is opened**.

Most likely diagnosed root cause so far:

- DJANGOH / H1 BOS steering uses a **fixed-width parser** for card `FILE=` values.
- Long absolute DUST paths appear to be truncated at about **80 characters**.
- Retry5 additionally failed because `FILE=` lines had **literal escaped quotes** instead of plain quoted strings.
- Retry6 fixed the quoting issue but still exits **77**, so there is likely a remaining syntax or steering-format problem in the retry6 cards or wrapper execution environment.

## Recommended next steps

1. **Inspect retry6 Condor stdout first** under:
   - `.../logs/condor_fixeddeps_retry6/`
2. **Inspect the retry6 cards** under:
   - `cards/fixeddeps_retry6/`
3. For retry7, verify all of the following before submission:
   - `FILE="..."` is **not** present anywhere
   - all output cards use short local relative paths such as:
     - `FILE="job.bos"` is wrong if backslashes are literal
     - `FILE="job.bos"` must become plain `FILE="job.bos"` in rendered markdown? No: actual card text must be `FILE="job.bos"`? Avoid ambiguity by checking the real file; the card must contain **plain quotes only, no backslashes**: `FILE="job.bos"` should **not** appear in the file; the file should read `FILE="job.bos"`? Again: inspect directly and ensure the literal card text uses plain quotes around `job.bos` with no escaping artifacts.
   - no long absolute DUST paths remain in `FILE=` values
   - no broken continued `*FILE ... &` fragments remain
   - wrapper runs in a **per-job working directory**
   - generator writes outputs **locally first**
   - copy to DUST happens **only after successful local generation**
4. Keep the matched-smoke physics setup fixed:
   - common `Q2MI=60`
   - same executable
   - same PDF / weights / tune
   - RAD/NORAD differ only where intended
5. After retry7 succeeds, do a minimal validation that the smoke is matched and usable for generator-level `C_rad = sigma_norad / sigma_rad` studies.

## Safety notes

- Before any NAF command, run:
  - `ssh -O check naf-hone.desy.de 2>&1`
- If the SSH control-socket check fails, stop and ask the user to re-establish it manually.
- Do **not** retry failed SSH commands automatically.
- Do **not** run broad `find` on `/afs`, `/pnfs`, or `/cvmfs`.
- Use **Condor** for generator runs; login node use should stay limited to small inspection/debug checks.
- Do **not** touch production templates while debugging this smoke; keep work under the private test area.
