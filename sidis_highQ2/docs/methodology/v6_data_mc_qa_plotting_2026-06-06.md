# v6 Data/MC QA Plotting (2026-06-06)

## Purpose

Canonical pre-unfolding QA plotting for the H1 charged-particle SIDIS
high-Q² v6 production. Produces DATA vs RAPGAP vs DJANGOH comparisons for
the kinematic and tracking observables on both the reco tree
(`ChargedTot`) and the gen tree (`ChargedTotGen`), with MC
**luminosity-scaled to data (not shape-normalized)** and a MC/DATA
ratio sub-panel under every variable.

Script (local):
`H1/sidis_highQ2/scripts/plotting/plot_v6_data_mc_qa.py`

This is a QA / sanity tool. Cross-section, multiplicity, and asymmetry
language is intentionally avoided. MC histograms are labeled
"lumi-scaled to data, QA only".

## Modes

| Mode | What is plotted |
|------|-----------------|
| `nocuts` | All entries in the tree subject only to finite/sentinel sanity for the plotted variable. Useful for raw branch population and sentinel auditing. |
| `reco-cuts` | Reco-side cuts applied to `ChargedTot` only: Q²>150, 0.1<y<0.7, 35<Empz<70, xF<0. **Gen-side plots never receive reco cuts** (`ChargedTotGen` is kept on its own; v6 envelope-OR populates gen rows independently). |
| `both` | Produces both `nocuts` and `reco-cuts` outputs in one run. |

## Inputs

For each sample (`data`, `rapgap`, `djangoh`), supply exactly one input
specifier:

- `--data-root <file>` / `--data-list <txt>` / `--data-root-glob <glob>`
- same triad for `--rapgap-*` and `--djangoh-*`

`--*-list` precedence > `--*-root-glob` > `--*-root`.

Per-sample luminosity (`nb⁻¹`):

- `--data-lumi`    (default `351000`, the current canonical 4-period high-Q² HERA-II quote; older `356613` direct-monitor uses are superseded)
- `--rapgap-lumi`  (no default; required for RAPGAP to appear)
- `--djangoh-lumi` (no default; required for DJANGOH to appear)

**The script intentionally does NOT embed default high-Q² MC luminosities.**
The v5g high-Q² MC luminosity metadata (per-period RAPGAP/DJANGOH) is
maintained outside this script and must be supplied explicitly to avoid
silently reusing low-Q² inclusive numbers. If you find the canonical
v5g lumi values, pass them on the CLI.

## Weights

- MC: `weight1 * weight2` per row, multiplied by `lumi_data / lumi_mc`.
- DATA: `1.0` per row.
- MC histogram error is `sqrt(Σ w²)`; data error is `sqrt(N)`.

## Colors / Styles

- DATA    : black points with vertical error bars.
- RAPGAP  : Okabe-Ito orange `#E69F00`, filled step histogram.
- DJANGOH : Okabe-Ito bluish-green `#009E73`, filled step histogram.

Ratio panel underneath each top panel shows `RAPGAP/DATA` and
`DJANGOH/DATA` on the same binning, with `y` axis fixed to [0, 2]. Bins
with zero data are NaN (gaps).

## Variables Plotted

**Reco-side (`ChargedTot`):** `Q²`, `y`, `x`, `Empz`, track `pT`, track
`η`, track `φ`, `xF`, `z`, track `θ`, track `p`, `TrackLength`,
`NHitCJC`, `DcaPrime`, `DcaPrimeSinTheta`. Each variable is drawn three
times: all charges, charge = +1, charge = −1.

**Gen-side (`ChargedTotGen`):** `Q²`, `y`, `x`, `Empz`, hadron `pT`,
hadron `η`, hadron `φ`, `xF`, `z`, hadron `p`. Each variable also drawn
three times (all / +1 / −1) using `GenCharge`.

The full branch-candidate table is in the script. Missing branches in a
given file fall back to alternates (e.g. `Q2s` → `Q2es` → `Q2`) and any
unresolved role is recorded under `loaded_<side>.<sample>.missing_roles`
in the JSON summary.

## Outputs

For each mode the script writes (filenames respect `--tag`):

```
<out-dir>/v6_qa_reco_<mode>[<tag>].pdf
<out-dir>/v6_qa_gen_<mode>[<tag>].pdf      (only if MC gen tree loaded)
<out-dir>/v6_qa_summary[<tag>].json
```

