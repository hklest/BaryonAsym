#!/usr/bin/env python3
"""WP-A2 S4 gate G1' evaluator (fork of wp-a/g1_eval_wpa.py usage; child plan D6, S4).

  prepare   : check the frozen s3_correction_record.json sha256 against --record-sha,
              link the odd-split n1_wpa2 partials (run with the frozen flags) into the
              layouts g1_eval_wpa.py and payload_wpa2.py read, build the frozen-definition
              odd payloads and write the S4 fit queue.
  evaluate  : run g1_eval_wpa.py (unchanged WP-A arithmetic: G1'(a) Plug N-1 shape and
              per-cell |f_fail,MC - f_fail,DATA|; G1'(b) admissible cocktail scan) per ND
              alternative, read the S4 fits for G1'(c), write g1prime_table.csv
              (alt, clause, cell, value, limit, pass) and print PASS/FAIL per clause and
              alternative.  Ruling input: PASS iff (a) and (c) pass for >= 1 of 7071,
              7091, nd4cat; (b) failing alone -> systematic leg.
The corrections (eta cut, threshold branch, fit range, noise removal) are applied
upstream through the frozen flags recorded in the correction record; this script
asserts that the inputs carry them."""
import argparse, csv, hashlib, json, os, subprocess, sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "wp-a"))
import wpa_common as W

S4 = W.D / "data/restart_2026-10-27/wp-a2/s4_g1p"
ALTS = ("7071", "7091", "nd4cat")
ND = {"7071": "7071", "7091": "7091", "nd4cat": "7071:7094:7096:7097"}
CELLS = ("D0", "D1", "D2", "ALL")


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def link(src, dst):
    dst.parent.mkdir(parents=True, exist_ok=True)
    if dst.is_symlink() or dst.exists():
        if os.path.realpath(dst) != os.path.realpath(src):
            raise SystemExit("FATAL: link mismatch " + str(dst))
        return
    os.symlink(src, dst)


def load_record(a):
    if sha(a.record) != a.record_sha:
        raise SystemExit("FATAL: correction record sha256 mismatch (frozen %s, found %s)" % (a.record_sha, sha(a.record)))
    r = json.load(open(a.record))
    return r, r["frozen_definition"]


def comps(ch):
    return W.DIFF8 + W.CHAINS[ch]["nd"]


def check_partial(p, fz, kind):
    x = json.load(open(p / "partial.json"))
    w2 = x["wpa2"]
    if w2["split"] != "odd" or abs(w2["eta_cut"] - fz["n1_eta_cut"]) > 1e-9:
        raise SystemExit("FATAL: %s not odd / eta_cut %s" % (p, w2.get("eta_cut")))
    if kind == "mc" and fz["noise_removal"] and not x["wpa"]["removal"]["applied"]:
        raise SystemExit("FATAL: remedy-1 not applied in " + str(p))


def prepare(a):
    r, fz = load_record(a)
    rows = []
    for ch in ALTS:
        for l in W.DATA_LABELS:
            src = S4 / "n1" / "data" / l; check_partial(src, fz, "data")
            link(src, S4 / "g1eval" / ch / "data" / l); link(src, S4 / "payload_root" / ch / "partials" / l / "odd")
        for l in comps(ch):
            # chain-specific partials only when a per-chain MC treatment (noise removal) is frozen
            src = S4 / "n1" / (ch if fz["noise_removal"] else "mc") / l; check_partial(src, fz, "mc")
            link(src, S4 / "g1eval" / ch / "mc" / l); link(src, S4 / "payload_root" / ch / "partials" / l / "odd")
        pl = S4 / "payloads" / ("odd_frozen_%s.json" % ch)
        cmd = [sys.executable, str(HERE / "payload_wpa2.py"), "--root", str(S4 / "payload_root" / ch), "--split", "odd",
               "--chain", ch, "--eta-branch", fz["fit_eta_branch"], "--out", str(pl)]
        if fz["fit_eta_max_cut"] is not None:
            cmd += ["--eta-max-cut", str(fz["fit_eta_max_cut"]), "--edge-merge"]
        if fz["fit_empz"]:
            cmd += ["--empz"]
        if fz["fit_cand"]:
            cmd += ["--cand"]
        subprocess.run(cmd, check=True)
        for b in ("fine", "coarse"):
            for s in CELLS:
                rows.append(",".join([ch, ND[ch], "-", b, s, str(pl), str(S4 / "fits"), "1000", str(fz["fit_templates"]), "s4", "3600"]))
    q = W.D / "condor/restart_2026-10-27/wp-a2/fit_wpa2/queue_s4.txt"
    q.write_text("\n".join(rows) + "\n")
    print(json.dumps({"queue": str(q), "n_jobs": len(rows), "payloads": {ch: sha(S4 / "payloads" / ("odd_frozen_%s.json" % ch)) for ch in ALTS}}))


