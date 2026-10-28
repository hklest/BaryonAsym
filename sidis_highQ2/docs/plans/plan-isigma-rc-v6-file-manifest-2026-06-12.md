# Approved Plan — High-Q² v6 I-Sigma C_rad File Manifest and Manifest-Matched Lumi Table

## Scope

Create one standalone Python script that inventories exact RAD and NonRad input file numbers for high-Q² v6 I-Sigma `C_rad`, parses the four Britzger `g1j_ES` GenLumi steerings, and emits a manifest plus an optional corrected MC lumi table only when file-number-to-lumi matching is complete and non-ambiguous.

## Paths

- Local script directory: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative_corrections/isigma_rc_diag_v6_2026-06-11/`
- Local script: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative_corrections/isigma_rc_diag_v6_2026-06-11/build_file_manifest_vs_steering.py`
- Remote script/output directory: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/radiative_corrections/isigma_rc_diag_v6_2026-06-11/full_norad_v1_lumi/`
- Remote script: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/radiative_corrections/isigma_rc_diag_v6_2026-06-11/full_norad_v1_lumi/build_file_manifest_vs_steering.py`
- Required workspace outputs:
  - `input_file_manifest_vs_steering.json`
  - `input_file_manifest_summary.csv`
  - `mc_lumi_period_table_manifest_corrected.json` only if exact corrected lumis are non-ambiguous.

## Implementation Plan

1. Create the local standalone script using only Python stdlib imports:
   ```python
   from pathlib import Path
   import glob
   import re
   import json
   import csv
   ```
2. Hard-code the workspace output directory, RAD DJ/RG exact input directories, NonRad DJ/RG globs, four `g1j_ES` GenLumi steering paths, and the existing `mc_lumi_period_table.json` path exactly as supplied in the prior prompt. Do not discover with recursive `find`.
3. Implement file ID parsing with explicit regexes for known file-name patterns. For each `(generator, mode, period)` record:
   - store sorted exact paths;
   - store parsed numeric IDs;
   - store `count`, `min_id`, `max_id`, `ids`, and any `unparsed_paths`;
   - fail acceptance if expected file counts are not met.
4. Expected counts to encode and validate:
   ```python
   EXPECTED_COUNTS = {
       ("djangoh", "rad", "all"): 501,
       ("rapgap", "rad", "all"): 364,
       ("djangoh", "nonrad", "ep0304"): 14,
       ("djangoh", "nonrad", "e0607"): 14,
       ("djangoh", "nonrad", "p0607"): 14,
       ("djangoh", "nonrad", "lumi06e"): 14,  # replace period labels with prior-prompt labels if different
       ("rapgap", "nonrad", "ep0304"): 16,
       ("rapgap", "nonrad", "e0607"): 16,
       ("rapgap", "nonrad", "p0607"): 16,
       ("rapgap", "nonrad", "lumi06e"): 16,  # replace period labels with prior-prompt labels if different
   }
   ```
   If the prior prompt used different four-period labels, use those exact labels consistently in manifest keys and CSV rows.
5. Parse each GenLumi steering by reading bounded raw text only from relevant lines matching labels and `fLumi` assignments. Capture:
   - steering path;
   - line number;
   - raw line text;
   - current relevant sample/label context;
   - parsed `fLumi` value;
   - parsed chunk/file ID candidates.
6. Match NonRad manifest IDs to steering chunks only when the relationship is one-to-one and complete for that `(generator, period)`:
   - every manifest ID has exactly one steering `fLumi`;
   - no steering chunk maps to multiple manifest IDs;
   - no unmatched manifest IDs;
   - no ambiguous duplicate ID candidates.
7. Compare existing `mc_lumi_period_table.json` to manifest-derived lumis when complete. Include both current and manifest-derived per-generator/per-period values plus absolute and relative deltas in the JSON.
8. Always write:
   - `input_file_manifest_vs_steering.json`
   - `input_file_manifest_summary.csv`
9. Write `mc_lumi_period_table_manifest_corrected.json` only when all exact NonRad matches are complete and non-guessed. If not, include `corrected_lumi_table_written: false` and blocking reasons in the manifest JSON.

## JSON Schema Expectations

Top-level `input_file_manifest_vs_steering.json` keys:

```json
{
  "created_by": "build_file_manifest_vs_steering.py",
  "purpose": "highQ2 v6 I-Sigma C_rad exact file-number manifest and manifest-matched lumi audit",
  "remote_output_dir": ".../full_norad_v1_lumi",
  "inputs": {
    "rad_dirs": {},
    "nonrad_globs": {},
    "steering_files": [],
    "current_lumi_table": ".../mc_lumi_period_table.json"
  },
  "manifest": {},
  "summary_rows": [],
  "steering_parse": {
    "relevant_raw_lines": [],
    "parsed_lumi_records": [],
    "match_results": {}
  },
  "lumi_comparison": {
    "current_table_loaded": true,
    "manifest_derived_lumis": {},
    "current_lumis": {},
    "deltas": {}
  },
  "validation": {
    "expected_counts": {},
    "count_checks": [],
    "unparsed_paths": [],
    "ambiguities": [],
    "corrected_lumi_table_written": false,
    "corrected_lumi_table_path": null
  }
}
```

CSV columns:

```text
generator,mode,period,count,expected_count,count_ok,min_id,max_id,ids_json,unparsed_count,first_path,last_path
```

Corrected lumi table schema: same top-level shape and keys as existing `mc_lumi_period_table.json`; only values supported by complete manifest-to-steering matching may be changed.

## Remote Transfer and Run

1. Ensure NAF ControlMaster is alive before NAF work:
   ```bash
   ```
   Stop if it fails; do not reconnect or retry.
2. Create the remote directory if needed and transfer the script:
   ```bash
   ssh naf-hone.desy.de 'mkdir -p /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/radiative_corrections/isigma_rc_diag_v6_2026-06-11/full_norad_v1_lumi'
   scp /data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative_corrections/isigma_rc_diag_v6_2026-06-11/build_file_manifest_vs_steering.py naf-hone.desy.de:/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/radiative_corrections/isigma_rc_diag_v6_2026-06-11/full_norad_v1_lumi/
   ```
3. Run bounded metadata-only inventory on NAF:
   ```bash
   ssh naf-hone.desy.de 'cd /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/radiative_corrections/isigma_rc_diag_v6_2026-06-11/full_norad_v1_lumi && python3 build_file_manifest_vs_steering.py'
   ```
   This must not process ROOT events and must not use recursive `find`.

## Acceptance Criteria

- `input_file_manifest_vs_steering.json` exists in the workspace output dir and is nonzero size.
- `input_file_manifest_summary.csv` exists in the workspace output dir and is nonzero size.
- Manifest count checks pass for RAD DJ = 501 and RAD RG = 364.
- Manifest count checks pass for NonRad DJ = 14 files per period and NonRad RG = 16 files per period.
- JSON includes `steering_parse.relevant_raw_lines`, `steering_parse.parsed_lumi_records`, and `steering_parse.match_results`.
- JSON records all unparsed paths and all matching ambiguities; no silent drops.
- `mc_lumi_period_table_manifest_corrected.json` is written only if matching is exact, complete, and non-ambiguous; otherwise it is absent and the manifest explains why.
- No event processing, no ROOT reading, no recursive filesystem traversal, and no Condor submission are used.

## Todo

- [ ] Create local directory `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative_corrections/isigma_rc_diag_v6_2026-06-11/` if absent.
- [ ] Write `build_file_manifest_vs_steering.py` with only `pathlib`, `glob`, `re`, `json`, and `csv` imports.
- [ ] Fill constants for exact RAD DJ/RG dirs, NonRad globs, four `g1j_ES` steering files, current lumi table, and output filenames from the prior prompt.
- [ ] Implement deterministic file collection and numeric ID parsing for RAD and NonRad files.
- [ ] Implement count/min/max/list manifest records per `(generator, mode, period)`.
- [ ] Implement expected-count validation for RAD DJ 501, RAD RG 364, NonRad DJ 14/period, and NonRad RG 16/period.
- [ ] Implement bounded steering parsing for relevant labels, raw lines, `fLumi`, and chunk/file ID candidates.
- [ ] Implement exact NonRad manifest-ID to steering-lumi matching with ambiguity detection.
- [ ] Implement comparison against existing `mc_lumi_period_table.json` when available.
- [ ] Always write manifest JSON and summary CSV in the workspace output directory.
- [ ] Write corrected lumi JSON only when exact matching is complete and non-ambiguous.
- [ ] Transfer the script to the workspace output directory after checking SSH ControlMaster.
- [ ] Run `python3 build_file_manifest_vs_steering.py` remotely from the output directory.
- [ ] Verify required output files and acceptance counts.
- [ ] Report whether corrected lumi table was written or blocked, with manifest-stated reason.
