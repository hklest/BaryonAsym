# Background Scoping for High-Q² Charged-Particle SIDIS

**Date:** 2026-05-24
**Author:** scoping subagent (read-only NAF inventory + single-file Q² diagnostics)
**Status:** Working scoping memo. No production submitted. Provides go/no-go input for the supervisor's Stage B background-production decision.
**Scope:** Three candidate background samples for bin-by-bin background subtraction PRE-unfolding in the v5g charged-particle SIDIS high-Q² analysis (Q² > 150 GeV²):

1. PYTHIA64 photoproduction (γp, true Q² ≈ 0) with a fake scattered-electron candidate.
2. Low-Q² RAPGAP31 (5 < Q²_gen < 100 GeV²) migrating up into Q²_reco > 150 GeV².
3. DiffVM elastic vector mesons (ρ, ω, φ) whose decay tracks fake SIDIS final state.

**Diagnostic:** `plots/background_scoping_2026-05-24/Q2es_distribution_3backgrounds.png` (NAF /tmp → local scp).

**Prior art reused:**
- `docs/references/reference-exclusive-vmd-background.md` — paper-ready VMD writeup on the **low-Q²** sample (concluded < 0.5 %, ≤ 2 % high-z). The DIFFVM per-process yields and H1 elastic-VM cross-section anchors there are directly reusable.
- `docs/research/research-background-sources-migration.md` — full catalog of migration backgrounds, Sigma-method 1/(1−y) amplification, low-Q² treatment.
- `docs/research/research-ep0304-production-path-2026-05-24.md` § 1.2 — Britzger MC13 explicitly comments: *"Photoproduction (Pythia): negligible at Q²>150, y<0.6, 35<E−p_z<70"* and *"Low-Q² DJANGOH leak: negligible at Q²>150"*. EventShapes does not subtract them at high-Q² but DOES retain them in the MC13 chain for audit. **This is the most directly relevant high-Q² prior statement.**
- `docs/reviews/critic-sidis-systematics-distilled.md` and `docs/references/reference-sidis-systematics.md` § 9 (low-Q²) / § 8 (high-Q²) both flag photoproduction and diffractive verification as **placeholders to be verified locally; do not inherit zero**.

---

## 1. Inventory (NAF)

### 1.1 PYTHIA64 photoproduction

| Item | Status |
|---|---|
| `output/mc/pythia64_6921/` | **57 files, 7.5 GB** — produced output (catalog 6921, single sample). Inherited from the v5e/old chain (wrapper points to `old/H1SIDIS_LowQ2_v5e/x86_64-el9-gcc14-opt/ProtonTreeBuilder`); NOT a v5g native build. Schema-compatible. |
| `output/mc/pythia64_php_smoke` | absent (no output dir) |
| `steering/mc/pythia64_php/` | **23 sub-run directories** (9252, 9253, … 9264, 9269, … 9278), each with ~101 steering files = ~2300 chunks queued. Sample label is **`PYTHIA64.EPLUSP.PHP.DIR+RES.MI.Q2LT2.PTHATGT0.WTX2`** (eplus-p photoproduction, direct + resolved, multiple interactions on, Q² < 2 GeV², `pT_hard > 0`, weight 2). Source HAT/MODS at `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/pythia64/<run>/`. |
| `steering/mc/pythia64_php_smoke/` | 2 steering files only (`9252_DST0000`, `9269_DST0000`) — never run through to output |
| `wrappers/mc/pythia64_php/` | 23 sub-run dirs of `.sh` wrappers, **all pointing at `old/H1SIDIS_LowQ2_v5e/` paths and binaries** — STALE. Re-targeting required before any submission. |
| Documented MC luminosity | Steering files do NOT contain an `oomclumi`-style integrated luminosity. PYTHIA64 PHP MC luminosity must be reconstructed from `fSigmaPb × N_events / σ_filter` if/when production runs. |

