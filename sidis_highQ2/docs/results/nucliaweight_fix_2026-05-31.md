# CC1 fix: NucliaWeight asymmetry in build_migration

> ## ⚠️ SUPERSEDED 2026-09-09 — THE "FIX" IN THIS MEMO IS PHYSICALLY WRONG. DO NOT APPLY.
>
> This memo asserts that `eps[j] = col_inner[j] / prior_gen_NWeff[j]` is the
> "pure geometric acceptance". **That is backwards.** The efficiency divided
> into DATA must be the efficiency DATA actually has.
>
> `NucliaWeight` is an MC-only down-weight (`NW = 1 - loss`, mean ~0.985) whose
> entire purpose is to lower the over-efficient simulated track efficiency to
> the real data efficiency. `H1NuclIACor::GetWeight` returns 1.0 on data.
> Therefore NW belongs in the response NUMERATOR only; the gen prior must stay
> the unweighted particle-level yield.
>
> With `C` = matched gen yield, `M` = miss gen yield, `n = <NW> < 1`:
>
> - CORRECT (shipped): `eps_old = nC / (C + M)`, so `D / eps_old = C + M = T`.
> - WRONG (this memo): `eps_NWeff = nC / (nC + M)`, so `D / eps_NWeff = nC + M < T`.
>
> `nC/(nC+M)` is neither the geometric acceptance nor the data-like efficiency.
> Using it removes the nuclear-loss correction from `eps` and restores the
> over-efficient MC value.
>
> **Status of the canonical v6/z005 release:** CORRECT AS SHIPPED. The merged
> response
> `output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/migration_inputs_v4_finalcandidate_*.root`
> contains ONLY the ordinary `h_prior_gen_{pos,neg}`; no `*_NWeff` object exists
> in it. The IBU loader (`crosscheck_unfold_methods_v4.py:391-403`) reads the
> ordinary prior. The NWeff objects were dropped at merge
> (`merge_migration_per_charge_v4.py:427-431,601-613`). That drop was silent and
> undocumented, but it left the release physically correct.
>
> **Had NWeff been used**, released cross sections would be biased LOW by
> 0.4502% (h+) and 0.4515% (h-), from measured integral
> `prior_old/prior_NWeff` = 1.004522 (h+) / 1.004536 (h-) on one rapgap31 em0405
> per-file migration output. Note the sign: this memo predicted a +1% UPWARD
> shift; the correct sign is downward.
>
> **Charge-differential residual:** `r+/r- = 0.9999861`, i.e. -1.4e-5 on h-/h+.
> NucliaWeight induces no meaningful fake charge asymmetry.
>
> **Still open (not fixed by this supersession):** the BBB acceptance in
> `crosscheck_unfold_methods_v4.py:535-563` is `A_j = sum_i B_ij / (sum_i B_ij + M_j)`,
> which is algebraically `eps_NWeff` and therefore carries the same ~0.45% low
> bias. BBB is a shipped diagnostic, not the central method; IBU k=4 is central.
> Tracked for repair.
>
> Retained below for audit provenance only.

