# Critic verdict — tracking-efficiency systematic (HQ-v5g-Bprime, 480-bin)

## Verdict

**CONCERN**

The implementation faithfully realizes a **common-mode per-track 0.5% systematic** with the correct rank-1 covariance and near-perfect A_h cancellation on the YIELD layer. The headline numbers (median fractional σ = 0.5%, n_same_side = 0, cov rank = 1, cov ≡ outer(δ,δ) to machine precision) are mathematically self-consistent with the stated method. **However, the systematic as built is physically incomplete for the published-physics use case stated by the supervisor**: it is the common-mode piece only; no charge-dependent residual is propagated; the per-event vs per-track propagation question for SIDIS multi-hadron events cannot be resolved from the supplied evidence; and several smaller code-correctness items are unverified because the actual reweight implementation in `source_tracking_efficiency.py` and `run_tracking_efficiency_systematic.py` was not provided.

The systematic is fit for the **yield/cross-section publication** with the recorded caveats. It is **NOT** by itself fit for the **A_h asymmetry publication** until a charge-dependent companion source is added.

## Material reviewed

- Observable: per-bin σ on unfolded charged-hadron yield N^{h±}(Q²,x,z,pT_h), 480-bin Scheme-B' 4D grid (2 charges × 8 (Q²,x) cells × 6 z × 5 pT_h).
- Channel: H1 charged-particle SIDIS, high-Q² (Q² > 150 GeV²), DJANGOH14 response, TUnfold-4D, τ = 0.01.
- Source ID: `tracking_efficiency`, claimed convention "DESY 20-176 §5: 0.5% per central primary-vertex track".
- Evidence read:
  - `data/systematics_sources/source_tracking_efficiency.json` — full content.
  - `data/systematics_sources/cov_tracking_efficiency.npz` — keys, shapes, and numerical structure (rank, diagonal, off-diagonal correlation, A_h Jacobian propagation, sign distribution, per-bin shift linearity).
  - Implementation source on NAF: **described in prompt, NOT directly inspected**.
  - Prior memo `research-tracking-systematics-deep.md`: **not present in local tree** (no file matches `**/*tracking*.md`); taken as supervisor-supplied context only.

## Fatal Issues

None at the YIELD/CROSS-SECTION level. The covariance build is mathematically what the stated method produces; the propagation is internally consistent; the numbers are not unphysical.

(See "Major Concerns" for the A_h-level concern that, if the asymmetry is also a publishable observable from this same systematic file, is analysis-killing for A_h.)

## Major Concerns

### M1. Missing charge-dependent tracking piece — analysis-killing for A_h

**Claim under criticism.** The supplied `cov_tracking_efficiency.npz` is the complete tracking-efficiency uncertainty for use on all observables including A_h.

**Evidence.**
- `source_id = "tracking_efficiency"`, `shift_kind = "per_track_weight_uniform"`, `shift_magnitude = 0.005`.
- The reweight described is a single flat factor (1 ± 0.005) applied identically to **every** matched MC reco hadron row, with no charge dependence.
- A_h Jacobian propagation through the rank-1 covariance:
  - cov ≡ outer(δ, δ) to machine precision (Frobenius diff = 0.0; rank = 1).
  - Per-bin fractional shift −δ/N_nom: median = 0.005000, p16 = 0.004759, p84 = 0.005052 → near-perfect common-mode in fractional terms across charges.
  - On the 207 well-populated (Np>0, Nm>0, |A| not tiny) bins, σ_A_h from this source has median ≈ 2.3 × 10⁻⁵ on |A| ~ 10⁻¹, i.e. fractional ~ 10⁻⁴. **As built, this source cancels A_h to ~10⁻⁴.**
- That is exactly correct **for the common-mode piece**. By construction it leaves nothing for a charge-dependent residual.

**Why it matters physically.** The supervisor's prompt states the H1 tracking-systematic decomposition has two pieces: a common ~0.5%/track that cancels in A_h, and a charge-dependent ~0.3%/track residual that does NOT cancel in A_h. This source file implements only the first. If the published A_h relies on this single source for "tracking" in its budget, the charge-asymmetry uncertainty from tracking is being reported as ~10⁻⁴ when the physical value is closer to √2 × 0.003 ≈ 4 × 10⁻³ per track-folded over yield ratio (order-of-magnitude — exact propagation depends on the residual model). That is one to two orders of magnitude underestimate on a source that does NOT cancel in the asymmetry.

