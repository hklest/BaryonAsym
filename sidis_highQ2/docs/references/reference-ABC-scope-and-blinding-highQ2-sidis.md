<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: A_BC Scope and Blinding for High-Q2 Charged-Particle SIDIS

**Status:** Definitional memo for gate G15.5 of `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` (r3).
**Scope:** High-Q2 charged-particle SIDIS only (`Q2 > 150 GeV2`) in the shared `sidis_lowQ2/` workspace.
**Role:** Freeze what `A_BC` is, which variants are headline, how the observable is binned, how it is blinded, and which control expectations govern interpretation.
**Non-role:** This memo does **not** define the implementation procedure for extracting corrected yields or computing the observable chain. That execution logic is governed by the plan and downstream analysis artifacts.

## 1. Purpose

`A_BC` is the beam-charge asymmetry observable for the high-Q2 charged-particle SIDIS analysis. It compares luminosity-weighted corrected yields measured in e+p and e-p running.

Its primary physics motivation is sensitivity to beam-charge-odd effects in DIS, with explicit interest in two-photon exchange (TPE). In the inclusive DIS control channel, a known small but non-zero contribution from gamma-Z interference exists at high `Q2`, especially for `Q2 > 1000 GeV2`. That electroweak contribution is not the target hadronization-level signal and must be distinguished from any TPE interpretation.

This memo therefore freezes the following analysis contract:

1. the four `A_BC` observables adopted by the plan;
2. the binning convention for hadronic and inclusive control variants;
3. the luminosity-weighted yield convention;
4. the cancellation expectations that differ from `A_h`;
5. the blinding rules;
6. the mandatory control and closure tests required before unblinding or interpretation.

This memo inherits gate dependencies from plan r3 and does not restate the full gate tree.

## 2. Observable definitions

### 2.1 General definition

Every `A_BC` value is a bounded ratio of the form

```math
A_BC = \frac{N_{e+p} - N_{e-p}}{N_{e+p} + N_{e-p}},
```

with observable-specific definitions of the corrected yields `N_{e+p}` and `N_{e-p}`.

For every variant in this memo:

- the observable range is bounded to `[-1, 1]` by construction;
- `N_{e+p}` and `N_{e-p}` are beam-charge-separated corrected yields;
- corrected yields are defined after the nominal acceptance treatment and after applying `NuclIA` and `C_rad` where applicable;
- `NuclIA` and `C_rad` must be treated as beam-charge-specific whenever the correction semantics are beam-charge-dependent;
- the beam-charge comparison is performed from per-period luminosity-weighted yields, not from raw event counts.

The symbol `N(...)` in the formulas below therefore denotes the final corrected, luminosity-weighted yield in the stated bin and beam-sign sample, unless explicitly stated otherwise.

### 2.2 Headline hadronic variant: positive hadrons

```math
A_BC(h+, Q2, x, z, pT_h) = \frac{N_{e+p}(h+) - N_{e-p}(h+)}{N_{e+p}(h+) + N_{e-p}(h+)}
```

Yield definition:

- `N_{e+p}(h+)` is the corrected positive-hadron yield in the specified `(Q2, x, z, pT_h)` bin, formed from the e+p periods only;
- `N_{e-p}(h+)` is the corrected positive-hadron yield in the same bin, formed from the e-p periods only;
- each corrected yield includes the nominal acceptance correction and the nominal `NuclIA` and `C_rad` treatment, with beam-charge-specific correction factors where required by the correction model;
- the period combination is performed through the luminosity-weighted prescription in Section 4.

Binning:

- the nominal binning is the multiplicity-grade 4D grid `(Q2, x, z, pT_h)` frozen at G5;
- this observable inherits that grid unchanged unless the contingency trigger defined in Section 3.3 fires.

Range:

- `A_BC(h+, Q2, x, z, pT_h) ∈ [-1, 1]`.

### 2.3 Headline hadronic variant: negative hadrons

```math
A_BC(h-, Q2, x, z, pT_h) = \frac{N_{e+p}(h-) - N_{e-p}(h-)}{N_{e+p}(h-) + N_{e-p}(h-)}
```

Yield definition:

