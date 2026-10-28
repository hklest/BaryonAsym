<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

> **RESOLVED** (2026-05-16): eSigma frozen as baseline. Retained for background reference.
>
**Status:** Current research note with partial resolution applied. `eSigma` is now the frozen baseline; retain this document for future photon-reconstruction improvements beyond the current baseline.

# Optimal Combination of DIS Kinematic Methods for Virtual-Photon Reconstruction in H1 Charged-Hadron SIDIS

**Scope.** Quantify how the multiple DIS reconstruction methods stored in the
H1 SIDIS trees (electron `e`, Sigma `s`, eSigma `eΣ`, double-angle `DA`,
Jacquet–Blondel/hadron `JB`) can be combined to reduce the resolution on
the virtual-photon 4-vector `q`, and through it the resolution of the SIDIS
observables `(z, pT_h, phi_h, xF)` that drive TMD interpretation. The
research is targeted at the low-Q² regime (5–100 GeV²) where the
photon-reconstruction uncertainty dominates the SIDIS resolution.

**Sources used (file:line).**
- `sidis_lowQ2/research-dis-kinematic-methods.md` (855 lines): per-method
  formulas, naming conventions, and H1OO HAT mapping; Sections 2 and 5 in
  particular for the published comparison tables.
- `sidis_lowQ2/research-fast-systematics-methods.md` (854 lines): the
  Σ_h-recovery identity `Σ_h = Empz − E'(1 − cos θ_e)`, post-hoc 4-vector
  reconstruction, and the verification that `Empz` is the **full** event
  E−p_z (median ~ 55.2 GeV).
- `h1oo-master/H1PhysUtils/H1MakeKine.{h,C}`: the canonical formulas for
  all five methods plus the inline `GetQ2es()/GetXes()/GetYes()` getters.
- `sidis_lowQ2/plan-tmd-sidis.md`: TMD coordinate definitions
  (especially §1.1–§1.3), the photon-axis pT_h definition, the Trento
  φ_h convention, and the OQ8/Task 2B.10 photon-method freeze status.
- `sidis_lowQ2/scripts/compare_kinematic_methods.py` (654 lines): proves
  the post-hoc `gamma → boost → (z, xF)` chain works on stored branches.
- `sidis_lowQ2/scripts/tmd_resolution_acceptance.py` (1510 lines):
  template for the resolution measurement (Stage 1) where the comparison
  proposed in this file should be plugged in.

---

## 1. Executive summary

1. **All five methods are already in the trees** (`Q2e/Ye`, `Q2s/Ys`,
   `Q2da/Yda`, `Q2h/Yh`, plus the algebraic eΣ via `Q2es=Q2e`,
   `Xes=Q2s/(Ys·s)`, `Yes=Q2e·Ys/Q2s`). The full lab-frame ingredients
   (`scatEX,Y,Z,EE`, `Empz`, `BeamElecE`, `BeamProtP`) are also stored,
   so the entire DIS reconstruction can be redone offline. No reprocessing
   is required to compare or combine methods.

2. **The current SIDIS-frame boost is electron-only.** Even when the
   tree's stored `Q2/y/x` are Σ or eΣ, the per-track `(z, pT_h, xF)`
   variables are computed from `q = k − k'_measured` (i.e., the raw
   measured scattered-electron 4-vector). This is the "latent bug"
   identified in `research-dis-kinematic-methods.md` §3 and §6: the
   choice of (Q², y) and the choice of `q`-4-vector are two separate
   knobs, and only the second one drives the boost that defines
   `(z, pT_h, phi_h, xF)`.

3. **For a single-method choice, the literature is unambiguous: eΣ wins
   at H1 low Q²** (DESY 20-176, arXiv:1302.1321, Bassler–Bernardi
   NIM A361 (1995) 197). H1's most directly comparable analysis explicitly
   adopts eΣ and cites "optimum in resolution and little sensitivity to
   QED radiative effects." ZEUS uses double-angle in their scaled-momentum
   SIDIS work, but this is a ZEUS tradition, not a published demonstration
   that DA beats eΣ at H1 low Q².

4. **Combining methods can in principle improve resolution, but the
   gains are modest and most are already captured by eΣ.** The five
   methods are **strongly correlated** because three of them
   (`e`, `Σ`, `eΣ`) share the scattered-electron measurement, and four
   of them (`Σ`, `eΣ`, `DA`, `JB`) share the HFS Σ_h. Any combination
   estimator must explicitly model these correlations. For practical H1
   purposes, the **best single method (eΣ) is within 5–15% of the
   theoretically optimal combination** for `Q²/y/x` in most of the low-Q²
   plane; the scope for further improvement is therefore bounded.

5. **The largest remaining gain is on the virtual-photon DIRECTION, not
   on Q²/y.** The transverse direction of `q` is fixed by `(theta_e,
   phi_e)` of the scattered electron and cannot be improved by Σ/JB/DA
   at low Q², because the HFS angle γ_h is poorly constrained when the
   forward HFS escapes through the beam pipe (the dominant low-x, low-Q²
   regime). The longitudinal component `q.E−q.Pz` can be improved by
   the Σ identity (replace `2 E_e − E'(1−cos θ_e) (1+cos θ_e)` with
   `Σ_h`), and this propagates into `z` and `xF` through the boost.

6. **Recommended approach (in order of complexity):**
   - **Tier 0:** adopt eΣ as the nominal single method (Q²=Q²_e,
     y=y_eΣ, x=x_eΣ) and rebuild the SIDIS-frame `q` consistently from
     `(theta_e, phi_e, Q²_e)` with the electron energy back-derived as
     `E'_eΣ = Q²_e / [2 E_e (1+cos θ_e)]`. This is the "Bassler–Bernardi
     consistent eΣ reconstruction" already documented in
     `research-dis-kinematic-methods.md` §6.1.
   - **Tier 1:** add a **Σ-constrained longitudinal correction** to the
     reconstructed scattered-electron 4-vector using the longitudinal
     E−p_z balance. This is a closed-form, single-event correction that
     captures most of the ISR and SpaCal-energy-scale benefit of Σ
     while preserving the electron-only transverse direction.
   - **Tier 2 (optional):** train a simple per-event regression
     (linear, or a shallow BDT) on matched MC to predict `(Q²_gen,
     y_gen)` from the five methods; use the regressed values to build
     a refined photon. Expected gain over Tier 0/Tier 1 is small
     (a few percent at most on z/pT_h RMS) and the validation cost is
     non-trivial. Recommend only if the Tier 0/Tier 1 baseline does not
     meet the TMD release criteria.

7. **Expected resolution gains (rule-of-thumb, to be measured on our
   MC):**
   - From electron-only `q` (current code) to eΣ `q` (Tier 0):
     - log₁₀(Q²) RMS: ~0% change (Q²=Q²_e in both)
     - log₁₀(x) RMS: 10–25% improvement (x_eΣ is much better than x_e
       at low y)
     - z RMS: 10–20% improvement at low y; ~5% at high y
     - pT_h RMS: 5–10% improvement (the pT_h gain is bounded because
       the photon transverse direction is electron-only in both)
     - φ_h: comparable; the φ_h direction is electron-only in both
   - From Tier 0 to Tier 1 (Σ-constrained longitudinal correction):
     additional ~3–8% on z and xF; negligible on pT_h and φ_h.
   - From Tier 1 to Tier 2 (regression): expected <5% additional gain
     on any SIDIS variable.

8. **The simplest defendable recipe** (Section 9) is Tier 0. It requires
   **no new branches**, just an offline analysis-time recomputation
   from existing tree contents. It is the same chain that
   `compare_kinematic_methods.py:173–251` already implements for DA,
   trivially adapted to eΣ.

---

## 2. The five methods, in one place

