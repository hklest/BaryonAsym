# Area 1 / Critic A — Cuts: event-level + track-level (implementation-critic)

**Verdict:** **CONCERN** — 4 fatal + 5 major + 3 latent
**Files reviewed:** `build_migration_th2_per_charge_NWfix.py` (cuts at lines 110-126, applied at 402-447, 517-528)

---

## Fatal bugs

### C1F1. `build_migration_th2_per_charge_NWfix.py:412` — VtxZ cut uses `np.abs(PVz) < 35` (exclusive), but EVENT_CUTS_RECO says `"VtxZ_abs": 35.0` (intent ambiguous between inclusive/exclusive). Track Eta cut same issue at line 430.

```python
& (np.abs(batch["PVz"]) < EVENT_CUTS_RECO["VtxZ_abs"])
...
& (np.abs(eta) < TRACK_CUTS_RECO["Eta_abs"])
```

**Bug:** Strict less-than at the boundary. Events with PVz = exactly 35.0 cm (rare but possible from MC fixed grid) get DROPPED at reco but the GEN cut at line 308 uses `(empz > GenEmpz_lo) & (empz < GenEmpz_hi)` — also strict. The harmonization is OK for empz, but for `|PVz|<35` vs `|GenZ_vtx|<?`, there's NO gen-side analog of the VtxZ cut. Gen events generated at |VtxZ|=40 still enter the gen prior because there's no gen-side VtxZ. **Asymmetry: reco cut on VtxZ has no gen counterpart → systematically removes events from the data side without removing them from the prior.**

**Severity:** Acceptance bias. The vertex distribution in MC is a Gaussian σ~12 cm, so `|PVz|<35` accepts ~99.7%. The 0.3% drop is at the reco side only — this means ε[j] is biased LOW by 0.3% per bin in regions where vertex acceptance matters (forward bins where high-eta events have wider PVz distributions).

**Fix:** Either (a) drop the VtxZ cut from reco (it's already inside the trigger acceptance), or (b) re-derive the prior to include the same selection at gen level (requires gen-level VtxZ which doesn't exist in ChargedTotGen).

### C1F2. `build_migration_th2_per_charge_NWfix.py:432` — `DcaCosh_abs < 2.0` is computed as `np.abs(dca / cosh_eta)` but with `cosh_eta != 0` check. cosh is always ≥ 1, never zero. The check is dead code.

```python
dca_norm = np.where(cosh_eta != 0, np.abs(dca / cosh_eta), np.inf)
```

**Bug:** `cosh_eta` is never 0 for any finite eta (cosh(x) ≥ 1). The np.where check is harmless but signals confusion — the original author wanted to protect against division by zero. The REAL danger is `cosh_eta = inf` for `|eta| > ~700` (sentinel rows). For those, dca/cosh_eta → 0 → passes the cut → false positive.

**Severity:** Sentinel-row contamination. The compute_reco_bin docstring notes "extreme-eta sentinel rows (e.g. geta ~ 999)" and wraps in `np.errstate(over='ignore', invalid='ignore', divide='ignore')`. Those rows then get `dca_norm = 0` and pass the DcaCosh cut, but get caught by `(np.abs(eta) < 1.3)` at line 430. So they don't actually enter the response. OK in practice; cosmetic bug only.

**Fix:** Use `(cosh_eta < 1e10)` instead of `(cosh_eta != 0)`.

### C1F3. `build_migration_th2_per_charge_NWfix.py:407-409` — Empz cut at reco (`Empz > 35 & Empz < 70`) is the v5g widened range, but the gen-side cut at `compute_gen_bin:308` uses `(empz > 35.0) & (empz < 70.0)` — same numbers, same strict bounds. **No off-by-one here.**

But: gen Empz lookup at line 514 (`evt_genempz = ct_genempz`) is built via per-event dict lookup with `evt_empz_lookup[key] = float(gen_empz[k])`. If multiple gen rows in the same event have DIFFERENT GenEmpz (they shouldn't — it's an event-level scalar — but the H1OO filler is bug-prone), the first one wins and the rest are silently masked. Verification needed: is GenEmpz really event-level scalar in v5g?

