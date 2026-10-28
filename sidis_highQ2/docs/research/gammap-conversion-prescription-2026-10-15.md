# Design memo — converting the high-Q² charged-hadron SIDIS ep cross-section release into a σ(γ\*p) virtual-photon–proton release

> **USER DECISION 2026-10-16: this γ*p program is OPTIONAL auxiliary material — reporting level is the ep cross section; the pending items below (§10 rebuild, §11 M2 fold, builder phases) are NOT blockers and NOT required. See `docs/methodology/ep_reporting_level_user_decision_2026-10-16.md`.**


**Date:** 2026-10-15 · **Type:** read-only design prescription (no release, script, or note was modified; the only file created by this task is this memo) · **Regime:** high-Q² charged-hadron SIDIS, Q² > 150 GeV², no species PID.

**Question:** can the current ep cross-section release be converted into a σ(γ\*p) release, with which exact formulas, and is it quantitatively feasible with what already exists?

**Answer in one line:** yes — as a **post-unfold derived sidecar** σ_γ\*p(bin) = RAW(bin) × C_gamma_born(bin) × ⟨1/Γ⟩(bin), with the H1-house Hand flux, a per-bin MC-derived cross-section-weighted effective flux, **no y-binning and no re-unfolding**; the arithmetic is verified end-to-end on the current cone basis in this memo.

