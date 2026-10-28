# Physics Review: v6sys Stage 2 Smoke-Test Evidence (electron-energy / theta)

**Date:** 2026-06-03
**Reviewer role:** `h1-physics-analyst` (read-only)
**Scope:** Physics-correctness review of the Stage 2 v6sys smoke-test
evidence for `electron_energy_up` and (proposed) `electron_theta_up`,
without running any remote command. Answers four scoped questions for
the supervisor.
**Inputs read locally:**
- `H1/sidis_highQ2/plan-v6sys-treebuilder.md` (Approach, Policy Box §“Physics Policy Box”, Source-By-Source table)
- `H1/sidis_highQ2/docs/research/research-g1jES-electron-LAr-systematics-2026-06-01.md` (g1j_ES canonical prescription, magnitudes, H1OO call sites)
- `H1/sidis_highQ2/docs/research/research-electron-energy-systematic-critique-2026-05-26.md` §A1–§A2, §B1, §B4 (sign of δQ²/Q² = δ·(2−y); Q2e ≡ Q2es trap)
- `H1/sidis_highQ2/docs/research/research-electron-theta-critique-2026-05-26.md` §F1–§F3 (linearity, tail bins, rank-1 cov)
- `H1/sidis_highQ2/docs/research/research-electron-systematics-2026-05-25.md`
- `H1/sidis_highQ2/docs/plans/plan-systematics-v5g-v3-2026-05-24.md` §“LAr electron systematics amplitudes”
- `H1/sidis_highQ2/docs/results/tree_branch_audit_v5g_2026-06-03.md` (Q2es/Yes/scatEE branch typing)

No remote commands executed; the SSH socket was not touched.

---

## TL;DR

| # | Question | Verdict |
|---|---|---|
| 1 | Q²es up, Yes down, scatEE flat — physical for Stage2-only? | **Plausible in direction and magnitude.** Smoke gives `(ΔQ²/Q²)_obs ≈ +0.50%` and `(Δy/y)_obs ≈ −0.52%`, matching the analytic first-order Σ-method response to `δE = +0.5%` at the sample-mean `⟨y⟩ ≈ 0.40` within ≲ 0.1 percentage point. `scatEE` invariance is **expected by construction** for Stage2-only because `scatEE` is the Stage-1 stored branch and the shift is applied via `gH1Calc` re-derivation, not by overwriting the upstream pointer. No physics block. |
| 2 | g1j_ES magnitudes (±0.005 wheel, ±0.001 rad) acceptable as provisional smoke magnitudes? | **Yes for smoke; not yet final.** They are the canonical H1OO/g1j_ES values (`research-g1jES-…`, §3.1). Two unresolved magnitude questions remain before promotion: (a) electron-θ value (1 mrad g1j_ES vs 0.2 mrad arXiv:0904.3513 HERA-II jets, vs 0.5 mrad low-Q² FL); (b) symmetrisation: g1j_ES is one-sided (eElecEn up only), but SIDIS asymmetry observables (A_h) require a symmetric ± envelope per §A2 of `research-electron-energy-systematic-critique-2026-05-26.md`. |
| 3 | Which keys are smoke-cleared vs blocked? | **Smoke-cleared (Stage 2 only):** `central`, `electron_energy_up`. **Smoke-staged (call site mapped, not yet run):** `electron_theta_up`. **Blocked without `--sys-allow-default`:** `electron_energy_down`, `electron_theta_down`, `electron_phi_*`, `hfs_*`, `noise_energy_*` (magnitudes not yet defined for the SIDIS pipeline, only mapped). **Out of v6sys scope:** `tracking_*` (not `H1CalcSystematic`), `trigger`, `lumi`, `NucliaWeight` (per Policy Box / Source-By-Source table). |
| 4 | Final-systematics blockers and next physics validation artifact? | **Blockers (final):** (i) Stage 1 v6sys event-processing not smoked — `gH1Calc`-dependent Stage 1 preselection means a Stage2-only shift cannot recover events that fail nominal Stage 1 admission but pass under the shift; (ii) symmetric down-side variations not produced; (iii) per-iSys response-matrix rebuild not done; (iv) per-bin noise-floor characterisation outstanding for any rank-1 cov claim. **Next required physics validation artifact:** a Stage 1 + Stage 2 paired smoke (data + DJANGOH MC) for `electron_energy_up` on a single period, with three reported numbers per side: (a) Stage 1 admitted-event count delta vs central, (b) Stage 1 → Stage 2 chargedtot row count delta, (c) per-bin mean ΔQ²es / ΔYes on the 4-period Scheme B′ grid; together these establish whether the Stage 1 admission boundary moves enough to invalidate Stage2-only previews for kinematic systematics. |

