# Corrected-normalization C_rad diagnostic — 2026-09-27

**Status:** `c_rad_applied=false`; no central v6 file or table was changed. This is a diagnostic re-finalization from the existing 12,978/12,978 integrity-PASS histogram partials.

## Mandatory closure gate

**FAIL / not executable from the retained partials.** The requested radiatively quiet high-Q2, moderate-y, moderate-z, low-to-moderate-pTh control must be evaluated separately in e+ and e-. The retained NPZ partials are already summed in `(Q2,x,z,pTh,charge)` and contain neither y slices nor event-level records. No proxy closure was substituted. Therefore none of the numerical tables below is valid for application or final physics interpretation.

## Exact diagnostic normalization

`C_rad=sigma_nonrad/sigma_rad`, `sigma=sum(weight1*weight2)/L_MC`; OLD and NEW NONRAD raw sums are pooled then divided by their summed exposure per beam. RAD `Q2Gki>100` is on the numerator only without RAD-luminosity rescaling. Beam weights are e+ `0.5140930272`, e- `0.4859069728`; `L_data` is absent.

All exposures are nb^-1 and carry provenance quality **DIAGNOSTIC_MANIFEST_LEVEL**: exact for retained/saved manifests, not certified as complete raw-generator exposures. RAD: e+ `16206393.0`, e- `13534017.0`; OLD NONRAD: e+ `12920115.855`, e- `12841572.442`; NEW NONRAD: e+ `1717376.821`, e- `1699702.771`; combined NONRAD: e+ `14637492.676`, e- `14541275.213`.

The full machine-readable result and all 490-row tables are in `/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/data/radiative/esigma_photon_variant_2026-09-26/corrected_normalization/`.

## Gates remaining

`P-CRAD-LUMI`, `P-CRAD-SPARSE`, OLD/NEW NONRAD bin-local pooling compatibility, and the failed/unavailable mandatory y-resolved closure remain open.
