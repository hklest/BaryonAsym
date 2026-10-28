# Script Audit 2026-10-19 — Loop 1: v6 TREEBUILDER Sources
## H1 High-Q2 Inclusive Charged-Hadron SIDIS (Q2 > 150 GeV^2, no species PID)

- **Date:** 2026-10-19
- **Auditor:** physics-correctness critic (read-only), loop 1 of 3
- **Scope:** line-by-line read-only critique of the v6 Stage 1 and Stage 2 tree builders
  plus their supporting headers, cross-checked against the canonical copies, with
  downstream contract verification by targeted grep (no edits, no recomputation,
  no ROOT inspection).

### Material reviewed

| Item | Path | Coverage |
|---|---|---|
| Stage 1 builder | `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` | all 3130 lines |
| Stage 2 builder | `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` | all 2123 lines |
| Stage 1 canonical | `scripts/canonical_highQ2_sidis_2026-06-30/builders/TreeBuilderHighQ2_chargedsidis_v6.C` | full; **byte-identical, zero divergence** |
| Stage 2 canonical | `scripts/canonical_highQ2_sidis_2026-06-30/builders/ChargedSidisTreeBuilderHighQ2_v6.C` | full; **byte-identical, zero divergence** |
| Stage 1 headers | `src_stage1/PhysicsConstants.h`, `src_stage1/TreeBuilderHighQ2_chargedsidis.h` | full |
| Stage 2 headers | `src/PhysicsConstants_chargedsidis_highQ2_v6sys.h`, `src/ChargedSidisUtils_highQ2_v6sys.h`, `src/H1CalcSystematic_v6sys.h` | full |
| Downstream contract evidence | `sidis_highQ2/context-handoff.md`, `scripts/canonical_highQ2_sidis_2026-06-30/docs/RUN_STATUS.md`, grep of `H1ChargedSidis_HighQ2/scripts` (z/nhit/radlen/MatchTier/EvlTree/lumi tokens) | targeted |

**Observable class:** inclusive charged-hadron SIDIS cross-sections (d sigma in nb,
IBU-unfolded, currently released as cross-sections, not multiplicities).
**Analysis family:** sidis_highQ2 (charged SIDIS).

## VERDICT: CONCERN — CONDITIONAL PASS

No CRITICAL/FATAL defect was found: the core delivered semantics (per-event weights,
pi/K/p-only gen truth, envelope-OR gating, frame algebra, species-dependent masses,
trigger-weight formula) are implemented as documented, data/MC-symmetrically, and the
working copies are byte-identical to the canonical snapshot. The builder is
**conditionally passed for the currently released cross-section chain**, whose
downstream gates have validated internal consistency. It is **not cleared for
unreviewed derivative use** — the five MAJOR items below must be dispositioned before
multiplicity products, gen-z-sensitive observables, or any DisTot re-attachment are
trusted.

## MAJOR findings (5)

