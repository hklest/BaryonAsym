---
name: h1-sidis-trackclass-impact
description: "Track-class reclassification impact and nonprimary_norm_rel term; v1 (2026-10-20) WITHDRAWN after the user-prompted triple-check, v2 conservative definition"
metadata:
  node_type: memory
  type: project
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-21T06:41:07.722Z
---

**v1 (2026-10-20) is WITHDRAWN.** The user challenged that real tracks were being called background. The 2026-10-21 triple-check confirmed it:
- Tier-2 matches are genuine primaries (curvature/DCA-consistent).
- P/GenP>1.5 tracks are badly measured real particles; a secondary cannot exceed its parent's momentum.
- The large-angle class is ambiguous (elastic scatters).

The v1 ±2.8%/±2.2% and −0.56%/+0.10% numbers must not be used.

**v2 removes only clear background:** P/GenP<0.7 GTR-linked tracks (DCA fit 3–4% primary), the K± keeper and det.int. Duplicates are a separate double-count column. Campaign: Condor 5803801. Outputs: `.../trackclass_variant_2026-10-20/{shards/v2*, nonprimary_norm_term_v2}`.

v2 results (2026-10-21, Condor 5803813/5803819/5803820):
- Impact: h+ −0.526% ± 0.005% (MC), h− +0.057%. The definition spread is only ±0.03–0.04%.
- nonprimary_norm_rel v2 (κ=1): coherent ±2.07% / ±1.39%, per-bin median 0.44% / 0.38%.
- Duplicates (separate column): ±0.33% / ±0.40%.
- Record: local `docs/trackclass_triplecheck_2026-10-21.md`.

v3 (definition ruling 2026-10-21 still holds; its term is superseded by v5): provenance-only background (det.int. + keeper); everything GTR-linked is unfolded.
- Impact −0.033% / −0.028%.
- Term v3 (κ=1): coherent ±0.93% / ±0.82%, per-bin max 2.6% / 3.4%; never the largest component.
- Overlay plots: local `plots/nonprimary_norm_v3_overlay_2026-10-21/`; local doc `docs/trackclass_v3_provenance_2026-10-21.md`.

v5 INSTALLED 2026-10-21 (user-authorized baselining). Budget `systematics_bgsub_env100hyp_nonprimary_optA_2026-10-21/`; covariance edition `z005_covariance_optA_nonprimary_v5_2026-10-21/`; record `sidis_highQ2/docs/promotions/nonprimary_v5_promotion_2026-10-21.md`. It passed 3 critic rounds:
- Hyperons are tied to the env100 V0 nuisance (`bgsub_env100_hyp_rel`).
- STR int is a ±100% fiat; STR μ is re-sized (DJ–RG f_μ).
- Total median 7.31→7.45% / 7.62→7.80%.
- Local doc: `docs/nonprimary_v5_baseline_candidate_2026-10-21.md`.

**Why:** the user wants big background claims triple-checked. Always cross-check background classes with an independent variable: DCA′ templates matched in track length, plus physical constraints such as momentum ≤ parent.

**How to apply:** quote v5 only (v1 withdrawn, v2–v4 superseded); κ is still the user's decision. Links: [[h1-sidis-track-class-leaks]], [[h1-sidis-detint-uncertainty-gap]].
