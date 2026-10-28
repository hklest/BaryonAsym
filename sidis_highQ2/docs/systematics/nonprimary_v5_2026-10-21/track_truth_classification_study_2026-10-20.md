# Which MC reco tracks are signal? det.int. definition, H1 GTR/STR conventions, and a truth-level signal selection

> **WITHDRAWN 2026-10-21 (v1 classification).** A triple-check (`docs/trackclass_triplecheck_2026-10-21.md`) found real primary tracks in the v1 background classes: Tier-2 matches, P/GenP > 1.5 tracks (badly measured real particles) and part of the large-angle class. The numbers below are superseded by the v2 results; do not quote them.


Study date: 2026-10-20. Read-only on NAF; no remote file was modified. Scripts are in
`scripts/track_class_study/`, run as `ssh klesthen@naf-hone.desy.de "FILE=<root file> python3 -" < <script>`.

**Goal (user, 2026-10-20):** remove every reco track that is not a genuine truth π/K/p from the
primary interaction, including tracks from interactions of real particles with the detector. The
task was to find the precise MC reco-level selection that guarantees this.

## Bottom line

1. **The det.int. label is too narrow, as suspected, but not in the way it looks.** Everything
   that is *not* matched to a π/K/p already goes to the fake column (or to the e/V0 templates).
   The leak runs the other way: non-primary tracks that are **GTR-matched** π/K/p, so they look
   like signal and sit in the response body. The det.int. classifier (`IsStr==1 && IsGtr==0`)
   cannot see them by construction.
2. **The largest leak is "absorbed secondaries":** ~1.0–1.1% of the current body. These are
   soft tracks from interactions whose hits H1SIM books onto a stored ancestor, so the truth link
   resolves to the primary's GTR record. Evidence:
   - p_reco/p_gen ≈ 0.2;
   - |DCA′| ≈ 0.4 cm, vs 0.011 cm for clean tracks;
   - reco charge disagrees with the matched gen charge ~38% of the time, vs 0.0% in the core.
   This is already larger than the whole det.int. class that S2' removes (0.62–0.70%).
3. **The other leaks are:**
   - split-track duplicates, i.e. two reco tracks Tier-0-linked to the same GTR particle: ~0.3%;
   - Tier-2 angular matches with no truth link: 0.09–0.15%;
   - the S2' "K-DIF keeper", which contains K interactions, since 12% of RAPGAP keeper daughters
     are p/K: ~0.05%.
4. **Proposed signal definition (MC reco track):**
   ```
   IsMatched==1 && MatchTier==0 && IsGtr==1 && |GenPDG| in {211,321,2212}
   && IsMatchedElectron==0 && IsMatchedV0Daughter==0
   && sign(Charge)==sign(GenCharge)
   && 0.7 < P/GenP < 1.5 && angle(reco, gen) < 10 deg
   && best (closest) reco row for that gen particle
   ```
   Everything else that passes the reco selection is background. It goes to the fake column,
   except e and V0, which keep their external templates. Relative to today's body this removes an
   extra **1.48% (DJANGOH14) / 1.58% (RAPGAP31)**: **8.7% at lab pT 0.15–0.2 GeV**, 4% at
   0.2–0.3, and ≈0.5% above 0.8 GeV. Window variations move the removed fraction by about ±0.35%
   absolute, which is a definitional systematic.
5. **Routing these tracks to the fake column does not remove MC dependence.** They are still
   subtracted at the MC-predicted rate, so the fake-class normalization uncertainty from
   `docs/detint_uncertainty_audit_2026-10-20.md` now applies to roughly 2.2% of the body
   (0.65% det.int. + ~1.5% new), not 0.65%.

## 1. H1 truth conventions (verified in H1OO 4.2.1 source)

