# C_rad statistical-floor study — half-difference vs max(term, k·u_diff) (2026-10-07)

**Companion JSON:** `crad_statfloor_study_2026-10-07.json` (status PASS). **Prospective sizing
only** — no budget, release, response, plot, or C_rad table was modified. The central v6 release
remains raw/no-`C_rad` (`c_rad_applied=false`, not Born-level).

## 1. Motivation

The generator-spread note (`docs/radiative/CRAD_UNC_GENERATOR_SPREAD_2026-10-07.md`, steps 3–4)
adopted `c_rad_gen_rel = 0.5 x GaussSmooth_pTrow(|rel|)` and **rejected a floor** on the C_rad
tables' own MC statistics, ruling that folding them in "would double the statistics into the
model envelope". That ruling targets the *additive* fold. However, the same note documents that
`152/245` (h+) and `153/245` (h-) bins have `|rel| < 2 x u_diff` — the generator difference is
MC-statistically unresolved there — and the B1 precedent floors `ibu_model_rel` with
`max(base, b1_floor)`. This study asks whether an equivalent *max-form* statistical floor is
admissible for the C_rad term, which variant to adopt, and what changes when `c_rad_applied`
flips to true. All numbers below are from the companion JSON; nothing was recomputed.

Definitions of the two legs: `rel = (C_rad_rapgap31 - C_rad_djangoh14)/C_rad_djangoh14` at the
matched xFGen<0, EMPZ>45 definition (490 bins), and
`u_diff = sqrt(u_rel_dj^2 + u_rel_rg^2)` from the shipped per-bin `relative_stat_unc` columns of
the two C_rad tables (`u_rel(C) = sqrt(u_rel_nonrad^2 + u_rel_rad^2)` per generator; designated
`errors/per_bin` in the co-located validation JSONs). `u_diff` is the MC-statistical uncertainty
of the two-generator *difference*.

## 2. Variant definitions

| Variant | Formula |
|---|---|
| V1 | `0.5 x abs(rel)` — raw half-difference, no smoothing, no floor |
| V2(k) | `max(V1, k x u_diff)`, k in {1.0, 1.5} |
| V3 | `0.5 x GaussSmooth_pTrow(abs(rel))` — the adopted house envelope (kernel `exp(-0.5*Delta^2)`, `sigma = 1.0` pT-bin, edge-truncated renormalized within each (charge, cell, z) 5-point pT row; identical construction to `phi_h_gauss_rel`) |
| V4(k) | `max(V3, k x u_diff)`, k in {1.0, 1.5} — house-smoothed envelope with a B1-style difference-noise floor |

## 3. Results (percent; median / p95 / max over 245 bins per charge)

| Quantity | h+ | h- |
|---|---|---|
| `u_diff` (floor input) | `0.62 / 3.26 / 6.26` | `0.65 / 4.09 / 9.12` |
| V1 | `0.42 / 2.72 / 7.22` | `0.45 / 2.19 / 9.16` |
| V2(1.0) — binds `152/245` | `0.75 / 3.98 / 7.22` | `0.81 / 4.15 / 9.16` — binds `153/245` |
| V2(1.5) — binds `185/245` | `1.04 / 4.98 / 9.39` | `1.07 / 6.13 / 13.68` — binds `189/245` |
| V3 (adopted envelope) | `0.53 / 2.17 / 5.87` | `0.50 / 2.08 / 5.83` |
| V4(1.0) — binds `139/245` | `0.74 / 3.74 / 6.26` | `0.79 / 4.15 / 9.12` — binds `150/245` |
| V4(1.5) — binds `177/245` | `0.96 / 4.89 / 9.39` | `1.02 / 6.13 / 13.68` — binds `182/245` |

