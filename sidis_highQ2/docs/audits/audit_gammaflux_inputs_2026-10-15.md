# Audit: kinematic inputs for the Hand-convention virtual-photon flux Γ(x, Q², y; E_e, E_p)

**Date:** 2026-10-15 · **Type:** read-only schema/branch audit · **Constraint:** NAF login node, ≤1 DATA + ≤1 MC ROOT file opened (uproot 5.6.9), no products touched, no multi-file loops.

**Question:** can the released high-Q² charged-hadron SIDIS ep cross sections be converted to γ*p cross sections by dividing per bin by the Hand flux, using only what already exists (no tree-builder change)?

**Files opened (ROOT, schema level + ≤200-entry samples):**
- DATA: `H1ChargedSidis_HighQ2/output_v6/central/sidistree/data/ep0607/sidistree_ep0607_d07_0000.root` → `ChargedTot` only, 8,713 entries, 100 branches (no `ChargedTotGen`, no `DisTot` — `DisTot` is booked but "DETACHED in v6sys (not written)", builder line 493).
- MC: `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root` → `ChargedTot` 313,776 entries; `ChargedTotGen` 1,936,073 entries, 60 branches.

---

## 1. Per-bin level (release directories): y, ε, W, beam energies, √s — **ABSENT**

- `bin_map.csv` / `bin_map.json`: `q2_min/q2_max/x_min/x_max` (+ z, pT edges) only.
- `ibu/cross_sections.csv` and `bbb/cross_sections.csv` (both releases, header inspected): bin-edge columns + `yield_unfolded`, `cross_section_nb`, the seven `*_nb`/`*_yield` uncertainty columns, `total_unc_rel`, `acceptance_rapgap`, `prior_gen_rapgap`, method/variant/caveat strings. **No y, no epsilon, no W, no beam-energy, no √s column.**
- `metadata.json`, `summary_totals.json`, `manifest.json`, `SELECTION.md`: no per-cell kinematics either (`grep -i "epsilon|y_min|y_max|beam_e|sqrt_s|w_gev"` over all release CSV/JSON/MD → zero hits). `L_data = 351000.0 nb⁻¹` is the only normalization constant stored.
- Consequence: a per-bin Γ must be **computed** — either per event from the trees (clean; supported, see below) and averaged per cell, or approximated at bin centres with an assumed s. (Q², x) + fixed s does fix y = Q²/(x·s), but the cell spans a Q²–x region and the release stores no s.

## 2. Event level in the analysis trees

### `ChargedTot` (reco; the data vector input) — **EXISTS**
- DIS kinematics, all methods: `Q2e/Ye/Xe` (electron), `Q2s/Ys/xBj/Ws` (Sigma; W present here), `Q2da/Yda/Xda` (double-angle), **`Q2es/Yes/Xes` (e-Sigma — the binning triple)**, `Q2h/Yh/Xh` (hadron), `Empz` (E−pz; the "Sigma" input).
- **Beam energies per event: `BeamElecE`, `BeamProtP`** (filled from HAT header `hatEBeamE`/`hatEBeamP`, builder lines 1122–1125). Sampled values: data d07 file → 27.611 / 919.125 GeV (constant within the file); MC → 27.600 / 920.000 GeV. These directly tag the beam configuration, including any 575 GeV period.
- **Scattered-electron four-vector: `scatEX, scatEY, scatEZ, scatEE`** → k′ reconstructable; q = k − k′ with k = (0,0,−E_e, E_e) from `BeamElecE`; proton beam P = (0,0,p,√(p²+M_p²)) from `BeamProtP`.
- Also: `ystar_es, yBreit_es, xF_SIDIS_es, z_SIDIS_es, ylab, ystar, yBreit, xF_SIDIS, z_SIDIS`, `PVz`, `RunNumber`, `EventNumber`, `weight1`, `weight2`.
- MC-only extras on `ChargedTot` (placeholders −999 in data, real in MC): `Q2sGen, YsGen, xGen, ystarGen, yBreitGen, xFGen, zGen, Q2_es_gen, Y_es_gen, x_es_gen, GenEnPhoton, ystar_es_gen, GenElecE, GenElecTheta, GenElecPhi, inFiducialChargedMatch`.

### `ChargedTotGen` (gen; MC only) — **EXISTS**
- Gen DIS kinematics, **all methods**: `Q2eGen/YeGen/XeGen`, `Q2sGen/YsGen/xGen`, `Q2daGen/YdaGen/XdaGen`, **`Q2esGen/YesGen/XesGen`**, `Q2hGen/YhGen/XhGen`, plus release-safe `Q2_es_gen/Y_es_gen/x_es_gen`, `GenEnPhoton`, `GenEmpz`, `ystarGen, yBreitGen, xFGen, zGen, ylab`.
- **Gen beam energies: `GenBeamElecE`, `GenBeamProtP`**; **gen electron four-vector: `GenScatEX/GenScatEY/GenScatEZ/GenScatEE`** and standardized aliases **`GenElecPx/GenElecPy/GenElecPz/GenElecE/GenElecTheta/GenElecPhi`**; `RunNumber`, `EventNumber`, `weight1`, `weight2`.

