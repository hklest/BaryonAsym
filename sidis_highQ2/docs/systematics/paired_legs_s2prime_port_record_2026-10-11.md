# S2′+shift Stage1 builder port record — 2026-10-11

One-time port of the S2′ detint-fake response delta into the paired-legs
shifted Stage1 builder, per
`paired_legs_s2prime_protocol_2026-10-11.md` §6.2 (items 1–8) and the
one-time port-validation contract (a)–(h). This record is the provenance
and validation evidence for the shared campaign builder used by all 12
paired detector-leg campaigns.

## Ported artifact

- **Builder (NEW, additive):**
  `H1ChargedSidis_HighQ2/scripts/unfolding/paired_legs_detints2prime_2026-10-11/build_shifted_response_data_vector_s2prime_v4.py`
  (2666 lines; dated campaign builder shared by all campaigns; no shared
  script was edited).
- **Parents:**
  - mechanical base (byte-faithful outside the delta):
    `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_shifted_response_data_vector_v4.py`
  - S2′ delta source (ported verbatim):
    `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`
- Shared dependencies (`binning_v3`, `_compute_pTh_cm_and_z`,
  `shifted_electron_kinematics_v4`) are imported from the legacy scripts
  directory via `sys.path`; the port hard-fails (SystemExit) if that
  directory is missing.

### Delta vs the legacy shifted builder (one line each)

1. `S2P_REQUIRED_BRANCHES` (`IsStr`, `IsGtr`, `GenMotherPDG`) added to the
   MC migration-mode read set with a fail-fast SystemExit (R10: a missing
   classifier branch would silently produce the S1 no-reroute response);
   `IsMatchedElectron` was already fail-fast via `CT_DATA_REQUIRED`.
2. Classifier/keeper/reroute verbatim: `is_detint` (IsStr & !IsGtr &
   IsMatchedHadron & !IsMatchedElectron & !IsMatchedV0Daughter),
   `is_kpar = is_detint & (|GenMotherPDG| == 321)` (direct mother ONLY,
   never `VetoAncestorPDG`), `s2p_reroute = is_detint & ~is_kpar`.
3. Matched gate: `matched_gen_ok = gen_candidate_ok & ~s2p_reroute`
   (keeper legs keep the central matched fill).
4. Fake-column categories fc3 (rerouted matched non-keeper) / fc4
   (pre-existing fake non-keeper) / fc5 (pre-existing fake keeper) / fc0
   (unflagged); fc3 legs fill `R[ir_shifted, GEN_OVERFLOW]` with the same
   `w_reco`, dispatched by GEN charge (the dispatch of the body cell they
   left), so per charge and per (shifted) reco bin `B_ij + F_i` is exactly
   conserved; per-reco-bin category TH1s `h_fake_detint_rerouted_*`,
   `h_fake_detint_prefail_*`, `h_fake_detint_keeper_prefail_*`,
   `h_fake_existing_other_*`, `h_keeper_retained_body_*`,
   `h_rerouted_reco_marginal_*` (each + explicit `_w2` companion) and the
   S2′ counters (`n_rerouted_*`, `n_keeper_retained_*`, `n_detint_*`,
   `n_rerouted_key_collisions`).
5. Miss-fill SUPPRESSION for reclassified legs (prototype reclassified-row
   miss fill NOT ported), with the legacy consumed-gen-key recording KEPT
   for rerouted legs, so the miss row and `prior_gen_NWeff` are
   bit-identical to the same-shift legacy fill by construction; rerouted-key
   collision counter kept as a defensive audit.
6. Metas stamp BOTH `meta_variant_id =
   detint_fake_response_variant_S2prime_2026-10-09` AND the legacy shift
   stamps (`meta_electron_energy_scale`, `meta_shift_name`,
   `meta_photon_mode`, …); QA keeps the legacy schema string
   `shifted_response_data_vector_v4_qa` and adds top-level `variant_id` +
   S2′ counters (consumable by the §6.3 step-1 steering guard).
7. `ReroutedLegsS2prime` dump TTree (`run, evt, chg, reco_chg, ir, ig,
   weight`) behind `--dump-rerouted-legs`; `compute_shifted_reco_bin`
   extended to a 7-tuple (adds `pass_pre_z`) so the det.int. counters keep
   the s2prime-parallel prez/postz/infill semantics (no z guard /
   finiteness-only guard / in-grid).
8. DO-NOT-PORT list held: e/V0 truth-veto disablement, alpha cone, the
   prototype's reclassified-row miss fill, and the S2′ builder's
   `z_outlier` range guard (the legacy finiteness-only z guard is kept, so
   the port stays bit-comparable to the same-shift legacy control);
   data-vector mode inherited unchanged and NOT used for the data leg.

## Output-object compatibility (merge contract)

Verified present in every port product (`a_*`/`i_*` checks):

- `merge_migration_per_charge_v4.py` fixed central set: `h_R_{pos,neg}`,
  `h_R_{pos,neg}_w2`, `h_prior_gen_{pos,neg}`,
  `h_matched_reco_marginal_{pos,neg}`, `h_mc_count_{pos,neg}`,
  `h_binmap_gen_to_reco` (binmap bit-identical to legacy).
- `merge_migration_per_charge_s2prime.py` `NAMES` (all 13 per-charge
  objects incl. the six S2′ category/keeper vectors + `_w2` companions)
  and `META_INT_SUM` metas (shard-summed integer counters), imported
  directly from the merge script in the verifier.
- The ROOT key set is EXACTLY the legacy key set plus 58 additive S2′
  objects (no legacy key removed, no unexpected extra key); the Stage2
  driver pair is reused unmodified.

## Validation

