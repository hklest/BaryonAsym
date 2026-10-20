# Superseded figures, 2026-10-08

Historical artifacts, preserved unchanged. Not deleted, never modified in place.

## What was replaced

| Superseded file (this directory) | What it was | Replaced by |
|---|---|---|
| `fig_results_xsec_density_release351_expanded_7panel_hplus_z005_20260924.pdf` | h+ nominal IBU k=4 finalcandidate release cross-section density, raw (no C_rad, not Born-level), vertical error bars = expanded candidate systematic budget (10-component nominal total + `background_rel`, `labpt_rel`, `dca_scan_rel`, `compound_track_quality_rel`, `empz25_rel`), plain finalcandidate prior, L_data = 351000.0 nb^-1, produced 2026-09-24 from the pre-Born v6/z005 release. | `../fig_results_xsec_density_born_crad13_coherentprior_7panel_hplus_z005_20261008.pdf` |
| `fig_results_xsec_density_release351_expanded_7panel_hminus_z005_20260924.pdf` | h- twin of the above, same release and budget. | `../fig_results_xsec_density_born_crad13_coherentprior_7panel_hminus_z005_20261008.pdf` |

## Provenance of the superseded plots

Pre-Born v6/z005 finalcandidate release (plain prior, no C_rad), with the
expanded candidate systematic budget discussed in the note's systematics
section. Raw cross section, not Born-level.

## Provenance of the replacements

Born-level provisional release (accepted 2026-10-08, style parity 45/45):
`H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14xfcut_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-07/`.
Applied C_rad table: DJANGOH14 xFGen<0 xFcut, EMPZ>45; integrated h+
1.1418534312056339, h- 1.1413755529189475. Coherent `body+miss` prior, IBU
k=4, L_data = 351000.0 nb^-1. Error bars: full 13-component total from
`..._coherentprior_b1floor_crad13_2026-10-07/` budget, including
`c_rad_gen_rel` and `c_rad_stat_rel`; median totals h+ 5.713%, h- 6.150%.
Source: `sidis_highQ2/plots/systematics/born_crad13_2026-10-08/DJANGOH14_xFcut/cross_section_density_nominal_no_bgsub_7panel_{pos,neg}.pdf`.

## Note

The LaTeX labels `fig:results:release351-expanded-xsec-density-hplus` and
`fig:results:release351-expanded-xsec-density-hminus` were kept unchanged to
avoid breaking cross-references; the label strings are now legacy names whose
text no longer matches the plotted content.
