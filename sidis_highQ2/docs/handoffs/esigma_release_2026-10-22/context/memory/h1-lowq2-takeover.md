---
name: h1-lowq2-takeover
description: "Low-Q2 SIDIS takeover (2026-10-20): where it lives on NAF, true state, parity plan, open decisions D1-D5"
metadata:
  node_type: memory
  type: project
  originSessionId: 17159089-dac2-4382-8a78-a99c3532d15b
  modified: 2026-10-22T14:55:51.703Z
---

User handed me the low-Q2 (5<=Q2<100, SpaCal) charged SIDIS analysis on 2026-10-20 with the goal of parity with high-Q2. Plan: local `docs/lowQ2_takeover_plan_2026-10-20.md`.
- Docs: NAF `BaryonAsym/sidis_lowQ2/` (STATUS.md is stale, from May; the real latest is `docs/BINNING_STATUS_2026-09-30.md`). Code: `H1SIDIS_LowQ2_v5g/`. The builder line is `src/ChargedSidisTreeBuilderLowQ2_v6isr.C` (v5g retired 2026-09-30).
- v6isr trees: `H1SIDIS_LowQ2_v5g/output/v6isr_highq2semantics_2026-09-25/`.
  - They lack `GenScatE*`, `ystar_es_gen`, `TrackLength` and the v6sys shift machinery, which is in `_v6sys.C`.
  - Background MC exists only in the v5g schema.
- The blocker is the binning resolution accumulator port.
- Decisions (2026-10-20): D1 new workspace `H1ChargedSidis_LowQ2/` (created); D2 GenEmpz>45 as in high-Q2; D3 binning deferred to the unfolding stage; D4 port the v3 track rule; D5 2003-04 data and the 4 deferred systematics are IN. Remote `sidis_lowQ2/STATE.md` was created and is the authority.
- F1: the v6isr MC is reco-gated (gtustin event lists), so there is no gen-in/reco-out. A new Stage 1 with envelope-OR is needed and all MC must be reprocessed.

- 2026-10-21: wrote Stage 1 `H1ChargedSidis_LowQ2/src_stage1/TreeBuilderLowQ2_chargedsidis_v6.C` and Stage 2 `src/ChargedSidisTreeBuilderLowQ2_v6.C` (v6isr + `// V6P`). Both compile; Condor smoke 5807774.
- BUG in low-Q2 v6isr Stage 2: there is no per-track reset of matched-truth fields (100% IsMatched=1), so all old low-Q2 MC truth-matching is invalid. Fixed in v6.
- The SpaCal electron energy shift needs an explicit SetShift(eElecEn); the high-Q2 header would give a 3% default.

- 2026-10-21: Wave 0 PASSED on the final binaries. The tooling is `condor_v6/run_chain.sh` and `scripts/builder/make_campaign.py`; results are in `sidis_lowQ2/STATE.md`.
  - Added: FNC and beam spin, `--no-gen-tree` slim legs, `Empz_calc` (the HAT Empz ignores shifts), and `_Exit` (teardown SIGABRT).
  - Blocked: 2003-04 (GRL, SpaCal calibration, dead cells) and DJ14 7080 (dead /acs ODS).
  - Wave A is not yet approved.
- 2026-10-22: Wave A finished, all 7214 jobs exit 0 (33 GB data, 912 GB MC); full validation runs as cluster 5817184. The binning study plan is at `docs/lowQ2_binning_study_plan_2026-10-22.md` (NAF `sidis_lowQ2/docs/plans/`).
  - WS1 (atom accumulator) and WS5 (gate precedent) subagents were dispatched 2026-10-22.
  - Next: WS2 + WS3 after WS1, then WS4, then synthesis for user approval.
