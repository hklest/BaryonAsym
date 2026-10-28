# SHERPA3 weighting / shape-bias study — primary-artifact resolution (2026-10-20)

Read-only login-node study. No Condor jobs launched, no analysis files edited.
Scope: resolve, from primary artifacts, (1) whether the released 490-bin SHERPA3
prediction histograms carry event-weight information, (2) whether unit-count
normalization explains the Q2-dependent MC/data ratio pattern, (3) the correct
rebuild path, (4) sigma_gen.

## Executive verdict

1. **The released 490-bin histograms ARE weighted in the existing YODA files.**
   `sumW != numEntries` in every one of them. The single-argument `fill()` calls in
   the Rivet routine are auto-weighted by Rivet 3 (event weight applied by the
   framework); the claim "fills unit weights, sumW == numEntries" in the 2026-10-20
   consensus document is wrong. The weight was destroyed **in the CSV producer**
   (`numEntries` column read, `sumW`/`sumW2` columns in the same rows discarded),
   not at fill time — and it is fully recoverable from the same files.
2. **The weighting artifact fully explains the released Q2-dependent ratio
   pattern.** Per-cell mis-scaling factors run 0.55 (S0) -> 1.0 (S1) -> 2.2 (S2)
   -> 6.4 (S3). Dividing them (and the x1.09 seed double-count) out of the
   released MC/data ratios collapses a x10.8 dynamic range into a flat 0.84-1.07
   band with no residual monotonic Q2 trend beyond ~15%.
3. **Rebuild path: existing-YODA sumW rebuild.** No Rivet re-run, no `.cc` change
   needed. Spec in Sec. 6; demonstrated end-to-end on the 100k base
   (corrected total 25.371834 nb vs released unit-count 26.498815 nb).
4. **sigma_gen:** do not keep 4.48721 nb blindly. The sample-internal estimator is
   `sumW/sum(NTrials)` = 4434.41 +- 46.73 pb for the 100k base (exactly the
   `/RAW/_XSEC`); per-seed values spread 4377-4585 pb. Use the full-10M
   aggregation (expected ~4.43-4.45 nb) or, cleaner, the self-normalizing form
   `sigma_bin = sumW_bin / sum(NTrials)` in which sigma_gen cancels.

---

## 1. What the Rivet routine actually fills (source + empirical semantics)

Routine: `H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/H1_2026_I_CHARGED_SIDIS_HIGHQ2.cc`

All histogram fills are single-argument: `_h_Q2->fill(Q2)` (L117),
`hs.z->fill(z)` etc. (L307-310), `hs[cellIdx]->fill(localBin + 0.5)` (L303),
`_s_cutflow->fill(...)` (L64/81/86/113/128), `_h_mult_nch->fill(n)` (L157),
`_nDIS += 1.0` (L115).

**Rivet 3 semantics (verified empirically, see table below): single-argument
`fill()` on a booked `Histo1DPtr` is weighted by the active event weight.** The
framework's wrapped-pointer layer applies the current weight-stream weight to
every fill; an explicit weight argument would only multiply it. The three prior
critics (consensus doc lines 39-40, 63-64) assumed `fill(x)` means weight 1.0 —
that is the raw YODA-layer default, not the Rivet-analysis-layer behavior, and
the YODA data refutes it directly:

| Object (seed910001, 10k events) | sumW | numEntries | Interpretation |
|---|---|---|---|
| `/RAW/_EVTCOUNT` (Rivet built-in, per-event weight) | 6.891424e+08 | 10,000 | mean weight 68,914.2 |
| `/RAW/.../TMP/cutflow` bin 1 (after beam check, `_s_cutflow->fill(1.0)`) | 6.8914e+08 | 10,000 | **identical to _EVTCOUNT sumW -> fill() carries event weight** |
| `/RAW/.../TMP/cutflow` bins 4-5 (after all DIS cuts) | 3.5967e+08 | 5,500 | 55.0% of events pass DIS cuts (y-cut dominant) |
| `/RAW/.../DIS/Q2` total | 3.596661e+08 | 5,500 | weighted Q2 spectrum; numEntries = event counts |
| `/RAW/.../Z005/hplus/S0_x0/yield_zpT_flat` bin 1 | 3.005771e+08 | 2,674 | mean weight per hadron fill 112,407 |
| `/RAW/.../Z005/hplus/S0_x0/yield_zpT_flat` bin 15 | **-6.011296e+05** | 14 | negative-weight (MC@NLO RS) dominated bin |

