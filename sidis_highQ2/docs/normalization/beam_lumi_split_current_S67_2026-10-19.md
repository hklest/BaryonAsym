# Per-Beam (e+ vs e−) Luminosity Split of the Four-Period S67 High-Q2 Sample under L_data = 351,000.0 nb^-1

**Date:** 2026-10-19
**Author role:** `h1-normalization` (luminosity and weight authority; read-only delegated task)
**Scope:** `sidis_highQ2/` (v6/z005, four-period S67 high-Q2 charged-SIDIS) only. Not exportable to any other analysis in this workspace.
**Products touched:** NONE. Read-only session; the only file written is this record.
**Basis under test:** the CURRENT normalization accounting `L_data = 351000.0 nb^-1 = 351 pb^-1` (four-period S67 target, includes ep0304; `STATE.md` line 12; `docs/status/normalization.md`).

---

## 0. Verdict at a glance

| Question | Answer |
|---|---|
| Recorded per-period split of the 351,000.0 target? | **No — none exists anywhere in the workspace** (`docs/normalization/beam_energy_period_composition_2026-10-15.md` §1.2, §9 item 1, re-confirmed by sweep this session). The chain needs none: `L_data` is applied once, globally, post-unfold; no per-period or per-beam weighting anywhere (`docs/status/normalization.md` "Do not scale each period..."). |
| Authoritative per-period *delivered* lumis | oolumi S67 GRL values (§1 below) — recorded fact, sum 356,613.59 nb^-1. |
| Defensible per-period/per-beam split of the 351,000.0 target | Proportional rescale of GRL shares by `351000.0/356613.59 = 0.9842586201` — **derived convention, not a recorded fact** (the standing convention named in `beam_energy_period_composition_2026-10-15.md` §1.2/§9; exact closure would need oolumi-level confirmation of the target's construction). |
| **L(e+p) @ 351,000.0** | **190,958.3023 nb^-1** (ep0304 + ep0607) |
| **L(e−p) @ 351,000.0** | **160,041.6977 nb^-1** (em0405 + em06) |
| **f_ep** | **0.5440407472973759** (= 194,012.324/356,613.59; total-invariant — identical under the 351,000.0 basis) |
| **f_em** | **0.4559592527026241** |
| Which split is authoritative | The **GRL inventory beam fractions** f_ep = 0.5440407473 / f_em = 0.4559592527 (recorded fact, total-invariant). The per-beam *luminosities* under the 351,000.0 basis follow from the documented proportional convention (§2). The superseded 297,657.8-nb^-1 split (f_ep = 0.5140930272) is **NOT** the current-basis split. |
| Realised v6 data-vector beam composition recorded? | **No** for the central v6 vector (no beam-charge branch; only hadron-charge in-grid counts). Closest v6-era realised record: the archived v6bsa controls' in-grid track split e+ 2,065,796 / e− 1,734,841 (ratio 1.19077), which is BSA-selection-subset, not a central normalization input (§5). |

---

## 1. Per-period delivered luminosities (recorded fact: oolumi S67 GRL inventory)

Source of truth: `H1ChargedSidis_HighQ2/LumiFiles/nc_goodruns_*.root` (oolumi outputs; bit-identical SIDIS-local copies of `H1BaryonAsym_HighQ2/LumiFiles/`, md5-verified 2026-05-23). Recorded in `docs/references/reference-highQ2-grl-manifest.md` (2026-05-19, frozen) §1 and `docs/normalization/beam_energy_period_composition_2026-10-15.md` §1.1. All four GRLs: S67 (LAr NC DIS), prescale limit 1, |z_vtx| < 35 cm, Compton correction on, min run lumi 0.2 nb^-1. Charge purity per period 100% (ep0304/ep0607 e+; em0405/em06 e−), confirmed from `data/bsa/validation_v6bsa_2026-09-16.json`; run sets disjoint (union 20,788 = sum).

| Period | Beam | Runs | L_GRL (nb^-1) | GRL share | L @ 351,000.0 basis (nb^-1, proportional convention) |
|---|---|---:|---:|---:|---:|
| ep0304 | e+ | 4,017 | 53,706.105 | 0.1506155 | 52,860.6968 |
| em0405 | e− | 5,194 | 105,028.172 | 0.2945437 | 103,374.8836 |
| em06 | e− | 3,486 | 57,573.094 | 0.1614398 | 56,666.8141 |
| ep0607 | e+ | 8,091 | 140,306.219 | 0.3934010 | 138,097.6055 |
| **Total** | | **20,788** | **356,613.590** | 1 | **351,000.0000** |

Proportional factor: `351000.0 / 356613.59 = 0.9842586200935304`. (Distinct from the bookkeeping factor `0.9842602485 = 351000.0/356613.0` quoted in `STATE.md` line 26, which uses the superseded direct-monitor total 356,613.0; and from the `0.984260` rounding in `beam_energy_period_composition_2026-10-15.md` §1.2. The exact-GRL-sum factor above is the correct one for the per-period split.)

## 2. Beam split under the 351,000.0 basis (derived per the documented convention)

- **L(e+p) = L(ep0304) + L(ep0607) = 52,860.6968 + 138,097.6055 = 190,958.3023 nb^-1**
- **L(e−p) = L(em0405) + L(em06) = 103,374.8836 + 56,666.8141 = 160,041.6977 nb^-1**
- Sum check: 190,958.3023 + 160,041.6977 = 351,000.0000 (exact by construction).
- **f_ep = 190,958.3023/351,000.0 = 0.5440407472973759**
- **f_em = 160,041.6977/351,000.0 = 0.4559592527026241**
- e+/e− ratio = 1.1931784344 (= GRL ratio 194,012.324/162,601.266; the proportional factor cancels, so the beam FRACTIONS are recorded-fact GRL shares, independent of the target total).

## 3. Cross-check against the historical fractions

| Accounting | Total (nb^-1) | L(e+) | L(e−) | f_ep | Status for the current 351,000.0 basis |
|---|---:|---:|---:|---:|---|
| **Current S67 four-period target** | **351,000.0** | **190,958.3023** (convention) | **160,041.6977** (convention) | **0.5440407473** (GRL share, fact) | **AUTHORITATIVE** (`STATE.md` line 12; `docs/status/normalization.md`; `docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md` §5) |
| Superseded 2005–2007-only | 297,657.8 | 153,023.8 (= 106,823.1 + 46,200.7) | 144,634.0 | 0.5140930272 (recorded/hardcoded; exact quotient of the rounded inputs 0.5140930290) | **REJECTED for the current basis** — excludes ep0304; still hardcoded as the applied C_rad mixed-beam weight (`BEAM={"ep":.5140930272,"em":.4859069728}`), induced C_rad bias bracketed ≤ 0.02% (`docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §ii/§iii, gap item 8; provenance `docs/radiative/norad_supplement_reconciliation_2026-09-11.md`) |
| Direct-monitor quote | 356,613.0 | (≈194,012) | (≈162,601) | 0.5440407 (recorded 0.5440407387464388 = 194,012/356,613 to ~1e-9; `docs/radiative/SESSION_HANDOFF_crad_2026-09-25.md` line 47; `docs/radiative/comparisons/current_sidis_crad_variants_2026-09-27.md` line 17) | **Total superseded** as a normalization default; its f_ep is numerically the SAME beam fraction as the current GRL share (differs by 8.6e-9) because both derive from the same GRL inventory — the fraction is total-invariant |

- Δ between current and superseded-297k fractions: f_ep shifts by +0.0299477 (0.5140930 → 0.5440407); the 297,657.8 split must never be quoted as the beam composition of the current four-period sample.
- Guard list unchanged (`docs/status/normalization.md`; `lumi_2p7_derivation_memo_2026-10-10.md` §5): `297,657.8` superseded; `356,613.0`/`356,000` superseded direct-monitor/stale; `126,880` FORBIDDEN (~42x wrong).
- Luminosity uncertainty: flat 2.7%, fully correlated (`lumi_rel = 0.027`, Andreev 2024 §6.3 anchor) — applies to the global 351,000.0 normalization; no per-beam uncertainty component exists or is needed.

**Authoritative statement:** for the current `L_data = 351000.0 nb^-1` accounting, the beam split is **f_ep = 0.5440407472973759 / f_em = 0.4559592527026241**, with per-beam luminosities **L(e+) = 190,958.3023 nb^-1 / L(e−) = 160,041.6977 nb^-1** under the documented proportional convention. The fractions are recorded facts (GRL inventory shares); the per-beam/target-decomposed luminosities are a derived convention pending oolumi-level confirmation of the target's construction (`beam_energy_period_composition_2026-10-15.md` §9 item 1 — the standing gap, materiality already bracketed ≤ 0.02% where it enters C_rad).

## 4. Chain behavior (why no per-beam number is applied)

All 658 central v6 data files (83/152/100/323 per period) are globbed into ONE merged 980-bin unweighted data vector; `L_data = 351,000.0` enters once, post-unfold, as a single global constant; MC vectors get one generator-wide scale (`docs/status/normalization.md`; `beam_energy_period_composition_2026-10-15.md` §3.1). No per-period lumi weighting, no per-period target split, no beam-charge weighting is applied anywhere in the central chain.

## 5. Realised data-vector beam composition for central v6 (task item 4)

- **No record gives the beam-charge composition of the central v6 data vector itself.** Central v6 trees carry no beam-charge branch (that was the isolated `v6bsa` fork addition); the central cone data vector records only HADRON-charge in-grid counts (3,865,073 = h+ 1,965,700 / h− 1,899,373; `STATE.md` line 10), which are not beam charges.
- **Superseded v5f-era realised record (do not use):** `quarantine/docs/results/lumi_effective_per_period_v5f.md` (+ companion JSON, now only in quarantine) — realised effective per-period lumis {ep0304 37,981.522; em0405 69,678.028; em06 22,798.097; ep0607 58,470.084} nb^-1, e+ 96,451.606 / e− 92,476.124, ratio 1.04299, f_ep ≈ 0.510521. These are DST-coverage-fraction products (35–66% GRL coverage) of the superseded v5f era; flagged "must not be used" in `beam_energy_period_composition_2026-10-15.md` §1.2 and carried there and in `CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §iii only as the realised-composition bracket.
- **Closest v6-era realised record (not a central input):** the archived v6bsa control JSONs — `data/bsa/araw_eplus_2026-09-16.json` (`lepton_charge: 1`, in-grid tracks 2,065,796) and `data/bsa/araw_eminus_2026-09-16.json` (`lepton_charge: -1`, in-grid tracks 1,734,841) — are built on trees with file-by-file entry parity with central v6 (658/658, 7,538,865 `ChargedTot` entries; `docs/bsa/RESULTS_bsa_araw_2026-09-16.md` §2). Beam-charge ratio 1.19077, subset f_ep = 0.543539. Caveats: this is the BSA polarized-selection subset (beamPolQuality/sign cuts; 83,882 polbad tracks dropped; subset sum 3,800,637 vs central in-grid 3,865,073), the BSA side study was declared NO LONGER PURSUED 2026-10-16 (`STATE.md` line 78), and it is not a normalization input. Its consistency with the GRL ratio (1.1908 vs 1.1932) supports that the v6 production realises approximately the delivered beam mix — unlike the v5f era — but no central-vector beam split is recorded or required.

## 6. Verification provenance (this session, read-only)

- Recomputed from the recorded GRL per-period values (Decimal arithmetic, 50 digits): total 356,613.590 exact; e+/e− subtotals 194,012.324 / 162,601.266 exact; proportional factor 0.9842586200935304; per-period and per-beam target values as tabulated; sum check exact; f_ep/f_em as quoted.
- Cross-checked the historical fractions: 153,023.8/297,657.8 = 0.5140930290 (recorded hardcoded 0.5140930272 — same to 8 decimals; the tiny last-digit difference is the script's rounding, immaterial at 2e-9); 194,012/356,613 = 0.5440407388 reproduces the recorded direct-monitor 0.5440407387464388 to ~1e-9.
- Sweep for any newer per-period split of 351,000.0 (docs/status/, data/flux/, docs/): none found; consistent with the 2026-10-15 record's "not recorded anywhere" statement.
- Primary sources: `sidis_highQ2/STATE.md` (line 12 lumi; line 26 bookkeeping factor; line 10 cone vector counts); `docs/status/normalization.md`; `docs/normalization/lumi_2p7_derivation_memo_2026-10-10.md` (§1, §5); `docs/normalization/beam_energy_period_composition_2026-10-15.md` (§0, §1.1–§1.3, §3.1, §6, §9); `docs/references/reference-highQ2-grl-manifest.md` (§1, FU3); `docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` (§i.2, §iii); `docs/radiative/norad_supplement_reconciliation_2026-09-11.md` (via the mixed-beam assessment); `docs/radiative/SESSION_HANDOFF_crad_2026-09-25.md` (line 47); `docs/radiative/comparisons/current_sidis_crad_variants_2026-09-27.md` (line 17); `quarantine/docs/results/lumi_effective_per_period_v5f.md`; `data/bsa/validation_v6bsa_2026-09-16.json`; `data/bsa/araw_{eplus,eminus}_2026-09-16.json`; `docs/bsa/RESULTS_bsa_araw_2026-09-16.md`.

## 7. Status and caveats

1. **PASS** — the beam split under the current accounting is derived, verified, and internally consistent; the authoritative fractions are the GRL inventory shares.
2. **Standing gap (carried, not new):** the 351,000.0 target has no oolumi-confirmed per-period construction; the per-beam luminosities here are the documented proportional convention (`beam_energy_period_composition_2026-10-15.md` §9 item 1). Materiality where it currently matters (C_rad mixed-beam weight) is already bracketed ≤ 0.02%.
3. The applied C_rad finalize weight f_ep = 0.5140930272 remains a **superseded-accounting artifact** (provenance-wording item per `CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md`); this record does not change any applied table or product.
