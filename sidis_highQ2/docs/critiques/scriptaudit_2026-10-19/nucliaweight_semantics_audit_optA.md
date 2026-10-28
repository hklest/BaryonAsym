# Nuclia-Weight Semantics Audit — OPT-A Campaign cone_s2prime_optA_2026-10-19 (read-only, 2026-10-20)

Auditor: h1-mc-expert, read-only. No analysis script modified.

## 1. Weight-branch inventory and provenance (VERIFIED)

**ChargedTot (CT, reco tracks)** — ChargedSidisTreeBuilderHighQ2_v6.C:
- weight1, weight2 (L745-746): event-level, copied from HAT pointers hatWeight1/hatWeight2 (L439-440); filled at L1039/L1040 (weight2 = isMC ? *hatWeight2 : 1.0f); gen-only MC events keep both from HAT (L1048-1049).
- NucliaWeight (L707): SEPARATE PER-TRACK branch, computed by the builder via H1NuclIACor::GetWeight (L1639-1650 pion ipart=1 default; L1830-1840 refined to kaon ipart=2 after matching; proton -> 1.0; -1.0 on non-finite). MC only; NOT copied from HAT; NOT folded into Weight1/Weight2.
- TrigWeightAC, TrigWeightRW (L709-710) — trigger weights, outside the w1*w2/NW contract.

**ChargedTotGen (CTG, gen hadrons)** — branches L813-893: weight1, weight2 (L852-853) only. NO NucliaWeight branch on CTG (verified empirically on rapgap31 7071_em06, 7094_ep0607 and djangoh14 7091_ep0607).

**H1OO semantics (source-verified)**: H1Hat/H1HatEvent.h L156-157 — fWeight1 = "event weight on L4 or MC GKI weight", fWeight2 = "event weight on L5 or MC user weight". H1CalcTrig.C L618-656: GetMcWeight() = Weight1 (sanity-clamped [0.1,1000] -> else 1), GetMcUserWeight() = Weight2. H1PhysUtils/H1NuclIACor.h: "corrections for nuclear i.a. undescribed by MC", per-track weights for MC pion/kaon tracks (A. Meyer, fortran K. Daum).

**Empirical values (single-file schema checks)**:
| Sample | CT weight1 | CT weight2 | CT NucliaWeight | CTG |
|---|---|---|---|---|
| rapgap31 7071_em06 (RAD) | per-event, 1-118.6 | == 1.0 (n_uniq=1) | per-track, min -2.583, max 1.0, mean 0.9845 | w2 == 1.0, no NW |
| rapgap31 7094_ep0607 (RAD) | per-event, 1-77.2 | == 1.0 | min -0.699, max 1.0, mean 0.9846 | no NW |
| djangoh14 7091_ep0607 (RAD) | per-event, 1-52.9 | == 1.0 | min -0.234, max 1.0, mean 0.9846 | no NW |

**C_rad input trees** (RC_treebuilder_genonly_v3.C, tree ChargedGenRC): branches weight1, weight2, weight with weight = weight1*weight2 (L57/61/67/76; bitwise-equal verified). No NucliaWeight branch (gen-only, structurally impossible). Empirical on DJ14 RAD / NONRAD-old / NONRAD-new (em legs): weight2 == 1.0 on all legs, weight1 per-event (1-149.6).

## 2. Nuclia semantics verdict

The premise that "Nuclia weight" is a per-event QED radiative reweight (NUCLIA/SIRIUS photon spectrum) is FALSE for this chain. NucliaWeight is exclusively the PER-RECO-TRACK nuclear-interaction (detector-material) correction from H1NuclIACor::GetWeight, an MC-only down-weight (mean ~0.985, can be negative, min -2.58) bringing over-efficient MC track efficiency to data. There is NO QED photon-spectrum reweight stored anywhere in these trees; radiative corrections enter this analysis exclusively via the RAD/NONRAD sample ratio C_rad = sigma_nonrad/sigma_rad. On RAD events: Weight1 = MC GKI generator event weight, Weight2 = MC user weight (== 1.0 on all audited samples), NW = per-track material correction. "NONRAD carries 1.0" is moot — NW is per-track and generator/mode-agnostic, not a per-event QED weight. Both RAPGAP31 and DJANGOH14 carry NW (booked on all MC by the builder; verified on both generators). No beam-charge dependence: identical branch structure and semantics on ep0607 and em06 shards; NW depends only on (species, charge, pt, phi, theta, dca, z0). NW is NOT inside Weight2 (Weight2 == 1.0; NW computed downstream per track), so a body-side NW multiplication is a single application, not double-counting.

