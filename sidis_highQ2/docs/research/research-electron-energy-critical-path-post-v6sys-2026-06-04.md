# Critical path: producing the `electron_energy` systematic uncertainty for high-Q² SIDIS unfolded datapoints after v6sys shifted outputs exist

**Topic:** Post-`v6sys` (Stage 1 + Stage 2 paired) electron-energy-scale systematic
on the final-bin / 210-bin TUnfold release.

**Date:** 2026-06-04.

**Scope:** Identify the exact ordered chain of scripts and artifacts that must
run on NAF + locally, *after* the v6sys Stage 1+Stage 2 shifted production for
`electron_energy_up` and `electron_energy_down` has been completed for data and
MC, to replace the BLOCKED metadata stub
`stub_electron_energy_BLOCKED.json` with a numerical per-bin σ + covariance
artifact wired into the unfolded high-Q² charged-SIDIS release. This is
research, not a plan or implementation.

**Sources cross-read:**
- `H1/sidis_highQ2/docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md`
  (frozen policy + 7-gate sequence)
- `H1/sidis_highQ2/docs/research/research-v6sys-smoke-physics-review-2026-06-03.md`
  (smoke state, Stage 1 admission-boundary bias rationale)
- `H1/sidis_highQ2/docs/research/research-electron-energy-systematic-critique-2026-05-26.md`
  (rank-1 cov FATAL verdict; noise-floor requirement)
- `H1/sidis_highQ2/docs/research/research-g1jES-electron-LAr-systematics-2026-06-01.md`
  (g1j_ES `AnalysisBase.C` precedent; shifted-response prescription)
- `H1/sidis_highQ2/docs/methodology/v6sys_h1_precedent_systematics_manifest_2026-06-03.md`
- `H1/sidis_highQ2/data/systematics_sources/v6sys_h1_precedent_manifest/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/{stub_electron_energy_BLOCKED.json, README.md}`
- `H1/sidis_highQ2/plan-v6sys-treebuilder.md`
- `H1/sidis_highQ2/scripts/unfolding/tunfold_official_v3_percharge/README.md`
  and `{wrap_build_migration_per_charge.sh, submit_build_migration_per_charge.condor, wrap_merge.sh, merge_migration_per_charge.py, unfold_tunfold_single_tau.C, submit_final_canonical.condor}`
- `H1/sidis_highQ2/scripts/analysis/build_finalbin_release_ptmerge.py`
  (FINALBIN release builder; currently does **not** ingest detector systematic
  sources at all)
- `H1/sidis_highQ2/scripts/background_subtraction/{subtract_low_q2_background_v3final.py, build_response_4d_lowq2_masked.py, README.md}`
- `H1/sidis_highQ2/scripts/systematics/{build_v6sys_h1_precedent_manifest.py, combine_systematics.py, apply_noise_correction.py, compute_noise_floor.py, noise_floor_bootstrap.py}`
- `H1/sidis_highQ2/docs/REPRODUCING_THE_ANALYSIS.md` (historical Phase 3
  precedent; superseded for v6sys but provides the only existing
  "shifted-response → re-unfold → emit source JSON" template)

---

## 0. Headline answer

There is **no existing turn-key driver** for the v6sys `electron_energy`
shifted-response → re-unfold → per-source covariance path against the current
**per-charge 240-bin TUnfold release**. The earlier `run_electron_systematic.py`
precedent referenced in `docs/REPRODUCING_THE_ANALYSIS.md` lines 532–541
operated on the **legacy 480-bin `build_response_4d.py` + `unfold_TUnfold_4d.py`
4D chain** and produced the rank-1 covariances that `h1-analysis-critic` later
flagged FATAL. The v6sys release runs on a different pipeline
(`scripts/unfolding/tunfold_official_v3_percharge/` → per-charge 240-bin gen ×
960-bin reco) plus a downstream `build_finalbin_release_ptmerge.py` that does
not currently consume detector-systematic sources. Most of the critical-path
glue is new code; the v6sys/per-charge response build + per-charge TUnfold
solve themselves can be reused with minor wrap-script changes.

**Recommended convention (release decision still required):** for the per-iSys
shifted-MC kinematic source (`electron_energy_up`, `electron_energy_down`),
unfold the **nominal data** against the **shifted MC response**. Take the
per-(charge, gen-bin) half-difference `δ_e = (n_unfold_up − n_unfold_down)/2`
as the per-bin shift, then build the noise-floor-respecting covariance.
This is the g1j_ES `AnalysisBase.C` recipe + Andreev 2024/2023 convention
already pinned in
`research-g1jES-electron-LAr-systematics-2026-06-01.md` §6 and §"shifted
response" notes, and it matches the frozen menu gate 5 wording ("Per-iSys
TUnfold-4D unfold of nominal data with shifted response completed"). Whether
to also re-unfold the shifted data against the nominal response as a
sanity-check (Aaron-style "data side too") is open and `h1-analysis-critic`
sign-off is required before any final-source numerics are admitted.

---

## 1. Pipeline state assumed at entry

This research assumes the v6sys production pipeline has already produced, per
syskey ∈ {`electron_energy_up`, `electron_energy_down`}, per period ∈
{`em0405`, `ep0405`, `ep0607a`, `ep0607b`} (and per generator ∈
{`djangoh14`, `rapgap31`} for MC):