### **ABSENT** (named branches, in both trees)
- `ElecE / ElecTheta / ElecPhi` (reco electron angles) — not branched on `ChargedTot`; **derivable from `scatEX/Y/Z/EE`**. They DO exist on the stage-1 `EvlTree` (`ElecE/ElecTheta/ElecPhi`, stage-1 builder lines 478–480), whose outputs live under `output_v6/central/evls/`.
- `Sigma` (as a branch; the E−pz quantity stored is `Empz`), `W` (only `Ws`, Sigma method, on `ChargedTot`; gen W derivable from `Q2sGen/YsGen` or four-vectors), `W2`, `SqrtS`/`s_hat`, `epsilon`, `Gamma`, `GenQ2/Genx/Geny` (these exact names don't exist; the `*Gen` method branches above are the equivalents).
- `DisTot` (event-level tree): booked but not written in v6.

## 3. Provenance of the binning variable — **e-Sigma method (`Q2es`, `Xes`)**

- Reco cell: `build_data_vector_v4_alphaveto.py` line 492 `i_cell = cell_index_reco(Q2es, Xes)`; event cuts lines 66/209/434–436 (`0.1 < Yes < 0.7`), 67/208/431–433 (`150 < Q2es < 20000`), `Empz > 45`. Directory: `H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_2026-10-11/`.
- Gen cell (response truth side): `build_migration_th2_per_charge_cone_s2prime.py` lines 1001–1010 (prefer `Q2esGen` + `x_es_gen`, fallback `XesGen`/`XeGen`), line 1031 `i_cell = cell_index_gen(Q2_bin, x_bin)`.
- Grid definition: `binning_v3.py` lines 28–102 (`Q2_STRATA_GEN`, `X_EDGES_GEN_PER_STRATUM`, `cell_index_gen` line 93, `cell_index_reco = cell_index_gen` line 102).
- The pure electron method (`Q2e/Xe`) exists as branches but is NOT the binning variable. C_rad grid confirmed release-safe `(Q2esGen, x_es_gen)` (release `SELECTION.md` line 49).

## 4. Run/period tagging — **three levels available**

- Directory: `output_v6/central/sidistree/data/{ep0304, em0405, em06, ep0607}/` (658 files: 83/152/100/323 per context-handoff).
- File name: `sidistree_<period>_d<YY>_<NNNN>.root` (e.g. `sidistree_ep0607_d06_0000.root`, `..._d07_0144.root`) — year-level split only (d03/d04/d06/d07). Caveat (context-handoff, BSA section): **`ep0607` mixes lepton charges across d06 (e−) and d07 (e+)** — directory stratification alone is insufficient; use run numbers.
- Per event: `RunNumber` on both trees (sampled d07 file: 492560; MC: 444307–444311) → run-range splits; and **`BeamElecE`/`BeamProtP` give the actual beam configuration per event** (27.611/919.125 in the sampled 2007 e+ file). A 575 GeV proton period, if present, is directly identifiable per event via `BeamProtP ≈ 575` — no builder change needed for the split or its verification. Note: per `docs/research/research-LT-separation-feasibility.md` (§ around lines 457–470), the MER 575 GeV / LER 460 GeV ODS samples have **not** been processed by this builder — the current central v6 sample is expected to be 920 GeV only.
- MC: `sidistree/mc/{rapgap31,djangoh14}/<MCDB genID>/sidistree_<gen>_<genID>_<period>_<NNNN>.root` (genIDs 7071/7094/7096/7097 for rapgap31); fixed gen beam energies via `GenBeamProtP`/`GenBeamElecE`.

## 5. Verdict — **YES: per-event Hand flux Γ is computable from existing branches, no builder change**

Per event on `ChargedTot` (data): x = `Xes`, Q² = `Q2es`, y = `Yes` (or y = Q2es/(x·s)), s = 4·`BeamElecE`·`BeamProtP` (√s ≈ 319 GeV at 920 GeV), and the full four-vector route (k from `BeamElecE`, k′ from `scatEX/Y/Z/EE`, P from `BeamProtP`) for ε = [1 + 2(1+ν²/Q²)·…] or any equivalent Hand-convention form. The gen side (`ChargedTotGen`) is equally complete (`Q2esGen/YesGen/XesGen` + `GenBeamElecE/GenBeamProtP` + `GenScatE*`/`GenElec*` + `GenEmpz`).

**No missing branch is blocking.** Convenience gaps only (all derivable): no stored ε, Γ, √s, or gen-W branch; no reco `ElecTheta/ElecPhi` (use `scatE*`); no per-bin y in the release tables (compute per event and average per (Q²,x) cell, matching the `Q2es/Xes` binning and the `0.1 < Yes < 0.7` event cut). The only analysis decision — not a builder change — is the per-cell Γ averaging convention (event-weighted mean over the released reco binning vs bin-centre evaluation), and, if any non-920 GeV events were ever admitted, period-resolved Γ via the per-event `BeamProtP`.

**Stale-path note:** `sidis_highQ2/scripts/extract_pertrack_v5g_fixedFrame_v3.py` (named as a place to look) does **not exist** at that path (nor anywhere under `sidis_highQ2/scripts/` or `H1ChargedSidis_HighQ2/` at depth ≤ 2; only old output dirs `H1ChargedSidis_HighQ2/output/pertrack_v5g_fixedFrame*` remain). The canonical snapshot `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/` was read instead (RUNLIST.md).