- `N_{e+p}(h-)` is the corrected negative-hadron yield in the specified `(Q2, x, z, pT_h)` bin, formed from the e+p periods only;
- `N_{e-p}(h-)` is the corrected negative-hadron yield in the same bin, formed from the e-p periods only;
- each corrected yield includes the nominal acceptance correction and the nominal `NuclIA` and `C_rad` treatment, with beam-charge-specific correction factors where required by the correction model;
- the period combination is performed through the luminosity-weighted prescription in Section 4.

Binning:

- the nominal binning is the multiplicity-grade 4D grid `(Q2, x, z, pT_h)` frozen at G5;
- this observable inherits that grid unchanged unless the contingency trigger defined in Section 3.3 fires.

Range:

- `A_BC(h-, Q2, x, z, pT_h) ∈ [-1, 1]`.

### 2.4 Headline control-plus-retained hadronic variant: charge-summed

```math
A_BC(sum, Q2, x, z, pT_h) = \frac{N_{e+p}(h+ + h-) - N_{e-p}(h+ + h-)}{N_{e+p}(h+ + h-) + N_{e-p}(h+ + h-)}
```

Yield definition:

- `N_{e+p}(h+ + h-)` is the corrected charge-summed hadron yield in the specified `(Q2, x, z, pT_h)` bin, formed from the e+p periods only;
- `N_{e-p}(h+ + h-)` is the corrected charge-summed hadron yield in the same bin, formed from the e-p periods only;
- the charge-summed corrected yield is the sum of the corrected positive- and negative-hadron contributions inside the same bin under the nominal correction chain;
- acceptance, `NuclIA`, and `C_rad` corrections remain part of the corrected yield definition;
- `C_rad` for this observable is the dedicated charge-summed `A_BC` control correction and is not interchangeable with either multiplicity `C_rad` or the charge-separated `A_BC(h+)` / `A_BC(h-)` corrections.

Binning:

- the nominal binning is the multiplicity-grade 4D grid `(Q2, x, z, pT_h)` frozen at G5;
- this observable inherits that grid unchanged unless the contingency trigger defined in Section 3.3 fires.

Range:

- `A_BC(sum, Q2, x, z, pT_h) ∈ [-1, 1]`.

### 2.5 Inclusive DIS control variant

```math
A_BC^{DIS}(Q2, x) = \frac{N_{e+p}^{DIS} - N_{e-p}^{DIS}}{N_{e+p}^{DIS} + N_{e-p}^{DIS}}
```

Yield definition:

- `N_{e+p}^{DIS}` is the corrected inclusive DIS yield in the specified `(Q2, x)` bin, formed from e+p periods only;
- `N_{e-p}^{DIS}` is the corrected inclusive DIS yield in the same `(Q2, x)` bin, formed from e-p periods only;
- the inclusive DIS control must use the zero-hadron-inclusive `DisTot` event definition, not a hadron-conditioned subset;
- the corrected yield definition follows the nominal inclusive DIS acceptance and correction semantics used for the high-Q2 chain;
- where `NuclIA` is inapplicable to the inclusive DIS denominator by construction, the yield definition remains governed by the DIS correction chain only;
- where `C_rad` is applied, it must preserve beam-charge separation for e+p and e-p.

Binning:

- the inclusive DIS control is binned in 2D `(Q2, x)` only;
- it does not inherit the hadronic `(z, pT_h)` dimensions.

Range:

- `A_BC^{DIS}(Q2, x) ∈ [-1, 1]`.

## 3. Binning convention

### 3.1 Nominal hadronic grid

The three hadronic `A_BC` variants

- `A_BC(h+)`,
- `A_BC(h-)`,
- `A_BC(sum)`

inherit the multiplicity-grade 4D grid `(Q2, x, z, pT_h)` frozen at G5 of plan r3.

This memo does not redefine that grid and does not duplicate its numerical edges. The authoritative hadronic binning is whatever G5 freezes for the multiplicity-grade 4D SIDIS analysis.

### 3.2 Inclusive DIS control grid

The inclusive control `A_BC^{DIS}` is defined on a 2D `(Q2, x)` grid only.

This control is intentionally hadron-independent. It is the beam-charge control observable used for:

- e- recovery validation at G15;
- gamma-Z reference comparison;
- protection against misinterpreting an inclusive electroweak beam-charge asymmetry as a hadronic TPE-like effect.

### 3.3 Contingency-coarsened fallback rule

A coarsened hadronic `A_BC` grid is **not** predefined in this memo.