Additional structural proof: the default-path `ESTIMATE1D` version of each Z005
histogram stores `value = sumW` and `error = sqrt(sumW2)`
(e.g. 3.005771e+08 +- 1.611904e+07 = sqrt(2.598233e+14)) — standard weighted-
histogram statistics. If fills were unit-weight, value would equal numEntries.

### Answers to the specific questions

- **(a) RAW `_EVTCOUNT`/`_XSEC` blocks:** weighted. `_EVTCOUNT` is a Counter with
  sumW = sum of event weights, numEntries = N events. `_XSEC` (Estimate0D) =
  sumW/sum(NTrials) per weight stream.
- **(b) per-Q2 / DIS RAW histograms:** weighted. `DIS/Q2`, `DIS/xBj`, `DIS/y`,
  `DIS/W`, `DIS/nChargedHadrons`, `MULT/*`, `Z005/*` all carry sumW = weighted
  content with unit numEntries. The weighted `DIS/Q2` is the direct source of
  the mean-weight-vs-Q2 profile (Sec. 3).
- **(c) released flat z005 490-bin histograms:** `sumW != numEntries` — they are
  weighted, and `sumW2` is present per bin. **Decisive answer: the existing YODA
  files DO contain per-bin weighted information for the released 490-bin
  histograms.** Weight information was destroyed only in the CSV conversion:

  - `absolute_unitcount_xfpos/producer/regen_unitcount_xfpos_490rows.py`,
    `collect_numentries_by_path()`: `values.append(max(0.0, float(parts[4])))`
    — reads column 5 (`numEntries`) of the
    `/RAW/H1_2026_I_CHARGED_SIDIS_HIGHQ2/Z005/{charge}/{cell}/yield_zpT_flat`
    HISTO1D rows; columns 1-2 (`sumW`, `sumW2`) sit in the same rows, discarded.
  - `sherpa3_10M_unitcount_xfpos/producer/run_sherpa3_10M_correctednorm.py`
    topup: `value = row[-1]` (= numEntries), again with negative-clamping.

Both scripts feed `sigma_bin = sigma_gen * numEntries / N_generated`.

## 2. YODA inventory: weight streams, and which weight Rivet consumed

Production ran Sherpa 3.0.3 `EVENT_GENERATION_MODE: Weighted` with
`HEPMC_USE_NAMED_WEIGHTS: true`; Sherpa's internal Rivet interface ran Rivet in
**multi-weight mode with 18 weight streams**: default (nominal) + 7 full
(ME+PS) scale variations `[MUR=..__MUF=..__LHAPDF=261000]` + 7 ME-only
variations `[ME.MUR=..]` + `EXTRA__MEWeight`, `EXTRA__NTrials`,
`EXTRA__WeightNormalisation`. Each stream gets its own copy of every analysis
object (suffix in brackets) plus `/RAW/` copies.

- The "~703-object YODA" of the prior review = this 18-stream structure
  (pre-Z005 smoke 100k file: 703 objects; production seeds: 1,207 objects incl.
  504 Z005 objects = 14 histograms x 2 path classes x 18 streams).
- **Which weight was consumed:** the default (unsuffixed) objects = the nominal
  Sherpa weight stream. Proof: `/RAW/_EVTCOUNT` sumW is bit-identical to
  `/RAW/_EVTCOUNT[MUR=1__MUF=1__LHAPDF=261000]` (6.918515e+09 in the smoke 100k;
  6.891424e+08 in seed910001), while ME-only streams differ (+0.5-1.5%).
