# Research: High-Q² Charged-SIDIS C_rad Normalization/Provenance Resolution

**Date:** 2026-06-13  
**Artifact:** `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/research/crad_resolution_highQ2_2026-06-13.md`  
**Agent:** generic `researcher`  
**Scope:** Evidence-backed diagnosis of the remaining high-Q² charged-SIDIS radiative-correction (`C_rad`) normalization/provenance issue and a concrete specialist handoff plan. This research covers local SIDIS high-Q² context/docs/scripts/data, bounded read-only NAF inspections of current CRAD scripts/artifacts/source/steering, and the least-risk path to a reliable high-Q² `C_rad` for phase space `Q2>150`, `0.1<y<0.7`, current hemisphere `xFGen<0`, `GenEmpz>45`, charged π/K/p only.

**Remote-work compliance:** Verified SSH control socket once before remote work: `ssh -O check naf-hone.desy.de 2>&1` returned `Master running (pid=74614)`. All NAF actions in this research were bounded read-only file/list/script inspections. No Condor, event-loop production, or edits were run.

**Supersession note (2026-06-20):** The original conclusions in this 2026-06-13 research artifact that treated multiplicity-ratio `C_rad` as the only sanctioned definition and cross-section-ratio `C_rad` as invalid are superseded. For current SIDIS radiative-correction work, `c_rad` / `C_rad` is the ratio of cross sections, not a multiplicity ratio: `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)` unless an analysis-specific convention document later explicitly defines a different orientation. Retain diagnostic/private RAPGAP versus final official correction provenance distinctions, but do not reject cross-section-ratio `C_rad` solely because it is a cross-section ratio.

---

> **HARD HISTORICAL WARNING (2026-06-25):** The recommendation below is superseded and must not be followed for current high-Q² z005 `C_rad`. The current convention is DJANGOH14 gen-only `C_rad(bin) = σ_nonrad(bin) / σ_rad(bin)` with `σ ∝ Σ(weight1*weight2)/L_MC`; see `docs/RUN_STATUS.md` for the maintained status, paths, outputs, RAPGAP quarantine, sparse-bin caveat, and alpha-veto guard. This document remains an audit artifact for the earlier diagnostic reasoning only.

## Original recommendation — SUPERSEDED, do not follow for current z005

**Do not try to rescue the current v6 RAD vs full_v1 NonRad cross-section-ratio products as the analysis `C_rad`.** The decisive, least-risk route is a **small paired RAD/NORAD diagnostic using `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C`**, followed by a mini-production only after the one-file diagnostic passes. The first diagnostic should compute the SIDIS **multiplicity convention**

```text
C_rad^M(bin) = M_norad(bin) / M_rad(bin)
             = [N_h^norad(bin) / N_DIS^norad(Q2,x)]
               / [N_h^rad(bin) / N_DIS^rad(Q2,x)]
```

with identical generator-level event selection (`Q2_es_gen > 150`, `0.1 < Y_es_gen < 0.7`, `GenEmpz > 45`) and hadron selection (`abs(PDG) in {211,321,2212}`, `IsGtr`, `Mother2 == -1`, `xFGen < 0`, finite `zGen`, current pT_h convention). **Primary smoke should not depend on MC luminosity at all**; luminosity is only needed for an explicitly marked cross-section-ratio audit, not for the sanctioned v6 `C_rad`.

Rationale:

