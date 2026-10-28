# RAPGAP-π ReqID 9134 (genID 5623): verified normalization facts — luminosity units, weights, scale, comparability

- **Date:** 2026-10-21
- **Author:** h1-normalization (luminosity and weight authority)
- **Scope:** verified normalization facts for the RAPGAP31 `PIPEXCH` (pion-exchange) MC sample ReqID 9134 (genID 5623, e+ 06_07e+ SW84, RAD, GRV-LO-π, Holtmann-π flux, LN θ-smearing `sigx=46.0e-6`/`sigy=168.0e-6`, Q2GT3), as processed by the v6fnc campaign of 2026-10-20, **before any rate-level data comparison**. No scaling has been applied and no weighted plots were produced in this task; this document records verified numbers, the correct scale formula, and a comparability verdict only.
- **Method:** login-node read-only checks only (single-file ROOT opens, each well under 5 min wall; plain ROOT 6 via the H1OO `thish1.sh` environment for file access only): the merged Stage-2 tree, one per-chunk Stage-2 file, one original HAT file on `/pnfs`, the campaign steering/sources/logs, and the workspace luminosity anchors. No Condor jobs; no multi-file scans.
- **Inputs read:** `sidis_highQ2/context-handoff.md`, `docs/fnc_study/rapgap_pi_mc_availability_2026-10-20.md`, `docs/fnc_study/fnc_rapgap_pi_comparison_2026-10-20.md`, `docs/fnc_study/fnc_rapgap_pi_highQ2_detailed_2026-10-21.md`, `sidis_highQ2/docs/status/normalization.md`, `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`, campaign steering/`src`/logs/`validation/finalize_v6fnc_rapgap_pi_9134.json`.
- **Related docs:** `rapgap_pi_mc_availability_2026-10-20.md` (sample inventory; MCDB Lumi provenance), `fnc_rapgap_pi_highQ2_detailed_2026-10-21.md` (shape-level comparison and tag fractions), `v6fnc_builder_change_and_smoke_plan_2026-10-20.md` (campaign plan).

---

## 1. Executive summary

| Question | Answer |
|---|---|
| MCDB Lumi for genID 5623 (ReqID 9134) | **305.06 pb⁻¹ = 305,060 nb⁻¹** — the recorded MCDB value with units **verified as pb⁻¹** (evidence chain in §3). |
| Generator cross section implied (unweighted production) | σ_gen(Q²>3, π-exchange, e+ 06_07e+) = N/L = 10,000,002 / 305,060 nb⁻¹ = **32.78 nb**; companion genID 5622 independently gives 32.77 nb — consistent. |
| Event weights in the 9134 HAT files | **Weight1 ≡ 1.0 and Weight2 ≡ 1.0 exactly** (verified on the original HAT file, §4). This is an **unweighted production** — NOT a wrong-file-source situation (investigated and cleared, §4). |
| What the v6fnc campaign steering configured | **No luminosity anywhere.** Stage-1 steer: `fHatFiles`/`fModsFiles`/`fLoadODS` only; Stage-2 steer: `fEventListFiles` only; no `SetMCLumi`/`SetDataLumi`/`fLumi` in either source; stored weights are the raw HAT `Weight1`/`Weight2` (§5). **Luminosity is NOT in the stored weights.** |
| Correct scale to data | `scale = L_data / L_MC = 351,000.0 nb⁻¹ / 305,060.0 nb⁻¹ = 1.15059`; MC→data-equivalent yield = `scale × Σ(weight1·weight2)` = `1.15059 × N_selected` (weights are unity, §6). |
| Unit trap (REJECTED reading) | Reading 305.06 as nb⁻¹ would give σ = 32.8 μb (impossible, >2000× the generator-recorded inclusive Q²>60 cross section) and scale = 1150.59 — a **1000× overestimate**. Rejected. |
| Comparability verdict | Shape-normalized comparisons and LN-tagged comparisons with an **explicit pion-exchange fraction** are meaningful; **absolute inclusive-data rate comparison is NOT meaningful** (pure π-exchange subset vs inclusive DIS); absolute LN tag rates are not like-for-like (§7). |
| New artifact-integrity finding | The merged Stage-2 file contains **two** trees named `ChargedTotGen`; the larger (4,502,688 rows) has unreadable/uninitialized weight branches. Do not compute weighted gen-level sums from the merged file. The reco tree `ChargedTot` is validated and unaffected (§8). |

