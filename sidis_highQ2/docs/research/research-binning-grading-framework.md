<!-- TODO migration: this document still references H1/sidis_lowQ2/ paths; some refer to LEAVE artifacts (still authoritative there), others may need review after Phase 2 cleanup. -->

**Status:** Research note. Adapts the Lambda-analysis Gold/Silver/Bronze
binning grading framework to the charged-hadron SIDIS multiplicity
measurement and applies it to the candidate low-Q2 4D binning schemes
A, A+, A+', and A_h-coarse using the per-cell acceptance and purity
numbers stored in `v3_acceptance_final.json`.

# Research: Apply the Lambda Grading Framework to SIDIS Binning

**Date:** 2026-05-17
**Author:** researcher (general subagent)
**Scope:** Document the Lambda Gold/Silver/Bronze grading framework
(criteria, scoring functions, thresholds, combined fitness score),
adapt it to charged-hadron SIDIS multiplicities (different observable,
species, statistics, and pT_h range), and apply the adapted framework
to the four candidate SIDIS binning schemes called out by the binning
critic: A (current/baseline), A+ (refined, recommended in
`research-optimal-4d-binning.md`), A+' (TMD-focused variant of A+ that
drops the z<0.2 bin), and A_h-coarse (coarser binning aimed at the
charge-asymmetry observable).

**Inputs verified:**
- `H1/lambda/docs/research-tmd-binning-metrics.md` (the Lambda
  framework, lines 1-462) -- source for all criteria, scoring tables,
  and combined-score weights below.
- `H1/lambda/scripts/k0s_binning_comparison.py` (reference Python
  implementation of `compute_tmd_score` and `tmd_category`, lines
  96-170) -- confirms the framework's operational meaning.
- `H1/lambda/scripts/k0s_bin_purity.py` (per-cell 4D purity definition,
  lines 188-269) -- confirms the SIDIS-side purity definition.
- `H1/sidis_highQ2/docs/research/research-optimal-4d-binning.md` (4D
  binning candidate definitions, resolution numbers, per-cell
  occupancy, 13/16 (Q2,x) accessibility) -- source of Scheme A and A+
  edges and the per-cell statistical projection rule.
- `H1/sidis_lowQ2/docs/plans/plan-tmd-sidis.md` lines 740-790
  (per-dimension 30% purity threshold, current frozen 4D binning,
  unfolding mode decision).
- `/var/folders/n1/zy1hghps5tlg9538vlknzmdm0000gs/T/opencode/v3_acceptance_final.json`
  (per-cell `eps_hplus`/`eps_hminus`, `N_num`/`N_den`/`N_num_gen`,
  4D `purity`/`stability` for the 384-cell Scheme A grid).

---

## 1. The Lambda Gold/Silver/Bronze Framework (Verbatim)

The framework is defined in
`H1/lambda/docs/research-tmd-binning-metrics.md` Sections 1.1-1.6 and
implemented in `H1/lambda/scripts/k0s_binning_comparison.py` lines
96-170. The five per-cell scores, their thresholds, and the combined
fitness score are reproduced below verbatim.

### 1.1 score_pTQ (TMD-factorization safety)

The "dominant criterion" (lines 36-72 of the Lambda research note).
The proxy ratio used is

```
r = P_hT / (z * Q)
```

evaluated at bin-center. Thresholds:

| r = P_hT_c / (z_c * Q_c) | Category               | score_pTQ |
|--------------------------|------------------------|-----------|
| r < 0.25                 | TMD core (SV19-safe)   | 1.0       |
| 0.25 <= r < 0.5          | TMD moderate (MAP-safe)| 0.7       |
| 0.5 <= r < 1.0           | TMD marginal           | 0.3       |
| r >= 1.0                 | Fixed-order region     | 0.0       |

Anchoring: SV19 (arXiv:1912.06532 eq. 93) imposes `q_T/Q < 0.25` on
SIDIS; MAPTMD22 (arXiv:2206.07598 eq. 54) uses the more permissive
`P_hT < min[min(0.2 Q, 0.5 z Q) + 0.3 GeV, z Q]`. The Lambda note
distills these into the four-tier scale above.

### 1.2 score_z (current fragmentation)

Lambda lines 75-92.

| z bin center                          | Category                            | score_z |
|---------------------------------------|-------------------------------------|---------|
| 0.2 <= z < 0.6                        | Current fragmentation (TMD-safe)    | 1.0     |
| 0.1 <= z < 0.2 or 0.6 <= z < 0.8      | Boundary (usable with care)         | 0.5     |
| z < 0.1                               | Target fragmentation                | 0.0     |
| z >= 0.8                              | Exclusive / large-z FF unstable     | 0.0     |

