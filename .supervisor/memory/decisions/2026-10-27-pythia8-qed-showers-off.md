---
domain: science
summary: Pythia8 quark- and lepton-line QED showers are off to match Sherpa's no-QED cards; a supervisor default the user may flip.
affects: [hit_level_sim/pythia8/cards/]
verified: 2026-10-27
source: .supervisor/archive/working/research/2026-10-27-pythia8-hit-level-port.md
supersedes:
---
Pythia8 hit-level cards switch QED showers off on both quark and lepton lines, so the generator comparison is LO without radiation on both sides.
Why: Sherpa's cards have no QED radiation; matching them isolates shower and hadronisation differences. Not asked of the user, who may flip it.
Rejected: Pythia8's default QED showering, which would add an ISR/FSR effect Sherpa lacks.
