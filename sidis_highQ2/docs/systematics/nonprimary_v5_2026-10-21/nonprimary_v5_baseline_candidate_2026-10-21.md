# v5: STR-only reco-track rate uncertainty, baseline candidate (DIAGNOSTIC, not installed)

Date: 2026-10-21. It follows the round-1 review (`nonprimary_norm_v3_critic_review_2026-10-21.md`) and
the round-2 user decisions.
- NAF package: `H1ChargedSidis_HighQ2/output_v6/studies/trackclass_variant_2026-10-20/nonprimary_norm_term_v5/`.
- Script: `make_nonprimary_v5.py`, Condor 5804049 plus a rerun after the bracket fix.
- Plots: `sidis_highQ2/plots/systematics/nonprimary_norm_v5_overlay_2026-10-21/`, local copy
  `plots/nonprimary_norm_v5_overlay_2026-10-21/`.

## Definition

The nominal response is unchanged: OPT-A, with the K± keeper as signal and not varied. The
fake-column content per provenance class comes from the v4cat export: builder md5 `c9af59df…`, with
the response bit-identical to the official one on all 174 shards.

| source | content | treatment |
|---|---|---|
| interaction-born (int) | STR-only π/K/p with a non-hyperon, non-K± direct mother, plus STR non-π/K/p non-μ (11–15% of int) | κ = ±100% **conservative fiat**. Independent component. |
| hyperon feed-down (hyp) | STR-only π/K/p with a hyperon direct mother (list includes Σ0 and starred states; GEANT-decayed in practice) | **Same nuisance as the V0 part of `bgsub_env100`** (user ruling: common JETSET strangeness, Ξ→Λπ chains). env100 is one e+V0 envelope, so hyp moves coherently with it. The joint shift (data_sub′ − κ(e+V0) and F + κC_hyp, re-unfolded) defines the combined effect. The installed env100 is kept, and hyp is added as the exact increment, giving `bgsub_env100_hyp_rel`, which replaces `bgsub_env100_rel`. Tying to V0 only gives the same increment (0.261 vs 0.264% median). |
| STR μ (strmu) | STR-only μ from π/K decay in flight | **Re-sized** (user ruling). The μ count scales with the π/K yield, so only the fraction f_μ = μ/(B+F) matters. Its uncertainty is the DJ14–RG31 spread of f_μ per (Q²–x cell, z) group. This piece is also contained in `ibu_model_rel` (conservative double count). |

- Per-bin new standalone component: `nonprimary_norm_v5_rel` = √(int² + strmu²).
- Total: `total_quoted_with_nonprimary_v5_rel` = √(total_quoted² − env100² + env100_hyp² + nonprimary_norm_v5²).
- Ratio and A_h use the mixed rule (user): int ρ=0; hyp (inside env100) and strmu ρ=1. Brackets vary
  int and strmu only.
- MC stat is audit-only and never added.

## Results (RAPGAP nominal)

| | h+ | h− |
|---|---:|---:|
| strmu re-sized, integrated | −0.022% | −0.024% |
| strmu per-bin median | 0.012% (MC 0.011%; < 2σ in 224 bins) | 0.012% (< 2σ in 220) |
| f_μ DJ/RG relative spread (median over groups) | 4.4% | 5.8% |
| hyp increment on env100: median / p95 / max | 0.26% / 0.65% / 1.54% | 0.23% / 0.74% / 2.54% |
| env100 integrated (re-unfold) → joint with hyp | 8.60% → 9.12% | 8.87% → 9.40% |
| new standalone (int + strmu): median / p95 / max | 0.087% / 1.01% / 2.48% (b189) | 0.070% / 0.80% / 1.62% (b189) |
| quoted total median | 7.312% → **7.447%** | 7.624% → **7.799%** |
| total increment: median / p95 / max | +0.12 / +0.39 / +0.80 pp | +0.11 / +0.32 / +0.78 pp |

Ratio h+/h− (new-term share), integrated:
- mixed: **±0.51%**;
- brackets: ρ0 ±0.51%, ρ1 ±0.13%.

The per-bin median is 0.17% (p95 1.68%, max 3.8%). A_h, integrated mixed: **±0.0026**.

**Consistency checks**
- The yield-weighted env100 column (8.56% / 8.83%) agrees with the exact re-unfold (8.60% / 8.88%).
  The ±7.7% in STATE.md is the Born-level coherent value.
