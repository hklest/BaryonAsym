# Britzger's EW gamma-Z / NoZ (photon-exchange-only) method — review of `norad_prod21/EW_gZ_correction/`

**Date:** 2026-10-07
**Author:** h1-mc-expert (MC simulation physics)
**Source directories (all inspected READ-ONLY; nothing under `/data/dust/group/h1/britzger/` was written, modified, moved, or created):**
- `/data/dust/group/h1/britzger/norad_prod21/EW_gZ_correction/` (test campaign, Jan 30 – Feb 4, 2023; re-touched Apr 11, 2025)
- `/data/dust/group/h1/britzger/djangoh14_NoZ/` (2023 CVS checkout, modified source + `djangoh14test` binary)
- `/data/dust/group/h1/britzger/lcg-releases/build-4.1.1/djangoh14-ariadne10-noZ/` (2021 NoZ build, production executable)
- `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/djangoh14/` (stock reference, from previous investigation)
- `/data/dust/group/h1/britzger/norad_prod21/{djangoh14_ar4.10_noZ,djangoh14_ar4.10_noZ-2,dst2all_djangoh14_NoZ,dst2all_djangoh14_NoZ_new,dst2all_djangoh14_BORN_NORAD}/` (scaled-up campaign artifacts)
- `/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/` (dCache GEN files; still readable as of 2026-10-07)

**Context:** follow-up to `EMINUS_CRAD_INVESTIGATION_AND_BLOCKER_2026-10-07.md` (e− Born-level C_rad question) and to the previous finding that stock DJANGOH14 has no functional Z-off switch (`'ZMAS'` card route only; `LPAR(17)` vestigial; `LPIN` bounded 1–11 in `gdjcha.F`).

---

## 1. MECHANISM VERDICT: **code modification, not steering**

NoZ was implemented by **zeroing the six fermion–Z coupling constants** in the HERACLES part of DJANGOH14. There is **no steering-card route** (`LPIN` card index is bounded 1–11 in `gdjcha.F`; `gdjcha.F` is *unmodified* in all NoZ trees — confirmed by `diff -rq`).

### 1.1 The 2023 build (3-file mod, "DB, Jan 2023" comments)
Tree: `/data/dust/group/h1/britzger/djangoh14_NoZ/` (CVS checkout Jan 31, 2023; binary `djangoh14test` built Feb 1, 2023 13:16; `version.h`: `CPP_DATE 010223`, `CPP_TIME 1316`). Versus stock `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/djangoh14/djangoh_h/`, exactly **three** Fortran files differ (`hssetf.F`, `hssetp.F`, `hsstr1.F`):

- **`hssetp.F`** (Born-level couplings): after `C---No Z-exchange DB, Jan 2023`, the six Z-coupling assignments
  `VAFI(2,1,2)=-B`, `VAFI(2,2,2)=B`, `VAFI(2,3,2)=-B`, `VAFI(1,1,2)=B*(4*SW2-1)`, `VAFI(1,2,2)=B*(1-8*SW2/3)`, `VAFI(1,3,2)=B*(4*SW2/3-1)`
  are commented out and replaced by `VAFI(1or2, 1..3, 2)=0D0`. Photon couplings `VAFI(*,*,1)` untouched. → `HSSAB0` Born ME becomes pure γ² (the γ²+2γZ+Z² sum is computed unconditionally in stock code; killing the couplings kills the γZ and Z² terms at source).
- **`hssetf.F`** (1-loop/effective-coupling path): same zeroing of `VAFI1(*,*,2)`; additionally the Z **vertex form-factor** additions `VAFI1(IVA,LF,2) += RHO1*DREAL(HSFHFB(T,IVA,LF,2,...))` are commented out and replaced by `VAFI1(IVA,LF,2)=0D0`. The **photon** running-α couplings `VAFI1(1,*,1)=EFFQ*{1,-2/3,1/3}` and the photon vertex form factors `VAFI1(IVA,LF,1)+=EFFQ*DREAL(HSFHFB(T,IVA,LF,1,ME2))` are **kept** (one commented-out experiment line `C VAFI1(IVA,LF,1)=0D0` shows an intermediate iteration that had them zeroed — see §4.2).
- **`hsstr1.F`** (corrected-structure-function path, LPAR(2)=1): same zeroing of `VAFI1(*,*,2)` with originals commented.