- **No x18 fill-multiplication contamination:** each stream fills its own copy;
  the default-stream `numEntries` are plain fill counts. Cross-check:
  100k base Z005 numEntries total 590,541 -> 5.905 fills/event; 10M dedup CSV
  total 59,210,379 -> 5.921 fills/event (statistically consistent).
- **Weight algebra (from the EXTRA streams, 100k base):**
  `Weight = MEWeight x WeightNormalisation / NTrials`, with
  `WeightNormalisation` constant = 60,448.0 per event, mean `NTrials` = 15.52,
  mean `MEWeight` = 20.0 (RMS 239 — RS events negative).
  Identity verified exactly: `mean(Weight) = sigma_pb x mean(NTrials)`
  (4434.409 x 15.519 = 68,817.6 = sumW/N), i.e.
  `sum(_EVTCOUNT sumW) = sigma(pb) x sum(NTrials)`, and Rivet's `_XSEC` =
  `sumW/sum(NTrials)`.
- **Weight spread:** 100k base mean weight 68,817.6, RMS 229,996 (CV ~3.3);
  10k seeds 68,914 / 71,941 / 70,787. Individual weights reach millions and go
  negative (MC@NLO real-subtraction events).

Stream-by-stream `_EVTCOUNT` (smoke 100k, sumW / meanW over 100,000 events):
default 6.918515e+09 / 69,185; full variations 6.786e+09-7.171e+09 /
67,863-71,706; ME-only 7.013e+09-7.077e+09 / 70,130-70,767. `_XSEC` per stream:
default 4435.27 +- 44.66 pb; full variations 4379-4597 pb; ME-only 4496-4537 pb.

## 3. Mean weight vs Q2 — verification of the disputed claim

From the weighted `DIS/Q2` histogram (sumW/numEntries per Q2 bin), 100k base
`run_reuse_p_pattern_gcc14_100k_z005_xfpos_20260629` (the actual base of the
released chain):

| Q2 bin [GeV^2] | mean event weight | Q2 bin | mean event weight |
|---|---|---|---|
| 150-398 | **113,933** | 7,346-7,594 | 3,803 |
| 398-646 | 59,473 | 9,827-10,075 | 810 |
| 646-894 | 38,334 | 12,308-12,556 | 4,022 |
| 894-1,142 | 30,521 | 14,789-15,038 | **1,193** |
| 2,383-2,631 | 13,643 | 17,271-17,519 | 436 |
| 4,864-5,112 | 8,681 | 19,752-20,000 | 181 |

Global mean weight 68,817.6 (all events); DIS-cut-passing events 65,124.5
(54,958/100,000 = 55.0% pass).

**Verdict on the adversarial critic's numbers:** the "~117,632 at Q2 150-398"
is verified in magnitude (113,933 here; 115,521 in seed910001; the exact value
presumably came from the older smoke YODA). The "~1,200-1,500" high-Q2 value is
real but sits at Q2 ~12,000-15,000, not Q2 ~5,000 (where the mean weight is
~8,700). The qualitative claim — a two-orders-of-magnitude (actually ~x630)
fall of mean weight with Q2 — is **correct**. The opposing "subdominant <=8%"
argument only holds at the integrated level (|RS| cross-section share 186.67 pb
of ~4,430-4,523 pb, confirmed in the seed910001 generator log: RS channels
-104.36 pb and -82.31 pb); it does not bound the per-bin shape effect, which is
governed by the weight-vs-Q2 correlation, not the integrated RS share.

## 4. Shape-bias mechanism, quantified per release cell

Unit counting computes `sigma_bin ~ numEntries_bin`; the correct weighted value
is `sigma_bin ~ sumW_bin`. Therefore

```
released/correct  =  A_bin  =  <w>_all / <w>_bin ,
<w>_all = sumW_EVTCOUNT / N_gen  (= 68,817.6 for the 100k base)
```