**Status.** Analysis-killing for A_h **if and only if** A_h is a published observable and no companion source (e.g. `tracking_efficiency_charge_dep` or similar) exists. The supervisor confirmed in the prompt that the charge-dependent piece is "missing entirely from the analysis" — that needs to be resolved before A_h publication. For cross-sections / yields, this is not a problem: the common-mode piece IS the dominant tracking term.

**Decisive follow-up.** Confirm whether a separate `source_tracking_efficiency_charge_dep.json` exists in `data/systematics_sources/`. If yes, recheck this verdict. If no, this source must be supplemented before A_h is published.

### M2. Per-track vs per-event propagation in multi-hadron events — partially resolved by the data, but the propagation contract is undocumented

**Claim under criticism.** "Multiply each MC reco hadron row by (1 ± 0.005) and re-unfold" correctly propagates a *per-track* 0.5% systematic to a *per-bin* yield.

**Evidence.**
- The numerical signature (median fractional shift = 0.500% on yields, p84 = 0.507%, max = 8.65% only in sparse S3 bins) shows that, in the well-populated bulk, multi-track-per-event amplification does NOT inflate the per-bin shift above 0.5%. This is the physically correct behavior for an **independent per-track** efficiency uncertainty when the yield is *linear* in the per-track weight: dN_bin/N_bin = Σ_h dw_h / Σ_h w_h = ε (uniform), regardless of how many hadrons share an event.
- The numerator (acceptance-corrected hadron count) is linear in the per-track reweight. So the propagation through `N_h_corr` alone reproduces 0.5% as expected.
- HOWEVER: the multiplicity formula declared by the agent spec is `M^h = N_h_corr / (N_DIS_corr · Δz · Δp_T)`. If `N_DIS_corr` is computed from a DIS-event-level efficiency (not from hadron tracks), then `N_DIS_corr` is **not** moved by this reweight, and the yield-level 0.5% propagates directly into M^h at 0.5%. That matches what the data shows.
- BUT the agent spec also says: "no absolute hadron cross-sections until normalization is validated" and references "the published 1–5% on P(N) (multiplicity distribution) vs 0.5% on differential M(z, pT)" from §1.4 of the prior memo. This source file does not state, anywhere, which observable it is propagating to. It just reports σ on the unfolded yield `nominal_unfolded`. If a downstream consumer reuses this σ on a *multiplicity-distribution* observable P(N), the assumption "0.5% per-track → 0.5% on yield → 0.5% on M" breaks; P(N) involves N(N−1)/2 pair terms whose response to a per-track inefficiency is non-linear and can reach the published 1–5%.

**Why it matters physically.** A flat 0.5% systematic is the right answer for differential yields and for M^h = N_h/N_DIS-style multiplicities. It is the *wrong* answer for higher moments of the hadron multiplicity distribution (⟨N⟩, P(N), variance), and for any quantity built from N_h-on-N_DIS ratios where N_DIS uses a different track-efficiency model than N_h.

**Status.** Concern, not fatal, **for the yield observable that is the stated target here**. The data structure of the source (a yield vector + cov) implicitly contracts it for yield-level use only. The source needs an explicit `applies_to:` field naming the observable class, or downstream code must guard against misuse.

**Decisive follow-up.** Add an `applies_to` field in `source_tracking_efficiency.json` enumerating the observables this cov is valid for ("N^h±", "M^h(z,pT) where N_DIS is event-counted"); explicitly forbid reuse on P(N), ⟨N⟩, or variance/dispersion observables.

### M3. Reweight target and fiducial cut not verifiable from supplied evidence

**Claim under criticism.** "The MC reco rows multiplied by (1 ± 0.005) are exactly those tracks satisfying the H1-published fiducial: pT > 0.15 GeV, |η| < 1.3, central, primary-vertex."

**Evidence supplied.** Convention string in JSON only. The actual reweight code in `source_tracking_efficiency.py` was described in the prompt but not inspected. There is no diagnostic in the NPZ recording the per-event count of tracks that received the weight, no record of any track that was OUT of the fiducial getting the weight by mistake, and no record of any track INSIDE the fiducial being missed.

