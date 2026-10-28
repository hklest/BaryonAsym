# PROVISIONAL INTERNAL DIAGNOSTIC — alpha0p2 application audit

**Date:** 2026-10-06  
**Scope:** the canonical z005 IBU/BBB release
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`
and the coherent-prior candidate
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release_candidates/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`.

## Conclusion

The **alpha(track,e) < 0.2 rad veto is applied upstream at v6 tree production** through
`gH1Calc->Had()->RejectTracksNearScatElec(0.2)`.  Consequently it applies to the
`ChargedTot` inputs from which both the canonical raw data vector and the central
RAPGAP response/fake column were made.  Neither the canonical data-vector script nor
the current response-rebuild script recomputes alpha from stored kinematics: they rely
on those already vetoed trees.  The release consumes no external alpha0p2 background
vector; that package is diagnostic only.

| Component | Verdict | Evidence |
|---|---|---|
| Data vector (980 reco bins) | **APPLIED (upstream tree level)** | The release driver reads the canonical raw data JSON at `release/_run/run_bbb_ibu_release.py:27-31`; release `metadata.json:25-26` records the same response and data-vector paths. The vector was built by `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_data_vector_v4.py`: its required input is `ChargedTot` (`:262-281`, `:443-462`) and its local selection is at `:367-405`. It has no second alpha calculation. Its upstream `ChargedTot` production invokes the 0.2-rad veto at `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:389-396` (also canonical snapshot `scripts/canonical_highQ2_sidis_2026-06-30/builders/TreeBuilderHighQ2_chargedsidis_v6.C:394`). |
| Signal-MC response body and fake column | **APPLIED (upstream tree level)** | Canonical response source is `unfolding/build_migration_th2_per_charge_v6.py` (`RUNLIST.md:54-64`). It reads `ChargedTot` reco fields (`:263-282`), applies its reco mask at `:560-601`, and fills matched/fake rows from those selected `ChargedTot` tracks (`:784-789`). It has no local alpha recomputation, but the same Stage-1 call above is common to MC tree production. The coherent rebuild likewise takes `ChargedTot` (`build_coherent_prior_response_shard.py:750-845`) and defines `pass_reco_full` at `:776-800`, with no local alpha override; hence it retains the upstream veto. |
| v6 tree level (`ChargedTot`) | **APPLIED** | `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:389-396` configures `RejectTracksNearScatElec(0.2)` before the event loop. Stage 2 explicitly skips the H1OO-tagged scattered-electron track at `src/ChargedSidisTreeBuilderHighQ2_v6.C:1573-1584`; it records `Theta` (`:677-705`) but has no alpha branch or additional numerical alpha cut. Thus the cone veto is configured in Stage 1/H1OO selection rather than stored as an alpha branch or re-applied in Stage 2. |
| External fake/background vectors | **NOT-APPLICABLE to either release driver** | Both release READMEs say `raw/no-bgsub` (canonical `README.md:1-7`; candidate `README.md:1-8`), and the canonical driver loads only the canonical response ROOT and raw data JSON (`release/_run/run_bbb_ibu_release.py:25-31`). The alpha0p2 background products are explicitly diagnostic and not central inputs (`docs/status/alpha0p2-background.md:6-10`; canonical `RUNLIST.md:96-110`). Their convention itself is alpha0p2: the handoff defines the `alpha<0.2` study (`docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:7-16`) and the copied alpha builder applies `alpha >= 0.2` at `output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py:390-397,681-698`. |

## Candidate inheritance check

The coherent-prior candidate is not a new data/fake selection.  Its `README.md:6-8`
states that only RAPGAP prior consumption changed to `body+miss`, while the data vector,
fake handling, `k=4`, luminosity and binning remain canonical.  Its `metadata.json:32-33`
points to the exact same canonical response ROOT and data-vector JSON as the canonical
release.  Therefore the alpha conclusion above transfers unchanged to the candidate.

## Consistency and P-ALPHA

There is **no data-versus-response alpha-cut mismatch found in this audit**: both derive
from v6 `ChargedTot` after the common Stage-1 0.2-rad H1OO rejection.  There is also no
external fake/background-vector convention mixed into either raw/no-bgsub release.

This does **not** close `P-ALPHA`.  `STATE.md:34-36` says central processing is
raw/no-bgsub and only uses response fake-fraction subtraction, and `STATE.md:66` lists
“Alpha-veto/background response-consistency validation (P-ALPHA)” as open.  The
alpha0p2 handoff specifies the remaining issue: non-V0 electrons are fake-column
handled, V0 daughters are excluded, and det.int. is approximately 86% matched / 14%
fake, so an external subtraction or all-fake treatment must be applied exactly once and
validated with closure/covariance (`docs/unfolding/alpha0p2_reco_background_allfake_handoff_2026-06-28.md:72-86,123-147`).
