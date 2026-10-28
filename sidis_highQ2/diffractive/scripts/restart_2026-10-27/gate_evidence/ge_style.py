"""Shared data-vs-MC drawing for the gate-evidence figures: main panel (data points, stacked or
overlaid MC, MC-stat band) over a data/MC ratio panel (data stat error bars, shaded +-MC-stat band)."""
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

C = {"data": "black", "signal": "#3b6fb6", "reggeon": "#9cc3e6", "nd": "#e8a33d",
     "7071": "#d1495b", "7094": "#8c5fbf", "7091": "#2e8b57", "nd4cat": "#e8a33d"}
plt.rcParams.update({"font.size": 10, "axes.labelsize": 10, "legend.fontsize": 8, "figure.facecolor": "white",
                     "axes.facecolor": "white", "savefig.facecolor": "white"})


def grid(ncol, nrow=1, w=4.0, h=3.6):
    fig = plt.figure(figsize=(w * ncol, h * nrow))
    gs = fig.add_gridspec(2 * nrow, ncol, height_ratios=[3, 1.25] * nrow, hspace=0.0, wspace=0.28)
    axes = []
    for r in range(nrow):
        row = []
        for c in range(ncol):
            a = fig.add_subplot(gs[2 * r, c]); b = fig.add_subplot(gs[2 * r + 1, c], sharex=a)
            a.tick_params(labelbottom=False); row.append((a, b))
        axes.append(row)
    return fig, axes


def _steps(edges, y):
    return np.repeat(edges, 2)[1:-1], np.repeat(y, 2)


