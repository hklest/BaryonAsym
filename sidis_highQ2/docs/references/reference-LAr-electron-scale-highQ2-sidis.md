<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Reference: LAr Electron Energy-Scale and Angle-Shift Procedure for High-Q² Charged SIDIS (Reuse Extract)

**Date:** 2026-05-19
**Author:** researcher
**Scope:** Document the LAr electron-energy-scale and polar-angle-shift systematic
procedure as it has been implemented (or planned) by the high-Q² baryon
analyses `proton_HiQ` and `lambda_HiQ`, for direct reuse by the high-Q²
charged-particle SIDIS analysis. Detector context is LAr only (Q² > 150 GeV²,
S67 trigger). The SpaCal low-Q² procedure is explicitly excluded and must not
be numerically transferred.
**Status of this extract:** NOT REUSE-READY. See the "Headline Finding" and
"Gaps to escalate" sections below. The upstream analyses do not contain an
implemented, validated, or even concretely specified LAr electron-scale
procedure that can be lifted as-is into SIDIS.

---

> **Glossary (added 2026-05-19).** In this document, `proton_HiQ` and
> `lambda_HiQ` refer to the SEPARATE proton/antiproton asymmetry analysis
> (workspace `H1/proton_HiQ/`) and the separate Lambda/K0s high-Q² analysis
> (workspace `H1/lambda_HiQ/`), respectively. They are **NOT** this SIDIS
> analysis. Every occurrence of `proton_HiQ` and `lambda_HiQ` below is a
> TRUE PROTON / TRUE Lambda reference to those external workspaces; this
> document inspects them as candidate sources of LAr-systematics
> infrastructure to be reused (or not) by the high-Q² SIDIS analysis. For
> the broader naming-alias context covering legacy proton-prefixed names
> inside the SIDIS workspace itself, see
> `docs/references/reference-naming-aliases-highQ2-sidis.md`.

---

## Headline Finding

The user's premise that the LAr electron-energy-scale and angle-shift
systematic is "solved in proton_HiQ and lambda_HiQ" is **not supported by the
local repository state** as of 2026-05-19. A full read of:

- `H1/proton_HiQ/context-handoff.md`
- `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md`
- All 22 files under `H1/proton_HiQ/docs/`
- `H1/lambda_HiQ/context-handoff.md`
- All 7 files under `H1/lambda_HiQ/docs/`
- The high-Q² TreeBuilder source snapshot
  `H1/lambda_HiQ/src_review/TreeBuilder.C`
- The proton_HiQ source snapshots under `H1/proton_HiQ/code_review/`

shows that:

1. **No numerical LAr electron-energy-scale shift value** (e.g. ±1%, ±2%,
   ±X%) is recorded in any proton_HiQ or lambda_HiQ document.
2. **No numerical LAr electron polar-angle-shift value** (e.g. ±1 mrad,
   ±3 mrad) is recorded.
3. **No application mechanism** is documented: there is no
   `H1CalcSystematic::Set...` call, no `--sys` CLI flag, no post-hoc
   electron-4-vector reweighter, and no derived-kinematic shift in any
   inspected source file.
4. **No "shifted-MC" production pass** has been executed by either
   `proton_HiQ` or `lambda_HiQ`. There is no `shifted/` output directory,
   no Condor submission script for shifted MC, and no merged-file naming
   convention for shifted samples.
5. The `lambda_HiQ` TreeBuilder does `#include "H1Calculator/H1CalcSystematic.h"`
   on line 92 of `H1/lambda_HiQ/src_review/TreeBuilder.C`, but the header is
   **never used**: grep finds zero `H1CalcSystematic::` symbol references in
   the body of the file. This is an inherited include, not an implemented
   procedure.
6. The lambda_HiQ phase plan
   (`H1/lambda_HiQ/docs/plan-highq2-to-final.md` Section "Phase 4", Work
   Package 6, lines 568-577) explicitly lists "**electron theta / energy
   sensitivity**" as a **TODO** item under "Evaluate high-Q2-specific
   detector / selection systematics", with completion criterion "**at
   minimum the following are screened and either quantified or rejected
   as negligible**". This task is **not marked complete** anywhere in the
   handoff.
