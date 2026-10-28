# Production-gate QA — P2 campaign 1 S2′ redo, eElecEn DOWN leg (`elecE_5wheel_m0p5pct`, five LAr wheels −0.5%) — 2026-10-11

- **Scope:** PRODUCTION-GATE QA of ONE side (down) of the first paired detector-leg
  campaign, on the S2′ detint-fake basis. This is NOT the paired symmetrisation
  battery — no noise classification, no side qualification, no symmetrisation
  (methodology `symmetrisation_tierB_covariance_methodology_2026-10-10.md` rev 1,
  production-gate items only; the battery runs when the campaign-2 up-side batch
  lands).
- **QA role:** independent (`h1-qa-specialist`). Guard assertions were NOT trusted —
  every load-bearing number below was re-derived read-only from the artifacts
  (CSV/JSON arithmetic; uproot read-only inspection of the Stage1/merged/
  diagnostics ROOTs; sha256/md5 recomputation). No files were modified outside this
  record; no production code was run.
- **Campaign workspace:**
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_down_s2prime_2026-10-11/`
  (Stage1 cluster 5486200, 8/8 exit 0; Stage2 cluster 5486230, exit 0; data vector
  REUSED from pre-S2′ cluster 5485777, sha256-verified).
- **Denominator (shift-ratio basis):** S2′ RAW release
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (IBU k=4, integrated h+ `11.446174` / h− `11.158554` nb — anchors reproduced to
  1e-9 from its `ibu/cross_sections.csv`; per-row `response_variant_id` uniformly
  `detint_fake_response_variant_S2prime_2026-10-09`).
- **Grounding note:** the `sidis-hq2-systematics` skill was not loadable in this
  runtime; the review is grounded in `STATE.md` (2026-10-11), the S2′ replan
  protocol `docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md` (§3, §4,
  §6.3, §6.5), the Tier-B methodology rev 1 (production-gate items), and the
  on-disk artifacts.

## VERDICT: PASS (production gate, down leg)

All six checks pass on the measurable evidence. One requested figure (the unfolded
pre-S2′-vs-S2′ basis-delta, check 6) is **NOT MEASURABLE**: the pre-S2′ campaign's
Stage2 was cancelled by the supervisor stop order before any unfold
(`paired_legs/elec_energy_down_2026-10-10/stage2/` is empty — verified), so no
pre-S2′-basis unfolded shift ratios exist anywhere. The decomposition and the
first-order expectation are reported instead (§6 below). No blockers. This PASS is
a production gate for the leg, NOT noise qualification (battery pending) and NOT
physics approval: the campaign release stays PROVISIONAL raw/no-bgsub, not
Born-level, S2′ observable basis, multiplicities blocked, no `C_rad`.

| # | Check | Verdict |
|---|---|---|
| 1 | Unfold integrity (closure/refold; IBU-vs-BBB pattern) | **PASS** (closure columns absent from saved products — driver's own refold metric recomputed read-only, comparable to nominal; per-bin IBU-vs-BBB pattern NOT degraded; integrated method-spread flag carried as context) |
| 2 | Shift-ratio sanity (490 ratios) | **PASS** (490/490 finite, 0 NaN/inf, 0 \|shift\|>100%, 0 >50%; 89 isolated outliers >5×MAD flagged — PARKED for the battery) |
| 3 | Direction/physics plausibility (gate-level) | **PASS** (coherent, predominantly one-signed; both charges positive; context flag on possible even content) |
| 4 | Response integrity (structure, f_i/G9, B+F, battery) | **PASS** (axes/binmap identical to nominal S2′; per-bin G9 identity EXACT job-for-job vs legacy controls; B+F conserved; §6.5 gate-4 battery items all pass) |
| 5 | Stamps/provenance (`meta_variant_id` + −0.005) | **PASS** (independently re-verified 8/8 ROOT metas via uproot + 8/8 QA JSONs + chain consistency) |
| 6 | Pre-S2′ same-shift control cross-check | **PASS (partial)** — job-for-job reco-migration alignment EXACT; unfolded basis-delta figure NOT MEASURABLE (pre-S2′ unfold cancelled by stop order; decomposition reported) |

**Overall: PASS.** Blockers: none. Parked/follow-ups: §"Parked items".

---

## 1. Unfold integrity — PASS

**Closure/refold.** The driver's own closure/refold columns are **not present** in
any saved product of this campaign: the coherent driver
(`run_bbb_ibu_release_v4_coherentprior.py`) writes reduced crosscheck JSONs
(`_run/bbb_ibu_crosscheck_*.json`: unfolded/err arrays + `response_aux` only), and
the `refold_chi2_ndf` / closure-assert code exists only in the `crosscheck` main()
path, which the Stage2 recipe does not invoke (verified from the driver source;
Stage2 stdout contains no refold banner). Recomputed the driver's own refold metric
(`refold_chi2_ndf`, coherent-prior form `A_ij = B_ij/P_j` with
`P = body+miss` of the same response; `d_sig = data·(1−f_i)`;
var `= data_w2·(1−f_i)²`) read-only from the saved merged response + data vector +
unfolded vectors:

| Quantity | Campaign (shifted S2′ leg) | Nominal S2′ release (same metric) |
|---|---|---|
| IBU k=4 refold χ²/ndf (h+) | 2183/483 = **4.52** | 2286/484 = 4.72 |
| IBU k=4 refold χ²/ndf (h−) | 2136/485 = **4.41** | 2073/482 = 4.30 |
| BBB refold χ²/ndf (h+ / h−) | 29.3 / 26.7 | 25.4 / 22.9 |
| Refold total − fake-subtracted data total | **0.0 (exact)**, both charges | 0.0 (exact), both charges |

The shifted leg does not degrade the refold behavior (IBU χ²/ndf sits between the
nominal's two charge values; BBB is worse by construction — bin-by-bin ignores
migration — and is within the same order). Total-flux closure is exact. The driver
metric is a sanity check, not a GOF p-value (its own docstring).

**IBU-vs-BBB per-bin agreement pattern** (relative difference (BBB−IBU)/IBU):

| | Campaign h+ | Nominal h+ | Campaign h− | Nominal h− |
|---|---|---|---|---|
| median | +3.57% | +3.87% | +3.05% | +3.21% |
| MAD | 12.31% | 13.64% | 12.68% | 12.96% |
| integrated BBB/IBU − 1 | **+0.2896%** | +0.0782% | **+0.3125%** | +0.1024% |

The per-bin pattern is comparable (medians and MADs marginally SMALLER on the
campaign) — not degraded. The integrated method spread is larger than the nominal's,
but matches the driver-pair precedent on shifted legs (2026-10-07
`detector_shifted_coherentprior/elecE_p1pct`: BBB−IBU integrated −0.309% / −0.310%);
the nominal's smaller value comes from the different (stub-driver) path. **Context
flag, not a gate failure:** on the shifted leg the integrated IBU−BBB spread (~0.3%)
exceeds the integrated shift signal (~0.2%) — relevant to the paired battery's
estimator bookkeeping, and one more reason the battery's measured floors decide.

Release audits: 490 rows per method, all finite, **0 negative cross sections**,
`acceptance_rapgap ∈ (0,1]` in all populated bins, `supported=True` 245/245 per
charge; crosscheck `unfolded_selected` == release CSV yields (max dev 0.0), k=4
selected out of iters 1–10 (so the k=3/k=5 regularization column is well-formed).

## 2. Shift-ratio sanity — PASS

490 rows (`comparison_vs_s2prime_nominal/shift_ratio_vs_s2prime_raw.csv`, 245+245;
stored `rel_shift` recomputes exactly from the yield columns, max dev 0.0):

- **Finite:** 490/490; 0 NaN/inf.
- **Pathological:** 0 bins with |shift| > 100%; 0 bins > 50%; **10 bins > 20%**
  (5 per charge): h+ max +22.69% (bin 174) / min −24.79% (bin 208); h− max +43.17%
  (bin 170) / min −20.15% (bin 172). All 10 sit in sparse rows (8 of 10 in the
  z≥0.7 row; exceptions h− bin 39 z0p4, h− bin 235 z3p4) with
  comparable-magnitude opposite-sign neighbors — sparse-bin behavior, carried raw
  per the frozen budget convention.
- **Smoothness:** adjacent-bin |Δshift| median 1.78% (p90 10.2%) against a per-bin
  shift MAD of ~1.1% — bin-level scatter dominates the ~0.2% median signal; the
  coherent structure appears at cell level (§3).
- **Isolated outliers (>5× MAD of kinematic neighbors):** neighbors = the
  (z±1,pT), (z,pT±1) grid neighbors within each (charge, (Q2,x) cell);
  flag if |shift − median(neighbors)| > 5×MAD(neighbors). **89 bins flagged
  (45 h+, 44 h−), PARKED for the paired-battery noise classification — no noise
  classification this round.** The criterion is dominated by flat-neighborhood
  statistical wobble (most flagged bins have sub-percent deviations against
  tiny neighbor MADs); the largest flagged excursions are h− bin 170 +43.2%,
  h− bin 239 +26.7%, h− bin 39 +24.5%, h+ bin 239 +21.5%, h− bin 172 −20.1%,
  h+ bin 244 −23.2%* (*not flagged — its neighborhood is itself dispersed).
  Several >20% bins are NOT flagged because their neighborhoods are also
  dispersed (h+ 174/208/209/244, h− 235/239) — locally wild sparse rows rather
  than isolated spikes.

## 3. Direction / physics plausibility (gate-level) — PASS

- **Per-charge integrated shift** (Σ campaign yield / Σ S2′ nominal yield − 1;
  nb-identical): **h+ +0.2035%** (11.469471 vs 11.446174 nb), **h− +0.2084%**
  (11.181812 vs 11.158554 nb). Both from the release `summary_totals.json`
  reproducing the CSV sums exactly.
- **Sign census:** h+ **141 positive / 104 negative / 0 zero** (57.6%);
  h− **145 / 100 / 0** (59.2%).
- **Coherence:** all **14/14** (charge, cell) median shifts positive (h+ cell
  medians +0.06%…+0.48%; h− +0.005%…+0.32%); adjacent-bin sign agreement 61.1%
  (h+) / 57.0% (h−) vs 50% under random signs; sign runs 96 / 106 vs ≈124 expected
  under random signs. The response is coherent and predominantly one-signed at
  cell/integrated level, not random bin-to-bin sign noise; the bin-level scatter is
  the expected statistical floor of a single shifted leg (classified only by the
  battery).
- **Context flag (no interpretation beyond bounds):** a −0.5% five-wheel DOWN
  shift produces a small POSITIVE integrated response, sub-linear in the
  amplitude. The legacy one-sided `elecE_p1pct` (+1% UP, pre-S2′ basis, same
  driver family) integrated **+0.094% / +0.100%** — the SAME sign as this DOWN
  leg. Same-sign up/down integrated responses are the signature the battery's
  SA-EVEN / SA-MAG machinery is built to resolve (hfs_theta precedent); the
  cross-amplitude, cross-basis comparison is weak evidence only, but the up-side
  campaign should be watched for it. Gate-level: coherent, plausible, PASS.

## 4. Response integrity — PASS

- **Structure:** merged shifted S2′ response
  `stage2/migration_inputs_v4_…_percharge.root` — `h_R_{pos,neg}_{rapgap,djangoh}`
  all (491, 246); rapgap-leg axis edges **identical** to the nominal S2′ variant
  response (`studies/detint_variant_s2prime_2026-10-09/merged/response_rapgap31_detint_s2prime.root`,
  sha256-anchored in the release metadata); `meta_binning_version` identical;
  campaign `binmap_v3.json` **deep-equal** to the nominal release `bin_map.json`
  (490→245, two children/gen bin, driver-gated). DJANGOH leg present in the merge
  (decision (a)) with 4/4 shards per generator; merge sums 4/4 files for every
  histogram.
- **f_i formed and only increased (G9, the protocol's HARD check for campaign 1):**
  per-reco-bin `f_i` is formed by the driver from the merged variant response
  (integrated rapgap leg: h+ **11.1824%** / h− **10.8042%** — ABOVE the
  pre-variant central 10.6153%/10.3124% and within a loose band of the
  nominal-variant 11.2403%/10.8737%). Campaign-scale per-bin identity vs the
  retained legacy same-shift controls, verified job-for-job with uproot over all
  16 (job, charge) pairs: `f_i(S2′) − f_i(legacy) == D_i/(B+F)` with max
  deviation **≤ 7.8e-16**, **zero** f_i-decrease violations, `D_i > 0` in
  355–450 of 490 reco bins per pair, and B+F row totals conserved to ≤5e-15
  relative. EXACT — PASS.
- **B+F conservation (carried from the Stage1 QAs):** per job per charge,
  `sumw_body + sumw_fake` identical legacy vs new (≤5e-15 rel); miss row
  **bit-identical** (0.0 rel, 16/16); fake-category partition closure
  (fc0+fc3+fc4+fc5 vs fake column, from the diagnostics npz/ROOT): max per-bin
  dev ≤1.4e-9, both generators, both charges.
- **§6.5 gate-4 battery items:** per-period QA 8/8 (inputs 61/121/61/121 RG +
  71/141/88/201 DJ = 865, `n_files_failed=0`, stamps); S2′ counters > 0
  (keeper-retained RG 22,357/18,924, DJ 26,693/23,057; rerouted RG 214,490/187,411,
  DJ 278,483/243,840; detint infill 490,168/638,938); pooled removable fractions
  **RG 0.906853** (parentage ref ≈0.9064; nominal-variant realized 0.90528) /
  **DJ 0.913036** (ref ≈0.9126; realized 0.91308) — inside the loose band;
  key collisions 4355 RG / 5741 DJ (nominal campaign 4375/5772, ≈1% of rerouted
  legs, adjudicated BENIGN — not gated on zero); negative-input/acceptance audit:
  data vector **0/980 negative** (`n_raw`, `sigma_raw`; sums 1,973,169.0 /
  1,906,433.0 h+/h−, exactly the merge-log fine totals), **0/490 negative cross
  sections**, `eff ∈ (0,1]` in populated bins.

## 5. Stamps / provenance — PASS (independently confirmed, not just guard-asserted)

- **Stage1 ROOTs (8/8), re-verified via uproot:** `meta_variant_id =
  detint_fake_response_variant_S2prime_2026-10-09` AND
  `meta_electron_energy_scale = "-0.005"` in every file.
- **Stage1 QA JSONs (8/8):** `variant_id` (top level and in `migration_summary`)
  + `shift.electron_energy_scale = −0.005` in every file.
- **Data vector reuse:** sha256 of the campaign copy == the pre-S2′ source ==
  the recorded value (`ffeedf09…a2ab6c3`; QA `01d6216a…cc4789e9`), 658 inputs /
  0 failed, −0.005 stamp, `bg_subtracted=false`, raw counts, 980 bins;
  `DATA_VECTOR_REUSE.json` present; the Stage2 script's sha256 guard passed
  (job proceeded) and the guard value matches my recomputation.
- **R10 steering guard:** source reviewed — it reads the ROOT TNamed metas (not
  just QA JSONs), checks the central object set, and exits 2 on any failure
  (fast-fail verified pre-launch); Stage2 log shows
  `[guard_s2prime] PASS: all 8 Stage1 products …`.
- **Release chain:** metadata `variant = paired_legs_elec_energy_down_s2prime_2026-10-11`,
  `electron_shift = elecE_5wheel_m0p5pct`, k=4, `L=351000.0 nb^-1`, prior basis
  `body+miss from this shifted response`; IBU/BBB CSV rows uniformly stamped
  `electron_shift = elecE_5wheel_m0p5pct` with the frozen caveat; crosscheck JSONs
  carry variant + shift; comparison rows carry the **three-part basis label**
  uniformly (prior: coherent `body+miss` from the campaign's shifted S2′ variant
  response; level: raw/no-bgsub, not Born-level, provisional; observable basis:
  S2′ detint-fake variant).
- **Denominator identity:** `compare_vs_s2prime_nominal.py` asserts the 490-row
  identity, the integrated anchors (11.446174/11.158554, 1e-6) and the per-row
  `response_variant_id` before dividing — all re-confirmed independently here; the
  superseded coherent-prior release is not referenced as denominator anywhere in
  the campaign chain.
- **Drivers:** campaign `drivers/` pair md5-identical to the pre-S2′ campaign-1
  copies AND to the accepted 2026-10-07 `elecE_p1pct` detector re-unfold sources
  (`DRIVER_COPIES.md5` matches my recomputation).
- **Known carried attribute (documented, not a defect):** the merged ROOT carries
  no `meta_variant_id` (the v4 merge does not propagate it; the pre-merge guard
  covers the Stage1 inputs, and release-level provenance is carried by the
  variant string + basis labels); the campaign driver computes the DJ-leg prior
  coherently per leg (protocol §4 item 5 — affects only the `model_unc`
  diagnostic column, not the rapgap-leg shift vector).

## 6. Pre-S2′ same-shift diff control (legacy cluster 5485776) — PASS (partial)

**Job-for-job reco-migration alignment: EXACT.** All 8 (mc, period) pairs,
legacy (`mig_shifted_v4_…`) vs redo (`mig_s2prime_…`), from the QA JSONs and
confirmed at per-bin level with uproot:

- Identical (shift- and reroute-invariant): input counts, `n_files_failed=0`,
  gen totals (`n_gen_total/in_ps/in_bin`), reco totals
  (`n_reco_total/ps_shifted/in_bin_shifted`), duplicate/conflicting key counters —
  the −0.005 shift content is the SAME.
- The ONLY differences are exactly the S2′ reroute, 16/16 (job, charge) pairs:
  `matched_legacy − matched_new == n_rerouted` (EXACT, count-level),
  `fake_new − fake_legacy == n_rerouted` (EXACT), B+F conserved (≤5e-15),
  miss row bit-identical, per-bin G9 identity exact (§4). Job-for-job alignment
  confirmed; the redo is the legacy fill plus the reroute, nothing else.

**Unfolded basis-delta figure: NOT MEASURABLE.** The requested per-charge
integrated difference between pre-S2′ and S2′ shift ratios cannot be measured:
the pre-S2′ campaign's Stage2 (merge + unfold + comparison) was **cancelled by
the supervisor stop order before submission** (2026-10-11; documented in
`paired_legs/elec_energy_down_2026-10-10/run_log_2026-10-11.md` §5; its `stage2/`
directory is empty — verified). No pre-S2′-basis unfolded campaign product exists
in any workspace (the only same-source precedent, `elecE_p1pct`, is a different
amplitude). Reported decomposition instead:

| Component (integrated) | h+ | h− |
|---|---|---|
| Measured S2′ down-leg shift ratio (this campaign) | **+0.2035%** | **+0.2084%** |
| Nominal-side basis change (S2′ vs pre-S2′ nominal IBU) | −0.3239% | −0.2844% |
| First-order expected basis-delta of the shift ratio (d_S2′ − d_pre) | ≈ 0 | ≈ 0 |

Basis-invariance of the shift ratio holds to first order because the S2′ reroute
applies to the shifted leg exactly as to the nominal (measured: pooled removable
fractions within +0.0016 (RG) / −0.0001 (DJ) of the nominal-variant realized
values; exact G9/B+F identities job-for-job); the residual second-order term is
`b_shift − b_nom`, estimated ≲0.01% — an ESTIMATE, not a measurement. A measured
figure would require re-running the cancelled pre-S2′ Stage2 against the
superseded basis (forbidden as a comparison partner without the observable-basis
flag; not needed — the battery runs entirely on the S2′ basis).

## Parked items (routed, not blocking this gate)

1. **89 flagged isolated outliers** (45 h+, 44 h−, §2 list) — parked for the
   paired battery's per-bin noise classification (`z±` on measured floors);
   not classified this round.
2. **Possible even content in `elec_energy`** (same-sign integrated responses of
   the +1% legacy UP leg and this −0.5% DOWN leg) — the campaign-2 up-side batch
   plus the battery's SA-EVEN/SA-MAG/SA-FLAT partition and `ρ̂_res` decide.
3. **Driver-emitted closure/refold columns absent** from the saved Stage2
   products (recomputed this round from saved inputs; the metric exists only in
   the unused crosscheck main() path). Recommendation: have the Stage2 chain emit
   `refold_chi2_ndf` (and the closure block) into `_run/` JSONs for campaigns 2–12
   — an artifact gap, closed this round by recomputation.
4. **Integrated IBU−BBB spread on the shifted leg (~0.3%)** vs the integrated
   shift (~0.2%) — driver-pair precedent scale (`elecE_p1pct` −0.31%); carry as
   context into the battery's estimator bookkeeping.
5. **Measured pre-S2′ basis-delta** — permanently unavailable (cancelled
   product); decomposition recorded above. No action.

## Next validation artifacts needed

1. Campaign-2 up-leg (`elecE_5wheel_p0p5pct`, S2′ basis, `elec_energy_up_*`)
   Stage1/Stage2 products + its own gate — then the paired symmetrisation
   battery (N=100 paired replicas, measured floors, `ρ̂_res`, census, placebo
   closure) per methodology §2.
2. (Recommended, non-blocking) refold summaries in future Stage2 outputs (item 3).

## Provenance register (primary artifacts examined)

- Campaign workspace `…/paired_legs/elec_energy_down_s2prime_2026-10-11/`:
  `README.md`, `run_log_2026-10-11.md`, `smoke/SMOKE_VERDICT.json` (via README),
  `steering/` (guard, comparison script, Stage2 script), `stage1/` (8 QA JSONs +
  8 ROOTs, data vector + reuse record), `stage2/` (merged response ROOT + binmap
  v3, diagnostics merges ×2 + manifests + npz, `release_coherentprior_s2prime/`
  IBU+BBB CSVs/JSONs, metadata, summary totals, `_run/` crosschecks ×4,
  comparison CSV/JSON), `logs/stage2/job.5486230.0.out`, `drivers/` (+ md5s).
- Legacy control `…/paired_legs/elec_energy_down_2026-10-10/`: `README.md`,
  `run_log_2026-10-11.md` (stop order §5), `stage1/per_file_migration/` (8 QA
  JSONs + 8 ROOTs), `stage2/` (empty — verified), `stage1/data_vector/` (sha256
  source).
- Nominal basis: S2′ RAW release `bbb_ibu_detints2prime_…_2026-10-09/`
  (metadata, IBU/BBB CSVs, bin_map.json), S2′ variant response
  `studies/detint_variant_s2prime_2026-10-09/merged/response_rapgap31_detint_s2prime.root`,
  central merged root (data vector leg), pre-S2′ coherent-prior release
  `bbb_ibu_coherentprior_…_2026-10-06/` (basis-change decomposition).
- Precedent: `detector_shifted_coherentprior_2026-10-07/elecE_p1pct/`
  (release summary, comparison JSON).
- Governing docs: `paired_legs_s2prime_protocol_2026-10-11.md`;
  `symmetrisation_tierB_covariance_methodology_2026-10-10.md` rev 1;
  `STATE.md` (2026-10-11); `detint_fake_response_variant_spec_2026-10-08.md`
  (via protocol).

**Quoting rule (every number above):** prior basis coherent `body+miss` from the
campaign's shifted S2′ variant response; level raw/no-bgsub, not Born-level,
provisional; observable basis S2′ detint-fake variant
(`detint_fake_response_variant_S2prime_2026-10-09`).
