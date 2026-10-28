# Binary-Identity Guardrails — Installation and Validation

**Date:** 2026-05-22
**Authors:** code-editor (binary-identity guardrails task)
**Status:** All 5 guardrails installed, deliberately-broken twice, both
breaks STOPPED at the guardrail with a clear error message.

## 1. Precipitating incident

The Stream C NORAD Condor campaign (1038 jobs) submitted on 2026-05-22
produced empty trees end-to-end. Root cause: both
`H1BaryonAsym/Makefile` (low-Q^2 SpaCal SIDIS) and
`H1BaryonAsym_HighQ2/Makefile` (high-Q^2 LAr SIDIS) declared
`BIN = TreeBuilder ...` and linked to the SAME shared
`$(BINDIR)/x86_64-el9-gcc14-opt/TreeBuilder`. Whichever `make` ran last
wrote the binary; the other Makefile's intent was silently overwritten.
The Stream C wrapper invoked
`/data/dust/.../bin/x86_64-el9-gcc14-opt/TreeBuilder` trusting that the
high-Q^2 source had compiled it. In fact the low-Q^2 SpaCal source had
compiled it last (May 21, 00:30); every Stream C Condor job ran the
WRONG selection (SpaCal box/ellipse instead of high-Q^2 LAr) on NORAD
HATs and emitted empty trees.

The wrapper had ONE pre-flight test (`test -x`) and ONE post-flight
test (check tree exists in output) — neither caught the wrong-binary
class of error. This memo records the FIVE permanent guardrails
installed on 2026-05-22 to make the failure class structurally
impossible to recur.

## 2. The five guardrails

### Guardrail 1 — Distinct binary names

The high-Q^2 builder source `H1BaryonAsym_HighQ2/src/TreeBuilder.{C,h}`
was renamed to `TreeBuilderHighQ2.{C,h}` and the Makefile updated to
emit `$(BINDIR)/TreeBuilderHighQ2` instead of `$(BINDIR)/TreeBuilder`.
The two builders can no longer collide at the install path.

| Before | After |
|--------|-------|
| `H1BaryonAsym_HighQ2/src/TreeBuilder.C` | `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.C` |
| `H1BaryonAsym_HighQ2/src/TreeBuilder.h` | `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.h` |
| Makefile `BIN = TreeBuilder ...` | Makefile `BIN = TreeBuilderHighQ2 ...` |
| Link target `TreeBuilder:` | Link target `TreeBuilderHighQ2:` |
| Output `-o $(BINDIR)/TreeBuilder` | Output `-o $(BINDIR)/TreeBuilderHighQ2` |

Pre-task backups preserved on NAF:
- `H1BaryonAsym_HighQ2/Makefile.bak_pre_guardrail_2026-05-22`
- `H1BaryonAsym_HighQ2/src/TreeBuilder.C.bak_pre_guardrail_2026-05-22`
- `H1BaryonAsym_HighQ2/src/TreeBuilder.h.bak_pre_guardrail_2026-05-22`
- `bin/x86_64-el9-gcc14-opt/TreeBuilder.bak_lowQ2_premake_2026-05-22`

Compiled binary identity (post-rename, 2026-05-22):

| Binary | md5 | size (B) |
|--------|------|-----------|
| `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2` (NEW) | `63e477156ef70ccc1e6ed4bdc4a7a01a` | 1,884,624 |
| `bin/x86_64-el9-gcc14-opt/TreeBuilder` (low-Q^2 SpaCal, UNTOUCHED) | `8de736bc2b8995b1a8428784a7cf3b95` | 1,516,144 |
| `bin/x86_64-el9-gcc14-opt/ChargedSidisTreeBuilderHighQ2` (Stage 2, untouched) | `d95158bf537331c2713f85c5f9d8f84b` | 604,856 |

The low-Q^2 binary md5 is bit-identical to its pre-task state
(`8de736bc2b8995b1a8428784a7cf3b95`, both pre-task and the
`.bak_lowQ2_premake_2026-05-22` copy). The high-Q^2 rename only ADDED a
new binary; it did not change the low-Q^2 one.

### Guardrail 2 — Binary identity manifest

The authoritative pin lives in BOTH places:

- Local: `sidis_highQ2/scripts/binary_identity_manifest.json`
- NAF:   `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/binary_identity_manifest.json`

The manifest carries: pinned md5, pinned size, compiled-from source
path + md5, distinct cutflow_signature labels, FORBIDDEN_FOR notes
where applicable, and a list of valid-for-samples. Manifest schema is
versioned (`manifest_version: 1.0`) and dated (`as_of_date:
2026-05-22`).

### Guardrail 3 — Wrapper assertions

The `sidis_highQ2/scripts/_binary_guard.py` module provides:

- `load_manifest(path)` — load & validate JSON
- `verify_binary(binary_key, manifest, ssh_target)` — SSH-remote
  md5+size on the binary and the compiled-from source; raises
  `BinaryIdentityError` on ANY mismatch.
