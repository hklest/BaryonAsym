# WP-A2 gate G1′ verdict

Date 2026-10-27. Package WP-A2 of `.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`; child
plan `wp-a2_plan.md` (sha256 `c553d2007df95fbd78d386a96636328d6434cf21c1171b717dd9a67eb2225b0d`). `D` =
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.

**Ruling: KILL** (issued by `h1-analysis-critic`, 2026-10-27; see the last section).

## Pre-registered gate text (child plan D6, verbatim; frozen in S1)

- **D6 Gate G1′ (binding, pre-registered here; the parent gates section cites this block).** On the odd-split Stage-B N−1
  sample, stack C, signed-v3 weights (nd4cat sidecar for 7094-7097), after the frozen S3 correction set:
  (a) Plug N−1 unit-area max |MC/DATA − 1| ≤ 0.10 in bins with ≥ 20 data entries, and |f_fail,MC − f_fail,DATA| ≤ 0.02
  absolute in each D cell; (b) FMD and FTS N−1 < 0.25 for at least one admissible cocktail (non-negative weights within ±50%
  of the signed-v3 scales; the G1(b) scan grid); (c) the §7.1 fit, with the frozen template and range definition, on the
  odd-split cut-removed payload reaches template-statistics-aware toy p ≥ 0.05, coarse and fine, in every D cell and ALL,
  with ≥ 1000 toys, for at least one of 7071, 7091, nd4cat (7094 alone is excluded: it represents ep0607 only,
  `g1_verdict.md` ‡). Thresholds are G1's, unchanged, so a pass is comparable with the KILL. **Kill:** (a) or (c) fails once
  the pre-registered matrix is exhausted (every H1-H6 test run, every admissible correction applied), or when 5 Condor-days
  or 7 calendar days from the first WP-A2 Condor submission are spent, whichever comes first; Condor-days = Σ(job wall ×
  request_cpus)/24 h from the logs, tallied by `h1-naf-operator` in `D/data/restart_2026-10-27/wp-a2/condor_tally.csv`.
  (b) failing alone downgrades to a systematic leg. A KILL recommends close-out to the user; a PASS unblocks WP-E with the
  frozen correction set as the WP-B start values.

The thresholds are G1's, unchanged. They are identical to D6. The parent gates section (parent plan lines 156-164)
has the same numbers; it leaves out the ≥ 20-entry, template-statistics-aware and ±50% qualifiers and defers to D6
for them.

## Numbers behind the ruling (odd split, frozen S3 set applied)

| clause | limit | 7071 | 7091 | nd4cat |
|---|---|---|---|---|
| (a) Plug N−1 unit-area max \|MC/DATA−1\| | ≤ 0.10 | 0.982 | 0.965 | 0.953 |
| (a) Δf_fail D0 / D1 / D2 | \|·\| ≤ 0.02 | −0.210 / −0.217 / −0.224 | −0.214 / −0.225 / −0.238 | −0.210 / −0.218 / −0.230 |
| (b) best admissible joint FMD/FTS N−1 (1.95 M cocktails) | < 0.25 | 0.775 | 0.744 | 0.763 |
| (c) toy p, 8 fits (fine and coarse × D0, D1, D2, ALL), 1000 toys | ≥ 0.05 in all 8 | max 0.015; ALL fine and coarse at floor 0.0010 | max 0.013; ALL at floor | max 0.018; ALL at floor |
| (c) ALL fine / coarse D/ndf | | 3456/265, 1778/76 | 2767/265, 1827/76 | 2624/265, 1715/76 |

- DATA f_fail is 0.272 / 0.300 / 0.337 (D0 / D1 / D2, η_max < 3.3), against MC 0.058-0.113.
- Of the 24 fits, 17 sit at the toy floor 1/1001; the other seven, all single-cell, give 0.002-0.018.
- Source: `D/data/restart_2026-10-27/wp-a2/s4_g1p/g1prime_table.csv` (sha256 `611d5140…b4c3`) and `s4_g1prime_record.md`.
- (a) and (c) fail for every alternative, and every H1-H6 test has run. The kill condition is therefore met. (b)
  also fails.

## Hypotheses H1-H6 (derivation split, D3 criteria with readings R1-R8)

