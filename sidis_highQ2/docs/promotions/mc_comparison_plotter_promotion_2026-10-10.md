# Canonical data-vs-MC comparison-plotter promotion — 2026-10-10

User declaration (analysis owner, 2026-10-10): promote the Born-level 4-generator
data-vs-MC comparison plotter to canonical. Executed with the
`sidis-hq2-baseline-update` checklist end-to-end.

## What was promoted

- Canonical data-vs-MC comparison plotter (2-generator and 4-generator):
  `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py`
  - md5 `b7cb52997227febfe387682af30d3c15`; checksum-verified identical copies in:
    the working `MC/scripts/` location, the registry snapshot
    `scripts/canonical_highQ2_sidis_2026-06-30/MC/compare_data_vs_mc_z005_4generator_born.py`,
    and the validated package
    `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_4generator_born_2026-10-08/`
    (promotion source; original untouched).
  - CLI: `--release-dir`, repeated `--prediction-csv`, `--out-dir`, `--lumi-pb 351.0`,
    `--differentiate-4d`; produces panelized absolute 4D differential plots,
    panelized MC/Data ratio plots, and the combined absolute/ratio panel-pair
    figure sharing the pT,h axis. Status `CURRENT_DIAGNOSTIC_OR_PROVISIONAL`.
- NORAD prediction-input builder registered (validated, package-local, NOT a
  registry script):
  `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_4generator_born_2026-10-08/build_nonrad_truth_predictions_2026-10-08.py`
  — aborts on bin-edge misalignment; carries a MANDATORY repoint condition (below).

## Superseded values (old -> new)

- Canonical data-vs-MC comparison plotter:
  `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005.py` ->
  `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py`.
  The old script, its registry snapshot `MC/compare_data_vs_mc_z005.py`, and its
  2026-07-05 output root
  `H1ChargedSidis_HighQ2/MC/plots/data_vs_mc_z005_20260705_10M_rivet_unitcount_xfpos_normfix_clean_differential4d_xcap1p0_correctedinputs_with_panelratio_dataunc`
  are retained untouched, historical/audit only.
- 4-generator curve basis (user decision 2026-10-08, reaffirmed by this
  promotion): `prior_gen`-scaled truth priors (2026-09-25 raw package) and the
  prior-times-C_rad construction (`superseded_priorcrad_*` CSVs) -> the
  generators' DIRECT NORAD sample predictions (per-bin `sigma_nonrad`, gen-only,
  xFGen<0 recomputed xF q-nominal, EMPZ>45, sigma=sum(w1*w2)/L_MC).
- Builder hard-coded inputs: xFcut C_rad tables
  `data/radiative/{djangoh14,rapgap}_crad_final_xfcut_2026-10-05/` (SUPERSEDED
  for application 2026-10-09) -> before ANY regeneration repoint to the
  releasebin tables `data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`
  (DJANGOH14 primary) and `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`
  (RAPGAP31 emfix) and revalidate totals and alignment.

## Conflicts surfaced (promotion proceeded under the user's explicit declaration)

1. SUPERSEDED-BASIS validated instance: the 2026-10-08 package was built on the
   2026-10-07 xFcut Born release and xFcut `sigma_nonrad` legs; both were
   superseded 2026-10-09 by the releasebin rederivation (critic A-F1).
   STATE.md blocker G carries the OPTIONAL regeneration of
   `plots/mc_predictions/data_vs_mc_z005_4generator_born_2026-10-08/` on the
   releasebin Born release `born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
   as an OPEN action. The package remains the note-facing comparison package
   (note commits `411d22e`/`41a6722`/`746ba47`, pushed) until regenerated.
2. RAPGAP31 NORAD legs predate the 2026-10-09 e- exposure repair
   (STATE.md blocker C; `-0.4008%` em bias, `+53,773.7 nb^-1` on
   `L_nonrad(em)`); the emfix table must be used on regeneration.
3. Standing caveats unchanged: RAPGAP31 shares its generator model with the
   central unfolding response (not independent of the unfolded data);
   `P-RAPGAP` open; absolute normalization of the NORAD curves inherits the
   C_rad campaign exposure provenance (`P-CRAD-LUMI` FILE-SET-RECONCILED,
   generator-log-open); PROVISIONAL INTERNAL DIAGNOSTIC.

## Files touched

- Installed: `H1ChargedSidis_HighQ2/MC/scripts/compare_data_vs_mc_z005_4generator_born.py`;
  registry snapshot `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/MC/compare_data_vs_mc_z005_4generator_born.py`
  (both md5-verified `b7cb52997227febfe387682af30d3c15`).
- Registry: both `RUNLIST.md` copies (remote + local snapshot) Phase 7 block
  rewritten. `SEVEN_CATEGORY_SCRIPT_INDEX.md` NOT updated: it contains no
  data-vs-MC plotter entry (no category-map change).
- State: `STATE.md` (head, comparison bullet, "Superseded — do not use" entry);
  `docs/RUN_STATUS.md` (routing paragraph + head);
  `context-handoff.md` (canonical plotter section, package paragraph, push status);
  `docs/LOOKUP.md`; `MC/CONTEXT.md`.
- Skills: `sidis-hq2-plotting` (Authoritative Facts, 2 inventory rows,
  Procedures step 3, 2 traps), `sidis-hq2-mc` (rad-off/NORAD clause, canonical
  trap), `sidis-hq2-docmap` (routing row), `sidis-hq2-baseline-update`
  (this addendum).
- Registration: `docs/skills_build/ownership.tsv` and
  `docs/skills_build/COVERAGE_MANIFEST.md` rows added for the 2026-10-08
  package (owner `sidis-hq2-plotting`) and this promotion record (owner
  `sidis-hq2-baseline-update`).
- No `docs/status/` file updated: none of the five per-topic status files
  (alpha0p2-background, current-release, normalization, radiative-corrections,
  unfolding-tunfold) covers plotting/comparison topics; `RUN_STATUS.md` is the
  aggregate authority for this change.

## Sibling skills verified unaffected

`sidis-hq2-state` (reports STATE.md; no comparison-plotter anchor),
`sidis-hq2-radiative` (no comparison-plot references; NORAD-leg semantics
already owned there), `sidis-hq2-results` (no comparison-plot references),
`sidis-hq2-normalization`, `sidis-hq2-unfolding`, `sidis-hq2-systematics`,
`sidis-hq2-backgrounds`, `sidis-hq2-builder`, `sidis-hq2-operations`,
`sidis-hq2-aziasym`, `sidis-hq2-lessons` (grep pass: no live canonical claims
on the superseded plotter or the prior-times-C_rad basis).

## Provenance

Historical originals untouched: the old canonical plotter and its snapshot, the
2026-09-25 raw package, the 2026-10-08 package (including its
`superseded_priorcrad_*` CSVs), the xFcut tables, and all prior promotion
records. Superseded facts are negated in `STATE.md`, both `RUNLIST.md` copies,
and the affected skills, never silently dropped.
