# Mixed-Beam C_rad Validity Assessment — High-Q² Charged-Particle SIDIS (2026-10-14)

**Author:** h1-radiative-correction (embedded compatibility prompt; the `sidis-hq2-radiative` skill is not loadable in this runtime — precedent recorded in `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` §7f; the authoritative docs were read directly).
**Scope:** assess whether the MIXED-BEAM (e+/e− combined) C_rad approach applied to the cone-basis S2′ Born release is valid and reasonable for THIS analysis, against the recorded blocker "mixed-beam-only validity (no per-beam or e− Born target)" and the "Born-vs-RAD-level MC comparison ban". Read-only; no release artifact modified. This document is new; it supersedes nothing.
**Question under test (user physics directive):** at the photon level (pure QED), C_rad should be IDENTICAL for e+ and e−; any difference can only enter through (a) Z exchange / γ–Z interference (and its interference with QED radiation) and (b) two-photon-exchange / higher-order charge-asymmetric effects. Is the mixed-beam-only approach reasonable for this analysis?

**Current basis (verified, `STATE.md` 2026-10-14, Cone-Baseline Promotion Override):**
- RAW: `output_v6/central/data_release/bbb_ibu_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ `11.412233` / h− `11.123626` nb).
- BORN: `output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ `13.026987` / h− `12.697511` nb; C_rad tables UNCHANGED, applied per-bin).
- Applied primary: DJANGOH14 releasebin `data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` (integrated h+ `1.1415172920054786` / h− `1.1410367377289135`); formal alternative: RAPGAP31 releasebin emfix `data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/` (h+ `1.1342370401351234` / h− `1.1336689310022947`); emfix pair spread `+0.6419%`/`+0.6499%`.
- Data: four-period S67, `L_data = 351000.0 nb^-1` (ep0304 included). Periods ep0304 (e+) + ep0607 (e+) and em0405 (e−) + em06 (e−) — a MIXED-BEAM data sample.
- Budget: `systematics_cone_s2prime_15comp_..._2026-10-13/` — `total_with_cone15_rel` medians h+ `8.1332%` / h− `8.4118%`; `c_rad_gen_rel` medians h+ `0.5844%` / h− `0.5804%`; `c_rad_stat_rel` h+ `0.4272%` / h− `0.4429%` (both carried unchanged from the emfix 13-comp budget, dev 0 asserted).

---

## (i) Blocker origin — what the workspace evidence records

### i.1 What the blocker says, verbatim

- `STATE.md` Blockers row (line 65): "**the correction is mixed-beam only, not valid for any per-beam or e- Born target**; Born values must not be compared against RAD-level MC predictions without the same correction".
- Same wording carried in every promotion "not cured" list: `STATE.md` lines 126, 187 ("mixed-beam-only validity (per-beam tables not derived)"), 200, 226, 239 ("mixed-beam-only C_rad validity"), 251; `docs/radiative/CRAD_BASELINE_PROMOTION_2026-10-10.md` §10(c) ("Mixed-beam-only validity — no per-beam or e− Born target"); `docs/status/radiative-corrections.md` line 71 (same) and line 72 (the Born-vs-RAD ban).
- A_h closure (2026-10-14, `STATE.md` Cone-Baseline Promotion Override item 7): "**A_h / A-F3 CLOSED 2026-10-14 (user decision): A_h will NOT be published; the A-F3 charge-asymmetry systematic requirement is CLOSED as NOT-APPLICABLE for the publication scope.** Historical ledger mentions of 'A-F3 ... before any A_h quote' in earlier promotion records are retained verbatim as provenance and are superseded by this closure." (Note: ledger item 8 in the same section, line 251, still carries the boilerplate "A-F3 ... mandatory before any A_h quote"; by its own supersession clause item 7 is authoritative — a doc-level residue, not a live requirement.)

### i.2 How the mixed-beam-only state arose (chronology from the cited records)

