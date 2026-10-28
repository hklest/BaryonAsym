<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

# Critique: phi_h Trento Convention Freeze Prep for High-Q2 SIDIS

**Verdict: CONCERN**

The low-Q2 phi_h atan2 formula is implemented in five places with a single
shared core (`compute_pth_phih_z`) and is, on reading, a correct Trento-
convention implementation in the H1 lab frame for both e+p and e-p beams.
Two structural items prevent issuing `SOUND` for the convention freeze:

1. The low-Q2 gen-phi proxy ("use reco SpaCal phi as the gen lepton-plane
   azimuth") is encoded inconsistently across scripts. Two scripts
   silently fall back to `phi_e_gen = 0` when `GenElecPhi` is missing,
   one falls back to `None` (also internally `0`), one uses
   `np.where(isfinite, GenElecPhi, 0.0)` per track, and the data-only
   moment extractor uses no gen at all. None of them currently apply the
   reco-phi proxy as advertised in the handoff. This is a CONCERN because
   the handoff's documented rationale ("RMS 0.104 rad with reco-phi
   proxy") is not what the code actually does.

2. The orientation around the H1 lab z-axis (proton +z, electron -z) is
   embedded silently in the beam constructor (`beam_4vectors`). The sign
   of phi_h is fixed by this choice; flipping the beam sign convention
   would flip phi_h. The current code is internally self-consistent for
   the H1 lab (which is the convention the paper would have to declare),
   but it is not labeled as such in the analysis output JSON metadata.
   This needs to be declared explicitly in G13's frozen-convention
   statement before the high-Q2 result is released, because the high-Q2
   path will be the first one to use stored gen-electron branches and
   any latent sign flip will be invisible until cross-comparison with
   the low-Q2 result.

Beam-charge consistency (item 3 from the task) is OK in the code: every
phi_h site reads `BeamElecE > 0` and builds `k_beam = (0,0,-E_e,E_e)`
unconditionally. This is the right thing for e-p (positron, same -z
direction), so the formula is identical for both beam charges. No
branching is needed.

The Trento formal definition (Bacchetta et al, arXiv:hep-ph/0410050) and
the implemented atan2 agree in sign and orientation under the standard
identity `(q×k_in)×(q×p_h) = ((q×k_in)·p_h) q`, so the sin-component
sign is correct.

---

## Material reviewed

- `H1/sidis_lowQ2/context-handoff.md` (Section 6 "phi_h Gen-Level
  Computation Bug" lines 401-424; Mistakes #7 and #8 lines 718-725;
  OQ9/OQ10 lines 636-637, 676-691; resolution-table lines 447-454).
- `H1/sidis_lowQ2/docs/plans/plan-cahn-boermulders.md` (low-Q2
  azimuthal-analysis plan, full file, esp. Sections 1.1, 1.3, 2.2,
  5.3 P0 ("freeze definitions"), Todo bullets 1 and 2).
- `H1/sidis_highQ2/docs/plans/plan-phih-systematics.md` (full file,
  esp. Sections "Sources/2. Detector systematics" for spacal_angle
  rotation and the `phi_h` shape sensitivity).
- `H1/sidis_highQ2/docs/plans/plan-highQ2-sidis-next-steps.md` (G13
  exit criterion line 258, Todo line 843, dependency rows line 352, 398).
- `H1/sidis_highQ2/docs/research/research-highQ2-sidis-extension.md`
  (Section 4 "Electron Reconstruction: LAr vs SpaCal", Section 4.1 angle
  resolution claim "~1 mrad" for both).
- `H1/sidis_highQ2/docs/research/research-highQ2-status-snapshot-2026-05-19.md`
  (Section 4.4 "Azimuthal moments" lines 279-294, Section 5.2 v5b branch
  additions lines 320-331, Section 6.1 inherited tooling lines 432-435).
- `H1/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md`
  (cross-checked for any frozen public phi_h convention; none — paper
  does not measure phi_h).
- `H1/sidis_highQ2/docs/research/research-h1-charged-particle-analysis.md`
  (cross-checked; no public phi_h convention).
- `H1/sidis_highQ2/scripts/tmd_analysis.py` (lines 61-65, 161-176,
  209-298, 388-389, 460-487, esp. `compute_pth_phih_z` lines 209-255 and
  `gen_scat_electron_from_kin` lines 258-298 — authoritative formula).
- `H1/sidis_highQ2/scripts/tmd_resolution_acceptance.py` (lines 201-263,
  385, 465-481 — same formula, same gen-phi fallback).
- `H1/sidis_highQ2/scripts/extract_azimuthal_moments.py` (lines 189-233,
  240-273, 286-338, 897 — formula and data-only RECO usage; no gen path).
- `H1/sidis_highQ2/scripts/azimuthal_acceptance.py` (lines 6-16, 115-212;
  uses matched-only MC for both numerator and denominator binning).
- `H1/sidis_highQ2/scripts/plot_phih_per_4d_bin.py` (lines 126-322,
  693-708 — JSON `phi_convention: 'Trento (compute_pth_phih_z)'`).
- `H1/sidis_highQ2/scripts/compute_phih_systematics.py` (lines 180-272,
  280-343 — including the `shift_spacal_angle` reasoning that phi_h
  shifts by exactly `-delta_phi` of the SpaCal phi shift).
- `H1/sidis_highQ2/scripts/extract_phi_moments_data.py` (lines 211-244,
  312-318, 352-456, 545-607, 688-740, 893-897 — current production-grade
  extractor; uses `GenElecPhi` directly with fallback to 0.0).
- `H1/sidis_highQ2/scripts/extract_phi_moments_final.py` (lines 6-25,
  432-489 — packaging script; no independent phi_h definition).
- `H1/sidis_highQ2/scripts/plot_corrected_phih.py` (no phi_h definition;
  reads JSON only).
- `H1/sidis_highQ2/scripts/tmd_unfold.py` and `unfold_phih.py` (not yet
  active for phi_h convention work per snapshot; both inherit from
  `compute_pth_phih_z`).
- Formal Trento reference: Bacchetta, Diehl, Goeke, Metz, Mulders,
  Schlegel, arXiv:hep-ph/0410050, Section 2 ("Kinematics") and Eq. (2.4)
  for the standard `phi_h` definition.

---

## Fatal Issues

None at this review depth. The atan2 formula is correct Trento under the
standard identity `(q×k_in)×(q×p_h) ∝ ((q×k_in)·p_h) q`, and the H1
lab z-orientation is internally consistent across all sites.

---

## Major Concerns

### MC-1. Gen-phi fallback is inconsistent across scripts

The handoff's documented resolution ("use reco SpaCal phi as proxy for
gen electron phi; gives RMS 0.104 rad") is **not what any current
script implements**. The actual implementations are:

| Script | Gen lepton-plane azimuth source | Fallback when `GenElecPhi` missing |
|--------|---------------------------------|-------------------------------------|
| `tmd_analysis.py` line 480-485 | `sub['GenElecPhi']` if any finite | `phi_e=None` → defaults to `0` (line 283-284) |
| `tmd_resolution_acceptance.py` line 475-480 | `sub['GenElecPhi']` if any finite | `phi_e=None` → defaults to `0` |
| `compute_phih_systematics.py` line 228-234 | `sub['GenElecPhi']` if any finite | `phi_e=None` → defaults to `0` |
| `extract_phi_moments_data.py` line 372-375, 707-710 | `np.where(isfinite(GenElecPhi), GenElecPhi, 0.0)` per-track | `0.0` (no proxy) |
| `extract_azimuthal_moments.py` | DATA-LIKE, RECO ONLY: no gen path at all (lines 280-338) | N/A |
| `azimuthal_acceptance.py` | uses `phi_h_gen` produced by `tmd_resolution_acceptance` | inherits its `0` fallback |

The handoff's RMS = 0.104 rad bound is based on the proxy approach
(reco SpaCal phi as gen phi). The handoff's `phi_h (buggy gen) = 1.68
rad` is the no-proxy `phi_e_gen = 0` approach. The current code falls
back to `0`, i.e., the buggy approach, whenever `GenElecPhi` is absent
or non-finite. With v5b builders, `GenElecPhi` is present on
`ChargedTot` for matched tracks AND on `ChargedTotGen` for unmatched
gen events, so in v5b output the fallback should never trigger. But:

- If `compute_pth_phih_z` is run on a non-v5b ROOT file (older v3 trees,
  legacy MC, or a partial low-Q2 reprocessing missing the v5b branch
  patch), it will silently revert to the 1.68-rad-RMS broken
  computation. There is no `raise` or warning in any of the three
  central sites.
- The handoff's own statement that the proxy approach "is currently
  used in low-Q2" is therefore obsolete: with v5b, the code uses the
  stored `GenElecPhi` directly; without v5b, it uses 0 (broken). The
  proxy was the diagnostic remediation in the original session but is
  not what landed in the scripts.

For the high-Q2 freeze, this matters because:
- the handoff's 0.104 rad RMS claim does not apply to the actual
  low-Q2 implementation on v5b trees (which uses stored gen),
- the high-Q2 implementation will also use stored gen,
- the regimes match in code path, but neither matches the handoff
  prose. The handoff prose needs to be updated, OR the convention
  statement must explicitly say "gen lepton-plane phi = stored
  `GenElecPhi`, no proxy."

### MC-2. Beam z-direction is implicit, not stamped into outputs

`beam_4vectors` (`tmd_analysis.py` line 161-171) builds
`k_beam = (0, 0, -E_e, E_e)` and `P_beam = (0, 0, +|P_p|, E_p)`
unconditionally. Every phi_h site inherits this. Flipping these two
signs would flip the sign of every `(l × h) · q̂` triple product and
hence the sign of `phi_h`. The JSON metadata emitted by
`plot_phih_per_4d_bin.py` (line 696) declares
`'phi_convention': 'Trento (compute_pth_phih_z)'` but does not declare
**which lab z-orientation** was used. A theory consumer would have to
read `tmd_analysis.py` line 165 to know.

This is a CONCERN, not FATAL, because:
- the H1 lab convention (proton +z, electron -z) is the unique standard
  for H1 papers; no published H1 result uses the opposite,
- the formula will yield the same physical phi_h regardless of which
  sign convention is adopted as long as `k_beam`, `k_scat`, and
  hadron 3-vectors are all transformed consistently,

but it must be declared in the frozen statement before G13 ships, and
the high-Q2 path must not silently change `beam_4vectors`.

### MC-3. Gen-electron phi precision differs e+p vs e-p in non-trivial way

At HERA-II the **proton** beam is invariant across periods (920 GeV +z).
The **lepton** beam is electron in em0405/em06 and positron in
ep0304/ep0607. Direction is the same (-z) in all four periods. So
`beam_4vectors` is correct for both. However:

- in e-p periods the trigger is still S61 (low-Q2) / S67 (high-Q2)
  which are inclusive-DIS triggers; the scattered lepton is a positron
  going forward (low-Q2: SpaCal backward; high-Q2: LAr) in exactly the
  same kinematic region as in e+p. So the lepton-plane azimuth is
  measured by the same calorimeter and same reconstruction code, and
  the phi resolution is identical.
- the **hadron** side has a small charge-dependent CJC efficiency
  modulation. This does not enter phi_h definition but does enter
  phi_h acceptance correction. Out of scope for the convention freeze.
- in e-p periods, the convention `l = k_beam × k_scat` still produces
  the lepton plane in the same sense, because both `k_beam` and
  `k_scat` flip charge sign together (the **direction** is what enters
  the cross product, not the charge). So phi_h is invariant under
  e+p ↔ e-p at fixed kinematic event.

This is a non-blocking confirmation: the formula is beam-charge-blind,
and **no branching on beam charge is required** in any script.

### MC-4. The handoff's "0.43 rad" vs "0.104 rad" residual gap

`plan-cahn-boermulders.md` line 121 quotes "later handoff note quoting
0.43 rad from the full gen-level study", while the handoff resolution
table (`context-handoff.md` line 453) gives 0.104 rad for the
correct-proxy case. These two numbers do not refer to the same
quantity:

- 0.104 rad: gen-reco residual of `phi_h` when reco SpaCal phi is used
  as gen lepton-plane phi (handoff Section 6, 1M-entry RAPGAP31 study).
- 0.43 rad: not located in the v5b-current handoff; this number appears
  only in the cahn-boermulders plan and is unsourced.

This is a documentation gap, not a code bug, but it affects how phi_h
binning width is justified. For the high-Q2 freeze, the equivalent
number needs to be re-measured with LAr (see "Missing Evidence").

---

## Non-Blocking Checks Confirmed

1. **Trento `cos` and `sin` signs are formally correct.** Using the
   identity `(q×k_in)×(q×p_h) = [(q×k_in)·p_h] q`, the code's
   `sin_phi = (l × h) · q̂ / (|l||h|)` reduces to
   `((q×k_in)·p_h) / (|q×k_in||q×p_h|)`, which is the Bacchetta et al
   2004 sin-component up to a positive scalar. The cos-component
   `(l·h)/(|l||h|)` is the standard plane-to-plane cosine. The
   `arctan2(sin, cos)` therefore returns Trento `phi_h ∈ (-π, π]`.
2. **q is computed as `k_beam − k_scat`,** which is the physics
   convention (incoming minus outgoing = virtual photon). All sites
   agree.
3. **`pT_h = |p_h × q̂|`** is the standard photon-axis transverse
   momentum and is independent of phi_h sign.
4. **`z = (P·p_h)/(P·q)`** is the standard photon-axis z. All sites
   agree.
5. **Beam-charge branching is absent in every phi_h script,** which is
   correct because the formula is beam-charge-blind under the H1 lab
   convention.
6. **SpaCal angle systematic** (`shift_spacal_angle` in
   `compute_phih_systematics.py` line 327-343) correctly interprets a
   SpaCal phi shift `delta_phi` as `phi_h → phi_h − delta_phi`. This is
   because the lepton-plane azimuth shifts by `+delta_phi` while the
   hadron-plane azimuth does not, so `phi_h = phi(hadron) − phi(lepton)
   → phi_h − delta_phi`. The sign is the right one for Trento.
7. **`extract_phi_moments_data.py` data path is RECO-ONLY** with
   `BeamElecE`, `BeamProtP` per-event and falls back to defaults only
   if those are non-positive. The H1 lab z-convention is fully
   self-consistent.
8. **No published H1 paper has frozen a phi_h convention,** so there is
   no backward-compatibility constraint to inherit. Bacchetta et al
   Trento is the only authoritative external convention. (Source:
   `research-h1-1302-1321-charged-multiplicity.md` and
   `research-h1-charged-particle-analysis.md` — neither lists a phi_h
   analysis. The 1302.1321 paper measures only `(η*, pT*)` charged-hadron
   multiplicities; no phi_h.)
9. **A_BC variants are integrated over phi_h.** Per
   `plan-highQ2-sidis-next-steps.md` Sections (G13 binning lines 256-260,
   `A_BC` definition lines 306-308) the `A_BC` observables are reported
   in `(Q2, x, z, pT_h)` bins, NOT differentially in phi_h. The integral
   `∫_{-π}^{π} dN/dphi_h dphi_h` is `2π A0` and is convention-blind. So
   phi_h convention drift does not bias `A_BC`. **Confirmed consistent
   with current scope.**
10. **No differential A_BC in phi_h is planned.** The plan documents
    list `(Q2, x, z, pT_h)` binning for `A_BC` everywhere
    (`plan-highQ2-sidis-next-steps.md` lines 44-46, 306-308, 686).

---

## Missing Evidence

### ME-1. LAr azimuthal resolution per event

`research-highQ2-sidis-extension.md` Section 4.1 claims:

> | Angle resolution | ~1 mrad | ~1 mrad (similar with LAr cluster centroid) |

with no source. The 1 mrad SpaCal number for low-Q2 is well established
(SpaCal Technical Note, embedded in H1 standard reconstruction). For
LAr the 1 mrad claim is uncited and likely too optimistic for the
azimuthal coordinate, because:

- The LAr is segmented in eta-φ but with a coarser inter-pad pitch
  than SpaCal, especially in the forward (high-η) wheels.
- LAr electron centroid resolution in φ is typically quoted in H1
  literature as 1-5 mrad depending on z-impact bin (the same z-impact
  binning that drives the 1-5% energy-scale uncertainty per
  `research-highQ2-sidis-extension.md` Section 4.1 table line 336).

This is **PROVISIONAL** in the high-Q2 frozen statement until
confirmed by either (a) the H1 LAr reconstruction technical note,
(b) a measurement in v5b high-Q2 MC of the
`wrap(phi_e_reco − GenElecPhi)` residual RMS, or
(c) the proton_HiQ or lambda_HiQ analysis source that already
characterizes LAr electron φ resolution.

The handoff documentation update from `h1-data-validator` should
include this number for both regimes.

### ME-2. `wrap(phi_h_reco − phi_h_gen)` RMS at high-Q2

The low-Q2 number (handoff line 453: 0.104 rad with stored
`GenElecPhi`) has not been recomputed for high-Q2. The high-Q2
resolution is expected to be **better** than low-Q2 for the lepton-plane
component (LAr is closer to the IP and covers a smaller acceptance
fraction per cluster) but **worse or comparable** for the hadron-plane
component (same CJC). The dominant contribution depends on the photon
direction; at high-Q2 the photon is more forward (smaller theta_q), so
the hadron-plane azimuth is more sensitive to hadron pT direction
errors. This is a quantitative gap that must be filled before G13
closes.

### ME-3. Convention metadata stamping in output JSONs

The `phih_per_4d_bin.json` (`plot_phih_per_4d_bin.py` line 696) writes:

```
'phi_convention': 'Trento (compute_pth_phih_z)',
'gen_acceptance_method': 'matched-only ... using GenElecPhi-based gen
                          virtual photon for the gen lepton plane'
```

But does NOT stamp:
- the H1 lab z-orientation (proton +z, electron/positron -z),
- the explicit formal Trento reference (Bacchetta et al, hep-ph/0410050),
- the per-event beam values used (`BeamElecE`, `BeamProtP` branches),
- the fallback policy when `GenElecPhi` is missing (currently silent →
  `phi_e_gen = 0` → broken),
- the beam-charge applicability (e+p AND e-p with same formula, no
  branching).

For G13 freeze these five items must be present in every output JSON
metadata block, identically.

### ME-4. Closure test of phi_h sign in high-Q2 MC

Before the freeze, a one-time check should be performed: in v5b
high-Q2 MC, compute `phi_h(reco)` and `phi_h(gen)` event-by-event,
plot the 2D distribution, and verify the diagonal is unit slope (not
slope −1). This catches any latent sign flip introduced by a different
high-Q2 builder convention or a different `scatEX/Y/Z/E` sign for the
LAr electron 4-vector vs the SpaCal one. The low-Q2 closure was done
implicitly in the 0.104 rad RMS study, but high-Q2 has not been done.

### ME-5. Update OQ10 resolution language

OQ10 in `context-handoff.md` line 637 ("What phi_h convention should
be frozen for publication?") is currently open. The resolution material
for OQ10 lives across multiple plans and scripts but has not been
collated into a single OQ10 closure note. The frozen-convention
statement at the end of this critique provides the technical content;
the handoff line 637 should be updated to point to it once G13 passes.

---

## Frozen high-Q2 phi_h convention statement

**Status: PROVISIONAL pending ME-1 (LAr azimuthal resolution) and ME-2
(high-Q2 phi_h residual RMS).** All other elements below are ready to
freeze verbatim. The statement is identical in form for low-Q2 and
high-Q2 except for the two clearly-flagged regime-specific lines.

```
phi_h CONVENTION (H1 BaryonAsym SIDIS, low-Q2 and high-Q2):

  Frame:         H1 laboratory frame.
  Beam axis:     proton +z (920 GeV), incoming lepton -z (27.6 GeV).
                 Same convention for e+p (ep0304, ep0607) and e-p
                 (em0405, em06) periods; the formula depends on the
                 lepton DIRECTION, not the lepton CHARGE, and the
                 incoming-lepton direction is -z in all four periods.

  Definition:    Trento convention per Bacchetta, Diehl, Goeke, Metz,
                 Mulders, Schlegel, arXiv:hep-ph/0410050, Eq. (2.4).

  Formula:       q  = k_beam - k_scat              (virtual photon)
                 l  = k_beam × k_scat              (lepton plane normal)
                 h  = q × p_h                      (hadron plane normal)
                 cos(phi_h) = (l · h) / (|l| |h|)
                 sin(phi_h) = ((l × h) · q̂) / (|l| |h|)
                 phi_h      = atan2(sin, cos)  ∈ (-π, π]

                 Range, sign, and orientation follow Trento (counter-
                 clockwise around q̂ viewed from the photon side, with
                 phi_h(p_h ∈ lepton plane, same side as scattered
                 lepton) = 0).

  Gen-level:     Low-Q2 (v5b+):  gen lepton-plane phi from stored
                                 `GenElecPhi` on ChargedTot (matched)
                                 and ChargedTotGen (gen-only). NO
                                 reco-phi proxy is used in v5b code.
                 High-Q2 (v5b+): identical — stored `GenElecPhi` on
                                 ChargedTot and ChargedTotGen.
                 Fallback:       FORBIDDEN. If `GenElecPhi` is missing
                                 or non-finite, the analysis must
                                 raise, not silently substitute zero.
                                 (Current low-Q2 scripts silently fall
                                 back to 0; this MUST be patched before
                                 high-Q2 ships — see Recommended Next
                                 Check.)

  Reco-level:    Reco lepton-plane phi from the measured scattered-
                 electron 4-vector branches `scatEX, scatEY, scatEZ,
                 scatEE`. Low-Q2 reads from SpaCal; high-Q2 reads from
                 LAr cluster centroid. Same code path, same atan2.

  Resolution:    Low-Q2  (SpaCal): wrap(phi_h_reco - phi_h_gen)
                                   RMS = 0.104 rad on RAPGAP31 v3 MC.
                 High-Q2 (LAr):    PROVISIONAL — to be measured in v5b
                                   high-Q2 MC before G13 closes.
                                   Expectation: ≲ 0.15 rad, dominated
                                   by hadron-plane azimuth (CJC).

  Output metadata: every analysis JSON produced by phi_h scripts must
                   stamp:
                     phi_convention      = "Trento (Bacchetta et al
                                            arXiv:hep-ph/0410050)"
                     frame               = "H1 lab, proton +z,
                                            incoming lepton -z"
                     gen_phi_e_source    = "GenElecPhi (ChargedTot or
                                            ChargedTotGen)"
                     gen_phi_e_fallback  = "raise"
                     beam_periods        = explicit list of e+p / e-p
                                           periods included
                     electron_detector   = "SpaCal" (low-Q2) or "LAr"
                                           (high-Q2)