| Object | Source | Meaning |
|---|---|---|
| **GTR bank** | `H1Filler/H1ModsFiller.C::FillMC` (l.~194–221) | All generator particles, including decayed intermediates. Appended to `H1PartMC` with `StrIdx=-1` → `IsGtr()==true`. Mothers `Mo1/Mo2` come from the generator `Pmo`. |
| **STR bank** | same, l.~224–287 | GEANT-transported tracks. An STR row with GTR link (`GetInt(7)>0`) is the transported generator particle and is not appended again. An STR row **without** GTR link (GEANT-created: decay or interaction product) is appended with `StrIdx=i` → `IsStr()==true, IsGtr()==false`. Its own PDG/4-vector are taken from STR. `Mo1` is the nearest ancestor already in the MC array, found by walking the STR parent chain (so in practice the direct parent). The pT/θ threshold is **commented out**: all unlinked STR rows are appended. |
| **SVX bank** | `H1OOBanks/ODS/H1Svx.h` | STR production vertex: X, Y, Z, `Ivtype` (1–5, vertex mechanism), Tof. **Not in MODS/sidistrees.** |
| **DTRY bank** | `H1OOBanks/ODS/H1Dtry.h`, used in `H1TrkFinder/H1CreateSelTracks.C` l.~115–145 | Per reco track: `Str` (best STR track), `Nhit` (hits from it), `Nposs` (possible hits). The truth link is the GTR record if the best STR has a GTR link, else the STR's own `H1PartMC`. **`Nhit/Nposs` (hit purity) is not propagated to MODS or to our trees.** |
| **Tier-0** | builder `ChargedSidisTreeBuilderHighQ2_v6.C` l.1688–1696 | `cand->GetPartMC()`, i.e. the DTRY link above. |
| **Tier-2** | same, l.1699–1727 | Only if Tier-0 is null: greedy nearest GTR/STR π/K/p within 2.0°, same charge, `Mother2==-1`. `usedGenIndices` only blocks Tier-2 reuse, not Tier-0 overlap. |
| **Gen truth (`ChargedTotGen`)** | same, l.1375–1406 | GTR only, π/K/p only, `Mother2==-1`, not a V0 daughter. Generator-decayed products are in, GEANT-decayed products are out. |

Consequences:
- **GEANT-decayed products** are always STR. Weak hyperon decays (Σ±, Ξ, Ω) are all GEANT-side:
  zero GTR π/K/p with a hyperon mother were seen in either file. K± and π± decays are also
  GEANT-side. Charm and bottom decays are generator-side (GTR), i.e. part of the primary lineage.
- A truth link to a GTR record only says that the **best STR track** of the reco track is the
  transported primary. It does **not** say that every hit, or even the hit majority, came from it.
  It also does not say the reco track is the primary itself and not an untracked secondary whose
  hits were booked to the primary. Direct evidence: reco tracks Tier-0-linked to GTR **neutrons**
  (14 rows in one DJ file), and a 38% charge-flip rate in the low-ratio tail.
- Inferred, not verified from H1SIM source: H1SIM does not store every GEANT secondary as its
  own STR record, and hits of unstored secondaries are attributed to the nearest stored ancestor.
  The H1SIM storage criteria (region/energy) should be checked against H1SIM documentation.

## 2. How the current cone-S2' response routes every MC reco track

Builder: `output_v6/studies/cone_alpha_central_build_2026-10-11/copied_scripts/build_migration_th2_per_charge_s2prime.py` (l.~846–1150).

| Truth label (after reco selection incl. α ≥ 0.2 cone, xF < 0, z guard) | Route |
|---|---|
| `IsMatchedElectron==1` (GTR or STR) | **excluded from response**; data gets the external e template |
| `IsMatchedV0Daughter==1` (6-hop ancestor walk: K0S/K0L/K0/Λ) | **excluded**; external V0 template |
| `IsMatched==0` | fake column (fc0) |
| matched, `|GenPDG|` ∉ {π,K,p} (μ, n, Λ, Σ±/Ξ tracks, π⁰…) | fake column (fc0) |
| STR π/K/p, mother ≠ K± (det.int. non-keeper) | fake column (fc3/fc4), S2' reroute |
| STR π/K/p, mother = K± (keeper) | **body** |
| GTR or Tier-2 π/K/p, gen event in fiducial and in grid | **body**, regardless of kinematic consistency or duplication |
| GTR π/K/p, gen event out of fiducial | fake column (fc0), i.e. migration fakes |

