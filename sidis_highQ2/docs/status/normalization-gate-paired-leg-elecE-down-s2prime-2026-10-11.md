# Normalization gate — P2 paired-leg campaign 1 REDO (S2′ basis), eElecEn DOWN (2026-10-11)

- **Gate ID:** normalization-gate-paired-leg-elecE-down-s2prime-2026-10-11
- **Date:** 2026-10-11 (gatekeeper: h1-normalization)
- **Scope:** refreshed normalization gate for the campaign-1 redo on the S2′
  detint-fake basis — eElecEn DOWN, five LAr wheels co-shifted −0.5%,
  tag `elecE_5wheel_m0p5pct`, workspace
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_down_s2prime_2026-10-11/`.
  Pre-launch gate: nothing submitted at gate time (Stage1 condor is
  configured but NOT executed; Stage2 condor BLOCKED ON Stage1 completion
  and supervisor approval).
- **SUPERSEDES:** `normalization-gate-paired-leg-elecE-down-2026-10-10.md`
  (pre-S2′ basis) IN FULL for the eElecEn DOWN campaign-1 normalization
  gate. The 2026-10-10 record remains accurate as the gate of the pre-S2′
  campaign workspace, which is retained untouched as the same-shift
  non-rerouted diff-control provenance (protocol Sec 3(b)/6.5); it is no
  longer the gate of record for this campaign. Its condition 1
  (comparison partner = coherent-prior release) is REPEALED; the comparison
  partner is now the S2′ RAW release (see item 2 below). Its conditions on
  relative-only extraction and quoting discipline carry forward as
  re-stated here.
- **Governing context:** user handoff of 2026-10-11 (task statement);
  `sidis_highQ2/docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md`
  (Sec 4, Sec 6.2–6.5; this gate = protocol Sec 6.5 gate 2 refreshed per
  Sec 6.5.2); `sidis_highQ2/docs/RUN_STATUS.md` 2026-10-11 S2′
  detint-fake promotion override (canonical RAW release, quoting rule,
  supersession ledger); `sidis_highQ2/STATE.md` end-of-file override.

## VERDICT: PASS — no blockers

Batch launch may proceed on supervisor approval, subject to the
non-blocking conditions C1–C3 below. (Condition C4 — pre-submission
workspace completion — was RESOLVED during the gate itself; see the C4
entry.)

## Campaign composition (verified facts)

| Leg | Composition | Files | L value (nb⁻¹) | Status |
|-----|-------------|-------|-----------------|--------|
| Data (reused) | canonical 658-file list, ep0304/em0405/em06/ep0607 = 83/152/100/323 | 658 | L_data = 351000.0 | REUSED bit-identical from pre-S2′ campaign (cluster 5485777); no data-vector job in the redo |
| RAPGAP31 MC | catalogs 7097/7096/7071/7094 (ep0304/em0405/em06/ep0607) | 364 (61/121/61/121) | L_MC_total = 9,132,891.0 | re-filled with S2′+shift port builder (4 jobs) |
| DJANGOH14 MC | catalogs 7093/7092/7072/7091 (ep0304/em0405/em06/ep0607) | 501 (71/141/88/201) | L_MC_total = 10,487,495.0 | re-filled with S2′+shift port builder (4 jobs) |
| Nominal denominator | S2′ RAW release `bbb_ibu_detints2prime_..._2026-10-09/` (IBU k=4, 490 rows) | — | lumi_nb = 351000.0; integrated IBU h+ 11.446174 / h− 11.158554 | shift ratios vs THIS release only |

Stage1 = 8 MC re-fill jobs (no data-vector job); Stage2 = 1 job. The
pre-S2′ Stage1 legacy products (cluster 5485776, `mig_shifted_v4_*`) are
retained untouched in the 2026-10-10 workspace as same-shift non-rerouted
diff controls and are consumed by nothing in the redo.

**No L_MC value is consumed anywhere in the campaign path.** The L_MC
totals above are composition/provenance facts from the authoritative mclumi
record (`H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`:
`grand_total_per_generator_nbinv/rapgap31 = 9132891.000000002`,
`/djangoh14 = 10487494.999999996`); a grep sweep of the redo workspace
finds no MC-luminosity constant in any steering script, driver, or
comparison code.

## Item-by-item verification

### 1. Sample identities, file counts, L values — VERIFIED

- Filelists are byte-identical to the lists verified file-for-file in the
  2026-10-10 gate (sha256: `data_files_658.txt` =
  `ff237cc27117fe6a24785f9bdb8badf96871f8d45732cde5250e7e222c9f8926`,
  `rapgap31_files_364.txt` =
  `5810355de77b527112093fbcf7e72e5f7a6b634754c9ebaafd3ea01c04a126b6`,
  `djangoh14_files_501.txt` =
  `8db1266fac7e12b8dd6ea86674be9bbe6096a91e40b4d95aba66077073ba2bef`).
- Data list: 658 file lines; period headers ep0304 (83) / em0405 (152) /
  em06 (100) / ep0607 (323). The reused data-vector QA independently
  confirms 658 opened / 0 failed.
- MC per-period counts re-verified by direct listing of the canonical
  sidistree inputs (`output_v6/central/sidistree/mc/<mc>/<NUMPER>/`):
  rapgap31 7097/7096/7071/7094 = 61/121/61/121 (=364); djangoh14
  7093/7092/7072/7091 = 71/141/88/201 (=501).
- Wrapper NUMPER mapping (`wrap_paired_elecE_migration_s2prime_v4.sh`) is
  identical to the pre-S2′ campaign, so the retained legacy Stage1
  products align job-for-job as diff controls.
- L_data = 351000.0 nb⁻¹ (current four-period regime value; RUN_STATUS
  promotion override). REJECTED on sight in this regime: 297,657.8
  (excludes ep0304; also the stale mclumi JSON header field — never
  quote), 356,613.0 / 356,000 (superseded direct-monitor quotes),
  126,880 (wrong by ~42×).

### 2. Relative-only semantics on the S2′ chain — VERIFIED

- **Stage2 driver:** campaign-local copy
  `drivers/run_bbb_ibu_release_v4_coherentprior.py` is md5-identical
  (`bccc9aed3278f1709cea7009c2a29509`) to the driver accepted in the
  2026-10-10 gate, whose semantics were verified in code: the ONLY
  absolute conversion is yields→nb via L_data (`LUMI_DEFAULT=351000.0`;
  wrapper passes `--lumi-nb 351000.0`), `--ibu-selected 4`, iters 1–10;
  per-reco-bin fake fraction formed by pure same-sample response ratios
  `f_i = F_i/(Σ_j B_ij + F_i)`; no MC luminosity can enter. The crosscheck
  driver copy is md5 `6f735fb6088237db77ff7514bd158f92` (identical pair).
- **Merge:** driver-facing merge is the UNCHANGED shared tool
  `merge_migration_per_charge_v4.py` (pure TH2/TH1 summation, verified
  2026-10-10); diagnostics merge uses the unchanged S2′ tool per
  generator (category vectors only, not driver-facing).
- **Per-bin shift:** formed ONLY by
  `steering/compare_vs_s2prime_nominal.py` as
  `rel_shift = yield_unfolded_campaign / yield_unfolded_S2prime_nominal − 1`
  — a pure yield ratio; the common L_data = 351000.0 nb⁻¹ (same value,
  same construction on both sides) cancels exactly; yield and nb columns
  are ratio-identical. No luminosity constant appears in the script.
- **Denominator protection (structural):** the comparison script
  fail-fast asserts, before dividing: 490 rows; charges pos/neg;
  integrated cross_section_nb h+ 11.446174 / h− 11.158554 (tolerance
  1e-6); and `response_variant_id == detint_fake_response_variant_S2prime_2026-10-09`
  on EVERY nominal row. The superseded pre-S2′ coherent-prior release
  (`bbb_ibu_coherentprior_..._2026-10-06/`) cannot be substituted as
  denominator without tripping these assertions.
- **Driver-facing chain order (Stage2 wrapper
  `run_paired_elecE_stage2_s2prime.sh`):** data-vector sha256 guard →
  R10 pre-merge guard → v4 merge → S2′ diagnostics merge → coherent
  unfold (`--lumi-nb 351000.0`, k=4, `--variant
  paired_legs_elec_energy_down_s2prime_2026-10-11`) → comparison.
  `DRY_RUN=1` re-executed clean at gate time (exit 0): config OK,
  reused data vector present with sha256 match, tools present,
  denominator = S2′ RAW release.

### 3. Reused data vector — VERIFIED bit-identical, raw, f-independent

- Source: pre-S2′ campaign-1 product, Condor cluster 5485777
  (`job.5485777.0.log` present in the pre-S2′ workspace logs).
- Redo copy is bit-identical to the source:
  `data_vector_shifted_elecE_5wheel_m0p5pct.json` sha256
  `ffeedf09596b129eb526b452844d0082e365e378ffde1042875a98ca7a2ab6c3`
  (md5 `d3b1f06ccbda6eb7d653c4c21f75d40c`), QA sha256
  `01d6216ae4264c3b92334de47c39929ef5d05264e2052f786623d148cc4789e9` —
  identical in both workspaces.
- Product facts: 658 files opened / 0 failed; shift stamp
  `electron_energy_scale = −0.005` (carrier name "zero",
  photon_mode shifted_electron, level NATIVE_FINE_RECO_SHIFTED);
  `bg_subtracted: false`; label `raw_data_no_bgsub_...`; raw track
  counts (seen 7,538,865 / in-PS 3,896,443 / in-grid 3,879,602). The
  data vector is raw counts and carries NO f dependence — the per-reco-bin
  variant f_i is formed downstream at response build from the MC response
  only (consistent with protocol Sec 3(a): the S2′ delta is inert on the
  data side, hence reuse is valid).
- The Stage2 wrapper hardcodes the expected sha256 (equal to the value
  above) and refuses a drifted copy (exit 4) before the merge.

### 4. Superseded-value sweep — CLEAN

- Grep sweep of the ENTIRE redo workspace for the superseded fake-fraction
  scalars `10.7141` / `10.4019`, the superseded release path
  (`coherentprior_..._2026-10-06`), and forbidden luminosity constants
  (`297657.8`, `356613`, `356000`, `126880`): NO applied or reference
  occurrence. (Single false positive: the substring "356000" inside the
  raw data-vector bin content `76.22335600063802` — a count, not a
  luminosity.)
- No legacy Stage1 product name (`mig_shifted_v4_*`, `merged_7091`) is
  consumed by any redo steering file.
- The applied fake-fraction basis is the per-reco-bin variant f_i computed
  from THIS campaign's own merged shifted S2′ response — not a scalar.
  The S2′ reference anchors (response-internal RG integrated
  h+ 11.2403% / h− 10.8737%; data-weighted 11.3577% / 10.9768%;
  DJANGOH14 variant legs 11.4493% / 11.0702%) are recorded in
  RUN_STATUS.md (promotion override item 4), the protocol (Sec 1, gate
  G9), and
  `studies/detint_variant_s2prime_2026-10-09/merged/MERGE_RUN_RECORD.md`
  — corroborated at all three; they SUPERSEDE the pre-S2′ scalars and do
  not appear as applied constants in the campaign path.

### 5. R10 meta_variant_id guard at steering level — PRESENT

- `steering/guard_variant_id_s2prime.py`: asserts, for ALL 8 per-period
  Stage1 products (4 rapgap31 + 4 djangoh14), in BOTH the ROOT file
  (`meta_variant_id`, `meta_electron_energy_scale` as TNamed) and the QA
  JSON (top-level `variant_id`, `migration_summary.variant_id`
  cross-check, `shift.electron_energy_scale`):
  `meta_variant_id == detint_fake_response_variant_S2prime_2026-10-09`
  and shift stamp `−0.005`; also checks the v4-merge central object set
  (h_R_*, w2 companions, priors, marginals, mc_count, binmap). Exit 2 on
  any failure. Invoked as Stage2 step 1, BEFORE the merge (the v4 merge
  does not propagate the marker) — exactly protocol Sec 6.3 step 1.
- The port builder stamps both metas in every product
  (`S2P_VARIANT_ID` at builder line 487; QA `variant_id` fields; ROOT
  meta write), so the guard has real material to assert on.
- One-time port validation (protocol Sec 6.2) is complete and PASS:
  `builder_port_smoke_2026-10-11/SMOKE_VERDICT.json` — `overall_pass:
  true`; per generator 82/82 items ok (rapgap31 and djangoh14, contract
  items (a)–(i) incl. (h) meta_variant_id + shift stamps) at this exact
  campaign shift (−0.005); G9 f_i identity exact per bin; the driver pair
  (v4 merge + coherent IBU driver) reused UNMODIFIED.

## Absolute-scale statement (carried forward, updated basis)

This campaign produces NO absolute-nb MC-scaled product. Its only
absolute-nb products are the campaign release and the S2′ RAW nominal,
both constructed with L_data = 351000.0 nb⁻¹ only (correct), and the
systematic is extracted as their per-bin ratio, where L cancels exactly.
If any absolute-nb MC-scaled product is ever introduced on this campaign,
it MUST use `351000.0 / L_MC` computed from the mclumi record
(RAPGAP31 0.03843251824641288; DJANGOH14 0.03346843073584302;
7091-alone 0.08013817556653238 by arithmetic) — with the O1
documentation defect from the 2026-10-10 gate still open (the
351000-based scale STRINGS in `normalization.md` are high by
+7.6…+10.7 ppm vs arithmetic; L_MC values themselves are consistent and
authoritative; no scale constant is consumed in this campaign path).

## 7091-alone cross-check — CONSISTENT

The DJANGOH14 leg is the merged 4-catalog treatment
(L_MC_total = 10,487,495.0 nb⁻¹; 7093/7092/7072/7091 = 71/141/88/201
shards). The 7091-alone trap (L_MC = 4,379,935.0 nb⁻¹, mclumi
`per_catalog/djangoh14_7091_ep0607/L_MC_nbinv = 4379934.999999998`,
documented scale 0.08013890218154498) does NOT apply here: no MC
luminosity is consumed anywhere in the redo, and the merged-leg
composition matches the nominal S2′ response basis (same four catalogs).
Any future per-catalog absolute scaling must take per-catalog L values
from the mclumi record with sample-identity verification first.

## Conditions (non-blocking, travel with the campaign)

1. **C1 — relative-only extraction and quoting discipline.** Extract the
   systematic strictly as the per-bin relative shift vs the S2′ RAW
   release (`compare_vs_s2prime_nominal.py` output); never rescale either
   side; every quoted value carries the three-part basis label (prior
   basis: coherent body+miss from the campaign's shifted S2′ variant
   response; level: raw/no-bgsub, not Born-level, provisional; observable
   basis: S2′ detint-fake variant). Quoting an S2′ value against a
   pre-S2′ value without the observable-basis flag is FORBIDDEN.
2. **C2 — future absolute products.** Any future absolute-nb MC-scaled
   product must use `351000.0/L_MC` from the mclumi record (values above)
   after resolving O1; the mclumi JSON's stale header field
   (`L_data_HERA-II_S67_nb: 297657.8`) must never be quoted.
3. **C3 — regime luminosity discipline.** L_data = 351000.0 nb⁻¹ is the
   only current data luminosity for this regime; 297,657.8 / 356,613.0 /
   356,000 / 126,880 are rejected on sight.
4. **C4 — workspace completion: RESOLVED during the gate (2026-10-11).**
   The artifacts pending at first inspection were produced and verified
   before the gate closed:
   - `smoke/SMOKE_VERDICT.json` (assembled by
     `smoke/assemble_smoke_verdict_s2prime.py`): `overall_pass: true`,
     ALL 22 checks true — data-vector copy/QA bit-identity, 658/0-failed,
     −0.005 stamp, raw/no-bgsub, port smoke overall + shift match +
     variant id + 82/82 and contract (a)–(h) per generator, all three
     filelists, denominator 490-row/integrated/variant-id identity, both
     driver copies.
   - `stage1/data_vector/DATA_VECTOR_REUSE.json`: records the cluster-5485777
     source, `sha256_source == sha256_copy` (`ffeedf09…a2ab6c3`) and
     QA (`01d6216a…cc4789e9`), 658/0-failed, n_tracks_seen 7,538,865,
     −0.005 stamp, `bg_subtracted: false`, raw weights, 980 bins, and the
     f-independence basis (protocol Sec 3(a)/Sec 4; S2′ release metadata
     `data_vector: central, UNCHANGED`; f_i applied downstream at the
     Stage2 driver) — every field agrees with this gate's independent
     verification.
   - `README.md` (66 lines): consistent with the gate on all points
     (reuse + sha256, k=4 / L=351000.0, S2′ RAW denominator with anchors,
     port builder stamps, list splits, L_MC totals cited from
     `normalization.md:55-62`, R10 guard, quoting rule).
   - `drivers/DRIVER_COPIES.md5`: present; records
     `bccc9aed3278f1709cea7009c2a29509` /
     `6f735fb6088237db77ff7514bd158f92` — identical to this gate's
     independent md5 verification.
   Trivial follow-up (non-blocking): README item 58 still describes this
   gate record as "pending / not yet written"; that line is satisfied by
   the present record and may be updated by the workspace owner at
   convenience.

## Provenance register (key artifacts)

- Campaign workspace:
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_down_s2prime_2026-10-11/`
  (steering: wrap/guard/compare/stage2 + 2 condor files; drivers
  md5 `bccc9aed3278f1709cea7009c2a29509` / `6f735fb6088237db77ff7514bd158f92`;
  filelists sha256 as in item 1; reused data vector sha256 as in item 3).
