# Low-Q² charged SIDIS: binning study plan (x, Q², z, pT_h) coupled to unfolding stability, 2026-10-22

**Goal.** Find the most differential (Q², x, z, pT_h) binning that can be unfolded stably, separately for h+ and h−. Binning and unfolding are decided together: a binning is accepted only if resolution, statistics and unfolding-stability gates all pass.

**Inputs.** Wave A production, validated by cluster 5817184:

| Sample | Path under `H1ChargedSidis_LowQ2/output_v6/campaigns/` | Chunks |
|---|---|---|
| data 2005–07 | `waveA_data_2026-10-21/<label>/sidistree.root` | 577 |
| DJANGOH14 rad | `waveA_djangoh14_2026-10-21/...` | 3951 |
| RAPGAP31 rad | `waveA_rapgap31_2026-10-21/...` | 2686 |

Trees: `ChargedTot` (reco tracks plus matched truth), `ChargedTotGen` (gen hadrons), `DisTot` (events). Authority for every convention: `sidis_lowQ2/STATE.md`.

## Fixed conventions (high-Q² OPT-A parity; do not re-decide)

- **Reco fiducial:**
  - 5 ≤ `Q2es` < 100, 0.1 < `Yes` < 0.6.
  - 45 < `Empz_calc` < 70. Use the shift-aware `Empz_calc`, not the HAT `Empz`.
  - |`PVz`| < 35, `PassRecoEnvelope`==1, current hemisphere `xF_SIDIS_es` > 0.
  - Track cuts: the high-Q² `eta1p6_radlen5_nhit5` set (verify the exact definitions in the high-Q² data-vector builder).
- **Gen fiducial (D2):**
  - 5 ≤ `Q2_es_gen` < 100, 0.1 < `Y_es_gen` < 0.6, `GenEmpz` > 45, `xFGen` < 0.
  - |GenPDG| ∈ {211, 321, 2212}.
  - NO gen detector cuts.
- **OPT-A:** the gen Q² *cell index* uses `Q2esGen`, mirroring the C_rad booking. The fiducial uses `Q2_es_gen`.
- **Truth classes (D4, v3 rule):** STR-only links are background; every GTR-linked match goes in the response body. Use plain `IsMatched` + hadron PDG for this study.
- **Weights:** body `weight1*weight2*NucliaWeight`, miss and gen `weight1*weight2`. MC is normalised to data luminosity.
- **Variables:** z = `z_SIDIS_es` (reco) / `zGen` (gen).
  - pT_h is **not stored**. Compute it offline in the photon-axis HCM (γ*p) frame from track 4-vectors and the scattered electron (`scatE*` reco, `GenScatE*` gen) with nominal beams.
  - Port the high-Q² implementation (`H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_optA_2026-10-19/`, pT in the nominal-beam CM) and cross-check against high-Q² on one file.
- **Structure rules (template):**
  - x is binned per Q² stratum along the kinematic band, not on a global grid.
  - z is 1:1 reco:gen with the first edge at 0.05 (`zlow005`).
  - pT_h reco is 2:1 finer than gen.
  - `n_reco ≥ n_gen`.
- **Unfolding nominal:** IBU with the coherent prior (truth marginal body+miss). k is scanned. TUnfold is a diagnostic only.

## Workstreams

### WS1: fine-grained "atom" accumulator (Condor). Unblocks WS2–WS4
Book everything once on a fine grid ("atoms"), so any candidate binning becomes a cheap rebin and no re-reading is needed.

- **Atoms:**
  - Q² in 24 log bins over [3, 150];
  - x in 30 log bins over [1e-5, 0.1];
  - z in 20 bins over [0, 1];
  - pT_h in 24 bins over [0, 3] GeV (finer below 1 GeV);
  - charge ±.
- **Content per MC sample (DJ14, RG31 separately) and per charge.** Store as sparse COO numpy (`.npz`), additively mergeable.
  - The response body: sparse (gen atom → reco atom) sum of w and w².
  - Gen totals, misses (gen-fid, no fid reco match), and fakes (reco-fid without a gen-fid match), each w, w².
  - Residual histograms for σ68: ln(Q2es/Q2_es_gen), log10(Xes/x_es_gen), z_reco − z_gen, pT_reco − pT_gen and pT ratio. They are booked per gen atom-group: Q²(6) × x(6) × z(5) × pT(5).
