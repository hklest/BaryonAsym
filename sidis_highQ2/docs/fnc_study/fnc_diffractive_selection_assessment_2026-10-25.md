# Physics adjudication: proposed "diffractive selection on data" for agreement with RAPGAP-π 9134 (high-Q² charged-SIDIS FNC study)

- **Date:** 2026-10-25
- **Author:** h1-physics-analyst (charged-hadron SIDIS physics adjudication)
- **Task:** READ-ONLY physics adjudication of the user-proposed methodology: *"We need to make the diffractive selection on our data for it to agree with RAPGAP-π."* No selection, cut, or change to any data product was applied in this task; no data product was modified.
- **Context adjudicated:** DATA ep0607 (FNC neutron-tagged, v6fnc) vs RAPGAP-π ReqID 9134 (genID 5623, RAPGAP31 `PIPEXCH` pion-exchange leading-neutron MC) at rate level, per `fnc_rates_2026-10-25.md`: in-cone (`FncThetaN<0.75 mrad`) data/pred = **0.4266 ± 0.0078**; out-of-cone 7.27 (MC flagged not credible there).
- **Method:** documentary evidence (six fnc_study docs, the sidis-hq2-mc skill and its RAPGAP references, the v6fnc builder sources) plus new single-file read-only ROOT checks on the NAF login node (≤5 min each, plain ROOT 6, one input file per invocation): the two merged Stage-2 study trees and one original 9134 HAT file on `/pnfs`. All checks are listed in §6 with the exact numbers they produced.

---

## 0. Executive verdict

| # | Question | Verdict |
|---|---|---|
| 1 | Is RAPGAP-π 9134 diffractive? | **No.** It is a pure **pion-exchange leading-neutron** sample (`PIPEXCH` tag; 100% of selected events have a leading gen neutron). Pion exchange and pomeron-exchange diffraction are **different** leading-particle mechanisms at HERA (leading n vs leading p; different exchanged trajectory, recoil baryon, flux, cross section). |
| 2 | Diffractive content on either side? | **No usable diffractive observable exists in the v6fnc study trees** (0 of 107 data / 116 MC `ChargedTot` branches; none in the Stage-1 EVLS schema). The raw HAT files carry the diffractive-*named* branches (`Xpom/MXGen/MYGen/TGen/Gap/GapTyp/EtaGapU`), populated in 9134 with **π-exchange topology** values, not pomeron values; they are not propagated into the study trees. A small diffractive contribution to the data tag sample is plausible in principle but unmeasurable here and bounded small (x_L>0.87 content ≈ 1% of tagged data); the MC conversely already contains a large **dissociated-recoil** component (M_Y>3 GeV in 43% of generated events). |
| 3 | Is "select data until it agrees with MC" valid? | **No — unsound.** Agreement tuning has no validation content, is circular given an a-priori-deficient MC (FNC sim cone collapse, no beam tilt, pure-π subset), and destroys the unconditional data composition needed for the h⁺–n correlation study. The documented §7 comparability verdict already defines the correct treatment (shape comparisons; LN-tagged yields with **external** π-exchange fraction; credibility cone only; no absolute inclusive comparison). |
| 4 | Is the in-cone 0.4266 discrepancy a data-composition effect? | **No — it is MC-side** (oo-4.0 FNC response cone + unmodelled proton-beam tilt + MC θ_n concentration: 87.9% of MC tags in-cone vs 29.9% in data; in-cone cluster-type composition also differs: data 24.5% photon-like vs MC 3.0%). **Sign argument (decisive):** the scaled MC over-predicts even the pure π-exchange component by ×2.34 in-cone; any data-side event-class removal can only lower data/pred further below 1. No diffractive-content hypothesis matches the sign pattern; a diffractive selection on data moves the in-cone ratio in the wrong direction. |
| 5 | Correct like-for-like construction | LN tag on data (`FncPresent && FncGoodRun`), **in-cone only**, with an a-priori harmonized cluster-type tag definition (e.g. hadron-like `FncClType∈{2,3}` on both sides, both variants reported); RAPGAP-π interpreted strictly as the **π-exchange component model**, scaled by the verified 1.15059; π-exchange fraction as an **external input**; MC recoil-dissociation composition (M_Y) carried as a model uncertainty; out-of-cone MC never used in ratios; **no event-class selection on data is warranted by the sample definitions** — diffractive or otherwise. |

