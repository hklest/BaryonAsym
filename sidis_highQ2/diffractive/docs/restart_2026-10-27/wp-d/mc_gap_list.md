# WP-D: MC inventory re-judgement and gap list (restart 2026-10-27)

**Scope.** This record re-judges, from scratch, every MC sample in the v6dif production and every
candidate named in WP-D of the program plan
`/data/dust/group/h1/klesthen/BaryonAsym/.supervisor/working/plans/2026-10-27-diffractive-sidis-highq2-restart.md`
for the Q² > 150 GeV² LRG X-frame x_F / p_T*² restart. It closes register rows R24 and R26 at the
evidence level and gives the gap list with owners and costs.

**Out of scope.**
- The C_rad pair usability ruling (WP-I).
- Normalisation or luminosity re-derivation (WP-E, `h1-normalization`).
- Any production run.
- Filing any MC request: per the supervisor decision, no new official request is made before gate G1.

**Conventions.**
- `D/` = `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/`.
- `DB` = the IMC database snapshot `/afs/desy.de/group/h1/iww2/imc/h1db.js` (dated 2019-03-15;
  request rows carry ReqID, genID, period, SW version and event count; generator rows carry
  comments).
- `PNFS` = `/pnfs/desy.de/dphep/online/h1/mc/`.
- Tags: **[V]** verified in the cited source or by a direct listing; **[P]** provisional, meaning a
  single source or an analogy; **[I]** an inference made by this record.

**Evidence chain.**
- Provenance comes from `h1-mc-db`: DB rows, steering and job scripts under `D/condor/`, and PNFS
  listings.
- Local facts (luminosities and scale rows) come from a `researcher-worker`.
- Response-level verdicts come from `h1-mc-expert`.
- The lead spot-checked the following by direct listing:
  - `/acs` is absent.
  - 8531 has 100 DST files on PNFS `rapgap31/05/8531`, and its HAT+MODS are present.
  - 8221, 7448 and 8329 have HAT+MODS only.
  - 8261 has 200 DST files.
  - 9013 has 48 HAT+MODS files.
  - The 6671 steering has `fLoadODS = false`.
  - The DB generator comments for 7071, 7091, 7094, 8947, 8948 and 8955.

## 1. Verdict table

Verdicts:
- **KEEP**: the sample enters the restart as is (re-produced in WP-E with the writer fork).
- **COND**: kept only under the stated condition.
- **DROP**: the sample is not used.

Notation in the table:
- "evls" means the sample was read through the parent's existing v6 evls files.
- "L" is the luminosity from `D/docs/mc/composite_scale_table_v3_2026-10-20.csv` (`V3:<row>`), the
  sidecar `D/docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv` (`SC:<row>`), or
  `D/docs/mc/luminosity_diffractive_mc_2026-10-16.csv` (`LUM:<row>`). The CSV `family` column is
  known to be mislabelled and was not read.

### 1a. Samples already in v6dif (`D/output_v6dif/mc/`)

