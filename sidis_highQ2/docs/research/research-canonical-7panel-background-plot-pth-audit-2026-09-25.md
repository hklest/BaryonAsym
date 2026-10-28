# Forensic audit — canonical background 7-column plot and pT_h frame

**Scope:** read-only audit of high-Q² inclusive charged-hadron SIDIS, current v6/z005 only.  This note distinguishes confirmed facts from inferences.  Context read: `STATE.md` (2026-09-25), `context-handoff.md`, and `docs/RUN_STATUS.md`; the latter names the current release contract as 490 published bins = 245 per charge ([RUN_STATUS.md:50-60](../RUN_STATUS.md)).

## Executive conclusion

**Verified:** the current source-yield background figure is **not a seven-Q²–x-panel plot**.  It is the 49-panels-per-charge **7col** figure at `plots/backgrounds/canonical_7panel_bg_yields_2026-09-25/`, despite its legacy/misleading directory stem.  `STATE.md:33-34` explicitly calls this the background-yields plot and settles `7col = 7 z columns × 7 (Q²,x) rows = 49 panels/charge`.

**Verified:** its plotted pT axis is the published `pT_h` grid `[0,.5,1,2,4,8]`, not an event-level lab `Pt` branch.  Every contributing event-level producer audited bins a CM photon-axis pT variable, except the explicitly quarantined det.int smoke script; the full det.int aggregate demonstrably excludes smoke members.  Therefore the user’s **lab-pT binning concern is REFUTED for the current source-yield figure**.

A separate, serious issue remains: the figure deliberately displays a known-defective, retired event-level low-Q² leakage component.  It is visibly/status-text flagged, but it is not a current v6-normalized physics component.

## 1. Identity and terminology

| Candidate / figure family | Convention | Producer | Date / lineage | Classification |
|---|---|---|---|---|
| `plots/backgrounds/canonical_7panel_bg_yields_2026-09-25/background_yields_{hplus,hminus}_{linear,log}_7col.pdf` and review PDF | **7col**: 7 z columns × 7 Q²–x rows, 49 panels/charge | `plots/backgrounds/canonical_7panel_bg_yields_2026-09-25/plot_background_yields_7col.py` | 2026-09-25; v6-path diagnostic synthesis | **Current source-yield background figure** named by `STATE.md:33-34`; provisional internal diagnostic, not a central/subtraction product. |
| `output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/plots/background_fractions_vs_data_onepage_7col.pdf` | **7col**, one page; panels alternate charge for Q²/x/z groups | `.../scripts/regen_minimal_7col_plots.py` (canonical snapshot copy: `scripts/canonical_highQ2_sidis_2026-06-30/plotting/regen_minimal_7col_plots.py`) | 2026-06-25/28 snapshot; v6 alpha0p2 diagnostic, historical normalization construction | The formerly declared canonical **background-fraction** display, per canonical snapshot `RUNLIST.md:96-108` and archived status `.../docs/RUN_STATUS.md:58,96-100`; not the 25-Sep yield figure and not a current central product. |
| `plots/reco_backgrounds_alpha0p2_z005_2026-06-24/background_fractions_7col/` | Metadata/sidecars only; no PDF found in this local directory | producers not retained here; README points to aggregate/fraction product roots | 2026-06-24; historical/provenance | README says older artifacts may retain 356613 normalization ([README:11-14](../../plots/reco_backgrounds_alpha0p2_z005_2026-06-24/background_fractions_7col/README.md)); not a current rendered canonical plot. |
| `plots/lowq2_djangoh_bg_alpha0p1_2026-06-23/background_fraction_pt_h_by_Q2_x_z_analysis_note/...onepage...py` | analysis-note one-page pT_h-by-Q²/x/z background fraction; **not established as 7col/7panel by the surviving evidence** | `plot_background_fraction_pt_h_by_Q2_x_z_analysis_note_onepage_x0to8_nobox.py` | 2026-06-23, alpha0p1 / historical | Historical low-Q²-only diagnostic; canonical snapshot records alpha0p2 as current and calls alpha0p1 provenance-only. |
| `plots/cross_sections/finalcandidate_7panel_release351_expanded_systematics_binwise_bg_2026-09-24/cross_section_density...7panel.pdf` | **7panel**: seven Q²–x cells/charge (2×4 layout with one blank) | `.../make_expanded_systematics.py` | 2026-09-24; v6 release cross-section diagnostic with historical B′ lowQ² uncertainty proxy | **Not a background-yield/fraction figure.**  It draws cross sections and folds a background *uncertainty* proxy; source labels call it provisional/historical (`make_expanded_systematics.py:12-24,44-64`). |

