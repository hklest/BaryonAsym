# Critique — 210-bin high-Q² charged-SIDIS binning freeze candidate

- **Date:** 2026-06-02
- **Critic:** `h1-analysis-critic` (read-only, adversarial)
- **Scope:** Audit whether `data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/` can be declared the **absolute final frozen output binning** for the high-Q² charged-particle SIDIS analysis.
- **Method:** Direct inspection of release JSON/CSV files, the per-charge per-bin σ rev6-area source JSONs that the 210-bin release stitches together, the S3 multi-objective summary, the 2026-06-01 FATAL rev6 critique, the 2026-06-02 closure-tauopt result, the 2026-06-02 low-pT-refined exploratory result, the Phase 1 systematics handoff, and the binning-freeze 2026-05-24 methodology memo.
- **Verdict:** **FATAL** for the proposed binning freeze in its current form.

The (Q², x, z, pT_h) **edge structure** (HQ-v5g-Bprime: 4 Q² × 2 x × 6 z × 5 pT for rev6 cells 0..5; S3 x-merged) is physically reasonable and the right qualitative response to the S3 instability finding. The release **mask + filtering** are honest book-keeping. The fatal issues are not the edges. They are:

1. The unfolding the release sits on top of was given a FATAL verdict 24 hours before the release was issued, with seven listed fatal issues — none of which have a corresponding "resolved" follow-up artifact in `docs/`.
2. The 30 S0/S1/S2 "merge candidate / negative / no-support" bins that the post-hoc 200 % filter culls are **not random low-stat corners**: they are a structured high-z pathology that points to a binning-and-unfolding mismatch the freeze has not addressed.
3. "Freeze 210" effectively freezes the 200 % post-hoc filter together with the binning. Per current numbers that filter removes 36 bins (h+) and 39 bins (h−) — i.e. **17 % – 19 % of the proposed binning is unpublishable** at freeze time. A binning that fails ≥ 17 % of its own cells is not a final binning.

The detailed evidence is below.

---

## 1. The 210-bin release is built on the rev6 TUnfold setup that was rejected FATAL on 2026-06-01

The release stitches together two TUnfold products:

| Block | Bins | Source file | τ used |
|---|---|---|---|
| Cells 0..5 (full x-differential) | 0..179 (180 bins) | `data/per_bin_sigma_tunfold_rev6_area/per_bin_sigma_tunfold_rev6_area_{pos,neg}.json` | pos: **2.154 × 10⁻⁵**, neg: **4.642 × 10⁻⁵** |
| S3a + S3b merged in x | 180..209 (30 bins) | S3 multi-objective Area selection | pos: 3.162 × 10⁻³, neg: 1.000 × 10⁻³ |

The τ used for the 180 bins of the rev6 cells block is identical to the supervisor-recommended τ in the rev6 τ-scan summary that `docs/critiques/critique-tunfold-rev6-tau-scan-2026-06-01.md` rejected as FATAL the day before this release was built. Direct quotes of the fatal findings against that τ:

> "F6. `pathology_flag` and `hard_reject` are TRUE at every recommended τ ... pos rec. (τ=2.15e-5): pathology_flag=true, hard_reject=true; neg rec. (τ=4.64e-5): pathology_flag=true, hard_reject=true."

> "F7. Negative-bin rate is 10% at recommended τ ... pos rec. τ=2.15e-5 → n_negative_pub = 23/240 = 9.6%; neg rec. τ=4.64e-5 → n_negative_pub = 24/240 = 10.0%."

I re-checked the source files directly:

| File | tau_nominal_used | n_unfolded ≤ 0 (rev6 240-bin grid) |
|---|---|---|
| `per_bin_sigma_tunfold_rev6_area_pos.json` | 2.15443469 × 10⁻⁵ | 23 / 240 |
| `per_bin_sigma_tunfold_rev6_area_neg.json` | 4.6415888 × 10⁻⁵ | 24 / 240 |

