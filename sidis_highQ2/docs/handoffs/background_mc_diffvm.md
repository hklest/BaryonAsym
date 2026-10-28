# DIFFVM exclusive φ/ω/ρ samples — reference for high-Q²

## Why this is here

DIFFVM exclusive vector-meson production happens at all Q² (samples generated without a Q² upper cut). For the high-Q² SIDIS measurement at Q² > 150 GeV², the cross-section is much smaller than at low Q², but a quick check is worthwhile because:
- exclusive ρ → π⁺π⁻ in the Q²>150 fiducial fakes 2-prong SIDIS events
- exclusive φ → K⁺K⁻ contributes similarly
- the per-meson cross-section ratios and t-slope behaviors are well-studied (H1 2010 published)

The low-Q² analysis already produced v5g trees for all 12 samples. They may or may not be directly reusable by the high-Q² builder — see schema notes in `background_mc_samples_handoff.md`.

## Sample inventory

12 samples organized by meson, beam, and period:

### φ → K⁺K⁻ (4 samples, 4 periods)
| Sample | Beam | Period | IMC LUMI (pb⁻¹) |
|--------|------|--------|----------------:|
| 8408 | e+ | 03-04 | 3,863.40 |
| 8413 | e− | 04-05 | 3,863.67 |
| 8418 | e− | 2006 | 3,865.61 |
| 8423 | e+ | 06-07 | 3,865.25 |

Total L_MC = 15,458 pb⁻¹ ≈ 52× a 297 pb⁻¹ data sample.

### ω → π⁺π⁻π⁰ (4 samples)
| Sample | Beam | Period | IMC LUMI (pb⁻¹) |
|--------|------|--------|----------------:|
| 8409 | e+ | 03-04 | 7,734.03 |
| 8414 | e− | 04-05 | 7,730.50 |
| 8419 | e− | 2006 | 7,729.81 |
| 8424 | e+ | 06-07 | 7,728.57 |

Total L_MC = 30,923 pb⁻¹ ≈ 104× data.

### ρ⁰ → π⁺π⁻ (all-decay, 4 samples)
| Sample | Beam | Period | IMC LUMI (pb⁻¹) |
|--------|------|--------|----------------:|
| 8445 | e− | 04-05 | 645.50 |
| 8446 | e− | 2006 | 645.69 |
| 8447 | e+ | 06-07 | 645.34 |
| 8448 | e+ | 03-04 | 645.20 |

Total L_MC = 2,582 pb⁻¹ ≈ 8.7× data.

**Use only the all-decay set (8445-8448).** The charged-only ρ samples (8407/8412/8417/8422) cover the same physical process and would double-count π⁺π⁻ if summed with the all-decay set.

## File locations on DUST

`/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2_v5g/output/mc/`:
- φ: `diffvm_phi_{8408,8413,8418,8423}/`
- ω: `diffvm_omega_{8409,8414,8419,8424}/`
- ρ: `diffvm_rho_{8445,8446,8447,8448}/`

Each sample has ~100 ROOT files of ~150 MB each. Total ~55 GB across all 12.

## Normalization

DIFFVM samples are **unweighted**: `weight1 = weight2 = 1.0` for every event. Per-event weight to apply = 1.0.

**Important — `oomclumi` does NOT work for DIFFVM** (the per-event GKI SIGMT field is unfilled by the DIFFVM generator). Use the IMC LUMI values directly:
```
L_MC_nb = IMC_LUMI_pb × 1000
scale_per_sample = L_data_nb / L_MC_nb
N_pred = (selected_track_count) × scale_per_sample
```

## Schema check for high-Q² use

Per the schema comparison in the master handoff:
- ChargedTot from low-Q² builder is a **subset** of high-Q² builder's ChargedTot (high-Q² has 5 extra branches: `GenEnPhoton`, `Q2_es_gen`, `Y_es_gen`, `x_es_gen`, `ystar_es_gen`)
- ChargedTotGen from low-Q² is **missing** 11 branches the high-Q² version produces, notably `GenScatE{X,Y,Z,E}` (gen scattered electron 4-vector)

If your high-Q² analysis needs those gen-electron branches for HCM-frame computation, the low-Q²-built DIFFVM trees won't work directly — you'd need to reproduce with the high-Q² builder.

## Recommended path

Reproduce DIFFVM with `ChargedSidisTreeBuilderHighQ2` once, store at a shared location, both analyses use the same files.

- Job count: 1,200 (100 HAT files × 12 samples)
- Wall: ~10-15 min on Condor (per the low-Q² production timing)
- Output size: ~55 GB

## How to use

After production (or after deciding the low-Q²-built version works for your analysis):

1. Apply the mainstream high-Q² SIDIS selection to each sample (Q² > 150 event cut, etc.).
2. Per-event weight = 1.0.
3. Per-sample N_pred at L_data = `count × (L_data_high_Q² / L_MC_sample_nb)`.
4. Sum across all 12 samples for the total DIFFVM contribution.
5. Per-cell subtract from data, or include as part of the background systematic budget.

## Suppression options to bear in mind

The low-Q² analysis found that DIFFVM exclusive VMD is dominantly 2-prong (φ, ρ) or low-multiplicity 3-prong (ω → 3π). An **N_central ≥ 3 charged-track event cut** suppresses DIFFVM by ~30× while costing only ~5% of SIDIS signal. If the high-Q² analysis adopts a similar cut, DIFFVM may be reducible to <0.5% of yield and absorbable into systematic uncertainty.

## References

- IMC entries: `/afs/desy.de/group/h1/web/internal/imc/h1db.js`, search `SetrequestDB(8{408,413,418,423,409,414,419,424,445,446,447,448})`
- Source of L_MC values: per-sample `LUMI` field in each `SetgenDB` entry
- Companion low-Q² card: `H1/sidis_lowQ2/docs/handoffs/background_mc_diffvm.md`
- DIFFVM normalization research history: `H1/sidis_lowQ2/data/php_background_2026-05-22/diffvm_normalization_research.md`
- Exclusive-phi analysis (uses DIFFVM as signal MC): `H1/exclu_phi/docs/research-mc-luminosity.md`
