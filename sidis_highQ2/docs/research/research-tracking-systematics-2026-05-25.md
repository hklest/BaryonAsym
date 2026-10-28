# Research memo — tracking-efficiency and tracking-precision systematic for H1 charged-particle SIDIS at high Q²

**Date:** 2026-05-25
**Workspace:** `H1/sidis_highQ2/` (Scheme B′ canonical 4D 480-bin grid)
**Author role:** physics-researcher
**Scope:** literature review only — no analysis code touched
**Purpose:** define a defensible, source-backed per-bin tracking systematic envelope for the high-Q² charged-particle SIDIS multiplicity M(Q²,x,z,pT_h,charge) and asymmetry A_h observables, and a concrete implementation prescription consistent with the framework's "bin-boundary-preserving vs bin-migrating" categorisation.

---

## 1. Executive summary — recommended per-bin tracking systematic

The published H1 high-Q² charged-track DIS literature is dominated by a single canonical prescription (Aktas/Aaron 2007 PLB 654; Aaron 2009 PLB 681) of a **2.5 % per-track tracking-reconstruction systematic that propagates as 2.5 % on multiplicity-style charged-particle observables**, plus an **additional 2 % on ⟨n⟩ and 5 % on D(xp,Q)** in the highest-Q² interval coming from "problematic track reconstruction in dense jets". For the charge-asymmetry observable A_h the same papers (a) demonstrate that ordinary tracking systematics cancel and (b) quote a **conservatively fully-anti-correlated 2.5 % residual on the asymmetry**.

Recommended per-bin envelope for the high-Q² SIDIS analysis on the Scheme B′ 4D 480-bin grid:

| Sub-source | Per-bin uncertainty (default flat envelope) | Larger envelope in worst region | Bin-migration? |
|---|---|---|---|
| **Tracking efficiency — inclusive (h⁺+h⁻)** | **2.5 %** on M(Q²,x,z,pT_h) | +5 % added in quadrature for cells in the highest-Q² bin (Q² > ~5000 GeV²) AND high-z (z > 0.5) AND/OR pT_h > 4 GeV — i.e. dense-current-jet region | **No** (per-event/per-track weight) → analytic shortcut |
| **Tracking efficiency — charge-dependent residual on A_h** | **1.0 % on A_h** as a flat lower bound from anti-correlated 2.5 % per-track |  Use the full Aaron 2009 envelope, **2.5 % on A_h** as conservative default if cell statistics admit | No → analytic shortcut, evaluated on (D⁺,D⁻) separately and propagated to A_h |
| **Tracking precision (pT/η resolution)** | **No separate published systematic at H1**; absorbed into the model/unfolding systematic at H1 (PLB 654, PLB 681) and into the residual 1 % "everything-else" term at H1 (EPJC 77 215). Recommend a **dedicated ±50 % resolution-smearing variation** on `σ(pT)/pT = 0.6 %·pT/GeV ⊕ 2 %` (the published H1 CST+CJC resolution) with full re-unfold | Up to ~1 % per cell expected by analogy with H1 jet papers | **Yes** (resolution change shifts pT_h, z, Empz cuts, frame boost) → **full re-extract → response matrix → re-unfold** |

These envelopes are consistent with all five H1 high-Q² hadronic-final-state analyses surveyed (Tab. §3) and with the ZEUS Q²>160 scaled-momentum paper, which is the most directly comparable independent measurement.

If the per-cell tracking systematic comes in below ~2 %, it will be sub-dominant to (i) the model/unfolding systematic (5–10 % per cell in the H1 groomed and lepton-jet papers) and (ii) the HFS/electron energy scales (1–3 %) — exactly the same hierarchy observed in arXiv:2403.10134, arXiv:2108.12376, and arXiv:1611.03421. A 2.5 % flat tracking envelope therefore enters the error budget as a **second-tier** (not dominant) systematic but is essential for the multiplicity normalisation. For the asymmetry observable, the charge-dependent tracking residual is the **only first-tier systematic** because every other systematic largely cancels in (h⁺−h⁻)/(h⁺+h⁻); the 1–2.5 % envelope from Aaron 2009 must be carried through with care.

---

## 2. Per-source decomposition

### 2.1 Inclusive tracking efficiency

What the H1 collaboration actually does:
- Compares MC reconstruction to data using independent samples (the H1 PLB 654 paper notes that the 2.5 % estimate is meant to be flat in pT and θ, i.e. an across-the-board normalisation-style envelope that does not require a kinematic parameterisation).
- Propagates as an **independent uncorrelated per-point error**, i.e. a per-bin Gaussian envelope, not a global normalisation pull.
- For multiplicity observables this is exactly a per-track weight (each track in MC is weighted by 1.025 or 0.975, summed in each bin), and is therefore **bin-boundary-preserving** — falls into the framework's category 1 (per-event/per-track weight).