**Overall verdict on the user proposal: UNSOUND** — on four independent grounds: (i) wrong subprocess premise (Q1); (ii) no selection tool exists in the study trees, and the MC itself is not free of target-side dissociation (Q2); (iii) invalid methodology, already superseded by the documented comparability verdict (Q3); (iv) the sign of the effect makes the proposal incapable of achieving even its own stated goal in the only region where the MC is credible (Q4). The legitimate kernel of the proposal — "the data tag sample is not compositionally identical to the MC" — is real and already handled by the documented construction (external π-exchange fraction, tag-definition harmonization, credibility cone), not by a diffractive selection.

---

## 1. Q1 — Subprocess identity: RAPGAP-π 9134 is pion exchange, not diffraction

**Verdict: RAPGAP-π ReqID 9134 is a pure pion-exchange leading-neutron sample. It is not a diffractive sample. Pion exchange and diffractive (pomeron/IP) exchange are different leading-particle mechanisms at HERA.**

Evidence:

1. **Production tags** (in every file name of the 101 HAT/MODS pairs, `rapgap_pi_mc_availability_2026-10-20.md` §2.1/§4; `fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` §6):
   `hat.4.0.25.RAPGAP.PIPEXCH.EPLUS.RAD.GRVLO.Q2GT3.smearing.9134.S39600.R97905.ftt.DST.NNNN.root`
   — `PIPEXCH` = pion exchange; `GRVLO` = GRV-LO **pion** PDF; Holtmann-**π** flux; **LN θ-smearing** `sigx=46.0e-6`, `sigy=168.0e-6`; `Q2GT3` = generator phase space Q²>3 GeV². Every one of these tags is π-exchange machinery; none is diffractive machinery.