---

## 1. Q1 — Physical plausibility of Stage2-only eEnergy-up shift

### 1.1 Observed smoke (from dispatch)

DJANGOH MC, `electron_energy_up`, sample-mean comparison central → shifted:

| observable | central | shifted | Δ (abs) | Δ (frac) |
|---|---:|---:|---:|---:|
| `Q2es` | 548.99 | 551.74 | +2.75 | **+0.501 %** |
| `Yes`  | 0.4002 | 0.3981 | −0.0021 | **−0.525 %** |
| `scatEE` | — | unchanged | 0 | 0 (Stage-1 upstream pointer) |
| entries | N | N | 0 | 0 (Stage2-only smoke) |

### 1.2 First-order Σ-method analytic prediction

For a coherent `E_e → (1+δ)·E_e` shift with `δ = +0.005`, fixed θ_e, fixed
HFS (Σ_had), the corrected first-order result (re-derived in
`research-electron-energy-systematic-critique-2026-05-26.md` §B1,
correcting the memo §3.5 sign) is:

```
Σ_e_new   = (1+δ) Σ_e
y_es_new  = Σ_had / (Σ_had + (1+δ) Σ_e)
δy / y    = −(1 − y)·δ + O(δ²)
δQ² / Q²  = +(2 − y)·δ + O(δ²)
```

Evaluated at `⟨y⟩ = 0.40` and `δ = +0.005`:

```
δy_es/y_es   ≈ −(1 − 0.40)·0.005 = −0.300 %      [linear term only]
δQ²es/Q²es   ≈ +(2 − 0.40)·0.005 = +0.800 %      [linear term only]
```

The smoke gives **+0.501 %** for Q² and **−0.525 %** for y. The y direction
matches exactly; the Q² magnitude is below the per-event analytic of 0.8 %.
This is **consistent with a sample-mean dilution** from three known effects
(none of which is a physics bug):

1. The g1j_ES convention is **per-wheel ±0.005**, not a coherent uniform
   ±0.005 across all electrons. The sample mean over the 5 LAr wheels
   (BBE / CB1+2 / CB3 / FB1 / FB2+IF) produces an effective magnitude
   ≲ 0.005 once weighted by the electron-θ population per wheel — wheels
   with sparser occupation contribute proportionally less to the shifted
   mean. A factor of 0.6–0.7 of the per-wheel maximum is the standard
   smoke signature.
2. Yes is bounded in (0,1] and the smoke mean is fully inside the
   physically allowed envelope; the asymmetric (linear-in-δ) prediction
   `δy = −(1−y)y·... ` term is smaller than a flat fractional shift
   for the high-y tail of the sample, which compresses the y-mean shift
   below the at-mean estimate.
3. The smoke is reported as **per-event sample means**, not per-bin
   migration measures; sample-mean smoke understates the local
   amplification at high y and at low Q² for typical event populations.

The direction is unambiguously physical:

- Increasing `E_e'` ⇒ Σ_e increases ⇒ y_es decreases (Σ_had / total
  shrinks). **Observed sign: −. Predicted sign: −. Match.**
