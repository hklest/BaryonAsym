"""WP-A shared definitions: cocktail chains, period luminosities, composite-scale
projection (represented data periods per component), and the registered t4
shape metric (copied verbatim from scripts/t4/assemble_localization_diag_2026-10-20.py:74-85)."""
import csv, hashlib, itertools, json
from pathlib import Path
import numpy as np

D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")
OUT = D / "data/restart_2026-10-27/wp-a"
PERIOD_MAP = OUT / "period_map.csv"
V3 = D / "docs/mc/composite_scale_table_v3_2026-10-20.csv"
SIDECAR = D / "docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv"
L_DATA = 351000.0
PERIODS = ["ep0304", "em0405", "em06_true", "ep0607_true"]
DATA_LABELS = PERIODS
DIFF8 = ["9012", "9024", "9033", "9155", "9157", "9158", "6671", "6677"]
SIGNAL = DIFF8[:6]
# chain -> ND labels and per-label exposure-factor overrides (nd4cat convention,
# scripts/t4/run_section_7_1_twofree_nd4cat_2026-10-20.py: 7071 models em06 only)
CHAINS = {
    "7071": {"nd": ["7071"], "fac_override": {}},
    "7091": {"nd": ["7091"], "fac_override": {}},
    "7094": {"nd": ["7094"], "fac_override": {}},   # added 2026-10-27 (coordinator; WP-D MC-3), pre-registered before S3
    "nd4cat": {"nd": ["7071", "7094", "7096", "7097"], "fac_override": {"7071": "em06_true"}},
    "meps3": {"nd": ["7094", "7096", "7097"], "fac_override": {}},
}
MIN_DATA_ENTRIES = 20
PASS_T, WARN_T = 0.10, 0.25


def sha256(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def period_lumi():
    L = {}
    for r in csv.DictReader(open(PERIOD_MAP)):
        L[r["period"]] = L.get(r["period"], 0.0) + float(r["lumi_accept_nb"])
    return L


def table_rows():
    rows = {}
    for p in (V3, SIDECAR):
        for r in csv.DictReader(open(p)):
            rows[r["component"]] = r
    return rows


def represented(label, chain=None):
    """Data periods a component represents: the unique subset of periods whose
    certified-lumi fraction sums to its exposure_factor (fail-closed)."""
    L = period_lumi()
    ov = CHAINS[chain]["fac_override"].get(label) if chain else None
    if ov:
        return [ov]
    fac = float(table_rows()[label]["exposure_factor"])
    hits = []
    for r in range(1, 5):
        for sub in itertools.combinations(PERIODS, r):
            if abs(sum(L[k] for k in sub) / L_DATA - fac) < 2e-5:
                hits.append(list(sub))
    if len(hits) != 1:
        raise SystemExit("FATAL: represented-period decomposition of %s (fac %s) not unique: %s" % (label, fac, hits))
    return hits[0]


def fac_used(label, chain):
    """Exposure factor the chain uses for this label (signed = exposure1 * fac)."""
    L = period_lumi()
    return sum(L[k] for k in represented(label, chain)) / L_DATA


def table_fac(label):
    return float(table_rows()[label]["exposure_factor"])


def load_partial(d, label):
    p = Path(d) / label / "partial.json"
    if not p.exists():
        raise SystemExit("FATAL: missing partial %s" % p)
    return json.load(open(p))


def shape_metric(data_hist, mc_hist):
    d = np.asarray(data_hist, float); m = np.asarray(mc_hist, float)
    q = d >= MIN_DATA_ENTRIES
    if not q.any() or d[q].sum() <= 0 or m[q].sum() <= 0:
        return None, None, 0
    dn = d[q] / d[q].sum(); mn = m[q] / m[q].sum()
    ratio = mn / np.where(dn > 0, dn, 1.0)
    with np.errstate(invalid="ignore", divide="ignore"):
        ratio = np.where(dn > 0, mn / dn, np.inf)
    dev = np.abs(ratio - 1.0)
    i = int(np.argmax(dev)); qb = np.where(q)[0]
    return float(dev[i]), int(qb[i]), int(q.sum())


def verdict(dev):
    if dev is None:
        return "NOQUALIFY"
    return "PASS" if dev < PASS_T else ("WARN" if dev < WARN_T else "FAIL")
