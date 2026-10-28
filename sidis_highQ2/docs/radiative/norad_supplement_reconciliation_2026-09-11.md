# DJANGOH14 NORAD supplement reconciliation — 2026-09-11

Campaign reconciled (read-only):
`/data/dust/group/h1/klesthen/Djangoh14/campaigns/hk_0123_500x10k_prod_20260708/`

## Verified Stage-3 artifacts

- Stage-3 output pattern: `stage3_treebuilder/jobs/job_####/root/crad_genonly_####.root`.
- Verified ROOT outputs: **500**.
- Stage-3 `stage3.ok` markers: **500**.
- Total ROOT payload: **4,263,886,000 bytes** (3.97 GiB; `du` directory allocation: 4.1G including Stage-3 ancillary files).
- Every output has the ROOT magic header and identical size of **8,527,772 bytes**.
- Bad-file summary: **0 zero-size; 0 short/size-outlier files**. This is a file-level reconciliation only, not a tree-content validation.
- Existing validation sidecar, `manifests/stage3_validation_status.json`, records `root_count=500`, `ok_count=500`, no failed logs, timestamp `2026-07-09T18:39:14.690953Z`.

## Manifest disagreement

- `manifests/stage3_manifest.tsv`: all **500** rows retain `cluster_id=DRY_OR_UNSUBMITTED` and `state=prepared`, despite the 500 outputs/OK markers and `manifests/stage3_submit_map.tsv` recording 500 successful submissions (`rc=0`, clusters beginning 4245993).
- `manifests/stage2_manifest.tsv`: all **500** rows likewise retain `cluster_id=DRY_OR_UNSUBMITTED` and `state=prepared`, despite downstream complete Stage-3 artifacts. Original manifests were not modified.
- `manifests/stage1_manifest.tsv` is consistent with execution: 500 submitted rows (clusters 4242820 onward).

## Generated-event and cross-section bookkeeping

- Requested: **10,000 events/job** × **500 confirmed-good Stage-3 outputs** = **5,000,000 requested events**.
- Each of the 500 Stage-1 generator logs reports `GEND: Total number of events generated 9999`; log-summed generator-reported total is **4,999,500 events**. Logs also report 10,000 records processed per job.
- Generator-reported cross section is consistent across all 500 logs: **7,323.9 pb** from HERACLES, equivalently `SIGTOT = 7.3239 nb ± 0.00073239 nb`.

## Queue check and remaining unknowns

- A current `condor_q` query for owner `klesthen`, filtered on the campaign marker in `Cmd`, returned **no jobs**.
- Remaining normalization choice: use the explicitly recorded generator total (4,999,500) versus requested/processed count (5,000,000) for the effective-luminosity convention; this reconciliation does not decide that physics/normalization policy.

## Normalization authority decision (2026-09-11)

### Effective-luminosity convention

**Use the generator-produced event count, `N_generated=4,999,500`, not the requested `NEVENT` total or BOS record count.**  H1/DJANGOH luminosity is the generated weighted-event exposure divided by the generator cross section.  The job-0001 log demonstrates the distinction: `NEVENT=10000`, output records `10002`, but the generator's `sum(wtx1)=9999` and every produced event bank (`HEAD`, `GKI`, etc.) has 9999 entries.  Thus the one-event/job difference is a real one generated-event deficit in the usable generator stream (not a luminosity-bearing record-count artifact); the two extra BOS records are run/file bookkeeping.  It is only a 0.01% exposure deficit, but it must not be silently restored.

### Supplement luminosity

DJANGOH14, NC `EMINUSP`, NORAD, `Q2MI=100 GeV^2`, CTEQ6L (`LST(15)=10041`), `E_e=27.6 GeV`, `E_p=920 GeV`:

`L_supp = 4,999,500 / (7.3239 +/- 0.00073239 nb) = 682,628.108 +/- 68.263 nb^-1` (cross-section uncertainty only; generated-count uncertainty not assigned).

### Compatibility and approved combination rule

**FAIL for naive global combination.**  The legacy DJANGOH14 NONRAD manifest confirms the same generator label, NORAD mode, `Q2GT100`, and CTEQ6L, so those cuts/PDFs agree.  However it covers both `EPLUSP` and `EMINUSP`, whereas this supplement is only `EMINUSP`; the legacy source also contains the `EPLUSP` 27.5-GeV ep0304 component while this e- steering is 27.6 GeV.  Beam charge is physics-relevant for radiative corrections.  In addition, `3,560,233 nb^-1` remains provisional rather than generator-log traced.  Therefore do not replace the global NONRAD denominator by a summed number or concatenate the full samples yet.

If, and only if, the baseline is split to the matching e- 27.6-GeV component and its effective luminosity is independently verified, form each bin as `sigma_nonrad=(S_base+S_supp)/(L_base+L_supp)`, with `S=sum(weight1*weight2)` per sample (and propagate `sumw2`); equivalently concatenate only then and divide once by `L_total`.  The conditional arithmetic is `L_nonrad=3,560,233+682,628.108=4,242,861.108 +/-68.263 nb^-1`; it is **not approved** as the global C_rad luminosity.

### Beam-charge balance

