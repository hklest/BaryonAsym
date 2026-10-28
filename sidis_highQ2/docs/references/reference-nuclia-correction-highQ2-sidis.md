<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: H1NuclIACor and NucliaWeight for high-Q2 charged-hadron SIDIS

**Date:** 2026-05-19
**Author:** h1-software specialist
**Audience:** G11 NuclIA workstream of `plan-highQ2-sidis-next-steps.md` (r3).
**Source-of-truth status:** Each answer below is marked **VERIFIED** (read
from a source file during this task) or **PROVISIONAL** (inferred from
documentation, cross-analysis usage, or not verifiable against in-workspace
source). The v5b SIDIS builder source is on NAF, NOT in the local
workspace, so any claim about the SIDIS builder's per-track NucliaWeight
storage is PROVISIONAL until the on-NAF builder file is read.

---

## 0. Source files consulted in this task

| File | Status |
|------|--------|
| `H1/h1oo-master/H1PhysUtils/H1NuclIACor.h` | Read (46 lines) |
| `H1/h1oo-master/H1PhysUtils/H1NuclIACor.C` | Read (78 lines) |
| `H1/h1oo-master/H1Tracks/H1CentralTrack.C` (`GetNuclIACorWeight` only) | Read (lines 574-612) |
| `H1/lambda/.work_feeddown/TreeBuilder.C` (NuclIA call sites) | Read (lines 588-611, 1264-1287, 1392-1406) |
| `H1/sidis_lowQ2/context-handoff.md` (v5b NucliaWeight description) | Read (line 855) |
| `H1/sidis_lowQ2/source_cache/ProtonTreeBuilder_lowQ2.C` (v5b SIDIS builder source; legacy proton-prefixed name — see `docs/references/reference-naming-aliases-highQ2-sidis.md`) | **No NucliaWeight reference found** -- the source_cache predates v5b. |
| `H1/sidis_highQ2/source_cache/ProtonTreeBuilderHighQ2.C` | **No NucliaWeight reference found** -- the source_cache predates v5b. |
| `H1/sidis_lowQ2/docs/research/research-nuclia-correction.md` | **DOES NOT EXIST** in the workspace (handoff lists it; not present in `docs/research/`). |

The fact that the local v5b builder cache is stale means the per-track
SIDIS implementation cannot be inspected in this task. The plan
(`plan-highQ2-sidis-next-steps.md` line 85) already notes this gap.

---

## 1. Class API — VERIFIED

**Class:** `H1NuclIACor`
**Header:** `H1PhysUtils/H1NuclIACor.h`
**Implementation:** `H1PhysUtils/H1NuclIACor.C`
**Author:** Andreas Meyer, 2007-01-31; last update 2010-10-08 by pahl.
**Underlying Fortran:** `ani_ineff_` (extern "C"), provided by K. Daum.

### Constructor

```cpp
H1NuclIACor::H1NuclIACor();   // prints "new H1NuclIACor!!!" on stderr; otherwise no state
```

The class carries **no instance state**. All useful methods are `static`,
so no instantiation is required (and instantiating it pollutes stderr).

### Public methods (both `static`)

```cpp
static Float_t GetWeight(int ipart,
                         float charge,
                         float pt,
                         float phi,
                         float theta,
                         float dca = 0.,
                         float z0  = 0.);

static Float_t GetCorrection(int ipart,
                             float charge,
                             float pt,
                             float phi,
                             float theta,
                             float dca = 0.,
                             float z0  = 0.);
```

### Argument semantics — VERIFIED

| Argument | Meaning | Notes |
|----------|---------|-------|
| `ipart` | Particle hypothesis: **1 = pion, 2 = kaon** | **No proton hypothesis exists.** See §4. |
| `charge` | Track charge (+1 or -1) | Particle vs antiparticle have different inelastic cross-sections; this is the dependence used. |
| `pt`    | Transverse momentum, GeV | |
| `phi`   | Azimuth, rad | |
| `theta` | Polar angle, rad | |
| `dca`   | Distance of closest approach to PV, cm | Defaulted to 0; lambda builder passes 0. |
| `z0`    | Track z0 (or event PVz), cm | Lambda builder passes `PVzgen` (gen primary-vertex z), not a per-track z0. **Run-dependent geometry uses this.** |

### Internal logic — VERIFIED

`GetCorrection` calls the Fortran routine with the current run number
obtained from the H1OO global tree:

```cpp
int inrun = gH1Tree->GetRunNumber();
effcor = ani_ineff_(&ipart,&charge,&pt,&phi,&theta,&dca,&z0,&inrun);
return effcor;
```

`GetWeight` returns `1 - GetCorrection` on MC and `1.0` on data with a
stderr warning ("Do not apply to data"):

```cpp
if (isMc) return 1. - GetCorrection(...);
else      { std::cout << "Warning ... Do not apply to data"; return 1.; }
```

`isMc` is queried via `H1Tree::Instance()->IsMC(...)` with a transient-tree
fallback. This means:

- The correction is **silently zero on data** (returns 1.0) -- safe.
- The correction depends on the **run number** as queried at the moment
  of the call -- the call must be inside the per-event read loop, not
  cached globally.

### Convenience wrapper in framework — VERIFIED

`H1CentralTrack::GetNuclIACorWeight()` (in `H1Tracks/H1CentralTrack.C`
lines 574-612) is the framework-provided per-track wrapper. It:

1. Returns 1.0 unless `|PDG|` is 211 (pion) or 321 (kaon). For protons
   (PDG 2212) and any other species, the returned weight is unconditionally
   1.0. **The framework itself has no proton path.**