**Basis (current central, cone triad):**
- Raw release: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ 11.412233 / h− 11.123626 nb).
- Born release: `.../data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ 13.026987 / h− 12.697511 nb).
- Canonical budget: `.../data_release/systematics_cone_s2prime_15comp_shiftedlegs_legenv_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-14/` (medians h+ 5.8032 % / h− 6.1919 %).
- Architecture input (consumed, not duplicated): `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/methodology/map_ep_xsec_pipeline_touchpoints_2026-10-15.md` (producers/consumers/covariance/Option-B derived-sidecar design/risks R1–R9) and `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/audits/audit_gammaflux_inputs_2026-10-15.md` (branch audit; verdict: per-event Γ computable, no builder change).
- Method note: the `sidis-hq2-*` skills are not loadable in this runtime; all statements below are taken directly from the authoritative workspace documents and files cited, plus a single-file read-only numeric prototype run today on the NAF login node (within the ≤1-file/≤5-min policy). Scratch output of the prototype lives outside the workspace at `/tmp/opencode/gammap_flux_prototype_2026-10-15.json` (session scratch, not a workspace artifact).

---

## 1. The ep → γ\*p conversion relation (Q1)

**Published H1 house convention** (exclusive VM electroproduction, the only H1-published σ(γ\*p) extraction chain this workspace already documents): H1 Collaboration, F.D. Aaron et al., "Diffractive Electroproduction of ρ and φ Mesons at HERA", JHEP 1005 (2010) 032, **arXiv:0910.5831**, §5.1 p. 30, Eqs. (9)–(10), transcribed with the exact formula in `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research-cross-section-method.md` lines 18–38:

```
sigma(gamma*p) = (1/Gamma) * d^2 sigma(ep) / (dy dQ^2)
Gamma = (alpha_em / pi) * (1 - y + y^2/2) / (y * Q^2)        [1/GeV^2]
```

with the γ\*p cross section defined as the ε-weighted total (ibid. lines 46–61):

```
sigma_gamma*p = sigma_T + eps * sigma_L = sigma_T * (1 + eps * R),   eps = (1-y)/(1 - y + y^2/2)
```

ZEUS uses the identical convention (S. Chekanov et al., Nucl. Phys. B 718 (2005) 3–31, arXiv:hep-ex/0504010, §4; ε = 2(1−y)/(1+(1−y)²) ≡ (1−y)/(1−y+y²/2)) — documented in `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research/phi_h1_process_spec.md` lines 397–428 and 689–694.

**SIDIS 4-D generalization (the release formula).** The released product is the bin-integrated ep cross section in 490 bins of (Q², x, z, p_T,h) — `cross_section_nb = N_Unf/L_data` (driver `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_2026-10-11/run_bbb_ibu_release_cone_s2prime.py`, `LUMI=351000.0` line 195, `cross_section_nb = vec[i]/LUMI` line 628; note-level definition `dσ/dx dQ² dz dp_T,h = N_Unf·c_rad/(L·ΔxΔQ²ΔzΔp_T,h)` at `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/observables.tex` lines 8–12). At the event level the one-photon-exchange factorization is

```
d sigma_ep = Gamma(y, Q^2) * d sigma_gamma*p        (per dy dQ^2 dz dpT, at fixed Q^2, x)
```

so, integrating over a bin region R and dividing by Γ pointwise:

```
sigma_gamma*p(bin) = INT_R (1/Gamma) d sigma_ep  =  sigma_ep^{gamma-only,Born}(bin) * <1/Gamma>_bin     [EXACT identity]
<1/Gamma>_bin = SUM_i w_i / Gamma(y_i, Q2_i)  /  SUM_i w_i        [ep-cross-section-weighted; MC gen level]
```

σ_γ\*p(bin) is then the γ\*p photoabsorption cross section σ_γ\*p(z, p_T; Q², x) integrated over the bin region **in the (y, Q², z, p_T) measure** (the measure in which the H1 VM formula divides by Γ); units nb·GeV². Two consequences:

- **The effective-flux route is not an approximation.** ⟨1/Γ⟩ as defined above makes the conversion an identity under the factorization; the only residual systematics are the MC model/level of the weighting sample (quantified in §3) and the convention (§2).
- **Measure convention must be stated once and frozen.** A "(Q²,x)-measure" variant (factor ⟨x/(yΓ)⟩, i.e. integrating σ_γ\*p against dQ² dx instead of dy dQ²) is definable from the same pass but differs by the in-bin average of x/y — an O(1) difference. This memo prescribes the **(y, Q²) measure** (H1-VM-faithful, factor ⟨1/Γ⟩) and forbids mixing the two, exactly like the existing withZ/noZ two-conventions rule (`sections/appendix_cross_section_tables.tex`, "must not be mixed"; extended by touchpoint-map risk R1).

**Does a single flux suffice, or is σ_ep = Γ_T σ_T + Γ_L σ_L needed?** A single **transverse** flux suffices: the longitudinal flux is suppressed by Q²/ν² ≤ 2.6×10⁻⁴ over our whole phase space (measured maximum in the prototype, §3), so Γ_L is negligible and the longitudinal contribution is carried *inside* σ_γ\*p as the ε-weighted term ε·σ_L — the standard H1/ZEUS treatment. Writing σ_ep = Γ_T(σ_T + εσ_L) + Γ_L σ_L would change nothing at our precision and is not the house convention.

**Reduced-cross-section / R route?** The Trento-conventions form (Bacchetta et al., arXiv:hep-ph/0611265, Eq. (7): dσ = (α²/(xyQ²))·(y²/(2(1−ε)))·(1+γ²/2x)·{F_UU,T + εF_UU,L + azimuthal}, ε per Eq. (8)) defines a **different observable**: TMD structure functions in the Bacchetta normalization, not a photoabsorption σ(γ\*p) in nb. It is legitimate for theory-facing TMD comparisons but is not the H1 γ\*p house product, and MAPTMD24 is delivered in ep nb/GeV³ (see §5), so the Hand route keeps every comparison level-consistent. Not recommended here.

**What is extractable given no y binning and no F_L separation:** exactly one quantity per bin — the **ε-weighted total σ_γ\*p = σ_T + ε̄σ_L** at the bin's effective kinematics. NOT extractable: σ_T alone (would need R = σ_L/σ_T, unmeasured), σ_L/F_L (needs a y-leveraged double-angle/LT separation; y is kinematically locked to (Q²,x) — cell y-ranges in §3 — and the released binning has no y dimension), and any y-resolved γ\*p structure. Note the important corollary: because the quoted quantity is the ε-weighted **sum**, **R drops out of the central value entirely**; R enters only if someone later converts to σ_T. (This refines touchpoint-map risk R5: for the total-γ\*p convention, R is not a central-value systematic.)

---

## 2. Flux convention options and recommendation (Q2)

**Option A — Hand / H1-house (recommended).** Γ(Q², y) = (α_em/π)·(1 − y + y²/2)/(y·Q²), algebraically identical to (α/2π)·(1+(1−y)²)/(y·Q²) since (1+(1−y)²)/2 = 1−y+y²/2. This is what H1 published for σ(γ\*p) (arXiv:0910.5831 Eq. 10) and what ZEUS used (arXiv:hep-ex/0504010 §4); it is the massless transverse Weizsäcker–Williams splitting flux. Use α_em = α(0) = 1/137.036 (on-shell coupling; the hard-scattering α convention lives inside the MC/theory, not in the flux).

**Option B — exact-Hand with electron-mass term.** Hand's exact massive form adds terms O(m_e²/Q², m_e²/E_e²); the H1-published simplified form omits them deliberately because they are "(relevant only for near-real photoproduction)" — `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research-cross-section-method.md` lines 40–44. At Q² ≥ 150 GeV² and E_e ≈ 27.6 GeV these are ≤ 10⁻⁹. **Numerically indistinguishable from Option A here.**

**Option C — Gilman / other K-factor conventions.** All standard transverse-flux conventions (Hand, Gilman, Williams) differ by O(γ²) and O(Q²/ν²) factors. Measured maxima over our seven cells (prototype, §3): γ² = 4M²x²/Q² ≤ 2.6×10⁻⁴ (worst: S3), Q²/ν² ≤ 2.6×10⁻⁴, |ε_H1 − ε_Trento| ≤ 5×10⁻⁶. **All convention differences are ≤ ~0.03 % in our range** — two to three orders below the 5.8–6.2 % budget. (The "~3–5 %" flux-convention systematic quoted for exclusive φ at `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research/diffvm_phi_normalization_audit.md` line 291 belongs to the low-Q² φ regime where Q²/ν² is large; it does not transfer to Q² > 150 GeV².)

**Option D — Trento-exact coefficient (Bacchetta Eq. 7).** Defines σ_γ\*p in structure-function normalization (different units/normalization of the quoted γ\*p object), not a photoabsorption cross section. Rejected as the release convention (see §1); the γ²-exactness it buys is ≤ 0.03 % here.

**Recommendation: Option A**, recorded in the product metadata as `flux_convention: "Hand/H1 arXiv:0910.5831 Eq. 10"`, with the numeric convention-difference bound (≤0.03 %) documented once in the flux-table README rather than carried as a live systematic leg.

---

## 3. Per-published-bin EFFECTIVE flux (Q3)

**Prescription.** For each of the 490 release bins, compute

```
<1/Gamma>(bin) = SUM_i w_i / Gamma(Y_es_gen_i, Q2_es_gen_i) / SUM_i w_i      [GeV^2]
```

over the gen-bin population of the **DJANGOH14 NoZ photon-only pure-Born e⁻ truth sample** (the C_gamma_born numerator sample: 260 `ChargedGenRC` trees under `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/`), with the selection mirroring `crad_v3_worker_releasebin.py` EXACTLY (weights `weight1×weight2`, DIS fiducial on `Q2_es_gen/Y_es_gen/x_es_gen` with 150 ≤ Q² < 20000 and 0.1 < y < 0.7, `GenEmpz > 45`, π/K/p-only hadron mask, recomputed xF < 0, cell assignment via `Q2esGen/x_es_gen`, y-bins via `Y_es_gen` — `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py` lines 118–142). Why this sample: the σ_ep being divided is **photon-only Born** (level-matched weighting), and at γ-only Born tree level e⁻/e⁺ are charge-symmetric, so the e⁻ truth sample validly weights the mixed-beam data.

**Why the effective flux and not bin-center Γ — quantified.** Γ varies enormously inside every cell because y is locked to (Q², x) by y = Q²/(x·s) and every cell spans a factor ≥2 in Q²:

| cell | Q² range | x range | y reach (analytic) | event-weighted Γ max/min | ⟨1/Γ⟩ [GeV²] |
|---|---|---|---|---|---|
| S0_x0 | 150–300 | 0.0021–0.0071 | [0.208, 0.700] | 8.9× | 63 297 |
| S0_x1 | 150–300 | 0.0071–0.030 | [0.100, 0.416] | 9.9× | 22 024 |
| S1_x0 | 300–700 | 0.0042–0.016 | [0.185, 0.700] | 12.1× | 128 792 |
| S1_x1 | 300–700 | 0.016–0.069 | [0.100, 0.431] | 12.3× | 47 753 |
| S2_x0 | 700–1500 | 0.0099–0.034 | [0.203, 0.700] | 9.7× | 301 943 |
| S2_x1 | 700–1500 | 0.034–0.150 | [0.100, 0.434] | 11.2× | 107 719 |
| S3_x0 | 1500–20000 | 0.021–1.5 | [0.100, 0.700] | 137× | 734 613 |

(Prototype values, one file; full-sample cross-section-weighted values agree to ≤1.0 % — see below. Effective kinematics per cell: ⟨Q²⟩ = 197.5/214.6/412.1/468.2/935.2/1035.5/3077.0 GeV², ⟨y⟩ = 0.460/0.187/0.447/0.184/0.461/0.188/0.347, ε̄ = 0.815/0.975/0.823/0.976/0.813/0.975/0.881; ε spans [0.55, 0.99] over the full fiducial y range.)

Consequences, all measured in today's prototype:

1. **Bins with in-bin flux spread > 10 %: effectively all of them.** Cell-level event-weighted Γ spreads are 8.9×–137×; per-bin measured spreads (lower bound, single file): 483 of 488 populated bins > 10 %, 474 > 2×, median 8.7×. The two bins empty in the single file (global 209, 453) are populated in the full sample (all 490 bins are finite in `cgammaborn_empz_gt45_nominal_490rows.csv`). **Answer: 490/490 for practical purposes.**
2. **The analytic bin-center fallback is not viable.** 1/Γ(bin-center) vs event-weighted ⟨1/Γ⟩ differs by **−14 % to +72 %** (geometric centers; per cell: −13.9, +69.0, −22.5, +70.6, −18.8, +71.8, −0.7 %) and **−29 % to +56 %** (log centers; −22.4, +46.1, −28.7, +55.7, −25.2, +50.2, −23.9 %) — large, cell-dependent and uncontrolled (S3's near-zero geometric bias is an accident of its huge one-sided x-range, where the geometric center x = 0.76 is far from the populated ⟨x⟩ = 0.11). The touchpoint-map risk R4 ("analytic bin-center-y table is the cheaper fallback") is hereby **quantified out**: the MC-integrated table is mandatory, not optional.
3. **The weighting must be cross-section-weighted (Σw), never count-weighted.** The NoZ sample is strongly reweighted: count-weighting biases ⟨1/Γ⟩ by up to **+12.1 %** (cell S0_x1). This is the single biggest trap in the prescription.
4. **Level dependence of the weighting (RAD vs Born): small.** Reweighting the same per-(cell,y) ⟨1/Γ⟩ with the full-sample y-shapes from `cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` (columns `sigma_gamma_born` vs `sigma_rad_mixed`): RAD-mixed weighting shifts ⟨1/Γ⟩ by only **+0.5 % to +1.75 %** (worst S3) relative to photon-Born weighting. Using the level-matched Born sample is correct and worth the ~1 %, but the result is robust against this choice.
5. **Single-file representativeness: adequate for design, not for the product.** One-file w-weighted per-cell ⟨1/Γ⟩ vs full-sample (260-file) w-weighted: ≤ 1.0 % per cell. Full-sample values: 62 982 / 22 128 / 128 576 / 47 553 / 299 933 / 108 816 / 735 729 GeV². The product table must come from the full 260-file Condor pass (Phase 1 below).
6. **Generator (model) dependence of the y-weighting: not yet measurable from existing tables.** There is **no** y-resolved RAPGAP table (only 490-row: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/` contains `crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` and two gen-unc CSVs, no `*yresolved*`), and no RAPGAP NoZ sample exists at all (`README_NOZBORN_RELEASEBIN.md` line 13). Interim bound: the level dependence (≤1.75 %) plus the DJ/RG C_rad spread already carried as `c_rad_gen_rel`. Phase 1 optionally runs the existing yresolved worker over the RG RAD emfix sample to make this a measured leg.

