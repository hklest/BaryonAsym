# P0.4 Cone Miss-Row Guard Refusal — Adjudication (2026-10-11)

**Case:** Phase F release driver `run_bbb_ibu_release_cone_2026-10-11.py` (Condor
cluster 5486250) refused at the P0.4 `cone_miss_row_guard` on all four
(tag, charge) combos: `miss_delta != matched_delta` exactly.
**Question:** Is the guard formula wrong (verdict a: LEGITIMATE refusal of a
too-strict assertion — supply the corrected assertion with numeric windows),
or is the cone-edited builder inconsistent (verdict b: BUG — name file, site,
fix, re-runs)?
**Verdict: (a) LEGITIMATE.** The refusal is correct behavior of a guard whose
exact-equality clause is provably too strict; the cone S2′ builder is
consistent. No production phase A–E output is invalidated. Only the driver's
guard clause must be corrected before the Phase F re-run.

All work for this adjudication was READ-ONLY (no builder, driver, or shard
files were modified; diagnostic scripts ran from `/tmp/opencode` on single
input files per the login-node policy).

---

## 1. The guard and what it asserted

`cone_miss_row_guard` (driver lines 344–392) reads integer fill counters
(`meta_n_matched_*`, `meta_n_miss_*`) summed over byte-identical shard lists
(cone: `production/s2prime_shards/{rapgap31,djangoh14}/`, 73/101 shards;
no-cone reference: `output_v6/studies/detint_variant_s2prime_2026-10-09/shards/`)
and asserts, per (tag, charge):

1. shard counts 73/101 — **correct, keep**;
2. `miss_delta == matched_delta` EXACTLY (line 353/369) — **wrong clause,
   the subject of this adjudication**;
3. `0 <= matched_delta` — **correct, keep**;
4. `matched_delta <= n_alpha_rejected_matched_signal + n_alpha_rejected_detint`
   — **correct, keep** (passes 4/4; slack = alpha-rejected legs that would
   have failed the z-guard/reco-grid, plus the gen-fail→fake-fill fraction).

Clause 2 encodes the assumption *"every cone-rejected matched fill frees
exactly one miss fill."* Sections 2–4 prove from the builder code and from
data that this assumption is false in the central machinery itself, and
measure the exact violation.

## 2. Exact bookkeeping identity (derivation)

Per (campaign, charge) define the miss-eligible gen pool
`|G*| = sum(h_mc_count_{pos,neg})` (CTG rows with `i_gen >= 0`: v4 gen
fiducial + species pool + V0-daughter-gen veto + gen grid, per
`compute_gen_bin`), the consumed-key set `C` (keys recorded by matched fills
and by fc=3 rerouted det.int. legs, builder lines 1317–1370), and
`m*(kappa)` = multiplicity of `kappa` among miss-eligible CTG rows. The miss
loop fills exactly the unconsumed miss-eligible rows, so

```
n_miss = |G*| - sum_{kappa in C} m*(kappa)
D     := n_matched + n_miss - |G*| = n_matched - sum_{kappa in C} m*(kappa)
```

`D` is the **standing excess** of matched fills over distinct consumed
miss-eligible gen rows. The cone never touches the gen side (reco-only alpha,
pre-z-guard; gen loop unchanged), so `|G*|` is cone-invariant and

```
deficit := matched_delta - miss_delta = D_nc - D_c     (EXACT identity)
```

Cone-rejecting a matched leg contributes to the deficit:
**+1** if its key maps to no miss-eligible row (k_parented keepers: STR
partners are never CTG rows) or if the key stays consumed by a surviving
co-recorder (shared gen partner); **0** for a normal leg (frees one miss
fill); **-(m*-1)** for a fully-freed multi-row key (measure-zero in practice).

## 3. Empirical verification

### 3.1 Campaign accounting (guard JSON, all four combos)