1. **The current authoritative v6 policy explicitly sanctions only multiplicity-ratio `C_rad` and forbids cross-section-ratio `C_rad`.** Local sources: `sidis_highQ2/context-handoff.md:85-89` and `sidis_highQ2/docs/RUN_STATUS.md:106-113`.
2. **The current cross-section-ratio script computes a different observable and is numerically pathological by construction for SIDIS.** Remote script `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/crad/compute_crad_user_xsec_ratio_full.py` defines `sigma=sum(weight1*weight2/fLumi_label)` and uses `C_rad=sigma_norad/sigma_rad` without any DIS denominator (`lines 390-393`), while its log reports `ANOMALOUS_RANGE: 42` and `LOW_STAT: 106` over 386 processed files (`condor_v6/crad/full_v1/compute_user_xsec_ratio_full/logs/compute.3647315.0.out:39-49`). Local metadata for the related xsec-ratio study shows all bins anomalous and medians near 4.2 (`data/crad_cross_section_v2_2026-06-04/metadata.json:85-141`).
3. **The current full_v1 `ChargedTotGen`-only products are not sufficient to prove the SIDIS multiplicity denominator.** The cradDiag fork was created specifically to add `sidisGenEventDiagTree`, one row per EVLS-selected MC event, exposing zero-current-hadron events and `N_DIS`-grade counters (`ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:484-489`, `930-950`, `1448-1489`). This is exactly the missing decisive denominator check.
4. **Luminosity/provenance ambiguity is real but secondary for the sanctioned multiplicity route.** g1j_ES `fLumi` values exist for many RAD/NORAD steering blocks, but the raw Britzger RAD file `NC.EMINUSP.RAD.C0000000` lacks an already-verified exact per-file `fLumi` mapping in the current handoff. A multiplicity smoke avoids that unresolved input. Let `h1-mc-db` close exact RAD raw-file luminosity only if a cross-section audit remains desired.

---

## Evidence table