| ReqID (genID) | Generator / process | Period, SW | Rad | Official formats; v6dif read | Events (DB); L (nb⁻¹) | Scale status | Verdict | Rationale |
|---|---|---|---|---|---|---|---|---|
| 9012 (4489) | RAPGAP 3.1034 POMUDS, elastic uds diffractive, Q²>5 | 06_07e+, SW 84 | RAD | DST 240 + HAT/MODS 24+24; read HAT+MODS+ODS | 1,199,996; 52,273.3 (LUM:17) | V3:2 signed | **KEEP** (nominal elastic signal and response) | SW-matched, ODS, unweighted. The "Ee 27.5" DB label is cosmetic: s_gen gives 27.601 GeV (`D/docs/research/revival_assessment_2026-10-27.md:180`) [V]. A single truth stream serves all 4 periods, with MC stat 16% in D2 (`D/docs/t0/t0_yield_prediction_2026-10-18.md:58-66`). The "Fit B" label is not in the DB comment [P]. |
| 9024 (4843) / 9033 (4842) | RAPGAP31 elastic charm, e+ / e− | 06_07e+ / 04_05_e−, SW 84 | RAD | DST 300/200; ODS read | 2,999,998 / 1,999,998; 716,371 / 477,581 | V3:3-4 | **KEEP** | Small charm addition. Gen 4842 is an event-exact prefix of 4843, so the two are correlated (`D/docs/mc/charm_sampling_adjudication_2026-10-17.md:49-52`). |
| 9155 / 9157 / 9158 (6191/6193/6194) | RAPGAP 3.2 PDISS uds "à la DIFFVM", pT_hat>3, Q²>1, xpom<0.2 | 04_05_e− / 2006_e− / 06_07e+, SW 84 | RAD | DST 800/400/800; ODS read | 7,999,800 / 3,999,900 / 7,999,800; ≈1.0M / 0.50M / 1.0M | V3:5-7; ×5.3611 on truth-in-cut signal only | **KEEP** | Dissociative signal and M_Y migrations (R11). Caveat [P] (`h1-mc-expert`): the pT_hat>3 generation cut may bias the X topology towards O(α_s) final states, so a shape check is needed (gap MC-7). |
| 9158 as ep0304 | (borrow of 9158) | sim 06_07e+ | RAD | same stream; exposure factor 2 (V3:7) | — | V3:7 | **COND** | Acceptable: same beam (e+, 27.6 × 920) and same lepton charge. The 2003-04 forward-detector conditions differ, so a transfer leg from the 9013/9012 twin pair is needed (gap MC-2). No ep0304 PDISS sample exists (§2, Q2). |
| 6671 (4860) / 6677 (4863) | RAPGAP31 light-flavour Reggeon, Q²>2, xpom<0.15 | 04_05_e− / 06_07e+, SW 70 | RAD | HAT+MODS 25+25 / 40+40; DST only on /acs; read **HAT+MODS, no ODS** | 1,999,920 each; 368,101 / 368,118 (LUM:14,16) | V3:8-9, reported separately | **COND → KEEP** under the cluster-η_max convention | Full SimRec, not generator-only. Cluster η_max and forward vetoes are populated; only `etaMaxVtx08` is −999 (§2, Q1) [V]. The SW 70 sim has no beam tilt and no z-vertex simulation (DB gen 4860 comment) [V], so it gets an SW leg (gap MC-1). |
| 7091 (5058) | DJANGOH 1.4, NC DIS Q²>60, **ARIADNE 4.10 ME + CDM PS** (DB comment) | 06_07e+, SW 72 | RAD | HAT+MODS 201+201; no PNFS DST; read through evls, no ODS | 19,999,998; 4,379,935 (V3:11) | V3:11 signed | **KEEP** (ND alternative, CDM class) | The cascade mode, previously unpinned (`D/docs/mc/nd_generator_leakage_adjudication_2026-10-20.md:93-97`), is now pinned from the DB comment [V-DB]. |
| 7071 (5031) | RAPGAP31 NC DIS Q²>60, file token `…EMINUSP.06…A`; DB comment: **"ARIADNE ME and CDM ps"** | 2006_e−, SW 72 | RAD | HAT+MODS 61+61; read through evls, no ODS | 5,999,999; 1,515,146 (V3:10, re-ruled) | V3:10 | **COND** (CDM-class generator-variant point, not an envelope edge; §3) | Its DB weighting "for E2start ≤ 150 GeV²" (≈1 at Q²>150) must be carried. Mode [P]: a single DB comment, and the generator log returns HTTP 401. See §3. |
| 7094 / 7096 / 7097 (5062/5063/5064) | RAPGAP 3.1038 **ME + PS**, NC DIS Q²>60 | 06_07e+ / 04_05_e− / 2004_e+, SW 72 | RAD | HAT+MODS 121/121/61 pairs; read through evls, no ODS | 11,999,998 / 11,999,998 / 5,999,999; 3.05 / 3.03 / 1.53 ×10⁶ (SC:2-4) | sidecar only, not signed | **KEEP** (ND alternative, ME+PS class) | They are the ME+PS edge of the ND envelope and cover 3 of the 4 periods. Their scale rows must be promoted to a signed table in WP-E (gap MC-4). The 7097 generator files are lost on tape, which does not matter because HAT+MODS exist; regeneration is impossible, though (`h1-mc-expert` E6). |

