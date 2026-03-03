# H1 Lambda/K0s Analysis Chain

Full execution order for measuring Lambda baryon and K0s meson cross sections
and baryon-antibaryon asymmetries from HERA II data (2005-2007).

---

## Phase 1: Batch Farm Jobs (HTCondor at DESY NAF)

These steps touch raw H1OO data on dCache and require the H1 environment.
Each step produces many small ROOT files that must be merged before the next step.

### Step 0 --- Verify environment

```bash
echo $H1DIST
echo $H1CURRENT
echo $BINDIR
which root
```

All three env vars must be set and ROOT must be found.

### Step 1 --- Build executables

```bash
cd $BASE/H1LambdaC
make create_evls lambdaTree kaonTree
```

Produces binaries in `$BINDIR/`. Verify:

```bash
ls -l $BINDIR/create_evls $BINDIR/lambdaTree $BINDIR/kaonTree
```

### Step 2 --- Generate steering files

Event-selection steerings (one per year):

```bash
cd $BASE/H1LambdaC/Steering/2005 && bash create_Event_steerings_05.sh
cd $BASE/H1LambdaC/Steering/2006 && bash create_Event_steerings_06.sh
cd $BASE/H1LambdaC/Steering/2007 && bash create_Event_steerings_07.sh
# Also 2007low, 2007med if needed
```

Each `.steer` file points at a chunk of H1 HAT/MODS data on dCache.
Luminosity steering files (`HERA2Lumi.steer`, `lumi06.steer`, etc.) define
run ranges, good-run exclusions, and detector status requirements
(CJC1, CJC2, LAR, SPAC, TOF, LUMI, CIP, BDC all on).

Verify:

```bash
ls Steering/2006/SelectedEvents06_*.steer | head -5
cat Steering/2006/SelectedEvents06_0.steer
```

### Step 3 --- Create event lists (batch, ~150-320 jobs per period)

`create_evls.C` applies DIS event selection (scattered electron ID, Q2/y cuts,
vertex requirements, subtrigger 61) and writes event lists.

**Single-job test (interactive):**

```bash
$BINDIR/create_evls \
  -f $BASE/H1LambdaC/Steering/2006/SelectedEvents06_0.steer \
  -o $BASE/H1LambdaC/EventLists/2006/test_evls_0.root
```

Verify output:

```bash
root -l -b -q -e '
  TFile f("'$BASE'/H1LambdaC/EventLists/2006/test_evls_0.root");
  f.ls();
  cout << "Keys: " << f.GetNkeys() << endl;
'
```

**Full batch submission:**

```bash
cd $BASE/H1LambdaC
bash SubmitScripts/submitallevlsjobs.sh
```

This calls `condor_submit` on `CondorScripts/multi_submit_create_evls_05`,
`_06`, `_07`, `_07low`, `_07med` (up to 320 jobs each).

Same for MC: `create_evlsMC.C` run over RAPGAP31 and DJANGOH14 samples.

### Step 3b --- Merge event lists

```bash
# Test merge of a few files first:
cd $BASE/H1LambdaC/EventLists/2006
hadd -k -f test_merged.root condor_06_evls_0.root condor_06_evls_1.root

# Full merge (all years + MC):
bash $BASE/H1LambdaC/SubmitScripts/mergeallfiles.sh
```

Outputs:
- `EventLists/Events2005.root`, `Events2006.root`, `Events2007.root` (data)
- `EventLists/RAPGAP31/Events_RAPGAP31_20XX.root` (MC)
- `EventLists/DJANGOH14/Events_DJANGOH14_20XX.root` (MC)
- `EventLists/merged.root` (combined, all years)

### Step 4a --- Generate tree steering files

These read how many event list files exist and create one `Tree0X_N.steer`
per file, each pointing at the corresponding event list from Step 3.

```bash
cd $BASE/H1LambdaC/Steering/2005 && bash create_Tree_steerings_05.sh
cd $BASE/H1LambdaC/Steering/2006 && bash create_Tree_steerings_06.sh
cd $BASE/H1LambdaC/Steering/2007 && bash create_Tree_steerings_07.sh
# Also 2007low, 2007med
```

