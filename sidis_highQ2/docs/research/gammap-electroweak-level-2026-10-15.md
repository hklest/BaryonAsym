# Electroweak level for a sigma(gamma* p) release — physics-convention review (2026-10-15)

**Author:** h1-radiative-correction (embedded compatibility run; the `sidis-hq2-radiative` skill is not loadable in this runtime — precedent `docs/radiative/EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` §7f; STATE.md, the skill file, and the authoritative docs were read directly).
**Scope:** READ-ONLY physics-convention review of the electroweak / radiative level of a prospective sigma(gamma* p) presentation of the high-Q2 inclusive charged-hadron SIDIS cross sections. The virtual-photon FLUX FACTOR is explicitly out of scope (owned elsewhere); only its alpha-convention consistency requirement is stated here. No existing file or release was modified; this memo is the only new file.
**Validated basis (verified):** 2026-10-13 cone triad per `sidis_highQ2/STATE.md`; Born release `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/born_crad_dj14relbin_detints2prime_alphaveto_cone_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-13/` (IBU h+ `13.026987` / h− `12.697511` nb).

---

## 0. Verdict summary

1. **Required EW level (Q1):** a published sigma(gamma* p) must be **gamma-only Born** — pure one-photon exchange with the Z exchange and the gamma–Z interference removed. Born-with-Z is an *ep-level* convention (the current primary product), not a gamma* p level; no cited H1/HERMES result quotes a "dressed/absorbed" gamma* p cross section. The one sub-convention that must be pinned and stated is the loop content: the delivered sidecar targets **pure photon Born at fixed alpha(0)** (recommended, consistent with the alpha(0)-based flux and the one-photon-exchange SIDIS formalism); the photon-only *dressed* alternative (running alpha + soft&virtual, NOZ1-type) exists as a sample but not as a table.
2. **F_EWgamma audit (Q2):** **PASS — fit for purpose** for the pure-Born-alpha(0) convention, with one mandatory labelling correction: it removes **Z exchange AND gamma–Z interference AND the full 1-loop content** (vacuum polarization / running alpha, soft&virtual, weak self-energies). It is NOT a Z-only factor: of the integrated −8.8%, only ≈ −0.35% is Z (mixed beam); ≈ −7.9% is loop content. The ~8–9% size and its Q² trend are physically plausible and anchored.
3. **C_rad release-grade today (Q3):** **NO.** The applied C_rad chain is PROVISIONAL and not publication-grade (`P-CRAD-LUMI` generator-log ceiling, permanently blocked externally), and the photon-Born sidecar is unapplied (`c_rad_applied=false`), DJANGOH-only, PROVISIONAL DIAGNOSTIC. Mathematically the chain is already the exact base (C_gamma_born = C_rad × F_EWgamma, hard-gated); the gaps are status, one missing generator leg, and the absence of an installed gamma* p release product.
4. **New systematic legs (Q4):** (i) generator dependence of F_EWgamma — unmeasurable with existing samples, anchored by the kappa-proxy (medians 0.622%/0.619% at kappa=0.02, SWAP not additive); (ii) PDF dependence of the Z-interference term — bounded ≲0.1% integrated (common-mode CTEQ6L cancellation), covered by the kappa anchor; (iii) F_EWgamma numerator MC statistics — median 0.551%, max 7.84%, swaps for `c_rad_stat_rel`; (iv) per-beam/per-charge legs — NONE required at the photon target (exact tree-level charge symmetry). Expected budget impact: ≲ +0.05 pp on the median totals (5.80%/6.19%).
5. **Base-product decision (Q5):** a gamma* p release **must** use the photon-Born sidecar (C_gamma_born) as its applied factor; the full-Born (gamma+Z) release **must remain the primary product**, with sigma(gamma* p) as a derived, separately labelled alternative convention — the two-convention structure the analysis note already implements.

---

## 1. Q1 — Which electroweak level is the convention for sigma(gamma* p)?

### 1.1 Answer

**Gamma-only Born**: the pure one-photon-exchange cross section, with the pure Z² term and the gamma–Z interference removed. Two sub-choices must additionally be declared on any gamma* p quote:

