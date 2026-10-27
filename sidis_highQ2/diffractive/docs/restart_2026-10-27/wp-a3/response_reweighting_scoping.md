# WP-A3: can a forward-veto response reweight rescue the diffractive restart?

Date 2026-10-27. Scoping study after the G1 and G1′ KILLs. It answers the user question "Do you think we can get around
this with reweighting?". `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Parent plan
`.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md` (Gates, WP-A, WP-A2); G1′ record
`D/docs/restart_2026-10-27/wp-a2/g1prime_verdict.md`. Every number here comes from the odd split unless it is marked
"even". [V] = verified from a file, [I] = inference or expert judgement.

**Short answer.** Partly for the Plug *rate*, not for anything that kills the gate.

- A data-driven veto-state map from the no-gap control brings the gap-sample Plug fail fraction from |Δf_fail| ≈ 0.21-0.24
  to ≤ 0.019 for nd4cat with maps XA/XB [V].
- The same map does not fix the Plug spectrum shape (0.26-0.41 against a 0.10 limit), FMD/FTS N−1 (best admissible
  0.62-0.67 against 0.25) or the LAr η_max edge (p ≤ 6e-8) [V].
- The map leaves the edge in place; WP-A2 H2/H6 and the no-veto H4 failure show it is not a veto or x-slice effect. No
  η_max cut sits inside a well-simulated LAr region (the drift starts near η ≈ 2.0-2.25) [V].
- The map makes the signal veto efficiency depend on the ND generator by 14/22/49% (D0/D1/D2) [V]. **Close out**; no G1″
  without the §6 `user:` scope changes.

## 1. What was run

- **Skim** [V]. `D/scripts/restart_2026-10-27/wp-a3/skim_wpa3.py` (md5 `aa9fb3d6…`), cluster 5912805, 17 jobs, 0.00378
  Condor-days: per-event A_pre skims, both parities, `D/data/restart_2026-10-27/wp-a3/skims/*.npz` (470 MB, not committed).
  Selection, D cells and signed-v3 weights are copied from `wp-a2/n1_wpa2.py`; E−p_z is joined from `ChargedTot`; n_pre0
  equals the WP-A2 S2 counts for all 17 labels.
- **Analysis** [V], one Condor job per run (scripts in `D/scripts/restart_2026-10-27/wp-a3/`).
  - Run 1 (coarse Plug classes): `fact_wpa3_run1.py` (md5 `eb7fb81b…`), cluster 5912808, output
    `D/data/restart_2026-10-27/wp-a3/fact/fact_results.json` (sha256 `8101d7aa…`).
  - Run 2 (finer Plug classes, appended to the docstring after run 1 had shown the rate closing but not the shape):
    `fact_wpa3.py` (md5 `654d9e7a…`), cluster 5912809, output `fact_results_fine.json` (sha256 `b3aae947…`).
  - Total WP-A3: **0.00637 of 0.2 Condor-days**, tally `D/data/restart_2026-10-27/wp-a3/condor_tally.csv`
    (`h1-naf-operator`). Condor files are in `D/condor/restart_2026-10-27/wp-a3/`.
- **Test `skim_identity_s4`: PASS** [V]. Without the map, the odd-split C_N1_PLUG and C_SB per D cell reproduce the WP-A2 S4
  partials exactly (rtol 1e-9). The baselines therefore equal G1′; for 7071, Δf_fail = −0.210/−0.217/−0.224 and the Plug
  shape is 0.982 (`g1prime_verdict.md:33-34`).
