#!/usr/bin/env python3
"""WP-D round 3 (h1-mc-expert 2026-10-28): write H1SteerTree steering files (comma-separated hat/mods lists)
and the Condor queue for McCandidateXScan. 8405/8403: all files in 4 parts; 9012: all 24 files; 8402/8404 (gen
twins of 8405/8403, same generator stream): first 8 chunks only (event-identity + per-period z-vtx/run check)."""
import glob, os
S = '/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-d/mc_candidates'
P = '/pnfs/desy.de/dphep/online/h1/mc/oo-4.0/rapgap31/'
plan = {'8405': 4, '8403': 4, '9012': 1, '8402': ('first', 8), '8404': ('first', 8)}
q = []
for req, spec in plan.items():
    hats = sorted(glob.glob(P + req + '/hat.*.root'))
    if isinstance(spec, tuple): hats = hats[:spec[1]]; nparts = 1
    else: nparts = spec
    for ip in range(nparts):
        sub = hats[ip::nparts]
        mods = [h.replace('/hat.', '/mods.') for h in sub]
        assert all(os.path.exists(m) for m in mods), req
        lab = '%s_p%d' % (req, ip)
        with open('%s/pkg_xscan/steering/xscan_%s.steer' % (S, lab), 'w') as f:
            f.write('// WP-D round-3 X-system scan, ReqID %s part %d/%d (%d files), gen-level only.\nH1SteerTree() {\n' % (req, ip, nparts, len(sub)))
            f.write('   fHatFiles  = "%s";\n   fModsFiles = "%s";\n   fLoadODS   = false;\n}\n' % (','.join(sub), ','.join(mods)))
        q.append(lab); print(lab, len(sub))
open(S + '/queue_r3.txt', 'w').write('\n'.join(q) + '\n')
