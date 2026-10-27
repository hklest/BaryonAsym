---
domain: science
summary: Pythia8 quark- and lepton-line QED showers stay off, matching Sherpa's no-QED cards; the user decided this.
affects: [hit_level_sim/pythia8/cards/]
verified: 2026-10-27
source: user 2026-10-27 ("No leave pythia8's QED off.")
supersedes: archive/memory/decisions/2026-10-27-pythia8-qed-showers-off.md
---
Pythia8 hit-level cards keep QED showers off on both quark and lepton lines, so the generator comparison is LO without radiation on both sides.
Why: Sherpa's cards have no QED radiation; matching them isolates shower and hadronisation differences. The user confirmed it when offered the flip.
Rejected: Pythia8's default QED showering, which would add an ISR/FSR effect Sherpa lacks.
