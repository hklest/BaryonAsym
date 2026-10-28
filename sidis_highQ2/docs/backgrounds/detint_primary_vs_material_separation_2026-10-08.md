# Detector-interaction track classes: primary-origin continuers vs material-born secondaries — current handling and separability (v6/z005)

- **Deliverable date:** 2026-10-08. Read-only study; no central product, tree, or diagnostic was modified.
- **Scope:** MC-truth semantics of the two user-defined track classes in the v6/z005 high-Q2
  inclusive charged-hadron SIDIS chain, their current treatment in the tree builder and the
  central response, and whether the current `ChargedTot` branch set can separate them.
- **Established context consumed as authoritative:** STATE.md (2026-10-08), the backgrounds skill
  (`sidis_highQ2/.opencode/skills/sidis-hq2-backgrounds/SKILL.md`), and the 2026-10-07
  background-subtraction/unfolding consistency ledger
  (`docs/backgrounds/bgsub_unfolding_consistency_ledger_2026-10-07.md`).
- **Evidence base:** H1OO 4.2.1 source
  (`/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/h1oo/`), the v6 tree
  builders, the central v4 response builder, the all-fake response variant script, the det.int.
  response-partition JSON, and ONE single-file login-node empirical check on
  `output_v6/central/sidistree/mc/djangoh14/7091/sidistree_djangoh14_7091_ep0607_0200.root`
  (28,575 `ChargedTot` rows; 14,640 pass a faithful mirror of the central reco selection).

---

## A. Class definitions (as fixed by the user for this record)

