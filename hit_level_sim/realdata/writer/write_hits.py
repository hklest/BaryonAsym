#!/usr/bin/env python3
"""WP1 real-data hits-tree writer (plan 2026-10-27-fm4npp-h1-real-data-hits, D1/D5/D7/D10/D11/D17).

Turns one rawhit_probe lam/ dump dir (calib/events.txt + calib/consts_<run>.json + cjc/cjcprobe.txt) plus the HAT
join into a flat ROOT `hits` tree with the branch set, names, C++ types, units and per-event structure of the MC tree
written by hit_level_sim/extract_hits.C (reference $MC0), plus the D7 data-only branches.

Data hits are defined as the MC's hits are, post-calibration (user ruling 2026-10-28):
  cjc_wire/ring/cell/layer   from the CRJE wire word, MC formulas
  cjc_drift [cm]             calibrated drift length: mean of |drd| of the two mirrors of CJC.simple() / 1000 (MC: one
                             value, (DL*0.001-T0)/scale; data: NDB calibration, the two mirrors differ by the side-dependent
                             corrections, so the mean is the one symmetric number)
  cjc_drift_raw              DL*0.001 (DL = FADC ticks, not ns; MC convention, extract_hits.C:159)
  cjc_qplus/qminus           raw CRJE charges
  cjc_zcd [cm]               z of CJC.simple(); MC code convention: |z| > 130 -> +-191, +-181 one end dead, 201 both
                             ends dead; the data-only overflow code 301 is mapped to 201 (counted in meta)
  cjc_wire_x/y               NDB wire position at zero drift (consts cp_x/cp_y)
  cjc_xp/yp, cjc_xm/ym       the two mirror points of CJC.simple() (cos beta = 1, per-run NDB constants, T0 of D11)
  cjc_reco_trk/side          from CJXR (kdr, itr, sign of DRFE); a hit on several tracks keeps the last (as extract_hits.C)
  cjkr_*, cjkt_*, cjkv_*     the DST CJKR/CJKT/CJKV rows, same columns as extract_hits.C
  coz_wire/drift/qplus/qminus the raw CRZE words
Truth-only branches (str_*, svx_*, str_orig, svx_orig, cjc_truth_*, crjt_*, crvt_*, crzt_*, crmt_*, coz_str) are empty
vectors, n_str_all = n_svx_all = 0. CST digis (csts_*, WP4), CIP/COP (cip_*, cop_*, WP5) and CJCU (not in the dumps) are empty.
Additive: is_data, cjc_t0, cjc_t0_src, run_selected, run_quality, det_on, hv_off, caen_off, bbl3_off, cjc_trk_x/y/z.

D17 is a (run, event) filter at write time: run list AND IsOn(CJC1, CJC2) (parity_cjc.det_on mode 'cjc'). `--select none`
writes every event with hits and keeps the flags. D10: events with zero CRJE hits are skipped and counted.

Run after `desygroupset h1` + thish1.sh (PyROOT, LCG python; never python3 -I):
  write_hits.py --lam-dir <dir> --runlist-table <txt> --out <hits_part_X.root> [--max-events N] [--select standard|none]
"""
import argparse, datetime, glob, hashlib, json, math, os, sys
import numpy as np

PY = '/data/dust/group/h1/klesthen/rawhit_probe/calib_consts/py'
PARITY = '/data/dust/group/h1/klesthen/BaryonAsym/hit_level_sim/realdata/validation/parity'
NDB = '/afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/releases/4.2.1/share/H1OOSnapshot.root'
sys.path.insert(0, PY)
sys.path.insert(0, PARITY)

# (name, kind): I Int_t, F Float_t, vf vector<float>, vi vector<int>. MC order (extract_hits.C), then the D7 additions.
MC_SCHEMA = [('run', 'I'), ('event', 'I')]
MC_SCHEMA += [(n, 'vf') for n in ('str_px', 'str_py', 'str_pz', 'str_e')]
MC_SCHEMA += [(n, 'vi') for n in ('str_pdg', 'str_parent', 'str_vtx', 'str_gtr')]
MC_SCHEMA += [(n, 'vf') for n in ('svx_x', 'svx_y', 'svx_z')] + [(n, 'vi') for n in ('svx_type', 'svx_parent')]
MC_SCHEMA += [('str_orig', 'vi'), ('svx_orig', 'vi'), ('n_str_all', 'I'), ('n_svx_all', 'I')]
MC_SCHEMA += [(n, 'vi') for n in ('cjc_wire', 'cjc_ring', 'cjc_cell', 'cjc_layer')]
MC_SCHEMA += [(n, 'vf') for n in ('cjc_drift', 'cjc_drift_raw', 'cjc_qplus', 'cjc_qminus', 'cjc_zcd', 'cjc_wire_x',
                                  'cjc_wire_y', 'cjc_xp', 'cjc_yp', 'cjc_xm', 'cjc_ym')]
