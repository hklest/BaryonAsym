#!/usr/bin/env python3
"""WP-A2 fork of scripts/restart_2026-10-27/wp-a/fit_7p1_wpa.py (child plan wp-a2_plan.md D9, S3/S4).
With --templates 2 (default) the arithmetic is unchanged.  --templates 3 splits the
diffractive template into ELASTIC+CHARM {9012,9024,9033} and PDISS+REGGEON
{9155,9157,9158,6671,6677}; the second's scale is t*a with t bounded to
--pdiss-bound (default 0.5,2.0) times its v3 ratio to the first (= 1, the
templates carry the v3 weights), the published range of arXiv:1203.4495 §4
(s1_definition_audit.md R4).  Same deterministic multistart, Poisson deviance and
toy method (both/all scales refit per toy).  Non-uniform edges (merged edge
bin) are supported; per-bin CSV edges are taken from the payload.

WP-A DOCSTRING: §7.1 full-range two-free-template fit, WP-A parameterised fork of
scripts/t4/run_section_7_1_twofree_v3_2026-10-20.py (fit/deviance/multistart/profile/
coarse/scan/toy arithmetic unchanged). Adds payload/outdir/n-toys/alt/nd-labels/
fac-override/scope/binning options. Toy p = (count+1)/(n_toys+1), floor 1/(n_toys+1)."""
import argparse, csv, hashlib, json, math, sys
from pathlib import Path
import numpy as np
from scipy.optimize import minimize, minimize_scalar
from scipy.special import gammaincc

ROOT=Path(__file__).resolve().parents[2]
IN=ROOT/'data/t4/datamc_cutremoved_eta_7p1_2026-10-20/assembled_v3/cutremoved_fullrange_eta_templates_7p1.json'
OUT=ROOT/'data/t4/leakage_fit_2026-10-19/section_7_1_fit'
CELLS=('D0','D1','D2'); DIFF=('9012','9024','9033','9155','9157','9158','6671','6677'); ELCH=DIFF[:3]; PDREG=DIFF[3:]

def dterms(y,m):
    y=np.asarray(y,float);m=np.maximum(np.asarray(m,float),1e-12)
    z=2*(m-y);q=y>0;z[q]+=2*y[q]*np.log(y[q]/m[q]);return z

def dev(y,m): return float(dterms(y,m).sum())
def fit(y,d,n,starts=None):
    """Non-negative two-template Poisson-deviance MLE, deterministic multistart."""
    f=lambda q:dev(y,q[0]*d+q[1]*n)
    # Total-yield starts plus pure-template corners; no arbitrary upper boundary.
    sy=float(y.sum()); sd=float(d.sum()); sn=float(n.sum())
    ss=[[1,1],[sy/sd if sd else 1,0],[0,sy/sn if sn else 1],[sy/(2*sd) if sd else 1,sy/(2*sn) if sn else 1]]
    if starts: ss+=starts
    best=None
    for s in ss:
        r=minimize(f,np.maximum(s,0),method='L-BFGS-B',bounds=((0,None),(0,None)),options={'ftol':1e-12,'gtol':1e-10,'maxiter':5000})
        if best is None or r.fun<best.fun: best=r
    q=np.maximum(best.x,0); return q,float(best.fun)

def fit3(y,d1,d2,n,tb):
    """Three-template Poisson-deviance MLE: y ~ a*d1 + a*t*d2 + c*n, a,c>=0, t in tb."""
    f=lambda q:dev(y,q[0]*d1+q[0]*q[1]*d2+q[2]*n)
    sy=float(y.sum()); sd=float((d1+d2).sum()); sn=float(n.sum())
    best=None
    for t0 in (tb[0],1.0,tb[1]):
        for s in ([1,t0,1],[sy/sd if sd else 1,t0,0],[0,t0,sy/sn if sn else 1],[sy/(2*sd) if sd else 1,t0,sy/(2*sn) if sn else 1]):
            r=minimize(f,np.array(s,float),method='L-BFGS-B',bounds=((0,None),tuple(tb),(0,None)),options={'ftol':1e-12,'gtol':1e-10,'maxiter':5000})
            if best is None or r.fun<best.fun: best=r
    q=np.array([max(best.x[0],0),min(max(best.x[1],tb[0]),tb[1]),max(best.x[2],0)]); return q,float(best.fun)

