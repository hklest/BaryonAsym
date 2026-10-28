# Critic review of `nonprimary_norm_rel` v3 before baselining (2026-10-21)

Three independent hostile critics reviewed the term, all read-only on NAF.

| critic | angle | verdict |
|---|---|---|
| 1 | physics / definition | **UNSOUND as constructed**. The gap is real, and a revised term is baselinable. |
| 2 | statistics / unfolding | **SOUND-WITH-CONDITIONS** |
| 3 | implementation / independent reproduction | **SOUND-WITH-CONDITIONS** |

Critic scratch on NAF: `H1ChargedSidis_HighQ2/output_v6/studies/trackclass_variant_2026-10-20/critic2/` and `critic3/`.

## What is established (verified by the critics)

- **Arithmetic is exact.** Critic 3's own code, run from the raw shards, reproduces every CSV cell to
  ≤ 2e-15:
  - integrated rel(±1): h+ −0.9093% / +0.9330%, h− −0.8008% / +0.8192%;
  - per-bin maxima 2.6008% / 3.4210%;
  - DJ coherent 1.0562% / 0.9322%;
  - nominal unfold vs the release: 1.6e-15.
- **Builder is clean.** With `TC_CLASSES=keeper`, the only change is the keeper move:
  - body loss = `h_keeper_retained_body` (4e-11);
  - fake gain = `h_fake_tc_keeper`;
  - miss row, prior and fc0/3/4/5 identical;
  - no keeper partner in the gen pool (593 = 335 + 258).
  - The null shard is content-identical (68/68). Budget base cells are verbatim, totals reproduce,
    and the covariance is PSD rank-1.
- **Emulation is right** for an additive background. Unfolding pseudo-data containing κC through
  the shifted response returns the κ=0 result to 1e-16. The emulated correction equals minus the
  true bias to 0.4% (median).
- **Regularization and prior.** It is robust to k (integrated −0.892 … −0.910% for k = 1–10; per
  bin, k=3/5 vs k=4 is about MC σ) and to realistic prior shapes (ratio 0.96–1.03).
  - Engine note: `ibu_unfold`'s P argument is degenerate for such ratios, so prior tests must use
    an own IBU.
- **Bootstrap is validated.** Split-half pulls have RMS 1.01–1.05. The MC-stat median 0.05% is
  credible. Data stat on the term is negligible.
- **Sign flips (5 h− bins) are mostly real:** IBU anticorrelation with P > 0.94 in 3 bins; the other
  two are ≤ 0.06%.
- **The gap is real.** No existing term varies the rate of STR-only π/K/p:
  - `bgsub_fakefrac_rel` is fc0-only;
  - `detint_removal_rel` is the DJ–RG removal-increment spread.

## Conditions raised (consolidated)

