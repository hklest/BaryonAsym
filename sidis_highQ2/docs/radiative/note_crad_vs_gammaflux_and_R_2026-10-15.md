# Note: C_rad vs the gamma*p flux division, and access to R = sigma_L/sigma_T (2026-10-15)

**Author:** h1-radiative-correction (delegated read-only verification; `sidis-hq2-radiative` skill content read directly from `sidis_highQ2/.opencode/skills/sidis-hq2-radiative/SKILL.md` after the runtime skill loader failed; same precedent as `docs/research/gammap-electroweak-level-2026-10-15.md` header).
**Type:** READ-ONLY. No C_rad table, script, release product, budget, or note file was modified. This memo is the only new file.
**Questions:** (A) Does the applied C_rad contain any equivalent-real-photon / virtual-photon-flux factor, such that dividing the released Born-level ep cross sections by the Hand flux Gamma(x,Q2,y) afterwards would DOUBLE-COUNT? (B) Can R = sigma_L/sigma_T (or an L/T decomposition) be obtained from what we already have, and if not, what is the smallest defensible external input?

**Basis audited (current products):**
- Born release `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (`sigma_Born(bin) = C_rad(bin) x Unfold(data; R)`; IBU h+ 13.026987 / h- 12.697511 nb).
- Applied primary DJANGOH14 releasebin table `sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (integrated h+ 1.1415172920054786 / h- 1.1410367377289135).
- Formal alternative RAPGAP31 emfix table `sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (integrated h+ 1.1342370401351234 / h- 1.1336689310022947).

---

## 0. Verdict summary

1. **QUESTION A — COMMUTE, no double counting.** C_rad as implemented is a pure per-bin ratio of beam-mixed weighted gen-level event/hadron sums (ep cross-section proxies) between the NONRAD and RAD samples at fixed release binning: `C_rad(bin) = sigma_nonrad(bin)/sigma_rad(bin)`, `sigma = sum(weight1*weight2)/L_MC`. It contains **no** Hand, Weizsäcker-Williams, Mott, `alpha/(x Q^4)`, or `y^2/(2(1-epsilon))` factor of any kind (grep evidence in §1.3). Dividing `sigma_Born` by Gamma afterwards is the **first and only** flux division in the chain.
2. **QUESTION B — R not needed under the sigma_gamma*p = sigma_T + epsilon*sigma_L convention (recommended), and NOT extractable from our samples if any other convention is chosen.** The trees carry no structure-function or gamma*/Z polarisation weights (only `weight1`/`weight2`, the HAT MCWE/GKI + user event weights of the full ep cross section); both generator legs are full (T+L) unpolarized NC samples; the production is single-beam-energy (920 GeV), so at fixed (x,Q2) epsilon is kinematically fixed and the in-cell y spread is 1:1 degenerate with the intra-cell (Q2,x) variation of the structure functions. If a transverse-only or epsilon-averaged convention were adopted, R must come from external input (NLO PDF x FF theory, R_SIDIS ~ 0.03-0.10 at our Q2, or published H1 inclusive F_L only up to Q2 ~ 800 GeV2 as a proxy); see §2.4-2.5.
3. **Convention flag (not a flux double-count, but it gates the same work item):** the Born level produced by the applied C_rad is the **full neutral-current ep Born** (gamma+Z, dressed). Dividing *that* by the Hand flux yields a mislabelled "(gamma*+Z)p" quantity; the gamma*p product must be built from the photon-Born sidecar `C_gamma_born = C_rad x F_EWgamma` (unapplied, DJANGOH-only, PROVISIONAL DIAGNOSTIC), per `docs/research/gammap-electroweak-level-2026-10-15.md`. See §1.6.

---

## 1. QUESTION A — is there any flux factor in C_rad?

### 1.1 The implemented definition, with file and line

The tables were produced by the releasebin campaign scripts (copied as provenance inside each table directory; forks of the canonical v3 machinery in `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/`):

| Step | File:line | What it does |
|---|---|---|
| Per-row weight | `.../djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py:118` | `w = a["weight1"].astype(float)*a["weight2"].astype(float)` — the only per-event weight; nothing else enters. |
| Histogram fill | same file, `:142` | `np.add.at(h["sumw_y"],(yy,glob),w[m])` — bin content is the plain weighted count `sum(w)` per release bin (plus `sumw2`, `n`). |
| Bin assignment | same file, `:120-121, :140` | release-safe `(Q2esGen, x_es_gen)` cell + `z`, `pT_h` (gamma*p CM) binning; DIS fiducial `150<=Q2_es_gen<20000`, `0.1<Y_es_gen<0.7` at `:122`. |
| sigma proxy | `.../djangoh14_crad_releasebin_2026-10-09/scripts/finalize_releasebin.py:41-48` (`mix_by_y`) | `sn += BEAM[b]*sumw_nonrad[b]/L_MC,nonrad[b]`, `sr += BEAM[b]*sumw_rad[b]/L_MC,rad[b]` — beam-mixed (f_ep 0.5140930272 / f_em 0.4859069728) weighted-sum / MC-luminosity proxies. Identical construction both legs. |
| The ratio | same file, `:38-40` (`def ratio`) | `c = n/r` — **C_rad = sigma_nonrad / sigma_rad**; the 490-row table at `:69` is the y-summed ratio per bin. |
| RAPGAP31 emfix leg | `.../rapgap_crad_releasebin_emfix_2026-10-09/scripts/finalize_releasebin.py:39, :43` | `sn/sr` proxies and `c = sn[ci,j]/sr[ci,j]` — same pure-ratio structure; summary key `'c_rad_definition':'sigma_nonrad/sigma_rad'` at `:47`. |
| Canonical v3 machinery | `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/crad_v3_worker.py:86` and `crad_v3_finalize.py:27-28, 34-35, 58` | identical logic (`c=n/r`; `summary["C_rad"]="sigma_nonrad/sigma_rad"` at `:48`); the releasebin scripts are its labelled forks (docstrings, `finalize_releasebin.py:1-12`). |
| Release contract | Born release `SELECTION.md:38-40` | "`sigma_Born = C_rad x Unfold(data; R)` ... `C_rad(bin) = sigma_nonrad(bin)/sigma_rad(bin)`, `sigma = sum(weight1*weight2)/L_MC`" — same definition, verbatim in the shipped contract. |
| Application metadata | `sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-07/build_born_candidate.py` (`c_rad_table_definition`, ~lines 1708-1716) | the applied-table definition string carried in every Born-release `metadata.json`. |

Orientation check (no conflict): every layer — worker, both finalizers, release `SELECTION.md`, skill, `STATE.md` — uses **sigma_nonrad/sigma_rad**. Integrated C_rad ≈ 1.141 (>1): the correction is a radiative-distortion ratio, not a flux-like <1 factor. The generic band "C_rad ~ 0.7-0.8, delta_QED = 1-C_rad ~ +0.2-0.3" does **not** hold for this analysis (delta_QED ≈ −0.14 integrated) because the hard truth cuts (`GenEmpz>45`, `0.1<y<0.7`, `xF<0`) remove radiatively-degraded events so sigma_rad < sigma_nonrad in-bin; this deviation is documented and gated (`docs/research/gammap-electroweak-level-2026-10-15.md` §6).

### 1.2 What `weight1`/`weight2` are (and are not)

- `RC_treebuilder_genonly_v3.C:57, :76` reads them straight from the HAT branches `Weight1`/`Weight2`; the H1OO semantics are documented at `h1oo/H1Hat/H1HatEvent.h:156-157` (release 4.2.1): `fWeight1` = "event weight on L4 or **MC GKI weight**", `fWeight2` = "event weight on L5 or **MC user weight**".
- For our MC legs these are the DJANGOH MCWE/GKI reweight and the user weight (non-unit: mean w1 ≈ 3.46 RAD / ≈ 2.53 NONRAD; ≈ 2.353 for the NoZ leg — `docs/radiative/crad_session_pitfalls_2026-09-28.md` defect 2; `docs/research/gammap-electroweak-level-2026-10-15.md` §2.1). They reweight the generated kinematic spectrum to the generator's **full ep cross section** (the complete NC ep matrix element, both photon polarisations, gamma+Z). They are generator bookkeeping weights; they carry **no L/T decomposition and no flux factorisation**.
- `L_MC = sum_generated(weight1*weight2)/SIGMT` (`crad_session_pitfalls_2026-09-28.md` §4 convention; `crad_machinery_guide_2026-09-28.md` §4). Hence `sigma_proxy(bin) = SIGMT x [weighted fraction of the sample in the bin]` — an ep-level cross-section proxy. **`L_data` never enters the ratio** (guide §4; skill Traps).

### 1.3 Flux-content scan: zero hits

`grep -i "hand|weizsacker|mott|gamma_flux|flux|alpha/(x|1-y)^2|..."` over all production scripts (`histogramming_v3/*.py`, both releasebin `scripts/*.py`, the emfix and nozborn finalizers): **no match** — no equivalent-photon flux, no `alpha/(x Q^4)`, no `y^2/(2(1-epsilon))`, no Mott term, no propagator reweighting is applied anywhere in the C_rad chain. The only kinematics entering are the binning/selection cuts themselves.

### 1.4 Why the division by Gamma COMMUTES (and cannot double-count)

Logical structure of the chain:

```
Unfold(data; R)   -> RAD-level ep cross section per bin (detector-corrected; response MC is RAD)
x C_rad(bin)      -> NONRAD-level ep cross section per bin = Born ep cross section (sigma_nonrad/sigma_rad cancels the QED-radiation distortion)
/ Gamma(bin)      -> gamma*p-level cross section      <-- the ONLY flux division, performed once, afterwards
```

Both `sigma_rad(bin)` and `sigma_nonrad(bin)` are *full ep cross sections* in the same bin. Whatever flux-like content the ep cross section contains (it is part of the ep matrix element the generator weights encode) is present **identically in the numerator and the denominator** — C_rad never divides it out; it only removes the RAD-vs-NONRAD distortion. Therefore:

- **Dividing the Born-level ep cross section by Gamma afterwards does NOT double-count any flux.** The two operations act on different factors of the ep cross section: C_rad on the radiative-distortion factor, 1/Gamma on the lepton-side flux factor. They commute in the sense that C_rad contains no 1/Gamma and 1/Gamma contains no radiative content.
- Conversely, C_rad is also not *missing* any flux: it is not meant to and must not be composed with any equivalent-photon correction. No WW/elastic-tail subtraction exists anywhere in the chain, and none is needed for the planned per-bin Gamma division (any equivalent-real-photon *background* treatment would be a separate, reco-level concern that our selection already handles via the electron-cone/alphaveto and e-Sigma cuts; nothing in the gen-only C_rad pre-empts it).

### 1.5 Fixed (Q2,x) with y implied — and what that means for the Gamma division

- C_rad is defined at fixed release bin = (Q2,x) cell (release-safe `Q2esGen`, `x_es_gen`) x `z` x `pT_h`, **with y implied**: the applied 490-row table is the exact y-projection over the strict selection `0.1 < Y_es_gen < 0.7` (worker `:122`; finalizer `:69` sums the six 0.1-wide y bins; the y-resolved `crad_v3_..._yresolved_2940rows.csv` sidecar exists per y bin). It does **not** depend on any explicit flux; its own mild y-dependence (flat to slightly falling, 1.158 -> 1.108 over y bins 1-6; slope −2.5% to −4.4% — machinery guide §7, script `02_crad_y_dependence.py`) is the y-dependence of the radiative distortion, not a flux.
- Consequence for the conversion: `Gamma(x,Q2,y)` varies with y **within** every bin, so `sigma_Born(bin)/Gamma(bin-centre)` is an approximation whose accuracy is set by the bin's y population. Populated y range (established): **y in (0.100, 0.700)**, i.e. Hand-convention epsilon in **(0.55, 0.99)** — epsilon(0.1) = 0.9945, epsilon(0.7) = 0.5505 (collider limit; gamma = 2Mx/Q negligible here). Per-cell y populations (computed read-only from the y-resolved sidecar `rad_n`, DJ14 leg; h+/h- agree to <1%):

| Cell | Q2 range | x range | populated y | mean y | mean epsilon | y-bin fractions (0.1-0.2 ... 0.6-0.7) |
|---|---|---|---|---|---|---|
| 0 | 150-300 | 0.0021-0.0071 | [0.20, 0.70] | 0.482 | 0.796 | 0 / 7.7 / 20.9 / 25.5 / 23.9 / 22.0 % |
| 1 | 150-300 | 0.0071-0.030 | [0.10, 0.50] | 0.211 | 0.968 | 50.9 / 37.6 / 11.4 / 0.2 / 0 / 0 % |
| 2 | 300-700 | 0.0042-0.016 | [0.10, 0.70] | 0.455 | 0.817 | 0.3 / 13.5 / 23.5 / 24.1 / 20.7 / 18.0 % |
| 3 | 300-700 | 0.016-0.069 | [0.10, 0.50] | 0.202 | 0.971 | 57.4 / 33.4 / 8.9 / 0.4 / 0 / 0 % |
| 4 | 700-1500 | 0.0099-0.034 | [0.20, 0.70] | 0.459 | 0.815 | 0 / 12.8 / 23.5 / 24.3 / 21.0 / 18.4 % |
| 5 | 700-1500 | 0.034-0.150 | [0.10, 0.50] | 0.198 | 0.972 | 60.6 / 31.4 / 7.7 / 0.4 / 0 / 0 % |
| 6 | 1500-20000 | 0.021-1.5 | [0.10, 0.70] | 0.352 | 0.879 | 24.8 / 20.5 / 17.0 / 14.4 / 12.4 / 10.9 % |

  Mean epsilon per cell spans 0.80-0.97 — a bin-centre Gamma can be materially off the population-weighted Gamma in cells 1/3/5 (mean y ≈ 0.20 vs the geometric centre ~0.30 of the selection window). The per-cell Gamma **averaging convention** (event-weighted mean over the released binning vs bin-centre evaluation) is the one open analysis decision already flagged in `docs/audits/audit_gammaflux_inputs_2026-10-15.md` §5; per-event Gamma from existing branches (no builder change) is the clean route. This is a binning-consistency caveat, **not** a double count.

### 1.6 What Born level C_rad produces — and the gamma*p convention flag

- The Born level of the applied C_rad is the **unobserved Born ep cross section**: the QED-nonradiative (no ISR/FSR) ep cross section at the release gen-level fiducial, mixed-beam, with **full neutral-current electroweak content** (gamma+Z tree exchange plus the stdjan loop dressing: `LPIN(2)=1, LPIN(7)=2, LPIN(8/9/11)=1` — analysis note `sections/radiative_corrections.tex:175-191`: "the Born level of the full neutral-current process, with both photon and Z-boson exchange").
- Therefore: `sigma_Born(current release) / Gamma` is an **ep-level Born quantity divided by a photon flux** — internally a "(gamma*+Z)p"-flavoured object, mislabelled by the Z/loop content (F_EWgamma integrated ≈ 0.912; of the integrated −8.8%, ≈ −0.35% is Z (mixed beam), ≈ −7.9% is loop/vacuum-polarization content; per-cell (1−F) grows 0.080 -> 0.153 with Q2 — `docs/research/gammap-electroweak-level-2026-10-15.md` §§2.2-2.3). This is a **convention mismatch, not a flux double-count**, and it is resolved by the already-derived photon-Born sidecar `C_gamma_born = C_rad x F_EWgamma` (integrated h+ 1.040960540463156 / h- 1.0401468080806677; unapplied, `c_rad_applied=false`, DJANGOH-only, PROVISIONAL DIAGNOSTIC). A gamma*p release must use that sidecar and must state the alpha convention: the Hand flux is an alpha(0) object, consistent with the pure-photon-Born-at-fixed-alpha(0) target (EW memo §1.1 recommendation).

### 1.7 QUESTION A verdict

**COMMUTE — no double counting.** Implemented definition: `C_rad(bin) = sigma_nonrad(bin)/sigma_rad(bin)` with `sigma = beam-mixed sum(weight1*weight2)/L_MC` — releasebin worker `crad_v3_worker_releasebin.py:118` (weight), finalizer `finalize_releasebin.py:38-40` (ratio `c=n/r`), `:41-48` (sigma proxies), `:69` (490-row table); RAPGAP31 emfix `finalize_releasebin.py:43` (`c=sn/sr`); canonical `crad_v3_finalize.py:27-28,34-35,58`; release contract `SELECTION.md:38-40`. No flux factor of any kind is present or removed; C_rad is defined at fixed (Q2,x) cell with y implied (integrated over 0.1<Y_es_gen<0.7) and produces the unobserved **full-NC ep** Born cross section — use the photon-Born sidecar before any Gamma division to stay within the one-photon-exchange convention the Hand flux assumes.

---

## 2. QUESTION B — access to R = sigma_L/sigma_T

### 2.1 What the trees actually store (B1)

- **RC v3 gen trees** (`GenDIS`, `ChargedGenRC`; builder `RC_treebuilder_genonly_v3.C`): branch inventory per `docs/radiative/crad_machinery_guide_2026-09-28.md` §2 and the worker `REQUIRED` list (`crad_v3_worker_releasebin.py:41`) — kinematics (`Q2Gki/XGki/YGki`, `Q2esGen/YesGen/XesGen`, `Q2_es_gen/Y_es_gen/x_es_gen`, `Q2sGen/YsGen`), radiation/beam state (`GenRad`, `GenEnPhoton`, `GenBeamElecE_eff`), four-vectors, provenance, and **`weight1`, `weight2` only** as per-event weights. **No structure-function branch, no F_L/F_2, no gamma*/Z polarisation weight.**
- **Central v6 analysis tree** (`ChargedTotGen`, builder `ChargedSidisTreeBuilderHighQ2_v6.C:830-887`): all DIS kinematic methods (`Q2eGen...XhGen` incl. `Q2esGen/YesGen/XesGen`), release-safe `Q2_es_gen/Y_es_gen/x_es_gen`, `zGen`, `xFGen`, `GenEmpz`, `GenEnPhoton`, gen electron/beam four-vectors, provenance flags, and `weight1`/`weight2` (`:856-857`). **No F_L/F_2/polarisation branch** (confirmed independently by `docs/audits/audit_gammaflux_inputs_2026-10-15.md` §2 "ABSENT" list: no `epsilon`, no `Gamma`, no `Sigma`, no polarisation branch).
- **weight1/weight2 semantics** (§1.2 above): HAT MCWE/GKI + user weights — they normalise each leg to the generator's **full ep cross section** (both photon polarisations folded together). There is no per-event L/T or polarisation information in any existing DJANGOH14 or RAPGAP31 sample of ours.
- **No polarisation-split sample exists.** The campaigns are standard inclusive NC DIS generations: DJ14 RAD (`Q2GT60`) / NONRAD (`Q2GT100`, old+new), RG31 RAD/NONRAD (`campaign_manifest.json` of both table dirs: only `rad`/`nonrad` x beam legs); the NoZ campaign isolates photon-vs-full EW content, **not** photon polarisation. The DJANGOH steering log (`Djangoh14/DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.B.LOG:86`) shows only `'POLA'` = *lepton-beam* polarisation (0 = unpolarized); no virtual-photon polarisation switch appears in any of our configurations.

