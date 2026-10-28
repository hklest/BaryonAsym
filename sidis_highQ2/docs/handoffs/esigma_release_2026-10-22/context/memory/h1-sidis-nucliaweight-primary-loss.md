---
name: h1-sidis-nucliaweight-primary-loss
description: "NucliaWeight primary-loss term built 2026-10-21 as a diagnostic candidate (±1.18%, not installed); the nominal under-applies NW by 0.45%"
metadata:
  node_type: memory
  type: project
  originSessionId: fb2c584f-062b-44ff-aa59-b65cb0b10601
  modified: 2026-10-21T22:40:54.348Z
---

On 2026-10-21 the user called the env100 plot-label bug and the NucliaWeight gap the "final two blockers". They are NOT quoting blockers: STATE.md lists M1, N2, plotter re-pointing and the note.

- **Plot labels:** fixed as the additive edition `plots/systematics/bgsub_env100_quoted_total_21comp_7col_fixedy_labelfix_2026-10-21/`.
- **NW candidate (user said "go ahead", κ=100%, independent of int):**
  - Builder copy at `scripts/unfolding/nucliaweight_variant_2026-10-21/`, Condor 5815153 (174 shards, about 2.4 CPU-h) and 5815199.
  - Loss-to-miss form: integrated ±1.18%, per-bin median about 0.77%, total median +0.04 pp. Charge-symmetric; DJ and RG agree.
  - Products: `output_v6/studies/nucliaweight_variant_2026-10-21/nucliaweight_term/`.
- **Nominal defect found:** IBU prior = colsum(B)+miss, with no NW deficit in the miss row, gives efficiency nC/(nC+M). The 2026-09-09 memo ruled that form physically wrong. The coherent-prior fix (2026-10-05) re-introduced it, and the OPT-A nominal is low by 0.453%.

- **Degeneracy check (user asked before install):** the fake-leg piece duplicates bgsub_fakefrac_rel (r_min = template/fake-col ratio, NW-driven). Recommended: body+miss only, per-bin median ≈0.65%. No shape degeneracy otherwise (shape-only R² 0.30 on all v5 vectors). Possible conceptual overlap with tracking_eff_m0p5 cannot be verified (H1 note unavailable).

- **INSTALLED 2026-10-21** after two critic reviews and user sign-off (κ=±100% ratified, overlaps kept, the 0.45% bias not logged, h+/h−/A_h zero by construction).
  - Budget `systematics_bgsub_env100hyp_nonprimary_nw_optA_2026-10-21/`, medians 7.4848%/7.8604%.
  - Covariance `z005_covariance_optA_nonprimary_v5_nw_2026-10-21/`, 7.4016%/7.6525%.
  - Canonical edits via `s14_promote` (sha-guarded).

**Why:** a new quoted column, or a nominal change, is a baseline decision. Per house practice it needs user authorization and critic rounds.
**How to apply:** the v5+NW budget is the current quoted one. Don't re-raise the 0.45% bias or the proton gap as blockers (both are user-dispositioned). A nominal NW fix would not require regenerating the term (it is convention-invariant). Record: local `docs/nucliaweight_primaryloss_candidate_2026-10-21.md`. Related: [[h1-sidis-trackclass-impact]], [[h1-sidis-detint-uncertainty-gap]].