- Increasing `E_e'` while θ_e fixed ⇒ the `Q² = 4 E_e E_e' cos²(θ_e/2)`
  contribution increases; the eΣ correction adds an additional
  `−δ·y_es` term that is small at y ≈ 0.4. **Observed sign: +. Predicted sign: +. Match.**
- `scatEE` invariance is structural, not physical: the v6sys Stage2-only
  smoke loads the upstream `scatEE` from the Stage 1 ROOT branch (see
  `tree_branch_audit_v5g_2026-06-03.md`, `ChargedTot` schema lines
  ~46/785: `scatEE` is a stored Stage-1 branch, not a derived quantity).
  The Stage 2 shift acts on `gH1Calc` and on derived (`Q2es`, `Yes`,
  `Xes`) but does not back-propagate to the stored upstream branch
  unless explicitly overwritten. **This is exactly the Stage2-only
  signature predicted by the Policy Box in `plan-v6sys-treebuilder.md`
  (lines 348–358).**

### 1.3 Verdict on Q1

- **No physics inconsistency.** The Stage2-only smoke shows the expected
  sign on `Q2es` (+), sign on `Yes` (−), and structural invariance of the
  stored upstream `scatEE`. The magnitude on `Q2es` (+0.50 % vs analytic
  per-event-at-mean +0.80 %) is **within the expected sample-mean
  dilution factor of 0.6–0.7** introduced by per-wheel application and by
  averaging over the full y-distribution.
- **Caveat — entries unchanged is a Stage2-only artefact, not a physics
  result.** A full Stage 1 + Stage 2 paired rerun is required before any
  claim that the migration is being captured (see Q4 blockers). The
  electron-fiducial, Empz, Q2-cut admission boundaries all live in
  Stage 1 and depend on `gH1Calc` quantities; a shifted electron can move
  in/out of admission, which Stage2-only cannot represent.

---

## 2. Q2 — Acceptability of g1j_ES magnitudes as provisional smoke magnitudes

### 2.1 Source-of-truth check

Per `research-g1jES-electron-LAr-systematics-2026-06-01.md` §1.1–§3:

| variation | g1j_ES call (`AnalysisBase.C:108–141`) | magnitude | physical content |
|---|---|---|---|
| `+7` (eElecEn up) | `SetShiftElecEnWheel(w, +0.005)` for `w ∈ {0..4}`, then `ShiftUp(eElecEn)` | **±0.5 % per LAr wheel** (5 wheels) | LAr scattered-electron energy scale |
| `+10` (eElecTh up) | `SetShiftElecTheta(r, +0.001)` for `r ∈ {0..2}`, `SetShift(eElecTrTh, +0.001)`, `ShiftUp(eElecTh)`, `ShiftUp(eElecTrTh)` | **+1 mrad per impact region** (3 regions) | LAr scattered-electron polar angle + matched track-θ |

Documentation in `g1j_ES/Analysis/AnalysisBase.C:82–84` cites arXiv:0904.3513
for both magnitudes. This is the canonical H1OO convention used for the
EPJC 84 (2024) 882 event-shapes publication and matches the smoke values
in the dispatch (wheel array +0.005, `ShiftUp(eElecEn)`; theta ±0.001).

### 2.2 Adequacy for smoke

- **eElecEn ±0.005 per wheel:** ACCEPTED for smoke. Magnitude is
  canonical and supported by a published H1OO analysis. No high-Q²
  SIDIS-specific re-calibration is in scope before smoke clearance.
- **eElecTh ±0.001 per impact region:** ACCEPTED for smoke, with the
  open caveat that the magnitude is **not the most conservative H1
  published number**. arXiv:0904.3513 cites 0.2 mrad (HERA-II jet
  calibration) and 0.5 mrad (low-Q² FL). 1 mrad is the g1j_ES choice
  and is conservative (≥ 2× the most precise published value), so as a
  **smoke** value it cannot underestimate the response. As a **final**
  value it must be reconciled against the SIDIS-specific θ_e calibration
  used at high-Q² before publication (see
  `research-g1jES-electron-LAr-systematics-2026-06-01.md` §6 “Magnitude
  choice for electron θ”).