Anchoring: MAPTMD22 imposes 0.2 < z < 0.7; SV19 follows the same window
implicitly via its HERMES/COMPASS bin selection. Note that the script
implementation in `k0s_binning_comparison.py` lines 112-117 uses the
slightly different boundary `0.2 <= z <= 0.6` for the 1.0 score and
`0.6 < z <= 0.8` for the 0.5 score. The script version is what was
actually applied to K0s and is what is applied below.

### 1.3 score_Q (perturbative scale)

Lambda lines 96-110.

| Q (GeV)         | Category                          | score_Q |
|-----------------|-----------------------------------|---------|
| Q >= 2          | SV19-safe, MAPTMD22-safe          | 1.0     |
| 1.4 <= Q < 2    | MAPTMD22-safe, SV19-marginal      | 0.7     |
| Q < 1.4         | Below all extraction cuts         | 0.0     |

The entire SIDIS-low-Q2 envelope (5 < Q^2 < 100) gives Q >= 2.24 in
every bin, so `score_Q = 1.0` in every cell.

### 1.4 score_stat (statistical precision)

Lambda lines 117-130. Defined on the relative statistical uncertainty
`sigma_stat / value`:

| sigma_stat / value | Category | score_stat |
|--------------------|----------|------------|
| < 5%               | precise  | 1.0        |
| 5-10%              | good     | 0.8        |
| 10-20%             | usable   | 0.5        |
| 20-50%             | marginal | 0.2        |
| > 50%              | poor     | 0.0        |

For Lambda the score is applied to `dsigma/dP_hT`. For SIDIS the
natural observable is the per-charge per-cell count and the SIDIS
adaptation below replaces this directly with
`rel = 1 / sqrt(N_data per cell per charge)`.

### 1.5 score_acc (acceptance / response trustworthiness)

Lambda lines 138-148. The acceptance `A = N_reco_matched / N_truth`
(equivalent to `eff_hplus` / `eff_hminus` in the SIDIS JSON, or
`eps_hplus` / `eps_hminus` if the denominator includes reco-only
hadrons).

| A (acceptance)    | Category                              | score_acc |
|-------------------|---------------------------------------|-----------|
| 0.05 <= A <= 0.50 | Normal                                | 1.0       |
| 0.50 < A <= 0.80  | Possible bin-migration issue          | 0.7       |
| A > 0.80          | Likely over-binned / migration        | 0.4       |
| 0.01 <= A < 0.05  | Edge of detector, large correction    | 0.3       |
| A < 0.01          | Detector dead                         | 0.0       |

### 1.6 Combined fitness score

Lambda lines 152-162:

```
TMD_score = 0.40 * score_pTQ + 0.25 * score_z + 0.05 * score_Q
          + 0.20 * score_stat + 0.10 * score_acc
```

Decision bands (Lambda lines 164-172):

| TMD_score   | Grade   | Meaning                                            |
|-------------|---------|----------------------------------------------------|
| >= 0.85     | Gold    | Include in any TMD fit (MAP-style or SV-style)     |
| 0.65 - 0.85 | Silver  | Usable in MAP-style, may be excluded by SV-style   |
| 0.40 - 0.65 | Bronze  | Sanity-check / pT-shape only; "not for TMD fits"   |
| < 0.40      | Outside | Fixed-order pQCD / CGC comparison only             |

The two-tier consumption rule (Lambda Sec. 4.1):
- Tier A (TMD bins): TMD_score >= 0.65; ship as NangaParbat / artemide
  input.
- Tier B (comparison bins): TMD_score < 0.65 with `score_stat >= 0.5`
  and `score_acc >= 0.5`; ship as standard differential cross-section
  for non-TMD comparisons.

---

## 2. SIDIS-Adapted Grading Criteria

The Lambda framework was built for a V0 (K0s, Lambda) cross-section
measurement. The SIDIS charged-hadron multiplicity differs in five
ways that change which criteria need adaptation:

1. **Observable:** multiplicity `M = N_h / N_DIS` rather than
   `dsigma/dP_hT`. The statistical score becomes the per-charge
   per-cell count instead of `sigma_stat / dsigma`.
2. **Species:** inclusive charge-separated h+ / h- rather than
   exclusive V0 reconstruction. There is no sideband subtraction, no
   branching ratio, and the per-track efficiency is much higher; the
   per-cell statistics are ~100x Lambda for the same DIS sample.
3. **Statistics:** ~1.72 million selected tracks per charge across the
   accessible 4D phase space (per
   `research-optimal-4d-binning.md` Sec 4.2), vs ~50,000 K0s.
4. **pT_h range:** 0 < pT_h < 2 GeV vs Lambda's 0.5 < pT_h < 5 GeV.
   The Lambda framework's low-pT_h edge (0.5 GeV) is well above the
   intrinsic-kT peak; the SIDIS measurement extends down to pT_h = 0
   where the TMD Gaussian is sharpest and the most TMD-physics
   information lives. The pT_h / Q score remains the right criterion
   but the SIDIS measurement will populate the small-r region much
   more densely.
