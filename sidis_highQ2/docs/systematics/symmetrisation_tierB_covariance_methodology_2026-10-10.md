# Symmetrisation rule and Tier-B covariance models for the paired detector-leg campaigns — 2026-10-10

**Status:** P1 METHODOLOGY DECISION DOCUMENT (gap audit §7 item 7:
"Symmetrisation + Tier-B covariance model decisions (§3.1–3.2) — must precede
interpretation of the P2 campaigns"). Read-only task; this document is the only
written artifact. It **extends** `FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`
(register rules 1–3, decisions 1–2, §4 combination prescription, §5 gates) and
does not override it; every point where the design departs from or extends the
frozen spec is flagged in §7 rather than silently applied.

**Revision note (2026-10-10, revision 1 — after critic PASS WITH CONDITIONS,
2026-10-10):** the P2 production design is cleared; this revision applies the
critic findings to the interpretation and covariance-assembly layers only.
Applied: (1) §4.2 SIGNED rank-1 on `Q_s` adopted as DEFAULT, all-ones block
demoted to a labeled conservative variant; the blanket "zero cross-charge
coupling is conservative for A_h" claim RETRACTED (false for measured negative
correlation — Var understates up to 2×); explicit ρ<0 branch added (§4.4);
(2) the `σ_data,diff ≈ sqrt(σ+²+σ−²)/2` identity and the "half-difference
noise is √2 below single-side noise" claim deleted/corrected — `d+^r` and
`d−^r` are functionals of the SAME replica noise through near-mirror operator
differences, anti-correlated under odd response (`σ_diff ≈ σ±`, no √2
suppression); the measured spread classifies and no formula may be substituted
for it; raw |δ| on noise-dominated bins is justified solely by the frozen
budget convention (§2.1, §3); (3) flag-gated `max(|d+|,|d−|)` envelope
candidates emitted for SA-MAG and SA-FLAT bins (approval-pending, like
SA-EVEN); dropped even part `||d+|−|d−||/2` recorded in the payload; CLEAN-ODD
tolerance recomputed with the measured side-residual correlation (§3);
(4) enforceability closures: exhaustive parity partition (the
{one side ∈ [2,3σ), other <2σ} hole closed), explicit census-only
side-qualification case, case-B single-side-fallback covariance defined,
numeric criteria pinned for the census test and the source-wide SA-EVEN
census (§2.3, §3); (5) §5 A_h list extended: fake-fraction/background
charge-asymmetry (f+ = 10.7141% ≠ f− = 10.4019%, zero propagated
uncertainty), trigger and NucliaWeight common-mode demonstration
requirements; "noise-floor parts uncorrelated" replaced by "correlation
UNMEASURED, gated by the E31-family measurement"; (6) c_rad_gen diagonal
variant FORBIDDEN in multi-bin assemblies; the +0.6419%/+0.6499% figures
labeled with provenance and distinguished from the raw +0.4458%/+0.4539%
cross-generator agreement; per-bin charge-commonness audit added as a
prerequisite for any "largely cancels" claim (§4.3, §5); (7) phi_h diagonal
labeled INVALID for coherent-integrated use; integrated phi_h quotes require
the option-B per-generator signed shapes (§4.3); (8) payload precedence:
`z_diff` governs `Q_s` membership, per-side classes are metadata only, dual
labels emitted on disagreement (§4.2, §3 Step 4). The battery design,
noise-floor evidence, quarantine route, and magnitude conventions are
UNCHANGED (critic-confirmed sound).

**Scope:** the six paired detector-leg campaigns (hfs_jes, hfs_rces, hfs_theta,
elec_energy, elec_theta, hfs_phi; 6 sources × 2 sides ≈ 12,300 re-reco jobs per
the gap audit §4), their symmetrisation into per-bin budget components, and the
correlation model per component class for the full 14-component (post-hfs_phi)
covariance on the 490-bin `(charge, bin_index)` map (245 h+ then 245 h−).

**Basis:** canonical budget
`H1ChargedSidis_HighQ2/output_v6/systematics/summary_plots/total_uncertainty_z005_coherentprior_b1floor_crad13_releasebin_emfix_2026-10-10/`
(2026-10-10 promotion; CSV byte-identical to the `emfix_2026-10-09` package cited
in the dispatch, sha256-verified; totals h+ median 5.7093% / h− 6.1523%);
component derivations per `budget_13components_derivation_2026-10-10.md`. All
components are per-bin RELATIVE; relative matrices are invariant under the
applied C_rad (Born-level absolute matrices scale entrywise by
`C_rad_i·C_rad_j`).

---

## 1. Objects and conventions

For source `s` and published bin `i` (490-bin map):

- `X0_i` — nominal coherent-prior IBU k=4 cross section (central release).
- `X+_i`, `X−_i` — full-chain unfolded cross sections of the up/down shifted
  campaigns (shifted re-reco of data AND MC, shifted response rebuild,
  shifted-response own `body+miss` prior, k=4, binmap v3, canonical 658-file
  data list, `L=351000.0 nb^-1` relative-only outputs).
