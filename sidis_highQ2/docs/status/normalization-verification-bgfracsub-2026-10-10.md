# Normalization and provenance verification — response fake-fraction basis for background subtraction (v6/z005 high-Q2 charged-SIDIS)

- **Date:** 2026-10-10
- **Verifier:** h1-normalization (luminosity and weight authority), read-only verification dispatch.
- **Scope:** normalization/provenance verification of the measured response fake-fraction
  contamination **10.7141% (h+) / 10.4019% (h−)** — the basis for the user-decided (2026-10-10)
  background subtraction on the final Born-level result. Subtraction *scheme design* is a parallel
  physics-analyst dispatch and is out of scope here; this report verifies the basis only.
- **Context authority:** `sidis_highQ2/STATE.md`, `docs/status/normalization.md`,
  `docs/RUN_STATUS.md`, `docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`.

---

## VERDICT: PASS WITH CAVEAT

All four verification questions (derivation chain, sample identity, weight/luminosity correctness,
charge symmetry) PASS individually against the primary artifacts, with exact independent
reproduction of every headline number. The caveats are scope conditions and one stale
provenance-record field that the subtraction design must carry (Sec. 6); none is a defect of the
fake-fraction figures themselves.

---

## 1. Derivation chain (found and reproduced)

**Chain (all stages artifact-verified):**

1. **Response MC input:** RAPGAP31 high-Q² four-period sidistrees
   `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/rapgap31/{7097,7096,7071,7094}/`
   (7097=ep0304: 61 files; 7096=em0405: 121; 7071=em06: 61; 7094=ep0607: 121; **364 files total**,
   no `--n-files` cap; mapping fixed in `wrap_build_migration_per_charge_v4.sh`).
2. **Per-period response build:** `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`.
   Reco-side fills use `w_reco = weight1 × weight2 × NucliaWeight` (lines 690–698). MATCHED body:
   `R[i_reco, i_gen] += w_reco` (line 925); FAKE column:
   `R[i_reco, GEN_OVERFLOW_IDX] += w_reco` (line 951) for reco tracks passing
   `pass_reco_full = evt & trk & not_e & not_v0_match_top` (line 659) with **no matched gen π/K/p
   partner**. Both matched and fake draws draw from the same `pass_reco_full` (comment lines
   654–658); V0-matched reco tracks are dropped entirely (F3/C2F2, lines 649–652).
3. **Merge:** `merge_migration_per_charge_v4.py` sums the four per-period files into
   `h_R_{pos,neg}_rapgap` (extended TH2 491×246; `h_R_*_nominal` is an alias of rapgap;
   `h_R_*_djangoh` is a separate model-diagnostic matrix).
4. **f_i construction:** `scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`,
   `derive_response_blocks` (lines 516–563):
   `f_i = F_i / (Σ_j B_ij + F_i)` (line 540–544), `d_sig = data_fine × (1 − f_i)` (line 545),
   variance ×`(1 − f_i)²` (release driver line 86).
5. **Integrated figures:** audit `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/`
   (`audit_central.py` → `central_audit_numbers.json`), computed from the merged central ROOT +
   central data vector JSON. First documented in
   `docs/handoffs/SESSION_HANDOFF_backgrounds_systematics_2026-09-25.md` §4; authoritative
   record `docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md` §3.
6. **Consumption:** release driver
   `output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/scripts/run_bbb_ibu_release_coherentprior.py`
   (`LUMI=351000.0`; metadata `central_response: RAPGAP31`). The coherent prior is taken from the
   validated rebuild (used **only** for IBU initialization/efficiency, reproduction 6.86e-15/9.42e-15);
   `f_i` comes from the central merged response ROOT. The figures therefore carry **unchanged**
   from the 2026-06-10 plain-prior release through the coherent-prior central release
   (2026-10-06/07) into the Born release raw parent (2026-10-09) — they depend only on the response
   matrix and the data vector, which are byte-identical inputs across those releases.

