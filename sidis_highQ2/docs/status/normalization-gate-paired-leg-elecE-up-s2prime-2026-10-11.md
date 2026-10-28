# Normalization gate — P2 paired-leg campaign 2 (S2′ basis), eElecEn UP (2026-10-11)

- **Gate ID:** normalization-gate-paired-leg-elecE-up-s2prime-2026-10-11
- **Date:** 2026-10-11 (gatekeeper: h1-normalization)
- **Scope:** normalization gate for campaign 2 side 2 on the S2′ detint-fake
  basis — eElecEn UP, five LAr wheels co-shifted +0.5%, tag
  `elecE_5wheel_p0p5pct`, workspace
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_up_s2prime_2026-10-11/`.
  Pre-launch gate: nothing submitted at gate time (Stage1 = 9 configured
  jobs, NOT executed; Stage2 BLOCKED ON Stage1 completion, supervisor
  approval, campaign-1 S2′ full-chain validation, and this record).
- **Methodology:** mirrors the campaign-1 S2′ redo gate
  (`normalization-gate-paired-leg-elecE-down-s2prime-2026-10-11.md`,
  PASS, 2026-10-11) item-for-item; this record is the UP-side companion.
  Differences from the down-side redo are called out explicitly (data
  vector NEWLY filled vs reused; QA-based integrity guard vs pinned
  sha256; port data-vector mode equivalence evidence).
- **Governing context:** user handoff of 2026-10-11 (task statement);
  `sidis_highQ2/docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md`
  (Sec 3(a), Sec 4, Sec 6.2 item 8, Sec 6.3–6.5; this gate = protocol
  Sec 6.5 gate 2 refreshed per Sec 6.5.2); `sidis_highQ2/docs/RUN_STATUS.md`
  2026-10-11 S2′ promotion override; `sidis_highQ2/STATE.md` end-of-file
  override.

## VERDICT: PASS — no blockers

Batch launch may proceed on supervisor approval (and, per the campaign
steering's own gating, after campaign-1 S2′ full-chain validation),
subject to the non-blocking conditions C1–C3 below.

## Campaign composition (verified facts)

| Leg | Composition | Files | L value (nb⁻¹) | Status |
|-----|-------------|-------|-----------------|--------|
| Data (NEW) | canonical 658-file list, ep0304/em0405/em06/ep0607 = 83/152/100/323 | 658 | L_data = 351000.0 | NEWLY filled by this campaign's own Stage1 job — LEGACY builder at +0.005 (canonical for data vectors, protocol Sec 6.2 item 8; S2′-inert); no prior +0.005 product exists (pre-S2′ up campaign never ran) |
| RAPGAP31 MC | catalogs 7097/7096/7071/7094 (ep0304/em0405/em06/ep0607) | 364 (61/121/61/121) | L_MC_total = 9,132,891.0 | S2′+shift port builder at +0.005 (4 jobs) |
| DJANGOH14 MC | catalogs 7093/7092/7072/7091 (ep0304/em0405/em06/ep0607) | 501 (71/141/88/201) | L_MC_total = 10,487,495.0 | S2′+shift port builder at +0.005 (4 jobs) |
| Nominal denominator | S2′ RAW release `bbb_ibu_detints2prime_..._2026-10-09/` (IBU k=4, 490 rows) | — | lumi_nb = 351000.0; integrated IBU h+ 11.446174 / h− 11.158554 | shift ratios vs THIS release only |

Stage1 = 9 jobs (1 data-vector + 4 rapgap31 + 4 djangoh14); Stage2 = 1
job. The pre-S2′ up-campaign workspace
(`paired_legs/elec_energy_up_2026-10-11/`) is retained untouched as
provenance — nothing was ever submitted from it (0 files in its logs and
stage1 trees at gate time); its legacy-builder smoke products at +0.005
serve as same-shift non-rerouted diff controls for this campaign's MC
smokes.

**No L_MC value is consumed anywhere in the campaign path.** The L_MC
totals are composition/provenance facts from the authoritative mclumi
record (`H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`:
`grand_total_per_generator_nbinv/rapgap31 = 9132891.000000002`,
`/djangoh14 = 10487494.999999996`); a grep sweep of this workspace finds
no MC-luminosity constant in any steering script, driver, or comparison
code.

## Item-by-item verification

### 1. Sample identities, file counts, L values — VERIFIED

- Filelists are byte-identical to the canonical lists verified file-for-file
  in the 2026-10-10 down-side gate and re-verified in the 2026-10-11
  down-side S2′ gate (sha256: `data_files_658.txt` =
  `ff237cc27117fe6a24785f9bdb8badf96871f8d45732cde5250e7e222c9f8926`,
  `rapgap31_files_364.txt` =
  `5810355de77b527112093fbcf7e72e5f7a6b634754c9ebaafd3ea01c04a126b6`,
  `djangoh14_files_501.txt` =
  `8db1266fac7e12b8dd6ea86674be9bbe6096a91e40b4d95aba66077073ba2bef`);
  the campaign smoke verdict independently re-verified the period splits
  (658 = 83/152/100/323; 364 = 61/121/61/121; 501 = 71/141/88/201) and
  byte-identity to the down-S2′ lists.
- Wrapper NUMPER mapping (`wrap_paired_elecE_migration_s2prime_v4.sh`) is
  identical to both down campaigns (RG 7097/7096/7071/7094; DJ
  7093/7092/7072/7091), giving job-for-job UP/DOWN leg-pair alignment.
- L_data = 351000.0 nb⁻¹ (current four-period regime value). REJECTED on
  sight in this regime: 297,657.8 (excludes ep0304; also the stale mclumi
  JSON header field — never quote), 356,613.0 / 356,000 (superseded
  direct-monitor quotes), 126,880 (wrong by ~42×).

### 2. Relative-only semantics on the S2′ chain — VERIFIED

- **Stage2 driver:** campaign-local copy
  `drivers/run_bbb_ibu_release_v4_coherentprior.py` is verified identical
  to the accepted driver pair TWO ways: md5
  `bccc9aed3278f1709cea7009c2a29509` (= the pair accepted in the
  2026-10-10 and 2026-10-11 down-side gates, whose L semantics were
  verified in code) and sha256
  `c87d476a6deb56e53395a31f91048b081e7b9ec0a338b352fd4c75b8dde51a00`
  (= the README-quoted value; crosscheck copy sha256
  `7af5bdeeb30094db3304359234cdda54d6a0b0330a7bad5c6127477870bcf6c6`,
  md5 `6f735fb6088237db77ff7514bd158f92`). The driver's ONLY absolute
  conversion is yields→nb via L_data; the Stage2 wrapper passes
  `--lumi-nb 351000.0`, `--ibu-selected 4`, iters 1–10, `--variant
  paired_legs_elec_energy_up_s2prime_2026-10-11`; per-reco-bin fake
  fraction `f_i = F_i/(Σ_j B_ij + F_i)` is formed by pure same-sample
  response ratios — no MC luminosity can enter.
- **Merge:** driver-facing merge is the UNCHANGED shared tool
  `merge_migration_per_charge_v4.py` (pure summation, verified in the
  prior gates); the S2′ diagnostics merge (unchanged tool, per generator)
  is not driver-facing.
- **Per-bin shift:** `steering/compare_vs_s2prime_nominal.py` is
  BYTE-IDENTICAL to the down-side S2′ gate's accepted version:
  `rel_shift = yield_unfolded_campaign / yield_unfolded_S2prime_nominal − 1`
  — a pure yield ratio; the common L_data = 351000.0 nb⁻¹ cancels
  exactly; yield and nb columns are ratio-identical; no luminosity
  constant appears in the script.
- **Denominator protection (structural):** fail-fast asserts before
  dividing — 490 rows; charges pos/neg; integrated cross_section_nb
  h+ 11.446174 / h− 11.158554 (tolerance 1e-6); and
  `response_variant_id == detint_fake_response_variant_S2prime_2026-10-09`
  on EVERY nominal row. The superseded pre-S2′ coherent-prior release
  cannot be substituted without tripping these assertions.
- **Chain order (Stage2 wrapper `run_paired_elecE_stage2_s2prime.sh`):**
  data-vector integrity guard (step 0) → R10 pre-merge guard → v4 merge →
  S2′ diagnostics merge ×2 → coherent unfold (`--lumi-nb 351000.0`, k=4)
  → comparison. `DRY_RUN=1` re-executed clean at gate time (exit 0):
  config OK, 0/4 + 0/4 MC migrations (batch not launched), campaign data
  vector not yet present (expected — own Stage1 product), tools present,
  denominator = S2′ RAW release with correct anchors.

### 3. Data-vector configuration — VERIFIED (legacy builder at +0.005 is the correct S2′-inert path; port-mode equivalence evidence EXISTS)

- **Batch path:** `wrap_paired_elecE_data_vector_v4.sh` invokes the LEGACY
  builder `build_shifted_response_data_vector_v4.py --mode data-vector`
  with `--shift zero --electron-energy-scale 0.005` over the canonical
  four-period data globs (expected 83/152/100/323 = 658) — exactly the
  down campaign's pre-S2′ data-vector job configuration (cluster 5485777)
  at the +0.005 sign. Per protocol Sec 6.2 item 8 the legacy builder
  REMAINS CANONICAL for data vectors (the S2′ delta is inert on data by
  construction; the data vector is f-independent raw counts, protocol
  Sec 3(a)/Sec 4). REPLACE production at the frozen amplitude — not a
  rescale of any legacy vector.
- **Port-mode equivalence evidence (required by this gate's item 3):**
  the campaign smoke exercised the port builder's inherited data-vector
  mode at +0.005 on the same file as the legacy builder:
  `smoke/smoke_data_portmode_shifted_elecE_5wheel_p0p5pct.*` vs
  `smoke/smoke_data_shifted_elecE_5wheel_p0p5pct.*`.
  `smoke/SMOKE_VERDICT.json` → `data_vector_builder_decision`:
  `port_mode_exercised_at_plus_0p005: true`, `port_mode_shift_stamp:
  0.005`, `port_mode_variant_id:
  detint_fake_response_variant_S2prime_2026-10-09`,
  **`port_mode_perbin_identical_to_legacy: true`**,
  **`port_mode_totals_identical: true`**, `batch_job_uses: LEGACY
  builder`, `pass: true`. The port QA carries the variant id and the
  +0.005 stamp; the legacy QA carries the +0.005 stamp (no variant id —
  correct for the legacy data builder). This is confirmation evidence
  only; the batch job uses the legacy builder.
- **Data-leg shift correctness (smoke):** five-wheel co-shift exact —
  energy ratio exactly 1.005 (max abs deviation 0.0), theta/phi
  unchanged, uniform across all wheel theta regions; migration evidence
  present (32 diff bins on the smoke file).
- **Production-product guard:** Stage2 step 0 asserts the produced data
  vector's QA before the merge — shift stamp exactly +0.005 with zero
  theta offsets, 658 input files, 0 failed, QA schema
  `shifted_response_data_vector_v4_qa`, JSON schema
  `data_vector_v4_shifted_native_fine_reco_raw_no_bgsub` (raw counts, no
  fake subtraction), 980 bins, exact label
  `raw_data_no_bgsub_elecE_5wheel_p0p5pct_fiveWheel_p0p5pct`; exit 4 on
  any failure. (Correctly NO variant-id requirement on the data leg —
  S2′-inert.)

### 4. Superseded-value sweep — CLEAN

- Grep sweep of the ENTIRE workspace for the superseded fake-fraction
  scalars `10.7141` / `10.4019` and the forbidden luminosities
  (`297657.8`, `356613`, `356000`, `126880`): ZERO occurrences, applied
  or reference.
- The superseded release path `bbb_ibu_coherentprior_..._2026-10-06`
  appears ONLY in denial context: the compare script docstring
  ("FORBIDDEN as denominator") and the smoke verdict's
  `superseded_forbidden_denominator` field. All other "coherentprior"
  strings are the driver FILENAME
  (`run_bbb_ibu_release_v4_coherentprior.py`, the accepted tool) and the
  campaign-local release-dir naming (`release_coherentprior_s2prime`,
  same convention as the down side) — not the superseded release.
- The applied fake-fraction basis is the per-reco-bin variant f_i from
  THIS campaign's own merged shifted S2′ response — not a scalar. The
  S2′ reference anchors (response-internal RG 11.2403%/10.8737%;
  data-weighted 11.3577%/10.9768%) remain recorded in RUN_STATUS/protocol
  and do not appear as applied constants here.

### 5. R10 meta_variant_id guard at steering level — PRESENT; +0.005 stamps VERIFIED in smokes

- `steering/guard_variant_id_s2prime.py` (UP version): asserts, for ALL
  8 per-period Stage1 products (expected names
  `mig_s2prime_elecE_5wheel_p0p5pct_{mc}_{period}`), in BOTH the ROOT
  file (TNamed `meta_variant_id`, `meta_electron_energy_scale`) and the
  QA JSON (top-level `variant_id`, `migration_summary.variant_id`
  cross-check, `shift.electron_energy_scale`):
  `meta_variant_id == detint_fake_response_variant_S2prime_2026-10-09`
  and shift stamp `0.005` (ROOT title carries no leading +); checks the
  v4-merge central object set; exit 2 on any failure; invoked as Stage2
  step 1, BEFORE the merge — exactly protocol Sec 6.3 step 1.
- **Smoke stamp verification (this gate's item 5):** campaign smoke
  `SMOKE_VERDICT.json` `overall_pass: true`, ALL 10 checks true. MC
  sections (real shards 7097/ep0304 and 7093/ep0304):
  `root_meta_energy_scale = "0.005"` and
  `root_meta_variant_id = detint_fake_response_variant_S2prime_2026-10-09`
  in both generators; QA shift stamps 0.005 everywhere; port-mode data
  smoke stamped 0.005 + variant id. MC S2′ semantics vs the pre-S2′
  up-campaign legacy same-shift controls: gen side invariant, priors /
  mc_count / miss rows BIT-IDENTICAL, B+F conservation holds, f_i only
  increases vs legacy (single-file integrated f_i: RG port 11.231%/10.822%
  vs legacy 10.550%/10.126%; DJ port 11.832%/11.574% vs legacy
  11.083%/10.876% — chunk-level values, consistent with the S2′ variant
  construction at +0.005), reroute evidence present.
- Port builder one-time validation (protocol Sec 6.2) remains PASS for
  the shared port builder (`builder_port_smoke_2026-10-11/`, 82/82 per
  generator at the down shift; this campaign's own smokes extend the
  evidence to +0.005 on the UP smoke units).

## Absolute-scale statement (same basis as the down-side gate)

This campaign produces NO absolute-nb MC-scaled product. Its only
absolute-nb products are the campaign release and the S2′ RAW nominal,
both constructed with L_data = 351000.0 nb⁻¹ only (correct), and the
systematic is extracted as their per-bin ratio, where L cancels exactly.
If any absolute-nb MC-scaled product is ever introduced on this campaign,
it MUST use `351000.0 / L_MC` computed from the mclumi record (RAPGAP31
0.03843251824641288; DJANGOH14 0.03346843073584302; 7091-alone
0.08013817556653238 by arithmetic) — with the O1 documentation defect
from the 2026-10-10 gate still open (the 351000-based scale STRINGS in
`normalization.md` are high by +7.6…+10.7 ppm vs arithmetic; L_MC values
themselves are consistent and authoritative; no scale constant is
consumed in this campaign path).

## 7091-alone cross-check — CONSISTENT

The DJANGOH14 leg is the merged 4-catalog treatment (L_MC_total =
10,487,495.0 nb⁻¹; 7093/7092/7072/7091 = 71/141/88/201 shards). The
7091-alone trap (L_MC = 4,379,935.0 nb⁻¹, documented scale
0.08013890218154498) does NOT apply: no MC luminosity is consumed
anywhere in this campaign, and the merged-leg composition matches the
nominal S2′ response basis (same four catalogs, same mapping as the down
side). Any future per-catalog absolute scaling must take per-catalog L
values from the mclumi record with sample-identity verification first.

## Conditions (non-blocking, travel with the campaign)

1. **C1 — relative-only extraction and quoting discipline.** Extract the
   systematic strictly as the per-bin relative shift vs the S2′ RAW
   release (`compare_vs_s2prime_nominal.py` output); never rescale either
   side; every quoted value carries the three-part basis label (prior
   basis: coherent body+miss from the campaign's shifted S2′ variant
   response; level: raw/no-bgsub, not Born-level, provisional; observable
   basis: S2′ detint-fake variant). Quoting an S2′ value against a
   pre-S2′ value without the observable-basis flag is FORBIDDEN. UP/DOWN
   leg-pair statements must use the two campaigns' shift ratios on the
   SAME S2′ basis (job-for-job aligned by construction).
2. **C2 — future absolute products.** Any future absolute-nb MC-scaled
   product must use `351000.0/L_MC` from the mclumi record (values above)
   after resolving O1; the mclumi JSON's stale header field
   (`L_data_HERA-II_S67_nb: 297657.8`) must never be quoted.
3. **C3 — regime luminosity discipline.** L_data = 351000.0 nb⁻¹ is the
   only current data luminosity for this regime; 297,657.8 / 356,613.0 /
   356,000 / 126,880 are rejected on sight.
4. **Minor workspace nits (non-blocking):** `drivers/DRIVER_COPIES.md5` is
   absent (the down-side workspace has one); the driver checksums are
   nonetheless recorded and verified twice (README sha256 quotes + smoke
   verdict `driver_copies` section, both confirmed by this gate's
   independent md5/sha256 computation). The Stage1 products (data vector
   + 8 migrations) do not exist yet BY DESIGN — they are this campaign's
   own batch outputs; the Stage2 step-0 integrity guard and the R10 guard
   will verify them before the merge.

## Provenance register (key artifacts)

- Campaign workspace:
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_up_s2prime_2026-10-11/`
  (steering: data-vector wrapper / migration wrapper / guard / compare /
  Stage2 + 3 condor files; drivers verified md5
  `bccc9aed3278f1709cea7009c2a29509` / `6f735fb6088237db77ff7514bd158f92`
  and sha256 `c87d476a…dea51a00` / `7af5bdee…bcf6c6`; filelists sha256
  as in item 1).