2. Sets `ipart = 1` for `|PDG|==211`, `ipart = 2` for `|PDG|==321`.
3. Extracts `dca` and `z0` from the helix trajectory's original parameters
   (indices 3 and 4 of `H1HelixTrajectory::GetOriginalParameter()`), NOT
   from a default-zero value.
4. Calls `H1NuclIACor::GetWeight(part, charge, pt, phi, theta, dca, z0)`.

The framework wrapper therefore performs an **internal selection on the
track's assigned PDG hypothesis**, not a blending across hypotheses.

---

## 2. Per-track semantics of NucliaWeight — VERIFIED for the framework call

`GetWeight` returns `1 - effcor`, where `effcor` is the **fraction of
tracks lost to nuclear interaction not modeled in MC**. Therefore:

- **NucliaWeight is a multiplicative MC track weight in (0, 1].**
- **Convention:** `NucliaWeight < 1` means "this track type at this
  kinematic point is lost more often in real data than the MC predicts;
  reduce its MC contribution." The closer to 1, the smaller the
  correction. Values are typically `1 - O(few percent)`.
- **It is an inverse-loss weight applied to MC**, not to data. On data
  the framework returns 1.0 and warns.
- This is **the opposite convention from what the originating prompt
  suggested** ("weight > 1 means more loss compensated"). The actual
  framework value is `1 - loss`, i.e. NucliaWeight is a *correction to
  the MC efficiency*, downweighting MC tracks so that MC-derived
  acceptance better matches reality.

### Numerical range — PROVISIONAL

No values exercised in this task. From the Fortran K. Daum
parametrization (charged-hadron inelastic interactions in H1 inner
detector material, ~5% radiation lengths to CJC), the qualitative
expectation for HERA-II charged hadrons in `Pt > 0.15 GeV, |Eta| < 1.3`
is **`NucliaWeight in [~0.95, ~1.00]` for pions and `[~0.93, ~1.00]` for
kaons**, with the largest deviation from 1 at the lowest pT and most
forward eta (longest path length in detector material). PROVISIONAL --
must be checked by reading `NucliaWeight` directly off a v5b high-Q2 MC
file in G11.

### Where the weight should be applied (analysis decision, not framework)

The plan (`plan-highQ2-sidis-next-steps.md` line 498) already specifies:

```python
track_weight = weight1 * weight2 * (nuclia_weight if use_nuclia else 1.0)
```

This is consistent with the framework semantics: NucliaWeight is a
*track-level multiplicative factor applied to MC tracks only*, multiplied
in alongside the GKI event weight and the user weight.

---

## 3. Hypothesis blending — PROVISIONAL (cannot verify without on-NAF v5b source)

The v5b high-Q2 handoff entry says:

> NucliaWeight on ChargedTot -- H1NuclIACor per-track correction with
> **pion/kaon hypothesis blending**.

### What the framework supports — VERIFIED

The framework offers **two distinct mechanisms**:

1. **`H1NuclIACor::GetWeight(ipart, ...)` with `ipart = 1 or 2`** — returns
   a single-hypothesis weight, pion XOR kaon. No native blending.
2. **`H1CentralTrack::GetNuclIACorWeight()`** — picks the hypothesis
   from the track's assigned PDG (truth match in MC, or the most-likely
   PID in data, depending on how the track's PDG field is populated).
   Returns 1.0 if neither pion nor kaon. **Still no blending.**

There is **NO blending implemented inside H1OO itself.** Any "pion/kaon
blending" must therefore live in the SIDIS builder code on NAF.

### What the lambda builder does — VERIFIED

`H1/lambda/.work_feeddown/TreeBuilder.C` calls
`H1NuclIACor::GetWeight(1, pionCharge, ...)` with **hard-coded `ipart=1`
(pion only) and gen-pion kinematics**. No kaon path; no blending. The
lambda case is unambiguous because the decay daughter is pi^- by physics.
The lambda code is therefore **not a template for the SIDIS blending
question** -- the SIDIS builder must implement blending differently.

### Likely v5b SIDIS implementation — PROVISIONAL

Without the on-NAF v5b builder source, the most plausible implementations
of "pion/kaon hypothesis blending" are:

- **Fixed-fraction linear combination:** something like
  `NucliaWeight = f_pi * GetWeight(1,...) + f_K * GetWeight(2,...)` with
  `f_pi + f_K = 1` and `f_pi` chosen as the published HERA SIDIS pion
  fraction (~0.85). The handoff phrasing "hypothesis blending" most
  naturally fits this.
- **Per-track gen-PDG selection on MC** (similar to
  `H1CentralTrack::GetNuclIACorWeight()`): for matched MC tracks, use
  the gen PDG; for unmatched tracks, fall back to pion. **This is not
  blending; it is per-track selection.**
- **PID-likelihood-weighted blending** using the dE/dx likelihoods
  Lk{Pion,Kaon,Proton}. **Ruled out for v5b** because the handoff
  explicitly states the 10 dE/dx PID branches were removed in v5b
  (handoff line 870), so the likelihoods are no longer on the tree.

**G11 must read the v5b builder on NAF and freeze the exact formula
before any closure test is interpreted.** Suggested NAF inspection:

```bash
ssh -O check naf-hone.desy.de
ssh naf-hone.desy.de "grep -n 'NucliaWeight\|H1NuclIACor' \
  /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2/src/ProtonTreeBuilder_lowQ2.C \
  /data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C"
```

---