So the τ and negative-bin rate at the time of release are **exactly the values that the FATAL critique rejected**. The 210-bin release does not cite or address the FATAL verdict anywhere in `data_release_rev6_TUnfold_AREA_210bin_FILTERED_INTERNAL_PRELIMINARY/README.md` or `release.json`.

The new `docs/results/tunfold_rev6_closure_tauopt_2026-06-02.md` actually selects very different τ via a closure-driven multi-objective score (pos τ = 1.00 × 10⁻⁶, neg τ = 2.59 × 10⁻⁶, both ~ 20–50× *lower* than the τ in the release). **Those closure-selected τ have never been propagated into a 210-bin release.** The 210-bin release therefore freezes the binning around an unfolding configuration that the latest closure study has implicitly superseded.

Additionally, none of these earlier rev6 FATAL items have a documented resolution:

| Critique item (2026-06-01) | Resolution found in current `docs/`? |
|---|---|
| F1 σ_reg evaluator broken (`n_trusted_used = 0`) | No follow-up artifact found. |
| F2 effective rank ≤ 4 / cond# ~ 1e17 | No follow-up artifact found; closure tauopt summary still does not publish `effective_rank` on its chosen τ. |
| F3 refold χ²/ndf ≥ 234 everywhere on nominal scan | Closure tauopt reports self-closure refold χ²/ndf ≈ 1e-4 (suspiciously good — consistent with kEConstraintArea forcing the fit) and cross-closure refold χ²/ndf 24.7 – 31.6, not the data-refold of the nominal MC scan that F3 was about. F3 is **not** addressed. |
| F4 `unfolded_total` = `data_total_with_overflow` to 12 sig fig under kEConstraintArea | No follow-up; still kEConstraintArea per release.json. |
| F5 reco_eff > 1 in trust mask (max 7×10⁴ neg) | Trust-mask definition in the per_bin_sigma source files now reads `untrust if mc_count_rapgap<50 OR (mc_count_rapgap<500 AND reco_eff<0.05) OR reco_eff>1`. That is a partial F5 fix (the `OR reco_eff>1` clause was added), but the response matrix itself was not rebuilt — the operator that produced those pathological efficiencies is still the operator that computed the published unfolded values, only the trust labelling changed. |
| F6 hard_reject flag at recommended τ | Closure tauopt picks a different τ that is not hard-rejected, but the 210-bin release does not use those τ. |
| F7 10 % negative bins | "Solved" administratively by the 200 % post-hoc filter (see Section 3). Underlying negatives still occur in the unfolded vector that propagates into systematics. |

For a freeze candidate, the expected paper trail is: FATAL critique → response artifact → new critique → SOUND/CONCERN. The first arrow is missing for items F1, F2, F3, F4. The freeze cannot be defensible until that chain is closed.

---

## 2. The pattern of unpublished bins reveals a physics-level high-z failure, not low-stat corners

Per-charge unpublished breakdown (recomputed directly from `unfolded_yields_210_filtered_{pos,neg}.csv`):

| Status | h+ | h− |
|---|---|---|
| `published` | 174 | 171 |
| `merge_candidate_large_uncertainty` (σ_total/|unf| > 200 %) | 8 | 10 |
| `not_reported_negative_unfolded` | 15 | 13 |
| `not_reported_zero_data` | 8 | 10 |
| `not_reported_no_response_support` | 5 | 6 |
| **unpublished total** | **36 / 210 (17.1 %)** | **39 / 210 (18.6 %)** |

Where do these unpublished bins live? Almost all of them are at **z ≥ 0.5**, concentrated in z=[0.5,0.7] and z=[0.7,1.0], spread across **all four Q² strata** (S0, S1, S2, and the S3-merged block).

Examples that are not "low data, expected garbage":