- `verify_binary_local(binary_key, manifest, local_path)` — local md5
  variant for tests.
- `verify_cutflow_signature(log_text, binary_key, manifest)` — parses
  the stdout log of a job and asserts the manifest's cutflow labels
  appear AND that no FORBIDDEN sibling-binary distinctive labels
  appear (catches a between-submit-and-exec swap).

The three submit wrappers
`submit_norad_charged_sidis_highQ2.py`,
`submit_phi_yields_v5f_condor.py`,
`submit_phi_gap_diag_condor.py`
each call `verify_binary` at start of `--dry-run` AND at start of
`--do-submit`. ANY mismatch exits 2 before any `condor_submit`. The
ChargedSidisTreeBuilderHighQ2 binary is the upstream-provenance pin
for the two phi wrappers (which exec a Python extractor in Condor but
consume v5f ROOTs produced by that builder).

### Guardrail 4 — Smoke gate before bulk submission

The `sidis_highQ2/scripts/_smoke_gate.py` module provides
`smoke_test_one_job(...)` which runs ONE wrapper invocation on the NAF
login node in a bounded (≤ 5 min wall) temporary work-dir, then opens
the produced ROOT with uproot and asserts:

- expected tree exists
- `num_entries >= min_entries` (default 1)
- expected schema branches exist and are non-empty

All channel smokes must PASS before ANY channel is submitted to
Condor. Failure raises `SmokeGateError` and exits 3.

The NORAD wrapper runs the smoke during BOTH `--dry-run` (after
emitting just ONE wrapper per charge so the smoke has something to
invoke) AND `--do-submit` (after emitting the full job set, before
condor_submit). The two phi wrappers run an extractor-smoke variant
(`/usr/bin/python3 <extractor> --mode ... --input <first file>`) and
verify the produced JSON / Parquet is non-empty.

### Guardrail 5 — Submission audit log

Each wrapper writes a per-channel forensic JSON to
`sidis_highQ2/docs/data/submission_audit_<wrapper>_<channel>_<phase>_<UTCstamp>.json`
containing: binary path + md5 + size at submission; source path + md5;
manifest version used; smoke-test result snapshot; cluster IDs (when
post-submission); operator note (`--note <text>`); wrapper path + git
commit hash; ISO UTC timestamp. Phases recorded: `_dryrun`,
`_presubmit`, `_postsubmit`.

## 3. Deliberate-break tests

Two break scenarios were executed end-to-end. In both cases the
wrapper exited cleanly (no condor_submit attempted) with an exit code
that surfaces the failure to a calling supervisor.

### Break 1: corrupt the manifest expected_md5

```
$ python3 -c "import json; m=json.load(open(...)); \
    m['binaries']['TreeBuilderHighQ2']['expected_md5'] = 'ff'*16; \
    json.dump(m, open(...,'w'), indent=2)"
$ python3 sidis_highQ2/scripts/submit_norad_charged_sidis_highQ2.py \
      --stage evls --dry-run --period eminusp
```

Result (excerpt):
```
FATAL [guardrail-3] BINARY md5 MISMATCH for 'TreeBuilderHighQ2' at
   /data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2.
  expected md5: ffffffffffffffffffffffffffffffff
  observed md5: 63e477156ef70ccc1e6ed4bdc4a7a01a
  ...
REMEDIATION: either rebuild the binary from /data/.../TreeBuilderHighQ2.C
and update the manifest's expected_md5/size in the SAME commit, or
restore the binary that matches the pinned manifest. STOP.
```
Exit code: 2. No condor_submit called. Manifest restored after the
test; verification re-ran clean (exit 0).

### Break 2: symlink the high-Q^2 binary to the low-Q^2 binary

```
$ ssh naf-hone.desy.de "cd /data/.../bin/x86_64-el9-gcc14-opt && \
    mv TreeBuilderHighQ2 TreeBuilderHighQ2.SAVE_BREAK_TEST && \
    ln -s TreeBuilder TreeBuilderHighQ2"
$ python3 sidis_highQ2/scripts/submit_norad_charged_sidis_highQ2.py \
      --stage evls --dry-run --period eminusp
```

Result (excerpt):
```
FATAL [guardrail-3] BINARY md5 MISMATCH for 'TreeBuilderHighQ2' at
   /data/dust/.../bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2.
  expected md5: 63e477156ef70ccc1e6ed4bdc4a7a01a
  observed md5: 8de736bc2b8995b1a8428784a7cf3b95
                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ THE LOW-Q^2 BINARY
```
Exit code: 2. The observed md5 is the low-Q^2 SpaCal binary's md5 —
the wrapper identified the wrong-binary swap by its content hash, not
by the symlink stat. Restored after the test:
```
TreeBuilderHighQ2: 63e477156ef70ccc1e6ed4bdc4a7a01a  (manifest match)
TreeBuilder:       8de736bc2b8995b1a8428784a7cf3b95  (manifest match)
```

## 4. Healthy-path validation

