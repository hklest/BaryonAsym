# RAPGAP RAD/NONRAD batch workflow for high-Q2 SIDIS C_rad

## 1. Purpose and canonical formula

This note is the persistent operational workflow for producing the high-Q2 SIDIS RAPGAP RAD and NONRAD campaigns in batch and deriving the radiative correction

`C_rad(bin) = sigma_nonrad(bin) / sigma_rad(bin)`.

Use the ratio of cross sections, not multiplicities. The bin-level implementation must count or integrate generator cross sections consistently for RAD and NONRAD in the same bin definition. Do not normalize with hadron multiplicities.

Canonical campaign root:

`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/MC/rapgap_rad_norad_highQ2_1M_2026-06-18`

Steering cards:

- RAD: `cmnd/rapgap_rad_q2_150_20000_y_0p1_0p7.steer.rad`
- NONRAD: `cmnd/rapgap_nonrad_q2_150_20000_y_0p1_0p7.steer.nonrad`

Wrapper to use:

- `scripts/run_single_job.corrected_keephepmc.sh`

Expected output split per chunk:

- HepMC: `hepmc/{rad,nonrad}/`
- ROOT: `output/{rad,nonrad}/`

## 2. Pre-flight checks

Before any submission:

1. Read `docs/RUN_STATUS.md` and `context-handoff.md` for current SIDIS-high-Q2 state.
2. Confirm the campaign root above is the intended working area.
3. Confirm DUST headroom by reading `/data/dust/group/h1/.quota-usage.csv`. This check is mandatory before full production. Recent headroom was about 19.867 TB free.
4. Use Condor standard jobs unless a validated wrapper explicitly requires otherwise:
   - `+RequestRuntime = 10800`
   - `RequestMemory = 2000`
   - `RequestCpus = 1`
5. Confirm the corrected keep-HepMC wrapper is the one wired into the submit path and that it preserves both HepMC and ROOT outputs in the subdirectories listed above.
6. Do not run broad Condor management commands on the shared account. Monitoring and removals must be campaign-specific.

## 3. Steering card checks

Verify the steering cards exactly before submission.

Common beam/process/phase-space settings:

- `PPIN=920`
- `PLEP=-27.6`
- `LEPI=-11`
- `HADI=2212`
- `IPRO=1200`
- `IDIR=1`
- `IDIF=0`
- `IVME=0`
- `QMIN=150.0`
- `QMAX=20000.0`
- `YMIN=0.1`
- `YMAX=0.7`

RAD-only switches:

- `LPIN(4)=1`
- `ISM3(1..3)=1`

NONRAD-only switches:

- `LPIN(4)=0`
- `ISM3(1..3)=0`

Keep the steering verification note with the campaign:

- `notes/nonrad_radoff_steering_verification_2026-06-19.txt`

Submit-file trap already fixed in this campaign:

- wrong: `Arguments = rad \ 10000`
- correct: `Arguments = rad $(seed) 10000`
- correct: `Arguments = nonrad $(seed) 10000`

Also ensure logs go to safe per-job paths under `condor/logs/`.

## 4. Validation submission workflow

Always run validation before the full campaign.

Validation design:

- 10 RAD jobs
- 10 NONRAD jobs
- 10,000 events per job

Validation goal:

- prove the wrapper, steering routing, submit arguments, log paths, HepMC retention, and ROOT writing all work for both modes before scaling.

Validation status from the current session:

- passed after fixing the missing NONRAD card and RAD bookkeeping issues
- RAD: 10/10 HepMC and 10/10 ROOT
- NONRAD: 10/10 HepMC and 10/10 ROOT

Operational rules:

1. Keep validation outputs namespaced under the campaign root.
2. Use campaign-specific Condor filters when checking status.
3. Poll with one SSH command per check; sleep locally between polls; stop on SSH failure.
4. Never use broad cleanup like `condor_rm klesthen`.

Expected downstream validation-style products:

- `data/crad_1d_ISR5deg_validation_10files/`
- `plots/crad_1d_ISR5deg_validation_10files/`

## 5. Full 1M Condor workflow

Only launch after validation passes and quota is confirmed.

Full design:

- RAD: 100 jobs x 10,000 events = 1,000,000 events
- NONRAD: 100 jobs x 10,000 events = 1,000,000 events

Required preservation model:

- keep HepMC per chunk
- keep ROOT per chunk
- keep RAD and NONRAD separated throughout production and validation

Submission checklist:

1. Reconfirm quota from `/data/dust/group/h1/.quota-usage.csv`.
2. Reconfirm correct submit arguments for both modes.
3. Reconfirm log, output, and HepMC destinations are under the campaign tree.
4. Reconfirm standard-job Condor resources.
5. Submit RAD and NONRAD as distinct, explicitly labelled campaigns.
6. Monitor only with campaign Cmd/path filters.

Full outputs should follow the same namespacing pattern as the validation products, but under the full campaign directory.

## 6. Output validation

Validate both file presence and physics sanity.

Minimum file checks:

- expected HepMC chunk count exists in `hepmc/rad/` and `hepmc/nonrad/`
- expected ROOT chunk count exists in `output/rad/` and `output/nonrad/`
- no mode mixing between RAD and NONRAD paths
- per-job logs exist under `condor/logs/`

HepMC interpretation trap:

- RAPGAP HepMC headers report `U GEV MM`, but the four-vectors are TeV-like, e.g. beam energies appear near `Ee~0.0276`, `Ep~0.920`
- downstream extractors must rescale momenta and energies by x1000
- beam validation after scaling should recover about 27.6 GeV electron beam and 920 GeV proton beam

Physics sanity checks:

- all-final `Empz` median should be near 55.2 GeV for both RAD and NONRAD after scaling
- if Empz is not near this scale, treat unit handling as broken before doing any C_rad extraction

ISR5 diagnostic used in this campaign:

- identify stable photons with `pdg=22` and `status=1`
- compute angle to the incoming electron beam direction `-z` as `theta = arccos((-pz)/|p|)`
- reject photons with `theta < 5 deg`

## 7. Cross-section-ratio calculation workflow

The correction to derive is

`C_rad(bin) = sigma_nonrad(bin) / sigma_rad(bin)`.

Workflow:

1. Use the same bin definition for RAD and NONRAD.
2. Extract the bin-level cross section consistently from generator/reducer metadata or from HepMC cross-section C-lines, as appropriate for the chosen reducer.
3. Ensure the same counting convention, phase-space definition, and normalization path is used in both modes.
4. Form the ratio only after those consistency checks pass.
5. Do not substitute multiplicity normalization or hadron-yield ratios for cross sections.

Implementation note:

- the exact bin-level extraction may come from generator metadata or HepMC C-lines, but the method must be identical in meaning for RAD and NONRAD within each bin
- document the source of sigma explicitly in the derived output

Output organization:

- validation-scale C_rad tables and plots should live under `data/crad_1d_ISR5deg_validation_10files/` and `plots/crad_1d_ISR5deg_validation_10files/`
- full-scale products should use the same style of namespacing under the campaign root

## 8. Known traps and failure modes

- Wrong convention: using multiplicity ratios instead of `sigma_nonrad / sigma_rad`.
- Wrong orientation: current corrected convention is NONRAD over RAD.
- Steering drift: RAD must have radiation on; NONRAD must have radiation off.
- Submit bug regression: malformed arguments like `rad \ 10000` are invalid.
- Missing-card regression: ensure both RAD and NONRAD steer files are present and routed correctly.
- Bad log paths: always keep safe per-job logs under `condor/logs/`.
- Wrapper regression: if HepMC is not preserved under `hepmc/{rad,nonrad}/`, do not scale up.
- Broad Condor actions on shared account: never use blanket `condor_rm klesthen`.
- Monitoring mistakes: use campaign-specific Cmd/path filters only; one SSH command per poll; separate sleep; stop on SSH failure.
- HepMC unit trap: header says GeV, payload behaves like TeV; rescale x1000 before beam or Empz checks.
- Unvalidated downstream extraction: if beams are not near 27.6/920 GeV after scaling, the extractor is wrong.
- Mixed normalization semantics: if RAD and NONRAD bin-level sigma are not derived with the same convention, the ratio is unusable.

## 9. Minimal handoff checklist

Before handing off, record these facts:

- campaign root used
- steering cards used
- wrapper used
- submit argument form used
- Condor resource stanza used
- quota check result from `/data/dust/group/h1/.quota-usage.csv`
- validation status for 10 RAD + 10 NONRAD jobs
- chunk counts written for HepMC and ROOT in both modes
- whether beam-energy and Empz sanity checks passed after x1000 scaling
- exact bin-level sigma source used for `C_rad`
- output directories for validation products and full products
- any failed jobs, missing chunks, or unresolved normalization ambiguities