def evaluate(a):
    r, fz = load_record(a)
    table = []; verdict = {}
    for ch in ALTS:
        out = S4 / "g1eval" / ch / "g1_eval.json"
        subprocess.run([sys.executable, str(HERE.parent / "wp-a" / "g1_eval_wpa.py"), "--data-dir", str(S4 / "g1eval" / ch / "data"),
                        "--mc-dir", str(S4 / "g1eval" / ch / "mc"), "--chain", ch, "--out", str(out)], check=True, stdout=subprocess.DEVNULL)
        g = json.load(open(out))
        pa = g["G1a"]
        table.append([ch, "a_shape", "ALL", pa["plug_n1_max_dev"], 0.10, bool(pa["shape_pass"])])
        for c in range(3):
            dlt = g["f_fail_plug"]["D%d" % c]["delta_plus_reg_" + ch]
            table.append([ch, "a_ffail", "D%d" % c, dlt, 0.02, abs(dlt) <= 0.02])
        pb = g["G1b"]
        table.append([ch, "b_best_joint", "ALL", pb["best_joint"]["max"], 0.25, bool(pb["PASS"])])
        cp = []
        for b, fb in (("fine", "fine"), ("coarse", "coarse_ge100_data")):
            for s in CELLS:
                f = S4 / "fits" / ("fit_summary_%s_%s_%s.csv" % (ch, b, s))
                if not f.exists():
                    raise SystemExit("FATAL: missing fit " + str(f))
                row = [x for x in csv.DictReader(open(f)) if x["binning"] == fb and x["scope"] == s]
                if len(row) != 1:
                    raise SystemExit("FATAL: fit row " + str(f))
                p = float(row[0]["toy_p"])
                fj = json.load(open(S4 / "fits" / ("section_7_1_twofree_fit_results_%s_%s_%s.json" % (ch, b, s))))
                g_ = fj[fb][ch][s]["template_statistics_aware_gof"]; nt = int(g_["n_toys"]); floor = abs(p - g_["p_floor"]) < 1e-12
                table.append([ch, "c_toy_p_%s" % b, s, p, 0.05, p >= 0.05, nt, floor]); cp.append(p >= 0.05)
                table.append([ch, "c_D_over_ndf_%s" % b, s, "%s/%s" % (round(float(row[0]["deviance"]), 1), row[0]["ndf"]), "", ""])
        A = bool(pa["PASS"]); B = bool(pb["PASS"]); C = all(cp)
        verdict[ch] = {"a": A, "b": B, "c": C}
    out = S4 / "g1prime_table.csv"
    with open(out, "w", newline="") as f:
        w = csv.writer(f); w.writerow(["alt", "clause", "cell", "value", "limit", "pass", "n_toys", "at_floor"]); w.writerows([t + [""] * (8 - len(t)) for t in table])
    ruling = "PASS" if any(v["a"] and v["c"] for v in verdict.values()) else "KILL"
    if ruling == "PASS" and not any(v["a"] and v["b"] and v["c"] for v in verdict.values()):
        ruling = "PASS with systematic leg"
    for ch, v in verdict.items():
        print("%s: (a) %s  (b) %s  (c) %s" % (ch, *("PASS" if v[k] else "FAIL" for k in "abc")))
    print("G1' ruling input:", ruling, "| table", out, "sha256", sha(out))
    json.dump({"verdict": verdict, "ruling_input": ruling, "table": str(out), "table_sha256": sha(out), "record_sha256": a.record_sha},
              open(S4 / "g1prime_summary.json", "w"), indent=1)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=("prepare", "evaluate"))
    ap.add_argument("--record", default=str(W.D / "data/restart_2026-10-27/wp-a2/s3_corr/s3_correction_record.json"))
    ap.add_argument("--record-sha", required=True)
    a = ap.parse_args()
    prepare(a) if a.mode == "prepare" else evaluate(a)


if __name__ == "__main__":
    main()