**Selection (verified identical in `build_migration_th2_per_charge_v4.py` and
`build_data_vector_v4.py`):** event `150 < Q2es < 20000`, `0.1 < Yes < 0.7`, `Empz > 45`,
`|PVz| < 35 cm`, `xF_SIDIS_es < 0` (current hemisphere); track `Pt > 0.15` (no upper cut),
`|η| < 1.6`, `NHitDedx ≥ 5`, `RadLength ≥ 5.0`, `|DcaPrime/cosh(η)| < 2`, `IsFromPrimary == 1`,
central/short/combined track class, computed z ∈ (0,1); masks `IsMatchedElectron == 0`,
`IsMatchedV0Daughter == 0` (MC-only flags, vacuous in data — ledger §4, ESTABLISHED).

**Binning:** 490 fine reco bins per charge = 7 (Q2,x) cells (S0x0…S3x0) × 7 z × 10 pT;
z edges [0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]; pT edges [0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0,
3.0, 4.0, 6.0, 8.0]; extended response (491, 246); charge-major data vector of 980 bins
(0–489 = h+, 490–979 = h−).

**Independent reproduction (this verification, from the primary ROOT + data-vector JSON,
2026-10-10):** exact match to the audit on every quantity (table in Sec. 4).

| Charge | data raw Σ | subtracted Σ | removed | **integrated removal (data-weighted)** | MC-side integrated fake fraction |
|---|---:|---:|---:|---:|---:|
| h+ | 1,975,301.0 | 1,763,665.127348 | 211,635.872652 | **10.7141%** | 10.6153% (ledger: 10.61%) |
| h− | 1,908,796.0 | 1,710,244.718995 | 198,551.281005 | **10.4019%** | 10.3124% (ledger: 10.31%) |

The headline 10.7141%/10.4019% are the **data-weighted** integrated removal
(`Σ d_i f_i / Σ d_i`); the MC-side integrated fractions are close but distinct. Both reproduce.

---

## 2. Sample identity verdict: PASS

- The fake fraction is derived from `h_R_{pos,neg}_rapgap` — **the central response itself**, i.e.
  the same sample/chain that defines the response used by the release (RAPGAP31 four-period
  catalogs 7097/7096/7071/7094; 364 files). This is the registered response provenance:
  release metadata `central_response: RAPGAP31`; STATE.md mixed-scheme configuration ("central
  response stays RAPGAP"); the audit JSON `meta_sums` blocks are `R_*_rapgap_*`.
- This is the **correct** sample for the subtraction: the fake column of the response that unfolds
  the data is, by construction, the response-consistent estimator of the MC-unflagged reco-fake
  contamination under the central selection. Subtracting `d_sig = data×(1−f_i)` removes exactly the
  class the response declares fake (exactly-once; ledger §3, §7).
- The DJANGOH matrix in the same merged file is a model diagnostic only and is **not** consumed for
  `f_i` in the central chain. For context (diagnostic, not used): its integrated removal would be
  10.7863% (h+) / 10.4712% (h−) — a generator spread of ≈0.07 pp integrated.
- No other sample's artifacts enter the derivation (no alpha0p2 template, no all-fake response, no
  DjBkgd/low-Q2/7091 file touches this chain).

---

## 3. Weight and luminosity verdict: PASS

**Weight consistency (numerator/denominator of f_i):**
- Fake column and body rows are filled in the **same pass, from the same files, with the same
  weight** `w_reco = weight1 × weight2 × NucliaWeight` (builder lines 922–952).
  `f_i = F_i/(Σ_j B_ij + F_i)` is therefore a same-sample, same-weight ratio: **any global MC
  normalization, including the MC luminosity, cancels exactly.**
- Data side is **unweighted** (count 1 per passing track; `build_data_vector_v4.py` docstring
  "Data events are unweighted … MC weights are NEVER applied"; `w2 = raw` → Poisson, verified
  `data_w2_equal_data_bins = 490/490` per charge; `bg_subtracted = false`).
- Weight sanity on the response sample (audit `mc_truth_weight_per_raw_event`, w1·w2 per raw
  event): h+ mean 1.407944 / median 1.087888 / p99 3.682616 / max 3.756705; h− mean 1.408244 /
  median 1.081047 / p99 3.617135 / max 3.643910; **0 bins > 5× mean** (CLEAN). This is a genuine
  GKI-weight distribution (median ≈ 1.08 with tails to ≈ 3.7), not the identically-1.0 wrong-source
  signature; note these are the high-Q² S67 catalogs, a different production from the V0-era
  RAPGAP31 sample, so the V0-era weight1 mean ≈ 4.14 expectation does not apply in this regime.

