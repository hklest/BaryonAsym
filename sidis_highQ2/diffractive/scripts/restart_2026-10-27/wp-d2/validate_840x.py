#!/usr/bin/env python3
"""WP-D2 validation of merged v6dif RAPGAP 840x trees (8402-8405) against 9012. Read-only."""
import sys, json, hashlib, csv, collections
import numpy as np, uproot
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive"
M = D + "/data/t2_detector/mc840x_merged_2026-10-28"
REF = D + "/data/t2_detector/mc9012_merged_2026-10-18/9012.root"
OUT = D + "/data/restart_2026-10-27/wp-d2/validation_840x.json"
SAMPLES = ["8402", "8403", "8404", "8405"]
REC_DIFF = dict(zip(SAMPLES, [401281, 401661, 402098, 401708]))
REC_ENV = dict(zip(SAMPLES, [68492, 68932, 68735, 67545]))
PLAN = {"8402": (357157, 367327, 357160, 392213), "8403": (398383, 436886, 398383, 436886),
        "8404": (444315, 447893, 444312, 466997), "8405": (468532, 500611, 468532, 500611)}
R = {"checks": {}}

def sha(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 24), b""): h.update(b)
    return h.hexdigest()

def types(t): return {k: str(v.typename) for k, v in t.items()}
def base_keys(f): return collections.Counter(k.split(";")[0] for k in f.keys())

ref = uproot.open(REF)
rtypes = {n: types(ref[n]) for n in ("DiffTot", "ChargedTot", "ChargedTotGen")}
rkeys = base_keys(ref)
# 1/2/3: integrity, counts, parity
shas = {l.split()[1]: l.split()[0] for l in open(M + "/sha256.txt") if l.strip()}
rows = list(csv.DictReader(open(M + "/tally_840x.csv")))
c1, c2, c3 = {}, {}, {}
for s in SAMPLES:
    p = f"{M}/{s}.root"
    r = {"sha_ok": sha(p) == shas[f"{s}.root"]}
    f = uproot.open(p)
    kc = base_keys(f)
    r["trees"] = {n: n in kc for n in ("DiffTot", "ChargedTot", "ChargedTotGen")}
    r["tree_cycles"] = {n: kc[n] for n in ("DiffTot", "ChargedTot", "ChargedTotGen")}
    r["ref_tree_cycles"] = {n: rkeys[n] for n in ("DiffTot", "ChargedTot", "ChargedTotGen")}
    r["key_total"] = sum(kc.values()); r["ref_key_total"] = sum(rkeys.values())
    r["keys_missing_vs_9012"] = sorted(set(rkeys) - set(kc)); r["keys_extra_vs_9012"] = sorted(set(kc) - set(rkeys))
    r["recovered_or_zombie"] = False
    try:
        for n in ("DiffTot", "ChargedTot", "ChargedTotGen"): f[n].num_entries
    except Exception as e:
        r["recovered_or_zombie"] = True; r["err"] = str(e)
    c1[s] = r
    tr = [x for x in rows if x["sample"] == s]
    nd = f["DiffTot"].num_entries
    pe = f["DiffTot"]["passRecoEnv"].array(library="np")
    c2[s] = {"DiffTot_entries": nd, "tally_DiffTot": sum(int(x["DiffTot"]) for x in tr), "record_DiffTot": REC_DIFF[s],
             "ChargedTot_entries": f["ChargedTot"].num_entries, "ChargedTotGen_entries": f["ChargedTotGen"].num_entries,
             "passRecoEnv": int(pe.sum()), "tally_reco_env": sum(int(x["reco_env"]) for x in tr), "record_reco_env": REC_ENV[s],
             "chunks": len(tr), "rc0": sum(int(x["rc"]) == 0 for x in tr), "verdict_PASS": sum(x["verdict"] == "PASS" for x in tr),
             "tally_evls": sum(int(x["evls"]) for x in tr)}
    c2[s]["ok"] = (nd == c2[s]["tally_DiffTot"] == REC_DIFF[s] and c2[s]["passRecoEnv"] == c2[s]["tally_reco_env"] == REC_ENV[s])
    p3 = {}
    for n in ("DiffTot", "ChargedTot", "ChargedTotGen"):
        t = types(f[n]); rt = rtypes[n]
        p3[n] = {"missing": sorted(set(rt) - set(t)), "extra": sorted(set(t) - set(rt)),
                 "type_diff": {k: [rt[k], t[k]] for k in rt if k in t and rt[k] != t[k]}}
    c3[s] = p3
R["checks"]["1_integrity"] = c1; R["checks"]["2_counts"] = c2; R["checks"]["3_parity"] = c3
R["checks"]["2_counts_global"] = {"chunks": len(rows), "rc0": sum(int(x["rc"]) == 0 for x in rows), "PASS": sum(x["verdict"] == "PASS" for x in rows)}

# 4/5/6/7
def read_diff(p):
    t = uproot.open(p)["DiffTot"]
    sc = [k for k, v in t.items() if v.typename in ("float", "double", "int32_t", "bool")]
    return t, sc