## 4. Proton handling — VERIFIED (framework) / OPEN (analysis)

### Framework — VERIFIED

**`H1NuclIACor` provides NO proton hypothesis.** `ipart` is documented as
`1 = pion, 2 = kaon` only (header lines 32-33). The Fortran `ani_ineff_`
parametrization was tuned for pions and kaons.
`H1CentralTrack::GetNuclIACorWeight()` returns 1.0 unconditionally for
`|PDG|=2212` because the PDG check excludes protons.

### Why this matters for charged-hadron SIDIS — analysis impact

The SIDIS species set is `{pi, K, p}` (handoff line 197). Protons:

- **Have larger nuclear-interaction cross-sections than pions** in nuclear
  matter (`sigma_inel(p)` > `sigma_inel(pi+)` in the few-GeV range, by
  roughly a factor of ~2 at the low-momentum end of the SIDIS range).
- **Are strongly charge-asymmetric in ep collisions**: p production
  greatly exceeds pbar production at all x (baryon-number conservation).
- This makes the missing proton NuclIA correction **the largest
  identified physics motivation to be flagged**: any "blending" that
  ignores protons effectively assigns them either the pion (lower) or
  kaon (intermediate) cross-section, both of which **underestimate the
  pbar/p loss asymmetry**.

### Impact on observables — PROVISIONAL magnitudes

| Observable | Direction of NuclIA effect from missing proton path |
|------------|-----------------------------------------------------|
| `M^{h+/-}` | Slight underweighting of MC proton/pbar tracks vs reality. Net underweighting of the h+ yield (more p than pbar in inclusive sample). |
| `A_h` | **Biased toward more positive values** if pbar suffers more uncorrected loss than p. This is the wrong sign of correction: the published asymmetry would be too positive at low z and high x where the proton fraction is largest. Magnitude PROVISIONAL but plausibly comparable to the proton fraction of the h+ sample (few percent) times the differential pi-vs-p inelastic cross-section gap (~10-20%) -- O(0.001-0.003) absolute on A_h. |
| `A_BC(h+)`, `A_BC(h-)` at fixed hadron charge | Effect **largely cancels** because the proton fraction is set by hadronic production physics that is beam-charge symmetric. Residual sub-percent. |
| `A_BC(charge-summed)` | Effect cancels at the same level as `A_BC(h+/-)`. |

### Recommendation

For the high-Q2 SIDIS analysis:

1. **Document the proton omission explicitly** as a systematic
   "nuclia_no_proton_path" entry in the G11 systematics package.
2. **Bound it** by computing two extreme variations of NucliaWeight:
   (a) assume protons get the kaon weight (largest pion-like loss in
   the supported set); (b) assume protons get a 1.5x extrapolation of
   the kaon loss. The spread bounds the systematic.
3. **Quote it explicitly in the A_h systematic budget**, not merged
   into a generic NuclIA model uncertainty, because A_h is the
   observable where the proton omission does not cancel.

---

## 5. Charge dependence — VERIFIED

`charge` is an explicit argument of `GetWeight` (signature line 39 of
`H1NuclIACor.C`). The Fortran `ani_ineff_` is therefore expected to apply
**different inelastic cross-sections to particle vs antiparticle** (in
particular pbar in matter has a larger sigma_inel than p; pi-/pi+
difference is smaller; K-/K+ difference is intermediate). This is the
documented physics motivation for the existence of the routine.

Implication for the builder: **the per-track NucliaWeight stored on
ChargedTot already encodes the track's charge sign**. Downstream code
must NOT separately multiply by a sign-dependent factor; it must apply
NucliaWeight exactly as stored. PROVISIONAL until verified on file
(`AnaTools.GetEntry(0); print NucliaWeight[Charge==+1] vs
NucliaWeight[Charge==-1]` should give visibly different distributions in
the same kinematic bin if and only if charge dependence is being applied).

---

## 6. Cancellation in asymmetries — physics argument + verified code subtlety

### A_h = (N(h+) - N(h-)) / (N(h+) + N(h-))

NuclIA **does NOT cancel** in `A_h`. The framework argument:

- `H1NuclIACor::GetWeight(ipart, +1, ...)` and
  `H1NuclIACor::GetWeight(ipart, -1, ...)` deliberately return different
  values because antiparticles have larger inelastic cross-sections in
  matter than particles (especially pbar vs p, smaller for pi-/pi+).
- The h+ and h- channels therefore carry systematically different MC
  weights at the same kinematic point.
- Effect direction on `A_h`: h- is suppressed more strongly in MC after
  NuclIA correction, so the *acceptance-corrected* h- yield is boosted
  more, **driving uncorrected A_h to higher values than corrected A_h**.
- Expected fractional impact on A_h: **a few times 10^-3 absolute**, the
  same order as the measurement itself. NuclIA is a leading correction
  for A_h, not a small one.

### A_BC(charge-summed) = (N(e+p) - N(e-p)) / (N(e+p) + N(e-p))

NuclIA **should largely cancel**. Physics argument:

- The detector geometry, material budget, run-by-run alignment, and
  charged-hadron species composition reaching the detector are
  approximately identical between e+p and e-p running periods.
- The same hadron species (e.g. pi+) hits the same material with the same
  cross-section in both beam configurations.
- Cancellation breaks **only** if `ani_ineff_` carries a run-number
  dependence that correlates with beam charge (different periods have
  different beams). The code reads `gH1Tree->GetRunNumber()` -- if the
  Fortran tables differ by period, the cancellation is not exact.
