# REPRODUCING_THE_ANALYSIS.md — H1 high-Q² charged-particle SIDIS

**Date:** 2026-05-28
**Audience:** A physicist who understands the analysis content (SIDIS multiplicities, unfolding, asymmetries) but has not worked on this codebase. You should be able to run the chain end-to-end by typing the commands below.
**Status of result:** Data release v1.2_final is on disk (`data_release_v1p2_final/`, 21 files, 41 MB). This is the canonical result. The v1.0 preliminary release and v1.1 intermediate release are retained for audit history only.

**Companion documents:**
- `docs/SCRIPTS_MANIFEST.md` — the categorized table of every script.
- `context-handoff.md` — current state, known traps, pending work.
- `data_release/README.md` — what the public-face package contains.

---

> **Quarantine note (2026-05-28):** 10 stale docs moved to `quarantine/docs/` —
> 2 handoffs (`decision-binning-amendment-HQv5fr2.md`,
> `handoff-HQv5fr2-rebuild-on-naf.md`), 6 methodology docs (`crad_v5f_methodology_FROZEN.md`,
> `session_state_2026-05-25.md`, `TUnfold_uniform_low_diagnosis_2026-05-24.md`,
> `TUnfold_scan_2026-05-25.md`, and 2 intermediate binning studies), plus
> 4 intermediate binning docs. Only `binning_adoption_HQ-v5g-Bprime_2026-05-24.md`
> and `binning_freeze_2026-05-24.md` are retained as durable decision records.
> References to quarantined files in §6 are annotated accordingly.

---

## 0. v3final canonical chain (2026-05-28)

**This is the chain that produced data release v1.2_final.** All 12 steps
use the Option C fiducial: `xF_Trento > 0 AND |GenEta| < 4 AND
35 < GenEmpz < 70 AND event cuts`. The five new driver scripts live in
`scripts/v3final/`. Steps 1–6 are identical to the historical v3p1/v3p2
chain in §§3–4 below; the divergence begins at step 7 (trust mask uses a
v3p2-namespaced script) and steps 8–12 (all v3final drivers).

### Step 1 — Extract (NAF Condor)

4 extractors covering data + MC (gen, reco, disevent):

```bash
# On NAF, in H1ChargedSidis_HighQ2/scripts/extract_pertrack_v5g_fixedFrame/
condor_submit submit_pertrack_v5g_fixedFrame_v3p2.condor   # data
# On NAF, in H1ChargedSidis_HighQ2/scripts/extract_pertrack_mc_v3/
condor_submit submit_pertrack_mc_v3p2.condor               # MC gen (Option C fiducial)
condor_submit submit_pertrack_mc_v3p1.condor               # MC reco + disevent
```

Extractors: `extract_pertrack_v5g_fixedFrame_v3p2.py` (data),
`extract_pertrack_mc_gen_v3p2.py`, `extract_pertrack_mc_reco_v3p1.py`,
`extract_pertrack_mc_disevent_v3p1.py`. After jobs finish, merge and pull
the 7 merged CSVs to `H1/sidis_highQ2/data/`. Wall time: ~2–3 hr Condor
+ ~40 min merge.

**Outputs:** 7 merged CSVs in `data/` and `data/mc_v3/`.

### Step 2 — Build response matrices (local)

```bash
cd H1/sidis_highQ2
python scripts/unfolding/build_response_4d.py \
    --data-csv    data/pertrack_v5g_fixedFrame_v3p2_merged.csv.gz \
    --mc-reco-csv data/mc_v3/pertrack_mc_v3p1_reco_djangoh14_merged.csv.gz \
    --mc-gen-csv  data/mc_v3/pertrack_mc_v3p2_gen_djangoh14_merged.csv.gz \
    --out         data/response_4d_djangoh_v3final.npz
# repeat with rapgap CSVs → data/response_4d_rapgap_v3final.npz
```

`data/response_4d_{djangoh,rapgap}_v3final.npz` are symlinked from the
v3p2 originals (byte-identical). Wall time: ~5 min each.

**Outputs:** `data/response_4d_{djangoh,rapgap}_v3final.npz`.

### Step 3 — Nominal unfolds (local)

```bash
python scripts/unfolding/unfold_BBB_4d.py    --response data/response_4d_djangoh_v3final.npz \
    --out data/unfolded_4d_v3final_BBB.json
python scripts/unfolding/unfold_IBU_4d.py    --response data/response_4d_djangoh_v3final.npz \
    --out data/unfolded_4d_v3final_IBU.json \
    --cov-out data/unfolded_4d_v3final_IBU_covariance.npz
python scripts/unfolding/unfold_TUnfold_4d.py --response data/response_4d_djangoh_v3final.npz \
    --out data/unfolded_4d_v3final_TUnfold.json \
    --cov-out data/unfolded_4d_v3final_TUnfold_covariance.npz
    # τ = 0.10 baked in
```

