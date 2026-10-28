# Det.int. mother-species re-slice and per-bin contamination map (refined definition) — 2026-10-09

**Analysis:** sidis_highQ2 v6/z005 inclusive charged-hadron SIDIS (Q2 > 150 GeV^2).
**Scope:** MC-truth extension of the completed det.int. parentage study
(`detint_parentage_results_2026-10-09`). No central product was modified; all
outputs are study products under `output_v6/studies/detint_parentage_2026-10-08/`.
**Supersedes:** the "decay-born stays signal" framing of the 2026-10-09 parentage
results, per the refined user definition below.

## 1. Refined definition and user rulings (authoritative for this study)

- **SIGNAL:** charged pi/K/p lineage from the primary interaction. Feed-down
  stays signal **only if the direct mother is a charged pi/K/p** (e.g. K -> pi
  decay products stay signal).
- **REMOVABLE:** (1) interaction-born GEANT secondaries (classes C4+C5);
  (2) decay-born feed-down (C1+C2) whose direct mother is **not** a charged
  pi/K/p; (3) k_parented (C3) — pending ruling, reported both ways everywhere.
- **Neutral-K ruling (2026-10-09, applied post-campaign):** K0S/K0L/K0-mother
  feed-down is **not** covered by the "kaons which decay are still signal"
  rule. It stays in the **V0 background class** exactly as the standing
  convention (flag species 310/130/311/3122 unchanged). The signal-mother rule
  covers charged pi/K/p only. K0S/K0L/K0-mother rows are reported as separate
  sub-lines within the V0-class context: inside the det.int. block they can
  only appear as unmatched-V0 leakage, i.e. they are **not part of the
  det.int. decay-born residual by construction**. Measured: **0 rows
  campaign-wide** (both generators) — the `IsMatchedV0Daughter` exclusion is
  leak-free at campaign scale, so the ruling flip is numerically inert.
- **k_parented definitional note:** under the signal-mother rule read
  literally, C3 rows (100% charged-K mothers) would fall on the signal side;
  the task nonetheless keeps C3 pending ruling, so both ways are reported.
  The C3 composition in Sec. 3.2 is the decision input.

## 2. Execution, inputs, and gates

- **Campaign inputs (unchanged, no new production):** the 865
  DetintParentageDiag outputs of cluster 5433299
  (`H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/outputs/full_v6_4period_detintparentage_20261008/`),
  per-row branches `mother_pdg`, `ancestor1..6_pdg`, `mother_is_gtr/str`,
  `parentage_category`, reco kinematics and weights.
- **Execution route:** Condor single unit **cluster 5435475** (accumulate over
  865 files, 1004.8 s, **zero anomalies**, resumable `agg2_state` checkpoints
  every 25 files) + finalize rerun from checkpoint on the NAF login node after
  fixing two finalize-only defects (a dict-comprehension crash; a worst-bin
  sort sign error that had ranked data-zero bins first — both fixed, products
  regenerated; the accumulate was not rerun).
- **Script:** `H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/aggregation/detint_parentage_aggregation_20261009/aggregate_detint_motherspecies_perbin.py`
  (smoke gate: unit `djangoh14_7091_ep0607_0200`, 122 selected, categories
  {C2:76, C3:10, C5:36}, per-bin closure in-grid+overflow = 122, decay split
  closure = 76 — PASS before submission).
- **Gates (all PASS):** 865/865 units; per-generator n_sel and category counts
  bit-identical to the delivered aggregation (DJ 643,422 / RG 493,417; pooled
  1,136,839); per-category sumw_b vs the previous results JSON max rel diff
  **0.0**; scanned==consumed everywhere; data-vector sums exactly
  3,884,097 / h+ 1,975,301 / h- 1,908,796; external vector sums reproduce the
  ledger template totals (electron 148,976.435, V0 132,065.277, lowQ2
  208.3795, det.int. alpha0p2 27,405.803).
- **Normalization (authoritative, unchanged):** sum-then-scale-once per
  generator, L_data = 351,000 nb^-1, L_MC RAPGAP31 9,132,891.0 /
  DJANGOH14 10,487,495.0 nb^-1; weight products w_b = w1*w2 (nominal,
  headline) and w_a = w1*w2*clip(NucliaWeight,0,10) (response convention).
  Selected population: `selected_full` (no pass_z cut), identical to the
  parentage-study headline population.

## 3. Deliverable A — mother-species re-slice

