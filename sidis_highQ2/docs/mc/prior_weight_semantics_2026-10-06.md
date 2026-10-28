# v6 response prior-weight semantics — 2026-10-06

**Scope.** Read-only source review for the coherent-prior response rebuild. No ROOT file was opened. Claims are explicitly labelled **VERIFIED AGAINST SOURCE** or **PROVISIONAL**.

## Bottom line

**VERIFIED AGAINST SOURCE:** `weight1` and `weight2` are *event-level HAT weight columns*, copied to every row of both `ChargedTot` and `ChargedTotGen`; they are not independently calculated per reconstructed or generator track. The v6 Stage-2 builder reads them once before either the gen or reco loop and both trees branch the same two storage variables. Therefore a genuine matched pair produced by the same event cannot have a source-level difference between `ChargedTotGen.weight1*weight2` and its `ChargedTot` partner's `weight1*weight2`.

**VERIFIED AGAINST SOURCE:** `NucliaWeight` exists only on `ChargedTot`, is a reconstructed-track nuclear-interaction correction, and is part of the matched response numerator as `w_reco = weight1*weight2*NucliaWeight` (after the response script’s finite-value replacement and `[0,10]` clipping). It is not a generator event weight and is not stored on `ChargedTotGen`.

**VERIFIED AGAINST SOURCE:** the exact source-level mechanism capable of making the current NWeff construction differ from `body+miss` is that the response body fills **every** accepted `ChargedTot` matched row, whereas `ChargedTotGen` is traversed once per truth row and the NWeff path stores only one `NucliaWeight` per rounded truth key. The stage-2 truth-link matching does not mark a truth object consumed; only the angular fallback uses `usedGenIndices_ch`. Thus two (or more) reco rows can be linked to one generator object through the preferred truth link. In that case the body contains all corresponding `w1*w2*NW_i` terms, but NWeff contains exactly one `w_gen*NW_partner` term (the dictionary value last assigned for that key). This is a many-reco-to-one-gen bookkeeping asymmetry, not an event-vs-track `weight1*weight2` difference.

**PROVISIONAL (no ROOT inspection permitted):** this mechanism is the best source-backed explanation of the reported per-bin 0.968–1.033 NWeff/(body+miss) scatter. The reported totals alone do not prove how many duplicated consumed keys occur, nor exclude rounded `(PDG,pt,eta)` key collisions or a selection/key mismatch. A read-only audit of duplicate consumed keys and their per-key body/NWeff contributions would be required to establish the exact fraction.

The physically correct truth marginal and the IBU efficiency denominator is **the response’s own `body+miss` marginal**, not the existing NWeff construction. This follows from response consistency: every response-matrix truth-column contribution must appear once and with the identical weight in its marginal. The `body+miss` construction has this identity by construction. The NWeff scheme is only equal to it under the additional condition of one accepted reco match per truth row and identical base event weights; the builder/source does not enforce the first condition. It must not be promoted as a truth marginal merely because it was intended to compensate the reco-only nuclear correction.

## 1. Builder payloads

### `weight1` and `weight2`

**VERIFIED AGAINST SOURCE:** Stage 1 takes `weight1` from `hatWeight1` and (for MC) `weight2` from `hatWeight2` in its ordinary event loops:

- `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C:1315-1320`;
- the analogous second event-loop block at `:2260-2265`;
- its gen-stage block also uses the same HAT columns at `:2865-2869`.

The Stage-2 v6 builder declares one shared pair of variables, explicitly labelled `// MC weights`, at `src/ChargedSidisTreeBuilderHighQ2_v6.C:524-529`. For each event it assigns this shared pair before either output loop:

- on reco-envelope events, `weight1=*hatWeight1`, `weight2=*hatWeight2` on MC at `:1027-1041`;
- on gen-only MC events it still retains `weight1=*hatWeight1`, `weight2=*hatWeight2`, with the comment that HAT `Weight2` is the gen weight, at `:1042-1050`.

Both tree schemas point to those same shared variables:

