# Loop 3 Plotting-Script Audit — H1 High-Q² Inclusive Charged-Hadron SIDIS (v6/z005)

**Date:** 2026-10-19
**Critic loop:** 3 of 3 (plotting scripts) — residual coverage + final adjudication of loops 1+2
**FINAL basis at audit time:** cone+S2′+Set-R′ background-subtracted clean release, installed 2026-10-17 (RAW `bbb_ibu_bgsub_setRprime_conebase_..._2026-10-17`, IBU h+ `10.508811` / h− `10.217503` nb; Born `born_crad_bgsub_setRprime_conebase_..._2026-10-15`, h+ `11.996558` / h− `11.663956` nb). Quoted-total note: STATE.md (2026-10-19 Env100 Systematics Freeze, INSTALL COMPLETE) supersedes `total_with_bgsub19_rel` as the QUOTED systematic with `total_with_env100_rel` (`systematics_bgsub_env100_21comp_2026-10-19/`); the 2026-10-17 install-record rule (`total_with_bgsub19_rel` ONLY) and the 2026-10-19 freeze are both on the books — adjudicated below as a required recorded decision, not silently resolved.
**Tooling notes:** the `sidis-hq2-plotting` skill again failed to load (tool error, same as loop 2); the audit proceeded from on-disk code and artifacts. Loop-1 C1/C2 and M1–M7 were treated as double-confirmed and were NOT re-litigated; loop-2 B-M1/B-m1–B-m3/B-i* were likewise accepted as confirmed. All loop-3 line numbers refer to current on-disk copies.

## VERDICT

**FAIL (audit FAIL; final adjudication).** Loops 1 and 2 established two FATAL physics defects (C1: the registered canonical Born plotter reads the superseded 2026-10-13 cone Born release joined to the superseded `total_with_cone15_rel` budget; C2: the bgsub-diagnostic variant normalizes every S3_x0 panel point with the uncapped container edge, ×0.662). Loop-3 residual coverage finds **no new CRITICAL physics defect** — the live 2026-10-17 fitnorm diagnostic plotters and the cone-basis stat-sidecar plot block are clean on every hunted class — but adds **three new MAJOR findings that extend the C1 defect family**: (R1) a second registry copy of the canonical plotter (the local mirror, designated "the LIVE canonical plotter" by the final-basis Born release README) is runnable today and reads an even STALER basis (2026-10-09 Born release + `total_with_crad13_rel`), so the "md5-identical across both registry copies" registry claim is false and BOTH copies must be repointed; (R2) the final-basis Born release tree itself ships a frozen `canonical_plotter_crad_applied_source.py` that embeds the ruled-out uncapped-dx convention ("normalization uses true x") with three-bases-stale paths and `no_bgsub` product naming, disclosed as informational-only but carrying no supersession caveat; (R3) the never-audited `plot_uncertainty_pth_panels.py` is a triple-dead wrong-era (rev6_v2, 240-bin) template that the canonical index still marks `CURRENT_DIAGNOSTIC_OR_PROVISIONAL` and whose cell labels would publish the x=1.5 container edge if ever rendered. No script in the workspace can currently produce final-basis (2026-10-17) plotting products.

## Material reviewed (loop-3 residual coverage)

