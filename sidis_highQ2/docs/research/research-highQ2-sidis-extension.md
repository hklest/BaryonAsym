<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note. High-Q2 builder parity is now updated and the high-Q2 merge on `/pnfs` is complete; keep this document for scope and publication-strategy guidance.

> **Editorial note (2026-05-19):** earlier drafts of this document referenced
> the NAF package as `H1ProtonAsymHighQ2/`. The actual NAF path is
> `H1SIDIS_HighQ2/` — the package was renamed during the proton-fork-to-SIDIS
> transition. References below have been updated 2026-05-19. The builder
> binary and source file names (`ProtonTreeBuilderHighQ2`,
> `ProtonTreeBuilderHighQ2.C`) retain their legacy proton-prefixed names;
> see `docs/references/reference-naming-aliases-highQ2-sidis.md` for the
> full alias table.

# Research: Extending the H1 Charged-Hadron SIDIS Analysis to High Q² (Q² > 150 GeV²)

**Date:** 2026-05-14
**Audience:** SIDIS supervisor and TMD planner deciding whether and how
to add the high-Q² regime to the existing low-Q² (5 < Q² < 100 GeV²)
charged-hadron SIDIS program.
**Scope:** Requirements, gaps, blockers, and effort estimate for moving
the high-Q² SIDIS workflow to feature parity with the remediated low-Q²
workflow, and for combining both regimes into one TMD-fit-grade dataset.

> This is research, not a plan. It catalogues what exists, what is
> missing, and what would have to be built. The decision of whether to
> pursue this as a single combined paper, a companion paper, or a
> separate analysis is left to the SIDIS supervisor.

## Inputs Used

- `H1/sidis_lowQ2/context-handoff.md` (low-Q² status, F1-F13 ledger)
- `H1/sidis_lowQ2/plan-tmd-sidis.md` Iteration 2 (Part 6 high-Q² scope)
- `H1/sidis_highQ2/source_cache/ProtonTreeBuilderHighQ2.C`
- `H1/sidis_lowQ2/research-sidis-systematics-framework.md` Section 3.6
  (low/high-Q² differences table)
- `H1/sidis_lowQ2/reference-sidis-systematics.md` (LAr vs SpaCal source ownership)
- `H1/sidis_lowQ2/critic-sidis-systematics-distilled.md`
  (LAr vs SpaCal numerical-transfer rule)
- `H1/CONTEXT.md` (luminosities, NAF infrastructure)
- `H1/proton_HiQ/context-handoff.md` and
  `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md`
  (high-Q² production state, what physically ran, blockers)
- `H1/proton_HiQ/docs/research-data-coverage-gap.md`
  (root cause of e⁻ data gap, recovery path)
- `H1/proton_HiQ/docs/research-mc-normalization-highQ2.md`
  (H1CalcWeight blocker for absolute normalization)
- `H1/proton_HiQ/docs/research-highQ2-proton-asymmetry.md`
  (LAr fiducial-volume cuts, S67 trigger, FidVolCut layers)
- `H1/lambda_HiQ/context-handoff.md` (parallel high-Q² infrastructure)
- Earlier-reported v2/v3 high-Q² smoke MC inspection summary
  (provided in the request)

---

## 1. Kinematic Coverage at High Q²

### 1.1 Q² and y window

| Cut | High-Q² value | Low-Q² value | Notes |
|-----|---------------|--------------|-------|
| Q² range | 150 -- ~20,000 GeV² | 5 -- 100 GeV² | factor ~30 in Q²_min |
| y range | 0.1 -- 0.9 (some refs 0.05 lower) | 0.1 -- 0.6 | y_max wider at high-Q² |
| E - p_z | [45, 65] GeV | [35, 70] GeV | tighter at high-Q² (anti-photoprod / QEDC) |
| |VtxZ| | < 35 cm | < 30 cm | slightly looser at high-Q² |
| Trigger | S67 (LAr NC) | S61 (SpaCal DIS) | physically different chain |
| Electron | LAr calorimeter | SpaCal | central vs backward |
| sqrt(s) | 319 GeV (HERA-II) | 319 GeV | identical |

Sources: `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` Section 9;
`H1/lambda_HiQ/context-handoff.md` Section 2;
`ProtonTreeBuilderHighQ2.C` header lines 16-23.

### 1.2 Bjorken-x reach

With `s ≈ 4·27.6·920 ≈ 101,568 GeV²` and `x = Q² / (s · y)`:

| Q² [GeV²] | y_min = 0.1 | y_max = 0.9 (or 0.7) |
|-----------|-------------|----------------------|
| 150 | x ≈ 1.5e-2 | x ≈ 1.6e-3 (y=0.9) |
| 300 | x ≈ 3.0e-2 | x ≈ 3.3e-3 |
| 700 | x ≈ 6.9e-2 | x ≈ 7.7e-3 |
| 2,000 | x ≈ 0.20 | x ≈ 0.022 |
| 5,000 | x ≈ 0.49 | x ≈ 0.055 |
| 10,000 | x ≈ 0.98 (kin. limit) | x ≈ 0.11 |

Note the example numbers in the task prompt (`Q²=150 → x ∈ [0.002, 0.03]`,
`Q²=10000 → x ∈ [0.1, 2]`) are consistent with this formula given a slightly
different y range. The kinematic limit `x ≤ 1` excludes the upper-right
corner.