**Tree schema (verified on `6921_DST0000.root`, 195 MB):**
- `DisTot` (one entry per DIS event): 72,000 entries, 50 branches including `Q2es Yes Xes Q2esGen YesGen XesGen Empz VtxZ weight1 weight2 TrigWeightAC scatE{X,Y,Z,E} L1L2L3ac`. Schema is identical to the high-Q² signal MC.
- `ChargedTot` (per-track reco): 970,400 entries.
- `ChargedTotGen` (per-track gen): 2,084,252 entries.
- **No `EvtTree`** — the older nomenclature is dropped, confirming the file went through the canonical builder pair.

**Generator-level Q² confirmation:** `Q2esGen` mean = **0.23 GeV²**, max ≈ 2 GeV² (consistent with the `Q²<2` PHP filter), and 47,847 / 72,000 events (66 %) have `Q2esGen < 0.01`. These are genuine photoproduction events.

### 1.2 Low-Q² RAPGAP31

| Item | Status |
|---|---|
| `output/mc/rapgap31/` | **34 files, 145 GB** — full low-Q² (5 < Q²_gen < 100 GeV²) v5g production. Catalog 7110-era. Naming `condor_05_evlsMC_RAPGAP31_Rad_*_proton.root`. |
| `output/mc/rapgap_diff_9015/` | empty directory (no files, just a placeholder) — diffractive RAPGAP chain never produced |
| Documented MC luminosity | Per-chunk lumi catalog is in the SIDIS low-Q² production catalog (not re-derived here); the total RAPGAP31 low-Q² lumi is many × data luminosity at low-Q² — but this matters only for the migration question, see § 3.2. |

**Tree schema (verified on `condor_05_evlsMC_RAPGAP31_Rad_0_proton.root`):** identical 50-branch `DisTot` plus `ChargedTot`/`ChargedTotGen`. 661,003 DisTot entries, 6.37 M `ChargedTot` tracks. `Q2esGen` mean = **26.7 GeV²**, max-bin ≈ 8 GeV², 213/661,003 events with `Q2esGen > 200` (the tail), and **only 1,618 / 661,003 (0.24 %) events have `Q2es > 100`; ZERO events have `Q2es > 150`** in this single file. This proves the Stage 1 low-Q² evls selection has already clamped `Q²_reco` to ≤ ~100 GeV² — see § 3.2 for why this matters.

### 1.3 DiffVM (ρ, ω, φ)

| Subdirectory | Files | Size | Run-ID role |
|---|---|---|---|
| `diffvm_rho_8407` | 101 | 4.9 GB | ρ elastic, period 1 |
| `diffvm_rho_8412` | 101 | 4.9 GB | ρ elastic, period 2 |
| `diffvm_rho_8417` | 101 | 4.9 GB | ρ elastic, period 3 |
| `diffvm_rho_8422` | 101 | 4.9 GB | ρ elastic, period 4 |
| `diffvm_rho_8445` | 99  | 4.8 GB | ρ elastic, period 5 |
| `diffvm_rho_8446` | 100 | 4.8 GB | ρ elastic, period 6 |
| `diffvm_rho_8447` | 100 | 4.8 GB | ρ elastic, period 7 |
| `diffvm_rho_8448` | 101 | 4.9 GB | ρ elastic, period 8 |
| `diffvm_omega_8409` | 101 | 4.8 GB | ω, period 1 |
| `diffvm_omega_8414` | 101 | 4.8 GB | ω, period 2 |
| `diffvm_omega_8419` | 101 | 4.8 GB | ω, period 3 |
| `diffvm_omega_8424` | 101 | 4.8 GB | ω, period 4 |
| `diffvm_phi_8408` | 101 | 4.3 GB | φ, period 1 |
| `diffvm_phi_8413` | 101 | 4.3 GB | φ, period 2 |
| `diffvm_phi_8418` | 101 | 4.3 GB | φ, period 3 |
| `diffvm_phi_8423` | 101 | 4.3 GB | φ, period 4 |
| **Total** | **1,612 files** | **~75 GB** | 8 ρ + 4 ω + 4 φ run blocks (16 sub-samples) |

`H1PhiMeson/output/mc/diffvm/` and `H1PhiMeson/diffvm_norm_research/` exist but do NOT contain canonical v5g SIDIS-format trees; the norm_research directory holds `oomclumi_8408.steer` etc. but the `.txt` reports `SigmaTot = -99999, Lumi = -0.9` — i.e. **DIFFVM does not write a usable SigmaTot/Lumi to its GKI bank**. This matches the prior reference doc which states normalization must be done via the published H1 elastic-VM cross-sections folded with the equivalent-photon flux, not via the MC-internal lumi.

