# Cold critique: coherent-prior IBU magnitude study and replacement proposal

## Verdicts

- **V1 (implementation fidelity): CONCERN.** The baseline central values are reproduced to `3.311839017904824e-15` and the study holds the documented response, data file, fake fraction prescription, `K=4`, and luminosity fixed. However, its integrated `initonly` and `effonly` attribution values are demonstrably calculated against the coherent result rather than their respective variants; those two reported integrated attribution numbers are not faithful.
- **V2 (fix correctness): CONCERN.** `body+miss` is exactly self-consistent with the stored response by construction, but the supplied builder source does not prove it is identical to the builder-designated `prior_gen_NWeff`: the matched body uses `weight1*weight2` from `ChargedTot`, whereas NWeff uses `w_gen` from `ChargedTotGen` multiplied by partner NW. No document proves equality of the two trees' `weight1*weight2` per matched object, and central ROOT has no stored NWeff object.
- **V3 (replace now): NO.** The single decisive missing item is a response rebuild that writes NWeff plus a recorded 490-bin equality check `body+miss == prior_gen_NWeff` (or a recorded matched-pair proof that the two tree weight products are equal); without it, the intended coherent prior is ambiguous.

**Material reviewed (read-only):** study README, script, validation, summary, per-bin comparison, status/logs, and NWeff availability record; central release driver and IBU dependency reviewed in the preceding critique; builder `build_migration_th2_per_charge_v6.py`; and `STATE.md`.

## Major Concerns

### 1. V1: integrated attribution calculation is wrong

- **Claim criticized:** the study's recorded attribution gives meaningful integrated shifts for `initonly` and `effonly`.
- **Evidence:** in `run_coherent_prior_study.py` line 67, `ir=(ini-base)/base` and `er=(eff-base)/base` are formed correctly. But `ss(x)` at line 69 always defines `integrated_relative_shift` as `(sum(coh)-sum(base))/sum(base)`, independently of its `x` argument. Lines 75 then apply that function to both `ir` and `er`. Accordingly, `comparison/summary.json` reports the same integrated shift for coherent, init-only, and efficiency-only: h+ `0.001280464557872736` (lines 19, 77, 85) and h− `0.0011993669953095278` (lines 45, 95, 103).
- **Physical relevance:** this is a reporting/attribution defect. It does not invalidate the explicitly per-bin `ir`, `er`, and coherent-minus-baseline arrays produced at script lines 67 and 78, nor the central-value coherent comparison. It prevents accepting the two integrated attribution claims as evidence.
- **Severity:** follow-up required before using attribution summaries; not, by itself, evidence that the coherent central-value delta is wrong.

### 2. V1: what is directly demonstrated

- **Claim checked:** no accidental data-vector, fake-handling, iteration, or luminosity substitution in the coherent central-value comparison.
- **Evidence:** the script pins the central ROOT, data vector, release CSV, `LUMI=351000.0`, and `K=4` (lines 8--15). It reads the tagged RAPGAP response and prior plus central `h_data_bgsub` (lines 47--51), retains the release fake-row prescription `d=raw*(1-fake/(body-row-sum+fake))` (line 52), and changes only `(p_init,p_eff)` across variants (line 53). Validation records the expected data SHA match (validation lines 3--6), unchanged central ROOT checksum (lines 7--11 and 83--88), 490 output rows, all-finite, and zero-negative values (lines 79--88). The baseline gate against the release is `3.311839017904824e-15` (summary lines 3--13).
- **Physical relevance:** these records support fidelity of the nominal central-value comparison. They do not make the erroneous integrated attribution summaries valid.
- **Severity:** non-blocking for the displayed coherent-minus-baseline per-bin central values; blocking for adopting the attribution totals as recorded.

### 3. V2: `w_gen` resolution and the unresolved cross-tree condition

- **Claim checked:** `w_gen` is `w1*w2`.
- **Evidence:** the builder defines `w1` and `w2` from the `ChargedTotGen` batch and explicitly sets `w_gen = w1*w2` at builder lines 464--470. Ordinary prior filling uses that `w_gen` at lines 769--780. Miss filling also uses `w_gen` at lines 852--875.
- **Claim criticized:** this alone proves `body+miss == prior_gen_NWeff`.
- **Evidence:** it does not. The matched response weight is `w_reco=w1*w2*NucliaWeight` formed from the `ChargedTot` batch (builder lines 591--603) and used in the body (lines 818--826). The NWeff accumulator instead takes the `ChargedTotGen`-derived `w_gen` and multiplies it by `NW_partner` extracted from the `ChargedTot` matched row (lines 837--844 and 877--900). Thus equality requires, for every consumed matched key, `ChargedTotGen.weight1*weight2 == ChargedTot.weight1*weight2`. The reviewed source documents equality of inlined *kinematics* to the partner row (lines 827--830); it does not document equality of `weight1` or `weight2` across the two sources.
- **Physical relevance:** `body+miss` is exactly the stored response truth marginal; the study records unity for that tautological conservation test in every charge/bin (validation lines 21--26 and 52--57). But the builder calls NWeff the canonical IBU/TUnfold prior and states its geometric-efficiency intent (lines 314--322 and 1008--1021). Without the missing cross-tree weight equality, the evidence cannot establish that the proposed marginal equals the builder's canonical NWeff construction rather than merely supplying a different self-consistent prior.
- **Severity:** analysis-blocking for a replacement decision based on claimed agreement with builder design intent.

## Non-Blocking Checks Confirmed

- The central ROOT contains no NWeff candidate key: `prior_variants_comparison.json` lines 5--58 gives the complete inventory and `nweff_candidate_keys: []`; the file SHA, size, and mtime are unchanged before/after (lines 59--69). Therefore no direct central-file NWeff comparison was performed or can be claimed.
- The coherent-vs-baseline result is bin-resolved, not merely integrated: `comparison/coherent_vs_baseline.csv` includes charge/bin-indexed baseline, coherent, central, and relative-delta fields (header line 1), while `summary.json` documents 245-bin statistics separately for both charges (lines 14--69).
- The magnitude is recorded: h+ median/max `1.031368%/3.583227%`, `125/245` above 1%, integrated `+0.128046%`; h− `1.099047%/3.462532%`, `129/245`, integrated `+0.119937%` (`comparison/summary.json` lines 16--24 and 42--50). These are measurements of the study's `body+miss` variant, not validation that it equals NWeff.

## Missing Evidence

- A rebuilt response containing `h_prior_gen_{pos,neg}_NWeff` and an explicit per-bin equality check against `body+miss`, with the matched-source `weight1*weight2` condition independently recorded.
- A corrected attribution summary in which init-only and efficiency-only integrated shifts are calculated from their own output vectors. This is necessary for attribution reporting but is not the single decisive ambiguity for V3.
- Replacement-release provenance: a regenerated central release, its updated uncertainty/covariance artifacts, and a manifest. These are not supplied by a study explicitly labelled provisional and non-central (README lines 1--5).

## Recommended Next Check

Rebuild the RAPGAP response while writing NWeff, then produce a 490-bin machine-readable assertion that `body+miss` equals `h_prior_gen_NWeff` (including equality of the matched `weight1*weight2` factor across the linked `ChargedTot` and `ChargedTotGen` rows) before choosing either as the replacement IBU prior.