---

## 2. Sample and campaign facts used (verified earlier, cited here)

- ReqID 9134 = genID 5623, RAPGAP31 `PIPEXCH` e+ 06_07e+ SW84, RAD, GRV-LO-π, Holtmann-π flux, LN θ-smearing `sigx=46.0e-6`/`sigy=168.0e-6`, generator files C00–C72, **10,000,002 generated events**; MCDB Lumi column **305.06** (units unverified at the time of `rapgap_pi_mc_availability_2026-10-20.md`; verified here).
- Companion genID 5622 (e− counterpart family): MCDB Lumi **273.45**, ~8.96M events.
- v6fnc campaign (2026-10-20): 101 HAT/MODS pairs from `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/9134/` (filenames embed `RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3.smearing.9134.S39600.R97905`); Stage-1 consumed **9,999,998** events (4-event edge difference vs generated — benign); Stage-1 retained **260,195** entries (2.60195%); Stage-2 tree **393,344 rows / 46,292 events**; **29,842** events pass the high-Q² stored-branch selection (documented in `fnc_rapgap_pi_comparison_2026-10-20.md` / `fnc_rapgap_pi_highQ2_detailed_2026-10-21.md`).
- **Never combine** 9134 with 8946 or 9137 (same genID 5623, 100% overlapping re-reconstructions of the same generated events — `rapgap_pi_mc_availability_2026-10-20.md`).

---

## 3. L_MC unit verification: 305.06 is pb⁻¹

The MCDB Lumi value for this sample family is in **pb⁻¹**. Evidence chain (workspace-internal anchors; no memory-based absolute cross sections used):

1. **fLumi unit correction (2026-09-25, `docs/status/normalization.md`):** MC lumi values sourced from MCDB/H1OO records are pb⁻¹. Calibration sample: the DjBkgd five-ReqID set (Q²gen>2, NORAD) with per-ReqID values 4557.60 / 3690.90 / 1708.80 / 2563.20 / 3292.90 **pb⁻¹** (sum 15,813.4 pb⁻¹ = 15,813,400 nb⁻¹). A 10M-event-scale production sits naturally at hundreds-to-thousands of pb⁻¹ in this column.
2. **oomclumi anchors (`H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`):** for every parsed central catalog, `L_MC = WeightsSum/σ` comes out at ~10³ pb⁻¹ — e.g. djangoh14/7091 (e+ 0607, Q²>60): `L_MC_pbinv = 4379.935` (WeightsSum 65,877,840 / SIGMT 15,040.826 pb); rapgap31/7094: `L_MC_pbinv = 3054.878`. A 101-chunk Q²>3 π-exchange production at 305.06 in the same MCDB Lumi column is consistent with this family; under an nb⁻¹ reading it would have ~14,000× less luminosity than a comparable-chunk-count Q²>60 catalog, which is structurally backwards (wider phase space ⇒ more events/lumi, not less).
3. **Cross-section sanity with the measured unit weights (Σw = N):**
   - **pb⁻¹ reading:** σ_gen = N/L = 10,000,002 / 305,060 nb⁻¹ = **32.78 nb** — a plausible pion-exchange leading-neutron subset of DIS at Q²>3.
   - **nb⁻¹ reading (REJECTED):** σ_gen = 10,000,002 / 305.06 = 32,783 nb = **32.8 μb** — impossible: more than 2000× the generator-recorded *inclusive* σ(Q²>60, e+ 0607) = **15.04 nb** (7091 `SIGMT_pb = 15040.826` anchor), and the π-exchange channel is a strict subset of DIS, so it cannot exceed any plausible total DIS cross section in this Q² range by two orders of magnitude.
   - **Companion-sample consistency:** genID 5622 independently gives σ = 8.96M / 273,450 = **32.77 nb** — the same channel cross section from a different event count and luminosity, confirming the L ∝ N (unweighted) convention of the MCDB record for this production pair.
