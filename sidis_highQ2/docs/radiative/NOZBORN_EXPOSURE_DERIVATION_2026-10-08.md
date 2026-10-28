# NOZ photon-only Born e- MC exposure — L_MC derivation for the NoZ C_gamma_born numerator

**Date:** 2026-10-08 · **Authority:** h1-normalization ·
**Machine-readable record:** `sidis_highQ2/data/radiative/nozborn_exposure_2026-10-08/nozborn_exposure_2026-10-08.json`

## Verdict

> **L_MC(NOZ, e-, photon-only pure Born) = 14,032,218 nb⁻¹** (rounded 14,032,000)
> ± 36,500 nb⁻¹ (1σ, 4-series anchor sampling; conservative envelope ±38,000 = 0.27%)
>
> **Formula:** `L_MC[nb⁻¹] = 1000 × Σ(weight1·weight2) / σ_Born`, with
> **σ_Born = 6708.559 pb** (oomclumi `SigmaBorn` from the NOZ hat run headers) and
> **Σw ≈ 94,135,964** (estimate; exact 260-chunk tree-sum pending on Condor).
>
> **Label:** DIAGNOSTIC_MANIFEST_LEVEL **+ SAMPLED_ESTIMATE** — replace with the exact
> weighted closure before any PASS verdict. Queue value to report: **14032218**.

## 1. Consumer and interface

- Target: the `PENDING` row of
  `H1ChargedSidis_HighQ2/studies/radiative_correction_genonly_v1/output_v3/nozborn_hist_xfcut_20261007/finalize_queue.tsv`
  (not edited by this task) via `run_finalize_nozborn.sh NOZBORN <L_nb>` (not edited).
- Fork: `crad_v3_finalize_xfcut_nozborn.py`; the exposure enters only as
  `snum = num["sumw_y"]/L`, i.e. the numerator is the **weighted**
  σ_proxy = Σ(weight1·weight2)/L_MC — the same convention as every existing leg.
- G4 (identity) is exposure-independent; G6 (range) needs this L.

## 2. Sample and integrity

- Sample: `DJANGOH14.NC.EMINUSP.NOZ_NORAD.Q2GT100.CTEQ6L` (series A–D × 65 chunks),
  `/data/dust/group/h1/britzger/norad_prod21/dst2all_djangoh14_NoZ/`.
  Photon-only PURE Born (NoZ neu exe, GEN header `'LPIN' 2 0 0` ACTIVE; LPIN(7)=2 inert
  at Born; LPIN(4/8/9/11)=0; INT3/ISM3 off).
- Complete on disk: 260/260 hat files, no dead stubs; dCache GEN crosscheck 260 files,
  206.6 GB. Event totals exact: 10,000,001 per series → **N_total = 40,000,004**
  (each dst2all log prints the ODS close line twice: 80,000,008/2).
- Per-chunk counts vary 81.8k–155.2k (avg 153,846). The "154,895 events/chunk" figure in
  earlier notes is one chunk (D.C0000044), not a constant.

## 3. σ authority

- **σ_Born = 6708.559 pb**, read at FILE level by oomclumi (`SigmaBorn`) from the hat run
  headers; identical on A.C0000000 and D.C0000000 → σ constant across chunks.
- Crosschecks: `log.Dj.NoZ.2Feb23.txt` SIGTOT 6.7086 nb (same exe banner 10223 1316;
  rel 6.1×10⁻⁶); fork `SIGTOT_REF_NB=6.7086` (±5% tolerance); NOZ1's 7.2859 nb correctly
  fails the Born band, so sample mixup is excluded by σ alone.

## 4. Weight structure — MCWE verdict

- weight2 ≡ 1.0; weight1 event-dependent (min 1.0, max 129.008, 42.26% exactly 1.0 on A.0).
- **MCWE 1 150 is ACTIVE** (Q2WG E2start=150 flattening, GKI word 20):
  Σw/N = 2.353399 ≠ 1 → **weighted normalization REQUIRED; count-based normalization is
  invalid for this sample.** (2021 LOGOUTPUT: per series `sum(wtx1) = 10,000,000.00000`
  exactly = N_written; N_gen ≈ 23.13M/series before cuts and weighting.)

## 5. Anchor measurements (hat `HAT` tree, `Weight1·Weight2`, single-file reads)