The high-Q² regime is therefore predominantly **valence-quark territory**:
typical x ~ 0.01 -- 0.3, well above the low-Q² access of ~10⁻⁴ -- 10⁻². The
two regimes are kinematically **complementary**, not overlapping. A small
overlap exists only in the strip `Q² ~ 100-300 GeV², y ~ 0.5-0.9,
x ~ 10⁻³ -- 10⁻²`, but the trigger and electron detector differ across that
strip so the overlap is conceptual rather than directly diagnostic.

### 1.3 Kinematic complementarity table

| Regime | Q² | x | Dominant physics | Quark sensitivity |
|--------|-----|------|-----------------|-------------------|
| Low-Q² (current) | 5-100 GeV² | ~1e-4 -- 1e-2 | Sea + small-x evolution | Sea quarks, gluon-driven |
| High-Q² (extension) | 150-2e4 GeV² | ~1e-3 -- ~0.5 | Valence + DGLAP regime | Valence u/d, charge asymmetric |

This is precisely the **TMD-evolution lever arm** advertised in
`plan-tmd-sidis.md` Section 7.1.

### 1.4 Hadron pT_h reach

Track-level cuts from `ProtonTreeBuilderHighQ2.C` (lines 38-43):
- pT > 0.1 GeV (tree level), 0.15 GeV (analysis level)
- |eta| < 1.5 (tree), 1.3 (analysis)
- P < 3.0 GeV (tree)
- These are identical to low-Q².

The relevant TMD criterion is pT_h/Q. At Q ≈ √Q² ≈ 12 GeV (Q²=150) or
Q ≈ 50 GeV (Q²=2500), the existing pT_h ≤ 2 GeV proposed range gives
pT_h/Q ≪ 1 across most of the high-Q² grid -- much safer for TMD
factorization than at low Q² where pT_h ~ Q is easily reached. **The
TMD-safe phase space is much larger at high Q² than at low Q²**, even
though the per-bin statistics are smaller.

### 1.5 Scattered-electron reach

At high Q² the scattered electron occupies the LAr `4° < θ < 154°` window
(see `research-highQ2-proton-asymmetry.md` Section 5.x). This is central-to-forward
in the H1 frame, where the LAr energy resolution and absolute scale
behave very differently from the backward SpaCal:
- LAr energy resolution: σ(E)/E ≈ 50%/√E + 2% (electromagnetic section).
- Position-dependent scale uncertainty: 1-5% across z-impact regions
  (per `H1CalcSystematic` semantics noted in
  `research-tracking-systematics-deep.md` and the task prompt).
- SpaCal energy: ~1% scale, ~1 mrad angle.

This drives the systematics differences in Section 4 below.

---

## 2. Available Data and MC

### 2.1 Production state of the high-Q² package

The high-Q² SIDIS extension is NOT a new build. The package
`H1SIDIS_HighQ2/` (legacy name `H1ProtonAsymHighQ2/`; renamed during the
proton-fork-to-SIDIS transition — see
`docs/references/reference-naming-aliases-highQ2-sidis.md`) already exists,
has been compiled, and produced output files. Earlier (v4) production was
driven by the proton/antiproton analysis and wrote both
`ProtonTot`/`ProtonTotGen` and `ChargedTot`/`ChargedTotGen` trees in the
same binary (`ProtonTreeBuilderHighQ2`). **v5b removed `ProtonTot` and
`ProtonTotGen`**; the production is now SIDIS-only, with the charged trees
as the primary product and the legacy proton trees absent.

| Item | Value | Source |
|------|-------|--------|
| Package | `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/` | proton_HiQ handoff §2 |
| Binary | `bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` | proton_HiQ handoff §2 |
| Builder | `src/ProtonTreeBuilderHighQ2.C` (991 lines) | source_cache |
| Constants | `src/PhysicsConstants.h` (high-Q² fork) | proton_HiQ handoff §7 |
| Data files (current) | 635 (ep0607 only) | proton_HiQ handoff §3 |
| MC RAPGAP31 | 360 files, 56.7M reco, 36.9M gen | proton_HiQ handoff §3 |
| MC DJANGOH14 | 486 files merged, not yet analyzed for SIDIS | proton_HiQ handoff §3 |

### 2.2 Luminosity status -- the e⁻ recovery story

Confirmed by `proton_HiQ/docs/research-data-coverage-gap.md`:

| Period | Beam | S67 lumi (nb⁻¹) | Currently processed | Recoverable |
|--------|------|-----------------|---------------------|-------------|
| ep0304 | e+ | 53,706 | NO (missing /03/ + wrong evls in /04/) | YES |
| em0405 | e- | 105,028 | NO (wrong good-run file) | YES |
| em06 | e- | 57,573 | NO (split-/06/ bug) | YES |
| ep0607 | e+ | 140,306 | YES | already in |
| **Total recoverable** | | **351,000.0 nb⁻¹** (current canonical quote; older direct-monitor total 356,613 nb⁻¹ is superseded) | **140,306 (39%)** | **+216,307 (61%)** |

**Critical fact:** the e⁻ data physically exists on /pnfs/ and the correct
event lists already exist in `H1BaryonAsym_HighQ2/output/evls/data/` --
they were produced by the lambda_HiQ pipeline. The v5b SIDIS pipeline
(which shares production infrastructure with the proton/antiproton analysis)
simply pointed at the wrong evls files. Recovery does NOT require
re-running event-list construction; it requires:
1. Adding `/03/` to the `DATA_PERIODS` map in `submit_full_production.py`.
2. Splitting `/06/` into the `em06` and `ep0607` sub-ranges
   (chunks c060220-c060319 vs c060000-c060212).
3. Re-pointing the SIDIS steering files (legacy directory `steering/proton/`)
   at the period-tagged evls names.
