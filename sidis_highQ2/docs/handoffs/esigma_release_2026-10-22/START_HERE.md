# START HERE: eΣ-photon release handoff (2026-10-22)

This is a handoff bundle for a Claude session running **directly on NAF**. It continues work done by a Claude Code session on
the user's laptop, which reached NAF over ssh. That session ran the eΣ-frame re-production and wrote the release plan. The
next job is to **execute that plan**.

Bundle location: `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/handoffs/esigma_release_2026-10-22/`
(additive; no canonical file was edited to create it).

## 1. Read in this order
1. This file.
2. `esigma_release_plan_2026-10-22.md` (this directory). It is the plan, with paths translated to NAF. Section 1 lists the
   user decisions U1–U6 to ask first.
3. `reference/esigma_frame_critic_review_2026-10-22.md`: open items 1–12, which the plan closes.
4. `reference/esigma_frame_production_plan_2026-10-22.md`: what was built, the gates, results, and §7–10 (dispatch,
   results, the carry-bug correction).
5. `reference/elecE_scale_study_2026-10-22.md`: why eΣ (the E′-scale term with the electron photon was too large).
6. `reference/final_production_plan_2026-10-21.md` §10–11: the electron-frame final re-release, user decisions D1–D4.
7. `context/local_CLAUDE.md`: the laptop session's project notes: installed baselines, audits, the record of
   user-dispositioned items. Its "Access" section is about ssh; ignore that part here.
8. `context/memory/*.md`: the laptop session's persistent memory. The `feedback-*` files are user corrections and must be
   followed.
9. Remote house rules: `sidis_highQ2/AGENTS.md`, the `STATE.md` head plus a grep of its `## ...` override sections (never
   cat the whole ~200 KB file), and the matching `.opencode/skills/sidis-hq2-*/SKILL.md` before answering topical questions.

## 2. You are on the login node
- Interactive: one-file checks only, ≤5 min wall. All multi-file work goes through Condor (`condor_submit` from the
  relevant script dir).
- Docs written on the laptop show commands like `ssh klesthen@naf-hone.desy.de 'bash -s' <<'EOF' ... EOF`. Here you just run
  the body directly.
- The login shell is zsh. Use `bash -c` or `bash -s` for anything with variables or word-splitting, and quote `"====="` in
  echo.
- Never `pkill -f` a pattern that matches your own command.
- Do not write loose files at the top level of `sidis_highQ2/` or `H1ChargedSidis_HighQ2/`.

## 3. Path map (laptop doc paths → NAF)
| written in the docs as | on NAF |
|---|---|
| `docs/<name>.md` (laptop docs) | `reference/<name>.md` in this bundle |
| `scripts/final_rerelease/esigma_frame/` | `H1ChargedSidis_HighQ2/scripts/unfolding/esigma_frame_2026-10-22/` (authoritative; md5-identical to the laptop copies except `post.sub`, whose NAF copy has `from = groupB` and is the correct one) |
| `scripts/final_rerelease/` (s02a/s02b/s03 drivers, groups A/B/C, assemble, release, born, detlegs) | `H1ChargedSidis_HighQ2/scripts/unfolding/final_rerelease_2026-10-22/` (md5-identical) |
| `scripts/final_rerelease/elecE_study/` | `H1ChargedSidis_HighQ2/scripts/unfolding/final_rerelease_2026-10-22/elecE_study/` (except `make_photon_html.py`, which is laptop-only, see below) |
| `plots/esigma_frame_budget_2026-10-22/index.html`, `plots/photon_methods_7col_2026-10-22/index.html`, `plots/photon_options_2026-10-22/index.html` | laptop-only HTML wrappers. The PNG/PDFs are on NAF in `output_v6/studies/esigma_frame_2026-10-22/downstream/plots/{esph_quoted_total_23comp_7col,methods_7col}/` and `output_v6/studies/final_rerelease_2026-10-22/elecE_study/photon_options/plots/` |
| `local_only_scripts/` (this bundle) | `make_esph_page.py` and `make_photon_html.py`. They only build those HTML pages and use laptop paths, so they are not needed for the chain |
| `$H1`, `H1ChargedSidis_HighQ2/` | `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2` |
| outputs `output_v6/...` | `$H1/output_v6/...` |

## 4. Where things stand (2026-10-22 ~01:00; nothing canonical written in this phase)
- **Installed baseline (unchanged):** OPT-A Q2esGen release, electron reco photon.
  - Quoted budget `output_v6/central/data_release/systematics_bgsub_env100hyp_nonprimary_nw_optA_2026-10-21/`, medians
    7.4848% (h+) / 7.8604% (h−).
  - Covariance `data/covariance/z005_covariance_optA_nonprimary_v5_nw_2026-10-21/`.
