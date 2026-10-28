#!/usr/bin/env python3
"""WP-C C1: provisional hashed mask on the frozen grid (C1-14), make_blind_mask.py schema keyed (charge, hemi, beta_bin, axis, bin).
MC-only: reads binstats_c1_A.csv, grid_c1.json (+ optional veto_syst_c1.csv); opens no data file.
Usage: c1_mask.py [--veto veto_syst_c1.csv] [--out mask.json] [--date YYYY-MM-DD]"""
import argparse, csv, datetime, hashlib, json, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import c1_lib as C


def main(argv=None):
    ap = argparse.ArgumentParser(); ap.add_argument("--veto", default=None); ap.add_argument("--out", default=None)
    ap.add_argument("--date", default=datetime.date.today().isoformat()); ap.add_argument("--table", default=str(C.W / "binstats_c1_A.csv"))
    a = ap.parse_args(argv if argv is not None else [])
    grid = json.load(open(C.W / "grid_c1.json")); sel = {n: v["pt2_grid"] for n, v in grid["cells"].items()}
    veto = {}
    if a.veto:
        for r in csv.DictReader(open(a.veto)):
            veto[(r["charge"], r["cell_name"], r["axis"], int(r["bin"]))] = float(r["sigma_rel"])
    out = a.out or str(C.W / ("mask_c1_%s_standin%s.json" % (a.date, "" if a.veto else "_d7plus")))
    bins = []
    for r in csv.DictReader(open(a.table)):
        if r["axis"] == "pt2" and r["grid"] != sel[r["cell_name"]]: continue
        why = [x for x in r["reasons"].split(";") if x]
        vs = veto.get((r["charge"], r["cell_name"], r["axis"], int(r["bin"])))
        if vs is not None and not vs <= C.THR["veto"]: why.append("veto_syst")
        p, s = float(r["purity"]), float(r["stability"])
        bins.append(dict(charge=r["charge"], hemi=r["hemi"], beta_bin=int(r["beta_bin"]), axis=r["axis"], grid=r["grid"], bin=int(r["bin"]),
                         lo=float(r["lo"]), hi=(None if r["hi"] == "inf" else float(r["hi"])), n_sig_pred=float(r["n_sig_pred"]), purity=p, stability=s,
                         p_mig=float(r["p_mig"]), f_fake=float(r["f_fake"]), f_nd=float(r["f_nd"]), veto_sigma_rel=vs, masked=bool(why), reasons=why,
                         marginal=(not why) and (p < 0.40 or s < 0.40)))
    doc = dict(rule="D7+ with the C1-7 additions (N>=50, purity>=0.35, stability>=0.35, p_mig>=0.40, f_fake<=0.35, >=100 raw MC per 2x reco sub-bin, "
                   ">=500 raw 840x per gen bin and >=20 per used response element, inter-cell migration<0.5, width>=2 sigma_core and >=1.5(sigma_core+|bias|))"
                   + ("; veto_syst: WP-A5 sigma_rel>0.05" if a.veto else "; veto_syst not yet applied"),
               source_table=a.table, source_sha256=C.sha256(a.table), grid_sha256=C.sha256(C.W / "grid_c1.json"),
               f_nd_source="W/wp-a6/edge30/f_nd_per_mx_edge30.csv largest chain, odd split, SB30 (stand-in; A56' re-fill pending)",
               veto_source=(a.veto or None), data_read=False, data_files_opened=[],
               created_utc=datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
               key="(charge, hemi, beta_bin, axis, bin)", n_bins=len(bins), n_masked=sum(b["masked"] for b in bins), n_marginal=sum(b["marginal"] for b in bins), bins=bins)
    blob = json.dumps(doc, indent=1, sort_keys=True).encode(); Path(out).write_bytes(blob)
    Path(out + ".sha256").write_text("%s  %s\n" % (hashlib.sha256(blob).hexdigest(), out))
    print(out, hashlib.sha256(blob).hexdigest(), "masked", doc["n_masked"], "marginal", doc["n_marginal"], "of", len(bins))
    return out


if __name__ == "__main__":
    main(sys.argv[1:])