- Stage 1 + Stage 2 paired data trees:
  `…/output/v6sys/data/<syskey>/<period>/sidistree_*.root`
- Stage 1 + Stage 2 paired MC trees (per generator):
  `…/output/v6sys/mc/<syskey>/<generator>/<period>/sidistree_*.root`
- Stage 1 admission-boundary smoke evidence (data + DJANGOH) recorded per
  syskey (frozen menu §4.D gate 1)
- Per-wheel setter audit log confirming the array form
  `for w in 0..4: SetShiftElecEnWheel(w, ±0.005)` was applied, not a single
  global `SetShift(eElecEn)` (gate 2)
- Both signs produced for both data and MC (gate 3)

The schema must match the v5g `sidistree`; metadata
`TrackQualityPayloadVersion = v6sys-trackqual-2026-06-04` must be present per
the Stage 2 payload smoke evidence captured in this session.

If any of those gates have not closed, the chain below cannot start; the
`electron_energy` source stays as `BLOCKED_PENDING_SHIFTED_RESPONSE`.

---

## 2. Critical-path table

Steps are ordered. "Reuse" means an existing script can be invoked as-is with
new arguments; "Reuse + small wrap edit" means an existing script is fine but a
new sysflag-aware wrapper or argfile is required; "NEW" means net-new glue.