### 2.2 Is the in-cell y-dependence sufficient for a Rosenbluth-like L/T fit? (B1)

**No — for three independent reasons.**

1. **No epsilon leverage at a fixed point.** At a single beam energy (our production is 920 GeV only; the MER 575 / LER 460 ODS periods were never processed by our builder — `docs/research/research-LT-separation-feasibility.md` §7.1/§7.3, `docs/audits/audit_gammaflux_inputs_2026-10-15.md` §4), y = Q2/(x·s) is **kinematically fixed** at fixed (x,Q2): epsilon has zero lever arm at a fixed kinematic point. This is exactly why the published H1 F_L extractions used the 920/575/460 energy scan (arXiv:1012.4355, arXiv:1312.4821).
2. **The per-cell y spread is degenerate with the structure functions.** The spread tabulated in §1.5 comes entirely from the cell's (Q2,x) extent (y is a function of Q2,x at fixed s). Fitting sigma(y) vs epsilon(y) within a cell conflates the epsilon-dependence of the L/T decomposition with the ordinary intra-cell (x,Q2) variation of F_T and F_L — the fit returns whatever (x,Q2)-model is imposed, not a measurement. The nominal epsilon spans per cell: cells 1/3/5 populate only y in [0.10,0.50] (mean epsilon ≈ 0.97; total lever Delta-epsilon ≈ 0.07); cells 0/2/4 populate y in [0.20,0.70] (mean epsilon ≈ 0.80-0.82); cell 6 spans the full window (mean epsilon ≈ 0.88). Only cells 0/2/4 and 6 even have a nominal lever, and it is unusable without an external model.
3. **Even with the full energy scan the separation is systematics-dominated.** The workspace's own feasibility study (`research-LT-separation-feasibility.md` §§3-4) finds that a single 2% point-to-point normalization offset inflates sigma(R)/R from ~26% to ~140% at R = 0.1; the luminosity cancellation that protects our multiplicity-style observables is broken in any Rosenbluth separation. Our samples have none of the required ingredients (no MER/LER trees, no per-energy C_rad, no per-energy relative-luminosity control).

