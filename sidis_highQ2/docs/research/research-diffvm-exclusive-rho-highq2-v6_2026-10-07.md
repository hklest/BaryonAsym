# Research: DIFFVM exclusive-ρ (and φ/ω) in the v6 high-Q² charged-hadron SIDIS fiducial

**Date:** 2026-10-07
**Author:** h1-physics-researcher (read-only research dispatch; no production submitted, no central-release edits)
**Scope:** High-Q² v6/z005 only (`Q² > 150 GeV²`, inclusive charged hadrons, no species PID). Never applies to low-Q² or v5g products except as explicitly labeled provenance.
**Question dispatched:** (1) Can the v6 builder digest DIFFVM, and do the inputs still exist? (2) Root cause of the exclusive-ρ null. (3) Literature expectation for the accepted exclusive-VM yield at 351 pb⁻¹. (4) Minimal empirical deliverable.

---

## Verdict

1. **The v6 high-Q² builder can digest DIFFVM with zero code changes.** MC samples are declared in v6 only through per-chunk `fHatFiles`/`fModsFiles` (stage 1) and `fEventListFiles` (stage 2) steering; the builder is generator-agnostic, and every HAT branch the v6 stage-1 and stage-2 read (22/22 checked) is present in the DIFFVM HAT files (verified by direct uproot schema read). **VERIFIED.**
2. **All 12 DIFFVM sample families are still on line** at `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/diffvm10/<runid>/` with 101 HAT + 101 MODS files each — including the ρ all-decay set 8445-8448 (~12 GB per sample directory). **VERIFIED by listing + two file reads.**
3. **The null is a coverage gap, not a physics result.** No DIFFVM sample was ever processed into v6 trees (output/steering evidence below), and no element of the v6 selection categorically vetoes a 2-prong exclusive-ρ event. **VERIFIED.**
4. **The single binding requirement is the event-level reconstructed-kinematics fiducial `Q2es > 150 GeV²`** (downstream final selection `build_migration_th2_per_charge_v4.py:239`; stage-1 reco-envelope precondition `q2 > 100` at `TreeBuilderHighQ2_chargedsidis_v6.C:764`, which presupposes a reconstructed scattered electron, `:699-711`). It is the only selection element that materially suppresses exclusive ρ; all track-level cuts listed in the dispatch (nhit, η, z, pT_h, radlen, alpha, electron-track exclusion) pass for ρ daughters. **VERIFIED for the cut contents; the survival physics is ANALYTIC/INFERRED (labeled below).**
5. **Expected accepted exclusive-VM charged tracks in the v6 fiducial at 351 pb⁻¹: ~300-750 (central ~450 ρ; ~500 all-VM), i.e. ~(0.8-1.9)×10⁻⁴, central ~0.012%, of the 3,865,073-track denominator — with a worst corner of ~0.2-0.5% in (z ≥ 0.7, low pT_h, lowest Q² stratum).** Two mechanisms contribute: true-Q²>150 elastic events (dominant, directly measured rate) and fake-scattered-electron leakage from the low-Q² bulk (v5g-provenance rate, mostly killed by |η|<1.6). **INFERRED from direct HAT measurement + literature anchors + kinematics; factor-2 honest.**
6. **MC statistical power:** with the combined ρ scale L_data/L_MC = 351/2582 = 0.13594, a v6-processed ρ sample would contain ~1,800-4,800 accepted tracks, measuring the integrated fraction at ~2% relative statistics and resolving any integrated true fraction above ~3×10⁻⁷ (3σ floor) — ~250× finer than the expected ~10⁻⁴. The scoping's never-applied 0.5%/0.05% corner assignments are testable at ≥10σ. **INFERRED from measured rates.**
7. **Minimal deliverable:** process ρ all-decay 8445-8448 through v6 stage-1 evls + stage-2 sidistree (404 + 404 = **808 standard Condor jobs**, ~0.5-1 GB output), apply the v6 finalcandidate selection, package accepted yield as reco980 `yield_980`/`var_980` vectors per sample. Not executed here; route to `h1-script-runner`/`h1-production` under a studies directory (never central).

---

## Coverage-gap evidence

**No DIFFVM ever entered v6 production.**

