# SPEC — RAD-vs-NONRAD truth-level phi_h overlay plots on the corrected v3 trees (2026-10-02)

**Status: SPECIFICATION (implementable). No code was edited and no production was run for this document.**

**Product class:** internal MC-only generator diagnostic. Not a systematic, not central,
not physics-approved, not Born-level, no `C_rad` application. It replaces the WITHDRAWN
v1-based set `sidis_highQ2/plots/phi_h/nonrad_gen_compact_2026-10-02/` (v1 `Q2_es_gen`
wrong-signed formula; audit cluster 5323363: purity ~72%, completeness ~9%, 63% wrong-cell;
full audit: `sidis_highQ2/data/radiative/v1_kincut_audit_2026-10-02/`).

**Goal:** DJANGOH14 RAD overlaid with DJANGOH14 NONRAD, and RAPGAP31 RAD with RAPGAP31
NONRAD, so the QED radiative effect on the truth-level Trento `phi_h` shape is visible, in
all bins, both charges.

---

## 0. Inputs (frozen)

Base: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/`
Tree: `ChargedGenRC` (one row per generated pi/K/p charged-hadron candidate; builder
`RC_treebuilder_genonly_v3.C`, corrected `Q2_es_gen`/`Y_es_gen`/`x_es_gen`; v1 bug values
preserved only in `*_v1bug` branches, never to be read).

| Sample key | Directory (both beams) | Files | Verified count 2026-10-02 |
|---|---|---:|---|
| `djangoh14_rad` | `rad/{ep,em}/*.root` | 1373 | 731 + 642 (verified) |
| `djangoh14_nonrad` (pooled) | `nonrad_old/{ep,em}/*.root` + `nonrad_new/{ep,em}/*.root` | 1580 | 520 + 1060 (verified) |
| `rapgap31_rad` | `rapgap_rad/{ep,em}/*.root` | 1273 | 691 + 582 (verified) |
| `rapgap31_nonrad` | `rapgap_nonrad/{ep,em}/*.root` | 520 | 264 + 256 (verified) |

All 23 branches required by this spec (the 12 plotter branches plus the gate witnesses
`Q2sGen, YsGen, XsGen, Q2Gki, gki_valid, RunNumber, EventNumber, IsGtr, Mother2,
GenEta_lab, xFGen`) were verified present in one representative file of each of the five
sets. The plotter must hard-fail on any missing branch (existing behaviour) and on any
file-count deviation (new, gate G0).

Existing plotter to fork (do not edit in place; it is provenance of the withdrawn set):
`H1ChargedSidis_HighQ2/scripts/plotting/phi_reco_luminorm_2026-06-12/plot_phi_reco_luminorm_v6_compact.py`.
Fork name: `plot_phi_rad_nonrad_gen_v3_compact.py` in the same directory.

Output directory:
`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/phi_h/rad_nonrad_gen_v3_<YYYY-MM-DD>/`
containing `manifest.json`, per-charge PDFs, `png_plus/`, `png_minus/`, `condor/` provenance.

Execution: this is a ~4,746-file, ~1e10-row pass — **Condor only** (AGENTS.md policy). The
audit's `condor/` wrapper pattern (cluster 5323363, uproot+numpy workers) is the reference
template. Login node: single-file `--max-files 1` smoke only. Run mechanics are delegated
to `h1-script-runner` / `h1-naf-operator`.

---

## 1. Observable definition and page layout (item 1)

**Observable.** For each sample `s` in {DJ RAD, DJ NONRAD, RG RAD, RG NONRAD}, charge
`c` in {h+, h-}, (Q2,x) cell `i` (7 cells), z bin `j` (7), pT_h bin `k` (5):

```
N_s,c,i,j,k,m = sum over selected hadron rows in bin m of (weight1 * weight2 * scale_s)
```

with 16 `phi_h` bins `m` on [-pi, pi] (Section 2). This is the same weighted-count
definition as the withdrawn set ("weighted gen hadrons / bin"), now on the corrected
basis, with per-sample luminosity scales (Section 5). Per-panel content is therefore
weighted counts; the *rendered* distribution panels additionally show each curve divided
by its own panel integral (unit area) because the product is a SHAPE comparison; the raw
panel integrals and their uncertainties are recorded in the manifest.

**Recommended layout (one layout, no alternatives shipped):**

- **Distribution pages** — one page per (Q2,x) cell per charge: the existing 7 (z rows)
  x 5 (pT columns) panel grid, each panel with all **4 curves** as staircase lines.
  - Visual encoding: colour = generator (DJANGOH `#0072B2`, RAPGAP `#D55E00`, unchanged
    palette); linestyle = radiative mode (NONRAD solid, RAD dashed). A 2x2
    colour-x-linestyle code survives greyscale printing and small panels.
  - Page 0 of each distributions PDF = one **cell-integrated projection page** (7 panels,
    phi_h summed over all z,pT in the cell), labelled "projection — not part of the
    245-bin scheme". This is where the (small) QED shape effect is most visible.
- **Ratio pages** — a second PDF per charge with the same 8-page structure, each panel
  showing two shape-ratio curves with 1-sigma bands:
  `R_DJ(phi) = f[DJ RAD](phi) / f[DJ NONRAD](phi)` and
  `R_RG(phi) = f[RG RAD](phi) / f[RG NONRAD](phi)`, where `f` is the unit-area curve.
  **Yes to the RAD/NONRAD ratio panel** — but as separate pages, not as per-panel
  sub-axes.
- Total: 8 distribution + 8 ratio pages per charge, 32 pages for both charges.

**Justification against the 7x5 panel density.** Four staircase curves x 16 bins is the
legibility ceiling at the current panel size (~2.2 x 1.5 inch); adding per-bin error bars
(4x16 = 64 bars/panel, >2,000/page) makes the panels unreadable, so uncertainty display
is moved entirely to the ratio pages (2 curves + 2 bands/panel). Rejected alternatives:
(a) per-panel twin ratio sub-axes (70 axes/page; sub-axes fall below legible tick size);
(b) one page per sample pair (4x page count, and cross-sample comparison would require
page flipping, defeating the overlay purpose); (c) no ratio pages (the RAD/NONRAD shape
contrast — the actual physics message — would have to be read by eye from four
near-coincident staircases, which is exactly what failed at 2-curve density already).

---

## 2. phi_h convention and the exact wrap rule (item 2)

**Stored convention (verified in `RC_treebuilder_genonly_v3.C` `safePhi`).** Trento
`phi_h` in the gamma*p CM: `q = k_in - k_out` (incoming beam electron along -z, generated
scattered electron), `u = q_hat`, `nl = k_in x u`, `nh = u x pT_h`;
`phi = atan2((nl x nh).u, nl.nh)` in (-pi, pi], then **`if (phi < 0) phi += 2*pi`**,
returned as `float` -> stored `Float_t phi_h` on **[0, 2pi]** in float32. Degenerate
geometry -> sentinel `-999` (finite! `np.isfinite` does NOT remove it). Known edge
population: **~1.6-2.3% of rows stored exactly at float32(2pi)** (given, verified on the
v3 basis); these are Trento angles at azimuth ~0 from the negative side, collapsed onto
the float32 grid by the builder's `p<0 -> p+2pi` mapping. ~0.0000% negative entries on
the audited nonrad sets.

**Exact wrap rule (normative).** Let `TWOPI = np.float64(2.0*np.pi)`,
`PI = np.float64(np.pi)`.

1. Widen: `phi = phi_h.astype(np.float64)` (exact, no precision change).
2. Sentinel/validity: require `np.isfinite(phi) & (phi >= 0.0)`; rows failing this are
   excluded **explicitly** and counted in the manifest (`phi_sentinel` class). Valid
   stored range is [0, 2pi]; `-999` is excluded here, not by binning underflow.
3. Wrap: `phi_sym = np.where(phi >= PI, phi - TWOPI, phi)` — comparison and subtraction
   **in float64**. All values in (pi, 2pi] map to (-pi, 0]; values in [0, pi) unchanged.

**Behaviour at the float32 edges (the required semantics):**

| stored value | float64 value | wrapped | assigned bin |
|---|---|---|---|
| `float32(2*pi)` = 6.2831854820251465 (= 2pi + 1.7484555e-07) | 6.2831854820251465 | **+1.7484555e-07** | `[0, +pi/16)` (bin 8) |
| `float32(pi)` = 3.1415927410125732 (= pi + 8.7422777e-08) | 3.1415927410125732 | -3.1415925661670128 | `[-pi, -pi+pi/8)` (bin 0) |
| 0.0 or -0.0 | 0.0 | 0.0 | `[0, +pi/16)` |
| any v in (pi, 2pi) | v | v - 2pi in (-pi, 0) | corresponding bin |

**Treatment of phi == 2pi exactly (normative statement):** a row stored at
float32(2pi) is the azimuth 0 approached from below (2pi = 0 mod 2pi). It is **kept**,
wraps to +1.75e-07 rad, and lands in the central-right bin `[0, +pi/16)`. It must never
be (a) dropped, (b) assigned to the last bin `[pi-pi/8, pi)`, or (c) wrapped to -pi. All
three failure modes are excluded by the float64 comparison: since every float32 value
that is >= pi is strictly > float64 pi (float32(pi) = pi + 8.7e-08), the branch always
fires for the edge values. Conversely, **comparing in float32** (`phi >
np.float32(np.pi)`) would leave float32(pi) unwrapped and the subsequent digitize would
silently drop it (out-of-range bin index); a float32 comparison is therefore
non-conforming. Using `>=` instead of the current plotter's `>` additionally closes the
(unrepresentable-from-float32, but defensive) exact-float64-pi case by mapping it to
-pi (bin 0) instead of dropping it.

**Bin edges (normative).** `PHI_BINS = np.linspace(-np.pi, np.pi, 17)` in float64 —
16 bins of width pi/8, edges `e_k = -pi + k*pi/8`, k = 0..16. Assignment with
`np.digitize(phi_sym, PHI_BINS) - 1`. Every valid wrapped value lies in (-pi, pi) and no
valid row can produce an out-of-range digitize index, so **zero rows may be lost to phi
binning**; this is asserted, not assumed (gate G2). Bin centres `0.5*(e[:-1]+e[1:])`.

---

## 3. Truth selection on v3 (item 3)

**Decision: select on the stored truth fiducial flags plus explicit re-application of the
embedded `Q2_es_gen`-family thresholds. Use `Q2_es_gen`/`Y_es_gen`/`x_es_gen` (correct in
v3), NOT `Q2sGen`/`YsGen`/`XsGen`, for selection and (Q2,x) cell digitization. Read
`Q2sGen`/`YsGen`/`XsGen` only as independent witnesses for the purity gate.**

Normative selection, applied identically to all four samples and both charges:

```
pass_truth_event_fiducial  == 1
pass_truth_hadron_fiducial == 1
150 <= Q2_es_gen < 20000        (re-applied; identical to the flag-embedded cut)
0.1 <  Y_es_gen < 0.7           (re-applied)
35  <= GenEmpz   <= 70          (re-applied)
isfinite(Q2_es_gen, x_es_gen, Y_es_gen, GenEmpz, zGen, pT_h, phi_h)
phi_h >= 0                      (excludes the -999 sentinel explicitly)
```

No reco cuts (no RadLength / NHitDedx / alpha / track quantities — none exist in this
tree anyway). No `Q2Gki` domain cut in the nominal selection (see below). Boundary
conventions must match the builder exactly (`>=150`, `<20000`, `>0.1`, `<0.7`, `>=35`,
`<=70`), so the re-application is bitwise-consistent with the flag.

The v3 flag semantics (verified, `RC_treebuilder_genonly_v3.C` lines 84/86):

- `pass_truth_event_fiducial = (Q2_es_gen>=150 && Q2_es_gen<20000 && Y_es_gen>0.1 &&
  Y_es_gen<0.7 && GenEmpz>=35 && GenEmpz<=70)` — exactly the intended fiducial, now on
  the corrected electron-method kinematics.
- `pass_truth_hadron_fiducial = pass_truth_event_fiducial && IsGtr && Mother2==-1 &&
  |GenEta_lab|<4 && isfinite(zGen) && 0<=zGen<=1 && xFGen<0`.
- Row universe = pi/K/p only (`|GenPDG| in {211,321,2212}`), one row per generated
  charged hadron; charge from `chargeSign` (+1/-1).

**Why `Q2_es_gen` and not `Q2sGen` (correlation-based justification):**

1. **Freeze.** The intended fiducial *is* the flag definition; v3 is the corrected
   realization of the same fiducial (the fix was made inside the definition, with the
   v1 values preserved as `*_v1bug`). The withdrawn set's selection semantics are
   thereby retained on a sound basis, and the selection stays consistent with the
   builder's own `nChargedGenSelected` bookkeeping.
2. **Measured equivalence to true kinematics** (audit cluster 5323363, all 2,140 v3
   nonrad files): `corr(Q2_es_gen, Q2sGen) = 1.000000`, `corr(Y_es_gen, YsGen) =
   1.000000`, `log10 corr(x_es_gen, XsGen) = 1.000000`; purity of the flag-based
   selection w.r.t. the `Q2sGen`-based intended condition 99.9983-99.9997%;
   completeness 99.9787-99.9837%; wrong-(Q2,x)-cell <= 0.0065% (RG 0.0015%). The RAPGAP
   raw Pearson x-correlation 0.319 is a pure multiplicative offset from the
   photon-subtracted `s_eff` used in `x_es_gen` (beam energy minus `GenEnPhoton`,
   unconditional in v3), not a shape corruption; cell assignment is unaffected. The
   two branch families therefore define the same sample to <0.03%, far below any
   visible effect, so the choice is a consistency decision — and the frozen definition
   wins.
3. **Convention family.** `Q2_es_gen` is the gen-level analogue of the analysis'
   electron-method (eSigma-family) DIS event selection — the same convention family the
   central response bins in (framework e-Sigma). It keeps the plot's event fiducial
   meaning "events that would pass the analysis' eSigma DIS selection with a perfect
   detector". `Q2sGen`/`YsGen`/`XsGen` are the generator event-record true kinematics —
   the right *witness*, but selecting on them would make the G1 purity gate circular
   (the gate must compare the selection against an independent standard to catch a
   v1-style formula defect).
4. `Q2sGen`-based selection would also require re-implementing the hadron-level
   conditions from raw branches (`IsGtr`, `Mother2`, `GenEta_lab`, `zGen`, `xFGen`) —
   more surface for silent divergence from the frozen definition.

**Q2Gki domain note (no cut, recorded diagnostic).** The FINAL `C_rad` applied a
numerator-only `Q2Gki > 100` to RAD (NONRAD is Q2GT100, RAD is Q2GT60); its in-fiducial
effect is ~0.09% of RAD events (weighted matched/unmatched 0.9993). This spec's nominal
selection does NOT apply it (the intended fiducial does not include it, and the effect
is invisible at plot resolution), but the manifest must record per RAD sample/charge the
fraction of selected rows with `Q2Gki <= 100` or `gki_valid == 0` (expected ~0.09%;
warn-level, not gated). If a strict `C_rad`-replica variant is ever needed, the domain
cut is `Q2Gki > 100` RAD-only per `CRAD_FINAL_PROVENANCE_2026-09-30.md` — out of scope
here.

---

## 4. Matching / migration statement (item 4)

- **Gen-level only.** One row per generated charged hadron; no reco tree exists in this
  schema, no reco-gen matching, no response matrix, no acceptance, no unfolding, no
  event-level join of any kind. Every consumed quantity is a truth quantity.
- **No event double-counting is possible in the `nonrad_old`/`nonrad_new` union,
  provided the following hold** (all enforced):
  1. The two sets are disjoint generation campaigns (OLD = legacy Britzger
     `NORAD.Q2GT100` official production; NEW = validated user campaigns
     `eplusp 20260911` + `eminusp regen 20260916`). The duplicate-RNIN July e- campaign
     (`hk_0123_500x10k_prod_20260708`, 500 files) is quarantined and is not in
     `output_v3/nonrad_new`; the fixed expected count (1060) enforces this.
  2. Input lists are deduplicated by absolute path (the existing `expand_inputs`
     `dict.fromkeys` dedup is retained) and the path-set intersection of the old and
     new lists is asserted empty (gate G0).
  3. **Pool, then scale:** OLD+NEW raw weights are summed into a single histogram
     *first*, and the single pooled scale is applied afterwards — the same pooling rule
     as the FINAL `C_rad` numerator ("pool OLD+NEW by summing raw weights first, then
     dividing by summed exposure"). Scaling the two sets separately and then adding is
     non-conforming.
  4. **Never join or deduplicate by `(RunNumber, EventNumber)` across files.** Keys are
     non-unique across NONRAD files by construction (27.6% of OLD and 61.9% of NEW keys
     repeat across files). A cross-file key join over-counts (the quarantined
     `isr_multiplicity_v1` bug); a cross-file key dedup would under-count. Rows are
     self-contained; no per-file join is needed either (the hadron tree carries the
     event-level branches through the shared event macro).
- Within a file, hadron rows are unique by construction (single pass over the particle
  array, one `Fill()` per pi/K/p candidate).
- **Verification gap statement:** the 2026-10-02 audit's v3 control table covered only
  the nonrad sets. The RAD sets (`rad/`, `rapgap_rad/`) have no prior purity audit; the
  in-plotter gate G1 is their first line of verification — all four samples must pass
  it, or the run is quarantined.

---

## 5. Normalization policy and the scale-factor interface (item 5)

**Policy for the SHAPE comparison:**

- Distribution panels: each of the 4 curves is divided by its own panel integral
  (unit area over the 16 phi bins). Any per-sample constant — including the luminosity
  scale — cancels, so the shape rendering is robust against scale errors; the scales
  still matter for the manifest absolutes and for the sanity gate.
- Ratio pages: `R_g(phi) = f_g_RAD(phi) / f_g_NONRAD(phi)` per generator `g`; this is
  the raw RAD/NONRAD ratio divided by the panel-integral ratio, i.e. a shape ratio with
  mean 1 over populated bins.
- The manifest records per (sample, charge, cell, z, pT): the raw weighted integral,
  its uncertainty, and the per-generator RAD/NONRAD integral ratio (the per-panel
  `1/C_rad`-like quantity under the plot fiducial).

**Scale-factor interface (values deferred to `h1-normalization`):**

- CLI: `--scales-json PATH` — **mandatory** for production. The plotter never computes
  luminosities and carries **no embedded luminosity defaults** (explicit trap: the
  current plotter's `MC_LUMI = {"djangoh14": 10487495.0, "rapgap31": 9132891.0}` and its
  `--scale-to-lumi` flag are the central v6 *response* MC luminosities and are WRONG for
  these RC trees; the fork must not reuse them).
- Schema (fixed keys; `lumi_nb` form preferred, `scale` form allowed):

```json
{
  "provided_by": "h1-normalization",
  "reference_lumi_nb": 351000.0,
  "samples": {
    "djangoh14_rad":     {"lumi_nb": 29740410.0},
    "djangoh14_nonrad":  {"lumi_nb": 29178767.889},
    "rapgap31_rad":      {"lumi_nb": 31997541.0},
    "rapgap31_nonrad":   {"lumi_nb": 27371759.0}
  }
}
```

  The plotter computes `scale_s = reference_lumi_nb / lumi_nb[s]` (or takes `scale`
  directly), refuses missing/non-positive entries, and records the effective scales and
  the file's sha256 in the manifest. `djangoh14_nonrad` must carry the **pooled** value
  (OLD+NEW exposure summed) consistent with the pool-then-scale rule.
- The numbers above are the reference values from `CRAD_FINAL_PROVENANCE_2026-09-30.md`
  (oomclumi, `DIAGNOSTIC_MANIFEST_LEVEL`): DJ RAD ep 16,206,393.0 + em 13,534,017.0;
  DJ NONRAD pooled ep 14,637,492.676 + em 14,541,275.213; RG RAD ep 17,536,474.0 +
  em 14,461,067.0; RG NONRAD ep 13,954,768.0 + em 13,416,991.0 (all nb^-1).
  `h1-normalization` owns confirming or replacing them; `L_data` never enters a
  RAD/NONRAD ratio.
- Unit scales are allowed only behind an explicit `--allow-unit-scales` smoke flag, and
  the manifest must then be stamped SMOKE.
- Degenerate-chunk note: the 17 degenerate NONRAD chunks (C0000032; oomclumi
  `Sigma=0.000, Lumi=-nan`) were excluded from BOTH exposure and histogramming in the
  `C_rad` chain. The plot consumes the tree files present in `output_v3` as-is; the
  resulting numerator/exposure mismatch is bounded by the chunks' ~8448-entry size
  (<=0.01%) and is recorded in the manifest; gate margins (Section 8) dwarf it.

---

## 6. Statistics and uncertainty display (item 6)

- **Accumulate `sumw2` alongside `sumw`** per (cell, z, pT, phi) bin and curve:
  `sumw2 += w_i^2` (the current plotter lacks this; it is a required addition). Per-bin
  variance of a weighted count is `Var[N_j] = sumw2_j`. Effective entries per panel:
  `n_eff = (sum sumw)^2 / (sum sumw2)`.
- **Distribution panels: no error bars and no bands** (4 curves x 16 bins is already
  the density ceiling; 4 overlapping bands are illegible). Uncertainty is quantified in
  the manifest and visualized only on the ratio pages.
- **Ratio pages: 1-sigma shaded bands**, band = `R_j * sqrt(relvar_Aj + relvar_Bj)` with
  `relvar_Xj = sumw2_Xj / (sum raw_Xj)^2` from the raw weighted counts of the two RAD
  and NONRAD samples (independent generations -> no covariance). This is deliberately a
  *shape-only* band: the per-panel integral (normalization) uncertainty is common-mode
  across bins, carries no shape information, and is reported once per panel in the
  manifest instead of inflating every band.
- **Sparse high-z / low-pT panels:** a curve with `n_eff < 50` in a panel is **omitted**
  from that panel (distribution pages) and the corresponding ratio curve is suppressed
  (ratio pages); if all curves of a panel are omitted, the panel shows only a centered
  "low stats" tag. Masked (sample, charge, cell, z, pT) combinations are listed in the
  manifest (gate G5) so reviewers cannot mistake masked panels for empty physics. The
  threshold is a presentation choice and is recorded; it is not applied to the manifest
  integrals (those are always computed from all selected rows).
- Keep the existing `--distribution-ymax-scale` behaviour for headroom; y-label of
  distribution panels: "weighted, unit area"; ratio pages: y-range fixed to [0.5, 1.5]
  with out-of-range clipping recorded, plus a y=1 reference line.

---

## 7. Figure scope: keep the 7z x 5pT grid; what "all bins" means (item 7)

**Keep the current 7z x 5pT grid per (Q2,x) page — do not demote the product to a
phi_h-only projected figure.**

"All bins" shall mean the **published 245-bin per-charge generator scheme** — 7 (Q2,x)
cells x 7 z bins (edges `[0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]`) x 5 pT_h bins (edges
`[0, 0.5, 1, 2, 4, 8]` GeV) — which **is exactly the 7x5x7 grid**: the cell pages of the
existing layout already tile the full published scheme, each bin now split into 16
`phi_h` bins. The projected cell-integrated page is added only as a labeled quick-look
(Section 1), never as a replacement.

Justification: the QED correction is known to have per-bin structure (final `C_rad`
per-bin min/median/max 0.8940/1.1402/1.5301 for RAPGAP; the open "z-rising /
pT_h-falling" residual trend), which a projection would average away; per-bin shape
inspection is the entire point of the overlay. The 245-bin scheme is also the only
binning in which the manifest's per-panel integral ratios can be cross-referenced
against the published per-bin `C_rad` CSVs (soft, non-gated).

---

## 8. Acceptance gates (item 8) — all written by the plotter into `manifest.json`

A run is showable only if `status == "PASS"` (all gates pass). Any gate failure sets
`status = "QUARANTINED"`, and the PDFs must not be circulated. Gate thresholds may be
changed only by `h1-normalization`/`h1-radiative` with a manifest-recorded reason; the
plotter never relaxes a gate silently. Every page carries the footer "INTERNAL
DIAGNOSTIC — MC-only, not physics-approved, not Born-level".

**G0 — Input integrity (hard).** Exact file counts: DJ RAD 1373 (731/642), DJ NONRAD
1580 (520 old + 1060 new), RG RAD 1273 (691/582), RG NONRAD 520 (264/256); zero
duplicate paths; zero old/new path overlap; zero failed files; all required branches
present with expected dtypes; tree `ChargedGenRC`; scales JSON present, parsed,
positive.

**G1 — Kinematic-purity self-check (the anti-v1 gate; hard).** Per sample and charge,
computed in the same pass against the *independent* true-kinematics branches
(`Q2sGen`, `YsGen`, `XsGen`):

- `purity` = selected rows satisfying `Q2sGen>=150 & 0.1<YsGen<0.7` / all selected rows.
  Audited v3: 99.9983-99.9997%. **Gate: >= 99.5%.**
- `completeness` = (selected AND intended) / (all rows satisfying the intended event
  condition AND the recomputed hadron-level conditions `IsGtr==1 & Mother2==-1 &
  |GenEta_lab|<4 & isfinite(zGen) & 0<=zGen<=1 & xFGen<0`) — evaluated over ALL rows,
  not only selected ones. Audited v3: 99.9787-99.9837%. **Gate: >= 99.5%.**
- `wrong_cell_fraction` = filled rows whose (`Q2sGen`,`XsGen`) cell differs from the
  plotted (`Q2_es_gen`,`x_es_gen`) cell. Audited: <= 0.0065%. **Gate: <= 0.5%.**
- Correlation witnesses over selected rows: Pearson `corr(Q2_es_gen, Q2sGen)`,
  `corr(Y_es_gen, YsGen)`, and `corr(log10 x_es_gen, log10 XsGen)`. Audited: 1.000000.
  **Gate: each >= 0.999.**
- Flag-consistency: `pass_truth_event_fiducial == (150<=Q2_es_gen<20000 & 0.1<Y_es_gen<0.7
  & 35<=GenEmpz<=70)` must hold row-by-row. **Gate: 100%.**

The v1 defect (purity 72.2%, completeness 9.2%, wrong-cell 63.3%, corr 0.017) fails
every one of these by 1-2 orders of magnitude, so a v1-style wrong-formula recurrence
can never silently pass. The RAD sets' first-ever purity verification is this gate.

**G2 — 2pi-wrap occupancy check (hard).** Per sample and charge:

- `frac_at_2pi` = selected rows with `phi_h == np.float32(2*np.pi)` (6.2831854820251465).
  Expected ~1.6-2.3%. **Gate: [0.1%, 10%]** (0% means the edge population was lost
  upstream; >10% means a builder-level anomaly).
- `frac_at_pi_exactly` (`phi_h == np.float32(np.pi)`): recorded, no gate.
- `phi_dropped` = selected rows with valid phi that fail to land in one of the 16 bins:
  **Gate: 0.**
- Half-range occupancy: fraction of filled rows with wrapped `phi < 0` — unpolarized
  DIS has cos-only phi structure, so the integrated halves must balance. **Gate:
  [0.40, 0.60].** (The v1 first-set bug rendered an entirely empty negative half ~0% —
  instant quarantine.)
- Central-bin containment: content of bin `[0, +pi/16)` **>=** `n_at_2pi` — the
  exactly-2pi rows must be present in the central-right bin. **Gate: hold.**

**G3 — Selection bookkeeping (hard).** Per sample/charge: `rows_selected` reconciles
with `rows_filled` through explicit, individually counted exclusion classes — phi
sentinel (expected 0 on nonrad; recorded for RAD), out-of-(Q2,x)-cell, pT_h >= 8 GeV
overflow, z out of range (**expected 0**; flag already enforces 0<=z<=1), phi dropped
(**expected 0**). Unexplained residuals quarantine the run. RAD selected rows with
`gki_valid == 0` are counted (expected 0; warn-level).

**G4 — RAD-vs-NONRAD sanity from the final `C_rad` values (hard).** With scaled
integrals over all filled bins, `R_int = (sumW_RAD * s_RAD) / (sumW_NONRAD * s_NONRAD)`
per generator and charge (= `sigma_rad/sigma_nonrad` under the plot fiducial):

Reference (final `C_rad = sigma_nonrad/sigma_rad`, EMPZ>45, `CRAD_FINAL_PROVENANCE_
2026-09-30.md`): DJ h+ 1.1375979129031535, DJ h- 1.1366708715220577; RG h+
1.1328903059596749, RG h- 1.1319063751575906; cross-generator agreement 0.41%.

- **Absolute anchor:** `R_int` in **[0.96, 1.06] x (1/C_rad)**. The asymmetric band
  absorbs the documented EMPZ-window difference — the plot fiducial is `35<=GenEmpz<=70`
  while `C_rad` used `GenEmpz>45`; the DJANGOH v3 EMPZ-mode scan gives
  `C_rad(35-70) ~ 1.1127` vs `C_rad(>45) ~ 1.1372`, i.e. an expected `R_int` ~ 1.022 x
  (1/C_rad) ~ 0.899-0.903 — plus MC statistics and the ~0.09% RAD-domain leak.
  Expected/band per curve: DJ h+ exp ~0.899, band [0.8439, 0.9318]; DJ h- exp ~0.900,
  [0.8446, 0.9325]; RG h+ exp ~0.902 (EMPZ shift unpublished for RG), [0.8474, 0.9357];
  RG h- exp ~0.903, [0.8481, 0.9365].
- **Cross-generator (fiducial-difference-cancelling):** `R_int(RG)/R_int(DJ)` per
  charge in **[0.99, 1.01]**; expected 0.9959 (h+) / 0.9958 (h-) from the 0.41%
  `C_rad` agreement (`= C_DJ/C_RG`).
- **Cross-charge:** `R_int(h+)/R_int(h-)` per generator in **[0.99, 1.01]**; expected
  0.9992 (DJ) / 0.9991 (RG) (the `C_rad` charge difference is 0.08%).
- Soft (recorded, not gated): per-(cell,z,pT) panel integral ratios may be
  cross-referenced against the published per-bin `C_rad` CSVs (`empz>45`) as a
  diagnostic; the fiducial difference forbids using it as a gate.

**G5 — Statistical quality (recorded).** `n_eff` per curve; full list of masked
low-stats (sample, charge, cell, z, pT) combinations; the `frac_at_2pi`, `frac_at_pi`,
sentinel and domain-leak counters of G2/G3.

**G6 — Provenance (hard).** Manifest records: script path + sha256; scales file +
sha256 + effective scales; complete input file lists with counts; all bin definitions;
the selection string; the wrap-rule string; every gate result; `created_utc`; output
paths; `status`. The manifest is the object shown together with the plots.

---

## 9. Known traps (implementer checklist)

1. Never reuse `MC_LUMI`/`--scale-to-lumi` from the old plotter (central response
   luminosities; wrong samples).
2. Never read `*_v1bug` branches; never select on `Q2esGen/YesGen/XesGen` either (that
   is the *framework* e-Sigma family — a different convention from the frozen flags;
   recorded only if ever needed for diagnostics).
3. The phi comparison must be float64 (`phi >= np.pi` on widened values); float32
   comparison silently drops the float32(pi) population.
4. `np.isfinite` does not exclude the `-999` phi sentinel; exclude via `phi >= 0`.
5. Never join or deduplicate `(RunNumber, EventNumber)` across files.
6. Pool DJ NONRAD before scaling (single `djangoh14_nonrad` scale).
7. Any consumer of `ChargedGenRC` `phi_h` must wrap (pi, 2pi] -> (-pi, 0]; binning
   directly on [0, 2pi) is non-conforming for this product (the withdrawn-set lesson).
8. `L_data` never enters a RAD/NONRAD ratio; the ratio gate uses MC luminosities only.
9. `thish1.sh` returns non-zero (unset `GROUPROOT`) — Condor wrappers must not use
   `set -e`/`set -u` around it (operations detail, delegated to `h1-script-runner`).

## 10. Open risks

1. **RAD sets unaudited to date** — G1 is their first purity verification; a marginal
   failure quarantines the run pending an audit extension (the audit script
   `v1_kincut_audit_2026-10-02/audit_kincut.py` generalizes directly).
2. **RAPGAP EMPZ-window dependence unpublished** — the RG absolute anchor rests on the
   cross-generator gate; a >~4% RG-specific EMPZ shift would trip G4 legitimately.
3. **The exactly-2pi population (~1.6-2.3%)** is a real delta-like azimuth-zero feature
   of the stored truth phi (coplanar configurations collapsed by the builder's
   negative-angle mapping); it concentrates in the `[0, +pi/16)` bin and must not be
   misread as physics structure in the ratio pages (cross-reference the AziAsym
   phi~0-spike discussions).
4. **Exposure provenance remains `DIAGNOSTIC_MANIFEST_LEVEL`** (`P-CRAD-LUMI` downgraded,
   not closed) — the shape product is insensitive (scales cancel), but the G4 anchor
   inherits the caveat.
5. **Degenerate-chunk treatment** differs from the `C_rad` chain by <=0.01%
   (Section 5); recorded, not gated.
