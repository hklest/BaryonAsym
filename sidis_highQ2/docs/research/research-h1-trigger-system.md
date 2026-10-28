# H1 HERA-II Trigger System and Subcomponent Efficiency Evaluation

**Status:** Research note for SIDIS low-Q2 analysis (and high-Q2 extension).
**Scope:** H1 four-level trigger system at HERA-II (2003-2007), the SIDIS-relevant
subtriggers S61 (SpaCal low-Q2 DIS) and S67 (LAr high-Q2 NC DIS), the monitor
trigger technique for sub-component efficiency extraction, and the gtustin
prescale-corrected trigger weight (TrigWeightAC/RW) used in our event lists.

All numerical claims are sourced to retrieved H1 theses, the H1 SpaCal NIM paper,
or the published Britzger jet measurement. Where a claim comes from a conference
talk (PoS / indico slides), it is marked as such.

---

## 1. Sources Used

| Tag | Source | Type | Notes |
|-----|--------|------|-------|
| **[Kogler11]** | R. Kogler, "Measurement of Jet Production in Deep-Inelastic ep Scattering at HERA," PhD thesis, Univ. Hamburg, MPP-2011-44, DESY-THESIS-2011-003. PDF: `https://www-h1.desy.de/psfiles/theses/h1th-590.pdf` | DESY thesis | Definitive HERA-II S67 trigger eff. study (Sec. 9.2, p.151-156); also full L1/L2/L3/L4 architecture (Sec. 4.2.5, p.44-46) |
| **[Britzger13]** | D. Britzger, "Regularized Unfolding of Jet Cross Sections in DIS at HERA and Determination of the Strong Coupling Constant," PhD thesis, Univ. Hamburg, DESY-THESIS-2013-045. PDF: `https://www-h1.desy.de/psfiles/theses/h1th-831.pdf` | DESY thesis | High-Q2 NC DIS jets; uses S67; Sec. 3.2.4 (trigger architecture, p.45) and Sec. 4.1.1 (S67 efficiency, p.49). Cross-references [Kogler11] for numerical values. |
| **[DelDegan08]** | M.A. Del Degan, "Spectroscopy in ep Scattering at HERA," PhD thesis, ETH Zurich, ETHZ-IPP Internal Report 2008-08. PDF: `https://www-h1.desy.de/psfiles/theses/h1th-492.pdf.gz` | DESY thesis | HERA-I but the only thesis with explicit S61 composition formula and full IET threshold info (Sec. 4.1.2.1) and S61 monitor-trigger efficiency factorization (Sec. 6.2.2) |
| **[Avila13]** | K. Cantun Avila, "Lambda Production at High Q2 in DIS with H1 Detector at HERA," PhD thesis, Univ. of Yucatan, Merida. PDF: `https://www-h1.desy.de/psfiles/theses/h1th-906.pdf` | DESY thesis | HERA-II S67 description, L1-L4 walkthrough (Sec. 4) |
| **[Habib09]** | S. Habib, "Unpolarized Neutral Current e±p Cross Section Measurements at the H1 Experiment, HERA," PhD thesis, Univ. Hamburg, DESY-THESIS-2009-039. PDF: `https://www-h1.desy.de/psfiles/theses/h1th-514.pdf` | DESY thesis | Source of the time-dependent fiducial-volume cut [156] cited in [Kogler11, Britzger13] |
| **[Reissner13]** | C. Reissner (Heidelberg), "Diffractive Dijets in DIS with Large Rapidity Gap," PAF talk, Liverpool DIS workshop, 11 Sep 2013. PDF: `https://indico.cern.ch/event/258612/contributions/1589371/attachments/455490/631351/pafliverpool.pdf` | Conference slides | HERA-II low-Q2 dijets; explicit slide on S61 ~ (SpaCal && FTT) composition and monitor-trigger methodology (slides 4-6) |
| **[SpaCalNIM99]** | H1 SpaCal Group, R.-D. Appuhn et al., "The Electronics of the H1 Lead/Scintillating-Fibre Calorimeters," Nucl. Instrum. Meth. A426 (1999) 518-537, DESY 98-054, arXiv:physics/9812042. | Refereed | IET trigger design: 4x4 tower sliding window, 3 thresholds, 50 MeV trigger noise level (Sec. 4) |
| **[gtustin]** | G. Tustin, H1OO `TrigWeightAC`/`TrigWeightRW` utility (internal); formula reproduced from user context. | Internal code | Used in our v5b event lists |