- `H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/` = `{djangoh14, rapgap31}`; `.../sidistree/mc_bg/` = `{djangoh14_lowQ2bkgd}` (directory listing, 2026-10-07). Caller-established: no `*diffvm*` anywhere in the v6 tree; `grep -ril diffvm condor_v6/` empty.
- `H1ChargedSidis_HighQ2/steering_v6/central/evls/mc/` = `{djangoh14, rapgap31}` only; v6 signal-MC evls steering declares samples purely as `fHatFiles`/`fModsFiles` pointing at `/pnfs/desy.de/dphep/online/h1/mc2/oo-4.0/<generator>/<runid>/hat|mods.*.root` (example: `steering_v6/central/evls/mc/djangoh14/7072/evls_v6_djangoh14_7072_em06_0000.steer`).
- `condor_v6/central/sidistree/wrappers/mc/` = `{djangoh14, rapgap31}`; `condor_v6/central/evls/wrappers/mc/` = `{djangoh14, rapgap31}`; `condor_v6/central/evls/mc_bg/` and `sidistree/mc_bg/` = `{djangoh14_lowQ2bkgd}` only.
- Zero DIFFVM references in the v6 sources: `grep -n diffvm` on `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` (3130 lines) and `src/ChargedSidisTreeBuilderHighQ2_v6.C` (2123 lines) returns nothing.
- `sidis_highQ2/STATE.md:37`: DIFFVM (ρ 8445-8448 all-decay, φ, ω) "is an ASSUMED ZERO and remains unassessed"; `STATE.md:27` records the never-applied scoping assignment (0.5% at z≥0.7 in 150-300 GeV² / 0.05% elsewhere) as OPEN.
- The handoff card `docs/handoffs/background_mc_diffvm.md:50-55` points at `H1SIDIS_LowQ2_v5g/output/mc/diffvm_*` (~55 GB, 12 samples) as reusable. **That path is dead**: each `diffvm_*` directory now contains only `_root_deletion_manifest_20260526_dst_staging.txt` (verified for `diffvm_rho_8445`, `diffvm_phi_8408`; the 2026-05-26 DST-staging ROOT deletions, ~52 MB/file × ~100 files/sample).
- **New finding (does not revive the dead path):** a complete v5g-binary DIFFVM re-processing exists at `H1SIDIS_LowQ2_v5g/output/mc/background/v5g_t11_2026-05-26/diffvm_{rho,phi,omega}/`, including the all-decay set `diffvm_rho_844{5,6,7,8}_DST0000-0100.root` (101 chunks each). Verified on `diffvm_rho_8445_DST0000.root`: DisTot = 90,000 entries = **all** generated events (the high-Q² tail is retained: 4 events with `Q2esGen>150`, matching the HAT count; 494 events with reco `Q2es>150` from fake electrons). **But it is wrong-regime, low-Q²-builder schema and cannot feed the canonical v6 chain**: DisTot lacks `GenEmpz`, `ElecE/ElecTheta/ElecPhi`, `GenScatE{X,Y,Z,E}`, `Q2_es_gen/Y_es_gen/x_es_gen/ystar_es_gen`, `GenEnPhoton`, and the v6 envelope flags (`pass_reco_envelope` absent); ChargedTot lacks the inlined `Q2_es_gen/Y_es_gen/x_es_gen` and per-row `GenEmpz`. The card's recommended path — reproduce with the high-Q² builder — stands.

**Conclusion:** "no exclusive-ρ signal has ever been seen" in v6 products because **no high-Q² DIFFVM sample was ever processed**. The ~300-750 expected exclusive-VM tracks are presumably present inside the 3,865,073 selected data tracks, unmodeled and unassigned. This is a coverage gap, not a physics null. **VERIFIED.**

### Input availability per sample family (bounded probes, 2026-10-07)

| Family | Sample | Path probe | Verdict |
|---|---|---|---|
| ρ⁰ all-decay (use ONLY these) | 8445, 8446, 8447, 8448 | `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/diffvm10/<r>/`: 202 entries each = 101 `hat.*` + 101 `mods.*` (~12 GB/dir); two HAT files read directly (8445 DST0000, 8447 DST0050) | **AVAILABLE** |
| φ | 8408, 8413, 8418, 8423 | same pattern, 101+101 each (listed) | **AVAILABLE** |
| ω | 8409, 8414, 8419, 8424 | same pattern, 101+101 each (listed) | **AVAILABLE** |
| ρ charged-only (NEVER sum with all-decay) | 8407, 8412, 8417, 8422 | present on line (8407 confirmed in `diffvm10/gen/04/` and `oo-4.0/diffvm10/`) | available but **forbidden** (double counting) |