### 1b. Candidates not in v6dif

| ReqID (genID) | Generator / process | Period, SW | Rad | Formats (verified listing) | Events (DB) | Verdict | Rationale |
|---|---|---|---|---|---|---|---|
| 9013 (4489) | twin of 9012, elastic uds | 03_04_e+, SW 84 | RAD | DST 240 + HAT/MODS 24+24 | 1,199,996 | **KEEP** (condition sample only) | It is event-identical to 9012 on every generator branch (`D/docs/mc/elastic_uds_replacement_search_2026-10-17.md:94`) [V]. Use it only for the ep0304 detector-condition leg and per-period response, never as added statistics. Needs a WP-E v6dif pass. |
| 8329, 8332 / 8335 / 8338, 8341 (4860 / 4860 / 4863) | SW 77 re-simulations of the 6671/6674/6677 generator events | 04_05_e− / 2006_e− / 06_07e+, SW 77 | RAD | HAT+MODS only (8329: 20+20) | 1,999,920 each | **COND** (SW cross-check, or nominal if better) | The same generated events as 6671/6674/6677, so the generator fluctuation cancels. Their LRG-pass ratio against the SW 70 parents per M_X bin measures the SW 70→77 reconstruction and simulation change only: the DB rows give BEAM_TILT=N and ZVERTEX_RUN=N, the same as 6671 [V]. Never sum them with their SW 70 parents. [P] 8329/8332 and 8338/8341 have identical DB rows and file tokens apart from the ReqID; whether each pair is a distinct re-simulation or a duplicate is unresolved, so never sum within a pair. |
| 6674 (4860) | SW 70 twin of 6671 (same generated events), light-flavour Reggeon | 2006_e−, SW 70 | RAD | HAT+MODS 40+40 (80 files); DST on /acs only | 1,999,920 | **COND** (em06-matched Reggeon) | Not in v6dif. V3:8 covers em06 by an exposure borrow of 6671. A v6dif pass on 6674 (or on its SW 77 child 8335) would give the 2006_e− Reggeon sim conditions. The same convention and caveats as 6671 apply. Do not sum it with 6671 (the generated events are identical). |
| 8531 (5719) | RAPGAP31 RAD e− Reggeon, Q²>1, y<0.8, xpom<0.2, weighted ~1/pT² | 04_05_e−, SW 80 | RAD | DST 100 + HAT/MODS 11+11 [V] | 999,800 | **COND** (cross-check only) | Independent generation and ODS-capable, but one period only and small at Q²>150. Weights must be carried. Report it, do not add it, if it is consistent. |
| 8221 (5536) / 7448 (5299) | RAPGAP31 Q²>2, IFPS10 (ARIADNE CDM; [P] for 8221 from the file token only, DB comment for 7448), elastic Pomeron | 06_07e+, SW 77 / SW 71 | RAD | HAT+MODS 60+60 each; DST on /acs only | 6.0M each | **COND** (parton-shower variant, elastic only) | The HAT carries `fEtaMaxClus`, `fNFmu`, `fNFts`, `fHfsClusPlugE` and 90,000 entries per chunk (`h1-mc-expert` one-file read) [V]. Analysis-convention η_max and tracks need a v6dif pass on HAT+MODS, as for 6671. Q²>150 yield ≈ 30k generated per sample (≈ 60k combined) [I, `h1-mc-expert` from `D/docs/mc/elastic_uds_replacement_search_2026-10-17.md:66`]. Conditions: a DPDF match to 9012 must be verified, and the effect is taken at hadron level (gap MC-5). |
| 8947-8954 (RAPGAP31 ME+PS) / 8955-8962 (DJANGOH14 ARIADNE 4.12 CDM), "MC13" | NC DIS Q²>60, weighted E2start | all four periods, SW 84 | RAD | DST (8948: 2000 files) + HAT/MODS (8948: 402 files) | ≈ 18.7-20M each (8947: 18,739,999) | **COND** (recommended nominal for both ND envelope edges if WP-E capacity allows) | They add no new model; they are the same two classes as 7094-7097 and 7091. They are SW 84 matched to the signal, ODS-capable, cover every period including ep0304, and give ×several template statistics. Costly to produce (gap MC-4). [P] Whether the MC13 RAPGAP carries no diffractive component needs confirmation by `h1-mc-db` (the DB comment reads plain "NC DIS"). |
| 8240-8271 (5547-5578) | DJANGOH14 **MEPS** (LEPTO-type ME+PS), polarisation parts × Q² slices >60/>100/>1000/>10000 | all four periods, SW 77 | RAD | DST + HAT/MODS (8261: 200 DST, 20+20 OO) | 1,999,998 per slice ReqID | **COND** (independent cross-check of the ME+PS edge; a full edge only if RAPGAP ME+PS is statistics-limited) | A second ME+PS implementation, with a different ME, PDF and remnant treatment from RAPGAP (`h1-mc-expert` E3 follow-up). The MEPS mode comes from the file token only; the DB comment reads "Q2>60/100" [P]. The overlapping Q² slices and polarisation parts need exclusive-range stitching and polarisation-weighted luminosity (`h1-normalization`). |
| 8529 | RAPGAP31 e− elastic uds, SW 80, weighted, y<0.8 | 04_05_e− | RAD | DST 100 | 1.0M | **DROP** (restart) | An e− period-matched elastic alternative (`D/docs/mc/elastic_uds_replacement_search_2026-10-17.md:100`). 9033 already supplies the e− elastic-condition check. |
| 7072 | DJANGOH14 em06, gendump only | 2006_e− | RAD | no v6dif tree | — | **DROP** (detector level) | Particle-level pair only (`D/docs/mc/period_matched_pair_record_2026-10-20.md:20,34`). |
| 7446 / 8222 | CDM light-flavour Reggeon | 06_07e+ / 06_07e+ | RAD | HAT+MODS, no PNFS DST | — | **DROP** | A Reggeon shower variant is not warranted for a subtracted fraction ≤ 0.15 (D4). |
| DIFFVM sets (e.g. 7646, 7730) | DIFFVM vector mesons | — | — | — | — | **DROP** | They do not reach Q²>150 (`D/docs/mc/diffractive_mc_inventory_2026-10-15.md:26-27`), and M_X ≥ 4 GeV removes exclusive VM (R5, R22). |
| Non-radiative diffractive SimRec | — | — | NON-RAD | **none exists** | — | gap MC-6 | Every rapgap31/32 diffractive DIS ReqID is RAD=Y. The only exception, 8604, has a RAD=N flag that contradicts its own `.RAD.` filename token. A scan of 582 OO directories found no NORAD token [V, `h1-mc-db`]. |
| Detector-level Fit A | — | — | — | **none identified** | — | gap MC-5 | There are 0 DB hits for "Fit A" or IPOM=−30. The fit is not stated for 9012 or 6671 [V negative; fit attribution INSUFFICIENT EVIDENCE]. |

