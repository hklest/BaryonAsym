---
name: h1-sidis-final-production-2026-10-21
description: "Final-production findings/decisions 2026-10-21 — 0.38% lumi deficit, 2 recovered data files, electron zero-shift artifact, C_rad truth mismatches, OPT-A native legs"
metadata:
  node_type: memory
  type: project
  originSessionId: 07349e28-697a-4d11-8dc0-cbfe17f90b05
  modified: 2026-10-22T04:52:05.813Z
---

User-approved 2026-10-21 "make everything consistent" final production (critic review before dispatch). Records: local `docs/final_production_plan_2026-10-21.md`, `docs/final_production_audit_2026-10-21.md`, `docs/n2_hfs_empz_2026-10-21.md`; NAF code `H1ChargedSidis_HighQ2/scripts/unfolding/final_detlegs_optA_2026-10-21/` and `scripts/radiative/final_crad_consistent_2026-10-21/`.

- Nominal data (658 files) was 0.278% low vs the full 351 pb^-1 (FU1 run-set check never done before): 0.246% from two files (ep0607_d06_0145, d07_0029) that never got central Stage-2 (late EVLS; recovered bit-reproducibly) + 0.031% from 13 GRL runs absent from HAT. 524 zero-event GRL runs are Poisson-empty processed runs and count. User chose recover + scale: L_eff = 350,890.85 nb^-1, f_ep = 0.5439017.
- HFS legs used 660 files vs nominal 658 → spurious +0.25% in every HFS leg (dominant part of hfs_rces/theta). N2 fix = Empz shim (Empz + 2E_e ΔYh).
- Electron shifted builder at zero shift ≠ nominal: +0.31% data vs +0.03% MC → installed electron terms contain this artifact; plan defines electron legs as U(shift) − U(zero).
- C_rad vs unfolded truth: kinematics identical; mismatches = η/J/ψ-descendant veto (−0.011%), RAD-only Q2Gki>100 cut (~−0.07%), beam mix 0.514 vs 0.544. User approved regenerating C_rad without them.
- All 10 detector legs to be built natively in OPT-A frame (frame test: per-bin up to 1.1 pp vs re-denomination).

**Why:** user wants the analysis "totally square" after this production so it can be finished for good.
**How to apply:** check critic verdicts and campaign status before quoting; downstream re-release (templates × L_eff, budget, covariance, Born) still needed after the campaign. Related: [[h1-sidis-nucliaweight-primary-loss]], [[feedback-smoke-before-production]].

**Status 2026-10-22:** production COMPLETE, all gates PASS (plan §9). Electron legs rebuilt with F1 (zero-shift == nominal bitwise; envelope vetoes; ±0.5% legs added). C_rad regenerated (DJ −0.07%, RG +0.025% integrated). Pending USER decisions before the downstream re-release: (1) leg definition MC-only IBU(D;M_leg) vs installed both-shifted (critic: installed understates elecE ~2.5–3×); (2) E' scale ±1% vs ±0.5%; (3) applied C_rad generator (DJ gen-only does not close to DJ detector-MC truth, +1.65% z-shaped; RG closes); (4) env100 reco-ratio vs re-unfold (M1).
**User decisions 2026-10-22:** (1) MC-only detector legs; (2) E' scale ±1%; (3) keep DJANGOH C_rad applied (document non-closure); (4) env100 → unfolded form. Recorded in plan §10.
**Status 2026-10-22 (later):** downstream budget assembled (plan §11; NAF `output_v6/studies/final_rerelease_2026-10-22/budget_final/`). Median quoted total 7.48→9.73% (h+) / 7.86→10.50% (h−), driven by elec_energy MC-only ±1% (median 4.1%, 7–11% in high-x cells; physical, RG=DJ, data-only same size). ±0.5% would give 8.15/8.14%. Group A bit-reproduction needs LCG_107a numpy. Covariance inputs (s03, s02a 660, s02b) done; s08/s09 + editions pending user budget checkpoint.
**elecE study 2026-10-22** (`docs/elecE_scale_study_2026-10-22.md`, plots `plots/elecE_scale_study_2026-10-22/`): user doubted the 4% elecE median. Cause: reco z/pT use the photon from the electron alone (y_e), δz/z = (1−y)/y·δE' → 11% at y≈0.09; high-x cells sit at y≈0.18–0.20. eΣ photon would give 1.8%. Options (a) keep, (b) scale magnitude, (c) eΣ-photon z/pT (full re-production) — awaiting user.

Photon-definition study 2026-10-22 (docs/elecE_scale_study_2026-10-22.md, page plots/photon_options_2026-10-22/index.html):
eΣ photon for z/pT (B), user hybrid (C), y-switch (D), kinematic fit (K0: E′+Σ_h+pT_h, beam fixed) all cut the high-x E′ term ~8.5%→3–4%.
K0/Ks best resolution & purity, comb ≈ B. Not decided; any adoption = full response/leg re-production.

eΣ FRAME (user decision 2026-10-22: eΣ photon for reco z/pT everywhere): full re-production done, all gates PASS
(docs/esigma_frame_production_plan_2026-10-22.md; NAF scripts/unfolding/esigma_frame_2026-10-22/, outputs output_v6/studies/esigma_frame_2026-10-22/).
Forks *_esph.py next to originals (SIDIS_RECO_PHOTON=electron reproduces originals bitwise). Total median 8.24/8.77% (vs 9.73/10.50 electron frame).
STUDY only, not installed. Open: Φ_h native recompute, QED-radiative + Σ_h-resolution legs (user decision), covariance, install sign-off.