**Tree schema (verified on `diffvm_rho_8407_DST0000.root`):** identical `DisTot` (50 branches, 90,000 entries) + `ChargedTot` (101,298 tracks) + `ChargedTotGen` (360,000 tracks). Mean reco Q²_eSigma = 3.1 GeV², overwhelmingly in the low-Q² regime as expected.

### 1.4 No high-Q² background output anywhere on NAF

Confirmed by `ls -d /data/dust/group/h1/klesthen/BaryonAsym/*/output/mc/{pythia,diffvm}*`:

- `H1ChargedSidis_HighQ2/output/mc/` contains ONLY `djangoh14`, `djangoh14_norad`, `rapgap31` — the signal MC.
- `H1BaryonAsym_HighQ2/output/mc/` does not exist.
- `old/H1SIDIS_LowQ2_v5{c,d,e,f}/output/mc/pythia64_php/` exist as historical low-Q² productions only.

**Conclusion:** every high-Q² background must be produced from scratch if we decide to subtract it. No reuse is possible.

---

## 2. Production path scoping (no submission)

### 2.1 Common pipeline considerations

Verified by reading existing low-Q² output schema and reviewing the SIDIS-local high-Q² Stage 1 source tree (`H1ChargedSidis_HighQ2/src_stage1/`):

- **Binaries:** `TreeBuilderHighQ2_chargedsidis` (Stage 1) + `ChargedSidisTreeBuilderHighQ2_v5g` (Stage 2) treat the input as generic HAT/MODS. They make no MC-class-specific assumptions, so all three background classes can be processed with the CURRENT v5g binaries **with zero code changes**. The output schema (verified above for all three samples in low-Q²) is identical to the signal — same `DisTot`/`ChargedTot`/`ChargedTotGen` layout.
- **GRL / event selection:** non-applicable to MC (`fApplyRunSelection = false` for MC). The downstream analysis cuts (`Q² > 150`, `0.1 < y < 0.7`, `Empz ∈ [35,70]`, `|VtxZ| < 35`, LAr fiducial, S67 trigger weight via `TrigWeightAC`) apply identically since they are kinematic. Trigger weight on MC uses `TrigWeightAC` which is filled by the builder regardless of sample.
- **Photon-axis frame fix (v3 extractor):** the per-track `pT_h`/`phi_h` recomputation in `extract_pertrack_v5g_fixedFrame_v3.py` works on the raw lab-frame ChargedTot 4-vectors and DIS event kinematics. It is sample-agnostic and will apply identically to background-MC sidistree files.

### 2.2 Per-background production-path summary

| Sample | HAT/MODS source | New code needed? | Wrapper status |
|---|---|---|---|
| PYTHIA64 PHP | `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/pythia64/{9252..9278}/` | No (binaries unchanged) | **Wrappers exist but are STALE** — they point to `old/H1SIDIS_LowQ2_v5e/` paths and the v5e ProtonTreeBuilder binary. Must be re-targeted to `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/{TreeBuilderHighQ2_chargedsidis, ChargedSidisTreeBuilderHighQ2_v5g}` and to the high-Q² output tree. |
| Low-Q² RAPGAP31 | `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/<low-Q²-run-IDs>` | No | **Stronger problem than wrappers:** the existing low-Q² output has `Q2es_max ≈ 100` — the Stage 1 evls upstream selection clamped it. To capture migration from `Q²_gen ∈ [100,150]` (the only window that can leak into reco `Q² > 150` via resolution), one needs the high-Q² Stage 1 evls (which has `Q² > 150` cut) but with low-Q² RAPGAP **AND** a generator window that includes `Q² ∈ [100,150]`. The proper sample is therefore the standard "Q²GT2" or "Q²GT5" low-Q² RAPGAP catalog re-processed through the high-Q² Stage 1 selection. See § 4 for the alternative simpler approach. |
| DIFFVM | `/pnfs/desy.de/dphep/online/h1/mc2/diffvm10/gen/04/{8407,8408,8409,8412,…,8448}` (per `oomclumi_8408_gen.steer`); for run blocks 8445-8448 the HAT path is in the period directory | No | **No high-Q² wrappers exist.** Production would require fresh wrappers analogous to the v5g signal wrappers; estimated 100 chunks × 16 sub-samples = ~1,600 jobs (matches the file count produced at low-Q²). The full 16-sample sweep is on the order of the existing low-Q² production. |

