# Proposal: noise-aware smoothing of the diagnostic flat-phi_h half-difference

**Date:** 2026-10-02  
**Scope:** high-Q2 charged-hadron SIDIS v6/z005, 490 published IBU-k=4 bins  
**Status:** diagnostic proposal only. It neither changes the central release nor promotes `phi_h_halfdiff_rel` into the systematic budget.

## Recommendation

**Yes, conditionally smooth the *signed per-generator flat-minus-nominal components*, not the already positive max-envelope, and only after a noise-floor gate.** The current per-bin half-envelope

\[
u_{\rm raw,i}=\tfrac12\max\left(|d_{\rm DJ,i}|,|d_{\rm RG,i}|\right),\qquad
 d_{g,i}=\frac{\sigma_{g,\rm flat,i}-\sigma_{\rm nominal,i}}{\sigma_{\rm nominal,i}},
\]

has two known statistical distortions: selection of the larger of two noisy absolute values is upward biased, and its selected generator switches bin-to-bin. The bin-resolved flat-weight study established that its fine-grained shifts are noise dominated; the pure half-sample control nearly reproduces them (and exceeds the RAPGAP signal). A cosmetic smooth fit to `nu_raw` would hide that fact and is rejected.

The recommended procedure keeps a localized excess whenever its signed residual is significant relative to the measured half-sample noise. It is therefore a noise-aware estimator for display/diagnostic sensitivity, not a claim that the underlying systematic is smooth in arbitrary dimensions. Pending all validation gates below, the defensible physics scale remains the iteration-1 cell-level sensitivity (~0.54% median per generator), and the source remains diagnostic.

## Inputs and definitions

Use exact alignment on `(charge, bin_index)` and the existing iteration-1, coherent truth-leg responses:

- \(\sigma_{\rm nominal}\): central v6/z005 IBU k=4 cross section;
- \(\sigma_{g,\rm unity}\), \(\sigma_{g,\rm flat}\): iteration-1 `g = DJANGOH14, RAPGAP31` response unfolds;
- half-sample noise proxy
  \[
  n_{g,i}=\left|\sigma_{g,\rm halfA,i}-\sigma_{g,\rm unity,i}\right|/\sigma_{\rm nominal,i}.
  \]

`halfA` is a pure weight-statistical control, not an independent physics variation. This proxy deliberately measures the response/unfold noise relevant to the fine flattening construction. A later adoption must replace or supplement it with independent reweight/bootstrap replicas if those become available.

## Proposed algorithm

1. **Construct signed components, not a magnitude.** For each generator, calculate \(d_{g,i}\) above and retain sign. Do not first take absolute values, take a max, or smooth `phi_h_halfdiff_rel`: those steps erase generator sign/coherence and make the noise-selection bias irreversible.
2. **Use the only structurally supported neighborhood.** Partition each charge independently into fixed `(Q2,x)` cell and `z` bin. Smooth only along the five ordered physical \(p_{T,h}\) bins in that row; never cross a `z` boundary, `(Q2,x)` cell boundary, or charge. Thus there are 98 independent five-point rows (49 per charge). The 2026-09-25 study directly observes the fine-bin weight noise rising with \(p_T\); this is an appropriate one-dimensional regularization direction, whereas smoothing across `z` would assert an unvalidated fragmentation-shape relation.
3. **Fit a local linear signed trend per generator.** At target pT index \(t\in\{0,\ldots,4\}\), fit \(a_0+a_1(j-t)\) to all row points \(j\), separately for DJANGOH14 and RAPGAP31, by weighted least squares with
   \[
   w_{g,j,t}=\frac{K(|j-t|/h)}{\max(n_{g,j}^2,10^{-12})},\quad
   K(u)=(1-u^3)^3\; (u<1),\quad K(u)=0\;(u\ge1),\quad h=2.01.
   \]
   The degree-one fit preserves an allowed row slope and behaves properly at the pT endpoints. \(h=2.01\) gives every member of the five-point row non-zero support but excludes any external row; it is a deliberately small, fixed width, not a fitted high-order curve. Call the fitted value \(\tilde d_{g,i}\).
4. **Apply a signed noise-floor excursion guard.** Form \(r_{g,i}=d_{g,i}-\tilde d_{g,i}\). If \(|r_{g,i}|>2n_{g,i}\), retain the measured signed value \(d^*_{g,i}=d_{g,i}\) and flag `preserve=True`; otherwise use \(d^*_{g,i}=\tilde d_{g,i}\). The two-noise-sigma threshold is a protection rule, not a discovery threshold: a candidate must survive the dedicated validation below before it can be interpreted as a physical localized feature.
5. **Only then form the diagnostic magnitude.** Compute
   \[
   \nu_{\rm smooth,i}=\tfrac12\max\left(|d^*_{\rm DJ,i}|,|d^*_{\rm RG,i}|\right).
   \]
   This signed-delta-then-envelope order avoids smoothing the upward-biased switching envelope. Report `nu_raw`, `nu_smooth`, both signed components, their noise proxies, residuals, and preserve flags. Do not infer a covariance from this smoother.