def panel(ax, rx, edges, data, mc_parts, mc_var, *, density=False, logy=False, ylabel="events",
          xlabel="", mc_lines=None, rlim=None, qual=None, top=8):
    """mc_parts: list of (label, values, color) stacked bottom-up; mc_var: variance of their sum.
    mc_lines: optional list of (label, values, var, color, ls) drawn as unstacked lines (own ratio).
    qual: boolean mask of bins drawn in the ratio panel (default: data > 0)."""
    edges = np.asarray(edges, float); wd = np.diff(edges); cen = 0.5 * (edges[1:] + edges[:-1])
    sc = (1.0 / wd) if density else np.ones_like(wd)
    d = np.asarray(data, float); de = np.sqrt(d)
    q = (d > 0) if qual is None else np.asarray(qual, bool)
    ratios = []; offs = []
    if mc_parts:
        bot = np.zeros_like(d)
        for lab, v, col in mc_parts:
            v = np.asarray(v, float)
            ax.fill_between(*_steps(edges, (bot) * sc), _steps(edges, (bot + v) * sc)[1], step=None, color=col, alpha=0.85, lw=0, label=lab)
            bot = bot + v
        tot = bot; err = np.sqrt(np.asarray(mc_var, float))
        x, y = _steps(edges, tot * sc); ax.plot(x, y, color="0.25", lw=0.8)
        ax.fill_between(x, _steps(edges, (tot - err) * sc)[1], _steps(edges, (tot + err) * sc)[1], facecolor="none",
                        edgecolor="0.3", hatch="////", lw=0, label="MC stat.")
        ok = q & (tot > 0)
        r = np.where(ok, d / np.where(tot > 0, tot, 1), np.nan); re = np.where(ok, de / np.where(tot > 0, tot, 1), np.nan)
        rb = np.where(tot > 0, err / np.where(tot > 0, tot, 1), 0)
        rx.fill_between(*_steps(edges, 1 - rb), _steps(edges, 1 + rb)[1], color="0.6", alpha=0.45, lw=0)
        rx.errorbar(cen[ok], r[ok], yerr=re[ok], xerr=wd[ok] / 2, fmt="o", ms=2.5, color="black", lw=0.8)
        ratios.append(r[ok]); offs.append((cen[ok], r[ok], "black"))
    for lab, v, var, col, ls in (mc_lines or []):
        v = np.asarray(v, float); e = np.sqrt(np.asarray(var, float))
        x, y = _steps(edges, v * sc); ax.plot(x, y, color=col, ls=ls, lw=1.4, label=lab)
        ok = q & (v > 0)
        r = np.where(ok, d / np.where(v > 0, v, 1), np.nan); re = np.where(ok, de / np.where(v > 0, v, 1), np.nan)
        rb = np.where(v > 0, e / np.where(v > 0, v, 1), 0)
        rx.fill_between(*_steps(edges, 1 - rb), _steps(edges, 1 + rb)[1], color=col, alpha=0.2, lw=0)
        rx.errorbar(cen[ok], r[ok], yerr=re[ok], fmt="o", ms=2.2, color=col, lw=0.7, mfc=col if ls == "-" else "white")
        ratios.append(r[ok]); offs.append((cen[ok], r[ok], col))
    ax.errorbar(cen[d > 0], (d * sc)[d > 0], yerr=(de * sc)[d > 0], xerr=wd[d > 0] / 2, fmt="o", ms=3, color="black", lw=0.9, label="data", zorder=5)
    if logy:
        ax.set_yscale("log")
        pos = (d * sc)[d > 0]
        if pos.size: ax.set_ylim(max(pos.min() * 0.3, 1e-3), pos.max() * top)
    else:
        tops = [(d * sc).max()] + ([(tot * sc).max()] if mc_parts else []) + [(np.asarray(v, float) * sc).max() for _, v, _, _, _ in (mc_lines or [])]
        ax.set_ylim(0, max(max(tops) * 1.3, 1))
    ax.set_ylabel(ylabel); rx.set_xlabel(xlabel); rx.set_ylabel("data / MC")
    rx.axhline(1, color="0.3", lw=0.7)
    if rlim is None:
        allr = np.concatenate([x[np.isfinite(x)] for x in ratios]) if ratios else np.array([1.])
        lo = max(0.0, min(0.5, np.nanmin(allr) - 0.15)) if allr.size else 0.5
        hi = min(4.0, max(1.5, np.nanmax(allr) + 0.2)) if allr.size else 1.5
        rlim = (lo, hi)
    rx.set_ylim(*rlim)
    for xc, rv, col in offs:  # off-scale ratio points: triangles at the panel edge
        hi_ = rv > rlim[1]; lo_ = rv < rlim[0]
        rx.plot(xc[hi_], np.full(hi_.sum(), rlim[1] - 0.04 * (rlim[1] - rlim[0])), "^", ms=4, color=col, mfc="white")
        rx.plot(xc[lo_], np.full(lo_.sum(), rlim[0] + 0.04 * (rlim[1] - rlim[0])), "v", ms=4, color=col, mfc="white")
    ax.set_xlim(edges[0], edges[-1])
    return rlim


def vline(ax, rx, x, label, color="0.2", ls="--"):
    ax.axvline(x, color=color, ls=ls, lw=1.0); rx.axvline(x, color=color, ls=ls, lw=1.0)
    if label:
        ax.text(x, 0.04, label + " ", transform=ax.get_xaxis_transform(), ha="right", va="bottom", fontsize=7, color=color, rotation=90)


def note(ax, txt, loc="ul"):
    x, ha = (0.03, "left") if loc[1] == "l" else (0.97, "right")
    y, va = (0.96, "top") if loc[0] == "u" else (0.04, "bottom")
    ax.text(x, y, txt, transform=ax.transAxes, ha=ha, va=va, fontsize=7.5,
            bbox=dict(boxstyle="round,pad=0.25", fc="white", ec="0.7", alpha=0.9))


def figlegend(fig, ax, ncol=None):
    hs, ls = ax.get_legend_handles_labels()
    fig.legend(hs, ls, loc="lower center", ncol=ncol or len(ls), frameon=False, fontsize=8, bbox_to_anchor=(0.5, -0.13))


def save(fig, path, title=None):
    if title: fig.suptitle(title, fontsize=11, y=0.995)
    fig.savefig(path, dpi=125, bbox_inches="tight"); plt.close(fig)
