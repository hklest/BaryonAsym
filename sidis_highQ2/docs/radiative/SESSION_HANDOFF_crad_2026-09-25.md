# C_rad session handoff — 2026-09-25

High-Q2 charged-hadron SIDIS (`Q2 > 150 GeV^2`) gen-only radiative correction.
`c_rad_applied=false`. Central release untouched and still raw / not Born-level.

## Headline

The shipped run3 `C_rad` (integrated h+ `0.6770210`, h- `0.8509564`) was **DEFECTIVE**.
Root cause: the run3 RAD accumulation omitted the `IsGtr != 0` gate, admitting GEANT
STR material secondaries into the RAD numerator. Restoring it gives

| product | integrated h+ | integrated h- |
|---|---|---|
| run3 (defective, superseded) | `0.6770210` | `0.8509564` |
| **V0 corrected (current)** | **`1.14063313`** | **`1.14017023`** |

Proof: full-sample variant scan V5 (= V0 minus `IsGtr`) reproduced the broken value at
h+ `0.6770678` / h- `0.8510136`, matching run3 to `0.007%`.

## Current best product

`/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_v0_isgtr_20260925/results/crad_truth_v2_490rows.csv`
490 rows, cluster `5240845`, 1h48m, all 2,953 files. summary.json alongside.

- finite `490/490`; median `1.15608`, min `0.85886`, max `1.55822`
- `166` bins outside `[0.5,1.2]`; `23` outside `[0.8,1.4]`
- rel-stat median `0.004238`, max `0.057679`
- charge split now `0.04%` (spurious asymmetry gone)
- residual trends NOT flat: median C_rad per z (h+) `1.125,1.136,1.139,1.145,1.183,1.221,1.277`;
  per pT_h (h+) `1.267,1.221,1.149,1.123,1.098`

Producer: `scripts/radiative/crad_truth_v0_isgtr_20260925/produce_crad_v0_isgtr.py`
Canonical plotter reused unmodified: `scripts/radiative/crad_truth_v2_20260923/plot_crad_truth_v2_7col.py`
(takes `--input`, `--outdir`, `--png`; self-test `SELF_TEST_OK`).
Original producer `scripts/radiative/crad_truth_v2_20260923/produce_crad_truth_v2.py`
md5 `b394e0f624270c0299bffce1e54f9a02`, deliberately unmodified.

## V0 selection (the convention that gives 1.14)

```
finite(weight1,weight2,Q2_es_gen,Y_es_gen,x_es_gen,GenEmpz,zGen,pT_h,xFGen)
150 < Q2_es_gen < 20000;  0.1 < Y_es_gen < 0.7;  GenEmpz > 45;  xFGen < 0
|GenPDG| in {211,321,2212};  IsGtr != 0;  Mother2 == -1;  IsV0DaughterGen == 0
0 <= zGen <= 1;  0 <= pT_h <= 8
weight = weight1*weight2;  charge from GenCharge (int32, +-1)
per beam: sigma_NONRAD=(sumw_OLD+sumw_NEW)/(L_OLD+L_NEW), sigma_RAD=sumw_RAD/L_RAD
sigma_mix = f_ep*sigma_ep + (1-f_ep)*sigma_em,  f_ep = 0.5440407387464388
C_rad = sigma_mix_NONRAD / sigma_mix_RAD
```
`IsGtr != 0` is MANDATORY. Dropping it is the known defect.

## GenEmpz scan (last completed work)

`data/radiative/crad_empz_scan_20260925/scan_results.csv`; plot
`plots/radiative/crad_empz_scan_2026-09-25/crad_vs_genempz_2026-09-25.pdf` (+PNG).
T=45 validated against the V0 product to `+6.5e-05`.

| T [GeV] | C_rad h+ | C_rad h- | RAD retained | NONRAD retained |
|---|---|---|---|---|
| none/0 | `1.10597` | `1.10581` | `1.000000` | `1.000000` |
| 20 | `1.10814` | `1.10792` | `0.998070` | `1.000000` |
| 35 | `1.11830` | `1.11795` | `0.989056` | `1.000000` |
| 40 | `1.12640` | `1.12599` | `0.981971` | `1.000000` |
| **45** | **`1.14070`** | **`1.14024`** | `0.969683` | `1.000000` |
| 47.5 | `1.15262` | `1.15212` | `0.959665` | `1.000000` |
| 50 | `1.17160` | `1.17105` | `0.944136` | `1.000000` |