- Campaign smoke:
  `smoke/SMOKE_VERDICT.json` (overall_pass true, 10/10 checks;
  `data_vector_builder_decision` = the port-mode equivalence record;
  MC sections vs the pre-S2′ up-campaign legacy controls).
- Legacy data-vector builder (canonical for the data leg):
  `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_shifted_response_data_vector_v4.py`.
- S2′+shift port builder:
  `H1ChargedSidis_HighQ2/scripts/unfolding/paired_legs_detints2prime_2026-10-11/build_shifted_response_data_vector_s2prime_v4.py`
  (one-time port validation: `builder_port_smoke_2026-10-11/`, 82/82 per
  generator).
- S2′ RAW nominal:
  `output_v6/central/data_release/bbb_ibu_detints2prime_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-09/`
  (lumi_nb 351000.0; integrated IBU h+ 11.446173604260975 /
  h− 11.158554076526913 nb; per-row response_variant_id asserted).
- Authoritative MC luminosities:
  `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
  (grand totals 9,132,891.0 / 10,487,495.0; 7091-alone 4,379,935.0;
  stale header field never quoted).
- Protocol: `sidis_highQ2/docs/systematics/paired_legs_s2prime_protocol_2026-10-11.md`.
- Regime context: `sidis_highQ2/docs/RUN_STATUS.md` (2026-10-11 S2′
  promotion override), `sidis_highQ2/STATE.md` (end-of-file override),
  `sidis_highQ2/docs/status/normalization.md` (normalization authority;
  O1 open).
- Companion gates: `normalization-gate-paired-leg-elecE-down-s2prime-2026-10-11.md`
  (campaign-1 S2′ redo, PASS — methodology mirrored here) and
  `normalization-gate-paired-leg-elecE-down-2026-10-10.md` (pre-S2′,
  superseded for campaign 1; its file-for-file list verification
  underpins the byte-identical lists used here).
- Pre-S2′ up-campaign workspace (retained provenance, nothing ever
  submitted): `paired_legs/elec_energy_up_2026-10-11/` — its legacy
  +0.005 smoke products are this campaign's diff controls.

## Gate checks summary

| Check | Result |
|-------|--------|
| Data leg identity (658 = 83/152/100/323; L_data 351000.0) | PASS |
| RAPGAP31 leg (7097/7096/7071/7094; 364 = 61/121/61/121; L_MC 9,132,891.0) | PASS |
| DJANGOH14 leg (7093/7092/7072/7091; 501 = 71/141/88/201; L_MC 10,487,495.0) | PASS |
| Relative-only semantics (L cancels; only conversion --lumi-nb 351000.0; no MC lumi) | PASS |
| S2′ RAW release as sole denominator (identity-asserted; compare script = down-side accepted version) | PASS |
| Data-vector configuration (legacy builder at +0.005, S2′-inert, replace-not-rescale) | PASS |
| Port data-vector mode equivalence smoke (per-bin arrays + totals identical) | PASS (evidence exists) |
| Stage2 data-vector integrity guard (QA facts, exit 4) | PASS |
| Superseded-value sweep (scalars, release path, forbidden lumis) | CLEAN |
| R10 meta_variant_id steering guard (pre-merge, 8 products, ROOT+QA, +0.005) | PASS |
| +0.005 stamps in smokes (ROOT + QA, both generators, port-mode data) | PASS |
| Campaign smoke verdict (10/10 checks, overall_pass true) | PASS |
| DRY_RUN=1 Stage2 config | PASS (exit 0) |

**Verdict: PASS.** Batch launch may proceed on supervisor approval (with
the campaign steering's own additional gate: campaign-1 S2′ full-chain
validation). No identity, luminosity, or normalization-semantics blocker
exists. Standing conditions: C1 (relative-only extraction + three-part
basis labels; UP/DOWN pairing on the same S2′ basis), C2 (future absolute
products use 351000.0/L_MC from the mclumi record; O1 documentation
defect still open), C3 (L_data = 351000.0 nb⁻¹ only; 297,657.8 /
356,613.0 / 356,000 / 126,880 rejected on sight).