4. Re-running the SIDIS tree-building stage (Stage 2; 1 Condor pass on
   ~700 additional files).

This is mostly a configuration bug, not a missing-data problem.

### 2.3 What exists in the high-Q² ChargedTot trees

Per the v2 smoke inspection (provided in the request) and confirmed by
the builder source:

| Tree | Branch count (current) | Branch count (low-Q² target) | Gap |
|------|------------------------|-------------------------------|-----|
| ChargedTot | 64 | 82 | -18 branches |
| ChargedTotGen | 22 (v2) / 28 (v3) | 35 | -13 (v2) / -7 (v3) |

The known gaps in the high-Q² ChargedTot tree relative to the low-Q²
remediated baseline:
- All non-eSigma DIS kinematic methods: `Q2e/Ye/Xe`, `Q2da/Yda/Xda`,
  `Q2es/Yes/Xes` (note: only `Q2s/Ys/xBj` are stored), `Q2h/Yh/Xh`.
  See builder lines 188-204 -- only `Q2s, Ys` are read from HAT; `xBj`
  is computed from them.
- eSigma-frame SIDIS variables: `ystar_es, yBreit_es, xF_SIDIS_es,
  z_SIDIS_es`. The high-Q² SIDIS variables are computed once in the
  electron-vs-Sigma hybrid frame used by the proton analysis (builder
  lines 538-548 use the e⁻-only virtual photon).
- `MatchAngle` is present (line 348).
- `ParticleSource_cg` (gen-level source classification): NOT present in
  ChargedTotGen of the high-Q² builder. The gen loop fills `IsGtr_cg`
  but does not store a per-gen source classification (lines 672-740).
- `inFiducialCharged` definition: in the high-Q² builder this uses the
  analysis-level cuts (`|GenEta| < 1.3 && GenPt > 0.15`, line 718), NOT
  the broad gen fiducial (`|GenEta| < 3.5, GenPt > 0.05`) adopted in the
  remediated low-Q² builder.

### 2.4 What is already correct

The high-Q² builder, per its own header comments (lines 54-60), already
has charged-particle "Fix 1-5":
- Fix 1: matched gen SIDIS variables on ChargedTot (`Q2sGen, YsGen,
  xGen, ystarGen, yBreitGen, xFGen, zGen`).
- Fix 2: exclusive one-to-one matching (`usedGenIndices_ch` set).
- Fix 3: `isChargedHadron()` PDG restriction (π, K, p only).
- Fix 4: charged gen SIDIS uses the same boost vectors as the proton gen
  loop.
- Fix 5: `ParticleSource_ch` defaults to -1 (sentinel).

This is consistent with the `context-handoff.md` claim that the high-Q²
builder was the "gold reference" for low-Q² charged-particle fixes.
Where the high-Q² builder is now BEHIND is the **DIS-method expansion**
and **SIDIS-method consistency** work that happened on low-Q² after the
charged-particle backport.

### 2.5 Bounded defects from the v2 inspection

Per the smoke-MC summary in the task prompt:
- F3/F6/F7 charge-mismatch leak: 0.76% (bounded but present).
- Gen-fiducial leak: 1.24%.

These numbers are small enough to publish charge asymmetries as
preliminary, but they would need to be closed for publication-grade
multiplicities at the precision targeted by TMD fits.

### 2.6 NonRad MC

NonRad DJANGOH14 is missing for both regimes. This blocks publication-grade
`C_rad` everywhere, not specifically a high-Q² problem. Per
`context-handoff-highQ2-proton.md` Section 6, `C_rad = 1` is the current
placeholder.

---

## 3. Builder Adaptation: What Needs to Change

### 3.1 Code-distance from low-Q² remediated builder

The high-Q² builder is **not** a clean port target for the entire low-Q²
builder. It was forked first and is now a sibling, not a descendant.
However, the diff is bounded.

Differences that must be preserved:
1. **DIS selection is upstream.** The high-Q² builder reads pre-selected
   evls files from `H1BaryonAsym_HighQ2`. It does NOT contain S67 trigger,
   FidVolCut, or LAr electron ID code. The low-Q² builder operates on
   directly-streamed events with embedded S61/SpaCal selection. Any port
   must NOT bring along low-Q² DIS selection.
2. **Hadronic calibration is upstream.** Low-Q² calls
   `CalibrateHadrooII` on the fly; high-Q² inherits it from the evls
   stage and explicitly skips it (builder lines 175-178).
3. **HAT pointer set is narrower.** High-Q² reads only `Q2s, Ys`
   because that is what the upstream stage stores after eSigma
   calibration. To add `Q2e/Q2da/Q2es/Q2h/...` the upstream evls stage
   may also need to write them, or the builder must compute them on the
   fly from primitives (`scatEX/Y/Z/E`, `Empz`, `BeamElecE`, `BeamProtP`,
   the HFS sum).
4. **Fiducial constants come from the high-Q² `PhysicsConstants.h`**,
   not the low-Q² header (builder lines 49-52).

### 3.2 Required builder changes for SIDIS feature parity

