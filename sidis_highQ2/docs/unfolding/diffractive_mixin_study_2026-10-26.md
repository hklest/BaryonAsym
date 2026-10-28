# Diffractive mix-in to the eΣ IBU response (2026-10-26)

**Scope.** This note asks whether adding a data-tuned diffractive RAPGAP component to the response changes the high-Q² (Q² > 150 GeV²) charged-SIDIS eΣ IBU result. The release is `.../output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24/`. The question is whether the change falls inside the existing ibu_model and closure terms. It is a study only: no release, budget, note or plan change. Decided items (eΣ photon, γ*p x_F, no-Z Born, strict observable with no veto, closure treatment R, DIFFVM subtraction s = 0.08 ± 0.15) are not reopened.

Path shorthand:
- `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`.
- `S` = `H/output_v6/studies/diffractive_mixin_2026-10-26` (all outputs).
- `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.

## Short answer

- **The mix changes the result by about as much as the existing model term, and RAPGAP versus DJANGOH does not cover it.**
  - With the nominal mixture (f = 0.133 of selected DIS events diffractive), the median |shift| per bin is 1.9%. The median ibu_model term is 1.7% and the median closure term 1.1–1.3%.
  - Mixing the same diffractive sample into DJANGOH gives the same shift (per-bin correlation 0.98, partly forced by the shared diffractive sample). The generator difference behind ibu_model therefore does not probe diffraction.
- **About half of the per-bin shift variance is diffractive-MC noise; the rest is real.**
  - A per-event bootstrap gives σ_boot ≈ σ_MC (median 2.4% / 2.6%, h⁺/h⁻).
  - The χ² of the shift against zero is 485 / 442 for 245 bins, so the real shift has an rms of about 0.9–1.0 σ_boot (≈ 2.3%).
  - The per-bin counts of bins that exceed ibu_model ⊕ closure are inflated by noise. After noise subtraction, 60 / 58 bins exceed it, against 35 / 40 expected from noise alone.
- **Corner bins move by tens of percent at about 1σ.** Bin 30 moves by +0.17 ± 0.16 (h⁺) and +0.25 ± 0.22 (h⁻). The other high-z/low-pT corners range from −0.54 to +0.50, with σ 0.01–0.41, so the MC statistics cannot resolve them.
- **RAPGAP-π is not processed and does not bear on the gap deficit (inferred).** Processing would take 68 grouped Condor jobs, about 3 h of CPU, plus prerequisites (§1).
- **Recommendation: a new systematic term, not a mixed nominal, and not "nothing".**
  - A mixed nominal is premature. f = 0.134 ± 0.06 has a poor GOF, and the diffractive MC would add noise as large as the effect to every bin.
  - The term should not be the per-bin noise-subtracted shift, because noise dominates that bin by bin. Use a smoothed shift(a), with the f band 0.08–0.18. One option is to average along x at fixed (z, pT), which keeps the corners. The kernel is untested (inferred); see Open question 2.
  - This is a user decision, because it changes the budget.

## 1. Diffractive MC inventory (verified, `S/provenance/diffractive_mc_provenance.md`)

| ReqID | genID | Content | Period | Gen cut | Events | v6dif status | L_MC (nb⁻¹) |
|---|---|---|---|---|---|---|---|
| 9012 | 4489 | elastic uds pomeron, HERACLES radiative, x_IP < 0.2 | ep0607 (projected to all 4 periods) | Q² > 5, no ceiling | 1.2M | processed 24/24 | 52,273 |
| 9155 / 9157 / 9158 | 6191 / 6193 / 6194 | uds pomeron with proton dissociation (DIFFVM-style M_Y) | em0405 / em06 / ep0607 (+ep0304 borrow) | Q² > 1, pT_hat > 3 (saturated at Q² > 150) | 8.0M / 4.0M / 8.0M | processed | 1,004,820 / 501,891 / 1,004,701 |
| 9024 / 9033 | 4843 / 4842 | elastic charm pomeron | e⁺ / e⁻ | Q² > 5 | — | processed | 716,371 / 477,581 |
| 6671 / 6677 | 4860 / 4863 | light-flavour reggeon, SW = 70 | e⁻ / e⁺ | — | — | processed, no ODS forward variables | 368,101 / 368,118 |
| 8945 / 8946 | 5622 / 5623 | RAPGAP31 π⁺ exchange, GRV-LO-π, radiative | 06_07e⁺ only | Q² > 3 | 9.0M / 10.0M | not processed | ~273,450 / ~305,060 (DB xsec field, provisional) |
| 9018/9019/9027/9028 | 4830/4829 | old f2d elastic uds | all | Born Q² ceiling 200 GeV² | 3.0M each | not processed; unusable above Q² ≈ 200 | — |

- **Normalisation already ruled** (`D/docs/mc/composite_scale_table_v3_2026-10-20.csv`):
  - Event weight is w1·w2·L_data·(period share)/L_MC.
  - PDISS gets ×5.3611 only for truth-in-cut events (M_Y < 1.6, |t| < 1, x_IP < 0.03, M_X > 2, Q² ≥ 150, 0.1 ≤ y ≤ 0.7). This is meant to give the ruled 83:17 elastic:dissociative mixture. The truth-in-cut split computed here is 78.4:21.6 (`S/etamax/etamax_fit.json` `check_83_17`). The difference is unresolved: a Q2sGen/YsGen proxy for calcQ2s, or the release-vs-certified exposure convention. The ruling is not reopened. The overall reco-selected PDISS share (16.8%) is a different quantity and is not comparable.
- **No DPDF fit label** (H1 2006 Fit A or B) is recorded in the database for 9012 or the PDISS sets. The generator logs return HTTP 401, so this is unknown.
- **RAPGAP-π statistics.** At Q² > 150 and 0.1 < y < 0.7 there are about 33,500 (8945) and 40,300 (8946) events. This is read from chunk 0000 of each sample and is provisional. The database cross-section field gives about 578 pb⁻¹ for the two together. SIGMT is not yet read, and the 2026-10-15 inventory's "L_prov 32,755" for 8945 is a cross-section in pb, not a luminosity.
- **Cost estimate (inferred, from v6dif Condor timings of 9158).**
  - 8945 with 3 chunks per job: 63 jobs, about 1.5 h CPU, 1–2 h elapsed.
  - 8945 + 8946 with 6 chunks per job: 68 jobs, about 3.2 h CPU.
  - Prerequisites: FPACK symlink farms (896 and 1000 links), the 0607 GRL, binaries pinned in `mck_samples.json`, a chunk-looping wrapper, and a luminosity record from h1-normalization.
  - Running the release response builder on top is one more job per sample.
- **Correction.** `sidis_highQ2/docs/research/research-background-sources-migration.md:135` says "RAPGAP includes diffractive component natively". That is wrong for the samples used here: the signal RAPGAP 7071/7094/7096/7097 has a truth LRG rate of about 1×10⁻⁴ per event (`D/docs/mc/period_matched_pair_record_2026-10-20.md`). That file is not edited here.

## 2. What the samples model (h1-mc-expert; inferred unless marked)

- **Elastic pomeron (9012) and low-M_Y dissociation (9155–9158)** populate η_max < 3.2 at x_IP ≲ 0.03. Higher-M_Y dissociation fills the forward region partly. Reggeon contributes only at x_IP ≳ 0.01, and charm is a few-% shape correction.
- **Double counting.** The ND RAPGAP is generated from proton PDFs fitted to inclusive F2, so its rate already contains σ_diff, but with a non-diffractive final state. The consistent mixture is therefore (1−f)·ND + f·D, not ND + D.
- **Generator-level cut on D.** Apply M_X > 2 GeV: exclusive ρ⁰ is already subtracted from the data via DIFFVM, and JETSET is unreliable at low M_X.
- **Generator-level checks (verified, `S/response/checks/checks.json`):**
  - The gen SIDIS record in the diffractive samples holds only π/K/p rows.
  - The generator electron is found in every event.
  - The >1% gen-Q² definition tail (14–19% of events) is the same in ND 7071, so it is not specific to diffraction.
- **Expected effect.** Diffractive events have no proton remnant and low multiplicity, so their hadrons are harder in z and softer in pT. They therefore sit in the high-z/low-pT corner, which is where the gap-veto excess in data lives (`sidis_highQ2/docs/backgrounds/vm_veto_cut_2026-10-26.md` §2–3).

## 3. η_max and the data-tuned fraction (verified, `S/etamax/etamax_fit.json`)

**Selection and inputs.**
- Release data-vector selection (`_select_and_bin`, patched only to return the row mask), applied to 528,749 data events.
- 1,343 events (0.25%) come from 2 files without DiffTot and are excluded.
- Normalisation uses the release luminosity 350,890.85 nb⁻¹ with release period shares.
- Condor cluster 5891703 (52 jobs) plus the fit jobs (final 5891811).

**Fit variable.** `etaMaxClus08`.
- The ODS variables (`etaMaxVtxExcl08`/`04`) are −999 for every ND RAPGAP, DJANGOH and reggeon event (odsLoaded = 0).
- Where both variables are filled they correlate at 0.997–0.9996, and the Excl08 variant gives the same f (0.134).

**Fraction f** (fraction of selected DIS events that are diffractive):

| fit | f | χ²/ndf |
|---|---|---|
| nominal: tail η_max < 2.5, N pinned to data | 0.134 ± 0.003 (stat) | 48/6 (f = 0: 11739/7) |
| D with gen M_X > 2 | 0.133 | — |
| D with M_X > 2 and x_IP < 0.1 | 0.106 | — |
| tail < 2.0 / < 3.0 | 0.124 / 0.174 | — / 299/8 |
| gap < 3.2, N free | 0.080 | 30.6/8 |
| Clus04 | 0.116 | — |
| elastic 9012 only | 0.156 | — |
| ND = DJANGOH 7091 | 0.145 | 86.7/6 |
| scale-table absolute (no fit) | 0.0975 | tail χ² 199 over 7 bins (ND only: 12309) |

- **Systematic on f.** ±0.047 from the fit variable and range, and ±0.038 from the template choice, giving ±0.06 in quadrature (band about 0.07–0.19). Variants (b)/(c) at 0.08/0.18 approximate this band.
- **Full-range fit.** The full-range shape fit with N free fails (χ²/ndf 2800/11). It is driven by the η_max ≈ 3.2–3.6 LAr-edge mismatch (data/ND 0.65–1.2), not by diffraction, and is not used.
- **No fit has a good GOF.** This is consistent with the parked extension (`D/context-handoff.md`).
- **Slices.** f falls with x and Q²:
  - x slices: 0.164 (x < 0.01), 0.137 (0.01–0.03), 0.005 ± 0.007 (x > 0.03).
  - Q² slices: 0.162 (150–250), 0.136 (250–500), 0.064 (> 500 GeV²).
  - In the x > 0.03 slice the absolute scale table predicts 0.051, so the fit and the table are in tension there.

**Tracks in the corner.**
- At z > 0.7 and pT < 1 GeV, the fitted diffractive track share is 0.29 ± 0.04 (h⁺) and 0.35–0.42 (h⁻).
- For gen bin 30 from the response columns it is 0.25 ± 0.08 (h⁺) and 0.42 ± 0.11 (h⁻).
- In the 7 veto-study reco bins the fit attributes 14 ± 5 of 46 data tracks to diffraction. This matches the 9–15 tracks the gap vetoes remove, against about 1 track predicted by ND only.

**Before and after mixing.**
- Below η_max ≈ 2.5, ND alone underpredicts the data by factors of up to about 40.
- With D mixed in, the tail agrees within about 30% (`S/etamax/plots/etamax_event_level_before_after.png`; corner tracks in `etamax_corner_tracks_before_after.png`).
- The absolute ND + D corner yield overshoots the data by 1.5–1.8×. This is already the case for ND alone (1.4–1.5×), so it is an ND high-z normalisation feature. IBU does not see it, because columns are normalised (inferred).

**Not in the fit.**
- DIFFVM has no DiffTot. At s = 0.08 it is about 13 of 529k events, which is negligible at event level.
- Reggeon (1.1–1.2% of the data each; 31% of its events have η_max < 3.2) and charm are not in the fit.

## 4. Mixed response and re-unfold

**Engine reproduction (verified, `S/unfold/ibu_driver.py`, `repro_check.json`).**
- The minimal driver calls `derive_response_blocks` and `ibu_unfold` from `H/scripts/unfolding/tunfold_official_v4_percharge/crosscheck_unfold_methods_v4.py`.
- Inputs are the release data vector `.../esigma_final_2026-10-24/downstream/vectors/data_vector_bgsub_setrprime_esph660.json`, after the release f_i step, and IBU iteration 4.
- It reproduces the release `ibu/cross_sections.csv` to 1.0×10⁻¹⁵ (h⁺) and 1.7×10⁻¹⁵ (h⁻), relative.

**Diffractive response (verified, `S/response/`).**
- Built with the md5-pinned release builder `cone_s2prime_optA_2026-10-19/build_migration_th2_per_charge_cone_s2prime_optA_esxf.py` (620e4f70…), called unmodified through the wrapper `resp_groups.py`.
- Events are split into truth groups (in-cut, M_X > 2, x_IP < 0.1).
- On release shard `rapgap31_0000` the wrapper reproduces the release shard to 7×10⁻¹⁵. Per-event weighting equals the group-sum weighting to 4×10⁻¹⁵.
- v6dif and v6 ChargedTot/ChargedTotGen are array-identical on 7071 (two files checked). Condor cluster 5891679.

**Mixing (`S/unfold/mix/mix.py`).**
- M_mix = M_ND,release + λ·M_D, with λ = f/(1−f) · N_ND/N_D, where N_ND and N_D are the reco-selected event sums from the η_max tables.
- N_ND = 12,873,498. A tracks-based cross-check agrees to 1.0%.
- Nominal λ = 37.25, and the reco-track ratio D/ND = 0.135.
- The prior is the coherent body+miss marginal of M_mix.
- σ_MC comes from 300 toys that fluctuate only the D response.
- The closure term is the rms of `bias_rel` over truths {djangoh, datalike}, data mode, from `H/output_v6/studies/stat_coverage_2026-10-26/analysis/per_bin_all_groups.csv`. No installed per-bin closure file exists.

**Per-variant results** (verified, `S/unfold/mix/shift_table.csv`, `summary.json`, `rel_shift.png`). Counts are bins (of 245, h⁺/h⁻) where |shift| exceeds each term.

| variant | > ibu_model | > closure | > quad | > total | bin 30 shift (h⁺/h⁻) |
|---|---|---|---|---|---|
| (a) nominal, M_X > 2, f = 0.133 | 133/135 | 159/180 | 100/112 | 41/61 | +0.174 / +0.252 |
| (b) f = 0.08 | 96/105 | 125/140 | 63/69 | 16/13 | +0.114 / +0.163 |
| (c) f = 0.18 | 160/156 | 174/197 | 129/140 | 76/88 | +0.217 / +0.314 |
| (d) M_X > 2, x_IP < 0.1, f = 0.106 | 111/126 | 128/146 | 75/86 | 26/36 | +0.167 / +0.249 |
| (e) elastic only, f = 0.156 | 152/150 | 170/191 | 129/134 | 75/90 | +0.144 / +0.323 |
| (f) scale-table absolute, f = 0.0975 | 108/121 | 141/161 | 75/86 | 21/26 | +0.144 / +0.202 |
| (g) nominal into DJANGOH | 126/136 | 151/177 | 94/115 | 38/62 | +0.165 / +0.237 |
| (h) prior only (mixed prior, ND response) | 193/205 | 201/210 | 185/198 | 163/183 | +0.482 / +0.630 |

**Corner bins, variant (a).** Shift (σ_MC), bins 30/65/100/135/170/205/240:
- h⁺: +0.17 (0.16), +0.12 (0.18), +0.36 (0.39), −0.08 (0.14), +0.30 (0.41), +0.50 (0.32), +0.00 (0.01).
- h⁻: +0.25 (0.22), +0.07 (0.16), +0.22 (0.29), −0.12 (0.12), −0.54 (0.36), −0.05 (0.05), −0.18 (0.17).
- The z = 0.5–0.7 corners (25 + 35k) move by up to 0.24 (h⁻ 25, σ_MC 0.10) and −0.52 (h⁻ 235, σ_MC 0.22).

**Noise check.** Lead computation from `shift_table.csv` and the per-event bootstrap `S/unfold/mix/boot/boot_table.csv`: 200 replicas, Poisson(1) weights per D event through the release builder, λ held at its nominal value, Condor 5891925. "Noise" is the count expected for pure noise with σ_boot.

| | χ²(s/σ_boot) / 245 | > 3σ_boot (noise) | \|s\| > quad, raw (noise) | √(s²−σ²) > quad (noise) | √(s²−σ²) > total (noise) | median σ_boot/σ_MC |
|---|---|---|---|---|---|---|
| h⁺ | 485 | 7 (0.7) | 100 (82) | 60 (35) | 20 (16) | 1.01 |
| h⁻ | 442 | 6 (0.7) | 112 (91) | 58 (40) | 33 (19) | 1.02 |

The raw per-variant counts in the table above carry a similar noise share.

- **Low z (z_idx < 5).** The shift is smaller: median 1.4–1.5%, with χ²/bin 1.6–1.8 (σ_boot).
- **Bins above 3σ_boot.** h⁺: 34, 64, 69, 96, 120, 144, 174. h⁻: 80, 108, 139, 149, 174, 194. 9 of these 13 are in the highest pT bin (pT_idx 4), and 7 of 13 have z_idx ≥ 5. None is a low-pT corner. h⁺ bin 69 (z_idx 6, pT_idx 4) is +13% at 6.7σ_boot.
- **Mechanism of the low-z shifts (inferred).** The shift there is an efficiency and normalisation effect of the mixture, not only a corner effect.

## Verified vs inferred

**Verified (file-backed in `S`):**
- Sample inventory and processing status.
- Engine and builder reproduction, tree identity, and weight linearity.
- All f values, χ² values and corner fractions.
- All per-bin shifts, σ_MC and σ_boot values, and counts.

**Inferred:**
- The physics content of each sample and the RAPGAP-π irrelevance (h1-mc-expert).
- The (1−f)·ND + f·D recipe.
- The attribution of the η_max 3.2–3.6 mismatch to the LAr edge.
- The RAPGAP-π statistics and luminosity, provisional from one chunk per sample.
- The DJANGOH N_ND, from the tracks method.
- That `bias_rel` is the treatment-R closure term. It may be normalised to the truth rather than to the release yield.

**Not quantified:**
- ND response MC statistics in the shift (only D is fluctuated).
- The reggeon and charm components.
- The DPDF fit label of 9012/PDISS.

## Risks and caveats

- **MC statistics.** The per-cell toys ignore correlations between tracks of the same event. The per-event bootstrap confirms them (σ_boot/σ_MC 0.84–1.12 in the corners). Only the D statistics are fluctuated. The ND response is common to the release and the mix, so its statistics enter the shift only at second order (inferred). The 9012 single ep0607 stream used for all four periods is a physics caveat (one e⁺ stream stands in for the e⁻ periods), not a statistics one.
- **Prior sensitivity.** The prior-only variant (h) moves bins by 8–10% (median), much more than the full mix. In (a) the response change partly cancels it.
- **f is a function of x.** A global f over-mixes the x > 0.03 region, where the fit gives f ≈ 0. An (x, Q²)-dependent f would reduce the shift in high-x bins; this was not run.
- **Fit variable.** Fitting with etaMaxClus08 rather than the vertex-corrected Excl08 is forced by the missing ODS for ND MC.

## Open questions

1. **Adopt a diffractive-admixture systematic?** Options:
   - (i) smoothed shift(a), band from (b)/(c) (recommended);
   - (ii) the raw per-bin |shift(a)|, which is noise-inflated by about √2;
   - (iii) defer until more diffractive MC statistics exist.

   The only statistics gain available without generator work is processing 9018-family events, which cover 150 < Q² < 200 only. This needs a user decision and a budget change, which are outside this study.
2. **Smoothing check.** Apply the chosen kernel to the 200 bootstrap replicas (`S/unfold/mix/boot/out/`) and confirm that the smoothed-shift rms stands above the smoothed-noise rms before adopting a term.
3. **x-dependent f variant.** Rerun the mix with f per x slice (0.164 / 0.137 / ≈0) before a term is adopted.
4. **RAPGAP-π.** Process 8945/8946 only if a leading-neutron or remnant-model variant is wanted. It is not expected to address the gap deficit.

## Outputs

All under `S`:
- `provenance/` (inventory).
- `etamax/` (fit JSON, plots, scripts).
- `response/` (per-sample npz, coverage, checks).
- `unfold/` (driver, reproduction, `mix/` shift tables and plots, `mix/boot/` bootstrap).

Condor clusters: 5891679 (response), 5891703 (η_max), 5891811 (final fit).
