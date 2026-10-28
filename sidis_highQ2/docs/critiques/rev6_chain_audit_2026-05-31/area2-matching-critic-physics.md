# Area 2 / Critic B — Matching physics (h1-analysis-critic)

**Verdict:** **CONCERN** — 2 fatal + 3 major + 1 unstated assumption

## Fatal physics issues

### C2P1. **Consumed-key matching is GEOMETRIC, not PHYSICS-CONSISTENT**

The consumed-key tuple `(run, evt, signed_pdg, round(pt,5), round(eta,5))` matches gen rows to consumed reco tracks by kinematic identity (Pt+eta within 5-decimal precision). This works because for matched events, the H1OO matcher copies the gen kinematics onto the reco row exactly (line 690-696 comment: "inlined gen kinematics on ChargedTot exactly equal the partner ChargedTotGen row").

**But:** If H1OO's matcher uses ΔR < 0.05 OR ΔPt/Pt < 5% as its criterion (typical), there are pathological cases where:
- Gen track A at (Pt=2.0001, Eta=0.5) is matched to reco track X.
- Gen track B at (Pt=1.9999, Eta=0.5) is matched to reco track Y.

Inlined gen on the reco rows gives X.gen = A and Y.gen = B (matcher correctly assigns). But our consumed_keys hash with `round(Pt, 5)` makes A's key = `(..., 2.00010, 0.50000)` and B's key = `(..., 1.99990, 0.50000)`. These DIFFER at 5-decimal precision, so collision is avoided.

**However**, if Pt = 2.00001 and 1.99999 (1e-5 apart, below 5-decimal precision rounding boundary), both round to 2.00000 — collision.

**Severity:** Extreme corner case but possible in dense gen samples. The collision rate has not been measured.

**Fix:** Empirical: count collisions in build by tracking `len(consumed_keys)` vs `n_matched`. If `n_matched > len(consumed_keys)`, collisions are happening. Add an assertion.

### C2P2. **Charge assignment for miss is from GEN, for matched is from RECO**

Lines 646 (gen prior fill), 720 (miss fill): both use `gen_chg[k]` (gen-derived charge).
Line 675 (matched fill): uses `reco_chg[k]` (reco-derived charge).
Fake fill (line 698): uses `reco_chg[k]`.

**Physics impact:** For a charge-confused matched event (true h+ reconstructed as h-), the matched contribution goes to the h- response matrix (per reco charge). The corresponding gen track has true charge h+ but the matched fill ASSIGNED the h- bin. Then in the h- response, the gen-axis bin is the GEN bin of a h+ track — wrong charge composition.

For miss events, the gen charge is correct.

For the per-charge ε[j] = col / prior:
- col_neg[j] (matched-reco h-) includes some events from h+ gen (charge-confused).
- prior_neg[j] (gen pool h-) excludes those events.

Net: ε_neg[j] > 1 in bins where charge confusion goes in one direction (h+ → reco h-). Mirror effect for h+: ε_pos[j] < 1 in bins where outgoing charge confusion (h- gen → reco h+) is dominant.

**Severity:** Integrated charge confusion is 0.35% — per-bin can be 1-2% in some bins. **The NW asymmetry critic (CC1) noticed ε[j] > 1 in 4 bins for h+ on a single file** — this could be the actual mechanism, not statistical fluctuation. Need to verify.

**Fix:** Either symmetrize (use gen charge throughout for the matched fill too, on the grounds that the gen charge IS the physical truth), OR explicitly compute charge-confusion contribution to the response and subtract.

---

## Major concerns

### C2M1. **V0 rejection: gen-level via IsV0DaughterGen, reco-level via IsMatchedV0Daughter; do they agree?**

Both should be set together by the H1OO matcher: when a gen track flagged IsV0Daughter==1 is matched to a reco track, the reco row's IsMatchedV0Daughter should be ==1. **Untested.**

**Concern:** If they disagree (matched gen V0 daughter but `IsMatchedV0Daughter==0` on the reco row), the matched fill keeps the V0 event (it passes `not_v0_match==True`) but the corresponding gen track is excluded from prior (failed `IsV0DaughterGen==1`). Net: ε[j] inflated.

### C2M2. **Species pool `pi/K/p` is tighter at gen than at reco**

At gen (line 312): `is_pool = np.isin(abs(GenPDG), POOL_PDG)`. POOL_PDG = (211, 321, 2212) = (π±, K±, p±).
At reco: no species check (reco is charged-track-based, no PID).

**Asymmetric:** Reco includes ALL charged hadrons (μ, e, other rare species). For inclusive charged-hadron multiplicity, "all charged hadrons" is the right reco-side definition. But the gen pool is restricted to pi/K/p.

**Physics impact:** Reco events from gen muons/electrons (rare but real: e.g., charm semi-leptonic, π → μν decays) are matched against pi/K/p gen pool → matcher might fail to find a partner → false fake. Fakes inflated.

**Severity:** Small (<1% of reco) but contributes to systematic fake-modeling uncertainty.

**Fix:** Either restrict reco to pi/K/p too (requires PID at reco — we don't have it), or expand gen pool to all charged hadrons (cleaner, removes asymmetry).

### C2M3. **Inclusive treatment of gen pi/K/p sums species without correction**

For an inclusive charged-hadron multiplicity, summing pi+K+p at gen is correct. But this means the per-bin acceptance ε[j] is a species-averaged number, weighted by the MC composition. If the MC species composition differs from data (e.g., Rapgap predicts 70/15/15 pi/K/p but data is 65/20/15), ε[j] is biased by the species-acceptance differences.

**Severity:** Acceptance differences pi vs K vs p are O(few%) at high-pT (different track-quality cut survival rates). Combined with composition uncertainty O(few%), could be a 1-2% per-bin systematic.

**Fix:** Compute ε[j] per species and combine using a data-vs-MC composition fit, OR quote σ_composition systematic from RG vs DJ composition difference.

---

## Unstated assumptions

### C2A1. **H1OO matcher is INDEPENDENT of reco track quality cuts**

The matcher runs on ALL reco-gen pairs (before the analysis track cuts at line 427). The analysis cuts (Pt>0.15, NHitDedx≥10, etc.) are applied AFTER the match flag is set. Therefore:
- A gen track matched to a "bad-quality" reco track that fails the Pt>0.15 cut: gen track is "matched" at the matcher level, but the matched fill at line 668 requires `pass_reco_full==True` which fails. The match is silently dropped from the response. The gen track key is NOT added to consumed_keys. Then step 7 miss fill picks up the gen track as a miss.

**Net:** Bad-reco matches are reclassified as missed. **This is correct for the unfolding logic** (we're measuring gen tracks with reco passing analysis quality cuts).

**But:** For NW handling in step 8, the consumed_keys.get(key, 1.0) returns 1.0 for these reclassified-miss tracks → they get NO NW weighting in prior_NWeff. This is correct (no reco partner ≡ no nuclear correction applies).

**Consistent. Document the assumption.**

---

## Recommended next physics check

**Most decisive:** Verify **C2P2 (charge confusion mechanism for ε > 1)**. Count bins with ε[j] > 1 in the full production sample (not the 1-file smoke test). If ε > 1 correlates with charge-confusion-dominant bins (small bins where one charge is rare), CC2 (charge confusion systematic) needs urgent propagation.