- **Electron-frame final re-release (study, not installed):** 660 files, L_eff = 350,890.85 nb⁻¹, MC-only detector legs,
  ±1% E′, unfolded env100, DJANGOH C_rad. Median total 9.73% / 10.50%, dominated by the E′ term at 4.1–4.2%.
- **eΣ-frame re-production (study, not installed), user choice "eΣ everywhere":**
  - Reco hadron z/pT built with q = k − k′_eΣ. The generator target is unchanged, so C_rad needs no rerun (physics critic).
  - Production: 4343/4343 jobs exit 0; all null and cross gates passed bitwise; merge gates PASS 24.
  - Integrated cross-section shift: −0.04% (h+) / −0.05% (h−).
  - Median quoted total 8.20% / 8.80% (23 terms), with the E′ term at 2.68 / 2.88%.
  - Outputs in `$H1/output_v6/studies/esigma_frame_2026-10-22/{prod,downstream}/`; budget
    `downstream/budget/esph_budget.csv` plus `esph_budget_summary.json`.
  - Still carried from the electron frame: detint_removal (+model/stat), tracking_eff_m0p5, phi_h_gauss, and the ibu_model B1
    floor. These are plan items A2–A5.
- **Critic round 2:** physics PROCEED-WITH-CONDITIONS; chain NOT-YET. Open items are in the critic review. The phi_h/tracking
  mixed-frame bug the chain critic found is already fixed (Condor 5839586).
- **Condor:** nothing from this work is running.
  - The ~107 held jobs under klesthen belong to older or other projects: 2026-09 C_rad smoke, crad_xbinning 2026-10-08,
    proton dE/dx 2026-10-21, hit_level_sim. Leave them alone unless the user asks.

## 5. First action
Re-read the STATE.md head and the recent override sections, since peers may have changed things. Then ask the user the
**batched decisions U1–U6** (plan §1) in one round before any compute:
- U1: ISR leg and its κ;
- U2: Σ_h smear/noise leg;
- U3: JES form (recommend symmetrized (up−down)/2);
- U4: low-Q² parity;
- U5: one bundled install and the release name;
- U6: ±0.5% E′ legs out of scope.

The Phase A items that need no decision (A1 provenance, A2 B1 floor, A3 native Φ_h, A6 validation) may start once the user
says to proceed.

## 6. Working rules the user has set (do not skip)
- **Plain language.** The user asked for less jargon. Explain the physics, not internal labels (they once asked "explain
  OPT-A to me, your jargon is too confusing").
- **Critic review before any big compute dispatch.** Use independent critics, without feeding them the author's concerns.
  Run a broad smoke matrix through the real production wrapper, with storage and CPU projections, before asking for a
  large production.
- **Any new quoted term** needs a hostile critic review, user sign-off on κ, and user sign-off on the install. A covariance
  edition must pass gates g0–g7 + ga10, and a new BASE package needs its own gates.
- **Canonical edits** must be scripted, additive and sha-guarded, made after re-reading the file, with pre-edit backups and a
  `--dry` run first (pattern: `s11_promote…`, `s14_promote…`).
- **Concurrency:** a peer session (h1-sidis-e5) and an OpenCode server also write to this analysis. Re-check sha256s right
  before every edit.
- **Backgrounds are defined by truth-link provenance only** (STR-only links). Never define them by reco kinematics.
  Triple-check any background class with independent variables before quoting its size.
- **Do not re-raise user-dispositioned items:** the NucliaWeight 0.45% nominal bias, the NW h+/h− zero, the kept double
  counts (NW fake-leg vs bgsub_fakefrac, NW vs tracking_eff).
- Report outcomes faithfully. If a gate fails, say so with the output.

## 7. Pitfalls (also plan §9)
- Re-point modules only AFTER anything that must stay in the old frame has run, or carry such columns explicitly.
- `components_*` modules import path constants by name. Re-point every namespace.
- The release preflight needs `merged_response_sha256.txt` and `merge_manifest_{gen}.json` in the merged dir.
- An IBU null gate reproduces to ~1e-16 only within the same numpy build. Groups A/C use LCG_107a; the rest uses
  `/usr/bin/python3`.
- Condor `queue … from` splits on commas. Use `executable = /usr/bin/python3` rather than `bash -c` with quotes.
- In Python, a gate count of 0 is falsy. Compare counts explicitly.

## 8. Bundle contents
- `START_HERE.md`: this file.
- `esigma_release_plan_2026-10-22.md`: the plan, NAF-pathed. The verbatim laptop copy is in `reference/`.
- `reference/`: all 31 laptop docs (2026-10-20 → 2026-10-22: audits, candidates, critic reviews, promotions, the final
  production and eΣ docs, low-Q² binning).
- `context/local_CLAUDE.md`: laptop project notes. `context/memory/`: laptop session memory (MEMORY.md is its index).
- `local_only_scripts/`: the two HTML-page builders.
- `MANIFEST.sha256`: checksums of every file in this bundle.
