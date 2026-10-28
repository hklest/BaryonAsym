# Current hemisphere of the γ*–IP system inside the CJC: where the events fall (2026-10-28)

`D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`. Numbers: `D/data/restart_2026-10-27/cjc_containment/cjc_containment.json`;
figures and provenance: `D/plots/restart_2026-10-27/cjc_containment/`; scripts: `D/scripts/restart_2026-10-27/cjc_containment/`. No selection, gate or plan changed.

## Selection definition
- Sample (LRG): A_pre, D cells, etaMaxClus08 < 3.3 (or sentinel), E_Plug < 7 GeV, selFmd, selFts, reco 0 < x_IP < 0.03, 4 ≤ M_X ≤ 40 GeV.
  M_Y and |t| are gen-level cuts and cannot be applied at reco. y < 0.7 and E−p_z are not applied (the same base as the gate-evidence samples);
  the "meas" rows below add 0.1 < y_eΣ < 0.7 and 35 < E−p_z < 70 GeV.
- Frame: WP-C frame B (p_X = q_eΣ + ξP with mass M_X,rec). x_F is Pomeron-positive as in the plan (D6) and the WP-C record (u = −q̂*);
  the current (photon) hemisphere is **x_F < 0**.
- The trees hold no cluster list: ChargedTot carries tracks only, and DiffTot carries the HFS sum. The hemisphere test therefore uses tracks.
  Tracks have p_T > 0.15 GeV, are primary, central or combined, lie ≥ 0.2 rad from the scattered electron, and have no η cut.
  The stored tracks reach |η| ≈ 2.2, so a track sample at |η| < 1.6 alone could not see a loss.
- **Contained (nominal, TRK):** ≥ 1 current-hemisphere track, and ≥ 90 % of that track energy at 20° < θ < 160° (|η| < 1.735).
  Not contained = defined but failing. Undefined (U) = no ChargedTot row, or no current-hemisphere track (2.3 % of data).
- Why tracks: the observable is charged hadrons, so the tracker must see the current hemisphere; neutral energy is not tested.
- **Proxy, no hadron list (PROXY):** η_hi = the largest lab η of a massless x_F < 0 direction. It is computed by boosting the X-frame equator
  to the lab (verified against 2×10⁵ random directions to 1e-3). PROXY = η_hi < 1.735.
- Gen check (DIFF8): charged π/K/p on the X side, G2 frame, all η. The same 90 % rule is applied.
  - Reco TRK agrees with gen in 97.1 % of LRG events; gen-contained given reco-contained is 0.977.
  - Reco PROXY agrees in 95.4 %.

## How the proxy tracks the full definition
- The forward boundary is never the problem. η_hi < 1.735 for 100 % of defined LRG data events (27 undefined) and 99.98 % of MC; in data the median η_hi is ≈ 0.5 and 99.8 % lie below 1.4.
  At Q² > 150 GeV² and x_IP < 0.03 the X system is central (median y_X ≈ 0.2).
- PROXY therefore accepts everything (TRK-contained ⊂ PROXY). All 693 TRK failures are PROXY-contained, and all failures lose their energy **backward**: 2030 GeV of track energy at η < −1.735 and 0 GeV forward, toward the SpaCal.
- The 95.4 % proxy/gen agreement is therefore the contained base rate. No geometric proxy can test the backward edge, where every failure
  occurs: the lab −z direction lies inside the current hemisphere in about 79 % of preselected em06 events (η_lo = −∞).

## Where the contained events fall (data, odd+even; per parity within ±0.5 %, ±0.8 % in D2)
| | D0 | D1 | D2 | ALL | odd / even (ALL) |
|---|---:|---:|---:|---:|---|
| contained fraction, data | 0.943 | 0.962 | 0.943 | **0.947** | 0.947 / 0.947 |
| MC full (7071 / 7091) | 0.949 / 0.949 | 0.968 / 0.969 | 0.986 / 0.984 | **0.954 / 0.954** | 0.952 / 0.956 (7071) |
| DIFF8 only (weighted / raw) | 0.948 / 0.943 | 0.970 / 0.964 | 0.989 / 0.966 | **0.954 / 0.948** | |
| data events C / N / U | 16216 / 588 / 402 | 4967 / 90 / 104 | 482 / 15 / 14 | 21665 / 693 / 520 | |
- η_max of contained data: 24.8 % < 2.0, 37.5 % < 2.5, 63.0 % < 3.0, and 37.0 % at 3.0–3.3. MC (7071): 27.9 / 42.6 / 69.1 %.
- Converse: of the η_max < 2.0 (2.5) data, 92.9 % (93.2 %) are contained, against MC 94.8 % (94.8 %). The 5796 (8712) events break down as in the table above.
- Ranges (5–95 %): M_X 7.1–31.9 GeV (median 17.1), x_IP 0.0073–0.029, β 0.18–0.83, y_Σ 0.14–0.62. Not-contained events sit at
  higher y (median 0.55 against 0.34) and higher M_X (24 against 17 GeV), as expected for a current jet thrown backward.
  Per-cell, per-category and per-parity distributions of η_max, x_IP, M_X, β, y, y_X and η_hi are in the JSON `hists`.