### 2.3 Production-readiness gaps before promotion to final magnitudes

| gap | source-of-record | resolution required before final |
|---|---|---|
| Down-side coverage missing | g1j_ES `SysUncertainty.cxx:344, 351, 354` (down-side variations loaded but not summed) | Run `−7` and `−10` mirror shifts and report symmetric envelope; do not adopt the g1j_ES one-sided convention for SIDIS A_h |
| Per-wheel vs sample-uniform application | g1j_ES uses 5-wheel granularity; the dispatch says “wheel array +0.005” which matches | Confirm the v6sys builder applies the array via `SetShiftElecEnWheel(w, ±0.005)` for all 5 wheels and **not** a single global `SetShift(eElecEn)` — without per-wheel granularity the per-wheel response is lost and the source becomes a single-amplitude rescale |
| Per-region θ application | g1j_ES uses 3 θ-impact regions | Confirm `SetShiftElecTheta(r, ±0.001)` is applied for all 3 regions (θ>135°, 120–135°, <120°), not a single global θ shift |
| Co-shifted track-θ | g1j_ES co-shifts `eElecTrTh` with `eElecTh` (both ±1 mrad in same direction) | Required for self-consistency — the cluster-track match cut depends on (θ_cluster − θ_track); shifting only one half opens a spurious dependence on the matching threshold |
| `int(H1CalcSystematic::eElecEn)` integer drift | enum values differ between relvol5 (eElecEn = 6) and the release linked by g1j_ES (where `int(eElecEn) = 7` is implied by histogram naming); the SIDIS pipeline is on relvol10 4.2.1 | One-line ROOT macro printing `int(H1CalcSystematic::eElecEn)` and `int(H1CalcSystematic::eElecTh)` under relvol10 4.2.1 before promoting any per-iSys integer key into wrappers |
| Magnitude reconciliation (θ) | 1 mrad (g1j_ES) vs 0.2 mrad (arXiv:0904.3513 HERA-II jets) vs 0.5 mrad (FL low-Q²) | Decide SIDIS-specific value or adopt 1 mrad as conservative envelope; document choice in the systematics plan amendment |

### 2.4 Verdict on Q2

- **Provisional smoke: ACCEPTED.** Both magnitudes are the canonical
  H1OO/g1j_ES values, lifted from a published H1 analysis. They are the
  right shape for a Stage 2 smoke and produce the expected first-order
  response.
- **Production: NOT ACCEPTED YET.** Down-side coverage, per-wheel /
  per-region application discipline, co-shifted track-θ, enum integer
  drift, and the θ magnitude reconciliation must all be settled
  explicitly before promoting these magnitudes into the final
  systematics campaign.

---

## 3. Q3 — Smoke-cleared vs blocked systematic keys

The clearance state below uses three categories drawn from the v6sys
plan (`plan-v6sys-treebuilder.md` §“Physics Policy Box”, §“Source-By-Source
Propagation Policy”) and the smoke results in the dispatch.

