# CJC-containment figures: provenance (2026-10-28)

`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`; scripts in `D/scripts/restart_2026-10-27/cjc_containment/`.
- Inputs: merged v6dif trees (DiffTot, ChargedTot, ChargedTotGen) listed in `queue_skim.txt` — the same 14 files the WP-A3
  skims were made from (`D/condor/restart_2026-10-27/wp-a3/queue_skim.txt`); nd4cat (7094/7096/7097) not processed.
- Skim: `cjc_skim.py` via `cjc_skim.sub` / `cjc_skim_job.sh`, Condor cluster 5930867 (14 jobs, rc 0; 7071/7091 held once
  for disk request and released with 15/40 GB; total wall ≈ 7 min). Output `D/data/restart_2026-10-27/cjc_containment/skims/*.npz`.
- Analysis: `cjc_analyze.py` via `cjc_analyze.sub`, cluster 5931108 (rc 0, 14 s) → `D/data/restart_2026-10-27/cjc_containment/cjc_containment.json`
  (script and input sha256 inside). Fitted scales read from `D/data/restart_2026-10-27/gate_evidence/veto_eff.json` (`fit_scales_fine`).
- Acceptance check in the job: odd Plug N−1 f_fail per D cell without the x_IP/M_X cut, data 0.2724 / 0.2996 / 0.3365 and
  unit-scale MC (7071) 0.0620 / 0.0825 / 0.1126, equal to `veto_efficiency_view.md` — PASS.
- Figures: `cjc_plot.py` (login node, reads the one JSON; style `gate_evidence/ge_style.py`).
- Reused code (imported, not copied): `wp-a2/n1_wpa2.py` (cells, run list, weights helpers), `wp-a/wpa_common.py`,
  `wp-c/study/xframe_obs.py` (frame B, G2, boosts), `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_final_2026-10-24/esigma_xf.py`
  (eSigma photon, gen photon). Weight block copied verbatim from `wp-a3/skim_wpa3.py`.