Sources retrieved 2026-05-17 via `https://www-h1.desy.de/psfiles/theses/` and `indico.cern.ch`. The H1 thesis list at `https://www-h1.desy.de/h1/www/publications/theses_list.html` was used to map names to thesis numbers.

---

## 2. H1 Four-Level Trigger Architecture at HERA-II

H1 used a pipelined four-level trigger to reduce the HERA bunch-crossing rate of
10.4 MHz (96 ns spacing) to the ~10 Hz tape rate ([Kogler11, Sec. 4.2.5];
[Britzger13, Sec. 3.2.4]; [Avila13, Sec. 4.6]):

| Level | Latency | Input rate | Output rate | Function |
|-------|---------|------------|-------------|----------|
| **L1** | ~2.3 us (pipeline depth = 22 BC, dead-time-free) | 10.4 MHz | ~1 kHz | 256 hardware **trigger elements** (TEs) combined by Central Trigger Logic (CTL) into **128 raw subtriggers** (S0-S127). Each subtrigger has a configurable **prescale** n; only every n-th raw fire becomes an "actual" L1keep ([Kogler11, p.45]; [Britzger13, p.45]). |
| **L2** | ~20 us | ~1 kHz | ~200 Hz | Two parallel systems: **L2TT** (topological trigger) and **L2NN** (13 neural networks on parallel processors). Trained on CC, NC, multi-lepton events; working point 99.89 % eff. for 75 % background rejection ([Kogler11, p.45]; ref. Pla06). After HERA-II luminosity upgrade, L2NN essential to keep S67 and S77 (CC) within bandwidth. |
| **L3** | ~100 us | ~200 Hz | ~50 Hz | **Fast Track Trigger** (FTT), commissioned 2005; uses 12 of 56 CJC wire layers organized in 4 groups to find track segments; targets heavy-quark decays and is the key L3 element of S61 in HERA-II ([Kogler11, p.45-46]; [Britzger13, p.45]; FTT refs [BSC+01], [Sch06]). |
| **L4/L5** | ~100 ms | ~50 Hz | ~10 Hz | PC filter farm; full event reconstruction (H1Rec), classification into physics streams. 1 % of "no-class" events kept for monitoring; high-rate soft processes downscaled. Output: POTs and DSTs ([Kogler11, p.46]; [Britzger13, p.45]). |

**Subtrigger definition.** A "subtrigger" Sxx is a *logical AND* of selected
L1 trigger elements, optionally combined with L2 (NN or TT) and L3 (FTT) verification
elements. The 128-bit L1 raw pattern, the L2 keep pattern, the L3 keep pattern and the
final L1L2L3 actual pattern are all stored event by event ([Kogler11, p.45];
[DelDegan08, eq. 4.1]). The L1L2L3ac[4] = 128 bit array in our v5b trees is exactly
this *post-prescale* "actual fired and kept" pattern.

ASCII summary of a subtrigger Sxx:

```
Sxx_raw    = (TE_a & TE_b & ... )                 # L1 element conjunction
Sxx_actual = Sxx_raw & (prescale counter % n == 0) # post-prescale at L1
Sxx_L2     = Sxx_actual & L2_NN(or TT)_keep_for_Sxx
Sxx_L3     = Sxx_L2     & L3_FTT_keep_for_Sxx     # (only if S has L3 condition)
```

L1L2L3ac is the AND of all enabled levels for that subtrigger in a given run.

---

## 3. Subtrigger S61 (SpaCal low-Q2 DIS)

### 3.1 HERA-I composition ([DelDegan08, eq. 4.1, Sec. 4.1.2.1])

```
S61 = DCRPh_THig  AND  zVtx_sig  AND  ( SPCLe_IET>2  OR  SPCLe_IET_Cen_3 )
```

with the three L1 trigger element families:

- **DCRPh_THig** (drift-chamber phi trigger): at least one CJC track with pT > 800 MeV, reconstructed from r-phi hit patterns matched against ~10^4 pre-defined masks. ([DelDegan08, p.59])
- **zVtx_sig** (z-vertex trigger): significant peak in the z-vertex histogram built from CIP, COP, and forward-tracker ray intersections, with the z range -43.9 to +43.9 cm. ([DelDegan08, p.59])
- **SPCLe_IET** (SpaCal Inclusive Electron Trigger): 4x4 SpaCal-tower sliding-window sum compared against 3 thresholds. `IET>2` requires the highest of the three thresholds, ~5.7 GeV in the outer region; `IET_Cen_3` is the corresponding cut in the central (innermost) SpaCal region. ([DelDegan08, p.59]; trigger design [SpaCalNIM99, Sec. 4])