def toy_gof3(y,d1,d2,n,raw_components,q,observed,seed,tb,n_toys=300):
    rng=np.random.default_rng(seed); vals=[]
    for _ in range(n_toys):
        a1=np.zeros_like(d1);a2=np.zeros_like(d2);nn=np.zeros_like(n)
        for lab,raw,wgt in raw_components:
            u=rng.poisson(raw)*np.divide(wgt,raw,out=np.zeros_like(wgt),where=raw>0)
            if lab in ELCH:a1+=u
            elif lab in PDREG:a2+=u
            else:nn+=u
        yy=rng.poisson(np.maximum(q[0]*d1+q[0]*q[1]*d2+q[2]*n,0))
        vals.append(fit3(yy,a1,a2,nn,tb)[1])
    vals=np.asarray(vals)
    return {'method':'parametric bootstrap as toy_gof; three templates, a, t (bounded) and c refit','n_toys':n_toys,'observed_deviance':observed,'toy_median':float(np.median(vals)),'toy_p_ge_observed':float((np.count_nonzero(vals>=observed)+1)/(n_toys+1)),'p_floor':1.0/(n_toys+1)}

def prof_interval(y,d,n,q,j):
    """Delta-D=1 profile interval for q[j], profiling the other nonnegative scale."""
    other=1-j; minimum=dev(y,q[0]*d+q[1]*n); target=minimum+1
    def p(v):
        rr=minimize_scalar(lambda u:dev(y,(v*d+u*n) if j==0 else (u*d+v*n)),bounds=(0,max(1,3*q[other]+3)),method='bounded',options={'xatol':1e-10})
        # the bounded bracket can be too short in a near-flat direction
        u=rr.x; hi=max(1,3*u+3)
        while dev(y,(v*d+hi*n) if j==0 else (hi*d+v*n)) < dev(y,(v*d+u*n) if j==0 else (u*d+v*n)) and hi<1e7: hi*=2
        rr=minimize_scalar(lambda x:dev(y,(v*d+x*n) if j==0 else (x*d+v*n)),bounds=(0,hi),method='bounded',options={'xatol':1e-10})
        return float(rr.fun),float(rr.x)
    center=float(q[j]); lo=0.0
    if center>0 and p(0)[0]>target:
        a,b=0.,center
        for _ in range(80):
            m=(a+b)/2
            if p(m)[0]>target:a=m
            else:b=m
        lo=(a+b)/2
    # find upper crossing, explicitly record no-crossing only if it reaches a huge scale
    a=center;b=max(1,2*center+1)
    while p(b)[0]<target and b<1e7:b*=2
    if p(b)[0]>=target:
        for _ in range(80):
            m=(a+b)/2
            if p(m)[0]>target:b=m
            else:a=m
        hi=(a+b)/2
    else: hi=None
    return {'minus':center-lo,'plus':None if hi is None else hi-center,'lower':lo,'upper':hi,
            'profile_at_zero':p(0)[0]-minimum,'interior_minimum':bool(center>1e-7)}

def coarse(y,*a):
    groups=[]; st=0; tot=0.
    for i,v in enumerate(y):
        tot+=v
        if tot>=100: groups.append((st,i+1));st=i+1;tot=0
    if st<len(y):
        if groups: groups[-1]=(groups[-1][0],len(y))
        else: groups=[(0,len(y))]
    return (*[np.array([x[l:r].sum() for l,r in groups]) for x in (y,)+a],groups)

def scan(y,d,n,q):
    # 61x61 surface, 0..2.5 times the fitted coordinate (plus unit range at zero)
    amax=max(1.,2.5*q[0]+1); bmax=max(1.,2.5*q[1]+1)
    aa=np.linspace(0,amax,61);bb=np.linspace(0,bmax,61);mn=dev(y,q[0]*d+q[1]*n)
    return {'s_diffractive_grid':aa.tolist(),'s_ND_grid':bb.tolist(),
      'delta_deviance':[[dev(y,a*d+b*n)-mn for b in bb] for a in aa]}

def toy_gof(y,d,n,raw_components,nd_labels,q,observed,seed,n_toys=300):
    rng=np.random.default_rng(seed); vals=[]
    for _ in range(n_toys):
        dd=np.zeros_like(d);nn=np.zeros_like(n)
        for lab,raw,wgt in raw_components:
            u=rng.poisson(raw)*np.divide(wgt,raw,out=np.zeros_like(wgt),where=raw>0)
            if lab in nd_labels:nn+=u
            else:dd+=u
        yy=rng.poisson(np.maximum(q[0]*d+q[1]*n,0))
        vals.append(fit(yy,dd,nn)[1])
    vals=np.asarray(vals)
    return {'method':'parametric bootstrap: Poisson DATA and independent per-bin raw-MC template counts; delivered weighted/raw conversion held fixed; both scales refit','n_toys':n_toys,'observed_deviance':observed,'toy_median':float(np.median(vals)),'toy_p_ge_observed':float((np.count_nonzero(vals>=observed)+1)/(n_toys+1)),'p_floor':1.0/(n_toys+1)}