| Change | Effort | Risk | Notes |
|--------|--------|------|-------|
| Add `Q2e/Ye/Xe`, `Q2da/Yda/Xda`, `Q2h/Yh/Xh`, `Q2es/Yes/Xes` reco branches | small | low | Compute from primitives or read from HAT if upstream stores them |
| Add `Q2eGen/...`, `Q2daGen/...`, `Q2hGen/...`, `Q2esGen/...` gen branches | small | low | Same |
| Add eSigma-frame SIDIS variables (`ystar_es, yBreit_es, xF_SIDIS_es, z_SIDIS_es`) | small | medium | Mirror the low-Q² eSigma virtual-photon construction; risk is silent degeneracy with Sigma-frame branches (see OQ8 / Task 2B.10 in `context-handoff.md`) |
| Add `ParticleSource_cg` to ChargedTotGen | trivial | low | Already classified in proton gen loop; just add branch and store |
| Adopt broad gen fiducial (`|GenEta|<3.5, GenPt>0.05`) for `inFiducialCharged` | trivial | low | One-line change; matches low-Q² F9 remediation |
| Verify F3/F6/F7 fixes are at full strength (current bounded leak) | medium | medium | Re-audit charge-mismatch and gen-fiducial paths against low-Q² `ProtonTreeBuilder_lowQ2.C` |
| Branch-name standardisation (drop `_ch` suffixes if they exist anywhere) | trivial | low | High-Q² builder already lacks `_ch` suffixes in branch names (`chargedTree->Branch("GenPDG", &GenPDG_ch)` etc., lines 422-441). Variable names use `_ch` internally only. |
| F13 (isMC vs isMCMode) consistency check | trivial | low | High-Q² builder uses both; verify they agree (lines 483-495) |

The single biggest risk item is **eSigma-frame SIDIS for high-Q²**. At
high Q² the eSigma method is already the upstream calibrated kinematic
choice (see lambda_HiQ handoff Section 2: "Calculator GetQ2es()/GetYes()").
This means the SIDIS frame variables stored under `Q2s/Ys` in the
ChargedTot tree are **already eSigma-tagged values**, despite using
Sigma-named variables. The builder currently does not store any
electron-only `Q2e` branch, so a comparison cannot even be done from
existing trees. This is structurally different from low-Q² where the
HAT `Q2s/Ys` are true Sigma values and the builder has to derive
eSigma separately.

This means the **OQ8 method-comparison story is fundamentally different at
high-Q²**. There is essentially one frozen "best" kinematic method
(eSigma, already calibrated), and additional methods are diagnostic
overlays rather than candidates for nominal selection.

### 3.3 Cannot simply "copy the remediated low-Q² builder"

For all the reasons above:
- DIS selection / hadronic calibration ownership differs.
- HAT pointer availability differs.
- The "Sigma" branch names hide an underlying eSigma kinematic choice.
- Fiducial constants live in a separate `PhysicsConstants.h` fork.

Practical recommendation: treat the high-Q² builder as the **second
fork** of the same source family. Cherry-pick the DIS-method storage
extension and the eSigma-frame SIDIS-variable extension from the low-Q²
builder, but keep upstream selection ownership unchanged.

Estimated builder effort: **1-2 days** of focused work (mostly code,
not physics), plus a smoke test pass. Less than the original low-Q²
F1-F13 remediation campaign because:
- Charged-particle fixes (F1-F5 ledger items 1-5 from header) are already
  applied.
- DIS-method extension is largely mechanical.
- Validation infrastructure (smoke MC pipelines) already exists.

### 3.4 Steering and configuration differences

| Item | Low-Q² | High-Q² | Action |
|------|--------|---------|--------|
| Event-list source | Embedded in builder | Upstream `H1BaryonAsym_HighQ2/output/evls/` | Keep separate |
| Per-period good-run files | `SelectedRuns.root` (S61) | `nc_goodruns_{0304,0405,06,0607}.root` (S67) | Keep separate; fix `submit_full_production.py` |
| Steering generator | `submit_*.py` for low-Q² | `H1SIDIS_HighQ2/scripts/submit_full_production.py` | Already exists, has 3 known bugs (Section 2.2) |
| Output paths | `H1ProtonAsym/output/...` | `H1SIDIS_HighQ2/output/...` | Already separate |

---

## 4. Electron Reconstruction: LAr vs SpaCal

### 4.1 Detector physics

| Quantity | SpaCal (low-Q²) | LAr (high-Q²) |
|----------|-----------------|---------------|
| θ_e range | ≈ 153° -- 178° (backward) | ≈ 4° -- 154° (central + forward) |
| Energy resolution | σ_E/E ≈ 7%/√E ⊕ 1% | σ_E/E ≈ 12%/√E ⊕ 1% (EM) → with hadrooII: ~50%/√E + 2% effective |
| Energy scale | 1% (well known) | 1-5% **position-dependent** across 5 z-impact regions |
| Angle resolution | ~1 mrad | ~1 mrad (similar with LAr cluster centroid) |
| Fiducial cuts | SpaCal box / ellipse / radius / dead cells | `FidVolCut` (3 layers: barrel-wheel transitions, octant phi cracks, escape clause) + `Zimpact` cuts |
| H1OO support | Standard | `H1HadronicCalibration::eHighPtJet`, special cluster code |

Sources: `research-sidis-systematics-framework.md` Section 3.6;
`research-highQ2-proton-asymmetry.md` Sections 2.5 and 5.x;
`reference-sidis-systematics.md` lines 220-227;
task prompt (5 z-impact regions, 1-5% scale uncertainty).

### 4.2 Systematics implications

Per the explicit rules in `critic-sidis-systematics-distilled.md`
lines 152-157 and `reference-sidis-systematics.md` lines 181, 220-227:

> Do NOT transfer the multiplicity-paper SpaCal 1-5% number directly to
> high-Q² LAr.

This means:
1. The high-Q² electron-scale systematic must be evaluated independently.
2. The variation procedure is "shift LAr cluster energy by the
   z-impact-bin-specific value (1-5%) and re-run from the evls stage".
   This is NOT a post-hoc 4-vector shift on stored branches because the
   shift is multi-valued across the cluster.
