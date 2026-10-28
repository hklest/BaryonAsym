# Electron Systematics Methodology for High-Q² Charged-Particle SIDIS

## Purpose

This document defines a durable, implementation-ready methodology for evaluating **electron-related detector systematics** in the H1 high-Q² charged-particle SIDIS analysis for corrected yields, multiplicities, and cross-section-like diagnostic products. It is written to stay valid across production refreshes and binning revisions.

Before using this document for any concrete run, read these runtime sources first:

1. `H1/CONTEXT.md`
2. `sidis_highQ2/context-handoff.md`
3. `sidis_highQ2/docs/RUN_STATUS.md`

Use those files for the current source tree, builder names, active outputs, binning status, and blocked items. Do **not** treat any older production label, release tag, or path cited elsewhere in the workspace as authoritative unless the runtime files above still endorse it.

## Scope

This methodology covers electron-related detector systematics that can change reconstructed event kinematics, bin assignment, acceptance, and unfolding response in the high-Q² charged-particle SIDIS analysis, including:

1. scattered-electron energy scale;
2. scattered-electron polar angle (`theta_e`);
3. scattered-electron azimuth (`phi_e`) when the observable depends on the lepton plane, the virtual-photon direction, the `\gamma^* p` boost, or `phi_h`;
4. electron identification, fiducial, and trigger-related electron selections **only when they are implemented as electron-side detector variations** rather than as pure normalization or trigger-efficiency sources.

This document applies to corrected detector-level products derived through BBB, IBU, TUnfold, or equivalent migration-aware workflows. It is especially intended for products that depend on eSigma kinematics and photon-axis hadron observables.

## Out Of Scope And Delegation Boundaries

- **Normalization constants, luminosity, and weight semantics** belong to `h1-normalization`.
- **MC truth semantics, generator meaning, truth-bank interpretation, BGcheck semantics, and radiative generator conventions** belong to `h1-mc-expert`.
- **Execution on NAF, Condor submission, remote orchestration, merges, and file movement** belong to `h1-script-runner` and `h1-naf-operator`.
- **Final global combination of all systematic sources into release tables and final covariance products** belongs to `h1-systematics`.
- **Physics-method choice for the nominal SIDIS observable definition** belongs to `h1-physics-analyst`.
- **Read-only adversarial review of whether a proposed implementation is physically safe** belongs to `h1-analysis-critic`.

This document gives the methodology and required checks; it does not replace those specialists.

## Durable Analysis Conventions

Unless a newer runtime authority explicitly supersedes them, preserve these charged-SIDIS conventions:

- high-Q² charged SIDIS uses **eSigma-style DIS kinematics** as the baseline reconstruction;
- hadron truth content is restricted to **charged π/K/p**, with **no species PID** in reconstructed observables;
- `pT_h` is the **photon-axis** transverse momentum in the `\gamma^* p` frame, not lab `p_T`;
- `z` is the **photon-axis / SIDIS-frame** hadron energy fraction;
- `phi_h` is defined relative to the lepton plane and virtual-photon axis, so electron-angle shifts can matter even when their effect on inclusive yields is small;
- some current products are diagnostic or partial by construction, e.g. raw, no-background, or no-`C_rad` products. Keep their labels explicit.

## Physics Principle

Electron-related detector systematics are **migration-affecting** whenever the shift changes any reconstructed quantity that can alter:

- event selection;
- fiducial acceptance;
- `(Q², x, y)` bin assignment;
- hadron `(z, pT_h, phi_h)` bin assignment through the virtual-photon 4-vector or `\gamma^* p` boost;
- the response matrix used by BBB, IBU, or TUnfold.

For those sources, a post-hoc rescaling of already-corrected bins is not physically valid. The shift must be propagated through the same detector-to-truth correction chain used for the nominal result.

## Electron Sources And Their Propagation Class

### 1. Electron Energy Scale

Treat the scattered-electron energy scale as **migration-affecting**.

Why:

- it changes reconstructed `Q2es`, `Yes`, and `x_es`;
- it changes the virtual-photon 4-vector `q = k - k'`;
- it changes the `\gamma^* p` boost used for `z`, `pT_h`, and possibly `phi_h`;
- it can move events across electron-energy, `E-p_z`, `y`, or DIS-threshold cuts;
- it changes the reco-side population entering the response matrix.

### 2. Electron Polar Angle (`theta_e`)