- `ChargedTot.weight1/weight2` at `src/ChargedSidisTreeBuilderHighQ2_v6.C:745-746`;
- `ChargedTotGen.weight1/weight2` at `:812-853` (specifically `:852-853`).

The `ChargedTotGen` loop fills once per retained generator hadron at `:1374-1533`; it does not reassign either weight in the loop. The reco central-track loop fills `ChargedTot` at `:1536-1903`, also without reassigning them. Hence `weight1*weight2` is an event factor denormalized into each track row in both trees, rather than a reco-vs-gen or track-dependent reweight.

### `NucliaWeight`

**VERIFIED AGAINST SOURCE:** `NucliaWeight` is a `ChargedTot` branch only (`src/ChargedSidisTreeBuilderHighQ2_v6.C:701-710`, especially `:706-707`); no corresponding `ChargedTotGen` branch is created in its schema (`:812-864`).

For a central reconstructed track, Stage 2 initializes it to unity and for MC calls `H1NuclIACor::GetWeight` with the *reconstructed* track’s charge, `Pt`, `Phi`, `Theta`, `DcaPrime`, and event `PVz` (`:1636-1650`). The provisional pion hypothesis uses `ipart=1`. After a successful truth association it is refined from the matched species: protons are set to 1 and kaons use `ipart=2`, retaining the same reconstructed-track arguments (`:1830-1840`). Thus it is neither a generator cross-section weight nor a per-event generator weight. Combined tracks are also assigned a reco-side pion-hypothesis correction and have no matching refinement (`:1983-1996`).

## 2. Canonical response algebra and discrepancy

### Canonical implementation

**VERIFIED AGAINST SOURCE:** In the canonical builder `scripts/canonical_highQ2_sidis_2026-06-30/unfolding/build_migration_th2_per_charge_v6.py`:

1. The `ChargedTotGen` base weight is `w_gen = weight1*weight2` at `:464-470`.
2. The `ChargedTot` base response weight is `w_reco = weight1*weight2*wn`, with non-finite `NucliaWeight -> 1` and clipping to `[0, NUCLIA_CLIP]`, at `:591-603`.
3. The matched response body is filled with `wr` for every accepted matched reco row at `:818-826`. The match’s rounded `(Run,Event,signed PDG,GenPt,GenEta)` key is mapped to the partner NW at `:827-844`.
4. A gen row not represented by a consumed key is sent to the miss row with `w_gen` at `:852-875`.
5. NWeff visits each accepted gen row once, assigns `w_gen*NW_partner` if its key is present and `w_gen` otherwise, at `:877-900`.

The code comments correctly state the *intended* equality/acceptance rationale (`:877-882`), but the implementation’s equality is conditional, not guaranteed by the code.

### Why equal base weights do not guarantee equality

**VERIFIED AGAINST SOURCE:** The preferred Stage-2 match is `H1PartCand::GetPartMC()` (`src/ChargedSidisTreeBuilderHighQ2_v6.C:1688-1696`). The `usedGenIndices_ch` set is consulted and populated only in the fallback angular matcher (`:1698-1727`), not after an accepted truth-link match. Every non-null `matched` pointer sets `IsMatched_ch=1` and is eligible to populate the inlined truth fields (`:1730-1828`). No source-level one-to-one guard protects truth-link matches.

Consequently, the response script’s `consumed_keys` dictionary is a **deduplication device for the gen/miss and NWeff passes**, not a deduplication device for body entries: the body has already been filled once per `ChargedTot` row (`canonical script :818-826`); the dictionary only holds one NW value per key (`:827-844`) and NWeff subsequently visits each `ChargedTotGen` row once (`:884-900`). For a key with two body legs,

```
body(key) = (w1*w2)*NW_A + (w1*w2)*NW_B
NWeff(key) = (w1*w2)*NW_last
```

while that gen row is removed from the miss path. This breaks the claimed NWeff = body+miss identity even though the base factors are identical. It can be either above or below unity depending on multiplicity and NW values, compatible in sign with the observed binwise scatter.

