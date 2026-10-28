# DetintParentageDiag full-campaign aggregation — results record (2026-10-09)

**Scope.** Aggregation of the validated DetintParentageDiag NOT_TREEBUILDER campaign
(Condor cluster **5433299**, 865 units, all five validation gates PASS) into the study's
headline deliverables, per the design's aggregation spec
(`detint_parentage_study_design_2026-10-08.md` §F) and the 2026-10-09 task contract.
This record computes fractions and internal-diagnostic scaled yields only; it modifies no
central tree, response, or steering artifact. Machine-readable twin:
`H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/detint_parentage_results_2026-10-09.json`
(+ CSV twins `composition_per_stratum.csv`, `scaled_yields_per_category.csv`, `breakdowns.csv`).

**Population.** 1,136,839 selected det.int. rows (`selected_full == 1`: detint label +
admission + event fiducial + cleanup + track + track-class + xF cuts, **no** z-outlier cut),
out of 339,913,864 retained Tier-0 matched rows across 865 files. DJANGOH14 643,422 /
RAPGAP31 493,417. Per-file row-vs-counter cross-checks: zero anomalies campaign-wide.

**Provenance.**
- Campaign outputs: `H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/outputs/full_v6_4period_detintparentage_20261008/` (865 files).
- Validation (gates PASS): `.../detint_parentage_study_NOT_TREEBUILDER/validation/campaign_5433299_validation_report_2026-10-08.{json,md}`.
- Scaling (authoritative): `sidis_highQ2/data/normalization/detint_parentage_scaling_verification_2026-10-08.{json,md}` — L_data = 351000.0 nb⁻¹; L_MC RAPGAP31 9,132,891.0 / DJANGOH14 10,487,495.0 nb⁻¹; **sum-then-scale-once per generator across all four periods**; per-period scaling, the steer "Chunk luminosity" comments, 297657.8 and 356613.0 were never used.
- Aggregation execution: Condor single-unit. Cluster **5434388** = first full pass (superseded: its matched-block gen grid used lab `GenPt`; see §6 note). Cluster **5434612** = corrected production pass (all numbers below), 865 files, 1823.5 s, zero anomalies. Login-node smoke gate reproduced the design references exactly (122 selected; strict pass_z 121; categories {C2:76, C3:10, C5:36}). Script/checkpoints: `.../detint_parentage_study_NOT_TREEBUILDER/aggregation/detint_parentage_aggregation_20261009/`.

---

## 1. Headline: f_int = N(hadronic_interaction)/N(selected det.int. rows)

Unweighted binomial uncertainty σ = sqrt(f(1−f)/N). Weighted variants:
(a) w1·w2·clip(NucliaWeight,0,10) — response reco convention; (b) w1·w2 — nominal /
C_rad-proxy convention; weighted σ uses N_eff = (Σw)²/Σw².

| Sample | N_sel | f_int (unweighted) | f_int weighted (a) | f_int weighted (b) |
|---|---|---|---|---|
| DJANGOH14 | 643,422 | **0.353553 ± 0.000596** | 0.357392 ± 0.000709 | 0.358310 ± 0.000709 |
| RAPGAP31 | 493,417 | **0.333067 ± 0.000671** | 0.340087 ± 0.000808 | 0.340721 ± 0.000808 |
| Pooled | 1,136,839 | **0.344662 ± 0.000446** | 0.349663 ± 0.000533 | 0.350453 ± 0.000534 |

Generator-average of unweighted fractions: 0.343310. N_eff (product a): DJANGOH14
457,313; RAPGAP31 343,969 (≈71%/70% of N_sel — weight tails retained; see §7).

Per generator × period × charge (unweighted; full table in `composition_per_stratum.csv`
and JSON `slicings.gen_period_charge`):

| Stratum | n | f_int | | Stratum | n | f_int |
|---|---|---|---|---|---|---|
| dj\|em0405\|h+ | 98,462 | 0.3590±0.0015 | | rg\|em0405\|h+ | 86,495 | 0.3398±0.0016 |
| dj\|em0405\|h− | 86,187 | 0.3044±0.0016 | | rg\|em0405\|h− | 74,880 | 0.2812±0.0016 |
| dj\|em06\|h+ | 51,186 | 0.4057±0.0022 | | rg\|em06\|h+ | 45,065 | 0.3904±0.0023 |
| dj\|em06\|h− | 43,676 | 0.3486±0.0023 | | rg\|em06\|h− | 38,069 | 0.3243±0.0024 |
| dj\|ep0304\|h+ | 57,277 | 0.3710±0.0020 | | rg\|ep0304\|h+ | 49,842 | 0.3514±0.0021 |
| dj\|ep0304\|h− | 50,278 | 0.3183±0.0021 | | rg\|ep0304\|h− | 43,425 | 0.2917±0.0022 |
| dj\|ep0607\|h+ | 138,410 | 0.3905±0.0013 | | rg\|ep0607\|h+ | 84,240 | 0.3757±0.0017 |
| dj\|ep0607\|h− | 117,946 | 0.3273±0.0014 | | rg\|ep0607\|h− | 71,401 | 0.3100±0.0017 |

