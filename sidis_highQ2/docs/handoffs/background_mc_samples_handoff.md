# Background MC samples — handoff to high-Q² SIDIS

**Date:** 2026-05-28
**From:** low-Q² SIDIS supervisor
**To:** high-Q² SIDIS supervisor
**Purpose:** Cross-analysis-sharing opportunity. Three background MC sample groups were recently produced via the low-Q² v5g pipeline. The trees are schema-compatible with the high-Q² analysis (`ChargedTot` superset; see "Schema compatibility" below). Some of these samples are directly relevant to the high-Q² background subtraction; others may not be.

This document is a catalog so the high-Q² analyst can decide:
1. Whether to reuse the existing low-Q²-built trees as-is.
2. Whether to reproduce specific samples with the high-Q² builder (`ChargedSidisTreeBuilderHighQ2`) for full schema parity.
3. Whether some samples are irrelevant to the high-Q² physics and can be ignored.

---

## TL;DR — what's relevant to high-Q²

| Background source | Relevance to high-Q² SIDIS (Q²>150) | Recommended action |
|-------------------|--------------------------------------|--------------------|
| **DIFFVM exclusive φ, ω, ρ** | ✓ Likely relevant — diffractive VMD produced at all Q², contributes to high-Q² background too | Use shared sample; consider reproducing with high-Q² builder |
| **PYTHIA64 9261 PHP (Q²<2)** | ✗ Unlikely relevant — Q²<2 PHP cannot migrate up to Q²>150 reco fiducial | Ignore |
| **DJANGOH14 7080-7083 (gen Q²∈[2,5])** | ✗ Unlikely relevant — same argument | Ignore |

The headline benefit for high-Q²: **DIFFVM exclusive VMD samples (~55 GB, 12 ROOT-file groups) are already produced** and could be shared via either using-as-is or reproducing once with the high-Q² builder.

---

## Schema compatibility (low-Q² built trees vs high-Q² built trees)

A representative low-Q² MC tree was compared against a representative high-Q² MC tree (DJANGOH14 7072, em06 period). The high-Q² builder produces a **strict superset** of the low-Q² branches for the workhorse trees:

| Tree | low-Q² branches | high-Q² branches | low-Q² ⊂ high-Q²? | Diff |
|------|----------------:|-----------------:|:------------------:|------|
| **ChargedTot** | 96 | 101 | ✓ YES | high-Q² adds: `GenEnPhoton`, `Q2_es_gen`, `Y_es_gen`, `x_es_gen`, `ystar_es_gen` |
| **ChargedTotGen** | 49 | 60 | ✓ YES | high-Q² adds 11: `GenBeamElecE`, `GenBeamProtP`, `GenEnPhoton`, `GenScatE{X,Y,Z,E}`, plus the 5 gen-frame helpers |
| **DisTot** | 50 | 49 | ⚠ partial | low-Q² has `PVz`, `Ws`, `xBj` not in high-Q²; high-Q² has `Xs`, `Xs_gen` not in low-Q² |

**Implication for high-Q² analysis reusing low-Q²-built trees:** the 5 ChargedTot and 11 ChargedTotGen extra branches the high-Q² builder produces would be MISSING. If the high-Q² acceptance correction or gen-frame computation depends on those branches (esp. `GenScatE{X,Y,Z,E}` for the gen-frame z, pT_h, phi_h), the low-Q²-built trees would need to be reproduced with the high-Q² builder.

**Recommendation:** assume the high-Q² builder's extra branches are needed for high-Q² gen-frame work; plan to reproduce shared samples with `ChargedSidisTreeBuilderHighQ2`.

---

## Sample catalog (sample-by-sample relevance assessment)

### 1. DIFFVM exclusive φ, ω, ρ — **likely relevant to high-Q²**

Diffractive exclusive vector-meson production. The 12 samples span the full HERA-II e+/e− periods. The DIFFVM generator extends across all Q² (samples produced with no Q² generator cut — full DIFFVM kinematics).