> 📍 **Entry-point docs:** [RUN_STATUS.md](../RUN_STATUS.md) (what's current) · [LESSONS_LEARNED.md](../LESSONS_LEARNED.md) (structural insights) · [rev6_audit/SYNTHESIS.md](../critiques/rev6_audit_2026-05-31/SYNTHESIS.md) (open issues).

---


**Date:** 2026-05-31 (continued, after rev6 audit)
**Issue:** Rev6 audit synthesis CC1 — NucliaWeight applied to matched-reco fill (`w_reco = w1·w2·NW`) but not to gen prior fill (`w_gen = w1·w2`). Asymmetry caused ε[j] = col_inner/prior to be biased LOW by ~1%, inflating 1/ε acceptance correction by the same amount.

## Diagnosis

- **NucliaWeight on ChargedTot (reco) only**: median 0.9914, mean 0.9864, min −2.58 (clipped to 0), max 1.000. Reco-only nuclear-interaction correction.
- **No gen-level analog** on ChargedTotGen. The asymmetry is structural: gen has no NW, reco does.
- The physically correct fix: define a NW-effective prior such that ε[j] = col_inner / prior_NWeff is the pure geometric acceptance, free of NW.

## Fix design (Option F: per-matched-event NW weighting of prior)

For each gen-pool row in PS, look up its (Run, Event, signed-PDG, round(pt,5), round(eta,5)) key in the consumed-keys dict built during the matched fill:
- **Matched gen row** (key in dict): weight by `w_gen × NW_partner` where NW_partner is the NucliaWeight of the matched reco partner.
- **Miss gen row** (key not in dict): weight by `w_gen` (NW = 1; no reco partner means no nuclear correction applies).

This produces `prior_gen_NWeff[j] = matched_gen × <NW>_partner + miss_gen × 1`. Algebraically:
- `col_inner[j]` is matched-reco at scale `w1·w2·NW`.
- `prior_gen_NWeff[j]` is on the same scale (matched contribution has NW, miss does not need it).
- `ε[j] = col_inner / prior_NWeff = matched_geometric / total_geometric` — pure acceptance, NW-cleaned.

## Implementation

4-part patch to `build_migration_th2_per_charge.py`:
1. Add `prior_gen_NWeff` and `prior_gen_NWeff_w2` arrays to `PerChargeAcc.__init__`.
2. Change `consumed_keys` from `set()` to `dict()` storing `key → NW`.
3. In the matched fill loop, store the partner NW (extracted as `w_reco / (w1·w2)` from in-scope ct_batch).
4. After the existing miss-fill loop (which already iterates gen rows), add a new step 8 that fills `prior_gen_NWeff` for ALL gen rows in PS, using NW from `consumed_keys.get(key, 1.0)`.

Companion patch to `merge_migration_per_charge.py`: read and write the new `h_prior_gen_<chg>_NWeff` histograms.

The original `h_prior_gen_<chg>` histograms are **kept for backward compat and audit**.

## Smoke test on 1 file

Test sample: `output/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root` (1 file, 258K reco tracks, 67K matched per charge).

| Metric (h+) | prior_old (unweighted) | prior_NWeff (NW-corrected) |
|---|---|---|
| Integrated prior | 1.6882e5 | 1.6793e5 |
| Ratio NWeff/old | 1.000 | **0.9947** (expected ~0.99 from NW mean) |
| ε[j] median | 0.7157 | **0.7181** (+0.3%) |
| ε[j] max | 1.2122 | 1.2185 |
| bins ε > 1.0 | 4/232 | 4/232 |

Both priors agree to 0.5% globally; ε rises by +0.3% per bin median as expected. The bins with ε > 1.0 are statistical fluctuations in this single-file sample (only 67K matched events; production has ~150K per bin).

## Self-closure test

IBU rev6 with both priors against `data = matched_reco_marginal` (textbook self-closure):

| Prior used | Unfolded total | self-closure |unf/prior − 1| median | max |
|---|---|---|---|
| prior_old | 1.6869e5 | 0.000000 | 1.0 (zero-eps bins) |
| prior_NWeff | 1.6781e5 | 0.000000 | 1.0 (zero-eps bins) |

Both close to machine precision. The IBU machinery is consistent with either prior — the difference is just whether ε is "absolute acceptance" (NWeff) or "acceptance × NW factor" (old).

## Production status

Cluster 3560523 launched: 8 jobs (4 rapgap periods + 4 djangoh periods), expected ~30 min runtime each. Outputs land in `output/tunfold_tree_v3_percharge/per_file_migration_NWfix/<mc>/`.

Next: when production completes, run the patched merge to produce a new `migration_inputs_v3_percharge_NWfix.root`, then re-run IBU rev6 and TUnfold rev5 reading the new file with the NWeff prior. Expected change in headline: **~+1% upward shift in unfolded integrated totals** (since prior_NWeff is ~1% smaller, the 1/ε correction is ~1% smaller, but the geometric ε is now ~1% larger → both methods should land at ~3.22-3.23e6 instead of 3.19-3.20e6 for h+).

## Files

- `sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge_NWfix.py` (1039 lines, +57 vs original)
- `sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge_NWfix.py` (~30 lines added to existing)
- NAF: `scripts/unfolding/tunfold_official_v3_percharge/{build,merge,wrap_build,submit_build}_*_NWfix.{py,sh,condor}`
- Condor cluster 3560523: 8 production jobs (running).
- Smoke output: `output/test_NWfix/test_NWfix_smoke.root` (1 file, verified prior_NWeff histograms present).

## Open downstream items (post-production)

1. Run patched merge to produce `migration_inputs_v3_percharge_NWfix.root`.
2. Re-run IBU rev6 reading `h_prior_gen_<chg>_NWeff` instead of `h_prior_gen_<chg>`. Quantify headline shift.
3. Re-run TUnfold rev5 with same. Quantify headline shift.
4. Re-run rev6 closure to confirm no new failure modes introduced.
5. Update per-bin σ tables to use NWeff prior.
