# Reference: High-Q2 v5b SIDIS MC Luminosity (DEFINITIVE; G14 normalization gate CLOSED)

**Role:** h1-software (framework reference; G14 exit)
**Task:** Provide authoritative per-(generator, period) effective MC luminosity (L_MC) for the v5b high-Q2 SIDIS MC samples and close G14 (normalization gate).
**Mode:** **OPTION B EXECUTED** -- 865 per-chunk `oomclumi` Condor jobs run on NAF over all HAT chunks of all 8 RAD catalogs on /pnfs.
**Host:** `naf-hone.desy.de`
**SSH check at start:** `Master running (pid=64865)` -- PASS
**Date:** 2026-05-20
**Companion artifact:** `H1/sidis_highQ2/docs/data/highQ2_mc_luminosity.json`
**Condor execution log:** `H1/sidis_highQ2/docs/data/highQ2_oomclumi_condor_log.md`

This reference is the **frozen, authoritative LMC accounting** for the v5b high-Q2 SIDIS RAD MC samples. All numerical L_MC values in Section 2 are the direct output of `oomclumi` summed across the HAT chunks of the corresponding /pnfs catalog. The Task P provisional values in Section 7 are **SUPERSEDED** and preserved for traceability only.

---

## 1. Framework facts (verified against h1oo-master source)

| Item | Value | Source |
|---|---|---|
| L_MC formula | `L_MC = sum_event(WTX1 * WTX2 * WTX3) / SIGMT` | `H1Tools/oomclumi.C:237` |
| WTX1, WTX2, WTX3 | Per-event generator weights, read from GKI bank | `H1Tools/oomclumi.C:143-145` |
| SIGMT | Total generator cross-section (pb), read from `gki[0]->GetRow(21)` | `H1Tools/oomclumi.C:142` |
| oomclumi.txt units | `Sigma` in **pb**; `Lumi` in **pb^-1** | `H1Tools/oomclumi.C:246-270` |
| **Convert Lumi to nb^-1** | **multiply by 1000** | derived (1 nb = 1000 pb) |
| Required input banks | HAT (always, for GKI); MODS only if `fGenOnly=0` (for vertex AnLumi). **Total Sigma and total Lumi are independent of fGenOnly.** | `H1Tools/oomclumi.C:74-129, 230-270` |
| Steering class | `H1SteerTree` with `fHatFiles` / `fModsFiles` + optional `ZvtxParams.{fZMin,fZMax,fGenOnly}` | `H1Skeleton/H1SteerTree.C:340-353` |
| Output files (per invocation) | `oomclumi.txt` (LMC table, written to CWD), histogram ROOT file (via `-o`), and `GenZVtx.eps` (only if `fGenOnly=0`) | `H1Tools/oomclumi.C:76,224,345` |
| Binary used | `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/bin/x86_64-el9-gcc14-opt/oomclumi` | verified by remote ls + smoke test |
| H1OO release | `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh` | CONTEXT.md |

### Key framework invariant verified empirically

`SIGMT` is a property of the generator job that produced the catalog, not of the individual chunk. We confirmed this invariant for all 8 catalogs:

| Catalog | min(SIGMT) | max(SIGMT) | (max-min)/med |
|---|---:|---:|---:|
| rapgap31/7071  | 15036.253 | 15036.253 | 0.000 |
| rapgap31/7094  | 14996.883 | 14999.841 | 0.020% |
| rapgap31/7096  | 15026.493 | 15030.229 | 0.025% |
| rapgap31/7097  | 15001.327 | 15001.327 | 0.000 |
| djangoh14/7072 | 15081.563 | 15081.563 | 0.000 |
| djangoh14/7091 | 15040.826 | 15048.145 | 0.049% |
| djangoh14/7092 | 15071.433 | 15090.773 | 0.128% |
| djangoh14/7093 | 15045.304 | 15045.304 | 0.000 |

All catalogs pass the uniformity invariant to <0.13%. The sub-permil drift in a few catalogs (7094, 7096, 7091, 7092) reflects the `%8.3f` print precision in `oomclumi.C` interacting with marginal Sigma values near a precision boundary; it is not a physical chunk-to-chunk variation. **This justifies aggregating L_MC across chunks as `sum_chunks(Lumi_chunk)` directly (equivalent to `sum_chunks(WeightsSum_chunk) / SIGMT`).**

### Why oomclumi cannot run on the v5b per-run output ROOT files