### 1.2 The 2021 build (2-file mod — the *production* NoZ executable)
Tree: `/data/dust/group/h1/britzger/lcg-releases/build-4.1.1/djangoh14-ariadne10-noZ/` (`version.h`: `CPP_DATE 200421`, i.e. 20.04.2021; binary dated Apr 20, 2021). Versus stock `build-4.1.1/djangoh14-ariadne10/djangoh_h/`, exactly **two** files differ: `hssetp.F` and `hsstr1.F`, with the **identical** zeroing pattern (originals kept as comments, no "DB Jan 2023" tag). **`hssetf.F` is NOT modified** in the 2021 build — i.e. the 2021 build leaves the Z form-factor additions in `hssetf.F` live. At the production flag settings used (LPAR(12)=0, LPAR(15)=0, LPAR(16)=0 → all vertex/weak form-factor blocks gated off) this gap is inert, but at other flag settings the 2021 build would leak residual Z form factors. The 2023 3-file mod closes exactly this gap.

This executable is symlinked into the production work dirs as
`/data/dust/group/h1/britzger/norad_prod21/djangoh14_ar4.10_noZ/djangoh14-ar4.10-noZ -> ../../lcg-releases/build-4.1.1/djangoh14-ariadne10-noZ/djangoh14test`
(also in `djangoh14_ar4.10_noZ-2/`).

### 1.3 `LPAR(17)` confirmation
The `log.Dj.NoZ.*.txt` stdout (2023 NoZ exe) still prints `** GAMMA OR/AND Z LPAR(17) = 1 **` — confirming the previous finding that LPAR(17) is vestigial printout; the Z removal lives in the couplings, not in any LPAR flag.

---

## 2. Steering cards

| Card (all in `EW_gZ_correction/` unless noted) | Key active lines | Role |
|---|---|---|
| `steer_djangoh14` (Feb 4, 2023) | `EELE 27.6`, `LLEP -1`, `XMIN 1e-4`, `Q2MI 1000`, `WMIN 1.1646`, `INT2(1)=1`, `NPVE 10000`, `LPIN(4)=0`, `LPIN(7)=2`; LPIN(1)/(2)/(8)/(9)/(10)/(11) commented → defaults (G_μ scheme, Born+1-loop, weak SE off); no INT3/ISM3, no MCWE/RNIN; `LST(8)=0` (QCD off) | main EW-scan test card |
| `steer_djangoh14_Sergey` (Jan 31, 2023) | same but `LLEP +1`, `NPVE 50000`, **no active Q2MI** (→ Q2>4 default), `LPIN(8)=1, LPIN(9)=1, LPIN(11)=1` | Sergey Levonian-derived card (RTX comment: "S.Levonian 11.02.2021 … no QED radiation, Q2 > 4 GeV^2, No weighting") |
| `stdjh14_nc_eminusp_noZ_q2gt100_cteq6l_a_2Feb23` (Feb 4, 2023) | **= the stock NORAD production card `stdjan_nc_eminusp_norad_q2gt100_cteq6l_a` with only these changes:** `NEVENT 10000001→2000`, `RNIN 845990` removed, `LPIN(7)=2` made explicit (was commented default), `LPIN(8) 1→0`, `LPIN(9) 1→0`, `LPIN(11) 1→0`, output file names → `NOZ_NORAD`; keeps `MCWE 1 150.`; `Q2MI 100`, `LLEP -1` | production-style noZ card (Q2GT100, CTEQ6L) |
| `test/steer_djangoh14` (Apr 11, 2025) | `Q2MI 1000`, `LLEP -1`, `LPIN(4)=0`, `LPIN(7)=2` (= "normalversion.photonselfenergyON2" config) | 2025 re-validation |

Physics reading of the noZ production configuration: **non-radiative** (`INT2/ISM2` on, no `INT3/ISM3`), `LPIN(4)=0` (no leptonic QED), `LPIN(2)=1` default (Born + soft&virtual 1-loop), `LPIN(7)=2` (running α_em, parametrized), `LPIN(8/9/11)=0` (weak self-energies off), Q2>100 — i.e. exactly the NORAD sample definition, only with Z couplings zeroed in the executable.

---

## 3. Complete log-variation map (`EW_gZ_correction/`)

