"""WP-C study: X (gamma* IP) rest-frame observables x_F and p_T*^2 (vectorised numpy, float64).

Four-vector order everywhere: (px, py, pz, E), arrays of shape (n,4) (or (4,) broadcast).

Definitions (plan D6):
  * rest frame of p_X; the gamma* direction q^ in that frame defines the axis;
  * x_F = 2 p_L* / M_frame, p_L* = p* . u with u = -q^*  (POSITIVE along the Pomeron/proton direction);
  * p_T*^2 = |p*|^2 - p_L*^2.
Frames:
  A  : p_X = stored HFS 4-vector (phPx,phPy,phPz,phE); M_frame = sqrt(p_X^2)
  B  : p_X = q + xi P collinear, xi solves (q + xi P)^2 = M_X,rec^2 exactly (quadratic root, keeps m_p);
       M_frame = M_X,rec.  The frame is then a pure boost along the gamma* p axis.
  G1 : gen proxy p_X = q_true + x_IP^mir P ; M_frame = sqrt(p_X^2)   (mass^2 = M_X^2 - t to first order)
  G2 : gen proxy with xi chosen such that the mass equals mirMX
"""
import os
import sys

import numpy as np

_ESIG = "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/esigma_final_2026-10-24"
if _ESIG not in sys.path:
    sys.path.insert(0, _ESIG)
import esigma_xf as X  # noqa: E402  (reco_photon, gen_truth, gen_mass, _beam are imported, not copied)

M_P = X.M_PROTON


def _f(a):
    return np.asarray(a, dtype=np.float64)


def proton4(Ep):
    Ep = _f(Ep)
    return np.stack([np.zeros_like(Ep), np.zeros_like(Ep), Ep, np.sqrt(Ep * Ep + M_P * M_P)], axis=-1)


def msq(p):
    p = _f(p)
    return p[..., 3] ** 2 - p[..., 0] ** 2 - p[..., 1] ** 2 - p[..., 2] ** 2


def mdot(a, b):
    a, b = _f(a), _f(b)
    return a[..., 3] * b[..., 3] - a[..., 0] * b[..., 0] - a[..., 1] * b[..., 1] - a[..., 2] * b[..., 2]


def boost_to_rest(p, F, mF=None):
    """Boost 4-vectors p into the rest frame of 4-vector F (F timelike). mF optional exact mass of F."""
    p, F = np.broadcast_arrays(_f(p), _f(F))
    with np.errstate(all="ignore"):
        E = F[..., 3]
        m = np.sqrt(msq(F)) if mF is None else _f(mF)
        b = F[..., :3] / E[..., None]
        b2 = np.sum(b * b, axis=-1)
        gam = E / m
        g2 = np.where(b2 > 0, (gam - 1.0) / np.where(b2 > 0, b2, 1.0), 0.0)
        bp = np.sum(b * p[..., :3], axis=-1)
        pe = gam * (p[..., 3] - bp)
        c = g2 * bp - gam * p[..., 3]
        pv = p[..., :3] + c[..., None] * b
    return np.concatenate([pv, pe[..., None]], axis=-1)


def pX_collinear(q, Ep, M):
    """p_X = q + xi P with (q + xi P)^2 = M^2 exactly. Returns (pX, xi). Invalid -> NaN."""
    q = _f(q)
    P = proton4(Ep)
    M = _f(M)
    with np.errstate(all="ignore"):
        Q2 = -msq(q)
        Pq = mdot(P, q)
        xi = (Q2 + M * M) / (Pq + np.sqrt(Pq * Pq + M_P * M_P * (Q2 + M * M)))
        pX = q + xi[..., None] * P
        bad = ~(np.isfinite(M) & (M > 0) & np.isfinite(xi) & (Pq > 0))
        pX = np.where(bad[..., None], np.nan, pX)
    return pX, xi


def pX_xip(q, Ep, xip):
    """Gen proxy G1: p_X = q + x_IP P."""
    q = _f(q)
    P = proton4(Ep)
    return q + _f(xip)[..., None] * P


def frame_obs(h, pX, q, mF=None):
    """x_F, p_T*^2, M_frame of hadrons h in the rest frame of pX with gamma* axis from q. NaN where undefined."""
    h, pX, q = np.broadcast_arrays(_f(h), _f(pX), _f(q))
    with np.errstate(all="ignore"):
        m2 = msq(pX)
        M = np.where(m2 > 0, np.sqrt(np.where(m2 > 0, m2, np.nan)), np.nan) if mF is None else _f(mF)
        hb = boost_to_rest(h, pX, M)
        qb = boost_to_rest(q, pX, M)
        qn = np.sqrt(np.sum(qb[..., :3] ** 2, axis=-1))
        u = -qb[..., :3] / qn[..., None]
        pl = np.sum(hb[..., :3] * u, axis=-1)
        p2 = np.sum(hb[..., :3] ** 2, axis=-1)
        xf = 2.0 * pl / M
        pt2 = np.maximum(p2 - pl * pl, 0.0)
        ok = np.isfinite(M) & (M > 0) & np.isfinite(qn) & (qn > 0)
        xf = np.where(ok, xf, np.nan)
        pt2 = np.where(ok, pt2, np.nan)
    return xf, pt2, M


def ystar_photon_axis(h, q, Ep):
    """Rapidity of hadrons in the gamma* p CM frame with z along the photon direction (H1Boost FillGen convention:
    photon on +z, X at high y*)."""
    h, q = np.broadcast_arrays(_f(h), _f(q))
    W = q + proton4(Ep)
    hb = boost_to_rest(h, W)
    qb = boost_to_rest(q, W)
    qn = np.sqrt(np.sum(qb[..., :3] ** 2, axis=-1))
    pz = np.sum(hb[..., :3] * (qb[..., :3] / qn[..., None]), axis=-1)
    E = hb[..., 3]
    with np.errstate(all="ignore"):
        return 0.5 * np.log((E + pz) / (E - pz))


def hadron4(pt, eta, phi, mass):
    pt, eta, phi, mass = (_f(a) for a in (pt, eta, phi, mass))
    px, py, pz = pt * np.cos(phi), pt * np.sin(phi), pt * np.sinh(eta)
    return np.stack([px, py, pz, np.sqrt(px * px + py * py + pz * pz + mass * mass)], axis=-1)