## 2. Resolved questions

**Q1. 6671/6677: SW 70 with no ODS (register R24).** The conflict is resolved as a wording
conflict, not a factual one [V].
- The semantics record's "hat-only, no DST" (`D/docs/mc/diffractive_mc_semantics_2026-10-16.md:37-42`)
  means "HAT+MODS only, no DST, so no ODS". It does not mean generator-only.
- 6671/6677 are DB detector-simulation request rows with SW 70 (`oo-4.0.6`, `S39100.R97606`).
- v6dif read their official HAT+MODS with `fLoadODS = false`
  (`D/condor/v6dif_mck_evls/2026-10-18/mc6671/steering/evls_v6dif_rapgap31_6671_em0405_0000.steer`).
- A direct read of one tree (`D/output_v6dif/mc/6671/sidistree/sidistree_v6dif_rapgap31_6671_em0405_0000.root`)
  shows:
  - `ChargedTot` has 5,598 entries and `DiffTot` 3,793.
  - `etaMaxClus08` is always populated (median 3.28).
  - `nFmu12`, `ePlug` and `nFts28` are populated.
  - `etaMaxVtx08 = −999` and `odsLoaded = 0` in every entry.

Verdict (`h1-mc-expert` E1): USABLE-WITH-SYSTEMATIC under the stack-C cluster-η_max convention
(`D/docs/t4/localization_verdict_2026-10-20.md:26`). The "not usable at detector level" statement
holds only for the vertex-η convention.

