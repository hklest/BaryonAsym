# eΣ reco-photon frame: production plan (2026-10-22)

**User decision (2026-10-22).** Reconstruct the hadron z and pT with the eΣ photon everywhere: option B of
`docs/elecE_scale_study_2026-10-22.md`. This replaces the photon built from the scattered electron alone. The goal
is to redo the quoted systematic budget and its canonical plots in the new frame.

**What does not change:** generator-level definitions (z_gen, pT_gen with the generator electron and nominal beam),
the cells, all cuts, the electron cone and xF. Nothing canonical is written; installing is a separate, user-gated step.

## 1. Definition
- The reco photon is q = k − k′_eS.
- k′_eS is the effective electron solved jointly from (Q²_eΣ, y_eΣ):
  - A = Q²/(2E_b), B = 2E_b(1−y);
  - E′ = (A+B)/2, cos θ = (A−B)/(A+B);
  - φ is the measured electron azimuth.
  - This is the same solve as the June `ESigmaPhotonRecoFrame` builder.
- Helper: `scripts/unfolding/esigma_frame_2026-10-22/esigma_photon.py` (md5 d4c2ae6b…).
  The environment variable `SIDIS_RECO_PHOTON=electron` returns the measured electron, which is the null mode.
- In the shifted (F1) machinery, the photon uses the shifted Q²_eΣ, y_eΣ and φ.
  - So the E′ and θ legs move the photon through the eΣ variables.
  - The HFS legs move it through the stored y_eΣ of the HFS-shifted trees: JES up gives a +0.16% median shift in y_eΣ;
    Q²_eΣ and E−pz are unchanged.

## 2. Forks
- Made by `make_esph_forks.py`. Each fork is a new `<original>_esph.py` next to its original, which is md5-verified
  and untouched. Every patch anchor matched exactly once; the manifest is `fork_manifest.json`.
- Each fork changes only the reco z/pT call, or the import of another fork.

| fork | change |
|---|---|
| OPT-A response builder | reco call (nominal response + HFS legs via the E−pz shim) |
| track-class v4cat builder, NucliaWeight variant builder | reco call |
| nominal DV builder (cone, 2026-10-11) | reco call |
| s02a event-table builder | reco call + BUILDER path (covariance; later) |
| f1_nominal_consistent | z/pT call in apply_f1 (electron legs, shifted DV) |
| shifted OPT-A response builder, shifted F1 DV builder | import the f1 fork |
| straddle per-cell counter | import the OPT-A fork |
| e/V0 template producer (June lineage) | reco call |
| lowQ2 template builder | pT photon; and in eΣ mode, z from the same photon. The stored `z_SIDIS_es` turns out to be ≈ the electron-photon z (within 0.4%): its "eΣ" energy is solved from Q²_e, which gives back the measured E′. |

## 3. Gates passed (Condor 5838335)
- **Null gates (electron mode vs the official outputs) are all bitwise IDENTICAL:**
  - OPT-A RG shard 0000 (136 objects);
  - zero-shift electron-leg shard (136);
  - HFS jes_up null shard through the shim (136);
  - v4cat (208), NW k1 (208);
  - straddle json (55);
  - template partials e (16) and V0 (40);
  - lowQ2 (13, fork vs original);
  - nominal DV and zero-shift DV on 3 data files.
- **eΣ cross-gates, all IDENTICAL:**
  - OPT-A fork == zero-shift leg fork (the "zero == nominal" construction holds in the new frame);
  - v4cat h_R == OPT-A h_R (the identity the group-C cache asserts);
  - nominal DV fork == zero-shift DV fork.
- **eΣ moves things as expected:** on 3 data files, 11% of the counts change bin; 7,076 of 122,264 response cells change.
- **Template chain:**
  - The e/V0 templates are exactly (Σ partial sumw) × k with var = k²·Σ sumw2:
    k = 351000/9,132,891 (RG) and 351000/10,487,495 (DJ). The constant is verified per bin to 1e-15.
  - The lowQ2 template is the builder yields × 351000/356613 (exact).
  - The eΣ templates are rebuilt by the same sums and constants. A gate on the original partials must reproduce the
    current templates.