| tag | charge | matched_delta | miss_delta | E = m−miss | E/matched | fake_delta | qa_bound | alpha_rej |
|---|---|---|---|---|---|---|---|---|
| rapgap | pos | 30,164 | 29,467 | **697** | 2.311% | −5,588 | 34,914 | 37,004 |
| rapgap | neg | 29,905 | 29,205 | **700** | 2.341% | −4,573 | 32,958 | 34,542 |
| djangoh | pos | 37,614 | 36,837 | **777** | 2.066% | −7,588 | 44,181 | 46,778 |
| djangoh | neg | 37,751 | 36,906 | **845** | 2.238% | −6,357 | 42,280 | 44,204 |

Bound clause passes 4/4. (`production/qa_tools/phase_f_guard_accounting.json`,
recomputed independently by `check_phase_f_guard_accounting.py`.)

### 3.2 Shard-level D-test (rapgap31_0000, cone vs no-cone, same inputs)

| charge | G (both campaigns) | n_matched nc→cone | n_miss nc→cone | D_nc | D_cone | deficit | identity |
|---|---|---|---|---|---|---|---|
| pos | 885,953 = 885,953 | 406,355 → 405,993 | 481,946 → 482,299 | 2,348 | 2,339 | 9 | **9 = 9 exact** |
| neg | 870,453 = 870,453 | 396,686 → 396,282 | 476,352 → 476,740 | 2,585 | 2,569 | 16 | **16 = 16 exact** |

Gen side bit-invariant per shard; **D_nc > 0 in the no-cone canonical campaign
itself** (0.578%/0.652% of matched fills) — the excess is pre-existing central
machinery, not a cone artifact. Rerouted-key collisions 74 vs 75 (cone
key-freeing, consistent with the Phase C 55/174-shard −1..−3 shifts).

Funnel closure on the same shard: 952 alpha-rejected legs = 924 fills lost
(766 matched + 158 fake; fake losses draw from all three funnel classes
because `_rej_sig`/`_rej_di` include gen-fail legs that fill FAKE) + 28 legs
that would have failed z-guard/reco-grid anyway. Exact.

### 3.3 Single-file composition diagnostic (both generators)

Method: import the cone S2′ builder module twice; neutralize
`_compute_reco_electron_alpha` to a finite 1.0 rad (>= threshold for every
leg, NaN legs included) in the second instance — an exact no-cone selection;
run the module's own `FileAccumulator.process_event_chunk`, then recompute
per-leg key/category composition with the module's own
`compute_gen_bin`/`compute_reco_bin`. Files:
`sidistree_rapgap31_7071_em06_0000.root` (first file of shard rapgap31_0000)
and `sidistree_djangoh14_7072_em06_0000.root` (first file of shard
djangoh14_0000). Scripts: `/tmp/opencode/composition_diag_rapgap31_em06_0000.py`,
`/tmp/opencode/dtest_rapgap31_0000.py` (read-only).

| quantity | rapgap31 em06 0000 | djangoh14 em06 0000 |
|---|---|---|
| CT / CTG rows | 313,776 / 1,936,073 | 228,645 / 1,544,427 |
| matched legs (no-cone) | 148,564 (74,999 pos / 73,565 neg) | 107,415 (54,289 / 53,126) |
| **D_nc composition** | **823 shared keys / 1,717 legs (excess 894) + 117 keepers = 1,011** | **588 shared keys / 1,217 legs (excess 629) + 97 keepers = 726** |
| composition D vs module D_nc | 1,011 = 485+526 **exact** | 726 = 351+375 **exact** |
| partnerless non-keeper legs | **0** | **0** |
| CTG-ineligible partners (V0-flag mismatch) | **0** | **0** |
| CTG key collisions (m* >= 2) | **0** (all 1,936,073 keys distinct) | **0** (all distinct) |
| in-cone fraction: normal / shared / keeper | 0.1043% / **0.3494% (3.4x)** / 0% | 0.1159% / 0.0000% / 0% |
| cone-rejected matched legs | 159 = 153 normal + 6 shared | 123 = 123 normal + 0 |
| deficit (module) vs composition prediction | 5 vs 5 **exact** (pos 1, neg 4) | 0 vs 0 **exact** |
| D_nc / n_matched | 0.68% | 0.67% |