- Port builder:
  `H1ChargedSidis_HighQ2/scripts/unfolding/paired_legs_detints2prime_2026-10-11/build_shifted_response_data_vector_s2prime_v4.py`
  (mechanical base: legacy shifted v4 builder; S2′ delta source:
  `detint_variant_s2prime_2026-10-09/build_migration_th2_per_charge_s2prime.py`).
- Port smoke: `paired_legs/builder_port_smoke_2026-10-11/SMOKE_VERDICT.json`
  (overall_pass true; 82/82 per generator; contract (a)–(i)).
- S2′ RAW nominal:
  `output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (metadata: lumi_nb 351000.0; response_variant_id asserted
  `detint_fake_response_variant_S2prime_2026-10-09`; prior basis variant
  body+miss R10-asserted; data_vector = central UNCHANGED; integrated IBU
  h+ 11.446173604260975 / h− 11.158554076526913 nb; variant response
  sha256 RG31 `f6431c1c…79330d` / DJ14 `58172ab9…0d62e0`).
- Authoritative MC luminosities:
  `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
  (grand totals 9,132,891.0 / 10,487,495.0; 7091-alone 4,379,935.0;
  stale header field never quoted).
- Protocol: `sidis_highQ2/docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md`
  (Sec 4, 6.2–6.5; this gate = Sec 6.5 gate 2 refreshed per Sec 6.5.2).
