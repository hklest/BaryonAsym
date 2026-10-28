2026-10-04 RUNSTATUS — Condor cluster 5323783 (submitted 2026-10-03T23:34Z, Running on slot2_9@batch1554.desy.de) regenerates reco phi_h compact Trento overlays via wrapper /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/plotting/phi_reco_luminorm_2026-10-03/run_phi_reco_trento_compact.sh and script /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/plotting/phi_reco_luminorm_2026-10-03/plot_phi_reco_trento_luminorm_compact_v6.py into /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/plots/phi_angular_overlays_reco_luminorm_v6/compact_alpha0p2_trento_L351000_20261003/ (expected: ~4 PDFs + histograms.npz + metadata.json + manifest.json; inputs data=658/djangoh14=501/rapgap31=364 all verified; expected runtime ~36-40 min per accepted precedent 4100612; submit file: plots/phi_angular_overlays_reco_luminorm_v6/compact_alpha0p2_trento_L351000_20261003_condor/submit_phi_reco_trento_compact.sub; logs in same _condor dir).

## Validation verdict (appended 2026-10-04 by NAF operator monitor, post-terminal)

**Terminal state:** Completed 2026-10-04 01:50:12, cluster 5323783.0, normal termination rc=0 (condor_history ST=C, run time 0+00:16:51 — faster than the ~36-40 min precedent estimate; stderr empty; stdout verdict: wrote 4 PDFs, npz cache, and manifests — PASS).

**Artifacts (7/7 present, nonzero, output dir `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/plots/phi_angular_overlays_reco_luminorm_v6/compact_alpha0p2_trento_L351000_20261003/`):**
- `phi_reco_luminorm_compact_distributions_plus.pdf` (361,014 B), `..._distributions_minus.pdf` (361,781 B), `..._ratios_plus.pdf` (445,619 B), `..._ratios_minus.pdf` (443,186 B), `phi_reco_luminorm_compact_histograms.npz` (383,946 B), `phi_reco_luminorm_compact_histograms.metadata.json` (13,851 B), `manifest.json` (13,851 B).

**Page counts (pypdf):** distributions_plus=7, distributions_minus=7, ratios_plus=7, ratios_minus=7 (28 total) — matches accepted precedent `compact_alpha0p2_styled_cache_retry_20260702` (7/7/7/7). PASS.

**Manifest checks (vs accepted precedent):**
- L_DATA = 351000.0 — PASS.
- L_MC present (djangoh14=10,487,495.0; rapgap31=9,132,891.0) with explicit `mc_scale_per_sample` (djangoh14=0.03346843073584302; rapgap31=0.03843251824641288), consistent with 351000/L_MC — PASS.
- Alpha cut recorded as 0.2 rad reco-only (`alpha0p2_cut=true`, `alpha_cut_rad=0.2`, reco-only survivor-cut statement) — PASS.
- Phi method `canonical_gamma_star_p_CM_Trento_from_extract_pertrack_v5g_fixedFrame_v3_boost_to_W_rest` (gamma*p CM Trento, boost to W rest) — PASS.
- Input file counts: data 658/658, djangoh14 501/501, rapgap31 364/364 — PASS.
- Total data entries `tree_entries_seen` = 7,538,865, equal to accepted value (no deviation) — PASS.

**Yield deltas (npz cache sums, new vs accepted):** data h+ new=2,236,277.0 vs accepted=2,236,277.0 (+0.0000%); data h- new=2,179,535.0 vs accepted=2,179,535.0 (+0.0000%). MC (context): djangoh14 h+/h- and rapgap31 h+/h- all +0.0000%. All bin edges identical to precedent; per-panel max abs rel diff 0.000% (244 non-empty panels per charge, 0 panels >3%). Note: the 0.0% delta (rather than a -0.5..-1% shift) is consistent because the accepted precedent manifest itself already records the same alpha0p2 cut (alpha_cut_rad=0.2), i.e., this is a faithful regeneration of an already-cut baseline — no structural change.

**Anomalies:** none. No negative bins in any histogram. One empty data panel per charge (h+: cell6 z(0,0.05)×pT(4,8) GeV; h-: cell7 z(0.7,1.0)×pT(0,0.5) GeV) — both identical to the accepted precedent (no new empty panels). MC panels all non-empty.

**VERDICT: PASS**
