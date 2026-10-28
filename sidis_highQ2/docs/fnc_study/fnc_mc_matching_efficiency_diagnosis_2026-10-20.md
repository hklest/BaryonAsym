# Diagnosis of the low fiducial gen-neutron → reco-cluster matching efficiency in djangoh14/7091 (v6fnc FNC study)

**Date:** 2026-10-20
**Author:** MC-simulation expert subagent (h1-mc-expert scope: generator/truth/detector-simulation interpretation)
**Question:** the Part-A fiducial gen→associated-reco efficiency in the merged v6fnc Stage-2 MC tree is 4.621% — where does the loss come from? Decompose into (a) fiducial-proxy generosity, (b) finder/storage/GoodRun gating, (c) missing/incomplete FNC or beamline simulation; spot-check a RAPGAP-π leading-neutron sample for comparison.
**Method:** single-file interactive checks only (login-node rule): merged Stage-2 trees via uproot (event-level de-dup by `(RunNumber,EventNumber)`), raw MODS files via plain ROOT 6.40.04 streamer-info emulation (`TTree::Draw`, technique validated in `fnc_h1oo_availability.md` §2/§5). All scripts listed under "Artifacts" re-generate every number below.
**Status:** measured, quantitative. Where a statement rests on general H1 knowledge rather than local evidence it is marked PROVISIONAL.

---

## 1. Executive verdict

The 4.621% is **not** a fiducial-definition artifact and **not** a storage/GoodRun-gating artifact. It is dominated by the **FNC simulation in the oo-4.0 MC DST chain being an acceptance-limited fast response**: it reproduces a calorimeter-like neutron energy response (E_reco/E_gen ≈ 1.03–1.06, rms 0.12–0.16) but only inside a narrow cone of projected radius r ≲ 8–10 cm (θ_n ≲ 0.8–0.9 mrad) around the ideal beam axis; beyond that cone the per-neutron response collapses to <1%. The real FNC, measured on the ep0607 data in the same v6fnc branches, accepts clusters over x ∈ [-30, +22] cm, y ∈ [-20, +28] cm (θ up to ≈ 2.7–3.4 mrad) with ≈ 96% azimuthal coverage (the 24 15°-φ-bins at r<30 cm are all populated). **75.5% of the fiducial gen neutrons project at r > 8 cm** — outside the simulated response region — and a further within-cone inefficiency (~31% cluster presence, 18.6% association for r < 7.9 cm) completes the loss.

| loss bucket | size (of the 95.4 pp missing from 4.621%) |
|---|---|
| (a) fiducial-proxy generosity | ≈ 0.5 pp (4.621% → 5.094% under the physical data-face fiducial) |
| (b) finder/storage/GoodRun gating | ≤ 0.2 pp (GoodRun not applied in the matching; MC FncPresent not run-gated) |
| (c) incomplete FNC/beamline simulation | ≈ 94–95 pp — **dominant** |

Corrected efficiency under the most physical fiducial definable from the existing branches (DATA face + pipe-slot + observed reco-φ arc): **5.094%** (2,730/53,595). Restricted to the simulation's own response cone (r < 7.9 cm, i.e. θ_n < 0.75 mrad): **18.55%** (2,646/14,261). A faithful full-face response — the simulation itself delivers 87.7% at the cone center in the RAPGAP-π sample — would associate ≈ 4.7×10^4 of the 53,595 face neutrons (~80%+, before single-cluster storage), i.e. the simulation limitation suppresses the matched efficiency by a factor ≈ 15.

A second, distinct geometry defect compounds any data/MC comparison: **both DJANGOH and RAPGAP emit the leading neutrons azimuthally symmetric around the ideal +z axis** (fiducial gen θ_y quantiles [-1.53, +0.10, +1.53] mrad, mean sin φ = 0.04), whereas the data clusters follow the proton-beam tilt (θ_y quantiles [-0.03, +1.01, +2.01] mrad, mean sin φ = 0.67, flux blob at (x,y) ≈ (-1, +11) cm). No analysis-level fiducial on the current branches can repair this frame mismatch.

---

## 2. Event-level populations and baseline reproduction (merged 10-file MC tree)