Overall populated ranges (established, confirmed numerically this session): **y in (0.100, 0.700), epsilon in (0.55, 0.99)** — epsilon(0.1) = 0.9945, epsilon(0.7) = 0.5505.

### 2.3 Do the generator docs, workspace docs, or the H1OO reference code record/provide F_L or R? (B2)

- **Workspace docs:** no F_L or R value is recorded *for our samples*. The only F_L/R statements are expectation bands in `docs/research/research-LT-separation-feasibility.md`: inclusive R ≈ 0.2-0.3 assumed in H1 low-Q2 fits (arXiv:1012.4355); pQCD/TMD R_SIDIS ~ alpha_s(Q2)/pi x O(1) ≈ 0.03-0.10; best-estimate band 0.05-0.15 *for the low-Q2 fiducial* (5-100 GeV2) — and §6.2: **no R_SIDIS measurement exists at HERA collider energies at all** (any such measurement would be first-of-its-kind).
- **H1OO reference code:** the path given in the task, `H1ChargedSidis_HighQ2/.../h1oo-master/`, does **not exist** in the workspace (workspace root has only `h1oo-build/{bin,lib}`; the reference source lives in the AFS release `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`). Greps of that release for `FL / F2 / SigmaL / PolLong / gammaStar polarisation / structure function / Rosenbluth / Hand / Weizsäcker / equivalent photon` return **no DIS structure-function or polarisation calculator**: the only `Fl` hits are trigger-weight utilities (`H1CalcWeight.h:315-317`, `SetNCTrigWeighToFl` — weighting data triggers to the FL-run/bulk samples of the H1 F_L analysis, not an F_L calculation), and the `H1Gki` ODS bank (`H1OOBanks/ODS/H1Gki.h:30-70`) stores generation kinematics, `SIGMT`, `Genw` and xPDF values — no F_L/F_2, no polarisation fractions.
- **Generator steering:** our DJANGOH/RAPGAP configurations (§2.1) contain no photon-polarisation selection; both generators compute the full unpolarized (T+L) ep cross section in these campaigns. A gamma*L-only or gamma*T-only sample would be a **new MC campaign** (generator-level polarisation steering plus the full production chain) — not "what we already have".