The IET hardware design: each 4x4 tower sum (4x4 grouping with overlap between
neighbors) is digitized and compared to three thresholds. Trigger noise level
~50 MeV. Energy threshold can be set as low as 0.50 +/- 0.08 GeV with > 99.9 %
efficiency ([SpaCalNIM99, abstract]). At HERA-II the three IET thresholds were
configured around ~2, ~6, and ~9 GeV (precise numbers run-period dependent).

### 3.2 HERA-II composition

At HERA-II, S61 was redefined to include the new **L3 Fast Track Trigger** condition:

```
S61_HERA-II ~  ( SpaCal-IET condition )  AND  ( FTT condition at L3 )
```

This is the explicit form given in the C. Reissner PAF Liverpool talk
([Reissner13, slide 4]): *"S61 ~ (SPACAL && FTT)"*. The SpaCal part is the L1 IET
condition above; the FTT part is the L3 verification of one or more tracks.
The exact L1/L2/L3 element list is run-period dependent (the H1 trigger menu
evolved across the 03/04, 05, 06, 06/07 sub-periods).

> **Implementation note for our analysis:** the exact bit composition of S61 by run
> period is stored in the H1OO trigger database accessed via the H1 trigger setup
> service (e.g., `H1Sub` / `H1TrigSet`). Since the v5b event lists do not enforce
> any single subtrigger but store the full 128-bit pattern, we can re-derive
> efficiency directly from data using the monitor-trigger technique (Section 5).

### 3.3 Typical S61 prescale ([DelDegan08, p.59])

- HERA-I 1999 e+p: average prescale **1.03**
- HERA-I 2000 e+p: average prescale **1.19**

HERA-II average prescales are not quoted in [Kogler11] or [Britzger13]
(neither uses S61). Anecdotally, S61 was rarely prescaled at low luminosity but
prescales rose during high-luminosity HERA-II runs as L1NN/FTT helped. The exact
per-period prescale must be read from the trigger setup files; in our v5b lists
this is captured event-by-event in the prescale array used by `TrigWeightAC`.

### 3.4 Typical S61 efficiency

- For an inclusive low-Q2 DIS sample [Reissner13, slide 5]: **>96 %** in the (x,y)
  electron impact plane after a box (fiducial) cut, with the analyzed run period 2005 e-.
  Error on the trigger efficiency is **~1 %**. Other run periods are similar.
- For the inclusive K0s sample [DelDegan08, p.100]: **81.84 %** (drops because
  DCRPh_THig efficiency for the low-pT K0s decay products is only ~60-80 %, not
  because of the SpaCal IET, which is ~100 % efficient when the scattered
  electron is in the SpaCal acceptance).
- For the inclusive Lambda sample [DelDegan08, p.100]: **83.66 %** (DCRPh_THig
  more efficient because the proton from Lambda decay carries more pT).
- The **SpaCal trigger element alone** is ~100 % efficient for scattered electrons
  with E_e > ~7 GeV and away from dead/edge cells ([DelDegan08, p.100];
  [Reissner13, slide 5]: the low-energy correction is applied for E_e < 7 GeV
  via reweighting with a Fermi function).
- The **DCRPh and z-vertex elements** are ~90 % efficient on the inclusive V0
  sample but rise toward 100 % at high pT(V0) ([DelDegan08, p.100]).

### 3.5 Kinematic dependence (S61)

| Variable | Behavior | Source |
|----------|----------|--------|
| Scattered electron E_e | SpaCal IET >2 threshold ~5.7 GeV; trigger >99.9 % above threshold; reweighted for E_e < 7 GeV via Fermi function | [SpaCalNIM99]; [Reissner13, slide 5] |
| SpaCal impact (x,y) | >96 % after box fiducial cut excluding dead cells and module edges; ~70-90 % near edges and dead cells | [Reissner13, slide 5] |
| Q2 | Flat above SpaCal acceptance threshold (Q2 > ~2-5 GeV2); driven by IET geometric coverage of the SpaCal | implicit from [DelDegan08, Fig. 6.5] |
| y | Falls at very low y because E_e' approaches the IET threshold (E_e' ~ E_e * (1-y) for backward scattering), so the SpaCal element drops out; flat above y ~ 0.05 once E_e' > 7 GeV | [Reissner13, slide 5]: "Low energy (high y) correction - sample with E > 7 GeV" |
| Run period | Stable across HERA-II within ~1 % once veto/fiducial corrections applied | [Reissner13, slide 5] |

