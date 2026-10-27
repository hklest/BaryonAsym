---
domain: code
summary: v7 trigger floor {61,0,3,67} on data only; high-Q2 V0 MC uses MC13 catalogs 8948-8962, not 70xx
affects: [H1BaryonAsym/src/TreeBuilder_v7.C, H1BaryonAsym/steering/v7/]
verified: 2026-10-27
source: user 2026-10-27 (supervisor decision)
supersedes:
---
Trigger floor {61,0,3,67} applied to data only; MC stores all bits. GenV0 rows with z>1 (0.27%, FSR-as-ISR) are stored, not cut. High-Q2 V0 MC uses MC13 replacement catalogs 8948-8962 (oo-4.0.25) for reco and gen. The 70xx catalogs are broken for the V0 path (MODS -> /acs/, GetV0 segfault) and are not used in v7.