So "too narrow" is really a body-purity problem. The body is everything matched to a π/K/p that
is not a non-keeper STR record.

## 3. Census (one ep0607 file per generator, w1·w2 weights)

Script: `track_truth_census.py`. "Body" is body-*eligible*: GenEmpz > 45 and gen-grid membership
are not re-applied, so it is a slight upper bound.

Selected-track composition, % of selected sumw:

| | DJ14 7091 `_0200` (15,760 trk) | RG31 7094 `_0002` (≈199k trk) |
|---|---:|---:|
| current body: GTR π/K/p | 82.76 | 82.56 |
| fake: GTR π/K/p with gen event out of fiducial | 7.98 | 7.93 |
| excluded e / V0 | 3.90 / 3.33 | 4.12 / 3.25 |
| fake: det.int. | 0.70 | 0.61 |
| fake: GTR non-π/K/p (μ, n, Λ, Σ, Ξ) | 0.61 | 0.63 |
| fake: STR non-π/K/p (mostly μ from π/K DIF) | 0.59 | 0.63 |
| body: Tier-2 | 0.07 | 0.13 |
| body: keeper | 0.04 | 0.05 |
| unmatched | 0.01 | 0.09 |

Body mothers (GTR π/K/p): resonances 72%, string/cluster 19–20%, **charm 7.4–7.8%**, bottom
0.3–0.5%. These are all primary lineage by the gen-truth definition.

### 3.1 Absorbed secondaries: GTR-linked π/K/p inconsistent with their gen particle

Script: `ratio2d.py`. Unique GTR π/K/p Tier-0 rows (RG):
- 98.9% have 0.85 < p_reco/p_gen < 1.18;
- 0.51% have ratio < 0.3, and **0.67% have ratio < 0.5**.

The low-ratio tail has reco pT 0.23 GeV (core 0.78), gen p 2.3 GeV (core 1.1), and |DCA′|
0.38 cm (core 0.011). Charge agreement by ratio bin (`qagree.py`, RG):

| p_reco/p_gen | <0.3 | 0.3–0.5 | 0.5–0.6 | 0.6–0.7 | 0.7–0.8 | 0.8–0.85 | 0.85–1.18 | 1.18–1.3 | 1.3–1.5 | 1.5–2 | >2 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| charge agree | 0.60 | 0.67 | 0.73 | 0.76 | 0.88 | 0.97 | **1.000** | 0.95 | 0.89 | 0.67 | 0.57 |

| angle (within 0.7<ratio<1.5) | 0–1° | 1–2° | 2–5° | 5–10° | 10–20° | 20–45° | >45° |
|---|---|---|---|---|---|---|---|
| charge agree | 1.000 | 1.000 | 0.998 | 0.983 | 0.973 | 0.916 | 0.41 |

Secondaries come with either sign, so the non-primary share is about 2.5 × (1 − agreement). It
is ~100% below ratio 0.5, ~30% at 0.7–0.8 and ~8% at 0.8–0.85, so the boundary near 0.7–0.85 is
where the ambiguity lives. Genuine primaries never flip charge in the core.

det.int. rows, matched to their own STR record, peak at ratio ≈ 1 with large angles (displaced
origin), as expected.

### 3.2 Split-track duplicates

