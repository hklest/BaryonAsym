# P-METHOD Decision Dossier — Nominal Unfolding Method for z005 (High-Q² Charged SIDIS)

**Date:** 2026-10-14
**Prepared by:** neutral assessor (charged-hadron SIDIS analysis specialist; **not** the builder of any study cited here)
**Purpose:** evidence base for the `P-METHOD` decision (blocker text: "nominal IBU/BBB vs native v6 TUnfold decision", `STATE.md` carried-blockers list) — to be attacked by a critic and decided by the user.
**Framing:** everything below is at the **raw level** (no C_rad, not Born-level), **diagnostic** in the TUnfold case (`diagnostic_only: true` in every TUnfold study summary), and **no theory commentary** is included or required. All evidence is from text artifacts on disk (JSON/CSV/MD); plot PDFs/PNGs were not used as evidence.

---

## 0. Read-first: observable-basis flag (mandatory)

**Every number in this dossier is on the no-cone S2′ detint-fake variant basis** (response variant `detint_fake_response_variant_S2prime_2026-10-09`, nominal response sha256 `f6431c1c81d1ea3aa6f014e99be93fee2f71153a6dde22826d9fedf7e879330d` — [REL-README], [V1]). Since 2026-10-14 that basis is **superseded as physics basis** by the CONE+S2′ promotion (canonical RAW `bbb_ibu_detints2prime_alphaveto_cone_..._2026-10-13/`, IBU h+ `11.412233` / h− `11.123626` nb; Born h+ `13.026987` / h− `12.697511` nb; canonical budget `systematics_cone_s2prime_15comp_shiftedlegs_..._2026-10-14/`, total medians `5.8776%`/`6.1919%` — [CURREL] lines 5, 15–17; dated addendum 2026-10-14: that budget is itself superseded same-day by the LEG-ENVELOPE budget `systematics_cone_s2prime_15comp_shiftedlegs_legenv_..._2026-10-14/`, medians `5.8032%`/`6.1919%`). The no-cone S2′ triad is retained as provenance only ([CURREL] line 19).

Consequences for this decision:

1. The **method comparison itself transfers**: both candidates were run on identical inputs (same data vector, same variant response, same f_i convention, same binning), so the relative method properties measured here are basis-independent to the extent the cone changes the response only moderately (cone data-vector removal `-0.4897%` vs central; response guards recorded — [CURREL] line 15).
2. **No TUnfold cone-basis chain exists.** All five TUnfold studies (v1–v5) consume the no-cone S2′ variant response ([V1]–[V5] basis blocks). Any option that makes TUnfold nominal (or even a cone-basis cross-check) requires rebuilding the chain on the cone basis.
3. Quote rule (three bases): any quote below must carry the flag **no-cone S2′ basis, raw level** ([CURREP] line 18; [STATE] cone promotion record).

---

## 1. Decision statement and scope

**Decision to be made:** which unfolding method is **nominal** for the z005 high-Q² charged-SIDIS central release (245 truth bins per charge, 7 Q²-x cells × 7 z × 5 pT; 490 reco bins per charge; `L_data = 351000.0 nb^-1` — [REL], [V1]).

**Candidates:**

- **Candidate A — IBU k=4 (+ BBB):** iterative Bayesian unfolding, coherent `body+miss` prior computed from the variant rapgap31 response (R10-asserted — [REL-README]), selected iteration k=4 of 1–10 ([REL]), plus the bin-by-bin (BBB) product as the release's second estimator.
- **Candidate B — native ROOT/TUnfold v6:** `TUnfoldDensity` (ROOT 6.38.04), no prior, custom curvature regularization (322 triplets), `kEConstraintNone`, misses in the reco/input-axis x-underflow bin 0, external per-reco-bin variant-f_i subtraction `d_sig = raw*(1-f_i)` ([V1], [V2] basis blocks).

**Options on the table:**

- **Option A:** keep IBU/BBB as nominal; retain the TUnfold v1–v5 chain as a validated second-method cross-check; close `P-METHOD`.
- **Option B:** switch the nominal method to native TUnfold.
- **Option C:** dual publication (both methods).

**Non-candidates (excluded, with reasons):** RooUnfoldSvd/TSVDUnfold — quarantined, 1D SVD regularization not topology-safe for the flattened multidimensional bin array ([TUSTAT] "SVD Quarantine"). The 2026-05/06 historical TUnfold v4/v5 studies — invalid/superseded encodings ([TUSTAT] "Historical TUnfold Paths").

---

## 2. Candidate descriptions

### 2.1 Candidate A — IBU k=4 + BBB (current nominal)

