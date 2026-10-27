#!/usr/bin/env python3
"""WP-A S1 checks and tables (JSON/npy only, login node).
Tests: split_complete, control_disjoint (keys), per-file (Run,Event) uniqueness;
tables: Plug N-1 f_fail per period/sub-period and D cell (data raw; MC per
component by own RunNumber period, signed), control-region counts and
per-detector fail fractions.  fail_fraction_recomputed runs in g1_eval_wpa.py
--check-t4 on split=all."""
import json, math, sys
from pathlib import Path
import numpy as np
import wpa_common as W

S1 = W.OUT / "s1_split/partials"; S1C = W.OUT / "s1_control/partials/control"
LABELS = W.DIFF8 + ["7071", "7091", "7094", "7096", "7097"] + W.DATA_LABELS


def main():
    out = {"split_complete": {}, "control_disjoint": {}, "unique": {}, "unmapped_pre0": {}}
    ok = True
    for l in LABELS:
        A = W.load_partial(S1 / "all", l); T = W.load_partial(S1 / "test", l); C = W.load_partial(S1C, l)
        bad = []
        for m in A["counts"]:
            if A["counts"][m]["raw_int"] != T["counts"][m]["raw_int"] + C["counts"][m]["raw_int"]:
                bad.append(m)
            if any(a != t + c for a, t, c in zip(A["counts"][m]["Dcells_raw"], T["counts"][m]["Dcells_raw"], C["counts"][m]["Dcells_raw"])):
                bad.append(m + "/cells")
        out["split_complete"][l] = "PASS" if not bad else bad
        kt = np.load(S1 / "test" / l / "keys_pre.npy"); kc = np.load(S1C / l / "keys_pre.npy")
        inter = int(np.intersect1d(kt, kc).size)
        out["control_disjoint"][l] = {"n_test": int(kt.size), "n_control": int(kc.size), "intersection": inter,
                                      "n_all_pre": A["wpa"]["n_pre0"], "sum_ok": int(kt.size + kc.size) == A["wpa"]["n_pre0"]}
        out["unique"][l] = A["wpa"]["run_event_unique"]; out["unmapped_pre0"][l] = A["wpa"]["unmapped_pre0"]
        ok &= (not bad) and inter == 0 and out["control_disjoint"][l]["sum_ok"] and out["unique"][l]
    # f_fail tables (split=all, stack C)
    tab = {}
    for l in LABELS:
        A = W.load_partial(S1 / "all", l); ps = A["wpa"]["period_stats"]; tab[l] = {}
        w = "raw" if l in W.DATA_LABELS else "signed"
        for g, s in ps.items():
            row = {}
            for c in ("D0", "D1", "D2"):
                n, p = s["C_N1_PLUG"][c], s["C_SB"][c]
                if n["raw"] == 0:
                    continue
                f = 1 - p[w] / n[w] if n[w] else None
                row[c] = {"f": f, "n_raw": n["raw"], "err": math.sqrt(f * (1 - f) / n["raw"]) if f is not None else None}
            if row:
                tab[l][g] = row
    ctrl = {}
    for l in LABELS:
        C = W.load_partial(S1C, l)
        ctrl[l] = {g: {"n_ctrl": v["ctrl"]["raw"], "f_stack_raw": 1 - v["ctrl_stack_pass"]["raw"] / v["ctrl"]["raw"],
                       "f_plug_raw": v["ctrl_plug_fail"]["raw"] / v["ctrl"]["raw"], "f_fmd_raw": v["ctrl_fmd_fail"]["raw"] / v["ctrl"]["raw"],
                       "f_fts_raw": v["ctrl_fts_fail"]["raw"] / v["ctrl"]["raw"]}
                   for g, v in C["wpa"]["control_stats"].items() if v["ctrl"]["raw"] > 0}
    out["f_fail_plug_C"] = tab; out["control"] = ctrl; out["ALL_TESTS_PASS"] = bool(ok)
    o = W.OUT / "s1_split/analysis/s1_checks.json"; o.parent.mkdir(parents=True, exist_ok=True)
    json.dump(out, open(o, "w"), indent=1)
    print(json.dumps({"ALL_TESTS_PASS": ok, "split_complete": out["split_complete"], "disjoint": {l: v["intersection"] for l, v in out["control_disjoint"].items()},
                      "unmapped": out["unmapped_pre0"]}))
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