Input: `H1ChargedSidis_HighQ2/output_v6fnc/mc7091_study/stage2/merged/chargedsidis_v6fnc_djangoh14_7091_mc10_merged.root`, 307,948 events (first-row-per-event de-duplication; FNC branches verified event-constant).

- `FncPresent==1`: 19,954 (6.480%); with `FncGoodRun==1`: 19,189 (6.231%).
- `FncGenNFound==1`: 107,942 (35.05%).
- `FncPresent && FncGenNFound`: 7,698 = **38.56% of FncPresent** (confirms the corrected overlap of the dated erratum; the 97.8% figure remains refuted).
- Baseline proxy fiducial (r = sqrt(FncGenNXfnc²+FncGenNYfnc²) ∈ [0.195, 31.167] cm, projection φ inside the MC 99% reco-φ arc [6.142, +6.037 rad]) and association (|Δθ|<0.4 mrad, |Δφ_wrapped|<0.5 rad, 0.3<E_reco/E_gen<2.0): **N(fid)=59,079, N(assoc)=2,730, eff=4.621%, both(fid∧present)=6,017, reco→fid purity=13.681%** — exact reproduction of the Stage-2 study.
- Association failure among the 6,017 coexistence events: fail |Δθ| 1,906; of the remainder fail |Δφ| 1,332; fail energy-only 49. Failing clusters point elsewhere (median cluster θ 0.43 vs gen 0.32 mrad, median Δφ -0.196 rad, median E ratio 0.94) — i.e. the stored cluster was produced by another particle (see §5).
- 61.4% of all clusters (12,259/19,954) are in events with **no** leading forward gen neutron in the builder envelope at all.

## 3. Bucket (a): fiducial-proxy generosity — measured ladder

Association cuts fixed; only the gen-side fiducial varies. `pres_eff` = P(any cluster | fiducial gen).

| fiducial on leading gen neutron | N(fid) | N(assoc) | eff | pres_eff |
|---|---:|---:|---:|---:|
| F0 proxy (r-band + φ arc) — baseline | 59,079 | 2,730 | **4.621%** | 10.19% |
| F0 + θ_n < 0.75 mrad | 14,274 | 2,654 | **18.59%** | 31.06% |
| DATA face (x∈[-30,30], y∈[-20,28]) minus slot y∈[21,27.5] + φ arc | 53,595 | 2,730 | **5.094%** | 11.01% |
| DATA face + sim cone (r<7.9 cm) | 14,261 | 2,646 | **18.55%** | 31.18% |
| face + θ<1.0 / 2.0 / 3.0 mrad | 21,358 / 43,944 / 59,073 | 2,723 / 2,730 / 2,730 | 12.75% / 6.21% / 4.62% | 22.9% / 12.9% / 10.2% |

Reading:

- The empirical r-band proxy is *close in area to the real face* — tightening to the physical data-derived face changes the efficiency by only +0.47 pp (and 90.7% of proxy-fiducial gen neutrons already project onto the face; 2,458 project into the slot band, 6,347 outside the face).
- The entire "fiducial" sensitivity is the **θ (equivalently r) dependence of the simulated response**: 97.2% of associated pairs have θ_gen < 0.75 mrad, and the efficiency falls monotonically as the fiducial admits larger angles. This is bucket (c) leaking through the fiducial definition, not fiducial generosity.
- The φ-arc element is a near no-op geometrically (it excludes only 0.246 rad of azimuth); the DATA φ occupancy at r<30 cm is populated in **all** 24 15°-bins (min 84 entries at -82.5°, max 3,731 at 97.5°). The expectation of a "~30% azimuthal-coverage ceiling" quoted in the task premise is **refuted by the data map**: the effective hard coverage is ≈ 96% of azimuth (99% shortest arc 6.037 rad); the strongly nonuniform φ *shape* is neutron flux following the proton-beam tilt, not a geometric aperture.

## 4. Bucket (b): storage/finder/GoodRun gating — small and non-gating in MC