## Is the forward problem benign in the contained subset? No: it is the same as in the full sample
All rows are N−1, contained subset (full sample in brackets), both parities, chain 7071 (7091), MC = DIFF8 + ND with fitted scales.
- Plug: f_data 0.180 ± 0.002 against MC 0.043 (0.045). Δ = **+0.137 ± 0.003** (+0.135) [full +0.136]. Purity 0.81 (0.78).
- FTS: 0.149 against 0.089 (0.099). Δ = **+0.060 ± 0.003** (+0.050) [+0.060]. Purity 0.80.
- FMD: 0.383 against 0.444 (0.432). Δ = **−0.061 ± 0.004** (−0.049) [−0.063]. Purity 0.54.
- COMB: Δ −0.025 ± 0.003 (−0.005) [−0.027].
- (i) The veto data−MC difference is **not** at the few-% level. Plug +0.137 and FTS +0.060 (absolute) are larger than the
  deep-gap view's +0.065 / +0.046 because the subset keeps the η_max 2.5–3.3 events.
- (ii) ND leakage is **not** < 10 %. The DIFF8 purity of the contained LRG sample is 0.83 (7071) / 0.79 (7091), against 0.83 / 0.79 in the
  full sample. Per η_max bin the 7071 ND share is ≤ 0.06 below 1.8, 0.08–0.14 over 2.0–2.8, and 0.21 / 0.34 (7071) in the two top bins.
- (iii) The LAr-edge region is **not** negligible: 37 % of the contained data lie at 3.0 ≤ η_max < 3.3.
- Data/MC η_max ratio, contained, 7071, 0.25 bins (fig_c1; per-bin values and MC-stat in the JSON `hists`): 0.60–0.82 at η_max < −0.2,
  0.71–0.89 over 1.05–2.8, and 1.08 / 1.17 in the two top bins. The relative MC-stat error is ±9–11 % below 0.3 and ±2–4 % above 2.0.
  The full sample is within 0.09 of these values in every bin; 7091 has the same shape and a top bin of 1.09.

## Statistics for the measurement (both parities; charged hadrons = release track cuts, |η| < 1.6, x_F < 0)
| M_X bin (GeV) | data events C / all | data current hadrons C / all | DIFF8 raw events C / all | 9012 raw C (hadrons) | DIFF8 pred. (7071 scale) C |
|---|---|---|---|---|---|
| [4, 10) | 3189 / 3412 | 10145 / 10256 | 3217 / 3402 | 431 (1248) | 3287 |
| [10, 20) | 10518 / 10918 | 44502 / 45165 | 9649 / 10017 | 1066 (4153) | 9118 |
| [20, 40] | 7958 / 8548 | 41125 / 43138 | 8887 / 9529 | 615 (3111) | 5735 |
- With y_eΣ < 0.7 and 35 < E−p_z < 70 GeV ("meas"): contained data 3134 / 10334 / 7384 events. All per-parity rows are in the JSON `stats`.
- The raw DIFF8 counts are comparable to data. The elastic 9012 sample alone holds 0.08–0.14 of the data counts, so the RAPGAP-elastic
  response is MC-stat-limited in every M_X bin; the subset changes this by < 7 %.

## Answer
- In the high-Q² LRG sample the current hemisphere of the γ*–IP system is essentially always inside the CJC on the forward side.
  Its forward boundary sits at median η ≈ 0.5, below the 20° edge in 99.9 % of events.
- 94.7 % of data events (95.4 % MC) pass the full track-based test. The 5 % that fail lose current-hemisphere tracks **backward**
  (θ > 160°), not forward.
- The contained events therefore fall everywhere the LRG sample falls: all M_X and x_IP, and η_max up to the 3.3 cut, with 37 % at 3.0–3.3.
- Requiring containment does not select small-η_max events, and it does not remove the forward-veto mismodelling (Plug +0.14, FTS +0.06),
  the ND leakage (17–21 %) or the LAr-edge population.
- A small-η_max sample has to be cut on η_max itself. Over η_max < 2.5 the ND share falls to ≤ 0.12, and the deep-gap veto numbers of
  `veto_efficiency_view.md` apply (Plug Δ +0.065). Containment is then automatic (93 %).
- Not covered: neutral energy, the nd4cat chain, and frame A.