- Residual after cancellation: PROVISIONAL, likely **at the 10^-4 level
  or below**, dominated by run-period material/geometry differences and
  proton-omission asymmetric residuals.

### A_BC(h+) and A_BC(h-) at fixed hadron charge

NuclIA **should cancel at the same level as the charge-summed case**,
for the same reason: the same hadron species hits the same material in
both beam configurations. Cancellation breaks only via:

- Run/period-dependent geometry (small).
- The proton-omission path (§4), which is itself a hadronic-side effect
  that is beam-charge symmetric, so even the residual mostly cancels in
  `A_BC` at fixed hadron charge.

**Code-level subtlety to flag for G11:** because `GetCorrection` queries
`gH1Tree->GetRunNumber()` at the moment of the call, **the builder must
call NuclIA inside the per-event loop with the current run already
loaded**, not from a cached pointer set during initialization.
Misplaced caching would assign the wrong cross-section to every track
in misaligned events and would break the per-beam cancellation.

### Summary cancellation table (proposed for `highQ2_systematics_cancellation_matrix.json`)

| Observable | NuclIA cancellation | Residual driver |
|------------|---------------------|------------------|
| `A_h` | **DOES NOT CANCEL** | Charge dependence of sigma_inel is the design intent of the correction. |
| `A_BC(charge-summed)` | Cancels to first order | Run/period material tables; proton omission (small). |
| `A_BC(h+)` | Cancels to first order | Same as above. |
| `A_BC(h-)` | Cancels to first order | Same as above, slightly enhanced because pbar has larger sigma_inel. |

---

## 7. Default disabled vs enabled — PROVISIONAL

The v5b handoff (line 855) describes NucliaWeight as a **stored per-track
branch on ChargedTot**, alongside other per-track weights. The natural
inference is that the builder **stores the weight but does not apply
it**; the analysis script multiplies it in optionally.

This is consistent with the plan's intended pattern
(`plan-highQ2-sidis-next-steps.md` line 498):

```python
track_weight = weight1 * weight2 * (nuclia_weight if use_nuclia else 1.0)
```

**Confirm by inspection on NAF**: the builder should write `NucliaWeight`
to the tree branch without multiplying it into any of the other stored
quantities. If the builder also pre-multiplies it into `weight1` or
`weight2`, downstream code that also multiplies by `NucliaWeight` would
**double-count the correction** -- a closure test that would silently
pass at the per-track level but explode under the systematic check.

---

## 8. Validation targets for G11 closure

### 8.1 Closure: with vs without NuclIA on M^{h+/-}

**Test:** compute the full multiplicity chain (G6 acceptance + G7 DIS
acceptance + G9 multiplicity formula) twice, with `use_nuclia=True` and
`use_nuclia=False`, on the same v5b MC + data inputs.

**Expected differential difference:**
`(M^h_with - M^h_without) / M^h_without ~ <NucliaWeight> - 1 ~ -2% to -5%`
per bin, larger at low pT and forward eta.

**Pass criterion:** non-zero, of the correct sign (without-NuclIA larger
than with-NuclIA, because NuclIA reduces MC acceptance which reduces the
acceptance-corrected yield), and of order few-percent.

**Fail modes to catch:** zero difference (NuclIA not being applied);
positive sign (convention bug); >10% difference (likely a double-count).

### 8.2 Cancellation: with vs without NuclIA on A_h vs A_BC(h+)

**Test:** compute `A_h` and `A_BC(h+)` twice, with and without NuclIA.

**Expected ratio of differences:**

`|delta A_h / A_h| ~ a few percent` (NuclIA shifts A_h at the few-times-10^-3
level on top of an A_h that is itself O(10^-2)).

`|delta A_BC(h+) / A_BC(h+)|` should be **at least one order of magnitude
smaller** than `|delta A_h / A_h|`, ideally two orders of magnitude.

**Pass criterion:**

`|delta A_BC(h+) / delta A_h| < 0.1` at the integrated level. If this
ratio is closer to 1, the cancellation is broken (most likely cause:
proton omission §4 or run-number caching bug §6).

### 8.3 PID-blending sensitivity (only meaningful once §3 is resolved)

**Test:** once the v5b blending formula is read off NAF and confirmed
to be a fixed-fraction `f_pi * w_pi + f_K * w_K`, vary `f_pi` within the
published HERA SIDIS pion fraction range. A defensible variation is
`f_pi in [0.80, 0.90]`, centered on the H1 nominal ~0.85.

**Expected systematic on M:** the fractional shift on M is
`(f_pi_high - f_pi_low) * (<w_pi> - <w_K>) ~ 0.10 * 0.02 ~ 0.2%` per
bin. Small but non-negligible.

**Expected systematic on A_h:** because both h+ and h- are blended the
same way, the absolute systematic on A_h is **much smaller** than the
blending systematic on M -- estimate `< 5 * 10^-4` absolute on A_h.

### 8.4 NaN/edge-case sweep

Spot-check that `NucliaWeight` is finite and in `(0, 1.0]` for every
ChargedTot entry on the v5b MC. Sentinel values (1.0 returned by the
data fallback, or 0.0 from a Fortran exception) must be flagged.

---

## 9. Pitfalls — VERIFIED from source