- One-sided relative shifts: `d+_i = (X+_i − X0_i)/X0_i`, `d−_i = (X−_i − X0_i)/X0_i`.
- Signed half-difference: `δ_i = (X+_i − X−_i)/(2·X0_i) = (d+_i − d−_i)/2`
  (identically the spec's `Delta_i=(X_i^+−X_i^−)/2` in relative units).
- Reflected down shift `a−_i = −d−_i` (under a linear/odd response `d+ ≈ a− ≈ δ`).
- Final per-bin component magnitude `u_i^s` (the budget column), and the signed
  sidecar `δ^s` retained per spec rule 2 (retain `X+`, `X−`, signed `Delta`).

Symmetrisation is applied **per source** to the *relative* shifts; absolute
uncertainties are `u_i·X0_i` (spec §4: `u = abs(x0)·r` for fractional entries).

## 2. Noise-qualification machinery

### 2.1 Per-bin noise floors (the measured scale)

The noise scale of a *shift estimate* is NOT the per-bin toy-stat envelope
`ibu_stat_rel`: nominal and shifted unfoldings share the same data events, so
the common data-Poisson component cancels in the difference
(`systematics_smoothing_multidim_proposal_2026-10-02.md` §2:
using `ibu_stat_rel` as the floor is "wrong in both directions"; measured paired
floors are 9–60× below the MAD scale, which itself sits at the `ibu_stat` scale).
The measured scale is the **paired-replica difference floor** (P1 Phase A
machinery, `systematics_signal_vs_noise_2026-10-03.md` §6/§8, extended to the
paired legs):

- **Battery:** N = 100 (minimum 50) paired Poisson data-vector bootstrap
  replicas; each replica is re-unfolded **three times** (nominal, up, down) with
  the frozen central IBU settings (k=4, coherent prior, fixed shifted
  responses). Replica-consistent shift estimates per replica r:
  `d±^r_i = (X±^r_i − X0^r_i)/X0^r_i`, `δ^r_i = (d+^r_i − d−^r_i)/2`.
- **Data floors:** `σ_data±,i` = per-bin spread (ddof=1) of `d±^r_i`;
  `σ_data,diff,i` = the MEASURED per-bin spread of `δ^r_i`. No independence
  identity is assumed and none may ever be substituted for the measurement
  (revision 1): `d+^r_i` and `d−^r_i` are functionals of the SAME replica
  noise through near-mirror operator differences, so under an odd response
  their noise contributions are ANTI-correlated and ADD in the
  half-difference — `σ_data,diff ≈ σ_data±` (there is NO √2 suppression
  relative to a single side; the former
  `≈ sqrt(σ_data+² + σ_data−²)/2` identity is RETRACTED as wrong — it would
  hold only for independent side noises, which the paired battery explicitly
  does not have; only the COMMON data-Poisson component cancels, which is
  already why the floors sit far below `ibu_stat`). The measured spread is
  what classifies. N=100 gives ~7% floor precision.
- **Side-residual correlation (revision 1):** the battery additionally
  records the per-bin correlation `ρ̂_res,i` of the side residuals
  (`d+^r − mean_r d+^r` vs `d−^r − mean_r d−^r` across replicas; cell-pooled
  where the per-bin population is too small). Under an odd/mirror response
  `ρ̂_res ≈ −1` (the same replica noise enters with opposite operator
  signs); under an even response `ρ̂_res ≈ +1` — so `ρ̂_res` is itself a
  parity diagnostic. It is required by the §3 magnitude-consistency
  tolerance; independence (`ρ̂ = 0`) is a stated last resort only.
- **MC flip bound:** the shifted responses are re-reconstructions of the SAME
  MC events as central (validated §8.4: common response noise cancels in the
  paired difference); the residual re-reconstruction "flip" noise is bounded
  first-order by the §8.4 net-flow estimator, per side: `σ_flip±,i`. For the
  half-difference, `σ_flip,diff,i` must be estimated DIRECTLY by applying the
  §8.4 net-flow estimator to the up-minus-down response difference
  `|B+ − B−|` (revision 1, consistency with the same-replica correction
  above): under a mirror response `|B+ − B−| ≈ 2|B+ − B0|`, so
  `σ_flip,diff ≈ σ_flip±` — the former per-side combination
  `sqrt(σ_flip+,i² + σ_flip−,i²)/2` is RETIRED (it assumes independent side
  flips and would UNDERestimate the difference floor under mirror
  anti-correlation; the former "may overestimate" claim was wrong). The
  definitive treatment remains the §4.4 response-bootstrap upgrade
  condition.
- **Total floors:** `σ_tot±,i = sqrt(σ_data±,i² + σ_flip±,i²)`;
  `σ_tot,diff,i = sqrt(σ_data,diff,i² + σ_flip,diff,i²)`.
- **Zero-shift / same-side placebo closure:** the battery must close — the
  placebo (zero-shift or nominal-side replica) spread reproduces the replica
  floor, and the replica-mean shift reproduces the campaign working point
  within the floor (P1 precedent: bias 0.066–0.075 of floor).

**Toy-stat envelope rule (restricted use):** `|d±_i| > ibu_stat_rel,i` is a
* sufficient* condition for "resolved" (the difference noise is far below the
toy envelope); failing it is **not** evidence of noise-domination. The
toy-stat envelope must never be used to fail a side or a bin.

### 2.2 Per-bin noise classes (workspace convention)

`z±,i = |d±_i|/σ_tot±,i` and `z_diff,i = |δ_i|/σ_tot,diff,i`;
classes: **NOISE** < 2σ, **MIXED** 2–3σ, **REAL** ≥ 3σ
(`systematics_signal_vs_noise_2026-10-03.md` §8.1; B1 certification §9.2).
Pure-noise expectation: 4.6% of bins ≥ 2σ, 0.27% ≥ 3σ.

### 2.3 Campaign-level side qualification

A **side** (one signed campaign of one source) is:

- **INVALID** if any production gate fails: canonical 658-file data list
  (gap-audit §3.4 fix), single-file smoke test, normalization check
  (`L=351000.0`, relative-only outputs), coherent `body+miss` prior, k=4,
  binmap v3, 490-row finite output, zero negative cross sections,
  closure/refold/stability checks, battery closure (§2.1).
  → mandatory rerun; **no fallback value may be derived from an invalid side**;
  the source is BLOCKED pending the rerun (the valid side may be quoted as a
  one-sided provisional audit value only).
- **Census significance (pinned, revision 1):** let `N₃` = number of bins
  (of 490) at ≥ 3σ on the side's own battery floor. Pure-noise expectation is
  0.27% ≈ 1.3 bins (Poisson). The census is **significant** iff
  `N₃ ≥ 10` (≈ 7.7× the expectation; p ≈ 1.2e-6), **noise-consistent** iff
  `N₃ ≤ 7` (p ≥ 4e-4), and **ambiguous** for `N₃ ∈ {8, 9}`
  (p ≈ 6e-5…9e-6): in the ambiguous band the cell-level evidence decides —
  a coherent cell response present → treat as significant (flag
  `census_ambiguous`); absent → treat as noise-consistent (flag
  `census_ambiguous` recorded). The ≥2σ census `N₂` is recorded alongside
  (expectation 22.4) but does not gate alone.
- **NOISE-DOMINATED** if all gates pass AND the census resolves
  noise-consistent (`N₃ ≤ 7`, or `N₃ ∈ {8,9}` with no coherent cell response
  — flag `census_ambiguous`) AND no coherent cell-level response
  (per-(Q2,x)-cell medians) survives. → single-side fallback (§3, case B),
  PROVISIONAL label.
- **QUALIFIED-coherent** if all gates pass AND the census resolves
  significant (`N₃ ≥ 10`, or `N₃ ∈ {8,9}` with a coherent cell response —
  flag `census_ambiguous`; measured one-sided precedent: REAL fractions
  70.8–92.4%, med z 6.8–35.8) AND a coherent cell-level response is present
  (cell medians ≥3σ in at least half the 14 (charge, cell) grids, charge
  sign agreement) AND the placebo closure passes.
- **QUALIFIED-census-only (flagged, revision 1 — the formerly undefined
  case):** all gates pass AND the census is significant (`N₃ ≥ 10`) BUT no
  coherent cell-level response is present. A significant ≥3σ census is
  direct evidence of real response (it cannot arise from noise), so the side
  is QUALIFIED with the mandatory flag `no_cell_coherence` — it is NOT
  demoted to NOISE. The flag forces the §4.4 cross-charge audit to start
  from per-charge blocks and records the missing pattern evidence for
  diagnosis.
- **Contradictory (mandatory review, revision 1):** the census resolves
  noise-consistent at `N₃ ≤ 7` yet cell medians ≥3σ appear (possible only
  outside the ambiguous band, which the cell evidence resolves by
  construction). This combination is not silently classifiable either way;
  the side goes to review before any qualification or fallback use.

Qualification is assessed on each campaign's own battery; the P1 Phase A
floors for the legacy one-sided tags are precedent for the method, not a
substitute for the per-campaign battery.

## 3. Symmetrisation rule (mechanical, per source)

Applied identically by every campaign QA. Output payload per source: the
signed sidecar (`d+`, `d−`, `δ`, floors, `ρ̂_res`, z-scores, classes, parity
flags, dual labels, dropped even parts, estimator used, approval status) plus
the budget column `u^s`.

**Step 0 — campaign gates.** Both sides must first pass §2.3 production gates.
If either side is INVALID → source BLOCKED (no component value; audit-only
one-sided quote from the valid side, labelled one-sided/provisional per spec
rule 3).

**Step 1 — side qualification census.** Classify each side QUALIFIED /
NOISE-DOMINATED per §2.3.

**Step 2 — per-bin parity classification** (uses §2.2 classes; revision 1:
the partition below is EXHAUSTIVE over the (z±, sign, magnitude-consistency)
combinations — the former `{one side ∈ [2,3σ), other < 2σ}` fall-through is
closed by the redefined `SA-FLAT`):

| flag | condition | meaning |
|---|---|---|
| `CLEAN-ODD` | z+ ≥ 2 AND z− ≥ 2 AND sgn(d+) = −sgn(d−) AND `||d+|−|d−|| ≤ T_i` | linear/odd response; half-difference valid |
| `SA-EVEN` | z+ ≥ 2 AND z− ≥ 2 AND sgn(d+) = sgn(d−) | **shape-asymmetry flag (even/quadratic)**: both sides move the same way w.r.t. nominal; half-difference invalid as a magnitude (can vanish while both sides moved) — the quarantined hfs_theta pattern (+0.2568%/+0.2588% signed means) |
| `SA-MAG` | z+ ≥ 2 AND z− ≥ 2 AND sgn(d+) = −sgn(d−) AND `||d+|−|d−|| > T_i` | odd+even mixture; magnitude asymmetry between sides — resolvable even content |
| `SA-FLAT` | exactly one side ≥ 2σ (MIXED or REAL), the other < 2σ | one-sided-resolved bin (asymmetric response or unresolved side); the resolved side's class (MIXED/REAL) is recorded |
| `NOISE` | z+ < 2 AND z− < 2 | bin unresolved |

with the magnitude-consistency tolerance (revision 1: measured correlation,
not independence)

`T_i = 2·sqrt(σ_tot+,i² + σ_tot−,i² + 2·ρ̂_res,i·σ_tot+,i·σ_tot−,i)`.

For opposite-sign resolved bins `|d+| − |d−| ≈ d+ + d−`, whose replica
variance carries the `+2·ρ̂_res` covariance term (§2.1): under the odd-mirror
response `ρ̂_res < 0`, so `T_i` is SMALLER than the former independence value
`2·sqrt(σ_tot+²+σ_tot−²)` and the SA-MAG (even-content) test is
correspondingly MORE sensitive. Where the per-bin `ρ̂_res,i` cannot be
estimated, use the cell-pooled value; `ρ̂ = 0` (independence) is a stated
last resort only. Every bin additionally carries the **dropped even part**
`e_i = ||d+_i| − |d−_i||/2` in the payload (revision 1), so no nonlinear
content is silently discarded by any estimator choice below.

Sign-agreement between sides is only meaningful when both sides are resolved;
sign structure below 2σ is noise (sign-flip rate ≈ 0.5 under noise) and must
not populate `SA-EVEN`.

**Step 3 — per-bin estimator (decision tree, exhaustive and ordered):**

- **(A) Both sides QUALIFIED (default path):**
  - `SA-EVEN` bins → `u_i = max(|d+_i|, |d−_i|)` (max-side envelope) written as
    the **candidate** value with the flag; promotion of any envelope candidate
    requires the explicit nonlinear-response approval of spec rule 2 / decision 1.
    If the `SA-EVEN` census is **source-wide and stable** — pinned (revision 1):
    SA-EVEN bins are ≥ 50% of the both-sides-resolved (≥2σ) bins in ≥ 5 of the
    7 (Q2,x) cells for both charges AND the cell-averaged even part
    `(d+ + d−)/2` is nonzero at ≥ 3σ with both sides' signed means of the same
    sign (the hfs_theta pattern: +0.2568%/+0.2588%) — the source goes to the
    **quarantine path** (spec decision 3): block, diagnose or rerun; neither
    rank-1 nor diagonal two-sided construction enters final totals. If
    approval is denied for non-source-wide even bins, the source cannot be
    promoted as final with those bins unresolved.
  - `SA-MAG` and `SA-FLAT` bins → default **`u_i = |δ_i|`** (coherent
    half-difference), but the payload additionally records the flag-gated
    **envelope candidate** `max(|d+_i|, |d−_i|)` (revision 1: the estimator
    must not discard nonlinear content — for these bins the half-difference
    tracks only the odd part while the excursion range is set by the larger
    side), pending the same nonlinear-response approval as SA-EVEN; the
    dropped even part `e_i` is recorded for every bin either way.
  - `CLEAN-ODD` and `NOISE` bins → **`u_i = |δ_i|`** (coherent
    half-difference), with the parity/noise flags attached.
  - For ALL classes: the bin-level noise class does NOT switch the estimator,
    and raw |δ| on noise-dominated bins is kept **solely** per the frozen
    budget convention for detector legs (raw per-bin magnitudes, no
    floor-lifting, sparse-bin excursions stay raw, as in the current budget —
    revision 1 removes the former noise-based justification, which was
    incorrect: there is no √2 suppression of the half-difference noise,
    §2.1). Flags feed the covariance model (§4), not the magnitude.
- **(B) Exactly one side QUALIFIED, the other NOISE-DOMINATED:** the
  half-difference would attribute only half the qualified side's excursion
  (the failed side carries no real antisymmetric partner). Use the
  **single-side fallback** `u_i = |d_qual,i|` for all bins of the source,
  labelled `one_sided_fallback` + PROVISIONAL source status. A documented
  diagnosis of the failed side (even response / genuine asymmetry /
  production-limited) must accompany the fallback. Its covariance (revision 1)
  is the §4.2 hybrid applied to the SIGNED single-side vector
  `v = d_qual^s` with `Q_s = {i : z_qual,i ≥ 3}` (the qualified side's own
  z-scores); the DEFAULT is diagonal `diag(u²)` and stays diagonal until the
  qualified side's own battery passes the §4.4 gates — a one-sided vector
  must not be granted rank-1 coherence without its own noise/coherence test.
  See §7 flag 1: this yields a budget VALUE for interpretability;
  final/IMPLEMENTED status still requires the failed side to be rerun and
  pass, or an explicit approval — the spec's "block, not one-sided promotion"
  governs STATUS, not the interim value.
- **(C) Both sides NOISE-DOMINATED:** source BLOCKED from the final detector
  tier (no coherent response demonstrated at either sign); quote both one-sided
  vectors as provisional audit values only.

**Step 4 — bookkeeping.** The signed half-difference sidecar (`δ^s`) is
retained for every source regardless of estimator (spec rule 2); the
per-bin payload carries `d+`, `d−`, `δ`, the floors, `ρ̂_res`, the z-scores
(`z±`, `z_diff`), the §2.2 classes, the parity flag, the dual label wherever
the per-side class and the `z_diff` class disagree (revision 1, see §4.2),
the dropped even part `e_i`, the estimator used, any envelope candidate, and
the approval status; the historical one-sided
values (`hfs_jes_down`, `hfs_rces_up`, `hfs_theta_up`, `elecE_p1pct`,
`elecTheta_p1mrad`) remain PROVISIONAL audit values excluded from the final
detector tier until replaced by paired campaigns (spec rule 3) — unchanged.

**Amplitude note:** the frozen amplitudes are the spec's (e.g. ±0.5% per each
of five LAr wheels for elec_energy — the +1% legacy vector is ~2× the frozen
amplitude and is replaced, not rescaled; ±1% in-jet JES / out-of-jet RCES;
±1 mrad eHadTh; ±1 mrad three-region eElecTh co-shifted with eElecTrTh;
±20 mrad eHadPh). Symmetrisation never rescales an amplitude.

