# Normalization Gate — P2 paired detector legs, campaign 1 side 1: eElecEn DOWN (2026-10-10)

- **Gate:** luminosity / sample-identity / normalization semantics of the paired-leg campaign
  `elec_energy_down_2026-10-10` (tag `elecE_5wheel_m0p5pct`, five LAr wheels co-shifted −0.5%),
  before Condor batch launch. Read-only verification.
- **Gating authority:** `h1-normalization` (regime context: `sidis_highQ2/context-handoff.md`,
  `STATE.md`, `docs/RUN_STATUS.md`, `docs/status/normalization.md` read first per AGENTS.md; the
  `sidis-hq2-normalization` skill was unavailable in this runtime, so the workspace documents were
  used directly).
- **Date:** 2026-10-10.
- **Campaign workspace:**
  `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_down_2026-10-10/`
  (steering, filelists, smoke verdicts, README).
- **Campaign protocol prerequisites** (gap audit `docs/systematics/provisional_to_final_gap_audit_2026-10-10.md`
  §4): canonical 658-file input list (§3.4 fix) + single-file smoke test + normalization check
  (`L=351000.0`, relative-only outputs) + coherent `body+miss` prior, `k=4`, binmap v3, comparison
  vs the coherent nominal. This record is the normalization-check component. The smoke component is
  already PASS (`smoke/SMOKE_VERDICT.json`, `overall_pass: true`, all three legs: data, rapgap31,
  djangoh14); the canonical-list component is verified here (check 1).

## VERDICT: **PASS**

All sample identities, file lists, and luminosity values verify against the authoritative workspace
records, and the shift computation is relative-by-ratio with no luminosity rescaling anywhere in
the campaign path. The absolute-nb intermediates that the Stage2 release produces are correctly
scaled by `L_data = 351000.0 nb^-1` only. **The batch is NOT blocked by normalization.** No
blockers. Non-blocking conditions and observations are listed at the end.

## Check results

### 1. Data leg — canonical 658-file list — **PASS**

- Campaign list `steering/filelists/data_files_658.txt`: **658 files = ep0304:83 / em0405:152 /
  em06:100 / ep0607:323** — file-for-file identical (sorted `diff` empty) to the on-disk canonical
  central globs `output_v6/central/sidistree/data/{period}/sidistree_*.root` that the Stage1 wrapper
  `wrap_paired_elecE_data_vector_v4.sh` resolves.
- Central data vector
  `output_v6/central/tunfold_official_v4_finalcandidate_.../data_vector_...json`:
  `n_input_files: 658`, `bg_subtracted: false` (raw, matching the campaign builder mode
  `raw_data_no_bgsub`); its embedded provenance list is truncated by design
  (`input_files_truncated: true`, 200 real entries) and **all 200 real entries are members of the
  campaign 658 list** (subset check PASS).
- Independent corroboration: tracking-efficiency validation proved input count 658 with period
  counts 83/152/100/323 (16/16 replicas); BSA production verified 658/658 entry-count parity with
  central v6; legacy `elecE_p1pct` QA records `n_input_files: 658`. The §3.4 HFS 660-file defect
  does not apply (electron legs used the exact 658 list; verified again here).
- **`L_data = 351000.0 nb^-1 = 351 pb^-1`** (four-period S67 including ep0304) — authoritative per
  `docs/status/normalization.md:10`, `STATE.md`, `context-handoff.md`. Semantic sweep of the
  campaign (steering, drivers, README, smoke artifacts): **zero occurrences** of the forbidden
  superseded values `297657.8`, `356613.0`, `356000`, `126880` nb^-1.

### 2. RAPGAP31 MC leg — **PASS**

- **Identity (precise):** the leg is NOT a single sample — it is the four per-period RAPGAP31
  catalogs of the central response: **7097/ep0304 (61 files), 7096/em0405 (121), 7071/em06 (61),
  7094/ep0607 (121) = 364 files**. (`7097` is the ep0304 catalog; the Stage1 wrapper NUMPER map is
  exactly 7097/7096/7071/7094; the single-file smoke used `7097/..._ep0304_0000.root`.)