| Bin | (Q², x, z, pT) | charge | data_count | unfolded |
|---|---|---|---|---|
| 51 | S0, x[0.0021,0.0071], z[0.5,0.7], pT[0.5,1] | h− | **408** | **−245** |
| 50 | S0, x[0.0021,0.0071], z[0.5,0.7], pT[0,0.5] | h+ | 192 | −93 |
| 110 | S1, x[0.0042,0.016], z[0.5,0.7], pT[0,0.5] | h+ | 77 | −176 |
| 110 | same bin | h− | 57 | −53 |
| 173 | S2, x[0.0099,0.034], z[0.5,0.7], pT[2,4] | h+ | 72 | −463 |
| 34 | S0, x[0.0021,0.0071], z[0.0,0.1], pT[4,8] | h+ | 217 | −165 |

These are bins with **hundreds of data tracks** that the unfolding pushes negative by hundreds. This is the rev6 cross-closure pathology (cross-closure p90 = 66 – 88 % per `tunfold_rev6_closure_tauopt_2026-06-02.md`) showing up as negative central values once the response is applied to real data. They are not "isolated outliers" — they form a **systematic high-z band** across S0, S1, S2.

Two consequences for the freeze argument:

- The 200 % filter is **not absorbing low-stat noise**. It is absorbing a **physics-region failure mode** (high z) that the binning itself has not been redesigned to handle. Freezing the binning before deciding whether high-z is merged in (z, pT), or merged across Q² like S3 was merged in x, or simply dropped from the publication grid, **freezes a known failure mode into the published structure**.
- The asymmetry between h+ and h− in the *same* (Q², x, z, pT) bin (e.g. bin 116: h+ unfolded ≤ 0; h− merge_candidate with rel σ ≈ 55) is the smoking gun that the failure is statistical / regularization-driven, **not** an intrinsic property of the bin edges. Two independent regularized fits to the same kinematic region disagree at the structural-pass/fail level. That is not the signature of a fully resolved binning.

---

## 3. Merge candidates are not merged. The 200 % filter is not a binning decision

The release policy is:

> publish iff data_count > 0, unfolded > 0, support_mask true, and all of sigma_stat/model/total ≤ 200 % of |unfolded|.

`merge_candidate_large_uncertainty` bins (8 h+, 10 h−) carry `σ_total/|unf|` from **2.1× up to 88.6×**. Concretely (pos):

| Bin | source / stratum | z | pT | unfolded | σ_total | rel σ_total |
|---|---|---|---|---|---|---|
| 140 | rev6, S2 | [0.5,0.7] | [0,0.5] | 0.47 | 41.96 | **88.6** |
| 171 | rev6, S2 | [0.5,0.7] | [0.5,1] | 37.9 | 708 | **18.7** |
| 57 | rev6, S0 | [0.7,1.0] | [1,2] | 22.6 | 308 | **13.6** |
| 59 | rev6, S0 | [0.7,1.0] | [4,8] | 15.7 | 70.4 | 4.5 |
| 86 | rev6, S1 | [0.7,1.0] | [0.5,1] | 15.1 | 62.4 | 4.1 |

`rel_sigma_total > 1` already means the unfolded central value is not statistically distinguishable from zero. Values of **18× and 88×** mean the bin carries no information at all.

This is not a "merge candidate", it is an **unrecoverable bin**. The release flags it without acting on it. The policy says it will be merged later. But:

- The release `README.md` does not specify the merging algorithm (along which axis, with which neighbour, with what weighting, whether covariance is preserved).
- The lowpt-refined exploratory document (`lowpt_refined_binning_v1_2026-06-02.md`) explicitly demonstrates that **changing the gen binning changes the per-bin uncertainties non-trivially** (refining pT made things worse). The converse (merging pT or z bins) will likewise change σ_stat / σ_reg / σ_model in non-target bins, because the response matrix and L-matrix change.
- Therefore, "decide later what to merge" is **a binning decision that has not been made yet**. A binning is not frozen while one in six cells is still pending a structural merge.

The honest accounting is:

| Strict bin count | h+ | h− |
|---|---|---|
| As-stated frozen | 210 | 210 |
| Effectively publishable today | 174 | 171 |
| Pending structural merge / drop | 36 | 39 |
| Fraction in flux | 17 % | 19 % |

A "210-bin binning" that is `(174 + 36 TBD) / (171 + 39 TBD)` per charge is not a frozen binning. It is **a frozen edge structure plus an unfrozen masking layer**, and the masking layer is where ≥ 17 % of the eventual published bins live.

---

## 4. The S3 strategy (x-merged, z ≥ 0.5 outright-masked) is internally documentable but partially incoherent

What is defensible:

- **S3a / S3b cannot be unfolded independently at this binning.** The multi-objective scan shows both are hard-rejected at every τ under both Area and None constraints (`c6 (S3a)/pos/None hard_reject=true`, `c6 (S3a)/neg/None hard_reject=true`, `c7 (S3b)` always hard_reject=true except in degenerate operating points). Merging in x is the correct response.
- **z ≥ 0.5 masked outright in S3 merged**: with effective_rank ~ 1.3 (pos) / 2.0 (neg) on the 30-bin merged solution and `n_neg_zhi` non-zero, masking the upper z half is the conservative move.

What is *not* defensible:

- **Asymmetric high-z treatment between strata.** S3 high-z is masked at construction; S0/S1/S2 high-z is *not* masked at construction, only filtered post-hoc on a 200 %-rel-σ rule. Same physics region, different policy. Pick one: either (a) mask z ≥ 0.5 across all strata when the unfolding cannot resolve it, or (b) build an honest stat-driven mask criterion applied uniformly. The current asymmetry will be very hard to defend in an analysis-note table footnote.
- **Per-charge τ at the same bin grid.** S3 merged uses τ_pos = 3.16 × 10⁻³, τ_neg = 1.00 × 10⁻³ (factor 3 apart). Rev6 cells 0..5 use τ_pos = 2.15 × 10⁻⁵, τ_neg = 4.64 × 10⁻⁵ (factor 2 apart). The closure-tauopt-selected τ have yet another ratio. Per-charge τ is admissible, but the *cross-charge consistency* of any derived ratio observable (A_h, A_n, A_BC) is sensitive to the τ pair; that sensitivity is not in the release's σ_model and has not been quantified.
- **Refold χ²/ndf of the S3 merged operating points** is 53.6 (pos Area), 64.1 (neg Area), 113.7 (merged-pos Area), 92.7 (merged-neg Area). These are not numbers consistent with "this τ measured the cross section". They are consistent with "this τ is the best of bad options". That is a legitimate engineering verdict, but for a *freeze*, the analysis note will have to either re-cast the S3 product as shape-only or explicitly state the refold inconsistency. Neither is done.

S3 is defensible *as a non-publication block* with an attached caveat. It is not defensible *as a frozen 30-bin block that will become part of the final published table* without that caveat being written into the release.

---

## 5. The low-pT refinement rejection is correct on the evidence presented, but not for a strict freeze

The exploratory `lowpt_refined_binning_v1_2026-06-02.md` study reaches a defensible negative verdict: refining pT to 7 gen bins gives:

- rank deficit ~ 12 % (rank ≈ 259 / 294),
- inner-vector / overflow oscillation that the canonical 210-bin design does not exhibit at the same scale,
- per-bin σ degraded in the *target* low-pT region (13–19 % negative bins vs 4–7 % at the canonical pT binning),
- χ²A/dof ≈ 300 and Cholesky failure inside TUnfold.

So the **direction of the conclusion** ("don't refine pT under the current intra-cell-only L matrix") is defensible.

What the study **does not** show, and what would be needed before "low-pT refinement is rejected" can support a *freeze*:

- It does not test cross-cell L coupling (its own recommendation 1). The conclusion is that this *particular* refinement under the *current* L is bad — not that no useful refinement exists.
- It uses uniform-spreading of the canonical 240-bin bg-subtracted JSON across the refined 840-bin reco axis. The study itself flags this as a strong assumption (recommendation 3). A genuine refinement attempt would re-extract data from the per-track CSV.
- It is run on a single τ representative (2.07 × 10⁻⁶) without the full L-curve / closure / rho-correlation suite that the canonical design uses.

So the rejection is *operationally* correct for the freeze-now decision, but it is **not** a structural proof that the canonical pT binning is the only viable choice. For a final-binning freeze, the language must be "low-pT refinement deferred pending a redesign of L and a per-track-CSV re-extraction" rather than "rejected." This affects how the binning is described in the analysis note.

---

## 6. Missing systematics are not blockers for the *edge* freeze, but they are blockers for the *masking-layer* freeze

You list six families not yet evaluated: fake model, hadron mass, NW (NuclIA weights), VtxZ, MC stat, charge confusion.

| Systematic family | Affects edge structure? | Affects merge/mask decisions? |
|---|---|---|
| fake-model variation | No directly | **Yes** — fake-subtraction provenance feeds the data row that TUnfold is constrained to. F4 of the rev6 critic is partly a statement about this. Could move bin populations across the 200 % cutoff. |
| hadron-mass scheme (z definition) | **Yes** — z edges shift in cells where the m_h ≠ 0 vs m_h = 0 assignment differs near the z-bin walls. Should be checked before freeze. | Yes |
| NuclIA-weight uncertainty | No | **Yes** — local-reweight systematic feeding all per-bin σ. Will change which bins fall under 200 %. |
| VtxZ shifted-extractor | No (event cut) | Marginal |
| MC stat (response-matrix bootstrap) | No | **Yes** — directly enters σ_MC-stat already documented in the v3 contract. Will increase σ_total in low-stat cells and push more bins above 200 %. |
| Charge confusion | No | **Yes for A_h / A_BC**; will modify pos/neg pairing for ratio observables. |

So: the *edge structure* is mostly insensitive to the missing systematics with the partial exception of the z-mass convention check. But the **set of cells that pass the 200 % filter** is sensitive to **fake-model, NW, MC-stat, and charge-confusion** systematics. Adding any of these will move the count of `merge_candidate` and `negative` bins. Since the "210-bin binning" includes the masking layer as a fact about which 174 / 171 bins are real, freezing the binning before the systematics finish is freezing under partial information about where the 200 % threshold lands.

Concretely: the σ_model in the release is `0.5 × |unf_RG − unf_DJ|` only. Adding MC-stat-bootstrap σ alone is known from `tunfold_rev6_closure_tauopt_2026-06-02.md` cross-closure p90 to be O(60–90 %) on a non-trivial minority of bins — easily enough to drag additional bins across the 200 % threshold and shrink the publishable count below 174 / 171.

---

## 7. Verdict and required actions

### Verdict

**FATAL** for "the 210-bin binning is absolutely final and frozen as of today."

The edge structure (Q², x, z, pT_h edges; cells 0..5 full; S3 x-merged) is defensible as the *intended* publication grid. The freeze decision fails on three independent grounds:

1. The underlying TUnfold τ in cells 0..5 was rejected FATAL on 2026-06-01 with seven listed issues, four of which (F1, F2, F3, F4) have no documented resolution. The closure-tauopt selection that *would* address some of them (F6, F7 partially) has never been propagated into a release.
2. ≥ 17 % of the bins in the proposed binning are not publishable today and the merge / mask policy that will decide their final fate has not been written. The freeze defers a structural decision into an undefined masking layer.
3. The high-z failure pattern across S0, S1, S2 has the same shape as the S3 failure that motivated x-merging, but the policy response is asymmetric (S3 masked at construction, S0/S1/S2 filtered post-hoc on a 200 % cutoff). This is inconsistent and not analysis-note-defensible as written.

### Fatal Issues