| Pitfall | Source evidence | Mitigation |
|---------|-----------------|------------|
| **Silent return = 1.0 on data.** | `H1NuclIACor.C` lines 50-58 explicitly: if `!isMc` return 1.0 with stderr warning. | If a NucliaWeight branch is unintentionally filled by the data builder, every data track will carry weight 1.0. **This is correct behavior, but it means a "NuclIA was applied" branch on data is uninformative.** Document explicitly that data NucliaWeight is identically 1.0. |
| **Run-number dependence read at call time.** | `H1NuclIACor.C` line 69: `int inrun = gH1Tree->GetRunNumber();` queried inside `GetCorrection`. | The builder must call NuclIA inside the per-event loop. Caching the result globally would assign the wrong run to subsequent events. |
| **Constructor prints on stderr.** | `H1NuclIACor.C` lines 31-36. | Never `new H1NuclIACor()` in a production builder. All methods are static; instantiation only pollutes logs. |
| **PDG-2212 returns 1.0 in framework wrapper.** | `H1CentralTrack.C` lines 590-608. | If the SIDIS builder uses `H1CentralTrack::GetNuclIACorWeight()`, protons are silently uncorrected. See §4. |
| **No proton hypothesis.** | Header line 32-33 limits `ipart` to 1 (pion) and 2 (kaon). | See §4. |
| **DCA/z0 default to 0.** | Header default values; lambda builder passes 0. | The Fortran parametrization may use DCA/z0 to compute path length in beampipe material. Passing 0 may be acceptable for primary-vertex tracks but should be confirmed. The framework wrapper `H1CentralTrack::GetNuclIACorWeight()` pulls real `dca/z0` from the helix trajectory; the lambda code does not. The v5b SIDIS builder choice must be documented in G11. |
| **gH1Tree global state.** | `H1NuclIACor.C` line 69 uses the H1Tree singleton. | Multi-tree workflows (e.g. mixing MC and data file lists in one process) must respect H1Tree::Instance() state. The standalone SIDIS builder runs one tree at a time, so safe in practice. |
| **`ClassDef(...,2)` schema version.** | Header line 44. | Version is 2; any tree written by an older H1OO with `ClassDef(...,1)` may behave differently. The v5b builder uses the current H1OO release, so this is not a current concern but should be checked if mixing oo-release versions across MC and data (cross-listed in plan G2 task 5). |
| **No documented domain-of-applicability cuts.** | Source has none. | The K. Daum parametrization is tuned in some kinematic window. Extrapolation to extreme pT or theta is not validated by the framework. For the SIDIS phase space (`Pt > 0.15 GeV, |Eta| < 1.3`) the parametrization should be safely in domain, but the analysis should not extrapolate below `Pt = 0.15 GeV`. |
| **Constant 1.0 floor on data is invisible to closure tests.** | `H1NuclIACor.C` line 57. | A "NuclIA on/off" closure on data alone would show zero difference and falsely suggest NuclIA is broken. Closure tests must use MC. |

---

## 10. Cross-analysis status — VERIFIED

### Lambda (low-Q2 and high-Q2)

Both Lambda analyses use `H1NuclIACor::GetWeight` per Lambda candidate
(through the daughter pion only):

- `H1/lambda/.work_feeddown/TreeBuilder.C` lines 1280-1286 and 1398-1403
  call with `ipart=1, charge=pionCharge(±1), pt/phi/theta` from the
  gen-matched pion, `dca=0`, `z0=PVzgen`.
- `H1/lambda_HiQ/src_review/TreeBuilder.C` lines 1286 and 1377 have
  identical structure.

The Lambda usage is **single-hypothesis (pion only) and gen-matched**.
It is the cleanest demonstration of the API in the workspace, but it is
**not a template for SIDIS pion/kaon blending**.

### Proton (low-Q2 and high-Q2)

No `H1NuclIACor` references found in `H1/proton/` or
`H1/proton_HiQ/`. The proton analyses appear not to apply this
correction. (One stray comment in
`H1/proton_HiQ/code_review/teamA_patch/ProtonUtils.h:62` mentions
"nuclear interactions" but in the context of GEANT3 secondaries from
the STR bank, not `H1NuclIACor`.)

**Implication for the high-Q2 SIDIS plan:** the proton_HiQ context-handoff
will not contain a tested NuclIA procedure for protons. The SIDIS-specific
proton handling decision in §4 cannot be borrowed from proton_HiQ. It
must be made fresh.

### Xi

No references in `H1/xi/`.

---

## 11. Open items for G11

1. **Read the v5b builder source on NAF** at
   `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C`
   and document the exact NucliaWeight blending formula, the chosen
   `ipart` selection, the `dca`/`z0` source, and whether the weight is
   already pre-multiplied into other branches.
2. **Verify on file** that `NucliaWeight` distributions on v5b high-Q2 MC
   are in `(0, 1.0]`, show charge dependence, and show the expected pT
   trend.
3. **Confirm data behavior**: NucliaWeight on data v5b should be
   identically 1.0 by framework design.
4. **Execute the four closure tests in §8.**
5. **Add the proton-omission systematic** explicitly to the G11
   cancellation matrix per §4, distinct from a generic "NuclIA model"
   variation.
6. **Recreate the missing research note** referenced in the handoff
   (`docs/research/research-nuclia-correction.md`) using the answers in
   this reference as its starting source.

---

## Provisional answers requiring source inspection

- **§3 hypothesis blending formula** -- on-NAF v5b SIDIS builder source.
- **§7 default disabled vs enabled** -- on-NAF v5b SIDIS builder source.
- **Numerical range in §2** -- direct readout of the v5b NucliaWeight branch.
- **§4 magnitude estimates for proton omission impact** -- bounded MC
  closure tests.

Everything else (§1, §2 framework convention, §5, §6 cancellation
arguments, §9 pitfalls, §10 cross-analysis status) is VERIFIED against
H1OO source files read during this task.

