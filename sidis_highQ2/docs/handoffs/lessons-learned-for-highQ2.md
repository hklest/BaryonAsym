# Lessons Learned: Low-Q2 SIDIS for the High-Q2 Supervisor

Audience: the high-Q2 SIDIS charged-hadron multiplicity supervisor.
Purpose: every trap, bug, and methodology decision discovered during the
low-Q2 analysis that must not be repeated. Each lesson is: **what went
wrong**, **why**, **what to do differently**.

Read this in full before submitting any high-Q2 production or writing any
acceptance/unfolding code.

---

## 1. MC Luminosity / Normalization

### 1.1 THE N_DIS BUG (analysis-killing)

**What went wrong.** The multiplicity formula was implemented as

    M(z, pT_h) = N_data_corr / (N_DIS_MC * Δz * ΔpT_h)

with `N_DIS` taken from the MC `DisTot` tree. Result: every multiplicity
was wrong by a factor

    L_MC / L_data ≈ 5.29e6 / 2.98e5 ≈ 17.8  (RAPGAP)
    L_MC / L_data ≈ 4.11e6 / 2.98e5 ≈ 13.8  (DJANGOH)

(combined effective miscount ~29 in earlier code that used both samples
naively summed). Multiplicities were ~1-2 orders of magnitude too small
and looked like nothing in the literature.

**Why.** Conceptual slip: the acceptance correction undoes detector
effects so `N_corr` is in **data units**. The DIS denominator must also
be in data units, otherwise units do not cancel.

**What to do.** The correct formula is

    ε(bin)        = N_matched_reco_MC(bin) / N_gen_MC(bin)      # dimensionless ratio
    N_corr(bin)   = N_data_reco(bin) / ε(bin)                   # data-equivalent gen yield
    M(z, pT_h)    = N_corr / (N_DIS_DATA * Δz * ΔpT_h)          # data DIS in denominator

`N_DIS_DATA` is the count of selected DIS events in the inclusive data
DST after the same DIS selection used upstream (Q^2, y, vertex cuts).
There must be exactly one `N_DIS` count per (Q^2, y) bin and it must
come from data.

For high-Q2, expect `N_DIS_DATA` ~ O(10^5-10^6) per Q^2 bin (the
high-Q2 sample is small). Sanity-check that `M < 1` at low z and
falls steeply, consistent with HERA SIDIS literature.

### 1.2 How MC luminosity cancels in acceptance

`ε = N_matched_reco / N_gen` is a ratio of two MC quantities **from the
same sample**. The per-event MC weight (which is `L_MC^{-1}` times the
cross-section) appears in numerator and denominator and cancels
**exactly**. Do not "luminosity-correct" the acceptance. Do not multiply
by `L_data / L_MC` anywhere in the acceptance pipeline.

### 1.3 RAPGAP vs DJANGOH normalization

Per-file luminosities differ because the samples were generated with
different settings:

    RAPGAP31 rad : L_MC = 5,293,265 nb^-1  (34 files)
    DJANGOH14 rad: L_MC = 4,108,957 nb^-1  (90 files)
    Data         : L_data = the superseded pre-rescale luminosity nb^-1

Two distinct comparisons:

1. **Acceptance model systematic** (RAPGAP vs DJANGOH ε): both are
   dimensionless ratios. No luminosity rescaling. The model
   systematic is `|ε_RAPGAP - ε_DJANGOH| / 2` per bin.

2. **Absolute yield comparison** (raw `N_gen`): RAPGAP has higher
   `L_MC` per file but fewer files. If you ever compare unweighted
   counts you will be misled. Always use the per-event weight stored
   in the tree (`Weight1` etc.), which already encodes `1/L_MC` times
   the cross-section, so `Σ weights ≈ N_expected_at_L_data` only after
   multiplying by `L_data`. The cleanest workflow is to keep
   acceptance as ratios and never use absolute weighted MC yields in
   the final multiplicity.

---

## 2. GenElecPhi Consistency