3. Per `research-sidis-systematics-framework.md` Section 4 (line 603),
   the electron-energy-scale shift "requires re-run of evls Condor jobs
   for BOTH data and MC". Cost estimate: 1.4k jobs (635 data + 846 MC
   per the current production state, scaled up ~2.5× when e⁻ is
   recovered). Wallclock: 24-48 hours per shift direction.

### 4.3 Acceptance correction differences

The acceptance correction itself is structurally identical
(`epsilon = N_reco / N_gen` per bin, with charge-separated treatment).
The differences arise in the **systematic evaluation**, not in the
nominal computation:
- LAr position-dependent scale variation must be propagated through the
  acceptance, not just through the data.
- The scale variation couples to `C_rad` because both depend on Q²
  (per `research-sidis-systematics-framework.md` Section 5 caveat about
  electron-scale / C_rad coupling).
- The 4D `(x, Q² z, pT_h)` binning sees electron-scale shifts mostly in
  Q² and x (event-level), and through the photon-axis pT_h definition
  in `pT_h` (because the virtual-photon direction depends on the
  scattered electron direction).

### 4.4 Open question: authoritative LAr scale procedure

The framework documents flag this as `OQ3` (open question, system
SB4): **"What is the authoritative high-Q² LAr electron-reconstruction
variation procedure and scale?"** Per `plan-sidis-execution.md` line 901,
this is owed to the human analysis lead and `h1-data-validator`. This
remains unresolved and is a **prerequisite for any publication-grade
high-Q² systematic budget**.

---

## 5. TMD Physics Motivation for Combining Low-Q² and High-Q²

### 5.1 What each regime contributes uniquely

**Low-Q²** (already addressed in `plan-tmd-sidis.md`):
- Smallest-x SIDIS data ever taken in collider mode at √s = 319 GeV
- Sea-quark and gluon-induced TMD fragmentation
- High statistics, but with TMD factorization marginal because
  pT_h ~ Q is reached easily
- Diffractive contamination significant at low x (`x < 10⁻³`)

**High-Q²**:
- Reaches valence-quark x ~ 0.01-0.5 with charged-hadron statistics
- pT_h/Q is generically small even up to pT_h ~ 2 GeV, so the
  TMD-safe phase space is generous
- Larger Q² means **TMD evolution is dynamically more important** --
  this is the regime where Collins-Soper-Sterman / TMD-evolution
  formalisms are most cleanly tested
- Very different flavor sensitivity (u-quark dominance via `e_q² ∝ 4/9`
  vs ~1/9 for d/s) means h⁺/h⁻ asymmetries probe valence flavor structure
- Lower diffractive contamination (large Q² suppresses diffractive
  cross-section relative to inclusive)

### 5.2 Why the COMBINATION is uniquely valuable

The dataset that spans `Q² = 5 → 20,000 GeV²` **at the same √s with the
same detector** is unique to H1. No other experiment in TMD-fit history
covers more than ~1.5 decades of Q² in SIDIS:

- HERMES (12-27 GeV beam): Q² ~ 1-15 GeV², ~1 decade
- COMPASS (160 GeV beam, fixed target): Q² ~ 1-100 GeV², ~2 decades
- JLab12 (11 GeV beam): Q² ~ 1-10 GeV², ~1 decade
- EIC (future): Q² up to ~1000 GeV², will be the natural successor
- **H1 combined**: Q² 5 -- 20,000 GeV² at fixed √s = 319 GeV → ~3.6 decades

Three decades of Q² at fixed √s is precisely the **TMD-evolution-only
lever arm** that fits cannot extract from any other dataset, because
fixing √s holds the soft-scale physics constant while sweeping the
hard scale. This is qualitatively different from combining HERMES
(low Q², low √s) with COMPASS (medium Q², medium √s) where soft-scale
physics differs.

### 5.3 What pT_h/Q ratio for TMD-safe bins at high Q²

`plan-tmd-sidis.md` Section 1.4 specifies for low-Q²:
- `pT_h < min(2.0 GeV, Q/2)` for the inclusive phase space
- `pT_h < min(1.5 GeV, Q/2)` for the TMD-fit-preferred subset
- Optional stricter `pT_h/Q < 0.3`

At high Q², the same `pT_h/Q < 0.3` rule gives:
- Q² = 150: Q ≈ 12 GeV, so pT_h < 3.7 GeV (the analysis cap of 2 GeV
  binds)
- Q² = 1000: Q ≈ 32 GeV, pT_h < 9.5 GeV (entire pT_h spectrum is safe)
- Q² = 10000: Q ≈ 100 GeV, pT_h < 30 GeV (entire spectrum trivially safe)

So at high Q² the TMD-safe phase space is **the entire measured pT_h
range** for almost every bin. The flag `pth_over_q_flag` from
`plan-tmd-sidis.md` Part 4 will rarely fire in the high-Q² sample.
This is one reason why the high-Q² regime, although statistically
weaker, is much more theoretically clean for TMD interpretation.

### 5.4 Consistency in observable definitions

If the combination is to be published as one dataset (or even as
side-by-side tables in two papers), the following must match between
regimes:
- `pT_h` definition (photon-axis) -- both can compute this offline from
  stored 4-vectors per `plan-tmd-sidis.md` Section 1.2
- `phi_h` definition (Trento convention)
- `z` definition (Lorentz-invariant `P·P_h / P·q`)
- Charge-blind h⁺ / h⁻ separation
- Theory-caution flag taxonomy
- Systematic-source naming