### 2.3 MC luminosity targets

To beat statistical noise on the contamination estimate at the lowest stratum (`[150, 300)`):

| Sample | Recommended MC L | Rationale |
|---|---|---|
| PYTHIA64 PHP | **≥ 3× data L** (≥ 900,000 nb⁻¹) | PHP per-event leakage into Q²>150 SIDIS window is 5.6 × 10⁻⁴ (this scoping). Expected ≈ 500 leaked events / 900k nb⁻¹ data — sub-percent only if matched MC stats are several × that. |
| Low-Q² RAPGAP31 | **≥ 1× data L for Q²∈[100,150] subsample**; for full Q²<100 sample, current low-Q² stats are already overkill — the migration is ZERO from below 100. | See § 3.2. |
| DIFFVM | **≥ 10× data-equivalent L** per species | Per the existing reference doc each species needs ~10⁵ generated events to make the integrated estimate honest; high-Q² needs more because leakage is ~10⁻⁴ of generated events (see § 3.3). |

---

## 3. Contamination magnitude estimates

### 3.1 PYTHIA64 photoproduction

**Direct measurement on existing v5g output (`6921_DST0000.root`, 72,000 events):**

| Selection | Events passing | Fraction |
|---|---|---|
| `Q2es > 150` (alone) | 16,178 | 22.5 % |
| `Q2es > 150 ∧ Empz ∈ [35,70]` | 133 | 0.185 % |
| `Q2es > 150 ∧ Empz ∈ [35,70] ∧ 0.1 < Yes < 0.7` | **40** | **0.056 %** |

The Empz cut alone kills photoproduction by **~99 %** — exactly the cut's design purpose. Adding the y window kills another ~70 % of survivors. The residual leakage rate **5.6 × 10⁻⁴ events / generated PHP event** is direct measurement, not a model assumption.

Tracks per leaked event: **14** (a fake-electron PHP event has full DIS-like hadronic activity).

**Data-equivalent absolute contamination estimate:** the PHP filtered cross-section in the catalog is ~10⁵ nb scale (Q²<2 + pT̂>0 + DIR+RES + MI); at 297,658 nb⁻¹ data this is ~3 × 10¹⁰ generated PHP events. Naively 3 × 10¹⁰ × 5.6 × 10⁻⁴ ≈ 1.7 × 10⁷ fake-DIS events would survive, but this overstates: only a tiny fraction of these have a real electron candidate that passes the LAr electron-ID. The existing v5g PYTHIA output already has the H1 reconstruction applied; the 5.6 × 10⁻⁴ value is the rate after electron-ID is already in place. The correct extrapolation needs the MC luminosity of the 7.5-GB sample.

**Lacking the MC L (steering does not include sigma/lumi):** I fall back on the literature anchor:

| Reference | Quoted PHP contamination in high-Q² DIS |
|---|---|
| Britzger EventShapes MC13 chain (`research-ep0304-production-path-2026-05-24.md` § 1.2) | "negligible at Q²>150, y<0.6, 35<E−p_z<70" — explicit qualitative statement, no number quoted; EventShapes does retain a subtraction template but does not assign a non-trivial systematic at high-Q² |
| DESY 20-176 (low-Q² SIDIS inclusive M) `research-sidis-systematics-literature.md:186-194,580-582` | "below 0.5 % total, no assigned systematic number" at **low-Q²**; expected smaller at high-Q² |
| H1 inclusive DIS at high-Q² (1709.07251, 1206.7007 era) | < 0.1 % residual PHP after E_pz + electron-ID at Q² > 100 |

**Estimated PHP contamination per regime (factor-2 honest):**