| hyp. | outcome | discriminant (criterion value vs threshold) | correction frozen |
|---|---|---|---|
| H5 Plug DB threshold/scale | not supported | EnCor = 1, Thresh = 0.3 GeV in every data and MC run (one IOV, 303906-511079); 1-7 GeV MC/DATA 0.855-0.903 (unit area, η_max > 3.5) | none |
| H1 η_max definition | supported (response branch) | excess / deficit move 0.91 / 4.23 (7071), 0.94 / 3.67 (7091) between 0.8 and 0.4 GeV (≥ 0.5 needed); Δη median shift 0.0004 (geometry needs ≥ 0.03) | N−1 η_max(0.8 GeV) < 3.3; fit on η_max(0.4 GeV) (edge residual 4070 vs 6153) |
| H6 beam tilt / z-vertex | not supported | geometry shift 0.0004 < 0.03; the 9012 reweight leaves the median at 0.0000 | none |
| H4 ND flow vs forward edge | supported (true-detector) | Plug-tagged edge sample: worst \|MC/DATA−1\| 0.71-0.73 (7071) and 0.89-1.03 (7091) ≥ 0.15, with the sign pattern in every D cell | fit range η_max < 3.4 plus merged [3.4, 5) |
| H3 fit structure | not supported | no variant raises p ≥ 10× in all 8 fits for any alternative (best variant (iii): max 0.47, ALL still near the floor); H4 true-detector holds (R3) | none |
| H2 overlay / forward noise | supported by its criterion; correction excluded | no run-order or luminosity-proxy trend (min block p 0.063); layer and station uniformity chi² p 1e-71 to 2.5e-7, with mixed-sign structure | none. Remedy-1 q fails D5 (`h1-analysis-critic`); systematic leg only, not run |

Sources: `s2_diagnostic_record.md`, `s3_correction_record.md`.

## Frozen correction set (WP-B start values on PASS; recorded here for close-out)

`D/data/restart_2026-10-27/wp-a2/s3_corr/s3_correction_record.json`, sha256
`4b2b5e269a1f8d07f22d34a24c9416742eba53ebb6da23b85014bcf3bf79d719`. It was frozen after the `h1-analysis-critic` round-2
sign-off, and both S4 submit logs quote it before any odd-split job ran.

- (1) The N−1 / Stage-B η_max cut uses `etaMaxClus08` < 3.3, the published 800 MeV / 3.3 of arXiv:1203.4495 §3.3.
- (2) The §7.1 fit uses `etaMaxClus04`.
- (3) The fit range is η < 3.4 plus one merged bin [3.4, 5).
- Remedy-1 noise removal is excluded. No `user:` item was applied.

## Where the failure sits (diagnosis)

- **Plug (G1′(a)).** The DB treatment is identical for data and MC. Two observations follow; this work does not
  show their common cause.
  - (i) In the η_max > 3.5 H5 sample, MC has about twice DATA's share of events with zero Plug energy and 10-15% too
    few with 1-7 GeV. This is an occupancy effect, not a scale effect (`h1-mc-expert`).
  - (ii) In the η_max < 3.3 N−1 sample, the ePlug > 7 GeV fail rate is 0.27-0.34 in DATA vs 0.06-0.11 in MC.
  - Neither is a DB-scale effect. A 10% Plug scale cannot close them, and remedy-1 removal is not admissible.
- **η_max fit (G1′(c)).** The LAr forward-edge response is the dominant definitional cause: the pattern depends on
  the threshold, is shared by RAPGAP and CDM, and appears in a Plug-tagged sample. Removing the edge from the fit
  lowers the deviance (even split, 7071: 3837/358 → 3137/265), but the fit stays at the floor. The 3.15-3.35 excess,
  and the 3.0-3.3 slope, which depends on the generator, stay inside the fit range.
- **FMD/FTS (G1′(b)).** The disagreements are mixed-sign and specific to layers and stations: unsimulated FMD layer-4
  inefficiency, dead FTS channels in ep0607 data, and an excess in the pre-toroid FMD layers 1-3 and FTS indices 0-3.

## Deviations from the pre-registration

None changes a threshold or the D6 text.

- S1: binding readings R1-R8 were added in the audit before any Condor job, because the critic found H4 and H2
  ambiguous. The plan file was not edited. The audit's sha256 (`bb95183d…bd23`) is quoted in the S2/S3/S4 submit
  logs.
