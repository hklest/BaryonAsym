# CRAD Exposure Provenance Audit — MCDB / oomclumi / generator-log reconciliation (2026-10-09)

**Status: COMPLETE (2026-10-09).** All five steps are finalized and
checkpointed to the evidence directory. Headline verdict (Step 5): all 8
recorded L rows reconcile exactly to per-file/per-chunk arithmetic; five of six
legs grade CLOSED; RG31 NONRAD e- is BOUNDED-OPEN with a quantified one-file
numerator/denominator mismatch (+0.40% em / +0.20% mixed on the alternative
scheme; applied primary unaffected); exposure provenance stays at
`DIAGNOSTIC_MANIFEST_LEVEL` pending the RG31 fix, with generator-log closure
permanently blocked externally (MCDB CGI disabled).

**Task:** Resolve the `P-CRAD-LUMI` blocker's provenance side for the H1 high-Q²
charged-SIDIS C_rad chain: (1) DJANGOH e- RAD 4.67% count-shortfall root-cause;
(2) per-leg ReqID/genID/chain identification for all six legs; (3)+(4) oomclumi /
generator-log luminosity reconciliation for DJ14 and RG31 legs; (5) grading verdict
on whether exposure provenance can move from `DIAGNOSTIC_MANIFEST_LEVEL`.

**Evidence directory:** `sidis_highQ2/data/radiative/exposure_audit_2026-10-09/mcdb_reconciliation/`
**Read-only scope:** all campaign and release products; this audit only adds new
evidence/report files.

## Recorded per-leg luminosities (targets to reconcile)

| Leg | Recorded L (nb^-1) | Source of record |
|---|---|---|
| DJ14 RAD e+ | 16,206,393.0 | oomclumi-exact sweep `rad_oomclumi_exact_v2_20260924.json` (PASS 1373/1373) |
| DJ14 RAD e- | 13,534,017.0 | oomclumi-exact sweep `rad_oomclumi_exact_v2_20260924.json` (PASS 1373/1373) |
| DJ14 NONRAD e+ | 14,637,492.676 | exact tree-sum record `nonrad_old_new_v2_ratio_20260922_jsonfix/summary.json` (= old + new; Step 3) |
| DJ14 NONRAD e- | 14,541,275.213 | exact tree-sum record `nonrad_old_new_v2_ratio_20260922_jsonfix/summary.json` (= old + new; Step 3) |
| RG31 RAD e+ | 17,536,474.0 | oomclumi-exact `rapgap_rad_oomclumi_exact_20260930.json` (PASS 1273/1273) |
| RG31 RAD e- | 14,461,067.0 | oomclumi-exact `rapgap_rad_oomclumi_exact_20260930.json` (PASS 1273/1273) |
| RG31 NONRAD e+ | 13,954,768.0 | `rapgap_nonrad_oomclumi_exact_20260930.json` (PASS_WITH_EXCLUSIONS 503/520) |
| RG31 NONRAD e- | 13,416,991.0 | `rapgap_nonrad_oomclumi_exact_20260930.json` (PASS_WITH_EXCLUSIONS 503/520) |

## Step 1 — DJANGOH e- RAD 4.67% count shortfall root-cause — COMPLETE (local-evidence level)

**Verdict: the "3,000,004 missing events" are not thirty lost 100k chunks.** The
shortfall vs the naive 642×100k = 64.2M expectation decomposes exactly as:

| Contribution | Events | Share of 3,000,004 |
|---|---|---|
| Component 8962 is a **half-size production** (54 chunks × ~50k) | 2,724,510 | 90.8% |
| Partial series-final/first chunks in 8956/8957/8961 | 265,581 | 8.9% |
| Normalization residual (full chunks not exactly 100k) | 9,912 | 0.3% |