## 4. Covariance models per component class (full 14-component assembly)

### 4.1 Assembly identity

On the 490-bin map, `C_total = C_stat + Σ_{s∈A} C_s + Σ_{s∈B} C_s + Σ_{s∈C} C_s`,
only IMPLEMENTED-and-gated sources entering numerically; blocked, dropped,
placeholder and diagnostic sources contribute **no numeric zero** (completeness
ledger; they prevent "final" status). Every `C_s` must be finite, symmetric,
PSD within tolerance (1e-12 house standard), aligned by `(charge, bin_index)`,
and carry source-status/correlation metadata. **Diagonal closure by
construction:** every model below satisfies `C_s,ii = (u_i^s)²` with `u^s` the
budget column, so the per-bin reporting identity
`u_total,i = sqrt(C_total,ii) = sqrt(Σ_s (u_i^s)²)` (spec §4.1) holds exactly.
Integrated quantities use `Var(I) = a^T C a`; never quadrature-sum per-bin
percentages. Born-level absolute matrices scale entrywise by `C_rad_i·C_rad_j`;
relative matrices are invariant.

### 4.2 The Tier-B hybrid construction (noise-gated, SIGNED rank-1 default)

For a signed shift shape `v^s` (the signed half-difference sidecar for paired
detector legs; the signed single-side vector `d_qual^s` for case-B fallback
sources, §3 case B; signed base vector for model/reg) with magnitudes `u^s`,
and the resolved bin set `Q_s = {i : z_i ≥ 3}` — where `z_i` is **`z_diff`**
for paired sources and `z_qual` for case-B fallback sources (revision 1:
`z_diff` governs `Q_s` membership; the per-side NOISE/MIXED/REAL classes are
METADATA only, and a dual label, e.g. `REAL(side)/NOISE(diff)`, is emitted
wherever the per-side class and the `z_diff` class disagree):