Notation (consistent with `H1MakeKine.h:26–117` and
`research-dis-kinematic-methods.md` §2):
- `E_e` — incoming electron beam energy (HERA-II: 27.6 GeV)
- `E_p` — incoming proton beam energy (HERA-II: 920 GeV)
- `s = 4 E_e E_p ≈ 101 568 GeV²`
- `E', θ_e, φ_e` — scattered electron energy and angles (lab)
- `Σ_h ≡ ∑_HFS (E_i − p_z,i)` — HFS longitudinal balance
- `pT_h² ≡ |∑_HFS (p_x, p_y)|²`
- `γ_h` — HFS polar angle, `cos γ_h = (pT_h² − Σ_h²)/(pT_h² + Σ_h²)`
- `Empz ≡ Σ_h + E'(1 − cos θ_e)`, conserved at `2 E_e ≈ 55.2 GeV`
  in the absence of ISR (verified in
  `research-fast-systematics-methods.md` §2.1, §8 risk #1).

### 2.1 Formulas

| Method | y | Q² | x | Stored HAT branch | Reco/Gen |
|---|---|---|---|---|---|
| Electron `e` | `1 − E'(1−cos θ_e)/(2 E_e)` | `4 E_e E' cos²(θ_e/2)` | `Q²_e/(s y_e)` | `Q2e`, `Ye`, `Q2eGen`, `YeGen` | both |
| Sigma `s` | `Σ_h / Empz` | `(E' sin θ_e)² / (1 − y_s)` | `Q²_s/(s y_s)` | `Q2s`, `Ys`, `Q2sGen`, `YsGen` | both |
| eSigma `eΣ` | `Q²_e / (x_s · s) = Q²_e · y_s / Q²_s` | `Q²_e` | `x_s = Q²_s/(s y_s)` | derived offline | both |
| Double-angle `DA` | `sin θ_e (1−cos γ_h) / D` | `4 E_e² sin γ_h (1+cos θ_e) / D` with `D = sin θ_e + sin γ_h − sin(θ_e + γ_h)` | `Q²_DA/(s y_DA)` | `Q2da`, `Yda`, `Q2daGen`, `YdaGen` | both |
| Hadron/JB `h` | `Σ_h / (2 E_e)` | `pT_h² / (1 − y_h)` | `Q²_h/(s y_h)` | `Q2h`, `Yh`, `Q2hGen`, `YhGen` | both |
| Y-average `av` | `y_e² + y_DA(1−y_DA)` (`H1MakeKine.C:144`) | `4 E_e² (1−y_av)/tan²(θ_e/2)` | derived | `Q2av`, `Yav` (rare in HAT) | – |

The Y-average method is included only for completeness and is not pursued
further; it has not been used by any H1 SIDIS-style charged-particle
analysis (`research-dis-kinematic-methods.md` §2.6).

### 2.2 What each method "owns"

| Ingredient | e | Σ | eΣ | DA | JB |
|---|---|---|---|---|---|
| `E'` | Q² and y | Q² (in (1−y_s) denom); y via Empz | Q² only | – | – |
| `θ_e` | Q² and y | Q² and y (via cos in Empz) | Q² only | both | – |
| `Σ_h` | – | both | y only (via Q²_s/y_s ratio) | y (via γ_h) | y only |
| `pT_h` | – | – | – | y (via γ_h) | Q² |
| `γ_h` (≡ derived from `Σ_h, pT_h`) | – | – | – | both | – |
| `E_e`-beam assumption | both | none | none | none | y |

Key consequences:
- **Σ is the only method that does not assume the beam energy is what
  hit the vertex** (the y_s formula uses `Empz`, not `2 E_e`). This is
  the central robustness against ISR.
- **eΣ keeps Σ's ISR robustness for y** (because y_eΣ = Q²_e · y_s/Q²_s
  inherits the Σ structure of y_s) **while keeping the electron's clean
  Q²** (no HFS noise). This is why H1 chose it.
- **DA is the only method whose Q² and y are dimensionless trigonometric
  functions of two angles only** (energy-scale-free at first order). At
  low Q² this advantage is partially negated because γ_h is poorly
  measured when the forward HFS is in the beam pipe.

### 2.3 Identities relevant to combination

These hold algebraically (no detector smearing) and are useful when
re-deriving any method from the stored branches:

- Energy/momentum balance:
  `Empz = Σ_h + E'(1 − cos θ_e)`
