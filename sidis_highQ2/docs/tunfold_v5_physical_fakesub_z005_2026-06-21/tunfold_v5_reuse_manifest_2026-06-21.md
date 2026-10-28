# TUnfold v5 physical/fake-sub z005 diagnostic reuse manifest (2026-06-21)

**Status:** diagnostic package only; **do not promote TUnfold outputs**. Central high-Q2 charged-SIDIS z005 remains IBU/BBB finalcandidate. Scope is inclusive charged-hadron SIDIS high-Q2 only; no PID/species work.

## Paths
- `script_root`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005`
- `output_root`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621`
- `manifest_dir`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest`
- `remote_json`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/tunfold_v5_reuse_manifest_2026-06-21.json`
- `remote_markdown`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/tunfold_v5_reuse_manifest_2026-06-21.md`
- `central_ibu_readonly_comparison_release`: `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_finalcandidate_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-06-10`
- `local_plot_products`: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/plots/tunfold_v5_physical_fakesub_z005_phase2_2026-06-21`
- `local_manifest_mirror_target`: `/Users/hklest/Desktop/opencode/H1/sidis_highQ2/docs/tunfold_v5_physical_fakesub_z005_2026-06-21/`

## Environment
- Required view: `LCG_109a`
- Setup: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh`

## Correct conventions
- **v4_bug:** fake/miss ordinary bins invalid.
- **v5_physical_fakesub:** 490x245 physical response, fake-subtracted 490-bin input, miss/efficiency sidecars.
- **truth_fiducial_correction:** x_corr = x_visible / eps_response.
- **closure_truth:** full gen-level matched + miss, not matched-only.
- **tau0_regularization_uncertainty:** tau=0 reg uncertainty = 0.
- **covariance_and_lumi_corrections:** TUnfold stat covariance propagated through efficiency; IBU stat/reg uncertainties converted with 356000 nb^-1 per user correction.

## Script inventory
| File | Present | Size | SHA256 | Purpose | Command pattern |
|---|---:|---:|---|---|---|
| `build_physical_response_fakesub_inputs_v5.py` | True | 6841 | `4926f203164ea6b737fa0664ff331f155008399942417479a8ed1c37a72cd5b5` | Builds v5 physical/fake-subtracted TUnfold input package from existing v4 response ROOT and data/reco sidecars. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 build_physical_response_fakesub_inputs_v5.py --input-v4-response [v4_response.root] --central-release [central_ibu_release] --output-dir [output_root]/inputs_physical_fakesub_v5` |
| `compare_tau0_tunfold_effcorr_vs_ibu.py` | True | 11916 | `56a14a304f3ecc03cc52871a19fa04725860948b98697c2ef7559f32080dac16` | Compares efficiency-corrected tau=0 TUnfold result against IBU. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 compare_tau0_tunfold_effcorr_vs_ibu.py --tunfold-visible [tau0_output] --eps-response [eps.json] --ibu-release [central_ibu_release] --outdir [effcorr_comparison_dir]` |
| `compare_tau0_tunfold_vs_ibu.py` | True | 11976 | `5ff080d62620c46217d361388a0334423e3fa01537ff89e7a627544621fb7684` | Compares tau=0 TUnfold diagnostic yields/cross sections to central IBU. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 compare_tau0_tunfold_vs_ibu.py --tunfold [tau0_output] --ibu-release [central_ibu_release] --outdir [comparison_dir]` |
| `compare_tunfold_ibu_bbb_v5_physical_fakesub.py` | True | 671 | `f56936c2576233c16cb77b5a21ab02e270fa8340fd34436be154aa3b653284b9` | Auxiliary script/configuration produced in the v5 diagnostic package; inspect script header and manifest before reuse. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run auxiliary file only within study-local package` |
| `make_cross_closure_rhoi_tau_browse_v5_from_existing.py` | True | 20733 | `a3e513b714923220a0ef3eafd5eb5c8a1dedd7034d98c705c2ab77c37d9582a1` | Auxiliary script/configuration produced in the v5 diagnostic package; inspect script header and manifest before reuse. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run auxiliary file only within study-local package` |
| `make_cross_closure_tau_browse_v5.py` | True | 19186 | `e1d7efec0bc604c58af74f4596fd3f5680f2356ad2df56f5c015b415fe34f586` | Builds tau-browse plots/tables for cross-generator closure. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 make_cross_closure_tau_browse_v5.py --closure-dir [cross_closure_dir] --outdir [tau_browse_dir]` |
| `make_tunfold_qa_plots_v5_physical_fakesub.py` | True | 6992 | `a551e6dd0c23c379af7053e77c8bc7e4546d00be70dcd64b1f8609a0448bc42e` | Creates TUnfold QA plots for tau/rho/refold/negative-bin diagnostics. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 make_tunfold_qa_plots_v5_physical_fakesub.py --input [scan_or_selection] --outdir [qa_plot_dir]` |
| `plot_tau0_effcorr_vs_ibu_with_uncertainties.py` | True | 22147 | `f3f238fe1e7c2fb8ca4ff30bde89244706f54990a15b2d8178c05e2d5a4135f8` | Builds uncertainty-aware TUnfold tau=0 efficiency-corrected vs IBU ratio plots. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 plot_tau0_effcorr_vs_ibu_with_uncertainties.py --comparison [effcorr_comparison.json] --uncertainties [uncertainty_sidecars] --outdir [uncertainty_plot_dir]` |
| `run_cross_generator_closure_v5_physical_fakesub.py` | True | 18827 | `032e233a7e0283c7b940ee7289bf848b7cd39edee581d1a1dc079cae8bded099` | Runs cross-generator closure diagnostics for RG/DJ response/truth combinations. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 run_cross_generator_closure_v5_physical_fakesub.py --inputs [v5_inputs] --outdir [cross_closure_dir]` |
| `run_poisson_closure_v5_physical_fakesub.py` | True | 1015 | `8e4bc85157fcf37edf181df9917f780c7f493ef5cb6da74bb6017cb6b460bfec` | Auxiliary script/configuration produced in the v5 diagnostic package; inspect script header and manifest before reuse. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run auxiliary file only within study-local package` |
| `run_tau_scan_expand_v5_physical_fakesub.sh` | True | 1280 | `6be748920d74f6bd31a8f6c3de4758b1094468584dd56f2f7eb06a3579373643` | Auxiliary script/configuration produced in the v5 diagnostic package; inspect script header and manifest before reuse. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run auxiliary file only within study-local package` |
| `run_tau_scan_v5_physical_fakesub.sh` | True | 1570 | `0c1c6bad22b0837d70028aef931734db98ab678320d201eeaab0e5bc0f6b7c23` | Shell wrapper for tau scan under LCG_109a/ROOT. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && bash run_tau_scan_v5_physical_fakesub.sh [input_root] [output_root]` |
| `run_truth_fiducial_closure_v5_physical_fakesub.py` | True | 16639 | `45720cf276ea4a9fe56aa0ed2aab1bc3376f0c0c474d06885259127166ce3a35` | Runs truth-fiducial closure with visible-to-truth correction. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 run_truth_fiducial_closure_v5_physical_fakesub.py --visible [tau0_visible] --eps-response [eps.json] --truth [full_gen_truth_matched_plus_miss] --outdir [truth_fiducial_closure_dir]` |
| `select_tau_v5_physical_fakesub.py` | True | 2988 | `9cd5ebf7622ecf20e3f2a1cf5c998aeec9575a7c169a4f606552dced00227042` | Ranks/selects tau candidates from scan diagnostics. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 select_tau_v5_physical_fakesub.py --scan-dir [tau_scan_dir] --output [tau_selection.json]` |
| `unfold_tunfold_single_tau_v5_physical_fakesub.C` | True | 5327 | `176948a29883b5b0fc616a61307117cc5df4f0572f6a839c2ef4e4d0647614e3` | Runs TUnfold once at a selected tau using physical response and fake-subtracted reco vector. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run study-local command for unfold_tunfold_single_tau_v5_physical_fakesub.C with explicit input/output arguments under the v5 diagnostic roots` |
| `unfold_tunfold_tau_scan_v5_physical_fakesub.C` | True | 84 | `a8361b926b24011da8460e1179f09aae3df664b096804b2d34dc60c4ac75ee38` | Runs TUnfold across tau grid for diagnostic scans. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run study-local command for unfold_tunfold_tau_scan_v5_physical_fakesub.C with explicit input/output arguments under the v5 diagnostic roots` |
| `validate_phase2_outputs_v5_physical_fakesub.py` | True | 3120 | `a09f59d35409dce1fc345654951e90d09726d0dda7f5f56e5927b9a091246b5e` | Auxiliary script/configuration produced in the v5 diagnostic package; inspect script header and manifest before reuse. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && run auxiliary file only within study-local package` |
| `validate_physical_tunfold_inputs_v5.py` | True | 3861 | `7776a7384ee692410bfb373eadd0d69656bb5dca78c00b0df6641b98f385cf4b` | Validates v5 physical/fake-sub inputs and sidecars before TUnfold. | `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 validate_physical_tunfold_inputs_v5.py --input-dir [output_root]/inputs_physical_fakesub_v5 --output-json [validation.json]` |