Regularities (ESTABLISHED as measured MC facts): f_int(hplus) exceeds f_int(hminus) by
5–7 pp in **every** generator×period stratum (e.g. dj|ep0607: 0.3905 vs 0.3273), and
DJANGOH14 exceeds RAPGAP31 by ~2 pp at fixed charge. Microscopic interpretation
(GHEISHA charge-dependent hadron–nucleus cross sections vs feed-down charge population)
is PROVISIONAL and not needed for any use of these numbers.

## 2. Full 7-category composition (same slicing; overall and per generator)

| Category | DJANGOH14 | RAPGAP31 | Pooled |
|---|---|---|---|
| C0 v0_ancestor | 0 (0.00%) | 0 (0.00%) | 0 (0.00%) |
| C1 dalitz_ancestor | 0 (0.00%) | 0 (0.00%) | 0 (0.00%) |
| C2 hyperon_decay | 359,728 (55.91%) | 282,880 (57.33%) | 642,608 (56.53%) |
| C3 k_parented | 56,229 (8.74%) | 46,177 (9.36%) | 102,406 (9.01%) |
| C4 em_initiated | 0 (0.00%) | 0 (0.00%) | 0 (0.00%) |
| C5 hadronic_interaction | 227,545 (35.36%) | 164,280 (33.31%) | 391,825 (34.47%) |
| C6 unresolved | 0 (0.00%) | 0 (0.00%) | 0 (0.00%) |

Class groupings (pooled): **decay-born (C1+C2) 56.53%; interaction-born (C4+C5) 34.47%;
k_parented (C3) 9.01%; unresolved (C6) 0.00%** — fractions sum to 1 exactly in every
stratum. k_parented (K-DIF/interaction mixed, never split per design §E) and unresolved
are reported separately, never forced into either class.

The four zero categories are physically consistent, not classification failures:
C0 = 0 is the design gate (veto-walk V0/γ/π⁰ ancestry excluded by the det.int. label);
C1 = 0 because π⁰/η/Jψ mothers decay essentially at the primary vertex, so their charged
daughters are primary-vertex tracks, not det.int. tracks (the audit's 6 η-veto-ancestor
rows carry nearer hyperon mothers and sit in C2); C4 = 0 because GEANT3/GHEISHA books
photonuclear secondaries under hadron/baryon mothers, which the C5 rule (with the
baryon-resonance fallback) captures; C6 = 0 shows the first-informative-ancestor walk
(6 levels + resonance fallback) resolved every campaign row.

## 3. Scaled yields at L = 351000.0 nb⁻¹ (per category per generator)

Sum-then-scale-once per generator (scale = 351000/L_MC; never per-period).

| Category | DJ Y_a | DJ Y_b | RG Y_a | RG Y_b |
|---|---|---|---|---|
| hyperon_decay | 15,658.14 | 15,847.38 | 14,757.45 | 14,947.49 |
| k_parented | 2,451.60 | 2,495.46 | 2,479.54 | 2,523.87 |
| hadronic_interaction | 10,071.88 | 10,242.38 | 8,883.10 | 9,029.34 |
| **total selected** | **28,181.62** | **28,585.22** | **26,120.10** | **26,500.70** |

Y_a = Σw1·w2·clip(NW,0,10) × scale (response reco convention); Y_b = Σw1·w2 × scale
(nominal / C_rad-proxy convention). Two-generator averages: Y_a 27,150.86; Y_b 27,542.96.
The (a)/(b) ratio is 0.98577, matching the alpha0p2 partition's response/nominal sumw
ratio 0.98582 — the mean NucliaWeight of det.int. rows is ≈0.986, and the clip at 10 is
inert campaign-wide (max NW = 1.0, zero rows clipped).

