# Per-bin charge-misID rank-1 covariance term for A_h — 2026-10-11

**Status:** DERIVATION COMPLETE (P1 item from the promotion plan; dispatched against the
cleared methodology `symmetrisation_tierB_covariance_methodology_2026-10-10.md` rev 1 §5,
which makes a per-bin misID term **MANDATORY for any A_h quote**). Derived from EXISTING
study artifacts only — no new production, no central/response/budget product touched.
Machine-readable term:
`data/systematics/charge_misid_ah_rank1_term_2026-10-11/` (CSV + JSON, this workspace).

**BASIS FLAG (mandatory on every quote):** **PRE-S2'-BASIS.** The source study
(`docs/unfolding/charge_misid_response_study_2026-10-08.md`, A-F3) was performed on the
then-canonical coherent-prior raw release
`bbb_ibu_coherentprior_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-06/`
(coherent `body+miss` prior, IBU `k=4`, `L_data=351000.0 nb^-1`). That release was
superseded as physics basis by the S2' detint-fake promotion (user decision 2026-10-10;
current canonical raw release `bbb_ibu_detints2prime_..._2026-10-09/`, Born release
`born_crad_dj14relbin_detints2prime_..._2026-10-10/`, canonical budget the 15-component
`systematics_detints2prime_15comp_..._2026-10-10/`). Quoting the values below against
S2'-basis values without this flag is FORBIDDEN (STATE.md observable-basis rule).

---

## 1. Per-bin availability (STOP-condition check)

The task's stop condition did **not** trigger. The study's Method-1 artifact
`H1ChargedSidis_HighQ2/output_v6/studies/charge_misid_response_2026-10-08/merged/unfolding_bias_per_bin.csv`
contains, per (charge, bin_index) — 490 rows = 245 matched h+/h− pairs on the 245-bin A_h
grid — **both** `yield_central` (the release IBU `yield_unfolded`) **and**
`yield_identity_consistent` (the identity-consistent-response unfolded yield), plus the
signed `bias = yield_central − yield_identity_consistent` and `rel_bias_pct`. No bias had
to be reconstructed from assumptions. Verified identities (this derivation):

| Check | Result |
|---|---|
| `bias == y_central − y_identity` | exact (max dev `0.0`) |
| `rel_bias_pct == 100·bias/y_identity` | max dev `8.9e-16` (convention: relative to the **ideal** yield, matching `integrated_shift_pct = 100·(y_c−y_id)/y_id`) |
| `yield_central` vs release `yield_unfolded` | max dev `2.4e-09` (float printing) |
| median rel_bias h+ / h− | `0.046446%` / `0.050722%` — matches `bias_summary.json` and the study report exactly |
| integrated sums and shifts | match `bias_summary.json` exactly (h+ `+0.335466%`, h− `−0.343725%` relative to ideal) |

## 2. Per-bin A_h bias vector

Definitions (RAW level, study basis; units cancel in the ratio):

- `A_central,i = (y+_c,i − y−_c,i)/(y+_c,i + y−_c,i)`
- `A_ideal,i  = (y+_id,i − y−_id,i)/(y+_id,i + y−_id,i)`
- **`delta_A_i = A_ideal,i − A_central,i`** — the signed **correction** to apply to the
  central per-bin A_h quote. The **bias of the central quote is `−delta_A_i`** (the central
  chain overstates A_h where `delta_A_i < 0`).

Census (245 bins, all finite; 0 bins with denominator ≤ 0, min central denominator 16.19;
all |A| < 1, range [−0.679, +0.706]):

| Quantity | Value |
|---|---|
| sign structure | **154 negative / 91 positive / 0 zero** |
| median \|delta_A\| | `4.547e-04` |
| mean \|delta_A\| | `1.533e-03` |
| max \|delta_A\| | `1.620e-02` (bin 244, `S3_x0`; A_c `+0.7063` → A_id `+0.6901`) |

Largest bins: 244 `S3_x0` `−0.01620`; 239 `S3_x0` `+0.01375`; 177 `S2_x1` `−0.01119`;
183 `S2_x1` `−0.01033`; 209 `S2_x1` `+0.01019`.