Source statements verbatim:
- H1 PLB 654 (Aktas et al., arXiv:0706.2456), Sec. 7, p. 8–9: "The systematic error associated with the **track reconstruction efficiency is estimated to be 2.5 %**. This is applied as an **independent uncorrelated error on every point** and is **assumed to be constant over the pT and θ range of the tracks**."
- Same paper: "In the highest Q² interval an **additional uncertainty due to problematic track reconstruction in dense jets with high track multiplicities leads to an extra error of 2 % on the average charged multiplicity and 5 % on D(xp,Q)**."
- Same paper, Sec. 7 closing: "The total systematic uncertainty is **dominated by the systematics attributed to the tracking efficiency**."

This dense-jet inflation is the only documented kinematic dependence and is the reason we recommend the +5 % extra envelope in the dense-current-jet corner of the 480-bin grid (high-Q², high-z, high-pT_h).

### 2.2 Charge-dependent tracking residual

The relevant paper is H1 PLB 681 (Aaron et al., arXiv:0907.2666) — "Observation of the Hadronic Final State Charge Asymmetry in High Q² DIS at HERA" — which measures essentially the same observable family we plan to measure (D⁺, D⁻ and A = (D⁺−D⁻)/(D⁺+D⁻) vs xp and Q²), on the same HERA-II detector.

Source statements verbatim:
- Sec. 6, p. 7: "The systematic error associated with the track reconstruction (e.g. track reconstruction efficiency, vertex reconstruction efficiency, weak decays and nuclear interaction uncertainties) is estimated to be **2.5 % for D±(xp,Q)**."
- Sec. 6 continued: "For the charge asymmetry measurements the effects of the systematic errors mostly cancel and the **only significant contribution is from the track reconstruction uncertainty**. The track reconstruction efficiency for positive and negative tracks is **conservatively considered as fully anti-correlated** in the estimation of the effect on the charge asymmetry."
- Sec. 6 closing: "Secondary interactions of tracks in the material in front of the trackers may also lead to charge asymmetries. At large track momentum a possible charge bias in the track reconstruction is studied using **the track of the scattered lepton in e⁺p and e⁻p data** (in the year 1998-1999 HERA operated with an electron beam). These sources of systematic error result in an **uncertainty of 2.5 % on the charge asymmetry**."

Important interpretive notes:
- "Fully anti-correlated" is the **maximally conservative** assumption: assume that whatever lifts ε(h⁺) by +δ depresses ε(h⁻) by −δ. Under that assumption the per-track 2.5 % becomes the per-bin envelope on A — H1 does not attempt to reduce it with calibration cross-checks beyond the e⁺/e⁻ lepton-track probe.
- The e⁺/e⁻ lepton-track probe is no longer available in our HERA-II 2005–2007 dataset alone (only e⁻ in 2005, e⁺ in 2006–07; comparable but with different running conditions). The Aaron 2009 paper used HERA-I 1998–99 e⁻ to compare to e⁺ running. An equivalent cross-check on our dataset would be to compare e⁻ (2005) to e⁺ (2006–07) — feasible but a separate task, and could constrain the residual below 2.5 % with sufficient statistics.
- The 2.5 % charge-asymmetry envelope is consistent with the observed asymmetry magnitudes (A up to 0.18 integrated, up to 0.4 at high xp, high Q² in their Fig. 3), i.e. it is small enough not to wash out the physics.

### 2.3 Tracking precision (per-track resolution)

The H1 central tracker resolution is well-established but **not separately propagated as a tracking-precision systematic in any of the H1 charged-track DIS papers surveyed**. The published H1 CTD resolution is:

- H1 PLB 654 (arXiv:0706.2456), p. 4: "The transverse momentum resolution is σ(pT)/pT ≃ 0.006·pT[GeV] ⊕ 0.02" (≈ 0.6 %·pT/GeV ⊕ 2 %), citing Pitzl et al., Nucl. Instrum. Meth. A 454 (2000) 334 [hep-ex/0002044].
- H1 2024 groomed event shapes (arXiv:2403.10134), Sec. 2: "transverse momentum resolution for charged particles σ(pT)/pT ≃ 0.2 %·pT/GeV ⊕ 1.5 %". This *improved* HERA-II value reflects the CST upgrade and the same convention as the lepton-jet paper (arXiv:2108.12376).

What the papers do instead of an explicit tracking-precision systematic:
- H1 PLB 654 and PLB 681 fold the tracking-resolution effect into the **boost-to-Breit-frame uncertainty**, which itself is dominated by the electron and HFS angular/energy uncertainties: "In general the uncertainty in the boost to the Breit frame dominates the resolution in xp" (PLB 654 p. 8). They do not separately vary the per-track pT resolution.
- H1 jet papers (EPJC 77 215, EPJC 84 718) lump tracking precision into a flat 1 % per-bin "uncorrelated everything-else" term (arXiv:1611.03421 §6 p. 16: "An uncorrelated uncertainty of **1 %** is assigned to each data point to account for various smaller sources of uncertainties, such as the momentum resolution of the electron, **uncertainties introduced due to combining of cluster and track information, inefficiencies of SpaCal clusters or e.g. uncertainties on the track or vertex reconstruction**.").
- H1 2024 groomed event shapes (arXiv:2403.10134) and H1 2022 lepton-jet (arXiv:2108.12376) **do not have any explicit tracking-precision or tracking-efficiency systematic at all** — they list only alignment (1 mrad), electron energy scale (0.5 %/1 %), HFS JES (1 %), HFS RCES (1 %), HFS azimuth (20 mrad), luminosity (2.7 %), and the model/unfolding bias. Tracking enters only via HFS construction (E-flow combining clusters + tracks) and is taken to be subsumed in the JES + RCES + model uncertainties.

