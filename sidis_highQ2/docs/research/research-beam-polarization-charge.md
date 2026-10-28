<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Current research note. Peripheral to the active v3 low-Q2 convergence path; keep for future spin/beam-charge extensions.

# Research: H1 Beam Polarization and Beam Charge for SIDIS Spin/Charge Asymmetries

**Date:** 2026-05-14
**Audience:** SIDIS supervisor evaluating whether BSA (A_LU) and BCA (A_C)
can be added to the charged-hadron SIDIS low-Q2 program.
**Scope:** How HERA-II lepton-beam polarization and charge are recorded in
H1OO and on tape, what the low-Q2 SIDIS data set actually contains, and
the feasibility of A_LU and A_C measurements with the current trees.

> The mechanics (storage, accessors, run-period mapping) are read directly
> from the H1OO source tree at `H1/h1oo-master/`. The current data status is
> read from `H1/sidis_lowQ2/context-handoff.md` and adjacent research notes.
> Where I quote luminosities by helicity sub-period, those are HERA-II
> reference numbers known to me only at the magnitude level and must be
> reconfirmed with `oolumi` on NAF before any publication-grade claim
> (see Section 4 and "Open Questions").

## 1. Beam Polarization in H1

### 1.1 Where it lives on tape

Per-event lepton-beam polarization is stored on the **HAT** tree as two
branches (see `H1/h1oo-master/H1Hat/H1HatEvent.h` lines 47-66 and
`H1HatEvent.C` lines 1105-1113, 1361-1362):

| HAT branch | Type | Meaning |
|------------|------|---------|
| `Pol`      | `Float_t` | Longitudinal lepton polarization for this event, in **percent** (sign included) |
| `PolType`  | `Short_t` | Polarimeter / quality flag: 1 = TPOL, 2 = LPOL, 3 = weighted mean T+L, 0 = measurement not OK, negative values = error |

The H1HatEvent.h header note (line 54) states:

> Lepton polarization is defined as positive for right-handed (?)
> leptons.

The convention is RH = positive (+P), LH = negative (-P). The "(?)" in
the header comment means the comment author was not 100% sure of the
sign convention; in practice the H1OO codebase treats `Pol > 0` as RH
and the per-period `fElectronBeamHelicity` table in
`H1Geom/H1Constants.C` lines 255-272 is consistent with that
convention (see Section 3 below).

### 1.2 Reading it through H1Calculator

`H1Calculator::Polarisation()` returns a `H1CalcPolarisation*` that
exposes:

```cpp
Float_t GetPolarisation();          // returns Pol/100 = signed fraction, 0 if quality bad
Int_t   GetPolarisationQuality();   // = PolType for data, = 1 for MC
```

(see `H1/h1oo-master/H1Calculator/H1CalcPolarisation.h` and `.C`).

Important behavioral details from `H1CalcPolarisation.C`:

1. **Data**: a `static H1FloatPtr Pol("Pol")` and `H1ShortPtr
   PolType("PolType")` read directly from HAT. If `PolType <= 0` the
   returned polarization is forced to 0.
2. **MC**: `GetPolarisation()` does **not** return zero in MC. Instead,
   for `RunPeriod >= eEplus0304` it samples a value at random from a
   data-derived polarization profile histogram `fPolHist` that the user
   must inject via `H1CalcPolarisation::SetPolHist(TH1F*)`. Without
   `SetPolHist` it prints `"H1CalcPolarisation: Error, No Polarisation
   Histo!"` and returns 0. (This is the H1 standard "MC sees the data
   polarization profile" pattern used for polarized-NC cross-section
   reweighting.)
3. The returned value is **already divided by 100** (i.e. a fraction in
   `[-1,+1]`, signed), to match `H1QCDFunc` conventions.

The "MC sees data profile" mechanism is also wired into `H1CalcWeight`
via `ApplyPolarisationInputMC(Bool_t)` and
`ApplyPolarisationOutputMC(Bool_t)` (`H1CalcWeight.h` lines 78-79,
263-267). This matters only if you want to reweight unpolarized MC to
data polarization conditions (which we do not need for an asymmetry
measurement -- the asymmetry is event-by-event and the MC just provides
acceptance).

### 1.3 Per-run, per-fill summary numbers