def sentinel_fracs(t, cols, mask):
    out = {}
    for k in cols:
        a = t[k].array(library="np")[mask].astype(float)
        if len(a) == 0: out[k] = None; continue
        out[k] = float(np.mean((a <= -900) | ~np.isfinite(a)))
    return out
c4, c5, c6, first = {}, {}, {}, {}
NF = 30000
for s in SAMPLES + ["9012"]:
    p = REF if s == "9012" else f"{M}/{s}.root"
    t, cols = read_diff(p)
    pe = t["passRecoEnv"].array(library="np").astype(bool)
    c6[s] = sentinel_fracs(t, cols, pe)
    if s == "9012": continue
    ev = t["EventNumber"].array(library="np"); rn = t["RunNumber"].array(library="np")
    w1 = t["weight1"].array(library="np"); w2 = t["weight2"].array(library="np"); z = t["hatVtxZ"].array(library="np")
    key = rn.astype(np.int64) * (1 << 32) + ev.astype(np.int64)
    c4[s] = {"weight1_min": float(w1.min()), "weight1_max": float(w1.max()), "weight1_all_one": bool(np.all(w1 == 1)),
             "weight2_min": float(w2.min()), "weight2_max": float(w2.max()), "weight2_mean": float(w2.mean()),
             "note": "skim multiplies weight1*weight2",
             "unique_run_event": int(len(np.unique(key))), "entries": int(len(key)), "unique_ok": bool(len(np.unique(key)) == len(key)),
             "unique_EventNumber_only": int(len(np.unique(ev)))}
    lo, hi, glo, ghi = PLAN[s]
    c5[s] = {"run_min": int(rn.min()), "run_max": int(rn.max()), "plan_expect": [lo, hi], "plan_window": [glo, ghi],
             "within_window": bool(rn.min() >= glo and rn.max() <= ghi), "hatVtxZ_mean": float(z.mean()), "hatVtxZ_rms": float(z.std()),
             "hatVtxZ_mean_recoenv": float(z[pe].mean()), "hatVtxZ_rms_recoenv": float(z[pe].std())}
    first[s] = {k: t[k].array(entry_stop=NF, library="np") for k in ("EventNumber", "RunNumber", "hatMXGen", "hatTGen", "hatXpomGki", "Q2s", "passRecoEnv")}
R["checks"]["4_weights_uniqueness"] = c4; R["checks"]["5_runs"] = c5
fl = {}
for k in c6["9012"]:
    row = {s: c6[s].get(k) for s in SAMPLES + ["9012"]}
    if any(v is not None and v > 0.05 for v in row.values()): fl[k] = row
R["checks"]["6_sentinel"] = {"branches_above_5pct": fl, "n_branches": len(c6["9012"]), "note": "reco-env subset, sentinel <=-900 or non-finite"}

def twin(a, b):
    A, B = first[a], first[b]
    out = {"first_n": int(min(len(A["EventNumber"]), len(B["EventNumber"])))}
    n = out["first_n"]
    out["positional_EventNumber_equal_frac"] = float(np.mean(A["EventNumber"][:n] == B["EventNumber"][:n]))
    ia = {int(e): i for i, e in reversed(list(enumerate(A["EventNumber"])))}
    ib = {int(e): i for i, e in reversed(list(enumerate(B["EventNumber"])))}
    common = sorted(set(ia) & set(ib))
    out["EventNumber_in_both_frac_of_a"] = len(common) / len(ia)
    ii = np.array([ia[e] for e in common]); jj = np.array([ib[e] for e in common])
    for k, tol in (("hatMXGen", 1e-4), ("hatTGen", 1e-4), ("hatXpomGki", 1e-4)):
        x, y = A[k][ii], B[k][jj]
        out[k + "_match_frac_of_matched"] = float(np.mean(np.isclose(x, y, rtol=tol, atol=1e-6)))
        out[k + "_match_frac_of_first_n"] = float(np.sum(np.isclose(x, y, rtol=tol, atol=1e-6)) / n)
    both = A["passRecoEnv"][ii].astype(bool) & B["passRecoEnv"][jj].astype(bool)
    out["Q2s_reco_match_frac_both_recoenv"] = float(np.mean(np.isclose(A["Q2s"][ii][both], B["Q2s"][jj][both], rtol=1e-4))) if both.any() else None
    out["passRecoEnv_agree_frac"] = float(np.mean(A["passRecoEnv"][ii] == B["passRecoEnv"][jj]))
    return out
R["checks"]["7_twins"] = {"method": f"chunk membership not recoverable; first {NF} DiffTot entries matched on EventNumber, gen quantities hatMXGen/hatTGen/hatXpomGki; Q2s (reco) compared where both reco-env",
                          "8402_vs_8405": twin("8402", "8405"), "8403_vs_8404": twin("8403", "8404")}
json.dump(R, open(OUT, "w"), indent=1)
print("wrote", OUT)
