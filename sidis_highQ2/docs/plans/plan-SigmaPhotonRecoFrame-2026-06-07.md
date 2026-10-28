# Plan: pure-Sigma photon reco-frame study for high-Q2 charged SIDIS

Status: **PREPARATORY ONLY**. Do not submit any production jobs while
the `ESigmaPhotonRecoFrame_nominalLabAcceptance` workflow is still
running. Pure-Sigma migration builder is **BLOCKED**; binned
resolution diagnostic is implementable as a comparison harness.

Author dispatch: h1-physics-analyst (prep) + h1-script-runner (prep);
both invoked under standalone-script-implementer authority for
preparatory artifacts only. Production handoff goes to h1-naf-operator
when authorized.

---

## 1. Scope and contract

User request: "do the same thing with the purely sigma method [as the
just-finished `ESigmaPhotonRecoFrame_nominalLabAcceptance` variation],
also do it in our different kinematic bins of x and q2, run over some
more MC to have more stats for that study."

Interpreted as TWO deliverables:

D1. A pure-Sigma migration builder analogous to
    `build_migration_th2_per_charge_v4_ESigmaPhotonRecoFrame.py`,
    using a pure-Sigma reco-side q-vector inside the SIDIS-frame
    `(pT_h, z)` rebuild only. Nominal lab acceptance and gen-side
    rebuild unchanged. Output guard requires substring
    `SigmaPhotonRecoFrame` (NOT `ESigmaPhotonRecoFrame`).

D2. A binned photon-resolution diagnostic over the analysis
    `(Q2, x)` cells (v3 binning) comparing the electron-method,
    eSigma joint-solve, and (where physically valid) pure-Sigma
    reco-frame photon-axis `(theta_gamma, E_gamma)` residuals
    against the gen-truth photon, with higher MC statistics than
    the existing single-file login-node diagnostic, run via Condor
    and packaged for local PDF rendering.

## 2. Pure-Sigma kinematics: canonical definition

From H1OO `H1Calculator/H1CalcKine.C::CallSig()` (verified locally in
`H1/h1oo-master/H1Calculator/H1CalcKine.C` lines 833-849):

    void H1CalcKine::CallSig() {
        const TLorentzVector& elec = Elec->GetFirstElectron();
        const TLorentzVector& had  = Fs->GetAllFsLessElectron();
        fMakeKine->MakeSig(elec.E(), elec.Theta(),
                           had.E() - had.Pz(),       // sigma_h
                           EBEAM, PBEAM);
        fQ2s = fMakeKine->GetQ2s();
        fYs  = fMakeKine->GetYs();
        fXs  = fMakeKine->GetXs();
    }

The pure-Sigma method therefore requires three reco-side inputs:
  - measured scattered-electron energy E'_e and polar angle theta_e;
  - the hadronic-only Sigma `sigma_h = sum_{i in HFS} (E_i - Pz_i)`,
    NOT the full-event E - Pz.
It produces `(Q2_pureSigma, Y_pureSigma, X_pureSigma)` only. It does
NOT produce a Sigma-method scattered-electron 4-vector or a Sigma
azimuth. Any virtual-photon q-vector requires an additional choice.

Standard textbook Sigma formulas (cross-check):
  - Y_Sigma = sigma_h / (sigma_h + E'_e (1 - cos theta_e))
  - Q2_Sigma = E'_e^2 sin^2(theta_e) / (1 - Y_Sigma)
  - X_Sigma = Q2_Sigma / (s * Y_Sigma)
where `s` is the CM-energy squared. Then a Sigma-method scattered
electron can be solved algebraically from `(Q2_Sigma, Y_Sigma)`
exactly as the eSigma builder does for `(Q2es, Yes)`:
        A_S = Q2_Sigma / (2 E_e_beam)
        B_S = 2 E_e_beam * (1 - Y_Sigma)
        Eprime_Sigma   = 0.5 * (A_S + B_S)
        cos_theta_Sigma = clip((A_S - B_S) / (A_S + B_S), -1, 1)

## 3. Blocker: pure Sigma is NOT implementable from current v6 sidistrees

### 3a. Branch audit (verified on
`output_v6/central/sidistree/mc/rapgap31/7071/sidistree_rapgap31_7071_em06_0000.root`
via TTree::GetListOfBranches on `ChargedTot`):

DIS-kinematics branches actually present:
  reco: `Q2e, Ye, Xe`, `Q2da, Yda, Xda`, `Q2es, Yes, Xes`,
        `Q2h, Yh, Xh`, `Q2s, Ys, xBj, Ws`, `Empz`
  gen : `Q2sGen, YsGen, xGen`, `Q2_es_gen, Y_es_gen, x_es_gen`,
        `Q2eGen, YeGen, ...`

### 3b. The branches named `Q2s, Ys` are NOT pure Sigma.