| # | severity | issue | raised by | resolution options |
|---|---|---|---|---|
| C1 | **BLOCKER** | Term and nominal define the observable differently. The term treats the K± keeper as fake; the OPT-A nominal books it in the body. S2' ruling 2 ("kaons which decay are still signal") is not superseded. | 1, 2, 3 | (a) drop fc5+fc8 from the term: compute it on the nominal response, det.int. only — critic 3: 0.854% / 0.751%, per-bin max 2.49% / 2.95%; or (b) promote the keeper reroute to the nominal (−0.033% / −0.028%), recording that it supersedes ruling 2, and regenerate the base budget on it |
| C2 | MAJOR | A single κ shared by bins **and charges** gives a rank-1 covariance. That hides the charge-asymmetry uncertainty: h+/h− integrated 0.11% at ρ=1 vs **1.21% at ρ=0** (critic 2). Sources differ: interaction-born (34.5%), hyperon feed-down (56.5%), K-DIF (9%). | 1, 2 | ≥ 3 source nuisances (interaction, hyperon, keeper) with per-class charge correlation; quote h+/h− and A_h at ρ=0 and ρ=1. Ξ→Λπ links the hyperon piece to the env100 V0 piece. |
| C3 | MAJOR | κ = ±100% has no physical basis. κ = −1 (no interactions, no hyperon decays) is physically excluded. H1 1302.1321 constrains secondary interactions to < 1% after the π/K cross-section correction. | 1, 2 | derive κ per class (H1NuclIACor uncertainty; strangeness / Λ data vs JETSET), or label it explicitly as a conservative fiat like env100. For κ < 1, take the RG/DJ shape envelope (DJ/RG per bin 0.6–3). |
| C4 | MAJOR | Not fully provenance-only: STR-only non-π/K/p tracks, mostly μ from π/K decay in flight (0.59–0.63% of selected, ≈ det.int. size), sit in fc0 with only the 1.8% `bgsub_fakefrac`. K→μν and K→ππ⁰ daughters are treated differently. | 1 | one κ policy for all STR-only classes, or a written justification |
| C5 | MAJOR | Interaction-born content is varied in isolation. A real interaction-rate change also moves primary loss (efficiency / tracking) and the GTR-linked absorbed secondaries (~1% of body). It is probably partially cancelling, so likely conservative, but not modelled. | 1, 2 | document that the correlated primary loss belongs to `tracking_eff_m0p5_rel`, or run one coherent interaction-rate variant |
| C6 | MAJOR (inferred) | NucliaWeight on secondary legs. NW < 1 says MC under-predicts nuclear interactions; then interaction-born secondaries are under-produced in MC, a possible central-value bias. | 1 | check what H1NuclIACor does and implies for STR legs; it may need a correction factor (links to the open 2026-10-20 NucliaWeight-convention item) |
| C7 | MINOR | `nonprimary_norm_mc_stat_rel` must **not** be added: `fakecol_mcstat_rel` already contains it. 18 / 33 bins are noise-dominated (term < 2σ). Keep as is (conservative); smoothing is optional. | 1, 2 | currently not added; keep it that way and document |
| C8 | MINOR | Partial double counting: the ±10% DJ/RG rate part sits in `ibu_model_rel` / `detint_removal_rel` as well. It is negligible (0.03% vs 0.39%). | 1 | document, or retire the rate part of `detint_removal_rel` |
| C9 | MINOR | The h− maximum (b244, 3.4%) is not robust: 2.8–5.7% with k, 2.0% with a flat or DJ prior, MC stat ±1.4%. | 2 | don't quote it as a robust maximum |
| C10 | MINOR | Provenance. RG keeper and null shards were built with the builder before its 01:14 edit (the edit only touched the charge-flip switch of the absorbed class, so numerically equivalent). `summary.json` construction text is stale (v2). "det.int." includes fc5 (keeper gen-fail). | 1, 3 | rebuild the 73 RG + null shards with the current file (or record it); fix the texts |
| C11 | MINOR | Born level: assumes C_rad is per-bin multiplicative. The covariance should be converted with Born σ. | 2 | verify in the C_rad code |
| C12 | MINOR | **Inherited plot bug:** the 2026-10-19 env100 producer's panel text shows wrong Q²/x ranges (e.g. S1 300–500 vs 300–700, S3 1000–20000 vs 1500–20000, S0_x0 x 0.002–0.004 vs 0.0021–0.0071). The data are selected correctly by `cell_label`. This affects the existing official plots too. | 3 | fix the panel labels from the binmap (both producers) |
| C13 | note | The in-cone C3 diagnostic identity doesn't close on keeper shards (29 vs 28 unfilled). Results unaffected. | 3 | note only |

## Bottom line

Not baselinable **as is**. The implementation and statistics are sound. The definition must be
revised:
- C1: keeper consistency;
- C2: source-split nuisances with charge correlation;
- C3: a κ policy;
- C4: STR-only μ.

C5, C6 and C10–C12 also need closing or documenting. Several items need user decisions: C1, C3,
C4, and the C2 correlation choice.

## User decisions (2026-10-21) and resulting v4 construction

| item | decision / closure |
|---|---|
| C1 keeper | **Drop the keeper from the term.** The term is computed on the unchanged OPT-A nominal response (no keeper reroute). fc5 (keeper gen-fail) is not varied. No ruling is reversed and the central value is unchanged. |
| C3 κ | **±100%, explicitly labelled a conservative fiat** (env100 convention), not a measured rate uncertainty. |
| C2 correlation | **Split by source.** Independent nuisances, each fully correlated across bins: interaction-born, hyperon feed-down and STR μ. Charge correlation is evaluated at ρ=1 and ρ=0; h+/h− and A_h are given at both, and the larger is quoted. |
| C4 STR μ | **Included**: all STR-only classes in the fake column are varied (int also absorbs the tiny STR non-π/K/p/non-μ class). |
| C6 NucliaWeight | **Source checked** (`H1PhysUtils/H1NuclIACor.C`). `GetWeight = 1 − ani_ineff_(…)`, "corrections for nuclear i.a. undescribed by MC" (K. Daum), computed from reco kinematics for every MC track. So data have ~1.5% (absolute, per track) more nuclear interactions than MC. The weight corrects each track's own survival, including secondaries, which is correct; it does not correct the production rate of interaction-born secondaries. That rate is plausibly higher in data by 1.5%/P_int,MC (tens of %), a probable small under-subtraction that is covered by the ±100% interaction nuisance. P_int,MC is not available from the trees: inferred, documented. |
| C5 efficiency | Documented: the term emulates additive background only. The correlated primary-loss side belongs to `tracking_eff_m0p5_rel` / NucliaWeight and is not double counted. |
| C7 MC stat | Kept as an audit-only column (`nonprimary_norm_mc_stat_rel_audit_only`); never added. |
| C10 provenance | Shards rebuilt with the current builder (md5 c9af59df…, `TC_CLASSES=none`) in `shards/v4cat/` (Condor 5804013). The response is null-checked against the official merged response, and class sums close against the fake categories exactly (smoke: 0.0). |
| C11 Born | **Verified**: `apply_crad_and_diff_vs_cleanrelease_optA.py` l.100–122 multiplies every `*_nb` column by the per-bin C_rad from a fixed table, so the relative term is level-invariant. The Born covariance is the relative one × σ_Born,i σ_Born,j. |
| C12 plot labels | **Confirmed**: the 2026-10-19 env100 producer hard-codes wrong Q²/x panel labels. The binmap has S0 150–300 x 0.0021–0.0071 / 0.0071–0.03; S1 300–700 x 0.0042–0.016 / 0.016–0.069; S2 700–1500 x 0.0099–0.034 / 0.034–0.15; S3 1500–20000 x 0.021–1.5. The z/pT edges are correct. The v4 plotter reads labels from `bin_rows()`. The official producer is NOT modified (to be fixed in a dedicated plotting change). |
| C8, C9, C13 | Documented in the v4 record. b244 is not quoted as a robust maximum. |