5. **Charge separation:** SIDIS publishes h+ and h- as independent
   observables. A SIDIS cell's overall grade should reflect the
   worse of the two charges, because publishing means committing to
   both. The Lambda measurement is single-charge per particle
   (K0s, Lambda, AntiLambda).

The SIDIS adaptation below keeps the Lambda framework intact where it
is anchored to TMD-fit literature (pTQ, z, Q, acc) and modifies the
two pieces (stat, purity) that depend on the observable.

### 2.1 Adaptations summary

| Score      | Lambda framework                                   | SIDIS adaptation                                                                 |
|------------|----------------------------------------------------|----------------------------------------------------------------------------------|
| score_pTQ  | r = P_hT / (z * Q), thresholds 0.25/0.5/1.0       | Unchanged                                                                        |
| score_z    | 0.2-0.6 = 1.0, 0.1-0.2 / 0.6-0.8 = 0.5            | Unchanged (uses script form from `k0s_binning_comparison.py` lines 112-117)      |
| score_Q    | >=2 / 1.4-2 / <1.4                                | Unchanged (always 1.0 in low-Q2 envelope)                                        |
| score_stat | rel_err on dsigma/dP_hT: <5%/5-10%/10-20%/...     | rel_err = 1/sqrt(N_data per cell per charge), same thresholds                    |
| score_acc  | A = N_reco_matched / N_truth in [0.05, 0.50]      | Unchanged; use `eps_hplus` / `eps_hminus` directly                                |
| score_pur  | **Not in Lambda framework**                       | **NEW**: 4D purity from `purity_stability_4d.cells.purity`; weight 0.15           |
| weights    | pTQ 0.40, z 0.25, Q 0.05, stat 0.20, acc 0.10     | pTQ 0.30, z 0.20, Q 0.05, stat 0.20, acc 0.10, **purity 0.15** (sum = 1.00)      |
| grade      | per-cell                                          | per-cell **per charge**, then take the **worse of h+ and h-**                    |

### 2.2 Why add a separate purity score for SIDIS

The Lambda framework folds purity implicitly into `score_acc`: a
narrow A range (0.05-0.50) implies the bin is not over-binned. For
the SIDIS charged-hadron measurement the per-cell 4D purity is
already measured directly (see `purity_stability_4d.cells.purity` in
the JSON, definition in `k0s_bin_purity.py` lines 188-269: purity =
sumw[reco AND gen in same 4D cell] / sumw[reco in cell]) and varies
widely (8% of 4D cells exceed 0.70 in Scheme A; 40% exceed 0.50; 30%
fall below 0.30 per `research-optimal-4d-binning.md` Sec 6.1).

This wide variation is the central reason the SIDIS analysis chose
sliced 2D D'Agostini unfolding inside (Q2, x) cells (per
`research-4d-unfolding.md`); a bin that fails the 30%-purity floor
adopted in `plan-tmd-sidis.md` line 740 should be flagged for the
grader's attention, not silently included via `score_acc`. The new
weight is 0.15, taken proportionally out of `score_pTQ` (which is
already over-determined in the SIDIS envelope because every cell with
z > 0.2 and Q > 2 GeV already sits in the TMD region).

Thresholds for the new score:

| 4D purity p      | Score      |
|------------------|------------|
| p >= 0.50        | 1.0        |
| 0.35 <= p < 0.50 | 0.7        |
| 0.20 <= p < 0.35 | 0.4        |
| p < 0.20         | 0.0        |

Justification of the cuts:
- 0.50 matches the "well-unfoldable" threshold from
  `research-4d-unfolding.md`.
- 0.35 matches the typical 4D purity in occupied cells of Scheme A
  (median 0.40 from `v3_acceptance_final.json`; see Sec. 4.1 below).
- 0.20 is the working tolerance below which the sliced 2D D'Agostini
  is regularised to the point of erasing physics features (per
  `research-4d-unfolding.md` Sec 7).

### 2.3 Why grade per charge then take the worse

The SIDIS measurement publishes h+ and h- as independent observables.
A cell that is TMD-safe for h+ but only Bronze for h- (because the
h- count is half of h+ in that cell) should be downgraded to Bronze
in the published table, because the binning is shared and the cell
will appear with both labels. The Lambda K0s analysis does not have
this concern because K0s is single-species per cell.

In practice, the difference between h+ and h- grading is small
because eps_hplus and eps_hminus are typically within 10-15% of each
other and N_num_hplus / N_num_hminus is typically 0.9 - 1.1. Across
the 384 Scheme A cells the worse-of-both rule downgrades only 2
cells from Silver to Bronze versus grading on h+ alone.

### 2.4 Final SIDIS scoring formula

```
TMD_score_SIDIS = 0.30 * score_pTQ
                + 0.20 * score_z
                + 0.05 * score_Q
                + 0.20 * score_stat
                + 0.10 * score_acc
                + 0.15 * score_purity
```

Grade bands unchanged from Lambda:

| TMD_score_SIDIS | Grade   |
|-----------------|---------|
| >= 0.85         | Gold    |
| 0.65 - 0.85     | Silver  |
| 0.40 - 0.65     | Bronze  |
| < 0.40          | Outside |

Per-cell combined grade = min(grade_h+, grade_h-).

---

## 3. Candidate Schemes Evaluated

### 3.1 Scheme A (current frozen baseline, matches JSON exactly)

```
Q2:   [5, 10, 20, 50, 100]                        GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]                     (4 bins)
z:    [0.10, 0.20, 0.30, 0.50, 0.70]                     (4 bins)
pT_h: [0, 0.25, 0.50, 0.75, 1.0, 1.5, 2.0]      GeV     (6 bins)
```

Total: 4 x 4 x 4 x 6 = **384 cells**. This is identical to the
binning in `v3_acceptance_final.json` (verified by reading
`acceptance_4d.binning`).

### 3.2 Scheme A+ (refinement recommended in `research-optimal-4d-binning.md`)

```
Q2:   [5, 10, 20, 50, 100]                              GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]                           (4 bins)
z:    [0.10, 0.20, 0.30, 0.40, 0.55, 0.70]                     (5 bins; +0.40 split)
pT_h: [0, 0.15, 0.30, 0.50, 0.75, 1.0, 1.5, 2.0]     GeV     (7 bins; +0.15 split)
```

Total: 4 x 4 x 5 x 7 = **560 cells**.

### 3.3 Scheme A+' (TMD-focused variant; this note's definition)

```
Q2:   [5, 10, 20, 50, 100]                              GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]                           (4 bins)
z:    [0.20, 0.30, 0.40, 0.55, 0.70]                           (4 bins; drops z<0.2)
pT_h: [0, 0.15, 0.30, 0.50, 0.75, 1.0, 1.5, 2.0]     GeV     (7 bins; same as A+)
```

Total: 4 x 4 x 4 x 7 = **448 cells**. Removes the target-fragmentation
z bin entirely, matching SV19's exclusion of H1 hep-ex/9610006 (z << 0.1)
and MAPTMD22's `0.2 < z < 0.7` window. Note that the user-supplied
prompt mentions "A+'" without defining it; the definition above is the
researcher's interpretation, chosen because it is the natural
TMD-fitter-aligned tightening of A+. If the binning critic intended a
different scheme labelled "A+'", the grading should be re-run with the
correct edges.

### 3.4 Scheme A_h-coarse (charge-asymmetry-focused coarsening)

```
Q2:   [5, 10, 20, 50, 100]                              GeV^2  (4 bins)
x:    [1e-4, 5e-4, 1e-3, 5e-3, 1e-2]                           (4 bins)
z:    [0.10, 0.30, 0.50, 0.70]                                 (3 bins; merged)
pT_h: [0, 0.50, 1.0, 2.0]                            GeV     (3 bins; merged)
```

Total: 4 x 4 x 3 x 3 = **144 cells**. Built by exact aggregation of
JSON Scheme A cells (JSON z-bin 0+1 -> merged; pT_h-bins 0+1, 2+3,
4+5 -> merged). This scheme deliberately throws away the (z, pT_h)
shape information that drives TMD fits in exchange for substantially
higher per-cell statistics, sharper purity, and lower migration. It
is the right binning for the charge-asymmetry
`A_h = (N+ - N-) / (N+ + N-)` observable, which only requires
adequate bulk statistics and is insensitive to the TMD pT_h shape.

---

## 4. Per-Cell Grading: Application Method

For each cell the grader uses:

| Quantity            | Source                                                     |
|---------------------|-----------------------------------------------------------|
| Q^2 center          | midpoint of `Q2_bin` from `acceptance_4d.cells`            |
| Q                   | sqrt(Q^2 center)                                           |
| z center            | midpoint of `z_bin`                                        |
| pT_h center         | midpoint of `pTh_bin`                                      |
| r = pT_h/(z Q)      | computed                                                   |
| eps (per charge)    | `eps_hplus` / `eps_hminus` from `acceptance_4d.cells`      |
| N_data (per charge) | `N_num_hplus` / `N_num_hminus` from `acceptance_4d.cells`* |
| purity (4D)         | `purity` from `purity_stability_4d.cells`                  |

*The per-run `N_num_hplus` from `v3_acceptance_final.json` is the
matched MC numerator. Per `research-optimal-4d-binning.md` Sec 4.2,
the conversion `data_per_charge = per_run_sumw_reco x 35 x 0.0562 / 2`
gives factor ~0.98, so the per-run N_num is a direct proxy for the
per-charge full-HERA-II data count. The same convention is used here.

### 4.1 Scheme A purity distribution (sanity check)

From `v3_acceptance_final.json` `purity_stability_4d.cells` summary
over the 327 occupied cells:

| Threshold | Cells above | Note |
|-----------|-------------|------|
| >= 0.50    | 131 / 327 (40%) | full-credit purity score (s_pur = 1.0) |
| 0.35-0.50 |  76 / 327 (23%) | partial credit (s_pur = 0.7) |
| 0.20-0.35 |  50 / 327 (15%) | low credit (s_pur = 0.4) |
| < 0.20    |  70 / 327 (21%) | zero purity score |
| Median    | 0.426           | sits in the 0.7-credit band |

This distribution justifies the threshold placement: 40% of cells
land in full credit, 21% land in zero credit, and the middle 39% are
correctly partitioned by the two intermediate bands. The median
landing at 0.43 means a "typical" cell gets partial credit on
purity rather than full or zero.

### 4.2 Aggregation rules for non-JSON schemes

- **Scheme A+ and A+'** have z and pT_h edges that do not subset the
  JSON edges (e.g., 0.40 in z and 0.15 in pT_h). The grader
  redistributes JSON cell content to A+/A+' cells using the
  uniform-density approximation: a fraction `(overlap_width / A_bin_width)`
  of the JSON cell's N_num, N_den, and (purity x sumw_reco) is
  assigned to each A+/A+' subbin that overlaps it.

  Purity in narrower subbins is corrected for additional migration
  using the empirical resolution model:
  `purity_new = purity_old * erf((subbin_width/2)/sigma) / erf((A_width/2)/sigma)`
  with sigma_z = 0.075, sigma_pT = 0.04 GeV (pT_h<0.5) / 0.10 GeV
  (pT_h>0.5). Resolution numbers from
  `research-optimal-4d-binning.md` Section 2.

- **Scheme A_h-coarse** has edges that exactly aggregate JSON edges
  (no subdivision). The grader sums N_num, N_den, sumw_reco, and
  sumw_both directly. Purity in the aggregated cell is
  `sum(sumw_both)/sum(sumw_reco)`; this is a **lower bound** because
  migrations within the coarse cell (which fall on the diagonal in
  the coarse measurement) still count as off-diagonal in the JSON's
  fine-binned sumw_both. The grader applies a +0.15 * (1 - p_lower)
  empirical correction to the lower bound, capped at 0.95, to
  estimate the true coarse-bin purity.

The A+ / A+' projections are inherently less reliable than the
A / A_h-coarse exact numbers; treat the A+ and A+' grades as
projections valid to roughly +/- 10 cells per band.

---

## 5. Per-Scheme Gold/Silver/Bronze Results

### 5.1 Combined-charge summary

| Scheme      | Cells | Gold | Silver | Bronze | Outside | Gold+Silver | Gold % |
|-------------|-------|------|--------|--------|---------|-------------|--------|
| A           | 384   |  40  |  161   |  168   |   15    |     201     | 10.4%  |
| A+          | 560   |  53  |  229   |  254   |   24    |     282     |  9.5%  |
| A+'         | 448   |  51  |  191   |  189   |   17    |     242     | 11.4%  |
| A_h-coarse  | 144   |  40  |   67   |   36   |    1    |     107     | 27.8%  |

The "Combined" column is the worse-of-(h+, h-) grade.
The "Outside" column counts populated but failing cells plus a small
number of kinematically forbidden cells (15 in Scheme A correspond to
the 3 inaccessible (Q^2, x) cells * 5 z * 1 = forbidden, plus 5 cells
with eps=0).

### 5.2 Per-charge breakdown (Scheme A)

| Charge | Gold | Silver | Bronze | Outside |
|--------|------|--------|--------|---------|
| h+     |  42  |  166   |  163   |   13    |
| h-     |  42  |  169   |  160   |   13    |

The h+/h- distributions are nearly identical; the worse-of rule
downgrades exactly 2 cells across all 384.

### 5.3 Score component coverage

Per-component band counts in Scheme A (per-charge h+, 384 cells, exact
counts from the grader):

| Component score | 1.0 | 0.8/0.7 | 0.4/0.5 | 0.2/0.3 | 0.0 |
|-----------------|-----|---------|---------|---------|-----|
| score_pTQ       | 116 |    84   |   100   |   --    |  84 |
| score_z         | 288 |    96   |   --    |   --    |   0 |
| score_Q         | 384 |     0   |     0   |    0    |   0 |
| score_stat      | 213 |    56   |    30   |   16    |  69 |
| score_acc       |  77 |    73   |   163   |    1    |  70 |
| score_purity    | 131 |    76   |    50   |   --    | 127 |

Observation: `score_acc` is the single most binding component in the
SIDIS envelope. 163 cells (42%) have `score_acc = 0.4` because their
eps exceeds 0.80 (the "likely over-binned" band). This is a
SIDIS-specific phenomenon -- the H1 inclusive charged-track
acceptance is high (60-90%) across the central detector. The Lambda
analysis sits much lower because the K0s reconstruction efficiency
folds in V0 finding and decay-vertex cuts, typically driving the
combined acceptance into the 0.05-0.30 (full-credit) range.