| Class | Definition (user's wording, made precise) | Truth-level signature |
|---|---|---|
| **Class 1 — SIGNAL** | A charged hadron (π/K/p) from the primary ep interaction that undergoes interactions in detector material (ionization energy loss, elastic-ish nuclear scattering, delta-ray emission) but **continues as the same physical particle**; the reconstructed track is that same particle. | Same GEANT3 track as the generator particle: the generator (GTR) record and the transported GEANT track are one and the same. |
| **Class 2 — FAKE (user's physics definition)** | A **new** charged particle created inside the detector by an inelastic interaction of some other track (or photon/neutron) on material; the reconstructed track is a **distinct secondary particle with no primary-vertex origin**. | A GEANT3 track created during simulation (no generator counterpart): appears only in the STR bank. |

Boundary note (ESTABLISHED at the simulation level, standard GEANT3 behavior — see UNVERIFIED
flag U2): continuous/elastic-ish processes (dE/dx, multiple scattering, delta rays, elastic
nuclear scattering) do not create a new GEANT track — the same track continues (Class 1).
An inelastic hadronic interaction terminates the incident track and creates new tracks; any
"continuing leading particle" after such an interaction is a NEW GEANT track (Class 2). The
class boundary therefore coincides with the new-track boundary in GEANT3.

---

## B. Current handling of Class 1 (continuing primary) — ESTABLISHED from code

**Answer: a Class-1 track stays linked to the same generator particle and is treated as signal
end-to-end.** The mechanism, verified at each step:

1. **Truth link (H1OO, Tier 0).** For every fitted track the H1OO DST→MODS conversion links the
   reconstructed track to the MC particle of its **best GEANT (STR) track** via the DTRY bank:
   `H1TrkFinder/H1CreateSelTracks.C` lines 115–145 — `istr = dtry[idtra]->GetInt(0)-1`
   ("index of 'best' STR track"); if that STR track has a GTR link
   (`igtr = str[istr]->GetInt(7)-1 > -1`) the candidate is linked to the **GTR record**
   (`candidate->AddPartMC(MCpart[igtr])`). For a Class-1 track the best GEANT track IS the
   generator particle's own transported track, so the link resolves to the generator π/K/p.
2. **GTR vs STR flags.** `H1Mods/H1PartMC.C` lines 187–196: `IsStr() == (fStrIdx != -1)`,
   `IsGtr() == (fStrIdx == -1)`. GTR-bank particles are filled with `StrIdx = -1`
   (`H1Filler/H1ModsFiller.C` `FillMC`, lines 194–221) → `IsGtr() == true`. A GTR-linked track
   therefore reads `IsGtr == 1`.
3. **Builder (Tier 0, preferred).** `src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 1688–1696:
   `matched = cand->GetPartMC()` → `MatchTier_ch = 0`. Common fill block lines 1731–1744:
   `IsMatched_ch = 1`, `IsGtr_ch = 1`, `IsStr_ch = 0`, and
   `IsMatchedHadron_ch = isChargedHadron(GenPDG_ch)` = true for a π/K/p GTR partner. The gen
   kinematics branches (`GenPt/GenEta/GenPhi/GenP`, lines 1752–1757) are the **generator-level
   (pre-transport) kinematics** of the same particle; the energy loss / elastic scattering the
   particle suffered in material is absorbed as ordinary reco-vs-gen migration in the response.
4. **Tier 2 fallback (rare).** Only when Tier 0 returns null: greedy nearest-angular match over
   the combined GTR+STR array within `CHARGED_MATCH_ANGLE = 2.0°`
   (`src/PhysicsConstants_chargedsidis_highQ2.h:63`; builder lines 1699–1727). Empirically
   Tier 2 fires for 0.09% of selected signal tracks (single-file check, Sec. C).
5. **Response treatment.** Central v4 response builder
   `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`:
   `matched_gen_ok` (lines ~784–806) requires `IsMatchedHadron==1 & IsMatched==1` + gen-event
   fiducial + species pool + in-grid; matched rows fill the physical response body
   `R[ir, ig]` (lines ~925–940). **There is no `IsGtr` requirement anywhere in the matched
   fill** — but for Class 1 this is moot: Class-1 tracks are GTR-matched and land in the body,
   i.e., **unfolded as signal**, exactly as the user wants.
6. **Nuclear-interaction efficiency correction (related, MC-only).** The chain already models
   the *complementary* Class-1 effect — primaries that interacted and were NOT reconstructed
   (absorbed): `NucliaWeight` from `H1NuclIACor::GetWeight` (builder lines 1639–1650, species
   refinement lines 1830–1840: pion default, kaon refined, proton → 1.0), applied in the
   response numerator (`w_reco = w1*w2*NucliaWeight`, response builder). This corrects the
   MC tracking efficiency for π/K nuclear-interaction losses; it does not touch the Class-2
   question.

**Caveat on the Class-1 label (PROVISIONAL, not quantifiable with current trees):** the truth
link follows the *best* (DTRY) GEANT track. A reconstructed track that merges the pre- and
post-interaction hit segments of an inelastic interaction (unresolved kink) is labeled by its
majority-hit track: if the primary owns the majority it is labeled Class 1 even though part of
its hits belong to a new particle. This "kink-merged hybrid" subset is an unquantified Class-2
contamination *inside* the signal label (see Sec. D, missing information, and UNVERIFIED U1).

---

## C. Current handling of Class 2 (material-born / decay-born secondaries) — ESTABLISHED from code + one-file empirics

### C.1 How a Class-2 track flows through the chain

A new GEANT track (no generator counterpart) exists only in the STR bank. H1OO appends it to
the combined MC array as an **STR-only `H1PartMC` record carrying its OWN PDG and OWN
kinematics**: `H1ModsFiller.C::FillMC` lines 224–287 — STR tracks with `igtr < 0` are appended
with `StrIdx = i` (→ `IsStr()==true, IsGtr()==false`), 4-vector = the STR bank
(Px,Py,Pz,E) of the secondary, `Mo1` = the **nearest ancestor present in the combined array**
(found by walking the STR parent chain). The ancestor is used only for mother/ancestor *labels*
(`GenMotherPDG_ch`, `VetoAncestorPDG_ch`), never for the matched kinematics.

For a reco track whose hits belong to such a secondary, the Tier-0 truth link resolves to the
secondary's own STR record: `H1CreateSelTracks.C` lines 137–142 — if the best STR track has no
GTR link, the candidate is linked to the `H1PartMC` whose `StrIdx == istr`, i.e., **the
secondary's own record**. In the builder (lines 1731–1771) this yields `IsStr_ch = 1`,
`IsGtr_ch = 0`, `GenPDG_ch` = the secondary's own π/K/p, hence
`IsMatchedHadron_ch = isChargedHadron(π/K/p) = true`, with `GenPt/GenEta/GenPhi/GenP` = the
secondary's own production kinematics (used verbatim for the matched gen bin: lines 1793–1824;
the response computes the gen (z, pT) bin from these branches, response builder
`compute_reco_bin`).

**So the det.int. category definition —
`IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && IsMatchedElectron==0 && IsMatchedV0Daughter==0`
(partition JSON `classifier` field) — is exactly the Class-2 label at truth-link level**, with
`IsMatchedHadron==1` meaning *matched to the secondary's own STR record* (not to an ancestor).
The e/V0 masks exclude secondary e± (conversions/Dalitz, mostly) and V0-decay daughters
(Λ/K0s/K0L chains, caught by the 6-hop `findVetoAncestorPDG` walk,
`src/ChargedSidisUtils_highQ2_v6sys.h` lines 181–197).

### C.2 What happens to them in the response (ESTABLISHED, ledger-consistent)

- The central response has **no `IsGtr`/`IsStr` requirement in the matched fill**: a det.int.
  track whose (secondary) gen kinematics and gen event pass the gen fiducial/grid lands in the
  **matched physical block** and is **unfolded as signal** — this is the 86.18% matched block
  (partition: 0.8617818696 nominal fraction, cluster 3971447, alpha0p2 selection). Its gen bin
  is computed from the secondary's own kinematics — the response never maps a det.int. track
  onto an ancestor's bin.
- The remaining 13.82% (0.1382181) land in the **fake column** — dominated (~92% per ledger) by
  rows whose matched (secondary) gen *event* fails the gen-side Q2/y/fiducial cuts, not by
  unmatched tracks — and are removed by `f_i` (ledger Sec. 3).
- Integrated in-observable magnitude (central convention): **0.7056%** of the data vector,
  worst bin 2.20% (h+ b420) — ESTABLISHED via ledger; the 86/18 *magnitudes* are
  alpha0p2-cone-conditional, the matched/fake split itself is structural in the central v4
  builder.
- External det.int. subtraction on top of the central response is FORBIDDEN (ledger Sec. 7):
  double-removal of the 13.82% + wrongful removal of the 86.18%.

### C.3 Empirical verification (ONE file: DJANGOH14 7091, `ep0607_0200`, unweighted, central-selection mirror) — PROVISIONAL magnitudes, ESTABLISHED mechanism

Selected-sample composition: 14,325 GTR-matched signal (97.85%), **122 det.int. (0.833%)**, 3
unmatched (0.020%).

| Observable (det.int. rows, n=122) | Value | Interpretation |
|---|---|---|
| `MatchTier == 0` (H1PartCand truth link) | **100.00%** (signal: 99.91%) | All det.int. matches are Tier-0 own-record links; the 2° greedy Tier-2 (the only ancestor-matching path) contributes **zero**. The load-bearing semantics question is resolved: **matched to their own STR records**, not to ancestors. |
| Opening angle reco-vs-matched-gen | median 2.09°; 50% > 2°; 30% > 5°; 20% > 10° (signal: median 0.32°, 4.9% > 2°) | Larger than signal but *structured*, not random: the signature of **displaced-origin tracks under a primary-vertex fit** (the helix is forced back to the PV). Consistent with own-record matching; ancestor matching would show random, much larger angles with dispersed momenta. |
| `(GenP − P)/P` | median −0.009, centered at 0 | Reco momentum ≈ the secondary's production momentum → own-record kinematics. An ancestor match would be wildly off-center. |
| Mother PDG composition | Σ±(±3222): 41; Σ∓(±3112): 21; Ξ∓(±3312): 14 → **76 (62.3%) hyperon-decay-born**; π±(±211): 24; K±(±321): 10; n(±2112): 6; p(±2212): 6 → **46 (37.7%) meson/baryon-mother** (interaction candidates + K decay-in-flight) | **The det.int. label is NOT pure material interaction.** In this file the majority is weak decays of charged hyperons inside the inner detector (Σ+ cτ=2.4 cm → p/π daughters; Ξ− cτ=4.9 cm → π), which are physics feed-down, not detector artifacts. π/n/p-mother rows are the user's interaction-born Class 2 proper; K-mother rows are dominated by K→ππ⁰ decay-in-flight (decay-born). |
| `VetoAncestorPDG` | 0 for 116/122; 221 (η) for 6 | No V0/γ/π⁰ ancestry (as designed — those are V0/e-flagged and excluded). |
| `ParticleSource` | 0 (kPrimarySignal) for 108; 2 (kXiDecay) for 14 | `classifySourceGTR` (`ChargedSidisUtils_highQ2_v6sys.h` lines 84–109) **cannot identify material secondaries** (returns kSecondaryInteraction only for null input; STR vertex checks were omitted when porting). Σ±-decay daughters read PS=0 because the PS=1 list covers Σ0/Σ* states (3212/3214/3224/3114), not Σ± (3222/3112). `ParticleSource` is NOT a usable Class-2 sub-classifier in v6. |
| `IsFromPrimary == 0` | 0 rows in either class | `H1PartSelTrack::IsFromPrimary()` (`H1PartSelTrack.C` lines 692–721) is a vertex-*fit* bookkeeping bit (false only for secondary-vertex-fitted tracks, i.e. V0-fit daughters), **not** a geometric origin cut. It removes zero Class-2 tracks here. (Corrects the implication in the builder's V5E comment, lines 1388–1391, that STR secondaries fail an `IsFromPrimary` cut.) |

---

## D. Branch inventory and separability verdict

### D.1 Per-track `ChargedTot` branches relevant to class separation (v6 Stage 2, builder lines 678–810)

| Branch | Meaning | Separation power for Class 1 vs Class 2 |
|---|---|---|
| `IsMatched`, `MatchTier` (0=truth link, 2=angular) | MC truth-link existence and mechanism | Class-2 quality check (Tier-0 vs Tier-2) |
| **`IsGtr`** | matched partner is a GTR (generator) record | **Class-1 identifier (with `IsMatchedHadron==1`)** |
| **`IsStr`** | matched partner is an STR-only (simulation-born) record | **Class-2 identifier (with `IsGtr==0`, `IsMatchedHadron==1`)** |
| `IsMatchedHadron`, `IsMatchedElectron`, `IsMatchedMuon` | truth PID of matched partner (π/K/p, e, μ) | species gating |
| `IsMatchedV0Daughter`, `IsMatchedConvOrDalitz`, `IsMatchedK0LDaughter`, `IsMatchedDIFMuon`, `VetoAncestorPDG` | ancestor-walk labels (V0/γ/π⁰/η/Jψ chains; K/π→μ) | removes decay-daughter subclasses from Class 2 |
| `GenPDG`, `GenMotherPDG` | matched partner PID; nearest-ancestor PID | **probabilistic** interaction-born vs decay-born split within Class 2 (no vertex → not clean) |
| `GenPt`, `GenEta`, `GenPhi`, `GenP`, `GenCharge`, `zGen`, `xFGen`, … | matched partner kinematics (own record, secondary's production kinematics for STR) | migration bookkeeping; reco-vs-gen angle is a displacement *proxy* |
| `GenMatchIdx`, `MatchAngle` | set **only for Tier-2** matches (Tier-0 leaves −1/−1) | no parent-index link for Tier-0 (the 100% case for det.int.) |
| `ParticleSource` | `classifySourceGTR` output | **not usable** (never flags material secondaries; Sec. C.3) |
| `DcaPrime`, `DcaPrimeSinTheta` | r-φ DCA of the non-vertex-fitted helix to the PV (`H1PartSelTrack.h` line 32) | reco-side displacement proxy only — empirically overlapping (Sec. D.3) |
| `RadLength` | **radial track length in cm** (`H1PartSelTrack.h` line 31) — NOT radiation lengths X/X₀ | none (distributions near-identical: median 55.5 vs 58.7 cm) |
| `NHitCJC`, `NHitCJC1`, `NHitDedx` | hit counts | selection only |
| `TrackLength` | underlying `H1Track` length, **DATA ONLY** (−999 sentinel on MC; `H1PartCand::GetTrack()` FPACK-aborts in MC per builder comment lines 1622–1633) | would help identify short secondary segments; unavailable on MC |
| `IsFromPrimary`, `IsCentralTrk`, `IsCombinedTrk`, `TrackType` | vertex-fit bookkeeping / track class | near-no-op for class separation (Sec. C.3) |
| `NucliaWeight` | MC nuclear-interaction efficiency weight (π/K) | not a class separator |
| dE/dx values | **removed from the v6 schema** (builder comment line 688) | would have given species/PID handles; absent |

### D.2 Verdict

**MC-truth separation of Class 1 vs Class 2: YES — the trees already carry it, by construction.**
Recipe (per `ChargedTot` row, MC):

- **Class 1 (signal, keep):** `IsMatched==1 && IsMatchedHadron==1 && IsGtr==1`
  (plus the analysis's `IsMatchedElectron==0 && IsMatchedV0Daughter==0` masks).
- **Class 2 (secondary-born, the user's fakes):**
  `IsMatched==1 && IsMatchedHadron==1 && IsStr==1 && IsGtr==0` + the same e/V0 masks —
  **this is exactly the existing det.int. classifier.** The separation is the H1OO truth-link
  itself (same-GEANT-track → GTR; new-GEANT-track → STR-only), not an analysis invention.

**What the current branch set CANNOT do (each with the missing information and the addition
that would supply it):**

1. **Interaction-born vs decay-born split within Class 2** (the user's strict Class 2 = the
   former). Missing: the STR partner's **production vertex/radius** — it exists in the ODS
   banks (`H1OOBanks/ODS/H1Str.h` links to `H1SvxRow`, which carries `GetX/GetY/GetZ/GetIvtype/
   GetTof`) but `H1PartMC` has no vertex members and the MODS-level production inputs carry no
   `H1StrBank` (confirmed by the electron-parentage diagnostic handoff: "There is no
   H1StrBank; SVX/STR fields use −999"). `GenMotherPDG` alone is only probabilistic (Sec. C.3).
   Required addition: a parentage diagnostic at MODS level (mother/ancestor decomposition) and,
   for a clean split, an ODS/DST-level pilot reading SVX (feasibility caveat: ODS/FPACK access,
   libdcap/Alma9 issues recorded elsewhere in this workspace), or a builder extension if STR/SVX
   can be staged.
2. **Kink-merged hybrids** (Class-2 hits inside a Class-1-labeled track, and vice versa).
   Missing: per-track GEANT **hit-truth fractions** (the DTRY bank lists contributing GEANT
   tracks; only the single "best" index is propagated to `H1PartCand::GetPartMC()`). Required
   addition: DTRY hit-fraction content at DST level (also needs UNVERIFIED U1 to close).
3. **Data-side separation.** All class flags are MC-only (ESTABLISHED: identically 0 in data —
   ledger Sec. 4). Reco-side proxies fail: empirically, 90.2% of det.int. tracks survive
   `|DcaPrime| < 1 cm` while 99.53% of signal does (58.2% vs 95.25% at 0.2 cm) — the
   distributions overlap strongly; no reco-only cut separates the classes. (Low-pT displaced
   helices point back near the PV, and the vertex fit pulls tracks toward the PV.)
4. **MC `TrackLength`** (sentinel −999) and **dE/dx** (removed) would have provided
   segment-length and species handles; both are absent from the current schema.

### D.3 Empirical overlap numbers (same single file)

| Reco-side proxy | det.int. | signal | Clean cut? |
|---|---|---|---|
| `|DcaPrime|` median | 0.139 cm | 0.011 cm | No — 90.2% vs 99.53% survive 1 cm |
| `RadLength` median | 55.5 cm | 58.7 cm | No — near-identical |

---

## E. Implication for a detint-fake response variant (spec sketch)

Under the user's definition (Class 2 = fakes to remove, Class 1 = signal to keep):

1. **The move is well-defined and already prototyped.** The all-fake response variant (cluster
   3987932, `output_v6/studies/alpha0p2_allfake_response_z005_20260627/scripts/build_migration_th2_per_charge_allfake.py`)
   already implements the exact routing: `is_detint` defined at lines 694–697 (same classifier
   as above), `matched_gen_ok = gen_candidate_ok & not_explicit_allfake` (lines ~853–857), so
   det.int. rows are excluded from the matched body and routed to the fake column with
   `fake_cat = 3` (priority electron → V0 → det.int., lines 868–874), with per-reco-bin
   category vectors `fake_detint[ir]` (lines 1038–1039) and a **miss-row fill for reclassified
   rows with valid signal-gen candidates** (lines 1044–1067) that keeps the truth marginal
   self-consistent. A **detint-only** variant = central builder + this single reroute (leaving
   e/V0 handling central) removes Class 2 and keeps Class 1 **exactly at the label level**:
   Class-1 (GTR-matched) rows are untouched in the body.
2. **Exactly-once accounting** (ledger Sec. 7): under this variant no external det.int.
   subtraction may be added, and the currently-fake 13.82% det.int. part (gen-event-fail rows)
   must be routed to the same fake column exactly once — the `fake_cat` bookkeeping already
   does this; the central 13.82% and the reclassified 86.18% are disjoint row sets by
   construction.
3. **Observable change:** the response body (and hence the coherent `body+miss` prior, the
   efficiency, and the unfolded observable) loses the det.int. content — integrated
   **0.7056%** of the data vector, worst bin 2.20% (ESTABLISHED magnitudes, alpha0p2-conditional
   transfer caveat). The published quantity changes meaning from "reco hadrons including GEANT
   secondaries, acceptance-corrected to GTR-only truth" to "GTR-origin hadrons only". This is a
   **convention decision for the supervisor/user** (ledger Sec. 8), not settled by this record.
4. **Composition caveat that the user should weigh before adopting it:** the removable block is
   ~62% hyperon weak-decay daughters + ~8% K decay-in-flight + ~30% interaction-born in the
   one-file proxy. A blanket detint-fake variant removes the decay-born feed-down as well as
   the material-interaction fakes. If the intent is strictly the user's Class 2 ("inelastic
   scattering on detector material"), the variant as-is **over-removes**; the stricter variant
   (interaction-born only) is **not buildable from current trees** (Sec. D.2 item 1) and needs
   the SVX/parentage study first. Note also the gen-level counterpart: `ChargedTotGen` is
   GTR-only (builder line 1401), and generator-decayed hyperon daughters ARE GTR π/K/p while
   GEANT-decayed ones are STR-only — so the decay-feed-down treatment is already
   generator-dependent at truth level; removing STR daughters while keeping generator-decayed
   daughters embeds that generator dependence into the observable.
5. Any variant requires the full revalidation cycle (truth conservation, closure, fake
   covariance — P-ALPHA remains open); the 2026-10-05 all-fake diagnostic chain
   (`studies/allfake_alpha0p2_L351000_ibu4_lowq2sub_cradfinal_z005_20261005/`) is the
   end-to-end precedent.

---

## F. ESTABLISHED / PROVISIONAL / UNVERIFIED register

**ESTABLISHED (source- or artifact-backed):**
1. H1OO truth-link semantics: DTRY best STR track → GTR link (same GEANT track) vs StrIdx scan
   (simulation-born track) — `H1CreateSelTracks.C` lines 115–145; `IsGtr/IsStr` from
   `fStrIdx` — `H1PartMC.C` lines 187–196; STR-only records carry own PID/kinematics with
   ancestor-linked `Mo1` — `H1ModsFiller.C` lines 224–287.
2. Class 1 (continuing primary) stays GTR-linked and is treated as signal end-to-end
   (Sec. B).
3. `IsMatchedHadron==1` for a GEANT secondary means **matched to its own STR record** — the
   combined `H1PartMCArrayPtr` (builder line 1372) carries STR-only records; the response's
   matched block for det.int. is filled at the secondary's own gen bin. The 86.18% matched
   det.int. block is therefore **Class-2-as-signal at the label level** under the central
   convention (by design; ledger "in-observable").
4. The det.int. classifier (`IsStr==1 && IsGtr==0 && IsMatchedHadron==1 && e/V0 vetoes`) IS the
   Class-2 label; Class 1 vs Class 2 are separable in MC with the existing flags (Sec. D.2).
5. det.int. is not pure material interaction (skill + ledger); empirically dominated by
   hyperon-decay daughters in the checked file (62.3%) — composition magnitude itself
   PROVISIONAL (single file).
6. Central response: no `IsGtr`/`IsStr` requirement in `matched_gen_ok` (builder lines ~784–806);
   86.18/13.82 split structural; 13.82% fake part ≈92% gen-event Q2/y fail; integrated
   in-observable 0.7056%, worst bin 2.20%; external det.int. subtraction forbidden (ledger).
7. All class flags are MC-only, ≡0 in data (ledger Sec. 4).
8. `IsFromPrimary` is a vertex-fit bookkeeping bit, not a geometric cut (H1OO source + 0-row
   empirical check); `RadLength` is radial length in cm, not X/X₀ (`H1PartSelTrack.h` line 31).
9. `classifySourceGTR` cannot flag material secondaries (returns 4 only for null input) —
   `ParticleSource` is not a Class-2 sub-classifier in v6.
10. The all-fake variant implements the det.int.→fake-column reroute with category vectors and
    miss-row consistency (script lines 694–698, 853–857, 868–874, 1020–1067).
11. SVX (STR production vertex X/Y/Z/Ivtype/Tof) exists at ODS bank level but has no
    `H1PartMC` counterpart and is absent from MODS-level production inputs (electron-parentage
    handoff: "no H1StrBank; SVX/STR fields use −999").

**PROVISIONAL (single-file, one generator/ReqID, unweighted, no cone; needs full-sample
production to close):**
1. det.int. fraction of the selected sample (0.833% in this file vs 0.7056% established
   integrated in-observable fraction — consistent order, file-level fluctuation expected).
2. Mother-PDG composition of det.int. (62.3% hyperon-decay / 37.7% meson-baryon-mother incl.
   ~8% K-DIF candidates); n=122 gives O(±9%) per-bin statistical uncertainty; generator- and
   period-dependence unexplored.
3. Reco-side proxy overlap numbers (DcaPrime/RadLength); the 90%/99.5% survival figures.
4. Tier-2 contribution (0% for det.int., 0.09% for signal in this file).

**UNVERIFIED (with the check that would verify):**
- **U1.** DTRY `GetInt(0)` = "best" STR track: the precise rule (majority of hits?) is a
  DST-simulation-level convention not verifiable from H1OO source. Check: H1SIM/DST
  documentation, or an empirical DTRY hit-fraction study on ODS files. If "best" is not
  majority-hits, the kink-hybrid caveat changes size (not direction).
- **U2.** GEANT3 process-to-track mapping (elastic/ionization keep the same track; inelastic
  creates new tracks; a "continuing leading particle" after inelastic interaction is a new
  track). Standard GEANT3/GHEISHA behavior; check: GEANT3 manual or an STR-chain study
  (parent-chain walk on ODS files).
- **U3.** STR 4-vector = production-point kinematics (vs first-hit/end-of-track). The
  empirical `(GenP−P)/P ≈ 0` and angle structure support it; check: STR/SVX comparison study
  at ODS level.
- **U4.** `H1PartCand::GetTrack()` FPACK abort on MC (TrackLength −999 sentinel) — taken from
  the builder's own comment (lines 1622–1633); independently reproducible only with a live MC
  file test, not attempted here.

**Not determinable from existing artifacts:** the interaction-born fraction of the 86.18%
matched det.int. block on the full sample (needed if the user wants to remove ONLY strict
material-interaction Class 2). The determining study: a **det.int. parentage diagnostic** at
MODS level (full-sample, weighted, per generator/period; mother/ancestor PDG decomposition —
directly analogous to
`docs/diagnostics/electron_parentage_hATmods_cleanrun_2026-07-31.md`, which already records
mother/ancestor/StrIdx fields and whose schema pattern can be reused for π/K/p), plus an
ODS/DST-level SVX production-vertex pilot for the clean interaction-vs-decay split. At the
label level, by contrast, the answer is determinable and stated above: essentially **all** of
the 86.18% matched det.int. block is Class 2 (≈0% Class 1), because a continuing primary
cannot produce an STR-only link by construction (Sec. B/C), modulo the U1 kink-hybrid caveat.

---

## Provenance

- Tree builders: `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` (Stage 2),
  `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` (Stage 1; per ledger,
  its `RejectTracksNearScatElec(0.2)` at line 394 affects hadronic calibration only).
- Utilities: `H1ChargedSidis_HighQ2/src/ChargedSidisUtils_highQ2_v6sys.h`,
  `src/PhysicsConstants_chargedsidis_highQ2.h`.
- Response: `scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py`;
  all-fake variant: `output_v6/studies/alpha0p2_allfake_response_z005_20260627/scripts/build_migration_th2_per_charge_allfake.py`.
- Partition: `output_v6/studies/reco_backgrounds_alpha0p2_z005_combinedL_rescaled_20260625/diagnostics/detint_response_partition_20260626/detint_response_partition_summary.json`.
- H1OO 4.2.1 source (read-only): `h1oo/H1Mods/{H1PartMC.*,H1PartSelTrack.*,H1PartCand.*}`,
  `h1oo/H1Filler/H1ModsFiller.C`, `h1oo/H1TrkFinder/H1CreateSelTracks.C`,
  `h1oo/H1OOBanks/ODS/{H1Str.h,H1Svx.h}`.
- Empirical check (login-node, single file, ≤5 min): DJANGOH14 ReqID 7091 file
  `sidistree_djangoh14_7091_ep0607_0200.root`, tree `ChargedTot`, central-selection mirror
  (`Pt>0.15`, `|Eta|<1.6`, `NHitDedx≥5`, `RadLength≥5`, `|DcaPrime|/cosh(Eta)<2`,
  `IsFromPrimary==1`, event fiducial 150<Q2es<20000, 0.1<Yes<0.7, Empz>45, |PVz|<35,
  xF<0, e/V0 masks). Scripts executed from /tmp only; no workspace product touched.