Residual risk: SW 70 has no beam-tilt and no z-vertex simulation (DB gen 4860), and the forward
LAr edge sits near η_max = 3.2. The z_vtx reweight leg covers the tilt and z-vertex effect, because the SW 77
re-sims carry the same N/N flags [V, DB]. The SW 77 ratio covers the SW change only. The fallback is ±50% on the Reggeon normalisation (≤ 7.5% of the subtracted yield at f_reg ≤ 0.15).

No SW 84 light-flavour Reggeon exists; the SW 84 Reggeon samples are D* charm (9001/9004/9010/9011)
or VFPS (8766-8771) [V].

**Q2. ep0304 PDISS.** No 03_04_e+ PDISS uds request exists in the DB [V].
- All 9 rapgap32 ReqIDs are 04_05_e−, 2006_e− or 06_07e+.
- The only 03_04_e+ proton-dissociation rows are 8983 (beauty), DIFFVM 9324-9342 (Q²<150) and TINTIN
  DVCS.
- Under the 2015 inclusion rule, no saved sample exists.

The 9158 borrow stays. It is COND with the 9013/9012 condition-transfer leg (`h1-mc-expert` E2).

**Q3. Third non-diffractive generator.** No LEPTO, PYTHIA, HERWIG or CASCADE NC DIS sample with
Q² ≥ 5 exists in HERA-II [V, `h1-mc-db`]. The detector-level candidates are DJANGOH14 MEPS
(8240-8271, SW 77, DST on PNFS) and the MC13 SW 84 sets (8947-8962). Neither is a new hadronisation
class (§3).

**Q4. /acs for 8221/7448.** `/acs` is not mounted: `ls /acs` returns "No such file or directory"
[V, lead]. The DB DST paths are `/acs/mc2/rapgap31/07/{8221,7448}/…`, and the PNFS DST field is empty.

HAT+MODS (60+60 each) are on PNFS [V]. Revival §3 item 5, "no ODS and no η_max", overstates the
gap: the HAT carries cluster η_max and the forward counts, and the analysis η_max follows from a
v6dif pass. Only the vertex-corrected η_max needs ODS [V for 8221 chunk 0000; P for 7448 by analogy].

**Q5. Official non-radiative diffractive SimRec.** None exists anywhere official [V negative]. Plan
D9 is confirmed (`h1-mc-expert` E5): C_rad is a generator-level ratio by construction, so a
detector-level non-rad sample is not needed. The standalone RAPGAP 3.305 pair exists with only
10,000 events per mode (`sidis_highQ2/MC/rapgap/README.md:19`). Its outputs are in
`H1ChargedSidis_HighQ2/MC/predictions_raw/rapgap_diff_{nonrad,rad}/`, and its steerings in
`H1ChargedSidis_HighQ2/MC/rapgap/cmnd/`. The plan D9 path `sidis_highQ2/MC/rapgap/` holds docs only.
The usability ruling belongs to WP-I.

