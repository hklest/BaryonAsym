VALIDATED: YES-WITH-CONDITIONS
S4 unit (d), h1-mc-expert. Basis: g1_eval_wpa.py "Stage-B yield ratio", n1_wpa.py :62,:104,:208,:294-322, g1_7071.json (s4_noplug). Source-read only; no new runs.

(a) Truth definition and envelope
- Cut at M_Y >= 1.6 GeV on the MY_EDGES grid is right: 1.6 is an exact bin edge (:104), so the row selection `edg[:-1] >= 1.6-1e-9` has no partial-bin leakage. Signal-side PDISS truth is M_Y < 1.6 (SIDIS diffractive definition), so M_Y >= 1.6 is the complement = migration.
- CONDITION 1 (not verified): the x5.3611 truth-in-cut flag uses hatMYGen (:208) while the migration selection uses mirMY (:294). Confirm mirMY == hatMYGen on 9155/9157/9158 (same M_Y, same events). If they differ, part of the M_Y>=1.6 rows could carry x5.3611 or the weights would be inconsistent. A one-line check (max|mirMY-hatMYGen|, or the sum of w in mirMY>=1.6 with truth-in-cut flag) settles it.
- The migration term is ALREADY inside N_MC,SB (signed v3, "included as predicted"); the envelope only varies it. So the ratio is not "corrected by" the term; this is consistent with the A9 wording only if read as "after including it". State this in the verdict.
- +-50% is a reasonable, but arbitrary, PDISS-normalisation envelope. It does not cover the dominant uncertainty of this term, the M_Y dependence (dsigma/dM_Y^2 slope, x_IP/t shape in M_Y>1.6 vs the x5.3611 rescaled M_Y<1.6 part). It is adequate for a PASS/FAIL test here because even x10 on the term cannot change the outcome (see c). Report it as a normalisation-only envelope.

(b) Plausibility of 1-2% without the Plug veto
- Yes, plausible. M_Y > 1.6 dissociation deposits energy at eta > ~3.5-4 (Plug/FTS/FMD region). Stage-B retains the FMD (nFmu12) and FTS (nFts28) vetoes and cluster eta_max < 3.2, all of which reject high-M_Y systems efficiently; the Plug adds only a tail (M_Y -> large) already mostly caught by FMD/FTS. Removing it should therefore add little, which matches 0.008-0.021 (noplug) vs 0.009-0.021 (remedy 1): identical within the stats of a small signed sum.
- Fraction falls D0->D2 (0.019->0.008), as expected: higher-|t|/x_IP-restricted D cells have lower M_Y reach in the PDISS samples.
- Caveat: this is only the generator-level PDISS (9155/57/58) content. The M_Y>1.6 content of the other 5 diffractive samples (DIFFVM/RAPGAP elastic) is zero by construction, and the ND alternative carries its own rapidity-gap fluctuations; neither is "migration".

(c) Does it change A9 PASS/FAIL?
- No. Data/MC ratios 1.21/1.31/1.51 (7071) up to 1.33/1.57/2.36 (7094) require MC to be low by 20-60% (up to x2.4 at D2); the migration term is 0.8-2.1% of MC and the envelope shifts the ratio by <= 0.03 (D0 1.201-1.224). Even x10 on the term moves the ratio by ~0.1-0.2 at most and never to 1+-0.10 across D0-D2. FMD N-1 1.07-2.33 and FTS 0.70-0.76 fail 0.10 by 7-23x independently.
- The ratio worsens with D index and with ND alternative (7071 -> 7091 -> 7094): the deficit is in the ND/background description in the Stage-B (and D2 low statistics of MC), not migration. The FAIL points at ND alternative/PDISS-vs-ND balance and the FMD/FTS description, i.e. a component-normalisation issue; do not try to cure it with the PDISS migration term.
- Remedy 1 comparison (1.21/1.30/1.43) shows the Plug veto brings only ~0.08 at D2; same conclusion.

Status: source-backed on weights, bin grid, envelope code, and JSON numbers; mirMY==hatMYGen is PROVISIONAL (CONDITION 1).
