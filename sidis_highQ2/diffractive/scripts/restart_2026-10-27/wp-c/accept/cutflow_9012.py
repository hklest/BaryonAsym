#!/usr/bin/env python3
"""WP-C acceptance chain: 9012 elastic-uds cut flow, weighted to several lumi bases. ONE file, login node."""
import json,numpy as np,uproot
D='/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/'
B=['passRecoEnv','passGenEnv','Q2s','Ys','hatVtxZ','BeamElecE','BeamProtP','etaMaxVtxExcl08','selFmd','ePlug','selFts','plugPass7','hatMYGen','hatMXGen','hatXpomGki','hatTGki','xIp','mx','weight1','weight2']
z=uproot.open(D+'data/t2_detector/mc9012_merged_2026-10-18/9012.root')['DiffTot'].arrays(B,library='np')
LMC=52273.3
bases={'signed_x4(T0)':4*6.714705*LMC/LMC*1.0,'certified_4per':317212.804/LMC,'em0405_cert':98087.051/LMC,'em0405_precert_snap':103398.440/LMC,'em0405_precert_GRL':105028.172/LMC}
bases['signed_x4(T0)']=26.858820
x=z['Q2s']/(4*z['BeamElecE']*z['BeamProtP']*np.where(z['Ys']!=0,z['Ys'],np.nan))
fid=(z['hatMYGen']<1.6)&(z['hatXpomGki']<.03)&(z['hatMXGen']>2)&(np.abs(z['hatTGki'])<1)
pre=z['passRecoEnv']&(z['Q2s']>150)&(z['Ys']>.1)&(z['Ys']<.9)&(np.abs(z['hatVtxZ'])<35)
eta=z['etaMaxVtxExcl08'];etaok=(eta<3.2)|~np.isfinite(eta)|(eta<-900)
steps=[('stage1 all',np.ones(len(fid),bool)),('truth fiducial (all stage1)',fid),('truth fid & passGenEnv',fid&z['passGenEnv']),
('truth fid & passRecoEnv',fid&z['passRecoEnv']),('truth fid & parent proxy',fid&pre),
('+eta_max<3.2',fid&pre&etaok),('+FMD',fid&pre&etaok&z['selFmd']),('+Plug<7',fid&pre&etaok&z['selFmd']&(z['ePlug']<7)),('+FTS = candidate&fid',fid&pre&etaok&z['selFmd']&(z['ePlug']<7)&z['selFts'])]
cand=pre&etaok&z['selFmd']&(z['ePlug']<7)&z['selFts']
steps+=[('candidate (no truth cut)',cand),('cand & reco xIp<0.03',cand&(z['xIp']<.03)),('cand & reco mx>2',cand&(z['mx']>2)),('cand & 4<mx<40',cand&(z['mx']>4)&(z['mx']<40)),
('cand & xIp<.03 & 4<mx<40',cand&(z['xIp']<.03)&(z['mx']>4)&(z['mx']<40)),('cand&fid&4<mx<40 (reco)',cand&fid&(z['mx']>4)&(z['mx']<40))]
out={'w1w2_unique':[float(v) for v in np.unique(z['weight1']*z['weight2'])[:3]],'bases':bases,'steps':{}}
print('w1*w2',out['w1w2_unique'])
print('%-30s %8s'%('step','N_MC')+''.join('%16s'%k for k in bases))
for n,m in steps:
    N=int(m.sum());out['steps'][n]=N
    print('%-30s %8d'%(n,N)+''.join('%16.0f'%(N*v) for v in bases.values()))
json.dump(out,open(D+'data/restart_2026-10-27/wp-c/accept/cutflow_9012.json','w'),indent=1)
