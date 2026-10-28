#!/usr/bin/env python3
"""WP-A6 edge30 figure: f_ND per M_X bin and chain at SB (ec08 < 3.3) and SB30 (ec08 < 3.0), odd and even splits, with
sigma_G2 bars, the in-fit part as an open marker and the 0.30 ceiling.  Reads edge30/f_nd_per_mx_edge30.csv.  Login node."""
import csv
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
SRC = D / "data/restart_2026-10-27/wp-a6/edge30/f_nd_per_mx_edge30.csv"; PD = D / "plots/restart_2026-10-27/wp-a6/edge30"
MX = ["MX4_10", "MX10_20", "MX20_40"]; LAB = ["[4,10)", "[10,20)", "[20,40]"]; CH = ["7071", "7091", "nd4cat"]; COL = ["C0", "C1", "C2"]


def main():
    R = {(r["split"], r["sel"], r["mx_bin"], r["chain"]): r for r in csv.DictReader(open(SRC))}
    fig, axs = plt.subplots(1, 2, figsize=(11, 4.2), sharey=True)
    for ax, sp in zip(axs, ("odd", "even")):
        for i, m in enumerate(MX):
            for j, ch in enumerate(CH):
                for k, (sel, mk) in enumerate((("sel", "s"), ("sel30", "o"))):
                    r = R[sp, sel, m, ch]; x = i + (j - 1) * 0.22 + (k - 0.5) * 0.09
                    ax.errorbar(x, float(r["f_nd"]), yerr=float(r["sig_g2"]), fmt=mk, color=COL[j], ms=5, capsize=2,
                                mfc=COL[j] if sel == "sel30" else "white", label=f"{ch} {'ec08<3.0' if sel == 'sel30' else 'ec08<3.3'}" if i == 0 else None)
                    ax.plot(x, float(r["f_nd_infit"]), "_", color=COL[j], ms=8)
        ax.axhline(0.30, color="k", ls="--", lw=1); ax.set_xticks(range(3)); ax.set_xticklabels(LAB); ax.set_xlabel("M_X [GeV]")
        ax.set_title(f"{sp} split"); ax.grid(alpha=0.3)
    axs[0].set_ylabel("f_ND (bar: sigma_G2; dash: in-fit part)"); axs[1].legend(fontsize=7, ncol=2)
    fig.tight_layout(); PD.mkdir(parents=True, exist_ok=True)
    fig.savefig(PD / "fnd_per_mx_edge30.pdf"); fig.savefig(PD / "fnd_per_mx_edge30.png", dpi=130); plt.close(fig)


if __name__ == "__main__":
    main()
