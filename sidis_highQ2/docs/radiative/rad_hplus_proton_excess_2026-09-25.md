# DJANGOH14 RAD positive-hadron/proton excess — generator-level diagnosis

**Status:** read-only bounded diagnosis. `C_rad_applied=false`. No source, ntuple, steering, production, or central release product was changed.

## Executive verdict

| rank | proposed cause | verdict | decisive evidence |
|---:|---|---|---|
| 1 | RAD/NONRAD generator-domain and selected-hadron-population mismatch | **VERIFIED dominant blocker** | The RAD `Q2GT60` and NONRAD `Q2GT100` campaigns are not a matched generated phase space. Their selected weighted multiplicities are RAD `(h+,h-)=(1.084657,0.849332)` and NONRAD `(1.514140,1.490246)` tracks/event; the ratio-of-ratios is `1.256925`, equal to the `1.2771/1.0160=1.2570` charge anomaly. |
| 2 | Photon-axis observable pathologies in rows outside the valid e-method fiducial | **VERIFIED, but not the integrated cause** | In representative RAD pT_h>=8 rows, 99.16% have `Y_es_gen<=0.1`, 75.8448% have `zGen>1`, and only 0.389% pass event fiducial. h+ is 79.4193% PDG `2212`; h- is 97.4324% PDG `211`. This is a RAD-side pathological population but lies outside the displayed `pT_h<=8` bins and cannot explain the integrated 0--8 result. |
| 3 | Nominal-beam q in `RC_treebuilder_genonly_v2.C:209` | **VERIFIED implementation defect; quantitatively subdominant** | The code uses `kin=(0,0,-GenBeamElecE,GenBeamElecE)` after computing e-method variables with `eeff=GenBeamElecE-GenEnPhoton`. On selected displayed rows, the bounded B/A retained RAD yield is 0.999611 for h+ and 0.999609 for h-, so the estimated integrated C_rad change is only +0.039%. |
| 4 | Beam-remnant/diffractive proton leakage through an xF sign flip | **REFUTED for the selected displayed population** | Recomputing xF with ISR-corrected q on 16 bounded RAD files found 0/129184 selected proton rows changing stored `xFGen<0` to corrected `xF>=0`; weighted fraction 0.000000. |
| 5 | Electron/positron track leakage | **REFUTED** | The fill loop applies `isChargedHadronPDG(gp->GetPDG())` before filling and this is exactly `abs(PDG) in {211,321,2212}`. PDG `11` cannot enter. `GenCharge` is the charge returned by the same `H1PartMC` GTR object and agrees with the PDG-defined selected species. |
| 6 | GEANT/material secondaries because v2 omitted `ParticleSource!=4` | **REFUTED** | `IsGtr!=0` removes STR particles; PS=4 is an STR material-secondary classification and is structurally impossible after GTR selection. The RAD tail is essentially PS=0, not PS=4. |

## Bounded ISR-corrected xF test

The test applies the exact truth-v2 source-pool/event cuts and frozen `(Q2,x,z,pT_h)` support, then calculates q from `kin_eff=(0,0,-eeff,eeff)`, `eeff=GenBeamElecE-GenEnPhoton`, and recalculates `xF=hcm.Pz()/pcm.P()` using `W=q+P`.

| quantity | bounded result |
|---|---:|
| RAD files | 16 (8 e+, 8 e-) |
| selected proton rows | 129184 |
| selected protons with stored xF<0 and corrected xF>=0 | 0 |
| unweighted fraction | 0.000000 |
| weighted fraction | 0.000000 |
| h+/h- before removal | 1.0167991 |
| h+/h- after removal | 1.0167991 |

This test directly rejects the proposed mechanism for selected protons: correcting the ISR beam four-vector does not expose a target-hemisphere proton population hidden by a sign-flipped `xFGen` in the bounded data. It does not repair the all-file RAD ratio `1.2771`, because the all-file anomaly instead tracks the unmatched selected multiplicity/domain populations.

## Truth-record semantics

| field | H1 meaning | remnant/diffraction consequence |
|---|---|---|
| `IsGtr()` / `IsGtr!=0` | Particle is from the generator GTR bank, not the GEANT3 STR bank. GTR is frozen before detector simulation. | Excludes GEANT3 material interactions and GEANT decays; does **not** exclude generator-level beam-remnant fragmentation or diffractive-system hadrons. |
| `Mother2==-1` | No second-mother index. A nonnegative `Mother2` flags multi-mother/string-bookkeeping or, for STR, reinteraction topology. | Defense-in-depth against multi-mother entries; it is **not** a beam-remnant, leading-proton, diffraction, or baryon-number veto. |
| `ParticleSource==0` | `kPrimarySignal`: direct generator hard-interaction/string-fragmentation classification. | Expected for generator-level remnant and diffractive fragmentation products as well as current fragmentation. It is not a remnant veto and is not a red flag. |
| `ParticleSource=1,2,3` | Sigma0, Xi, Omega feed-down. | Generator ep signal/feed-down; not remnant classifications. |
| `ParticleSource=4` | GEANT3/material secondary interaction. | Rejected already by `IsGtr`; an additional `ParticleSource!=4` truth-v2 veto is redundant. |
| `ParticleSource=5` | Multi-mother/combinatorial classification. | Already removed by `Mother2==-1`. |

Therefore, **neither `Mother2==-1` nor PS=0 establishes current-fragmentation origin**. The hemisphere definition is the physics discriminator. A valid `xFGen<0` is necessary, but the present bounded corrected-xF test shows that the observed selected h+ excess is not due to ISR-induced sign migration of selected protons.

