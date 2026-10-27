"""Greedy merge proposal of bin edges per (M_X bin, axis) for one frame. Criteria: n_sig_pred>=50 both charges (D7),
purity>=PMIN and stability>=SMIN both charges, width >= 2*core sigma(reco-gen) of tracks in the bin."""
import sys, json, csv, numpy as np
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
T,E=load(); w=T["t_w"]; gv=T["t_gv"]; mxr=T["t_mx"]; mxg=T["t_gmx"]; ch=T["t_charge"]
NMIN=50; PMIN=float(sys.argv[2]) if len(sys.argv)>2 else 0.35; SMIN=0.35
frame=sys.argv[1]

FINE={"xF":np.array([-1,-.7,-.5,-.4,-.3,-.2,-.15,-.1,-.05,0,.05,.1,.15,.2,.3,.4,.5,.7,1.]),
      "pt2":np.array([0,.05,.1,.15,.2,.3,.4,.5,.75,1.,1.5,2,3,5,10.])}
OBS={"xF":("t_xf"+frame,"t_xf1"),"pt2":("t_pt2"+frame,"t_pt21")}

def metrics(ax,elist):
    rc,gc=OBS[ax]; out=[]
    for c in (1,-1):
        m=ch==c
        r=pur_stab(mxr[m],T[rc][m],mxg[m],T[gc][m],gv[m],w[m],MX_EDGES,elist)
        r2=pur_stab(mxr[m&gv],T[rc][m&gv],mxg[m&gv],T[gc][m&gv],gv[m&gv],w[m&gv],MX_EDGES,elist)
        out.append((r,r2))
    return out

def sig_local(ax,elist,i):
    """core sigma (reco-gen) of gv tracks per bin of M_X bin i"""
    rc,gc=OBS[ax]; e=elist[i]; res=[]
    m0=gv&(mxg>=MX_EDGES[i])&(mxg<MX_EDGES[i+1]+(1e-9 if i==2 else 0))
    for a,b in zip(e[:-1],e[1:]):
        m=m0&(T[gc]>=a)&(T[gc]<b)
        res.append(wstats(T[rc][m]-T[gc][m],w[m])["core"] if m.sum()>20 else np.nan)
    return np.array(res)

def offs(elist): 
    o=[0]
    for e in elist: o.append(o[-1]+len(e)-1)
    return o

def viol(ax,elist,i):
    M=metrics(ax,elist); o=offs(elist); sl=slice(o[i],o[i+1]); e=elist[i]
    n=np.minimum(M[0][1]["w"]["nreco"][sl],M[1][1]["w"]["nreco"][sl])
    p=np.minimum(M[0][0]["w"]["purity"][sl],M[1][0]["w"]["purity"][sl]); s=np.minimum(M[0][0]["w"]["stab"][sl],M[1][0]["w"]["stab"][sl])
    sg=sig_local(ax,elist,i); wd=np.diff(e)
    score=np.zeros(len(n))
    n=np.where(np.isfinite(n),n,0); p=np.where(np.isfinite(p),p,0); s=np.where(np.isfinite(s),s,0)
    # badness: relative shortfall
    bad=np.maximum.reduce([np.maximum(0,1-n/NMIN),np.maximum(0,1-p/PMIN),np.maximum(0,1-s/SMIN),np.where(np.isfinite(sg),np.maximum(0,1-wd/(2*sg)),0)])
    return bad,n,p,s,sg

res={}; elist=[FINE["xF"].copy()]*3
for ax in ("xF","pt2"):
    elist=[FINE[ax].copy() for _ in range(3)]
    for i in range(3):
        while True:
            bad,n,p,s,sg=viol(ax,elist,i)
            if bad.max()<=1e-9 or len(elist[i])<=2: break
            j=int(np.argmax(bad)); e=elist[i]
            # candidates: remove left edge (merge with left) or right edge
            cands=[k for k in (j,j+1) if 0<k<len(e)-1]
            if not cands: break
            best=None
            for k in cands:
                e2=np.delete(e,k); el2=list(elist); el2[i]=e2
                b2=viol(ax,el2,i)[0]; sc=(b2.max(),b2.sum())
                if best is None or sc<best[0]: best=(sc,k)
            elist[i]=np.delete(e,best[1])
    res[ax]=[e.tolist() for e in elist]
    for i in range(3):
        bad,n,p,s,sg=viol(ax,elist,i)
        print(frame,ax,"MX bin",i,"edges",np.round(elist[i],3).tolist(),"\n   n_min",n.round(0),"pur_min",p.round(2),"stab_min",s.round(2),"sig",np.round(sg,3),"width/2sig",np.round(np.diff(elist[i])/(2*sg),1),"bad max %.2f"%bad.max())
json.dump(res,open(BASE+"proposal_frame%s_pmin%.2f.json"%(frame,PMIN),"w"),indent=1)
