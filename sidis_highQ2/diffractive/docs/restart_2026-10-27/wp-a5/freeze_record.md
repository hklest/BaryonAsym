# WP-A5 freeze record: even-split forward-veto correction

Child plan A5-D1 (`.supervisor/working/plans/2026-10-28-wp-a5-veto-signal-loss-correction.md`).

- Frozen file: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a5/veto_correction_even.json` (made read-only)
- sha256: `c026604e29a02f854c10dd2c773f47534d5b807c3a10eccdfa10687327f52dad`
- Frozen at (UTC): 2026-10-28T06:17:56Z
- Produced by: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a5/derive_veto_corr.py --split even` from
  `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a5/veto_corr_hists.json` (sha256 `6b3e0e7825cb2ce049b77f0cfe5fc46384f0036c76ae4627eceacdbebc41a418`, Condor cluster 5931280).
- State at freeze: no odd-split per-(M_X bin, D cell) value had been computed or read. The odd-split integrated
  numbers published in `gate_evidence/veto_eff.json` (ALL / tertile granularity) were known before this package; the freeze
  is procedural, not blind (A5-D1).
- Rule: the JSON is never edited (`corrections_are_definitions`); errata go into this file.