The fallback exists only if the plan’s statistics contingency trigger fires after full e- recovery. The trigger is:

- invoke a coarser `A_BC` grid only if, after full recovery, **more than 20% of populated 4D bins have e- yield below 25 events**.

If this trigger is met:

- coarsening applies to `A_BC` only;
- multiplicities and `A_h` remain on the nominal grid;
- the coarsened grid must be defined by the gated analysis workflow and recorded in the corresponding G15 artifact.

This memo intentionally does not pre-freeze the fallback bin edges.

## 4. Luminosity weighting

### 4.1 Per-period normalization convention

`A_BC` is defined from per-period luminosity-weighted yields.

For each period,

```math
N_{period}(...) = \frac{N_{period}^{measured}(...) }{L_{period}}
```

where:

- `N_{period}^{measured}(...)` is the corrected measured yield in the period and analysis bin;
- `L_{period}` is the integrated luminosity for that period.

The beam-sign samples are then formed by summing the period-normalized yields separately over the e+p and e-p running periods:

```math
N_{e+p}(...) = \sum_{p \in \{ep0304,\, ep0607\}} \frac{N_{p}^{measured}(...) }{L_p}
```

```math
N_{e-p}(...) = \sum_{p \in \{em0405,\, em06\}} \frac{N_{p}^{measured}(...) }{L_p}
```

This convention is mandatory for all `A_BC` variants in this memo.

### 4.2 Periods included in the beam-charge sums

The e+p beam-charge class is defined as the sum of:

- `ep0304`
- `ep0607`

The e-p beam-charge class is defined as the sum of:

- `em0405`
- `em06`

No other period grouping is part of the nominal `A_BC` definition.

### 4.3 Per-period luminosities

The nominal per-period luminosities are those published in the GRL and luminosity artifacts produced by the high-Q2 recovery chain, specifically the task-3 / G15-output manifest and luminosity table, and are authoritative from `oolumi` on `SelectedRuns.root`.

The working period values currently documented in the high-Q2 status snapshot are:

- `L(ep0304) = 53,706 nb^-1`
- `L(em0405) = 105,028 nb^-1`
- `L(em06) = 57,573 nb^-1`
- `L(ep0607) = 140,306 nb^-1`

These values must be taken from the finalized GRL/luminosity manifest artifact at use time. If the finalized GRL artifact updates any period total, the artifact supersedes the snapshot values above.

### 4.4 Luminosity uncertainty handling

The per-period luminosity uncertainty for `A_BC` is defined as:

- `2.5%` flat per period;
- symmetric;
- correlated across all bins within that period.

This uncertainty is propagated as a correlated systematic source in all `A_BC` variants.

It does **not** cancel in `A_BC`, because the numerator and denominator are built from beam-charge-separated luminosity-weighted yields.

## 5. Cancellation expectations

### 5.1 Scope of the cancellation statements

The statements below define the required interpretation of first-order cancellation in `A_BC`. They are part of the observable definition contract and must be used consistently with the high-Q2 systematics framework.

These statements distinguish `A_BC` from `A_h`. They do not replace explicit closure tests.

### 5.2 Charge-summed beam-charge asymmetry

For `A_BC(sum)`:

- beam-charge-symmetric detector effects cancel to first order;
- common tracking efficiency cancels to first order;
- common acceptance cancels to first order.

These are first-order cancellation expectations, not permission to omit explicit validation.

### 5.3 Charge-fixed beam-charge asymmetries

For `A_BC(h+)` and `A_BC(h-)`:

- charge-dependent tracking does **not** cancel;
- selecting a fixed hadron charge removes the `h+` versus `h-` averaging that would otherwise cancel the common part of charge-dependent tracking effects;
- the cancellation argument applies only to beam-charge-symmetric detector effects, not to hadron-charge-dependent tracking asymmetries.

This distinction is mandatory and must not be softened in later reporting.

### 5.4 Luminosity does not cancel

For all `A_BC` variants:

- per-period luminosity uncertainties do **not** cancel;
- they enter as correlated systematic uncertainties through the beam-charge-separated period-normalized yields.

### 5.5 NuclIA cancellation is conditional, not assumed

For all `A_BC` variants:

- `NuclIA` cancels only if its parametrization is beam-symmetric;
- beam-symmetry of the parametrization must not be assumed from the name of the correction;
- the required closure test is the explicit `NuclIA-on` minus `NuclIA-off` comparison in the relevant `A_BC` channel.

