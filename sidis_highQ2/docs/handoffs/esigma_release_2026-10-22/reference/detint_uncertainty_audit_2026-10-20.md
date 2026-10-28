# det.int. background: is an uncertainty assigned, or is it blindly removed?

Audit date: 2026-10-20. Read-only audit of the NAF tree; nothing on NAF was modified.
Analysis: H1 high-Q2 (Q2 > 150 GeV^2) charged-hadron SIDIS,
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2` (docs/state) and
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6` (products).

## Short answer

1. **det.int. is not externally subtracted.** External det.int. pre-subtraction is forbidden by
   design. Since the S2' promotion (2026-10-10), the "removable" det.int. class is removed
   **response-internally**: those matched legs are rerouted to the response fake column, which
   enlarges the per-reco-bin fake fraction `f_i`. The miss row is not filled. The mechanism
   itself is response-consistent and exactly-once.
2. **The size of that removal carries no normalization (rate) uncertainty.** The amount
   removed is 100% MC-predicted (RAPGAP31 response, H1SIM/GEANT3). No budget term varies the
   det.int. rate the way `bgsub_env100_rel` varies e+V0 by ±100%. The det.int.-related terms
   that do exist cover only generator spread and MC/classification statistics (table below).
   **In uncertainty terms, det.int. is effectively blindly removed.**
3. **Size of the gap:** about 0.7% of the reco yield integrated, a per-bin median of about
   0.3%, and up to about 2.6% in the worst bins. Added in quadrature at the median, it is
   small (~+0.01 pp). It does matter in the 1–2.6% bins and as a coherent normalization of
   about ±0.7% at ±100%.

## What "det.int." means here

From the `sidis-hq2-backgrounds` skill:
`IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && IsMatchedElectron==0 && IsMatchedV0Daughter==0`.
That is, GEANT/STR-only charged π/K/p secondaries. It is **not** pure material interaction.

Parentage record `docs/backgrounds/detint_parentage_results_2026-10-09.md`:

| class | share of det.int. block | S2' treatment |
|---|---:|---|
| C4+C5 interaction-born (hadronic interaction in material) | 34.47% | removed (to fake column) |
| C1+C2 hyperon decay feed-down (Σ± 86.8%, Ξ 12.6%, Ω 0.57%; zero Λ) | 56.53% | removed (to fake column) |
| C3 charged-K decay-in-flight "k_parented keeper" (mother ±321 only) | 9.01% | kept as signal |

Removable fraction: `0.909920 ± 0.000269` pooled (DJ14 0.912611, RG31 0.906412).
Spec: `docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md` (Rev 2 Sec 0, user-decided 2026-10-09).

## Current quoted budget and the det.int.-touching terms

Quoted budget (since 2026-10-19):
`H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_bgsub_env100_21comp_2026-10-19/`,
quoted total `total_with_env100_rel`. Medians over 245 bins per charge, read from
`bgsub_env100_21comp_budget.csv`:

| component | definition | h+ median / p95 / max | h− median / p95 / max | probes det.int. **rate**? |
|---|---|---|---|---|
| `bgsub_env100_rel` | flat ±100% on `(e_avg + v0_avg)/data_sub'`, fully correlated | 3.16 / 6.93 / 28.4 % | 3.56 / 7.08 / 29.9 % | **No**: e + V0 only |
| `detint_removal_model_rel` | `0.5*abs(inc_DJ - inc_RG)/v`, inc = S2'-variant minus central coherent IBU increment | 0.030 / 0.10 / 0.40 % | 0.029 / 0.11 / 0.56 % | **No**: DJ and RG share H1SIM/GEANT3, material and NucliaWeight, so only primary-spectrum differences enter |
| `detint_removal_stat_rel` | parentage-classification stat σ/R on the increment | ~0.0001 % | ~0.0001 % | No (stat only) |
| `detint_removal_rel` | hypot(model, stat) | 0.030 / 0.10 / 0.40 % | 0.029 / 0.11 / 0.56 % | No |
| `fakecol_mcstat_rel` | MC-stat of the whole `f_i` (fake-column w2 + body w2 through the IBU k=4 Jacobian) | 0.34 / 1.65 / 2.79 % | 0.35 / 1.96 / 3.63 % | No (MC stat of all fake classes) |
| `bgsub_fakefrac_rel` | `abs(1 - r_min) * f_i,standing`, r_min = 0.98167 | 0.18 / 0.35 / 0.44 % | 0.18 / 0.33 / 0.40 % | **No**: explicitly **fc0-only** scope (`f_standing = F_fc0/(B+F)`), so the det.int. fake categories (fc3) are excluded |
| `tracking_eff_m0p5_rel` | 16 seeded data-side 0.5% random track drops | 0.51 / 0.70 / 1.04 % | 0.52 / 0.84 / 1.60 % | No: generic efficiency, not the secondary-production rate |
| `total_with_env100_rel` | quoted total | 7.32 / 29.8 / 73.7 % | 7.64 / 39.2 / 88.4 % | — |