File-name pattern (from the 2026-era low-Q² steering and confirmed by listing): `hat.4.0.6.diffvm11001_rho_{eminus,eplus}_ela_job{3,4}.<r>.S39400.R97901.ftt.DST.<NNNN>.root` + matching `mods.*`. There is no RAD/NORAD leg structure for DIFFVM (single elastic generation, `ela`); "RAD-pass inputs" in the dispatch is interpreted as these standard HAT/MODS pairs. DIFFVM is unweighted (`Weight1=Weight2=1.0` — verified identically 1.0 on 90,000 events) and `oomclumi` is invalid (GKI SIGMT unfilled), so normalization is `L_data/L_MC` per sample against IMC lumi (645.50/645.69/645.34/645.20 pb⁻¹; combined 2582 pb⁻¹).

**Schema digestibility (direct measurement, 8445 DST0000 HAT, 219 branches):** all 22 stage-2 HAT pointers (`EBeamE, EBeamP, ElecE, ElecPhi, ElecTheta, GenEnElec, GenEnHad, GenEnPhoton, GenPhElec, GenPzHad, GenThElec, Il1l2l3ac, Il1l2rw, Il1l3rw, Q2s, Q2sGen, RunNumber, VtxZ, Weight1, Weight2, Ys, YsGen`) present; all stage-1 gen-envelope branches (`Q2sGen, YsGen, GenEnElec, GenThElec, GenEnHad, GenPzHad, VtxZ`) present. `Q2sGen` spans [1.000, 232.97] (mean 2.577) on 8445 DST0000 — the sample is the physical elastic-ρ distribution over Q² > 1 GeV², unweighted. `GenEmpz` reconstructed from HAT gen branches is **constant 55.20 GeV** (= 2E_e elastic energy conservation) — i.e. every elastic DIFFVM event passes `Empz > 45` by construction.

---

## v6 cut-survival analysis (can a 2-prong exclusive ρ event survive?)

Authoritative selection: stage-1 `src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` (envelope-OR), stage-2 `src/ChargedSidisTreeBuilderHighQ2_v6.C` (fill gates), downstream finalcandidate `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py` (v4 four-cut contract; mirrored in `sidis_highQ2/scripts/unbinned_v2/omnifold_multifold_v6z005.py:22`).