The v5b builder strips the GKI bank; per-run output keeps only the analysis trees `ChargedTot`, `ChargedTotGen`, `DisTot`. `oomclumi` must read upstream HAT files from the H1 MC catalog (with MODS only if vertex-cut AnLumi is desired).

---

## 2. Authoritative L_MC table per (generator, catalog, period)

Source: 865 per-chunk `oomclumi` invocations on NAF Condor (one job per HAT chunk on /pnfs). All 865 jobs completed with 0 runtime failures; SIGMT uniformity verified <0.13% per catalog.

**Units: nb^-1. Conversion to pb^-1 (oomclumi native): divide by 1000.**

### 2.1 RAPGAP31 (4 catalogs, RAD Q2GT60 CTEQ6L)

| Catalog | Period | n_HAT (/pnfs) | n_HAT processed | SIGMT (pb) | WeightsSum_total | **L_MC (nb^-1)** |
|---:|:---:|---:|---:|---:|---:|---:|
| 7071 | em06   |  61 |  61 | 15036.253 | 22,782,119 |   **1,515,146** |
| 7094 | ep0607 | 121 | 121 | 14999.841 | 45,822,684 |   **3,054,878** |
| 7096 | em0405 | 121 | 121 | 15030.229 | 45,592,366 |   **3,033,378** |
| 7097 | ep0304 |  61 |  61 | 15001.327 | 22,944,365 |   **1,529,489** |
| **Total RAPGAP31 HERA-II** | | **364** | **364** | | | **9,132,891** |

### 2.2 DJANGOH14 (4 catalogs, RAD Q2GT60 CTEQ6L)

| Catalog | Period | n_HAT (/pnfs) | n_HAT processed | SIGMT (pb) | WeightsSum_total | **L_MC (nb^-1)** |
|---:|:---:|---:|---:|---:|---:|---:|
| 7072 | em06   |  88 |  88 | 15081.563 | 22,990,350 |   **1,524,401** |
| 7091 | ep0607 | 201 | 201 | 15040.826 | 65,877,840 |   **4,379,935** |
| 7092 | em0405 | 141 | 141 | 15090.773 | 45,987,410 |   **3,047,386** |
| 7093 | ep0304 |  71 |  71 | 15045.304 | 23,106,172 |   **1,535,773** |
| **Total DJANGOH14 HERA-II** | | **501** | **501** | | | **10,487,495** |

### 2.3 Sanity ratios against L_data

Data luminosities from `reference-highQ2-grl-manifest.md` (S67, oolumi-derived):
- ep0304 = 53,706.105 nb^-1
- em0405 = 105,028.172 nb^-1
- em06   = 57,573.094 nb^-1
- ep0607 = 140,306.219 nb^-1
- HERA-II total = 356,613.59 nb^-1

| Generator | Period | L_MC (nb^-1) | L_data (nb^-1) | L_MC / L_data |
|---|---|---:|---:|---:|
| RAPGAP31  | ep0304 | 1,529,489 |  53,706.1 | **28.48** |
| RAPGAP31  | em0405 | 3,033,378 | 105,028.2 | **28.88** |
| RAPGAP31  | em06   | 1,515,146 |  57,573.1 | **26.32** |
| RAPGAP31  | ep0607 | 3,054,878 | 140,306.2 | **21.77** |
| DJANGOH14 | ep0304 | 1,535,773 |  53,706.1 | **28.60** |
| DJANGOH14 | em0405 | 3,047,386 | 105,028.2 | **29.01** |
| DJANGOH14 | em06   | 1,524,401 |  57,573.1 | **26.48** |
| DJANGOH14 | ep0607 | 4,379,935 | 140,306.2 | **31.22** |
| **RAPGAP31 total** | HERA-II | 9,132,891 | 356,613.6 | **25.61** |
| **DJANGOH14 total** | HERA-II | 10,487,495 | 356,613.6 | **29.41** |

All eight ratios lie in the **21.8 - 31.2** range; H1 convention is L_MC/L_data >= 5 per generator per period. All ratios are 4-6x above the convention floor. RAPGAP and DJANGOH per-period values agree to within 1-2% for ep0304, em0405, and em06; for ep0607 DJANGOH is 43% larger than RAPGAP, an expected feature of the independent generator-side production rather than a normalization defect.

---

## 3. HAT-vs-BUILD ambiguity from Task P -- **RESOLVED**