Verify:

```bash
ls Steering/2006/Tree06_*.steer | head -5
cat Steering/2006/Tree06_0.steer  # should reference your event list path
```

### Step 4b --- Candidate finding (batch, ~100-320 jobs each)

**Lambda candidates** (`lambdaTree.C`): loops over `H1PartLambda` candidates,
applies cuts (|eta| < 1.3, 0.5 < pT < 3.5 GeV, track start radius < 35 cm,
|DCA/dDCA| > 5, dE/dx likelihood, secondary vertex requirement, duplicate
removal), and records ~60 branches per candidate into TTrees
(`Lambda Reg`, `Lambda Bar`, `Lambda Tot`).

**K0s candidates** (`kaonTree.C`): same for `H1PartK0` candidates (pi+pi-
pairs), with analogous cuts, outputting a `K0` TTree.

**Single-job tests (interactive):**

```bash
# Lambda
$BINDIR/lambdaTree \
  -f $BASE/H1LambdaC/Steering/2006/Tree06_0.steer \
  -o $BASE/H1LambdaC/LambdaTree/2006/test_LambdaTree06_0.root

# Kaon
$BINDIR/kaonTree \
  -f $BASE/H1LambdaC/Steering/2006/Tree06_0.steer \
  -o $BASE/H1LambdaC/KaonTree/2006/test_KaonTree06_0.root
```

Verify:

```bash
root -l -b -q -e '
  TFile f("'$BASE'/H1LambdaC/LambdaTree/2006/test_LambdaTree06_0.root");
  f.ls();
  TTree* t = (TTree*)f.Get("Lambda Tot");
  if(t) cout << "Lambda Tot entries: " << t->GetEntries() << endl;
'

root -l -b -q -e '
  TFile f("'$BASE'/H1LambdaC/KaonTree/2006/test_KaonTree06_0.root");
  f.ls();
  TTree* t = (TTree*)f.Get("K0");
  if(t) cout << "K0 entries: " << t->GetEntries() << endl;
'
```

**Full batch submission:**

```bash
cd $BASE/H1LambdaC
bash SubmitScripts/submitallparticletreejobs.sh
```

Same for MC: `lambdaTreeMC.C`, `kaonTreeMC.C` over RAPGAP31 and DJANGOH14.

### Step 4c --- Generator-level MC trees (batch)

`genMCTree.C` applies the same kinematic-range selections at generator
(truth) level and outputs `genMCTree.root` with particle-level counts.
Run separately for RAPGAP31 and DJANGOH14, and for both radiative and
non-radiative DJANGOH14 samples. These are needed for efficiency
denominators and radiative corrections.

### Step 5 --- Merge all candidate trees

```bash
# Test one particle/year:
cd $BASE/H1LambdaC/KaonTree/2006
hadd -k -f merged.root KaonTree06_*.root

# Full merge:
bash $BASE/H1LambdaC/SubmitScripts/mergeallfiles.sh
```

Final merged files:
- `KaonTree/merged.root`, `LambdaTree/merged.root` (data)
- `KaonTreeMC/RAPGAP31/merged.root`, `LambdaTreeMC/RAPGAP31/merged.root` (MC reco)
- `KaonTreeMC/DJANGOH14/merged.root`, `LambdaTreeMC/DJANGOH14/merged.root` (MC reco)
- `genMCTree/RAPGAP31/Rad/merged.root` (MC gen-level)
- `genMCTree/DJANGOH14/Rad/merged.root`, `genMCTree/DJANGOH14/NonRad/merged.root`

---

## Phase 2: Local/Interactive Analysis (ROOT macros)

These read from the merged TTrees and are fast enough to run locally.
They perform signal extraction, correction, and physics measurement.

All macros called via wrapper shell scripts in `SubmitScripts/` which pass
bin edges, luminosities, input file paths, and per-bin fit initializations.

### Step 6 --- Cross section measurement

`CrossSections.C` is called via `SubmitScripts/CrossSections{Q2,Eta,Pt,X}.sh`.