**12-component total at the median bin** (`sqrt(total_with_phih_rel^2 + V^2)`, base
`total_uncertainty_z005_with_phih_2026-10-02`): h+ base `6.047%` -> V1 `6.054%`, V3 `6.081%`,
V2(1.0)=V4(1.0) `6.124%`, V2(1.5)=V4(1.5) `6.204%`; h- base `6.356%` -> V1 `6.357%`,
V3 `6.364%`, V2(1.0)=V4(1.0) `6.377%`, V2(1.5)=V4(1.5) `6.403%`. At the worst-total bins the
choice is immaterial: h+ bin 179 `162.887 -> 162.892` (V3/V1) or `162.908` (V4(1.0)), `+0.02 pp`
at most; h- bin 424 `89.168 -> 89.208` at most (`+0.04 pp`). The C_rad term never sets the total
anywhere; it ranks ~8th of 12 components at the median.

V2 vs V4: identical totals at the median, but V4 keeps the smoothed tail — V2's raw single-bin
excursions reach `7.22%`/`9.16%` where V4 tops out at the floor `6.26%`/`9.12%` (h+/h-), because
smoothing already suppresses MC-stat spikes (raw bin 244/484 tail `14.4%/18.3%` -> `5.87%/5.83%`).

## 4. Noise vs real

Rule `REAL if |rel| > 2*u_diff`: **`93/245` (h+, 38.0%) and `92/245` (h-, 37.6%) bins are REAL**;
`152/153` are NOISE_CONSISTENT. Key identity: **the k=1.0 floor binds exactly in the
noise-consistent bins** — `u_diff > V1 = 0.5|rel|` is algebraically `|rel| < 2*u_diff`, and the
binding counts match the noise-consistent counts bin-for-bin (`152 = 152`, `153 = 153`). A k=1.0
floor on the raw half-difference is therefore not an extra knob: it is the noise-consistency
classification expressed as an uncertainty. On the smoothed envelope (V4) the binding count drops
to `139/150` — smoothing lifts `13/3` bins above the resolution scale.

## 5. The 18 dangerous bins (V1 < 0.5% AND u_diff > 2%)

Rule `V1 < 0.5% AND u_diff > 2%`: **18 bins total, 10 h+ / 8 h-**, all in high-z rows (17 of 18 at
z 0.7–1.0) — exactly where the RAD/NONRAD proxy samples are thin. Unfloored variants (V1, V3)
quote sub-percent "model" uncertainties in bins the comparison cannot resolve. Full list
(labels from the shipped per-bin fields of the companion JSON; cell map 0–6 = `S0_x0, S0_x1,
S1_x0, S1_x1, S2_x0, S2_x1, S3_x0`):

| charge | bin | cell | z | pT (GeV) | V1 | u_diff |
|---|---|---|---|---|---|---|
| h+ | 209 | `S2_x1` | 0.7–1.0 | 4–8 | `0.44%` | `6.26%` |
| h+ | 170 | `S2_x0` | 0.7–1.0 | 0–0.5 | `0.20%` | `4.27%` |
| h+ | 138 | `S1_x1` | 0.7–1.0 | 2–4 | `0.22%` | `3.32%` |
| h+ | 65 | `S0_x1` | 0.7–1.0 | 0–0.5 | `0.15%` | `3.26%` |
| h+ | 135 | `S1_x1` | 0.7–1.0 | 0–0.5 | `0.28%` | `3.07%` |
| h+ | 30 | `S0_x0` | 0.7–1.0 | 0–0.5 | `0.23%` | `2.84%` |
| h+ | 100 | `S1_x0` | 0.7–1.0 | 0–0.5 | `0.23%` | `2.82%` |
| h+ | 103 | `S1_x0` | 0.7–1.0 | 2–4 | `0.21%` | `2.49%` |
| h+ | 67 | `S0_x1` | 0.7–1.0 | 1–2 | `0.23%` | `2.38%` |
| h+ | 136 | `S1_x1` | 0.7–1.0 | 0.5–1 | `0.15%` | `2.33%` |
| h- | 450 | `S2_x1` | 0.7–1.0 | 0–0.5 | `0.13%` | `5.14%` |
| h- | 418 | `S2_x0` | 0.7–1.0 | 2–4 | `0.17%` | `4.76%` |
| h- | 416 | `S2_x0` | 0.7–1.0 | 0.5–1 | `0.24%` | `4.09%` |
| h- | 486 | `S3_x0` | 0.7–1.0 | 0.5–1 | `0.34%` | `4.07%` |
| h- | 451 | `S2_x1` | 0.7–1.0 | 0.5–1 | `0.14%` | `3.93%` |
| h- | 275 | `S0_x0` | 0.7–1.0 | 0–0.5 | `0.12%` | `3.18%` |
| h- | 414 | `S2_x0` | 0.5–0.7 | 4–8 | `0.08%` | `2.82%` |
| h- | 278 | `S0_x0` | 0.7–1.0 | 2–4 | `0.00%` | `2.48%` |

