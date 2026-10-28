#!/usr/bin/env python3
"""WP1 schema-parity check: real-data `hits` trees against the MC tree written by extract_hits.C (D1, D7).

Per data file, branch by branch against MC0: name, leaf C++ type, leaf count, vector element type (branch class),
relative order of the MC branches. Differences outside the D7 allowlist are reported; the exit code is 0 only when there
are none. Added branches must be D7-declared; no MC branch may be missing (D7: truth branches are present but empty).
Additional per-file D7 invariants: `meta` TNamed (JSON, counters identity), truth/CST/CIP/COP branches empty,
per-event vector lengths consistent, zcd codes, reco links, D17 flags (standard selection: run_selected = det_on = 1).

Usage (after desygroupset h1 + thish1.sh):
  check_schema.py --mc <MC0> --data '<glob>' --out-json <json>
"""
import argparse, glob, json, sys
import numpy as np

# D7 declared additions (written by WP1: first 12; declared for later packages: coz_trk_*, cst_sp_*)
ADDED = {'is_data', 'cjc_t0', 'cjc_t0_src', 'run_selected', 'run_quality', 'det_on', 'hv_off', 'caen_off', 'bbl3_off',
         'cjc_trk_x', 'cjc_trk_y', 'cjc_trk_z', 'coz_trk_x', 'coz_trk_y', 'coz_trk_z'}
ADDED_PREFIX = ('cst_sp_',)
REMOVED = set()          # D7: every MC branch is present (truth ones as empty vectors)
EMPTY_PREFIX = ('str_', 'svx_', 'cjc_truth_', 'crjt_', 'crvt_', 'crzt_', 'crmt_', 'csts_', 'cip_', 'cop_', 'cjcu_')
EMPTY_NAMES = {'cjc_ntruth', 'coz_str'}
ZCODES = {-191.0, -181.0, 181.0, 191.0, 201.0}


def describe(fn):
    import ROOT
    f = ROOT.TFile.Open(fn)
    t = f.Get('hits')
    if not t:
        return None, None, None
    out = []
    for b in t.GetListOfBranches():
        l = b.GetListOfLeaves().At(0)
        out.append({'name': b.GetName(), 'leaf_type': l.GetTypeName(), 'leaf_len': int(l.GetLen()),
                    'class': b.GetClassName(), 'n_leaves': int(b.GetListOfLeaves().GetEntries())})
    meta = f.Get('meta')
    m = None
    if meta:
        try:
            m = json.loads(meta.GetTitle())
        except Exception as e:                                       # noqa
            m = {'_error': str(e)}
    n = int(t.GetEntries())
    f.Close()
    return out, m, n


def is_empty_branch(n):
    return n.startswith(EMPTY_PREFIX) or n in EMPTY_NAMES


def schema_diff(mc, da):
    mcd = {b['name']: b for b in mc}
    dad = {b['name']: b for b in da}
    d = {'missing_in_data': [n for n in mcd if n not in dad and n not in REMOVED],
         'removed_declared': [n for n in mcd if n not in dad and n in REMOVED],
         'extra_undeclared': [n for n in dad if n not in mcd and n not in ADDED and not n.startswith(ADDED_PREFIX)],
         'added_declared': [n for n in dad if n not in mcd and (n in ADDED or n.startswith(ADDED_PREFIX))],
         'type_diff': [], 'order_diff': None}
    for n, b in mcd.items():
        if n in dad:
            for k in ('leaf_type', 'leaf_len', 'class', 'n_leaves'):
                if b[k] != dad[n][k]:
                    d['type_diff'].append({'branch': n, 'field': k, 'mc': b[k], 'data': dad[n][k]})
    common_mc = [n for n in mcd if n in dad]
    common_da = [n for n in dad if n in mcd]
    if common_mc != common_da:
        d['order_diff'] = [i for i, (a, b) in enumerate(zip(common_mc, common_da)) if a != b][:5]
    n_diff = (len(d['missing_in_data']) + len(d['extra_undeclared']) + len(d['type_diff'])
              + (1 if d['order_diff'] else 0))
    return d, n_diff


