# START HERE: eΣ-photon release, after install (2026-10-24)

Handoff for a fresh Claude session on the NAF login node, resuming the H1 high-Q² charged-hadron SIDIS release that uses the
electron-Sigma (eΣ) photon. The release is installed. One user decision is open (section 3), plus note cleanup and housekeeping.

This bundle is a pointer file. It copies nothing; every path below is absolute and points at the live file.
Bundle: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-24/`
**Login node rule:** interactive work is one input file and at most 5 minutes wall. Anything else goes through Condor.

## 1. Read in this order
1. This file.
2. Section "ESIGMAPHOTON PROMOTION 2026-10-23 Baseline Override" in
   `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/context-handoff.md` (find it with `grep -n '^## '`, read with `sed -n`).
3. The head of `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md` (never cat the whole file).
4. Install record (the authority on what is installed):
   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/INSTALL_RECORD_esigmaphoton_2026-10-23.json`
5. Promotion record: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/promotions/esigmaphoton_promotion_2026-10-23.md`
6. The decision entries `/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/memory/decisions/2026-10-23-esigma-*.md` (7 files).
7. The earlier bundle `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/START_HERE.md`
   §2, §6, §7 for working rules and traps (topics listed in section 7 below).

**First action:** re-read the ESIGMAPHOTON section of context-handoff.md and the STATE.md head (peer sessions may have changed
things), then ask the user the xF decision in section 3.

## 2. Where things stand
- **The eΣ release is installed and canonical (2026-10-23).** In the eΣ method the virtual photon is built from the scattered
  electron together with the hadronic final state, rather than from the electron alone. Q², z and pT are all computed with this
  photon. The generator-level target is unchanged, so the QED radiative correction C_rad did not need rerunning.
- Install: step s15, Condor 5844143; 106/106 files bitwise equal to the package MANIFEST.
  - Package: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_2026-10-23`
  - Installed release: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphoton_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-23`
  - Uncertainty budget: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_esigmaphoton_2026-10-23`
    (the quoted total is column `total_quoted_esph_rel`).
  - Covariance: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/covariance/z005_covariance_optA_esigmaphoton_2026-10-23`;
    gates g0–g7 and ga10 PASS (step s08 Condor 5843553, s09 Condor 5843566).
- Promotion: step s16 edited 21 canonical files additively, step s17 made 6 text fixes; post-install audit PASS. Pre-edit
  backups: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/promotions/esigmaphoton_2026-10-23_preedit/`
- Scripts:
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/` (phaseA/, phaseC/, phaseD/s08s09/)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_release_2026-10-23/` (s15 install, s16 promote, s17 text fixes)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_cutvar_2026-10-23/` (cut variations)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/plots/esigmaphoton_2026-10-23/` (systematics_names.json,
    budget_grouped/, cut_variations/, plot_covariance_esph.py)
- **Numbers (preliminary; quoting is blocked, section 6):**
  - Median total uncertainty 8.34% (h+) / 8.89% (h−); 95th percentile 29.95% / 35.12%. The previous electron-only release
    had 7.49% / 7.86%.
  - Swapping one source at a time shows the scattered-electron energy (E′) term dominates, then the Σ_h smearing term, then Φ_h.
  - Integrated cross sections: Born 12.02 / 11.69 nb; raw, before C_rad, 10.54 / 10.25 nb (h+ / h−).
  - Integrated Born asymmetry A_h = +0.0141 ± 0.0036.
- **Settled decisions (do not reopen):** no separate ISR/QED uncertainty term (not standard in H1; the applied C_rad covers it);
  Σ_h smearing term only, no LAr calorimeter proxy; jet-energy-scale term symmetrized as (up−down)/2, RapGap only, chosen by a
  significance test (p = 1.3e-6); low-Q² parity documented only; ±0.5% E′ variations out of scope; one bundled promotion; the
  detector-interaction removal term uses κ = ±100%, fully correlated across bins (rank one), sign inferred (all bins negative);
  Φ_h keeps the legacy convention 1.21% / 1.32%.