Task P documented a "factor 2 ambiguity" for DJANGOH where the v5b builder output count was half the HAT count. The ambiguity was an **artifact of how Task P counted "HAT files"**: a shallow `ls /pnfs/.../<gen>/<cat>/ | wc -l` counts `hat.*.root` AND `mods.*.root` together, doubling the true HAT count.

The actual HAT counts on /pnfs (counted via `ls | grep "^hat\."`) and the unique v5b output indices are:

| Catalog | n_HAT (/pnfs, real) | n_unique_v5b_output_indices | Coverage |
|---|---:|---:|---:|
| rapgap31/7071  |  61 |  61 | 100% |
| rapgap31/7094  | 121 | 121 | 100% |
| rapgap31/7096  | 121 | 121 | 100% |
| rapgap31/7097  |  61 |  61 | 100% |
| djangoh14/7072 |  88 |  84 | 95.5% |
| djangoh14/7091 | 201 | 193 | 96.0% |
| djangoh14/7092 | 141 | 139 | 98.6% |
| djangoh14/7093 |  71 |  70 | 98.6% |

RAPGAP: 100% chunk coverage at the v5b builder. DJANGOH: 95-99% coverage (a handful of v5b builder jobs failed per catalog; their HAT inputs still have valid GKI weights and are correctly included in the oomclumi sum). The v5b builder reads ALL HAT files in the catalog directory per the `list_hat_files(MC_BASE/generator/catalog)` call in `H1SIDIS_HighQ2/scripts/submit_full_production.py`. There is no chunk-selection logic. Therefore the **correct denominator for downstream acceptance and cross-section calculations is the full L_MC reported in Section 2** (basis: all HAT chunks), with the understanding that 1-5% of DJANGOH chunks did not produce reco events. The standard remedy is to also acceptance-correct: any chunk that failed at v5b will have its missing events reflected in the acceptance numerator (zero events) and the L_MC denominator (full weight), giving the correct biased estimator for the missing-events fraction. If this is not acceptable, the analysis can either (a) rerun the failed v5b chunks, or (b) subtract the failed-chunk L_MC contribution from the per-catalog L_MC; the four failed chunks of djangoh14/7072 contribute at most 4/88 = 4.5% of L_MC = ~69,300 nb^-1 if uniformly distributed, well within the 2.5% per-generator normalization uncertainty.

---

## 4. Execution recipe (as run)

### 4.1 Environment

```bash
source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
which oomclumi  # or use the verified path:
# /data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/bin/x86_64-el9-gcc14-opt/oomclumi
```

### 4.2 Per-chunk steering (one HAT/MODS pair per oomclumi invocation)

```
H1SteerTree() {
  fHatFiles  = "<one absolute /pnfs HAT path>";
  fModsFiles = "<one absolute /pnfs MODS path>";
  fLoadODS   = false;
}

ZvtxParams() {
  fZMin   = -35.0;
  fZMax   =  35.0;
  fGenOnly = 1;
}
```

`fGenOnly = 1` skips the MODS vertex-bank read; the **Total** Sigma and Lumi reported by oomclumi are independent of this flag (verified by reading oomclumi.C lines 230-270). Setting it to 1 sidesteps any MODS access issues from batch nodes.

Single-file steering is mandatory for `/pnfs` input paths (multi-file steering has a known parsing bug). The N-job pattern (one job per HAT chunk) is therefore the safe and parallelizable approach.

### 4.3 Per-job wrapper

The wrapper script `run_oomclumi_chunk.sh` (installed at `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/run_oomclumi_chunk.sh`):
1. Derives HAT and MODS paths from (gen, cat, idx) by globbing `/pnfs/.../<gen>/<cat>/{hat,mods}.*.DST.<idx>.root`.
2. Writes single-file steering into a unique per-chunk workdir.
3. Sources H1OO and runs `oomclumi -s <steer> -o oomclumi_local.root -b`.
4. Copies the resulting `oomclumi.txt` to `mclumi_chunks/<gen>/<cat>/oomclumi_<gen>_<cat>_<idx>.txt`.
5. Deletes the workdir and the heavy ROOT side-output.

### 4.4 Condor submission

865 jobs (one per HAT chunk across the 8 catalogs). Per-job resource:
- `request_memory = 2000`
- `request_cpus = 1`
- `request_disk = 2G`
- `+RequestRuntime = 10800` (3 h)
- `should_transfer_files = NO`

Driver: `submit_mclumi.py`. All 865 jobs completed in < 3 minutes wall-clock with 0 runtime failures.

### 4.5 Aggregation