`src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 427-429, 509-512,
1028-1056 (verified): in the high-Q2 regime `Q2s` and `Ys` are
**HAT pointers** filled by `stage_evls` upstream from
`H1BaryonAsym_HighQ2`, where they are the **upstream-calibrated
eSigma values** (NOT pure Sigma). The builder explicitly comments:

    // DIS kinematics (calibrated eSigma values from upstream stage_evls)
    H1FloatPtr hatQ2s("Q2s");
    H1FloatPtr hatYs("Ys");
    ...
    // Q2s/Ys above stay as upstream-calibrated eSigma values (high-Q2
    // regime-specific; do NOT overwrite).

`H1CalcKine::GetQ2s() / GetYs() / GetXs()` are NEVER called in
`ChargedSidisTreeBuilderHighQ2_v6.C` (verified by grep: zero
matches). The genuine pure-Sigma values are therefore not written
anywhere in the v6 ChargedTot/DisTot trees.

### 3c. There is no usable independent `sigma_h` branch.

The only event-level E-pz quantity on the tree is `Empz`, computed
at line 1172-1175 as:

    elecContrib = E'_e * (1 - cos theta_e)
    sigmaH      = (Ys * elecContrib) / (1 - Ys)         (when 0<Ys<1)
    Empz        = sigmaH + elecContrib

i.e. `sigma_h` is algebraically reconstructed FROM the eSigma-
calibrated `Ys`. Solving back for sigma_h and feeding it into
`MakeSig(..., sigma_h, ...)` would just re-derive `Ys` and therefore
yields nothing different from eSigma. This is an algebraic identity,
not a physically independent Sigma measurement.

No HFS 3-vector branches (no `HfsPx, HfsPy, HfsPz, HfsE`, no per-
particle HFS list, no `HfsEpz`) are present on the v6 sidistree.

### 3d. Gen side IS usable (cross-check only).

The branch `Q2sGen, YsGen, xGen` is filled at lines 942-... from
HAT pointers `Q2sGen / YsGen`, which on the GEN side are the
true Sigma-method values evaluated at the generator final state
(consistent with `Fs->GetAllFsLessElectronGen()` -> generator HFS).
These are usable as a gen-side cross-check, but the reco match
they would be compared to does NOT exist.

### 3e. Decision

A migration builder named
`build_migration_th2_per_charge_v4_SigmaPhotonRecoFrame.py` built
**only from current v6 branches** would either:
  (i)  be algebraically identical to the eSigma variation (if it
       re-uses Q2s/Ys = HAT eSigma calibrated), in which case it is
       deceptively named -- DO NOT CREATE; OR
  (ii) require a hybrid construction (e.g. (Q2s_HAT, Yes), or
       (Q2da, Yda), or (Q2h, Yh) etc.), which is not pure Sigma --
       DO NOT NAME IT `SigmaPhotonRecoFrame`.

==> **D1 is BLOCKED**. To implement a genuine pure-Sigma migration
    builder the v6 stage-2 tree builder must FIRST be extended to
    persist the genuine `H1CalcKine` pure-Sigma values as new
    branches:

        Float_t Q2sPure = kine->GetQ2s();
        Float_t YsPure  = kine->GetYs();
        Float_t XsPure  = kine->GetXs();

    and analogous gen-side branches `Q2sPureGen, YsPureGen,
    XsPureGen` from `H1CalcKine::GetQ2sGen()` etc. The branch names
    `Q2sPure / YsPure / XsPure` are chosen to be disjoint from
    `Q2s/Ys/xBj` (which must remain the upstream-calibrated eSigma
    values for v4/v6 selection compatibility).

    Builder change owner: `h1-builder-engineer`. Required scope is
    purely additive (new branches, no deletion), so this is a v6
    sidecar systematic rather than a central re-production -- but
    it still needs a smoke-test pass and either a re-production of
    one MC subrun under a `_sigmaPure` sidecar tag, OR a one-off
    rebuilt small MC sample for the SigmaPhotonRecoFrame study
    only, kept entirely under
    `$WORK/output_v6/variations/sigmaPure_branch_addition/`.

## 4. Hybrid alternative (NOT pure Sigma; only on explicit request)

If the user accepts a hybrid that is NOT named "pure Sigma",
candidate hybrids that ARE implementable from current v6 branches:

  H-A. "SigmaHatPhotonRecoFrame": use the upstream-calibrated
       (Q2s_HAT, Ys_HAT, Xs_HAT==xBj) as the reco Q2,y inputs to
       the same eSigma-style joint solve (Eprime, theta) +
       measured-electron phi. Because in high-Q2 the HAT pointers
       were eSigma-calibrated upstream, this is essentially a
       different *calibration source* for the same eSigma method;
       it is NOT a different DIS method.

  H-B. "DAPhotonRecoFrame":  use (Q2da, Yda) (double-angle), same
       joint solve. Genuinely independent reco DIS method; well
       defined; output guard substring `DAPhotonRecoFrame`. This is
       the easiest *non-eSigma* cross-check available without a
       builder change.

  H-C. "JBPhotonRecoFrame" / "HadronPhotonRecoFrame": use
       (Q2h, Yh) (Jacquet-Blondel / hadron-only) with same joint
       solve. Also genuinely independent.

If a true Sigma cross-check is needed BEFORE the v6 builder
addition lands, H-B (DA) is the recommended substitute (independent
of the electron-energy calorimeter calibration). It must be
explicitly labelled `DAPhotonRecoFrame`, not `SigmaPhotonRecoFrame`,
in all output paths, output filenames, and method-definition
strings.

## 5. Implementable now: D2 binned photon-resolution diagnostic

A higher-statistics binned diagnostic comparing the three
implementable reco methods against the gen-truth photon, in the
v4/v3 `(Q2, x)` cells. Pure-Sigma can only enter as a gen-side
overlay (using `Q2sGen, YsGen` to define a gen-truth Sigma photon
for comparison) -- it cannot enter on the reco side.

Outline:
- Inputs: v6 central sidistree MC, both rapgap31 and djangoh14, all
  available subdirs (rapgap31: 7071, 7094, 7096, 7097; djangoh14:
  7072, 7091, 7092, 7093 -- confirmed via `ls`).
- Per (Q2_cell, x_cell) from `binning_v3.py::cell_index_reco(Q2es, Xes)`,
  accumulate per-event arrays of:
    theta_gamma_elec - theta_gamma_gen
    theta_gamma_eSigma - theta_gamma_gen
    theta_gamma_DA     - theta_gamma_gen        (cross-check, NOT pure Sigma)
    (E_gamma_elec - E_gamma_gen) / E_gamma_gen
    (E_gamma_eSigma - E_gamma_gen) / E_gamma_gen
    (E_gamma_DA     - E_gamma_gen) / E_gamma_gen
- Gen-truth photon as in existing diagnostic
  `compare_photon_resolution_ESigmaPhotonRecoFrame.py` lines 189-211:
    Eb_eff = BeamElecE - max(GenEnPhoton, 0)
    q_gen  = (0,0,-Eb_eff,Eb_eff) - (GenElecE * dir(GenElecTheta,GenElecPhi))
- For each cell write robust per-method summary stats: n, mean,
  median, rms, MAD, p16, p84, central68, p2.5, p97.5.
- Output: per-Condor-job JSON + CSV, designed to be merged by a
  lightweight reducer (sum of stats containers) before a final PDF
  render on the laptop.

Execution policy (HONORED):
- single MC file run on login node ALLOWED as a smoke test
  (<5 min, 1 file). Not authorized in this dispatch.
- multi-file production over all 4 subdirs * O(10) files per
  generator goes to CONDOR, NOT login node. One job per input
  file (embarrassingly parallel). Submission held until the user
  authorizes after the current eSigma workflow finishes.

Output layout (unique, isolated from running production):
    $WORK/output_v6/variations/photon_resolution_binned_Q2x_SigmaPhotonRecoFrame/
        per_file/
            <gen>_<subrun>_<seq>.json
            <gen>_<subrun>_<seq>.csv
        merged/
            summary_<gen>.json
            summary_<gen>.csv
        pdf/         (rendered locally, not on NAF)
    $WORK/condor_v6/variations/photon_resolution_binned_Q2x_SigmaPhotonRecoFrame/
        submit_photon_resolution_binned.condor
        wrap_photon_resolution_binned.sh
        args.txt

Naming guard: every script enforces that --outdir contains the
substring `photon_resolution_binned_Q2x` AND that it lives under
`output_v6/variations/` so it cannot accidentally land on top of
the running `ESigmaPhotonRecoFrame_nominalLabAcceptance` outputs.

## 6. Artifacts prepared in this dispatch

  - `plan-SigmaPhotonRecoFrame.md`  (this file; local copy at
    `/var/folders/.../sigma_prep/`, will be staged into
    `$WORK/scripts/unfolding/tunfold_official_v4_percharge/` under
    a clearly preparatory filename).
  - `compare_photon_resolution_binned_Q2x.py` (binned diagnostic
    implementation, isolated under `scripts/diagnostics/`).
  - NO migration builder created. Creating
    `build_migration_th2_per_charge_v4_SigmaPhotonRecoFrame.py`
    requires the v6 builder addition described in §3e; see
    `h1-builder-engineer` handoff.

## 7. Next-step submission sequence (HELD until authorized)

After the eSigma migration/merge/plot pipeline completes AND the user
authorizes pure-Sigma work:

  S1. Decide between (a) DA cross-check now, vs (b) wait for v6
      builder addition for genuine pure Sigma. h1-physics-analyst /
      h1-analysis-critic to recommend.
  S2. If (a): adapt diagnostic to add DA arm; dispatch to
      h1-naf-operator for Condor submission of the binned
      diagnostic over all 8 subrun subdirs.
  S3. If (b): dispatch h1-builder-engineer to add Q2sPure/YsPure
      branches, smoke-test, build one MC sidecar subrun, then run
      the diagnostic + a SigmaPhotonRecoFrame migration builder.

Do not run any of S1-S3 in this dispatch.