## 3. Per-site audit

**(a) C_rad worker — PASS.** crad_v3_worker_releasebin.py L118: w = weight1*weight2, same formula on BOTH legs (RAD and NONRAD; --sample only toggles the RAD-only Q2Gki>100 numerator cut, L115/119). L121 books the release-safe cell (Q2esGen, x_es_gen) — the booking OPT-A harmonizes to. RC v3 input trees carry no NW branch, so no NW can or should enter. Finalizer forms sigma_proxy = sum(weight1*weight2)/L_MC per leg; L_data never enters; matches the frozen contract in STATE.md and the releasebin validation JSON (c_rad_definition: sigma_nonrad/sigma_rad, Q2_binning: Q2esGen).

**(b) OPT-A response builder — PASS.** Body AND fake reco legs: w_reco = w1*w2*NucliaWeight (L1373-1381; NW non-finite -> 1.0, clipped to [0, NUCLIA_CLIP=10.0] L597; body fill L1950-1956, fake fill L2010-2013); pool/miss/prior gen legs: w_gen = w1*w2 (L1144-1146, miss semantics L104). This is the canonical shipped convention per the 2026-09-09 supersession banner in sidis_highQ2/docs/results/nucliaweight_fix_2026-05-31.md: NW belongs in the response numerator only; the gen prior stays the unweighted particle-level yield (the NWeff prior would bias cross sections -0.45%; the release is "CORRECT AS SHIPPED" with the plain h_prior_gen). The builder retains prior_gen_NWeff explicitly marked "SUPERSEDED 2026-09-09 — PHYSICALLY WRONG. DO NOT USE" (L797-806), consumed only as the weight-level truth-conservation comparator (L123-130: body(w1*w2*NW)+miss(w1*w2) == prior_gen_NWeff). No double-count: NW not in Weight2.

**(c) Stage-2 re-unfold driver and data-side scaling — PASS.** run_bgsub_setrprime_reunfold_cone_optA.py and run_bbb_ibu_release_cone_s2prime_optA.py (LUMI=351000.0 L271): response matrix A_ij = B_ij/P_j and eps_j = col_inner_j/prior_j (crosscheck_unfold_methods_v4.py L623-697) — a weighted-efficiency ratio in which the raw MC weight scale is internal and consistent; IBU k=4 is data-driven. NO L_data/L_MC factor multiplies any MC weight in the response/unfold chain; absolute normalization is post-unfold division by L_data = 351000.0 nb^-1 only (cross-section rows yield/L_DATA_NB, L860-894). The MC luminosity scale enters only in (i) background-template vectors and (ii) the C_rad sigma_proxy legs — separate sites with their own documented exactly-once policies. The merge carries the plain h_prior_gen as the consumed prior and *_NWeff audit-only. apply_crad_and_diff_vs_cleanrelease_optA.py: sigma_Born = sigma_raw * C_rad(bin), no weights.