Findings:

- **The excess is fully and exactly decomposed** into (i) shared gen partners
  consumed by >= 2 reco legs (split-track structure; keys recorded once, N
  matched fills) and (ii) k_parented keepers (STR partners, by construction
  absent from the GTR-only CTG). There are **no** other channels on either
  generator: zero F7/Mother2 partnerless legs, zero IsMatchedV0Daughter /
  IsV0DaughterGen mismatches, zero CTG key collisions. The matched-gen mirror
  of the miss-eligibility cuts is exact in practice.
- **The deficit is the in-cone sampling of this excess population.** When no
  excess leg is cone-rejected (djangoh file), `miss_delta == matched_delta`
  holds EXACTLY (63==63, 60==60) — reproducing the S2′-smoke coincidence
  (85==85, 72==72) and explaining why the central-copy smoke saw 2/4 deficits
  ("consumed-key collisions — a pre-existing central-machinery effect").
- **Shared legs are enriched in-cone** (0.349% vs 0.104% on the RG file;
  split tracks cluster geometrically near the electron direction), which is
  why the campaign deficit fraction (2.07–2.34% of matched_delta) exceeds the
  standing-excess fraction (0.61–0.68%).
- **Campaign-magnitude cross-check:** E_RG = 1,397 observed vs ~1,200–2,100
  predicted from N_shared_keys(~300k) x (2 p_s − p_s²); E_DJ = 1,622 vs
  ~1,200–2,000. Consistent; per-file deficits fluctuate (0–9 per file), the
  campaign sums are stable at ~2.2% of matched_delta.

## 4. Verdict

**(a) LEGITIMATE.** The P0.4 refusal correctly stopped the release on a
guard clause that cannot hold: `miss_delta == matched_delta` exact equality
is violated by the central consumed-key machinery's standing excess
(shared gen partners + keepers), which exists identically in the no-cone
canonical campaign and is sampled by the cone at ~2.2% of matched rejections.
The cone S2′ builder is consistent: gen side bit-invariant, the
deficit = D_nc − D_c identity holds exactly at every scale tested
(shard, file, campaign), and the full alpha/fill accounting closes exactly.
No re-runs of Phases A–E are required; their products are sound.

## 5. Corrected guard assertion (replacement for clause 2)

Per (tag, charge), over byte-identical shard lists, in addition to clauses
1, 3, 4 (kept unchanged):

- **(2a)** `miss_delta <= matched_delta` (replaces exact equality).
- **(2b)** `E := matched_delta - miss_delta` must satisfy
  `0.01 * matched_delta <= E <= 0.04 * matched_delta`
  (mechanism window: in-cone sampling of the ~1.2% shared-key/keeper excess
  with its 2–3.5x in-cone enrichment; measured E/matched_delta spans
  2.066–2.341% across the four combos). E outside the window indicates a
  real bookkeeping break and must refuse.
- **(2c) NEW** gen-side invariance: `sum(h_mc_count_pos)`,
  `sum(h_mc_count_neg)` (equivalently the h_prior_gen sums) must be
  **identical** between the cone and no-cone shard lists per tag. This is
  the clause that actually protects the miss-row semantics; it is currently
  satisfied bit-identically.
- *(optional secondary)* `fake_delta <= 0` and
  `|fake_delta| <= n_alpha_rejected` (observed −4,573..−7,588 within
  34,542..46,778).

Numeric acceptance windows for the CURRENT campaign (deterministic
reproduction of the same build must hit E exactly; the window clauses guard
future semantically-different builds):

| combo | matched_delta | E measured | acceptance window [1%, 4%] |
|---|---|---|---|
| rapgap pos | 30,164 | 697 | E in [302, 1207] |
| rapgap neg | 29,905 | 700 | E in [299, 1196] |
| djangoh pos | 37,614 | 777 | E in [376, 1505] |
| djangoh neg | 37,751 | 845 | E in [378, 1510] |

