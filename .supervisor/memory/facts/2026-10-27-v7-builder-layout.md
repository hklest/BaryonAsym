---
domain: code
summary: v7 TreeBuilder locations, build command, and unfillable branches
affects: [H1BaryonAsym/src/TreeBuilder_v7.C, H1BaryonAsym/src/TreeBuilder_v7.h, H1BaryonAsym/steering/v7/, lambda/scripts/sidis_v0/wp0p/, H1BaryonAsym/output/v7/]
verified: 2026-10-27
source: user 2026-10-27 (supervisor)
supersedes:
---
Builder H1BaryonAsym/src/TreeBuilder_v7.{C,h}; binary H1BaryonAsym/bin/x86_64-el9-gcc14-opt/TreeBuilder_v7 (make TreeBuilder_v7); steering H1BaryonAsym/steering/v7/; scripts lambda/scripts/sidis_v0/wp0p/; outputs H1BaryonAsym/output/v7/. Unfillable branches: HadFracBarrel, L2vst, PrescaleWeight (use TrigWeightAC/RW + Prescale[128]), V0 DCA'/Dz0'/MomCov, HERA-II dE/dx chi2, ProcessId. MC lacks run-lumi fields.