**Luminosity:**
- **No MC luminosity enters the fake-fraction chain anywhere.** Builder: weights only. Merge:
  sums only. `derive_response_blocks`: pure ratio. Release driver: `LUMI = 351000.0 nb⁻¹` (L_data)
  only, used solely to convert unfolded yields to nb.
- Recorded MC luminosity for the response sample (verified, correct sample — not reused from any
  other sample): `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`,
  `per_catalog/rapgap31_*`:
  7097 (ep0304) 1,529,489.0 + 7096 (em0405) 3,033,378.0 + 7071 (em06) 1,515,146.0 +
  7094 (ep0607) 3,054,878.0 = **9,132,891.0 nb⁻¹** (matches `docs/status/normalization.md`
  RAPGAP31 `L_MC_total`; generator-wide template scale 351000.0/9132891.0 = 0.03843251824641288 —
  relevant only to absolute template normalization elsewhere, **not** to f_i).
  Chunk records (61/121/61/121) match the consumed sidistree file counts exactly — full-sample
  consumption, no partial-file risk.
- **Forbidden-lumi traps: none present** in this chain. No 126,880 nb⁻¹; no 297,657.8 nb⁻¹ and no
  356,613.0/356,000 nb⁻¹ in the fake-fraction derivation or the release driver. L_data is the
  current 351,000.0 nb⁻¹ (four-period S67 including ep0304) everywhere it is used.
- Data vector totals verified: 658/658 files, 7,538,865 tracks seen, 3,902,480 in PS,
  3,884,097 in grid; h+ 1,975,301 / h− 1,908,796.

**Audit gates (all clean, reproduced):** 0/980 negative raw reco inputs; 0 negative
fake-subtracted bins; 0 background-exceeds-data bins; 0/490 negative IBU and 0/490 negative BBB
cross sections (audit JSON negativity block, recorded at the 2026-06-10 release path; the current
coherent-prior release revalidation independently reports 490 rows finite / 0 negatives per charge).

---

## 4. Per-charge fake fractions (integrated and per-bin)

The subtraction is **per-charge by construction**: `f_i` is computed separately from
`h_R_pos_rapgap` and `h_R_neg_rapgap` and applied to the corresponding charge's data vector
(release driver loops `charge ∈ {pos, neg}` with per-charge `load_response_and_data`).

**Integrated (data-weighted):**
- h+ **10.7141%** (1,975,301 → 1,763,665.127)
- h− **10.4019%** (1,908,796 → 1,710,244.719)
- Charge asymmetry: **0.3122 pp** — the "charge-symmetric to ~0.3 pp" statement in
  `docs/critiques/publication-grade-baseline-review-2026-10-10.md` is accurate.

**Per-bin f_i ranges (490 bins per charge; reproduced):**

| Charge | min | median | p99 | max | zero-f_i bins |
|---|---:|---:|---:|---:|---:|
| h+ | 0.0 | 10.0987% | 27.3667% | 32.2439% | 3 |
| h− | 0.0 | 9.9844% | 24.1467% | 30.6705% | 6 |

**Per-charge vs common fractions:** per-charge `f_i` is the response-consistent treatment and is
what the chain already applies; it should be retained for the published subtraction. A common
(charge-averaged, 10.5580%) fraction would under-remove h+ and over-remove h− by ≈0.1561 pp each.
Final scheme choice rests with the parallel physics-analyst dispatch; from the
normalization/provenance side the per-charge vectors are the verified basis.

---

## 5. Provenance register (primary artifacts)

- Merged central response + data vector:
  `H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/migration_inputs_...root`,
  `data_vector_...json` (+ `.binmap_v3.json`).
- Per-period response files: `.../per_file_migration/rapgap31/mig_v4_rapgap31_{em0405,em06,ep0304,ep0607}.root`.
- Builder/merge/data-vector/f_i code: `.../scripts/unfolding/tunfold_official_v4_finalcandidate_.../`
  and `.../scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`.
- Release driver: `output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/scripts/run_bbb_ibu_release_coherentprior.py`.
- Audit: `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/{audit_central.py,central_audit_numbers.json}`.
- Ledger (authoritative background accounting):
  `sidis_highQ2/docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md` (+ JSON sidecar
  `sidis_highQ2/data/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.json`).