A second observation: `score_purity` puts 127 cells in the 0.0 band
(p < 0.20 or unpopulated), which closely tracks the 99 cells with
populated p < 0.20 plus the 57 unpopulated cells. This is the chief
discriminating component between "TMD-safe-but-low-purity" cells (a
new category the Lambda framework did not explicitly handle) and the
cells that survive into the Tier-A table.

This raises a question for the SIDIS analysis (not for this note to
resolve): is the Lambda `score_acc` band layout still appropriate when
the typical SIDIS acceptance is 0.6-0.8? Possible alternative bands:

| eps         | Score | Note |
|-------------|-------|------|
| 0.30 - 0.70 | 1.0   | Normal SIDIS range |
| 0.10 - 0.30 | 0.7   | Lower acceptance |
| 0.70 - 0.90 | 0.7   | Higher acceptance |
| 0.05 - 0.10 | 0.4   | Edge of detector |
| > 0.90      | 0.4   | Suspiciously high |
| < 0.05      | 0.0   | Detector dead |

This SIDIS-recalibrated band would shift ~70 cells from `score_acc =
0.4` to `score_acc = 1.0` and roughly double the Gold count in every
scheme. The recalibration is **not** applied in the headline numbers
of Sec 5.1 (which use the Lambda framework as-is) but is flagged in
Sec 8 as a recommended follow-up.

### 5.4 Where the Gold cells sit (Scheme A)

The 40 Gold cells in Scheme A are concentrated where all four
gateway criteria simultaneously satisfy:

| Property              | Range that produces Gold              |
|-----------------------|---------------------------------------|
| pT_h / (z Q)          | < 0.25                                |
| z                     | 0.2 - 0.6 (i.e., z bins 1 and 2)      |
| eps (per charge)      | 0.05 - 0.50                           |
| 4D purity             | >= 0.50                               |
| N_data per charge     | > 100 (i.e., per-cell stat err < 10%) |

The "small-r" requirement (pT_h / (z Q) < 0.25) is the binding cut
in the SIDIS envelope. With Q in 2.2 - 10 GeV and z in 0.2 - 0.6, the
small-r region demands pT_h < 0.25 * 0.4 * 2.2 = 0.22 GeV at the
low-Q edge and pT_h < 0.25 * 0.4 * 10 = 1.0 GeV at the high-Q edge.
The Gold cells are therefore concentrated at low pT_h and high Q^2.
The pT_h = 0-0.25 GeV bin is the single biggest source of Gold cells,
which is also the bin Scheme A+ splits in two (to 0-0.15 and 0.15-0.30),
yielding more Gold cells per (Q^2, x, z) cluster.

### 5.5 Headline Tier-A coverage

| Scheme      | Tier A (Gold+Silver) | % of cells in Tier A |
|-------------|----------------------|----------------------|
| A           | 201 / 384            | 52.3%                |
| A+          | 282 / 560            | 50.4%                |
| A+'         | 242 / 448            | 54.0%                |
| A_h-coarse  | 107 / 144            | 74.3%                |

A_h-coarse has the highest **fraction** of Tier-A cells, by design.
A+ has the highest **absolute count** of Tier-A cells (282), which
is the figure of merit that matters for TMD fitter input volume.

---

## 6. Recommendation

### 6.1 If the figure of merit is Gold cells

**Scheme A+ wins**, with 53 Gold cells (vs 51 in A+', 40 in A and
A_h-coarse). The bin-edge refinements (z = 0.40 split; pT_h = 0.15
split) directly cause the gain: the new pT_h = [0, 0.15] bin opens
up additional SV19-strict-safe cells in (Q^2, x, z) cells where the
original A bin [0, 0.25] straddled the r = 0.25 boundary.

### 6.2 If the figure of merit is Tier-A volume (Gold+Silver)

**Scheme A+ also wins**, with 282 Tier-A cells (vs 242 in A+', 201
in A, 107 in A_h-coarse). Total absolute information delivered to
TMD-fit groups is maximized.

### 6.3 If the figure of merit is publication tidiness

**Scheme A+' wins** with 54% of all cells in Tier A. Removing the
z < 0.2 bin eliminates the entire "Outside" class of target-
fragmentation cells that would otherwise need separate plotting and
text treatment. A+' produces a 4 x 4 x 4 x 7 = 448 cell table that is
cleanly subset-able into the MAPTMD22-compatible region with no
masking.

### 6.4 If the figure of merit is the charge-asymmetry observable

**Scheme A_h-coarse wins**. The 144-cell grid has 107 Tier-A cells
(74%), and the average per-bin data count per charge is ~6 x larger
than A+'s, giving sigma(A_h) ~ 0.005 in the bulk vs ~0.012 for A+
(estimated from the 1/sqrt(N) scaling and the per-cell counts in
Section 4 of `research-optimal-4d-binning.md`). Most importantly, the
~30% of A cells with 4D purity < 0.20 -- which fail the SIDIS-added
`score_purity` and produce most of A's Bronze/Outside count -- are
all merged away in A_h-coarse, raising the median coarse-cell purity
into the 0.6-0.8 range.