Wall time: BBB ~10 s, IBU ~30 s, TUnfold ~1 min.

**Outputs:** `data/unfolded_4d_v3final_{BBB,IBU,TUnfold}.{json,npz}`.

### Step 4 — 4-term unfolding budget (local + NAF Condor for σ_MCstat)

```bash
# σ_reg (local, ~5 min)
python scripts/unfolding/sigma_reg_TUnfold_4d.py \
    --response data/response_4d_djangoh_v3final.npz \
    --out      data/sigma_reg_TUnfold_4d_v3final.json

# σ_MCstat — 100-replica bootstrap (~9 min wall on 12 GB Condor)
# On NAF, in scripts/unfold4d/
bash wrap_sigma_mcstat_v3p2.sh        # sets env
condor_submit submit_sigma_mcstat_v3p2.condor
# Pull result:

# σ_model (local, ~3 min)
python scripts/unfolding/unfold_data_4d_DJ_vs_RG.py \
    --data-csv    data/pertrack_v5g_fixedFrame_v3p2_merged.csv.gz \
    --response-dj data/response_4d_djangoh_v3final.npz \
    --response-rg data/response_4d_rapgap_v3final.npz \
    --out         data/unfolded_4d_v3final_data_DJvsRG.json
```

σ_stat = analytic diagonal of `unfolded_4d_v3final_TUnfold_covariance.npz` (step 3).

**Outputs:** `data/sigma_reg_TUnfold_4d_v3final.json`,
`data/sigma_MCstat_TUnfold_4d_v3final.json`,
`data/unfolded_4d_v3final_data_DJvsRG.json`.

### Step 5 — 6 detector systematics (NAF Condor + local re-unfold)

```bash
# On NAF, in scripts/systematics/
condor_submit submit_tracking_efficiency_v3p2.condor
condor_submit submit_tracking_resolution_v3p2.condor
condor_submit submit_electron_systematic_v3p2.condor
# Wait. Then run re-unfold drivers on NAF:
python run_tracking_efficiency_systematic.py
python run_tracking_resolution_systematic.py
python run_electron_systematic.py
# Plus charge-dependent tracking (analytic, local):
python scripts/systematics/source_tracking_chargedep.py
# Pull 6 source JSONs + 6 covariance NPZs to data/systematics_sources_v3final/
```

Wall time: ~1 hr Condor + minutes re-unfold.

**Outputs:** `data/systematics_sources_v3final/source_{tracking_efficiency,
tracking_chargedep,tracking_resolution,electron_energy,electron_theta,
electron_phi}.json` + `cov_*.npz`.

### Step 6 — Noise floor (NAF Condor)

20+20 Poisson replicas (data-mode + MC-mode):

```bash
# On NAF
condor_submit submit_noise_floor_v3p2.condor    # 40 jobs
# Aggregate (NAF or local):
python compute_noise_floor.py \
    --data-glob 'noise_floor_replicas/noise_floor_data_*.json' \
    --mc-glob   'noise_floor_replicas/noise_floor_mc_*.json' \
    --out-json  data/systematics_sources_v3final/noise_floor.json \
    --out-npz   data/systematics_sources_v3final/cov_noise.npz
```

Wall time: ~20 min Condor + seconds aggregate.

**Output:** `data/systematics_sources_v3final/noise_floor.json` +
`cov_noise.npz`.

### Step 7 — Trust mask (local)

```bash
python scripts/v3p2/diag_catastrophic_4D_bins_v3p2.py
# → data/v5g_4d_trust_mask_v3final.json  (367/480 trusted)
```

Wall time: ~30 s.

**Output:** `data/v5g_4d_trust_mask_v3final.json`.

### Step 8 — Combine systematics (local, v3final driver)

Uses `scripts/v3final/combine_systematics_v3final.py` — an inline
replacement for the generic combiner. The v3final source JSONs in
`data/systematics_sources_v3final/` do not carry the older v3 contract
schema, so the canonical `systematics/combine_systematics.py` cannot be
used here without modification.

```bash
python scripts/v3final/combine_systematics_v3final.py
# → data/systematics_combined_v3final.json
```

Wall time: seconds.

**Output:** `data/systematics_combined_v3final.json`.

### Step 9 — Apply noise correction (local, v3final driver)

σ_sys_corr = sqrt(max(σ_sys² − 2σ_noise², 0)) applied per bin.

```bash
python scripts/v3final/apply_noise_correction_v3final.py
# inputs:  data/systematics_combined_v3final.json
#          data/systematics_sources_v3final/noise_floor.json
# → data/systematics_combined_v3final_noise_corrected.json
```

Wall time: seconds.

**Output:** `data/systematics_combined_v3final_noise_corrected.json`.

### Step 10 — Cross sections (local, v3final driver)