### M1. Reco event-level kinematics read from frozen production-HAT pointers; provenance claim unverified; systematic-shift legs carry mixed shifted/unshifted branches
- **File/lines:** Stage 2 `ChargedSidisTreeBuilderHighQ2_v6.C` 1030–1051 (also 109–110, 1122–1134); Stage 1 `TreeBuilderHighQ2_chargedsidis_v6.C` 475/877 (sidecar write), 399–413 (HAT reads), 1003/1988 (HAT reads in stage_lambda/kaon).
- **Quote:** Stage 2: `Q2s = *hatQ2s; Ys = *hatYs; weight1 = *hatWeight1; ... PVz = *hatVtxZ;` with comment "Q2s, Ys are the calibrated eSigma values from upstream stage_evls" (1027–1029) and header "The HAT pointers Q2s, Ys (calibrated eSigma values) are readable in event-list mode" (109–110). Stage 1 stage_evls writes Q2s **only** to the EvlTree sidecar (`evlTree->Branch("Q2s", &t_Q2s, "Q2s/D")`, filled `t_Q2s = q2` from the HadrooII-calibrated calculator); no HAT-branch write exists anywhere in stage_evls.
- **Why it is a risk:** In event-list mode the HAT is the production DST HAT, so `*hatQ2s`, `*hatYs`, `*hatElecE/Theta/Phi`, `*hatVtxZ` are **production-frozen** values, not the stage_evls-calibrated calculator values that drove the envelope decision (sidecar `t_Q2s/t_Ys/t_Empz/t_VtxZ/t_ElecE...`). The in-code claim that they are the calibrated upstream values is unsubstantiated by any write path. Three consequences: (a) the released Q2/y/Empz spectra are on the production eSigma definition while the analysis documentation describes a HadrooII-calibrated selection; (b) `gamma`/`z_SIDIS`/`Ws`/`xBj` (built from HAT `ElecE/ElecTheta`) can be built on a slightly different electron than the one the selection and QED-Compton veto used; (c) on v6sys systematic legs, `ApplyV6SysShift` moves only calculator outputs — the HAT-frozen branches (`Q2s`, `scatEX/Y/Z/E`, hence `z_SIDIS`, `ystar`, `xF`, `W`) stay **unshifted**, while `Q2e/Q2es` (calculator, lines 1057–1069) and the sidecar `ElecE/ElecTheta` (Stage 1 re-run with the shift) **are** shifted. The z observable actually used downstream, `z_SIDIS_es` (built from `Q2e`), does carry the shift, but Q2-bin-side quantities read from `Q2s` do not — a systematic leg can therefore under-respond in parts of the chain in a branch-dependent way.
- **Impact:** Live event-loss is unlikely (the v6 envelope is deliberately wider than the final cuts — Q2 100 vs 150 GeV^2, y 0.05–0.9, Empz 30–75 vs 35–70 — so percent-level definition differences are absorbed; data and MC use identical branches so the migration is consistently defined). But the kinematic basis of the release and the shift propagation of the detector systematic legs are both unproven at the branch level. Not analysis-killing on current evidence; **follow-up required**.

### M2. No rad-safe gen z (nor rad-safe gen gamma*/Breit frames); downstream gen-z binning uses the ISR-biased branch
- **File/lines:** Stage 2, line 1279 `gammaGen = eBeam4 - genScatElec4;` (nominal `*hatEBeamE`), feeding `zGen_cg` (1468–1470), `zGen_ch` (1821–1824), `ystarGen_cg` (1450–1451), `yBreitGen_cg` (1464–1466), `xFGen_cg` (1462). Rad-safe V5G-2 block uses `E_e_eff = GenBeamElecE_evt - GenEnPhoton` only for `Y_es_gen/Q2_es_gen/x_es_gen` (1325–1336) and V5G-3 only for `ystar_es_gen` (1350–1361).
- **Why it is a bug risk:** For RAD MC (DJANGOH14 7091, RAPGAP31 RAD), the exchanged photon is `gamma_true = eBeam - k_ISR - p_e'`; using the nominal-energy `gammaGen` overestimates `p.q`, biasing gen z **low** in ISR events. The truth level stored is therefore internally mixed: rad-safe gen Q2/x (`Q2_es_gen`, `x_es_gen` — confirmed as the release/C_rad binning variables) alongside nominal-beam gen z. Downstream evidence confirms the exposure: `scripts/scan_TUnfold/unfold_TUnfold_v3.py` bins gen on `zGen`, and the BCA vector builder declares `z_observable = "z_SIDIS_es"` on the reco side — so the response maps a (partially) rad-safe reco z against a non-rad-safe gen z for RAD MC, distorting the z migration and, through it, the unfolded z spectrum in a genuinely physical way.
- **Impact:** Potential systematic bias in the released z spectrum (especially z<0.05 and high-z bins) at a level that must be quantified, not assumed negligible; the direction is consistent (gen z biased low → migration smearing asymmetric). **Not yet demonstrated to invalidate released numbers**, but the truth-level definition is inconsistent within one response. Follow-up required: quantify `zGen` vs a rad-safe `z = p.p_h / p.(gamma - k_ISR)` on 7091, or produce the documented truth-contract decision that gen z is defined at nominal beams.

