# Area 1 / Critic B — Cuts physics correctness (h1-analysis-critic)

**Verdict:** **CONCERN** — 3 fatal + 3 major + 2 unstated assumptions
**Material reviewed:** `build_migration_th2_per_charge_NWfix.py` cuts blocks, fiducial definitions, gen/reco harmonization

---

## Fatal physics issues

### C1P1. **`y_cut > 0.1`** at gen and reco is unphysical for HERA-II charged-hadron SIDIS

The y cut `0.1 < y < 0.7` is applied identically at gen and reco (good, symmetric). But **y > 0.1 corresponds to W² > ~225 GeV² at Q² > 150 GeV²** — the HERA-II low-y trigger turn-on is at y > 0.6, not y > 0.1. Why is the lower cut so low?

**Concern:** At y < 0.5, hadron acceptance drops dramatically (forward boost). The bins with y in [0.1, 0.3] will be dominated by gen tracks with no reco partner — inflating miss row, deflating ε[j] artificially.

**Severity:** The `0.1 < y < 0.7` range was chosen 2026-05-23 to match the published v3final fiducial (note: pre-rev6). Need to verify this matches H1's standard SIDIS multiplicity y range OR document the unusual choice. ZEUS NPB 776 used y ∈ [0.04, 0.95]; H1 inclusive multiplicities (e.g., EPJ C61 2009) used y ∈ [0.05, 0.6] typically. **0.1 < y < 0.7 is borderline non-standard.**

**Fix or document:** Verify y range against ZEUS/H1 reference; if intentional document and propagate to fid systematic.

### C1P2. **Vertex cut `|PVz| < 35 cm` is at the H1 vertex resolution scale, NOT a fiducial**

`H1`'s primary vertex resolution along the beam is ~few cm. A cut at 35 cm is essentially "anything that reconstructs as DIS" with very loose handle. Useful for rejecting beam-gas backgrounds.

**Concern:** Beam-gas backgrounds tend to peak at |PVz|>50 cm (upstream/downstream collisions), so a cut at 35 captures most but not all. **Combined with the upstream Q²gen<60 bgsub, beam-gas might be partially double-counted (subtracted via MC bg + cut at vertex).** This is the same IP-M4 concern from the rev6 audit.

### C1P3. **`DcaCosh_abs < 2.0` is a track-quality cut chosen at the H1 detector limit**

`Dca' / cosh(eta) < 2.0 cm` is the "primary track" rejection cut. Hadrons from secondary vertices (Lambda → pπ, K_s → ππ, photon conv → e+e−) typically have DCA > 1 cm. The cut at 2 keeps most primaries while suppressing V0 daughters and conversion electrons.

**Concern:** The complementary `IsMatchedV0Daughter == 0` cut at line 533 explicitly rejects V0 partners in the matched fill. **If both DcaCosh and V0-matched cuts are active, V0 contamination is being removed twice** — once via the DCA cut (most V0s have DCA > 2) and once via the matcher flag. Not a bug per se; just inefficient and ambiguous.

**Severity:** Cosmetic, but: when computing σ_V0_contamination as a systematic, must not double-count the rejection.

---

## Major concerns

### C1M1. **Pt > 0.15 GeV reco cut without gen counterpart**

(Same as C1M2 in critic A.) Low-Pt gen tracks (Pt < 0.15) enter prior but cannot match a reco partner → forced to miss → ε[j] artificially low in iz=0 / ipt=0 bin. Physics impact: ~5-10% per-bin in lowest-Pt bin.

**Defensibility:** The 0.15 GeV reco cut is the H1 CTD acceptance edge — physically motivated. But the asymmetry inflates the 1/ε correction in low-Pt bins, biasing unfolded yields HIGH by ~5-10% there.

**Fix:** Add `GenPt > 0.15` to gen cuts (symmetrize). This is the cleanest fix and matches the published fiducial.

### C1M2. **`xF_SIDIS_es < 0` cut definition — CHECKED, NOT A BUG**

**Initial concern:** Code says `xF_SIDIS_es < 0` keeps events; methodology doc says `xF_Trento > 0` (current hemisphere). Sign flip?

**Verification (added 2026-05-31, post-critic):** Read `src/ChargedSidisTreeBuilderHighQ2_v5g.C` lines 1555-1580. `xF_SIDIS_es = 2 · pz(hadron, γ*p rest frame) / W` where the rest frame is obtained by `Boost(-boostGammaP_es)`. The `-boostGammaP_es` puts the γ*p system at rest but PRESERVES the proton beam direction along lab +z. In this frame, +z is along the PROTON direction, which is OPPOSITE to the γ* direction. Trento convention has +z along γ*. Therefore `xF_SIDIS_es = −xF_Trento` by sign convention. Cut `xF_SIDIS_es < 0` ≡ `xF_Trento > 0` ≡ current-fragmentation hemisphere. **Methodology and code are consistent.**

