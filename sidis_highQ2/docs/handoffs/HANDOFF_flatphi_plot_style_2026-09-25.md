# Handoff: Flat Gen-phi_h Reweight Sensitivity Plot Style

> **PRIORITY -- ANALYSIS-NOTE CANDIDATE FIGURE.** The eight-page bundle at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/cutshift_style_shifts_perbin/flatphi_perbin_cutshift_style_review.pdf` is a **candidate figure for the analysis note**. Bring it to publication standard first. All other bundles listed below are supporting or internal diagnostics and have lower priority.

**Audience:** plotting specialist. This handoff supplies the interpretation and non-negotiable messaging. Improve rendering and layout without re-deriving, changing, or selectively presenting the physics.

**Study status:** `PROVISIONAL_INTERNAL_DIAGNOSTIC`; raw/no background subtraction/no `C_rad`/not Born level; diagnostic side study, not an approved systematic or central result. Do not silently change this status.

## Inventory and Priorities

All products are below `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/plots/`. PNG page images accompany each PDF; some are under a `png_pages/` subdirectory.

| Bundle | Pages | Priority |
|---|---:|---|
| `cutshift_style_shifts_perbin/flatphi_perbin_cutshift_style_review.pdf` | 8 | **ANALYSIS-NOTE CANDIDATE** |
| `phih_truth_pre_post_perbin/phih_truth_pre_post_perbin.pdf` | 7 | Supporting validation |
| `cutshift_style_shifts/flatphi_cutshift_style_review.pdf` | 5 | Iteration-1 internal |
| `phih_datamc_pre_post/phih_datamc_gen_pre_post.pdf` | 4 | Iteration-1 internal |
| `flatphi_genreweight_study/flatphi_genreweight_study.pdf` | 7 | Iteration-1 overview |

Start with the priority atlas only. Preserve the current bundle and create a clearly versioned restyle/review product rather than overwriting the review master unless the supervisor explicitly directs otherwise.

## Generating Scripts and Data

### Plot scripts

- Priority atlas: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/scripts/plot_cutshift_style_flatphi_perbin.py`
- Iteration-1 cut-shift atlas: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/scripts/plot_cutshift_style_flatphi.py`
- Iteration-1 overview: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/scripts/plot_flatphi_study.py`
- Gen-level pre/post-phi_h plots: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/studies/flatphi_genreweight_z005_20260925/plot_phih_gen_pre_post.py`

The priority script uses the Agg backend, creates its PDF with `PdfPages`, and renders page PNGs with `pdftoppm`. It already contains numerical assertions for headline medians and significance counts. Preserve or strengthen those checks after restyling.

### Priority-atlas data

- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/flatphi_genreweight_z005_20260925/ibu/comparison_perbin/perbin_decomposition.csv`

The CSV has 980 total rows: 490 published bins for each generator. Each generator set has the charge, bin index, global bin index, cell/z/pT indices and edges, `S_perbin`, `S_halfA`, `S_cell`, two noise-subtracted estimates, `unity_stat_rel`, and `total_with_tracking_rel`. Do not reorder or subset the 490-bin atlas without clearly declaring a different figure purpose.

## Scientific Content That Must Not Be Distorted

The study reweighted the **generator-level CM-frame Trento** `phi_h` distributions of DJANGOH14 and RAPGAP31 to be flat, rebuilt the responses, and reran IBU at `k=4` using the **unchanged nominal data vector**. It tests sensitivity of acceptance/unfolding to the generator `phi_h` model.

The truth-coherent weighting policy is part of the result: prior and miss use generator `phi_h`; a matched reconstructed leg uses its matched generator partner's `phi_h`; fakes have unity weight. Do not recast the study as a reconstructed-phi_h reweight or as a direct data/MC comparison.

Iteration 2 resolves the flattening by published bin and is **noise-dominated**:

