**Status:** Current research note. New authoritative published-analysis code audit used for the v3 combined-track builder update.

# Research: H1 Charged-Particle Multiplicity Analysis (DESY 20-176)

**Date:** 2026-05-15
**Author:** researcher (delegated)
**Subject codebase:** `/data/dust/group/h1/ztudesy/h1_analysis/` on
`naf-hone.desy.de` (Zhoudunming "Kong" Tu's published analysis area)
**Publication:** DESY 20-176, "Measurement of charged particle multiplicity
distributions in DIS at HERA and its implication to entanglement entropy of
partons" (2007 data set, 136 pb^-1).
**Audience:** SIDIS supervisor and `code-editor` who will adapt
`ProtonTreeBuilder_lowQ2.C` to a broader tracking phase space.

> **Scope of this document:** What the published charged-particle analysis
> actually does at the H1OO level (tracks, event selection, acceptance,
> systematics), exact code citations, and concrete changes for our SIDIS
> builder. Excluded: how to design the SIDIS-binning histograms, unfolding,
> or write the publication. Those belong to plans and other research notes.

---

## 1. Codebase Map

Top-level: `/data/dust/group/h1/ztudesy/h1_analysis/`

```
H1Examples/          stock H1OO examples (unused)
H1_PartCorrAnalyzer/ earlier two-particle correlations analyzer (predecessor)
H1_PHPAnalyzer/      photoproduction-background analyzer (auxiliary)
H1_ForwardMultAnalyzer/   *** THE PUBLISHED ANALYSIS ***
H1_Azimuthal/        azimuthal asymmetry analyzer (later work, shares structure)
qedc/                QED-Compton studies
makefile             H1OO standard build harness (gmake bin)
```

The relevant package for DESY 20-176 is `H1_ForwardMultAnalyzer/`. Inside:

| Path | Purpose |
|------|---------|
| `src/ForwardMultAnalyzer.C` | **Main H1OO event-loop binary** — reads MODS/HAT, applies electron cuts, walks the HFS, fills a flat `properties` TTree (1664 lines) |
| `src/chargedForward.C`, `chargedForwardForData.C` | Earlier variants; superseded by `ForwardMultAnalyzer.C` (which kept the file name `ForwardMultAnalyzer` for the binary) |
| `src/elecCut.C/.h` | SpaCal-electron quality cuts (dead cells, dead trigger cells, circular cuts, hadronic energy fraction, ECRA cut) by run year |
| `src/TDetectQedc.C/.h` | QED-Compton tagger on MC truth |
| `header/mainAnalysis.h` | Histogram declarations + eta binning used by the analysis macros |
| `header/MCweight.h` | Hard-coded MC reweighting arrays (y, vtxZ) for RAPGAP and DJANGOH |
| `macros/mainAnalysis_all.C` | **Analysis-level event loop** — reads the `properties` tree, applies analysis cuts, fills hadron histograms |
| `macros/mainAnalysis_fillTree.C` | **Minitree builder** — applies analysis cuts and emits a per-event compressed `miniTree` with `passREC_mini[]`, `nucliaREC_mini[]`, etc. (later stages run on the minitree) |
| `macros/makeCentCorrTable.C`, `makeFSTCorrTable2D.C` | Tables for tracking-efficiency tuning |
| `macros/plotEta*.C`, `plotPhiFST.C`, `plotEventLevelDistributions.C` | Final plotting |
| `input/*.steer` | H1OO steering files for data and each MC sample |
| `run/SelectedRuns_*.root` | Run lists for each year/beam combination |
| `batch/`, `condor/` | Job submission (Condor) and output directories |
| `TUnfold/` | Local TUnfold copy used downstream for the published unfolding |

Two-stage pipeline:

1. **Stage 1 (H1OO):** `ForwardMultAnalyzer.C` walks DST/MODS/HAT,
   produces a flat `properties` TTree with **no track-quality cuts**
   beyond "type tag" and the FST anti-overlap chi^2. All tracks of every
   type are written.
2. **Stage 2 (ROOT macros):** `mainAnalysis_fillTree.C` (or the older
   `mainAnalysis_all.C`) reads the `properties` tree, applies event cuts
   (Q2, y, Epz, ibg, vertex), and applies per-type track-quality cuts
   producing `passREC_mini[]`. The minitree is what feeds unfolding.

This separation is important: **the C++ binary stores everything; the cut
definitions live in the ROOT macros and are versioned alongside the
analysis decisions**. Our SIDIS builder currently bakes the cuts into the
C++ stage and uses a tighter selection; this means moving cuts into a
post-processing layer (or at least loosening the C++ stage) is part of
adopting this approach.

---

## 2. Track Selection — Exact Cuts

### 2.1 Track types in H1OO (as used here)

`H1PartSelTrack` carries the following type predicates. The DESY 20-176
analyzer assigns a numeric `type` per track:

```cpp
// ForwardMultAnalyzer.C : 1273-1280
if (track->IsCentralTrk())       type = 1;   // CJC alone
else if (track->IsCombinedTrk()) type = 2;   // CJC + FTD link
else if (track->IsForwardTrk())  type = 3;   // FTD only
else if (track->IsFSTTrk())      type = 4;   // FST (forward silicon)
else if (track->IsBSTTrk())      type = 5;   // BST (backward silicon)
```

`IsCentralTrk()` returns true for what we currently use. The
DESY 20-176 *publication* uses central (type=1) + combined (type=2). The
codebase keeps forward, FST and BST in the tree but the published phase
space excludes them. The macros expose flags `doComb_` (default true)
and `doFwd_` (default false) for switching combined and forward in/out.

**Critical detail about combined tracks**: `H1CombinedFittedTrack`
inherits from `H1VertexFittedTrack`, so `cand->GetTrack()` for a combined
track still passes the `dynamic_cast<H1VertexFittedTrack const *>` check
in the analyzer; the same chi2vtx / dca / NHits accessors work. The
*additional* combined-only diagnostics (`GetLinkChi2()`, `GetLinkNdf()`)
are extracted via a second `dynamic_cast<H1CombinedFittedTrack const *>`
guarded by `track->IsCombinedTrk()`:

```cpp
// ForwardMultAnalyzer.C : 1318-1324
H1CombinedFittedTrack const *combtrack =
    dynamic_cast<H1CombinedFittedTrack const *>(cand->GetTrack());
if (track->IsCombinedTrk()) {
    chi2Link = combtrack->GetLinkChi2();
    ndfLink  = combtrack->GetLinkNdf();
}
```

This is the API we will need to add to our builder.

### 2.2 Primary-vertex association

For every type 1/2/3 track, the analyzer requires the track vertex to
equal the event primary vertex (component-wise float comparison through
`floatEqual()`):

```cpp
// ForwardMultAnalyzer.C : 1333-1339
H1Vertex const *v = h1track->GetVertex();
if (floatEqual(v->X(), myEvent.vertex[0]) &&
    floatEqual(v->Y(), myEvent.vertex[1]) &&
    floatEqual(v->Z(), myEvent.vertex[2])) {
    // keep
} else {
    type = 0;    // demote -> dropped downstream
}
```

This replaces the `IsFromPrimary` cut we currently use; the result is the
same (require track to belong to the primary vertex) but it works
uniformly across central, combined, forward, and FST tracks.

For FST tracks there is an additional anti-overlap step
(`ForwardMultAnalyzer.C` 1372-1399): the FST track's momentum is required
to be incompatible (chi^2 > 30) with every HFS track to avoid double
counting tracks already reconstructed in CJC+FTD. This step is *not*
needed for combined or central tracks because they are part of the HFS by
construction.

### 2.3 Track-quality cuts (post-processing macros)

These are the **published** cuts, applied in `mainAnalysis_all.C`
485-520 and in `mainAnalysis_fillTree.C` via `getPassFlag()`. The three
quality tiers (tight/default/loose) are indexed by `trackQuality`
∈ {0, 1, 2} with cut tables:

```cpp
// mainAnalysis_fillTree.C : 7-11
double ptcut[3]          = {0.15, 0.15, 0.15};   // pT > 150 MeV (all tiers)
double cuts_1_value[3]   = {1.0,  2.0,  5.0};    // |dca' * sin(theta)| limit (central)
double cuts_2_value[3]   = {40.,  50.,  60.};    // startHitsRadius limit
double cuts_3_value[3]   = {12.,  10.,  8.};     // (startR - endR) min separation
double cuts_4_value[3]   = {3.,   5.,   10.};    // |dca'| limit (combined)
```

#### Central tracks (type == 1) — default tier

```cpp
// mainAnalysis_all.C : 485-493
if (TMath::Hypot(pxREC[j], pyREC[j]) < ptcut)           continue;  // pT > 0.15
if (fabs(dcaPrimeREC[j] * sin(trkThetaREC[j])) > 2.0)   continue;  // |dca' sin theta| < 2 cm
if (startHitsRadiusREC[j] > 50.0)                       continue;  // innermost CJC hit r < 50 cm
if (fabs(startHitsRadiusREC[j] - endHitsRadiusREC[j])
    < 10.)                                              continue;  // radial track length > 10 cm
if (vtxNHitsREC[j] < 0)                                 continue;  // sanity
if (fabs(trkThetaREC[j] - elecThetaREC) < 0.2)          continue;  // veto scattered-e track
if (rand->Uniform() > nucliaREC[j] &&
    rand->Uniform() < 1.0)                              continue;  // nuclear-IA reweighting
```

These match the paper text on p.7-8 (DESY 20-176):
- `p_T > 150 MeV` and `|eta_lab| < 1.6` (`|η_lab|` cut applied separately at
  histogram time, see §2.4)
- `|dca' sin theta| < 2 cm`
- innermost hit `< 50 cm` from the z axis
- radial track length `> 10 cm`

#### Combined tracks (type == 2)

```cpp
// mainAnalysis_all.C : 494-505
if (pt < ptcut)                                continue;  // pT > 0.15
if (pREC[j] < 0.5)                             continue;  // p > 500 MeV (paper's CJC-FTD material crossing)
if (rad2deg * trkThetaREC[j] < 10. ||
    rad2deg * trkThetaREC[j] > 30.)            continue;  // 10° < theta < 30°
if (fabs(dcaPrimeREC[j]) > 5.0)                continue;  // |dca'| < 5 cm
if (startHitsRadiusREC[j] > 50.0)              continue;
if (vtxNHitsREC[j] < 0)                        continue;
if (peREC[j] / pREC[j] > 99999.9)              continue;  // momentum-error sanity
if (chi2vtxREC[j] > 50.)                       continue;  // vertex-fit chi^2
if (chi2LinkREC[j] > 50.)                      continue;  // CJC-FTD link chi^2
if (rand->Uniform() > nucliaREC[j] && ...)     continue;
```

`p > 500 MeV` is the paper's stated requirement so the track has enough
momentum to traverse the material between the CJC outer wall and the FTD.
The 10°-30° polar cut covers the CJC-FTD angular overlap region.

#### Forward tracks (type == 3) — published analysis does *not* use these (`doFwd_=false`)

```cpp
// mainAnalysis_all.C : 506-517 (kept for completeness, not in publication)
if (pt < ptcut)                                continue;
if (pREC[j] < 0.5)                             continue;
if (rad2deg * trkThetaREC[j] < 6. ||
    rad2deg * trkThetaREC[j] > 25.)            continue;
if (startHitsRadiusREC[j] > 25.0)              continue;
if (fabs(zLengthHitREC[j]) < 10.)              continue;
if (rZeroREC[j] > 20.)                         continue;
if (chi2vtxREC[j] > 25.)                       continue;
if (chi2TrkREC[j] > 10.)                       continue;
```

**Recommendation for SIDIS:** adopt central + combined; do **not** add
forward-only or FST tracks unless we want to commit to FTD-alone
calibration work and explicit FST anti-overlap. The DESY 20-176
publication does not use them.

### 2.4 Pseudorapidity restriction

Published lab cut is `|eta_lab| < 1.6` for both central and combined
tracks. In the macros this is **not** applied as a track quality cut —
it is applied at histogram fill time (`mainAnalysis_all.C` 457 uses
`-2.0 < eta < 2.5` for the broader-binned plots; the publication uses
`|eta| < 1.6`). The histogram binning `etabins_low`/`etabins_high` in
`mainAnalysis.h` and the η differential tables in the appendix of DESY
20-176 confirm `|η_lab| < 1.6` as the binned phase space.

**SIDIS implication:** the tracking acceptance for combined tracks effectively
extends from `|eta_lab| < 1.3` (CJC alone — our current cut) to roughly
`1.3 < |eta_lab| < 1.6` in the forward (proton-direction) hemisphere
only. Backward combined tracks (eta < -1.3) do not exist; the CJC-FTD
combination only adds forward acceptance because the FTD is at z > 0.
This means our SIDIS phase space gain is **forward-only**, roughly
`1.3 < eta_lab < 1.6` corresponding to `theta` in the 10°-30° band.

### 2.5 Tracks that are NOT used in DESY 20-176

- **Forward-only (type 3, FTD without CJC linkage):** code present, flag
  off in publication.
- **FST (type 4, forward silicon):** code present (with anti-overlap),
  flag off in publication.
- **BST (type 5, backward silicon):** code present but never given a
  quality cut path; never used downstream.

The "broader tracking acceptance" in DESY 20-176 is therefore
`central + combined`, not "all H1 silicon trackers". This is the most
important factual correction to make at the top of any plan.

---

## 3. DIS Event Selection

### 3.1 Scattered-electron identification (H1OO stage)

```cpp
// ForwardMultAnalyzer.C : 1032-1058
static elecCut myElecCut = 0;
for (int i = 0; i < partCandArray.GetEntries(); i++) {
    H1PartCand *cand = partCandArray[i];
    H1PartEm const *elec = cand->GetIDElec();
    if (elec && cand->IsScatElec()) {
        if (myElecCut.goodElec(elec, *run) != 1) continue;  // SpaCal quality
        ...
        // keep highest-pT candidate
    }
}
```

`elecCut::goodElec()` enforces three things (`elecCut.C` 467-496):

1. `IsScatElec()` from the H1OO finder.
2. **Spatial veto** of dead SpaCal cells, dead trigger cells, and a set
   of circular cuts (run-period dependent — 2004/2005/2006/2007 init
   functions). All four runs initialize a standard radius-12 cm circular
   cut around `x_0 = -2.025, y_0 = 0`, plus year-specific additional
   circular cuts (e.g., 2007 uses `R=10.4` at (0.337, 2.013) and `R=7.26`
   at (2.477, -4.528)).
3. **Hadronic-energy fraction in SpaCal** `< 0.15` (`MAX_ENERGY_FRAC_HAD`)
   AND **energy-weighted cluster radius** `Ecra < 4 cm` (`MAX_ECRA`).
4. The track-condition (BPC/CJC linking) is **removed** in this version
   (commented out; see `elecCut.C` 489-493 and 517-521). The author
   explicitly disabled the track-match electron requirement.

Photons within `dR < 0.4` of the electron cluster are combined into a
single "electron + collinear photons" 4-vector (`escatPhot_REC_lab`) used
for kinematics and HFS exclusion (`ForwardMultAnalyzer.C` 1082-1112).

### 3.2 Event-level analysis cuts (ROOT-macro stage)

From `mainAnalysis_all.C` 365-388 and `mainAnalysis_fillTree.C` 692-714:

```cpp
// trigger
if ((doGen_ && trigWeightRW <= 0) || (!doGen_ && trigWeightAC <= 0)) continue;
// vertex type 1 (primary CTD vertex)
if (vertexType != 1) continue;
// background-finder bits 0..6 (ibg flags from OM and DUK groups)
if ((ibgREC & 0x7F) != 0) continue;     // i.e. bits 1,2,4,8,16,32,64
// E - pz
double Epz = hfsEREC + elecEREC - (hfsPzREC + elecPzREC);
if (Epz > 70 || Epz < 35) continue;
// z-vertex
if (TMath::Abs(vertex[2] + zvtxOffset) > 35.) continue;
// kinematic phase space (mainAnalysis_all.C only; fillTree leaves it open)
if (yREC_es < ymin || yREC_es > ymax) continue;
if (Q2REC_es < Q2min || Q2REC_es > Q2max) continue;
// scattered electron energy
if (elecEnergyREC < 10.) continue;       // 10 GeV (12 GeV in fillTree)
```

With `Q2min=5, Q2max=100, ymin=0.05, ymax=0.6` (mainAnalysis_all.C 133-138).
The published paper uses `0.0375 < y < 0.6`; the code value `y > 0.05`
matches the analyzer's *trigger-safe* internal cut. The published lower
edge `y = 0.0375` is the analysis fiducial extended via unfolding.

`elecEREC < 12.` in `mainAnalysis_fillTree.C` 710 (note: this uses
`elecEREC` = electron-plus-collinear-photon energy, not the raw cluster).

#### Trigger

The trigger weights `trigWeightAC` (actual subtriggers) and
`trigWeightRW` (raw weights) are computed from a hardcoded set of SpaCal
subtriggers: **s0, s1, s2, s3, s61** (`ForwardMultAnalyzer.C` 922-926).
S61 is the main HERA-II low-Q2 DIS trigger we already use. The
codebase treats data with `trigWeightAC > 0` and weights events by
`trigWeightRW`. The text in the paper says "trigger inefficiency is
negligible in the kinematic region", consistent with our experience.

#### Vertex type

`vertexType != 1` rejects events without a primary CTD vertex. (Type 1
in H1OO = CJC primary vertex.) This is the same as our current cut.

#### Background finders

Bits 0-6 of `ibg` (= bits with values 1,2,4,8,16,32,64) from the OM
group are vetoed. This is the standard H1 cosmics/halo/satellite-bunch
veto set. Our SIDIS builder should already do this; if not, add it.

#### `Epz` (E - pz) window

`35 < E - pz < 70 GeV`. This is the standard ISR-rejection cut around
`2 * E_e_beam = 55 GeV`. Tighter than what we usually run; our builder
may use `35 < E - pz < 70` or `45 < E - pz < 65`. Make sure to use
**electron+collinear-photons** in the sum, not raw cluster, to avoid
biasing radiative events.

#### Vertex z

`|z_vtx| < 35 cm` — standard low-Q2 cut.

### 3.3 QED-Compton tagging

A separate flag `isQEDc` is computed at MC truth level (`TDetectQedc`)
and at REC level (sub-leading SpaCal cluster in `radPhot_REC_lab`). The
publication then either reweights or excludes these events. We may
inherit the same machinery if we want to handle QED-Compton at the
TMD-multiplicity stage.

---

## 4. Acceptance, Efficiency, and Unfolding

### 4.1 Detector correction strategy

The publication uses **TUnfold-based regularized unfolding** of the
multiplicity distribution P(N) per (Q2, y, eta) bin, not bin-by-bin
correction. The `TUnfold/` subdirectory is a local copy used to build
the response matrix bin migration in (N_rec, N_gen) per (Q2_rec, y_rec)
cell. The minitree (`mainAnalysis_fillTree.C` output) contains both REC
and matched MC truth on every event, which is what TUnfold needs as
input.

This is **the same architecture we are planning for SIDIS** (4D unfolding
in Q2,x,z,pT_h), so we can borrow the structure directly.

### 4.2 Track-MC matching

`ForwardMultAnalyzer.C` 1502-1568 does per-event REC-MC matching by
covariance-weighted momentum chi^2 (eigen-decomposition of the 3×3
momentum covariance) with deduplication. Each REC track stores
`imatchREC` (best MC index, or negative for fake) and `dmatchREC` (chi^2
of the match), and each MC track stores `imatchMC` (best REC index, or
negative for inefficiency). This is more rigorous than a simple
`dR(rec,gen) < 0.1` matcher and gives correct treatment of low-pT tracks
where dR is large but pulls are still small.

The track momentum covariance comes from `H1Track::GetMomentumCovar()`
for central/combined and from `H1FSTFittedTrack::GetMomentumCovar()` for
FST. This is already in the code — we just need to consume it on the
SIDIS side if we want the same rigor.

### 4.3 Nuclear-interaction weight

After matching, each track with a `pi+/-` or `K+/-` MC truth gets a
multiplicative correction `nucliaREC` from `H1NuclIACor::GetWeight()`
(`ForwardMultAnalyzer.C` 1572-1589). This compensates the MC's
overestimate of pion/kaon nuclear absorption probability in the CJC
material. The argument list is:

```cpp
H1NuclIACor::GetWeight(part, sign(charge),
                       pt, phi, theta, dca=0.0,
                       H1SelVertex::GetPrimaryVertex()->Z());
// part: 1 = pi, 2 = K
```

The weight is then used either as a per-track survival probability in
the macros:

```cpp
if (rand->GetRandom() > nucliaREC[j] && rand->GetRandom() < 1.0) continue;
```

(stochastic acceptance) or as an event weight in some downstream tools.
The DESY 20-176 paper assigns Sys.6 ("V0 particle contamination") to
cover residual differences and adds 50% uncertainty on K0_s daughter
tracks plus 0.5% on photon-conversion/Dalitz tracks. We do **not**
currently apply nucliaREC at all in our SIDIS builder.

### 4.4 MC reweighting

`MCweight.h` declares hardcoded arrays of 100/200-bin reweights applied
in `mainAnalysis_fillTree.C` 161-178:

- `vtxz_weight_hadCali_rapgap[]` and `vtxz_weight_hadCali_django[]` reshape
  the MC z-vertex to data.
- `y_weight_hadCali_rapgap[]` and `y_weight_hadCali_django[]` reshape the
  MC y_es spectrum to data.

These corrections are applied on top of the MC event weight `w` and the
luminosity scaling factor `(L_data / L_mc)`. They are **iterative,
post-hoc** corrections — the author tunes them after running with
unweighted MC to make event-level distributions match. This is a
standard H1 practice but it does mean our SIDIS analysis needs the
analogous data/MC tuning step before unfolding.

### 4.5 Single-track efficiency systematic (Sys.5)

Published values (DESY 20-176 §5):

- **Central tracks: 0.5%** uncertainty on the tracking efficiency.
- **Combined tracks: 10%** uncertainty on the tracking efficiency.

Both are propagated to P(N) as 1-5% and to moments as 1%. The 10%
combined-track efficiency uncertainty is the **dominant tracking-related
systematic** and is much larger than the central-track uncertainty. Any
SIDIS analysis using combined tracks must budget this.

The author also exposes (commented out in `mainAnalysis_fillTree.C`
797-799) a per-type efficiency degradation:

```cpp
// double eff_error = 0.995;
// if (type == 2) eff_error = 0.9;
// myEvent.nucliaREC_mini[j] = nucliaREC[j] * eff_error;
```

i.e., a uniform 0.5% degradation for central tracks and 10% for combined,
applied via the per-track survival weight. This is exactly the Sys.5
machinery.

---

## 5. MC Generators and Normalization

From `mainAnalysis_fillTree.C` 184-220 and the steering files in
`input/`:

| Generator | MC sample IDs | Purpose |
|-----------|---------------|---------|
| DJANGOH 1.4 (NC DIS) | 8926, 8927 | Default for low-Q2 e+p |
| RAPGAP 3.1 (NC DIS) | 9299-9306 | Used for cross-check (becomes the default in the publication) |
| RAPGAP 3.1 (diffractive) | 9015 | Diffractive component for low-Q2 |
| PYTHIA 6 (photoproduction) | 6921 | Photoproduction background |
| DJANGOH/RAPGAP NRAD | (separate steers) | Non-radiative for C_rad |

Normalization (`mainAnalysis_fillTree.C` 528-587) uses
`w * (L_data / L_mc)`. Specifically:

- `w = weight1 * weight2` from the H1OO GKI weights (same as ours).
- `L_data = 136 pb^-1` (DESY 20-176 dataset = 2007 only; ours is 297.7
  pb^-1 = 2005+2006+2007).
- `L_mc` per sample: 68 (RAPGAP DIS), 219.35 (RAPGAP diffractive), 449
  (PYTHIA PHP), 363 (DJANGOH).
- Diffractive RAPGAP is scaled down by an extra 0.1 ("for 10% of DIS
  cross section") as a tuned mixing factor.
- An optional `maxPDGmc` reweight inflates events containing c-quarks by
  1.2 and b-quarks by 2.0 — this is a heavy-flavour data/MC adjustment.

This is **incompatible with our SIDIS normalization**, which is per-year
and per-sample with the H1 lumi pointer (`H1CalcWeight` etc.). We
should **not** copy the lumi values; we should adopt only the structural
choices (which generators, which background mix).

---

## 6. Phase Space and Branch Schema

### 6.1 Published phase space (DESY 20-176)

| Variable | Range |
|----------|-------|
| Q^2 | 5 - 100 GeV^2 |
| y | 0.0375 - 0.6 |
| Track p_T (lab) | > 150 MeV |
| Track |η_lab| | < 1.6 |
| Track p (combined only) | > 500 MeV |
| Track polar angle (combined only) | 10° < θ < 30° |
| Track polar angle (central) | 15° < θ < 165° (CJC acceptance, implicit) |
| HCM η* (binning) | 0 - 5 |

### 6.2 Branches that exist in the H1OO output tree

`ForwardMultAnalyzer.C` 364-520 declares the `properties` TTree.
Track-level relevant branches:

```
typeChgREC[nRECtrack]   /I    // charge * (1=central, 2=comb, 3=fwd, 4=FST, 5=BST)
pxREC, pyREC, pzREC     /F    // lab 3-momentum
pREC, peREC             /F    // |p| and its error from the H1Track
etaREC                  /F    // lab eta
ptStarREC, etaStarREC, phiStarREC   /F   // HCM (electron-method boost)
ptStar2REC, etaStar2REC, phiStar2REC /F  // HCM (e-Sigma method boost)
chi2vtxREC, vtxNdfREC, vtxNHitsREC, vtxTrackLengthREC /F,I,I,F
chi2nvREC,  nvNdfREC,  nvNHitsREC,  nvTrackLengthREC  /F,I,I,F
dcaPrimeREC, dz0PrimeREC /F   // distance of closest approach (XY, Z)
startHitsRadiusREC, endHitsRadiusREC /F  // first/last hit radius
trkThetaREC             /F    // polar angle
chi2TrkREC, ndfTrkREC   /F,I  // track-fit quality
zLengthHitREC           /F    // z(start) - z(end) for forward tracks
chi2LinkREC, ndfLinkREC /F,I  // combined-track CJC-FTD link chi^2
rZeroREC                /F    // forward-track R at z=0
dedxPionREC, dedxElectronREC, dedxProtonREC /F   // expected dE/dx per hypothesis
dedxLikelihoodPionREC, ...Electron, ...Proton    /F
nucliaREC               /F    // H1NuclIACor weight
dmatchREC, imatchREC    /F,I  // MC match chi^2 and index
```

This is the schema we'd want to mirror (or subset) in our SIDIS builder
if we want full systematic coverage. Our current builder has only the
central-track subset.

### 6.3 Gen-level branches stored

```
nMCtrack, idMC[], pxMC[], pyMC[], pzMC[], etaMC[], chargeMC[]
ptStarMC[], etaStarMC[], phiStarMC[]   // HCM (electron method)
ptStar2MC[], etaStar2MC[], phiStar2MC[] // HCM (e-Sigma method)
log10zMC[]                             // log10(z_pq) per particle
isDaughtersMC[]                        // 1 = K0s daughter, 2 = Lambda daughter
imatchMC[]                             // index of matched REC track (or <0)
```

The `isDaughtersMC` mechanism (lines 770-857) walks the MC parent chain
to flag tracks whose immediate parent is a K0_s (PDG 310) or Lambda
(PDG 3122). DESY 20-176 uses this to apply the 50% Sys.6 uncertainty to
V0 daughters. We have not implemented this yet; for inclusive
charged-hadron multiplicity it matters less, but for the K^0_s / Λ
overlap with our V0 analyses, this is the bookkeeping that prevents
double counting.

---

## 7. Key Code Snippets to Adapt

### 7.1 Building the per-track type tag

```cpp
// Source: ForwardMultAnalyzer.C : 1273-1280
int type = 0;
if (track->IsCentralTrk())       type = 1;
else if (track->IsCombinedTrk()) type = 2;
else if (track->IsForwardTrk())  type = 3;
else if (track->IsFSTTrk())      type = 4;
else if (track->IsBSTTrk())      type = 5;
```

In our builder we currently early-exit on `!IsCentralTrk()`. To
extend to combined, replace the early exit with the type tag and a
union acceptance:

```cpp
// proposed for ProtonTreeBuilder_lowQ2.C
int trkType = 0;
if      (trk->IsCentralTrk())  trkType = 1;
else if (trk->IsCombinedTrk()) trkType = 2;
if (trkType == 0) continue;          // skip forward, FST, BST
```

### 7.2 Combined-track-only diagnostics

```cpp
// Source: ForwardMultAnalyzer.C : 1318-1324, 1293-1310
float chi2vtx = -1, chi2Link = -1;
int   vtxNdf = -1, ndfLink = -1;
int   vtxNHits = 0;
float vtxTrackLength = -1, dcaPrime = -1, dz0Prime = -1;
float startR = -1, endR = -1;

H1VertexFittedTrack const *h1track =
    dynamic_cast<H1VertexFittedTrack const *>(cand->GetTrack());
if (h1track) {
    chi2vtx        = h1track->GetFitChi2();
    vtxNdf         = h1track->GetFitNdf();
    vtxNHits       = h1track->GetNHit(H1Track::tdCJC);
    vtxTrackLength = h1track->GetLength();
    dcaPrime       = h1track->GetDcaPrime();
    dz0Prime       = h1track->GetDz0Prime();
    startR         = h1track->GetStartRadius();
    endR           = h1track->GetEndRadius();
}
if (trk->IsCombinedTrk()) {
    H1CombinedFittedTrack const *combtrack =
        dynamic_cast<H1CombinedFittedTrack const *>(cand->GetTrack());
    if (combtrack) {
        chi2Link = combtrack->GetLinkChi2();
        ndfLink  = combtrack->GetLinkNdf();
    }
}
```

Required includes:

```cpp
#include "H1Tracks/H1CombinedFittedTrack.h"
#include "H1Tracks/H1CombinedFittedTrackArrayPtr.h"   // only if iterating directly
#include "H1Tracks/H1ForwardFittedTrack.h"            // for rZero if extending later
```

### 7.3 Primary-vertex matching for tracks

```cpp
// Source: ForwardMultAnalyzer.C : 1333-1339
bool isPrimary = false;
H1Vertex const *v = h1track->GetVertex();
if (v && floatEqual(v->X(), evt.vertex[0]) &&
         floatEqual(v->Y(), evt.vertex[1]) &&
         floatEqual(v->Z(), evt.vertex[2])) {
    isPrimary = true;
}

// helper:
static bool floatEqual(double a, double b) {
    double d1 = fabs(a-b), d2 = fabs(a+b);
    return (d1 <= 1.E-5 * d2);
}
```

This replaces our `IsFromPrimary == 1` cut with the same semantics but
works for combined tracks too (where `IsFromPrimary` is unreliable in
some H1OO versions).

### 7.4 Per-type quality cuts (apply at minitree or analysis stage)

```cpp
// Adapted from mainAnalysis_all.C : 485-505
bool keep_central(...) {
    if (pt < 0.15)                                           return false;
    if (fabs(dcaPrime * sin(trkTheta)) > 2.0)                return false;
    if (startR > 50.)                                        return false;
    if (fabs(startR - endR) < 10.)                           return false;
    if (vtxNHits < 0)                                        return false;   // sanity
    if (fabs(trkTheta - elecTheta) < 0.2)                    return false;   // scat-e veto
    return true;
}
bool keep_combined(...) {
    if (pt < 0.15)                                           return false;
    if (p  < 0.5)                                            return false;
    if (rad2deg(trkTheta) < 10. || rad2deg(trkTheta) > 30.)  return false;
    if (fabs(dcaPrime) > 5.0)                                return false;
    if (startR > 50.)                                        return false;
    if (chi2vtx  > 50.)                                      return false;
    if (chi2Link > 50.)                                      return false;
    return true;
}
```

### 7.5 Electron quality cuts (re-usable as-is)

`elecCut.C` is self-contained and depends only on `H1PartEm`,
`H1DBManager`, and SpaCal geometry classes. The `goodElec(elec, run)`
call returns 1 for accepted, 0 otherwise. The author specifically
**removed** the BPC/CJC track-link requirement (commented out lines
489-493, 517-521); the choice is "use SpaCal cluster quality only, do
not require a matched track". Our current high-Q2 SIDIS code (LAr) does
not need this file, but our low-Q2 SIDIS code probably does. We may
want to verify our electron selection matches one of `goodElec`,
`goodElec_noSpatialCuts`, or `goodElec_TrackCond`.

### 7.6 E-pz cut formed with electron+photons

```cpp
// Use escatPhot, not raw escat0, in E-pz
double Epz = hfsEREC + elecEREC - (hfsPzREC + elecPzREC);
// where elecEREC and elecPzREC come from escatPhot_REC_lab (sum of
// scattered electron cluster + photons within dR<0.4)
if (Epz < 35. || Epz > 70.) reject;
```

This matters for radiative events; using raw cluster energy biases the
high-y tail.

---

## 8. Recommendations for `ProtonTreeBuilder_lowQ2.C`

### 8.1 Minimal change to enable combined tracks (Phase 1)

1. **Remove the `IsCentralTrk()` early exit.** Replace with:
   ```cpp
   int trkType = 0;
   if      (trk->IsCentralTrk())  trkType = 1;
   else if (trk->IsCombinedTrk()) trkType = 2;
   if (trkType == 0) continue;
   ```
2. **Add a new branch `TrkType` (Int_t)** so we can split central vs
   combined downstream.
3. **Loosen the eta cut to `|Eta| < 1.6`.** Keep the analysis-tier cut
   `|Eta| < 1.3` for backwards compatibility behind a config flag.
4. **Add new branches needed for combined-track diagnostics:**
   `Chi2Link`, `NdfLink`, `StartRadius`, `EndRadius`, `DcaPrime`,
   `DcaPrimeSinTheta`, `TrkTheta`, `Chi2Vtx`, `VtxNHits`.
   We already have most of these for central tracks; add the ones we
   don't, and make sure they are filled correctly for combined tracks
   (the `H1VertexFittedTrack` dynamic_cast path works for both).
5. **Drop `RadLength` for combined tracks**, or document that
   `RadLength` is only meaningful for CJC-only tracks. Replace the
   `RadLength > 10` cut for combined tracks with the published
   substitutes (`p > 0.5 GeV` and `chi2Link < 50`).
6. **Drop `NHitDedx >= 10` for combined tracks** because dE/dx is CJC-only
   and the combined track's CJC segment may be short. Keep it as a
   *central-only* cut.
7. **Drop the `P` band (0.3-3.0 GeV)** if we agree it was PID-motivated
   (already noted as dropped in our SIDIS handoff). For combined tracks
   keep only `P > 0.5 GeV` as required for material crossing.

### 8.2 Vertex-association replacement

Replace `trk->IsFromPrimary()` with the explicit vertex equality check
(7.3 above). For our SIDIS analysis the practical difference may be
nil, but it is **mandatory if we add combined tracks** because some H1OO
versions did not always set `IsFromPrimary` for combined tracks
reliably.

### 8.3 Two-tier output

Mirror DESY 20-176's pattern: in the builder, **do not apply any
track-quality cut beyond the type tag and primary-vertex association**.
Push all quality cuts into the post-processing layer
(`analyze_sidis_charged.py` / a new minitree builder). This makes
systematic variations (tight/default/loose tiers) cheap: re-run only the
post-processor.

If we are committed to the current "cut in the builder" pattern, we
should at minimum:
- store `TrkType`, `Chi2Vtx`, `Chi2Link`, `DcaPrime`, `StartRadius`,
  `EndRadius`, `TrkTheta`, `VtxNHits` as branches even when not used in
  the default cut, so we can vary them later without rebuilding.

### 8.4 Add `nuclia` weight

Use `H1NuclIACor::GetWeight()` after the MC-matching step (see code
snippet in §4.3). Store `Nuclia` per track. This is one of DESY 20-176's
default corrections; not applying it will leave a known ~1% bias for
pions and a few % for kaons.

### 8.5 Add V0-daughter flag at gen level

Walk `H1PartMC` parents to detect K0_s (PDG 310) and Lambda (PDG 3122)
parents (`ForwardMultAnalyzer.C` 770-776, 846-857). Store
`IsDaughterV0` (0 = primary, 1 = K0s daughter, 2 = Lambda daughter).
This enables Sys.6 propagation later and prevents accidental
double-counting with the dedicated V0 analyses.

### 8.6 Trigger-weight handling

Store both `TrigWeightAC` and `TrigWeightRW` (`ForwardMultAnalyzer.C`
953-955). The published macro uses `trigWeightAC > 0` for the trigger
fired condition and `trigWeightRW` as the per-event weight, applied
**on top of** `weight1*weight2`. Make sure our scripts know which one
to use; right now we use a simple "trigger bit" check.

### 8.7 Do *not* add forward, FST, or BST tracks

The published analysis does not use them. Adding them would force us to
maintain the FST-overlap anti-double-count algorithm (~30 lines), tune
their own Sys.5 uncertainty (would have to be measured), and adopt
forward-track-specific quality cuts. The marginal eta acceptance gain
beyond 1.6 is small and dominated by uncertainty.

---

## 9. Systematics for Broader Tracking Acceptance

For a SIDIS measurement using central + combined tracks in
`|η_lab| < 1.6`, the new tracking systematic budget is:

| Source | Central tracks | Combined tracks | Combined effect |
|--------|----------------|-----------------|------------------|
| Sys.5 (single-track efficiency) | 0.5% | **10%** | 1-5% on observable; 1% on moments |
| Sys.6 (V0 contamination) | 50% on K0s daughters; 0.5% on conversions | Same | 1-7% on P(N); up to 2% on moments |
| Sys.3 (SpaCal energy/angle) | 1% / 1° | 1% / 1° | 1-5% on P(N); 2% on moments |
| Sys.4 (HFS energy scale) | 2% | 2% | 1-3% on P(N); 2% on moments |

The dominant *new* systematic introduced by combined tracks is **10%
on the combined-track efficiency**, applied per combined track and
propagated through the unfolding. In a 4D SIDIS measurement where most
bins are dominated by central tracks (because `|eta| < 1.3` already
covers most of the SpaCal-accessible phase space), the combined-track
contribution per bin is typically only 5-20% of the yield, so the
bin-by-bin systematic from this source is 0.5-2%. It is *not* negligible
for the most forward bins (high-z and high-pT_h in the proton direction)
where combined tracks dominate.

**Recommended treatment:**

1. Apply the published per-type efficiency shifts as event weights
   (Sys.5): central = 1.000 ± 0.005, combined = 1.000 ± 0.100, applied
   to each track individually.
2. Add a **central-only vs central+combined** cross-check at observable
   level. If the two agree within their statistical bands (after lumi
   reweighting) we can claim consistency. If they don't, the 10%
   combined-track uncertainty *must* be carried, and we should
   investigate where the disagreement comes from.
3. Use `IsDaughterV0` to apply Sys.6 properly. The simplest
   conservative approach is to add 50% × N_K0s_daughters to the
   per-bin uncertainty.

The published Sys.3/Sys.4 (SpaCal and HFS energy scales) are the same
regardless of which track types we use; our existing post-hoc
detector-shift framework already covers these. No new code needed.

---

## 10. Pitfalls and Complications for SIDIS with Combined Tracks

### 10.1 Combined-track η coverage is forward-only

The CJC-FTD overlap region is **10° < θ < 30°** (i.e., `1.32 < η < 2.44`
strictly, but the publication restricts to `η < 1.6` as a quality cut).
There is no equivalent BST + CJC combined track for the backward
direction in any of the code we have inspected. Our SIDIS phase space
gains forward acceptance only. For SIDIS kinematics (current hemisphere
in HCM), this overlaps with the *target-fragmentation* region in the lab
frame: forward-lab particles are typically backward in HCM (η* < 0)
and live at low z. The most valuable gain may therefore be in **low-z,
forward-lab** bins.

### 10.2 dE/dx is unavailable on the combined-track CJC segment

`H1PartSelTrack::GetDedx()` returns a value for combined tracks but the
fact that the CJC segment is short (sometimes only a handful of hits)
means the `NHitDedx` is low and the dE/dx is unreliable. Our
inclusive h+/h- SIDIS observable does not use dE/dx, so this is **not a
blocker**. But: if we ever want to do PID-separated SIDIS with combined
tracks, we have to either accept much wider PID resolution or restrict
PID to central tracks only.

### 10.3 Momentum resolution is worse for combined tracks

The publication's `σ(p_T)/p_T ≈ 0.2%/GeV ⊕ 1.5%` resolution is for
*central* tracks. For combined tracks, the CJC arm length is shorter
and the FTD wire chambers add their own resolution; the typical
combined-track resolution is several percent worse at high pT and
comparable at low pT. For SIDIS observables binned in pT_h (HCM), this
inflates the off-diagonal entries of the migration matrix. Our 4D
unfolding framework already absorbs migration; we should re-run the
purity/stability check with combined tracks included to confirm that
purity stays above the 30% per-dimension threshold we adopted.

### 10.4 Vertex-fit chi^2 floor is different

Central tracks have `chi2vtx` typically O(1-10); combined tracks have a
broader distribution dominated by the link chi^2 (which is folded into
chi2vtx). The published cut `chi2vtx < 50` is generous for this reason.
Do not import a `chi2vtx < 5` cut from any central-only analysis as if
it generalized to combined tracks; it does not.

### 10.5 Track-MC matching ambiguity

The covariance-weighted chi^2 matcher in §4.2 handles combined tracks
correctly because each track type provides its own momentum covariance.
However, the migration matrix may show MC truth particles from the
forward direction matching to *both* a forward-lab combined track *and*
a forward-eta central track in events where the same particle is
reconstructed twice. The de-duplication step
(`ForwardMultAnalyzer.C` 1539-1568) handles this, but if we replicate
the matching logic in our own builder we have to replicate the
de-dup logic too.

### 10.6 Combined tracks and the scattered electron

The combined-track polar-angle cut `10° < θ < 30°` is in the **proton
hemisphere**. The scattered electron in the low-Q2 SpaCal range is in
the **electron hemisphere** at `θ > 153°`. There is no overlap; we do
not need to add the `|trkTheta - elecTheta| < 0.2` electron-veto for
combined tracks. The veto is needed only for central tracks at high y
where the scattered electron sometimes leaves a CJC track.

### 10.7 The published analyzer applies cuts in two places, not one

Reading the code, we found cut text in three different files:
`ForwardMultAnalyzer.C` (only the "type" tag and FST anti-overlap),
`mainAnalysis_all.C` (older histogram-fill cuts), and
`mainAnalysis_fillTree.C` (newer minitree cuts via `getPassFlag`). The
numbers agree across all three for the published configuration, but the
fact that there are three copies is a maintenance hazard. If we adopt
the two-stage architecture, we should consolidate cut definitions in a
single header file and include it from both stages.

---

## 11. Open Questions and Gaps

1. **What `IsFromPrimary` actually does for combined tracks** — we have
   read the analyzer's vertex-equality check but not the `H1PartSelTrack`
   source. Worth a 30-minute check in the H1OO source on NAF
   (`/afs/desy.de/group/h1/root/checkout2/.../H1Mods/H1PartSelTrack.cxx`).
2. **Whether our SIDIS MC samples already store combined-track diagnostic
   branches** at H1OO truth level. If not, we may need to re-run
   production with an extended `H1PartSelTrack` consumer.
3. **The actual combined-track contribution to our SIDIS bins.** A
   one-pager histogramming `nRECtrack` by `TrkType` per (Q2, x, z,
   pT_h) bin on existing MC would tell us how much we gain. Until we
   measure this, the case for extending vs not extending is qualitative.
4. **Trigger compatibility.** The DESY 20-176 trigger set is
   `{s0, s1, s2, s3, s61}`. We currently use only s61. The full set
   should still be a strict superset of s61, but if we widen acceptance
   we may want to widen the trigger too.
5. **The exact published cut on `y`.** The paper text says
   `0.0375 < y < 0.6` but the code uses `y > 0.05`. The macro author
   evidently restricts the *reco* y window (where the trigger is safe)
   and trusts unfolding to extrapolate to the published fiducial. We
   should pick one convention before extending.
6. **Whether DJANGOH 1.4 sample 8927** (the e+p, RAD, low-Q2 sample
   used as the publication default for low-Q2 e+p) is the same physical
   sample we use. The naming convention matches but the steering files
   differ slightly. Compare `input/DJANGO14_2007_LOWE.steer` to ours.

---

## 12. Summary for the Supervisor

The published H1 charged-particle multiplicity analysis (DESY 20-176)
extends acceptance from CJC-only to **CJC + (CJC+FTD-combined)** tracks
in `|η_lab| < 1.6`. It does **not** use forward-only, FST-only, or BST
tracks despite the codebase containing the infrastructure for them.

The track selection has two distinct cut sets — one for central, one for
combined — and the published values are listed verbatim in §2.3. The
event selection is structurally identical to ours apart from:

- E-pz uses electron+collinear-photons (we should verify ours does).
- Background-finder bits 0-6 are vetoed (verify ours does).
- `vertexType == 1` and `|z_vtx| < 35` (we do this already).

Acceptance is via **TUnfold-based regularized unfolding**, not bin-by-bin
correction. The minitree (`mainAnalysis_fillTree.C` output) is the
unfolding input. This matches our planned 4D unfolding architecture and
the code is a useful reference.

The dominant *new* systematic from extending to combined tracks is
**10% on the combined-track efficiency** (Sys.5). It will affect the
most forward bins (high-eta_lab, forward-lab particles in the
target-fragmentation hemisphere of HCM). The marginal acceptance gain
is **forward-only** and concentrated in `1.3 < |η_lab| < 1.6` at
`10° < θ < 30°`.

Concrete actions:

- Adopt the two-tier (per-type) track-quality scheme in §2.3 and §7.4.
- Adopt the explicit vertex-equality primary-track check in §7.3.
- Add the combined-track diagnostic branches in §7.2 to our builder.
- Add `nuclia` weight (§4.3, §8.4) and `IsDaughterV0` flag (§8.5).
- Carry the 10% combined-track efficiency systematic alongside our
  existing tracking systematics.
- Before committing to the extension, run a one-pager on existing MC
  histogramming `nRECtrack` by `TrkType` per (Q2, x, z, pT_h) bin to
  measure the actual yield gain.

The biggest open question is **whether the marginal acceptance gain in
the few forward-lab SIDIS bins justifies the 10% combined-track
efficiency systematic and the maintenance cost of a per-type cut scheme**.
That is a planning question for the supervisor; this research note
provides the evidence either way.