**Lesson:** Two opposite-sign conventions for xF in HEP literature (γ*-z vs proton-z). The fact that this required reading C++ source to verify shows it should be DOCUMENTED at the cut site. Add a comment to build_migration_th2_per_charge_NWfix.py line 432 explaining the sign convention.

### C1M2-orig. ~~Original concern (now superseded by verification above):~~

The cut keeps `xF < 0` (target fragmentation region in the γ*-p frame). For a Trento-frame xF computation this is the BACKWARD hemisphere. The xF_SIDIS_es branch uses the eSigma electron method.

**Concern:** This means the analysis is **only backward-hemisphere hadrons**. For charged SIDIS multiplicity this is unusual — most published multiplicities cover the full xF range or specify "current-region xF > 0". Why backward-only?

If the analysis is intentionally restricted to target fragmentation, the published cross-section will not be comparable to standard multiplicity papers without xF integration. **Document the choice OR verify this is target-region motivation, not a sign-convention bug.**

**Severity:** If sign-convention bug → analysis is WRONG hemisphere. If intentional → narrow comparison scope.

**Fix:** Check the actual Trento xF convention in the v5g extractor. The original notes say "Option C fiducial: xF_Trento > 0" — but code says `xF_SIDIS_es < 0`. **These are different signs.** Either the convention flipped between extraction and analysis, or the docs are wrong.

### C1M3. **`empz_lo=35` and `empz_hi=70` widened from previous `[45, 65]`**

This widening was documented 2026-05-23 (`empz_cut_loosening_2026-05-23.md`). The widening accepts more events but also accepts more events with poorly-reconstructed hadronic final state (Empz < 45 means significant missing energy upstream of detector).

**Concern:** Empz ∈ [35, 45) bins probably have higher fake rate (poor reco quality), worse z resolution, larger σ_model. Should be quantified.

**Severity:** Per-bin systematic, especially for low-z bins.

---

## Unstated assumptions

### C1A1. **The gen fid `|GenEta| < 4` includes the FORWARD beam pipe**

The CTD goes to |eta| < ~1.5; backward/forward beam pipes extend to |eta| ~ 4. Including |eta| up to 4 at gen means gen acceptance includes tracks that **cannot possibly be reconstructed in the CTD**. They contribute to the miss row by construction. **This inflates the prior by a factor (acceptance |eta|<4) / (acceptance |eta|<1.5) ~ 2.7×.**

The IBU 1/ε correction then recovers these forward gen tracks based on MC prediction of the matched-vs-miss ratio. If MC mis-models the forward acceptance (likely, since extrapolation), the 1/ε correction is wrong.

**Severity:** This is the biggest implicit assumption in the entire analysis. The published "gen pool" includes high-|eta| events that are NEVER measured at reco. The 1/ε factor extrapolates MC into an unmeasured region.

**Fix:** Either tighten gen `|GenEta|<1.3` to match reco (cleaner — measures only what's measurable), or quote an extrapolation systematic. The original Option C choice was `|GenEta|<4` to match published H1 SIDIS conventions, but this comes with a heavy MC dependence.

### C1A2. **`Q²>150` and `xF<0` are NOT the same fiducial at gen and reco for some bins**

Cell S3 spans Q² ∈ [1500, 20000] but the Q² estimator differs between es and electron methods by 5-10% in the high-Q² tail. The reco Q² (Q2es) at the cell boundary may be 1500.001 (in S3) while gen Q² (Q²_es_gen) is 1499.999 (in S2). Same physical event, different cell → migration that's NOT captured in the response (it's a gen-fid effect, not a reco-bin effect).

**Severity:** Cell-boundary bins (90, 120, 150, 180, 210 — cell-edge gen bins) may have systematic biases from gen-fid migration.

**Fix:** Quantify the gen-reco Q² estimator difference per event; add a buffer zone or document the boundary effect.

---

## Recommended next physics check

**Most decisive:** Verify the **xF sign convention (C1M2)**. The code says `xF_SIDIS_es < 0`. The Option C fiducial documentation says `xF_Trento > 0`. **These can't both be right.** If this is a sign-flip bug, the entire analysis is on the wrong hemisphere of the event — a categorical error.

Verification: take 100 events with `IsMatchedHadron==1` and check `xF_SIDIS_es` vs Trento-frame xF (computed from the hadron momentum dotted with q in the γ*-p frame). If they have opposite signs, the convention is flipped somewhere upstream.