### 3.1 Decay-born (C1+C2 = 642,608 rows pooled) mother species

| Mother species (pooled) | n | fraction of decay-born |
|---|---|---|
| Sigma+ (3222) | 181,324 | 0.28208 |
| Sigma+bar | 170,558 | 0.26544 |
| Sigma- (3112) | 103,755 | 0.16144 |
| Sigma-bar | 102,211 | 0.15904 |
| Xi- (3312) | 40,627 | 0.06322 |
| Xi-bar | 39,866 | 0.06204 |
| Omega-bar (3334) | 1,888 | 0.00294 |
| Omega- | 1,792 | 0.00279 |
| Xi0-bar (3322) | 335 | 0.00052 |
| Xi0 | 252 | 0.00039 |
| **charged pi/K/p mothers** | **0** | **0.0** |
| **neutral-K mothers (K0S/K0L/K0)** | **0** | **0.0** |
| gamma / other / none | 0 | 0.0 |

- **Decay-born is 100.000% non-charged-pi/K/p-mother** in both generators
  (DJ 359,710 / RG 282,898 rows, all hyperon mothers; weighted fractions w_b
  and w_a identical: hyperon = 1.0). Per-generator top-mother ordering matches
  the pooled table (Sigma+ > Sigma+bar > Sigma- > Sigma-bar > Xi- > Xi-bar).
- **No Lambda(3122) mothers anywhere** (0 rows). Mechanism (verified against
  the class construction): Lambda -> p pi and K0S -> pi pi are the V0
  channels; their daughters are V0-daughter-matched and thereby excluded from
  the det.int. class (`IsMatchedV0Daughter==0` is part of the label). The
  det.int. decay-born residual is therefore exclusively the non-V0-topology
  hyperon decays: Sigma+- -> N pi (single prong), Xi -> Lambda pi direct
  daughters, Omega -> Lambda K.
- **K0 sub-line (user ruling):** K0S/K0L/K0-mother rows = **0** campaign-wide.
  They belong to the V0 background class (flag species 310/130/311/3122
  unchanged); by construction they are not part of the det.int. decay-born
  residual, and the measurement confirms zero leakage.

### 3.2 k_parented (C3 = 102,406 rows pooled) composition

- **Mother species: 100% charged kaons** (K- 51,525 + K+ 50,881 pooled;
  DJ 28,278/27,950, RG 23,247/22,931). No other mother species.
- **Mother record split (ruling input):** mother_is_gtr = 102,121 (**99.72%**;
  DJ 56,065 / RG 46,056) — the parent K is the generator (GTR) record, i.e.
  **primary-K decay-in-flight**; mother_is_str = 285 (0.28%) —
  interaction-produced K decays. The decay-vs-interaction split of the
  daughter within the GTR-K majority is not recorded (the design's
  "never split" decision); the campaign trees do carry the handles for a
  future split — `mother_svx_x/y/z`, `mother_svx_ivtype`, and the
  `mother_svx_itrhis`/`mother_str_itrhis` GEANT history codes (campaign
  script `targeted_detint_parentage_diagnostic_NOT_TREEBUILDER.C`, node
  macro `NB` at line 86, invoked as `NB("mother",...)` at line 318).
- **Ancestry of the parent K (anc1, pooled):** string(92) 41,931; K*0 10,597;
  K*0bar 10,713; K*+ 5,526; K*+bar 5,562; D0(421) 4,920 — i.e. the parent K's
  are primary fragmentation products (incl. K* -> K pi and charm D0 -> K pi
  feed-down), consistent with the GTR-mother majority.

### 3.3 Refined-definition removable fraction of the det.int. block

| Scope | removable (k_parented kept) | removable (+ k_parented) |
|---|---|---|
| **Pooled** | **0.909920 +- 0.000269** (w_b 0.909155 +- 0.000321, N_eff 799,712; w_a 0.909463 +- 0.000321) | **1.000000** (w_b 1.0, w_a 1.0) |
| DJANGOH14 | 0.912611 (w_b 0.912701 +- 0.000418, N_eff 456,868) | 1.000000 |
| RAPGAP31 | 0.906412 (w_b 0.904762 +- 0.000501, N_eff 343,820) | 1.000000 |
| Generator average | 0.909511 unweighted (w_b 0.908732) | 1.000000 |