MC_SCHEMA += [(n, 'vi') for n in ('cjc_truth_str', 'cjc_ntruth', 'cjc_truth_crjt_idx')]
MC_SCHEMA += [(n, 'vf') for n in ('cjc_truth_x', 'cjc_truth_y', 'cjc_truth_z')]
MC_SCHEMA += [(n, 'vi') for n in ('cjc_reco_trk', 'cjc_reco_side', 'crjt_wire', 'crjt_str')]
MC_SCHEMA += [(n, 'vf') for n in ('crjt_xin', 'crjt_yin', 'crjt_zin', 'crjt_xout', 'crjt_yout', 'crjt_zout', 'crjt_dlen',
                                  'crjt_eloss')]
MC_SCHEMA += [('crjt_hist', 'vi')]
MC_SCHEMA += [(n, 'vf') for n in ('cjkr_kappa', 'cjkr_phi', 'cjkr_theta', 'cjkr_dca', 'cjkr_z0', 'cjkr_rad', 'cjkr_rend',
                                  'cjkr_chi2')]
MC_SCHEMA += [('cjkr_ndf', 'vi'), ('cjkr_dedx', 'vf'), ('cjkr_ncjc', 'vi'), ('cjkr_nhit_packed', 'vi'),
              ('cjkr_next', 'vi'), ('cjkr_cov9', 'vf')]
MC_SCHEMA += [(n, 'vf') for n in ('cjkt_kappa', 'cjkt_phi', 'cjkt_theta', 'cjkt_chi2')]
MC_SCHEMA += [(n, 'vi') for n in ('cjkt_ndf', 'cjkt_vtx', 'cjkt_cjkr')]
MC_SCHEMA += [(n, 'vf') for n in ('crvt_x', 'crvt_y', 'crvt_z', 'crvt_pin')] + [(n, 'vi') for n in ('crvt_str', 'crvt_pdg')]
MC_SCHEMA += [(n, 'vi') for n in ('csts_strip', 'csts_adc', 'csts_crvt')]
MC_SCHEMA += [(n, 'vf') for n in ('crzt_x', 'crzt_y', 'crzt_z')] + [('crzt_str', 'vi')]
MC_SCHEMA += [(n, 'vi') for n in ('coz_wire', 'coz_drift', 'coz_qplus', 'coz_qminus', 'coz_str')]
MC_SCHEMA += [(n, 'vf') for n in ('crmt_x', 'crmt_y', 'crmt_z')] + [(n, 'vi') for n in ('crmt_det', 'crmt_str', 'crmt_aux')]
MC_SCHEMA += [(n, 'vi') for n in ('cip_pad', 'cop_pad', 'cip_bank', 'cop_bank')]
MC_SCHEMA += [(n, 'vf') for n in ('cjkv_x', 'cjkv_y', 'cjkv_z')]
MC_SCHEMA += [(n, 'vi') for n in ('cjkv_cjkt', 'cjkv_type', 'cjkv_aux', 'cjcu_link', 'cjcu_flag')]
ADD_SCHEMA = [('is_data', 'I'), ('cjc_t0', 'F'), ('cjc_t0_src', 'I'), ('run_selected', 'I'), ('run_quality', 'I'),
              ('det_on', 'I'), ('hv_off', 'vi'), ('caen_off', 'vi'), ('bbl3_off', 'vi'),
              ('cjc_trk_x', 'vf'), ('cjc_trk_y', 'vf'), ('cjc_trk_z', 'vf')]
SCHEMA = MC_SCHEMA + ADD_SCHEMA
NWIRE = 2640


def zcode(z, counters):
    """MC code convention for cjc_zcd (extract_hits.C:~330): |z| > 130 -> +-191; +-181, 201 pass; data overflow 301 -> 201."""
    if z == 301.0:
        counters['zcd_301_to_201'] += 1
        return 201.0
    if z in (181.0, -181.0, 191.0, -191.0, 201.0):   # exact h1rec codes
        return float(z)
    if abs(z) > 130.0:                       # includes corrected values just beyond +-180
        counters['zcd_gt130_to_191'] += 1
        return math.copysign(191.0, z)
    return float(z)


