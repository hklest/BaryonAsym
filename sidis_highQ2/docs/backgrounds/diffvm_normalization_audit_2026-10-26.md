# DIFFVM ρ⁰ normalisation audit for the high-Q² eΣ release (2026-10-26)

**Scope.** This audit covers the scale factor 0.135913 = L_data/L_MC = 350,891 / 2,581,730 nb⁻¹ that the high-Q² charged SIDIS eΣ release (`esigmaphotonxf_2026-10-24`) applies to the DIFFVM elastic ρ⁰ MC. That MC is reqIDs 8445–8448, genIDs 5679–5682. The audit checks four things:
- what the samples are;
- whether the branching fraction, cross section, event count, luminosity, sample combination and phase space are handled correctly;
- an independent recomputation of the expected yield;
- a comparison with the published H1 and ZEUS ρ σ(Q²).

It made no changes to code, the release, or the budget. Its only executions were single-file login-node checks and scratch scripts in `/tmp`.

Path shorthand:
- `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`
- `IMC` = `/afs/desy.de/group/h1/iww2/imc`
- `DV` = `/afs/desy.de/group/h1/h1gen/diffvm/i386_suse63/ftn`

## Short answer

- **The normalisation is correct. The corrected factor is 0.135913, unchanged.**
  - No branching fraction is missing or applied twice.
  - L_MC agrees with the generator logs to 7×10⁻⁶.
  - All 4 × 9,999,998 events were processed.
  - The single global scale on the summed counts is the right combination, and nothing is double counted.
- **Branching fraction.** DIFFVM's cross section and luminosity are σ(ep → eρ⁰p) for all ρ⁰ decays, with BR = 1. DIFFVM decays the ρ⁰ itself, with all channels above BR 0.01 kept. About 99% of events carry a π⁺π⁻ pair. The DB lists about 0.9% neutral modes (π⁰γ, ηγ), which the all-decay MC already includes. The correct BR factor is therefore 1.
  - The "only one decay mode" concern would matter for φ, where BR(K⁺K⁻) = 0.49. For ρ⁰ it is a ≤1% question.
- **Independent check.** Integrating the DIFFVM model formula reproduces the log's σ_ep:
  - 15.98 nb, against 16.0 nb in the log (without cuts);
  - it predicts that a fraction 8.06×10⁻⁵ of events have Q² > 150 GeV², i.e. 3,224 of 40M;
  - the sample has 3,215 such events.
  - The MC is therefore internally consistent. It predicts about 1.29 pb, or about 450 generated events in L_data, at Q² > 150 GeV² before cuts.
- **Why the data show only s = 0.08 ± 0.15.** The cause is physics, not bookkeeping. DIFFVM's fixed n = 2.4 and its saturating R = σ_L/σ_T (which reaches 12 at Q² = 150) overshoot the measured ρ σ(Q²):
  - the overshoot is already 3–5× at Q² ≈ 40 and about 16× at Q² = 100 (ZEUS);
  - extrapolated to Q² = 150–300, the expected overshoot is about 5–8× for n = 2.37, 14–23× for n = 3, and 22–51× for n = 3.5; the ZEUS local slopes above Q² ≈ 40 (3.46, 3.89) favour the steeper cases;
  - so the expected s is about 0.02–0.2, which is consistent with the fit.
- **Effect on bin 30 and on s.** Neither changes, because the factor does not change. The release ±100% term, 34.6% in h⁻ gen bin 30, stays as signed off. The fit result s = 0.08 needs no rescaling.

## 1. What genIDs 5679–5682 (reqIDs 8445–8448) are

Verified from the IMC database `IMC/h1db.js:1374-1377` and `:2994-2997`, the steering headers `IMC/steering_files/mcrequest_844[5-8]`, and the generator logs `IMC/logfiles/diffvm10/gen_56{79,80,81,82}.txt`. Cited line numbers are the same in all four logs.

| reqID | genID | simulated period (RUN_PERIOD) | beam | Tot x-sect (pb) | e-p σ after cuts | simulated L (pb⁻¹) | DST events |
|---|---|---|---|---|---|---|---|
| 8445 | 5679 | 04_05_e- | e⁻ | 15491.76 ± 5.09 | 15.5 nb | 645.504 | 9,999,998 |
| 8446 | 5680 | 2006_e- | e⁻ | 15487.13 ± 5.09 | 15.5 nb | 645.698 | 9,999,998 |
| 8447 | 5682 | 06_07e+ | e⁺ | 15495.69 ± 5.09 | 15.5 nb | 645.341 | 9,999,998 |
| 8448 | 5681 | 03_04_e+ | e⁺ | 15498.91 ± 5.10 | 15.5 nb | 645.206 | 9,999,998 |