- Numerator (pooled, kpar kept): 1,034,433 = 391,825 interaction-born
  (C4+C5) + 642,608 non-charged-pi/K/p-mother decay-born; the remaining
  102,406 rows are C3. With C3 also removed the fraction is exactly 1 because
  the signal-side (charged pi/K/p-mother decay-born) is **0 rows** and
  unresolved (C6) is 0.
- **K0-flip variant:** identical numbers (0 neutral-K rows).
- **Reading:** under the refined definition the det.int. block is ~91%
  removable with k_parented kept, 100% with k_parented removed. This does not
  change the central treatment (the det.int. label is in-observable per the
  2026-10-07 ledger); it quantifies what the block contains under the refined
  truth-side definition.

## 4. Deliverable B — per-bin contamination maps

### 4.1 Binning, mapping, denominators (stated)

- **Binning source:** canonical `binning_v3.py` from
  `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/`,
  **verified equal to the central release `bin_map.json`**
  (`binmap_v3_inverted`, release `bbb_ibu_coherentprior_..._2026-10-06`) on
  2026-10-09: Z edges (reco=gen) [0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0];
  pT edges reco [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0, 6.0, 8.0], gen
  [0, 0.5, 1.0, 2.0, 4.0, 8.0]; Q2 strata S0 150-300, S1 300-700, S2 700-1500,
  S3 1500-20000 GeV^2; x edges per stratum identical reco/gen; 7 cells
  (S0_x0..S3_x0). Index convention (release): `i_reco = cell*70 + z_idx*10 +
  pT_idx` per charge; **980 charge-major: bins 0-489 h+, 490-979 h-**
  (data-vector `charge_index_layout`). The same `binning_v3.py` is the one
  imported by the BSA scripts (`scripts/bsa/build_bsa_araw.py`).
- **Row mapping:** `cell_index_reco(Q2es, Xes)` + `zp_local_reco(z_response_pion,
  pT_response_pion)` + `flat_reco` — the same functions the response uses
  (response CM-frame reco variables). Rows outside the grid go to an explicit
  overflow counter, never dropped: **5,137 rows (0.4519%)** — 2 cell-outside,
  5,135 zp-outside (z>1/non-finite or pT>=8), DJ 3,109 / RG 2,028.
- **Denominator:** central raw data vector per-bin counts
  (`data_vector_...json`, `per_bin.n_subtracted`, schema
  `data_vector_v4_native_fine_reco_raw_no_bgsub`): sum 3,884,097; h+
  1,975,301 / h- 1,908,796. 14 bins have zero data (fractions reported None).
- **Numerators:** per-generator scaled-MC expected events (sum-then-scale-once,
  w_b) per component per bin; the headline maps use the two-generator average
  (the central convention), per-generator maps also delivered.
- **Assumption (stated):** MC per-bin shape as proxy for data per bin — the
  same assumption the central background vectors make. Det.int. columns are
  central-selection (no alpha cone) scaled MC; the external e/V0/lowQ2 columns
  are the published alpha0p2-cone-conditional expected-event vectors at
  L351000 from the uniform package
  (`reco_backgrounds_L351000_finalcandidate_20260626/`). Classes are disjoint
  by construction (det.int. excludes electron-matched and V0-daughter-matched
  rows; lowQ2 is a source-distinct DJANGOH sample). The selection-convention
  mismatch (cone) affects ~0.16% of det.int. rows (cone survival 0.9984,
  parentage study).

### 4.2 Map statistics (two-generator average; per-generator in JSON/CSV)

| Map | Worst bin | fraction | bins >= 0.5% | >= 1% | >= 2% |
|---|---|---|---|---|---|
| det.int. total | 481 (h+, S3_x0, z[0.70,1.00], pT[0.25,0.50]) | 5.2685% | 230 | 75 | 8 |
| interaction-born | 975 (h-, S3_x0, z[0.70,1.00], pT[1.50,2.00]) | 1.7975% | 66 | 19 | 0 |
| removable decay-born (non-charged-pi/K/p mother) | 481 (h+, S3_x0, z[0.70,1.00], pT[0.25,0.50]) | 5.2685% | 71 | 10 | 4 |
| k_parented | 412 (h+, S2_x1, z[0.70,1.00], pT[0.50,0.75]) | 0.6405% | 2 | 0 | 0 |
| combined removable (kpar kept) | 481 (h+, S3_x0, z[0.70,1.00], pT[0.25,0.50]) | 5.2685% | 207 | 65 | 5 |
| combined removable (+kpar) | 481 | 5.2685% | 230 | 75 | 8 |