### M3. Tier-0 truth-link matching provides no exclusivity, no angle, no index — the "exclusive one-to-one matching" contract holds only for the Tier-2 fallback
- **File/lines:** Stage 2, 1690–1696: `const H1PartCand* cand = trk->GetParticle(); if (cand) { matched = cand->GetPartMC(); if (matched) { MatchTier_ch = 0; } }` — no `usedGenIndices_ch.insert`, no `GenMatchIdx_ch` (stays −1), no `MatchAngle_ch` (stays −1), no charge check, no `isChargedHadron` check. Tier 2 (1699–1728) does enforce `gp->GetCharge() != trk->GetCharge()` (1710), the 2° `PhysConst::CHARGED_MATCH_ANGLE` (1721, = 2.0° per header line 74), and exclusivity via `usedGenIndices_ch` (1724).
- **Why it is a risk:** Two central tracks can Tier-0-claim the same gen particle (split tracks, secondary interactions), and a reco track can Tier-0-link to a non-pi/K/p gen particle; the builder delivers none of the guarantees the delegation's contract states. Downstream is demonstrably aware — `scripts/analysis/qt_over_Q_resolution_2026-10-07/extract_qt_over_Q_resolution.py`: "Tier-0 uniqueness is a PROXY only (GenMatchIdx is unusable for Tier 0; guaranteed unique Tier-0 key ... its claimants dropped)" and `bgsub_fitnorm_2026-10-19` classifies on `MatchTier`/`MatchAngle` — but the mitigation lives in consumer scripts, not in the builder. Any consumer that assumes builder-level one-to-one matching double-counts matched legs (response matched leg, fake-fraction classification).
- **Impact:** No demonstrated corruption of the current released chain (the audited consumers implement the workaround), but every new consumer must re-implement uniqueness, and `MatchAngle_ch = -1` for all Tier-0 rows silently changes the meaning of any MatchAngle cut. Follow-up: confirm in loop 2 that the **central** chain's response/parentage scripts apply the Tier-0 unique-key rule.

### M4. Zero-hadron DIS events are absent from all written Stage-2 outputs (DisTot detached) — the N_DIS denominator must come from the EvlTree sidecar
- **File/lines:** Stage 2, 2115 `// v6sys: DisTot intentionally NOT written (detached at construction).`; 1542–1549 reco block skipped for non-reco-env events; gen rows only for MC. Same in the v6sys variant (`src/ChargedSidisTreeBuilderHighQ2_v6sys.C` 366–367 "DETACHED in v6sys (not written)").
- **Why it is a risk:** Events passing the DIS selection with zero accepted charged tracks produce **no** ChargedTot row. Any downstream multiplicity `M = N_h / N_DIS` built by counting distinct `(RunNumber,EventNumber)` on ChargedTot silently drops zero-hadron events, underestimates N_DIS, and **overestimates the multiplicity** — the classic SIDIS denominator trap. The correct ingredient exists: the Stage-1 EvlTree sidecar has one row per kept event with `Q2s/Ys/x/VtxZ/Empz/Weight1/Weight2/RunNumber/TrigS67/BunchType/pass_reco_envelope` (branches 475–495), i.e., zero-hadron events included.
- **Impact:** The currently released products are cross-sections (no N_DIS), so no live impact on released numbers; this is a **contract guarantee the builder does not deliver** and a live trap for any future multiplicity derivative. Follow-up: enforce sidecar-sourced N_DIS; never re-attach DisTot without fixing M6/m1 first.

