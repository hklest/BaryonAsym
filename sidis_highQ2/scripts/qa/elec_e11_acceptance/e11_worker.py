#!/usr/bin/env python3
"""E_e'(gen)<11 GeV corner study (2026-10-29). Per gen bin (frozen 27b grid, builder's own compute_gen_bin):
gen weight split by GenElecE<11 / >=11, and the event-level reco pass (event present in ChargedTot with
150<Q2es<20000, 0.1<Yes<0.7, Empz>45; ChargedTot is written only for events passing reco E_e'>11).
Args: list_file out_npz [max_files]"""
import sys, numpy as np, uproot
sys.path.insert(0, "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/cone_s2prime_optA_2026-10-19")
import build_migration_th2_per_charge_cone_s2prime_optA_esxf as B
from esigma_xf import gen_truth
NG = B.NB_G
files = [l.strip() for l in open(sys.argv[1]) if l.strip()]
if len(sys.argv) > 3: files = files[:int(sys.argv[3])]
# acc[charge, cls, bin]: cls 0 gen_ge,1 gen_lt,2 reco_ge,3 reco_lt,4 gen_lt_isr,5 gen_all_isr ; w2 likewise
acc = np.zeros((2, 6, NG)); acc2 = np.zeros((2, 6, NG))
ebins = np.linspace(0, 40, 81); eh = np.zeros((2, 2, NG, 80))  # [charge, recopass, bin, E]
def key(r, e): return r.astype(np.int64) * 10**9 + e.astype(np.int64)
for fn in files:
    f = uproot.open(fn)
    ct = f["ChargedTot"].arrays(["RunNumber", "EventNumber", "Q2es", "Yes", "Empz"], library="np")
    ok = (ct["Q2es"] > 150) & (ct["Q2es"] < 20000) & (ct["Yes"] > 0.1) & (ct["Yes"] < 0.7) & (ct["Empz"] > 45)
    rk = np.unique(key(ct["RunNumber"][ok], ct["EventNumber"][ok]))
    t = f["ChargedTotGen"]
    br = [b for b in B.CTG_BRANCHES if b in t.keys()]
    for batch in t.iterate(br, library="np", step_size="200 MB"):
        i, w, ch, _ = B.FileAccumulator.compute_gen_bin(None, batch)
        tr = gen_truth(batch, "ChargedTotGen")
        sel = i >= 0
        E = batch["GenElecE"].astype(float)
        lt = (E < 11.0)
        rp = np.isin(key(batch["RunNumber"], batch["EventNumber"]), rk)
        isr = np.asarray(tr["isr"], bool)
        for c in (0, 1):
            m = sel & (ch == c)
            for cls, mm in ((0, m & ~lt), (1, m & lt), (2, m & ~lt & rp), (3, m & lt & rp), (4, m & lt & isr), (5, m & isr)):
                acc[c, cls] += np.bincount(i[mm], w[mm], NG); acc2[c, cls] += np.bincount(i[mm], w[mm]**2, NG)
            for r in (0, 1):
                mm = m & (rp == bool(r))
                eb = np.clip(np.digitize(E[mm], ebins) - 1, 0, 79)
                np.add.at(eh[c, r], (i[mm], eb), w[mm])
    print("done", fn, flush=True)
np.savez(sys.argv[2], acc=acc, acc2=acc2, eh=eh, ebins=ebins, nfiles=len(files))
