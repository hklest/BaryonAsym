WP-A4 (2026-10-27): kinematic scaling of forward data/MC mismatch + MC LAr-edge response. PREPARED, NOT SUBMITTED.
Expected cost: skims 17 jobs ~0.004-0.006 Condor-days (wp-a3 skim 0.00378, +4 small branches), analysis 1 job <0.001 -> total ~0.007 (cap 0.2).
Order: (1) condor_submit skim_wpa4.sub ; (2) after ALL 17 done (17 npz in data/restart_2026-10-27/wp-a4/skims): condor_submit kin_scaling_wpa4.sub. Output data/restart_2026-10-27/wp-a4/kin_scaling.json.
composition_wpa4 (follow-up, 1 job, reads the 17 skims, needs no new skims): condor_submit composition_wpa4.sub ; expected <0.001 Condor-days. Output data/restart_2026-10-27/wp-a4/composition.json