- Release: `bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/` ([REL-README]). Integrated IBU h+ `11.446174 nb` / h− `11.158554 nb`; BBB h+ `11.455122 nb` / h− `11.169982 nb` ([REL]).
- Coherent prior := variant response truth marginal `body+miss`; the driver structurally cannot consume a central response (pre-flight guard; provenance sha re-asserted) ([REL-README]).
- Positivity by construction: **0 negative nominal cross-section bins** in both charges (re-derived from the budget CSV `nominal_cross_section_nb` column, 490 rows — [BUDGET]).
- Statistical uncertainty from the authoritative 1000-toy sidecar (1000 toys/charge, seeds 12345/22345, IBU k=4, fixed variant f_i/response/prior) ([SIDECAR]).
- Full 15-component systematic budget on the same basis ([BUDGET]); full promoted infrastructure on the **current cone basis** (RAW + Born + shifted-legs budget + sidecar + gate suite 18/18 PASS — [CURREL] lines 15–17).
- Independent-implementation cross-check: RooUnfoldBayes (ROOT 6.38.04 / RooUnfold 2.0.1) reproduces the in-house IBU k=4 to max relative difference `3.31e-15` (h+) / `3.66e-15` (h−) ([CURREL] line 34; earlier record `2.6e-15` — [CLOSURE] lines 93, 119). The TUnfold studies additionally reproduce the release IBU k=4 in-script to `1.72e-15`/`1.75e-15` ([V1] gates).
- Known recorded open gap: the **response body/miss MC statistics are not propagated** in the budget (`fakecol_mcstat_rel` covers only sigma(f_i); body/matched response MC-stat unpropagated — [CURREL] promotion ledger item 3; quantified in-solve on the TUnfold chain at medians `1.4769%`/`1.5752%` — [V4] `stat_decomposition`).

### 2.2 Candidate B — native ROOT/TUnfold v6 (diagnostic chain v1–v5, 2026-10-12 → 2026-10-14)