dσ^h/(dQ²dxdzdpT_h) = N_unfolded / (L × Δ4D).
L = 351,000.0 nb⁻¹ = 351 pb⁻¹ (current canonical SIDIS 4-period S67 quote including ep0304; supersedes the older direct-monitor 356,613 nb⁻¹ quote). Units: nb·GeV⁻³.

```bash
python scripts/v3final/compute_xsec_v3final.py
# inputs:  data/unfolded_4d_v3final_TUnfold.json
#          data/response_4d_djangoh_v3final.npz  (bin widths)
# → data/xsec_v3final.json
```

Wall time: seconds.

**Output:** `data/xsec_v3final.json`.

### Step 11 — A_h asymmetry (local, v3final driver)

240 (cell, z, pT_h) bins; 181 in the published set (trust-mask filtered).
Cancels-in-A_h logic suppresses sources that drop out of the ratio.

```bash
python scripts/v3final/compute_Ah_v3final.py
# inputs:  data/unfolded_4d_v3final_TUnfold.json
#          per-source covariance NPZs (cancels_in_A_h logic)
# → data/Ah_v3final.json
```

Wall time: seconds.

**Output:** `data/Ah_v3final.json`.

### Step 12 — Build data release v1.2_final (local, v3final driver)

```bash
python scripts/v3final/build_data_release_v1p2_final.py
# inputs:  data/xsec_v3final.json
#          data/Ah_v3final.json
#          data/v5g_4d_trust_mask_v3final.json
#          data/systematics_combined_v3final_noise_corrected.json
#          per-source cov NPZs
#          schema/examples from v1.1 (data_release_v1p1/)
# → data_release_v1p2_final/  (21 files, 41 MB)
```

Wall time: ~1 min.

**Output:** `data_release_v1p2_final/` — 21 files, 41 MB. Verify:

```bash
cd data_release_v1p2_final && sha256sum -c MANIFEST.sha256
```

---

## 1. Prerequisites

### 1.1 NAF access