## DJANGOH RAD versus NONRAD physics

DJANGOH14 RAD includes HERACLES QED ISR/FSR and changes the sampled leptonic phase space and event weights; it does not merely attach a photon to an otherwise fixed hadronic final state. The `RAD.Q2GT60` and `NORAD.Q2GT100` campaigns are explicitly different generated domains. A low-generated-Q2 radiative event can migrate into an analysis selection defined with reconstructed/e-method kinematics after ISR, and its complete hadronic final state includes the proton remnant/diffractive system. Thus the answer to the migration question is **YES** in principle.

However, that statement is not evidence that the selected h+ anomaly is literal remnant leakage: the selected corrected-xF proton sign-flip fraction is zero in the bounded test. The verified observation is a charge-dependent selected-hadron multiplicity mismatch between unmatched RAD and NONRAD domains, not a demonstrated remnant tag.

## Baryon/charge interpretation

A proton-rich target-side population is physically capable of generating a positive charge excess because the ep hadronic final state has baryon number +1. The observed pattern (h+ proton dominance in pathological RAD tails, and low-z C_rad failure) is qualitatively compatible with target-side sensitivity. It is not quantitatively established as target contamination in the selected 0--8 GeV truth-v2 result: the low-z RAD h+/h- proxy is 1.1826, whereas the largest ratios occur at `z=0.05--0.10` (1.8008), `pT_h=2--4` (1.9106), and the separate `pT_h>=8` tail. The all-file multiplicity ratio-of-ratios provides the decisive quantitative explanation of the integrated charge difference.

## Ranked remedies and predicted effects

| rank | remedy | h+/h- effect | C_rad effect | status |
|---:|---|---|---|---|
| 1 | Build/use a matched DJANGOH RAD/NONRAD generated-domain pair; do not form C_rad from `Q2GT60` RAD versus `Q2GT100` NONRAD `SIGMT`/exposure domains. Pin identical truth definitions and compare per-event selected multiplicities. | Must remove the observed 1.2569 multiplicity ratio-of-ratios; target is controlled near-unity charge consistency, not a forced h+/h-=1. | Required before a meaningful correction; current 0.6770/0.8510 candidate remains invalid. | **Required blocker closure.** |
| 2 | Resolve OLD/NEW NONRAD provenance and source-pool-matched bin-local compatibility, then rerun a controlled v1/v2 selection closure. | Quantifies and removes numerator-pool charge bias; prediction unavailable until matching is demonstrated. | Required; exposure and selection controls must precede any C_rad reconstruction. | **Required blocker closure.** |
| 3 | Fix builder q consistently (`kin=(0,0,-eeff,eeff)`) for zGen, pT_h, phi_h, xFGen; rebuild and validate separately. | Bounded selected-bin estimate: negligible charge-ratio change (h+ and h- retained factors 0.999611 and 0.999609). | Bounded estimate: about +0.039% integrated; removes invalid z>1/pT tail pathology but cannot repair charge split. | **Required software-quality fix, not causal cure.** |
| 4 | Keep `IsGtr && Mother2==-1`; retain xF current-hemisphere cut after corrected-q rebuild. Do not add a `ParticleSource!=4` veto. | No predicted change from PS=4 veto; selected GTR already excludes it. | No material C_rad effect. | **Do not use as a remedy.** |
| 5 | Do not add a generic PS=0/PS<=1 or explicit remnant-baryon veto to the inclusive charged-hadron signal. Use a documented, independently validated physical hemisphere/diffraction definition only if the measurement definition changes. | A proton/remnant veto would lower h+/h- by construction but would be model-dependent and redefine the inclusive observable. | Would arbitrarily raise h+ C_rad where RAD protons are removed; not an approved correction. | **Not recommended.** |
| 6 | Restrict z only as a labelled diagnostic, not as a repair. | Removing low-z may reduce sensitivity to target-side baryons, but the largest verified RAD ratios are not confined to z<0.05. | May conceal rather than cure anomalous bins; cannot validate integrated C_rad. | **Not recommended as repair.** |

## Required next checks

1. Produce a matched RAD/NONRAD campaign/domain or demonstrate an event-level reweighting that makes the generated phase spaces identical before applying the common truth selection.
2. Run the full Condor-only OLD/NEW source-pool compatibility/exposure reconciliation required by `G-CRAD-NONRAD-EXPOSURE` and `G-CRAD-OLDNEW-COMPAT`.
3. Implement the q consistency correction only in an isolated new builder version, then validate all retained rows and selected cutflows; do not overwrite v2.
4. If a remnant/diffractive attribution remains required, add generator-record observables/ancestry that actually label remnant/diffractive systems. `Mother2`, GTR/STR, and `ParticleSource` do not provide that label.

## Evidence and artifacts

- Builder defect: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v2.C:200-220`.
- Truth-v2 selection/exposure implementation: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/crad_truth_v2_20260923/produce_crad_truth_v2.py:20-28,93-108,184-204`.
- Integrated charge/multiplicity diagnosis: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/crad_normalization_forensics_2026-09-25.md:39-85`.
- Tail diagnosis: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/research_rad_pth_tail_rootcause_2026-09-25.md:5-23`.
- Truth semantics: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/memo-mc-truth-policy-charged-sidis.md:21-65,69-123,127-160,166-249,257-303`.
- Bounded xF recalculation table: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rad_hplus_proton_excess_20260925/bounded_xf_recompute.tsv`.