4. **Design-scale remark:** L_MC = 305.06 pb⁻¹ is ~87% of L_data = 351 pb⁻¹ — the natural sample-design scale for a dedicated signal MC.

**Verdict: L_MC(9134) = 305.06 pb⁻¹ = 305,060 nb⁻¹.**

*Limitation (recorded):* the MCDB CGI is disabled (P-CRAD-LUMI-era blocker), so the MCDB record could not be re-queried; oomclumi has not been run on the 9134 catalog (its generator-recorded σ/SIGMT is not locally available), so the value is taken **as recorded** with units verified by the anchors above. This is sufficient for the study; an oomclumi pass over the 101 HAT files would close the last provenance gap if ever required.

---

## 4. Event weights: unweighted production, verified at the HAT source

**Measurements (plain ROOT, single files):**

| File | Tree | Entries | weight1 / Weight1 | weight2 / Weight2 |
|---|---|---|---|---|
| Merged Stage-2 `chargedsidis_v6fnc_rapgap_pi_9134_merged.root` | `ChargedTot` (reco) | 393,344 rows / 46,292 events | mean = min = max = **1.00000** | mean = **1.000000** |
| Same file | `ChargedTotGen` (597,179-row cycle) | 597,179 | **1.00000** | **1.000000** |
| Per-chunk `sidistree_v6fnc_rapgap_pi_9134_ep0607_0000.root` | `ChargedTotGen` | 38,649 | **1.00000** | **1.000000** |
| **Original HAT** `/pnfs/.../9134/hat...DST.0000.root` | `HAT` (both cycles: 90,000 + 49,044) | 139,044 | mean = min = max = **1.000000** | mean = **1.000000** |

**Wrong-file-source rule invoked and cleared.** The workspace rule "RAPGAP31 mean weight1 ~1.0 on a purported MC sample ⇒ stop, wrong file source" (calibrated on V0-era BaryonAsym RAPGAP31 samples with mean w1 ≈ 4.14, range 1–53) was triggered by the w≡1 measurement and **investigated to conclusion**:

- The **original 9134 HAT file itself** carries `Weight1 ≡ Weight2 ≡ 1.0` exactly — the v6fnc chain faithfully preserved the source weights; nothing was lost or overwritten in Stage-1/Stage-2.
- Sample identity is independently established: 101 hat/mods pairs from `/pnfs/.../rapgap31/9134/` with the production tags in every filename; MC identity auto-detected from HAT RunType; 9,999,998 events consumed ≈ 10,000,002 generated; Q²>3 generator spectrum and π-exchange LN content per the campaign comparison docs.
- **Conclusion:** ReqID 9134 is an **unweighted MC production** (a special smearing production). The V0-era mean-w1≈4.14 expectation **does not transfer** to this production and must not be used to flag it in future audits. (For the central high-Q² catalogs the non-trivial weights are real — e.g. 7094 WeightsSum 45,822,684 — the difference is production-specific, not pipeline-specific.)

**Consequence for normalization:** the general MC weight rule `weight1 × weight2` still applies formally; here it evaluates to exactly 1 per event, so **Σ(weight1·weight2) = N_selected** in any selection. (For data, weight2 = 1.0 by construction in this chain.)

---

## 5. What the v6fnc campaign actually configured

