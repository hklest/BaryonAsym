# fitnorm branch pricing — R1 decision support (read-only)

- **Date**: 2026-10-19. **Status**: READ-ONLY PRICING NOTE. No promotion, no central change, no
  re-unfold. Numbers computed from released vectors + fit CSVs only (no tree processing).
- **Basis/level discipline**: all components and totals below are RAW-reconstructed-level
  *relative* uncertainties on the official_reunfold IBU k=4 result on `data_sub′`
  (cone+S2′+Set-R′ clean release, promoted 2026-10-17; L_data = 351000.0 nb⁻¹ only).
  Branch-A shifted integrals are quoted at RAW IBU level (h+ 10.508811 / h− 10.217503 nb) and
  Born IBU level (h+ 11.996558 / h− 11.663956 nb), cone+S2′+Set-R′ basis. Nothing here modifies
  the Born triad.
- **Inputs** (all read-only): `output_v6/studies/bgsub_fitnorm_2026-10-19/fits/fitnorm_results.csv`,
  `.../fits/variations/fitnorm_{V1,V4a,V4b,X1_rg,X1_dj}.csv`, `.../variations/fitnorm_variations.csv`,
  `.../budget/candidate_fitnorm_20comp/` (B1 as installed-candidate), released vectors
  `output_v6/studies/bgsub_setr_cone_build_2026-10-11/cone_15comp/stage2_setrprime_2026-10-15/vectors/bgsub_setrprime_vectors_{490,980}.csv`,
  installed 19-comp budget `output_v6/central/data_release/systematics_bgsub_setRprime_19comp_cleandenom_..._2026-10-17/bgsub_setrprime_budget19_cleandenom.csv`.
  Absolute prefix `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2`.

## 0. Fit outcome (released CSV, verified)

Pre-registered gates: **R1 fired in 5 groups** (z = |μ_comb−1|/σ_comb > 2): h+S0 6.57, h−S0 20.93,
h−S1 11.77, h−S2 5.25, h−S3 3.62. **R6 also fired in 8/8 groups** (p < 0.001; χ²/ndf 2.8–30.3,
worst h−S0 30.3) — a pre-registered STOP upstream of the A/B choice. μ_V0 sits exactly at the
≥0 bound in 4/4 h+ groups (0.0000, 0.0000, 0.0000, 0.0058) vs h− 0.370–0.851, against a
charge-symmetric MC V prediction (V_g pooled 134,074 h+ vs 129,832 h−, 3.2% apart) — physically
implausible as a normalization statement; the Hessian σ_V0 at the bound is not a proper marginal.

| group | μ_e | μ_V0 | μ_comb | σ_comb | corr | z |
|---|---|---|---|---|---|---|
| h+ S0..S3 | 1.944 / 1.831 / 1.670 / 1.539 | 0.000 / 0.000 / 0.000 / 0.006 | 0.9558 / 0.9900 / 1.0023 / 1.0194 | 0.0067 / 0.0091 / 0.0155 / 0.0237 | −0.97..−0.99 | 6.57 / 1.10 / 0.15 / 0.82 |
| h− S0..S3 | 1.369 / 1.221 / 1.120 / 0.962 | 0.370 / 0.515 / 0.623 / 0.851 | 0.8744 / 0.9063 / 0.9272 / 0.9257 | 0.0060 / 0.0080 / 0.0139 / 0.0205 | −0.96..−0.98 | 20.93 / 11.77 / 5.25 / 3.62 |

## 1. Branch B price table (490-bin release grid, installed 19-comp cleandenom basis)

Component = per-bin RAW-level relative term; total20 = quadrature vs the installed chain
(`sqrt(total20² − bgsub_fitnorm² + variant²)`); "coh" = yield-weighted mean Σ term·data_sub′/Σ data_sub′
(the integrated one-sided shift if the term acted fully correlated). R3 empty-D bins
(pos 109/179, neg 240) get 0. Maxima sit at bin 34 (S0_x0, z6, pT4; D = 75/62 tracks).