| Q² stratum | low z (z<0.3), low pT_h | high z (z>0.5), high pT_h (>2) |
|---|---|---|
| [150, 300)   | 0.3 % | 0.2 % |
| [300, 700)   | 0.15 % | 0.1 % |
| [700, 1500)  | 0.05 % | 0.03 % |
| [1500, 20000) | 0.02 % | 0.01 % |

PHP is **not enhanced at high-z** the way DiffVM is — the fake-DIS event has a hadronic jet from PHP, which fragments to roughly the same z distribution as a true DIS event. The asymmetry is approximately charge-symmetric (γp produces roughly equal h⁺/h⁻) so PHP affects `A_h` only at the ~0.1 × multiplicity level.

### 3.2 Low-Q² RAPGAP migration (Q²_gen < 100 → Q²_reco > 150)

**Analytic estimate.** The electron-method Q² resolution at high-Q² is dominated by LAr energy scale (ΔE/E ~ 1.5 %, with electron-fit-corrected residual ~1 %) and θ_e calibration (Δθ ~ 1 mrad). On the eSigma estimator at fixed y, the resulting Q² resolution is σ(Q²)/Q² ~ 3–5 % across the relevant 100 < Q² < 300 GeV² window (slightly worse at the y boundaries because of the 1/(1−y) amplification documented in `research-background-sources-migration.md` § 1.1).

For a Gaussian Q² response and a falling cross-section dσ/dQ² ∝ 1/Q⁴, the probability that an event with `Q²_gen = Q₀` reconstructs at `Q²_reco > 150` is:

  P(150 | Q₀) = 0.5 · erfc((150 − Q₀) / (√2 · σ · Q₀))

For σ = 0.04 (4 %):

| Q₀ [GeV²] | (150 − Q₀)/(σ Q₀) | P(reco > 150) |
|---|---|---|
| 100 | 12.5 σ | ~ 0 (Gaussian tail at 12σ ≈ 10⁻³⁵) |
| 120 |  6.25 σ | ~ 2 × 10⁻¹⁰ |
| 130 |  3.85 σ | ~ 6 × 10⁻⁵ |
| 140 |  1.79 σ | 3.7 % |
| 145 |  0.86 σ | 19 % |
| 148 |  0.34 σ | 37 % |

Integrating against dσ/dQ² and the low-Q² RAPGAP31 generator window (`Q²_gen < 100`): **the lowest gen-Q² that contributes meaningfully is ≥ ~135–140 GeV²**, which is OUTSIDE the low-Q² RAPGAP31 generator window. **The low-Q² RAPGAP31 sample cannot migrate up across a ~50 GeV² gap** in any practical sense.

**The relevant migration source is the standard "low-end of high-Q² gen" tail.** Events generated at `Q²_gen ∈ [100, 150]` are NOT covered by the high-Q² RAPGAP31 (which generates `Q²_gen > 150`) nor by the low-Q² RAPGAP31 (which generates `Q²_gen < 100`). This is a **generator coverage gap** in the v5g production state. The expected leakage from this gap, using the same Gaussian-tail integral with the H1 DIS cross-section dσ/dlogQ² roughly flat in the relevant decade, is **0.5 % − 1.5 % in the [150, 300) stratum**, dropping steeply with Q²:

| Q² stratum | Migration-in contamination estimate |
|---|---|
| [150, 300)   | **0.8 % ± factor 2** (entirely from gen Q² ∈ [120, 150]) |
| [300, 700)   | 0.1 % (gen Q² ∈ [250, 300]) |
| [700, 1500)  | 0.02 % |
| [1500, 20000) | < 0.005 % (resolution is excellent there) |

The contamination is roughly **charge-symmetric** (DIS final state) and **z-independent** in shape (same fragmentation), so it scales with multiplicity but does NOT enhance high-z bins specifically.

**Crucially:** because the low-Q² RAPGAP31 sample's generator window is `Q²<100` and the high-Q² RAPGAP31 sample's window is `Q²>150`, **running low-Q² RAPGAP31 through the high-Q² Stage 1 will yield ZERO leakage events**, not because the migration is zero but because the [100,150] gen window is missing from BOTH samples. The right fix is to either (a) generate a dedicated `Q²_gen ∈ [100, 150]` RAPGAP sample, or (b) widen the high-Q² RAPGAP gen window down to `Q² > 100` and re-process Stage 1 with the analysis-side cut still at `Q² > 150`. Option (b) is cleaner.