Chronology and executable identification via the stdout compile banner (`compiled at date DDMMLY`, from `dateqq.inc`/`version.h`) and via physics signatures (e+/e− asymmetry ⇒ Z present; σ sensitivity to `LPIN(7)` ⇒ running-α active):

| Date | Files | Executable | Meaning of name |
|---|---|---|---|
| Jan 30, 2023 | `log.e±.default.txt` + `djangoh14.log.default.e±` | normal, built 30.03.2020 ("300320 1231") | Sergey card, Q2>4, weak SE **on** (LPIN 8/9/11=1) |
| Jan 30 | `log.e±.noZ.txt` + `djangoh14.log.noZ.e±` | same normal exe | **"noZ" here = weak SE off only (LPIN 8/9/11=0) — Z still present!** |
| Jan 30 | `log.qq1000.{e+,e-}{,.noZ}.txt` | normal exe | "qq1000" = Q2MI=1000; no-suffix = weak SE on, ".noZ" = weak SE off |
| Jan 30 | `log.qq1000.born.noZ.e±.txt`, `log.qq1000.born2.noZ.e±.txt`, `log.qq100000.born.noZ.e±.txt` | normal exe | "born" = LPIN(2)=0 (+LPIN(7)=2); "born2" = LPIN(2)=0+LPIN(7)=0, NPVE=10000; "qq100000" = **same config as qq1000.born** (steering echo shows Q2MI=1000; no Q2MI=100000 run exists) |
| Jan 30 | `djangoh14.log.e+.Ee127GeV` | normal exe | `EELE=127.6` GeV beam-energy probe (note: LLEP=−1 despite the "e+" in the name) |
| Jan 31 | `djangoh14.log.NoZ.e±` | **first 2023 NoZ-code build** (broken, see §4.2; binary since overwritten by the Feb-1 rebuild) | first true photon-only runs: LPIN(2)=0, LPIN(7)=0, Q2MI=1000, NPVE=10000 |
| Feb 1 | `djangoh14.log.NoZ.LPIN11.1.e-` | first 2023 NoZ build | + LPIN(11)=1 (inert at Born) |
| Feb 1 | `djangoh14.log.NoZ.photonselfenergy{OFF,ON,ON1,ON2}` | first 2023 NoZ build | LPIN(7)=0/2/1/2 scan at LPIN(2)=1 — **all identical ⇒ running-α broken in this build** |
| Feb 1 | `djangoh14.log.normalversion.photonselfenergy{ON0,ON2}` | normal exe | control: LPIN(7)=0/2 at LPIN(2)=1 — running-α works |
| Feb 1 | `djangoh14.log.LPIN2.{0,1}.allCorr{On,Off}.e-`, `#djangoh14.log.LPIN2.0.onlyBorn.e-#` | normal exe (except onlyBorn → NoZ build) | LPIN(2)=0/1 × LPIN(7/8/9/11)=1/0 hierarchy scan ("allCorrOn" = LPIN(7)=1,8=1,9=1,11=1) |
| Feb 1 | `djangoh14.log.4-{0,1}.7-{0,2}` | normal exe | **naming = `4-LPIN(4).7-LPIN(7)`** (not kinematics): LPIN(4)=0/1 × LPIN(7)=0/2 |
| Feb 1 | `djangoh14.NoZ.log.4-0.7-{0,2}` | first 2023 NoZ build (broken) | LPIN(7)=2 inert ⇒ both 218.842 pb |
| Feb 1 | `djangoh14.NoZneu.log.4-0.7-{0,2}` | **rebuilt ("neu" = new) NoZ exe, Feb 1, 2023 13:16** | LPIN(7)=2 now active ⇒ 243.489 pb |
| Feb 4 | `log.Dj.NoZ.2Feb23.txt` | NoZ "neu" exe (banner "10223 1316") | Q2MI=100, LPIN(2)=0 (Born only), LPIN(7)=2 |
| Feb 4 | `log.Dj.NoZ.NoBORN.txt` | NoZ "neu" exe | "NoBORN" = LPIN(2) left at default 1 (corrections ON, *not* only-Born) |
| Feb 4 | `log.Dj.NoZ.NoBORN.LPIN8911{,eq0}.txt` | NoZ "neu" exe | ± LPIN(8/9/11)=1/0 (weak SE) |
| Apr 11, 2025 | `test/djangoh14.log`, `test/DJANGOH14.Q2G4.CTEQ5L.NORAD.ARI.C00`, `test/steer_djangoh14` | 2025-era exe (build-4.2.1 checkout date) | re-validation: σ = 263.05634 pb exactly reproduces the 2020/2021-exe full-EW corrected value at Q2>1000; C00 = one Sergey-card (Q2>4, CTEQ5L) GEN test file (43.9 MB) |