- **`FncGoodRun` on MC is a real lookup against data-like run numbers.** The merged tree carries 722 unique run numbers in 468531–472427 (2006-era e+ numbering); 11 runs fall inside `H1FNCQuality` excluded ranges (469485, 469486, 469494, 470908, 471211–471219) → 11,478 events (3.73%) have `FncGoodRun==0`.
- **`FncPresent` is NOT gated by run quality in MC**: 765 FNC-present events sit in "FNC-bad" runs (the MC DST chain wrote FNC content regardless of the HERA-II run-quality list, unlike real data where FNC-excluded runs have empty `FNCPart` — verified earlier on chunk c070000).
- The Stage-2 matching never applied GoodRun; requiring it post hoc moves 107 of the 2,730 associated events: eff 4.621% → 4.440% (face fiducial 5.094% → 4.894%). **≤ 0.2 pp effect.**
- The production-time `H1FindFNC` gates (FNCL cluster 0 only; hard E>10 GeV; `fnc_h1oo_availability.md` §3.2) are mirrored on the gen side (E>10) by the builder; only 20 of 59,079 fiducial gen neutrons have E ≤ 15 GeV. The single-cluster truncation is bounded by the multi-neutron fraction (3,816 of 35,602 fwd-neutron events in the raw file have ≥2 fwd GTR neutrons E>10) — a few-percent effect, not separable further with the stored one-cluster/one-leading-neutron branches.

## 5. Bucket (c): the FNC/beamline simulation — evidence and mechanism

### 5.1 Raw-file scan (djangoh14/7091 file `...DST.0001.root`, 100,000 events)

- `FNCPart` non-empty in 6,268 events (6.27%); cluster E mean 273.6 / median 199.5 / max 1,431 GeV; z mean 10,584 cm; bulk positions x ∈ [-13.6, +10.0], y ∈ [-9.2, +13.2] cm (1–99%).
- Forward GTR neutrons (PDG 2112, E>10 GeV, θ<10 mrad): 39,565 rows in 35,602 events (35.6%), mean E ≈ 338 GeV. **Measured minimum θ = 0.008 mrad** — the builder-record statement "no GTR neutron with E>10 GeV has theta < 0.0008 rad" is **refuted** (θ quantiles of the fwd population: [0.008, 0.77, 2.50, 37.5...] mrad for [min, q10, median, q90]). The number 0.8 mrad is instead the approximate *edge of the FNC simulation's response cone* (below), with which it was apparently conflated.
- **Simulated per-neutron response** (cluster within 3 cm of the straight-line projection to z=10,550 cm), as a function of projected radius:

| projected r of gen neutron | N gen (E>10) | has cluster (dr<3cm) | fraction |
|---|---:|---:|---:|
| 0–2 cm | 487 | 266 | **54.6%** |
| 2–4 cm | 1,220 | 331 | 27.1% |
| 4–6 cm | 1,847 | 280 | 15.2% |
| 6–8 cm | 2,017 | 194 | 9.6% |
| 8–10 cm | 2,035 | 18 | **0.9%** |
| 10–14 cm | 3,630 | 4 | 0.1% |
| ≥14 cm | 16,757 | 0 | 0.0% |

  Event-level cluster *presence* for gen neutrons at r<2 cm is 71.5% (65.3% within 10 cm). **Hard response cliff at r ≈ 8–10 cm (θ ≈ 0.76–0.95 mrad).**