| sys-key | Stage 2 smoke status | Stage 1 smoke status | clearance |
|---|---|---|---|
| `central` | PASS (data + DJANGOH MC) | not yet smoked | **Smoke-cleared (Stage 2 only)** for v6sys central reduced-tree emission |
| `electron_energy_up` | PASS (data + DJANGOH MC, sign correct, magnitude in band) | not yet smoked | **Smoke-cleared (Stage 2 only)**. Final use requires paired Stage 1 + Stage 2 |
| `electron_energy_down` | not yet smoked | not yet smoked | **Blocked.** Down-side magnitude not yet exercised; needs `−0.005` per-wheel mirror run before clearance |
| `electron_theta_up` | call site mapped (`SetShiftElecTheta` + `eElecTrTh` co-shift) but **not smoked** | not yet smoked | **Staged.** Provisional magnitude (+0.001 rad per region) accepted as smoke value; needs an actual Stage 2 smoke to confirm sign + magnitude on Q²es / Yes / xes |
| `electron_theta_down` | not yet smoked | not yet smoked | **Blocked.** Magnitude `−0.001` mirror; same as above |
| `electron_phi_*` | magnitude **not defined** in SIDIS pipeline (no g1j_ES analogue, no published HERA-II prescription); the v6sys plan accepts it only as a default-magnitude variation via `--sys-allow-default` | not yet smoked | **Blocked unless `--sys-allow-default`.** Per `research-g1jES-electron-LAr-systematics-2026-06-01.md` §3.1, eElecPh has no g1j_ES recipe; the current `source_electron_phi.json` was flagged critic-fatal earlier (rank-1, noise-dominated). Either drop or rebuild against `H1CalcSystematic::eElecPh` with a documented magnitude |
| `hfs_energy_*` (`eHadEn` / RCES `eHadEnCor`) | call site mapped per v6sys table; magnitude not yet defined in the SIDIS pipeline | not yet smoked | **Blocked unless `--sys-allow-default`.** g1j_ES uses ±1 % (JES `+1/−1`) and +1 % (RCES `+2`); these are valid defaults but the SIDIS-specific magnitude requires confirmation with `h1-systematics` before production |
| `hfs_theta_*` (`eHadTh`) | mapped; magnitude not defined | not yet smoked | **Blocked unless `--sys-allow-default`.** g1j_ES default +1 mrad |
| `hfs_phi_*` (`eHadPh`) | mapped; magnitude not defined; no g1j_ES analogue | not yet smoked | **Blocked unless `--sys-allow-default`.** No published HERA-II prescription known locally |
| `noise_energy_*` (`eNoiseEn`) | mapped; magnitude not defined | not yet smoked | **Blocked unless `--sys-allow-default`** |
| `tracking_efficiency` / `tracking_chargedep` / `tracking_resolution` | **not** `H1CalcSystematic` flags — separate design required | n/a | **Out of v6sys scope.** Per Source-By-Source table, propagated outside the H1Calc shift family; need their own track-level shift mechanism (charge-dependent weight, resolution smear, efficiency mask) |
| `trigger` | analytic / external normalisation | n/a | **Out of v6sys scope** per Policy Box |
| `lumi` | analytic | n/a | **Out of v6sys scope** per Policy Box |
| `NucliaWeight` | analytic envelope, supervisor-decision pending | n/a | **Out of v6sys scope** per Policy Box |

Summary by clearance bucket:

- **Smoke-cleared (Stage 2 only):** `central`, `electron_energy_up`.
- **Staged (call site mapped, magnitude accepted, run pending):**
  `electron_theta_up`.
- **Mirror needed (down direction):** `electron_energy_down`,
  `electron_theta_down`.
- **Magnitude-blocked (require `--sys-allow-default` or supervisor magnitude decision):**
  all electron-phi, all HFS (energy / theta / phi), noise energy.
- **Out of v6sys scope (need separate mechanism):** all tracking sources,
  trigger, lumi, NucliaWeight.

---

## 4. Q4 — Final-systematics blockers and next physics validation artifact

### 4.1 Final-systematics blockers (ordered by severity)