- Σ-from-tree recovery (used post-hoc when only Empz, E', θ_e are stored):
  `Σ_h = Empz − E'(1 − cos θ_e)`
- eΣ as Σ-rescaling of Q²:
  `Q²_eΣ / Q²_s = Q²_e / Q²_s` (electron's pT² over Σ's pT²/(1−y_s))
- The Σ method enforces `y_s + Σ_e/(2 E_e) = 1` where
  `Σ_e ≡ E'(1−cos θ_e)` is the electron contribution to `Empz`.
- Cross-method correlations: any pair sharing E', θ_e, or Σ_h is
  positively correlated. The "least correlated pair" is
  `(electron Q², JB Q²)` because Q²_e uses neither Σ_h nor pT_h, and
  Q²_h uses neither E' nor θ_e.

### 2.4 The virtual-photon 4-vector versus the (Q², y) pair

The 4-vector `q = k − k'_measured` has four components fixed by the
electron measurement alone:
```
q.x = − E' sin θ_e cos φ_e
q.y = − E' sin θ_e sin φ_e
q.z = E_e − E' cos θ_e          (with E_e along −z by H1 convention)
q.E = − E_e + E'                ( = q⁰ in standard notation)
```
The transverse direction `(q.x, q.y)` is therefore **electron-only**.
Replacing the chosen (Q², y) does not change this transverse direction
unless we also re-derive a self-consistent electron energy.

For "consistent" reconstruction with method M:
- Take measured `θ_e, φ_e` (same in all methods).
- **Choose the electron energy `E'_M`** that, together with `θ_e`,
  reproduces `Q²_M` exactly: `E'_M = Q²_M / [2 E_e (1 + cos θ_e)]`.
- Build `k'_M = (E'_M sin θ_e cos φ_e, ..., E'_M cos θ_e, E'_M)`.
- Build `q_M = k − k'_M` and use it for all SIDIS-frame boosts.

This is the protocol used by `compare_kinematic_methods.py:194–213` for
DA. For eΣ, since Q²_eΣ = Q²_e, the back-derived `E'_eΣ ≡ E'_measured`
**up to Q²-only floating** (Q²_e itself is `2 E_e E'(1+cos θ_e)`, so
the back-derivation reproduces E' identically for eΣ — nothing
is gained from the back-derivation when the chosen Q² is Q²_e). The
**y choice still propagates to z, xF, ystar** through the Σ-derived
y_eΣ entering the energy/longitudinal-momentum balance of the photon
when boosting the gamma\*p frame.

This is the source of the practical eΣ advantage on z/xF: the y in the
boost is different from the y_e implicit in `q = k − k'`. To make the
choice fully consistent, the correct procedure is **either**:
- (a) accept the small inconsistency (use `q = k − k'` for direction,
  but compute z from `(P·p_h)/(P·q)` where P·q is overridden to be
  `(s/2) y_eΣ`); or
- (b) construct a synthetic photon `q'` whose E and Pz components are
  forced by the eΣ y choice via `q'.E + q'.Pz = (constant from q)` and
  `q'.E − q'.Pz = 2 y_eΣ E_e` (rearranging the Σ identity), preserving
  q'.x and q'.y from the measured electron.

Procedure (b) is the **Σ-constrained longitudinal correction** that
Tier 1 of the recommendation implements (Section 6).

---

## 3. Q1 — Per-method resolution comparison

### 3.1 What we know analytically

From Bassler–Bernardi NIM A361 (1995) 197 (= hep-ex/9412004), Tables 1
and Figs. 4–5, parameterized for HERA-era detector resolutions in the
low-x, low-Q² region (`Q² ∈ [10, 100] GeV²`, `y ∈ [0.1, 0.5]`):

| Quantity | electron | Σ | DA | eΣ |
|---|---|---|---|---|
| σ(Q²)/Q² | 5–8% | 10–20% | 5–8% | 5–8% |
| σ(y)/y at y < 0.1 | 100% (≈ 1/y blow-up) | 8–15% | 8–15% | 8–15% |
| σ(y)/y at y > 0.5 | 8–10% | 12–20% | 10–15% | 8–10% |
| σ(x)/x | 25–40% | 10–15% | 10–15% | 8–12% |
| σ(W)/W | poor (inherits y_e) | good | good | good |
| ISR robustness | weakest | strongest | strong | strong |
| HFS-scale sensitivity | none | strong | strong (via γ_h) | y inherits Σ; Q² immune |
| SpaCal scale sensitivity | strong (E' enters Q²,y) | strong (E' enters Q²_s/(1−y_s)) | weakest (angles only) | strong (Q²=Q²_e) |

These are intrinsic-detector numbers; the H1 SpaCal+LAr+CJC mix maps onto
the same shape but specific values must be measured on our MC.

### 3.2 Translation to the SIDIS variables

The decomposition (`research-dis-kinematic-methods.md` §3):
- `z = (P·p_h)/(P·q) ≈ (E_p (E_h − p_z,h))/((s/2) y)` — z RMS is
  dominated by **y RMS** (and weakly by hadron-momentum RMS, which is
  not a kinematic-method choice).
- `W = √(s y − Q² + M_p²)` — W RMS scales as 0.5 σ(y)/y at fixed Q².
- The **gamma\*p boost** β_z ≈ `q.Pz / q.E` depends on both `q.E` and
  `q.Pz`, both of which are dominated by E_e and E' for a given θ_e.
  The resolution on `q.E − q.Pz` is what the Σ method controls, and it
  enters z directly.
- The **Breit-frame boost** `q + 2 x P = 0` adds the x dependence; eΣ
  has the best x at low y.
- `pT_h` (photon-axis): magnitude of `p_h × q̂`. The transverse part of
  `q̂` is electron-only; the longitudinal part depends on q.E and q.Pz.
  For a hadron with measured `(p_x, p_y, p_z)`, the photon-axis pT_h
  RMS decomposes into:
  - hadron-tracking RMS (constant across methods, irreducible),
  - `q̂`-direction RMS (transverse: electron-only; longitudinal: method-dependent).
- `phi_h` (Trento azimuth around `q`): depends on the orientation of
  the lepton plane (electron-only) and the hadron plane. The lepton
  plane is electron-only in all methods; the only method-dependent
  piece is the slightly different `q̂` direction once the longitudinal
  component is method-corrected.

### 3.3 What the user has measured already

From the prompt:
- log₁₀(Q²) RMS = 0.053 (~12% on Q²)
- log₁₀(x) RMS = 0.084 (~21% on x)
- z RMS = 0.075 in the TMD-safe range
- pT_h RMS = 0.18–0.39 GeV depending on pT_h itself

These are the **current** numbers using whatever method the existing
`compare_kinematic_methods.py` selection produces. Per
`research-dis-kinematic-methods.md` §1, the **stored** SIDIS branches
`z_SIDIS, xF_SIDIS` use `q = k − k'` (electron-only photon), even though
Q²_s and Y_s drive the event selection. The eΣ branches `z_SIDIS_es,
xF_SIDIS_es` use the eΣ-consistent boost (after the
`ProtonTreeBuilder_lowQ2.C` extension recommended in
`research-dis-kinematic-methods.md` §8 has been merged — verify on NAF
whether this has actually happened in the current trees).

### 3.4 The per-method comparison the user should run

The mechanics are already in `compare_kinematic_methods.py`. To extend
it to all five methods with **per-method-consistent boosts**, modify
`compute_da_sidis()` into a generic `compute_method_sidis(arr, method)`
that takes one of `{e, sigma, esigma, da, jb}` and applies:

```python
def back_derived_E_prime(Q2_method, theta_e, E_e):
    one_plus_c = 1.0 + np.cos(theta_e)
    return np.where(one_plus_c > 1e-6,
                    Q2_method / (2.0 * E_e * one_plus_c),
                    np.nan)
```

then build the photon, the gamma\*p boost, and per-track `(z, pT_h^photon,
phi_h^Trento, xF)`. Bin in `(Q², y)` in the analysis grid (5–100 GeV² in
log; 0.05–0.6 in linear y) and report:

1. RMS of `log₁₀(Q²_method) − log₁₀(Q²_gen)` per (Q², y) bin
2. RMS of `log₁₀(x_method) − log₁₀(x_gen)` per (Q², y) bin
3. RMS of `(z_method − z_gen)` per (Q², y, z_gen) cell
4. RMS of `(pT_h^method − pT_h^gen)` per (Q², y, pT_h^gen) cell
5. RMS of `Δφ_h = atan2(sin(φ_h^method − φ_h^gen), cos(φ_h^method − φ_h^gen))`
6. Resolution of the photon DIRECTION measured as
   `cos α = q̂_reco · q̂_gen` per (Q², y) cell — report `1 − ⟨cos α⟩`
   or equivalently the angular RMS in milliradians.

### 3.5 Predicted ranking (to be confirmed on our MC)

Based on Bassler–Bernardi and the H1 detector mix:

For **Q²** at low Q²:
1. e ≈ eΣ (best, both ~5–8%)
2. DA (5–8% in the bulk; degrades at low y when γ_h is poorly measured)
3. Σ (10–20%)
4. JB (factor 2–3 worse than e)

For **y** at low y (y < 0.1):
1. eΣ ≈ Σ ≈ DA (8–15%)
2. JB (similar to Σ but no electron cross-check)
3. e (catastrophic, ~1/y blow-up)

For **y** at high y (y > 0.5):
1. eΣ ≈ e (~8–10%)
2. DA (10–15%)
3. Σ (12–20%)
4. JB (~Σ)

For **virtual-photon DIRECTION**:
- The transverse direction is electron-only in `e, Σ, eΣ` and is
  influenced by γ_h in DA and JB. At low Q² where γ_h has large RMS,
  **DA and JB are worse** on direction, not better.
- The longitudinal direction `q.Pz` is best constrained by methods that
  use Σ (which fixes `q.E − q.Pz = (s/2) y_method`).
- Net: **eΣ is best on direction at low Q²** by a small margin over e.

For **photon-axis pT_h**:
- The dominant contribution is hadron-tracking RMS, which is method-
  independent. The method-dependent part is the `q̂` direction RMS.
- Predicted ranking: **eΣ ≈ e (best)**, then DA, then Σ ≈ JB
  (worse because their `q̂` longitudinal direction depends on Σ_h,
  which has degraded resolution near y → 0 where Σ_h → 0).

For **z**:
- z RMS scales with y RMS for fixed hadron P. Predicted ranking:
  **eΣ (best at all y)**, then Σ at low y / DA at moderate y,
  then e (catastrophic at low y).

---

## 4. Q2 — Combination strategies

### 4.1 What "combining" means

The five methods are not five independent measurements of `(Q², y)`.
Three of them (`e, Σ, eΣ`) share the scattered electron 4-vector; four
(`Σ, eΣ, DA, JB`) share Σ_h. So the algebraic dimension of the space
spanned by the five methods is at most **3** (the three "primitives":
`E', θ_e, Σ_h`, supplemented by `pT_h` for DA/JB and `γ_h` derived
from Σ_h and pT_h). Storing all five method values is convenient
bookkeeping; it does **not** add five independent constraints.

This means the canonical "combine N independent measurements with
inverse-variance weights" formula does **not** apply directly. Any
combination must be derived in **primitive space** (E', θ_e, Σ_h, pT_h)
and then projected back to (Q², y).

### 4.2 Strategy A — Inverse-variance weighted average of (Q², y)

**Form.** `Q²_comb = (∑ Q²_m / σ_m²) / (∑ 1/σ_m²)`, similarly for y.

**Why it is wrong as written.** The five Q²_m are correlated. A
correct weighted average needs the full 5×5 covariance:
`Q²_comb = (1ᵀ V⁻¹ Q²_vec) / (1ᵀ V⁻¹ 1)`. The covariance matrix V is
**not diagonal** and must be measured on MC.

**Why it can still help.** Even with correlated inputs, the
covariance-weighted estimator is the BLUE (Best Linear Unbiased
Estimator). On our MC the covariance matrix can be measured per (Q², y)
bin from `Cov[(Q²_m^reco − Q²_gen), (Q²_n^reco − Q²_gen)]`. The BLUE
weights `w = V⁻¹ 1 / (1ᵀ V⁻¹ 1)` are 5-vectors that can be tabulated
per bin and applied at analysis time.

**Cost vs benefit.**
- Cost: per-(Q², y) bin tabulation of two 5×5 covariance matrices (one
  for Q², one for y); store as JSON; apply at analysis time.
- Benefit: in the limit that one method dominates (eΣ for Q² in most
  of the plane), BLUE returns ~100% weight on that method. Gains over
  pure eΣ are expected to be ≲ 5% in σ(Q²)/Q² and σ(y)/y in the bulk
  of the plane; possibly 10–20% near the (low-y, low-Q²) corner where
  no single method dominates.

**Recommendation.** Implement and benchmark, but **do not promote to
nominal** unless gains exceed 10% in a kinematic region of interest.

### 4.3 Strategy B — χ² minimization in primitive space

**Form.** Treat `(E', θ_e, Σ_h, pT_h)` as the four primitives. Each
method gives a constraint among them:
- e:    `Q²_e_meas = 2 E_e E' (1 + cos θ_e)`
- s:    `Y_s_meas = Σ_h / (Σ_h + E'(1−cos θ_e))`,
        `Q²_s_meas = (E' sin θ_e)² / (1 − Y_s_meas)`
- da:   `cos γ_h = (pT_h² − Σ_h²)/(pT_h² + Σ_h²)`,
        `Q²_da_meas, Y_da_meas` as above
- jb:   `Y_h_meas = Σ_h/(2 E_e)`,
        `Q²_h_meas = pT_h²/(1 − Y_h_meas)`

**Procedure.** For each event, given the measured (E', θ_e, Σ_h, pT_h)
and their resolutions, construct
```
χ²(E', θ_e, Σ_h, pT_h) = ∑_i [(measured_i − model_i)/σ_i]²
```
and minimize. This produces a self-consistent (Q², y) per event.

**Caveat.** With four primitives and exactly four direct measurements
(E', θ_e, Σ_h, pT_h) plus their derived combinations, the χ² has zero
degrees of freedom **if** we already trust the primitives. The χ² is
non-trivial only when we have measurement noise on each primitive **and**
some redundancy; the redundancy comes from the conserved quantity
`Empz` (which the primitives must satisfy: `Empz_meas = Σ_h +
E'(1−cos θ_e)` should equal `2 E_e` modulo ISR).

**This means χ² minimization is equivalent to enforcing the longitudinal
balance constraint** `Empz_meas = 2 E_e`, which is a **single constraint**
on the four primitives. The result: a one-parameter family of
self-consistent solutions, projected onto the most likely point given
the resolutions.

**This collapses to Strategy D (Σ-constrained longitudinal correction)
in the limit where the only redundancy is Empz balance.** See §6.2.

### 4.4 Strategy C — Event-by-event method selection

**Form.** Per event, pick one method based on which kinematic region
the event sits in. E.g.:
- y > 0.3: use eΣ
- y < 0.3 and Q² > 50: use eΣ
- y < 0.3 and Q² < 50: use Σ or DA
- ISR-tagged events (Empz < 45 GeV): use Σ

**Pros.** Captures the "best method per region" intuition; trivial to
implement; no covariance machinery needed.

**Cons.** Discontinuous; the bin boundaries induce kinks in the
acceptance and response matrices that are hard to handle in unfolding.
Also, "the best method" is only a few-percent improvement over eΣ
across most of the plane (Bassler–Bernardi Tab. 1), so the operational
gain is small.

**Precedent.** Used informally for inclusive-DIS cross-section
measurements (e.g., the H1 high-Q² DIS analyses use DA above some Q²
and eΣ below). For SIDIS, no published H1 charged-particle analysis has
used event-by-event selection; they all pick a single nominal method.

**Recommendation.** Not preferred. The discontinuity is harmful for
multidifferential acceptance corrections.

### 4.5 Strategy D — Σ-constrained longitudinal correction (Tier 1)

**Form.** Use eΣ for (Q², y, x) (Tier 0). For the SIDIS-frame photon,
keep the measured electron transverse direction `(p_x, p_y)` of `k'`
unchanged, and **rescale the longitudinal component** of `k'` to enforce:
```
Σ_h_event ≈ 2 E_e − E'_corr (1 − cos θ_e_corr)
```
i.e., the measured Σ_h should be exactly the eΣ y_s up to the chosen
y_eΣ. Algebraically (preserving θ_e and φ_e from measurement, but
rescaling E'):

```
y_eΣ given.
(s/2) y_eΣ = E_p (q.E − q.Pz) = E_p (E_e − E' cos θ_e + E_e − E')   [using q.E = -E_e+E']
              = E_p [2 E_e − E' (1 + cos θ_e)]                          [signs: see §2.4]
```

Wait — that gives `Q²_e/(s y_eΣ) = x_eΣ = x_s` constraint. Re-derive
cleanly:

`(P·q) = E_p (E_e + E_e cos θ_q...)` is messy in lab. The cleanest
form is to use the longitudinal balance directly:

```
δ_e ≡ E'(1 − cos θ_e) = E_pz contribution of electron
δ_h ≡ Σ_h = E_pz contribution of HFS
δ_e + δ_h = Empz
```

The eΣ identity says `y_eΣ = δ_h_corrected / Empz_corrected`. To force
a chosen y_eΣ, fix `δ_e_corr = (1 − y_eΣ) · Empz_corr` and
`δ_h_corr = y_eΣ · Empz_corr`. The simplest choice: fix
`Empz_corr = 2 E_e` (no ISR), then:
```
δ_e_corr = (1 − y_eΣ) · 2 E_e
E'_corr  = δ_e_corr / (1 − cos θ_e)
```
This is the **ISR-corrected** scattered-electron energy — exactly the
electron energy we would have measured if there were no ISR.

Then build:
```
k'_corr = (E'_corr sin θ_e cos φ_e, ..., E'_corr cos θ_e, E'_corr)
q_corr  = k_beam − k'_corr
Q²_corr = 2 E_e E'_corr (1 + cos θ_e)        # may differ from Q²_e
```

This is **almost** eΣ in the no-ISR limit (`Empz = 2 E_e` already).
When there is ISR (Empz < 2 E_e), the correction puts the missing
longitudinal energy back into the electron, which is what eΣ
implicitly assumes anyway.

**Important refinement.** The above construction may shift Q²_corr away
from Q²_e by the amount of ISR-induced E' miscalibration. In an ISR
event with measured E' too low by δE', the correction increases E' by
roughly δE' and Q² by ~ 2 E_e δE' (1 + cos θ_e). This is the desired
behavior: it removes the ISR bias from Q² as well as from y.

**Variant (preserve Q²_e).** If preserving Q²_e is desired (the eΣ
convention), apply the longitudinal correction only to the photon
4-vector used in the boost, not to the Q²/y/x stored values:
```
# Stored values (eΣ convention):
Q² = Q²_e_measured
y  = y_eΣ_derived
x  = x_eΣ_derived

# Boost 4-vector (Σ-constrained):
E'_corr = (1 − y) · Empz_measured / (1 − cos θ_e)
k'_corr = (E'_corr sin θ_e cos φ_e, ..., E'_corr cos θ_e, E'_corr)
q_corr  = k_beam − k'_corr
```
This is the "two-step decoupling" suggested by
`research-dis-kinematic-methods.md` §3.

**Expected gain.** This procedure recovers the longitudinal degree of
freedom of the photon that is mishandled by `q = k − k'_measured` in
events with ISR. The fraction of events with significant ISR
(Empz < 50 GeV) is ~10–20% at low y. For those events, the gain on z
and xF is large (factor 2 reduction in tail); for non-ISR events the
correction is a no-op. Net effect on the **inclusive** RMS of z:
expect 3–8% improvement on top of Tier 0.

### 4.6 Strategy E — Regression / ML

**Form.** Train a regressor `f(Q²_e, Y_e, Q²_s, Y_s, Q²_da, Y_da,
Q²_h, Y_h, E', θ_e, Σ_h, pT_h, φ_e, Empz) → (Q²_gen, y_gen)` on
matched MC. Use the predicted (Q², y) at analysis time, and back-build
`E'_pred` from Q²_pred and θ_e to get the photon 4-vector.

**Choices.**
- Linear: `Q²_pred = ∑ a_i x_i + b`, with coefficients fit by ridge
  regression. Interpretable; trivial to validate; gains capped at the
  BLUE limit (Strategy A).
- Shallow BDT (~ 50 trees, depth 4): captures non-linear effects
  (e.g., the y → 0 catastrophe of y_e is naturally handled by the BDT
  switching to y_s in that region). Gains over BLUE: typically a few
  percent in RMS, sometimes more on tails.
- Deep NN: overkill for this problem; not recommended.

**Pros.** Captures the right method-switching behavior automatically;
no need to hand-tune BLUE bins.

**Cons.** Adds ML dependency to the SIDIS analysis chain, which is a
review/reproducibility burden (model artifact, training script,
hyperparameter freeze). Validation requires model-independence checks
(train on RAPGAP, test on DJANGOH) which add review work.

**Precedent.** No H1 SIDIS analysis has used ML for kinematic
reconstruction. ZEUS has experimented with NN-based reconstruction in
unrelated contexts. The H1 single-jet measurements use ML in **NNUF**
unfolding but not in DIS reconstruction.

**Recommendation.** Defer to Tier 2. Implement only if Tier 0 + Tier 1
benchmarks show a clear deficit relative to the TMD release goals.

### 4.7 Cross-method correlation matrix to measure first

Before any combination is implemented, the user should measure the
following 5×5 matrices on matched MC:

```
C_Q2[m,n] = Cov[(log Q²_m^reco − log Q²_gen), (log Q²_n^reco − log Q²_gen)]
C_y [m,n] = Cov[(log y_m^reco − log y_gen),   (log y_n^reco − log y_gen)]
```

(Use log space because the relative resolution is the natural
quantity.) These matrices are:
- the inputs to BLUE (Strategy A);
- the diagnostic for whether a regression (Strategy E) can do better
  than BLUE;
- the empirical confirmation that the methods span at most 3 effective
  dimensions (the smallest two eigenvalues of C_Q2 should be close to
  zero).

---

## 5. Q3 — Published precedent

### 5.1 H1 charged-particle multiplicity, DESY 20-176 (arXiv:2011.01812)

Same kinematic regime as our low-Q² (5 < Q² < 100 GeV², 10⁻⁴ < x <
10⁻²). Section 4.1 of the paper:

> "Similar to the H1 measurement of charged particle momentum spectra
> [52], the eΣ method is used [53], where these variables are defined
> as [...]. **This method provides an optimum in resolution of the
> kinematic variables and shows only little sensitivity to QED
> radiative effects.**"

References [52] = arXiv:1302.1321 (the H1 charged spectra paper, same
regime, also eΣ); [53] = Bassler–Bernardi NIM A361 (1995) 197.

**No combination.** The H1 multiplicity paper picks one method (eΣ)
and uses it everywhere. They do not combine methods; they do not
switch per event; they do not regress.

The production code (ztudesy `H1_PartCorrAnalyzer.C`, lines 568–573,
978–984) confirms this: `MakeESig(...)` is called once per event at
both gen and reco, and the resulting (Q²_eΣ, y_eΣ, x_eΣ) is used for
everything. They **do** store electron-only `(Q², y, x)` alongside, so
the analysis macro can A/B compare at unfolding time
(`research-dis-kinematic-methods.md` §4.2). This is exactly the storage
pattern we should adopt.

### 5.2 H1 charged-particle momentum spectra, EPJC 73 (2013) 2406 (arXiv:1302.1321)

Same conclusion: eΣ everywhere. Their systematic uncertainty
discussion (Section 5) treats electron-scale, electron-angle, and
hadronic-scale shifts via post-hoc 4-vector shifts (i.e., they
already implicitly use the analytic propagation of Section 4 of
`research-fast-systematics-methods.md`). They do not study method
combinations.

### 5.3 H1 high-Q² (proton/Lambda asym, V0 chain)

`lambda_HiQ/src_review/TreeBuilder.C:442–445` uses eΣ at reco. Gen-side
uses raw `*hatQ2sGen` (Σ). This is the documented reco/gen method
mismatch from `research-dis-kinematic-methods.md` §1. Not a combination
either; just a single-method choice with an inconsistency.

### 5.4 ZEUS scaled-momentum spectra, arXiv:1001.4026

Uses **double-angle (DA)** for the Breit-frame boost. The boost depends
on both `θ_e` (electron polar angle, ±2 mrad alignment uncertainty) and
hadron polar angles (±4 mrad). The systematic uncertainty quoted is
(−2, +3)%. ZEUS is the consistent DA-using collaboration in HERA SIDIS;
H1 is the consistent eΣ-using collaboration. Both are single-method
choices; neither group has published a combination scheme.

### 5.5 HERMES (lower √s, fixed-target electron beam)

HERMES uses a single-method electron-only kinematic reconstruction
(typical for fixed-target experiments where the proton-rest-frame
formulas are straightforward). They do not combine methods because
they only measure the electron at fixed-target kinematics; there is
no useful Σ/DA equivalent. HERMES azimuthal-modulation work
(arXiv:1212.5407) reweights MC to data for systematics
(`research-fast-systematics-methods.md` Tab. 7), not for kinematic
reconstruction.

### 5.6 COMPASS (muon beam, fixed-target)

Same situation: single-method (electron-equivalent) reconstruction,
no DA/Σ combinations because the kinematics are fixed-target.

### 5.7 TMD fitter conventions

The TMD-fit community (JAM, MAP, Pavia, artemide) does **not**
prescribe a kinematic reconstruction method. They consume the
experiment's published `(x, Q², z, pT_h)` and treat the kinematic
labels as given. What they care about is:
- the precision of the per-bin kinematic centroid (mean values);
- the bin-migration correction (which is a property of the
  experiment's reconstruction-method choice, encapsulated in their
  response matrix);
- per-bin theory flags (whether `pT_h ≪ Q`, `z` in the safe range,
  etc.).

Conclusion: no fitter group will reject an H1 release because we used
eΣ instead of, say, a BLUE combination. The experimental community's
strong precedent (H1 = eΣ, ZEUS = DA) is what the fitters expect. A
combination would be a novel choice and would require extra
documentation; it is not "what fitters want."

### 5.8 Summary of precedent

| Group | Method | Comment |
|---|---|---|
| H1 multiplicity (arXiv:2011.01812) | eΣ | Single-method; both eΣ and electron-only stored, eΣ is nominal |
| H1 charged spectra (arXiv:1302.1321) | eΣ | Single-method |
| H1 high-Q² V0/proton asym | eΣ (reco) / Σ (gen, inconsistency) | Single-method per side |
| ZEUS scaled momenta (arXiv:1001.4026) | DA | Single-method |
| ZEUS strangeness (hep-ex/0612023) | DA | Single-method |
| HERMES | electron-only (fixed-target) | Method combination not applicable |
| COMPASS | electron-only (fixed-target) | Method combination not applicable |
| TMD fitters (JAM/MAP/Pavia/artemide) | accept any | Care about resolution & metadata, not method |

**No published HERA SIDIS analysis uses a combination of methods.** The
default is single-method (eΣ for H1, DA for ZEUS) plus extensive
storage of the alternatives for systematic studies. **Our recommended
recipe (Section 9) follows that precedent for the nominal release**
and reserves combinations for cross-checks and possible future Tier 1
upgrades.

---

## 6. Q4 — Low-Q² specific challenges

### 6.1 Where the photon resolution is dominated

At Q² ∈ [5, 10] GeV²:
- Scattered electron in the **SpaCal** (backward calorimeter), which has
  energy resolution ~ 7%/√E ⊕ 1% and angular resolution ~ 1–2 mrad. The
  SpaCal energy scale is the dominant systematic (1% scale at H1).
- Electron energy E' ~ 20–25 GeV (most of the beam energy is preserved
  because Q² is small). The electron angle θ_e is large (> 150°).
- Q²_e = 4 E_e E' cos²(θ_e/2). At θ_e ≈ 170°, `cos²(θ_e/2) ≈ 0.0076`,
  so a 1% E' shift gives a 1% Q² shift; a 1 mrad θ_e shift gives a
  small Q² shift because dQ²/dθ_e ∝ −sin θ_e ≈ −0.17 at θ_e=170° —
  actually quite small for `Q² = 4 E_e E' (1 + cos θ_e)/2`, since
  `d/dθ_e(1+cos θ_e) = −sin θ_e`, giving `dQ²/Q² = −sin θ_e dθ_e /
  (1 + cos θ_e) ≈ −0.17 · 0.001 / 0.0076 ≈ −2%` per mrad — non-trivial.

So Q²_e at low Q² is sensitive to **both** SpaCal energy scale and
SpaCal angle, with comparable contributions. This is why DA (which
removes the energy-scale dependence at first order) is sometimes
preferred at low Q² — except the gain is killed by the bad γ_h
resolution (next bullet).

### 6.2 The HFS at low Q² and the γ_h problem

At low Q² and low x, most of the proton break-up products go forward
(small polar angles in the lab), with a significant fraction lost
through the forward beam pipe (θ < 5° in the LAr). Concretely:
- Σ_h ~ y · 2 E_e ∼ 5–30 GeV depending on y.
- γ_h is recovered from `cos γ_h = (pT_h² − Σ_h²)/(pT_h² + Σ_h²)`. When
  Σ_h is small (low y) and pT_h is even smaller, the numerator is
  −Σ_h² and γ_h ≈ π — but this is dominated by noise on Σ_h and pT_h.
  The published H1 numbers (Bassler–Bernardi Fig. 5) show σ(γ_h) ~ 50
  mrad at low y at low Q².
- DA inherits this γ_h RMS into Q²_DA and y_DA, which is why DA
  underperforms eΣ at low Q² in spite of its angle-only formulation.

### 6.3 ISR contamination

At low y, ISR shifts y_e systematically downward because the ISR
photon carries away beam energy that the electron-method formula
assumes hit the vertex. Σ replaces this assumption with the measured
Empz, hence Σ-method y is ISR-immune. In the H1 charged-spectra paper
this is the central reason for choosing eΣ over electron-only.

### 6.4 The dominant source of photon-DIRECTION uncertainty at low Q²

Decompose the photon direction into transverse and longitudinal:
- **Transverse** (`q_x, q_y`): set entirely by the SpaCal cluster
  position (1–2 mrad RMS at low Q²). Independent of method.
- **Longitudinal** (`q_z`, or equivalently the `q.E − q.Pz` invariant):
  set by `E'` and `θ_e`. The electron-only photon has σ(q.E − q.Pz)
  dominated by SpaCal energy scale (1%) and ISR (a few % systematic).
  The Σ-constrained photon (Tier 1) has σ(q.E − q.Pz) dominated by
  Σ_h resolution, which is ~ HFS-scale (few %) plus the small forward
  losses (typically already accounted for in the calibration).

**Net at low Q²:**
- The transverse direction is electron-only, identical across all
  methods. Its RMS sets a floor on pT_h and φ_h resolution.
- The longitudinal direction can be improved by Σ-constraint
  (Tier 1) for ISR events; for non-ISR events the gain is small.

This is the key insight for Q4: **the photon transverse direction at
low Q² is method-independent**, so the achievable improvement on pT_h
and φ_h from any combination is limited by the SpaCal angular
resolution, not by the choice of (Q², y) method.

### 6.5 What the Q² and y choices DO buy

What method choice buys at low Q²:
- Q² value used for the bin label (affects bin migration);
- y value used for the bin label and for the boost normalization (`P·q
  = (s/2) y`);
- x value used for binning (and for the Breit-frame boost, less
  relevant for SIDIS in the gamma\*p frame).

The biggest impact of method choice is therefore on **bin migration**
(z, xF, pT_h between bins of (Q², y)) and on the boost-normalization
factor that enters z = (P·p_h)/(P·q). Both are well-documented
strengths of eΣ.

---

## 7. Q5 — Practical implementation

### 7.1 Can we compute a "best photon" at analysis time?

**Yes**, with the existing trees and no rebuilding. The required
ingredients are all stored:
- `scatEX, scatEY, scatEZ, scatEE` (lab-frame electron 4-vector)
- `BeamElecE, BeamProtP` (beam configuration)
- `Empz` (full event E−p_z; verified `~ 55.2 GeV` median)
- `Q2e, Ye, Q2s, Ys, Q2da, Yda, Q2h, Yh` (all four primary methods,
  reco and gen)
- per-track `Px, Py, Pz` (hadron 4-vector for the photon-axis pT_h)

Σ_h is **not** stored as a branch but is recoverable from the identity
`Σ_h = Empz − E'(1 − cos θ_e)`. This identity is the same one used by
the post-hoc systematics framework (`research-fast-systematics-methods.md`
§2.1).

### 7.2 What functional form should the combination take?

From the analysis in §4, three candidate forms:

(F0) **Single-method nominal (eΣ)**: store Q²=Q²_e, y=y_eΣ, x=x_eΣ;
build `q_eΣ` from back-derived E'_eΣ ≈ E'_measured (since Q²_eΣ=Q²_e).
This is **identical** to electron-only photon for the boost direction.

(F1) **eΣ with Σ-constrained longitudinal correction**: store Q²=Q²_e,
y=y_eΣ, x=x_eΣ; build `q_corr` with E'_corr derived from
`(1−y_eΣ)·Empz/(1−cos θ_e)` rather than from E'_measured. This is
Tier 1 (§4.5) and is the recommended single-photon improvement.

(F2) **BLUE-combined Q² and y**: store Q²=Q²_BLUE, y=y_BLUE; build
`q_BLUE` with E'_BLUE from Q²_BLUE and θ_e. Requires per-bin
covariance tabulation. Tier 2 in our complexity ranking.

**Recommendation.** Implement F1 as a sibling to F0 in the analysis
script (not a tree branch). Compare directly on matched MC. If the
gain on z/pT_h RMS is < 5%, ship F0 as the nominal; otherwise ship F1.

### 7.3 Should the combination depend on (Q², y)?

For BLUE (F2): yes, the optimal weights vary with (Q², y) because
the per-method resolutions vary across the plane. A fixed-weight
combination would be suboptimal.

For event-by-event selection (Strategy C): yes, by definition.

For F1 (Σ-constrained correction): no, the correction is a single
algebraic transformation that applies uniformly. This is a strong
practical advantage.

### 7.4 How would we validate that the combined photon is actually better?

Three closure tests (mirrors of the validation protocol in
`research-fast-systematics-methods.md` §5.5):

1. **Zero-shift closure (F1 only).** For events with `Empz = 2 E_e`
   (no ISR), F1 should reduce to F0 to within numerical precision.
   Verify per-event for a sub-sample.

2. **Resolution comparison on matched MC.** For each variable
   `v ∈ {z, xF, pT_h, phi_h, Q², y, x}`, compute the RMS of
   `(v_method − v_gen)` per (Q², y, v_gen) cell, separately for F0,
   F1, and (if implemented) F2. Report:
   - per-cell improvement in RMS (F1 vs F0)
   - response-matrix purity in the analysis binning (F1 vs F0)
   - response-matrix stability (F1 vs F0)
   - bin-migration tail fraction (F1 vs F0)

3. **Generator independence.** Repeat the comparison on RAPGAP31 and
   DJANGOH14 separately. The chosen method should give consistent
   gains across both generators (within statistics). Generator
   sensitivity > 30% on the relative gain is a red flag.

---

## 8. Q6 — Impact on TMD observables

### 8.1 Decomposing the pT_h resolution

For a hadron with measured `(p_x, p_y, p_z)` and a photon `q̂`, the
photon-axis pT_h is `|p̂ × q̂|·|p|`. To first order in small
perturbations:
```
δ(pT_h) ≈ (∂pT_h/∂p) δp + (∂pT_h/∂q̂) · δq̂
```
The first term (hadron tracking) has RMS ~ 1.5%·|p| ~ 0.01–0.05 GeV
across the analysis range. The second term has RMS proportional to
the photon angular RMS times |p|, with the photon angular RMS itself
dominated by the SpaCal angular resolution (~ 1–2 mrad) for the
transverse direction and by E'/Σ_h resolution for the longitudinal
direction.

For a typical SIDIS hadron with |p| = 1 GeV and a photon angular RMS
of 5 mrad (transverse) plus a longitudinal contribution that
**depends on method**, the pT_h RMS contribution from the photon is:
- Transverse-only floor: ~ 5 mrad · 1 GeV ~ 0.005 GeV (negligible)
- Longitudinal contribution from electron-only `q`: ~ 1% on q.E and
  q.Pz, propagating to ~ 1–3% on pT_h ~ 0.01–0.04 GeV.

Comparing with the user's measured pT_h RMS = 0.18–0.39 GeV, the
photon contribution at this level is **not** the dominant term. The
dominant term must be the **hadron tracking** through the boost
(which is sensitive to the boost magnitude through |p_h^*|) and the
**bin-edge resolution** when the analysis bins in pT_h.

This means the **direct gain on pT_h RMS from changing the kinematic
method is small** (a few percent at most). The larger gain is on z
RMS (which is ∝ y RMS, and is a factor 2 better in eΣ vs electron-only
at low y) and on xF (similar story plus W).

### 8.2 Quantitative estimate (rule-of-thumb, not yet measured on our MC)

| Quantity | Current (electron-q boost) | Tier 0 (eΣ-q) | Tier 1 (Σ-corrected q) | Notes |
|---|---|---|---|---|
| log₁₀ Q² RMS | 0.053 (measured) | ~0.053 | ~0.050 | Q²_eΣ = Q²_e; Tier 1 corrects ISR |
| log₁₀ x RMS | 0.084 (measured) | 0.067–0.074 | 0.063–0.070 | eΣ x is much better at low y |
| z RMS (TMD-safe) | 0.075 (measured) | 0.065–0.070 | 0.063–0.067 | Mostly y-driven |
| pT_h RMS (low pT_h ~ 0.3 GeV) | 0.18 GeV (measured) | 0.17–0.18 GeV | 0.17 GeV | Photon contribution is small |
| pT_h RMS (mid pT_h ~ 1 GeV) | 0.30 GeV | 0.28–0.29 GeV | 0.27–0.28 GeV | Boost-magnitude effect grows |
| φ_h RMS (rad) | (not yet measured) | comparable | ~ 5–10% improvement | Needs photon longitudinal direction |

**Numbers above are educated guesses based on Bassler–Bernardi scaling
laws.** The user must measure them on our MC. The most informative
single comparison is the **per-(Q², y, pT_h_gen) cell RMS** of pT_h
under each photon recipe — this directly answers "is it worth it?"

### 8.3 Is the improvement uniform or kinematic-dependent?

Strongly kinematic-dependent. Expectations:
- **Low y (y < 0.2)**: largest gains, factor 2 on σ(x)/x and σ(y)/y;
  ~10–20% on z RMS; smaller on pT_h.
- **Moderate y (0.2 < y < 0.5)**: 5–10% on most variables.
- **High y (y > 0.5)**: small gains (electron and eΣ have similar y
  resolution at high y). Tier 1 ISR correction still helps a few %
  for the tail.
- **Low Q² (Q² < 10 GeV²)**: same as low y essentially; the SpaCal
  angle dominates pT_h floor, so combinations cannot get much below
  this floor.
- **High Q² (Q² > 50 GeV²)**: small gains; eΣ ≈ electron in this regime.

### 8.4 Does the improvement justify the added complexity?

Tier 0 (eΣ as nominal): **yes, unambiguously.** This is what the H1
multiplicity paper does and what `research-dis-kinematic-methods.md`
§7 already recommends. No new complexity beyond what is already
planned in the eΣ adoption.

Tier 1 (Σ-corrected longitudinal): **likely yes** for the TMD
release, especially if z and pT_h response-matrix purity at low y is
the bottleneck. The added complexity is one extra function in the
analysis script (∼ 30 lines). No tree changes.

Tier 2 (BLUE or regression): **only if Tier 0/Tier 1 are insufficient
to meet release criteria.** The complexity-to-gain ratio is poor:
significant validation overhead, marginal gain.

---

## 9. Concrete recipe

### 9.1 Step-by-step recipe (Tier 0 + Tier 1)

This is the recipe to compute the "best photon" `q_best` per event from
existing tree branches, and from it the SIDIS variables
`(z, pT_h^photon, phi_h^Trento, xF)`.

**Inputs** (per event, from the merged ChargedTot tree):
- `BeamElecE, BeamProtP`
- `scatEX, scatEY, scatEZ, scatEE` (measured electron 4-vector)
- `Empz`
- `Q2e, Ye, Q2s, Ys` (the four ingredients of eΣ)

**Inputs** (per track):
- `Px, Py, Pz` (lab hadron 3-momentum)
- `GenPDG` (for hadron mass; default to π if unknown)

**Outputs** (per track):
- `Q2_eS, y_eS, x_eS` (eΣ DIS labels — Tier 0)
- `q_corr` (Σ-corrected photon 4-vector — Tier 1)
- `pT_h_photon, z, phi_h, xF` (SIDIS observables in the gamma\*p
  frame, computed using `q_corr`)

**Step 1: derive eΣ (Q², y, x) [Tier 0].**
```python
M_PROTON = 0.93827
s_cm = 4.0 * BeamElecE * BeamProtP

# Electron measurement
sx, sy, sz, se = scatEX, scatEY, scatEZ, scatEE
p_e = np.sqrt(sx*sx + sy*sy + sz*sz)
cos_te = np.where(p_e > 0, sz / p_e, 1.0)
sin_te = np.where(p_e > 0, np.sqrt(np.maximum(sx*sx + sy*sy, 0.0)) / p_e, 0.0)
phi_e = np.arctan2(sy, sx)
theta_e = np.arccos(np.clip(cos_te, -1.0, 1.0))

# eSigma DIS labels
Q2_eS = Q2e
x_eS = np.where((Ys > 0) & (s_cm > 0), Q2s / (Ys * s_cm), np.nan)
y_eS = np.where((Q2s > 0) & np.isfinite(x_eS),
                Q2e * Ys / Q2s, np.nan)
```

**Step 2: build the Σ-corrected scattered electron [Tier 1].**
```python
# Σ-constrained correction: enforce Empz_corr = 2 BeamElecE,
# δ_e_corr = (1 − y_eS) · 2 BeamElecE
# This puts any ISR back into the electron implicitly.
two_Ee = 2.0 * BeamElecE
denom = 1.0 - cos_te
E_prime_corr = np.where(denom > 1e-6,
                         (1.0 - y_eS) * two_Ee / denom,
                         se)  # fallback to measured E' if degenerate
# Defensive: clamp to physical range [0.5 GeV, 2 BeamElecE]
E_prime_corr = np.clip(E_prime_corr, 0.5, two_Ee)

scatE4_corr = np.stack([
    E_prime_corr * sin_te * np.cos(phi_e),
    E_prime_corr * sin_te * np.sin(phi_e),
    E_prime_corr * cos_te,
    E_prime_corr,
], axis=1)
```

**Step 3: build the corrected virtual photon and gamma\*p frame.**
```python
zeros = np.zeros_like(BeamElecE)
eBeam4 = np.stack([zeros, zeros, -BeamElecE, BeamElecE], axis=1)
p_E    = np.sqrt(BeamProtP**2 + M_PROTON**2)
pBeam4 = np.stack([zeros, zeros, BeamProtP, p_E], axis=1)

q_corr     = eBeam4 - scatE4_corr
gammaP     = q_corr + pBeam4
W2_photon  = m2_minkowski(gammaP)
W_photon   = np.where(W2_photon > 0, np.sqrt(np.maximum(W2_photon, 0)), np.nan)

safe_E = np.where(gammaP[:, 3] > 0, gammaP[:, 3], 1.0)
beta_gp = gammaP[:, :3] / safe_E[:, None]
```

**Step 4: photon direction unit vector (lab frame, for pT_h and φ_h).**
```python
q3 = q_corr[:, :3]
q3_mag = np.sqrt(np.sum(q3*q3, axis=1, keepdims=True))
q_hat = np.where(q3_mag > 0, q3 / q3_mag, q3)
```

**Step 5: per-track SIDIS observables (gamma\*p frame).**
```python
# Hadron 4-vector in lab
mh = mass_for_pdg(GenPDG)   # see compare_kinematic_methods.py:160-166
E_h = np.sqrt(Px*Px + Py*Py + Pz*Pz + mh*mh)
had_lab = np.stack([Px, Py, Pz, E_h], axis=1)

# z = (P · p_h) / (P · q)
PdotQ = dot4_minkowski(pBeam4, q_corr)
z = np.where(np.abs(PdotQ) > 0,
             dot4_minkowski(pBeam4, had_lab) / PdotQ, np.nan)

# pT_h relative to the photon axis (lab frame)
p3 = had_lab[:, :3]
p_dot_q = np.sum(p3 * q_hat, axis=1, keepdims=True)
p_perp = p3 - p_dot_q * q_hat
pT_h_photon = np.sqrt(np.sum(p_perp*p_perp, axis=1))

# phi_h Trento azimuth around q
# l_perp = component of incoming-lepton 3-momentum transverse to q_hat
l3 = eBeam4[:, :3]
l_dot_q = np.sum(l3 * q_hat, axis=1, keepdims=True)
l_perp = l3 - l_dot_q * q_hat
l_perp_mag = np.sqrt(np.sum(l_perp*l_perp, axis=1, keepdims=True))
l_hat = np.where(l_perp_mag > 0, l_perp / l_perp_mag, l_perp)
h_perp_mag = np.sqrt(np.sum(p_perp*p_perp, axis=1, keepdims=True))
h_hat = np.where(h_perp_mag > 0, p_perp / h_perp_mag, p_perp)
sin_phi = np.sum(q_hat * np.cross(l_hat, h_hat), axis=1)
cos_phi = np.sum(l_hat * h_hat, axis=1)
phi_h = np.arctan2(sin_phi, cos_phi)

# xF in gamma*p frame
had_star = boost_to(had_lab, beta_gp)   # see compare_kinematic_methods.py:136
xF = np.where((W_photon > 0) & np.isfinite(W_photon),
              2.0 * had_star[:, 2] / np.where(W_photon > 0, W_photon, 1.0),
              np.nan)
```

**Step 6: gen-side mirror.**
Replace `(BeamElecE, BeamProtP, scatEX/Y/Z/EE, Empz, Q2e, Ye, Q2s, Ys)`
with the corresponding `Gen` branches: `(BeamElecE, BeamProtP,
genElecPx, genElecPy, genElecPz, genElecE, EmpzGen if available,
Q2eGen, YeGen, Q2sGen, YsGen)`. Build `q_corr_gen`, gen frame, and
gen `(z, pT_h, phi_h, xF)` using the same recipe. **At gen level the
correction is an identity to within MC-truth precision** (no detector
smearing); the recipe still applies but the difference between Tier 0
and Tier 1 at gen level should be ≤ 0.1% per event.

### 9.2 Validation checklist

Before promoting Tier 1 to nominal, run:

1. **Zero-ISR closure**: select events with `|Empz − 2·BeamElecE| < 1
   GeV`. For these events, Tier 0 and Tier 1 should agree on
   `(q.E, q.Pz)` to within 0.1%. Histogram the per-event ratio.

2. **Σ-recovery closure**: verify `Σ_h_recovered = Empz − scatEE *
   (1 − cos θ_e)` matches the event-level Σ_h that built `Q2s, Ys`
   in the tree (within 10⁻³ relative). This is a sanity check that
   the tree's Q2s/Ys are internally consistent with the stored
   scattered electron and Empz.

3. **Resolution comparison** (the headline benchmark): on matched MC,
   per (Q²_gen, y_gen) cell, compute σ of:
   - `log Q²_method − log Q²_gen`
   - `log y_method − log y_gen`
   - `log x_method − log x_gen`
   - `(z_method − z_gen)`
   - `(pT_h_method − pT_h_gen)`
   - `Δφ_h` (mod 2π)
   for `method ∈ {electron-q (current), Tier 0 (eΣ), Tier 1
   (Σ-corrected eΣ)}`. Report per-cell and inclusive RMS, plus
   response-matrix purity in the TMD analysis bins.

4. **Generator independence**: repeat (3) on RAPGAP31 and DJANGOH14.

5. **Cross-check against the published resolution numbers**: the
   inclusive RMS of `log Q²` for Tier 0 should match the
   Bassler–Bernardi 5–8% range; if it does not, there is an
   inconsistency between our recipe and the H1OO-internal eΣ.

### 9.3 What changes downstream

If Tier 0 is adopted:
- The `pTh_photon, z, phi_h, xF` analysis-level variables that
  `plan-tmd-sidis.md` §1.1–§1.3 specifies are computed **using
  `q_corr` (or `q_eΣ` if only Tier 0 is wanted)** rather than
  `q = k − k'_measured`.
- The DIS-side `(Q², y, x)` labels are eΣ values.
- The TMD-fit grid `(x, Q², z, pT_h)` uses these labels.
- The frozen photon convention (pending OQ8 / Task 2B.10 in
  `plan-tmd-sidis.md`) becomes "eΣ + Σ-corrected longitudinal" or
  simply "eΣ" depending on Tier 0 vs Tier 1.

If Tier 1 is adopted:
- Same as above, but the photon used in the boost has the Σ
  longitudinal correction applied.
- The systematic uncertainty for "electron energy scale" must be
  evaluated **after** the Σ correction. Effectively, the Σ
  correction reduces sensitivity to E' miscalibration (because E' is
  back-derived from y and Empz), at the price of increasing
  sensitivity to Empz miscalibration (which is dominated by HFS
  energy scale). This is a **redistribution** of the systematic
  budget, not necessarily a reduction.
- Concretely, the post-hoc systematic recipe in
  `research-fast-systematics-methods.md` §5.1 needs to be updated to
  shift Empz consistently with the HFS scale shift, rather than
  shifting `(scatEX, scatEY, scatEZ, scatEE)` alone for the electron-
  scale shift.

### 9.4 What NOT to do

Do **not** mix conventions across reco and gen. The current high-Q²
chain has reco=eΣ but gen=Σ (`research-dis-kinematic-methods.md`
§4.4); this asymmetry biases the response matrices and must be fixed
either by recomputing gen with eΣ (the recommendation above) or by
recomputing reco with Σ (not recommended).

Do **not** ship a per-event method-selection scheme (Strategy C)
without explicit acceptance corrections that account for the
discontinuity at the selection boundary. This is harder than just
shipping eΣ everywhere.

Do **not** train a regression (Strategy E) on RAPGAP only and ship.
Cross-generator validation is mandatory, and the gain over Tier 1 is
likely too small to justify the validation overhead at the present
release stage.

---

## 10. Risks, gaps, and open questions

1. **The Σ-correction recipe (Tier 1) has not been published for
   SIDIS at H1.** It is an internal invention motivated by the
   prompt's question. While the algebra is clean and reduces to F0
   in the no-ISR limit, the user should treat it as a candidate
   improvement that requires explicit closure tests (§9.2). If the
   gain on z/pT_h RMS is < 5% across the analysis range, ship F0
   (eΣ) without the Σ correction and document the decision.

2. **The eΣ branches `z_SIDIS_es, xF_SIDIS_es` may already exist in
   the trees.** `compare_kinematic_methods.py:267` reads them. The
   user should verify on NAF whether these branches use the
   Bassler–Bernardi eΣ-consistent boost (§2.4 procedure (a) or (b))
   or some hybrid. If they do, Tier 0 is essentially free at
   analysis time — just read the existing branches.

3. **The current low-Q² builder uses Σ for stored Q²/y (not eΣ)**
   per `research-dis-kinematic-methods.md` §1. The user should
   confirm whether this is still the case in the latest reprocessed
   trees, or whether the §6.1 "option (a)" eΣ extension has been
   merged. The Tier 0 recipe is method-agnostic at analysis time:
   it computes Q²_eΣ from `Q2e, Q2s, Ys` regardless of what the
   tree-level `Q2s` field contains.

4. **The pT_h formula in `plan-tmd-sidis.md` §1.1 uses the photon
   3-direction in the lab.** This is correct for the published
   observable. The recipe in §9.1 Step 5 implements exactly this.
   Note that some TMD literature (especially SCET) uses pT_h in the
   gamma\*p frame; for low-y SIDIS at HERA energies, the two are
   numerically very close but not identical. The H1 published
   convention (= the lab-frame photon-axis projection) is the
   standard for HERA SIDIS releases.

5. **φ_h sign conventions are easy to get wrong.** The Trento
   convention (`plan-tmd-sidis.md` §1.3) requires the precise
   formula in §9.1 Step 5. Verify the sign by comparing with
   ztudesy `PartCorrAnalyzer.C` (which computes phi_h for the H1
   multiplicity paper) on a benchmark sub-sample.

6. **No HAT branch for `Σ_h` directly.** As confirmed in
   `research-dis-kinematic-methods.md` §9, `Σ_h` is not a stored
   branch. The recovery `Σ_h = Empz − E'(1 − cos θ_e)` is exact in
   principle but inherits any inconsistency between the H1OO
   `H1Calculator` Σ_h and the stored `Empz`. Add a closure test
   per §9.2 step 2.

7. **Per-cluster HFS effects on the Σ correction.** The Σ
   correction takes Σ_h as a black-box scalar. In reality, Σ_h is
   built from many HFS clusters with their own resolutions and
   correlations. A truly optimal photon would use the full HFS
   covariance, which is not stored in the trees. The Σ-scalar
   correction is a first-order approximation. For the gain
   estimates in §8 to hold, the Σ_h scalar resolution must be
   close to a Gaussian with the published 5–10% width.

8. **Beam-energy and ISR coupling at the highest y.** The Σ
   correction assumes `Empz_corr = 2 E_e` (no ISR). In reality,
   genuine ISR events have `Empz < 2 E_e`. Forcing `Empz_corr =
   2 E_e` puts the lost energy back into the electron; this is the
   right thing for the boost (it gives the gamma\*p frame the
   correct invariant mass) but it makes the Q²_corr value
   different from Q²_e. To preserve the eΣ Q² label, use the
   "two-step decoupling" variant in §4.5 (store Q²=Q²_e, but use
   `q_corr` only for the boost). This is the recommended
   convention.

9. **No high-Q² coverage in this artifact.** All recipes here are
   for the low-Q² regime. High-Q² (LAr electron, larger Q² range)
   has different SpaCal-vs-LAr resolution mix and the relative
   ranking of methods may shift. The same architectural recipe
   (eΣ + optional Σ correction) is the right starting point at
   high Q², but the resolution gains may be smaller.

10. **Method-combination has not been benchmarked for the
    high-Q² regime in this research.** The high-Q² extension
    research (`research-highQ2-sidis-extension.md`) should be
    consulted separately if combinations are pursued there.

---

## 11. Summary

- The H1 trees already store five DIS reconstruction methods (e, Σ,
  eΣ, DA, JB) at both reco and gen, plus the lab-frame ingredients
  needed to recompute any of them and to combine them in any way at
  analysis time. **No reprocessing is required** to study or apply
  combinations.

- **The five methods span at most three independent dimensions** (the
  primitives `E', θ_e, Σ_h, pT_h`), so the BLUE/χ² combination space
  is more constrained than naive "combine 5 measurements" suggests.

- **The literature is unanimous: H1 picks one method (eΣ at low Q²),
  not a combination.** The H1 multiplicity paper DESY 20-176 (the
  closest analog to ours) explicitly uses eΣ and cites it as
  "optimum in resolution and little sensitivity to QED radiative
  effects." No published HERA SIDIS analysis uses a combination
  estimator.

- **The single most impactful change is to switch from electron-only
  `q = k − k'_measured` to an eΣ-consistent photon for the
  SIDIS-frame boost (Tier 0).** This is what
  `research-dis-kinematic-methods.md` §6–§7 already recommends.
  Expected gains: 10–25% on σ(x)/x, 10–20% on z RMS at low y, 5–10%
  on pT_h RMS, smaller on Q² and φ_h.

- **An additional "Σ-constrained longitudinal correction" (Tier 1)
  is a closed-form, single-event correction** that adds ~3–8% on
  z/xF RMS by handling ISR robustly. It is novel for our context
  but algebraically clean and easy to validate.

- **More elaborate combinations (BLUE, regression, ML) are Tier 2.**
  Expected marginal gain over Tier 1 is small (a few percent at
  most) and the validation cost is high. Do not pursue unless Tier 0
  + Tier 1 fail to meet the TMD release criteria.

- **The transverse direction of the photon at low Q² is electron-
  only** (`(p_x, p_y)` of the SpaCal cluster) and **independent of
  method**. This sets a floor on pT_h and φ_h resolution that no
  combination can reduce. Improvements from method choice are
  concentrated in z, xF, x, and y, not pT_h or φ_h.

- **The concrete recipe in §9.1 produces a "best photon" from
  existing tree branches in ~30 lines of Python.** Validation steps
  are in §9.2. Adoption decision is binary: ship Tier 0 if Tier 1
  improvement is < 5%, otherwise ship Tier 1.

- **Open issues to verify on NAF before locking in the recipe**
  (§10): existence and convention of the `_es` branches, current
  low-Q² builder convention (Σ vs eΣ), `Empz` vs Σ_h consistency,
  φ_h sign convention against ztudesy.