| variant | median pos / neg | max pos / neg | total20 median pos / neg | bins >100% | coherent pos / neg |
|---|---|---|---|---|---|
| **B1** (R2 default, μ_comb envelope; = candidate) | 0.0634% / 0.3296% | 1.270% / 3.764% | 5.9768% / 6.2991% | 0 / 0 | 0.247% / 0.871% |
| **B2** (per-class envelope: (|μ_e−1|⊕σ_e)·e + (|μ_V0−1|⊕σ_V0)·v0)/ds′ | 2.9524% / 1.2292% | 26.97% / 11.52% | 6.9979% / 6.4669% | 0 / 0 | 7.192% / 3.348% |
| **B3** (combined shift |μ_comb−1| both classes ⊕ per-class marginal σ_e, σ_V0) | 0.3387% / 0.5190% | 4.609% / 3.979% | 5.9772% / 6.3407% | 0 / 0 | 0.707% / 1.060% |
| (B3-alt: per-class shifts ⊕ σ_comb) | 2.8950% / 1.0736% | 26.93% / 11.45% | 6.9977% / 6.4657% | 0 / 0 | 7.142% / 3.241% |
| (option b: flat-30% on B′ replacing the 4 released bgsub legs) | 0.9481% / 1.0690% | 14.03% / 8.98% | 6.0467% / 6.3570% | 0 / 0 | 2.317% / 2.393% |

- B1 verified: reproduces the candidate `bgsub_fitnorm_rel` column to 1e-16; installed totals
  5.9768%/6.2991% vs 19-comp 5.9764%/6.2968% (+0.0004/+0.0023 pp). R5: no flag (medians < 5%).
  Coherent decomposition: |μ_comb−1| leg 0.223%/0.868%, σ leg 0.069%/0.063% — i.e. B1's content is
  a **one-sided coherent normalization statement** of ≈0.22% (h+) / 0.87% (h−) on the e+V0
  subtraction, not a random per-bin noise.
- B2 is the literal "classes treated separately" reading of R2; with μ_V0(h+)=0 its V0 term is
  hypot(1, σ_V0) ≈ 1.00–1.12 → the ~100%-of-class term: pos median 2.95% vs neg 1.23%.
- R5 pathological territory: none of B1/B2/B3 exceeds the 5% median flag; the spec's own
  audit-only species-resolved variant (÷(e+v0), quadrature) is 67.9%/27.8% median — it is on a
  different denominator (relative to the subtracted class, not data_sub′) and is not
  budget-commensurate.
- Option (b) component medians 0.948%/1.069% reproduce the decision-brief figures
  0.948%/1.067% (≤0.002 pp); totals vs installed 19-comp: +0.070 pp (pos) / +0.060 pp (neg).

**B3 defensibility ruling.** Defensible: B3 as priced (combined central shift marginalized
through μ_comb, per-class *marginal* σ from the full 2×2 covariance) — but only as a diagnostic
decomposition showing the per-class marginal errors are cheap (totals +0.0004/+0.0416 pp).
Not defensible: (i) B3-alt (per-class central shifts paired with σ(μ_comb)) — σ_comb is the
E:V-weighted combined uncertainty; pairing it with per-class shifts keeps the unstable split
shifts while discarding the split uncertainty, and it numerically collapses to B2
(2.90%/1.07% vs 2.95%/1.23%) because |μ_V0−1|≈1 dominates σ_comb≈0.006–0.024; (ii) installing
B2 or B3 as the default at all — R2 pre-registered exactly one default (B1) and explicitly
downgrades species-resolved per-class readings to audit-only; and the σ_V0 values in h+ S0–S2
are boundary Hessian values (μ_V0 pinned at 0), not proper marginals.

## 2. Branch A price (980 fine grid, fitted μ_e/μ_V0 per group, lowQ2 fixed)