`plot_perbin_7col.py` is also not the background-fraction producer.  It is the canonical template for a matched-V0-daughter/true-π/K/p analysis-note ratio (`RUNLIST.md:104-105`; source docstring/inputs at `plot_perbin_7col.py:1-39`).

## 2. pT_h versus lab Pt audit

| Chain element | exact binned variable and evidence | verdict |
|---|---|---|
| `.../scripts/produce_reco_background_vector_alpha0p2_v2.py` | `pth_cm`, returned by `compute_pth_cm_and_z` (`390-425`); it boosts `h_lab` and `q` to W rest (`411-412`) and forms `pth_h_cm` perpendicular to boosted `qhat` (`417-425`). It bins `pth_idx = pth_index_reco980(pth_cm,...)` (`683-701`). | **PASS — CM pT_h.** `Pt` is only the quality cut `Pt > 0.15` (`125-131`, `659-669`). |
| `produce_detint_full_alpha0p2.py` | `pth_h_cm` from the same W-rest helper (`391-426`), then `pth_idx = pth_index_reco980(pth_cm,...)` (`697-715`). | **PASS — CM pT_h.** Lab `Pt` is only quality (`126-132`, `673-683`). **Separate normalization defect:** its `L_DATA_TOTAL_NB=356613` (`37`) is historical. |
| `aggregate_detint_full_alpha0p2.py` | No event pT calculation: sums supplied 980 vectors (`276-314`). Rejects smoke unless `--allow-smoke` (`53`, `138-146`); output records `allow_smoke` (`323-335`). Actual final djangoh aggregate metadata has `allow_smoke:false` and 501 `smoke:false` per-file members (`AGGREGATED...json:9-15,41-59`). | **PASS (no pT rebinned).** Smoke partials were not consumed. Historical 356613 normalization (`19`) remains. |
| `produce_detint_smoke.py` | `pth = sqrt(px²+py²)` explicitly described as lab proxy (`27-44`), digitized at `97`. | **FAIL, but quarantined smoke only.** It has `L=356613` (`9`) and fake coarse cells (`68-70`); not an aggregate input. |
| `aggregate_reco_background_vectors_alpha0p2_v2.py` | No event pT calculation; raw 980 vector aggregation (`276-314`), with 351000 target (`19`). | **PASS (no pT rebinned).** Rejects smoke by default (`53`, `138-146`). |
| `scripts/background_subtraction/v6_bprime_djangoh_lowq2/build_lowq2_djangoh_bg_hist_v6_bprime.py` | imports canonical `compute_pTh_cm` (`127-162`); computes `pT_h=compute_pTh_cm(...)` (`430-434`) and bins `ipt=pt_index(pT_h)` (`437-460`). | **PASS — CM pT_h.** `Pt > Pt_min` is only quality (`201`, `358`, `405-410`). |
| `plot_background_yields_7col.py` | Does not calculate an event observable.  It uses final release `pT_min/pT_max` row metadata (`53-56,64-72`) and fixed display edges `PT=[0,.5,1,2,4,8]` (`20-23`). Axis label is `$p_{T,h}$` (`72`). | **PASS — consumes pT_h-labelled published-grid edges.** |
| `regen_minimal_7col_plots.py` | No event calculation. It reads 980 vectors and pairs native **pT_h** bins `(0+1)...(8+9)` (`35-40`, `243-277`); source metadata labels native/display pT bins and axis `$p_{T,h}$` (`474`, `517-521`). | **PASS — pT_h bin metadata/vector aggregation.** |
| `plot_perbin_7col.py` | No event calculation; uses CSV fields `pT_idx,pT_min,pT_max` (`43-48`, `248-250`) and labels `$p_{T,h}$` (`37-39`). | **PASS for the plotter’s pT_h-labelled input; it does not feed the background-fraction/yield figure.** |