| chunk | N | Σw | mean w | std w |
|---|---|---|---|---|
| A.C0000000 | 154,870 | 364,081.656 | 2.350886 | 4.820154 |
| A.C0000032 | 155,070 | 364,421.799 | 2.350047 | — (drift probe: 0.036%) |
| B.C0000000 | 154,862 | 366,682.545 | 2.367802 | — |
| C.C0000000 | 154,843 | 363,522.202 | 2.347683 | — |
| D.C0000000 | 154,716 | 363,153.316 | 2.347225 | — |

- 4-series pooled mean weight: **2.3533989** → Σw_est = 2.3533989 × 40,000,004 ≈ **94,135,964**.
- oomclumi spot agreement (Lumi = Σw_chunk/σ): A.0 → 54.2712 pb⁻¹ (prints 54.271),
  D.0 → 54.1328 pb⁻¹ (prints 54.133); matches uproot Σw at ~3×10⁻⁶.
- Uncertainty: SE(mean_w)/chunk = 4.8202/√154870 = 0.012248 (0.521%); pooled over 4
  independent series anchors → 0.006124 (**0.2602%**); anchor spread consistent with
  statistics (χ²/dof = 0.63, dof 3). SE(Σw) ≈ 245,000 → SE(L) ≈ **36,500 nb⁻¹**;
  envelope **±38,000 (0.27%)**.

## 6. Method reconciliation (all three candidate routes)

1. **Count-based N/σ — REJECTED.** L = 1000×40,000,004/6708.559 = **5,962,533 nb⁻¹**.
   With it, σ_proxy is inflated by the mean weight 2.353× → integrated C ≈ 2.46,
   far outside the G6 NOZBORN band [0.95, 1.12]. (This is exactly the fork's
   count-based provenance formula — see §8.)
2. **Britzger GenLumi fLumi route — NOT AVAILABLE for the primary NOZ sample.** No
   GenLumi steer references `dst2all_djangoh14_NoZ` (NOZ_NORAD files).
   `es.Norad.em.NoZ.GenLumi.steer` fLumi = 12964 pb⁻¹ points at
   `dst2all_djangoh14_NoZ_new` (NOZ1_NORAD) — it is the **NOZ1** exposure, not NOZ.
   fLumi semantics: **fLumi[pb⁻¹] = Σw/σ** (weighted), verified on NORAD em at 1.1 ppm
   (steer 12841.587 vs tree-sum 12841.5724 pb⁻¹). Caveat: NORAD **ep** steer
   (12970.73 pb⁻¹) is 0.39% ABOVE its tree-sum (12920.116) — ep steers are unreliable.
3. **"fLumi × σ ≈ 94M" explained.** fLumi×σ = Σ(weight1·weight2) — the **weighted**
   event sum, not N_generated (N_gen ≈ 92.53M total, 1.6% below Σw). ~94M is universal
   across the Britzger e- samples because each is a 40M-event production flattened by
   the common MCWE 1 150 (mean weight ≈ 2.35):

| sample | Σw | σ (pb) | L (nb⁻¹) | basis |
|---|---|---|---|---|
| NORAD em 2021 (nonrad_old) | 94,050,906.07 | 7323.94 | 12,841,572.44 | exact tree-sum |
| **NOZ (primary)** | **94,135,964 ± 245k** | **6708.559** | **14,032,218 ± 36.5k** | **4-anchor estimate (this record)** |
| NOZ1 | 94,454,835 (implied) | 7285.933 | 12,964,000 | fLumi 12964 (steer verified) |
| BORN em | 94,241,601 (implied) | 6750.831 | 13,960,000 | fLumi 13960 (steer verified) |

All four are mutually consistent: same Σw ≈ 94M, L differs exactly as 1/σ.

## 7. How the existing exposures were formed (consistency of the convention)

- **RAPGAP31 NONRAD em 13,416,991.0 nb⁻¹**: `L = 1000 × Σ_chunks oomclumi Lumi[pb⁻¹]`,
  503/520 chunks; oomclumi Lumi = Σw/SIGMT (weighted identity Σw = SIGMT[pb]·Lumi[pb⁻¹])
  — `CRAD_FINAL_PROVENANCE_2026-09-30.md`. (RAPGAP RAD similarly: em 14,461,067.0 /
  ep 17,536,474.0.)
- **DJANGOH14 RAD (fork constants ep 16,206,393.0 / em 13,534,017.0)**: oomclumi
  per-chunk weighted sums (642 em / 731 ep chunks, Σw identity PASS) —
  `rad_oomclumi_exact_v2_20260924.json`.
