# Script Audit 2026-10-19 — C_rad truth-contract adjudication: radiative correction inside the unfolding vs the applied C_rad (v6, high-Q2 charged SIDIS)

**Auditor:** physics-correctness critic, read-only. No analysis script modified.

**VERDICT: CONCERN** — no wholesale double-count exists: the unfolding performs no QED correction of its own, and the z/pT radiative migration is owned exactly once by C_rad (conventions verified identical on both sides, at line level, on the applied table's own archived worker). The single level inconsistency is on the Q2 axis and is exactly the already-documented N1 straddle, which interacts with C_rad's radiative-level Q2 binning to leave a percent-level, sign-definite, budget-uncovered per-cell bias in the released Born values (integrated ~ conserved; per-cell values already PROVISIONAL per loop-3). The current C_rad table is natively consistent with **Option A** (radiative-level truth everywhere, C_rad owns all QED). The loop-3 N1/M2 rebuild **as written** (Born-direction harmonization + `z_rad`) would *worsen* the Born-level Q2 error and break the currently-exact z consistency unless C_rad is co-re-derived on the Born basis in the same pass.

## Part A — what exactly is inside C_rad, and what is inside the unfolding

### A.0 Basis correction (provenance; no change to the physics verdict)

The releasebin and xFcut table generations **share CSV filenames** (documented trap: directory-qualified paths are mandatory). The applied primary since the 2026-10-09 releasebin promotion is the **releasebin** table (integrated h+ `1.1415172920054786` / h- `1.1410367377289135`; loop-2 pin sha prefix `cf810be4…`), **not** the same-named xFcut CSV in `djangoh14_crad_final_xfcut_2026-10-05/` (integrated h+ `1.1418534312056339` / h- `1.1413755529189475`; promotion-era sha `cab01f26…`), superseded-for-application since 2026-10-09 (A-F1: the xFcut tables bin the x-cell on framework `XesGen`; per-bin |dC_rad|/C_rad median 3.5049%, max 11.3712%, integrated cancels). Loop-2's applied-table census (35/490 sub-unity bins, min 0.8394 at bin 194, h+, cell 5, z[0.2,0.3), pT[4,8), cells 1/3/5/6) vs the xFcut twin's structure (h+ min 0.8404 at bin 229, cell 6, same z/pT corner) differ exactly by the A-F1 x-cell rederivation. The two generations differ **only** in the x-cell operand; binning Q2 operand, selection, z/pT constructions, weights, exposures, beam mixture, and grid are identical. **The level-consistency adjudication is therefore identical for both generations.** Corollary: the summary JSONs beside both table generations carry hardcoded stale labels (`"c_rad_applied": false`, `PROVISIONAL_DIAGNOSTIC`, xFcut-fork contract string — finalizer L59); live applied status is authoritative only in the promotion records/skill/STATE.

### A.1 The C_rad contract (verified on the applied table's own worker/finalizer)

- **Definition, per bin per charge:** `C_rad = sigma_nonrad/sigma_rad`, each leg `sigma = sum(weight1*weight2)/L_MC` (worker L118; finalizer L53, L69), beam-mixed by y in six uniform 0.1-wide bins (mixture ep 0.5141 / em 0.4859 from the per-leg exposures), per-bin `relative_stat_unc` carried (medians h+ 0.4272% / h- 0.4429%). Integrated value = sum(sigma_nonrad)/sum(sigma_rad) per charge over the 245 bins (finalizer L79).
- **Selection (both legs, identical):** rad-safe DIS fiducial `150 <= Q2_es_gen < 20000`, `0.1 < Y_es_gen < 0.7` (worker L120, L122; config metadata `"y_definition": "Y_es_gen, strict fiducial bounds"`, L143); `GenEmpz > 45` (L133; convention CLOSED 2026-10-09, release-matched truth contract); recomputed safeXF(q_nominal, stored GenE) < 0 current-hemisphere cut, stored `xFGen` branch not used (L129, L136, L61-75) — the shared nominal-beam-q bias (+0.039% integrated) deliberately retained so the C_rad fiducial and the release gen fiducial remain consistent (docstring L9-15; campaign check: 0 sign disagreements vs the stored branch, max |d| 8.4e-5); charged hadrons pi/K/p only, `IsGtr==1`, `Mother2==-1`, V0-daughter/ancestor vetoes (L125); RAD leg additionally carries the `Q2Gki>100` domain cut (~0.09% in-fiducial, rad-only; L115, L119).
- **Binning:** `bindex(Q2esGen, x_es_gen, z, pT)` — 7 (Q2,x) cells x 7 z x 5 pT = 245 bins/charge, h- offset +245 (L121, L140, L32-35, L83-89, L141) — identical to the response gen grid (`binning_v3.py`) and to the 490-row CSV layout consumed by the one-to-one `global_bin_index` merge (loop-2 verified, charge-label checked).
- **z/pT (released "nominal" variant):** q = nominal-beam electron minus gen scattered electron (`q_nominal`, L76-79); z = (P.h)/(P.q) with massive proton (M_P = 0.93827) and hadron at stored GenE; pT = |p_perp qhat| in the W = q+P CM frame with pion-mass hadron helper (M_PI = 0.13957) (L50-60, L138). These are **radiative-level (nominal-beam) observables**.
- **Level classification of the binning operands — the Part A decisive answer:**
  - **Q2 axis: radiative-level on the RAD leg.** `Q2esGen` is the eSigma calculator accessor evaluated with nominal beam energies; under ISR it exceeds the photon-subtracted rad-safe `Q2_es_gen` 100% one-directionally (loop-3 N1 empirical finding). On the NONRAD leg there is no radiation, so `Q2esGen` == Born there. `C_rad(k)` is therefore `sigma_Born(k) / sigma_rad_radiatively-binned(k)`: **it corrects radiative-level cell content to Born-level cell content.**
  - **x axis:** rad-safe (`x_es_gen`) on both legs.
  - **z/pT:** radiative-level (nominal-beam q) on the RAD leg; on the NONRAD leg nominal-beam = Born.
  - **Selection:** rad-safe (Born-definition fiducial) on both legs.
  - The table's truth contract is thus a **hybrid**: Born-defined fiducial; radiative-level Q2/z/pT transport coordinates; rad-safe x — precisely the "release-safe (Q2esGen, x_es_gen)" convention of the campaign manifest and skill.

### A.2 The response/unfolding side (established loop-2/loop-3 + this chain's builder verification)

- Gen pool/miss binning: `(Q2esGen, x_es_gen)` (cone builder 1001-1031) — **identical operands to the C_rad legs**.
- Matched body binning: `(Q2_es_gen, x_es_gen)` (1347-1407) — **the N1 defect**: Born-level Q2 on the body only.
- Gen fiducial: rad-safe `Q2_es_gen/Y_es_gen` + `GenEmpz>45` (EVENT_CUTS_GEN Empz_lo=45.0) + `xFGen<0` — **identical to the C_rad legs' selection** (xF sign-identical to the worker's recomputed safeXF, 0 sign disagreements).
- Gen z: stored `zGen` (nominal-beam safeZ; differs from the worker's massive-proton z only in the proton-mass treatment, ~1e-6 relative — negligible). Gen pT: `_compute_pth_cm_and_z` (CM frame, pion-mass hadron, massive proton, nominal-beam q) — **exact match to the worker's pT construction**.
- Gen grid: `binning_v3.py` = worker CELLS exactly. Released IBU nominal response = RAPGAP31 cone+S2' variant; DJ14 response = model-diagnostic branch. No QED reweighting or photon correction exists anywhere in the response build.

### A.3 Answer to the delegated question

**The unfolding does not perform a radiative correction.** It is a detector-migration operator that transports radiative MC events at hybrid truth level; the only QED "content" inside it is which events exist, not any correction to them. Consequently: (i) there is **no double-count** on z/pT — the response books gen z/pT with the identical nominal-beam constructions the C_rad worker uses, so the C_rad z-gradient and pT-tail corrections are applied exactly once; (ii) the only level inconsistency in the released scheme is the **Q2 axis**, where the response *body* is Born-binned while the C_rad denominator is radiatively binned — precisely the already-documented N1 straddle, extended to its Born-level consequence (Part B.1); (iii) the response-vs-C_rad generator difference (RAPGAP31 response, DJANGOH14 C_rad) is the budgeted `c_rad_gen_rel` modeling term (emfix-pair agreement +0.6419%/+0.6499% integrated; medians 0.5844%/0.5804%), not a level inconsistency — present identically under both options.

## Part B — per-axis double-count / gap adjudication

### B.1 Q2 axis — partial GAP (N1 straddle x C_rad convention), no double-count

- **Mechanism.** Under ISR, `Q2esGen > Q2_es_gen` 100% one-directionally, so radiative gen rows straddle upward across cell boundaries. The response books them radiatively in the pool/miss but at Born level in the matched body (N1); the C_rad denominator books them radiatively. Because the released scheme computes `Born(k) = C_rad(k) x IBU(k)` with `C_rad(k) = sigma_Born(k)/sigma_rad_radiol(k)` while the IBU output is anchored on the Born-binned body, the product carries a residual factor `E(k) = N_B(k)/N_R(k)` per cell (Born-binned vs radiologically binned rad content).
- **Signs (two levels, opposite directions).** *Raw level* (loop-3 C.2): upper straddle cells' unfolded values biased HIGH, lower cells biased LOW (efficiency-split mechanism), magnitude a diluted fraction of the per-cell straddle rate. *Born level* (this adjudication): the C_rad-convention residual has the **opposite sign** — net-depleted (lower) cells are over-corrected (biased HIGH; `N_R < N_B` -> C_rad larger than the Born-basis value), net-inflated (upper) cells under-corrected (biased LOW). The released Born per-cell values carry the **net** of the two: percent-level, sign-definite per cell, partially self-cancelling.
- **Magnitude bound (not recomputed).** Bounded by the matched fraction `f_matched` x per-cell straddle rate. Per-cell rates on the quantified slice: S2_x0 8.74%, S3_x0 6.21%, S1_x0 2.76%, S1_x1 2.34%, S2_x1 1.84%, S0 0%; overall 1.94% of builder-fiducial gen rows (weighted 1.57%). **Integrated ~ conserved** (redistribution-shaped).
- **Coverage:** not covered by the quoted budget (common-mode structural defect; generator-model spread blind to it). Physics content of the CONCERN verdict; already captured by loop-3's PROVISIONAL ruling on per-cell values.

### B.2 z axis — CLEAN at the contract level; M2 is a bin-definition question contract-coupled to C_rad

- Response gen z (`zGen`, nominal-beam safeZ) and C_rad z (worker `observables` under `q_nominal`) are the **same convention** -> the per-bin C_rad z-gradient (~1.05 at lowest z rising to ~1.4-1.57 at high z) is the ISR-induced downward z-shift at nominal beams, **undone exactly once** by C_rad. No double-count, no gap.
- Loop-3's M2 (3.25% of fiducial gen rows change z bin, 100% upward toward rad-safe z; dominant 0->1: 465, 1->2: 289; concentrated in z<0.05 and 0.05-0.1) is the **same physics quantified as bin displacement**. Resolution is contract-coupled: of loop-3's two fix branches, only "documented nominal-beam truth-contract decision + z-shift systematic" is consistent with the current C_rad table. The `z_rad` rebuild branch **requires** C_rad re-derivation at Born z — otherwise the ~+5%->+57% z-gradient would be applied to already-Born-binned z content, converting the currently clean z axis into a z-migration **double-count** at the 3.25%-row-shift strength.

### B.3 pT axis — CLEAN

Response gen pT and C_rad pT use the exact same construction (CM frame of W = q+P, pion-mass hadron, massive proton, nominal-beam q). The sub-unity high-pT corrections (35/490 bins < 1, min 0.8394, bin 194, h+, cell 5, z[0.2,0.3), pT[4,8); all high-pT 2-8 GeV tails of cells 1/3/5/6; h+/h- symmetric) are radiative migration into the tails at radiative-level q, **owned exactly once** by C_rad. The RG31-emfix alternative having zero sub-unity bins confirms the tail behavior is generator-specific, captured through `c_rad_gen_rel`/`c_rad_stat_rel`.

### B.4 Decomposition of the integrated C_rad (1.14152 / 1.14104)

The integrated value measures the **net differential depletion of the RAD leg under the shared truth-level fiducial**; intra-grid migration cancels in the sum by construction and appears only in the per-bin shape. Approximate multiplicative decomposition (marginal sensitivities; factors not exactly independent):
- DIS Q2/y boundary + rad-safe-estimator differential: **~ +10.6%** (the no-Empz-cut integrated value is 1.10597, so this term survives without any Empz bite);
- GenEmpz>45 window: **x ~1.032** (nocut 1.10597 -> nominal 1.14152 -> >50 GeV 1.17160; convention CLOSED 2026-10-09);
- xF<0 hemisphere cut: **x ~1.004** (xF-free -0.373%/-0.413% integrated, releasebin addendum);
- rad-only Q2Gki>100: **x ~1.001** (~0.09%).

## Part C — Option A vs Option B adjudication

### C.1 Native consistency of the current C_rad table

**The current releasebin C_rad table is natively consistent with Option A (radiative-level truth everywhere; C_rad owns all QED).** Its Q2 binning operand is the calculator `Q2esGen` (radiative-level), its x is rad-safe, its z/pT are nominal-beam, and its selection is the rad-safe fiducial shared with the response pool. Under Option A the factorization `Born(k) = C_rad(k) x IBU(k)` is **exact** (up to the budgeted `c_rad_gen_rel` generator term) once the response body is harmonized to the pool/miss/C_rad convention — i.e., **the response should book gen truth exactly as the C_rad worker books its legs**: same selection, same binning operands, same z/pT constructions. Option B is *not* natively supported: it requires re-deriving C_rad with both legs binned at `Q2_es_gen` and z/pT at photon-subtracted (Born) q.

### C.2 Bounded migration of released numbers under each option (bounds only)

- **Option A (body -> Q2esGen, keep the verified table):** released per-cell Born values move by the removal of the N1 net bias — percent-level on the five affected cells (bounded by f_matched x straddle rate, slice upper bounds 1.84-8.74%), ~ conserved integrated; z/pT bin values shift only via second-order cell-harmonization side effects; the raw product becomes internally consistent in the same pass.
- **Loop-3 N1/M2 fix as written (pool/miss -> Q2_es_gen, `z_rad`, keep the current table):** the raw product becomes consistent, but the Born-level Q2 residual becomes the **full net straddle flux per cell** (undiluted, no longer partially cancelled) — the Born per-cell error **grows**; and the z axis acquires a migration double-count at the 3.25%-row-shift strength (B.2). Central warning: **the rebuild direction and the C_rad contract must be co-designed.**
- **Option B done fully (Born response + Born-basis C_rad'):** exact end state; released numbers move by the same bias-removal magnitudes as Option A, but the final numbers live in Born-coordinate bins and per-bin C_rad' differs from C_rad by the same straddle/z-shift fractions on the correction side.
- All three: integrated ~ conserved (redistribution-level).

### C.3 Recommendation: **Option A**

1. **Physics cleanliness:** exact factorization with the already-promoted, gate-validated table; the response stays a pure detector-migration operator at the level the detector actually measures (radiative-level reco <-> radiative-level gen: tighter gen-reco correlation, no QED straddle injected into the matrix); all QED owned by the single DJANGOH14-derived correction with the RAPGAP31-emfix systematic.
2. **Re-derivation cost:** zero new C_rad production. The response rebuild is a one-operand harmonization (body bq: `Q2_es_gen` -> `Q2esGen`) + IBU k=4 re-run. Option B requires worker changes (`REQUIRED += GenEnPhoton`; photon-subtracted q variant), both-leg Condor re-runs (DJ14 2953 + RG31 1777 trees), re-validation, and a new promotion chain.
3. **H1 precedent:** post-unfold, gen-level, per-bin radiative correction on the unfolded hadron-level distribution is the released scheme's own promoted definition and the standard H1 SIDIS convention; the 2026-10-09 "release-matched truth contract" decisions already anchor C_rad's fiducial to the response-pool side — Option A extends the identical principle to the Q2 binning axis.
4. **Consistency with the verdict chain:** loop-3's N1 ACTION is fully compatible; only the harmonization *direction* (C.4 action 1) and the `z_rad` switch (action 2) need amendment.

### C.4 Required actions (Option A path)

1. **Implement the N1 harmonization in the Option-A direction:** response BODY gen-Q2 cell index -> `Q2esGen` (retaining the pool/miss convention and the rad-safe fiducial), making the response an exact mirror of the C_rad worker's booking. Do **not** harmonize pool/miss to `Q2_es_gen` while keeping the current table.
2. **Drop or defer the gen-z -> `z_rad` switch**; keep `zGen` and nominal-beam pT so the z/pT axes stay exactly matched to C_rad. Resolve M2 via a documented nominal-beam truth-contract decision carrying the 3.25% z-bin shift as a systematic.
3. Rebuild the response, re-run IBU k=4, re-release per-cell values; quantify per-cell shifts vs the current release.
4. Add a one-paragraph **truth-contract statement** to the radiative docs: C_rad is derived and applied at `(Q2esGen, x_es_gen)` cell binning with the rad-safe selection (`Q2_es_gen/Y_es_gen/x_es_gen` + `GenEmpz>45` + safeXF(q_nom)<0) and nominal-beam z/pT; response truth must use the identical binning/definitions (cite worker L120-121, L128, L138, L147).
5. **Provenance hygiene:** annotate the releasebin/xFcut summary JSONs' hardcoded stale labels as superseded by the promotion records; enforce directory-qualified C_rad quotes.
6. **If Option B is ever chosen instead:** re-derive C_rad on the Born basis *before* any Born re-release from a `z_rad`/`Q2_es_gen` response — the two must land in the same pass.
7. (Documentation) Reconcile the skill's selection-summary wording (`150 < Q2sGen < 20000`, `0.1 < YsGen < 0.7`) with the worker's actual rad-safe fiducial (`Q2_es_gen/Y_es_gen`, worker L120/L122/L143) — `Q2sGen/YsGen` are used only in the `sigma_photon` q-variant (L128); the worker's own metadata is correct.

## Major Concerns

- **MC-1 (Q2-axis level inconsistency = N1 x C_rad convention).** The released Born per-cell values are not level-consistent: body `(Q2_es_gen)` vs C_rad denominator `(Q2esGen)` binning (worker L121; cone builder 1001-1031 vs 1347-1407); loop-3 straddle rates and raw-level sign ruling; this adjudication's Born-level residual (B.1). Percent-level, sign-definite per cell, integrated ~ conserved, not covered by the budget. Follow-up-blocking for per-cell quotability; not analysis-killing.
- **MC-2 (rebuild-without-co-rederivation worsening).** Loop-3's fix #1/#2 as written (Born-direction harmonization + `z_rad`) would *worsen* the released Born values: the Born-level Q2 residual becomes the full undiluted net straddle flux and the z axis acquires a 3.25%-strength double-count. The response truth level must match the C_rad denominator level; harmonizing the response away from it while keeping the table widens the mismatch. Adopt the Option-A direction (C.4 actions 1-2) or co-derive the Born-basis table (action 6).

## Non-Blocking Checks Confirmed

- **z/pT conventions identical** between response and C_rad (worker `observables` L50-60/L138 vs `_compute_pth_cm_and_z`; both nominal-beam q, pion-mass CM pT, massive-proton z; zGen safeZ difference ~1e-6) — dominant radiative migration components owned exactly once; no double-count.
- **Fiducials identical**: rad-safe DIS (L120/L122 = EVENT_CUTS_GEN), `GenEmpz>45` both sides (convention CLOSED, release-matched), safeXF(q_nom)<0 ~ xFGen<0 (0 sign disagreements, max |d| 8.4e-5; shared +0.039% bias by design).
- **Grid and merge identity**: worker CELLS/ZED/PTED (L32-35) = `binning_v3` gen grid; 245/charge with +245 h- offset (L141) = 490-row CSV = loop-2's one-to-one `global_bin_index` merge with charge-label check.
- **Ratio direction, weights, mixing**: `C_rad = sigma_nonrad/sigma_rad` (finalizer L53/L69); weights `weight1*weight2` (L118); beam-mixed by y (L41/L66); mixed-beam-only validity trap carried; `L_data` absent from the gen-only ratio.
- **Gen truth discipline**: pi/K/p only, `IsGtr==1`, `Mother2==-1`, V0/ancestor vetoes on both legs (L125).
- **No QED correction inside the unfolding** (no radiative reweighting in the response build); RG31-vs-DJ14 generator difference is the budgeted `c_rad_gen_rel`, not a level inconsistency.
- **Verdict robustness**: the xFcut -> releasebin table change (A-F1) is a value-level x-cell rederivation only; both generations share the identical truth-level contract.

## Missing Evidence

- Full-sample (Condor) straddle and z-shift rates (current quantification is one em06 slice of RAPGAP31 7071) — decides the definitive per-cell MC-1 magnitudes.
- The matched fraction `f_matched` for the dilution bound (same Condor pass).
- Post-rebuild per-cell delta table (loop-3 pending) — decides whether re-release shifts exceed quoted per-bin systematics.
- A direct full per-bin structural characterization of the **applied releasebin table** (z-gradient shape on the applied basis).

## Recommended Next Check

Amend loop-3's rebuild pass to the Option-A direction and execute it: **rebuild the full RAPGAP31 response via Condor with the matched body harmonized to `Q2esGen` (pool/miss convention retained) and the z/pT axes kept at `zGen`/nominal-beam pT, then diff per-cell IBU k=4 against the released clean release.** This single pass quantifies the N1 bias it removes, keeps the C_rad contract exact with the verified applied table, and produces the re-release candidate.

--- audit metadata ---
- Auditor: H1 BaryonAsym physics-correctness critic (C_rad truth-contract adjudication), read-only; no analysis script was modified.
- User decision 2026-10-19: Option A adopted.
- Report persisted by the delegating supervisor session (critic toolset read-only).
