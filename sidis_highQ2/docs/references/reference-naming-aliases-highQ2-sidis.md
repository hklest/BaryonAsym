# Reference: legacy naming aliases for the high-Q2 charged-hadron SIDIS analysis

**Status:** authoritative naming-alias reference. Companion to
`docs/reviews/audit-proton-naming-highQ2-sidis.md` (the audit of record).

**Created:** 2026-05-19 as part of the Tier 1 + Tier 2 remediation pass
following the proton-naming audit.

---

## 1. Purpose

The high-Q2 charged-hadron SIDIS analysis (v5b builder, this workspace) was
forked from the high-Q2 proton/antiproton asymmetry analysis. To preserve
compile/run continuity, **many on-disk artifacts (source files, binary,
output ROOT files, submission scripts, condor wrappers, steering subdirs)
retain proton-prefixed naming** even though the physics they implement is
inclusive charged-hadron SIDIS.

This document is the **single authoritative map** between legacy
proton-prefixed names and their actual SIDIS intent. Any reader who
encounters a "proton" string in a high-Q2 SIDIS context (docs, JSON,
scripts) should consult Section 4 below to determine whether the name is:

- **LEGACY** — a SIDIS artifact named after the proton fork ancestor; keep
  the literal name on disk (changing it would break the pipeline), but
  understand its SIDIS meaning.
- **TRUE PROTON** — a legitimate reference to the species proton, the beam
  proton, or the separate `H1/proton_HiQ/` analysis. Leave alone.
- **DEFECT** — an obsolete reference to v4 schema (`ProtonTot`,
  `ProtonTotGen`, dE/dx PID branches) that is wrong in any v5b SIDIS
  context.

---

## 2. What this analysis IS

Inclusive charged-hadron semi-inclusive deep-inelastic scattering (SIDIS) at
**high Q^2 (Q^2 > 150 GeV^2)** with the H1 detector on HERA-II data.

**Observables:**
- Charge-separated multiplicities `M^{h+/-}(Q2, x, z, pT_h)`.
- Charged-hadron asymmetry `A_h = (N_{h+} - N_{h-}) / (N_{h+} + N_{h-})`.
- `A_BC` (charge-summed beam-helicity / charge-symmetry variants).
- Azimuthal moments `<cos(phi_h)>`, `<cos(2 phi_h)>` extracted from the
  fitted `A0 [1 + A1 cos(phi_h) + A2 cos(2 phi_h)]` form in coarse
  (Q2, x, z, pT_h) bins.

**PID:** none at reco level. The v5b builder removed all dE/dx PID
branches (`DedxRaw`, `Dedx{Pion,Kaon,Proton}`, `Lk{Pion,Kaon,Proton}`,
`Nlh{Pion,Kaon,Proton}`). Species identification, where needed, is
done via MC truth on `ChargedTotGen` using `GenPDG` (PDG code from the
H1 generator record).

**Species set:** {pi+, pi-, K+, K-, p, pbar} (i.e. |PDG| in {211, 321, 2212}).
No hyperon-inclusive denominator.

---

## 3. What this analysis is NOT

This analysis is **NOT** the proton/antiproton asymmetry analysis. That
analysis:

- Lives in a **separate workspace** at `H1/proton_HiQ/`.
- Uses **dE/dx PID** for species separation at reco level.
- Computes `A_p = (N_p/eff_p - N_{pbar}/eff_{pbar}) / (N_p/eff_p + N_{pbar}/eff_{pbar})`
  (single-species proton/antiproton asymmetry).
- Has its own NAF code root, builder, output files, and Condor
  infrastructure.

Any reference in SIDIS docs to "proton" that means the separate
proton/antiproton analysis (typically appearing as `proton_HiQ`,
`H1/proton_HiQ/`, "proton-asymmetry analysis", or
"the proton analysis") is **TRUE PROTON** and should be left alone.

Similarly, references to:
- `proton_HiQ` (separate analysis workspace),
- `lambda_HiQ` (separate Lambda/K0s analysis),
- beam proton physics,
- `M_PROTON` / `PROTON_MASS` (species mass constant),
- "Lambda -> p pi-" (decay product),

are all TRUE PROTON.

---

## 4. Active naming aliases (on-disk vs intent)

