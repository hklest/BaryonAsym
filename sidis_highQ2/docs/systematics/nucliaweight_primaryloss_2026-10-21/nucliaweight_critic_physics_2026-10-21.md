# Hostile physics critic review: `nucliaweight_primaryloss_rel` (candidate 2026-10-21)

## Verdict: SOUND-WITH-CONDITIONS
There are no blockers. The construction is correct and does not depend on the nominal's convention (F1). The conditions
concern scope claims (charge-blind construction, protons) and documentation and bookkeeping. The critic made no remote
writes (read-only `python -B` checks on the login node).

## Findings

- **F1 (PASS): loss-to-miss is the right derivative, and the term is invariant under the nominal's convention.**
  - In the engine, A = B/P and n0 = P, so y_j ∝ P_j·h_j(B, d). Loss-to-miss keeps P fixed.
  - Re-running the term around a nominal with the deficit moved into the miss row gives per-bin max abs difference
    6.2e-15 (h+) / 6.0e-15 (h−). The integrated value goes +1.179 → +1.180%.
  - Engine-native moves P together with B, hence its 0.61× size.
  - Action: state this invariance in the record.
- **F2 (MAJOR): the nominal carries a known one-sided low bias of +0.453% from NW.** This is the convention the
  2026-09-09 supersession forbids (builder L833-840). The per-bin shift is exactly D_col_j/P_j.
  - Action: escalate to the user as an open central-value defect (STATE + `unpropagated_sources.json`).
  - Install text must say the term does not cover this bias.
- **F3 (MAJOR): charge symmetry holds only by construction.** `ani_ineff.f` L44-67 is charge-blind (`trq` only sets the
  sign of the curvature, `pt = abs`), and protons get NW = 1.
  - The house's own estimates: material-induced charge asymmetry 0.1–0.5% at low pT
    (`research-tracking-systematics-deep.md` §3.2); proton omission O(0.001–0.003) on A_h (reference-nuclia doc §4).
  - Action: do not quote the NW ratio/A_h numbers as an uncertainty on those observables. Either book a charge-split
    `nuclia_no_proton_path` piece (protons get the kaon weight, or 1.5×), or register an open gap for h+/h−/A_h.
  - Also fix reference-doc §5, which wrongly calls the routine charge-dependent.
- **F4 (MINOR): cross-charge ρ = 1 (NW) vs ρ = 0 (v5 int) for the same driver.**
  - Action: justify ρ = 1 by the charge-blind correction; the asymmetric part is handled under F3.
- **F5 (MINOR): κ = ±100% is a fiat choice, not yet ratified.**
  - The routine documents no uncertainty and dates from 2005. Its geometry table is valid only up to run 436888;
    later runs fall back to `irrange=4`.
  - The 1302.1321 source README L13 says "1.0% combined … nuclear-interaction". The earlier INDEX.md flagged "±100% vs
    H1-style ±50% needs supervisor decision".
  - At ±50%: integrated ≈ 0.59%, median total increment ≈ +0.01 pp.
  - Action: the user records the κ decision explicitly.
- **F6 (MINOR): sign convention for the covariance.** max(|s+|, |s−|) has a median ratio to the mean |s| of 1.0074, and
  |s(+1) + s(−1)| has a median of 0.011%.
  - Action: build the rank-1 vector as sign(s(+1))·term.
- **F7 (MINOR): the double-count wording is inaccurate.**
  - (a) The tracking source says the H1 number includes nuclear interactions. The installed tracking term is a
    random-drop ensemble with diagonal covariance and ρ = 0, so "≤ 0.5% coherent" overstates the overlap.
  - (b) `bgsub_fakefrac_rel` is a fc0 common-model proxy that only carries an NW caveat. It also covers
    e/other/det.int. model dependence, and |1 − r_min| = 1.83% is 155% of the 1.18% NW fake deficit.
  - Action: reword both as "partial and unproven overlap". The keep-both decision stands.
- **F8 (MINOR): subtracted backgrounds do not move with κ.** The B_R′ templates exclude NW by convention, but V0 and
  lowQ2 tracks suffer the same loss. The missing piece has the same sign as the term, so the term is slightly low:
  ≲ 0.07% integrated, a few tenths of a percent in the highest-background bins.
  - Action: document as a known omission, or bound it.
- **F9 (MINOR): rank-1 with a single κ is normalization-like.** It cannot represent p/θ shape errors of the per-track
  correction, and in a fit its flat part is degenerate with lumi.
  - Action: document this in the covariance README.
- **F10 (MINOR): κ = +1 gives negative weights with no re-clip.** One negative body cell (h+). Note only.
- **F11 (PASS): Born/raw invariant.** C_rad is gen-only and contains no NW.
- **F12 (MINOR): install bookkeeping.**
  - Update `unpropagated_sources.json` L81-84, the v5 addendum L107 caveat, STATE, RUN_STATUS and current-release.
  - Re-read the canonical files right before editing.

## Conditions for install
1. The user explicitly ratifies κ (F5).
2. The +0.453% bias is logged as an open user decision, and the install text says the term does not cover it (F1, F2).
3. Ratio and A_h:
   - Label the NW ratio/A_h figures "zero by construction".
   - Book the proton / charge-asymmetric loss, or register it as an open gap (F3).
   - Justify ρ = 1 (F4).
4. Correct the overlap wording (F7), and document F8 and F9.
5. Covariance edition with sign(s(+1))·term, gates re-run, and register/caveat entries updated (F6, F12).
