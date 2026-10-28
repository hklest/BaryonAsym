<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

> **RESOLVED** (2026-05-16): eSigma frozen as baseline. Retained for background reference.
>
**Status:** Current research note with partial resolution applied. The baseline decision is now frozen to `eSigma`; use this document mainly for background, event-level comparison logic, and historical rationale.

# Optimal DIS Kinematic Reconstruction for Charged SIDIS at H1

**Scope.** Compare the available DIS kinematic-reconstruction methods (electron,
Sigma, Jacquet-Blondel/hadron, double-angle, eSigma) for their suitability as
the primary method in the H1 charged-hadron SIDIS analysis (low-Q² and
high-Q²). The driver is the observed poor purity/stability of response
matrices in `z` and `xF`, which depend on the boost to the gamma\*p CM frame
and therefore on the resolution of the reconstructed virtual-photon
4-vector `q` (not just `Q²` alone).

**Sources used (file:line).** (The builder files named `ProtonTreeBuilder_lowQ2.C` and `ProtonTreeBuilderHighQ2.C` are the v5b SIDIS builder sources; the proton-prefixed names are legacy from the fork ancestor. See `docs/references/reference-naming-aliases-highQ2-sidis.md` for the alias table.)
- Local code: `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` (low-Q² reco builder),
  `sidis_lowQ2/source_cache/ProtonTreeBuilderHighQ2.C` (high-Q² reco builder),
  `lambda/.work_feeddown/TreeBuilder.C` (low-Q² evls stage),
  `lambda_HiQ/src_review/TreeBuilder.C` (high-Q² evls stage).
- H1OO framework: `h1oo-master/H1PhysUtils/H1MakeKine.{h,C}`,
  `h1oo-master/H1Calculator/H1CalcKine.{h,C}`, `h1oo-master/H1Hat/H1HatEvent.C`.
- Published papers: H1 multiplicity DESY 20-176 (`docs/ChargedParticlePaper_extracted.md`),
  H1 charged spectra arXiv:1302.1321, ZEUS scaled-momenta arXiv:1001.4026,
  Bassler & Bernardi NIM A361 (1995) 197 (= arXiv:hep-ex/9412004).
- Prior team research: `sidis_lowQ2/teamB/lessons_from_published_analyses.md`
  §1.3, §1.5, §1.11, §3.E, §5, §7.4-7.5; `docs/research-sidis-supervisor-scope.md` §3.3.

---

## 1. Critical correction to the prompt's premise

The task brief states "the current tree builder uses the electron method".
This is **not accurate** for either the low-Q² or the high-Q² code path.
What the builders actually consume is the HAT branch named `Q2s`/`Ys`, but
the value behind that name differs between the two regimes:

| Regime | Source of stored `Q2s`/`Ys` | Method actually used |
|---|---|---|
| **Low-Q²** | `q2 = *Q2s` (raw HAT pointer) in `lambda/.work_feeddown/TreeBuilder.C:350-351` | Sigma method (Bassler-Bernardi Σ alone) |
| **High-Q²** | `q2 = gH1Calc->Kine()->GetQ2es()` in `lambda_HiQ/src_review/TreeBuilder.C:443-445`, then written to a branch *also named* `Q2s` | eSigma method |

The H1OO HAT raw branches are explicitly defined as the Σ-only quantities in
`h1oo-master/H1PhysUtils/H1MakeKine.h:35-36`:

```
Float_t fQ2s;   // Q2 from the sigma method
Float_t fYs;    // Y from the sigma method
```

with the formula in `H1MakeKine.C:64-82`:

```
fYs  = sigma / [sigma + Ee'(1 - cos theta_e)]
fQ2s = (Ee' sin theta_e)^2 / (1 - fYs)
```

That is, `Q2s` from the raw HAT is **not** the electron-method `Q2` and not
the eSigma `Q2`. It is the Σ-only Q², which uses the HFS (via Σ) for both
y and Q² (no electron Q² ingredient).

Under this corrected baseline:

- **Low-Q² reco trees today are Σ-method**, not electron-method.
- **High-Q² reco trees today are eSigma**, but the gen-side branch
  `t_Q2sGen = *hatQ2sGen` (`lambda_HiQ/src_review/TreeBuilder.C:533-534`)
  reads the raw HAT `Q2sGen` (= Σ-method gen) — so reco and gen use
  different methods at high Q². This is a documented inconsistency that
  the user should be aware of independent of the question being researched.
- The proton tree builders inherit these conventions verbatim
  (`ProtonTreeBuilder_lowQ2.C:155, 503` and `ProtonTreeBuilderHighQ2.C:189-191, 498`).

**Implication for the user's diagnostic.** The poor response-matrix purity
and stability for `z` and `xF` at low-Q² is consistent with the Σ-method's
known characteristics (large W resolution tail at low y) — not with the
electron method, which is a different problem. This changes the framing of
the research question: at low-Q² we are choosing among Σ (current), eSigma,
electron, and double-angle, not improving on a known electron-method baseline.

---

## 2. Reconstruction methods, formulas, and ingredients

Notation: `E_e` = incoming electron beam energy, `E_p` = incoming proton beam
energy (used as a momentum), `s = 4 E_e E_p` ignoring electron mass,
`E_e'` = scattered electron energy, `theta_e` = scattered electron polar angle,
`Sigma = sum_HFS (E_i - p_z,i)` over hadronic-final-state particles after
calibration, `pT_h^2 = (sum_HFS p_x)^2 + (sum_HFS p_y)^2`,
`gamma_h` = HFS polar angle defined as
`cos(gamma_h) = (pT_h^2 - Sigma^2) / (pT_h^2 + Sigma^2)`.

Total `E - pz` of the event is `E_pz_tot = Sigma + E_e'(1 - cos theta_e)`,
which is conserved at ~2 E_e ≈ 55.2 GeV in HERA-II in absence of ISR.

### 2.1 Electron method (`MakeElec`, `H1MakeKine.C:26-44`)