- **v1** ([V1]): first full S2′-basis study. Tau grid of 20 points; tau rule = largest tau on the historical grid passing plateau gates (refold chi2/ndf ≤ 1.01×chi2(0), neg bins ≤ neg(0), |integral/integral(0)−1| ≤ 0.005). Selected tau `1e-5` (rapgap) / `3e-5` (djangoh); L-curve corner sits at `0.001` (not used). Unclipped solutions have **14/15/11/10 negative bins** (pos-rg/pos-dj/neg-rg/neg-dj); scan minima 9/8/8/8 only in the over-regularized region. Refold chi2/ndf `4.178` (h+) / `3.849` (h−) (chi2 2039.05/488 and 1874.68/487); djangoh-chain chi2/ndf ≈ `10.2`/`9.8`. ρ_i median `0.487`/`0.498`, max `0.9924`/`0.9941` (rapgap), `0.9974`/`0.9973` (djangoh). Integrated `11.433468`/`11.148826 nb`, shift vs IBU `-0.111%`/`-0.087%`; per-bin vs IBU median |ratio−1| `2.994%`/`2.890%`, max `5.415`/`8.185` (unclipped). Closures are deterministic arithmetic-only (self-rg residual median `3.0e-4`/`2.6e-4`; cross rg↔dj residual median `0.028`/`0.025` (h+), `0.030`/`0.027` (h−)).
- **v2** ([V2]): non-negativity study. Step-1 gate (neg_bins == 0 within the plateau) is **EMPTY for all four cases** — negative-bin count is tau-flat over the whole plateau (14/15/11/10) and only falls to 7–9 in the over-regularized region (numeric grid minima 7–9; artifact prose says 8–12) (integral bias −30…−70%, chi2/ndf ~1e3). `kEConstraintArea` scanned and **does not enforce per-bin positivity** (ROOT 6.38.04 `EConstraint = {None, Area}` only; counts unchanged). Fallback: **delivered vector = max(x(tau_sel), 0)** with one-sided per-bin clip bias propagated; 0 negative bins after; `14` (h+) / `11` (h−) clipped bins. Clip bias integrated `+0.028176%` (h+) / `+0.010674%` (h−); one-sided, **not** in the quadrature total. Pre-clip pulls max `1.675` (h+) / `1.071` (h−). Clipped refold chi2/ndf `4.372`/`4.051` (Δchi2 `+89.9`/`+98.2`; the h+ Δ crosses a 1-bin mask change 488→487). Integrated delivered `11.436689`/`11.150016 nb`, shift vs IBU `-0.0829%`/`-0.0765%`; per-bin median |ratio−1| `2.994%`/`2.890%`, max `3.163`/`3.952` (delivered basis). Fold-operator gate vs `GetFoldedOutput` ≤ `8.3e-16` (gate 1e-10).
- **v2.1 addendum** ([V2.1]): additive response to the external review of v2, verdict **SOUND-WITH-CONDITIONS**. Conditions addressed: M1 per-bin method-tension table (one-sided clip interval excludes IBU in `6/14` (h+) / `7/11` (h−) clipped bins; max exclusion significance `0.93σ` bin 201 / `0.43σ` bin 242; max combined pull stat+clip `1.21σ` / `0.88σ`; `5/14` / `2/11` bins with empty 1σ stat-consistent range, pulls up to `1.67`/`1.07`); M2 model-uncertainty clip truncation (corrected model = unclipped spread everywhere; corrected medians `42.50`/`38.36` yield; materially affected bins `18/245` (h+) / `14/245` (h−); corrected total medians `158.14`/`142.75` = `6.81%`/`7.02%` relative); M3 absolute integral bias of the tau rule (rapgap chain `-0.436%`/`-0.439%` at 1e-5; djangoh chain `-3.344%`/`-3.367%` at 3e-5; the v1/v2 gate is tau-RELATIVE and blind to this); M4 stat wording (TUnfold linear covariance vs IBU toy dispersion: **no gate**, ratio of medians `2.1367`/`2.0623`; toy propagation through TUnfold and the clip OPEN); M5 two chi2 implementations identical; M6 f_i geography (max f_i over all 490 reco bins `0.326`/`0.309`; dominant reco bins of clipped truth bins f_i ≤ `0.248`/`0.209`; worst matched efficiency among clipped bins `0.033` (h+) — negatives are structural, not an f_i≈1 pathology). Minors: all "chi2/ndf" are chi2/N_active (488/487), **not** true-dof GOF — per true fit dof (490−245=245) the pre-clip GOF is **~8.32 (h+) / ~7.65 (h−)**. Open items recorded: (1) toy validation of TUnfold stat and clip; (2) P-METHOD; (3) v3 code hygiene + absolute-bias tau gate.
- **v3** ([V3]): 1000-toy Poisson validation (cluster 5549892) through the full production chain **including the clip**; data vector only fluctuated, response/f_i/miss/prior fixed (response-MC fluctuation explicitly out of scope). Tau rule v3 = v1-plateau gates + 1% absolute-bias gate → tau `1e-5` on **both** chains (djangoh closed-system shrinkage `-3.34%` → `-0.45%`; delivered vector **bit-identical to v2**, max dev 0.0). Headline data-scale (`mc_scaled`) results: TUnfold delivered bias median |rel| `0.167%`/`0.226%`; IBU on the same toys `0.0707%`/`0.0678%`; TUnfold coverage (stat band) median `0.696`/`0.695`, m2-corrected total coverage `0.72`/`0.731`; IBU sidecar-represented coverage `0.662` (min `0.013` h+ / `0.366` h−); toy sigma vs reported stat `1.021`/`1.013` (mc_scaled) and `0.975`/`0.969` (data scheme). Self-closure (MC scale, secondary): TUnfold delivered bias `0.093%`/`0.080%`; IBU (coherent prior) `0.0150%`/`0.0161%`; IBU on the djangoh plain-prior chain `1.070%`/`1.170%` — mechanism verdict: a **prior-choice fixed-point basin effect, not a finite-k effect** (IBU reaches its fixed point at k=1 on the exact self-closure input; the coherent marginal restores the exact fixed point, 0.0000% at every k). WP2 clipped-bin forensics (F1-corrected pulls): pos 14 bins → `7` data-driven-sign-instability / `6` local-data-mc-tension / `1` rare-tail-excursion; neg 11 → `7`/`3`/`1`. Status PASS.
- **v4** ([V4]): systematic-uncertainty covariance-in-solve prototype (data-side shifts propagated through `G = GetDXDY`). Central vector and stat **bit-identical to v2** (max dev 0.0). Feasibility: 8 sources in-solve (data-stat reference + hfs_jes/rces/theta + elec_energy/theta + tracking ensemble-mean + fakecol diagonal), 7 post-hoc (model, reg, lumi, phi_h, c_rad×2, detint) — phi_h has **no shifted S2′-basis response artifact** (fabricating one "would be a contrivance"). Stat decomposition: total `5.9747%`/`6.4894%` = data `5.7290%`/`6.3451%` + **response-mcstat `1.4769%`/`1.5752%`** (the IBU budget's recorded open gap, carried in-solve by the TUnfold chain since v1). Totals vs budget: `total_full` medians `9.1630%`/`10.1001%` vs budget `8.1864%`/`8.4079%` (median ratio `1.160`/`1.174`); `total_in_solve` `8.4428%`/`9.4206%` (ratio `1.040`/`1.093`). Same-denominator yield ratios (v4/budget): hfs_jes `1.126`/`1.166`, hfs_rces `1.050`/`1.082`, hfs_theta `1.022`/`1.045`, elec_energy `1.116`/`1.146`, elec_theta `1.427`/`1.268`, tracking `0.999`/`1.000`, fakecol `1.425`/`1.432`, data-stat vs budget ibu_stat `1.451`/`1.448`. Djangoh AddSysError response-side diagnostic report-only: `2.639%`/`2.727%` vs budget ibu_model `1.362%`/`1.427%` (ratio ~2.0; not folded). 500-toy covariance spotcheck: six ensembles, median toy/exact ratios in `[0.980, 0.996]` — PASS.
- **v5** ([V5]): consolidated consistency package. Delivered-vector bit-identity re-confirmed (max dev 0.0); refold chi2/ndf reproduced exactly (`4.1784`/`3.8494`). Global IBU-vs-TUnfold consistency: chi2/ndf(pulls) `0.456`/`0.459` (p ≈ 1); chi2/ndf(full in-solve covariance, 245 dof) `0.513`/`0.470` (p ≈ 1); pull mean `-0.028`/`-0.037`, RMS `0.675`/`0.676`; bins |z|>3: `1` (h+) / `1` (h−); clipped-bin pulls all within `[-1.17, -0.09]` (h+) / `[-0.82, -0.07]` (h−). 55-page package (text evidence taken from the JSON summary only).

**Common inputs (identical for both candidates):** data vector and binmap central/unchanged; S2′ variant responses (rapgap31 nominal, djangoh14 model); external per-reco-bin variant-f_i subtraction; miss handling; `L_data = 351000.0 nb^-1`; 245 truth × 490 reco bins per charge ([REL-README], [V1]).

---

## 3. Criteria-by-criteria comparison

### 3.1 Summary table (h+ / h−; all values no-cone S2′ basis, raw level)

| # | Criterion | IBU k=4 + BBB | native TUnfold (v2 delivered) | Edge |
|---|---|---|---|---|
| 1 | Unbiasedness (data-scale 1000-toy, delivered) | bias median \|rel\| `0.0707%`/`0.0678%` [V3] | `0.167%`/`0.226%` [V3]; closed-system reg. shrinkage at tau_sel `-0.44%` (rg) [V2.1 M3] | IBU (marginal) |
| 2 | Statistical uncertainty (median) | `3.5902%`/`3.8330%` (1000-toy sidecar) [BUDGET] | data-stat `5.7290%`/`6.3451%`; with response-mcstat `5.9747%`/`6.4894%` [V4] | IBU (×1.45–1.57 like-for-like [V4]) |
| 3 | Systematic budget (15-comp, total median) | `8.1864%`/`8.4079%` [BUDGET] | `9.1630%`/`10.1001%` total_full [V4] (ratio `1.160`/`1.174`) | IBU |
| 4 | Negative bins | `0` by construction (re-derived [BUDGET]) | `14/15/11/10` unclipped; `0` only via post-hoc clip of `14`/`11` bins [V1][V2] | IBU |
| 5 | Estimator well-definedness | closed-form iterative map, positive, k=4 of 1–10, reg = k3/k5 half-difference [REL][CLOSURE] | solve + post-hoc `max(x,0)`; one-sided clip bias outside quadrature; covariance structurally unclipped; no positivity constraint available in API [V2][V2.1 m1] | IBU |
| 6 | Coverage (data-scale toys) | sidecar-represented `0.662`/`0.662` [V3] | stat `0.696`/`0.695`; m2-corrected total `0.72`/`0.731` [V3] | TUnfold (moderate) |
| 7 | Validation depth | 1000-toy sidecar closure (G5: 0 and 4-at-edge \|z\|>3, pre-registered, unweakened) [SIDECAR]; full budget validation | deterministic closures [V1] → non-negativity study [V2] → 1000-toy through clip [V3] → covariance-in-solve + 500-toy spotcheck [V4] → global consistency [V5] | TUnfold (broader method-level validation; IBU has deeper release-level validation) |
| 8 | Independent-implementation cross-check | RooUnfoldBayes `3.31e-15`/`3.66e-15` [CURREL:34]; in-study reproduction `1.72e-15`/`1.75e-15` [V1] | none on disk (native ROOT implementation only) | IBU |
| 9 | Infrastructure integration cost (to current cone basis) | zero — promoted triad installed (RAW/Born/budget/sidecar, gates 18/18) [CURREL] | full chain rebuild on cone basis + re-derivation bill (Sec. 4.2) | IBU |
| 10 | Residual known gaps | response body/miss MC-stat unpropagated (median `1.4769%`/`1.5752%` when quantified in-solve [V4]); coverage 0.662 | refold chi2/ndf `4.18`/`3.85` (~`8.3`/`7.7` per true dof [V2.1 m2]); ρ_i max `0.992`/`0.994` [V1]; 14/11 method-tension bins [V2.1 M1][V3 wp2]; no Born product; response-MC fluctuation untested (stated out of scope [V3]) | split (different gaps) |

### 3.2 Per-criterion evidence notes

**C1 — Unbiasedness.** On the same 1000 data-scale toys, delivered biases are small for both: IBU `0.0707%`/`0.0678%`, TUnfold `0.167%`/`0.226%` median |rel| ([V3] `mc_scaled` `bias_delivered_rel_median_abs`, `ibu_bias_delivered_rel_median_abs`). In MC-scale self-closure the ordering is the same (IBU coherent-prior `0.0150%`/`0.0161%` vs TUnfold delivered `0.093%`/`0.080%` — [V3]). The IBU djangoh-chain toy bias (`1.070%`/`1.170%`) is a prior-basin effect of the stored plain prior, mechanistically isolated (fixed point at k=1; coherent marginal restores the exact fixed point at every k — [V3] `ibu_kscan`); the **nominal** IBU chain uses the coherent prior ([REL-README]). TUnfold's operating point carries a closed-system regularization shrinkage of `-0.436%`/`-0.439%` (rapgap tau_sel, self-closure — [V2.1] M3); the v3 tau gate removed the larger djangoh-chain shrinkage (`-3.34%` → `-0.45%`) without changing the delivered vector ([V3] `wp3`). No unacceptable-bias finding against either candidate at data scale.

**C2 — Statistical uncertainty.** Budget `ibu_stat_rel` medians `3.5902%`/`3.8330%` ([BUDGET], re-derived; identical values recorded in [V4] `stat_decomposition.budget_ibu_stat_rel_median`). TUnfold: v4 data-stat medians `5.7290%`/`6.3451%`; total stat including the response-mcstat piece `5.9747%`/`6.4894%` ([V4]). Like-for-like: same-denominator yield ratio data-stat/budget-ibu_stat `1.451`/`1.448` ([V4] `same_denominator_yield_comparison`); per-bin same-bin ratio median `1.519`/`1.565` ([V2.1] M4); toy-dispersion ratio medians `2.288`/`2.354` ([V3] `mc_scaled`, ratio of medians across bins — not like-for-like). The TUnfold stat was toy-validated: toy sigma vs reported stat `0.975`–`1.021` median across schemes ([V3]); the v4 expected covariance passed a 500-toy spotcheck at ratios `0.980`–`0.996` ([V4]). **IBU is the tighter estimator by ~1.45–1.6× like-for-like.**

**C3 — Systematic budget.** IBU 15-component budget total medians `8.1864%`/`8.4079%` ([BUDGET] `total_with_s2prime15_rel`, re-derived; = [CURREL] line 19). Component medians (h+/h−): `elec_energy` `4.5590`/`4.8422%`, `ibu_stat` `3.5902`/`3.8330%`, `lumi` `2.7000`/`2.7000%`, `phi_h_gauss` `1.0494`/`1.0568%`, `ibu_model` `1.3623`/`1.4271%`, `c_rad_gen` `0.5844`/`0.5804%`, `ibu_reg` `0.5767`/`0.5669%`, `c_rad_stat` `0.4272`/`0.4429%`, `elec_theta` `0.5091`/`0.5601%`, `tracking_eff_m0p5` `0.4910`/`0.4929%`, `hfs_jes` `0.3624`/`0.3918%`, `fakecol_mcstat` `0.3443`/`0.3507%`, `hfs_theta` `0.2965`/`0.3120%`, `hfs_rces` `0.2595`/`0.2794%`, `detint_removal` `0.0296`/`0.0282%` (all [BUDGET], re-derived). TUnfold v4 `total_full` medians `9.1630%`/`10.1001%` (ratio vs budget `1.160`/`1.174` — [V4]). **Assessor arithmetic (derived, inputs cited):** if the IBU budget's open response-mcstat piece (medians `1.4769%`/`1.5752%`, [V4]) were added in quadrature to the IBU totals, the result would be `sqrt(8.1864² + 1.4769²) = 8.319%` (h+) and `sqrt(8.4079² + 1.5752²) = 8.554%` (h−) — still below the TUnfold totals. **IBU has the smaller total budget even after closing its known gap.**

**C4 — Negative-bin handling.** IBU: 0 negative nominal bins, both charges (re-derived from [BUDGET]; positivity by construction of the iterative map). TUnfold: no acceptable tau yields a non-negative solution (step-1 gate EMPTY in all four cases; negative-bin count tau-flat at `14/15/11/10` over the plateau; only the over-regularized region reaches 7–9 bins at `-30…-70%` integral bias, chi2/ndf ~1e3 — [V2] gates); `kEConstraintArea` does not enforce per-bin positivity ([V2] gates); the delivered vector is `max(x, 0)` with `14`/`11` clipped bins and a one-sided clip bias (integrated `+0.0282%`/`+0.0107%`) kept outside the quadrature total ([V2]). The clip is toy-validated (toys ran through it — [V3]) and forensically classified (7/6/1 and 7/3/1 verdicts, no f_i pathology — [V3] wp2, [V2.1] M6). **IBU does not need any of this machinery.**

**C5 — Estimator well-definedness.** IBU: a closed-form iterative map with a fixed prior basis, selected iteration k=4 (of 1–10), positivity by construction, regularization component defined as the k=3/k=5 variation ([REL]; [CLOSURE] line 93). TUnfold: the *solver* is a standard regularized least-squares solve; the *delivered estimator* is that solve followed by a post-hoc clip, with (i) the one-sided clip bias reported separately rather than in quadrature ([V2]), (ii) the stat covariance being the structurally **unclipped** linear covariance of the unclipped solution (documented as conservative — [V2.1] m1), and (iii) no positivity-enforcing constraint available in the ROOT 6.38.04 API used ([V2] gates). The empirical behavior of the full solve+clip pipeline is toy-validated ([V3]); what has no on-disk counterpart is a formulation in which positivity is enforced inside the solve (a constrained re-solve), so that the delivered vector and its covariance come from one well-defined estimation procedure. **On-disk record status of the external review of v2:** verdict SOUND-WITH-CONDITIONS with conditions M1–M6 ([V2.1]); the v2.1 open item "toy validation of the TUnfold stat (and of the clip)" was discharged by v3 ([V3] `validation` block: "Poisson toys through the full production chain … AND through the non-negativity clip"); the delivered vector remains the post-hoc clip in every subsequent version (bit-identity assertions in [V3], [V4], [V5]). The round-1 review text itself is **not on disk** (see Sec. 8), so conditions beyond the addendum's M1–M6 list cannot be cited verbatim.

**C6 — Coverage.** Same 1000 data-scale toys, each method with its own authoritative stat band: TUnfold stat-band coverage medians `0.696`/`0.695`; with the v2.1-m2-corrected total, `0.72`/`0.731` ([V3] `mc_scaled`; definitions in the v3 aggregation script, lines 214–233: per-toy clipped solution ± per-toy stat; corrected total; IBU band = release sidecar `toy_stat_unc`). IBU sidecar-represented coverage median `0.662`/`0.662` (min `0.013` (h+, bin 179) / `0.366` (h−, bin 235) — [V3]; worst bins re-derived in [V5] `independent_rechecks`). Both methods under-cover at the ~68% level on the stat band; TUnfold's corrected-total coverage is closer to nominal. **Moderate edge to TUnfold**; note neither reaches nominal coverage and the MC-scale `self_*` coverages must not be quoted as data-scale ([V3] `wp1_headline_note`).

**C7 — Validation depth.** IBU release-level: 1000-toy sidecar with pre-registered G5 closure (pos `0` bins |z|>3, z_absmax `2.9636`; neg `4` bins, z_absmax `3.7010`, exactly at the pre-registered ≤4 edge, criterion not weakened; n|z|>4 = 0; integrated |z| `0.9862`/`0.8597` — [SIDECAR]); gate suites at each promotion (S2′: G1–G13 13/13; cone: 18/18 — [CURREL]). TUnfold method-level: v1 deterministic closures → v2 non-negativity + constraint scan → v2.1 critic-condition addendum → v3 1000-toy through-clip validation + clipped-bin forensics + tau gate → v4 covariance-in-solve + 500-toy spotcheck → v5 global consistency vs IBU. **The TUnfold chain is the more extensively method-validated of the two as a method** (precisely because it was the challenger); the IBU release has the deeper release-level validation and promotion infrastructure.

**C8 — Independent-implementation cross-check.** IBU: RooUnfoldBayes `3.31e-15`/`3.66e-15` max rel diff ([CURREL] line 34); `2.6e-15` earlier record ([CLOSURE] lines 93, 119); in-study reproductions at `1.7e-15` level in both v1 and v2 ([V1], [V2] gates). TUnfold: **no second implementation exists on disk**; all v1–v5 results come from the same native ROOT `TUnfoldDensity` code path, cross-checked internally (fold-operator vs `GetFoldedOutput` gate ≤ `8.3e-16` — [V2]; two chi2 implementations identical — [V2.1] M5; linearization vs exact shifted solves at 1e-15 — [V4]) but not against an independent unfolding package.

**C9 — Infrastructure integration cost.** IBU: the full triad is already promoted **on the current cone basis** (RAW, Born, shifted-legs budget, sidecar; gate suite 18/18 PASS; install records — [CURREL] lines 15–17). TUnfold: nothing exists beyond the no-cone S2′ diagnostic studies; a nominal TUnfold would need the rebuild bill of Sec. 4.2. **Asymmetric by construction.**

**C10 — Residual known gaps (different for the two).** IBU: response body/miss MC-stat covariance unpropagated in the budget (open ledger item; quantified at `1.4769%`/`1.5752%` median when carried in-solve — [V4]); stat-band coverage `0.662` ([V3]); the historical G5 neg-leg edge watch ([SIDECAR]). TUnfold: refold chi2/ndf `4.178`/`3.849` (per true dof ~`8.32`/`7.65` — [V2.1] m2); ρ_i max `0.992`/`0.994` with median `0.487`/`0.498` ([V1]); `14`/`11` method-tension bins with documented but non-enveloped IBU disagreement ([V2.1] M1; [V3] wp2); no Born-level product and no response-MC fluctuation test (stated out of scope — [V3] `validation.fluctuation`); the djangoh response-side model diagnostic (~2× the budget model component — [V4] report-only) has no folded counterpart.

---

## 4. Risk analysis

### 4.1 Option A — keep IBU/BBB nominal, TUnfold as validated cross-check

**Risks accepted:**

1. **Response-MC-stat gap stays open in the budget.** Size is quantified (medians `1.4769%`/`1.5752%` — [V4]); assessor quadrature arithmetic shows closing it would raise totals to `8.319%`/`8.554%`, still below the TUnfold totals (Sec. 3.2 C3). Residual risk: small and quantified, but it must be stated in the note rather than left implicit.
2. **Coverage statement.** The sidecar-represented IBU coverage is `0.662` ([V3]); a note-level statement about under-coverage on the stat band is needed if per-bin intervals are published.
3. **Method-tension bins remain documented, not enveloped.** The `14`/`11` clipped bins are method-tension bins ([V2.1] M1 caveat: never quote the per-bin median `2.994%`/`2.890%` without the max `3.163`/`3.952`). Under Option A these stay as a cross-check observation.
4. **Second-method dependency.** The cross-check value depends on the TUnfold chain remaining reproducible; it is currently pinned to a superseded basis (Sec. 0). A future cone-basis regeneration is optional, not required for the decision.
5. **Critic-provenance gap.** The round-1 review text of v2 is not on disk (Sec. 8); closing P-METHOD under Option A should archive whatever review record exists elsewhere.

**What Option A does not risk:** no estimator-definition exposure, no budget re-derivation, no Born/sidecar/tracking/phi_h/detint re-derivation, no cone-basis rebuild, no change to any released number (integrated method shift `-0.0829%`/`-0.0765%` — [V2] — is not adopted).

### 4.2 Option B — switch nominal to native TUnfold

**Risks accepted:**

1. **Estimator definition.** The delivered estimator remains solve + post-hoc clip with a one-sided bias term and a structurally unclipped covariance ([V2], [V2.1] m1); no positivity-enforcing solve exists in the API used ([V2] gates). Making this nominal without a constrained re-solve (or an equivalent formalization) puts a post-hoc construction at the center of the publication. This is the largest single risk: it is a definitional, not a numerical, issue, and it is not dischargeable by re-running the existing chain.
2. **Precision loss.** Stat ×1.45–1.57 like-for-like ([V4]); total budget ×1.16/×1.17 ([V4]).
3. **Refold quality.** chi2/ndf `4.18`/`3.85` (≈`8.3`/`7.7` per true dof — [V2.1] m2) remains an unexplained-structure observation a reviewer can attack.
4. **No independent implementation** for the new nominal ([V3]–[V5] are all one code path).
5. **No physics payoff at the integrated level.** The delivered vector differs from IBU by `-0.0829%`/`-0.0765%` integrated ([V2]) — smaller than every budget component median except `detint_removal_rel` (`0.0296%`/`0.0282%` — [BUDGET]) and ~99×/110× below the totals; v5 finds no global tension (chi2/ndf `0.456`/`0.459`, p ≈ 1 — [V5]).
6. **Re-derivation bill (all items required, none currently exist on the cone basis):**
   - constrained re-solve or equivalent formal estimator definition (see 1);
   - cone-basis TUnfold chain: response inputs, tau re-selection, clip census re-run (v1+v2 equivalents);
   - full budget rebuild on the cone basis per the v4 prototype (8 in-solve sources + 7 post-hoc; v4 is a no-cone prototype only — [V4]);
   - Born chain (no TUnfold Born product exists; `sigma_Born = C_rad × Unfold(data; R)` would need the TUnfold Unfold leg — [CURREL] line 22 definition);
   - stat-sidecar convention decision (the IBU 1000-toy sidecar protocol is IBU-specific; TUnfold stat comes from `GetEmatrixTotal`/v4 decomposition — [V4]);
   - tracking ensemble re-derivation through the TUnfold operator (v4 did it in-solve on the no-cone basis — [V4]);
   - phi_h / detint_removal / c_rad component treatments (currently post-hoc carried; phi_h has no shifted-basis response artifact — [V4]);
   - note and critic loop for the new nominal, plus re-validation analogues of the release gate suites.
7. **Schedule/consistency risk.** Every currently promoted product (RAW, Born, budget, sidecar — cone basis) is IBU-based; a switch invalidates the promotion state built 2026-10-10 → 2026-10-14.

**What Option B buys:** in-solve response-MC-stat propagation (already quantified, small — Sec. 3.2 C3), better corrected-total coverage (`0.72`/`0.731` vs `0.662` — [V3]), and a formally prior-free estimator. None of these changes a released number materially.

### 4.3 Option C — dual publication

**Risks accepted:** everything in 4.2 (the bill must be paid to publishable standard for the second method too, or the second method is publishable only as a cross-check — at which point it is Option A); permanent dual maintenance across future basis changes; mixed-message risk if the two methods' per-bin tension bins (`14`/`11`, max ratio `3.163`/`3.952` — [V2]) appear in print without the forensics context ([V3] wp2); and `P-METHOD` remains formally open. Dual publication of *both central vectors* is not supported by the evidence basis anyway: only one of the two methods has a Born chain, a budget, and a promoted basis.

---

## 5. Recommendation

**Recommendation: Option A — keep IBU/BBB as the nominal method, retain the TUnfold v1–v5 chain as the validated second-method cross-check, and close `P-METHOD` with that decision recorded.**

**Evidence drivers (all on-disk):**

1. **Switching buys no physics change.** Integrated method shift `-0.0829%` (h+) / `-0.0765%` (h−) ([V2] `shift_pct_vs_ibu_v2`) against total budget medians `8.1864%`/`8.4079%` ([BUDGET]); v5 global consistency finds no tension (chi2/ndf `0.456`/`0.459`, pull RMS `0.675`/`0.676`, p ≈ 1 — [V5]).
2. **The challenger's delivered estimator is a post-hoc construction.** No acceptable tau gives non-negativity ([V2] gates); `kEConstraintArea` does not enforce per-bin positivity ([V2] gates); the delivered vector is `max(x, 0)` with a one-sided clip bias outside quadrature and a structurally unclipped covariance ([V2], [V2.1] m1). IBU is positive by construction with 0 negative bins ([BUDGET], re-derived).
3. **The challenger is less precise and costs more budget.** Stat ×1.45–1.57 like-for-like ([V4]); total_full `9.1630%`/`10.1001%` vs `8.1864%`/`8.4079%` ([V4] vs [BUDGET]); even after hypothetically closing IBU's response-MC-stat gap the IBU totals (`8.319%`/`8.554%`, assessor arithmetic from [BUDGET]+[V4]) stay below the TUnfold totals.
4. **Asymmetric validation and infrastructure.** IBU has an independent implementation (RooUnfoldBayes `3.31e-15`/`3.66e-15` — [CURREL] line 34) and a fully promoted cone-basis triad ([CURREL] lines 15–17); TUnfold has neither, and its refold chi2/ndf (`4.18`/`3.85`, ~`8.3`/`7.7` per true dof — [V2.1] m2) plus ρ_i max `0.992`/`0.994` ([V1]) would each need note-level treatment under a switch.
5. **The points in TUnfold's favor are real but secondary:** in-solve response-MC-stat (quantified small — [V4]), better corrected-total coverage (`0.72`/`0.731` vs `0.662` — [V3]), and the extensive v1–v5 method validation itself — which is exactly what makes it valuable **as the cross-check**.

This recommendation is a method-level judgement on the no-cone S2′ evidence base; per Sec. 0 it transfers to the cone basis, and nothing in it weakens the requirement to keep the three-basis quote rule ([CURREL] line 18).

---

## 6. What each option leaves open

**Option A leaves open:**
- the response body/miss MC-stat budget gap (quantified `1.4769%`/`1.5752%` median — [V4]; open ledger item — [CURREL]): either fold a derived component into a future budget revision or state it as a bounded open item;
- a note-level coverage statement for the `0.662` sidecar-represented stat-band coverage ([V3]) and the historical G5 neg-leg edge watch ([SIDECAR]);
- the disposition of the `14`/`11` method-tension bins in any per-bin publication (cross-check observation; [V2.1] M1, [V3] wp2);
- optional future work: a cone-basis TUnfold cross-check regeneration, and archiving the round-1 review text (Sec. 8);
- `P-PLOTS`, `P-ALPHA`, `P-CRAD-LUMI` and the other carried blockers are untouched by this decision ([STATE] carried-blockers list).

**Option B leaves open (in addition to everything above):** the constrained re-solve / formal estimator definition; the cone-basis TUnfold chain; the full budget rebuild; the Born chain; the stat-sidecar convention; the tracking ensemble re-derivation; phi_h/detint/c_rad component treatments; the note/critic loop; and an independent-implementation cross-check for the new nominal.

**Option C leaves open:** both lists simultaneously, plus the formal `P-METHOD` closure.

---

## 7. Artifact citation legend (absolute paths)

| Key | Artifact |
|---|---|
| [REL] | `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/summary_totals.json` |
| [REL-README] | `.../bbb_ibu_detints2prime_..._2026-10-09/README.md` |
| [SIDECAR] | `.../bbb_ibu_detints2prime_..._2026-10-09/stat_sidecar_1000toy/README.md` |
| [BUDGET] | `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_detints2prime_15comp_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/total_uncertainty_summary_s2prime_variant_15comp.csv` (medians re-derived 2026-10-14 for this dossier; totals cross-checked against [CURREL] line 19 and [V4] `budget_rel_median`) |
| [V1] | `H1ChargedSidis_HighQ2/output_v6/studies/tunfold_s2prime_z005_20261012/unfolded_z005_summary.json` |
| [V2] | `H1ChargedSidis_HighQ2/output_v6/studies/tunfold_s2prime_z005_v2_nonneg_20261012/unfolded_z005_summary.json` |
| [V2.1] | `.../tunfold_s2prime_z005_v2_nonneg_20261012/v2_1_addendum/README_v2_1_addendum.md` |
| [V3] | `H1ChargedSidis_HighQ2/output_v6/studies/tunfold_s2prime_z005_v3_toyvalidation_20261013/v3_study_summary.json` (coverage definitions: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_s2prime_z005_v3_20261013/run_tunfold_s2prime_v3_aggregate.py` lines 207–240) |
| [V4] | `H1ChargedSidis_HighQ2/output_v6/studies/tunfold_s2prime_z005_v4_syssolve_20261013/v4_syssolve_summary.json` |
| [V5] | `H1ChargedSidis_HighQ2/output_v6/studies/tunfold_s2prime_z005_v5_consistency_20261014/v5_consistency_summary.json` |
| [CURREL] | `sidis_highQ2/docs/status/current-release.md` (line numbers cited in text) |
| [TUSTAT] | `sidis_highQ2/docs/status/unfolding-tunfold.md` |
| [STATE] | `sidis_highQ2/STATE.md` (cone promotion record; carried-blockers lists) |
| [CLOSURE] | `sidis_highQ2/docs/unfolding/UNFOLDING_CLOSURE_PLAN_2026-09-30.md` |

All paths are relative to the workspace root `/data/dust/group/h1/klesthen/BaryonAsym/`.

---

## 8. Evidence gaps (not closable from on-disk artifacts)

1. **The round-1 external review text of v2 is not on disk.** The only on-disk record of the review is the v2.1 addendum ([V2.1]), which states the verdict (SOUND-WITH-CONDITIONS) and enumerates the conditions it responded to (M1–M6 + minors) and its open items. `docs/critiques/` contains no TUnfold-v2 review file (verified 2026-10-14 by listing and content search). Any characterization of the review beyond the addendum's own list cannot be cited from disk and is therefore not asserted in this dossier.
2. **No TUnfold cone-basis chain exists** (all v1–v5 on the no-cone S2′ variant response — [V1]–[V5] basis blocks), so no cone-basis number for the challenger can be quoted; the transfer argument in Sec. 0 is structural, not measured.
3. **No independent TUnfold implementation** exists on disk (Sec. 3.2 C8); the challenger's implementation-equivalence evidence is internal-gates only.
4. **Response-MC fluctuation through the TUnfold solve is untested** (explicitly out of scope in [V3] `validation.fluctuation`); the v4 in-solve response-mcstat is a linear propagation, toy-checked only against its own linear model ([V4] `toy_spotcheck`).
5. The v3 `mc_scaled` toys fix the response/f_i/miss/prior ([V3]); **neither** method has a data-scale toy test with fluctuated response for bias/coverage. For IBU the same limitation applies to the sidecar (fixed variant response/prior — [SIDECAR]).
