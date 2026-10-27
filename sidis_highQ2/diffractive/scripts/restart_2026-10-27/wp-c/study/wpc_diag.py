import sys, numpy as np
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
T,E=load()
np.set_printoptions(linewidth=200,precision=3,suppress=True)
print("selected events",len(E["e_w"]),"genfid",E["e_genfid"].sum(),"weighted sel evts",E["e_w"].sum())
for c in (1,-1):
    m=T["t_charge"]==c; print("charge",c,"tracks raw",m.sum(),"weighted",T["t_w"][m].sum(),"gv raw",(m&T["t_gv"]).sum(),"gv w",T["t_w"][m&T["t_gv"]].sum())
for i,l in enumerate(LABELS):
    m=(T["t_samp"]==i); print(l,"raw tr",m.sum(),"w",T["t_w"][m].sum().round(0),"gv frac",T["t_gv"][m].mean().round(3))
# fake composition
why=T["t_why"]; w=T["t_w"]
print("fake reasons (weighted frac of all selected tracks):")
for b,nm in ((1,"nonhadron/unmatched"),(2,"veto"),(4,"phase space"),(8,"notX"),(16,"event not gen-fid")): print(" ",nm,(w[(why&b)>0].sum()/w.sum()).round(3))
# event resolution
g=E["e_genfid"]; ew=E["e_w"]
def rel(a,b): return (a-b)/b
print("\nEVENT-LEVEL (sel & genfid), weighted; by gen M_X bin")
vars_={"MX(mx)":(E["e_mx"],E["e_mirMX"],1),"MX(sqrt phM2)":(np.sqrt(np.where(E["e_phM2"]>0,E["e_phM2"],np.nan)),E["e_mirMX"],1),
 "MX(mx2Ye)":(np.sqrt(np.where(E["e_mx2Ye"]>0,E["e_mx2Ye"],np.nan)),E["e_mirMX"],1),"xIP":(E["e_xIp"],E["e_mirXip"],1),"beta":(E["e_beta"],E["e_mirBeta"],1)}
for nm,(r,gg,rl) in vars_.items():
    for lo,hi in [(4,40),(4,10),(10,20),(20,40)]:
        m=g&(E["e_mirMX"]>=lo)&(E["e_mirMX"]<hi+(1e-9 if hi==40 else 0))
        s=wstats(rel(r[m],gg[m]),ew[m]); print("%-14s [%2d,%2d) n=%5d mean %+.3f rms %.3f iqr %.3f core %.3f"%(nm,lo,hi,s["n"],s["mean"],s["rms"],s["iqr"],s["core"]))
for nm,(r,gg) in {"beta abs":(E["e_beta"],E["e_mirBeta"]),"xIP abs":(E["e_xIp"],E["e_mirXip"])}.items():
    s=wstats((r-gg)[g],ew[g]); print(nm,"mean %+.4f rms %.4f iqr %.4f core %.4f"%(s["mean"],s["rms"],s["iqr"],s["core"]))
print("hatMXGen/hatXpom vs mir (gen-fid events): MX ratio",wstats((E["e_hatMXGen"]/E["e_mirMX"])[g]) ["med"],"mean",np.nanmean((E["e_hatMXGen"]/E["e_mirMX"])[g]),"frac |ratio-1|<1e-3",np.mean(np.abs(E["e_hatMXGen"]/E["e_mirMX"]-1)[g]<1e-3),
      " xip ratio frac<1e-3",np.mean(np.abs(E["e_hatXpom"]/E["e_mirXip"]-1)[g]<1e-3))
print("gen proxy mass: M1 vs mirMX (tracks gv):",wstats(((T["t_M1"]-T["t_gmx"])/T["t_gmx"])[T["t_gv"]],T["t_w"][T["t_gv"]]))
# track resolution
gv=T["t_gv"]; w=T["t_w"]
print("\nTRACK-LEVEL x_F (reco-gen1) per gen M_X bin, frames A,B")
for fr in ("A","B"):
    for lo,hi in [(4,10),(10,20),(20,40)]:
        m=gv&(T["t_gmx"]>=lo)&(T["t_gmx"]<hi)
        s=wstats(T["t_xf"+fr][m]-T["t_xf1"][m],w[m]); print(fr,"MX[%d,%d) n=%d xF: mean %+.3f rms %.3f iqr %.3f core %.3f"%(lo,hi,s["n"],s["mean"],s["rms"],s["iqr"],s["core"]))
print("x_F vs G2 proxy:")
for fr in ("A","B"):
    m=gv; s=wstats(T["t_xf"+fr][m]-T["t_xf2"][m],w[m]); print(fr,"all: mean %+.3f core %.3f iqr %.3f"%(s["mean"],s["core"],s["iqr"]))
s=wstats((T["t_xf1"]-T["t_xf2"])[gv],w[gv]); print("G1-G2 xF diff: mean %+.4f rms %.4f core %.4f"%(s["mean"],s["rms"],s["core"]))
s=wstats((T["t_pt21"]-T["t_pt22"])[gv],w[gv]); print("G1-G2 pt2 diff: mean %+.4f rms %.4f core %.4f"%(s["mean"],s["rms"],s["core"]))
print("x_F range gen (gv): min %.4f max %.4f frac outside [-1,1] G1 %.2e G2 %.2e"%(np.nanmin(T["t_xf1"][gv]),np.nanmax(T["t_xf1"][gv]),np.mean(np.abs(T["t_xf1"][gv])>1+1e-9),np.mean(np.abs(T["t_xf2"][gv])>1+1e-9)))
print("x_F range reco (all selected tracks): frac |xF|>1  A %.4f  B %.4f ; nan A %.4f B %.4f"%(np.average(np.abs(T["t_xfA"])>1,weights=w),np.average(np.abs(T["t_xfB"])>1,weights=w),np.mean(~np.isfinite(T["t_xfA"])),np.mean(~np.isfinite(T["t_xfB"]))))
print("   raw (unweighted) frac >1: A %.4f B %.4f"%(np.mean(np.abs(T["t_xfA"])>1),np.mean(np.abs(T["t_xfB"])>1)))
print("\nTRACK-LEVEL pt2 relative (reco-gen)/gen in gen pt2 bins and abs")
for fr in ("A","B"):
  for lo,hi in [(4,10),(10,20),(20,40)]:
    for a,b in zip(PT2_EDGES[:-1],PT2_EDGES[1:]):
        m=gv&(T["t_gmx"]>=lo)&(T["t_gmx"]<hi)&(T["t_pt21"]>=a)&(T["t_pt21"]<b)
        s=wstats(T["t_pt2"+fr][m]-T["t_pt21"][m],w[m]); r=wstats((T["t_pt2"+fr][m]-T["t_pt21"][m])/T["t_pt21"][m],w[m])
        print(fr,"MX[%2d,%2d) pt2[%.2f,%.2f) n=%5d abs mean %+.3f core %.3f | rel mean %+.2f core %.2f"%(lo,hi,a,b,s["n"],s["mean"],s["core"],r["mean"],r["core"]))