- File list `steering/filelists/rapgap31_files_364.txt` is file-for-file identical (sorted `diff`
  empty) to the on-disk canonical
  `output_v6/central/sidistree/mc/rapgap31/{7097,7096,7071,7094}/sidistree_rapgap31_*.root`; the
  counts equal the nominal central per-period migrations' `meta_n_input_files` = 61/121/61/121
  (read from the ROOT meta directly).
- **Effective MC luminosity** (authoritative record
  `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`, `per_catalog`, 0 chunks
  failed, chunk counts equal file counts):
  7097 = 1,529,489.0 + 7096 = 3,033,378.0 + 7071 = 1,515,146.0 + 7094 = 3,054,878.0 =
  **`L_MC_total(RAPGAP31) = 9,132,891.0 nb^-1`** — matches `docs/status/normalization.md:59`.
- Scale to 351000.0 **if absolute MC scaling were ever applied**: documented constant
  `0.03843280892435371` (`normalization.md:59`); arithmetic recomputation
  `351000.0/9132891.0 = 0.03843251824641288`. **Not consumed anywhere in this campaign** (see
  check 4); the ppm-level difference between the documented string and the arithmetic is a
  pre-existing documentation defect (observation O1), irrelevant here.

### 3. DJANGOH14 MC leg — **PASS**

- **Identity:** four per-period DJANGOH14 catalogs: **7093/ep0304 (71 shards), 7092/em0405 (141),
  7072/em06 (88), 7091/ep0607 (201) = 501 shards**. File list
  `steering/filelists/djangoh14_files_501.txt` is file-for-file identical (sorted `diff` empty) to
  the on-disk canonical `output_v6/central/sidistree/mc/djangoh14/{7093,7092,7072,7091}/`.
- Matches the **nominal** response's DJ leg: nominal central per-period migrations
  `mig_v4_djangoh14_{period}.root` carry `meta_n_input_files` = 71/141/88/201 (read from the ROOT
  meta directly; `meta_mc_label = djangoh14`), and the legacy `elecE_p1pct` DJ shifted legs carry
  QA `n_input_files` = 71/141/88/201. The campaign therefore reproduces the nominal leg exactly
  (decision (a) intent: shifted merged response structurally identical to nominal, detector shift
  as the only difference).
- **`L_MC_total(DJANGOH14) = 10,487,495.0 nb^-1`** — CONFIRMED at `docs/status/normalization.md:60`
  (as cited by the campaign README) and independently from the authoritative mclumi record:
  7093 = 1,535,773.0 + 7092 = 3,047,386.0 + 7072 = 1,524,401.0 + 7091 = 4,379,935.0 =
  10,487,495.0 nb^-1 (JSON `grand_total_per_generator.djangoh14` = 10487494.999999996; 0 chunks
  failed; chunk counts equal file counts).
- Scale to 351000.0 **if absolute MC scaling were ever applied**: documented constant
  `0.03346878747387811` (`normalization.md:60`, README:125 — provenance quote only); arithmetic
  recomputation `351000.0/10487495.0 = 0.03346843073584302`. **Not consumed anywhere in this
  campaign** (see check 4).
- Count nit: gap audit §4 line 87 says "(+502 if the DJANGOH14 branch is also shifted)"; the
  correct shard count is **501** (502 = 501 shards + the `merged_7091.root` standing file, or a
  stale count). The campaign uses the 501 shards, consistent with the nominal leg. Documentation
  nit only (observation O3).

### 4. CRITICAL semantics: relative-only shift, no luminosity rescaling — **PASS**

- **Stage1** (`build_shifted_response_data_vector_v4.py` via the campaign wrappers): MC weighted
  fills only (`w_reco = weight1 × weight2 × NucliaWeight`, `w_gen = weight1 × weight2`); data
  unweighted. No luminosity anywhere in either wrapper.
- **Stage2 merge** (`merge_migration_per_charge_v4.py`): pure TH2/TH1 **summation** per
  (MC, charge) across the per-period files; no luminosity, no scale constants. The REV4
  data-overflow constraint uses the same-response count ratio
  `miss_row_mc × (data_total / matched_total)` — a data/MC count ratio, not a luminosity.