```

If the verdict is `INSUFFICIENT EVIDENCE` for any aspect: the only two
unfinalized items are ME-1 (LAr phi resolution citation) and ME-2
(measured high-Q2 phi_h residual RMS). Both are mechanical to close;
neither blocks the formula or the convention statement itself. They
block only the `Resolution:` numerical line above.

---

## Recommended Next Check

1. **Patch all three central phi_h sites to raise on missing
   `GenElecPhi`** instead of silently falling back to `phi_e_gen = 0`.
   Sites to patch (with exact filenames):
   - `H1/sidis_highQ2/scripts/tmd_analysis.py` lines 480-485
   - `H1/sidis_highQ2/scripts/tmd_resolution_acceptance.py` lines 475-480
   - `H1/sidis_highQ2/scripts/compute_phih_systematics.py` lines 228-234
   - `H1/sidis_highQ2/scripts/extract_phi_moments_data.py` lines 372-375
     and 707-710 (currently uses `np.where(isfinite, ..., 0.0)`; change
     to `if not np.all(isfinite(GenElecPhi)): raise`).
   - `H1/sidis_highQ2/scripts/azimuthal_acceptance.py` (no direct site;
     inherits via `tmd_resolution_acceptance`).

2. **Add the convention-metadata block** described in the freeze
   statement to every script that writes a phi_h JSON:
   - `H1/sidis_highQ2/scripts/plot_phih_per_4d_bin.py` line 692-712
   - `H1/sidis_highQ2/scripts/extract_phi_moments_data.py` line 1316-1350
   - `H1/sidis_highQ2/scripts/extract_phi_moments_final.py` (packaging
     output)
   - `H1/sidis_highQ2/scripts/azimuthal_acceptance.py` line 687-700
   - `H1/sidis_highQ2/scripts/compute_phih_systematics.py` line 735-755
   - any new high-Q2 driver to be written under G13.

3. **Run a one-time closure** on v5b high-Q2 MC to (a) measure
   `wrap(phi_h_reco − phi_h_gen)` RMS for ME-2, and (b) verify the
   `phi_h_reco` vs `phi_h_gen` 2D distribution has unit-slope diagonal
   (no sign flip vs low-Q2). Same script pattern as
   `tmd_resolution_acceptance.py` lines 1186-1267.

4. **Update handoff Section 6 lines 401-417** to reflect that the
   reco-phi proxy is no longer used in v5b code (stored `GenElecPhi`
   replaces it). Add a note that the low-Q2 publication-grade
   acceptance was always intended to use stored `GenElecPhi`, and the
   reco-phi proxy was a diagnostic-stage workaround that should not be
   cited as the production method.

5. **Resolve OQ10 in `context-handoff.md` line 637** by pointing to this
   frozen-convention statement once ME-1 and ME-2 are closed. OQ9 (line
   636) is already resolved by v5b but the handoff text has not been
   updated.

6. **Cross-script consistency test**: pick 1000 random matched tracks
   from a v5b RAPGAP file (low-Q2 or high-Q2), compute phi_h with
   `tmd_analysis.compute_pth_phih_z`, with
   `extract_phi_moments_data.compute_pth_phih_z`, and with
   `extract_azimuthal_moments.compute_pth_phih_z`, and verify bit-for-
   bit agreement. The three functions are textually identical but
   should be confirmed identical at runtime. This is a 30-line test;
   add to `H1/sidis_lowQ2/scripts/` as `test_phih_consistency.py`.