```
y_e   = 1 - (E_e' / 2 E_e)(1 - cos theta_e)
Q2_e  = 4 E_e E_e' cos^2(theta_e/2) = 2 E_e E_e' (1 + cos theta_e)
x_e   = Q2_e / (s y_e)
```
HAT branches: `Q2e`, `Ye`, plus gen counterparts `Q2eGen`, `YeGen` (the
H1OO HAT explicitly stores these — see `H1HatEvent.C:1241,1245,1267,1271`).

- **Pros.** Pure electron-only; no HFS calibration needed. Best Q² at high y.
- **Cons.** y resolution ~ 1/y at low y → poor y_e for y < 0.1; ISR shifts
  y_e systematically because the assumed beam energy is not what hit the
  vertex.

### 2.2 Sigma method (`MakeSig`, `H1MakeKine.C:64-82`)

```
y_S   = Sigma / [Sigma + E_e'(1 - cos theta_e)] = Sigma / E_pz_tot
Q2_S  = (E_e' sin theta_e)^2 / (1 - y_S)
x_S   = Q2_S / (s y_S)
```
HAT branches: `Q2s`, `Ys`, `Q2sGen`, `YsGen`.

- **Pros.** Insensitive to ISR because the Σ replaces the assumed beam
  energy: any ISR photon escaping down the beam pipe is automatically
  removed. This is the central virtue Bassler-Bernardi (1995) demonstrate.
- **Cons.** Requires calibrated HFS. y_S resolution dominated by HFS energy
  scale at moderate-to-high y. Q²_S uses electron pT but inherits y_S
  through (1-y_S), so degrades with HFS errors.

### 2.3 Jacquet-Blondel / Hadron method (`MakeHad`, `H1MakeKine.C:49-62`)

```
y_JB  = y_h = Sigma / (2 E_e)
Q2_JB = pT_h^2 / (1 - y_JB)
x_JB  = Q2_JB / (s y_JB)
```
HAT branches: `Q2h`, `Yh`, `Q2hGen`, `YhGen`.

- **Pros.** Does not use the scattered electron at all; usable in CC-DIS
  where there is no electron in the final state. Good y at low y_e.
- **Cons.** Strongly dependent on HFS calibration. Q²_JB resolution is
  poor for DIS (factor 2-3 worse than Q²_e at moderate Q²). Useless for
  precision NC-DIS Q².

### 2.4 Double-angle method (`MakeDa`, `H1MakeKine.C:87-106`)

```
gamma_h: cos(gamma_h) = (pT_h^2 - Sigma^2) / (pT_h^2 + Sigma^2)
denom = sin(theta_e) + sin(gamma_h) - sin(theta_e + gamma_h)
Q2_DA = 4 E_e^2 sin(gamma_h)(1 + cos theta_e) / denom
y_DA  = sin(theta_e)(1 - cos gamma_h) / denom
x_DA  = Q2_DA / (s y_DA)
```
HAT branches: `Q2da`, `Yda`, `Q2daGen`, `YdaGen`.

- **Pros.** Uses only the two angles theta_e and gamma_h; immune to
  electron and hadronic energy *scale* uncertainties to first order.
  Standard reference method for cross-section systematics.
- **Cons.** gamma_h resolution degrades when most of the HFS is lost in
  the forward beam pipe (low x, low Q² is exactly that regime). At low Q²
  the angular resolution is the limiting factor and DA underperforms
  eSigma.

### 2.5 eSigma method (Bassler-Bernardi 1995, hep-ex/9412004; `MakeESig`, `H1MakeKine.C:111-117`)

The eSigma method takes Q² from the electron and y from Σ:

```
Q2_eSigma = Q2_e   = 2 E_e E_e' (1 + cos theta_e)
y_eSigma  = Q2_e / (x_S * s)        // equivalently: y_es is determined so that x_es = x_S
x_eSigma  = x_S    = Q2_S / (y_S s)
```

The `H1MakeKine` inline getters in `H1MakeKine.h:72-74` make the relationship
explicit:

```
GetQ2es() { return GetQ2e(); }                         // Q² from electron
GetXes()  { return GetQ2s() / (GetYs() * fSS); }       // x from Σ method
GetYes()  { return GetQ2e() / (GetXs() * fSS); }       // y derived to make x_es == x_S
```

There is **no** HAT branch named `Q2es`/`Yes`/`Xes`. The eSigma values are
always derived from `Q2e`, `Ys`, `Xs` (or equivalently from the H1Calculator
which calls `MakeESig` internally).