- **Design**. The base design was frozen in the `fact_wpa3.py` docstring before any skim was read.
  - Control CTRL = A_pre ∧ 35 < E−p_z < 70 ∧ `etaMaxClus08` ≥ 3.3, with no veto. It is ND-dominated and disjoint from the gap
    sample by the η cut.
  - Veto state s = Plug class × selFmd × selFts. Run 1 has 8 Plug classes (edges 0/0.1/1/3/5/7/10/20 GeV) plus non-finite.
    Run 2 has 0.5 GeV classes up to 10 GeV, then 10-20, 20-50 and > 50 GeV.
  - The map is r_s(x) = P_DATA(s|x)/P_MC(s|x), derived on CTRL even with MC = DIFF8 + ND(chain) under signed v3 weights.
    It is applied as a per-event MC weight that preserves the MC x-distribution, and it is tested on odd.
  - x-sets: XA = D cell; XB = XA × run period; XC = XB × z_vtx (4 bins) × E−p_z (3 bins); XE = XB × η08 band (3.3-3.4,
    3.4-3.5, ≥ 3.5). XD = XB × (nPrt > 0) is degenerate with XB: nPrt = 0 everywhere (`D/docs/t4/my_forward_correlation_2026-10-19.md:52`).
  - The brief's first candidate, a LAr energy sum in 2.8 < η < 3.2, **is not stored** [V]. The only forward LAr quantities
    are the η_max variants and `nMixedClus` (`D/pkg_diffractive_smoke/src/DiffractiveMachinerySmoke.C:265-330`, `h1-mc-expert`).

## 2. Factorisation test, odd split (G1 thresholds unchanged)

**(T0) Closure inside the control** [V], run 2. After the map |Δf_fail| ≤ 0.016 in every cell for all chains and x-sets,
and the 0-10 GeV Plug shape closes (0.048-0.076, against 0.79-0.90 before). **T0 still fails as pre-registered:** the full
G1 grid (0-50 GeV) stays at 0.22-0.42, driven by bin 95 (47.5 GeV) inside the single 20-50 GeV class.

**(T1) Is the control ratio flat in η_max?** [V] Even split, data/MC Plug-fail ratio over the three CTRL η08 bands, D0-D2:
7071 1.04-1.16, 7091 0.95-1.00, nd4cat 1.05-1.16. It varies by ≤ 0.06 across bands within a chain but by up to 0.20
between chains: it depends on the ND model, not on η_max.

**(T2) Transfer to the gap sample.** Odd split, stack-C N−1 with η08 < 3.3, run 2. XB equals XA to ±0.003 for every
entry; nd4cat XB is shown.

| chain | map | Plug shape, G1 grid / 0-10 GeV (≤ 0.10) | Δf_fail D0 / D1 / D2 (\|·\| ≤ 0.02) | FMD / FTS N−1; best admissible joint (< 0.25) | η04 edge 2.8-3.4, FIT ALL: p, max dev |
|---|---|---|---|---|---|
| 7071 | none | 0.982 / 0.683 | −0.210 / −0.217 / −0.224 | 2.16 / 0.71; 0.775 | 6e-30, 0.49 |
| 7071 | XA | 1.288 / 0.279 | −0.008 / +0.022 / +0.022 | 1.74 / 0.59; 0.662 | 1e-8, 0.29 |
| 7071 | XC | 1.242 / 0.407 | −0.004 / +0.027 / +0.024 | 1.67 / 0.61; 0.655 | 5e-8, 0.26 |
| 7091 | none | 0.965 / 0.728 | −0.214 / −0.225 / −0.238 | 1.70 / 0.71; 0.744 | 1e-33, 0.59 |
| 7091 | XA | 1.251 / 0.258 | −0.016 / −0.020 / −0.038 | 1.36 / 0.60; 0.627 | 4e-10, 0.35 |
| 7091 | XC | 1.409 / 0.311 | −0.011 / −0.026 / −0.034 | 1.31 / 0.60; 0.620 | 2e-9, 0.35 |
| nd4cat | none | 0.953 / 0.696 | −0.210 / −0.218 / −0.230 | 2.14 / 0.68; 0.763 | 7e-33, 0.49 |
| nd4cat | XA | 1.187 / 0.282 | +0.002 / +0.019 / +0.012 | 1.72 / 0.59; 0.666 | 1e-9, 0.30 |
| nd4cat | XB | 1.186 / 0.280 | +0.001 / +0.019 / +0.014 | 1.72 / 0.59; 0.666 | 1e-9, 0.30 |
| nd4cat | XC | 1.040 / 0.369 | +0.011 / +0.019 / +0.020 | 1.65 / 0.58; 0.654 | 6e-9, 0.26 |

Data f_fail is 0.272/0.300/0.337 (stat 0.003/0.004/0.007) [V]. Run 1 (coarse classes, all five x-sets) gives the same
picture; XE does no better (nd4cat shape 1.37/0.39). 1.96% of gap data events are outside the E−p_z window and are clamped
into the XC edge bins.