- MC luminosity record: `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
  (`per_catalog/rapgap31_709{4,6,7}_*`, `rapgap31_7071_*`).

---

## 6. Caveats, discrepancies, and residuals (the "WITH CAVEAT" content)

1. **Scope of the figures — unflagged fake class ONLY.** 10.7141%/10.4019% quantify the
   MC-unflagged reco-fake class removed by `f_i`. They do **not** include the surviving residual
   categories (ledger §6: residual electrons 3.8355% cone-applied [≈4.1441% no-cone, PROVISIONAL],
   V0 daughters 3.4002%, low-Q2 0.0054%, DIFFVM ρ 0.0074%; det.int. is in-response by design,
   86.18/13.82, and external det.int. subtraction is FORBIDDEN). Any published subtraction built on
   this basis must keep the categories disjoint (exactly-once rules, ledger §7) and must not
   re-subtract what `f_i` already removes.
2. **Closure assumption (inherent, documented ledger §9.1):** the `f_i` transfer to data assumes
   the data fake composition matches the MC-unflagged fake class; there is no data-side
   independent validation of this assumption.
3. **No per-bin f_i statistical uncertainty is propagated.** Response `sumw2` histograms exist
   (`h_R_*_rapgap_w2`) but are not consumed; the fake-fraction statistical uncertainty is not
   quantified in the audit (known covariance gap, STATE.md). The subtraction design must at
   minimum state this; the generator diagnostic spread (RAPGAP vs DJANGOH ≈ 0.07 pp integrated)
   is a lower-bound indication of model dependence, not a statistical error.
4. **Stale provenance-record field (discrepancy, not consumed):** the MC-lumi aggregation file
   `highQ2_mc_luminosity_aggregated.json` carries header field
   `L_data_HERA-II_S67_nb: 297657.8` — the stale 2005–2007-only data luminosity. The
   fake-fraction chain does not read it (release driver hardcodes 351000.0), and the per-catalog
   MC luminosities are unaffected, but that field must not be quoted as a data luminosity.
5. **Audit negativity-block path age:** `audit_central.py` records the IBU/BBB negativity gate at
   the 2026-06-10 release path (`bbb_ibu_finalcandidate_...`). The `f_i` figures themselves are
   computed from the shared central ROOT/data vector, which are the same inputs as the current
   coherent-prior release (verified: same `ROOT`/`DATAJSON` paths in the current driver), so the
   figures are unaffected; the current release's own revalidation (490 finite / 0 negatives per
   charge) covers the current chain.
6. **Residual-template normalization basis (for the subtraction designer; NOT part of this
   verification's verdict):** if/when the residual categories are added as external vectors, the
   recorded generator-wide scales apply (normalization.md): RAPGAP31 `L_MC_total = 9,132,891.0`
   nb⁻¹ (scale 0.03843251824641288), DJANGOH14 `L_MC_total = 10,487,495.0` nb⁻¹ (scale
   0.03346843073584302), DjBkgd five-ReqID sum 15,813,400.0 nb⁻¹ (scale 0.0221963651; provenance
   not closed), 7091 `L_MC = 4,379,935.0` nb⁻¹ (scale 0.08013817556653238). `L_data = 351000.0`
   nb⁻¹ throughout. None of these enter the fake fraction.

**Residuals summary:** exact agreement (0 residual) on all reproduced headline quantities
(integrated removal both charges, MC-side fractions, per-bin min/median/p99/max both charges,
data totals, audit gates). The only numeric discrepancy found anywhere in the provenance chain is
the stale `L_data_HERA-II_S67_nb` header field (item 4), which is unconsumed by this chain.

---

## 7. Open blockers relevant to a published subtraction (carried, unchanged)

- `P-ALPHA` (alpha0p2 response-consistency unvalidated; alpha0p2-derived residual magnitudes
  remain diagnostic-grade).
- Track-level background systematic blocked on missing `overlap_980`/`n_raw_980` in the alpha0p2
  aggregates.
- DjBkgd template exposure provenance (oomclumi/generator-log accounting) outstanding.
- `cRad_bgfracsub` must remain a separate derived variant (STATE.md); the background-subtracted
  Born product must go through its own gate before replacing the nominal.

---

*Verification performed read-only on the NAF login node (single-file ROOT/JSON reads only).
No release, table, or artifact was modified.*
