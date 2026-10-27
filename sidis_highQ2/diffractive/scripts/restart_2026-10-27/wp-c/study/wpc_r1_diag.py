import sys, numpy as np, uproot
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
T,E=load(); w=T["t_w"]; gv=T["t_gv"]
print("=== F3: frame-B p_T*^2 bias (reco-variant minus gen pt2_G2) on gen-valid tracks, weighted mean / core sigma")
gen=T["t_pt22"]
for lo,hi in zip(MX_EDGES[:-1],MX_EDGES[1:]):
    m=gv&(T["t_gmx"]>=lo)&(T["t_gmx"]<hi)
    for nm in ("base","gphot","gtrk","gmass","allgen"):
        d=T["t_f3_"+nm][m]-gen[m]; s=wstats(d,w[m]); mlow=m&(gen<0.25); sl=wstats(T["t_f3_"+nm][mlow]-gen[mlow],w[mlow])
        print("MX[%d,%d) %-7s all: mean %+.4f med %+.4f core %.4f | gen pt2<0.25: mean %+.4f med %+.4f"%(lo,hi,nm,s["mean"],s["med"],s["core"],sl["mean"],sl["med"]))
print("check base == stored t_pt2B:",np.nanmax(np.abs(T["t_f3_base"]-T["t_pt2B"])))
print("\n=== F4: selected events outside gen fiducial, weighted (ruled exposure), by sample")
ew=E["e_w"]; fid=E["e_genfid"]; sm=E["e_samp"]
reas=[("mirXip>=0.03",E["e_mirXip"]>=0.03),("mirMX>40",E["e_mirMX"]>40),("mirMX<4 (incl no mirror)",E["e_mirMX"]<4),("mirMY>=1.6",E["e_mirMY"]>=1.6),("|mirT|>=1",np.abs(E["e_mirT"])>=1),("Q2true<150",E["e_Q2t"]<150),("ytrue out [0.1,0.7]",(E["e_yt"]<.1)|(E["e_yt"]>.7))]
print("%-6s %9s %9s %6s | "%("sample","sel_w","outfid_w","frac")+" ".join(r[0][:12].rjust(12) for r in reas))
for tag,msk in [(l,sm==i) for i,l in enumerate(LABELS)]+[("PDISS",np.isin(sm,[3,4,5])),("ALL",sm>=0)]:
    o=msk&~fid
    print("%-6s %9.0f %9.0f %6.3f | "%(tag,ew[msk].sum(),ew[o].sum(),ew[o].sum()/ew[msk].sum())+" ".join(("%12.0f"%ew[o&r[1]].sum()) for r in reas))
print("(reasons overlap; also exclusive first-failing order xip,MX,MY,t,Q2,y:)")
order=[0,1,2,3,4,5,6]
for tag,msk in [("PDISS",np.isin(sm,[3,4,5])),("ALL",sm>=0)]:
    rem=msk&~fid; out=[]
    for k in order:
        c=rem&reas[k][1]; out.append((reas[k][0],round(ew[c].sum()))); rem=rem&~reas[k][1]
    print(tag,out,"unassigned",round(ew[rem].sum()))
print("\n=== F5: weighted mean x_F (gen-valid tracks) [printed by wpc_r1_diag.py; earlier by wpc_tables.py last line]")
print("reco A %.4f  B %.4f  gen G1 %.4f  G2 %.4f"%tuple(np.average(T[k][gv&np.isfinite(T[k])],weights=w[gv&np.isfinite(T[k])]) for k in ("t_xfA","t_xfB","t_xf1","t_xf2")))
f=uproot.open("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/t2_detector/mc9012_merged_2026-10-18/9012.root")
C=f["ChargedTot"].arrays(["MatchTier","GenMatchIdx"],library="np")
print("9012 ChargedTot rows",len(C["MatchTier"]),"; GenMatchIdx>=0:",int((C["GenMatchIdx"]>=0).sum()),"; MatchTier counts",{int(k):int(v) for k,v in zip(*np.unique(C["MatchTier"],return_counts=True))})