**PROVISIONAL:** the numerical median and range quoted in the task have not been re-derived here, and source review cannot identify whether truth-link duplicates, key rounding collisions, or both dominate. The alternative proposed in the question—different `ChargedTotGen.weight1*weight2` and matched `ChargedTot.weight1*weight2` fill payloads—is contradicted by the common-variable fill architecture above, barring an unobserved corruption outside these sources.

## 3. Rebuild-copy comparison

**VERIFIED AGAINST SOURCE:** The study copy is `output_v6/studies/coherent_prior_response_rebuild_z005_20261006/scripts/build_coherent_prior_response_shard.py`. For the NWeff algorithm itself it faithfully implements the canonical design:

- it fills the body per accepted matched reco row, `:1077-1084`;
- it records the rounded consumed key and one NW value, `:1085-1103`;
- it fills unconsumed gen rows as misses using `w_gen`, `:1111-1134`;
- it performs one gen-row NWeff fill as `w_gen*consumed_keys.get(key,1)`, `:1136-1159`.

The copy’s direct extraction of finite, clipped `NucliaWeight` (`:1095-1102`) is algebraically equivalent to canonical’s `wr/(weight1*weight2)` (`canonical :837-844`) for the canonical base weight, and avoids accidentally including the copy’s truth-phi modifier in NW. It therefore does **not** explain the reported discrepancy by departing from the canonical NWeff convention.

**VERIFIED AGAINST SOURCE, qualification:** the copy is not a byte-for-byte canonical response rebuild: it intentionally changes the response fiducial/grid and supports generator-truth phi modifiers (documented in its header `:3-193`; base/matched weighting at `:687-695`, `:1066-1076`). Those intentional study changes are separate from the NWeff key/weight algorithm. The faithful statement is therefore: **faithful to canonical NWeff fill semantics, not identical to the full canonical response configuration.**

## 4. Correct prior for IBU and efficiency

**VERIFIED AGAINST SOURCE:** Let `R_body(r,g)` denote the finite reco/gen body that IBU consumes, and `R_miss(g)` its overflow/miss row. The truth marginal intrinsic to that response is

```
T_response(g) = sum_r R_body(r,g) + R_miss(g)  = body(g)+miss(g).
```

This is the sole denominator that has an exact response-matrix conservation identity, because both terms are filled directly into `R` with their respective weights (`canonical :818-826`, `:852-875`). Therefore use

```
prior(g) = body(g)+miss(g)
epsilon(g) = col_inner(g) / [body(g)+miss(g)]
```

for the coherent IBU prior and the efficiency consumed by IBU. This conclusion is also consistent with the current project state record: `sidis_highQ2/STATE.md:61-68` describes the central plain `w_gen` prior as defective and identifies `body+miss` as the validated coherent prior/efficiency basis; this memo independently establishes why NWeff cannot replace it when one-to-one matching is not enforced.

**VERIFIED AGAINST SOURCE:** NWeff attempts to transport a reconstruction-only nuclear correction onto the truth leg. That can be a useful *diagnostic conditional reweighting* under a strictly one-to-one match relation, but it is not a generator truth marginal: its matched-leg factor depends on a selected reconstructed partner and its detector-track correction. With the extant many-to-one possibility, it cannot represent every response column contribution once. A response-consistent efficiency must share the response matrix’s truth marginal, not a reconstructed-track-dependent proxy.

**PROVISIONAL (physics-policy boundary):** this memo does not decide whether nuclear-interaction correction should ultimately be represented as a response weight, an explicit correction, or a systematic; that is an unfolding/physics-method decision. It establishes only the required algebra if the already built weighted response matrix is the object consumed by IBU.

## Required follow-up (read-only)

**PROVISIONAL:** To close the numerical cause without modifying central code/products, make a key-level audit in the study area that records, per charge and gen bin: (i) count of body legs per consumed key, (ii) all body `wr` values, (iii) stored/last NW, (iv) NWeff contribution, and (v) rounded-key collision candidates. If all consumed keys are unique, investigate file/chunk event alignment or an unrecorded fill path; source inspection alone then cannot account for the observed ratios.