7. The lambda_HiQ script-mapping research
   (`H1/lambda_HiQ/docs/research-script-mapping.md` Section 5.4,
   lines 304-318) defines a planned `lar_systematics.py` script
   ("**NEW: LAr-specific detector systematics**") with content
   "**LAr fiducial variation, Hadronic calibration / eSigma migration,
   Vertex window variation, E-pz window variation, ye cut variation,
   Electron theta/energy sensitivity**", effort **8-12 hours**, and tier
   **Tier 4: After nominal result frozen**. The script does not exist;
   the effort has not been spent.
8. The lambda_HiQ completed-systematics list in
   `H1/lambda_HiQ/context-handoff.md` Section 6, "Systematic Uncertainties
   (5 sources on asymmetry)", names only: *Signal window,
   ProtonDedxLikelihood cut, decay radius, DCA, generator model*. **No
   electron-scale or angle-shift source is listed.**
9. The proton_HiQ handoff (`H1/proton_HiQ/docs/context-handoff-highQ2-proton.md`)
   does not list electron-scale or angle-shift in any section. Its
   "Open Issues" Section 6 lists "**MC normalization**, **species-separated
   acceptance**, **charge-separated proton acceptance**, **cutflow diagrams**,
   **4D binning implementation**" -- none of which is an LAr electron-scale
   systematic. The string "energy scale" appears in
   `H1/proton_HiQ/docs/research-sidis-4d-binning.md` lines 780-781 and
   `H1/proton_HiQ/docs/research-4d-tmd-binning.md` lines 522-526 only as
   **expected magnitude in a planning table** ("1-3% Electron energy
   scale", "sigma(theta_e) ~ 1-3 mrad"), not as an implemented procedure
   with a chosen variation amplitude.
10. The SIDIS reference playbook
    (`H1/sidis_highQ2/docs/references/reference-sidis-systematics.md`)
    independently corroborates that the procedure is unspecified upstream:
    - Line 150 (high-Q² catalog source #3, "Electron reconstruction /
      energy-angle scale in the LAr regime"): "**TBD - to be evaluated
      locally; framework says a few percent expected but no
      authoritative H1 number is established**".
    - Line 223 (High-Q²-only override): "**Replace the SpaCal-specific
      electron source with an LAr electron-reconstruction source in
      high-Q2.**" (Statement of policy; not a recipe.)
    - Line 229 (Prohibited cross-transfer): "**Do not transfer the
      multiplicity-paper SpaCal 1-5% number directly to high-Q2 LAr.**"
    - Line 507 (Open Question 3): "**What is the authoritative high-Q2
      electron-reconstruction variation procedure and numerical scale in
      the LAr regime?**" -- listed under "track in supervisor sessions".

The high-Q² SIDIS next-steps plan asserts that the procedure is "solved
upstream" and "must be reused":

- `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` line 110:
  "*The LAr electron-scale procedure is treated as solved upstream in
  prior high-Q2 analyses and must be reused from `proton_HiQ` /
  `lambda_HiQ`, not redeveloped from H1OO primitives.*"
- Same file line 622, task: "**Extract the authoritative LAr
  electron-energy-scale and angle-shift procedure from the `proton_HiQ`
  and `lambda_HiQ` high-Q2 analysis chains and document it in a high-Q2
  SIDIS reference note**".

**That assertion is contradicted by the local upstream artifacts.** The
present reference note therefore records exactly what is and is not present
in `proton_HiQ` and `lambda_HiQ`, and flags every missing piece as a gap
that must be closed before G11 LAr sub-gate can be declared REUSE-ready.

---

## Q1 -- Nominal LAr electron-energy-scale shift used by proton_HiQ and lambda_HiQ

**Documented numerical value in the upstream high-Q² baryon analyses:
NONE.**

| Document inspected | Numerical value found? | Citation |
|---|---|---|
| `H1/proton_HiQ/context-handoff.md` | No | full file, 39 lines |
| `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` | No | full file, 333 lines; "Open Issues" Section 6 does not mention electron scale |
| `H1/proton_HiQ/docs/plan-highQ2-proton-asymmetry.md` | No | grep for "electron\|LAr\|scale\|shift" returns only fiducial/DIS-selection lines (138, 355, 533, 1253, 1274) |
| `H1/proton_HiQ/docs/research-sidis-4d-binning.md` lines 780-781 | Planning table only: "DIS kinematic reconstruction 1-3% Electron energy scale" | Expected magnitude in a binning-design table, not an implemented variation |
| `H1/proton_HiQ/docs/research-4d-tmd-binning.md` lines 522-527 | Planning text only: "sigma(E_e')/E_e' ~ 7.1%/sqrt(E_e') + 1% (electromagnetic section); sigma(theta_e) ~ 1-3 mrad" | Statement of LAr resolution, not a chosen systematic shift amplitude |
| `H1/proton_HiQ/docs/research-sidis-tmd-optimal-binning.md` line 247 | "Q2 ~5-10% (LAr calorimeter, electron method)" | Resolution estimate, not a systematic shift |
| `H1/lambda_HiQ/context-handoff.md` Section 6 systematics list | No | The 5 listed sources do not include LAr electron scale |
| `H1/lambda_HiQ/docs/plan-highq2-to-final.md` Phase 4 WP6 lines 568-577 | "electron theta / energy sensitivity" listed as TODO with no numerical value chosen | This is a forward-looking requirement, not a recorded result |
| `H1/lambda_HiQ/docs/research-script-mapping.md` Section 5.4 lines 304-318 | "Electron theta/energy sensitivity" listed as content of a not-yet-written script `lar_systematics.py` | No numerical value |

**Result: Q1 cannot be answered from upstream artifacts. There is no
adopted numerical LAr electron-energy-scale value to inherit.**

## Q2 -- Nominal LAr electron polar-angle shift used by proton_HiQ and lambda_HiQ

**Documented numerical value in the upstream high-Q² baryon analyses:
NONE.**

The only quantitative LAr angle statements in the inspected documents are:

- `H1/proton_HiQ/docs/research-4d-tmd-binning.md` line 506
  ("Lab theta CJC tracking ~1 mrad Excellent" -- this is the *track*
  angle resolution, not an LAr-electron systematic shift).
- `H1/proton_HiQ/docs/research-4d-tmd-binning.md` line 526
  ("sigma(theta_e) ~ 1-3 mrad" -- LAr resolution estimate, **not** a
  chosen systematic shift amplitude).
- `H1/proton_HiQ/docs/research-sidis-tmd-optimal-binning.md` and
  `research-4d-tmd-binning.md` mention the LAr angle resolution as
  *typical performance*, never as an applied systematic.

The 5 systematic sources actually quantified by `lambda_HiQ`
(`context-handoff.md` Section 6: signal window, ProtonDedxLikelihood cut,
decay radius, DCA, generator model) contain no electron-angle source.

**Result: Q2 cannot be answered from upstream artifacts.**

> Note for SIDIS reuse: The low-Q² SpaCal value of "±1 mrad polar angle"
> (`H1/sidis_lowQ2/docs/plans/plan-finish-sidis-analysis-r3.md` Section
> F4.1 and the published H1 charged-multiplicity paper, see
> `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md` line
> 136) **must not be carried over numerically** per the explicit
> prohibition in `reference-sidis-systematics.md` lines 229 and the
> distilled critic brief lines 156-157.

## Q3 -- Application mechanism (builder shift vs post-hoc reweighting vs derived-kinematic shift)

**Documented mechanism in upstream high-Q² baryon analyses: NONE
IMPLEMENTED.** The mechanism is *planned* but not built.

What the *plans* say the mechanism would be (forward-looking, not
extracted from existing implementation):

1. `H1/lambda_HiQ/docs/research-script-mapping.md` Section 5.4 line 318:
   "Effort: 8-12 hours (requires running **TreeBuilder with `--sys`
   variations**)" -- this says the *intent* is a builder-stage shifted
   re-run, not a post-hoc reweight. The `--sys` CLI flag does **not
   exist** in any inspected high-Q² TreeBuilder source.
2. `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md`
   Sections 4.2 (lines 354-364) and 4.4 (lines 382-389) repeat the
   "evls-stage re-run" intent and note that this is **OQ3** (open
   question, not resolved):
   - "The variation procedure is 'shift LAr cluster energy by the
     z-impact-bin-specific value (1-5%) and re-run from the evls stage'.
     This is NOT a post-hoc 4-vector shift on stored branches because
     the shift is multi-valued across the cluster."
   - "the electron-energy-scale shift 'requires re-run of evls Condor
     jobs for BOTH data and MC'. Cost estimate: 1.4k jobs ... Wallclock:
     24-48 hours per shift direction."
   - "This remains unresolved and is a prerequisite for any
     publication-grade high-Q² systematic budget."
3. `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md` line
   319 ("Variation Execution Cookbook"): the high-Q² `electron_scale_shift`
   handle is listed with `Cost class: full-reproduction (Condor-expensive)`
   and points at "evls-stage rerun requirement" -- i.e. a *builder*
   variation, not a post-hoc reweight.

What is actually in the source today (only piece of evidence in
implementation code):

- `H1/lambda_HiQ/src_review/TreeBuilder.C` line 92:
  `#include "H1Calculator/H1CalcSystematic.h"`
  This include is **dead**: zero usages of `H1CalcSystematic::` symbols
  in the rest of the file (`grep "H1CalcSystematic" TreeBuilder.C` matches
  only this one line). The lambda_HiQ TreeBuilder neither calls any
  H1CalcSystematic setter nor reads any CLI flag that would switch it on.
- `H1/proton_HiQ/code_review/ProtonTreeBuilderHighQ2_v2.C` and the
  teamA/teamB patched variants: no `H1CalcSystematic` include and no
  `--sys` CLI flag (confirmed by `grep "H1CalcSystematic\|--sys\|escale\|theta_shift"` across the entire H1/ tree, which returns no matches in proton_HiQ code).

**Result: Q3 cannot be answered from upstream artifacts. The intended
mechanism is builder-stage (evls-stage Condor re-production), but no
such production has been executed, no CLI plumbing exists, and no
H1CalcSystematic setter call exists in either builder.**

## Q4 -- H1CalcSystematic handles and method names involved

**Documented handles in upstream analyses: NONE.**

Of all 21 specialist agents in `H1/agents/`, only the planning narrative
(SIDIS-side, not high-Q² baryon-side) references `H1CalcSystematic` at
all, and only as the *expected* primitive layer:

- `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md`
  line 120: "Position-dependent scale uncertainty: 1-5% across z-impact
  regions (per `H1CalcSystematic` semantics noted in
  `research-tracking-systematics-deep.md` and the task prompt)."
- Same file line 553: "Identify the authoritative LAr electron variation
  procedure (likely `H1CalcSystematic` with z-impact-binned shifts)."

The word "likely" is the strongest commitment made anywhere. The upstream
proton_HiQ and lambda_HiQ code does not call any `H1CalcSystematic`
method, so no signatures can be extracted.

**Result: Q4 cannot be answered from upstream artifacts. No method
signatures are available.**

## Q5 -- Kinematic dependence (flat vs z-impact-binned vs energy-binned)

**Documented binning in upstream high-Q² baryon analyses: NONE
IMPLEMENTED.**

The only statement in the local repository about the *expected* binning
is in `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md`:

- Section 1.5 line 119: "Position-dependent scale uncertainty: 1-5%
  across **5 z-impact regions** (per `H1CalcSystematic` semantics noted
  in `research-tracking-systematics-deep.md` and the task prompt)."
- Section 4.1 line 336: "Energy scale | 1% (well known) | **1-5%
  position-dependent across 5 z-impact regions**".

These quantitative claims come from the task prompt that originally
authored the extension research, not from a measurement or
implementation in proton_HiQ or lambda_HiQ. They cannot be cited as
upstream-validated. There is **no explicit z-impact-bin edge list, no
per-bin shift table, and no central-value calibration** in the inspected
upstream artifacts.

**Result: Q5 cannot be answered from upstream artifacts. The "5
z-impact regions, 1-5% scale" statement is forward-looking framework
text, not an extracted result.**

## Q6 -- SIDIS-specific propagation aspects that upstream did not exercise

Even if a numerical procedure were inherited, several propagation steps
that are *trivial or absent* in the V0 / proton baryon analyses become
**load-bearing** for the SIDIS observables M^h±, A_h, A_BC, and ⟨φ_h⟩
moments. These all require new SIDIS code on top of any inherited
electron-shift procedure:

1. **Photon-axis pT_h.** SIDIS pT_h is computed in the (γ*, p) frame.
   The virtual-photon direction is `q = e_beam - e_scattered`, so any
   LAr energy/angle shift on the scattered electron rotates the
   gamma*-axis and shifts every hadron's pT_h. proton_HiQ and lambda_HiQ
   store and analyze lab-frame pT, not pT_h. (Source:
   `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` Section 3,
   branch list: `Pt` (lab), `Eta`, `Q2s`, `Ys`, `xBj`, `z_SIDIS`,
   `ystar`, `yBreit`, `xF_SIDIS`. The `z_SIDIS` variable is Lorentz
   invariant per `research-sidis-4d-binning.md` line 841, so it does
   *not* probe the photon-axis rotation. No `pT_h` branch is computed
   in the photon-axis frame in the high-Q² baryon trees.) SIDIS must
   re-propagate the LAr electron shift into pT_h.
2. **φ_h via the lepton-plane normal.** The Trento φ_h is defined by
   the cross product `(k × k') × q` evaluated event by event. Any
   electron polar-angle shift rotates `k'`, which directly biases
   φ_h-modulations (cos φ_h, cos 2φ_h, sin φ_h moments). proton_HiQ and
   lambda_HiQ do **not** compute φ_h at all (no `phi_h` branch in the
   tree inventory listed in `context-handoff-highQ2-proton.md`
   Section 3 or `lambda_HiQ/context-handoff.md` Section 4). SIDIS must
   build this propagation from scratch.
3. **z_SIDIS sensitivity through Q² migration.** Although `z_SIDIS =
   P·P_h / P·q` is Lorentz invariant, the binning of z_SIDIS is done in
   bins of reconstructed Q² and x, both of which migrate under the
   electron shift. The proton_HiQ asymmetry-vs-z table
   (`context-handoff-highQ2-proton.md` Section 5.1) does not bin in Q²
   and x simultaneously, so this migration-into-finer-bins effect has
   not been exercised upstream.
4. **A_BC (beam-charge asymmetry).** This observable does not exist at
   all in the proton_HiQ or lambda_HiQ analyses (proton_HiQ data is
   ep0607 only -- single beam charge -- per `proton_HiQ/context-handoff.md`
   table; lambda_HiQ has all four periods but does not form A_BC). The
   beam-charge-symmetric structure of the LAr-electron shift cannot be
   verified upstream; SIDIS must verify it locally.
5. **Per-charge propagation discipline (h+ and h- separately).** The
   SIDIS systematics reference
   (`reference-sidis-systematics.md` lines 355-358 and 521-559
   pointers) requires per-charge shifted multiplicities before forming
   the asymmetry. proton_HiQ does compute h⁺/h⁻ raw asymmetries (Section
   5.1 table), but lambda_HiQ does *not* compute A_h for charged tracks
   at all (it computes Λ/Λ̄ baryon asymmetry, a different observable
   with different cancellation structure). SIDIS must therefore handle
   per-charge propagation itself; the lambda_HiQ baryon-asymmetry
   cancellation conclusions are not transferable to A_h.

## Q7 -- Validated or Provisional status of the upstream procedure

**Status: NEITHER VALIDATED NOR PROVISIONAL -- the procedure is ABSENT.**

- `lambda_HiQ/context-handoff.md` Section 6 lists 5 systematic sources
  that have been quantified for the Lambda asymmetry; LAr electron
  scale and LAr angle shift are **not among them**.
- `lambda_HiQ/docs/plan-highq2-to-final.md` Phase 4 WP6 lists "electron
  theta / energy sensitivity" as an item still to be *screened*; this
  is a Phase 4 deliverable that has not been ticked off.
- `proton_HiQ/docs/context-handoff-highQ2-proton.md` "Open Issues"
  Section 6 does not list electron scale as an open issue at all, but
  it also does not list it as resolved -- the source is simply absent
  from the analysis to date.
- The SIDIS reference catalog
  (`reference-sidis-systematics.md` line 150 column "Executable today")
  labels the high-Q² electron-reconstruction source as
  "**Placeholder**".

**No "validated" stamp exists upstream. No "provisional" treatment
exists upstream. The procedure is missing.**

## Q8 -- Cancellation behavior in A_h and A_BC(charge-summed) per upstream documentation

**Direct upstream finding: NONE (proton_HiQ and lambda_HiQ do not
quantify either cancellation for an LAr electron-scale shift, because
the shift was never executed).**

What the *general systematics framework* in the SIDIS reference asserts
(not an upstream-baryon result):

- `reference-sidis-systematics.md` line 199 ("Charge-Asymmetry
  Cancellation Matrix"): "Electron energy / angle scale | **Yes** |
  Same event-level kinematic migration for h+ and h- ." Classified as
  fully cancelling in A_h at first order.
- Same document line 200 for hadronic scale: "Yes to first order".

These are **expectations**, not measurements. They have not been
verified by proton_HiQ or lambda_HiQ, because the shift was never run.

**For A_BC(charge-summed)**: the proton_HiQ and lambda_HiQ analyses do
not compute A_BC at all (Q6 item 4 above). No upstream finding is
available. The expected cancellation argument is the same -- an LAr
electron-side shift is beam-charge symmetric at the detector level --
but this has not been measured.

**For A_h**: the lambda_HiQ baryon analysis computes Λ/Λ̄ asymmetry, not
A_h(charged) (the charged-particle asymmetry). Their reported numbers
in Section 6 (e.g. A = +0.046 ± 0.018 stat) come from the V0
reconstruction chain, not from charged-track multiplicities. The
electron-scale cancellation seen (or not seen) in Λ/Λ̄ does not
transfer mechanically to the charged-hadron A_h.

**SIDIS must therefore measure both cancellation behaviors itself.**

---

## Items requiring SIDIS-specific extension (would apply even if the procedure existed upstream)

These are items that are demonstrably NOT exercised in proton_HiQ or
lambda_HiQ and therefore would need to be added on top of any reused
procedure:

1. **Propagation into photon-axis pT_h** (Q6 item 1). New
   per-event recomputation: shift LAr `(E_e', θ_e, φ_e)`, recompute
   `q = e_beam - e_scattered`, rotate hadron 3-vectors into the
   `(γ*, p)` Breit-like frame, recompute pT_h. proton_HiQ stores only
   lab-frame `Pt`. New SIDIS code required.
2. **Propagation into φ_h via the Trento lepton-plane normal**
   (Q6 item 2). Recompute φ_h under each LAr variation. proton_HiQ and
   lambda_HiQ do not compute φ_h at all. New SIDIS code required.
3. **Per-charge propagation in A_h** (Q6 item 5). proton_HiQ reports
   raw h⁺/h⁻ asymmetries but never re-derives them under a shifted
   LAr; lambda_HiQ does not address charged A_h. New SIDIS code
   required.
4. **Verification of A_BC cancellation** (Q6 item 4). proton_HiQ has
   only one beam charge in production; lambda_HiQ never builds A_BC.
   SIDIS will need both beam charges available (i.e. the e⁻ recovery
   gate, separately tracked as a hard precondition).
5. **Coupling to per-charge C_rad** (per
   `reference-sidis-systematics.md` line 119): if the C_rad tables are
   recomputed under the shifted electron kinematics, the LAr shift can
   leak into A_h through the per-charge C_rad even though it would
   cancel at the detector level. SIDIS must re-run radiative tables
   under each shift. Upstream baryon analyses use a single C_rad per
   Q²-bin without per-charge separation, so this coupling has never
   been tested.
6. **Re-binning under Q²- and x-migration** (Q6 item 3). The SIDIS
   4D scheme `(Q², x, z_SIDIS, pT_h)` is finer than the 3 high-Q²
   bins used by lambda_HiQ and the 1D z table used by proton_HiQ, so
   bin-migration effects from the LAr shift will be more visible in
   SIDIS than upstream.
7. **Validation against a beam-charge-symmetric null test.** A_BC
   computed in MC under the shift should remain consistent with zero
   (within the shifted statistical band) if the cancellation argument
   holds. This null test has not been performed upstream and will need
   to be performed by SIDIS as part of the closure check.

---

## Gaps to escalate (preventing G11 LAr sub-gate from REUSE-ready declaration)

Every gap below is a blocker. None can be resolved by re-reading the
proton_HiQ or lambda_HiQ documents more carefully -- they are gaps in
the upstream work itself.

| # | Gap | Reason this blocks REUSE-ready | Earliest closure path |
|---|---|---|---|
| G-LAr-1 | No numerical LAr electron-energy-scale shift value chosen upstream | Q1 unanswerable | Open Question OQ3 in SIDIS reference (line 507); requires human analysis-lead decision or H1-internal LAr-calibration document |
| G-LAr-2 | No numerical LAr electron polar-angle-shift value chosen upstream | Q2 unanswerable | Same as G-LAr-1 |
| G-LAr-3 | No application mechanism implemented (no `--sys` flag, no `H1CalcSystematic` setter call, no shifted-MC production) | Q3 unanswerable; nothing to lift | Implementation of a shifted-MC `--sys` variation in either `ProtonTreeBuilderHighQ2` or `TreeBuilder` (high-Q² fork), then a full Condor re-production |
| G-LAr-4 | No `H1CalcSystematic` method signatures available in upstream code | Q4 unanswerable | Read H1OO source under `h1oo-master/H1Calculator/` (explicitly out of scope per task instructions); or wait for upstream implementation |
| G-LAr-5 | No kinematic-dependence binning (flat vs z-impact-binned) chosen | Q5 unanswerable | Same as G-LAr-1 |
| G-LAr-6 | No upstream measurement of A_h cancellation under LAr shift | Q8 unanswerable | Requires the shifted-MC production above plus a per-charge propagation pass |
| G-LAr-7 | No upstream measurement of A_BC cancellation under LAr shift | Q8 unanswerable | Same as G-LAr-6, additionally requires e⁻ recovery (Period/beam-charge gate) |
| G-LAr-8 | Dead `#include` in `lambda_HiQ/src_review/TreeBuilder.C:92` is the only trace of an intent to use `H1CalcSystematic`; no code path consumes it | Confirms absence of implementation | Closing G-LAr-3 will incidentally exercise this include |
| G-LAr-9 | `lar_systematics.py` (planned 8-12h script) does not exist in `lambda_HiQ` | Confirms the procedure has not been driven from the analysis side either | `lambda_HiQ` Phase 4 WP6 to be executed |
| G-LAr-10 | The high-Q² SIDIS plan asserts (`plan-highQ2-sidis-next-steps.md:110`) that the procedure is "solved upstream and must be reused" -- this assertion is false relative to the local upstream state | The G11 task cannot complete as written; either the plan is wrong about upstream status or the upstream artifacts are incomplete and require closure | Escalate to supervisor: G11 must be reformulated as a develop-then-reuse task, not a pure reuse task |

---

## Summary table for downstream consumption

| Item | Value to adopt for SIDIS | Source |
|---|---|---|
| LAr energy-scale shift amplitude | **NOT AVAILABLE** | No upstream value documented (Q1) |
| LAr polar-angle shift amplitude | **NOT AVAILABLE** | No upstream value documented (Q2) |
| Application mechanism | **NOT AVAILABLE in implementation; planning intent is "evls-stage builder re-run via `--sys`"** | Q3; intent documented in `research-highQ2-sidis-extension.md` §4.2 and `reference-sidis-systematics.md` line 319 |
| H1CalcSystematic method handles | **NOT AVAILABLE** | Q4 |
| Kinematic dependence (z-impact binning, etc.) | **NOT AVAILABLE; planning-text expectation: "1-5% across 5 z-impact regions" (not upstream-validated)** | Q5; expectation source `research-highQ2-sidis-extension.md` §4.1 |
| Validation status | **ABSENT (not VALIDATED, not PROVISIONAL)** | Q7 |
| A_h cancellation behavior | **NOT MEASURED upstream; expectation: cancels at first order** | Q8 and `reference-sidis-systematics.md` line 199 |
| A_BC(charge-summed) cancellation behavior | **NOT MEASURED upstream; expectation: cancels at detector level** | Q8 (no upstream A_BC exists) |
| Should the SIDIS analysis numerically inherit the low-Q² SpaCal ±1% energy + ±1 mrad angle values? | **NO -- explicitly prohibited** | `reference-sidis-systematics.md` lines 181, 229; `critic-sidis-systematics-distilled.md` lines 156-157 |

---

## Citations (file paths and line numbers)

- `H1/proton_HiQ/context-handoff.md` (full file, 39 lines): no electron-scale content
- `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` Section 6 "Open Issues" (lines 219-256): no electron-scale entry
- `H1/proton_HiQ/docs/context-handoff-highQ2-proton.md` Section 3 (lines 95-101): tree branch inventory shows lab-frame `Pt`, no `pT_h` photon-axis, no `phi_h`
- `H1/proton_HiQ/docs/plan-highQ2-proton-asymmetry.md` (1274 lines, grepped): no electron-scale variation procedure
- `H1/proton_HiQ/docs/research-sidis-4d-binning.md` lines 780-781: planning-table expected magnitude (1-3%, electron energy scale)
- `H1/proton_HiQ/docs/research-4d-tmd-binning.md` lines 494-535: LAr resolution discussion (not a systematic shift recipe)
- `H1/proton_HiQ/docs/research-sidis-tmd-optimal-binning.md` lines 247-254: LAr resolution discussion
- `H1/proton_HiQ/code_review/ProtonTreeBuilderHighQ2_v2.C` and `teamA_patch/`, `teamB_patch/` variants: no `H1CalcSystematic` include, no `--sys` flag (grep confirms)
- `H1/lambda_HiQ/context-handoff.md` Section 6, "Systematic Uncertainties (5 sources on asymmetry)" (lines 203-208): LAr electron scale absent
- `H1/lambda_HiQ/docs/plan-highq2-to-final.md` Phase 4 WP6 lines 568-577: "electron theta / energy sensitivity" listed as TODO under "Evaluate high-Q2-specific detector / selection systematics"
- `H1/lambda_HiQ/docs/research-script-mapping.md` Section 5.4 lines 304-318: `lar_systematics.py` planned but not implemented; effort 8-12 h
- `H1/lambda_HiQ/docs/research-script-mapping.md` Table "Tier 4" line 372: "Write LAr-specific detector systematics | New | TreeBuilder --sys runs | 8-12 hrs"
- `H1/lambda_HiQ/src_review/TreeBuilder.C` line 92: `#include "H1Calculator/H1CalcSystematic.h"` (dead include; zero downstream usage)
- `H1/sidis_highQ2/docs/references/reference-sidis-systematics.md` line 150: high-Q² catalog source #3 "TBD - to be evaluated locally"; line 223 "Replace the SpaCal-specific electron source with an LAr electron-reconstruction source in high-Q2"; line 229 "Do not transfer the multiplicity-paper SpaCal 1-5% number directly to high-Q2 LAr"; line 319 "Electron-scale variation `electron_scale_shift` ... full-reproduction (Condor-expensive)"; line 507 Open Question 3 "What is the authoritative high-Q2 electron-reconstruction variation procedure and numerical scale in the LAr regime?"
- `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` lines 110, 622, 756, 830: G11 task definition (the task that drove this research note)
- `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md` Sections 1.5, 4.1, 4.2, 4.4 (lines 112-389): forward-looking framework expectations (5 z-impact regions, 1-5% scale, evls-stage re-run cost ~24-48 h per direction); explicitly marked OQ3 "remains unresolved and is a prerequisite for any publication-grade high-Q² systematic budget"
- `H1/sidis_highQ2/docs/research/research-highQ2-status-snapshot-2026-05-19.md` lines 63-65, 437-442, 513-518, 553, 564, 651: independently corroborates that the procedure is undocumented authoritatively and is a top open question

---

## Conclusion

The G11 LAr sub-gate cannot be declared REUSE-ready on the basis of the
local proton_HiQ and lambda_HiQ artifacts. The SIDIS analysis cannot
inherit a numerical procedure that does not exist upstream. The G11 task
as written in `plan-highQ2-sidis-next-steps.md` line 622 ("Extract the
authoritative LAr electron-energy-scale and angle-shift procedure from
the `proton_HiQ` and `lambda_HiQ` high-Q2 analysis chains") rests on a
factual premise (procedure exists upstream) that is contradicted by the
upstream artifacts themselves.

**Recommendation:** Escalate to supervisor. G11 should be reformulated
either as (i) a develop-and-document task that first defines the LAr
procedure locally to SIDIS (e.g. by consulting H1OO source or H1-internal
calibration notes -- both explicitly out of scope for the current
read-only research pass), or (ii) as a coordinated task that drives the
upstream `lambda_HiQ` Phase 4 WP6 to closure first, then lifts the
result.