Key fact: **NONRAD retention is exactly `1.000000` at every threshold through 50.**
No NONRAD event lies below 50 GeV in E-pz. The cut only ever removes RAD yield, so
`C_rad` inherits a direct dependence on it: `1.10597/0.9697 = 1.1406`. Total swing
no-cut to >50 is `+5.9%`. Smooth and monotonic, no plateau.
Decomposition of RAD `GenEmpz<=45`: `60.81%` removed by `xFGen<0`, `0.30%` by pT_h
range, `0%` by zGen range; a real ~3% remainder survives to be cut by GenEmpz.

## Hypotheses tested and CLOSED (do not redo)

- **Sparse bins / low stats** — RESOLVED. `490/490` finite, min occupancy `279`,
  rel-stat median `0.42%`, max `5.74%`. Extreme bins do NOT sit at low N_eff
  (extremes harmonic N_eff median `36,158.6` vs all-bin `57,078.4`).
- **RAD h+/h- = 1.2771 charge asymmetry** — ARTIFACT of the missing `IsGtr` gate.
  True full-selection value is `1.0157`(RAD), `1.0161`(OLD NR), `1.0163`(NEW NR).
  Direct single-file check: `1.015357`. Without `IsGtr`: `1.2656606`.
- **RAD reco-conditioning** — REFUTED. RAD files hold exactly `100,000` GenDIS
  events each (round generator-job counts); HAT=MODS=H1Tree=GenDIS=90,000 for
  8956/0000; steering has no `fEventListFiles`/eventlist; runtime log says
  "No HAT selection". Forward-eta depletion tracks RADIATIVENESS not acceptance:
  `|eta|>3` fraction `0.0152926` at `Eg=0` -> `0.0083806` at `Eg>=5`.
  Detector *simulation* happened (hence STR particles) but events were not
  filtered on reconstruction. `IsGtr!=0` removes STR cleanly.
- **Scattered e+/e- leaking in** — REFUTED. Builder requires
  `|PDG| in {211,321,2212}`; electron is PDG 11. No PDG/charge mismatch path.
- **Beam-remnant proton xF sign flip** — REFUTED. `0/129184` selected RAD protons
  flip xFGen sign under ISR-corrected q.
- **NONRAD exposure convention** — REFUTED as the driver. Four alternative NONRAD
  exposure maps all leave C_rad at `0.64-0.68`(h+).
- **Track vs event level exposure** — tested, five conventions; none put both
  charges in range; charge ratio pinned at `0.7956` in all of them (pre-IsGtr-fix).
- **ISR effective-beam boost bug** — REAL but tiny. Builder line 209 uses nominal
  `GenBeamElecE` for `kin` while lines 200-204 use `eeff = GenBeamElecE - GenEnPhoton`.
  Corrupts zGen/pT_h/phi_h/xFGen for radiative events, but only `+0.039%` on C_rad
  for rows passing selection. NOT fixed; no v3 builder written.
- **Q2GT60 vs Q2GT100 domain mismatch** — real but immaterial. V1 (`Q2sGen>100`)
  gives `1.1406978`, identical to V0 to 7 digits.

## Open questions

1. Is `1.14` acceptable, or is the `~1.05` target right? The GenEmpz dependence
   means the answer depends on whether the E-pz cut should match the experimental
   cut (user asserts it should, to put C_rad on the same footing). If so `1.14`
   is what that selection gives and `1.05` is not the benchmark.
2. Residual z-rising / pT_h-falling trend — physical QED or leftover mismatch?
3. NONRAD MC exposure provenance still `DIAGNOSTIC_GENDIS_EVENT_LEVEL`, not
   publication-grade. RAD exposure IS traced (oomclumi PASS 1373/1373,
   e+ `16206393.0`, e- `13534017.0` nb^-1).
4. e- GenDIS count shortfall: retained `61,199,996` vs `642 x 100,000 = 64,200,000`
   = `4.67%`; e+ only `0.41%`. Partial chunks or real attrition, unresolved.