After both break tests restored cleanly, the healthy dry-run on the
NORAD wrapper produces:

- `[ssh-check] Master running` ✓
- `[guardrail-2] manifest loaded ... version 1.0, as_of 2026-05-22` ✓
- `[guardrail-3] TreeBuilderHighQ2 md5=63e477156ef70ccc1e6ed4bdc4a7a01a OK` ✓
- `[guardrail-4] dry-run smoke PASS for ['evls_eminusp']` (only when
  the underlying input is healthy — see Section 5 caveat)
- `[guardrail-5] wrote submission audit -> .../submission_audit_norad_eminusp_dryrun_*.json` ✓
- exit 0

## 5. Independently-discovered finding from the smoke gate

The dry-run smoke on the NORAD `eminusp` first HAT
(`hat.DJANGOH14.NC.EMINUSP.NORAD.Q2GT100.CTEQ6L.A.C0000000.root`)
exec'd `TreeBuilderHighQ2 --stage evls --mode mc ...`. The job ran to
completion (`exit code 0`) on the login node within the 5-min cap. The
binary correctly identified itself in stdout (all 17 manifest cutflow
labels appeared) and produced an `EvlTree`; HOWEVER the tree contained
ZERO entries because every event was rejected at the `Electron E`
cut. Diagnostic excerpt:

```
stage_evls (high-Q2 LAr): 0 / 154913 events passed DIS selection
  Cutflow summary:
    Run list rejected:        0
    ...
    Electron E rejected:      154913    <-- all rejected here
    Electron theta rejected:  0
    ...
Warning in <H1Constants::Year>: no known year for run 1, returning 0
```

The "no known year for run 1" warning suggests the NORAD HATs carry a
dummy run number (1) that the year-of-run lookup cannot map, which
defaults to a year-zero electron-energy threshold that no real
electron passes. This is an independent finding — the wasted-Stream-C
incident's BINARY-identity root cause is now closed, but the NORAD
sample also has a real DOWNSTREAM bug.

Because the smoke gate caught the empty-tree condition before any
condor_submit, the guardrail prevented a SECOND wasted campaign. The
operator now has a clear, blocking signal to investigate the NORAD
electron-E selection mismatch before launching Stream C against the
high-Q^2 LAr cuts.

This is precisely the failure mode the guardrails were built to
catch — and they caught it on the very first run.

## 6. Files added / modified by this task

| Path | Status |
|------|--------|
| `sidis_highQ2/scripts/binary_identity_manifest.json` | NEW |
| `sidis_highQ2/scripts/_binary_guard.py` | NEW |
| `sidis_highQ2/scripts/_smoke_gate.py` | NEW |
| `sidis_highQ2/scripts/submit_norad_charged_sidis_highQ2.py` | EDITED (+~200 LOC) |
| `sidis_highQ2/scripts/submit_phi_yields_v5f_condor.py` | EDITED (+~210 LOC) |
| `sidis_highQ2/scripts/submit_phi_gap_diag_condor.py` | EDITED (+~210 LOC) |
| NAF `H1BaryonAsym_HighQ2/Makefile` | EDITED in place (Guardrail 1) |
| NAF `H1BaryonAsym_HighQ2/src/TreeBuilderHighQ2.{C,h}` | NEW (rename of TreeBuilder.{C,h}) |
| NAF `bin/x86_64-el9-gcc14-opt/TreeBuilderHighQ2` | NEW (recompiled) |
| NAF `H1ChargedSidis_HighQ2/binary_identity_manifest.json` | NEW (copy of local) |
| NAF `H1ChargedSidis_HighQ2/scripts/_binary_guard.py` | NEW (copy of local) |

## 7. Operator note

The wrappers each gained a `--note <text>` CLI argument that is
recorded verbatim in every per-channel `submission_audit_*.json`. Use
it to record the campaign intent (e.g. `--note "Stream C NORAD
re-launch attempt after run-1 fix"`). The audit JSON is the forensic
record that prevents "we don't know what we submitted" situations.

## 8. What each guardrail catches

| Failure mode | Caught by |
|--------------|-----------|
| Two Makefiles colliding at the same `$(BINDIR)` path | **Guardrail 1** (rename) |
| Operator forgets to rebuild before submitting | **Guardrail 3** (source-md5 drift detected) |
| A previous campaign's binary still on disk after source changes | **Guardrail 3** (binary md5 mismatches manifest) |
| Operator points `--evls-bin` at a wrong-named binary | **Guardrail 3** (path-vs-manifest cross-check) |
| Binary swapped on disk between submission and execution | **Guardrail 3** verify_cutflow_signature on completed jobs |
| Binary is correct but emits empty trees (THE Stream C mode) | **Guardrail 4** (smoke gate; min_entries) |
| Binary is correct but emits wrong tree name | **Guardrail 4** (expected_tree assertion) |
| Binary is correct but emits wrong schema | **Guardrail 4** (schema_branches assertion) |
| "We don't know what we submitted" months later | **Guardrail 5** (per-channel audit JSON) |