The mixed per-bin sign structure (63% negative) is a **real feature of the deterministic
IBU-propagated bias** (IBU smoothing redistributes the inter-charge migration), not noise:
there is no replica battery for this term, so **no noise-gating applies** and the signed
vector is carried as computed. The integrated bias remains negative (central overstates
the integrated asymmetry by ~36%).

## 3. Rank-1 covariance construction — SELECTED variant

**SELECTED: signed rank-1, `C = v v^T` with `v_i = delta_A_i` (RAW level), 100%
bin-correlated.** Selected by the methodology's own conventions:

- methodology §5 (rev 1): the misID term "enters `A_h` at full amplitude … **as a
  covariance it is rank-1 from the signed anti-symmetric per-bin vector**"
  (construction-wise the §4.4 branch-(c) anti-correlated cross-charge form with a
  documented origin, projected onto the A_h grid);
- house style (c_rad components): rank-1 is the **only admissible multi-bin form** for a
  single coherent nuisance; the diagonal is strictly single-bin bookkeeping.

**Diagonal variant** `diag(delta_A_i^2)`: provided for comparison only / strictly
single-bin use. It must NOT be used for any multi-bin or integrated A_h assembly — with
it, the integrated misID uncertainty would collapse toward the incoherent sum and fail
the reproduction check below.

**490-bin map equivalent (for full-map assemblies):** with the per-charge signed
absolute-yield bias vectors `b±` (columns `bias_pos_abs`/`bias_neg_abs` in the CSV;
`bias_neg = −bias_pos` identically in Method 1 by construction, integrated h+ `+0.335%` /
h− `−0.344%` relative to ideal), the branch-(c) form is
`C = [[b+ b+^T, −1·b+ b−^T], [−1·b− b+^T, b− b−^T]]` (cross-charge coupling
`ρ_cc = −1`, anti-correlated by construction). Push-through with the A_h Jacobian is
first-order equivalent to the A_h-grid `v v^T`; `v` here is the exact varied-yield bias,
not the linearization.

