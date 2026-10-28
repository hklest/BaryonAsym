# Loop-1 M1 follow-up: sidecar-vs-Stage2 kinematic provenance check (read-only inspection)

Date: 2026-10-19. Inspector: h1-root-analyst (read-only; no analysis files modified).

## Files (2, both small; all reads < 1 s each; no Condor)

- Stage-1 sidecar: `H1ChargedSidis_HighQ2/output_v6/central/evls/data/03/evls_v6_d03_0006.root` — EvlTree, 67 entries (data, ep0304).
- Stage-2 output: `H1ChargedSidis_HighQ2/output_v6/central/sidistree/data/ep0304/sidistree_ep0304_d03_0006.root` — ChargedTot, 575 tracks, 65 unique events (2 sidecar events trackless: run 362518, sidecar Q2s = 343.8, 683.4).

Stage-2 job log (text only): `condor_logs/sidistree_highQ2_v6_central/sidistree/data/03/sidistree_ep0304_d03_0006.out` — "575 reco charged tracks from 67 events", "RunNumber mismatch warnings: 0".

## Join method

Sidecar has no EventNumber branch (TEventList keys use a different event-number convention than ChargedTot's `H1Tree::GetEventNumber()`), so the join is order-based per the v6 lockstep contract (H1EventList order; 0 lockstep RunNumber mismatches in the Stage-2 log), anchored event-by-event by exact VtxZ+ElecE agreement: 63/65 pairs anchored coherently; the remaining 2 resolved by unique order position between neighbor pairs plus partial anchors (event B: VtxZ exact match 8.6852 and Stage-2 live Q2es = sidecar Q2s = 278.206; event A: ElecE exact match 21.3276). Order preserved across all 65 pairs.

## Per-quantity verdicts (65 joined events; "clean 63" excludes the two pathological events A/B)

- **Q2s — PROVENANCE-MISMATCH.** Stage-2 `Q2s` (HAT ptr `Q2s`) vs sidecar `Q2s` (calibrated eSigma): 0/65 identical (0 within rel 1e-6). Clean 63: rel diff median 4.9%, mean 9.5%, max 52.4%. Cross-check: Stage-2's own live-recomputed `Q2es` column reproduces sidecar Q2s exactly on 64/65 (exception event A, rel 60%) — i.e. the calibrated values were available in-framework but the stored Q2s is the frozen production-HAT value. Cut impact: Q2>150 decision flips for 7/65 events.
- **Ys — PROVENANCE-MISMATCH.** 0/65 identical. Clean 63: median 6.5%, mean 11.1%, max 75.2%. Live `Yes` column is much closer (median 1.7%) but not stored.
- **VtxZ vs PVz — CONSISTENT (with 1 exception).** 64/65 bit-identical; event A (run 362516/787): HAT VtxZ 75.37 vs sidecar 17.78 (|75.37|>30 would fail a downstream PVz vertex cut).
- **ElecE vs scatEE — CONSISTENT (with 1 exception).** 64/65 agree to <=1.1e-9 relative (float32 rounding of same value); event B (run 362526/19348): HAT ElecE = 0 vs sidecar 24.67.
- **Empz — DIFFERENT BY DESIGN (documented Sigma-method approximation built from HAT Ys, eE).** 0/65 identical; clean 63: median 0.82%, mean 1.4%, max 26%; inherits the Ys provenance problem (event B: 0 vs 53.0).
- **Weight1 — CONSISTENT.** Identically 1.0 in sidecar (all 67) and Stage-2 (all 65 events / 575 tracks). Data weights are 1.0.

## Pathological events (both real DIS data events with tracks)

- Event B: run 362526 / ChargedTot EventNumber 19348 (5 tracks). Sidecar: Q2s 278.21, Ys 0.149, ElecE 24.67, Empz 53.0, VtxZ 8.685. Stage-2 stored: Q2s 0.0, Ys 1.0, scatEE 0.0, Empz 0.0, PVz 8.685. The frozen HAT branches are degenerate (0/1) while Stage-2's live calculator reconstructs Q2es = 278.21.
- Event A: run 362516 / EventNumber 787 (3 tracks). Sidecar: Q2s 173.62, Ys 0.1225, VtxZ 17.78. Stage-2 stored: Q2s 89.01, Ys 0.0998, PVz 75.37 (scatEE matches).

## Task-4c answer

The Stage-2 input HAT carries a frozen production `Q2s` whose values (as relayed into ChargedTot by `H1FloatPtr("Q2s")`) differ from the sidecar calibrated eSigma on 65/65 events (median ~5%, max 52% clean; one degenerate 0-value event). The HAT file itself was not opened (two-file limit); this is measured via the Stage-2-stored HAT-pointer values plus code inspection of `ChargedSidisTreeBuilderHighQ2_v6.C` (hatQ2s/hatYs/hatVtxZ/hatElecE/hatWeight1 pointers, lines ~428-442, ~1037-1041) and the Stage-1 filler `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` (sidecar = live `gH1Calc->Kine()->GetQ2es()/GetYes()`, calibrated Elec, Vertex, Fs Empz).

## Bottom line

M1 confirmed for Q2s/Ys: Stage 2 stores frozen production-HAT kinematics, not the HadrooII-calibrated sidecar values used for the Stage-1 envelope decision; PVz/scatEE are mostly consistent but each has one pathological event; Empz differs by design; Weight1 fine.
