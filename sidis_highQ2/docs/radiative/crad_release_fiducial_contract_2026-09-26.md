# Fiducial Phase Space and Observable Contract for Release-Compatible Gen-Only `C_rad`

**Status:** authoritative implementation contract for rebuilding a *candidate* gen-only QED correction for the central high-Q2 charged-hadron SIDIS v6/z005 release.  It does **not** approve application of that candidate: the central release remains raw/no-`C_rad`, and the documented luminosity, sparse-bin, and review gates remain open.

**Target release:**
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10/`

**Correction convention:** in every published charge/bin,

```text
C_rad = [sum_nonrad(weight1 * weight2) / L_nonrad]
        / [sum_rad(weight1 * weight2) / L_rad].
```

Apply precisely the same contract below to RAD and NONRAD. `L_data` is not used in this ratio.

## 1. Governing distinction: release truth phase space, not the response envelope

The central response builder deliberately keeps a loose truth *envelope*: `GenQ2>1`, `0.01<GenY<0.99`, finite track kinematics and `zGen`, pi/K/p, and a V0-daughter veto.  It deliberately removes the former truth cuts on final `Q2`, `y`, `GenEmpz`, `xF`, `GenEta`, and track `GenPt`; see `build_migration_th2_per_charge_v6.py` lines 6--50 and 380--470.  This is response coverage, not a `C_rad` definition.

The 490 released results are the 245 per-charge truth bins created by the release runner from the **gen** bin map (`run_bbb_ibu_release.py` lines 49--71).  The release runner contains no final truth-side `eta`, `xF`, or momentum cut; the metadata explicitly says “pT gen 5-bin reco-support variant; **gen unchanged**” (`metadata.json`, line 32).  Consequently the defensible Born measurement is the binned physics volume stated below, with the explicit DIS fiducial cuts that define the release selection.  It is **not** the loose response envelope and it is **not** a detector-geometry extrapolation truncated at generator `eta`.

The release chain does not serialize an explicit post-unfold truth mask.  Therefore the explicit `y` and `E-pz` requirements below follow the finalcandidate eSigma event selection and the response script's documented downstream analysis fiducial (`0.1<Y_es_gen<0.7`, `35<GenEmpz<70`; `build_migration...` lines 27--45 and constants at 172--204), rather than the envelope.  This is also required by the present user instruction: **`y` is mandatory in `C_rad`.**

## 2. Implementable Born-level fiducial selection

All numerical bounds are lower-inclusive/upper-exclusive unless a strict inequality is explicitly displayed.  Keep finite values only.  A candidate hadron enters exactly one table row only if it passes every applicable requirement and maps to a valid published bin.

### 2.1 Event-level DIS requirements

| quantity | required selection | required representation and rationale |
|---|---:|---|
| `Q2` | `150 <= Q2_es_gen < 20000 GeV^2` | Use the radiation-safe eSigma-labelled generator quantity `Q2_es_gen` (or an exactly validated equivalent reconstructed from generated lepton kinematics), **not** `Q2sGen`/`Q2sGen`-derived “Sigma” and not an unspecified hard-process `Q2`.  This matches the response's eSigma truth branch preference (lines 372--381 and 618--621) and the release Q2 strata. |
| `y` | `0.1 < Y_es_gen < 0.7` | Use `Y_es_gen`, not `YsGen`, `YesGen`, or true-generator `y` unless bitwise/equivalently validated.  The reco finalcandidate selector uses eSigma `Yes` with these strict bounds (lines 511--520), and the intended downstream truth fiducial recorded by the response source is the same range (lines 27--45).  The Q2/x-cell boundaries were engineered around this interval but rectangular cells alone do not constitute an adequate substitute. |
| `x` | one of the seven Q2/x cells in §4 | Use `x_es_gen`, consistently with `Q2_es_gen` and `Y_es_gen`.  Do not use legacy `XsGen` or `xGen`; radiative `xGen` is documented as unsafe in the v6 builder (source lines 557--563). |
| generated event `E-pz` | `35 < GenEmpz < 70 GeV` | **Apply it.**  This is the generator counterpart of the finalcandidate event containment/radiative window and is the analysis fiducial retained as downstream truth selection in the response source.  It is not the response envelope.  Do not use `35 <= ... <= 70` silently: the legacy C_rad worker uses strict bounds (though its tree builder records inclusive bounds in its flag). |
| vertex, trigger, reconstructed-electron cuts | no additional gen-only cut | These are reconstruction/event-list requirements, not a Born hadron fiducial in the released 245-bin truth map.  Do not import a reco-only alpha cone cut or a photon-theta veto. |

### 2.2 Hadron-level requirements

| quantity | required selection | contract decision |
|---|---:|---|
| species | `abs(GenPDG) in {211, 321, 2212}` | Charged pion, kaon, proton truth pool only; no leptons, hyperons, or PID measurement.  This is the central builder's gen-loop definition (source lines 1374--1406). |
| charge | `GenCharge>0` => h+; `GenCharge<0` => h- | Zero charge is invalid.  Use the signed charge, not PDG ordering.  Produce separate 245-row h+ and h- tables. |
| truth provenance | `IsGtr==1` and `Mother2==-1` | Mandatory.  They are enforced at central-tree fill time (source lines 1382--1406), but must be rechecked for a raw gen-only builder. |
| V0 / conversion / decay products | veto particles classified as V0 daughters, and veto conversion/Dalitz daughters when that provenance is available | The central response requires `IsV0DaughterGen==0` on the gen leg and `IsMatchedV0Daughter==0` on matched tracks (`build_migration...` lines 419--422 and 657--692).  A replacement gen-only tree must persist an equivalent ancestry classification; a `Mother2` veto alone is not a demonstrated substitute. |
| `ParticleSource` | preserve and audit it; **do not add a `ParticleSource!=4` phase-space veto** unless the response/release truth selection is rebuilt with the identical rule | The central v6 response does not cut `ParticleSource`; its truth definition is pi/K/p + GTR + `Mother2` + V0-daughter treatment.  The old RC builder writes `ParticleSource=-1` for every row (lines 339--342), so the existing worker's `!=4` condition is presently ineffective and cannot define compatibility. |
| lab pseudorapidity | **no truth `eta` cut** | Do **not** impose `abs(GenEta_lab)<1.6`, `<4`, `<3.5`, or any geometry proxy.  `|Eta|<1.6` is a reco release cut; metadata says gen unchanged.  The v6 response explicitly removed the gen-eta geometric cut (lines 27--39, 393--404).  Thus this is a Born truth measurement extrapolated through acceptance beyond reco geometry, as far as the bin definition permits. |
| current hemisphere | **no truth `xF` cut** | Do **not** impose `xFGen<0` (nor the opposite-sign external convention).  The v6 response explicitly removed that truth cut from its analysis selection (lines 27--39 and 396--404); it is absent from the release runner/bin map. |
| lab `pT`, `p`, and track-quality cuts | no additional truth threshold | Do not apply reco `Pt`, `P`, `NHitDedx`, `RadLength`, DCA, primary-track, or track-type cuts.  The published variable itself has `0 <= pT_h < 8 GeV` through the bin map. |
| `z` and `pT_h` bin support | `0 <= z < 1`, `0 <= pT_h < 8 GeV`, through §4 bin lookup | The seven z and five CM-frame `pT_h` bins define the hadron phase space.  `zlow005` is only a release-path tag: the first bin is `[0,0.05)`, **not** `z>=0.005`. |

## 3. Observable algorithms

### 3.1 Four-vectors and `z`

Let the nominal incoming beams be

```text
k_in = (0, 0, -E_e, E_e)
P    = (0, 0, +E_p, sqrt(E_p^2 + m_p^2))
k_out = generated scattered-electron four-vector
q = k_in - k_out
h = generated hadron four-vector
```

using the generated electron and the event's generated beam energies, with the central-script fallbacks `E_e=27.6 GeV`, `E_p=920 GeV` only when the branches are unavailable/invalid.  The release `z` is

```text
z = (P . h) / (P . q).
```

Use this `q` constructed from the **generated scattered electron**, not the reco electron and not a lab-axis proxy.  This is the invariant formula used by the migration code (`_compute_pTh_cm_and_z`, lines 253--256) and by the central builder's gen fill (`ChargedSidisTreeBuilderHighQ2_v6.C`, lines 1468--1470).  Use the hadron's physical generator four-vector/mass for `z`; reject non-finite or non-positive denominator results and then apply the bin range.

### 3.2 Exact release-compatible `pT_h`

`pT_h` is **not** `|p_h x qhat|` in the laboratory.  Implement the following literal translation of `build_migration_th2_per_charge_v6.py` lines 215--256.  Four-vectors use ordering `(px,py,pz,E)`.  The intentional pion-mass convention is retained here: the migration helper sets `E_h=sqrt(|p_h|^2+m_pi^2)` for every pi/K/p truth row.

```text
function boost_to_W_rest(a, W):
    # Boost vector is the negative W velocity: beta = -W_3/W_E.
    beta = (-W.px/W.E, -W.py/W.E, -W.pz/W.E)
    beta2 = min(dot(beta,beta), 1 - 1e-15)
    gamma = 1/sqrt(max(1-beta2, 1e-30))
    bp = dot(beta, a.spatial)
    gamma2 = (gamma-1)/dot(beta,beta) if dot(beta,beta)>0 else 0
    a_prime.spatial = a.spatial + gamma2*bp*beta + gamma*beta*a.E
    a_prime.E       = gamma*(a.E + bp)
    return a_prime

