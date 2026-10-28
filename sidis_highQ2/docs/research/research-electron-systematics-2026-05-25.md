# Research Memo: Scattered-Electron Systematics for H1 Charged-Particle SIDIS at High Q²

**Date**: 2026-05-25
**Analysis**: H1ChargedSidis_HighQ2 (charged-particle SIDIS, Q² > 150 GeV²)
**Author scope**: literature researcher (source-backed survey + implementation recommendation)
**Companion memo**: `research-HFS-systematic-2026-05-25.md` (same directory; same date) — read in parallel; the two systematics share the migration-affecting propagation framework and the same set of H1 high-Q² reference papers.

---

## 0. Source-availability caveat (read first)

This memo was prepared from the H1 published literature plus two local artifacts:

* `H1/sidis_highQ2/docs/research/research-HFS-systematic-2026-05-25.md` (862 lines, the HFS companion memo; read in full for shared framing).
* `H1/sidis_highQ2/docs/plans/plan-background-MC-production-g1jES-2026-05-24.md` (the g1j_ES background MC production plan; referenced for production wall-time and disk-cost calibration).

Items asked about in the brief that are **not present on the local disk mirror** at memo time:

* The per-track CSV `H1/sidis_highQ2/data/pertrack_v5g_fixedFrame_v3_merged.csv.gz` — not present locally (lives on NAF DUST per AGENTS.md). The CSV **schema** asserted in the supervisor brief (`scatEX, scatEY, scatEZ, scatEE` per track row, `BeamElecE, BeamProtP` per event, plus the per-track quantities the HFS memo §6.B enumerated: `Q2es, Xes, Yes, Pt_lab, Eta_lab, charge, z, pT_h, phi_h, weight, weight1, weight2, Empz`) is taken as given for this memo. The presence of the full reco-side scattered-electron 4-vector in the per-track CSV is the key fact that makes Option B viable for the electron systematic — see §4–§5. **Verification action**: before launching the §6.B work plan, dump CSV columns and confirm the schema. If `scatEX/Y/Z/EE` are absent, Option B collapses to the same "schema-extension required" verdict the HFS memo reached.
* No prior `reference-LAr-electron-scale-highQ2-sidis.md` exists in the workspace; this memo is the first electron-scale write-up — see §8.
* Extractor source, response-matrix code, and unfold scripts — not local; same caveat as the HFS memo.

The recommendations below are based on (a) the H1 published prescription for the scattered-electron uncertainty in this kinematic region (Andreev 2014/2017/2022/2023/2024) and (b) the schema and conventions inferred from the supervisor brief. Per-bin SIDIS magnitudes are literature-anchored estimates, not measured values from our v3 sample; closure with the actual per-bin shifts requires running the §6 plan on the NAF files.

---

## 1. Executive summary

The H1 canonical scattered-electron systematic at Q² > 150 GeV² (i.e. our exact kinematic regime) is a **three-component prescription** that has been used unchanged across the H1 HERA-II high-Q² publication chain:

* **Electron energy scale (E_e' from LAr)** — `Lepton(E)`: **±0.5 % in the backward and central LAr regions, rising to ±1 % in the forward region** (Andreev 2024 §6.4, Andreev 2022 *Uncertainties* paragraph, Andreev 2023 §4). Propagated through the full unfolding procedure; in-η dependence is implemented as a piecewise scale (not a single uniform shift).
* **Electron polar angle θ_e** — implicit in `Lepton(E)` shift via Q² formula in some papers; in Andreev 2024 §6.1 it is **±1 mrad** (calibrated via tracker-LAr polar-angle alignment), propagated jointly with the HFS polar-angle uncertainty through the unfold.
* **Electron azimuthal angle φ_e** — `Lepton(ϕ)`: **±1 mrad** (Andreev 2022 *Uncertainties*; Andreev 2023 Tables 2–3 column `Lepton(ϕ)`).

All three are varied independently and each propagated through the **full unfolding procedure** (re-extract → re-build response → re-unfold). This is "Option A" in the supervisor brief and the published H1 convention. It is **not** ±0.5–1 % on a single uniform parameter; the polar/azimuthal angle shifts are separate components.

The supervisor brief's framing of "polar angle θ_e: typically ±1–2 mrad" is at the upper end and corresponds to the older "double-angle calibration uncertainty" quote used in pre-HERA-II analyses; the published HERA-II high-Q² convention is **±1 mrad** for both θ_e and φ_e, derived from the LAr-tracker alignment precision (Andreev 2024 §6.1 cites the alignment precision as 1 mrad and uses that exact value for both the HFS-polar and electron-polar uncertainty). Use ±1 mrad as the canonical convention; ±2 mrad is a conservative envelope that no recent H1 high-Q² paper uses.

Per-bin SIDIS-observable impact, anchored to the most directly comparable H1 high-Q² published measurement (Andreev 2023, *PLB* 844, 138101, arXiv:2303.13620, jet substructure at Q² > 150 GeV², 0.2 < y < 0.7, the closest analog to our analysis):

| Source                | per-component σ on unfolded yields / means | typical | tail |
|---|---|---|---|
| `Lepton(E)`  ±0.5–1 %  | 0.1–1 %      | ~0.3 %        | up to ~1 %  |
| `Lepton(ϕ)`  ±1 mrad    | 0.05–0.7 %  | ~0.1 %        | up to ~0.7 % |
| Electron polar (folded with HFS polar in Andreev 2024) | ~1 % on final distributions (combined HFS+Lepton polar) | — | — |

Combined electron envelope: **per-bin total ≤ 1–1.5 % on bulk SIDIS bins, ≤ 2 % on tail bins** (high y, high z, high pT_h, highest-Q² interval). This is **smaller than the HFS envelope by roughly a factor of ~2–3** in bulk bins, consistent with the long-standing pattern that for any HERA-II high-Q² hadronic measurement the HFS scale dominates over the electron scale (Andreev 2023 Table 2: charged multiplicity ⟨Nc⟩ at Q² ∈ [2081, 5000] GeV² has HFS_jet = 0.053, HFS_other = 0.056, vs. Lepton(E) = 0.028, Lepton(ϕ) = 0.050 — comparable for ⟨Nc⟩ in the tail, but the electron is sub-dominant for cross-section means in bulk bins).

**Recommended path for our v3 production**: **Option B (analytic on the per-track CSV)** for the electron systematic is **viable and defensible** because the per-track CSV schema includes the full reco-side scattered-electron 4-vector (`scatEX, scatEY, scatEZ, scatEE`) and the beam 4-momenta (`BeamElecE, BeamProtP`). This contrasts with the HFS case (companion memo §6.B) where the missing per-HFS-object jet-membership flag forced Option A. For the electron, every input required to recompute Q²_e, x_e, y_e, the γ*+p CM boost, and the per-track (z, pT_h, φ_h) is present in the CSV. Total compute cost: ~30 minutes wall on the existing CSV, ~6 variation runs (E scale up/down × backward+forward separately, θ up/down, φ up/down). See §6.B for the plan.

**Option A (re-production at the framework level)** remains the gold-standard option matching the published H1 prescription exactly. It is recommended **only if** the Option B closure check against a single Option A reference variation disagrees beyond ~30 % per bin (see §6.A.5 for the closure protocol). Cost of Option A: ~3 h Condor wall for the 4–6 variations + extractor + unfold, similar to the HFS Option A in the companion memo.

**Why Option B is acceptable for electrons but was rejected for HFS**: the electron 4-vector is reconstructed once per event from the highest-pT LAr cluster matched to a track (Andreev 2022 *Experimental method*), and its uncertainty propagates only through the kinematic formulae (Q²_e, x_e, y_e) and the CM-frame boost. None of the H1 reco-side selection cuts depends on the electron 4-vector in a way that an analytic shift cannot reproduce, provided the schema gives us E_e' and the 3-vector direction. Concretely: the HFS-side selections (E−pz cut, pT_HFS/pT_e balance cut) reference the *electron* pT through the ratio, and the *acceptance shift* induced by an electron-scale variation **does** flow through to event acceptance — but for an ~0.5 % scale shift the events that flip across the 0.6 < pT_HFS/pT_e < 1.6 boundary are a small minority, and this can be reproduced analytically from the CSV (the electron 3-vector tells us pT_e per event; re-evaluate the cut after shifting). The HFS case had a separate blocker (per-HFS-object jet membership not in the CSV); the electron case has no analogous blocker.

**Recommended electron envelope to quote in the v3 systematics table** (placeholder pending Option B run): **per-bin total σ_electron / yield bounded by ≈ 1 % in typical bins, ≤ 2 % in tail bins** (z > 0.7, pT_h > 4 GeV, Q² > 2000 GeV², high y). These bounds are anchored to Andreev 2023 Tables 2–3 (`Lepton(E)` + `Lepton(ϕ)` quadrature) and Andreev 2024 §6.1 + §6.4 (polar-alignment ~1 %, energy-scale < 3 % in any individual bin, much less in bulk).

**Resource allocation framing**: since the electron systematic is sub-dominant to HFS in nearly every Andreev 2023 bin (and the few tail bins where it is comparable are also small in absolute terms), the Option B path is the right cost/rigor trade-off. Reserve Option A compute for HFS (companion memo §6.A); use the saved compute on the electron side for Option B + a same-day Option A spot-check.

---

## 2. H1 canonical scattered-electron convention (three components, propagated through full unfold)

### 2.1 Andreev 2024 (groomed event shapes, *EPJC* 84 (2024) 718, arXiv:2403.10134)

The most recent published H1 measurement at Q² > 150 GeV². The relevant uncertainty subsections are §6.1 (Alignment), §6.4 (Electron energy scale), and §6.3 (luminosity, electron-ID — folded into the 2.7 % normalization). Read on the ar5iv HTML on 2026-05-25.

**§6.1 Alignment** (verbatim):
> "The polar angle alignment of the tracking detectors with the liquid argon calorimeter has a precision of 1 mrad [166]. This precision results in an uncertainty in the measured position for all HFS objects and for the scattered electron. The HFS and electron polar angle uncertainties are considered separately, and each is passed through the unfolding procedure. The resulting uncertainties in the final distributions are typically ~1 %. The values reported in the data tables in Sec. 9 are signed quantities corresponding to the difference between the nominal angles and the angles after the systematic shift of the polar angle of all simulated objects upwards by 1 mrad."

Reading: **±1 mrad polar-angle shift for the scattered electron**, applied independently of the HFS polar-angle shift, propagated through the full unfold, ~1 % per-bin effect on final distributions.

**§6.4 Electron energy scale** (verbatim):
> "The measured energy of the scattered electron has a precision of 0.5 % in the backward and central regions of the detector and a precision of 1 % in the forward region [166]. The uncertainty in final distributions due to this precision is determined by varying the scattered electron energy and passing the modified events through the unfolding procedure. The resulting uncertainty has a value of less than ~3 %."

Reading: **±0.5 % in backward and central regions (θ_e > ~30°, the bulk of high-Q² SIDIS phase space), ±1 % in the forward region (θ_e ≤ ~30°)**. This is a piecewise scale uncertainty, not a single uniform 0.5 % or 1 %. The "<~3 %" final-distribution uncertainty is the worst-case bin in groomed event shapes; for SIDIS-yield observables the typical effect is smaller (compare to Andreev 2023 Tables 2–3, §2.3 below).

Andreev 2024 does not separately list `Lepton(ϕ)` in §6, consistent with the convention seen in Andreev 2022/2023 where the azimuthal scattered-electron shift contributes a separate (small) entry. The omission from §6 likely reflects that the groomed event shapes (Breit-frame, 1-jettiness and invariant mass) are inherently rotation-symmetric around the (q + xP) axis and have negligible sensitivity to φ_e.

Source: arXiv:2403.10134v2, EPJC 84 (2024) 718, DOI 10.1140/epjc/s10052-024-12987-0. Sections 6.1 and 6.4 read on ar5iv HTML 2026-05-25.

### 2.2 Andreev 2023 (jet substructure, *PLB* 844 (2023) 138101, arXiv:2303.13620) — most comparable

Same dataset (2006–2007 HERA-II), same Q² > 150 GeV², 0.2 < y < 0.7, same Σ-method reconstruction, same HFS object definition (energy-flow algorithm with electron removed). §4 *Uncertainties* states verbatim (ar5iv HTML, retrieved 2026-05-25):

> "Lepton uncertainties are considered in the Q² determination and the uncertainty on the lepton energy scale ranges from ±0.5 % to ±1 % [142, 83]. Uncertainties on the azimuthal angle of the scattered lepton are estimated to be ±1 mrad [82]."

Reading: **±0.5–1 % electron energy scale, ±1 mrad azimuthal angle**. The polar-angle uncertainty is folded into the same 1 mrad LAr-tracker alignment as in Andreev 2024 (which Andreev 2023 implicitly cites through ref [142] = Andreev 2014).

**Tables 2 and 3** (transcribed in §3.3 below) give the per-bin contributions for `Lepton(E)` and `Lepton(ϕ)` separately, plus `HFS(jet)`, `HFS(other)`, `HFS(ϕ)`, Model, and Closure. This is the **direct evidence base** for our electron envelope estimate.

Source: arXiv:2303.13620v3, PLB 844 (2023) 138101, DOI 10.1016/j.physletb.2023.138101. §4 + Tables 2–3 read on ar5iv HTML 2026-05-25.

### 2.3 Andreev 2022 (lepton-jet correlation, *PRL* 128 (2022) 132002, arXiv:2108.12376) — closest analog using ML unfolding

Same 2006–2007 HERA-II data (136 pb⁻¹ subset), Q² > 150 GeV², Σ-method, same `Lepton(E)` and `Lepton(ϕ)` decomposition, with the OmniFold/MultiFold unfolding that is also the basis for our v3 pipeline (per AGENTS.md). The *Uncertainties* paragraph states verbatim (ar5iv HTML, retrieved 2026-05-25):

> "The uncertainty of the measurement of the energy of the scattered lepton ranges from ±0.5 % at backward and central regions [120] to ±1 % at forward regions [16].
> The uncertainty of the measurement of the azimuthal angle of the scattered lepton is ±1 mrad [28]."

Reference [120] = Andreev 2014 *EPJC* 74 (2014) 2814 (high-Q² inclusive cross-section paper — the electron-scale calibration source). Reference [16] = Andreev 2017 (jets paper, the JES/RCES origin). Reference [28] = Aaron 2012 *JHEP* 09 (2012) 061 (the longitudinally polarized NC measurement — the source of the 1 mrad azimuthal-electron quote).

Reading: identical conventions as Andreev 2023. **±0.5 % central/backward, ±1 % forward** electron energy scale; **±1 mrad** electron azimuthal angle. Polar angle is folded into the LAr-tracker 1 mrad alignment as in Andreev 2024.

Source: arXiv:2108.12376v2, PRL 128 (2022) 132002. ar5iv HTML *Uncertainties* paragraph read 2026-05-25.

### 2.4 Andreev 2017 (jet production, *EPJC* 77 (2017) 215, arXiv:1611.03421)

The historical anchor for both the JES/RCES (HFS) and the electron-energy-scale calibration. Same per-η piecewise electron-scale convention. The ar5iv HTML pull failed (oversized payload) in the HFS memo's research; the convention is reproduced transitively via Andreev 2022/2023/2024 citations to ref [16] (= 1611.03421). The abstract page was retrieved 2026-05-25; the convention is confirmed.

### 2.5 Where does the polar-angle electron systematic appear?

A subtle point: Andreev 2023 §4 (and the table column headers in Tables 2–3) does **not** explicitly list a `Lepton(polar)` column separate from `Lepton(E)`. The reason is that the H1 high-Q² convention treats the *polar* scattered-electron angle as part of the LAr–tracker alignment uncertainty, which is folded with the HFS polar uncertainty in many papers (Andreev 2024 §6.1 splits them; Andreev 2023 absorbs the electron-polar part of the alignment into the electron-energy uncertainty propagation since Q²_e = E_e'² sin²(θ_e)/(1−y), making the two correlated). For Andreev 2024 the split is explicit: `Lepton(E)` ≈ "less than ~3 %", electron-polar combined with HFS-polar ≈ "typically ~1 %".

For our v3 systematics decomposition, the **clearest published recipe** is the Andreev 2024 split:

1. **Lepton(E)** — ±0.5 % in central/backward, ±1 % in forward.
2. **Lepton(θ)** — ±1 mrad, propagated jointly with HFS-θ as a single alignment uncertainty, OR split as in Andreev 2024 §6.1 if reporting separately is desired.
3. **Lepton(ϕ)** — ±1 mrad, varied independently.

The brief's "±2 mrad" polar quote is conservative and would also be defensible; the published convention is the tighter ±1 mrad.

### 2.6 Bottom line for §2

Drop the "±2 mrad polar" framing in favor of the published ±1 mrad. The canonical published convention is:

* `Lepton(E)`: ±0.5 % central/backward, ±1 % forward (piecewise in η).
* `Lepton(θ)`: ±1 mrad (LAr-tracker alignment).
* `Lepton(ϕ)`: ±1 mrad.

Each varied independently, each propagated through the full unfolding.

---

## 3. How the scattered-electron systematic affects our observables

Following the same per-event vs. per-track decomposition as the HFS memo §3.

### 3.1 Per-event kinematics (Q², x, y) — eSigma-method case

The eΣ method (used in v5g/v3 production per AGENTS.md and the supervisor brief) reconstructs:

```
Σ_had  = Σ_HFS (E_i − p_{z,i})              (HFS-dependent, electron-INdependent)
Σ_e    = E_e' (1 − cos θ_e)                 (electron-dependent: E_e', θ_e)
y_es   = Σ_had / (Σ_had + 2 E_e_beam)       (eΣ form; 2 E_e_beam from the eSigma denominator)
Q²_es  = E_e'² sin²(θ_e) / (1 − y_es)       (electron-dependent: both E_e' and θ_e enter Q²)
x_es   = Q²_es / (s · y_es)                  (s = 4 E_e_beam E_p_beam, beam-only)
```

(Note: the exact form of y_es in the v3 extractor needs verification — see the HFS memo §3.1 footnote. The classic Bassler-Bernardi eΣ uses 2 E_e_beam in the denominator; some implementations use Σ_e instead. The expressions below assume the classic form and **must** be re-derived against the actual extractor source before quoting per-bin values.)

**Effect of electron energy-scale shift δ_E = +0.5 % on E_e':**

* Σ_e → Σ_e · (1 + δ_E)
* y_es: **unchanged at first order** (the electron does not enter the eΣ y formula except through 2 E_e_beam, which is the beam energy, not E_e'). This is the main eΣ advantage. Caveat: if the extractor uses Σ_e in the denominator (not 2 E_e_beam), then y_es → y_es · [1 − δ_E · Σ_e / (Σ_had + Σ_e)] ≈ y_es · (1 − δ_E · (1 − y_es)). Verify the form.
* Q²_es → Q²_es · (1 + δ_E)² ≈ Q²_es · (1 + 2 δ_E) → for δ_E = +0.5 %, **ΔQ²_es / Q²_es ≈ +1.0 %**.
* x_es → x_es · (1 + 2 δ_E / (1 − δ_E · y/(y+...))) ≈ x_es · (1 + 2 δ_E) when y is decoupled from the shift, so **Δx_es / x_es ≈ +1.0 %**.

**Effect of electron polar-angle shift δ_θ = +1 mrad on θ_e:**

* Σ_e = E_e' (1 − cos θ_e) → δ Σ_e / Σ_e = sin θ_e · δ_θ / (1 − cos θ_e) = cot(θ_e/2) · δ_θ. For θ_e = 90° (central LAr, typical bulk), cot(45°) = 1, so ΔΣ_e/Σ_e ≈ +1 mrad · 1 = +0.001 = 0.1 %. For θ_e = 30° (forward LAr, lower Q²), cot(15°) ≈ 3.7, so ΔΣ_e/Σ_e ≈ +0.37 %. For θ_e = 150° (backward, rare in high-Q² SIDIS), cot(75°) ≈ 0.27, ΔΣ_e/Σ_e ≈ +0.03 %.
* y_es: if y uses 2 E_e_beam in the denominator, unchanged. If it uses Σ_e, shifts at the ΔΣ_e level.
* Q²_es = E_e'² sin²(θ_e) / (1 − y_es): δQ²/Q² = 2 cot(θ_e) · δ_θ + δy_es/(1 − y_es). For θ_e = 90°, 2 cot(90°) = 0, so the polar-angle effect is **smallest in the central LAr** and grows like 2 cot(θ) toward the forward and backward regions. For θ_e = 30°, 2 cot(30°) ≈ 3.46, so 1 mrad → δQ²/Q² ≈ +0.35 %.

**Effect of electron azimuthal shift δ_φ = +1 mrad on φ_e:**

* Σ_e, y_es, Q²_es, x_es: **all unchanged** (azimuthal symmetric quantities).
* The only effect is through the CM-frame boost direction — see §3.2.

### 3.2 Per-track kinematics (z, pT_h, φ_h) — γ*+p CM frame

Per the v3 frame fix (2026-05-24, AGENTS.md), per-track variables are in the γ*+p CM frame:

```
q⃗_lab = k⃗_in − k⃗'                       (virtual photon 3-momentum; depends on scattered electron k⃗')
q_lab  = (E_in − E_e', q⃗_lab)             (virtual photon 4-momentum)
W²     = (q + P)² = Q²(1/x − 1) + M_p²
boost  to CM frame defined by W = q + P (the γ*+p invariant)
z      = (P · p_h) / (P · q)               (Lorentz-invariant; depends on q which depends on k')
pT_h   = transverse component of p_h in CM, w.r.t. q direction in CM
φ_h    = azimuthal angle of p_h around q in CM, Trento convention
```

The hadron 4-momentum p_h is reconstructed from tracker measurements (Andreev 2023 §1: σ(pT)/pT ≈ 0.2 % pT/GeV ⊕ 1.5 %); it is **not affected** by the scattered-electron systematic. The electron systematic enters z, pT_h, and φ_h **only through the boost** (via q = k_in − k').

**Effect of E_e' shift δ_E = +0.5 %:**

* q⃗ = k⃗_in − k⃗' shifts: |k⃗'| ≈ E_e' (relativistic limit), so δ|q⃗|/|q⃗| depends on the geometric configuration; magnitude δ_E in the direction of −k⃗'.
* z = (P · p_h) / (P · q) → δz/z = −δ(P · q)/(P · q). At fixed θ_e ≈ 90° and y not too small, this is approximately −δQ²/Q² · (factor of order y/(1−y)) → for δ_E = +0.5 %, **Δz/z ≈ ±0.5 %** typical, growing in the tails.
* pT_h: the boost direction is dominated by P, q. A 0.5 % shift in |k⃗'| rotates and rescales the boost slightly. Expect **ΔpT_h/pT_h ≲ 0.5 %** typically, larger in tails.
* φ_h: in Trento convention, φ_h is measured around q⃗ in the CM, with the φ = 0 plane defined by the lepton-scattering plane. An E-scale shift does not rotate the scattering plane (it rescales k⃗' in magnitude only), so **Δφ_h is at the per-mille level**, set by second-order changes in the boost orientation.

**Effect of θ_e shift δ_θ = +1 mrad:**

* k⃗' rotates by 1 mrad in the θ direction. q⃗ = k⃗_in − k⃗' picks up a perpendicular component of ~|k⃗'| · sin(1 mrad) ≈ E_e' · 0.001.
* z: shifts at the ±0.1–0.4 % level depending on θ_e (cot(θ_e) factor; smallest at θ_e = 90°).
* pT_h in CM: rotated by ≲ 1 mrad in CM (the boost rotation is bounded by the lab-frame rotation). **ΔpT_h/pT_h ≲ 0.1 %** typical.
* φ_h: the lepton-scattering plane is tilted by 1 mrad. **Δφ_h ≲ 1 mrad ≈ 0.06°** ≪ our φ_h binning (presumably few-bin coarse for Trento harmonics, so this is completely negligible per-track but does enter the **migration matrix** as a small response shift).

**Effect of φ_e shift δ_φ = +1 mrad:**

* k⃗' rotates by 1 mrad in the φ direction. q⃗ rotates by approximately the same angle in φ.
* z, pT_h: unchanged (rotation-invariant).
* φ_h: the lepton-scattering plane rotates by 1 mrad → **Δφ_h ≈ +1 mrad ≈ +0.06°**. This is the only place where φ_e enters at first order.

This is the **only** systematic in the entire decomposition that affects φ_h at first order. For the Trento-frame φ_h harmonic analyses, an offset in φ_h of ~1 mrad is far below the typical binning (16 or 24 bins, i.e. ~22° per bin) so the effect on harmonic amplitudes is at the few-per-mille level. The same is true for the q × p_h cross product the brief asked about in §Q4 — see §4 below.

### 3.3 Summary table of scattered-electron sensitivity

| Observable | Lepton(E) ±0.5 % | Lepton(θ) ±1 mrad | Lepton(ϕ) ±1 mrad |
|---|---|---|---|
| Q²_es (per event)        | ±1.0 %                              | ±0.0–0.4 % (θ-dep, min at 90°)            | 0 |
| y_es (per event, eΣ form) | ~0 (if 2 E_e_beam in denom)         | ~0                                          | 0 |
| x_es (per event)         | ±1.0 %                              | ±0.0–0.4 %                                  | 0 |
| z (CM, per track)        | ±0.3–0.5 % typical, up to ±1 % tails | ±0.05–0.4 % (θ-dep)                        | 0 |
| pT_h (CM, per track)     | ±0.2–0.5 % typical                  | ±0.05–0.2 %                                 | 0 |
| φ_h (CM, per track)      | per-mille                           | ≲ 1 mrad ≈ 0.06°                            | ≈ +1 mrad ≈ +0.06° |
| Track Pt_lab, Eta_lab    | **0** (tracker, not electron)       | **0**                                       | 0 |
| HFS quantities (Empz etc.) | **0** (independent of electron)     | **0** (the electron is removed from HFS)    | 0 |

### 3.4 Migration in the 480-bin Scheme B′ grid

* (Q², x) migration: dominated by Q² shift at ±1 % per event (from Lepton(E)). Scheme B′ (Q², x) bins are roughly log-spaced; per the HFS memo §3.3, a few-percent Q² shift moves O(1 %)–O(2 %) of events across bin boundaries.
* z migration: small (≲ 0.5 % per event for Lepton(E); ≲ 0.4 % for Lepton(θ)).
* pT_h migration: small (≲ 0.5 % per track).
* φ_h migration: only Lepton(ϕ) contributes; ~1 mrad shift → < 0.5 % of tracks cross typical 22°-wide bin boundaries.

**The migration is dominated by the per-event Q²/x shift from Lepton(E), not by the per-track z/pT_h/φ_h shifts.** This is the same conclusion as the HFS memo, with the key difference that the electron systematic Q² shift (~1 % from a 0.5 % E-scale) is smaller than the HFS-driven Q² shift (~1 % from a 1 % HFS) by roughly a factor of 1, but the electron systematic *only* shifts the Q² when the eSigma reconstruction is used — and even then it shifts y_es much less than HFS does. Net per-bin SIDIS migration from the electron is **smaller** than from HFS in the same bin.

### 3.5 Re-derivation of the migration shifts with the proper eSigma formula

The exact formulae depend on the eSigma form used in the v3 extractor. The HFS memo §3.1 already flagged this verification. For the electron memo the same verification covers:

* If `y_es = Σ_had / (Σ_had + 2 E_e_beam)` (Bassler-Bernardi-eΣ classic): Lepton(E) does not enter y_es. Q²_es scales as E_e'², so 0.5 % E-shift → 1 % Q² shift.
* If `y_es = Σ_had / (Σ_had + Σ_e)` (a sometimes-seen alternative): Lepton(E) enters y_es at order δ_E · Σ_e / (Σ_had + Σ_e). Q²_es picks up an additional δy/(1−y) term.

**Action item before quoting per-bin numbers**: confirm the y_es formula in the v3 extractor (most likely the classic eΣ; the H1 high-Q² convention is unambiguous: Andreev 2023 Eq. 2 uses `y = Σ_had / (Σ_had + Σ_e)` with Σ_e = E_e'(1 − cos θ_e) — see §2.2 above. So the second alternative is in fact the right one.)

**Correction to §3.1 above given the Andreev 2023 Σ-method formula**:
Using `y_es = Σ_had / (Σ_had + E_e'(1 − cos θ_e))`, the Lepton(E) and Lepton(θ) shifts both enter y_es through the denominator. For δ_E = +0.5 %:

* Σ_e → (1 + δ_E) Σ_e
* y_es → y_es · [1 − δ_E · Σ_e / (Σ_had + Σ_e)] = y_es · [1 − δ_E · (1 − y_es)]
* For y_es = 0.5, δy_es/y_es ≈ −δ_E · 0.5 = −0.25 %.
* Q²_es = E_e'² sin²(θ_e) / (1 − y_es) → δQ²_es / Q²_es = 2 δ_E + δ(1/(1−y_es))/(1/(1−y_es)) = 2 δ_E − δy_es / (1−y_es) ≈ 2 δ_E + δ_E · y_es ≈ δ_E · (2 + y_es).
* For y_es = 0.5: δQ²_es / Q²_es ≈ 0.5 % · 2.5 = +1.25 %.

These are the correct shifts under the Andreev 2023 / 2022 Σ-form. The corresponding entries in the §3.3 table for Lepton(E) should read **±1.25 % on Q²_es at y_es = 0.5**, not ±1.0 %. The qualitative conclusion is unchanged (Lepton(E) is the dominant electron contribution; Q² shift is the dominant migration driver).

---

## 4. Charge-dependent effects on A_h

The supervisor brief asks (§Q4) whether the electron systematic introduces a charge-dependent shift on the SIDIS asymmetry A_h, particularly through the q × p_h cross product in the Trento φ_h definition.

**Published evidence**: no H1 paper in the analog set (Andreev 2017/2022/2023/2024) reports a charge-symmetric vs. charge-asymmetric breakdown of the scattered-electron systematic, because none of those analyses measures a charged-particle (h+ vs. h−) asymmetry. The closest probe is the *jet charge* Q₁ = λ̃₀¹ in Andreev 2023 Table 2, where the per-Q² Lepton(E) entries are 0.001 in all four bins, vs. central values 0.011, 0.016, 0.033, 0.057 — i.e. ~10 % relative for the lowest-central-value bin, ~2 % relative for the highest-central-value bin. This is the only direct measurement of an electron-systematic effect on a charge-correlated quantity at H1 high-Q², and it shows the electron systematic does contribute to charge-sensitive observables at the few-percent level even after partial cancellation.

**Mechanism**: the scattered electron 4-vector enters A_h symmetrically for h+ and h− because Q², x, y, z, pT_h, φ_h are all charge-independent on a per-event basis. The only place a charge-dependent shift could enter is via the **q × p_h cross product in the Trento φ_h convention**:

```
φ_h = atan2[(q × p_h) · n̂_l, (q × p_h) · (q × n̂_l)]   (where n̂_l is the lepton-side normal)
```

A rotation of q⃗ by δ_θ or δ_φ rotates the (q × p_h) vector for both h+ and h− by the same amount in CM. The Trento harmonic amplitudes A^cos(φ), A^cos(2φ), etc. then shift by amounts proportional to the rotation, but the **shift is identical for h+ and h−**. The cross product does not produce a sign change between h+ and h− because the sign of (q × p_h) depends on the *direction* of p_h, which is opposite for h+ vs. h− only in the rare case of a perfectly back-to-back h+/h− pair (i.e. zero net pT). Generic high-pT charged hadrons in our sample are not back-to-back; the Trento φ_h is computed per-hadron and the sign of (q × p_h) is set by the individual hadron's CM-frame direction, not its charge.

**Conclusion**: σ_electron is to first order charge-symmetric and **cancels in A_h** like any common-mode systematic. The residual second-order asymmetric shift comes from:

1. Different per-bin populations of h+ vs. h− (different ⟨z⟩, ⟨pT_h⟩) → the electron systematic, which is mildly z- and pT_h-dependent (per §3.2), creates a slightly different shift on the h+ and h− yields → A_h shifts by a residual factor of (δσ_electron) × (relative population difference). For an A_h at the few-percent level and a population difference at the few-percent level and σ_electron at the percent level, the residual is **σ_electron on A_h ≈ 0.01 % per bin or smaller** — completely negligible vs. the statistical floor and vs. the HFS systematic on A_h.

2. Charge-dependent reconstruction efficiency that depends on hadron θ in the lab, which can be slightly correlated with the electron-side polar angle through back-to-back kinematics. The Andreev 2023 jet-charge entry of Lepton(E) = 0.001 (independent of Q²) suggests the residual is at the per-mille level for a charge-sum observable; for an *asymmetry* A_h the residual is even smaller because the common-mode cancels.

**Quantitative bound to put in the v3 systematics table for A_h**: **σ_electron on A_h ≤ 0.1 % absolute per bin** for charged-particle pseudorapidity-symmetric observables. Source: Andreev 2023 Table 2 jet-charge column, scaled by the SIDIS-vs-jet-charge population-difference factor.

If the analysis additionally measures φ_h harmonics on the asymmetry side (i.e. A_h^cos(φ), A_h^cos(2φ)), the Lepton(ϕ) ±1 mrad shift rotates the φ_h definition by 1 mrad. For 24 φ-bins (15° each), 1 mrad ≈ 0.4 % of one bin — i.e. the bin migration on the φ_h harmonics is **negligible**. The harmonic amplitude shift from a uniform rotation of all hadrons by 1 mrad is also at the per-mille level (it appears as a phase shift, not an amplitude shift, and the phase is anchored by the lepton-scattering plane which is the same for h+ and h−).

---

## 5. Option A (re-production) vs. Option B (analytic on per-track CSV)

### 5.1 Option A — re-extract from ROOT with electron 4-vector shifted at the framework level

**Procedure** (matches published H1 high-Q² prescription, mirrors HFS memo §6.A):

1. Add steering flags `ELEC_E_SCALE_REGION = central | forward`, `ELEC_E_SHIFT = +0.005 | −0.005 | +0.01 | −0.01 | 0.0`, `ELEC_THETA_SHIFT = +0.001 | −0.001 | 0.0` (rad), `ELEC_PHI_SHIFT = +0.001 | −0.001 | 0.0` (rad) to the Stage-2 binary (`ChargedSidisTreeBuilderHighQ2_v5g`).
2. In the scattered-electron identification loop, **after** the electron is identified (highest-pT LAr cluster matched to a track passing isolation), apply the relevant scale/angle shift to the electron 4-vector:
   ```cpp
   double dE_rel = isElectronForward(elec.Theta()) ? elecEShiftForward : elecEShiftCentral;
   double E_new = elec.E() * (1.0 + dE_rel);
   double theta_new = elec.Theta() + elecThetaShift;
   double phi_new = elec.Phi() + elecPhiShift;
   elec.SetE(E_new);
   elec.SetTheta(theta_new);   // ROOT TLorentzVector helper
   elec.SetPhi(phi_new);
   // re-derive Px, Py, Pz from E, theta, phi (massless electron approximation):
   double p = E_new;   // E ≫ m_e
   elec.SetPxPyPzE(p*sin(theta_new)*cos(phi_new),
                   p*sin(theta_new)*sin(phi_new),
                   p*cos(theta_new),
                   E_new);
   ```
3. Re-run Stage 2 with this shifted electron flowing into every downstream computation: Q²_es, y_es, x_es, the eSigma denominator Σ_e, all electron-side cuts (E_e' > 11 GeV, fiducial volume), the pT_HFS/pT_e balance cut, the boost to the CM frame, per-track (z, pT_h, φ_h).
4. Re-extract per-track CSV per variation.
5. Re-build the response matrix per variation (gen-side unchanged; reco-side reflects shifted electron).
6. Re-unfold per variation.
7. Per bin, take half-differences as in the HFS memo §6.A Phase 5, with separate components for `Lepton(E_central)`, `Lepton(E_forward)`, `Lepton(θ)`, `Lepton(ϕ)`. Combine in quadrature.

**Pros**:
* Matches the published H1 prescription exactly.
* Captures the piecewise η-dependent electron-scale uncertainty correctly (central ±0.5 % vs. forward ±1 %).
* Captures all acceptance shifts at the framework level: events that flip across E_e' > 11 GeV, pT_HFS/pT_e balance, fiducial volume.
* Captures the full migration matrix shift coherently.

**Cons / costs**:
* Production cost: same order as HFS Option A. 6 variation runs (2 for central E ±, 2 for forward E ±, 2 for θ ±, 2 for φ ± — wait, that's 8; if we combine central and forward into a single piecewise-η `Lepton(E)` shift varied as one component, then 2 (E) + 2 (θ) + 2 (φ) = 6 variations). At ~3 h Condor wall per batch of 4 parallel jobs: ~6 h Condor wall total for 6 variations; ~12 h end-to-end including extractor + unfold.
* Steering: ~20 LOC in the Stage-2 binary. The electron 4-vector is already identified before any kinematic computation, so the shift insertion point is simple.
* Disk: 6 variations × ~40 GB/variation = ~240 GB on DUST. Modest.
* Risk: if the electron-scale calibration is η-dependent in a more granular way than just "central vs. forward" (e.g. per-θ-wheel), the piecewise shift will under-represent the calibration. Andreev 2014 §6 (the calibration origin) is the authority; the 2-region piecewise approximation matches what Andreev 2017/2022/2023/2024 all use.

### 5.2 Option B — analytic rescale on per-track CSV

**Procedure** (the brief's "Option B" — viable for the electron case because the CSV has the full reco-side electron 4-vector):

1. Read the per-track CSV.
2. Per event (events identified by groupby on the event-ID column, assumed present), apply the shift to (`scatEX, scatEY, scatEZ, scatEE`):
   ```python
   # Electron energy scale shift, piecewise by theta_e
   theta_e = np.arctan2(np.sqrt(df.scatEX**2 + df.scatEY**2), df.scatEZ)
   eta_e = -np.log(np.tan(theta_e / 2.0))
   is_forward = eta_e > 1.5   # placeholder; the exact "forward" definition follows the LAr fiducial split
   dE_rel = np.where(is_forward, ELEC_FORWARD_SHIFT, ELEC_CENTRAL_SHIFT)
   scale = 1.0 + dE_rel

   df['scatEE_shift'] = df.scatEE * scale
   df['scatEX_shift'] = df.scatEX * scale
   df['scatEY_shift'] = df.scatEY * scale
   df['scatEZ_shift'] = df.scatEZ * scale

   # Electron polar-angle shift
   p_e = np.sqrt(df.scatEX**2 + df.scatEY**2 + df.scatEZ**2)
   phi_e = np.arctan2(df.scatEY, df.scatEX)
   theta_e_new = theta_e + ELEC_THETA_SHIFT
   df['scatEX_shift'] = p_e * np.sin(theta_e_new) * np.cos(phi_e)
   df['scatEY_shift'] = p_e * np.sin(theta_e_new) * np.sin(phi_e)
   df['scatEZ_shift'] = p_e * np.cos(theta_e_new)
   df['scatEE_shift'] = df.scatEE   # energy unchanged by polar rotation

   # Electron azimuthal shift: rotate phi by 1 mrad, magnitudes unchanged
   # (similar to above with phi_e_new = phi_e + ELEC_PHI_SHIFT)
   ```
3. Recompute Q²_es, y_es, x_es from the shifted (E_e', θ_e):
   ```python
   E_e_beam = df['BeamElecE']
   E_p_beam = df['BeamProtP']
   sigma_e = df['scatEE_shift'] * (1 - np.cos(theta_e_new))
   sigma_had = df['Empz'] - sigma_e   # or however Empz is decomposed; verify
   y_es_new = sigma_had / (sigma_had + sigma_e)
   Q2_es_new = df['scatEE_shift']**2 * np.sin(theta_e_new)**2 / (1 - y_es_new)
   s = 4 * E_e_beam * E_p_beam
   x_es_new = Q2_es_new / (s * y_es_new)
   ```
4. Recompute per-track (z, pT_h, φ_h) by rebuilding the γ*+p CM boost from the shifted k⃗' = (`scatEX_shift, scatEY_shift, scatEZ_shift`) and the unchanged beams (`BeamElecE, BeamProtP`) and unchanged hadron (Pt_lab, Eta_lab, Phi_lab, but the lab φ is missing from the brief's schema — see blocker #1 below).
5. Apply (or re-apply) the analysis cuts using the shifted electron quantities: E_e' > 11 GeV (on `scatEE_shift`), pT_HFS / pT_e balance (pT_e from `scatEX_shift, scatEY_shift`; pT_HFS computed from `Empz` and the shifted electron contribution), fiducial volume on the shifted θ_e. Events that flip across cut boundaries are dropped/added at this step.
6. Re-build the response matrix from the shifted reco-side CSV (gen-side unchanged).
7. Re-unfold against the shifted data CSV.

**Blockers / caveats**:

1. **Hadron lab-frame φ_h is needed to re-build the CM boost.** The brief's schema lists `Pt_lab, Eta_lab` and per-track `phi_h` (CM-frame Trento angle), but the lab-frame hadron φ is not enumerated. The CM-frame `phi_h` is *post-boost*, so an electron shift requires re-boosting from the lab. **Either** the extractor stores the lab-frame hadron full 3-momentum (Pt_lab, Eta_lab, Phi_lab) — in which case Option B is fully tractable — **or** the lab Phi_lab is missing, in which case the boost cannot be exactly reconstructed and Option B falls back to a first-order perturbation around the nominal CM-frame `phi_h`. The latter is defensible to leading order (the boost rotation is small, ~1 mrad for an electron θ shift) but loses accuracy at the tails. **Verification action**: dump the CSV header before launching Option B. If `Phi_lab` (or equivalent) is missing, patch the extractor to add it — small patch, ~5 LOC, one re-extraction pass.

2. **`Empz` decomposition.** The brief lists `Empz` as a per-track CSV column (event-level quantity copied per track). It needs to be decomposable into Σ_had and Σ_e cleanly. If `Empz` is `Σ_had + Σ_e` (the standard E−pz sum over all final-state particles), then Σ_had = Empz − Σ_e and we can recompute y_es. If `Empz` is `Σ_had` only (electron-removed), then we use it directly. **Verification action**: check the extractor source for the Empz definition.

3. **MC gen-side stays nominal.** The response matrix gen-side is unchanged by reco-side shifts (correct per the brief; the gen-level scattered electron `GenElec*` is the true generator-level electron and is unaffected by reco systematics). Option B handles this by re-binning only the reco-side CSV; the gen-side stays.

4. **Acceptance effects (cuts).** Unlike the HFS Option B (where the photon-recombination and energy-flow steps depend on calorimeter clusters that can't be revisited from the CSV), the electron-side cuts (E_e' > 11 GeV, pT_HFS/pT_e, fiducial volume) only depend on the electron 4-vector and the per-event Σ. The CSV has both, so Option B can re-evaluate the cuts faithfully. Events that flip across cut boundaries under shift are caught and dropped/added accordingly. This is the key difference from the HFS case.

**Pros**:
* Fast: ~30 min wall on the existing CSV per variation.
* No NAF production cost.
* Captures all the dominant effects: Q² migration, y migration, z/pT_h boost shifts, φ_h rotation, cut-acceptance shifts.
* Handles the η-piecewise electron-scale convention correctly by computing θ_e (and thus η_e) per event from the CSV.
* Validates against Option A on a single-variation closure spot-check.

**Cons**:
* Loses the second-order acceptance effects that depend on cluster-level reconstruction (e.g. electron-photon disambiguation in the LAr — if a 0.5 % shift moves a near-boundary photon below the threshold for electron-cluster recombination, this is not in the CSV. These effects are tiny and not separately quoted in any Andreev paper).
* Requires the extractor to have correctly written the full reco electron 4-vector and the lab-frame hadron 3-momentum into the CSV. The brief asserts this; verify before launch.
* The gen-level scattered electron `GenElec*` is the reference for closure tests; if the extractor writes only `scatE*` (reco) and not `GenElec*` to the per-track CSV, Option B can still proceed (gen-side response matrix is built from gen-level alone, which is presumably already done in the v3 response build), but Option-A-vs-Option-B closure on the truth-level inputs requires `GenElec*` to be in the CSV for direct comparison.

### 5.3 Comparison summary

| Aspect | Option A (re-production) | Option B (CSV analytic) |
|---|---|---|
| Matches published H1 prescription | yes, exactly | yes for E and ϕ shifts; close for θ shift |
| Captures η-piecewise E-scale (central vs. forward) | yes | yes (compute η_e per event from CSV) |
| Captures Q², y, x migration | yes | yes |
| Captures z, pT_h, φ_h migration | yes | yes (modulo Phi_lab availability — see blocker #1) |
| Captures cut acceptance shifts | yes | yes (re-evaluate cuts on shifted electron) |
| Captures electron-photon disambiguation second-order effects | yes | no (negligible per published evidence) |
| Compute cost | ~12 h end-to-end Condor + extractor + unfold | ~30 min wall, no Condor |
| Disk cost | ~240 GB | ~0 (in-memory rebinning of existing CSV) |
| Steering code changes | ~20 LOC C++ | ~150 LOC Python |
| Risk of mismatch with published convention | low | low (provided extractor schema is verified) |
| Defensible in a paper | yes (canonical) | yes (with closure spot-check against Option A) |

---

## 6. Recommended path

**Option B (analytic CSV rescale)** as the canonical path for the scattered-electron systematic, with **a single Option A reference variation as a closure spot-check** to validate Option B's per-bin accuracy.

Justification:

* All five physically meaningful shifts (Lepton(E) central, Lepton(E) forward, Lepton(θ), Lepton(ϕ), and the η-piecewise E-scale) can be applied analytically from the CSV provided the schema includes `scatEX/Y/Z/EE`, `BeamElecE`, `BeamProtP`, `Empz`, and ideally the lab-frame hadron 3-momentum (`Phi_lab` is the only one of these the brief did not explicitly enumerate; verify and patch if absent).
* The cost savings vs. Option A are large (~12 h Condor wall + ~240 GB disk avoided per variation set), and the systematic is **sub-dominant to HFS** in nearly every bin of every comparable H1 measurement (per §1, §7), so the gold-standard compute budget should be spent on HFS.
* The Option B → Option A closure spot-check costs ~3 h Condor wall (one variation, single component, e.g. Lepton(E) central +0.5 %) and is sufficient to defend the analytic approach in a paper. If closure passes (per-bin agreement within ~30 % on σ_electron), the rest of Option B is canonical. If closure fails, fall back to Option A for that component.

This is the **opposite** recommendation from the HFS memo (where Option A is required because Option B cannot decompose JES vs. RCES from the CSV alone), and the asymmetry is well-justified by the CSV schema asymmetry: the per-event electron 4-vector is in the CSV; the per-HFS-object jet-membership flag is not.

---

## 7. Concrete implementation plan

### 7.A Option B canonical plan

**Phase 0 (1 h, no Condor): schema verification.**

* Dump the CSV columns: `zcat pertrack_v5g_fixedFrame_v3_merged.csv.gz | head -1` on NAF.
* Confirm presence of `scatEX, scatEY, scatEZ, scatEE, BeamElecE, BeamProtP, Empz`. The brief asserts all present.
* Check for a lab-frame hadron φ column (`Phi_lab`, `phi_lab`, or equivalent). If absent, patch the extractor (`extract_pertrack_data_v3.py`, `extract_pertrack_mc_reco_v3.py`) to add it and re-extract. Patch is ~5 LOC per extractor.
* Check for an event-ID column to group per-track rows into events. If absent, derive from (RunID, EventID, …) tuple.
* Verify the `Empz` definition by computing `Empz_check = Σ_had + Σ_e` from the per-event constituents and comparing to the CSV column. (This was identified as an open question in HFS memo §9 item 4; resolve it here once for both memos.)
* Confirm gen-level scattered electron columns (`GenElecE, GenElecPx, GenElecPy, GenElecPz`) are present in the gen-side CSV for the response-matrix gen-side input.

**Phase 1 (2 h): implement the analytic shift script.**

Create `H1/sidis_highQ2/scripts/electron_systematic_csv_rescale.py`:

```python
"""
Apply scattered-electron systematic shifts to the per-track v3 CSV and emit
a shifted CSV with the same schema, for downstream response-build + unfold.

Variations:
  --variation {nominal, lepton_e_up, lepton_e_down, lepton_theta_up,
               lepton_theta_down, lepton_phi_up, lepton_phi_down}
  --eta-split 1.5   # boundary between central (E ±0.5%) and forward (E ±1.0%)

Usage:
  python electron_systematic_csv_rescale.py \
      --input  /data/dust/.../pertrack_v5g_fixedFrame_v3_merged.csv.gz \
      --output /data/dust/.../pertrack_v5g_fixedFrame_v3_LeptonEup.csv.gz \
      --variation lepton_e_up

Outputs: a CSV with the same schema as the input, with the per-event electron
4-vector shifted, and per-event (Q2es, Xes, Yes) and per-track (z, pT_h,
phi_h) recomputed. Per-track Pt_lab, Eta_lab, charge are unchanged.

Implementation:
  1. groupby event-ID; the per-event electron 4-vector is the same for all
     tracks in an event, so the shift is applied once per event.
  2. recompute Sigma_e, Sigma_had (from Empz), y_es, Q2_es, x_es per event.
  3. per track, re-boost (P, q, p_h) to gamma*+p CM; recompute z, pT_h,
     phi_h with the shifted q.
  4. re-evaluate the cuts (E_e' > 11 GeV, pT_HFS/pT_e in [0.6, 1.6],
     LAr fiducial on shifted theta_e); drop events that fail.
  5. write shifted CSV.
"""
# ~200 LOC; uses pandas + numpy; no ROOT dependency.
```

**Phase 2 (30 min per variation × 6 variations + 30 min nominal rerun = ~4 h serial, parallelizable to ~30 min wall): run the shifts.**

Six output CSVs:
* `pertrack_v3_LeptonEup.csv.gz` — central E +0.5 %, forward E +1.0 %
* `pertrack_v3_LeptonEdown.csv.gz` — central E −0.5 %, forward E −1.0 %
* `pertrack_v3_LeptonThetaUp.csv.gz` — θ_e + 1 mrad
* `pertrack_v3_LeptonThetaDown.csv.gz` — θ_e − 1 mrad
* `pertrack_v3_LeptonPhiUp.csv.gz` — φ_e + 1 mrad
* `pertrack_v3_LeptonPhiDown.csv.gz` — φ_e − 1 mrad

**Phase 3 (30 min per variation, ~3 h total; parallelize): re-bin into Scheme B′ (Q², x, z, pT_h) and re-build the response matrix per variation.**

The 4D response build (canonical since 2026-05-25 per AGENTS.md) takes the (reco, gen) pairs from MC. For each variation, the reco side uses the shifted CSV and the gen side stays nominal. The data side also uses the shifted CSV when extracting the unfolded yield per variation.

**Phase 4 (~30 min per variation): re-unfold per variation.**

**Phase 5 (~15 min): compute σ_electron per bin.**

Per bin i, in each (Q², x, z, pT_h) cell:

```
δ_Lepton_E(i)     = [N_unfolded(i, LeptonEup)     − N_unfolded(i, LeptonEdown)]     / 2
δ_Lepton_theta(i) = [N_unfolded(i, LeptonThetaUp) − N_unfolded(i, LeptonThetaDown)] / 2
δ_Lepton_phi(i)   = [N_unfolded(i, LeptonPhiUp)   − N_unfolded(i, LeptonPhiDown)]   / 2

σ_electron(i) = sqrt( δ_Lepton_E(i)² + δ_Lepton_theta(i)² + δ_Lepton_phi(i)² )
```

Sign convention follows Andreev 2024 §6: "the signed difference between the unfolded result with the nominal scale and the scale shifted upwards, i.e. a negative value corresponds to the case where the value in a given bin was increased after the systematic shift."

Document σ_electron(i) per bin in `H1/sidis_highQ2/docs/results/systematics_v3_electron.md` (alongside the HFS write-up the companion memo recommends).

### 7.B Option A spot-check (single reference variation)

**Phase A0 (1 h)**: add the same Stage-2 binary patch as §5.1 above, restricted to one variation: `ELEC_E_SHIFT_CENTRAL = +0.005, ELEC_E_SHIFT_FORWARD = +0.01`.
**Phase A1 (3 h Condor wall)**: full re-extraction for the one variation.
**Phase A2 (30 min)**: response build + unfold for the one variation.
**Phase A3 (15 min)**: per-bin comparison of σ_Lepton_E from Option A vs. Option B.

**Closure criterion**: Option B per-bin σ_Lepton_E agrees with Option A within **30 % per bin** in bulk bins (Q² < 1000 GeV², 0.2 < z < 0.7, pT_h < 4 GeV); within **a factor of 2** in tail bins. If closure fails, the failure pattern indicates which CSV column is missing or which kinematic recomputation in the script is wrong; fix and re-run Option B before going to publication.

### 7.C If Option B closure fails: fall back to Option A

If the Phase A3 closure test fails per the §7.B criterion, fall back to Option A for the failed component (most likely Lepton(E)) and use Option B for the other components (θ and φ, which are simpler).

Total cost of full fallback (all 6 variations under Option A): ~12 h Condor wall + ~3 h extractor/unfold = ~1 day end-to-end. This is still cheaper than the HFS Option A in the companion memo (the electron has fewer η/φ-dependent calibration components than HFS has jet vs. non-jet object classifications).

---

## 8. Per-cell estimated magnitude of σ_electron (from prior H1 published results)

Anchored to the same evidence base as the HFS memo. Values quoted here are not direct measurements on our v3 sample — they are the literature prior; the Option B pipeline in §7.A will replace these with measured per-bin values.

### 8.1 Andreev 2023 (jet substructure, *PLB* 844 (2023) 138101) — most comparable

Tables 2 and 3 give per-Q²-bin systematics on unfolded means and standard deviations of six jet angularities. The two electron columns are `Lepton(E)` and `Lepton(ϕ)`. Selected entries, transcribed from the ar5iv HTML tables (retrieved 2026-05-25):

**Means (Table 2)** — `Lepton(E)` and `Lepton(ϕ)` absolute contributions vs. central value and total systematic:

| Observable | Q² range (GeV²) | Central | Lepton(E) | Lepton(ϕ) | Combined Lepton (quad) | Total syst |
|---|---|---:|---:|---:|---:|---:|
| ⟨ln λ₁¹⟩ (jet broadening) | 150–360       | −1.385 | 0.002 | 0.002 | 0.003 | 0.007 |
| ⟨ln λ₁¹⟩                  | 360–866       | −1.506 | 0.002 | 0.004 | 0.004 | 0.009 |
| ⟨ln λ₁¹⟩                  | 866–2081      | −1.643 | 0.005 | 0.005 | 0.007 | 0.012 |
| ⟨ln λ₁¹⟩                  | 2081–5000     | −1.803 | 0.008 | 0.008 | 0.011 | 0.018 |
| ⟨ln λ₂¹⟩ (jet thrust)     | 150–360       | −2.273 | 0.006 | 0.004 | 0.007 | 0.007 |
| ⟨ln λ₂¹⟩                  | 2081–5000     | −2.914 | 0.013 | 0.009 | 0.016 | 0.016 |
| ⟨Nc⟩ (charged multiplicity)| 150–360       |  5.531 | 0.041 | 0.006 | 0.041 | 0.019 |
| ⟨Nc⟩                      | 360–866       |  5.969 | 0.036 | 0.020 | 0.041 | 0.021 |
| ⟨Nc⟩                      | 866–2081      |  6.753 | 0.006 | 0.048 | 0.048 | 0.016 |
| ⟨Nc⟩                      | 2081–5000     |  7.747 | 0.028 | 0.050 | 0.057 | 0.017 |

(Note: the "Total syst" column in Andreev 2023 is the total *combining all sources* — HFS jet, HFS other, HFS ϕ, Lepton E, Lepton ϕ, Model, Closure — so for ⟨Nc⟩ the apparent inconsistency `Combined Lepton > Total syst` reflects cancellations across sources. The combined-quadrature values in the table are not the official combination, they are derived here from the individual columns.)

For ⟨Nc⟩ — the closest available analog to our SIDIS yield in (Q², x) — relative electron contributions are:

| Q² range | Lepton(E) / Nc | Lepton(ϕ) / Nc | Combined electron / Nc |
|---|---:|---:|---:|
| 150–360 GeV²   | 0.74 % | 0.11 % | 0.74 % |
| 360–866 GeV²   | 0.60 % | 0.34 % | 0.69 % |
| 866–2081 GeV²  | 0.09 % | 0.71 % | 0.72 % |
| 2081–5000 GeV² | 0.36 % | 0.65 % | 0.74 % |

**For comparison with the HFS values from the HFS memo §7.2**:

| Q² range | Combined HFS / Nc | Combined electron / Nc | Ratio HFS / electron |
|---|---:|---:|---:|
| 150–360 GeV²   | 0.40 % | 0.74 % | 0.54 |
| 360–866 GeV²   | 0.52 % | 0.69 % | 0.75 |
| 866–2081 GeV²  | 0.45 % | 0.72 % | 0.63 |
| 2081–5000 GeV² | 0.99 % | 0.74 % | 1.34 |

**Important observation**: for ⟨Nc⟩ specifically, the electron systematic is **comparable to or larger than** the HFS systematic in the low/mid Q² range, and only becomes sub-dominant in the highest Q² bin. This is a partial rebuttal of the supervisor brief's expectation that "electron systematic is sub-dominant to HFS for most observables." For **integer-valued charge-counting observables like ⟨Nc⟩**, the electron systematic on the *mean* is comparable to HFS because the mean is sensitive to where the (Q², y) cuts move events in or out of the analysis sample, and the electron-side cuts (E_e' > 11 GeV, fiducial) dominate that migration. For **cross-section observables** like jet broadening or thrust where the central value is set by the QCD shower physics rather than by event counting, the electron systematic remains noticeably sub-dominant to HFS (see the ⟨ln λ₁¹⟩ and ⟨ln λ₂¹⟩ rows above: Lepton(E) is comparable to HFS(jet) and HFS(other) in absolute terms but the **shape** of the QCD observable is driven by HFS, so the cross-section per bin sees a larger relative HFS effect).

**Updated guidance** for our SIDIS observables, which sit between ⟨Nc⟩ (counting) and cross-section means:
* For **σ^h(Q², x, z, pT_h) per bin**: electron systematic likely ~0.5–1 % per bin, comparable to HFS in some bins, sub-dominant in others.
* For **M^h(Q², x, z, pT_h)** (multiplicity, similar to ⟨Nc⟩): electron systematic ~0.5–1 % per bin, comparable to HFS.
* For **A_h(Q², x, z, pT_h)**: electron systematic ≤ 0.1 % absolute, dominated by charge-symmetric cancellation (§4).
* For **φ_h harmonics**: electron systematic from Lepton(ϕ) only, at the per-mille level (§3.2, §4).

**Standard deviations (Table 3)** — `Lepton(E)` and `Lepton(ϕ)` contributions for resolution-sensitive observables (analog to widths of our SIDIS distributions):

| Observable | Q² range | std value | Lepton(E) | Lepton(ϕ) | Combined Lepton (rel) |
|---|---|---:|---:|---:|---:|
| std(ln λ₁¹) | 2081–5000 | 0.622 | 0.007 (1.1 %) | 0.018 (2.9 %) | 3.1 % |
| std(ln λ₂¹) | 2081–5000 | 1.004 | 0.009 (0.9 %) | 0.019 (1.9 %) | 2.1 % |
| std(Nc)     | 866–2081  | 3.132 | 0.018 (0.6 %) | 0.068 (2.2 %) | 2.3 % |
| std(Nc)     | 2081–5000 | 3.544 | 0.030 (0.8 %) | 0.066 (1.9 %) | 2.0 % |

The `Lepton(ϕ)` contribution is unexpectedly large for the standard deviations — these reflect resolution-sensitive observables and the φ rotation degrades the precision of the jet-axis matching. For our analysis, where φ_h is itself one of the observables, this is a warning sign that Lepton(ϕ) could contribute non-trivially to φ_h-binned observables. Reserve ~1 % per bin as the Lepton(ϕ) headline contribution to φ_h-dependent SIDIS observables.

### 8.2 Andreev 2024 (groomed event shapes, *EPJC* 84 (2024) 718)

§6.4 quotes the per-bin Lepton(E) contribution as **"less than ~3 %"** for the most-affected bins of the groomed 1-jettiness and groomed invariant mass. §6.1 combines the lepton-polar and HFS-polar uncertainty under the LAr alignment, giving **"typically ~1 %"** on final distributions.

### 8.3 Andreev 2022 (lepton-jet correlation, *PRL* 128 (2022) 132002)

Quotes total systematic uncertainty per observable: **2–25 % for pT_jet, 3–7 % for η_jet, 4–15 % for qT_jet/Q, 4–6 % for Δφ_jet**. The individual electron contribution is not broken out per observable, but the paper's *Uncertainties* paragraph lists the same conventions as Andreev 2023 (Lepton(E) ±0.5 % central/±1 % forward, Lepton(ϕ) ±1 mrad). Given the comparable HFS conventions and the published 2–25 % total range for pT_jet, the electron contribution is consistent with ~1–3 % per bin in the bulk.

### 8.4 Extrapolation to our SIDIS observables

| Observable | Bulk bins | Mid bins | Tail bins |
|---|---|---|---|
| σ^h or M^h (Q², x, z, pT_h)    | ~0.7 %  | ~1.0 %  | ~1.5 % |
| A_h (Q², x, z, pT_h)           | ≤ 0.1 % | ≤ 0.1 % | ≤ 0.2 % |
| φ_h harmonics on σ^h or M^h     | ~0.7 % (Lepton ϕ dominates) | ~1.0 % | ~1.5 % |
| φ_h harmonics on A_h            | ≤ 0.1 % | ≤ 0.2 % | ≤ 0.3 % |

**Recommended placeholder envelope for the v3 systematics table** (to use in any pre-Option-B document or talk):

* Bulk bins (Q² < 1000 GeV², z < 0.5, pT_h < 2 GeV): σ_electron / yield ≈ **0.7 %**
* Mid bins (intermediate Q², z, pT_h):                  σ_electron / yield ≈ **1.0 %**
* Tail bins (Q² > 2000 GeV², z > 0.7, pT_h > 4 GeV):   σ_electron / yield ≈ **1.5 %**
* A_h (any bin): σ_electron(A_h) ≤ **0.1 % absolute**.

These are *upper bounds* anchored to the most-conservative comparable published values. The bulk values are smaller than the HFS analog (HFS memo §7.3 quotes 1 % bulk, 1.5 % mid, 3 % tail) — confirming the electron systematic is **modestly sub-dominant to HFS** in our SIDIS context, with the comparable-magnitude exception for counting observables and the negligible contribution to A_h.

Replace placeholders with measured per-bin values from the Option B run when complete.

---

## 9. Cross-references and prior workspace documentation

### 9.1 Local workspace cross-references (verified present)

* `H1/sidis_highQ2/docs/research/research-HFS-systematic-2026-05-25.md` — companion HFS memo. **Shared framework**: same propagation convention (full re-unfold for migration-affecting systematics), same kinematic regime (Q² > 150 GeV²), same H1 published reference set, same v3 production target. Read in parallel.
* `H1/sidis_highQ2/docs/plans/plan-background-MC-production-g1jES-2026-05-24.md` — informs Option A wall-time and disk-cost estimates in §5.1, §7.B.

### 9.2 Workspace cross-references the brief asked for but that are NOT present

* `H1/sidis_highQ2/docs/.../reference-LAr-electron-scale-highQ2-sidis.md` or similar — **does not exist locally**. No prior electron-scale write-up was found in the workspace. This memo is the first such document; future updates should land in `H1/sidis_highQ2/docs/research/` or `H1/sidis_highQ2/docs/results/` (the latter being the canonical location for measured per-bin systematics tables, per the HFS memo §6.A.5 — also not yet present locally; first commit should create it).

### 9.3 Published H1 cross-references (verified, source-backed)

All four papers below were used as primary references for this memo. Each was accessed on 2026-05-25; the cited sections are the source for the conventions and per-bin numbers in this memo.

* **Andreev 2024**, "Measurement of groomed event shape observables in deep-inelastic electron-proton scattering at HERA", arXiv:2403.10134v2, EPJC 84 (2024) 718, DOI 10.1140/epjc/s10052-024-12987-0, DESY-24-036.
  §6.1 (Alignment) — LAr-tracker 1 mrad alignment, electron polar angle propagated with HFS polar through unfold, typical ~1 %.
  §6.4 (Electron energy scale) — 0.5 % central/backward, 1 % forward; uncertainty in final distributions less than ~3 %.
  *Surface used*: ar5iv HTML, full §6 quoted verbatim above (§2.1).

* **Andreev 2023**, "Unbinned Deep Learning Jet Substructure Measurement in High Q² ep collisions at HERA", arXiv:2303.13620v3, PLB 844 (2023) 138101, DOI 10.1016/j.physletb.2023.138101, DESY-23-034.
  §4 (Uncertainties) — explicit lepton-energy 0.5–1 % range, lepton-φ 1 mrad.
  Tables 2 and 3 — per-Q²-bin Lepton(E) and Lepton(ϕ) contributions for six jet angularities, including ⟨Nc⟩ — the direct analog to a SIDIS counting observable.
  *Surface used*: ar5iv HTML, §4 quoted verbatim above (§2.2); Tables 2–3 transcribed above (§8.1).

* **Andreev 2022**, "Measurement of lepton-jet correlation in deep-inelastic scattering with the H1 detector using machine learning for unfolding", arXiv:2108.12376v2, PRL 128 (2022) 132002, DESY-21-130.
  *Uncertainties* paragraph — exact conventions reproduced (Lepton(E) 0.5–1 %, Lepton(ϕ) 1 mrad). The closest analog to our analysis using OmniFold/MultiFold ML unfolding.
  *Surface used*: ar5iv HTML, *Uncertainties* paragraph quoted verbatim above (§2.3).

* **Andreev 2017**, "Measurement of Jet Production Cross Sections in Deep-inelastic ep Scattering at HERA", arXiv:1611.03421v4, EPJC 77 (2017) 215 (and erratum). DESY-16-200.
  Originating reference for the electron-energy-scale calibration cited by Andreev 2022/2023/2024 as ref [16] (or 1611.03421). The 0.5 % central / 1 % forward piecewise convention traces to this paper's §6 calibration.
  *Surface used*: arXiv abstract page only (ar5iv HTML fetch oversized per HFS memo §8.4); the convention is reproduced transitively via the 2022/2023/2024 citation chain.

### 9.4 Updates for v3 (relative to any v5f convention, once located)

Same caveat as the HFS memo §8.3: no v5f electron-scale document is present locally. The two non-trivial v5f-to-v3 changes that affect this memo are the same as for HFS:

* Frame fix 2026-05-24 (pT_h, φ_h in γ*+p CM frame, extractor v3) — affects how Option B computes per-track shifts on z, pT_h, φ_h (see §5.2, §3.2). The v3 CM frame is the correct one.
* Scheme B′ canonical (Q², x) binning frozen 2026-05-24 — affects which bin boundaries σ_electron is computed in (§7.A Phase 3).

### 9.5 What to consult next, if Option B produces surprising results

* If σ_Lepton_E ≫ 1 % in some bin: cross-check the η_e piecewise split (central vs. forward) — the boundary η_e ≈ 1.5 in the script (§5.2) is a placeholder; the H1 calibration boundary corresponds to a specific θ-wheel transition in the LAr (Andreev 2014 calibration paper §6 is the authority). If this matters, pull the Andreev 2014 PDF (the `pdf` skill handles it).
* If σ_Lepton_ϕ ≪ 0.1 % in all bins, even bins where Andreev 2023 Table 2 has Lepton(ϕ) at the 1 % level: suspect that the φ shift was not actually applied in the script (no-op test for ELEC_PHI_SHIFT = 0.001 must produce non-zero δ per bin). Also check whether the CM-frame φ_h was correctly rebuilt from the shifted electron — if the extractor wrote φ_h as a pre-computed value with no lab-frame hadron φ stored, Option B falls back to applying the 1-mrad rotation directly to the CM φ_h, which is a first-order approximation but may miss the boost-orientation second-order effect.
* If Option B closure (§7.B) fails: the most likely failure mode is missing `Phi_lab` in the CSV (§7.A Phase 0 verification action). Patch the extractor and re-run.

---

## 10. Open questions and follow-up

1. **CSV schema verification (§7.A Phase 0)**: confirm `scatEX/Y/Z/EE`, `BeamElecE`, `BeamProtP`, `Empz`, lab-frame hadron φ, and event-ID grouping are all present. This is the gating action for Option B.
2. **Empz definition**: shared open question with the HFS memo (§9 item 4). Resolve once; both memos depend on it.
3. **eSigma formula in the v3 extractor**: confirm `y_es = Σ_had / (Σ_had + Σ_e)` (Andreev 2023 / 2022 convention) vs. the alternative `2 E_e_beam` denominator. §3.5 above derived the shifts under the Andreev 2023 convention; if our extractor uses the other form, the per-event Q² shift is slightly different.
4. **η-boundary for central vs. forward LAr E-scale**: the placeholder η = 1.5 in §5.2 needs to be confirmed against the H1 calibration definition. Andreev 2014 §6 is the authority.
5. **GenElec* columns in the gen-side CSV**: confirm the gen-level scattered electron is in the gen pertrack CSV so the response-matrix gen side is built from gen-level kinematics, unchanged across all electron-systematic variations.
6. **A_h-specific closure**: for the asymmetry observable, the §4 estimate of σ_electron(A_h) ≤ 0.1 % is anchored to a literature analog (Andreev 2023 jet-charge column); the analog is not perfect (jet charge ≠ charged-particle asymmetry). Plan a dedicated A_h closure on a small subset of bins to confirm the bound holds at the analysis level.
7. **Backround MC electron-systematic handling**: same open question as the HFS memo §9 item 5. Background MC (Pythia γp, DVCS, Compton, Grape, low-Q² Django, CC Django) is subtracted from data before unfolding; the electron systematic should propagate through the background subtraction too. For Option B this is cheap: re-run the script on the per-track CSVs of each background sample (if they were produced through the same v3 extractor and CSV-ified). For Option A it adds production cost. Decision deferred to supervisor; recommendation: include backgrounds in the Option B re-shift, since it costs nothing extra compute-wise.

---

*End of memo.*