Reading [V]:

- **Rate.** The rate transfers. The joint state learns that in data the Plug fires *alone*, with FMD and FTS passing, about
  4× more often than in MC [I]. This is the occupancy excess found in WP-A2 H5(ii).
- **Shape.** The Plug shape does not transfer. After the map, the 0-10 GeV ratio is still 1.25-1.29 at 2.0-2.5 GeV, and
  1.1-1.4 at 9.5-10.5 GeV.
- **High-energy bins.** The G1-grid driver is bins 64 and 66 (32 and 33 GeV). They reach 1.7-2.4 after the map, against
  0.24-0.29 before it; their neighbours sit at 0.4-1.0. A few large-weight MC events amplified by the fail-state weights
  are the likely cause [I].
- **Clause (a).** No chain and no x-set passes G1(a), which needs both shape and rate. The rate half alone passes for
  nd4cat XA/XB; XC is at 0.020 in D2.
- **x-set and clause (b).** Period, vertex and E−p_z (XB, XC) change nothing beyond statistics (no run trend, WP-A2 H2).
  FMD/FTS N−1 improves by about 15%, but FMD stays at 1.3-1.7, so G1(b) fails.

**η_max edge (where the ND template fit fails)** [V]:

- In the veto-passing fit basis, the map moves the edge only through the correlation of veto state with η_max. FIT ALL
  max dev goes from 0.49-0.59 to 0.26-0.35, and p from 1e-30-1e-33 to ≤ 6e-8. That is still about 6 orders of magnitude
  below 0.05.
- In the no-veto H4 sample, max dev is unchanged (observed): 0.18-0.21 before and after. p moves from 5e-43-2e-111 to
  2e-28-7e-62.
- By construction, this design cannot show the edge factorising in period, z_vtx or E−p_z, because w′ keeps the MC
  x-distribution. The evidence against such a factorisation comes from elsewhere: WP-A2 H6 (geometry shift 0.0004 against
  the 0.03 needed), H2 (no run trend), and the H4 edge failing with no veto at all. `h1-mc-expert` reads the edge as a LAr
  clustering or acceptance-edge feature [I].
- An η-conditioned map (XE) would tune the edge by construction. That is circular, and it is not used for the edge claim.

**Where is the well-simulated LAr region?** [V] Even split, from the WP-A2 S2 partials, using
`D/scripts/restart_2026-10-27/wp-a3/etacut_wpa3_variants.py` (variant C).

Cut-removed `etaMaxClus08`, ALL, DIFF8 + ND normalised in 1.0 ≤ η < 2.5: the first 0.05 bin outside 10% is at η = 2.55
for all three chains (outside 15%: 2.55-2.8). Coarse data/MC (nd4cat) at η = 1.75/2.0/2.25/2.5/2.75/3.0/3.25 is
1.01/1.02/1.07/1.17/1.38/1.68/1.42. There is no plateau at 3.0 or 2.8.

Signal cost of a tighter cut [V]. SIGNAL6 MC, efficiency relative to η08 < 3.3, from `etacut_wpa3_cost.py`:

| cut | D0 eff | D1 eff | D2 eff | M_Y > 1.6 share (D0/D1/D2) | data kept, even ×2 (D0/D1/D2) |
|---|---|---|---|---|---|
| < 3.3 | 1 | 1 | 1 | 0.027/0.030/0.031 | 31770/16678/6994 |
| < 3.0 | 0.643 | 0.547 | 0.445 | 0.027/0.031/0.048 | 13558/5980/1992 |
| < 2.8 | 0.525 | 0.412 | 0.316 | 0.027/0.030/0.025 | 9850/4010/1112 |
| < 2.6 | 0.446 | 0.337 | 0.251 | 0.028/0.032/0.026 | 8070/3114/754 |

The signal η_max spectrum peaks at 3.2-3.3, so a tighter cut removes elastic and PDISS events together: M_Y migration stays
near 3% and no purity is gained. A cut at 2.6 keeps 25-45% of signal and 11-25% of data; it is a `user:` item (§6) and
does not fix the Plug or FMD/FTS clauses.

## 3. Circularity guard