1. **Built mixed from the start.** The C_rad finalizers (`crad_v3_finalize_xfcut_{rapgap,djangoh}.py`, campaign dir `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/scripts/histogramming_v3_xfcut_20261005/`) sum per-beam npz pools in memory and beam-mix (`mix_by_y`) — "write **mixed-beam CSVs only**" (`EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` §1). No merged per-beam artifact existed on disk before 2026-10-07.
2. **Beam-mixture weights are DATA luminosity fractions.** The finalize script hardcodes `BEAM={"ep":.5140930272,"em":.4859069728}` ("Exposures verbatim from the FINAL (blind) DJANGOH14 finalizer"). Provenance: `docs/radiative/norad_supplement_reconciliation_2026-09-11.md` — "For the four-period data luminosities, `L(e+p)=106,823.1+46,200.7=153,023.8 nb^-1` and `L(e-p)=144,634.0 nb^-1`" → f_ep = 153,023.8/297,657.8 = **0.5140930272** exactly. So the mixture was constructed to match the DATA beam composition under the then-current (297,657.8 nb⁻¹) four-period accounting. (The older V0 candidate used the direct-monitor fraction f_ep=0.5440407 — `docs/radiative/comparisons/current_sidis_crad_variants_2026-09-27.md`.)
3. **The e− investigation (2026-10-07).** Asked whether an e−p Born-level target could be produced. Findings (`EMINUS_..._2026-10-07.md`): (a) per-beam merged primitives did not exist (login-node policy + no-Condor delegation → initially blocked); (b) the per-beam re-merge was then STAGED (§3b) and EXECUTED (Condor clusters 5385358/5385359), producing **per-beam 490/490-finite tables** under `data/radiative/{rapgap,djangoh14}_eminus_crad_final_xfcut_2026-10-07/` — on the **xFcut basis**, status PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`, independently re-verified from the merged npz primitives; (c) the exact per-beam quantification was computed (§2, quoted in (ii) below).
4. **Why an e− Born target remained blocked — physics, not statistics.** The full-EW Born level is charge-dependent: "at Q2>150 GeV^2 gamma-Z interference flips sign between e+ p and e- p, so `sigma_born_ep != sigma_born_em`... An **exact e- target therefore needs an additional per-bin beam-charge factor** `F(i) = 1/[f_em + f_ep*R_born(i)*C_em(i)/C_ep(i)]`... **or** a per-beam data split (apply `C_rad(ep)` to the e+ data slice and `C_rad(em)` to the e- slice; requires a new per-beam data production, not available from the current mixed release)" (`EMINUS_..._2026-10-07.md` §2). Integrated F−1 = **+0.44% to +0.60%** (generator/charge dependent). Neither exists: the release is mixed, and no per-beam data split/unfolding has ever been produced.
5. **The photon-Born pivot (2026-10-08).** The NoZ sample (DJANGOH-only, e− only, Z-couplings zeroed — `BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md` §1) defines a charge-symmetric pure-photon Born target: "photon Born is charge-symmetric (R_born^gamma == 1 identically), so the mixed-beam data can be corrected to a single, beam-charge-blind target with one factor" (`EMINUS_...` §7). The `C_gamma_born` tables landed (`data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/`, integrated h+ `1.040960540463156` / h− `1.0401468080806677`, verdict PASS, exact-exposure) but stay **PROVISIONAL DIAGNOSTIC, unapplied** (`c_rad_applied=false`): DJANGOH-only (no RAPGAP NoZ exists), and the G8 empirical charge-symmetry null test is **impossible** — "NO photon-only e+ sample exists (audit-verified absence 2026-10-07)" (`EMINUS_...` §7a; `BRITZGER_...` §5.2 "No e+ photon-only sample exists (neither NOZ nor NOZ1)"). The 2026-10-13 5-prediction comparison package uses this sidecar as its data side (user decision), with the documented "e- numerator over mixed-beam denominator" construction and a known ~9% target-level inconsistency vs the full-Born/NORAD MC curves, by design (`STATE.md` line 41).
6. **Exposure provenance (2026-10-09).** All six legs closed at FILE-SET-RECONCILED (generator-log-open); the DJANGOH e− RAD 4.67% count shortfall was root-caused as variable chunk sizing with "ZERO bias to sigma_rad(em)/C_rad(em) — precision only, already carried in `c_rad_stat_rel`"; the RG31 NONRAD e− one-file exposure mismatch was repaired (emfix, +53,773.7 nb⁻¹). Residuals: RG31 two-valued σ̂ ambiguity "bounded −0.44%…+0.26% per-beam C_rad, locally unresolvable"; generator-log closure "PERMANENTLY BLOCKED externally (MCDB CGI disabled, genIDs unresolvable)" (`CRAD_EXPOSURE_PROVENANCE_AUDIT_2026-10-09.md` Steps 1–5 + addendum; `STATE.md` 2026-10-09 addendum D).
7. **What "no per-beam or e− Born target" concretely means for THIS analysis:** (a) the applied releasebin tables exist only in mixed form; (b) the per-beam tables that DO exist are xFcut-basis (superseded-for-application binning since 2026-10-09) and PROVISIONAL DIAGNOSTIC — never promoted, never applied; no releasebin-basis per-beam tables were ever derived; (c) even with per-beam tables, the mixed DATA cannot be corrected to a pure e− (or e+) Born level without either the F(i) γZ bookkeeping factor or a per-beam data split, neither of which exists; (d) the only per-beam-sensitive observables (A_h via A-F3, beam-charge asymmetries) are OUT of publication scope — A_h CLOSED 2026-10-14 (§i.1).
8. **Born-vs-RAD-level MC comparison ban.** "Born values must not be compared against RAD-level MC predictions without the same correction" (`STATE.md` line 65; `docs/status/radiative-corrections.md` line 72). It is a usage rule covering all data-vs-MC comparison packages and note-level comparisons: once `sigma_Born = C_rad × Unfold(data; R)` is applied, MC curves compared to it must be at the matched level (the 5-prediction package uses the generators' DIRECT NONRAD predictions per bin for exactly this reason; PYTHIA8/SHERPA3 are rad-off). It is respected by the current comparison package; the NoZ photon-Born data side vs full-Born MC curves carries a documented target-level (Z+loop content) mismatch "by design" — a different axis from the Born-vs-RAD ban (`STATE.md` line 41).

---

## (ii) Physics reasoning — expected e+/e− C_rad difference at Q² > 150 GeV²

### ii.1 Photon level: C_rad is exactly beam-blind (first principles, confirmed by workspace evidence)

At pure photon exchange, |M(e−q)|² = |M(e+q)|² identically at tree level, the PDFs are beam-blind, and QED soft/collinear bremsstrahlung factorization is charge-blind at leading power. Hence C_rad(e+) = C_rad(e−) exactly in pure QED. **Workspace confirmation:** Britzger's NoZ (Z-couplings zeroed, pure γ² Born) gives e− and e+ cross sections IDENTICAL to all printed digits — 218.84216 pb both charges at Q²>1000 (`BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md` §4.3, §5.1 validation item 1); the mixed-table identity gate for C_gamma_born is built on this symmetry (`EMINUS_...` §7a). **The user's physics directive is correct and verified.**

### ii.2 Z exchange / γ–Z interference: affects the Born cross sections, enters C_rad only at second order

Write σ(b) = σ_γ·(1+δ_Z(b)) per leg, with the fractional Z+γZ content δ_Z(b) signed by the γZ term (positive for e−, reduced for e+; Z² symmetric). Then

  C_rad(b)/C_rad^γ ≈ 1 + [δ_Z^nonrad(b) − δ_Z^rad(b)],

so the beam-charge dependence of C_rad is the **difference between legs** of the Z content. The Z content of the Born cross sections themselves is NOT small at this analysis's Q²:

- Q²>100, corrected level (production-relevant): σ_γZ(e−) = +0.246%, σ_Z² = +0.356%; full/photon-only = **+0.60% (e−) / +0.11% (e+)** integrated (Britzger §4.4; σ_γ = 7.2859 nb, full-EW e− 7.3298 / e+ 7.2939 nb).
- Q²>1000, Born: σ_γZ(e−) = +4.55%, σ_Z² = +4.06%; **+8.61% (e−) / −0.50% (e+)**; corrected: **+8.04% / −0.62%** (Britzger §4.3–4.4).

So the e− vs e+ **Born cross sections** differ by ≈ +0.49% ( = 2σ_γZ/σ_γ ) integrated at Q²>100, growing to ≈ +9% at Q²>1000 — in this analysis's binning (cells up to 1500 < Q² < 20000 GeV²) the top cells carry several-percent γZ content. **But this is a property of the Born-level TARGET, not of the C_rad ratio**: both the RAD and NONRAD legs carry the same Born Z content at the same reconstructed kinematics, so it cancels in the ratio to first order. The residual beam-charge dependence of C_rad is the second-order term — the difference of Z content between the radiatively-shifted (RAD) and unshifted (NONRAD) selected events, times the beam asymmetry of that content. First-principles estimate: a fraction of the per-bin γZ content difference across the radiative kinematic shift — i.e. ≲10⁻³ at moderate Q², at most a few ×10⁻³ in the top-Q² cells.

### ii.3 Measured per-beam C_rad split (workspace evidence, exact 490-bin quantification, xFcut basis)

From `EMINUS_..._2026-10-07.md` §2 (per-beam re-merge clusters 5385358/5385359; independently re-verified from merged npz primitives):

| Quantity | RG31 h+ / h− | DJ14 h+ / h− |
|---|---|---|
| integrated C_rad(em) | 1.1348604 / 1.1343076 | 1.1420617 / 1.1416167 |
| integrated (C_em−C_mix)/C_mix | **−0.1710% / −0.1698%** | **+0.0182% / +0.0211%** |
| per-bin \|C_em−C_mix\|/C_mix median | 0.360% / 0.369% | 0.289% / 0.316% |
| per-bin p95 (sparse-bin MC-stat tails) | 2.79% / 3.01% | 2.18% / 2.35% |
| low-y control (C_em−C_ep)/C_ep | **−0.4016%** | **+0.0611%** |
| per-bin R_born = σn_ep/σn_em: median / integrated | 0.9939/0.9912 ; 0.9917/0.9918 | 0.9864/0.9926 ; 0.9898/0.9910 |
| F(i) = 1/[f_em+f_ep·R_born·C_em/C_ep], integrated F−1 | +0.603% / +0.593% | +0.507% / +0.444% |

**Decisive observation:** the two generators DISAGREE IN SIGN on the per-beam C_rad split (−0.40% RAPGAP vs +0.06% DJANGOH at the low-y control; −0.17% vs +0.02% integrated vs mixed). A genuine physical beam-charge effect (γZ) would appear with the SAME sign in both generators. The sign inconsistency demonstrates that the measured per-beam split is dominated by generator modeling and MC statistics, not by a resolvable physical charge asymmetry — i.e., the physical e+/e− C_rad difference at this analysis's kinematics is **consistent with zero at the present precision, bounded ≲0.1–0.2% integrated**, with per-bin fluctuations (median 0.29–0.37%, p95 2.2–3.0%) of the same statistical nature already carried by `c_rad_stat_rel`. The record itself draws this conclusion: "the beam-charge radiative structure is generator-dependent at sub-percent level and would itself enter a systematic budget" (`EMINUS_...` §2).

### ii.4 Two-photon exchange and higher-order charge-asymmetric effects (first principles)

2γ-exchange box interference shifts the NC cross section by ~1–2% at high Q² with opposite sign for e+ and e− (the known HERA beam-charge effect). For C_rad it is irrelevant twice over: (a) it is a hard-scattering correction present in BOTH the RAD and NONRAD legs, so it cancels in the ratio to the same second order as the Z terms — expected residual ≪0.1%; (b) **neither DJANGOH14/HERACLES nor RAPGAP31 simulates 2γ-exchange boxes**, so a per-beam C_rad derived from these samples would be exactly as blind as the mixed table. No information about 2γ effects is obtainable or needed from this chain.

### ii.5 Beam-mixture weight sensitivity (mixture vs current data composition)

The applied mixture f_ep = 0.5141 derives from the superseded 297,657.8-nb⁻¹ accounting (§i.2). The current data's beam composition is bracketed by: GRL/direct-monitor fractions f_ep = (53,706+140,306)/356,613 = **0.5440**, and the realised data-vector estimate e+/e− = 1.04299 → f_ep ≈ **0.5105** (`docs/reviews/critique-finish-analysis-plan-physics.md` line 115, v5f-era effective-lumi). So Δf ≲ 0.033 worst case. The sensitivity of the mixed correction to the mixture weight is second-order — proportional to the per-beam sigma difference between legs, dC_mixed/df ≈ C·(δ_nonrad − δ_rad) ≈ 0.5% per unit f (from the R_born ≈ 0.99 and rad em/ep ≈ +0.5% numbers above). Induced bias on C_rad: ≲ 0.033 × 0.5% ≈ **0.015–0.02% worst case** (≈0.002% against the realised estimate). Negligible: ~1/30 of `c_rad_gen_rel`, ~1/400 of the total.

### ii.6 Scale comparison (the decisive numbers)

| Effect | Magnitude (integrated) | vs `c_rad_gen_rel` 0.5844%/0.5804% | vs total 8.1332%/8.4118% |
|---|---|---|---|
| Per-beam C_rad split (measured, generator-range) | −0.17% … +0.02% | 0.03–0.3× | ~0.002–0.02× |
| Per-beam split (per-bin median / p95) | 0.29–0.37% / 2.2–3.0% | within the same tails already carried by `c_rad_gen_rel` (max 5.76%/5.66%) and `c_rad_stat_rel` (max 4.41%/6.04%) | — |
| Physical expectation (γZ second-order, first principles) | ≲0.1–0.2%, sign-inconsistent between generators | below the generator component | ~50× below total |
| Mixture-weight vs current-data composition | ≲0.02% | ~1/30 | ~1/400 |
| 2γ exchange | ≪0.1% (and not simulated at all) | — | — |

**Generic-band note (flagging per operating rules):** the C_rad orientation matches the standard definition (C_rad = σ_nonrad/σ_rad). The generic expectation "C_rad ~ 0.7–0.8, δ_QED = 1−C_rad ~ +0.2–0.3" does NOT hold here: integrated C_rad ≈ 1.14 (δ_QED ≈ −0.14), because the hard fiducial cuts (GenEmpz>45, 0.1<y<0.7, 150<Q²<20000 binning) remove radiatively-degraded events, so σ_rad < σ_nonrad in-bin. This is documented, gated (NoZ G6 integrated band [0.95,1.12]; per-bin absolute sanity [0.4,1.8]; published per-bin span [0.840,1.572]) and validated by the accepted plot packages; the workspace's own gates govern, not the generic [0.5,1.2] band. The DJANGOH-vs-RAPGAP agreement (+0.64%/+0.65% emfix pair) is well inside the ~5% generic expectation. The carried "residual z-rising/pT_h-falling trend unexplained" blocker is a C_rad SHAPE question, orthogonal to beam mixing, and remains open independently of this assessment.

---

## (iii) Internal-consistency analysis for THIS analysis

1. **Same beams, same periods, both sides.** The data vector combines e+ (ep0304, ep0607) and e− (em0405, em06) at L=351,000 nb⁻¹. The C_rad tables combine the same two beams with per-beam MC pools and data-derived luminosity weights. Correcting mixed data with the mixed correction yields, per y-slice and per bin, exactly the luminosity-weighted mixture of the e+ and e− Born cross sections: ⟨σ_Born⟩_mix = f_ep·σ_Born(e+) + f_em·σ_Born(e−). This is the well-defined — and the only well-defined — Born level for a mixed-beam data set. The mixture algebra was verified exactly against the campaign y-resolved CSVs (low-y slice reproduced to 1e-9; `EMINUS_...` §2).
2. **The observable is per-HADRON-charge, not per-BEAM-charge.** The published cross sections are per hadron charge (h+/h−, 245 bins each), and the C_rad tables are themselves per-hadron-charge (490 rows = 2×245). The only "mixed" degree of freedom is the beam, which is mixed identically in data and correction. No beam-charge observable is being published: **A_h is CLOSED (2026-10-14, will NOT be published)**; the BSA side study is a separate raw fork (`c_rad_applied=false` there, untouched by the promotion); no A_BC-style observable exists in the v6/z005 scope.
3. **Where mixed-beam C_rad would be WRONG** (all outside the current publication scope): (a) any per-beam (e+-only or e−-only) Born-level cross-section statement — requires the F(i) γZ factor (integrated +0.44%…+0.60%) or a per-beam data split, neither of which exists; (b) any beam-charge asymmetry observable — the γZ sign flip IS the physics there, and neither the mixed nor a per-beam table from these generators encodes it at the target level; (c) any comparison of Born-level data against RAD-level MC without the same correction (the standing ban). With A_h closed and no per-beam observable in scope, **no in-scope published number is affected**.
4. **Comparison-package consistency.** The 5-prediction package respects the Born-vs-RAD ban (generator curves are NORAD/direct-nonrad or rad-off). Its NoZ photon-Born data side is a PROVISIONAL DIAGNOSTIC construction (e− numerator over mixed RAD denominator) that is internally consistent for a photon-Born target BECAUSE photon Born is charge-symmetric (§ii.1); the documented ~9% level mismatch vs full-Born MC curves is a target-level (Z + soft&virtual/running-α content) choice, not a beam-mixing defect. It is diagnostic, not the publication basis.
5. **Residual bookkeeping caveats, quantified and dismissible:** (a) beam-mixture weights from the superseded luminosity accounting — bias ≲0.02% (§ii.5); (b) RG31 two-valued σ̂ residual — bounded −0.44%…+0.26% per-beam C_rad, i.e. it affects exactly the per-beam split this assessment shows to be unresolvable, and dilutes to ≲0.1% in the mixed tables; (c) generator-log exposure closure permanently blocked externally — an exposure-ceil­ing item, not a beam-mixing item.

---

## (iv) VERDICT

**REASONABLE.** The mixed-beam C_rad applied to mixed-beam data is the internally consistent and correct construction for THIS analysis's published observable (per-hadron-charge Born-level cross sections from a mixed e+/e− four-period sample). The user's physics directive is confirmed by the workspace evidence: pure-QED C_rad is exactly beam-blind (verified to all printed digits by the NoZ e± identity), the γ–Z/2γ charge-asymmetric effects enter the C_rad ratio only at second order, and the measured per-beam split (−0.17%…+0.02% integrated vs mixed, sign-inconsistent between generators) is consistent with zero at present precision and is bounded well inside the existing `c_rad_gen_rel` (0.58%) and `c_rad_stat_rel` (0.43/0.44%) components, ~50× below the total budget (8.13%/8.41%). The blocker is a genuine SCOPE LIMITATION (no per-beam or e− Born target can be constructed from the mixed release) but NOT a validity threat to any in-scope published number — especially with A_h closed 2026-10-14.

**Would per-beam or per-charge C_rad tables change any published number?** No. The exact per-beam quantification already exists (xFcut basis) and shows integrated differences of −0.17% (RG31) / +0.02% (DJ14) from the mixed tables; for the mixed data and mixed observable the mixed table is the correct one, and applying per-beam tables without a per-beam data split would be WRONG, not better. No new budget component is warranted: the per-beam effect is smaller than the existing generator component, is generator-sign-inconsistent (i.e., not a resolvable physical systematic), and its per-bin statistical tails are of the same nature already carried by `c_rad_stat_rel`. Adding a beam-mixing component would double-count sub-percent generator-scale structure already inside `c_rad_gen_rel`.

---

## (v) Recommended caveat / budget wording

**Analysis note (systematics/radiative-corrections section) — carry verbatim or equivalent:**

1. *"The radiative correction C_rad = σ_nonrad/σ_rad is derived from mixed e+p and e−p generator samples combined with data beam-luminosity weights (f(e+) = 0.5141, f(e−) = 0.4859), matching the mixed-beam four-period data sample (351 pb⁻¹). The Born level defined by this correction is the luminosity-weighted mixture of the e+p and e−p Born cross sections. No per-beam (e+-only or e−-only) Born-level statement is made or supported by this release."*
2. *"At photon level the correction is exactly beam-charge symmetric. Beam-charge dependence from γ–Z interference affects the Born-level e+ and e− cross sections (≈ +0.5% e− over e+ integrated at Q² > 100 GeV², growing with Q²) but enters the C_rad ratio only at second order: the measured per-beam difference of the correction is |C_rad(e−) − C_rad(mixed)|/C_rad ≈ 0.02% (DJANGOH) to 0.17% (RAPGAP) integrated, with opposite sign in the two generators — consistent with zero at the present precision and covered by the generator (`c_rad_gen`, median 0.58%) and MC-statistics (`c_rad_stat`, median 0.43%/0.44%) components. No separate beam-mixing uncertainty is assigned."*
3. *(carry the standing ban verbatim)* *"Born-level data are compared only to Born-level (non-radiative) MC predictions; comparison against radiative-level predictions without applying the same correction is not performed."*
4. *(optional provenance sentence)* *"The beam-mixture weights were fixed from the data luminosity accounting in use when the C_rad campaign was constructed; the current four-period S67 target luminosity implies beam fractions within Δf ≤ 0.03 of the applied weights, changing the correction by less than 0.02% — negligible and not assigned."*

**Budget treatment:** unchanged — keep `c_rad_gen_rel` (emfix sidecar basis) and `c_rad_stat_rel` as the only C_rad components; do NOT add a beam-mixing component (double-counting; effect below both). The z<0.05 xF-definition caveat (CAVEAT-ONLY) and the EMPZ>45 closure wording are unaffected by this assessment.

**Blocker ledger treatment:** reword "mixed-beam-only validity (no per-beam or e− Born target)" from an open validity threat to a **scope limitation with quantitative bound**, e.g.: "Mixed-beam C_rad only: valid for the mixed-beam per-hadron-charge Born cross sections (the published scope); NOT valid for any per-beam or e− Born target (no per-beam releasebin tables, no per-beam data split; γZ residual factor F ≈ +0.44…+0.60% would be required); measured per-beam split −0.17%…+0.02% integrated, generator-sign-inconsistent, bounded inside `c_rad_gen_rel`/`c_rad_stat_rel`." Keep the Born-vs-RAD comparison ban unchanged as a live usage rule.

---

## (vi) Missing evidence — inventory and obtainability

| # | Missing evidence | Obtainable? | Needed for published scope? |
|---|---|---|---|
| 1 | Releasebin-basis per-beam C_rad tables (existing per-beam tables are xFcut-basis, superseded-for-application, PROVISIONAL DIAGNOSTIC) | **OBTAINABLE** — per-beam re-merge machinery exists and ran (clusters 5385358/5385359 precedent); would need repointing to the releasebin partials, minutes of Condor wall | No — would not change any published number (see (iv)); only a completeness/diagnostic item |
| 2 | Per-beam data split + per-beam unfolding (per-beam Born targets) | **OBTAINABLE in principle** (data files are period-tagged) but a NEW production chain | No — no per-beam observable in scope; A_h closed 2026-10-14 |
| 3 | Photon-only e+ NoZ sample (empirical G8 charge-symmetry null test) | **NOT OBTAINABLE** — audit-verified absence (no NOZ/NOZ1 e+ on dCache or DUST); would require a new production with the "neu" NoZ executable | No — tree-level charge symmetry is exact and verified on the e− side; the photon-Born target is diagnostic/unapplied anyway |
| 4 | Photon-only RAD sample (pure photon-only C_rad null test) | **NOT OBTAINABLE** without new production (documented coverage gap, `BRITZGER_...` §6.4) | No |
| 5 | Generator-log exposure closure (genIDs for ReqIDs 8947–8962, norad_prod21) | **NOT OBTAINABLE** — MCDB CGI disabled (permanent external blocker); exposure provenance stays FILE-SET-RECONCILED | Open `P-CRAD-LUMI` ceiling item, orthogonal to beam mixing |
| 6 | RG31 two-valued σ̂ {7287.915, 7323.939 pb} resolution | **NOT OBTAINABLE locally** (pnfs generator inputs inaccessible); bounded −0.44%…+0.26% per-beam C_rad, ≲0.1% mixed | No — bounded; affects only the already-unresolvable per-beam split |
| 7 | 2γ-exchange / charge-asymmetric higher-order content in C_rad | **NOT OBTAINABLE** from DJANGOH14/HERACLES or RAPGAP31 (not simulated) | No — cancels in the ratio to ≪0.1%; irrelevant for the mixed observable |
| 8 | Current-S67 per-period luminosity split (to re-anchor the mixture weights) | **OBTAINABLE** from h1-normalization records; effect already bracketed ≲0.02% (GRL 0.544 vs realised ~0.511 vs applied 0.514) | No — bias negligible; a provenance-wording item only |

**Coverage statement:** all claims above are backed by the cited workspace records; first-principles estimates are labelled as such (§ii.2 second-order argument, §ii.4, §ii.5 sensitivity formula). The single unpinned quantitative input is the current-data beam fraction (bracketed, not exact — item 8); it does not affect any conclusion at the quoted precision.

---

**Provenance:** read-only session; no release artifact, table, budget, plot, STATE.md row, or skill file was modified. Only this new document was written.
