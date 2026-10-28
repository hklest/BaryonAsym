# Script Audit 2026-10-19 — Loop 3 (final adjudication): TREEBUILDER + central-chain consumption (v6, high-Q2 charged SIDIS)

**Auditor:** physics-correctness critic, loop 3 of 3, read-only. No analysis script modified.

**Material reviewed:** loop-1 and loop-2 treebuilder reports; the M1 empirical provenance check; the N1/M2 RAPGAP31 straddle quantification; fresh loop-3 reads of the Stage-2 eSigma/HAT computation block (`src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 960–1219, pointer declarations 428–452), the three central submitters (`scripts/submit_stage2_v6_central.py` all 1221 lines; `scripts/submit_full_production_v6.py`; `scripts/production/v6_central/submit_full_production_v6_central.py`), `binary_identity_manifest.json`, the actual central Stage-2 wrapper and steer for the M1-check production unit, consumer confirmation in `scripts/unfolding/cone_s2prime_2026-10-11/{build_data_vector_v4_alphaveto.py, build_migration_th2_per_charge_cone_s2prime.py}`, and the central evls tree population. Observable class: IBU-unfolded inclusive charged-hadron ep cross-sections (2026-10-17 Set-R′ clean release, cone basis). Family: sidis_highQ2.

## VERDICT: CONDITIONAL-PASS (builder + submission layer); released per-cell values PROVISIONAL pending N1/M2 rebuild and N2 budget closure

The per-event tree content, species truth, weights, frame algebra, and the submission/steering wiring are sound and were verified end-to-end; no fatal defect exists in the builder or in what the submitters feed it. The empirical follow-ups converted N1 and M2 from unquantified exposures into demonstrated MATERIAL, common-mode biases of the released per-cell and per-z-bin central values — uncovered by the quoted error budget — and N2 is a demonstrated missing systematic channel. Integrated released values remain robust (the biases are redistribution-level).

## Part A — eSigma residual check (decisive; from the unfolding loop-3 recommendation)

**Answer: the unfolding layer's M1 exposure is bounded to zero ONLY for the Q2/y/x eSigma fiducial axes; it is NOT bounded to zero overall, because `scatE*` is not a calculator branch. The unfolding PASS therefore does not stand unconditionally.**

- `Q2es/Yes/Xes` are read from `gH1Calc->Kine()` (lines 1064–1066) after `gH1Calc->Reset()` (965) and `v6sys::ApplyV6SysShift` (969): calculator-derived, independent of the HAT-frozen `Q2s/Ys` (separate variables, 1037–1038), and shift-responsive on sys legs (loop-2 confirmed). The central data-vector/response fiducial cuts on exactly these branches (`0.1<Yes<0.7`, `150<Q2es<20000`, `cell_index_reco(Q2es,Xes)`; data-vector lines 207–213, 431–436, 492). M1 exposure of the Q2>150 / y / reco-cell selection: **zero**. Empirical corroboration: the live `Q2es` column reproduces the sidecar calibrated values 64/65.
- `scatE*` is computed from production-frozen HAT pointers `hatElecE/hatElecTheta/hatElecPhi` (lines 1128–1134) — NOT from the eSigma calculator. The unfolding layer builds the released z and pT_h axes and the alpha cone from `scatEX/Y/Z/E` + beam energies (`_compute_pTh_cm_and_z`; data-vector lines 245, 399–402, 484). The released observable axes therefore share HAT-frozen inputs.
- `Empz` (Sigma-approximation from HAT `Ys` + HAT electron, 1171–1177) and `PVz` (`*hatVtxZ`, 1041) are HAT-frozen and enter the released event cuts (`Empz>45`, `|PVz|<35`). `xF_SIDIS_es` is mixed (calculator `Q2e` + HAT `eTheta/ePhi`, gammaES block 1184–1206).

**Consequence:** the nominal central values remain internally consistent (identical branches on data and MC; `scatEE` empirically equals the calibrated sidecar ElecE on 64/65 clean events, PVz 64/65, Weight1 identically 1.0), so no corruption of nominal central values is demonstrated. The surviving exposure is (i) definitional provenance — the released observable basis (z, pT_h, alpha) and the Empz/PVz cuts sit on production-HAT state, not the HadrooII-calibrated state the documentation describes (M1; must be documented truthfully), and (ii) on HFS systematic legs the frozen Empz/scatE* do not respond — the missing HFS-Empz channel (N2). The elec energy/theta legs are shifted analytically downstream (loop-2 verified) and are not corrupted by the freeze. The unfolding layer's PASS must be amended from "M1 exposure bounded to zero" to "bounded to zero for the Q2/y/x fiducial only; observable axes and Empz/PVz cuts remain on frozen state".

## Part B — submission/steering sweep

**Verdict: the actual central production wiring is CLEAN (verified end-to-end); four latent operational traps found, none corrupting the released trees.**

Path correction: the originally stated paths (`scripts/canonical_highQ2_sidis_2026-06-30/submit_*.py`) do not exist. The audited submitters live at `H1ChargedSidis_HighQ2/scripts/` (two) and `scripts/production/v6_central/` (the canonical two-stage driver).

- **No unread builder feeds central submission.** All three submitters reference only `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2_chargedsidis_v6` and `.../ChargedSidisTreeBuilderHighQ2_v6`. No `src/` stage2 variant that loops 1–2 did not read (v6sys, v6bsa, v6dif, v6flatphiLink, v6_assocElecTest, v6_cradDiag, v5g, plain, radMigration diagnostic) is wired into central submission.
- **End-to-end wiring verified** on the M1-check production unit: wrapper runs the v6 Stage-2 binary, `--mode data`, steer sets only `fEventListFiles = output_v6/central/evls/data/03/evls_v6_d03_0006.root`, output to `output_v6/central/sidistree/data/ep0304/`. Correct binary, correct current evls, correct output tree. GRL dispatch (per-period; year-06 chunk-220 em06/ep0607 split; year-04 c040062 zero-output commissioning tail) is correct and smoke-documented; Stage-2 wrappers pass no GRL (correct in event-list mode).
- **B1 (ROBUSTNESS, dormant wrong-input trap).** `scripts/submit_full_production_v6.py` — despite the `_v6` name, v6 binaries, and the CENTRAL output base (`output_v6/central/sidistree`) — globs the SUPERSEDED `evls_highQ2_*.root` in its Stage-2 arm (lines 991, 1043) and parses only that prefix. Dormant today (verified: only `evls_v6_*` exists in `output_v6/central/evls/` data and MC trees, so a rerun submits nothing and warns). If its `--stage evls` arm were run first it would seed `evls_highQ2_*` files into the v6 evls tree, and its Stage-2 arm would then build central-output trees from superseded event lists under output labels that collide with the canonical ones (`sidistree_<period>_<tail>`). It is NOT the driver of current production (it cannot match `evls_v6_*`).
- **B2 (ROBUSTNESS).** `submit_stage2_v6_central.py` — the ACTUAL central Stage-2 driver (its `condor_v6/central/sidistree/wrappers` and `steering_v6/central/sidistree` artifacts exist on disk and match the production logs) — has a stale Stage-1 arm: it writes `evls_highQ2_{label}.root` into `output_v6/central/evls/` (the v6-canonical tree) while its Stage-2 arm reads only `evls_v6_*`. Its Stage-2 arm is correct (v6 glob, dual-prefix parse, per-chunk period dispatch).
- **B3 (ROBUSTNESS).** `submit_full_production_v6_central.py` (canonical two-stage driver; correct `evls_v6_*` Stage-1 naming; v6 binary md5 stamps matching STATE.md's recorded `9a585a3d...`) silently accepts a legacy `evls_highQ2_*` fallback in its Stage-2 union glob (lines 695–699, 732–733). Benign today; remove the fallback.
- **B4 (ROBUSTNESS, guardrail regression).** None of the three v6 submitters invokes the Guardrail-3 `_binary_guard.py` verification, and `binary_identity_manifest.json` (v1.6) predates the v6 binaries (pins only through v5g). v6 binary identity rests on the submitter md5 docstring stamps (matching STATE.md) plus loop-1's canonical source byte-identity; the empirical checks corroborate source↔binary correspondence (stored HAT-frozen Q2s + live calculator Q2es behave exactly as the audited source predicts).
- Stale v5g docstrings in both root-level submitters (docstring says the Stage-2 binary is `_v5g` while the constant is `_v6`); log-tree split (`sidistree_highQ2_v6` vs `..._v6_central`); mixed steer-dir constants — documentation/cosmetic.

## Part C — final adjudication

### C.1 Adjudication table

| ID | Class | Finding | Corrupts released central values? | Corrupts quoted errors? | Exact fix | Blocking? |
|---|---|---|---|---|---|---|
| M1 | BUILDER-PROVENANCE | Stage-2 event kinematics from production-frozen HAT pointers; "calibrated eSigma from upstream" claim false (0/65 identical Q2s/Ys; 7/65 Q2>150 decision flips frozen-vs-calibrated; PVz/scatEE consistent 64/65; Weight1 = 1.0) | **No** — released fiducial on calculator Q2es/Yes (Part A); data/MC symmetric | Via N2 only | Correct header/provenance claims; document the true released branch basis (Q2es/Yes calculator; z/pT_h/alpha on HAT scatE*; Empz Sigma-approx; PVz HAT); one-time degenerate-event audit | ACTION (method description must be true); numbers stand |
| M2 | TRUTH-DEFINITION | Gen z on nominal beams (ISR-biased low) while gen Q2/y/x are rad-safe — one response mixes truth definitions; 3.25% of fiducial gen rows change z bin (100% upward; ISR median 9.7%, max 81%; dominant 0→1: 465, 1→2: 289) | **Yes — per-z-bin** (concentrated in z<0.05 and 0.05–0.1 rows) | Not covered (common-mode: both DJ and RG responses carry it; `ibu_model_rel` blind) | Rebuild response with offline-computable rad-safe `z_rad` (validated formula; branches exist), or documented nominal-beam truth-contract decision + z-shift systematic | **ACTION — blocks quotability** |
| M3 | MATCHING | Tier-0 truth link: no exclusivity/angle/index; one-to-one contract only at Tier-2 | No demonstrated corruption (central builders protect the miss side via consumed-gen keys; pool-identity gates passed) | No | Quantify Tier-0 wrong-gen-link rate once; add angle+exclusivity at Tier-0 in next builder revision | Hardening |
| M4 | DENOMINATOR-TRAP | Zero-hadron DIS events write no Stage-2 row (DisTot detached); N_DIS must come from the EvlTree sidecar | No — released cross-sections are per-track yields ÷ L; no N_DIS consumed | No | Enforce sidecar-sourced N_DIS for any multiplicity product; never re-attach DisTot before fixing m1 | Hardening (latent trap) |
| M5 | ROBUSTNESS | Sidecar exhaustion continues silently with passRecoEnv = true | Not observed; operator-error-triggered; corrupts all downstream if fired | No | Hard-fail on exhaustion (1017–1021) | Hardening |
| m1 | DENOMINATOR-TRAP | `nChargedGen` reset after the gen loop (latent, detached DisTot only) | No (DisTot detached) | No | Move reset before gen loop if DisTot re-attached | Hardening |
| m2 | BUILDER-PROVENANCE | Stale Stage-2 header claims final cuts applied upstream | No (docs) | No | Fix header (M1 doc sweep) | Hardening |
| m3 | ROBUSTNESS | Trigger-weight header contradiction; null `H1TrigInfo` → zero weights (loud failure) | No | No | Align docs; optional fail-fast | Hardening |
| m4 | BUILDER-PROVENANCE | Comment claims Stage 1 wrote zero for gen-only HAT quantities | No (docs; code defensively correct) | No | Fix comment | Hardening |
| m5 | ROBUSTNESS | Gen-only mode hardcodes \|VtxZ\|<30 vs `PhysConst::VTX_Z_MAX`=35 | No (NORAD VtxZ≈0 today) | No | Use the constant | Hardening |
| m6 | BUILDER-PROVENANCE | Sidecar `GenVtxZ` stores HAT/reco VtxZ (no consumer) | No | No | Rename or document | Hardening |
| m7 | ROBUSTNESS | GRL missing → silent continue | No (central submitters pass per-period GRLs correctly — verified) | No | Fail-fast on missing GRL | Hardening |
| m8 | ROBUSTNESS | Pass-2 sanity cuts let NaN Pt/P through (loud failure) | No | No | Two isfinite guards | Hardening |
| N1 | TRUTH-DEFINITION | Response bins prior/miss gen by calculator `Q2esGen` (cone builder 1001–1031) but matched body by rad-safe `Q2_es_gen` (1347–1407); ISR rows straddle cells: 1.94% of builder-fiducial gen rows (weighted 1.57%); per-cell S2_x0 8.74%, S3_x0 6.21%, S1_x0 2.76%, S1_x1 2.34%, S2_x1 1.84%, S0 0%; 100% one-directional (Q2esGen > Q2_es_gen) | **Yes — per-(Q2,x)-cell** (see C.2) | Not covered (common-mode structural defect; generator-model spread blind) | Harmonize gen-Q2 cell index (rad-safe on the pool/miss side, matching fiducial+body); rebuild response; re-run IBU k=4; re-release; quantify per-cell shift | **ACTION — blocks quotability** |
| N2 | SYSTEMATIC-UNDERRESPONSE | HFS legs leave `Empz` (and scatE*/z/pT) bit-identical; the physical HFS-scale → Empz>45 channel absent from the released budget | No (central values) | **Yes — quoted HFS-leg errors under-cover** | Add HFS-Empz channel (analytic Empz shift from stored primitives or envelope); re-freeze budget | **ACTION — blocks quoted errors** |
| N3 | MATCHING | Body dispatched by GEN charge vs data by RECO charge (deliberate ε>1 fix); residual bounded by unquantified Tier-0 wrong-charge links | No demonstrated corruption | No | Quantify with the M3 Tier-0 audit | Hardening |
| N4 | (status) | Released cross-sections on raw/no-bgsub stopgap | **RESOLVED** by the 2026-10-17 Set-R′ clean-release promotion (STATE.md FINAL) | — | None | Resolved |
| N5 | BUILDER-PROVENANCE | Stale docstrings (MATCHED-row charge dispatch; v5g submitter headers) | No (docs) | No | Documentation sweep | Hardening |
| B1 | ROBUSTNESS | `submit_full_production_v6.py` Stage-2 globs superseded `evls_highQ2_*` with `_v6` name and central output base (dormant) | Not current (dormant; no legacy evls in tree — verified) | No | Quarantine/rename or re-glob `evls_v6_*` + hard-fail if none | **ACTION (cheap corruption-prevention)** |
| B2 | ROBUSTNESS | `submit_stage2_v6_central.py` Stage-1 arm writes `evls_highQ2_*` into the v6 evls tree | Not current | No | Fix Stage-1 naming or disable the arm | Hardening |
| B3 | ROBUSTNESS | Canonical driver's union glob accepts legacy fallback | Not current (no legacy files — verified) | No | Remove the fallback | Hardening |
| B4 | ROBUSTNESS | No binary guard wired for v6; manifest predates v6 binaries | Not current (md5 stamps + empirical corroboration) | No | Add v6 manifest pins; wire `_binary_guard` | Hardening |

### C.2 N1 mechanism ruling — why IBU re-normalization does NOT absorb the straddle

The coherent prior is the response's own truth marginal `body+miss`. For straddling ISR rows, the miss/pool books the gen hadron in the HIGHER-Q2 cell (calculator `Q2esGen`) while the matched body books it in the TRUE (rad-safe) cell. Per gen bin: the upper cell's miss is inflated → its efficiency `body/(body+miss)` is biased LOW; the lower cell's body contains straddlers whose miss partners sit in the upper cell → its efficiency biased HIGH. The IBU fixed point preserves the TOTAL (the marginal is coherent) but inherits the per-cell efficiency split, because the response's gen axis is internally inconsistent — the same physical gen hadron is assigned to two different cells on the two sides of the same response. The 100% one-way direction (prior → higher stratum) makes the per-cell bias coherent and sign-definite (upper cells' unfolded cross-sections biased high, lower cells' biased low), so it does not average out within a cell. Expected magnitude: a diluted fraction of the per-cell straddle rate (2.3–8.7% across the five affected cells) — percent-level, comparable to or above individual quoted systematic components. Integrated cross-sections are nearly conserved (redistribution); per-cell cross-sections are biased. Hence: response rebuild required; not a normalization patch.

### C.3 Action-required vs hardening-only

**ACTION before the analysis is quotable beyond provisional status:**
1. **N1** — harmonized gen-Q2 cell index, response rebuild, re-release of per-cell values.
2. **M2** — rad-safe gen z (or documented truth-contract decision + z-shift systematic), response rebuild.
3. **N2** — HFS-Empz systematic channel added; budget re-frozen.
4. **B1** — remove the dormant wrong-input submitter trap (minutes of work; protects any re-production).
5. (Action-lite, no number changes) **M1** — provenance/documentation corrected so the method description matches the implementation; one-time degenerate-event audit.

**Hardening-only (do not block the release):** M3, M4, M5, m1–m8, N3, B2, B3, B4, and the N5 documentation sweep.

### C.4 Overall verdict

**CONDITIONAL-PASS** for the treebuilder sources plus central-chain consumption. The per-event tree content, species truth, weights, frame algebra, and submission/steering wiring are sound and verified end-to-end; no fatal defect exists in the builder or in what the submitters feed it. The conditionality has sharpened from loops 1–2: the empirical checks converted N1 and M2 from unquantified exposures into demonstrated MATERIAL, common-mode biases of the released per-cell and per-z-bin central values (uncovered by the quoted error budget), and N2 is a demonstrated missing systematic channel. The released clean-release integrated values remain robust; the per-cell and per-z-bin values must be treated as PROVISIONAL until the response rebuild (N1+M2) and the budget closure (N2) land.

### C.5 Minimal fix list (priority order)

1. N1: harmonize gen-Q2 cell index to rad-safe `Q2_es_gen` on the pool/miss side in the cone/v4 response builders; rebuild response; re-run IBU k=4; re-release and quantify per-cell shifts.
2. M2: rebuild the response gen-z axis on the offline-computable rad-safe `z_rad` (validated formula) or issue a documented nominal-beam truth-contract decision carrying the 3.25% z-bin shift as a systematic.
3. N2: add the HFS-Empz channel (analytic Empz shift or envelope) to the systematic budget and re-freeze.
4. B1: quarantine or re-glob `scripts/submit_full_production_v6.py` (superseded `evls_highQ2_*` glob, central output base).
5. M1: correct builder/submitter provenance and header claims to the true HAT/calculator split; document the released branch basis; run the degenerate-event audit.
6. M5: hard-fail on EvlTree sidecar exhaustion.
7. B2/B3: fix the stale Stage-1 naming in `submit_stage2_v6_central.py`; remove the legacy union fallback in the canonical driver.
8. M4/m1: keep DisTot detached; mandate sidecar-sourced N_DIS for any multiplicity derivative.
9. M3/N3: one Condor quantification of the Tier-0 wrong-gen/wrong-charge link rate; add Tier-0 angle+exclusivity in the next builder revision.
10. m8 + B4: Pass-2 NaN isfinite guards; v6 manifest pins + wired binary guard.
11. Documentation sweep: m2/m3/m4/m6/m7, N5, stale v5g submitter docstrings.

## Missing evidence

- Full-sample (Condor) straddle and z-shift rates for N1/M2 (current quantification is one em06 slice of RAPGAP31 7071; the one-way direction, ISR fraction ~32%, and order of magnitude are robust; per-cell rates are slice-dependent).
- Post-rebuild per-cell delta table (N1/M2) — decides whether the re-release shifts exceed the quoted per-bin systematics.
- Tier-0 wrong-link / wrong-charge rate (M3/N3) — one Condor pass.
- HAT `ElecTheta`/`ElecPhi` vs calibrated (the M1 empirical check verified ElecE only).

## Recommended Next Check

Execute fix #1 on the full RAPGAP31 response sample via Condor — harmonize the gen-Q2 cell index, rebuild the response (switching the gen-z axis to the validated rad-safe `z_rad` in the same pass), and diff the per-cell IBU k=4 cross-sections against the released clean release. This single pass quantifies the N1 bias it removes, re-baselines M2, and produces the re-release candidate.

--- audit metadata ---
Auditor: H1 BaryonAsym physics-correctness critic, loop 3 of 3 (treebuilder final adjudication), read-only; no analysis script was modified. Report persisted by the delegating supervisor session (critic toolset read-only).