- **The response that does exist is a genuine simulated calorimeter response, not an arbitrary subsample:** for clusters within 3 cm of a gen neutron, E_reco/E_gen = 1.055 ± 0.164 (median 1.062; merged-tree matched set: 1.0525 ± 0.1309, with the energy-dependent profile 1.19 → 1.02 from E_gen 100–200 → 650–920 GeV, consistent with leakage-type response). Matched angular residuals: Δθ rms 0.092 mrad, Δφ_wrapped rms 0.210 rad.
- **Cluster producers:** nearest GTR n/p/n̄/γ(E>10) within 3 cm of a cluster: neutron 15.6%, proton 14.9%, n̄ 0.4%, γ 20.1%, **none 49.0%**. The proton coincidences do not track energy (E_reco/E_p median 0.157, E_p median 920 GeV — the on-axis remnant proton projects into the crowded cone center) and the γ coincidences likewise (E_γ ≤ 27.5 GeV vs E_reco ~ 200 GeV): both are geometric coincidences, not causal matches. GTR protons almost never produce clusters (response 1.7% at r<2 cm). The unattributed half of the cluster population (77.4% of clusters lie >10 cm from any fwd gen neutron; 61.0% of cluster events contain no fwd GTR neutron at all; cluster energies reach 1,431 GeV, above any single gen hadron) cannot be resolved from MODS content — candidates are GEANT STR secondaries, charged hadrons, and overlapping deposits. PROVISIONAL as to origin; the quantitative response map above is unaffected.
- Where the FNC content comes from: the FNC simulation stage is **not part of H1OO 4.2.1** (only the reader/finder chain exists: `H1FindFNC` → FNCL → `H1PartFNC`/MODS `FNCPart`, `H1CalcFnc`, `H1FNCQuality` — `fnc_h1oo_availability.md` §3). The FNCL content in these MC DSTs was produced inside the DESY H1MC production chain (H1SIM/H1REC side), whose parametrization is not locally inspectable; the behavior is established empirically here. The published H1 leading-neutron measurements used a dedicated, full beamline-transport + GEANT FNC simulation (PROVISIONAL — general H1 knowledge, not locally verified; provenance questions belong to `h1-mc-db`).

### 5.2 DATA reference (ep0607 merged Stage-2 tree, 116,281 events; 23,285 `FncPresent&&FncGoodRun`)

θ/φ of clusters projected to z=10,550 cm:

- θ quantiles [1%, 10%, 50%, 90%, 99%] = [0.074, 0.263, 1.221, 2.136, 2.749] mrad (range to 3.35).
- x ∈ [-29.6, +22.0] cm, y ∈ [-20.0, +27.9] cm (hard edges: nothing below y=-20, above y≈+28, left of x=-30).
- **Pipe-slot signature**: occupancy dips to near zero in the band y ∈ [22.5, 27.5] cm across all x and reappears at y ∈ [27.5, 28) — the proton beam pipe crosses the face near the top (the "slot at the top" of the task premise), with active calorimeter material above the pipe.
- φ occupancy populated in all 24 fifteen-degree bins (see §3) → ≈ full-azimuth acceptance; flux blob at (x,y) ≈ (-1.3, +10.6) cm, i.e. along the proton-beam direction tilted ≈ +1.0 mrad in y / -0.1 mrad in x.

**MC-vs-DATA acceptance comparison:** the real FNC accepts θ up to ≈ 2.7–3.4 mrad over x ±30 cm, y -20..+28 cm; the oo-4.0 MC FNC response dies beyond θ ≈ 0.8–0.9 mrad (r ≈ 8–10 cm). The simulated acceptance covers roughly **1/3 of the real face radius and ~6% of its area**, and reproduces neither the face edges, nor the slot, nor the beam tilt.

### 5.3 Generator frame (both DJANGOH and RAPGAP)

Fiducial gen neutrons are emitted azimuthally symmetric around the ideal +z axis: θ_y quantiles [-1.53, +0.10, +1.53] mrad, θ_x [-1.56, -0.02, +1.40], mean sin φ = 0.043 (djangoh fid0; RAPGAP-π: mean sin φ = -0.001). Data clusters: θ_y [-0.03, +1.01, +2.01] mrad, mean sin φ = 0.669. The missing proton-beam direction in the generator compounds the simulated-acceptance deficit for any data/MC shape comparison; the two effects are distinguishable (frame vs aperture) but both are outside what the current v6fnc branches can correct.

## 6. RAPGAP-π spot check (ReqID 9134, `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/9134/`)

/pnfs IS readable from this node. One file: `mods.4.0.25.RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3.smearing.9134.S39600.R97905.ftt.DST.0000.root` (90,000 events; pion-exchange leading-neutron sample with LN θ smearing, GRV-LO-π, Q2>3 GeV² — note: NOT the Q2>150 GeV² SIDIS phase space).