- Regime context: `sidis_highQ2/docs/RUN_STATUS.md` (2026-10-11 S2′
  promotion override), `sidis_highQ2/STATE.md` (end-of-file override),
  `sidis_highQ2/docs/status/normalization.md` (normalization authority;
  O1 open).
- Superseded gate: `sidis_highQ2/docs/status/normalization-gate-paired-leg-elecE-down-2026-10-10.md`
  (pre-S2′; retained as provenance of the retained pre-S2′ campaign
  workspace and its diff-control Stage1 products, cluster 5485776).

## Gate checks summary

| Check | Result |
|-------|--------|
| Data leg identity (658 = 83/152/100/323; L_data 351000.0) | PASS |
| RAPGAP31 leg (7097/7096/7071/7094; 364 = 61/121/61/121; L_MC 9,132,891.0) | PASS |
| DJANGOH14 leg (7093/7092/7072/7091; 501 = 71/141/88/201; L_MC 10,487,495.0) | PASS |
| Reused data vector bit-identity + raw/no-f (sha256 guard) | PASS |
| Relative-only semantics (L cancels; only conversion --lumi-nb 351000.0; no MC lumi) | PASS |
| S2′ RAW release as sole denominator (identity-asserted) | PASS |
| Superseded-value sweep (scalars, paths, forbidden lumis) | CLEAN |
| R10 meta_variant_id steering guard (pre-merge, 8 products, ROOT+QA) | PASS |
| Port builder one-time validation (82/82 per generator, this shift) | PASS |
| DRY_RUN=1 Stage2 config | PASS (exit 0) |
| Campaign-local smoke verdict (22/22 checks, overall_pass true) | PASS |
| Workspace completion (smoke verdict, reuse record, README, DRIVER_COPIES.md5) | PASS (C4 resolved during gate) |

**Verdict: PASS.** Batch launch may proceed on supervisor approval. No
identity, luminosity, or normalization-semantics blocker exists. Standing
conditions: C1 (relative-only extraction + three-part basis labels),
C2 (future absolute products use 351000.0/L_MC from the mclumi record;
O1 documentation defect still open), C3 (L_data = 351000.0 nb⁻¹ only;
297,657.8 / 356,613.0 / 356,000 / 126,880 rejected on sight).
