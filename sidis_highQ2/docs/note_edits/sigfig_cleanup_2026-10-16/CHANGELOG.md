# Significant-figures cleanup pass — analysis note sources

**Date:** 2026-10-16
**Note root:** `sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/` (git branch `development`)
**Scope:** numeric-only pass over `main.tex` + `sections/*.tex`; all measured/derived
physics values capped at 4 significant figures. No prose, structure, labels, or
non-numeric content changed. No commit/push (user edits Overleaf concurrently).
**Companion CSV (every value):** `sigfig_changes.csv` (file, pre-edit line, old, new, class).

## Summary

- **41 values changed** across **6 files** (29 changed lines, all verified numeric-only).
- Per file: `results.tex` 22, `appendix_cross_section_tables.tex` 8,
  `radiative_corrections.tex` 6, `summary.tex` 2, `systematics.tex` 2,
  `backgrounds.tex` 1.
- Repeated values kept consistent across files after rounding
  (e.g. Born totals 13.03/12.70 nb in `results.tex` and the appendix framing;
  budget medians 5.803%/6.192% in results, appendix, summary, systematics).

### Values changed (deduplicated)

| Old | New | Occurrences |
|-----|-----|-------------|
| 13.026987 | 13.03 | 3 |
| 12.697511 | 12.70 | 3 |
| 13.004519 | 13.00 | 2 |
| 12.682479 | 12.68 | 2 |
| 11.412233 | 11.41 | 2 |
| 11.123626 | 11.12 | 2 |
| 11.420985 | 11.42 | 1 |
| 11.134861 | 11.13 | 1 |
| 22.673755 | 22.67 | 1 |
| 1.1415173 | 1.142 | 1 |
| 1.1410367 | 1.141 | 1 |
| 1.1418534 | 1.142 | 1 |
| 1.1413756 | 1.141 | 1 |
| 1.1415172920054786 | 1.142 | 1 |
| 1.1410367377289135 | 1.141 | 1 |
| 1.1342370401351234 | 1.134 | 1 |
| 1.1336689310022947 | 1.134 | 1 |
| 1.040960540463156 | 1.041 | 1 |
| 1.0401468080806677 | 1.040 | 1 |
| 5.8032 | 5.803 | 6 |
| 6.1919 | 6.192 | 6 |
| 0.909618 | 0.9096 | 1 |
| 1.0114 | 1.011 | 1 |

## Exemptions applied (classes, left unchanged)

1. **Exact integer counts/dimensions:** 490, 245, 980, 491,246 (response shape),
   3,865,073, 19,024, cluster IDs, years.
2. **Defined constants and bin edges:** luminosity 351000.0 nb^-1 (~10
   occurrences), Q² edges 150 / 300 / 700 / 1500 / 20000 GeV², x edges 0.0021 /
   0.0071 / 0.030, y range 0.1–0.7, beam/dataset definitions.
3. **PDG codes:** 211, 321, 2212; 310, 130, 3122, 3222, 3116, 3312, 3322, 3334.
4. **Paths and file names:** all `\path{...}` contents, figure file names with
   date stamps (20261007, 20261008, 20261016).
5. **Generated-block comment headers** (provenance; not rendered).
6. **References/bibliography** — untouched.

Post-edit scan: all 44 remaining ≥5-sigfig tokens in the sources fall in the
exempt classes above; zero remaining >4-sigfig measured values.

## Values deliberately left at >4 sigfigs

None beyond the exemption classes listed above (all leaves are definitions,
counts, codes, paths, or dates — not measurements).

## Rebuild verification

Toolchain: `make` with
`export PATH=/cvmfs/sft.cern.ch/lcg/external/texlive/2025/bin/x86_64-linux:$PATH`.

| Check | Baseline (pre-edit) | Final (post-edit) |
|-------|--------------------|-------------------|
| Build rc | 0 | 0 |
| Pages | 116 | 116 |
| Undefined references/citations | 0 | 0 |
| Overfull hboxes | 24 | 22 |

Overfull attribution was done against a stash-rebuild of the pre-edit tree:
the pass **cured** two pre-existing overfull boxes (radiative_corrections.tex
L83–91 paragraph with the C_rad factors; results.tex L35–57 integrated
cross-section paragraph) and introduced **no new** ones; the remaining 22 are
pre-existing (table/figure layout in monte_carlo, unfolding, backgrounds,
systematics, results, appendix blocks).

## tables/ check

- `tables/highQ2_z005_cone_s2prime_born_public_2026-10-16/blocks/` (28
  generated blocks, input by `sections/appendix_cross_section_tables.tex`):
  rendered lines contain only bin-edge definitions above 4 sigfigs (1500.0,
  20000.0); data values already follow the generator's 4-sigfig digit rule. No
  edits needed.
- `tables/bincenter_points_2026-10-16/` (untracked bin-center package): left
  untouched; its formatter already caps at ≤4 sigfigs.

## Observations (flagged, not changed)

1. **RAPGAP C_rad pair collapses at 4 sigfigs:** 1.1342370… (h+) and
   1.1336689… (h−) both round to 1.134 (they differ by 0.05%). The following
   sentence keeps the explicit DJANGOH−RAPGAP differences (0.6419% / 0.6499%),
   so no information is lost.
2. **Pooled removable fraction:** the note (`backgrounds.tex`) quotes
   0.909618; `sidis_highQ2/STATE.md` records 0.909920 ± 0.000269 pooled.
   Pre-existing discrepancy between conventions — flagged for the supervisor;
   not reconciled in this numeric-only pass (rounded to 0.9096 as printed).
3. The low-Q² companion ratio 1.0114 → 1.011 remains consistent with the
   quoted σ values (233.3/230.7 = 1.011).

## Reversibility

Every edit is a working-tree modification visible in `git diff` (6 files,
29 lines); the pre-edit line numbers are recorded in `sigfig_changes.csv`.
Nothing was committed or pushed.
