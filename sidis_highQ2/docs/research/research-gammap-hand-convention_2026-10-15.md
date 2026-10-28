# Research — Hand-convention virtual-photon flux and the σ(γ\*p) observable convention for the high-Q² charged-hadron SIDIS γ\*p conversion (primary-literature verification)

**Date:** 2026-10-15 (research completed 2026-10-15/16) · **Type:** read-only literature and convention research; no code, no tree running, no release product touched; this file is the only artifact written by this task.
**Regime:** high-Q² inclusive charged-hadron SIDIS, 150 < Q² < 20000 GeV², 0.1 < y < 0.7, z > 0.05 fiducial, 490 release bins (7 (Q²,x) cells × 7 z × 5 p_T, two charges).
**Companion documents (reused, not duplicated):**
- `sidis_highQ2/docs/research/gammap-conversion-prescription-2026-10-15.md` — the design memo: per-bin effective flux ⟨1/Γ⟩, prototype numbers, phased plan (hereafter "the prescription memo");
- `sidis_highQ2/docs/research/gammap-electroweak-level-2026-10-15.md` — the EW-level review: pure photon Born at α(0) via C_gamma_born (hereafter "the EW memo");
- `sidis_highQ2/docs/audits/audit_gammaflux_inputs_2026-10-15.md` — branch audit (per-event Γ computable; no y/ε/Γ columns in release tables);
- `sidis_highQ2/docs/methodology/map_ep_xsec_pipeline_touchpoints_2026-10-15.md` — pipeline touchpoints, risks R1–R9, diagonal-congruence covariance;
- `sidis_highQ2/docs/research/research-LT-separation-feasibility.md` — R and ε expectations (§3.3, §5.1);
- `exclu_phi/docs/research-cross-section-method.md` — in-house transcription of the H1 flux (independently re-verified against the primary source here).

**Facts this document is consistent with (established by other specialists / supervisor):**
1. Single beam configuration: e± 27.6 GeV on 920 GeV protons, fixed s = 101 568.0 GeV² (√s = 318.697 GeV); **no 575 or 460 GeV proton-energy period in the sample** — the mixed-beam-energy flux problem is MOOT.
2. The Born-level release is the **full neutral-current (γ+Z) ep Born cross section**; a γ\*p presentation requires the pure electromagnetic (photon-only, α(0)) Born level — delivered by the C_gamma_born sidecar (`C_gamma_born = C_rad × F_EWgamma`).
3. `C_rad` is verified to be a pure RAD/NONRAD yield ratio with **no flux factor in it** — dividing by Γ afterwards introduces no double counting.

---

## BOTTOM LINE / RECOMMENDED PRESCRIPTION

> **σ^{γ\*p}(bin) = σ^{ep,γ-only Born,α(0)}(bin) × ⟨1/Γ⟩(bin)**
>
> with the **H1-house Hand flux** (H1, "Diffractive Electroproduction of ρ and φ Mesons at HERA", JHEP 1005 (2010) 032, arXiv:0910.5831, §5.1, Eqs. (9)–(10), verified verbatim from the primary source this session):
>
>   **Γ(y,Q²) = (α_em/π) · (1 − y + y²/2) / (y · Q²)**  [GeV⁻², density in the (y,Q²) measure]
>
> and the γ\*p observable defined as the **ε-weighted total**:
>
>   **σ^{γ\*p} ≡ σ_T + ε̄ σ_L = σ_T (1 + ε̄ R)**,  **ε = (1−y)/(1−y+y²/2) ≡ 2(1−y)/(1+(1−y)²)**
>
> — identical to ZEUS (arXiv:hep-ex/0504010, §4) and equal, at γ→0, to the Trento ε of Bacchetta et al. (arXiv:hep-ph/0611265, Eq. (8)) with target-mass corrections **|Δε| ≤ 5×10⁻⁶** over our whole fiducial phase space (measured; analytic corner bound ≤ 2×10⁻⁶).
>
> - **EW level:** pure one-photon-exchange Born at fixed α(0) = 1/137.0359895 — σ^{ep,γ-only Born}(bin) = RAW(bin) × C_gamma_born(bin) (EW memo §1–2). α_em in Γ is the same α(0).
> - **Flux averaging:** per-bin, cross-section-weighted, MC-derived **⟨1/Γ⟩(bin) = Σ_i w_i/Γ(y_i,Q²_i) / Σ_i w_i** from the DJANGOH NoZ photon-Born e⁻ truth sample (prescription memo §3). No y-binning, no re-unfolding.
> - **Measure:** (y, Q², z, p_T) — bin-integrated product in **nb·GeV²**.
> - **R = σ_L/σ_T does not enter the central value**; ε̄ is shipped per cell so any consumer can convert to σ_T with their own R (at a 3.4–8.5 % penalty, §Q3).
> - Units guide: 1/Γ is in GeV²; per-bin γ\*p values span ⟨1/Γ⟩ = 16 066 – 2 886 400 GeV² (prototype, prescription memo App. A).

---

## Q1 — The Hand-convention flux, K_H, and ε with target-mass terms; numeric sizes at Q² ≥ 150 GeV²

### 1.1 The published H1/ZEUS form (primary-source verified)