- The worst det.int. bin (481) holds a single data track (sparse high-z bin);
  per-generator worst is the same bin (DJ 6.694%, RG 3.843%). The det.int.
  contamination is a high-z (target fragmentation) phenomenon throughout.
- **Per-(Q2,x)-cell aggregates** (average maps, both charges pooled),
  removable (kpar kept) / (+kpar):
  S0_x0 0.611%/0.673%; S0_x1 0.596%/0.657%; S1_x0 0.592%/0.650%;
  S1_x1 0.685%/0.756%; S2_x0 0.605%/0.661%; S2_x1 0.950%/1.047%;
  S3_x0 1.027%/1.125%. (With +kpar the cell yields equal the det.int.-total
  cell yields exactly, as expected from the 100% removable-with-kpar result.)

### 4.3 Cross-reference with external vectors (B.7)

Integrated (expected events at L351000 over the raw data vector):

| Component | keepK-way | +kpar way |
|---|---|---|
| det.int. refined-removable (this study, central selection, in-grid) | 24,903.39 | 27,397.91 |
| residual electrons (alpha0p2 avg) | 148,976.44 | 148,976.44 |
| V0 daughters (alpha0p2 avg) | 132,065.28 | 132,065.28 |
| lowQ2 (DJANGOH) | 208.38 | 208.38 |
| **total removable** | **306,153.48 (7.8822% of data)** | **308,648.0 (7.9465%)** |

- The +kpar total (308,648.0) reproduces the alpha0p2 ledger all-component
  total (308,655.89) to 0.003% — the difference is exactly the det.int.
  central-vs-alpha0p2 selection/cone difference. Strong end-to-end consistency.
- **Combined-map worst bins:** 839 (h-, S2_x0, z[0.70,1.00], pT[6.00,8.00])
  at **48.14%** (electron-dominated: 45.21% electron + 2.09% V0 + 0.84%
  det.int. removable; 4 data tracks); then 68 (h+, S0_x0, same z/pT) 34.73%;
  349 (h+, S2_x0) 29.99%; 559 (h-, S0_x0) 29.17%; 699 (h-, S1_x0) 26.51%.
  Bins >= 1%: 947 (keepK) / 948 (+kpar); >= 2%: 820 / 824. These large
  per-bin fractions are driven by the **external** components in sparse
  high-z/high-pT bins, not by det.int.
- det.int.-only in-grid yield 27,397.91 vs alpha0p2 det.int. template
  27,405.80 (ratio 0.9997, consistent with the parentage study's valid-analog
  comparison); per-bin shape agreement over the 953 bins with template > 0:
  ratio median 1.0000, p05 0.933, p95 1.006.

## 5. Deliverable C — gen-side feasibility (no production; spot-verified)

**Verdict: the generator-side (GTR) feed-down composition of the response
TRUTH content is quantifiable from existing artifacts — no new production
needed; one light aggregation pass over the existing v6 trees suffices.**

Evidence:
- The v6 `ChargedTotGen` trees already record, per gen-level pi/K/p row:
  `GenMotherPDG` (direct GTR mother, builder
  `ChargedSidisTreeBuilderHighQ2_v6.C` line 817, filled at lines 1421-1423
  from `gtrdata[gp->GetMother1()]->GetPDG()`), `ParticleSource`
  (classifySourceGTR, line 1435), `VetoAncestorPDG`, `IsConvOrDalitzGen`,
  `IsK0LDaughter`, `IsDIFMuon`. The gen loop is GTR-only (V5E `IsGtr()` filter)
  with the second-mother veto, so the truth pool is pure generator content.
- **Spot check (one chunk per generator, login node):** DJANGOH14 run 7091
  chunk 0000 (1,931,496 rows) and RAPGAP31 run 7071 chunk 0000 (1,936,073
  rows): truth-pool mother composition DJ/RG = string/resonance mothers
  ("other": rho, omega, K*, eta, Delta, D0, string-92) 94.38%/95.03%;
  no-mother-record 4.42%/4.49%; **hyperon mothers 0.48%/0.46%**; charged
  pi/K/p mothers 0.72%/0.02%.