def invariants(fn, meta):
    """D7 per-file invariants with uproot; returns a dict of failures (empty = clean) and some counts."""
    import uproot, awkward as ak
    t = uproot.open(fn)['hits']
    br = t.keys()
    fails = []
    ne = t.num_entries
    # empty branches
    nonempty = []
    for n in br:
        if is_empty_branch(n):
            a = t[n].array(library='ak')
            if ak.sum(ak.num(a, axis=1)) != 0:
                nonempty.append(n)
    if nonempty:
        fails.append(f'truth/CST/CIP/COP/CJCU branches not empty: {nonempty[:6]}')
    for n in ('n_str_all', 'n_svx_all'):
        if np.any(t[n].array(library='np') != 0):
            fails.append(f'{n} != 0')
    A = t.arrays(['cjc_wire', 'cjc_drift', 'cjc_drift_raw', 'cjc_qplus', 'cjc_qminus', 'cjc_zcd', 'cjc_wire_x', 'cjc_wire_y',
                  'cjc_xp', 'cjc_yp', 'cjc_xm', 'cjc_ym', 'cjc_reco_trk', 'cjc_reco_side', 'cjc_ring', 'cjc_cell',
                  'cjc_layer', 'cjc_trk_x', 'cjc_trk_y', 'cjc_trk_z', 'cjkr_kappa', 'cjkr_cov9', 'cjkr_ncjc', 'cjkr_ndf',
                  'cjkr_chi2', 'cjkr_dedx', 'cjkr_rad', 'cjkr_rend', 'cjkr_next', 'cjkr_nhit_packed', 'cjkr_phi',
                  'cjkr_theta', 'cjkr_dca', 'cjkr_z0', 'cjkt_kappa', 'cjkt_phi', 'cjkt_theta', 'cjkt_chi2', 'cjkt_ndf',
                  'cjkt_vtx', 'cjkt_cjkr', 'cjkv_x', 'cjkv_y', 'cjkv_z', 'cjkv_cjkt', 'cjkv_type', 'cjkv_aux', 'coz_wire',
                  'coz_drift', 'coz_qplus', 'coz_qminus', 'run_selected', 'det_on', 'cjc_t0_src', 'is_data'], library='ak')
    nd = ak.num(A['cjc_wire'], axis=1)
    for n in ('cjc_drift', 'cjc_drift_raw', 'cjc_qplus', 'cjc_qminus', 'cjc_zcd', 'cjc_wire_x', 'cjc_wire_y', 'cjc_xp',
              'cjc_yp', 'cjc_xm', 'cjc_ym', 'cjc_reco_trk', 'cjc_reco_side', 'cjc_ring', 'cjc_cell', 'cjc_layer',
              'cjc_trk_x', 'cjc_trk_y', 'cjc_trk_z'):
        if not ak.all(ak.num(A[n], axis=1) == nd):
            fails.append(f'{n} length != cjc_wire length in some event')
    nk = ak.num(A['cjkr_kappa'], axis=1)
    for n in ('cjkr_ncjc', 'cjkr_ndf', 'cjkr_chi2', 'cjkr_dedx', 'cjkr_rad', 'cjkr_rend', 'cjkr_next', 'cjkr_nhit_packed',
              'cjkr_phi', 'cjkr_theta', 'cjkr_dca', 'cjkr_z0'):
        if not ak.all(ak.num(A[n], axis=1) == nk):
            fails.append(f'{n} length != cjkr_kappa length')
    if not ak.all(ak.num(A['cjkr_cov9'], axis=1) == 9 * nk):
        fails.append('cjkr_cov9 length != 9 * n_cjkr')
    ntk = ak.num(A['cjkt_kappa'], axis=1)
    for n in ('cjkt_phi', 'cjkt_theta', 'cjkt_chi2', 'cjkt_ndf', 'cjkt_vtx', 'cjkt_cjkr'):
        if not ak.all(ak.num(A[n], axis=1) == ntk):
            fails.append(f'{n} length != cjkt_kappa length')
    nv = ak.num(A['cjkv_x'], axis=1)
    for n in ('cjkv_y', 'cjkv_z', 'cjkv_cjkt', 'cjkv_type', 'cjkv_aux'):
        if not ak.all(ak.num(A[n], axis=1) == nv):
            fails.append(f'{n} length != cjkv_x length')
    nc = ak.num(A['coz_wire'], axis=1)
    for n in ('coz_drift', 'coz_qplus', 'coz_qminus'):
        if not ak.all(ak.num(A[n], axis=1) == nc):
            fails.append(f'{n} length != coz_wire length')
    if ne and not ak.all(nd > 0):
        fails.append('event with zero CJC hits written (D10)')
    z = ak.to_numpy(ak.flatten(A['cjc_zcd']))
    bad = ~((np.abs(z) <= 130.0) | np.isin(z, list(ZCODES)))
    if bad.any():
        fails.append(f'{int(bad.sum())} cjc_zcd values neither |z|<=130 nor a code in {sorted(ZCODES)}')
    side = ak.to_numpy(ak.flatten(A['cjc_reco_side']))
    if not np.isin(side, (-1, 0, 1)).all():
        fails.append('cjc_reco_side outside {-1,0,1}')
    rt = ak.flatten(A['cjc_reco_trk'])
    ntr = ak.flatten(ak.broadcast_arrays(nk, A['cjc_reco_trk'])[0])
    if ak.any(rt >= ntr):
        fails.append('cjc_reco_trk >= number of CJKR rows')
    if ak.any((A['cjc_reco_trk'] >= 0) != (A['cjc_reco_side'] >= 0)):
        fails.append('cjc_reco_trk and cjc_reco_side assigned inconsistently')
    if meta and meta.get('select') == 'standard':
        if not (ak.all(A['run_selected'] == 1) and ak.all(A['det_on'] == 1)):
            fails.append('standard selection but run_selected/det_on != 1 in some event')
    if not ak.all(A['is_data'] == 1):
        fails.append('is_data != 1')
    if not meta or '_error' in (meta or {}):
        fails.append('meta TNamed missing or not JSON')
    elif not meta.get('seen_equals_written_rejected_empty_skips'):
        fails.append('meta counters: seen != written + rejected + empty + skips')
    if meta and meta['counters']['written'] != ne:
        fails.append(f"meta written {meta['counters']['written']} != entries {ne}")
    return fails


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--mc', required=True)
    ap.add_argument('--data', required=True, help='glob or file')
    ap.add_argument('--out-json', required=True)
    a = ap.parse_args()
    mc, _, n_mc = describe(a.mc)
    files = sorted(glob.glob(a.data))
    res, total = [], 0
    for fn in files:
        da, meta, n = describe(fn)
        if da is None:
            res.append({'file': fn, 'error': 'no hits tree'})
            total += 1
            continue
        d, nd_ = schema_diff(mc, da)
        inv = invariants(fn, meta) if n else []   # zero-entry file (every event rejected by D17): schema only
        total += nd_ + len(inv)
        res.append({'file': fn, 'entries': n, 'n_branches': len(da), 'schema_diff': d, 'n_schema_diffs': nd_,
                    'invariant_failures': inv})
        print(f'{fn.split("/")[-1]}: {n} entries, {len(da)} branches, schema diffs {nd_}, invariant failures {len(inv)}',
              flush=True)
    summary = {'mc': a.mc, 'mc_branches': len(mc), 'mc_entries': n_mc, 'files': len(files), 'total_differences': total,
               'verdict': 'PASS' if files and total == 0 else 'FAIL', 'added_allowlist': sorted(ADDED),
               'removed_allowlist': sorted(REMOVED), 'results': res}
    json.dump(summary, open(a.out_json, 'w'), indent=1)
    print('verdict', summary['verdict'], 'files', len(files), 'total differences', total)
    return 0 if summary['verdict'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