### M5. Sidecar-exhaustion path silently continues with passRecoEnv = true
- **File/lines:** Stage 2, 1017–1021: `} else { cerr << "WARN: ran out of EvlTree entries ..." << endl; // Defensive: keep going; treat as reco-default for data. }` — while `passRecoEnv` was initialized `true` (992) and the RunNumber mismatch guard (1002–1015) can no longer fire once the sidecar is exhausted.
- **Why it is a risk:** Lockstep alignment is contractually guaranteed by Stage 1, so exhaustion indicates a real setup error (wrong sidecar file, truncated chain, mismatched steer). The strict-align machinery exists precisely for this failure class, but exhaustion bypasses it: all remaining events are processed as reco-envelope-passed, on data filling ChargedTot rows with unverified envelope status. On MC the same path also misclassifies events into the reco category.
- **Impact:** Requires operator error to trigger, but the failure is silent (one warning line) and corrupts everything downstream of it. Not observed in evidence; robustness defect. Follow-up: hard-fail on exhaustion.

## MINOR findings (8)

### m1. `nChargedGen = 0` reset AFTER the gen loop already incremented it (latent, detached-tree only)
- Stage 2, 1558–1560 — placed after the gen loop (1531–1533). For every passRecoEnv event the gen multiplicity is discarded before the DisTot fill; only gen-only events keep the true count. Zero impact today (DisTot not written); **latent trap if DisTot is re-attached**.

### m2. Stale Stage-2 header claims the final DIS cuts were already applied upstream
- Stage 2, 102–110 and 119: claims upstream applied "E-pz cut [45,65] GeV, Q2 > 150 GeV^2, y in [0.1, 0.9], |VtxZ| < 35 cm" and "DisTot - one entry per DIS event". Actual v6 design applies a deliberately **wider** envelope (Q2 > 100, 0.05 < y < 0.9, 30 < Empz < 75) and leaves the final cuts to downstream; DisTot is detached. Documentation defect; zero direct physics impact.

### m3. Trigger-weight header contradicts implementation; null-`H1TrigInfo` silently yields zero weights
- Stage 2, 631–636 comment vs implementation (1071–1119): both AC and RW computed from HAT subtrigger bits × prescales with the gtustin formula (`weight = 1/(1 - prod(1 - 1/p))`). If `H1TrigInfo` is missing from the DB, `TrigWeightAC/RW` remain `0.0f` for **all** events (warning on first event only). Loud-failure mode, not silent bias; MINOR.

### m4. Comment claims Stage 1 "wrote zero" for gen-only HAT quantities — unverified and likely wrong for RAD MC
- Stage 2, 1032–1035. Stage 1 zeroes only the **sidecar** `t_` columns; for RAD MC, gen-only events are fully reconstructed DST events whose production HAT carries real values. Code defensively correct (explicit zeroing at 1042–1051); documentation defect obscuring the true HAT provenance (see M1).

### m5. Gen-only mode hardcodes `|VtxZ| < 30.0` instead of `PhysConst::VTX_Z_MAX = 35`
- Stage 1, 600. Zero impact today (NORAD VtxZ ≈ 0); latent tighter-than-documented cut if gen-only mode meets real vertex spread.

### m6. Sidecar `GenVtxZ` stores the HAT/reco VtxZ, not the true gen vertex
- Stage 1, 933 `t_GenVtxZ = isMC ? (double)(*hatVtxZAll) : 0.0;` — misleading branch name; the true gen vertex is correctly provided by Stage 2 via `H1SelVertex::vtGen` (`GenVtxZ_cg`). No consumer identified; MINOR.

### m7. Good-run / detector-status filtering silently disabled if the file is missing
- Stage 1: warns and continues without run filtering if the list is absent/unreadable. Consistent with run-list validation living in `submit_highQ2_sidis_data.py` (whose EvlTree RunNumber-vs-run-set check is "Not implemented here", line 417). MINOR robustness.