## Output inventory summary
- Output products indexed: `263`
- Hash coverage count: `280` (scripts/configs and output products <=10 MiB; large artifacts size/mtime only)
- Nonzero records: `281`; zero-size records: `0`

### Key output index
- **tau_browse:** 9 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/root/cross_closure_correlation_tau_browse_v5.root`
- **rho_browse:** 4 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
- **tau0_ibu_comparison:** 19 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/csv/tunfold_tau0_effcorr_vs_ibu_ratios.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/json/tunfold_tau0_effcorr_vs_ibu_ratios.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/manifest_tau0_effcorr_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_zoom_0p75_1p25.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/csv/tunfold_tau0_effcorr_vs_ibu_with_uncertainties.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/json/tunfold_tau0_effcorr_vs_ibu_with_uncertainties.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/manifest_tau0_effcorr_vs_ibu_with_uncertainties.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/_tmp_bundle_page.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_zoom_0p75_1p25.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/csv/tunfold_tau0_vs_ibu_ratios.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/json/tunfold_tau0_vs_ibu_ratios.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_zoom_0p75_1p25.pdf`
- **truth_fiducial_closure:** 4 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/csv/truth_fiducial_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/json/truth_fiducial_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/pdf/truth_fiducial_closure_residuals_tau0_tau1em08.pdf`
- **uncertainty_aware_ratio:** 0 product(s)
- **closure:** 27 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/csv/cross_generator_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/cross_generator_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/manifest_cross_generator_closure_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/manifest_cross_generator_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_combined_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_combined_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_neg_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_neg_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_pos_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_pos_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residuals_with_crossclosure.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/poisson_closure_pending_manifest_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/root/cross_generator_closure_v5_physical_fakesub.root`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/root/cross_closure_correlation_tau_browse_v5.root`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/csv/truth_fiducial_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/json/truth_fiducial_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/pdf/truth_fiducial_closure_residuals_tau0_tau1em08.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/pdf/tunfold_v5_physical_fakesub_z005_phase2_QA_bundle_with_crossclosure.pdf`
- **manifest:** 17 product(s)
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/preexisting_files_phase1_start.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/v5_phase1_manifest.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/v5_phase1_summary.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/manifest_cross_generator_closure_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/manifest_cross_generator_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/poisson_closure_pending_manifest_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/manifest_tau0_effcorr_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/manifest_tau0_effcorr_vs_ibu_with_uncertainties.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/manifest_phase2_tunfold_v5_physical_fakesub_z005.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/validation_phase2_tunfold_v5_physical_fakesub_z005.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/validation_phase2_tunfold_v5_physical_fakesub_z005.txt`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/compare_tunfold_ibu_bbb_not_run_manifest.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/qa_plot_manifest_v5_physical_fakesub.json`

