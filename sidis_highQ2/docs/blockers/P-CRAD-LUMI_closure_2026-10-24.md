# P-CRAD-LUMI closure: radiative-correction exposure and the DJANGOH closure gap

Status: CLOSED 2026-10-24 for the eΣ (esigmaphoton 2026-10-23) release, by user decision (accepted gaps). Nothing was computed; the gaps are accepted and documented.

## What the blocker was
The radiative correction is C_rad = σ_nonrad / σ_rad. Each of the DJANGOH14 and RAPGAP31 samples (with and without radiation) needs the right MC exposure (the effective luminosity of that generated sample). This is not the data luminosity. Two things were open:
- The file sets were reconciled on 2026-10-09 (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md`; `context-handoff.md` L336, L351, L376; `STATE.md` L86), but the generator-log cross-check could not be done. The H1 MC database web interface is disabled.
- The DJANGOH generator-only C_rad samples do not close against the DJANGOH detector-MC truth: +1.65% integrated, and −2.6%, −4.0%, −6.8% at z = 0.2, 0.3, 0.5. RAPGAP closes to 0.2% (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/reference/critic_phys_final_production_2026-10-21.md` L27, row 8, MAJOR documentation).

## User decisions (2026-10-24)
1. Accept leaving the generator-log cross-check undone because the MC database web interface is disabled. Revisit if MC database access returns.
2. Accept the current handling of the DJANGOH closure gap (up to −6.8% at z = 0.5). That handling is promotion decision D3 (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/promotions/esigmaphoton_promotion_2026-10-23.md`, about L45): the applied C_rad stays DJANGOH14 (consistent-definition tables), RAPGAP31 is the formal alternative, and the DJANGOH gen-only σ_rad does not close to the DJANGOH detector-MC truth (+1.65% integrated, z-shaped; RAPGAP closes to 0.2%).
3. The no-Z Born factor C_gammaBorn (`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_490rows.csv`) carries the two C_rad uncertainty components below and gets no new term (signed off).

## Evidence: uncertainty carried
- `c_rad_gen_rel` = 0.5 × smooth5(|RAPGAP − DJANGOH| / DJANGOH) and `c_rad_stat_rel` = the DJANGOH relative statistical uncertainty (`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/final_rerelease_2026-10-22/budget_groupB/components_groupB.py` L240-262, L481-483).
- Median values in the eΣ budget: 0.57% and 0.43%.
- These are inherited electron-chain generator-level tables and were not rerun. The critic ruled that the generator target is unchanged (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/reference/esigma_frame_critic_review_2026-10-22.md:9`).
- No new uncertainty term is added.

## What this does not change
- The RAPGAP−DJANGOH spread (median about 0.57%) is smaller than the DJANGOH closure gap at high z. The user accepted this as documented in D3; this note does not judge it.
- The generator-log check is not done. It reopens only if MC database access returns.

## Pointers
- Audit: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/blockers/quoting_blockers_audit_2026-10-24.md` (follow-up, P-CRAD-LUMI section).
- Memory (`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/memory/decisions/`): `2026-10-24-p-crad-lumi-accepted-gaps.md`, `2026-10-24-noz-born-factor-no-own-uncertainty.md`, `2026-10-24-m1-n2-closed-d2-eprime-cut-shifted-legs.md`.
- Script that applies the registry and status edits: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_release_2026-10-23/s18_close_quoting_blockers_2026-10-24.py`.
