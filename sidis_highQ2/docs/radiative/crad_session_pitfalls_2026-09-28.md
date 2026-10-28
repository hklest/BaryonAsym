# C_rad session findings and pitfall log (high-Q2 charged-hadron SIDIS, 2026-09-25 to 2026-09-28)

**STATUS: DIAGNOSTIC. `c_rad_applied=false`. Central v6 untouched.**

The session began from a reported `C_rad ~ 1.15` (reaching 1.5 in some bins). Seven distinct defects were found and fixed, five in the study code and two in session-level analysis scripts. The current best estimate of the per-DIS-event hadron multiplicity ratio is **~1.05 at `GenEmpz > 45`**, which the analyst judges consistent with the ungroomed 1-jettiness benchmark. The binned cross-section ratio at the same cut is 1.1372; whether ~1.05 or ~1.14 is the correct answer depends on an undecided normalization question (Section 4).

## 1. Pitfall log with C_rad values

| # | Pitfall | What was wrong | C_rad with defect | C_rad after fix |
|---|---|---|---|---|
| 1 | Mixed/mislabeled NONRAD population | Candidate's "OLD NONRAD 790 e+/760 e-" was actually 260 legacy Britzger + 530 new (e+), and 260 legacy + 500 **quarantined duplicate-RNIN** July campaign (e-) | 1.1156 per charge; median 1.1347, max 4.0668, 114/490 bins outside [0.5,1.2] | Population re-enumerated: 260+260 legacy, 530+530 new |
| 2 | **MC luminosity convention mismatch** (largest single error) | RAD used weighted `L = sum_w(w1*w2)/SIGMT`; OLD NONRAD used unweighted `L = N_generated/SIGMT`. DJANGOH MCWE/GKI weights are non-unit (mean w1 ~3.46 RAD, ~2.53 NONRAD), so conventions differ by ~1.83 | 2.04009 (h+) / 2.03902 (h-) | 1.11295 / 1.11237 |
| 3 | Swapped sin/cos in electron-method Q2 and y | Study used `Q2 ~ sin^2(theta/2)`, `y ~ cos^2(theta/2)`. Correct for H1 (theta from +z proton direction): `Q2 = 4*E_eff*E'*cos^2(theta/2)`, `y = 1-(E'/E_eff)*sin^2(theta/2)` | Pearson r vs hard-vertex `Q2Gki` = 0.0187; 92.21% of RAD events with true `Q2Gki<100` promoted above the `Q2>150` cut | r = 1.000000; promotion 0.00% |
| 4 | Lab-frame instead of gamma*p-CM pT_h | Study projected transverse to lab `q`; release bins `pTh_cm` after boosting to the `W=q+P` rest frame. The gamma*p boost is ~123 deg off the photon direction, so transverse invariance does not apply | Table not bin-compatible with the release at all | Matches central migration helper to **max abs diff 0.0 GeV** |
| 5 | RAD/NONRAD generated-domain mismatch | RAD is Q2GT60, NONRAD Q2GT100; SIGMT ratio 15040.275/7287.9104 = 2.0637294059 | Fixed by numerator-only `Q2Gki>100` on RAD | +0.066% (h+) / +0.068% (h-) |
| 6 | **Cross-file event-key collision** (analysis-side, mine) | NONRAD `(RunNumber, EventNumber)` keys repeat across files: 27.6% duplicate in OLD, 61.9% in NEW. A global `np.isin` join over-counted hadrons | Multiplicity ratio 1.6507 (e+) / 1.6834 (e-), reported as a factor-1.65 pathology | 1.0467 / 1.0593 with file-scoped joins |
| 7 | **Subset-vs-full-manifest exposure mismatch** (analysis-side, mine, **UNRESOLVED**) | Event-rate/multiplicity decomposition summed weights over only 40 files per sample while dividing by full-manifest luminosities (RAD 731, OLD 260, NEW 530 files) | Nonsensical event-rate factor 2.8226 (ep) / 2.3689 (em); product 2.98 / 2.53, irreconcilable with binned 1.1372 | Decomposition **invalid**, must be redone over the full manifest |