The lumi-implied retained total (61,209,908, from per-chunk oomclumi Lumi normalized
to each component's interior full-chunk lumi) matches the documented retained GenDIS
count 61,199,996 to **0.016%**; the e+ control matches to 0.057% (72,841,395 vs
72,799,996; its 300,004 shortfall is 86% partial finals/firsts).

Independent physical confirmation: 8962 hat chunks are ~32 MB vs ~64 MB for
full-size components (file-size check 2026-10-09), and σ×L_full/N_full is uniform
across components (mean event weight ≈3.33) — so 8962 is half the *events*, not a
weight change.

**Recoverability: zero.** Pool == retained exactly for all 8 components (1373
files); every series is a contiguous prefix from index 0000 with no interior gaps;
mc13-era steer counts are exactly 3× the pool counts for every component, so the
pool has had these counts since the mc13 production era. Nothing was lost between
the official OO pool and the campaign, and nothing is recoverable at pool level.

**Bias bound.** `sigma_rad(em) = sumW / L_rad(em)`, and the recorded
L_rad(em) = 13,534,017.0 nb⁻¹ is the *exact* oomclumi sum over the same 642 files
whose weights enter the C_rad numerator (sweep PASS 1373/1373; per-chunk
sumW = Sigma×Lumi identity holds). Numerator and denominator cover the same event
set, so the shortfall does **not** bias sigma_rad(em) or C_rad(em) — it only reduces
statistical precision, already carried as `c_rad_stat_rel`. The only residual bias
mechanism is a content-correlated truncated prefix of a larger MCDB request; local
evidence weighs against it (contiguous prefixes, mc13-era counts identical,
half-size is a production feature), but the request side is uncheckable: **MCDB CGI
is disabled** ("H1 scripts are disabled", 2015 DESY-IT transition) and **genIDs for
ReqIDs 8955–8962 are not identified**, so generator-log URLs
`https://www-h1.desy.de/imc/logfiles/djangoh14/gen_<genID>.txt` cannot even be formed.

This **supersedes** the "thirty lost chunks" reading in
`rad_conditioning_resolution_2026-09-25.md` and resolves caveat 1 of
`EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` (attrition-vs-partial) in
favor of variable chunk sizing at the pool level.

Evidence: `step1_shortfall_documented_facts.json`, `step1_rad_perchunk_oomclumi_lumi.json`,
`step1_pool_inventory_findings.json`, `step1_shortfall_rootcause_verdict.json`.

## Step 2 — Per-leg ReqID/genID/chain identification — COMPLETE

All six legs (8 recorded-L rows) are identified to the maximum level local evidence
supports. Summary table:

| Leg | Files (ep/em) | Source identity | Provenance level |
|---|---|---|---|
| DJ14 RAD | 731 / 642 | Official OO pool `/pnfs/.../mc2/oo-4.0/djangoh14/<id>/`, components **8955–8962** (workspace ReqID convention; `S39600.R97905`, release 4.0.25), pattern `4.0.25.DJANGOH14.NC.{EPLUSP.0304,EPLUSP.0607,EMINUSP.05,EMINUSP.06}.RAD.Q2GT60.CTEQ6L.{A1,A2,A3}`; component 8962 is the half-size e- production (54 chunks, Step 1) | OO-pool directory-ID (ReqID) level; trees verified on disk |
| DJ14 NONRAD-old | 260 / 260 | Britzger 2021 `norad_prod21/dst2all_djangoh/` (djangoh14 ar4.10): 4 series (A–D) × 65 chunks × 2 beams = 520; σ bookkeeping from 2021 LOGOUTPUT (em 7323.93945 pb, ep 7287.91504 pb, uniform per beam); 1:1 hat↔tree mapping in the inherited 734-row manifest | Production-directory + series + LOG-σ level; no ReqID/MCDB identity exists (not an MCDB-registered OO sample) |
| DJ14 NONRAD-new | 530 / 530 | `hk_0123_530x10k_eplusp_prod_20260911` (ep) + `hk_0123_eminusp_regen_20260916` (em); 530 jobs × 10k events, fresh seeds (rnin 910000–910529), **identical generator config to the 2021 baseline** (same stdjan steering, NORAD.Q2GT100.CTEQ6L, 27.6 GeV) — this config identity is what licenses the shared σ constant | Local campaign-directory level with preserved production metadata (steer, dbpar, rnin, stage logs); no MCDB/ReqID identity by construction (2026 private production) |
| DJ14 NONRAD-combined | — | Arithmetic sum of old + new per beam; the L actually used by the DJ14 finalizers for the pooled NONRAD leg | Defined operationally; reconciled in Step 3 |
| RG31 RAD | 691 / 582 | Official OO pool `/pnfs/.../mc2/oo-4.0/rapgap31/<reqid>/`, ReqIDs **8947–8954** (mirror set of 8955–8962), RAD.Q2GT60.CTEQ6L, `S39600.R97905`; per-component SIGMT recorded by the oomclumi sweep (1273-file sweep; 8951 has two generator sub-runs, 0.015% SIGMT spread) | OO-pool directory-ID (ReqID) level; trees = sweep files exactly |
| RG31 NONRAD | consumed 256 / 248 (expected 264 / 256) | Britzger `norad_prod21/dst2all_rapgap/`, series A–H, ep C0000000–C0000032 (33/series), em C0000001–C0000032 (32/series); 520 expected; per-chunk oomclumi records exist (503 usable) | Production-directory + series + per-chunk-record level; no ReqID/MCDB identity |

**RG31 NONRAD exclusions (17 = 16 + 1, classes differ — full analysis in Step 4):**

- **16 × C0000032** (8 ep + 8 em): degenerate chunks (8448 HAT entries vs ~156k
  typical; oomclumi Sigma=0.000, Lumi=-nan). Excluded from BOTH the exposure and
  the tree/histogram set — matched, no bias.
- **1 × D/em/C0000009**: oomclumi result file EMPTY (unusable; reproduced
  interactively as Sigma=0.000/Lumi=-nan). Excluded from the exposure ONLY — its
  tree `rapgap_nonrad_em_0104` IS histogrammed in all campaigns (blind FINAL,
  xFcut 2026-10-05, releasebin 2026-10-09). This is the one numerator/denominator
  file-set mismatch in the whole chain.

**genID/MCDB status, all legs:** no genIDs are identified for any pool-directory
ID (8955–8962, 8947–8954); MCDB CGI is disabled ("H1 scripts are disabled",
2015 DESY-IT transition — recorded fact from Step 1, not re-attempted), so
generator-log URLs cannot even be formed. The Britzger norad_prod21 and hk_0123
samples have no MCDB registration to resolve. **Honest ceiling for all legs:
source-directory + series + file level, with exact per-file exposure
arithmetic; generator-log cross-check is impossible.**

Evidence: `step2_leg_identification.json` (legs, components, series, tree counts
verified on disk: rad 731/642, nonrad_old 260/260, nonrad_new 530/530,
rapgap_rad 691/582, rapgap_nonrad 264/256 built / 504 consumed).

## Step 3 — DJ14 oomclumi / generator-log accounting — COMPLETE

**DJ14 RAD (e+ 16,206,393.0 / e- 13,534,017.0 nb⁻¹): CLOSED at oomclumi-exact
level, inherited from Step 1.** The recorded constants are the exact per-chunk
oomclumi sums over all 1373 pool files (sweep PASS 1373/1373; per-chunk
sumW = Sigma×Lumi identity holds), and the numerator and denominator file sets
are identical (trees 731/642 = sweep files). Residuals are external only
(genIDs unresolvable; the e- count shortfall is a pool-sizing feature, Step 1,
and does not bias sigma_rad(em) because both sides cover the same 642 files).

**DJ14 NONRAD (all four constants): CLOSED at exact tree-sum level.** Every
constant traces exactly to `nonrad_old_new_v2_ratio_20260922_jsonfix/summary.json`
as L = 1000 × sumW/σ over exactly the consumed tree files per beam:

| Leg | Beam | Files | sumW | σ (pb) | L (nb⁻¹) | Recorded |
|---|---|---|---|---|---|---|
| old | e+ | 260 | 94,160,646.7066884 | 7287.9104 | 12,920,115.854702111 | 12,920,115.855 ✓ |
| old | e- | 260 | 94,050,906.06759143 | 7323.94 | 12,841,572.441553513 | 12,841,572.442 ✓ |
| new | e+ | 530 | 12,516,088.396776915 | 7287.9104 | 1,717,376.821314504 | 1,717,376.821 ✓ |
| new | e- | 530 | 12,448,521.110179782 | 7323.94 | 1,699,702.7706643941 | 1,699,702.771 ✓ |
| combined | e+ | — | — | — | 14,637,492.676017 | 14,637,492.676 ✓ |
| combined | e- | — | — | — | 14,541,275.212218 | 14,541,275.213 ✓ (rounding, below) |

**σ provenance (2021 LOGOUTPUT, `norad_prod21/djangoh14_ar4.10/*.LOG`, uniform
per beam across all 4 series):** e- LOG 7323.93945 pb — used constant 7323.94 is
its exact rounding. e+ LOG 7287.91504 pb vs used 7287.9104 — a **6.4×10⁻⁷
relative bookkeeping discrepancy**, negligible; the constant has been in
workspace use since at least 2026-09-20 (`supplemental_nonrad_530_v1_summary.json`).
The new legs share the old legs' σ constants, licensed by config identity
(identical stdjan steering, NORAD.Q2GT100.CTEQ6L, 27.6 GeV; fresh seeds only —
Step 2).

**Combined-value rounding note:** the recorded combined constants equal the sum
of the 3-decimal-rounded legs; at full precision the e- sum is 14,541,275.212218
(delta −7.8×10⁻⁴ nb⁻¹, 5.4×10⁻¹¹ relative). A rounding-convention artifact, not a
reconciliation failure.

**Cross-checks and conventions:**

- **v2↔v3 tree equivalence:** `old_nonrad_ep_0000.root` is content-identical in
  the v2 ntuples and the v3 campaign trees (154,674 entries, sumW 364,787.7).
- **Per-file oomclumi is NOT available for DJ14 NONRAD** (Britzger dst2all and
  hk_0123 HATs yield no usable per-chunk oomclumi records). The
  numerator/denominator identity instead holds by construction at leg level:
  L = 1000·sumW/σ is computed over exactly the same files and weights that feed
  the histogrammed numerator.
- **Inherited-manifest convention trap:** the per-file
  `exposure_nb_inv_contributed` for nonrad_old in
  `input_output_exposure_manifest_inherited_2026-10-05.csv` is **count-based**
  (N_file/σ with σ = 7287.91 at 2 decimals): component sums 5,467,297.071 (e+)
  / 5,461,541.738 (e-) — a different convention from the campaign L
  (weighted sumW/σ; ratio = mean event weight 2.3632). Not an error (the
  finalizers used the weighted constants), but these count-based numbers must
  never be mistaken for the campaign L.

Evidence: `step3_dj14_nonrad_lumi_reconciliation.json`.

## Step 4 — RG31 oomclumi / generator-log accounting — COMPLETE

**RG31 RAD (e+ 17,536,474.0 / e- 14,461,067.0 nb⁻¹): CLOSED at oomclumi-exact
level.** Sweep PASS 1273/1273 (691 e+ + 582 e-); per-component SIGMT consistent
(8951 has two generator sub-runs, 0.015% spread); sumW = Sigma×Lumi identity
passes all 8 components; **numerator set = L set exactly** (trees 691/582 =
sweep files). Residual: genIDs unresolvable (external).

**RG31 NONRAD (e+ 13,954,768.0 / e- 13,416,991.0 nb⁻¹): recorded L reconciles
EXACTLY to per-chunk records; the file-SET composition does not.** Independent
re-sum of all per-chunk oomclumi results (this session): e+ 256 usable files →
13,954,768.000 nb⁻¹ (delta 0.000000); e- 247 usable files → 13,416,991.000
(delta 0.000000). The arithmetic is not in question — the 17 exclusions are:

| Class | Chunks | Nature | Treatment | Bias |
|---|---|---|---|---|
| A | 16 × C0000032 (8 ep + 8 em) | degenerate: 8448 HAT entries vs ~156k typical; oomclumi Sigma=0.000/Lumi=-nan | excluded from BOTH exposure and histogramming (0/16 in xFcut and releasebin queues, verified) | **none** — matched |
| B | 1 × D/em/C0000009 | **normal chunk**: 156,131 events, sumW 391,898.12; tree 210 MB vs 7 MB for degenerate em_0031; oomclumi result file 0 bytes, interactive rerun Sigma=0.000/Lumi=-nan (corrupted σ-bookkeeping header, not bad events) | excluded from the **exposure only**; its tree `rapgap_nonrad_em_0104` IS histogrammed in the blind FINAL, xFcut 2026-10-05, and releasebin 2026-10-09 | **one-file numerator/denominator mismatch**: em numerator 248 files vs em denominator 247 files |

The mismatch is visible in the releasebin campaign manifest itself
(`nonrad_em` input count 248 recorded against the 247-file em L).

**Missing-exposure reconstruction.** The six chunks surrounding C0000009 in
series D/em (C0000006–C0000008, C0000010, C0000011) all carry σ̂ = 7287.915 pb,
so the local-block estimate is L_miss = 391,898.12/7287.915 pb =
**53,773.7 nb⁻¹**. Variants: series-majority σ̂ (7323.939) → 53,509;
D-series neighbor-mean Lumi (n=30, range 53.375–55.327, mean 54.2608 pb) →
54,261. All inside the neighbor range; conclusions below move by <0.005 pp.

**Bias on C_rad (quantified; NO table modified — read-only audit):**

- **e- leg:** σ_nonrad(em) = sumW(248 files)/L(247 files) is overestimated →
  per-beam C_rad(em) biased **HIGH +0.4008%** (= 53,773.7/13,416,991). The
  pre-beam-mixing closure control RAPGAP31 em 1.1598444505 carries the full
  bias.
- **Mixed h+/h− tables** (pooled ep+em NONRAD denominator 27,371,759 nb⁻¹):
  both charges biased **HIGH +0.1965%**.
- Corrected integrated values (recorded → corrected, divide by 1.00196457
  mixed / 1.00400788 em):
  xFcut h+ 1.1368044553 → **1.134576**, h− 1.1362373359 → **1.134009**;
  releasebin h+ 1.1364508942 → **1.134223**, h− 1.1358814929 → **1.133654**;
  closure control em 1.1598444505 → **1.155214**.
- **Remedy options** (decision: radiative-correction owner, not this audit):
  (a) add the reconstructed 53,773.7 nb⁻¹ to L_nonrad(em) and re-finalize the
  RG31 tables, or (b) exclude em_0104 from the RG31 histogram set and
  re-finalize. Either closes the mismatch.

**Documentation discrepancy (corrected by this audit).**
`CRAD_FINAL_PROVENANCE_2026-09-30.md` caveat 1 says the 17 chunks are "all
C0000032 … excluded from BOTH exposure and histogramming so numerator and
denominator stay matched." **Wrong in detail:** the 17th is D/em/C0000009 (not
C0000032) and it was excluded from the exposure ONLY. The origin is traced to
the sweep JSON's own `excluded_note`, which conflates the two exclusion classes
(the unusable chunk is listed separately in the same file's `unusable` key).
The matched-numerator/denominator claim holds for 16 of 17 only.

**New finding — two-valued per-chunk σ̂ bookkeeping (RG31 NONRAD only).** The
consumed sample's per-chunk σ̂ is two-valued {7287.915, 7323.939 pb} —
numerically identical to the DJANGOH norad LOG σ pair (framework-copied
constants, not RAPGAP-computed) — mixed within every series and both beams:
e+ 226/256 chunks at 7287.915 (σ_eff = 7292.099 pb), e- 119/247 (σ_eff =
7306.505 pb). Checks: all 520 campaign hats are symlinks into
`dst2all_rapgap_Ee27p6/` only (Ee27p5 is a separate, unconsumed production) —
so the mixing occurs within one production, it is NOT an energy mix; steer
files are identical between σ̂ classes; the constants originate upstream in the
generator BOS inputs (/pnfs/…/generator_files/rapgap31/, inaccessible).
**Bounded impact if a single per-beam constant was intended:** beam-matched
(ep→7287.915, em→7323.939) → C_rad bias e+ +0.06%, e- −0.24%, mixed −0.09%;
swapped → e+ −0.44%, e- +0.26%, mixed −0.10%. If the per-chunk values are
genuine, L is self-consistent (no bias). Locally unresolvable; creates no
numerator/denominator mismatch and does not affect the exactness of the
recorded-L reconciliation, but it caps the leg's grading one level below the
RAD legs. Contrast: DJ14 bookkeeping is uniform per beam; RG31 RAD SIGMT is
consistent to ≤0.015%.

**Applied-primary implication.** The mismatch bias sits in the RG31 *formal
alternative* scheme only; the applied primary (DJ14 xFcut, h+ 1.1418534312 /
h− 1.1413755529) is unaffected. Correcting RG31 (−0.20% mixed, −0.40% em) would
slightly increase the DJ14/RG31 integrated spread (currently +0.4441%/+0.4522%)
feeding `c_rad_gen_rel` — flagged as an implication, not re-derived here.

Evidence: `step4_rg31_lumi_reconciliation.json`.

## Step 5 — Grading verdict — COMPLETE

**Grading scale** (used below): `CLOSED_OOMCLUMI_EXACT` = recorded L is exact
per-chunk oomclumi arithmetic with matched sets; `CLOSED_TREE_SUM_EXACT` = exact
tree-sum/constant arithmetic, identity by construction; `CLOSED_PER_CHUNK_EXACT`
= exact per-chunk arithmetic and matched sets, but a σ-bookkeeping residual is
carried; `BOUNDED_OPEN` = arithmetic exact but a quantified numerator/denominator
mismatch is pending a fix decision.

| Leg | L recorded vs reconciled | Set match | Verdict |
|---|---|---|---|
| DJ14 RAD | e+ 16,206,393.0 / e- 13,534,017.0 — exact oomclumi sums (sweep PASS 1373/1373) | identical (trees 731/642 = sweep files) | **CLOSED_OOMCLUMI_EXACT** (residual: genIDs unresolvable; Step 1 shortfall = sizing, no L bias) |
| DJ14 NONRAD-old | e+ 12,920,115.855 = 1000×94,160,646.7066884/7287.9104; e- 12,841,572.442 = 1000×94,050,906.06759143/7323.94 (260 files each) | by construction | **CLOSED_TREE_SUM_EXACT** (residual: ep σ 6.4×10⁻⁷) |
| DJ14 NONRAD-new | e+ 1,717,376.821 / e- 1,699,702.771 — same formula over 530 files each | by construction | **CLOSED_TREE_SUM_EXACT** (shared σ licensed by config identity) |
| DJ14 NONRAD-combined | e+ 14,637,492.676 / e- 14,541,275.213 — sums of legs (rounding artifact −7.8×10⁻⁴ nb⁻¹ em) | by construction | **CLOSED_TREE_SUM_EXACT** |
| RG31 RAD | e+ 17,536,474.0 / e- 14,461,067.0 — exact oomclumi sums (PASS 1273/1273, identity all 8 components) | identical (691/582) | **CLOSED_OOMCLUMI_EXACT** (residual: genIDs unresolvable) |
| RG31 NONRAD e+ | 13,954,768.0 — exact re-sum over 256 per-chunk records (delta 0.000000) | matched (256 = 256) | **CLOSED_PER_CHUNK_EXACT** (σ̂ residual: C_rad(ep) ambiguity +0.06%/−0.44%) |
| RG31 NONRAD e- | 13,416,991.0 — exact re-sum over 247 per-chunk records (delta 0.000000); arithmetic exact, **set is not** | **MISMATCH: numerator 248 vs denominator 247** | **BOUNDED_OPEN** (C_rad(em) +0.4008% high, mixed +0.1965% high; σ̂ residual −0.24%/+0.26%) |

**RG31 NONRAD 17-exclusion disposition (explicit):**

- **16 × C0000032:** correctly excluded from BOTH sides in all campaigns
  (queue-verified 0/16); matched; no bias; **no action**.
- **1 × D/em/C0000009:** incorrectly excluded from the exposure ONLY while its
  normal 156,131-event tree stayed in the numerator → +0.40% em / +0.20% mixed
  bias on the RG31 alternative scheme. **Requires a fix** — (a) add the
  reconstructed 53,773.7 nb⁻¹ to L_nonrad(em) and re-finalize, or (b) exclude
  em_0104 from the RG31 histogram set and re-finalize. Decision: radiative-
  correction owner. The documentation error (CRAD_FINAL_PROVENANCE caveat 1 and
  the sweep JSON `excluded_note`) is corrected by this audit.

**Overall verdict: exposure provenance CANNOT yet leave
`DIAGNOSTIC_MANIFEST_LEVEL`.**

- The reconciliation side of `P-CRAD-LUMI` is **complete at the local-evidence
  ceiling**: all 8 recorded L rows trace exactly to per-file/per-chunk
  arithmetic (deltas 0.0, or explained rounding artifacts); all input file sets
  are enumerated; the "thirty lost chunks" reading is superseded (Step 1); the
  filesystem duplicate/overlap audit is clean (0/0 — cited, not redone).
- **Five of six legs qualify** for an upgraded "FILE-SET-RECONCILED
  (generator-log-open)" label; **RG31 NONRAD em does not** until its mismatch
  is fixed and the RG31 tables re-finalized. Relabeling is a state change owned
  by the blocker owner, to be executed only after the RG31 fix.
- **No leg can reach generator-log closure**: genIDs are unresolvable (MCDB CGI
  disabled — permanent external blocker) and the RG31 NONRAD σ constants are
  unresolvable locally (two-valued σ̂; pnfs generator inputs inaccessible).
- **Publication-grade: NO — unchanged.** The separate radiative gates stand:
  PROVISIONAL xFcut baseline (promotion-gate review only), open EMPZ>45
  convention decision (a future flip supersedes the applied table), mixed-beam
  validity only.
- **Applied primary unaffected** by every finding in this audit (the defect is
  in the RG31 alternative scheme only). Correcting RG31 would slightly increase
  the DJ14/RG31 spread feeding `c_rad_gen_rel` — systematics re-derivation at
  that point, out of audit scope.

Evidence: `step5_grading_verdict.json`.

## Addendum — RG31 NONRAD e- exposure repair and grading update (2026-10-09)

**User decision executed:** remedy (a), add reconstructed exposure to the existing
histogrammed denominator.  The finalizer was rerun against the unchanged releasebin
partials with `L_nonrad(em)` increased from `13,416,991.0` to `13,470,764.7 nb^-1`
by `53,773.7 nb^-1`.  The reconstructed contribution is for normal tree
`D/em/C0000009` / `rapgap_nonrad_em_0104` (156,131 events; sumW 391,898.12), using
the Step-4 local-block `sigma_hat=7287.915 pb` reconstruction.  There was no
re-histogramming and no Condor campaign: the original 248-tree e- NONRAD histogram
already contained this chunk.

**Corrected product:**
`sidis_highQ2/data/radiative/rapgap_crad_releasebin_emfix_2026-10-09/`.
The original `rapgap_crad_releasebin_2026-10-09/` is retained unchanged as provenance
and contains `README_SUPERSEDED_2026-10-09.md`.  Corrected releasebin integrated
values are h+ `1.1342370401351234` and h- `1.1336689310022947`, consistent with the
Step-4 rounded predictions `1.134223` / `1.133654` (unrounded deviations
`+0.00001404` / `+0.00001493`).  Validation is in
`emfix_validation.json` beside the table.

### Step 4 update

The former one-file numerator/denominator mismatch is **CLOSED**: the e- exposure now
covers the 248-tree histogrammed file set.  The 16 degenerate `C0000032` chunks remain
excluded from both sides.  The two-valued RG31 NONRAD `sigma_hat` provenance residual
remains as documented in the original Step 4; it is not a file-set mismatch and is
unchanged by this repair.

### Step 5 grading update

| Leg | Updated set match | Updated verdict |
|---|---|---|
| RG31 NONRAD e- | matched: numerator 248 trees = corrected exposure 247 usable records + reconstructed D/em/C0000009 contribution | **CLOSED_PER_CHUNK_EXACT (reconstructed missing exposure; sigma-hat provenance residual retained)** |

**Overall verdict upgraded:** **FILE-SET-RECONCILED (generator-log-open)**.  All
histogrammed input sets now match their finalization exposures at the local-evidence
level.  Generator-log closure remains permanently blocked externally because MCDB CGI
is disabled and genIDs cannot be resolved; the RG31 two-valued sigma-hat bookkeeping
residual also remains.  This upgraded grading does **not** make C_rad publication-grade
and does not alter the applied DJANGOH14 primary table.
