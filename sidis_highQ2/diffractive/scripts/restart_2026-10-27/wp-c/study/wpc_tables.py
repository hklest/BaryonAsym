import sys, json, csv, numpy as np
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
T,E=load(); w=T["t_w"]; gv=T["t_gv"]; mxr=T["t_mx"]; mxg=T["t_gmx"]; ch=T["t_charge"]; samp=T["t_samp"]
HDR=["charge","mx_bin","axis","bin","lo","hi","n_data","n_sig_pred","n_mc_raw","f_nd","f_reg","f_fake","purity","stability","masked"]
MXN=["4-10","10-20","20-40"]
def write(fn,frame,edges):
    rows=[]; extra=[]
    for ax,gc in (("xF","t_xf1"),("pt2","t_pt21")):
        rc="t_%s%s"%(ax if ax=="pt2" else "xf",frame) if ax=="pt2" else "t_xf"+frame
        rc="t_pt2"+frame if ax=="pt2" else "t_xf"+frame
        el=[np.array(e) for e in edges[ax]]
        for c in (1,-1):
            m=ch==c
            r=pur_stab(mxr[m],T[rc][m],mxg[m],T[gc][m],gv[m],w[m],MX_EDGES,el)
            g=m&gv
            r2=pur_stab(mxr[g],T[rc][g],mxg[g],T[gc][g],gv[g],w[g],MX_EDGES,el)
            idx=0
            for i in range(3):
                for j in range(len(el[i])-1):
                    rawsamp=pur_stab(mxr[g&(samp==0)],T[rc][g&(samp==0)],mxg[g&(samp==0)],T[gc][g&(samp==0)],gv[g&(samp==0)],w[g&(samp==0)],MX_EDGES,el)["raw"]["nreco"][idx] if False else None
                    rows.append(["h+" if c>0 else "h-",MXN[i],ax,j,el[i][j],el[i][j+1],"",round(r2["w"]["nreco"][idx],2),int(r2["raw"]["nreco"][idx]),"","",round(r["w"]["fake"][idx],4),round(r["w"]["purity"][idx],4),round(r["w"]["stab"][idx],4),""])
                    idx+=1
    with open(fn,"w",newline="") as f:
        cw=csv.writer(f); cw.writerow(HDR); cw.writerows(rows)
    return rows
start={"xF":[XF_EDGES.tolist()]*3,"pt2":[PT2_EDGES.tolist()]*3}
for fr in ("A","B"):
    write(BASE+"binstats_start_frame%s.csv"%fr,fr,start)
    p=json.load(open(BASE+"proposal_frame%s_pmin0.35.json"%fr))
    rows=write(BASE+"binstats_proposal_frame%s.csv"%fr,fr,p)
    nb=len(rows)//2
    print("frame",fr,"proposal bins per charge:",nb,"| min n_sig_pred %.0f | min raw %d | min purity %.3f | min stab %.3f | max fake %.2f"%(min(r[7] for r in rows),min(r[8] for r in rows),min(r[12] for r in rows),min(r[13] for r in rows),max(r[11] for r in rows)))
    print("  edges:",json.dumps({ax:[np.round(e,3).tolist() for e in p[ax]] for ax in p}))
# per-sample raw counts for frame B proposal (h+ and h-): raw gv reco tracks per sample per bin, aggregated min
p=json.load(open(BASE+"proposal_frameB_pmin0.35.json"))
print("\nraw gv reco tracks per sample in smallest proposal bins (frame B) [9012,9024,9033,9155,9157,9158]:")
for ax,rc,gc in (("xF","t_xfB","t_xf1"),("pt2","t_pt2B","t_pt21")):
    el=[np.array(e) for e in p[ax]]
    for i in range(3):
        for j in range(len(el[i])-1):
            m=gv&(ch>0)&(mxr>=MX_EDGES[i])&(mxr<MX_EDGES[i+1]+(1e-9 if i==2 else 0))&(T[rc]>=el[i][j])&(T[rc]<el[i][j+1])
            cnt=[int((m&(samp==s)).sum()) for s in range(6)]
            if m.sum()<400: print(" ",ax,MXN[i],"bin",j,"[%.2f,%.2f)"%(el[i][j],el[i][j+1]),cnt,"w frac elastic %.2f"%(w[m&(samp==0)].sum()/w[m].sum()))
# totals
for c in (1,-1):
    m=ch==c; print("charge",c,"selected reco tracks raw",m.sum(),"weighted",w[m].sum().round(0),"| signal (gen-valid) raw",(m&gv).sum(),"weighted",w[m&gv].sum().round(0),"| per sample raw gv",[int((m&gv&(samp==s)).sum()) for s in range(6)])
# x_F sign sanity: mean xF of reco tracks in frame A,B (gen proxy G1)
print("weighted mean xF (gv): reco A %.3f B %.3f gen G1 %.3f"%(np.average(T["t_xfA"][gv],weights=w[gv]),np.average(T["t_xfB"][gv],weights=w[gv]),np.average(T["t_xf1"][gv],weights=w[gv])))