Notes on #5: the in-fiducial effect is small (`Q2Gki>100` removes only ~0.09% of RAD events in-fiducial, though 33-40% of all generated RAD events). A proposed repair that additionally rescaled `L_RAD` by the generated-domain weighted fraction was **refuted** by direct measurement: `f_w = 0.511 (ep) / 0.513 (em)`, which would have driven C_rad to ~0.59.

Notes on #3 and #4: **CENTRAL v6 IS NOT AFFECTED** by either. Central uses `2*E_eff*E'*(1+cos theta) = 4cos^2(theta/2)`, verified at `ChargedSidisTreeBuilderHighQ2_v6.C:1330-1336`, and its pT_h is internally consistent across truth, reco, and matched-truth legs.

Notes on #6: the artifact `isr_multiplicity_check.json` is **superseded and quarantined**; use `isr_multiplicity_check_v2.json`.

Notes on #7: the multiplicity ratio ~1.05 is **not** affected, because numerator and denominator come from the same files and luminosity cancels exactly.

## 2. GenEnPhoton is the ISR photon, but has an unphysical tail

The analyst asked whether `GenEnPhoton` is the ISR photon or the virtual photon.

Measurements: correlation with the lab virtual-photon energy `q0 = E_e - E_e'` is only **0.050** (0.107 among events with `GenEnPhoton>0.5`). It is therefore **not** the virtual photon. HAT documentation describes it as the energy of the hard initial-state radiated photon, used to build `E_eff = E_e - E_gamma_ISR`.

However, **0.052% of RAD events (407 of 790,000) have `GenEnPhoton` above the 27.6 GeV electron beam energy, with a maximum of 237.0 GeV**, which is kinematically impossible for ISR. These are corrupt or mis-filled entries and should be vetoed.

**Correction to previously reported results:** the 32-200 GeV bin in this session's Fig 1 and Fig 4 is populated **entirely** by these unphysical events, and the 16-32 GeV bin is partly contaminated above 27.6 GeV. The previously reported "~28% ISR multiplicity suppression in the hard tail" is **overstated**. Within the physically allowed range the suppression is from 14.083 at `GenEnPhoton -> 0` to about 11.5 in the 16-27.6 GeV bin, i.e. **roughly 18%**. The monotonic falling trend and the qualitative conclusion that ISR reduces hadron multiplicity are unchanged.

Recommend adding a `GenEnPhoton < GenBeamElecE` veto and regenerating those two figures.

## 3. What is established

- **The analyst's ISR mechanism is confirmed.** Lower `sqrt(s)` under ISR gives lower hadron multiplicity. `<n_had>` per fiducial DIS event falls monotonically with `GenEnPhoton`, and as `GenEnPhoton -> 0` the RAD value (14.083 e+, 14.051 e-) approaches the NONRAD value (14.742 e+, 14.884 e-).
- **The virtual-photon definition does NOT drive the magnitude.** Variants A (nominal electron q), B (Sigma photon) and C (e-Sigma photon) agree to within 0.007% integrated, and to 4 decimal places after the normalization fix. They differ only in **shape**: across z, A rises 1.132->1.299 (+14.7%), B 1.135->1.217 (+7.2%), C 1.134->1.256 (+10.7%); across pT_h, A falls -4.8%, B -1.2%, C -1.9%.
- **Correct e-Sigma algebra:** `q = (sqrt(Q2(1-y))cos(phi_q), sqrt(Q2(1-y))sin(phi_q), -E_e*y - Q2/(4E_e), E_e*y - Q2/(4E_e))` with `phi_q = phi_e + pi`. Q2 and y fix `q0`, `qz` and `|qT|`; the electron azimuth fixes only the transverse orientation. Closure verified: `|q.q + Q2|/Q2 < 5.46e-16`, y recovered to 4.44e-16.
- **EMPZ scan** (integrated, variant A): `empz_none` 1.0999, `35<EMPZ<70` 1.1127, `>45` 1.1372, `>50` 1.1699, `>55` 1.3632. C_rad rises monotonically with the EMPZ threshold. This reproduces the analyst's report that the groomed 1-jettiness analysis, with a tight E-pz cut, obtained ~1.15 while the ungroomed analysis at E-pz>45 sat lower.
- **y dependence** is flat to slightly falling: -2.5% to -4.4% across `0.1<y<0.7` for all modes except `>55`, which is -10.3%.
- **z-vertex:** RAD HAT `VtxZ` is genuinely smeared (mean +0.365 cm, RMS 16.067 cm, 3.139% outside `|VtxZ|<35 cm`) while OLD and NEW NONRAD are exactly `VtxZ=0`. This confirms the analyst's recollection and the prior note `docs/research/research-crad-lumi-pilot-bunch-zvertex-2026-06-09.md`. It is quantitatively excluded as a cause here (max 3.24% effect; gen-only partials apply no vertex cut), but becomes a live trap the moment a vertex cut is introduced, where the remedy is `L_eff = L_file * f_z` per sample.