Production rate at high Q²: the DIFFVM cross-section falls steeply with Q² (typically as Q⁻⁴ to Q⁻⁶ depending on the VM), but is non-zero. The high-Q² analysis needs to assess:
- Does exclusive ρ→π⁺π⁻ at high Q² fake the SIDIS measurement?
- Does φ→K⁺K⁻ at high Q² leak into the analysis?
- What's the cross-section magnitude in the Q²>150 fiducial?

A quick yield check on the existing low-Q²-built DIFFVM trees with high-Q² analysis cuts (Q²>150 instead of Q²<100) will tell you whether the contribution is non-negligible at high Q².

| Meson | Sample IDs | L_MC (pb⁻¹ each) | Total L_MC |
|-------|-----------|------------------|------------|
| φ | 8408 / 8413 / 8418 / 8423 | 3,863-3,866 | 15,458 |
| ω | 8409 / 8414 / 8419 / 8424 | 7,729-7,734 | 30,923 |
| ρ⁰ (all-decay) | 8445 / 8446 / 8447 / 8448 | 645.20-645.69 | 2,582 |

Files on DUST:
- φ: `output/mc/diffvm_phi_{8408,8413,8418,8423}/` (~17 GB total)
- ω: `output/mc/diffvm_omega_{8409,8414,8419,8424}/` (~19 GB)
- ρ: `output/mc/diffvm_rho_{8445,8446,8447,8448}/` (~19 GB)
- Total: ~55 GB

L_MC values are from H1 IMC database (`/afs/desy.de/group/h1/web/internal/imc/h1db.js`, the per-sample `LUMI` field). `oomclumi` does NOT work for DIFFVM — use IMC values directly.

Weights: `weight1 = weight2 = 1.0` (DIFFVM is unweighted).

**Important — ρ samples:** use only the **all-decay** samples (8445/8446/8447/8448), NOT the charged-only set (8407/8412/8417/8422) which would double-count π⁺π⁻.

Detailed per-sample reference card: `H1/sidis_lowQ2/docs/handoffs/background_mc_diffvm.md`.

### 2. PYTHIA64 9261 PHP (Q²<2) — **unlikely relevant to high-Q²**