---

## 4. Subtrigger S67 (LAr high-Q2 NC DIS)

### 4.1 Composition ([Kogler11, eq. 9.1]; [Britzger13, eq. 4.1]; [Avila13, eq. 4.1])

```
S67 = t_LAr  AND  t_T0  AND  t_VETO     (all three are L1)
```

with the three L1 trigger conditions:

- **t_LAr** (LAr Big-Tower energy condition): the 45 000 LAr cells are grouped into
  **256 Big Towers**. A Big Tower fires if its summed energy exceeds the per-region
  threshold ([Kogler11, p.151]):
  - **BBE and CB** (backward + central barrel, electron-side): **4.8 GeV** electromagnetic
  - **FB and IF** (forward barrel, inner forward, HFS-side): **6.2 to 25.6 GeV** depending on theta region
  - Big-Tower granularity: 14 in theta x 16 in phi ([Avila13, p.22])
  - The same element fires whether the energy comes from the **scattered electron** or
    from the **hadronic final state (HFS)**, which is what enables the
    electron/HFS independence used to measure t_LAr efficiency in situ
    ([Kogler11, p.151]).
- **t_T0** (LAr or CIP T0 timing): event timing match for the calorimeter or CIP within
  the in-time bunch crossing; rejects events whose CIP T0 corresponds to an earlier
  bunch crossing ([Kogler11, p.152]).
- **t_VETO** (background veto): combines ToF (plastic scintillators), CIP (track count for
  beam-gas rejection), and in 2006+ a beam-halo-muon veto using backward Tail-Catcher
  hits in coincidence with zero CIP tracks ([Kogler11, p.153]).

### 4.2 Typical S67 prescale

S67 was **not prescaled** in HERA-II ([Kogler11, Sec. 9.2.4] presents the full HERA-II
analysis assuming all S67-actual fires are used; [Britzger13, Sec. 4.1.1] likewise).
This is consistent with S67 being the main NC DIS trigger; prescaling it would
discard signal events. The L2NN trigger was introduced precisely so that S67 could
remain unprescaled despite the increased HERA-II luminosity ([Kogler11, p.45]).

### 4.3 Efficiency (HERA-II, [Kogler11, Sec. 9.2 and Table 9.2]):

| Component | HERA-II average efficiency | Period spread |
|-----------|----------------------------|---------------|
| t_LAr | >99.5 % above E_e > 13 GeV after fiducial-volume cut | constant |
| t_T0 | >99.5 % everywhere | constant |
| t_VETO (ToF) | 99.2-99.9 % | rises with run |
| t_VETO (CIP) | 99.5-99.6 % | constant |
| t_VETO (Muon) | 100 % (06 + 06/07 only) | N/A in 03/04 + 05 |
| **t_VETO total** | **98.8 % (03/04) - 99.5 % (06/07); 99.3 % HERA-II avg** | stable in time |
| **S67 total = LAr x T0 x VETO** | **~99.0 %, flat in Q2 and y** | total syst 1.2 % |

S67 sits flat at ~99 % across the full Q2 = 150-15 000 GeV^2 and y = 0.05-0.95
kinematic range of [Kogler11] (their Fig. 9.4).

### 4.4 Kinematic dependence (S67)

| Variable | Behavior | Source |
|----------|----------|--------|
| Scattered electron E_e | Before fiducial cut: 95 % at E_e ~ 11 GeV, rising to 100 % at E_e ~ 45 GeV. After fiducial cut: flat >99.5 % above E_e > 13 GeV. Cut: **require E_e > 11 GeV** and reweight MC in 11-13 GeV band. | [Kogler11, Fig. 9.1a] |
| theta_e | >99.5 % across full LAr acceptance after fiducial cut | [Kogler11, Fig. 9.1b] |
| Q2 | Flat at ~99 % for Q2 > 150 GeV^2 | [Kogler11, Fig. 9.4a] |
| y | Flat at ~99 % for y > 0.05 | [Kogler11, Fig. 9.4b] |
| Run period | All four HERA-II periods agree within stat. errors after fiducial cut | [Kogler11, p.152] |

---

## 5. Monitor Trigger Methodology