`<w>_bin` measured directly from the released Z005 histograms
(sumW/numEntries aggregated per cell, 100k base; pos/neg):

| Cell | <w>_cell (pos/neg) | **A = released/correct** | seeds 910001/910002/910990 (pooled charges) |
|---|---|---|---|
| S0_x0 | 125,661 / 126,404 | **0.548 / 0.544** | 0.629 / 0.500 / 0.555 |
| S0_x1 | 123,125 / 123,966 | **0.559 / 0.555** | 0.488 / 0.552 / 0.534 |
| S1_x0 | 65,692 / 66,399 | **0.954 / 0.965** | 1.150 / 1.135 / 0.968 |
| S1_x1 | 63,740 / 63,647 | **0.926 / 0.925** | 1.184 / 1.013 / 1.057 |
| S2_x0 | 30,614 / 30,559 | **2.248 / 2.252** | 2.327 / 2.244 / 2.050 |
| S2_x1 | 32,058 / 31,600 | **2.147 / 2.178** | 2.147 / 2.172 / 3.139 |
| S3_x0 | 10,881 / 10,485 | **6.325 / 6.564** | 6.495 / 8.149 / 7.844 |

Pattern stable across independent samples. Unit counting **understates S0 by
x0.55 and overstates S3 by x6.4-6.6**, with S1 essentially unbiased and S2
overstated x2.2.

### Predicted artifact vs observed released MC/data pattern

Released curve = `data_vs_mc_z005_20260704_10M_rivet_unitcount_xfpos_normfix_clean`
(SHERPA3 tag; total 28.9618 nb, includes the x1.09 double-count). Integrated
per-cell MC/data ratios (R_obs), the predicted artifact (A x 1.09), and the
corrected ratio R_corr = R_obs/(A x 1.09):

| Cell (pos/neg) | R_obs | A x 1.09 | **R_corr** |
|---|---|---|---|
| S0_x0 | 0.606 / 0.605 | 0.597 / 0.593 | **1.016 / 1.020** |
| S0_x1 | 0.556 / 0.558 | 0.609 / 0.605 | **0.912 / 0.922** |
| S1_x0 | 1.110 / 1.110 | 1.040 / 1.052 | **1.068 / 1.056** |
| S1_x1 | 1.022 / 1.037 | 1.010 / 1.009 | **1.013 / 1.028** |
| S2_x0 | 2.231 / 2.253 | 2.450 / 2.454 | **0.911 / 0.918** |
| S2_x1 | 1.998 / 2.061 | 2.340 / 2.374 | **0.854 / 0.868** |
| S3_x0 | 5.813 / 6.005 | 6.894 / 7.152 | **0.843 / 0.840** |

The released pattern's x10.8 dynamic range (0.556 -> 6.005) collapses to
**0.84-1.07, flat in Q2** — no residual monotonic trend beyond ~15%. Per-bin
medians behave the same way (e.g. S3_x0 pos median 5.857 -> corrected ~0.85;
S0 medians 0.585-0.617 -> corrected ~0.99-1.03). The 2026-10-13 5-generator
package (data side NoZ photon-Born; its table stores per-bin densities, so
per-bin ratios are used) gives the same conclusion: observed S0 medians
0.63-0.68 rising to S3 medians 6.7-8.3; corrected medians **0.97-1.24, flat**.

**Verdict: the weighting artifact (plus the known x1.09 double-count) fully
explains the observed Q2-dependent ratio pattern.** The "SHERPA3 under-predicts
low-Q2 / over-predicts high-Q2" narrative is an artifact signature, not
physics. The residual flat offset is a level-convention matter: the 2026-07-04
package's data side uses the superseded L = 297,657.8 nb^-1 (its 26.7039 nb
total = 22.6457 nb x 1.17915); at matched full-Born level the consensus +3%
statement stands, and the mild residual high-Q2 deficit (~0.84-0.92 at
S2/S3 in these conventions) is within the combined effect of the 100k-based A
applied to the 10M curve (~±3% per cell) and genuine generator shape
differences now visible at the ~15% level.

