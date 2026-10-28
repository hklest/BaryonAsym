# Pre-production audit: what else belongs in the final detector-leg production (2026-10-21)

Requested by the user before launching the N2 campaign ("make sure this is the final production run").
Companion: `docs/n2_hfs_empz_2026-10-21.md`. No canonical file touched; study products only under NAF
`output_v6/studies/hfs_empz_n2_2026-10-21/` (scripts `scripts/unfolding/hfs_empz_n2_2026-10-21/`).

## A. Central-value finding: the nominal data vector misses 0.278% of the luminosity (FU1, never done before)

**UPDATE 2026-10-22 (supersedes the 0.38% estimate below):** a HAT run-set audit shows all 660 HAT chunks were
processed; only 13 GRL runs (110.9 nb^-1) are absent from HAT. The 537-run "no EVLS" set = those 13 + 524
Poisson-empty processed runs (354.2 nb^-1, observed vs expected zero-run counts agree; see
`lumi_runset_audit/poisson_zero_runs.json`), which COUNT. True deficit of the 658-file nominal: 0.246% (2 files) +
0.031% (non-HAT runs) = **0.278%**. Final: L_eff = 351,000 × 0.9996890 = **350,890.85 nb^-1**, f_ep = **0.5439017**
(`lumi_eff_final.json`). The table below is kept as the original record.
GRL per-run lumis (H1RunList, 4 S67 files, 20,788 runs, 356,613.8 nb^-1) vs runs present in the data trees
(Condor 5816031; `lumi_runset_audit/runset_reconciliation.json`):

| run set | L (nb^-1) | fraction of GRL |
|---|---|---|
| runs in the 658 central data sidistrees (nominal DV) | 355,269.8 | **0.99623** |
| runs in the 660 central EVLS | 356,148.7 | 0.99870 |
| 93 runs only in EVLS (files `ep0607_d06_0145`, `ep0607_d07_0029`; central Stage-2 never produced them) | 879.0 | 0.00246 |
| 537 GRL runs in no EVLS (all 660 HAT chunks were processed; e.g. run 477594 = 32 nb^-1 with 0 events → absent from the HAT production) | 465.1 | 0.00130 |

`L_data = 351,000 nb^-1` is the published full-dataset value (Andreev 2024 "same 351 pb^-1"), applied globally;
it is not derived from the processed run list. → **all nominal cross sections are ≈0.38% low (charge-symmetric,
pure normalization).** Remedies: (a) L_eff = 351,000 × 0.99623 = 349,677 nb^-1 (scale only, relative budget unchanged);
(b) produce the 2 missing central sidistrees (Stage-2 from existing EVLS), rebuild the nominal on 660 files and
use L_eff = 351,000 × 0.99870 (re-release cascade). The choice fixes the file set the detector legs must use.

## B. Response frame: build legs natively with the OPT-A builder
Frame test (Condor 5816032 + merge 5816087): `hfs_jes_down` (frozen Empz, 658 files) built with the unmodified OPT-A
builder vs the current practice (cone-frame leg re-denominated by nom_old/nom_new):
- own OPT-A nominal chain reproduces the official release yields to 1.5e-15;
- median |shift| 0.1812% vs 0.1900% (h+), 0.1974% vs 0.1993% (h−);
- per-bin |native − re-denominated|: median 0.004 pp, p95 0.044 / 0.063 pp, **max 0.51 / 1.07 pp**.
HFS legs: native OPT-A costs nothing extra (same jobs, OPT-A merge script). Electron legs (elec_energy median 1.6%,
p90 >10%) are also cone-frame + re-denomination; a native rebuild needs an OPT-A clone of the shifted builder
(OPT-A patch applies 15/17 hunks; gate: zero shift on central trees must reproduce the OPT-A nominal shards bit-identically).
Electron data vectors are reco-only and stay valid.

## C. HFS phi (±20 mrad, frozen menu #7, register B14 "never launched") — documented zero with evidence
One-file Stage-2 `--sys-key hfs_phi_up` on central EVLS d06_0000: Empz, Q2es, tracks, electron bit-identical;
Yes/Xes/Yh change ≤ 2.5e-7 (float rounding); only Q2h and double-angle variables move (unused). No production needed.