## 3. Pending user decision: the x_F cut
The hemisphere cut x_F < 0 still uses the electron-only photon, at reconstructed and generator level. Record:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A7_records/A7_xf_cut_photon.md`

A two-way check (Condor 5868500) switched the cut to the eΣ photon:
- Reconstructed level: 0 of 3.88M data tracks change, 0 of 76.7M DJANGOH tracks, 4 of 64.3M RAPGAP tracks (fraction 6e-8;
  these 4 have an undefined (NaN) eΣ x_F).
- Generator level: about 0.25% of tracks flip, and more than 99% of those sit at z_gen < 0.05, below the measured range.
- Results: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_frame_2026-10-22/phaseA/A6_validation/xf_esigma_twoway/`
  (summary.json, README.md). Helper: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/esigma_xf.py`

Ask the user to choose:
1. **Keep the release** and correct the note text to say the x_F cut uses the electron photon (recommended).
2. Rerun the chain with eΣ x_F. The change is expected to be negligible (evidence above); it costs about a day of Condor.

## 4. The analysis note
- Its own git repo: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ`
  (origin github hklest/H1-ChargedSIDIS-AnalysisNote-HighQ, branch development).
- origin/development is at 66331d8. One local commit is not pushed: 48aebe3 (appendix tables with 2-significant-figure
  uncertainties, C_γ column dropped). **Do not push without the user's say-so.**
- Already pushed: 03653ed (merge of the user's c45c3ef), c237ad1 (restyled breakout figures), 86875bf (cut variations
  E−pz > 25, pT > 0.10, |η| < 1.4, DCA < 1, DCA < 3; for |η| < 1.4, 7 of 490 bins fall outside the total uncertainty band),
  66331d8 (figures pruned, 122 → 97 pages; appendix tables regenerated from the installed release).
- Current content: plain-English systematics names in 6 groups (from systematics_names.json), jargon removed, uncertainties
  rounded to 2 significant figures, eΣ defined once.
- Remaining note work:
  - Migration, purity and acceptance figures still use the electron method (captions say so).
  - The C_rad-variants table is electron-frame (no eΣ generator exists).
  - The noZ captions in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/tables/highQ2_z005_esigmaphoton_born_public_2026-10-23/table_config_esigmaphoton_public_2026-10-23.json` still say two factors.
  - Some older figure images have old titles baked into the image.
  - Untranslated jargon remains: "fid-gate overlay variant", "STR-mu rank-one piece", cell IDs S0_x0….
  - The appendix-table generator v1.3.0 (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09/`) is untracked in the BaryonAsym repo.

## 5. Plots
- Release directory: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/releases/esigmaphoton_2026-10-23/`
  with subdirectories budget (including grouped/, six restyled 4-term breakouts), covariance, born_xsec, ratio, asym,
  swap_one, controls, cut_variations; plus README.md and MANIFEST.sha256.
- Tarball (about 62 MB): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/releases/esigmaphoton_2026-10-23.tar.gz`
- Not produced: multiplicity overlays of MC and background. No plot of signed shift versus global bin exists in either frame.

## 6. Still blocked, and housekeeping for the user
- **Quoting is blocked** by three older open items: M1 (the env100 builder), N2 (HFS E−pz), P-CRAD-LUMI (C_rad and luminosity).
- Housekeeping; do none of this unless the user asks:
  - `condor_rm 5840395 5842557 5842558 5842610 5842612 5842992`
  - The 21 canonical docs/skills and the release scripts are untracked in the BaryonAsym git repo; the user decides whether
    to track them (context-handoff.md and CONTEXT.md are themselves untracked).
  - Delete candidates: old fig_cutvar_*_ratio.pdf in the note figures/ directory, and
    `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/esigma_cutvar_2026-10-23/prenominal/test_*`
  - The A2 check of the unfolding-model floor (B1) fails in 1 h+ bin; this is an inherited, known class of failure.
  - 246 old held Condor jobs are untouched; leave them.

## 7. Working rules and traps
Read them in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/START_HERE.md`:
- §2 Login node: one-file ≤5 min rule; zsh quoting (use `bash -c`/`bash -s`); no `pkill -f` on your own pattern; no loose
  top-level files in sidis_highQ2/ or H1ChargedSidis_HighQ2/.
- §6 User rules: plain language; independent critics before big compute; sign-off for new quoted terms and installs;
  scripted, additive, sha-guarded canonical edits with backups and a dry run; peer sessions write here too; backgrounds by
  truth-link only; do not re-raise user-dispositioned items; report outcomes faithfully.
- §7 Pitfalls: frame re-pointing order; per-namespace path constants; release preflight files; numpy-build reproducibility;
  Condor `queue … from` comma splitting; a gate count of 0 is falsy in Python.

## 8. Bundle contents
- `START_HERE.md` (this file) and `MANIFEST.sha256` (its checksum). Nothing else; the bundle only points.