```
C_s = D_s · M_s · D_s,   D_s = diag(u_i^s),
M_s = ( S on Q_s ) ⊕ ( I on the complement ),
S_ij = sgn(v_i)·sgn(v_j)  ∀ i,j ∈ Q_s      (DEFAULT, revision 1)
```

- The DEFAULT block is the **signed rank-1**: `C_ij = v_i·v_j` on `Q_s`
  (since `u_i = |v_i|` there), i.e. the signed outer product of the resolved
  shift shape. Bins keep their measured SIGNS, so zero-crossings and
  sign-changing structure are represented instead of being forced
  co-positive. PSD: `S = s s^T` with `s_i = sgn(v_i)` is rank-1 PSD, `I` is
  PSD, their direct sum is PSD, and congruence by `D_s` preserves PSD.
  Diagonal is exactly `u²` everywhere.
- The former all-ones block `P_ij = 1` (equivalently `|v||v|^T` on `Q_s`) is
  RETAINED ONLY as an explicitly labeled **conservative variant**
  (`all_ones_envelope`), for use where the signed shape's sign structure is
  not trusted (e.g. minimally resolved sources). It overstates coherent
  integrated variance by forcing co-positive bins and must be named in any
  assembly that uses it. It is NO LONGER the default.
- For bins promoted to the envelope estimator (§3 case A, approval-gated),
  the covariance sign is that of the dominant side (the larger-magnitude
  shift) and the bin enters `Q_s` at its envelope magnitude; the signed
  sidecar remains the record.