**Integration consistency (validation of the rank-1 form):** with yield weights
`a_i = (y+_i + y−_i)/Σ(y+ + y−)`, the rank-1 term predicts
`Var(A_int) = (Σ a_i v_i)^2`. Computed: `Σ a_i v_i = −0.003395660` vs the exact
integrated correction `−0.003395678` — agreement to `1.9e-08`. The rank-1 vector
therefore carries the study's full integrated impact coherently:
`σ_misID(A_int) = 0.003396` absolute, ~36% of the (pre-S2'-basis) integrated `A`.

## 4. Reproduction check of the study's integrated ΔA

The study defines the integrated asymmetry from **integrated yields**
(ratio of sums, not an average of per-bin values). Reproduced exactly from the per-bin
CSV:

| Quantity | This derivation | Study report | Deviation |
|---|---|---|---|
| A_central (integrated, RAW) | `0.01292196` | `0.012922` | rounding |
| A_identity (integrated, RAW) | `0.00952628` | `0.009526` | rounding |
| **ΔA (integrated, RAW)** | **`−0.00339568`** | **`−0.0034`** | `+4.3e-06` (report's 4-dp rounding only) |

No discrepancy beyond the report's own rounding. The per-bin vector is the exact
generator of the reported integrated number.

## 5. Born-level secondary columns

The CSV additionally carries Born-level columns, constructed as
`σ_Born,±,i = C_rad,±,i · y_raw,±,i` with per-bin per-charge
`C_rad,±,i = Born_nb/raw_nb` taken from the 2026-10-09 releasebin Born release
(`born_crad_dj14relbin_..._2026-10-09/`, pre-S2'), applying the study's §9 statement that
C_rad is common to central and corrected yields. This closes the gap that the study's own
§9 Born quotes referenced the then-current but now-superseded xFcut Born release.
Census: median |delta_A_born| `4.544e-04`, max `1.631e-02` (bin 244), same sign
structure (154/91); integrated Born `A_c 0.012924 → A_id 0.009553`, ΔA_born
`−0.003371`. The **RAW-level vector is the primary term** (the study's basis); the
Born-level difference is small because per-charge C_rad factors are nearly equal per bin.
C_rad remains PROVISIONAL (P-CRAD-LUMI); the C_rad tables are unchanged under S2', so the
per-bin factors remain the current applied values.

## 6. Basis supersession (S2') and re-derivation requirement

The S2' detint-fake promotion (2026-10-10) changed the response fake-fraction bookkeeping
(enlarged per-reco-bin `f_i`; pre-S2' data-weighted scalars `10.7141%`/`10.4019%`
superseded by `11.3577%`/`10.9768%`) and moved the nominal yields (~`−0.3%` integrated).
It did **not** target the A-F3 mechanism (GEN-charge routing of matched response rows),
and the track-level misID rates (`0.3368%` h+→h−, `0.3353%` h−→h+) are unchanged in kind.
Nevertheless the IBU-propagated per-bin biases would shift at the S2' yield-shift level,
so **a final A_h quote on the S2' chain requires re-running the identity-consistent
response + unfolding study on the variant chain** (new production; not performed here per
the task's no-new-production constraint). Until then this term is the only per-bin misID
artifact in existence and is valid **on its stated pre-S2' basis**.

## 7. Mandatory A_h caveat language

Any use of this term must carry, verbatim or equivalent:

1. "The charge-misID unfolding bias is included as a per-bin rank-1 systematic
   (`C = v v^T`, 100% bin-correlated, signed per-bin vector from the A-F3 study);
   integrated it contributes `±0.003396` absolute on A_h (~36% of the integrated
   asymmetry, pre-S2'-basis)."
2. "It is MC-based (RAPGAP31 charge-confusion model, unvalidated in data) and scales
   ~linearly with the true misID rate; the rank-1 bin-correlation is a conservative
   envelope assumption, not a measured correlation."
3. "It is A_h-only: the per-bin cross-section impact is negligible (0/490 bins exceed the
   per-bin budget) and no cross-section budget component is added."
4. "This term does not by itself make A_h quotable" — the A_h product remains gated on
   charge-dependent tracking (failed 2.5%-on-A_h sanity), the h+/h− statistical
   cross-block (zero-by-assumption), the fake-fraction charge asymmetry (now the S2'
   per-reco-bin `f_i`, charge-asymmetric), trigger/NucliaWeight common-mode
   demonstration, and P-ALPHA.
5. Every quote states prior basis (`body+miss`), level (RAW primary / Born secondary),
   and the PRE-S2'-BASIS flag until re-derived on the S2' chain.

## 8. Artifacts and provenance

- `data/systematics/charge_misid_ah_rank1_term_2026-10-11/charge_misid_ah_bias_per_bin_2026-10-11.csv`
  — 245 rows: bin_index, cell_label, z/pT edges, central + ideal yields (both charges),
  per-charge absolute/relative biases, `A_central_raw`, `A_ideal_raw`, **`delta_A_raw`**
  (the vector `v`), per-charge C_rad, Born-level A columns and `delta_A_born`.
- `data/systematics/charge_misid_ah_rank1_term_2026-10-11/charge_misid_ah_rank1_term_2026-10-11.json`
  — construction conventions, selected variant, validation, censuses, reproduction
  numbers, caveats, provenance with md5s.
- Inputs (read-only): study `merged/unfolding_bias_per_bin.csv` (md5 `87931b32…`),
  `merged/bias_summary.json` (md5 `d2a02ff0…`), pre-S2' raw release `ibu/cross_sections.csv`
  (md5 `97a657e0…`), 2026-10-09 releasebin Born release `ibu/cross_sections.csv`
  (md5 `79095f48…`); full paths in the JSON.
- Governing docs: methodology rev 1 (§4.4, §5), gap audit §7, study report, STATE.md
  (S2' promotion addendum, 2026-10-11).

## 9. Ledger disposition

Not a 14th/16th budget component: the cross-section impact is subdominant (0/490 bins),
so the term is registered as a **mandatory A_h-only covariance term** alongside the
budget, per the methodology. It satisfies the methodology §5 requirement mechanically on
the study (pre-S2') basis; it does **not** unblock any A_h gate (charge-dependent
tracking, stat cross-block, fake charge asymmetry, P-ALPHA remain), and it must be
re-derived on the S2' chain before any final A_h quote.