Environment: LCG_109a `python3` on the NAF login node (single-file,
≤5 min wall each; builder smoke ~10–30 s per run, verifier ~5 s).

1. **Compile:** `python3 -m py_compile` clean.
2. **Self-test:** `--self-test` ALL 13 PASS (11 legacy-behaviour tests +
   S2′ reroute/keeper identities + S2′×shift composition at −0.005).
3. **One-time port smoke** (protocol §6.2, contract (a)–(h)): the port at
   `--electron-energy-scale -0.005` and its zero control on the campaign-1
   smoke units (RAPGAP31 `7097/..._ep0304_0000.root`, DJANGOH14
   `7093/..._ep0304_0000.root`), against the EXISTING same-file same-shift
   legacy smoke products + zero controls, plus the canonical S2′ builder
   (unshifted) as a soft reference. Outputs:
   `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/builder_port_smoke_2026-10-11/`
   (6 ROOT + 4 QA JSON + `ReroutedLegsS2prime` dumps).
4. **Verifier:** `verify_port_smoke_s2prime_2026-10-11.py` (same
   directory) — **82/82 check items PASS per generator, sections (a)–(i)
   all PASS, `SMOKE_VERDICT.json` `overall_pass: true`** (exit 0).

### Verdict highlights (per generator, shifted run vs legacy same-shift control)

| Quantity | RAPGAP31 | DJANGOH14 |
|---|---|---|
| rerouted legs (pos/neg) = dump legs | 605 / 568 (= 1173) | 595 / 536 (= 1131) |
| keeper-retained (pos/neg); keeper pool = retained + prefail | 66 / 59; 140 = 125 + 15 | 64 / 53; 130 = 117 + 13 |
| non-keeper pool = rerouted + prefail-nonkeeper | 1292 = 1173 + 119 | 1277 = 1131 + 146 |
| det.int. selection prez/postz/infill (raw scan == metas) | 1435 / 1435 / 1432 | 1409 / 1409 / 1407 |
| keeper mother-PDG set | {−321, +321} only | {−321, +321} only |
| matched identity (pos) | 76526 − 605 = 75921 | 67866 − 595 = 67271 |
| fake identity (pos) = fc0+fc3+fc4+fc5 | 6607 + 605 = 7212 = 6537+605+63+7 | 6385 + 595 = 6980 = 6294+595+82+9 |
| reco-side counts == legacy same-shift (ps = in-bin) | 163725 | 146987 |
| gen side (n_gen_total/in_ps/in_bin) == legacy | 1936331 / 324596 / 324263 | equal (check c PASS) |
| E′ ratio (shared shift kernel, input file) | 0.995 exact, 311573 rows, max dev 1.1e-16, uniform over 5 wheel θ slices | 0.995 exact, 282895 rows |
| G9 identity `f′−f == D/(B+F)` max residual | 1.6e-16 (pos) | 1.4e-16 (pos) |
| f_i direction | increases only in D>0 bins (95/88 bins) | increases only (108/107 bins) |
| chunk-integrated f_i delta (weighted, pos/neg) | +0.688 / +0.699 pp | +0.751 / +0.703 pp |
| B+F row-sum conservation max diff | 5.5e-12 | (check e PASS) |
| body-cell diff vs dump legs max residual | 5.7e-12 | 4.7e-12 |
| `n_rerouted_key_collisions` (recorded, not gated) | 9 | 19 |

Bit-identical to the same-shift legacy control (np.array_equal): miss row,
`h_prior_gen_*`, `h_prior_gen_*_NWeff`, `h_mc_count_*` (values+errors), all
non-rerouted body/fake cells, `h_binmap_gen_to_reco`. Stamps: ROOT
`meta_electron_energy_scale` −0.005 (shifted) / 0 (zero), `meta_shift_name`
"zero", `meta_photon_mode` "shifted_electron", exact
`meta_variant_id` in every product (shifted AND zero), QA top-level +
`migration_summary` `variant_id`.

Composition checks (i): (port_sh − port_z) == (legacy_sh − legacy_z) full
row sums per (charge, reco bin); port gen-side counts and
priors/mc_count bit-identical across shifts. Note:
`prior_gen_NWeff` is reco-dependent by construction (consumed-key lookups)
and differs shifted-vs-zero in the LEGACY builder as well; per-shift
identity vs legacy is proven bit-identical in (d).

Soft reference (REPORT ONLY): port-zero vs canonical S2′-nominal builder
counters agree within ±1–2 legs (e.g. RG31 detint prez 1438 vs 1439,
rerouted pos 603 vs 604, keeper pos 67 vs 67; collisions equal), the
expected signature of recomputed-vs-stored reco kinematics plus the
z-guard convention difference (DO-NOT-PORT item).

## Downstream impact

- No shared script was edited; no consumer needs a schema change: the v4
  merge, the S2′ diagnostics merge, and the coherent IBU driver pair are
  reused unmodified.
- Campaign 1 (eElecEn DOWN): MC migrations to be REDONE with this builder
  (8 Condor jobs, same lists, same −0.005; hand-off to h1-naf-operator);
  data vector REUSED; Stage2 per §6.3 in a NEW dated sibling workspace;
  per-campaign `SMOKE_VERDICT.json` and refreshed normalization-gate
  record per §6.5.
- Campaign 2 (eElecEn UP): re-point the campaign-local steering to this
  builder (workspace exists, nothing submitted).
- Campaigns 3–12: their per-campaign shifted-builder ports follow the same
  §6.2 checklist (the S2′ delta rides the planned HFS/theta/phi machinery
  builds).
- Steering-level variant-id guard (§6.3 step 1) can key on the QA
  `variant_id` / ROOT `meta_variant_id` stamped by this port.