- **The hyperon component is exclusively strong decays of excited hyperons**
  (Sigma*(3224/3114/3214) and Xi*(3324/3314) mothers, daughters 100% pi+-;
  ParticleSource flags them PS=1 (Sigma lineage) / PS=2 (Xi lineage), and
  PS1+PS2 counts equal the direct-hyperon-mother counts exactly).
  **Lambda(3122)- and Xi(3312/3322)-mother rows are exactly 0** in both
  generators: Lambda/K0S (and per the det.int. campaign, Xi, Sigma+-, Omega)
  are generator-stable and decay only in GEANT — i.e. their daughters are the
  reco-side det.int. block mapped by this study. The GTR-side weak-decay
  hyperon feed-down of the truth content is therefore **0 by construction**
  for Lambda; the only truth-side hyperon content is the ~0.5% prompt
  Sigma*/Xi* -> pi component.
- **Cost of full quantification:** one light pass over the existing 865 v6
  tree units reading ChargedTotGen only (same manifest, same order of cost as
  the det.int. campaign, ~20-40 min single Condor unit), mirroring the
  response gen selection and gen binning. **Definitional caveat for the
  user:** at truth level, "prompt" conventionally includes primary
  fragmentation resonance decays (rho/omega/K*/D0 -> pi/K, ~94% of the pool
  by direct-mother bookkeeping); the refined definition's "non-pi/K/p-mother
  removable" clause, if applied literally at truth level to resonance
  mothers, would remove nearly the whole truth pool. The truth-level
  extension should scope "feed-down" to displaced weak decays (hyperons,
  K0S), where the GTR-side removable component is ~0.5% (Sigma*/Xi*) with
  Lambda = 0 exactly.

## 6. QA summary

- Per-file row-vs-counter mismatches: 0; scanned==consumed in all 865 units;
  C0 v0-ancestor rows: 0; unresolved C6: 0 (campaign-wide, unchanged).
- n_sel / category counts / per-category sumw_b **bit-identical** to the
  delivered 2026-10-09 aggregation (max rel diff 0.0) — this pass is a pure
  re-slice of the same rows.
- Overflow explicit: 5,137 rows (0.4519%), never dropped; per-component
  overflow in the JSON.
- 14 data-zero bins (fraction None); external-vector layout verified
  (yield_980[:490] == yield_hplus per file); external sums reproduce the
  ledger totals exactly.
- Absolute scaled yields remain PROVISIONAL INTERNAL DIAGNOSTIC (same label
  as the parentage study); all fractions, compositions, maps, and gate
  outcomes are statistics-limited ESTABLISHED.

## 7. Products

- `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/detint_motherspecies_perbin_results_2026-10-09.json`
- `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/perbin_contamination_motherspecies_2026-10-09.csv`
  (980 rows = per-charge 490 published bins x 2 charges, charge-major; 59
  columns: bin index + Q2/x/z/pT edges + data count; per-component RG/DJ/avg
  yields and avg fractions for detint_total, interaction, decay_piKp,
  decay_K0, decay_nonpiKp, kparented, removable_keepK, removable_rmK; external
  e/V0/lowQ2 yields and fractions; alpha0p2 det.int. reference; total-removable
  yields and fractions both ways — plotting-ready)
- `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/motherspecies_decayborn_2026-10-09.csv`
- `H1ChargedSidis_HighQ2/output_v6/studies/detint_parentage_2026-10-08/kparented_composition_2026-10-09.csv`
- This record: `sidis_highQ2/docs/backgrounds/detint_motherspecies_perbin_results_2026-10-09.md`
- Aggregation script + checkpoints + Condor logs (cluster 5435475):
  `H1ChargedSidis_HighQ2/diagnostics/detint_parentage_study_NOT_TREEBUILDER/aggregation/detint_parentage_aggregation_20261009/`

**ESTABLISHED:** all removable fractions and uncertainties; decay-born and
k_parented mother-species compositions; the zero signal-side and zero
neutral-K-leakage results; per-bin maps, worst-bin/threshold statistics,
per-cell aggregates; the B7 combined accounting and its consistency with the
alpha0p2 ledger; the gen-side feasibility verdict with its spot-check
evidence; all gates.

**PROVISIONAL (internal diagnostic):** absolute scaled yields (expected-event
numbers); the MC-shape-as-data-proxy assumption per bin (inherited from the
central background-vector convention); microscopic interpretations of the
species mix (e.g. the exact Sigma+/Sigma- ratio reflects generator + GEANT
decay tables, not a measurement); the worst-bin values in sparse bins (1-4
data tracks) are shape statements, not rate measurements.