- **Process.** The process is elastic ρ⁰, with the meson code at `gen_5679.txt:76` (`'IPAR' 3 0 113`). Both vertices are elastic:
  - the proton vertex at `:128` (`'IPAR' 6 0 0`);
  - the VM vertex at `:146` (`'IPAR' 7 0 0`);
  - the summary at `:757` reads "Diffractive elastic rho0 production".
  - The header comment at `:8-13` ("gamma-proton-dissociative … Q2 = 0-100") is stale template text, contradicted by the active cards.
- **Beams.** 27.6 × 920 GeV (`:61`, `:64`).
- **Pairing.** genID 5681 belongs to reqID 8448 and 5682 to 8447, per `SetgenDB`. 8448 models 2003-04 e⁺, not 2006-07.
- **Phase space.** 20 < W < 350 GeV (`:179`, `:183`, `:760`) and 1 < Q² < 10⁵ GeV² (`:187`, `:191`, `:761`). The y range is 0–1 (`:762`), with E_e′ > 4 GeV and θ_e < 179° (`:120-122`, `:775-776`). The cut efficiency is 96.97% (`:777`). There are no cuts on the decay pions (`:94`).
- **Decay and BR.**
  - `'IPAR' 4 0 0` at `:85` means "all decay channels with BR > 0.01".
  - `'RPAR' 33 0 1.0` at `:276` is the BR card.
  - The DB `MAIN_CUTS` (`IMC/h1db.js:2994-2997`) reads "elastic rho in DIS, all decay channels, including neutral decays pi0+gamma (0.6%) and eta+gamma (0.3%)".
- **Cross section.** The cross section is σ, not σ×BR (see §2). "Tot x-sect" is in pb, inferred from 9,999,999 / 15491.757 = 645.50 pb⁻¹, which matches the printed luminosity.
- **Events.** 10,000,001 generated, 9,999,999 accepted (`:741`, `:779`), and 9,999,998 at DST level (DB `EVENTS`).
- **DB problem flag.** About 1.7×10⁻⁴ of events have an undecayed ρ (log `:851`, `DECRHO: mass of rho too small`). This is negligible.
- **Sum of L_MC.** 2,581,749 nb⁻¹ from the logs, against 2,581,730 nb⁻¹ used. The value used is the sum of the rounded DB `LUMI` fields 645.5, 645.69, 645.34 and 645.2. The relative difference is 7.4×10⁻⁶.
- **Other samples, not used.**
  - diffvm11000 elastic ρ (8407/8412/8417/8422) covers the same four periods at about 645 pb⁻¹ each. Adding it would double count unless its luminosity were added too.
  - The proton-dissociative DIS ρ samples (9323–9326, gen 6305/6306, 2 < Q² < 150) are not in OO format.
  - There are also ω and φ samples, but none of these enters the release.

## 2. DIFFVM decay and cross-section semantics

Verified from the DIFFVM 1.07/1.08 Fortran source. The 11001 source is not on disk; its log cards and outputs behave identically.

- **BR enters only through the BR card**, at `DV/gdfend.f:360` `SIGEP = BR*SIGGP*F/SIGWT` and `DV/gdiend.f:203` `SIGMT = BR*SIGGP*NACC/NGEN*FLUX/SIGWT`. The luminosity is `NACC/SIGMT` (`gdfend.f:696`, `:1004`). With BR = 1.0 the luminosity is σ(ep → eρp) with nothing folded in.
- **ρ⁰ decay.** DIFFVM does the decay itself with SCHC angular distributions, not through JETSET.
  - `DV/decrho.f:26` gives "rho0 -> pi+ pi- (99.0%), pi+ pi- gamma (1.0%)".
  - `decrho.f:142-146` selects the mode.
  - Counting tracks from the all-decay MC times L_data/L_MC therefore includes the decay split automatically.
- **Ways a BR error could arise, and their status here.**
  - Omitting the BR is impossible, because σ is the production cross section and the decay is in the events.
  - Applying it twice would need an explicit factor. None exists (§3).