## Round 2 (v4) — all three critics: SOUND-WITH-CONDITIONS (physics, statistics) / SOUND (implementation)

**Verified:**
- Implementation (critic 3): every v4 CSV column and summary number reproduced with independent code to ≤ 1.8e-14.
  - v4cat response equals the official one in all histograms on all 174 shards; the merged response is exact (0.0).
  - Closure int+hyp+kpar = fc3+4+5 to 6.8e-13 on every shard; kpar = fc5 exactly; strmu+strother ≤ fc0.
  - Covariances PSD, rank 3 (ρ1) / 6 (ρ0), off-charge block of ρ0 exactly 0.
  - Plots and labels verified against the binmap.
- Statistics (critic 2):
  - The ratio/A_h algebra matches an exact joint re-unfold (≤ 0.057% abs).
  - Additivity: joint shift = sum of single-source shifts (0.24% median deviation).
  - The MC-stat formula agrees with a direct bootstrap (ratio 0.99).
  - Robust to k = 3–6 and genuine prior changes; integrated ρ0 ratio stays 1.397–1.398% under every variation.
- Physics (critic 1): the round-1 blocker (keeper) is resolved; the source split, STR μ inclusion and fiat label are accepted.

**Open decisions (user):**

| # | issue | options / critic positions |
|---|---|---|
| D1 | strmu (π/K decay in flight, GEANT-decayed) is the largest source (0.78% / 0.80%). A ±100% fiat on a textbook decay rate is "the first thing a referee will challenge". Its rate scales with the measured π/K yield; it is μ-linked vs parent-linked only by DTRY hit majority. | keep ±100% with a written justification; or re-size from the data/MC π/K yield or DJ–RG spread (~±0.1% per critic 1) |
| D2 | Three independent fiats in quadrature are 1.6× smaller than one common fiat: integrated h+ 1.00% vs 1.62%, h− 0.97% vs 1.52%. | independent quadrature (physically distinct origins) or linear/common (more conservative) |
| D3 | Quoting rule for h+/h− and A_h. **The critics disagree.** | critic 2: ρ0 for all sources → ratio ±1.40% integrated, A_h ±0.0070, since Σ⁺/Σ⁻ also differ; critic 1: mixed (int ρ0; hyp, strmu ρ1) → ratio ±0.51%, A_h ~±0.0026, with ρ0 and ρ1 as brackets |
| D4 | hyp shares the Ξ→Λπ chain and JETSET strangeness with the V0 piece of env100. Correlating them linearly would raise the total median 7.32→7.44% / 7.63→7.80% (an upper bound). | independent (record) or partially correlated |

**Fixes owed by me (no decision needed):**
- Construction text: fc5 kpar not varied (~4.9k/charge); strother is 11–15% of the int source, not "tiny"; the hyperon mother list includes resonances/Σ0.
- C5 wording: the primary-loss side is "not covered (pre-existing gap: no NucliaWeight uncertainty column)", not "belongs to tracking_eff".
- Document the fc0 overlap with `bgsub_fakefrac_rel` (~0.01%).
- Per-bin maxima (2.50% / 2.39%) and per-bin ρ1 values are not robust / noise-limited.
- Delete the stale `CELLS` edges from the plotter.
