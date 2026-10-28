# Build plan — current-basis covariance package for the high-Q2 charged-hadron SIDIS IBU clean release (2026-10-17)

**Plan date:** 2026-10-17 (read-only planning task; no central product touched, nothing submitted).
**Goal (user decision):** proper covariance matrices for ALL uncertainty components on the FINAL canonical
clean-release basis — single denominator = official_reunfold IBU `k=4` on `data_sub'` (integrated h+
`10.50881084` / h− `10.21750255` nb, `L=351000.0 nb^-1`) — publishable for global fitters, following the
expert prescription: per-source **signed** shift vectors δ_k with rank-one δ_kδ_kᵀ; `C_stat` and `C_MCstat`
as full bootstrap matrices; uncorrelated per-bin terms diagonal;
`C_total = C_stat + C_MCstat + Σ_k δ_kδ_kᵀ + diag(uncorrelated)`; bootstrap matrices published as tables;
each δ_k published as a labelled signed column; PSD/rank validation with the near-singularity caveat.

**Baseline document:** `sidis_highQ2/docs/audits/covariance_definitions_vs_expert_prescription_2026-10-17.md`
(component-by-component verdicts; gaps G1–G7 as numbered in the tasking). Every gap is costed below.

**Governing anchors (STATE.md 2026-10-17 Clean-Release Promotion Override, FINAL):**
- RAW release: `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/`
- Budget (public quote = `total_with_bgsub19_rel`, medians h+ 5.9764% / h− 6.2968%): `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/`
- Stat sidecar (245×245 per charge, 1000 toys, seeds 12345/22345, SA-3 Poisson-only): `.../bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17/covariance/ibu_toy_{covariance,correlation}_{pos,neg}.json`
- Engine: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`
- Budget driver + common module (reusable chain machinery): `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/scripts/stage2_cone_2026-10-15/run_bgsub_budget19_cleandenom_cone.py` and `common_bgsub_setr_cone.py`

---

## 0. Planning findings that UPDATE the audit's cost model (all verified on disk 2026-10-17)

These findings change the production cost of G3/G2/G1 relative to the audit's wording. None of them
weakens the audit's verdicts; they make the plan cheaper and more precise.

1. **HFS minus-leg trees partially EXIST.** Under `H1ChargedSidis_HighQ2/output_v6/systematics/hfs/`:
   - `hfs_theta_down/`: **COMPLETE** — 865/865 per-file MC twins (364 rapgap31 + 501 djangoh14) and
     660/660 shifted DATA trees, `v6sys_key=hfs_theta_down` stamps verified
     (`ChargedSidisTreeBuilderHighQ2_v6sys/2026-06-03`). Only the unneeded `merged_7091` twin is absent.
     **Zero Stage-2 tree production required** for this leg.
   - `hfs_jes_up/`: 857/865 MC twins — exactly **8 per-file MC sidistrees missing**
     (djangoh14/7072/em06: `0012,0024,0027`; djangoh14/7092/em0405: `0101,0107,0120,0126`;
     rapgap31/7096/em0405: `0088`); EVLS dir holds 1523 vs 1525 (2 missing). DATA trees complete (660).
     Completion = ≤10 Stage-1/Stage-2 jobs.
   - `hfs_rces_down/`: **ABSENT ENTIRELY** (no EVLS, no trees). This is the only full production.
     The builder binary SUPPORTS the key (`src/H1CalcSystematic_v6sys.h` family `hfs_rces` →
     `SetShift(eHadEnRCES, ±0.01)`), but `scripts/submit_hfs_v6_systematics.py` scopes it OUT
     (first-wave list = jes_up/down, rces_up, theta_up/down). Leg EVLS differ from central EVLS
     (md5-verified different), so the recorded production path is Stage-1 (EVLS) **and** Stage-2
     (sidistree) with the same `--sys-key` — unless the Stage-2-only equivalence smoke (S5c) passes.
2. **The audit's "3 Stage-2 shifted-MC reruns" is therefore stale as a cost statement:** 1 leg needs
   nothing, 1 leg needs ≤10 jobs, 1 leg needs a full campaign. The remaining heavy work is the
   response/DV/IBU campaign per leg (identical machinery for all three).
3. **The response w2 EXISTS on the current cone basis.** The merged cone responses
   `output_v6/studies/cone_alpha_central_build_2026-10-11/production/migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.root`
   carry `h_R_{pos,neg}` **and** full 2D `h_R_{pos,neg}_w2` (both (491,246); Neff spans 1 … 6.6e6).
   A w2-based response-replica C_MCstat is constructible TODAY without re-reading MC trees.
4. **Signed per-bin custody for the current basis ALREADY EXISTS in the budget component caches:**
   - `component_cache/legs.json`: `ibu_v_sub` (nominal subtracted IBU, 245/charge), `dj_v_coh_sub`
     (DJ coherent model leg), `k3_v_sub`, `k5_v_sub` (iteration legs), plus `B_v`, `data_c`, `f_i_v`,
     `prior_v` — G1's model/reg signed δ_k are direct differences of cached vectors.
   - `component_cache/detector.json`: `legs_rg` holds per-bin SUBTRACTED IBU vectors for **all seven
     detector legs** (`hfs_jes_down`, `hfs_rces_up`, `hfs_theta_up`, `elecE_p1pct`, `elecE_m1pct`,
     `elecTheta_p1mrad`, `elecTheta_m1mrad`), 245/charge, plus envelope-selection metadata
     (`elec_env`) and per-leg integrated values on the clean denominator.
   - `component_cache/tracking.json` (`comp`, `canonical`, `rediag`) and the rederivation records
     carry the signed tracking mean−base and detint model half-diff custody.
5. **The minus-leg campaign machinery already exists** (built for the elec sign-symmetry study):
   `scripts/unfolding/cone_s2prime_shiftedlegs_2026-10-14/make_campaign_files_cone_s2prime_signsym_minus.py`,
   `merge_migration_per_charge_cone_s2prime_signsym_minus.py` (cone-whitelisted merge clone),
   `run_bbb_ibu_signsym_minus_stage3.py`, DV clones, and validators. HFS legs need only a dated
   clone with the leg list and tree-path substitution changed (the plus-leg generator
   `make_campaign_files_cone_s2prime_shiftedlegs.py` already encodes the HFS path-substitution pattern).
6. **The shifted-electron builder is parameterized** (`--shift` presets + `--electron-energy-scale`):
   ±0.5% elecE legs for the two-size linearity check need only a new preset value, no physics change.
7. **Per-event joint charge information exists in the data sidistrees** (`ChargedTot` tree:
   `RunNumber`, `EventNumber`, `Charge` per track) — a joint h+/h− event-level toy design is feasible
   for G4; the retained 1000-toy matrices are NOT retained (only covariance/correlation JSONs), so the
   cross-block is indeed unmeasurable from retained products (audit confirmed).
8. **The current sidecar `nominal` key is empty** and the sidecar is a CENTRAL product — the package
   must NOT edit it (R9 discipline); the package carries its own nominal record sourced from
   `.../bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17/ibu/cross_sections.csv`.

---

## 1. Convention decisions to fix BEFORE build (decision register; supervisor sign-off required)

These are the plan's proposed defaults; each becomes a dated decision record in the package.

| ID | Decision | Proposed default |
|----|----------|------------------|
| D1 | Symmetrization (G7) | Two-sided sources: δ_k = (U_up − U_down)/2, sign kept. Any source with only one side produced: δ_k = signed (U_leg − U_nom), flagged `SAME_DIRECTION_LEG`. The budget's FULL-ABS single-sided / leg-envelope magnitudes are NOT recomputed — the budget quote stays untouched (see §9). |
| D2 | elecE 1-sigma scaling (G3b) | Build ±0.5% legs; if the linearity check passes (per-bin linearity ratio within tolerance), publish the covariance elecE δ_k at the menu 1σ (×0.5 of the ±1% δ) as PRIMARY per the expert prescription, and ALSO publish the budget-convention (±1%) twin column for reconciliation. If linearity fails, keep ±1% and record the deviation. |
| D3 | Model floor sign rule (G6) | Where the B1 floor binds (152/167 bins), the published `ibu_model` δ_k takes the MAGNITUDE from the floor and the SIGN from the signed RG/DJ half-diff; floor-bound bins are flagged per-bin. Rationale: the floor is a replica SD (unsigned by construction); discarding the sign would make the rank-1 term unformable. |
| D4 | C_stat cross-block (G4) | Default Option A (block-diagonal, zero cross-block BY SA-3 CONSTRUCTION, documented as an assumption with the event-level caveat). Option B (event-level joint toys) recommended if the supervisor wants a measured cross-block; costs in S2. |
| D5 | C_MCstat cross-charge block | Zero by construction (per-cell w2 replicas carry no pos/neg cell covariance; the merged TH2s do not retain cross-charge cell covariance). Documented assumption; an event-level response replica campaign is explicitly NOT planned (cost disproportionate). |
| D6 | Significance/smoothing policy (G5) | Per source: z_k = \|δ_k\| / σ_boot(δ_k) from K=50 same-events shard-bootstrap replicas. Pre-registered rule (mirrors the B1 classification): z≥3 real; z<2 noise-dominated. A source with <50% of bins at z≥2 is smoothed (phi_h-style Gaussian, kernel precedent) for the published δ_k column; raw columns retained in custody; no per-bin zeroing without supervisor sign-off. |
| D7 | Tracking correlation form | Diagonal (v4-package TIER_A precedent; 16-seed mean is not a coherent shift; charge-dependent tracking convention caveat stays OPEN). |
| D8 | c_rad_gen form | Rank-1 signed δ_k with the V3 per-z-row Gaussian-smoothed magnitude (bit-consistent with the budget column) and the per-bin sign of the raw RG−DJ half-diff; the raw signed half-diff column also retained. c_rad_stat: diagonal (budget column); the z-row-correlated alternative form from the emfix `covariance_cradstat/` package is carried as a basis-flagged option. |
| D9 | Package tiers | TIER_A: C_stat (sidecar), lumi rank-1, tracking, bgsub_* diagonals, c_rad_stat, phi_h, fakecol, detint_stat. TIER_B: detector-leg, model, reg, detint_model, c_rad_gen rank-1 signed terms (validated by S4 significance before promotion to "validated" tier). TIER_C: C_MCstat (new; validated by S3 closure gates). |

---

## 2. Stage overview

| Stage | Gap | Route | Scale | Long pole? |
|-------|-----|-------|-------|-----------|
| S0 Basis lock + inventory | — | login node | minutes | no |
| S1 Signed δ_k custody columns | G1 | single-node script | minutes | no |
| S2 h+/h− stat cross-block (decision + optional joint toys) | G4 | decision local; Option B Condor | ~660 jobs | no |
| S3 C_MCstat response-replica toys | G2 | Condor 1–2 jobs | ~1–2 h | no |
| S4 Detector-shift significance vs MC noise | G5 | Condor ~250–500 jobs | hours | no |
| S5 HFS minus legs (trees → responses → DV → IBU sub-chain) | G3a | **Condor, HEAVY** | ~2,514 campaign jobs + ≤3,050 rces_down Stage-1/2 + ≤18 completion | **YES** |
| S6 elecE two-size linearity + 1σ scaling | G3b | Condor (reduced scope) | ~1,470 jobs | parallel with S5 |
| S7 Unfolding-own systematic verification (truth-reweight closure) | G6 | 1 Condor job + local | minutes | no |
| S8 Symmetrization + budget reconciliation tables | G7 | single-node script | minutes | no |
| S9 Assembly, C_total, packaging | G1/G7 | single-node script | minutes | no |
| S10 Hygiene: superseded pointers, sidecar-nominal surrogate | P5/P4 | local, gated | minutes | no |
| S11 Critic passes (3) + closure records | — | critics | — | no |

Critical path: S0 → S5 (≈1.5–2.5 days wall including queue) → S8 → S9. S1/S3/S4/S6/S7 run in parallel
with S5. Precedent scale: the plus-leg campaign (870 shard + 3301 DV jobs) and the elec minus-leg
campaign (348 + 1318) each launched and drained within one day (2026-10-14).

**Output roots (all additive; nothing central is ever written):**
- Studies/intermediates: `H1ChargedSidis_HighQ2/output_v6/studies/covariance_cleanrelease_2026-10-XX/`
  (XX = execution date; subdirs `inventory/`, `signed_deltas/`, `response_mcstat_toys/`,
  `shard_bootstrap/`, `joint_stat_toys/`, `truthreweight/`, `reconciliation/`, `logs/`).
- New-leg campaign products: `H1ChargedSidis_HighQ2/output_v6/studies/cone_s2prime_covlegs_2026-10-XX/`
  (dated clone of the shifted-legs study layout; the 2026-10-14 study tree is provenance and stays untouched).
- New scripts: `H1ChargedSidis_HighQ2/scripts/systematics/covariance_cleanrelease_2026-10-XX/`
  and `H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_covlegs_2026-10-XX/` (dated clones only;
  no precedent script modified).
- Final package: `H1ChargedSidis_HighQ2/data/covariance/z005_covariance_cleanrelease_2026-10-XX/`.

---

## 3. Stage details

### S0 — Basis lock and input inventory (login node, light)

**Inputs (absolute, all verified to exist):**
- `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-17/` (`ibu/cross_sections.csv`, `run_record_sub.json`, `covariance/*.json`, `f_i_sidecar/*`)
- Budget dir (25 files): `.../systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17/` (esp. `bgsub_setrprime_budget19_cleandenom.csv`, `component_cache/{legs,detector,tracking,fakecol}.json`, `rederivation_records/*.json`, `carried_components_record.json`, `validation_checks.json`, `provenance.json`)
- Responses: `H1ChargedSidis_HighQ2/output_v6/studies/cone_alpha_central_build_2026-10-11/production/migration_s2prime_cone_{rapgap31,djangoh14}_2026-10-11.root` (sha pins `fda5466f…`/`c84a8c2d…` in the run record)
- c_rad tables: `H1ChargedSidis_HighQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`, `.../rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`, `.../rapgap_crad_releasebin_emfix_2026-10-09/crad_gen_unc_v3_releasebin_emfix_490rows.csv`
- B1 floors: `H1ChargedSidis_HighQ2/output_v6/studies/p1_replica_floors_coherentprior_2026-10-07/ensemble_b1/b1_ibu_model_floors.json`
- phi_h: budget column (carried; provenance `output_v6/systematics/phi_h_coherentprior_2026-10-06/`)
- Tracking: `H1ChargedSidis_HighQ2/output_v6/studies/tracking_eff_m0p5_randdrop_z005_20260702/ensemble_ibu_data4p_v2_20260703/`
- Bin map: `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-15/bin_map.csv`
- HFS tree audit targets: `H1ChargedSidis_HighQ2/output_v6/systematics/hfs/{hfs_jes_up,hfs_theta_down,hfs_rces_down}/`

**Script:** NEW `s00_lock_basis_and_inventory.py` (single node). Emits
`.../covariance_cleanrelease_<date>/inventory/basis_lock.json` with sha256 of every input, the
nominal anchors re-parsed (integrated h+ `10.50881084` / h− `10.21750255` nb asserted to ≤1e-12
against `ibu/cross_sections.csv`), 490-row/980-reco alignment of budget CSV vs bin map vs release,
and the HFS tree 1:1 twin census (per leg: MC twin count, DATA twin count, EVLS count, v6sys stamps).

**Acceptance gates:** all shas recorded; alignment 490/490 charge-major exact; nominal anchors match;
twin census matches §0.1 (theta_down 865/660, jes_up 857/660 with the 8 missing files named,
rces_down 0/0). FAIL → stop, no downstream stage.

**Rollback:** none needed (read-only + one new JSON).

---

### S1 — Signed δ_k columns from existing custody (G1; single node, light)

**Script:** NEW `s01_build_signed_deltas.py`. Reads the S0 inventory + component caches; emits
`.../signed_deltas/delta_k_columns.csv` (490 rows × labelled signed columns, absolute nb AND relative)
plus `signed_deltas_record.json`.

**Column constructions (per source, all on the single denominator):**
- `hfs_jes`, `hfs_rces`, `hfs_theta`: δ = `legs_rg[leg] − ibu_v_sub` (signed; single-sided legs,
  flagged `SAME_DIRECTION_LEG` per D1 until S5 delivers the opposite sides).
- `elec_energy`, `elec_theta`: both signs cached → δ_sym = (plus − minus)/2 (signed, D1); the
  budget-envelope selected-leg column reproduced for reconciliation.
- `ibu_model`: δ = 0.5·(`dj_v_coh_sub` − `ibu_v_sub`)… precisely 0.5·(IBU_DJ − IBU_RG) from
  `dj_v_coh_sub` vs `ibu_v_sub`, with D3 floor-sign handling; per-bin `floor_binds` flag.
- `ibu_reg`: δ = 0.5·(`k5_v_sub` − `k3_v_sub`) (signed).
- `detint_model`: signed half-diff from the detint rederivation custody; `detint_stat` diagonal.
- `tracking`: signed mean−base extracted from `component_cache/tracking.json` custody (carried
  DIAGONAL per D7; signed column published for information only).
- `c_rad_gen`: D8 construction from the emfix sidecar + two releasebin tables.
- Diagonal-only magnitudes (no δ column needed): `phi_h_gauss`, `bgsub_stat/model/stage/fakefrac`,
  `fakecol_mcstat`, `c_rad_stat`, `lumi` (rank-1 peel: δ_lumi = 0.027·σ).

**Acceptance gates (machine):**
- For every budget component column C: `max | |δ_k|/σ_nom − C | ≤ 1e-12` for the directly-signed
  sources (detector legs, reg, tracking, detint, c_rad_stat, bgsub_*, phi_h, fakecol).
- For `ibu_model`: equality on non-floor-bound bins; floor-bound bins reconcile via D3 (flagged,
  counted, no silent mismatch).
- For `elec_*`: the selected-envelope column reproduces the budget column exactly.
- 490 rows, charge-major, finite everywhere; empty cells FORBIDDEN (missing ≠ zero → FAIL).

**Rollback:** additive files only.

---

### S2 — h+/h− stat cross-block (G4; decision + optional Condor)

**Decision (supervisor-gated, record first):**
- **Option A (default, light):** block-diagonal C_stat with the cross-block recorded as
  `ZERO_BY_SA3_CONSTRUCTION` + the documented caveat that event-level h+/h− correlations (tracks in
  the same event) are not captured by the bin-level Poisson-only toy lineage. Cost: zero jobs.
- **Option B (measured, Condor ~660 jobs + 1 toy job):** event-level joint toys.
  1. NEW `s02a_build_event_joint_tables.py` (Condor perfile over the 658 central data sidistrees:
     `H1ChargedSidis_HighQ2/output_v6/central/sidistree/data/**` — per-file sparse
     (event → h+/h− bin occupancy, weights) JSON/parquet; 658 jobs + 1 merge, precedent:
     DV perfile jobs).
  2. NEW `s02b_joint_toy_unfold.py` (1 Condor job): 1000 toys, event-cluster resampling
     (multinomial over events), rebuild the JOINT 980-bin cone data vector per toy, subtract the
     FIXED Set R' (template variance stays in `bgsub_stat_rel` — SA-3 distinctness preserved),
     unfold each charge IBU k=4 with the engine; seeds NEW and recorded (e.g. 32345/42345);
     explicit lineage statement that this SUPERSEDES SA-3 for the cross-block only.
  3. Validation: per-charge diagonal blocks must agree with the sidecar within toy tolerance
     (quantifies the event-clustering excess over pure Poisson); cross-block measured and published
     with its toy error; if per-charge blocks deviate beyond pre-registered tolerance, the
     discrepancy is published, not tuned away.

**Output:** `.../covariance_cleanrelease_<date>/joint_stat_toys/` (Option B) or the recorded
assumption in the package README (Option A).

**Rollback:** additive; the sidecar is never modified.

---

### S3 — C_MCstat body/miss via response-replica toys through IBU (G2; Condor 1–2 jobs)

**Method decision: response-MC toys through IBU (primary), analytic Jacobian retained as the
G12-style cross-check slice.** This closes the audit's one GAP (row 20) and re-bases the
~1.48/1.58% bounded estimate onto the clean release.

**Script:** NEW `s03_response_mcstat_toys.py` (single Condor job; precedent runtime: the official
1000-data-toy run cluster 5658880). Spec:
1. Read `h_R_{pos,neg}` + `h_R_{pos,neg}_w2` ((491,246)) from the merged cone rapgap31 response
   (governing generator), plus `h_prior_gen_*`, `h_prior_gen_*_NWeff`, fake-column w2 pieces.
2. Build K=200 replicas (seed fixed, recorded): per cell, sample the cell content from its
   w2-implied distribution — Neff = (Σw)²/Σw²; Poisson(Neff)·(Σw/Neff) for Neff ≥ 10; cells with
   smaller Neff kept fixed and flagged (avoids fabricating large relative noise on
   single-effective-event cells). Replica-own coherent prior (body+miss) and replica-own f_i
   recomputed per replica (B1 replica-own-prior precedent).
3. Unfold `data_sub'` through IBU k=4 per replica per charge; emit toy matrix (K×245 per charge)
   and the derived covariance.
4. Also emit the fake-column-only toy slice (perturb only the fake column + body-row w2 per G12)
   for the closure check against `fakecol_mcstat_rel`.

**Acceptance gates:**
- Closure vs G12: fake-column-only slice diagonal vs `fakecol_mcstat_rel` budget column —
  medians agree within pre-registered tolerance (target ≤20% relative on medians; the G12
  construction is a Jacobian linearization, the toys are not, so exact equality is not expected —
  the tolerance and the sign of the difference are recorded).
- Diagonal magnitude vs the STATE-carried bounds (1.4769%/1.5752% medians, dossier basis):
  same order, re-based numbers published.
- Toy-mean bias vs nominal ≤ pre-registered bound (unfolding bias check).
- PSD, symmetric, finite; block-diagonal cross-charge per D5 (recorded assumption).
- K-sensitivity: repeat at K=100; diagonal stability reported.

**Output:** `.../response_mcstat_toys/{toys_pos.npy, toys_neg.npy, cov_mcstat.json, closure_report.json}`.

**Rollback:** additive.

---

### S4 — Significance of each detector shift vs MC statistical noise (G5; Condor ~250–500 jobs)

**Method: same-events shard-level bootstrap.** Nominal and shifted responses are built from the SAME
865 MC files in identical sharding (1:1 twins, verified in the 2026-10-14 campaign for the plus legs;
re-verified for new legs in S5). Resampling shards with replacement = same-events replica.

**Script:** NEW `s04_shard_bootstrap_significance.py` (campaign generator + merge driver):
- For each of the 5 detector legs (governing rapgap31 chain: 73 shards per leg; nominal shards from
  `output_v6/studies/cone_alpha_central_build_2026-10-11/production/` — exact shard paths confirmed
  in S0): K=50 replicas; per replica: resample the 73 shard indices with replacement, merge
  nominal-response-replica and shifted-response-replica from the SAME shard draw (exposure-rescaled
  to nominal), rebuild replica priors/f_i, unfold `data_sub'` (nominal-replica) and the leg DV
  (shifted-replica) through IBU k=4, form δ_k^r; take σ_boot(δ_k) across replicas.
- Batch shape: ~50 replicas × 5 legs × (2 merges + 2 unfolds) ≈ 500 short jobs, or ~50 batched jobs;
  either fits in hours.
- Decision rule D6 applied per source; output per-source noise classification
  (`frac z≥2`, `frac z≥3`, per-bin z CSV) and the smoothing recommendation.

**Acceptance gates:** replica-count stability (repeat 20 replicas on one leg, σ_boot stable within
tolerance); z-distribution sane (no >50% z<1 for a source whose budget median ≫ C_MCstat diagonal);
report feeds D6 and the TIER_B→validated promotion.

**Output:** `.../shard_bootstrap/<leg>/...` + `significance_report.json`.

**Rollback:** additive.

---

### S5 — HFS minus legs (G3a; Condor, HEAVY — the long pole)

Target legs (opposite sides of the budget's installed single-sided legs):
`hfs_jes_up` (opposite of installed `hfs_jes_down`), `hfs_rces_down` (opposite of `hfs_rces_up`),
`hfs_theta_down` (opposite of `hfs_theta_up`).

**S5a. Tree audit + smokes (login node first, then ≤18-job Condor):**
- Re-verify 1:1 twins + `v6sys_key` stamps for `hfs_theta_down` (complete) and `hfs_jes_up`
  (8 missing MC sidistrees; check which of the 8 have existing EVLS — EVLS dir is 2 short).
- Single-file smokes on the precedent units (`djangoh14_7091_ep0607_0200` MC / `ep0607_d06_0000`
  data): response builder over one `hfs_theta_down` tree (UNMODIFIED cone builder
  `scripts/unfolding/cone_s2prime_2026-10-11/build_migration_th2_per_charge_cone_s2prime.py`),
  expecting the stored `Yes` shifted DOWN and Q2es-invariant behaviour per the plus-leg precedent.
  Gates mirror the campaign plan §6 (count identity 0/245 violations ×2, gen side bit-identical).

**S5b. `hfs_jes_up` completion (≤10 jobs):** run `TreeBuilderHighQ2_chargedsidis_v6` +
`ChargedSidisTreeBuilderHighQ2_v6` with `--sys-key hfs_jes_up` for the missing files only
(submitting via a dated clone of `scripts/submit_hfs_v6_systematics.py` scoped to a file list;
inputs `/pnfs/desy.de/dphep/online/h1/{data,mc2}/oo-4.0/` — currently readable, flagged unreliable
since 2026-05-22; the EVLS gap for ≤2 files resolved first). Acceptance: twin census 865/865 + 660/660.

**S5c. `hfs_rces_down` production (up to ~3,050 jobs):**
1. **Equivalence smoke (decision gate, login node):** Stage-2 `--sys-key hfs_rces_down` over a
   CENTRAL EVLS file vs over a freshly Stage-1-produced shifted EVLS file; if the resulting
   sidistrees/responses are bit-identical (plausible: the recorded plus-leg verification showed
   Q2es/Empz/xF/tracks bit-identical, the shift acting through stored `Yes`/`Xes`), then run
   **Stage-2 only** over the 1525 central EVLS (1525 jobs). Otherwise full production:
   Stage-1 EVLS (1525 jobs) → Stage-2 sidistree (1525 jobs), same `--sys-key` both stages
   (recorded policy of `submit_hfs_v6_systematics.py`; scope extension = dated clone, submitter
   and precedents untouched).
2. Acceptance per file: `v6sys_key=hfs_rces_down` stamp; twin census 865/865 MC + 660/660 data.

**S5d. Response + DV campaign for the three legs (≈2,514 jobs):**
- NEW dated campaign generator `scripts/unfolding/cone_s2prime_covlegs_<date>/make_campaign_files_covlegs.py`
  — clone of `make_campaign_files_cone_s2prime_signsym_minus.py` with `ALL_LEGS =
  ('hfs_jes_up','hfs_rces_down','hfs_theta_down')`, HFS tree-path substitution
  `.../central/sidistree/mc/...` → `.../systematics/hfs/<leg>/sidistree/mc/...` + `_<leg>.root`
  suffix (pattern already encoded in the plus-leg generator), DV streams over the legs' shifted DATA
  trees (660 perfile + 1 merged per leg), shard memory 28000 MB / DV 2000 MB per precedent.
- Merge via the existing cone-whitelisted clone `merge_migration_per_charge_cone_s2prime_signsym_minus.py`
  (dated copy; whitelists already cover the cone TH1/meta keys).
- Per-leg totals: 174 shard jobs (73 rapgap + 101 djangoh) + 661 DV jobs + ~3 merge/check jobs
  → ×3 legs ≈ 2,514 jobs. Precedent drain rate: one day for ~4,200 jobs.
- Outputs under `output_v6/studies/cone_s2prime_covlegs_<date>/{shards,merged,data_vector}/`
  (NEVER central; the 2026-10-14 study tree untouched).

**S5e. Stage-3 subtracted-chain IBU for the new legs (1 Condor job):**
- Dated clone of `run_bbb_ibu_signsym_minus_stage3.py` with `LEGS`/`LEG_KIND`/`DET_COLUMN` extended
  (the `hfs` kind path exists in the plus-leg Stage-3 `run_bbb_ibu_shiftedlegs_stage3.py`), producing
  the UNSUBTRACTED shifted IBU on the cone basis; then the subtracted-chain re-derivation via a dated
  clone of the budget driver's `build_variant_chain` (from
  `run_bgsub_budget19_cleandenom_cone.py` + `common_bgsub_setr_cone.py`) emitting `legs_rg`-style
  SUBTRACTED vectors for the three new legs on the clean denominator.
- Acceptance: nominal gate (cone nominal anchors reproduce), sub_guard per leg (Set R' applied
  exactly once, same B_R′ custody), integrated values recorded, 245×2 finite, 0 negative bins.

**S5f. HFS sign-symmetry record v2 (light):** dated clone of
`compare_signsym_minus_vs_plus_stage4.py` → flag criteria as pre-registered (0.05 pp primary);
record under `output_v6/studies/cone_s2prime_covlegs_<date>/hfs_signsym_record_<date>.{json,md}`.
Expected wall/queue total for S5: ≈1.5–2.5 days (≈2,514 campaign jobs + up to ≈3,050 Stage-1/2 jobs
for rces_down, minus the Stage-2-only shortcut if the S5c smoke passes).

**Rollback:** all products in new dated study trees; no central or budget file touched; failed
clusters leave the trees partial — rerun is additive.

---

### S6 — elecE two-size linearity check and menu-1σ scaling (G3b; Condor, reduced scope ≈1,470 jobs)

- New presets `elecE_p0p5pct` / `elecE_m0p5pct` (`--electron-energy-scale ±0.005`) in a dated clone
  of the shifted builder (`build_migration_th2_per_charge_cone_s2prime_shifted.py` is already
  parameterized; preset table addition only) + the shifted DV clone.
- **Reduced scope (proposed):** governing rapgap31 chain only — 2 legs × (73 response shards +
  658 DV perfile + 1 DV merge) ≈ 1,464 jobs + stage-3 (1 job). Full-scope twin (both generators,
  ≈2,940 jobs) only if the supervisor requires the DJ diagnostic.
- Analysis (light): per-bin linearity ratio `|δ_±0.5%| / (0.5·|δ_±1%|)`; integrated ratio; symmetric
  ratio stability across strata. Pre-registered pass: median ratio in [0.9, 1.1] and no stratum
  >20% out → D2 scaling applied (publish elecE δ_k at 1σ primary + budget-convention twin).
- Note: elec_theta ±1 mrad and all HFS magnitudes match the frozen menu 1σ (no scaling, no
  linearity production).
- Output: `.../covariance_cleanrelease_<date>/elecE_linearity/`.

---

### S7 — Unfolding-own systematic verification on the current basis (G6; 1 Condor job + local)

1. `ibu_reg` and `ibu_model` signed δ_k: S1 (done from custody).
2. **Truth-reweight-to-result closure (currently absent anywhere in the chain):** NEW
   `s07_truthreweight_closure.py` — prior := per-charge SMOOTHED IBU result on `data_sub'`
   (the coherent-prior study `run_coherent_prior_study.py` variant machinery as precedent),
   re-unfold IBU k=4, quote per-bin and integrated change vs nominal. Gate: change inside the
   `ibu_model` envelope (base or floor per bin) in ≥95% of bins → closure PASS, recorded; else
   emit a new signed `prior_reweight` δ_k column REPORT-ONLY and escalate to the supervisor
   (adding a budget component is out of scope for this package).
3. B1 floor mapping: re-assert the budget's machine check (`b1_floor_mapping_vs_canonical_budget`,
   dev 0) as an input gate; carry the `b1_aggregation_status: FAIL` report-only flag forward.
4. `phi_h`: stays a diagonal smoothed term (prescription point 9); smoothing provenance carried.

**Output:** `.../truthreweight/truthreweight_closure.json`.

---

### S8 — Symmetrization assembly + budget reconciliation (G7; single node, light)

**Script:** NEW `s08_symmetrize_and_reconcile.py`:
- Forms the final per-source δ_k per D1/D2/D3 from S1 columns + S5 legs + S6 scaling + S4 smoothing
  recommendation (smoothing applied only per D6 with the raw retained).
- Emits the **reconciliation table** (the deliverable that makes the convention deviation auditable):
  per source × per bin, budget column magnitude vs package term magnitude
  (`sqrt(diag(δδᵀ))` or diagonal), with deviation classified into the named convention causes
  (symmetrization; elecE 1σ scaling; envelope→signed; floor-sign rule). Any deviation NOT explained
  by a named cause = FAIL.
- Confirms (machine) that no budget file was modified: sha256 of all 25 budget files before/after.

**Acceptance:** reconciliation clean or explained; quadrature cross-check: the package's per-bin
total (without lumi, with budget-convention twins) reproduces `sqrt(total_with_bgsub19_rel² − lumi²)`
within the documented convention deviations only.

---

### S9 — Assembly, C_total, packaging (single node, light)

**Script:** NEW `s09_build_covariance_package.py` — dated re-pointing of the validated v4 builder
(`sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/scripts/build_covariance_package.py`)
to the clean basis, extended per the prescription:

**Package contents** (`H1ChargedSidis_HighQ2/data/covariance/z005_covariance_cleanrelease_<date>/`):
- `cov_stat.npy` + `rel_cov_stat.npy`: 490×490 from the sidecar per-charge blocks (+ measured
  cross-block if S2 Option B; else block-diagonal per D4). Sidecar matrices ALSO re-published as
  tables (prescription: bootstrap matrices as tables).
- `cov_mcstat.npy` + rel twin: from S3 (+ D5 assumption recorded).
- Per-source rank-1 terms: `cov_<source>_rank1.npy` for `hfs_jes`, `hfs_rces`, `hfs_theta`,
  `elec_energy` (1σ primary + budget-convention twin), `elec_theta`, `ibu_model`, `ibu_reg`,
  `detint_model`, `c_rad_gen`; each with a diagonal-fallback twin (v4 schema).
- Rank-1 correlated peel: `cov_lumi.npy` = 0.027²·σσᵀ (published separately AND in C_total_variant).
- Diagonal terms: `cov_tracking`, `cov_phi_h`, `cov_bgsub_{stat,model,stage,fakefrac}`,
  `cov_fakecol_mcstat`, `cov_detint_stat`, `cov_crad_stat`.
- `delta_k_columns.csv`: every δ_k as a labelled SIGNED column (absolute nb + relative), with
  per-bin flags (`same_direction_leg`, `floor_binds`, `smoothed`, `menu_1sigma_scaled`).
- `C_total` variants: `cov_total_with_lumi.npy`, `cov_total_no_lumi.npy`
  (= C_stat + C_MCstat + Σδδᵀ + diag), `cov_total_diagonalB.npy` (diagonal-fallback variant,
  v4 precedent), each + rel twins.
- `bin_map.csv` (copied from the Born clean-release dir; 490 rows), `manifest.json` (every input
  path + sha256, seeds, K counts, lineage statements), `README.md` (basis, conventions D1–D9,
  near-singularity caveat, unpropagated sources), `validation.json`, `unpropagated_sources.json`
  (trigger placeholder; DIFFVM/photoproduction register rows 18/19; e∩V0 ignored-by-decision;
  NucliaWeight r_min proxy; lowQ2 α≥0.2 rebuild; DjBkgd provenance — each labelled unevaluated /
  bounded, NEVER zero).

**Validation gates (all machine, all must PASS):**
1. Every matrix: 490×490 float64, finite, exactly symmetric, diagonal ≥ 0, PSD
   (eigvalsh ≥ −1e-12·scale), rank recorded.
2. Near-singularity report: eigenvalue spectrum of C_total + explicit caveat block (rank-1 terms).
3. Diagonal consistency vs budget: per-source `sqrt(diag(C_k))/σ` vs budget column per S8's
   reconciliation rules.
4. Denominator discipline: all relative inputs re-based to the official IBU CSV nominal;
   integrated anchors h+ `10.50881084` / h− `10.21750255` asserted; sidecar `nominal`-key
   workaround documented (package carries its own nominal record; sidecar NOT edited).
5. Bin alignment: 490 rows charge-major vs bin_map and budget CSV; reco-side 980 convention
   documented for consumers.
6. Total-vs-stat guidance: median total comparable to stat (expect ~1.6× with lumi; report both).
7. No central file modified: pre/post sha sweep of the budget + release trees (857-file-style proof
   confined to reads).

**Rollback:** package is a new dated directory; delete-to-rollback is safe (nothing else references it
until critics pass).

---

### S10 — Hygiene (gated, local, additive-only)

- `README_SUPERSEDED_<date>.md` pointers added to the three stale covariance packages
  (`H1ChargedSidis_HighQ2/data/covariance/z005_supplement_modelreg_coherentprior_2026-10-07/`,
  `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/covariance_cradstat/`,
  `sidis_highQ2/data/covariance/z005_provisional_v4_2026-09-23/`) naming the new package —
  additive files, executed only AFTER S9 validation passes and critics endorse.
- The empty `sidecar.nominal` key is NOT edited (central product; R9). The package README records
  the authoritative nominal source instead.

---

### S11 — Critic passes (3) and closure

Three critic passes follow the build. Evidence each critic needs (all paths absolute, all produced
by the stages above):

1. **Definitions/conventions critic (cold):** the 2026-10-17 audit; this plan; the D1–D9 decision
   records; `signed_deltas_record.json`; the S5f HFS sign-symmetry record; the S6 linearity report;
   the reconciliation table (S8); the package README. Key questions: symmetrization rule vs the
   supervisor-signed FULL-ABS budget convention (no silent mixing); D3 floor-sign rule; D2 dual
   elecE columns; D4/D5 zero-by-construction blocks clearly labelled assumptions, not measurements.
2. **Construction/reproduction critic (hot):** `basis_lock.json` (input shas); S1 acceptance output
   (per-column budget reproduction at 1e-12); S3 closure report (fakecol slice vs G12; bounded
   1.48/1.58% re-based; toy bias; K-stability); S4 significance report (z-classification, σ_boot
   stability); S5 custody manifests (stage-3 sub_guard, twin censuses, cluster IDs);
   `validation.json` (PSD/rank/symmetry/alignment); seeds and K counts everywhere; the
   pre/post sha sweep proving central products untouched.
3. **Publication-readiness critic:** `C_total` variants + near-singularity caveat;
   `unpropagated_sources.json` (nothing silently zero); the budget-quote separation statement
   (§9); `bin_map.csv` + units + ordering documentation for global fitters; superseded pointers
   (S10); the reconciliation table; explicit statement of which terms are TIER_B pending S4
   validation.

Closure: STATE.md/handoff update (supervisor-owned), recording the package as the current-basis
covariance product and the superseded-pointer state.

---

## 4. Gap → stage coverage map

| Gap (audit) | Stage(s) | Status after build |
|---|---|---|
| G1 no current-basis signed δ_k / C_total | S1, S8, S9 | closed (custody deltas immediate; HFS signed after S5) |
| G2 C_MCstat body/miss unevaluated | S3 (+S4 cross-check) | closed as a measured C_MCstat with closure gates; D5 cross-charge assumption documented |
| G3 HFS minus legs missing; elecE 1σ alignment | S5 (a–f), S6 | closed: theta_down needs no tree production; jes_up ≤10 jobs; rces_down full production; elecE scaled to menu 1σ only after the linearity check |
| G4 h+/h− stat cross-block unmeasurable | S2 | decision recorded; Option A (documented assumption) or Option B (measured, ~660 jobs) |
| G5 significance vs MC noise | S4 | closed for all 5 detector legs (same-events shard bootstrap, pre-registered rule D6) |
| G6 unfolding-own systematic | S1, S7 | model/reg signed from custody; truth-reweight closure NEW in S7; B1 floor sign rule D3 |
| G7 symmetrization + budget reconciliation | S8, S9 | closed: rule fixed (D1), reconciliation table published, budget quote untouched |

---

## 5. Risk register

| Risk | Stage | Mitigation |
|---|---|---|
| `/pnfs` raw-input unreliability (flagged 2026-05-22; readable today) | S5b, S5c | Stage-2-only shortcut smoke (S5c.1) removes the EVLS rebuild if it passes; file-level resume; if /pnfs fails, hfs_rces_down δ_k stays `SAME_DIRECTION_LEG`-flagged and the package ships with the documented deviation (audit discipline: unevaluated ≠ zero) |
| hfs_rces_down Stage-1→Stage-2 dependency ordering | S5c | recorded policy (both stages same sys-key); the equivalence smoke may legitimately collapse this |
| C_MCstat replica noise on tiny-Neff cells | S3 | Neff≥10 threshold + fixed-cell flagging; K-sensitivity check; closure vs G12 |
| Symmetrized magnitudes diverge from budget beyond convention causes | S8 | reconciliation table FAILs the build; escalation to supervisor, no silent publication |
| elecE linearity fails | S6 | D2 fallback: keep ±1%, record deviation, publish both columns unscaled |
| Joint-toy per-charge blocks disagree with sidecar (S2 Option B) | S2 | publish the discrepancy; per-charge sidecar blocks remain the TIER_A stat term; cross-block published with its own lineage |
| Queue congestion for the ~2.5k-job campaign | S5d | precedent drained 4.2k jobs in a day; submit legs sequentially, rces_down trees first |

**Single riskiest stage: S5c/S5d (hfs_rces_down full production + the 3-leg campaign)** — it is the
only stage with a raw-input (/pnfs) dependency, a Stage-1→Stage-2 ordering constraint, and it gates
the signed HFS δ_k columns; everything else is either light or has direct precedent.

---

## 6. Budget quote vs covariance package — separation (CONFIRMED, with flags)

- The **public systematic quote stays `total_with_bgsub19_rel`** on the 19-component clean-denominator
  budget (medians h+ 5.9764% / h− 6.2968%), per the 2026-10-17 promotion quoting rule. The covariance
  package is a **separate deliverable** under `H1ChargedSidis_HighQ2/data/covariance/…` with its own
  convention set (signed symmetrized δ_k, menu-1σ elecE pending D2, peeled rank-1 lumi/c_rad).
  No central release, budget, or sidecar file is edited at any stage. **No conflict exists** — the
  promotion's quoting rule governs public per-bin totals; the package governs covariance consumers.
- **Flags that make the separation honest:** (i) the package's C_total diagonal will NOT equal
  `total_with_bgsub19_rel` per bin — the S8 reconciliation table quantifies every difference and
  attributes it to a named convention; (ii) the elecE 1σ scaling (D2) and the symmetrization (D1)
  are convention deviations from the supervisor-signed budget conventions and REQUIRE supervisor
  sign-off before the package is labelled publishable; (iii) the package README must state that the
  near-singular rank-1 construction is a variation-envelope form (explicit 100% coherence
  assumption), per the v4/supplement precedent wording.

---

## 7. Execution order and wall-clock summary

1. **Day 0 (hours):** S0 → S1; S2 decision; S3 submission; S4 submission; S7 submission.
   Submit S5c smoke + S5b/S5c production and the S5d campaign generator the same day (long pole).
2. **Day 1–2.5:** S5d campaign drains; S6 submitted in parallel; S5e/S5f; S4/S3 analyses land.
3. **Day 2.5–3:** S8 → S9 → S10 (gated) → S11 critics.

Heavy (Condor) vs light:
- **Condor:** S2 Option B (~660 jobs), S3 (1–2 jobs), S4 (~250–500 jobs), S5 (≈2,514 campaign jobs
  + up to ≈3,050 hfs_rces_down Stage-1/2 jobs + ≤18 completion jobs), S6 (≈1,470 reduced-scope jobs),
  S7 (1 job), S5e (1 job).
- **Single-node scripts:** S0, S1, S2-decision, S5a smokes, S5f analysis, S6 analysis, S8, S9, S10,
  all validation/report assembly.

*Read-only planning product. No central release, budget, sidecar, or study product was modified;
no Condor jobs were submitted by this task.*
