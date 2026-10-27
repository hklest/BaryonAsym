import sys, numpy as np
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
T,E=load(); w=T["t_w"]; gv=T["t_gv"]
np.set_printoptions(linewidth=220,precision=3,suppress=True)
# event-level fake reasons
sel=np.ones(len(E["e_w"]),bool); fid=E["e_genfid"]; ew=E["e_w"]
print("sel events weighted",ew.sum().round(0),"genfid frac (w)",(ew[fid].sum()/ew.sum()).round(3))
nf=~fid
for nm,c in (("mirXip>=0.03",E["e_mirXip"]>=0.03),("mirMX>40",E["e_mirMX"]>40),("mirMX<4",E["e_mirMX"]<4),("mirMY>=1.6",E["e_mirMY"]>=1.6),("|t|>=1",np.abs(E["e_mirT"])>=1),("Q2t<150",E["e_Q2t"]<150),("yt out",(E["e_yt"]<.1)|(E["e_yt"]>.7))):
    print("  not-fid events with",nm,":",(ew[nf&c].sum()/ew[nf].sum()).round(3))
print("per sample genfid frac(w):",[(LABELS[i],(ew[(E['e_samp']==i)&fid].sum()/ew[E['e_samp']==i].sum()).round(2)) for i in range(6)])
print("Empz>45 variant event count/Empz35-70 count:",E["e_empz45"].mean().round(3))
# N_D per reco M_X bin (selected events, weighted) and raw
for nm,x in (("reco mx",E["e_mx"]),("gen mirMX (genfid only)",np.where(fid,E["e_mirMX"],np.nan))):
    for lo,hi in zip(MX_EDGES[:-1],MX_EDGES[1:]):
        m=(x>=lo)&(x<hi+(1e-9 if hi==40 else 0)); print("N_D %s [%d,%d): raw %d weighted %.0f"%(nm,lo,hi,m.sum(),ew[m].sum()))
m=(E["e_mx"]>=3)&(E["e_mx"]<4); print("reco mx in [3,4):",m.sum(),ew[m].sum().round(0)); m=E["e_mx"]>40;print("reco mx>40:",m.sum(),ew[m].sum().round(0))
# distributions of x_F, pt2 for gv tracks per M_X bin (gen G1), weighted percentiles
def wq(x,ww,p):
    o=np.argsort(x); c=np.cumsum(ww[o]); c/=c[-1]; return x[o][np.searchsorted(c,p)]
for lo,hi in zip(MX_EDGES[:-1],MX_EDGES[1:]):
    m=gv&(T["t_gmx"]>=lo)&(T["t_gmx"]<hi)&np.isfinite(T["t_xf1"])
    print("gen MX[%d,%d) xF pct 1,5,25,50,75,95,99:"%(lo,hi),[round(wq(T["t_xf1"][m],w[m],p),3) for p in (.01,.05,.25,.5,.75,.95,.99)],
          " pt2 pct 25,50,75,90,99:",[round(wq(T["t_pt21"][m],w[m],p),3) for p in (.25,.5,.75,.9,.99)])
# start grid purity/stability, per charge, both axes both frames
mxr=T["t_mx"]; mxg=T["t_gmx"]
for fr in ("A","B"):
  for ax,edg,rc,gc in (("xF",XF_EDGES,"t_xf"+fr,"t_xf1"),("pt2",PT2_EDGES,"t_pt2"+fr,"t_pt21")):
    for c in (1,-1):
        m=T["t_charge"]==c
        r=pur_stab(mxr[m],T[rc][m],mxg[m],T[gc][m],gv[m],w[m],MX_EDGES,[edg]*3)
        nb=len(edg)-1
        print("\nSTART GRID frame %s axis %s charge %+d  (rows: M_X bins; cols: obs bins)"%(fr,ax,c))
        for k in ("nreco","purity","stab","fake"):
            print(" w",k.ljust(7)," | ".join(str(np.round(r["w"][k][i*nb:(i+1)*nb],2 if k!="nreco" else 0)) for i in range(3)))
        print(" raw nreco ", " | ".join(str(r["raw"]["nreco"][i*nb:(i+1)*nb].astype(int)) for i in range(3)))
        print(" raw nsame ", " | ".join(str(r["raw"]["nsame"][i*nb:(i+1)*nb].astype(int)) for i in range(3)))
        # n_sig_pred = gv reco tracks
        r2=pur_stab(mxr[m&gv],T[rc][m&gv],mxg[m&gv],T[gc][m&gv],gv[m&gv],w[m&gv],MX_EDGES,[edg]*3)
        print(" w n_sig_pred (gv reco tracks)", " | ".join(str(np.round(r2["w"]["nreco"][i*nb:(i+1)*nb],0)) for i in range(3)))