| Dispatch-listed cut | Actual v6 status (file:line) | ρ-daughter survival |
|---|---|---|
| `nhit >= 5` | `NHitDedx_min: 5` — `build_migration_th2_per_charge_v4.py:272` | **Passes.** Clean, energetic central π± tracks. |
| `η < 1.6` | `Eta_abs: 1.6` — `:271` | **Passes for true-Q²>150 events.** At Q²=200, y=0.3 the VM lab 4-vector is (E≈12.5 GeV, pT≈11.8, pz≈−4.0) → η_VM ≈ −0.33; daughters collimated within ~M_ρ/E_VM ≈ 0.06 rad. (Low-true-Q² fake-electron events have η_VM ≈ −1.8 to −4.4 → daughters mostly FAIL; see Literature section.) INFERRED from exact elastic kinematics (E−pz = 2E_e·y conservation + mass shell). |
| `z >= 0.05` | **Not a selection cut.** Reco z binning starts at 0.0: edges `[0.0, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0]` (`binning_v3.py:44`, `plots/reco_backgrounds_alpha0p2_z005_2026-06-24/background_fractions_7col/BINNING_AND_SCHEMA.md:45`). The `zlow005` label = the 0.05 z-edge, not a threshold. | **Passes.** Daughter z ≈ (1+cosθ*)/2 · z_VM with z_VM ≈ 1 → z ~ U(0,1): ~95% above 0.05, all below 1. Even z<0.05 daughters are counted (first bin). |
| pT_h threshold | **Not a selection cut.** Reco pT_h edges start at 0.0: `[0.0, 0.25, 0.5, …, 8.0]` (`binning_v3.py:74-75`, `BINNING_AND_SCHEMA.md:46`); `pT5` in the release label refers to the 5 gen pT_h bins. Support is pT_h ∈ [0,8) GeV. | **Passes — and is confined.** pT is preserved under the longitudinal boost from the VM rest frame, so daughter pT_h ≈ (M_ρ/2)·sinθ* ≤ 0.385 GeV, plus t-smearing (√⟨|t|⟩ ~ 0.3-0.6 GeV) → ρ tracks land almost exclusively in reco pT_h bins 0-1 (0-0.5 GeV). **Exclusive VMs cannot reach pT_h > ~1 GeV** — the contamination corner is high-z × LOW pT_h, not the high-pT_h corner the 2026-05-24 scoping table assumed. INFERRED (two-body kinematics). |
| Radiation length | `RadLength_min: 5.0` — `build_migration_th2_per_charge_v4.py:272` | **Passes** for central tracks. |
| `ALPHA < 0.2` near-electron veto | **Not in the central finalcandidate selection** (absent from `EVENT_CUTS_RECO`/`TRACK_CUTS_RECO`, `:238-274`). `alpha0p2` is the diagnostic/provisional background-candidate veto (`sidis-hq2-backgrounds` skill; `STATE.md:27,34`: central is raw/no-bgsub, electrons and V0 removed by matched-classification vetoes and response fake-fraction subtraction). | **Passes anyway.** At high Q² the daughters follow q̂ = −k̂'᷀T while the scattered electron is on the opposite side transversely; opening angle ≫ 0.2 rad. INFERRED. |
| Builder's scattered-electron track exclusion | `if (trk->GetParticle() && trk->GetParticle()->IsScatElec()) continue;` — `ChargedSidisTreeBuilderHighQ2_v6.C:1581` (central) and `:1922` (combined) | **Passes.** π± daughters are never H1OO-tagged as the scattered electron. (In fake-electron events the mis-tagged π track is excluded here, further reducing that route.) |
| Other fill/selection gates | `IsFromPrimary` (`:1585`), central/combined track type (`:1588`, `:1953`), `IsMatchedElectron==0`, `IsMatchedV0Daughter==0` (`build_migration_th2_per_charge_v4.py` header `:48-52`), `|DcaPrime/coshη|<2` (`:273`), `|PVz|<35` (`:267`) | **Pass.** ρ daughters are primary charged pions, not electrons or V0 daughters. |
| Event-level kinematics | `150 < Q2es < 20000`, `0.1 < Yes < 0.7`, `Empz > 45` — `build_migration_th2_per_charge_v4.py:238-246`; xF current hemisphere `xF_SIDIS_es < 0` (`:243-245`); stage-1 reco envelope (electron E/θ, Empz 30-75, Q2>100, y 0.05-0.9, BG-finder and QED-Compton gates) — `TreeBuilderHighQ2_chargedsidis_v6.C:699-832`; MC gen-envelope OR rescue (Q2sGen>60, 0.01<YsGen<0.99, 25<GenEmpz<75) — `:834-859` | **Empz>45 passes ALWAYS** (elastic E−pz = 2E_e = 55.06; measured GenEmpz ≡ 55.20). **xF<0 passes**: the VM follows q̂, the current-hemisphere direction; daughters are collimated. **y window passes for ~2/3** of the Q²>150 tail (measured 8/12 events in (0.1,0.7) on two chunks). **Q2es>150 is the binding requirement** (see below). |