| Claim | Status | Evidence | Implication |
|---|---:|---|---|
| v6 policy: use only `M_norad/M_rad`; do not use xsec-ratio `C_rad`. | **Verified** | `sidis_highQ2/context-handoff.md:85-89`: “Valid: `C_rad` as a multiplicity ratio only — `M_norad / M_rad`… Invalid / do not use: cross-section-ratio”; same in `docs/RUN_STATUS.md:106-113`. | The current xsec-ratio products cannot become central without overturning current policy. |
| Existing v6 central production paths are `condor_v6/central/` and `output_v6/central/`; v5g/pre-v6 products are audit only. | **Verified** | `docs/RUN_STATUS.md:10-25`, `context-handoff.md:5-20`. | Any new central `C_rad` must be v6-compatible and must not silently consume v5g/rev6 historical products as central. |
| v6 Stage 2 keeps gen-only MC events and `ChargedTotGen` is filled for all MC events with valid gen electron. | **Verified** | `context-handoff.md:47-53`; remote source `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:24-27`, `1551-1555`. | A correct multiplicity denominator is feasible if event-level rows are retained. |
| `cradDiag` adds a per-event diagnostic tree specifically for DIS denominators and zero-hadron events. | **Verified** | Remote source comments/branches: `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:484-489`, `930-950`; event-phase definition at `1448-1489`; charged-hadron counters at `1502-1539`. | `cradDiag` is the right one-file smoke target; current `ChargedTotGen` alone cannot audit zero-hadron DIS denominator. |
| `cradDiag` event phase is close to the user target but currently uses `Q2_es_gen`, `Y_es_gen`, `GenEmpz>45`, no upper `GenEmpz`, and no global z/pT/eta cut. | **Verified** | Remote source `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:1451-1456`, `1486-1489`. | Matches the requested event phase-space better than older scripts; pT/z are axis selections, not event cuts. |
| `ChargedTotGen` writes the needed branches for C_rad: `GenPDG`, `GenCharge`, `GenPt/Eta/Phi`, gen electron 4-vector, `Q2_es_gen/Y_es_gen/x_es_gen`, `GenEmpz`, `xFGen`, `zGen`, `weight1/2`, `GenVtxZ`. | **Verified** | Remote source branch definitions `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:847-928`. | One-file diagnostic can compute hadron numerator and pT_h in the analysis frame. |
| Current full_v1 production completed structurally. | **Verified** | Local `data/crad/full_v1/diagnostics/crad_full_v1_production_completeness_audit.json` shows no missing/zero evls/tree defects for listed labels; e.g. `DjNorad_Eminus0405_1` has one evls and one sidistree, both nonzero (`lines 14-29`). | Completeness is not the problem; formula/provenance/denominator are. |
| Current full_v1 g1j lumi coverage audit has parser/provenance red flags. | **Verified, but interpretation unresolved** | Local `data/crad/full_v1/diagnostics/crad_full_v1_g1j_lumi_coverage_audit.json`: `defect_totals.missing_steering_block = 386`, `missing_fLumi = 0`; `queue_summary` lists GenLumi and nonrad steering basenames; `steering_summary.n_labels_indexed = 0`. | Some luminosity values were present, but the local coverage audit did not successfully index steering labels; downstream fLumi use needs h1-mc-db/h1-normalization review. |
| Current xsec-ratio full script uses per-label `source_lumi` and no DIS denominator. | **Verified** | Remote `scripts/crad/compute_crad_user_xsec_ratio_full.py:203-218`, `242-267`, `291-321`, `390-393`. | It computes a hadron cross-section ratio, not the sanctioned SIDIS multiplicity correction. |
| Current xsec-ratio run completed but flagged many anomalies. | **Verified** | Remote log `condor_v6/crad/full_v1/compute_user_xsec_ratio_full/logs/compute.3647315.0.out:39-49`: 386 files processed, `ANOMALOUS_RANGE=42`, `LOW_STAT=106`, outputs under `data/crad/full_v1/`. | This is a diagnostic artifact, not a central correction. |
| Local xsec-ratio v2 already demonstrates pathological normalization. | **Verified** | Local `data/crad_cross_section_v2_2026-06-04/metadata.json:85-141`: all finite bins in all views are outliers; finalbin median `4.217`, weighted mean `4.220`. | The “values around 4” are reproducible from the xsec-ratio algebra, not a one-off manifest bug. |
| Local multiplicity-ratio v3 gives plausible O(1) values and explicitly cancels luminosity. | **Verified** | `scripts/compute_crad_v3_HQv5gBprime.py:7-14`, `558-572`, `673-684`, `735-740`; output sample `data/crad_v3_HQv5gBprime_2026-05-24.json:204-246` shows S0a h+ values ~0.93–1.40 depending z/pT. | The known-good convention is multiplicity ratio, not xsec ratio. |
| Local adaptive multiplicity script deliberately did not apply `GenEmpz` because old CSVs lacked the branch. | **Verified** | `scripts/analysis/compute_crad_adaptive_v2.py:42-49`, `864-871`. | Old CSV-based results are not the final answer for the user’s `GenEmpz>45` target; cradDiag or v6 trees with `GenEmpz` are needed. |
| Python/uproot on raw Britzger HAT/MODS lacks `ChargedTotGen` by design. | **Verified** | Local `scripts/radiative_corrections/britzger_paired_crad_2026-06-13/compute_britzger_paired_crad.py:31-34`, `116-163`, `300-325` returns `INSUFFICIENT_SCHEMA` if no `ChargedTotGen`. | Raw HAT/MODS require H1OO/C++ builder access; Python-only is insufficient. |
| Britzger raw paired candidate files exist. | **Verified remote** | `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_RAD/` contains `hat.NC.EMINUSP.RAD.C0000000.root`; `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/` contains `hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.root`. | Exact one-file diagnostic inputs are available on DUST. |
| Britzger fpack steerings identify raw generator names but not enough luminosity for RAD by themselves. | **Verified remote** | `fpack.NC.EMINUSP.RAD.C0000000.steer:3-5` points to `/pnfs/.../generator_files/djangoh14/NC.EMINUSP.RAD.C0000000`; NORAD fpack points to `DJANGOH14.NC.EMINUSP.NORAD.Q2GT100...C0000000`. | fpack files are input provenance, not an authoritative C_rad luminosity table. |
| g1j_ES GenLumi steerings provide NORAD chunk fLumi and MC13 RAD fLumi examples, but raw Britzger RAD C0000000 exact fLumi remains unresolved. | **Verified/inferred** | Remote `es.Norad.em06disMC13.Norad2021.GenLumi.steer:497-500` gives `DjNorad_Eminus06_1`, `fLumi=12841.59`; `:95-99` gives MC13 `Django_Eminus06_101`, `fLumi=5014.37`. Remote `es.em06disMC13.steer:157-160` gives older `Django_Eminus06_101`, `fLumi=6499.58`. | Do not guess a RAD raw-file lumi for xsec ratio; first smoke should avoid lumi or have h1-mc-db provide the exact mapping. |
| The user’s single-file NonRad/RAD selected sumw mismatch is consistent with missing DIS denominator and/or fLumi unit/scope ambiguity. | **Inferred from verified code + user-provided numbers** | User: NonRad selected sumw `1,099,848.625`, RAD `410,181.0`; possible ratios `0.0045`, `4.51`, `0.320`, `3.20`. Remote xsec script applies `scale=1/lumi` per label and forms `sigma_norad/sigma_rad`; local xsec metadata shows medians ~4.2. | The large ratios are not a physics result; they are a convention/provenance symptom. |

