# Hot critic: central response prior mismatch

**Reviewed (read-only):** `STATE.md` (2026-10-04); the 2026-10-04 investigation and its one-file audit source/output; central release `README.md`, `metadata.json`, `manifest.json`, and frozen `_run/run_bbb_ibu_release.py`; canonical `RUNLIST.md`, response builder `build_migration_th2_per_charge_v6.py`, and `crosscheck_unfold_methods_v4.py`; and the 2026-09-25 central audit JSON.

## Claim 1 verdict: SOUND

**Claim:** the audited migration ROOT is the response input consumed by the central IBU/BBB finalcandidate release.

**Evidence.** The release metadata explicitly records exactly the audited absolute path as `merged_root` (`metadata.json:23-26`). The immutable-in-release driver constructs the same path from the same `OUT`/variant (`_run/run_bbb_ibu_release.py:10-13,27-30`) and passes it to `load_response_and_data` (`:74-82`). The release identifies itself as that `tunfold_official_v4_finalcandidate...percharge` variant (`README.md:5-7`; `manifest.json:2-4`). Independently, the audit hard-codes that same root (`data/audits/.../audit_response_prior_mismatch.py:8`) and reports the matching 491x246 response objects (`summary.json:3-5,55-64,102-111`). This is a direct identity chain, not a filename inference.

### Attack result: naming trap — **FAILED**

`STATE.md:72-76` correctly says that v4/v5 **TUnfold** products are superseded in favor of native-v6 TUnfold diagnostics. That cannot erase the separately documented current central IBU/BBB release: the current `RUNLIST.md:7-12` calls it the IBU/BBB z005 finalcandidate release, while its release README calls its variant `tunfold_official_v4_finalcandidate...` (`README.md:5-7`). The dangerous naming is real, but it labels a common response-family/variant directory and a historical TUnfold lineage; it is not evidence of a different IBU response file.

### Attack result: wrong-file / checksum escape — **FAILED, with a provenance limitation**

No alternative migration path is recorded in `metadata.json`, the frozen release driver, `ibu/`, or `bbb/`: all supplied provenance points to the audited file. The release manifest hashes packaged release artifacts but does **not** contain a SHA-256 for its external ROOT dependency (`manifest.json:5-160`). Thus byte-level immutability of the ROOT between June and October is not proven. That is a missing hash guard, not positive evidence for a substitute input; the same live path is what the release driver consumes.

## Claim 2 verdict: FATAL (for use/certification of the current IBU k=4 result); acceptance subclaim rejected

**Claim:** central IBU k=4 consumes a plain `w_gen` prior rather than the response truth marginal because matched body entries use `w1*w2*NucliaWeight` while misses use `w_gen`; the discrepancy affects IBU and/or the release acceptance.

**Evidence of the mismatch.** This is not an integrated-anchor complaint. The audit directly computes the appropriate column identity, `prior[j]/(sum_i body[i,j]+miss[j])` (`audit_response_prior_mismatch.py:24-38`), using the physical 490 rows and the same-column miss row. It finds non-unity in **245/245 bins for each charge**, no stored NWeff histogram, and `body+miss` totals larger by 0.1035651% (h+) and 0.0957758% (h-) (`summary.json:55-100,102-146`). The per-bin `body+miss/prior` ranges are 0.964168–1.028029 and 0.965375–1.028519, respectively (`response_prior_mismatch_investigation_2026-10-04.md:20-25`).

**Evidence of mechanism.** The central builder fills plain `prior_gen` from accepted generator rows with `w_gen=w1*w2` (`build_migration_th2_per_charge_v6.py:464-470,769-782`); matched body entries use `w_reco=w1*w2*NucliaWeight` (`:591-603,818-826`); misses use `w_gen` (`:852-875`). It even documents `prior_gen_NWeff` as the matched-leg-consistent, "canonical for IBU/TUnfold" quantity while retaining ordinary `prior_gen` only for backward compatibility (`:304-322,877-900,1008-1021`). The audited central ROOT has no NWeff key (`summary.json:19-30,56-60,103-107`).

**(a) IBU k=4 — affected: YES.** The frozen release driver uses the RAPGAP loaded `prior_gen` as `P` in `ibu_unfold` (`_run/run_bbb_ibu_release.py:74-82`), and the loader resolves tagged plain `h_prior_gen_<charge>_<tag>` before fallback (`crosscheck_unfold_methods_v4.py:391-403,427-445`). IBU explicitly sets `A_ij=B_ij/P_j`, `eps_j=sum_i B_ij/P_j`, and initializes `n^(0)=P` (`:623-640,675-703`). Therefore a bin-dependent P/truth mismatch changes both the response normalization and starting distribution. The result is not merely a mislabeled histogram or harmless bookkeeping.

