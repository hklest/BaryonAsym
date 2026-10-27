# WP-A gate G1 verdict

Date 2026-10-27. Package WP-A of `.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`; child plan `wp-a_plan.md`.

**Ruling: KILL** (issued by `h1-analysis-critic`; see the last section).

## Pre-registered gate text (verbatim, parent plan lines 142-155)

> - **G1 (WP-A, kill gate).** On the Stage-B N−1 sample, stack C, signed-v3 weights, after the
>   nominal remedy: (a) Plug N−1 unit-area max \|MC/DATA−1\| ≤ 0.10 in bins with ≥ 20 data entries,
>   and \|f_fail,MC − f_fail,DATA\| ≤ 0.02 absolute in each D cell (data fractions 0.207/0.230/0.283
>   carry ≤ 0.007 stat; 0.02 ≈ the 10% PASS window of
>   `D/docs/t4/datamc_fail_diagnosis_2026-10-19.md:10` applied to a rate); (b) FMD and FTS N−1 < 0.25
>   (below the pre-registered FAIL boundary) for at least one admissible cocktail (non-negative weights within
>   ±50% of the signed-v3 scales); (c) the §7.1 two-free-template η_max fit on the rebuilt cut-removed
>   payload reaches template-statistics-aware toy p ≥ 0.05, coarse and fine, with ≥ 1000 toys, for at
>   least one ND alternative (the conventional 5% level; calibrated toys make a χ²/ndf ceiling
>   redundant; the prior fits sat at the 300-toy floor p ≤ 0.0033-0.01, so a pass is a ≥ 5-15× change,
>   not a marginal one). **Kill:** the WP-A budget (Open Q1; default 10
>   Condor-days, 2 calendar weeks, whichever comes first; Condor-days = Σ(job wall × request_cpus)/24 h
>   from the Condor logs, tallied by `h1-naf-operator` in the WP-A Log) is spent with (a) or (c) failing. (b) failing alone downgrades to a
>   systematic leg, not a kill.

Supervisor readings that bind this ruling (child plan Log):
- **G1(c) pass scope:** every D cell and ALL, coarse and fine.
- **Remedy 3 (A9):** G1(a) is read as FMD/FTS N−1 ≤ 0.10 plus a Stage-B yield ratio within 1 ± 0.10 after the PDISS M_Y > 1.6 migration term.
- **Ordering (A10/D3):** the first remedy that passes is nominal and a later passing one is a systematic leg. If no remedy passes, the ruling is KILL.

## Numbers behind the ruling

All numbers are on the Stage-B N−1 test sample (control events excluded; disjointness proven), stack C, signed v3 weights.

**G1(a): Plug N−1 shape (limit 0.10) and |f_fail,MC − f_fail,DATA| per D cell (limit 0.02).**

The data f_fail on the test sample is 0.219 / 0.236 / 0.285 (D0/D1/D2). On the full frozen sample it reproduces 0.207 / 0.230 / 0.283.

| remedy / ND alternative | MC f_fail D0 / D1 / D2 | max \|Δ\| | Plug N−1 max dev | (a) |
|---|---|---:|---:|---|
| none / 7071 | 0.043 / 0.059 / 0.094 | 0.191 | 0.977 | FAIL |
| 1 / 7071 | 0.214 / 0.229 / 0.247 | 0.037 | 4.04 † | FAIL |
| 1 / 7091 | 0.210 / 0.223 / 0.239 | 0.046 | 6.36 | FAIL |
| 1 / 7094 ‡ | 0.257 / 0.253 / 0.297 | 0.039 | 6.80 | FAIL |
| 1 / nd4cat | 0.213 / 0.232 / 0.242 | 0.043 | 6.09 | FAIL |
| 2 | no pre-registered threshold (3-15 GeV) satisfies A8 in the control region: Plug-fail \|z\| ≥ 2.2-3.5 at 15 GeV | — | 0.977 (threshold-independent) | not evaluable, no nominal |
| 3 (A9) / 7071, 7091, 7094, nd4cat | FMD N−1 2.30 / 1.84 / 1.07 / 2.33; FTS 0.73 / 0.73 / 0.76 / 0.70; yield ratio D0-D2 1.21-1.51 / 1.24-1.96 / 1.33-2.36 / 1.21-1.55 | — | — | FAIL |