- Per bin, env100's reco-fraction definition differs from the exact unfolded effect by a factor
  0.69–1.5 (p05–p95), because of migrations. That is a property of the installed env100.
- The hyp increment is negative in 8 h− bins (anticorrelated through IBU), where the combined value
  is below env100 alone.

## Documented scope and caveats

- **κ = ±100% for int is a fiat.** H1 1302.1321 constrains secondary interactions to about 1% after
  the nuclear-interaction correction. NucliaWeight (`H1NuclIACor`: 1 − ani_ineff, "nuclear i.a.
  undescribed by MC") implies data have about 1.5% more interactions per track. That probably
  under-produces interaction secondaries in MC, which this term covers.
- **Not covered by any budget term:** the correlated primary-loss side of a material-interaction
  change. There is no NucliaWeight uncertainty column; this is a pre-existing gap.
- **Overlaps:**
  - strmu with `ibu_model_rel` (DJ vs RG response);
  - fc0 content with `bgsub_fakefrac_rel` (~0.01%);
  - the DJ/RG rate part of `detint_removal_rel` (~0.03%).
- **Not varied:** keeper gen-fail (fc5) is not varied, by decision (~4.9k per charge).
- **Born:** the term is relative and level-invariant (C_rad multiplicative per bin). The Born
  covariance is the relative covariance × σ_Born,i σ_Born,j.
- **Per-bin maxima and per-bin ρ1 values are not robust** (regularization- and MC-noise-limited).
  Quote integrated values and distributions.

## Round 3 (v5): all three critics SOUND-WITH-CONDITIONS, no blockers

- **Implementation:** the critic's own code reproduces all 22 deterministic v5 columns to ≤ 2.6e-15.
  - Covariance `cov_mixed_rel` is PSD, rank 4, and its diagonal equals nn² + env100_hyp².
  - Base and v4 columns are verbatim. Row mapping is verified against `h_binmap_gen_to_reco`.
- **Statistics:** env100 + D is legitimate (D/hyp_standalone 0.94–1.07; a proportional alternative agrees to 0.975–1.09). strmu is unbiased; keep it unsmoothed and not zeroed. The covariance is PSD.
- **Physics:** R1–R6 met; the total swap and sign conventions are verified.

### Conditions and how they were closed (2026-10-21, Condor 5805679 rerun)

| condition | resolution |
|---|---|
| N1 (stat): the ratio clamp disagreed with the covariance in 126 cells | **Fixed.** No clamp. Ratio/A_h columns are now consistent with `cov_mixed_rel`: `ratio_unc_bg_mixed_rel` (full background term) plus a signed net change vs env100-only. Integrated h+/h−: background terms ±0.58% vs env100-only ±0.27% (net +0.51%). Per bin, median 0.37% vs 0.26%; net negative in 87 cells. A_h: ±0.0029 (background terms). |
| H1 (impl): installed env100 not reproduced by re-unfold; guard bins | **Documented**: mixed-definition convention, "exact" wording removed, M1 open. **Guard bins fixed**: no e+V0 data shift where data_sub′ is unsubtracted (hyp is standalone there). hyp increment median 0.262% / 0.231%. |
| H3: stale v4 columns | **Fixed.** Renamed `*_v4_superseded` (18 columns). |
| H4 / F3: plotter docstring, mixed env100_hyp definition | **Fixed / documented.** |
| N2/N4 (stat): env100 sign; env100 once in total_quoted | **Verified.** The installed cross-charge block = lumi⊗lumi + env100⊗env100, all positive (4e-16). The v5 env100_hyp vector is positive in all 490 rows. The 21 active components reproduce total_quoted (6e-16), so env100 enters once. |
| F1/N3: strmu | Kept, unsmoothed, labelled a "generator-spread placeholder / noise-level bound". GEANT decay/kink/link modelling not probed; also inside `ibu_model_rel` (conservative). |
| F2: A_h driven by the int fiat at ρ=0 | Documented. Upgrade path: DJ/RG or GHEISHA charge-composition estimate for int. |
| F4: covariance | **Install condition:** together with the OPT-A covariance re-basing (open), with the env100 rank-1 block replaced by env100_hyp. |
| F5: per-bin maxima, per-bin ρ1 | Quote integrated values and distributions only. |

**Status:** ready to baseline *conditionally*. The remaining dependency is the OPT-A covariance re-basing (pre-existing open item) and the user's go-ahead for the baseline-update procedure.
