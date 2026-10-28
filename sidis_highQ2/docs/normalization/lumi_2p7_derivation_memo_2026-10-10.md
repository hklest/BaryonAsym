# Derivation memo — flat 2.70% luminosity uncertainty (`lumi_rel`) for the four-period S67 high-Q2 charged-SIDIS target

**Date:** 2026-10-10
**Author role:** `h1-normalization` (luminosity and weight authority for H1 BaryonAsym / charged-hadron SIDIS).
**Purpose:** Close the `lumi_rel` derivation-artifact gap flagged by the provisional-to-final gap audit
(`docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` §6.3, disposition §5, plan item §7 P0-1:
"Lumi derivation memo (§6.3) → `lumi_rel` final"). Documentation-only: **no value is changed, no product is
regenerated, no rerun is launched.**
**Scope:** H1 high-Q2 inclusive charged-hadron SIDIS (`sidis_highQ2/`, v6/z005) only. This value is
regime-specific to the four-period S67 high-Q2 target and must not be exported to any other analysis in
this workspace; other analyses carry their own normalization contexts.

---

## 1. The value

| Property | Statement |
|---|---|
| Component | `lumi_rel`, component #9 of the canonical 13-component z005 systematic budget |
| Magnitude | **Flat 2.70%** — `lumi_rel = 0.027`, identical in all 490 published bins (245 h+ + 245 h−) |
| Normalization it applies to | `L_data = 351000.0 nb^-1 = 351 pb^-1` — the current four-period S67 target luminosity for high-Q2 charged-SIDIS, **including ep0304** (periods ep0304 + em0405 + em06 + ep0607; 658-file data vector, per-period file counts 83 / 152 / 100 / 323) |
| Correlation model | **Fully correlated across all bins and both charges, rank-1**: `C_ij = (0.027 sigma_i)(0.027 sigma_j)`; in Tier-A covariance assembly `C_lumi = l l^T` with `l_i = 0.027 x_i` (spec §4.2). Rank-1 here is by construction — a single nuisance parameter scaling the absolute normalization — not the Tier-B measured-coherence kind that requires a noise-floor test. |
| Observable inclusion | **Absolute cross sections only.** Excluded from multiplicities, from the charge asymmetry `A_h` ("Luminosity is excluded exactly", spec §4.3), and from azimuthal moment/asymmetry observables constructed as ratios, where the common luminosity normalization cancels exactly (spec §4.4; frozen menu row 20: "applies only to absolute cross sections, not to multiplicities or `A_h`"; "cancels in `A_h`"). |
| Propagation type | Analytic, one magnitude; included in quadrature in the budget total (`total_with_crad13_rel`) |
| Disjointness | `lumi_rel` is **disjoint** from `c_rad_stat_rel`; `L_data` does **not** enter the C_rad ratio (`C_rad = sigma_nonrad/sigma_rad` uses per-leg MC luminosities only) and does **not** enter any budget arithmetic — the budget is relative-only (normalization gate 2026-10-09, checks 1–2; `STATE.md`; `docs/RUN_STATUS.md`). |

## 2. External anchor

The 2.7% magnitude is the **H1 standard luminosity uncertainty for this dataset**, anchored to:

> V. Andreev et al. (H1 Collaboration), "Measurement of groomed event shape observables in
> deep-inelastic electron-proton scattering at HERA", *Eur. Phys. J. C* **84** (2024) 718,
> arXiv:2403.10134 (v2, DESY-24-036), DOI 10.1140/epjc/s10052-024-12987-0 — **§6.3**.

Applicability of the anchor: the Andreev 2024 groomed-event-shapes measurement is the most recent H1
high-Q2 TUnfold systematics statement and was performed **on the same 351 pb^-1 HERA-II (2003–2007)
dataset** used by this analysis (recorded in-workspace in
`docs/research/research-tracking-systematics-2026-05-25.md` §2 table: "the same dataset (351 pb⁻¹,
2003–2007) we are using"). Its Sec. 6 uncertainty list includes **luminosity 2.7%** (same research
record; also `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md`:
"arXiv:2403.10134 — … JES + RCES ±1%, HFS θ alignment 1 mrad, **lumi 2.7%**. Most recent H1 TUnfold
high-Q² systematics statement.").