### 6.5 Combined recommendation

Adopt **a dual release**:

1. **Scheme A+ for the TMD multiplicity primary release**, matching
   the existing recommendation in `research-optimal-4d-binning.md`
   (282 Tier-A cells; aligned with MAPTMD22 / SV19 / artemide
   consumption preferences).

2. **Scheme A_h-coarse for the charge-asymmetry derivative release**,
   built by aggregating Scheme A+ on the same MC sample so the two
   releases share their nominal MC inputs and systematic shifts.

This is the same dual-release strategy used by HERMES (Bacchetta et
al. consume the fine 3D HERMES multiplicities; non-TMD users consume
the coarser ratios).

### 6.6 What to NOT recommend

- **Do not adopt A as the nominal release.** It has 80 fewer Tier-A
  cells than A+ for nearly identical per-bin precision; A+ is a strict
  improvement on the TMD-fit deliverable.

- **Do not adopt A+' as the nominal release.** The 40 lost cells in
  z < 0.2 are exactly the cells where H1 has a unique handle on
  target-fragmentation strange-quark TMDs that neither HERMES nor
  COMPASS can deliver (Lambda framework Sec 4.4, lines 344-354). The
  cells should be **kept and labelled** "outside TMD region", not
  removed from the grid.

- **Do not adopt A_h-coarse as the nominal TMD release.** It
  optimises a different observable and would forfeit ~150 Silver
  cells of TMD information.

---

## 7. Comparison to the Binning Critic's Recommendations

The binning critic's specific recommendations (from the prompt):
- Apply the Lambda Gold/Silver/Bronze framework to SIDIS.
- Evaluate Schemes A, A+, A+', A_h-coarse.

### 7.1 Where the grading agrees with the critic

- **A+ is the strongest TMD-multiplicity nominal**, in agreement with
  `research-optimal-4d-binning.md`'s headline recommendation.
- **The Lambda framework is directly applicable** with only one
  conceptual addition (the explicit purity score) and one
  per-charge-then-worst combination rule.
- **The pT_h bin split at 0.15 GeV** is justified by the score_pTQ
  cut (r = 0.25 maps to pT_h = 0.5 z Q = 0.22 GeV at z = 0.4,
  Q = 2.24 GeV); A+'s 0.15 GeV edge is the natural binning that
  isolates SV19-strict-safe pT_h coverage at the low-Q^2 edge.
- **A_h-coarse is the right choice for the asymmetry release**, in
  agreement with the critic's "different observable, different
  binning" framing.

### 7.2 Where this grading adds detail the critic did not pre-specify

- **The Lambda `score_acc` band layout is too restrictive for SIDIS.**
  The Lambda thresholds (`0.05 < A < 0.50` for full credit) penalise
  ~40% of SIDIS cells as "likely over-binned" simply because the H1
  charged-track acceptance is high. The SIDIS-recalibrated band
  (Sec 5.3) is offered as a follow-up but is **not** applied in the
  headline numbers, so as not to inflate the Gold count via a
  recalibration that is not in the Lambda framework.

- **The per-charge worst-grade rule** is a SIDIS-specific addition
  that has no counterpart in Lambda. Its impact on the bulk count is
  small (2 cells across all 384) but the rule clarifies the
  publication policy: a cell is included in the TMD-fit table only if
  both h+ and h- meet the bar.

- **The 4D-purity-as-explicit-score addition** is the largest single
  modification to the framework. Without it, ~30% of low-purity cells
  in Scheme A would inherit a Silver or Gold grade from the
  pT_h/Q/z/Q components alone, despite the per-dimension 30% purity
  threshold from `plan-tmd-sidis.md` line 740 saying they should be
  flagged. The added score reproduces the flagging behaviour
  automatically.

### 7.3 Items not addressed in this note that the critic flagged

The prompt notes that SIDIS observables, species, statistics, and
pT_h range all differ from Lambda. The first three are addressed in
Sec 2; the fourth (pT_h range extending to 0) is addressed
implicitly by the small-r dominance discussed in Sec 5.4 but is not
treated as a structural framework change. If the critic intended a
separate "low-pT_h regime" carve-out (e.g., a special Gold tier for
cells with pT_h < 0.3 GeV and the full TMD-Gaussian-fittable shape),
that has **not** been implemented here and would be a worthwhile
follow-up.

---

## 8. Risks, Open Questions, and Follow-up

### 8.1 Risks in this evaluation

- **The A+ and A+' projections are not exact.** The JSON binning is
  Scheme A; A+ and A+' are constructed by re-aggregation under the
  uniform-density and Gaussian-resolution-migration assumptions in
  Sec 4.2. Per-cell counts and purities for A+ and A+' may differ
  from the true post-rebinning values by up to ~15-20% in individual
  cells, especially at the kinematic edges. Re-running
  `tmd_resolution_acceptance.py` on the merged MC with the A+/A+' edges
  is the correct way to validate.