### m8. Pass-2 combined-track sanity cuts let NaN Pt/P through
- Stage 2, 1928–1933: Pass 2 checks `Pt < 0.05f`, `isfinite(Eta)`, `P > 100.0f` — NaN Pt or NaN P **passes** (only Eta is finite-checked), unlike Pass 1 (1598–1600). NaN would propagate as a loud total failure rather than silent bias. MINOR (add the two isfinite guards).

## INFO findings (7)

- **i1.** Stale luminosity constants, unused by builders: `DATA_LUMI_ALL = 356613.0` (superseded) and MC placeholders `= 1.0` in the PhysicsConstants headers. Neither builder normalizes anything; the current `L_data = 351000.0 nb^-1` is applied downstream. One careless import away from misuse; recommend a banner comment.
- **i2.** 7091/DjBkgd luminosity trap: builders are clean. No MC luminosity anywhere; the scale `351000.0/4379935.0 = 0.08013817556653238` and the forbidden `15813.4`/`22.1963651` are purely downstream concerns.
- **i3.** `xBj`/`Ws` are NaN on gen-only events (1139–1141, 0/0). Reaches only the detached DisTot; zero impact while detached.
- **i4.** Stage-2 `Empz` is a Sigma-method approximation, explicitly a cross-check (1165–1177). The sidecar carries the Stage-1 calculator Empz. Downstream E-pz cuts must pick one source consistently (loop-2 check).
- **i5.** Gen rows are filled for every list event, not only gen-envelope events; the sidecar lacks EventNumber. Downstream gen sums must re-apply the gen envelope from stored gen kinematics or join via `(RunNumber,EventNumber)`; the sidecar has no EventNumber, so flag-joins require lockstep or re-derived cuts. Current release gates indicate this is handled; recorded as a contract subtlety.
- **i6.** Lineage asymmetries in the Stage-1 file unused by the charged path: `stage_kaon` DCA sig >= 5 vs `stage_lambda` >= 1; Stage-1 `TreeUtil::matchRecoToGen` MATCH_ANGLE_THRESHOLD = 15.0 (V0 stages) vs Stage-2 charged matcher 2.0°. Not a conflict; recorded to preempt confusion.
- **i7.** Cut-tag semantics: builder is agnostic and complete; application is Stage-3. The tags `zlow005_pT5_eta1p6_radlen5_nhit5` are Stage-3 cut tags; the builder stores open-acceptance and provides every needed branch (`Pt`, `Eta`, `RadLength`, `NHitCJC` AND `NHitDedx`, `z_SIDIS`, `z_SIDIS_es`, `zGen`). Pass-1 vs Pass-2 minimal-cut asymmetries are irrelevant under the downstream pT > 0.5 cut. No gen `pT_h` branch is produced (the "blocked until six gen-electron branches" rule is respected — gen-electron 4-vector components stored for downstream pT,h computation).

## Suspected bugs examined and RULED OUT (with reasons)