**Externally anchored, not in-workspace:** the publication itself (PDF/HTML) is not stored in this
workspace. The in-workspace research notes that read it on ar5iv (2026-05-25) quote its §§6.1, 6.2, and
6.4 verbatim and record the Sec. 6 uncertainty list including "luminosity 2.7%", but **no verbatim
quotation of the §6.3 text exists in-workspace**. The §6.3 subsection attribution is carried by the
frozen v6sys menu (rows 20 and source-inventory table: "2.7% (HERA-II combined) — Andreev 2024 §6.3")
and by the definition spec (row 14). This memo therefore records the anchor as **externally anchored**
per existing provenance and does not invent precision beyond what the in-workspace records carry.

## 3. Internal ratification trail (chronological)

1. **2026-06-03 — inventory + caveat.**
   `docs/methodology/v6sys_h1_precedent_systematics_manifest_2026-06-03.md` §2.4 indexes `lumi` →
   `data/systematics_sources/source_lumi.json` (2.7%), status
   `PRESENT_INDEXED_PENDING_NORMALIZATION_REVIEW`, with the explicit caveat that "`h1-normalization`
   must sign off whether the 2.7% HERA-II combined value applies to the v6sys high-Q² release as-is.
   No revised magnitude is invented here." The same date, the frozen menu
   (`docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md`, row 20) freezes
   the magnitude: "**2.7%** (HERA-II combined) — Andreev 2024 §6.3", category D (normalization, handled
   outside v6sys), status DONE, "applies only to absolute cross sections, not to multiplicities or
   `A_h`".
2. **2026-06-13 — flat-assignment artifact (origin of the budget column).**
   The base budget build provenance
   `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/provenance.json`
   records verbatim: `"lumi_label": "luminosity 2.7% flat"`, `"luminosity 2.7% flat": "lumi_rel = 0.027
   for all 490 bins"`, `"lumi_rel": 0.027`. This is the artifact the gap audit dates the component to
   ("produced 2026-06-13 (flat assignment)").
3. **2026-09-30 — definition spec ratifies the definition.**
   `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` register row 14 `lumi_rel`:
   "Flat 2.7% absolute-normalisation uncertainty; Andreev 2024 §6.3 and current v6 bookkeeping.
   Analytic, one magnitude. Fully charge- and bin-correlated rank-1: `C_ij=(0.027 sigma_i)(0.027
   sigma_j)`. **IMPLEMENTED** for absolute cross sections only. Exclude from multiplicities and charge
   asymmetries." Tier-A prescription §4.2: `C_lumi = l l^T`, `l_i = 0.027 x_i`, "only for absolute
   cross sections".
4. **2026-09-23 → 2026-10-09 — normalization-basis repair and gate.**
   The 2026-09-23 normalization-basis repair restored the authoritative `L_data = 351000.0 nb^-1`
   constant after a 2026-06-14 producer rerun had silently reintroduced the stale 2005-2007-only
   `297657.8 nb^-1` into nb-space payload fields (record:
   `docs/systematics/covariance_package_z005_2026-09-23.md`). Relative components, including `lumi_rel`,
   were invariant under this repair (regime-B invariance). The **normalization gate**
   `docs/status/normalization-gate-releasebin-2026-10-09.md` (gating authority: `h1-normalization`,
   i.e. precisely the sign-off role named in the 2026-06-03 caveat) then verified, verdict **PASS WITH
   CAVEAT**: check 2 — "Budget candidate: relative-only product; `lumi_rel = 0.027` (2.7% flat) carried
   byte-identical; `L_data` does not enter any budget arithmetic"; check 1 — `L_data` is outside the
   C_rad ratio; check 4 — forbidden-lumi sweep found **zero** references to superseded data lumis
   `297657.8` / `356613` / `126880` anywhere in the candidate set. The caveat that "absolute
   normalization of any Born quote inherits the manifest-level exposure uncertainty until `P-CRAD-LUMI`
   closes" concerns the **MC exposure provenance of C_rad**, not the data luminosity uncertainty, and
   is carried separately.