---

## 4. METHOD reconstruction and extracted numbers

### 4.1 What was measured
The campaign measures the **Z-exchange + γZ-interference contribution** to the NC DIS cross section, separately for e− and e+, by comparing the *full-EW* executable against the *photon-only (NoZ)* executable at otherwise identical settings, plus auxiliary scans (running α, weak self-energies, leptonic soft&virtual QED, Born vs corrected). σ is taken from the VEGAS-integrated `SIGTOT` (stdout, nb) or the LOGOUTPUT tail (pb); the generated event counts (999/9999) are irrelevant stubs.

### 4.2 The broken-running-α episode (important validation lesson)
In the **first 2023 NoZ build** the photon running-α (LPIN(7), "fermionic contributions to the photon self energy") was **inert**: `NoZ.photonselfenergyON/OFF/ON1/ON2` and `NoZ.log.4-0.7-2` all give σ = 218.84216 pb regardless of LPIN(7). The normal-exe control (`normalversion.photonselfenergyON0/ON2`: 237.675 vs 263.056 pb) proved the switch itself works. The cause is visible in the surviving source as commented-out experiments zeroing the *photon* couplings/form factors in `hssetf.F` (`C VAFI1(1,1,1)=1D0`, `C VAFI1(IVA,LF,1)=0D0`): an intermediate iteration had killed the photon form factors together with the Z ones. The **rebuilt "neu" executable (Feb 1, 2023 13:16)** restores `VAFI1(1,*,1)=EFFQ` and the photon vertex form factors (`NoZneu.log.4-0.7-2` = 243.489 pb ⇒ running-α active again), and is the binary preserved in `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh14test`. The Feb-4 `log.Dj.NoZ.*` runs validate it at Q2>100 immediately before the NOZ1 production.

### 4.3 Extracted cross sections (all values verified from the logs)

**Q² > 1000 GeV² (test cards: x>1e-4, W>1.1646, NPVE=50000 unless noted):**

| Configuration | exe | e− [pb] | e+ [pb] |
|---|---|---|---|
| Born (LPIN2=0), full-EW | normal | 237.68 | 217.75 |
| Born (LPIN2=0), **photon-only** (LPIN7=0) | NoZ | **218.842** | **218.842** (identical ⇒ charge symmetry ✓) |
| Born, LPIN7=0 vs 2 ("born" vs "born2") | normal | 237.68 ≡ 237.68 | 217.75 ≡ 217.75 (LPIN7 inert at Born ✓) |
| Corrected (LPIN2=1, LPIN7=2), full-EW, weak SE off (8/9/11=0) | normal | 263.06 | 241.98 |
| Corrected, full-EW, weak SE on (8/9/11=1) | normal | 261.96 | 240.89 |
| Corrected (LPIN2=1), photon-only, LPIN7=0 | NoZ neu | 218.842 | — |
| Corrected, photon-only, LPIN7=2 ("NoZneu 4-0.7-2") | NoZ neu | **243.489** | — |
| Corrected, full-EW + leptonic soft&virt QED (LPIN4=1, LPIN7=0/2) | normal | 120.711 / 133.577 | — |

**Q² > 100 GeV² (production-card structure: Q2MI=100, x>1e-4 default, MCWE 1 150, NPVE=50000; flags LPAR(2)=1, LPAR(7)=2, LPAR(4)=0, LPAR(8/9/11)=0):**

| Configuration | exe | σ [nb] |
|---|---|---|
| Full-EW, corrected (`log.stdjan_nc_{eminusp,eplusp}_norad_q2gt100_cteq6l_a.OUT`, Apr 2021, exe built 22.02.2021) | normal | **e− 7.3298**, **e+ 7.2939** |
| Photon-only, Born (`log.Dj.NoZ.2Feb23.txt`) | NoZ neu | 6.7086 |
| Photon-only, corrected (`log.Dj.NoZ.NoBORN.txt`) | NoZ neu | **7.2859** |
| Photon-only, corrected, weak SE on (`...LPIN8911.txt`) | NoZ neu | 7.2878 (+0.026%) |
| Photon-only, corrected, weak SE off (`...LPIN8911eq0.txt`) | NoZ neu | 7.2859 |