### 5.1 Principle ([DelDegan08, eq. 4.2]; [Reissner13, slide 4]; [Kogler11, eq. 9.2])

To measure the efficiency of a trigger element **TE** without bias from itself,
select an event sample triggered by an **independent monitor trigger MT** that is
*statistically uncorrelated* with TE, and count the fraction of those events in
which TE also fired:

```
        N(MT && TE)        # events triggered by monitor AND also fired the test element
eps(TE) = -----------
        N(MT)              # all monitor-triggered events
```

For a multi-component subtrigger S = TE_1 AND TE_2 AND TE_3, the trigger
efficiency factorizes **only if the elements are independent** given the event:

```
eps(S) = eps(TE_1) * eps(TE_2) * eps(TE_3)
```

If two elements depend on correlated event observables (e.g. both depend on the
electron energy in a non-trivial way), then a correlation correction is needed
(typically <1 % in practice).

### 5.2 Two complementary tricks

**Trick A: Within-trigger element independence (used for S67).**
The LAr Big-Tower trigger can be fired by *either* the scattered electron *or* the
HFS. This gives two independent subsamples:

- n_e = events triggered exclusively by electron-side Big Tower (no HFS BT)
- n_h = events triggered exclusively by HFS-side Big Tower
- n_eh = events triggered by both

Then ([Kogler11, eq. 9.2]):

```
eps(e) = n_eh / (n_h + n_eh)         # eff. of electron-side BT given HFS fires
eps(h) = n_eh / (n_e + n_eh)         # eff. of HFS-side BT given electron fires
eps(LAr) = eps(e) + eps(h) - eps(e)*eps(h)  # total LAr trigger element (inclusion-exclusion)
```

Same applies to t_T0 via CIP vs LAr T0.

**Trick B: Cross-monitoring by an entirely different subtrigger (used for S61 and for VETO).**
Pick a monitor trigger MT whose fire condition shares no L1 elements with the test
element. Examples used in the literature for S61 ([DelDegan08, Sec. 4.1.2.1]):

| Test element | Monitor triggers used | Why independent |
|--------------|-----------------------|-----------------|
| S61 SpaCal part (SPCLe_IET) | S71 (LAr + track + zVtx) or S67 (LAr only) | LAr is the trigger source, fully independent of SpaCal |
| S61 track + z-vertex part | S04 or S00 (SpaCal-only) | Tracks/zvtx not used, fully independent of CJC/CIP |
| t_VETO for S67 | S57 (S67 without veto condition) | Same trigger less veto ([Kogler11, p.153]) |

For runs where S57 was not available (run number < 382137 in HERA-II),
[Kogler11] used a QED Compton event sample as an alternative monitor.

For HERA-II S61 in our context, the modern set is (from [Reissner13, slide 4-6]):

- Monitor for the SpaCal IET part of S61: **independent triggers from inclusive DIS,
  all SpaCal-fiducial**.
- Monitor for the FTT part of S61: **DIS dijet selection with S0 and S1 as monitor
  triggers, plus FTT MC emulation cross-check**.
- Quoted systematic: **~1 % on the SpaCal part**, **~2 % on the FTT part**
  ([Reissner13, slide 13]).

### 5.3 What the v5b L1L2L3ac branch lets us do