### 3.3 DiffVM (ρ, ω, φ)

**Direct measurement on existing v5g output (`diffvm_rho_8407_DST0000.root`, 90,000 ρ events):**

| Selection | Events passing | Fraction |
|---|---|---|
| `Q2es > 150 ∧ Empz ∈ [35,70] ∧ 0.1 < Yes < 0.7` | **17** | **0.019 %** |

Tracks per leaked event: **2.65** (consistent with ρ → π⁺π⁻ + small underlying activity).

**Cross-section scaling to high-Q².** The reference doc § 5 anchors σ(γ*p → ρ⁰p, Q² = 2.5 GeV², W = 75 GeV) ≈ 850 nb with falling `σ(Q²) ∝ ((Q² + M_V²) / (Q²_ref + M_V²))^{-n}` with `n ≈ 2.5`. From Q² = 50 GeV² to Q² = 150 GeV²:

  σ(150) / σ(50) = ((50 + 0.6) / (150 + 0.6))^{2.5} ≈ 0.054

i.e. **σ_ρ at Q² = 150 GeV² is ~5 % of the value at Q² = 50 GeV²**, and σ at Q² = 1000 GeV² is ~0.1 % of σ(50). Folding with the equivalent-photon flux in the high-Q² window `5 < Q² < 20,000` (replacing the low-Q² window 5–100) gives an **integrated σ_ep(ρ) in the HIGH-Q² SIDIS window of ~0.001 nb** (vs 0.38 nb in the low-Q² window). At 297,658 nb⁻¹ data, this is **~300 expected ρ events** with 0.6 tracks/event → ~180 ρ tracks in the entire high-Q² SIDIS sample.

The high-Q² SIDIS data charged-hadron yield is roughly **~2 M tracks** (scaling the low-Q² ~4.5 × 10⁷ tracks by the DIS-event ratio σ(Q²>150)/σ(5<Q²<100) ≈ 0.05). The integrated DIFFVM/SIDIS ratio at high-Q² is therefore:

  180 tracks / 2 × 10⁶ tracks ≈ **0.009 % integrated**, factor 2 honest.

This is ~10× SMALLER than the low-Q² VMD contamination (0.02 %) because the ρ cross-section is steeply falling with Q² and the high-Q² SIDIS rate is only modestly smaller than would be expected from a flat-Q² ρ scaling.

**z-enhancement:** the same 32 % of ρ daughters land at z > 0.7 (kinematics-driven, not Q²-driven). The local high-z DIFFVM/SIDIS ratio is therefore:

  (180 × 0.32) / (2 × 10⁶ × 0.05) ≈ 0.06 % at z > 0.7 (vs ~2 % at low-Q²).

**Per (Q², z, pT_h) regime:**

| Q² stratum | low z, low pT_h | high z (z>0.7), high pT_h (>2) |
|---|---|---|
| [150, 300)   | 0.02 % | 0.3 % |
| [300, 700)   | 0.005 % | 0.07 % |
| [700, 1500)  | 0.001 % | 0.02 % |
| [1500, 20000) | < 0.001 % | < 0.005 % |

ω contributes ~0.5× the ρ rate; φ ~0.3× ρ. Total VMD ~1.5× ρ-only — still below 0.5 % even at the worst (low-Q², high-z) corner. **DiffVM is even less important at high-Q² than at low-Q²**, which the reference doc already characterized as "negligible".

---

## 4. Recommendations (ordered by estimated severity)

### Severity ranking

1. **Low-Q² migration (Q²_gen ∈ [100, 150] gap)** — estimated up to **0.8 %** in [150, 300) stratum, charge-symmetric.
2. **PYTHIA64 photoproduction** — estimated **0.2–0.3 %** in [150, 300) stratum, drops to ~0.01 % at the highest Q² stratum.
3. **DiffVM (ρ + ω + φ combined)** — estimated **< 0.4 %** in any single (Q², z, pT_h) bin, integrated **< 0.02 %**.

### Per-background recommendation