- **Known source-versus-binary difference.** The 1.07/1.08 decay table lists only π⁺π⁻ and π⁺π⁻γ. The 11001 DB entry adds π⁰γ and ηγ (0.9%). This is ≤1% and is absorbed by the all-decay MC.
- **Model formula.** From `DV/gengam.f:639-642` and `:654`:
  - σ_T = 15 µb · (W/90 GeV)^0.88 · (1 + Q²/Λ²)^−2.4, with Λ² = 0.5929 GeV²;
  - σ_L = R σ_T, with R = ξQ²/(Λ² + ξχQ²), ξ = 0.30, χ = 0.07, saturating at 1/χ = 14.3;
  - the log's printout "[Q²/(Q²+Λ²)]^n" is a mislabelled print statement; the code uses (Λ²/(Q²+Λ²))^n.

| Q² (GeV²) | R (DIFFVM) | σ_T + σ_L at W = 90 (nb) |
|---|---|---|
| 40 | 8.4 | 5.53 |
| 100 | 11.1 | 0.812 |
| 150 | 12.0 | 0.331 |
| 300 | 13.1 | 0.068 |

## 3. How 0.135913 is computed in the release code

- **Constant.** `H/scripts/unfolding/esigma_final_2026-10-24/steps/diffvm_agg.py:17` hard-codes `L_MC_NB = 2581730.0`, and `:52` asserts `scale == L/2581730.0`. The same constant appears at `.../budget/components_groupC.py:85` (assert `:352`) and `.../esph_downstream.py:41`. Its provenance is `sidis_highQ2/data/normalization/diffvm_rho_v6_normalization_2026-10-07/NORMALIZATION.md` §3.
- **L_data.** L_data = `L_eff_nb` = 350,890.85 nb⁻¹, from `H/output_v6/studies/hfs_empz_n2_2026-10-21/lumi_runset_audit/lumi_eff_final.json`. It covers 2003-04 e⁺, 2004-05 e⁻, 2006 e⁻ and 2006-07 e⁺.
  - These are the same four periods the four MC requests simulate.
  - `components_groupC.py:82` and `esph_downstream.py:50` read the same JSON.
- **Processed events.** 404 files were processed, 101 per request, with 4 × 9,999,998 = 39,999,992 input events. The campaign record is `H/output_v6/studies/diffvm_rho_v6_2026-10-07/CAMPAIGN_STATUS.md:36-42`, with all stages 404/404 rc = 0. The aggregation log (`.../esigma_final_2026-10-24/logs/post.5887957.out:16`) reads `files=404 counts=2196 scale=0.135913071 sum_T=298.465`. The processed fraction is 1.000, so L_MC needs no reduction.
- **Combination.** One global scale is applied to the counts summed over the four requests.
  - That is equivalent to weighting each request by σ/N, because all four have the same σ to within 0.08%.
  - The per-request scale of about 0.544 noted in `README_PROVENANCE.md` is not summed (that would overcount by 4×).
  - The diffvm11000 requests appear in none of the 808 steering files (NORMALIZATION.md §1).
- **Weights and BR.** There is no per-event weight: `diffvm_agg.py:46-47` asserts integer counts, and weight1 = weight2 = 1 (NORMALIZATION.md §2). No BR factor appears in `diffvm_agg.py` or the DIFFVM part of `components_groupC.py`. This is correct per §2.
- **Phase space.** The DIFFVM trees go through the release selection: 150 < Q²_es < 20000, 0.1 < y_es < 0.7, E−p_z > 45, |z_vtx| < 35, and the track cuts. The per-file JSON `fiducial_contract` field confirms this. Nothing divides by a generated-range fraction; T = scale × counts. Using the full-sample σ and N with reco-level cuts is the correct and consistent treatment.

## 4. Independent recomputation of the yield

- **Model integral.** The DIFFVM formula of §2, integrated with WWA transverse and longitudinal fluxes over the generated range, gives the values below. The h1-mc-expert integration (`/tmp/dvm_calc.py`) gives σ_ep = 15.98 nb, against 16.0 nb in the log (without cuts).
- **Q² fraction.** The fraction with Q² > 150 is 8.06×10⁻⁵, which predicts 3,224 events in 39,999,992. The sample has Q²_gen > 150 in 3,215 events (`H/output_v6/studies/diffvm_rho_v6_2026-10-07/CAMPAIGN_STATUS.md:36-42`, counted on the kept EvlTree events; the agreement with 3,224 indicates the preselection keeps essentially all of them). In one file (`/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/diffvm10/8445/hat....DST.0000.root`, 90,000 events), the measured fractions above Q²_gen = 10, 20, 40 and 100 agree with the model to within a few percent.
- **Expected generated events in data.**
  - 3,215 × 0.135913 = 437 events with Q²_gen > 150;
  - equivalently, σ(Q² > 150) ≈ 1.25–1.29 pb × 350.9 pb⁻¹ ≈ 440–450 events;
  - after the reco selection, the release has T = 298.5 tracks in total.
