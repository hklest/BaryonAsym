# Cold critique: central response/prior mismatch

**Material reviewed (read-only):** central-release `manifest.json`, `metadata.json`, `README.md`, `_run/run_bbb_ibu_release.py`, `_run/bbb_ibu_crosscheck_rapgap_pos.json`, and run log; the canonical `RUNLIST.md`, `SEVEN_CATEGORY_SCRIPT_INDEX.md`, response builder, and IBU dependency; `STATE.md`; the 2026-10-04 investigation and its `summary.json`/`audit.log`; and the 2026-09-25 `central_audit_numbers.json`.

## Verdict — CLAIM 1: SOUND

The audited file is the response ROOT consumed by the central IBU/BBB finalcandidate release. The release metadata names this exact `merged_root` at lines 23--26, and the preserved release driver constructs the same path as `ROOT` (lines 10--13 and 27), checks that it exists (line 206), and passes it to `load_response_and_data` (lines 74--78). The release manifest identifies the same finalcandidate variant (line 3) and records the driver (lines 32--39). The canonical index labels that driver and its IBU dependency `CURRENT_CENTRAL` / `CURRENT_CENTRAL_DEPENDENCY_FOR_IBU_BBB_RELEASE` (lines 88--94). The release log records 490 rows and IBU selected iteration 4 (lines 5--19). This is direct provenance, not a naming inference.

## Verdict — CLAIM 2: FATAL

The central IBU calculation has a demonstrated response/prior inconsistency. The audit's recorded computation is column-wise `prior[j]/(sum body rows + miss row)` (`summary.json` line 5). It records no NWeff object (lines 54--60 and 102--107), non-unit ratios in all 245 bins per charge (lines 61--85 and 108--132), and the stated totals: h+ body+miss `91,879,281.48165227` versus prior `91,784,225.07914555` (+0.1035651%); h- `90,271,538.73605749` versus `90,185,163.14188254` (+0.0957758%). The per-bin truth/prior ranges are 0.9641677--1.0280295 (h+) and 0.9653747--1.0285185 (h-).

### Fatal Issues

- **Claim criticized:** the release IBU response is normalized with a stored plain prior that is not its truth marginal.
  - **Evidence:** the release driver passes `ld["prior_gen"]` to `ibu_unfold` (release driver line 78). The loader selects the tag-qualified `h_prior_gen_{charge}_rapgap` before fallbacks (IBU dependency lines 391--403). `ibu_unfold` explicitly forms `Aij=B/P`, `eps=sum_i B_ij/P_j`, and starts with `n^(0)=P` (lines 623--640 and 675--702). The central release calls `rapgap` (driver lines 89, 212--215), and its crosscheck records `response_tag: "rapgap"` (JSON lines 3--10).
  - **Why it matters physically:** the implementation uses `P` in both the response normalization/efficiency and the initial IBU state. Since the documented release `P` is not `body+miss` in every truth bin, the IBU correction is not based on a self-consistent response truth marginal. This is analysis-killing for the present central IBU bin values until a coherent response/prior rebuild and rerun establishes replacement values.

- **Claim criticized:** the stated mechanism is an unmatched weight definition between ordinary prior, matched body, and miss.
  - **Evidence:** the canonical builder declares ordinary `prior_gen` and an NW-effective prior separately, calling the latter canonical for IBU/TUnfold (builder lines 304--322). It defines matched `w_reco = weight1*weight2*NucliaWeight` (lines 591--603), fills the ordinary prior from `w_gen` (lines 769--782), fills the body with `w_reco` (lines 818--826), and fills misses with `w_gen` (lines 852--875). It documents that the effective prior weights a matched gen row by its partner NW and a miss by one (lines 877--900), and writes it as `h_prior_gen_*_NWeff` (lines 1008--1021). The audited central file's recorded key list has plain tagged priors but no NWeff keys (`summary.json` lines 6--52 and 54--60).
  - **Why it matters physically:** a plain `w_gen` denominator cannot equal a body containing the extra matched-leg NW factor plus a `w_gen` miss contribution unless an accidental bin-wise cancellation occurs. The audit demonstrates that no such cancellation occurs in any central RAPGAP truth bin.

### Non-Blocking Checks Confirmed

- **(a) IBU k=4 result:** affected. This follows directly from the central driver passing the plain prior to `ibu_unfold` and from the implementation using that argument to construct both `Aij`/`eps` and `n^(0)` (cited above). The submitted evidence does not contain a coherent-prior k=4 rerun, so it does **not** establish a numerical shift for any released bin.
- **(b) Acceptance/efficiency used by the release:** the stored-prior mismatch does **not** affect the separately calculated BBB/release `acceptance_rapgap` value. `derive_response_blocks` defines `A_j = sum_i B_ij / (sum_i B_ij + M_j)` exclusively from body and miss (IBU dependency lines 516--563). The release computes BBB from `blocks["A_j"]` (release driver lines 76--80) and writes `acceptance_rapgap` from that same array (line 105). This acceptance is not calculated with `prior_gen`.
- **(c) bookkeeping only:** rejected for IBU. It would be bookkeeping-only only if the plain prior were absent from the IBU formula. It is an explicit IBU argument and formula operand (release driver line 78; IBU dependency lines 632--640, 675--702).

### Missing Evidence

- No post-fix response/prior ROOT file, k=4 rerun, or bin-level old-versus-coherent IBU comparison was supplied. Therefore the sign and magnitude of the released IBU bias cannot be certified from these materials.
- No direct historical build log/source hash was supplied that ties the exact audited ROOT production to the reviewed canonical builder revision. The release consumer identity and the ROOT contents are directly documented; the builder fill-path explanation is supported by the reviewed snapshot and investigation, but exact historical source identity is not documented in the supplied release artifacts.
- No uncertainty/covariance rerun with the coherent prior was supplied.

## Recommended Next Check

Rebuild the central RAPGAP response with `h_prior_gen_{pos,neg}_rapgap` defined exactly as each response column's `body+miss`, then rerun the preserved release driver at `k=4` and record a 490-bin old-versus-new IBU comparison plus a `body+miss/prior = 1` assertion for both charges.