- **Data:** reco-atom counts per charge (no weights).
- **Event-level QA:** reco-box event counts per period.
- **Steps:**
  1. Write `scripts/binning/accumulate_atoms.py` (one file → one `.npz`) plus a merge script.
  2. Login-node smoke on 1 data + 1 MC file (≤5 min).
  3. **Check that `waveA_*/validation.json` reports `n_failed: 0` for all three campaigns before the full Condor submit.**
  4. Submit via a `make_campaign`-style submit with all output dirs pre-created.
  5. Merge, then write `output_v6/studies/binning_2026-10-22/atoms/{dj14,rg31,data}_{pos,neg}.npz` + README.

### WS2: resolution and candidate generator (after WS1)
- σ68 maps per variable vs (Q², x, z, pT) from the residual histograms.
- **Candidate family:** for k ∈ {1.0, 1.5, 2.0}, gen bin width ≥ k·σ68 locally. Q² strata are chosen first, then x bands per stratum, then z, then pT; reco is 1:1 in z and 2:1 in pT.
  - Also include the existing Candidate B (`H1SIDIS_LowQ2_v5g/unfolding_highq2parity_2026-09-23/config/frozen_candidate_B.json`) and a coarse template-like candidate (4 Q² strata × 2 x, 7 z, 5 pT).
- **Output:** `candidates/*.json` (edges plus metadata), all satisfying `n_reco ≥ n_gen`.

### WS3: occupancy and statistics per candidate (after WS1; can run alongside WS2)
- Per cell: data reco counts, MC effective N (Kish) for gen, body and miss, and the MC/data luminosity ratio.
- **Provisional gates** (WS5 refines them): data reco ≥ 100 per reco cell; MC n_eff ≥ 100 (truth) and ≥ 50 (matched).
- **Output:** per-candidate occupancy tables plus a list of failing cells.

### WS4: unfolding stability scan per candidate (after WS2+WS3)
Run per candidate, per charge, from the atoms.

- **Response and migration:**
  - Build R from the body plus misses/efficiency, with fakes subtracted pre-unfold (fake fraction from MC).
  - Purity = correct/all-reco and stability = correct/all-gen, per bin, with the full denominators.
  - Condition number of R, and the effective rank.
- **IBU:** k = 1…10 with the coherent prior. Choose k where the change in the result falls below the stat error.
- **Closure:**
  - 200 Poisson toys of reco pseudo-data, with a split-half response (statistically independent).
  - Same-generator and cross-generator (DJ↔RG): bias/σ, pull width, coverage, refold χ², negative bins.
- **TUnfold diagnostic:** `n_reco ≥ n_gen`, L-curve/ScanTau, global ρ. Not used for selection unless an interior τ passes all gates.
- **Output:** a per-candidate scorecard JSON, plus failing cells with the reason.

### WS5: precedent and gate thresholds (independent; start now)
- Survey bin-width and purity/stability practice for comparable measurements:
  - H1 low-Q² charged-particle / SIDIS papers, e.g. arXiv:1302.1321 and the H1 TMD/hadron multiplicity analyses;
  - ZEUS; HERMES and COMPASS multiplicities (k·σ rules, purity/stability floors, min stats).
- **Output:** a recommended gate set with citations: purity floor, stability floor, k, min N, closure bias/pull limits, cond(R) limit.

### Synthesis (me)
- Pick the most granular candidate whose cells pass ALL gates. Allow local merges in failing (Q², x) cells (like `S3xmerged`).
- Report to the user for approval before freezing.
- Then record it in `STATE.md` and freeze the config at `output_v6/studies/binning_2026-10-22/FROZEN_binning.json`.

**Dependencies:** WS1 ∥ WS5 → (WS2 ∥ WS3) → WS4 → synthesis.

**Rules for all agents:**
- Login node: ≤5 min, one file. Use Condor for multi-file work, and create every output dir before submitting.
- No loose files at the remote top level; write only under `H1ChargedSidis_LowQ2/{scripts/binning,output_v6/studies/binning_2026-10-22,condor_v6/binning_2026-10-22}`.
- zsh on NAF: quote `"====="`. Use `bash -c` for condor commands, and `+` instead of commas in queue-from args.
- Never modify Wave A outputs or other analyses' files.