- **Rank** = 1 + |complement of Q_s| (or 2 + |complement| if per-charge blocks
  are used, §4.4).
- Physics: a single coherent nuisance (one JES scale, one wheel-energy scale,
  …) produces a deterministic signed shift shape → signed-full correlation
  where the shape is RESOLVED; noise-dominated bins' apparent shifts are
  noise draws → no imposed coherence. This operationalizes spec decision 2's
  evidence line ("same-side audit showing `abs(Delta_i)` exceeds noise where
  rank-1 is used") and gate 7 ("rank-1 is prohibited without a passed
  coherence/noise test"). It is a new matrix FORM beyond the spec's three
  enumerated options — see §7 flag 2.

### 4.3 Class-by-class models

| # | Component (tier) | Construction | Rank | Enters |
|---|---|---|---|---|
| 1 | `ibu_stat_rel` (A) | measured 1000-toy covariance, dense per-charge 245×245 blocks; h+/h− cross-block **ZERO BY ASSUMPTION** (independent Poisson loops, seeds h+ 12345 / h− 22345) — block-diagonal, flagged; final A_h requires the measured cross-block | ≤245 per charge block | xsec, mult., ratios/A_h (cross-block gate) |
| 2 | `lumi_rel` (A) | `C = l l^T`, `l_i = 0.027·x_i`; fully bin- and charge-correlated | 1 | **absolute xsec ONLY**; excluded from multiplicities and A_h (cancels exactly) |
| 3 | `tracking_eff_m0p5_rel` (A, pinned) | diagonal `diag(u²)`, `u = |ensemble mean − nominal|` (16 seeds); charge-independent, cross-charge block zero; seed scatter QA-only (excluded) | 490 | xsec, mult., ratios/A_h (conservative in A_h GIVEN the measured positive charge correlation, §5) |
| 4–9 | detector legs ×6: `hfs_jes`, `hfs_rces`, `hfs_theta`, `elec_energy`, `elec_theta`, `hfs_phi` (B) | **default (pre-test): diagonal** `diag(u²)`; **post-test: hybrid §4.2 (SIGNED rank-1 default on `Q_s`)** with `v = δ^s`, `Q = {z_diff ≥ 3}` (`z_diff` governs; per-side classes metadata only); **case-B fallback sources:** `v = d_qual^s`, `Q = {z_qual ≥ 3}`, default diagonal until the qualified side's battery passes §4.4 | 490 (diag) / 1(–2)+n_rest (hybrid) | xsec, mult., ratios/A_h (varied charge-resolved yields) |
| 10 | `ibu_model_rel` (C, B1-floored) | `u = max(|d_base|, floor_B1)` (Option A rule); hybrid §4.2 with `v =` signed base `0.5(IBU_DJ−IBU_RG)` on REAL bins (≥3σ vs B1 floor; 20.7% certified REAL), diagonal elsewhere (u = floor where bound); B1 floor is the COMPLETE statistical floor (data-free crossclosure estimator; no flip bound added); existing approved forms retained (full signed rank-1 = envelope scenario; diagonal = fallback) | 1+n_rest | xsec, mult., ratios/A_h |
| 11 | `ibu_reg_rel` (C) | requires the **signed sidecar** `δ_reg = 0.5(IBU(k=5)−IBU(k=3))` (P1 companion; B2 machinery exists — published column = `|N5−N3|/2`, matched 1e-11; B2 half-spread floors medians 0.38/0.40%); hybrid §4.2 with `Q = {≥3σ vs B2 floor}` (26–27% REAL); pre-sidecar: diagonal fallback | 490 / 1+n_rest | xsec, mult., ratios/A_h |
| 12 | `phi_h_gauss_rel` (C, disposition pending) | unsigned floored envelope (0.5·max-gen smoothed `|δ|`, 85% floor) → **diagonal default** (no single signed nuisance shape exists); the diagonal is INVALID for coherent-integrated use (revision 1) — single-bin bookkeeping only; integrated/multi-bin phi_h quotes REQUIRE the option-B per-generator signed shapes from the disposition + approval, and until they exist NO valid multi-bin phi_h covariance exists (the gap must be carried explicitly); double-count caveat (~1.6% median fork/model content already in `ibu_model_rel`) while option A stands | 490 (single-bin only) | xsec, mult., ratios/A_h (needs signed per-generator deltas, which exist) |
| 13 | `c_rad_gen_rel` (C) | house-style mirror of the cradstat package: **rank-1 `u u^T`** (u = the V3 smoothed-`|δ|` budget column; 100% bin+charge coherence as a documented conservative ENVELOPE assumption, not a measurement). The diagonal variant is FORBIDDEN in any multi-bin/integrated assembly (revision 1): it understates the coherent-integrated variance, and the generator choice is ONE discrete coherent nuisance — rank-1 is the ONLY admissible multi-bin form; the diagonal is strictly single-bin bookkeeping | 1 | xsec, Born products, mult. (per C_rad convention); largely cancels in A_h ONLY after the §5 charge-commonness audit |
| 14 | `c_rad_stat_rel` (C, already emitted) | rank-1 `u u^T` (u = applied DJ14 releasebin `relative_stat_unc`) + diagonal, absolute and relative (`covariance_cradstat/`, carried byte-identical through emfix); same fixed MC sample feeds both charges → charge-common multiplicative; per the cradstat package guidance the rank-1 form is for integrated/multi-bin quotes, the diagonal for strictly single-bin (same restriction as row 13) | 1 | xsec, Born products, mult.; largely cancels in A_h ONLY after the §5 charge-commonness audit |

`hfs_phi` (#9) is the 14th component only after its paired campaign exists and
passes (spec row 7 BLOCKED today; no numeric zero before that).

### 4.4 Detector-leg details, upgrade ladder, and gates

- **Ladder per Tier-B source:** (0) diagonal `diag(u²)` — always available,
  the spec's provisional default; (1) hybrid §4.2 after the noise/same-side
  tests pass; (2) a measured replica covariance would supersede either — but
  note the P1-style replicas measure the NOISE (they resample the data around
  fixed shifted responses; the systematic shift is common-mode across
  replicas), so a *systematic* shape covariance is not measurable from them; a
  full shift-shape covariance would require nuisance-resampling (multiple
  amplitudes), which the frozen single-± prescription does not provide. The
  hybrid is therefore the terminal form under the frozen prescription.
- **Cross-charge admission (revision 1 — three-branch ρ rule; the former
  blanket "zero cross-charge coupling is conservative for A_h" claim is
  RETRACTED as false when the measured cross-charge correlation is
  negative: with `Var(δ+ − δ−) = σ+² + σ−² − 2ρσ+σ−`, zero coupling
  OVERSTATES Var(A_h) only for ρ > 0 and UNDERSTATES it by up to 2× for
  ρ < 0):** the general cross-charge form is per-charge signed rank-1 blocks
  with an explicit coupling
  `C = [[v+ v+^T, ρ_cc·v+ v−^T], [ρ_cc·v− v+^T, v− v−^T]]`
  (PSD iff |ρ_cc| ≤ 1), `v±` the per-charge signed shapes on `Q_s`. Branches:
  - **(a) audit passes** — cell-median sign agreement ≥ 6/7 (charge, cell)
    grids AND trimmed bin-level charge correlation ρ significantly positive
    (≥ 3σ) → `ρ_cc = +1`, the single spanning signed vector (the DEFAULT
    single-nuisance form). Measured one-sided precedents to re-measure on the
    paired legs: hfs_jes 7/7 (ρ_cell +0.77), hfs_rces 7/7 (+0.73), hfs_theta
    7/7 (bin-level ~0 — charge-specific varying component), elec_theta 6/7
    (+0.66), elec_energy 5/7 (trimmed +0.35; sparse-tail anti-correlated
    outliers).
  - **(b) ρ measured consistent with zero** AND bounded away from
    significantly negative → `ρ_cc = 0` (two per-charge blocks, zero
    coupling), labeled: conservative for integrated cross sections when the
    true ρ ≥ 0, NOT conservative for `A_h` if the true ρ < 0 — admissible
    only with the measured-zero evidence in hand.
  - **(c) ρ measured SIGNIFICANTLY NEGATIVE** → either the anti-correlated
    block with `ρ_cc` = the measured value (an `A_h`-amplifying construction,
    used only with a documented physical origin, e.g. a migration-type
    source moving tracks between charge pools), or BLOCK that source's
    `A_h` use (the source may still enter cross sections with per-charge
    blocks). A significantly negative ρ with no documented origin DEFAULTS
    TO THE BLOCK.
  These numbers must be re-measured on the paired legs.
- **Flip-bound condition:** if `σ_flip` materially moves the REAL census
  (precedent: elec_energy 70.8% → 41.8% REAL with the flip bound), a B1-style
  response bootstrap of both shifted responses is REQUIRED before hybrid
  promotion for that source (the §8.4 first-order bound is then not sufficient).
- **Same-side gates:** sources with promoted envelope bins (§3 case A) carry
  the even-component flag into their metadata; a quarantined even response
  (hfs_theta precedent) never enters through either covariance form.
- **Diagonal-closure note:** the hybrid's diagonal is exactly `u²` everywhere
  (the signed block `S = s s^T` and the identity both have unit diagonal), so
  the budget quadrature identity is preserved for every source and for the
  total.

### 4.5 Blocked / absent rows (ledger, no numeric zero)

`response_mcstat_rel` (row 12 — the central-response MC-stat bootstrap
covariance; NOT covered by B1, which resamples the model closure only),
`tracking_chargedep_rel` (row 11), `trigger_rel` (15), `NucliaWeight_rel` (16),
`background_rel`/`diffvm_rel`/`gammap_bkg_30pct_rel` (17–19, P-ALPHA chain),
and the DROPPED rows 20–24 (method spread never added). These appear in the
completeness ledger and prevent "final" status; they are not zero.

## 5. h+/h− ratio and `A_h` inheritance (charge-misID consistency)

First-order asymmetry response for `A = (x+ − x−)/(x+ + x−)`:
`ΔA ≈ (δ+ − δ−)/(x+ + x−) − A·(δ+ + δ−)/(x+ + x−)`. A **charge-common** shift
(δ+ = δ−) cancels; an **anti-correlated** shift (δ+ = −δ−) enters at FULL
amplitude; **uncorrelated** parts enter in quadrature (no cancellation). Per
spec §4.3: form `A±` from varied **charge-separated** yields (or
`C_A = J C_x J^T`); never infer cancellation from separate h+/h− totals;
luminosity is excluded exactly.

Consistency with `charge_misid_response_study_2026-10-08.md` (A-F3):

- **Cancels exactly:** `lumi_rel` (excluded by rule).
- **Largely cancels — ONLY after the per-bin charge-commonness audit
  (revision 1):** for every source claimed to "largely cancel" in `A_h`,
  decompose the per-bin charge-separated variation vectors `v±` into the
  common mode `(v+ + v−)/2` and the charge-asymmetric mode `(v+ − v−)/2`
  (cheap, per-bin, no new production); the claim is admissible only if the
  yield-weighted RMS of the asymmetric mode is ≤ 0.5 × that of the common
  mode (pinned), otherwise the asymmetric mode is propagated as an explicit
  `A_h` term. Under this audit: `c_rad_stat_rel` (one fixed MC sample feeds
  both charges' tables → charge-common multiplicative; the rank-1 form lets
  it cancel); `c_rad_gen_rel` (nearly charge-common: the h+ +0.6419% /
  h− +0.6499% figures are the raw integrated spread of the emfix pair,
  `DJ_int/RG_emfix_int − 1`, per the canonical budget README — a quantity
  DISTINCT from both the per-bin V3 smoothed component's yield-weighted
  integral and the pre-repair releasebin pair's raw cross-generator
  agreement h+ +0.4458% / h− +0.4539%; the two charges' spreads agree to
  0.008 pp); the tracking common mode (measured ρ +0.63 at 9.9σ, cell-median
  ρ +1.00, 7/7 sign agreement — but the PINNED diagonal form does not model
  it, so in the current form tracking does NOT cancel in A_h; the diagonal is
  conservative here ONLY because the measured correlation is POSITIVE — the
  general zero-coupling claim is retracted, §4.4); the resolved common modes
  of the detector legs (charge-coherence audit per §4.4).
- **Does NOT cancel — mandatory added term:** the **charge-misID unfolding
  bias** (A-F3). It is anti-correlated between charges by construction
  (`Delta_neg = −Delta_pos` identically; integrated h+ +0.335% / h− −0.344%;
  direction asymmetry Y/X ≈ 2.83), so it enters `A_h` at full amplitude:
  integrated `A` overstated by +0.0034 absolute (0.012922 → 0.009526, ~36% of
  the corrected value; h+/h− ratio inflated +0.68% relative). Per-bin
  magnitudes: median 0.0464%/0.0507%, max +1.669%/−4.558% — negligible for
  cross sections (0/490 bins exceed the per-bin budget; quadrature increment
  ≤ 0.18 pp; registered as a documented subdominant source, NOT a budget
  component), MATERIAL for `A_h`. Any `A_h` quote must carry it as an explicit
  per-bin systematic (±|bias| from `unfolding_bias_per_bin.csv`, Method 1) or
  use the identity-consistent correction; as a covariance it is rank-1 from the
  signed anti-symmetric per-bin vector (cross-charge anti-correlated → no
  cancellation; construction-wise this is the §4.4 branch-(c) anti-correlated
  form with a documented origin). MC-based caveat: RAPGAP31 charge-confusion
  model, unvalidated in data; biases scale ~linearly with the true misID rate.
- **Does NOT cancel — blocks any `A_h` release:** charge-dependent tracking
  (row 11, fully anti-correlated, 2.5%-on-`A_h` Aaron precedent, BLOCKED);
  the h+/h− statistical cross-block (ZERO BY ASSUMPTION, unmeasured — spec
  decision 11 / gate 15; measure via joint toys from the same data events);
  the **fake-fraction / background family** (revision 1): the release is
  raw/no-bgsub with a fixed response fake-fraction subtraction
  `d_sig = d·(1−f_i)` whose fractions are CHARGE-ASYMMETRIC
  (f+ = 10.7141% ≠ f− = 10.4019%) and whose uncertainty is NOT propagated
  (the stat toys hold it fixed) — an unquantified charge-asymmetric
  correction that cannot be assumed to cancel in `A_h`; any `A_h` quote is
  gated on the background decision chain (P-ALPHA, rows 17–19) closing or on
  an explicit propagation of the fake-fraction charge asymmetry;
  **trigger and NucliaWeight** (rows 15/16, BLOCKED, revision 1): per spec
  §4.3 a common trigger or common NucliaWeight cancels only if the paired
  charge-resolved variation DEMONSTRATES the common mode — no such
  demonstration exists, so no cancellation may be assumed for either.
- **Partially cancels — evaluate, never assume:** `ibu_model_rel`
  (charge-correlated ρ +0.51 at 8σ, cell ρ +0.87, 6/7 sign agreement — but not
  charge-common: cell medians −1.35…+1.13%); `ibu_reg_rel` (magnitude rank
  ρ +0.68; signed sidecar needed); `phi_h_gauss_rel` (unsigned envelope — no
  cancellation demonstrated; per-generator signed deltas exist for the
  evaluation); and the noise-floor parts of EVERY component (revision 1:
  their cross-charge correlation is UNMEASURED — the per-charge batteries
  measure per-bin spreads, not cross-charge noise correlation; no
  cancellation may be assumed, and their treatment is gated by the
  E31-family joint charge-resolved noise-correlation measurement, the same
  programme as the h+/h− statistical cross-block), bookkept separately in
  the varied-yield propagation.

**Consequence:** the asymmetry budget is NOT the 13/14-component per-bin
quadrature with zero cross-charge assumptions. It inherits: the covariance
assembly of §4 with the charge-resolved blocks, minus lumi, plus the misID
rank-1 term, with the stat cross-block measured and charge-dependent tracking
resolved. A-F3 remains in every promotion's does-not-cure list until the `A_h`
product exists.

## 6. Open items this design depends on

1. Signed `ibu_reg` sidecar emission (P1 companion; B2 machinery, no detector
   production).
2. Tracking baseline decision (delivered −0.5% ensemble vs frozen 1.0%⊕0.2%
   analytic; P1 item 4) — the Tier-A diagonal form is pinned either way.
3. `phi_h_gauss` disposition, option A vs B (gap audit §6.1) — governs whether
   a rank-1 phi_h form ever becomes constructible.
4. B1 follow-up attribution for the 4 failing bins (1 h+ / 3 h−;
   report-only).
5. `lumi_rel` derivation memo (gap audit §6.3; documentation-only).
6. h+/h− statistical cross-block measurement (joint toys; P3 item) —
   extended in revision 1 to the E31-family joint charge-resolved
   noise-correlation measurement, which also gates the §5 noise-part
   treatment for every component.
7. `response_mcstat_rel` 490-bin response-bootstrap covariance (blocked row 12).
8. The P2 paired campaigns themselves, each with its §2.1 battery; the
   classifier/enum/setter audits (JES/RCES jet classifier, `eHadEnRCES` enum,
   `eElecTrTh` co-shift) per gap audit §4.
9. C_rad publication-grade approval (P-CRAD-LUMI) for the c_rad terms' final
   status (chain-current; separate gate).

## 7. Spec conflicts and flagged deviations (explicit, none silently applied)

1. **Single-side fallback vs "block, not one-sided promotion"** (spec decision
   1, rule 3). This design emits a single-side VALUE (§3 case B) when one side
   is noise-dominated, so the budget stays interpretable; the spec's block rule
   is preserved as a STATUS gate (source PROVISIONAL; final requires the
   failed-side rerun or explicit approval). Value-vs-status distinction is
   deliberate; flagged because the spec text alone reads as "no number".
2. **The hybrid rank-1/diagonal form is new.** Spec decision 2 enumerates
   rank-1 `ΔΔ^T`, diagonal, and a measured replica covariance; §4.2 adds a
   fourth, noise-gated form (revision 1: SIGNED rank-1 on the resolved set as
   the default; the all-ones co-positive block survives only as a labeled
   conservative variant `all_ones_envelope`). It implements the spec's own
   evidence requirement ("`abs(Delta_i)` exceeds noise where rank-1 is used")
   and gate 7, but it is an EXTENSION: adopting it for a source requires the
   same promotion approval as the spec's forms; the pure forms remain
   available (diagonal = fallback; full-vector rank-1 = unvalidated envelope
   scenario, retained for comparison only).
3. **Toy-stat envelope demoted.** The dispatch suggested the toy-stat
   envelope as a noise reference; workspace measurements show it is wrong in
   both directions as a shift-noise scale (common data noise cancels in
   differences; measured floors 9–60× below the MAD/~ibu_stat scale, while
   sparse-tail floors exceed it). It is admitted only as a sufficient
   "resolved" condition (§2.1); all qualification uses the paired-replica
   measured floor + flip bound.
4. **Envelope candidates for same-side and mixed bins.** The mechanical rule
   writes max-side envelope candidates for `SA-EVEN` bins (candidate value)
   and, since revision 1, flag-gated candidates for `SA-MAG` and `SA-FLAT`
   bins (metadata only; default `u = |δ|` stands), but their promotion into
   final totals remains gated on the explicit approval the spec requires
   (rule 2); source-wide even structure routes to quarantine (decision 3),
   not into totals. The QA emits; the gate decides.
5. **Tracking: measured charge-common mode not modeled.** The spec pins the
   diagonal Tier-A tracking matrix; the measured ρ +0.63 / 7/7 cell sign
   agreement implies the common mode would largely cancel in `A_h` if modeled.
   The pinned form is kept (conservative for A_h GIVEN the measured positive
   correlation — the unconditional zero-coupling claim is retracted, §4.4);
   a correlated upgrade (rank-1 common mode ⊕ diagonal seed scatter) is
   flagged as a future A_h-focused decision requiring approval and the
   baseline decision (item 2).
6. **`c_rad_gen` rank-1 is an unsigned-envelope assumption** (mirrors the
   `c_rad_stat` house style), because the frozen V3 magnitude smooths `|δ|`
   (signed smoothing was explicitly rejected in the V3 recipe). A signed-shape
   rank-1 would be physics-preferred for integrated quantities but breaks
   diagonal closure with the frozen budget column; flagged as an alternative,
   not adopted. Revision 1 additionally FORBIDS the diagonal variant in any
   multi-bin/integrated assembly (§4.3 row 13): it understates the
   coherent-integrated variance, and the generator choice is one discrete
   coherent nuisance; the diagonal is strictly single-bin bookkeeping.
7. **`phi_h_gauss_rel` is outside the spec's 24-row register** (user-adopted
   2026-10-02, diagnostic status, disposition pending). Its diagonal treatment
   here confers no register or final status. Revision 1: the diagonal is
   INVALID for coherent-integrated use — single-bin bookkeeping only;
   integrated phi_h quotes require the option-B per-generator signed shapes
   (§4.3 row 12).
8. **Budget artifact currency.** The dispatch cited the
   `..._emfix_2026-10-09/` package; since the 2026-10-10 promotion the
   canonical artifact is `..._emfix_2026-10-10/` (CSV byte-identical,
   sha256-verified; the 10-09 package retained as provenance). All statements
   apply to both.
9. **Tier-A phrasing.** The dispatch's "Tier A = rank-1 fully correlated,
   e.g. lumi" is a simplification; the spec's Tier A also contains the
   measured (dense, non-rank-1) stat covariance and the pinned diagonal
   tracking matrix. This document keeps the spec's definitions.
10. **`c_rad_gen` "+0.6419%/+0.6499%" provenance (revision 1).** The critic
    verdict labels these figures "yield-weighted smoothed component,
    distinct from the raw +0.4458%/+0.4539% cross-generator agreement". The
    canonical budget README defines them arithmetically as the raw integrated
    spread of the emfix pair, `DJ_int/RG_emfix_int − 1` (verified:
    1.1415172920054786/1.1342370401351234 − 1 = +0.6419% h+;
    1.1410367377289135/1.1336689310022947 − 1 = +0.6499% h−), and the
    +0.4458%/+0.4539% figures as the pre-repair releasebin pair's raw spread
    on the same formula (1.1415172920054786/1.1364508941898332 − 1 = +0.4458%;
    1.1410367377289135/1.1358814928541603 − 1 = +0.4539%). This document
    applies the demanded provenance labeling and the distinction from
    +0.4458%/+0.4539% using the README-verifiable definition, and flags the
    critic's alternative provenance (yield-weighted smoothed-component
    integral) for verification rather than adopting it unverified — §5 keeps
    the three quantities (emfix-pair raw spread; pre-repair-pair raw spread;
    per-bin V3 smoothed component) explicitly distinct.