| quantity (per event) | djangoh14/7091 (file 0001) | rapgap31/9134 (file 0000) |
|---|---:|---:|
| `FNCPart` non-empty | 6.27% | **37.48%** |
| events with fwd GTR n (E>10, θ<10 mrad) | 35.6% | **100%** |
| FNC events containing a fwd GTR n | 39.0% | **100%** |
| clusters within 3 cm of a fwd gen n | 17.1% | **69.5%** |
| E_reco/E_gen for those | 1.062 (median) | 1.036 ± 0.115 |
| gen-n θ median [mrad] | 2.50 (2.46 selected-tree leading) | **0.60** |
| gen-n projected r median [cm] | 46.6 (13.9 fiducial-selected) | **6.4** |
| neutron response at r<2 cm | 54.6% | **87.7%** |
| neutron response at r ≥ 10 cm | ≈ 0% | **≈ 0%** |
| cluster population vs face | compact blob, y ∈ [-9,+13] (1–99%) | full face: x ∈ [-25.6,+22.0], y ∈ [-19.4,+28.6] |

Interpretation:

- The RAPGAP-π pion-exchange spectrum (leading neutron generated nearly on-axis, θ median 0.60 mrad) sits largely **inside** the oo-4.0 FNC simulation's response cone, so its cluster occupancy (37.5%), reco–truth overlap (100%), and cluster-proximity purity (69.5% within 3 cm, energy response 1.034) are all far better than DJANGOH's — the FNC content is demonstrably driven by the generator's forward-neutron spectrum folded with the same limited simulated aperture.
- **The same response cliff at r ≈ 8–10 cm is present** (87.7% → 54.2% → 26.1% → 10.3% → ~0% across the r bins): the cone is a property of the oo-4.0 MC FNC simulation, not of the generator.
- RAPGAP-π clusters do populate the full face out to the data edges — but the beyond-cone component (clusters ≥10 cm from any fwd gen neutron, 16.3%) is a **low-energy** population (median 64 GeV vs the event's leading gen neutron 597 GeV; E ratio ~0.12), consistent with forward-jet hadrons in this Q2>3 sample plus a background-like component — not a full-face neutron response.
- Same generator-frame issue: RAPGAP-π gen neutrons are also azimuthally symmetric around the ideal axis.

## 7. Quantitative loss decomposition (baseline eff 4.621%, loss 95.38 pp)

Using the physical data-face fiducial (53,595 gen neutrons; eff 5.094%, loss 94.91 pp):

1. **(a) Fiducial-proxy generosity: 0.47 pp.** The proxy over-counts 5,484 gen neutrons outside the true face/slot (9.3% of the proxy sample); N_assoc unchanged.
2. **(b) Storage/finder/GoodRun gating: ≤ 0.2 pp.** GoodRun non-gating in MC (§4); finder floor mirrored; single-cluster truncation bounded at a few percent of the multi-neutron 10%.
3. **(c) Simulation limitation: ≈ 94.4 pp**, split as (fractions of the 53,595 face-fiducial gen neutrons):
   - **73.4% project beyond the simulated response cone (r > 7.9 cm, response <1%)**: ≈ 73.3 pp of the loss;
   - within the cone (26.6%), only 31.2% produce any cluster and 18.55% associate: ≈ 21.7 pp;
   - together 73.3 + 21.7 ≈ 95.0 pp ≈ the whole 94.91 pp loss (rounding); the within-cone gap is dominated by the sim's soft radial response (54.6%→9.6% across r = 0–8 cm) plus association geometry on clusters produced by other particles (§2 failure breakdown).

**Corrected efficiency under the most physical fiducial: 5.094%.** Under the simulation's own cone: 18.55%. The matched-efficiency shortfall vs a faithful full-face simulation is a factor ≈ 15 (2,730 observed vs ≈ 47,000 expected at the sim's own cone-center response level).

## 8. What is needed for a proper FNC acceptance correction in MC