Treat the electron polar-angle source as **migration-affecting**.

Why:

- it changes `Q2es`, `Yes`, `x_es` through the electron direction;
- it rotates the virtual-photon direction and lepton plane;
- it changes the `\gamma^* p` frame and therefore `z`, `pT_h`, and `phi_h`;
- it can move events across electron fiducial and angular acceptance cuts.

### 3. Electron Azimuth (`phi_e`)

Treat the electron-azimuth source as **migration-affecting whenever** any target observable or intermediate quantity depends on the lepton plane or `q` orientation.

For this analysis, that includes at least:

- `phi_h`-sensitive observables;
- any observable computed after a `\gamma^* p` boost using the shifted `q` direction;
- any workflow that compares or bins in `phi_h`, harmonic moments, or related frame observables.

For purely `phi_h`-integrated yield products, the physical effect is usually smaller than for energy or `theta_e`, but it is still not automatically a post-hoc nuisance. The correct classification is still migration-affecting unless a dedicated derivation proves that the full detector-level propagation collapses to a negligible analytic effect in the exact analysis chain.

### 4. Electron ID / Fiducial / Trigger-Related Electron Selections

These split into two classes.

#### 4a. Migration-affecting detector variations

If the source is implemented as a change in the reconstructed electron object or its cut boundary, such as:

- electron fiducial edge movement;
- electron-ID discriminant threshold movement;
- electron-trigger selection tied to electron kinematics or acceptance;
- electron-track matching or electron cluster definition changes;

then propagate it through the **full rerun chain**. It changes which reco events enter the data vector and response.

#### 4b. Pure normalization or efficiency weights

If the source is represented as a well-validated, bin-independent or weight-only efficiency factor that does **not** move bin boundaries or event acceptance definitions, it may belong to a separate trigger or normalization treatment rather than the electron-detector migration set. In that case, route the source design to `h1-normalization` or the relevant trigger owner.

Do not silently move a migration-affecting electron selection into a post-hoc normalization bucket just because the integrated effect looks small.

## Kinematic Dependency Map

Electron shifts propagate through the analysis by changing the scattered-electron 4-vector `k'`.

### DIS Kinematics

Let the incoming electron and proton 4-vectors be `k` and `P`, and the scattered-electron 4-vector be `k'`. Then

```text
q = k - k'
Q² = -q²
x = Q² / (2 P·q)
y = (P·q) / (P·k)
```

For the eSigma reconstruction actually used in this analysis, the implementation details may differ, but the dependency is the same: shifts in electron energy or direction change reconstructed DIS kinematics.

In practice, future agents must verify the exact branch and formula names in the current builder/extractor, but should expect electron-systematic sensitivity in:

- `Q2es`
- `Yes`
- `x_es` or equivalent Bjorken-x branch
- `Empz`-related acceptance if the electron enters the event `E-p_z` sum or a decomposition built from it

### Photon-Axis Hadron Variables

Because the hadron observables are defined relative to the virtual photon and the `\gamma^* p` frame, electron shifts also modify:

- `q` direction;
- `W = q + P` and the associated boost;
- photon-axis `z`;
- photon-axis `pT_h`;
- `phi_h`, especially for `theta_e` and `phi_e` variations.

This is why electron sources are not just DIS-bin nuisances. They also change hadron migration inside a fixed `(Q², x)` stratum.

## Canonical Propagation Rule

For any migration-affecting electron source, use this propagation chain:

```text
shifted reco-level electron variables
→ shifted event selection / fiducial decision
→ shifted reco data vector
→ shifted MC reco quantities
→ shifted response / acceptance object
→ rerun BBB / IBU / TUnfold correction
→ compare shifted corrected result to nominal
```

Equivalently:

```text
ROOT/tree-level shift → rebuilt data vector and response → rerun correction/unfolding
```

Do **not** do this instead:

```text
nominal unfolded bins → add post-hoc electron bin shift
```

unless a source has already been proven to be strictly non-migratory in the current chain.

## Data And MC Treatment

### General Rule

Apply the detector systematic consistently to:

1. **data reco side**, because the data measurement is what is being reinterpreted under the detector uncertainty;
2. **MC reco side**, because the response matrix must reflect the same reco-level variation;
3. **MC truth side unchanged**, unless the source is explicitly a truth-model variation rather than a detector systematic.