---

## Ranked hypotheses for nonsensical ratios

### H1 — Wrong observable: hadron cross-section ratio used where SIDIS multiplicity ratio is required

**Rank:** 1 / strongest  
**Status:** Verified mechanism; sufficient to explain order-unity-to-multi-unit pathologies.

Evidence:

- Current v6 policy says only `M_norad/M_rad` is valid (`context-handoff.md:85-89`, `RUN_STATUS.md:106-113`).
- Remote full script labels itself “user-directed `C_rad = sigma_norad/sigma_rad`” and computes `sigma=sum(weight1*weight2/fLumi_label)` (`compute_crad_user_xsec_ratio_full.py:2-7`, `390-393`). It never divides by `N_DIS`.
- Local xsec-ratio metadata reports finalbin median `4.217` and all bins outlier (`data/crad_cross_section_v2_2026-06-04/metadata.json:100-112`), whereas multiplicity-ratio code explicitly cancels MC luminosity (`compute_crad_v3_HQv5gBprime.py:7-14`, `735-740`) and yields plausible O(1) values.

Mechanism:

For SIDIS, the radiative effect on `N_h` is dominated by the DIS event rate and event selection. A hadron-yield-per-file-lumi ratio includes large RAD/NORAD differences in generated/selected DIS event counts. The multiplicity ratio divides each side by its own DIS denominator, canceling the event-rate component and leaving the per-DIS hadron-yield correction expected near O(1) (often ~1.05–1.1 in useful convention, bin-dependent).

### H2 — Missing per-event DIS denominator, especially zero-current-hadron events

**Rank:** 2 / strongly supported  
**Status:** Verified architectural gap in current approach; cradDiag directly targets it.

Evidence:

- `cradDiag` adds `sidisGenEventDiagTree` because current `ChargedTotGen` rows alone cannot represent events with zero generated current-hemisphere charged hadrons (`ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:484-489`).
- The event diagnostic tree stores `weight1/2`, event kinematics, `GenEmpz`, pass flags, and charged-hadron counters (`:930-950`, `:1448-1489`).
- Existing scripts that derive ratios from `ChargedTotGen` alone can only count hadrons, not all DIS events in the selected phase-space.

Mechanism:

Multiplicity needs `N_DIS^rad` and `N_DIS^norad` per `(Q2,x)` (or chosen denominator bin), including events with no selected hadron in the numerator bin. A `ChargedTotGen`-only calculation undercounts `N_DIS` differently for RAD and NORAD if event/hadron selection efficiencies differ, biasing `C_rad`.

### H3 — Luminosity unit/scope mismatch (`pb^-1` vs `nb^-1`, per-file vs per-chunk vs full-period)

**Rank:** 3 / real for xsec audit; not needed for primary multiplicity smoke  
**Status:** Verified ambiguity; exact RAD raw-file mapping unresolved.

Evidence:

- g1j_ES `H1SteerTree` blocks carry `fLumi` (remote examples: `es.Norad.em06disMC13.Norad2021.GenLumi.steer:95-99`, `497-500`). Earlier research documents `fLumi` as file/chain luminosity and notes native H1 convention issues (`research-crad-lumi-pilot-bunch-zvertex-2026-06-09.md:45-70`, `114-152`).
- The local full_v1 coverage audit reports `missing_fLumi=0` but `missing_steering_block=386` and `n_labels_indexed=0`, indicating the automated mapping still needs review (`data/crad/full_v1/diagnostics/crad_full_v1_g1j_lumi_coverage_audit.json`, compact fields inspected locally).
- The user’s single-file interpretations differ by factors of 10 and 1000 (`0.0045`, `4.51`, `0.320`, `3.20`), exactly the signature of unit/scope confusion.

Mechanism:

Cross-section ratios require a consistent luminosity convention for each sample. g1j_ES `fLumi` is per `H1SteerTree` chain; scripts may label values as `source_lumi` without resolving native units and chunk width. A factor-1000 pb↔nb error or chunk/file share error directly rescales `sigma_norad/sigma_rad`. **This is why the first decisive smoke should use multiplicities and avoid luminosity.**