- **Loop content:** pure Born at fixed alpha(0) = 1/137.0359895 (the delivered NoZ target: HERACLES hard-codes ALPHA at Born, `LPIN(2)=0` switches off the soft&virtual 1-loop, `LPIN(7)=2` is inert at Born) versus photon-only *dressed* (Born + soft&virtual + running alpha, the NOZ1 configuration). **Recommendation: fixed-alpha(0) pure Born**, because (a) it is the level of the one-photon-exchange SIDIS formalism that defines gamma* p-level quantities — the Bacchetta/Diehl/Goeke/Metz/Mulders/Schlegel cross section with the alpha²/(x y Q²) prefactor (`docs/research/research-LT-separation-feasibility.md` §1.1; Trento conventions per `docs/references/reference-sidis-frame-gamma-star-p.md` §2, §6.3); (b) the standard virtual-photon flux (Hand convention; the epsilon(y) definition already used in the LT study §1.1) is an alpha(0) object — a dressed sigma(gamma* p) divided by an alpha(0) flux would be an internally inconsistent hybrid (consistency to be confirmed with the flux-factor owner; flux is out of scope here); (c) it is the already-derived, gated, note-integrated product (the note's noZ tables apply exactly this factor); (d) the only photon-only theory curve in the current comparison package (MAPTMD24) is photon-only.
- **Statement duty:** every gamma* p quote must state the EW level ("pure photon-exchange Born at fixed alpha(0), Z and gamma–Z removed, QED loops removed"), the applied factor (DJANGOH14 NoZ releasebin), and the flux convention. The note's own rule applies verbatim: the two Born conventions "map the same measurement to different Born conventions and must not be composed, averaged, or interchanged; any quoted cross section must state which of the two Born conventions it uses" (`sections/radiative_corrections.tex` lines 188–191).

### 1.2 Justification from H1 and identified-/inclusive-hadron SIDIS gamma* p results in the workspace

1. **By definition.** sigma(gamma* p) is the cross section of *virtual-photon*–proton scattering. At this analysis's Q² the Z content is not negligible — full/photon-only = **+0.60% (e−) / +0.11% (e+) integrated at Q²>100, corrected level; +8.61% (e−) / −0.50% (e+) at Q²>1000, Born** (`docs/radiative/BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md` §4.3–4.4) — so a "gamma* p" cross section that retained Z exchange would be mislabelled by up to several percent in the top cells. The Britzger EW campaign was built precisely "for a per-bin gammaZ/Z correction at Q²>100 for the HERA-II high-Q2 analyses built on these very samples" (same doc §4.5); the NoZ sidecar is its realization in our chain.
2. **H1 inclusive charged-hadron SIDIS (arXiv:1302.1321, the H1 charged-particle paper our analysis inherits):** the multiplicity observables are defined in the virtual gamma–p rest frame (HCM) and corrected with DJANGOH+HERACLES bin-by-bin factors; radiative treatment is folded into the correction (no separate C_rad published) — at Q² < 100 GeV² the Z content (≲0.35%, see Britzger §4.4) is negligible, so the H1 inclusive charged-hadron gamma* p-level results are **pure photon Born by construction** (`docs/research/research-h1-1302-1321-charged-multiplicity.md` §2, §8; frame convention `docs/references/reference-sidis-frame-gamma-star-p.md` §2, §6.1).
3. **HERMES identified-hadron SIDIS (arXiv:1212.5407 / PRD 87 (2013) 074029):** the correction maps reconstructed yields to **Born-level counts "from a second MC pass without radiative/detector effects"** (`docs/research/research-genreco-matching-critique.md` §1.3), inside the one-photon-exchange SIDIS formalism (Bacchetta et al., `research-LT-separation-feasibility.md` §1.1). At HERMES Q² (~few GeV²) Z exchange is ~10⁻³ and is never included in a gamma* p-level quantity. COMPASS (1709.07374) follows the same philosophy (same doc §1.4). **Identified-hadron SIDIS gamma* p results are therefore photon-only Born.**
4. **H1 high-Q2 practice at the ep level:** where HERA publishes at high Q², the quoted quantity is the **ep** NC cross section at Born level *including* gamma+Z (with running alpha and weak loops, the configuration of our full-EW NONRAD legs: `LPIN(2)=1, LPIN(7)=2, LPIN(8/9/11)=1`) — i.e., the current primary product is the H1-standard ep Born. The analysis note handles the duality exactly this way: the appendix provides two table sets — "The first convention retains the interference with the Z boson. The second gives the pure photon-exchange contribution. The two conventions must not be mixed within a single comparison" (`sections/appendix_cross_section_tables.tex` lines 26–28, 52–58, 94–100), and the radiative-corrections section declares the full-NC Born as the level of all results with the pure-photon Born as the diagnostic variant (lines 175–191).
5. **"Dressed/absorbed" is not a publication convention for gamma* p** in any cited reference. The only place dressing enters is the alpha(0)-vs-running-alpha sub-choice of §1.1; it is a convention declaration, not a systematic, and it moves the quoted sigma(gamma* p) by ~+8.6–9% integrated at Q²>100 (NOZ1/NOZ = 7.2859/6.7086 nb, `EMINUS_..._2026-10-07.md` §7b) — it must be decided, never budgeted.
6. **Naming flag ("HERA-B"):** no HERA-B ep SIDIS gamma* p result exists (HERA-B was fixed-target proton–nucleus hadron production, not ep SIDIS), and the workspace contains zero HERA-B references (grep of `sidis_highQ2/docs/` and the note sections). The identified-hadron SIDIS gamma* p precedents in the workspace's reference base are **HERMES** (1212.5407 / PRD 87 074029) and COMPASS (1709.07374); this memo assumes HERMES was intended. If a specific HERA-B document was meant, it is not in the workspace and the conclusion is unaffected (the gamma-only-Born requirement stands on the definition plus the H1/HERMES evidence above).

---

## 2. Q2 — Does the existing F_EWgamma compute exactly the right thing?

### 2.1 What is actually computed (audited at source)

From the releasebin finalizer `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/scripts/crad_v3_finalize_nozborn_releasebin.py` and its delivered table `cgammaborn_empz_gt45_nominal_490rows.csv` (sha256 `7ec901af...`, byte-identical to the consolidated-baseline copy `data/radiative/crad_release_baseline_2026-10-10/noz_cgamma_born_provisional_diagnostic/`, verified this session):

- `C_gamma_born(i) = sigma_gamma_born,e-(i) / sigma_rad_mixed(i)`; `F_EWgamma(i) = sigma_gamma_born(i) / sigma_nonrad_mixed(i)`; the decomposition `C_gamma_born = C_rad_mixed × F_EWgamma` is enforced per bin as hard gate G4 at rel 1e-9 (exposure-independent).
- **Numerator:** DJANGOH14 **NoZ photon-only pure-Born e−** sample (`dst2all_djangoh14_NoZ`, 260 chunks, Britzger "neu" executable sha256 `22e105df...`): Z couplings zeroed **at source** in `hssetp.F`/`hssetf.F`/`hsstr1.F` (VAFI(*,*,2)=0 — kills the γ²+2γZ+Z² sum's γZ **and** Z² terms exactly at tree level), `LPIN(2)=0` ACTIVE per GEN headers (no soft&virtual 1-loop), `LPIN(7)=2` recorded but inert at Born, fixed α(0)=1/137.0359895, `INT3/ISM3` off (non-radiative); validation log `SIGMA-GAMMA-Z/Z/W = 0`, SIGTOT 6.7086 nb; exposure `L_MC = 14,119,125.013920 nb^-1`, `EXACT_OOMCLUMI_SWEEP`, status PASS (260/260, four end-of-production trailer chunks documented and integrated).
- **Denominator:** the published mixed-beam DJANGOH14 **full-EW NONRAD** primitives (stdjan family: `LPIN(2)=1`, `LPIN(7)=2`, `LPIN(8/9/11)=1` — Born + soft&virtual with running alpha and weak self-energies), beam-mixed with the data luminosity fractions `f_ep=0.5140930272 / f_em=0.4859069728` by the byte-identical baseline `mix_by_y` construction (G3: reconstructed baseline matches the published C_rad table at 1e-9).
- **Cuts and weights (identical on both legs):** release-safe `(Q2esGen, x_es_gen)` cells; `GenEmpz>45`; recomputed `xF<0` (q-nominal) current hemisphere; hadron pool pi/K/p with `IsGtr==1`, `Mother2==-1`, V0-daughter veto; `sigma_proxy = sum(weight1*weight2)/L_MC` (MCWE weights live, mean 2.353399); no reco cuts, no alpha cone (gen-only, per the cone-promotion invariant). These are exactly the applied C_rad truth contract (`sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-07/build_born_candidate.py:1713-1716`), so F_EWgamma is release-bin-aligned by construction.
- **Charge handling:** the numerator is e− only; this is **exact** for the photon-Born target because photon-only Born is exactly charge-symmetric at tree level (|M(e−q)|² = |M(e+q)|²; verified to all printed digits, NoZ e± = 218.84216 pb both, Britzger §4.3/§5.1), so "the beam mixture enters only through the mixed RAD denominator and no residual per-beam factor is required" (`EMINUS_..._2026-10-07.md` §7a). In the mixed denominator the charge-antisymmetric γZ term is diluted by (f_em − f_ep) ≈ 0.028 to ≲0.01%.

### 2.2 What F_EWgamma removes — and what it does NOT

`F_EWgamma = sigma_γ,pureBorn,α(0) / sigma_fullEW,Born+SV+runningα,mixed` removes, relative to the current full-Born target:

| Content | Removed? | Integrated size (Q²>100, production-relevant) |
|---|---|---|
| Pure Z² exchange | **Yes** (couplings zeroed at source) | ≈ +0.36% of the denominator (Britzger §4.4: σ_Z² = +0.356%) |
| γ–Z interference | **Yes** (same zeroing); in the mixed denominator it is already diluted to ≲0.01% by the beam mixture | σ_γZ(e−) = +0.246% before mixing |
| Vacuum polarization / running α | **Yes** (numerator is Born, `LPIN(2)=0`) | ≈ +8.6% (photon-only corrected/Born = 7.2859/6.7086; +11.27% at Q²>1000) |
| Soft&virtual QED 1-loop | **Yes** (same switch) | included in the above |
| Weak self-energies | **Yes** (numerator `LPIN(8/9/11)=0`; inert once Z is gone, +0.026%) | ≈ +0.03% |

Arithmetic check: loop factor 6.7086/7.2859 = 0.92078; Z factor (e−, corrected level) 7.2859/7.32394 = 0.99481; product = 0.91599 = the full-phase-space anchor 6.7086/7.32394 quoted in the finalizer. **So F_EWgamma is NOT a "Z-removal" factor: ~90% of its size is loop/vacuum-polarization content, not Z.** The mc-expert verdicts already warn against the adjacent misreadings (κ is an empirical anchor; LPIN(8/9/11) do not remove tree-level Z; NOZ1-vs-stdjan is a MIXED comparison) — `cgammaborn_gen_unc_proxy_v3emfix.json` key `mc_expert_validation_2026_10_09`.

### 2.3 Is the ~8% plausible at Q² up to 20000 GeV², x down to 0.0021?

**Yes — size and trend are physically consistent.** Measured per-cell means of F_EWgamma (this session, from the 490-row table):

| Cell | Q² range [GeV²] | F_EWgamma mean | 1−F mean |
|---|---|---|---|
| 0/1 | 150–300 | 0.9196 / 0.9172 | 0.080 / 0.083 |
| 2/3 | 300–700 | 0.9100 / 0.9096 | 0.090 / 0.090 |
| 4/5 | 700–1500 | 0.8957 / 0.8916 | 0.104 / 0.108 |
| 6 | 1500–20000 | 0.8472 | 0.153 |

Overall: min 0.7846, median 0.9074, max 0.9906; integrated (yield-weighted) 0.91191 (h+) / 0.91158 (h−); data-weighted integrated C_gamma/C_rad = 0.911633 / 0.911309 (the ~8.9% of the current comparison package). The monotone fall with Q² matches (i) the running of α_em (integrated +8.61% at Q²>100, +11.27% at Q²>1000, growing per-bin with Q²) and (ii) the growing Z+γZ content at and above the Z pole (e− full/photon-only reaches +8.6% at Q²>1000 Born). The curve is smooth across bins — no statistical fluctuation pattern; per-bin numerator MC statistics: relative_stat_unc median 0.551%, max 7.84% (sparse high-Q²/high-pT bins; min count 237, median 50,537 weighted entries per bin). The deviation is confined to the expected sign and magnitude band (soft gate [0.75, 1.02] per-bin; zero violations recorded at finalize).

### 2.4 Verdict

**PASS — fit for purpose** as the projection factor from the current full-EW dressed Born level to the **pure one-photon-exchange Born at fixed α(0)**, i.e. exactly the gamma* p level recommended in §1. Conditions:

1. **Label discipline:** never describe F_EWgamma (or the −8.9%) as "Z-boson removal"; the correct statement is "removal of the Z exchange, the γ–Z interference, and the QED/electroweak loop content (vacuum polarization, soft&virtual, weak self-energies), mapping the full-NC Born level to the pure photon-exchange Born level at fixed α(0)". The Z-only part is ≈ −0.35% (mixed) integrated at Q²>100.
2. **Convention coupling:** if the dressed-photon convention were chosen instead (§1.1), this factor would over-correct by ~8.6–9% and a NOZ1-based table (expected integrated ≈ 1.136 per the finalizer's crosscheck band) would be the required product. That table does not exist today; the pure-Born target was the recorded user decision of 2026-10-08 (`EMINUS_...` §7b).
3. **Status caveats carried:** DJANGOH-only (no RAPGAP NoZ exists — coverage gap, not a defect of the factor); G8 empirical charge-symmetry null test impossible (no photon-only e+ sample, audit-verified) — carried by the exact tree-level symmetry argument plus Britzger's integrated e± identity verification; the product is PROVISIONAL DIAGNOSTIC, `c_rad_applied=false`.

---

## 3. Q3 — Is the current C_rad chain adequate as the base of a gamma* p release?

### 3.1 Mathematically yes, and the mixed-beam property becomes a feature

- The gamma* p factor is exactly the delivered composition: `C_gamma_born = C_rad × F_EWgamma` per published bin, hard-gated (G4, 1e-9) against the applied C_rad table; same bins, same truth selection, same hadron definition, same weights. Applying the sidecar to the same unfolded data (currently the cone-basis raw release) is the gamma* p product — no new C_rad machinery is needed.
- At the photon target the standing "mixed-beam-only" scope limitation **vanishes as a physics concern**: photon Born is exactly charge-symmetric, so the mixed-beam data are corrected to a single, beam-charge-blind target with one factor (`EMINUS_...` §7a). The per-beam F(i) γZ bookkeeping factor (integrated +0.44…+0.60%) is required only for a *full-EW* per-beam target — not for gamma* p. The mixed-beam validity assessment already concluded the mixed correction is the only well-defined Born level for the mixed data (`docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` (iii)1, (iv)); at the photon level this statement is stronger, not weaker.

### 3.2 But the chain is NOT release-grade today

Carried blockers that apply to a gamma* p quote (none is created by the gamma* p choice; all already gate the ep Born release):

1. `P-CRAD-LUMI`: exposure provenance FILE-SET-RECONCILED, generator-log closure permanently blocked externally; **C_rad is not publication-grade**; the releasebin baseline is PROVISIONAL (promotion gate, not publication clearance). This ceiling propagates identically into any gamma* p product built on C_rad.
2. The NoZ sidecar is **unapplied** (`c_rad_applied=false`), PROVISIONAL DIAGNOSTIC, **DJANGOH-only** (no RAPGAP NoZ sample exists, verified) — the generator cross-check that backs the applied C_rad does not exist for the EW factor.
3. G8 (empirical photon charge symmetry) is conditional/impossible with existing samples.
4. No installed gamma* p release product exists: the sidecar appears only as the diagnostic data side of the 5-prediction comparison package and as the note's noZ table mode (`noz_crad_gen_mode=carry`).

### 3.3 Minimum work to make it release-grade for a gamma* p quote

1. **Recorded promotion decision + workflow** for the NoZ releasebin sidecar: from PROVISIONAL DIAGNOSTIC/unapplied to the applied factor of a gamma* p derived release, following the recorded precedent pattern (completed review of the new baseline, gate evidence, checksummed install, recorded user decision — as executed for the 2026-10-07 c_rad application and the 2026-10-13 cone Born build).
2. **Build and install the gamma* p Born release on the current cone basis**: `sigma_gammaBorn(bin) = C_gamma_born(bin) × Unfold_cone(data; R)`. The C_rad and C_gamma_born tables are cone-invariant (gen side unchanged, `crad_table_inheritance` sha-asserted precedent), so the per-bin gamma* p shift equals the per-bin raw cone shift; the same build/validation recipe as `born_crad_dj14relbin_detints2prime_alphaveto_cone_..._2026-10-13/` applies (scale the absolute-nb columns, keep yields and relative columns bitwise, CHOICE-A yield policy, per-bin shift-equality gate).
3. **Generator-systematic policy decision** for the EW factor: either adopt the κ-proxy (`cgammaborn_gen_unc_proxy_v3emfix.json`: `u_model(bin;κ)=sqrt(v3_emfix_rel² + [κ(1−F_EWgamma)]²)`, central κ=0.02, conservative κ=0.03, mc-expert-validated as an EMPIRICAL anchor) or keep the note tables' carry-over of the WithZ spread (`noz_crad_gen_mode=carry`) — and execute the recorded **SWAP rule**: the proxy/leg replaces `c_rad_gen_rel` (never additive) and the statistical term swaps simultaneously to the NoZ legs.
4. **Statistical leg swap:** the NoZ numerator `relative_stat_unc` (median 0.551%, max 7.84%) replaces `c_rad_stat_rel` (medians 0.427%/0.443%).
5. **Convention statement in the note:** EW level (pure photon Born at α(0)), factor basis, the G8-substitute argument (exact tree-level charge symmetry + the Britzger e± integrated identity), the flux α(0)-consistency statement (jointly with the flux owner), and the standing never-mix rule.
6. **P-CRAD-LUMI cannot be closed internally** (external MCDB block); it ships as a documented caveat exactly as the ep Born release already carries it (`sections/radiative_corrections.tex` §caveats, items on provisional status and open generator-log cross-checks).
7. **Not required:** per-beam C_rad tables (charge-blind target); a photon-only e+ production (G8 stays conditional with the recorded argument); new RAD/NONRAD C_rad campaigns; any change to the applied C_rad tables.

---

## 4. Q4 — Additional systematic legs required by the gamma* p level choice

| Leg | Necessity | Expected magnitude (from existing tables) |
|---|---|---|
| Generator dependence of F_EWgamma (missing RAPGAP NoZ leg) | REQUIRED (currently unmeasured; coverage gap) | Anchored, not measured: κ-proxy k002 medians h+ 0.622% / h− 0.619% (max 5.77%/5.67%) as the swap for `c_rad_gen_rel` (WithZ emfix medians 0.584%/0.580%, max 5.76%/5.66%); κ=0.03 variant medians 0.658%/0.653%. The κ-term alone (2–3% of (1−F)): median 0.185% (κ=0.02) / 0.278% (κ=0.03), max 0.431% / 0.646%; per-cell mean 0.16% (Q² 150–300) rising to 0.31% / 0.46% (cell 6). Net budget impact vs the current leg-envelope totals (medians 5.80%/6.19%): ≲ +0.05 pp in medians; the max bins are the same sparse-bin structure already carried. |
| F_EWgamma numerator MC statistics | REQUIRED (swap) | Median 0.551%, max 7.84% (replaces `c_rad_stat_rel` 0.427%/0.443% medians). |
| PDF dependence of the Z-interference term | Optional dedicated leg; bounded | Numerator and denominator are both CTEQ6L → common-mode cancellation in the ratio; the residual acts only on the flavour-mix of the Z term. Z content ≈ +0.35% mixed integrated at Q²>100 (a few % in the top cells); a conservative 20% flavour-mix variation gives ≲0.07% integrated, ≲0.5% in cell 6 — inside the κ anchor. Recommend documenting as covered by κ (or a dedicated ≤0.1% component); a measured leg needs new production. |
| Per-hadron-charge (h±) vs combined | NONE (already per-charge) | Tables are 2×245 per charge; h+ vs h− F integrated difference ≤0.04% (0.91191 vs 0.91158). |
| Per-BEAM-charge legs | NONE at the photon target | Exact tree-level charge symmetry; mixed γZ residue = (f_em−f_ep)·σ_γZ ≈ 0.028 × 0.25% ≈ 0.007% at Q²>100; beam-fraction uncertainty Δf ≲ 0.033 changes it by ≲0.01% (`CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §ii.5). |
| Loop-convention (α(0) vs dressed) | NOT a systematic | A ~+8.6–9% convention statement (NOZ1/NOZ = 7.2859/6.7086 at Q²>100); must be declared, not budgeted (§1.1, §2.4-2). |

---

## 5. Q5 — Must the gamma* p release use the photon-Born sidecar, and what happens to the full-Born release?

1. **Yes — by definition.** A gamma* p cross section is the photon-only level. The only existing chain to that level is the photon-Born sidecar (`C_gamma_born = C_rad × F_EWgamma`, the composed per-bin factor); applying C_rad alone leaves the result at the full-NC ep Born level, and dividing *that* by a photon flux yields a mislabelled "(γ*+Z)p" cross section. There is no route from the full-Born release to gamma* p except applying F_EWgamma — which is exactly the sidecar.
2. **Yes — keep the full-Born (withZ) release as the primary product**, with sigma(gamma* p) as a derived, separately labelled alternative:
   - the withZ chain is the only one carrying the two-generator cross-check (RAPGAP31 releasebin emfix formal alternative → `c_rad_gen_rel`); the photon chain is DJANGOH-only;
   - the withZ level is the H1-standard ep Born convention, preserving continuity with HERA NC measurements and with the entire installed release/budget/plot machinery (the 15-component cone budget and all note tables are on this basis);
   - the analysis note already implements precisely this two-convention structure (withZ primary tables, noZ alternative tables, "must not be mixed"; `sections/appendix_cross_section_tables.tex` lines 26–28, 94–100; `sections/radiative_corrections.tex` lines 175–191);
   - every gamma* p quote must state: EW level (pure photon Born at fixed α(0)), factor basis (DJANGOH14 NoZ releasebin table, applied status at release time), flux convention, and observable basis (cone-basis S2'), per the standing quoting rule.
3. **Consistency note for the comparison packages:** the 5-prediction package's photon-Born data side is already this construction (integrated h+ `11.910263` / h− `11.606904` nb; C_gamma/C_rad `0.911633`/`0.911309`), with the known ~9% target-level inconsistency against the full-Born/NORAD MC curves documented "by design" — a gamma* p release resolves that inconsistency on the side of the data by declaring the level, and any MC curve compared to it must be photon-only Born (as MAPTMD24 is).

---

## 6. Generic-band note (standing deviation, flagged per operating rules)

The generic expectation `C_rad ~ 0.7–0.8, δ_QED = 1−C_rad ~ +0.2–0.3` does NOT hold for this analysis: integrated C_rad ≈ 1.14 (δ_QED ≈ −0.14) because the hard fiducial truth cuts (GenEmpz>45, 0.1<y<0.7, xF<0) remove radiatively-degraded events, so σ_rad < σ_nonrad in-bin. This is documented and gated (NoZ G6 integrated band [0.95,1.12]; per-bin [0.4,1.8]; published per-bin span [0.840,1.572]); the workspace's own gates govern, per the precedent recorded in `CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` §ii.6. The EW analogue, δ_EW = 1−F_EWgamma, is positive (+0.089 integrated), smooth, and monotonically growing with Q² (+0.080 → +0.153 per cell) — the expected sign and shape for a radiative-level projection factor.

---

## 7. Evidence index (absolute paths)

- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/STATE.md` — current validated state, cone triad, blockers (P-CRAD-LUMI, PROVISIONAL baseline, mixed-beam scope, NoZ unapplied status), 5-prediction package data-side numbers.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_crad_releasebin_2026-10-09/crad_v3_xfgen_lt0_empz_gt45_nominal_490rows.csv` — applied primary C_rad (integrated h+ 1.1415172920054786 / h− 1.1410367377289135).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/radiative/born_candidate_release_2026-10-07/build_born_candidate.py:1713-1716` — applied C_rad definition (sigma_proxy = sum(w1·w2)/L_MC; xFGen<0; EMPZ>45).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_empz_gt45_nominal_490rows.csv` — C_gamma_born / F_EWgamma per bin (integrated h+ 1.040960540463156 / h− 1.0401468080806677; F_EWgamma min/median/max 0.7846/0.9074/0.9906; per-cell table in §2.3). Byte-identical (sha256 verified this session) to `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/crad_release_baseline_2026-10-10/noz_cgamma_born_provisional_diagnostic/cgammaborn_empz_gt45_nominal_490rows.csv`.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/scripts/crad_v3_finalize_nozborn_releasebin.py` — audited definition, gates G1–G8, exposures, loop references (SIGMA_NONRAD_EW_NB=7.32394; anchor 6.7086/7.32394 = 0.91598).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/cgammaborn_gen_unc_proxy_v3emfix.json` — κ-proxy construction, k002/k003 values, double-counting SWAP rule, mc-expert validation verdicts.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/djangoh14_nozborn_crad_releasebin_2026-10-09/README_NOZBORN_RELEASEBIN.md` — releasebin NoZ product statement (definition, numerator/denominator, DJANGOH-only, G8 conditional).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/BRITZGER_EW_gZ_NOZ_METHOD_REVIEW_2026-10-07.md` — NoZ mechanism (coupling zeroing; γZ and Z² both killed), cross-section tables §4.3–4.4 (Z/γZ decomposition; +0.60%/+0.11% at Q²>100; +8.61%/−0.50% at Q²>1000), campaign purpose §4.5, e± photon-only identity §5.1.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` — §2 per-beam quantification and F(i); §7/§7a photon-Born charge symmetry and identity gate; §7b loop verdict (NOZ = pure Born; NOZ1 = corrected; full-EW NONRAD legs LPIN(2)=1/7=2/8,9,11=1); §7g campaign execution and exact exposure.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/radiative/CRAD_MIXEDBEAM_VALIDITY_ASSESSMENT_2026-10-14.md` — mixed-beam verdict, Z-content numbers, generic-band note §ii.6, scope limitation wording.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/sections/radiative_corrections.tex` (lines 1–95 definition; 172–191 photon-only variant), `sections/appendix_cross_section_tables.tex` (lines 20–28, 52–58, 94–100 two-convention tables), `sections/systematics.tex` (lines 15–24), `sections/introduction.tex` (lines 56–58) — note-level convention statements.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/H1-ChargedSIDIS-AnalysisNote-HighQ/tables/highQ2_z005_s2prime_born_public_2026-10-11b/blocks/block_noZ_pos_S0_x0.tex` — generated noZ-table header: C_gamma_Born applied factor, C_rad reference-only, F_EWgamma consistency gate, `noz_crad_gen_mode=carry`, status caveats.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/references/reference-sidis-frame-gamma-star-p.md` — gamma* p (HCM) frame convention; HERMES/COMPASS/JLab/H1-1302.1321/DESY-20-176 precedent (§2, §6).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/research-LT-separation-feasibility.md` §1.1 — one-photon-exchange SIDIS cross section (Bacchetta et al.) and Hand-convention ε.
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/research-h1-1302-1321-charged-multiplicity.md` §2, §8 — H1 inclusive charged-hadron radiative treatment (HERACLES folded; eΣ method).
- `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/docs/research/research-genreco-matching-critique.md` §1.3–1.4 — HERMES 1212.5407 Born-level correction definition; COMPASS parallel.

**Provenance:** read-only session; no release artifact, table, budget, plot, STATE.md row, or note file was modified. Only this new memo was written.
