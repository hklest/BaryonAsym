# Production 8529/8530/8531/9022/9023 (2026-10-28)
Chain: stage-1 evls (clusters 5959356/57/59/61/62) -> stage-2 v6dif --diff-ods (5959455/60/61/62/63) -> hadd (5960857). Default priority, all Condor. No luminosity normalisation done; mck_samples.json and existing merges untouched.
Gates per chunk: rc=0, DiffTot=evls entries, odsLoaded=100%, no scalar DiffTot branch >5% sentinel (<-998) on reco-env events, weight1 present (9022/9023 all 1; 8529/30/31 min >=1). All 40 chunks pass after retry.
Failures: stage 2 first attempt rc=134 ("malloc(): unsorted double linked list corrupted" after output written) in 9 chunks (8529: 5,10; 8530: 0,1,8; 8531: 4; 9022: 4; 9023: 1,2); each rerun once, all rc=0 with identical-gate PASS. Retry logs: mc<R>/logs/ (first try in logs/try1/). Stage 1: no failures. Cause of the abort not investigated.
Events in (hat generator input) not tallied; evls kept = EvlTree entries.
| R | chunks | evls kept | reco-env | sum weight1 | merged (size) |
|---|---|---|---|---|---|
| 8529 | 11/11 | 124286 | 35869 | 202818.14 | data/t2_detector/mcnew_merged_2026-10-28/8529.root (195 MB) |
| 8530 | 11/11 | 80438 | 23216 | 188464.52 | .../8530.root (135 MB) |
| 8531 | 11/11 | 130260 | 41476 | 213741.17 | .../8531.root (214 MB) |
| 9022 | 6/6 | 21302 | 4338 | 21302 | .../9022.root (29 MB) |
| 9023 | 6/6 | 21305 | 4277 | 21305 | .../9023.root (29 MB) |
Merged DiffTot = sum of chunk entries and reco-env/Σweight1 match the tally for all five. Per-chunk tally CSV and sha256: data/t2_detector/mcnew_merged_2026-10-28/{tally_2026-10-28.csv,README.md}.
Notes: 9022/9023 are the POMC charm samples (DB name RG31034.POMC), not uds. Weighted samples: Σweight1 is a raw sum, unnormalised.