**PROVISIONAL INTERNAL DIAGNOSTIC** (per the normalization record's caveat): absolute
scaled yields of this diagnostic sample are not publication quantities; ratios and
fractions below are the robust content.

## 4. Comparison against the alpha0p2 det.int. template 27405.8031

**Weight-convention finding (evidence-based correction to the task premise).** The
template 27405.8031 is the **nominal (w1·w2)** convention: it equals the two-generator
average of the alpha0p2 partition's per-generator `yield_L351000_nominal` valid-denominator
yields, (28,432.25674 + 26,379.40361)/2 = 27,405.830 (agreement to 1×10⁻⁶ relative;
residual = per-bin vs global summation path), and the partition's own normalization note
states response-weight sums are audit-only. The exact like-for-like against the template
is therefore the **(b)** product, not (a).

| Quantity (this study) | Value | Ratio vs 27405.8031 |
|---|---|---|
| Y_b, all selected (central, no-cone, no-z) | 27,542.96 | **+0.50%** |
| Y_b, valid analog (sel & pass_z & in-reco-bin) | 27,397.91 | **−0.029%** |
| Y_a, all selected | 27,150.86 | −0.93% |
| per-generator valid Y_b: DJ / RG | 28,423.79 / 26,372.02 | −0.030% / −0.028% vs partition refs |

**Cone interpretation (the previously UNBOUNDED no-cone det.int. magnitude, now bounded).**
The template is alpha0p2-cone-conditional; this diagnostic is central no-cone. The naive
expectation from the electron analog (cone survival 0.9241/0.9270, i.e. ~7.5% removed)
would be a no-cone excess of ~8%. **Measured: the det.int. cone survival is ≈0.9984** —
only 1,772 of 1,136,839 selected rows (0.156%) fall inside the 0.2-rad scattered-electron
cone (994 DJ / 778 RG; zero α sentinels). The full central-vs-template excess decomposes
as: z-outlier cut 0.280% + out-of-reco-bin 0.171% (together 0.527% selected→valid) and a
−0.029% residual on the valid analog, which is the Tier-0-only scope of this study vs the
tree-side Tier-2-inclusive partition (valid counts: 640,313/491,389 here vs 640,325/491,388
partition — a 12-row net difference, 1×10⁻⁵). Unlike fake electrons, det.int. tracks are
broadly distributed in angle and nearly inert under the cone. **Conclusion: the no-cone
det.int. magnitude exceeds the alpha0p2-cone-conditional template by only +0.50% all-in
(+0.16% from the cone alone), so cone-conditional background statements about det.int. are
robust at the sub-percent level.** ESTABLISHED.

## 5. Central-selection matched/fake partition re-derivation (86/18 analog)

Matched block mbB = gen event cuts (Q2_es_gen∈(150,20000), Y_es_gen∈(0.1,0.7), GenEmpz>45,
xFGen<0) & validGenElec & pool species & finite gen sanity & in-gen-grid — **the shipped
v4 response convention (no GenPt>0.15 cut)**, grid = zp_local_gen(zGen, pTh_cm_gen) with
pTh_cm_gen recomputed offline by the verbatim-ported response `_compute_pTh_cm_and_z`
(mirror of `partition_worker.py:193-203`). Denominator: valid analog (sel & pass_z &
in-response-reco-bins), sumw fractions (w1·w2), matching the partition's convention.

| | central (this study) | alpha0p2 reference |
|---|---|---|
| DJANGOH14 matched/fake | **86.08 / 13.92** | 85.86 / 14.14 |
| RAPGAP31 matched/fake | **86.77 / 13.23** | 86.57 / 13.43 |
| Pooled matched/fake | **86.39 / 13.61** | 86.18 / 13.82 |

Agreement at the +0.20–0.22 pp level, with the offset fully consistent with the cone
conditioning (central includes the 0.156% in-cone rows, which are matched-heavy, and
excludes tree-side Tier-2 rows). Count-based matched fractions (89.81%/90.65% here vs
89.95% partition combined) show the same ~4 pp count-vs-sumw gap on both sides — fakes
carry higher average weights. Fake-reason mix (fraction of fake sumw): gen_event_Q2y_fail
92.4%/92.2%, gen_empz_fail 4.9%/5.2%, gen_xF_fail 2.5%/2.4%, gen_bin_outside
0.26%/0.28% (DJ/RG) — the partition's mix (92.3/5.0/2.4/0.27 combined). The bin_outside
reason closes **bit-level**: n = 267/266 (DJ) and 211/211 (RG); sumw = 306.07/305.07 and
261.52/261.52 — the offline pTh_cm_gen recomputation is exact.

Design-convention variant mbA (matched_block_ingredients incl. GenPt>0.15, same grid):
84.73%/85.38% — the GenPt>0.15 symmetrization removes ≈1.3–1.4 pp of matched sumw. Both
conventions are in the JSON. The 86/14 structure is confirmed under central selection;
the design's instruction stands (det.int. is in-observable, subtracted once inside the
response-conditional treatment; external det.int. subtraction remains FORBIDDEN per the
2026-10-07 ledger).

## 6. Per-(Q2,x)-cell and gen-(z,pT) breakdowns of f_int