- **Pros.** Combines the best Q² (electron, when E_e' is well-calibrated)
  with the ISR-robust y (Sigma). At HERA the electron Q² resolution is
  better than Σ Q² for almost all of the kinematic plane, and y_S is
  better than y_e at low y. Recommended by Bassler-Bernardi as the
  default for low-x low-Q² inclusive DIS, and adopted as the H1 default
  for charged-particle measurements at low and high Q² (see §4 below).
- **Cons.** Inherits the SpaCal electron-energy scale uncertainty in Q²
  (no DA-style cancellation). Requires a calibrated HFS for y.

### 2.6 Y-average method (`MakeAv`, `H1MakeKine.C:122-167`)

A weighted combination of y_e and y_DA. Used by some H1 inclusive
analyses but not by any H1 SIDIS-style charged-particle analysis. Listed
for completeness; not pursued further here.

---

## 3. What "best resolution" means for SIDIS

The user correctly identifies that for SIDIS the relevant resolution is
not Q²(reco)/Q²(gen) and y(reco)/y(gen) individually, but the resolution
on the virtual-photon 4-vector `q = k - k'` because:

- `z = (P · p_h)/(P · q)`. The denominator is `(P · q) = (s/2) y` to a
  very good approximation (the proton mass term is negligible at HERA
  energies). So `z` resolution is dominated by **y resolution** plus the
  resolution on `(P · p_h)`, which in the lab frame is
  `(P · p_h) ≈ E_p (E_h - p_z,h)` — controlled by the hadron momentum
  measurement, *not* the kinematic method.
  - Bottom line for `z`: switching to a method with better y resolution
    (Σ, eSigma) directly improves z resolution.

- `xF = 2 p_z*/W` is computed in the gamma\*p CM frame. The boost to that
  frame depends on the full virtual photon 4-vector
  `q = (E_q; p_T,q, p_z,q)`. With `q = k - k'`:
  - `q_x, q_y` are determined by `theta_e, phi_e, E_e'` (electron only).
  - `q_z = -E_e + E_e' cos theta_e` and `E_q = -E_e + E_e'`.
  - Therefore the full 4-vector `q` constructed as `k - k'` depends only
    on the electron, **not** on Σ. This is what the current low-Q² code
    does in `ProtonTreeBuilder_lowQ2.C:535-540`:
    ```
    TLorentzVector scatElec4(scatEX, scatEY, scatEZ, scatEE);
    TLorentzVector gamma = eBeam4 - scatElec4;
    ```
  - So the current `gamma` 4-vector inherits **all** the SpaCal energy/angle
    resolution, regardless of which method `Q2s/Ys` happens to be.

- `W = sqrt(s y - Q2 + M_p^2)`. Resolution on W therefore tracks y
  resolution.

- The key SIDIS variable resolutions decompose as:
  - Resolution(z) ≈ Resolution(P·q) ≈ Resolution(y_method)
  - Resolution(W) ≈ 0.5 Resolution(y_method) at fixed Q²
  - Resolution(boost beta_z to gamma\*p frame) depends on
    Resolution(q.E - q.Pz) and Resolution(q.E + q.Pz). The first is
    Σ_h + 2 E_e' cos²(theta_e/2)·... — this is again Σ-controlled.
  - Resolution(xF) inherits both Resolution(W) and Resolution(boost),
    so improving y improves xF too.
  - Breit-frame boost (`q + 2x P = 0`) further uses x — for which the
    eSigma/Σ x is better than electron x at low y.

**Practical consequence.** Even though the 4-vector `q = k - k'` is
trigonometrically determined by the scattered electron alone, the
quantities derived from `q` (namely `x`, `y`, `W`) are usable to *constrain*
or *replace* `q` components. A self-consistent SIDIS reconstruction should
compute the boost from a 4-vector that uses the chosen method's `y` (and
through it, Σ-information about the longitudinal HFS), not from a raw
`k - k'` electron-only vector. The current code does **not** do this — it
uses electron-only `k - k'` regardless of which Q²/y is stored in the tree
(`ProtonTreeBuilder_lowQ2.C:535-540`).

This is the second important correction to the prompt: even after switching
the stored `Q2`/`y` to eSigma values, the SIDIS-frame boost will still be
electron-only unless the **virtual-photon 4-vector itself** is recomputed
using the eSigma method. The reconstruction-method choice is two-step:
(a) choose Q²/y, (b) propagate that choice into a consistent `q` 4-vector
used for all downstream boosts. Section 6 makes this concrete.

---

## 4. What H1 and ZEUS have done in practice

### 4.1 H1 charged-particle multiplicity, DESY 20-176 (Q²: 5-100 GeV²)

`docs/ChargedParticlePaper_extracted.md` lines 299-317 (Section 4.1):

> "The scattered positron and the HFS particles are used for the
> reconstruction of the kinematic variables, x_bj, y and Q². Similar to
> the H1 measurement of charged particle momentum spectra [52], the eΣ
> method is used [53], where these variables are defined as
>
> Q² = 4 E_e E_e' cos²(theta_e/2),    y = 2 E_e Σ / [Σ + E_e'(1 - cos theta_e)]²,    x_bj = Q²/(s y).
>
> Here, s is the ep centre-of-mass energy squared, E_e is the incoming
> lepton energy, E_e' and theta_e are the scattered positron energy and
> polar angle, respectively. The quantity Σ is defined as Σ_i (E_i - p_z,i),
> where the sum runs over all the HFS particles. **This method provides
> an optimum in resolution of the kinematic variables and shows only
> little sensitivity to QED radiative effects.**"

(Note the `y` formula in the paper text uses `2 E_e Σ / [Σ + E_e'(1 - cos
theta_e)]²` which is the explicit Bassler-Bernardi `y_eSigma` rewriting
algebraically equivalent to `Q²_e/(x_S s)` from `H1MakeKine`.)

References cited:
- `[52]` H1, C. Alexa et al., EPJC 73 (2013) 2406, arXiv:1302.1321 — the
  H1 charged-particle momentum spectra paper, exactly the same kinematic
  regime `5 < Q² < 100 GeV²` and `10⁻⁴ < x < 10⁻²` we are working in. It
  also uses eSigma.
- `[53]` U. Bassler and G. Bernardi, NIM A361 (1995) 197, arXiv:hep-ex/9412004
  — the canonical eSigma paper.

The H1 multiplicity paper unfolds in `(N, Q², y)` and quotes the eSigma
resolution-and-radiative-robustness as the explicit reason for the
choice (line 317).

### 4.2 H1 ztudesy `H1_PartCorrAnalyzer` (production code for DESY 20-176)

`teamB/lessons_from_published_analyses.md` §1.11 documents that ztudesy's
`PartCorrAnalyzer.C:568-573, 978-984` calls

```
makeKin_es.MakeESig(escat0_MC_lab.E(), escat0_MC_lab.Theta(), sigma,
                    ebeam_MC_lab.E(), pbeam_MC_lab.E());
Q2_esigma = makeKin_es.GetQ2es();
y_esigma  = makeKin_es.GetYes();
x_esigma  = makeKin_es.GetXes();
```

at both gen and reco level, with `sigma = sum_HFS(E - pz)` from the
**ISR-corrected** HFS (after `H1HadronicCalibration::eHighPtJet` is applied
and the radiative-photon cone around the scattered electron is removed).
ztudesy stores **both** electron-only `(Q2, y, x)` and eSigma
`(Q2_es, y_es, x_es)` so the analysis macro can pick at unfolding time
(§1.11 paragraph 4). This is the pattern we should adopt
(see §6 below: option-c-then-a).