6. **Keep contamination separate.** The `fork_rg`/`fork_dj` decomposition fields are retained for audit but are excluded from \(d_g\), \(n_g\), fitting, and the envelope. They are different effects: fork/prior mismatch (RG-unity versus central; DJ-unity versus central) and DJ–RG generator variation already represented by `ibu_model_rel`. If a future study wants a decomposition, smooth each *signed, independently validated* physics component using its own control/noise reference, then combine only under an approved source-overlap/correlation model; never smooth or add the contamination fields into this flat-phi source by convenience.

## Noise-floor and validation gates before adoption

Adoption is blocked unless all of these are recorded on the exact 490-bin basis.

1. **Control closure:** run the identical smoothing/guard on halfA (and, preferably, multiple disjoint half or bootstrap controls). Its retained-feature rate, distribution of \(|r|/n\), and residual autocorrelation must be consistent with the stated false-retention expectation. A single halfA is insufficient to claim a calibrated 2-sigma false-positive rate.
2. **Signal-versus-noise separation:** show, by generator and pT index, that the change in the full flat response is distinguishable from the rescaled half-control noise. The prior result says the iteration-2 signal is noise dominated; this proposal must not reverse that conclusion.
3. **Robustness:** repeat with degree 0 and degree 1, \(h=1.5,2.01,2.5\), and leave-one-pT-point-out fits. The median and integrated diagnostic must be stable; a feature that appears only for one width/order is not retained as a physical excursion.
4. **Physical and numerical integrity:** preserve 490 unique finite rows; verify no negative/unphysical response issue is introduced (the smoother operates only on derived deltas); verify no cross-boundary borrowing; compare pT trends against per-generator acceptance/response statistics. Inspect every preserved bin, especially sparse high-z/edge-pT bins.
5. **Generator and source separation:** verify that smoothing has not absorbed `fork_rg`, `fork_dj`, or the DJ–RG unity spread; confirm that the result is not combined with `ibu_model_rel` without an approved overlap treatment. Retain the iteration-1 truth-conservation and common-data-vector checks.
6. **Uncertainty treatment:** do not create a rank-1 covariance \(\nu_i\nu_j\). Until replica-derived correlations exist, use this only as a per-bin diagnostic magnitude; any budget/covariance promotion needs QA and systematics-review approval, explicit correlation assumptions, and an integrated-observable propagation test.

Failure of any gate means retain the unsmoothed diagnostic and quote the cell-level result rather than a smoothed per-bin source.

## H1 precedent and limits

No located H1 digest supports smoothing a noisy one-sided systematic *difference* and then treating the fitted curve as an approved per-bin uncertainty. The applicable precedent is instead methodological:

- `docs/research/research-h1-1302-1321-charged-multiplicity.md`, Secs. 6–7 (H1 Coll., EPJC 73 (2013) 2406, arXiv:1302.1321): DJANGOH-versus-RAPGAP detector-correction differences were used as the MC-model systematic, ranging from 0.2% to 5.5%; it does not prescribe a smoothing fit. This supports retaining generator variations explicitly, not merging them with the flat-phi effect.
- `docs/research/research-electron-energy-systematic-critique-2026-05-26.md`, Sec. A1–A2: a noise-floor closure is required before interpreting unfolded per-bin shifts; random noisy signs must not be imposed as fully correlated structure. Its recommended remedies include a noise-floor study and a diagonal/noise-separated fallback. This directly supports the present preserve-or-smooth guard and the prohibition on inventing covariance.
- `docs/research/research-tunfold-native-miss-handling-z005-2026-06-21.md`, Sec. 8: regularization excludes artificial boundaries, supporting the strict prohibition on smoothing across `(Q2,x)`, z, or charge boundaries.

Thus the proposed row smoother is an explicitly limited, structurally constrained diagnostic estimator, not an H1-precedent-approved replacement for raw systematic variations.

## Demonstration (not an adoption test)

The associated quick demo applies the prescribed algorithm to the existing 490-bin inputs. It writes signed raw/smoothed/final components, halfA noise proxies, guard flags, and both envelopes. No v1/v2 PDFs, central products, or total-systematics products were regenerated or changed.