- S1: the `gate_text_frozen` diff command was trimmed of the D7 line and the two headings (form only).
- S2: `h4_control_is_inclusive` failed (ND share 0.849 / 0.875 in D0 / D1 < 0.90). The H4 reading is kept: the test
  is not part of the D3 criterion, and the admixture can move the edge ratio by at most about 0.18.
- S2: the first analysis pass grouped MC by `RunNumber` per period. That is wrong for run-dependent MC that carries
  only its simulation period's runs, and the H2 and H5 per-period tables were fixed with a luminosity-share
  projection.
- S2: code review found that the luminosity ordering read an empty column, so the per-run A_pre count is used as the
  proxy, and that the layer chi² lacked MC errors. Both were fixed on the even split; no reading changed.
- S3: remedy-1 (D3 H2 "kept as the noise correction") was **excluded** by `h1-analysis-critic` on D5 (i)/(ii). This
  settles a conflict inside the plan between D3 H2 and the D5 kind list, and the `h1-mc-expert` dissent.
- S3 erratum (after the freeze): the frozen JSON still carries the first-pass H2 values (chi² p 1e-240 to 1e-10;
  min block p 0.046). The values quoted here (1e-71 to 2.5e-7 and 0.063) are the corrected ones, from code review
  C1/C2 on the even split. The JSON was not edited, so its hash stays valid. H2 has no correction in the frozen set.
- S3/S4: `n1_wpa2.py` gained `--eta-cut` (default 3.2; identity re-proved) to carry the H1 correction. S4 ran 17 n1
  jobs, not per-chain MC jobs, because no per-chain treatment was frozen.
- Plan S3 expected ≤ 16 fits. In practice there were 12 (variant, alternative) rows of 8 jobs each, 96 Condor jobs,
  plus 17 + 24 in S4.

## Budget

- Condor: 0.06585 Condor-days of 5 (S2 0.0215, S3 0.0316, S4 n1 0.0085, S4 fits 0.0043).
- Calendar: 0.377 days of 7 since the first WP-A2 submission (2026-10-27T04:30:47Z).
- Tally: `D/data/restart_2026-10-27/wp-a2/condor_tally.csv` (`h1-naf-operator`).

## Records (sha256 at verdict time)

- S1 `s1_definition_audit.md`: `bb95183d9a12e8baf49157e25e4c72f1df9a2c1372eb63053d35f09a6d34bd23`
- S2 `s2_diagnostic_record.md`: `c74c5b923d555db80c4df1447b0a2c467d4077feac930bd15f3405d7fab54edd`
- S3 `s3_correction_record.md`: `a29eb74d5cdaf657d98bbaa5a20eb637faae3e0a470d6616001a53039e12aa3f`, plus JSON `4b2b5e269a1f8d07f22d34a24c9416742eba53ebb6da23b85014bcf3bf79d719`
- S4 `s4_g1prime_record.md`: `32ba8749ad3ee91ac263f48af930ce45f3a09c77efb69fb9c03813e7271b7abb`; `g1prime_table.csv`: `611d5140fba1716587015c5d347b261f2b684022d05a72f54abdb4a9f0f0b4c3`

## Open items for the user (none applied)

- D6: a KILL recommends close-out of the diffractive restart.
- Open Q1 (abandon the Plug veto) is not formally triggered, because H5 is not supported. The Plug failure is an
  occupancy mismatch, which no admissible definition fixes.
- Open Q2 (new ND MC request) is not triggered, because H4 reads true-detector, not true-physics. The remaining edge
  and Plug-occupancy mismatches would need a forward-detector simulation or noise campaign. The user excluded that
  from this package (Non-goals).

## Ruling (`h1-analysis-critic`)

RULING: KILL. Clauses (a) and (c) fail by large margins (about 10× in shape and Δf_fail; every ALL fit at the toy
floor) for every admissible alternative (7071, 7091 and nd4cat). This holds after every H1-H6 test ran and every
admissible correction was applied, so the pre-registered D6 kill condition is met. (b) also fails and does not affect
the ruling. SIGNED: yes. The verdict's D6 block matches the plan word for word, and its numbers match
`g1prime_table.csv` and `g1_eval.json`. The frozen-record sha256 is quoted in both S4 submit logs, each S4 queue was
submitted once, and no `user:` item was applied.