**Q² > 4 (Sergey card, Jan 30, normal exe):** default (weak SE on): e− 309.259 / e+ 309.216 nb; weak SE off: e− 309.279 / e+ 309.237 nb (weak-SE effect ≈ +0.007% only); `EELE=127.6` probe (e−, weak SE on): 371.341 nb (vs 309.259 nb at 27.6 GeV).

### 4.4 Derived γZ / Z decomposition (this review's arithmetic from the table above)

Full σ = σ_γ + σ_γZ(e±) + σ_Z², with σ_γ charge-symmetric (verified: NoZ e± identical), σ_γZ antisymmetric, σ_Z² symmetric.

**Q²>1000, Born, fixed α:** σ_γ = 218.842 pb; σ_γZ(e−) = (237.68−217.75)/2 = **+9.97 pb** (+4.55% of σ_γ, negative for e+); σ_Z² = 237.68−218.84−9.97 = **+8.87 pb** (+4.06%). Consistency: e+ = 218.84+8.87−9.97 = 217.75 ✓ (exact). Totals: **e− +8.61%, e+ −0.50%** (full/photon-only).

**Q²>1000, corrected (running α):** e− 263.06/243.49 = **+8.04%**; e+ 241.98/243.49 = **−0.62%**.

**Q²>100, corrected (production-relevant):** σ_γ = 7.2859 nb; σ_γZ = (7.3298−7.2939)/2 = **+0.0180 nb** (±0.246%); σ_Z² = **+0.0260 nb** (+0.356%). Totals: **e− +0.60%, e+ +0.11%**.

Auxiliary factors: running-α (LPIN7=2) enhancement: +10.68% (full-EW) / +11.27% (photon-only) at Q2>1000; +8.61% (photon-only) at Q2>100. Weak self-energies (8/9/11 on): −0.42% (e−) / −0.45% (e+) at Q2>1000 full-EW; +0.026% at Q2>100 photon-only (essentially inert once Z is gone). Leptonic soft&virtual QED (LPIN4=1, with LPAR(3)=3 exponentiation): **−49.2%** at Q2>1000 (huge high-y suppression — why the RAD/NONRAD distinction matters).

### 4.5 What the correction was meant for
The campaign produces exactly the ingredient set for a **per-bin γZ/Z correction at Q²>100 for the HERA-II high-Q2 analyses built on these very samples** (the `norad_prod21` NORAD/RAD pool — the same MC basis as our `nonrad_old` pool): full-EW NORAD e± (2021), pure-Born BORN_NORAD e± (Feb 2023), and photon-only NOZ/NOZ1 e− (Feb 2023). The full/photon-only ratio per (x,Q²) bin removes the Z+γZ contribution (or quantifies the e± Born difference); the BORN_NORAD pair separates the soft&virtual/running-α piece. No README or applied-correction write-up exists in the directory; only integrated σ values are in the logs. The e−-only noZ samples imply reliance on the verified charge symmetry of the photon-only term for e+.

---

## 5. STATUS / VALIDATION / SCALED-UP PRODUCTION

### 5.1 Validation evidence found
1. **Charge symmetry of photon-only**: `NoZ.e±` = 218.84216 pb identical for both charges ✓.
2. **Three-term consistency**: σ_Z² extracted from e− equals σ_Z² from e+ to <0.2% at both Q2>1000 and Q2>100 (§4.4) ✓.
3. **Switch-hierarchy checks**: LPIN(2)=0 freezes out all correction switches (`LPIN2.0.allCorrOn ≡ allCorrOff ≡ 237.675`); LPIN(7) inert at Born (`born ≡ born2`) ✓.
4. **The broken-running-α discovery and fix** (§4.2) — the photonselfenergy scan + normal-exe control + "neu" rebuild + Feb-4 re-validation is a complete diagnose-fix-validate loop.
5. **Cross-build reproducibility**: the Apr 11, 2025 re-run (`test/`) reproduces 263.05634 pb — the exact 2020/2021-exe full-EW corrected value at Q2>1000.
6. Minor irregularities: `qq100000.*` files are **not** a Q2MI=100000 run (echo shows Q2MI=1000; σ identical to qq1000.born); `#...#`-marked files are disabled iterations; `djangoh14.log.e+.Ee127GeV` actually ran LLEP=−1.

