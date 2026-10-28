# Physics-correctness critique: "Finish the Analysis" plan (G18–G33)

**Verdict:** FATAL

**Role:** h1-analysis-critic (read-only, adversarial).
**Date:** 2026-05-21.
**Material reviewed:**
- `H1/sidis_highQ2/context-handoff.md`, "Detailed Plan: Finish the Analysis" appended section (lines 679–1962), gates G18–G33, frozen-decision block F1–F15, dependency graph, observable formulas, code anchors, and todo list.
- `H1/sidis_highQ2/docs/data/highQ2_v5f_schema_delta.md` (v5f 96/55/49 schema; Option H mask; charge-flip 2.38 %; hadron/electron/muon truth-PID composition 82.56 / 15.17 / 1.34 %).
- `H1/sidis_highQ2/docs/references/reference-ABC-scope-and-blinding-highQ2-sidis.md` (A_BC observable contract, blinding policy, cancellation expectations, required closure tests, reporting requirements).
- `H1/sidis_highQ2/docs/references/reference-highQ2-crad-availability.md` (C_rad strategy 2, four-way beam-sign × hadron-charge separation, per-observable cancellation table).
- `H1/sidis_highQ2/docs/references/reference-nuclia-correction-highQ2-sidis.md` (per-track gen-PDG dispatch, proton-omission systematic, sentinel-clip recommendation, in-range-but-unphysical -2.58 finding, A_h non-cancellation argument).
- `H1/sidis_highQ2/docs/data/lumi_effective_per_period_v5f.json` + `lumi_effective_per_period_v5f.md` (realised per-period luminosity, A_BC e+p/e-p ratio 1.04299).
- `H1/sidis_highQ2/docs/results/eps_dis_data_application_v5f_highQ2.md` (the prior CONCERN at the cuts vs MC mismatch and at high-Q²/high-x bin spread).
- `H1/sidis_highQ2/docs/references/reference-highQ2-grl-manifest.md` (S67 GRL totals; GRL e+p/e-p ratio 1.193).

Observable classes attacked: M^{h±}, d⁴σ^{h±}/dQ² dx dz dpT,h, A_h, A_BC{h+, h-, sum, DIS}, ⟨cos φ_h⟩ / ⟨cos 2φ_h⟩.

The plan is structurally thorough and gate-driven, but it has two demonstrated formula-level bugs that will produce physically wrong central values if implemented as written, several major cancellation/asymmetric-treatment risks, and multiple gate criteria too soft to keep a wrong answer out of the internal package.

---

## Fatal Issues

### F-1. The "absolute cross-section" formula at G26 and §8.1 omits the acceptance correction.

**Claim being criticized.** The plan freezes the cross-section formula (Section 1.1, repeated at §8.1, repeated at the code anchor in §13) as:

```
sigma4_h = N_h_corr / (L_data * delta_Q2 * delta_x * delta_z * delta_pth)
```

with the explanation that `N_h_corr` is "built from v5f reco yields corrected for acceptance, possible unfolding, and `C_rad`" (Section 1.1, G25/G26 method bullets).

**Evidence the implementation will not match the intent.**
- The Representative implementation snippets at G25 and G26 (lines 1247–1249 and 1289–1291) and the §13 code anchor (lines 1882–1886) all use the bare name `N_h_corr` (and `N_h_data_corr`) with no explicit pre-step that divides by ε_h(bin, charge).
- The Phase 4 / G26 to-do bullet (line 1941) reads "Reuse corrected hadron yields to compute absolute `d^4σ/dQ²dxdzdpT,h` per period". It does **not** state that the input must be acceptance-divided.
- §8.3 ("Shared correction chain with multiplicities") says the only structural difference from multiplicities is the **denominator**; this implies the cross-section reuses the numerator unchanged, but the multiplicity numerator already has the correction chain wrapped into `N_h_corr` through G25's "Correct yields with the nominal G23 method and G24 `C_rad`" — that is unfolding or BBB applied to the **reco-binned** yield, not an explicit ε_h division.

**Why it matters physically.** The H1 absolute differential cross-section is

```
d^4σ/dQ²dxdzdpT,h = ( N_h_data_raw - N_bkg ) / ( ε_h(bin, charge) * L_data * bin_volume )   [BBB regime]
                  = U[ N_h_data_raw ] / ( L_data * bin_volume )                              [unfolded regime]
```

where `U[·]` is the unfolding operator built from the acceptance×purity-encoded response matrix (i.e. unfolding folds acceptance into the operator). In BBB regions (G22 marks bins `bbb_allowed` when purity ≥ 0.5), the plan offers **no** explicit ε_h division step. If `N_h_corr` in G25 means "data divided by reco MC × gen MC" per the G23 snippet `bbb = (reco_data / reco_mc) * gen_mc`, then it is implicitly an acceptance×migration correction that converts to truth-binned yield — but only if reco_mc and gen_mc are normalized the same way; the snippet does not write the normalization. For unfolded regions, the response is built in G21 with no explicit statement that it is normalised to incorporate ε; G21 says "row or column normalization documented explicitly" without choosing.