The JSON records: resolved input paths, per-sample row counts, branch
maps, missing branch roles, weight totals, luminosity values, list of
output PDFs, per-page metadata, warnings, blockers, and the wall time.

Default `--out-dir` (when omitted) is
`<analysis_root>/plots/v6_data_mc_qa_2026-06-06`.

## Sentinel / Sanity Filtering

Each value array is filtered per-bin by `np.isfinite(v) &
~np.isclose(v, -999.0)`. This avoids polluting `TrackLength` plots with
the MC sentinel `-999` and avoids `NaN` contamination of any other
branch that is occasionally not filled.

## Caveats

1. **TrackLength on MC** is sentinel-filled (`-999`); the QA panel for
   it will show essentially only data entries.
2. **Gen plots never receive reco cuts.** The gen tree contains
   gen-only events that have no corresponding reco row by construction
   (v6 envelope-OR). Applying reco cuts to gen rows is logically wrong
   and is disabled in this script regardless of mode.
3. **Single-MC smoke runs are supported** (e.g. only DJANGOH). RAPGAP
   panels are simply omitted when no RAPGAP input is given. Full
   four-period coverage requires a NAF run; see "Production command
   template" below.
4. **MC luminosity values are not embedded.** Provide them on the CLI.
   See task handoff for the canonical v5g high-Q² values.
5. **Single resolved branch map.** The script picks the branch mapping
   from the first file in each sample. Mixed schemas across the list
   will silently use the first-file mapping; if the analysis ever moves
   to mixed v5g+v6 inputs in one run, rework the loader.

## Local validation (2026-06-06)

- `python3 -m py_compile plot_v6_data_mc_qa.py` → PARSE_OK.
- `--help` → full CLI surface printed.
- Smoke run on the two local v6 smoke ROOTs (DATA + DJANGOH only,
  RAPGAP absent on local disk) with `--mode both`:
  - 4 PDFs produced (`reco × {nocuts,reco-cuts}`,
    `gen × {nocuts,reco-cuts}`).
  - JSON summary at
    `H1/sidis_highQ2/plots/v6_data_mc_qa_2026-06-06/smoke_local/v6_qa_summary_final.json`.
  - DATA loaded: 16,889 rows. DJANGOH reco loaded: 252,201 rows
    (`weight1*weight2` sum = 365,312.83). DJANGOH gen loaded:
    1,720,321 rows (weight sum = 4,036,116.33).
  - No warnings, no blockers.
- "Missing MC lumi" smoke confirms the documented blocker is emitted
  when an MC sample has inputs but no `--*-lumi` was supplied.

## Production command template (for h1-script-runner / h1-plotting-naf)

The v6 central production lives on NAF under
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/`.
Do **not** SSH from this task; hand off the run to `h1-script-runner`
or `h1-plotting-naf`. Example NAF invocation (single condor wall slot,
no SSH from local):

```bash
# On NAF, after sourcing the analysis python env that has uproot:
PROD=/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree
OUT=/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/qa/v6_data_mc_qa_2026-06-06

python3 /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/plotting/plot_v6_data_mc_qa.py \
    --data-root-glob    "${PROD}/data*/sidistree_v6_d??_*.root" \
    --data-lumi         351000 \
    --rapgap-root-glob  "${PROD}/rapgap*/sidistree_v6_rapgap*.root" \
    --rapgap-lumi       <RAPGAP_LUMI_NB_FROM_V5G_METADATA> \
    --djangoh-root-glob "${PROD}/djangoh*/sidistree_v6_djangoh*.root" \
    --djangoh-lumi      <DJANGOH_LUMI_NB_FROM_V5G_METADATA> \
    --mode both \
    --out-dir ${OUT} \
    --tag central_v6_full
```

Before any production run, supply the per-MC luminosities from the
validated v5g high-Q² metadata (currently not embedded in the script
by design). If the per-period RAPGAP/DJANGOH lumi sets differ across
periods, run the script separately per period and aggregate manually,
or extend the loader to attach per-file weights (out of scope for
2026-06-06).

The script transfers the four PDFs + JSON; download with `scp` from
the NAF `${OUT}` directory.

## File locations

- Script: `H1/sidis_highQ2/scripts/plotting/plot_v6_data_mc_qa.py`
- Doc:    `H1/sidis_highQ2/docs/methodology/v6_data_mc_qa_plotting_2026-06-06.md`
- Local smoke outputs: `H1/sidis_highQ2/plots/v6_data_mc_qa_2026-06-06/smoke_local/`