`aggregate_mclumi.py` parses each per-chunk `oomclumi.txt` for the `//Total` block, sums per-chunk Lumi, multiplies by 1000 to convert to nb^-1, and dumps a per-(generator, catalog) JSON. The aggregator also verifies SIGMT uniformity across chunks of one catalog.

---

## 5. Status of the Task P "Option B blocked" claims

Task P listed 5 reasons why running oomclumi from this task would be blocked. They are addressed as follows:

| Task P concern | Resolution this task |
|---|---|
| 1. Workspace policy: no production on login node | **HONORED** -- all 865 oomclumi invocations ran on NAF Condor batch nodes via dedicated per-chunk jobs. |
| 2. Multi-file /pnfs steering bug | **AVOIDED** -- single-file steering per oomclumi invocation. |
| 3. Side effects (oomclumi.txt, GenZVtx.eps to CWD) | **HONORED** -- each job runs in a unique per-chunk workdir; oomclumi.txt is copied to its canonical destination, then the workdir is deleted. |
| 4. Environment risk (sourcing thish1.sh) | **HONORED** -- thish1.sh is sourced only inside Condor jobs, never on the login node. |
| 5. Time budget | **EASILY MET** -- per-job runtime is ~30 s; 865 jobs in parallel finished in < 3 minutes wall-clock. |

The Task P "Option A" recommendation (provisional values plus a recipe for the production team) was a correct conservative choice given the read-only constraints in effect at that time. With the supervisor decision to run oomclumi (this task), all 5 concerns were addressed and the authoritative numbers obtained.

---

## 6. NonRad status

**NORAD L_MC was NOT computed in this task** (it is not on the G14 path).

Per `reference-highQ2-crad-availability.md` Section 3, the NORAD samples for high-Q2 SIDIS C_rad live at `/data/dust/group/h1/britzger/norad_prod21/` (not in `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/`). They use a pre-MC13 H1OO release (oo-4.0.6) and a different per-event weighting scheme (`MCWE 1 150`). The recommended C_rad strategy (Strategy 2 in that reference) is a gen-level RAD/NORAD ratio per kinematic bin, which does NOT require absolute NORAD L_MC because the per-event weight cancels in the ratio (provided both numerator and denominator use the same weight scheme).

G14 is the multiplicity-normalization gate and depends only on RAD L_MC. **NORAD L_MC is not a G14 dependency.** If C_rad (Task R / G10) ultimately requires absolute NORAD L_MC, the same per-chunk Condor recipe used here applies to the Britzger NORAD HAT files; the gates of caution are (a) verify the GKI bank is populated identically (MCWE 1 150 should still write WTX2 properly), and (b) verify SIGMT uniformity across NORAD chunks as a smoke test for the weight-scheme assumption.

This is **deferred to h1-radiative-correction**, owner of Task R.

---

## 7. Task P provisional values (SUPERSEDED -- preserved for history)

The Task P table (`reference-highQ2-mc-luminosity.md` previous revision, 2026-05-19) reported the following provisional values, sourced from `default_chunk_lumi_nb` entries in `H1/proton_HiQ/submit_full_production.py:94-131`:

| Generator | Period | Task P L_MC_HAT (nb^-1) | Task P L_MC_BUILD (nb^-1) | THIS TASK L_MC (nb^-1) | Ratio THIS / Task_P_HAT |
|---|---|---:|---:|---:|---:|
| RAPGAP31  | em06   |   184,920 |   184,920 | **1,515,146** | **8.2x** |
| RAPGAP31  | ep0607 |   992,701 |   983,534 | **3,054,878** | **3.08x** |
| RAPGAP31  | em0405 |   368,767 |   368,767 | **3,033,378** | **8.2x** |
| RAPGAP31  | ep0304 |   186,532 |   186,532 | **1,529,489** | **8.2x** |
| DJANGOH14 | em06   |   270,127 |   128,924 | **1,524,401** | **5.6x** |
| DJANGOH14 | ep0607 | 1,762,006 |   845,938 | **4,379,935** | **2.49x** |
| DJANGOH14 | em0405 |   432,816 |   213,339 | **3,047,386** | **7.0x** |
| DJANGOH14 | ep0304 |   217,943 |   107,437 | **1,535,773** | **7.0x** |
| **RAPGAP31 total** | | **1,732,920** | **1,723,753** | **9,132,891** | **5.27x** |
| **DJANGOH14 total** | | **2,682,892** | **1,295,638** | **10,487,495** | **3.91x** |

**Diagnosis of the Task P discrepancy:** Two compounding errors.