- **Stage-1 steering** (`condor_v6/studies/v6fnc_rapgap_pi_9134_2026-10-20/steer_stage1/*.steer`): only `fHatFiles`, `fModsFiles`, `fLoadODS`. The steer format carries a comment that no period/run-number/lumi fields exist; MC identity is auto-detected from the HAT RunType. The Stage-1 `.out` logs confirm only `fModsFiles`/`fHatFiles`/`fTreeCacheSize` accepted.
- **Stage-2 steering** (`steer_stage2/*.steer`): only `fEventListFiles`.
- **Sources:** Stage-1 `src_stage1/TreeBuilderHighQ2_chargedsidis_v6fnc.C` and Stage-2 `src/ChargedSidisTreeBuilderHighQ2_v6fnc.C` store the raw HAT weights — `weight1 = *hatWeight1; weight2 = isMC ? *hatWeight2 : 1.0f` (reco-envelope path), with the gen-only path also reading the raw HAT values. **No `SetMCLumi`, no `SetDataLumi`, no `fLumi` anywhere** in either source. `H1CalcWeight.h` is included and the standard vertex/trigger weight steering is loaded (H1VertexWeight, H1CalcNCTrigWeight, H1CalcNCHighQ2TrigWeight) — vertex reweighting context only; no luminosity enters the stored weights.
- **Log evidence:** every stage-1 log carries `<H1Constants::SetSubPeriodLumis>: Your runlist file does not exist` — the framework found no runlist and loaded **no** luminosity.
- **Consistency remark:** even the central v6 7091 sidistree steers carry only a comment (`// Chunk luminosity: 4383.1 nb^-1`, the nominal 1000-way division of the 4,379,935 nb⁻¹ total) — luminosity is applied at analysis level in this workspace, never in tree production.

**Bottom line: no luminosity was set at any stage; the stored weights are the raw HAT `Weight1`/`Weight2` (both unity for this sample); luminosity is NOT in the weights.**

---

## 6. Correct scale formula (conceptual — NOT yet applied)

With L_data the current four-period S67 high-Q² target and L_MC the verified sample luminosity:

```
scale = L_data / L_MC = 351,000.0 nb⁻¹ / 305,060.0 nb⁻¹ = 1.15059
```

- **MC→data-equivalent yield** in any selection: `N_pred = scale × Σ(weight1·weight2) = 1.15059 × N_selected` (weights unity).
- **Equivalent generator-level form:** `N_pred = L_data × σ_gen × ε_sel` with σ_gen = 32.78 nb and ε_sel the selection efficiency over the *full* generated sample.
- **L_data authority:** `351,000.0 nb⁻¹` (ep0304 + em0405 + em06 + ep0607). **Rejected for this regime:** `297,657.8 nb⁻¹` (2005–2007 only; excludes ep0304) and the superseded direct-monitor quotes `356,613.0` / `356,000 nb⁻¹`. Note the aggregated-JSON field `L_data_HERA-II_S67_nb: 297657.8` is the 05–07 scope and must NOT be used here.
- **Unit trap:** reading 305.06 as nb⁻¹ gives scale = 1150.59 — a 1000× overestimate. Keep both luminosities in nb⁻¹ (or both in pb⁻¹: 351.0/305.06 = 1.15059, identical).
- **Validity condition:** `scale × N_selected` is valid for like-for-like selections applied to the filtered study tree. The tree is **Stage-1/Stage-2 filtered** (Stage-1 envelope-OR retention 260,195/9,999,998 = 2.60195%; only ~0.886% of generated events have Q2eGen > 150) — it is **not** an unbiased Q²>3 sample and must not be normalized to "all Q²>3 generated events" for anything.
- **Standard rules that still apply:** for shape QA, scale the MC integral to data; MC luminosity cancels in efficiency ratios (cross-section style); if a luminosity uncertainty is quoted, it is 2.5% flat, correlated across all bins.

---

## 7. Comparability verdict for a pure pion-exchange leading-neutron sample

**Meaningful uses:**

1. **Shape-normalized comparisons** of LN kinematics (x_L, Q², W, FNC θ/x spectra) between data and RAPGAP-π — with shape normalization, never absolute normalization of the shapes.
2. **LN-tagged data yields vs RAPGAP-π predictions** — only with an **explicit pion-exchange fraction** input: RAPGAP-π predicts the π-exchange component; data LN-tagged yields contain π-exchange + all other LN production mechanisms.
3. **FNC detector-response / acceptance studies** within the credibility window (reco `FncThetaN < 0.75 mrad`).

**NOT meaningful:**