k_in = (0,0,-E_e,E_e)
P    = (0,0,+E_p,sqrt(E_p^2+m_p^2))
k_out = (GenElecPx, GenElecPy, GenElecPz, GenElecE)
q = k_in - k_out
W = q + P
h = (GenPx, GenPy, GenPz, sqrt(GenPx^2+GenPy^2+GenPz^2+m_pi^2))

h_cm = boost_to_W_rest(h, W)
q_cm = boost_to_W_rest(q, W)
if |q_cm.spatial| == 0: reject
qhat = q_cm.spatial / |q_cm.spatial|
h_perp = h_cm.spatial - dot(h_cm.spatial,qhat)*qhat
pT_h = |h_perp|
```

The boost sign is essential: `beta=-W_3/W_E`, i.e. boost both `h` and `q` **to the rest frame of `W=q+P`**.  A lab-frame transverse projection is noncompliant.

### 3.3 `phi_h`

`phi_h` is not a `C_rad` bin dimension and introduces no additional fiducial cut in this contract.  If stored for validation, compute Trento `phi_h` in the same gamma*p CM frame, using generated `k_in`, `k_out`, `q`, and CM transverse hadron momentum.  Do not use the obsolete lab-frame `safeTrentoPhi` currently in `RC_treebuilder_genonly_v1.C` lines 81--95.

## 4. Exact published bin map (245 per charge; 490 total)

### `(Q2_es_gen, x_es_gen)` cells

| cell | `Q2_es_gen` [GeV2] | `x_es_gen` |
|---|---:|---:|
| 0 `S0_x0` | `[150,300)` | `[0.0021,0.0071)` |
| 1 `S0_x1` | `[150,300)` | `[0.0071,0.030)` |
| 2 `S1_x0` | `[300,700)` | `[0.0042,0.016)` |
| 3 `S1_x1` | `[300,700)` | `[0.016,0.069)` |
| 4 `S2_x0` | `[700,1500)` | `[0.0099,0.034)` |
| 5 `S2_x1` | `[700,1500)` | `[0.034,0.150)` |
| 6 `S3_x0` | `[1500,20000)` | `[0.021,1.5)` |

The edges are from the release `bin_map.json` and release-compatible `binning_v3.py` lines 28--58.  The old `C_rad` cells are not a harmless approximation; they differ in S0 high-x, S1, and S2 boundaries.

```text
z edges       = [0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]  # 7 bins
pT_h edges GeV = [0, 0.5, 1, 2, 4, 8]                      # 5 bins
```

For zero-based local indices `cell in [0,6]`, `iz in [0,6]`, and `ipT in [0,4]`, use

```text
bin_index_per_charge = cell*35 + iz*5 + ipT
# 0 <= bin_index_per_charge < 245