### H4 — RAD and NonRad source/phase-space mismatch (`Q2GT60` vs `Q2GT100`, MC13 vs raw Britzger, 27.5/27.6 GeV)

**Rank:** 4 / important for final systematic and xsec audit  
**Status:** Verified structural mismatch; magnitude inferred.

Evidence:

- Local availability reference documents NORAD `Q2GT100` and RAD `Q2GT60` (`docs/references/reference-highQ2-crad-availability.md:142-159`, `203-215`).
- Remote g1j_ES RAD examples show `RAD.Q2GT60`; NORAD examples show `NORAD.Q2GT100` (`es.Norad.em06disMC13.Norad2021.GenLumi.steer:95-99`, `497-500`).
- `research-crad-lumi-pilot-bunch-zvertex-2026-06-09.md:72-91`, `445-463` identifies the Q²gen lower-edge mismatch as a dominant structural source of generator-dependent offsets in xsec-ratio formulations.

Mechanism:

For `Q2>150`, RAD samples include generated events down to 60 that can migrate into the selected region; NORAD starts at 100. For a pure multiplicity in selected generator phase-space this is less dangerous, but for absolute yield/lumi it changes the cross-section denominator and is generator dependent.

### H5 — `GenEmpz` / ISR convention mismatch

**Rank:** 5 / plausible contributor; not sole cause  
**Status:** Partly verified.

Evidence:

- Old local adaptive multiplicity scripts explicitly did not apply `GenEmpz` because old CSVs lacked it (`compute_crad_adaptive_v2.py:42-49`, `864-871`).
- `cradDiag` reads `GenEmpz` from EvlTree and falls back to HAT `GenEnHad - GenPzHad + e` term (`ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C:1469-1475`, `1673-1680`).
- User reports NonRad `GenEmpz` narrowly near 55–56 GeV while RAD is broader.

Mechanism:

If RAD `GenEmpz` includes ISR photon effects differently from NORAD, applying `GenEmpz>45` (and especially an upper window like `35<GenEmpz<70`) can change event rates. This is physical if applied symmetrically to the intended variable; it becomes a bug only if old CSVs lacked the cut or if RAD/NORAD use incompatible definitions. The cradDiag smoke directly tests the branch definition.

### H6 — Per-event weight column mismatch

**Rank:** 6 / known trap; likely not the current main problem  
**Status:** Known from prior docs, not re-verified across all files here.

Evidence:

- `research-crad-v5f-attempt.md:109-137` documents the NORAD `WTX1*WTX2*WTX3` vs `WTX2`-alone trap and identifies `weight1` as cached product.
- Current full xsec script uses `weight1*weight2` (`compute_crad_user_xsec_ratio_full.py:233-234`), matching current builder conventions.

Mechanism:

Wrong weight column could produce large absolute normalization errors. It should be audited in the one-file cradDiag smoke by printing sumw and comparing to known `fLumi` only in the optional xsec audit.

---

## Can current v6 RAD/full_v1 NonRad sidistrees be salvaged?

**For a central reliable `C_rad`: no, not as currently computed.**

What can be salvaged:

- The **production completeness** of full_v1 is useful provenance; local audit shows no missing or zero Stage 1/2 products for expected labels.
- `ChargedTotGen` rows in full_v1 can still be used to debug hadron-side shape differences and branch distributions.
- The full xsec-ratio JSON/table can be retained as an audit artifact demonstrating why the cross-section convention is invalid for SIDIS.

What cannot be salvaged without reprocessing or an event denominator:

1. **The xsec-ratio values cannot be used as `C_rad`** because they intentionally compute `sigma_norad/sigma_rad`, not `M_norad/M_rad`.
2. **A `ChargedTotGen`-only pass cannot prove `N_DIS`**, especially zero-current-hadron events. The cradDiag `sidisGenEventDiagTree` was created for precisely this gap.
3. **The lumi mapping remains insufficiently authoritative** for any xsec use. The local `crad_full_v1_g1j_lumi_coverage_audit.json` has contradictory-looking signs (`missing_fLumi=0` but all `missing_steering_block` and `n_labels_indexed=0`), so it needs specialist provenance review before any xsec-ratio audit is trusted.