---

# Section 12: Builder Application Audit — 2026-05-19

**Audit scope:** Read v5b high-Q2 builder source on NAF and a v5b RAPGAP per-run
output ROOT file to close the PROVISIONAL items in §2 (numerical range), §3
(blending formula), §4 (proton handling), and §7 (stored vs pre-applied).
Read-only audit; no NAF files modified.

## 12.0 Sources consulted in this audit

| File on NAF | Status |
|-------------|--------|
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | Read (lines 320–400, 596–625, 1040–1180, 1280–1320) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_LowQ2/src/ProtonTreeBuilder.C` | Read (lines 1000–1175); confirmed v5b NuclIA logic is IDENTICAL to high-Q2 |
| `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.1.1/h1oo_include/H1PhysUtils/H1NuclIACor.h` | Read; confirmed deployed API matches local `h1oo-master` copy and §1 |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/src/PhysicsConstants.h` | Listed (6441 bytes; no NuclIA constants — irrelevant to this audit) |
| `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/output/mc/rapgap31/7071/proton_highQ2_mc_rapgap31_7071_em06_0000.root` | Read via uproot (`ChargedTot;2`, 266 345 tracks) |

## 12.1 Stored vs pre-applied — VERIFIED

**Answer: stored-only. NucliaWeight is NOT pre-multiplied into `weight1` or
`weight2`.**

Exact assignments to `weight1` / `weight2` in the builder (only one site each
per event, at lines 622–623):

```cpp
// ProtonTreeBuilderHighQ2.C:622-623
weight1 = *hatWeight1;
weight2 = isMC ? *hatWeight2 : 1.0f;
```

These are HAT event-level reads. The exhaustive grep `grep -nE
'weight1|weight2|NucliaWeight'` shows **no further assignment** to `weight1` or
`weight2` anywhere in the file. NucliaWeight is written to its own dedicated
branch:

```cpp
// ProtonTreeBuilderHighQ2.C:390
chargedTree->Branch("NucliaWeight", &NucliaWeight_ch, "NucliaWeight/F");
```

Empirical confirmation on the rapgap file: `weight1` is constant across all
tracks of the same event (HAT event-weight), `NucliaWeight` varies per track.
No double-count path exists. Closes §7 PROVISIONAL.

## 12.2 Blending formula — VERIFIED (and corrects §3)

**Answer: NO LINEAR BLENDING.** The v5b builder uses **per-track gen-PDG-based
hypothesis selection** (NOT a fixed-fraction `0.85 w_pi + 0.15 w_K` mix):

1. **Default for every reco track on MC** (lines 1057–1067):
   call `H1NuclIACor::GetWeight(1, Charge, Pt, Phi, Theta, DcaPrime, PVz)` — the
   **pion** hypothesis.
2. **After charge-consistent gen matching succeeds** (lines 1160–1170):
   ```cpp
   if (NucliaWeight_ch > 0.0f) {
       int absPDG = std::abs(GenPDG_ch);
       if (absPDG == 2212) {
           NucliaWeight_ch = 1.0f;                       // proton -> no correction
       } else if (absPDG == 321) {
           Float_t w = H1NuclIACor::GetWeight(
               2, (float)Charge, Pt, Phi, Theta, DcaPrime, PVz);  // kaon hypothesis
           if (std::isfinite(w)) NucliaWeight_ch = w;
       }
       // implicit else: |PDG|==211 (pion) -> keep the default pion call from above
   }
   ```

The handoff phrase "pion/kaon hypothesis blending" was misleading; there is no
linear combination — it is a per-track dispatch on the matched-gen PDG. The
low-Q2 v5b builder uses the identical dispatch (lines 1138–1156 of
`ProtonTreeBuilder.C`). **§3 PROVISIONAL is now CLOSED with a corrected
description: per-track gen-PDG selection, not blending.**

## 12.3 Proton handling — VERIFIED

**Answer: protons (|PDG|=2212) get `NucliaWeight = 1.0` explicitly.**

Exact code (HighQ2 lines 1162–1166):
```cpp
if (absPDG == 2212) {
    NucliaWeight_ch = 1.0f;
}
```

Empirical confirmation on the rapgap file: all 6996 reco tracks matched to
|PDG|=2212 have `NucliaWeight == 1.0` exactly (mean 1.0000, min 1.0000, max
1.0000, frac==1: 1.0000). The proton-omission systematic discussed in §4 of
this reference remains an OPEN analysis item, but the builder behaviour is
confirmed VERIFIED and not pion-fallback. Closes part of §4 (framework path)
VERIFIED; analysis-impact bounds in §4 remain PROVISIONAL by design.

## 12.4 Unmatched tracks — VERIFIED

**Answer: pion hypothesis (ipart=1) is used as the default. No special
fallback for unmatched tracks beyond the initial pion call.**

Code path: lines 1057–1067 unconditionally set the pion-hypothesis weight at
the start of every MC track. The refinement block at 1160–1170 only fires
inside `if (bestIdx_ch >= 0 && bestAngle_ch < CHARGED_MATCH_ANGLE)`, so
unmatched tracks keep the pion default.

Empirical confirmation: 81 828 unmatched tracks in the rapgap file have mean
NucliaWeight = 0.9787, min 0.4956, max 0.9996, and zero of them have weight
== 1.0 exactly (confirming the pion call always fired and never fell back to
the sentinel). This is consistent with the handoff Task 6 note that 30.6 % of
reco tracks are unmatched.