1. `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/plotting/plot_uncertainty_pth_panels.py` — 530 lines, read in full (never audited in any loop).
2. `H1ChargedSidis_HighQ2/output_v6/studies/bgsub_setr_cone_build_2026-10-11/fitnorm_2026-10-17/scripts/make_fitnorm_plots.py` — 168 lines, full; LIVE (both inputs exist).
3. `.../fitnorm_r2_2026-10-17/scripts/make_fitnorm_r2_plots.py` — 344 lines, full; LIVE; plus the r1-plotter copy inside the r2 tree.
4. `.../cone_15comp/scripts/assemble_stat_sidecar_cone.py` — embedded C2 plotting block (lines ~469–492) plus surrounding gate context.
5. `output_v6/central/data_release/born_crad_bgsub_setRprime_conebase_..._2026-10-15/provenance/canonical_plotter_crad_applied_source.py` — paths/CRAD/density/manifest regions (the ONLY plotting script in either clean-release tree).
6. `.../cone_15comp/scripts/stage2_cone_2026-10-15/package_bgsub_setrprime_born_release_cone.py` — CANON_SCRIPT/frozen-block provenance regions.
7. `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` — the LOCAL-MIRROR registry copy.
8. Supporting: `merge_fitnorm_partials.py`, `merge_fitnorm_r2_partials.py`, `extract_fitnorm_histos.py`, `run_fitnorm_fit.py`, `run_fitnorm_r2_fit.py`; both fit-result JSONs; the studies-tree matplotlib sweep; clean-release tree listings; `bgsub_env100_21comp_budget.csv` and `bgsub_setrprime_budget19_cleandenom.csv` headers; `RUN_STATUS.md`; `STATE.md`; the Born release `README.md`.

**Sweep completeness:** the RAW clean release (2026-10-17) contains NO plotting scripts; the Born clean release (2026-10-15) contains exactly one (item 5); the `stage2_cone_2026-10-15` scripts contain no plotting code; the env100 budget tree contains no plotting code; the full studies tree contains no other matplotlib code than items 2–5. No stale luminosity constants (297657.8 / 356613 / 126880) anywhere in the studies tree; all fitnorm scales derive from L=351000.0 nb⁻¹ (351000.0/9132891.0, 351000.0/10487495.0, lowQ2 0.08013817556653238).

## Part A — residual-coverage findings

### CRITICAL — none new

### MAJOR

**R1. Second registry copy of the canonical plotter (local mirror) is runnable and reads an even staler basis; the registry's md5-identity claim is false.**
- **Files/lines:** `sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py:94` (`BORN_ROOT = ".../born_crad_dj14relbin_S3xmerged_..._2026-10-09"`), `:100-102` (`BUDGET_DIR = ".../total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-09"`, `BUDGET_COLUMN = "total_with_crad13_rel"`), `:394` (P-XVOL cap present), `:688` (lumi 351000.0, correct).
- **Evidence:** the remote-snapshot copy audited by loops 1–2 reads the 2026-10-13 cone Born release + 2026-10-14 cone15 budget; this second copy reads the 2026-10-09 Born release + the 2026-10-09 crad13-emfix budget — TWO generations staler than even the superseded remote-snapshot targets, and three supersessions behind the final basis. Its inputs are installed trees, so it RUNS today and produces 2026-10-09-basis, crad13-budget figures under the registered canonical name. RUN_STATUS.md line 171 states the canonical plotters are "md5-identical across both registry copies" — no longer true. The final-basis Born release README (line 34) designates THIS mirror path as "the LIVE canonical plotter" with a pinned sha256 — the pin documents the 2026-10-15 state and the mirror has since drifted (P-XVOL patch).
- **Class/impact:** PLOT-NUMBER (active wrong-basis production capability) + PROVENANCE. **Blocks final-basis figure production** until both copies are repointed (or the mirror is explicitly de-registered/annotated).
- **Fix:** apply the C1 repoint to BOTH copies and re-assert md5-identity, or annotate the mirror as NON-CANONICAL and correct RUN_STATUS line 171 and the Born-release README designation.