### Negative-weight bins and statistical errors

- 100k base: **4/490 bins with sumW < 0** (weighted truth negative, released
  unit-count CSV reports them as positive counts); seed910001: **18/490**
  (listed: hplus S0_x0 bins 10/15, S0_x1 bin 5, S1_x1 bins 5/15/25, S2_x0 bin 4,
  S2_x1 bin 24; hminus S1_x0 bin 25, S1_x1 bin 24, S2_x0 bins 4/5/20/27, S2_x1
  bins 4/24, S3_x0 bins 15/19). Correct predictions are negative in these bins;
  any rebuild must not clamp them.
- Weighted statistical errors sqrt(sumW2) are ~3.3x the naive sqrt(N) the
  released CSV used (100k base: 0.434% vs 0.130% integrated) — the MC@NLO
  weight spread reduces effective statistics and must be propagated via sumW2.

### Integrated-level effect

100k base: sumW-based total = 4.48721 x (3.891123e10 / 6.881760e9) =
**25.371834 nb** vs unit-count 26.498815 nb (-4.25%). The integrated shift is
modest because the S0 understatement and S2/S3 overstatement compensate; the
shape effect is not. Estimated corrected 10M dedup total:
26.568940 x 0.9575 ~ **25.44 nb** (to be computed exactly in the rebuild).

## 5. Demonstration rebuild (existing files only, task 4a)

From the 100k base YODA alone, per bin:
`sigma_bin(sumW) = 4.48721 x sumW_bin / 6.881760e9`,
`stat_bin = 4.48721 x sqrt(sumW2_bin) / 6.881760e9`, no clamping.
Cross-check: parsed `numEntries` reproduce all 490 released-CSV values exactly
(0 mismatches). Per-cell integrated comparison (sumW-based / unit-count):

| Cell (pos) | unit [nb] | sumW [nb] | ratio | | Cell (pos) | unit | sumW | ratio |
|---|---|---|---|---|---|---|---|---|
| S0_x0 | 2.313964 | 4.225316 | 1.826 | | S2_x0 | 1.253996 | 0.557845 | 0.445 |
| S0_x1 | 1.943859 | 3.477846 | 1.789 | | S2_x1 | 1.008141 | 0.469628 | 0.466 |
| S1_x0 | 2.208784 | 2.108455 | 0.953 | | S3_x0 | 3.070059 | 0.485407 | 0.158 |
| S1_x1 | 1.585960 | 1.468933 | 0.926 | | TOTAL | 26.498815 | 25.371834 | 0.9575 |

(neg charges analogous: 1.837, 1.801, 1.037, 1.081, 0.444, 0.459, 0.152.)
Full 490-row demonstration CSV with per-bin numEntries/sumW/sumW2/sigma_unit/
sigma_sumw/stat_sumw/ratio: `/tmp/opencode/rebuild_100k_sumw_vs_unitcount.csv`
(scratch; regenerate with the spec below).

## 6. Rebuild spec — sumW-based 490-row CSV from EXISTING YODAs (Condor, not launched)

**No Rivet re-run and no routine change are required.** The `.cc` is already
weighted (Sec. 1); the defect is confined to the two CSV producer scripts.

Inputs (991 files, read-only):
- Base (1): `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/sherpa3_mepstest_20260628/rivet_smoke_20260629/work/run_reuse_p_pattern_gcc14_100k_z005_xfpos_20260629/Analysis.yoda.gz`
- Seeds (990): `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/sherpa_rivet_z005_xfpos_to1M_20260703/outputs/seed910001_n10000/Analysis.yoda.gz` ... `seed910990_n10000/` — the dedup set (all 990; assert against the dedup manifest, and assert the set is disjoint from any reference CSV input).
- Exclusions: plus4M campaign seeds 950001-950400; the pre-Z005 smoke 100k YODA at `data_vs_release_7col_20260629/sherpa/Analysis.yoda` (contains NO Z005 objects — it predates the 2026-07-01 routine revision and must not be substituted).