- **What the conditioning avoids** [V]. x uses no gap-sample events (CTRL η08 ≥ 3.3) and no Plug or LAr-edge variable
  (apart from the diagnostic XE). The map is derived on even and tested on odd in a *different* region, so the transfer
  test is genuinely out of sample.
- **What it cannot avoid** [V]. P_MC(s|x) in CTRL comes from DIFF8 + ND(chain), so r_s inherits the ND generator's forward
  flow. The detector simulation is the same in every chain, so differences between chains can only be ND physics.
  - With XA, the Stage-B signal weight changes by −9 to −13% for nd4cat (D0 11243 → 10246, D1 4550 → 3978, D2 1130 → 979)
    and by +4/+7/+28% for 7091 (11660/4871/1445). Full chain range: **14% / 22% / 49%** (D2 minimum 972, 7071).
  - No stored Plug-independent x separates the noise part, which is event-independent and so valid for diffractive MC,
    from the flow part, which is ND-specific.
  - A multiplicative r_s is also the wrong model for additive noise. The noise-overlay alternative needs a random-trigger
    (RT) sample, which does not exist (item (iii) below) [I, `h1-analysis-critic`].
  - `h1-mc-expert` [I]: elastic events (9012) see only noise and beam background (ePlug mean 1.74 GeV), PDISS events see
    M_Y fragments (9158, 5.71 GeV), and ND events at η_max ≥ 3.3 see mostly remnant flow.
- **Validation needed before any use** [I]:
  - (i) The ePlug = 0 and < 1 GeV shares in the gap N−1 sample, data against a 9012-dominated MC. Noise dominates there.
  - (ii) PDISS 9155/9157/9158 against 9012 (MC only), to size the dissociation share of the Plug > 7 GeV fail fraction,
    which the map must not absorb.
  - (iii) A data random-trigger or empty-bunch sample: unavailable, only LAr/SpaCal/CPC noise pools exist
    (`D/docs/restart_2026-10-27/wp-a/s1c_mc_expert_lookup.md:6-13`); the MC carries RT overlay
    (`D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md` §2). (iv) Published H1 LRG efficiencies: not local,
    only the uncertainties are quoted (`wp-a2/s1_definition_audit.md:19`).

## 4. Uncertainty projection, had the map passed

Per D cell, from run 2 [V]. Y_diff = DATA_SB − ND_SB(mapped); the nd4cat XA ND share of Stage-B is 25%/32%/52%.

Replica rows are 1σ; conditioning and chain rows are full ranges (max − min) over XA/XB/XC and over 7071/7091/nd4cat, so
the quadrature sum is indicative only.

| term (relative, D0/D1/D2) | signal efficiency | diffractive yield via ND subtraction |
|---|---|---|
| map statistics, 20 replicas, XA/XB (1σ) | 1.7-1.8 / 2.3-2.7 / 3.2-3.6% | ND 1.2-2.0 / 2.1-2.3 / 2.4-2.9% → ≤ 0.7 / 1.1 / 3.2% |
| conditioning XA/XB/XC, nd4cat (range) | 2.4 / 4.1 / 4.8% | 2.1 / 3.0 / 6.6% |
| ND chain 7071/7091/nd4cat, XA (range) | 14 / 22 / 49% | 2.5 / 4.1 / 11% |
| even→odd transfer residual Δf_fail/(1−f) | 0.3 / 2.7 / 1.8% | – |
| **indicative quadrature** | **≈ 14 / 23 / 49%** | **≈ 3.3 / 5.2 / 13%** |

The observable is per-event normalised: (1/N_D) dn/dx_F and dn/dp_T*² per charge, in 3 M_X bins that integrate over the
D cells (`wp-c/binning_record.md:17-31`). A truly uniform event-level factor would cancel exactly. The measured map
efficiency is not uniform: it varies by −9 to −13% across D cells, with a 14-49% chain range. It therefore reweights the
D-cell mixture inside each M_X bin, and the signal-efficiency column enters as well [I].

Projected measurement (stat from the `binning_record.md` study CSVs; data stat = 1/√n_sig_pred, a lower bound):