| Quantity | DJANGOH14 | RAPGAP31 |
|---|---:|---:|
| Median `|S_perbin|` | 1.0183% | 0.9589% |
| Median `|S_halfA|` | 1.0011% | 0.9737% |
| Median `|S_cell|` | 0.5372% | 0.5364% |
| Bins with `|S_perbin| < |S_halfA|` | 232/490 (47.35%) | 244/490 (49.80%) |
| Noise-subtracted median, raw | 0.0202% | 0.0037% |
| Noise-subtracted median, sqrt(2)-scaled | 0.7021% | 0.6301% |

`S_halfA` is a half-sample weight statistical-noise control: its weights are derived from half of the MC and applied to the full sample, so it contains **no real model variation by construction**. For RAPGAP31, the control exceeds the apparent signal. Therefore the apparent rise from the iteration-1 cell-level result of about 0.54% to approximately 1.0% per bin is largely weight statistical noise, **not** newly exposed generator-model dependence. Iteration 1's 0.54% cell-level result remains the defensible estimate.

## Non-Negotiable: No Misreading as a 1% Model Sensitivity

**No page, viewed in isolation, may be misread as claiming an approximately 1% generator-model sensitivity. This is non-negotiable.**

The current priority pages meet this requirement through explicit noise-control captions. A restyle must preserve the guarantee on every individual page, including zoomed/unzoomed atlas pages and summary pages. Keep a concise, conspicuous page-local statement that `S_halfA` is pure weight statistical noise with no model variation and that the apparent approximately 1% per-bin shifts must not be interpreted as model sensitivity. Make this readable at print size; do not rely on a cover page, an external caption, hover text, or the reader seeing another page.

A successful note-facing design can improve hierarchy and reduce clutter, but it cannot demote the noise-control conclusion below the title/legend/visual emphasis of `S_perbin`.

## Uncertainty Bands: Definitions and Required Caveats

The priority atlas draws two symmetric relative-uncertainty bands:

- `unity_stat_rel`: IBU statistical uncertainty.
- `total_with_tracking_rel`: imported **PROVISIONAL** uncertainty budget, exactly the quadrature sum of 10 components, verified to `3.3e-16`: `ibu_stat_rel`, `ibu_model_rel`, `ibu_reg_rel`, `hfs_jes_rel`, `hfs_rces_rel`, `hfs_theta_rel`, `elec_energy_rel`, `elec_theta_rel`, `lumi_rel` (flat 2.7%), and `tracking_eff_m0p5_rel`.

The imported total has median `0.060858` and maximum `1.625084` (relative units). Its labels, caption, or an adjacent explanatory page must keep these caveats visible and unambiguous:

1. `background_rel` is currently **RETIRED** from this budget. It was traced to one anomalous MC event weight `43.1292`, so background is **not covered** by this total.
2. `ibu_stat_rel` dominates the median; the displayed “total” is substantially statistical.
3. `elec_energy_rel` drives the 1.63 maximum tail.
4. The total already includes `ibu_model_rel` and `ibu_reg_rel`; comparing a phi response/model variation with it is partly self-comparison.

Do not redesign the bands so that a reader can mistake `total_with_tracking_rel` for a complete independent systematic envelope or a final approved uncertainty.

## Style-Defect History and Root Causes

These plots were **rejected three times** in review before approval. Do not reintroduce any of the following defects.

1. **Round 1:** a diagnostic/luminosity footer overlapped x-axis labels, tick labels, and category labels on all seven pages of the overview bundle.
2. **Round 2:** after fixing the footer, the h- median annotation was obscured by the legend, and a right-side amplification annotation was overlapped by the legend.
3. **Round 3 (iteration 2):** the top legend overlapped and obscured the main title on four of eight pages of the priority bundle; four of seven pages of the truth-level bundle had no visible legend at all.

**Root cause:** manually placed `fig.text()` footers and `loc='best'` legends interacted badly with `tight_layout()` / `constrained_layout`. Do not assume a layout engine will protect manually positioned content. Reserve explicit margins and place legends outside the axes in a dedicated, tested region. Render every page and inspect the rasterized PDF output; source-code inspection is not evidence that layout is correct.