**R2. The final-basis Born release tree ships a frozen "canonical plotter source" that embeds the ruled-out uncapped-dx convention, three-bases-stale paths, and `no_bgsub` product naming — with no supersession caveat.**
- **File/lines:** `output_v6/central/data_release/born_crad_bgsub_setRprime_conebase_..._2026-10-15/provenance/canonical_plotter_crad_applied_source.py` — `:94` (BORN_ROOT = 2026-10-09 Born release), `:100-102` (crad13-emfix budget, `total_with_crad13_rel`), `:394` (UNCAPPED `df['dx'] = df['x_max'] - df['x_min']`), `:452-453` ("Cap x_max display at 1.000 (text only; normalization uses true x)"), `:44`, `:575-579` + `:160` (output names `cross_section_density_nominal_no_bgsub_7panel*`, dir `plots/nominal_no_bgsub_cRad_applied/`), `:701-702` (manifest formula strings claiming `* C_rad` while the code at `:398` does not multiply).
- **Evidence:** the packaging script (`package_bgsub_setrprime_born_release_cone.py:1005`) copied this file from `CANON_SCRIPT`, which points at the LOCAL MIRROR (R1) — so the release's "canonical plotter source" is the staler mirror version. The file predates the 2026-10-16 P-XVOL patch: if executed, every S3_x0 density is low by ×0.662, and its self-describing strings assert exactly the convention the 2026-10-16 user binning ruling declares wrong. The README discloses it as "recorded informationally only — never executed by this build" (honest) but nothing records that it predates the P-XVOL patch or that its basis/budget are superseded.
- **Class/impact:** PROVENANCE. Does not corrupt any number by itself. Must be neutralized by a dated caveat in the same pass as the C1 repoint.
- **Fix:** do NOT edit the frozen file (immutable provenance; sha-ledgered). Record a dated provenance caveat (RUN_STATUS entry + `provenance/CAVEAT_canonical_plotter_source_2026-10-19.md` beside the release).

**R3. `plot_uncertainty_pth_panels.py`: triple-dead wrong-era template, registered as `CURRENT_DIAGNOSTIC_OR_PROVISIONAL`, with a latent binning-language-ruling violation.**
- **File/lines:** `plotting/plot_uncertainty_pth_panels.py` — `:53-59` (inputs `HERE/data/data_side_systematics_rev6_v2/...`, `HERE/data/per_bin_sigma_rev6_v2/...` — neither exists), `:72-90` (8 cells × 6 z × 5 pT = 240 bins/charge, "Scheme B′, HQ-v5g-Bprime"; `X_EDGES` S3: `[0.021, 0.087, 1.5]`), `:93-100` (`cell_phys_label` renders `x∈[0.087,1.5)` for the top cell).
- **Evidence:** triple-dead (snapshot-local inputs absent; docstring-intended inputs absent; targets the extinct rev6_v2/v5g-Bprime 240-bin era). SEVEN_CATEGORY_SCRIPT_INDEX.md:76 marks it `CURRENT_DIAGNOSTIC_OR_PROVISIONAL`. Plotting logic itself sound where it applies (charge map correct; relative uncertainties width-free; Y_CAP with on-canvas off-scale annotations; nonpositive bases skipped and counted). But `cell_phys_label(7)` publishes the internal container edge 1.5 as binning — violates the 2026-10-16 binning language ruling if resurrected.
- **Class/impact:** PROVENANCE (registry asserts currency for a dead, wrong-era template) + latent DISPLAY. Not blocking (cannot run).
- **Fix:** annotate the registry entries as `NOT RUNNABLE / historical rev6_v2-era (inputs removed)`; optionally quarantine.

### MINOR

**R4.** Dead r1-plotter copy inside the fitnorm_r2 tree (r1-named inputs absent there, no pre-flight; traceback crash). Confusion risk only; true r2 plotter is `make_fitnorm_r2_plots.py`. Fix: remove/annotate; add pre-flight guard if kept.

**R5.** Fitnorm plotters: Poisson error floor `sqrt(max(d,1))` (overstates at d=0, never understates) and log-scale floor 0.4, undisclosed display conventions. Ratio errors data-side only; ratio orientation matches label. Display-only; acceptable for internal diagnostics.

### Confirmed clean (residual coverage)