The only structural mismatch is the **DIS kinematic method**:
low-Q² is currently defaulting to `Q2s/Ys` (true Sigma) but evaluating
eSigma alternatives, while high-Q² already uses calibrated eSigma
labelled as `Q2s/Ys` in the trees. As long as both publish the same
eSigma-final outcome, the labels can be reconciled in the metadata.

---

## 6. Production Roadmap to Reach Low-Q² Parity

This section lists concrete steps. It does NOT include timelines per
the planning conventions in this directory.

### 6.1 Stage 0: Decide ownership

Determine whether SIDIS at high-Q² will:
- (A) live in `H1SIDIS_HighQ2/` (current state — the package was renamed
  from `H1ProtonAsymHighQ2/` during the proton-fork-to-SIDIS transition;
  leverages existing production infrastructure and historically shared trees
  with proton/antiproton, though v5b removed the proton-specific trees)
- (B) get its own package separate from any proton-asymmetry infrastructure
  (cleaner for publication, but duplicates infrastructure)

Recommendation: keep (A). The v5b SIDIS charged trees are the same physical
trees as those formerly used by the proton/antiproton analysis (the
`ProtonTot`/`ProtonTotGen` trees having been removed in v5b); both
observables historically came from the same files.

### 6.2 Stage 1: Recover the missing periods (HB4 closure)

Per `proton_HiQ/docs/research-data-coverage-gap.md`:
1. Patch `submit_full_production.py`:
   - Add `/03/` to data periods
   - Restructure `/06/` to handle em06 and ep0607 sub-ranges
   - Pass `--good-runs` with the period-specific file in the evls stage
2. Re-point the SIDIS steering files (legacy directory `steering/proton/`)
   at `evls_highQ2_dXX_PERIOD_NNNN.root`
3. Submit ~700 additional Condor jobs (data only, since MC already
   covers all periods)
4. Re-merge data file to include ep0304 + em0405 + em06 + ep0607
5. Verify the new total against 356,613 nb⁻¹ S67 lumi reference

Cost: ~1 day prep + 1 day Condor + 0.5 day validation.
Outcome: lumi tripled from 140k → 357k nb⁻¹.

### 6.3 Stage 2: Builder DIS-method and eSigma-SIDIS extension

Cherry-pick from low-Q² builder:
- All DIS kinematic method branches (Q2e/Q2da/Q2es/Q2h plus Y and X
  variants) on both reco and gen sides
- eSigma-frame SIDIS variables (`*_es`)
- `ParticleSource_cg` on ChargedTotGen
- Broad gen fiducial in `inFiducialCharged`
- Re-audit F3/F6/F7 to bring leak below 0.2% (current 0.76%)

Cost: 1-2 days code + 1 day smoke test + recompile and rerun all MC
+ data (high-Q² has 1481+ jobs as of stage 1).
Outcome: branch counts 82/35 (or close) matching low-Q² target schema.

### 6.4 Stage 3: Schema and method consistency gates

Run the equivalent of low-Q² Task 2B.10 (eSigma consistency gate):
- Confirm `*_es` branches are physically distinct from default branches
- Reconcile naming: at high-Q² the upstream `Q2s/Ys` is already
  eSigma-calibrated; document this in the per-bin metadata
- Verify gen-level method-consistent reconstruction

Run validation gates equivalent to low-Q² post-merge validation:
- Branch population checks
- Charge / species / source sanity
- Run coverage across all four periods
- 4D phase-space hole detection in the new (x, Q², z, pT_h) grid

### 6.5 Stage 4: Acceptance and acceptance systematics

Reuse the low-Q² acceptance machinery essentially unchanged:
- Charge-separated 4D `(x, Q², z, pT_h)` acceptance maps for h⁺ and h⁻
- `(z, pT_h)` sliced response within coarse `(x, Q²)` cells if needed
  per `plan-tmd-sidis.md` Section 3.2
- Apply `effective_neff >= 1000` threshold (likely the binding
  constraint at high Q² given lower statistics)

The high-Q² 4D grid likely needs **coarser binning** than the low-Q²
4D grid because per-bin statistics will be ~3-5× lower. Concrete bin
edges are not specified in any current document; this is a gap that
the analysis must close.

### 6.6 Stage 5: Establish the high-Q² electron-scale procedure (OQ3)

Resolve `OQ3` / `SB4` from `plan-sidis-execution.md`:
- Identify the authoritative LAr electron variation procedure
  (likely H1CalcSystematic with z-impact-binned shifts).
- Either re-run evls with shifted electron energy (24-48 h Condor per
  shift direction) or implement a post-hoc analytic propagation if
  feasible (only feasible if the shift is single-valued, which the
  position-dependent LAr is not).
- Document the procedure in a `reference-sidis-systematics-highQ2.md`
  companion to the low-Q² reference.

### 6.7 Stage 6: H1CalcWeight integration (HB5 closure)

Per `proton_HiQ/docs/research-mc-normalization-highQ2.md` Section 8:
- Either integrate `H1CalcWeight::GetWeight()` into the builder
  (stores fully corrected per-event weight)
- Or run one MC chunk through the event-shapes binary and extract the
  empirical scale

This is required for absolute multiplicities (denominator `N_DIS_corr`)
but NOT for charge asymmetries. If the first publication is asymmetries
only, this can be deferred. If multiplicities are in scope, this is on
the critical path.

### 6.8 Stage 7: NonRad MC for C_rad (HB6 + low-Q² C_rad)

Out of scope for this research, but flagged: the same NonRad MC
production blocker affects both regimes. A unified DJANGOH14 NonRad
production would unblock C_rad in both at once.

