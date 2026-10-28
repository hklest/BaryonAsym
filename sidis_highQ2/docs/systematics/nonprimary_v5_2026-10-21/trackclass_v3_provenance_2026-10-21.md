# v3: provenance-only background definition, impact, uncertainty term and overlay plots (CURRENT, DIAGNOSTIC)

Date: 2026-10-21. This supersedes the v2 definition in `trackclass_v2_selection_spec.md` and
`trackclass_triplecheck_2026-10-21.md`. The v2 triple-check evidence remains valid as validation.

## Ruling (user, 2026-10-21)

The v2 cut "GTR-linked & P/GenP < 0.7 → background" mixed conventions: it used reco kinematics
to define background. Tracks whose truth link is a primary π/K/p — however badly measured — are
migration, and the unfolding must correct for them. 95% of them are the only reconstruction of
their gen particle, so the response already treats them as that particle's smeared measurement.

For the same reason, duplicate removal (which chooses a copy by kinematics) is dropped:
multiplicity is unfolded.

## v3 selection (exact)

- Reco selection, e/V0 exclusion, gen-candidate test and truth definition are as in
  `trackclass_v2_selection_spec.md`, Steps 0–2.
- Among gen candidates:
  - **background** = `IsStr==1 && IsGtr==0` (STR-only, GEANT-created truth link):
    - det.int. (`abs(GenMotherPDG) != 321`, S2' fc3/4/5);
    - K± keeper (`abs(GenMotherPDG) == 321`, fc8, no consumed key).
  - **signal** = every GTR-linked π/K/p (any P/GenP, angle, charge; duplicates; Tier-2).
- Implementation: the builder with `TC_CLASSES=keeper`, i.e. the `keeper` config.
  - RG shards: Condor 5803732. DJ shards: Condor 5803906, 101/101 exit 0.

## Impact of v3 vs the OPT-A nominal (Condor 5803931, `impact_v3/`)

v3 differs from the nominal only by the keeper reroute.

| | h+ | h− |
|---|---:|---:|
| RG31 integrated | −0.0326% ± 0.0004% (MC) ± 0.0000% (data) | −0.0276% ± 0.0004% |
| DJ14 integrated | −0.0340% | −0.0301% |
| per-bin max \|impact\| (RG) | 0.23% | 0.43% |

## Uncertainty term `nonprimary_norm_rel` v3 (Condor 5803930, `nonprimary_norm_term_v3/`)

- κ = 1 (±100%) rate shift of det.int. + keeper fake-column content, re-unfolded.
- Per bin: max(|up|, |down|). Fully correlated.
- Duplicate column ≡ 0.

| | h+ | h− |
|---|---:|---:|
| coherent integrated | ±0.93% (det.int. 0.85%, keeper 0.08%) | ±0.82% (0.75% / 0.07%) |
| DJ14 coherent | ±1.06% | ±0.93% |
| per-bin median / p95 / max | 0.39% / 1.57% / **2.60% ± 0.27%** (b189 S2_x1 z2 pT4, total 11.3%) | 0.31% / 1.43% / **3.42% ± 1.43%** (b244 S3_x0 z6 pT4, total 61%) |
| max term / quoted total (any bin) | 0.39 | 0.25 |
| max term / largest existing component | 0.61 | 0.35 |
| best rank among the 22 components (any bin) | 3rd (7 bins) | 3rd (5 bins) |
| increase of quoted total: p95 / max | +0.12 / +0.48 pp | +0.07 / +0.20 pp |
| MC stat of the term (median) | 0.055% | 0.050% |

## Plots

- Location: `sidis_highQ2/plots/systematics/nonprimary_norm_v3_overlay_2026-10-21/` (local copy
  `plots/nonprimary_norm_v3_overlay_2026-10-21/`).
- Contents:
  - the 7×7 overlay per charge on the 21 active quoted components;
  - y ranges 0–15, 0–50 and 0–100%, plus log y;
  - `nonprimary_norm_v3_perbin_scan.png`, all 490 bins with flags.
- Flags: 20 bins, all only because the term exceeds the small IBU stat in high-yield z0 bins.
  The term is ≤ 2.4% there, against totals of 11–17%.

**Conclusion.** In no bin does the term become large. Its worst bins are about 3% (2.6% on an
11% total; 3.4% on a 61% sparse-bin total). It is never the largest component, and it is at most
39% of the total in any bin.

## Status

DIAGNOSTIC. Nothing installed. κ is a user decision; the term is linear in κ
(`nonprimary_norm_rel_per_unit_kappa`).