This avoids a false hybrid where the data vector is shifted but the migration model is not, or vice versa.

### For BBB

If BBB is used as a diagnostic or comparison method, rebuild the acceptance or efficiency object from the shifted reco-side MC and recompute the shifted corrected result using the shifted reco-side data vector. Do not keep the nominal efficiency table while shifting only the data histogram.

### For IBU / TUnfold

Rebuild the migration matrix and any derived ingredients from the shifted reco-side MC, then rerun the unfolding on the shifted data vector with the same nominal analysis definition unless the systematic itself changes the analysis definition. Keep the truth-side published phase-space definition fixed unless the source is explicitly defined to move that truth acceptance.

### For Cross-Method Comparisons

When comparing BBB, IBU, and TUnfold under an electron variation:

- use the **same shifted reco input** and **same truth definition** for all methods;
- do not compare a shifted TUnfold result to a nominal BBB result;
- make sure any method-specific regularization or prior stays on its intended prescription, with only the detector-side input changed.

## Recommended Implementation Hierarchy

### Preferred Hierarchy

1. **Best**: apply the variation at ROOT/tree-builder or equivalent reco-object level, then rerun the extractor and response build.
2. **Acceptable only if validated**: apply the variation in an intermediate ROOT or columnar representation that still contains all information required to recompute electron-dependent cuts, DIS kinematics, `q`, `\gamma^* p` boost, and hadron variables.
3. **Not acceptable**: apply the variation only to already-binned corrected outputs.

### CSV / Flat-Table Warning

Do not silently fall back to a CSV-only implementation if the current flat table lacks branches needed to recompute:

- the scattered-electron 4-vector;
- event-level `Empz` decomposition or equivalent;
- hadron lab-frame momentum needed for a fresh `\gamma^* p` boost;
- cut-defining quantities;
- event identity needed to regroup tracks by event.

If required branches are absent, **stop** and rebuild from ROOT or extend the extractor. The shared workspace rule is explicit: the derivative CSV is not the canonical source for migration-affecting systematics when required branches are missing.

## Nominal And Shifted Products

For each electron source `s`, produce at minimum:

- nominal corrected vector `N_i^nom`;
- upward variation `N_i^{s,+}`;
- downward variation `N_i^{s,-}`.

Here `i` indexes the published bins of the relevant product.

Depending on the product class, `N_i` may mean:

- corrected yield;
- multiplicity numerator or final multiplicity;
- cross-section-like diagnostic quantity;
- asymmetry input yield by charge.

For asymmetries, always vary the underlying charge-separated corrected yields first and form the asymmetry afterward.

## Uncertainty Extraction Formulae

### Per-Bin Signed Shift

Define the signed half-difference:

```text
Δ_i^(s) = (N_i^{s,+} - N_i^{s,-}) / 2
```

and the absolute symmetrized systematic magnitude:

```text
σ_i^(s) = |Δ_i^(s)|
```

This is the default only when the up/down variations behave like a coherent detector response.

### Same-Side / Noise-Dominated Warning

If `N_i^{s,+}` and `N_i^{s,-}` move in the same direction relative to nominal for a substantial fraction of bins, do **not** automatically trust `|N_i^{s,+} - N_i^{s,-}|/2` as the physical systematic. That pattern can indicate that unfolding noise or unstable regularization is dominating the measured difference.

In that case, treat the source as requiring an additional noise-floor diagnosis before final use. At minimum:

- quantify whether the half-difference is signal-dominated or unfold-noise-dominated;
- do not ship a fully coherent rank-1 covariance built from noise-dominated signed shifts.

This is a hard lesson already recorded in prior electron-source critiques in this workspace.

### Covariance For A Coherent Detector Source

If the source passes the coherence and noise-floor checks, construct its covariance as:

```text
C_ij^(s) = Δ_i^(s) Δ_j^(s)
```

This is the correct covariance for a single coherent nuisance parameter only when `Δ_i^(s)` is actually the detector-response shift, not mostly statistical fluctuation.

### Conservative Diagonal Fallback

If a source magnitude is usable but the signed coherence is not defensible, a conservative temporary fallback is:

```text
C_ij^(s) = δ_ij [σ_i^(s)]²
```

where `δ_ij` is the Kronecker delta.

This fallback is less informative and should be clearly labeled as conservative and non-final. It is preferable to a fake fully correlated covariance built from random signs.