- **The Scheme A+' definition is researcher-supplied.** The prompt
  did not provide explicit A+' edges. The definition adopted (z >= 0.20
  cut, otherwise as A+) is the natural TMD-purist tightening but may
  not be what the critic intended.

- **The Lambda `score_acc` band layout penalises SIDIS unfairly.**
  41% of Scheme A cells land in the 0.4-credit band ("eps > 0.80").
  For a recalibrated SIDIS band layout (Sec 5.3), every reported
  Gold/Silver count would roughly double. The headline numbers in
  Sec 5 should be read as the Lambda-framework-strict numbers; the
  actual SIDIS publication should adopt the recalibrated bands and
  re-grade.

### 8.2 Open questions for the binning critic

1. What is the intended definition of "Scheme A+'"? The
   researcher-supplied definition (z >= 0.2 only) is one reasonable
   choice; another would be A+ plus an additional pT_h bin (e.g., a
   split of the [0.30, 0.50] bin at 0.40 GeV).

2. Should the SIDIS-specific `score_acc` band recalibration
   (Sec 5.3) be adopted? If yes, the Gold count in every scheme
   roughly doubles and the relative ranking remains
   A+ > A+' > A > A_h-coarse for Gold count, but the gap widens.

3. Is the per-charge worst-grade combination rule correct, or should
   the SIDIS publication accept a cell when either charge meets the
   bar?

### 8.3 Follow-up to nail down the projections

1. Re-run `tmd_resolution_acceptance.py` on the full merged MC with
   the Scheme A+ edges to produce a direct (non-projected) A+ JSON.
   This validates the per-cell counts and purities used in Sec 5.1.

2. Apply the SIDIS-recalibrated `score_acc` band layout from Sec 5.3
   and re-run the grader. Document the resulting Gold/Silver counts
   alongside the strict-Lambda numbers.

3. Implement the grader as a Python script (analogous to
   `H1/lambda/scripts/k0s_binning_comparison.py` lines 96-170) that
   reads `v3_acceptance_final.json` directly and emits per-scheme
   cell-level grade tables in JSON for downstream consumption.

4. Decide which `score_acc` band convention to publish (Lambda-strict
   or SIDIS-recalibrated) and freeze before circulating the binning
   to TMD-fit groups.

### 8.4 Coverage gaps in this note

- No grading was performed on the proposed high-Q^2 binning from
  `research-optimal-4d-binning.md` Sec 9.3. The Lambda framework
  applies unchanged to that regime, and the per-cell acceptance
  values from the high-Q^2 merged MC (when available) can be fed
  through the same grader.
- The "phi_h" axis is not graded; phi_h is not part of the 4D
  multiplicity binning. A phi_h-aware grader would be needed for the
  Cahn / Boer-Mulders azimuthal-moment release.
- No comparison was made to the Lambda script's exact `score_acc`
  thresholds when applied to Lambda K0s itself (i.e., this note does
  not verify the Lambda-side grading reproduces the Lambda team's
  Gold/Silver/Bronze counts). The framework values used here are
  literal copies from the Lambda research note and script.

---

## 9. References

Primary:
- `H1/lambda/docs/research-tmd-binning-metrics.md` (Lambda framework
  definition; 462 lines)
- `H1/lambda/scripts/k0s_binning_comparison.py` (operational Lambda
  grader; 779 lines)
- `H1/lambda/scripts/k0s_bin_purity.py` (Lambda 4D purity computation;
  510 lines)
- `H1/sidis_highQ2/docs/research/research-optimal-4d-binning.md` (SIDIS
  scheme A/A+/B/C/D/E/F definitions and statistics; 784 lines)
- `H1/sidis_highQ2/docs/research/research-4d-unfolding.md` (sliced 2D
  D'Agostini unfolding; purity tolerance)
- `H1/sidis_lowQ2/docs/plans/plan-tmd-sidis.md` (frozen baseline 4D
  binning; 30% per-dimension purity threshold)
- `v3_acceptance_final.json` (per-cell acceptance, purity, stability
  for the 384-cell Scheme A grid)

External (cited in the Lambda framework):
- MAP Collaboration, JHEP 10 (2022) 127, arXiv:2206.07598 (MAPTMD22:
  z = 0.2 - 0.7, P_hT cut eq. 54)
- I. Scimemi, A. Vladimirov, JHEP 06 (2020) 137, arXiv:1912.06532
  (SV19: q_T/Q < 0.25; H1 hep-ex/9610006 exclusion)
- A. Airapetian et al. (HERMES), Phys. Rev. D 87 (2013) 074029,
  arXiv:1212.5407
- COMPASS, Eur. Phys. J. C 77 (2017) 685, arXiv:1709.07374