**What went wrong.** The v5b builder stores `GenElecPhi` on every
`ChargedTot` track (broadcast from the per-event generator-level
scattered electron). The low-Q2 acceptance code used this `GenElecPhi`
in the **numerator** (matched-reco loop) to compute gen-level
`phi_h_gen`, but in the **denominator** (gen-loop) it had a fallback
`phi_e = 0` whenever the per-track field was missing. Because
`phi_h = phi_pi - phi_e`, the two sides used inconsistent reference
frames, and `ε > 1` appeared in many `phi_h` bins (efficiency cannot
exceed unity).

**Why.** Defensive coding ("if missing, use 0") silently shifted the
azimuthal reference frame in one of the two terms.

**What to do.** Both numerator and denominator must read `GenElecPhi`
from the same column with the same fallback policy. The strict policy
adopted for low-Q2 is:

    if not numpy.isfinite(GenElecPhi):
        raise ValueError("GenElecPhi non-finite in v5b tree")

v5b trees are guaranteed to have finite `GenElecPhi` on every track and
every gen entry. Do not add `numpy.where(..., 0.0, ...)` style
fallbacks. For high-Q2 the same v5b builder will be used (confirm the
column is present in the high-Q2 production output before running
acceptance) and the same strict policy must apply.

---

## 3. Beam Energy Consistency

**What went wrong.** Acceptance numerator used per-event `BeamElecE`
and `BeamProtP` from the reco tree; denominator used hard-coded
defaults `27.6` and `920.0`. Numerical impact was verified to be zero
(beams are constant for the 2005-2007 HERA-II e+p run), but the
inconsistency is a latent bug if the dataset ever changes.

**What to do for high-Q2.** Same physical beams (27.6 GeV e+ on 920 GeV
p). Use fixed values `Ee = 27.6`, `Ep = 920.0` on **both** sides of
the acceptance. Document the choice in a comment at the top of the
acceptance script.

---

## 4. phi_h Analysis Traps

### 4.1 The tautology bug

**What went wrong.** Closure test was implemented as "use MC reco as
pseudo-data, correct with the same MC's acceptance". Because

    N_corr = N_reco / (N_reco / N_gen) = N_gen

the corrected distribution is **identical** to gen-truth by
construction. The closure looked perfect and revealed nothing.

**What to do.** Closure must use either (a) real data, or (b) a
**different** MC for pseudo-data than for the response. The low-Q2
working closure uses RAPGAP reco as pseudo-data with DJANGOH
acceptance (or vice-versa), then checks recovery of the pseudo-data's
own gen-truth.

### 4.2 IBU is biased for phi_h; use BBB

**What went wrong.** Iterative Bayesian unfolding (IBU) was the
initial choice for phi_h, with 4 iterations. Result: A1 (cos phi_h
coefficient) came out systematically too negative (~0.05-0.10 below
truth) in closure. The cause is the large efficiency gradient across
phi_h (0.18 at SpaCal-edge phi values, 0.82 at central phi), which
IBU's prior-update step over-corrects.

**What to do.** Use bin-by-bin (BBB) unfolding for phi_h:

    N_corr(bin) = N_data(bin) / ε(bin)

The phi_h response matrix is nearly diagonal (91% purity, see
section 8), so BBB is the natural choice. Closure with BBB recovers
gen-truth A1 to within `δA1 = 0.002-0.064` per (Q^2, y, z, pT_h)
cell. IBU was retained as a cross-check only.

### 4.3 |A1| > 1 is physical

**What went wrong.** Initial fits to phi_h with the 3-parameter form

    dN/dphi_h = A0 * [1 + A1 cos(phi_h) + A2 cos(2 phi_h)]

returned `|A1| > 1` in several bins and were rejected as unphysical.
They are not unphysical. At H1 kinematics (Q ~ 3-4 GeV, pT_h up to
2 GeV), perturbative NLO contributions (Georgi-Politzer term and
beyond) push the cos modulation amplitude above unity. The 3-term
truncation is then insufficient — the true distribution requires
higher harmonics or a non-perturbative shape.

**What to do.** Publish the corrected 12-bin phi_h distribution **as
the primary result**. Report A1/A2 from the fit as secondary,
diagnostic numbers with the caveat that the truncation is incomplete
at high pT_h. Do not clamp or reject `|A1| > 1` bins.