1. Fitnorm r1 plotter end-to-end conventions: `SCALE_RG=0.03843251824641288` / `SCALE_DJ=0.03346843073584302` = 351000.0/9132891.0 and 351000.0/10487495.0 identical in plotter/fit/merge; single application; charge axes consistent extract→merge→fit→plot; class axes consistent; observable axis edges identical; fit-JSON group keys match; μ factors applied to the same scaled templates; no density normalization (no bin-width/x-cap defect possible); no ratio inversion.
2. Fitnorm r2 plotter: same conventions verified; `SCALE_LQ=0.08013817556653238` identical to the r2 fit; honest labels on canvas.
3. `assemble_stat_sidecar_cone.py` C2 plot block: renders the exact matrices just written (diagonal==1 asserted); correct per-charge titles; display-only, correct by construction.
4. Clean-release tree sweeps: no unexpected plotting code anywhere.
5. Luminosity/stale-constant sweep: no 297657.8 / 356613 / 126880 hits; every scale derives from L=351000.0 nb⁻¹.
6. P-XVOL cap on both live canonical-plotter copies; the only uncapped instances remain C2/M5 and the R2 frozen archive copy.

## Part B — final adjudication (loops 1 + 2 + 3)

### Consolidated defect register

| # | Defect | Source | Class | Blocks final-basis figures? | Exact fix |
|---|---|---|---|---|---|
| C1 | Registered canonical Born plotter (remote snapshot) reads superseded 2026-10-13 cone Born release + superseded `total_with_cone15_rel` budget | L1/L2 | **PLOT-NUMBER** (+PROVENANCE) | **YES** | Repoint per plan below |
| C2 | bgsub-diagnostic plotter uncapped dx (line 255): every S3_x0 panel point ×0.662 | L1/L2 | **PLOT-NUMBER** | YES | `df['dx'] = np.minimum(df['x_max'], 1.0) - df['x_min']` |
| R1 | Local-mirror registry copy runnable on an even staler basis (2026-10-09 + crad13); false md5-identity claim | L3 | **PLOT-NUMBER** (+PROVENANCE) | **YES** | Same repoint on the mirror (or de-register) + correct the registry claim |
| M1 | Canonical plotter manifest describes a computation the code no longer performs | L1/L2 | PROVENANCE | Rider | Rewrite `plotted_yerr_source`, counts/formulas (`:229,232,199,46,665,699,716-717`) |
| M2 | CRAD role/decision/caveat strings mislabel the applied primary (`DJANGOH14_relbin` → "QUARANTINED") | L1/L2 | PROVENANCE | Rider | Rewrite role map + decision/caveat strings (`:219-224,756-763,992-999`) |
| M3 | xQ2 manifest names the 2026-10-06 release, not the input read | L1/L2 | PROVENANCE | Rider | Derive `input.release` from the actual `IBU_CSV` path (`:948`) |
| M4 | bgsub-diagnostic plotter dead as instantiated | L1/L2 | PROVENANCE | Rider (with C2) | Absolute-`RELEASE_DIR` repoint + pre-flight |
| M5 | bgsub x-Q² map draws the S3 rectangle to x=1.5 | L1/L2 | DISPLAY | No | `x_hi = min(float(row["x_max"]), 1.0)` (`:644-645`) |
| M6 | `plot_perbin_7col.py`: laptop-only paths + conditional h+/h− label inversion | L1/L2 | PROVENANCE (+latent DISPLAY) | No | NAF paths + fix `charge_label` or certify the input convention |
| M7 | Data-vs-MC chain: validated instance superseded-basis; no 2026-10-17-basis instance; stale caveat string | L1/L2 | PROVENANCE | No | Regenerate on Born clean-release tables (or labeled raw side) + rewrite caveat |
| B-M1 | RUNLIST/SEVEN_CATEGORY declare two `ibu_crossclosure` plotters absent from disk | L2 | PROVENANCE | No | Restore + loop review, or strike/annotate as NOT INCLUDED |
| R2 | Final-basis Born release ships frozen plotter source with ruled-out uncapped dx, stale paths, `no_bgsub` naming; no caveat | L3 | PROVENANCE | Rider (same pass as C1) | Dated provenance caveat; do NOT edit the frozen file |
| R3 | `plot_uncertainty_pth_panels.py` dead wrong-era template marked current; container-edge label latent | L3 | PROVENANCE (+latent DISPLAY) | No | Annotate registry entries NOT RUNNABLE / historical |
| B-m1 | Only covariance plotter targets the superseded v4 package; none for the 2026-10-19 clean-release package | L2 | PROVENANCE (minor) | No | Dated clone when covariance figures needed |
| m1 | No refuse-list for superseded/quarantined CRAD_TABLE_KEYs | L1 | PROVENANCE (minor) | No | Add refuse-list |
| m6 | `plot_perbin_7col.py` forced ylim partial disclosure + phantom `SCRIPT_OUT` | L1 | PROVENANCE (minor) | No | Drop phantom manifest entry |
| R4 | Dead r1-plotter copy inside fitnorm_r2 tree | L3 | PROVENANCE (minor) | No | Remove/annotate; optional pre-flight |
| m2 | 0-suppression disclosed off-canvas only | L1 | DISPLAY (minor) | No | Acceptable under style rule; noted |
| m4 | Per-panel ratio figures can hide off-scale points with no count | L1 | DISPLAY (minor) | No | Add clipped-point counter |
| m5 | regen fixed ylim no disclosure; NaN step-bridging; unrecorded denominator-key choice | L1 | DISPLAY (minor) | No | Record key; add clipped counter |
| B-m2 | plot_covariance floors diagonal at 1e-12 without on-canvas disclosure | L2 | DISPLAY (minor) | No | Keep; optional footnote |
| B-m3 | MAPTMD overlay fixed ylim (0,3.1) can clip; sidecar discloses | L2 | DISPLAY (minor) | No | Optional per-panel clipped counter |
| R5 | Fitnorm Poisson floor + log floor 0.4, undisclosed, conservative | L3 | DISPLAY (minor) | No | Optional on-canvas note; keep |
| m7 | Integrated C_rad gate tolerance ~3% headroom over serialization residues | L1 | Robustness | No | Widen to ~1e-11 relative or document the bound |