Per file, parse the `YODA_HISTO1D_V3` blocks at
`/RAW/H1_2026_I_CHARGED_SIDIS_HIGHQ2/Z005/{hplus,hminus}/{S0_x0,S0_x1,S1_x0,S1_x1,S2_x0,S2_x1,S3_x0}/yield_zpT_flat`
(default stream, unsuffixed): physical bins are rows[1:-1] (first row =
underflow, last = overflow; 35 bins; columns = sumW, sumW2, sumW(A1), sumW2(A1),
numEntries). Also parse `/RAW/_EVTCOUNT` (Counter: first numeric field = sumW,
third = numEntries; assert numEntries == 100000 base / 10000 seeds) and
`/RAW/_EVTCOUNT[EXTRA__NTrials]` (Counter sumW = sum of NTrials).

Aggregation over files (independent samples — sum linearly):
```
S_bin    = sum_files sumW_bin          (per 490 bins)
S2_bin   = sum_files sumW2_bin
W_evt    = sum_files sumW_EVTCOUNT
T_tot    = sum_files sum(NTrials)

sigma_bin [pb] = sigma_gen_pb * S_bin / W_evt        # sigma_gen_pb = W_evt / T_tot makes this
                                                     # exactly  sigma_bin = S_bin / T_tot
stat_bin [pb]  = sigma_gen_pb * sqrt(S2_bin) / W_evt
```
Do NOT clamp negative bins (MC@NLO RS content is physical). Keep numEntries
columns for continuity and assert `sum(numEntries) == 59,210,379` (dedup CSV
total) as a chain check. Expected total ~25.44 nb; expected per-cell scale vs
the dedup released curve: S0 x~1.8, S1 x~1.0, S2 x~0.45, S3 x~0.155.

Optional at zero extra cost: the 14 suffixed variation-stream copies
(`[MUR=..__MUF=..__LHAPDF=261000]` and `[ME.MUR=..]`, plus `EXTRA__*`) give the
7 full + 7 ME-only scale-variation envelopes for the same 490 bins by running
the same aggregation on each stream.

Execution: 991 gz files (~1.3 MB each) — multi-file processing, so per
login-node policy this goes to Condor (single-core loop ~40-60 min, or shard by
seed). Nothing was launched in this study.

## 7. sigma_gen

| Source | sigma |
|---|---|
| Hardcoded in producers | 4487.21 pb (4.48721 nb) |
| 100k base `/RAW/_XSEC` (= sumW/sum NTrials = 6.881760e9/1.5519e6) | **4434.41 +- 46.73 pb** |
| seed910001 `/RAW/_XSEC` | 4377.34 +- 126.81 pb |
| seed910002 / seed910990 `/RAW/_XSEC` | 4584.91 / 4489.96 pb |
| seed950001 generator log (plus4M campaign, excluded) | 4523.31 +- 121.58 pb |
| seed910001 log, process level | 2_2 BVI +4402.12; 2_3 RS -104.36; 2_3 BVI +1929.41; 2_4 RS -82.31 pb |

The hardcoded 4.48721 nb is +1.19% above the 100k sample's own estimate and
sits inside the per-run spread (±2.5%), but its provenance is untracked.
**Recommendation:** in the corrected CSV use the self-normalizing form
`sigma_bin = S_bin / T_tot` (sigma_gen cancels), or set
`sigma_gen = W_evt / T_tot` aggregated over the full 10M (expected
~4.43-4.45 nb; compute in the rebuild job). Quote 4.48721 nb only as a
cross-check, flagged as +1.2% vs the sample-internal estimator.

## 8. Corrections to the 2026-10-20 consensus document