### 5.2 Scaled-up production: **YES — full campaign samples exist**
dCache `/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/` (owner `levonian`, still listable): all Q2GT100/CTEQ6L, 4 sub-series (A–D) × 65 files ≈ 260 GEN files ≈ 208 GB each:
- `DJANGOH14.NC.EMINUSP.NORAD...` and `...EPLUSP.NORAD...` (full-EW, 2021 baseline)
- `DJANGOH14.NC.{EMINUSP,EPLUSP}.BORN_NORAD...` (pure Born, Feb 2023)
- `DJANGOH14.NC.EMINUSP.NOZ_NORAD...` (photon-only gen-1, GEN timestamps Feb 2, 2023)
- `DJANGOH14.NC.EMINUSP.NOZ1_NORAD...` (photon-only gen-2, Feb 5, 2023)
- `NC.{EPLUSP,EMINUSP}.RAD` (65/61 files)

DUST conversions (ODS+MODS+HAT ROOT per file, in `/data/dust/group/h1/britzger/norad_prod21/`):
- `dst2all_djangoh14_NoZ/` — NOZ_NORAD e−: 260 ods (+mods+hat) files, Feb 2–3, 2023, ≈189 GB
- `dst2all_djangoh14_NoZ_new/` — NOZ1_NORAD e−: 260 ods files, Feb 5–6, 2023, ≈188 GB
- `dst2all_djangoh14_BORN_NORAD/` — EMINUSP BORN_NORAD (steer names), Feb 2, 2023, ≈1038 ods files
- `djangoh14_ar4.10_noZ/oo-files/` (94 GB) and `djangoh14_ar4.10_noZ-2/oo-files/` (48 GB) — full-EW NORAD ODS conversions (Apr 2021); `*/pnfs/` subdirs (97 GB / 49 GB) hold local copies of the full-EW GEN files.

**No e+ photon-only sample exists** (neither NOZ nor NOZ1) — e+ coverage relies on photon-term charge symmetry.

### 5.3 Open provenance questions (route to `h1-mc-db`)
1. **Which executable produced NOZ_NORAD vs NOZ1_NORAD.** Candidates: 2021 2-file build (`build-4.1.1/djangoh14-ariadne10-noZ`), first 2023 broken build (running-α inert ⇒ σ ≈ Born level, visibly wrong), or the 2023 "neu" build. No LOG/banner survives on dCache or DUST for these productions; needs reading run/version info embedded in the FPACK/ODS files.
2. The 2021 production LOGOUTPUT tails (`10000000  3169.42` / `3137.23` / `3135.90`) do **not** match the same-dir `.OUT` `SIGTOT` (7.3298/7.2939 nb, open card, NEVENT 10000001) — factor ≈2.31; either the tail column is not σ_pb at production scale or the 10M-event runs used a different (fiducial-cut) card variant.
3. Exact executable of the Apr-2025 `test/` re-run (assumed 2025-era normal build; C00 FPACK header would confirm).

---

## 6. IMPLICATIONS FOR OUR e− BORN-LEVEL C_rad QUESTION