def run3(args,iscope,y,d1,d2,n,rr,NT,TB,alt,result,rows,edges,NB):
 """Three-template branch: fine and/or coarse fit, toy GoF; no profile scans."""
 for b in ('fine','coarse'):
  if args.binning not in (b,'both'): continue
  if b=='fine': yy,a1,a2,nn,src=y,d1,d2,n,rr; groups=None
  else:
   yy,a1,a2,nn,groups=coarse(y,d1,d2,n)
   src=[(lab,np.array([r[l:h].sum() for l,h in groups]),np.array([w[l:h].sum() for l,h in groups])) for lab,r,w in rr]
  q,D=fit3(yy,a1,a2,nn,TB)
  rec={'scope':iscope,'templates':3,'a_elastic_charm':float(q[0]),'t_pdiss_over_elastic':float(q[1]),'s_pdiss_reggeon':float(q[0]*q[1]),'s_ND':float(q[2]),
       'pdiss_bound':list(TB),'t_at_bound':bool(abs(q[1]-TB[0])<1e-6 or abs(q[1]-TB[1])<1e-6),'deviance':D,'ndf':len(yy)-3,
       'p_asym':float(gammaincc(.5*(len(yy)-3),.5*D)),'data':yy.tolist(),'prediction':(q[0]*a1+q[0]*q[1]*a2+q[2]*nn).tolist(),
       'deviance_contribution':dterms(yy,q[0]*a1+q[0]*q[1]*a2+q[2]*nn).tolist(),'groups':groups}
  seed=(41020 if b=='fine' else 51020)+len(alt)*10+len(iscope)+3
  rec['template_statistics_aware_gof']=toy_gof3(yy,a1,a2,nn,src,q,D,seed,TB,n_toys=NT)
  key='fine' if b=='fine' else 'coarse_ge100_data'
  result[key][alt][iscope]=rec
  for i,(u,pp,dt) in enumerate(zip(rec['data'],rec['prediction'],rec['deviance_contribution'])):
   rows.append(['fine' if b=='fine' else 'coarse_ge100',alt,iscope,i,edges[i%NB] if b=='fine' else '',edges[i%NB+1] if b=='fine' else '',u,pp,dt])