5. **2026-10-10 — gap audit and budget derivation record.**
   `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md`: component 9 `lumi_rel` — verdict
   **VALID** ("normalization-only; chain-independent; 'L_data does not enter any budget arithmetic'"),
   with the final-gate caveat "derivation-artifact gap (§6.3); rank-1, excluded from
   multiplicities/`A_h`"; §5 lists `lumi_rel` as promotable **without reruns** ("documentation-only
   (§6.3)"); §7 makes this memo P0 item 1. The companion record
   `docs/systematics/budget_13components_derivation_2026-10-10.md` (row 9 and provenance-thinness flag
   1) states: "budget provenance records only the flat assignment (`lumi_rel = 0.027 for all 490
   bins`); no in-workspace derivation artifact exists. The value is externally anchored (H1 publication
   standard for the same 351 pb⁻¹ dataset, Andreev 2024 §6.3 as cited in the definition spec)."
6. **2026-10-10 — this memo.** Closes the documentation gap. With this memo, the disposition choice
   recorded in gap audit §6.3 ("request a short normalization-owner derivation note") is executed;
   the alternative ("formally accept the precedent citation as the derivation record") is moot.

## 4. Where the 2.7% enters the systematics budget

- **Budget component #9 `lumi_rel`** of the canonical 13-component budget
  `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/`
  (canonical since the 2026-10-10 c_rad baseline promotion). Verified directly for this memo: the CSV
  `total_uncertainty_summary_with_phih_coherentprior_b1floor_crad13_releasebin_emfix.csv` has
  **490/490 rows with `lumi_rel == 0.027`** (column 17). Medians and maxima are 2.70% / 2.70% for both
  charges (budget derivation record, component maxima list).
- **Quadrature inclusion:** `total_with_crad13_rel = sqrt(sum of 13 components^2)`; current-release
  bookkeeping: "Luminosity component: flat `2.7%`, `lumi_rel = 0.027`, included in quadrature"
  (`docs/status/current-release.md`, Completed Components).
- **Byte-identical carry:** `lumi_rel` was carried unchanged through the coherent-prior swap
  (2026-10-07), both crad13 rebuilds (2026-10-09) and the emfix promotion (2026-10-10) — 12 carried
  components byte-identical, 0/14,700 cell mismatches (gap audit §1; budget derivation record).
- **Covariance package:** Tier A, "rank-one 2.7% luminosity"
  (`docs/systematics/covariance_package_z005_2026-09-23.md`); matrices `cov_lumi.npy` /
  `rel_cov_lumi.npy` in `data/covariance/z005_provisional_v4_2026-09-23/`.
- **Invariance statements:** the relative budget (including `lumi_rel`) is invariant under the
  lumi-constant repair (regime-B invariance, normalization gate 2026-10-09) and under the C_rad Born
  flip — only absolute-nb products rescale (`STATE.md`; `docs/RUN_STATUS.md`).
- **Bookkeeping quotes:** `STATE.md` ("`lumi_rel` (flat 2.7%) and the new `c_rad_stat_rel` are
  DISJOINT") and `docs/RUN_STATUS.md` ("`lumi_rel` flat `2.7%` and `c_rad_stat_rel` are DISJOINT").

## 5. Superseded and forbidden luminosity constants (guard list)

| Value | Status | Note |
|---|---|---|
| `351000.0 nb^-1 = 351 pb^-1` | **CURRENT** | Four-period S67 target incl. ep0304; the only valid denominator for absolute normalization in this analysis (`docs/status/normalization.md`; `STATE.md`; `docs/RUN_STATUS.md`). |
| `297657.8 nb^-1` | **SUPERSEDED** | 2005-2007-only; **excludes ep0304** — wrong for the current four-period target. History: correctly rescaled to 351000.0 on 2026-06-11; a 2026-06-14 producer rerun retained a stale hardcoded `LUMI=297657.8` and overwrote the corrected nb payload; the 2026-09-23 repair restored the authoritative basis (`docs/systematics/covariance_package_z005_2026-09-23.md`). Consequence on record: the superseded 2026-07-05/07-08 two-generator data-vs-MC package used it — its data denominator is low by exactly `297657.8/351000.0 = 0.84802792` in all 490 bins; do **not** quote its `26.7039 nb` total (`docs/handoffs/SESSION_HANDOFF_mc_4generator_comparison_2026-09-25.md` §4). |
| `356613.0 nb^-1 = 356.613 pb^-1` | **SUPERSEDED** | Older direct-monitor quote; not a current default (`docs/status/normalization.md`). Appears historically in background-fraction bookkeeping (content step at 356613.0, then luminosity factor `351000.0/356613.0`); treat artifacts scaled to it as historical/provisional unless corrected. |
| `356000 nb^-1` | **STALE** | Diagnostic shorthand; never authoritative. |
| `126880 nb^-1` | **FORBIDDEN** | Wrong by ~42x; reject on sight. Included in the normalization gate's forbidden-value sweep (zero references in the 2026-10-09 candidate set). |

Validation rule (standing, `docs/status/normalization.md`): before any normalization-sensitive result,
verify both sample identity and effective MC luminosity; if the file path, reqID, period, generator, or
MC-lumi record is ambiguous, stop and resolve provenance before scaling.

## 6. Verification performed for this memo (read-only)

| Fact | Artifact checked | Result |
|---|---|---|
| Flat 0.027 in every bin | Canonical budget CSV `...crad13_releasebin_emfix_2026-10-10/total_uncertainty_summary_with_phih_coherentprior_b1floor_crad13_releasebin_emfix.csv` | 490/490 rows `lumi_rel == 0.027` (direct column check) |
| Flat-assignment origin | `.../total_uncertainty_z005_2026-06-13_with_electron_lumi/provenance.json` | `"lumi_rel": 0.027`, `"lumi_label": "luminosity 2.7% flat"`, `"luminosity 2.7% flat": "lumi_rel = 0.027 for all 490 bins"` |
| Current-release quote | `docs/status/current-release.md` (Completed Components) | "flat `2.7%`, `lumi_rel = 0.027`, included in quadrature" |
| Definition + rank-1 formula + exclusions | `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` row 14, §4.2–§4.4 | As quoted in §1 above |
| Gate verification | `docs/status/normalization-gate-releasebin-2026-10-09.md` checks 1, 2, 4 | PASS; `lumi_rel` byte-identical; `L_data` outside C_rad ratio and budget arithmetic; forbidden-lumi sweep clean |
| Anchor reference identity + same-dataset statement | `docs/research/research-electron-systematics-2026-05-25.md` (§2.1, references), `docs/research/research-tracking-systematics-2026-05-25.md` (§2 table, references), `docs/research/research-HFS-systematic-2026-05-25.md` (§2.1) | EPJC 84 (2024) 718, arXiv:2403.10134, DOI 10.1140/epjc/s10052-024-12987-0, DESY-24-036; "luminosity 2.7%" in its Sec. 6 list; same 351 pb^-1 2003–2007 dataset |
| §6.3 attribution | Frozen menu `docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` rows 20/217/386; spec row 14 | "Andreev 2024 §6.3" carried consistently; verbatim §6.3 text NOT in-workspace (externally anchored) |
| Four-period target lumi | `STATE.md`, `docs/status/normalization.md`, `docs/RUN_STATUS.md`, gap audit §1 | `L_data=351000.0 nb^-1`, four-period S67 incl. ep0304 (658-file vector) |
| Supersession records | `docs/systematics/covariance_package_z005_2026-09-23.md`; `docs/handoffs/SESSION_HANDOFF_mc_4generator_comparison_2026-09-25.md` §4; `docs/status/normalization.md` | As tabulated in §5 above |
| Gap-audit cross-reference | `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` §3 row 9, §5, §6.3, §7 P0-1; `docs/systematics/budget_13components_derivation_2026-10-10.md` row 9 + flag 1 | As quoted in §3 |

## 7. Discrepancies, residual caveats, and status

1. **Provenance-path discrepancy (minor, does not affect the value).** The 2026-06-03 methodology
   manifest and frozen menu index the lumi source as
   `data/systematics_sources/source_lumi.json` (status `PRESENT_INDEXED_PENDING_NORMALIZATION_REVIEW`,
   σ_rel = 0.027). That file is **not present** at the recorded path as of 2026-10-10
   (`data/systematics_sources/` contains only the `tracking_central_h1_1302_1321/` and
   `v6sys_h1_precedent_manifest/` README subtrees). The operative flat-assignment artifacts are the
   2026-06-13 budget provenance JSON and the budget CSVs themselves, both verified above. The indexed
   filename should be treated as an unreferenced inventory entry, not as a resolvable artifact.
2. **Externally anchored precision.** The 2.70% magnitude, its Andreev 2024 Sec. 6 attribution, the
   same-dataset applicability, and the §6.3 subsection number are all recorded in-workspace, but the
   publication's verbatim §6.3 text is not in-workspace. Anyone re-deriving this value from the
   publication directly should confirm the §6.3 wording; no in-workspace record contradicts the
   attribution.
3. **What this memo does and does not close.** It closes the `lumi_rel` derivation-artifact gap
   (gap audit §6.3 / P0-1) — the component is documentation-complete and final-eligible per gap audit
   §5 ("documentation-only"). It does **not**: make the 13-component budget publication-grade (the
   spec census remains IMPLEMENTED 4 / PROVISIONAL 6 / BLOCKED 9 / DROPPED 5; one-sided detector legs,
   `phi_h` disposition, B1 follow-up, and the BLOCKED rows remain open); close `P-CRAD-LUMI` (MC
   exposure provenance of C_rad, generator-log-open); alter the `trigger_rel` ≤0.5% placeholder; or
   touch any central product, budget CSV, or covariance matrix.
4. **No rederivation required.** Per the gap audit, zero of the 13 components are STALE by chain
   drift; `lumi_rel` is normalization-only and chain-independent, and no analysis change since
   2026-06-13 can affect a flat relative constant that does not enter any budget arithmetic.

## 8. In-workspace references

- `sidis_highQ2/STATE.md` (2026-10-10); `sidis_highQ2/docs/RUN_STATUS.md`;
  `sidis_highQ2/docs/status/current-release.md`; `sidis_highQ2/docs/status/normalization.md`;
  `sidis_highQ2/docs/status/normalization-gate-releasebin-2026-10-09.md`
- `sidis_highQ2/docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` (row 14, §4)
- `sidis_highQ2/docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` (§3 row 9, §5, §6.3, §7)
- `sidis_highQ2/docs/systematics/budget_13components_derivation_2026-10-10.md` (row 9, flags)
- `sidis_highQ2/docs/systematics/covariance_package_z005_2026-09-23.md`
- `sidis_highQ2/docs/methodology/v6sys_h1_precedent_systematics_manifest_2026-06-03.md` (§2.4)
- `sidis_highQ2/docs/research/research-v6sys-h1precedent-systematics-menu-frozen-2026-06-03.md` (row 20, §refs)
- `sidis_highQ2/docs/research/research-tracking-systematics-2026-05-25.md`,
  `research-electron-systematics-2026-05-25.md`, `research-HFS-systematic-2026-05-25.md`
  (Andreev 2024 reference identity, Sec. 6 list, same-dataset statement)
- `sidis_highQ2/docs/handoffs/SESSION_HANDOFF_mc_4generator_comparison_2026-09-25.md` (§4, 297657.8 supersession)
- `H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_2026-06-13_with_electron_lumi/provenance.json`
  (flat-assignment origin); `.../total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/`
  (canonical budget, 490/490 at 0.027)
- External anchor (not in-workspace): V. Andreev et al. (H1), EPJC 84 (2024) 718, arXiv:2403.10134,
  DOI 10.1140/epjc/s10052-024-12987-0, §6.3.