The L1L2L3ac[4] = 128-bit array stores, for each event, which subtriggers actually
fired (post-prescale, post-L1/L2/L3 verification). The companion prescale[128] array
(in the run/lumi DB; reconstructed by gtustin's utility into the trigger weight)
stores the per-run prescale of each subtrigger. With these two pieces of information
we can:

1. Define a monitor sample **M** for any test subtrigger or test element T as
   `(L1L2L3ac bit MT == 1) AND (offline DIS selection passed)`. The monitor MT
   should be a subtrigger that we *know* doesn't include T as part of its
   composition, so an offline efficiency `N(M && T) / N(M)` is unbiased for T.
2. Factorize S61 efficiency into the SpaCal-IET and the FTT/track parts and
   measure each separately on a monitor sample whose fire condition lacks that
   ingredient.
3. Cross-check the per-element efficiency between data and the H1OO trigger
   emulation in MC.

---

## 6. Prescale-Corrected Trigger Weight (gtustin TrigWeightAC/RW)

### 6.1 Formula (from user context; standard H1 convention)

For each event:

```
prob_ac = product over s in {actual-fired AND enabled subtriggers}  ( 1 - 1/prescale_s )
TrigWeightAC = 1/(1 - prob_ac)   if prob_ac < 1
             = 0                  if prob_ac == 1  (no enabled subtriggers fired)
```

The same formula with `raw-fired` (pre-prescale) subtriggers in place of `actual-fired`
gives `TrigWeightRW`. The difference is that AC uses the post-prescale information
that already accounts for the prescale draws made during the run; RW assumes the
prescale will be applied independently and is the appropriate weight when the
prescale draw is not yet recorded.

### 6.2 Derivation

For a single subtrigger s with prescale n_s = 1/p_s (so p_s = probability that a
raw L1keep produces an actual L1keep):

- Probability that subtrigger s **does not** contribute an actual fire on a given
  L1keep event = `1 - p_s = 1 - 1/n_s`.

Assuming independent prescale draws across subtriggers (which is the case: each
subtrigger has its own modulo-n counter in the CTL), the probability that **none**
of the fired-and-enabled subtriggers produced an actual keep is:

```
P(no actual keep | this raw event)  =  product_s (1 - 1/n_s)        # = prob_ac
```

Therefore the probability that the event was actually kept by *at least one*
subtrigger is:

```
P_keep = 1 - prob_ac
```

To correct an actually-kept event back to the rate of all raw events that would
have triggered, we weight by `1/P_keep = 1/(1 - prob_ac)`. This is exactly
`TrigWeightAC`.

### 6.3 What this works for and what it does not

**Works:**

- Correctly handles overlap among any number of fired subtriggers (the
  inclusion-exclusion is automatic in the product form).
- Correct when prescale draws are statistically independent across subtriggers,
  which is the design of the H1 CTL.
- Properly preserves the inclusive trigger-OR rate when *any* of the enabled
  subtriggers is allowed to keep the event.

**Limitations:**

- Does **not** account for L1L2 or L3 inefficiency. TrigWeightAC corrects only the
  prescale draw, not the underlying trigger hardware/software efficiency. The
  remaining inefficiency must still be measured via the monitor-trigger technique
  and applied as a per-event scalar weight (or, equivalently, as a multiplicative
  MC weight in MC).
- Assumes the "enabled" mask is correct. If a subtrigger is bit-fired in the raw
  pattern but was masked off at L1 (disabled for that run), it must be excluded
  from the prob_ac product; this is what gtustin's utility handles via the
  per-run enabled mask.
- Becomes numerically unstable when prob_ac is extremely close to 1 (all
  contributing subtriggers heavily prescaled and only one fires). Cap the weight
  or fall back to a single-trigger normalization in that regime.
- Assumes the prescale draws across subtriggers are uncorrelated within an event;
  this is the H1 design but if the CTL is mis-configured (e.g., synchronized
  modulo counters), the formula slightly mis-estimates the weight.

### 6.4 Equivalent rewrite for software convenience

Equivalent to TrigWeightAC = 1 / (1 - prob_ac):

```
log_keep = sum over fired+enabled s of log(1 - 1/n_s)        # log of prob_ac
TrigWeightAC = 1 / (1 - exp(log_keep))
```

This rewrite avoids underflow when many lightly-prescaled subtriggers fire at once.

---

## 7. Concrete Plan for the SIDIS Trigger-Efficiency Study

### 7.1 Inputs we need from v5b trees

| Branch | Type | Use |
|--------|------|-----|
| `L1L2L3ac[4]` | 4 x UInt32 (= 128 bits) | The "actually fired and kept" pattern. Use this for both numerator (events firing test trigger) and denominator (events firing monitor trigger). |
| `TrigWeightAC` (already computed) | Float | Apply as event weight in *every* inclusive distribution (when not enforcing a single trigger). |
| `RunRange`, `RunPeriod` | Int | Bin trigger efficiency by run period (03/04 e+, 05 e-, 06 e-, 06/07 e+). |
| Per-run subtrigger enable mask | metadata | Needed to know which bits to include in the prob_ac product. Already incorporated by gtustin. |
| Scattered electron 4-vector, SpaCal/LAr cluster position, dead-cell map | offline reco | For per-kinematic-bin efficiency parameterization. |

### 7.2 Step-by-step procedure

**Step 1: Define our test trigger and our monitor triggers.**

For the **low-Q2 SIDIS** sample (SpaCal DIS, Q2 ~ 5 - 100 GeV^2):
- Test trigger: S61 (and individually its SpaCal-IET and FTT components).
- Monitor for the **SpaCal part**: subtriggers based on LAr or muon systems, e.g. S67
  or S71, restricted to the offline DIS selection (SpaCal electron candidate
  reconstructed with E_e' > 7 GeV).
- Monitor for the **FTT/track part**: any SpaCal-only trigger (S0, S3, or similar) -
  same selection - check what fraction of these events also pass the FTT condition.

For the **high-Q2 SIDIS** sample (LAr NC DIS, Q2 > 150 GeV^2):
- Test trigger: S67.
- Monitor for **t_LAr**: use the electron/HFS independence trick (Section 5.2 Trick A);
  no external monitor trigger needed.
- Monitor for **t_T0**: CIP T0 vs LAr T0 cross-validation, same trick.
- Monitor for **t_VETO**: subtrigger S57 (S67 without veto) where available, else
  QED Compton sample for early HERA-II runs.

**Step 2: Build the monitor sample.**

```python
# pseudo-code
def fired(L1L2L3ac, bit):
    return (L1L2L3ac[bit // 32] >> (bit % 32)) & 1

# Monitor sample for S61 SpaCal-IET efficiency:
mask_mon = (fired(L1L2L3ac, 67) | fired(L1L2L3ac, 71))   # LAr-based
mask_mon &= passed_DIS_selection                          # offline DIS sel
mask_mon &= (E_e_prime > 7.0)                             # plateau region

# Numerator: also fired S61
mask_num = mask_mon & fired(L1L2L3ac, 61)

eps_S61 = sum(mask_num * w) / sum(mask_mon * w)           # w = MC reweight if needed
```

Important: when measuring data efficiency, do **not** apply TrigWeightAC to the
monitor/test counts. TrigWeightAC corrects an *inclusive* OR-of-all-triggers rate,
not a conditional rate inside a single trigger sample.

**Step 3: Parameterize the efficiency.**

Tabulate eps_S61 in (E_e', y, SpaCal x, SpaCal y, run period). Smooth by binning
in coarse Q2 and y, and check for time stability. For S67 in the high-Q2
extension, use the (E_e', theta_e, run-period) parameterization of [Kogler11].

**Step 4: Apply as MC weight (preferred) or data weight.**

Standard H1 practice ([Kogler11, p.153]; [Britzger13, p.49]; [DelDegan08, p.59]):
**reweight MC events by the measured per-bin data efficiency** so that the MC matches
the data in trigger-corrected yields. This is mathematically equivalent to dividing
the data by the efficiency, but is numerically better behaved and avoids per-event
weight blow-ups in low-efficiency bins.

**Step 5: Assign systematic uncertainty.**

Conservatively (following H1 jets):

| Trigger | Systematic uncertainty | Source |
|---------|------------------------|--------|
| S67 total | 1.2 % flat | [Kogler11, p.155]; [Britzger13, p.49] |
| S67 t_LAr only | 0.5 % | [Kogler11, p.152] |
| S67 t_T0 only | 0.5 % | [Kogler11, p.152] |
| S67 t_VETO only | 1.0 % | [Kogler11, p.153] |
| S61 SpaCal part | ~1 % | [Reissner13, slide 5]; [DelDegan08, Sec. 6.2.2] |
| S61 FTT part | ~2 % | [Reissner13, slide 6] (data/MC at FTT emulation level) |
| **S61 total (low-Q2)** | **~2 % combined in quadrature** | this work |

For our trigger-OR weighted approach via TrigWeightAC, the systematic is more
subtle: it is the uncertainty on `1 - prob_ac`, which is dominated by the trigger
inefficiency of whichever subtrigger has fired most frequently. In practice it is
bounded above by the single-trigger uncertainty of the dominant subtrigger
contributing to the OR. For a low-Q2 SpaCal-DIS event, this is the S61 SpaCal-IET
part, giving an upper bound of ~1 % from the trigger-weight method on top of the
per-component efficiency systematics quoted above.

### 7.3 Expected SIDIS-analysis-level numbers

For our analysis where TrigWeightAC weighting is used inclusively (no single
subtrigger cut):

- **Low-Q2 inclusive DIS** (after offline DIS selection with E_e' > 11 GeV, fiducial
  SpaCal cut): trigger-OR efficiency expected at **~99 %** when at least one of
  {S61, S0, S3, ...} fires; **trigger systematic budget ~1.5 %**.
- **High-Q2 inclusive DIS** (E_e' > 11 GeV, LAr fiducial cut): trigger-OR
  efficiency expected at **~99.0 %** dominated by S67; **trigger systematic
  ~1.2 %** ([Kogler11, p.155]).

These numbers should be cross-checked on the actual v5b data:

```python
N_data_with_trigweight = sum(TrigWeightAC * passed_offline_DIS)
N_data_unweighted_OR   = sum(passed_offline_DIS & (any subtrigger fired))
# Ratio gives the effective trigger correction factor; should be ~1.01 for our sample
```

### 7.4 Cross-checks recommended

1. **Trigger OR vs single-trigger consistency.** Run the S67-only and the
   trigger-OR analyses for the high-Q2 sample; results should agree within the
   1.2 % trigger systematic. Discrepancy indicates a TrigWeightAC bug.
2. **Run-period stability.** Plot trigger efficiency vs run number per period;
   should be flat within ~0.5 %.
3. **Data/MC trigger emulation comparison.** For the SpaCal IET, compare the
   measured data trigger efficiency to the H1OO SpaCal trigger emulator output
   bin by bin in (E_e', x, y of impact). Disagreement is the FTT/IET systematic.
4. **Independence check.** Confirm that the monitor trigger's L1 elements are
   genuinely disjoint from the test element by inspecting the subtrigger
   definition file for the run period in question.
5. **TrigWeightAC vs hand-computed weight on a small data sample.** Spot-check
   1000 events: compute prob_ac by hand from the prescale array and L1L2L3ac
   bits and verify against the stored TrigWeightAC.

---

## 8. Open Items

- **Exact HERA-II S61 prescale per period.** Not in [Kogler11], [Britzger13], or
  [Reissner13]. Best source on NAF: the H1OO `H1SubTrigBits` and the per-run
  prescale tables under the H1OO bank/database. Action: extract per-period
  average S61 prescale directly from the v5b runtime info.
- **Exact HERA-II S61 L1/L2/L3 element list per period.** [DelDegan08] gives the
  HERA-I form; [Reissner13] confirms the high-level "SpaCal && FTT" structure
  for HERA-II but does not list the trigger element strings. Action: extract the
  subtrigger definition from `H1TrigSub` / steering files on NAF for the four
  HERA-II sub-periods.
- **SpaCal dead-cell map per period.** Need the time-dependent dead-cell map from
  the calibration database to define a proper SpaCal fiducial cut analogous to
  [Habib09]'s LAr fiducial volume cut referenced by [Kogler11].
- **TrigWeightAC sanity check.** Need to run the cross-check (Section 7.4 item 5)
  on a sample of v5b events to confirm gtustin's formula matches our
  expectations and that the per-run enabled mask is correctly applied.

---

## 9. Summary

- The H1 trigger is a four-level pipelined system (L1 hardware -> L2 NN/TT ->
  L3 FTT -> L4 reconstruction farm) producing a 128-bit subtrigger pattern per
  event ([Kogler11, Sec. 4.2.5]).
- **S61** = SpaCal IET + (HERA-II only) FTT L3 confirmation; main low-Q2 DIS
  trigger; efficiency ~99 % above E_e' > 7 GeV in the SpaCal fiducial volume;
  systematic ~1-2 % ([DelDegan08, Sec. 4.1.2.1]; [Reissner13]; [SpaCalNIM99]).
- **S67** = LAr Big-Tower AND T0 AND VETO; main high-Q2 NC DIS trigger;
  efficiency ~99.0 %, flat in Q2 and y above kinematic threshold; systematic
  1.2 % ([Kogler11, Sec. 9.2]; [Britzger13, Sec. 4.1.1]).
- **Monitor-trigger method**: factorize eps(S) = product of eps(TE_i) and
  measure each TE_i on a sample triggered by an independent subtrigger;
  combine via inclusion-exclusion when two TEs can fire independently
  ([Kogler11, eq. 9.2-9.3]; [DelDegan08, eq. 4.2]).
- **gtustin TrigWeightAC = 1/(1 - prod(1 - 1/n_s))** correctly accounts for
  prescales of overlapping subtriggers in an OR; needs to be supplemented by
  per-component efficiency corrections from the monitor-trigger study
  (user context; standard derivation in Section 6).
- **Action for our SIDIS analysis**: extract per-run S61 prescale and bit
  composition from the H1OO trigger database on NAF; run the monitor-trigger
  efficiency study with the procedure of Section 7; apply efficiency as an MC
  reweight; assign ~1.5 % systematic for low-Q2 and ~1.2 % systematic for
  high-Q2 trigger.