1. **Confirms: no steering-only Z-off route.** Britzger — the author of the production machinery — also found it necessary to modify code. Our previous verdict (stock DJANGOH14: `LPIN`≤11, `LPAR(17)` vestigial, ZMAS as the only in-card route) stands; his coupling-zeroing is a cleaner, scheme-independent alternative to ZMAS: it kills γZ and Z² *exactly* at the coupling level, with no G_μ-scheme re-derivation hazard (B is multiplied by zero regardless of scheme; his runs used the G_μ default LPAR(4)=2).
2. **The self-energy pitfall we warned about actually materialized** — in the photon sector: the first 2023 build silently killed the running-α (photon self-energy) corrections. Any coupling-level surgery **must** be validated with the LPIN(7)=0/1/2 scan (σ must move by ≈+9–11% at Q2>100/1000) and against a normal-exe control. His `LPIN(8/9/11)` scan also shows the weak self-energies become essentially inert (+0.026%) once Z is gone — consistent with (and weaker than) our ZMAS recipe's advice to zero the companions.
3. **His numbers quantify exactly the effect blocking our e− Born target:** at the production phase space (Q2>100, corrected level), Z+γZ = **+0.60% (e−) vs +0.11% (e+)** integrated — the e± Born difference our `EMINUS_CRAD_INVESTIGATION` record needs, and it grows to **+8.0% (e−) / −0.6% (e+) at Q2>1000**. The per-bin version is obtainable from the sample ratios (§5.2).
4. **Reusability for our photon-only null-test diagnostic:**
   - **Directly usable now:** `dst2all_djangoh14_NoZ_new/` (NOZ1_NORAD e−, ODS/MODS/HAT ROOT on DUST) is an analysis-grade photon-only NORAD Q2GT100 e− sample at the production card — readable by our ODS pipeline for a photon-only vs full-EW per-bin comparison. Prerequisite: settle §5.3-1 (confirm NOZ1 = fixed-exe generation) via `h1-mc-db`; if unresolved, use the ratio NOZ1/NORAD only as a cross-check, not a correction.
   - **Executable reuse:** `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh14test` (2023 "neu") runs read-only, but it is a 4.1.1-era build; mixing it into our 4.2.1-based C_rad pairs would add build systematics. **Preferred:** copy the three modified `.F` files (`hssetp.F`, `hssetf.F`, `hsstr1.F`) from `/data/dust/group/h1/britzger/djangoh14_NoZ/djangoh_h/` into our own build tree and rebuild against 4.2.1 (the diffs are small and self-contained), then validate with the LPIN(7) scan and the charge-symmetry null test before any production.
   - **Coverage gap:** no photon-only **RAD** (INT3/ISM3 on) and no e+ photon-only samples exist. A photon-only C_rad (σ_nonrad/σ_rad both photon-only) — the cleanest null test of the e± C_rad split — would require producing the photon-only RAD counterpart ourselves; the existing samples only support the non-radiative side.

---

## 7. Key file paths (quick reference)

- 2023 NoZ source/binary: `/data/dust/group/h1/britzger/djangoh14_NoZ/{djangoh_h/hssetp.F,djangoh_h/hssetf.F,djangoh_h/hsstr1.F,djangoh14test,version.h}`
- 2021 NoZ build (production exe): `/data/dust/group/h1/britzger/lcg-releases/build-4.1.1/djangoh14-ariadne10-noZ/{djangoh_h/hssetp.F,djangoh_h/hsstr1.F,djangoh14test,version.h}`
- Stock references: `/data/dust/group/h1/britzger/lcg-releases/build-4.2.1/djangoh14/djangoh_h/`, `/data/dust/group/h1/britzger/lcg-releases/build-4.1.1/djangoh14-ariadne10/djangoh_h/`
- Test campaign: `/data/dust/group/h1/britzger/norad_prod21/EW_gZ_correction/` (steering cards, `djangoh14.log.*` LOGOUTPUTs, `log.*.txt` stdouts with HSPRLG PARAMETER LIST + SIGTOT, `test/` 2025 re-validation)
- Production-style noZ card: `.../EW_gZ_correction/stdjh14_nc_eminusp_noZ_q2gt100_cteq6l_a_2Feb23`
- Full-EW reference σ runs: `/data/dust/group/h1/britzger/norad_prod21/djangoh14_ar4.10_noZ/log.stdjan_nc_{eminusp,eplusp}_norad_q2gt100_cteq6l_a.OUT`
- Scaled-up samples: `/data/dust/group/h1/britzger/norad_prod21/{dst2all_djangoh14_NoZ,dst2all_djangoh14_NoZ_new,dst2all_djangoh14_BORN_NORAD}/`, dCache `/pnfs/desy.de/dphep/scratch/h1/generator_files/djangoh14/`

**Verification status:** all σ values, card lines, parameter lists, diffs, dates, and file counts above were read directly from the cited files during this review (verified). The exe assignments for NOZ/NOZ1 GEN productions and the 2021 LOG-tail vs .OUT σ discrepancy are PROVISIONAL (§5.3, route to `h1-mc-db`). The interpretation of the campaign's purpose (§4.5) is inference from the sample matrix, not from a written statement by the author.