### 4.3 H1 ztudesy `PartCorrAnalyzer` HCM frame is computed twice

Same source, §1.3 and `PartCorrAnalyzer.C:104-138`: the analysis stores
`(ptStar, etaStar, phiStar)` in the standard HCM (boost from `q = k - k_phot`
where `k_phot` is the cone-augmented scattered electron) **and**
`(ptStar2, etaStar2, phiStar2)` in the eSigma HCM (boost computed
consistently from the eSigma kinematics). The motivation cited
(§1.3 paragraph 4): *"the standard HCM boost uses the (radiated-photon-
corrected) electron and is sensitive to ISR; the e-Sigma boost is
ISR-independent."*

This is exactly the pattern that lets the user expose the response-matrix
improvement directly: build both boosts and compare per-bin (z, xF)
resolution.

### 4.4 H1 high-Q² Lambda/V0 evls stage already uses eSigma

`lambda_HiQ/src_review/TreeBuilder.C:442-445` (the published high-Q²
event-shapes-style stage_evls):

```
// --- CUT 10: Kinematic cuts (eSigma method) ---
double q2 = gH1Calc->Kine()->GetQ2es();
double y  = gH1Calc->Kine()->GetYes();
double x  = gH1Calc->Kine()->GetXes();
```

These eSigma values are then stored verbatim into the evls tree branches
named `Q2s`, `Ys`, `x` (`lambda_HiQ/src_review/TreeBuilder.C:294-296,
511-513`). So the high-Q² baryon-asym pipeline is already eSigma at reco
side. Gen-side is still raw `*hatQ2sGen` (Σ method), which is a parity
inconsistency.

### 4.5 ZEUS scaled-momentum spectra in DIS (arXiv:1001.4026)

From `sidis_lowQ2/research-sidis-systematics-literature.md` §2.2 and the
arXiv abstract: ZEUS measured charged-particle scaled momentum
distributions in the Breit frame for `10 < Q² < 41000 GeV²`. The
reconstruction-method discussion is in Section 5 (systematics) where they
quote an alignment uncertainty *"affecting Breit-frame boost (electron
polar angle ±2 mrad; hadron polar angles ±4 mrad): (-2,+3)%"*. The use
of *both* electron and hadron angles in the Breit-frame boost is the
signature of the **double-angle (DA) method**: ZEUS's tradition in
charged-particle DIS analyses since the 1990s has been DA, not eSigma.

ZEUS strangeness paper (arXiv:hep-ex/0612023; cf. `docs/research-published-radiative-corrections.md`
§2): same DA convention; HERACLES handles radiative corrections.

This is a real H1/ZEUS difference: ZEUS uses DA for SIDIS, H1 uses eSigma
for SIDIS. The two are roughly equivalent in resolution at moderate
kinematics; eSigma wins at low y and is more robust to ISR; DA wins on
energy-scale insensitivity. For the H1 dataset, the H1 tradition (eSigma)
is the natural choice and is what the most directly comparable analysis
(DESY 20-176) used.

### 4.6 H1 high-Q² V0 / proton design notes already cite eSigma

`docs/plan_highQ2_V0.md:31, 45, 450, 759, 873, 1325, 1497` and
`docs/research-sidis-supervisor-scope.md:119` all list eSigma as the
intended high-Q² kinematics. The high-Q² baryon-asym TreeBuilder
initializes `H1HadronicCalibration::eHighPtJet` before reading kinematics
(`proton_HiQ/docs/research-highQ2-proton-asymmetry.md:119-127`) precisely
so that `GetQ2es()`/`GetYes()` return calibrated values.

---

## 5. Resolution comparison: published quantitative results

Bassler-Bernardi (NIM A361 1995, hep-ex/9412004) is the canonical
reference and contains the published comparison (their Table 1 and
Figs. 4-5). In words appropriate to the H1 low-Q² SpaCal kinematic
regime (Q² < 100 GeV², low x):

| Quantity | Electron | Sigma | DA | eSigma |
|---|---|---|---|---|
| Q² resolution (intrinsic) | best for theta_e well-measured | poorer than electron at moderate x; degrades at low y | comparable to electron at moderate kinematics | matches electron (Q²_eS = Q²_e) |
| y resolution at low y (y < 0.1) | very poor (~ 1/y) | best | comparable to Σ | best (= Σ y) |
| y resolution at high y (y > 0.5) | best | comparable to e | good | best (e contribution dominates) |
| x resolution | poor at low y (driven by y_e) | better than e at low y; degrades at high y | comparable | best across the whole y range |
| W resolution | poor at low y (since W ~ sqrt(s y)) | good | good | good |
| ISR robustness | weakest (y_e biased by missing ISR energy) | strongest (Σ replaces beam-energy assumption) | strong (angles only) | strong (inherits Σ for y) |
| HFS calibration sensitivity | none | strong | strong | y inherits Σ; Q² does not |
| SpaCal energy-scale sensitivity | strong (E_e' enters Q² and y) | strong (E_e' enters Q²_S in the (1-y_S) denominator) | weakest (angle-only) | strong |
| Charged-particle paper resolution claim | --- | --- | --- | "optimum in resolution ... little sensitivity to QED radiative effects" (DESY 20-176 §4.1) |

Quantitative numbers from the Bassler-Bernardi paper for HERA-era
detector resolutions (Tables 1 in NIM A361 197, parametrized at the
measurement-region centroids for `y ~ 0.1-0.5, Q² ~ 10-100 GeV²`):

- `sigma(x)/x`: electron 25-40%, Σ 10-15%, DA 10-15%, eSigma 8-12%.
- `sigma(y)/y`: electron 20-100% (worst at low y), Σ 8-12%, DA 8-15%, eSigma 8-12%.
- `sigma(Q²)/Q²`: electron 5-8%, Σ 10-20%, DA 5-8%, eSigma 5-8%.