1. **Absolute inclusive-data rate comparison.** 9134 is a *pure* π-exchange sample (every event has a leading neutron from pion exchange) covering only the π-exchange subset of DIS (σ_gen = 32.78 nb at Q²>3); inclusive data contain all DIS. An L_data/L_MC-scaled RAPGAP-π yield is a prediction for the π-exchange component only, not for inclusive data.
2. **Absolute LN tag-rate comparison.** Tag fractions are not like-for-like: RAPGAP-π 34.596% (100% LN by construction, tag-limited) vs DATA 20.205% vs DJANGOH14/7091 6.582% (inclusive MC underproduces LN). Comparing absolute tagged rates across these samples mixes sample definition with physics.
3. **FNC absolute angle/position comparisons beyond the credibility window** — the proton-beam tilt is not modelled in MC (per `fnc_rapgap_pi_highQ2_detailed_2026-10-21.md`).

**Combination rule:** use exactly one ReqID of genID 5623 (9134 recommended; never combined with 8946/9137 — 100% overlap ⇒ double counting).

---

## 8. Artifact-integrity caveats (new findings, 2026-10-21)

1. **Merged Stage-2 file — doubled `ChargedTotGen`.** `output_v6fnc/rapgap_pi_9134_study/stage2/merged/chargedsidis_v6fnc_rapgap_pi_9134_merged.root` contains **two** trees named `ChargedTotGen`:
   - 597,179 rows — clean (w≡1, valid IDs) but **partial**;
   - 4,502,688 rows — weight1/weight2 and run/event-id branches read as **uninitialized garbage** (values up to ~10³⁰⁸; never-filled/stale-cycle content from the 101-file `hadd`).
   Their sum (5,099,867) is presumably the full gen-row total, but **neither merged cycle is a complete, valid gen tree**. Per-chunk Stage-2 files contain a single clean `ChargedTotGen` (chunk 0000: 38,649 rows). **Rule: do not compute weighted gen-level sums from the merged file — chain the per-chunk files, or re-merge with per-tree entry validation.**
   - The reco tree `ChargedTot` in the merged file is **validated and unaffected**: 393,344 rows (= the finalize-JSON stage-2 total), 46,292 events, w1=w2≡1. All reco-side FNC study results to date used this tree.
2. **Stage-1 merged tree is not a Stage-2 fork input** (already documented in the campaign finalize JSON): `hadd` drops the `EvlList` key (`evllist_key_present_after_hadd: false`). Stage-2 must consume the per-file Stage-1 EVLS. This is the known H1EventList-class hazard; the campaign already routes around it.
3. **HAT chunk 0000 structure note:** the file contains two `HAT` tree cycles (90,000 + 49,044 entries; the availability-doc spot-check used the 90,000-entry cycle). Both cycles carry unit weights, so the §4 conclusion is cycle-independent.

---

## 9. Blockers and open items

- **MCDB CGI disabled** — no independent re-query of the MCDB record (value taken as recorded; units verified via §3 anchors).
- **oomclumi not run on 9134** — σ_gen/SIGMT for this catalog is not locally recorded; running oomclumi over the 101 HAT files would independently re-derive L_MC and is the recommended closure if this sample ever enters a normalization-critical release.
- **No rate-level comparison produced in this task** — per the task scope, only verified numbers, the scale formula, and the verdict were delivered; applying the scale and producing weighted plots is the delegating session's next step.
- **Gen-tree re-merge** (§8.1) recommended before any gen-level weighted use of the study sample.

---

## 10. Reference values (quick copy)

| Quantity | Value | Source/verdict |
|---|---|---|
| L_MC(9134) | **305.06 pb⁻¹ = 305,060 nb⁻¹** | MCDB as recorded; units verified §3 |
| L_MC(5622, companion) | 273.45 pb⁻¹ | MCDB as recorded; consistent via σ |
| σ_gen(9134, Q²>3, π-exchange) | 32.78 nb | N/L with unit weights |
| L_data (current four-period) | **351,000.0 nb⁻¹** | `context-handoff.md`; 297,657.8 and 356,613/356,000 rejected |
| scale = L_data/L_MC | **1.15059** | §6 |
| Wrong-unit scale (REJECTED) | 1150.59 | 1000× overestimate trap |
| Stored weights (9134) | weight1 = weight2 = 1.0 exactly | §4, verified at HAT source |
| Luminosity in stored weights | **No** | §5 |
| Luminosity uncertainty (if quoted) | 2.5% flat, correlated | workspace rule |