1. **Trigger-weight formula / double counting** — `1/(1 - prod(1 - 1/prescale))` over fired+enabled subtriggers matches gtustin exactly, `prescales[st] > 0` guard, intended 0.0 sentinel; standard H1 128-subtrigger bit packing. No product with `weight1/weight2` precomputed in-builder.
2. **Charge swaps** — Tier-2 enforces `gp->GetCharge() == trk->GetCharge()`; `chargeSignForPDG` handles e-/mu-/Sigma- sign exceptions; `Charge` branch = `trk->GetCharge()` in both passes.
3. **Weight double-counting / luminosity misuse** — `weight1`, `weight2` stored raw once per row; no luminosity, no MC normalization, no cross-section anywhere in either builder. Data: `weight2 = 1.0`; MC: `weight2 = *hatWeight2` — matches the workspace `weight1*weight2` convention. (Caveat: assumes data HAT `Weight1 = 1.0`, standard H1 production convention — worth a one-time verification.)
4. **Gen-denominator species leak** — gen loop is pi/K/p-only (`isChargedHadron`), `IsGtr()` filter restored (V5E), `Mother2 == -1` veto (F7); STR admission retained only in the matching pool, with `IsStr`/`IsMatchedV0Daughter` flags for downstream Option-H masking.
5. **Scattered-electron track contamination** — `IsScatElec()` skip present in both passes (1581, 1922), before the primary/central filters.
6. **Pass-2 dedup defects** — central-wins, same-charge, dR < 0.05 with phi wraparound handled (1939–1950); sound.
7. **Empz Sigma-method algebra** — verified consistent with its documented approximation.
8. **QED-Compton / LAR fiducial cuts** — acoplanarity `-cos(dphi) > 0.95`, zimpact/phi-crack handling standard.
9. **Integer division / type mismatches** — all kinematic math in double/Float_t; `1.0/prescales[st]` double; HAT byte pointers indexed as booleans per H1OO convention; sidecar doubles vs tree floats (precision loss negligible).
10. **v6sys shift helper** — audited enum whitelist, frozen magnitudes (elecE ±0.5% wheel array; elecTh ±1 mrad paired with eElecTrTh; HFS ±1%; hfs_theta ±1 mrad; hfs_phi ±20 mrad), coherent paired eHadEn+eHadEnRCES for `hfs_energy_combined` only, central sentinel −9999 no-op, per-event `ResetAllShifts()` before the shift, applied after `gH1Calc->Reset()` and before any calculator read. No double-shift, no sign error. (Branch-dependent propagation is M1's sub-point, not a helper defect.)
11. **Per-track/per-row initialization** — resets verified present before every fill in both passes and the gen loop (1658–1683, 2041–2071, 1421–1433); `TrackLength_ch` set in both branches; all `_es`/`_es_gen` sentinels handled in else-branches. (Exception: m8.)
12. **Canonical divergence** — none: Stage 1 identical for all 3130 lines, Stage 2 identical for all 2123 lines (full sequential comparison, not sampling).
13. **Release-binning gen variables** — builder stores `Q2esGen` (calculator) and rad-safe `Q2_es_gen`/`x_es_gen` per row and per event, matching the releasebin C_rad scheme `(Q2esGen, x_es_gen)`; `GenEmpz` available on the sidecar for the EMPZ>45 truth contract.

## Missing evidence (for loops 2–3 or specialist handoff)

1. **Stage-3 / central-chain scripts** (loop 2): actual application point of `zlow005/pT5/eta1p6/radlen5/nhit5`; which Empz/Q2 branch the central data-vector builder cuts on; whether the **central** response/parentage scripts apply the Tier-0 unique-key rule (M3 evidence so far is from side-study scripts).
2. **Structural check of one evls output + its Stage-2 output** (h1-root-analyst, read-only): does the HAT in Stage-2 inputs carry a production `Q2s`, and per-event scatter of sidecar `Q2s/Ys/Empz/VtxZ/ElecE` vs stored `Q2s/Ys/Empz/PVz/scatE*` (settles M1).
3. **Documented truth-contract decision** for gen z (nominal-beam vs rad-safe) — needed to disposition M2; if absent, a quantification of the `zGen` ISR bias on 7091.
4. **Data HAT `Weight1` value check** (one-file, read-only) to close the ruled-out item 3 caveat.

## Recommended Next Check

**Per-event join of the Stage-1 EvlTree sidecar (`Q2s/Ys/Empz/VtxZ/ElecE`) against the Stage-2 stored branches (`Q2s/Ys/Empz/PVz/scatE*`) on one evls file plus its Stage-2 output, by `h1-root-analyst` (read-only).** This single check settles M1 — the largest unproven provenance question, including whether systematic-shift legs carry the shift in every branch the central chain consumes — and directly feeds loop 2's cut-application audit. M2's rad-safe-z quantification is the follow-on physics check.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 1 of 3 (treebuilder), read-only; no analysis script was modified.
- Report persisted by the delegating supervisor session (critic toolset read-only).