Key observations:
- `bgsub_fakefrac_rel` was originally scoped to cover det.int. In the 2026-10-10 spec
  (`docs/systematics/BGSUB_PUBLISHED_SCHEME_AND_GATE_SPEC_2026-10-10.md`, row 17) it "covers
  the model dependence of removing electrons/other/det.int.-fake (incl. the 86/14 det.int.
  partition model)". It was later **narrowed to fc0-only** ("Rev-2 correction"). After that
  narrowing, no term covers the det.int. fake content.
- The same 2026-10-10 spec (line 45) says of det.int.: *"its data–MC mismatch is an
  uncertainty, not a subtraction"*. That intent never became an implemented budget term.
- The S2' spec (Sec 2.3) states the residual model dependence "rides the existing `model`
  budget component" via the DJANGOH14 closure (G6). A critic caveat in the same spec (G6)
  notes that the DJ leg is invariant under the reroute when the stored plain prior is used.
  The coherent-prior fix addressed that, but either way the DJ–RG spread is not a rate
  variation.
- The integrated removal increment recorded in
  `systematics_bgsub_setRprime_19comp_cleandenom_.../rederivation_records/detint_removal_rel.json`
  is about `-9e-7 %`. Moving det.int. from body to fake column barely moves the unfolded
  result when data look like MC, because the `f_i` growth and the efficiency change cancel.
  So the removal increment is the wrong object to size the uncertainty from. A data/MC rate
  mismatch δ leaves a residual of about δ × (det.int. share of the reco bin) in either
  convention.

## Magnitude of the unpriced det.int. content

Computed 2026-10-20 from the cone S2' RAPGAP31 response
`H1ChargedSidis_HighQ2/output_v6/studies/cone_alpha_central_build_2026-10-11/production/migration_s2prime_cone_rapgap31_2026-10-11.npz`
with `scripts/detint_fc3_share.py` (this folder). Definitions:
- fc3 = `h_fake_detint_rerouted + h_fake_detint_prefail + h_fake_detint_keeper_prefail`
- B = `h_matched_reco_marginal`
- F = fc0 + fc3, with fc0 = `h_fake_existing_other`
- Share s_i = fc3_i / (B_i + F_i), over 490 fine reco bins per charge.

Sanity checks:
- The body row-sum and fake column reproduce B and F to 1e-9.
- The rerouted integrated share, 0.6246% (h+), matches the G9 growth 0.006246 recorded in the budget.