**Why fatal.** The intended response probability is not implemented with a denominator equal to the response's own full-truth column (`body+miss`). A response whose denominator and its stated truth marginal differ in every published bin makes the released IBU result non-certifiable until a coherent response/prior rerun demonstrates the effect. The exact k=4 shift is **not** established by these materials and must not be invented from the 0.1% total mismatch; the investigation's sub-percent-to-few-percent expectation is a hypothesis, not a replacement result (`response_prior_mismatch_investigation_2026-10-04.md:46-50`).

**(b) Release acceptance/efficiency — affected by this prior mismatch: NO.** This is the strongest escape and it succeeds. `derive_response_blocks` computes acceptance only as `A_j=sum_i B_ij/(sum_i B_ij+M_j)` from body plus miss (`crosscheck_unfold_methods_v4.py:516-563`), before `P` is supplied to IBU. The release calls BBB with this `A_j` (`_run/run_bbb_ibu_release.py:76-82`), and writes the same response-derived acceptance into both IBU and BBB tables (`:105`). Consequently the stored plain prior mismatch does **not** alter the release's acceptance calculation. It may diagnose an incoherent weighting convention, but Claim 2 must not say it directly biases release acceptance.

**(c) Only bookkeeping — NO.** Although the acceptance escape narrows the impact, the direct IBU code path above excludes a bookkeeping-only interpretation.

### Attack result: cancellation / iteration-renormalization escape — **FAILED as a rescue; impact magnitude remains unmeasured**

IBU re-normalizes posterior weights per reco row (`crosscheck...py:688-702`), but it contains no global or per-truth-bin operation that replaces `P` with `body+miss` or otherwise enforces column conservation. Four iterations can reduce prior sensitivity in well-constrained migration modes, but that is not a proof of cancellation; the code still uses mismatched P in `Aij`, `eps`, and `n^(0)`. No supplied k=4 coherent-prior rerun quantifies the residual.

### Attack result: intentional-design / plain-prior-is-correct escape — **FAILED**

A separately stored generator-pool prior could legitimately describe a physics generator spectrum, but then it cannot simultaneously serve as the denominator of `B/P` and the efficiency `sum B/P` unless it is the complete response truth marginal. Here those roles are exactly how it is consumed (`crosscheck...py:632-640`). The builder's own comments explicitly distinguish plain backward-compatible prior from NWeff, designed to make the matched-plus-miss convention coherent (`build_migration...py:314-322,877-900`). The 245-bin measured failure closes the remaining "merely unweighted by design" escape for this implementation.

### Attack result: acceptance-prior escape — **SUCCEEDED**

Acceptance uses `body/(body+miss)`, not plain prior nor NWeff. Accordingly the statement that the mismatch affects “the acceptance used by the release” is false; it affects IBU, not BBB acceptance.

## Non-Blocking Checks Confirmed

- The audit uses the correct **column-wise** miss convention, not an erroneous row sum (`audit_response_prior_mismatch.py:34-37`).
- The release contract is internally identified as 245 truth bins/charge, 490 reco bins/charge, response `(491,246)`, and k=4 (`RUNLIST.md:24-30`; release `metadata.json:27-31,46-60`).
- The finding does not claim a final physics release: the package itself labels it provisional, raw/no-background-subtraction, no `C_rad`, and not Born-level (`README.md:1-7`).

## Missing Evidence

1. A frozen checksum/creation provenance for the external migration ROOT is absent from the release manifest.
2. No controlled rerun holding data/fake treatment fixed while replacing P by the exact `body+miss` marginal is supplied; therefore the binwise k=4 numerical bias and any cancellation at k=4 remain unmeasured.
3. The central merged ROOT lacks the NWeff object, so the exact historical component split between NucliaWeight, matching eligibility, and merge provenance cannot be reconstructed from supplied artifacts alone.

## Recommended Next Check

Run a read-only controlled k=4 comparison on the exact recorded ROOT and data vector, replacing only IBU's P with its own `body+miss` column marginal (do not alter `A_j`), and publish the 490-bin delta plus a strict column-conservation gate before any central IBU value is used.
