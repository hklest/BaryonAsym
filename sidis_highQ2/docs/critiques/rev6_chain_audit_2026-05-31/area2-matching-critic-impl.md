# Area 2 / Critic A — Gen-reco matching + fiducial (implementation-critic)

**Verdict:** **CONCERN** — 4 fatal + 4 major + 3 latent
**Files reviewed:** build_migration_th2_per_charge_NWfix.py matching block (lines 593-727)

## Fatal bugs

### C2F1. `build_migration_th2_per_charge_NWfix.py:691-697` — consumed-key hash uses `round(GenPt, 5)` and `round(GenEta, 5)` as set members. **Float-rounding collision risk.**

```python
consumed_keys[_ck] = _nw_partner
# where _ck = (run, evt, pdg_int, round(GenPt, 5), round(GenEta, 5))
```

**Bug:** Two distinct gen tracks in the same event with identical (run, evt, pdg, GenPt rounded to 5 decimals, GenEta rounded to 5 decimals) will share the same key. In high-density Q²/pT cells, this can happen with probability ~few × 10⁻⁴ per event (resolution at the 5-decimal rounding level vs typical reco-gen ΔR matching width 0.05 means ~1e-4 per pair).

When a collision happens:
- Matched fill: only ONE of the two events gets stored in consumed_keys (dict overwrites).
- Miss fill (line 716): the OTHER event's key is in consumed_keys → falsely marked as consumed → does NOT get the miss-row fill.
- Result: ONE event is matched but counted as miss (in unfilled overflow), the OTHER is missed but NOT counted (because its key was consumed by the first event's match).

Net: integrated counts are correct (one match, one miss preserved per pair). But the assignment is WRONG — wrong gen bin gets the matched count.

**Severity:** Per-bin contamination at 10⁻⁴ level. Negligible at integrated level. Could matter at the 0.1% per-bin level in densely populated cells.

**Fix:** Add the reco track Pt to the key (gen and reco partners have nearly identical Pt for matched events, so this disambiguates collisions). Or use 8-decimal rounding instead of 5.

### C2F2. `build_migration_th2_per_charge_NWfix.py:533` — `not_v0_match = (batch["IsMatchedV0Daughter"] == 0)` applied ONLY in matched fill, NOT in fake fill.

**Bug:** A reco track with IsMatchedV0Daughter==1 fails `matched_gen_ok` (line 557) → `i_gen_inlined = -1` → falls through to FAKE branch at line 697. **V0-daughter reco tracks are counted as fakes when they should be either (a) rejected entirely (don't enter response at all) or (b) routed to a separate V0-daughter category.**

Result: fakes are contaminated by V0 daughters. Fake-subtraction over-subtracts.

**Severity:** Depends on V0 fraction of reco tracks. Typical for charged SIDIS analyses is V0 contamination ~1-2% of reco. Over-subtraction by that amount in data_inner.

**Fix:** Add `not_v0_match` to `pass_reco_full` at line 447, OR explicitly route V0-matched reco tracks to a separate dropped category (not fakes).

### C2F3. `build_migration_th2_per_charge_NWfix.py:558-564` — `matched_gen_ok` requires BOTH `is_mat_h` AND `not_v0_match` AND `gevt_ok` AND `gfid` AND `gpool` AND `i_flat_gen>=0`. If inlined gen kinematics are NaN (extreme-eta sentinel, gen_phi NaN), `pTh_cm_gen` is NaN → `i_zp_g = -1` → `i_flat_gen = -1` → matched_gen_ok = False → falls to FAKE.

**Bug:** Same as C2F2 but for a different failure mode. A reco track that IS matched at the H1OO level but whose gen partner has bad kinematics (sentinel row) gets counted as a fake. Wrong physics.

**Severity:** Depends on sentinel-row prevalence; likely ~0.1% of matched events.

**Fix:** When `is_mat_h==1 AND i_flat_gen<0` (kinematic gen failure rather than fid/species failure), exclude from BOTH matched and fake — drop the row.

### C2F4. `build_migration_th2_per_charge_NWfix.py:712` — `_nw_partner = (wr / _w12) if _w12 > 0 else 1.0` extracts NW by division.

**Bug:** Floating-point precision: if `wr = w1·w2·NW` where NW is exactly 1.0 numerically, `wr / w12` may round to 0.9999999998 or 1.0000000002. The accumulator then uses a slightly-wrong NW. Cumulative effect over millions of events: per-bin drift of order 1e-10 — negligible.

**But:** If `wr` was clipped at line 483 (`wn = np.clip(wn, 0.0, NUCLIA_CLIP=10.0)`), the clipped NW (0.0 or 10.0) is reproduced exactly. Same with negative-NW events clipped to 0 — `wr = 0` and `_nw_partner = 0/_w12 = 0` → stored. Then in step 8 at line 767, `w_eff = w12 * 0 = 0` → the matched gen track gets ZERO weight in prior_NWeff while its reco partner contributed `wr=0` (also zero) to col. So col=0 and prior=0 for that event — consistent, but the matched event is effectively dropped from BOTH numerator and denominator of ε.

**Severity:** For events with NW<0 clipped to 0 (probably <0.01% of events given mean NW=0.99), the matched event is silently dropped from the response. Affects very few bins.

**Fix:** Detect clipped-to-zero matched events and route them to miss instead of dropping (since the gen track exists and should be in the gen pool).

---

## Major concerns

### C2M1. `build_migration_th2_per_charge_NWfix.py:603-614` — `evt_empz_lookup` keeps the FIRST GenEmpz value for each (run, evt) key.

```python
for k in range(len(gen_run)):
    key = (int(gen_run[k]), int(gen_evt[k]))
    if key in evt_empz_lookup:
        continue
    evt_empz_lookup[key] = float(gen_empz[k])
```

**Concern:** If gen_empz varies across gen rows in the same event (shouldn't happen physically, but the H1OO filler could have a bug), the first row wins arbitrarily. Other rows' Empz values silently discarded.

**Fix:** Assert all gen rows in an event have the same GenEmpz (within float tolerance). One-time check, then trust.

### C2M2. `build_migration_th2_per_charge_NWfix.py:710` — gen-side consumed key uses signed `gen_pdg_ctg[k]`; matched-side key at line 693 uses `pdg_inl[k]` (also signed).

Both are signed PDG. Good, matches REV2 §F6 fix. But:

**Concern:** signed PDG distinguishes pi+ (211) from pi- (-211). If a track is MISIDENTIFIED at gen (PDG flipped by some MC bug), the consumed key won't match → the matched reco partner correctly stores under one sign in matched, but the gen miss-pass looks for the other sign → both events counted (matched at one sign, missed at the other). Net: ONE extra entry in response.

**Severity:** Depends on gen PDG correctness. Should be 100% in well-formed MC.

### C2M3. `build_migration_th2_per_charge_NWfix.py:709-727` — miss-fill loop is Python-level for-loop over `np.where(miss_mask)[0]`. With ~50K miss tracks per file, this is ~10 ms per file. Slow but not broken.

Could be vectorized via numpy boolean indexing + np.add.at — order of magnitude faster.

### C2M4. `build_migration_th2_per_charge_NWfix.py:759-769` — NEW step 8 loop ALSO iterates `np.where(miss_mask)[0]` AGAIN. Two Python loops over the same gen rows. Adds another ~10 ms per file.

**Fix:** Fuse step 7 and step 8 loops into a single iteration. Minor performance, but cleaner code.

---

## Latent risks

### C2L1. `build_migration_th2_per_charge_NWfix.py:439` — `is_mat_h = (IsMatchedHadron == 1) & (IsMatched == 1)` redundancy: when can `IsMatchedHadron==1` but `IsMatched==0`? Possibly in H1OO filler bugs, but generally these are tied. Defensive AND is fine.

### C2L2. The matched-vs-fake split is binary (`if ig >= 0: matched else fake`). No "partial match" category for marginal cases (matched at H1OO level but failed fid). Those fall to FAKE.

**Concern:** If many such "partial-fid" matches exist, the fake bin is contaminated. Fake-subtraction scale (closed-form mean) is then biased.

### C2L3. `build_migration_th2_per_charge_NWfix.py:712-713` — `_w1_k = ct_batch["weight1"][k]` re-reads weight1 in the loop. Slow Python attribute access ~1e-6 s × n_ct → few hundred ms per file. Pre-vectorize: `weights_w12 = ct_batch["weight1"] * ct_batch["weight2"]` outside the loop.

---

## Recommended next implementation check

**Most decisive:** Verify **C2F2 (V0-daughter contamination in fakes)**. Add `not_v0_match` to `pass_reco_full`. Re-run smoke test and check fake fraction drops from 12.7% to ~11%. If yes, the V0 contamination is real and the fix is essential.