release row / global_bin_index = bin_index_per_charge          for h+
                              = 245 + bin_index_per_charge    for h-
```

This is exactly `index_convention_gen` in the released `bin_map.json` (line 646) and the charge-major release runner convention (lines 61--70).  Table rows must be output in h+ indices `0..244`, then h- indices `0..244` (global `245..489`), never interleaved.

## 5. Required differences from the existing gen-only C_rad study

| existing location / behavior | deviation | required replacement |
|---|---|---|
| `RC_treebuilder_genonly_v1.C` lines 77--80 and 350 | `pT_h` is lab-axis `|p - (p.qhat)qhat|`. | Replace with the W-rest-frame algorithm in §3.2, including `beta=-W_3/W_E`, boosting both hadron and photon, and CM projection. |
| RC builder line 351 / `safeTrentoPhi` | lab-frame Trento-like construction. | Not needed for table filling.  If retained, calculate CM-frame Trento `phi_h`; do not use this value for a selection. |
| `full_crad_worker.py` lines 75--76 | bins on `Q2sGen`, `YsGen`, `XsGen`. | Use `Q2_es_gen`, `Y_es_gen`, `x_es_gen` consistently for event cut and binning.  Stop if these branches/equivalent validated values are absent. |
| worker lines 14--20 and `full_crad_finalize.py` | obsolete Q2/x cells: e.g. S0 high-x extends to `1.5`, S1 starts `0.0044`, S2 starts `0.0088`. | Replace with the seven release cells in §4 exactly. |
| RC builder line 317 / worker lines 76--80 | event flag uses eSigma-like variables, but histogrammer independently uses legacy Sigma variables; builder uses inclusive `GenEmpz` endpoints while worker uses strict endpoints. | Use one selection definition in the worker: `150<=Q2_es_gen<20000`, `0.1<Y_es_gen<0.7`, `35<GenEmpz<70`, and release x bins. |
| legacy code generally | no explicit release-compatible `y` method consistency. | `Y_es_gen` cut is mandatory in both RAD and NONRAD and must be recorded in sidecar metadata. |
| RC builder lines 357--359 / worker line 82 | applies `abs(GenEta_lab)<4` and `xFGen<0`. | Remove both.  Neither is a published truth fiducial in the central release; they are incompatible with “gen unchanged.” |
| RC builder line 358 / worker binning | stored `zGen` is fine, but the pT definition is wrong; worker only tests finiteness before binning. | Recompute/use validated release-compatible `z` and CM `pT_h`; then require the actual `[0,1) x [0,8)` bin support. |
| RC builder line 342 and worker lines 83--85 | source is hardcoded `-1`; worker's `ParticleSource!=4` test is therefore a no-op. | Persist real provenance.  Do not impose a source-4 veto unless central-response truth selection is changed/rebuilt identically; retain source composition diagnostics. |
| RC builder lines 354--360 | only `IsGtr`, `Mother2`, eta, z, xF; it lacks demonstrated V0/conv/Dalitz ancestry treatment. | Enforce `IsGtr==1`, `Mother2==-1`, and a response-equivalent V0-daughter/decay-product veto.  Block compatibility if ancestry cannot be supplied. |
| RC builder line 317 and worker line 80 | endpoint convention is inconsistent (`>= <=` versus `> <`). | Use strict `35<GenEmpz<70` throughout for the candidate contract. |
| RC builder `pBeam(0,0,E_p,E_p)` lines 325--328 | proton energy is approximated as `E_p`, whereas the migration helper uses `sqrt(E_p^2+m_p^2)`. | Use the exact helper convention for W/rest-frame `pT_h` and invariant `z`: `P.E=sqrt(E_p^2+m_p^2)`. |
| RC builder lines 308--314 | ISR-effective beam energy is used to form eSigma-labelled DIS variables, but `q` for hadron observables is separately constructed from the generated electron. | Retain this separation: use eSigma/radiation-safe variables for `(Q2,y,x)`; use `q=k_in-k_out` for the release hadron observables.  Document it in output metadata. |

## 6. Ambiguities, decisions, and required validation gates

1. **No serialized final truth mask in the release.**  The release runner only maps the 245 truth bins; it does not itself apply a post-unfold `Q2/y/Empz/xF/eta` mask.  **Default adopted here:** explicit eSigma `Q2`, `y`, and `GenEmpz` analysis cuts, plus the released Q2/x/z/pT bins; no truth eta/xF/lab-track cuts.  This follows the response source's stated downstream analysis selection, finalcandidate reco selection, and the user mandate to include `y`.  Before applying a correction, validate the rebuilt truth mask against the response's intended truth marginal and document any unavoidable historical response mismatch.

2. **Truth eta scope is decisive.**  The release metadata labels the reco cut `|Eta|<1.6` and says “gen unchanged”; the current v6 response source explicitly removes gen eta.  **Default:** no `GenEta` cut, not `|GenEta|<1.6`.  A later decision to publish a detector-fiducial hadron observable would require rebuilding/validating the response and recomputing `C_rad`; it cannot be patched by a correction-table selection.

3. **`xF` scope is likewise absent from the release truth map.**  Although older Option-C/legacy C_rad material uses `xFGen<0`, v6 envelope production explicitly removed it and the 490-bin release carries no xF field.  **Default:** no xF cut.  If an xF-current-hemisphere observable is desired, it is a distinct measurement and needs a matching response and `C_rad`.

4. **V0/decay provenance must be made real in the replacement RC tree.**  The old RC tree does not write a usable `ParticleSource`, V0-daughter, or conversion/Dalitz ancestry flag.  **Default:** block final compatibility until an H1OO-equivalent classifier reproduces the response's V0-daughter veto.  `IsGtr` and `Mother2` are necessary but not proven sufficient.

5. **Species-mass convention in `pT_h`.**  The exact migration helper uses `m_pi` to construct `h_lab` for the CM pT algorithm even for K/p.  **Default:** reproduce this literal convention for table compatibility.  Use the physical generated hadron four-vector for invariant `z`.  Any species-mass refinement is a new observable definition and must be response-revalidated.

6. **Q2 lower endpoint.**  The central response uses `>` for its broad guard and the finalcandidate reco selector uses `Q2es>150`, whereas the released bin-map arithmetic is `[150,300)`.  **Default:** assign `Q2_es_gen==150` to S0, consistent with the published bin map; this measure-zero convention is recorded explicitly.  If exact code-level parity is required, document the negligible boundary exclusion rather than changing an edge silently.

7. **`GenEmpz` endpoint convention.**  The historic builder flag uses inclusive endpoints while the histogram worker uses strict ones.  **Default:** strict `35<GenEmpz<70`, matching the historical worker and the documented downstream cut notation.  This must be identical for RAD/NONRAD.

8. **Generated-electron/beam validity.**  If a sample cannot provide generated scattered-electron four-vector and beam energies sufficient for the algorithm in §3.2, it cannot supply release-compatible `pT_h`.  **Default:** fail the candidate rather than substitute a lab pT or reco electron.

## Source record

Primary evidence read for this contract: release `metadata.json`, `bin_map.json`, and `_run/run_bbb_ibu_release.py`; canonical `unfolding/build_migration_th2_per_charge_v6.py` (especially lines 6--50, 172--256, 380--470, 505--707); v6 builder `ChargedSidisTreeBuilderHighQ2_v6.C` (especially lines 1374--1474); canonical `RUNLIST.md`; and `sidis_highQ2/docs/status/current-release.md` plus `docs/RUN_STATUS.md`.  The existing-study comparisons use canonical `radiative_corrections/RC_treebuilder_genonly_v1.C` and study-local `full_crad_worker.py`.
