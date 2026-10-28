#!/usr/bin/env python3
"""Aggregate hatscan_v2_<label>.jsonl (h1-mc-expert 2026-10-28, WP-D round 2) -> hatscan_v2_aggregate.json + stdout table."""
import json, glob, os, sys
import numpy as np
O = '/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-d/mc_candidates/'
SIG = {'9012': 22972.115, '6670': 59574.145, '8221': 59528.395, '7448': 59735.414, '8405': 30209.441}   # pb, gen-log Tot x-sect (4489/4859/5536/5299)
LDB = {'9012': 1199996 / 22972.115, '6670': 274.85, '8221': 277.24, '7448': 276.45, '8405': 331.02}
agg = {}
for f in sorted(glob.glob(O + 'hatscan_v2_*.jsonl')):
    lab = os.path.basename(f)[11:-6]
    recs = [json.loads(l) for l in open(f) if l.strip()]
    a = {'files': len(recs), 'n': sum(r['n'] for r in recs), 'sum_w': sum(r['sum_w'] for r in recs),
         'n_hard': sum(r['n_hard'] for r in recs), 'n_q2gt150': sum(r['n_q2gt150'] for r in recs), 'sum_w_q2gt150': sum(r['sum_w_q2gt150'] for r in recs),
         'w1_min': min(r['w1_min'] for r in recs), 'w1_max': max(r['w1_max'] for r in recs), 'w2_ne1': sum(r['w2_ne1'] for r in recs),
         'pt2hat_min_phys': min(r['pt2hat_min_phys'] for r in recs if r['pt2hat_min_phys'] is not None),
         'y_min': min(r['y_q_all'][0] for r in recs), 'y_max': max(r['y_q_all'][-1] for r in recs), 'xpom_max': max(r['xpom_max_all'] for r in recs),
         'q2_min': min(r['q2_min_all'] for r in recs), 'q2_max': max(r['q2_max_all'] for r in recs),
         'genrad': {}, 'w_hist_all': list(np.sum([r['w_hist_all'] for r in recs], axis=0))}
    for r in recs:
        for k, v in r['genrad_hist'].items(): a['genrad'][k] = a['genrad'].get(k, 0) + v
    # weight vs Q2 (soft class), n-weighted mean
    wq = []
    for ib in range(len(recs[0]['w_mean_soft_vs_q2'])):
        ns = [r['w_mean_soft_vs_q2'][ib]['n'] for r in recs]; ws = [r['w_mean_soft_vs_q2'][ib]['w_mean'] or 0 for r in recs]
        wq.append({'bin': recs[0]['w_mean_soft_vs_q2'][ib]['bin'], 'n': int(sum(ns)), 'w_mean': float(np.dot(ns, ws) / max(1, sum(ns)))})
    a['w_mean_soft_vs_q2'] = wq
    for reg in ('R', 'R8', 'Rfid'):
        rr = [r[reg] for r in recs]
        d = {k: sum(x[k] for x in rr) for k in ('n', 'sum_w', 'sum_w2', 'n_hard', 'sum_w_hard', 'n_soft', 'sum_w_soft')}
        for k in ('mx_hist_all', 'mx_whist_all', 'mx_hist1_all', 'mx_hist_hard', 't_hist_all', 'xpom_hist_all', 'w_hist'):
            d[k] = [float(x) for x in np.sum([x[k] for x in rr], axis=0)]
        d['q2bins_all'] = [{'bin': rr[0]['q2bins_all'][i]['bin'], 'n': sum(x['q2bins_all'][i]['n'] for x in rr)} for i in range(len(rr[0]['q2bins_all']))]
        d['genrad'] = {}
        for x in rr:
            for k, v in x['genrad_hist'].items(): d['genrad'][k] = d['genrad'].get(k, 0) + v
        d['mx_median_files'] = float(np.median([x['mx_median'] for x in rr if x['mx_median'] is not None]))
        d['my_frac_lt1p6'] = float(np.mean([x['my_frac_lt1p6'] for x in rr if x['my_frac_lt1p6'] is not None]))
        a[reg] = d
    a['sigma_tot_pb'] = SIG.get(lab); a['L_db_pb-1'] = LDB.get(lab)
    a['L_sumw_over_sigma'] = a['sum_w'] / SIG[lab] if lab in SIG else None
    a['sigma_R_pb'] = a['R']['sum_w'] / a['L_sumw_over_sigma'] if a['L_sumw_over_sigma'] else None
    agg[lab] = a
json.dump(agg, open(O + 'hatscan_v2_aggregate.json', 'w'), indent=1)
for lab, a in agg.items():
    print('== %s files=%d n=%d sum_w=%.0f  L=sum_w/sigma=%.2f pb-1 (DB %.2f)  w1[%.3f,%.3f] w2!=1:%d  hard %.4f  pt2min %.2f  Q2[%.2f,%.0f] y[%.4f,%.4f] xpom<=%.3f genrad %s' % (
        lab, a['files'], a['n'], a['sum_w'], a['L_sumw_over_sigma'] or -1, a['L_db_pb-1'] or -1, a['w1_min'], a['w1_max'], a['w2_ne1'], a['n_hard'] / a['n'], a['pt2hat_min_phys'], a['q2_min'], a['q2_max'], a['y_min'], a['y_max'], a['xpom_max'], a['genrad']))
    print('   <w>soft vs Q2:', ['%g-%g:%.3f' % (b['bin'][0], b['bin'][1], b['w_mean']) for b in a['w_mean_soft_vs_q2']])
    for reg in ('R', 'R8', 'Rfid'):
        d = a[reg]
        print('   %5s n=%6d sum_w=%.1f (neff=%.0f) hard=%d (%.3f) sigma_reg=%.2f pb  MX[<4,4-10,10-20,20-40,>40]=%s  MX1=%s  t<0.1 frac %.3f  xpom bins %s  MY<1.6 %.4f  genrad %s' % (
            reg, d['n'], d['sum_w'], d['sum_w']**2 / max(1e-9, d['sum_w2']), d['n_hard'], d['n_hard'] / max(1, d['n']), d['sum_w'] / a['L_sumw_over_sigma'] if a['L_sumw_over_sigma'] else -1,
            [int(x) for x in d['mx_hist_all']], [int(x) for x in d['mx_hist1_all']], d['t_hist_all'][0] / max(1, sum(d['t_hist_all'])), [int(x) for x in d['xpom_hist_all']], d['my_frac_lt1p6'], d['genrad']))