def main():
 ap=argparse.ArgumentParser()
 ap.add_argument('--payload',default=str(IN)); ap.add_argument('--outdir',required=True)
 ap.add_argument('--n-toys',type=int,default=300); ap.add_argument('--alt',required=True)
 ap.add_argument('--nd-labels',default=None); ap.add_argument('--fac-override',action='append',default=[])
 ap.add_argument('--scope',default='all',choices=['D0','D1','D2','ALL','all'])
 ap.add_argument('--binning',default='both',choices=['fine','coarse','both'])
 ap.add_argument('--templates',type=int,default=2,choices=[2,3]); ap.add_argument('--pdiss-bound',default='0.5,2.0')
 args=ap.parse_args(); NT=args.n_toys; OUT=Path(args.outdir); alt=args.alt; TB=tuple(float(v) for v in args.pdiss_bound.split(','))
 ND_LABELS=tuple(args.nd_labels.split(',')) if args.nd_labels else (alt,)
 IN_=Path(args.payload); payload_sha=hashlib.sha256(IN_.read_bytes()).hexdigest()
 x=json.load(open(IN_)); assert x['pass'] and x['conservation']['all_pass']
 edges=np.asarray(x['edges'],float); c=x['modes']['cluster']; fac=dict(x['scale_table_basis']['exposure_factors'])
 for ov in args.fac_override:
  k,v=ov.split('=',1); fac[k]=float(v)
 data={z:np.asarray(c['data']['templates']['raw'][z],float) for z in CELLS}
 def weighted(lab,z):return np.asarray(c['components'][lab]['templates']['exposure1'][z],float)*fac[lab]
 def raw(lab,z):return np.asarray(c['components'][lab]['templates']['raw'][z],float)
 diff={z:sum((weighted(k,z) for k in DIFF),np.zeros(len(edges)-1)) for z in CELLS}
 nd={alt:{z:sum((weighted(l,z) for l in ND_LABELS),np.zeros(len(edges)-1)) for z in CELLS}}
 el={z:sum((weighted(k,z) for k in ELCH),np.zeros(len(edges)-1)) for z in CELLS}; pr={z:sum((weighted(k,z) for k in PDREG),np.zeros(len(edges)-1)) for z in CELLS}
 NB=len(edges)-1
 result={'schema':'t4-section-7-1-two-free-template-v3','arithmetic_only':True,'input':str(IN_),'payload_sha256':payload_sha,'argv':sys.argv,'n_toys':NT,'nd_labels':list(ND_LABELS),'fac_used':{l:fac[l] for l in DIFF+ND_LABELS},'input_sha_basis':'assembled_v3 only; v2-bound payload forbidden','fit_definition':'full [-1,5] cluster eta, binned Poisson deviance, nonnegative two-free-template fit DATA=a_diff*D+b_ND*T_ND','diffractive_template_components':list(DIFF),'diffractive_template_interpretation':'ODS signal (9012,9024,9033,9155,9157,9158) plus Reggeon (6671,6677), with ruled relative weights fixed internally and one free common normalization','nd_alternatives':'one alternative per run (--alt); ND template = sum over --nd-labels of exposure1*fac','fine':{},'coarse_ge100_data':{},'metadata':{},'templates':args.templates,'pdiss_bound':list(TB) if args.templates==3 else None,'edges':edges.tolist()}
 rows=[]
 for alt in (alt,):
  result['fine'][alt]={};result['coarse_ge100_data'][alt]={}
  scopes={z:(data[z],diff[z],nd[alt][z],el[z],pr[z]) for z in CELLS}
  scopes['ALL']=tuple(np.concatenate([X[z] for z in CELLS]) for X in (data,diff,nd[alt],el,pr))
  for iscope,(y,d,n,d1,d2) in scopes.items():
   if args.scope not in ('all',iscope): continue
   # raw sources for MC-stat toy, concatenated for ALL
   rr=[]
   for lab in DIFF+ND_LABELS:
    r=np.concatenate([raw(lab,z) for z in CELLS]) if iscope=='ALL' else raw(lab,iscope)
    w=np.concatenate([weighted(lab,z) for z in CELLS]) if iscope=='ALL' else weighted(lab,iscope)
    rr.append((lab,r,w))
   if args.templates==3:
    run3(args,iscope,y,d1,d2,n,rr,NT,TB,alt,result,rows,edges,NB); continue
   if args.binning in ('fine','both'):
    q,D=fit(y,d,n); pi=[prof_interval(y,d,n,q,j) for j in (0,1)]
    # retained sample means same payload with eta<3.2; explicitly exclude eta under/overflow from fraction denominator.
    keep=edges[1:]<=3.2+1e-10
    if iscope=='ALL':
     kr=np.concatenate([edges[1:]<=3.2+1e-10 for z in CELLS]); denom=float(sum(data[z][keep].sum() for z in CELLS)); nret=float(sum(nd[alt][z][keep].sum() for z in CELLS))
    else: denom=float(y[keep].sum());nret=float(n[keep].sum())
    frac=q[1]*nret/denom if denom else float('nan')
    rec={'scope':iscope,'data_fullrange':float(y.sum()),'diff_template_fullrange':float(d.sum()),'nd_template_fullrange':float(n.sum()),'s_diffractive':float(q[0]),'s_ND':float(q[1]),'profile_s_diffractive':pi[0],'profile_s_ND':pi[1],'deviance':D,'ndf':len(y)-2,'p_asym':float(gammaincc(.5*(len(y)-2),.5*D)),'boundary_flags':{'s_diffractive_zero':not pi[0]['interior_minimum'],'s_ND_zero':not pi[1]['interior_minimum']},'data':y.tolist(),'prediction':(q[0]*d+q[1]*n).tolist(),'deviance_contribution':dterms(y,q[0]*d+q[1]*n).tolist(),'profile_scan':scan(y,d,n,q),'cut_retained_eta_lt_3p2':{'data':denom,'ND_prediction':float(q[1]*nret),'ND_fraction':float(frac),'ND_fraction_minus':None if pi[1]['minus'] is None else float(pi[1]['minus']*nret/denom),'ND_fraction_plus':None if pi[1]['plus'] is None else float(pi[1]['plus']*nret/denom),'note':'profile uncertainty propagates b_ND with s_diffractive profiled; denominator is observed cut-retained DATA'}}
    rec['template_statistics_aware_gof']=toy_gof(y,d,n,rr,ND_LABELS,q,D,41020+len(alt)*10+len(iscope),n_toys=NT)
    result['fine'][alt][iscope]=rec
    for i,(yy,pp,dt) in enumerate(zip(rec['data'],rec['prediction'],rec['deviance_contribution'])):rows.append(['fine',alt,iscope,i,edges[i%NB],edges[i%NB+1],yy,pp,dt])
   if args.binning in ('coarse','both'):
    cy,cd,cn,groups=coarse(y,d,n);cq,cD=fit(cy,cd,cn); cp=[prof_interval(cy,cd,cn,cq,j) for j in (0,1)]
    cr={'scope':iscope,'n_groups':len(groups),'groups':groups,'s_diffractive':float(cq[0]),'s_ND':float(cq[1]),'profile_s_diffractive':cp[0],'profile_s_ND':cp[1],'deviance':cD,'ndf':len(cy)-2,'p_asym':float(gammaincc(.5*(len(cy)-2),.5*cD)),'data':cy.tolist(),'prediction':(cq[0]*cd+cq[1]*cn).tolist(),'deviance_contribution':dterms(cy,cq[0]*cd+cq[1]*cn).tolist(),'note':'pre-registered left-to-right adjacent grouping, every group >=100 DATA except impossible only-group case'}
    coarse_sources=[]
    for lab, r, w in rr:
     coarse_sources.append((lab,np.array([r[l:h].sum() for l,h in groups]),np.array([w[l:h].sum() for l,h in groups])))
    cr['template_statistics_aware_gof']=toy_gof(cy,cd,cn,coarse_sources,ND_LABELS,cq,cD,51020+len(alt)*10+len(iscope),n_toys=NT)
    result['coarse_ge100_data'][alt][iscope]=cr
    for i,(yy,pp,dt) in enumerate(zip(cr['data'],cr['prediction'],cr['deviance_contribution'])):rows.append(['coarse_ge100',alt,iscope,i,'','',yy,pp,dt])

 result['metadata']['high_eta_gt3p2']={z:{'data':float(data[z][edges[:-1]>=3.2].sum()),'fraction_of_fullrange_data':float(data[z][edges[:-1]>=3.2].sum()/data[z].sum()),alt+'_template':float(nd[alt][z][edges[:-1]>=3.2].sum())} for z in CELLS}
 OUT.mkdir(parents=True,exist_ok=True)
 sfx='' if (args.binning=='both' and args.scope=='all') else f'_{alt}_{args.binning}_{args.scope}'
 json.dump(result,open(OUT/f'section_7_1_twofree_fit_results{sfx}.json','w'),indent=2)
 with open(OUT/f'per_bin_data_prediction_deviance{sfx}.csv','w',newline='') as f:
  w=csv.writer(f);w.writerow(['binning','alternative','scope','bin_index','eta_low','eta_high','data','prediction','deviance_contribution']);w.writerows(rows)
 with open(OUT/f'fit_summary{sfx}.csv','w',newline='') as f:
  w=csv.writer(f);w.writerow(['binning','alternative','scope','s_diffractive','sd_minus','sd_plus','s_ND','sn_minus','sn_plus','deviance','ndf','asym_p','toy_p','retained_nd_fraction','retained_fraction_minus','retained_fraction_plus'])
  for b in ('fine','coarse_ge100_data'):
   for a in (alt,):
    for s,r in result[b][a].items():
     cc=r.get('cut_retained_eta_lt_3p2',{}); pd_=r.get('profile_s_diffractive',{}); pn_=r.get('profile_s_ND',{})
     w.writerow([b,a,s,r.get('s_diffractive',r.get('a_elastic_charm')),pd_.get('minus'),pd_.get('plus'),r['s_ND'],pn_.get('minus'),pn_.get('plus'),r['deviance'],r['ndf'],r['p_asym'],r.get('template_statistics_aware_gof',{}).get('toy_p_ge_observed'),cc.get('ND_fraction'),cc.get('ND_fraction_minus'),cc.get('ND_fraction_plus')])
 print(json.dumps({'out':str(OUT),'toy_p':{b:{s:r['template_statistics_aware_gof']['toy_p_ge_observed'] for s,r in result[b][alt].items()} for b in ('fine','coarse_ge100_data')}},indent=2))
if __name__=='__main__':main()
