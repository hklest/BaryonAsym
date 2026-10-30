#!/usr/bin/env python3
"""Redraw figures/HERA_xQ2_virtual_photon_eta.pdf of the high-Q2 analysis note.

Analytic (x_Bj, Q2) map: isolines of constant virtual-photon pseudorapidity
eta_gamma*, isolines of constant y, the y=1 boundary and the measurement phase
space (150<Q2<20000 GeV2, 0.1<y<0.7). No event data; runs in seconds on the
login node.

FRAME: eta is computed explicitly in the H1 LAB frame, +z = proton direction.
  P = (Ep,0,0,+Ep), k = (Ee,0,0,-Ee), q = k - k'.
  From y = P.q/P.k and q^2 = -Q2, with qT^2 = Q2 (1-y) (massless electron):
      q0 - qz = 2 y Ee ,   q0 + qz = -Q2/(2 Ee)   (so qz = (q0+qz - (q0-qz))/2)
  eta = atanh(qz/|q|) = -ln tan(theta/2), theta = polar angle w.r.t. +z.
  eta < 0 therefore means the photon points into the electron-beam (backward)
  hemisphere, as expected.
The old figure (Matplotlib 3.8.3 Mac output, producer not on NAF) was checked
against this: e.g. at x=1 the old eta=-0.2 isoline sits at Q2~450 and eta=-0.5
at Q2~3000, matching eta_photon(1,450)=-0.197 and eta_photon(1,3000)=-0.498
here, so the old figure was in the lab frame. `--check` verifies the closed
form against an explicit 4-vector (k'^2=0, q^2=-Q2, eta=-ln tan(theta/2)).
"""
import argparse, os
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

EE, EP = 27.6, 920.0
S = 4 * EE * EP
Q2MIN, Q2MAX, YMIN, YMAX = 150.0, 20000.0, 0.1, 0.7
OUT = ("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/analysis_note/"
       "H1-ChargedSIDIS-AnalysisNote-HighQ/figures/HERA_xQ2_virtual_photon_eta.pdf")
OR, BL, GR = "#d95f02", "#1f77b4", "#119a6b"


def eta_photon(x, q2):
    """Lab-frame (proton +z) pseudorapidity of the virtual photon."""
    y = q2 / (S * x)
    qm = 2 * y * EE            # q0 - qz
    qp = -q2 / (2 * EE)        # q0 + qz
    q0, qz = 0.5 * (qp + qm), 0.5 * (qp - qm)
    qt2 = q2 * (1 - y)
    p = np.sqrt(qz**2 + qt2)
    return 0.5 * np.log((p + qz) / (p - qz))


plt.rcParams.update({"font.size": 13})


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default=OUT)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    if a.check:
        # independent 4-vector check: build k' with given (y,Q2), phi=0
        x, q2 = 1e-2, 1000.0
        y = q2 / (S * x)
        qt = np.sqrt(q2 * (1 - y))
        # k' = k - q ; solve q from the two light-cone relations
        qm, qp = 2 * y * EE, -q2 / (2 * EE)
        qz = 0.5 * (qp - qm)
        ke = np.array([EE, 0, 0, -EE])
        q = np.array([0.5 * (qp + qm), qt, 0, qz])
        kp = ke - q
        print("k'^2 =", kp[0]**2 - kp[1]**2 - kp[3]**2, "(0 expected)")
        print("q^2  =", q[0]**2 - qt**2 - qz**2, "vs -Q2 =", -q2)
        th = np.arctan2(qt, qz)
        print("eta (formula) =", eta_photon(x, q2), " eta=-ln tan(th/2) =", -np.log(np.tan(th / 2)))
        return

    fig, ax = plt.subplots(figsize=(9.675, 7.751))
    xs = np.logspace(-5, 0, 800)
    xlo, xhi, ylo, yhi = 1e-5, 1.0, 1.0, 1e5
    for e in (-0.2, -0.5, -1.0, -2.0):
        np.seterr(all="ignore")
        # solve eta(x,Q2)=e for Q2 at each x by bisection in log Q2
        lo = np.full_like(xs, np.log(1e-3)); hi = np.log(S * xs)
        for _ in range(60):
            mid = 0.5 * (lo + hi)
            v = eta_photon(xs, np.exp(mid))
            # eta decreases (more negative) with rising Q2 at fixed x
            tooneg = (v < e) | ~np.isfinite(v)
            hi = np.where(tooneg, mid, hi); lo = np.where(tooneg, lo, mid)
        q2 = np.exp(0.5 * (lo + hi))
        ok = np.isfinite(eta_photon(xs, q2)) & (abs(eta_photon(xs, q2) - e) < 1e-3) & (q2 < S * xs * 0.9999) & (q2 > ylo * 0.5)
        i = np.argmin(abs(np.log10(q2[ok]) - 0.9))  # label near Q2 ~ 8 GeV2
        ax.text(xs[ok][i] * 1.1, q2[ok][i] * 1.05, r"$\eta_{\gamma^*}=%g$" % e, color=OR,
                fontsize=10, rotation=65, rotation_mode="anchor")
        ax.plot(xs[ok], q2[ok], color=OR, lw=1.3,
                label=r"Isolines of constant $\eta_{\gamma^*}$" if e == -0.2 else None)
    for yv in (0.1, 0.7):
        ax.plot(xs, yv * S * xs, color=GR, lw=0.9, ls="--",
                label="Isolines of constant $y$" if yv == 0.1 else None)
    ax.plot(xs, S * xs, color=BL, lw=2.2, label="$y=1$ boundary")
    xx = np.logspace(-5, 0, 800)
    lo_b = np.maximum(Q2MIN, YMIN * S * xx); hi_b = np.minimum(Q2MAX, YMAX * S * xx)
    ax.fill_between(xx, lo_b, np.where(hi_b > lo_b, hi_b, lo_b), color="#f5c26b",
                    alpha=0.7, lw=0, label="Measurement Phase Space")
    # labels
    ax.text(0.04, 0.80, "$E_e$ = 27.6 GeV\n$E_p$ = 920 GeV\n"
            r"$150 < Q^2 < 20000$ GeV$^2$" "\n$0.1 < y < 0.7$",
            transform=ax.transAxes, fontsize=13, va="top")
    ax.set_xscale("log"); ax.set_yscale("log")
    ax.set_xlim(xlo, xhi); ax.set_ylim(ylo, yhi)
    ax.set_xlabel(r"$x_{\mathrm{Bj}}$"); ax.set_ylabel(r"$Q^2$ [GeV$^2$]")
    ax.legend(loc="upper left", frameon=False, fontsize=12)
    fig.tight_layout()
    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    fig.savefig(a.out)


if __name__ == "__main__":
    main()