- **Verdict.** 0.135913 is the correct normalisation of the DIFFVM model to the data luminosity. A recount of tracks inside the 404 ROOT files was not done, but the logged input counts and the matching Q² fraction make a processing loss unlikely.

## 5. Cross-check against published ρ σ(Q²)

Values below were read from the paper tables (PDFs extracted to `/tmp/rho/`).
- **ZEUS, arXiv:0708.1478, Table 4** (W = 90 GeV), compared with the DIFFVM model at the same Q² and W:

| Q² (GeV²) | ZEUS σ (nb) | DIFFVM (nb) | DIFFVM / data |
|---|---|---|---|
| 24 | 6.03 | 14.9 | 2.5 |
| 37 | 1.88 | 6.46 | 3.4 |
| 60 | 0.36 | 2.42 | 6.7 |
| 100 | 0.05 ± 0.03 | 0.81 | ≈16 (±60%) |

- **H1, arXiv:0910.5831**:
  - Table 14 (W = 75) agrees with ZEUS: DIFFVM/data is 2.2 at Q² = 18.8 and 4.8 at Q² = 46.
  - Table 8 gives n = 2.37 ± 0.06 for a constant-n fit over Q² = 1–60.
  - Table 9 gives n_T = 2.86 and n_L = 2.17.
  - ZEUS §10.2 reports that a constant-n fit fails and that n increases with Q². The local slopes from Table 4 are 2.75, 3.46 and 3.89.
- **R.** Measured R is 4.6 at Q² = 19.5 and 6.1 (+2.8/−1.6) at Q² = 40 (ZEUS Table 7). DIFFVM gives 5.7–6.6 and 8.1 at those points, and R saturates at 14.3. No R is measured above Q² ≈ 60.
- **Extrapolation to Q² = 150–300.** No measurement exists above 160 GeV², so these are extrapolations.
  - DIFFVM/data, anchored on ZEUS at Q² = 60: 7.9–8.4 for n = 2.37, 14–23 for n = 3.0, and 22–51 for n = 3.5.
  - Anchoring on H1 at Q² = 46 or ZEUS at Q² = 37 changes the low end to about 5–6.
  - The low-Q² normalisation is right: DIFFVM gives about 650 nb at Q² = 2.4, against 647 nb from ZEUS. The excess therefore comes from the Q² shape (n and R), not from σ_γp.

## Facts and inferences

**Verified facts:**
- the generator cards, cross sections, luminosities and event counts;
- BR = 1 and all-channel decay;
- the formula and decay-mode source lines;
- the release constants and asserts;
- the 404/404 processed files with 39,999,992 inputs;
- that no BR or event weight is applied;
- that the integral matches the log σ_ep and the generated Q² > 150 fraction;
- the published ZEUS and H1 table values.

**Inferences:**
- that the 11001 binary uses the same cross-section and BR code as the 1.07/1.08 source (its decay table differs by the ≤1% neutral modes, §2);
- that the expected DIFFVM/data at Q² > 150 is about 5–51 (n = 2.37 to 3.5), which depends on extrapolating n and R beyond measured Q².

## Risks, open questions and coverage gaps

- **Period mix.** The MC has equal luminosity per period, while the data weights are about 15%/30%/16%/40%. This affects only period-dependent acceptance, not σ. It is not checked, and it is expected to be small.
- **Proton-dissociative ρ.** It is not modelled, because 8445–8448 are elastic only. Including it would raise the prediction, which would make the data deficit larger, not smaller.
- **Fake-electron events.** Reco-level DIFFVM tracks from low-Q² events with a fake electron are not separated from true high-Q² events (`sidis_highQ2/docs/research/research-diffvm-exclusive-rho-highq2-v6_2026-10-07.md:89`).
- **Not done:**
  - no per-track recount inside the 404 trees;
  - no empirical check of decay daughters in the truth tree (`ChargedTotGen`);
  - H1 hep-ex/9902019 was read only at abstract level, and HEPData was not consulted.