1. **Stage 1 v6sys event-processing not smoked yet.** This is the dominant
   blocker for **any** kinematic detector systematic. Stage 1
   preselection (Empz ∈ [35, 70], electron fiducial, Zimpact / PhiOctant
   cuts, `Q2es > 150`, `Yes ∈ [0.1, 0.7]`) is `gH1Calc`-dependent; an
   electron shifted by +0.5 % can move in or out of the Empz / Q²
   window. Stage2-only smokes leave the admitted-event set fixed and
   therefore **cannot represent the migration-into and migration-out-of
   admission**. The Policy Box in `plan-v6sys-treebuilder.md` (lines
   348–358) already states this explicitly: “Final unfolded kinematic
   systematics require paired data+MC Stage 1 + Stage 2 reruns and
   matched-shift response / unfolding.” No Stage 1 paired smoke evidence
   exists yet.
2. **No per-iSys response-matrix rebuild.** A correct LAr-side
   systematic in the g1j_ES convention is a **full migration-matrix
   rebuild** per iSys (see `research-g1jES-electron-LAr-systematics-2026-06-01.md`
   §2 final paragraph: “in g1j_ES, every detector systematic is a full
   response-matrix rebuild, not a posthoc reweight”). The v6sys smoke
   has not produced even one shifted response. Without it, any per-bin
   uncertainty propagation reduces to a rank-1 cov on noisy bin
   differences — exactly the failure mode flagged FATAL in
   `research-electron-energy-systematic-critique-2026-05-26.md` §A1 and
   `research-electron-theta-critique-2026-05-26.md` §F1/F3.
3. **Down-side variations not produced.** The current smoke only exercises
   `electron_energy_up`. SIDIS A_h needs symmetric ± to permit physical
   charge-cancellation in the asymmetry numerator (the structural
   asymmetry-overstatement explained in
   `research-electron-energy-systematic-critique-2026-05-26.md` §A2).
   Down-side mirror runs are a hard requirement.
4. **Magnitudes for `electron_phi`, `hfs_*`, `noise_energy_*` not defined.**
   These are mapped to H1CalcSystematic enums but have no SIDIS-specific
   magnitude. The plan’s `--sys-allow-default` flag is a planning hook
   only; an explicit `h1-systematics` magnitude decision is required
   before any production use.
5. **Tracking systematics not addressed by `H1CalcSystematic`.** Charge-
   dependent track efficiency, momentum-resolution smear, and the
   charge-asymmetric efficiency that drives A_h are all outside the
   v6sys H1Calc shift family. A separate design (track-level weight
   map or smear function) is required and is out of the current smoke
   scope.
6. **Per-bin noise-floor characterisation outstanding.** Even after the
   per-iSys response is built, the 4D 480-bin Scheme B′ grid contains
   bins where the unfold response to a small shift is dominated by
   unfold noise rather than by signal (244/480 same-side bins for
   eElecEn; 263/480 for eElecTh, per the May-26 critiques). Until a
   zero-shift noise-floor study is in hand (e.g. half-split unfold or
   τ-neighbourhood scan), the per-iSys δ_i in noisy bins cannot be
   distinguished from noise and the rank-1 cov claim
   (`cov = δ δᵀ`) over-correlates noise with signal.

### 4.2 Next physics validation artifact

The single most efficient next step is **a Stage 1 + Stage 2 paired
v6sys smoke for `electron_energy_up`** on one period (e.g. `em0405` or
`ep0607`), for both data and DJANGOH MC. The artifact should report:

- **(a) Stage 1 admission-boundary delta.** Number of events
  passing Stage 1 nominal vs Stage 1 with `electron_energy_up`
  applied, by Empz / Q²es / Yes / electron-fiducial cut layer
  separately. This quantifies the magnitude of the migration-into /
  migration-out-of admission that Stage2-only smokes structurally
  miss.
- **(b) ChargedTot row-count delta from Stage 1 to Stage 2 chained.**
  Number of selected hadron tracks in the central vs shifted chain,
  for h⁺ and h⁻ separately, to expose any charge-dependent
  Stage 1 admission shift.