## 4. Production (`make_campaign_esph.py`, wrapper `run_prod.sh`; all forks md5-pinned at run time)
Same inputs as the original campaigns; outputs only under `output_v6/studies/esigma_frame_2026-10-22/prod/`.

| item | jobs | memory |
|---|---|---|
| OPT-A nominal response RG 73 + DJ 101 | 174 | 28 GB |
| electron legs E ±1%, θ ±1 mrad, zero (gate) | 870 | 28 GB |
| HFS legs jes/rces/theta up/down (E−pz shim) | 1044 | 28 GB |
| track-class v4cat | 174 | 28 GB |
| NucliaWeight k1 | 174 | 28 GB |
| straddle counter | 174 | 28 GB |
| e/V0 template partials | 1730 | 4 GB |
| lowQ2, nominal DV (660 files), zero-shift DV (gate) | 3 | 4 GB |

Total 4,343 jobs. The ±0.5% electron legs (linearity check only), the HFS null shards (shim gate) and the leg data
vectors (data-only cross-check) are not rerun.

## 5. Downstream (single jobs, the same drivers as `final_rerelease_2026-10-22`, re-pointed)
1. Merges with the original merge scripts.
   - Gate: zero leg == nominal, every array, RG and DJ.
2. Templates and the bgsub vector (L_eff scaling as in the final re-release).
3. OPT-A release (thin driver; sha pins re-pinned to the new responses) and Born.
4. Budget groups A, B and C, the MC-only detector legs, and the assembly into the same 23 terms.
5. The canonical budget plots: the existing plotters, re-pointed.

**Terms carried, not recomputed in the new frame:**
- tracking_eff_m0p5 and phi_h_gauss: relative terms, carried exactly as in the final re-release.
- The ibu_model B1 floor: a carried per-bin floor, as before.
- detint_removal and bgsub_fakefrac: these used the 2026-10-11 response pair and its f_i. They cannot be re-unfolded
  across frames, so their **relative per-bin columns from `budget_final/final_budget.csv` are carried as is**. Both are
  small; this is documented as a limitation.

Covariance (s02a/s02b/s03, s08/s09) waits until the user decides on install.

## 6. Questions for the critics
- Is the photon definition right in the shifted machinery: shifted Q²_eΣ, y_eΣ and φ, with E_b from the beam branch?
- The lowQ2 z: its definition changes from the stored z_SIDIS_es to the eΣ photon z.
- Is the carry list complete and defensible?
- Is the scope complete: does anything else bin reco z/pT?
- Which pins, whitelists and checks downstream will refuse the new responses?

## 7. Critic reviews (2026-10-22) and fixes
- **Implementation critic: DISPATCH-WITH-FIXES.** All fork diffs were verified to be exactly the intended change, and the
  merge whitelists will not refuse the new shards. Fixes applied:
  - production submit files at 6 GB (heavy) / 4 GB (light), `on_exit_hold`, per-job logs;
  - `run_prod.sh` hard-pins `fork_manifest.json` (328051af…) plus every module pin of the original detlegs `run_job.sh`
    and `f1_nominal_consistent.py`; `/usr/bin/python3` for non-LCG jobs; environment logged;
  - TNamed metadata added to `compare_outputs.py`;
  - the lowQ2 fork no longer preselects on the stored z in eΣ mode;
  - smoke round 2 (Condor 5838510), all through the production wrapper:
    - null gate incl. TNamed: 256 objects IDENTICAL;
    - certified benign-mismatch rows on RG 0008/0015/0069: still exactly 1 each in eΣ;
    - NW k1, E′ +1% and HFS rces_up run in eΣ;
    - lowQ2 null mode == original.