All four measured E values sit inside their windows (55–63% of the upper
edge), as does every per-shard/per-file deficit tested.

## 6. Phase E disposition (f_i crosscheck)

**Accept the QA-tooling closure as valid; no re-run.**

- The canonical `crosscheck_unfold_methods_v4.py` is architecturally
  unrunnable for this chain: it requires `--tunfold-json`, the cone campaign
  produces IBU only, and no cone TUnfold JSON exists. Refusing to substitute
  the no-cone TUnfold JSON was **correct** (it would compare cone responses
  against a no-cone TUnfold solution — mismatched semantics).
- The QA-tooling substitute `check_phase_e_fi_qa.py` achieved the run-sheet
  intent and **passes 4/4** (`production/f_i_crosscheck_cone_*.json`):
  denominator reconciliation rel_diff 3.63e-8 (tolerance 1e-6, shard-meta
  rounding); cone-vs-no-cone f_i: 207/490 rows bit-identical, 156 rows
  merge-order float noise <= 1e-6, 127 real-change rows (median rel 0.17%,
  max 2.24%), total denominator rel drop 0.094% (~0.09–0.10% across combos);
  S2′-vs-central-cone denominator conserved at 2.4e-16 with f_i moves exactly
  the designed det.int. body→fake rerouting.
- **Action:** record the run-sheet gap (Phase E wording assumes the canonical
  TUnfold crosscheck; an IBU-only chain cannot satisfy it) as a
  documentation item. No Phase E re-run is needed or possible without
  producing a cone TUnfold JSON, which is out of scope for this chain.

## 7. Scope of re-runs (verdict a)

- **Phase F only** (the release driver `run_bbb_ibu_release_cone_2026-10-11.py`),
  after the guard correction of Section 5 is applied to the driver
  (a code edit owned by the supervisor/builder-engineer; this adjudication
  did not modify the driver, per run rules — the driver was correctly left
  unpatched when it refused).
- Phases A–E (shards, merges, central/coherent replicas, f_i crosschecks)
  are sound and require **no re-runs**.

## 8. Evidence provenance

- Driver with the guard: `H1ChargedSidis_HighQ2/output_v6/studies/cone_alpha_central_build_2026-10-11/production/run_bbb_ibu_release_cone_2026-10-11.py` (lines 344–392).
- Cone S2′ builder: `.../cone_alpha_central_build_2026-10-11/copied_scripts/build_migration_th2_per_charge_s2prime.py` (cone at 943–998; matched/fake fill + consumed keys 1269–1395; miss loop 1397–1420; meta writer 1640–1743).
- Guard accounting: `.../production/qa_tools/phase_f_guard_accounting.json` (+ tool `check_phase_f_guard_accounting.py`).
- Funnel totals: `.../production/s2prime_shards_alpha_qa_sum.json` (exact funnel decomposition pre = after_matched_signal + after_detint + after_unflagged_fake + Σrejected, both charges).
- Shards: `.../production/s2prime_shards/{rapgap31,djangoh14}/` and `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/shards/`.
- Input files (single-file diagnostics): `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root`, `.../mc/djangoh14/7072/sidistree_djangoh14_7072_em06_0000.root` (shard lists: `output_v6/studies/coherent_prior_response_rebuild_z005_20261006/shard_lists/`).
- Phase E: `.../production/f_i_crosscheck_cone_{rapgap,djangoh}_{pos,neg}.json`.
- Sem references: `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md` (S2′ §2.2, CTG GTR-only), `sidis_highQ2/docs/audits/cone-alpha-track-selection-audit-2026-10-11.md`, `sidis_highQ2/docs/backgrounds/cone_alpha_cut_impact_study_2026-10-11.md`.
- Independent soundness (from the campaign record, not re-derived here):
  prez 174/174 exact; Sec 2.5 B+F conservation 348/348 at 2.46e-15; Phase D
  central-replica bit-exact 174/174; gen side (h_prior_gen/h_mc_count)
  bit-identical to the June no-cone merge.