**Why it matters physically.** Two opposite mistakes are possible and equally invisible to the numerical signature:
1. **Over-application.** If the reweight also touches tracks at pT < 0.15 GeV or |η| > 1.3 (which the DESY 20-176 convention does NOT cover), the systematic is overestimated for any analysis bin that mixes in those tracks, AND it is being applied to tracks whose efficiency uncertainty is documented to be different (and larger) than 0.5%.
2. **Under-application.** If the analysis BASE_CUT for tracks entering the SIDIS sample is *looser* than the fiducial used to derive the 0.5% (e.g. extends to |η| > 1.3 or to pT < 0.15 GeV), then this source UNDERESTIMATES the tracking systematic on the looser bins because the published 0.5% does not cover them.
The numerical "median = 0.500%" cannot distinguish these cases — it would look identical in both because every track gets the same weight by construction.

**Status.** Concern. The numerical answer is consistent with the stated method; the stated method is consistent with the published convention; but the link between "tracks in the analysis selection" and "tracks the published 0.5% applies to" is asserted, not demonstrated.

**Decisive follow-up.** Add to the JSON a diagnostic block: number of tracks reweighted per period, fraction with pT < 0.15 GeV, fraction with |η| > 1.3, fraction with IsCentralTrk == 0 or IsFromPrimary == 0. All four numbers must be 0 (or the source must explicitly cover the looser fiducial with a different magnitude).

### M4. The "negative-nominal" bins distort fractional σ as advertised but ALSO break the sign of δ

**Claim under criticism.** "Pre-existing 33 negative-N bins distort fractional σ but should not affect the covariance structure."

**Evidence.**
- 33 bins with `nominal_unfolded < 0` (TUnfold regularization undershoot in sparse cells), as reported in the JSON.
- δ has 450 negative and 30 positive entries. The expected physical sign for a reweight that multiplies MC reco by (1+0.005) is δ < 0 in all bins (if MC predicts more reco hadrons, the unfolded truth must drop to match the same data marginal; therefore N_unf_up < N_unf_dn and δ = (up−dn)/2 < 0). The 30 positive-δ bins are concentrated on negative-nominal bins or on the saturating boundary cells.
- More importantly: in 465 of 480 bins, sgn(δ) ≠ sgn(N_nom). So **σ_i = |δ_i|** does not preserve the geometric meaning "the shift is in the direction of |N|"; in the 33 negative-nominal bins the fractional σ_i / N_i is *negative* even though σ_i / |N_i| reads as ~0.5%. The cov build outer(δ, δ) then carries the **wrong relative sign** between negative-N and positive-N bins — those bin pairs get correlation +1 even though physically the systematic moves them in opposite *absolute* directions.

**Why it matters physically.** For pure cross-section reporting it does not matter — yields and uncertainties are reported with sign and propagation is linear. For ANY downstream that builds a fractional shift or a ratio across bins (e.g. a slope vs z, a moment, a charge asymmetry that includes one of these 33 cells), the correlation +1 between a negative-N bin and a positive-N bin understates the variance because their physical shifts have opposite sign in the observable. In particular, A_h cancellation will be *over-claimed* on bin pairs where one of the two (Np, Nm) is in the negative-N set: my A_h Jacobian scan reports σ_A_h ≈ 8.7 on the bin pair (231, 471) where Np ≈ −1131 and Nm ≈ +1168 — i.e. A_h on a negative numerator is meaningless. The cov returns a sigma but the underlying yields are not yet physical.

**Status.** Concern, not fatal, **provided** these 33 bins are excluded from final reporting (as is standard for TUnfold undershoot). If they leak into a published table or into a χ²/covariance combination, the rank-1 cov will mis-correlate them with their positive-N neighbors.

**Decisive follow-up.** Add a `bin_mask_recommended` array to the NPZ flagging the 33 bins with N < 0 as "drop or floor". Verify the downstream summarizer respects it.

## Non-Blocking Checks Confirmed