## Reuse runbook
### 0. Confirm diagnostic scope
- Command pattern: `Read RUN_STATUS.md and context-handoff.md; confirm central z005 is IBU/BBB finalcandidate and TUnfold v5 remains diagnostic/not promoted.`
- Acceptance: No central-release or v4 canonical scripts are modified.

### 1. Set environment
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh`
- Acceptance: python3, ROOT, and required PyROOT/uproot stack import successfully.

### 2. Build physical/fake-sub inputs from existing v4 response ROOT
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 build_physical_response_fakesub_inputs_v5.py --input-v4-response [v4_response.root] --central-release [central_ibu_release] --output-dir [output_root]/inputs_physical_fakesub_v5`
- Acceptance: Response is 490x245 physical matrix; reco input is 490 bins and fake-subtracted; miss/eps_response sidecars exist and are nonzero.

### 3. Validate v5 inputs
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 validate_physical_tunfold_inputs_v5.py --input-dir [output_root]/inputs_physical_fakesub_v5 --output-json [validation.json]`
- Acceptance: Validation passes shape, finite, nonzero, fake-subtraction, miss, and efficiency checks.

### 4. Run tau scan and tau/rho browse
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && bash run_tau_scan_v5_physical_fakesub.sh [input_root] [output_root] ; source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 select_tau_v5_physical_fakesub.py --scan-dir [tau_scan_dir] --output [tau_selection.json] ; source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 make_tunfold_qa_plots_v5_physical_fakesub.py --input [scan_or_selection] --outdir [qa_plot_dir]`
- Acceptance: Tau and rho browse products exist; negative-bin counts, refold chi2, tails, and high correlations are reviewed.