Generator cut Q² < 2 GeV². Cannot migrate to Q²>150 reco fiducial (resolution doesn't span ~7 decades of Q²). Listed here for completeness only.

If the high-Q² analysis ever extends to lower Q², the sample exists with a known WTX2 normalization. Detailed card: `H1/sidis_lowQ2/docs/handoffs/background_mc_pythia_9261.md`.

### 3. DJANGOH14 7080+7081+7082+7083 (gen Q²>2) — **unlikely relevant to high-Q²**

Generated with Q² > 2 GeV² cut, used by low-Q² as a gap-filler for the Q² ∈ [2, 5] region. The samples DO extend to high Q² (up to ~200 GeV² per the gen log) but with steeply falling statistics. At Q² > 150 the statistics from these samples are negligible compared to a properly-Q²>150-generated DJANGOH sample.

For high-Q² migration, the analogous nominal high-Q² DJANGOH/RAPGAP samples already in your production should suffice.

Detailed card: `H1/sidis_lowQ2/docs/handoffs/background_mc_djangoh_qgt2.md`.

---

## Sharing options for DIFFVM (the high-relevance set)

Three options, ordered by effort and risk:

### Option A — reuse low-Q²-built trees as-is
**Effort:** zero (files already exist on DUST)
**Risk:** medium — if the high-Q² analysis needs `GenScatE{X,Y,Z,E}` or the other 11 ChargedTotGen branches the high-Q² builder uniquely produces, this won't work.
**Test:** open one low-Q²-built DIFFVM tree and check whether the high-Q² analysis selection script can compute everything it needs from the available branches. If yes, use as-is.

### Option B — reproduce DIFFVM with the high-Q² builder
**Effort:** ~10-15 min Condor (~1200 jobs, mirrors the prior DIFFVM low-Q² production)
**Risk:** low — full schema parity; identical trees as if produced natively by the high-Q² analysis
**Disk:** ~55 GB total; replaces the low-Q²-built copies (need to decide: keep both or just the high-Q²?)
**Cleanest path:** produce one canonical copy with the high-Q² builder; both analyses point at the same files. Low-Q² compatibility maintained because high-Q² is a schema superset.

### Option C — share the v5g binary
The low-Q² builder source is at `H1/sidis_lowQ2/src/ChargedSidisTreeBuilderLowQ2.C`; high-Q² at `H1ChargedSidis_HighQ2/src/`. Look for the diff and consider unifying the two source trees so that a single binary produces a single schema. Larger upfront effort but eliminates the schema-divergence problem permanently.

**Recommendation: Option B.** ~10 min Condor cost saves ~55 GB and gives both analyses canonical trees.

---

## How to use the samples (recipe)

Whichever path you choose, the application pattern is:

1. **Apply the mainstream high-Q² selection** (whatever your equivalent of low-Q²'s `d1_extract_yields.py` is) to each DIFFVM sample.
2. **Per-event weight = 1.0** (DIFFVM is unweighted).
3. **Per-sample scale = L_data_high_Q² / L_MC_sample** where L_MC values are from the table above × 1000 to convert pb⁻¹ → nb⁻¹.
4. **No additional gen filter** (the entire DIFFVM sample is exclusive VMD by construction).
5. **Sum across the 12 samples** to get the total DIFFVM contribution per (Q², x, z, pT_h, phi_h) cell.
6. **Subtract per-cell** from your data yield (or include in your background-systematic budget).

L_data for current high-Q² charged-SIDIS: use `351000.0 nb⁻¹ = 351 pb⁻¹` for the 0304-included four-period S67 sample unless `docs/RUN_STATUS.md` or the handoff supersedes it. The older direct-monitor `356613.0 nb⁻¹ = 356.613 pb⁻¹` quote is superseded. The `the superseded pre-rescale luminosity nb⁻¹` value is 2005-2007 only and must not be used for 0304-included high-Q² charged-SIDIS.

---

## Existing files at-a-glance

Everything under `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5g/output/mc/`:

| Group | Subdirs | N files | Total size |
|-------|---------|--------:|-----------:|
| DIFFVM φ | `diffvm_phi_{8408,8413,8418,8423}/` | ~400 | ~17 GB |
| DIFFVM ω | `diffvm_omega_{8409,8414,8419,8424}/` | ~400 | ~19 GB |
| DIFFVM ρ all-decay | `diffvm_rho_{8445,8446,8447,8448}/` | ~400 | ~19 GB |
| PYTHIA 9261 (PHP) | `pythia_9261/` | 91 | ~16 GB |
| DJANGOH 7080-7083 (gap-filler) | `djangoh_{7080,7081,7082,7083}/` | 650 | ~110 GB |

---

## Open questions for the high-Q² supervisor

1. **Does the high-Q² acceptance correction need `GenScatE{X,Y,Z,E}` from ChargedTotGen** (only present in high-Q²-built trees)? If yes, Option A is not viable and Option B is needed for any reuse.
2. **Does the high-Q² SIDIS measurement include exclusive VMD as a background source?** If yes, the DIFFVM samples are a free deliverable.
3. **Resolved L_data convention:** current 0304-included high-Q² charged-SIDIS uses `351000.0 nb⁻¹ = 351 pb⁻¹`; the older direct-monitor `356613.0 nb⁻¹` quote is superseded, and `the superseded pre-rescale luminosity nb⁻¹` is 2005-2007 only. Per-sample scale factors still require sample-identity and effective-MC-lumi verification.

---

## Coordination note

The low-Q² analysis is in a handoff state to its data analyst; that handoff has the same content for the three sample groups at:
- `H1/sidis_lowQ2/docs/handoffs/background_mc_samples_handoff.md` (master)
- `H1/sidis_lowQ2/docs/handoffs/background_mc_diffvm.md` (DIFFVM card)
- `H1/sidis_lowQ2/docs/handoffs/background_mc_pythia_9261.md` (PHP card)
- `H1/sidis_lowQ2/docs/handoffs/background_mc_djangoh_qgt2.md` (gap-filler card)

If you adopt Option B (reproduce with the high-Q² builder), the low-Q² analysis can point at the resulting high-Q²-built trees too (schema superset). Coordinate placement: shared location like `/data/dust/group/h1/klesthen/BaryonAsym/shared_mc/diffvm_*/` keeps both analyses pointing at a single canonical set.