H1, arXiv:0910.5831, §5.1 (fetched from ar5iv 2026-10-15/16; equations extracted verbatim from the latexml `alttext`):

- Eq. (9): **σ(γ\*+p→V+Y) = (1/Γ) · d²σ(e+p→e+V+Y)/dy dQ²**
- Eq. (10): **Γ = (α_em/π) · (1−y+y²/2)/(y·Q²)**, with y = p·q/(p·k)
- **σ_tot(γ\*+p→V+Y) = σ_T + ε σ_L = σ_T (1 + ε R)**, **ε ≃ (1−y)/(1−y+y²/2)**; in their domain 0.91 < ε < 1.00 with ⟨ε⟩ = 0.98.
- Level statement (verbatim): "Elastic and proton dissociative cross sections are given at the Born level (i.e. they are corrected for QED radiation effects) in terms of γ\*p cross sections." — i.e. **published HERA γ\*p numbers are QED-Born-level, one-photon-exchange objects** (see Q-electroweak note below).
- The paper's bibliography cites **L.N. Hand, Phys. Rev. …1834** (the standard record is Phys. Rev. 129 (1963) 1834; author/journal/page verified in the fetched bibliography entry, volume/year per the standard record).

ZEUS, "Exclusive electroproduction of φ mesons at HERA" (S. Chekanov et al., Nucl. Phys. B 718 (2005) 3; arXiv:hep-ex/0504010, §4; title verified from arXiv metadata this session):

- **d²σ^{ep→eφp}/dy dQ² = Γ_T(y,Q²) (σ_T + ε σ_L)**, with Γ_T "the flux of transverse virtual photons" and **ε = 2(1−y)/(1+(1−y)²)** — algebraically identical to H1's ε.

So the HERA house convention is a **single transverse flux Γ_T** with the longitudinal contribution carried *inside* σ^{γ\*p} as εσ_L; no separate Γ_L σ_L term appears anywhere in the published formulae. The electron-mass terms of the exact Weizsäcker–Williams form (present in generator fluxes, e.g. DIFFVM `gephot.f`) are omitted deliberately — they are ≤ 10⁻⁹ at Q² ≥ 150 GeV², E_e ≈ 27.6 GeV (prescription memo §2 Option B).

### 1.2 The Hand K_H connection — a bookkeeping convention that MUST be frozen

Hand's original flux convention chooses the virtual-photon energy K = (W²−M²)/(2M) = ν(1−x) (Hand 1963). The H1/ZEUS published flux Eq. (10) contains no explicit K; the K choice is absorbed into the normalization of σ_T. First-principles matching (this session's derivation, γ→0; standard one-photon reduced cross section d²σ^{ep}/dxdQ² = (2πα²/(xQ⁴))·Y₊·[F₂ − (y²/Y₊)F_L], Y₊ = 1+(1−y)²):

- The H1-house flux implies **σ_T = (4π²α/Q²)·2xF₁** and **σ_L = (4π²α/Q²)·F_L**, i.e. **σ_T + σ_L = (4π²α/Q²)·F₂** (the "Q²-convention", K = ν-type normalization; σ_T = (4π²α/(Mν))F₁).
- The Hand-K_H-normalized quantities differ by the pure factor **ν/K_H = 1/(1−x)**: σ^{Hand} = σ^{H1-house}/(1−x), and correspondingly Γ^{Hand} = Γ^{H1-house}·(1−x) — the product Γ·σ (the ep cross section) is invariant.
- At VM electroproduction kinematics (x ~ 10⁻³) this is a 0.1 % bookkeeping detail, which is why the H1/ZEUS VM papers never discuss it. **At our x it is not:** with populated x up to ~0.15 (S2_x1) and cell-center x from 0.004 to 0.14:

| cell | x_gm | 1/(1−x_gm) | x_max (populated) | 1/(1−x_max) |
|---|---|---|---|---|
| S0_x0 | 0.0039 | 1.0039 | 0.0071 | 1.0072 |
| S0_x1 | 0.0146 | 1.0148 | 0.0291 | 1.0300 |
| S1_x0 | 0.0082 | 1.0083 | 0.0160 | 1.0163 |
| S1_x1 | 0.0332 | 1.0344 | 0.0680 | 1.0729 |
| S2_x0 | 0.0183 | 1.0187 | 0.0340 | 1.0352 |
| S2_x1 | 0.0714 | 1.0769 | 0.1456 | 1.1704 |
| S3_x0 | 0.1448 | 1.1694 | 0.975 (extreme corner; cross-section-negligible) | (39.4 at the corner — unpopulated in practice) |