1. **The oo-4.0 hat/mods FNC content cannot serve as a full-acceptance FNC simulation** for either generator (cone θ ≲ 0.8–0.9 mrad; no face/slot/beam-tilt geometry; half the cluster content unattributable to gen neutrons). It is usable only for θ_n ≲ 0.75 mrad analyses, where the response is calorimeter-like and the RAPGAP-π purity is high (69.5%).
2. **Vehicle:** RAPGAP-π (pion-exchange, LN-smearing; ReqIDs 8946/9134/9137 for genID 5623, SW-84-era; also 8434 without smearing — availability/routing to `h1-mc-db`) is the correct generator for leading-neutron FNC work — its forward-neutron spectrum is compatible with the simulated aperture — but it still needs (i) a full beamline+FNC detector simulation or an external acceptance map, and (ii) reweighting to the Q2 > 150 GeV² SIDIS phase space (the inspected sample is Q2 > 3 GeV²).
3. The published H1 leading-neutron measurements used a dedicated GEANT-based beamline+FNC simulation (PROVISIONAL, not locally verified). Whether any FNC-dedicated MC (DST with real FNCL from a full sim, or acceptance maps) can be obtained for this analysis must be routed to `h1-mc-db`; luminosity/normalization side to `h1-normalization`.
4. Any data/MC shape comparison must account for the **generator frame defect** (leading neutrons azimuthally symmetric around the ideal axis instead of along the ≈1 mrad proton-beam tilt). Fixing it requires generator-side beam-direction treatment or a documented analysis-level rotation — not achievable from the current v6fnc branches alone.
5. The empirical DATA occupancy map (§5.2) is flux-convolved and v6/S67-selected; it defines the face/slot edges robustly but is **not** by itself an acceptance×efficiency map.

## 9. Limitations / provisional items

- Cluster-producer attribution beyond the neutron component (49% of djangoh clusters) is unresolvable from MODS content — STR-secondary / charged-hadron candidates, PROVISIONAL.
- The `H1PartMC`-based projection uses the momentum direction from the IP (vertex effects ≪ 0.1 cm at the FNC plane); per-particle production vertices are unavailable (documented builder limitation, `FncGenNVtxMode==1`).
- The data reference map derives from the v6fnc-selected ep0607 sample; a trigger/luminosity-unbiased acceptance map would need the full data campaign (Condor).
- All raw-file numbers are from single files (djangoh14/7091 `DST.0001`, rapgap31/9134 `DST.0000`); file-to-file variation is expected to be statistical only.
- The refuted θ-claim in `v6fnc_builder_change_and_smoke_plan_2026-10-20.md` ("no GTR neutron with E>10 GeV has theta < 0.0008 rad") should be corrected by the builder-record owner: measured min θ = 0.008 mrad, and 0.8 mrad coincides with the simulated response-cone edge, not with any generator population feature.

## 10. Artifacts

Scripts (reproduce all numbers; caches under `/tmp/opencode/fnc_study/` are regenerable):

- `sidis_highQ2/scripts/fnc_study/diagnose_npz_overlap.py` — raw-scan npz overlap hypotheses, gen-θ distribution.
- `sidis_highQ2/scripts/fnc_study/diagnose_merged_mc.py` — merged-tree populations, baseline reproduction, fiducial ladder, spatial maps.
- `sidis_highQ2/scripts/fnc_study/diagnose_merged_data.py` — data cluster θ/φ/x/y reference map, slot signature, φ occupancy.
- `sidis_highQ2/scripts/fnc_study/raw_mods_match_scan.py` — djangoh raw-file cluster↔gen matching, response map, energy correlations.
- `sidis_highQ2/scripts/fnc_study/followup_response_gating.py` — within-cone response, GoodRun gating quantification, data-face containment.
- `sidis_highQ2/scripts/fnc_study/rapgap9134_spotcheck.py` — RAPGAP-π 9134 single-file spot check.
- `sidis_highQ2/scripts/fnc_study/rapgap_cluster_origin.py` — RAPGAP cluster origin (proximity distribution, displaced/low-E component).

Condor: **not required** for any conclusion above (all single-file interactive checks). Condor campaigns would be needed only for production-scale follow-ups (full 101-file RAPGAP-π scan, FNC-branch Stage-1 extension to RAPGAP-π steering, full-data unbiased acceptance mapping); none was launched.

*End of artifact.*