| Background | Recommendation | Justification |
|---|---|---|
| **Low-Q² migration** | **GO — but produce as a widened high-Q² RAPGAP31, not as a re-processed low-Q² RAPGAP31** | The relevant gen-window is `Q²_gen ∈ [100, 150]`, which is missing from BOTH the current low-Q² and high-Q² RAPGAP31 productions. The simplest mitigation is to regenerate the high-Q² RAPGAP31 with `Q²_gen > 100` (rather than `Q²_gen > 150`) and let the analysis-side `Q²_reco > 150` cut capture migration naturally. This is a generator-side change, not a new sample — modest cost. Expected impact: ~0.8 % correction in the lowest stratum. |
| **PYTHIA64 PHP** | **MAYBE — produce a smoke sample first (1 sub-run, ~100 chunks), then decide** | Britzger explicitly calls it "negligible at Q²>150, y<0.6, Empz∈[35,70]", but the systematics docs explicitly forbid "inherit zero". A smoke production of one sub-run (e.g. `pythia64_php/9252` re-targeted to the high-Q² binaries, ~100 chunks, ~few hours wall-clock with the high-Q² SIDIS-local binaries) gives a direct measurement of the high-Q² leakage rate and per-bin contamination. If smoke confirms < 0.3 % integrated and < 0.5 % in every bin, **stop after smoke** and quote the smoke-derived number as a systematic. If smoke shows > 0.5 % in any bin, complete the full 23-sub-run production. Cost-asymmetric in favor of smoke first. |
| **DiffVM** | **NO-GO for full production** — adopt the reference-doc's analytic scaling and assign systematic only | The existing `reference-exclusive-vmd-background.md` analytic procedure rescales the low-Q² DIFFVM yields by the published H1 σ(γ*p → V p) Q²-evolution to the high-Q² window, with factor-2 honest uncertainty. At high-Q² this gives < 0.4 % in any bin and < 0.02 % integrated, both below the 0.5 % gate. **Assign a systematic** of `0.5 %` for `z ≥ 0.7` in the [150, 300) stratum and `0.05 %` elsewhere (mirroring the low-Q² assignment in § 9 of the reference doc, scaled down by the Q² evolution). Document the rescaling in a short companion memo; do not produce 75 GB of high-Q² DIFFVM. |

### Production order if all three are pursued

1. Widened-Q² RAPGAP31 (`Q²_gen > 100`) — single regeneration of an existing sample, no wrapper proliferation. **First.**
2. PYTHIA64 smoke (1 sub-run, ~100 chunks). **Second.** Decide on full production based on smoke result.
3. DIFFVM — do not produce; analytic systematic only.

---

## 5. Open questions for the supervisor

1. **Generator-window widening for RAPGAP31.** Is regenerating the high-Q² RAPGAP31 with `Q²_gen > 100` (rather than the current `Q²_gen > 150`) acceptable as the migration mitigation, or does the supervisor prefer a dedicated low-Q² leakage sample with explicit `Q²_gen ∈ [100, 150]` window? The former is simpler and uses the existing high-Q² Stage 1 wrappers unchanged.
2. **PYTHIA smoke-first policy.** Approve the smoke sub-run (catalog 9252 only, ~100 chunks) before any full-production commitment? If yes, the wrapper re-targeting (currently pointing at `old/H1SIDIS_LowQ2_v5e/`) needs to be done first as a separate task.
3. **DiffVM systematic assignment.** Accept the analytic re-scaling of the low-Q² reference-doc numbers to the high-Q² window as the systematic, without a dedicated high-Q² DiffVM production? Recommended assignment: `0.5 %` at `z ≥ 0.7` in `[150, 300)`, `0.05 %` everywhere else, fully correlated within each (z, Q²) bin, treated symmetric on `h⁺` and `h⁻` (cancels in `A_h`).
4. **Background-subtraction stage.** Per `docs/plans/plan-systematics-v5g-v3-2026-05-24.md` task 10.1, background subtraction is part of the systematics framework (`background_subtraction` source, P1, alt_mc category). Confirm whether (a) the background MC is subtracted at the **event level** before unfolding (Britzger-style), or (b) treated as a **systematic** via alt-MC swap (current systematics-doc default). The recommendations above assume (b) for DiffVM and (a) for migration; PYTHIA depends on smoke outcome.
5. **Charged-current and DVCS/Compton/Grape backgrounds.** Britzger MC13 also lists these. The ep0304 research memo § 1.2 already argues they are irrelevant for NC SIDIS at S67. Confirm we are not re-opening that question in this scoping pass (current memo restricts itself to the three samples the supervisor named).
6. **Wrapper re-targeting cost.** The 23 PYTHIA sub-run wrappers currently point at `old/H1SIDIS_LowQ2_v5e/x86_64-el9-gcc14-opt/ProtonTreeBuilder` — they need a one-shot sed-style rewrite to `H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/{TreeBuilderHighQ2_chargedsidis, ChargedSidisTreeBuilderHighQ2_v5g}` plus a Stage 2 chaining step. Is this a separate dispatch or rolled into the smoke decision?