`H1RunLumi` (`H1/h1oo-master/H1Tools/H1RunLumi.h`) carries per-run:

| Method                                    | Meaning                                              |
|-------------------------------------------|------------------------------------------------------|
| `GetPolRunFraction()`                     | Lumi-weighted fraction of run with valid pol meas    |
| `GetPolarisation()`                       | Mean signed pol of the run (in percent, signed)      |
| `GetPolarisationErrorStat()`              | Stat error                                           |
| `GetPolarisationErrorUncorrSysTPOL/LPOL/HERA` | Uncorrelated sys errors (T-polarimeter, L-polarimeter, HERA orbit) |
| `GetPolarisationErrorCorrSysTPOL/LPOL`    | Correlated sys errors per polarimeter                |

`H1RunList` (`H1/h1oo-master/H1Tools/H1RunList.h` lines 72-100) gives
**lumi-weighted averages** that are exactly what we need for a BSA
sample:

```cpp
Float_t GetAveragePolarisation(Int_t charge, Int_t helicity,
                               Int_t runStart, Int_t runEnd);
Float_t GetAveragePolarisationForPeriod(Int_t charge, Int_t helicity);
H1ArrayF GetAveragePolarisation(const H1ArrayI& runRangeDivision);
```

These are the canonical accessors used by all H1 polarized-cross-section
analyses (e.g. polarized NC and CC inclusive). The fill-level fluctuations
relevant for BSA are encoded in the per-event HAT `Pol` branch; the
per-run / per-period averages above are the input for BCA-style splits
where the polarization sign is fixed by selecting a helicity sub-period.

### 1.4 Bias / quality flag semantics

The header documentation in `H1HatEvent.h` (lines 64-65) and the use in
`H1CalcPolarisation.C` line 95-97 jointly imply the operational rule:

```
Use the event's polarization only if PolType > 0.
   PolType = 1 -> TPOL only
   PolType = 2 -> LPOL only
   PolType = 3 -> weighted T+LPOL average  (preferred when available)
   PolType = 0 -> measurement not OK   -> drop or use period mean
   PolType < 0 -> error                -> drop
```