Worst case h+ bin 209 (`S2_x1`, z 0.7–1.0, pT 4–8): V1 `0.44%` vs u_diff `6.26%` — a 14x
under-claim if unfloored; V4(1.0) quotes `6.26%`.

## 6. Double-count audit (against the "doubling the statistics into the model envelope" ruling)

**Why `sqrt(term^2 + (k*floor)^2)` IS the violation.** Quadrature asserts that the bin carries
BOTH the model envelope AND `k*u_diff` of additional, independent statistical uncertainty. The
C_rad tables' MC statistics thereby enter the model term *additively, in every bin*, and the
"model" component stops measuring generator disagreement — it becomes a statistics term wearing
a model label. It is also forward-looking double counting: once `c_rad_applied=true` and the
applied table's per-bin `relative_stat_unc` enters the statistical budget (mandated by the
spread note's caveat), the same MC statistics would be counted twice.

**Why `max(term, k*floor)` is NOT.** The max never adds the two inputs; it selects between two
readings of ONE measurement. Where the measured (smoothed) half-difference exceeds `k*u_diff`,
the statistics contribute exactly zero. Where it does not, the bin's model difference is
statistically unresolved, and the term quotes the comparison's *resolution* instead of a
sub-resolution number that under-claims. The assertion is "the model difference in this bin is
smaller than this comparison can resolve — quote the resolution bound," not "model effects and
MC statistics both act." At k=1.0 this is exactly the noise-consistency rule (Section 4), i.e.
the same 2-sigma classification the spread note itself computed, promoted from a label to a
coverage statement. The floor is a *lower bound applied to the model term*, never a *statistical
contribution folded into it*.

**Reconciliation with B1.** `ibu_model_rel_floored = max(base, b1_floor)` coexists with a
separate `ibu_stat_rel` without double counting because the two are disjoint functionals: the B1
floor is the response-bootstrap noise *of the difference estimator* (the data-free cross-closure
half-difference — nominal and shifted unfoldings share data events, so `ibu_stat_rel` does not
describe it), while `ibu_stat_rel` is the statistical uncertainty *of the unfolded result*. The
C_rad case is the same pattern: `u_diff` is the MC-stat noise of the generator-spread estimator,
and today **nothing else in the budget carries any C_rad statistics at all** (`c_rad_applied=false`;
none of the 11/12 components is a radiative term), so there is no second carrier to double.

**What changes when `c_rad_applied` flips to true.** Then the applied table's per-bin
`relative_stat_unc` (u of the *nominal* DJANGOH14 table — the correction actually applied) must
be carried as its own separate statistical component, and in that regime **the floor form becomes
wrong**: every floor-binding bin would carry `k*u_diff = k*sqrt(u_dj^2+u_rg^2)` in the model block
plus `u_dj` in the statistical block — the `u_dj` part counted twice, and the `u_rg` leg (which
never enters the applied correction) masquerading as model uncertainty. Correct bookkeeping:

- **Regime A (today, `c_rad_applied=false`):** `c_rad_gen_rel = max(V3, 1.0*u_diff)`,
  PROVISIONAL DIAGNOSTIC, prospective only; `u_diff` enters solely as a lower bound; no other
  C_rad statistical carrier exists, so nothing is doubled.
