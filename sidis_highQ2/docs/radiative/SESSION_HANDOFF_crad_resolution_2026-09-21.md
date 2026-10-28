# C_rad resolution session handoff — 2026-09-21

## Status

The central v6/z005 release remains raw: no `C_rad` application and no Born-level claim. Retain the existing RAD sample; supplement NONRAD only. The updated `C_rad` product is a candidate, not a release input.

## Validated NONRAD supplement

| Beam | Valid campaign | Stage 3 | `GenDIS` | `ChargedGenRC` |
|---|---|---:|---:|---:|
| e+ | `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_530x10k_eplusp_prod_20260911` | `530/530` | `5,298,940` | `118,109,262` |
| e- | `/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_eminusp_regen_20260916` | `530/530` | `5,298,940` | `117,875,988` |

Both campaigns pass unique-content validation. The July e- campaign is invalid because of duplicate `RNIN` and remains quarantined. The supplemental aggregate is `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/tables/supplemental_nonrad_530_v1_20260920/`; weighted exposures are e+ `2,519,454.5686446126 nb^-1` and e- `2,469,812.987172118 nb^-1`.

## Compatibility and RAD provenance

The OLD-versus-NEW NONRAD shape audit is `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/nonrad_supp_vs_old_20260921/summary.json`. Broad shapes pass, but e+ `pT_h` and h- occupancy are statistically tense; their normalized absolute deltas are at most `0.0002106`. Overall QA is **INCONCLUSIVE** pending a bin-local compatibility/provenance sidecar.

No further RAD production is desired. New RAD smokes fatally aborted in `AREMIT` (`MSTA39`) and produced header-only BOS files; do not use them. Existing RAD gen-only ntuples are retained at `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/ntuples/DJANGOH14/rad/` (`1373` files; components `8955`–`8962`). Detector-MC RAD periods are `7093/7092/7072/7091`.

The common-subset event comparison is **INTERFACE_BIASED**, not evidence of a generator difference: detector `ChargedTotGen` lacks `Mother2` and photon-axis `pT_h`, and has a different interface/population. User direction is to retain existing RAD and supplement NONRAD only.

## Candidate (not applied)

Candidate directory: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/final_crad_candidate_20260921/` (Condor cluster `5167043`).

- Integrated candidate: h+ `1.1155728783 +- 0.0001163697`; h- `1.1155253915 +- 0.0001169047`.
- Finite values: `479/490`; finite for both beams: `468/490`.
- Relative statistical uncertainty: median `0.4485%`, p90 `15.0441%`, p95 `28.9468%`, maximum `109.5445%`.
- `114` values are outside `[0.5,1.2]`.

Exposures used: RAD e+ `16,206,393.0`, RAD e- `13,534,017.0`, combined NONRAD e+ `15,439,571.132472707`, and combined NONRAD e- `15,311,385.428725630 nb^-1`. Data mix: e+ `190,958.2993`, e- `160,041.7007 nb^-1` (sum `351,000`).

## Release blockers and exact next actions

1. Finalize OLD/NEW NONRAD per-bin ratios and the provenance sidecar.
2. Independently validate the candidate normalization JSON.
3. Decide the sparse-bin mask/merge policy.
4. Obtain publication-grade exposure provenance.
5. Take physics review of a coherent RAPGAP-response versus DJANGOH-`C_rad` choice; the current central response is RAPGAP and a DJANGOH-response cross-check exists.
6. Release tables only after these gates close.
