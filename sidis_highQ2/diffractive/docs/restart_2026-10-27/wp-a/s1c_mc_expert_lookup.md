# WP-A S1(c) MC-expert lookup (2026-10-27, h1-mc-expert; read-only)

Basis: D/docs/t2/mc_randomtrigger_overlay_determination_2026-10-22.md (§0,§2), re-checked today against
/afs/desy.de/group/h1/iww2/imc/steering/blocks/{OSRN,fpack_noise.*} and steering_files/mcrequest_<ReqID>.

## Q1. Are the RT pool files readable today?  YES (HIGH confidence)
`ls -l` + `head -c1 >/dev/null` on /pnfs/desy.de/dphep/online/h1/noise/ : LARNOISE.RUN473317 (943 MB),
LARNOISE.RUN403791 (890 MB), LARNOISE.RUN451971 (613 MB), LARNOISE.RUN365321 (1.06 GB), CPCNOISE.2006_2 (26 MB):
all listed (dated 2014-01-30, group h1) and READ_OK. Not opened with ROOT/H1OO.
Caveat: these are LARNOISE/SPNOISE/CPCNOISE files (FPACK, need libdcap/ODS tooling, absent on Alma9 per
project notes). No PLUG/FMD/FTS-named pool file exists in that directory; the FMD/PLUG/FTS/FNC noise is NOT a
separate pool file in the steering (see Q2). Do not count on re-deriving a data-side forward-stack RT noise
rate from these files on this node.

## Q2. Does RT_NOISE=Y imply Plug ENERGY noise?  YES, probable (MEDIUM-HIGH; mechanism source-backed, energy-sum detail inferred)
- blocks/OSRN word 9 "PLUG @ HERA-2", module h1rec/pscale, = 1; word 8 FMD h1sim/mdigi = 1; word 7 FTS incl. PRT
  h1rec/dstmak = 1; word 10 FNC h1rec/fncrec = 1. All 1 in every v6dif request (mk_steer lines 44-47: RT_NOISE=Y -> OSRN included).
- h1simrec_VT.html line 282 (VT-45/46/48/49): "Implementation of new RT-noise simulation for FMD, PLUG and FNC detectors";
  FMD fixes VT-50..60 (lines 237-273); FTS RT-noise VT-43/44 (l.308), PRT VT-42 (l.318). Line 265: PLUG reco bug fix
  "read correct database bank for MC case". SW_VERSION 84/72/70 of all components >> VT-60.
- Nature of PLUG noise: it is applied at the REC step in `pscale` (the calorimeter energy-scale/calibration module),
  i.e. on cell/energy level, not as a trigger bit. FMD is applied at the digi step (mdigi) i.e. hit level; FTS at dstmak.
  So noise enters reconstructed PLUG energy, hence ePlug. The docs I could read do not spell out "cell energies vs
  thresholds"; I have not read pscale source (not found under imc/). That sub-point is PROVISIONAL.
- Empirical support (earlier note §2.3): 9012 smoke: ePlug mean 1.74 GeV (elastic MC, truth Plug empty), 5.3% fail
  ePlug>=7; 25.4% fail FMD; 2.5% fail FTS. Elastic MC without overlay would be near zero, so overlay energy reaches ePlug.
- Fidelity: the MC noise comes from a handful of real RT runs per period (LAr pools: 4 runs 06_07e+, 4 for 04_05_e-,
  2 for 2006_e-, 4 for 03_04_e+), so it is a snapshot; it does not track luminosity-dependent / synchrotron / beam-gas
  variation across the period. This is a plausible origin of the data > MC excess (0.207/0.230/0.283 vs <=0.146) beyond
  noise-level mismatch, i.e. part of the excess may be beam-related activity not in the RT snapshot. The data-vs-MC
  deep-gap control measurement is the right tool; do not assume the RT overlay is quantitatively right.

## Q3. Period structure per component (VERIFIED from mcrequest_<ReqID> RUN_PERIOD/RUNS, RT_NOISE: Y for all)
| ReqID | sim period | RUNS | noise pool (fpack_noise.*) |
| 9012, 9024, 9158, 6677, 7091, 7094 | 06_07e+ | 468530-500611 | LARNOISE RUN473317/485050/492632/496510 |
| 9033, 9155, 6671 | 04_05_e- | 398376-436893 | RUN403791/419474/423936/436268 |
| 7096 | 04_05_e- | 399629-436893 | same |
| 9157, 7071 | 2006_e- | 444307-466997 | RUN451971/466278 |
| 7097 | 2004_e+ (03_04_e+ pool) | 367284-392213 | RUN365321/369144/375086/382483 |
Confirmed: 7097 is the only component carrying the 03_04_e+ pool (pool name taken from fpack_noise.03_04_e+; I did not
open mcrequest_7097's OPEN lines, so the 7097->03_04_e+ pool link is by period name; HIGH). All six 06_07e+ components share one pool set.
Note 9024/9033/9158/6677/6671 are the carriers of the borrowed periods (no ep0304 sim for 9024/9158/6677; no em06 sim for
9033/6671), so ep0304 and em06 projections carry the 06_07e+ / 04_05_e- noise respectively.

Is composite-scale projection a sound basis for a per-data-period excess?  PARTIALLY.
- Sound as a book-keeping of WHICH data periods a component stands for (exposure_factor), and as the weight for the
  MC-side expected fail fraction per represented period: the removal probability p_p = max(0, f_data,p - f_MC,p)/(1-f_MC,p)
  in the control region, applied per represented period, preserves the correct data-period mix of the luminosity weights.
- Caveat (period mismatch): the MC event's actual overlaid noise is from its SIM period (e.g. a 9012 event "representing"
  em0405 carries ep0607 RT noise and ep0607 run-dependent ANOF sub-period assignment; 9024/9158/6677 representing ep0304
  carry ep0607 noise; 9033/6671 representing em06 carry 04_05_e- noise; 9012 is ONE stream projected to ALL FOUR periods,
  fully correlated). Therefore the MC fail fraction f_MC,p must be measured per COMPONENT-by-SIM-period (MC as simulated),
  while f_data,p is per DATA period; the excess for a represented period p is f_data,p minus the f_MC of the stream that is
  projected onto p (not minus a p-specific MC). This remains valid only if the control-region MC fail fraction is
  evaluated on the same stream. A residual period-mismatch systematic = difference of f_MC between sim-period streams
  (e.g. 9012 vs 9157-type 2006 e-) in the same control region; vary the assignment (own sim period vs represented period)
  as a systematic. Also: beam-induced (not RT-overlayed) forward activity differs between e+ and e-, 2004 vs 2007.
- Hand-off: provenance/ID details to h1-mc-db; weights/exposure_factor semantics to h1-normalization.
