#!/usr/bin/env python3
"""WP-A2 S3 purity fact for WP-B/WP-F (D3 H3, not a correction): share of the 9012
cut-removed template (A_pre && selFmd && ePlug<7 && selFts, full eta range, even
split) with generator x_IP (hatXpomGki) >= 0.03, per H3 payload variant
(i) base, (ii) 35<Empz<70, (iii) (ii) && xIp<0.04 && mx>3.  One file, login node.
9012 carries w_event = 1, so the share is a raw-count fraction."""
import json, sys
from pathlib import Path
import numpy as np, uproot
F = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/t2_detector/mc9012_merged_2026-10-18/9012.root"
OUT = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2/s3_corr/analysis/xip_share_9012.json")
f = uproot.open(F)
d = f["DiffTot"].arrays(["RunNumber", "EventNumber", "passRecoEnv", "Q2s", "Ys", "hatVtxZ", "selFmd", "ePlug", "selFts", "xIp", "mx", "hatXpomGki"], library="np")
key = (d["RunNumber"].astype(np.int64).astype(np.uint64) << np.uint64(32)) | d["EventNumber"].astype(np.int64).astype(np.uint64)
if np.unique(key).size != key.size: sys.exit("FATAL: duplicate keys")
even = (d["EventNumber"].astype(np.int64) % 2) == 0
pre = d["passRecoEnv"] & (d["Q2s"] > 150) & (d["Ys"] > .1) & (d["Ys"] < .9) & (np.abs(d["hatVtxZ"]) < 35) & even
cr = pre & d["selFmd"].astype(bool) & (d["ePlug"] < 7) & d["selFts"].astype(bool)
t = f["ChargedTot"].arrays(["RunNumber", "EventNumber", "Empz"], library="np")
ck = (t["RunNumber"].astype(np.int64).astype(np.uint64) << np.uint64(32)) | t["EventNumber"].astype(np.int64).astype(np.uint64)
uk, ix = np.unique(ck, return_index=True); j = np.clip(np.searchsorted(uk, key), 0, uk.size - 1)
empz = np.where(uk[j] == key, t["Empz"][ix][j], np.nan); win = np.isfinite(empz) & (empz > 35) & (empz < 70)
gx = np.asarray(d["hatXpomGki"], float)
res = {}
for name, m in (("i_base", cr), ("ii_empz", cr & win), ("iii_empz_cand", cr & win & (d["xIp"] < 0.04) & (d["mx"] > 3))):
    ok = m & np.isfinite(gx)
    res[name] = {"n": int(m.sum()), "n_gen_xip_ge_0p03": int((ok & (gx >= 0.03)).sum()), "share": float((ok & (gx >= 0.03)).sum() / max(ok.sum(), 1))}
res["note"] = "H3 (iv) uses the (ii) payload; its share equals ii_empz"
json.dump(res, open(OUT, "w"), indent=1); print(json.dumps(res))