- **Regime B (after application):** the floor is retired — the model term reverts to the
  un-floored smoothed envelope V3, and the statistical block gains a separate
  `c_rad_stat_rel = relative_stat_unc(applied table)` component. The max-form floor is a
  pre-application stopgap that exists precisely because regime A has no other carrier for the
  comparison's statistical resolution.

## 7. Prior/nominal dependence (does the C_rad floor inherit the B1 defect?)

**No.** The pre-promotion critique (`critic-prerepromotion-regen-cold_2026-10-07.md`) flags the
B1 floor as prior/nominal dependent because it is built from response-bootstrap replicas of
*unfoldings* — the response matrix and prior enter directly, so the plain->coherent prior change
forces B1 regeneration (a promotion gate). The C_rad floor has no such pathway, verified from the
artifacts:

1. `rel` is a ratio of **generator-level** C_rad values: true-level RAD/NONRAD proxy yields from
   the two C_rad tables (xFGen<0, EMPZ>45), no detector, no response, no unfolding, no prior, no
   IBU iterate. The spread note confirms the disjointness in both directions: "C_rad is not
   applied anywhere in the cross-closure", and conversely nothing from the unfolding chain enters
   the C_rad tables.
2. `relative_stat_unc` is shipped per bin in the C_rad CSVs from the MC samples' own statistics
   (`u_rel(C) = sqrt(u_rel_nonrad^2 + u_rel_rad^2)`) and is designated `errors/per_bin` in the
   co-located validation JSONs (companion JSON, `field_provenance`) — again no unfolding-prior
   pathway.

The only "nominal" dependence is the convention choice of DJANGOH14 as denominator (and the
xFcut/EMPZ definition) — a fixed, validated definition, not the regenerating prior basis that
makes B1 nominal-dependent. **The C_rad floor does not inherit the B1 prior/nominal dependence.**

## 8. Recommendation

**Adopt V4(1.0):**

```
c_rad_gen_rel(bin) = max( 0.5 * GaussSmooth_pTrow(|rel|)(bin),  u_diff(bin) )
  rel    = (C_rad_rapgap31 - C_rad_djangoh14)/C_rad_djangoh14   (matched xFGen<0, EMPZ>45)
  u_diff = sqrt(u_rel_dj^2 + u_rel_rg^2)                         (shipped relative_stat_unc columns)
  kernel exp(-0.5*Delta^2), sigma = 1.0 pT-bin, edge-truncated renormalized
  within each (charge, (Q2,x) cell, z-row) five-point pT row
```

Rationale:

- **Does not under-claim in statistics-limited bins.** V1/V3 quote `0.44%` (bin 209) against a
  `6.26%` resolution; V4(1.0) quotes the resolution in all 18 dangerous bins and all 139/150
  noise-consistent bins.