| scenario | bins per charge (total) | data stat (median) | map term (yield; signal-efficiency mixing on top) | map term ⊕ detector median (indicative, not a budget) |
|---|---|---|---|---|
| D7+ frame B, x_IP < 0.03 (`binning_record.md:238`) | 5 (10) | 1.1% | 3.3-13% (+ up to 14-49%) | ≈ 8-15% |
| D7+ frame B, x_IP < 0.025 (`:239`) | 8 / 7 (15) | 1.2% | 3.3-13% (+ up to 14-49%) | ≈ 8-15% |
| P1 survivors under D7+ (`:217`) | 8 (16) | 1.9% | 3.3-13% (+ up to 14-49%) | ≈ 8-15% |

- Detector median = 7.5-7.9% of the 2026-10-21 parent budget (`sidis_highQ2/docs/RUN_STATUS.md:8`), superseded by the
  2026-10-27 release (line 5) and used only as a proxy. The last column omits ND leakage (G2 caps it near 10% per bin), the
  FMD/FTS leg (4%/7% published), the Plug scale and PDISS fakes. The projection is moot, because G1(a) and G1(c) fail.
- "28 bins with ≥ 5× MC" is not a WP-C result: 28 bins per charge is the rejected P1 grid (`:206`), and the 5× request is
  "not yet evaluated" (`:397`).

## 5. Recommendation

**Does not factorise → close out.** The pre-registered ruling input (`fact_wpa3.py` docstring) requires T0 and T2(a) to pass
for some x-set and chain. Both fail: T0 on the G1 grid (0.22-0.42); T2(a) on shape for every chain and x-set (best 0.258 at
0-10 GeV, 1.04 on the G1 grid, against 0.10). Run 2 only gives the map more freedom, so it cannot bias toward a kill.

- Reweighting repairs the Plug fail *rate* (nd4cat XA/XB ≤ 0.019), a possible systematic tool for a future analysis with
  validation (i) or (ii) of §3. It does not reach the Plug spectrum, FMD/FTS N−1, or the LAr η_max edge that sets G1(c);
  reweighting η_max itself would tune the fit variable to data, which is circular.
- No G1″ is proposed. If the user approved the §6 items, a G1″ would keep G1's three clauses and thresholds, freeze the map
  on even, evaluate on odd, and kill after ≤ 3 Condor-days or 5 calendar days. Predicted outcome [I]: (a) fails on shape;
  (c) is untested with toys, but the edge χ² proxy (stricter than the two-template fit) fails at p ≤ 6e-8.

## 6. `user:` items (none applied)

- `user:` Tighten η_max to < 2.6 (< 2.8) to stay roughly inside the well-simulated LAr region: signal efficiency falls to
  0.25-0.45 (0.32-0.53) of its current value, with no purity gain.
- `user:` Drop the FMD/FTS vetoes or move them to a systematic leg (G1(b) is already downgrade-only); this departs from the
  published LRG definition.
- `user:` Replace the Plug veto with a Plug-rate map. The 14-49% ND-chain dependence of the signal efficiency would be
  carried as a systematic, or validation (i) or (ii) of §3 would be required first.
- `user:` A forward-detector simulation, a noise campaign, a producer change (forward-band LAr energy), or a ≥ 5×
  signal-MC request. Each needs new production, which the user excluded from WP-A2.

## 7. Coverage gaps

- G1(c) was not run (no §7.1 two-template toy fit on mapped weights); the map's x_F/M_X dependence was not measured.
- Thin XC cells fall back to XB (ND replica spread up to 24%); the 32-33 GeV MC weight spikes were not traced to events.
- The published LRG efficiencies were not checked (no local copy of the paper).

## Sign-off (`h1-analysis-critic`, 2026-10-27)

RULING: does not factorise → close-out. The pre-registered rule needs T0 and T2(a) to pass for some x-set and chain.
Both fail: T0 on the G1 grid (0.22-0.42), and T2(a) on shape, best 0.258 (0-10 GeV) and 1.04 (G1 grid), against 0.10.
Run 2 was added after run 1, but it only enlarges the map's freedom and cannot bias toward the kill. The partial rate
closure for nd4cat does not satisfy the conjunctive G1(a). The map carries ND-model forward flow into the signal
efficiency (chain spread 14/22/49% D0/D1/D2), which confirms it is not a detector-only response.
SIGNED: yes, conditional on findings 1 (T0), 8 (cancellation) and 9 (total relabel), which are applied above.