def sha256(fn):
    h = hashlib.sha256()
    with open(fn, 'rb') as f:
        for b in iter(lambda: f.read(1 << 20), b''):
            h.update(b)
    return h.hexdigest()


def read_table(fn):
    """runlist table: run selected quality."""
    sel, qual = {}, {}
    for l in open(fn):
        if l.strip() and not l.startswith('#'):
            r, s, q = (int(x) for x in l.split()[:3])
            sel[r], qual[r] = s, q
    return sel, qual


def build_tree(fout):
    import ROOT
    f = ROOT.TFile(fout, 'RECREATE')
    t = ROOT.TTree('hits', 'H1 central tracker hits (CJC/CST/COZ), real data (WP1 writer, schema of extract_hits.C)')
    sc, vec = {}, {}
    for name, kind in SCHEMA:
        if kind in ('I', 'F'):
            sc[name] = np.zeros(1, dtype=np.int32 if kind == 'I' else np.float32)
            t.Branch(name, sc[name], f'{name}/{kind}')
        else:
            vec[name] = ROOT.std.vector('float' if kind == 'vf' else 'int')()
            t.Branch(name, vec[name])
    return f, t, sc, vec


def process_dir(a):
    from h1hits import CJC, cjavt0, load
    from events import read_events, read_dataprobe, xr_dict, flt
    import parity_cjc as pc
    import ROOT

    d = a.lam_dir.rstrip('/')
    steer = open(os.path.join(d, 'calib/s.steer')).read()
    hat = steer.split('fHatFiles')[1].split('"')[1]
    evs = read_events(os.path.join(d, 'calib/events.txt'))[:a.max_events]
    raw = read_dataprobe(os.path.join(d, 'cjc/cjcprobe.txt'))
    rsel, rqual = read_table(a.runlist_table)
    status = pc.read_hat_status(hat, [(e['run'], e['evt']) for e in evs])

    C = dict(seen=0, written=0, rejected=0, rej_runlist=0, rej_status=0, rej_both=0, empty=0, empty_no_probe=0, nmismatch=0, no_consts=0, no_hat=0, run_not_in_table=0,
             t0_hat=0, t0_cjavt0=0, t0_default=0, hat_t0_zero=0, hits_written=0, hits_empty_events=0,
             xr_assigned=0, xr_multi=0, xr_map_bad=0, ncjc_vs_packed_diff=0, cjkr_written=0,
             zcd_301_to_201=0, zcd_gt130_to_191=0, zcd_code_181=0, zcd_code_191=0, zcd_code_201=0,
             hv_off_events=0, hits_zcd_total=0)
    C.update({f'flag_{k}_off': 0 for k in pc.FLAG_BITS})

    fout, tree, sc, vec = build_tree(a.out)
    cj, cjhash = {}, {}
    for ev in evs:
        run, en = ev['run'], ev['evt']
        C['seen'] += 1
        if run not in rsel:
            C['run_not_in_table'] += 1
        rs = rsel.get(run, 0) == 1
        st = status.get((run, en))
        if st is None:
            C['no_hat'] += 1
        don = st is not None and pc.det_on(st, 'cjc')
        if st is not None:
            for k, b in pc.FLAG_BITS.items():
                C[f'flag_{k}_off'] += pc.bits_off(st, b)
        sel = rs and don
        if not sel:
            C['rejected'] += 1
            C['rej_runlist'] += (not rs)
            C['rej_status'] += (not don)
            C['rej_both'] += (not rs and not don)
            if a.select == 'standard':
                continue
        rw = raw.get((run, en), {'HIT': []})['HIT']
        if not rw:
            C['empty'] += 1
            C['empty_no_probe'] += ((run, en) not in raw)
            continue
        H = ev['H']
        if len(rw) != len(H):
            C['nmismatch'] += 1
            continue
        if run not in cj:
            fn = os.path.join(d, f'calib/consts_{run}.json')
            cj[run] = CJC(load(fn)) if os.path.exists(fn) else None
            cjhash[run] = sha256(fn) if os.path.exists(fn) else None
        c = cj[run]
        if c is None:
            C['no_consts'] += 1
            continue
        prep = c.prepare_event(rw)
        # --- event T0 (D11)
        hat_t0 = st[3] if st is not None else None
        tt = [(flt(r[26]), abs(flt(r[27]))) for r in ev['CJKR'] if flt(r[26]) != 0.0 and flt(r[27]) != 0.0]
        if hat_t0 is not None and hat_t0 != 0.0:
            t0, src = hat_t0, 0
            C['t0_hat'] += 1
        else:
            if hat_t0 is not None:
                C['hat_t0_zero'] += 1
            if tt:
                t0, src = cjavt0([x for x, y in tt], [y for x, y in tt])[0], 2
                C['t0_cjavt0'] += 1
            else:
                t0, src = c.A0[0], 1
                C['t0_default'] += 1
        # --- CJC digis
        nd = len(rw)
        o = {n: [] for n, k in SCHEMA if k in ('vf', 'vi')}
        for k in range(nd):
            row = rw[k]
            w = row[0] if 0 <= row[0] < NWIRE else 0
            p = c.simple(prep[k], t0, 1.0)
            m = c.simple(prep[k], t0, -1.0)
            o['cjc_wire'].append(w)
            o['cjc_ring'].append(1 if w < 720 else 2)
            o['cjc_cell'].append(w // 24 if w < 720 else (w - 720) // 32)
            o['cjc_layer'].append(w % 24 if w < 720 else 24 + (w - 720) % 32)
            o['cjc_drift'].append(0.5 * (abs(p['drd_final']) + abs(m['drd_final'])) / c.CUNIT)
            o['cjc_drift_raw'].append(row[1] * 0.001)
            o['cjc_qplus'].append(float(row[2]))
            o['cjc_qminus'].append(float(row[3]))
            z = zcode(p['z'], C)
            C['hits_zcd_total'] += 1
            C['zcd_code_181'] += abs(z) == 181.0
            C['zcd_code_191'] += abs(z) == 191.0
            C['zcd_code_201'] += z == 201.0
            o['cjc_zcd'].append(z)
            o['cjc_wire_x'].append(c.c['cp_x'][w])
            o['cjc_wire_y'].append(c.c['cp_y'][w])
            o['cjc_xp'].append(p['x']); o['cjc_yp'].append(p['y'])
            o['cjc_xm'].append(m['x']); o['cjc_ym'].append(m['y'])
        o['cjc_reco_trk'] = [-1] * nd
        o['cjc_reco_side'] = [-1] * nd
        o['cjc_trk_x'] = [0.0] * nd; o['cjc_trk_y'] = [0.0] * nd; o['cjc_trk_z'] = [0.0] * nd
        # --- CJKR/CJKT/CJKV rows (extract_hits.C columns)
        for i, r in enumerate(ev['CJKR']):
            F = lambda cc: flt(r[cc])
            o['cjkr_kappa'].append(F(0)); o['cjkr_phi'].append(F(1)); o['cjkr_theta'].append(F(2))
            o['cjkr_dca'].append(F(3)); o['cjkr_z0'].append(F(4)); o['cjkr_rad'].append(F(5))
            o['cjkr_ndf'].append(r[16]); o['cjkr_chi2'].append(F(17)); o['cjkr_next'].append(r[18])
            o['cjkr_nhit_packed'].append(r[19]); o['cjkr_rend'].append(F(21)); o['cjkr_dedx'].append(F(22))
            o['cjkr_cov9'].extend(F(cc) for cc in range(7, 16))
        ntrk = len(ev['CJKR'])
        ncjc = [0] * ntrk
        # --- CJXR hit -> track assignment: rows ascend in track, the last track wins (as the CTJX walk in extract_hits.C)
        seen_k = {}
        for r in sorted(ev['XR'], key=lambda q: q[1]):
            x = xr_dict(r)
            if x['kdr'] <= 0 or x['itr'] < 1 or x['itr'] > ntrk:
                continue
            k = x['kdr'] - 1
            if k >= nd or prep[k]['row'] != H[k]:
                C['xr_map_bad'] += 1
                continue
            C['xr_multi'] += seen_k.get(k, 0) > 0
            seen_k[k] = seen_k.get(k, 0) + 1
            ncjc[x['itr'] - 1] += 1
            o['cjc_reco_trk'][k] = x['itr'] - 1
            o['cjc_reco_side'][k] = 0 if x['DRFE'] > 0 else 1
            o['cjc_trk_x'][k] = x['XHIT']; o['cjc_trk_y'][k] = x['YHIT']; o['cjc_trk_z'][k] = x['ZHIT']
        C['xr_assigned'] += sum(1 for v in o['cjc_reco_trk'] if v >= 0)
        o['cjkr_ncjc'] = ncjc
        C['ncjc_vs_packed_diff'] += sum(1 for i, r in enumerate(ev['CJKR']) if ncjc[i] != (r[19] & 255))
        C['cjkr_written'] += ntrk
        for r in ev['CJKT']:
            o['cjkt_kappa'].append(flt(r[0])); o['cjkt_phi'].append(flt(r[1])); o['cjkt_theta'].append(flt(r[2]))
            o['cjkt_ndf'].append(r[8]); o['cjkt_chi2'].append(flt(r[9]))
            o['cjkt_vtx'].append(r[12]); o['cjkt_cjkr'].append(r[14])
        for r in ev['CJKV']:
            o['cjkv_x'].append(flt(r[0])); o['cjkv_y'].append(flt(r[1])); o['cjkv_z'].append(flt(r[2]))
            o['cjkv_cjkt'].append(r[7]); o['cjkv_type'].append(r[8]); o['cjkv_aux'].append(r[9])
        for r in ev['CRZE']:                       # [wire, DL, Q+, Q-, flag1, flag2]
            o['coz_wire'].append(r[0]); o['coz_drift'].append(r[1])
            o['coz_qplus'].append(r[2]); o['coz_qminus'].append(r[3])
        if st is not None:
            o['hv_off'] = [int(i) for i in np.nonzero(np.asarray(st[0]) == 1)[0]]
            o['caen_off'] = [int(i) for i in np.nonzero(np.asarray(st[1]) == 1)[0]]
            o['bbl3_off'] = [int(i) for i in np.nonzero(np.asarray(st[2]) == 1)[0]]
        # --- fill
        sc['run'][0], sc['event'][0] = run, en
        sc['n_str_all'][0] = sc['n_svx_all'][0] = 0
        sc['is_data'][0] = 1
        sc['cjc_t0'][0] = t0
        sc['cjc_t0_src'][0] = src
        sc['run_selected'][0] = int(rs)
        sc['run_quality'][0] = rqual.get(run, 0)
        sc['det_on'][0] = int(don)
        for n, kind in SCHEMA:
            if kind in ('vf', 'vi'):
                v = vec[n]
                v.clear()
                if o.get(n):
                    v += o[n]
        tree.Fill()
        C['written'] += 1
        C['hits_written'] += nd
        C['hv_off_events'] += bool(o['hv_off'] if st is not None else False)
    fout.cd()
    tree.Write()
    ident = C['seen'] == C['written'] + (C['rejected'] if a.select == 'standard' else 0) + C['empty'] \
        + C['nmismatch'] + C['no_consts']
    meta = {'writer': os.path.abspath(__file__), 'writer_sha256': sha256(os.path.abspath(__file__)),
            'date': datetime.datetime.now().isoformat(timespec='seconds'), 'lam_dir': d, 'hat': hat, 'ndb': NDB,
            'select': a.select, 'max_events': a.max_events, 'schema_ref': 'extract_hits.C (MC0)',
            'h1hits_py': os.path.join(PY, 'h1hits.py'), 'h1hits_sha256': sha256(os.path.join(PY, 'h1hits.py')),
            'runlist_table': a.runlist_table, 'runlist_table_sha256': sha256(a.runlist_table),
            'runlist_root': pc.RUNLIST_ROOT, 'runlist_root_sha256': sha256(pc.RUNLIST_ROOT),
            'probe_files': {'events': os.path.join(d, 'calib/events.txt'), 'cjcprobe': os.path.join(d, 'cjc/cjcprobe.txt')},
            'consts_sha256': {str(r): h for r, h in sorted(cjhash.items())},
            'runs': sorted(cj), 'counters': C, 'seen_equals_written_rejected_empty_skips': ident,
            'd17': 'run list AND H1DetectorStatus::IsOn(CJC1,CJC2 incl BBL3 0); COZ/CST/CIP bits are flags only',
            't0_source_codes': '0 HAT T0_CJC, 1 run default ARC0J8(1), 2 cjavt0(CJKR)',
            'zcd_convention': 'MC: |z|>130 -> +-191; 181/-181 one end dead, 201 none; data 301 -> 201',
            'empty_branches': 'truth (str_*, svx_*, *_orig, cjc_truth_*, crjt_*, crvt_*, crzt_*, crmt_*, coz_str), CST digis '
                              '(WP4), cip_*/cop_* (WP5), cjcu_* (not in the dumps)'}
    nm = ROOT.TNamed('meta', json.dumps(meta))
    nm.Write()
    fout.Close()
    return meta


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--lam-dir', required=True)
    ap.add_argument('--runlist-table', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--max-events', type=int, default=10**9)
    ap.add_argument('--select', choices=('standard', 'none'), default='standard')
    a = ap.parse_args()
    meta = process_dir(a)
    print(json.dumps(meta['counters']))
    print('identity seen == written + rejected + empty + skips:', meta['seen_equals_written_rejected_empty_skips'])


if __name__ == '__main__':
    main()