- **DJANGOH14 nonrad_old em 12,841,572.442 nb⁻¹** = 1000 × Σ(w1·w2 tree-sum, 260 files,
  Σw = 94,050,906.068) / 7323.94 pb — `nonrad_weighted_exposure_2026-09-12.json`.
- **nonrad_new (fork constants ep 1,717,376.821 / em 1,699,702.771)**: weighted
  supplement exposures (fork constants authoritative).
- **Every leg is weighted; none is count-based.** The NOZ L_MC is formed by the identical
  convention → directly compatible with the beam-mixed denominator. All legs carry
  DIAGNOSTIC_MANIFEST_LEVEL; NOZ additionally SAMPLED_ESTIMATE until exact closure.

## 8. Finalize validation checks

1. **Closure identity (correct form):** `L × σ_Born/1000 = Σw_total ≈ 94,135,964`.
   The check `L × σ = N_total` is **wrong** for this convention (fails by 2.353×).
2. **G6 integrated:** expected ≈ 1.046 (= 1.14185 × 0.91598) per charge, band [0.95, 1.12].
   ±1% L error → [1.036, 1.057] (inside); count-based L → ≈ 2.46 (excluded). The 0.26%
   sampling uncertainty cannot flip G6.
3. **G6 per-bin absolute:** [0.4, 1.8].
4. **G4 identity:** exposure-independent (rel 1e-9), already smoke-verified (2.2e-16).
5. **oomclumi spot agreements:** §5 (A.0 / D.0).
6. **Exact closure before PASS (Condor, multi-file → not login node):** 260-chunk
   weighted tree-sum of weight1·weight2, or oomclumi sweep with
   `L = 1000 × Σ per-chunk Lumi[pb⁻¹]`. Expected within ±0.5% of 14,032,218; the exact
   value replaces this estimate and drops the SAMPLED_ESTIMATE label.
7. **Fork provenance-schema incompatibility:** the fork's provenance block computes
   `L_calc = n_events/(sigma_pb/1000)` — count-based. Passing
   `{n_events: 40000004, sigma_pb: 6708.559}` would record
   `lumi_from_events_nb = 5,962,533` and `rel_diff_vs_arg ≈ 0.575` (recorded, NOT gated,
   but misleading). **Do not pass a count-based provenance JSON**; the schema should be
   `{sumw, sigma_pb, authority}` with `L_calc = sumw/(sigma_pb/1000)`.

## 9. Crosscheck analogues (not campaign legs)

- **NOZ1** (`dst2all_djangoh14_NoZ_new`, photon-only corrected, σ 7285.933 pb):
  **L_full = 12,964,000 nb⁻¹** (= 1000 × fLumi 12964). Implied mean weight 2.361371 vs
  measured A.0 mean 2.38093 (0.83%, ~1.6σ of single-chunk SE). Caveat: B.C0000060 is a
  dead stub (1406 B, no trees) → 259/260 usable, N = 39,844,970, on-disk exposure
  ≈ 12,913,800 nb⁻¹. G6_NOZ1_CROSSCHECK band [1.06, 1.21], expected ≈ 1.136.
- **BORN em** (`dst2all_djangoh14_BORN_NORAD` EMINUSP, full-EW Born, σ 6750.831 pb):
  **L_full = 13,960,000 nb⁻¹** (= 1000 × fLumi 13960). Implied mean weight 2.356040 vs
  measured A.0 mean 2.36036 (0.18%). Caveat: em coverage 259/260 over all four series
  (missing em B.C0000047; A.C0000031 half-size, 81,829 events), N_ondisk = 39,771,949,
  on-disk exposure ≈ 13,880,400 nb⁻¹.

## 10. Provenance and read-only statement

Inputs read: the two NOZ audit docs (2026-10-07), the campaign README/wrapper/queue, the
finalize fork, `CRAD_FINAL_PROVENANCE_2026-09-30.md`, the nonrad weighted-exposure and RAD
oomclumi JSONs, the NOZ/NOZ1/BORN/NORAD directories (hat inventories, fpack steers,
dst2all logs, 2021 LOGOUTPUT), and the two GenLumi steers. All access under
`/data/dust/group/h1/britzger/` was strictly read-only (single-file hat reads and
directory/log/steer inspections on the login node within the 5-minute single-file
policy; no multi-file processing outside Condor). Nothing was written under the britzger
trees; `finalize_queue.tsv` and `run_finalize_nozborn.sh` were not edited.