### 6.9 Stage 8: Combined-regime release artifact

If both regimes are to ship together, build a combined HEPData /
auxiliary-JSON manifest with:
- Bin definitions joined across regimes
- Common `pT_h_photon`, `phi_h`, `z` definitions
- A `regime` flag per bin (`low_Q2` | `high_Q2`)
- A `dis_method` flag per bin (`Sigma`, `eSigma`, `electron`, `DA`, `JB`)
- All theory-caution flags from `plan-tmd-sidis.md` Section 4.1

---

## 7. Blockers and Risks

### 7.1 Blocking issues for high-Q² SIDIS

| ID | Issue | Class | Path forward |
|----|-------|-------|--------------|
| HB1 | Builder lacks DIS-method-method branches | Builder | Cherry-pick from low-Q² (Section 6.3) |
| HB2 | Builder lacks eSigma-frame SIDIS branches | Builder | Cherry-pick (Section 6.3) |
| HB3 | F3/F6/F7 leak 0.76% (publication-grade requires <~0.2%) | Builder | Re-audit (Section 6.3) |
| HB4 | Missing e⁻ data (em0405, em06) and ep0304 | Production | Recovery via Section 6.2 (no new physics, fix submission scripts) |
| HB5 | H1CalcWeight not integrated → no absolute normalization | Builder + framework | Integrate (Section 6.7); blocks multiplicities, not asymmetries |
| HB6 | Missing gen-electron branches for 4D pT_h acceptance in v2 | Builder | v3 trees already have these per task prompt; verify and extend if needed |
| HB7 | OQ3: authoritative LAr electron-scale procedure undecided | Framework | Resolve with `h1-data-validator` (Section 6.6) |
| HB8 | NonRad MC missing → C_rad placeholder | MC | Cross-regime issue; not high-Q² specific |

### 7.2 Risks specific to the combination

| Risk | Description | Mitigation |
|------|-------------|------------|
| R1 | DIS-method nominal differs between regimes | Low-Q² may freeze on Sigma or eSigma; high-Q² is structurally eSigma | Document method per bin in metadata; do not paper over |
| R2 | Diffractive treatment differs | Low-Q² has significant diffractive contamination at low x; high-Q² less so | Carry per-bin `x_diffr_flag` in both, with different thresholds |
| R3 | Trigger systematics differ | S61 (low-Q²) vs S67 (high-Q²) have different systematic origins | Two separate `trigger_*` sources, both mostly cancelling in A_h |
| R4 | Electron-systematic procedures differ | SpaCal post-hoc shift vs LAr position-dependent re-run | Two separate `electron_scale_*` sources, never combined into one band |
| R5 | Bin definitions in (x, Q²) overlap awkwardly at the boundary | Low-Q² grid stops at Q²=100; high-Q² starts at Q²=150 -- gap from 100 to 150 | Either accept the gap (keep `Q² ∈ [100, 150]` unmeasured) or extend low-Q² to Q²=150 with a careful trigger study |
| R6 | Period coverage differs | Low-Q² has all 3 years; high-Q² currently has 1 year, recoverable to 4 | Beam-charge asymmetry comparison can only happen after HB4 closure |
| R7 | Different MC reprocessing campaigns | Low-Q² uses oo-4.0.6; high-Q² uses oo-4.0.25 MC13 | Potential systematic from MC reconstruction code version; test by reprocessing one period in both |
| R8 | Acceptance ε > 1 anomaly in highest-pT bin (proton trees) | Already documented in proton_HiQ; same issue may affect charged tree | Validate on charged-particle gen denominator; the broad-fiducial fix should resolve |
| R9 | At very high Q² (> 2000 GeV²) electroweak γ-Z interference is non-negligible | Sub-leading but present; not in current MC weighting | Either restrict to Q² < 2000 or apply EW correction |

### 7.3 What could go irreparably wrong

The risks above are all manageable. The two genuinely hard scenarios:
1. **OQ3 (LAr electron-scale procedure) cannot be closed.** If no
   authoritative procedure can be cited, the high-Q² systematic budget
   for electron-scale must remain a "bounded, undocumented" placeholder
   and would not survive H1 internal review. Mitigation: borrow the
   procedure from the H1 inclusive DIS multiplicity paper (arXiv:2011.01812
   per `research-sidis-systematics-framework.md` line 727) and adapt.
2. **High-Q² statistics insufficient for 4D bins.** If even the coarse
   `3 × 3 × 4 × 5` grid from `plan-tmd-sidis.md` Section 1.5 is too
   sparse for high-Q² alone, the analysis must publish coarser
   `(Q², z, pT_h)` 3D tables instead of the full 4D set, sacrificing
   x-differentiality. This is a real possibility given that the high-Q²
   sample after recovery will have ~250k h⁺ + h⁻ tracks (estimated by
   scaling current 513k tracks at 140k nb⁻¹ to 357k nb⁻¹), spread
   across the much larger Q² range. **The TMD program would still
   benefit; it just looks more like 3D + flat-x rather than full 4D.**

---

## 8. Recommendation

### 8.1 Pursue as the same analysis or separately?

The evidence supports **one analysis program, two publication tracks**:

**Recommendation: ONE coordinated analysis program, TWO papers (or one
paper + companion note).**

Rationale:
1. Same observables (charge-separated multiplicities, A_h), same
   physics target (TMD-fit input), same TMD-fit consumer audience.
2. Same code family (the high-Q² builder is a structural fork of the
   low-Q² builder; they share the charged-particle remediation work).
3. Same supervisor logic (`h1-sidis-supervisor` already covers SIDIS;
   no second supervisor needed).