- **Covariance is exactly rank-1.** `np.linalg.matrix_rank(cov) == 1`. Frobenius norm of `cov − outer(δ, δ) == 0.0` (machine zero). `sigma == |delta|` confirmed bit-exact. Off-diagonal `|corr| == 1` for all 480·479/2 entries. The "fully correlated rank-1" claim in the JSON is structurally correct.
- **Shift linearity is verified.** up/nom has median 0.995025, p16/p84 of (0.994973, 0.995264). dn/nom has median 1.005025, p16/p84 of (1.004782, 1.005077). The shift behaves linearly to ~10⁻⁴, as expected for a 0.5% reweight that is small compared to nominal acceptance corrections.
- **(up+dn)/2 − nom is consistent with zero up to TUnfold regularization noise.** Max |mid| = 17 yield units, median fractional |mid/nom| = 2.5 × 10⁻⁵. This means the up/down shifts ARE symmetric to the precision of the unfolder, validating the central-difference δ = (up − dn)/2 and showing the 0.05 yield-unit "median up/down asymmetry" reported in the JSON is genuinely negligible.
- **n_same_side = 0 verified.** I recomputed (up − nom) and (dn − nom): zero bins have both shifts on the same side of nominal. This is the correct monotonic behavior expected from a linear reweight on a well-posed unfolder. (Contrast with the electron-systematics output noted in the prompt at ~50%, where it indicated non-monotonic response.)
- **A_h cancellation works on well-posed bins.** On the 207 bin pairs where both Np and Nm are positive and |A| is well-defined, σ_A_h from this source has median 2.3 × 10⁻⁵ and p84 8.4 × 10⁻⁴ in absolute A_h units. That is consistent with the residual TUnfold regularization mismatch between the up/down responses, and is physically negligible compared to statistical A_h uncertainties of O(10⁻²). The cov structure DOES cancel A_h correctly for the common-mode piece, as claimed.
- **Q²-stratum monotonicity.** Σ|σ|/Σ|N| in the JSON: S0 = 0.500%, S1 = 0.500%, S2 = 0.459%, S3 = 0.396%. The drop in S2/S3 is consistent with stronger TUnfold regularization in sparse high-Q² bins suppressing the integrated shift below 0.5%; this is a known TUnfold τ-dependent effect, not a bug.

## Missing Evidence

To upgrade verdict to SOUND (yield-level) and to make a verdict on A_h-level:

1. **Reweight code:** the actual `source_tracking_efficiency.py` source so the fiducial cut applied to the reweight can be compared to the analysis BASE_CUT. Specifically the line that selects which MC reco rows get the (1 ± 0.005) factor.
2. **Companion source for charge-dependent piece:** either `data/systematics_sources/source_tracking_efficiency_charge_dep.json` (or equivalent) or explicit confirmation that this is being tracked as a separate, still-to-be-built systematic.
3. **`applies_to` declaration:** a contract field in the JSON listing exactly which observable classes the cov is valid for (yields, M^h(z, pT), charge-summed cross-sections) and which it is NOT valid for (P(N), ⟨N⟩, moments).
4. **Fiducial-applied diagnostic:** per-period count of tracks reweighted vs total tracks in the analysis sample, broken down by pT and |η| bins crossing the 0.15 GeV and 1.3 thresholds.
5. **Source memo:** the cited `research-tracking-systematics-deep.md` could not be located in the local tree. The quantitative "common ~0.5% / charge-dep ~0.3%" decomposition is taken on supervisor's authority only. The source citation in the JSON ("DESY 20-176 §5") should be supplemented by the exact in-text quote and table reference; §5 of arXiv:2011.01812 covers tracking efficiency at high-Q², but the numerical 0.5% should be tied to a specific equation or table.
6. **Bin-mask handling for negative-N bins:** confirmation that the 33 negative-N bins are excluded from the final cov used in downstream χ²/combinations, or that the rank-1 cov is rebuilt after a mask is applied.

## Recommended Next Check

Inspect `H1ChargedSidis_HighQ2/scripts/systematics/source_tracking_efficiency.py` directly. Specifically: (a) confirm the fiducial-cut filter on the MC reco row before applying (1 ± 0.005), (b) confirm the reweight is applied to the **same** track collection that enters the nominal response build, and (c) confirm no second-pass multiplication or denominator effect on N_DIS. If those three items pass, this source becomes SOUND for yield/cross-section publication. Independently, decide whether A_h is in scope and, if so, commission `source_tracking_efficiency_charge_dep` (a 0.3%/track, charge-anti-correlated reweight) as a *separate* source whose cov will have a different correlation structure (h+ and h− anti-correlated, NOT rank-1 across charges).