| On-disk name | Intent | Category |
|---|---|---|
| `H1SIDIS_HighQ2/` (NAF code root) | SIDIS analysis code root. (Already renamed from the legacy `H1ProtonAsymHighQ2/` — the new name is canonical.) | SIDIS |
| `H1SIDIS_HighQ2/src/ProtonTreeBuilderHighQ2.C` | High-Q2 SIDIS builder source (legacy proton-prefixed name from the fork). | LEGACY |
| `H1SIDIS_HighQ2/src/PhysicsConstants.h` | Generic constants header. File name is fine; contains some unused LEGACY `PROTON_*` constants. | TRUE-NAMED file / LEGACY contents |
| `bin/x86_64-el9-gcc14-opt/ProtonTreeBuilderHighQ2` | Compiled SIDIS builder binary (legacy name derived from source basename). | LEGACY |
| `H1SIDIS_HighQ2/output/mc/<gen>/<run>/proton_highQ2_mc_*.root` | v5b SIDIS per-run MC output ROOT files (legacy proton-prefixed basename pattern from the submission template). | LEGACY |
| `H1SIDIS_HighQ2/output/mc/<gen>/<run>/proton_highQ2_<gen>_<run>_<period>_NNNN.root` (no `_mc_` segment) | Duplicate of the MC file under the dual-naming pattern; deduped to keep only the `_mc_` variant. | LEGACY |
| `H1SIDIS_HighQ2/output/data/.../proton_highQ2_data_smoke.root` | v5b SIDIS smoke-test data file (legacy proton-prefixed name). | LEGACY |
| `merged_proton_highQ2_mc_<gen>.root` | Per-generator merge target for v5b SIDIS MC (legacy proton-prefixed name; defined in `merge_mc_highQ2.sh`). | LEGACY |
| `H1SIDIS_HighQ2/steering/proton/` | Subdirectory holding SIDIS steering files (legacy directory name). | LEGACY |
| `H1SIDIS_HighQ2/condor/proton/subs/` | Subdirectory holding Condor submit files for the SIDIS stage (legacy name). | LEGACY |
| `H1SIDIS_HighQ2/scripts/submit_full_production.py` (with PROTON_BIN, PROTON_PKG, create_proton_data_job, etc.) | SIDIS submission driver; internal Python identifiers carry the legacy `PROTON_*` prefix. | LEGACY |
| `H1SIDIS_HighQ2/scripts/submit_proton_highQ2_batch.sh` | SIDIS batch submitter (legacy filename). | LEGACY |
| `H1SIDIS_HighQ2/scripts/merge_mc_highQ2.sh` | SIDIS MC merge driver; writes `merged_proton_highQ2_mc_*.root` (legacy output stem). | LEGACY |
| `H1SIDIS_HighQ2/scripts/analyze_proton_highQ2.py` | UNRESOLVED. May be a stale fork-leftover or an active analyzer for a different observable. See `audit-proton-naming-highQ2-sidis.md` Section 11 R2. | AMBIGUOUS |
| `H1ProtonAsymHighQ2/` (cited in `research-highQ2-sidis-extension.md` and a few older snapshots) | Pre-rename name; does NOT exist on NAF. The actual NAF path is `H1SIDIS_HighQ2/`. Any current doc that uses the old name is a documentation defect. | DEFECT (doc) |
| `ProtonTot` / `ProtonTotGen` (tree names) | REMOVED in v5b. Any reference in a v5b SIDIS context that treats these as still-present is a defect. The v5b reco tree is `ChargedTot` (86 branches); the v5b gen tree is `ChargedTotGen` (50 branches); the v5b event tree is `DisTot` (49 branches). | DEFECT |
| `ProtonUtil::isChargedHadron`, `ProtonUtil::classifySourceGTR`, `ProtonUtil::getMassForPDG` (C++ namespace + symbols in `source_cache/ProtonUtils_*.h`) | SIDIS utility functions still living in a namespace named `ProtonUtil` (legacy). | LEGACY |
| `DedxProton`, `LkProton`, `NlhProton` (former PID branch names) | REMOVED in v5b. Listed here for completeness. References in docs that say "removed" are correct; references that treat them as live are defects. | DEFECT (if treated as live) / TRUE PROTON (as species-hypothesis name) |

---

## 5. Frozen SIDIS-correct tree and branch names (authoritative)

The v5b high-Q2 SIDIS schema is:

| Tree | Branches | Role |
|---|---:|---|
| `ChargedTot` | **86** | Per-track reco tree (one entry per reconstructed charged track passing builder-level pre-cuts) |
| `ChargedTotGen` | **50** | Per-track gen tree (one entry per generated charged hadron in the broad gen fiducial) |
| `DisTot` | **49** | Per-event DIS tree (one entry per DIS event passing the event-level cuts) |

**Notable additions over the v5 schema:**

- `ChargedTotGen` branch 50 is `IsV0DaughterGen`, a per-track V0-veto flag
  added in V6 to allow exclusion of charged hadrons originating from V0
  decays (Lambda, K0s) at the gen level.
- `DisTot` branch 49 is `isMC`, a per-event MC-metadata flag distinguishing
  MC events from data events when the two are processed through the same
  downstream pipeline.

**Trees that are NOT in v5b:**

- `ProtonTot` (was: per-track tree with proton-hypothesis dE/dx PID; REMOVED).
- `ProtonTotGen` (was: per-track gen tree with proton-only species filter; REMOVED).

Any v5b SIDIS code, doc, or script that lists `ProtonTot` or `ProtonTotGen`
as part of an active schema or `EXPECTED_TREES` list is wrong. (Positive
guards of the form `FORBIDDEN_TREES = ("ProtonTot",)` are CORRECT and
should be retained.)

---

## 6. Cross-reference policy

Every SIDIS document that uses a LEGACY name should add, **at the first
mention in that document**, a one-line parenthetical of the form:

> `proton_highQ2_mc_<run>_NNNN.root` (legacy proton-prefixed name from the
> v5b SIDIS fork; see `reference-naming-aliases-highQ2-sidis.md`).

Subsequent mentions in the same document need not repeat the
cross-reference. The single parenthetical at first mention is sufficient
to defuse confusion for any reader who consults the alias doc.

For session-produced JSON files that store NAF path strings as values, the
path strings themselves must be **preserved unchanged** (they cite real
on-disk paths). If the JSON has a `_meta`-style header block with prose
that uses LEGACY terminology in descriptive form, the prose should be
updated; the path-value strings should not.

---

## 7. Remediation tiers (status)

The proton-naming audit (`audit-proton-naming-highQ2-sidis.md`) defines a
five-tier remediation. Status as of 2026-05-19:

| Tier | Scope | Status |
|---|---|---|
| **Tier 0a** | Patch active-code-path defects in `produce_shifted_mc.py`, `produce_shifted_mc.C`, `merge_and_validate.py` that reference removed `ProtonTot`/`ProtonTotGen` trees | Executed (Task K) |
| **Tier 1** | Safe documentation prose rewrites (no literal NAF paths altered) | **Executed in this pass (2026-05-19)** |
| **Tier 2** | Session-produced artifacts: add cross-reference to alias doc in companion Markdown; preserve literal NAF path strings | **Executed in this pass (2026-05-19), alias-doc form** |
| **Tier 3** | NAF builder source rename (`ProtonTreeBuilderHighQ2.C` -> `SIDISTreeBuilderHighQ2.C`) + recompile | **Deferred** to next builder major revision (v6 or v5b data-campaign launch) |
| **Tier 4** | NAF output-file rename (`proton_highQ2_mc_*.root` -> `sidis_highQ2_mc_*.root`) for all 1210+ MC files | **Deferred / Rejected** unless a fresh production cycle is being launched anyway |
| **Tier 5** | Submission/downstream script patches to match Tier 3/4 renames | **Deferred** (tied to Tier 3/4) |

---

## 8. Related documents

- `docs/reviews/audit-proton-naming-highQ2-sidis.md` — the audit of record;
  contains per-file occurrence counts, line-by-line classifications, and
  cost analysis for each tier.
- `context-handoff.md` — v5b schema reality (`ChargedTot=86`, `ChargedTotGen=50`,
  `DisTot=49`).
- `docs/research/research-highQ2-sidis-extension.md` — high-Q2 extension
  requirements (note: earlier drafts referenced the package as
  `H1ProtonAsymHighQ2/`; corrected to `H1SIDIS_HighQ2/` in the Tier 1 pass).
- `docs/references/reference-LAr-electron-scale-highQ2-sidis.md` — LAr
  electron-scale reference; uses `proton_HiQ` extensively to refer to the
  SEPARATE proton/antiproton analysis (TRUE PROTON), not this SIDIS
  analysis. A glossary box has been added at the top.