- † The remedy-1 Plug N−1 values (4.0-6.8) are **stack-attributed, not a validation**.
  - A5 puts the whole stack excess into Plug, about 3× the Plug-only excess (h1-mc-expert S2d).
  - The maximum sits in one high-ePlug bin of a 12-35-event excess spectrum.
  - The physics-meaningful shape failure is the uncorrected 0.977 (see "Why it fails" §1). The best rescale still leaves a deviation of about 0.65.
- ‡ For chain 7094 the projected control cocktail has no ND member in ep0304, em0405 and em06, because 7094 represents only ep0607. Its q is therefore inflated to 0.232 / 0.258 / 0.245 in those periods (0.191 in ep0607), which explains the D0 overshoot. The ruling is unchanged: (a) fails on the shape alone, and (c) is free-normalised and at the toy floor.
- The remedy-3 yield ratios for 7094 are inflated and not comparable, because 7094 represents ND only in ep0607. The ruling does not rest on them: FMD N−1 and G1(c) fail independently.

**G1(b): FMD and FTS N−1 < 0.25 for at least one admissible cocktail.** The scan covered 1.95 M cocktails, using multipliers 0.5-1.5 in steps of 0.25.
- Best joint maximum after remedy 1: 0.759 / 0.709 / 0.680 / 0.738 (7071 / 7091 / 7094 / nd4cat).
- Best joint maximum after remedy 3: 0.914 / 0.762 / 0.710 / 0.920.
- FAIL, which on its own would only be a systematic downgrade.

**G1(c): §7.1 toy p ≥ 0.05, coarse and fine, every D cell and ALL.** Each fit used 1000 toys (floor 0.0010).

| remedy / alt | max toy p over 8 fits | ALL fine p, D/ndf | ALL coarse p, D/ndf | (c) |
|---|---:|---|---|---|
| 1 / 7071 | 0.011 (fine D2) | 0.0010, 6117/358 | 0.0010, 5130/115 | FAIL |
| 1 / 7091 | 0.002 | 0.0010, 6530/358 | 0.0010, 5619/115 | FAIL |
| 1 / 7094 | 0.0010 | 0.0010, 6272/358 | 0.0010, 4980/115 | FAIL |
| 1 / nd4cat (A7 fallback) | 0.003 | 0.0010, 5902/358 | 0.0010, 5052/115 | FAIL |
| 3 / 7071, 7091, 7094, nd4cat | 0.0010 (all 32 at floor) | 0.0010, 8609 / 8884 / 8135 / 7651 over 358 | 0.0010, 7569 / 8231 / 7029 / 6972 over 122 | FAIL |

The 2026-10-20 baseline (7071, fine ALL) was D = 5925.8/358 with p = 0.0033 at the 300-toy floor, and it is reproduced here. No remedy moves p by the ≥ 5-15× the gate text calls a pass.

## Remedies used

- **Nominal:** none. No remedy passes G1.
  - Remedy 1 (control-region stack-removal probability q_k = 0.152-0.192 per period for 7071/7091/nd4cat, and 0.191-0.258 for 7094 ‡; A5 ePlug replacement) fails (a), (b) and (c).
  - Remedy 2 has no admissible threshold.
  - Remedy 3 fails (a)-A9, (b) and (c).
- **Systematic legs:** none, since a leg needs a passing remedy (A10).
- **Every pre-registered remedy has been run.** The A7 four-catalogue fallback was also run, for both remedy 1 and remedy 3.

## Deviations from the pre-registration (none changes the ruling; h1-analysis-critic)

- **Removal probability:** q = (f_data − f_MC)/(1 − f_MC) replaces A4's p = f_data − f_MC. This is the exact control-region closure of the independent-noise model, endorsed by h1-mc-expert.
  - Using p would lower MC f_fail by 0.03-0.04 and make the rate clause worse (D2 Δ ≈ −0.07).
  - It cannot move G1(c).
