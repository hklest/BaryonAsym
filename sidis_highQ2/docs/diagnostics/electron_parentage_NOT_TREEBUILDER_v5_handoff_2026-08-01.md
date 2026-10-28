# NOT_TREEBUILDER v5 parentage smoke QA handoff

Status: PASS for the requested smoke QA. Remote validation artifacts are under the smoke_v5_20260801 directory:
- `validation_summary_20260801.json`
- `validation_summary_20260801.md`

MC ROOT: 219 diagnostic rows, 97 branches; data ROOT: 0 rows, 97 branches. Trees opened successfully and schemas are identical. MC and data both consumed/scanned 100 EvlTree entries; MC's 219 rows are per retained electron track, while data's zero rows are the expected semantic. MC numeric values are finite (0 non-finite values). Parentage fields are retained; documented SVX/STR unavailable fields use -999 sentinels.

Mapping closure is exact for all 219 populated MC rows (219/219), using q2/x/z/pth to reco490 with the binmap's strict half-open edges and `cell*70 + z*10 + pth` convention. Canonical equivalence evidence is source-level audit plus exact stored-index closure. Independent numerical pTh/z comparison is limited because v5 does not store source four-vectors. Binmap has NB_R=490 and reco-to-gen length 490.

No central/v3 files were changed. Plotting is unblocked for this smoke diagnostic's structural/parentage QA scope; this does not alter central-release policy.