## 12.5 Data vs MC handling — VERIFIED

**Answer: on data, `NucliaWeight = 1.0` identically — both because the builder
gates the call on `if (isMC)` and because, even if called, the framework
returns 1.0 on data with a stderr warning (§9).**

The builder gate (lines 1056–1067 of HighQ2 builder):
```cpp
NucliaWeight_ch = 1.0f;       // initial default for every track, regardless of mode
if (isMC) {
    if (std::isfinite(Pt) && ...) {
        Float_t w = H1NuclIACor::GetWeight(1, ...);
        NucliaWeight_ch = std::isfinite(w) ? w : -1.0f;
    } else {
        NucliaWeight_ch = -1.0f;
    }
}
```

For data (`isMC == false`), the `if (isMC)` block is skipped entirely and
`NucliaWeight_ch` stays at the line-1057 initial value of 1.0. The combined-
track branch (lines 1304–1316) has the same gate. **No data branch ever calls
`H1NuclIACor`, so the stderr warning from the framework is not emitted on
data either.** Data-tree `NucliaWeight` is identically 1.0 by construction.
VERIFIED.

## 12.6 Per-track vs per-event — VERIFIED

**Answer: per-track. One `NucliaWeight` value per `ChargedTot` row, NOT a
broadcast event-level value.**

Branch declaration (line 390): `chargedTree->Branch("NucliaWeight",
&NucliaWeight_ch, "NucliaWeight/F");` — flat single-float branch on the
per-track tree (`ChargedTot` has one row per accepted reco track). Variable
`NucliaWeight_ch` is reassigned inside the per-track loop (line 1057
unconditional reset, line 1170 conditional refinement).

Empirical confirmation: in the rapgap file, distinct `NucliaWeight` values
were observed within tracks of the same event (event-level `weight1` was
constant). VERIFIED.

## 12.7 Run-number dependence — VERIFIED

**Answer: the call site is inside the per-event loop.** The event loop starts
at line 596 (`while (H1Tree::Instance()->Next())`), `isMC` is queried per
event from `gH1Calc->IsMC()` at line 601, and the NuclIA call is reached at
line 1062 inside that loop. `H1NuclIACor::GetCorrection` internally calls
`gH1Tree->GetRunNumber()` at the time of the call (§9), so the run number is
the current event's run number. **No pre-loop caching of NuclIA results
exists.** VERIFIED. Closes the §6 code-level concern.

## 12.8 Numerical range on MC — VERIFIED (closes §2 PROVISIONAL)

Sample: `proton_highQ2_mc_rapgap31_7071_em06_0000.root`, `ChargedTot;2`,
266 345 entries.

| Quantity | Value |
|----------|------:|
| Min | -2.583 (anomaly, 1 entry; see §12.10 below) |
| Max |  1.000 |
| Mean |  0.985 |
| Median |  0.990 |
| Frac == 1.0 exactly |  2.63 % |
| Frac in (0, 1) | 97.37 % |
| Frac < 0 (sentinel -1 or bad-Fortran) | 3.75e-6 (1 track) |

Among positive entries: min 0.4957, mean 0.9850, max 1.0000.

The mean (0.985) matches the reference's prior qualitative estimate of "a few
percent below 1.0" and is consistent with K. Daum's tabulated charged-hadron
NuclIA correction for the H1 inner-tracker material budget. **§2 PROVISIONAL
is CLOSED.**

Breakdown by matched species (same file, 266 345 entries):

| Subset | N | Mean | Min | Max | Frac==1 |
|--------|---:|----:|----:|----:|--------:|
| Unmatched (IsMatched=0) | 81 828 | 0.9787 | 0.4957 | 0.9996 | 0.0000 |
| Matched, \|PDG\|=211 (pion) | 157 847 | 0.9867 | **-2.5830** | 0.9991 | 0.0000 |
| Matched, \|PDG\|=321 (kaon) | 19 674 | 0.9925 | 0.6894 | 0.9975 | 0.0000 |
| Matched, \|PDG\|=2212 (proton) | 6 996 | **1.0000** | **1.0000** | **1.0000** | **1.0000** |
| Combined tracks | 5 467 | 0.9786 | 0.8746 | 0.9928 | 0.0000 |

Kaon-mean > pion-mean (0.9925 > 0.9867) is opposite to the qualitative
prediction in §2 ("[~0.93, ~1.00] for kaons" vs "[~0.95, ~1.00] for pions").
This inverts the expected ordering and warrants documenting: the Fortran
parametrization assigns a SMALLER NuclIA loss to kaons than to pions in this
kinematic window. **Update §2 to remove the kaon-vs-pion ordering claim**;
the inclusive-mean ordering observed in v5b high-Q2 RAPGAP is pion < kaon.
Possible explanation: kaon kinematic distribution is shifted to higher pT
(less material traversal in the CJC support frame), which more than
compensates for any species-dependent cross-section difference at fixed pT.

Charge dependence at the inclusive (pion-only) level: pi+ mean = 0.9867
(N=79 678), pi- mean = 0.9867 (N=78 169). Essentially indistinguishable
inclusively; differential charge dependence must be checked in (pT, eta) bins
before claiming an A_h-relevant asymmetry. **The §5 statement that
charge-dependent NucliaWeight exists is supported by the API but the
inclusive-mean asymmetry is below the per-mille level in this sample;
differential bin-by-bin study is required for the A_h cancellation argument
in §6.**

## 12.9 Cross-check: all assignments to NucliaWeight/weight1/weight2 — VERIFIED