- Active Kerberos ticket (`kinit klesthen@DESY.DE`) at submit time. Tickets expire after ~24 h; re-issue before any long-running babysitter dispatch.
- DUST storage area: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/`.

### 1.2 NAF environment (for any Condor or compile step)

```bash
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
```

Compile with `make bin`, NOT `make all`. For the current central production use v6 sources and products (`src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C`, `src/ChargedSidisTreeBuilderHighQ2_v6.C`, `condor_v6/central/`, `output_v6/central/`, `evls_v6_*.root`). The older v5g binary pair documented in historical manifests is audit-only and must not be used for current z005 production.

Before ANY production submission, run the guardrail check:

```bash
python H1/sidis_highQ2/scripts/_binary_guard.py
python H1/sidis_highQ2/scripts/_smoke_gate.py
```

If either raises, STOP and resolve the manifest mismatch before continuing.

### 1.3 Local Python environment

Python 3.10+ with: `numpy`, `scipy`, `pandas`, `matplotlib`, `uproot`, `h5py`. No CUDA, no GPU, no special build. All local steps run on a laptop in well under an hour total once the merged CSVs are downloaded.

### 1.4 Login-node policy (strict)

The NAF login node is for **interactive single-file checks only**, capped at 5 min wall and one input file. Everything else MUST go through Condor. Specifically:

- Multi-file extraction → Condor.
- Multi-file merge → Condor or the dedicated `merge_*.sh` wrappers running sequentially in a dedicated session.
- Smoke-sweeps, batch validation, production passes → Condor.

---

## 2. What you get for free (canonical artifacts already on disk)

If you just want to USE the measurement, do not re-run anything. Read `data_release_v1p2_final/README.md` and consume the 21 files in `data_release_v1p2_final/`. The example loader is `data_release_v1p2_final/examples/read_dataset_python.py`. (The older `data_release/` directory is the superseded v1.0 preliminary release.)

If you want to verify intermediate products, these files are the canonical artifacts:

| Product | Path | Produced by |
|---|---|---|
| Merged per-track data CSV (γ\*+p frame) | `data/pertrack_v5g_fixedFrame_v3_merged.csv.gz` | NAF: `extract_pertrack_v5g_fixedFrame_v3.py` + `merge_*_v3.py` |
| Merged MC CSVs (reco, gen, disevent × DJANGOH/RAPGAP) | `data/mc_v3/pertrack_mc_v3_{reco,gen,disevent}_{djangoh14,rapgap31}_merged.csv.gz` | NAF: `extract_pertrack_mc_v3/extract_*_v3.py` + `merge_*.py` |
| Response matrices (480×480) | `data/response_4d_{djangoh,rapgap}.npz` | `scripts/unfolding/build_response_4d.py` |
| Nominal unfolds | `data/unfolded_4d_v3_{BBB,IBU,TUnfold}.json` (+ covariance NPZs for IBU and TUnfold) | `unfold_{BBB,IBU,TUnfold}_4d.py` |
| 4-term unfolding budget | `data/sigma_reg_TUnfold_4d.json`, `data/sigma_MCstat_TUnfold_4d.json`, `data/unfolded_4d_v3_data_DJvsRG.json`, plus `data/unfolded_4d_v3_TUnfold_covariance.npz` (the σ_stat source) | `sigma_reg_TUnfold_4d.py`, `sigma_MCstat_TUnfold_4d.py`, `unfold_data_4d_DJ_vs_RG.py` |
| 6 detector systematic source JSONs + 480×480 covariance NPZs | `data/systematics_sources/source_{tracking_efficiency,tracking_chargedep,tracking_resolution,electron_energy,electron_theta,electron_phi}.json` + `cov_*.npz` | NAF `source_*.py` + `run_*.py`, except `source_tracking_chargedep.py` which is analytic and local-runnable |
| Normalization sources | `source_lumi.json`, `source_trigger.json`, `source_NucliaWeight.json` | `emit_source_{lumi,trigger,NucliaWeight}.py` |
| Noise floor + 480×480 covariance | `data/systematics_sources/noise_floor.json` + `cov_noise.npz` (aggregated from `data/noise_floor_replicas/*.json`) | `noise_floor_bootstrap.py` × 40 + `compute_noise_floor.py` |
| Master combined budget | `data/systematics_combined_v3_HQv5gBprime.json` | `combine_systematics.py` |
| Trust mask | `data/v5g_4d_trust_mask.json` (364/480 trustworthy at τ=0.10) | `diag_catastrophic_4D_bins.py` |
| Catastrophic-bin diagnostic | `data/v5g_catastrophic_4D_bins_diag.json` | same |
| Public data release v1.0 | `data_release/` (17 files, 40.4 MB) | **NO script in repo (gap)** — assembled ad hoc; see §6 |
| C_rad provisional baseline | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/full_crad_v1/full_crad_v1_summary.json` | DJANGOH14 gen-only v1; old `data/crad_v3_HQv5gBprime_2026-05-24.json` is historical/superseded and must not be used for current z005 |

---

## 3. Historical reference: v3 → v3p1 chain (superseded by v3final)

> **Superseded.** The steps below describe the v3/v3p1 chain that preceded
> v3final. They are retained as a historical record. For the current
> canonical chain, see §0 above.

The chain has 4 logical phases. Within each phase, sub-steps either depend on the previous output or run independently.

```
PHASE 1 (Extract)          NAF Condor      hours
  ↓
PHASE 2 (Unfold)           local laptop    ~10 min
  ↓
PHASE 3 (Systematics)      mixed: NAF Condor for shifted-response builds,
                                   local for re-unfolds + analytic sources
  ↓
PHASE 4 (Combine + Release) local laptop    ~5 min
```

---

### PHASE 1 — Extract (NAF Condor) [historical v3/v3p1]

**Skip this phase if `data/pertrack_v5g_fixedFrame_v3_merged.csv.gz` and `data/mc_v3/pertrack_mc_v3_*_merged.csv.gz` already exist on your laptop.** They do today (May 2026).

#### Step 1.1 — Build data + MC sidistree ROOT inventory (Stage 1 + Stage 2 production)

ONLY needed if you change the binary, the cuts, the GRL routing, or the 4-period sample composition. Otherwise the inventory of 660 data + 865 MC sidistree files at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/{ep0304,em0405,em06,ep0607}/` is already what you want.

```bash
# On NAF, in /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/
python submit_full_production.py --include-ep0304        # dispatches per-period GRL Stage 1 + Stage 2
```

Wall time: many hours; runs ~1500 Condor jobs. The wrapper enforces the per-period GRL routing and the year-06 chunk boundary at chunk 220.

#### Step 1.2 — Data per-track extraction (γ\*+p CM frame)

```bash
# On NAF, in scripts/extract_pertrack_v5g_fixedFrame/
condor_submit submit_pertrack_v5g_fixedFrame_v3.condor
# ... wait ...
python merge_pertrack_v5g_fixedFrame_v3.py \
    --in-dir  /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame_v3 \
    --out     /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame_v3/pertrack_v5g_fixedFrame_v3_merged.csv.gz
# Pull to laptop:
```

Wall time: ~30 min Condor + ~10 min merge.

**Why v3, not v3p1?** v3 is the canonical 18-column extractor for the nominal chain. v3p1 adds 8 columns (`scatEX/EY/EZ/EE`, `Phi_lab`, `BeamElecE`, `BeamProtP`, `Empz`) that are needed ONLY for the electron-energy/θ/φ systematic shift, which lives in the NAF `source_electron_shift.py` driver. Use v3p1 only when re-doing that systematic; for the unfolding chain proper, v3 is sufficient and is what `build_response_4d.py` reads.

#### Step 1.3 — MC per-track extraction (DJANGOH + RAPGAP, both reco and gen and disevent)

```bash
# On NAF, in scripts/extract_pertrack_mc_v3/
condor_submit submit_pertrack_mc_v3.condor   # reco, gen, disevent for both samples
# ... wait ...
python merge_pertrack_mc_v3.py
# Pull all 7 merged CSVs to laptop:
```

Wall time: ~2 hr Condor (gen has ~144 M rows) + ~30 min merge. Output sizes: 5-19 GB per CSV.

**Note on v3 vs v3p1 vs v3p2:** The current `data/mc_v3/` CSVs are v3 outputs. v3p1 (reco/disevent) and v3p2 (gen, Option C fiducial — handoff §1) are pending production runs; once those merges land, switch downstream consumers to the new CSVs.

---

### PHASE 2 — Unfold (local laptop) [historical v3/v3p1]

#### Step 2.1 — Build the 4D response matrix

```bash
cd H1/sidis_highQ2
python scripts/unfolding/build_response_4d.py \
    --data-csv     data/pertrack_v5g_fixedFrame_v3_merged.csv.gz \
    --mc-reco-csv  data/mc_v3/pertrack_mc_v3_reco_djangoh14_merged.csv.gz \
    --mc-gen-csv   data/mc_v3/pertrack_mc_v3_gen_djangoh14_merged.csv.gz \
    --out          data/response_4d_djangoh.npz

python scripts/unfolding/build_response_4d.py \
    --data-csv     data/pertrack_v5g_fixedFrame_v3_merged.csv.gz \
    --mc-reco-csv  data/mc_v3/pertrack_mc_v3_reco_rapgap31_merged.csv.gz \
    --mc-gen-csv   data/mc_v3/pertrack_mc_v3_gen_rapgap31_merged.csv.gz \
    --out          data/response_4d_rapgap.npz
```

Wall time: ~5 min each. Output: 480×480 response + per-bin priors and data marginal.

**Validation:** open the `.npz` and check `response.shape == (480, 480)`, `prior_gen.shape == (480,)`, `data_marginal.sum() > 0`. The DJANGOH response is the nominal; the RAPGAP response is used only for σ_model.

#### Step 2.2 — Run the three nominal unfolds

```bash
python scripts/unfolding/unfold_BBB_4d.py     --response data/response_4d_djangoh.npz \
                                               --out data/unfolded_4d_v3_BBB.json

python scripts/unfolding/unfold_IBU_4d.py      --response data/response_4d_djangoh.npz \
                                               --out data/unfolded_4d_v3_IBU.json \
                                               --cov-out data/unfolded_4d_v3_IBU_covariance.npz

python scripts/unfolding/unfold_TUnfold_4d.py  --response data/response_4d_djangoh.npz \
                                               --out data/unfolded_4d_v3_TUnfold.json \
                                               --cov-out data/unfolded_4d_v3_TUnfold_covariance.npz
                                               # τ = TAU_FIXED = 0.10 baked into the script
```

Wall time: BBB ~10 s, IBU ~30 s, TUnfold ~1 min.

**Validation:** the three should agree on `N_unfolded` to within a few percent on the "trustworthy" bins. Disagreement of >20% on a bin is expected on the ~98 catastrophic bins identified in step 2.5.

#### Step 2.3 — Cross-MC closure (4D)

```bash
python scripts/unfolding/closure_4d_crossMC.py \
    --response-dj  data/response_4d_djangoh.npz \
    --response-rg  data/response_4d_rapgap.npz \
    --out          data/unfolded_4d_v3_closure_crossMC.json
```

Wall time: ~5 min (4 tests × 3 solvers).

#### Step 2.4 — 4-term unfolding budget (σ_reg, σ_MCstat, σ_model)

```bash
# σ_reg — re-runs TUnfold at τ ∈ {0.01, 0.05, 0.1, 0.2, 1.0}
python scripts/unfolding/sigma_reg_TUnfold_4d.py \
    --response data/response_4d_djangoh.npz \
    --out      data/sigma_reg_TUnfold_4d.json

# σ_MCstat — one-shot preprocessor then 100-replica bootstrap (long but parallelizable)
python scripts/unfolding/_prep_mc_arrays_for_bootstrap.py \
    --mc-reco-csv data/mc_v3/pertrack_mc_v3_reco_djangoh14_merged.csv.gz \
    --mc-gen-csv  data/mc_v3/pertrack_mc_v3_gen_djangoh14_merged.csv.gz \
    --out-reco    data/_bootstrap_arrays_djangoh.npz \
    --out-gen     data/_bootstrap_arrays_djangoh_gen.npz

python scripts/unfolding/sigma_MCstat_TUnfold_4d.py \
    --response       data/response_4d_djangoh.npz \
    --bootstrap-reco data/_bootstrap_arrays_djangoh.npz \
    --bootstrap-gen  data/_bootstrap_arrays_djangoh_gen.npz \
    --replica-dir    data/mcstat_replicas_tau0p1 \
    --out            data/sigma_MCstat_TUnfold_4d.json \
    --n-replicas     100

# σ_model — DJANGOH vs RAPGAP, BBB+IBU+TUnfold half-spread
python scripts/unfolding/unfold_data_4d_DJ_vs_RG.py \
    --data-csv     data/pertrack_v5g_fixedFrame_v3_merged.csv.gz \
    --response-dj  data/response_4d_djangoh.npz \
    --response-rg  data/response_4d_rapgap.npz \
    --out          data/unfolded_4d_v3_data_DJvsRG.json
```

Wall times: σ_reg ~5 min; σ_MCstat ~30 min for 100 replicas (≈ 18 s each); σ_model ~3 min.

The 4th term, σ_stat, is the analytic diagonal of `unfolded_4d_v3_TUnfold_covariance.npz` from step 2.2.

---

### PHASE 3 — Systematics (mixed NAF Condor + local) [historical v3/v3p1]

#### Step 3.1 — Bin-grid bootstrap from the nominal observable

```bash
python scripts/systematics/emit_nominal_uncertainty_reference.py
# writes data/systematics_sources/source_00_nominal.json
```

Wall time: seconds. Run once per binning change.

#### Step 3.2 — Normalization sources (analytic, local)

```bash
python scripts/systematics/emit_source_lumi.py            # 2.7% lumi
python scripts/systematics/emit_source_trigger.py         # 0.5% placeholder
python scripts/systematics/emit_source_NucliaWeight.py    # envelope from clip variation
```

Wall time: seconds each.

**Outstanding fix:** `source_lumi.json` writes `source_id="lumi"`; combiner expects `"luminosity"`. Edit one line in `emit_source_lumi.py` before re-running for the next release.

#### Step 3.3 — Detector sources: charge-dependent tracking (analytic, local)

```bash
python scripts/systematics/source_tracking_chargedep.py
# → data/systematics_sources/source_tracking_chargedep.json + cov_tracking_chargedep.npz
```

Wall time: seconds. NOT τ-dependent (analytic per-charge ±0.3% reweight on already-unfolded yields).

#### Step 3.4 — Detector sources: migration-affecting (NAF Condor + local)

These four sources MUST go through `ROOT → shifted CSV/response → re-unfold`:

1. **tracking efficiency (common-mode 0.5%/track)** — shifted response, then re-unfold.
2. **tracking resolution (±20% on σ_pT)** — smeared response, then re-unfold.
3. **electron energy (±0.5% central / ±1% forward LAr)** — shifted CSV, shifted response, then re-unfold.
4. **electron θ (±1 mrad), electron φ (±1 mrad)** — same as electron energy.

On NAF:

```bash
# in scripts/systematics/ on NAF
condor_submit submit_tracking_efficiency.condor    # builds shifted responses up/down
condor_submit submit_tracking_resolution.condor    # builds smeared response (single seed)
condor_submit submit_electron_systematic.condor    # builds shifted CSVs (E/θ/φ × up/down)

# Wait for all jobs to finish. Then run the re-unfold drivers (also on NAF, since they
# import from scripts/unfold4d/):
python run_tracking_efficiency_systematic.py
python run_tracking_resolution_systematic.py
python run_electron_systematic.py
```

Each `run_*.py` driver does: load nominal unfolded JSON; for each variation, re-solve the SAME TUnfold-4D problem against the SHIFTED response operator; compute `δ_signed = (N_up − N_down)/2`; emit `source_*.json` + rank-1 `cov_*.npz`.

Then pull the 6 per-source JSONs + covariance NPZs back to laptop:

```bash
```

Wall time: ~1 hr total NAF Condor.

**Critic verdicts (round 1, τ=0.01; some improved at τ=0.10):** several of these sources have a large fraction of "same-side" bins indicating sub-noise sensitivity. The supervisor must decide per source whether to keep, conservatively bound, or relegate to v2. See `docs/critic/` memos. None of those decisions changes how you run the chain; they only affect whether the source ships in `data_release/`.

#### Step 3.5 — Noise floor (40-replica bootstrap)

```bash
# On NAF (Condor): dispatch 20 data-mode + 20 mc-mode replicas
cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/systematics
condor_submit submit_noise_floor.condor

# Wait. Then aggregate locally OR on NAF — both copies of compute_noise_floor.py work:
python compute_noise_floor.py \
    --data-glob 'output/systematics/noise_floor_replicas/noise_floor_data_*.json' \
    --mc-glob   'output/systematics/noise_floor_replicas/noise_floor_mc_*.json' \
    --out-json  output/systematics/noise_floor.json \
    --out-npz   output/systematics/cov_noise.npz

# Pull to laptop:
```

Wall time: ~20 min Condor + seconds aggregate.

**Important:** `noise_floor_bootstrap.py`, `compute_noise_floor.py`, and `apply_noise_correction.py` had a hardcoded `unfold4d/` path. That bug was fixed on 2026-05-26 to `unfolding/` (local) and remains `unfold4d/` (NAF). Do not be alarmed by the asymmetric naming — it matches the directory layout on each side.

#### Step 3.6 — Apply noise correction to FATAL sources

```bash
# For each FATAL or CONCERN source that you want to publish, subtract the noise floor.
for src in tracking_efficiency tracking_resolution electron_energy electron_theta electron_phi; do
    python scripts/systematics/apply_noise_correction.py \
        --source-json data/systematics_sources/source_${src}.json \
        --cov-npz     data/systematics_sources/cov_${src}.npz \
        --noise-floor-json data/systematics_sources/noise_floor.json \
        --out-dir     data/systematics_sources/corrected
done
```

Wall time: seconds per source. Output: `data/systematics_sources/corrected/source_*_corrected.json` + `cov_*_corrected.npz`.

**Status:** `data/systematics_sources/corrected/` is empty as of 2026-05-27; this step has not been run at τ=0.10.

---

### PHASE 4 — Combine + Release (local laptop) [historical v3/v3p1]

#### Step 4.1 — Master combine

```bash
python scripts/systematics/combine_systematics.py
# → data/systematics_combined_v3_HQv5gBprime.json
```

Wall time: seconds. The combiner walks `data/systematics_sources/source_*.json`, validates each against `docs/methodology/systematics_data_contract_v3.md`, and combines per-bin uncertainties using each source's declared correlation class.

**Outstanding (handoff §10.1):** has not been re-run at τ=0.10 with the current 6 detector sources.

#### Step 4.2 — Trust mask + catastrophic-bin diagnostic

```bash
python scripts/diag_catastrophic_4D_bins.py
# → data/v5g_4d_trust_mask.json + data/v5g_catastrophic_4D_bins_diag.json + PDF
```

Wall time: ~30 s. This is the gatekeeper for which bins enter `data_release/`: 364/480 are trustworthy at τ=0.10.

#### Step 4.3 — Canonical priority plots

```bash
python scripts/plotting/make_tau0p1_priority_plots.py
# → plots/sigma_per_bin_full_budget_tau0p1/*.pdf (5 plots)
```

Wall time: ~1 min.

#### Step 4.4 — Assemble `data_release/`

**No script exists for this step (gap; see SCRIPTS_MANIFEST §3.2).** The v1.0 release on disk was assembled ad hoc. Until a builder lands, treat `data_release/` as "frozen v1.0" and do not regenerate. When you DO add a builder, its inputs should be: the master combined JSON (4.1), the trust mask (4.2), the per-source covariance NPZs, and the bin/cell definition from `_unfold_4d_common.py`. Its outputs are documented in `data_release/CHANGELOG.md`.

---

## 4. How to validate intermediate outputs

| After step | Quick sanity check |
|---|---|
| 1.2 (data extractor) | `gzcat data/pertrack_v5g_fixedFrame_v3_merged.csv.gz \| head -1` → should be the 18-column header starting with `period,run,event,Q2e,Q2es,...`. Row count: ~hundreds of millions. |
| 1.3 (MC extractor) | Same column check on each of the 4 MC CSVs. DJANGOH gen CSV should be ~5 GB; RAPGAP gen ~4 GB. |
| 2.1 (response) | `python -c "import numpy as np; d=np.load('data/response_4d_djangoh.npz'); print(d['response'].shape, d['prior_gen'].sum(), d['data_marginal'].sum())"` → `(480,480), ~10⁸, ~10⁶` ish. |
| 2.2 (unfolds) | `jq '.bins | length' data/unfolded_4d_v3_TUnfold.json` → 480. Check no `null` N_unfolded values. |
| 2.4 (4-term) | The 4 component JSONs all have `bins[i]` arrays of length 480 with `sigma_*` fields. |
| 3.4 (detector sources) | Each `source_*.json` validates against the data contract (the combiner enforces this). `cov_*.npz` is a 480×480 array. |
| 3.5 (noise floor) | `data/noise_floor_replicas/` contains 20 `data_*` and 20 `mc_*` JSONs, each with `unfolded` (480,). |
| 4.1 (combined) | `jq '.bins[0].sigma_total' data/systematics_combined_v3_HQv5gBprime.json` → positive number. |
| 4.2 (trust) | `jq '[.trust[]] | add' data/v5g_4d_trust_mask.json` → 364 (at τ=0.10). |
| 4.4 (release) | `cd data_release && sha256sum -c MANIFEST.sha256` → all OK. |

---

## 5. Common pitfalls

These come straight from the handoff (`context-handoff.md` §7) — read them BEFORE running anything.

1. **`pT_h` and `phi_h` must be γ\*+p CM-frame.** The C++ TreeBuilder stores raw lab-frame 4-vectors; the canonical extractor `extract_pertrack_v5g_fixedFrame_v3.py` performs the per-event Lorentz boost. Do not invent your own per-track CSV with lab-frame quantities.
2. **BBB acceptance is `N_reco_match / N_gen`, NOT `N_reco_AND_gen / N_gen`.** The latter over-corrects data by ~20%. Fixed at line 417 of `compute_xsec_v3_HQv5gBprime.py` on 2026-05-24.
3. **The per-track CSV is a derivative.** Migration-affecting systematics (electron E/θ/φ, HFS, tracking precision) MUST work from ROOT on NAF, not from CSV-only Python.
4. **NAF Kerberos expires after ~24 h.** Re-issue `kinit` before launching multi-step Condor work.
5. **Source-id must match `EXPECTED_SOURCE_IDS` in `combine_systematics.py`.** Mismatch → "missing source" warning. The known live mismatch is `source_lumi.json`'s `"lumi"` vs combiner's `"luminosity"`.
6. **Canonical τ is 0.10, not 0.01.** The pre-2026-05-26 τ=0.01 outputs are in `quarantine/data/` with the `_tau0p01_backup` suffix. Do not mix τ=0.01 and τ=0.10 outputs in the same combine.
7. **Current SIDIS luminosity is 351,000.0 nb⁻¹ = 351 pb⁻¹, NOT the superseded pre-rescale luminosity nb⁻¹.** The latter is the 2005-2007-only value. The current high-Q² S67-triggered SIDIS 4-period sample includes ep0304 and uses `351000.0 nb⁻¹`; the older direct-monitor `356,613.0 nb⁻¹ = 356.613 pb⁻¹` quote is superseded.
8. **Verify MC sample identity before background scaling.** Processed `merged_7091.root` is `djangoh14/7091` RAD signal MC with `L_MC=4,379,935 nb⁻¹` and current scale `351000.0 / 4379935.0 = 0.08013817556653238`; `15,813.4 nb⁻¹` / `22.1963651` belongs to `djangoh14/7110` DjBkgd and must not be used for 7091.
9. **The v1.0 `data_release/` is PRELIMINARY.** Backgrounds not subtracted; trigger + lumi-norm systematics deferred to v2; QED radiative corrections deferred to v2.
10. **Do not run multi-file work on the NAF login node.** 5 min wall, one input file, period.

---

## 6. Where to find each canonical artifact

### Inputs (raw, on NAF)
| Item | Path |
|---|---|
| Stage 1 evls files | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/evls/data/{03,04,05,06,07}/` |
| Stage 2 sidistree files (data) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/data/{ep0304,em0405,em06,ep0607}/` |
| Stage 2 sidistree files (MC) | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output/mc/...` |
| GRLs | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/LumiFiles/` |
| Binaries | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/` |

### Intermediates (local, `H1/sidis_highQ2/data/`)
See the table in §2 above.

### Outputs
| Item | Path |
|---|---|
| Master uncertainty JSON | `data/systematics_combined_v3_HQv5gBprime.json` |
| Trust mask | `data/v5g_4d_trust_mask.json` |
| Catastrophic-bin diag | `data/v5g_catastrophic_4D_bins_diag.json` |
| Headline plots (5) | `plots/sigma_per_bin_full_budget_tau0p1/*.pdf` |
| Public release | `data_release/` |

### Documentation
| Item | Path |
|---|---|
| This recipe | `docs/REPRODUCING_THE_ANALYSIS.md` |
| Scripts manifest | `docs/SCRIPTS_MANIFEST.md` |
| Supervisor handoff (authoritative current state) | `context-handoff.md` |
| Doc index | `docs/INDEX.md` |
| Unfolding pipeline freeze | `docs/methodology/unfolding_pipeline_FINAL_2026-05-25.md` |
| τ-scan rationale | ~~`docs/methodology/TUnfold_scan_2026-05-25.md`~~ → **quarantined** `quarantine/docs/methodology/TUnfold_scan_2026-05-25.md` (pre-τ=0.10 final; superseded) |
| Binning freeze | `docs/methodology/binning_freeze_2026-05-24.md` |
| Frame bug | `docs/methodology/pTh_phih_frame_bug_2026-05-24.md` |
| C_rad methodology | ~~`docs/methodology/crad_v5f_methodology_FROZEN.md`~~ → **quarantined** `quarantine/docs/methodology/crad_v5f_methodology_FROZEN.md` (v5f era; superseded) |
| Systematics data contract | `docs/methodology/systematics_data_contract_v3.md` |
| Critic memos (per source) | `docs/critic/` |