---

**Provenance consulted:** gap audit
`provisional_to_final_gap_audit_2026-10-10.md`; derivations
`budget_13components_derivation_2026-10-10.md`; spec
`FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`; noise evidence
`systematics_signal_vs_noise_2026-10-03.md` (§6, §8, §9 incl. P1 Phase A
floors, MC flip bound, B1/B2),
`systematics_smoothing_multidim_proposal_2026-10-02.md` (§2, S7);
covariance precedent `covariance_package_z005_2026-09-23.md`,
`z005_supplement_modelreg_coherentprior_2026-10-07/README.md`,
`covariance_cradstat/README.md` (emfix_2026-10-10 budget); component packages
`stat_toys_coherentprior_2026-10-07/`, `detector_shifted_coherentprior_2026-10-07/`,
`tracking_ensemble_coherentprior_2026-10-07/`, `phi_h_coherentprior_2026-10-06/`,
`p1_replica_floors_coherentprior_2026-10-07/`; A_h evidence
`charge_misid_response_study_2026-10-08.md`; runbook
`z005_shifted_systematics_pipeline_2026-06-13.md`; `STATE.md` (2026-10-10);
critic verdict PASS WITH CONDITIONS (2026-10-10) — revision 1 applies its
findings 1–8; the emfix-pair spread arithmetic re-verified against the
canonical budget README (`..._emfix_2026-10-10/README.md`, line 42).