An e-p-only addition cannot be combined into a charge-summed C_rad denominator without biasing the e+/e- mixture.  For the four-period data luminosities, `L(e+p)=106,823.1+46,200.7=153,023.8 nb^-1` and `L(e-p)=144,634.0 nb^-1`, hence the required ratio is `L(e+p)/L(e-p)=1.0580071`.  Match the new e- supplement with an e+ supplement of `722,224.3 nb^-1`.  At the same quoted cross section this corresponds to **5,289,507 generated e+p events** (practically at least 5,290,000, subject to the e+ generator cross section); use the e+ log cross section for the final event request.  Otherwise retain charge-separated RAD/NORAD cross-section proxies and combine only with explicit data-luminosity weighting.

### Blockers

1. Generator-log/steering luminosity traceability for the legacy `3,560,233 nb^-1` baseline, including its e+/e- and beam-energy split.
2. A matching e+p supplement (and its own `N_generated/sigma` luminosity) or an explicitly charge-separated C_rad construction.
3. The existing radiative-release blockers remain: approved sparse-bin policy and RAPGAP disposition.  No central release is changed or Born-corrected by this record.

## Baseline luminosity resolution (2026-09-11)

### Verdict and evidence

**REJECT `L_nonrad=3,560,233 nb^-1` for the DJANGOH14 NORAD Q2GT100 CTEQ6L baseline used by `full_crad_v1`.**  It is a stale/mis-transcribed aggregate constant in the manifest/finalizer, not the effective luminosity of the 520 processed baseline ntuples.

The direct, charge-split tally of those exact ntuples (`nonrad_baseline_charge_split_2026-09-11.json`) finds 260 files per charge and `GenDIS` entries

| baseline component | generated events | generator cross section | authoritative effective luminosity |
|---|---:|---:|---:|
| DJANGOH14 NORAD Q2GT100 CTEQ6L, EPLUSP | 39,845,169 | 7,287.91 pb | **5,467,297.071 nb^-1** |
| DJANGOH14 NORAD Q2GT100 CTEQ6L, EMINUSP | 40,000,004 | 7,323.94 pb | **5,461,541.738 nb^-1** |
| combined processed baseline | 79,845,173 | charge-specific | **10,928,838.810 nb^-1** |

Thus every baseline input file is represented; this is not a generated-versus-processed subset distinction.  `ChargedGenRC` has 1,777,852,281 track entries because it is a hadron tree, whereas the event exposure is the 79,845,173 `GenDIS` entries.  The old `full_crad_v1` diagnostics also consumed 520 NORAD partials (one was hadron-empty but does not change its `GenDIS` event exposure).  There is no ntuple-production acceptance/filter mechanism that can change luminosity: truth/analysis cuts act on the weighted numerator after the exposure is fixed.

The weights do not rescue the old value.  `weight2=1`; the direct ntuple check gives mean `weight1=2.36316` (EPLUSP) and `2.35127` (EMINUSP).  As required by the gen-only convention, use `sum(weight1*weight2)/L_MC` for each cross-section proxy; `weight1*weight2` is not luminosity and must not be folded into `L_MC=N_generated/sigma`.  Both charge samples report `GenBeamElecE=27.6 GeV`; this resolves the ntuple-level 27.5/27.6 ambiguity in favour of the baseline steering value, while the source-period provenance remains to be archived.

The difference is a real normalization error: `10,928,838.810 / 3,560,233 = 3.069697632`; the old value is low by `7,368,605.810 nb^-1` (206.9698%).

### Consequence for `full_crad_v1`

The current finalizer hard-codes `L_rad=4,108,957 nb^-1` and the rejected `L_nonrad=3,560,233 nb^-1`.  With unchanged accumulated NORAD and RAD weighted sums,

`C_rad,new = C_rad,full_crad_v1 * (3,560,233 / 10,928,838.810) = 0.325764984 * C_rad,full_crad_v1`.

Equivalently, the luminosity factor changes from `L_rad/L_nonrad=1.154125868` to `0.375973795`.  The published provisional integrated values would become `0.343789492` (h+) and `0.343816812` (h-) instead of `1.055329789` and `1.055413655`.  **RECOMPUTATION OF `full_crad_v1` IS REQUIRED** (including all bin values and uncertainties); the old tables are normalization-invalid and remain provisional only.

### Supplement rule

For the e- 27.6-GeV supplement, `L_eff=682,628.108 nb^-1`.  Its corrected relative weight against the matching baseline e- component is

`w_supp/base(e-) = 682,628.108 / 5,461,541.738 = 0.124988170`

(or `w_base/w_supp=8.000757183`).  Combine matched samples only as
`(S_base+S_supp)/(L_base+L_supp)`, using each sample's `S=sum(weight1*weight2)` and `sumw2`; the matched e- combined luminosity would be `6,144,169.846 nb^-1`.  Do not add the e--only supplement to the charge-summed baseline without an explicit charge-separated construction or a matching e+ supplement.

### Remaining blockers

1. Regenerate `full_crad_v1` from the same partials with the charge-aware baseline luminosities (or explicitly justified charge combination), then preserve the old artifact as invalidated provenance.
2. Archive generator-log/steering provenance and the e+/e- period mapping; ntuple-level energy is 27.6 GeV, but period composition must still be made explicit.
3. Existing release blockers remain: approved sparse-bin policy and RAPGAP inclusion/disposition.  No central result is Born-corrected by this resolution.