Function signature:
```cpp
void CrossSections(
    int varidx,                          // 0=Q2, 1=Y, 2=pT, 3=Eta, ...
    vector<double> q2bins,               // bin edges
    double DataIntLumi06,                // data integrated luminosity (nb^-1)
    double MCDjangoh14radgen,            // DJANGOH14 rad gen-MC luminosity
    double MCDjangoh14nonradgen,         // DJANGOH14 non-rad gen-MC luminosity
    string filekaonTree,                 // data kaon merged tree
    string filelambdaTree,               // data lambda merged tree
    string filekaonTreeMC,               // MC kaon reco tree (RAPGAP)
    string filelambdaTreeMCRAPGAP,       // MC lambda reco tree (RAPGAP)
    string filelambdaTreeMCDJANGOH,      // MC lambda reco tree (DJANGOH)
    string filegenMCTreeRAPGAP,          // gen-level MC tree (RAPGAP rad)
    string filegenMCTreeDJANGOH,         // gen-level MC tree (DJANGOH rad)
    string fileNonRadgenMCTree,          // gen-level MC tree (DJANGOH non-rad)
    string fileRadgenMCTree,             // gen-level MC tree (DJANGOH rad)
    string xaxistitle,                   // plot x-axis label
    vector<vector<double>> binparaminitsLambda,   // per-bin fit initializations
    vector<vector<double>> binrangedefsLambda,    // per-bin signal extraction windows
    vector<vector<double>> binparaminitsKaon,
    vector<vector<double>> binrangedefsKaon,
    vector<vector<double>> binparaminitsMCLambda,
    vector<vector<double>> binrangedefsMCLambda,
    vector<vector<double>> binparaminitsMCKaon,
    vector<vector<double>> binrangedefsMCKaon
)
```

Internally, for each kinematic bin this:
1. Extracts signal via RooFit composite fit (Student's t signal + power-law
   exponential background) to the invariant mass spectrum, with sideband
   subtraction in defined windows
2. Computes efficiency: N_reco(MC) / N_gen(MC) using RAPGAP trees
3. Computes radiative corrections: DJANGOH Rad / DJANGOH NonRad gen-level,
   scaled by luminosity
4. Computes cross section:
   `dsigma/dX = N_signal / (eff * BR * Lumi * rad_corr * dX)`
   where BR(Lambda->p pi) = 0.639, BR(K0s->pi pi) = 0.692

**Compile check (no data needed):**

```bash
root -l -b -q -e '.L CrossSections.C+'
```

**Run:**

```bash
cd $BASE/H1LambdaC/SubmitScripts
bash CrossSectionsQ2.sh    # produces AnalysisResultFiles/CrossSectionsQ2_All.{root,txt}
bash CrossSectionsEta.sh
bash CrossSectionsPt.sh
bash CrossSectionsX.sh
```

**Verify:**

```bash
root -l -b -q -e '
  TFile f("'$BASE'/H1LambdaC/AnalysisResultFiles/CrossSectionsQ2_All.root");
  f.ls();
'
```

### Step 7 --- Asymmetry measurement

`Asym.C` has the same signature as `CrossSections.C` but with
Lambda/LambdaBar fit initializations instead of Lambda/Kaon.

For each bin, it fits Lambda and Anti-Lambda invariant mass spectra
separately, then computes:
```
A = (N_Lambda - N_AntiLambda) / (N_Lambda + N_AntiLambda)
```

**Run:**

```bash
cd $BASE/H1LambdaC/SubmitScripts
bash AsymQ2.sh             # produces AnalysisResultFiles/AsymQ2_All.{root,txt}
bash AsymEta.sh
bash AsymPt.sh
bash AsymX.sh
bash AsymIntegrated.sh     # single-bin inclusive asymmetry
```

### Step 8 --- Stopping rates

```bash
bash StoppingRates.sh
bash StoppingRatesIntegrated.sh
```

### Step 9 --- Supporting studies

These are zero-argument macros with hardcoded input file paths at the top
of each `.C` file. Update paths before running.