Re-derived subtracted vector B_R′(A) = μ_e·e_avg + μ_V0·v0_avg + lowq2; data_sub′ recomputed.
Validation of the released grid reproduction: per-group Σ n_raw_cone == N_data exact (8/8);
Σ e_avg == E_pred/2 and Σ v0_avg == (V_pred+overlap)/2 to ≤4e-16; pooled Σ B_R′ = 281,250.0916
= 7.2767% of D_cone (3,865,073), B′/data_sub′ = 7.8478% — all matching STATE/spec anchors.

| quantity (RAW level, cone+S2′+Set-R′) | h+ | h− | pooled |
|---|---|---|---|
| B′/D_cone released → branch A | 7.1686% → 6.9735% | 7.3886% → 6.5834% | 7.2767% → 6.7818% |
| B′/data_sub′ released → branch A | 7.7222% → 7.4962% | 7.9781% → 7.0474% | 7.8478% → 7.2752% |
| coherent rel. shift on Σ data_sub′ | +0.2102% | +0.8694% | +0.5337% |
| RAW IBU integral (nb) | 10.508811 → 10.530897 (+0.022086) | 10.217503 → 10.306333 (+0.088830) | — |
| Born IBU integral (nb) | 11.996558 → 12.021771 (+0.025213) | 11.663956 → 11.765362 (+0.101406) | — |

(IBU k=4 is linear in the data vector with fixed response/prior, so the integral scales
coherently with Σ data_sub′; the official re-unfold under branch A is separately authorized
work and would confirm to first order.)

- **Per-bin subtraction fraction f** (980 grid): released median 3.262%/3.624% (max 52.4%/89.8%)
  → branch-A median 2.859%/3.140%, **max 100.93%/108.11%**, f>0.5 in 3 (h+) / 2 (h−) fine bins
  (vs 0 today; on the 490 release grid: 1 pos bin, f_new max 89.36%, neg max 43.53%).
- **Unphysical stop-check**: bins with re-derived B > D_cone **and D_cone > 0: 0** on both
  charges, both grids. Strict count including zero-data fine bins: 6 (2 h+ / 4 h−), all
  `empty_bin_guard` bins with D_cone = 0 and B_A ≤ 0.19 MC-predicted tracks (no data there).
  The 3 released empty-D 490 bins stay guarded (D_cone = 0.0).
- **V0-equivalents**: released Σ v0_avg = 67,092.2 (h+) / 64,973.1 (h−) / **132,065.3 pooled**
  (the "132k"). Branch A subtracts μ_V0·v0_avg = **11.4** (h+) / 28,992.3 (h−) — i.e. accepting
  μ_V0(h+)=0 **removes 67,081 of 67,092 h+ V0-equivalents (99.98%)**, traded into the e class
  (e-equivalents 73,701 → 136,947 h+; 75,275 → 95,963 h−).

**Degeneracy diagnostics (released variation CSVs).** V1 (2D |DcaPrime|×RadLength shape choice)
swings μ_V0(h+) 0 → 0.447/0.496/0.422/0.704 and μ_e 1.94 → 1.14–1.44, while μ_comb moves only
0.011–0.028 — but that is **0.9–3.2× σ_comb in 6/8 groups**. V4a/V4b (cone radius 0.15/0.25):
μ_comb stable to ≤2.0e-3 (cone boundary is not the driver). X1_rg/X1_dj (single-generator
templates, cross-check only): μ_comb 1.24–2.26 with χ²/ndf up to 100 — the class shapes
themselves are generator-inconsistent, and the admitted split family spans μ_V0 ≈ 0 → 1.8
(X1_dj h+S0: μ_e 1.32, μ_V0 1.76), i.e. the released central (1,1) is *interior* to the family
while the nominal fit's (1.94, 0) is an edge. Branch A's own central is model-dependent: under
V1-based μ the coherent shift is +0.374% (h+) / +0.963% (h−) vs +0.210%/+0.869% nominal
(±0.16/±0.09 pp), with per-bin |A_nom − A_V1|/data_sub′ median 0.250%/0.120%, max 13.10%/12.57%
(36/15 fine bins > 1%).