- **F-bin-1.** Release built on the rejected rev6 area-constrained τ-scan (τ_pos = 2.15 × 10⁻⁵, τ_neg = 4.64 × 10⁻⁵). Same τ that critic explicitly flagged `pathology_flag=true, hard_reject=true`. Negative-bin rate in the underlying 240-bin grid is still 23/240 (pos) and 24/240 (neg).
- **F-bin-2.** No follow-up artifact for the 2026-06-01 critique items F1 (broken σ_reg evaluator), F2 (effective rank ≤ 4 / cond# ~ 1e17), F3 (refold χ²/ndf ≥ 234), F4 (`unfolded_total ≡ data_total_with_overflow` under kEConstraintArea).
- **F-bin-3.** Closure-tauopt re-selected τ (pos 1e-6, neg 2.59e-6) on 2026-06-02 with *different* criteria, but the 210-bin release was not re-built at those τ. There are two incompatible "canonical" τ in the workspace at freeze time.
- **F-bin-4.** Merge candidates with rel σ_total up to 88× are not merged in the release. The merge algorithm is not specified anywhere in the release directory.
- **F-bin-5.** High-z (z ≥ 0.5) failure is treated by two different policies in different Q² strata (S3 masked at construction; S0/S1/S2 post-hoc filtered). No consolidated rationale.

### Major Concerns

- **M-bin-1.** Per-charge τ for both blocks; cross-charge τ ratio (2× – 3×) is not folded into σ_model. A_h, A_n, A_BC ratio observables will inherit this uncovered systematic at freeze.
- **M-bin-2.** σ_model uses only `0.5 × |unf_RG − unf_DJ|` at the chosen τ. It does not include the cross-closure tails (`tunfold_rev6_closure_tauopt_2026-06-02.md` reports cross-closure p90 of 66–88 % per charge); the user statement that "cross tails to be carried as model/unfolding systematic" is **not** present in the release JSONs.
- **M-bin-3.** The reco_eff > 1 entries that contaminated the 2026-06-01 critic's trust mask are now only **labelled** untrusted in the per-bin σ sources via the new `OR reco_eff>1` clause. The **response matrix** was not rebuilt. The unfolded values for the trusted bins were computed using the same operator that produces 7×10⁴-magnitude reco efficiencies elsewhere. The freeze depends on the assumption that this contamination does not bleed into the trusted set; that assumption has not been audited at release time.
- **M-bin-4.** kEConstraintArea remains in force. The release inherits F4: the integrated total of the unfolded vector is a forced quantity, not a measurement. This needs to be either explicitly stated as a shape-only result in the release README or rebuilt under kEConstraintNone before freeze.
- **M-bin-5.** Six systematic families (fake-model, hadron mass, NW, VtxZ, MC stat, charge confusion) are not in the release. The first, third, fifth and sixth are individually capable of shifting bins across the 200 % cutoff and changing the publishable bin count.
- **M-bin-6.** The 200 % cutoff has no physics justification cited. A 200 % cutoff lets through bins with σ_total = 1.99 × |unf|, i.e. central values that are not even 1σ away from zero. This is administratively convenient but is an unusually loose default for a "publishable" cell. A defensible freeze should publish only cells with rel σ ≤ some explicitly justified threshold (e.g. 50 %, 100 %).

### Non-blocking Confirmations

- File structure (manifest, SHA-256, JSON/CSV pair) is clean and reproducible.
- The 4-Q²-stratum × split-x edge design (Scheme B′) is consistent with the 2026-05-24 binning freeze memo.
- S3 x-merge as a single 30-bin block is internally consistent and physically motivated by the multi-objective scan.
- Rejection of the 294-bin low-pT refinement is operationally correct for the freeze-now question; the study's negative verdict is properly documented.
- Trust-mask definition now contains the `reco_eff>1` clause, which is a partial response to the 2026-06-01 F5 finding.
- Status-counts are honestly published per charge, not hidden.
- Self-closure metrics at the closure-tauopt-chosen τ are excellent (median p50 ~ 1 %, p90 < 5 %); the question is only whether the *release* uses those τ. Today it does not.

### Required Actions Before Freeze

1. **Publish a written response artifact** to the 2026-06-01 FATAL critique covering items F1, F2, F3, F4, F5, F6, F7 explicitly. Mark each as RESOLVED / DEFERRED / ACCEPTED-AS-LIMITATION. Without this, the freeze inherits an open FATAL.
2. **Rebuild the 210-bin release at the closure-tauopt-selected τ** (pos 1.00 × 10⁻⁶, neg 2.59 × 10⁻⁶) or document why those τ are *not* preferred over the τ in the current release. The workspace cannot have two "canonical" τ at freeze.
3. **Specify and apply the merge algorithm** for the merge_candidate bins before the freeze, not after. Either:
   (a) merge in pT within fixed (Q², x, z) cells until rel σ_total ≤ threshold,
   (b) merge in z within fixed (Q², x) cells, or
   (c) treat all `unpublished` bins as a single combined "high-z residual" block per stratum.
   Whichever is chosen, the **post-merge bin count is the frozen binning**, not 210.
4. **Unify the high-z policy across Q² strata.** Either mask z ≥ 0.5 in S0/S1/S2 the same way S3 is masked, or unmask z ≥ 0.5 in S3 with the same 200 % filter. Asymmetric treatment is not defensible in an analysis note.
5. **Replace the 200 % cutoff with a physics-justified threshold** (recommend ≤ 100 %; ideally a per-bin signal-to-uncertainty argument that ties to the analysis observable). Document the choice in the release README.
6. **Add MC-stat-bootstrap σ and the cross-closure tail (the σ_model from `tunfold_rev6_closure_tauopt_2026-06-02.md` cross-modes) to σ_total before applying the cutoff.** Otherwise the cutoff is being applied to an incomplete σ_total and will need to be re-applied once systematics land — meaning the bin count changes again post-freeze.
7. **Decide and document whether this is a shape-only release or an absolute-normalization release.** Under current kEConstraintArea, the integrated total carries no information independent of `data − fakes + miss-overflow`. If the deliverable is absolute multiplicity, drop the area constraint and re-run; if it is shape only, say so in the release README and propagate the caveat into the analysis note.
8. **Audit the response matrix for the reco_eff > 1 source.** The trust mask now hides those cells, but the matrix that produced the unfolded values is unchanged. Either rebuild the response or document that the bleed into trusted cells has been quantified and bounded.
9. **Hadron-mass / z-convention check.** Confirm the z bin edges are stable against the m_h treatment in the extractor before the z-edge freeze.
10. **Run a single sanity systematic that is known to be analytic** (e.g. luminosity ±2.3 % or tracking-eff common 2%/track per Phase 1 plan) **through the 210-bin pipeline** before freeze to confirm the contract holds end-to-end and the 200 % filter does not move under it.

### Recommendation

**Do not freeze yet.**

The right next steps are, in order:

- (a) Resolve the open FATAL critique items in a written follow-up.
- (b) Rebuild the 180-bin rev6 cells block at the closure-tauopt τ to bring the two competing τ choices into one canonical product.
- (c) Specify the merge algorithm and apply it; declare the *post-merge* count as the frozen number.
- (d) Add MC-stat and cross-closure tail to σ_total before re-applying a justified-threshold publication cutoff.
- (e) Then freeze. A realistic frozen binning count under this sequence is likely ≈ 170 – 180 bins per charge, *not* 210.

A more pragmatic "freeze with caveats now" is **not** recommended here, because the caveat list (open FATAL items, two competing τ, unspecified merge policy, asymmetric high-z policy, incomplete σ_total) is exactly the kind of caveat list that has historically been forgotten by the time the analysis note is written. Freezing now means freezing those caveats into the published record.

The shortest credible path to a defensible freeze is the sequence above. Given the workspace state and the existence of the closure-tauopt result already on disk, steps (a)-(c) are plausibly a few-day effort, not weeks.