## D. Background templates (Set R') are not shifted in any detector leg
e_cone + V0 templates = 7.2% / 7.4% of data integrated (lowQ2 0.006%) from a separate June producer over nominal MC.
Expected co-variation ≈ template × selection change ≈ ≤0.01% integrated. Proposal: bound it in the leg-vector step by
scaling the templates with the per-bin reco-yield change of each shifted response (no extra jobs). Exact shifted
templates would need ≈10k extra producer jobs.

## E. Verified clean
- Shifted trees differ from central only in calculator kinematics (Yes, Xes, Yh, Xh, Q2h, Yda/Xda/Q2da); 92 other reco
  branches and all 60 gen branches identical (jes_down, jes_up rebuild, rces_down, theta_up; data and MC).
- MC file sets identical to the OPT-A nominal (364 RG / 501 DJ, 73 / 101 shards); all 865 MC + 658 data twins exist for 6 legs.
- HFS opposite legs exist and pass the null gate; Stage-1 event-set differences (≈1e-4 of events, unmatched → no Empz transfer) negligible.

## G. Truth-level consistency: unfolded (radiative-level) truth vs the C_rad definition
User requirement: unfold to the radiative level, then C_rad (= σ_nonrad/σ_rad) takes it to the non-radiative level —
so σ_rad in C_rad must be booked with exactly the truth definition of the response.
- **Kinematics: IDENTICAL.** Same generated hadrons, both codes (OPT-A builder `compute_gen_bin` functions vs the
  applied C_rad worker's functions, imported unmodified): 100% same cell / z / pT bin and same acceptance
  (DJ 31,179/31,179, RG 318,542/318,542 hadrons; z, pT agree to ≤1.3e-5). Cell = (Q2esGen, x_es_gen); fiducial
  rad-safe Q2_es_gen/Y_es_gen + GenEmpz>45; xF<0, z, pT all on the nominal photon q. The two tree builders compute
  Q2_es_gen/Y_es_gen/x_es_gen (E_eff = E_beam − E_γ,ISR), Q2esGen (calculator) and GenEmpz identically from the same HAT bank.
- **Mismatch 1 — η/J/ψ-descendant veto in C_rad only.** C_rad drops hadrons with `VetoAncestorPDG_v3 ∈ {221, 443}`
  (3.35% of hadrons, rad and nonrad alike); the response truth keeps them, as does the analysis hadron definition
  ("keep short-lived resonance daughters", sidis-hq2-mc skill). Effect (C_rad's own samples, 96-file subsample,
  same-event double ratio): −0.011% integrated; per-bin median 0.07–0.08% (largely subsample MC noise).
- **Mismatch 2 — RAD-only `Q2Gki > 100` domain cut.** Introduced because the gen-only RAD sample is Q2GT60 and NONRAD Q2GT100.
  But the response MC is also RAD Q2GT60 (`NC.EPLUSP.0607.RAD.Q2GT60`), and nature/data contain these events, so the
  unfolded truth includes radiative events with hard-vertex 60<Q²<100 that C_rad's σ_rad excludes (~0.09% of
  in-fiducial RAD; documented integrated effect +0.066%/+0.068% on C_rad). NONRAD at Q2>100 is complete for the
  Q²≥150 fiducial, so the consistent C_rad drops this cut. (Reverses the 2026-09-27 numerator-only decision.)
- **Mismatch 3 — beam mixture.** C_rad mixes e+p/e−p with f_ep = 0.5141 (superseded 297.7 pb^-1 accounting); the
  processed data fraction (after recovering the 2 files) is 0.5439. Effect −0.001% integrated, per-bin median 0.017%,
  max 0.27%/0.61%. Exact fix = re-run the finalizer only.
- V0-daughter veto: trivially identical (K0S/Λ are stable at generator level; no V0 daughters in either truth tree).
- Net: all three are small (≲0.1% integrated) but definitional; the consistent C_rad needs one worker rerun
  (DJ applied + RG alternative, ≈4,700 gen-only jobs) with mismatches 1–2 removed, plus finalize with f_ep = 0.5439.

## F. Open items that this production cannot address (unchanged)
Trigger efficiency (UNEVALUATED placeholder, not in the quoted total); DIFFVM rho / photoproduction (unevaluated);
S7 prior-reweight bracket (report-only, gate FAIL, medians exceed the totals); M2 z-shift (documented zero; scheme-pair open).