**Coverage consequence — yes, branch A (and equally branch B) needs an enlarged envelope for the
0-vs-1 split degeneracy; B1 does not provide it.** Decomposing the fitted per-class shift into
the μ_comb direction (covered by B1) and the orthogonal split redistribution SPLIT_i =
|(μ_e−μ_comb)e_i + (μ_V0−μ_comb)v0_i|/data_sub′: **SPLIT medians 0.547% (pos) / 0.197% (neg),
max 23.75%/13.24%, integrated ≈ 0 per group (signed sums −33…−0.09 tracks)** — a pure
shape/anticorrelated term 8.6× (pos) / 1.7× (neg) the B1 median, uncovered by it. The released
`bgsub_model_rel` leg (RG↔DJ half-spread, medians 0.202%/0.228%) covers SPLIT in only 55/245
(pos) / 143/245 (neg) bins. The full μ_V0 0-vs-1 term (v0-share of data_sub′) is median
1.61%/1.71%, max 7.40%/8.82% — larger than flat-30% in 229/245 and 232/245 bins. Adding SPLIT in
quadrature to B1 gives totals 6.0589%/6.3080%. So: branch A priced honestly = central
re-derivation **plus** a split envelope at least at the demonstrated V1-swing scale (median
0.25%/0.12%, max ~13%) and defensibly at the SPLIT scale — not the naive σ(μ_comb)·f term.

## 3. Recommendation (from the numbers)

1. **Treat the fit as model-failed; defer the A/B central decision.** R6 fired 8/8 (p ≤ 4.2e-7,
   χ²/ndf 2.8–30.3) — a pre-registered STOP upstream of R1's A/B menu — and the released
   diagnostics show the e/V0 split is a shape artifact (V1 swings μ_V0(h+) 0→0.42–0.70;
   branch-A central moves ±0.09–0.16 pp with the shape choice; μ_V0 = 0 at the bound in 4/4 h+
   groups against a 3%-charge-symmetric MC prediction). Branch A would zero 67,081 of 67,092 h+
   V0-equivalents on that basis and still need the SPLIT envelope — not supportable now.
2. **B1-now is the one option the numbers contradict**: it charges 0.063%/0.330% (medians,
   totals +0.0004/+0.0023 pp) for a quantity whose own shape-choice diagnostic moves μ_comb by
   0.9–3.2× the charged σ in 6/8 groups and whose split term (0.55%/0.20% median) it leaves
   uncovered — adopting it as closure would understate a demonstrated instability.
3. **If the fit's information must be captured in the budget now: option (b) flat-30%-replacing-
   the-4-bgsub-legs** (component medians 0.948%/1.069% = the decision brief's figures; totals
   6.047%/6.357%, +0.070/+0.060 pp vs installed; coherent 2.32%/2.39%; 0 bins >100%) — the only
   variant whose coverage exceeds every demonstrated swing (B1 coherent 0.25%/0.87%, branch-A
   +0.21%/+0.87%, V1-vs-nominal ±0.09–0.16 pp, SPLIT median-wise). Record B1/B2/B3/SPLIT as audit
   rows; keep the installed 19-comp budget (5.9764%/6.2968%) as the quoted budget until a refit
   passes GOF (χ²/ndf ≈ 1) with the split stable at the σ level (2026-10-17 path-A review
   conditions C1–C5). Residual honesty bound: even flat-30% does not cover a full μ_V0 0-vs-1
   zeroing (v0-share median 1.6–1.7%) — that hypothesis is alive only because GOF failed, and
   killing or confirming it is precisely the re-measurement's job.

*Computed 2026-10-19 from released artifacts only; scratch scripts `/tmp/opencode/fitnorm_pricing*.py`
(not workspace artifacts). No files under `output_v6/` or `data_release/` were modified.*