Script: `dup_pairs.py`. 84 groups with ≥2 selected rows on the same gen particle in one DJ file
(0.50% of body; RG 0.48%). The typical pair is one long CJC2-dominated track plus one short
**CJC1-only** segment (NHitCJC = NHitCJC1 ≈ 9–17, RadLength 11–20 cm), with the summed hits
≈ one track. About a third of duplicate-group weight is itself absorbed-secondary-like (ratio
< 0.5), and the rest are genuine splits. The builder never deduplicates, so the gen particle
enters the body twice.

Reco-cut handles on the extra copies (DJ):
- `RadLength ≥ 15 cm` kills 35% of them at 0.6% signal cost;
- `≥ 20 cm` kills 54% at 6.6% signal cost;
- `NHitCJC ≥ 15` kills 37% at 0.8% signal cost.

None is clean. Dedupe in truth instead.

### 3.3 Keeper and Tier-2

- Keeper daughters: DJ 100% π (n=10); RG π 88%, p 8.6%, K 3.3%. p and K cannot come from a
  K± decay, so the keeper contains K inelastic interactions. K→μν (~64% of K decays) already
  goes to the fake column (μ is not in the pool), so keeping K→ππ⁰ daughters as signal is also
  internally inconsistent.
- Tier-2: rows with no DTRY link, angular-matched to a GTR π/K/p. None duplicates a selected
  Tier-0 row. There is no truth evidence that they are the matched particle.

## 4. Proposed truth-level signal selection and its effect

Applied to the current body (`track_truth_census.py`, window 0.7 < P/GenP < 1.5, angle < 10°):

| | DJ14 | RG31 |
|---|---:|---:|
| kept: clean primary-lineage π/K/p | 98.52% | 98.42% |
| removed: GTR-linked, kinematically inconsistent | 0.67% | 0.75% |
| removed: GTR-linked, charge flip | 0.35% | 0.34% |
| removed: duplicate copy | 0.32% | 0.27% |
| removed: Tier-2 | 0.09% | 0.15% |
| removed: keeper | 0.05% | 0.06% |
| **total additionally removed** | **1.48%** | **1.58%** |
| reference: S2' det.int. reroute (already removed) | 0.70% | 0.62% |

Window systematic (RG, fraction of GTR-π/K/p rows failing): 1.10% (0.5–2, <20°), 1.19%
(0.6–1.7, <15°), 1.34% (nominal), 1.83% (0.8–1.25, <5°).

Per-bin (RG, `perbin.py`), removed share of the current body:

| lab pT [GeV] | 0.15–0.2 | 0.2–0.3 | 0.3–0.5 | 0.5–0.8 | 0.8–1.2 | 1.2–2 | 2–5 | >5 |
|---|---|---|---|---|---|---|---|---|
| removed | 8.70% | 3.99% | 1.79% | 0.83% | 0.57% | 0.53% | 0.45% | 0.82% |

| reco z | <0.05 | 0.05–0.1 | 0.1–0.2 | 0.2–0.3 | 0.3–0.5 | 0.5–0.7 | >0.7 |
|---|---|---|---|---|---|---|---|
| removed | 2.14% | 0.85% | 0.57% | 0.53% | 0.47% | 1.18% | 0.99% |

The dependence on Q2 is flat (1.4–2.1%).

## 5. Reco-level complement (acts on data too)

Script: `dcascan.py`, RG. Fraction of each class surviving |DCA′| < cut:

| class (% of sel.) | 2 cm | 1 | 0.5 | 0.3 | 0.2 | 0.1 |
|---|---|---|---|---|---|---|
| clean signal (88.5) | 1.000 | 0.999 | 0.993 | 0.980 | 0.961 | 0.910 |
| absorbed secondary (1.26) | 0.978 | 0.806 | 0.623 | 0.488 | 0.361 | 0.215 |
| split duplicate (0.25) | 0.994 | 0.958 | 0.847 | 0.739 | 0.626 | 0.454 |
| det.int. (0.65) | 0.990 | 0.895 | 0.763 | 0.636 | 0.520 | 0.322 |
| non-π/K/p matched (1.25) | 0.998 | 0.950 | 0.878 | 0.808 | 0.747 | 0.629 |
| electron (4.64) / V0 (3.23) | 0.99 | 0.93/0.94 | 0.84/0.86 | 0.75/0.76 | 0.67/0.64 | 0.51/0.42 |

