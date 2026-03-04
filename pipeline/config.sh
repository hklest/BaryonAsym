#!/usr/bin/env bash

# Central pipeline configuration (safe defaults for a minimal smoke test run).
# Paths are repo-relative where possible.

PIPELINE_DATA_EVL_SUBMITS=(
  "H1LambdaC/CondorScripts/multi_submit_create_evls_06"
)
PIPELINE_DATA_LAMBDA_SUBMITS=(
  "H1LambdaC/CondorScripts/multi_submit_lambdaTree06"
)
PIPELINE_DATA_KAON_SUBMITS=(
  "H1LambdaC/CondorScripts/multi_submit_kaonTree06"
)

PIPELINE_MC_EVL_SUBMITS=(
  "H1LambdaC_MC/CondorScripts/multi_submit_create_evlsMC_Rad_RAPGAP31_06"
)
PIPELINE_MC_LAMBDA_SUBMITS=(
  "H1LambdaC_MC/CondorScripts/multi_submit_lambdaTreeMC_Rad_RAPGAP31_06"
)
PIPELINE_MC_KAON_SUBMITS=(
  "H1LambdaC_MC/CondorScripts/multi_submit_kaonTreeMC_Rad_RAPGAP31_06"
)
PIPELINE_MC_GEN_SUBMITS=(
  "H1LambdaC_MC/CondorScripts/multi_submit_genMCTree_Rad_RAPGAP31_06"
)

# Optional hadd and analysis commands (leave empty to skip if your environment differs).
PIPELINE_HADD_COMMANDS=(
  ""
)
PIPELINE_ANALYSIS_COMMANDS=(
  ""
)