**Explicit y binning instead?** Rejected. y is not a free dimension here — at fixed (Q², x) it is determined, with cell-specific reaches (table above); slicing the 7 cells into the 6 campaign y-bins would create up to 42 (Q²,x,y) cells of which many are kinematically empty (e.g. S0_x0 has no events below y = 0.208), would fragment the 490-bin product, and — decisively — would require a **full re-unfolding chain** (new response, prior, budget, covariance toys) for zero physics gain, since the effective-flux identity already converts each released bin exactly. The y-resolved infrastructure that exists (`YED = [.1,.2,.3,.4,.5,.6,.7]`, 6×490 tables) is used *diagnostically* (item 4 above), not as the binning.

---

## 4. Exact tree/branch names carrying the lepton kinematics (Q4)

All verified read-only; schema audit `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/audits/audit_gammaflux_inputs_2026-10-15.md` §2 (lines 20–36) plus builder source `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C`.

**For the flux-table pass (the prescription's input) — tree `ChargedGenRC`** (gen-only campaign trees; the C_rad/C_gamma_born machinery already runs on them). Required branch list (worker line 41, `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py`):

- weights: `weight1`, `weight2`
- rad-safe gen eΣ kinematics (flux inputs): `Q2_es_gen`, `Y_es_gen`, `x_es_gen`
- eΣ-native gen kinematics (cell assignment): `Q2esGen`, `YesGen`, `XesGen`; also `Q2sGen`, `YsGen`, `Q2Gki`
- gen beam energies (per-event s = 4·`GenBeamElecE`·`GenBeamProtP`): `GenBeamElecE`, `GenBeamProtP`
- gen electron four-vector (alternative q constructions): `GenElecPx`, `GenElecPy`, `GenElecPz`, `GenElecPhi`
- event fiducial: `GenEmpz`
- gen hadron (binning + mask): `GenPx`, `GenPy`, `GenPz`, `GenE`, `GenPDG`, `GenCharge`, `IsGtr`, `Mother2`, `IsV0DaughterGen_v3`, `VetoAncestorPDG_v3`

Branch-choice sensitivity measured: evaluating Γ at (`Y_es_gen`, `Q2_es_gen`) vs (`YesGen`, `Q2esGen`) changes ⟨1/Γ⟩ by ≤ 0.014 % (pure-Born sample, no radiation) — either is fine; prescribe the rad-safe triple for consistency with the fiducial definition.

**In the central analysis trees** (for any future y-sidecar or cross-checks; audit §2 with builder line numbers):
- `ChargedTot` (reco, per hadron): binning triple `Q2es`/`Yes`/`Xes`; alternatives `Q2e/Ye/Xe`, `Q2s/Ys/xBj`, `Q2da/Yda/Xda`, `Q2h/Yh/Xh`; `Ws`, `Empz`; per-event beam energies `BeamElecE`, `BeamProtP` (builder lines 1122–1125); scattered-electron four-vector `scatEX/scatEY/scatEZ/scatEE`; MC-only gen mirrors `Q2_es_gen/Y_es_gen/x_es_gen`, `GenElecTheta/GenElecPhi/GenElecE`, `GenEnPhoton` (builder lines 799–801, 808–810); `weight1`, `weight2`.
- `ChargedTotGen` (gen, per gen hadron; 60 branches): `Q2esGen/YesGen/XesGen`, `Q2_es_gen/Y_es_gen/x_es_gen` (builder lines 835–837, 846–848), `GenBeamElecE`, `GenBeamProtP`, `GenScatEX/GenScatEY/GenScatEZ/GenScatEE`, `GenElecPx/GenElecPy/GenElecPz/GenElecE/GenElecTheta/GenElecPhi`, `GenEmpz`, `weight1`, `weight2`.
- `DisTot` gen kinematics exist in the builder (lines 948–950) but the tree is detached in v6 (audit line 36).

**Beam energies:** data ≈ 27.611 / 919.125 GeV (per event via `BeamElecE`/`BeamProtP`), MC gen 27.6 / 920 GeV → s ≈ 101 568 GeV²; differences are 10⁻⁵-level and irrelevant to Γ. No fixed-s assumption is needed anywhere — use the per-event branches. **The release tables carry no y, ε, W or s columns at all** (audit §1, lines 13–18), which is precisely why the per-bin Γ must be MC-derived (§3).

Single-file schema/numeric verification performed today: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/nozborn_em_0000.root` — `ChargedGenRC` present, all REQUIRED branches present, 777 094 selected hadron entries under the mirrored selection (per-file rate consistent with the full-sample 200 M entries in the yresolved CSV).

---

## 5. Converting the five predictions (Q5)

**Universal rule:** every ep-level, photon-only-Born prediction in the same 490 bins converts with the **same per-bin factor** ⟨1/Γ⟩(bin) as the data — the factorization identity holds for MC exactly as for data, so no per-generator machinery is needed beyond level bookkeeping.

1. **PYTHIA8.317, SHERPA3.0, RAPGAP31 NORAD, DJANGOH14 NORAD** (the four generator curves of the comparison package `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/`): these are ep-level **Born** curves. Before dividing by Γ they must be at **photon-only** level: apply the per-bin EW factor **F_EWgamma = C_gamma_born/C_rad_mixed_published** (a column of `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_490rows.csv`; per-bin range 0.785–0.991, integrated 0.9116 (h+)/0.9113 (h−) on the current basis). **GATE:** the Z-content of the PYTHIA8 and SHERPA3 curves must be verified first (the package already carries a known ~9 % level inconsistency: data side photon-Born vs MC full-Born NORAD — touchpoint map §3 item 3); DJANGOH/RAPGAP NORAD are γ+Z Born and take F_EWgamma directly. Then ×⟨1/Γ⟩(bin).
2. **MAPTMD24** (`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/MAPTMD/MAPTMD24_H1.csv`): **does NOT already publish γ\*p** — it is ep-level, photon-only Born, confirmed numerically earlier in this analysis cycle (MAPTMD bin (S0_x0, z[0.05,0.1], p_T[0,0.5]) = 9.97–12.0 nb/GeV³ vs the NoZ photon-Born data sidecar 11.25 nb/GeV³ in the same bin). It therefore converts by ×⟨1/Γ⟩(bin) with **no** F_EWgamma step, and the γ\*p product gives MAPTMD24 a level-consistent data side for the first time (touchpoint map §5 pros). Remaining MAPTMD caveats carry over: 230/490 bin coverage, full-grid provider CSV still open (`docs/RUN_STATUS.md`).

---

## 6. Radiative / electroweak treatment (Q6)

- **Does γ\*p require the photon-only (F_EWgamma) level? Yes, by definition.** σ(γ\*p) is the virtual-**photon**–proton cross section; dividing a γ+Z Born ep cross section by Γ would leave an ~9 % Z contamination and mislabel the result. The natural base is therefore **C_gamma_born**, which already delivers the photon-only ep Born level in one step: σ_γ\*p(bin) = RAW(bin) × C_gamma_born(bin) × ⟨1/Γ⟩(bin). Equivalently, starting from the withZ Born release, apply the per-bin F_EWgamma column first. (Do **not** confuse "photon-exchange-only ep Born" with "γ\*p reduced" — touchpoint map R3(ii): C_gamma_born is NOT a flux.)
- **Is pure Born QED the right level? Yes.** The flux factorization is a tree-level statement about the ep Born cross section; the chain already corrects RAW → Born via C_rad (withZ) or C_gamma_born (photon-only), both defined against the same mixed-beam RAD denominator (`README_NOZBORN_RELEASEBIN.md` lines 5–9). The flux division commutes with both (per-bin multiplicative ep-level factors, no double-counting — touchpoint map R3).
- **Absorbed-virtual / soft-photon conventions:** no additional convention choice is required beyond the already-frozen C_rad/C_gamma_born convention (HERACLES-style σ_nonrad/σ_rad, xFGen<0, GenEmpz>45, release-safe binning). The γ\*p product **inherits** every C_gamma_born caveat and must say so: mixed-beam denominator vs e⁻ numerator, DJANGOH-only (no RAPGAP NoZ sample exists — README line 13), **PROVISIONAL DIAGNOSTIC** status (`c_rad_applied=false`), P-CRAD-LUMI not publication-grade.
- **Numbers on the current cone basis (verified today):** RAW 11.412233/11.123626 nb → ×C_rad = 13.026987/12.697511 nb (reproduces the Born release exactly — bin-alignment gate) → ×C_gamma_born = **11.875722/11.571280 nb** photon-only ep Born (integrated CγB_eff 1.0406/1.0402; the 11.91/11.61 nb quoted in the touchpoint map are the superseded no-cone S2′-basis values) → ×⟨1/Γ⟩ = **1 170 035 / 1 129 842 nb·GeV²** γ\*p prototype totals (single-file flux; expect ≲1 % shift from the full-sample flux pass). σ-weighted mean ⟨1/Γ⟩ over the noZ cross section: 98 523 / 97 642 GeV²; per-bin prototype ⟨1/Γ⟩ spans 16 066 – 2 886 400 GeV².

---

## 7. Phased execution plan (Q7)

Architecture = touchpoint-map **Option B** (derived sidecar; Option A "edit the release driver in place" is rejected there for exactly the right reasons — protected releases, gate integrity, level-flag hygiene). No re-unfolding anywhere: this is a post-unfold per-gen-bin rescaling; covariance is the documented diagonal congruence `Cov_γp[i][j] = (1/Γ_i)·Cov_ep[i][j]·(1/Γ_j)` (touchpoint map §4, lines 149–164); the 15-component relative budget is unchanged (flux cancels in every relative leg); charge asymmetry is unchanged (flux cancels).

**Phase 0 — freeze conventions (this memo).** Parent basis: the current cone RAW release (2026-10-13). Observable: σ_γ\*p = σ_T + ε̄σ_L, Hand/H1 flux, (y,Q²) measure, binned nb·GeV² product. If a density render is ever wanted, divide by the bin's exact (y,Q²,z,p_T) phase-space area — ∫∫_R Q²/(x²·s) dx dQ² clipped to 0.1 < y < 0.7 and x < 1, times Δz·Δp_T (the resulting 4D density is in nb/GeV) — never by the (Q²,x) volume (that would silently change measure convention and re-open risk R6). **[ERRATUM 2026-10-15, back-ported per QA remediation: the original Phase-0 text wrote the area integrand as ∫∫_R (1/(x·s)) dQ² dx — that is the dimensionless (x,y) area, not a GeV² area. The correct (y,Q²)-measure Jacobian is Q²/(x²·s) (= y/x; equivalently dx = (x²·s/Q²)·dy at fixed Q²), and the clip includes the physical x < 1 bound in addition to 0.1 < y < 0.7. Verified: S0_x0 A = 58.1986 GeV² and S3 A = 10623.04 GeV² are reproduced both by the direct (y,Q²) image and by the closed form; the builder (`build_gammap_release.py`, `yq2_area_clipped`) and the release `unit_statement` already carried the corrected form, so this edit restores memo/code agreement. Also note for Phase 2: the covariance congruence recipe below (C'_i = C_gamma_born(bin_i)/(L_data·Γ_i)) is authoritative — the 2026-10-15 build initially omitted the 1/L_data factor and was rebuilt in place per the QA remediation record in `scripts/gammap_2026-10-15/README.md`.]**

**Phase 1 — flux-factor table (Condor; mechanics to `h1-script-runner`/`h1-naf-operator`).** Extend `crad_v3_worker_releasebin.py` with one extra accumulator (`sum_w_invGamma` per global bin; optionally per-(bin,y) for the diagnostic sidecar — the y-resolved array structure already exists, worker lines 107–112) and run over the 260 `nozborn/em` files; optionally the same over the RG RAD emfix sample for the model-dependence leg. Emit `sidis_highQ2/data/flux/hand_gammap_flux_releasebin_<date>/gamma_flux_hand_490rows.csv` with 1/Γ per bin, per-cell ε̄/⟨y⟩/⟨Q²⟩/⟨x⟩ sidecar, sha256, README (convention, sample, selection, α, measure), gates:
- cutflow on file 0 identical to today's prototype (777 094 selected entries; selection mirrors the CγB campaign);
- all 490 ⟨1/Γ⟩ finite, positive;
- per-cell ⟨1/Γ⟩ within 1 % of the prototype values (§3 item 5);
- count-weighted variant computed and rejected by gate (bias up to +12.1 % — the anti-trap gate);
- optional RG leg: |⟨1/Γ⟩_RG − ⟨1/Γ⟩_DJ| folded into a `flux_model_rel` systematic leg.

**Phase 2 — derived builder (`h1-builder-engineer`-pattern code, new dated category dir).** `sidis_highQ2/scripts/cross_section/gammap_derived_<date>/build_gammap_derived.py`, copying the derived-sidecar precedent `build_data_side_noz_photonborn_2026-10-13.py` (per-bin factor × RAW, all `*_nb` columns scaled, caveat replaced, rest byte-identical, integrity/total gates — touchpoint map §3 item 3 and §5 lines 173–182). Factor = C_gamma_born(bin) × ⟨1/Γ⟩(bin) read from the Phase-1 table + the existing CγB table; round_trip exact-text; yields byte-identical; metadata keys: `level: "gamma*p reduced (derived)"`, `parent_release`+sha, `flux_table_path/sha256`, `flux_convention: "Hand/H1 arXiv:0910.5831 Eq.10"`, `R_treatment: "epsilon-weighted total (sigma_T + eps sigma_L); no F_L separation; R not needed for central value"`, `y_convention`, `measure: "(y,Q2,z,pT)"`, extended `born_covariance_recipe` (`C'_i = C_gamma_born(bin_i)/(L_data·Γ_i)`), `raw_yield_vs_gammap_cross_section_warning` (CHOICE-A analogue). Output directory **outside** `output_v6/central/data_release/`. Smoke gates: ΣRAW×C_rad == Born totals (13.026987/12.697511, exact); ΣRAW×CγB == 11.875722/11.571280; γ\*p totals within ~1 % of 1 170 035/1 129 842 nb·GeV².

**Phase 3 — consumers.**
- LaTeX tables: new `gammap` mode in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/tables/latex_cross_section_tables_2026-10-09/generate_latex_cross_section_tables.py` — config edit + new factor column + re-derived expected integrals; register the new sidecars in `inputs.previous_product` (touchpoint map §2.3). Binned form (nb·GeV²) recommended, sidestepping R6.
- Comparison package: rerun with `--release-dir` at the derived dir after the generator Z-content gate (§5); MAPTMD overlay becomes level-consistent.
- 7-panel/x-Q² renders: decide parent-basis registration first (the ep cone render itself is PARKED — touchpoint map §2.2/§5 table); any γ\*p render must state the measure and the effective kinematics sidecar (especially for S3, whose in-bin Γ spread is 137×).
- Analysis note: extend `sections/observables.tex` (add the γ\*p definition alongside lines 8–12), `sections/results.tex`, `sections/systematics.tex` (new legs), and the appendix "two conventions must not be mixed" sentence → three (then four with the measure) — touchpoint map R1.

**New systematic legs:** (i) `flux_model_rel` — MC model/level dependence of the ⟨1/Γ⟩ weighting (measured level effect ≤1.75 %; RG leg optional per Phase 1); (ii) y-convention — resolved by construction (bin-center route rejected with numbers; the leg collapses into (i) plus a documented convention statement); (iii) flux-convention — ≤0.03 %, documented, not a live leg; (iv) inherited CγB provisional status and F_EWgamma generator dependence (DJANGOH-only) — carried as caveats plus the existing `c_rad_gen_rel` leg. Absolute normalization stays provisional (touchpoint map R8); multiplicities remain blocked and are NOT created by this product (R9).

**Artifacts NOT touched:** release tables, response matrices, unfolding, budget chain, covariance toys, charge asymmetry.

---

## 8. Risks — what would make γ\*p NOT defensible, and cheapest mitigations (Q8)

Inherits touchpoint-map R1–R9 (level-flag collision; parent-basis fragmentation; C_rad/flux interaction; no-y-in-release-tables; R-treatment; x_max=1.5 volume split; CHOICE-A yield relation; provisional normalization; multiplicities blocked). Additions from this study:

- **R10 — count-weighting trap.** Weighting ⟨1/Γ⟩ by event counts instead of cross-section weights biases central values up to +12.1 %. Mitigation: Σw-formula frozen in the worker extension + the Phase-1 anti-trap gate. (Cheapest and highest-value mitigation in the whole plan.)
- **R11 — single-file/representativeness.** A one-file or bin-center shortcut would inject −22 %…+72 % (bin-center) or ~1 % (single-file) errors. Mitigation: full 260-file pass mandatory; prototype numbers recorded as gates.
- **R12 — generator Z-content.** If PYTHIA8/SHERPA3 curve levels cannot be established, γ\*p generator comparisons are indefensible for those two. Mitigation: verify EW mode first (gate in §5); compare MAPTMD24 + DJ/RG NORAD×F_EWgamma meanwhile.
- **R13 — interpretive weakness in S3.** With a 137× in-bin Γ spread, a single σ_γ\*p number per S3 bin is exact but hard to *interpret* at "the" bin kinematics. Mitigation: ship the per-bin/per-cell effective-kinematics sidecar (⟨Q²⟩, ⟨y⟩, ⟨x⟩, ε̄) and consider presenting S3 with an explicit spread statement; the observable itself remains well-defined.
- **Hard blockers (would stop the product):** (a) if C_gamma_born's PROVISIONAL status is judged disqualifying for a frozen derived product — cheapest mitigation: label the γ\*p product PROVISIONAL-DIAGNOSTIC identically to its parent, or fall back to quoting the withZ Born ÷ Γ strictly as "γ\*/Z\*p reduced" (a different, honestly-named object); (b) if the noZ level cannot be rebuilt on the cone RAW basis — already disproven today (arithmetic verified on the cone basis, §6); (c) if the flux table cannot pass its gates — the prototype indicates it will.

No hard blocker exists today. The product is feasible, exact under stated conventions, and quantitatively anchored.

---

## RECOMMENDATION

RECOMMENDATION: proceed with the H1-house Hand flux Γ = (α_em/π)(1−y+y²/2)/(y·Q²) (arXiv:0910.5831 Eq. 10; σ_γ\*p ≡ σ_T + ε̄σ_L, no F_L separation) and a per-bin, cross-section-weighted, MC-derived effective flux ⟨1/Γ⟩ from the DJANGOH NoZ photon-Born e⁻ truth sample — no y-binning, no re-unfolding — as a derived sidecar on the current cone RAW release (σ_γ\*p = RAW × C_gamma_born × ⟨1/Γ⟩), gated on the documented C_gamma_born provisional-status caveats and the count-weighting anti-trap.

---

## Appendix A — prototype numeric record (2026-10-15, one file, read-only)

Input: `nozborn_em_0000.root` (ChargedGenRC; 777 094 selected hadron entries; selection = worker lines 118–142). Flux: Hand/H1, α = 1/137.036, Γ evaluated at (`Y_es_gen`, `Q2_es_gen`).

| cell | ⟨1/Γ⟩ 1-file [GeV²] | ⟨1/Γ⟩ full-sample w-weighted [GeV²] | ε̄ | ⟨Q²⟩ | ⟨y⟩ | Γ spread (event-weighted) | analytic envelope | γ²_max | Q²/ν²_max |
|---|---|---|---|---|---|---|---|---|---|
| S0_x0 | 63 297 | 62 982 | 0.815 | 197.5 | 0.460 | 8.9× | 9.0× | 1.2e−6 | 1.2e−6 |
| S0_x1 | 22 024 | 22 128 | 0.975 | 214.6 | 0.187 | 9.9× | 10.2× | 1.0e−5 | 1.0e−5 |
| S1_x0 | 128 792 | 128 576 | 0.823 | 412.1 | 0.447 | 12.1× | 12.5× | 3.0e−6 | 3.0e−6 |
| S1_x1 | 47 753 | 47 553 | 0.976 | 468.2 | 0.184 | 12.3× | 12.7× | 2.3e−5 | 2.3e−5 |
| S2_x0 | 301 943 | 299 933 | 0.813 | 935.2 | 0.461 | 9.7× | 10.1× | 5.8e−6 | 5.8e−6 |
| S2_x1 | 107 719 | 108 816 | 0.975 | 1035.5 | 0.188 | 11.2× | 11.7× | 4.9e−5 | 4.9e−5 |
| S3_x0 | 734 613 | 735 729 | 0.881 | 3077.0 | 0.347 | 137× | 152× | 2.6e−4 | 2.6e−4 |

Level (RAD vs Born) reweighting: +0.93 / +1.00 / +0.54 / +0.51 / +0.46 / +0.56 / +1.75 %. Count-vs-weight bias (born level): +6.98 / +12.14 / +2.75 / +10.25 / +0.00 / +1.63 / +0.00 %. Bin-center bias vs ⟨1/Γ⟩, geometric centers: −13.9 / +69.0 / −22.5 / +70.6 / −18.8 / +71.8 / −0.7 %; log centers: −22.4 / +46.1 / −28.7 / +55.7 / −25.2 / +50.2 / −23.9 % (cells 0–6 in order).

Current-basis integrals (cone RAW × per-bin factors, today): RAW 11.412233/11.123626 nb; ×C_rad 13.026987/12.697511 nb (Born release, exact); ×C_gamma_born 11.875722/11.571280 nb (photon-only ep Born); ×CγB×⟨1/Γ⟩ 1 170 035/1 129 842 nb·GeV² (γ\*p prototype); F_EWgamma per-bin 0.785–0.991, integrated 0.9116/0.9113; all 490 bins finite.

## Appendix B — verification log (all read-only, 2026-10-15)

1. Release driver lines 195/628 and `observables.tex` lines 8–12 re-verified by direct read.
2. Cone RAW CSV schema verified (charge `pos`/`neg`, per-charge `bin_index` 0–244, `global_bin_index` 0–489, 490 nonzero rows) and aligned to the CγB/C_rad tables (global `bin_index`, charge `h+`/`h−`); ΣRAW and ΣRAW·C_rad reproduce the raw and Born release totals exactly.
3. CγB 490-row table columns confirmed (`C_gamma_born`, `F_EWgamma`, `C_rad_mixed_published`, `identity_residual`, counts); yresolved 2940-row table columns confirmed (`sigma_gamma_born`, `sigma_rad_mixed`, `gamma_born_n`, `rad_mixed_n`, 6 y-bins).
4. Worker selection (lines 118–142) mirrored exactly in the prototype; per-file entry rate consistent with full-sample CSV counts (≈200 M entries / 260 files).
5. No `*yresolved*` file exists under `rapgap_crad_releasebin_emfix_2026-10-09/` (glob) — RG model-dependence leg requires the optional Phase-1 pass.
6. H1/ZEUS flux formulas transcribed from `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research-cross-section-method.md` lines 18–61 and `/data/dust/group/h1/klesthen/BaryonAsym/exclu_phi/docs/research/phi_h1_process_spec.md` lines 397–428, 674–709 (which quote arXiv:0910.5831 §5.1 Eqs. 9–10 and arXiv:hep-ex/0504010 §4); the flux-table README should cite the papers directly. Trento Eqs. (7)–(10) verified earlier in this analysis cycle from arXiv:hep-ph/0611265.
7. No existing file was modified; the only workspace file created by this task is this memo.