## Concrete Improvement Objectives

1. Apply the project `plotting-style` conventions: Okabe-Ito palette, prescribed sans-serif rcParams, no grids, clean white background, consistent line/marker weights, and colorblind-safe contrast.
2. Make the priority atlas publication-grade: coherent typography, readable print-size labels, consistent axis treatment, restrained annotations, and no chartjunk.
3. Ensure greyscale legibility. Do not encode generator, variation, or uncertainty-band identity in color alone: use distinguishable line styles, marker shapes, hatching, ordering, and clear labels as appropriate.
4. Retain canonical cut-shift atlas conventions: all 490 bins on one x-axis; h+ bins 0-244 then h- bins 245-489; visible charge boundary; marked cell boundaries; and both zoomed and unzoomed variants.
5. Establish a deliberate information hierarchy. The actual plotted shifts, the `S_halfA` control, and the page-local noise interpretation must remain more prominent than decorative status treatment.
6. Use an explicit layout plan rather than magic coordinates: fixed title band, legend band, axes rectangle, and caption/caveat band. Verify that all are non-overlapping at the final PDF dimensions.
7. Retain PDF as review master and regenerate per-page PNGs for visual review. Use the Agg backend before importing `pyplot`.

## Acceptance Criteria for the Restyle

A restyled priority bundle is acceptable only if all of the following are true:

- The analysis-note candidate designation, provisional status, and page-local no-misreading guarantee are retained.
- Each page is independently interpretable and explicitly states that `S_halfA` is a pure statistical-noise control with no model variation; no page implies a roughly 1% confirmed generator-model sensitivity.
- No title, legend, annotation, footer, axis label, tick label, charge label, or cell label overlaps, clips, or is hidden on any page.
- Legends are visible on every page and do not obscure titles, data, band definitions, medians, or conclusions.
- The 490-bin order, h+/h- split, charge boundary, cell boundaries, and zoomed/unzoomed atlas variants are unchanged.
- Text remains legible in print and single-column reproduction; colors remain interpretable for color-vision deficiencies and greyscale output.
- The two bands retain their identities and all four uncertainty caveats above remain accessible in the bundle's labels/captions/adjacent explanatory material.
- The style follows `plotting-style`: DejaVu Sans, Okabe-Ito-based palette, no grids, white background, visible spines, and uncluttered axes.
- The render uses a PDF master plus inspected PNG pages. Review the full multipage PDF after rasterization, not merely a subset.
- Recomputed/rendered values agree with the scientific-content table in this handoff: the six listed medians and the two `|S_perbin| < |S_halfA|` counts/fractions. Retain or add automated assertions.
- The updated script and products remain under the isolated study root; central products are not modified.

## Open Decisions for the Supervisor

1. **Diagnostic banner for a note-facing version:** `PROVISIONAL_INTERNAL_DIAGNOSTIC` remains mandatory while this study is unapproved. A candidate analysis-note figure may need the caveat moved from a large overlay banner to a caption-based treatment to meet publication presentation standards. This is a **supervisor decision**. Do not remove, reduce, or relocate the diagnostic labelling without explicit approval.
2. **Figure selection and scope:** decide whether the full eight-page atlas, a selected atlas page, or a purpose-built compact derivative is the actual analysis-note submission candidate. Do not silently omit the control or choose a more favorable-looking subset.
3. **Uncertainty-band presentation:** decide whether the imported provisional total band belongs in the note-facing figure, given its incomplete background coverage, statistical dominance, and partial self-comparison with model-related components. If retained, preserve the stated caveats.
4. **Approval status:** a style improvement does not approve the study as a systematic or central result. Any change in scientific status requires supervisor/physics review.

## Full Study Reference

For the complete methodology, production validation, numerical interpretation, and iteration-1 context, read:

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/systematics/flatphi_gen_reweight_sensitivity_2026-09-25.md`