**No event-level charged-multiplicity cut exists anywhere in v6** (stage-1 CUT 1-14 list, `TreeBuilderHighQ2_chargedsidis_v6.C:645-832`; the v4 contract's "FOUR physics-fiducial cuts", `build_migration_th2_per_charge_v4.py:232-253`). The low-Q² card's suggested `N_central ≥ 3` mitigation (`docs/handoffs/background_mc_diffvm.md:96`) was never adopted. A 2-prong event is therefore fully admissible.

**Single binding requirement:** the reconstructed high-Q² DIS fiducial, `Q2es > 150 GeV²` — `build_migration_th2_per_charge_v4.py:239` (`"Q2_min": 150.0`), with its stage-1 precondition of a found scattered electron (`TreeBuilderHighQ2_chargedsidis_v6.C:699-704` CUT 3, `:706-711` CUT 4) and reco-Q² envelope (`:757-767`, `q2 <= 100.0` reject). Directly measured, only **4/90,000** (8445 DST0000) and **8/100,000** (8447 DST0050) DIFFVM events have `Q2sGen > 150`; combined **(4+8)/(190,000) = 6.3×10⁻⁵**, of which ~2/3 also satisfy 0.1<y<0.7. Every other selection element passes for the ρ topology. Note the stage-1 gen-envelope OR (`:834-859`) keeps the high-Q² tail even when the reco electron is missed (measured 56-61 envelope passes per chunk, a superset of the Q²>150 events), so stage 1 does not pre-clamp the tail.

---

## Literature expectation (accepted exclusive-VM yield at L_data = 351,000 nb⁻¹)

**Anchors (verified against local docs; published values as cited there):**
- H1 elastic ρ⁰: σ(γ*p→ρ⁰p) ≈ 850 nb at Q²=2.5, W=75; Q²-fall ~(Q²+M_V²)^−n, n≈2.5; H1 JHEP 1005 (2010) 032 (measured to Q²≈90). `docs/references/reference-exclusive-vmd-background.md:97-101` (LOW-Q² PROVENANCE).
- H1 elastic φ: 72.6 nb at Q²=2.71 (EPJC C46 (2006) 585); ω: ~80 nb at Q²=2.5 (Z. Phys. C75 (1997) 607). Same source.
- Low-Q² measured per-event survival (v5c DIFFVM smoke, LOW-Q² PROVENANCE): DIS-window survival ≈ 9.6%, **0.61 accepted tracks per DIS-passing ρ event** at |η|<1.3, 32% of ρ tracks at z>0.7. `reference-exclusive-vmd-background.md:70-74,153-161`.
- 2026-05-24 scoping (LOW-Q² PROVENANCE, superseded luminosity 297,657.8 nb⁻¹): flux-integral σ_ep(ρ, Q²>150 window) ≈ 0.001 nb → ~300 events → ~180 ρ tracks → 0.009% of an assumed ~2M-track sample. `docs/methodology/background_scoping_2026-05-24.md:202-216`. Its own direct measurement — **17/90,000 events pass `Q2es>150 ∧ Empz∈[35,70] ∧ 0.1<Yes<0.7`** on `diffvm_rho_8407_DST0000` — was reported but not used for the normalization (`:194-200`).

**Direct high-Q² route (this work, VERIFIED inputs):** the on-line DIFFVM HATs are the physical unweighted elastic distribution over Q²>1. Per-chunk IMC lumi = 2582 pb⁻¹ / 404 chunks ≈ 6.39 pb⁻¹. Measured: σ_ep(elastic ρ, Q²>150) ≈ 4/(90,000/6390 nb⁻¹) ≈ 0.6 pb; with the y-window, ≈ 0.42 pb.

| Step | Value | Basis |
|---|---|---|
| ρ events with Q²_gen>150, 0.1<y<0.7, 351 pb⁻¹ | **~220 ± 60** (stat) | measured 8/190,000 on two chunks × (38M events × 0.13594) |
| reco retention (electron found, eΣ fiducial; Empz passes by construction) | 0.7-0.9 | INFERRED (high-Q² electrons clean; ±3-5% Q² resolution at the 150 edge) |
| accepted tracks per event | 1.4-1.8 | 2 π± (BR≈100%) × per-track acceptance (central η, quality cuts, tracking) — INFERRED; replaces the low-Q²-measured 0.61, which is inapplicable because at low Q² the ρ is backward (η ≈ −1.8 to −4.4 by E−pz kinematics) and the |η|<1.3 cut removed most daughters, while at Q²>150 the VM is central (η ≈ −0.3 to −1) |
| **True-Q² ρ accepted tracks** | **~250-400 (central ~300)** | product; factor-2 honest |
| Fake-electron route: low-true-Q² events with a π⁺ mis-tagged as the scattered electron reconstructing Q2es>150 | ~990 events × 0-0.3 accepted tracks → **~0-300 tracks (central ~150)** | 17/90,000 (v5g provenance, one 8407 chunk) × data-equivalent generated events (L_data·σ_ep(Q²>1) = 351,000 × ~14.9 nb ≈ 5.2M); daughters at η ≈ −1.8 to −4.4 mostly fail \|η\|<1.6 (INFERRED); the mis-tagged track is excluded by the builder's `IsScatElec` veto |
| **Total ρ accepted tracks** | **~250-650 (central ~450)** → **fraction (0.65-1.7)×10⁻⁴, central ~0.012% of 3,865,073** | two mechanisms, independent systematics |
| φ: ×~0.18 (sample-lumi σ ratio) × BR(K⁺K⁻)=0.491; ω: ×~0.09 × BR(π⁺π⁻π⁰)=0.892 | φ ~20-60, ω ~20-60 tracks | INFERRED; φ/ω together ≈ +15% |
| **All exclusive VM** | **~300-750 tracks (central ~500) → ~0.008-0.019%, central ~0.013%** | |

**Cross-checks and discrepancies (labeled):**
- The scoping's flux route gives 212 ρ tracks at 351 pb⁻¹ (0.0055%) — consistent with this work's true-Q² route once the per-event track yield is corrected upward for the high-Q² central topology (their 0.61 is a low-Q² number). LOW-Q² PROVENANCE for their inputs; the disagreement is understood, not forced.
- The scoping's **integrated** 0.009% is therefore likely ~1.5-3× low, but its **worst-corner** assignment survives: ~30% of ρ tracks sit at z>0.7 (isotropic decay → z~U(0,1); matches the low-Q²-measured 32%), essentially all at pT_h<0.5 GeV, and ~70-80% in the lowest Q² stratum. Against an inferred data yield of ~2-5×10⁴ tracks in (z≥0.7, pT_h<0.5, S0), the local fraction is **~0.2-0.5%** — the same order as the scoping's never-applied 0.5% at z≥0.7 in [150,300). The corner is at LOW pT_h, not the high-pT_h corner of the scoping table (exclusive VMs cannot make pT_h > ~1 GeV).
- All numbers above are INFERRED expectations pending the empirical deliverable; the only VERIFIED quantities are the per-chunk event counts, the elastic GenEmpz ≡ 55.2, the unweighted weights, and the 17/90,000 v5g-era leakage rate.

---

## MC statistical power

Combined ρ all-decay scale: L_data/L_MC = 351,000/2,582,000 = **0.13594** (per-sample ≈ 0.5438-0.5440; φ 0.02271; ω 0.01135).

- Expected accepted ρ tracks in the v6-processed MC: N_MC ≈ (250-650)/0.13594 ≈ **1,800-4,800** (central ~3,300).
- The data-equivalent integrated fraction f = 0.13594·N_MC/3,865,073 is then measured with relative statistical error 1/√N_MC ≈ **1.6-2.4%** — i.e. the expected ~10⁻⁴ is measured, not merely bounded.
- **Minimum true integrated fraction resolvable (3σ nonzero, N_MC ≳ 9-10):** f_min ≈ 10×0.13594/3,865,073 ≈ **3.5×10⁻⁷ (0.000035%)** — ~250× below the expected central value. The sample over-resolves the question by two orders of magnitude.
- Per-aggregation: z≥0.7 slice ≈ 1,000 MC tracks → ~3% relative; per fine reco980 bin: bins with ≥100 MC tracks (S0/S1, z≥0.5, pT_h<0.75 — exactly the contamination corner) resolve per-bin fractions to ≤10% relative; high-Q² strata (S2/S3) will hold <10 tracks/bin → limit-only statements there.
- φ/ω: N_MC ≈ 250-700 each → integrated fractions measured at 4-6% relative.

**Implication:** a single v6 ρ campaign can (a) confirm or kill the scoping's 0.5%/0.05% corner assignment at ≥10σ, (b) deliver a per-bin reco980 vector usable either as a fifth background component or as a validated systematic, and (c) empirically separate the true-Q² and fake-electron mechanisms (via `Q2esGen` on the same trees).

---

## Minimal deliverable (specified, NOT executed)

**Goal:** settle the assumed zero empirically — accepted exclusive-ρ yield under the exact v6 selection, packaged for the background chain.

1. **Process ρ all-decay 8445-8448 with the existing v6 binaries (zero code changes):**
   - Stage 1 (`TreeBuilderHighQ2_chargedsidis_v6 --stage evls --mode mc`): per-chunk steering declaring `fHatFiles`/`fModsFiles` for each of the 101 chunks × 4 samples → **404 jobs**.
   - Stage 2 (`ChargedSidisTreeBuilderHighQ2_v6 --mode mc`): per-chunk steering declaring `fEventListFiles` → **404 jobs**.
   - **Total: 808 standard Condor jobs** (1 core, 2 GB, 3 h class; ~10-15 min each expected by low-Q² precedent and the 2026-06-10/11 mc_bg campaign timing). Optional φ+ω extension: +1,616 jobs (12-sample total 2,424).
2. **Inputs (verified on line, read in place from /pnfs — no staging):** `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/diffvm10/{8445,8446,8447,8448}/{hat,mods}.4.0.6.diffvm11001_rho_*_ela_job*.<r>.S39400.R97901.ftt.DST.<0000-0100>.root` (808 files, ~48 GB). Also required: the GRL file (wrapper argument; run selection is skipped for MC — `TreeBuilderHighQ2_chargedsidis_v6.C:681`), `FidVolCut.steer` in the job CWD (`:449-451`), and the existing v6 binaries (md5-guarded; do not rebuild).
3. **Location discipline:** a studies campaign (e.g. `condor_v6/studies/diffvm_rho_<date>/` + `output_v6/studies/diffvm_rho_<date>/`), mirroring the mc_bg pattern — never `condor_v6/central/` or `output_v6/central/`.
4. **Selection + packaging:** apply the v6 finalcandidate reco selection (`build_migration_th2_per_charge_v4.py:238-274` event/track cuts + xF<0) with weight 1.0; scale per sample by L_data/L_MC (IMC lumi 645.50/645.69/645.34/645.20 pb⁻¹); package as charge-major **reco980** `yield_980`/`var_980` vectors (490 h+ then 490 h−; `r = cell*70 + z_idx*10 + pT_idx`), per the alpha0p2 packaging convention (`plots/reco_backgrounds_alpha0p2_z005_2026-06-24/background_fractions_7col/BINNING_AND_SCHEMA.md:27-46`). Produce two variants: central selection and alpha0p2-veto, plus per-sample and combined, with per-sample IMC-lumi provenance retained. Adapt the existing `build_lowq2_djangoh_bg_hist_v6_alpha0p1_reco980.py` pattern.
5. **Expected output size:** the stage-1 envelope keeps only ~6×10⁻⁴ of events (~56-61 per chunk, measured), so sidistree files are ~0.5-2 MB each → **~0.5-1 GB total for ρ** (the card's 55 GB figure applies only to the low-Q² build, which kept ~10% of events). Evls outputs similar or smaller.
6. **Decision rule:** if the combined accepted fraction is <0.05% integrated and <0.5% in every (Q²,x,z,pT_h) bin → document as a systematic (validates the scoping assignment; route the assignment to `h1-systematics`). If any bin exceeds → promote to a fifth background component in the alpha0p2-family chain (route to `h1-systematics`/`h1-unfolder` for exactly-once accounting; the backgrounds skill requires DIFFVM to be either validated-assessed or documented-systematic, never assumed zero).
7. **Routing:** submission and execution belong to `h1-script-runner`/`h1-production` under supervisor approval; this research dispatch does not run it.

---

## Open questions

1. **Chunk-size variance and exact N_gen:** measured 90,000 (8445 DST0000) vs 100,000 (8447 DST0050) events per chunk; total N_gen ≈ 38M is extrapolated. A Condor-side count (or the stage-1 cutflow logs of the deliverable) fixes σ_ep(Q²>1) = N_gen/L_MC exactly and tightens the ±29% Poisson on the 12-event tail count. **Unresolved, non-blocking.**
2. **Fake-electron mechanism under v6 gates:** the 17/90,000 leakage rate was measured with v5g-era cuts (Empz∈[35,70]) on one 8407 chunk (charged-only). The v6 stage-1 adds BG-finder, QED-Compton, ye, Zimpact/FidVol gates that may change it, and the daughter |η|<1.6 acceptance for backward ρ events is uncertain at the factor-2 level. Only the deliverable separates this mechanism from the true-Q² route (via `Q2esGen` vs `Q2es`). **Unresolved.**
3. **t-slope at Q²>150:** the daughter pT_h smearing above M_ρ/2 depends on b(Q²), not measured above Q²≈90 (H1 JHEP 1005:032 reaches ~90). Affects only the pT_h-bin population, not survival. **Unresolved, minor.**
4. **Proton-dissociative DIFFVM:** the 8445-8448 set is elastic (`ela`). Proton-dissociative ρ (unstated here) would add ≤2× at low Q² per the reference doc's uncertainty band; at Q²>150 its W/t phase space is similar and it is NOT covered by any on-line sample identified in this dispatch. If the elastic result lands near a decision threshold, the dissociative contribution needs a separate estimate. **Unresolved.**
5. **S67 trigger simulation on DIFFVM:** `TrigWeightAC` is filled by the builder for MC; DIFFVM 2-track events at high Q² rely on the electron trigger leg — expected efficient, but worth one cutflow check in the deliverable (stage-1 counters exist). **Unresolved, minor.**
6. **Low-Q²-built v5g_t11 trees:** now known to exist and to retain the high-Q² tail, but they cannot feed the canonical v6 chain (missing branches listed above). A bespoke extractor over them could deliver a poor-man's cross-check without any production, at the cost of exact v6-selection parity. Worth considering only if the 808-job campaign is deferred. **Optional.**

---

## Source register (primary evidence)

- `sidis_highQ2/STATE.md` lines 12, 27, 34, 37 (lumi, DIFFVM assumed-zero OPEN item, central bgsub semantics, event-level inventory).
- `sidis_highQ2/docs/handoffs/background_mc_diffvm.md` (sample card: lumi, weights, dead reuse path, recommended reproduction).
- `sidis_highQ2/docs/methodology/background_scoping_2026-05-24.md` §3.3 (low-Q²-provenance estimate; 17/90,000 measurement; NO-GO recommendation).
- `sidis_highQ2/docs/references/reference-exclusive-vmd-background.md` §4-9 (low-Q² measured survivals, cross-section anchors, z-distribution).
- `H1ChargedSidis_HighQ2/src_stage1/TreeBuilderHighQ2_chargedsidis_v6.C` lines 367-993 (stage_evls; cuts 1-14; gen envelope 834-859).
- `H1ChargedSidis_HighQ2/src/ChargedSidisTreeBuilderHighQ2_v6.C` lines 1542-1548, 1581, 1922 (envelope gating; IsScatElec exclusion).
- `H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge/build_migration_th2_per_charge_v4.py` lines 232-274; `binning_v3.py` lines 44-45, 74-75.
- `H1ChargedSidis_HighQ2/steering_v6/central/evls/mc/djangoh14/7072/evls_v6_djangoh14_7072_em06_0000.steer`; `condor_v6/central/evls/mc_bg/djangoh14_lowQ2bkgd/steering/evls_lowQ2bkgd_7087_em0405_0000.steer`; matching wrappers.
- Direct probes (2026-10-07): `/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/diffvm10/{8445..8448,8408,8413,8418,8423,8409,8414,8419,8424}/` listings (101 hat + 101 mods each); uproot reads of `hat.4.0.6.diffvm11001_rho_eminus_ela_job3.8445...DST.0000.root` (HAT: 90,000 events, 219 branches, Q2sGen∈[1.0,233.0], GenEmpz≡55.20, weights≡1.0, 4 events Q²>150, 56 gen-envelope passes, 3 with y∈(0.1,0.7)) and `hat...8447...DST.0050.root` (100,000 events, 8 events Q²>150, 5 with y-cut); `H1SIDIS_LowQ2_v5g/output/mc/diffvm_rho_8445/` (manifest only) and `.../background/v5g_t11_2026-05-26/diffvm_rho/diffvm_rho_8445_DST0000.root` (90,000 DisTot, 4 Q2esGen>150, 494 Q2es>150, 102,140 ChargedTot, v6 branches missing as listed).