- 2026-10-22: the h1 DUST group quota is FULL (39.993/40 TB). NAF writes fail, so WS3/WS4 run locally under `data/lowQ2_binning_ws{2,3,4}_2026-10-22/`.
  - Pending user decision: what to delete (v6isr 230 GB; v5g output ~1 TB).
  - WS2: 9 candidates; the leaders are template (280 gen) and k2p0_q4_ptz (1620 gen).
  - WS3: the MC>=10x data gate is unattainable (MC n_eff ~2x data); pending user decision. Data/MC normalisation F 0.83/0.88/0.93 by year.
  - WS4 is done (local). Synthesis: `docs/lowQ2_binning_synthesis_2026-10-22.md`.
    - Recommendation: k2p0_q4_ptz (1620 gen bins/charge) plus local merges; a second iteration is needed before freezing.
    - Iteration 2 done (NAF `binning_2026-10-22/iteration2/`).
      - Variant B (qT-type gen restriction, η_c=−1.8, removes 1.8% of xsec): 1222 gen bins/charge, all provisional gates pass.
      - Variant A (merge): 3 wedge cells fail acc×eff.
      - Awaiting the user's ruling on the B restriction vs D2.
    - Awaiting user decisions: stability gate (S_mig + acc×eff), MC-stat gate, IBU k by convergence (5) vs 4, empty-corner handling.
  - NAF STATE.md has not yet been updated with WS2-4 (quota).
- 2026-10-22 iter3: user gates acceptance >= 0.10 and purity >= 0.20 (S_mig 0.30 kept provisionally). Local `data/lowQ2_binning_iter3_2026-10-22/`. B: 1374 cells, all pass; A: 1355 cells, 4 wedge failures. Canonical-style 7col plots are in `plots/purity_acceptance_7col/`. Installed on NAF `binning_2026-10-22/iteration3/` + `scripts/binning/iter3/`; STATE.md appended. USER: k=5 (2026-10-22). eta_c scan: -1.8 = loosest with 0 failures. Layout plots in `plots/binning_layout/`. Awaiting the B-vs-D2 ruling, S_mig and the freeze.
- USER DIRECTION 2026-10-22: select only kinematics where the struck-quark direction is measurable, i.e. an event-level QPM cut eta_q = ln(Q sqrt(1-y)/(2 y E_e)) > eta_c, which is a y_max(Q2) cut (eta_c=-1.6 gives y<0.18 at Q2=5 and y<0.58 at Q2=100; keeps 54% of gen hadrons with z>=0.05 and 68% of data). This supersedes the variant-B q_T wedge.
  - phi_h coverage is also required: an unmodelled cos phi / cos 2phi in the MC biases the acceptance correction by ~dA_n * m_n, where m_n are the acceptance Fourier moments.
  - Atoms v2 DONE (NAF `atoms_v2/`, local `data/lowQ2_binning_atoms_v2_2026-10-22/`). Iteration 4 DONE (`data/lowQ2_binning_iter4_2026-10-22/`, NAF `iteration4/`, STATE appended).
    - eta_c=-1.6: 991 cells per charge; all gates pass; k=6 (k=5 at 0.215 sigma).
    - phi_h: a blind ~60 deg slice at phi_h~0 where pT_h~zQ sqrt(1-y) (hadron along the beam pipe) in ~30% of cells; tightening eta_c does not fix it. Bias envelope q95 4.7%.
    - Awaiting the user on eta_c and phi handling (reweight / phi-differential unfolding / gate).
  - Builder issue: some events have scatE*=0 with valid Q2es (~15k MC tracks).
- ORIGINAL INSTRUCTION (2026-10-22): once Wave A has finished and been validated, PLAN a binning study. It should establish how differentially we can bin in x, Q2, z and pT_h, jointly with unfolding stability (resolution, purity/stability, occupancy, IBU/TUnfold condition, closure). Then dispatch subagents to execute it.
  - Wave A: clusters 5815113 data, 5815116 DJ14, 5815803 RG31 (done). Validator: `condor_v6/waveA_validate/validate.sub`.
- The user wants thorough multi-sample smoke tests before big Condor/storage spends; see [[feedback-smoke-before-production]].

**Why:** the user wants the low-Q2 analysis to have parity with the high-Q2 analysis; see [[naf-h1-sidis-access]] and [[h1-sidis-trackclass-impact]].
**How to apply:** start from the plan doc. Check the NAF state before acting, since the plan was written from a token-lean survey.