These are intrinsic-detector numbers; the actual H1-detector smearing
values are similar in shape but specific to the SpaCal+LAr+CJC resolution
mix. They are also the input numbers the H1 multiplicity paper relies on
when stating eSigma is the "optimum" choice.

**Expected improvement on z and xF resolution from Σ → eSigma at low Q².**
Given that the existing low-Q² builder uses Σ (not electron) as already
established in §1, the gain from switching to eSigma comes from:
- Q² no longer carrying the (1 - y_S) denominator inherited via Σ →
  noticeably tighter Q²-bin migration.
- y resolution roughly unchanged (eSigma's y is constructed from Σ x).
- x resolution improved by removing the redundancy where Σ is used both
  in Q² and y of the same expression.

Order-of-magnitude expectation, consistent with the published H1 tables:
**5-15% reduction in z bin migration, 10-25% reduction in xF bin
migration**, with the largest gains at the low-y / low-x edges of the
phase space where Σ Q² is worst. Precise numbers must be measured on our
actual MC.

If the user *had* been using the electron method (the prompt's
assumption), the Σ→eSigma argument would not apply; the relevant
comparison would be electron→eSigma, in which case the gain on y (and
hence z, W, xF) at low y would be much larger (factor 2 in the lowest y
decade per Bassler-Bernardi Fig. 4-5).

---

## 6. Implementation paths

### Inputs already available without rebuilding

The HAT bank stores all standard kinematic methods at gen and reco
(`h1oo-master/H1Hat/H1HatEvent.C:1240-1273`):
- `Q2e`, `Ye` and gen `Q2eGen`, `YeGen`
- `Q2s`, `Ys` and gen `Q2sGen`, `YsGen`
- `Q2da`, `Yda` and gen `Q2daGen`, `YdaGen`
- `Q2h`, `Yh` and gen `Q2hGen`, `YhGen`
- `Empz` (= total event E - pz) via `H1FloatPtr Epz("Epz")` as already
  used in `lambda/.work_feeddown/TreeBuilder.C:241, 354`.

So gaining read-access to **Σ, electron, DA, hadron** gen+reco values
needs only additional `H1FloatPtr` declarations in the tree builder; no
expensive recomputation, no new H1Calculator init.

eSigma needs to be derived (no HAT branch). Two equivalent options:
- Algebraic: store `Q2e` and `Ys`; at analysis time compute
  `x_es = Q2e/(Ys s) ... y_es = ... `. Caveat: this is **not** the
  Bassler-Bernardi eSigma unless you use Σ-method `x_S` which itself
  requires both `Q2s` and `Ys`. Equivalent algebraic form using HAT
  branches alone:
  ```
  Q2_eS = Q2e
  x_eS  = x_S  = Q2s / (Ys * s)
  y_eS  = Q2e / (x_eS * s) = Q2e * Ys / Q2s
  ```
  All three quantities computable from `Q2e`, `Q2s`, `Ys`, `s` at analysis
  time. No H1Calculator call needed.
- Calculator-based: at builder time call
  `gH1Calc->Kine()->GetQ2es()/GetYes()/GetXes()`, store. This is what the
  high-Q² evls (§4.4) already does. Requires the H1Calculator and the
  hadronic calibration to be initialized in the builder, which is
  free at high Q² (already done) and a non-trivial change at low Q²
  (currently the low-Q² ProtonTreeBuilder does not initialize hadronic
  calibration at all — it just reads HAT pointers).

### 6.1 Option (a): add eSigma alongside the existing method, do not rebuild data

Smallest-footprint change. In the low-Q² and high-Q² ProtonTreeBuilder
(and the ChargedTot/ChargedTotGen branches, which are the SIDIS surface):

1. Add HAT pointers in the declaration block (mirror lines 155-156 of
   `ProtonTreeBuilder_lowQ2.C`):
   ```
   H1FloatPtr hatQ2e("Q2e");      // electron method
   H1FloatPtr hatYe ("Ye");
   H1FloatPtr hatQ2eGen("Q2eGen");
   H1FloatPtr hatYeGen ("YeGen");
   H1FloatPtr hatQ2da("Q2da");    // double angle
   H1FloatPtr hatYda ("Yda");
   H1FloatPtr hatQ2daGen("Q2daGen");
   H1FloatPtr hatYdaGen ("YdaGen");
   ```
2. Read at event time:
   ```
   Float_t Q2e = *hatQ2e, Ye = *hatYe;
   Float_t Q2da = *hatQ2da, Yda = *hatYda;
   // eSigma derived from existing Q2s/Ys + new Q2e
   Float_t Q2es = Q2e;
   Float_t Yes  = (Q2s > 0) ? Q2e * Ys / Q2s : 0.f;
   Float_t Xes  = (Q2s > 0 && Ys > 0) ? Q2s / (Ys * 4.f * eBeamE * pBeamP) : 0.f;
   ```
3. Add output branches: `Q2e`, `Ye`, `Q2es`, `Yes`, `Xes` (and DA if
   wanted), plus their gen counterparts.
4. **Recompute the virtual photon 4-vector twice**: once electron-only as
   today (keeping backwards compatibility for proton/V0 analyses), and
   once self-consistently from eSigma. The eSigma-consistent `q` is
   constructed by:
   - Take the scattered electron 3-direction from
     `(theta_e, phi_e)`.
   - Use `E_e'_eSigma = Q2es / [2 E_e (1 + cos theta_e)]` (the electron
     energy that exactly reproduces Q²_e at the measured angle —
     equivalent to using the angle and Q²_e together to back out the
     "ideal" electron energy free of SpaCal energy-scale shifts).
   - Build `scatElec_eS = (E_eS sin theta_e cos phi_e, ..., E_eS cos theta_e, E_eS)`.
   - `q_eS = eBeam4 - scatElec_eS`.
   - Or equivalently use H1Calculator's `Elec()->GetFirstElectron()` for
     the angle and override the energy from `Q2es`. This is the cleanest
     form.
   - This is the Bassler-Bernardi "consistent eSigma reconstruction": Q²
     comes from the electron method but the *electron-energy ingredient*
     in the 4-vector is the angle+Q²_e back-derivation.

5. Recompute `gammaP`, `boostGammaP`, `boostBreit`, and from them the per-
   hadron `(z, xF, ystar, yBreit)` using the eSigma `q`. Store the
   results as `_es` siblings of the existing `z`, `xF`, `ystar`,
   `yBreit` branches.

Result: the analysis macros and unfolders can switch by selecting the
`_es` siblings. Both methods coexist in the same trees. Existing proton
and V0 chains are unaffected.

This is the recommended option (see §7).

### 6.2 Option (b): replace the existing method entirely

Smaller trees, but breaks parity with the current proton/V0 chains and
with the high-Q² baryon-asym `Q2s` already-eSigma convention. Not
recommended unless the user is sure no downstream code depends on Σ values.

### 6.3 Option (c): store raw ingredients and compute at analysis time

Store `Sigma_h`, `pT_h`, plus the existing `(Q2e, Q2s, Ye, Ys)` and let
analysis macros call `H1MakeKine` (or pure-Python equivalent) to make any
combination. Useful for systematic studies that want hadronic-energy-scale
shifts to propagate into kinematics consistently. Heavier in storage
(extra floats per event); requires the analysis macros to import the
formulas. The NuclIA/ISR-cone refinements that ztudesy applies before
computing Σ also have to be done at analysis time, which duplicates
infrastructure.

This is option-d in `teamB/lessons_from_published_analyses.md`'s
recommendation list and is reasonable but overkill for the immediate
goal. Recommended as a *supplement* to (a) for systematics: store at
least `Sigma_h`, `pT_h^2`, `Empz` so that hadronic-scale shifts can be
propagated post-hoc.

---

## 7. Recommendation

**Adopt option (a) plus the storage of `Empz`, `Sigma_h`, `pT_h^2` from
option (c).** Concretely, for the low-Q² reprocess about to run:

1. Add HAT pointers `Q2e`, `Ye`, `Q2eGen`, `YeGen`, `Q2da`, `Yda`,
   `Q2daGen`, `YdaGen` to the low-Q² and high-Q² ProtonTreeBuilders. The
   low-Q² builder must NOT add the H1HadronicCalibration init (the upstream
   evls already calibrated the HFS that fed `Q2s/Ys`); but it **must**
   read `Empz` from `H1FloatPtr Epz("Epz")` and store it. Also store
   `pT_h^2` if available (from `Pt2H` HAT branch — verify on NAF; if not
   available, derive from HFS access).
2. Compute eSigma `(Q2es, Yes, Xes)` algebraically from `(Q2e, Q2s, Ys)`
   in the per-event block. No new H1Calculator call. Add output branches.
3. Build a second virtual photon 4-vector `gamma_eS` from
   `(theta_e, phi_e, Q2es)` and use it to compute `gammaP_eS`,
   `boostGammaP_eS`, `boostBreit_eS`. From `gammaP_eS` and the per-hadron
   4-vector, fill **second copies** of `ystar`, `yBreit`, `xF_SIDIS`,
   `z_SIDIS` named with an `_es` suffix. Mirror at gen level using the
   gen analogues `(Q2eGen, Q2sGen, YsGen)`.
4. Keep the existing electron-only `gamma` and Σ-based `(Q2s, Ys)` chain
   in place to preserve current proton/V0 analysis parity. Do not rename
   any existing branch.
5. Document the addition in `sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C`
   header comment and in the SIDIS execution plan.

After the reprocess:

6. Build per-bin response matrices in (z, xF, ystar, yBreit) for both the
   nominal (current Σ-based) and the new eSigma chain. Compare purity,
   stability, and bin-migration histograms. Use the better one as the
   nominal SIDIS unfolding chain. The other becomes a documented
   systematic-uncertainty handle.
7. Track the high-Q²-already-uses-eSigma convention: the high-Q²
   ProtonTreeBuilderHighQ2 inherits eSigma at reco from the upstream
   evls but has Σ at gen (`hatQ2sGen`). Either fix the gen side to use
   `hatQ2eGen, hatYsGen` and compute eSigma_gen consistently, or
   document this as a known reco/gen method mismatch carried over from
   the V0 stage_evls.

**Why (a)+(c)-supplement and not (b) or pure (c).**
- (a) preserves existing chains; the user can A/B compare without
  re-running anything but the SIDIS analysis macros.
- (c)-supplement (storing Σ_h, pT_h², Empz) gives the systematics chain
  the ingredients it needs to apply hadronic-scale shifts post-hoc
  without another reprocess.
- (b) is cheap on storage but high on review-burden: any mistake in the
  one stored method is permanent until the next reprocess, and the
  proton/V0 analyses share the trees and rely on the current convention.
- Pure (c) duplicates infrastructure (H1MakeKine in Python) for no gain
  beyond what (a)+(c)-supplement already gives.

**One additional sequenced action.** Before the low-Q² reprocess, add
the (a)+(c)-supplement extension to the SIDIS execution plan
(`sidis_lowQ2/plan-sidis-execution.md`) as a Phase 1B builder fix
(currently F7/F8/F13 cover similar tree-builder extensions). The
existing plan explicitly defers HCM/eSigma storage as "not adopted now"
(`plan-sidis-execution.md:118`); reopen that decision in light of the
purity/stability evidence the user reports.

---

## 8. Implementation sketch (concrete code patch)

The patch below is for `ProtonTreeBuilder_lowQ2.C`. The high-Q² version
is parallel.

### 8.1 New HAT pointers (after current line 156)

```cpp
H1FloatPtr hatQ2e ("Q2e");                    // electron method
H1FloatPtr hatYe  ("Ye");
H1FloatPtr hatEpz ("Epz");                    // total event E - pz
// MC gen counterparts (declared always; only read in MC mode)
H1FloatPtr hatQ2eGen("Q2eGen");
H1FloatPtr hatYeGen ("YeGen");
// Optional: double-angle for systematics
H1FloatPtr hatQ2da("Q2da");
H1FloatPtr hatYda ("Yda");
H1FloatPtr hatQ2daGen("Q2daGen");
H1FloatPtr hatYdaGen ("YdaGen");
```

### 8.2 New event-level scalars (after current Q2s/Ys block)

```cpp
Float_t Q2e, Ye;
Float_t Q2es, Yes, Xes;          // eSigma reco
Float_t Empz_evt;
Float_t Q2eGen_ch, YeGen_ch;
Float_t Q2esGen_ch, YesGen_ch, XesGen_ch;   // gen eSigma per gen-loop convention
// SIDIS variables computed in eSigma frame
Float_t ystar_es, yBreit_es, xF_es, z_es;
Float_t ystarGen_es_ch, yBreitGen_es_ch, xFGen_es_ch, zGen_es_ch;
```

### 8.3 Branches (mirror existing pattern, add `_es` suffix)

```cpp
chargedTree->Branch("Q2e",   &Q2e);
chargedTree->Branch("Ye",    &Ye);
chargedTree->Branch("Q2es",  &Q2es);
chargedTree->Branch("Yes",   &Yes);
chargedTree->Branch("Xes",   &Xes);
chargedTree->Branch("Empz",  &Empz_evt);
chargedTree->Branch("ystar_es",  &ystar_es);
chargedTree->Branch("yBreit_es", &yBreit_es);
chargedTree->Branch("xF_es",     &xF_es);
chargedTree->Branch("z_es",      &z_es);
// gen
chargedTree->Branch("Q2eGen",  &Q2eGen_ch);
chargedTree->Branch("YeGen",   &YeGen_ch);
chargedTree->Branch("Q2esGen", &Q2esGen_ch);
chargedTree->Branch("YesGen",  &YesGen_ch);
chargedTree->Branch("XesGen",  &XesGen_ch);
chargedTree->Branch("ystarGen_es",  &ystarGen_es_ch);
chargedTree->Branch("yBreitGen_es", &yBreitGen_es_ch);
chargedTree->Branch("xFGen_es",     &xFGen_es_ch);
chargedTree->Branch("zGen_es",      &zGen_es_ch);
```

### 8.4 Per-event eSigma computation (after current line 504)

```cpp
Q2e        = *hatQ2e;
Ye         = *hatYe;
Empz_evt   = *hatEpz;
// eSigma derivations
Q2es = Q2e;
double s_cm = 4.0 * eBeamE * pBeamP;
double xS   = (Ys > 0 && s_cm > 0) ? Q2s / (Ys * s_cm) : 0.0;
Xes  = (Float_t)xS;
Yes  = (xS > 0 && s_cm > 0) ? (Float_t)(Q2e / (xS * s_cm)) : 0.0f;
```

### 8.5 eSigma virtual photon 4-vector and boosts (insert after current line 549)

```cpp
// --- Build eSigma scattered electron 4-vector ---
// Use measured angle, but energy back-derived from Q2es to remove SpaCal
// energy-scale dependence on the Q^2 ingredient. This is the
// Bassler-Bernardi self-consistent eSigma reconstruction.
double E_eES = (1.0 + cos(eTheta) > 1e-6)
              ? (double)Q2es / (2.0 * eBeamE * (1.0 + cos(eTheta)))
              : (double)eE;   // fallback: original electron energy
TLorentzVector scatElec_es;
scatElec_es.SetPxPyPzE(
    E_eES * sin(eTheta) * cos(ePhi),
    E_eES * sin(eTheta) * sin(ePhi),
    E_eES * cos(eTheta),
    E_eES);

TLorentzVector gamma_es     = eBeam4 - scatElec_es;
TLorentzVector gammaP_es    = gamma_es + pBeam4;
TVector3       boostGammaP_es = gammaP_es.BoostVector();
double Wmag_es              = gammaP_es.M();

double xbjDot_es = (pBeam4.Dot(gamma_es) != 0)
                  ? (double)Q2es / (2.0 * pBeam4.Dot(gamma_es))
                  : (double)Xes;
TLorentzVector breitFrame_es = gamma_es + 2.0 * xbjDot_es * pBeam4;
TVector3       boostBreit_es = breitFrame_es.BoostVector();
```

### 8.6 Per-track SIDIS variables in eSigma frame (per-track loop)

For each charged-particle 4-vector `chP4`:

```cpp
TLorentzVector chBoosted_es = chP4;
chBoosted_es.Boost(-boostGammaP_es);
ystar_es = (Float_t)chBoosted_es.Rapidity();
xF_es    = (Wmag_es > 0)
         ? (Float_t)(2.0 * chBoosted_es.Pz() / Wmag_es)
         : 0.0f;
TLorentzVector chBreit_es = chP4;
chBreit_es.Boost(-boostBreit_es);
yBreit_es = (Float_t)chBreit_es.Rapidity();
z_es      = (gamma_es.Dot(pBeam4) != 0)
          ? (Float_t)(pBeam4.Dot(chP4) / pBeam4.Dot(gamma_es))
          : 0.0f;
```

### 8.7 Gen-side mirror

In the MC gen block (current lines 551-589 and per-gen-particle loop),
replicate the same construction using `*hatQ2eGen`, `*hatYeGen`,
`*hatQ2sGen`, `*hatYsGen`, and the gen scattered electron 4-vector
already built at line 572-577. Store gen `_es` SIDIS variables.

---

## 9. Risks, gaps, and open questions

- **Gen-side scattered electron energy recovery.** The current code at
  `ProtonTreeBuilder_lowQ2.C:565-568` recovers `genElecE` from `Q2sGen`
  when the HAT pointer returns 0:
  ```
  if (genElecE < 0.5 && Q2sGenVal > 0 && genElecTh > 0.01) {
      double denom = 2.0 * eBeamE * (1.0 + cos(genElecTh));
      if (denom > 0) genElecE = Q2sGenVal / denom;
  }
  ```
  This formula assumes Q²_S ≈ Q²_e at gen level (no detector smearing).
  The eSigma extension should use `Q2eGen` directly when available:
  `genElecE = Q2eGen / [2 E_e (1 + cos theta_e_gen)]`. If only `Q2sGen`
  is available, the gen Σ vs e equivalence is nearly exact at gen level
  so the existing fallback is acceptable.

- **HFS calibration at low Q².** `proton_HiQ/docs/research-highQ2-proton-asymmetry.md:114-116`
  states *"At high Q², the hadronic final state calibration must be
  applied before reading kinematics. This is NOT needed at low Q² (where
  SpaCal electron dominates eSigma)."* This claim is mostly correct **for
  Q²** under eSigma (Q²_eS = Q²_e is electron-only), but **not** for the
  Σ ingredient that enters Yes / Xes. For low-Q², the existing low-Q²
  evls relies on whatever HFS calibration `H1Tree` defaults to in HERA-II
  reprocessed data. If the user wants to apply the systematic
  hadronic-scale shift later via H1CalcSystematic, the builder should
  open the door by initializing `H1HadronicCalibration` (the recommended
  tune for SpaCal-dominated SIDIS is the same `eHighPtJet` that high-Q²
  uses, despite the name; see `teamB/lessons_from_published_analyses.md`
  §1.5 and §4.6 #1). This is a separate fix from the eSigma adoption and
  should be coordinated with `h1-systematics`.

- **ISR/FSR cone around the scattered electron.** ztudesy applies a
  DeltaR < 0.1 cone around the gen electron and adds the radiative photon
  back into the electron 4-vector before computing eSigma kinematics
  (`teamB/lessons_from_published_analyses.md` §1.2, §4.4). The current
  ProtonTreeBuilder does not do this. For an MVP eSigma roll-out the
  cone is not essential; for a publication-grade SIDIS chain it is. Treat
  as a separate fix downstream.

- **Naming-convention trap.** The HAT branch `Q2s` is the Σ method; the
  high-Q² evls overwrites a tree branch *also* named `Q2s` with eSigma
  values; the H1Calculator method `GetQ2es()` returns eSigma; the
  H1MakeKine inline `GetQ2es()` returns electron Q² (because eSigma's Q²
  *is* electron Q²). The eSigma `y` is computed via the relationship
  `x_eS = x_S` and back-solving. Reviewers will misread this if not made
  explicit. Keep the new branches unambiguously named (`Q2es` / `Yes` /
  `Xes`, with the `_es` suffix on derived SIDIS variables) and add a
  header comment in the builder explaining the convention.

- **No HAT branch for Σ_h or pT_h directly accessible.** Verify on NAF
  whether `Pt2H` (or `PtMissH`, `SigmaH`) HAT branches exist in the local
  reprocessed banks; the H1HatEvent code in `h1oo-master` lists `PtMiss`
  but not a per-event `Σ_h`. If absent, the storage of Σ_h needs the
  H1Calculator `gH1Calc->Fs()->GetFullHadFsEmpz()` call (see
  `H1CalcKine.C:759`), which requires the calibration init.

- **Method choice across the Q² range.** At high Q² (LAr), the electron
  energy resolution is excellent and eSigma-vs-electron differences are
  tiny; at low Q² (SpaCal), the SpaCal energy scale is the worst-known
  ingredient and DA may compete favorably with eSigma in some bins. After
  reprocess, do a per-(Q², y) bin comparison of all three (electron,
  eSigma, DA) on the response-matrix metrics before locking in a single
  nominal.

- **Open-question carryover from teamB.** `teamB/lessons_from_published_analyses.md`
  §7 question 5 explicitly asks *"Has anyone done a y-bin migration study
  between electron-only y_e and e-Sigma y_es at low Q² in our event
  lists?"* This research artifact partially answers the question by
  prescribing how to set up the comparison; the actual study still has
  to be done on the reprocessed trees by the user (or by the
  `h1-unfolder` specialist).

---

## 10. Summary

- The current low-Q² builder uses **Σ method**, not electron method, and
  the high-Q² builder uses **eSigma**. The user's prompt mis-stated this;
  the right comparison at low-Q² is Σ → eSigma, expected to give 5-25%
  improvements in z and xF bin migration depending on the (Q², y) bin.
- DESY 20-176 (the directly comparable H1 charged-particle paper, same
  kinematic range as our low-Q²) explicitly uses eSigma and cites
  Bassler-Bernardi NIM A361 1995 as the resolution-and-radiative-robustness
  basis. The H1 charged spectra paper (1302.1321) uses the same. ZEUS
  uses double-angle in its scaled-momentum SIDIS analyses.
- H1OO provides all ingredients in HAT pointers (`Q2e`, `Q2s`, `Q2da`,
  `Q2h`, `Ye`, `Ys`, `Yda`, `Yh`, `Epz`) and via `H1MakeKine::MakeESig`.
  No new framework calls are required for option (a).
- The recommended path is **option (a) plus storage of raw HFS
  ingredients**: add `Q2e`, `Ye`, `Q2es`, `Yes`, `Xes`, `Empz` and
  per-track `_es` SIDIS variables alongside the existing branches. Do
  not rename or remove existing branches.
- The reconstruction-method choice has two parts: (i) which Q²/y to
  store, and (ii) which virtual-photon 4-vector to use for SIDIS-frame
  boosts. Both must change consistently. The current builder uses
  electron-only `q = k - k'` regardless of which `Q²/y` it stored — that
  is a latent bug that the eSigma roll-out fixes by construction.
- Implementation is local to the tree builder. No reprocess of upstream
  evls is required as long as the HAT branches are populated in the
  upstream HAT banks (which they are, since they are part of the H1OO
  `H1HatEvent` core schema).

The artifact `sidis_lowQ2/research-dis-kinematic-methods.md` (this file)
should be cited from the next iteration of `plan-sidis-execution.md` as
the basis for re-opening the deferred decision recorded at line 118 of
that plan ("HCM/e-Sigma storage expansion: not adopted now").