### 4.4 A1 vs ⟨cos phi_h⟩ convention

The amplitude `A1` from the fit `A0 [1 + A1 cos phi_h + ...]` is **not**
the same as the moment `⟨cos phi_h⟩`. In the continuum limit

    ⟨cos phi_h⟩ = A1 / 2

Be explicit in tables and plots which one is being reported. Low-Q2
publishes both, labelled distinctly.

---

## 5. Systematic Uncertainty Traps

### 5.1 SpaCal angle: phi rotation vs polar shift

**What went wrong.** The SpaCal angular-resolution systematic was first
implemented as a rotation of phi_h by `±δphi`. Because the
acceptance and the data distribution are rotationally invariant in
phi_h (the detector is azimuthally symmetric over the SpaCal
acceptance to good approximation), the systematic came out exactly
zero. This is correct for a rotation but is not the systematic that
was intended.

**What to do.** The physically meaningful systematic is a shift of the
scattered-electron **polar** angle, `θ_e → θ_e ± δθ`. This
propagates into `Q^2`, `y`, `z`, `pT_h`, and `phi_h` via the
event-by-event kinematic reconstruction. The proper implementation
re-runs the kinematic reconstruction with shifted `θ_e` and
re-bins. For high-Q2, where the scattered electron lands in the LAr
(not SpaCal), use the LAr angular-resolution number instead and shift
the LAr `θ_e`.

### 5.2 Cut-scan systematic: do not vary the data

**What went wrong.** For a cut-variation systematic (e.g., vary the
vertex z cut), the initial code re-ran the full chain and computed

    M_varied = h_num_varied / acc_varied
             = h_num_varied / (h_num_varied / h_den_varied)
             = h_den_varied

This **cancels the variation in the numerator** and reduces to just
the denominator. The systematic was meaningless.

**What to do.** For multiplicities, the correct cut-scan is

    M_varied = h_num_NOMINAL / acc_varied

i.e. hold the data numerator fixed at the nominal cut and divide by
the acceptance computed with the varied cut. The systematic is then
`|M_varied - M_nominal|`. Equivalently, vary the cut **only in the
acceptance computation** (MC reco and gen consistently), never in
the data.

### 5.3 Tracking efficiency

H1 standard tracking-efficiency uncertainty is **0.5% per track**,
flat. For multiplicities of any single charge it propagates directly.
For the charge asymmetry

    A_h = (N+ - N-) / (N+ + N-)

the flat 0.5% **cancels** (it multiplies N+ and N- equally). The
charge-dependent component (±0.2% asymmetric between + and -) does
**not** cancel and is the residual tracking systematic on A_h.

---

## 6. Data Streaming and Batch

### 6.1 ROOT 100 GB TTree limit

Do **not** `hadd` per-run ROOT files when the resulting TTree exceeds
~100 GB. ROOT silently corrupts or truncates around this limit. The
low-Q2 merged data tree is well above this.

**What to do.** Use `uproot` per-file iteration. Open each file,
process, close, move on. Never materialize the full merged tree in
memory or in a single output file. For high-Q2 the per-file pattern is
the same; the high-Q2 sample is smaller so this is less acute but the
discipline should be kept.

### 6.2 Memory management

Call `gc.collect()` after each file when streaming hundreds of files
on NAF. The login node has ~7.5 GB of shared RAM and uproot's array
cache will grow without explicit collection. Long-running streaming
scripts that look "fine" on the first 50 files OOM on file 300
otherwise.

### 6.3 Condor settings

Always use Condor for production. Never run on the login node.
Standard job:

    +RequestRuntime = 10800     # 3 h
    RequestMemory   = 4000      # MB
    RequestCpus     = 1

Memory-intensive (full systematics pass with all variations held in
memory): `RequestMemory = 12000`.

### 6.4 H1OO environment on batch

The default batch environment does not have the `desygroupset h1`
function. Source the group-set script before `thish1.sh`:

    source /etc/profile.d/02-desygroupset.sh
    source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh

Compile with `make bin`, not `make all`. (This is general H1OO
guidance, also in `AGENTS.md`.)

---

## 7. NonRad MC for C_rad

