# Hostile implementation critic: F1 (nominal-consistent shifted electron kinematics), 2026-10-22

Scope: the new NAF code in `scripts/unfolding/final_detlegs_optA_2026-10-21/`:
- `f1_nominal_consistent.py` (md5 7edf4cfe…);
- `build_migration_th2_per_charge_cone_s2prime_shifted_optA.py` (2930c702…, compared against `*.pre_f1.py`);
- `build_data_vector_v4_alphaveto_shifted_f1.py` (7c6feb09…, compared against the 2026-10-14 shifted DV builder 2108365c);
- `empz_hfs_shim.py` (5e1d82ef…);
- `run_job.sh` pins;
- the production subs and gate scripts.

I made no remote writes. My scratch scripts are in `scripts/critic_impl/`: f1_nonzero.py, f1_xf_odd.py, f1_xf_mc.py, f1_gate_spot.py.

**Verdict: DISPATCH-WITH-FIXES.** I found no error in apply_f1 at nonzero shift. Signs, the θ/φ conventions, the E'_ES scaling and dtype are all correct. Every selection and binning variable now goes through F1 in both builders. The one fix is cheap and does not need to precede dispatch: make the F1 contract (zero == nominal) a hard full-sample gate (F1-M1).

## Findings
| # | Sev | Finding | Fix |
|---|---|---|---|
| F1-M1 | MAJOR (before using results) | The defining F1 contract, `--shift zero` == nominal bitwise, is gated only on 4 of 174 MC shards and on the data DV. The production gates check neither part of it. `merge_and_check.py` checks only gen-side identity and shard counts; `check_dvs.py` checks file accounting, HFS null vs old, and nominal−rec2 == 658. One gap matters here: rows where `shift_electron` returns NaN (scatEE ≤ 0, \|p\| = 0) get NaN deltas even at zero shift. Any such row that passes the nominal selection in the untested 170 shards would make zero ≠ nominal. | Add two hard gates. (a) Merged `zero` response == OPT-A nominal merged response, all 68 hists, RG and DJ. (b) `dv_zero_660` == `dv_nominal_660`, bitwise `n_raw`. Both cost nothing and make the electron-leg definition exactly (shift − nominal) = (shift − zero). |
| F1-m1 | MINOR | `apply_f1` takes the raw `BeamElecE`/`BeamProtP` for z/pT and xF. The nominal builder's z/pT call uses the fallbacks `Eb>0 → 27.6` and `finite&>0 → 920`. This is identical whenever the beam branches are valid: 0 bad rows in data d06_0050, em0405 d05_0001 and MC 7097_0060, and the gates are bit-identical. It is still fragile. | Apply the same fallbacks inside apply_f1, or assert finiteness and positivity there. |
| F1-m2 | MINOR / INFO (pre-existing nominal-chain issue, not F1) | **In MC, stored `xF_SIDIS_es` uses the true matched-particle mass.** The Stage-2 eΣ-photon replica reproduces stored xF to median 5e-11 (p99 1e-7) in data. In MC it does so only for π-matched rows: 21% of selected rows differ (up to Δ ≈ 0.06), and they are exactly the K, p, e, μ-matched tracks. With the kaon/proton/electron/muon mass they match exactly (2307/911/1673/352 rows). Data necessarily uses the pion mass. Effect on the xF<0 cut in this MC file: 1 hadron-matched row of 21,725 flips (5e-5), so it is negligible. For F1, the MC xF delta uses the pion-mass derivative for K/p rows. That is a second-order error on a ~2%-relative ΔxF, so also negligible. | Log it as a nominal-chain note (MC reco xF should use the pion hypothesis, as data does). No production change needed. |
| F1-m3 | INFO (pre-existing) | Some rows carry a stored xF sentinel of −999 (x_replica = 0, i.e. W ≤ 0 in the Stage-2 formula): 4/9,936 selected rows in data d06_0050 and 19/24,634 in MC 7097_0060. They pass `xF<0` in the nominal chain. F1 keeps them at −999 + 0, so it behaves like the nominal. (The legacy core gave NaN and dropped them, which was part of the old zero ≠ nominal.) | No production change. Note it for the nominal chain. |
| F1-m4 | INFO | The legacy `compute_shifted_kinematics` is still called first. Its outputs are dropped except `Sigma_h` and `W`, and neither is consumed by any selection or binning. `y_sigma`/`Q2_sigma` are set to absolute shifted values (not deltas) but are diagnostics only; nothing consumes them. | None. |