### 2.4 Smallest defensible external input IF a transverse-only / epsilon-averaged convention were adopted (B3)

Ranked from smallest to largest footprint:

1. **Preferred: avoid R entirely.** Adopt `sigma_gamma*p = sigma_T + epsilon sigma_L` (the Hand-convention folding). Then `sigma_Born,ep(bin) = <Gamma x (sigma_T + epsilon sigma_L)>_bin` and the per-bin Gamma division needs **no R** — this is also the convention consistent with the one-photon-exchange SIDIS formalism (Bacchetta et al., eq. 2.7: `dσ = alpha^2/(x y Q^2) · y^2/(2(1-epsilon)) · [F_UU,T + epsilon F_UU,L + ...]`, cited in `research-LT-separation-feasibility.md` §1.1) and with the pure-photon-Born alpha(0) target of the EW memo. The only residual convention item is the per-cell Gamma/epsilon averaging (§1.5).
2. **If a transverse-only convention is forced: NLO theory R_SIDIS from PDF x FF.** No measured R_SIDIS exists at HERA collider kinematics (§2.3). The smallest defensible input is an NLO collinear calculation of F_UU,L/F_UU,T at our cells using globally fitted PDFs and fragmentation functions (HERAPDF/H1PDF-type PDFs x DSS-type FFs; the photon-only MAPTMD24 curve already packaged in the 5-prediction comparison is a candidate provider for the same (x,Q2) grid). **Expected size at our kinematics:** R_SIDIS ~ **0.03-0.10** (pQCD alpha_s/pi x O(1) suppression; the low-Q2 band 0.05-0.15 of the LT study tightens downward at Q2 >= 150 GeV2), with the epsilon-weighted L-term contribution `epsilon·R/(1+epsilon·R)` ≈ **4-10%** of sigma_gamma*p at our per-cell mean epsilon 0.80-0.97. Expected uncertainty on such a theory R: ~30-50% (FF and scale uncertainty dominate; the PDFs are well constrained at our x), propagating to ~2-5% on a transverse-only sigma_T — entirely avoidable under convention 1.
3. **Inclusive-F_L proxy (NOT recommended as primary):** the published H1 inclusive F_L from the energy scan reaches only Q2 ≈ 800 GeV2 (arXiv:1312.4821; sigma(F_L) ≈ 0.02-0.06, i.e. sigma(F_L)/F_L ≈ 15-30%; inclusive R ≈ 0.2-0.3 at low Q2 per the LT study §3.3/§5.1). It covers cells 0-3 and part of cell 4; cells 4-6 (up to 20000 GeV2) would need theory extrapolation anyway, and inclusive R is only a proxy for the charged-hadron SIDIS R. An NLO F_L from a PDF set is in any case the cleaner form of the same input.

