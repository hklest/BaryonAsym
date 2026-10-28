# FINAL production plan — detector legs (OPT-A frame), 660-file data, consistent C_rad (2026-10-21)

Purpose: one last compute campaign after which every input of the quoted result is mutually consistent.
User decisions (2026-10-21): recover the 2 unprocessed data files and scale the remaining luminosity;
build all 10 detector legs natively in the OPT-A response frame; make C_rad consistent with the unfolded truth;
critic review BEFORE dispatch. Companion records: `docs/n2_hfs_empz_2026-10-21.md`,
`docs/final_production_audit_2026-10-21.md` (sections A–G).

All code on NAF: `H1ChargedSidis_HighQ2/scripts/unfolding/final_detlegs_optA_2026-10-21/` (detector legs),
`H1ChargedSidis_HighQ2/scripts/radiative/final_crad_consistent_2026-10-21/` (C_rad),
`H1ChargedSidis_HighQ2/scripts/unfolding/hfs_empz_n2_2026-10-21/` (N2 studies, audits).
All outputs: `output_v6/studies/{final_detlegs_optA,final_crad_consistent,hfs_empz_n2}_2026-10-21/`. Nothing canonical is written.

## 1. What runs (heavy compute = items A, B; everything else is single jobs / login node)

| item | jobs | est. CPU | output size |
|---|---|---|---|
| A. detector-leg response shards: 6 HFS legs × 174 + 5 electron legs (±1% E, ±1 mrad θ, **zero**) × 174 | 1914 | ~11 CPU-h (≈20 s/shard measured) | ≈1.2 GB |
| A'. data vectors over the SAME 660 stems: 6 HFS corr + 6 HFS null (gate) + 5 electron + 1 nominal | 18 | ~6 CPU-h | <20 MB |
| A''. merges + gen-side checks (22 merged responses) | 1 | <1 h | ≈0.3 GB |
| B. consistent C_rad worker: DJANGOH14 2953 + RAPGAP31 1777 gen-only files | 4730 | ~100–200 CPU-h (1–3 min/file, 8 GB) | ≈0.3 GB |
| B'. finalize (2 generators) with processed-data beam mixture | 1 | minutes | small |
Total ≈ 6,660 Condor jobs, < 3 GB.

## 2. Definitions (pre-registered)
- **Data file set:** 660 stems = 658 central + `ep0607_d06_0145`, `ep0607_d07_0029` (Stage-2 never run in June: their
  EVLS arrived 06:22 after the 04:33 Stage-2 pass). Recovered with the UNCHANGED June binary (md5 9a585a3d…);
  reproduction gate: re-running it on `d06_0144` is BIT-IDENTICAL to the central tree (8411 rows, 100 branches).
  Recovered trees agree with their HFS twins except calculator kinematics.
- **Luminosity (RESOLVED 2026-10-22):** L_eff = 351,000 nb^-1 × f_proc, f_proc = L_GRL(runs in the HAT production)/L_GRL(all S67)
  = 356,502.93/356,613.82 = **0.9996890 → L_eff = 350,890.85 nb^-1**; processed e+p fraction **f_ep = 0.5439017**
  (`lumi_runset_audit/lumi_eff_final.json`). HAT audit (Condor 5817185): all 660 HAT chunks → EVLS; only 13 GRL runs
  (110.9 nb^-1) are absent from HAT. The 524 GRL runs with zero EVLS events (354.2 nb^-1) are Poisson-empty
  processed runs (Condor 5817201: observed/expected zero-runs 79/79.5, 148/138.8, 213/204.3, 69/72.5 for
  L = 0.1–2 nb^-1; 15/5.1 at 2–5 nb^-1 (≈23 nb^-1, 0.007%); 0 above 5 nb^-1; largest 2.7 nb^-1) → they COUNT.
  So the original 658-file nominal was low by 0.278% (0.246% two files + 0.031% non-HAT runs), not 0.38%.
  MC templates normalized to 351,000 must be rescaled to L_eff (downstream).