Recorded kinematics permit both (reco cell from Q2es/Xes with the response decode; gen
grid zGen × pTh_cm_gen, the response's gen binning). Pooled over charges/generators:

Per reco (Q2,x) cell (all N≫50; the 7 cells contain ≥99.9998% of selected rows):

| Cell | n | f_int |
|---|---|---|
| S0_x0 | 337,848 | 0.2518 ± 0.0007 |
| S1_x0 | 233,152 | 0.2703 ± 0.0009 |
| S0_x1 | 174,709 | 0.3450 ± 0.0011 |
| S1_x1 | 144,344 | 0.4222 ± 0.0013 |
| S2_x0 | 76,200 | 0.3099 ± 0.0017 |
| S2_x1 | 78,005 | 0.5661 ± 0.0018 |
| S3_x0 | 92,579 | 0.5912 ± 0.0016 |

f_int rises monotonically with Q2/x — a factor 2.3 from S0_x0 (0.252) to the merged S3
cell (0.591). The det.int. pool becomes progressively more interaction-born at higher
Q2. ESTABLISHED as measured; interpretation PROVISIONAL.

Per coarse gen-(z,pT) (35-bin grid; 36 populated incl. outside; full per-(gen,charge)
tables and the 245-bin gen cell×zp grid in JSON `breakdowns`): the population is heavily
low-z (61% of rows in z∈[0,0.05), pT<0.5 GeV); f_int is ≈0.34–0.40 at z<0.10, dips to
≈0.15–0.25 at z∈[0.20,0.30), and only 1,052 rows (0.09%) fall outside the gen grid.

## 7. QA

- **Weight tails / N_eff**: weight1 on retained rows reaches 149.94 (DJ) / 149.98 (RG)
  campaign-wide (the 103.2 quoted in the normalization record was the smoke-file value);
  on selected rows 107.54 / 81.49. This is the same `weight1` branch as central v6,
  bit-identical by the parity gates — not a defect. N_eff (product a): 457,313 / 343,969.
- **NucliaWeight**: max 1.0 campaign-wide on selected rows; zero rows clipped at 10 —
  the response clip convention is inert here.
- **Strict pass_z vs selected_full**: 1,133,649 vs 1,136,839 (−3,190, −0.28%; DJ −1,847,
  RG −1,343). The design smoke precedent (121 vs 122) is the same one-sided z-outlier
  removal at file scale. Headline uses selected_full (audit C.3 convention); the strict
  variant is a documented QA companion, not an inconsistency.
- **C0 v0_ancestor = 0** campaign-wide (row-level; matches the per-file counter
  `n_v0_ancestor_in_detint` = 0 and the validation gate). **Pass-2 rows in selected = 0**;
  α sentinels = 0; per-file row-vs-counter mismatches = 0 (865/865).
- **Tier-2 context**: `n_no_tier0_link_selected` totals 376,204 (Tier-2 greedy-fallback
  candidates, never rowed, excluded by construction). Aggregate tree-side consistency:
  this study's selected counts (643,422/493,417, central no-cone no-z Tier-0) vs the
  alpha0p2 tree-side prebin (641,624/492,104, cone+z, Tier-2-inclusive) differ by
  +0.28%/+0.27% — exactly the cone+z removal plus a ~79-row Tier-2 residual (1×10⁻⁴
  relative, as designed).
- **Execution route**: single-unit Condor (multi-file policy; per-file cost ~1–2 s,
  total ~30 min) with 25-file checkpoint snapshots (resumable). Cluster 5434612
  (production; 5434388 was the superseded first pass whose matched-block gen grid used
  lab GenPt instead of the response's CM-frame pTh_cm_gen — corrected after the
  bin_outside cross-check against `partition_worker.py:193-203`; headline, composition
  and yields are grid-independent and identical in both passes).

## 8. Status labels

- **ESTABLISHED** (validated campaign, statistics-limited): all f_int values and
  uncertainties; the 7-category composition and class split; the zero-category
  statements; the cone-survival bound (§4) and template ratios; the central 86/14
  partition re-derivation and its bin_outside bit-closure; the breakdown tables; all QA
  closure statements.
- **PROVISIONAL INTERNAL DIAGNOSTIC**: absolute scaled yields (§3) per the normalization
  caveat; microscopic interpretations of the charge/Q2 dependences; the C1/C4 zero-category
  mechanism explanations (consistent with GEANT3/GHEISHA bookkeeping but not independently
  re-derived from GHEISHA source).

## 9. Files

- Results JSON: `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/detint_parentage_results_2026-10-09.json`
- CSV twins: `composition_per_stratum.csv`, `scaled_yields_per_category.csv`, `breakdowns.csv` (same directory)
- Aggregation script + wrapper + .sub + checkpoints + Condor logs:
  `H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/aggregation/detint_parentage_aggregation_20261009/`
- This record: `sidis_highQ2/docs/backgrounds/detint_parentage_results_2026-10-09.md`