`H1DetectorStatus.C` (lines 296-298, 877-882) further enforces a
**run-level polarization gate** if `fPolFlag` is set in the lumi
calculation: events / runs without a valid pol measurement are excluded
from the integrated luminosity. This is the polarized-DIS analysis
recipe. For a BSA we will need to switch this on, which causes the
luminosity quoted in `H1/sidis_lowQ2/context-handoff.md` (the superseded pre-rescale luminosity
nb^-1) to drop slightly (typically a few percent of HERA-II is "no
polarisation measurement").

## 2. Beam Charge in H1

### 2.1 Per-run, not per-event

There is **no per-event "beam charge" branch** on HAT. Beam charge is
derived from the run number via the `H1Constants` machinery
(`H1/h1oo-master/H1Geom/H1Constants.h/.C`):

| API                                              | Returns              |
|--------------------------------------------------|----------------------|
| `H1Constants::ElectronBeamCharge(Int_t run)`     | +1 (e+p) or -1 (e-p) |
| `H1Constants::GetElectronBeamCharge()`           | Same, for current run |
| `H1Constants::ElectronBeamHelicity(Int_t pol_period)` | +1 RH, -1 LH, 0 unpol |
| `H1Constants::GetPolPeriod(Int_t run)`           | enum `PolPeriod`     |

`SetConstants(Int_t run)` (lines 413-430 of `H1Constants.C`) caches all
of these for the current run. The `RunPeriod` enum (`H1Constants.h`
lines 211-215) maps the calendar period to charge:

```
eEplus0304   -> e+p (2003-2004)
eEminus0405  -> e-p (2004-2005)
eEminus06    -> e-p (2006)
eEplus0607   -> e+p (2006-2007)
eEplus07lowE460/575 -> e+p (low-E run, separate)
```

### 2.2 Beam-charge consistency with the scattered electron

`H1CalcElec` (`H1Calculator/H1CalcElec.h` lines 35, 147, 310, 455-460)
provides:

```cpp
Int_t  GetBeamChargeMatch(Int_t ElecNumber=FirstElectron);
void   UseBeamCharge(Bool_t = true);
```

`GetBeamChargeMatch` returns a comparison flag of the reconstructed
scattered-electron charge with the beam charge -- useful as a QA cut for
NC events but not as the primary beam-charge tag.

### 2.3 What "beam charge" means for our trees

The bottom line: **to know the beam charge of an event in our SIDIS
trees, look up `RunNumber` in the H1 run-period map.** The boundaries
relevant to us (from `H1Constants.C` lines 157-187 and 249-272 plus
`GetPolPeriod`) are:

```
e+p running:   367257..396673   (eEplus0304)
e-p running:   396674..444093   (eEminus0405)         <-- "em0405"
e-p running:   444094..468529   (eEminus06)           <-- "em06"
e+p running:   468530..492499   (eEplus0607, 2006 part)
e+p running:   492508..500611   (eEplus0607 / eEplus07Per1NegHel)  <-- 2007 e+p LH only
e+p (low-E)    500918..511079   (eEplus07lowE460 / 575)
```

These boundaries match the run cuts already coded in our SIDIS analysis
scripts (`source_cache/analyze_charged_xsec_corrected.py` lines 93-94
and `critique-systematics-implementation.md` lines 101-103).

## 3. Running-Period Inventory (HERA-II)

The full HERA-II polarization-period map, taken verbatim from
`H1/h1oo-master/H1Geom/H1Constants.C` (lines 157-187 for run starts and
252-272 for the helicity assignment), is the authoritative source. The
relevant rows:

| `PolPeriod` enum       | Run range         | Year | Charge | Helicity (sign of P) | Approx. lumi (nb^-1) |
|------------------------|-------------------|------|--------|----------------------|----------------------|
| `eEplus03RH1`          | 357072 - 367256   | 2003 | e+p    | +1 (RH)              | small  (HERA-II startup) |
| `eEplus04RH1`          | 367257 - 376562   | 2004 | e+p    | +1 (RH)              | ~10k   |
| `eEplus04LH1`          | 376563 - 386696   | 2004 | e+p    | -1 (LH)              | ~25k   |
| `eEplus04RH2`          | 386697 - 392213   | 2004 | e+p    | +1 (RH)              | ~15k   |
| `eEminus04RH1`         | 396674 - 398818   | 2004 | e-p    | +1 (RH)              | ~3k    |
| `eEminus05RH1`         | 399101 - 402991   | 2005 | e-p    | +1 (RH)              | ~4k    |
| `eEminus05LH1`         | 402992 - 415619   | 2005 | e-p    | -1 (LH)              | ~50k   |
| `eEminus05RH2`         | 415620 - 427812   | 2005 | e-p    | +1 (RH)              | ~33k   |
| `eEminus05LH2`         | 427813 - 436893   | 2005 | e-p    | -1 (LH)              | ~17k   |
| `eEminus06LH1`         | 444094 - 458792   | 2006 | e-p    | -1 (LH)              | ~38k   |
| `eEminus06RH1`         | 458793 - 466997   | 2006 | e-p    | +1 (RH)              | ~21k   |
| `eEplus06RH1`          | 468530 - 485713   | 2006 | e+p    | +1 (RH)              | ~70k   |
| `eEplus06LH1`          | 485714 - 486072   | 2006 | e+p    | -1 (LH)              | ~1k (very short) |
| `eEplus06RH2`          | 486073 - 490161   | 2006 | e+p    | +1 (RH)              | ~13k   |
| `eEplus06LH2`          | 490162 - 492499   | 2006 | e+p    | -1 (LH)              | ~12k   |
| `eEplus07LH1`          | 492508 - 500611   | 2007 | e+p    | -1 (LH)              | ~46k   |
| `eEplus460LH1`         | 500918 - 507824   | 2007 | e+p (low-E) | -1 (LH)         | ~12k (NOT in our SIDIS sample) |
| `eEplus575LH1`         | 507843 - 511079   | 2007 | e+p (low-E) | -1 (LH)         | ~6k  (NOT in our SIDIS sample) |

> **Caveat:** the per-sub-period luminosities above are typical HERA-II
> reference numbers and must be regenerated with `oolumi` on
> `SelectedRuns.root` before any quoted result. The shape of the table
> (which periods carry which charge/helicity) is verbatim from
> `H1Constants.C`.

The user-supplied period catalog in the task brief
("2003-2004 e+p, 2004-2005 e-p, 2005-2006 e+p, 2006 e-p, 2007 e+p")
is **incorrect**: HERA actually ran e-p in 2005 and most of 2006, and
e+p only at the end of 2006 and in 2007. The H1Constants table above is
the authoritative source.

### 3.1 What our SIDIS low-Q2 data actually contains

The SIDIS low-Q2 sample integrates the superseded pre-rescale luminosity nb^-1 over 2005, 2006,
2007 (`context-handoff.md` lines 79-80, 146-152):

| Year | L_data (nb^-1) | Charge content |
|------|----------------|----------------|
| 2005 | 106,823.1      | **all e-p** (covers eEminus05RH1, LH1, RH2, LH2) |
| 2006 | 144,634.0      | **mixed**: ~45% e-p (em06 RH1+LH1, runs 444094-466997) and ~55% e+p (eEplus0607, runs 468530-492499) |
| 2007 |  46,200.7      | **all e+p**, single helicity (eEplus07LH1, runs 492508-500611, **LH only**) |
| **Total** | **the superseded pre-rescale luminosity** | |

Charge composition (rough HERA-II ratios):

- **e-p (lumi-weighted):** ~106.8k + ~65k = **~170-175k nb^-1**, ~57%
- **e+p (lumi-weighted):** ~80k + 46.2k     = **~125-130k nb^-1**, ~43%

This is **completely different from the high-Q2 SIDIS situation**. The
high-Q2 production currently has only `ep0607` (140,306 nb^-1, all e+p,
see `research-highQ2-sidis-extension.md` lines 144-158). For low-Q2 we
already have **both beam charges in the merged tree** -- the BCA is not
blocked by missing data the way the high-Q2 BCA is.

### 3.2 Helicity composition

From the table in Section 3, summed by helicity over the runs we have:

- **2005 e-p RH** (eEminus05RH1+RH2): runs 399101-402991, 415620-427812
  -> rough lumi ~37k nb^-1
- **2005 e-p LH** (eEminus05LH1+LH2): runs 402992-415619, 427813-436893
  -> rough lumi ~67k nb^-1
- **2006 e-p RH** (eEminus06RH1):     runs 458793-466997   -> ~21k nb^-1
- **2006 e-p LH** (eEminus06LH1):     runs 444094-458792   -> ~38k nb^-1
- **2006 e+p RH** (eEplus06RH1+RH2):  runs 468530-485713, 486073-490161 -> ~83k nb^-1
- **2006 e+p LH** (eEplus06LH1+LH2):  runs 485714-486072, 490162-492499 -> ~13k nb^-1
- **2007 e+p LH** (eEplus07LH1):      runs 492508-500611   -> ~46k nb^-1

Aggregated:

| Beam | Helicity | Approx lumi (nb^-1) |
|------|----------|---------------------|
| e-   | RH       | ~58k                |
| e-   | LH       | ~105k               |
| e+   | RH       | ~83k                |
| e+   | LH       | ~59k                |

So we have **non-trivial lumi in all four (charge x helicity) combinations**.
This is the key fact for both BSA and BCA feasibility.

### 3.3 Typical achieved polarization

Per H1 published HERA-II analyses, the lumi-weighted mean longitudinal
polarization fractions are typically:

- |P_e| ~ 0.30-0.40 in HERA-II e-p periods
- |P_e| ~ 0.30-0.36 in HERA-II e+p periods

The exact values must be obtained by calling
`H1RunList::GetAveragePolarisationForPeriod(charge, helicity)` on the
SelectedRuns list on NAF. Per-period sigma_P is order ~10% of P. This
is research input, not a result of this document.

## 4. BSA (A_LU) Feasibility

### 4.1 What A_LU actually measures

For an unpolarized proton target at H1, the BSA is

```
A_LU = (1 / <|P_e|>) * (N_R - N_L) / (N_R + N_L)
```

(per beam charge separately, since the structure-function combination
differs for e+p and e-p). The relevant SIDIS structure function is
F_LU^{sin phi_h}, a twist-3 object accessing the genuine twist-3 PDF
e(x) (Collins partner) and twist-3 fragmentation Etilde, Htilde
(Collins-Mulders-Tangerman classification). It is small (~few %)
and modulates as sin(phi_h).

For a MEANINGFUL BSA result one extracts an azimuthal moment:

```
A_LU^{sin phi_h} ~ <sin(phi_h) * helicity> / (D * <|P_e|>)
```

with D the depolarization factor (~y-dependent, O(1)).

### 4.2 Statistical reach -- order of magnitude

Take e-p as the larger sample. Total e-p in our SIDIS data ~170k nb^-1.
The HERMES/COMPASS sin(phi_h) BSA moments are typically 1-5% of size.
The statistical precision on a sin(phi_h) moment is

```
sigma(A_LU^{sin phi_h}) ~ sqrt(2 / N) / (<|P_e|> * <D> * <fdil>)
```

For our charged-hadron tree (~18M reco h+/- entries on full dataset, see
`context-handoff.md` line 443), in a coarse 1D phi_h fit (no Q2/x/z/pT_h
binning), with `<|P_e|> ~ 0.36` and `<D> ~ 0.6`:

```
sigma(A_LU^{sin phi_h}) ~ sqrt(2 / 18e6) / (0.36 * 0.6) ~ 0.0015  ~  0.15%
```

This is **statistically excellent** for an inclusive measurement. Even
with the HERA-II-typical 0.05 (5%) target precision per (Q2, x, z, pT_h)
4D bin, we have ~1.2M tracks per (Q2, x) cell (handoff line 443) which
gives:

```
sigma_4D(A_LU^{sin phi_h}) ~ sqrt(2 / 1.2e6) / (0.36 * 0.6) ~ 0.6%
```

per (Q2, x) cell at coarse z, pT_h -- still measurable for any BSA
above ~1%.

### 4.3 The hard parts

- **Polarization-aware luminosity:** As discussed in Section 1.4, the
  `fPolFlag` switch in lumi accounting drops events without a valid pol
  measurement. The the superseded pre-rescale luminosity nb^-1 value in our handoff was computed
  from `SelectedRuns.root` **without** the polarization gate. We will
  lose a few percent of this when restricting to `PolType > 0`. This
  needs an explicit `oolumi` recomputation.
- **Sign convention for phi_h vs charge:** Trento phi_h flips sign
  when the beam charge flips (the convention is defined w.r.t. the
  outgoing lepton plane). This is exactly the OQ10 issue already
  flagged in `context-handoff.md` lines 645-650. It must be frozen
  before BSA per-charge results can be combined.
- **Helicity-asymmetric MC handling:** RAPGAP31 and DJANGOH14 are
  unpolarized generators. The MC acceptance is helicity-symmetric by
  construction. This is good for BSA: any helicity-dependent reco
  efficiency is a real systematic, not a generator artifact.
- **Per-event polarization use vs per-period mean:** the cleanest BSA
  uses `Pol` event-by-event so that fills of different pol enter with
  the right weight. The simpler approach is a per-period mean
  (`<P>` per helicity sub-period from `H1RunList::GetAveragePolarisation`).
  Both are standard.
- **What is needed in the trees:** see Section 6.

## 5. BCA (A_C) Feasibility

### 5.1 What A_C actually measures

For inclusive SIDIS without polarization,

```
A_C = (dsigma(e+p) - dsigma(e-p)) / (dsigma(e+p) + dsigma(e-p))
```

probes the gamma/Z interference for inclusive DIS but for a SIDIS
hadron-level observable it accesses the cos(phi_h) modulation at twist
2 differently than the unpolarized cross-section: the relevant
combinations of structure functions F_UU^{cos phi_h} and the gamma-Z
interference term enter with opposite signs for e+/e-. At low Q2 the
gamma-Z interference is negligible (sin^2(theta_W) suppressed by Q2 / M_Z^2),
which is the reason no SIDIS BCA at low Q2 has ever been measured at
H1 or ZEUS.

> **Physics caveat for low Q2:** the predicted leading-twist BCA from
> gamma-Z interference is < 0.001 for our Q2 < 100 GeV2 range -- well
> below any reasonable systematic floor. The case for low-Q2 BCA is
> therefore much weaker than for high-Q2, where Z-mediated effects
> become measurable. We have the data for it, but the physics signal
> is small at low Q2.

A different, more interesting low-Q2 use of a charge split is to
**cancel detector charge bias** in `A_h = (N_h+ - N_h-)/(N_h+ + N_h-)`
by combining e+p and e-p samples (the leading detector charge asymmetry
flips sign relative to the physics signal). This is essentially what
the SIDIS systematics catalog calls the "period / beam-charge split"
(`reference-sidis-systematics.md` lines 152-155, 207-208). It is more
useful as a systematic cross-check than as a primary observable.

### 5.2 Statistical reach

If we did pursue the cos(phi_h) BCA at low Q2:

```
sigma(A_C^{cos phi_h}) ~ sqrt(2 / N) ~ sqrt(2 / 18e6) ~ 0.05%
```

inclusively, ~0.5% per (Q2, x) cell. Statistically trivial; but the
predicted signal is essentially zero at low Q2.

### 5.3 Period/charge split for systematics

This *is* useful, and is effectively free given that we already have
RunNumber on every event:

```python
# in any analysis script:
mask_eplus  = (runs >= 468530) & (runs <= 500611)   # 2006 e+p + 2007 e+p
mask_eminus = (runs >= 399101) & (runs <= 466997)   # 2005 e-p + 2006 e-p (em06)
```

This split is implemented today only as a 3-year split (2005/2006/2007)
in `compute_systematics_charged.py` line 731 etc. The 2006 split into
em06 and ep0607 (boundary at run 468530) is **not** done. Adding it is
a one-line change.

## 6. What Branches/Variables to Add to the Trees

The current `ChargedTot` tree (handoff Section 4) already stores
`RunNumber`, which is sufficient to derive beam charge offline. To
support BSA we need polarization. The minimum change:

| New branch       | Type    | Source                                                         |
|------------------|---------|----------------------------------------------------------------|
| `Pol`            | Float_t | `H1FloatPtr Pol("Pol")` (HAT) -- raw value, in **percent**, signed |
| `PolType`        | Short_t | `H1ShortPtr PolType("PolType")` (HAT)                           |

Pattern (in `ProtonTreeBuilder_lowQ2.C` near line 154-170 where HAT
pointers are declared):

```cpp
H1FloatPtr hatPol("Pol");
H1ShortPtr hatPolType("PolType");

Float_t Pol;       Short_t PolType;
chargedTree->Branch("Pol",     &Pol);
chargedTree->Branch("PolType", &PolType);

// in event loop:
Pol     = *hatPol;
PolType = *hatPolType;
```

**Optional, but useful:**

| Branch           | Type    | Reason |
|------------------|---------|--------|
| `BeamCharge`     | Int_t   | +1/-1 from `H1Constants::ElectronBeamCharge(RunNumber)`. Redundant with RunNumber but trivially fast. |
| `BeamHelicity`   | Int_t   | +1/-1/0 from `H1Constants::ElectronBeamHelicity(GetPolPeriod(RunNumber))` -- the **nominal/intended** helicity of the period (different from event Pol sign in tails) |
| `PolPeriod`      | Int_t   | `(Int_t)H1Constants::GetPolPeriod(RunNumber)` -- the enum, useful for period-binned averaging |

These three are derived from `RunNumber` and could equally well be
applied at analysis time using the H1Constants singleton, so they are
**not strictly needed in the tree** -- but they make the tree
self-describing for downstream non-H1OO consumers (e.g. uproot-only
Python analysis).

**Not needed in the tree** (better to compute at analysis time):

- Per-run mean polarization and its errors (`H1RunLumi::GetPolarisation` etc).
  These belong in a separate run-summary file, not duplicated per event.
- `PolarisationQuality` from `H1CalcPolarisation` -- this is just `PolType`
  for data and 1 for MC, so storing `PolType` is enough.

**Builder hooks for MC polarization profile:** if we want MC events to
carry a representative `Pol` value (rather than 0), we must call
`H1Calculator::Instance()->Polarisation()->SetPolHist(myDataPolHist)`
once at startup, **then** read `gH1Calc->Polarisation()->GetPolarisation()
* 100` (in percent) into the MC `Pol` branch. This is a cosmetic / QA
choice; for an asymmetry, the MC carries no helicity information by
construction and `Pol = 0` (or "undefined") is fine.

## 7. Previous H1 Measurements

H1 has published several polarized DIS measurements at HERA-II:

- **Polarized inclusive NC and CC cross-sections** (multiple papers,
  2009-2018). These are the canonical use of the H1OO polarization
  machinery and the source of the lumi-weighted average polarization
  values discussed above.
- **Helicity dependence of CC cross-sections** -- the strongest HERA-II
  polarization observable, since CC vanishes for one beam helicity.
- **No published H1 SIDIS BSA**, to my knowledge. ZEUS and HERMES have
  SIDIS BSA results but with different detectors and (HERMES) different
  kinematics.
- **No published H1 SIDIS BCA**, to my knowledge.

This means a low-Q2 SIDIS A_LU^{sin phi_h} result, even at modest
precision, would be a **first-of-its-kind H1 measurement** and a
genuine new TMD-relevant input. The community interest is in the
twist-3 e(x) PDF, which is poorly constrained. ZEUS (e.g. ZEUS BSA in
SIDIS, arXiv 1603.05151) is the closest precedent.

This research note does **not** include a published-literature
deep-dive for HERA SIDIS BSA/BCA; if the supervisor decides to pursue
A_LU we recommend a follow-up `arxiv-reader` task to establish the
ZEUS / HERMES baselines and the theoretical modulation expectations.

## 8. Recommendation

### 8.1 BSA (A_LU^{sin phi_h}) -- **YES, feasible, recommend pursuing**

- **Data exists**: both helicities are present in our 2005, 2006,
  2007 data with non-trivial lumi (Section 3.2).
- **Polarization is recorded per event**: HAT branches `Pol` and
  `PolType` carry signed polarization in percent and a quality flag,
  with full H1OO accessor support via `H1CalcPolarisation` and
  `H1RunLumi`.
- **Statistics are excellent**: O(0.1%) inclusive precision, O(0.5%)
  per (Q2, x) cell -- easily sensitive to any sin(phi_h) BSA at the
  ~1% level expected from twist-3 SIDIS structure functions.
- **Tree changes needed**: just add `Pol` (Float) and `PolType`
  (Short) HAT branches to ChargedTot. ~10 lines of builder code,
  no rerun of MC needed (MC will just carry zeros for these branches
  unless we wire `SetPolHist`, which we don't need for asymmetries).
- **MC compatibility**: RAPGAP31/DJANGOH14 are unpolarized; this is
  fine for acceptance. Asymmetry cancels MC normalization.
- **First H1 SIDIS BSA**: likely a publishable, novel TMD input.

The main blockers are *not* technical:
1. The phi_h convention (Trento vs handedness vs charge sign) must be
   frozen -- already an open question (OQ10 in handoff).
2. The polarization-gated luminosity must be recomputed.
3. A literature/theory study of expected A_LU^{sin phi_h} at low Q2
   is needed to scope the binning and target precision.

### 8.2 BCA (A_C) -- **NO at low Q2 for primary physics, YES as a systematic check**

- **Data exists**: ~170k nb^-1 e-p and ~125k nb^-1 e+p in our merged
  sample (Section 3.1). The "high-Q2 BCA blocked by missing e- data"
  problem **does not apply** at low Q2 -- we have both beam charges.
- **Physics signal at low Q2 is tiny**: gamma-Z interference is
  ~Q2/M_Z^2 -- below 10^-3 in our Q2 < 100 GeV2 range. There is no
  measurable BCA in the leading-twist sense.
- **Useful as a systematic cross-check**: a 4-bin charge x helicity
  split of A_h and of phi_h moments is a strong test of charge-dependent
  detector bias. This is essentially free given `RunNumber`.
- **Tree changes needed**: none (RunNumber suffices). Optional
  convenience branch `BeamCharge`.

### 8.3 Concrete next actions if BSA is approved

1. Add `Pol`, `PolType` branches to `ProtonTreeBuilder_lowQ2.C`. 
   Recompile (`make bin`) and rerun data only (MC not needed).
2. Recompute polarization-gated luminosity per (charge x helicity)
   sub-period using `H1RunList::GetIntLumi(charge, helicity, ...)` and
   `oolumi` with `fPolFlag = true`.
3. Decide phi_h sign convention for e+ vs e- (resolves OQ10).
4. Build phi_h x helicity histograms in coarse (Q2, x, z, pT_h) bins,
   per beam charge separately.
5. Fit `A_LU^{sin phi_h} * sin(phi_h)` plus a constant baseline; correct
   by `1 / (<|P_e|> * D)` per bin.
6. Commission as inclusive (1D phi_h) result first; expand to 2D, 4D
   only after the inclusive number passes systematics review.

## 9. Open Questions and Risks

| ID  | Question / risk                                                                       | Severity |
|-----|---------------------------------------------------------------------------------------|----------|
| BP1 | Per-sub-period lumi values in Section 3 are estimates. Need `oolumi` confirmation on `SelectedRuns.root` per (charge, helicity).| Medium |
| BP2 | Polarization-gated `fPolFlag = true` luminosity is unmeasured for our sample. Will reduce the superseded pre-rescale luminosity nb^-1 by a few %. | Low |
| BP3 | The phi_h sign convention for e+ vs e- (OQ10) blocks combining the two charges in a BSA. | Medium |
| BP4 | The header comment in `H1HatEvent.h` line 54 ("(?)") on the RH = +Pol convention is not 100% explicit. Cross-check by comparing per-period `Pol` mean sign vs `fElectronBeamHelicity` table is recommended. | Low |
| BP5 | MC has no per-event helicity. If we ever want a polarized-MC closure test, must call `H1CalcPolarisation::SetPolHist` with a data-derived polarization profile. | Low (not needed for asymmetry) |
| BP6 | The 2007 sample (eEplus07LH1) is single-helicity. Within 2007 alone, no BSA possible -- it must be combined with a same-charge opposite-helicity sample (eEplus06RH1+RH2 vs eEplus06LH1+LH2 + eEplus07LH1). Helicity balance for e+ is then ~83k RH vs ~59k LH, sufficient. | Low |
| BP7 | The user-supplied period catalog in the task brief ("2003-2004 e+p, 2004-2005 e-p, 2005-2006 e+p, 2006 e-p, 2007 e+p") is incorrect. Section 3 supersedes it. | Resolved here |
| BP8 | A literature check on H1/ZEUS/HERMES SIDIS BSA precedents and theory expectations was *not* performed in this note. Recommend a follow-up `arxiv-reader` pass. | Medium for scoping; not blocking the technical decision |

## 10. Summary Table

| Question                                            | Answer |
|-----------------------------------------------------|--------|
| Is per-event polarization stored?                   | Yes -- HAT `Pol` (Float, in %, signed) and `PolType` (Short, quality flag) |
| Accessor in H1OO?                                   | `H1Calculator::Polarisation()->GetPolarisation()` returns signed fraction (Pol/100); `GetPolarisationQuality()` returns `PolType` |
| Per-run polarization summary?                       | `H1RunLumi` and `H1RunList::GetAveragePolarisation(charge, helicity, ...)` |
| Is per-event beam charge stored?                    | No -- derived per run via `H1Constants::ElectronBeamCharge(RunNumber)` |
| Are both e+ and e- in our SIDIS low-Q2 data?       | **Yes** -- ~170k nb^-1 e-p (2005 + em06) + ~125k nb^-1 e+p (eEplus0607 + 2007) |
| Are both helicities in our data?                    | **Yes** in all four (charge x helicity) combinations -- though 2007 alone is e+p LH only |
| Is BSA (A_LU^{sin phi_h}) feasible?                 | **Yes**. Statistically excellent. ~10 lines of builder code to add `Pol`, `PolType`. |
| Is BCA (A_C) feasible at low Q2?                    | Statistically yes; physically the signal is tiny (gamma-Z suppressed by Q2/M_Z^2). Useful only as systematic cross-check. |
| What tree changes are needed?                       | Add `Pol` (Float) and `PolType` (Short) HAT branches. Optional convenience: `BeamCharge`, `BeamHelicity`, `PolPeriod` derived from RunNumber. |
| Recommendation                                      | Pursue BSA. Skip BCA as primary physics; use charge split as a systematic check. |