**Consequence:** the σ-normalization convention must be frozen once and recorded. Recommendation (consistent with the prescription memo's frozen `flux_convention` string): use the **H1-house (Q²-convention) normalization** — it is what H1/ZEUS published, and it makes σ^{γ\*p} exactly (4π²α/Q²)× the reduced cross section (Q7). Any comparison against σ tables extracted in the Hand-K_H normalization must apply the 1/(1−x) rescale (up to +17 % at x = 0.15, ~+4 % at x = 0.034). The in-house exclusive-φ "3–5 % flux-convention systematic" belongs to the low-Q² φ regime where Q²/ν²-type factors are large; it does **not** transfer to Q² > 150 GeV² (prescription memo §2 Option C).

### 1.3 ε with target-mass (γ²) terms — exact form and numeric size

Bacchetta et al., arXiv:hep-ph/0611265 (JHEP 0702:093), Eq. (7) cross-section prefactor and Eq. (8) ε (both verified verbatim from ar5iv this session):

- Eq. (7): dσ ∝ (α²/(xyQ²)) · (y²/(2(1−ε))) · **(1 + γ²/(2x))** · {F_UU,T + ε F_UU,L + azimuthal}, with γ = 2Mx/Q.
- Eq. (8): **ε = (1 − y − ¼γ²y²) / (1 − y + ½y² + ¼γ²y²)** — the γ→0 limit is exactly the H1/ZEUS ε.

Numeric sizes over our fiducial (0.1 < y < 0.7, x < 1, s = 101 568.0 GeV², M = 0.938272 GeV; grid scan this session, cross-checked against the prescription memo's measured values):

| cell | Q² range | x range | populated y reach | γ²_max | γ²y²/4_max (=M²Q²/s²) | max \|ε_Trento − ε_H1\| | ε̄ (measured, full sample) |
|---|---|---|---|---|---|---|---|
| S0_x0 | 150–300 | 0.0021–0.0071 | [0.208, 0.700] | 1.2×10⁻⁶ | 2.6×10⁻⁸ | 7.5×10⁻⁸ | 0.815 |
| S0_x1 | 150–300 | 0.0071–0.030 | [0.100, 0.416] | 1.0×10⁻⁵ | 2.6×10⁻⁸ | 7.1×10⁻⁸ | 0.975 |
| S1_x0 | 300–700 | 0.0042–0.016 | [0.185, 0.700] | 3.0×10⁻⁶ | 6.0×10⁻⁸ | 1.7×10⁻⁷ | 0.823 |
| S1_x1 | 300–700 | 0.016–0.069 | [0.100, 0.431] | 2.4×10⁻⁵ | 6.0×10⁻⁸ | 1.7×10⁻⁷ | 0.976 |
| S2_x0 | 700–1500 | 0.0099–0.034 | [0.203, 0.700] | 5.8×10⁻⁶ | 1.3×10⁻⁷ | 3.7×10⁻⁷ | 0.813 |
| S2_x1 | 700–1500 | 0.034–0.150 | [0.100, 0.434] | 5.1×10⁻⁵ | 1.3×10⁻⁷ | 3.6×10⁻⁷ | 0.975 |
| S3_x0 | 1500–20000 | 0.021–1.5 | [0.100, 0.700] | 3.5×10⁻⁴ (analytic corner; 2.6×10⁻⁴ measured) | 1.7×10⁻⁶ | 5.0×10⁻⁶ | 0.881 |

- ε at the y-cut endpoints: **ε(0.1) = 0.99448, ε(0.7) = 0.55046** → populated ε ∈ [0.550, 0.994] (the supervisor's [0.55, 0.99]).
- **Identity (verified numerically): Q²/ν² ≡ γ² = 4M²x²/Q²** — the "longitudinal-flux suppression parameter" of the prescription memo §1 is the same γ²; ≤ 3.5×10⁻⁴ at the analytic corner of S3.
- The Trento normalization factor (1+γ²/2x) ≤ 1 + 1.7×10⁻⁴ everywhere in the fiducial (bound from the y > 0.1 cut: x ≤ Q²/(0.1·s)).
- **Bottom line for Q1:** all target-mass (γ²) corrections to the flux/ε — Bacchetta-vs-H1 ε, (1+γ²/2x), Q²/ν², electron-mass terms — are **≤ 0.03 % at flux level and ≤ 5×10⁻⁶ in ε** over our whole phase space; two to three orders below the 5.80/6.19 % budget. Use the simple H1-house forms; record the bound once in the flux-table README (prescription memo §2). The γ²-exact Trento ε may be used in the sidecar at zero cost if desired.

---

## Q2 — Which σ^{γ\*p} convention does each experiment use, and which ONE do we recommend

| Experiment / framework | Published convention | Evidence |
|---|---|---|
| **H1 exclusive VM** (ρ/φ electroproduction) | σ^{γ\*p} ≡ σ_T + εσ_L, single transverse flux Γ Eq. (10), Born level | arXiv:0910.5831 §5.1 Eqs. (9)–(10) + σ_tot decomposition + ε range — **verified verbatim this session** |
| **ZEUS exclusive VM** (φ electroproduction) | identical: d²σ/dy dQ² = Γ_T(σ_T+εσ_L), ε = 2(1−y)/(1+(1−y)²) | arXiv:hep-ex/0504010 §4 — **verified verbatim this session** |
| **H1 inclusive charged-hadron SIDIS** (the paper our analysis inherits, arXiv:1302.1321) | **no γ\*p cross section**: multiplicities normalized to inclusive DIS, defined in the γ\*p (HCM) rest frame, bin-by-bin HERACLES corrections | in-house `research-h1-1302-1321-charged-multiplicity.md` §2, §8; frame doc `reference-sidis-frame-gamma-star-p.md` §2, §6.1 |
| **HERMES** (identified-hadron SIDIS) | **no γ\*p cross section**: multiplicity = (1/N_DIS(Q²))·dN^h/dz — DIS-count-normalized, Born-level corrected | arXiv:1212.5407 (PRD 87 (2013) 074029), multiplicity equation — **verified verbatim this session** |
| **COMPASS** (charged-hadron TMD multiplicities) | **no γ\*p cross section**: d²M^h/dzdp_T² = (1/d²N^DIS)·d⁴N^h/dzdp_T²·(1−η^h/η^DIS)·(1/a^h)·C^h/C^DIS | arXiv:1709.07374 "Transverse-momentum-dependent Multiplicities of Charged Hadrons in Muon-Deuteron Deep Inelastic Scattering" (title verified; definition equation verified verbatim this session) |
| **CLAS / JLab fixed target** | DIS-normalized multiplicity-ratio practice (same fixed-target family); **no verified CLAS γ\*p extraction found this session** — statement labeled inferred from the shared practice, with the JLab precedent per the in-house frame doc | `reference-sidis-frame-gamma-star-p.md` (JLab precedent); **inferred, not primary-verified** |
| **Trento (Bacchetta et al.)** | structure-function normalization: dσ ∝ (α²/(xyQ²))(y²/(2(1−ε)))(1+γ²/2x){F_UU,T+εF_UU,L+…} — a γ\*p-level factorization but NOT a photoabsorption cross section in barns | arXiv:hep-ph/0611265 Eq. (7) — verified; conventions paper arXiv:hep-ph/0410050 per frame doc §2 |

**Recommendation: σ^{γ\*p} ≡ σ_T + ε̄ σ_L (the ε-weighted total), H1-house flux.** Drivers:
1. It is the only convention HERA ever published for γ\*p cross sections (H1+ZEUS VM precedent, verified above).
2. **R drops out of the central value exactly** (prescription memo §1 refines touchpoint-map risk R5): the released single-bin observable is precisely this combination; σ_T alone would require an unmeasured R at 3.4–8.5 % penalty (Q3); dividing by (1+ε) instead has no HERA precedent and silently assumes σ_L = σ_T; the "reduced σ_r" is an ep-level structure-function object with different units, not a γ\*p cross section (and MAPTMD24, our natural first consumer, is delivered at ep level — prescription memo §5).
3. It is algebraically the reduced cross section × (4π²α/Q²) (Q7), giving a clean F₂-based sanity chain and continuity with the inclusive-HERA σ^{γ\*p} ↔ F₂ practice (ZEUS very-low-Q² σ_γp extraction from F₂, arXiv:hep-ex/9905055, title verified).
4. The fixed-target SIDIS world (HERMES/COMPASS/CLAS) never quotes γ\*p cross sections at all — their DIS-normalized multiplicities are a *different* observable (and blocked for us, touchpoint-map R9); there is therefore no competing fixed-target γ\*p convention to reconcile against.

---

## Q3 — R = σ_L/σ_T treatment and the uncertainty to carry

- **Central value:** none needed — σ^{γ\*p} = σ_T + ε̄σ_L is exact per bin. **Carry no R systematic on the central value.**
- **If a consumer converts to σ_T:** σ_T = σ^{γ\*p}/(1+ε̄R) → relative penalty ε̄·δR/(1+ε̄R). With measured ε̄ per cell (0.813–0.976, S3 0.881):
  - R = 0.15 ± 0.10 → penalty **7.2 – 8.5 %** per cell;
  - R = 0.25 ± 0.05 (H1 inclusive-R precision) → penalty **3.4 – 3.9 %**.
- **Expected R magnitudes at our kinematics (in-house LT doc, reused):** inclusive R ≈ 0.2–0.3 at HERA (H1 2010 arXiv:1012.4355; H1 2014 arXiv:1312.4821; H1 fits use R ≈ 0.25 with weak Q² dependence); SIDIS-level expectations R_SIDIS ~ α_s(Q²)/π × O(1) ≈ 0.03–0.10 (pQCD/TMD), HERMES-consistent ≲ 0.05–0.10, LT-doc best estimate 0.05–0.15 with z/p_T dependence (`research-LT-separation-feasibility.md` §3.3, §5.1).
- **Recommendation:** ship the per-cell ε̄ (and ⟨y⟩, ⟨Q²⟩, ⟨x⟩) sidecar with the product (prescription memo Phase 1/§7 and risk R13); document the σ_T-conversion penalty once; do not attempt an L/T separation (LT doc: not publishable at our precision).

---

## Q4 — Beam energy and bin-averaged flux (mixed-beam question is MOOT; the real issue is per-cell y/ε averaging)

- **Single configuration, established:** e± 27.6 GeV on 920 GeV protons, fixed s = 101 568.0 GeV² (√s = 318.697 GeV). The 2007 MER (575 GeV) and LER (460 GeV) runs are **excluded** from the sample (GRL boundary + S67 trigger off during MER/LER — `docs/references/reference-highQ2-per-run-beam-energy.md`); the original brief's "575 GeV for 2007 / 46.0 GeV lepton" premise is therefore not realized in this sample (46 GeV does not correspond to any HERA lepton beam; it is consistent with a garbled "460 GeV" LER proton energy, which is itself excluded). No per-period flux split, no mixed-√s campaign, no luminosity-weighted Γ combination is needed. Per-event branches exist anyway (data `BeamElecE`/`BeamProtP` ≈ 27.611/919.125 GeV; MC gen 27.6/920 — audit doc §2, prescription memo §4); differences are 10⁻⁴–10⁻⁵ relative in s and irrelevant to Γ.
- **Residual beam-spread bound (documented, not a leg):** the data √s spread is [318.1, 319.2] GeV (`docs/references/reference-beam-energy-policy.md`). At fixed (x,Q²) this moves y by ≤ ±0.35 %, hence per-event Γ by ≤ ±0.5 % (high-y cells) and ε by ≤ ±0.3 %; the effect on the MC-weighted ⟨1/Γ⟩ sits well inside the measured level/model dependence of the weighting (+0.5–1.75 %, prescription memo §3 item 4) and is absorbed by the `flux_model_rel` treatment.
- **Luminosity context (per-period GRL, `docs/references/reference-highQ2-grl-manifest.md`):** ep0304 53 706.105, em0405 105 028.172, em06 57 573.094, ep0607 140 306.219 nb⁻¹ (sum 356 613.59 nb⁻¹ vs the release normalization L_data = 351 000.0 nb⁻¹); e+/e− split 194 012.324 / 162 601.266 nb⁻¹ (f_ep = 0.5141, f_em = 0.4859 — the beam fractions used by the EW memo's mixed-beam denominator). At the photon-Born target the lepton-charge mixture is *irrelevant* (exact tree-level charge symmetry, EW memo §2/§3.1), so these numbers are context only.
- **The real averaging problem — per-cell y and ε:** y is kinematically locked to (Q²,x) (y = Q²/(x·s)); populated y spans [0.100, 0.700] and ε spans [0.550, 0.994] across the seven cells, with in-cell Γ dynamic ranges of **10×–151×** (analytic envelope; event-weighted measured 8.9×–137×, prescription memo §3). The prescription is the per-bin, cross-section-weighted **⟨1/Γ⟩** (prescription memo §3), which makes the conversion an exact identity; quantified rejections reused from the prescription memo (and re-confirmed independently this session):
  - bin-center / effective-kinematics Γ: biases −29 % … +72 % (geometric/log centers, prescription memo §3 item 2); evaluating 1/Γ at the cell's *measured effective* (⟨Q²⟩,⟨y⟩) still leaves a Jensen gap of **+4 % … +14 %** vs ⟨1/Γ⟩ (computed this session: ⟨1/Γ⟩/1/Γ(eff) = 1.040, 1.064, 1.059, 1.068, 1.043, 1.077, 1.142 for S0_x0…S3_x0) — the event-weighted average is mandatory, not optional;
  - count-weighting instead of cross-section weighting: bias up to **+12.1 %** (S0_x1) — the R10 anti-trap gate;
  - ε̄ per cell (measured, full sample): 0.815 / 0.975 / 0.823 / 0.976 / 0.813 / 0.975 / 0.881 — ship as the sidecar.

---

## Q5 — Jacobian / differential-form question

- **The conversion is a multiplicative per-bin scalar division**, not a variable change: σ^{γ\*p}(bin) = σ^{ep}(bin)·⟨1/Γ⟩(bin). No Jacobian factor appears **provided ⟨1/Γ⟩ is computed as an average of the flux density in the same measure and binning in which the ep cross section is integrated** — which is exactly the prescription (same worker, same cell assignment, same weights).
- **Measure convention (must be stated once, frozen):** the H1 Eq. (9)–(10) factorization is in the **(y,Q²) measure**: d²σ^{ep}/dy dQ² = Γ(y,Q²)·σ^{γ\*p}. The equivalent (x,Q²)-measure flux is Γ_x = (α/2π)·Y₊/(xQ²) (derived: Γ_y = Γ_x·(x/y), matching the Jacobian |dx/dy| = x/y); the two per-bin integrated products differ by the in-bin average of x/y — an O(1) difference. The prescription memo freezes the **(y,Q²) measure** (H1-VM faithful); mixing the two is forbidden exactly like the withZ/noZ two-convention rule (touchpoint-map R1/R6).
- **W-representation:** W² = M² + Q²(1−x)/x is 1:1 with (x,Q²) at fixed Q², so the pointwise γ\*p cross section is representation-invariant; a W-binned presentation requires re-binning the ep product and recomputing ⟨1/Γ⟩ in the new binning from the same events (a re-run of the same accumulator), not a Jacobian patch on the old ⟨1/Γ⟩.
- **z, p_T are untouched** by the flux (Γ has no z/p_T dependence); the fourfold generalization is d⁴σ^{ep}/dy dQ² dz dp_T = Γ(y,Q²)·d³σ^{γ\*p}/dz dp_T (at fixed (W,Q²)), which is what the per-bin product realizes after integration over the bin.
- **Density renders** (if ever wanted): divide by the bin's exact (y,Q²,z,p_T) phase-space area — ∫∫_R (1/(x·s)) dQ² dx clipped to 0.1 < y < 0.7, times Δz·Δp_T — never by the (Q²,x) volume (prescription memo Phase 0; x_max = 1.5 cap per R6).

---

## Q6 — Uncertainty propagation and new flux-induced uncertainties

- **Propagation rule:** per-bin scalar factor f_i = C_gamma_born(i)·⟨1/Γ⟩(i) shared by all (z,p_T) bins of a (Q²,x) cell → diagonal congruence **C^{γ\*p} = F·C^{ep}·Fᵀ** (touchpoint map §4 lines 149–164; prescription memo §7). Absolute errors scale by f_i; **all relative systematic legs are unchanged** (flux cancels in every relative leg); the h+/h− charge asymmetry is unchanged (same Γ per cell both charges); IBU-stat correlations are preserved (same-cell (z,p_T) bins share the identical factor).
- **New / changed uncertainty sources, with sizes:**
  1. `flux_model_rel` — MC model/level dependence of the ⟨1/Γ⟩ weighting: measured level (RAD-mixed vs photon-Born weighting) **+0.5 … +1.75 %** (worst S3); generator dependence unmeasurable with existing tables (no y-resolved RAPGAP table, no RAPGAP NoZ sample) — anchor via the optional RG yresolved pass or a κ-style proxy (prescription memo §3 item 6, Phase 1); the ≤ ±0.5 % beam-spread effect (Q4) is absorbed here.
  2. ⟨1/Γ⟩ MC statistics — same NoZ sample as the CγB numerator (per-bin stat median 0.551 %, max 7.84 %, EW memo §4); propagate or demonstrate ≪ the CγB stat leg — **open decision** (see below).
  3. Count-weighting trap — not an uncertainty but a gate: bias up to +12.1 % if Σw is replaced by counts (prescription memo R10, anti-trap gate in Phase 1).
  4. Flux-convention (Hand vs Gilman-K vs Trento γ²-exact vs electron-mass): **≤ 0.03 %** total at our Q² (Q1) — documented bound, not a live leg. The K_H σ-normalization rescale (1/(1−x), Q1.2) is a *convention statement*, not an uncertainty.
  5. R = σ_L/σ_T: **no contribution to the central value**; conversion penalty documented (Q3).
  6. Inherited (EW memo §4, verbatim carry-over): F_EWgamma generator dependence (κ-proxy medians 0.622 %/0.619 %, SWAP not additive vs `c_rad_gen_rel`), F_EWgamma numerator MC stat (median 0.551 %, max 7.84 %, swaps `c_rad_stat_rel`), DJANGOH-only coverage gap, PROVISIONAL-DIAGNOSTIC status, P-CRAD-LUMI caveat.
- **No double counting:** C_rad (and C_gamma_born) are pure RAD/NONRAD (and EW-projection) yield ratios with no flux factor (supervisor-verified fact; EW memo §2.1); the Γ division is applied strictly after (outside) them; the flux is an α(0) object matching the α(0) photon-Born σ side by construction.

---

## Q7 — Algebraic cross-check route via the reduced cross section / F₂

One-photon, γ→0 algebra (derived this session from the standard reduced form; all steps exact):

d²σ^{ep}/dxdQ² = (2πα²/(xQ⁴))·Y₊·σ̃_r, with σ̃_r ≡ F₂ − (y²/Y₊)F_L and Y₊ = 1+(1−y)². Since ε = 2(1−y)/Y₊, one has (1−ε) = y²/Y₊ exactly, hence

- **σ^{γ\*p} = σ_T + εσ_L = (4π²α/Q²)·[F₂ − (1−ε)F_L] = (4π²α/Q²)·σ̃_r(x,Q²)** (with σ_T = (4π²α/Q²)·2xF₁, σ_L = (4π²α/Q²)·F_L);
- dividing the reduced form by this definition gives exactly **Γ = (α/2π)·Y₊/(xQ²)** in the (x,Q²) measure, i.e. **Γ = (α/π)(1−y+y²/2)/(yQ²)** in the (y,Q²) measure — *the H1 Eq. (10) flux is nothing but the reduced-cross-section factorization*, re-derived from first principles and matching the published form coefficient-for-coefficient.
- Limits and cross-checks this enables: (i) σ_T + σ_L = (4π²α/Q²)·F₂ — the classic relation used by ZEUS to extract σ^{γ\*p} from F₂ at very low Q² ("Measurement of the Proton Structure Function F2 and of the Total Photon-Proton Cross Section σ(tot) at Very Low Q2 and Very Low x", arXiv:hep-ex/9905055, title verified); (ii) any published or H1-measured F₂, F_L at our cell kinematics gives an independent γ\*p-level normalization anchor for the DIS part; (iii) target-mass corrections to the identity are the same ≤ 0.03 % γ² terms (Q1); (iv) internally, the per-bin identity σ_ep(bin)·⟨1/Γ⟩(bin) ≡ ∫_bin (1/Γ) dσ_ep holds by construction of the weighting (prescription memo §1) — the Jensen-gap diagnostic (Q4) is its practical gate.

---

## Q8 — Published target values / sanity checks; the γ\*p analogue of the 13.03/12.70 nb sums

- **No published HERA (or any-collider) γ\*p charged-hadron SIDIS cross section exists at Q² ≥ 150 GeV²** — the product would be first-of-kind; the fixed-target SIDIS world publishes DIS-normalized multiplicities instead (Q2), so there is no external like-for-like target. Published γ\*p anchors that DO exist, for scale and convention only:
  - HERA low-Q² inclusive σ^{γ\*p}(Q²) extracted from F₂ (ZEUS arXiv:hep-ex/9905055, title verified; the F₂↔σ_γp route of Q7) — μb-scale at Q² ≲ 1 GeV², W ~ 100–200 GeV;
  - H1/ZEUS exclusive VM σ^{γ\*p} (e.g. arXiv:0910.5831: nb-scale, Born level, σ_T+εσ_L convention, ε ∈ [0.91, 1.00]) — the house-precedent chain, at VM kinematics.
- **Internal anchors (reuse, prescription memo §6 / App. A, verified on the current cone basis):** RAW 11.412233 / 11.123626 nb → ×C_rad = 13.026987 / 12.697511 nb (the Born release) → ×C_gamma_born = **11.875722 / 11.571280 nb** (photon-only ep Born; integrated CγB_eff 1.0406/1.0402; F_EWgamma per-bin 0.7846–0.9906, integrated 0.9116/0.9113) → ×⟨1/Γ⟩ = **1 170 035 / 1 129 842 nb·GeV²** γ\*p prototype totals (single-file flux; ≲ 1 % shift expected from the full 260-file pass). Per-bin ⟨1/Γ⟩ spans 16 066 – 2 886 400 GeV²; per-cell full-sample ⟨1/Γ⟩: 62 982 / 22 128 / 128 576 / 47 553 / 299 933 / 108 816 / 735 729 GeV².
- **The γ\*p analogue of the 13.03/12.70 nb per-bin sums** is Σ_bins σ^{ep,γ-only Born}(bin)·⟨1/Γ⟩(bin) ≈ 1.17×10⁶ / 1.13×10⁶ nb·GeV² (prototype) — NOT (13.03 nb) divided by any single Γ: (i) the 13.026987/12.697511 nb release values are **full-NC (γ+Z) Born**; dividing them by a photon flux would mislabel the result as "(γ\*+Z)p" — the honest analogue starts from the photon-Born sidecar (EW memo §5.1); (ii) Γ varies by orders of magnitude across cells (⟨1/Γ⟩ 2.2×10⁴–7.4×10⁵ GeV² per cell), so a global division is meaningless — the sum is per-bin first, summed after; (iii) the resulting grand total is a sum of nb·GeV² objects in the (y,Q²,z,p_T) measure, interpretable per cell as ⟨n_charged-in-cuts⟩ × (γ\*p DIS cross section integrated over the cell's (y,Q²) area), with ⟨n⟩ the per-event charged-hadron multiplicity in our fiducial cuts (speculative-order-of-magnitude: 2–3 at low x, from typical charged multiplicities; a quantitative statement needs the inclusive DIS cross section in the same fiducial, which does not exist in the release — multiplicities stay blocked, touchpoint-map R9). The DIS-level comparison anchor is (4π²α/Q²)·σ̃_r (Q7), available to the supervisor from published F₂/F_L at the cell effective kinematics (⟨Q²⟩ = 197.5 … 3077.0 GeV², ⟨x⟩ from the sidecar).

---

## OPEN DECISIONS FOR THE SUPERVISOR

1. **Freeze the convention set** (literature-verified here; prescription memo Phase 0): H1-house flux Γ = (α_em(0)/π)(1−y+y²/2)/(yQ²) [arXiv:0910.5831 Eq. (10)]; σ^{γ\*p} ≡ σ_T + ε̄σ_L; (y,Q²,z,p_T) measure; pure photon-Born α(0) EW level via C_gamma_born. Record in metadata, including the K_H σ-normalization note (Hand-K_H tables rescale by 1/(1−x); up to +17 % at populated x = 0.15).
2. **⟨1/Γ⟩ MC-statistical leg:** propagate as a dedicated leg, or gate-demonstrate ≪ the CγB numerator stat leg (median 0.551 %) — pick one in Phase 1.
3. **`flux_model_rel`:** κ-style anchored leg vs optional RAPGAP yresolved pass over the existing RG RAD emfix sample (no RAPGAP NoZ sample exists; coverage gap, EW memo §3.2) — decide before Phase 1 freeze.
4. **Status inheritance:** the γ\*p derived product inherits PROVISIONAL-DIAGNOSTIC status, DJANGOH-only F_EWgamma, and the P-CRAD-LUMI caveat verbatim (EW memo §3.2) — confirm the labelling in the product README and note tables.
5. **Effective-kinematics sidecar** (⟨Q²⟩, ⟨y⟩, ⟨x⟩, ε̄, per-cell Γ spread): ship with the product (recommended YES — the only mitigation for S3's 137× in-bin spread, prescription memo R13) or as a diagnostic only.
6. **σ_T-only requests** (e.g. from a theory consumer): decline for the central product; provide ε̄ sidecar + documented conversion penalty (3.4–8.5 % at plausible R) instead.
7. **S3 presentation:** single σ^{γ\*p} number per S3 bin is exact but hard to interpret at "the" bin kinematics (Γ spread 137×; ε̄ 0.881 over y ∈ [0.1, 0.7]) — decide whether S3 gets an explicit spread statement in note-level renders.

---

## Verification log (all read-only; 2026-10-15/16)

1. **Primary fetches (ar5iv/arXiv, cached at `/tmp/opencode/*.html` — session scratch, not workspace artifacts):** arXiv:0910.5831 (H1 ρ/φ) — Eqs. (9)–(10), σ_tot = σ_T+εσ_L, ε range 0.91–1.00/⟨ε⟩=0.98, "Born level" statement, Hand bibliography entry, all extracted verbatim; arXiv:hep-ex/0504010 (ZEUS φ) — Γ_T(σ_T+εσ_L), ε = 2(1−y)/(1+(1−y)²), title; arXiv:hep-ph/0611265 (Bacchetta) — Eq. (7) prefactor, Eq. (8) ε, depolarization factors; arXiv:1212.5407 (HERMES) — DIS-count-normalized multiplicity equation; arXiv:1709.07374 (COMPASS) — title + multiplicity definition equation; arXiv:hep-ex/9905055 (ZEUS very-low-Q²) — title. Hand 1963: author/journal/page-end ("L.N. Hand., Phys. Rev. …1834") verified inside the fetched H1 bibliography; volume 129 (1963) per the standard bibliographic record (INSPIRE/Semantic Scholar lookups returned no hit / rate-limited — labeled accordingly).
2. **In-house re-verification:** the `exclu_phi/docs/research-cross-section-method.md` transcription of Eqs. (9)–(10) matches the primary source exactly; the prescription memo's per-cell y-reach, γ², Γ-envelope and Δε values are reproduced by an independent grid scan this session (`/tmp/opencode/gammap_numerics.py`, session scratch): y-reaches identical, γ²_max within grid resolution (S3 corner 3.5×10⁻⁴ analytic vs 2.6×10⁻⁴ measured), Γ envelopes 10.0×–151.5× (vs 9.0×–152×), Δε ≤ 5.0×10⁻⁶ (identical bound).
3. **Numerics computed this session:** ε endpoints 0.99448/0.55046; Q²/ν² ≡ γ² identity; (1+γ²/2x) ≤ 1+1.7×10⁻⁴; K_H rescale table; R-penalty table; Jensen gap +4 %…+14 % (⟨1/Γ⟩ vs 1/Γ at effective kinematics).
4. **Consistency with sibling memos:** EW level (pure photon Born, α(0), C_gamma_born = C_rad × F_EWgamma), per-bin ⟨1/Γ⟩ prescription, prototype totals, ε̄/⟨y⟩/⟨Q²⟩ sidecar values, and the count-weighting/bin-center rejections are quoted from and attributed to the prescription and EW memos; nothing contradicts them. The original brief's mixed-beam premise (575/460/46 GeV) is disproven by in-house evidence and handled as moot (Q4).
5. No existing file was modified; no code run against analysis trees (the only computation is standalone arithmetic in `/tmp/opencode`); this document is the single artifact written.

## Primary references

- H1 Collaboration (F.D. Aaron et al.), "Diffractive Electroproduction of ρ and φ Mesons at HERA", JHEP 1005 (2010) 032, **arXiv:0910.5831** — §5.1 Eqs. (9)–(10), σ_tot/ε definitions, Born-level statement (VERIFIED verbatim).
- ZEUS Collaboration (S. Chekanov et al.), "Exclusive electroproduction of φ mesons at HERA", Nucl. Phys. B 718 (2005) 3, **arXiv:hep-ex/0504010** — §4 flux/ε (VERIFIED verbatim; journal ref per in-house `exclu_phi/docs/research/phi_h1_process_spec.md`).
- M. Bacchetta et al., JHEP 0702 (2005) 093, **arXiv:hep-ph/0611265** — Eqs. (7)–(8) (VERIFIED verbatim); Trento conventions: arXiv:hep-ph/0410050 (per in-house frame doc).
- L.N. Hand, Phys. Rev. **129** (1963) **1834** — original K = (W²−M²)/(2M) flux convention (cited as such by arXiv:0910.5831; entry verified in fetched bibliography, volume/year per standard record).
- HERMES Collaboration (A. Airapetian et al.), PRD 87 (2013) 074029, **arXiv:1212.5407** — DIS-normalized multiplicities (VERIFIED verbatim).
- COMPASS Collaboration, "Transverse-momentum-dependent Multiplicities of Charged Hadrons in Muon-Deuteron Deep Inelastic Scattering", **arXiv:1709.07374** — DIS-normalized multiplicities (title + definition VERIFIED).
- ZEUS Collaboration, "Measurement of the Proton Structure Function F2 and of the Total Photon-Proton Cross Section σ(tot) at Very Low Q2 and Very Low x", **arXiv:hep-ex/9905055** (title VERIFIED) — the F₂ ↔ σ^{γ\*p} extraction precedent.
- H1, arXiv:1012.4355 (2010) and arXiv:1312.4821 (2014) — inclusive F_L / R ≈ 0.2–0.3 at HERA (as cited and quoted in-house by `research-LT-separation-feasibility.md` §3.3).
- H1, arXiv:1302.1321 — the inclusive charged-hadron multiplicity paper our analysis inherits (DIS-normalized multiplicity convention; in-house research doc §2, §8).
