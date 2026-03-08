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

# Fallback executable search paths used when submit files reference non-existing binaries.
# Intentionally restricted to klesthen area only.
PIPELINE_EXEC_SEARCH_DIRS=(
  "/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-el9-gcc14-opt"
  "/data/dust/group/h1/klesthen/BaryonAsym/bin/x86_64-centos7-gcc9-opt"
)

# If 1, rewrite Condor submit executable path to the first existing fallback match by basename.
AUTO_FIX_EXECUTABLE="${AUTO_FIX_EXECUTABLE:-1}"

# Optional path rewrites applied to submit-file contents before submission.
# Format: "old_prefix|new_prefix"
PIPELINE_PATH_REWRITES=(
  "/data/dust/group/h1/gtustin/h1oo|/data/dust/group/h1/klesthen/BaryonAsym"
)

# If 1, validate steering file for the first process and ensure output directories exist.
PRECHECK_STEERING_AND_OUTPUTS="${PRECHECK_STEERING_AND_OUTPUTS:-1}"

# If 1, auto-generate known minimal steering files for process 0 when missing.
AUTO_GENERATE_MINIMAL_STEERING="${AUTO_GENERATE_MINIMAL_STEERING:-1}"

# If 1, wait for currently submitted clusters to finish after each stage block before continuing.
WAIT_BETWEEN_STAGES="${WAIT_BETWEEN_STAGES:-1}"