| # | Step | Script / artifact (canonical path) | Inputs | Outputs | Reuse / NEW | Side | Blocker / open |
|---|------|------------------------------------|--------|---------|-------------|------|----------------|
| 1 | Pertrack CSV extraction (MC, shifted) | `scripts/sidis/extract_pertrack_v5g_fixedFrame_v3.py` (existing extractor; runs on NAF over the v6sys MC `sidistree_*.root`) plus a per-syskey wrap that points at `…/output/v6sys/mc/<syskey>/<gen>/<period>/sidistree_*.root` and routes to `…/output/pertrack_mc_v3p2_v6sys/<syskey>/<gen>/pertrack_mc_v3p2_<syskey>_reco_<gen>_<period>.csv.gz` | shifted MC `sidistree_*.root` (×2 sides × 2 generators × 4 periods × 2 streams reco+gen = 32 inputs) | pertrack CSVs per (syskey, gen, period, reco/gen) | Reuse + small wrap edit | MC only | None if v6sys MC trees expose the same branch set the v5g extractor expects (the v6sys plan §"reduced schema" preserves them per `plan-v6sys-treebuilder.md` §"Architecture and Output Layout"). |
| 2 | Pertrack CSV extraction (data, shifted) | Same extractor as #1, pointed at shifted data trees | shifted data `sidistree_*.root` per period × 2 sides | pertrack data CSVs per (syskey, period) → merged `pertrack_v5g_v3p2_data_<syskey>_merged.csv.gz` | Reuse + small wrap edit | Data only | The "consistent unfolding convention" choice (§3) decides whether step 2 is required at all. If the chosen convention is "nominal data + shifted response", step 2 is optional / cross-check only. |
| 3 | Low-Q² bg subtraction on nominal data (already done) | `scripts/background_subtraction/subtract_low_q2_background_v3final.py` | Nominal data pertrack CSV; bg-MC CSVs | `data_subtracted_v3final_REAL_LUMI_filtered.json` (480-bin) — **already exists for nominal**; for the v6sys path the per-charge 240-bin merge later rebins via uniform spread, so the FINE input is the 480-bin nominal JSON | Reuse (no change) | Nominal data | None for nominal. If §3 chooses to also build a "shifted-data" branch, an additional bg-subtraction pass on the shifted data CSV is required → produces `data_subtracted_v3final_<syskey>_REAL_LUMI_filtered.json`. Bg-MC luminosities are still `PLACEHOLDER`; final numerics blocked on `h1-normalization` finalizing those values. |
| 4 | Per-(MC, period, syskey) per-charge migration TH2 build (shifted MC) | `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge.py` via a per-syskey copy of `wrap_build_migration_per_charge.sh` + `submit_build_migration_per_charge.condor` that points `INPUTS` at `$WORK/output/v6sys/mc/<syskey>/<gen>/<period>` instead of `$WORK/output/mc/<gen>/<period>` and writes to `…/output/tunfold_tree_v3_percharge_v6sys/<syskey>/per_file_migration/<gen>/mig_v3_<gen>_<period>.root` | shifted MC `sidistree_*.root` per (gen, period) | per-charge TH2 (`h_R_pos`, `h_R_neg`, sumw², `h_prior_<chg>`, `h_mc_count_<chg>`) per (syskey, gen, period) | **NEW** wrap + condor file (8 jobs per syskey: 4 RAPGAP + 4 DJANGOH); same compiled `build_migration_th2_per_charge.py` body | MC only (per syskey) | Decision: does syskey label live in output dir or in file name? Recommend output dir (mirrors v6sys output convention from `plan-v6sys-treebuilder.md`). |
| 5 | Merge per-syskey shifted MC into a per-syskey migration ROOT | `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge.py` via per-syskey copy of `wrap_merge.sh` that points `PER_FILE_DIR` at the per-syskey dir from #4 and writes `…/output/tunfold_tree_v3_percharge_v6sys/<syskey>/migration_inputs_v3_percharge.root` plus `…/<syskey>/migration_inputs_v3_percharge.binmap_v3.json`. The `--data-json` flag remains the **nominal** `data_subtracted_v3final_REAL_LUMI_filtered.json` (recommended convention; see §3) | per-syskey per-charge TH2s from #4 + nominal data JSON | merged per-syskey response ROOT containing per-charge response `h_R_<chg>_djangoh`, `h_R_<chg>_rapgap`, `h_R_<chg>_nominal` (= rapgap by convention) and per-charge data TH1 `h_data_bgsub_<chg>` (+ `_w2`, overflow per F1) | **NEW** wrap (1 job per syskey); compiled `merge_migration_per_charge.py` body unchanged | Both inputs (shifted MC + nominal data) | Per the F1 convention in `merge_migration_per_charge.py` lines 363–569, the `miss_yield_overflow` is built from the **shifted** MC's miss row (`sum_j R[NB_R, j] * data_total / prior_total`); this is correct under the recommended convention. |
| 6 | Per-charge TUnfold solve at the canonical τ, per syskey | `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C` invoked via a per-syskey copy of `wrap_final_canonical.sh` + `submit_final_canonical.condor`, with `--write-cov` enabled, pointed at the per-syskey merged ROOT from #5 and at the canonical taus `TAU_TAUOPT = {pos: 1.000e-06, neg: 2.5929437974e-06}` from `build_finalbin_release_ptmerge.py:93`. Mode = `data` (i.e. real `h_data_bgsub_<chg>` from #5; per recommended convention this is the **nominal** data); the response loaded is the **shifted** `h_R_<chg>_nominal` from the per-syskey ROOT | per-syskey merged response ROOT; canonical τ per charge | per-(syskey, charge) `unfolded_v3_percharge_<chg>_tau<…>_<syskey>.root` (~15 MB) with `h_unfolded_pub` (240 bins), `h_emat_total` (241×241), `h_rho_total`, `h_refold` + per-(syskey, charge) JSON `unfolded_v3_percharge_<chg>_tau<…>_<syskey>.json` carrying `unfolded_pub[240]`, `sigma_pub[240]`, gates, score | **NEW** wrap (2 charges × 2 sides = 4 jobs per syskey); compiled `unfold_tunfold_single_tau.C` body unchanged | Uses shifted MC response + nominal data (recommended convention) | Decision: pin τ to TAUOPT exactly, or scan a small ±¼-decade neighbourhood per syskey to detect τ-induced bias? The `sigma_reg` source already covers τ uncertainty on the **nominal**; for the shifted side, holding τ fixed at TAUOPT is the literature-standard recipe (Schmitt 2012; Andreev 2024 §6.4). Recommend τ fixed at TAUOPT; flag any τ-dependence ≥ 5% as a critic gate. |
| 7 | Aggregate per-(syskey, charge) 240-bin unfolds into the FINALBIN merge map | `scripts/analysis/build_finalbin_release_ptmerge.py` invoked via a per-syskey CLI mode (current driver runs only on the nominal). The aggregator already consumes per-tau JSONs, builds the 210→finalbin map, and aggregates `sigma_stat`, `sigma_reg`, `sigma_model`. A new `--systematic-source <syskey> --systematic-input <per-syskey JSON dir>` mode aggregates the shifted-response yield vectors `N^{up}_{c}` and `N^{dn}_{c}` onto the SAME pT-merge scheme that was frozen for the nominal release (using `final_binning_map_ptmerge.{json,csv}` from the nominal release dir as the canonical merge map) | per-syskey per-charge unfold JSONs from #6; nominal release `final_binning_map_ptmerge.json` | per-syskey per-charge `unfolded_finalbin_<chg>_<syskey>.json` with the same bin layout as the nominal `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE` final-bin yields | **NEW** glue (one-shot Python; ~150 lines, mostly remap-and-aggregate using existing helpers in `build_finalbin_release_ptmerge.py`) | n/a (postprocessing) | Must use the **frozen nominal** merge map, not re-derive per syskey; otherwise the per-bin half-difference is taken against a moving target. |
| 8 | Per-bin shift δ_e on the final-bin grid | NEW Python script `scripts/systematics/source_electron_energy_v6sys.py` that loads `unfolded_finalbin_<chg>_electron_energy_up.json` and `unfolded_finalbin_<chg>_electron_energy_down.json` plus the nominal finalbin `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/final_unfolded_<chg>.json`, computes per-charge per-bin `δ_central = 0.5 * (N_up − N_dn)`, `δ_up = N_up − N_nom`, `δ_dn = N_dn − N_nom` | per-syskey finalbin JSONs from #7; nominal finalbin JSON | per-charge per-bin shift arrays (length = N_final-bins, dictated by the release; ≤210 after pT-merge) | **NEW** | n/a | Per the critic memo `research-electron-energy-systematic-critique-2026-05-26.md` §4: half-difference δ_central is the central estimator; report both signed `δ_up` and `δ_dn` separately so noise-floor masking (#9) can act on each side. |
| 9 | Noise-floor characterisation of δ_e (gate 6) | `scripts/systematics/compute_noise_floor.py` already produces `noise_floor.json`/`cov_noise.npz` on the **480-bin** grid via `noise_floor_bootstrap.py` (20 data-Poisson + 20 MC-response replicas). It must be re-cast onto the **finalbin** grid (consume the nominal MC-stat bootstrap `mcstat_replicas_tau0p1/` and the per-charge `finalbin_toys_djangoh/finalbin_toys_djangoh_{pos,neg}.npz` already present per `README.md` lines 49–53 of the FINALBIN candidate release). Then `apply_noise_correction.py` (existing) compares per-bin `\|δ_e\|` against the per-bin noise floor σ_noise; bins with `\|δ_e\| < σ_noise` are MASKED (per critic FATAL note §5) | per-bin δ_e from #8; nominal finalbin MC-stat bootstrap; nominal finalbin Poisson bootstrap | per-bin noise-floor mask, corrected δ_e (where applicable), per-bin σ_e for the source | Reuse (`apply_noise_correction.py`, `compute_noise_floor.py`) + **NEW** finalbin-grid adapter (the existing tools are wired to the 480-bin grid; the finalbin grid is smaller and the per-bin replicas already exist; only the loader/aggregator changes) | n/a | This step is non-optional. Skipping it reproduces the critic-FATAL rank-1 cov pattern flagged in `research-electron-energy-systematic-critique-2026-05-26.md` §A1. |
| 10 | Per-source covariance build | NEW `scripts/systematics/source_electron_energy_v6sys.py` (same script as #8) emits: `data/systematics_sources/source_electron_energy_v6sys.json` (per-charge per-bin `δ_up`, `δ_dn`, `δ_central`, `sigma_per_bin`, `noise_mask`) and `data/systematics_sources/cov_electron_energy_v6sys.npz` (a covariance matrix that respects the noise-floor mask — **not** a blanket rank-1 `δδᵀ`. Recommended form per critic §4 + §A2: diagonal where the mask is true, off-diagonals only between bins sharing the same LAr wheel; i.e. block-diagonal in 5 wheels with off-diagonals restricted to bins whose reconstructed electron landed in the same wheel). Schema must declare `correlation_class = "correlated_within_wheel_only"` (a new class) or, conservatively, fall back to `"uncorrelated"` if the wheel-grouping cannot be reconstructed at the finalbin level | per-bin δ_e and mask from #9; per-bin wheel assignment of the scattered electron averaged over the nominal data (already available from extractor branch `scatEX/EY/EZ` if recomputed; otherwise approximate via Q²,x bin → η_e' band → wheel mapping) | `source_electron_energy_v6sys.json` + `cov_electron_energy_v6sys.npz` in `data/systematics_sources/` | **NEW** | n/a | The "no rank-1 cov on noise-dominated bins" requirement is the hard constraint from `research-electron-energy-systematic-critique-2026-05-26.md` §A1. The wheel-grouped covariance form is the literature-anchored shape (g1j_ES per-wheel application implies per-wheel correlation). |
| 11 | Replace BLOCKED stub in release manifest | Rerun `scripts/systematics/build_v6sys_h1_precedent_manifest.py` after #10; it auto-detects the presence of `source_electron_energy_v6sys.json` (the script must be edited to switch the `electron_energy` entry from `BLOCKED_PENDING_SHIFTED_RESPONSE` to `PRESENT_VALIDATED` when both files exist and pass schema checks) | `source_electron_energy_v6sys.json` + `cov_electron_energy_v6sys.npz` | Updated manifest in `data/systematics_sources/v6sys_h1_precedent_manifest/data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` (and the 210bin_FILTERED twin) replacing `stub_electron_energy_BLOCKED.json` with the validated source | Reuse + small edit to `build_v6sys_h1_precedent_manifest.py` Category-C branch | n/a | Critic sign-off (gate 7) must precede this step. |
| 12 | Inject the source into the published release | `build_finalbin_release_ptmerge.py` must learn to consume `data/systematics_sources/source_electron_energy_v6sys.json` and add a per-bin `sigma_electron_energy` column to its release JSON, fold it into a combined `sigma_total` in quadrature with the existing `sigma_stat`, `sigma_reg`, `sigma_model`, `sigma_MCstat_response`, `sigma_C_rad`, `sigma_tracking_central_h1_1302_1321`, `sigma_NucliaWeight`, `sigma_trigger`, `sigma_lumi` (per frozen budget Eq. in `research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` §4.E lines 266–283) | nominal finalbin JSONs + `source_electron_energy_v6sys.json` | Updated `data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/` release JSON with `sigma_electron_energy` and updated `sigma_total` | **Mostly NEW** in `build_finalbin_release_ptmerge.py` (the current 1403-line driver does not contain `electron_energy`, `sigma_electron_energy`, or any `source_*.json` ingestion; verified by grep) | n/a | The current `combine_systematics.py` in `scripts/systematics/` is the **v3 (480-bin) framework** and is NOT the finalbin combiner; do not chain it. The finalbin combiner is in-driver per `build_finalbin_release_ptmerge.py`. |

---

## 3. Which steps need which inputs, and the convention decision

### Recommended convention (literature-anchored)

**Use shifted MC response + nominal data for the unfolding step.** This is:
- the g1j_ES `AnalysisBase.C:108–129` recipe transcribed in
  `research-g1jES-electron-LAr-systematics-2026-06-01.md` §6 lines 261–293
  ("rebuild the migration matrix with the shifted reco + nominal gen, then
  unfold the **nominal data** with each shifted response"),
- the H1 TUnfold high-Q² practice in Andreev 2024 §6.4 and Andreev 2023
  Tables 2–3,
- the wording of frozen gate 5 in
  `research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` line 256
  ("Per-iSys TUnfold-4D unfold of nominal data with shifted response"),
- and the `stub_electron_energy_BLOCKED.json:6` validation gate.

### What each step needs

| Step | Needs shifted MC | Needs shifted data | Needs nominal data | Needs nominal response |
|------|------------------|--------------------|---------------------|------------------------|
| 1 (pertrack extract MC) | Yes | – | – | – |
| 2 (pertrack extract data) | – | Optional (sanity-check only) | – | – |
| 3 (bg subtraction) | – | If §2 done | Yes (already done) | – |
| 4 (per-(MC,period) TH2) | Yes | – | – | – |
| 5 (merge per-syskey) | Yes | – | Yes (the nominal `data_subtracted_v3final_REAL_LUMI_filtered.json`) | – |
| 6 (TUnfold solve) | Yes (loaded as `h_R_<chg>_nominal` inside per-syskey ROOT) | – | Yes (loaded as `h_data_bgsub_<chg>` inside per-syskey ROOT) | – |
| 7 (finalbin aggregate) | Indirect (via #6 outputs) | – | Indirect (via #6 outputs) | – |
| 8 (per-bin δ) | – | – | Yes (nominal finalbin yields for the reference baseline) | – |
| 9 (noise floor) | – | – | – | Indirect (uses existing nominal MC-stat bootstrap on finalbin grid) |
| 10 (cov build) | – | – | – | – |
| 11–12 (manifest + release injection) | – | – | – | – |

### Prior decision required from supervisor before #6

1. **Convention:** confirm "nominal data + shifted MC response", as
   recommended. If supervisor instead wants the symmetric Aaron-style cross
   ("shifted data + nominal response" cross-check), step 2 + an extra branch
   of step 5 become mandatory.
2. **τ policy on the shifted side:** fix τ at TAUOPT (recommended) or run a
   3-point τ neighbourhood and report any > 5% deviation.
3. **Covariance shape on noise-floor-masked bins:** "uncorrelated" diagonal
   only (conservative, recommended as the entry-level option), OR "correlated
   within LAr wheel" block-diagonal (closer to physical truth but requires
   the wheel-assignment lookup per finalbin). `h1-analysis-critic` should
   sign off on the choice.
4. **Down-side handling:** SIDIS A_h needs symmetric ± per
   `research-electron-energy-systematic-critique-2026-05-26.md` §A2; do NOT
   drop the down-side as g1j_ES does. Confirm.

---

## 4. Existing vs new scripts

### Existing scripts that can be reused (no body changes)

- `scripts/sidis/extract_pertrack_v5g_fixedFrame_v3.py` — the pertrack
  extractor. The v6sys reduced tree is schema-compatible per
  `plan-v6sys-treebuilder.md` §"Output layout and naming"; verify at smoke
  time.
- `scripts/background_subtraction/subtract_low_q2_background_v3final.py` and
  `build_response_4d_lowq2_masked.py` — bg subtraction on nominal data is
  already canonical; the masked-response builder is only needed if §3 picks
  the symmetric "shifted data" branch.
- `scripts/unfolding/tunfold_official_v3_percharge/build_migration_th2_per_charge.py`
  — per-(MC, period) per-charge TH2 builder. Unchanged.
- `scripts/unfolding/tunfold_official_v3_percharge/merge_migration_per_charge.py`
  — merge driver. Unchanged.
- `scripts/unfolding/tunfold_official_v3_percharge/unfold_tunfold_single_tau.C`
  — TUnfold solve at fixed τ. Unchanged.
- `scripts/systematics/compute_noise_floor.py` and
  `scripts/systematics/apply_noise_correction.py` — noise-floor tooling.
  Needs a finalbin-grid loader/adapter but not body changes.
- `scripts/systematics/build_v6sys_h1_precedent_manifest.py` — manifest
  generator. Needs a small edit to flip Category-C status on detection of
  validated source artifacts.

### Existing scripts that need wrap-script / arg-file edits only

- `wrap_build_migration_per_charge.sh`, `submit_build_migration_per_charge.condor`
  → per-syskey twins (mostly path substitutions: `INPUTS`, `OUTDIR`).
- `wrap_merge.sh` → per-syskey twin (per-syskey `PER_FILE_DIR` and
  `OUT_ROOT`; nominal `DATA_JSON` unchanged).
- `wrap_final_canonical.sh`, `submit_final_canonical.condor` → per-syskey
  twin (per-syskey input ROOT; same TAUOPT τ).

### Net-new glue (must be written)

1. `scripts/sidis/wrap_extract_pertrack_v6sys.sh` (NAF) — wraps the existing
   pertrack extractor pointing at `…/output/v6sys/{mc,data}/<syskey>/...`.
2. `wrap_build_migration_per_charge_v6sys.sh` +
   `submit_build_migration_per_charge_v6sys.condor` (per-syskey, NAF).
3. `wrap_merge_v6sys.sh` (per-syskey, NAF).
4. `wrap_final_canonical_v6sys.sh` +
   `submit_final_canonical_v6sys.condor` (per-syskey, NAF; 2 charges × 2 sides
   per syskey).
5. `scripts/analysis/aggregate_finalbin_systematic_v6sys.py` (local) — reads
   per-syskey per-charge per-tau JSONs, applies the **frozen nominal** pT
   merge map from `final_binning_map_ptmerge.json`, emits
   `unfolded_finalbin_<chg>_<syskey>.json`.
6. `scripts/systematics/source_electron_energy_v6sys.py` (local) — δ build,
   noise-floor masking, covariance build, source JSON + NPZ emission.
7. Patch to `scripts/analysis/build_finalbin_release_ptmerge.py` (local) —
   consume `source_electron_energy_v6sys.json` and emit per-bin
   `sigma_electron_energy` + folded `sigma_total`. Currently the driver does
   not import any `source_*.json` artifacts at all (verified by grep on
   `source_electron`, `sigma_electron`, `systematic_source`).
8. Patch to `scripts/systematics/build_v6sys_h1_precedent_manifest.py` —
   auto-detect validated source files for Category-C entries.
9. Adapter for `compute_noise_floor.py` / `apply_noise_correction.py` to
   operate on the finalbin grid instead of the 480-bin grid.

### Existing scripts that should NOT be reused

- `scripts/v3final/combine_systematics_v3final.py` and
  `scripts/v3p2/build_data_release_v1p1.py` — historical 480-bin v3 release
  combiners; superseded by the in-driver combiner inside
  `build_finalbin_release_ptmerge.py`.
- `scripts/systematics/combine_systematics.py` — v3 data-contract combiner
  (`schema_version = "systematics_v3.0"`, `binning = "HQ-v5g-Bprime"`,
  `extractor = "extract_pertrack_v5g_fixedFrame_v3.py"`, hardcoded 8 cells ×
  6×5 grid). Not aligned with the finalbin-merged 210→N grid produced by
  `build_finalbin_release_ptmerge.py`.
- The legacy NAF `run_electron_systematic.py` referenced in
  `docs/REPRODUCING_THE_ANALYSIS.md` lines 536–541 — it targets the legacy
  480-bin 4D pipeline and the FATAL rank-1 cov path. Do not resurrect.

---

## 5. Validation gates before replacing `stub_electron_energy_BLOCKED.json`

All seven gates from
`research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` §4.D
(also enumerated in `stub_electron_energy_BLOCKED.json:48-55`) must close in
order:

1. **Stage 1 v6sys event-processing smoke** passes for `electron_energy_up`
   AND `electron_energy_down` on ≥1 period of DJANGOH MC + data. Reports:
   per-period admitted-event count Δ vs central; Stage 1 → Stage 2 chargedtot
   row count Δ; per-bin mean ΔQ²es and ΔYes on the Scheme B' grid. Owner:
   `h1-builder-engineer` + `h1-script-runner`. **Current state (2026-06-04):
   FAILED** — MC smoke blocked by NAF `/acs/mc2` worker mount per session
   context.
2. **Per-wheel setter audit** — confirms the v6sys builder applies the array
   form `for w in 0..4: SetShiftElecEnWheel(w, ±0.005)` and NOT a single
   global `SetShift(eElecEn)`. Owner: `h1-builder-engineer`. The
   `explicit_setter_recipe` in `stub_electron_energy_BLOCKED.json:15-20` is
   the contract.
3. **Symmetric ± production complete** — both `electron_energy_up` AND
   `electron_energy_down` produced for both data and both MC generators across
   all 4 periods. Owner: `h1-naf-operator`.
4. **Response matrix rebuilt per iSys** — per-syskey per-charge TH2s from
   step 4 + merge from step 5 produce the per-syskey
   `migration_inputs_v3_percharge.root` with `h_R_<chg>_djangoh` and
   `h_R_<chg>_rapgap` from the shifted MC. Owner: `h1-unfolder`.
5. **Per-iSys TUnfold-4D unfold of nominal data with shifted response
   completed** — step 6 outputs exist for both charges, both sides; pathology
   gates (`max_abs_rho_pub_offdiag < 0.95`, `n_negative_pub < 12`,
   `condition_number_pub < 1e6`, etc., per `README.md` lines 346–353) all
   pass on the shifted-response solve. Owner: `h1-unfolder`.
6. **Noise floor characterised** for the per-bin shift δ_e on the finalbin
   grid; bins with `\|δ_e\| < σ_noise_per_bin` are MASKED or replaced by the
   noise-floor estimate. Owner: `h1-systematics`. Per critic memo
   `research-electron-energy-systematic-critique-2026-05-26.md` §A1, a
   blanket rank-1 outer-product `δδᵀ` cov is FORBIDDEN on the unmasked side.
7. **`h1-analysis-critic` sign-off** — no rank-1 outer-product cov on
   noise-dominated bins; same-side fraction reported (should be < 50%);
   linearity-ratio diagnostic (per critique memo) reported.

Additional validation that should be added for the v6sys path specifically
(not in the original 7-gate list, but implied by step #6):

- **`area_residual_rel < 0.01`** under the F1 overflow convention (per
  `tunfold_official_v3_percharge/README.md` §"REV3 fixes" lines 17–23). The
  per-syskey unfold must pass the same hard-reject sentinel `1e6` test that
  the nominal unfold passes.
- **`reco_efficiency`** per syskey reported by the `[bug1-watchdog]` line in
  `build_migration_th2_per_charge.py`; values < 0.50 indicate matching is
  broken (Bug 1 regression).
- **Charge-conservation** per syskey: total positive yield + total negative
  yield should match (within noise) the nominal sum, after F1 miss
  accounting.
- **τ stability** at TAUOPT ± ¼ decade per syskey: per-bin Δ < 5% is the
  pass criterion (avoids importing τ-induced bias into δ_e).

---

## 6. Outputs / paths produced

### NAF (under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/`)

```
v6sys/
  data/<syskey>/<period>/sidistree_*.root                  # entry, assumed exists
  mc/<syskey>/<gen>/<period>/sidistree_*.root              # entry, assumed exists
pertrack_v5g_fixedFrame_v3p2_v6sys/<syskey>/
  pertrack_v5g_v3p2_<syskey>_data_merged.csv.gz            # step 2 (optional)
pertrack_mc_v3p2_v6sys/<syskey>/<gen>/
  pertrack_mc_v3p2_<syskey>_reco_<gen>_<period>.csv.gz     # step 1
  pertrack_mc_v3p2_<syskey>_gen_<gen>_<period>.csv.gz      # step 1
bg_subtraction/
  data_subtracted_v3final_<syskey>_REAL_LUMI_filtered.json # step 3 (optional)
tunfold_tree_v3_percharge_v6sys/<syskey>/
  per_file_migration/<gen>/mig_v3_<gen>_<period>.root      # step 4
  migration_inputs_v3_percharge.root                       # step 5
  migration_inputs_v3_percharge.binmap_v3.json             # step 5
  final_canonical_v6sys/
    unfolded_v3_percharge_pos_tau1.000e-06_<syskey>.root   # step 6
    unfolded_v3_percharge_pos_tau1.000e-06_<syskey>.json   # step 6
    unfolded_v3_percharge_neg_tau2.593e-06_<syskey>.root   # step 6
    unfolded_v3_percharge_neg_tau2.593e-06_<syskey>.json   # step 6
```

### Local (under `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/`)

```
data/systematics_sources/
  source_electron_energy_v6sys.json                        # step 10
  cov_electron_energy_v6sys.npz                            # step 10
  v6sys_h1_precedent_manifest/
    data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/
      stub_electron_energy_BLOCKED.json   → REMOVED at step 11
      source_electron_energy_v6sys.json   → ADDED   at step 11
      manifest.json                       → UPDATED at step 11
      README.md                           → UPDATED at step 11
    data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/
      ... same updates ...
data/unfolded_finalbin_v6sys/<syskey>/
  unfolded_finalbin_pos_<syskey>.json                      # step 7
  unfolded_finalbin_neg_<syskey>.json                      # step 7
data_release_rev6_TUnfold_AREA_FINALBIN_INTERNAL_CANDIDATE/
  ... existing release JSONs gain a sigma_electron_energy column
      and updated sigma_total at step 12 ...
```

---

## 7. Wall-time and resource estimate (per syskey, both sides)

Per the frozen menu §4.B and the per-charge pipeline README:

- Stage 1 + Stage 2 v6sys MC production: ~3 h Condor wall per side per period
  per generator ⇒ 2 sides × 4 periods × 2 generators ≈ ~16 h Condor wall
  per syskey (and ~16 h is the per-syskey envelope cited in frozen menu line
  232 for `electron_energy`).
- Pertrack extraction (step 1, 2): ~30 min Condor per period per syskey side
  per generator/stream ⇒ ~16 jobs × 30 min ≈ low fan-out, ~1 h wall.
- Per-(MC, period) per-charge TH2 (step 4): 8 jobs × ~30 min each per syskey
  side ⇒ ~30 min wall, ~4 h CPU.
- Merge (step 5): 1 job × ~15 min per syskey side ⇒ ~15 min.
- TUnfold solve (step 6): 4 jobs × ~3 min each per syskey ⇒ ~3 min wall.
- Aggregator + δ + noise-floor (steps 7–10): seconds–minutes local.

Production total per syskey (excluding the upstream v6sys MC trees, which are
the dominant cost): ≈ 2 h Condor wall + < 1 h local.

---

## 8. Risks and surprises

1. **Stage 1 v6sys MC smoke is currently FAILED.** Per session context the
   `/acs/mc2` worker mount blocks full MC production. The entire critical
   path above is gated on gate 1 closing; nothing downstream is meaningful
   until then.
2. **The current FINALBIN driver does not consume detector-systematic
   sources at all.** Verified by grep on `build_finalbin_release_ptmerge.py`
   (1403 lines, zero matches on `source_electron`, `sigma_electron`,
   `systematic_source`, `shifted_response`). Step 12 is a non-trivial patch
   on a complex driver, not a one-line addition.
3. **The legacy `combine_systematics.py` is binding to v3 480-bin schema and
   cannot be reused** for the finalbin release. Do not let supervisors point
   at it as the combiner.
4. **The `source_tracking_central_h1_1302_1321` precedent (DONE 2026-06-03)
   followed a different pattern**: post-unfolding diagonal source, no
   shifted-MC, no re-unfold. It is not a template for the migration-affecting
   electron-energy path; only categories B/D should follow it.
5. **The bg-MC luminosities are still PLACEHOLDER** (`1.0e6 nb⁻¹` per
   `subtract_low_q2_background_v3final.py`). The shifted-data branch (step 3
   optional) would inherit the same placeholder; final numerics are blocked
   on `h1-normalization` finalising real values.
6. **Per-wheel covariance correlation structure is undefined for the
   finalbin grid.** The frozen menu treats `electron_energy` as
   "fully-correlated within source" but per the critic memo §4 this is too
   strong on noise-dominated bins. The "correlated within wheel" alternative
   requires a wheel-lookup table that does not currently exist for finalbin
   cells. Default to diagonal-on-unmasked-bins until `h1-analysis-critic`
   sign-off.
7. **Charge-confusion ~1% on low-pT_h / low-z / S3b** is still pending
   wire-up per `tunfold_official_v3_percharge/README.md` §"open items". It
   couples to the per-charge δ_e calculation: low-pT_h bins with high charge
   confusion may show artificial residuals between `pos` and `neg`
   shifted-response unfolds; flag those bins separately.
8. **The 1.4-line F-A weight harmonisation defect** in the per-charge merge
   (`merge_migration_per_charge.py` §F-A) introduces a ~2% systematic on the
   F1 miss-yield overflow that already propagates to the nominal unfold. The
   shifted-response unfold inherits the same bias. Cross-check that the
   shifted-side δ_e is not dominated by the F-A miss-yield drift; this is
   the highest-priority residual bias risk in the critical path.
9. **`electron_phi` and `electron_theta` are critically adjacent.** The
   critic memo §8.1 notes the canonical H1 prescription has three
   independent components (E, θ, φ); shipping only `electron_energy` numerics
   without the other two leaves a missing 1.5–3% on σ_electron in φ_h-binned
   observables. The same critical path applies to `electron_theta`
   (`eElecTh + eElecTrTh`, ±1 mrad × 3 regions) with the same script
   structure. `electron_phi` is DROPPED from baseline per the frozen menu.
10. **Final-bin grid sensitivity.** Steps 7–10 use the **frozen** nominal
    `final_binning_map_ptmerge.json` as the merge target; if any pT-merge
    schemes change (e.g. an S3 z-row drop differs per syskey because of
    shifted-response yield migration into low-population bins), δ_e will
    be defined against a moving target. Mitigation: enforce the nominal
    map and re-flag any per-syskey bin that fails the nominal
    `support_mask` as a separate per-source `bin_invalid` mask.

---

## 9. Areas needing further investigation

- **Wheel-lookup table** for finalbin cells (needed to make the per-wheel
  covariance structure of #10 implementable).
- **Per-syskey τ stability scan** (3-point neighbourhood) decision and pass
  criterion; pin a defensible threshold (recommended 5%, but no literature
  anchor inside this workspace).
- **Cross-MC for the shifted side** (RAPGAP shifted response vs DJANGOH
  shifted response): the frozen menu's `rerun_plan` cites both generators,
  but the nominal pipeline uses RAPGAP as the canonical `h_R_<chg>_nominal`
  and DJANGOH only via the `dataDJ` cross-check. Whether the shifted-DJANGOH
  response is needed for an `electron_energy` final number, or only as a
  model-systematic cross-check, is unsettled.
- **Schema for `cov_electron_energy_v6sys.npz`**: the existing
  `cov_*.npz` files in `data/systematics_sources/` are 480×480; finalbin is
  N×N with N ≤ 210. A new schema version flag should be introduced so
  downstream consumers don't silently load a wrong-shape covariance.
- **Combined-source ordering in `build_finalbin_release_ptmerge.py`**:
  whether `sigma_electron_energy` should be added in quadrature
  before or after `sigma_tracking_central_h1_1302_1321`, etc. is mechanical
  (quadrature is associative for uncorrelated sources), but the
  fully-correlated electron-energy treatment with the diagonal tracking
  treatment requires the combiner to track per-source correlation classes
  explicitly. The current driver does not. Patch in step 12 must add this.

---

## 10. Compact summary (table of new vs reused, by side)

| # | New / reused | MC side | Data side | Per-syskey twin needed |
|---|--------------|---------|-----------|------------------------|
| 1 | Reuse + wrap edit | shifted MC | – | yes |
| 2 | Reuse + wrap edit | – | shifted (optional) | optional |
| 3 | Reuse | – | nominal (already done) | only if §2 ran |
| 4 | NEW wrap, reuse body | shifted MC | – | yes |
| 5 | NEW wrap, reuse body | shifted MC | nominal | yes |
| 6 | NEW wrap, reuse body | shifted MC response | nominal data | yes (2 charges × 2 sides) |
| 7 | NEW Python | indirect | indirect | n/a |
| 8 | NEW Python | – | – | n/a |
| 9 | Reuse + finalbin adapter | – | – | n/a |
| 10 | NEW Python | – | – | n/a |
| 11 | Reuse + small edit | – | – | n/a |
| 12 | NEW patch in driver | – | – | n/a |

All "shifted MC" entries require BOTH `electron_energy_up` AND
`electron_energy_down`; the data-side requirement is ONLY for the optional
sanity-check branch chosen in §3 decision (1).