- **Stage2 coherent driver** (`drivers/run_bbb_ibu_release_v4_coherentprior.py`, md5
  `bccc9aed3278f1709cea7009c2a29509`, identical to the 2026-10-07 accepted source and to
  `DRIVER_COPIES.md5`): the **only** luminosity is `L_data` (`--lumi-nb 351000.0`, default
  `LUMI_DEFAULT = 351000.0`), used solely to convert unfolded yields to nb
  (`cross_section_nb = vec[i]/lumi` etc.). `derive_response_blocks` is pure same-sample ratios
  (`f_i = F_i/(Σ_j B_ij + F_i)` — any global MC normalization, including MC luminosity, cancels
  exactly; `d_sig = data × (1 − f_i)`; `A_j` a pure ratio); the coherent prior is this response's
  own truth marginal (`body+miss`); IBU/BBB produce data-scaled yields. The driver's companion
  `crosscheck_unfold_methods_v4.py` carries only OPTIONAL diagnostic `--lumi-nb` scaling, not
  invoked by the campaign Stage2 script.
- **Semantic sweep result:** zero occurrences in the campaign path (steering, drivers, smoke) of
  `9132891`, `10487495`, `4379935`, `15813.4`, `22.2017`, `0.0801…`, `0.0384…`, `0.0334…`, or any
  forbidden data-lumi value; the only MC-lumi number anywhere in the campaign is the README's
  provenance quote of the DJ total (documentation, not code).
- **Explicit statement:** the Stage2 driver does **NOT** produce relative-only quantities. It
  produces a full release: lumi-free yield columns, relative uncertainty columns, and
  **absolute-nb columns scaled by `L_data = 351000.0` only** — which is the correct current
  four-period target. The per-bin detector shift (shifted unfolded / nominal unfolded − 1) is
  formed downstream against the coherent nominal (central release
  `bbb_ibu_coherentprior_..._2026-10-06/`, `lumi_nb: 351000.0`, `body+miss`, `k=4` — verified in
  its `metadata.json`). Because both sides use the SAME `L_data = 351000.0`, **the absolute L
  factor cancels exactly in the ratio** (identical result whether the yield or nb columns are
  used), and because **no MC luminosity enters the unfolded yields at any point** (the response
  enters only through conditional/ratio structures), **no effective-MC-scale error can
  contaminate the shift**. The effective-MC-scale question is therefore moot for this campaign's
  deliverable; it would matter only for absolute-nb MC-yield products, which this campaign does
  not produce.
- `DRY_RUN=1 bash steering/run_paired_elecE_stage2.sh` re-executed at gate time: config OK,
  DJ-leg decision-abort REMOVED (decision (a)), Stage1 products pending, exit 0.

### 5. Known-good 7091 cross-check — **PASS** (distinct treatments, both correct)

- Known-good single-sample values stand: `merged_7091.root` = `djangoh14/7091` RAD high-Q² signal
  MC (`NC.EPLUSP.0607.RAD.Q2GT60`), `L_MC = 4,379,935.0 nb^-1` (mclumi record
  `per_catalog/djangoh14_7091_ep0607`), documented scale at `L_data=351000.0` =
  `0.08013890218154498` (arithmetic `0.08013817556653238`); the `15813.4`-based DjBkgd scale
  `0.0221963651` and the stale `22.2017` remain what the trap records say.
- The campaign's merged DJ leg is the **four-catalog** set with `L_MC_total = 10,487,495.0 nb^-1`
  — **2.394× the 7091-alone luminosity** (7091 is included as the ep0607 catalog). Applying the
  7091-alone scale to the merged leg would be wrong; the campaign does **not** do this — it
  applies **no MC scale at all** (the shift is relative; check 4). The differing treatment is
  documented: `normalization.md:21-31` (7091-alone trap) vs `normalization.md:46-62`
  (generator-wide combined lumis for absolute template normalization), and the campaign README
  cites the combined value with its source line. The 7091-alone constant applies to
  `merged_7091`-type single-sample absolute products only (e.g. the alpha0p2 low-Q² leakage
  template), not to this campaign.

## Conditions (non-blocking; bind the batch campaign, not the launch)

1. **Extract the systematic as the relative ratio** shifted-unfolded / nominal-unfolded − 1
   against the coherent nominal (both sides `L=351000.0`, `body+miss`, `k=4`), per the §4
   protocol ("relative-only outputs"). Never rescale either side to a different luminosity, and
   never quote the campaign's absolute-nb columns as physics without the standard Born-or-raw and
   provisional caveats (they are raw/no-bgsub, `c_rad` NOT applied inside this campaign).