NonRad DSTs exist for both generators at:

    /data/dust/group/h1/britzger/norad_files/gc_out/

Steering files for processing them are in `H1BaryonAsym/steering/crad/`
(reused from the Lambda/proton analyses). The SIDIS v5b builder reads
HAT/MODS identically for Rad and NonRad input — no builder changes are
needed to produce a NonRad v5b tree.

NonRad output structure:

    ChargedTotGen : populated (generator-level tracks)
    ChargedTot    : 0 entries (no detector simulation in NonRad)
    DisTot        : populated

The radiative correction per kinematic bin is

    C_rad(bin) = N_gen_rad(bin) / N_gen_nonrad(bin)

computed from `ChargedTotGen` in both samples. Apply

    N_corr_rad-removed = N_corr / C_rad

after acceptance correction. For high-Q2, repeat with the same
RAPGAP/DJANGOH NonRad files; the high-Q2 cut on `Q^2 > 100` (or
whichever threshold) is applied on the gen-level kinematics on both
sides identically.

---

## 8. Response Matrix Properties (phi_h, 12x12 per 4D cell)

Properties measured on low-Q2 nominal RAPGAP response:

    Diagonal purity       : 91%
    Condition number      : median 2.2, max ~1900 (sparse edge cells)
    phi_h resolution      : 0.104 rad
    phi_h bin width       : 0.524 rad  (12 bins over 0-2π)

Resolution is ~5x smaller than the bin width → migration is mostly
into nearest neighbours → BBB is justified.

Matrix inversion and SVD unfolding produce pulls ~ 10^14 on sparse
edge cells (where some bins have <10 entries). They are numerically
unusable. Only BBB and IBU are stable; IBU is biased per section 4.2.
**Use BBB.**

For high-Q2, re-measure these numbers with the high-Q2 response. If
purity drops below ~80% or the condition number worsens
significantly, revisit the choice (but the geometry is similar —
expect BBB to remain the right answer).

---

## 9. Key Numbers Reference

| Quantity                          | Value             |
|-----------------------------------|-------------------|
| Data luminosity (2005-07 e+p)     | the superseded pre-rescale luminosity nb^-1   |
| RAPGAP31 rad MC luminosity        | 5,293,265 nb^-1   |
| DJANGOH14 rad MC luminosity       | 4,108,957 nb^-1   |
| Data files                        | 577               |
| RAPGAP MC files                   | 34                |
| DJANGOH MC files                  | 90                |
| Data tracks (BASE_CUT)            | 77.3 M            |
| RAPGAP reco selected              | 311 M             |
| DJANGOH reco selected             | 336 M             |
| 4D acceptance median              | 0.82              |
| 4D purity median                  | 0.52              |
| BBB closure bias on A1            | 0.002 - 0.064     |
| Model systematic on A1            | 0.001 - 0.005     |

These are the low-Q2 reference values. Reproduce the equivalent table
for high-Q2 at the start of the analysis and pin it in the high-Q2
context handoff.

---

## Summary checklist for high-Q2 startup

1. Use **data** `N_DIS` in the multiplicity denominator. Never MC.
2. Treat acceptance as a pure ratio — no luminosity rescaling.
3. Read `GenElecPhi` from v5b identically in numerator and denominator,
   strict (raise on non-finite). No silent fallbacks.
4. Hard-code `Ee = 27.6`, `Ep = 920.0` on both sides of acceptance.
5. Closure must use real data or a different MC, never the same MC.
6. Use **BBB** for phi_h. IBU only as a cross-check.
7. Allow `|A1| > 1` and publish the 12-bin distribution as primary.
8. SpaCal/LAr angle systematic = polar-angle shift, not phi rotation.
9. Cut-scan systematic varies the cut in **acceptance only**, with
   nominal data numerator.
10. Stream ROOT with uproot per-file + `gc.collect()`. Never `hadd`
    past ~100 GB.
11. Use Condor with `+RequestRuntime = 10800`, `RequestMemory = 4000`
    (12000 for systematics).
12. Source `02-desygroupset.sh` before `thish1.sh` in batch jobs.
13. Reuse `H1BaryonAsym/steering/crad/` NonRad steering files for the
    high-Q2 C_rad computation.