- **House consistency.** Smoothing matches `phi_h_gauss_rel`/`ibu_model_rel` conventions (V2's
  raw tail imports single-bin MC-stat spikes: `7.22%/9.16%` vs V4's `6.26%/9.12%`); the max-form
  floor matches the B1 precedent (`max(base, b1_floor)`); k=1.0 is the actual statistical
  resolution of the two-leg difference (and the noise-consistency boundary, Section 4).
- **k=1.5 rejected:** no precedent (B1 floors at the replica RMS itself, k=1), and it inflates
  the h- max to `13.68%` — *above* the raw measured maximum (`9.16%`) — which is no longer a
  resolution statement but over-claiming, for a budget effect that is invisible anyway.
- **Budget cost at the median:** h+ `6.047 -> 6.124%` (`+0.077 pp`), h- `6.356 -> 6.377%`
  (`+0.021 pp`); vs the adopted V3 the extra cost is `+0.043 pp` (h+) / `+0.012 pp` (h-). The
  worst-total bins move `<= 0.04 pp` — dominated by other components; the choice is a matter of
  principle, not budget impact.

**Floor-binding flag: YES — report a per-bin boolean** (e.g. `crad_floor_bind`,
true where `u_diff > V3`). It documents which bins are resolution-quoted vs model-quoted, makes
the noise-vs-real crosscheck auditable (binding at k=1.0 must equal the NOISE_CONSISTENT set),
and is the operative mechanism for the regime-B switch: at application the floor is retired
exactly in the flagged bins and the statistics move to their own term (Section 6).

**On the ~62% binding fraction:** acceptable. Binding at k=1.0 occurs *precisely* in the bins
where the generator difference is MC-statistically unresolved — there the honest per-bin
statement IS the resolution, and the alternative (V3) quotes numbers the comparison cannot
support. V4 is itself the hybrid the situation calls for: the house-smoothed model envelope
where the difference is resolved, the B1-style resolution bound where it is not. The term's
partly-resolution character is furthermore temporary: it is inert pre-application (Section 9)
and the floor retires at application, when the model term reverts to the pure V3 envelope.

## 9. Caveat — the term is inert while `c_rad_applied = false`

`C_rad` is not applied centrally (raw v6 release, not Born-level) and both xFcut tables carry
PROVISIONAL DIAGNOSTIC status. `c_rad_gen_rel` therefore sizes the model uncertainty of a
correction that is **not yet in the central chain**: all totals in Section 3 are prospective
12-component sizings, not released values. The term becomes meaningful only once C_rad
application is enabled — at which point the floor form must be retired in favor of the
regime-B bookkeeping of Section 6 (un-floored V3 model term + separate `c_rad_stat_rel`
statistical component from the applied table's `relative_stat_unc`).

## 10. Addendum (2026-10-07) — the term must be sized at the BLIND pair

Sections 1–9 sized the floor on the **xFGen<0** pair, but the central and alpha-veto plots apply
the **BLIND (no xFGen cut) DJANGOH14 FINAL** table
(`data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/crad_v3_empz_gt45_nominal_490rows.csv`,
integrated h+ `1.1375979129031535` / h- `1.1366708715220577`). Nominal and uncertainty must
share one definition, so the study was rerun blind-vs-blind against the quarantined blind
RAPGAP31 FINAL table
(`data/radiative/quarantine/rapgap_crad_final_2026-09-30/crad_v3_empz_gt45_nominal_490rows.csv`,
integrated h+ `1.1328903059596749` / h- `1.1319063751575906`), whose content reproduces the
documented means of `CRAD_FINAL_PROVENANCE_2026-09-30.md` exactly. Results are in the
`blind_definition` block of the companion JSON (`status PASS`); the xFcut block is retained
unchanged and reproduces exactly (max deviation `0.0`).

Blind numbers (median/p95/max %): `u_diff` h+ `0.60/3.26/6.26`, h- `0.64/4.09/9.12`;
V4(1.0) h+ `0.76/3.74/6.26` (floor binds `132/245`), h- `0.80/4.15/9.12` (binds `146/245`);
REAL fractions `105/245` (42.9%, h+) and `95/245` (38.8%, h-); dangerous bins again **18**
(10 h+ / 8 h-), all covered by V4(1.0). Every structural conclusion survives: the k=1.0 floor
again binds exactly the NOISE_CONSISTENT set (`140 = 140`, `150 = 150`), and k=1.5 is again
rejected (h- max `13.68%` vs raw `9.26%`). Median shifts vs the xFcut sizing are negligible:
V4(1.0) h+ `+0.018 pp`, h- `+0.003 pp`; 12-component totals h+ `6.047 -> 6.124%` (`+0.077 pp`),
h- `6.356 -> 6.377%` (`+0.021 pp`) — the same costing as at xFcut.

**Definition guidance:** the recommended **V4(1.0)** term is unchanged in form but must be
computed from the **BLIND pair** — `rel` and `u_diff` both from the blind DJANGOH14/RAPGAP31
FINAL tables — so that the uncertainty matches the definition of the nominal C_rad the plots
apply. Never mix an xFcut leg into a blind-sized term; the block-to-pair mapping is recorded in
`definition_consistency_note` of the companion JSON.
