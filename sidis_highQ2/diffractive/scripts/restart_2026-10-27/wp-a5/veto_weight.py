"""WP-A5 forward-veto signal-loss correction: the downstream interface (WP-E, WP-F, WP-H import this module).

The record is data/restart_2026-10-27/wp-a5/veto_correction.json (frozen even-split q, D14 sigma_q); this module only
reads it.  For each MC signal event that passes the three vetoes the weight is  prod_v (1 - q_v)  (1 - q clipped at 0;
q < 0 gives a weight above 1).  q is a step function in (M_X bin [4,10) [10,20) [20,40], D cell); events with cell -1
take the ALL-cells q of their M_X bin, events with M_X outside [4,40] (or non-finite) the ALL-M_X q of their cell, both
outside -> ALL|ALL.  It is the same for frame A and frame B (event-level quantity).
Variants: nominal; up / down (all detectors shifted coherently by +sigma_q / -sigma_q); eta (q + delta_v(eta08 bin),
the additive eta-shape variant of A5-D4); eta_up / eta_down (eta form shifted by +-sigma_q, for the Open Q-B
alternative); zero (weight 1); and per detector nominal_<det> (only <det> corrected), up_<det> / down_<det> / eta_<det>
(only <det> varied, the others nominal), <det> in N1_PLUG, N1_FTS, N1_FMD.
eta08 bins: [-1,1) [1,2) [2,2.5) [2.5,3.0) [3.0,3.2) [3.2,3.3); < -1, sentinel or non-finite use delta[-1,1); >= 3.3 uses
delta[3.2,3.3)."""
from pathlib import Path
import numpy as np

VETO_CORRECTION_JSON = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a5/veto_correction.json")
DETS = ("N1_PLUG", "N1_FTS", "N1_FMD")
MXK = ("4-10", "10-20", "20-40"); CK = ("D0", "D1", "D2")
ETAE = np.array([-1.0, 1.0, 2.0, 2.5, 3.0, 3.2, 3.3])
ETAK = ("[-1,1)", "[1,2)", "[2,2.5)", "[2.5,3.0)", "[3.0,3.2)", "[3.2,3.3)")
_CACHE = {}


def load(path=VETO_CORRECTION_JSON):
    import json
    p = str(path)
    if p not in _CACHE: _CACHE[p] = json.load(open(p))
    return _CACHE[p]


def _tables(corr, det):
    """(4 x 4) q and sigma_q tables, index [mx(0..2, 3=ALL), cell(0..2, 3=ALL)], and the eta delta vector."""
    q = np.zeros((4, 4)); s = np.zeros((4, 4))
    for i, m in enumerate(MXK + ("ALL",)):
        for j, c in enumerate(CK + ("ALL",)):
            e = corr["cells"][det]["%s|%s" % (m, c)]; q[i, j] = e["q"]; s[i, j] = e["sigma_q"]
    b = corr["eta_shape"][det]["bins"]; d = np.array([b[k]["delta"] for k in ETAK])
    return q, s, d


def mx_index(mx):
    mx = np.asarray(mx, float)
    with np.errstate(invalid="ignore"):
        i = np.minimum(np.searchsorted([4.0, 10.0, 20.0, 40.0], mx, side="right") - 1, 2)
        return np.where(np.isfinite(mx) & (mx >= 4.0) & (mx <= 40.0), i, 3)


def eta_index(ec08):
    x = np.asarray(ec08, float)
    with np.errstate(invalid="ignore"):
        i = np.searchsorted(ETAE, x, side="right") - 1
        i = np.where(np.isfinite(x) & (x >= 3.3), 5, i)
        return np.where(np.isfinite(x) & (x >= -1.0), np.clip(i, 0, 5), 0)


def weights(mx, cell, ec08, variant="nominal", detectors=DETS, corr=None):
    corr = load() if corr is None else corr
    im = mx_index(mx); c = np.asarray(cell); ic = np.where((c >= 0) & (c <= 2), c, 3).astype(int); ie = eta_index(ec08)
    w = np.ones(im.size)
    if variant == "zero": return w
    base, _, det_only = variant.partition("_")
    if base not in ("nominal", "up", "down", "eta") and variant not in ("eta_up", "eta_down"):
        raise ValueError("unknown variant %s" % variant)
    if variant in ("eta_up", "eta_down"): base, det_only = variant, ""
    elif "_" in variant and det_only not in detectors:
        raise ValueError("variant %s: detector suffix must be one of %s" % (variant, tuple(detectors)))
    for det in detectors:
        q, s, d = _tables(corr, det); qq = q[im, ic]
        if det_only and det != det_only:
            if base == "nominal": continue          # nominal_<det>: only <det> corrected
            w *= np.clip(1 - qq, 0, None); continue  # up_/down_/eta_<det>: the others nominal
        if base in ("up", "eta_up"): qq = qq + s[im, ic]
        if base in ("down", "eta_down"): qq = qq - s[im, ic]
        if base in ("eta", "eta_up", "eta_down"): qq = qq + d[ie]
        w *= np.clip(1 - qq, 0, None)
    return w
