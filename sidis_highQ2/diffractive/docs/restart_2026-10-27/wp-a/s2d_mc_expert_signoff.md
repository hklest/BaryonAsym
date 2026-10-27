SIGN-OFF: YES-WITH-CONDITIONS
# WP-A S2(d) MC-expert review of remedy-1 noise-correction semantics (h1-mc-expert, 2026-10-27)
Basis: review of the semantics as described in the brief plus the S1c lookup (s1c_mc_expert_lookup.md). Code files
were NOT re-read line by line; items marked PROVISIONAL rest on the brief's description.

## (a) Soundness given RT_NOISE=Y: YES, with conditions
- Overlay noise is already in every MC event (OSRN words 7/8/9/10 = 1; FMD at mdigi, PLUG at pscale, FTS at dstmak),
  so the correct object is the RESIDUAL data-minus-MC noise, not an absolute rate. Items 1-5 do that. Sound.
- Control region (deep gap, xIp<0.04, mx>3, eta_max<1.5) is signal-poor for forward activity: right choice. Even/odd
  split with proven key-disjointness is fine.
- Using sim-period overlay noise kept and re-weighting components to data periods is correct; the sim-period
  diagnostic being composition-confounded is expected (em06 = 9157+7071 only) and not evidence against it.
- Conditions: C1 the 12-35 excess events/period make the replacement spectrum statistically thin; fix the RNG seed,
  store the spectrum, and show the Plug N-1 shift with the spectrum statistical uncertainty as a systematic.
  C2 the data>MC excess may be beam-related activity absent from the RT snapshot (S1c Q2), i.e. it is not a
  "noise-hit" in the MC sense; the model is an effective correction. State this in the note.
## (b) q formula: YES (exact for the model), note the deviation
- f' = 1-(1-f_MC)(1-q) assumes the removal acts independently on top of the overlay survivors; q=(f_data-f_MC)/(1-f_MC)
  is then exact closure. EPJC-72 (H1 diffractive) used a removal probability equal to the data-MC fail difference in
  the control sample; q differs from p_naive by factor 1/(1-f_MC)~1.3-1.4 (q .155-.191 vs p .11-.14). This is
  not a physics difference but a definition; q is the better-defined one. Cite as "exact closure, extends A4's leading
  order". I could not verify the EPJC-72 text here: PROVISIONAL on the literal EPJC wording.
- Independence is an assumption; the Plug/FMD/FTS correlations are untested. Closure holds by construction in control.
## (c) >=7 GeV restriction and MC normalised to data control count: ACCEPTABLE
- Restriction is mandatory (a removed event fails the veto by construction); consistent. Overflow inclusion fine.
  Condition: negative-clipped bins bias the spectrum slightly upward in tail; quote the clipped fraction.
- Normalising MC to the data control count is acceptable for a SHAPE/excess definition because yield ratio is
  0.92-1.01 and absolute lumi would fold in cross-section/ND-DIFF fraction uncertainty. But it injects the
  yield-ratio (<=8%) into the excess count: propagate as systematic (variation: absolute-lumi normalisation).
## (d) Per-detector split vs "all to Plug": NOT a blocker for this run; flag to user as a stated limitation
- Semantics: q is the right probability for the stack-veto efficiency; Stage-B and the cut efficiency (veto as an AND of
  three) are therefore corrected correctly in total. What is wrong is only the attribution: Plug N-1 gets too large
  an upper tail (excess is ~1/3 Plug, ~0.064-0.075 of 0.17-0.19 q), FMD/FTS N-1 are left uncorrected.
- Consequence is confined to the N-1 distributions/closures that individually look at Plug (and FMD, FTS); the
  analysed yield after the full veto is unaffected to first order. Therefore do not block, run it as specified,
  but (i) label Plug/FMD/FTS N-1 plots "stack-attributed", (ii) the Plug-N-1-based closure is NOT a validation,
  (iii) report the ePlug spectrum draw as the A5 spec, and (iv) escalate to the user a recommended follow-up
  variant (per-detector q_d with a joint draw, or at least a split 1/3 Plug) since A5 as adopted is physically
  mis-attributed. This is a user decision (plan fixed single probability); I do not override it.
- Condition: the corrected run must additionally record the three per-detector N-1 for audit so the variant can be tested
  without resubmitting the whole chain.
## (e) Same q for ND: YES
- ND MC carries the same RT overlay and the control-region deficit is measured with ND in the cocktail; the
  forward noise is an event-independent detector effect, so apply to all (A6). Caveat: ND has a real forward-energy
  tail (hadronic activity in Plug at eta_max-large), but the ePlug>=7 replacement acts only on events passing the
  ePlug<7 draw with probability q, so no double counting beyond the model. Condition: show the ND share of removed events.
## Other conditions
- Period decomposition must exactly sum exposure factors (reported unique): assert it in the log.
- Keep ep0607 as one period (halves <2 sigma): agree.
- Items verified: overlay semantics (source-backed, S1c). Unverified: code line-level behaviour (PROVISIONAL).
