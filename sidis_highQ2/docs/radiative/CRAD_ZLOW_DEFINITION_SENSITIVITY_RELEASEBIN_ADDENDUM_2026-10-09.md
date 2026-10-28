# z<0.05 definition sensitivity: releasebin addendum — 2026-10-09

**Status: completed matched releasebin xF-definition refresh.  Historical values are retained only for comparison; the refreshed values are current-basis evidence and do not alter an applied table.**

## Source and exact historical definition

The carried `+/-6.8%` (h+) / `+/-3.9%` (h-) statement comes from
`CRAD_UNC_GENERATOR_SPREAD_2026-10-07.md`, Step 2, lines 90--109.  It was the **definition-only comparison** between:

- A: blind, hemisphere-unrestricted (`xf_cut: False`) DJANGOH14 C_rad table; and
- B: xFcut (`xFGen<0` on RAD and NONRAD) DJANGOH14 table.

It is therefore a hemisphere-definition sensitivity, not an EMPZ sensitivity and not the matched DJANGOH14--RAPGAP31 generator spread.  The source reports zero median `|rel|` in every `z>=0.05` row and maxima `6.825%` h+ and `3.870%` h- in `z<0.05` (with 1.551%/1.076% in 0.05--0.1 and <=0.36% elsewhere).  The quoted rounded blocker values derive directly from these maxima.

## Releasebin refresh (completed 2026-10-09)

A matched xF-free production was run with the releasebin worker, changing **only** the hemisphere mask from `xFGen<0` to no xF cut.  It retained release-safe `(Q2esGen,x_es_gen)` assignment, `GenEmpz>45`, DIS/RAD-domain gates, weights, per-leg luminosities, and beam mixture.  The xF-on comparator is the accepted releasebin table and was not recomputed.

Smoke jobs passed at 8 GB (DJANGOH14 cluster `5435947`, RAPGAP31 `5435948`; two leg representatives each).  Throttled full production passed with no held jobs: DJANGOH14 `5436036`, `5436126`--`5436133` (2,953/2,953 exit 0); RAPGAP31 `5436037`, `5436134`--`5436136`, `5436138` (1,777/1,777 exit 0).  Reducer retry `5437036` passed.

The table gives `|C_xF-free/C_xF-on - 1|`.  The requested `z<0.05` maximum and the median across all `z>=0.05` rows are percentages; `int` is the signed integrated xF-free shift relative to xF-on.

| generator | h+ z<0.05 max | h+ z>=0.05 median | h+ int | h− z<0.05 max | h− z>=0.05 median | h− int |
|---|---:|---:|---:|---:|---:|---:|
| DJANGOH14 | **9.925%** | 0.000% | -0.373% | **8.023%** | 0.000% | -0.413% |
| RAPGAP31 | **6.461%** | 0.000% | -0.344% | **5.200%** | 0.000% | -0.381% |
| old xFcut DJANGOH14 basis | 6.825% | 0.000% | +0.374% (xF-on vs free) | 3.870% | 0.000% | +0.414% (xF-on vs free) |

The old and new integrated shifts have opposite sign only because this table reports **xF-free minus xF-on**, whereas the historical provenance quoted **xF-on minus blind/free**.  Their magnitudes are comparable.  Differentially, the current DJANGOH14 releasebin maxima are larger than the old values; RAPGAP31 provides an independently matched definition result with maxima of 6.461%/5.200%.

## Current conclusion

- **Confirmed source meaning:** the blocker is an xF-hemisphere definition sensitivity, concentrated in `z<0.05`.
- **Refreshed current basis:** use the table above, not the old xFcut-only `6.825%/3.870%`, when describing the releasebin definition sensitivity.
- **No table/budget action:** this study does not change an applied table or systematic budget; it quantifies the carried blocker.

Artifacts: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/crad_xf_definition_releasebin_2026-10-09/xf_definition_sensitivity_results.json` and its two 490-row comparison CSVs.
