---
domain: science
summary: Pythia8 hit-level samples split 50k hiQ2 plus 50k loQ2, mirroring the Sherpa samples; a supervisor default the user was not asked about.
affects: [hit_level_sim/production/pythia100k/]
verified: 2026-10-27
source: .supervisor/archive/working/research/2026-10-27-pythia8-hit-level-port.md
supersedes:
---
The "100k" Pythia8 hit-level sample is 50k per card (hiQ2 and loQ2), matching the Sherpa samples, so the two generators compare card by card.
Why: the brief said to run exactly the same way as Sherpa; the user was not asked and may change the split.
Rejected: 100k of a single card, which breaks the one-to-one comparison with Sherpa.