4. Strong physics case for the combined dataset (Section 5.2: three
   decades of Q² at fixed √s).
5. But trigger / electron-detector / systematics-procedure differences
   are large enough that one paper trying to merge both into a single
   "H1 SIDIS multiplicities" table risks burying the systematic
   distinctions under a common total-error band -- which TMD fitters
   would correctly distrust.

The natural sequencing follows `plan-tmd-sidis.md` Section 7-8 already:
- **Paper 1 (low-Q²):** the unique-low-x H1 SIDIS multiplicity dataset.
  This is closer to publication-ready and has unique value standalone.
- **Paper 2 or major addendum (high-Q²):** the unique-high-Q²
  H1 SIDIS multiplicity dataset, framed as the TMD-evolution lever-arm
  partner.
- **Optional Paper 3 (combined release):** a unified HEPData package
  that fitters can ingest as one table with regime flags. This requires
  no new measurement, only release packaging.

### 8.2 Do NOT block low-Q² on high-Q²

`plan-tmd-sidis.md` Section 7.3 is correct on this:

> Do not block the low-Q2 TMD multiplicity paper on high-Q2 completion.

The high-Q² extension is high value but requires:
- HB4 (data recovery, ~1 week)
- HB1+HB2+HB3 (builder extension and re-audit, ~1 week)
- HB7 (LAr electron-scale procedure, undefined timeline)
- HB5 (H1CalcWeight, undefined timeline)
- Full reprocessing on the recovered dataset
- Acceptance and systematics studies in 4D (or 3D fallback)

This is a multi-month additional effort even if every step succeeds on
the first attempt. The low-Q² result has its own publication value and
should not wait.

### 8.3 What to start now in parallel with low-Q² finishing

Three high-leverage items can begin immediately while low-Q² closes:
1. **HB4 closure (period recovery).** The work is well-understood per
   `research-data-coverage-gap.md`; mostly a submission-script patch
   plus one Condor pass. Can be done by `h1-production` in parallel
   with low-Q² merging and validation.
2. **HB1+HB2 builder extension.** Cherry-pick DIS methods and eSigma
   SIDIS into the high-Q² builder. Mechanical and well-bounded. Can
   be done by `h1-software` while low-Q² systematics run.
3. **OQ3 / HB7 framework research.** Identify the authoritative LAr
   electron-scale procedure from H1 internal documents (the H1
   multiplicity paper arXiv:2011.01812 is the best lead per
   `research-sidis-systematics-framework.md` line 727). This is
   research, not code, and unblocks the systematic budget downstream.

The remaining items (HB5 H1CalcWeight, HB6 4D pT_h acceptance, HB8
NonRad MC) are larger and should follow once the low-Q² analysis is
in publication review.

### 8.4 Bottom line

- The high-Q² extension is **technically tractable and physics-valuable**.
- It is **not a copy-and-paste exercise**: the high-Q² builder needs
  cherry-picked DIS-method and eSigma extensions, and the systematic
  budget for the LAr electron is structurally different from the SpaCal
  budget already implemented for low-Q².
- The data exists; the e⁻ recovery is mostly a configuration fix.
- The right scope is **a coordinated SIDIS-program with at least two
  publication tracks**, where the low-Q² paper ships first and the
  high-Q² extension follows under the same observable definitions and
  release schema.
- The combined-regime dataset, when released, is **uniquely valuable
  for TMD evolution constraints** because it spans ~3.6 decades in Q²
  at fixed √s, which no other dataset achieves.

---

## 9. Areas Needing Further Investigation

The following questions are out of scope of this research but must be
resolved before high-Q² SIDIS extraction can proceed:

1. **Does the v3 high-Q² tree already have all gen-electron branches
   needed for 4D photon-axis pT_h acceptance?** Task prompt says yes
   (28 ChargedTotGen branches). Verify by direct branch inspection and
   compare against the low-Q² 35-branch target schema.
2. **What is the actual data event count after e⁻ recovery?** The
   research-data-coverage-gap document estimates ~195k additional
   events but does not reconcile with the actual evls EvlTree counts.
3. **What 4D bin grid is statistically supported at high Q²?** The
   low-Q² grid in `plan-tmd-sidis.md` Section 1.5 is not directly
   transferable. A high-Q²-specific occupancy study is needed.
4. **What is the H1 internal authoritative LAr electron-scale variation
   procedure?** This is `OQ3` and remains unresolved.
5. **Is the bounded F3/F6/F7 leak (0.76%) acceptable for publication
   asymmetries, or must it be closed first?** The low-Q² remediation
   reduced the analogous leak below 0.1%; the threshold for high-Q²
   publication is not documented.
6. **Will diffractive-MC processing happen for high-Q² alongside low-Q²?**
   The diffractive contamination at high-Q² is smaller but not zero;
   `x_diffr_flag` thresholds need calibration for the high-Q² regime.
7. **Should the electron-scale variation be applied via re-run from
   evls, or is there a faster post-hoc analytic proxy that's good enough
   for the 4D TMD release?** This affects whether HB7 takes weeks or
   months.

---

## 10. Annotation Cycle Status

- Iteration: 1
- Status: First-pass research artifact, written from existing
  documentation in `H1/sidis_lowQ2/`, `H1/proton_HiQ/`, and `H1/lambda_HiQ/`.
- Open follow-ups: items in Section 9.
- Audience-next: SIDIS supervisor (`h1-sidis-supervisor`) for the
  pursue/defer/scope decision; `h1-software` for builder cherry-pick
  scoping; `h1-production` for HB4 recovery scoping.