2. **Pure-LN construction, re-verified in this task** (read-only ROOT, merged Stage-2 MC tree): `FncGenNFound == 1` for **29,842 / 29,842 = 100.00%** of high-Q²-selected events. Every event contains a leading forward generated neutron. This is the definition of a leading-neutron signal sample, not a diffractive sample (whose every-event signature would be a leading **proton**/intact-Y system and a large rapidity gap).
3. **Workspace generator documentation separates the two channels.** `MC/rapgap/README.md` (four-mode mini-production): diffractive modes are `diff_nonrad`/`diff_rad`, IPRO=12/1200 with **IPOM=-30 = H1 2006 set A pomeron flux** (σ_gen = 138.92 pb at Q²>150 in the workspace mini-production); `RAPGAP_pion_exchange_knobs_reference.md` (source-audited): pion exchange is `IPOM ∈ {20,21,22}` → exchanged PDG `KINT(2,2) ∈ {-211, 111, +211}` (π⁻/π⁰/π⁺) with recoil baryon Δ⁺⁺/p/**n** computed at `partdf.F:850-858`; the RAPGAP manual (L1110-1115) states the outgoing particle is "a **p, n or Δ⁺⁺ corresponding to π⁰, π⁺ or π⁻ exchange**". The shared `IDIR=0` steering channel ("pomeron **or** pion exchange", `rapcha.F:232`) is a common code path, **not** a common subprocess.
4. **Physics distinction at HERA.** Diffractive DIS proceeds by pomeron (IP) exchange: leading **proton** (or low-mass Y system) carrying x_L→1, large rapidity gap, M_X/M_Y topology, ~10% of DIS events. Pion exchange proceeds by π⁺ exchange off the proton: leading **neutron** at x_L ≈ 0.2–0.9, baryonic recoil, no LRG signature by construction. The two are the distinct leading-particle mechanisms measured by the separate published H1 leading-neutron and diffractive programs, each with its own MC (RAPGAP-π for LN; RAPGAP-diff/DIFFVM-class samples for diffraction). A "diffractive selection" on data has **no subprocess-matched counterpart** in RAPGAP-π 9134: selecting diffractive events in data moves the data sample *further* from the MC's pure-π-exchange content, not closer.
5. **Naming trap, measured and closed in this task.** The 9134 HAT tree carries the standard 219-branch OO HAT diffractive-*named* branches and they are **populated** (§2 below). Their presence does not make the sample diffractive: the HAT producer labels the `IDIR=0` two-system exchange topology with the diffractive branch names regardless of which Reggeon was exchanged. Here the exchange is a pion (`PIPEXCH`) and the recoil is baryonic (n/N*), per the tags in (1) and the generator source in (3).

---

## 2. Q2 — Diffractive content on both sides, and the observables actually available

**Verdict: no diffractive observable exists in the v6fnc study trees, on either side. The raw HAT files carry diffractive-named branches that in 9134 quantify π-exchange topology. A diffractive contribution to the data tag sample is plausible but small and unmeasurable with current tree content; the MC conversely contains a substantial dissociated-recoil component of its own. Neither direction supports "data selection must reproduce the other side's diffractive content".**

### 2.1 Study-tree content audit (new measurements, this task)

Single-file read-only ROOT checks (macro `fnc_diffractive_check.C`, §6), applying the exact rate-study selection (first `ChargedTot` row per `(RunNumber,EventNumber)`; `Q2es>150`, `0.1<Yes<0.7`, `Empz>45`, `|PVz|<35`; DATA `FncPresent&&FncGoodRun`, MC `FncPresent`):

| check | DATA ep0607 | RAPGAP-π 9134 |
|---|---|---|
| `ChargedTot` branches | **107** | **116** |
| branches matching pom/diff/lrg/gap/mx/my/diss (case-insensitive) | **0** | **0** |
| selected / tagged / in-cone tagged (binding reference counts) | 73,738 / 14,899 / 4,455 (**exact reproduction**) | 29,842 / 10,324 / 9,076 (**exact reproduction**) |
| `FncGenNFound==1` among selected | — | **29,842 (100%)** |

The Stage-1 EVLS schema (from `src_stage1/TreeBuilderHighQ2_chargedsidis_v6fnc.C`) is event-level DIS kinematics + trigger/vertex flags + the 16 reco FNC branches + the `FncGenN*` gen-neutron branches — also **no** diffrective observable. The Stage-2 116-branch schema (from `src/ChargedSidisTreeBuilderHighQ2_v6fnc.C`) likewise. **Conclusion: an x_Pomeron / M_X / LRG-gap / M_Y selection cannot be constructed from the existing v6fnc trees.** Using such observables at all would require a new Stage-1 campaign re-reading the raw HAT files with new branches — that is a production change, not "a selection", and it is not warranted by the physics below.

### 2.2 What the raw HAT truth actually contains (new measurements, this task)

`hat_value_scan.C` / `hat_gap_scan.C` / `hat_my_scan.C` on `/pnfs/.../9134/hat...DST.0000.root` (HAT, 90,000 events):

| HAT branch | populated fraction | reading |
|---|---|---|
| `Xpom` > 0 | 78,280/90,000 = **87.0%** (mean 0.045 of positives) | exchange momentum fraction in the shared `IDIR=0` formula — here the **pion-exchange** topology, not a pomeron |
| `MXGen` > 0 | 95.1% (mean 26.4 GeV) | mass of the γ*π→X system |
| `MYGen` > 0 | 98.1% (mean 16.3 GeV) | recoil-baryon-system mass: **elastic-neutron peak (m_n=0.9396±0.01) in only 31.3%** of positives; 1–3 GeV in 25.3%; **>3 GeV in 43.4%** |
| `TGen` < 0 | 96.9% | physical t<0 at the exchange vertex |
| `Gap` > 0, `GapTyp` > 0, `EtaGapU` valid | 99.2% / 99.2% / 100% | standard OO gap observables, computed for the hadronic final state; in a π-exchange event they reflect the forward-neutron topology, not diffractive-gap physics |

These branches are **not propagated** into the study trees (§2.1). Interpretation for the adjudication:

- **RAPGAP-π contains no diffractive (pomeron) component** — by subprocess tag and by construction (Q1). What it does contain is π exchange with a **mixed elastic/dissociated recoil**: only ~31% of events have an elastic neutron recoil; ~43% have M_Y > 3 GeV. (The exact production mechanism — e.g. an NFRA/prodiss setting, permitted for π⁺ exchange per `eventep.F:996` in the knobs reference — is not locally verifiable: `/acs` is unmounted and the MCDB CGI is disabled. The measured composition stands regardless.) So "proton-dissociation composition" is a **property of the MC production** that must be carried as a model uncertainty — it is not a data-side deficiency that a selection could or should reproduce.
- **Data side:** the FNC-tagged data sample is inclusive high-Q² DIS with an FNC-cluster tag. It plausibly contains, besides π-exchange neutrons: other LN mechanisms (N*→nπ⁺, π⁰/π⁻ exchange with dissociated baryons), **photon-like clusters** (`FncClType==1`: measured in this task at **24.5% of in-cone data tags** — see §4), and at most a small diffractive component. The latter is bounded small by kinematics: an elastic diffractive leading proton would appear at E ≈ 920 GeV (x_L→1); the data tagged content above 800 GeV is 143 events ≈ **1.0% of tagged** (rates doc FncE table, bin 8+9), and diffractive neutrons from dissociated M_Y systems are a sub-component of the same order. There is no stored observable that could isolate any of this.

**Bottom line:** the premise "data contains diffractive events that RAPGAP-π lacks, so we must select them out" is (a) unmeasurable with the existing trees, (b) quantitatively implausible (≈1% scale vs the 57% in-cone deficit that would need explaining), and (c) backwards on composition — the MC, not the data, is the side carrying a large target-dissociation component.

---

## 3. Q3 — Methodology: "select data until it agrees with MC" is not a valid like-for-like treatment

**Verdict: unsound as a methodology, independent of the specifics of this sample pair.**

1. **Circularity / agreement tuning.** A data selection whose free parameter is tuned until data/MC agreement is reached manufactures the conclusion it claims to test. It has no validating content: agreement with a deficient model is not evidence of correctness, it is evidence that the filter imported the model's deficiencies into the data.
2. **The MC is a-priori known to be deficient in exactly the compared region.** The oo-4.0 FNC simulation is an acceptance-limited fast response: credible only for θ_n ≲ 0.75 mrad (reco), response <1% beyond r ≈ 8–10 cm, ~1/3 of the real face radius and ~6% of its area, with no face edges, no beam-pipe slot, and no proton-beam tilt (`fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` §1, §5, §7). Tuning data to agree with this MC in any θ_n-dependent quantity freezes the sim's artifacts into the analysis.
3. **Destruction of the measurement.** The goal of the FNC study is the back-to-back h⁺–n correlation (`fnc_ptvector_decorrelation_2026-10-21.md`, Ψ observable). Any event-class filter applied to data "until agreement" conditions the data on the MC's event topology, biasing every subsequent correlation observable in an unquantifiable way.
4. **The documented treatment already exists and is correct.** `rapgap_pi_9134_normalization_2026-10-21.md` §7 (comparability verdict), restated as the mandatory framing of `fnc_rates_2026-10-25.md`:
   - *Meaningful:* (1) **shape-normalized** comparisons of LN kinematics (never absolute-normalized shapes); (2) **LN-tagged data yields vs RAPGAP-π predictions only with an explicit pion-exchange fraction input** — RAPGAP-π models the π-exchange component, data LN-tagged yields contain π-exchange + all other LN mechanisms; (3) FNC detector-response/acceptance studies **within the credibility window**.
   - *NOT meaningful:* absolute inclusive-data rate comparison (pure-π sample σ_gen = 32.78 nb vs inclusive DIS); absolute LN tag-rate comparison (34.596% MC vs 20.205% data vs 6.582% DJANGOH — sample-definition, not physics); absolute θ/φ comparisons beyond the cone (beam tilt unmodelled).
   - The π-exchange fraction **must come from an external input, not from these ratios**. A selection tuned to agreement would be exactly the forbidden internal derivation of that fraction — and derived on a detector-sim-deficient comparison at that.

**Conclusion:** no data-side event-class selection is part of the correct treatment. The correct treatment is the documented one: shape comparisons, external π-exchange fraction, credibility-cone restriction, and stated model uncertainties.

---

## 4. Q4 — Physics of the in-cone discrepancy: detector simulation, not data composition

**Verdict: the in-cone data/pred = 0.4266 ± 0.0078 (MC over-predicts ×2.34) is attributable to MC-side effects — the FNC response-cone geometry, the unmodelled proton-beam tilt, and the MC's θ_n concentration — not to the diffractive (or any other) composition of the data. A diffractive selection on data cannot repair it: the sign of the effect makes that impossible.**

### 4.1 The discrepancy's documented anatomy

- θ_n concentration (`fnc_rates_2026-10-25.md`): **87.9% of MC tags are in-cone vs 29.9% in data**; in-cone tag fractions of selected events: MC 30.414% vs data 6.042% (×5.0). Even inside the credibility cone both sub-bins are over-predicted (FncThetaN bins 0–0.375 mrad: 0.3350; 0.375–0.75 mrad: 0.6218).
- FNC sim response (`fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` §5, §6): hard response cliff at r ≈ 8–10 cm (θ ≈ 0.76–0.95 mrad); simulated aperture ≈ 1/3 of the real face radius, ~6% of its area; real FNC accepts θ up to 2.7–3.4 mrad over x∈[-30,+22], y∈[-20,+28] cm. At the cone center the sim responds calorimeter-like and **efficiently** (RAPGAP-π neutron response 87.7% at r<2 cm; E_reco/E_gen = 1.036 ± 0.115).
- Beam tilt/frame defect (same doc §5.2–§5.3): data clusters follow the proton-beam direction (flux blob at (x,y) ≈ (-1.3, +10.6) cm, θ_y ≈ +1.0 mrad tilt; data θ quantiles [0.074, 0.263, **1.221**, 2.136, 2.749] mrad — the **median** data cluster sits beyond the 0.75 mrad credibility cone); both MC generators emit neutrons azimuthally symmetric around the ideal +z axis (RAPGAP-π gen θ median 0.60 mrad, mean sin φ ≈ 0). The LN θ-smearing of the 9134 production (`sigy=168 μrad` = 0.168 mrad) does not model the tilt.

**Mechanism:** a real data neutron emitted along the proton-beam direction carries θ_n ≈ 1 mrad relative to the ideal axis and lands **outside** the reco cone; the MC's ideal-axis neutrons pile up **inside** it, where the sim tags them at high efficiency. This alone produces the observed sign pattern — MC in-cone over-prediction (×2.34) and data out-of-cone excess against a not-credible MC (×7.27). The same frame defect is documented as uncorrectable from the current branches (diagnosis doc §8.4).

### 4.2 The composition mismatch that *does* exist — and its sign (new measurement, this task)

`FncClType` composition of in-cone tags (macro `fnc_diffractive_check.C`, §6):

| `FncClType` | DATA in-cone | fraction | MC in-cone | fraction |
|---|---:|---:|---:|---:|
| 1 (photon-like) | 1,091 / 4,455 | **24.5%** | 271 / 9,076 | 3.0% |
| 2 (hadron-like) | 1,323 | 29.7% | 1,525 | 16.8% |
| 3 (hadron-like) | 2,041 | 45.8% | 7,280 | 80.2% |

The data tag is compositionally *dirtier* than the MC tag (photon-like clusters, mixed hadronic content) — a genuine like-for-like mismatch at the **tag-definition** level (addressed in §5), but again **not diffractive**, and again of the wrong sign for the proposal: removing photon-like clusters from data would lower the in-cone ratio to 3,364/10,442.8 = **0.322**, i.e. further from agreement.

### 4.3 The sign argument (decisive)

The scaled RAPGAP-π prediction is, by its own comparability framing, a prediction for the **π-exchange component of LN-tagged data only**. In-cone: data total (all LN mechanisms + photon-like + everything) = 4,455, while the π-exchange-only prediction = 10,442.8. Hence the MC over-predicts even the π-exchange component by **at least** ×2.34 — the data's π-exchange component is at most the total, and the total is already a factor 2.34 short. **Any data-side event-class removal (diffractive or otherwise) only subtracts from the numerator and drives data/pred further below 1.** Agreement in-cone cannot be reached from the data side by any selection whatsoever; the discrepancy is MC-side (sim geometry + frame + the production's θ_n/x_L spectrum — note also the high-x_L FncE bins 5–8, where π exchange dominates in data, are over-predicted ×4.4–8.0 in-cone).

A "diffractive contamination in data" hypothesis would predict a data **excess** over the π-exchange prediction. The observed in-cone deficit has the opposite sign; no diffractive-content hypothesis matches it. Out-of-cone, the data excess (×7.27) is fully accounted for by the documented sim response collapse (MC response ≈ 0 beyond the cone) — a region where the MC is flagged **not credible** and must not be used for any ratio, so there is nothing legitimate to "repair" there either.

---

## 5. Q5 — Recommended like-for-like comparison construction (h⁺–n back-to-back correlation study)

**Verdict: construct the comparison by tag definition + interpretation discipline, not by data-side event-class selection. No diffractive (or other) event-class selection on data is warranted by the sample definitions.**

Recommended construction:

1. **Data tag:** `FncPresent && FncGoodRun` on the standard high-Q² selection (`Q2es>150`, `0.1<Yes<0.7`, `Empz>45`, `|PVz|<35`), restricted to the **credibility cone** `FncThetaN < 0.75 mrad` for any data/MC ratio or acceptance statement. Out-of-cone data is descriptive only; out-of-cone MC is never used.
2. **Tag-definition harmonization, fixed a priori:** given the measured composition mismatch (§4.2), harmonize the cluster-type requirement on **both** sides — nominal `FncClType ∈ {2,3}` (hadron-like, suppressing the data photon-like contamination, consistent with published LN methodology) with the no-ClType-cut variant reported as the systematic bracket. The choice must be fixed from the composition evidence and external methodology, **never** tuned to agreement. (Reported for completeness: hadron-like-only data in-cone = 3,364, ratio 0.322 — the harmonization worsens the in-cone rate ratio, which is expected and harmless because rate ratios in-cone are not the physics product; §3.)
3. **MC interpretation:** RAPGAP-π 9134 (single ReqID; never combined with 8946/9137) is the **π-exchange component model only**, scaled by the verified `scale = 1.15059` (`L_data = 351,000.0 nb⁻¹`, `L_MC = 305,060 nb⁻¹`, unit weights verified at the HAT source). It is never an inclusive-DIS prediction and never a tag-rate prediction.
4. **π-exchange fraction:** an **external input** (published H1 leading-neutron measurements / dedicated studies), with its uncertainty propagated. Data LN-tagged yields are then reported *against* the externally-fractioned π-exchange expectation; the residual non-π fraction is a measurement statement about data, not a tuning target.
5. **Neutron-source / recoil composition:** carry as stated model uncertainty — (a) the MC's own recoil composition (elastic n only ~31%, M_Y > 3 GeV in ~43% of generated events, §2.2) is a production property, documented as a caveat on the π-flux model; (b) the data neutron-source mix (π exchange, N* decays, other) is not separable from the stored branches; (c) the `FncClType` composition difference is handled at tag level (item 2). None of these is a data-side event selection.
6. **The h⁺–n correlation observable (Ψ):** keep it a **shape-level** comparison (unit-area, as already produced in `fnc_ptvector_decorrelation_2026-10-21.md`), with the error-model split at `pTn = 0.5 GeV`, and carry the frame caveat explicitly: the missing beam tilt makes the MC's neutron azimuth symmetric where the data's is tilted (blob at θ_y ≈ +1 mrad), and the FNC φ coverage is detector geometry, not physics. Any future acceptance-corrected correlation requires a full beamline+FNC GEANT simulation or an external/data-driven acceptance map (diagnosis doc §8) — the current oo-4.0 FNC sim cannot provide it, even in-cone, for absolute θ/φ quantities. **This — not data composition — is the actual blocker for the correlation study.**
7. **If a diffractive control is ever genuinely wanted** (it is not required by this comparison): it needs (a) new tree content (Gap/Xpom/MX/MY from the raw HAT via a Stage-1 extension — the branches exist on both data and MC HAT per the 219-branch schema, `rapgap_pi_mc_availability_2026-10-20.md` §4) and/or (b) a dedicated diffractive MC (workspace RAPGAP diff modes exist only as 10k-event generator-level mini-productions, pure pomeron IPOM=-30, exploratory per the sidis-hq2-mc skill). It would be a **control sample study**, routed via h1-mc-expert / h1-mc-db for provenance — never a data selection tuned to agreement.

---

## 6. Audit trail (all checks read-only, single-file, plain ROOT 6, ≤5 min each)

| check | input (one file) | result |
|---|---|---|
| `fnc_diffractive_check.C` | `H1ChargedSidis_HighQ2/output_v6fnc/ep0607_study/stage2/merged/chargedsidis_v6fnc_d07_ep0607_merged.root` | 107 branches, **0 diffractive-suspect**; counts 73,738/14,899/4,455 reproduced exactly; ClType table §4.2 |
| `fnc_diffractive_check.C` | `H1ChargedSidis_HighQ2/output_v6fnc/rapgap_pi_9134_study/stage2/merged/chargedsidis_v6fnc_rapgap_pi_9134_merged.root` | 116 branches, **0 diffractive-suspect**; counts 29,842/10,324/9,076 reproduced exactly; `FncGenNFound` 100%; ClType table §4.2 |
| `hat_branch_scan.C` | `/pnfs/.../9134/hat...DST.0000.root` | 219 HAT branches; diffractive-named set present (`Xpom`, `XpomGki`, `MXGen`, `MYGen`, `TGen`, `Gap`, `GapTyp`, `EtaGapU`) |
| `hat_value_scan.C` + `hat_gap_scan.C` + `hat_my_scan.C` | same HAT file | `Xpom`>0 87.0%; `MXGen`>0 95.1%; `MYGen`>0 98.1% with elastic-n 31.3% / 1–3 GeV 25.3% / >3 GeV 43.4%; `TGen`<0 96.9%; `Gap`>0 99.2% |
| schema enumeration from source | `src_stage1/TreeBuilderHighQ2_chargedsidis_v6fnc.C`, `src/ChargedSidisTreeBuilderHighQ2_v6fnc.C` | Stage-1 EVLS and Stage-2 `ChargedTot` schemas contain no diffractive observable; `FncClType` semantics: 1 = photon-like, 2/3 = neutron/hadron-like |

Macros are transient under `/tmp/opencode/` and regenerable; every number they produced is tabulated above. **No selection, cut, weighting, or file change was applied to any data or MC product; no output ROOT/PDF/JSON artifact was created.**

## 7. Documentary inputs

- `fnc_rates_2026-10-25.md` (rate study, mandatory framing, binned ratios, θ_n credibility flags)
- `rapgap_pi_9134_normalization_2026-10-21.md` (§7 comparability verdict; scale 1.15059; σ_gen = 32.78 nb; unit weights)
- `fnc_rapgap_pi_comparison_2026-10-20.md`, `fnc_rapgap_pi_highQ2_detailed_2026-10-21.md` (sample content, tag fractions 20.205%/34.596%/6.582%, Q²-scope correction)
- `fnc_mc_matching_efficiency_diagnosis_2026-10-20.md` (FNC sim response-cone collapse; aperture ~1/3 face radius, ~6% area; beam tilt; cluster-producer composition)
- `rapgap_pi_mc_availability_2026-10-20.md` (production tags; ReqID/genID structure; overlap verdict)
- `fnc_ptvector_decorrelation_2026-10-21.md`, `fnc_hadron_neutron_correlations_stage2_2026-10-20.md` (the h⁺–n correlation goal and its current shape-level status)
- `sidis-hq2-mc` skill + `MC/rapgap/README.md`, `MC/rapgap/RAPGAP_pion_exchange_knobs_reference.md`, `MC/rapgap/RAPGAP_pion_exchange_NaN_diagnosis.md` (diffractive vs pion-exchange steering; recoil-baryon computation; p-diss guard asymmetry)

*End of assessment. Read-only adjudication; no data products modified.*