**Qualified salvage option:** If the existing `output_v6/crad/diag_smoke/2026-06-10_cradDiag_phase_v2/` one-file RAD/NORAD outputs are schema-valid and contain both `ChargedTotGen` and `sidisGenEventDiagTree`, they can be used for the first formula smoke without rerunning. If not, rerun exactly one paired file through `cradDiag`.

---

## Decisive next smoke test

### Purpose

Demonstrate, on one paired RAD/NORAD input, that the sanctioned multiplicity-ratio formula produces an O(1) correction and that the previous nonsensical xsec ratios are due to observable/denominator/lumi convention rather than a hidden H1OO branch failure.

### Exact input pair

Use **DJANGOH14 EMINUSP** because both RAD and NORAD raw Britzger files exist and current remote cradDiag smoke artifacts already used em0405-like labels.

Primary raw-file candidates:

| Role | Exact file | Supporting steering/provenance |
|---|---|---|
| RAD raw HAT | `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_RAD/hat.NC.EMINUSP.RAD.C0000000.root` | fpack steering `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_RAD/fpack.NC.EMINUSP.RAD.C0000000.steer` points to generator file `NC.EMINUSP.RAD.C0000000` (`lines 3-5`, remote read). |
| RAD MODS sibling | `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_RAD/mods.NC.EMINUSP.RAD.C0000000.root` | Inferred sibling naming from H1OO HAT/MODS convention and existing directory content; verify by `h1-builder-engineer` before run. |
| NORAD raw HAT | `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.root` | fpack steering `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/fpack.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.steer` points to generator file `DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000` (`lines 3-5`, remote read). |
| NORAD MODS sibling | `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/mods.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.root` | Inferred sibling naming; verify before run. |

If a pre-existing cradDiag one-file output is used instead of rerun, use:

- RAD: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/crad/diag_smoke/2026-06-10_cradDiag_phase_v2/djangoh_rad_em0405/Django_Eminus0405_101/sidistree_v6_cradDiag_Django_Eminus0405_101.root`
- NORAD: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/crad/diag_smoke/2026-06-10_cradDiag_phase_v2/djangoh_norad_em0405/DjNorad_Eminus0405_1/sidistree_v6_cradDiag_DjNorad_Eminus0405_1.root`

### Luminosity convention for this smoke

**Primary multiplicity smoke:** no luminosity input; `L_MC` cancels. Required outputs are `N_h`, `N_DIS`, and `C_rad^M` only.

**Optional xsec audit (not central):** do not use guessed lumi. Known steering values are:

- NORAD chunk: `DjNorad_Eminus06_1` in `/data/dust/group/h1/klesthen/g1j_ES/Analysis/Steering/es.Norad.em06disMC13.Norad2021.GenLumi.steer` has `fLumi = 12841.59` for files `C.C0000000-0000009` (`remote lines 497-500`). If split uniformly, per-file share would be `1284.159` in the steering’s native units, but unit (`pb^-1` vs `nb^-1`) must be confirmed by `h1-normalization`.
- RAD MC13 chunk examples are not the raw Britzger RAD `NC.EMINUSP.RAD.C0000000`: `Django_Eminus06_101` in `es.Norad.em06disMC13.Norad2021.GenLumi.steer` has `fLumi = 5014.37` for official `/pnfs/.../djangoh14/8961` files (`remote lines 95-99`); `es.em06disMC13.steer` has older `fLumi = 6499.58` for `/pnfs/.../djangoh14/7072` (`remote lines 157-160`). **Neither is verified as the exact raw Britzger RAD C0000000 lumi.**

Therefore the decisive smoke must not depend on RAD raw-file luminosity.

### Formula and acceptance checks

For the one-file diagnostic, compute at least:

1. Event denominators from `sidisGenEventDiagTree`:
   - `N_DIS_rad = Σ weight1*weight2` for `Q2_es_gen>150`, `0.1<Y_es_gen<0.7`, `GenEmpz>45`.
   - `N_DIS_norad` with the identical cut.