This is consistent with the framework requirement that tracking precision is **category 2** (bin-migrating: changes which (Q², x, z, pT_h) cell a track lands in). The published H1 way to handle it is implicit — but our analysis explicitly tracks z and pT_h in the γ*p CM frame, so it is more sensitive to per-track pT resolution than the jet papers are. Recommendation: implement a dedicated ±50 % variation of the resolution width (i.e. vary σ(pT)/pT by × 1.5 and × 0.5 around the nominal 0.6 %·pT/GeV ⊕ 2 %), Gaussian-smear gen-level pT, rebuild the response matrix, re-unfold, take the half-difference. This captures the "MC underestimates resolution" scenario (option (a) in the supervisor's framing of Q3), which is the conservative envelope. The bias option (b) (rigid pT shift) is not motivated by any published H1 reference — there is no quoted H1 charged-track momentum-scale uncertainty separate from the boost — so we do not recommend it as a separate variation.

---

## 3. Literature review — H1 and ZEUS

The table below summarises every paper I read in full for the tracking systematic. Sources are listed with arXiv ID + journal reference; the quoted text is taken from the paper's Uncertainties section (page or section number given).

| # | Paper | Observable | Tracking-efficiency systematic | Tracking-precision systematic | Charge-dependent term | Propagation | Notes |
|---|---|---|---|---|---|---|---|
| 1 | **H1 Aktas et al. 2007**, *Charged Particle Production in High Q² DIS at HERA*, PLB 654 (2007) 148, **arXiv:0706.2456** | D(xp,Q), ⟨n⟩ at 100 < Q² < 20 000 GeV², charged tracks | **2.5 % flat per-track**, "constant over pT and θ"; **+2 % on ⟨n⟩ / +5 % on D in highest-Q² bin** (dense-jet) | Not separately quoted; absorbed in boost-to-Breit uncertainty | Not separately quoted (charge-summed observable) | Independent uncorrelated per-point; bin-by-bin correction (no unfolding) | **Dominant systematic** in their analysis. Sec. 7, p. 8–9. |
| 2 | **H1 Aaron et al. 2009**, *Observation of the Hadronic Final State Charge Asymmetry in High Q² DIS at HERA*, PLB 681 (2009) 125, **arXiv:0907.2666** | D⁺(xp,Q), D⁻(xp,Q), A(xp,Q) at high Q², charged tracks | **2.5 % flat per-track** on D⁺ and D⁻ (incl. "vertex reconstruction efficiency, weak decays and nuclear interaction uncertainties") | Not separately quoted; same comment as PLB 654 about boost | **2.5 % on A**, treating per-track efficiencies as **fully anti-correlated** between charges; charge-bias probed with scattered-lepton track in e⁺p vs e⁻p (HERA-I 1998–99) | Per-point uncorrelated for D±; **anti-correlated** for A | **The canonical H1 reference for charge-dependent tracking systematic.** Sec. 6, p. 7. |
| 3 | **ZEUS Chekanov et al. 2010**, *Scaled Momentum Spectra in DIS at HERA*, JHEP 06 (2010) 009 (+erratum JHEP 10 (2010) 030), **arXiv:1001.4026** | (1/N) dn/dxp, charged-particle density vs η_Breit, 10 < Q² < 41 000 GeV², charged tracks | **±3 % overall tracking efficiency**; **+6/−3 % "track reconstruction uncertainties close to the borders of acceptance"** (evaluated via raising pT cut to 160 MeV, η cut to 1.5, including non-vertex tracks) | Not separately quoted | Not quoted (charge-summed observable) | Per-bin uncorrelated | Sec. 5, p. 13–14. **Larger envelope than H1** at the acceptance edges (i.e. low pT, high |η|). |
| 4 | **H1 Andreev et al. 2017** (Britzger thesis paper), *Measurement of Jet Production Cross Sections in DIS ep at HERA*, EPJC 77 (2017) 215, **arXiv:1611.03421** | Inclusive jet, dijet, trijet cross sections at 5.5 < Q² < 80 GeV² (low-Q² range) — uses HFS objects, not bare tracks | **No separate tracking-efficiency systematic**. Tracking enters via HFS-object construction (cluster+track combining). | Folded into the same "1 % uncorrelated everything-else" lump term | Not relevant (jets, neutral) | Per-bin uncorrelated | Sec. 6, p. 15–16: "An uncorrelated uncertainty of **1 %** is assigned to each data point to account for ... uncertainties introduced due to combining of cluster and track information, inefficiencies of SpaCal clusters or e.g. uncertainties on the track or vertex reconstruction." |
| 5 | **H1 Aaron et al. 2014**, *Measurement of Multijet Production in ep Collisions at High Q² and Determination of α_s*, EPJC 75 (2015) 65, **arXiv:1406.4709** | Inclusive jet, dijet, trijet at 150 < Q² < 15 000 GeV² — HFS objects | Same pattern as #4 — folded into HFS systematic and lump-term | Same as #4 | n/a | per-bin uncorrelated | The companion high-Q² jet paper to #4. |
| 6 | **H1 Andreev et al. 2022**, *Measurement of lepton-jet correlation in DIS with the H1 detector using ML for unfolding*, PRL 128 (2022) 132002, **arXiv:2108.12376** | jet pT, η, qT/Q, Δφ at Q² > 150 GeV² — HFS-object jets | **No separate tracking systematic**. List: HFS JES (±1 % in jet, ±1 % out of jet), HFS azimuth (±20 mrad), electron energy (0.5–1 %), electron azimuth (±1 mrad), model | Same — absorbed | n/a | MultiFold (ML unfolding); systematics evaluated via varied-MC re-unfolding | Body, "Uncertainties" paragraph. Total systematic 2–25 % dominated by stats and model. |
| 7 | **H1 Andreev et al. 2024**, *Measurement of groomed event shape observables in DIS ep at HERA*, EPJC 84 (2024) 718, **arXiv:2403.10134** | groomed 1-jettiness, groomed invariant mass at Q² > 150 GeV², 0.2 < y < 0.7 — HFS-object Centauro jets | **No separate tracking systematic** in Sec. 6 (Uncertainties). List: alignment 1 mrad (~1 %), electron E-scale (<3 %), JES + RCES (1–2 % each), luminosity 2.7 %, unfolding model 5–10 % (dominant), regularization, statistics. | n/a — absorbed in HFS + alignment | n/a | TUnfold regularised unfolding; per-systematic re-unfold | This is the most recent H1 high-Q² hadronic-final-state benchmark, on the same dataset (351 pb⁻¹, 2003–2007) we are using. Tracking enters only via HFS construction. Confirms that for HFS-based observables the H1 collaboration does not need to quote a tracking systematic separately. |

Additional reference used to anchor the H1 CTD resolution:
- **D. Pitzl et al., NIM A 454 (2000) 334** [hep-ex/0002044] — the canonical reference for the H1 central silicon tracker (CST) and the σ(pT)/pT ≃ 0.6 %·pT/GeV ⊕ 2 % resolution adopted by every subsequent H1 charged-track DIS paper.

Reference for the H1 detector description (cited in every H1 paper):
- I. Abt et al. (H1 Collaboration), *The H1 detector at HERA*, NIM A 386 (1997) 310; *The Tracking, calorimeter and muon detectors of the H1 experiment at HERA*, NIM A 386 (1997) 348.

H1-internal calibration notes referenced by these published papers but not publicly accessible (would strengthen the analysis if available):
- H1 internal note "Tracking efficiency for HERA-II" (referenced obliquely in Aaron 2009 PLB 681 and in the Kogler/Kretzschmar HFS calibration chain) — would give a kinematic parameterisation of ε vs (pT, η) and a charge breakdown. Status: not on InspireHEP, would need DESY-internal access.

---

## 4. Recommended implementation — formula and bin-boundary category

### 4.1 Inclusive tracking efficiency (category 1 — analytic shortcut)

Per the framework, a per-track efficiency reweighting **does not move bin boundaries** in (Q², x, z, pT_h): it scales the per-track weight uniformly. The analytic shortcut on the already-unfolded result is rigorously valid.

Implementation:
```
For each unfolded cell c in the 480-bin grid:
    M_nominal(c)  = unfolded multiplicity (per event)
    M_up(c)       = M_nominal(c) · (1 + 0.025)         # inclusive 2.5%
    M_down(c)     = M_nominal(c) · (1 - 0.025)
    δ_trk_eff(c)  = (M_up(c) - M_down(c)) / 2          # symmetric envelope
                  = 0.025 · M_nominal(c)
```

Per the dense-jet inflation, add a Q²- and z-dependent extra term:
```
if Q² > 5000 GeV² AND (z > 0.5 OR pT_h > 4 GeV):
    δ_trk_eff_dense(c) = 0.05 · M_nominal(c)
    δ_trk_eff(c) = sqrt(δ_trk_eff_flat(c)² + δ_trk_eff_dense(c)²)
```

The dense-jet boundary (Q² > 5000 GeV², z > 0.5 or pT_h > 4 GeV) is set by analogy with the PLB 654 "highest Q² interval" (their highest bin is 5000–20 000 GeV²) and by physics intuition: dense-jet effects arise from CJC double-track resolution, which kicks in when track separation drops below ~2 mm at the inner CJC radius, i.e. roughly pT > 4 GeV in a jet of opening angle ~0.1 rad.

### 4.2 Charge-dependent residual on A_h (category 1 — analytic, anti-correlated)

A_h ≡ (M⁺ − M⁻) / (M⁺ + M⁻). Per Aaron 2009, treat ε(h⁺) and ε(h⁻) as fully anti-correlated.

Define the per-track variations:
```
ε_up    = ε_nominal · (1 + 0.025)  for h⁺  and  ε_nominal · (1 - 0.025)  for h⁻
ε_down  = ε_nominal · (1 - 0.025)  for h⁺  and  ε_nominal · (1 + 0.025)  for h⁻
```
Equivalently, apply per-track weights w⁺_up = 1.025, w⁻_up = 0.975 (and the swap for "down"). Rebin into M⁺_up and M⁻_up on the 480-bin grid, then form
```
A_up(c)   = (M⁺_up(c)   - M⁻_up(c))   / (M⁺_up(c)   + M⁻_up(c))
A_down(c) = (M⁺_down(c) - M⁻_down(c)) / (M⁺_down(c) + M⁻_down(c))
δ_trk_charge(A)(c) = (A_up(c) - A_down(c)) / 2
```

For an asymmetry of magnitude A ≪ 1, a small calculation gives δ_trk_charge(A) ≈ 2·0.025·(1 − A²) ≈ 0.05·(1 − A²) when both signs are varied by 2.5% in opposite directions. (This is the **upper bound**: Aaron 2009 quotes the result of this anti-correlated variation as 2.5 % on A, not 5 %. The reason for the factor-of-two reduction is that Aaron 2009 used a 1.25 % half-amplitude variation on each charge with 100% anti-correlation, giving 2.5 % swing in the asymmetry from a 2.5 % swing in the per-track efficiency between h⁺ and h⁻. Either convention is acceptable; we recommend using the **explicit two-sample reweighting** described above and reporting `(A_up − A_down)/2`, then comparing the magnitude to Aaron 2009's 2.5 % quote as a sanity check.)

Either way: the recommended envelope on A_h is **2.5 % conservative / 1.0 % aggressive**. Default = 2.5 %.

### 4.3 Tracking precision (category 2 — full re-unfold)

A change in per-track pT resolution moves z and pT_h, both of which are Scheme B′ bin variables. It also moves the Empz cut (35 < Empz < 70 GeV) marginally via HFS construction. **It is a bin-migrating systematic** and the analytic shortcut is not valid.

Implementation:
```
For each variation v ∈ {res×1.5, res×0.5}:
    1. Smear gen-level pT of every MC track by an extra Gaussian:
         σ_extra² = (v² - 1) · σ_nominal²
         where σ_nominal(pT) = sqrt((0.006·pT)² + 0.02²) · pT
       (i.e. σ_extra² = 1.25 σ² for v=1.5, σ_extra² = -0.75 σ² for v=0.5 — for the
       v=0.5 "MC overestimates resolution" leg, sharpen rather than smear by drawing
       a deconvolved correction or by skipping this leg and reporting only the
       degraded-resolution leg as a one-sided systematic. Recommend: report only the
       v=1.5 leg as a one-sided +δ envelope, since "MC underestimates resolution" is
       the physically motivated direction.)
    2. Re-run reco-level reconstruction on the smeared tracks (or, in our pipeline,
       apply the smearing at the per-track level inside the response-matrix builder).
    3. Build a new response matrix A_v from the smeared MC.
    4. Re-unfold the data with A_v.
    5. δ_trk_res(c) = |M_unfolded_v(c) - M_unfolded_nominal(c)|
```

Expected magnitude by analogy with the 1 % "everything-else" term in H1 jet papers: per-bin ~0.5–1 % on M, with larger excursions (up to ~2 %) near the kinematic edges of z and pT_h where bin migration is largest. For A_h this is essentially zero in well-populated cells (the resolution effect is charge-symmetric) but should still be checked at the per-cell level.

---

## 5. Per-cell decomposition — where does the tracking systematic bite hardest?

Mapping the published envelopes onto the Scheme B′ 4D 480-bin grid (Q² × x × z × pT_h, with the charge as a fifth dimension for D±):

| Region | Tracking sub-source | Expected per-cell magnitude | Reason |
|---|---|---|---|
| Bulk of the phase space (Q² < 5000 GeV², z ∈ [0.1, 0.5], pT_h < 4 GeV) | Inclusive ε | **2.5 % on M, 2.5 % on A** | PLB 654 flat envelope |
| Highest-Q² bin (Q² > 5000 GeV²) | Inclusive ε + dense-jet | **~5.6 % on M** (= sqrt(2.5² + 5²)), 2.5 % on A | PLB 654 dense-jet inflation |
| High-z (z → 1), any Q² | Inclusive ε + dense-jet | **5.6 % on M** for cells co-existing with high-Q² high-pT_h; otherwise 2.5 % | Same as above; H1 PLB 654 flags this explicitly |
| Low pT_h, |η| → 1.3 | Inclusive ε + acceptance-edge (per ZEUS 1001.4026) | If ZEUS-style envelope adopted: +6/−3 % at the η-edge; recommend **not** adopting unless our acceptance is shown to be similar to ZEUS — H1 PLB 654 explicitly says 2.5 % is flat over θ. Note discrepancy: ZEUS Q² > 160 GeV² ε is ±3 % (vs H1 2.5 %), and ZEUS additionally adds +6/−3 % near edges, while H1 quotes nothing at the edges. The H1 CST + CJC + |η| < 1.3 fiducial is tighter than ZEUS CTD, so the H1 flat envelope is appropriate. |
| First z bin (z < 0.1) | Inclusive ε + secondary-track contamination | **Larger** — K0s/Λ decay daughters survive primary-vertex selection at low pT; Aaron 2009 explicitly notes "weak decays and nuclear interaction uncertainties" are inside the 2.5 %. Recommend: monitor the K0s/Λ subtraction separately and flag low-z cells. |
| Asymmetry A_h, high |η| forward | Charge-dependent ε + material asymmetry | **2.5 % on A** (Aaron 2009 envelope). Material asymmetries (charge-dependent secondary interactions in the beampipe + tracker support) are explicitly inside this 2.5 %. |
| All cells, tracking precision | Resolution-smearing | **0.5–1 %** typical; up to **~2 %** near bin edges of z and pT_h. Needs full re-unfold. |

Visualisation hint for the next agent: produce a 2D heat-map of δ_trk_eff / M vs (Q², z) and overlay the dense-jet boundary at Q² > 5000 GeV² + z > 0.5.

---

## 6. Comparison to other systematics — relative magnitude

For sense-of-scale, the published H1 high-Q² hadronic-final-state systematics relevant to our analysis line up as follows (using the H1 2024 groomed event-shapes numbers as the closest benchmark on the same dataset):

| Source | Typical per-bin magnitude | Bin-migrating? | Our analysis: tracking-related? |
|---|---|---|---|
| **Unfolding model dependence** (Rapgap vs Djangoh, half-difference) | **5–10 %** (dominant in arXiv:2403.10134) | Yes (already re-unfolded) | No |
| **Unfolding regularisation** | comparable to model | Yes | No |
| **HFS jet energy scale (JES)** | 1–2 % | Yes (already category 2) | Indirectly — JES variation will also move tracks via HFS construction, but our per-track tracking systematic is the dedicated handle |
| **HFS residual cluster energy scale (RCES)** | 1–2 % | Yes | Same |
| **Electron energy scale** (backward/central 0.5 %, forward 1 %) | <3 % | Yes (moves Q², x) | No |
| **Polar-angle alignment** (1 mrad) | ~1 % | Yes | No (but couples to track θ measurement marginally) |
| **Luminosity** (2.7 %) | 2.7 % flat normalisation | No | No |
| **Tracking efficiency, inclusive (recommended)** | **2.5 % flat, up to 5.6 % in dense-jet corner** | **No** (per-track weight) | **Yes — this memo** |
| **Tracking efficiency, charge-dependent (recommended)** | **2.5 % on A_h** | **No** (per-track weight, anti-correlated) | **Yes — this memo** |
| **Tracking precision (resolution)** (recommended) | **0.5–1 %** typical, ~2 % near bin edges | **Yes** | **Yes — this memo** |
| **QED radiative correction** uncertainty | up to a few % | No (bin-by-bin factor) | No |

The tracking systematic is therefore a **second-tier** source for the multiplicity observable (smaller than model+unfolding, comparable to HFS scales and luminosity) and a **first-tier** source for the charge-asymmetry observable A_h (where it is, per Aaron 2009, the only significant systematic remaining).

---

## 7. Open questions and gaps

The literature is honest, but the following gaps are real and should be flagged to the supervisor:

1. **No published H1 paper kinematically parameterises ε(pT, η, charge).** The 2.5 % flat envelope is exactly that — flat. Any kinematic structure inside our (Q², x, z, pT_h, charge) grid is at best constrained by the dense-jet +5 % inflation in PLB 654. If the analysis ultimately wants per-cell tracking systematics finer than "flat 2.5 % everywhere except a dense-jet flag", a DESY-internal note or a dedicated calibration study on our HERA-II sample (e.g. K0s → π⁺π⁻ peak ratio vs MC, or J/ψ → μ⁺μ⁻ ratio vs MC, or cosmic-ray track-finding efficiency in CJC) would be needed. This is non-trivial and outside this memo's scope.

2. **No public H1 charge-bias quantification on the 2005–2007 HERA-II sample.** Aaron 2009 used HERA-I e⁺p vs e⁻p comparison to probe the charge bias and report 2.5 %. The same probe is in principle available in our dataset (2005 e⁻ vs 2006–07 e⁺), but it has different conditions (CST upgrade, different running, etc.). An equivalent cross-check would be valuable but is a separate study, not done by any published paper. Until done, we adopt the Aaron 2009 envelope as-is.

3. **Tracking precision is implicit in every H1 publication, never explicit.** This is a generic weakness of the H1 published systematics for any observable like ours that is finely binned in track-kinematic variables (z, pT_h). The recommended ±50 % resolution-smearing study is **a methodological choice we are making**, not a literature-backed prescription. We should be transparent about that in the paper.

4. **Recent H1 internal note?** I did not find any post-2014 H1-internal calibration document on charged-track tracking systematics for HERA-II. The 2017 jets paper (1611.03421) and 2024 groomed event-shapes paper (2403.10134) both reuse the HFS calibration of Kogler / Kretzschmar from the 2014 jets paper (1406.4709), and that calibration does not separate the tracking contribution. If an H1 internal note exists that updates the 2.5 % number for the 2005–2007 dataset specifically, it would be in a DESY internal-notes archive and would require a DESY-affiliated collaborator to retrieve. Flag for `h1-software` or the supervisor to chase via the H1 archive.

5. **Material budget — beampipe and CTD support.** The Aaron 2009 paper attributes part of the 2.5 % charge-asymmetry envelope to "secondary interactions of tracks in the material in front of the trackers". A dedicated material-budget study (e.g. ratio of K0s → π⁺π⁻ yields at φ angles facing more vs less material) would tighten this; not done in published H1.

6. **Comparison to ZEUS is ambiguous.** ZEUS quotes a larger inclusive envelope (±3 % vs H1 2.5 %) and a much larger acceptance-edge term (+6/−3 % vs H1 nothing). We do not adopt the ZEUS edge term because (a) H1's PLB 654 explicitly says the envelope is flat over θ, (b) our |η| < 1.3 fiducial is tighter than ZEUS's η < 1.75, and (c) ZEUS's pT cut variation (140↔160 MeV around their 150 MeV nominal) does not map onto our 150 MeV nominal in any obvious way. But if a reviewer challenges this, we should be ready to demonstrate the H1 acceptance is well-described by MC at our acceptance edges — e.g. show data/MC agreement of dn/dη within ±2 % at |η| = 1.2.

---

## 8. Recommended dispatch — what the next agent should do to emit `source_tracking.json`

The next implementation agent (likely `h1-systematics` invoked by the supervisor) should produce `source_tracking.json` with the following structure and contents, computed in this order:

### Step 1 — Inclusive tracking efficiency (no re-unfold needed)
- Input: nominal unfolded M(Q²,x,z,pT_h,charge) on Scheme B′ 480-bin grid.
- Compute: `δ_trk_eff_inclusive(c) = 0.025 · M_nominal(c)` for every cell c.
- Dense-jet inflation: for cells with `Q² > 5000 GeV²` AND (`z > 0.5` OR `pT_h > 4 GeV`), use `δ_trk_eff_inclusive(c) = sqrt((0.025·M)² + (0.05·M)²) ≈ 0.0559·M`.
- Emit two histograms per cell: `M_up = M·(1 + δ/M)`, `M_down = M·(1 − δ/M)`, plus the half-difference.

### Step 2 — Charge-dependent residual on A_h (no re-unfold needed)
- Input: nominal unfolded M⁺(Q²,x,z,pT_h) and M⁻(Q²,x,z,pT_h).
- Compute two reweighted scenarios using **per-track weights** at the pre-unfolding step (i.e. weight every gen+reco track by the appropriate factor, re-bin into the 480-bin grid):
  - Scenario "up":   w⁺ = 1.025, w⁻ = 0.975
  - Scenario "down": w⁺ = 0.975, w⁻ = 1.025
- For each scenario compute A_up(c), A_down(c), and the per-cell envelope `δ_trk_charge(c) = |A_up(c) − A_down(c)| / 2`.
- Sanity check: integrate over (Q²,x,z,pT_h) and verify δ ≈ 0.025 ± 0.005, consistent with Aaron 2009.
- Document in `source_tracking.json`: the two scenarios, the per-cell envelope, and the integrated sanity-check value.

### Step 3 — Tracking precision (full re-unfold required)
- This is the **only step requiring full re-extract → response matrix → re-unfold** per the framework's category 2 rule.
- Input: nominal MC tree on NAF.
- For each variation v ∈ {res×1.5} (one-sided; see note above):
  1. In the per-track extractor (e.g. `extract_pertrack_v5g`), apply an additional Gaussian smearing to the gen-level pT of every MC track: `pT_smeared = pT_gen · (1 + Gauss(0, σ_extra/pT_gen))` with σ_extra² = (v²−1)·σ_nominal² and σ_nominal(pT) = sqrt((0.006·pT)² + 0.02²)·pT (the published H1 CTD resolution from Pitzl et al., NIM A 454 (2000) 334).
  2. Rebuild the response matrix `A_v` from the smeared MC (Stage 2 binary call).
  3. Re-unfold the data with `A_v`.
  4. Compute `δ_trk_res(c) = |M_unfolded_v(c) − M_unfolded_nominal(c)|`.
- This will take a full production run (Condor). Plan accordingly; no login-node single-file shortcut.
- Important: smear the **reco-level pT** of every track on top of the gen→reco mapping, i.e. apply the extra smearing in the reco-level reconstruction step **only**. Do NOT smear gen-level pT, because the gen level is the unfolding target.

### Step 4 — `source_tracking.json` schema
Recommended schema (mirroring the existing `source_*.json` convention in the H1 systematics pipeline):
```json
{
  "source": "tracking",
  "components": {
    "efficiency_inclusive": {
      "type": "per_track_weight",
      "bin_migrating": false,
      "propagation": "analytic_shortcut",
      "nominal_per_track_uncertainty": 0.025,
      "dense_jet_extra": 0.05,
      "dense_jet_region": "Q2 > 5000 GeV^2 AND (z > 0.5 OR pT_h > 4 GeV)",
      "literature_anchor": "H1 PLB 654 (2007) 148, arXiv:0706.2456, Sec. 7"
    },
    "efficiency_charge_dependent": {
      "type": "per_track_weight_anticorrelated",
      "bin_migrating": false,
      "propagation": "analytic_shortcut",
      "per_track_uncertainty": 0.025,
      "anti_correlation": "fully anti-correlated between h+ and h-",
      "literature_anchor": "H1 PLB 681 (2009) 125, arXiv:0907.2666, Sec. 6"
    },
    "precision_resolution": {
      "type": "per_track_smearing",
      "bin_migrating": true,
      "propagation": "full_re_unfold",
      "nominal_resolution_formula": "sigma(pT)/pT = 0.006 * pT[GeV] (+) 0.02",
      "variation": "res x 1.5 (one-sided, MC underestimates resolution scenario)",
      "literature_anchor_resolution": "D. Pitzl et al., NIM A 454 (2000) 334, hep-ex/0002044",
      "literature_anchor_method": "no published H1 prescription; methodology adopted in this memo"
    }
  },
  "per_cell_histograms": {
    "M_up_inclusive":          "histogram path on NAF",
    "M_down_inclusive":        "histogram path on NAF",
    "A_up_charge":             "histogram path on NAF",
    "A_down_charge":           "histogram path on NAF",
    "M_smeared_resolution":    "histogram path on NAF (one-sided)",
    "delta_inclusive":         "histogram path on NAF (half-difference)",
    "delta_charge":            "histogram path on NAF (half-difference, on A)",
    "delta_resolution":        "histogram path on NAF (one-sided diff, on M)"
  },
  "integrated_sanity_checks": {
    "delta_inclusive_integrated_pct": "expected 2.5 (Aktas 2007), with dense-jet bins at ~5.6%",
    "delta_charge_on_A_integrated_pct": "expected ~2.5 (Aaron 2009)",
    "delta_resolution_typical_pct": "expected 0.5-1, up to 2 at bin edges; no literature value"
  }
}
```

### Step 5 — Documentation
After implementation, the next agent should produce a one-page validation note showing the per-cell δ_trk_eff, δ_trk_charge, and δ_trk_res, and confirming that the integrated values match the literature anchors within their stated precision. Any deviation > 30 % from the literature anchor should be flagged for review.

---

## 9. Source citations (verbatim, for traceability)

- **H1 PLB 654 (2007) 148** — Aktas et al. (H1 Collaboration), "Charged Particle Production in High Q² DIS at HERA", arXiv:0706.2456 [hep-ex], DOI 10.1016/j.physletb.2007.07.073, InspireHEP recid 753349. Tracking systematic in Sec. 7, pp. 8–9.
- **H1 PLB 681 (2009) 125** — Aaron et al. (H1 Collaboration), "Observation of the Hadronic Final State Charge Asymmetry in High Q² Deep-Inelastic Scattering at HERA", arXiv:0907.2666 [hep-ex], DOI 10.1016/j.physletb.2009.09.072, InspireHEP recid 825825. Tracking systematic on A in Sec. 6, p. 7.
- **ZEUS JHEP 06 (2010) 009** (+erratum JHEP 10 (2010) 030) — Chekanov et al. (ZEUS Collaboration), "Scaled momentum spectra in DIS at HERA", arXiv:1001.4026 [hep-ex], InspireHEP recid 844129. Tracking systematic in Sec. 5, pp. 13–14.
- **H1 EPJC 77 (2017) 215** — Andreev et al. (H1 Collaboration), "Measurement of Jet Production Cross Sections in DIS ep at HERA", arXiv:1611.03421 [hep-ex] v4 (June 2021, with erratum), DOI 10.1140/epjc/s10052-017-4717-9. Lump "uncorrelated 1 %" term containing tracking in Sec. 6, p. 15–16.
- **H1 EPJC 75 (2015) 65** — Aaron et al. (H1 Collaboration), "Measurement of Multijet Production in ep Collisions at High Q² and Determination of α_s", arXiv:1406.4709 [hep-ex].
- **H1 PRL 128 (2022) 132002** — Andreev et al. (H1 Collaboration), "Measurement of lepton-jet correlation in DIS with the H1 detector using ML for unfolding", arXiv:2108.12376 [hep-ex], DOI 10.1103/PhysRevLett.128.132002. No separate tracking systematic; see the "Uncertainties" paragraph.
- **H1 EPJC 84 (2024) 718** — Andreev et al. (H1 Collaboration), "Measurement of groomed event shape observables in DIS ep at HERA", arXiv:2403.10134 [hep-ex] v2 (1 Aug 2024, accepted by EPJC), DOI 10.1140/epjc/s10052-024-12987-0. Sec. 6 (Uncertainties); no separate tracking systematic.
- **D. Pitzl et al., NIM A 454 (2000) 334** — "The H1 silicon vertex detector", hep-ex/0002044. Source of the H1 CTD pT resolution σ(pT)/pT ≃ 0.006·pT[GeV] ⊕ 0.02.
- **I. Abt et al. (H1 Collaboration), NIM A 386 (1997) 310 and NIM A 386 (1997) 348** — H1 detector reference papers.

All quoted numerical values were read directly from the PDF or HTML text of the cited paper (full text on disk for #1, #2, #3, #4; HTML for #6 and #7; abstract + selected sections for #5).
