# Published background-subtraction scheme and validation gate — specification (2026-10-10)

**Status:** SPECIFICATION + GATE REQUIREMENTS. No products were built, no releases
were promoted, and every release, budget, and table referenced below was treated
read-only. This document defines the background-subtracted Born chain that the
2026-10-10 decision promotes to the published correction **through its own
validation gate** (superseding the critic's minimal-cost option), and specifies
that gate.

**Companion machine-readable spec:** `docs/systematics/bgsub_scheme_spec_2026-10-10.json`
**Input verification (all numbers below re-derived read-only):**
`sidis_highQ2/data/systematics/bgsub_scheme_input_verification_2026-10-10.json`,
produced by `sidis_highQ2/scripts/systematics/verify_bgsub_scheme_inputs_2026-10-10.py`
(schema `bgsub_scheme_input_verification_2026-10-10_v1`, all cross-checks PASS).

---

## 1. What the existing bgfracsub machinery does today

The registered plotter
`H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py`
applies a **post-unfold, plot-level multiplicative background correction** to the
central-chain unfolded release: on each of the 490 release bins it computes
`sigma = cross_section_nb * (1 - f_b) * C_rad`, where `f_b = B/D` is formed by
980→490 pT-pair summation of generator-averaged templates
`B = 0.5*(RG+DJ)` (variance `0.25*(varRG+varDJ)`) from the alpha0p2
cone-vetoed template bank, and `D` is the alpha0p2-selected data vector
(3,865,073). Set A subtracts `V0 + det.int. + lowQ2` (integrated
`f_b = 0.04131344`); Set B adds electrons (`0.07985771`). The template's own
statistical width enters as `sigma_B_nb = sqrt(varB)*(sigma_raw/D)*C_rad` added
in quadrature to the total. All products are **PROVISIONAL diagnostics** under
xFcut C_rad keys (2026-10-07/08); the plotter has **no releasebin-key products**
(its CRAD_CANDIDATES default was repointed to the DJ14 releasebin table but never
re-run). It is a correct dilution-approximation calculator, not a
response-consistent subtraction.

## 2. Fiducial adjudication — "charged hadrons from the DIS event"

### 2.1 Central-chain contamination accounting (verified 2026-10-10)

| Source category | Central-chain handling today | Share of data vector | Adjudication |
|---|---|---|---|
| Non-V0 electrons | 100% fake column | h+ 3.749% / h− 3.963% (alpha0p2 D) | **Already removed** by the response fake fraction — must NOT be subtracted again |
| "other / unmatched" | fake column | (included in fake column) | **Already removed** — same |
| det.int. (detector interaction) | partition: 86.18% matched physical block (85.0% response-weighted), 13.82% fake column (`detint_response_partition_20260626`) | h+ 0.749% / h− 0.667% | Fake-column 13.82% **already removed**; matched 86.18% is **response-modeled signal proxy** — do NOT blind-subtract under the unchanged central response; its data–MC mismatch is an uncertainty, not a subtraction |
| V0 daughters | **dropped from the response entirely** (no fake column, no matched block) | h+ 3.413% / h− 3.421% | **UNSUBTRACTED GAP** — the fiducial residual; data contains them (untaggable) but the response reco space excludes them |
| low-Q2 source | source-distinct; absent from the central response | h+ 0.0061% / h− 0.0047% | **UNSUBTRACTED GAP** — source-distinct, subtract explicitly |

The response fake fraction (applied at the data-vector level,
`d_sig = d*(1-f_i)`) removes **10.7141% (h+) / 10.4019% (h−)** of the central
data vector (audit sums h+ 1,975,301→1,763,665.13; h− 1,908,796→1,710,244.72;
per-charge fake-fraction medians ~0.100 / 0.0998). The charge asymmetry of that
removal is **0.3122 pp** — the "~0.3 pp" charge-symmetry statement is verified.

**Fiducial-residual set (the exact complement of what the response handles):**

```
R = V0(gen-avg) + lowQ2
```

- Integrated on the central denominator: **h+ 3.4026% / h− 3.4085%**
  (B_R h+ 67,211.631 = V0 67,092.201 + lowQ2 119.431;
   B_R h− 65,062.025 = V0 64,973.076 + lowQ2 88.949; total 132,273.656).
- Integrated on the alpha0p2 denominator: h+ 3.4192% / h− 3.4254%
  (all-charge 3.4223%).
- Charge asymmetry of set R: **−0.0059 pp** (central D) / −0.0062 pp (alpha D) —
  the subtracted set is charge-symmetric at the integrated level to <0.01 pp.

### 2.2 Verdicts on the candidate subtraction sets

- **Set B (electron+V0+det.int.+lowQ2, integrated 7.9858%): REJECTED — fiducially
  wrong.** Electrons are 100% fake-column and are already removed by the response
  fake fraction; subtracting them again double-counts (adds a spurious
  3.75%/3.96% removal).
- **Set A (V0+det.int.+lowQ2, integrated 4.1313%): REJECTED — over-subtracts.**
  It subtracts 100% of det.int., but 13.82% of det.int. is fake-column and
  already removed; the over-subtraction is 13.82% of the det.int. share
  (~0.10% of D, h+) — small but fiducially incoherent, and it blind-subtracts
  the 86% response-modeled matched body, violating the house rule against
  subtracting response-modeled signal proxies under an unchanged response.
- **Set R (V0+lowQ2): ADOPTED.** It is exactly the fiducial residual — every
  category the central response fake column already handles is excluded; every
  category the response cannot handle is included; nothing is subtracted twice.
- **Allfake chain (alpha0p2 selection + cone veto, exactly-once fake-column,
  G0–G10 PASS, Run B 20.735708 nb pre-C_rad): documented FALLBACK ONLY.** It is
  the fiducially purest alternative (selection-coherent by construction), but
  promoting it would swap the response, re-derive the whole budget, change the
  event selection, and re-attach C_rad (its blind sidecars are STALE /
  quarantined). Blast radius forbids it as the published path. It remains the
  cross-check universe for exactly-once bookkeeping.

### 2.3 Remaining gaps under set R (carried as bounded uncertainties)

1. **Template selection mismatch (the alpha0p2 residual).** The B_R templates are
   alpha0p2-selected (electron-cone veto) while the published chain is central
   (no cone). Cone difference: **19,024 tracks (0.49% of D), electron-dominated**.
   Because electrons are not in set R, the set-R-relevant cone content is only
   the V0/lowQ2 part inside the cone. Measured bound: swapping the denominator
   alpha0p2↔central moves the integrated published result by **−0.011 pp**
   (h+) / −0.011 pp (h−). This is the "alpha0p2 width" anchor — bounded, carried
   in `bgsub_stage_rel` (§5), not eliminated.
2. **V0-daughter MC modeling.** Single generator pair (RG31 vs DJ14) with a
   relative half-spread of **7.27% / 7.79%** of the V0 template at bin median
   (integrated V0 half-spread 0.131%/0.135% of D) → `bgsub_model_rel`.
3. **low-Q2 template.** DJANGOH DjBkgd only, tiny (0.005% of D), ±30% scale
   precedent (arXiv:1302.1321) → folded into `bgsub_model_rel` (negligible,
   ~0.002%).
4. **Fake-fraction common model.** The response fake fraction `f_i` is
   MC-derived; the G9/S1b template-vs-fakecolumn audit found integrated ratios
   **0.9817–0.9872** (NucliaWeight caveat) → `bgsub_fakefrac_rel`.
5. **Empty-bin convention.** 3 release bins have D=0 (h+ 109, 179; h− 240) with
   tiny template content (≤0.12 tracks). Convention (already used by the
   plotter): **no subtraction, f=0** — must be preserved bit-for-bit.

## 3. The published scheme (specified; NOT yet built)

### 3.1 Stage — PRE-UNFOLD data-vector subtraction, then the unchanged chain

**Rationale (response consistency, row-17 requirement).** The central response's
reco space excludes V0-daughter tracks and low-Q2 source tracks; the data vector
contains them (untaggable). Unfolding contaminated data with that response
smears the residual into signal bins, and a post-unfold `(1-f)` factor is only a
dilution approximation — "a generic post-hoc fraction," which the systematics
definition spec (row 17 `background_rel`, BLOCKED) explicitly forbids as the
published treatment. Subtracting set R from the data vector **before** unfolding
restores agreement between the data vector and the response's modeled reco
content. This is the decisive argument for the pre-unfold stage.

**Chain (the published background-subtracted Born chain):**

1. **Subtract (980-bin data-vector level):**
   `data_sub[i] = data_central[i] − B_R[i]`,
   `B_R[i] = V0_genavg[i] + lowQ2[i]`,
   `V0_genavg = 0.5*(RG31_V0 + DJ14_V0)`, variance
   `varB_R = 0.25*(varRG_V0 + varDJ_V0) + var_lowQ2`.
   Guard: if `data_central[i] == 0` → no subtraction (empty-bin convention).
   Require 0 negative bins.
2. **Variance (E1b convention, exactly-once Poisson):**
   `var_sub[i] = data_sub[i] + varB_R[i]` (Poisson of the subtracted count ⊕
   template variance — the allfake chain plan rev-3 corrected form). The
   released/toy statistical input is Poisson-only (`w2 = data_sub`); the
   template term is carried by the budget component `bgsub_stat_rel` instead
   (no double-count; see §5.1).
3. **Fake fraction (UNCHANGED):** `d_sig[i] = data_sub[i] * (1 − f_i[i])` with
   the **byte-identical central per-bin fake-fraction table** (integrated
   removal 10.7141% / 10.4019%). The fake fraction is NOT recomputed. This
   preserves exactly-once handling: electrons + other/unmatched + det.int.
   fake-column (13.82% of det.int.) via the response; V0 + lowQ2 via the
   explicit subtraction; det.int. matched body (86.18%) in neither — it stays
   response-modeled signal.
4. **Re-unfold:** unchanged central configuration — RAPGAP31 central response
   (491,246 shapes), coherent `body+miss` prior, IBU k=4, `L = 351,000 nb⁻¹`,
   identical 980→490 release mapping. The ONLY input that differs from the
   nominal release is the data vector.
5. **C_rad:** the applied releasebin DJANGOH14 table
   (`sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`),
   applied exactly as in the Born release (bitwise `C_rad × raw` identity,
   max dev 0.0).
6. **Product:** a new dated release directory (`born_crad_bgsub_setR_centralbase_<tag>_<date>`)
   with exact-text serialization, a README carrying the invariants, and a
   v4-style `validation.json` extended with the bgsub checks (§6). Existing
   releases are not touched.

### 3.2 Estimator (per-bin, formal)

```
sigma_bgsub[b] = (1/L) * IBU_k4( data_central − B_R , response RG31,
                                prior body+miss, fake fraction f_i )
                       [b] * C_rad_releasebin[b]
```

**Integrated expectation (dilution-approximation anchors, verified):**

| Quantity | h+ | h− |
|---|---|---|
| Unsubtracted Born (release, nb) | 13.107258 | 12.772782 |
| Data-weighted f_res (central D) | 3.6633% | 3.6582% |
| Dilution anchor (central D), nb | **12.627105** | **12.305533** |
| bgfracsub-heritage anchor (alpha D), nb | 12.625770 | 12.304163 |

The pre-unfold published result must land within the gate window of the
central-D anchor (§6 RU-3); the difference between pre-unfold subtraction and
the post-unfold dilution factor is the unfolding non-linearity to a ~3.4–3.7%
input change and is itself measured and budgeted (CP-1 → `bgsub_stage_rel`).

### 3.3 Per-bin treatment

- `f_res` per-bin statistics (set R, central D): **h+ median 1.549%, mean 1.697%,
  p99 4.707%, max 14.022%** (single bin, global 34 — a high-z, low-pT corner);
  **h− median 1.609%, mean 1.769%, p99 4.462%, max 5.225%** (global 245).
  Bins ≥10%: exactly 1 (h+); ≥20%: 0; ≥50%: 0.
- z-row structure (median): 2.80% (z0) → 1.02% (z5) → 1.14% (z6, inflated by the
  single 14% outlier bin). No bin requires exclusion; the 14% bin stays in with
  its full variance and is monitored by name in the gate.
- Empty bins (h+ 109/179, h− 240): no subtraction, f=0, remain exactly zero.
- Templates are kept charge-separated (no charge-averaging of B_R).

### 3.4 Charge symmetry

The subtracted set is charge-symmetric at the integrated level: set-R fraction
asymmetry **−0.0059 pp** (central D). The response fake-fraction asymmetry
(0.3122 pp) is unchanged and present in both the unsubtracted and subtracted
chains. Expected shift of the published h+/h− integrated asymmetry from the
subtraction alone: ≤0.01 pp (gate CP-3).

### 3.5 Roles retained for the non-published variants

- **Post-unfold `(1−f_res)` factor** (bgfracsub-style): retained as (a) the
  cross-check the published chain is compared against (CP-1) and (b) the
  measured input to `bgsub_stage_rel`. Never the published number.
- **Allfake chain:** diagnostic fallback and exactly-once bookkeeping
  cross-check; its STALE blind C_rad sidecars must never be re-attached
  (PM-3).

## 4. New uncertainty components vs widening — DECISION: new components

The canonical 13-component budget (emfix 2026-10-10; medians h+ 5.7093% /
h− 6.1523%) contains **no background term** (row 17 `background_rel` BLOCKED).
Decision: the background treatment enters as **four NEW per-bin relative
components** (register 13 → 17). The existing 13 are NOT widened, NOT recomputed,
and must remain byte-identical to the emfix package. Rationale: the 13 are frozen
and audited (quadrature closure, 0/14,700 mismatches across rebuilds); the gap
was the documented absence; row 17 reserves exactly this slot; and the IBU toys
stay Poisson-only on `data_sub` so nothing widens implicitly.

| # | Component | Definition (per release bin, relative) | Verified width anchors (h+ / h−) |
|---|---|---|---|
| 14 | `bgsub_stat_rel` | `sqrt(varB_R) / data_sub` — template MC statistics of the subtracted set (V0 gen-avg variance + lowQ2 variance). Statistical class: **no smoothing**, raw per bin | median ≈ **0.056% / 0.062%**; max 6.23% / 2.37% (sparse-bin tail; driven by near-empty bins) |
| 15 | `bgsub_model_rel` | generator half-spread `0.5*|RG−DJ|(V0)/data_sub` ⊕ low-Q2 scale `0.30*lowQ2/data_sub` (±30% precedent, arXiv:1302.1321) | median ≈ **0.113% / 0.122%** (V0 half-spread/D; integrated 0.131% / 0.135%); max 1.67% / 3.35% (sparse); lowQ2 term ~0.002% (negligible). If sparse-bin spikes fail smoothness QA, apply the house pT-row Gaussian smoothing (σ=1 bin, edge-truncated renormalised, floor 0.85·unsmoothed) as for `c_rad_gen_rel` |
| 16 | `bgsub_stage_rel` | stage + selection coherence: per-bin `|pre-unfold − post-unfold dilution| / published` (measured at gate, CP-1) ⊕ the alpha0p2 cone-selection residual bound `|f_res_alphaD − f_res_centralD|` | expected O(0.1%) median; anchors: denominator effect **−0.011 pp** integrated; cone Δ 19,024 tracks (0.49% of D, electron-dominated); set-R-relevant cone content bounded to the V0/lowQ2 part |
| 17 | `bgsub_fakefrac_rel` | response fake-fraction common-model dependence: per-bin `|1 − r_min| * f_i`, `r` from the G9/S1b template-vs-fakecolumn category ratios (**0.9817–0.9872**, NucliaWeight caveat); covers the model dependence of removing electrons/other/det.int.-fake (incl. the 86/14 det.int. partition model) | with `f_i` medians 0.100 / 0.0998 → expected median ≈ **0.18%**; re-derive `r` with the NucliaWeight caveat resolved at gate time, or carry the caveat |

Approximate quadrature addition of the four at median: ~0.24% — negligible
against the 13-component totals (5.71% / 6.15%) but it formally un-BLOCKS row 17.
Rows 18 (`diffvm_rel`) and 19 (`gammap_bkg_30pct_rel`) remain BLOCKED and are
unchanged by this spec.

**Quoting policy:** once the gate passes, the published total is the 17-component
quadrature (`total_with_crad17_rel` or successor naming fixed by the systematics
owner); the 13-component total must no longer be quoted as the published total
(consistent with the budget doc's "consequence for quoting").

## 5. Where the numbers come from (verification provenance)

Every number in §2–§4 was re-derived read-only from primary artifacts by
`verify_bgsub_scheme_inputs_2026-10-10.py` into
`sidis_highQ2/data/systematics/bgsub_scheme_input_verification_2026-10-10.json`:
template totals re-validated (electron 148,976.4352 / V0 132,065.2768 /
det.int. 27,405.8031 / lowQ2 208.3795 at L351,000); denominators verified against
the central audit (3,884,097 = h+ 1,975,301 + h− 1,908,796; alpha0p2 3,865,073;
cone Δ 19,024); the bgfracsub Set A product table reproduced exactly (all
component columns + `f_b` to 1e-12 including the 3 empty-bin zeros; integrated
0.04131343946000542); fake-fraction asymmetry recomputed from audit sums
(0.3122 pp); dilution anchors computed from the installed Born release and the
applied releasebin C_rad table (Born totals reproduced to <1e-4 nb).

## 6. Validation gate — 24 checklist items

The gate is modeled on the allfake G0–G10 ledger and the Born release
validation.json v4 invariants. ALL items must PASS before any promotion; the
published chain must NOT merge into the nominal release until then (state rule).
Arithmetic invariants are exact; tolerances are stated per item.

**A. Input verification (IV, 5 items)**

1. **IV-1 Template totals.** The four template banks reproduce the validated
   L351,000 totals within 1e-3: electron 148,976.4352; V0 132,065.2768;
   det.int. 27,405.8031; lowQ2 208.3795 (h+ 119.4307 / h− 88.9488).
2. **IV-2 Central data vector.** Sums equal the audit: h+ 1,975,301;
   h− 1,908,796; total 3,884,097; `bg_subtracted` flag false; 980 bins.
3. **IV-3 Alpha0p2 vector and cone residual.** Sums 3,865,073
   (h+ 1,965,700 / h− 1,899,373); cone difference vs central exactly 19,024.
4. **IV-4 Binning identity.** The 980→490 pT-pairing map is identical to the
   release construction (490 rows per charge set, one-to-one, no orphans).
5. **IV-5 Machinery cross-check.** The existing Set A product table is
   reproduced: all component columns match; `f_b` matches to 1e-12 including
   the 3 empty-bin zeros; integrated `f_b = 0.04131343946000542`.

**B. Subtraction arithmetic (SA, 5 items)**

6. **SA-1 No negative bins.** `data_sub = data_central − B_R` has 0 negative
   bins at 980 and at 490; the 3 empty bins (h+ 109/179, h− 240) subtract
   exactly 0.
7. **SA-2 Integrated sums.** B_R sums: h+ 67,211.631 / h− 65,062.025 /
   total 132,273.656; `data_sub` sums: h+ 1,908,089.369 / h− 1,843,733.975 /
   total 3,751,823.344 (tolerance 1e-6 relative).
8. **SA-3 Variance convention (E1b, exactly-once Poisson).**
   `var_sub = data_sub + varB_R` with
   `varB_R = 0.25*(varRG_V0 + varDJ_V0) + var_lowQ2`; the toy/released
   statistical input is Poisson-only (`w2 = data_sub`) — adding the Poisson
   term twice is the retired rev-2 defect (data variance ×2.00022) and is a
   STOP condition.
9. **SA-4 f_res envelope.** Per-bin `f_res` (central D) within the verified
   envelope: h+ max 14.022% (global bin 34, the single ≥10% bin), h− max
   5.225%; medians 1.549% / 1.609%; exactly 0 bins ≥20%; exactly 3 zero-D bins
   with f=0. Any bin outside the envelope: STOP and investigate before gate.
10. **SA-5 Charge symmetry of the subtraction.** Integrated `f_res` asymmetry
    within ±0.02 pp of −0.0059 pp; per-charge B_R kept separate; per-bin
    asymmetry distribution recorded in the release validation block.

**C. Chain identity and re-unfold (RU, 5 items)**

11. **RU-1 Configuration identity.** Response (RAPGAP31 central, 491,246
    shapes), coherent `body+miss` prior, IBU k=4, `L = 351,000` all identical
    to the nominal release; the config diff vs nominal is the input vector only.
12. **RU-2 Fake fraction exactly once.** The per-bin fake-fraction table is
    byte-identical (sha256) to the nominal release sidecar; applied as
    `d_sig = data_sub*(1−f_i)`; integrated removal on `data_sub` within
    [10.60%, 10.83%] (h+) / [10.29%, 10.51%] (h−) (the unchanged f_i on a
    3.4%-smaller vector); recomputing f_i is a STOP condition.
13. **RU-3 Magnitude window.** Published integrated cross sections within
    **±0.5%** of the central-D dilution anchors h+ 12.627105 nb /
    h− 12.305533 nb (equivalently within ±0.7% of the alpha-D heritage anchors
    12.625770 / 12.304163). Outside: STOP (mis-calibration lesson from the
    retired allfake rev-2 window).
14. **RU-4 C_rad bitwise.** The applied C_rad table is the releasebin DJ14 file
    (sha256 ledger); `C_rad × raw` reproduces bitwise (max dev 0.0, as in the
    Born release); no other C_rad variant enters the published product.
15. **RU-5 Physicality and serialization.** 0 negative cross sections; 490 bins
    per charge; empty bins exactly zero; exact-text serialization; validation
    block written (v4-style, extended).

**D. Consistency and physics tests (CP, 5 items)**

16. **CP-1 Pre-vs-post stage comparison.** Per-bin
    `|pre-unfold − Born*(1−f_res)| / pre-unfold ≤ 1%` (≤2% only in declared
    sparse bins with written justification); integrated ≤ 0.5%. The signed
    difference vector is saved and is the measured input of `bgsub_stage_rel`.
17. **CP-2 Add-back closure (wiring).** Unfolding `data_sub + B_R` (bitwise
    identical to `data_central`) reproduces the nominal published release
    bitwise (≤1e-9 relative on all central totals; stronger than the allfake
    E1b-invariance precedent because the input is bitwise identical).
18. **CP-3 Charge-asymmetry stability.** The published h+/h− integrated
    asymmetry shifts by ≤0.02 pp relative to the unsubtracted Born asymmetry
    (expected ≈ −0.006 pp from the set-R asymmetry).
19. **CP-4 Template-vs-fakecolumn consistency.** On the overlap categories the
    G9/S1b results reproduce: shape correlations ≥0.999; integrated ratios
    within [0.9817, 0.9872] — or the ratios are re-derived with the
    NucliaWeight caveat resolved and the `bgsub_fakefrac_rel` width updated
    accordingly; per-category results recorded.
20. **CP-5 Budget closure.** The 13 existing components are byte-identical to
    the emfix package (0/14,700 mismatches); the 17-component total satisfies
    the quadrature identity per bin (max dev ≤1e-12); medians reported h+/h−;
    the quoting-policy switch (§4) is recorded in the release README.

**E. Promotion mechanics (PM, 4 items)**

21. **PM-1 P-ALPHA disposition.** P-ALPHA (the alpha0p2 selection item) is
    either closed or explicitly waived in the release README with the bounded
    cone-residual argument (Δ 19,024 tracks, electron-dominated; set-R-relevant
    cone content bounded; denominator effect −0.011 pp) and the resulting
    `bgsub_stage_rel` width. The PROVISIONAL/diagnostic status of all
    alpha0p2-template products is closed by P-ALPHA or carried as documented
    PROVISIONAL with the waiver.
22. **PM-2 Release hygiene.** New dated release directory only; README with
    invariants; extended validation.json; existing releases (incl.
    `born_crad_dj14relbin_..._2026-10-09`) untouched, read-only.
23. **PM-3 Sidecar hygiene.** No stale blind C_rad sidecars are re-attached
    (allfake lesson); C_rad enters only from the releasebin table; sha256
    ledger of every input (templates, data vectors, fake-fraction table,
    C_rad, response, prior).
24. **PM-4 State rule.** No merge into the nominal chain until items 1–23 all
    PASS; STATE.md / RUN_STATUS.md promotion entries written only after PASS;
    the 17-component total becomes the quoted published total at promotion.

**Gate checklist count: 24** (IV 5 + SA 5 + RU 5 + CP 5 + PM 4).

## 7. Standing preconditions and blockers

- **P-ALPHA remains open** and is the standing precondition the gate must close
  (PM-1); all alpha0p2-template products carry PROVISIONAL/diagnostic status
  until then.
- The critic's Findings 1–3 / C-2..C-7 items remain open separately and are NOT
  addressed by this spec.
- The fake-fraction provenance / MC-sample-identity dispatch (normalization
  owner) runs in parallel; this spec does not duplicate it. If that dispatch
  re-identifies the template MC sample, IV-1..IV-5 re-run automatically (they
  re-derive from primary artifacts).
- Absolute hadron cross-section quoting remains governed by the existing
  normalization policy; this spec changes only the background correction chain
  and the uncertainty register.

## 8. Source index (primary artifacts, read-only)

- bgfracsub plotter: `H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/release/scripts/make_finalcandidate_7panel_and_xQ2_release351_cRad_bgfracsub.py`
- Template bank: `H1ChargedSidis_HighQ2/output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/reco_backgrounds_L351000_finalcandidate_20260626/` (7 NPZ)
- det.int. partition: same tree, `diagnostics/detint_response_partition_20260626/` (86.18% matched / 13.82% fake, nominal weights)
- Central/alpha0p2 data vectors: `H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_..._percharge/` and `H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/`
- Central audit: `sidis_highQ2/data/audits/unfolding_bgsub_audit_2026-09-25/central_audit_numbers.json`
- Born release: `H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
- Raw IBU release: `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
- C_rad releasebin table: `sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv`
- bgfracsub Set A products: `sidis_highQ2/plots/release/bgfracsub_tracklevel_cRad_applied_2026-10-07/DJANGOH14_xFcut/setA_v0_detint_lowq2/`
- Allfake chain: `sidis_highQ2/docs/unfolding/bgsub_reunfold_crad_chain_plan_2026-10-05.md` (rev 3) and `H1ChargedSidis_HighQ2/output_v6/studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/`
- Budget register: `sidis_highQ2/docs/systematics/budget_13components_derivation_2026-10-10.md`; definition spec `sidis_highQ2/docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` (rows 17–19)
- Critic review: `sidis_highQ2/docs/critiques/publication-grade-baseline-review-2026-10-10.md`
