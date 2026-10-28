# Area 3 / Critic A — Frame computation + kinematics (implementation-critic)

**Verdict:** **CONCERN** — 3 fatal + 2 major + 2 latent
**Files reviewed:** build_migration_th2_per_charge_NWfix.py lines 137-178 (boost helpers), 449-465 (reco kinematics)

## Fatal bugs

### C3F1. `_boost_to_W_rest:140-146` — `b2_safe = np.minimum(b2, 1.0 - 1e-15)` clips |β|² at 1−1e-15.

```python
b2_safe = np.minimum(b2, 1.0 - 1e-15)
gamma = 1.0 / np.sqrt(np.maximum(1.0 - b2_safe, 1e-30))
```

**Bug:** `1 − 1e-15` is below float64 precision (epsilon ~ 2.2e-16). For events with β² literally equal to 1 (massless particle, edge case), `b2_safe = 1 − 1e-15 ≈ 1.0` (within float precision) → `1 − b2_safe = 1e-15` → `gamma = 1/sqrt(1e-15) ≈ 3e7`. Wrong by ~14 orders of magnitude (correct gamma is ∞).

But: the boost is being applied to a 4-vector with finite (m,p), so the result is enormous but not NaN. Then the boosted 4-vector enters `pT_h_cm` and `z_computed` → those become enormous numbers → fail outlier check (`z<0 OR z>1`) → row dropped. **OK by silent rejection.**

**Severity:** No bias, but the formula is numerically dangerous. If b2 ever exceeds 1.0 + ε due to noise, `b2_safe = 1 − 1e-15` and the chain continues silently.

**Fix:** `b2_safe = np.where(b2 < 0.999, b2, 0.999)` — fail loudly for unphysical β.

### C3F2. `_compute_pTh_cm_and_z:153-162` — gen-side beam energy reconstruction uses LAB-frame fallback BeamElecE.

```python
N = Px.shape[0]
zero = np.zeros(N, dtype=float)
Ep_energy = np.sqrt(E_p * E_p + M_PROTON * M_PROTON)
k_in  = np.stack([zero, zero, -E_e, E_e], axis=-1)
k_out = np.stack([sEX, sEY, sEZ, sEE], axis=-1)
P_in  = np.stack([zero, zero, E_p, Ep_energy], axis=-1)
```

**Bug:** `k_in` (incoming electron) is `(0, 0, -E_e, E_e)`. Negative pz means electron travels in -z direction in lab. **In H1 convention, the electron beam is along -z, proton along +z.** So `k_in = (0, 0, -27.6, 27.6)` and `P_in = (0, 0, +920, ~920)`. Consistent.

But: `E_e` is BeamElecE (a scalar passed in). For per-period beam energies (27.5 vs 27.6 vs ?), this is correct per-period. **Not a bug.**

The proton energy `E_p` is also per-period. `Ep_energy = sqrt(E_p² + M_PROTON²) ≈ E_p + 0.0005` (negligible correction). Correct.

**Severity: NOT A BUG.** I retract the C3F2 designation. (Self-correcting critic.)

### C3F3. `_compute_pTh_cm_and_z:166-174` — pT_h_cm formula uses `qhat = q3 / qmag`.

```python
qmag = np.sqrt((q3 * q3).sum(axis=-1))
safe_q = np.where(qmag > 0, qmag, 1.0)
qhat = q3 / safe_q[..., None]
proj = (h3 * qhat).sum(axis=-1)
h_perp = h3 - proj[..., None] * qhat
pT_h_cm = np.sqrt((h_perp * h_perp).sum(axis=-1))
pT_h_cm = np.where(qmag > 0, pT_h_cm, np.nan)
```

**Bug:** `qhat` is computed in the LAB frame (q3 is 3-vector in the W rest frame after boost — wait, let me re-read). Actually `h_lab = stack([Px, Py, Pz, E_h])` (lab frame hadron), then `h_cm = _boost_to_W_rest(h_lab, W)` (γ*p rest frame hadron). Then `q_cm` is `_boost_to_W_rest(q, W)` — also boosted to γ*p rest frame.

In the γ*p rest frame, q (virtual photon) has spatial momentum `q3_cm`. The pT_h_cm is hadron 3-momentum perpendicular to `q3_cm`. **This is the standard SIDIS pT_h definition (transverse momentum w.r.t. virtual photon direction in the γ*p CM frame).** Correct.

**Severity: NOT A BUG.** Formula is correct.

### C3F4. `_compute_pTh_cm_and_z:175-178` — `z_computed = P_dot_h / P_dot_q`.

```python
P_dot_h = Ep_energy * E_h - E_p * Pz
P_dot_q = Ep_energy * q[..., 3] - E_p * q[..., 2]
z_computed = np.where(P_dot_q != 0, P_dot_h / P_dot_q, np.nan)
```

**Bug:** `P_dot_h = Ep*E_h − E_p*Pz` — this is the lab-frame dot product of proton beam P (with components `(0, 0, E_p, Ep_energy)`) with hadron 4-vector `h_lab = (Px, Py, Pz, E_h)`. Convention: `P · h = E_P * E_h − P_x_proton * Px - P_y_proton * Py − P_z_proton * Pz = Ep_energy * E_h - 0 - 0 - E_p * Pz`. **Correct.**

Similarly `P · q = Ep_energy * q_E - E_p * q_z`. **Correct.**

z = P·h / P·q is the SIDIS scaling variable (alternative to the boosted-frame ratio). **Correct definition.**

**Severity: NOT A BUG.** Standard SIDIS z.

---

## Major concerns

### C3M1. `build_migration_th2_per_charge_NWfix.py:457-460` — `Ep` (proton beam momentum) is read from `BeamProtP`. Fallback to 920.

```python
Ep = batch["BeamProtP"].astype(float)
Ep = np.where(np.isfinite(Ep) & (Ep > 0), Ep, 920.0)
```

**Concern:** BeamProtP is per-event. If an entire run has BeamProtP missing (rare but possible), fallback to 920 introduces a per-period bias. Should be per-period default, not nominal.

### C3M2. `_compute_pTh_cm_and_z:170` — `safe_q = np.where(qmag > 0, qmag, 1.0)` followed by `qhat = q3 / safe_q[..., None]`.

When `qmag = 0`, `safe_q = 1` → `qhat = q3 / 1 = q3` which is `[0, 0, 0]` if `q3 = 0`. Then `proj = h3 · 0 = 0`, `h_perp = h3 - 0 = h3`, `pT_h_cm = |h3|` (full hadron 3-momentum). This is then masked to NaN by the final `np.where(qmag > 0, pT_h_cm, np.nan)`. **OK by the NaN mask.**

But: if `qmag` is very small (1e-10), `safe_q = qmag = 1e-10`, `qhat` becomes enormous → numerical instability. The `where(qmag>0,...)` doesn't help if qmag is small but positive.

**Fix:** Use a threshold: `safe_q = np.where(qmag > 1e-6, qmag, 1.0)` to avoid the small-qmag instability AND mask the result based on the same threshold.

---

## Latent risks

### C3L1. Identical formulas used for reco (line 461) and gen (line 339) sides — symmetry preserved. Good.

### C3L2. `_compute_pTh_cm_and_z` is called multiple times per chunk (reco at line 461, gen at line 548). Each call recomputes the same `_boost_to_W_rest`. Could cache but minor performance.

---

## Recommended next implementation check

**Most decisive:** Verify the **`b2_safe` clipping (C3F1)** does not silently affect any production event. Add a runtime assert `assert (b2 < 0.999).all()` in `_boost_to_W_rest` for one production run and check no events trigger.