| | h+ | h− |
|---|---:|---:|
| integrated `f_i` | 11.23% | 10.87% |
| integrated det.int. in fake column (fc3) | **0.736%** | **0.653%** |
| … rerouted (S2' removable, gen-passing) | 0.625% | 0.561% |
| … prefail (det.int. failing gen fiducial; already fake pre-S2') | 0.101% | 0.082% |
| … keeper prefail | 0.011% | 0.010% |
| fc3 / F | 6.55% | 6.00% |
| per-bin share: median | 0.345% | 0.302% |
| per-bin share: p95 | 1.39% | 1.31% |
| per-bin share: max | 2.65% (reco bin 350) | 2.62% (reco bin 489) |
| bins with share > 1% / > 2% | 40 / 6 | 40 / 3 |

So the current det.int. terms (median ~0.03%) cover roughly a tenth of the removed det.int.
content per bin. A ±100% envelope analogous to env100 would add about +0.01 pp to the total
medians. It would add ~1–2.6% in the worst bins and a coherent integrated ±0.7%, against
env100's ±7.7%.

Caveats on these numbers:
- They are reco-level, single-generator (RG31), dilution-approximation shares on the
  2026-10-11 response, which predates the OPT-A nominal.
- The real unfolded-level term must be produced by running the chain. The per-bin mapping
  to the 245 unfolded bins will differ through migrations.

## Why a rate uncertainty is physically warranted

- 56.5% of the removable class is charged-hyperon (Σ±, Ξ) weak-decay feed-down. Its rate
  scales with the generator's strangeness/hyperon production in hadronization. DJANGOH and
  RAPGAP both hadronize with JETSET/PYTHIA (verify the shared tune), so DJ–RG spread does
  not probe it.
- 34.5% is interaction-born. It scales with the GEANT3 hadronic interaction model and the
  material description, both common to DJ and RG.
- H1 precedent (arXiv:1302.1321, cited in the S2' spec Sec S1): hadronic-interaction
  secondaries were covered by a **1% per-track tracking-efficiency systematic**. Our
  tracking term is a 0.5% generic drop and is not tied to secondaries.

## Related open items seen during the audit

- **C1 (scriptaudit 2026-10-19, `docs/critiques/scriptaudit_2026-10-19/loop1_systematics.md`):**
  `detint_removal_rel` enters `total_with_env100_rel` **twice**. Its subcomponents and the
  combined column are all listed in `quoted_components`. The effect is ≤ ~171 ppm and
  conservative. The fix is listed as a quoting condition for the OPT-A budget regeneration.
  This is a bookkeeping bug, not a det.int. normalization uncertainty.
- **NucliaWeight convention mismatch, unverified:** the S2' spec says the response uses
  `w1*w2*NucliaWeight` on matched and rerouted legs. `docs/backgrounds/bgsub_fitnorm_campaign_spec_2026-10-19.md`
  (line 67) says "NucliaWeight EXCLUDED from nominal, producer convention". Check which
  convention the installed response actually uses, because NucliaWeight directly affects
  the absorption/secondary-rate modeling.
- **Quoting is currently blocked anyway:** STATE.md, 2026-10-20 OPT-A promotion. The
  bgsub19/env100 budgets and the covariance package must be regenerated on the OPT-A nominal
  `bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_..._2026-10-20/`. That regeneration is the
  natural place to add a det.int. normalization term.

## Recommendation (needs a user decision on κ)

Add a `detint_norm_rel` (or `detint_env_rel`) component:
1. **Variant chain:** scale the det.int. fake-column categories (fc3: rerouted + prefail +
   keeper-prefail) by (1 ± κ) in `f_i`. Leave fc0 and the body unchanged. Re-unfold IBU k=4
   with the coherent OPT-A prior and take the half-difference / nominal per bin.
2. **Decide κ:**
   - κ = 1.0 matches env100's fiat treatment of e/V0 and is conservative but cheap.
   - Alternatively, split by class: ±X% on interaction-born (material/GEANT; H1-style ~1%
     per track would be far smaller in relative terms) and ±Y% on hyperon feed-down
     (strangeness-production uncertainty), with a written justification.
3. **Correlation:** fully correlated across bins and charges, like env100. Add it to the
   covariance package.
4. Fix the C1 double count in the same regeneration.
5. Decide whether the keeper (C3, kept as signal; body share ~0.06%) needs anything. It is
   acceptance-corrected like signal, so probably not.

Until this is done, the honest statement is: *the det.int. removable class is removed
response-internally with the MC-predicted rate; its uncertainty budget covers generator
spread and MC statistics only, not the secondary-production rate.*
