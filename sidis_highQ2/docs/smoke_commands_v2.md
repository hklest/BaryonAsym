# Smoke commands — alpha0p2 reco background vectors v2

All commands below are one-file smokes only and are safe for later execution agents to review/run on NAF login within the stated policy.

Implementation root:
`/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/`

## RAPGAP electron smoke

```bash
python3 scripts/produce_reco_background_vector_alpha0p2_v2.py \
  --mode electron \
  --mc rapgap31 \
  --period ep0304 \
  --input-root /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/rapgap31/7097/sidistree_rapgap31_7097_ep0304_0000.root \
  --response-script /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py \
  --binmap /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json \
  --output-npz /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_rapgap31_ep0304_electron_reco_background_vector_alpha0p2_v2.npz \
  --output-json /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_rapgap31_ep0304_electron_reco_background_vector_alpha0p2_v2.json \
  --smoke \
  --max-entries 200000
```

## RAPGAP V0-daughter smoke

```bash
python3 scripts/produce_reco_background_vector_alpha0p2_v2.py \
  --mode v0daughter \
  --mc rapgap31 \
  --period ep0304 \
  --input-root /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/rapgap31/7097/sidistree_rapgap31_7097_ep0304_0000.root \
  --response-script /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py \
  --binmap /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json \
  --output-npz /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_rapgap31_ep0304_v0daughter_reco_background_vector_alpha0p2_v2.npz \
  --output-json /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_rapgap31_ep0304_v0daughter_reco_background_vector_alpha0p2_v2.json \
  --smoke \
  --max-entries 200000
```

## DJANGOH electron smoke

```bash
python3 scripts/produce_reco_background_vector_alpha0p2_v2.py \
  --mode electron \
  --mc djangoh14 \
  --period ep0607 \
  --input-root /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/djangoh14/merged_7091.root \
  --response-script /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py \
  --binmap /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json \
  --output-npz /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_djangoh14_ep0607_electron_reco_background_vector_alpha0p2_v2.npz \
  --output-json /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_djangoh14_ep0607_electron_reco_background_vector_alpha0p2_v2.json \
  --smoke \
  --max-entries 200000
```

## DJANGOH V0-daughter smoke

```bash
python3 scripts/produce_reco_background_vector_alpha0p2_v2.py \
  --mode v0daughter \
  --mc djangoh14 \
  --period ep0607 \
  --input-root /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/sidistree/mc/djangoh14/merged_7091.root \
  --response-script /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/copied_scripts/build_migration_th2_per_charge_v4.py \
  --binmap /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/migration_inputs_v4_alpha0p2_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_percharge.binmap_v3.json \
  --output-npz /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_djangoh14_ep0607_v0daughter_reco_background_vector_alpha0p2_v2.npz \
  --output-json /data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/electron_cone_alpha0p2_z005_20260623/reco_background_vectors_impl_v2_20260624/smoke/SMOKE_djangoh14_ep0607_v0daughter_reco_background_vector_alpha0p2_v2.json \
  --smoke \
  --max-entries 200000
```

## Aggregation smoke example

```bash
python3 scripts/aggregate_reco_background_vectors_alpha0p2_v2.py \
  --input-npz smoke/SMOKE_part1.npz \
  --input-json smoke/SMOKE_part1.json \
  --input-npz smoke/SMOKE_part2.npz \
  --input-json smoke/SMOKE_part2.json \
  --output-npz smoke/SMOKE_aggregated_alpha0p2_reco_background_vector_v2.npz \
  --output-json smoke/SMOKE_aggregated_alpha0p2_reco_background_vector_v2.json \
  --allow-smoke
```