- "Rivet routine fills unit weights; released z005 hists have sumW ==
  numEntries, so per-bin event-weight information is destroyed at CSV build"
  (lines 39-40) — **wrong**. Fills are auto-weighted; sumW != numEntries;
  information was dropped by the CSV producer reading the numEntries column,
  and is recoverable from the same YODA files.
- The README wording "raw weighted hadron counts" (flagged as a doc bug,
  line 63-64) — **the README was right**; the code review that called it a bug
  mis-read Rivet 3 fill semantics.
- "Resolution requires a weighted-fill Rivet variant + sumW-based CSV; no
  sumW-based per-bin table exists for any SHERPA sample today" (lines 50-51) —
  the first half is unnecessary: no Rivet re-run is needed; the second half
  remains true until the Sec. 6 aggregation runs.
- The adversarial critic's mean-weight magnitudes are verified (113,933 at
  Q2 150-398 in the released-chain 100k base; claimed 117,632); the high-Q2
  value ~1,200-1,500 corresponds to Q2 ~12-15k, not Q2 ~5k. The implementation
  critic's "<=8%" is an integrated-level statement only; the shape effect is
  x0.55-x6.6 per cell and fully explains the released pattern.

## 9. Evidence index (inspected read-only)

- Routine: `H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/H1_2026_I_CHARGED_SIDIS_HIGHQ2.cc`
- Card: `H1ChargedSidis_HighQ2/MC/sherpa3_mepstest_20260628/rivet_smoke_20260629/work/proc_reuse_p_pattern_20260629/Sherpa.yaml`
- Pre-Z005 smoke 100k YODA (703 objects, 18 streams):
  `H1ChargedSidis_HighQ2/MC/rivet_h1_2026_sidis_smoke_2026-06-29/data_vs_release_7col_20260629/sherpa/Analysis.yoda`
- Released-chain 100k base Z005 YODA (1,207 objects):
  `H1ChargedSidis_HighQ2/MC/sherpa3_mepstest_20260628/rivet_smoke_20260629/work/run_reuse_p_pattern_gcc14_100k_z005_xfpos_20260629/Analysis.yoda.gz`
- Seeds: `H1ChargedSidis_HighQ2/MC/sherpa_rivet_z005_xfpos_to1M_20260703/outputs/seed910001_n10000/Analysis.yoda.gz` (+910002, +910990), and `seed910001_n10000.sherpa.log`
- Producers: `data_vs_release_7col_20260629/absolute_unitcount_xfpos/producer/regen_unitcount_xfpos_490rows.py`; `data_vs_release_7col_20260629/sherpa3_10M_unitcount_xfpos/producer/run_sherpa3_10M_correctednorm.py`
- Released CSVs: `absolute_unitcount_xfpos/producer/prediction_sherpa3_unitcount_xfpos_490rows.csv` (100k; reproduced 490/490 numEntries); `sherpa3_10M_unitcount_xfpos/producer/prediction_sherpa3_unitcount_xfpos_10M_correctednorm_490rows_dedup_2026-10-20.csv` (total 26.568940 nb, numEntries 59,210,379 — both verified)
- Ratio tables: `H1ChargedSidis_HighQ2/MC/plots/data_vs_mc_z005_20260704_10M_rivet_unitcount_xfpos_normfix_clean/data_vs_mc_z005_ratio_table.csv` (integrated values; data side at superseded L=297,657.8 nb^-1) and `sidis_highQ2/plots/mc_predictions/data_vs_mc_z005_5generator_born_releasebin_2026-10-13/data_vs_mc_z005_ratio_table.csv` (density convention; per-bin ratios used)
- Scratch: `/tmp/opencode/rebuild_100k_sumw_vs_unitcount.csv` (490-row demonstration)

Approximations stated: per-cell A factors from the 100k base applied to the
10M released curve (per-cell A stable to ~±3% at 100k statistics; 10k seeds
scatter more); per-bin A varies within cells (sparse bins fluctuate strongly,
e.g. negative-weight-dominated bins) — the rebuild of Sec. 6 is per-bin exact.