A tighter |DCA′| < 0.5 cm, against the current |DCA′/cosh η| < 2, would cut every fake class
by 12–38% for 0.7% signal loss, rising to ~7% loss at pT < 0.3 GeV for a 0.3 cm cut. That
reduces how much rides on the MC-predicted fake rate, but it needs a data/MC DCA′ validation and
does not replace the truth definition.

## 6. Implementation notes (for when this is adopted)

- Add the predicate in `compute_reco_bin` of the S2' builder as an extra reroute, e.g.
  `fc=6 absorbed`, `fc=7 duplicate`, `fc=8 tier2`, plus making the keeper rerouted. All needed
  branches are in the trees: `P`, `GenP`, `Charge`, `GenCharge`, `Eta`, `Phi`, `GenEta`,
  `GenPhi`, `MatchTier`.
- **Miss-row consistency is the trap.** S2' reroutes leave the CTG consumed key set. That is
  harmless for STR det.int. rows, whose STR particles are not in the GTR-only gen truth. The new
  reroutes are **GTR-linked**, so their gen particle *is* in truth:
  - If a clean leg also consumes it (duplicate case), nothing changes.
  - If no clean leg exists, the gen particle **must fill the miss row**. Otherwise it vanishes
    from body+miss, which biases the prior and the efficiency.
  Use the existing (Run, Event, signed GenPDG, round(GenPt,5), round(GenEta,5)) key.
- Dedupe has to be per event. The builder already processes per-event chunks and has the key
  machinery. Rank copies by |ln(P/GenP)| + angle/10°, as in the census.
- The data vector is unchanged; `f_i` grows by ~1.5% integrated, and by up to ~9% at the lowest
  pT.
- Re-run the gate suite (closure, refold, G9-style `f_i` identity) and add a normalization
  systematic for the enlarged fake class.

## 7. Caveats

- Two files only, both ep0607, one per generator; the 0.5–1.6% numbers agree between them. A
  full-sample census (all periods, both generators) should go through Condor before adoption.
- Body eligibility omits GenEmpz > 45 and gen-grid membership. The z guard uses the tree
  `z_SIDIS_es` instead of the builder's own recomputation.
- The absorbed-secondary mechanism (unstored GEANT secondaries booked to the ancestor) is
  inferred from the charge-flip and GTR-neutron evidence. It is not verified from H1SIM code.
- Kink hybrids (primary plus secondary hits merged, labelled by the best STR) cannot be resolved
  without DTRY `Nhit/Nposs`. The ratio/angle window catches only those that distort the
  kinematics.
- The keeper decision was a user ruling (2026-10-09). Removing it needs a new user decision.

## 8. Decisions needed and suggested next steps

1. **Adopt the signal definition?** Choose the window: nominal 0.7–1.5 / 10° (recommended),
   with the alternatives above as the systematic.
2. **Keeper:** remove it (recommended), consistent with K→μν already being fake and with the
   p/K daughters seen.
3. **Tier-2:** treat as fake (recommended), or apply the same consistency window.
4. Run a full-sample Condor census (`track_truth_census.py` per file, aggregated) for all
   periods/generators, plus per-analysis-bin removal maps.
5. Optional, for a cleaner definition: ODS/DST pilot reading DTRY `Nhit/Nposs` and SVX
   `Ivtype`/vertex for one MC file (the parentage design §H pilot). That would replace the
   kinematic window with hit purity and production vertex, and would settle the H1SIM storage
   question.
6. Fold the enlarged fake class into the normalization-uncertainty term proposed in
   `docs/detint_uncertainty_audit_2026-10-20.md`.