**(d) Per-cell straddle counter — PASS (the "w1*w2*NW" spec is correct only where applied, and it is applied correctly).** opta_straddle_percell_counter.py: the POOL-side straddle matrix uses w_gen = w1*w2 (L256-258; conventions L644-648), docstring explicitly adjudicating (L24-28) that CTG has no NucliaWeight so w1*w2*NW == w1*w2 on the pool side; the NW factor enters only the body-side matrix via w_reco. The BODY-side changed matrix uses w_reco = w1*w2*NucliaWeight (L46, L433, L652), captured verbatim from the builder's own compute_reco_bin via CapturingAcc (no reimplementation). Hard asserts tie each side to its response booking: pool matrix row-sums == mc_count_per_gen and weight row-sums == prior_gen (L265-270); changed-rows total == the builder's n_opta_body_cell_changed_vs_radsafe (L410-414). NW is a real stored branch (CT only); "w1*w2*NW" is the CORRECT combination for body/reco-side per-cell rates and would be a spec error on the pool side — the counter already implements exactly this split. Pool-side weights consistent with the C_rad gen-only booking (both w1*w2); body-side weights consistent with the response body booking. The aggregator documents the identical conventions and only sums.

**(e) Frozen 2026-10-11 builder — PASS (bit-identical weight handling).** cone_s2prime_2026-10-11 builder: NUCLIA_CLIP = 10.0 (L546), w_gen = w1*w2 (L1047), NW clip (L1274), w_reco = w1*w2*wn (L1277). The full OPT-A-vs-frozen diff (381 lines) contains ZERO weight-code changes; the only physics change is the body gen-Q2 cell operand (Q2_es_gen -> per-event Q2esGen from CTG), exactly as the RUN_SPEC §1 freezes. OPT-A did NOT change weight handling.

## 4. Verdict

**The Nuclia weight is treated properly everywhere in this chain.** Applied exactly once — on the reco-side (matched-body and fake) legs of the response and the counter's body-side matrix (w1*w2*NW, clipped [0,10]) — and correctly absent from every gen-side booking (pool, miss, prior, C_rad legs, pool-side straddle matrix), all of which use w1*w2. This is the supersession-correct convention (NW in the efficiency numerator only). No site has a wrong, missing, or double application. NO FIXES REQUIRED. Nuance for the campaign record: the ~0.45% body-vs-pool weight deficit is the expected NW effect and must be compared against prior_gen_NWeff (the counter and builder already do this); the negative-NW clip at 0 is the frozen builder behavior inherited verbatim.

## Missing Evidence

1. HAT-production-side fill of MC Weight1/Weight2 (DST GKI/USER bank -> HAT) not located inside the h1oo tree (H1Red has no Weight1/Weight2 references; HAT production is upstream H1 software). Semantics settled by the h1oo API (H1HatEvent.h, H1CalcTrig) plus empirical values (Weight2 == 1.0 on every audited RAD/NONRAD, ep/em leg). Framework-level provenance of the GKI weight would route to h1-software/h1-mc-db. Everything analysis-relevant VERIFIED; only this upstream production detail PROVISIONAL.
2. Path discrepancy in prior tasking: H1ChargedSidis_HighQ2/scripts/canonical_highQ2_sidis_2026-06-30/radiative_corrections/ contains only v1-era files; the audited releasebin worker copy is at studies/radiative_correction_genonly_v1/output_v3/nozborn_hist_releasebin_20261009/scripts/crad_v3_worker_releasebin.py (L118/L121 match the citations and the RUN_SPEC "worker L121" reference exactly; base v3 worker scripts/histogramming_v3/crad_v3_worker.py L86 identical formula). The main DJ14/RG31 releasebin campaign's own worker copy not individually checksum-located (campaign JSONs record no script paths) — the weight formula is pinned redundantly by the RC v3 tree schema (weight == w1*w2 bitwise), both workers, and the finalizer. Exact main-campaign copy pinning would be an h1-mc-db provenance lookup.
3. RAPGAP31 7096/7097_ep0304 shards not opened individually; branch schema builder-determined and identical across shards by construction; both beam charges covered empirically (7071_em06, 7094_ep0607). DJANGOH14 7110 DjBkgd not mixed into this chain (no audit needed).

--- audit metadata ---
- Report persisted by the delegating supervisor session (auditor toolset read-only), 2026-10-20.
- User directive answered: "Make sure we treat the nucliaweight properly" — verdict: treated properly everywhere; no fixes required.