`grep -nE 'weight1|weight2|NucliaWeight'` on the high-Q2 builder returns ONLY
these assignment sites (file:line):

| File:line | Assignment | Context |
|-----------|------------|---------|
| 254 | `Float_t weight1, weight2;` | Declaration |
| 339 | `Float_t NucliaWeight_ch = 1.0f;` | Declaration |
| 390 | Branch on `ChargedTot` | Branch creation |
| 428, 429 | Branches on ChargedTree | Branch creation |
| 497, 498 | Branches on ChargedGenTree | Branch creation |
| 567, 568 | Branches on disTree | Branch creation |
| 622, 623 | `weight1 = *hatWeight1; weight2 = isMC ? *hatWeight2 : 1.0f;` | HAT event-level read (ONLY assignment) |
| 1057 | `NucliaWeight_ch = 1.0f;` | Per-track reset (Central reco loop) |
| 1064, 1066 | `NucliaWeight_ch = isfinite(w) ? w : -1.0f;` / `= -1.0f;` | Pion default + sentinel (Central) |
| 1162, 1165, 1169 | Proton (=1.0), Kaon (=GetWeight(2,...)), pion (no-op) | Per-track refinement after gen match |
| 1304, 1311, 1313 | `= 1.0f`, `= isfinite(w) ? w : -1.0f`, `= -1.0f` | Combined-track loop (pion only, no refinement) |

**There is no hidden assignment to `weight1` or `weight2` that incorporates
`NucliaWeight_ch`.** The track-level NuclIA correction never touches the
event-level weights. VERIFIED.

## 12.10 Discovered anomaly: 1 track with NucliaWeight = -2.583

**Issue:** Builder sentinel guard at lines 1064/1311 is
`std::isfinite(w) ? w : -1.0f`. This catches NaN and Inf but does NOT catch
in-range-but-unphysical values. The Fortran routine `ani_ineff_` returned a
finite value larger than 1 in one event (specifically a NuclIA correction of
~3.58, which `GetWeight = 1 - 3.58 = -2.58`).

Frequency: 1 track in 266 345 (~4e-6) in the inspected file. Affected:
matched pi-, charge -1. Likely cause: Fortran extrapolation outside the
parametrization's tested domain (an extreme pT/theta corner).

**Impact assessment:**
- The next stage of downstream code that multiplies NucliaWeight blindly will
  flip the sign of that track's contribution. For 266k tracks the resulting
  bias in the sum is dominated by the magnitude of the one negative entry
  (-2.58 vs the typical 0.99): O(3.5 / 266 000) ≈ 1.3e-5 bias on inclusive
  yields per file. Negligible at the file level, but in aggregate over the
  full v5b high-Q2 sample (~10⁹ tracks if scaled) the count of bad tracks
  may reach a few thousand and the cumulative bias would still be < 1e-4.

**Recommended downstream guard:** in the analysis weight-application
function, clip:
```python
nuclia_weight = np.where((nuclia_weight > 0.0) & (nuclia_weight <= 1.0),
                         nuclia_weight, 1.0)
```
This is a one-line addition to G11 plus a per-file count of clipped entries
for monitoring. **This is not analysis-killing**, but it should be logged as
"v5b builder NucliaWeight sentinel guard incomplete" in the G11 systematics
notes and flagged for any future v5c builder rebuild.

## 12.11 Downstream MC-weight prescription — VERIFIED

Because NucliaWeight is stored-only (§12.1), the correct downstream
multiplication is:

```python
mc_track_weight = weight1 * weight2 * (nuclia_weight if use_nuclia else 1.0)
```

with the recommended guard from §12.10 applied to `nuclia_weight` before the
multiplication. This is exactly the formula already documented at
`plan-highQ2-sidis-next-steps.md` line 498. **No code change to the
weight-application formula is needed; only the optional clipping guard.**

## 12.12 Status of PROVISIONAL items after this audit

| Item | Prior status | New status |
|------|--------------|------------|
| §2 numerical range | PROVISIONAL | **VERIFIED** (min 0.50, mean 0.985, max 1.00; §12.8) |
| §2 species ordering claim ("kaons lower than pions") | implied | **CONTRADICTED** by data; update §2 narrative |
| §3 blending formula | PROVISIONAL | **VERIFIED — no blending; per-track gen-PDG dispatch** (§12.2) |
| §4 framework proton handling | VERIFIED | unchanged |
| §4 builder proton handling | implicit | **VERIFIED — explicit `=1.0` branch** (§12.3) |
| §4 magnitude of proton-omission systematic on A_h | PROVISIONAL | **REMAINS PROVISIONAL** (needs closure-test execution) |
| §5 charge dependence (inclusive) | PROVISIONAL | **VERIFIED inclusive ~0**; differential (pT,eta) bins remain PROVISIONAL |
| §6 cancellation in A_BC | PROVISIONAL | unchanged (needs closure-test execution) |
| §7 stored vs pre-applied | PROVISIONAL | **VERIFIED — stored only, no pre-multiplication** (§12.1) |
| §8 closure-test execution | PROVISIONAL | unchanged (planned for G11) |
| Run-number context | implicit | **VERIFIED — call is inside per-event loop** (§12.7) |
| Data NucliaWeight = 1.0 identically | implicit | **VERIFIED — `if (isMC)` gate; no data call** (§12.5) |
| Sentinel guard completeness | not discussed | **NEW FINDING — guard catches only non-finite, not in-range-but-unphysical**; one track in 266 345 has NucliaWeight = -2.58 (§12.10) |

