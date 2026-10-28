# WP-D2 validation of merged v6dif RAPGAP 840x trees (2026-10-28)
Script: scripts/restart_2026-10-27/wp-d2/validate_840x.py; Condor cluster 5961008 (1 job, rc 0, ~2 min wall, 12 s CPU, 50 MB used of 8 GB).
Results: data/restart_2026-10-27/wp-d2/validation_840x.json. Inputs: data/t2_detector/mc840x_merged_2026-10-28/{8402..8405}.root; ref 9012.root.

| # | Check | Verdict |
|---|-------|---------|
| 1 | Integrity: 4/4 open, sha256 match, no zombie/recovered, DiffTot/ChargedTot/ChargedTotGen all single-cycle | PASS |
| 2 | DiffTot 401281/401661/402098/401708 == tally == record; passRecoEnv 68492/68932/68735/67545 == tally; 402/402 chunks rc=0, PASS (100/100/101/101) | PASS |
| 2b | ChargedTot entries 572042/566427/568560/556449; ChargedTotGen 4974128/4977962/4983284/4979304 | recorded |
| 3 | Branch parity vs 9012 (DiffTot 96, ChargedTot 100, ChargedTotGen 60): 0 missing, 0 extra, 0 type diffs, all 4 files | PASS |
| 4 | weight1 == 1 (min=max=1), weight2 min=max=1 (skim multiplies weight1*weight2), (Run,Event) unique (also EventNumber alone unique) | PASS |
| 5 | Run ranges (see below) | PASS with note |
| 6 | Sentinel (<=-900/non-finite) on reco-env: 0 of 92 numeric branches above 5 % in 840x and in 9012 | PASS |
| 7 | Twins (first 30000 entries, EventNumber match): 8402/8405 99.95 %, 8403/8404 99.94 %; hatMXGen/hatTGen/hatXpomGki agree for 100 % of matched | PASS |

Run ranges (min-max; hatVtxZ mean/rms all / reco-env, cm):
8402 357149-392201 (1.70/16.5; 2.84/11.2); 8403 398378-436886 (1.52/16.4; 2.00/11.1);
8404 444307-466997 (1.36/16.2; 1.96/10.6); 8405 468532-500611 (0.77/15.6; 1.04/9.5).

Notes: (a) 8402, 8403, 8404 run minima lie 5-11 below the plan window lower edge (357160, 398383, 444312), 8402 max 392201 < 392213 ok; the "expected" narrow
sub-ranges are not a bound (8404 spans the full 2006 range, not only 444315-447893). Harmless if the later GRL/period cut is applied; flag only.
(b) Twin event order is not positional (13-17 % equal at same index; merged-chunk ordering differs), so matching is by EventNumber; ~0.05 % of the first
30000 have no partner in the first 30000 of the twin (ordering truncation, not mismatch). Reco Q2s and passRecoEnv differ between twins (93.6 % agree)
as expected from the different period overlay.
(c) Merged files carry 1105/1116 per-chunk metadata keys (11 per chunk) vs 269 in 9012: expected for chunk-merged files.