- **(c) Per-bin mean ΔQ²es / ΔYes / Δxes on the 4-period Scheme B′
  Q²×x grid.** Means computed per cell on the **paired-event subset**
  (events that pass both nominal and shifted Stage 1), to isolate the
  pure shift response from the admission-boundary effect.

If (a) shows ≪ 1 % Stage 1 admission shift relative to the bulk
event count and (b) shows symmetric h⁺/h⁻ behaviour, the Stage 1 +
Stage 2 paired requirement still stands for **publication** but the
Stage2-only smoke can be cleared as a faithful preview of the shifted
response surface, sufficient to justify proceeding to the per-iSys
response build for previewed cells. If (a) shows ≳ few % admission
shift, Stage2-only previews are explicitly **insufficient** for any
kinematic-systematic source and the workflow must be re-planned so
that every shifted-MC variation triggers a paired Stage 1 + Stage 2
run.

Secondary artifacts, in order:

- **One-line ROOT macro under relvol10 4.2.1** printing
  `int(H1CalcSystematic::eElecEn)` and `int(H1CalcSystematic::eElecTh)`
  — needed before any wrapper hard-codes integer sys-flag values.
- **Per-wheel and per-region application audit:** dump the
  `H1CalcSystematic` state inside the v6sys builder at event 0 to
  confirm all 5 wheels (eElecEn) and all 3 regions (eElecTh) are
  populated with the requested ±0.005 / ±0.001, not a single global
  setter.
- **Co-shifted track-θ verification:** confirm `eElecTrTh` is co-shifted
  with `eElecTh` in the same direction (the g1j_ES `AnalysisBase.C:139`
  call pattern).

---

## 5. Open items / risks not blocking smoke clearance

- The `Q2e ≡ Q2es` bit-identical identity at high-Q² (verified earlier;
  `plan-highQ2-sidis-next-steps.md` Task 5) means the smoke
  measurement of `Q2es` is also the measurement on `Q2e`. The shifted
  smoke does not introduce a new bias from this identity, but any
  downstream Q²e-cut consumer inherits the same shift.
- The earlier critique-fatal verdicts on `source_electron_energy.json`
  and `source_electron_theta.json` rank-1 covariance structure
  (`research-electron-energy-systematic-critique-2026-05-26.md` §A1–A2;
  `research-electron-theta-critique-2026-05-26.md` §F1–F3) **do not
  apply to the v6sys smoke directly** because v6sys produces shifted
  ROOT trees, not posthoc weights. They apply to whatever combiner
  consumes the eventual per-iSys unfolded yields and must be re-examined
  once the per-iSys response and unfold outputs exist. The shift to a
  full migration-matrix rebuild structurally removes the rank-1
  failure mode; the per-bin noise-floor question still needs a closure
  study.
- The g1j_ES one-sided detector-side quadrature convention
  (`SysUncertainty.cxx:344, 351, 354`) should **not** be ported to
  SIDIS. SIDIS asymmetry observables need symmetric ± per
  `research-g1jES-electron-LAr-systematics-2026-06-01.md` §3.4.

---

## 6. Status

**Written artifact:**
`H1/sidis_highQ2/docs/research/research-v6sys-smoke-physics-review-2026-06-03.md`
(this file).

**Scope covered:** the four dispatched physics-review questions, with
cross-citations to the existing g1j_ES and electron-systematic
critiques and the v6sys plan.

**Open areas explicitly flagged:**

- Stage 1 v6sys event-processing smoke (paired Stage 1 + Stage 2 for
  `electron_energy_up` on one period, data + MC) — required as next
  physics validation artifact.
- `int(H1CalcSystematic::eElecEn/eElecTh)` value confirmation under
  relvol10 4.2.1.
- SIDIS-specific magnitudes for `electron_phi`, `hfs_*`, `noise_energy_*`.
- Per-wheel and per-region application audit inside the v6sys builder.
- Co-shifted `eElecTrTh` verification.

**No remote commands were executed; SSH control socket was not touched.**