## 4. Open items

- **OLD vs NEW NONRAD disagree in `<n_had>`:** OLD 14.7873 (e+) / 14.9437 (e-) versus NEW 14.0358 / 13.9610, a ~5% spread comparable to the residual RAD-vs-NONRAD offset itself. NEW agrees with RAD no-ISR to ~0.3%. **This is the dominant open question** and must be resolved before finalizing.
- The event-rate vs multiplicity decomposition must be redone over the full manifest (pitfall 7).
- Whether the DIS event-rate factor belongs inside C_rad or cancels against the data normalization is **undecided**, and determines whether the answer is ~1.05 or ~1.14.
- `P-CRAD-LUMI`: exposures are `DIAGNOSTIC_MANIFEST_LEVEL` — exact for retained manifests, not raw-generator certified.
- `P-CRAD-SPARSE`: no approved sparse-bin masking/merging/smoothing policy.
- **Reco EMPZ discrepancy in central v6:** the response executes `35 < Empz < 70` (`build_migration_th2_per_charge_v6.py:172-178, 511-521`), Stage-1 uses a `(30,75)` envelope, a Stage-2 comment says `[45,65]`, while the analyst states the data cut is `>45`. Needs a ruling; potentially release-affecting.
- Central v6 truth response applies **no** `GenEmpz` cut ("read for downstream diagnostics; NOT cut on"), so imposing a truth EMPZ cut in a gen-only C_rad may be inconsistent with the open Born phase space the unfolding corrects to.

## 5. Key artifacts

| Item | Path |
|---|---|
| Corrected-normalization tables | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/esigma_photon_variant_2026-09-26/corrected_normalization/` |
| EMPZ>45 nominal + all modes | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/quarantine/esigma_photon_variant_2026-09-26/empz45_nominal/` |
| ISR multiplicity data | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/isr_multiplicity_diagnostics_2026-09-27/isr_multiplicity_check_v2.json` (non-v2 file **QUARANTINED**) |
| Plots | `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/radiative/isr_multiplicity_2026-09-27/` (`isr_multiplicity_diagnostics.pdf`, `fig1`-`fig4` PNGs, `page-1..4.png`) |
| Builder | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/src/RC_treebuilder_genonly_v3.C` |
| Worker / finalizer | `.../scripts/histogramming_v3/crad_v3_worker.py`, `crad_v3_finalize.py` |
| Campaigns | `.../output_v3/histogramming_v3_empz45_20260927/` (51,912 partials); `.../histogramming_v3_empz6_20260927/` (77,868 partials) |

## 6. Operational lessons

- `thish1.sh` returns non-zero and references an unset `GROUPROOT`. Wrappers must **not** use `set -e` or `set -u` around it, and must validate the environment positively instead of trusting the exit code. Violating this gave empty logs and 100% job failure.
- HTCondor `retry_until` is a **STOP-on-true** condition. For AFS-flaky nodes use `retry_until = (ExitCode != 127)`, not `== 127`. The inverted form silently never retried.
- `MAX_JOBS_PER_OWNER` is 5000 on this schedd, and unrelated held jobs consume the quota, so large campaigns must be chunked.
- Always validate a wrapper with a real multi-job Condor test, not a login-node test. A login-only check passed while every Condor job failed.
- Regression-compare against a freshly rebuilt baseline binary, never a stale historical partial. The latter produced a spurious `phi_h` 2*pi failure.