If the correction is beam-symmetric within the tested precision, the residual may be small. The cancellation claim is still earned only by closure.

### 5.6 Radiative corrections are beam-charge specific

For all `A_BC` variants:

- `C_rad` differs by beam charge because the sign of interference terms differs between e+p and e-p;
- `C_rad` must therefore be derived separately for e+p and e-p;
- for `A_BC(h+)` and `A_BC(h-)`, the charge-separated hadron-channel corrections are also distinct from one another;
- the e+p-to-e-p correction ratio enters `A_BC` non-trivially and is part of the observable definition chain.

Reusing an inclusive or beam-averaged `C_rad` is not consistent with this memo.

## 6. Physics expectations

### 6.1 Inclusive DIS expectation scale

In inclusive DIS, TPE-induced `A_BC` in the high-Q2 regime is expected at the `10^-3` level.

This expectation is literature-driven. The precise citation set for the internal review note is **citation TBD** unless one or two dedicated references are frozen separately. The present memo freezes only the scale expectation required for gate interpretation.

### 6.2 Gamma-Z interference expectation

Gamma-Z interference produces a `Q2`-dependent inclusive `A_BC` contribution at the `10^-3` level for `Q2 > 1000 GeV2`.

This electroweak contribution is part of the expected inclusive DIS baseline. It must be:

- subtracted, or
- corrected for,

before interpreting any residual inclusive beam-charge asymmetry as a TPE contribution.

For hadronic `A_BC` interpretation, the inclusive gamma-Z baseline is the control reference, not the sought hadronization observable.

### 6.3 Hadronic-side expectation

For hadronic-side `A_BC(h+)`, `A_BC(h-)`, and `A_BC(sum)` at HERA energies:

- no first-principles prediction is adopted in this memo;
- no mature HERA-era benchmark expectation is frozen here for the hadronization-level beam-charge asymmetry in 4D SIDIS bins;
- the measurement is treated as exploratory.

This exploratory status makes the result physics-motivated but blinding-sensitive. The absence of a firm hadronic benchmark is not a license to relax control requirements.

### 6.4 Interpretation boundary

The interpretation boundary is therefore fixed as follows:

- `A_BC^{DIS}(Q2, x)` is the inclusive beam-charge control quantity;
- agreement of `A_BC^{DIS}` with the electroweak baseline is a closure requirement;
- hadronic `A_BC` values may be discussed as exploratory hadronization-level beam-charge asymmetries only after that control passes.

## 7. Blinding policy

### 7.1 Default blinding state

All four `A_BC` variants are blinded by default during construction:

- `A_BC(h+)`
- `A_BC(h-)`
- `A_BC(sum)`
- `A_BC^{DIS}` is the only exception and is handled separately in Section 7.4.

For the three hadronic variants, the blinded state is mandatory until the unblinding conditions in Section 7.5 are met.

### 7.2 Meaning of “blinded”

For blinded hadronic `A_BC` observables:

- the per-bin numerator may be computed;
- the per-bin denominator may be computed;
- the underlying beam-charge-separated corrected yields may be stored;
- the per-bin `A_BC` values themselves must **not** be examined directly.

This prohibition includes direct numerical tables, plots, debug printouts, or derived summaries that reveal the central values.

### 7.3 Acceptable blinding mechanism

The acceptable blinding mechanism is:

- store per-bin yields independently;
- store per-bin systematic components independently;
- defer the final `A_BC` division until an explicit unblinding decision is recorded.

Equivalent bookkeeping mechanisms are allowed only if they enforce the same practical restriction: no direct access to per-bin hadronic `A_BC` values before unblinding.

### 7.4 Inclusive DIS control is not blinded

The inclusive DIS control

```math
A_BC^{DIS}(Q2, x)
```

is **not blinded**.

Its role is functional and mandatory:

- it is the closure quantity that proves e- recovery at G15;
- it is the gamma-Z reference quantity required for G15.5;
- it must agree with the gamma-Z prediction at the per-mille level to pass G15.

This exception applies only to the inclusive DIS control. It does not authorize inspection of hadronic `A_BC` values.

### 7.5 Unblinding decision trigger

Unblinding of the hadronic `A_BC` variants is triggered only after all of the following are true:

- G14 has passed for the normalization context relevant to the packaged result;
- G15 has passed for e- recovery and period validation;
- G15.5 has passed for `A_BC` physics validation and gamma-Z/TPE separation;
- the `A_BC` systematics budget is finalized.

No earlier milestone authorizes unblinding.

### 7.6 Unblinding sign-off path

The unblinding sign-off path is frozen structurally but not yet person-named:

- approving authority: **SIDIS supervisor sign-off person TBD**;
- decision record artifact: a dedicated sign-off record in the G15.5 or package-level review artifact, explicitly naming date, approver, scope of unblinding, and artifact versions.

Until the approver is named, the sign-off path remains incomplete but the blinding rule remains fully in force.

## 8. Required closure tests

The following closure tests are mandatory parts of the `A_BC` definition and interpretation framework.

### 8.1 NuclIA closure

Required test:

- compute `A_BC(h+)` with `NuclIA` enabled;
- compute `A_BC(h+)` with `NuclIA` disabled;
- report the magnitude of the difference.

This test establishes whether `NuclIA` is effectively beam-symmetric in the realized correction chain.

The same logic may be extended to the other `A_BC` variants, but the mandatory named closure test in this memo is the `A_BC(h+)` on/off comparison.

### 8.2 Period closure

Required test:

- evaluate per-period `A_BC^{DIS}` stability across runs within each period;
- require the result to be flat within the stated uncertainty.

This is a beam-recovery and luminosity-consistency closure, not a hadronic physics result.

### 8.3 Gamma-Z closure

Required test:

- evaluate `A_BC^{DIS}(Q2 > 1000 GeV2)`;
- require consistency with the electroweak prediction at the `10^-3` level.

This is the core control test preventing misinterpretation of the inclusive beam-charge signal.

### 8.4 C_rad closure

Required test:

- compute `A_BC(h+)` with charge-separated `C_rad`;
- compute `A_BC(h+)` without charge-separated `C_rad`;
- require the difference to match a known beam-charge-asymmetric correction effect.

The sign and scale of the difference are part of the expected radiative-correction behavior. A null-difference expectation is not permitted.

## 9. Systematic uncertainty sources mapped to A_BC

The full source table is defined in plan r3 and in `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md`. This memo does not duplicate that table.

For `A_BC`, the SIDIS-specific pitfalls that must be called out explicitly are:

1. **Per-period luminosity uncertainty**
   - does not cancel;
   - enters as a correlated beam-charge normalization systematic.

2. **Period-mixing in MC weights or data bookkeeping**
   - does not cancel;
   - can fake beam-charge structure if e+p and e-p periods are merged incorrectly.

3. **NuclIA beam-asymmetry risk**
   - cancellation may not be assumed;
   - closure is required with `NuclIA` toggled.

4. **C_rad beam-asymmetry risk**
   - cancellation may not be assumed;
   - e+p and e-p corrections must remain distinct because interference terms change sign.

These four points are the minimum `A_BC`-specific systematic interpretation overlay that must accompany the master systematics table.

## 10. Reporting requirements

No `A_BC` value may be quoted unless the report includes all of the following:

1. bin definition;
2. per-period yields;
3. per-period luminosities;
4. `NuclIA`-applied flag;
5. `C_rad`-applied flag;
6. gamma-Z-subtracted flag;
7. statistical uncertainty;
8. systematic uncertainty broken out by source;
9. total uncertainty.

These requirements apply to:

- internal tables;
- plots with machine-readable backing;
- review notes;
- publication-draft tables.

Any `A_BC` number lacking this metadata is non-compliant with the observable definition memo and must not be cited as a result.

## 11. Gate linkage and authoritative artifacts

This memo is written to satisfy the definitional part of G15.5 in plan r3.

Authoritative supporting artifacts are:

- plan r3: `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md`
- shared analysis state: `H1/sidis_lowQ2/context-handoff.md`
- production state snapshot: `H1/sidis_highQ2/docs/research/research-highQ2-status-snapshot-2026-05-19.md`
- systematics framework: `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md`
- finalized GRL / luminosity outputs from the high-Q2 recovery chain, including the task-3 / G15 good-run-list manifest and luminosity table

If a future artifact conflicts with this memo on implementation detail, this memo controls the observable definition, blinding rule, and interpretation boundary unless superseded by an explicitly approved replacement memo.
