# P2 paired detector-leg campaigns — S2′ detint-fake basis replan protocol (2026-10-11)

- **Status:** REPLAN PROTOCOL (read-only determination + revised campaign plan). Nothing was
  launched, submitted, or rebuilt by this document.
- **Trigger:** the 2026-10-10 S2′ detint-fake promotion (user decision; documented 2026-10-11 in
  `sidis_highQ2/STATE.md` end-of-file override section and
  `sidis_highQ2/docs/RUN_STATUS.md` remaining-work ledger item 1) changed the canonical raw
  central release to
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`.
  The 12-campaign P2 paired detector-leg program (user-approved, decision
  `D-2026-10-10-3` in `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`) was
  configured against the pre-S2′ canonical chain; STATE.md open follow-up #1 requires the
  detector-leg shifted-response productions to be rebuilt with the detint-fake builder to
  restore the central full-chain convention on the variant nominal.
- **Scope of this document:** (1) determine, from the builder/driver sources, the S2′ spec Rev 2,
  the S2′ study campaign, and the installed S2′ release metadata, exactly where the detint-fake
  change enters the paired-leg chain and what it invalidates; (2) issue the revised per-campaign
  protocol. Frozen shift prescriptions (± amplitudes, five-wheel co-shift, campaign order,
  symmetrisation rules) are UNCHANGED.
- **Determination basis (all read-verified 2026-10-11):**
  - S2′ spec Rev 2: `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`
    (Sec 0 rulings 1–7, Sec 2.1–2.5, Sec 5 items 2–3, gates G9/G13, risks R10/R11).
  - S2′ study: `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/`
    (shards 174 = 73 RAPGAP31 + 101 DJANGOH14; `merged/MERGE_RUN_RECORD.md`;
    `validation/gate_suite_G1_G13_report_2026-10-10.md`, G1–G13 PASS 13/13).
  - S2′ machinery (canonical, gate-validated):
    `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/` —
    `build_migration_th2_per_charge_s2prime.py` (shard builder, clusters 5469609/5469610),
    `merge_migration_per_charge_s2prime.py` (merge, clusters 5473512/5473607),
    `run_bbb_ibu_release_s2prime_stub.py` (R10-guarded release driver, cluster 5474976),
    `validate_smoke_s2prime.py`, `check_merged_s2prime.py`.
  - Installed S2′ release metadata:
    `.../bbb_ibu_detints2prime_..._2026-10-09/metadata.json` (prior basis = variant `body+miss`,
    R10-asserted; variant response sha256 `f6431c1c…79330d` (RG31) / `58172ab9…0d62e0` (DJ14);
    data vector + binmap CENTRAL, unchanged).
  - Legacy paired-leg machinery:
    `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/`
    — `build_shifted_response_data_vector_v4.py` + `shifted_electron_kinematics_v4.py` (Stage1),
    `merge_migration_per_charge_v4.py` (Stage2 merge), and the campaign driver pair
    `run_bbb_ibu_release_v4_coherentprior.py` + `crosscheck_unfold_methods_v4.py`
    (checksum-verified copies in each campaign `drivers/`).
  - Campaign state: `output_v6/systematics/paired_legs/elec_energy_down_2026-10-10/`
    (README, `run_log_2026-10-11.md`, steering, smoke) and
    `output_v6/systematics/paired_legs/elec_energy_up_2026-10-11/` (README, steering, smoke);
    normalization gate
    `sidis_highQ2/docs/status/normalization-gate-paired-leg-elecE-down-2026-10-10.md`;
    gap audit `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` §3.4/§4/§7-P2.

---

## 1. What the S2′ promotion changed in the chain

Per the spec Rev 2 Sec 0 (USER-DECIDED 2026-10-09) and the promotion record: det.int. Class 2
(interaction-born, 34.47% of the block, + non-π/K/p-mother hyperon feed-down, 56.53%) is removed
RESPONSE-INTERNALLY via an enlarged per-reco-bin fake fraction `f_i`; the charged-K DIF
k_parented keeper (102,406 pooled rows = 9.01% of the block; direct-mother PDG ±321 ONLY) stays
signal; the coherent `body+miss` prior is computed FROM THE VARIANT rapgap31 response
(R10-asserted); `k=4`, `L_data=351000.0 nb^-1` unchanged. Mechanically (verified in
`build_migration_th2_per_charge_s2prime.py`): the reroute is a **migration-fill-time,
truth-side re-routing of reco legs** — `is_detint = (IsStr==1)&(IsGtr==0)&(IsMatchedHadron==1)&(IsMatchedElectron==0)&(IsMatchedV0Daughter==0)`,
`is_kpar = is_detint & (|GenMotherPDG|==321)`, and the matched-fill gate becomes
`matched_gen_ok = gen_candidate_ok & ~(is_detint & ~is_kpar)`; non-keeper det.int. matched legs
leave the body and fill the fake column (`R[ir, GEN_OVERFLOW]`, same `w_reco = w1·w2·NucliaWeight`,
GEN-charge dispatch, so per charge and per reco bin `B_ij + F_i` is exactly conserved); the
miss-row fill for reclassified legs is SUPPRESSED (spec Sec 2.2, R1 signed off), with consumed-key
recording kept for rerouted legs so the miss row and `prior_gen_NWeff` are bit-identical to the
non-rerouted fill by construction. NO tree re-production, NO H1OO re-reconstruction — all
consumed branches (`IsStr`, `IsGtr`, `IsMatchedHadron`, `IsMatchedElectron`,
`IsMatchedV0Daughter`, `GenMotherPDG`) already exist per-reco-leg in the v6 trees.

Consequences for the fake fraction (gate report G9): response-internal RG integrated `f_i`
`10.6153% → 11.2403%` (h+) / `10.3124% → 10.8737%` (h−); data-weighted convention
`10.7141% → 11.3577%` / `10.4019% → 10.9768%`; identity `f_i_v = f_i_c + D_i/(B_c+F_c)` exact
per bin (residual 2.1e-15), denominator invariant; DJANGOH14 variant legs `11.4493%`/`11.0702%`.
The pre-S2′ scalars `10.7141%`/`10.4019%` are SUPERSEDED (they were the data-weighted integrated
summaries of the per-reco-bin `f_i`, never an applied constant — see §2).

---

## 2. Determination 1 — where the fake fraction enters the chain

**The fake fraction enters at Stage2, in the driver's response-block construction — not at the
Stage1 migration fill and not at the merge.** In the campaign driver pair,
`crosscheck_unfold_methods_v4.py::derive_response_blocks()` (lines 540–545) computes the
per-reco-bin fake fraction from the merged response matrix,
`f_i = F_i / (Σ_j B_ij + F_i)`, and applies the data-side subtraction
`d_sig = data_fine · (1 − f_i)`; the coherent prior is then
`coherent_prior = B.sum(axis=0) + M_ext[-1,:-1]` (`run_bbb_ibu_release_v4_coherentprior.py`
lines 134–137), i.e. the `body+miss` truth marginal of the SAME merged response the driver was
given. Stage1 is f-free on both legs, verified from source: the data-vector mode produces raw
reco counts (weights 1.0, `bg_subtracted=false`, 980 bins — confirmed in the campaign-1 product
`data_vector_shifted_elecE_5wheel_m0p5pct.json`), and the migration mode fills only
body/fake/miss blocks (`build_shifted_response_data_vector_v4.py` lines 920–1047: matched rows →
`R[ir,ig]`, gen-fail rows → `R[ir,GEN_OVERFLOW]`, CTG-unconsumed → miss row); the Stage2 merge
(`merge_migration_per_charge_v4.py`) is pure TH2/TH1 summation. **The per-reco-bin variant `f_i`
is applied at exactly the same stage, with the same formula and the same driver code, as the
pre-S2′ per-reco-bin `f_i`** — the campaign chain never applied a scalar f (the quoted
`10.7141%`/`10.4019%` were integrated summaries). What S2′ changes is the VALUE of `f_i`
(`f_i_v = f_i_c + D_i/(B_c+F_c)`), which is realized by the Stage1 fill change (§4), not by any
change at the application stage.

---

## 3. Determination 2 — reusability of campaign-1 Stage1 products

**(a) Shifted data vector (658 files → `data_vector_shifted_elecE_5wheel_m0p5pct.json`,
cluster 5485777): REUSABLE, unchanged, bit-identical in role.** It is a pure reco-level data
product (raw counts, no f, no response dependence); the S2′ variant does not touch the data side
(spec Sec 5 item 2: "Data vector builder: UNCHANGED"; S2′ release metadata: `data_vector:
"central, UNCHANGED"`); the det.int. removal is statistical via the response only (data-side
per-track separation impossible — spec audit facts). The S2′ chain consumes the same data vector
object; the variant `f_i` subtraction happens downstream at the driver. Campaign 1's data-vector
job is NOT redone.

**(b) Shifted MC migration histograms (8 per-period ROOTs, cluster 5485776): NOT reusable as
S2′-chain inputs; must be re-filled.** They contain f-dependence only indirectly — but they
contain the WRONG body/fake partition: they were filled with the legacy gate
(`matched_gen_ok` at `build_shifted_response_data_vector_v4.py` lines 920–923 has no
`IsStr`/`IsGtr`/`GenMotherPDG` logic), so the matched non-keeper det.int. content sits INSIDE the
body as signal, and the legacy fill records no per-category decomposition (no fc3/fc4/fc5
vectors, no rerouted marginal) from which the variant partition could be reconstructed
downstream. The S2′ reroute is a fill-time decision requiring the truth flags per reco leg;
it is NOT a response-build-time reco-side reweighting. Therefore the 8 MC migration jobs are
redone with the S2′+shift builder (same 865 MC input files, same −0.005 shift). The completed
legacy Stage1 products are RETAINED (never overwritten) and take a new role: same-shift
non-rerouted CONTROLS for the redo's diff validation (§6.5) — the exact analogue of the S2′
study's use of the central response as its diff reference.

---

## 4. Determination 3 — what exactly must change for Stage2 on the S2′ basis

1. **Stage1 MC migration fill (the actual change):** a dated campaign builder combining the
   shift machinery with the S2′ delta. The canonical S2′ builder
   (`build_migration_th2_per_charge_s2prime.py`, the machinery behind the promoted release via
   clusters 5469609/5469610) is a clone of the central NOMINAL v4 builder and has NO shift hooks
   (CLI: `--mc-root-files/--mc-label/--out-root/--n-files/--dump-rerouted-legs` only); the
   legacy shifted builder has NO reroute. Neither existing script can produce a shifted S2′
   response. The port is specified in §6.2.
2. **Stage2 merge:** `merge_migration_per_charge_v4.py` remains the driver-facing merge
   (RG+DJ legs + shifted data vector + binmap; the S2′-filled per-period outputs carry the same
   central object names `h_R_{pos,neg}(+_w2)`, `h_prior_gen_*`, `h_matched_reco_marginal_*`,
   `h_mc_count_*`, so the merge works unchanged). Because its object list is the fixed central
   set, it DROPS the S2′ category vectors — therefore each campaign ALSO runs the canonical
   `merge_migration_per_charge_s2prime.py` per generator (it merges the fc0/fc3/fc4/fc5,
   keeper-retained-body and rerouted-marginal vectors plus the S2′ meta counters, and writes
   manifest/npz) as the diagnostics/G13 pass. Both merge tools stay UNMODIFIED (shared scripts).
3. **Stage2 coherent driver:** the campaign driver pair
   (`run_bbb_ibu_release_v4_coherentprior.py` + `crosscheck_unfold_methods_v4.py`) is reused
   UNMODIFIED — it computes the coherent `body+miss` prior from the `--response-root` it is
   given (lines 134–137), so pointed at the merged S2′-filled shifted response it automatically
   computes the prior FROM THE VARIANT (SHIFTED) RESPONSE. The R10 hardcoded-prior trap applies
   to the central-family driver (`run_bbb_ibu_release_coherentprior.py` /
   `run_bbb_ibu_release_s2prime_stub.py`), which the campaigns do NOT use; the R10 discipline is
   instead enforced by a steering-level variant-id guard (§6.5) plus the `--variant` basis label.
   `k=4`, `--lumi-nb 351000.0`, iterations 1–10 unchanged.
4. **Shift-ratio nominal denominator:** the S2′ RAW release unfolded values —
   `bbb_ibu_detints2prime_..._2026-10-09/ibu/cross_sections.csv` (rapgap leg, IBU k=4, 490 rows;
   integrated h+ `11.446174 nb` / h− `11.158554 nb`). Per-bin relative shift =
   shifted-variant-unfolded / S2′-nominal-unfolded − 1 (yield or nb columns are ratio-identical;
   same `L=351000.0` both sides). The pre-S2′ coherent nominal
   (`bbb_ibu_coherentprior_..._2026-10-06/`) is SUPERSEDED as comparison partner; quoting a
   campaign shift against it without the observable-basis flag is FORBIDDEN (STATE.md quoting
   rule, extended: prior basis + Born-vs-raw level + observable basis on every quote).
5. **Known carried attribute (documented, not a defect):** the campaign driver computes the
   coherent `body+miss` prior per leg, including the DJANGOH leg, whereas the central/S2′
   release drivers use the STORED plain prior on the DJ leg (spec G6 model-leg caveat; S2′
   metadata `prior_djangoh: "variant stored h_prior_gen"`). This affects only the campaign
   release's `model_unc` (|DJ−RG|) diagnostic column, not the rapgap-leg shift vector, and was
   equally true of the accepted 2026-10-07 `detector_shifted_coherentprior` re-unfolds. Any
   generator-dependence attribution must use the exported fc3 category vectors per the G6
   caveat, not the release-level |DJ−RG|.

---

## 5. Determination 4 — does the shifted MC leg need re-reco at Stage1?

**No re-reco, no tree re-production, no new inputs — but YES, the Stage1 MC migration fill must
be redone.** The detint-fake change is a truth-side classification/rerouting inside the
migration fill (keeper logic + matched-fill gate + fake-column category fill + miss-fill
suppression), reading branches already present in the same v6 MC trees; the shifted-reco
computation itself is untouched. It is NOT a downstream reco-side reweighting: the legacy
campaign-1 migration histograms cannot be re-partitioned into the variant body/fake split
because the non-keeper det.int. matched content is not separately recorded in them. Hence
campaign 1's Stage1 MC leg (8 jobs) is re-run with the S2′+shift builder over the identical
input lists (`rapgap31_files_364.txt`, `djangoh14_files_501.txt`) at the identical −0.005
shift; its Stage1 data-vector leg is kept. The reroute composes cleanly with the shift: reco-bin
indices come from the shifted kinematics, the gen side stays nominal, the truth flags are
shift-invariant per reco leg, and the `B+F` conservation and miss-row identities hold per
(shifted) reco bin exactly as in the nominal variant.

---

## 6. Revised campaign protocol

### 6.1 Frozen UNCHANGED (do not reopen)

- Shift prescriptions and amplitudes: `eElecEn` ±0.5% per each of five LAr wheels co-shifted
  (BBE, CB1+2, CB3, FB1, FB2+IF); `hfs_jes` ±1% in-jet (kT R=1.0, pT>5 GeV,
  −1.5<η_lab<2.75); `hfs_rces` ±1% out-of-jet (independent); `hfs_theta` ±1 mrad;
  `elec_theta` ±1 mrad ×3 regions + `eElecTrTh` ±1 mrad co-shift; `hfs_phi` ±20 mrad.
- Campaign order (D-2026-10-10-3): eElecEn → hfs_jes + hfs_rces → hfs_theta → elec_theta →
  hfs_phi; 6 sources × 2 sides = 12 campaigns; replace, never rescale, legacy one-sided vectors.
- Prerequisites: canonical 658-file data list (§3.4 fix), `L=351000.0 nb^-1`, relative-only
  shift extraction, coherent `body+miss` prior from the campaign's own (now variant) response,
  IBU `k=4`, binmap v3, DJANGOH14 leg in every merged response (decision (a), 2026-10-10),
  closure/refold + noise-floor/same-side tests, symmetrisation by coherent half-difference only
  after the noise/coherence test.
- Stage2 job shape: merge + coherent unfold in one campaign job; smoke before batch; login node
  for single-file checks only.

### 6.2 New machinery (one-time): the dated S2′+shift Stage1 builder

Create ONE dated campaign builder (shared by all 12 campaigns), e.g.
`H1ChargedSidis_HighQ2/scripts/unfolding/paired_legs_detints2prime_2026-10-11/build_shifted_response_data_vector_s2prime_v4.py`
— a dated copy of `build_shifted_response_data_vector_v4.py` (migration mode) with the S2′ delta
ported verbatim from `build_migration_th2_per_charge_s2prime.py` (extend-canonical-scripts
rule; no shared script is edited). Port checklist:

1. Branch requirements: add `IsStr`, `IsGtr`, `GenMotherPDG` (S2P_REQUIRED_BRANCHES) and
   `IsMatchedElectron` to the MC migration-mode read set (fail-fast if absent).
2. Classifier/keeper/reroute verbatim: `is_detint`, `is_kpar` (direct-mother |PDG|==321 ONLY,
   never `VetoAncestorPDG`), `s2p_reroute = is_detint & ~is_kpar`.
3. Matched gate: `matched_gen_ok &= ~s2p_reroute` (keeper legs keep the central matched fill).
4. Fake-column fill: fc categories fc3 (rerouted matched non-keeper), fc4 (pre-existing fake
   non-keeper), fc5 (pre-existing fake keeper), fc0 (unflagged); fc3 legs fill
   `R[ir_shifted, GEN_OVERFLOW]` with the same `w_reco`, GEN-charge dispatch; per-reco-bin
   category TH1s, `h_rerouted_reco_marginal_*`, `h_keeper_retained_body_*`, S2′ counters
   (`n_rerouted_*`, `n_keeper_retained_*`, `n_detint_*`, `n_rerouted_key_collisions`).
5. Miss-fill SUPPRESSION for reclassified legs, with consumed-key recording kept for rerouted
   legs (miss row + `prior_gen_NWeff` bit-identical to the same-shift legacy fill by
   construction); keep the rerouted-key collision counter as a defensive audit (campaign-scale
   nonzero expected — the nominal campaign measured 4375 RG / 5772 DJ, adjudicated BENIGN).
6. Metas: stamp BOTH `meta_variant_id = detint_fake_response_variant_S2prime_2026-10-09` AND
   the shift stamps (`meta_electron_energy_scale` etc.), so every Stage1 product carries
   observable basis + shift.
7. DO-NOT-PORT list (spec Sec 2.1): e/V0 truth-veto disablement; alpha cone; the prototype's
   miss-row fill for reclassified rows. Everything else byte-faithful to the legacy shifted
   builder (shift machinery, gen side, weights, Encoding-B shapes (491,246), charge-major
   conventions).
8. Data-vector mode: inherited unchanged and NOT used for the data leg (the legacy builder
   remains canonical for data vectors; the S2′ delta is inert on data by construction).

For the HFS/elec-theta/elec-phi campaigns (3–12) the same checklist applies to their
(per-campaign) shifted-builder ports; the original plan already included building the paired
HFS/theta/phi machinery (legacy one-sided tags are excluded), so the S2′ delta rides that work.

**One-time port validation (login node, single file, ≤5 min each):** run the port on the two
campaign-1 smoke units — RAPGAP31 `7097/..._ep0304_0000.root` and DJANGOH14
`7093/..._ep0304_0000.root` at −0.005 — against the EXISTING same-file, same-shift legacy smoke
products (`smoke_mc_shifted_elecE_5wheel_m0p5pct_{rapgap31,djangoh14}.root` + zero controls in
the campaign-1 smoke dir), which are exactly the right non-rerouted references. Checks
(superset of the S2′ study's `validate_smoke_s2prime.py` contract, adapted to the shifted
control): (a) shapes (491,246), no NaN/negatives; (b) shift evidence identical to legacy
(E′ ratio 0.995 uniform across θ regions, stamps); (c) gen side bit-identical
(`n_gen_total/in_ps/in_bin`); (d) miss row, `h_prior_gen_*`, `prior_gen_NWeff`, `h_mc_count_*`
bit-identical to the same-shift legacy control; (e) body-cell diff == exactly the rerouted legs'
weights (use `--dump-rerouted-legs`), fake-column diff == rerouted weight per reco bin,
per-(charge, reco bin) `B+F` conservation, fc0+fc3+fc4+fc5 partition closure,
`matched_reco_marginal` diff == rerouted marginal; (f) G9 identity vs the same-shift legacy
control: `f_i(S2′) − f_i(legacy) == D_i/(B+F)` per bin, f_i may only increase; (g) keeper
population > 0 with mother PDGs ±321 only; (h) `meta_variant_id` + shift stamps present.
Then the standard per-campaign smoke (below), then Condor.

### 6.3 Stage2 recipe on the S2′ basis (per campaign)

1. Pre-merge variant-id guard (campaign-local steering): assert
   `meta_variant_id = detint_fake_response_variant_S2prime_2026-10-09` and the correct shift
   stamp in all 8 per-period Stage1 ROOTs/QA JSONs before merging (R10 discipline at steering
   level; the v4 merge does not propagate the marker).
2. Driver-facing merge: `merge_migration_per_charge_v4.py --rapgap-files <4 S2′ RG per-period
   roots> --djangoh-files <4 S2′ DJ roots> --data-json <shifted data vector> --out-root
   <campaign migration_inputs root> --out-binmap-json <binmap v3>` (unchanged tool).
3. Diagnostics merge: `merge_migration_per_charge_s2prime.py` per generator over the same 8
   roots (category vectors + S2′ counters + manifest; unchanged tool).
4. Coherent unfold: `run_bbb_ibu_release_v4_coherentprior.py --response-root <merged S2′
   shifted root> --binmap-json ... --data-json <shifted data vector> --release-dir
   stage2/release_coherentprior_s2prime --electron-shift <tag> --variant
   paired_legs_<campaign>_s2prime_<date> --lumi-nb 351000.0 --ibu-iters 1,...,10
   --ibu-selected 4` (unchanged driver pair, checksum-verified copies; prior = this shifted
   VARIANT response's own body+miss, recorded automatically in the release metadata).
5. Comparison: per-bin relative shift vs the S2′ RAW release (rapgap-leg IBU k=4
   `yield_unfolded`), plus closure/refold, noise-floor and same-side checks (mechanics
   unchanged; response = variant). Campaign release outputs carry the three-part basis label
   (prior basis `body+miss` from the shifted variant response; raw/no-bgsub, not Born-level;
   observable basis S2′ detint-fake variant) via the `--variant` string + campaign README +
   comparison JSONs — the frozen driver CAVEAT string is not edited.

### 6.4 Per-campaign scope

| # | Campaign (tag) | Pre-S2′ state | Revised scope |
|---|---|---|---|
| 1 | eElecEn DOWN (`elecE_5wheel_m0p5pct`) | Stage1 COMPLETE (data vector 5485777; 8 MC migrations 5485776, drained+verified); Stage2 NOT run (stop order) | Data vector REUSED (reference + sha256 recorded). MC migrations REDONE with the S2′+shift builder (8 jobs, same lists, same −0.005); legacy Stage1 products retained as same-shift controls. Stage2 per §6.3 in a NEW dated sibling workspace `paired_legs/elec_energy_down_s2prime_<date>/` (legacy workspace untouched). Full diff validation vs the legacy controls (§6.5). |
| 2 | eElecEn UP (`elecE_5wheel_p0p5pct`) | Steering + smoke complete; NOTHING submitted | Re-point campaign-local steering to the S2′+shift builder IN the existing workspace (unexecuted; README revision note); regenerate the MC-leg smoke verdicts with the new builder (data-leg smoke verdict stands — machinery unchanged); launch data vector (1) + MC migrations (8) + Stage2 (1). |
| 3–4 | hfs_jes ±1% in-jet | Not set up | Set up directly on the S2′ basis: build the paired ± HFS-JES shifted machinery (original-plan work) WITH the S2′ delta (§6.2 checklist); jet-classifier audit first; then 1+8+1 jobs per side. |
| 5–6 | hfs_rces ±1% out-of-jet | Not set up | Same as 3–4 with the relvol10 enum/classifier verification first. |
| 7–8 | hfs_theta ±1 mrad | Not set up | Resolve the quarantined even/quadratic two-sided response first (original-plan prerequisite), then same shape. |
| 9–10 | elec_theta ±1 mrad ×3 regions + `eElecTrTh` co-shift | Not set up | Setter-level co-shift audit first (original-plan prerequisite), then same shape. |
| 11–12 | hfs_phi ±20 mrad | Not set up (new source) | Same shape; becomes the new budget component on completion (D-2026-10-10-3). |

No campaign 3–12 may produce pre-S2′ Stage1 products: the S2′ basis is the starting convention
for every campaign not yet launched.

### 6.5 Gates

1. **Smoke gate (per campaign, before batch):** single-file, login node, ≤5 min. Data leg:
   unchanged machinery — campaign 1's verdict stands; campaigns 2–12 run the standard data smoke.
   MC legs (both generators): THREE single-file runs with the S2′+shift builder and controls —
   S2′-shifted, LEGACY-shifted (same file, same shift; the reroute-diff control), and zero
   control — checked against the §6.2 one-time-validation contract (a)–(h). A new
   `SMOKE_VERDICT.json` per campaign must record the S2′ checks explicitly; the legacy-builder
   smoke verdicts do NOT validate the S2′ port.
2. **Normalization gate (refreshed per campaign):** a new dated record
   (`normalization-gate-paired-leg-<campaign>-s2prime-<date>.md`) that (i) re-affirms
   `L=351000.0 nb^-1`, relative-only shift semantics, canonical 658/364/501 lists (all
   unchanged, and the L factor still cancels exactly in the ratio); (ii) RE-POINTS the
   comparison partner to the S2′ RAW release (condition 1 of the 2026-10-10 gate is superseded
   in this one respect); (iii) REPLACES the fake-fraction statement: the chain's fake
   subtraction is the per-reco-bin variant `f_i` computed from each campaign's own merged
   shifted variant response; nominal-variant reference integrated values RG response-internal
   `11.2403%`/`10.8737%` (data-weighted `11.3577%`/`10.9768%`); the pre-S2′ scalars
   `10.7141%`/`10.4019%` are SUPERSEDED and must not appear as current anywhere in the campaign
   path; (iv) restates the three-part quoting rule and that external det.int. vectors remain
   FORBIDDEN (double removal + keeper violation).
3. **Variant-id steering guard:** §6.3 step 1 (merge refuses non-S2′ inputs).
4. **Per-campaign S2′ validation battery (batch-level):** per-period QA (input counts, 0 failed,
   shift stamp, `meta_variant_id`, S2′ counters > 0); from the diagnostics merge — fc partition
   closure, G13-type integrated removable fraction vs the parentage reference (≈0.9064 RG /
   ≈0.9126 DJ; nominal-variant realized 0.90528/0.91308), keeper counters, response-internal
   integrated `f_i` sanity vs the nominal-variant values (loose band — the shift moves reco
   content; the HARD check is the G9 identity, exact for campaign 1 against its legacy controls,
   and single-file-exact for every campaign at smoke time); `B+F` per-reco-bin conservation;
   negative-input/acceptance audit (0/980 negative reco inputs, 0/490 negative cross sections,
   `eff ∈ (0,1]` in populated bins).
5. **Quoting rule:** every campaign number is quoted with prior basis (coherent `body+miss`
   from the campaign's shifted variant response), level (raw/no-bgsub, not Born-level,
   provisional), and observable basis (S2′ detint-fake variant). Never ratio an S2′-basis
   campaign value against a pre-S2′ value without the observable-basis flag.

### 6.6 Job scope estimate vs the original 12-campaign plan

- Original implemented shape (established by campaign 1): 10 batch jobs per campaign
  (1 data-vector + 8 per-period MC migrations + 1 Stage2) ⇒ **120 jobs** for 12 campaigns.
  (The gap-audit §4 file-granularity figure — ≈1,022 re-reco jobs/side RAPGAP-only, +501 DJ,
  ≈12,300 for the program — was the pre-implementation estimate; the per-period batching
  already superseded it and is unaffected by this replan.)
- Revised: **net UNCHANGED, −1 job.** Campaign 1: 8 MC redo + 1 Stage2 = 9 jobs (data vector
  reused). Campaigns 2–12: 10 jobs each = 110. Total mandatory ≈ **119 batch jobs**, plus one
  optional lightweight diagnostics-merge job per campaign (§6.3 step 3; seconds of wall, may be
  folded into the Stage2 job) ⇒ ≈131 with diagnostics. Wall-time per MC job unchanged
  (precedent 493–2508 s vs the 10800 s cap; the S2′ delta adds four branch reads + boolean
  masks per event). The only superseded production in the program is campaign 1's 8 legacy MC
  jobs (≈42 min batch drain on 2026-10-11); they are retained as controls, not deleted.
- One-time added work (no batch jobs): the §6.2 builder port + one-time port smoke
  (login-node); per-campaign refreshed smoke/normalization gate records; the §6.5 validation
  battery (analysis-side).

### 6.7 Downstream consumers and non-goals

- On completion of each ± pair, the campaign's full-chain detector components replace the
  DATA-SIDE-ONLY components in the 15-component S2′ budget
  (`systematics_detints2prime_15comp_..._2026-10-10/`; `elec_energy_rel`, `elec_theta_rel`,
  `hfs_jes_rel`, `hfs_rces_rel`, `hfs_theta_rel`), closing STATE.md open follow-up #1 for the
  detector tier and shrinking the ~+2.5 pp data-side-only convention gap
  (elec_energy-dominant ~+3.2 pp) toward the central full-chain convention. `hfs_phi_rel` is
  added as a new component when campaigns 11–12 complete. Budget reassembly is a SEPARATE step
  with its own record.
- NOT in scope here: the tracking ensemble (separate post-unfolding construction, no shifted
  response), the gen-side hyperon feed-down exclusion (parked, spec Sec 0 ruling 6), the
  body/matched response MC-stat covariance gap (G12(b) scope, `fakecol_mcstat_rel` covers only
  σ(f_i)), Born-level derivatives of campaign outputs (C_rad application is a separate,
  already-defined step; per-bin Born shift == per-bin raw shift), and any edit to the S2′
  release, the central chain, or shared scripts.

### 6.8 Prohibitions and carried caveats

- External det.int. subtraction: FORBIDDEN on the S2′ chain (double removal + keeper
  violation; spec Sec 3, "stays forbidden, more strongly").
- Never rebuild priors the plain `w_gen`-only way; never reuse a central/nominal marginal for a
  campaign release (the campaign driver is structurally immune; keep it that way).
- Do not port the prototype's e/V0 veto disablement, alpha cone, or reclassified-row miss fill.
- Keeper condition is direct-mother PDG ±321 ONLY (R11); a deep-ancestor or resonance-PDG match
  is a defect.
- Campaign releases remain PROVISIONAL diagnostics (raw/no-bgsub, no C_rad, not Born-level,
  multiplicities blocked); the `cRad_bgfracsub`-style derived-variant separation precedent
  applies to any plotting derivative.
- The `n_rerouted_key_collisions` counter is expected nonzero at campaign scale (nominal
  campaign: 4375 RG / 5772 DJ ≈ 1.1% of rerouted legs; adjudicated BENIGN, present in the
  central builder too) — do not gate on zero; the single-file smoke may legitimately see 0.

---

## 7. Provenance register (primary)

- S2′ promotion: `sidis_highQ2/STATE.md` end-of-file override (2026-10-10 S2′ section, items
  1–6, remaining-work ledger item 1); `sidis_highQ2/docs/RUN_STATUS.md` remaining-work item 1.
- S2′ spec Rev 2: `sidis_highQ2/docs/backgrounds/detint_fake_response_variant_spec_2026-10-08.md`.
- S2′ study + gates: `H1ChargedSidis_HighQ2/output_v6/studies/detint_variant_s2prime_2026-10-09/`
  (`merged/MERGE_RUN_RECORD.md`; `validation/gate_suite_G1_G13_report_2026-10-10.{md,json}`).
- S2′ machinery: `H1ChargedSidis_HighQ2/scripts/unfolding/detint_variant_s2prime_2026-10-09/`
  (`build_migration_th2_per_charge_s2prime.py`, `merge_migration_per_charge_s2prime.py`,
  `run_bbb_ibu_release_s2prime_stub.py`, `validate_smoke_s2prime.py`, `check_merged_s2prime.py`).
- Installed S2′ release:
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (`metadata.json`, `ibu/cross_sections.csv`).
- Legacy shifted machinery: `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/`
  (`build_shifted_response_data_vector_v4.py`, `shifted_electron_kinematics_v4.py`,
  `merge_migration_per_charge_v4.py`).
- Campaign records: `output_v6/systematics/paired_legs/elec_energy_down_2026-10-10/`
  (README, `run_log_2026-10-11.md`, steering, `smoke/SMOKE_VERDICT.json`,
  `stage1/data_vector/data_vector_shifted_elecE_5wheel_m0p5pct.json`);
  `output_v6/systematics/paired_legs/elec_energy_up_2026-10-11/` (README, steering, smoke);
  `sidis_highQ2/docs/status/normalization-gate-paired-leg-elecE-down-2026-10-10.md`.
- Program authorities: `docs/systematics/FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md`
  (rows 4–9; decision log D-2026-10-10-3); `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md`
  (§3.4, §4, §7-P2); 15-component budget
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/systematics_detints2prime_15comp_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-10/`.
