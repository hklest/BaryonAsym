#!/usr/bin/env python3
"""Merge e11_worker outputs, compute per-bin corner metrics, compare RG31/DJ14, write CSV + plot."""
import glob, json, csv, numpy as np
D = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/scripts/qa/elec_e11_acceptance/out"
P = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/plots/qa/elec_e11_acceptance"
REL = "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/output_v6/central/data_release/bbb_ibu_bgsub_setRprime_conebase_optA_q2esgen_esigmaphotonxf_S3xmerged_zlow005_pT5_eta1p6_radlen5_nhit5_2026-10-27b"
G = {}
for g in ("rapgap31", "djangoh14"):
    fs = sorted(glob.glob(f"{D}/{g}_*.npz")); a = 0; a2 = 0; eh = 0
    for f in fs:
        d = np.load(f); a = a + d["acc"]; a2 = a2 + d["acc2"]; eh = eh + d["eh"]
    G[g] = dict(acc=a, acc2=a2, eh=eh, n=len(fs)); print(g, len(fs), "shards")
rows = list(csv.DictReader(open(f"{REL}/ibu/cross_sections.csv")))
xc = {(c, t): json.load(open(f"{REL}/_run/bbb_ibu_crosscheck_{t}_{c}.json")) for c in ("pos", "neg") for t in ("rapgap", "djangoh")}
def met(g, c):
    a, a2 = G[g]["acc"][c], G[g]["acc2"][c]
    ge, lt, rge, rlt = a[0], a[1], a[2], a[3]
    gt = ge + lt
    with np.errstate(all="ignore"):
        f_lt = lt / gt; eps_ge = rge / ge; eps_lt = rlt / lt
        X = np.where(lt > 0, (lt - np.where(eps_ge > 0, rlt / eps_ge, 0)) / gt, 0.0)
        sX = np.sqrt(a2[1]) / gt  # stat of lt share (approx)
        isr = a[4] / lt
    return dict(f_lt=f_lt, eps_ge=eps_ge, eps_lt=np.nan_to_num(eps_lt), X=X, sX=sX, isr=np.nan_to_num(isr), gt=gt)
out = []; summ = {}
for ci, c in enumerate(("pos", "neg")):
    R, Dj = met("rapgap31", ci), met("djangoh14", ci)
    uR = np.array(xc[(c, "rapgap")]["methods"]["IBU"]["unfolded_selected"]); uD = np.array(xc[(c, "djangoh")]["methods"]["IBU"]["unfolded_selected"])
    aR = np.array(xc[(c, "rapgap")]["response_aux"]["acceptance_per_gen"]); aD = np.array(xc[(c, "djangoh")]["response_aux"]["acceptance_per_gen"])
    rr = [r for r in rows if r["charge"] == c]
    for b in range(245):
        r = rr[b]
        out.append(dict(charge=c, bin=b, cell=r["cell_label"], z=f'{r["z_min"]}-{r["z_max"]}', pT=f'{r["pT_min"]}-{r["pT_max"]}',
            sigma_nb=float(r["cross_section_nb"]), tot_unc_rel=float(r["total_unc_rel"]), model_unc_rel=float(r["model_unc_nb"]) / float(r["cross_section_nb"]),
            f_lt_RG=R["f_lt"][b], f_lt_DJ=Dj["f_lt"][b], eps_lt_over_ge_RG=R["eps_lt"][b] / R["eps_ge"][b], eps_lt_over_ge_DJ=Dj["eps_lt"][b] / Dj["eps_ge"][b],
            X_RG=R["X"][b], X_DJ=Dj["X"][b], dX=R["X"][b] - Dj["X"][b], sX_RG=R["sX"][b], isr_frac_lt_RG=R["isr"][b],
            acc_RG=aR[b], acc_DJ=aD[b], ibu_RG_minus_DJ_rel=(uR[b] - uD[b]) / uR[b]))
    w = np.array([float(r["cross_section_nb"]) for r in rr])
    summ[c] = dict(integrated_X_RG=float((w * R["X"]).sum() / w.sum()), integrated_X_DJ=float((w * Dj["X"]).sum() / w.sum()),
                   integrated_f_lt_RG=float((w * R["f_lt"]).sum() / w.sum()),
                   n_X_gt_0p5=int((R["X"] > 0.5).sum()), n_X_gt_0p1=int((R["X"] > 0.1).sum()), n_X_gt_0p01=int((R["X"] > 0.01).sum()),
                   max_X_RG=float(R["X"].max()), max_X_DJ=float(Dj["X"].max()))
with open(f"{P}/e11_corner_per_bin.csv", "w") as fh:
    wr = csv.DictWriter(fh, fieldnames=list(out[0].keys())); wr.writeheader()
    for o in out: wr.writerow({k: (f"{v:.5g}" if isinstance(v, float) else v) for k, v in o.items()})
json.dump(summ, open(f"{P}/e11_corner_summary.json", "w"), indent=1); print(json.dumps(summ, indent=1))
# per-cell aggregates
cells = sorted(set(o["cell"] for o in out), key=lambda s: [o["cell"] for o in out].index(s))
for c in ("pos", "neg"):
    for cl in cells:
        sel = [o for o in out if o["charge"] == c and o["cell"] == cl]; w = np.array([o["sigma_nb"] for o in sel])
        f = lambda k: (w * np.array([o[k] for o in sel])).sum() / w.sum()
        print(c, cl, "f_lt RG %.4f DJ %.4f | X RG %.4f DJ %.4f | maxX %.4f | eps_lt/ge RG %.3f" % (f("f_lt_RG"), f("f_lt_DJ"), f("X_RG"), f("X_DJ"), max(o["X_RG"] for o in sel), np.nanmean([o["eps_lt_over_ge_RG"] for o in sel])))
import matplotlib; matplotlib.use("Agg"); import matplotlib.pyplot as plt
fig, ax = plt.subplots(2, 1, figsize=(11, 7), sharex=True)
for i, c in enumerate(("pos", "neg")):
    sel = [o for o in out if o["charge"] == c][:70]
    x = np.arange(len(sel))
    ax[i].errorbar(x, [o["X_RG"] for o in sel], [o["sX_RG"] for o in sel], fmt="o", ms=3, label="RAPGAP31 X")
    ax[i].plot(x, [o["X_DJ"] for o in sel], "s", ms=3, mfc="none", label="DJANGOH14 X")
    ax[i].plot(x, [o["f_lt_RG"] for o in sel], "_", ms=8, label="RAPGAP31 f(E_gen<11)")
    ax[i].axvline(34.5, color="k", lw=0.5); ax[i].set_ylabel(f"fraction ({'h+' if c=='pos' else 'h-'})"); ax[i].legend(fontsize=8)
    ax[i].text(10, ax[i].get_ylim()[1] * 0.9, "S0_x0"); ax[i].text(45, ax[i].get_ylim()[1] * 0.9, "S0_x1")
ax[1].set_xlabel("gen bin index (S0: 150<Q2<300; 35 z x pT bins per cell)")
fig.suptitle("E_e'(gen)<11 GeV corner: gen fraction and extrapolated share X = f_lt(1-eps_lt/eps_ge), esigmaphotonxf_2026-10-27b grid")
fig.tight_layout(); fig.savefig(f"{P}/e11_corner_S0.png", dpi=110)