The imported v6 reference independently states that pT_h is recomputed from four-vectors, **not lab Pt**, and gives the W-rest / photon-axis definition (`build_beam_sign_data_vectors_v6.py:103-113,389-431`).

## 3. v6 completeness and the low-Q² normalization issue

### 25-Sep source-yield figure

**Verified v6/current pieces.** Its track-level inputs and release table are under `output_v6` (`plot_background_yields_7col.py:16,19`); it declares `LUMI=351000` and scales displayed event-level yields by `351000/356613` (`20-22,49`).  The release table has 490 rows enforced (`53-57`), and the binmap collapses each charge’s 490 reco bins to 245 published bins (`33-35,38-49`).  Thus its displayed grid is **490 = 245+245**, with the finalcandidate `pT_h` edges, not a 480-bin grid.

**Not fully v6/current.** The figure imports: (i) a B′ event-level low-Q² template explicitly made at target 356613 with scale 22.551317 (`.../lowq2_djangoh_bg_v6_bprime_245.json:29-43`); and (ii) a `tunfold_official_v4...migration_inputs_v4...root` solely for the 490→245 mapping (`plotter:17-18,38-49`).  The B′ template itself is based on `output_v6/central/sidistree/mc_bg/djangoh14_lowQ2bkgd` (`template JSON:10-14`) and has 245 bins/charge (`134-155`), so it is v6-tree lineage and grid-compatible but **not current-normalization/current-background-policy clean**. `STATE.md:48-51` supersedes 356613 with 351000.

### Is 15813.4 a 7091 mis-scale?

**Resolved with high confidence:** no, not for this event-level template.  It is a different DJANGOH low-Q² background sample, not processed high-Q² RAD 7091.  Direct evidence is its input path `mc_bg/djangoh14_lowQ2bkgd` (`template JSON:10-14`) and its truth gate `4 < Q2sGen < 60` (`template JSON:23-27`; builder `398-403`).  The global current-status authority explicitly says 15813.4 belongs to **djangoh14/7110 DjBkgd**, whereas 7091 is the high-Q² RAD signal with 4,379,935 nb⁻¹ (`RUN_STATUS.md:105-109`; `context-handoff.md:213-217`).  Therefore applying 15813.4 to 7091 would be the 277.04× trap; using it as the DjBkgd template exposure is legitimate sample identity.  Its **356613 target** remains stale and is only plot-time rescaled by the 25-Sep script.

### Retired defective component

**Verified:** the 25-Sep figure still displays `lowQ2 (event-level leakage)` as one of five components (`plotter:25-29,47-50`) and includes it in TOTAL.  It explicitly labels the result provisional, says central v6 consumes none of the templates, states the one-event `w=43.1292` defect, and says the bin is marked/unclipped (`28-29`).  The h− bin-34 annotation is drawn at `76-79`; manifest records it (`106`).  This agrees with `STATE.md:24-29`: `background_rel` is retired, the event-level template is known defective, and the central release is raw/no-bgsub.  Hence it is an **annotated forensic display**, not an acceptable current systematic component.

## Actionable routing (no execution performed)

* **h1-plotting / h1-physics-analyst:** if a current canonical background plot is needed, preserve the 7col convention but exclude/visibly quarantine the retired event-level component and use a response-consistent, track-level-only source after P-ALPHA closure.
* **h1-normalization + h1-mc-db:** document the lowQ² DjBkgd ReqID/exposure provenance alongside the template before any reuse; do not conflate it with 7091.
* **h1-systematics:** retain `background_rel` retirement; the figure’s component must not re-enter an uncertainty budget.

No analysis product was modified.  This is the sole requested markdown research artifact.