**Specific path to a wrong number.** If G21 produces a **row-stochastic** response (P(reco | gen)), then `unfolded = response^{-1} reco_data` yields a truth-spectrum estimate that has **not** been divided by ε_h; the analyst must divide by ε_h separately before σ. The plan currently does not require that step anywhere. If G21 instead produces a response containing the acceptance (efficiency-folded), the BBB path described at G23 (`bbb = reco_data / reco_mc * gen_mc`) and the unfolded path will disagree by exactly the acceptance factor.

**Analysis-killing vs follow-up.** Analysis-killing for σ. A factor-of-1/ε bias is between roughly 1.05× (in central bins where ε~0.95) and ~2× (at high-z / high-pT,h rim bins, since acceptance can be that low at high-Q² SIDIS). Cross-sections will be wrong by a multiplicative factor that varies bin-to-bin and that the per-bin sanity check σ ≥ 0 will not catch.

**Minimum fix.** G25, G26, G27, G30, §1.1, §8.1, §8.3, and §13 must (a) state the bin-by-bin correction chain explicitly: `N_h_corr(bin) = U[ N_h_data_raw / ε_h(bin, charge) ] × C_rad(bin, charge[, beam])` (or equivalent if ε is folded into U), and (b) require G23 to declare unambiguously whether the response matrix is built efficiency-folded or efficiency-stripped, with the BBB formula adjusted to match. The G26 numerical exit must include a closure test that the cross-section recovers the input MC cross-section when run on MC pseudo-data with all corrections in place.

---

### F-2. The multiplicity formula at G25 is missing the DIS-event acceptance correction.

**Claim being criticized.** The plan freezes the multiplicity formula (Section 1.1; G25 method bullets; §13 anchor) as:

```
M_h = N_h_corr / ( N_DIS_corr * delta_z * delta_pth )
```

and at G25 line 1224 instructs: "Divide by `N_DIS_corrected(Q²,x) * Δz * ΔpT,h`."

**Evidence.** `N_DIS_corr` is named but its construction is never written in the plan. The companion data-side input artifact is `docs/data/n_dis_data_v5f_highQ2.json` (cited at G25 line 1238) plus `eps_dis_data_application_v5f_highQ2.json` (line 1239). The latter is the prior application memo showing that `N_DIS_corrected = N_DIS_data / ε_DIS_MC` is applied bin-by-bin — but that artifact:
- uses `Empz ∈ [35, 70]` (the MC eps_DIS prototype window), not the frozen P5 `Empz ∈ [45, 65]`;
- uses `Yes ∈ [0.1, 0.6]`, not the frozen P5 `0.1 < y < 0.9`;
- is flagged CONCERN by its own validation.

So `N_DIS_corr` as currently sitting on disk is **not** the frozen-cut quantity that should feed G25. The plan's "denominator sync" todo (line 1936) acknowledges this in the abstract but does not freeze the corrected-denominator formula explicitly.

**Why it matters physically.** M^{h±} per the standard SIDIS recipe is

```
M^{h±}(Q², x, z, pT,h) = [ N_h_data(bin)  / ε_h(bin, charge) / C_rad,h(bin) ]
                       / [ N_DIS_data(Q²,x) / ε_DIS(Q²,x) / C_rad,DIS(Q²,x) ]
                       / ( Δz * ΔpT,h ).
```

The plan's two-term denominator (`N_DIS_corr × Δz × ΔpT,h`) hides three independent corrections inside `N_DIS_corr` and conflates them with the hadron-numerator corrections inside `N_h_corr`. There is no in-plan equation stating that the hadron numerator's hadron-fiducial efficiency and the DIS denominator's electron+vertex efficiency are computed and applied separately, with C_rad_DIS in the denominator paired with C_rad_h in the numerator (the partial cancellation that determines whether residual C_rad on M is ~1 % or ~10 %).

