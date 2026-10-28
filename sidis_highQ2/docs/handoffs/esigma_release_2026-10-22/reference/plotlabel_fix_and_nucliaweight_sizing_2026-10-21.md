# Env100 plot-label fix + NucliaWeight primary-loss sizing (2026-10-21)

Neither item is a quoting blocker. Per remote STATE.md, quoting is blocked on M1 (env100 builder reconstruction),
N2 (HFS-Empz channel), plotter band re-pointing and the analysis-note update.

## 1. Env100 plot labels — FIXED (additive)

- New edition: NAF `sidis_highQ2/plots/systematics/bgsub_env100_quoted_total_21comp_7col_fixedy_labelfix_2026-10-21/`
  (producer, 3 PDFs × 2 pages, PNGs, manifest, README). The 2026-10-19 package is retained untouched.
- The labels are now read from the binmap via `bin_rows()`, the same source as the v5 overlay. S1 is 300–700, S2 700–1500
  and S3 1500–20000; the x edges are 0.0021/0.0071/0.03, 0.0042/0.016/0.069, 0.0099/0.034/0.15 and 0.021–1.
  The S3 upper edge is drawn as the physical cap 1; the binmap's overflow edge is 1.5, and the v5 overlay prints 1.5.
- Data unchanged: a pixel diff against the 2026-10-19 PNGs gives 0 changed pixels outside the label boxes and title (6/6 pages).
- The binning never changed: every `output_v6` binmap and the canonical cross-section plotter agree.
  The wrong table is a copy-paste lineage going back to 2026-09-24, found in about 14 superseded systematic producers plus
  the v3/v4 overlay scripts. Those were not re-rendered; they are listed in the new README.
- Canonical text edited (sha-guarded, re-read immediately before writing): the caveat bullet in the STATE.md latest
  override, RUN_STATUS.md, and line 7 of the plotting skill.

## 2. NucliaWeight primary-loss column — SIZED, NOT INSTALLED (needs user decisions)

- `H1NuclIACor::GetWeight = 1 − ani_ineff` (K. Daum, `H1PhysUtils/ani_ineff.f`): "inefficiency missing in MC w.r.t. data",
  from the ρ analysis shown at the tracking meeting on 2005-11-16. **The routine documents no uncertainty**, so any κ is the
  analysis's own choice. Pions use `nucintfit(ecm)`, kaons an exponential in p; protons get no correction (NW = 1).
- The register's "NucliaWeight | existing per-event-weight envelope" row is a phantom: `emit_source_NucliaWeight.py` and
  `source_NucliaWeight.json` do not exist anywhere under the scripts or `output_v6`.
- One-file census (DJ14 7072 em06_0000, 108k body tracks, v5 body definition), script `scripts/nucliaweight/nw_census.py`:
  - Mean NW 0.98989. Deficit d = Σw(1−NW)/ΣwNW = **1.02%**, with h+ 1.024% / h− 1.019% (charge-symmetric).
  - By species: π 1.12% (84% of the body), K 0.69% (11%), p 0 (5%, uncorrected).
  - By kinematics: largest at lab pT 0.15–0.3 GeV and low z (2.5%); 0.5–0.8% for pT > 0.5 GeV. Larger in the backward and
    forward η quarters (1.4% / 1.2%).
  - At κ = 100%, the first-order per-bin term is about 0.5–2.5%, median ≈ 0.8%, coherent ≈ ±1.0%. It cancels in h+/h− and
    A_h (≈ 0.005%). Its effect on the quoted median total is small, roughly +0.04 pp at κ = 1 and +0.01 pp at κ = 0.5.
- Proposed construction: a single Condor pass booking the derivative response D_ij = Σ w1w2(1−NW) on the body legs.
  Then R(κ) = R_nom − κD, followed by the lean IBU k=4 re-unfold. The result is linear in κ, so any κ follows from that one pass.
- Open decisions:
  1. κ: 100% fiat, as for env100 and int; 50%; or derived.
  2. Correlation with `nonprimary_norm_v5` int: independent (conservative, since the true coherent effect partly cancels)
     or one coherent interaction-rate nuisance.
  3. Overlap with `tracking_eff_m0p5_rel` (flat −0.5%/track, median 0.51%).
  4. Whether to also cover uncorrected (anti)proton absorption.