### Recommended repoint plan for the registered canonical Born plotter (C1, covering R1)

**Targets (both registry copies):**
- Remote snapshot: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (lines ~106, ~112-114, ~115-120).
- Local mirror: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_nominal_release351_cRadApplied.py` (lines 94, 100-102, 103-107).

1. `BORN_ROOT` → `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_bgsub_setRprime_conebase_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-15` (IBU h+ `11.996558` / h− `11.663956` nb).
2. **Budget column — RECORDED USER DECISION REQUIRED (quoting-rule contradiction):** the 2026-10-17 install record pins `total_with_bgsub19_rel` (`systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17/bgsub_setrprime_budget19_cleandenom.csv`); STATE.md's 2026-10-19 Env100 Systematics Freeze (INSTALL COMPLETE, dated later) makes `total_with_env100_rel` (`systematics_bgsub_env100_21comp_2026-10-19/bgsub_env100_21comp_budget.csv`) the quoted total and marks bgsub19 "superseded as the quoted systematic, retained for provenance" (medians: bgsub19 h+ `5.9764%`/h− `6.2968%` vs env100 h+ `7.3203%`/h− `7.6371%`). **Recommendation:** repoint `BUDGET_*` to the env100 CSV with `BUDGET_COLUMN = "total_with_env100_rel"` (that CSV carries BOTH columns, 490 rows, charge labels pos/neg matching the plotter join), record the rule choice in the plotter manifest; if the user pins the 2026-10-17 rule instead, set `total_with_bgsub19_rel` and annotate the env100 supersession. Either way update `BUDGET_COMPONENTS` to the chosen budget's active inventory and rewrite the M1 strings.
3. **RAPGAP31_relbin alternative-scheme subdir:** the clean Born release has NO `alternative_scheme_rapgap31_relbin_emfix/` subdirectory (verified) — the plotter's RAPGAP31_relbin branch would resolve to a nonexistent path after repoint. Options: (i) **recommended now** — make that branch abort with an explicit message; (ii) build/gate/install the subdir only if alternative-scheme figures are actually required.
4. **Same-pass riders:** M1/M2/M3 string rewrites; R1 mirror repoint + restoration of the cross-copy md5-identity claim (or de-registration); R2 dated provenance caveat beside the Born release; m1 refuse-list optionally.

### Totals

- **By severity (all loops):** FATAL/CRITICAL **2** (C1, C2) · MAJOR **11** (M1–M7, B-M1, R1, R2, R3) · MINOR **11** — **24 confirmed defects** (loop-1 m3 folded into M7; info items excluded).
- **By class:** PLOT-NUMBER **3** (C1, C2, R1) · PROVENANCE **13** · DISPLAY **7** · robustness **1** (m7).
- **Loop-3 residual contribution:** CRITICAL 0 · MAJOR 3 (R1, R2, R3) · MINOR 2 (R4, R5).

### Overall verdict

**FAIL.** No plotting script in the workspace can currently produce final-basis (2026-10-17 clean release) figures: the registered canonical plotter and its mirror both read superseded bases/budgets (C1, R1), and the only bgsub-capable variant is both dead-as-instantiated (M4) and S3-normalization-defective (C2). The residual sweep found no new physics-number defect in the live 2026-10-17 fitnorm diagnostics.

### Minimal fix set required before any final-basis figure may be produced

1. **C1 repoint (both copies)** — `BORN_ROOT` → the 2026-10-15 clean Born release; `BUDGET_*` → recorded env100-vs-bgsub19 decision; RAPGAP31_relbin branch → fail-loudly.
2. **R1** — mirror repointed identically (or de-registered) and the cross-copy identity claim restored/corrected.
3. **M1+M2+M3** — manifest/provenance string rewrites riding the repoint.
4. **C2 (+M4+M5)** — one-line dx cap at bgsub-variant line 255, absolute-path repoint, x-Q² rectangle cap — one pass, only if the variant stays registered.
5. **R2** — dated provenance caveat recorded beside the clean Born release; frozen file NOT edited.
6. **R3** — registry annotation: `plot_uncertainty_pth_panels.py` NOT RUNNABLE / historical rev6_v2-era (follow-up).
Items 1–4 hard-block final-basis figure production; item 5 must accompany the first final-basis shipment; item 6 is a follow-up registry repair. B-M1 remains a strongly recommended companion fix.

## Missing evidence (carried)

- The two declared `ibu_crossclosure` plotter files (B-M1) — absent from disk.
- `plot_perbin_7col.py` input CSV charge-column convention (M6b) — laptop file.
- The M7/overlay regeneration plan — unchanged.
- A clean-release covariance plotting driver (B-m1) — none exists for the 2026-10-19 package.
- The env100-vs-bgsub19 quoted-total decision for the repointed band — user decision required; both CSVs verified on disk with both columns present in the env100 CSV.

## Recommended next check

Execute the C1 repoint exactly as specified (both registry copies, recorded budget-column decision, fail-loud RAPGAP31_relbin branch, M1–M3 string riders, R2 caveat note), then re-run the plotter's own pre-flight/assert suite against the clean Born release CSV and the chosen budget CSV (490-row join, per-charge 245/245, kinematic-edge asserts) and submit the repointed script plus its first rendered final-basis 7-panel/x-Q² products to a focused re-review before any figure is referenced externally. The single most decisive validation: confirm the repointed plotter's integrated Born totals reproduce h+ `11.996558` / h− `11.663956` nb and that its S3_x0 densities use dx = 0.979.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic, loop 3 of 3 (plotting scripts; residual coverage + final adjudication), read-only; no analysis script was modified.
- Files audited this loop: 7 primary targets + 6 convention-support scripts + registry/release/budget artifacts; loops 1–2 findings accepted as double-confirmed and adjudicated.
- Tooling: `sidis-hq2-plotting` skill load failed (tool error, loops 2 and 3); audit proceeded from on-disk evidence. Report persisted by the delegating supervisor session.
