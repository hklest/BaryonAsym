# A reco-level veto against exclusive vector mesons (2026-10-26)

**Scope.** This study covers the high-Q² (Q² > 150 GeV²) charged-hadron SIDIS measurement with the installed eΣ release (`.../data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-24/`). It asks whether a reco-level event selection exists that removes exclusive VM events (DIFFVM elastic ρ⁰, reqIDs 8445–8448, scale 0.135913) and leaves the generator-level observable unchanged. "Unchanged" means three things: no cut on the generator-level phase space, a signal efficiency ε ≈ 1 in every published bin, and no RAPGAP/DJANGOH model dependence. This is a study only. The release, budget, note and plans are unchanged. It follows `diffvm_large_bins_2026-10-25.md` and `exclusive_vm_precedent_2026-10-26.md` in this directory.

Path shorthand: `H` = `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`, `S` = `H/output_v6/studies/vm_veto_cut_2026-10-26` (all outputs of this study), `D` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive`.

## Short answer

- **No SIDIS-neutral veto exists with the stored variables.** Fourteen candidates were tested. Every candidate that rejects most DIFFVM also removes 3–15% of the non-diffractive signal in the high-z, low-pT bins where DIFFVM sits, so the published values would rely on the MC to correct that loss. The cause is physical: real SIDIS events with one leading hadron at high z and one or two central tracks look like exclusive ρ⁰ to any central-detector selection.
- **Best compromise: T4.** The selection is "exactly 2 opposite-sign loose tracks and |z₁+z₂−1| < 0.1". It rejects 59% of DIFFVM tracks overall, and 69% (h⁺) and 82% (h⁻) in gen bin 30. Its signal efficiency is 0.96–0.97 in bin 30 and ≥ 0.93 in every populated bin, for both MCs. In 67–70 of about 480 populated bins it is below 0.99. In bin 30 RAPGAP and DJANGOH agree within 1.2 ± 1.2%; the largest per-bin difference, 3.5%, is in h⁻ 139, outside the DIFFVM bins.
- **The higher-rejection targeted veto: T2.** The selection is "2 OS tracks, and e + tracks balance E−pz and pT within 2 GeV". It rejects 83% of DIFFVM (bin 30: 77%/84%). Its signal efficiency is 0.95–0.97 in bin 30, but drops to 0.85–0.91 in bin 65. RAPGAP and DJANGOH differ by up to 6% (per-bin maximum, h⁻ 65, 2.2σ).
- **Gap and forward vetoes are excluded.** η_max, PLUG and FMD vetoes remove 9–15 of the 46 data tracks in the corner bins, against about 1 predicted by the signal MCs. Both signal MCs are non-diffractive, so most of this loss is presumably diffractive SIDIS (inferred; part may be exclusive ρ⁰), and it is not modelled anywhere in the response.
- **On uncertainties alone, T4 would help in bin 30.** The parallel data-tuned fit (`H/output_v6/studies/diffvm_datatuned_2026-10-26/fit/s_final_r2.json`, in progress) gives a DIFFVM scale s = 0.08 ± 0.15. That implies about 2.8% exclusive content in h⁻ 30 (95% UL 11–15%) and 1.7% in h⁺ 30 (UL 7–9%). The veto's signal loss is corrected through ε in the response, so its cost is the uncertainty on ε, not the loss itself. In bin 30 the subtraction uncertainty σ_s·`diffvm_rho_rel` is about 5.0% (h⁻) and 3.2% (h⁺) without a veto (this assumes the data-tuned s replaces the release's ±100% term, which today is 34.6%/22% in bin 30), and about 0.9% and 1.0% after T4's DIFFVM rejection. The RAPGAP−DJANGOH ε difference that T4 adds there is −1.2 ± 1.2% (h⁻) and +0.3 ± 1.2% (h⁺).
- **Why it is still not "SIDIS-neutral".** ε is 0.96–0.97, not 1, in bin 30, and the correction comes from non-diffractive MC that contains no exclusive events. A veto corrected that way removes exclusive ρ⁰ from the data without putting it back, so the published quantity becomes "SIDIS excluding exclusive VM". By the strict particle-level definition, exclusive ρ⁰ → π⁺π⁻ is signal (§3).
- **Recommendation.** The choice is a definition decision for the user, not a cut-tuning one. If the observable stays the strict inclusive definition, do not veto in the nominal and keep T4 as a cross-check. If "excluding exclusive VM" is acceptable (the release budgets DIFFVM as a ±100% background term but does not subtract it), T4 is the cut to adopt, ideally with a matching generator-level exclusion; this needs a new edition.

## 1. Inputs and validation (verified)

**Available variables.**
- `ChargedTot` holds the event kinematics (Empz, Yh, Xh, Q2h, Yes, Q2es) and the scattered-electron 4-vector (scatE{X,Y,Z,E}, BeamElecE). Per-track it holds Px/Py/Pz/Pt/Eta, z_SIDIS_es, IsFromPrimary and the track-quality branches.
- Yh is the HFS Σ(E−pz)/(2E_e). On a RAPGAP file, Empz = (E−pz)_e + 2E_e·Yh holds to 10⁻³ GeV. "The e + tracks account for the full HFS" is therefore computable from the stored trees.
- `ChargedTot` has no η_max, forward LAr, FMD, PLUG or FTS branches. These are in the sidecar tree `DiffTot` (event-level, keyed by Run/Event), written by `H/src/ChargedSidisTreeBuilderHighQ2_v6dif.C` into `D/output_v6dif/{data,mc}`. The join was tested on one file: 2212 of 2212 events matched.

**DiffTot coverage.**

| sample | files with DiffTot |
|---|---|
| data | 658 of 660 |
| RAPGAP (7071, 7094, 7096, 7097) | 364 of 364 |
| DJANGOH | 7091 only, 201 of 501 |
| DIFFVM | none |
| diffractive RAPGAP 9012, 9155, 9157, 9158 | present |

**Definitions reused unchanged.** The md5-pinned release builders were run as they are. The response builder partitions events into veto groups and drops the reco columns of vetoed groups while keeping the full gen prior; the data and DIFFVM counts zero Empz for vetoed events.
- Response: `H/scripts/unfolding/cone_s2prime_optA_2026-10-19/build_migration_th2_per_charge_cone_s2prime_optA_esxf.py` (FileAccumulator.process_event_chunk).
- Reco-980 counts: `H/scripts/unfolding/cone_s2prime_2026-10-11/build_data_vector_v4_alphaveto_esxf.py` `_select_and_bin` (lines 383–515).
- Gen bins 0–244 per charge, from `binning_v3.py`.

**Validation against the release (exact).**
- RAPGAP (13 jobs) and DJANGOH (7 jobs) response column sums match the release shards to a relative difference of 3×10⁻¹⁵, with identical priors.
- The data reco-980 total, 3,877,298, equals the release `n_raw` bin by bin.
- The DIFFVM total, 2196, equals `counts_980` bin by bin.

All DJANGOH ε use only the 7091 (ep0607) shards, so RAPGAP−DJANGOH differences mix generator and period.

**Definition of ε.** ε_j = (response weight with gen bin j and reco-selected, after the veto) / (the same before the veto). The analysis weights are used. The populated bins are the roughly 480 bins with N_eff ≥ 100.

## 2. Candidates and results (verified, `S/veto_summary.json`, `S/eps_per_genbin.csv`)

**Track and variable definitions.**
- L: loose primary tracks with pT > 0.15 GeV and |η| < 1.6, excluding the electron.
- nL: the number of L tracks.
- dE: 2E_e·Yh − Σ(E−pz)_L.
- pTbal: |pT,e + Σ pT,L|.

**Candidates.**
- M1: nL ≤ 2.
- M2: exactly 2 OS tracks.
- E1: dE < 1 GeV.
- P1: pTbal < 1 GeV.
- F1: η_max of the primary tracks < 1.
- T1: nL ≤ 2, dE < 2, pTbal < 2.
- T2: 2 OS tracks, dE < 2, pTbal < 2.
- T3: nL ≤ 3, dE < 3, pTbal < 3.
- T4: 2 OS tracks and |Σz−1| < 0.1.
- T5: 2 OS tracks and dE < 2.
- T6: nL ≤ 3 and track η_max < 1.
- G1: EtaMaxClus(800 MeV) < 2.
- G2: nL ≤ 3 and EtaMaxClus < 3.
- G3: nL ≤ 3 and E_PLUG < 1 GeV.

**Column key.** "DV rej" is the fraction of DIFFVM tracks removed. The "7 bins" are the gen bins with unfolded `diffvm_rho_rel` > 0.10: h⁺ 30, 31, 65 and h⁻ 30, 31, 65, 100. "Worst ε" is the lowest ε over the populated bins, given as RAPGAP / DJANGOH. "RG−DJ" is the largest per-bin |ε_RAPGAP − ε_DJANGOH|.

| veto | DV rej all | DV rej bin 30 h⁺/h⁻ | DV rej, 7 bins | ε overall (RAPGAP) | ε bin 30 (RAPGAP) | worst ε | RG−DJ |
|---|---|---|---|---|---|---|---|
| M1 | 0.98 | 0.98/0.97 | 0.97–1.0 | 0.990 | 0.81/0.81 | 0.60 / 0.59 | 0.09 |
| M2 | 0.92 | 0.82/0.94 | 0.79–0.97 | 0.994 | 0.89/0.88 | 0.79 / 0.76 | 0.09 |
| E1 | 0.85 | 0.85/0.81 | 0.81–0.94 | 0.966 | 0.72/0.74 | 0.43 / 0.45 | 0.13 |
| P1 | 0.76 | 0.86/0.86 | 0.72–0.94 | 0.979 | 0.64/0.62 | 0.62 / 0.66 | 0.07 |
| F1 | 0.95 | 0.99/1.0 | 0.97–1.0 | 0.897 | 0.65/0.69 | 0.64 / 0.61 | 0.08 |
| T1 | 0.85 | 0.88/0.85 | 0.82–0.97 | 0.9998 | 0.95/0.93 | 0.84 / 0.82 | 0.054 |
| **T2** | 0.83 | 0.77/0.84 | 0.71–0.95 | 0.9998 | 0.97/0.95 | 0.89 / 0.85 | 0.060 |
| T3 | 0.90 | 0.92/0.89 | 0.89–0.97 | 0.998 | 0.77/0.76 | 0.53 / 0.53 | 0.07 |
| **T4** | 0.59 | 0.69/0.82 | 0.41–0.92 | 0.9998 | 0.97/0.96 | 0.93 / 0.93 | 0.035 |
| T5 | 0.85 | 0.77/0.84 | 0.71–0.97 | 0.999 | 0.96/0.94 | 0.81 / 0.79 | 0.077 |
| T6 | 0.94 | 0.98/0.97 | 0.97–1.0 | 0.991 | 0.81/0.83 | 0.76 / 0.73 | 0.06 |
| G1–G3 | not measured (no DIFFVM DiffTot) | – | – | 0.995–0.998 | 0.95–0.99 | 0.84–0.98 / 0.81–0.95 | 0.04–0.10 |

The RG−DJ column is the per-bin maximum |Δε| over about 480 bins (T4: h⁻ 139, 1.5σ; T2: h⁻ 65, 2.2σ). The largest per-bin pull is 2.7–3.9σ for T1–T5, consistent with statistical fluctuation; it is 7.7σ for T6 and 10–59σ for the global vetoes.

**Signal efficiency in the 7 DIFFVM bins** (RAPGAP / DJANGOH):

| veto | h⁺30 | h⁺31 | h⁺65 | h⁻30 | h⁻31 | h⁻65 | h⁻100 | populated bins with ε < 0.99 / < 0.95 |
|---|---|---|---|---|---|---|---|---|
| T2 | .970/.963 | .965/.958 | .894/.897 | .949/.974 | .968/.978 | .914/.854 | .960/.963 | 55 / 16 (RAPGAP), 51 / 13 (DJANGOH) |
| T4 | .970/.967 | .961/.973 | .953/.957 | .960/.971 | .974/.984 | .954/.933 | .961/.967 | 70 / 10 (RAPGAP), 67 / 6 (DJANGOH) |
| T1 | .946/.943 | .940/.928 | .844/.837 | .929/.972 | .945/.962 | .880/.830 | .934/.948 | 62 / 27, 54 / 25 |

The worst T4 bin is h⁻ 206 in RAPGAP, at ε = 0.932 ± 0.011. The worst T2 bin is h⁻ 65 in DJANGOH, at 0.854 ± 0.025.

**Data vs MC for the cut variables.**
- Over the full sample, the data pass fractions for T1, T2 and T4 are 0.9998, and both MCs agree to ≤ 0.001.
- For M1 the data pass fraction is 0.988. For F1 it is 0.895 in data, against 0.899 (RAPGAP) and 0.915 (DJANGOH).
- EtaMaxClus(800 MeV) < 2 removes 2.3% of data tracks but only 0.45% of RAPGAP tracks.
- The distributions of nL, dE, pTbal, η_max, Σz, EtaMaxClus, E_PLUG and FMD are in `S/veto_summary.json` under `variable_agreement`.

**Corner test.** This uses reco bins 550, 551, 200, 620, 60, 760 and 690, which hold 46 data tracks and 15.1 scaled DIFFVM tracks. The prediction is signal MC normalised to (data − DIFFVM), plus the DIFFVM tracks.

| veto | data tracks removed | predicted removed |
|---|---|---|
| T4 | 4 | 14.7 |
| T2 | 7 | 15.3 |
| T1 | 9 | 16.3 |
| M1 | 18 | 20.9 |
| G1 | 9 | 0.4 |
| G2 | 15 | 0.8 |
| G3 | 11 | 1.6 |

- The exclusive vetoes remove less data than predicted. This agrees with the 10-25 null test and the data-tuned fit: DIFFVM overestimates the exclusive ρ⁰ yield.
- The gap vetoes remove about 10 times more data than predicted. The G predictions include no DIFFVM rejection (no DiffTot), so about 1–6 of the excess tracks may be exclusive ρ⁰ (s = 0.08 to the conservative UL). The rest is presumably diffractive data, i.e. real signal (inferred).

**Price of the exclusivity tag.** 344 data events have 2 OS tracks and |Σz−1| < 0.1. In that selection, RAPGAP normalised to the data predicts about 455 non-diffractive events, and DIFFVM adds 89. (RAPGAP normalised with the stage-A event ratio, 24.4.) The tag population is therefore mostly ordinary SIDIS.

## 3. The diffractive subtlety (verified unless marked)

- **The signal MCs have no rapidity-gap (LRG) events at a realistic rate.**
  - The central samples are RAPGAP 7071/7094/7096/7097 and DJANGOH 7072/7091/7092/7093, all NC RAD Q2GT60 and labelled non-diffractive. Sources: `D/docs/mc/composite_scale_table_v3_2026-10-20.csv` and `nd_generator_leakage_adjudication_2026-10-20.md`.
  - The truth-level fiducial LRG rate is about 1×10⁻⁴ per event in RAPGAP and about 2.5×10⁻⁴ in DJANGOH (`D/docs/mc/period_matched_pair_record_2026-10-20.md` §0.3).
  - In data, 7.6% of preselected events are LRG candidates (`D/docs/t2/lrg_selection_study_2026-10-18.md`).
  - Neither generator produces exclusive VMs.
- **A gap veto's efficiency is unmodelled, not merely model-dependent.** In the response MC it is about 1. In data the veto removes the diffractive part of the signal, about 7–10% of events and, per h1-mc-expert (inferred), about 10–25% of tracks at z > 0.7 and pT < 0.5 GeV. The response cannot put that loss back. Absorbing it would need a non-diffractive + diffractive RAPGAP (+ DIFFVM) mixture. The parked diffractive extension found that these mixtures fail the goodness-of-fit (`D/context-handoff.md`, "Current Status — PARKED"). Two further known problems: FMD noise is not simulated (`H1CalcForwDets.C:31-46`), and the FMD veto shape fails under every mixture tested (DF-SYS-02).
- **Effect on the diffractive RAPGAP samples** (9012, 9155, 9157, 9158; unnormalised, N_eff 1–18 in bin 30). T4 keeps ε = 0.82–1.0 in bin 30, and T1/T2 keep 0.67–1.0. For the T vetoes the overall ε is ≥ 0.999. Within these low statistics, the targeted vetoes therefore cost little diffractive signal beyond what they cost in non-diffractive events.
- **Is exclusive ρ⁰ signal or background?** By the particle-level definition (stable charged hadrons, one true γ*), exclusive ρ⁰ → π⁺π⁻ is signal. It is ep → e h X, even though it lies outside fragmentation factorisation (h1-mc-expert, inferred). HERMES and COMPASS subtracted it and published the results with and without the correction. A reco veto whose efficiency is taken from non-diffractive MC silently redefines the observable as "excluding exclusive VM". Making that consistent would need a matching generator-level exclusion, which is a change to the observable definition.

## 4. What no candidate rejects

- **Extra central tracks.** 1.3% of DIFFVM events have ≥ 3 L tracks. Any nL ≤ 2 or 2-OS veto keeps them. After T1, the surviving DIFFVM events are 82 with 1 track, 103 with 2 tracks and 15 with ≥ 3. One-track survivors are the other pion outside |η| < 1.6.
- **Proton-dissociative ρ⁰.** This is about 0.5–0.8× the elastic yield and is not in DIFFVM. Per h1-mc-expert (inferred), the Y system sits at η ≈ 5–7. It is invisible to tracks and the LAr, and partly visible to FTS/PLUG. The T vetoes act on it exactly as on elastic ρ⁰, but no MC exists to measure that.
- **ω, φ and ρ′.** The ω gives three pions including a π⁰. The φ gives K⁺K⁻ at z ≈ 0.5 each. Neither has a sample.
- **Gap vetoes on DIFFVM.** DIFFVM has no DiffTot, so the rejection was not measured. The pion η_max (5th/50th/95th percentile −1.1, −0.05, 0.97) suggests G1 would reject nearly all elastic DIFFVM (inferred), but G1 also removes real diffractive signal (§2).

## 5. Cost of adoption

| item | T4 or T2 (stored variables) | gap/forward vetoes |
|---|---|---|
| New variables | none | DIFFVM DiffTot: a stage-2-only v6dif rerun of 404 jobs, about 1–2 h (plan `H/condor_v6/diffvm_rho_2026-10-07/SUBMISSION_PLAN.md`); DJANGOH 7072/7092/7093 DiffTot (300 files) |
| Response | rerun the release migration builder with the veto (stage B of this study is a working prototype, 38 jobs) | same, plus a diffractive-MC mixture in the response, which is known to fail the goodness-of-fit |
| Release | re-unfold, groups A–C budget, assemble: a new edition and a new user decision | same |
| New systematic | RAPGAP−DJANGOH ε difference in bin 30 ≤ 1.2 ± 1.2% (T4) or 2.6 ± 1.4% (T2), per-bin maxima 3.5% / 6% consistent with fluctuation; a cut-variation term; in exchange the DIFFVM term in bin 30 drops from 5.0/3.2% to about 1% | unmodelled diffractive loss of order 10–25% in the corner |
| Observable | redefined as "excluding exclusive VM" unless a generator-level exclusion is added | removes diffractive DIS, which theory includes |

## Verified vs inferred

- **Verified (file-backed in `S`):** all ε, rejection, pass-fraction and corner numbers; the validation against the release; DiffTot coverage; the non-diffractive labelling of the signal MCs and their LRG rates.
- **Inferred:**
  - the reading that DIFFVM overestimates the exclusive yield, based on the corner deficits;
  - the attribution of the gap-veto excess to diffractive data;
  - the diffractive track share at high z (10–25%);
  - where the proton-dissociative Y system goes, and the G1 rejection on DIFFVM;
  - the signal-versus-background classification of exclusive ρ⁰.

  The data-tuned scale s = 0.08 ± 0.15 comes from a parallel study that is still in progress.

## Open questions

1. Strict inclusive observable or "excluding exclusive VM"? See the Short answer and §5. This needs a user decision.
2. Not done: the h1-mc-db steering confirmation, the charm and Reggeon diffractive samples, and the composite normalisation of the diffractive samples.

## Outputs

- `S/veto_summary.json`, `S/eps_per_genbin.csv`, `S/reco980_counts.csv`.
- Scripts in `S/scripts/` (vetovars.py, cands.py, stageA_vars.py, stageB_resp.py, analyze.py, and the .sub/.args files); stage outputs in `S/out/A` and `S/out/B`; logs in `S/logs/`.
- Condor clusters:
  - 5891093 and 5891122: stage A (49 jobs; data and DJANGOH rerun in 5891122).
  - 5891177: stage B, 38 of 38 succeeded.
  - 5891430: analysis, 1 job.