2. Hadron numerators from `ChargedTotGen`:
   - `N_h_rad(bin) = Σ weight1*weight2` for the event cut plus `abs(GenPDG) in {211,321,2212}`, `IsGtr==1`, `Mother2==-1` if branch/API available, `xFGen<0`, finite `zGen`, `0<=zGen<1`, pT_h bin computed in γ*p CM or a coarser no-pT integrated smoke first.
   - `N_h_norad(bin)` identically.
3. `C_rad^M = (N_h_norad/N_DIS_norad)/(N_h_rad/N_DIS_rad)`.
4. A sidecar xsec ratio `C_rad^xsec = (N_h_norad/L_norad)/(N_h_rad/L_rad)` only if exact lumis are provided; label it invalid for central use.

Acceptance criteria:

- `sidisGenEventDiagTree` exists in both files and has nonzero entries.
- Event denominators are nonzero after the event cut.
- `ChargedTotGen` has nonzero current-hemisphere selected hadrons.
- Integrated and coarse `(Q2,x)` `C_rad^M` is O(1) and plausibly near the expected useful convention (~1.05–1.1, but allow bin/one-file fluctuation; reject values like 0.0045, 4.5, 0.32, 3.2 as central-candidate results).
- The xsec ratio, if computed, may differ strongly; this is expected and must not block multiplicity if `C_rad^M` is sane.
- Compare RAD vs NORAD `GenEmpz` distributions from event tree; document whether the NonRad narrow 55–56 GeV feature persists and whether it affects only event-rate/xsec or also per-DIS multiplicity.

---

## Specialist handoff plan

### 1. `h1-mc-db` — provenance gaps before any xsec audit

**Task:** Close exact raw-file and steering provenance for the first paired smoke.

Deliverables:

- Verify existence and HAT/MODS pairing for:
  - `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_RAD/hat.NC.EMINUSP.RAD.C0000000.root`
  - matching `mods.NC.EMINUSP.RAD.C0000000.root`
  - `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NORAD/hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.root`
  - matching `mods.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.C.C0000000.root`
- Identify whether any g1j_ES `H1SteerTree` block gives an exact `fLumi` for raw Britzger RAD `NC.EMINUSP.RAD.C0000000`; do not substitute official MC13 `Django_Eminus06_101` unless proving it is equivalent.
- Clarify native units of the relevant `fLumi` values and whether per-file shares are valid.
- Explain `es.NoRadDjango_em06_All.steer` vs `es.Norad.em06disMC13.Norad2021.GenLumi.steer` path difference (`dst2all_djangoh` vs `dst2all_djangoh14_NORAD`) and which is canonical for current full_v1.

### 2. `h1-normalization` — formula and lumi/DIS-denominator convention

**Task:** Approve the normalization convention before any result is promoted.

Deliverables:

- Explicit approval that central high-Q² charged-SIDIS `C_rad` is `M_norad/M_rad`, not `sigma_norad/sigma_rad`, under v6 policy.
- Decide the exact event denominator binning: at minimum `(Q2,x)`; if z/pT-dependent denominators are unnecessary, state why.
- Confirm whether `GenEmpz>45` alone or a window (`35<GenEmpz<70`) is the intended Born-level event cut for this user-requested `C_rad`; current user target says `GenEmpz>45`, current xsec script used `35<GenEmpz<70` (`compute_crad_user_xsec_ratio_full.py:242-246`, metadata says `35<GenEmpz<70` at `390-393`).
- For optional xsec audit only, define how `fLumi` units and chunk/per-file shares must be applied and whether `H1MCLumiCalc`/vertex corrections are required.

### 3. `h1-builder-engineer` — one-file cradDiag smoke

**Task:** Run or validate exactly one paired `cradDiag` smoke, not production.

Deliverables:

- Confirm `ChargedSidisTreeBuilderHighQ2_v6_cradDiag.C` compiles and the binary used matches this source.
- If existing `output_v6/crad/diag_smoke/2026-06-10_cradDiag_phase_v2/...` outputs are schema-valid, reuse them for the smoke; otherwise process exactly the one RAD/NORAD pair listed above.
- Validate output schema includes both `ChargedTotGen` and `sidisGenEventDiagTree` with required branches.
- Produce a compact JSON/table with `N_DIS`, `N_h`, `C_rad^M`, and (optional, marked invalid-for-central) `C_rad^xsec` for integrated and coarse `(Q2,x)` bins.
- Include a `GenEmpz` comparison plot or quantiles from `sidisGenEventDiagTree` for RAD vs NORAD.

