---
domain: science
summary: QED-on Djangoh/Rapgap hit samples share one pool with sherpa100k_set2 and pythia100k; only the generator name distinguishes them (rapgap100k, djangoh100k).
affects: [hit_level_sim/production/]
verified: 2026-10-27
source: user 2026-10-27 ("No, same pool"); .supervisor/working/research/2026-10-27-rapgap-djangoh-hit-level-feasibility.md
supersedes:
---
QED-on Djangoh and Rapgap hit samples are not a separate sample class: they live beside sherpa100k_set2 and pythia100k, named rapgap100k and djangoh100k.
Why: the user wants one pool, so downstream code treats every generator alike.
Rejected: a separate QED-on sample class or directory.