### Combination Across Electron Subsources

If the electron-energy, electron-`theta`, electron-`phi`, and electron-ID/fiducial components are treated as independent nuisance sources, combine their covariances as:

```text
C_ij^(electron total) = Σ_s C_ij^(s)
```

with `s` running over the electron subsources.

Do not combine them by taking the largest source per bin and discarding the rest unless a specific envelope prescription is explicitly adopted.

### Combination With Statistical, Model, And Regularization Terms

Keep source-resolved covariance matrices separate until the final combination step. For a product with data-statistical, model, regularization, response-MC-statistical, and electron-detector terms:

```text
C_total = C_stat,data + C_model + C_reg + C_MCstat,response + C_electron + ...
```

Only sum covariance terms that are defined on the same published bin basis and have passed validation.

Do not:

- add correlated and uncorrelated pieces as scalar per-bin percentages only;
- fold unfolding statistical variance into the electron systematic by accident;
- double-count electron effects both as a detector variation and as part of a generic model envelope.

## Asymmetry Treatment And Cancellation Logic

For charge asymmetries such as

```text
A_h = (N^+ - N^-) / (N^+ + N^-)
```

proceed as follows.

1. Compute shifted **charge-separated corrected yields** `N_i^{+,s,±}` and `N_i^{-,s,±}`.
2. Form shifted asymmetries:

```text
A_i^{s,±} = (N_i^{+,s,±} - N_i^{-,s,±}) / (N_i^{+,s,±} + N_i^{-,s,±})
```

3. Define the asymmetry source shift from the shifted asymmetries, not from a guessed cancellation factor.

Electron systematics are often approximately charge-symmetric at first order, so substantial cancellation in `A_h` is physically expected. But do not impose that cancellation analytically without checking the actual shifted charge-resolved outputs.

Conversely, do not use a covariance construction that forbids this cancellation by injecting random binwise signs from noise-dominated shifts.

## Diagnostic Versus Final Product Labels

Every output produced under this methodology must carry one of the following labels:

- **diagnostic**: useful for method development, closure, or cross-method comparison; not a final physics uncertainty;
- **intermediate physics**: physically meaningful but incomplete because one or more required upstream ingredients remain blocked;
- **final-candidate**: all required ingredients for that product class are present and current, pending global combination/review.

Examples:

- a corrected yield without background subtraction or without a valid `C_rad` mapping is **diagnostic** or **intermediate**, not final;
- a cross-section-like quantity produced before the current `C_rad` mapping is re-derived must remain explicitly non-final.

## Validation Gates

A shifted electron-source result is not valid until all applicable gates below pass.

### 1. Schema And Branch Availability

Confirm that the implementation has access to every branch needed to recompute the varied quantities. Required checks include:

- electron 4-vector branches or equivalent;
- event-level quantities needed for eSigma kinematics;
- hadron momentum branches needed for recomputing photon-axis `z`, `pT_h`, and `phi_h`;
- event/group identifiers;
- current cut-defining quantities.

If any required branch is missing, stop. Do not silently approximate with incomplete information.

### 2. No Silent CSV Fallback

If the intended implementation originally required ROOT branches and the flat derivative table lacks them, stop and either:

- extend the extractor; or
- rerun directly from ROOT.

### 3. Same Binning And Fiducial Definition

Nominal and shifted products must use the same published bin definition, same truth phase-space definition, and same fiducial conventions unless the source explicitly changes a reco-side cut by design.

### 4. Response / Data-Vector Consistency

The shifted reco data vector and shifted response must be built from the **same** variation definition. Do not mix:

- shifted data with nominal response;
- nominal data with shifted response;
- one electron variation on data and a different one on MC reco.

### 5. Finite Outputs

Check for:

- NaN or Inf in corrected bins;
- non-finite covariance entries;
- singular or malformed matrices beyond what the nominal method already tolerates.

### 6. Physical Sanity

Check for:

- unphysical acceptance behavior, e.g. pathological values or sign flips where not allowed;
- cross-section-like outputs becoming negative in broad physically populated regions without a known method-specific reason;
- dramatic discontinuities isolated to one variation that indicate implementation failure rather than detector sensitivity.

### 7. Closure / Refold / Stability Checks

For unfolding workflows, run the same class of validation used for the nominal result on the shifted chain:

- closure or pseudo-data tests where appropriate;
- refold residual checks;
- migration-matrix consistency checks;
- regularization stability checks.

A systematic variation does not get to bypass the same health diagnostics required of the nominal unfolding chain.

### 8. Noise-Floor Check For Signed Covariance Use

Before using a signed outer-product covariance, verify that the per-bin signed shifts are not dominated by unfolding noise. Recommended checks include at least one of:

- nominal pseudo-experiment or bootstrap replicas to estimate the zero-shift unfold noise floor;
- repeated unfolding of the same shifted input to separate deterministic shift from solver noise;
- a same-side-bin audit (`up` and `down` both moving with respect to nominal).

If noise is comparable to the measured signed shift, do not use a fully coherent rank-1 covariance without further treatment.

## Failure Modes And Stop Conditions

Stop the workflow and mark the source invalid if any of the following occurs:

1. required branches for recomputing electron-dependent quantities are absent;
2. the implementation would require a silent approximation to `q`, `z`, `pT_h`, or `phi_h` without the needed inputs;
3. nominal and shifted chains use inconsistent binning or fiducial definitions;
4. shifted data and shifted response are produced with inconsistent variation definitions;
5. outputs are non-finite or structurally malformed;
6. the variation produces widespread unphysical behavior with no plausible detector mechanism;
7. the measured source shift is indistinguishable from unfolding noise, yet a fully correlated covariance is still being proposed;
8. the source would be double-counted with another systematic family;
9. a product is being labeled final even though upstream ingredients remain blocked.

When a stop condition is triggered, the correct action is to return to the last valid stage and fix the methodology or inputs, not to smooth over the problem with ad hoc per-bin clipping.

## Recommended Artifacts To Produce

For each electron source, produce at minimum:

1. a short metadata note stating the exact varied quantity, shift size, and implementation level;
2. tables of `N_i^nom`, `N_i^{+}`, `N_i^{-}`, `Δ_i`, `σ_i`, and fractional shifts;
3. source covariance matrix on the published bin basis;
4. heatmaps of fractional shift and covariance / correlation;
5. same-side versus opposite-side bin counts for the up/down response;
6. summaries by `(Q², x)` stratum and by charge;
7. comparison plots for BBB vs IBU vs TUnfold under the same variation;
8. closure/refold diagnostics for the shifted chain when applicable;
9. explicit lists of bins with non-finite, negative, or otherwise pathological outputs;
10. for asymmetries, charge-resolved shifted yields plus the shifted asymmetry itself.

## Minimal Pseudocode

```text
for source in [electron_energy, electron_theta, electron_phi, electron_id_or_fid_if_applicable]:
    read runtime context docs
    verify current nominal chain and current active paths
    verify required branches exist
    if required inputs absent:
        STOP

    build shifted reco data inputs for source up/down
    build shifted reco MC inputs for source up/down
    keep truth definition fixed

    rebuild shifted response/acceptance objects

    rerun correction method(s):
        BBB diagnostic if needed
        IBU if in comparison set
        TUnfold if in comparison set

    validate:
        same binning/fiducial
        finite outputs
        response/data consistency
        closure/refold/stability
        same-side noise audit for signed covariance use

    extract per-bin shifts
    construct source covariance only at the level justified by the validation
    save source-resolved artifacts and label diagnostic/final-candidate status
```

## Practical Rules For Future Agents

- Prefer explicit detector-side reruns over elegant-looking post-hoc shortcuts.
- When uncertain whether a source is migration-affecting, default to **yes** until proven otherwise.
- Never use stale production labels as normative facts; always defer to runtime context files.
- Keep method labels explicit: BBB, IBU, and TUnfold are not interchangeable.
- Keep source labels explicit: electron energy, electron `theta`, electron `phi`, and electron-ID/fiducial are different nuisances.
- Preserve source-resolved covariances; downstream users may need them separately.

## Critic Iteration Log

- **Round 1:** strengthened the migration-affecting rule, added explicit ROOT/tree-level propagation, clarified effects on eSigma kinematics and photon-axis observables, and added formulas plus covariance guidance so the methodology does not collapse into post-hoc bin shifts.
- **Round 2:** tightened the treatment of noise-dominated up/down responses, forbade blindly using rank-1 covariances when same-side behavior indicates unfold noise, clarified asymmetry cancellation handling, and removed stale production dependence by pointing users back to runtime authority files.