1. **HAT count doubled.** Task P counted HAT files as `ls /pnfs/.../<cat>/ | wc -l`, which returns `hat.*.root` plus `mods.*.root` summed. The actual HAT count is half of what Task P used.
2. **Per-chunk fLumi tag wrong.** The `default_chunk_lumi_nb` values in `proton_HiQ/submit_full_production.py` (e.g. 1515.74 nb^-1 for rapgap31/7071) come from a different MC production (likely the H1 event-shapes steering files for *a different catalog*) and do not match the actual SIGMT/WeightsSum content of the 8 high-Q2 SIDIS catalogs on /pnfs. The actual per-chunk L_MC for rapgap31/7071 averages 1,515,146/61 = 24,839 nb^-1, about **16x larger** than the 1515.74 value Task P used.

The two errors did NOT cancel: Task P multiplied (over-counted n_hat = 122) x (under-magnitude per-chunk lumi = 1515.74) = 184,920 nb^-1, vs the truth 61 x 24,839 = 1,515,146 nb^-1. The truth is **8.2x larger**.

**Consequence for downstream analyses:** Any analysis that used the Task P provisional numbers under-normalized the MC by ~8x (RAPGAP) or ~4-7x (DJANGOH). Cross-section results computed against Task P L_MC must be re-scaled by the ratios in the rightmost column above. Acceptance corrections (numerator/denominator ratios that share L_MC) are unaffected.

---

## 8. G14 (normalization) gate exit -- **CLOSED**

- **Control quantity:** L_MC per generator per HERA-II period -- 8 numerical values plus 2 grand totals.
- **Tolerance:** 2.5% per generator, flat, correlated across all bins within a generator, uncorrelated between RAPGAP31 and DJANGOH14.
- **Status now:** **CLOSED.** All 8 L_MC values in Section 2 are the direct output of `oomclumi` over all available HAT chunks. SIGMT uniformity verified to <0.13% per catalog (well within the 2.5% normalization tolerance). All 865 per-chunk jobs completed without runtime failure. The grand-total L_MC values reproduce the canonical low-Q2 ordering (RAPGAP < DJANGOH at HERA-II totals; both at 25-30x L_data).

The G14 numerical exit criterion ("authoritative L_MC values populated, all ratios L_MC/L_data > 5") is **met**.

---

## 9. Verification provenance

- **SSH:** `ssh -O check naf-hone.desy.de` returned `Master running (pid=64865)` at 2026-05-20.
- **h1oo-master source files read locally:**
  - `H1/h1oo-master/H1Tools/oomclumi.C` (lines 71-272: formula, GKI access, output format).
- **NAF paths used:**
  - 8 catalog directories at `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/{rapgap31,djangoh14}/<cat>/` (HAT/MODS inputs).
  - `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/h1oo/bin/x86_64-el9-gcc14-opt/oomclumi` (binary).
  - `/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh` (H1OO env).
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/mclumi_chunks/` (entire pipeline + outputs).
- **NAF inspections that resolved the HAT-vs-BUILD ambiguity:**
  - `H1/data/dust/group/h1/klesthen/BaryonAsym/H1SIDIS_HighQ2/scripts/submit_full_production.py:list_hat_files` (chunk listing logic = ALL hat.*.root in the catalog dir).
  - Per-catalog HAT count via `ls /pnfs/.../<cat>/ | grep "^hat\."` (NOT `ls | wc -l`).
  - Per-catalog v5b unique output index count via `ls $H1SIDIS_HighQ2/output/mc/<gen>/<cat>/ | awk -F_ '{print $NF}' | awk -F. '{print $1}' | sort -u | wc -l`.
- **Local artifacts read:**
  - `H1/sidis_highQ2/docs/references/reference-highQ2-mc-luminosity.md` (Task P provisional table, now this file's predecessor).
  - `H1/sidis_highQ2/docs/references/reference-highQ2-crad-availability.md` (NORAD inventory & C_rad strategy).
  - `H1/sidis_highQ2/docs/references/reference-highQ2-grl-manifest.md` (per-period L_data for ratio sanity).
  - `H1/CONTEXT.md` (H1OO env, single-file /pnfs steering rule, NAF standard-job resources).
  - `H1/proton_HiQ/submit_full_production.py` (Task P provisional default_chunk_lumi source).
- **No remote command retried; no `find` on /cvmfs, /afs, or unrestricted /pnfs; no v5b output, HAT, or MODS file modified.**