**Severity:** Latent if GenEmpz drifts per gen row; FATAL if so since the matched fill uses ct_genempz for fid check.

**Fix:** Assert `len(set(gen_empz[gen_run==r & gen_evt==e])) == 1` per event during build.

### C1F4. `build_migration_th2_per_charge_NWfix.py:422-423` — xF cut applied as `np.isfinite(xF_all) & (xF_all < 0)` at reco but at gen it's `(xF_h1 < 0)` (line 305) WITHOUT the `np.isfinite` check.

```python
# Reco line 422-423
xF_keep = np.isfinite(xF_all) & (xF_all < 0)
# Gen line 305
& (xF_h1 < 0)
```

**Bug:** Gen rows with `xFGen = nan` will pass the `< 0` cut as False (numpy semantics: NaN comparisons always False), so they get masked out by the chain `pass_fid` — fine. But reco-side NaN xF gets explicit isfinite check first. **Asymmetric handling of NaN at reco vs gen, equivalent in outcome.** No actual bug, but the asymmetry invites future bugs if someone changes the cuts.

**Fix:** Make cuts identical: `np.isfinite(xF_h1) & (xF_h1 < 0)` at gen too.

---

## Major concerns

### C1M1. `build_migration_th2_per_charge_NWfix.py:464` — z_outlier rejection drops `z<0` AND `z>1`:

```python
z_outlier = (~np.isfinite(z_comp)) | (z_comp < 0.0) | (z_comp > 1.0)
```

**Concern:** z_comp can be marginally outside [0,1] due to numerical precision (-1e-9 or +1.0001). These are physically z≈0 or z≈1, not bad reconstructions. Dropping them biases the highest-z bin. The gen z (`zGen`) at line 342 has NO outlier rejection — accepts anything. **Asymmetric: reco drops z near boundary, gen doesn't.**

**Severity:** Per-bin acceptance bias at the z=1 edge (bin iz=5, [0.7, 1.0]) of ~0.5%.

**Fix:** Clip z_comp to [0, 1] instead of dropping, OR apply the same outlier rejection to zGen.

### C1M2. `build_migration_th2_per_charge_NWfix.py:117` — `Pt_min = 0.15, Pt_max = 15.0` for reco; no gen-side Pt cut.

Gen Pt distribution extends to >15 GeV. Gen tracks with Pt > 15 get binned (line 326 just uses Pt) — but the BINNING `PTH_EDGES_GEN = [0, 0.5, 1.0, 2.0, 4.0, 8.0]` caps at 8 GeV. Gen Pt > 8 GeV gives `i_zp_gen = -1` and the row is dropped (line 170-172). So binning enforces an implicit cut. But the reco Pt > 8 GeV gets accepted up to Pt_max=15 — then the FINE reco binning also caps at 8 GeV (`PTH_EDGES_RECO[-1] = 8.0`), so reco Pt>8 also gets dropped. **OK in practice — both binnings cap at 8.**

But: reco Pt cut `>0.15` vs gen no cut → gen tracks with Pt<0.15 enter the prior. Since gen binning starts at Pt=0, those low-Pt gen tracks contribute to bins (iz, ipt=0) at gen. Reco partner with Pt<0.15 is dropped. **Asymmetry: low-Pt gen tracks have no reco partner by construction → forced into miss row → inflates ε[j]^{-1} in low-Pt bins.**

**Severity:** Per-bin acceptance bias in bin ipt=0 (Pt [0, 0.5]) of order 5-10% from low-Pt cut. Could be physically intentional (low-Pt < 0.15 is below tracker acceptance), but should be propagated to gen by an explicit cut.

**Fix:** Either add `(GenPt > 0.15)` to `compute_gen_bin:` cuts, or document the cut as part of the published fiducial.