### 5. Run tau=0 diagnostic comparison
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 compare_tau0_tunfold_vs_ibu.py --tunfold [tau0_output] --ibu-release [central_ibu_release] --outdir [comparison_dir]`
- Acceptance: Tau=0 outputs are nonzero; reg uncertainty is zero; comparison is labelled diagnostic only.

### 6. Apply truth-fiducial correction and closure
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 run_truth_fiducial_closure_v5_physical_fakesub.py --visible [tau0_visible] --eps-response [eps.json] --truth [full_gen_truth_matched_plus_miss] --outdir [truth_fiducial_closure_dir]`
- Acceptance: Use x_corr=x_visible/eps_response and full gen-level matched+miss truth; corrected closure tables/plots are produced.

### 7. Build efficiency-corrected TUnfold-vs-IBU uncertainty-aware plots
- Command pattern: `source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 compare_tau0_tunfold_effcorr_vs_ibu.py --tunfold-visible [tau0_output] --eps-response [eps.json] --ibu-release [central_ibu_release] --outdir [effcorr_comparison_dir] ; source /cvmfs/sft.cern.ch/lcg/views/LCG_109a/x86_64-el9-gcc14-opt/setup.sh && python3 plot_tau0_effcorr_vs_ibu_with_uncertainties.py --comparison [effcorr_comparison.json] --uncertainties [uncertainty_sidecars] --outdir [uncertainty_plot_dir]`
- Acceptance: Plots/tables include TUnfold stat covariance through efficiency and IBU stat/reg converted with 356000 nb^-1; tau0 reg=0.

### 8. Package and critique
- Command pattern: `Write/retrieve manifest JSON+Markdown; send critic target list.`
- Acceptance: Manifests are nonzero locally and remotely; all caveats remain visible.

## Known caveats/blockers
- Diagnostic only; do not promote TUnfold outputs or use as central physics cross-check.
- Central z005 remains IBU/BBB finalcandidate.
- HISTORICAL 2026-06-21 caveat superseded 2026-06-24: DJANGOH14 gen-only `C_rad` v1 is now the provisional z005 baseline; final physics release remains blocked by sparse-bin policy, publication-grade DJ MC-lumi provenance, and RAPGAP quarantine decision.
- Poisson-fluctuated closure is pending; deterministic/noiseless closure is not sufficient.
- epsilon MC uncertainty and full covariance propagation are pending.
- High refold chi2/tails/negative bins persist and must be treated as method pathology.
- SVD/RooUnfoldSvd/TSVDUnfold remains quarantined for multidimensional flattened-bin topology.
- No PID/species splitting is in scope; inclusive h+/h- only.