## Independent verification (what I reproduced)
1. **Consumers.**
   - In the response builder, the only `kin_shift` keys consumed are `Q2_es_shift`, `Y_es_shift`, `Empz_shift` (event fiducial), `xF_internal_shift` (hemisphere), `scatE{X,Y,Z}_shift` (α-cone), `z_shift` and `pT_h_shift` (z-guard and local bin), and `Q2_es_shift` with `x_es_shift` (cell) (L1419–1590). apply_f1 overwrites all of them.
   - The DV builder consumes the same set (fiducial, xF, cone, z-guard, binning).
   - The diffs against the pre-F1 builders contain only the apply_f1 call, the imports, and the added branches (`Yh`; `Q2e` in the DV). The two builders are consistent.
2. **Stored nominal == F1 formulas at zero, so the deltas are exact.** At nominal, `esigma_kinematics(scatEE, θ, 2·27.6·Yh, 27.6, 920)` reproduces the stored Q2es/Yes/Xes (median ratio 1.00000 on data e+p, e−p and MC). The Stage-2 xF replica reproduces the stored data xF to 1e-7. So F1 at nonzero shift equals the nominal formulas evaluated on the shifted electron, not an approximation.
3. **Nonzero shifts (median relative delta; F1 vs the legacy core's own shift − zero).** Files: data ep0607 d06_0050, data em0405 d05_0001, MC RG 7097_0060.

   | quantity | E +1% | E −1% | θ +1 mrad | θ −1 mrad |
   |---|---|---|---|---|
   | Q2 | +1.000% | −1.000% | −0.264% | +0.264% |
   | Y | −1.37% | +1.40% | −0.049% | +0.049% |
   | x | +2.41% | −2.37% | −0.215% | +0.215% |
   | Empz | +0.69% | −0.69% | +0.025% | −0.025% |
   | z | +2.09% | −2.00% | +0.079% | −0.079% |
   | xF | −2.34% | +2.25% | −0.089% | +0.089% |

   - F1 and the legacy core deltas agree to ≤1e-4 relative, with correlation 1.0000 for Q2, Y, x, Empz, z and pT. For xF the correlation is 0.96–0.99, as expected, because the two use different photon definitions.
   - The θ sign is physical: θ is measured from +z (the proton direction), and a larger backward angle lowers Q2.
   - The effective electron matches the core's shifted electron to ≤2e-6 GeV.
   - θ shifts by exactly ±1 mrad and φ is preserved.
   - E'_ES scales by (1+dE) under an energy shift and is unchanged under a θ shift. Both are correct, since Q2e ∝ E'(1+cosθ).
   - No new non-finite values appear. xF<0 flips are 0–4 rows per file.
   - All outputs are float64.
4. **Gates spot-checked.**
   - `smoke_f1e/zero_rapgap31_0060` and `zero_djangoh14_0086` against the OPT-A nominal shards: 68/68 hists bit-identical.
   - `dv_zero_660` == `dv_nominal_660` bitwise (3,874,767 tracks).
   - `elecE_p1pct` RG_0000 reco-side yield vs nominal: +0.18% / +0.19%.
5. **Campaign.**
   - 2958 shard lines (1914 + 1044 `hfsnull` written to `shards_null/`) and 18 DV lines. All outputs are unique and none exists yet.
   - `prod_shards.sub` and `prod_dvs.sub` have 4 GB memory and `on_exit_hold` on a nonzero exit.
   - `run_job.sh` md5-pins all imported files, including f1 (7edf4cfe) and the shim (5e1d82ef).
   - The shim now uses the fixed 27.6 GeV beam energy and refuses `library != "np"`.
   - `merge_and_check.py` asserts 73/101 shards and `processed N/N` (the builders print `[write] <path>`, so the log mapping works), merges 34 responses, and checks gen-side bit identity.
   - `check_dvs.py` checks 660/660/0 file accounting.