- **Response frame:** all legs with the OPT-A builder (HFS: unmodified OPT-A builder + Empz shim; electron:
  OPT-A clone of the shifted builder). Leg vector = (IBU_k4(D_leg − B_R'_guarded; M_leg) − nominal)/nominal on the
  subtracted basis, own response/prior/f_i (budget19 STEP-6 construction), native OPT-A — no re-denomination.
- **HFS legs:** E−pz channel via Empz_corr = Empz + 2E_e·(Yh_leg − Yh_nom) (frozen Empz + calculator ΔΣ_h),
  twin matched on (Run, Event), reco rows only; unmatched events (≈1e-4) get no transfer. 6 legs; budget columns
  keep the installed single-leg convention (jes_down, rces_up, theta_up); covariance keeps the S5f two-sided forms.
- **Electron legs (CHANGE, flagged for review):** Δ = U(shift) − U(**zero**), both from the same shifted builder +
  shifted DV builder, instead of U(shift) − U(nominal). Reason: the shifted machinery recomputes Q², y, x, E−pz
  from HAT-frozen primitives; at zero shift it differs from the nominal by **+0.31% in data** (per-bin median 0.36%,
  p95 1.0–1.2%) but only **+0.03% in MC** (h_R) — a data/MC-asymmetric artifact currently inside every installed
  electron term. The old (shift − nominal) form will be reported alongside.
- **C_rad:** worker fork = applied worker minus the η/J/ψ (VetoAncestorPDG_v3 ∈ {221,443}) hadron veto, run with
  `--no-rad-q2gki-gt100`; finalizer forks take f_ep from the processed data (≈0.5439) instead of 0.5141.

## 3. Gates already passed (smoke matrix)
| gate | result |
|---|---|
| Empz shim null vs 2026-10-14 HFS campaign (1 file + 6 leg×gen shard-0000 + 3 minus-leg×2 gens) | 68/68 hists bit-identical, all |
| HFS DV null (658) == old 660-file DV − 2 extra per-file vectors | bitwise, 3 legs |
| Leg-vector machinery replica vs cached legs_rg / installed OPT-A columns / installed cov deltas | 1.1e-10 rel (cancellation floor) / 8.1e-16 abs |
| OPT-A builder clone for electron legs: structural diff (clone vs OPT-A) == (shifted vs cone) | only comments/docstrings differ |
| clone `--shift zero` vs cone shifted `--shift zero` | differences only in body/in-cone/miss bookkeeping (the OPT-A operand) |
| production wrapper `zero` shard == pre-smoke zero shard | 68/68 identical |
| HFS shard (jes_up rebuild root) via production wrapper: gen side vs OPT-A nominal shard | h_prior_gen, h_mc_count bit-identical |
| electron OPT-A merge: positive (zero) / negative (cone-frame shard) / HFS via OPT-A merge | PASS / FAIL as required / PASS |
| twin fallback for the 2 recovered files | twins resolved to recovered trees; 1 unmatched event of 2135 |
| C_rad: applied worker reproduces campaign partials | bitwise, 4/4 (DJ, RG × rad, nonrad) |
| C_rad fork is a per-bin superset | 4/4; +3.35–3.51% hadron weight |
| C_rad finalizer forks with f_ep = 0.5140930272 reproduce installed tables | DJ ≤7.7e-16, RG ≤6.3e-16 (1−f_ep is one ulp from the old literal 0.4859069728 — implementation critic m2) |
| frame test (jes_down, native OPT-A vs re-denominated) | medians agree; per-bin max 0.5/1.1 pp |

## 4. Downstream after the campaign (single jobs; separate review before install)
New nominal DV (660) → templates × L_eff/351,000 → Set R' vectors / data_sub' → OPT-A raw release (L_eff) →
new C_rad → Born release → budget regeneration (all re-unfold-based components incl. new native detector legs,
env100, nonprimary v5, NW, bgsub, detint, ibu_model/reg, stat & MC-stat toys) → covariance edition (g0–g7 + ga10)
→ M1 builder install → plotters → note. Relative per-file-campaign terms computed on 658 files (tracking ensemble,
phi_h) are carried as relative terms unless the review asks for a rerun.

## 5. Questions for the critics
1. Is U(shift) − U(zero) the right electron-leg definition? Any reason to keep (shift − nominal)?
2. Empz transfer form (additive calculator ΔΣ_h onto frozen Empz) and the unmatched-event treatment.
3. L_eff definition (EVLS-run-based vs HAT-run-based) and the per-beam fraction for C_rad.
4. Removing the RAD-only Q2Gki>100 cut: is NONRAD (Q2GT100) complete for the Q²_es_gen ≥ 150 fiducial?
5. Anything that would force a second production (missing legs, wrong file sets, schema traps, merge whitelists).

## 6. Implementation-critic fixes applied (review: `docs/critic_impl_final_production_2026-10-21.md`, verdict DISPATCH-WITH-FIXES)
- B1: production submit files written and reviewable: `final_detlegs_optA_2026-10-21/condor/prod_{shards,dvs,merge}.sub`,
  `final_crad_consistent_2026-10-21/condor/prod_{djangoh14,rapgap31}.sub`, `finalize_prod.sub`; all job subs
  `on_exit_hold = (ExitCode =!= 0)` (held jobs inspected before release); 4 GB shards/DVs, 8 GB C_rad.
- M1: `check_shards.py` (every job log 'processed N/N files OK' + DONE; OPT-A watchdog mismatch 0; shim unmatched < 1e-3;
  null shards zero transfer) and `check_dvs.py` (660/660 opened, 0 failed; HFS null DV == old 660-file DV bitwise;
  nominal660 − recovered2 == installed 658 bitwise; unmatched < 1e-3).
- M2: `merge_and_check.py` asserts 73/101 shards per leg, the per-shard log gate, and h_prior_gen_*/h_mc_count_* bit-identity
  vs the OPT-A nominal for all 34 merged responses (NWeff excluded: reco-dependent by construction).
- M3: `run_finalize_prod.sh` asserts the exact partial counts (DJ 642/731, 260/260, 530/530; RG 582/691, 248/256) and a
  PASS verdict in every worker log before finalizing; RG uses the emfix exposures; CRAD_F_EP = 0.5439017406824048.
- m3: shim refuses arrays(library≠'np') and uproot.iterate/concatenate/lazy; re-gated null == 2026-10-14 smoke (68/68).
- m4: `run_job.sh` md5-pins every imported script (OPT-A builder, electron clone, both DV builders, shift core,
  all binning_v3 copies, shim, runner).
- m8 (adopted): +1044 OPT-A-frame null-mode HFS shards (`shards_null/`), so the E−pz-only effect is isolated natively.
  Totals now 2958 shard jobs + 18 DV jobs + 1 merge + 4730 C_rad + 1 finalize.

## 7. Physics-critic F1 + follow-ups (reviews `docs/critic_phys_final_production_2026-10-21.md`, `docs/critic_phys_F1_2026-10-22.md`, `docs/critic_impl_F1_2026-10-22.md`)
- **F1 implemented and exact.** `f1_nominal_consistent.py` (md5 c5018455…): every shifted selection/binning variable =
  stored nominal value + [g(shift_electron(dE,dθ)) − g(shift_electron(0,0))], g = the NOMINAL definition (eΣ scalars with
  H1Calc fixed beams 27.6/920 + calculator Σh = 2·27.6·Yh; E−pz + ΔΣ_e; z/pT via the nominal `_compute_pTh_cm_and_z`
  on the effective shifted electron; xF = stored + Δ[Stage-2 eΣ-photon xF]); NaN-safe deltas; nominal beam fallbacks.
  Root cause of the old offset: run beam energies + HAT-frozen Σh in the legacy core, the core's own xF photon, and
  float32/float64 edge flips. **Gate: zero-shift == OPT-A nominal BIT-IDENTICAL** on 4 shards (RG/DJ, incl. ep0304
  catalogs 7097/7093) and the full 660-file DV (3,874,767 = 3,874,767). Production gates (merge_and_check, check_dvs)
  now hard-require zero == nominal for the full merged responses and DV. Both critics: F1 sound at nonzero shift.
- **D2 envelope re-cuts:** Stage-1 electron envelope (E' ≥ 11 GeV, θ < 2.7 rad, 30 < E−pz < 75 GeV, y_e < 0.94)
  re-applied to the shifted electron as "moved-out" vetoes (exactly inactive at zero). One-sided: events that would
  move IN are absent from the trees; bound = the mirror leg's moved-out loss (documented).
- **Shim** uses H1Calc's fixed 27.6 GeV in ΔΣh.
- **Added ±0.5% electron-energy legs** (presets ±1% with scale override 0.005; merge pins added) so the E'-scale
  magnitude decision needs no new production; also a linearity test.
- **D4 / F1-m2 (record):** in MC the stored xF_SIDIS_es uses the true matched-particle mass (data: pion); effect on the
  xF<0 cut 5e-5; the F1 MC xF delta for K/p is off only at second order.
- **D1 (BLOCKER FOR QUOTING, not for dispatch) — leg definition:** the installed legs shift data AND MC
  (IBU(D_leg;M_leg) − IBU(D;M)); a calibration uncertainty is the data-vs-MC scale, so the standard H1 definition is
  MC-only: IBU(D;M_leg) − IBU(D;M). Decomposition of the installed legs: per-bin median elecE MC-only 3.6–4.0% vs
  installed 1.3–1.7%; data-only legs ≈ −MC-only (corr −0.7…−0.8). The production provides both M_leg and D_leg, so
  either definition can be formed. **User decision required** (with the E'-scale magnitude: ±1% vs ±0.5%).
- **DJ C_rad non-closure (record):** RG gen-only σ_rad closes to the RG response truth (0.998, per-bin p05–p95
  0.990–1.008); DJ gen-only σ_rad does NOT close to the DJ detector-MC truth (+1.65% integrated, z-shaped). The
  definitions are now identical; the DJ difference is a sample difference. Relevant to which generator's C_rad is applied.

## 8. Dispatch record (2026-10-22)
- C_rad: Condor 5817208 (DJ, 2953) + 5817209 (RG, 1777): 4730/4730 exit 0; finalize 5817213 PASS (all count gates).
  New tables `output_v6/studies/final_crad_consistent_2026-10-21/tables/{djangoh14,rapgap31}/`: integrated C_rad
  DJ h+ 1.141517→1.140718 (−0.070%), h− −0.069%; RG +0.025%/+0.026%; per-bin |ΔC/C| median 0.04–0.07%, max 0.61%.
- Detector legs: Condor 5817218 (3306 shards: 6 HFS corr + 6 HFS null + 7 electron legs, × 174) + 5817219 (20 DVs).

## 9. PRODUCTION COMPLETE — all gates PASS (2026-10-22)
- Detector legs: Condor 5817218 (3306) + 5817219 (20 DVs), all exit 0; electron legs then re-run (5817227, 1218, exit 0)
  after the full-sample zero gate caught ONE event (DJ 7092 em0405_0089, run 425420 / evt 1583234640, 12 tracks) with
  stored Yes == 0.7f exactly: the nominal response builder cuts on raw float32 branches (0.7f < 0.7 → False) while the
  shifted builder compared float64. Fix: the shifted response builder evaluates the Q2/y/E−pz event cuts in float32
  exactly like the nominal (md5 51e7c013…). Superseded electron shards kept in `superseded_pre_f32evtcut_2026-10-22/`.
- Gates (Condor 5817230, `postgates`): check_shards PASS 3306/3306 (log 'processed N/N', DONE, OPT-A mismatch counts ==
  the nominal's own 3 certified-benign flips on rapgap31_0008/0015/0069, shim unmatched < 1e-3, null shards zero
  transfer); check_dvs PASS (20 DVs 660/660/0; HFS null == old 660-file DVs bitwise ×6; nominal660 − recovered2 ==
  installed 658 bitwise; **dv_zero_660 == dv_nominal_660 bitwise**; unmatched fractions ≤ 1.23e-3 (hfs_jes_up; events that
  pass Stage-1 only with JES up; bound ≈ 3e-5 of the yield)); merge_and_check PASS 38/38 (exact 73/101 shards, gen-side
  bit-identity vs OPT-A nominal, **zero merged response == OPT-A nominal in every array, RG and DJ**).
- Products: `output_v6/studies/final_detlegs_optA_2026-10-21/merged/{shards,shards_null}/{rapgap31,djangoh14}/<leg>/`,
  `data_vector/dv_*_660.json`; C_rad `output_v6/studies/final_crad_consistent_2026-10-21/tables/`; recovered nominal data
  trees `output_v6/studies/hfs_empz_n2_2026-10-21/recovered_central_data/`; L_eff = 350,890.85 nb^-1, f_ep = 0.5439017.
- Nothing canonical written. Next: downstream re-release (leg vectors → budget → covariance → Born → plots → note),
  gated on the user decisions in section 7 (D1 leg definition, E'-scale magnitude, applied C_rad generator).

## 10. USER DECISIONS (2026-10-22) for the downstream re-release
1. Detector legs (electron E/θ, HFS JES/RCES/θ): **MC-only** definition, Δ = IBU(D_nom; M_leg) − IBU(D_nom; M_nom)
   (data-only IBU(D_leg; M_nom) kept as a cross-check). Applies to budget and covariance deltas.
2. Electron energy scale: **±1%** (±0.5% legs retained as linearity check only).
3. Applied C_rad: **keep DJANGOH14** (consistent-definition tables); RAPGAP31 = formal alternative; DOCUMENT that the DJ
   gen-only σ_rad does not close to the DJ detector-MC truth (+1.65% integrated, z-shaped; RG closes to 0.2%).
4. env100: **switch to the unfolded form** (IBU k=4 re-unfold of data_sub' ∓ (e+V0) on the OPT-A chain), replacing the
   reco-ratio; the v5 hyperon increment then combines on one (unfolded) definition — the "mixed-definition" caveat ends.

## 11. Downstream re-release status (2026-10-22) — study products only, nothing canonical written
NAF scripts `scripts/unfolding/final_rerelease_2026-10-22/` (local `scripts/final_rerelease/`); outputs
`output_v6/studies/final_rerelease_2026-10-22/`.
- **Vector / release / Born** (thin drivers, null gates reproduce the installed products with 0 differing cells):
  `vectors/data_vector_bgsub_setrprime_final660.json` (+0.273% data_sub'), `release/final660/` (integrated +0.305% h+ /
  +0.313% h−), `born/final660/` (Born +0.232% / +0.240%).
- **Detector legs, MC-only** (`detlegs/detlegs_mconly.json`): nominal vs release 1.8e-15; zero leg exactly 0.
- **Budget groups** (each old-input null gate reproduces the installed columns: A ≤3.8e-16 in LCG_107a; B bitwise /
  ≤4.8e-13 (tracking toys); C bitwise): `budget_groupA/`, `budget_groupB/`, `budget_groupC/`.
- **Final budget** `budget_final/final_budget.csv` + `final_budget_summary.json` (`assemble_final_budget.py`): 23 terms,
  same composition as installed `total_quoted_with_nonprimary_v5_nw_rel` (installed total re-derived from its 23 columns
  to 1e-15). Median total 7.485 → **9.732%** (h+), 7.860 → **10.502%** (h−). Swap-one attribution: elec_energy
  +2.25/+2.65 pp; everything else net −0.2 pp (env100_hyp unfolded −0.10/−0.08 at the median but p95 7.75→8.64 /
  8.08→11.21%; HFS legs drop: the old rces/θ were dominated by the 658-vs-660 file artifact).
  - elec_energy MC-only ±1%: median 4.08/4.23%; high-x cells (x1) 7–11%, low-x 1–2.5%; RG = DJ; data-only shift same size
    (4.9%); ±0.5% = half (linear) → total median 8.15/8.14%; symmetric (U+−U−)/2 form → 9.69/9.78%.
- **Covariance inputs**: s03 C_MCstat final (gate bitwise == installed; final PASS, abs +0.25% = data growth);
  s02a 660-file joint event table (merge dv980 == dv_nominal_660 bitwise; 658 old tables reproduced exactly);
  s02b joint toys gate bitwise == installed (clip stats 2890 identical), final DONE (clip 2910/965; sidecar agreement
  pattern as before). Next: s08/s09 package + v5/NW editions on the final inputs, after the budget checkpoint.