### C1M3. `build_migration_th2_per_charge_NWfix.py:419-420` — Track class cut `(ictr == 1) | (tt == 1) | (tt == 2)`:

```python
track_class_ok = (ictr == 1) | (tt == 1) | (tt == 2)
```

**Concern:** This includes (IsCentralTrk==1 OR TrackType ∈ {1,2}). A track with `ictr=1, tt=3` passes — but does the H1OO matcher even MATCH non-central tracks? If `IsMatched` is set only for central tracks, then this OR is moot but invites confusion. If non-central matched tracks exist, they shouldn't enter SIDIS multiplicity.

**Severity:** Open — depends on H1OO matcher semantics.

**Fix:** Assert `(IsMatched == 1).implies(ictr == 1)` in a one-off smoke test.

### C1M4. `compute_gen_bin:303-307` — Gen fid for V0 rejection is `IsV0DaughterGen == 0`, while at reco the equivalent is `IsMatchedV0Daughter == 0` (line 533). These come from DIFFERENT branches and DIFFERENT semantics.

```python
# Gen (compute_gen_bin)
v0 = (batch["IsV0DaughterGen"] == 0)
# Reco matched (compute_reco_bin)
not_v0_match = (batch["IsMatchedV0Daughter"] == 0)
```

**Concern:** `IsV0DaughterGen` flags whether the GEN track came from a V0 decay in MC truth. `IsMatchedV0Daughter` flags whether the matched gen partner is a V0 daughter. **These should be equivalent for matched events** (same gen particle, same V0 status). For unmatched gen → only `IsV0DaughterGen` available. **Verify they agree for IsMatched==1 events.**

### C1M5. `EVENT_CUTS_GEN:122` has only Q²/y, no Empz cut, while `GEN_FID_OPTION_C:123-126` adds Empz to the FID check.

This is technically fine — Empz is part of the fiducial definition, not the event selection — but the split is opaque. The fid check at line 304-309 mixes:
- `evt_ok` (event-level Q²/y)
- `fid` (track-level eta/xF + event-level empz)

**Concern:** Future edits might break the split. Document or merge.

---

## Latent risks

### C1L1. `_compute_pTh_cm_and_z:174` — `pT_h_cm = np.where(qmag > 0, pT_h_cm, np.nan)` — sets pT_h to NaN when q=0. q (virtual photon 4-vector) being zero means the event is non-DIS (q² = -Q² > 0 always for DIS), so q=0 only happens with bad reco. The NaN propagates correctly into `i_zp = -1` → row dropped. OK.

### C1L2. `compute_gen_bin:336-338` — beam-energy fallback to 27.6/920:
```python
Eb = np.where(np.isfinite(Eb) & (Eb > 0), Eb, 27.6)
Ep = np.where(np.isfinite(Ep) & (Ep > 0), Ep, 920.0)
```
Uses nominal values for missing gen beam energies. This is per-event scalar so the assignment is correct, but: HERA-II beam energies vary slightly by period (920 → 920.0, but 27.5 vs 27.6 GeV electron). Fallback to 27.6 is conservative; if some event has Eb=27.5 (rare), gen pTh computed with wrong electron energy → small bias. Use period-appropriate fallback.

### C1L3. Sentinel-row handling via `np.errstate('ignore')` at line 588 — silences ALL numpy errors in the chunk. This includes legitimate errors that should fail loudly (e.g., dividing by a real zero in some new code path).

---

## Recommended next implementation check

**Most decisive:** Verify the **VtxZ asymmetry (C1F1)**. The fix is to either remove reco's `|PVz|<35` cut (rare in DIS analyses to apply a vertex cut without gen counterpart) or to **explicitly compute the per-bin VtxZ-cut efficiency loss as a systematic** (~0.3% globally, larger in forward bins).

If VtxZ cut is intentional and well-motivated, add a `σ_VtxZ_cut` systematic estimated by varying the cut to ±5 cm and propagating per-bin.