**Specific path to a wrong number.** If a downstream agent reads the formula literally and treats `N_DIS_corr` as raw `N_DIS_data` × TrigWeightAC (it is named "N_DIS" everywhere with the word "corrected" attached only via the artifact title, not the plan's symbolic definition), the multiplicity is wrong by ~1/ε_DIS ≈ 1.05 globally and by larger factors in corner Q²-x cells (the prior application memo shows ε_DIS down to 0.8941 in the (700–2000, 0.1–0.5) bin). Likewise, if N_h_corr is treated as ε_h-divided but the C_rad split is left unwritten, the C_rad cancellation that the plan **assumes** at §1.1 ("does NOT cancel" only as a vague statement) will not be enforced consistently in code.

**Analysis-killing vs follow-up.** Analysis-killing for M^{h±}. The multiplicity is the headline observable and its absolute normalization is the absolute-cross-section blocker in disguise (HB5 from the upstream handoff).

**Minimum fix.** Replace the §1.1 multiplicity formula with the full four-correction form, name C_rad_h and C_rad_DIS distinctly in the formula, and require G25's exit criterion to include a `with-vs-without ε_DIS` and `with-vs-without C_rad_DIS / C_rad_h` toggle test per bin.

---

### F-3. A_h carries an unsubstantiated cancellation argument and no charge-separated C_rad / NuclIA gate.

**Claim being criticized.** The plan defines `A_h = (M^{h+} − M^{h−}) / (M^{h+} + M^{h−})` (§1.1) and at G27 says "Form `A_h` per period and for the luminosity-combined result. Propagate statistical errors from the corrected yields / multiplicities. Attach systematic uncertainties from G31, especially NuclIA and model dependence." No closure test for the C_rad cancellation in A_h is required. The to-do list for G27 (line 1944) is purely arithmetic plus a sparse-bin mask.

**Evidence cancellation cannot be assumed.**
- `reference-highQ2-crad-availability.md` §5.1 row 2 explicitly classifies A_h as **CANCELS to first order iff C_rad is hadron-charge-symmetric**, and explicitly says "classify in the systematics matrix as **partial** ... do not assume full cancellation without measuring."
- `reference-nuclia-correction-highQ2-sidis.md` §6 ("A_h"): "NuclIA **does NOT cancel** in A_h. ... Expected fractional impact on A_h: **a few times 10⁻³ absolute**, the same order as the measurement itself. NuclIA is a leading correction for A_h, not a small one." That reference is in the plan's frozen-reference list (F7 + handoff §10).
- The same reference flags an explicit proton-omission systematic specifically because it does not cancel in A_h.

The plan never instructs G24 to produce a charge-separated `C_rad_h(h+)`, `C_rad_h(h−)` for A_h (G24 lists "`C_rad_hplus`, `C_rad_hminus` for multiplicities / `A_h`" — but the wiring into G27 is missing: G27's Preconditions reference only "G25, G31 partial". A_h is then "formed from the multiplicities" with no instruction to use charge-separated C_rad before forming A_h, and no closure-test cell for "A_h with C_rad on / off" or "A_h with NuclIA on / off".

**Why it matters physically.** A_h is expected at the 10⁻² level for HERA-II SIDIS (baryon-number-conservation drives p > p̄, and pi+ > pi−). NuclIA shifts A_h at a few × 10⁻³, i.e. at the 10–30 % relative level. C_rad's residual non-cancellation is per-mil and possibly larger at high-Q² where γ-Z interference modifies the inclusive radiative correction. The plan's wording invites the implementer to ignore both because they are "small corrections that mostly cancel" — exactly the wrong characterisation per the in-house references.

**Analysis-killing vs follow-up.** Analysis-killing for A_h. The fraction of A_h's expected signal carried by these corrections is large enough that omitting them moves the central value by an O(1) fraction of the result.

**Minimum fix.** G27 must (a) require ε_h and C_rad_h to be applied **per charge** before forming M^{h±} (this is implicit but not enforced; line 1939 says only "Divide by `N_DIS_corrected * Δz * ΔpT,h`"), (b) require a NuclIA-on / NuclIA-off closure test on A_h with a numerical exit criterion, (c) require a `C_rad(h+) − C_rad(h−)` differential check with a numerical exit criterion, and (d) carry the proton-omission systematic as a named source in G31 (it is currently absent from the G31 source list).

---

## Major Concerns

### MC-1. The plan never numerically reconciles A_BC with the realised e+p / e−p luminosity asymmetry, which has changed the headline ratio by ~14 % vs the GRL.

`lumi_effective_per_period_v5f.json` makes the e+p / e−p ratio 1.04299 (realised) vs 1.193 (GRL). Section 7.2 of the §13 anchor block hard-codes the realised numbers in code, but the **plan body** does not check that the per-period A_BC weighting actually defends against the regression to GRL totals anywhere downstream of the script that wrote those numbers. The A_BC reference document §4.3 still cites the GRL values (53,706 / 105,028 / 57,573 / 140,306) as the "working period values currently documented in the high-Q2 status snapshot"; the plan does not flag that those numbers are now superseded for A_BC purposes by the realised values. A downstream agent reading the A_BC reference will adopt the wrong four-period weights. The realised-vs-GRL ratio differs by 14 % at the beam-charge sum level — well above any expected A_BC physics signal at HERA, and capable of faking the entire observable.

**Fix.** Insert into F10/F12 and into G28 the explicit instruction "use the four realised per-period luminosities {37981.522, 69678.028, 22798.097, 58470.084} nb⁻¹ as both the per-period normalisation in A_BC^{DIS} closure (G29) and in the four hadronic variants. The GRL totals are not to be used as luminosity denominators anywhere in G28/G29." Update `reference-ABC-scope-and-blinding-highQ2-sidis.md` §4.3 likewise — and have G28 explicitly fail if the script reads from the GRL JSON instead of the realised JSON.

### MC-2. The G29 unblinding criterion is per-mille at the per-bin level but the reference quantity is not sourced.

G29 numerical exit: "Inclusive `A_BC^{DIS}` agrees with the chosen gamma-Z expectation at the **per-mille** level after the validated correction chain." Open question OQ-gammaZ (§0.2) defers whether the prediction is a published number or an in-house computation. The plan treats this as a paper-trail decision; it is in fact a physics-correctness decision. The gamma-Z contribution to A_BC^{DIS} is Q²-dependent (the A_BC reference §6.2 expects ~10⁻³ for Q² > 1000 GeV²) and the per-mille tolerance is **the same order as the predicted effect itself**. A per-bin exit at "per-mille" is therefore a 1σ-of-signal tolerance, not a control. Two failure modes:

1. If the chosen reference is bin-integrated but the data is per-bin, the test will pass or fail by where mass concentrates inside each bin, not by physics.
2. Per-mille is below the per-period luminosity uncertainty (§4.4 of the A_BC reference, 2.5 % flat per period) and below the realised lumi error implicit in (lumi_effective).json. A "per-mille agreement" exit criterion that is below the systematic floor of the measurement is non-falsifiable: the analyst will report agreement.

**Fix.** Replace G29's exit with "consistency within (stat ⊕ syst) per bin, where syst includes per-period luminosity, NuclIA, C_rad, and trigger systematics; the reference value source is a citation-frozen calculation specified in advance, not an in-house calculation done after seeing the data; pre-mille is the **goal**, not the exit criterion."

### MC-3. Charge-flip handling (2.38 % per the schema delta) is nowhere in the plan.

The v5f schema delta memo §6 reports a measured **charge-flip rate of 2.38 %** on matched tracks. A charge-flip is exactly an h+/h− migration: it pulls events from the A_h numerator into the A_h denominator (and across the beam-charge bins of A_BC, depending on where the e+/e− track was assigned).

- **G21 response matrix.** The plan asks G21 to "Build 1D response matrices in `Q²`, `x`, `z`, `pT,h`, and `phi_h`" and a 4D matrix. It does not ask for a 2 × 2 charge-migration matrix or for the 4D × charge matrix that would let unfolding correct charge-flip. Charge is treated as a label, not as a migrated coordinate.
- **G27 A_h.** A 2.38 % h+→h− flip on a signal at the few-percent level is a leading correction; the plan never lists it.
- **G28 A_BC.** A charge-flip can move tracks between beam-charge bins via two routes: (i) hadron-charge flips affect A_BC^{h±} bookkeeping, (ii) any charge-conditioning in event selection (e.g. P6/F11 enforces no charge requirement on the DIS event, so this is the smaller of the two channels). The plan handles neither.

**Fix.** Either add a "G21.5 charge-flip response" with explicit numerical exit, or require G21's response to include charge as a migrated coordinate. Require G27/G28 to apply the charge-flip correction (or carry it as a named, non-trivial systematic with an explicit unfolded-vs-not-unfolded comparison).

### MC-4. The Option H mask's known +18 % M^{h±} shift is not gated.

The schema-delta memo §4.2 states explicitly that "**At high-Q², the electron contamination is HIGHER** than at low-Q²" and projects "approximately **−18 %** relative to v5b" for M^{h±} at high-Q² (versus the spec's −11.5 % at low-Q²), driven by the explicit electron + muon vetoes in the Option H mask. The memo flags this as an empirical extrapolation that must be re-derived on real data.

The plan does not enumerate this as a gate. G25 has no exit criterion against this prior expectation. If implementation produces only a few-percent shift or a wrong-sign shift, the plan offers no immediate red flag. The expected shift is large enough that misapplying the mask (say, dropping the gen-side requirement, or applying the mask asymmetrically in numerator vs response matrix) would not be visibly wrong from a "M ≥ 0" sanity check.

**Fix.** Add to G25 numerical exit: "M^{h±} differs from v5b by roughly the expected fraction of vetoed contamination (~15 % electron + ~1 % muon at high-Q² per the schema-delta memo). Out-of-band shifts must be explained or are a hard failure." Also require G20's acceptance numerator and the G21 response matrix to be built with the **same** Option H mask as G25; the plan currently asserts this via F11/F3 but does not exit on it.

### MC-5. G19's >2σ binning rule is under-defined and weak.

- "σ" is defined (4.1) as the **bin-center or worst-case** residual width, with the worst-case applied only "if σ varies strongly within a bin" — strongly is not defined.
- For Q² and x the σ definition is a **fractional** residual (`(Q²_reco − Q²_gen)/Q²_gen`); the test `bin_width >= 2 σ` then compares an **absolute** edge difference to a fractional σ. The two quantities have different units and the comparison is wrong unless explicitly converted. §4.1 says "convert to an effective absolute or fractional `σ` at the bin center" — but the comparison rule in §4.2 just uses `sigma_local` without saying which.
- "Worst case" is undefined; one option is the worst sub-bin σ, another is the worst tail width (RMS or p16–p84 half-width, both of which G18 records). Per the same bin, RMS and Gaussian σ can disagree by 30–50 % for non-Gaussian residuals, especially at boundaries between calibrated and uncalibrated regions of the LAr at high-Q².
- φ_h: the resolution at high-Q² depends on LAr azimuthal resolution, and `reference-LAr-electron-scale-highQ2-sidis.md` is partially open. G19's rule is not robust against that openness.

**Fix.** Pin the definition: σ_local = max(Gaussian-core fit σ, half-width of the 68 % interquantile range) over all sub-cells of the bin, restricted to populated sub-cells with ≥ 50 entries; for Q² and x, the bin-width test is on the fractional bin width vs the fractional σ at the bin-center. Add the criterion "if any sub-cell of a candidate bin has σ that exceeds half the bin width, the bin fails regardless of the central-value σ."

### MC-6. G21's response-matrix-normalization choice is left to the implementer.

G21 numerical exit: "4D response matrix is finite, sparse, and normalized consistently (`row` or `column` normalization documented explicitly)." The plan does not pick one and the choice changes the meaning of the unfolded result. If row-normalised (P(reco | gen)), it must be inverted; the inversion absorbs acceptance only if it is built with the gen denominator that includes ε_h. If column-normalised (P(gen | reco)), the snippet `bbb = (reco_data / reco_mc) * gen_mc` of G23 is approximately P(gen | reco)-like and **does not commute with the acceptance step**. Letting the implementer pick is what breaks F-1 above.

**Fix.** Pin to a specific convention with an explicit formula for `M^{h±}` in BBB regime and unfolded regime that uses the chosen convention. Tie both formulae back to the same per-bin ε_h.

### MC-7. The C_rad cancellation for M^{h±} is asserted but never numerically gated.

G24 ("Method" bullet 4): "Respect cancellation guidance from the reference: multiplicity residual does not cancel completely; A_h mostly cancels but must be measured; A_BC does not cancel." But none of G24, G25, G27 require a per-bin numerical check of (C_rad_DIS / C_rad_h) and how far it deviates from unity. The reference (§5.1) gives O(1 %) per bin for M^{h±} cancellation residual but warns this is a statement, not a measurement. Without an explicit numerical exit, the analyst can pass G24 with a single bullet "we computed C_rad" and never verify the cancellation actually happens at the claimed scale.

**Fix.** Add to G25 exit: "Report the per-bin ratio C_rad_DIS / C_rad_h; flag any bin where the deviation from unity is > 5 % for explicit narrative." Same for G27 and the per-charge C_rad ratio.

### MC-8. The "Empty-file" handling rule is missing.

The eps_DIS application memo notes that the v5f data has 651 / 973 empty output files (correct by construction from the /06/ split between em06 and ep0607). MC has zero empty files. The plan never says "skip files with `DisTot.num_entries == 0`" anywhere, even though that exact pattern is the standard idiom in the application memo. If a future agent re-implements the data loader and treats an empty file as an error, jobs will fail; if they treat it as an `open-and-merge` with zero entries, that is fine; if they treat it as a `read N_DIS = 0 and divide`, NaNs propagate.

**Fix.** Add to G25/G26/G28/G30 the explicit empty-file rule: "Skip files where `DisTot.num_entries == 0`; do not double-count their luminosity (luminosity is taken per-run from the per-run CSV joined to the union of run numbers across non-empty files)."

### MC-9. G31's systematics list omits required sources called out by the in-house references.

G31's "Required source list" includes the standard menu but the in-house references add named sources that the plan must carry but does not:

- **Proton-omission NuclIA path** (nuclia reference §4): "Quote it explicitly in the A_h systematic budget, not merged into a generic NuclIA model uncertainty, because A_h is the observable where the proton omission does not cancel." Not on the list.
- **In-range-but-unphysical NuclIA sentinel anomaly** (nuclia reference §12.10): one in 266k tracks has NucliaWeight = −2.58; the clip recommended is `where (>0) & (≤1) else 1.0`. The plan's MC weight snippet at §13 *does* implement the clip — but G31 lists "NuclIA sentinel anomaly / clipping sensitivity" as a source without saying it is the *negative-weight* path versus the standard sentinel; without an explicit name, future implementers may treat "sentinel clipping" as the framework-level `1.0` data return.
- **Charge-flip** (schema-delta §6): not listed as a systematic source.
- **Symmetric event-cut prescription violation (F3 guard)**: listed as `F3-linked guard source`; this is a guard for a bug, not a physics systematic source. Listing it is fine but it does not displace the genuine missing sources above.
- **Empty-file double-counting guard**: not a source; should be a precondition check.

**Fix.** Add proton-omission NuclIA, charge-flip, and Option-H mask sensitivity (Option H is a strong veto chain; varying any veto component is a systematic) as explicitly named sources.

### MC-10. Per-period beam-energy uncertainty (F13 / ep0304 caveat) is asserted but not numerically gated.

Risk 8 in the register says "Beam-energy table for ep0304 remains partly period-default rather than HAT-derived. Mitigation: use F13 as authoritative but carry beam-energy uncertainty explicitly in G31, especially for kinematic migrations and `phi_h`." G31 lists "beam-energy uncertainty from artifact 18 / F13" as a source. Neither the G31 numerical exit nor the per-bin uncertainty propagation requires the **per-run** variation to be re-sampled into the kinematics, only that "uncertainty is propagated." The per-run beam-energy column has per-run uncertainty in artifact 18; if propagated as a single per-period number, it cancels in A_BC by accident even though the realised ep0304 fraction of 27.5 GeV runs is unresolved per the eps_DIS application memo §5.

**Fix.** Require G31 to propagate beam-energy as a per-run-resampled systematic for the kinematic-migration sources (M^{h±} via Q², x, z, pT,h migration; A_BC via the period-dependent normalisation; φ_h via the per-run beam axis).

### MC-11. G29 closure also relies on the data-side cut prescription matching the MC eps_DIS prescription; the prior eps_DIS application memo demonstrates this is not yet true.

The eps_DIS application memo §3 explicitly applied `Empz ∈ [35, 70]` and `Yes ∈ [0.1, 0.6]` — neither matches the frozen P5 / F-set. Per the memo's own §12.1 caveat: "Workstream 1a output was not present at task start. N_DIS_data was computed inline by this task on the same v5f sample with the cuts listed in section 3. If Workstream 1a later writes `docs/data/n_dis_data_v5f_highQ2.json` with a different cut prescription (e.g. the frozen P5 Empz in [45,65]), the eps_DIS-application step must be re-run." G25 currently lists both `n_dis_data_v5f_highQ2.json` and `eps_dis_data_application_v5f_highQ2.json` as inputs **without** stating which cut prescription they correspond to or requiring an explicit re-derivation. If G25 runs with the existing inputs, the multiplicity inherits the non-frozen-cut prescription silently.

**Fix.** G25 must include an exit check that both `n_dis_data_v5f_highQ2.json` and `eps_dis_data_application_v5f_highQ2.json` were produced under the frozen P5 cuts; if not, G25 must re-derive them first. The plan currently relegates this to a Risk-register bullet (Risk 6) instead of a hard gate.

### MC-12. φ_h moment fit at G30 is not paired with a C_rad recipe.

G30 requires correction by acceptance and unfolding "where needed", but the only correction explicitly mentioned in the φ_h workflow is acceptance + unfolding (G20 / G23). C_rad in the φ_h channel is a documented subtlety: it can be applied either φ_h-differential (modifies the cos φ_h and cos 2 φ_h moments through the φ_h dependence of the radiative correction, which is mostly small but non-zero) or φ_h-integrated (cancels by construction in the cos φ_h moment but not in cos 2 φ_h because of acceptance non-flatness). The plan does not say which. The C_rad reference does not call out a φ_h-binned C_rad either, so this is genuinely an open spec — but the plan should escalate it as such, not silently leave the moment uncorrected.

**Fix.** Add to G30 method bullet: "Apply C_rad either φ_h-integrated (with explicit acknowledgement that the cos φ_h and cos 2 φ_h moments inherit no C_rad correction) or φ_h-binned (using a φ_h-binned C_rad from G24); the choice is supervisor-frozen and recorded in `phih_moments_v5f_highQ2.md`."

### MC-13. The A_BC inclusive-DIS closure is the unblinded G15 closure, but the plan moves it inside the analysis package without saying who has approved early exposure.

`reference-ABC-scope-and-blinding-highQ2-sidis.md` §7.4 says A_BC^{DIS}(Q², x) is **not** blinded because "it is the closure quantity that proves e- recovery at G15; it is the gamma-Z reference quantity required for G15.5". The plan inherits this at G29. But the plan also requires the inclusive DIS control to be computed at G28 alongside the hadronic blinded products and reported "numerically" before G29. That is consistent with the reference, but the reference also lists the unblinding sign-off as "SIDIS supervisor sign-off person TBD". The plan never escalates the TBD. If A_BC^{DIS} is reported numerically at G28 and used at G29 with no named approver, and if its physics value drives the unblinding decision for the hadronic A_BC channels at G32, the analysis package effectively unblinds without a documented sign-off.

**Fix.** G28 must declare A_BC^{DIS} numerical reporting only after the named approver is recorded; otherwise the closure variable's exposure constitutes a partial-unblinding act on the hadronic A_BC chain because the same correction toggles (NuclIA, C_rad) drive both. Either freeze the approver or compute A_BC^{DIS} as a final-gate-locked artifact released only at G29 closure.

### MC-14. The dependency graph allows G24 to run before G19, which can produce C_rad tables on the wrong binning.

The dependency graph in §3 marks G24 as depending on G19 (correct), and the Blocking-relationships table at §3.1 says G24 blocks G25/G26/G28/G30/G31. But the "First-week actionable subset" item 4 says "Launch G24 `C_rad` production with the Strategy-2 HAT-driven RAD/NORAD path, including beam-sign and charge separation" in parallel with the G18 + G19 + G20 + G21 + G23 chain. If the first-week list is implemented literally, G24 runs against a non-frozen binning candidate, and its output `crad_v5f_per_bin.json` will need to be redone after G19 finalises the binning. This is a process trap that produces a wasted artifact, but it is more dangerous than waste because a downstream agent may simply re-bin the existing C_rad table by histogram-rebinning rather than re-running on truth tables, which is a category-error.

**Fix.** The first-week list must explicitly defer G24 until after G19 closes (or run G24 only on a clearly-tagged "preliminary binning" output that downstream gates must refuse).

### MC-15. The plan's expected v5b → v5f M^{h±} shift sign-check is empirical but the per-bin numerical exit at G20 against the prior MC prototype is `<= 5 %` shifts in the majority of common bins.

G20 numerical exit: "Relative change from the last accepted MC prototype is `<= 5%` in the majority (>80%) of common bins, OR the deviations are explicitly attributed to truth-link migration / changed binning." The schema-delta memo §4.2 already says the integrated `N_v5d / N_v5b` matched-fiducial numerator changes by factor 1.2466 at high-Q² (i.e. +25 % integrated) — well outside the 5 % exit. The plan's "OR the deviations are explicitly attributed" escape clause then drains the exit of force. Without a numerical baseline for the expected shift per bin (which the schema-delta memo provides at the integrated level but not per bin), G20 can pass with any per-bin shift "explained" by a sentence.

**Fix.** Strengthen the G20 exit to: "Integrated per-charge matched-fiducial yield differs from v5b by 1.2466 ± [analyst-provided tolerance band]; per-bin shifts that deviate from the integrated factor by more than 30 % must be explained by named local truth-link/binning shifts."

### MC-16. G28's A_BC^{sum} formula in §1.1 sums hadron yields **before** dividing by lumi, but the A_BC reference defines yields to be already corrected and luminosity-normalised before summing.

The plan's §1.1:
```
A_BC_sum = ( ((N_ep_hplus + N_ep_hminus)/L_ep) - ((N_em_hplus + N_em_hminus)/L_em) ) / ( ... )
```
sums **raw** charge-separated yields, then divides by per-beam-class lumi, then forms the asymmetry. The reference (§2.4) requires `N_{e+p}(h+ + h−)` to be the corrected charge-summed hadron yield — i.e. corrected per charge first, then summed, **then** lumi-weighted **per period** (not per beam class):

```
N_{e+p}(...) = sum_{p in {ep0304, ep0607}} N_p_measured / L_p
```

The plan's snippet uses `L_ep` and `L_em` (single per-beam-class numbers) and feeds in already-summed-across-periods raw yields. This is mathematically distinct from the reference's per-period normalisation when the two e+p periods have different efficiency or acceptance shifts (which they do: 27.5 vs 27.6 GeV). The plan's snippet biases A_BC by exactly the inter-period acceptance-weighted-by-period-lumi shift, which is the very thing the per-period normalisation is intended to remove.

**Fix.** Rewrite the §1.1 A_BC formulas to match the A_BC reference §4.1: per-period lumi-weighting (`sum_p N_p / L_p`) before summing across periods within a beam class. The plan's hard-coded `L_ep`, `L_em` at §13 are also a trap; replace with per-period luminosities.

### MC-17. The plan never requires a generator pseudo-data closure on the full analysis chain.

The standard test that catches all of F-1, F-2, F-3, MC-1, MC-3, MC-6 is to run the full extraction chain (mask → ε → unfold → C_rad → divide) on **MC pseudo-data** with the truth M^{h±}, A_h, A_BC, σ known, and verify recovery to within stat. The plan does not require this. G23's "closure pulls" test refolds the unfolded truth and compares to reco — that catches unfolding bias only, not the rest of the chain.

**Fix.** Add a gate (G24.5 or new G31 closure entry) "Run the full extraction pipeline on MC pseudo-data and recover the truth observables within stat; deviation by more than 2σ in any populated bin must be explained or blocks G32."

---

## Missing Evidence

1. **C_rad strategy 2 closure on the frozen binning is not produced.** G24 cites the strategy; the actual `crad_v5f_per_bin.json` does not exist. The four-way (h± × beam-sign) table is required for A_BC (per the C_rad reference) but the gate exit does not require evidence that the four-way split is finite per bin (the gate only requires "finite nominal DJANGOH C_rad").

2. **Charge-flip 2.38 % impact on A_h and A_BC^{h±} is not quantified.** The schema-delta memo measures the rate but the plan never produces a propagation table.

3. **NuclIA proton-omission impact bound on A_h is PROVISIONAL per the NuclIA reference §4.** No closure test is scheduled.

4. **A_BC^{DIS} reference value source.** The gamma-Z prediction citation is missing (OQ-gammaZ).

5. **Per-bin acceptance ε_h for the v5f production.** The prior prototype is on RAPGAP31 100k; G20 will produce the full version but the plan does not require ε_h to be reported per-charge with its statistical uncertainty as a stand-alone artifact (G20's output is named but the schema is not pinned).

6. **Per-period beam-energy uncertainty propagation method.** F13 says "use the lookup", G31 says "propagate"; the propagation formula is not specified.

7. **G19's σ definition disambiguation** (Gaussian core vs RMS vs interquantile) and the fractional-vs-absolute σ convention per variable.

8. **G21's row-vs-column normalization choice for the 4D response matrix.**

9. **G29's gamma-Z reference value, per-bin or integrated, and its uncertainty.**

10. **Sign and rough magnitude of the expected A_BC^{DIS}** at the four populated (Q², x) bins, against which "per-mille agreement" can be falsified.

11. **The expected sign and rough magnitude of A_h** by charge at high-Q², so that G27's exit can be informed by physics priors rather than "smooth behaviour".

12. **Explicit confirmation that the v5f data N_DIS and ε_DIS artifacts cited at G25 inputs satisfy the frozen P5 cuts**; the existing artifacts do not.

13. **The chosen approver for A_BC unblinding** (per the A_BC reference §7.6 "TBD").

14. **Explicit treatment of the LAr electron-scale systematic.** OQ-LAr is left open and G31 tags the source as "implemented / deferred / blocking" without numerical exit.

15. **An MC-pseudo-data closure of the full chain.** Not in any G18–G33 gate.

---

## Recommended Next Check

Before any G25 / G26 / G27 implementation is started, rewrite the §1.1 observable formulas to make the corrections explicit:

```
N_h_corr(bin, charge) = U_{response} [ ( N_h_data(bin, charge) * TrigWeightAC )
                                       / eps_h(bin, charge) ]
                       * ( C_rad_h_BORN_over_RAD(bin, charge) )

M^{h±}(Q², x, z, pT,h) = N_h_corr(Q², x, z, pT,h ; ±)
                       / ( N_DIS_corr(Q², x) * Δz * ΔpT,h )

N_DIS_corr(Q², x) = U_{response,DIS} [ ( N_DIS_data(Q², x) * TrigWeightAC )
                                       / eps_DIS(Q², x) ]
                    * C_rad_DIS_BORN_over_RAD(Q², x)

d^4σ^{h±}/dQ²dxdzdpT,h = N_h_corr(Q², x, z, pT,h ; ±)
                       / ( L_data_realised * ΔQ² * Δx * Δz * ΔpT,h )

A_h = ( M^{h+} − M^{h−} ) / ( M^{h+} + M^{h−} )

A_BC^{h±} = [ Σ_{p ∈ e+p} N_p^{h±}/L_p − Σ_{p ∈ e-p} N_p^{h±}/L_p ]
          / [ Σ_{p ∈ e+p} N_p^{h±}/L_p + Σ_{p ∈ e-p} N_p^{h±}/L_p ]
```

where every per-bin per-charge quantity is independently corrected before any combination across charge, period, or beam class. Then re-derive G19–G31's numerical exit criteria against these formulas, and only afterwards re-open the dependency graph. Without this rewrite, the gate chain in its current text will produce numerically wrong M^{h±} and σ even if every gate passes.
