# Electron Parentage HAT+MODS Clean-Run Handoff (2026-07-31)

## Purpose

**NOT_TREEBUILDER diagnostic.** This HAT+MODS/no-ODS study is for integrated yields and source fractions of truth-matched reconstructed `e±` in selected high-Q2 MC, reported separately by generator and data-taking period. It is not central production, a central tree, a response/unfolding input, a background estimate, or a final physics result.

## Exact selected denominator

For each covered generator-period subset, the source-fraction denominator is the nominal selected Tier-0 truth-matched electron population:

> all selected entries with `abs(PDG) == 11` within the covered valid files.

Pre-cleanup counts are QA only. Do not scale observed counts to the 865 planned units or infer the unavailable contribution.

## Native v6 event-list contract

The diagnostic samples the native selected event stream from `H1Tree::Next()` in the exact v6 per-chunk `sidistree` steer. It uses only the embedded `fEventListFiles`.

- The positional `EvlTree` contract is one entry per `H1Tree::Next()` loop iteration.
- Abort on EvlTree exhaustion.
- Event-loop and EvlTree counts must match.
- Do **not** join by run/event identifiers.

## Schema/output contract

- Schema: `electron_parentage_NOT_TREEBUILDER_v3`
- Tree: `ElectronParentageDiag`
- Branch count: 87
- Accepted outputs have valid schema, source metadata, and counters, and are unique matches to manifest units.

## Allowed parentage fields/categories

The diagnostic records matched PDG/status, GTR/STR and `StrIdx`, mothers, three ancestor levels, sibling and opposite-sign-sibling topology, reconstructed `P`, `Pt`, `Eta`, `Phi`, `radlength`, `DcaPrime`, `NHitDedx`, electron MLP, and nominal flags.

There is no `H1StrBank`; SVX/STR fields use `-999` where unavailable. Permitted source categories must be constructed only from these recorded parentage/topology fields and must retain their operational definitions in any aggregation report.

## Coverage/campaign status

Clean authoritative cluster: **4655647**. Of 865 terminated units, 674 have valid outputs; 191 are missing because PNFS MODS were unavailable.

| Generator | Period | Planned | Valid | Missing |
|---|---:|---:|---:|---:|
| DJANGOH14 | ep0304 | 71 | 61 | 10 |
| DJANGOH14 | em0405 | 141 | 117 | 24 |
| DJANGOH14 | em06 | 88 | 43 | 45 |
| DJANGOH14 | ep0607 | 201 | 164 | 37 |
| RAPGAP31 | ep0304 | 61 | 59 | 2 |
| RAPGAP31 | em0405 | 121 | 92 | 29 |
| RAPGAP31 | em06 | 61 | 56 | 5 |
| RAPGAP31 | ep0607 | 121 | 82 | 39 |
| **Total** | **all** | **865** | **674** | **191** |

No integrated yield or source-fraction aggregation has yet been performed.

## QA facts

- The 674 accepted outputs passed schema, source-metadata, and counter checks.
- Output uniqueness against manifest units was accepted.
- The central production area was untouched.
- This is explicitly a HAT+MODS/no-ODS diagnostic; coverage is limited to the valid output subset above.

## Exact remote locations

| Item | Location |
|---|---|
| Implementation root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/diagnostics/electron_parentage_study_NOT_TREEBUILDER/` |
| Implementation README, source, launcher | In the implementation root; use the README and the source/launcher names recorded there |
| Diagnostic binary | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/bin/x86_64-el9-gcc14-opt/targeted_electron_parentage_diagnostic_NOT_TREEBUILDER` |
| Authoritative clean outputs | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/diagnostics/electron_parentage_study_NOT_TREEBUILDER/outputs/full_v6_4period_cleanrun_20260731/` |
| Clean campaign root | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/condor_v6/central/qa/electron_parentage_hATmods_full4p_cleanrun_20260731/` |
| Campaign artifacts | `manifest.tsv`, `manifest_validation.txt`, `campaign.args`, `electron_parentage_full4p_cleanrun_20260731_v2.sub` in the clean campaign root |

## Future-session recommended aggregation work

1. Aggregate exactly the 674 v3 accepted outputs, retaining generator and period as separate report groups.
2. For each group, report coverage, selected denominator count, category counts, fractions, and statistical uncertainties.
3. Include a coverage-sensitivity statement, but do not infer values for missing files.
4. Combine groups only under a documented weighting scheme; do not mix generators or periods before their separate reports exist.

## Hard interpretive restrictions

- Do not label any category as material conversion, conversion rate, or detector material.
- `H1PartMC` ancestry, GTR/STR information, `radlength`, and sibling topology are insufficient for those claims; no production-vertex or process information is available.
- Do not scale counts to all 865 planned units.
- Do not mix generators or periods before separate reports.
- Do not promote this diagnostic to central trees, responses, unfolding, background estimates, or final results.

## Superseded campaign history

The older `electron_parentage_hATmods_full4p_20260730` campaigns are superseded operational history. Never combine their outputs with the authoritative clean-run outputs.