### 2.5 QUESTION B verdict

**R not needed because the chosen convention is sigma_T + epsilon sigma_L** — and if that convention is adopted (recommended, §2.4-1), no R enters anywhere in the gamma*p conversion. Supporting facts: the trees expose only `weight1`/`weight2` (HAT MCWE/GKI + user weights; `H1HatEvent.h:156-157`) plus kinematics branches (`Q2esGen/YesGen/XesGen`, `Q2_es_gen/Y_es_gen/x_es_gen`, `zGen`, `GenEmpz`, ...); **no** `F_L`/`F_2`/polarisation branch exists in either the RC v3 trees or `ChargedTotGen`; no polarisation-split MC leg exists; single-beam-energy production makes any Rosenbluth-like in-cell fit degenerate (per-cell y/epsilon spread in §1.5). Should a transverse-only convention nevertheless be chosen, **R is extractable only from external input** (NLO PDF x FF theory, R_SIDIS ~ 0.03-0.10 at our kinematics, L-term weight ~4-10%; published H1 inclusive F_L stops at Q2 ≈ 800 GeV2).

---

## 3. Flags and hard blockers for the gamma*p work item

1. **No flux double-count blocker** (QUESTION A: COMMUTE). The planned `sigma_gamma*p = sigma_Born/Gamma` arithmetic is sound *given* the two convention decisions below.
2. **EW-level decision (gating):** the current Born release is full-NC ep Born (gamma+Z, dressed). A gamma*p product must be built from the photon-Born sidecar `C_gamma_born = C_rad x F_EWgamma` (unapplied, DJANGOH-only, PROVISIONAL DIAGNOSTIC; promotion requires a recorded user decision and the swap-rule systematics per the EW memo §3.3). Dividing the full-Born release by Gamma without this mislabels the result by the Z/loop content (integrated ≈ −8.8%, of which Z ≈ −0.35% mixed-beam).
3. **Gamma-averaging convention decision (gating, not blocking):** per-cell event-weighted mean vs bin-centre evaluation (audit `audit_gammaflux_inputs_2026-10-15.md` §5); per-event Gamma is computable from existing branches (`BeamElecE/BeamProtP`, `Q2es/Yes/Xes`, `scatE*`) with no builder change. The per-cell mean epsilon spans 0.80-0.97 (§1.5), so the choice is numerically material.
4. **alpha-convention consistency (statement duty):** the Hand flux is an alpha(0) object; the gamma*p target must be pure-photon Born at fixed alpha(0) (EW memo §1.1), and every quote must state EW level, factor basis, flux convention, and observable basis (cone-basis S2').
5. **Carried ceilings (unchanged by this note):** `P-CRAD-LUMI` (C_rad not publication-grade; FILE-SET-RECONCILED, generator-log closure blocked externally) and the PROVISIONAL status of the releasebin baseline propagate identically into any gamma*p product; the C_rad correction remains mixed-beam-only at the full-EW level (the photon-Born target is charge-blind by exact tree-level symmetry).

---

## 4. Evidence index (absolute paths)

- Workers/finalizers (production scripts of the applied tables):
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/crad_v3_worker_releasebin.py` (:41 REQUIRED branches, :118 weight, :122 y fiducial, :140-142 binning/fill)
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/scripts/finalize_releasebin.py` (:38-40 ratio, :41-48 sigma proxies, :69 490-row table)
  - `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/scripts/{crad_v3_worker_releasebin.py,finalize_releasebin.py}` (:43 `c=sn/sr`, :47 definition key)
  - Canonical machinery: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3/{crad_v3_worker.py:86, crad_v3_finalize.py:27-28,34-35,48,58}`
- Tree builders: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v3.C` (:57, :76 weights; branch table in `docs/radiative/crad_machinery_guide_2026-09-28.md` §2); `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` (:830-887 ChargedTotGen branches).
- H1OO weight semantics: `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/H1Hat/H1HatEvent.h:156-157`; no FL/F2/polarisation calc anywhere in that release (grep evidence §2.3); `H1OOBanks/ODS/H1Gki.h:30-70`.
- Release contract: `.../born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/SELECTION.md:38-40` and `metadata.json` (`c_rad_table_definition`); application script `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-07/build_born_candidate.py` (~:1708-1716).
- y/epsilon populations: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_yresolved_2940rows.csv` (read-only recomputation of per-cell `rad_n` y-fractions, this session).
- Companion documents: `docs/audits/audit_gammaflux_inputs_2026-10-15.md` (branch availability for per-event Gamma); `docs/research/gammap-electroweak-level-2026-10-15.md` (EW level, F_EWgamma, alpha(0) convention); `docs/research/research-LT-separation-feasibility.md` (L/T formalism, epsilon(y), R expectations, energy-scan blockers); `docs/radiative/crad_machinery_guide_2026-09-28.md` and `crad_session_pitfalls_2026-09-28.md` (weights/L_MC conventions); analysis note `sections/radiative_corrections.tex:95-120, 172-191`.
- Steering evidence: `/data/dust/group/h1/klesthen/BaryonAsym/Djangoh14/DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.B.LOG:86` (`'POLA'` lepton-beam polarisation only); campaign manifests of both table dirs (rad/nonrad legs only).

**Provenance:** read-only session (one CSV recomputation and single-file log/grep inspections on the login node within the ≤1-file interactive policy; no ROOT event loop run). No C_rad table, script, release product, budget, plot, STATE.md row, or note file was modified. Only this new memo was written.

---

# FOLLOW-UP SECTION (2026-10-15, second pass): the NoZ / photon-Born basis for the gamma*p product

**Scope:** read-only. Enumerate every existing NoZ / photon-Born / `C_gamma_born` artifact; quantify the gamma+Z -> gamma shift per bin, per charge, and per Q2 stratum; state the promotion path to an APPLIED photon-Born baseline; state the fallback labeling if promotion is not acceptable now. No product was modified; this appended section is the only change.

## 5.1 Inventory of every existing NoZ / photon-Born / C_gamma_born artifact

| # | Artifact (absolute path) | Rows / basis | Generator | Coverage & status |
|---|---|---|---|---|
| 1 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_490rows.csv` | **Full 490 rows** (245 h+ / 245 h-); release-safe `(Q2esGen, x_es_gen)`; `GenEmpz>45`; recomputed `xF<0` both legs; columns `C_gamma_born`, `F_EWgamma`, `C_rad_mixed_published`, `sigma_gamma_born`, `sigma_rad_mixed`, `sigma_nonrad_mixed`, `identity_residual`, per-bin stat | DJANGOH-only numerator (NoZ photon-only pure-Born e-, 260 truth trees) / mixed-beam DJ14 RAD denominator | Integrated h+ `1.040960540463156` / h- `1.0401468080806677`; 490/490 finite; per-bin stat median 0.551%, max 7.84%; **PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`**. Validation `crad_djangoh14_nozborn_releasebin.json`: G1-G7 PASS (G4 identity `C_gamma_born = C_rad x F_EWgamma` at 2.2e-16; G6 integrated band [0.95, 1.12], per-bin soft band [0.75, 1.02], zero violations), **G8 charge symmetry = None (impossible: no e+ NoZ control)** |
| 1a | same dir, `cgammaborn_empz_gt45_nominal_yresolved_2940rows.csv` | 2940 rows (6 y-bins x 490) | same | y-resolved companion, same status |
| 1b | same dir: `campaign_manifest.json`, `cgammaborn_v1_summary.json`, 6 merged per-beam primitives (`merged_primitives_{rad,nonrad_old,nonrad_new}_{ep,em}.npz`), `exposure_provenance_reused_exact.json`, `nozborn_exposure_exact_2026-10-08.json`, `scripts/{crad_v3_finalize_nozborn_releasebin.py, crad_v3_worker_releasebin.py, merge_releasebin_primitives.py, run_finalize_releasebin.sh}`, `verification/{hdr_NOZ_A.txt, hdr_NOZ_D0.txt, log.Dj.NoZ.2Feb23.txt}` (GEN-header + validation-log loop evidence) | campaign provenance | DJANGOH14 NoZ (`dst2all_djangoh14_NoZ`, Britzger "neu" executable; Z couplings zeroed at source; `LPIN(2)=0` active = no soft&virtual 1-loop; fixed alpha(0)=1/137.0359895; sigma 6708.559 pb) | Numerator exposure **EXACT_OOMCLUMI_SWEEP, status PASS**, `L_MC = 14,119,125.013920162 nb^-1` (reused verbatim, binning-independent; sumw 94,718,983.18) — the strongest exposure provenance in the C_rad chain |
| 2 | same dir, `cgammaborn_gen_unc_proxy_v3emfix_490rows.csv` + `cgammaborn_gen_unc_proxy_v3emfix.json` | 490 rows, identical 12-column bin key vs the applied table (max key deviation 0.0) | proxy (WithZ emfix spread + kappa term) | `u_model(bin;kappa) = sqrt(v3_emfix_rel^2 + [kappa(1-F_EWgamma)]^2)`; **kappa=0.02 adopted central** (medians h+ 0.62225% / h- 0.61890%; max 5.77%/5.67%), kappa=0.03 conservative (0.658%/0.653%); **SWAP-not-additive rule** for `c_rad_gen_rel`; mc-expert validation verdicts recorded in the JSON (kappa is an EMPIRICAL anchor; LPIN(8/9/11) do not remove tree-level Z; never cite HERACLES code identity) |
| 3 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/noz_cgamma_born_provisional_diagnostic/` | 4 byte-identical SHA256-verified copies (the 490-row CSV `7ec901af...`, validation JSON `c9ad4d60...`, kappa CSV `3e9dd915...`, kappa JSON `9683d8ba...`) | same | Consolidated baseline REFERENCE role; `RELEASE_2026-10-10.md` manifest lines 14, 19, 25, 42-45 ("NoZ C_gamma_born ... PROVISIONAL DIAGNOSTIC, unapplied"; carried caveats: P-CRAD-LUMI ceiling, PROVISIONAL baseline, mixed-beam-only, NoZ DJANGOH-only); **reference-only, NOT a repointing** |
| 4 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/data_side_noz_photonborn/` (`ibu/cross_sections.json`; builder `../build_data_side_noz_photonborn_2026-10-13.py`; validation `../validation_data_side_noz_photonborn.json`) | **490 rows; the only existing photon-Born product applied to data**: `sigma_photonBorn = C_gamma_born x raw cross_section_nb` per bin; `photon_born_c_gamma_applied=true`, `c_rad_applied=false` | DJANGOH14 NoZ table applied to the S2' unfolded raw | **NO-CONE S2' basis** (raw base `bbb_ibu_detints2prime_S3xmerged_..._2026-10-09`); integrated h+ `11.910263` / h- `11.606904` nb; PROVISIONAL DIAGNOSTIC, comparison-package data side only, "does NOT supersede the canonical S2' Born release"; **no cone-basis photon-Born product exists** (verified: no noz/photonborn artifact under `H1ChargedSidis_HighQ2/output_v6/`) |
| 5 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/tables/highQ2_z005_s2prime_born_public_2026-10-11b/blocks/block_noZ_{pos,neg}_S*_x*.tex` (14 blocks) | per-(Q2,x)-cell note tables | DJANGOH14 NoZ | In the noZ tables **C_gamma_Born is the APPLIED factor, C_rad reference-only**; `C_gamma_Born / C_rad = F_EWgamma` per bin (gated `factor_columns_consistency`); generator scheme spread CARRIED OVER from the WithZ pair (`noz_crad_gen_mode=carry`); no-cone S2' basis; analysis-note PROVISIONAL status caveats embedded |
| 6 | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_final_xfcut_2026-10-07/` | 490-row `cgammaborn_empz_gt45_nominal_490rows.csv` (framework-`XesGen` x-binning, A-F1) + pair-v1 tables (`crad_pairv1_fullEW_vs_nozborn_490rows.csv`, `crad_pairv1_nozborn_perbeam_490rows.csv`, 491 lines each) + exposure files + oomclumi sweeps + trailer diagnosis | DJANGOH-only | **SUPERSEDED for NoZ purposes** by the releasebin product (validation-JSON supersession field: "NoZ photon-Born release-safe binning update only; predecessor remains untouched provenance") |
| 7 | Upstream sample/campaign provenance: NoZ truth trees `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn/em/` (260 `ChargedGenRC` trees); hist campaign `.../output_v3/nozborn_hist_releasebin_20261009/`; exposure derivation `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/nozborn_exposure_2026-10-08/` + `docs/radiative/NOZBORN_EXPOSURE_DERIVATION_2026-10-08.md`; docs `docs/radiative/NOZ_PROVENANCE_INVENTORY_2026-10-07.md`, `docs/radiative/BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md`; diagnostic plots `plots/radiative/cgammaborn_v4_xfcut_nozborn_7col_2026-10-08/` (xFcut basis) | -- | DJANGOH14 NoZ | provenance and diagnostics only |

**Coverage verdict:** a **full 490-row, release-bin-aligned photon-Born table exists** (item 1) with 490/490 finite bins and the identical bin key as the applied C_rad table — the plot sidecar (item 4) is a derived application of it, not the only form. **Generator basis is DJANGOH-only**; no RAPGAP31 NoZ sample exists (verified 2026-10-09 sample search; recorded in the validation JSON and the kappa-proxy JSON `coverage_gap`).

## 5.2 Size of the gamma+Z -> gamma shift (full-NC Born -> photon-only Born)

The shift factor is exactly `F_EWgamma(bin) = sigma_gamma_born / sigma_nonrad_mixed` (per-bin column of item 1; `sigma_photonBorn = F_EWgamma x sigma_Born` bin-by-bin since `C_gamma_born = C_rad x F_EWgamma`). A per-bin vector EXISTS — nothing needs to be computed beyond reading the table (statistics below recomputed read-only from the 490-row CSV this session; they match `docs/research/gammap-electroweak-level-2026-10-15.md` §2.3).

**Integrated (sigma-weighted, per charge):**

| Charge | integrated F_EWgamma | shift (1 - F) |
|---|---|---|
| h+ | 0.911910 | **-8.809%** |
| h- | 0.911580 | **-8.842%** |

Data-weighted cross-check (5-prediction package sidecar vs the no-cone S2' Born release): `C_gamma/C_rad = 0.911633` (h+) / `0.911309` (h-) — i.e. -8.84% / -8.87%.

**Per Q2 stratum (sigma-weighted shift, both charges combined per cell):**

| Cells | Q2 range [GeV2] | sigma-weighted F | shift |
|---|---|---|---|
| 0 / 1 | 150-300 | 0.92019 / 0.91803 | **-7.98% / -8.20%** |
| 2 / 3 | 300-700 | 0.90935 / 0.90935 | **-9.07% / -9.07%** |
| 4 / 5 | 700-1500 | 0.89426 / 0.89364 | **-10.57% / -10.64%** |
| 6 | 1500-20000 | 0.85171 | **-14.83%** |

The shift grows monotonically with Q2 (running alpha + growing Z/gamma-Z content), as expected. **Largest shifts sit in the sparse high-z / high-pT bins of cells 5-6**: bin 209 (h+, cell 5, z 0.7-1.0, pT 4-8) F = 0.7846 → **-21.54%**; bin 488 (h-, cell 6, z 0.7-1.0, pT 2-4) F = 0.7974 → -20.26%; bin 485/474 (h-, cell 6) -18.28%. Smallest: bin 174 (h+, cell 4, z 0.7-1.0, pT 4-8) F = 0.9906 → -0.94% (a sparse-bin statistical outlier, not a trend).

**Per-bin vector min / median / max of the shift (1 - F_EWgamma):**

| Charge | min | median | max |
|---|---|---|---|
| h+ | -0.94% | -9.04% | **-21.54%** |
| h- | -4.86% | -9.44% | **-20.26%** |

(`F_EWgamma` h+ min/median/max 0.7846/0.9096/0.9906; h- 0.7974/0.9056/0.9514. The composed `C_gamma_born` per-bin span: h+ 0.7081-1.3580, median 1.0443; h- 0.7378-1.3411, median 1.0406 — includes the C_rad distortion; sparse-bin extremes.)

**Composition (from the EW memo §§2.2-2.3, anchored):** of the integrated ≈ -8.8%, only ≈ **-0.35% is Z exchange (mixed beam)**; ≈ **-7.9% is loop / vacuum-polarization (running-alpha) content**. The Z/gamma-Z part grows to +8.6% (e-, Q2>1000, Born) in the top cells — which is why the per-stratum shift rises to -14.8% in cell 6.

## 5.3 Promotion path: NoZ table -> APPLIED photon-Born baseline (parent of a gamma*p product)

Required, in dependency order (per `docs/research/gammap-electroweak-level-2026-10-15.md` §3.3 and the recorded precedent pattern of the 2026-10-07 / 10-09 / 10-13 promotions):

1. **Recorded user promotion decision + workflow** flipping the NoZ releasebin sidecar from PROVISIONAL DIAGNOSTIC / unapplied to the applied factor of a **derived** gamma*p release — with completed review of the baseline, gate evidence, checksummed additive install, and the decision record. The **full-Born (withZ) release stays primary**; the gamma*p product is a derived, separately labelled alternative (the note's two-convention structure, never-mix rule).
2. **Build and install the gamma*p Born release on the CURRENT CONE basis**: `sigma_gammaBorn(bin) = C_gamma_born(bin) x Unfold_cone(data; R)`. The tables are cone-invariant (gen side unchanged; `crad_table_inheritance` sha-asserted precedent; per-bin photon-Born shift == per-bin cone raw shift, gated <= 1e-12; CHOICE-A yield policy; scale absolute-nb columns only, yields and relative columns byte-identical). The existing applied sidecar (item 4) is on the **superseded no-cone S2' basis** and does NOT carry — a cone-basis build is required.
3. **Generator-systematic policy decision (required):** EITHER adopt the kappa-proxy (`u_model_rel_k002` as the SWAP replacement for `c_rad_gen_rel`) OR keep the note tables' carry mode (`noz_crad_gen_mode=carry`). In both cases the leg REPLACES `c_rad_gen_rel` (never additive) and the statistical leg swaps simultaneously to the NoZ numerator stat (median 0.551%, max 7.84%, replacing `c_rad_stat_rel` medians 0.4272%/0.4429%).
4. **Provenance closure items:** numerator exposure is already EXACT_OOMCLUMI_SWEEP / PASS (reused, binning-independent); the RAD denominator and the composed `C_rad` component inherit **P-CRAD-LUMI FILE-SET-RECONCILED** (generator-log closure permanently blocked externally) — the composed factor and any gamma*p product therefore remain NOT publication-grade and ship with the documented caveat, exactly as the ep Born release does.
5. **Conventions (no open physics item, but must be stated in the promotion record):** `GenEmpz>45` nominal (CLOSED 2026-10-09 user decision, retained; the NoZ table is on the same mode); recomputed `xF<0` current-hemisphere on both legs (same truth contract as the applied C_rad, matching the release gen pool); release-safe `(Q2esGen, x_es_gen)` binning with the identical 12-column bin key vs the applied table (max key deviation 0.0). Loop sub-convention: **pure photon Born at fixed alpha(0)** (recorded user decision 2026-10-08; `LPIN(2)=0` active per GEN headers; the dressed NOZ1 alternative exists as a sample but NOT as a table — expected integrated ≈ 1.136 if ever required).
6. **Coverage gaps carried (unresolvable without new production):** no RAPGAP31 NoZ sample → no two-generator cross-check for the EW factor (anchored by the kappa-proxy, mc-expert-validated as an EMPIRICAL anchor; never justified by HERACLES code identity); G8 empirical photon charge symmetry impossible (no e+ NoZ) — carried by the exact tree-level charge-symmetry argument plus the Britzger e+-integrated identity (NoZ e+- = 218.84216 pb both).
7. **Note/flux statements:** EW level (pure photon Born at alpha(0)), factor basis, the alpha(0)-consistency statement with the Hand flux (jointly with the flux owner), the G8-substitute argument, and the never-mix rule; P-CRAD-LUMI ships as a documented caveat.
8. **NOT required:** per-beam C_rad tables (charge-blind photon target); a photon-only e+ production; new RAD/NONRAD C_rad campaigns; any change to the applied WithZ C_rad tables.

**Does the 15-component systematics budget commute with the per-bin multiplicative factor? YES for every non-C_rad component.** `C_gamma_born` is applied post-unfold as a per-bin multiplicative factor on the absolute-nb columns only — the same CHOICE-A structure the Born release already implements, under the recorded table-flip invariance ("INVARIANT under any table flip: data vector, response, migration, acceptance, IBU k=4 raw output, and all RELATIVE systematic products — only absolute nb products rescale"; the cone promotion re-derived relative products because the observable basis changed, not because of a table flip). All 13 central-mirroring components plus `detint_removal_rel` and `fakecol_mcstat_rel` carry unchanged. **Exception — the two C_rad legs do NOT commute:** `c_rad_gen_rel` and `c_rad_stat_rel` must take the recorded SWAP of item 3 (kappa-proxy or carry + NoZ stat). Expected net budget impact: ≲ +0.05 pp on the median totals (5.8032%/6.1919% -> ~5.85%/6.24%).

## 5.4 Fallback if promotion is not acceptable now

- **Presenting `(full-NC ep Born)/Gamma` as "sigma(gamma*p)" is NOT defensible.** The gamma*p level is by definition pure one-photon exchange; the current Born release retains Z exchange and full loop dressing. The EW memo is unambiguous: no cited H1/HERMES/COMPASS result quotes a dressed/absorbed gamma*p cross section; dividing the full-Born release by a photon flux "yields a mislabelled '(gamma*+Z)p' cross section" (§5.1); and the one-photon-exchange theory curve in the comparison package (MAPTMD24) is photon-only, so a direct comparison would be wrong by the shift quantified in §5.2.
- **The honest fallback labels:** (a) publish the **ep Born cross section (full NC)** — the current release, the H1-standard HERA high-Q2 convention — with no flux division, and defer the gamma*p presentation to after promotion; or (b) if a flux-normalized quantity is wanted now, label it **"flux-normalized full-NC Born ep cross section, sigma_Born^ep(full NC)/Gamma_Hand"** with an explicit contamination statement: "retains Z-boson exchange and electroweak loop content; differs from the one-photon-exchange gamma*p cross section by -8.8% integrated (per-bin -0.9% to -21.5%; -8.0% at Q2 150-300 rising to -14.8% at 1500-20000 GeV2); the one-photon-exchange (photon-Born) level requires the additional DJANGOH-only provisional factor C_gamma_born". It must never be called sigma(gamma*p), never compared to photon-only theory without the F_EWgamma correction, and the note's never-mix rule applies verbatim.
- **Recommended parent basis for the gamma*p product: the NoZ photon-Born table (item 1), promoted to the applied factor, on the current cone basis — i.e. promote first, then divide by Gamma.** `sigma_gamma*p(bin) = C_gamma_born(bin) x Unfold_cone(data;R) / Gamma(bin)` at the pure-photon-Born alpha(0) level; the full-NC Born release remains the primary product. The fallback (b) hybrid is a presentation-level construct with no literature precedent and would be flagged in review; it is acceptable only as an explicitly labelled intermediate, never as the gamma*p deliverable.

**Provenance (follow-up):** read-only session; the only file changed is this appended section. Statistics in §5.2 recomputed from the 490-row NoZ CSV (`cgammaborn_empz_gt45_nominal_490rows.csv`) and the sidecar JSON (`data_side_noz_photonborn/ibu/cross_sections.json`); manifest facts from `RELEASE_2026-10-10.md`, `README_NOZBORN_RELEASEBIN.md`, `cgammaborn_gen_unc_proxy_v3emfix.json`, `crad_djangoh14_nozborn_releasebin.json`, and the note-table block headers.