- **Code path:** `payload_wpa.py` (n1-partial cut-removed blocks, identity-proven) replaces `eta_builder_wpa.py` plus the v3 assembler, and `g1_eval_wpa.py` replaces the assemble/analyze forks.
- **ND alternatives:** 7094 was added as a third alternative before any fit (coordinator, WP-D MC-3).
- **Waived test:** `removal_rate_closure` FAILS its literal per-row 3σ rule in one of 77 rows (7071|ep0304, pull −3.26). It is waived on the look-elsewhere argument (P(max |pull| ≥ 3.26 in 77 rows) ≈ 0.08; rate offset 0.4% of q).
- **MC period assignment:** periods come from the composite-scale projection, not the sim RunNumber. q spans only 0.155-0.191, so any misassignment moves MC f_fail by < 0.01.
- **Process:** clusters 5902092-5902096 were submitted by the lead and audited after the fact by h1-naf-operator.

## Why it fails (attribution, for the parent decision)

1. **The Plug N−1 mismatch is mostly below threshold.** In 1-7 GeV the MC/DATA unit-area ratio is 0.29-0.44, and at 0 GeV it is 1.35. Event removal (remedy 1) or a threshold move (remedy 2) cannot reshape it.
2. **The forward energy scale is low in MC.** In the η_max > 3.5 region the MC ⟨ePlug⟩ is 9-17% below data, which points to a Plug response deficit rather than noise alone.
3. **The control-region excess is not Plug-dominated.** The data − MC fail excess is about 0.065 for Plug, 0.054-0.101 for FMD and 0.036-0.073 for FTS. It persists in the deepest gaps (0.09-0.18 for η_max < 0), which is consistent with forward activity that the 2-4-run random-trigger overlay snapshot does not reproduce (h1-mc-expert).
4. **The §7.1 shape failure (D/ndf ≈ 17) is independent of the Plug treatment.** It is the same or worse under every remedy.

## Budget tally (`D/data/restart_2026-10-27/wp-a/condor_tally.csv`, signed by `h1-naf-operator`)

- **Start:** 2026-10-27.
- **Condor:** 175 jobs in clusters 5902090-5902096, all rc 0, total wall 2306 s × 1 CPU = **0.0267 Condor-days** of the 10 allowed.
- **Calendar:** **1 of 14** days used.
- **Login node:** login-node runs count zero.

The budget is not spent. The ruling is KILL rather than CONTINUE because no pre-registered remedy remains unrun (S5 rule; A10). Any further attempt needs a new, user-approved remedy (open items).

## Records

`s1_period_split_record.md`, `s2_remedy1_record.md`, `s3_gof_record.md`, `s4_remedies_record.md`, `s1c_mc_expert_lookup.md`, `s2d_mc_expert_signoff.md`, `s4d_mc_expert_migration.md`. Each record cites its partials, payloads and sha256 values under `D/data/restart_2026-10-27/wp-a/`.

## Ruling by h1-analysis-critic

**RULING: KILL** (h1-analysis-critic, 2026-10-27; read-only review, numbers spot-checked against `g1c_fit_table.csv`, `s2_remedy1/analysis/g1_7071.json` and `condor_tally.csv`).

- **CONTINUE is unavailable.** The S5 rule needs both remaining budget and an unrun pre-registered remedy. Remedies 1, 2 and 3 have all run, plus the A7 nd4cat fallback for remedies 1 and 3.
- **PASS is unavailable.** No remedy passes G1(a) or G1(c) for any ND alternative.
- **The budget-spent kill clause** is a sufficient condition, not the only one. Under A10 and D3 (fixed remedy order, no tuning), once the registered remedies are exhausted the remaining budget has no admissible use. Anything new would be an unregistered remedy.
- **KILL is not premature.**
  - No in-scope variant (per-detector attribution, p instead of q, sim-period assignment, any threshold, any G1(c) scope reading) plausibly turns both (a) and (c) into a pass.
  - The (a) shape failure is sub-threshold and removal-invariant.
  - The (c) payload and its η_max shape do not depend on the q attribution, and every ALL fit sits at D/ndf 7-30 at the toy floor.

No BLOCKING findings. The SHOULD items (deviation labelling, stack-attributed Plug values, the waived closure test, non-comparable 7094 yields, the missing coarse D/ndf) are applied above.
