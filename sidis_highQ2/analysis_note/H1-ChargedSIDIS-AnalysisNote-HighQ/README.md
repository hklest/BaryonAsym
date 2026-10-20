# H1 Charged-Hadron SIDIS Analysis Note (High-Q²)

LaTeX source for the H1 internal analysis note covering the
multidifferential charged-hadron SIDIS multiplicities and azimuthal
modulations in the high-Q² regime (Q² > 150 GeV²) using the current
four-period HERA-II/S67 high-Q² sample including ep0304, corresponding
to a target integrated luminosity of 351,000.0 nb⁻¹ = 351 pb⁻¹.
The older direct-monitor quote 356,613.0 nb⁻¹ = 356.613 pb⁻¹ is superseded.

This is an **H1 internal analysis note**, not a journal paper.

**Sister analysis:** The low-Q² companion (5 < Q² < 100 GeV²) is at
[H1-ChargedSIDIS-AnalysisNote-LowQ](https://github.com/hklest/H1-ChargedSIDIS-AnalysisNote-LowQ).
The two analyses share the same H1OO-based framework and HERA-II dataset
but operate in distinct kinematic regimes with separate steering, binning
(Scheme B′ canonical for high-Q²), and unfolding configurations. Do not
mix code, results, or binning tables between the two workspaces.

## Build

```
pdflatex main && bibtex main && pdflatex main && pdflatex main
```

or equivalently:

```
make
```

## Layout

- `main.tex` — master document (article class, A4, 2.5 cm margins).
- `sections/` — section sources, included via `\input` (16 files).
- `references.bib` — bibliography (JHEP style).
- `Makefile` — build helpers (`make`, `make clean`, `make distclean`).
- `.gitignore` — LaTeX auxiliary-file ignore list.
- `figures/` — source figures for inclusion (PDF and PNG tracked; build outputs excluded).

## Key analysis features (high-Q² specific)

- Kinematic regime: Q² > 150 GeV², 0.1 < y < 0.7
- 4D binning: (Q², x, z, pT^h) with Scheme B′ canonical binning (frozen 2026-05-24)
- Unfolding: 4D iterative Bayesian unfolding (canonical; sliced approach superseded 2026-05-25)
- Extractor: v3 (pT_h + φ_h frame fix applied 2026-05-24)
- Production sample: SIDIS-local v5g 4-period sample, Empz=[35,70], ep0304 included
- Phase 2 systematics in progress (critic-iteration cycle)

## Overleaf

This source tree can be mirrored to an Overleaf project for collaborative
editing. Push from git to Overleaf via the standard `git push overleaf`
workflow; pull edits with `git pull overleaf master`.

<!-- connectivity test 2026-09-09T21:13Z -->