2. **If any future absolute-nb MC-scaled product is ever derived from these legs**, use
   `351000.0 / L_MC` computed from the authoritative mclumi record (RAPGAP31
   `0.03843251824641288`; DJANGOH14 `0.03346843073584302`; 7091-alone `0.08013817556653238`) and
   resolve observation O1 first.
3. **Do not quote** the mclumi JSON header field `L_data_HERA-II_S67_nb: 297657.8` (stale
   2005–2007-only value; known documented discrepancy, unconsumed by the chain).

## Observations (no action required for launch)

- **O1 — pre-existing scale-string documentation defect (workspace-level, unconsumed here).** The
  three 351000-based mclumi scale strings in `docs/status/normalization.md`
  (`0.03843280892435371` RG31, `0.03346878747387811` DJ14, `0.08013890218154498` 7091) are high by
  +7.6 / +10.7 / +9.1 ppm relative to `351000.0/L_MC` arithmetic from the authoritative record;
  the 356613-based constant (`0.08141970143392539`) and the DjBkgd scale (`0.0221963651`)
  reproduce exactly. The `L_MC` values themselves are mutually consistent across all records and
  are authoritative; the scale strings appear to be frozen legacy strings. Zero effect on this
  campaign (no constant consumed). Recommend a `normalization.md` follow-up: recompute or annotate
  the scale strings.
- **O2 — stale condor header comment.** `submit_paired_elecE_stage2.condor` still says "BLOCKED ON
  the Stage2 DJANGOH-leg decision item"; the decision was resolved 2026-10-10 (option (a)) and the
  executable's abort was removed (`DRY_RUN=1` clean). Cosmetic; the executable is authoritative.
- **O3 — gap-audit count nit.** §4 line 87 "(+502 if the DJANGOH14 branch is also shifted)" vs the
  actual **501** shards (502 = 501 + `merged_7091.root`, or stale). The campaign list and the
  nominal leg meta both say 501.
- Driver copies checksum-verified twice (campaign `DRIVER_COPIES.md5` and independent md5 vs the
  2026-10-07 `detector_shifted_coherentprior` sources); no shared script or H1OO source was edited
  for this campaign; smoke products are single-file and distinct from the batch outputs.

## Provenance register (primary artifacts)

- Campaign README: `H1ChargedSidis_HighQ2/output_v6/systematics/paired_legs/elec_energy_down_2026-10-10/README.md`
- Filelists: `.../steering/filelists/{data_files_658,rapgap31_files_364,djangoh14_files_501}.txt`
- Smoke verdict: `.../smoke/SMOKE_VERDICT.json` (`overall_pass: true`)
- Stage2 driver: `.../drivers/run_bbb_ibu_release_v4_coherentprior.py` (+ `DRIVER_COPIES.md5`)
- MC luminosity record: `H1SIDIS_HighQ2/mclumi_chunks/highQ2_mc_luminosity_aggregated.json`
  (per_catalog rapgap31_709{6,7}/7071/7094, djangoh14_709{1,2,3}/7072; stale header field per
  condition 3)
- Normalization authority: `sidis_highQ2/docs/status/normalization.md` (L_data §"Current Data
  Luminosity"; 7091 trap; combined lumis :55-62)
- Nominal per-period migrations (leg-identity cross-check):
  `H1ChargedSidis_HighQ2/output_v6/central/tunfold_official_v4_finalcandidate_.../per_file_migration/{rapgap31,djangoh14}/`
- Coherent nominal (comparison partner):
  `H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_coherentprior_..._2026-10-06/`
  (`metadata.json`: `lumi_nb: 351000.0`, `prior_basis: body+miss`, `ibu_selected_iter: 4`)
- Protocol: `sidis_highQ2/docs/systematics/provisional_to_final_gap_audit_2026-10-10.md` §3.4, §4;
  `FINAL_SYSTEMATICS_DEFINITION_SPEC_2026-09-30.md` row 8 / §2.2-1

**Gate decision: PASS — the normalization gate for campaign `elecE_5wheel_m0p5pct` (side 1, DOWN)
is closed. Batch launch may proceed on the supervisor's approval; conditions 1–3 travel with the
campaign.**