- **Physics critic: SOUND-WITH-CONDITIONS, no blocker.**
  - Beam mismatch 27.606 vs 27.6: about 0.02% coherent in data, harmless (smaller than in the old frame).
  - QED radiation is no longer common-mode between reco and the gen target. RAPGAP/HERACLES encodes it, but no budget
    term probes the radiative modelling. A possible new leg (radiative-fraction reweighting) is a user decision.
  - z/pT now depend on Σ_h. A Σ_h resolution / smearing leg (about 174 shards) is a possible new term, a user decision.
    The ±1% JES/RCES legs are complete and correctly sized.
  - `phi_h_gauss_rel` is frame-dependent. It is carried for the plots but must be recomputed natively before quoting
    (fork of the flat-φ reweighting builder).
  - Before quoting:
    - data/MC control plots of pT_h/pT_e, y_eΣ/y_e and E−pz at low y;
    - B1-floor / k = 4 re-check;
    - the m2 record footnote;
    - the xF note: the stored xF is effectively electron-photon. This is numerically inert (0 sign flips in 77k rows).
- Neither critic found anything else in the 23 terms that bins reco z/pT.

## 8. Dispatch (2026-10-22)
- Condor 5838556 (big, 2610) + 5838557 (small, 1733).

## 9. Results (2026-10-22): production complete, every gate PASS
- **Production:** 4,343 of 4,343 jobs exit 0 (Condor 5838556/5838557). The post chain (Condor 5838870) ran merge → plots.
- **Gates:**
  - merges 24/24 PASS; every merged generator side is bitwise identical to the electron-frame nominal;
  - zero leg == new nominal in every array (RG, DJ);
  - dv_zero == dv_nominal; the template rebuild gate (original partials → installed templates ≤1e-13) passes;
  - release re-pointing null gate: final660 reproduced, BBB/f_i byte-identical, IBU ≤6e-16 (numpy floor);
  - the release's own RU-2 windows pass; detlegs nominal vs release ≤1.3e-15; zero legs exactly 0;
  - the straddle aggregate reconciles; mismatch 3/0 unchanged.
- **Integrated cross section:** h+ 10.5392 vs 10.5434 nb (−0.04%), h− 10.2471 vs 10.2518 nb (−0.05%).
  The per-bin median |shift| is 2.4% (p95 27%).
- **Quoted total, median (p95):**

  | budget | h+ | h− |
  |---|---|---|
  | eΣ photon | 8.24% (29.1%) | 8.77% (34.8%) |
  | electron-photon final re-release | 9.73% (37.1%) | 10.50% (48.7%) |
  | installed 2026-10-21 | 7.49% | 7.86% |

- **Terms that change (median %, electron → eΣ):**
  - elec_energy 4.08 → 2.68 / 4.23 → 2.88; elec_theta 0.32 → 0.27;
  - hfs_jes 0.49 → 0.60 / 0.49 → 0.73 (two-sided JES 0.73/0.79); rces 0.06 → 0.14; θ_h 0.07 → 0.10;
  - ibu_stat 3.71 → 3.77 / 4.02 → 4.16; ibu_model 1.34 → 1.50 / 1.43 → 1.59; ibu_reg 0.51 → 0.64 / 0.53 → 0.59.
  - Everything else changes by less than 0.1 point.
- **Products:** NAF `output_v6/studies/esigma_frame_2026-10-22/downstream/` (budget/esph_budget.csv,
  plots/esph_quoted_total_23comp_7col/); local `plots/esigma_frame_budget_2026-10-22/index.html`.
- **Open before quoting (critics):**
  - recompute Φ_h natively;
  - decide on new legs (QED radiative fraction; Σ_h resolution);
  - data/MC control plots of y_eΣ/y_e, pT_h/pT_e and E−pz at low y;
  - B1-floor / k = 4 re-check;
  - m2 and xF notes;
  - covariance (s02/s03/s08/s09) on the eΣ inputs;
  - install with user sign-off.

## 10. Correction after the second critic round (2026-10-22, Condor 5839586)
- The phi_h / tracking carry bug is fixed: these columns are now taken verbatim from the electron-frame final budget.
  bgsub_fakefrac is now native eΣ.
- Corrected quoted total, median: 8.20% (h+) / 8.80% (h−); p95 29.3% / 34.1%.
- See `docs/esigma_frame_critic_review_2026-10-22.md` for the open-item list.