**Correction to the inventory.** genID 5296 has a ReqID (7446, CDM Reggeon). This contradicts
inventory §Gaps item 1 [V, `h1-mc-db`].

## 3. ND model classes (bears on R16/R19 and G1(c), G2)

The DB generator comments pin the cascade modes:

| Class | Samples |
|---|---|
| CDM (ARIADNE) | 7091 (DJANGOH14), 8955-8962 (DJANGOH14, SW 84), and **7071 (RAPGAP31) [P]** |
| ME+PS (RAPGAP) | 7094/7096/7097 (SW 72), 8947-8954 (SW 84) |
| ME+PS (LEPTO-type) | DJANGOH14 MEPS 8240-8271 |

`h1-mc-expert` re-read these DB comments in the parsed snapshot and confirmed them. The cascade modes of 7071 and of
MEPS stay [P]: each rests on a single DB comment or a file token.

**Prior envelope.** If the comments hold, "7071 vs 7091" was CDM vs CDM. The two samples differ only
in the hard-process interface, the PDF, the radiative treatment and the tune. That pair has no
ME+PS edge, so it likely understates the ND leakage model spread: CDM and ME+PS differ most in
forward radiation, which is what drives leakage into the gap. Any earlier conclusion that relied on
the envelope bracketing shower models needs re-examination, including the localization and G1(c)
fits that used 7071 and 7091 as the two alternatives.

**Recommended restart envelope:**

| Role | Samples |
|---|---|
| CDM edge | 8955-8962 nominal (SW 84, all four periods, ODS-capable); 7091 as the SW cross-check |
| ME+PS edge | 8947-8954 nominal; 7094/7096/7097 as the already-processed cross-check |
| CDM-class point, not an edge | 7071 (and the 7072 gen twin), testing the hard-process dependence at fixed shower |
| ME+PS cross-check | DJANGOH14 MEPS 8240-8271 |

**Fallback without MC13 production.** The edges are 7091 (CDM) vs 7094/7096/7097 (ME+PS).

**G1 prerequisite.** Pin the cascade modes of 7071 and MEPS with `h1-mc-db` (gap MC-3).


## 4. Gap list (owner and cost per gap)

| ID | Gap | Owner | Cost | Fallback / leg |
|---|---|---|---|---|
| MC-1 | SW 70 Reggeon response (no beam tilt, no z-vertex sim) | WP-E (`h1-naf-operator` v6dif pass on 8329-8341 HAT+MODS); WP-J leg | 3-5 ReqIDs × 2M events, HAT+MODS only, small (6671 tree set is 108 MB) | ±50% Reggeon normalisation; z_vtx reweight on/off |
| MC-2 | No ep0304 PDISS | WP-E (v6dif pass on 9013), WP-F / WP-J leg | 1 ReqID, 24 chunks, like 9012 (66 MB of trees) | 9158 borrow + 9013/9012 transfer + 9155/9157/9158 period spread scaled by the ep0304 luminosity share |
| MC-3 | ND cascade-mode attribution of 7071 and MEPS (a G1(c) prerequisite: the prior envelope may have been CDM vs CDM) | `h1-mc-db` (generator log, when reachable) or `h1-mc-expert` (truth-bank check of the cascade, e.g. ARIADNE dipole status codes in one 7071 file) | one lookup, or a one-file login-node check | treat 7071 as CDM; ME+PS edge = 7094/7096/7097 |
| MC-4 | ND templates: 7094-7097 unsigned; no SW 84 / ODS / four-period ND | WP-E (`h1-normalization` signs the scale rows); optional MC13 8947-8962 production | signing: one record. MC13: ≈ 2 × 4 × 20M events, ≈ 400 HAT+MODS files per ReqID, ≫ 1 Condor-day [I] | 7091 vs 7094/7096/7097 at SW 72 with no ODS (cluster convention) |
| MC-5 | Parton-shower and DPDF model variants (no Fit A, no ODS ARIADNE) | WP-E (v6dif pass on 8221/7448); `h1-mc-expert` to verify the DPDF fit of 8221/7448 vs 9012; WP-J leg | 2 × 6M events, 120 OO files each | gen-level x_IP-flux / β reweighting (WP-J legs already listed); the shower leg taken at hadron level |
| MC-6 | No non-rad diffractive SimRec | WP-I (`h1-radiative-correction`, `h1-mc-expert`) | ≥ 1M-event standalone pair (≤ 1 Condor-day, plan Assumption 3); the existing 10k per mode is insufficient | D9 bound max(\|1−C_rad,est\|, 3%) |
| MC-7 | PDISS pT_hat>3 cut may bias the X shape | `h1-mc-expert` (one-file check of QPM content below the cut in 9155) | one-file login-node check | PDISS shape systematic in WP-J |
| MC-8 | Per-sample tree entry counts are undocumented except 9012 (57,790 DiffTot) | WP-E `h1-data-validator` (entry_counts test) | part of WP-E validation | — |