```bash
cd $BASE/H1LambdaC

# Radiative corrections (DJANGOH Rad vs NonRad gen-level comparison)
root -l -b -q 'RadCorr.C'                  # -> RadCorr.root

# Bin migration matrices and purity studies from MC
root -l -b -q 'MigrationPlots.C'           # -> MigrationPlots.root

# Trigger efficiency
root -l -b -q 'TriggerEff.C'               # -> TriggerEff.root

# DIS kinematic control distributions
root -l -b -q 'DISControlPlots.C'          # -> DISControlPlotsTEST.root

# Final combined result overlay with H1 2009 publication
root -l -b -q 'TotalResults.C'             # -> TotalResults.root
```

### Step 10 --- Validate physics output

```bash
root -l $BASE/H1LambdaC/AnalysisResultFiles/CrossSectionsQ2_All.root
# In ROOT: TBrowser b;
```

---

## Minimal Smoke Test (compile-only, no data needed)

```bash
cd $BASE/H1LambdaC

# 1. Build executables
make create_evls lambdaTree kaonTree

# 2. Compile-check all analysis macros
for macro in CrossSections.C Asym.C AsymIntegrated.C RadCorr.C \
             MigrationPlots.C TriggerEff.C DISControlPlots.C \
             TotalResults.C StoppingRates.C; do
  echo "Compiling $macro..."
  root -l -b -q -e ".L $macro+" 2>&1 | tail -1
done
```

Pass if every macro compiles with no errors.

---

## What Runs Where

| Step | Code | Where | Time |
|------|------|-------|------|
| 1. Build | `make` | Local (H1 env) | Minutes |
| 2. Steering files | `create_*_steerings_0X.sh` | Local | Seconds |
| 3. Event lists | `create_evls` | **HTCondor** (~150-320 jobs) | Hours |
| 3b. Merge event lists | `mergeallfiles.sh` (top section) | Local | Minutes |
| 4a. Tree steerings | `create_Tree_steerings_0X.sh` | Local | Seconds |
| 4b. Candidate trees | `lambdaTree`, `kaonTree` | **HTCondor** (~100-320 jobs) | Hours |
| 4c. Gen-level MC | `genMCTree` | **HTCondor** | Hours |
| 5. Merge all trees | `mergeallfiles.sh` (all sections) | Local | Minutes |
| 6. Cross sections | `CrossSections.C` | Local `root -l -q` | Minutes |
| 7. Asymmetries | `Asym.C` | Local `root -l -q` | Minutes |
| 8. Stopping rates | `StoppingRates.C` | Local `root -l -q` | Minutes |
| 9. Support plots | Various `.C` macros | Local `root -b` | Minutes |

---

## Hardcoded Paths

All scripts and source files use two path prefixes that must be updated
for your working area:

| Old prefix | Used in |
|------------|---------|
| `/data/dust/group/h1/gtustin/h1oo/` | Most files |
| `/nfs/dust/h1/group/gtustin/h1oo/` | Some steering scripts and `.C` macros |

Global find-and-replace:

```bash
MYPATH="/data/dust/group/h1/yourusername/h1oo"
find . -not -path './.git/*' -type f | xargs sed -i \
  "s|/data/dust/group/h1/gtustin/h1oo|${MYPATH}|g; \
   s|/nfs/dust/h1/group/gtustin/h1oo|${MYPATH}|g"
```

Key live-code paths to verify (not just in scripts but in C++ source):
- `create_evls.C` line 1572: `goodRunFileName` (SelectedRuns.root)
- All zero-argument macros (`RadCorr.C`, `MigrationPlots.C`, `TriggerEff.C`,
  `DISControlPlots.C`, `TotalResults.C`): hardcoded file strings at top of file

---

## Key Physics Constants

- Data integrated luminosity (combined 2005-2007): 297,657.8 nb^-1
- DJANGOH14 rad gen-MC luminosity: 4,108,957.0 nb^-1
- DJANGOH14 non-rad gen-MC luminosity: 3,560,233.0 nb^-1
- BR(Lambda -> p pi): 0.639
- BR(K0s -> pi+ pi-): 0.692
- Q2 binning: {5, 7, 10, 15, 25, 100} GeV^2
- eta binning: {-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3}
- x binning: {0.00004, 0.0001, 0.0002, 0.0004, 0.001, 0.01}
- pT binning: {0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5} GeV