## Critic target list
### Scripts
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/build_physical_response_fakesub_inputs_v5.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/compare_tau0_tunfold_effcorr_vs_ibu.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/compare_tau0_tunfold_vs_ibu.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/compare_tunfold_ibu_bbb_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/make_cross_closure_rhoi_tau_browse_v5_from_existing.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/make_cross_closure_tau_browse_v5.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/make_tunfold_qa_plots_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/plot_tau0_effcorr_vs_ibu_with_uncertainties.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_cross_generator_closure_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_poisson_closure_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_tau_scan_expand_v5_physical_fakesub.sh`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_tau_scan_v5_physical_fakesub.sh`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/run_truth_fiducial_closure_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/select_tau_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/unfold_tunfold_single_tau_v5_physical_fakesub.C`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/unfold_tunfold_tau_scan_v5_physical_fakesub.C`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/validate_phase2_outputs_v5_physical_fakesub.py`
- `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v5_physical_fakesub_z005/validate_physical_tunfold_inputs_v5.py`

### Outputs
- **tau_browse**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/root/cross_closure_correlation_tau_browse_v5.root`
- **rho_browse**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
- **tau0_ibu_comparison**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/csv/tunfold_tau0_effcorr_vs_ibu_ratios.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/json/tunfold_tau0_effcorr_vs_ibu_ratios.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/manifest_tau0_effcorr_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/pdf/tunfold_tau0_effcorr_vs_ibu_ratio_zoom_0p75_1p25.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/csv/tunfold_tau0_effcorr_vs_ibu_with_uncertainties.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/json/tunfold_tau0_effcorr_vs_ibu_with_uncertainties.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/manifest_tau0_effcorr_vs_ibu_with_uncertainties.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/_tmp_bundle_page.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/pdf/tunfold_tau0_effcorr_vs_ibu_with_unc_zoom_0p75_1p25.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/csv/tunfold_tau0_vs_ibu_ratios.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/json/tunfold_tau0_vs_ibu_ratios.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_bundle.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/pdf/tunfold_tau0_vs_ibu_ratio_zoom_0p75_1p25.pdf`
- **truth_fiducial_closure**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/csv/truth_fiducial_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/json/truth_fiducial_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/pdf/truth_fiducial_closure_residuals_tau0_tau1em08.pdf`
- **closure**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/csv/cross_generator_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/cross_generator_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/manifest_cross_generator_closure_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/manifest_cross_generator_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_combined_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_combined_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_neg_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_neg_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_pos_fullrange.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residual_abs_pos_zoom_0to25pct.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/pdf/cross_generator_closure_residuals_with_crossclosure.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/poisson_closure_pending_manifest_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/root/cross_generator_closure_v5_physical_fakesub.root`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_rhoi_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/csv/cross_closure_correlation_tau_browse_metrics.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_rhoi_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/json/cross_closure_correlation_tau_browse_summary.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_rhoi_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/pdf/cross_closure_correlation_tau_browse_v5.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/root/cross_closure_correlation_tau_browse_v5.root`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/csv/truth_fiducial_closure_residuals_v5_physical_fakesub.csv`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/json/truth_fiducial_closure_residuals_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/pdf/truth_fiducial_closure_residuals_tau0_tau1em08.pdf`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/pdf/tunfold_v5_physical_fakesub_z005_phase2_QA_bundle_with_crossclosure.pdf`
- **manifest**
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/preexisting_files_phase1_start.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/v5_phase1_manifest.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/manifest/v5_phase1_summary.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/json/manifest_cross_generator_closure_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/manifest_cross_generator_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure/poisson_closure_pending_manifest_v5_physical_fakesub.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_rhoi_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_tau_browse/manifest_cross_closure_correlation_tau_browse.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_closure_truth_fiducial/manifest_truth_fiducial_closure_v5_physical_fakesub.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu/manifest_tau0_effcorr_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_effcorr_vs_ibu_with_uncertainties/manifest_tau0_effcorr_vs_ibu_with_uncertainties.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_comparisons/tau0_vs_ibu/manifest_tau0_vs_ibu.md`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/manifest_phase2_tunfold_v5_physical_fakesub_z005.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/validation_phase2_tunfold_v5_physical_fakesub_z005.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_manifest/validation_phase2_tunfold_v5_physical_fakesub_z005.txt`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/compare_tunfold_ibu_bbb_not_run_manifest.json`
  - `/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/studies/tunfold_official_v5_physical_fakesub_z005_20260621/phase2_qa/qa_plot_manifest_v5_physical_fakesub.json`

### Questions
- Does v5 remove the v4 fake/miss ordinary-bin bug everywhere?
- Is x_corr=x_visible/eps_response applied with full matched+miss truth in closure?
- Are tau=0 regularization and statistical covariance conventions implemented correctly?
- Are IBU uncertainties converted with 356000 nb^-1 and not stale luminosity?
- Do refold chi2, tails, negative-bin patterns, and rho matrices justify diagnostic-only status?

## Verification
- `scripts_inventoried`: `18`
- `known_required_scripts_present`: `13`
- `known_required_scripts_missing`: `0`
- `output_products_indexed`: `263`
- `hash_coverage_count`: `280`
- `hash_coverage_scope`: `All inventoried script/config files and output products <=10 MiB were hashed; large output artifacts recorded with size/mtime only.`
- `size_coverage_count`: `281`
- `nonzero_count`: `281`
- `zero_size_record_count`: `0`