### 4. `h1-script-runner` / `h1-naf-operator` — bounded remote execution mechanics

**Task:** Execute only after h1-builder-engineer defines the one-file command or the existing diag-smoke reuse path.

Deliverables:

- Follow H1 SSH rules; no retries; one-file only on login node if under 5 minutes, otherwise a single standard Condor smoke job.
- Do not launch multi-file production until the smoke is reviewed.
- Package smoke output paths and logs for local review.

### 5. `h1-analysis-critic` — final formula review before promotion

**Task:** Review the one-file smoke and final formula semantics.

Deliverables:

- Confirm central `C_rad` uses multiplicity formula and correct direction (`M_norad/M_rad`) for the intended downstream application.
- Confirm cross-section-ratio artifacts are quarantined/audit-only and not mixed into systematics or publication tables.
- Confirm the event denominator includes zero-hadron events and matches the final analysis phase space.
- Confirm `xFGen<0` is the H1 current-hemisphere convention (`context-handoff.md:364-380`).

---

## Risks, assumptions, and open questions

### Risks

1. **Formula drift risk:** Multiple scripts and docs use different `C_rad` meanings (`M_norad/M_rad`, `sigma_norad/sigma_rad`, historical V0 denominator convention). Every output must carry `crad_type` and formula metadata.
2. **Luminosity overreach risk:** If anyone tries to interpret the full_v1 xsec-ratio outputs as central, unresolved fLumi unit/scope/provenance can shift results by factors of 10–1000.
3. **Event denominator risk:** Without `sidisGenEventDiagTree` or equivalent event tree, `N_DIS` can be biased by requiring at least one selected hadron row.
4. **Empz convention risk:** Old CSV-based multiplicity outputs lacked `GenEmpz`; current xsec script used `35<GenEmpz<70`; user target says `GenEmpz>45`. This must be standardized before mini-production.
5. **Raw Britzger RAD lumi risk:** Exact `fLumi` for raw `NC.EMINUSP.RAD.C0000000` is unresolved; avoid xsec dependence until `h1-mc-db` closes it.
6. **One-file statistical risk:** One file can prove convention/mechanics, not final per-bin precision. Promotion requires a mini/full pass after formula review.

### Assumptions

- The expected useful high-Q² SIDIS correction near `~1.05–1.1` refers to the multiplicity-style correction or downstream application factor, not the hadron xsec ratio.
- `weight1*weight2` is the correct per-row gen weight in v6/cradDiag sidistrees; this matches current scripts and builder branch semantics but should be printed in the smoke.
- The H1 current hemisphere is `xFGen<0` for the internal branch, as documented in `context-handoff.md:364-380`.

### Open questions

1. Does an existing cradDiag smoke already contain `sidisGenEventDiagTree` and enough entries to compute `C_rad^M` without rerunning? Candidate paths under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/crad/diag_smoke/2026-06-10_cradDiag_phase_v2/`.
2. What is the exact raw Britzger RAD `fLumi` for `NC.EMINUSP.RAD.C0000000`, and in what units? This is only needed for optional xsec audit.
3. Should final `C_rad` event cut be `GenEmpz>45` (user target) or `35<GenEmpz<70` (some existing scripts)?
4. Should final denominators use `Q2_es_gen/Y_es_gen/x_es_gen` exclusively, or should there be a fallback to `Q2sGen/YsGen/xGen` for NORAD edge cases? `cradDiag` exposes both.
5. Should the final correction be reported as `C_rad^M = M_norad/M_rad` or its reciprocal depending on downstream application? The formula must be approved by `h1-normalization` and `h1-analysis-critic` before release.

---

## Bottom line for downstream planning

The remaining issue is not primarily a missing manifest mapping anymore. It is a **semantic normalization problem**: the current full_v1 cross-section-ratio path computes the wrong quantity for high-Q² charged-SIDIS `C_rad` and reintroduces unresolved MC-lumi provenance. The clean path is to validate and then produce `C_rad` from **paired RAD/NORAD v6/cradDiag outputs with an explicit event-level DIS denominator**, using `M_norad/M_rad` and no MC luminosity dependence for the central multiplicity correction.