## 5. `user:` items (Open Q3; none filed, per the supervisor decision)

Each item lists the official request it would need, the justification, and the fallback if it is
not granted. The recommendation for every item is not to request before G1.
- **U1. SW 84 light-flavour Reggeon.** Justification: the SW-matched Reggeon response. Fallback:
  MC-1 (6671/6677 with the 8329-8341 ratio leg and ±50%).
- **U2. ep0304 PDISS uds.** Justification: period coverage for the dissociative signal. Fallback:
  MC-2 (the 9158 borrow with the 9013 transfer leg).
- **U3. Detector-level Fit A or ODS ARIADNE diffractive.** Justification: model variants with the
  vertex-η convention. Fallback: MC-5 (8221/7448 HAT+MODS pass and gen-level reweighting).
- **U4. Non-radiative diffractive SimRec.** Not needed in principle (Q5). Fallback: D9.
- **U5. Restoring access to /acs (6671/6677/8221/7448 DST, hence ODS).** Only needed if the
  vertex-corrected η_max becomes the convention. Fallback: the cluster η_max (stack C). DF-SYS-02
  then stays cluster-substituted for the no-ODS components, a stated limitation.

Production scope additions for WP-E are 9013, 8329-8341, 8221/7448, and optionally MC13. They
need no official request, so they are WP-E plan items, not `user:` items. The cost of MC13 falls
under the Open Q4 campaign envelope.

## 6. Coverage gaps and open risks

- **Generator logs are unreachable.** `https://www-h1.desy.de/imc/logfiles/rapgap31/gen_4860.txt`
  returns TLS failure, and HTTP 401 with `-k`. The CGI page at `/cgi-bin/imc` is a placeholder. All
  fit and cascade attributions therefore rest on DB comments.
- **DB snapshot date.** The snapshot is dated 2019-03-15. Later additions would not appear, which
  is unlikely for HERA-II.
- **HAT `fEtaMaxClus` threshold.** It is not known to be 0.8 GeV [P].
- **Weights.** The 8221/7448 weights at Q²>150 (Weight1 1-6, mean 2.75) and the 8531 1/pT²
  weights are unverified.
- **7097 period label.** The DB period is "2004_e+", not "03_04_e+"; the luminosity coverage of
  ep0304 by 7097 must be checked in WP-E.
- **9012 and 6671 fit.** The DPDF fit of 9012 and 6671 is INSUFFICIENT EVIDENCE.
- **DB path discrepancy for 9012.** The steering reads `mc2/oo-4.0` while the DB `ooPATH` says
  `mc/oo-4.0`. Both directories exist (`h1-mc-db`).