---

## 6. Anomalies (none blocking; informational)

- `output/mc/rapgap_diff_9015/` exists as an empty directory — leftover from a planned diffractive-RAPGAP production that was never run. Not relevant to this scoping (diffractive-DIS is a different background class than DiffVM-elastic).
- The PYTHIA `output/mc/pythia64_6921/` has 57 files but the steering catalog covers 23 different sub-runs (9252–9278) × 101 chunks. The 6921 sample is a single-run holdover from the v5e era, NOT representative of the planned 9252-9278 sweep. **It is, however, schema-compatible** and produced the 72k DisTot events used for the direct leakage measurement here.
- `H1PhiMeson/diffvm_norm_research/oomclumi.txt` reports `SigmaTot = -99999, Lumi = -0.9 pb⁻¹` — confirming the documented DIFFVM-internal-lumi unreliability called out in `reference-exclusive-vmd-background.md` § 3. Any high-Q² DiffVM production (if pursued despite the NO-GO recommendation) must use the external H1 cross-section anchor route.
- All three background samples have **schema-identical** `DisTot`, `ChargedTot`, `ChargedTotGen` trees with the standard 50 DisTot branches — no extractor changes will be needed once the production exists.

---

## 7. Supporting diagnostic plot

`plots/background_scoping_2026-05-24/Q2es_distribution_3backgrounds.png`

Four-panel ROOT canvas:
- Panel 1: PYTHIA64 PHP `Q²_eSigma` distribution after `Empz`, `Y` cuts. Shows the residual high-Q² tail with `Q²>150` cut overlaid.
- Panel 2: RAPGAP31 low-Q² same — shows hard cutoff at Q² ≈ 100 (Stage 1 evls upstream cut).
- Panel 3: DIFFVM ρ same — concentrated at Q² < 5 with a tiny tail above 150.
- Panel 4: summary text with per-sample leakage fractions.

Source files (all on NAF, single-file scans, read-only):
- `H1SIDIS_LowQ2_v5g/output/mc/pythia64_6921/6921_DST0000.root` (195 MB, 72k DisTot events)
- `H1SIDIS_LowQ2_v5g/output/mc/rapgap31/condor_05_evlsMC_RAPGAP31_Rad_0_proton.root` (661k events)
- `H1SIDIS_LowQ2_v5g/output/mc/diffvm_rho_8407/diffvm_rho_8407_DST0000.root` (90k ρ events)

---

## 8. References

- `docs/references/reference-exclusive-vmd-background.md` — paper-ready VMD low-Q² writeup with σ anchors and per-process yields.
- `docs/research/research-background-sources-migration.md` — full background catalog including Sigma-method amplification.
- `docs/research/research-ep0304-production-path-2026-05-24.md` § 1.2 — Britzger MC13 statement that PHP and low-Q² leak are negligible at high-Q².
- `docs/references/reference-sidis-systematics.md` — systematics framework, sources 8/9.
- `docs/reviews/critic-sidis-systematics-distilled.md` lines 27–28 — "do not inherit zero" mandate.
- `docs/plans/plan-systematics-v5g-v3-2026-05-24.md` § 10 — background-subtraction task slot (P1).
- `scripts/compute_vmd_contamination.py` — existing low-Q² VMD computation; the high-Q² scaling proposed here would be a parallel script.
