# Root-cause investigation — RAD photon-axis pT_h tail

**Status:** read-only one-file-per-sample diagnosis; no builder, production, C_rad application, or central product changed.

## Decisive ranked conclusion

1. **Verified — the pathological rows are outside the ISR-safe truth-event phase space, and a near-lightlike nominal-beam boost amplifies their hadron momentum.** In RAD ep `rad_ep_0642.root`, weighted pT_h>=8 rows have median Y_es_gen=0.00446312; 99.16% weighted have Y_es_gen<=0.1, 74.02% have Q2_es_gen<150, 20.39% have invalid x_es_gen=-999, and only 0.389% pass `pass_truth_event_fiducial`. W boost median |beta|=0.99969465 (p99=0.999953), versus 0.985405 for 0<=pT_h<4. This is the direct condition that makes finite lab momentum acquire extreme CM transverse component. Evidence: supporting table and source lines 60-74, 197-220.
2. **Verified — the tail is proton-dominated, explaining charge asymmetry without a charge-dependent boost.** h+ takes 81.93% of weighted tail rows and is 79.42% PDG 2212; h- takes 18.07% and is 97.43% pi-. The representative-file tail h+/h- ratio is 4.53, consistent with the all-sample factor ~3.2. ParticleSource=0 is essentially 100% for either charge: not a source-4 material-secondary signature.
3. **Verified — zGen is corrupted, so defect is not isolated to the boost.** 75.8448% weighted tail rows have zGen>1; only 24.1552% have 0<=zGen<=1. zGen=P.h/P.q uses the same nominal-beam q but no CM boost, so the source event is inconsistent with the nominal incoming lepton. The ISR-effective-beam hypothesis is revived.

## GenEnPhoton discriminator

**No; it is not a valid discriminator for the retained RAD ntuple population.** RAD ep has 61.7721% unweighted / **52.7450% weighted exactly zero**, and 74.5643% / 71.0643% below 0.5 GeV; weighted median=0 and p99=26.616425 GeV. OLD and NEW NONRAD are exactly zero in all rows and weight. For RAD GenEnPhoton==0, |Q2sGen-Q2_es_gen| median/p99=0.00174/0.07666 GeV2 and |YsGen-Y_es_gen| median/p99=8.69e-06/2.56e-04: they are kinematically nonradiative under the stored field. Thus it cannot rule out the nominal-beam construction problem because the tail is dominated by rows outside valid e-method phase space. No GenRad branch exists in the 40-branch ChargedGenRC inventory.

## Required builder remedy and validation

**Recommended change (requires h1-builder-engineer; no edit made):** at `RC_treebuilder_genonly_v2.C` line 209, form the incoming lepton used for q, cmPhotonAxisPt, invariantZ, cmTrentoPhi, and cmXF from eeff rather than nominal GenBeamElecE:

    const TLorentzVector kin(0,0,-eeff,eeff);

Retain eeff>0.5 guard. Also evaluate C_rad only with `pass_truth_hadron_fiducial==1` (or its exact cuts); the tree writes all charged GTR rows, including `pass_truth_event_fiducial==0` rows (source lines 212-220).

**Decisive validation:** isolated v3 one-file RAD and matched NONRAD rebuild, compare only `pass_truth_hadron_fiducial==1`: (i) zGen>1 must be zero by fill-time definition; (ii) pT_h>=8 must return to nonrad O(3e-4), or be physically demonstrated; (iii) W.M2>0, |beta_W|<1, 150<=Q2_es_gen<20000, and 0.1<Y_es_gen<0.7 mandatory. A raw-row control must show that the eeff four-vector removes zGen>1 and tail amplification. Do not use retained v2 RAD C_rad until this and source-pool selection closure are reviewed.

## Sources, limits, and handoff

- Code: `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v2.C:60-80,197-220`.
- Existing anomaly: `data/radiative/crad_branch_audit_20260925/audit.json:496-740`.
- RAD provenance: `docs/radiative/SESSION_HANDOFF_crad_resolution_2026-09-21.md:16-22`; components 8955-8962. HAT all-zero semantics remain unresolved without component generator/HAT records.
- OLD/NEW comparison is one e+ file per group only; supports their all-zero expectation but no multi-file statistical claim.
- Route v3 implementation/smoke to h1-builder-engineer; component HAT/GenRad audit to h1-data-validator or h1-root-analyst; provenance first to h1-mc-db then semantics to h1-mc-expert; C_rad selection decision to h1-radiative-correction.