5. 490-bin product should be regenerated at any newly agreed selection.
6. Canonical 7-panel plotter does not stamp the mandatory
   "DIAGNOSTIC - C_rad NOT APPLIED" / source / date annotation. Needs an approved patch.

## Environment gotchas (cost real time this session)

- `uproot` lives in AFS home (`/afs/desy.de/user/k/klesthen/.local/lib/python3.9/site-packages`).
  Some workers have no AFS -> `ModuleNotFoundError: No module named 'uproot'`.
  FIX IN PLACE: DUST-resident copy at
  `scripts/radiative/crad_empz_scan_20260925/pylibs` (needs uproot, awkward,
  awkward_cpp, fsspec, cramjam, numpy, xxhash). Wrapper `run_stage1_dust.sh` sets
  `PYTHONPATH` to it and unsets `PYTHONHOME`. Reuse this for all future Condor python.
- `set -u` in a wrapper breaks `/afs/desy.de/group/h1/scripts/h1set.sh` line 118
  ("HOST: unbound variable").
- `getenv=True` poisons PYTHONHOME/PYTHONPATH so `/usr/bin/python3` cannot import encodings.
- Wildcard paths in submit files fail; enumerate files explicitly.
- `pdftoppm` absent on workers, present at `/usr/bin/pdftoppm` on login node. Rasterize there.
- Login node is 5 min / one file. Multi-file aggregation over 2,953 JSONs takes
  ~5 min and MUST be launched with `setsid nohup ... & disown` or it gets killed.
- Input ntuples use `ep/` and `em/` SUBDIRECTORIES; flat globs silently return nothing.
- 743 HELD jobs in the queue belong to a DIFFERENT analysis (`H1SIDIS_LowQ2_v5g`). Leave alone.

## Inputs

Tree `ChargedGenRC`, all under
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output/`
- RAD `rad_retained_v2_20260923/ntuples/{ep,em}/` — 731 ep, 642 em (components 8955-8962, RAD.Q2GT60.CTEQ6L, SIGMT 15040.275-15088.075 pb)
- OLD NONRAD `old_nonrad_v2/ntuples/{ep,em}/` — 260, 260 (NORAD.Q2GT100.CTEQ6L, SIGMT e+ 7287.9104 pb)
- NEW NONRAD `alpha0p2_truth_v2_20260922/ntuples/nonrad_new/{ep,em}/` — 530, 530

Exposures (nb^-1): rad_ep `16206393.0`, rad_em `13534017.0`,
old_nonrad_ep `12920115.8547021`, old_nonrad_em `12841572.4415535`,
new_nonrad_ep `1717376.821314504`, new_nonrad_em `1699702.770664394`.

## Artifacts written 2026-09-25

docs/radiative/: `crad_normalization_forensics`, `crad_track_vs_event_exposure`,
`crad_truth_v2_diagnosis_and_gates`, `infiducial_boost_impact`,
`rad_conditioning_resolution`, `rad_hplus_proton_excess`, `rad_reco_bias_test`,
`rad_sample_reco_filter_provenance`, `rc_builder_v3_isr_boost_fix`,
`research_rad_pth_tail_rootcause`, `unconditioned_truth_access` (all `_2026-09-25.md`).

data/radiative/: `crad_v0_isgtr_20260925` (BEST PRODUCT), `crad_empz_scan_20260925`,
`crad_variant_scan_20260925` (17 variants V0-V16), `crad_branch_audit_20260925`,
`crad_normalization_forensics_20260925`, `crad_track_vs_event_20260925`,
`rad_conditioning_resolution_20260925`, `rad_reco_bias_test_20260925`, others.

plots/radiative/: `crad_v0_isgtr_7col_2026-09-25` (4pp canonical 7-panel, PDF+PNG),
`crad_empz_scan_2026-09-25` (threshold scan), `crad_charge_asymmetry_2026-09-25`
(84pp h+/h- kinematics, full 2953 files, cache in `cache/`),
`crad_truth_v2_run3_diagnostic_2026-09-25` (3pp, of the superseded product).

## Suggested next step

Decide the GenEmpz convention question (item 1) since everything downstream scales
with it, then close NONRAD exposure provenance. Do not rerun the closed hypotheses.
